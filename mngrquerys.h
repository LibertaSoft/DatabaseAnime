#ifndef MNGRQUERYS_H
#define MNGRQUERYS_H

#include "globalenum.h"
#include <QtSql>
#include <QSqlQueryModel>
#include <QString>

class MngrQuerys
{
private:
    static QString filterToString(Filter::filter);
    static QString sortToString(Sort::sort);
public:
    MngrQuerys();
    static QString getTableName( Tables::table );
    static QString getTableName( sections::section );
    static int  selectSection( QSqlQueryModel* model, sections::section = sections::none );
    static int  selectSection( QSqlQueryModel* model, sections::section, QString filter, Filter::filter filter2, Sort::sort sort );
    static int  selectSection( QSqlQueryModel* model, sections::section, Filter::filter filter, Sort::sort sort );

    static bool createTable_Anime();
    static bool createTable_Manga();
    static bool createTable_Amv();
    static bool createTable_Dorama();

    static QStringList getAnimeTags();
    static QStringList getMangaTags();
    static QStringList getAmvTags();
    static QStringList getDoramaTags();

    static bool updateRecord(sections::section table, unsigned long long recoord_id, QString field, QString newValue);

    static bool insertAnime(QMap<QString, QVariant> &data);
    static bool insertManga(QMap<QString, QVariant> &data);
    static bool insertAmv(QMap<QString, QVariant> &data);
    static bool insertDorama(QMap<QString, QVariant> &data);

    static bool updateAnime(QMap<QString, QVariant> &data);
    static bool updateManga(QMap<QString, QVariant> &data);
    static bool updateAmv(QMap<QString, QVariant> &data);
    static bool updateDorama(QMap<QString, QVariant> &data);

};

#endif // MNGRQUERYS_H
