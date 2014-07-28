QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseAnime
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    formsettings.cpp \
    imagecover.cpp \
    lookprogressbar.cpp \
    dialogaddanime.cpp \
    dialogaddmanga.cpp \
    dialogaddamv.cpp \
    dialogadddorama.cpp \
    mngrconnection.cpp \
    mngrquerys.cpp \
    stackwgt_mousehideover.cpp

FORMS += \
    mainwindow.ui \
    formsettings.ui \
    dialogaddanime.ui \
    dialogaddmanga.ui \
    dialogaddamv.ui \
    dialogadddorama.ui

HEADERS += \
    mainwindow.h \
    formsettings.h \
    imagecover.h \
    lookprogressbar.h \
    dialogaddanime.h \
    dialogaddmanga.h \
    dialogaddamv.h \
    dialogadddorama.h \
    mngrconnection.h \
    mngrquerys.h \
    stackwgt_mousehideover.h

OTHER_FILES += \
    README.md \
    ToDo.md

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts \
    DatabaseAnime_en.ts
