QT       += core gui sql #svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app
#CONFIG += console

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formsettings.cpp \
    imagecover.cpp \
    lookprogressbar.cpp \
    dialogaddanime.cpp \
    mngrconnection.cpp \
    mngrquerys.cpp \
    stackwgt_mousehideover.cpp \
    dialogaddmanga.cpp

FORMS += \
    mainwindow.ui \
    formsettings.ui \
    dialogaddanime.ui \
    dialogaddmanga.ui

HEADERS += \
    mainwindow.h \
    formsettings.h \
    imagecover.h \
    lookprogressbar.h \
    dialogaddanime.h \
    mngrconnection.h \
    mngrquerys.h \
    stackwgt_mousehideover.h \
    dialogaddmanga.h

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts \
    DatabaseAnime_en.ts
