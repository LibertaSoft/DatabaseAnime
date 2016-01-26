#include "animemodel.h"

#include "mngrquerys.h"

int AnimeModel::seasone() const
{
    return _seasone;
}

void AnimeModel::setSeasone(int seasone)
{
    _seasone = seasone;
}

QString AnimeModel::studio() const
{
    return _studio;
}

void AnimeModel::setStudio(const QString &studio)
{
    _studio = studio;
}

int AnimeModel::series_total_tv() const
{
    return _series_total_tv;
}

void AnimeModel::setSeries_total_tv(int series_total_tv)
{
    _series_total_tv = series_total_tv;
}

int AnimeModel::series_total_ova() const
{
    return _series_total_ova;
}

void AnimeModel::setSeries_total_ova(int series_total_ova)
{
    _series_total_ova = series_total_ova;
}

int AnimeModel::series_total_ona() const
{
    return _series_total_ona;
}

void AnimeModel::setSeries_total_ona(int series_total_ona)
{
    _series_total_ona = series_total_ona;
}

int AnimeModel::series_total_special() const
{
    return _series_total_special;
}

void AnimeModel::setSeries_total_special(int series_total_special)
{
    _series_total_special = series_total_special;
}

int AnimeModel::series_total_movie() const
{
    return _series_total_movie;
}

void AnimeModel::setSeries_total_movie(int series_total_movie)
{
    _series_total_movie = series_total_movie;
}

int AnimeModel::series_viewed_tv() const
{
    return _series_viewed_tv;
}

void AnimeModel::setSeries_viewed_tv(int series_viewed_tv)
{
    _series_viewed_tv = series_viewed_tv;
}

int AnimeModel::series_viewed_ova() const
{
    return _series_viewed_ova;
}

void AnimeModel::setSeries_viewed_ova(int series_viewed_ova)
{
    _series_viewed_ova = series_viewed_ova;
}

int AnimeModel::series_viewed_ona() const
{
    return _series_viewed_ona;
}

void AnimeModel::setSeries_viewed_ona(int series_viewed_ona)
{
    _series_viewed_ona = series_viewed_ona;
}

int AnimeModel::series_viewed_special() const
{
    return _series_viewed_special;
}

void AnimeModel::setSeries_viewed_special(int series_viewed_special)
{
    _series_viewed_special = series_viewed_special;
}

int AnimeModel::series_viewed_movie() const
{
    return _series_viewed_movie;
}

void AnimeModel::setSeries_viewed_movie(int series_viewed_movie)
{
    _series_viewed_movie = series_viewed_movie;
}

QStringList AnimeModel::ganres() const
{
    return _ganres;
}

void AnimeModel::setGanres(const QStringList &ganres)
{
    _ganres = ganres;
}

bool AnimeModel::wantToLook() const
{
    return _wantToLook;
}

void AnimeModel::setWantToLook(bool wantToLook)
{
    _wantToLook = wantToLook;
}

bool AnimeModel::editing() const
{
    return _editing;
}

void AnimeModel::setEditing(bool editing)
{
    _editing = editing;
}

QString AnimeModel::director() const
{
    return _director;
}

void AnimeModel::setDirector(const QString &director)
{
    _director = director;
}

QString AnimeModel::postScoring() const
{
    return _postScoring;
}

void AnimeModel::setPostScoring(const QString &postScoring)
{
    _postScoring = postScoring;
}

AnimeModel::AnimeModel()
{

}

AnimeModel::AnimeModel(const QString id)
{
    setId( id );
    loadFromDatabase();
}

bool AnimeModel::loadFromDatabase()
{
    bool ok;
    QSqlRecord record = MngrQuerys::selectData(sections::anime, id().toULongLong(&ok));
    qDebug() << "Success convert id to ULongLong:" << ok;

    setWantToLook( ! record.value( Tables::Anime::Fields::isHaveLooked ).toBool() );
    setEditing( ! record.value( Tables::Anime::Fields::isEditingDone ).toBool() );

    setTitle( record.value( Tables::Anime::Fields::Title ).toString() );
    setAltTitle( record.value( Tables::Anime::Fields::AltTitle ).toString() );
    setDirector( record.value( Tables::Anime::Fields::Director ).toString() );
    setPostScoring( record.value( Tables::Anime::Fields::PostScoring ).toString() );
    setDate( QDate(record.value( Tables::Anime::Fields::Year ).toInt(), 1, 1) );
    setSeasone( record.value( Tables::Anime::Fields::Season ).toInt() );
    setStudio( record.value( Tables::Anime::Fields::Studios ).toString() );

    setSeries_total_tv(      record.value( Tables::Anime::Fields::SeriesTV      ).toInt() );
    setSeries_total_ova(     record.value( Tables::Anime::Fields::SeriesOVA     ).toInt() );
    setSeries_total_ona(     record.value( Tables::Anime::Fields::SeriesONA     ).toInt() );
    setSeries_total_special( record.value( Tables::Anime::Fields::SeriesSpecial ).toInt() );
    setSeries_total_movie(   record.value( Tables::Anime::Fields::SeriesMovie   ).toInt() );

    setSeries_viewed_tv(      record.value( Tables::Anime::Fields::vSeriesTV      ).toInt() );
    setSeries_viewed_ova(     record.value( Tables::Anime::Fields::vSeriesOVA     ).toInt() );
    setSeries_viewed_ona(     record.value( Tables::Anime::Fields::vSeriesONA     ).toInt() );
    setSeries_viewed_special( record.value( Tables::Anime::Fields::vSeriesSpecial ).toInt() );
    setSeries_viewed_movie(   record.value( Tables::Anime::Fields::vSeriesMovie   ).toInt() );

    setGanres( record.value( Tables::Anime::Fields::Tags ).toString().split(", ") );
    setDescription( record.value( Tables::Anime::Fields::Description ).toString() );
    setLocalPath( record.value( Tables::Anime::Fields::Dir ).toString() );
    setUrl( record.value( Tables::Anime::Fields::Url ).toString() );
    setCover( record.value( Tables::Anime::Fields::ImagePath ).toString() );

    return true;
}












