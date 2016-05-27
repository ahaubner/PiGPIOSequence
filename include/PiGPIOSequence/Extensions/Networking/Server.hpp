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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVER_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVER_HPP

#include <functional>

#include <boost/asio.hpp>

#include <PiGPIOSequence/Sequence.hpp>
#include <PiGPIOSequence/Extensions/Async.hpp>
#include <PiGPIOSequence/Extensions/Networking/IDGen.hpp>
#include <PiGPIOSequence/Extensions/Networking/ServerSpace/Connection.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Server
			{
				public:
					static constexpr auto Port = 2719;

                    Server(bool debug_mode = false);

					void run();
					void prepare_RunOne();
					void run_one();


					boost::asio::io_service & getIOService();

                    template <typename Archive>
                    void deserializeSequence(Archive & a)
                    {
                        wait(m_Handle, [&a, this]()
                        {
                            a(m_Sequence);
                        });
                    }

					bool scheduleSequenceRun();
						void cancelScheduledSequence();
					bool maySequenceBeRunning() const;
					bool isSequenceRunning() const;

                    bool runSequence(const std::function<void ()> & callback = std::function<void()>());
					bool runParallelSequences(std::function<void()> callback = std::function<void()>());

                    bool stopSequence();

					Message::Header_t::Content_e getServerState() const;

					void connectionFinished(ServerSpace::Connection * ptr);

					void registerPlugin(ServerPlugin::Pointer plugin);
					void unregisterPlugin(ServerPlugin::Pointer plugin);
					ServerPlugin::id_type getPluginID(std::string const & stringID) const; //invalid_id when not registered
					ServerPlugin::Pointer getPluginByID(ServerPlugin::id_type id);

				private:
					using ConnectionContainer = std::vector<ServerSpace::Connection *>;

                    using PluginIDGen = IDGen<ServerPlugin::id_type>;
					using PluginVector = std::vector<ServerPlugin::Pointer>;

					void log(const char * msg);

					void startAccepting();
					void handleAccept(ServerSpace::Connection::Pointer Connection, boost::system::error_code const & error);

                    void handleSequenceRun(std::function<void ()> callback);

					boost::asio::io_service m_IOService;
					boost::asio::ip::tcp::endpoint m_Endpoint;
					boost::asio::ip::tcp::acceptor m_Acceptor;

                    Sequence m_Sequence;
                    AsyncHandle m_Handle;

					ConnectionContainer  m_Connections;

                    PluginIDGen m_PluginIDGen;
					PluginVector m_Plugins;

					bool m_SRunScheduled {false};
					bool m_DebugMode;
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVER_HPP
