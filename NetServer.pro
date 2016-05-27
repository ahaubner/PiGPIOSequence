TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = NetServer

SOURCES += \
    app_src/NetServer/main.cpp

QMAKE_CXXFLAGS += -pedantic
CONFIG -= c++11
CONFIG += c++14

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = bin/NetServer/debug
} else {
    DESTDIR = bin/NetServer/release
}

OBJECTS_DIR = $$DESTDIR/.obj


#Depend--------------------------------------------------

#Shall it use WiringPi?
#DEFINES += USE_WIRING_PI

contains(DEFINES, USE_WIRING_PI) {
    LIBS += -lwiringPi

    message(Using libWiringPi)
} else {
    message(Not using libWiringPi)
}

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

#PiGPIOSequenceNetworking
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequenceNetworking
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequenceNetworking

#IPFind
CONFIG(release, debug|release): LIBS += -L$$PWD/libs/IPFind/lib/release -lIPFind
CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/IPFind/lib/debug -lIPFind

INCLUDEPATH += $$PWD/libs/IPFind/include
DEPENDPATH += $$PWD/libs/IPFind/include

unix {
    LIBS += -lpthread
}
