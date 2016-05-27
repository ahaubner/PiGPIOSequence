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

#ifndef PIGPIOSEQUENCE_HPP
#define PIGPIOSEQUENCE_HPP

namespace PiGPIOSequence
{
	extern void initialize();
}

#include <PiGPIOSequence/PinConfig.hpp>
#include <PiGPIOSequence/Sequence.hpp>
#include <PiGPIOSequence/ParallelSequences.hpp>
#include <PiGPIOSequence/Utils.hpp>

#endif // PIGPIOSEQUENCE_HPP
