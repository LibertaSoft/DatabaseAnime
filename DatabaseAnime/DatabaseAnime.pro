QT       += core gui widgets sql xml network

TARGET = DatabaseAnime
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

VERSION = 1.4.2.4
#Last digit:
# 0 - Pre-Alpha
# 1 - Alpha
# 2 - Beta
# 3 - Release Candidate
# 4 - Release

RC_ICONS = images/DBA_Icon.ico
QMAKE_TARGET_COMPANY = LibertaSoft
QMAKE_TARGET_PRODUCT = DatabaseAnime
QMAKE_TARGET_DESCRIPTION = Your database of an anime
QMAKE_TARGET_COPYRIGHT = (CopyLeft) LibertaSoft

HEADERS += \
    mainwindow.h \
    mngrconnection.h \
    mngrquerys.h \
    dialogs/addamv.h \
    dialogs/addanime.h \
    dialogs/adddorama.h \
    dialogs/addmanga.h \
    dialogs/settings.h \
    widgets/imagecover.h \
    widgets/lookprogressbar.h \
    widgets/stackwgt_mousehideover.h \
    definespath.h \
    globalenum.h \
    share/xml/xmldbareader.h \
    share/xml/xmldbawriter.h \
    dbalocalization.h \
    shikimoriapi.h \
    widgets/colorpicker.h \
    stylemanager.h \
    imageloader.h \
    core/models/basemodel.h \
    models/animemodel.h \
    models/mangamodel.h \
    models/amvmodel.h \
    models/doramamodel.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mngrconnection.cpp \
    mngrquerys.cpp \
    dialogs/addamv.cpp \
    dialogs/addanime.cpp \
    dialogs/adddorama.cpp \
    dialogs/addmanga.cpp \
    dialogs/settings.cpp \
    widgets/imagecover.cpp \
    widgets/lookprogressbar.cpp \
    widgets/stackwgt_mousehideover.cpp \
    definespath.cpp \
    share/xml/xmldbareader.cpp \
    share/xml/xmldbawriter.cpp \
    dbalocalization.cpp \
    shikimoriapi.cpp \
    widgets/colorpicker.cpp \
    stylemanager.cpp \
    imageloader.cpp \
    core/models/basemodel.cpp \
    models/animemodel.cpp \
    models/mangamodel.cpp \
    models/amvmodel.cpp \
    models/doramamodel.cpp

FORMS += \
    mainwindow.ui \
    addamv.ui \
    addanime.ui \
    adddorama.ui \
    addmanga.ui \
    settings.ui \

OTHER_FILES += \
    ToDo.md \
    ChangeLog.md

RESOURCES += \
    resource.qrc

TRANSLATIONS += \
    DatabaseAnime_ru.ts \
    DatabaseAnime_en.ts
