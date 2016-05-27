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

#ifndef PIGPIOSEQUENCE_UTILS_HPP
#define PIGPIOSEQUENCE_UTILS_HPP

#include <functional>

namespace PiGPIOSequence
{
	namespace Utils
	{
		using PinNumber = int;
		using Milliseconds = unsigned int;
		using PinMode_it = int;
		using OutputMode_it = int;

		enum class PinMode : PinMode_it
		{
			input = 0,
			output = 1
		};

		enum class OutputMode : OutputMode_it
		{
			low = 0,
			high = 1
		};

		void setPinMode(PinNumber pin, PinMode mode);
		void delay(Milliseconds milliseconds);
		void digitalWrite(PinNumber pin, OutputMode mode);

		void setPinMode_ThreadSafe(PinNumber pin, PinMode mode);
		void digitalWrite_ThreadSafe(PinNumber pin, OutputMode mode);

		bool isEmulating();

        using GPIOModeCallback = std::function<void (Utils::PinNumber, Utils::PinMode)>;
        using GPIOWriteCallback = std::function<void (Utils::PinNumber, Utils::OutputMode)>;
	}
}

#endif // UTILS_HPP
