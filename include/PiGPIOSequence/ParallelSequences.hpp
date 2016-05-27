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

#ifndef PIGPIOSEQUENCE_PARALLELSEQUENCES_HPP
#define PIGPIOSEQUENCE_PARALLELSEQUENCES_HPP

#include <vector>
#include <functional>

#include <PiGPIOSequence/Sequence.hpp>

namespace PiGPIOSequence
{
    class ParallelSequences
	{
        public:
            using SequenceVector = std::vector<Sequence>;

			using IndexType = SequenceVector::size_type;
			using Callback = std::function<void ()>;

            ParallelSequences() = default;
			ParallelSequences(ParallelSequences const & rhs);
			ParallelSequences(ParallelSequences && rhs);

			ParallelSequences & operator=(ParallelSequences const & rhs);
			ParallelSequences & operator=(ParallelSequences && rhs);

            Sequence & addSequence(Sequence rhs);

            Sequence const & getSequence(SequenceVector::size_type i);
            Sequence & accessSequence(SequenceVector::size_type i);
            SequenceVector const & getSequences() const noexcept;

			IndexType size() const;
			bool empty() const;

            void erase(SequenceVector::size_type i);
            void erase(SequenceVector::const_iterator i);

			void clear();

            template <typename Archive>
            void save(Archive & ar) const
            {
                ar(m_Sequences);
            }

            template <typename Archive>
            void load(Archive & ar)
            {
                ar(m_Sequences);
            }

        private:
            SequenceVector m_Sequences;
	};

    Sequence combine_sequences(ParallelSequences const & p);
    void run(ParallelSequences const & p, Utils::GPIOWriteCallback const & GPIOWriteCall = Utils::digitalWrite, Utils::GPIOModeCallback const & GPIOModeCall = Utils::setPinMode);
}

#endif // PIGPIOSEQUENCE_PARALLELSEQUENCES_HPP
