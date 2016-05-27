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

#include <PiGPIOSequence/PinConfig.hpp>

namespace PiGPIOSequence
{
	PinConfig::PinConfig(Utils::PinNumber number, bool pinstate, Utils::Milliseconds delay)
		:	PinNumber(number),
			state(pinstate),
			delay(delay)
	{}

    PinConfig::PinConfig(const PinConfig & source)
        :   PinNumber(source.PinNumber),
			state(source.state),
			delay(source.delay)
	{}

	PinConfig::PinConfig(PinConfig && source)
		:	PinNumber(std::move(source.PinNumber)),
			state(std::move(source.state)),
			delay(std::move(source.delay))
	{}

	PinConfig &PinConfig::operator=(const PinConfig & source)
	{
		PinNumber = source.PinNumber;
		state = source.state;
		delay = source.delay;

		return *this;
	}

	PinConfig & PinConfig::operator=(PinConfig && source)
	{
		PinNumber = std::move(source.PinNumber);
		state = std::move(source.state);
		delay = std::move(source.delay);

        return *this;
    }

    void run(const PinConfig & p, const Utils::GPIOWriteCallback & GPIOWriteCall)
    {
        GPIOWriteCall(p.PinNumber,

                     (p.state ?
                         Utils::OutputMode::high :
                         Utils::OutputMode::low
                        )
                     );

        Utils::delay(p.delay);
    }
}
