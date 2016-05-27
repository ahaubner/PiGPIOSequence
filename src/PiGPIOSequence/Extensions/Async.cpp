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

#include <PiGPIOSequence/Extensions/Async.hpp>

#include <cassert>
#include <memory>

using namespace std;

namespace PiGPIOSequence
{
    namespace Extensions
    {
        static std::mutex mutex_loc;
        static std::atomic_bool running_thread_loc(false);
        static std::atomic_bool stop_thread_flag_loc(false);
        static std::thread main_thread_loc;
        static std::thread stop_thread_loc;

        void main_thread_f(Sequence s, std::function <void()> const & finished_callback,  const Utils::GPIOWriteCallback & GPIOWriteCall, const Utils::GPIOModeCallback & GPIOModeCall)
        {
            std::lock_guard<std::mutex> lock(mutex_loc);
            std::unique_ptr<std::atomic_bool, std::function<void(atomic_bool *)>> guard(&running_thread_loc, [](auto ptr)
            {
               *ptr = false;
            });

            auto & pins = s.get_pins();

            //Erst Mode für alle setzen
            for (auto & p : pins)
            {
                GPIOModeCall(p.PinNumber, Utils::PinMode::output);

                if (stop_thread_flag_loc)
                {
                    return;
                }
            }

            //Dann ausführen
            for (auto & p : pins)
            {
                run(p, GPIOWriteCall);

                if (stop_thread_flag_loc)
                {
                    return;
                }
            }

            if (finished_callback)
            {
                finished_callback();
            }
        }

        void stop_thread_f(AsyncHandle const & h, std::function <void()> const & callback)
        {
            stop(h);
            if (callback)
            {
                callback();
            }
        }

        AsyncHandle run_async(Sequence s, std::function <void()> const & finished_callback,  const Utils::GPIOWriteCallback & GPIOWriteCall, const Utils::GPIOModeCallback & GPIOModeCall)
        {
            AsyncHandle h;

            std::lock_guard<std::mutex> lock(mutex_loc);
            assert(!is_running(h));
            running_thread_loc = true;

            try
            {
                if (main_thread_loc.joinable())
                {
                    main_thread_loc.join();
                }

                main_thread_loc = std::thread(main_thread_f, std::move(s), finished_callback, GPIOWriteCall, GPIOModeCall);
            }

            catch (...) //TODO Korrekte Exception abfangen
            {
                running_thread_loc = false;
            }

            return h;
        }

        bool is_running(AsyncHandle const & h)
        {
            return running_thread_loc;
        }

        void wait(const AsyncHandle & h, const std::function<void ()> & callback)
        {
            std::lock_guard<std::mutex> lock(mutex_loc);
            if (callback)
            {
                callback();
            }

            return;
        }

        void stop(const AsyncHandle & h)
        {
            stop_thread_flag_loc = true;
            std::lock_guard<std::mutex> lock(mutex_loc);
            stop_thread_flag_loc = false;
        }

        void stop_async(const AsyncHandle & h, std::function<void ()> const & callback)
        {
            if (stop_thread_loc.joinable())
            {
                stop_thread_loc.join();
            }

            stop_thread_loc = std::thread(stop_thread_f, h, callback);
        }
    }
}
