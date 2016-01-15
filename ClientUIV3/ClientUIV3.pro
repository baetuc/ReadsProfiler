#-------------------------------------------------
#
# Project created by QtCreator 2016-01-08T14:23:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientUIV3
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    Reader.cpp \
    Socket.cpp \
    Author.cpp \
    Book.cpp \
    Creation.cpp \
    Rating.cpp \
    SerializerDeserializer.cpp \
    User.cpp \
    Utility.cpp \
    dialog.cpp \
    search.cpp \
    view.cpp \
    SearchInfo.cpp

HEADERS  += mainwindow.h \
    Reader.h \
    Socket.h \
    Author.h \
    Book.h \
    Creation.h \
    Rating.h \
    SerializerDeserializer.h \
    User.h \
    Utility.h \
    dialog.h \
    search.h \
    view.h \
    SearchInfo.h

FORMS    += mainwindow.ui \
    dialog.ui \
    search.ui \
    view.ui \
