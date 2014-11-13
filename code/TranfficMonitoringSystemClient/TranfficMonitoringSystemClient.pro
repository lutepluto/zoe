#-------------------------------------------------
#
# Project created by QtCreator 2012-07-02T16:09:14
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql

TARGET = TranfficMonitoringSystemClient
TEMPLATE = app


SOURCES += main.cpp\
        clientwindow.cpp \
    tcpclient.cpp \
    networkoptionwindow.cpp \
    logindig.cpp

HEADERS  += clientwindow.h \
    tcpclient.h \
    networkoptionwindow.h \
    logindig.h \
    connection.h

FORMS    += clientwindow.ui \
    networkoptionwindow.ui \
    logindig.ui
