#ifndef MNGRQUERYS_H
#define MNGRQUERYS_H

#include "globalenum.h"
#include <QtSql>
#include <QSqlQueryModel>
#include <QString>

namespace Tables{
    namespace UniformField{
        enum field{All, id, Title, AltTitle, Url, Dir, ImagePath};
    }
    namespace Anime{
        const QString TableName("animeSerials");
        namespace Fields{
            const QString All("*");
            const QString id("id");
            const QString Description("Description");
            const QString Dir("Dir");
            const QString Director("Director");
            const QString ImagePath("ImagePath");
            const QString isAdult("isAdult");
            const QString isEditingDone("isEditingDone");
            const QString isHaveLooked("isHaveLooked");
            const QString AltTitle("OrigTitle");
            const QString PostScoring("PostScoring");
            const QString Score("Score");
            const QString Season("Season");
            const QString SeriesMovie("SeriesMovie");
            const QString SeriesONA("SeriesONA");
            const QString SeriesOVA("SeriesOVA");
            const QString SeriesSpecial("SeriesSpecial");
            const QString SeriesTV("SeriesTV");
            const QString Studios("Studios");
            const QString Tags("Tags");
            const QString Title("Title");
            const QString Url("URL");
            const QString vSeriesMovie("vSeriesMovie");
            const QString vSeriesONA("vSeriesONA");
            const QString vSeriesOVA("vSeriesOVA");
            const QString vSeriesSpecial("vSeriesSpecial");
            const QString vSeriesTV("vSeriesTV");
            const QString Year("Year");
        }
    }
    namespace Manga{
        const QString TableName("mangaPosters");
        namespace Fields{
            const QString All("*");
            const QString id("id");
            const QString Description("Description");
            const QString Dir("Dir");
            const QString Author("Author");
            const QString ImagePath("ImagePath");
            const QString isAdult("isAdult");
            const QString isEditingDone("isEditingDone");
            const QString isHaveLooked("isHaveLooked");
            const QString AltTitle("AltTitle");
            const QString Translation("Translation");
            const QString Score("Score");
            const QString Ch("Ch");
            const QString Vol("Vol");
            const QString Pages("Pages");
            const QString vCh("vCh");
            const QString vVol("vVol");
            const QString vPages("vPages");
            const QString Tags("Tags");
            const QString Title("Title");
            const QString Url("URL");
            const QString Year("Year");
        }
    }
    namespace Amv{
        const QString TableName("amvVideos");
        namespace Fields{
            const QString All("*");
            const QString id("id");
            const QString AuthorComment("AuthorComment");
            const QString Dir("Dir");
            const QString Author("Author");
            const QString ImagePath("ImagePath");
            const QString isAdult("isAdult");
            const QString isEditingDone("isEditingDone");
            const QString ContainingMusic("ContainingMusic");
            const QString ContainingAnime("ContainingAnime");
            const QString Score("Score");
            const QString Contestant("Contestant");
            const QString Tags("Tags");
            const QString Title("Title");
            const QString Url("URL");
            const QString Year("Year");
        }
    }
    namespace Dorama{
        const QString TableName("doramaSerials");
        namespace Fields{
            const QString All("*");
            const QString id("id");
            const QString Description("Description");
            const QString Dir("Dir");
            const QString Director("Director");
            const QString ImagePath("ImagePath");
            const QString isAdult("isAdult");
            const QString isEditingDone("isEditingDone");
            const QString isHaveLooked("isHaveLooked");
            const QString AltTitle("AltTitle");
            const QString Actors("Actors");
            const QString Score("Score");
            const QString Season("Season");
            const QString SeriesMovie("SeriesMovie");
            const QString SeriesSpecial("SeriesSpecial");
            const QString SeriesTV("SeriesTV");
            const QString Tags("Tags");
            const QString Title("Title");
            const QString Url("URL");
            const QString vSeriesMovie("vSeriesMovie");
            const QString vSeriesSpecial("vSeriesSpecial");
            const QString vSeriesTV("vSeriesTV");
            const QString Year("Year");
        }
    }
}


class MngrQuerys
{
private:
    static QString filterToString(Filter::filter);
    static QString sortToString(Sort::sort);

public:
    static QString fieldToString(Tables::UniformField::field field, sections::section section);

    static QString getTableName( sections::section );
    static bool selectSection(QSqlQueryModel *model, sections::section section,
                              Tables::UniformField::field field,
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

    static QSqlRecord  selectData(sections::section table, quint64 id);
    static QStringList selectUnWatched(sections::section table);

    static bool updateRecord(sections::section table, quint64 recoord_id, QString field, QString newValue);

    static bool insertAnime(QMap<QString, QVariant> data);
    static bool insertManga(QMap<QString, QVariant> data);
    static bool insertAmv(QMap<QString, QVariant> data);
    static bool insertDorama(QMap<QString, QVariant> data);

    static bool updateAnime(QMap<QString, QVariant> &data);
    static bool updateManga(QMap<QString, QVariant> &data);
    static bool updateAmv(QMap<QString, QVariant> &data);
    static bool updateDorama(QMap<QString, QVariant> &data);

};

#endif // MNGRQUERYS_H
