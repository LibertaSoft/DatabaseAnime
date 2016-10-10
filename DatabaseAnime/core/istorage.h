#pragma once

#include "core/keyvalue.h"
#include "core/section.h"

template <class T>
class QList;
class QString;
class QAbstractTableModel;

class IStorage
{
public:
//    virtual ~IStorage() = 0;
    virtual KeyValue getByPk(const sections::section section, const QString &pk) = 0;
    virtual QAbstractTableModel* getTableModel(const sections::section section) = 0;

    virtual bool save(const sections::section section, const KeyValue &data) = 0;
    virtual bool save(const sections::section section, const QList<KeyValue> &data) = 0;
};
