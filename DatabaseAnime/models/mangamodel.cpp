#include "mangamodel.h"
#include "mngrquerys.h"

QString MangaModel::author() const
{
    return _author;
}

void MangaModel::setAuthor(const QString &author)
{
    _author = author;
}

QString MangaModel::translator() const
{
    return _translator;
}

void MangaModel::setTranslator(const QString &translator)
{
    _translator = translator;
}

int MangaModel::series_total_volumes() const
{
    return _series_total_volumes;
}

void MangaModel::setSeries_total_volumes(int series_total_volumes)
{
    _series_total_volumes = series_total_volumes;
}

int MangaModel::series_total_charapters() const
{
    return _series_total_charapters;
}

void MangaModel::setSeries_total_charapters(int series_total_charapters)
{
    _series_total_charapters = series_total_charapters;
}

int MangaModel::series_total_pages() const
{
    return _series_total_pages;
}

void MangaModel::setSeries_total_pages(int series_total_pages)
{
    _series_total_pages = series_total_pages;
}

int MangaModel::series_viewed_volumes() const
{
    return _series_viewed_volumes;
}

void MangaModel::setSeries_viewed_volumes(int series_viewed_volumes)
{
    _series_viewed_volumes = series_viewed_volumes;
}

int MangaModel::series_viewed_charapters() const
{
    return _series_viewed_charapters;
}

void MangaModel::setSeries_viewed_charapters(int series_viewed_charapters)
{
    _series_viewed_charapters = series_viewed_charapters;
}

int MangaModel::series_viewed_pages() const
{
    return _series_viewed_pages;
}

void MangaModel::setSeries_viewed_pages(int series_viewed_pages)
{
    _series_viewed_pages = series_viewed_pages;
}

QStringList MangaModel::ganres() const
{
    return _ganres;
}

void MangaModel::setGanres(const QStringList &ganres)
{
    _ganres = ganres;
}

bool MangaModel::wantToLook() const
{
    return _wantToLook;
}

void MangaModel::setWantToLook(bool wantToLook)
{
    _wantToLook = wantToLook;
}

bool MangaModel::editing() const
{
    return _editing;
}

void MangaModel::setEditing(bool editing)
{
    _editing = editing;
}

QStringList MangaModel::getGanresList()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Futunari")          << QObject::tr("Music")             << QObject::tr("Shounen")
            << QObject::tr("Adventure")         << QObject::tr("Game")              << QObject::tr("Mystery")           << QObject::tr("Shounen-ai (Yaoi)")
            << QObject::tr("Bara")              << QObject::tr("Gender intrigue")   << QObject::tr("Parody")            << QObject::tr("Space")
            << QObject::tr("Comedy")            << QObject::tr("Guro")              << QObject::tr("Philosophy")        << QObject::tr("Sport")
            << QObject::tr("Cooking")           << QObject::tr("Harem")             << QObject::tr("Police")            << QObject::tr("Steampunk")
            << QObject::tr("Cyberpunk")     	<< QObject::tr("Hentai")            << QObject::tr("Post-apocalyptic")	<< QObject::tr("Supernatural")
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

MangaModel::MangaModel()
{

}

MangaModel::MangaModel(const QString &id)
{
    setId( id );
    loadFromDatabase();
}

bool MangaModel::save(const bool update)
{
    KeyValue data = toKeyValue();
    if( update ){
        return MngrQuerys::updateManga(data);
    }
    return MngrQuerys::insertManga(data);
}

bool MangaModel::loadFromDatabase()
{
    bool ok;
    QSqlRecord record = MngrQuerys::selectData( sections::manga, id().toULongLong(&ok) );
    qDebug() << "Success convert id to ULongLong:" << ok; /// \debug
    if( ! ok ) return false;

    setWantToLook( ! record.value( Tables::Manga::Fields::isHaveLooked ).toBool() );
    setEditing( ! record.value( Tables::Manga::Fields::isEditingDone ).toBool() );

    setTitle( record.value( Tables::Manga::Fields::Title ).toString() );

    setAltTitle( record.value( Tables::Manga::Fields::AltTitle ).toString() );
    setAuthor( record.value( Tables::Manga::Fields::Author ).toString() );
    setTranslator( record.value( Tables::Manga::Fields::Translation ).toString() );

    setDate( QDate(record.value( Tables::Manga::Fields::Year ).toInt(), 1, 1) );

    setSeries_total_volumes( record.value( Tables::Manga::Fields::Vol ).toInt() );
    setSeries_total_charapters( record.value( Tables::Manga::Fields::Ch ).toInt() );
    setSeries_total_pages( record.value( Tables::Manga::Fields::Pages ).toInt() );

    setSeries_viewed_volumes( record.value( Tables::Manga::Fields::vVol ).toInt() );
    setSeries_viewed_charapters( record.value( Tables::Manga::Fields::vCh ).toInt() );
    setSeries_viewed_pages( record.value( Tables::Manga::Fields::vPages ).toInt() );

    setGanres( record.value( Tables::Manga::Fields::Tags ).toString().split(",") );
    setDescription( record.value( Tables::Manga::Fields::Description ).toString() );
    setLocalPath( record.value( Tables::Manga::Fields::Dir ).toString() );
    setUrl( record.value( Tables::Manga::Fields::Url ).toString() );

    setCover( record.value( Tables::Manga::Fields::ImagePath ).toString() );

    return true;
}

KeyValue MangaModel::toKeyValue()
{
    using namespace Tables::Manga::Fields;
    KeyValue data;

    data[isHaveLooked]  = ! wantToLook(); /// \todo remove NOT
    data[isEditingDone] = ! editing(); /// \todo remove NOT
    data[Tables::Manga::Fields::id]    = this->id();
    data[Title]         = title();
    data[AltTitle]      = altTitle();
    data[Author]        = author();
    data[Translation]   = translator();
    data[Vol]           = series_total_volumes();
    data[Ch]            = series_total_charapters();
    data[Pages]         = series_total_pages();
    data[vVol]          = series_viewed_volumes();
    data[vCh]           = series_viewed_charapters();
    data[vPages]        = series_viewed_pages();
    data[Year]          = date().year();
    data[Tags]          = ganres().join(",");
    data[Description]   = description();
    data[Url]           = url();
    data[Dir]           = localPath();
    data[ImagePath]     = cover();

    return data;
}

bool MangaModel::fromKeyValue(const KeyValue &data)
{
    using namespace Tables::Manga::Fields;

    setTitle( data[Title].toString() );

    setAltTitle( data[AltTitle].toString() );
    setAuthor( data[Author].toString() );
    setTranslator( data[Translation].toString() );
    setDate( QDate(data[Year].toInt(), 1, 1) );

    setSeries_total_volumes( data[Vol].toInt() );
    setSeries_total_charapters( data[Ch].toInt() );
    setSeries_total_pages( data[Pages].toInt() );

    setGanres( data[Tags].toString().split(",") );

    setDescription( data[Description].toString() );

    setUrl( data[Url].toString() );

    setCover( data[ImagePath].toString() );

    /// \todo write all fields
    return true;
}
