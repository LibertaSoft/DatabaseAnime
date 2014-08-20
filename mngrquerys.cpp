#include "mngrquerys.h"
#include <QApplication>

QString MngrQuerys::filterToString(Filter::filter filter)
{
    switch( filter ){
    case Filter::editing :
        return "isEditingDone = 0";
    case Filter::wanttolook :
        return "isHaveLooked = 0";
    case Filter::tv :
        return "SeriesTV > 0";
    case Filter::ova :
        return "SeriesOVA > 0";
    case Filter::ona :
        return "SeriesONA > 0";
    case Filter::special :
        return "SeriesSpecial > 0";
    case Filter::movie :
        return "SeriesMovie > 0";
    case Filter::looked :
        return "SeriesTV != 0 AND SeriesTV = vSeriesTV";
    case Filter::all :
    default:
        return "0 = 0";
    }
}

QString MngrQuerys::sortToString(Sort::sort sort)
{
    switch( sort ){
    case Sort::asc :
        return "ORDER BY Title ASC";
    case Sort::desc :
        return "ORDER BY Title DESC";
    case Sort::year :
        return "ORDER BY Year DESC, Title ASC";
    case Sort::none :
    default:
        return QString::null;
    }
}

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

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section, QString filter, Filter::filter filter2, Sort::sort sort )
{
    model->setQuery( QString("SELECT id,Title FROM %1 %2 %3").arg(
                   getTableName(section), "WHERE " + filterToString( filter2 )+ " AND " +filter, sortToString( sort ) ) );
    return 0;
}

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section, Filter::filter filter, Sort::sort sort )
{
    QString strFilter( "WHERE " + filterToString( filter ) );
    QString strSort( sortToString( sort ) );

    model->setQuery( QString("SELECT id,Title FROM %1 %2 %3").arg(
                  getTableName(section), strFilter, strSort ) );
    return 0;
}

bool MngrQuerys::createTable_Anime()
{
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "isAdult        INTEGER, "
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
                  "Score          INTEGER, "
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
        qCritical() << QString("Table %1 is not created!").arg( getTableName(sections::anime) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::createTable_Manga()
{
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "isAdult        INTEGER, "
                  "Title          VARCHAR(128), "
                  "AltTitle       VARCHAR(128), "
                  "Author         VARCHAR(32), "
                  "Translation    VARCHAR(128), "
                  "Vol            INTEGER, "
                  "Ch             INTEGER, "
                  "Pages          INTEGER, "
                  "vVol           INTEGER, "
                  "vCh            INTEGER, "
                  "vPages         INTEGER, "
                  "Score          INTEGER, "
                  "Year           INTEGER, "
                  "Tags           VARCHAR(256), "
                  "Description    TEXT, "
                  "URL            VARCHAR(256), "
                  "Dir            VARCHAR(256), "
                  "ImagePath      VARCHAR(256) "
                  ");").arg( getTableName( sections::manga ) );
    QSqlQuery query;
    if( !query.exec(sql) ){
        qCritical() << QString("Table %1 is not created!").arg( getTableName(sections::manga) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::createTable_Amv()
{
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
                  "isEditingDone   INTEGER, "
                  "isAdult         INTEGER, "
                  "Title           VARCHAR(128), "
                  "Author          VARCHAR(32), "
                  "Сontestant      VARCHAR(128), "
                  "Score           INTEGER, "
                  "Year            INTEGER, "
                  "Tags            VARCHAR(256), "
                  "ContainingMusic TEXT, "
                  "ContainingAnime TEXT, "
                  "AuthorComment   TEXT, "
                  "URL             VARCHAR(256), "
                  "Dir             VARCHAR(256), "
                  "ImagePath       VARCHAR(256) "
                  ");").arg( getTableName( sections::amv ) );
    QSqlQuery query;
    if( !query.exec(sql) ){
        qCritical() << QString("Table %1 is not created!").arg( getTableName(sections::amv) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::createTable_Dorama()
{
    QString sql = QString("CREATE TABLE IF NOT EXISTS %1( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "isAdult        INTEGER, "
                  "Title          VARCHAR(128), "
                  "AltTitle       VARCHAR(128), "
                  "Director       VARCHAR(32), "
                  "SeriesTV       INTEGER, "
                  "SeriesSpecial  INTEGER, "
                  "SeriesMovie    INTEGER, "
                  "vSeriesTV      INTEGER, "
                  "vSeriesSpecial INTEGER, "
                  "vSeriesMovie   INTEGER, "
                  "Score          INTEGER, "
                  "Year           INTEGER, "
                  "Season         INTEGER, "
                  "Tags           VARCHAR(256), "
                  "Description    TEXT, "
                  "Actors         TEXT, "
                  "URL            VARCHAR(256), "
                  "Dir            VARCHAR(256), "
                  "ImagePath      VARCHAR(256) "
                  ");").arg( getTableName( sections::dorama ) );
    QSqlQuery query;
    if( !query.exec(sql) ){
        qCritical() << QString("Table %1 is not created!").arg( getTableName(sections::dorama) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

QStringList MngrQuerys::getAnimeTags()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Futunari")          << QObject::tr("Music")             << QObject::tr("Shounen")
            << QObject::tr("Adventure")         << QObject::tr("Game")              << QObject::tr("Mystery")           << QObject::tr("Shounen-ai (Yaoi)")
            << QObject::tr("Bara")              << QObject::tr("Gender intrigue")	<< QObject::tr("Parody")            << QObject::tr("Space")
            << QObject::tr("Comedy")            << QObject::tr("Guro")              << QObject::tr("Philosophy")        << QObject::tr("Sport")
            << QObject::tr("Cooking")           << QObject::tr("Harem")             << QObject::tr("Police")            << QObject::tr("Steampunk")
            << QObject::tr("Cyberpunk")         << QObject::tr("Hentai")            << QObject::tr("Post-apocalyptic")	<< QObject::tr("Supernatural")
            << QObject::tr("Daily occurrence")	<< QObject::tr("Historical")        << QObject::tr("Psychology")        << QObject::tr("Tale")
            << QObject::tr("Demons")            << QObject::tr("Horror")            << QObject::tr("Ranobe")            << QObject::tr("Tentacles")
            << QObject::tr("Detective")         << QObject::tr("Josei")             << QObject::tr("Romantic")          << QObject::tr("The children's")
            << QObject::tr("Drama")             << QObject::tr("Kodomo")            << QObject::tr("Samurai")           << QObject::tr("Thriller")
            << QObject::tr("Ecchi")             << QObject::tr("Madness")           << QObject::tr("School")            << QObject::tr("Tragedy")
            << QObject::tr("Family")            << QObject::tr("Magic")             << QObject::tr("Science Fiction")	<< QObject::tr("Vampires")
            << QObject::tr("Fantastic")         << QObject::tr("Magical girl")      << QObject::tr("Seinen")            << QObject::tr("War")
            << QObject::tr("Fantasy")           << QObject::tr("Martial Arts")      << QObject::tr("Shojo")             << QObject::tr("Yonkoma")
            << QObject::tr("Fighter")           << QObject::tr("Mech")              << QObject::tr("Shojo-ai (Yuri)");
}

QStringList MngrQuerys::getMangaTags()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Futunari")          << QObject::tr("Music")             << QObject::tr("Shounen")
            << QObject::tr("Adventure")         << QObject::tr("Game")              << QObject::tr("Mystery")           << QObject::tr("Shounen-ai (Yaoi)")
            << QObject::tr("Bara")              << QObject::tr("Gender intrigue")   << QObject::tr("Parody")            << QObject::tr("Space")
            << QObject::tr("Comedy")            << QObject::tr("Guro")              << QObject::tr("Philosophy")        << QObject::tr("Sport")
            << QObject::tr("Cooking")           << QObject::tr("Harem")             << QObject::tr("Police")            << QObject::tr("Steampunk")
            << QObject::tr("Cyberpunk")     	<< QObject::tr("Hentai")            << QObject::tr("Post-apocalyptic")	<< QObject::tr("Supernatural")
            << QObject::tr("Daily occurrence")	<< QObject::tr("Historical")        << QObject::tr("Psychology")        << QObject::tr("Tale")
            << QObject::tr("Demons")            << QObject::tr("Horror")            << QObject::tr("Ranobe")            << QObject::tr("Tentacles")
            << QObject::tr("Detective")         << QObject::tr("Josei")             << QObject::tr("Romantic")          << QObject::tr("The children's")
            << QObject::tr("Drama")             << QObject::tr("Kodomo")            << QObject::tr("Samurai")           << QObject::tr("Thriller")
            << QObject::tr("Ecchi")             << QObject::tr("Madness")           << QObject::tr("School")            << QObject::tr("Tragedy")
            << QObject::tr("Family")            << QObject::tr("Magic")             << QObject::tr("Science Fiction")	<< QObject::tr("Vampires")
            << QObject::tr("Fantastic")         << QObject::tr("Magical girl")      << QObject::tr("Seinen")            << QObject::tr("War")
            << QObject::tr("Fantasy")           << QObject::tr("Martial Arts")      << QObject::tr("Shojo")             << QObject::tr("Yonkoma")
            << QObject::tr("Fighter")           << QObject::tr("Mech")              << QObject::tr("Shojo-ai (Yuri)");
}

QStringList MngrQuerys::getAmvTags()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Drama")         << QObject::tr("M@D")                   << QObject::tr("Romance")
            << QObject::tr("Character Profile")	<< QObject::tr("Ecchi")         << QObject::tr("MEP")                   << QObject::tr("Sentimental")
            << QObject::tr("Comedy (Fun)")      << QObject::tr("GMV")           << QObject::tr("MMV")                   << QObject::tr("Story")
            << QObject::tr("Cross-Over")        << QObject::tr("Horror")        << QObject::tr("Original Animation")	<< QObject::tr("Trailer")
            << QObject::tr("Dance")             << QObject::tr("Instrumental")  << QObject::tr("Psychedelic");
}

QStringList MngrQuerys::getDoramaTags()
{
    return QStringList()
            << QObject::tr("Adventure")         << QObject::tr("Friendship")      << QObject::tr("Policy")      << QObject::tr("Vampires")
            << QObject::tr("Art-house")         << QObject::tr("Game")            << QObject::tr("Psychology")	<< QObject::tr("Western")
            << QObject::tr("Biography")         << QObject::tr("Gay theme")       << QObject::tr("Relationship")
            << QObject::tr("Business")          << QObject::tr("Gender intrigue") << QObject::tr("Romance")
            << QObject::tr("Comedy")            << QObject::tr("Historical")      << QObject::tr("School")
            << QObject::tr("Crime")             << QObject::tr("Horror")          << QObject::tr("Sci-Fi")
            << QObject::tr("Daily occurrence")	<< QObject::tr("Lesbian theme")   << QObject::tr("Sitcom")
            << QObject::tr("Detective")         << QObject::tr("Live-action")     << QObject::tr("Sport")
            << QObject::tr("Documentary")       << QObject::tr("Martial Arts")    << QObject::tr("Suspense")
            << QObject::tr("Drama")             << QObject::tr("Medicine")        << QObject::tr("Taiga")
            << QObject::tr("Erotic")            << QObject::tr("Melodrama")       << QObject::tr("Tale")
            << QObject::tr("Family")            << QObject::tr("Military")        << QObject::tr("Thriller")
            << QObject::tr("Fantastic")         << QObject::tr("Musical")         << QObject::tr("Tokusatsu")
            << QObject::tr("Fantasy")           << QObject::tr("Mysticism")       << QObject::tr("Tragedy")
            << QObject::tr("Fighter")           << QObject::tr("Parody")          << QObject::tr("TV Show");
}

QString MngrQuerys::getAnimeCoversPath()
{
    const QString DBPath(QDir::homePath() + QDir::separator() + "."
                             + QApplication::organizationName()
                             + QDir::separator() + QApplication::applicationName() + QDir::separator());

    QSettings settings;
    return QString( settings.value("WorkDirectory", DBPath).toString()
                    + QDir::separator()
                    + "animeCovers" + QDir::separator() );
}
QString MngrQuerys::getMangaCoversPath()
{
    const QString DBPath(QDir::homePath() + QDir::separator() + "."
                             + QApplication::organizationName()
                             + QDir::separator() + QApplication::applicationName() + QDir::separator());

    QSettings settings;
    return QString( settings.value("WorkDirectory", DBPath).toString() + QDir::separator() + "mangaCovers" + QDir::separator() );
}
QString MngrQuerys::getAmvCoversPath()
{
    const QString DBPath(QDir::homePath() + QDir::separator() + "."
                             + QApplication::organizationName()
                             + QDir::separator() + QApplication::applicationName() + QDir::separator());

    QSettings settings;
    return QString( settings.value("WorkDirectory", DBPath).toString() + QDir::separator() + "amvCovers" + QDir::separator() );
}
QString MngrQuerys::getDoramaCoversPath()
{
    const QString DBPath(QDir::homePath() + QDir::separator() + "."
                             + QApplication::organizationName()
                             + QDir::separator() + QApplication::applicationName() + QDir::separator());

    QSettings settings;
    return QString( settings.value("WorkDirectory", DBPath).toString() + QDir::separator() + "doramaCovers" + QDir::separator() );
}

bool MngrQuerys::updateRecord(sections::section table, unsigned int recoord_id, QString field, QString newValue)
{
    QSqlQuery query;
    query.prepare( QString("UPDATE %1 SET %2 = :value WHERE id = :id;").arg( getTableName( table ) ).arg(field) );
    query.bindValue(":value", newValue);
    query.bindValue(":id", recoord_id );
    if( !query.exec() ){
        qCritical() << QString("Cannot update data in table %1").arg( getTableName( table ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::insertAnime(QMap<QString, QVariant> &data)
{
    QSqlQuery query;


    query.prepare( QString("INSERT INTO %1("
                  "isHaveLooked, isEditingDone, isAdult, Title,"
                  "OrigTitle, Director, PostScoring,"
                  "SeriesTV, SeriesOVA, SeriesONA, SeriesSpecial, SeriesMovie,"
                  "vSeriesTV, vSeriesOVA, vSeriesONA, vSeriesSpecial, vSeriesMovie,"
                  "Score, Year, Season, Studios,"
                  "Tags, Description,"
                  "URL, Dir, ImagePath"
                  ") VALUES "
                  "(:isHaveLooked, :isEditingDone, :isAdult, :Title,"
                  ":OrigTitle, :Director, :PostScoring,"
                  ":SeriesTV, :SeriesOVA, :SeriesONA, :SeriesSpecial, :SeriesMovie,"
                  ":vSeriesTV, :vSeriesOVA, :vSeriesONA, :vSeriesSpecial, :vSeriesMovie,"
                  ":Score, :Year, :Season, :Studios,"
                  ":Tags, :Description,"
                  ":URL, :Dir, :ImagePath)"
                  ).arg( MngrQuerys::getTableName( sections::anime ) ) );


    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":OrigTitle",     data["OrigTitle"] );
    query.bindValue( ":Director",      data["Director"] );
    query.bindValue( ":PostScoring",   data["PostScoring"] );
    query.bindValue( ":SeriesTV",      data["SeriesTV"] );
    query.bindValue( ":SeriesOVA",     data["SeriesOVA"] );
    query.bindValue( ":SeriesONA",     data["SeriesONA"] );
    query.bindValue( ":SeriesSpecial", data["SeriesSpecial"] );
    query.bindValue( ":SeriesMovie",   data["SeriesMovie"] );
    query.bindValue( ":vSeriesTV",     data["vSeriesTV"] );
    query.bindValue( ":vSeriesOVA",    data["vSeriesOVA"] );
    query.bindValue( ":vSeriesONA",    data["vSeriesONA"] );
    query.bindValue( ":vSeriesSpecial",data["vSeriesSpecial"] );
    query.bindValue( ":vSeriesMovie",  data["vSeriesMovie"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Season",        data["Season"] );
    query.bindValue( ":Studios",       data["Studios"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                        MngrQuerys::getTableName( sections::anime ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::insertManga(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("INSERT INTO %1("
                  "isHaveLooked, isEditingDone, isAdult, Title,"
                  "AltTitle, Author, Translation,"
                  "Vol, Ch, Pages,"
                  "vVol, vCh, vPages,"
                  "Score, Year,"
                  "Tags, Description,"
                  "URL, Dir, ImagePath"
                  ") VALUES "
                  "(:isHaveLooked, :isEditingDone, :isAdult, :Title,"
                  ":AltTitle, :Author, :Translation,"
                  ":Vol, :Ch, :Pages,"
                  ":vVol, :vCh, :vPages,"
                  ":Score, :Year,"
                  ":Tags, :Description,"
                  ":URL, :Dir, :ImagePath)"
                  ).arg( MngrQuerys::getTableName( sections::manga ) ) );

    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":AltTitle",      data["AltTitle"] );
    query.bindValue( ":Author",        data["Author"] );
    query.bindValue( ":Translation",   data["Translation"] );
    query.bindValue( ":Vol",           data["Vol"] );
    query.bindValue( ":Ch",            data["Ch"] );
    query.bindValue( ":Pages",         data["Pages"] );
    query.bindValue( ":vVol",          data["vVol"] );
    query.bindValue( ":vCh",           data["vCh"] );
    query.bindValue( ":vPages",        data["vPages"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                        MngrQuerys::getTableName( sections::manga ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::insertAmv(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("INSERT INTO %1("
                  "isEditingDone, isAdult, Title,"
                  "Author, Сontestant,"
                  "Score, Year, Tags,"
                  "ContainingMusic, ContainingAnime, AuthorComment,"
                  "URL, Dir, ImagePath"
                  ") VALUES ("
                  ":isEditingDone, :isAdult, :Title,"
                  ":Author, :Concursant,"
                  ":Score, :Year, :Tags,"
                  ":ContainingMusic, :ContainingAnime, :AuthorComment,"
                  ":URL, :Dir, :ImagePath)"
                  ).arg( MngrQuerys::getTableName( sections::amv ) ) );

    query.bindValue( ":isEditingDone",   data["isEditingDone"] );
    query.bindValue( ":isAdult",         data["isAdult"] );
    query.bindValue( ":Title",           data["Title"] );
    query.bindValue( ":Author",          data["Author"] );
    query.bindValue( ":Concursant",      data["Сontestant"] );
    query.bindValue( ":Score",           data["Score"] );
    query.bindValue( ":Year",            data["Year"] );
    query.bindValue( ":Tags",            data["Tags"] );
    query.bindValue( ":ContainingMusic", data["ContainingMusic"] );
    query.bindValue( ":ContainingAnime", data["ContainingAnime"] );
    query.bindValue( ":AuthorComment",   data["AuthorComment"] );
    query.bindValue( ":URL",             data["URL"] );
    query.bindValue( ":Dir",             data["Dir"] );
    query.bindValue( ":ImagePath",       data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                        MngrQuerys::getTableName( sections::amv ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::insertDorama(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("INSERT INTO %1("
                  "isHaveLooked, isEditingDone, isAdult, Title,"
                  "AltTitle, Director,"
                  "SeriesTV, SeriesSpecial, SeriesMovie,"
                  "vSeriesTV, vSeriesSpecial, vSeriesMovie,"
                  "Score, Year, Season,"
                  "Tags, Description, Actors,"
                  "URL, Dir, ImagePath"
                  ") VALUES "
                  "(:isHaveLooked, :isEditingDone, :isAdult, :Title,"
                  ":AltTitle, :Director,"
                  ":SeriesTV, :SeriesSpecial, :SeriesMovie,"
                  ":vSeriesTV, :vSeriesSpecial, :vSeriesMovie,"
                  ":Score, :Year, :Season,"
                  ":Tags, :Description, :Actors,"
                  ":URL, :Dir, :ImagePath)"
                  ).arg( MngrQuerys::getTableName( sections::dorama ) ) );


    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":AltTitle",      data["AltTitle"] );
    query.bindValue( ":Director",      data["Director"] );
    query.bindValue( ":PostScoring",   data["PostScoring"] );
    query.bindValue( ":SeriesTV",      data["SeriesTV"] );
    query.bindValue( ":SeriesSpecial", data["SeriesSpecial"] );
    query.bindValue( ":SeriesMovie",   data["SeriesMovie"] );
    query.bindValue( ":vSeriesTV",     data["vSeriesTV"] );
    query.bindValue( ":vSeriesSpecial",data["vSeriesSpecial"] );
    query.bindValue( ":vSeriesMovie",  data["vSeriesMovie"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Season",        data["Season"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":Actors",        data["Actors"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );

    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                        MngrQuerys::getTableName( sections::dorama ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::updateAnime(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("UPDATE %1 SET "
                  "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, isAdult = :isAdult, Title = :Title,"
                  "OrigTitle = :OrigTitle, Director = :Director, PostScoring = :PostScoring,"
                  "SeriesTV = :SeriesTV, SeriesOVA = :SeriesOVA, SeriesONA = :SeriesONA, SeriesSpecial = :SeriesSpecial, SeriesMovie = :SeriesMovie,"
                  "vSeriesTV = :vSeriesTV, vSeriesOVA = :vSeriesOVA, vSeriesONA = :vSeriesONA, vSeriesSpecial = :vSeriesSpecial, vSeriesMovie = :vSeriesMovie,"
                  "Score = :Score, Year = :Year, Season = :Season, Studios = :Studios,"
                  "Tags = :Tags, Description = :Description,"
                  "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;").arg( MngrQuerys::getTableName( sections::anime ) )
                  );

    query.bindValue( ":id",            data["id"] );
    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":OrigTitle",     data["OrigTitle"] );
    query.bindValue( ":Director",      data["Director"] );
    query.bindValue( ":PostScoring",   data["PostScoring"] );
    query.bindValue( ":SeriesTV",      data["SeriesTV"] );
    query.bindValue( ":SeriesOVA",     data["SeriesOVA"] );
    query.bindValue( ":SeriesONA",     data["SeriesONA"] );
    query.bindValue( ":SeriesSpecial", data["SeriesSpecial"] );
    query.bindValue( ":SeriesMovie",   data["SeriesMovie"] );
    query.bindValue( ":vSeriesTV",     data["vSeriesTV"] );
    query.bindValue( ":vSeriesOVA",    data["vSeriesOVA"] );
    query.bindValue( ":vSeriesONA",    data["vSeriesONA"] );
    query.bindValue( ":vSeriesSpecial",data["vSeriesSpecial"] );
    query.bindValue( ":vSeriesMovie",  data["vSeriesMovie"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Season",        data["Season"] );
    query.bindValue( ":Studios",       data["Studios"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot update data in table %1").arg(
                        MngrQuerys::getTableName( sections::anime ) )
                    << "\nSqlError: " << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::updateManga(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("UPDATE %1 SET "
    "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, isAdult = :isAdult, Title = :Title,"
    "AltTitle = :AltTitle, Author = :Author, Translation = :Translation,"
    "Vol = :Vol, Ch = :Ch, Pages = :Pages,"
    "vVol = :vVol, vCh = :vCh, vPages = :vPages,"
    "Score = :Score, Year = :Year,"
    "Tags = :Tags, Description = :Description,"
    "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;").arg(
    MngrQuerys::getTableName( sections::manga ) )
    );

    query.bindValue( ":id",            data["id"] );
    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":AltTitle",      data["AltTitle"] );
    query.bindValue( ":Author",        data["Author"] );
    query.bindValue( ":Translation",   data["Translation"] );
    query.bindValue( ":Vol",           data["Vol"] );
    query.bindValue( ":Ch",            data["Ch"] );
    query.bindValue( ":Pages",         data["Pages"] );
    query.bindValue( ":vVol",          data["vVol"] );
    query.bindValue( ":vCh",           data["vCh"] );
    query.bindValue( ":vPages",        data["vPages"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot update data in table %1").arg(
                        MngrQuerys::getTableName( sections::manga ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::updateAmv(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("UPDATE %1 SET "
    "isEditingDone = :isEditingDone, isAdult = :isAdult, Title = :Title,"
    "Author = :Author, Сontestant = :Concursant,"
    "Score = :Score, Year = :Year, Tags = :Tags,"
    "ContainingMusic = :ContainingMusic, ContainingAnime = :ContainingAnime, AuthorComment = :AuthorComment,"
    "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;"
    ).arg( MngrQuerys::getTableName( sections::amv ) ) );

    query.bindValue( ":id",              data["id"] );
    query.bindValue( ":isEditingDone",   data["isEditingDone"] );
    query.bindValue( ":isAdult",         data["isAdult"] );
    query.bindValue( ":Title",           data["Title"] );
    query.bindValue( ":Author",          data["Author"] );
    query.bindValue( ":Concursant",      data["Сontestant"] );
    query.bindValue( ":Score",           data["Score"] );
    query.bindValue( ":Year",            data["Year"] );
    query.bindValue( ":Tags",            data["Tags"] );
    query.bindValue( ":ContainingMusic", data["ContainingMusic"] );
    query.bindValue( ":ContainingAnime", data["ContainingAnime"] );
    query.bindValue( ":AuthorComment",   data["AuthorComment"] );
    query.bindValue( ":URL",             data["URL"] );
    query.bindValue( ":Dir",             data["Dir"] );
    query.bindValue( ":ImagePath",       data["ImagePath"] );
    if( !query.exec() ){
        qCritical() << QString("Cannot update data in table %1").arg(
                        MngrQuerys::getTableName( sections::amv ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::updateDorama(QMap<QString, QVariant> &data)
{
    QSqlQuery query;

    query.prepare( QString("UPDATE %1 SET "
    "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, Title = :Title,"
    "AltTitle = :AltTitle, Director = :Director,"
    "SeriesTV = :SeriesTV, SeriesSpecial = :SeriesSpecial, SeriesMovie = :SeriesMovie,"
    "vSeriesTV = :vSeriesTV, vSeriesSpecial = :vSeriesSpecial, vSeriesMovie = :vSeriesMovie,"
    "Year = :Year, Season = :Season,"
    "Tags = :Tags, Description = :Description, Actors = :Actors,"
    "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;").arg( MngrQuerys::getTableName( sections::dorama ) )
    );

    query.bindValue( ":id",            data["id"] );
    query.bindValue( ":isHaveLooked",  data["isHaveLooked"] );
    query.bindValue( ":isEditingDone", data["isEditingDone"] );
    query.bindValue( ":isAdult",       data["isAdult"] );
    query.bindValue( ":Title",         data["Title"] );
    query.bindValue( ":AltTitle",      data["AltTitle"] );
    query.bindValue( ":Director",      data["Director"] );
    query.bindValue( ":PostScoring",   data["PostScoring"] );
    query.bindValue( ":SeriesTV",      data["SeriesTV"] );
    query.bindValue( ":SeriesSpecial", data["SeriesSpecial"] );
    query.bindValue( ":SeriesMovie",   data["SeriesMovie"] );
    query.bindValue( ":vSeriesTV",     data["vSeriesTV"] );
    query.bindValue( ":vSeriesSpecial",data["vSeriesSpecial"] );
    query.bindValue( ":vSeriesMovie",  data["vSeriesMovie"] );
    query.bindValue( ":Score",         data["Score"] );
    query.bindValue( ":Year",          data["Year"] );
    query.bindValue( ":Season",        data["Season"] );
    query.bindValue( ":Tags",          data["Tags"] );
    query.bindValue( ":Description",   data["Description"] );
    query.bindValue( ":Actors",        data["Actors"] );
    query.bindValue( ":URL",           data["URL"] );
    query.bindValue( ":Dir",           data["Dir"] );
    query.bindValue( ":ImagePath",     data["ImagePath"] );

    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                        MngrQuerys::getTableName( sections::dorama ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
