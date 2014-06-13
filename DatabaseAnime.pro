QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app

CONFIG += debug

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formsettings.cpp \
    dialogaddedit.cpp

FORMS += \
    mainwindow.ui \
    formsettings.ui \
    dialogaddedit.ui

HEADERS += \
    mainwindow.h \
    formsettings.h \
    dialogaddedit.h

RESOURCES += \
    resource.qrc