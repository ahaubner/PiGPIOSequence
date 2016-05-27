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

#include <PiGPIOSequence/Extensions/Networking/ClientPlugin.hpp>

#include <utility>
#include <stdexcept>

#include <PiGPIOSequence/Extensions/Networking/ServerPlugin.hpp>
#include <PiGPIOSequence/Extensions/Networking/Client.hpp>
#include <PiGPIOSequence/Extensions/Networking/PluginMessage.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			static_assert(std::is_same<ClientPlugin::id_type, ServerPlugin::id_type>::value, "Plugin id type is not the same as ServerPlugin id type");
			static_assert(ClientPlugin::invalid_id == ServerPlugin::invalid_id, "Plugin id invalid state is not compatible with ServerPlugin format");

			ClientPlugin::~ClientPlugin()
			{

			}

			bool ClientPlugin::hasClientParent() const
			{
				return (m_ClientPtr != nullptr);
			}

			Client &ClientPlugin::getClientParent()
			{
				if (m_ClientPtr)
				{
					return *m_ClientPtr;
				}

				throw std::runtime_error("Plugin is not activated. It has no Client object as parent.");
			}

			void ClientPlugin::sendPluginMessage(PluginMessage && pMsg)
			{
				Message Msg;
					pMsg.Header().PluginID = getID();
					pMsg.storeIntoMessage(Msg);

				getClientParent().sendMessage(std::move(Msg));
			}

			void ClientPlugin::setID(ClientPlugin::id_type id)
			{
				m_ID = id;
			}

			ClientPlugin::id_type ClientPlugin::getID() const
			{
				return m_ID;
			}

			void ClientPlugin::setClientParent(PiGPIOSequence::Extensions::Networking::Client * ptr)
			{
				m_ClientPtr = ptr;
			}


		}
	}
}
