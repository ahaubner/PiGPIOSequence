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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_PLUGINMESSAGE_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_PLUGINMESSAGE_HPP

#include <cstdint>
#include <vector>
#include <limits>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Message;

			class PluginMessage
			{
				public:
					class Header_t
					{
						public:
							using size_type = std::uint32_t;
							static constexpr auto invalid_size = std::numeric_limits<size_type>::max();

							Header_t();
							Header_t(size_type pid, size_type id, size_type content, size_type bsize);
							Header_t(Header_t const & source);
							Header_t(Header_t && source);

							Header_t & operator=(Header_t const & source);
							Header_t & operator=(Header_t && source);

							size_type PluginID {invalid_size};
							size_type ContentData {invalid_size};
							size_type BodySize {0};
							size_type ID {invalid_size};
					};

					using byte = std::uint8_t;
					using Body_t = std::vector<char>;

					static_assert(sizeof(byte) == sizeof(char), "char size is not equal the size of a byte");

					static constexpr auto HeaderSize = sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize) + sizeof(Header_t::ID);

					PluginMessage();
					PluginMessage(Header_t && headerinit);
					PluginMessage(PluginMessage const & source);
					PluginMessage(PluginMessage && msg);

					PluginMessage & operator=(PluginMessage const & source);
					PluginMessage & operator=(PluginMessage && source);

					Header_t & Header();
					Body_t & Body();

					void storeIntoMessage(Message & msg);
					void restoreFromMessage(Message & msg);

				private:
					Header_t m_Header;
					Body_t m_Body;
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_PLUGINMESSAGE_HPP
