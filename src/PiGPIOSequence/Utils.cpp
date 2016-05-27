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

#include <PiGPIOSequence/Utils.hpp>

#include <type_traits>
#include <thread>
#include <mutex>
#include <chrono>

#include "WrapperImpl.cpp"

#ifndef USE_WIRING_PI
	#include <iostream>
#endif

using namespace std;

namespace PiGPIOSequence
{
	namespace Utils
	{
        static mutex local_mutex;

		//Cast-Helper
		template<typename T>
		auto cast_to_underlying(T Enum) -> typename underlying_type<T>::type
		{
		   return static_cast<typename underlying_type<T>::type>(Enum);
		}
		//~

		void setPinMode(PinNumber pin, PinMode mode)
		{
			#ifdef USE_WIRING_PI
				::pinMode(pin, cast_to_underlying(mode));
			#else
				cout << "pinMode(" << pin << ", " << cast_to_underlying(mode) << ");" << endl;
			#endif
		}

		void delay(Milliseconds milliseconds)
		{
			#ifndef USE_WIRING_PI
				cout << "delay(" << milliseconds << ");" << endl;
			#endif

			this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		}

		void digitalWrite(PinNumber pin, OutputMode mode)
		{
			#ifdef USE_WIRING_PI
				::digitalWrite(pin, cast_to_underlying(mode));
			#else
				cout << "digitalWrite(" << pin << ", " << cast_to_underlying(mode) << ");" << endl;
			#endif
		}

		void setPinMode_ThreadSafe(PinNumber pin, PinMode mode)
		{
            lock_guard<mutex> Guard{local_mutex};
			setPinMode(pin, mode);
		}

		void digitalWrite_ThreadSafe(PinNumber pin, OutputMode mode)
		{
            lock_guard<mutex> Guard{local_mutex};
			digitalWrite(pin, mode);
		}

		bool isEmulating()
		{
			#ifdef USE_WIRING_PI
				return false;
			#else
				return true;
			#endif
		}

	}
}
