/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequence Designer program.
 *
 *   PiGPIOSequence Designer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequence Designer is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequence Designer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SEQUENCEGENERATOR_HPP
#define SEQUENCEGENERATOR_HPP

#include <PiGPIOSequence.hpp>

#include <memory>

class SequenceGenerator
{
    public:
        using UniqueSequencePtr = std::unique_ptr<PiGPIOSequence::Sequence>;

		SequenceGenerator();
		virtual ~SequenceGenerator();

        virtual UniqueSequencePtr generate() = 0;
        virtual bool apply(PiGPIOSequence::Sequence & Ptr) = 0;
};

#endif
