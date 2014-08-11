#ifndef MNGRQUERYS_H
#define MNGRQUERYS_H

#include <QtSql>
#include <QSqlQueryModel>
#include <QString>

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}
namespace Filter {
    enum filter{all = 0, editing, wanttolook, tv, ova, ona, special, movie, looked};
}

namespace Tables {
    enum table{none = 0, vAnime, vManga, vAmv, vDorama};
}

namespace Sort{
    enum sort{none = 0, asc, desc};
}

class MngrQuerys
{
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
    static QString getAnimeCoversPath();
    static QString getMangaCoversPath();
    static QString getAmvCoversPath();
    static QString getDoramaCoversPath();

    static bool updateRecord(sections::section table, unsigned int recoord_id, QString field, QString newValue);
    static bool insertAnime(QMap<QString, QString> &data);

};

#endif // MNGRQUERYS_H
