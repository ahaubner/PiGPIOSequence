
CONFIG -= qt

TARGET = PiGPIOSequenceNetworking
TEMPLATE = lib

DEFINES += PIGPIOSEQUENCENETWORKING_LIBRARY

SOURCES += \
    src/PiGPIOSequence/Extensions/Networking.cpp \
    src/PiGPIOSequence/Extensions/Networking/Server.cpp \
    src/PiGPIOSequence/Extensions/Networking/Client.cpp \
    src/PiGPIOSequence/Extensions/Networking/ServerSpace/Connection.cpp \
    src/PiGPIOSequence/Extensions/Networking/Message.cpp \
    src/PiGPIOSequence/Extensions/Networking/PluginMessage.cpp \
    src/PiGPIOSequence/Extensions/Networking/ServerPlugin.cpp \
    src/PiGPIOSequence/Extensions/Networking/ClientPlugin.cpp \
    src/PiGPIOSequence/Extensions/Networking/Utils.cpp

HEADERS += \
    include/PiGPIOSequence/Extensions/Networking.hpp \
    include/PiGPIOSequence/Extensions/Networking/Server.hpp \
    include/PiGPIOSequence/Extensions/Networking/Client.hpp \
    include/PiGPIOSequence/Extensions/Networking/ServerSpace/Connection.hpp \
    include/PiGPIOSequence/Extensions/Networking/Message.hpp \
    include/PiGPIOSequence/Extensions/Networking/PluginMessage.hpp \
    include/PiGPIOSequence/Extensions/Networking/ClientPlugin.hpp \
    include/PiGPIOSequence/Extensions/Networking/ServerPlugin.hpp \
    include/PiGPIOSequence/Extensions/Networking/Utils.hpp \
    include/PiGPIOSequence/Extensions/Networking/IDGen.hpp


QMAKE_CXXFLAGS += -pedantic
CONFIG -= c++11
CONFIG += c++14

INCLUDEPATH += $$PWD/include

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = lib/debug
} else {
    DESTDIR = lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj

#Depend--------------------------------------------------

#boost
unix|win32: LIBS += -L$$PWD/libs/boost/lib -lboost_system -lboost_serialization

INCLUDEPATH += $$PWD/libs/boost/include
DEPENDPATH += $$PWD/libs/boost/include

win32 {
    LIBS += -lws2_32 -lwsock32
}

#cereal
INCLUDEPATH += $$PWD/libs/cereal/include

#PiGPIOSequence
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequence
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequence

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

#PiGPIOSequence Async
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequenceAsync
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequenceAsync
