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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_UTILS_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_UTILS_HPP

#include <vector>
#include <cstdint>

#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

#include <boost/archive/text_iarchive.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Networking
		{
			namespace Utils
			{
				template <typename Archive>
				class IArchiveWrapper
				{
						static_assert(std::is_base_of<boost::archive::detail::common_iarchive<Archive>, Archive>::value, "Archive has to be an input archive.");

					public:
						using byte = char;
						static_assert(sizeof(byte) == sizeof(std::uint8_t), "Char does not have the size of a byte");

					private:
						using BufferType = boost::interprocess::bufferstream;
						BufferType m_Buffer;
						Archive m_Archive;

						static_assert(std::is_same<BufferType::char_type, byte>::value, "Boost bufferstream char is not compatible");

					public:
						IArchiveWrapper(std::vector<byte> & source)
							:	m_Buffer(source.data(), source.size()),
								m_Archive(m_Buffer)
						{}

						Archive & get()
						{
							return m_Archive;
						}

						Archive const & get() const
						{
							return m_Archive;
						}

						template <typename T>
						Archive & operator>>(T & t)
						{
						    m_Archive >> t;
						    return m_Archive;
						}
				};

				template <typename Archive>
				class OArchiveWrapper
				{
						static_assert(std::is_base_of<boost::archive::detail::common_oarchive<Archive>, Archive>::value, "Archive has to be an output archive.");

					public:
						using byte = char;
						static_assert(sizeof(byte) == sizeof(std::uint8_t), "Char does not have the size of a byte");

					private:
						using StreamType = boost::interprocess::basic_vectorstream<std::vector<byte>>;
						StreamType m_VStream;
						Archive m_Archive;

						static_assert(std::is_same<StreamType::char_type, byte>::value, "Boost vectorstream char is not compatible");

					public:
						OArchiveWrapper()
							:	m_Archive(m_VStream)
						{}

						Archive & get()
						{
							return m_Archive;
						}

						Archive const & get() const
						{
							return m_Archive;
						}

						template <typename T>
						Archive & operator<<(T & t)
						{
						    m_Archive << t;
						    return m_Archive;
						}

						void saveToVector(std::vector<byte> & target)
						{
							m_VStream.flush();
							m_VStream.swap_vector(target);
							m_VStream.clear();
						}
				};

				using Text_IArchiveWrapper = IArchiveWrapper<boost::archive::text_iarchive>;
				using Text_OArchiveWrapper = OArchiveWrapper<boost::archive::text_oarchive>;

//				template <typename Archive>
//				inline IArchiveWrapper<Archive> archiveFromVector(std::vector<char> & source)
//				{
//					return {source};
//				}
			}
		}
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_UTILS_HPP
