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

#ifndef PIGPIOSEQUENCE_PINCONFIG_HPP
#define PIGPIOSEQUENCE_PINCONFIG_HPP

#include <PiGPIOSequence/Utils.hpp>

namespace PiGPIOSequence
{
    class PinConfig
	{
		public:
			PinConfig(Utils::PinNumber number = -1, bool pinstate = false, Utils::Milliseconds delay = 100);
			PinConfig(PinConfig const & source);
			PinConfig(PinConfig && source);

			PinConfig & operator=(PinConfig const & source);
			PinConfig & operator=(PinConfig && source);

			Utils::PinNumber PinNumber;
			bool state;
			Utils::Milliseconds delay;

            template <typename Archive>
            void save(Archive & ar) const
            {
                ar(PinNumber, state, delay);
            }

            template <typename Archive>
            void load(Archive & ar)
            {
                ar(PinNumber, state, delay);
            }
	};

    void run(PinConfig const & p, Utils::GPIOWriteCallback const & GPIOWriteCall = Utils::digitalWrite);
}

#endif // PIGPIOSEQUENCE_PINCONFIG_HPP
