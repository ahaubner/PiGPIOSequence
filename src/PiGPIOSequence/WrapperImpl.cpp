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

#ifdef USE_WIRING_PI
	#include <wiringPi.h>

	#include <type_traits>

	#include <boost/typeof/typeof.hpp>
	#include <boost/type_traits.hpp>

	#include <PiGPIOSequence/Utils.hpp>

	using namespace std;

	//Sicherstellen, dass der Wrapper funktioniert
	namespace PiGPIOSequence
	{
		namespace WrapperImpl
		{
			//Helper
			template<typename T>
			constexpr auto cast_to_underlying(T Enum) -> typename underlying_type<T>::type
			{
			   return static_cast<typename underlying_type<T>::type>(Enum);
			}

			//pinMode(...)
			using WPi_PinMode_ft = BOOST_TYPEOF(pinMode);

			using WPi_PinMode_PinArgT = boost::function_traits<WPi_PinMode_ft>::arg1_type;
			using WPi_PinMode_ModeT = boost::function_traits<WPi_PinMode_ft>::arg2_type;

			//delay(...)
			using WPi_delay_ft = BOOST_TYPEOF(delay);

			using WPi_delay_MillisecondsT = boost::function_traits<WPi_delay_ft>::arg1_type;

			//digitalWrite(...)
			using WPi_digitalWrite_ft = BOOST_TYPEOF(digitalWrite);

			using WPi_digitalWrite_PinArgT = boost::function_traits<WPi_digitalWrite_ft>::arg1_type;
			using WPi_digitalWrite_OutputModeT = boost::function_traits<WPi_digitalWrite_ft>::arg2_type;

			//Tests
			static_assert(is_same<WPi_PinMode_PinArgT, Utils::PinNumber>::value, "Wrapper Pin-Nummertyp stimmt nicht mit WiringPi Typ überein");
			static_assert(is_same<WPi_PinMode_ModeT, Utils::PinMode_it>::value, "Wrapper PinMode stimmt nicht mit WiringPi PinMode Typ überein");

			static_assert(is_same<WPi_delay_MillisecondsT, Utils::Milliseconds>::value, "Wrapper Utils::Milliseconds Typ stimmt nicht mit WiringPi delay Argument-Typ überein");

			static_assert(is_same<WPi_digitalWrite_PinArgT, Utils::PinNumber>::value, "Wrapper Pin-Nummertyp stimmt nicht mit WiringPi Typ überein");
			static_assert(is_same<WPi_digitalWrite_OutputModeT, Utils::OutputMode_it>::value, "Wrapper Pin-Ausgabetyp stimmt nicht mit WiringPi Typ überein");

			#ifndef INPUT
				static_assert(false, "INPUT Makro von WiringPi wurde nicht gefunden");
			#else
				static_assert(INPUT == cast_to_underlying(Utils::PinMode::input), "Wrapper PinMode::input stimmt nicht mit WiringPi INPUT Makro überein");
			#endif

			#ifndef OUTPUT
				static_assert(false, "OUTPUT Makro von WiringPi wurde nicht gefunden");
			#else
				static_assert(OUTPUT == cast_to_underlying(Utils::PinMode::output), "Wrapper PinMode::output stimmt nicht mit WiringPi OUTPUT Makro überein");
			#endif

			#ifndef LOW
				static_assert(false, "LOW Makro von WiringPi wurde nicht gefunden");
			#else
				static_assert(LOW == cast_to_underlying(Utils::OutputMode::low), "Wrapper OutputMode::low stimmt nicht mit WiringPi LOW Makro überein");
			#endif

			#ifndef HIGH
				static_assert(false, "HIGH Makro von WiringPi wurde nicht gefunden");
			#else
				static_assert(HIGH == cast_to_underlying(Utils::OutputMode::high), "Wrapper OutputMode::high stimmt nicht mit WiringPi HIGH Makro überein");
			#endif
		}
	}
#else
//	#include <iostream>

//	#define pinMode(a, b) std::cout << "pinMode(" << a << ", " << b << ");\n"
//	#define delay(a) std::cout << "delay(" << a << ");\n"; std::this_thread::sleep_for(std::chrono::milliseconds(a));
//	#define digitalWrite(a, b) std::cout << "digitalWrite(" << a << ", " << b << ");\n"

//	#define OUTPUT 1
//	#define HIGH 1
//	#define LOW 0
#endif
