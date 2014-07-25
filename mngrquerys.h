#ifndef MNGRQUERYS_H
#define MNGRQUERYS_H

#include <QtSql>
#include <QSqlQueryModel>
#include <QMap>

#include <QFile>
#include <QString>
#include <QVariant>

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}
namespace Filter {
    enum filter{all = 0, editing, wanttolook, tv, ova, ona, special, movie};
}

namespace Tables {
    enum table{none = 0, vAnime, vManga, vAmv, vDorama, tAnime, tManga, tAmv, tDorama};
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
    static int  selectSection( QSqlQueryModel* model, sections::section, QString filter );
    static int  selectSection( QSqlQueryModel* model, sections::section, Filter::filter filter );

    static bool createTable_Anime();
    static bool createTable_Manga();
    static bool createTable_Amv();
    static bool createTable_Dorama();
    static QStringList getAnimeTags();
    static QStringList getMangaTags();
    static QStringList getAmvTags();
    static QStringList getDoramaTags();

};

#endif // MNGRQUERYS_H
