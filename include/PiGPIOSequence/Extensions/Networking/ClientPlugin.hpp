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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENTPLUGIN_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENTPLUGIN_HPP

#include <string>
#include <memory>
#include <limits>
#include <cstdint>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Client;
			class PluginMessage;

			class ClientPlugin
			{
				public:
					using id_type = std::uint32_t;
					static constexpr auto invalid_id = std::numeric_limits<id_type>::max();

					using Pointer = std::shared_ptr<ClientPlugin>;

					virtual ~ClientPlugin();

					virtual std::string const & StringID() const = 0;

					virtual void handleMessage(PluginMessage &&, Client &) = 0;

				protected:
					bool hasClientParent() const;
					Client & getClientParent();

					void sendPluginMessage(PluginMessage && pMsg);

				private:
					friend class Client;

					void setID(id_type id);
					id_type getID() const;

					void setClientParent(Client * ptr);

					id_type m_ID {invalid_id};
					Client * m_ClientPtr {nullptr};
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_CLIENTPLUGIN_HPP
