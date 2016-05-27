/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceAsync library.
 *
 *   PiGPIOSequenceAsync is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceAsync is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceAsync.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PIGPIOSEQUENCE_EXTENSIONS_ASYNC_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_ASYNC_HPP

#include <PiGPIOSequence.hpp>
#include <thread>
#include <functional>
#include <stdexcept>
#include <mutex>

namespace PiGPIOSequence
{
	namespace Extensions
	{
        class AsyncHandle
        { /*Momentan leere Hülle*/ };

        AsyncHandle run_async(Sequence s, std::function <void()> const & finished_callback = {}, const Utils::GPIOWriteCallback & GPIOWriteCall = Utils::digitalWrite, Utils::GPIOModeCallback const & GPIOModeCall = Utils::setPinMode);
        void wait(AsyncHandle const & h, std::function<void()> const & callback = {});
        void stop(AsyncHandle const & h);
        void stop_async(AsyncHandle const & h, const std::function<void ()> & callback = {});
        bool is_running(const AsyncHandle & h);
	}
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_ASYNC_HPP
