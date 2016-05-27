/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceNetworking library.
 *
 *   PiGPIOSequenceNetworking is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceNetworking is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceNetworking.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <PiGPIOSequence/Extensions/Networking/Client.hpp>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>

#include <PiGPIOSequence/Extensions/Networking/Server.hpp>
#include <PiGPIOSequence/Extensions/Networking/Utils.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

using namespace boost::asio::ip;
using namespace boost::interprocess;
using namespace std;

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{

			Client::Error::Error(Client::Error::Code error_code)
				:	m_Code(error_code)
			{}

			Client::Error::Code Client::Error::getCode() const
			{
				return m_Code;
			}

			Client::Error::operator bool() const
			{
                                return (m_Code != Code::None);
			}

			Client::Client(bool debug_mode)
				:	m_Socket(m_IOService),
					m_Resolver(m_IOService),
					m_DebugMode(debug_mode)
			{
				m_IDGen.generateID(); //Erste ist für spezielle Zwecke vorgesehen
			}

			void Client::run()
			{
				log("Running IOService");

				m_IOService.run();
			}

            void Client::run_one()
            {
                m_IOService.poll_one();
            }

            void Client::poll()
            {
                m_IOService.poll();
            }

            void Client::poll_one()
            {
				m_IOService.poll_one();
			}

			void Client::reset()
			{
				m_IOService.reset();
//				m_Socket.cancel();
//				m_Socket.close();
				m_Writing = false;
				m_Reading = false;
				m_Connected = false;
			}

			void Client::dispatchCallback(const std::function<void ()> & F)
			{
				m_IOService.dispatch(F);
			}

			void Client::postCallback(const std::function<void ()> & F)
			{
				m_IOService.post(F);
			}

			void Client::connect(const string & host, const Client::Callback_t & callback)
			{
				log("Trying to resolve address...");

				tcp::resolver::query Query(host, to_string(Server::Port));

                m_Resolver.async_resolve(Query, bind(&Client::handleResolve, this, placeholders::_1, placeholders::_2, callback));
            }

            void Client::disconnect(const Client::Callback_t &callback)
            {
                m_IOService.post([this, callback]()
                {
                    log("Disconnect requested!");

                    if (!m_Socket.is_open())
                    {
                        if (callback)
                        {
                            callback({Error::Code::NotConnected});
                        }

                        return;
                    }

					m_Socket.cancel();
					m_Socket.close();
					if (m_Socket.is_open())
					{
						//Sinnvoll?
						m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
					}

                    m_IOService.stop();
                    m_IOService.reset();

                    if (callback)
                    {
                        callback(Error());
                    }
                });
            }

            void Client::isConnected(const Client::Callback_t &callback)
            {
                m_IOService.dispatch([this, callback]()
                {
                    if (m_Socket.is_open())
                    {
                        callback(Error());
                    }

                    else
                    {
                        callback({Error::Code::NotConnected});
                    }
                });
            }

			void Client::sendSequence(const Sequence & sequence, const Client::Callback_t & callback)
			{
				if (!m_Socket.is_open() ||
					!m_Connected)
				{
                    throw Exception(Error::Code::NotConnected);
				}

				basic_vectorstream<vector<ostream::char_type>> VStream;
                { //Flushing archive by destructor
                    cereal::JSONOutputArchive oarchive(VStream);

                    oarchive(sequence);
                }

                VStream.swap_vector(m_WriteMessage.BodyData());

				auto Handle = m_IDGen.generateID();

                m_WriteMessage.Header().setContent(Message::Header_t::Content_e::Client_GPIOSequence);
                m_WriteMessage.Header().BodySize = static_cast<Message::Header_t::size_type>(m_WriteMessage.BodyData().size());
                m_WriteMessage.Header().ID = Handle;

				log("Sending sequence...");

				writeMessage([this, callback, Handle](const boost::system::error_code & error, size_t)
				{
					if (!error)
					{
						registerMessageHandler(Handle, [this, callback](MsgIDType id, MessageReadState state)
						{
							switch (state)
							{
								case MessageReadState::Fine:
                                    if (m_ReadMessage.Header().Content() == Message::Header_t::Content_e::Server_Accept)
									{
										log("Sent sequence successfully!");

										if (callback)
										{
											callback(Error());
										}
									}

                                    else if (m_ReadMessage.Header().Content() == Message::Header_t::Content_e::Server_Deny)
									{
										if (callback)
										{
											callback({Error::Code::SequenceAlreadyRunning});
										}
									}
								break;

								default:
									if (callback)
									{
										callback({Error::Code::SendingSequenceFailed});
									}
								break;
							}

							unregisterMessageHandler(id);
						});
					} else {
						if (callback)
						{
							callback({Error::Code::SendingSequenceFailed});
						}
					}
				},

				[callback](const boost::system::error_code &)
				{
					if (callback)
					{
						callback({Error::Code::SendingSequenceFailed});
					}
				});
			}

            void Client::stopSequence(Callback_t const & callback)
            {
                log("Requesting server to stop running sequence..");

                m_WriteMessage.Header().setContent(Message::Header_t::Content_e::Client_StopSequence);
                m_WriteMessage.Header().ID = m_IDGen.generateID();
                m_WriteMessage.Header().BodySize = 0;

                registerMessageHandler(m_WriteMessage.Header().ID, [this, callback](MsgIDType id, MessageReadState state)
                {
                    if (!callback)
                    {
                        unregisterMessageHandler(id);
                        return;
                    }

                    if (state == MessageReadState::Fine)
                    {
                        if (m_ReadMessage.Header().Content() == Message::Header_t::Content_e::Server_Accept)
                        {
                            log("Server answered: Sequence stopped!");

                            callback(Error());
                        }

                        else
                        {
                            log("Server answered: Sequence not stopped. Wasn running");

                            callback({Error::Code::SequenceNotRunning});
                        }
                    }

                    else
                    {
                        callback({Error::Code::ReceivingAnswerFailed});
                    }

                    unregisterMessageHandler(id);
                });

                writeMessage([callback](const boost::system::error_code & error, size_t)
                {
                    if (error)
                    {
                        if (callback)
                        {
                            callback({Error::Code::SendingFailed});
                        }
                    }
                },

                [callback](const boost::system::error_code &)
                {
                    if (callback)
                    {
                        callback({Error::Code::SendingFailed});
                    }
                });
            }

            void Client::requestStatus(const Client::StatusCallback_t &callback)
            {
                log("Requesting server's status...");

                m_WriteMessage.Header().setContent(Message::Header_t::Content_e::Client_GetStatus);
                m_WriteMessage.Header().ID = m_IDGen.generateID();
                m_WriteMessage.Header().BodySize = 0;

                registerMessageHandler(m_WriteMessage.Header().ID, [this, callback](MsgIDType id, MessageReadState state)
                {
                    if (!callback)
                    {
                        unregisterMessageHandler(id);
                        return;
                    }

                    if (state == MessageReadState::Fine)
                    {
                        log("Server answered: Got status: " + to_string(static_cast<int>(state)));

                        switch (m_ReadMessage.Header().Content())
                        {
                            case Message::Header_t::Content_e::Server_State_Normal:
                                callback(ServerStatus::Normal, Error());
                            break;

                            case Message::Header_t::Content_e::Server_State_RunningSequence:
                                callback(ServerStatus::RunningSequence, Error());
                            break;

                            default:
                                callback(ServerStatus::Unknown, Error());
                            break;
                        }
                    }

                    else
                    {
                        callback(ServerStatus::Unknown, {Error::Code::ReceivingAnswerFailed});
                    }

                    unregisterMessageHandler(id);
                });

                writeMessage([callback](const boost::system::error_code & error, size_t)
                {
                    if (error)
                    {
                        if (callback)
                        {
                            callback(ServerStatus::Unknown, {Error::Code::SendingFailed});
                        }
                    }
                },

                [callback](const boost::system::error_code &)
                {
                    if (callback)
                    {
                        callback(ServerStatus::Unknown, {Error::Code::SendingFailed});
                    }
                });
            }

            void Client::onSequenceFinished(const std::function<void ()> & callback)
            {
                m_OnSequenceFinished = callback;
            }

            void Client::onConnectionClosed(const std::function<void ()> & callback)
            {
				m_OnConnectionClosed = callback;
			}

			void Client::registerPlugin(ClientPlugin::Pointer plugin, Callback_t const & callback)
			{
				//
				if (!m_Socket.is_open() ||
					!m_Connected)
				{
                    throw Exception(Error::Code::NotConnected);
				}

				Utils::OArchiveWrapper<boost::archive::text_oarchive> Archive;

				Archive.get() << plugin->StringID();

				Archive.saveToVector(m_WriteMessage.BodyData());

				auto Handle = m_IDGen.generateID();

				m_WriteMessage.Header().setContent(Message::Header_t::Content_e::Client_PluginRequest);
                m_WriteMessage.Header().BodySize = static_cast<Message::Header_t::size_type>(m_WriteMessage.BodyData().size());
				m_WriteMessage.Header().ID = Handle;

				log("Sending plugin id request...");

				writeMessage([this, callback, Handle, plugin](const boost::system::error_code & error, size_t)
				{
					if (!error)
					{
						registerMessageHandler(Handle, [this, callback, plugin](MsgIDType id, MessageReadState state)
						{
							switch (state)
							{
								case MessageReadState::Fine:
									if (m_ReadMessage.Header().Content() == Message::Header_t::Content_e::Server_PluginSupported)
									{
										if (m_ReadMessage.Header().BodySize == 0)
										{
											if (callback)
											{
												callback({Error::Code::PluginRequestFailed});
											}
										}

										Utils::IArchiveWrapper<boost::archive::text_iarchive> Archive (m_ReadMessage.Body());

										auto id = ClientPlugin::invalid_id;

										Archive.get() >> id;

										if (id != ClientPlugin::invalid_id)
										{
											unregisterPlugin(plugin);

											plugin->setID(id);
											plugin->setClientParent(this);
											m_Plugins.push_back(plugin);

											log("Registered plugin successfully!");

											if (callback)
											{
												callback({});
											}
										}

										else
										{
											if (callback)
											{
												callback({Error::Code::PluginRequestFailed});
											}
										}
									}

									else if (m_ReadMessage.Header().Content() == Message::Header_t::Content_e::Server_PluginNotSupported)
									{
										if (callback)
										{
											callback({Error::Code::PluginNotSupported});
										}
									}
								break;

								default:
								break;
							}

							if (callback)
							{
								callback({Error::Code::PluginRequestFailed});
							}

							unregisterMessageHandler(id);
						});
					} else {
						if (callback)
						{
							callback({Error::Code::PluginRequestFailed});
						}
					}
				},

				[callback](const boost::system::error_code &)
				{
					if (callback)
					{
						callback({Error::Code::PluginRequestFailed});
					}
				});
			}

			void Client::unregisterPlugin(ClientPlugin::Pointer plugin)
			{
				for (auto i = m_Plugins.begin(); i != m_Plugins.end(); ++i)
				{
					if (i->get() == plugin.get())
					{
						plugin->setID(ClientPlugin::invalid_id);
						plugin->setClientParent(nullptr);
						m_Plugins.erase(i);
						return;
					}
				}
			}

			void Client::sendMessage(Message && msg, const Callback_t & callback)
			{
				m_WriteMessage = std::move(msg);
				m_WriteMessage.Header().ID = Message::Header_t::first_id;

				writeMessage([callback](const boost::system::error_code & error, size_t)
				{
					if (error)
					{
						if (callback)
						{
							callback({Error::Code::SendingFailed});
						}
					}

					else
					{
						if (callback)
						{
							callback({});
						}
					}
				},

				[callback](const boost::system::error_code & error)
				{
					if (error)
					{
						if (callback)
						{
							callback({Error::Code::SendingFailed});
						}
					}

					else
					{
						if (callback)
						{
							callback({});
						}
					}
				});
			}

			void Client::writeMessage(const std::function<void (const boost::system::error_code &, size_t)> & callback,
									  const std::function<void (const boost::system::error_code &)> & HeaderError)
			{
				if (m_Writing)
				{
					log("Tried to write twice at the same socket. aborting. throwing exception!");

					throw DoubleWrite();
				}

				m_Writing = true;
                m_WriteMessage.encode_header();

				log("Sending message...");

                boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage.HeaderData(), Message::HeaderSize),
										 [this, callback, HeaderError](boost::system::error_code const & error, size_t)
				{
					if (!error)
					{
                        m_WriteMessage.encode_body();
                        boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage.BodyData(), m_WriteMessage.Header().BodySize),
												 [this, callback](boost::system::error_code const & error_, size_t size_)
						{
							m_Writing = false;
							if (callback)
							{
								callback(error_, size_);
							}
						});
					} else {
						m_Writing = false;

						log("Failed to send header.");

						if (HeaderError)
						{
							HeaderError(error);
						}
					}
				});
			}

			void Client::readMessage(const std::function<void (const boost::system::error_code &, size_t)> & callback,
									 const std::function<void (const boost::system::error_code &)> & HeaderError)
			{
				if (m_Reading)
				{
					log("Tried to read twice from the same socket. aborting. throwing exception!");

					throw DoubleRead();
				}

				m_Reading = true;

				log("Reading message asynchronously...");

                boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.HeaderData(), Message::HeaderSize),
										 [this, callback, HeaderError](boost::system::error_code const & error, size_t)
				{
					if (!error)
					{
                        m_ReadMessage.decode_header();
                        boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.BodyData(), m_ReadMessage.Header().BodySize),
												 [this, callback](boost::system::error_code const & error_, size_t size_)
						{
							m_Reading = false;
                            m_ReadMessage.decode_body();

							if (callback)
							{
								callback(error_, size_);
							}
						});
					} else {
						m_Reading = false;

						log("Failed to read header.");

						if (HeaderError)
						{
							HeaderError(error);
						}
					}
				});
			}

			void Client::registerMessageHandler(Client::MsgIDType id, Client::MessageHandler handler)
			{
				m_MessageHandlers[id] = handler;
			}

			void Client::unregisterMessageHandler(Client::MsgIDType id)
			{
				auto find = m_MessageHandlers.find(id);

				if (find != m_MessageHandlers.end())
				{
					m_MessageHandlers.erase(find);
					m_IDGen.releaseID(id);
				}
			}

			void Client::handleConnect(const boost::system::error_code & error, const Callback_t & callback, boost::asio::ip::tcp::resolver::iterator endpointIterator)
			{
				if (!error)
				{
					log("Connected!");

					m_Connected = true;

					readMessage(bind(&Client::handleRead, this, placeholders::_1, placeholders::_2, false), bind(&Client::handleRead, this, placeholders::_1, 0, true));

					if (callback)
					{
						callback(Error());
					}
				}

				else if(endpointIterator != tcp::resolver::iterator())
				{
					log("Failed to connect. Retrying with next endpoint...");

					m_Socket.close();

                    tcp::endpoint endpoint = *endpointIterator;
					log((std::string("Connecting to ") + endpoint.address().to_string() + ":" + to_string(endpoint.port())).c_str());
					m_Socket.async_connect(endpoint, bind(&Client::handleConnect, this, placeholders::_1, callback, ++endpointIterator));
				}

				else
				{
					log("Failed to connect!");

					if (callback)
					{
						callback({Error::Code::ConnectFailed});
					}
				}
			}

			void Client::handleResolve(const boost::system::error_code & error, boost::asio::ip::tcp::resolver::iterator iterator, const Callback_t & callback)
			{
				if (!error)
				{
					tcp::endpoint endpoint = *iterator;

					log((std::string("Address resolved. Connecting to ") + endpoint.address().to_string() + ":" + to_string(endpoint.port())).c_str());

					m_Socket.async_connect(endpoint, bind(&Client::handleConnect, this, placeholders::_1, callback, ++iterator));
				} else {
					log("Failed to resolve address.");

					if (callback)
					{
						callback({Error::Code::HostResolveFailed});
					}
				}
			}

			void Client::handleRead(const boost::system::error_code & error, size_t, bool HeaderFailed)
			{
                auto Handler = m_MessageHandlers.find(m_ReadMessage.Header().ID);

				if (Handler != m_MessageHandlers.end())
                {
                    log("Invoking registered handler for " + to_string(m_ReadMessage.Header().ID));

					if (HeaderFailed)
					{
                        Handler->second(m_ReadMessage.Header().ID, MessageReadState::HeaderError);
					}

					else
					{
						if (error)
						{
                            Handler->second(m_ReadMessage.Header().ID, MessageReadState::BodyError);
						}

						else
						{
                            Handler->second(m_ReadMessage.Header().ID, MessageReadState::Fine);
						}
					}
                }

                else
                {
                    log("No handler for id registered: " + to_string(m_ReadMessage.Header().ID));
                    log("Assuming common message");

                    if (HeaderFailed)
                    {
                        handleCommonMessage(m_ReadMessage.Header().ID, MessageReadState::HeaderError);
                    }

                    else
                    {
                        if (error)
                        {
                            handleCommonMessage(m_ReadMessage.Header().ID, MessageReadState::BodyError);
                        }

                        else
                        {
                            handleCommonMessage(m_ReadMessage.Header().ID, MessageReadState::Fine);
                        }
                    }
                }

                if (error)
                {
                    if( error == boost::asio::error::eof ||
                        error == boost::asio::error::connection_aborted ||
                        error == boost::asio::error::broken_pipe ||
                        error == boost::asio::error::connection_refused ||
                        error == boost::asio::error::interrupted ||
                        error == boost::asio::error::network_down ||
                        error == boost::asio::error::shut_down ||
                        error == boost::asio::error::timed_out
                        )
                    {
                        cout << "Connection closed. Stopping reading." << endl;

                        if (m_OnConnectionClosed)
                        {
                            m_OnConnectionClosed();
                        }

                        return;
                    }

                    ++m_Fails;

                    log("Error while reading message: " + error.message());

                    if (m_Fails > 10)
                    {
                        log ("Too many read errors. aborting.");

                        if (m_OnConnectionClosed)
                        {
                            m_OnConnectionClosed();
                        }

                        return;
                    }
                }

                else
                {
                    m_Fails = 0;
                }

                readMessage(bind(&Client::handleRead, this, placeholders::_1, placeholders::_2, false), bind(&Client::handleRead, this, placeholders::_1, 0, true));
            }

			void Client::handleCommonMessage(Client::MsgIDType, Client::MessageReadState state)
            {
                if (state == MessageReadState::Fine)
                {
                    switch(m_ReadMessage.Header().Content())
                    {
                        case Message::Header_t::Content_e::Server_SequenceFinished:
                            log("Server sends info: Sequence finished.");

                            if (m_OnSequenceFinished)
                            {
                                m_OnSequenceFinished();
                            }
                        break;

                        default:
                            log("Could not handle message content: " + to_string(m_ReadMessage.Header().ID));
                        break;
                    }
                }
            }

            void Client::log(const string &msg)
			{
				if (m_DebugMode)
				{
                    cout << "PiGPIOClient DEBUG: " << msg << endl;
				}
			}

			Client::DoubleWrite::DoubleWrite(const string & msg)
				:	runtime_error("Tried to write twice to the same socket. " + msg)
			{}

			Client::DoubleRead::DoubleRead(const string & msg)
				:	runtime_error("Tried to read twice from the same socket. " + msg)
			{}

			Client::Exception::Exception(Client::Error::Code error_code, const string & msg)
				:	Error(error_code),
					runtime_error("Client exception. " + msg)
			{}

		}
	}
}
