#-------------------------------------------------
#
# Project created by QtCreator 2016-01-07T10:47:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Socket.cpp \
    Reader.cpp \
    LibraryPanel.cpp

HEADERS  += mainwindow.h \
    Socket.h \
    Reader.h \
    LibraryPanel.h

FORMS    += mainwindow.ui \
    LibraryPanel.ui

INCLUDEPATH += ~/home/Programs/RC/Proiect/ClientUI
