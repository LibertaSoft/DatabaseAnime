#ifndef DORAMAMODEL_H
#define DORAMAMODEL_H

#include "core/models/basemodel.h"

class DoramaModel : public BaseModel
{
    QString _director;
    int _season = 0;

    int _series_all_tv = 0;
    int _series_all_special = 0;
    int _series_all_movie = 0;

    int _series_viewed_tv = 0;
    int _series_viewed_special = 0;
    int _series_viewed_movie = 0;

    QStringList _ganres;
    QString _roles;

    bool _wantToLook = false;
    bool _editing = false;

    int _score = 0;

public: // static
    static QStringList getGanresList();
public:
    DoramaModel();
    DoramaModel(const QString &id);


    bool save(const bool update);

    bool loadFromDatabase();
    KeyValue toKeyValue();
    bool fromKeyValue(const KeyValue &data);

    // get/set
    QString director() const;
    void setDirector(const QString &director);
    int season() const;
    void setSeason(int season);
    int series_all_tv() const;
    void setSeries_all_tv(int series_all_tv);
    int series_all_special() const;
    void setSeries_all_special(int series_all_special);
    int series_all_movie() const;
    void setSeries_all_movie(int series_all_movie);
    int series_viewed_tv() const;
    void setSeries_viewed_tv(int series_viewed_tv);
    int series_viewed_special() const;
    void setSeries_viewed_special(int series_viewed_special);
    int series_viewed_movie() const;
    void setSeries_viewed_movie(int series_viewed_movie);
    QStringList ganres() const;
    void setGanres(const QStringList &ganres);
    QString roles() const;
    void setRoles(const QString &roles);
    bool wantToLook() const;
    void setWantToLook(bool wantToLook);
    bool editing() const;
    void setEditing(bool editing);
    int score() const;
    void setScore(int score);
};

#endif // DORAMAMODEL_H
