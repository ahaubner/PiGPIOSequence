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

#ifndef PSEQUENCEMETA_HPP
#define PSEQUENCEMETA_HPP

#include <PiGPIOSequence/ParallelSequences.hpp>
#include <string>

//Container für ParallelSequences mit Metadaten
class PSequenceMeta
{
    public:
        PSequenceMeta() = default;
        PSequenceMeta(std::string name, PiGPIOSequence::ParallelSequences pseq = {});

        void setName(std::string rhs);
        std::string const & getName() const noexcept;

        PiGPIOSequence::ParallelSequences const & getPSeq() const noexcept;
        PiGPIOSequence::ParallelSequences & accessPSeq() noexcept;

        template <typename Archive>
        void save(Archive & ar) const
        {
            ar(m_Name, m_PSeq);
        }

        template <typename Archive>
        void load(Archive & ar)
        {
            ar(m_Name, m_PSeq);
        }

    private:
        std::string m_Name;
        PiGPIOSequence::ParallelSequences m_PSeq;
};

#endif // PSEQUENCEMETA_HPP
