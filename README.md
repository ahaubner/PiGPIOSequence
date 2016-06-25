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
- boost serialization (Network features)

# Build instructions
## Setup
### GNU/Linux

[Ubuntu] Installing packages:
```sh
sudo apt-get -y install git build-essential qt5-default libboost-system-dev libboost-serialization-dev
```

Cloning additional libraries and project:
```sh
git clone https://github.com/USCiLab/cereal.git
git clone https://github.com/ahaubner/IPFind.git
git clone https://github.com/ahaubner/PiGPIOSequence.git
```

Symlink libs (if you don't want to install them):
```sh
mkdir IPFind/libs
mkdir -p PiGPIOSequence/libs/IPFind

ln -s $(pwd)/cereal IPFind/libs/cereal
ln -s $(pwd)/cereal PiGPIOSequence/libs/cereal

ln -s $(pwd)/IPFind/include PiGPIOSequence/libs/IPFind/include
ln -s $(pwd)/IPFind/lib PiGPIOSequence/libs/IPFind/lib
```

TODO: Windows

## Building
### GNU/Linux
The general idea:
```sh
cd PiGPIOSequence
qmake [DEFINES+=USE_WIRING_PI] my_target.pro
make
```

- When building for the Raspberry Pi inlude the "`DEFINES+=USE_WIRING_PI`"
- Replace `my_target.pro` with the project component you want to build
- Examples
  - Main lib: `qmake PiGPIOSequence.pro`
  - Main lib for Raspberry Pi: `qmake DEFINES+=USE_WIRING_PI PiGPIOSequence.pro`

| Subproject               | Target File        | What to build first                                     |
|--------------------------|--------------------|---------------------------------------------------------|
| PiGPIOSequence           | `PiGPIOSequence.pro` | (nothing)                                               |
| PiGPIOSequenceAsync      | `Async.pro`          | `PiGPIOSequence.pro`                                      |
| PiGPIOSequenceNetworking | `Networking.pro`     | `PiGPIOSequence.pro` `Async.pro`                            |
| PiGPIOSequenceWidgets    | `Widgets.pro`        | `PiGPIOSequence.pro`                                      |
| NetServer                | `NetServer.pro`      | `PiGPIOSequence.pro` `Async.pro` `Networking.pro`             |
| Designer                 | `designer.pro`       | `PiGPIOSequence.pro` `Async.pro` `Networking.pro` `Widgets.pro` |

TODO: Windows

# Known bugs
- See bugtracker
- Stopping sequences fails

# License
The PiGPIOSequence libraries and applications are licensed under the terms of the GNU Affero General Public License Version 3.

See the source files and COPYING.AGPL.
