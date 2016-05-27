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

#include <PiGPIOSequence/Extensions/Networking/ServerSpace/Connection.hpp>

#include <functional>
#include <iostream>
#include <utility>

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <PiGPIOSequence/Extensions/Networking/Server.hpp>
#include <PiGPIOSequence/Extensions/Networking/Utils.hpp>
#include <PiGPIOSequence/Extensions/Networking/PluginMessage.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

using namespace boost::asio::ip;
using namespace boost::interprocess;
using namespace std;

template <typename T>
class moveOnCopy
{
	private:
		mutable T m_Object;

		moveOnCopy & operator=(moveOnCopy const & source) = delete;
		moveOnCopy & operator=(moveOnCopy && source) = delete;

	public:
		moveOnCopy(T && object)
			:	m_Object(move(object))
		{}

		moveOnCopy(moveOnCopy const & source)
			:	m_Object(move(source.m_Object))
        {}

		T & value()
		{
			return m_Object;
		}
};

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			static_assert(sizeof(std::istream::char_type) == sizeof(char), "streams use unsupported char type");

			namespace ServerSpace
			{
				Connection::Pointer Connection::create(Server & server, bool debug_mode)
				{
					return Pointer(new Connection(server, debug_mode));
				}

				Connection::~Connection()
				{
					log("Connection died.");
                    m_Server.connectionFinished(this);
				}

				void Connection::startCommunication()
				{
					if (m_Socket.is_open())
					{
						log("Waiting asynchronously for messages...");

						readMessageLoop();
                        writeMessageLoop();
					} else {
                        log("Socket closed. Stopping communication");
					}
				}

				tcp::socket &Connection::Socket()
				{
					return m_Socket;
				}

				Connection::Connection(Server & server, bool debug_mode)
					:	m_Server(server),
						m_Socket(server.getIOService()),
						m_Writing(false),
						m_Reading(false),
						m_Fails(0),
						m_DebugMode(debug_mode)
				{
					m_IDGen.generateID(); //Erste ID ist reserviert
				}

				void Connection::registerMessageHandler(Connection::MsgIDType id, Connection::MessageHandler handler)
				{
					m_MessageHandlers[id] = handler;
				}

				void Connection::unregisterMessageHandler(Connection::MsgIDType id)
				{
					auto find = m_MessageHandlers.find(id);

					if (find != m_MessageHandlers.end())
					{
						m_MessageHandlers.erase(find);
						m_IDGen.releaseID(id);
					}
				}

				void Connection::handleRead(MessageRead_Context & context)
				{
					Message & msg = get<0>(context);
					boost::system::error_code & error = get<1>(context);

					auto Handler = m_MessageHandlers.find(msg.Header().ID);

					if (Handler != m_MessageHandlers.end())
					{
						if (error)
						{
							Handler->second(msg.Header().ID, MessageReadState::BodyError, context);
						}

						else
						{
							Handler->second(msg.Header().ID, MessageReadState::Fine, context);
						}
					}

					else
					{
						handleCommonMessage(context);
					}
				}

				void Connection::handleCommonMessage(MessageRead_Context & context)
				{
					log("Handling message...");

					auto self = shared_from_this();

					Message & msg = get<0>(context);
					auto id = msg.Header().ID;
					auto & EraseContext = get<2>(context);

					switch (msg.Header().Content())
					{
						case Message::Header_t::Content_e::Client_GPIOSequence:
							log("Client sent sequence to run");

							if (m_Server.maySequenceBeRunning())
							{
                                log("Already running sequence -> denying request");

                                writeMessage({
                                                 id,
                                                 Message::Header_t::Content_e::Server_Deny,
                                                 0
                                             });

								EraseContext();
							}

							else
							{
								log("Requesting server to schedule sequence run");

								m_Server.scheduleSequenceRun();

                                log("Sending confirmation...");

                                writeMessage({
                                                 id,
                                                 Message::Header_t::Content_e::Server_Accept,
                                                 0
                                             },

								[this, self, &msg, &EraseContext](boost::system::error_code const & error, size_t)
                                {
                                    if (!error)
                                    {
                                        log("Confirmation sent, starting sequence asynchronously.");

                                        m_Server.getIOService().post([this, self, &msg, &EraseContext]()
                                        {
                                            bufferstream input(msg.Body().data(), msg.Body().size());
                                            cereal::JSONInputArchive iarchive(input);

                                            m_Server.deserializeSequence(iarchive);
                                            m_Server.runSequence();

                                            //WARNING Was wenn s Laden nicht klappt?

                                            EraseContext();
                                        });
                                    } else {
                                        log("Sending confirmation failed");
                                        EraseContext();
                                    }
                                });
							}
						break;

                        case Message::Header_t::Content_e::Client_StopSequence:
                            log ("Client requests server to stop sequence");

                            if (m_Server.stopSequence())
                            {
                                writeMessage({
                                                 id,
                                                 Message::Header_t::Content_e::Server_Accept,
                                                 0
                                             },

                                [self, &EraseContext](const boost::system::error_code &, size_t)
                                {
                                    EraseContext();
                                },

                                [self, &EraseContext](const boost::system::error_code &)
                                {
                                    EraseContext();
                                });
                            }

                            else
                            {
                                writeMessage({
                                                 id,
                                                 Message::Header_t::Content_e::Server_Deny,
                                                 0
                                             },

                                [self, &EraseContext](const boost::system::error_code &, size_t)
                                {
                                    EraseContext();
                                },

                                [self, &EraseContext](const boost::system::error_code &)
                                {
                                    EraseContext();
                                });
                            }
                        break;

                        case Message::Header_t::Content_e::Client_GetStatus:
                            log ("Client requests server's status");

                            writeMessage(   {
                                                id,
                                                m_Server.getServerState(),
                                                0
                                            },

                            [this, self, &EraseContext](const boost::system::error_code & error, size_t)
                            {
                                if (error)
                                {
                                    log("Sending status failed.");
                                }

                                else
                                {
                                    log("Sent status!");
                                }

                                EraseContext();
                            },

                            [this, self, &EraseContext](const boost::system::error_code &)
                            {
                                log("Sending status failed. (HeaderError)");

                                EraseContext();
                            });
                        break;

						case Message::Header_t::Content_e::Client_PluginRequest:
							{
								log("Client asks for plugin support");

								Utils::Text_IArchiveWrapper Archive(msg.Body());

								std::string pStrID;
								Archive.get() >> pStrID;

								auto pid = m_Server.getPluginID(pStrID);

								if (pid == ServerPlugin::invalid_id)
								{
									log((std::string("Plugin not supported: ") + pStrID).c_str());

									writeMessage({
													 id,
													 Message::Header_t::Content_e::Server_PluginNotSupported,
													 0
												 });

									EraseContext();
								}

								else
								{
									log((std::string("Plugin supported: ") + pStrID).c_str());

									log("Sending confirmation...");

									Message RespMsg
									{
										id,
										Message::Header_t::Content_e::Server_PluginSupported,
										0
									};

									{
										Utils::Text_OArchiveWrapper Archive;

										Archive.get() << pid;
										Archive.saveToVector(RespMsg.BodyData());
									}

                                    RespMsg.Header().BodySize = static_cast<Message::Header_t::size_type>(RespMsg.BodyData().size());

									writeMessage(std::move(RespMsg),

									[this, self, &msg, &EraseContext](boost::system::error_code const & error, size_t)
									{
										if (!error)
										{
											log("Plugin support confirmation sent.");
										} else {
											log("Sending plugin support confirmation failed");
										}

										EraseContext();
									});
								}
							}
						break;

						case Message::Header_t::Content_e::PluginMsg:
							{
								log("Client sent PluginMsg");

								try
								{
									PluginMessage pMsg;

									pMsg.restoreFromMessage(msg);

									auto plugin = m_Server.getPluginByID(pMsg.Header().PluginID);

									if (plugin)
									{
										plugin->handleMessage(std::move(pMsg), self);
									}

									else
									{
										log("No plugin registered for this message.");
									}
								}

                                catch(std::exception const &)
								{
									log("Restoring plugin message failed.");
								}

								EraseContext();
							}
						break;

						default:
							log("Message meaning unknown");
							log(("Content ID: " + to_string(msg.Header().ContentData)).c_str());
							log("Body preview:");
							log((msg.Header().BodySize < 100) ?
									string(msg.Body().data(), msg.Header().BodySize).c_str() :
									string(msg.Body().data(), 100).c_str());

							EraseContext();
						break;
					}
				}

				void Connection::writeMessage(Message && msg,
											  const MessageWrite_Callback & callback,
											  const MessageWrite_HeaderFailCallback & HeaderError)
				{
                    m_MessageWriteBuffer.emplace_back
							(
                                forward<Message>(msg),
                                callback,
                                HeaderError
							);

					if (!m_Writing)
					{
						writeMessageLoop();
                    }
				}

				void Connection::writeMessageLoop()
				{
					if (m_Writing)
					{
						log("Tried to write twice at the same socket. aborting, but not throwing");
                        //FIXME, Vermute m_Writing/m_Reading sind trotz ioService nicht thread-safe

						return;
						//throw DoubleWrite();
					}

					else if(m_MessageWriteBuffer.empty())
					{
						return;
					}

                    m_Writing = true;

					auto self = shared_from_this();

					MessageWrite_Context & context = m_MessageWriteBuffer.front();
					Message & msg = get<0>(context);
					MessageWrite_Callback & callback = get<1>(context);
					MessageWrite_HeaderFailCallback & HeaderError = get<2>(context);

					msg.encode_header();

					log("Sending message...");

					boost::asio::async_write(m_Socket, boost::asio::buffer(msg.HeaderData(), Message::HeaderSize),
											 [this, self, &msg, &callback, &HeaderError](boost::system::error_code const & error, size_t)
					{
						if (!error)
						{
							msg.encode_body();
							boost::asio::async_write(m_Socket, boost::asio::buffer(msg.BodyData(), msg.Header().BodySize),
													 [this, self, &callback](boost::system::error_code const & error_, size_t size_)
							{
                                log("Message sent!");

                                if (callback)
								{
									callback(error_, size_);
								}

                                m_MessageWriteBuffer.pop_front();

                                m_Writing = false;

                                if (!m_MessageWriteBuffer.empty())
                                {
                                    writeMessageLoop();
                                }
							});
						} else {
							log("Failed to send header.");

							if (HeaderError)
							{
								HeaderError(error);
							}

                            m_MessageWriteBuffer.pop_front();

                            m_Writing = false;

                            if (!m_MessageWriteBuffer.empty())
                            {
                                writeMessageLoop();
                            }
                        }
					});
				}

				void Connection::readMessageLoop()
				{
					if (m_Reading)
					{
						log("Tried to read twice from the same socket. aborting, but not throwing exception!");

						return;
						//throw DoubleRead();
					}

                    m_Reading = true;

                    auto self = shared_from_this();

					m_MessageReadBuffer.push_back(make_tuple(Message(), boost::system::error_code(), MessageRead_EraseCallback()));

						auto i = prev(m_MessageReadBuffer.end());
							Message & msg = std::get<0>(*i);
							boost::system::error_code & error = std::get<1>(*i);
                            get<2>(*i) = [this, self, i]()
							{
								m_MessageReadBuffer.erase(i);
							};

					log("Reading message asynchronously...");

					boost::asio::async_read(m_Socket, boost::asio::buffer(msg.HeaderData(), Message::HeaderSize),
											 [this, self, &msg, &error, i](boost::system::error_code const & error_, size_t /*size_*/)
					{
						error = error_;

						if (!error_)
						{
							m_Fails = 0;
							log("Got message header -> decoding it, waiting for body");

							msg.decode_header();
							boost::asio::async_read(m_Socket, boost::asio::buffer(msg.BodyData(), msg.Header().BodySize),
													 [this, self, &msg, &error, i](boost::system::error_code const & error__, size_t /*size__*/)
							{
								m_Reading = false;
								error = error__;

								log("Successfully read message, passing asynchronously to handler");

								m_Server.getIOService().post([this, self, &msg, i]()
								{
									msg.decode_body();
									handleRead(*i);
								});

								log("Restarting read loop...");
								readMessageLoop();
							});
						}

						else if(error_ == boost::asio::error::eof ||
								error_ == boost::asio::error::connection_aborted ||
								error_ == boost::asio::error::broken_pipe ||
								error_ == boost::asio::error::connection_refused ||
								error_ == boost::asio::error::interrupted ||
								error_ == boost::asio::error::network_down ||
								error_ == boost::asio::error::shut_down ||
								error_ == boost::asio::error::timed_out
								)
						{
							log("Connection closed. Stopping");
							m_Socket.close();
							m_MessageReadBuffer.erase(i);
							m_Reading = false;
							//m_Server.connectionFinished(this);
						}

						else if (m_Fails >= 10)
						{
							log("Failed to read header.");

							log("Too many connection fails. Stopping");
							m_Socket.close();
							m_MessageReadBuffer.erase(i);
							m_Reading = false;
							//m_Server.connectionFinished(this);
						}

                        else
						{
                            ++m_Fails;

							log("Receiving message failed. Error:");
							log((error_.message() + " Code: " + to_string(error_.value())).c_str());
							log("Retrying...");

							m_Reading = false;
							m_MessageReadBuffer.erase(i);

							readMessageLoop();
						}
					});
				}

				void Connection::log(const char * msg)
				{
					if (m_DebugMode)
					{
                        cout << "PiGPIOServer - Connection DEBUG: " << msg << endl;
					}
				}

				void Connection::signal_SequenceFinished()
				{
                    //Server tells connection that running sequence finished
                    Message msg;
                    msg.Header().setContent(Message::Header_t::Content_e::Server_SequenceFinished);
					msg.Header().ID = IDGenerator::firstID;
                    msg.Header().BodySize = 0;

					writeMessage(move(msg));
				}

				Connection::DoubleWrite::DoubleWrite(const string & msg)
					:	runtime_error("Tried to write twice to the same socket. " + msg)
				{}

				Connection::DoubleRead::DoubleRead(const string & msg)
					:	runtime_error("Tried to read twice from the same socket. " + msg)
				{}

			}
		}
	}
}
