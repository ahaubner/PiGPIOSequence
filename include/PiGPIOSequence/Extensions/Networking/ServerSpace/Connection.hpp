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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERSPACE_CONNECTION_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERSPACE_CONNECTION_HPP

#include <functional>
#include <stdexcept>
#include <istream>
#include <memory>
#include <deque>
#include <tuple>

#include <boost/asio.hpp>

#include <PiGPIOSequence/Extensions/Networking/IDGen.hpp>
#include <PiGPIOSequence/Extensions/Networking/Message.hpp>
#include <PiGPIOSequence/Extensions/Networking/ServerPlugin.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Server;

			namespace ServerSpace
			{
				class Connection
						:	public std::enable_shared_from_this<Connection>
				{
					public:
						using Pointer = std::shared_ptr<Connection>;

						static Pointer create(Server & server, bool debug_mode = false);

						~Connection();

						void startCommunication();

						boost::asio::ip::tcp::socket & Socket();

					private:
						friend class Networking::Server;

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
							BodyError
						};

						using MessageWrite_Callback = std::function<void (const boost::system::error_code &, size_t)>;
						using MessageWrite_HeaderFailCallback = std::function<void (const boost::system::error_code &)>;
						using MessageWrite_Context = std::tuple<Message, MessageWrite_Callback, MessageWrite_HeaderFailCallback>;
						using MessageWrite_Queue = std::deque<MessageWrite_Context>;

						using MessageRead_EraseCallback = std::function<void ()>;
						using MessageRead_Context = std::tuple<Message, boost::system::error_code, MessageRead_EraseCallback>; //Message, ReadError
						using MessageRead_Queue = std::deque<MessageRead_Context>;

						using MsgIDType = decltype(Message::Header_t::ID);

						using MessageHandler = std::function<void (MsgIDType, MessageReadState, MessageRead_Context &)>;
                        using MessageHandlerMap = std::map<MsgIDType, MessageHandler>;
                        using IDGenerator = IDGen<MsgIDType>;

						Connection(Server & server, bool debug_mode = false);

						void registerMessageHandler(MsgIDType id, MessageHandler handler);
						void unregisterMessageHandler(MsgIDType id);

						void handleRead(MessageRead_Context & context);

						void handleCommonMessage(MessageRead_Context & context);

						void writeMessage(Message && msg,
										  const MessageWrite_Callback & callback = MessageWrite_Callback(),
										  const MessageWrite_HeaderFailCallback & HeaderError = MessageWrite_HeaderFailCallback());

							void writeMessageLoop();

						void readMessageLoop();



						void log(const char * msg);

						Server & m_Server;
							//Server signals:
                            void signal_SequenceFinished();

						boost::asio::ip::tcp::socket m_Socket;
						bool m_Writing;
						bool m_Reading;

						std::vector<std::istream::char_type> m_SequenceBuffer;

						MessageWrite_Queue m_MessageWriteBuffer;
						MessageRead_Queue m_MessageReadBuffer;

						MessageHandlerMap m_MessageHandlers;
                        IDGenerator m_IDGen;

						unsigned int m_Fails;
						bool m_DebugMode;
				};
			}
		}
	}
}
			


#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERSPACE_CONNECTION_HPP
