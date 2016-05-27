QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = designer
TEMPLATE = app


QMAKE_CXXFLAGS += -pedantic
CONFIG -= c++11
CONFIG += c++14

#TargetDir
CONFIG(debug, debug|release) {
    DESTDIR = bin/designer/debug
} else {
    DESTDIR = bin/designer/release
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

#PiGPIOSequenceNetworking
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequenceNetworking
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequenceNetworking

#PiGPIOSequence Widgets
CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lPiGPIOSequenceWidgets
CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lPiGPIOSequenceWidgets

#IPFind
CONFIG(release, debug|release): LIBS += -L$$PWD/libs/IPFind/lib/release -lIPFind
CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/IPFind/lib/debug -lIPFind

INCLUDEPATH += $$PWD/libs/IPFind/include
DEPENDPATH += $$PWD/libs/IPFind/include

unix {
    LIBS += -lpthread
}

FORMS += \
    app_src/designer/EasyGenDialog.ui \
    app_src/designer/InfoDialog.ui \
    app_src/designer/MainWindow.ui \
    app_src/designer/ParallelSequenceContainerWidget.ui \
    app_src/designer/ParallelSequencesEditDialog.ui \
    app_src/designer/PGSPluginExtrasDialog.ui \
    app_src/designer/PulseGenDialog.ui

HEADERS += \
    app_src/designer/EasyGenDialog.hpp \
    app_src/designer/InfoDialog.hpp \
    app_src/designer/MainWindow.hpp \
    app_src/designer/MixerGenerator.hpp \
    app_src/designer/ParallelSequenceContainerWidget.hpp \
    app_src/designer/ParallelSequencesEditDialog.hpp \
    app_src/designer/ParallelSequencesViewer.hpp \
    app_src/designer/PGSPluginExtrasDialog.hpp \
    app_src/designer/PulseGenDialog.hpp \
    app_src/designer/SequenceGenerator.hpp \
    app_src/designer/SequencesNormalViewer.hpp \
    app_src/designer/PSequenceMeta.hpp

SOURCES += \
    app_src/designer/EasyGenDialog.cpp \
    app_src/designer/InfoDialog.cpp \
    app_src/designer/main.cpp \
    app_src/designer/MainWindow.cpp \
    app_src/designer/MixerGenerator.cpp \
    app_src/designer/ParallelSequencesEditDialog.cpp \
    app_src/designer/ParallelSequencesViewer.cpp \
    app_src/designer/PGSPluginExtrasDialog.cpp \
    app_src/designer/PulseGenDialog.cpp \
    app_src/designer/SequenceGenerator.cpp \
    app_src/designer/SequencesNormalViewer.cpp \
    app_src/designer/PSequenceMeta.cpp \
    app_src/designer/ParallelSequenceContainerWidget.cpp
