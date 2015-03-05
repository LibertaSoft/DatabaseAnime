#-------------------------------------------------
#
# Project created by QtCreator 2014-11-01T16:12:20
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = Test_DefinesPath
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../

SOURCES += \
          test_definespath.cpp \
    ../../definespath.cpp
HEADERS += \
    ../../definespath.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"
