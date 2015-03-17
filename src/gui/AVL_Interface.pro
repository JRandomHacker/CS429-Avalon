#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T19:57:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVL_Interface
TEMPLATE = app

CONFIG += c++11

SOURCES  += main.cpp\
    optionswindow.cpp \
    gamewindow.cpp \
    createserverwindow.cpp \
    joinserverwindow.cpp \
    displayerrors.cpp \
    guihelpers.cpp

HEADERS  += optionswindow.hpp \
    gamewindow.hpp \
    createserverwindow.hpp \
    joinserverwindow.hpp \
    displayerrors.hpp \
    guihelpers.hpp

FORMS    += optionswindow.ui \
    gamewindow.ui \
    createserverwindow.ui \
    joinserverwindow.ui

INCLUDEPATH  += ../protos/
INCLUDEPATH  += ../common/
INCLUDEPATH  += ../client/

OBJECTS  += ../protos/*.o
OBJECTS  += ../common/*.o
OBJECTS  += ../client/*.o

QMAKE_LFLAGS += -pthread
win32:LIBS += -L/mingw64/lib -lprotobuf -lz -lws2_32

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += protobuf
    QT_CONFIG -= no-pkg-config
}
