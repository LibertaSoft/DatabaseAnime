#include "animedata.h"


QString AnimeData::description() const
{
    return _description;
}

void AnimeData::setDescription(const QString &description)
{
    _description = description;
}

QString AnimeData::director() const
{
    return _director;
}

void AnimeData::setDirector(const QString &director)
{
    _director = director;
}

bool AnimeData::adult() const
{
    return _adult;
}

void AnimeData::setAdult(bool adult)
{
    _adult = adult;
}

bool AnimeData::editFinished() const
{
    return _editFinished;
}

void AnimeData::setEditFinished(bool editFinished)
{
    _editFinished = editFinished;
}

bool AnimeData::haveLooked() const
{
    return _haveLooked;
}

void AnimeData::setHaveLooked(bool haveLooked)
{
    _haveLooked = haveLooked;
}

QString AnimeData::voice() const
{
    return _voice;
}

void AnimeData::setVoice(const QString &voice)
{
    _voice = voice;
}

QString AnimeData::score() const
{
    return _score;
}

void AnimeData::setScore(const QString &score)
{
    _score = score;
}

int AnimeData::season() const
{
    return _season;
}

void AnimeData::setSeason(const int season)
{
    if( season < 0 )
        return;

    _season = season;
}

QStringList AnimeData::tags() const
{
    return _tags;
}

void AnimeData::setTags(const QStringList &tags)
{
    _tags = tags;
}

int AnimeData::viewTvSeries() const
{
    return _tv.current();
}

void AnimeData::setViewTvSeries(const int count)
{
    _tv.setCurrent(count);
}

int AnimeData::viewOvaSeries() const
{
    return _ova.current();
}

void AnimeData::setViewOvaSeries(const int count)
{
    _ova.setCurrent(count);
}

int AnimeData::viewOnaSeries() const
{
    return _ona.current();
}

void AnimeData::setViewOnaSeries(const int count)
{
    _ona.setCurrent(count);
}

int AnimeData::viewSpecialSeries() const
{
    return _special.current();
}

void AnimeData::setViewSpecialSeries(const int count)
{
    _special.setCurrent(count);
}

int AnimeData::viewMovieSeries() const
{
    return _movie.current();
}

void AnimeData::setViewMovieSeries(const int count)
{
    _movie.setCurrent(count);
}

int AnimeData::allTvSeries() const
{
    return _tv.all();
}

void AnimeData::setAllTvSeries(const int count)
{
    _tv.setAll(count);
}

int AnimeData::allOvaSeries() const
{
    return _ova.all();
}

void AnimeData::setAllOvaSeries(const int count)
{
    _ova.setAll(count);
}

int AnimeData::allOnaSeries() const
{
    return _ona.all();
}

void AnimeData::setAllOnaSeries(const int count)
{
    _ona.setAll(count);
}

int AnimeData::allSpecialSeries() const
{
    return _special.all();
}

void AnimeData::setAllSpecialSeries(const int count)
{
    _special.setAll(count);
}

int AnimeData::allMovieSeries() const
{
    return _movie.all();
}

void AnimeData::setAllMovieSeries(const int count)
{
    _movie.setAll(count);
}
AnimeData::AnimeData(quint64 id, QString title)
    : BasicData(id, title)
{}
