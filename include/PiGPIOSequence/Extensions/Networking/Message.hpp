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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_MESSAGE_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_MESSAGE_HPP

#include <cstdint>
#include <stdexcept>
#include <array>
#include <vector>
#include <limits>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			class Message
			{
				public:
					class Header_t
					{
						public:
							using size_type = std::uint32_t;

							static auto constexpr invalid_size = std::numeric_limits<size_type>::max();
							static auto constexpr invalid_id = invalid_size;
							static auto constexpr first_id = std::numeric_limits<size_type>::min();

							enum class Content_e : size_type
							{
								None = 0,

                                Client_GPIOSequence = 1,
                                Client_StopSequence = 2,
                                Client_GetStatus = 3,

                                Server_Accept = 4,
                                Server_Deny = 5,
                                Server_SequenceFinished = 6,

                                Server_State_RunningSequence = 7,
                                Server_State_Normal = 8,

                                Client_PluginRequest = 9,
                                Server_PluginSupported = 10,
                                Server_PluginNotSupported = 11,
                                PluginMsg = 12
                            };

							Header_t();
                            Header_t(size_type id, Content_e content, size_type bsize);
							Header_t(Header_t const & source);
							Header_t(Header_t && source);

							Header_t & operator=(Header_t const & source);
							Header_t & operator=(Header_t && source);

							size_type ContentData {static_cast<size_type>(Content_e::None)};
							size_type BodySize {0};
							size_type ID {first_id};

							Content_e Content() const;
							void setContent(Content_e content);
					};

					using byte = std::uint8_t;
					using Body_t = std::vector<char>;

					static_assert(sizeof(byte) == sizeof(char), "char size is not equal the size of a byte");

					class DecodeError
							:	public std::runtime_error
					{
						public:
							enum class Error_e
							{
								Undefined,
								WrongBodySize
							};

							DecodeError(Error_e error = Error_e::Undefined, std::string const & msg = std::string("Message decoding error"));
							Error_e Error() const;

						private:
							Error_e m_Error;
					};

					static constexpr auto HeaderSize = sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize) + sizeof(Header_t::ID);

					using HeaderStorage = std::array<byte, HeaderSize>;
					using BodyStorage = std::vector<char>;

					Message();
                    Message(decltype(Header_t::ID) id, Header_t::Content_e content, decltype(Header_t::BodySize) bsize);
					Message(Message const & source);
					Message(Message && msg);

					Message & operator=(Message const & source);
					Message & operator=(Message && source);

					Header_t & Header();
					Body_t & Body();

					HeaderStorage & HeaderData();
					BodyStorage & BodyData();

					void decode_header();
					void decode_body();

					void encode_header();
					void encode_body();

				private:
					Header_t m_Header;
					Body_t m_Body;

					HeaderStorage m_HeaderData;
					BodyStorage m_BodyData;
			};
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_MESSAGE_HPP
