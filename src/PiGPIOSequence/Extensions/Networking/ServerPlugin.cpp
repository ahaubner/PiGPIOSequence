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

#include <PiGPIOSequence/Extensions/Networking/ServerPlugin.hpp>

#include <PiGPIOSequence/Extensions/Networking/PluginMessage.hpp>
#include <PiGPIOSequence/Extensions/Networking/ServerSpace/Connection.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			static_assert(std::is_same<ServerPlugin::id_type, PluginMessage::Header_t::size_type>::value, "Plugin id type is not compatible with PluginMessage format");
			static_assert(ServerPlugin::invalid_id == PluginMessage::Header_t::invalid_size, "Plugin id invalid state is not compatible with PluginMessage format");
			static_assert(std::is_same<ServerPlugin::ConnectionPtr, ServerSpace::Connection::Pointer>::value, "Connection pointer type is not the same");

			ServerPlugin::~ServerPlugin()
			{}

			void ServerPlugin::setID(ServerPlugin::id_type id)
			{
				m_ID = id;
			}

			ServerPlugin::id_type ServerPlugin::id()
			{
				return m_ID;
			}


		}
	}
}
