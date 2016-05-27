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

#include <PiGPIOSequence/Extensions/Networking/Server.hpp>

#include <functional>
#include <iostream>

using namespace boost::asio::ip;
using namespace std;

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			using namespace ServerSpace;

            Server::Server(bool debug_mode)
				:	m_Endpoint(tcp::v4(), Port),
                    m_Acceptor(m_IOService, m_Endpoint),
					m_DebugMode(debug_mode)
			{}

			void Server::prepare_RunOne()
			{
				startAccepting();
			}

			void Server::run_one()
			{
				m_IOService.poll_one();
			}

			void Server::run()
			{
				startAccepting();

				log("Running IOService");
				m_IOService.run();
			}

            boost::asio::io_service & Server::getIOService()
			{
				return m_IOService;
			}

			bool Server::scheduleSequenceRun()
			{
				if (maySequenceBeRunning())
				{
					log("Sequence scheduling denied due to already scheduled sequence");
					return false;
				}

				log("Sequence scheduled");
				m_SRunScheduled = true;
				return true;
			}

			void Server::cancelScheduledSequence()
			{
				if (isSequenceRunning())
				{
					log("Canceling scheduled sequence denied due to already running sequence");

					return;
				}

				m_SRunScheduled = false;
			}

			bool Server::maySequenceBeRunning() const
			{
				return m_SRunScheduled;
			}

			bool Server::isSequenceRunning() const
			{
                return is_running(m_Handle);
			}

            bool Server::runSequence(std::function<void ()> const & callback)
			{
				if (!isSequenceRunning())
				{
					m_SRunScheduled = true;

					log("Running sequence asynchronously...");
                    m_Handle = run_async(m_Sequence, bind(&Server::handleSequenceRun, this, callback));

					return true;
				}

				log("Running sequence denied due to already running sequence");
				return false;
			}

            bool Server::stopSequence()
            {
                if (maySequenceBeRunning())
                {
                    log("Stopping sequence.");

                    stop(m_Handle);

                    return true;
                }

                log("Tried to stop sequence. Isn't running -> failed");

                return false;
            }

            Message::Header_t::Content_e Server::getServerState() const
            {
                return (maySequenceBeRunning() ?
                            Message::Header_t::Content_e::Server_State_RunningSequence :
                            Message::Header_t::Content_e::Server_State_Normal);
            }

			void Server::connectionFinished(Connection * ptr)
			{
                for (ConnectionContainer::size_type i = 0; i < m_Connections.size(); ++i)
				{
					if (m_Connections[i] == ptr)
					{
                        m_Connections.erase(m_Connections.begin() + i);
						return;
					}
				}
			}

			void Server::registerPlugin(ServerPlugin::Pointer plugin)
			{
				unregisterPlugin(plugin);
				plugin->setID(m_PluginIDGen.generateID());
				m_Plugins.push_back(plugin);
			}

			void Server::unregisterPlugin(ServerPlugin::Pointer plugin)
			{
				for (auto i = m_Plugins.begin(); i != m_Plugins.end(); ++i)
				{
					if (i->get() == plugin.get())
					{
						m_PluginIDGen.releaseID(plugin->id());
						m_Plugins.erase(i);
						return;
					}
				}
			}

			ServerPlugin::id_type Server::getPluginID(const string & stringID) const
			{
				for (PluginVector::size_type i = 0; i < m_Plugins.size(); ++i)
				{
					if (m_Plugins[i]->StringID() == stringID)
					{
						return m_Plugins[i]->id();
					}
				}

				return ServerPlugin::invalid_id;
			}

			ServerPlugin::Pointer Server::getPluginByID(ServerPlugin::id_type id)
			{
				for (PluginVector::size_type i = 0; i < m_Plugins.size(); ++i)
				{
					if (m_Plugins[i]->id() == id)
					{
						return m_Plugins[i];
					}
				}

				return {};
			}

			void Server::log(const char * msg)
			{
				if (m_DebugMode)
				{
                    cout << "PiGPIOServer DEBUG: " << msg << endl;
				}
			}

			void Server::startAccepting()
			{
				auto NewConnection = Connection::create(*this, m_DebugMode);

				log("Waiting asynchronously for connection");

				m_Acceptor.async_accept(NewConnection->Socket(),
										bind(&Server::handleAccept, this, NewConnection, placeholders::_1));

			}

			void Server::handleAccept(Connection::Pointer connection, const boost::system::error_code & error)
            {
				if (!error)
				{
					log("Accepted connection -> starting communication");
                    m_Connections.emplace_back(connection.get());
					connection->startCommunication();

				} else {
					log((std::string("Accepting connection failed. Error: ") + error.message() + "Code: " + to_string(error.value())).c_str());
					//delete connection;
				}

				startAccepting();
			}

			void Server::handleSequenceRun(std::function<void ()> callback)
			{
				m_IOService.post([this, callback]()
				{
					log("Sequence finished!");
					if (callback)
					{
						callback();
					}

                    //Telling every connection
                    for (auto i : m_Connections)
                    {
                        i->signal_SequenceFinished();
                    }

					m_SRunScheduled = false;
				});
            }
		}
	}
}
