/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequence library.
 *
 *   PiGPIOSequence is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequence is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequence.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PIGPIOSEQUENCE_SEQUENCE_HPP
#define PIGPIOSEQUENCE_SEQUENCE_HPP

#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <functional>

#include <PiGPIOSequence/PinConfig.hpp>

namespace PiGPIOSequence
{
    class Sequence
    {
        public:
            using PinConfigVector = std::vector<PinConfig>;
            using IndexType = PinConfigVector::size_type;

            Sequence() = default;
			Sequence(Sequence const & source);
            Sequence(Sequence && source);

            static Sequence fromVector(PinConfigVector v);

            Sequence & operator=(Sequence const & source);
            Sequence & operator=(Sequence && source);

            PinConfig & add(PinConfig config);

            PinConfigVector const & get_pins() const noexcept;
            PinConfig const & get_pin(PinConfigVector::size_type i) const;
            PinConfig & access_pin(PinConfigVector::size_type i);

            void erase(PinConfigVector::size_type i);
            void erase(PinConfigVector::const_iterator iter);

			void clear();

            IndexType size() const noexcept;

            template <typename Archive>
            void save(Archive & ar) const
            {
                ar(m_PinConfigs);
            }

            template <typename Archive>
            void load(Archive & ar)
            {
                ar(m_PinConfigs);
            }

        private:
            PinConfigVector m_PinConfigs;
	};

    void run(Sequence const & s, const Utils::GPIOWriteCallback & GPIOWriteCall = Utils::digitalWrite, Utils::GPIOModeCallback const & GPIOModeCall = Utils::setPinMode);
}

#endif // PIGPIOSEQUENCE_SEQUENCE_HPP
