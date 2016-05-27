CONFIG -= qt

TARGET = PiGPIOSequenceAsync
TEMPLATE = lib

DEFINES += PIGPIOSEQUENCEASYNC_LIBRARY

SOURCES += \
    src/PiGPIOSequence/Extensions/Async.cpp

HEADERS += \
    include/PiGPIOSequence/Extensions/Async.hpp

CONFIG -= c++11
CONFIG += c++14

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = lib/debug
} else {
    DESTDIR = lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj

INCLUDEPATH += $$PWD/include

#Depend--------------------------------------------------

#PiGPIOSequence
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequence
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequence

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
