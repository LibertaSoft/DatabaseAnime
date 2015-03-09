#ifndef DBALOCALIZATION_H
#define DBALOCALIZATION_H

#include <QStringList>
#include <QLocale>
#include <QDir>

class DbaLocalization
{
private:
    static QString getPathToLocalizationFile(QString filePrefix, QLocale::Language, QStringList paths);
public:
    static QMap<QLocale::Language,QString> readExistsLocalizations(QStringList paths);
    static QString getFileOfLocalization(QLocale::Language, QStringList paths);
    static QString getQtBaseFileOfLocalization(QLocale::Language, QStringList paths);
};

#endif // DBALOCALIZATION_H
