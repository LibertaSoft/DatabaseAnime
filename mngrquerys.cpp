#include "mngrquerys.h"

MngrQuerys::MngrQuerys()
{
}

QString MngrQuerys::getTableName(Tables::table table)
{
    switch( table ){
        case Tables::vAnime :
            return "animeSerials";
            break;
        case Tables::vManga :
            return "mangaPosters" ;
            break;
        case Tables::vAmv :
            return "amvVideos" ;
            break;
        case Tables::vDorama :
            return "doramaSerials" ;
            break;
        case Tables::none :
        default:
            return "" ;
    }
}

QString MngrQuerys::getTableName(sections::section table)
{
    switch( table ){
        case sections::anime :
            return getTableName(Tables::vAnime);
            break;
        case sections::manga :
            return getTableName(Tables::vManga) ;
            break;
        case sections::amv :
            return getTableName(Tables::vAmv) ;
            break;
        case sections::dorama :
            return getTableName(Tables::vDorama) ;
            break;
        case sections::none :
        default:
            return getTableName(Tables::none) ;
    }
}

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section)
{
    model->setQuery( QString("SELECT id,Title FROM %1").arg( getTableName(section) ) );
    return 0;
}

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section, QString filter)
{
    QSettings settings;
    Sort::sort sort = static_cast<Sort::sort>( settings.value("Sorting", Sort::asc).toInt() );

    QString strFilter("WHERE ");

    QString strSort("");
    switch( sort ){
    case Sort::asc :
        strSort = "ORDER BY Title ASC";
        break;
    case Sort::desc :
        strSort = "ORDER BY Title DESC";
        break;
    case Sort::none :
    default:
        ;
    }

    model->setQuery( QString("SELECT id,Title FROM %1 %2 %3").arg( getTableName(section), strFilter+filter, strSort ) );
    return 0;
}

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section, Filter::filter filter)
{
    QSettings settings;
    Sort::sort sort = static_cast<Sort::sort>( settings.value("Sorting", Sort::asc).toInt() );

    QString strFilter("WHERE ");
    switch( filter ){
    case Filter::editing :
        strFilter += "isEditingDone = 0";
        break;
    case Filter::wanttolook :
        strFilter += "isHaveLooked = 0";
        break;
    case Filter::tv :
        strFilter += "SeriesTV > 0";
        break;
    case Filter::ova :
        strFilter += "SeriesOVA > 0";
        break;
    case Filter::ona :
        strFilter += "SeriesONA > 0";
        break;
    case Filter::special :
        strFilter += "SeriesSpecial > 0";
        break;
    case Filter::movie :
        strFilter += "SeriesMovie > 0";
        break;
    case Filter::all :
    default:
        strFilter = "";
    }

    QString strSort("");
    switch( sort ){
    case Sort::asc :
        strSort = "ORDER BY Title ASC";
        break;
    case Sort::desc :
        strSort += "ORDER BY Title DESC";
        break;
    case Sort::none :
    default:
        ;
    }

    model->setQuery( QString("SELECT id,Title FROM %1 %2 %3").arg( getTableName(section), strFilter, strSort ) );
    return 0;
}

bool MngrQuerys::createTable_AnimeSerials()
{
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "Title          VARCHAR(128), "
                  "OrigTitle      VARCHAR(128), "
                  "Director       VARCHAR(32), "
                  "PostScoring    VARCHAR(128), "
                  "SeriesTV       INTEGER, "
                  "SeriesOVA      INTEGER, "
                  "SeriesONA      INTEGER, "
                  "SeriesSpecial  INTEGER, "
                  "SeriesMovie    INTEGER, "
                  "vSeriesTV      INTEGER, "
                  "vSeriesOVA     INTEGER, "
                  "vSeriesONA     INTEGER, "
                  "vSeriesSpecial INTEGER, "
                  "vSeriesMovie   INTEGER, "
                  "Year           INTEGER, "
                  "Season         INTEGER, "
                  "Studios        VARCHAR(48), "
                  "Tags           VARCHAR(256), "
                  "Description    TEXT, "
                  "URL            VARCHAR(256), "
                  "Dir            VARCHAR(256), "
                  "ImagePath      VARCHAR(256) "
                  ");").arg( getTableName( sections::anime ) );
    QSqlQuery query;
    if( !query.exec(sql) ){
        qDebug() << "Table animeSerials is not created! Error: " << query.lastError();
//        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("There was a mistake at table creation in a database.") );
        return false;
    }
    return true;
}

QStringList MngrQuerys::getAnimeTags()
{
    return QStringList()
            << QObject::tr("Action",			"1.1")	<< QObject::tr("Futunari",			"1.2")	<< QObject::tr("Music",				"1.3")	<< QObject::tr("Shounen",			"1.4")
            << QObject::tr("Adventure",			"2.1")	<< QObject::tr("Game",				"2.2")	<< QObject::tr("Mystery",			"2.3")	<< QObject::tr("Shounen-ai (Yaoi)",	"2.4")
            << QObject::tr("Bara",				"3.1")	<< QObject::tr("Gender intrigue",	"3.2")	<< QObject::tr("Parody",			"3.3")	<< QObject::tr("Space",				"3.4")
            << QObject::tr("Comedy",			"4.1")	<< QObject::tr("Guro",				"4.2")	<< QObject::tr("Philosophy",		"4.3")	<< QObject::tr("Sport",				"4.4")
            << QObject::tr("Cooking",			"5.1")	<< QObject::tr("Harem",				"5.2")	<< QObject::tr("Police",			"5.3")	<< QObject::tr("Steampunk",			"5.4")
            << QObject::tr("Cyberpunk",			"6.1")	<< QObject::tr("Hentai",			"6.2")	<< QObject::tr("Post-apocalyptic",	"6.3")	<< QObject::tr("Supernatural",		"6.4")
            << QObject::tr("Daily occurrence",	"7.1")	<< QObject::tr("Historical",		"7.2")	<< QObject::tr("Psychology",		"7.3")	<< QObject::tr("Tale",				"7.4")
            << QObject::tr("Demons",			"8.1")	<< QObject::tr("Horror",			"8.2")	<< QObject::tr("Ranobe",			"8.3")	<< QObject::tr("Tentacles",			"8.4")
            << QObject::tr("Detective",			"9.1")	<< QObject::tr("Josei",				"9.2")	<< QObject::tr("Romantic",			"9.3")	<< QObject::tr("The children's",	"9.4")
            << QObject::tr("Drama",				"10.1")	<< QObject::tr("Kodomo",			"10.2")	<< QObject::tr("Samurai",			"10.3")	<< QObject::tr("Thriller",			"10.4")
            << QObject::tr("Ecchi",				"11.1")	<< QObject::tr("Madness",			"11.2")	<< QObject::tr("School",			"11.3")	<< QObject::tr("Tragedy",			"11.4")
            << QObject::tr("Family",			"12.1")	<< QObject::tr("Magic",				"12.2")	<< QObject::tr("Science Fiction",	"12.3")	<< QObject::tr("Vampires",			"12.4")
            << QObject::tr("Fantastic",			"13.1")	<< QObject::tr("Magical girl",		"13.2")	<< QObject::tr("Seinen",			"13.3")	<< QObject::tr("War",				"13.4")
            << QObject::tr("Fantasy",			"14.1")	<< QObject::tr("Martial Arts",		"14.2")	<< QObject::tr("Shojo",				"14.3")	<< QObject::tr("Yonkoma",			"14.4")
            << QObject::tr("Fighter",			"15.1")	<< QObject::tr("Mech",				"15.2")	<< QObject::tr("Shojo-ai (Yuri)",	"15.3");
}

QStringList MngrQuerys::getMangaTags()
{
    return QStringList()
            << QObject::tr("Action",			"1.1")	<< QObject::tr("Futunari",			"1.2")	<< QObject::tr("Music",				"1.3")	<< QObject::tr("Shounen",			"1.4")
            << QObject::tr("Adventure",			"2.1")	<< QObject::tr("Game",				"2.2")	<< QObject::tr("Mystery",			"2.3")	<< QObject::tr("Shounen-ai (Yaoi)",	"2.4")
            << QObject::tr("Bara",				"3.1")	<< QObject::tr("Gender intrigue",	"3.2")	<< QObject::tr("Parody",			"3.3")	<< QObject::tr("Space",				"3.4")
            << QObject::tr("Comedy",			"4.1")	<< QObject::tr("Guro",				"4.2")	<< QObject::tr("Philosophy",		"4.3")	<< QObject::tr("Sport",				"4.4")
            << QObject::tr("Cooking",			"5.1")	<< QObject::tr("Harem",				"5.2")	<< QObject::tr("Police",			"5.3")	<< QObject::tr("Steampunk",			"5.4")
            << QObject::tr("Cyberpunk",			"6.1")	<< QObject::tr("Hentai",			"6.2")	<< QObject::tr("Post-apocalyptic",	"6.3")	<< QObject::tr("Supernatural",		"6.4")
            << QObject::tr("Daily occurrence",	"7.1")	<< QObject::tr("Historical",		"7.2")	<< QObject::tr("Psychology",		"7.3")	<< QObject::tr("Tale",				"7.4")
            << QObject::tr("Demons",			"8.1")	<< QObject::tr("Horror",			"8.2")	<< QObject::tr("Ranobe",			"8.3")	<< QObject::tr("Tentacles",			"8.4")
            << QObject::tr("Detective",			"9.1")	<< QObject::tr("Josei",				"9.2")	<< QObject::tr("Romantic",			"9.3")	<< QObject::tr("The children's",	"9.4")
            << QObject::tr("Drama",				"10.1")	<< QObject::tr("Kodomo",			"10.2")	<< QObject::tr("Samurai",			"10.3")	<< QObject::tr("Thriller",			"10.4")
            << QObject::tr("Ecchi",				"11.1")	<< QObject::tr("Madness",			"11.2")	<< QObject::tr("School",			"11.3")	<< QObject::tr("Tragedy",			"11.4")
            << QObject::tr("Family",			"12.1")	<< QObject::tr("Magic",				"12.2")	<< QObject::tr("Science Fiction",	"12.3")	<< QObject::tr("Vampires",			"12.4")
            << QObject::tr("Fantastic",			"13.1")	<< QObject::tr("Magical girl",		"13.2")	<< QObject::tr("Seinen",			"13.3")	<< QObject::tr("War",				"13.4")
            << QObject::tr("Fantasy",			"14.1")	<< QObject::tr("Martial Arts",		"14.2")	<< QObject::tr("Shojo",				"14.3")	<< QObject::tr("Yonkoma",			"14.4")
            << QObject::tr("Fighter",			"15.1")	<< QObject::tr("Mech",				"15.2")	<< QObject::tr("Shojo-ai (Yuri)",	"15.3");
}

QStringList MngrQuerys::getAmvTags()
{
    return QStringList()
            << QObject::tr("Action",			"1.1")	<< QObject::tr("Drama",			"1.2")	<< QObject::tr("M@D",				"1.3")	<< QObject::tr("Romance",			"1.4")
            << QObject::tr("Character Profile",	"2.1")	<< QObject::tr("Ecchi",			"2.2")	<< QObject::tr("MEP",				"2.3")	<< QObject::tr("Sentimental",		"2.4")
            << QObject::tr("Comedy (Fun)",		"3.1")	<< QObject::tr("GMV",			"3.2")	<< QObject::tr("MMV",				"3.3")	<< QObject::tr("Story",				"3.4")
            << QObject::tr("Cross-Over",		"4.1")	<< QObject::tr("Horror",		"4.2")	<< QObject::tr("Original Animation","4.3")	<< QObject::tr("Trailer",			"4.4")
            << QObject::tr("Dance",				"5.1")	<< QObject::tr("Instrumental",	"5.2")	<< QObject::tr("Psychedelic",		"5.3");
}

QStringList MngrQuerys::getDoramaTags()
{
    return QStringList()
            << QObject::tr("Adventure",			"1.1")	<< QObject::tr("Friendship",		"1.2")	<< QObject::tr("Policy",			"1.3")	<< QObject::tr("Vampires",			"1.4")
            << QObject::tr("Art-house",			"2.1")	<< QObject::tr("Game",				"2.2")	<< QObject::tr("Psychology",		"2.3")	<< QObject::tr("Western",			"2.4")
            << QObject::tr("Biography",			"3.1")	<< QObject::tr("Gay theme",			"3.2")	<< QObject::tr("Relationship",		"3.3")
            << QObject::tr("Business",			"4.1")	<< QObject::tr("Gender intrigue",	"4.2")	<< QObject::tr("Romance",			"4.3")
            << QObject::tr("Comedy",			"5.1")	<< QObject::tr("Historical",		"5.2")	<< QObject::tr("School",			"5.3")
            << QObject::tr("Crime",				"6.1")	<< QObject::tr("Horror",			"6.2")	<< QObject::tr("Sci-Fi",			"6.3")
            << QObject::tr("Daily occurrence",	"7.1")	<< QObject::tr("Lesbian theme",		"7.2")	<< QObject::tr("Sitcom",			"7.3")
            << QObject::tr("Detective",			"8.1")	<< QObject::tr("Live-action",		"8.2")	<< QObject::tr("Sport",				"8.3")
            << QObject::tr("Documentary",		"9.1")	<< QObject::tr("Martial Arts",		"9.2")	<< QObject::tr("Suspense",			"9.3")
            << QObject::tr("Drama",				"10.1")	<< QObject::tr("Medicine",			"10.2")	<< QObject::tr("Taiga",				"10.3")
            << QObject::tr("Erotic",			"11.1")	<< QObject::tr("Melodrama",			"11.2")	<< QObject::tr("Tale",				"11.3")
            << QObject::tr("Family",			"12.1")	<< QObject::tr("Military",			"12.2")	<< QObject::tr("Thriller",			"12.3")
            << QObject::tr("Fantastic",			"13.1")	<< QObject::tr("Musical",			"13.2")	<< QObject::tr("Tokusatsu",			"13.3")
            << QObject::tr("Fantasy",			"14.1")	<< QObject::tr("Mysticism",			"14.2")	<< QObject::tr("Tragedy",			"14.3")
            << QObject::tr("Fighter",			"15.1")	<< QObject::tr("Parody",			"15.2")	<< QObject::tr("TV Show",			"15.3");
}
