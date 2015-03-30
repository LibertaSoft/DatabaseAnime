#ifndef ANIMEDATA_H
#define ANIMEDATA_H

#include "basicdata.h"
#include "seriesrange.h"
#include <QStringListModel>

class AnimeData : public BasicData
{
    QString     _description;
    QString     _director;
    bool        _adult        = false;
    bool        _editFinished = false;
    bool        _haveLooked   = false;
    QString     _voice;
    QString     _score;
    int         _season       = 0;
    QStringList _tags;
    SeriesRange _tv;
    SeriesRange _ova;
    SeriesRange _ona;
    SeriesRange _special;
    SeriesRange _movie;

public:
    AnimeData(quint64 id, QString title);
    QString description() const;
    void setDescription(const QString &description);
    QString director() const;
    void setDirector(const QString &director);
    bool adult() const;
    void setAdult(bool adult);
    bool editFinished() const;
    void setEditFinished(bool editFinished);
    bool haveLooked() const;
    void setHaveLooked(bool haveLooked);
    QString postScoring() const;
    void setPostScoring(const QString &postScoring);
    QString voice() const;
    void setVoice(const QString &voice);
    QString score() const;
    void setScore(const QString &score);
    int season() const;
    void setSeason(const int season);
    QStringList tags() const;
    void setTags(const QStringList &tags);

    int viewTvSeries() const;
    void setViewTvSeries(const int count);
    int viewOvaSeries() const;
    void setViewOvaSeries(const int count);
    int viewOnaSeries() const;
    void setViewOnaSeries(const int count);
    int viewSpecialSeries() const;
    void setViewSpecialSeries(const int count);
    int viewMovieSeries() const;
    void setViewMovieSeries(const int count);

    int allTvSeries() const;
    void setAllTvSeries(const int count);
    int allOvaSeries() const;
    void setAllOvaSeries(const int count);
    int allOnaSeries() const;
    void setAllOnaSeries(const int count);
    int allSpecialSeries() const;
    void setAllSpecialSeries(const int count);
    int allMovieSeries() const;
    void setAllMovieSeries(const int count);
};

#endif // ANIMEDATA_H
