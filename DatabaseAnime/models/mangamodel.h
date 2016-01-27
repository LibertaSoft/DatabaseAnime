#ifndef MANGAMODEL_H
#define MANGAMODEL_H

#include "core/models/basemodel.h"

class MangaModel : public BaseModel
{
private:
    QString _author;
    QString _translator;

    int _series_total_volumes = 0;
    int _series_total_charapters = 0;
    int _series_total_pages = 0;

    int _series_viewed_volumes = 0;
    int _series_viewed_charapters = 0;
    int _series_viewed_pages = 0;

    QStringList _ganres;


    bool _wantToLook = false;
    bool _editing = false;

public: // static
    static QStringList getGanresList();
public:
    MangaModel();
    MangaModel(const QString &id);

    bool save(const bool update);

    bool loadFromDatabase();
    KeyValue toKeyValue();
    bool fromKeyValue(const KeyValue &data);

    // set/get
    QString author() const;
    void setAuthor(const QString &author);
    QString translator() const;
    void setTranslator(const QString &translator);
    int series_total_volumes() const;
    void setSeries_total_volumes(int series_total_volumes);
    int series_total_charapters() const;
    void setSeries_total_charapters(int series_total_charapters);
    int series_total_pages() const;
    void setSeries_total_pages(int series_total_pages);
    int series_viewed_volumes() const;
    void setSeries_viewed_volumes(int series_viewed_volumes);
    int series_viewed_charapters() const;
    void setSeries_viewed_charapters(int series_viewed_charapters);
    int series_viewed_pages() const;
    void setSeries_viewed_pages(int series_viewed_pages);
    QStringList ganres() const;
    void setGanres(const QStringList &ganres);
    bool wantToLook() const;
    void setWantToLook(bool wantToLook);
    bool editing() const;
    void setEditing(bool editing);
};

#endif // MANGAMODEL_H
