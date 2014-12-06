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
//    static uniformFields();

    static QString fieldToString(Tables::AnimeField::field field);
    static QString fieldToString(Tables::MangaField::field field);
    static QString fieldToString(Tables::AmvField::field field);
    static QString fieldToString(Tables::DoramaField::field field);
    static QString fieldToString(Tables::UniformField::field field, sections::section section);

    static Tables::AnimeField::field   animeFieldFromString(QString fieldName);
    static Tables::MangaField::field   mangaFieldFromString(QString fieldName);
    static Tables::AmvField::field       amvFieldFromString(QString fieldName);
    static Tables::DoramaField::field doramaFieldFromString(QString fieldName);

    static QMap<Tables::AnimeField::field, QVariant>  convertAnimeData(QMap<QString,QVariant> data);
    static QMap<Tables::MangaField::field, QVariant>  convertMangaData(QMap<QString,QVariant> data);
    static QMap<Tables::AmvField::field,   QVariant>    convertAmvData(QMap<QString,QVariant> data);
    static QMap<Tables::DoramaField::field,QVariant> convertDoramaData(QMap<QString,QVariant> data);

    static QString getTableName( Tables::table );
    static QString getTableName( sections::section );
    static bool selectSection(QSqlQueryModel *model, sections::section section,
                              Tables::UniformField::field field = Tables::UniformField::Title,
                              Filter::filter filter = Filter::all,
                              Sort::sort sort = Sort::none, QString search = QString::null);

    static bool createTable_Anime();
    static bool createTable_Manga();
    static bool createTable_Amv();
    static bool createTable_Dorama();

    static quint64  count(sections::section section);
    static void     count(quint64 &anime, quint64 &manga, quint64 &amv, quint64 &dorama);

    static bool dropTable(sections::section section);
    static QSqlQuery selectAll(sections::section section);
    static QString getImagePath(sections::section section, quint64 id);
    static bool deleteRecord(sections::section section, quint64 id);

    static QStringList getAnimeTags();
    static QStringList getMangaTags();
    static QStringList getAmvTags();
    static QStringList getDoramaTags();

    static QSqlRecord selectData(sections::section table, quint64 id);

    static bool updateRecord(sections::section table, quint64 recoord_id, QString field, QString newValue);

    static bool insertAnime(QMap<Tables::AnimeField::field, QVariant> data);
    static bool insertManga(QMap<Tables::MangaField::field, QVariant> data);
    static bool insertAmv(QMap<Tables::AmvField::field, QVariant> data);
    static bool insertDorama(QMap<Tables::DoramaField::field, QVariant> data);

    static bool updateAnime(QMap<Tables::AnimeField::field, QVariant> &data);
    static bool updateManga(QMap<Tables::MangaField::field, QVariant> &data);
    static bool updateAmv(QMap<Tables::AmvField::field, QVariant> &data);
    static bool updateDorama(QMap<Tables::DoramaField::field, QVariant> &data);

};

#endif // MNGRQUERYS_H
