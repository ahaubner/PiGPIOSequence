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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENT_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENT_HPP

#include <functional>

#include <boost/asio.hpp>

#include <PiGPIOSequence.hpp>

#include <PiGPIOSequence/Extensions/Networking/IDGen.hpp>
#include <PiGPIOSequence/Extensions/Networking/Message.hpp>
#include <PiGPIOSequence/Extensions/Networking/ClientPlugin.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Client
			{
				public:
					class Error
					{
						public:
							enum class Code
							{
								None,
								Undefined,
								NotConnected,
								HostResolveFailed,
								ConnectFailed,
								SendingSequenceFailed,
                                SequenceAlreadyRunning,
                                SequenceNotRunning,
                                SendingFailed,
								ReceivingAnswerFailed,
								PluginRequestFailed,
								PluginNotSupported
							};

							Error(Code error_code = Code::None);

							Code getCode() const;

							operator bool() const;

						private:
							Code m_Code;
					};

                    enum class ServerStatus
                    {
                        Normal,
                        RunningSequence,
                        Unknown
                    };

					class Exception
							:	public Error,
								public std::runtime_error
					{
						public:
							Exception(Error::Code error_code = Error::Code::None, std::string const & msg = std::string());
					};

                    using Callback_t =  std::function<void (Error const &)>;
                    using StatusCallback_t = std::function<void (ServerStatus, Error const &)>;

					Client(bool debug_mode = false);

					void run();
                    void run_one();
                    void poll();
                    void poll_one();
					void reset();

					void dispatchCallback(std::function<void ()> const & F);
					void postCallback(std::function<void()> const & F);

					void connect(std::string const & host, Callback_t const & callback);
                    void disconnect(Callback_t const & callback = Callback_t());
                        void isConnected(Callback_t const & callback);

					void sendSequence(Sequence const & sequence, Callback_t const & callback = Callback_t());

                    void stopSequence(Callback_t const & callback = Callback_t());
                    void requestStatus(StatusCallback_t const & callback = StatusCallback_t());

                    void onSequenceFinished(std::function<void()> const & callback);
                    void onConnectionClosed(std::function<void()> const & callback);

					void registerPlugin(ClientPlugin::Pointer plugin, Callback_t const & callback = Callback_t());
					void unregisterPlugin(ClientPlugin::Pointer plugin);

					void sendMessage(Message && msg, Callback_t const & callback = Callback_t());

				private:
					class DoubleWrite
							:	public std::runtime_error
					{
						public:
							DoubleWrite(std::string const & msg = std::string());
					};

					class DoubleRead
							:	public std::runtime_error
					{
						public:
							DoubleRead(std::string const & msg = std::string());
					};

					enum class MessageReadState
					{
						Fine,
						HeaderError,
						BodyError
					};

                    using MsgIDType = decltype(Message::Header_t::ID);

					using MessageHandler = std::function<void (MsgIDType, MessageReadState)>;
					using MessageHandlerMap = std::map<MsgIDType, MessageHandler>;

					using PluginVector = std::vector<ClientPlugin::Pointer>;

                    using IDGenerator = IDGen<MsgIDType>;

					void writeMessage(const std::function<void (const boost::system::error_code &, size_t)> & callback,
									  const std::function<void (const boost::system::error_code &)> & HeaderError = std::function<void (const boost::system::error_code &)>());

					void readMessage(	const std::function<void (const boost::system::error_code &, size_t)> & callback,
										const std::function<void (const boost::system::error_code &)> & HeaderError = std::function<void (const boost::system::error_code &)>());

					void registerMessageHandler(MsgIDType id, MessageHandler handler);
					void unregisterMessageHandler(MsgIDType id);

					void handleConnect(const boost::system::error_code & error, Callback_t const & callback, boost::asio::ip::tcp::resolver::iterator endpointIterator);
					void handleResolve(const boost::system::error_code & error, boost::asio::ip::tcp::resolver::iterator iterator, Callback_t const & callback);
					void handleRead(const boost::system::error_code & error, size_t, bool HeaderFailed = false);

					void handleCommonMessage(MsgIDType /*id*/, MessageReadState state);

                    void log(std::string const & msg);

					boost::asio::io_service m_IOService;
					boost::asio::ip::tcp::socket m_Socket;
					boost::asio::ip::tcp::resolver m_Resolver;

                    Message m_ReadMessage;
                    Message m_WriteMessage;

					MessageHandlerMap m_MessageHandlers;

					PluginVector m_Plugins;

                    std::function<void ()> m_OnSequenceFinished;
                    std::function<void ()> m_OnConnectionClosed;

					unsigned int m_Fails {0};

					bool m_Writing {false};
					bool m_Reading {false};
					bool m_Connected {false};

                    IDGenerator m_IDGen;

					bool m_DebugMode;
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENT_HPP
