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

SOURCES += main.cpp\
        optionswindow.cpp

HEADERS  += optionswindow.h

INCLUDEPATH  += ../
INCLUDEPATH  += ../../protos/

FORMS    += optionswindow.ui

OBJECTS  += ../*.o

QMAKE_LFLAGS += -pthread
win32:LIBS += -L/mingw64/lib -lprotobuf -lz -lws2_32

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += protobuf
    QT_CONFIG -= no-pkg-config
}
