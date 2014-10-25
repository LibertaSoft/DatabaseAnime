#ifndef DEFINESPATH_H
#define DEFINESPATH_H

#include <QString>
#include <QSettings>

class DefinesPath
{
public:
    static QString home();
    static QStringList share();
    static QString rootShare();
        static QString l10n();

    static QString appData(bool useDefault = false);
        static QString dbPath(bool appendFileName = false);
        static QString animeCovers(const QString prefix = QString::null);
        static QString mangaCovers(const QString prefix = QString::null);
        static QString amvCovers(const QString prefix = QString::null);
        static QString doramaCovers(const QString prefix = QString::null);



};

#endif // DEFINESPATH_H
