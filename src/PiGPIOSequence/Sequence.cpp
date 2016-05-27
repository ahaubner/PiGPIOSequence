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

#include <PiGPIOSequence/Sequence.hpp>

#include <thread>
#include <stdexcept>
#include <cassert>

#include <PiGPIOSequence/Utils.hpp>

using namespace std;

namespace PiGPIOSequence
{
    Sequence::Sequence(const Sequence &source)
    {
		operator =(source);
	}

	Sequence::Sequence(Sequence && source)
	{
        operator =(std::move(source));
    }

    Sequence Sequence::fromVector(Sequence::PinConfigVector v)
    {
        Sequence seq;
        seq.m_PinConfigs = std::move(v);

        return seq;
    }

    Sequence & Sequence::operator=(const Sequence & source)
    {
        m_PinConfigs = source.m_PinConfigs;

        return *this;
    }

    Sequence &Sequence::operator=(Sequence && source)
    {
        m_PinConfigs = std::move(source.m_PinConfigs);

		return *this;
    }

    PinConfig & Sequence::add(PinConfig config)
    {
        m_PinConfigs.emplace_back(std::move(config));

        return m_PinConfigs.back();
    }

    const Sequence::PinConfigVector &Sequence::get_pins() const noexcept
    {
        return m_PinConfigs;
    }

    const PinConfig &Sequence::get_pin(PinConfigVector::size_type i) const
    {
        assert(i < m_PinConfigs.size());

        return m_PinConfigs[i];
    }

    PinConfig &Sequence::access_pin(PinConfigVector::size_type i)
    {
        assert(i < m_PinConfigs.size());

        return m_PinConfigs[i];
    }

    void Sequence::erase(PinConfigVector::size_type i)
    {
        assert(i < m_PinConfigs.size());

        auto iter = m_PinConfigs.begin() + static_cast<PinConfigVector::difference_type>(i);

        erase(iter);
    }

    void Sequence::erase(PinConfigVector::const_iterator iter)
    {
        assert(iter != m_PinConfigs.end());

        m_PinConfigs.erase(iter);
    }

	void Sequence::clear()
    {
		m_PinConfigs.clear();
	}

    Sequence::IndexType Sequence::size() const noexcept
	{
		return m_PinConfigs.size();
	}

    void run(const Sequence & s, Utils::GPIOWriteCallback const & GPIOWriteCall, const Utils::GPIOModeCallback & GPIOModeCall)
    {
        auto & pins = s.get_pins();

        //Erst Mode für alle setzen
        for (auto & p : pins)
        {
            GPIOModeCall(p.PinNumber, Utils::PinMode::output);
        }

        //Dann ausführen
        for (auto & p : pins)
        {
            run(p, GPIOWriteCall);
        }
    }

}
