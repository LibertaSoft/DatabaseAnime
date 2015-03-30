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

void BasicData::setId(const quint64 &id)
{
    _id = id;
}

QString BasicData::getTitle() const
{
    return _title;
}

void BasicData::setTitle(const QString &title)
{
    _title = title;
}

QString BasicData::getAltTitle() const
{
    return _altTitle;
}

void BasicData::setAltTitle(const QString &altTitle)
{
    _altTitle = altTitle;
}

QString BasicData::getUrl() const
{
    return _url;
}

void BasicData::setUrl(const QString &url)
{
    _url = url;
}

QString BasicData::getLocalDir() const
{
    return _localDir;
}

void BasicData::setLocalDir(const QString &localDir)
{
    _localDir = localDir;
}

QString BasicData::getCoverPath() const
{
    return _coverPath;
}

void BasicData::setCoverPath(const QString &coverPath)
{
    _coverPath = coverPath;
}

QString BasicData::getYear() const
{
    return _year;
}

void BasicData::setYear(const QString &year)
{
    _year = year;
}
