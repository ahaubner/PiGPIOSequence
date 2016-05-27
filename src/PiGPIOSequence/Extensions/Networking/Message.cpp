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

#include <PiGPIOSequence/Extensions/Networking/Message.hpp>

#include <utility>

#include <boost/asio.hpp>

#include <PiGPIOSequence/Extensions/Networking/IDGen.hpp>

using namespace std;

using namespace boost::asio::detail::socket_ops;

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
            static_assert(sizeof(host_to_network_long(Message::Header_t::size_type(0))) == sizeof(Message::Header_t::size_type), "Byteorder conversion function does not match size requirements");
            static_assert(sizeof(network_to_host_long(Message::Header_t::size_type(0))) == sizeof(Message::Header_t::size_type), "Byteorder conversion function does not match size requirements");
            static_assert(Message::Header_t::first_id == IDGen<Message::Header_t::size_type>::firstID, "IDGen first ID does not match assumption");

			Message::Message()
            {}

            Message::Message(decltype(Header_t::ID) id, Message::Header_t::Content_e content, decltype(Header_t::BodySize) bsize)
                :   m_Header(id, content, bsize)
            {}

			Message::Message(const Message & source)
			{
				operator =(source);
			}

			Message::Message(Message && msg)
			{
				operator =(forward<Message>(msg));
			}

			Message & Message::operator=(const Message & source)
			{
				m_Header = source.m_Header;
				m_Body = source.m_Body;
				m_HeaderData = source.m_HeaderData;
				m_BodyData = source.m_BodyData;

				return *this;
			}

			Message & Message::operator=(Message && source)
			{
				m_Header = move(source.m_Header);
				m_Body = move(source.m_Body);
				m_HeaderData = move(source.m_HeaderData);
				m_BodyData = move(source.m_BodyData);

				return *this;
			}

			Message::Header_t & Message::Header()
			{
				return m_Header;
			}

			Message::Body_t & Message::Body()
			{
				return m_Body;
			}

			Message::HeaderStorage &Message::HeaderData()
			{
				return m_HeaderData;
			}

			Message::BodyStorage &Message::BodyData()
			{
				return m_BodyData;
			}

			void Message::decode_header()
			{
                std::copy(std::begin(m_HeaderData), std::begin(m_HeaderData) + sizeof(Header_t::ContentData), reinterpret_cast<byte *>(&(m_Header.ContentData)));
				std::copy(std::begin(m_HeaderData) + sizeof(Header_t::ContentData), std::begin(m_HeaderData) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize), reinterpret_cast<byte *>(&(m_Header.BodySize)));
				std::copy(std::begin(m_HeaderData) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize), std::begin(m_HeaderData) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize) + sizeof(Header_t::ID), reinterpret_cast<byte *>(&(m_Header.ID)));

                m_Header.ContentData = network_to_host_long(m_Header.ContentData);
                m_Header.BodySize = network_to_host_long(m_Header.BodySize);
                m_Header.ID = network_to_host_long(m_Header.ID);

				m_BodyData.resize(m_Header.BodySize);
			}

			void Message::decode_body()
			{
				if (m_BodyData.size() != m_Header.BodySize)
				{
					throw DecodeError(DecodeError::Error_e::WrongBodySize);
				}

				m_Body.swap(m_BodyData);
			}

			void Message::encode_header()
			{
                auto ContentData_NB = host_to_network_long(m_Header.ContentData);
                auto BodySize_NB = host_to_network_long(m_Header.BodySize);
                auto ID_NB = host_to_network_long(m_Header.ID);

                byte * ContentData = reinterpret_cast<byte *>(&(ContentData_NB));
                byte * BodySize = reinterpret_cast<byte *>(&(BodySize_NB));
                byte * ID = reinterpret_cast<byte *>(&(ID_NB));

                std::copy(ContentData, ContentData + sizeof(ContentData_NB), std::begin(m_HeaderData));
                std::copy(BodySize, BodySize + sizeof(BodySize_NB), std::begin(m_HeaderData) + sizeof(ContentData_NB));
                std::copy(ID, ID + sizeof(ID_NB), std::begin(m_HeaderData) + sizeof(ContentData_NB) + sizeof(BodySize_NB));

				m_BodyData.resize(m_Header.BodySize);
			}

			void Message::encode_body()
			{
				m_BodyData.resize(m_Header.BodySize);
			}

			Message::DecodeError::DecodeError(Message::DecodeError::Error_e error, const std::string & msg)
			:	runtime_error(msg),
				m_Error(error)
			{}

			Message::DecodeError::Error_e Message::DecodeError::Error() const
			{
				return m_Error;
			}

			Message::Header_t::Header_t()
			{}

            Message::Header_t::Header_t(Message::Header_t::size_type id, Message::Header_t::Content_e content, Message::Header_t::size_type bsize)
                :   BodySize(bsize),
                    ID(id)
            {
                setContent(content);
            }

			Message::Header_t::Header_t(const Message::Header_t & source)
			{
				operator =(source);
			}

			Message::Header_t::Header_t(Message::Header_t && source)
			{
				operator =(forward<Header_t>(source));
			}

			Message::Header_t & Message::Header_t::operator=(const Message::Header_t & source)
			{
				ContentData = source.ContentData;
				BodySize = source.BodySize;
				ID = source.ID;

				return *this;
			}

			Message::Header_t & Message::Header_t::operator=(Message::Header_t && source)
			{
				ContentData = move(source.ContentData);
				BodySize = move(source.BodySize);
				ID = move(source.ID);

				return *this;
			}

			Message::Header_t::Content_e Message::Header_t::Content() const
			{
				return static_cast<Content_e>(ContentData);
			}

			void Message::Header_t::setContent(Message::Header_t::Content_e content)
			{
				ContentData = static_cast<size_type>(content);
			}

		}
	}
}
