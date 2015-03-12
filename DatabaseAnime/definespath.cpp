#include "definespath.h"
#include "globalenum.h"

#include <QSettings>
#include <QDebug>

/// Any string(path) ended of QDir::separator()

/// QString DefinesPath::home()
///
/*! \~russian
 * \brief Метод для получения пути к домашней директории
 * \return QString - путь к домашней директории пользователя
 */
/*! \~english
 * \brief Method for receiving a path to a house directory
 * \return QString - path to writable location a user home directory
 */
QString DefinesPath::home()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

/*! \~russian
 * \brief Метод для получения пути к директории с лог-файлами
 * \return QString - путь к доступной для записи директории предназначенной
 * для хранения логов
 */
/*! \~english
 * \brief Method for receiving a way to a directory with log files
 * \return QString - path to writable location a log-files of program
 */
QString DefinesPath::log()
{
    return QStandardPaths::writableLocation( QStandardPaths::CacheLocation ) + QDir::separator() + "log" + QDir::separator();
}

/*! \~russian
 * \brief Метод для получения списка директорий с данными приложения
 * \return QStringList - список директорий с данными приложения
 */
/*! \~english
 * \brief Method for obtaining the list of dataful directories of application
 * \return QStringList - list of standard location of application data files
 */
QStringList DefinesPath::share()
{
    QStringList paths = QStandardPaths::standardLocations( QStandardPaths::DataLocation );
    QStringList newPaths;
    foreach (QString path, paths) {
        newPaths << path + QDir::separator();
    }
    return newPaths;
}

/*! \~russian
 * \brief Метод для получения пути к пользовательской директории с данными приложения
 * \param useDefault - использовать путь по умолчанию
 * \return QString - путь к доступной для записи директории приложения
 * если useDefault == true - возвращается стандартное значение
 * иначе возвращется значение указанное в настройках пользователя
 */
/*! \~english
 * \brief Method for receiving a path to an application dataful directory
 * \param useDefault - use default path
 * \return QString - path to writable location an application data
 * if useDefault is true - returned a default, system location
 * else returned a user settings location
 */
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

/*! \~russian
 * \brief Метод для получения пути к файлу базы данных
 * \param appendFileName - добавить к возвращаемому значению имя файла базы данных
 * \return QString - путь к файлу быза данных
 * если appendFileName равен true - добавляется имя файла
 */
/*! \~english
 * \brief Method for receiving a path to the file of a database
 * \param appendFileName - append to return value a database file name
 * \return QString - path to database file
 * if appendFileName is true - appended a file name
 */
QString DefinesPath::dbPath(bool appendFileName)
{
    if(appendFileName)
        return appData() + "DatabaseAnime.sqlite";
    else
        return appData();
}

/*! \~russian
 * \brief Метод для получения доступной для записи директории с файлами стилей
 * \return QString - доступная для записи директория
 */
/*! \~english
 * \brief Method for receiving a directory, available to record, with files of styles
 * \return QString - directory, available to write
 */
QString DefinesPath::styleLocation()
{
    return QStandardPaths::writableLocation( QStandardPaths::DataLocation ) + QDir::separator() + "Style" + QDir::separator();
}

/*! \~russian
 * \brief Интерфейс для получения директории с обложками для указанного раздела
 * \param section - требуемый раздел
 * \param prefix - использовать как путь
 * \return QString - путь к обложкам указанного раздела
 */
/*! \~english
 * \brief The interface for receiving a directory with covers for the specified section
 * \param section - the required section
 * \param prefix - to use as a path
 * \return QString - a path to covers of the specified section
 */
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

/*! \~russian
 * \brief Метод для получения пути к директории с файлами обложек аниме
 * \param prefix - использовать как путь
 * \return Путь к директории с обложками аниме
 * Если префикс не QString::null - используется путь(prefix) из параметра
 * иниче используется appData() как prefix
 */
/*! \~english
 * \brief Path to anime covers directory
 * \param prefix - use it is as path
 * \return path to anime covers
 * if prefix is not QString::null - used a prefix
 * else used a appData() as prefix
 */
QString DefinesPath::animeCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "animeCovers" + QDir::separator();
    return appData() + "animeCovers" + QDir::separator();
}

/*! \~russian
 * \brief Метод для получения пути к директории с файлами обложек манги
 * \param prefix - использовать как путь
 * \return Путь к директории с обложками манги
 * Если префикс не QString::null - используется путь(prefix) из параметра
 * иниче используется appData() как prefix
 */
/*! \~english
 * \brief Path to manga covers directory
 * \param prefix - use it is as path
 * \return path to manga covers
 * if prefix is not QString::null - used a prefix
 * else used a appData() as prefix
 */
QString DefinesPath::mangaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "mangaCovers" + QDir::separator();
    return appData() + "mangaCovers" + QDir::separator();
}

/*! \~russian
 * \brief Метод для получения пути к директории с файлами обложек amv
 * \param prefix - использовать как путь
 * \return Путь к директории с обложками amv
 * Если префикс не QString::null - используется путь(prefix) из параметра
 * иниче используется appData() как prefix
 */
/*! \~english
 * \brief Path to amv covers directory
 * \param prefix - use it is as path
 * \return path to amv covers
 * if prefix is not QString::null - used a prefix
 * else used a appData() as prefix
 */
QString DefinesPath::amvCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "amvCovers" + QDir::separator();
    return appData() + "amvCovers" + QDir::separator();
}

/*! \~russian
 * \brief Метод для получения пути к директории с файлами обложек дорамы
 * \param prefix - использовать как путь
 * \return Путь к директории с обложками дорамы
 * Если префикс не QString::null - используется путь(prefix) из параметра
 * иниче используется appData() как prefix
 */
/*! \~english
 * \brief Path to dorama covers directory
 * \param prefix - use it is as path
 * \return path to dorama covers
 * if prefix is not QString::null - used a prefix
 * else used a appData() as prefix
 */
QString DefinesPath::doramaCovers(const QString prefix)
{
    if(prefix != QString::null)
        return QDir(prefix).path() + QDir::separator() + "doramaCovers" + QDir::separator();
    return appData() + "doramaCovers" + QDir::separator();
}
