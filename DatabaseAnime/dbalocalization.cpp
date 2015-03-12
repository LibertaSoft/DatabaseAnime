#include "dbalocalization.h"

#include <QDebug>

/*! \~russian
 * \brief Статический метод для получения пути до файла локализации
 * \param filePrefix - префикс имени файла 'DatabaseAnime_'ru.qm
 * \param lang - требуемый язык
 * \param paths - пути для поиска файлов
 * \return строка, абсолютный путь к файлу локализации или QString::null если локализация не найдена
 *
 * Метод позволяет получить абсолютный путь к первому попавшемуся, соответствующиму файлу локализации.
 */
/*! \~english
 * \brief Static method for receiving a path to the file of localization
 * \param filePrefix - file name prefix 'DatabaseAnime_'ru.qm
 * \param lang - the required language
 * \param paths - for search of files
 * \return line, absolute path to the file of localization or QString:: null if localization isn't found
 *
 * The method allows to receive an absolute path to the first, appropriate file of localization.
 */
QString DbaLocalization::getPathToLocalizationFile(QString filePrefix, QLocale::Language lang, QStringList paths)
{
    if( paths.isEmpty() ){
        qCritical() << "Empty paths to localization";
        return QString::null;
    }

    foreach (QString path, paths) {
        QString l10n = QDir(path).path() + QDir::separator() + "l10n" + QDir::separator();
        foreach(QString fileName, QDir(l10n).entryList( QStringList(filePrefix+"*")) ){
            QString fullFileName = fileName;
            fileName.replace(filePrefix+"_", "").replace(".qm", "");
            QLocale::Language language = QLocale(fileName).language();
            if( language == lang ){
                return l10n + fullFileName;
            }
        }
    }

    qWarning() << filePrefix + " localization " + QLocale::languageToString(lang) + " not found";
    return QString::null;
}

/*! \~russian
 * \brief Статический метод предназначен для получения списка всех доступных локализаций.
 * \param paths - список директорий в которых искать файлы локализации
 * \return Ассоциативный массив код-строка, где код - код локализации, а строка - язык.
 */
/*! \~english
 * \brief The static method is intended for obtaining the list of all available localizations.
 * \param paths - the list of directories in which to look for files of localization
 * \return The associative array a code-string, where a code - a localization code, and a string - language.
 */
QMap<QLocale::Language,QString> DbaLocalization::readExistsLocalizations(QStringList paths)
{
    QMap<QLocale::Language,QString> localizationList;
    if( paths.isEmpty() ){
        qCritical() << "Empty paths to localization";
        return localizationList;
    }

    foreach (QString path, paths) {
        QString l10n = QDir(path).path() + QDir::separator() + "l10n" + QDir::separator();
        foreach(QString fileName, QDir(l10n).entryList( QStringList("DatabaseAnime*")) ){
            fileName.replace("DatabaseAnime_", "").replace(".qm", "");
            QLocale::Language language = QLocale(fileName).language();
            localizationList.insert(language, QLocale::languageToString( language ) );
        }
    }

    return localizationList;
}

/*! \~russian
 * \brief Интерфейс для метода получения файла локализации.
 * \param lang - требуемый язык.
 * \param paths - список директорий для поиска файлов локализации.
 * \return строка - абсолютный путь к файлу локализации.
 *
 * Вызывает метод получения пути к файлу локализации с префиксом - "DatabaseAnime"
 */
/*! \~english
 * \brief The interface for a method of obtaining the file of localization.
 * \param lang - the required language.
 * \param paths - the list of directories for search of files of localization.
 * \return string - an absolute path to the file of localization.
 *
 * Calls a method of receiving a way to the file of localization with a prefix - "DatabaseAnime"
 */
QString DbaLocalization::getFileOfLocalization(QLocale::Language lang, QStringList paths)
{
    return getPathToLocalizationFile("DatabaseAnime", lang, paths);
}

/*! \~russian
 * \brief Интерфейс для метода получения файла локализации.
 * \param lang - требуемый язык.
 * \param paths - список директорий для поиска файлов локализации.
 * \return строка - абсолютный путь к файлу локализации или QString::null
 *
 * Вызывает метод получения пути к файлу локализации с префиксом - "qtbase"
 * Возвращает QString::null если запрашивается локализация QLocale::English
 * в связи с тем что данная локализация вшита по умолчинию.
 */
/*! \~english
 * \brief The interface for a method of obtaining the file of localization.
 * \param lang - the required language.
 * \param paths - the list of directories for search of files of localization.
 * \return string - an absolute path to the file of localization.
 *
 * Calls a method of receiving a way to the file of localization with a prefix - "qtbase"
 * Returns QString:: null if is requested localization of QLocale:: English
 * because this localization is sewed on an default.
 */
QString DbaLocalization::getQtBaseFileOfLocalization(QLocale::Language lang, QStringList paths)
{
    if( lang == QLocale::English )
        return QString::null;

    return getPathToLocalizationFile("qtbase", lang, paths);
}
