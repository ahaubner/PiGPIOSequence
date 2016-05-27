/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as
 *   published by the Free Software Foundation, either version 3 of the
 *   License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>

#include <PiGPIOSequence.hpp>
#include <PiGPIOSequence/Extensions/Networking.hpp>

#include <IPFind.hpp>

#include <cstring>

using namespace std;
using namespace PiGPIOSequence::Extensions;
using namespace PiGPIOSequence::Extensions::Networking;

int main(int argc, char * argv[])
{
    cout << "[PiGPIOSequence NetServer]\n"

         << "Copyright (C) 2016 Anton Haubner - anton.haubner@outlook.de\n\n"

         << "This program comes with ABSOLUTELY NO WARRANTY; for details start with -w option. \n"
            "This is free software, and you are welcome to redistribute it\n"
            "under certain conditions; start with -c option for details.\n\n"

         << "This program makes use of several libraries. For license and copyright information start with -l option\n"

		 << endl;

    bool DebugMode = false;
    bool StoppingOption = false;

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "-d") == 0)
        {
            DebugMode = true;
        }

        else if (std::strcmp(argv[i], "-w") == 0)
        {
            cout << "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION." << endl;
            StoppingOption = true;
        }

        else if (std::strcmp(argv[i], "-c") == 0)
        {
            cout << "For details on redistribution conditions see the GNU AGPL. You should have received a copy of the GNU Affero General Public License along with this program. If not, see <http://www.gnu.org/licenses/>." << endl;
            StoppingOption = true;
        }

        else if (std::strcmp(argv[i], "-l") == 0)
        {
            cout << "This program is free software: you can redistribute it and/or modify\n"
                    "it under the terms of the GNU Affero General Public License as\n"
                    "published by the Free Software Foundation, either version 3 of the\n"
                    "License, or (at your option) any later version.\n"
                    "\n"
                    "This program is distributed in the hope that it will be useful,\n"
                    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                    "GNU Affero General Public License for more details.\n"
                    "\n"
                    "You should have received a copy of the GNU Affero General Public License\n"
                    "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
                    "\n"
                    "\n"
                    "This program makes use of the following libraries, each listed with its license notes, if required:\n"
                    "\n"
                    "------\n"
                    "The PiGPIOSequence libraries\n"
                    "(also licensed under the terms of the GNU AGPL Version 3)\n"
                    "------\n"
                    "\n"
                    "------\n"
                    "The IPFind library\n"
                    "(licensed under the terms of a zlib like license)\n"
                    "------\n"
                    "\n"
                    "------\n"
                    "The Boost C++ libraries\n"
                    "------\n"
                    "\n"
                    "------\n"
                    "The Cereal C++ library\n"
                    "\n"
                    "Cereal license info:\n"
                    "Copyright (c) 2014, Randolph Voorhies, Shane Grant\n"
                    "All rights reserved.\n"
                    "\n"
                    "Redistribution and use in source and binary forms, with or without\n"
                    "modification, are permitted provided that the following conditions are met:\n"
                    " * Redistributions of source code must retain the above copyright\n"
                    "   notice, this list of conditions and the following disclaimer.\n"
                    " * Redistributions in binary form must reproduce the above copyright\n"
                    "   notice, this list of conditions and the following disclaimer in the\n"
                    "   documentation and/or other materials provided with the distribution.\n"
                    " * Neither the name of cereal nor the\n"
                    "   names of its contributors may be used to endorse or promote products\n"
                    "   derived from this software without specific prior written permission.\n"
                    "\n"
                    "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL RANDOLPH VOORHIES OR SHANE GRANT BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
                    "\n"
                    "------\n"
                    "\n"
                    "------\n"
                    "This program also possibly uses the wiringPi library (when enabled during compilation).\n"
                    "wiringPi is licensed under the terms of the GNU Lesser General Public License.\n"
                    "If wiringPi is enabled, you should have received a copy of the GNU Lesser General Public License Version 3\n"
                    "along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.\n"
                    "\n"
                    "Copyright info on wiringPi:\n"
                    "Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>\n"
                    "Additional copyright info can be found in a wiringPi source distribution (https://git.drogon.net/?p=wiringPi)\n"
                    "------\n"
                    "\n"
                 << endl;
            StoppingOption = true;
        }
    }

    if (StoppingOption)
    {
        return 0;
    }

    if (DebugMode)
    {
        cout << "DebugMode active!\n" << endl;
    }

    else
    {
        cout << "Start with -d for debug output\n" << endl;
    }

    PiGPIOSequence::initialize();

    //Mache mich im Netzwerk bemerkbar
    IPFind::Server FinderServ(4124);
		FinderServ.listen_async();

    //Server erstellem
    try
    {
        Server server(DebugMode);

		server.run();
    }

    catch(exception const & e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    cout << "Stopping execution." << endl;

    return 0;
}
