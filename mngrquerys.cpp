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
                  "(:v41), (:v42), (:v43), (:v44), (:v45), "
                  "(:v46), (:v47), (:v48), (:v49), (:v50), "
                  "(:v51), (:v52), (:v53), (:v54), (:v55), "
                  "(:v56), (:v57), (:v58), (:v59)").arg( "animeTags" )
                  );
    query.bindValue(":v01", "Бара");
    query.bindValue(":v02", "Безумие");
    query.bindValue(":v03", "Боевик");
    query.bindValue(":v04", "Боевые искусства");
    query.bindValue(":v05", "Вампиры");
    query.bindValue(":v06", "Война");
    query.bindValue(":v07", "Гарем");
    query.bindValue(":v08", "Гендерная интрига");
    query.bindValue(":v09", "Гуро");
    query.bindValue(":v10", "Демоны");
    query.bindValue(":v11", "Детектив");
    query.bindValue(":v12", "Детское");
    query.bindValue(":v13", "Дзёсэй");
    query.bindValue(":v14", "Драма");
    query.bindValue(":v15", "Ёнкома");
    query.bindValue(":v16", "Игра");
    query.bindValue(":v17", "Исторический");
    query.bindValue(":v18", "Киберпанк");
    query.bindValue(":v19", "Кодомо");
    query.bindValue(":v20", "Комедия");
    query.bindValue(":v21", "Космос");
    query.bindValue(":v22", "Кулинария");
    query.bindValue(":v23", "Магия");
    query.bindValue(":v24", "Махо-сёдзё");
    query.bindValue(":v25", "Меха");
    query.bindValue(":v26", "Мистика");
    query.bindValue(":v27", "Музыкальный");
    query.bindValue(":v28", "Научная фантастика");
    query.bindValue(":v29", "Пародия");
    query.bindValue(":v30", "Повседневность");
    query.bindValue(":v31", "Полиция");
    query.bindValue(":v32", "Постапокалиптика");
    query.bindValue(":v33", "Приключения");
    query.bindValue(":v34", "Психология");
    query.bindValue(":v35", "Ранобэ");
    query.bindValue(":v36", "Романтика");
    query.bindValue(":v37", "Самураи");
    query.bindValue(":v38", "Сверхъестественное");
    query.bindValue(":v39", "Семейный");
    query.bindValue(":v40", "Сёдзё");
    query.bindValue(":v41", "Сёдзё-ай (Юри)");
    query.bindValue(":v42", "Сёнэн");
    query.bindValue(":v43", "Сёнэн-ай (Яой)");
    query.bindValue(":v44", "Сказка");
    query.bindValue(":v45", "Спорт");
    query.bindValue(":v46", "Стимпанк");
    query.bindValue(":v47", "Сэйнэн");
    query.bindValue(":v48", "Тентакли");
    query.bindValue(":v49", "Трагедия");
    query.bindValue(":v50", "Триллер");
    query.bindValue(":v51", "Ужасы");
    query.bindValue(":v52", "Фантастика");
    query.bindValue(":v53", "Философия");
    query.bindValue(":v54", "Футунари");
    query.bindValue(":v55", "Фэнтези");
    query.bindValue(":v56", "Хентай");
    query.bindValue(":v57", "Школа");
    query.bindValue(":v58", "Экшен");
    query.bindValue(":v59", "Этти");

    if( !query.exec() ){
        qDebug() << "Cannot insert data in table animeTags: " << query.lastError();
        //(new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}
