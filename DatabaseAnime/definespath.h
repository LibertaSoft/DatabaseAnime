#ifndef DEFINESPATH_H
#define DEFINESPATH_H

#include <QString>
#include <QDir>
#include <QStandardPaths>
#include "globalenum.h"

class DefinesPath
{
public:
    static QString home();
    static QString log();
    static QStringList share();

    static QString appData(bool useDefault = false);
        static QString dbPath(bool appendFileName = false);
        static QString styleLocation();
        static QString getCoversFolder(const sections::section section, const QString prefix = QString::null);
        static QString animeCovers(const QString prefix = QString::null);
        static QString mangaCovers(const QString prefix = QString::null);
        static QString amvCovers(const QString prefix = QString::null);
        static QString doramaCovers(const QString prefix = QString::null);
};

#endif // DEFINESPATH_H
