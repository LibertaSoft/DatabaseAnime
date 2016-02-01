#include "amvmodel.h"
#include "mngrquerys.h"

QString AmvModel::author() const
{
    return _author;
}

void AmvModel::setAuthor(const QString &author)
{
    _author = author;
}

QString AmvModel::contestant() const
{
    return _contestant;
}

void AmvModel::setContestant(const QString &contestant)
{
    _contestant = contestant;
}

QStringList AmvModel::tags() const
{
    return _tags;
}

void AmvModel::setTags(const QStringList &tags)
{
    _tags = tags;
}

QString AmvModel::sourceVideo() const
{
    return _sourceVideo;
}

void AmvModel::setSourceVideo(const QString &sourceVideo)
{
    _sourceVideo = sourceVideo;
}

QString AmvModel::sourceMusic() const
{
    return _sourceMusic;
}

void AmvModel::setSourceMusic(const QString &sourceMusic)
{
    _sourceMusic = sourceMusic;
}

QString AmvModel::authorComment() const
{
    return _authorComment;
}

void AmvModel::setAuthorComment(const QString &authorComment)
{
    _authorComment = authorComment;
}

bool AmvModel::editing() const
{
    return _editing;
}

void AmvModel::setEditing(bool editing)
{
    _editing = editing;
}

int AmvModel::score() const
{
    return _score;
}

void AmvModel::setScore(int score)
{
    _score = score;
}

QStringList AmvModel::getTagsList()
{
    return QStringList()
            << QObject::tr("Action")            << QObject::tr("Drama")         << QObject::tr("M@D")                   << QObject::tr("Romance")
            << QObject::tr("Character Profile")	<< QObject::tr("Ecchi")         << QObject::tr("MEP")                   << QObject::tr("Sentimental")
            << QObject::tr("Comedy (Fun)")      << QObject::tr("GMV")           << QObject::tr("MMV")                   << QObject::tr("Story")
            << QObject::tr("Cross-Over")        << QObject::tr("Horror")        << QObject::tr("Original Animation")	<< QObject::tr("Trailer")
            << QObject::tr("Dance")             << QObject::tr("Instrumental")  << QObject::tr("Psychedelic");
}

AmvModel::AmvModel()
{}

AmvModel::AmvModel(const QString &id)
{
    setId( id );
    loadFromDatabase();
}

bool AmvModel::save(const bool update)
{
    KeyValue data = this->toKeyValue();
    /// \todo add logging
    if( update ){
        return MngrQuerys::insertAmv(data);
    }
    return MngrQuerys::updateAmv(data);
}

bool AmvModel::loadFromDatabase()
{
    QSqlRecord record = MngrQuerys::selectData( sections::amv, id().toULongLong() );

    setEditing( record.value( Tables::Amv::Fields::isEditingDone ).toBool() == false );
    setTitle( record.value( Tables::Amv::Fields::Title ).toString() );
    setAuthor( record.value( Tables::Amv::Fields::Author ).toString() );
    setContestant( record.value( Tables::Amv::Fields::Contestant ).toString() );
    setDate( QDate(record.value( Tables::Amv::Fields::Year ).toInt(), 1, 1) );
    setTags( record.value( Tables::Amv::Fields::Tags ).toString().split(",") );
    setAuthorComment( record.value( Tables::Amv::Fields::AuthorComment ).toString() );
    setSourceVideo( record.value( Tables::Amv::Fields::ContainingAnime ).toString() );
    setSourceMusic( record.value( Tables::Amv::Fields::ContainingMusic ).toString() );
    setLocalPath( record.value( Tables::Amv::Fields::Dir ).toString() );
    setUrl( record.value( Tables::Amv::Fields::Url ).toString() );
    setCover( record.value( Tables::Amv::Fields::ImagePath ).toString() );

    return true;
}

KeyValue AmvModel::toKeyValue()
{
    using namespace Tables::Amv::Fields;
    KeyValue data;

    data[Tables::Amv::Fields::id] = this->id();
    data[isEditingDone] = ! editing();
    data[isAdult] =  false;
    data[Title] = title();
    data[Author] =  author();
    data[Contestant] =  contestant();
    data[Score] =  score();
    data[Year] = date().year();
    data[Tags] = tags().join(",");
    data[ContainingMusic] = sourceMusic();
    data[ContainingAnime] = sourceVideo();
    data[AuthorComment] = authorComment();
    data[Url] = url();
    data[Dir] = localPath();
    data[ImagePath] = cover();

    return data;
}

bool AmvModel::fromKeyValue(const KeyValue &data)
{
    using namespace Tables::Amv::Fields;
    setId( data[Tables::Amv::Fields::id].toString() );
    setEditing( data[isEditingDone].toBool() );
    setTitle( data[Title].toString() );
    setAuthor( data[Author].toString() );
    setContestant( data[Contestant].toString() );
    setScore( data[Score].toInt() );
    setDate( QDate(data[Year].toInt(), 1, 1) );
    setTags( data[Tags].toString().split(",") );
    setSourceMusic( data[ContainingMusic].toString() );
    setSourceVideo( data[ContainingAnime].toString() );
    setAuthorComment( data[AuthorComment].toString() );
    setUrl( data[Url].toString() );
    setLocalPath( data[Dir].toString() );
    setCover( data[ImagePath].toString() );

    return true;
}
