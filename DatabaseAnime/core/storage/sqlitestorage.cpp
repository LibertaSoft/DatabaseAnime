#include "sqlitestorage.h"
#include "mngrquerys.h"
#include <QSqlTableModel>

SqliteStorage::SqliteStorage()
{
    _connection.open();
    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();
    MngrQuerys::createTable_Amv();
    MngrQuerys::createTable_Dorama();
}

SqliteStorage::~SqliteStorage()
{
    _connection.commit();
    _connection.close();
}

MngrConnection *SqliteStorage::getConnection()
{
    return &_connection;
}

bool SqliteStorage::commit()
{
    return _connection.commit();
}

bool SqliteStorage::transaction()
{
    return _connection.transaction();
}

KeyValue SqliteStorage::getByPk(const sections::section section, const QString &pk)
{

}

QAbstractTableModel *SqliteStorage::getTableModel(const sections::section section)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    MngrQuerys::selectSection( model, section, Tables::UniformField::TITLE, Filter::all, Sort::none );

//    QSqlTableModel *model = new QSqlTableModel;
//    model->setTable( MngrQuerys::getTableName( section ) );
//    model->select();
//    model->setFilter("any");

    return model;
}

bool SqliteStorage::save(const sections::section section, const KeyValue &data)
{

}

bool SqliteStorage::save(const sections::section section, const QList<KeyValue> &data)
{

}
