#include "definespath.h"

#include <QDebug>

/// Any string(path) ended of QDir::separator()

QString DefinesPath::home()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

QString DefinesPath::log()
{
    return QStandardPaths::writableLocation( QStandardPaths::CacheLocation ) + QDir::separator() + "log" + QDir::separator();
}

QStringList DefinesPath::share()
{
    QStringList paths = QStandardPaths::standardLocations( QStandardPaths::DataLocation );
    QStringList newPaths;
    foreach (QString path, paths) {
        newPaths << path + QDir::separator();
    }
    return newPaths;
}

QString DefinesPath::appData(bool useDefault)
{
    if(useDefault)
        return QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator();
    DbaSettings s;
    return QDir(s.value(Configs::General::WorkDirectory,
                   QStandardPaths::writableLocation( QStandardPaths::DataLocation )
                   ).toString()).path() + QDir::separator();

    // Use it:
    // - dbPath()
    // - animeCovers()
    // - mangaCovers()
    // - amvCovers()
    // - doramaCovers()
}

QString DefinesPath::dbPath(bool appendFileName)
{
    if(appendFileName)
        return appData() + "DatabaseAnime.sqlite";
    else
        return appData();
}

QString DefinesPath::animeCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "animeCovers" + QDir::separator();
    return appData() + "animeCovers" + QDir::separator();
}

QString DefinesPath::mangaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "mangaCovers" + QDir::separator();
    return appData() + "mangaCovers" + QDir::separator();
}

QString DefinesPath::amvCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "amvCovers" + QDir::separator();
    return appData() + "amvCovers" + QDir::separator();
}

QString DefinesPath::doramaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "doramaCovers" + QDir::separator();
    return appData() + "doramaCovers" + QDir::separator();
}
