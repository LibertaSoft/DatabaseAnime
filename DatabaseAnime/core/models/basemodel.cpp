#include "basemodel.h"

QString BaseModel::id() const
{
    return _id;
}

void BaseModel::setId(const QString &id)
{
    _id = id;
}

QString BaseModel::title() const
{
    return _title;
}

void BaseModel::setTitle(const QString &title)
{
    _title = title;
}

QString BaseModel::altTitle() const
{
    return _altTitle;
}

void BaseModel::setAltTitle(const QString &altTitle)
{
    _altTitle = altTitle;
}

QDate BaseModel::date() const
{
    return _date;
}

void BaseModel::setDate(const QDate &date)
{
    _date = date;
}

QString BaseModel::cover() const
{
    return _cover;
}

void BaseModel::setCover(const QString &cover)
{
    _cover = cover;
}

QString BaseModel::localPath() const
{
    return _localPath;
}

void BaseModel::setLocalPath(const QString &localPath)
{
    _localPath = localPath;
}

QString BaseModel::url() const
{
    return _url;
}

void BaseModel::setUrl(const QString &url)
{
    _url = url;
}

QString BaseModel::description() const
{
    return _description;
}

void BaseModel::setDescription(const QString &description)
{
    _description = description;
}

BaseModel::BaseModel()
{

}
