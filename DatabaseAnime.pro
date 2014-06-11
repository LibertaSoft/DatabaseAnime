QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app

CONFIG += debug

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formsettings.cpp

FORMS += \
    mainwindow.ui \
    formsettings.ui

HEADERS += \
    mainwindow.h \
    formsettings.h

RESOURCES += \
    resource.qrc
