#-------------------------------------------------
#
# Project created by QtCreator 2012-07-03T16:34:35
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

TARGET = ServerEnd
TEMPLATE = app


SOURCES += main.cpp\
        server_end.cpp \
    logindig.cpp \
    tcpserver.cpp \
    clientthread.cpp \
    recognitionsystem.cpp

HEADERS  += server_end.h \
    logindig.h \
    connection.h \
    tcpserver.h \
    clientthread.h \
    recognitionsystem.h

FORMS    += server_end.ui \
    logindig.ui \
    recognitionsystem.ui
