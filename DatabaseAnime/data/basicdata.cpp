#include "basicdata.h"


BasicData::BasicData(quint64 id, QString &title)
{
    _id = id;
    _title = title;
}

quint64 BasicData::getId() const
{
    return _id;
}

void BasicData::setId(const quint64 &value)
{
    _id = value;
}

QString BasicData::getTitle() const
{
    return _title;
}

void BasicData::setTitle(const QString &value)
{
    _title = value;
}

QString BasicData::getAltTitle() const
{
    return _altTitle;
}

void BasicData::setAltTitle(const QString &value)
{
    _altTitle = value;
}

QString BasicData::getUrl() const
{
    return _url;
}

void BasicData::setUrl(const QString &value)
{
    _url = value;
}

QString BasicData::getLocalDir() const
{
    return _localDir;
}

void BasicData::setLocalDir(const QString &value)
{
    _localDir = value;
}

QString BasicData::getCoverPath() const
{
    return _coverPath;
}

void BasicData::setCoverPath(const QString &value)
{
    _coverPath = value;
}

QString BasicData::getYear() const
{
    return _year;
}

void BasicData::setYear(const QString &value)
{
    _year = value;
}
