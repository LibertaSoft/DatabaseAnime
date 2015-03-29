#ifndef MANAGERQUERYS_H
#define MANAGERQUERYS_H

#include <QString>
#include <QMap>

#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include "globalenum.h"

typedef QMap<QString,QString>  Fields;

class ManagerQuerys
{
    ManagerQuerys();
    static QString filterToString(const Filter::filter field);
    static QString sortToString(  const Sort::sort sort);
public:
    static bool createTable(const QString& tableName, const Fields& fields);
    static bool dropTable(  const QString& tableName);

    static bool insertRecord(const QString& tableName, const QSqlRecord& record);
    static bool deleteRecord(const QString& tableName, const quint64 id);
    static bool updateRecord(const QString& tableName, const quint64 id, const QSqlRecord& record);

    static quint64 count(const QString& tableName);
    static QSqlQuery selectRecords(const QString& tableName, const quint64 first = 0, const long limit = 10000);

    static QSqlRecord selectRecord(const QString& tableName, const quint64 id);
    static QString getCoverPath(   const QString& tableName, const quint64 id);
    static void modelSelect(QSqlQueryModel&      model,
                            const QString&       tableName,
                            const QString&       field,
                            const Filter::filter filter = Filter::all,
                            const Sort::sort     sort   = Sort::none,
                            const QString&       search = QString::null);
};

#endif // MANAGERQUERYS_H
