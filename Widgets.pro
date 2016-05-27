QT       += widgets

TARGET = PiGPIOSequenceWidgets
TEMPLATE = lib

DEFINES += PIGPIO0SEQUENCEWIDGETS_LIBRARY

SOURCES += \
    src/PiGPIOSequence/Extensions/Widgets/SequenceListItem.cpp \
    src/PiGPIOSequence/Extensions/Widgets/PinList.cpp \
    src/PiGPIOSequence/Extensions/Widgets/PinEdit.cpp \
    src/PiGPIOSequence/Extensions/Widgets/PinSelector.cpp \
    src/PiGPIOSequence/Extensions/Widgets/SequenceMixer.cpp \
    src/PiGPIOSequence/Extensions/Widgets/SequenceList.cpp

HEADERS += \
    include/PiGPIOSequence/Extensions/Widgets/SequenceListItem.hpp \
    include/PiGPIOSequence/Extensions/Widgets/PinList.hpp \
    include/PiGPIOSequence/Extensions/Widgets/PinEdit.hpp \
    include/PiGPIOSequence/Extensions/Widgets/PinSelector.hpp \
    include/PiGPIOSequence/Extensions/Widgets/SequenceMixer.hpp \
    include/PiGPIOSequence/Extensions/Widgets/SequenceList.hpp

FORMS += \
    forms/PiGPIOSequence/Extensions/Widgets/SequenceListItem.ui \
    forms/PiGPIOSequence/Extensions/Widgets/PinList.ui \
    forms/PiGPIOSequence/Extensions/Widgets/PinEdit.ui \
    forms/PiGPIOSequence/Extensions/Widgets/PinSelector.ui \
    forms/PiGPIOSequence/Extensions/Widgets/SequenceMixer.ui \
    forms/PiGPIOSequence/Extensions/Widgets/SequenceList.ui


INCLUDEPATH += include

QMAKE_CXXFLAGS += -pedantic
CONFIG -= c++11
CONFIG += c++14


#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = lib/debug
} else {
    DESTDIR = lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui

#Depend--------------------------------------------------

#PiGPIOSequence
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequence
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequence

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

#cereal
INCLUDEPATH += $$PWD/libs/cereal/include
