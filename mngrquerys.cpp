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
        case Tables::tAnime :
            return "animeTags";
            break;
        case Tables::tManga :
            return "mangaTags" ;
            break;
        case Tables::tAmv :
            return "amvTags" ;
            break;
        case Tables::tDorama :
            return "doramaTags" ;
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
    QString strFilter("WHERE ");
    model->setQuery( QString("SELECT id,Title FROM %1 %2").arg( getTableName(section), strFilter+filter ) );
    return 0;
}

int MngrQuerys::selectSection(QSqlQueryModel* model, sections::section section, Filter::filter filter)
{
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
    case Filter::film :
        strFilter += "SeriesFilm > 0";
        break;
    case Filter::all :
    default:
        strFilter = "";
    }

    model->setQuery( QString("SELECT id,Title FROM %1 %2").arg( getTableName(section), strFilter ) );
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
                  "SeriesFilm     INTEGER, "
                  "vSeriesTV      INTEGER, "
                  "vSeriesOVA     INTEGER, "
                  "vSeriesONA     INTEGER, "
                  "vSeriesSpecial INTEGER, "
                  "vSeriesFilm    INTEGER, "
                  "Year           INTEGER, "
                  "Season         INTEGER, "
                  "Studios        INTEGER, "
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

bool MngrQuerys::createTable_AnimeTags()
{
    QString sql = "CREATE TABLE IF NOT EXISTS animeTags("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "tagName VARCHAR(32) "
                  ");";
    QSqlQuery query;
    if( !query.exec(sql) ){
        qDebug() << "Table animeTags is not created! Error: " << query.lastError();
//        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("There was a mistake at table creation in a database.") );
        return false;
    }
    return true;
}

bool MngrQuerys::insert_defaultAnimeTags()
{
    QSqlQuery query;

    query.prepare( QString("DELETE FROM %1").arg( "animeTags" ) );
    if( !query.exec() ){
        qDebug() << "Cannot truncate table AnimeTags: " << query.lastError();
        //(new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }

    query.prepare( QString("INSERT INTO %1(tagName) VALUES "
                  "(:v01), (:v02), (:v03), (:v04), (:v05), "
                  "(:v06), (:v07), (:v08), (:v09), (:v10), "
                  "(:v11), (:v12), (:v13), (:v14), (:v15), "
                  "(:v16), (:v17), (:v18), (:v19), (:v20), "
                  "(:v21), (:v22), (:v23), (:v24), (:v25), "
                  "(:v26), (:v27), (:v28), (:v29), (:v30), "
                  "(:v31), (:v32), (:v33), (:v34), (:v35), "
                  "(:v36), (:v37), (:v38), (:v39), (:v40), "
                  "(:v41), (:v42), (:v43), (:v44)").arg( "animeTags" )
                  );
    query.bindValue(":v01", "Сёнен");
    query.bindValue(":v02", "Сёнен Ай");
    query.bindValue(":v03", "Сейнен");
    query.bindValue(":v04", "Сёдзе");
    query.bindValue(":v05", "Сёдзе Ай");
    query.bindValue(":v06", "Дзёсей");
    query.bindValue(":v07", "Комедия");
    query.bindValue(":v08", "Романтика");
    query.bindValue(":v09", "Школа");
    query.bindValue(":v10", "Безумие");
    query.bindValue(":v11", "Боевые исскуства");
    query.bindValue(":v12", "Вампиры");
    query.bindValue(":v13", "Военное");
    query.bindValue(":v14", "Гарем");
    query.bindValue(":v15", "Демоны");
    query.bindValue(":v16", "Детское");
    query.bindValue(":v17", "Драма");
    query.bindValue(":v18", "Игры");
    query.bindValue(":v19", "Исторический");
    query.bindValue(":v20", "Космос");
    query.bindValue(":v21", "Магия");
    query.bindValue(":v22", "Машины");
    query.bindValue(":v23", "Меха");
    query.bindValue(":v24", "Мистика");
    query.bindValue(":v25", "Музыка");
    query.bindValue(":v26", "Пародия");
    query.bindValue(":v27", "Повседневность");
    query.bindValue(":v28", "Полиция");
    query.bindValue(":v29", "Приключения");
    query.bindValue(":v30", "Психологическое");
    query.bindValue(":v31", "Самураи");
    query.bindValue(":v32", "Сверхъестественное");
    query.bindValue(":v33", "Фурри");
    query.bindValue(":v34", "Спорт");
    query.bindValue(":v35", "Супер сила");
    query.bindValue(":v36", "Ужасы");
    query.bindValue(":v37", "Фантастика");
    query.bindValue(":v38", "Фэнтези");
    query.bindValue(":v39", "Экшен");
    query.bindValue(":v40", "Этти");
    query.bindValue(":v41", "Триллер");
    query.bindValue(":v42", "Хентай");
    query.bindValue(":v43", "Яой");
    query.bindValue(":v44", "Юри");

    if( !query.exec() ){
        qDebug() << "Cannot insert data in table animeTags: " << query.lastError();
        //(new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}
