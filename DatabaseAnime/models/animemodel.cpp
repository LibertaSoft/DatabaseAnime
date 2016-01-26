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

bool AnimeModel::getIsAdult() const
{
    return _isAdult;
}

void AnimeModel::setIsAdult(bool isAdult)
{
    _isAdult = isAdult;
}

int AnimeModel::getScore() const
{
    return _score;
}

void AnimeModel::setScore(int score)
{
    _score = score;
}

QStringList AnimeModel::getGanresList()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Futunari")          << QObject::tr("Music")             << QObject::tr("Shounen")
            << QObject::tr("Adventure")         << QObject::tr("Game")              << QObject::tr("Mystery")           << QObject::tr("Shounen-ai (Yaoi)")
            << QObject::tr("Bara")              << QObject::tr("Gender intrigue")	<< QObject::tr("Parody")            << QObject::tr("Space")
            << QObject::tr("Comedy")            << QObject::tr("Guro")              << QObject::tr("Philosophy")        << QObject::tr("Sport")
            << QObject::tr("Cooking")           << QObject::tr("Harem")             << QObject::tr("Police")            << QObject::tr("Steampunk")
            << QObject::tr("Cyberpunk")         << QObject::tr("Hentai")            << QObject::tr("Post-apocalyptic")	<< QObject::tr("Supernatural")
            << QObject::tr("Daily occurrence")	<< QObject::tr("Historical")        << QObject::tr("Psychology")        << QObject::tr("Tale")
            << QObject::tr("Demons")            << QObject::tr("Horror")            << QObject::tr("Ranobe")            << QObject::tr("Tentacles")
            << QObject::tr("Detective")         << QObject::tr("Josei")             << QObject::tr("Romantic")          << QObject::tr("The children's")
            << QObject::tr("Drama")             << QObject::tr("Kodomo")            << QObject::tr("Samurai")           << QObject::tr("Thriller")
            << QObject::tr("Ecchi")             << QObject::tr("Madness")           << QObject::tr("School")            << QObject::tr("Tragedy")
            << QObject::tr("Family")            << QObject::tr("Magic")             << QObject::tr("Science Fiction")	<< QObject::tr("Vampires")
            << QObject::tr("Fantastic")         << QObject::tr("Magical girl")      << QObject::tr("Seinen")            << QObject::tr("War")
            << QObject::tr("Fantasy")           << QObject::tr("Martial Arts")      << QObject::tr("Shojo")             << QObject::tr("Yonkoma")
            << QObject::tr("Fighter")           << QObject::tr("Mech")              << QObject::tr("Shojo-ai (Yuri)");
}

AnimeModel::AnimeModel()
{

}

AnimeModel::AnimeModel(const QString id)
{
    setId( id );
    loadFromDatabase();
}

bool AnimeModel::save(const bool update)
{
    KeyValue data = toKeyValue();
    if( update ){
        return MngrQuerys::updateAnime(data);
    }
    return MngrQuerys::insertAnime(data);
}

bool AnimeModel::loadFromDatabase()
{
    bool ok;
    QSqlRecord record = MngrQuerys::selectData(sections::anime, id().toULongLong(&ok));
    qDebug() << "Success convert id to ULongLong:" << ok; /// \debug

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

KeyValue AnimeModel::toKeyValue()
{
    using namespace Tables::Anime::Fields;
    KeyValue data;

    data[isHaveLooked]   = wantToLook();
    data[isEditingDone]  = editing();
    data[isAdult]        = false;
    data[Tables::Anime::Fields::id]             = id().toULongLong();
    data[Title]          = title();
    data[AltTitle]       = altTitle();
    data[Director]       = director();
    data[PostScoring]    = postScoring();
    data[SeriesTV]       = series_total_tv();
    data[SeriesOVA]      = series_total_ova();
    data[SeriesONA]      = series_total_ona();
    data[SeriesSpecial]  = series_total_special();
    data[SeriesMovie]    = series_total_movie();
    data[vSeriesTV]      = series_viewed_tv();
    data[vSeriesOVA]     = series_viewed_ova();
    data[vSeriesONA]     = series_viewed_ona();
    data[vSeriesSpecial] = series_viewed_special();
    data[vSeriesMovie]   = series_viewed_movie();
    data[Score]          = 0;
    data[Year]           = date().year();
    data[Season]         = seasone();
    data[Studios]        = studio();
    data[Tags]           = ganres().join(",");
    data[Description]    = description();
    data[Url]            = url();
    data[Dir]            = localPath();
    data[ImagePath]      = cover();

    return data;
}

bool AnimeModel::fromKeyValue(const KeyValue &data)
{
    using namespace Tables::Anime::Fields;

    setWantToLook( data[isHaveLooked].toBool() );
    setEditing( data[isEditingDone].toBool() );
    setIsAdult( data[isAdult].toBool() );

    setId( data[Tables::Anime::Fields::id].toString() ); /// \todo \fixme
    setTitle( data[Title].toString() );
    setAltTitle( data[AltTitle].toString() );
    setDirector( data[Director].toString() );
    setPostScoring( data[PostScoring].toString() );

    setSeries_total_tv( data[SeriesTV].toInt() );
    setSeries_total_ova( data[SeriesOVA].toInt() );
    setSeries_total_ona( data[SeriesONA].toInt() );
    setSeries_total_special( data[SeriesSpecial].toInt() );
    setSeries_total_movie( data[SeriesMovie].toInt() );

    setSeries_viewed_tv( data[vSeriesTV].toInt() );
    setSeries_viewed_ova( data[vSeriesOVA].toInt() );
    setSeries_viewed_ona( data[vSeriesONA].toInt() );
    setSeries_viewed_special( data[vSeriesSpecial].toInt() );
    setSeries_viewed_movie( data[vSeriesMovie].toInt() );

    setScore( data[Score].toInt() );
    setDate( QDate(data[Year].toInt(), 1, 1) );
    setSeasone( data[Season].toInt() );
    setStudio( data[Studios].toString() );
    QString tags = data[Tags].toString();
    setGanres( tags.split(", ") );
    setDescription( data[Description].toString() );
    setUrl( data[Url].toString() );
    setLocalPath( data[Dir].toString() );
    setCover( data[ImagePath].toString() );

    return true;
}












