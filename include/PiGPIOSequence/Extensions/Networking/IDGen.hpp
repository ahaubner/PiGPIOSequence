/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceNetworking library.
 *
 *   PiGPIOSequenceNetworking is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceNetworking is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceNetworking.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_IDGEN_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_IDGEN_HPP

#include <vector>
#include <limits>
#include <stdexcept>
#include <cassert>

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Networking
        {
            template <typename T>
            class IDGen
            {
                private:
                    using Pair = std::pair<T, bool>; //True means, ID is in use
                    using IDVector = std::vector<Pair>;

                public:
                    T generateID()
                    {
                        for (auto & PairRef : m_IDs)
                        {
                            if (!PairRef.second)
                            {
                                PairRef.second = true;
                                return PairRef.first;
                            }
                        }

                        //Keine Leere gefunden

                        //Prüfen ob maximum erreicht
                        if (m_Next == m_Maximum)
                        {
                            if (m_IDs.back().first == m_Maximum) //Maximum wird bereits genutzt
                            {
                                throw std::out_of_range("Maximum ID reached. No more unused IDs");
                            }

                            else
                            {
                                m_IDs.emplace_back(m_Next, true);
                                return m_Next;
                            }
                        }

                        else
                        {
                            m_IDs.emplace_back(m_Next, true);

                            return m_Next++;
                        }
                    }

                    void releaseID(T id)
                    {
                        for (auto & PairRef : m_IDs)
                        {
                            if (PairRef.first == id)
                            {
                                PairRef.second = false;
                                return;
                            }
                        }

                        throw std::invalid_argument("Got unregistered ID. Therefore it can't be released.");
                    }

                    bool wasGenerated(T id) const
                    {
                        for (auto & PairRef : m_IDs)
                        {
                            if (PairRef.first == id)
                            {
                                return true;
                            }
                        }

                        return false;
                    }

                    bool isInUse(T id) const
                    {
                        for (auto & PairRef : m_IDs)
                        {
                            if (PairRef.first == id)
                            {
                                return PairRef.second;
                            }
                        }

                        return false;
                    }

                    typename IDVector::size_type size() const
                    {
                        return m_IDs.size();
                    }

                    static constexpr T firstID = std::numeric_limits<T>::min();
                    static constexpr T lastID = std::numeric_limits<T>::max();

                    private:
                        IDVector m_IDs;

                        T m_Next {firstID};
                        T m_Maximum {lastID};
            };
        }
    }
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_NETWORKING_IDGEN_HPP
