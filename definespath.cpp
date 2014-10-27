#include "definespath.h"
#include <QApplication> // [[deprecated]]
#include <QDir>
#include <QStandardPaths>

#include <QDebug>

/// Any string(path) ended of QDir::separator()

QString DefinesPath::home()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

//[[deprecated("Use share()")]]
QString DefinesPath::rootShare()
{
    #pragma /*[[*/Deprecated/*]]*/
    return QApplication::applicationDirPath() + QDir::separator()
            + ".." + QDir::separator() + "share" + QDir::separator()
            + QApplication::organizationName() + QDir::separator()
            + QApplication::applicationName() + QDir::separator();
    // Use case:
    // - l10n()
}

QString DefinesPath::l10n()
{
    return rootShare() + "l10n" + QDir::separator();
}

QStringList DefinesPath::share()
{
    return QStandardPaths::standardLocations( QStandardPaths::DataLocation );
}

QString DefinesPath::appData(bool useDefault)
{
    if(useDefault)
        return QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator();
    QSettings s;
    return s.value("WorkDirectory",
                   QStandardPaths::writableLocation( QStandardPaths::DataLocation )
                   ).toString() + QDir::separator();

    // Use case:
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
