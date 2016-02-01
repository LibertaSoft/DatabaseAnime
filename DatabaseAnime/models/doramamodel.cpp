#include "doramamodel.h"
#include "mngrquerys.h"

QString DoramaModel::director() const
{
    return _director;
}

void DoramaModel::setDirector(const QString &director)
{
    _director = director;
}

int DoramaModel::season() const
{
    return _season;
}

void DoramaModel::setSeason(int season)
{
    _season = season;
}

int DoramaModel::series_all_tv() const
{
    return _series_all_tv;
}

void DoramaModel::setSeries_all_tv(int series_all_tv)
{
    _series_all_tv = series_all_tv;
}

int DoramaModel::series_all_special() const
{
    return _series_all_special;
}

void DoramaModel::setSeries_all_special(int series_all_special)
{
    _series_all_special = series_all_special;
}

int DoramaModel::series_all_movie() const
{
    return _series_all_movie;
}

void DoramaModel::setSeries_all_movie(int series_all_movie)
{
    _series_all_movie = series_all_movie;
}

int DoramaModel::series_viewed_tv() const
{
    return _series_viewed_tv;
}

void DoramaModel::setSeries_viewed_tv(int series_viewed_tv)
{
    _series_viewed_tv = series_viewed_tv;
}

int DoramaModel::series_viewed_special() const
{
    return _series_viewed_special;
}

void DoramaModel::setSeries_viewed_special(int series_viewed_special)
{
    _series_viewed_special = series_viewed_special;
}

int DoramaModel::series_viewed_movie() const
{
    return _series_viewed_movie;
}

void DoramaModel::setSeries_viewed_movie(int series_viewed_movie)
{
    _series_viewed_movie = series_viewed_movie;
}

QStringList DoramaModel::ganres() const
{
    return _ganres;
}

void DoramaModel::setGanres(const QStringList &ganres)
{
    _ganres = ganres;
}

QString DoramaModel::roles() const
{
    return _roles;
}

void DoramaModel::setRoles(const QString &roles)
{
    _roles = roles;
}

bool DoramaModel::wantToLook() const
{
    return _wantToLook;
}

void DoramaModel::setWantToLook(bool wantToLook)
{
    _wantToLook = wantToLook;
}

bool DoramaModel::editing() const
{
    return _editing;
}

void DoramaModel::setEditing(bool editing)
{
    _editing = editing;
}

int DoramaModel::score() const
{
    return _score;
}

void DoramaModel::setScore(int score)
{
    _score = score;
}

QStringList DoramaModel::getGanresList()
{
    return QStringList()
            << QObject::tr("Adventure")         << QObject::tr("Friendship")      << QObject::tr("Policy")      << QObject::tr("Vampires")
            << QObject::tr("Art-house")         << QObject::tr("Game")            << QObject::tr("Psychology")	<< QObject::tr("Western")
            << QObject::tr("Biography")         << QObject::tr("Gay theme")       << QObject::tr("Relationship")
            << QObject::tr("Business")          << QObject::tr("Gender intrigue") << QObject::tr("Romance")
            << QObject::tr("Comedy")            << QObject::tr("Historical")      << QObject::tr("School")
            << QObject::tr("Crime")             << QObject::tr("Horror")          << QObject::tr("Sci-Fi")
            << QObject::tr("Daily occurrence")	<< QObject::tr("Lesbian theme")   << QObject::tr("Sitcom")
            << QObject::tr("Detective")         << QObject::tr("Live-action")     << QObject::tr("Sport")
            << QObject::tr("Documentary")       << QObject::tr("Martial Arts")    << QObject::tr("Suspense")
            << QObject::tr("Drama")             << QObject::tr("Medicine")        << QObject::tr("Taiga")
            << QObject::tr("Erotic")            << QObject::tr("Melodrama")       << QObject::tr("Tale")
            << QObject::tr("Family")            << QObject::tr("Military")        << QObject::tr("Thriller")
            << QObject::tr("Fantastic")         << QObject::tr("Musical")         << QObject::tr("Tokusatsu")
            << QObject::tr("Fantasy")           << QObject::tr("Mysticism")       << QObject::tr("Tragedy")
            << QObject::tr("Fighter")           << QObject::tr("Parody")          << QObject::tr("TV Show");
}

DoramaModel::DoramaModel()
{}

DoramaModel::DoramaModel(const QString &id)
{
    setId( id );
    loadFromDatabase();
}

bool DoramaModel::save(const bool update)
{
    KeyValue data = toKeyValue();
    if( update ){
        return MngrQuerys::updateDorama(data);
    }
    return MngrQuerys::insertDorama(data);
}

bool DoramaModel::loadFromDatabase()
{
    bool ok;
    QSqlRecord record = MngrQuerys::selectData( sections::dorama, this->id().toULongLong( &ok ) );
    qDebug() << "Success convert id to ULongLong:" << ok; /// \debug

    setWantToLook( record.value( Tables::Dorama::Fields::isHaveLooked ).toBool() == false );
    setEditing( record.value( Tables::Dorama::Fields::isEditingDone ).toBool() == false );
    setTitle( record.value( Tables::Dorama::Fields::Title ).toString() );
    setAltTitle( record.value( Tables::Dorama::Fields::AltTitle ).toString() );
    setDirector( record.value( Tables::Dorama::Fields::Director ).toString() );
    setDate( QDate(record.value( Tables::Dorama::Fields::Year ).toInt(), 1, 1) );
    setSeason( record.value( Tables::Dorama::Fields::Season ).toInt() );
    setSeries_all_tv( record.value( Tables::Dorama::Fields::SeriesTV ).toInt() );
    setSeries_all_special( record.value( Tables::Dorama::Fields::SeriesSpecial ).toInt() );
    setSeries_all_movie( record.value( Tables::Dorama::Fields::SeriesMovie ).toInt() );
    setSeries_viewed_tv( record.value( Tables::Dorama::Fields::vSeriesTV ).toInt() );
    setSeries_viewed_special( record.value( Tables::Dorama::Fields::vSeriesSpecial ).toInt() );
    setSeries_viewed_movie( record.value( Tables::Dorama::Fields::vSeriesMovie ).toInt() );
    setGanres( record.value( Tables::Dorama::Fields::Tags ).toString().split(",") );
    setDescription( record.value( Tables::Dorama::Fields::Description ).toString() );
    setRoles( record.value( Tables::Dorama::Fields::Actors ).toString() );
    setLocalPath( record.value( Tables::Dorama::Fields::Dir ).toString() );
    setUrl( record.value( Tables::Dorama::Fields::Url ).toString() );
    setCover( record.value( Tables::Dorama::Fields::ImagePath ).toString() );

    return true;
}

KeyValue DoramaModel::toKeyValue()
{
    using namespace Tables::Dorama::Fields;
    KeyValue data;

    data[isHaveLooked]   = ! wantToLook();
    data[isEditingDone]  = ! editing();
    data[isAdult]        = false;
    data[Tables::Dorama::Fields::id] = this->id();

    data[Title]          = title();

    data[AltTitle]       = altTitle();
    data[Director]       = director();
    data[SeriesTV]       = series_all_tv();
    data[SeriesSpecial]  = series_all_special();
    data[SeriesMovie]    = series_all_movie();
    data[vSeriesTV]      = series_viewed_tv();
    data[vSeriesSpecial] = series_viewed_special();
    data[vSeriesMovie]   = series_viewed_movie();
    data[Score]          = score();
    data[Year]           = date().year();
    data[Season]         = season();
    data[Tags]           = ganres().join(",");
    data[Description]    = description();
    data[Actors]         = roles();
    data[Url]            = url();
    data[Dir]            = localPath();
    data[ImagePath]      = cover();

    return data;
}

bool DoramaModel::fromKeyValue(const KeyValue &data)
{
    using namespace Tables::Dorama::Fields;

    setWantToLook( ! data[isHaveLooked].toBool() );
    setEditing( ! data[isEditingDone].toBool() );
    setId( QString::number( data[Tables::Dorama::Fields::id].toULongLong() ) );
    setTitle( data[Title].toString() );
    setAltTitle( data[AltTitle].toString() );
    setDirector( data[Director].toString() );
    setSeries_all_tv( data[SeriesTV].toInt() );
    setSeries_all_special( data[SeriesSpecial].toInt() );
    setSeries_all_movie( data[SeriesMovie].toInt() );
    setSeries_viewed_tv( data[vSeriesTV].toInt() );
    setSeries_viewed_special( data[vSeriesSpecial].toInt() );
    setSeries_viewed_movie( data[vSeriesMovie].toInt() );
    setScore( data[Score].toInt() );
    setDate( QDate( data[Year].toInt(), 1, 1 ) );
    setSeason( data[Season].toInt() );
    setGanres( data[Tags].toString().split(",") );
    setDescription( data[Description].toString() );
    setRoles( data[Actors].toString() );
    setUrl( data[Url].toString() );
    setLocalPath( data[Dir].toString() );
    setCover( data[ImagePath].toString() );

    return true;
}
