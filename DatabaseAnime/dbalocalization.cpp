#include "dbalocalization.h"

#include <QDebug>

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

QString DbaLocalization::getFileOfLocalization(QLocale::Language lang, QStringList paths)
{
    return getPathToLocalizationFile("DatabaseAnime", lang, paths);
}

QString DbaLocalization::getQtBaseFileOfLocalization(QLocale::Language lang, QStringList paths)
{
    if( lang == QLocale::English )
        return QString::null;

    return getPathToLocalizationFile("qtbase", lang, paths);
}
