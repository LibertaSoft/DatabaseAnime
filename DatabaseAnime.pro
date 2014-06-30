QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app

CONFIG += debug

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formsettings.cpp \
    imagecover.cpp \
    lookprogressbar.cpp \
    dialogaddanime.cpp \
    mngrconnection.cpp

FORMS += \
    mainwindow.ui \
    formsettings.ui \
    dialogaddanime.ui

HEADERS += \
    mainwindow.h \
    formsettings.h \
    imagecover.h \
    lookprogressbar.h \
    dialogaddanime.h \
    mngrconnection.h

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts
