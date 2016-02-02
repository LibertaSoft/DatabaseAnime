#pragma once

#include "core/keyvalue.h"

template <class T>
class QList;
class QString;
class QAbstractTableModel;

class IStorage
{
public:
    virtual KeyValue getByPk(const QString &pk) = 0;
    virtual QAbstractTableModel getByTitle(const QString &title) = 0;
};
