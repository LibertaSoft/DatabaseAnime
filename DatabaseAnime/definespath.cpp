#include "definespath.h"
#include "globalenum.h"

#include <QSettings>
#include <QDebug>

/// Any string(path) ended of QDir::separator()

/// QString DefinesPath::home()
/// Returned a path to writable location a user home directory
QString DefinesPath::home()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

/// QString DefinesPath::log()
/// Returned a path to writable location a log-files of program
QString DefinesPath::log()
{
    return QStandardPaths::writableLocation( QStandardPaths::CacheLocation ) + QDir::separator() + "log" + QDir::separator();
}

/// QStringList DefinesPath::share()
/// Returned a list-paths to standard location a data-files
QStringList DefinesPath::share()
{
    QStringList paths = QStandardPaths::standardLocations( QStandardPaths::DataLocation );
    QStringList newPaths;
    foreach (QString path, paths) {
        newPaths << path + QDir::separator();
    }
    return newPaths;
}

/// QString DefinesPath::appData(bool useDefault)
/// Returned a path to writable location an application data
/// if useDefault is true - returned a default, system location
/// else returned a user settings location
QString DefinesPath::appData(bool useDefault)
{
    if(useDefault)
        return QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator();
    QSettings settings;
    return QDir(settings.value(Options::General::WorkDirectory,
                   QStandardPaths::writableLocation( QStandardPaths::DataLocation )
                   ).toString()).path() + QDir::separator();

    // Use in:
    // - dbPath()
    // - animeCovers()
    // - mangaCovers()
    // - amvCovers()
    // - doramaCovers()
}

/// QString DefinesPath::dbPath(bool appendFileName)
/// Returned a path to database file
/// if appendFileName is true - appended a file name
QString DefinesPath::dbPath(bool appendFileName)
{
    if(appendFileName)
        return appData() + "DatabaseAnime.sqlite";
    else
        return appData();
}

QString DefinesPath::styleLocation()
{
    return QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator() + "Style" + QDir::separator();
}

QString DefinesPath::getCoversFolder(const sections::section section, const QString prefix)
{
    switch (section) {
        case sections::anime:
            return animeCovers(prefix);
        case sections::manga:
            return mangaCovers(prefix);
        case sections::amv:
            return amvCovers(prefix);
        case sections::dorama:
            return doramaCovers(prefix);
        case sections::none :
        default:
            return QString::null;
    }
}

/// QString DefinesPath::animeCovers(const QString prefix)
/// Returned a path to anime covers
/// if prefix is not QString::null - used a prefix
/// else used a application data as prefix
QString DefinesPath::animeCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "animeCovers" + QDir::separator();
    return appData() + "animeCovers" + QDir::separator();
}

/// QString DefinesPath::mangaCovers(const QString prefix)
/// Returned a path to manga covers
/// if prefix is not QString::null - used a prefix
/// else used a application data as prefix
QString DefinesPath::mangaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "mangaCovers" + QDir::separator();
    return appData() + "mangaCovers" + QDir::separator();
}

/// QString DefinesPath::amvCovers(const QString prefix)
/// Returned a path to amv covers
/// if prefix is not QString::null - used a prefix
/// else used a application data as prefix
QString DefinesPath::amvCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "amvCovers" + QDir::separator();
    return appData() + "amvCovers" + QDir::separator();
}

/// QString DefinesPath::doramaCovers(const QString prefix)
/// Returned a path to dorama covers
/// if prefix is not QString::null - used a prefix
/// else used a application data as prefix
QString DefinesPath::doramaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "doramaCovers" + QDir::separator();
    return appData() + "doramaCovers" + QDir::separator();
}
