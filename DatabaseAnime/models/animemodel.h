#ifndef ANIMEMODEL_H
#define ANIMEMODEL_H

#include "core/models/basemodel.h"

class AnimeModel : public BaseModel
{
private:
    bool    _isAdult = false;
    int     _seasone = 0;
    int     _score = 0;
    QString _studio;
    QString _director;
    QString _postScoring;

    int _series_total_tv      = 0;
    int _series_total_ova     = 0;
    int _series_total_ona     = 0;
    int _series_total_special = 0;
    int _series_total_movie   = 0;

    int _series_viewed_tv      = 0;
    int _series_viewed_ova     = 0;
    int _series_viewed_ona     = 0;
    int _series_viewed_special = 0;
    int _series_viewed_movie   = 0;

    QStringList _ganres;

    bool _wantToLook = false;
    bool _editing = false;
public: // static
    static QStringList getGanresList();
public:
    AnimeModel();
    AnimeModel(const QString id);

    bool save(const bool update);

    bool loadFromDatabase();
    KeyValue toKeyValue();
    bool fromKeyValue(const KeyValue &data);

    // get/set
    int seasone() const;
    void setSeasone(int seasone);
    QString studio() const;
    void setStudio(const QString &studio);
    int series_total_tv() const;
    void setSeries_total_tv(int series_total_tv);
    int series_total_ova() const;
    void setSeries_total_ova(int series_total_ova);
    int series_total_ona() const;
    void setSeries_total_ona(int series_total_ona);
    int series_total_special() const;
    void setSeries_total_special(int series_total_special);
    int series_total_movie() const;
    void setSeries_total_movie(int series_total_movie);
    int series_viewed_tv() const;
    void setSeries_viewed_tv(int series_viewed_tv);
    int series_viewed_ova() const;
    void setSeries_viewed_ova(int series_viewed_ova);
    int series_viewed_ona() const;
    void setSeries_viewed_ona(int series_viewed_ona);
    int series_viewed_special() const;
    void setSeries_viewed_special(int series_viewed_special);
    int series_viewed_movie() const;
    void setSeries_viewed_movie(int series_viewed_movie);
    QStringList ganres() const;
    void setGanres(const QStringList &ganres);
    bool wantToLook() const;
    void setWantToLook(bool wantToLook);
    bool editing() const;
    void setEditing(bool editing);
    QString director() const;
    void setDirector(const QString &director);
    QString postScoring() const;
    void setPostScoring(const QString &postScoring);
    bool getIsAdult() const;
    void setIsAdult(bool isAdult);
    int getScore() const;
    void setScore(int score);
};

#endif // ANIMEMODEL_H
