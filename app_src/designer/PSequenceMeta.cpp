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

#include "PSequenceMeta.hpp"

PSequenceMeta::PSequenceMeta(std::string name, PiGPIOSequence::ParallelSequences pseq)
    :   m_Name(std::move(name)),
        m_PSeq(std::move(pseq))
{

}

void PSequenceMeta::setName(std::string rhs)
{
    m_Name = std::move(rhs);
}

const std::string &PSequenceMeta::getName() const noexcept
{
    return m_Name;
}

const PiGPIOSequence::ParallelSequences &PSequenceMeta::getPSeq() const noexcept
{
    return m_PSeq;
}

PiGPIOSequence::ParallelSequences &PSequenceMeta::accessPSeq() noexcept
{
    return m_PSeq;
}
