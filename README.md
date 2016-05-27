# PiGPIOSequence

This is a collection of cross platform libraries and utility applications for controlling the GPIOs of a Raspberry Pi.

This project is by no means complete! Until now, the code was mostly written "on the go" and needs improvements/bug fixes/unit tests.
However, it is in a somewhat usable condition ;)

| Subproject               | Description                                                                                                                          |
|--------------------------|--------------------------------------------------------------------------------------------------------------------------------------|
| PiGPIOSequence           | Base library for creating  sequences  of GPIO pin accesses and running them. Multiple sequences in parallel execution are supported. |
| PiGPIOSequenceAsync      | Utilities for running sequences asynchronously                                                                                       |
| PiGPIOSequenceNetworking | Server and Client classes for running sequences on a remote Raspberry Pi                                                             |
| PiGPIOSequenceWidgets    | Qt Widgets for creating GUI applications which operate on PiGPIOSequence classes                                                     |
| NetServer                | Simple server application using PiGPIOSequenceNetworking                                                                             |
| Designer                 | GUI application for simple creation of sequences. Supports saving, loading and sending over network.                                 |

# Dependencies
## Build tools:
- C++14 compiler, tested with...
  - clang 3.8
  - g++ 6.1.1
- QMake

## Libraries
- IPFind (NetServer and Designer only)
- cereal
- Qt 5 (Designer only)
- wiringPi (required only for real use on Raspberry Pi)
- boost asio (Network features only)
  - boost system

# Build instructions
TODO

# Known bugs
- See bugtracker
- Stopping sequences fails

# License
The PiGPIOSequence libraries and applications are licensed under the terms of the GNU Affero General Public License Version 3.

See the source files and COPYING.AGPL.
