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

#include <PiGPIOSequence/ParallelSequences.hpp>

#include <functional>
#include <iterator>
#include <cassert>

namespace PiGPIOSequence
{
	ParallelSequences::ParallelSequences(const ParallelSequences & rhs)
        :   m_Sequences(rhs.m_Sequences)
    {}

    ParallelSequences::ParallelSequences(ParallelSequences && rhs)
        :   m_Sequences(std::move(rhs.m_Sequences))
    {}

	ParallelSequences & ParallelSequences::operator=(const ParallelSequences & rhs)
	{
        m_Sequences = rhs.m_Sequences;

        return *this;
	}

	ParallelSequences & ParallelSequences::operator=(ParallelSequences && rhs)
	{
        m_Sequences = move(rhs.m_Sequences);

		return *this;
    }

    Sequence & ParallelSequences::addSequence(Sequence rhs)
    {
        m_Sequences.emplace_back(std::move(rhs));

        return m_Sequences.back();
    }

    const Sequence &ParallelSequences::getSequence(SequenceVector::size_type i)
    {
        assert(i < m_Sequences.size());

        return m_Sequences[i];
    }

    Sequence &ParallelSequences::accessSequence(SequenceVector::size_type i)
    {
        assert(i < m_Sequences.size());

        return m_Sequences[i];
    }

    const ParallelSequences::SequenceVector & ParallelSequences::getSequences() const noexcept
    {
        return m_Sequences;
    }

	ParallelSequences::IndexType ParallelSequences::size() const
    {
        return m_Sequences.size();
	}

	bool ParallelSequences::empty() const
	{
        return m_Sequences.empty();
    }

    void ParallelSequences::erase(SequenceVector::size_type i)
    {
        assert(i < m_Sequences.size());

        erase(std::cbegin(m_Sequences) + static_cast<SequenceVector::difference_type>(i));
    }

    void ParallelSequences::erase(SequenceVector::const_iterator i)
    {
        assert(i != std::cend(m_Sequences));

        m_Sequences.erase(i);
    }

	void ParallelSequences::clear()
    {
		m_Sequences.clear();
	}

    void run(const ParallelSequences & p, const Utils::GPIOWriteCallback & GPIOWriteCall, const Utils::GPIOModeCallback & GPIOModeCall)
    {
        auto mixed = combine_sequences(p);

        run(mixed, GPIOWriteCall, GPIOModeCall);
    }

    Sequence combine_sequences(const ParallelSequences & p)
    {
        struct optional_delay
        {
            Utils::Milliseconds value;
            bool valid;
            ParallelSequences::SequenceVector::size_type index;
        };

        using DelayVector = std::vector<optional_delay>;

        auto & seqs = p.getSequences();
        DelayVector delays;
            delays.reserve(seqs.size());

        const auto pick_shortest_delay = [&delays]()
        {
            auto found = std::end(delays);
            auto lowest = std::numeric_limits<Utils::Milliseconds>::max();

            for (auto itr = std::begin(delays); itr != std::end(delays); std::advance(itr, 1))
            {
                if (itr->valid)
                {
                    if (itr->value < lowest)
                    {
                        found = itr;
                        lowest = itr->value;
                    }
                }
            }

            return found;
        };

        std::vector<PinConfig> new_seq;
            { //Bootstrap
                std::vector<PinConfig>::size_type new_size = 0;

                for (auto & s : seqs)
                {
                    auto & pins = s.get_pins();
                    if (!pins.empty())
                    {
                        new_size += pins.size();

                        auto & current_pin = pins.front();
                        delays.push_back({current_pin.delay, true, 0});

                        new_seq.emplace_back(current_pin.PinNumber, current_pin.state, 0);
                    }

                    else
                    {
                        delays.push_back({0, false, 0});
                    }
                }

                if (new_size == 0)
                {
                    return {};
                }

                new_seq.reserve(new_size);
            }

        //Neue Sequenz bauen
        for (auto delay_itr = pick_shortest_delay(); delay_itr != std::end(delays); delay_itr = pick_shortest_delay())
        {
            new_seq.back().delay = delay_itr->value;

            //Differenz abgelehnter delays bestimmen
            for (auto itr = std::begin(delays); itr != std::end(delays); std::advance(itr, 1))
            {
                if (itr != delay_itr && itr->valid)
                {
                    if (delay_itr->value >= itr->value)
                    {
                        itr->value = 0;
                    }

                    else
                    {
                        itr->value -= delay_itr->value;
                    }
                }
            }

            //kürzesten Delay durch nächsten in Reihe ersetzen, wenn möglich
            const auto index = static_cast<ParallelSequences::SequenceVector::size_type>(std::distance(std::begin(delays), delay_itr));
            {
                auto & current_pins = seqs[index].get_pins();
                const auto next_pin_index = delay_itr->index + 1;

                if (next_pin_index < current_pins.size())
                {
                    auto & next_pin = current_pins[next_pin_index];

                    delay_itr->value = next_pin.delay;
                    delay_itr->index = next_pin_index;

                    new_seq.emplace_back(next_pin.PinNumber, next_pin.state, 0);
                }

                else
                {
                    delay_itr->valid = false;
                }
            }
        }

        return Sequence::fromVector(std::move(new_seq));
    }

}

