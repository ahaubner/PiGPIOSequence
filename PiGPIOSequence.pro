CONFIG -= qt

TARGET = PiGPIOSequence
TEMPLATE = lib

DEFINES += PIGPIOSEQUENCE_LIBRARY

SOURCES += \
    src/PiGPIOSequence/PinConfig.cpp \
    src/PiGPIOSequence/Sequence.cpp \
    src/PiGPIOSequence.cpp \
    src/PiGPIOSequence/Utils.cpp \
    src/PiGPIOSequence/WrapperImpl.cpp \
    src/PiGPIOSequence/ParallelSequences.cpp

HEADERS += \
    include/PiGPIOSequence/Sequence.hpp \
    include/PiGPIOSequence/PinConfig.hpp \
    include/PiGPIOSequence.hpp \
    include/PiGPIOSequence/Utils.hpp \
    include/PiGPIOSequence/ParallelSequences.hpp

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = lib/debug
} else {
    DESTDIR = lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj

#DEFINES += USE_WIRING_PI

contains(DEFINES, USE_WIRING_PI) {
    LIBS += -lwiringPi

    message(Using libWiringPi)
} else {
    message(Not using libWiringPi)
}

QMAKE_CXXFLAGS += -pedantic
CONFIG -= c++11
CONFIG += c++14

INCLUDEPATH += $$PWD/include
