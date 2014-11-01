#-------------------------------------------------
#
# Project created by QtCreator 2014-11-01T18:15:49
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = test_dbalocalization
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += test_dbalocalization.cpp \
           ../../dbalocalization.cpp
HEADERS += ../../dbalocalization.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"
