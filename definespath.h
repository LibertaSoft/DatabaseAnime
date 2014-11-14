#ifndef DEFINESPATH_H
#define DEFINESPATH_H

#include "dbasettings.h"
#include <QString>
#include <QDir>
#include <QStandardPaths>

class DefinesPath
{
public:
    static QString home();
    static QStringList share();

    static QString appData(bool useDefault = false);
        static QString dbPath(bool appendFileName = false);
        static QString animeCovers(const QString prefix = QString::null);
        static QString mangaCovers(const QString prefix = QString::null);
        static QString amvCovers(const QString prefix = QString::null);
        static QString doramaCovers(const QString prefix = QString::null);
};

#endif // DEFINESPATH_H
