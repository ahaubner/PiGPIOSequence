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

#include <PiGPIOSequence/Extensions/Networking/PluginMessage.hpp>

#include <iterator>
#include <algorithm>

#include <boost/asio.hpp>

#include <PiGPIOSequence/Extensions/Networking/Message.hpp>

using namespace boost::asio::detail::socket_ops;

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			static_assert(sizeof(host_to_network_long(Message::Header_t::size_type(0))) == sizeof(PluginMessage::Header_t::size_type), "Byteorder conversion function does not match size requirements");
			static_assert(sizeof(network_to_host_long(Message::Header_t::size_type(0))) == sizeof(PluginMessage::Header_t::size_type), "Byteorder conversion function does not match size requirements");

			PluginMessage::PluginMessage()
			{}

			PluginMessage::PluginMessage(PluginMessage::Header_t && headerinit)
				:	m_Header(std::move(headerinit))
			{}

			PluginMessage::PluginMessage(const PluginMessage & source)
				:	m_Header(source.m_Header),
					m_Body(source.m_Body)
			{}

			PluginMessage::PluginMessage(PluginMessage && msg)
				:	m_Header(std::move(msg.m_Header)),
					m_Body(std::move(msg.m_Body))
			{}

			PluginMessage & PluginMessage::operator=(const PluginMessage & source)
			{
				m_Header = source.m_Header;
				m_Body = source.m_Body;

				return *this;
			}

			PluginMessage & PluginMessage::operator=(PluginMessage && source)
			{
				m_Header = std::move(source.m_Header);
				m_Body = std::move(source.m_Body);

				return *this;
			}

			PluginMessage::Header_t & PluginMessage::Header()
			{
				return m_Header;
			}

			PluginMessage::Body_t & PluginMessage::Body()
			{
				return m_Body;
			}

			void PluginMessage::storeIntoMessage(Message & msg)
			{
				auto & Storage = msg.BodyData();

				Storage.resize(HeaderSize);

				//Storing Header
				auto PluginID_NB = host_to_network_long(m_Header.PluginID);
				auto ContentData_NB = host_to_network_long(m_Header.ContentData);
				auto BodySize_NB = host_to_network_long(m_Header.BodySize);
				auto ID_NB = host_to_network_long(m_Header.ID);

				byte * PluginID = reinterpret_cast<byte *>(&(PluginID_NB));
				byte * ContentData = reinterpret_cast<byte *>(&(ContentData_NB));
				byte * BodySize = reinterpret_cast<byte *>(&(BodySize_NB));
				byte * ID = reinterpret_cast<byte *>(&(ID_NB));

				auto HeaderBegin = std::begin(Storage);

				std::copy(PluginID,		PluginID + sizeof(PluginID_NB),			HeaderBegin);
				std::copy(ContentData,	ContentData + sizeof(ContentData_NB),	HeaderBegin + sizeof(PluginID_NB));
				std::copy(BodySize,		BodySize + sizeof(BodySize_NB),			HeaderBegin + sizeof(PluginID_NB) + sizeof(ContentData_NB));
				std::copy(ID,			ID + sizeof(ID_NB),						HeaderBegin + sizeof(PluginID_NB) + sizeof(ContentData_NB) + sizeof(BodySize_NB));

				//Storing Body
				Storage.reserve(Storage.size() + m_Body.size());
				std::move(std::begin(m_Body), std::end(m_Body), std::back_inserter(Storage));
				m_Body.clear();

				msg.Header().setContent(Message::Header_t::Content_e::PluginMsg);
				msg.Header().BodySize = Storage.size();
			}

			void PluginMessage::restoreFromMessage(Message & msg)
			{
				if (msg.Header().Content() != Message::Header_t::Content_e::PluginMsg)
				{
					throw std::runtime_error("Can't restore PluginMessage from Message due to invalid content flag");
				}

				else if (msg.Header().BodySize < HeaderSize)
				{
					throw std::runtime_error("Can't restore PluginMessage from Message, because of too little body size.");
				}

				//Restoring Header
				auto & Storage = msg.Body();
				auto HeaderBegin = std::begin(Storage);

				std::copy(HeaderBegin,																								HeaderBegin + sizeof(Header_t::PluginID),																						reinterpret_cast<byte *>(&(m_Header.PluginID)));
				std::copy(HeaderBegin + sizeof(Header_t::PluginID),																	HeaderBegin + sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData),														reinterpret_cast<byte *>(&(m_Header.ContentData)));
				std::copy(HeaderBegin + sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData),									HeaderBegin + sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize),							reinterpret_cast<byte *>(&(m_Header.BodySize)));
				std::copy(HeaderBegin + sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize),	HeaderBegin + sizeof(Header_t::PluginID) + sizeof(Header_t::ContentData) + sizeof(Header_t::BodySize) + sizeof(Header_t::ID),	reinterpret_cast<byte *>(&(m_Header.ID)));

				m_Header.PluginID = network_to_host_long(m_Header.PluginID);
				m_Header.ContentData = network_to_host_long(m_Header.ContentData);
				m_Header.BodySize = network_to_host_long(m_Header.BodySize);
				m_Header.ID = network_to_host_long(m_Header.ID);

				//Restoring Body
				if (msg.Body().size() < HeaderSize + m_Header.BodySize)
				{
					throw std::runtime_error("Message body is too small to contain the full PluginMessage");
				}

				auto BodyBegin = HeaderBegin + HeaderSize;
				m_Body.clear();

				std::move(BodyBegin, BodyBegin + m_Header.BodySize, std::back_inserter(m_Body));
				Storage.clear();
			}

			PluginMessage::Header_t::Header_t()
			{}

			PluginMessage::Header_t::Header_t(PluginMessage::Header_t::size_type pid, PluginMessage::Header_t::size_type id, PluginMessage::Header_t::size_type content, PluginMessage::Header_t::size_type bsize)
				:	PluginID(pid),
					ContentData(content),
					BodySize(bsize),
					ID(id)
			{}

			PluginMessage::Header_t::Header_t(const PluginMessage::Header_t & source)
				:	PluginID(source.PluginID),
					ContentData(source.ContentData),
					BodySize(source.BodySize),
					ID(source.ID)
			{}

			PluginMessage::Header_t::Header_t(PluginMessage::Header_t && source)
				:	PluginID(std::move(source.PluginID)),
					ContentData(std::move(source.ContentData)),
					BodySize(std::move(source.BodySize)),
					ID(std::move(source.ID))
			{}

			PluginMessage::Header_t & PluginMessage::Header_t::operator=(const PluginMessage::Header_t & source)
			{
				PluginID = source.PluginID;
				ContentData = source.ContentData;
				BodySize = source.BodySize;
				ID = source.ID;

				return *this;
			}

			PluginMessage::Header_t & PluginMessage::Header_t::operator=(PluginMessage::Header_t && source)
			{
				PluginID = std::move(source.PluginID);
				ContentData = std::move(source.ContentData);
				BodySize = std::move(source.BodySize);
				ID = std::move(source.ID);

				return *this;
			}

		}
	}
}
