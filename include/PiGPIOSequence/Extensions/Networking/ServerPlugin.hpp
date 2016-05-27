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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERPLUGIN_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERPLUGIN_HPP

#include <string>
#include <cstdint>
#include <limits>
#include <memory>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			namespace ServerSpace
			{
				class Connection;
			}

			class Server;
			class PluginMessage;

			class ServerPlugin
			{
				public:
					using id_type = std::uint32_t;
					static constexpr auto invalid_id = std::numeric_limits<id_type>::max();

					using Pointer = std::shared_ptr<ServerPlugin>;

					using ConnectionPtr = std::shared_ptr<ServerSpace::Connection>;

					virtual ~ServerPlugin();

					virtual std::string const & StringID() const = 0;

					virtual void handleMessage(PluginMessage &&, ConnectionPtr) = 0;

				private:
					friend class Server;
					friend class ServerSpace::Connection;

					void setID(id_type id);
					id_type id();

					id_type m_ID {invalid_id};
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_SERVERPLUGIN_HPP
