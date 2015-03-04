#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T19:57:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVL_Interface
TEMPLATE = app


SOURCES += main.cpp\
        optionswindow.cpp \
    connectwindow.cpp \
    gamewindow.cpp \
    createserverwindow.cpp \
    joinserverwindow.cpp

HEADERS  += optionswindow.h \
    connectwindow.h \
    gamewindow.h \
    createserverwindow.h \
    joinserverwindow.h

FORMS    += optionswindow.ui \
    connectwindow.ui \
    gamewindow.ui \
    createserverwindow.ui \
    joinserverwindow.ui
