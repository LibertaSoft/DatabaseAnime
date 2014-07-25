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
    dialogaddmanga.cpp \
    mngrconnection.cpp \
    mngrquerys.cpp \
<<<<<<< HEAD
    stackwgt_mousehideover.cpp \
    dialogaddmanga.cpp \
    dialogaddamv.cpp
=======
    stackwgt_mousehideover.cpp
>>>>>>> master

FORMS += \
    mainwindow.ui \
    formsettings.ui \
    dialogaddanime.ui \
    dialogaddmanga.ui \
    dialogaddamv.ui

HEADERS += \
    mainwindow.h \
    formsettings.h \
    imagecover.h \
    lookprogressbar.h \
    dialogaddanime.h \
    dialogaddmanga.h \
    mngrconnection.h \
    mngrquerys.h \
<<<<<<< HEAD
    stackwgt_mousehideover.h \
    dialogaddmanga.h \
    dialogaddamv.h
=======
    stackwgt_mousehideover.h
>>>>>>> master

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts \
    DatabaseAnime_en.ts
