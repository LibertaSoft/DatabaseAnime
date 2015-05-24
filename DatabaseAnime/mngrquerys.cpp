#include "mngrquerys.h"
#include <QApplication>

QString MngrQuerys::fieldToString(Tables::UniformField::field field, sections::section section)
{
    using namespace Tables::UniformField;
    switch (field) {
        case All :
            // "*"
            return Tables::Anime::Fields::All;
        case id :
            // "id"
            return Tables::Anime::Fields::id;
        case Title :
            switch(section){
                case sections::anime :
                    return Tables::Anime::Fields::Title;
                case sections::manga :
                    return Tables::Manga::Fields::Title;
                case sections::amv :
                    return Tables::Amv::Fields::Title;
                case sections::dorama :
                    return Tables::Dorama::Fields::Title;
                case sections::none :
                    return QString::null;
            }
        case AltTitle :
            switch(section){
                case sections::anime :
                    return Tables::Anime::Fields::AltTitle;
                case sections::manga :
                    return Tables::Manga::Fields::AltTitle;
                case sections::amv :
                    return Tables::Amv::Fields::Title;
                case sections::dorama :
                    return Tables::Dorama::Fields::AltTitle;
                case sections::none :
                    return QString::null;
            }
        case Url :
            switch(section){
                case sections::anime :
                    return Tables::Anime::Fields::Url;
                case sections::manga :
                    return Tables::Manga::Fields::Url;
                case sections::amv :
                    return Tables::Amv::Fields::Url;
                case sections::dorama :
                    return Tables::Dorama::Fields::Url;
                case sections::none :
                    return QString::null;
            }
        case Dir :
            switch(section){
                case sections::anime :
                    return Tables::Anime::Fields::Dir;
                case sections::manga :
                    return Tables::Manga::Fields::Dir;
                case sections::amv :
                    return Tables::Amv::Fields::Dir;
                case sections::dorama :
                    return Tables::Dorama::Fields::Dir;
                case sections::none :
                    return QString::null;
            }
        case ImagePath :
            switch(section){
                case sections::anime :
                    return Tables::Anime::Fields::ImagePath;
                case sections::manga :
                    return Tables::Manga::Fields::ImagePath;
                case sections::amv :
                    return Tables::Amv::Fields::ImagePath;
                case sections::dorama :
                    return Tables::Dorama::Fields::ImagePath;
                case sections::none :
                    return QString::null;
            }
    }
    return QString::null;
}

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

QString MngrQuerys::getTableName(sections::section table)
{
    switch( table ){
        case sections::anime :
            return Tables::Anime::TableName;
            break;
        case sections::manga :
            return Tables::Manga::TableName;
            break;
        case sections::amv :
            return Tables::Amv::TableName;
            break;
        case sections::dorama :
            return Tables::Dorama::TableName;
            break;
        case sections::none :
        default:
            return QString::null;
    }
}

bool MngrQuerys::selectSection(QSqlQueryModel *model, sections::section section,
                               Tables::UniformField::field field, Filter::filter filter,
                               Sort::sort sort, QString search)
{
    if(section == sections::none){
        model->clear();
        return false;
    }
    //"SELECT {id}, {Title} FROM {animeSerials} WHERE {0 = 0} [AND Title LIKE '{search}'] [ORDER {ASC}] "
    // {} - Dynamic; [] - Optional
    QString sql = "SELECT "
                + fieldToString(Tables::UniformField::id, section)
                + ", "
                + fieldToString(field, section)
                + " FROM "
                + getTableName(section)
                + " WHERE " + filterToString( filter );
    if( ! search.isNull() )
        sql += " AND " + fieldToString(field, section) + " LIKE '%" + search + "%'";
    sql += " " + sortToString( sort );

    model->setQuery( sql );
    return true;
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
                  "Contestant      VARCHAR(128), "
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

quint64 MngrQuerys::count(sections::section section)
{
    QSqlQuery query;
    QString sql( "SELECT count(*) FROM %1" );
    if( false == query.exec( sql.arg( getTableName(section) ) ) ){
        qCritical() << "Error. Can't counted records in table '" + getTableName( section ) + "'"
                    << "\nSqlError: "
                    << query.lastError();
        return 0;
    }
    query.next();
    return query.value(0).toULongLong();
}

void MngrQuerys::count(quint64 &anime, quint64 &manga, quint64 &amv, quint64 &dorama)
{
    anime  = count(sections::anime );
    manga  = count(sections::manga );
    amv    = count(sections::amv   );
    dorama = count(sections::dorama);
}

bool MngrQuerys::dropTable(sections::section section)
{
    QSqlQuery query;
    QString sql( "DROP TABLE IF EXISTS %1" );
    if( false == query.exec( sql.arg( getTableName(section) ) ) ){
        qCritical() << "Error when deleting a table " + getTableName( section )
                    << query.lastError();
        return false;
    }
    return true;
}

QSqlQuery MngrQuerys::selectAll(sections::section section)
{
    QSqlQuery query( "SELECT * FROM " + getTableName(section) );
    if( ! query.exec() )
        qCritical() << QString("Cannot select data from table " + getTableName(section) )
                    << query.lastError();
    return query;
}

QString MngrQuerys::getImagePath(sections::section section, quint64 id)
{
    QSqlQueryModel model;
    model.setQuery( QString( "SELECT ImagePath FROM %1 WHERE id = %2").arg( getTableName(section) ).arg( id ) );
    return model.record(0).value("ImagePath").toString();
}

bool MngrQuerys::deleteRecord(sections::section section, quint64 id)
{
    QSqlQuery query;
    query.prepare( QString("DELETE FROM '%1' WHERE id = :id;").arg( getTableName(section) ) );
    query.bindValue(":id", id);
    int result = query.exec();
    if( ! result ){
        qCritical() << "It was not succeeded to remove record from table " + getTableName(section)
                    << "\nSqlError: "
                    << query.lastError();
    }
    return result;
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

QSqlRecord MngrQuerys::selectData(sections::section table, quint64 id)
{
    QSqlQuery query;
    QString sql("SELECT * FROM '%1' WHERE id='%2'");
    sql = sql.arg( getTableName(table) ).arg( id );
    query.exec( sql );
    query.next();
    return query.record();
}

QStringList MngrQuerys::selectUnWatched(sections::section table)
{
    QSqlQuery query;
    QString field = fieldToString(Tables::UniformField::Title, table);
    QString tableName = getTableName(table);
    QString where;
    switch (table) {
        case sections::anime:
            where = Tables::Anime::Fields::SeriesTV + ">" + Tables::Anime::Fields::vSeriesTV;
            break;
        case sections::manga:
            where = Tables::Manga::Fields::Ch + ">" + Tables::Manga::Fields::vCh;
            break;
        case sections::amv:
            where = "0=1";
            break;
        case sections::dorama:
            where = "0=1";
            break;
        default:
            where = "0=1";
            break;
    }
    QString sql("SELECT " + field + " FROM " + tableName + " WHERE " + where);
    qDebug() << sql;

    if( ! query.exec(sql) ){
        qCritical() << query.lastError().text();
        return QStringList();
    }

    QStringList result;
    while( query.next() ){
        result.append( query.value( query.record().indexOf(field) ).toString() );
    }

    qDebug() << result;
    return result;
}

bool MngrQuerys::updateRecord(sections::section table, quint64 recoord_id, QString field, QString newValue)
{
    QSqlQuery query;
    query.prepare( QString("UPDATE %1 SET %2 = :value WHERE id = :id;").arg( getTableName( table ) ).arg(field) );
    query.bindValue(":value", newValue);
    query.bindValue(":id", recoord_id );
    if( !query.exec() ){
        qCritical() << QString("Cannot update data in table %1").arg( getTableName( table ) )
                    << "\nSQL("+query.lastQuery()+")"
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::insertAnime(QMap<QString, QVariant> data)
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
    using namespace Tables::Anime::Fields;
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":OrigTitle",     data[AltTitle] );
    query.bindValue( ":Director",      data[Director] );
    query.bindValue( ":PostScoring",   data[PostScoring] );
    query.bindValue( ":SeriesTV",      data[SeriesTV] );
    query.bindValue( ":SeriesOVA",     data[SeriesOVA] );
    query.bindValue( ":SeriesONA",     data[SeriesONA] );
    query.bindValue( ":SeriesSpecial", data[SeriesSpecial] );
    query.bindValue( ":SeriesMovie",   data[SeriesMovie] );
    query.bindValue( ":vSeriesTV",     data[vSeriesTV] );
    query.bindValue( ":vSeriesOVA",    data[vSeriesOVA] );
    query.bindValue( ":vSeriesONA",    data[vSeriesONA] );
    query.bindValue( ":vSeriesSpecial",data[vSeriesSpecial] );
    query.bindValue( ":vSeriesMovie",  data[vSeriesMovie] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Season",        data[Season] );
    query.bindValue( ":Studios",       data[Studios] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );
    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                           getTableName( sections::anime ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::insertManga(QMap<QString, QVariant> data)
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

    using namespace Tables::Manga::Fields;
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":AltTitle",      data[AltTitle] );
    query.bindValue( ":Author",        data[Author] );
    query.bindValue( ":Translation",   data[Translation] );
    query.bindValue( ":Vol",           data[Vol] );
    query.bindValue( ":Ch",            data[Ch] );
    query.bindValue( ":Pages",         data[Pages] );
    query.bindValue( ":vVol",          data[vVol] );
    query.bindValue( ":vCh",           data[vCh] );
    query.bindValue( ":vPages",        data[vPages] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );
    if( !query.exec() ){
        qCritical() << QString("Cannot insert data in table %1").arg(
                           getTableName( sections::manga ) )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
bool MngrQuerys::insertAmv(QMap<QString, QVariant> data)
{
    QSqlQuery query;

    query.prepare( QString("INSERT INTO %1("
                  "isEditingDone, isAdult, Title,"
                  "Author, Contestant,"
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

    using namespace Tables::Amv::Fields;
    query.bindValue( ":isEditingDone",   data[isEditingDone] );
    query.bindValue( ":isAdult",         data[isAdult] );
    query.bindValue( ":Title",           data[Title] );
    query.bindValue( ":Author",          data[Author] );
    query.bindValue( ":Concursant",      data[Contestant] );
    query.bindValue( ":Score",           data[Score] );
    query.bindValue( ":Year",            data[Year] );
    query.bindValue( ":Tags",            data[Tags] );
    query.bindValue( ":ContainingMusic", data[ContainingMusic] );
    query.bindValue( ":ContainingAnime", data[ContainingAnime] );
    query.bindValue( ":AuthorComment",   data[AuthorComment] );
    query.bindValue( ":URL",             data[Url] );
    query.bindValue( ":Dir",             data[Dir] );
    query.bindValue( ":ImagePath",       data[ImagePath] );
    if( !query.exec() ){
        qCritical() << "Cannot insert data in table" + getTableName( sections::amv )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}

bool MngrQuerys::insertDorama(QMap<QString, QVariant> data)
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

    using namespace Tables::Dorama::Fields;
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":AltTitle",      data[AltTitle] );
    query.bindValue( ":Director",      data[Director] );
    query.bindValue( ":SeriesTV",      data[SeriesTV] );
    query.bindValue( ":SeriesSpecial", data[SeriesSpecial] );
    query.bindValue( ":SeriesMovie",   data[SeriesMovie] );
    query.bindValue( ":vSeriesTV",     data[vSeriesTV] );
    query.bindValue( ":vSeriesSpecial",data[vSeriesSpecial] );
    query.bindValue( ":vSeriesMovie",  data[vSeriesMovie] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Season",        data[Season] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":Actors",        data[Actors] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );

    if( !query.exec() ){
        qCritical() << "Cannot insert data in table " + getTableName( sections::dorama )
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

    using namespace Tables::Anime::Fields;
    query.bindValue( ":id",            data[id] );
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":OrigTitle",     data[AltTitle] );
    query.bindValue( ":Director",      data[Director] );
    query.bindValue( ":PostScoring",   data[PostScoring] );
    query.bindValue( ":SeriesTV",      data[SeriesTV] );
    query.bindValue( ":SeriesOVA",     data[SeriesOVA] );
    query.bindValue( ":SeriesONA",     data[SeriesONA] );
    query.bindValue( ":SeriesSpecial", data[SeriesSpecial] );
    query.bindValue( ":SeriesMovie",   data[SeriesMovie] );
    query.bindValue( ":vSeriesTV",     data[vSeriesTV] );
    query.bindValue( ":vSeriesOVA",    data[vSeriesOVA] );
    query.bindValue( ":vSeriesONA",    data[vSeriesONA] );
    query.bindValue( ":vSeriesSpecial",data[vSeriesSpecial] );
    query.bindValue( ":vSeriesMovie",  data[vSeriesMovie] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Season",        data[Season] );
    query.bindValue( ":Studios",       data[Studios] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );
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

    using namespace Tables::Manga::Fields;
    query.bindValue( ":id",            data[id] );
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":AltTitle",      data[AltTitle] );
    query.bindValue( ":Author",        data[Author] );
    query.bindValue( ":Translation",   data[Translation] );
    query.bindValue( ":Vol",           data[Vol] );
    query.bindValue( ":Ch",            data[Ch] );
    query.bindValue( ":Pages",         data[Pages] );
    query.bindValue( ":vVol",          data[vVol] );
    query.bindValue( ":vCh",           data[vCh] );
    query.bindValue( ":vPages",        data[vPages] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );
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
    "Author = :Author, Contestant = :Concursant,"
    "Score = :Score, Year = :Year, Tags = :Tags,"
    "ContainingMusic = :ContainingMusic, ContainingAnime = :ContainingAnime, AuthorComment = :AuthorComment,"
    "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;"
    ).arg( MngrQuerys::getTableName( sections::amv ) ) );

    using namespace Tables::Amv::Fields;

    query.bindValue( ":id",              data[id] );
    query.bindValue( ":isEditingDone",   data[isEditingDone] );
    query.bindValue( ":isAdult",         data[isAdult] );
    query.bindValue( ":Title",           data[Title] );
    query.bindValue( ":Author",          data[Author] );
    query.bindValue( ":Concursant",      data[Contestant] );
    query.bindValue( ":Score",           data[Score] );
    query.bindValue( ":Year",            data[Year] );
    query.bindValue( ":Tags",            data[Tags] );
    query.bindValue( ":ContainingMusic", data[ContainingMusic] );
    query.bindValue( ":ContainingAnime", data[ContainingAnime] );
    query.bindValue( ":AuthorComment",   data[AuthorComment] );
    query.bindValue( ":URL",             data[Url] );
    query.bindValue( ":Dir",             data[Dir] );
    query.bindValue( ":ImagePath",       data[ImagePath] );
    if( !query.exec() ){
        qCritical() << "Cannot update data in table " + getTableName( sections::amv )
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

    using namespace Tables::Dorama::Fields;
    query.bindValue( ":id",            data[id] );
    query.bindValue( ":isHaveLooked",  data[isHaveLooked] );
    query.bindValue( ":isEditingDone", data[isEditingDone] );
    query.bindValue( ":isAdult",       data[isAdult] );
    query.bindValue( ":Title",         data[Title] );
    query.bindValue( ":AltTitle",      data[AltTitle] );
    query.bindValue( ":Director",      data[Director] );
    query.bindValue( ":SeriesTV",      data[SeriesTV] );
    query.bindValue( ":SeriesSpecial", data[SeriesSpecial] );
    query.bindValue( ":SeriesMovie",   data[SeriesMovie] );
    query.bindValue( ":vSeriesTV",     data[vSeriesTV] );
    query.bindValue( ":vSeriesSpecial",data[vSeriesSpecial] );
    query.bindValue( ":vSeriesMovie",  data[vSeriesMovie] );
    query.bindValue( ":Score",         data[Score] );
    query.bindValue( ":Year",          data[Year] );
    query.bindValue( ":Season",        data[Season] );
    query.bindValue( ":Tags",          data[Tags] );
    query.bindValue( ":Description",   data[Description] );
    query.bindValue( ":Actors",        data[Actors] );
    query.bindValue( ":URL",           data[Url] );
    query.bindValue( ":Dir",           data[Dir] );
    query.bindValue( ":ImagePath",     data[ImagePath] );

    if( !query.exec() ){
        qCritical() << "Cannot insert data in table " + getTableName( sections::dorama )
                    << "\nSqlError: "
                    << query.lastError();
        return false;
    }
    return true;
}
