#pragma once

#include "core/istorage.h"
#include "mngrconnection.h"

class SqliteStorage : public IStorage
{
private:
    MngrConnection _connection;
public:
    SqliteStorage();
    ~SqliteStorage();

    // tmp:
    MngrConnection* getConnection();
    bool commit();
    bool transaction();

    // IStorage interface
public:
    KeyValue getByPk(const sections::section section, const QString &pk);
    QAbstractTableModel* getTableModel(const sections::section section, const Filter::filter filter = Filter::all);
    bool save(const sections::section section, const KeyValue &data);
    bool save(const sections::section section, const QList<KeyValue> &data);
};

