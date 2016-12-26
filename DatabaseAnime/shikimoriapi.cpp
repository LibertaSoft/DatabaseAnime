#include "shikimoriapi.h"
#include "mngrquerys.h"

ShikimoriApi::ShikimoriApi(QString lang, QObject *parent)
    :QObject(parent)
{
    _lang = lang;
}

/*! \~russian
 * \brief Устанавливает язык для поиска на сайте
 * \param lang - язык для поиска на сайте \warning используется только для тегов
 * \todo использовать для остальных полей,
 *  я не знаю о наличии информации на английском языке на самом сайте
 */
/*! \~english
 * \brief Setting language for search on the website
 * \param lang - language for search on website \warning Used only for tags
 * \todo use for other field, i don't know of an exists english info on website
 */
void ShikimoriApi::setLang(QString lang)
{
    _lang = lang;
}

QString ShikimoriApi::getShikimoriUrl()
{
    QSettings cfg;
    if( cfg.value( Options::Network::USE_SSL, true ) == true ) {
        return "https://shikimori.org";
    } else {
        return "http://shikimori.org";
    }
}

/*! \~russian
 * \brief Формирует запрос на указанный url
 * \param url - адрес запроса
 * \return QNetworkRequest - сформированый запрос
 * Формирует QNetworkRequest на указанный url адрес
 * и добавляет заголовок User-Agent.
 */
/*! \~english
 * \brief Creates request for the specified url
 * \param url - request address
 * \return QNetworkRequest - the created request
 * Creates QNetworkRequest to the address specified to url
 * also adds User-Agent header.
 */
QNetworkRequest ShikimoriApi::request(QUrl &url)
{
    QNetworkRequest request;
    request.setUrl( url );
    request.setHeader( QNetworkRequest::UserAgentHeader, qApp->applicationName() +" "+ qApp->applicationVersion() );

    return request;
}

/*! \~russian
 * \brief Разбот JSON ответа от сайта, на поиск имеющихся названий
 * \param data - данные пришедшие в ответе от сайта (JSON)
 * \return QStringList - список названий аниме/манги
 * \todo Исправить хардкод obj["russian"]
 */
/*! \~english
 * \brief Parsing answer JSON from website
 * \param data - is a data recieved from the website (JSON)
 * \return QStringList - List of names of an anime/manga
 * \todo Fix a hardcode obj["russian"]
 */
ShikimoriApi::TitleLists ShikimoriApi::jsonParse_search(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();

    TitleLists lists;

    foreach(QJsonValue val, arr){
        QJsonObject obj = val.toObject();
        lists.eng.append( obj["name"].toString() );
        if( obj["russian"].toString().isEmpty() == false )
            lists.rus.append( obj["russian"].toString() );
    }

    return lists;
}

/*! \~russian
 * \brief Разбор JSON ответа от сайта, на поиск по названию
 * \param data - данные пришедшие в ответе от сайта (JSON)
 * \return  quint64 - id записи на сайте
 */
/*! \~english
 * \brief Parsing answer JSON from website, on search of an title
 * \param data - is a data recieved from website (JSON)
 * \return - quing64 - id of record on the website
 */
quint64 ShikimoriApi::jsonParse_getId(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();
    QJsonObject obj = arr.at(0).toObject();
    return obj["id"].toInt();
    #pragma int2ulonglong;
}

/*! \~russian
 * \brief Разбот JSON ответа от сайта, на поиск конкретного аниме
 * \param data - JSON данные принятые с сайта
 * \return QMap<QString,QVariant> - разобраные данные <Ключ-Значение> принятые с сайта
 * \todo Исправить хардкод obj["russian"]
 */
/*! \~english
 * \brief Parsing answer JSON from the site, on search of a specific anime
 * \param data - JSON data recieved from the website
 * \return QMap<QString,QVariant> - parsed data <key-value> recieved from the website
 * \todo Fix a hardcode obj["russian"]
 */
QMap<QString,QVariant> ShikimoriApi::jsonParse_animeData(QByteArray data)
{
    QMap<QString,QVariant> map;

    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonObject obj = doc.object();

    using namespace Tables::Anime::Fields;
    map[Title] = obj["name"].toString();
    map[AltTitle] = obj["russian"].toString();

    //map["director"] = obj["director"].toString();
    //map["PostScoring"] = obj["PostScoring"].toString();

    QDate date = QDate::fromString( obj["aired_on"].toString(), Qt::ISODate );
    map[Year] = date.year();
    //map["Season"] = obj["Season"].toInt();

    QJsonArray studiosArray = obj["studios"].toArray();
    QJsonObject studioObj = studiosArray.at(0).toObject();
    map[Studios] = studioObj["name"].toString();

    /// \todo Fix hardcode field names
    map["Type"] = obj["kind"].toString();
    map["Series"] = obj["episodes"].toInt();

    QJsonArray tagArray = obj["genres"].toArray();
    QString tags;

    bool ruLang = (_lang == "ru")? true : false;
    for( int i = 0; i < tagArray.size(); ++i ){
        QJsonObject tagObj = tagArray.at(i).toObject();
        if( i > 0)
            tags += ", ";

        if( ruLang ){
            tags += tagObj["russian"].toString();
        }else{
            tags += tagObj["name"].toString();
        }
    }
    map[Tags] = tags;
    map[Description] = obj["description_html"].toString();
    map[Url] = ShikimoriApi::getShikimoriUrl() + obj["url"].toString();
    map[ImagePath] = ( obj["image"].toObject() )["original"].toString();

    return map;
}

/*! \~russian
 * \brief Разбот JSON ответа от сайта, на поиск конкретной манги
 * \param data - JSON данные принятые с сайта
 * \return QMap<QString,QVariant> - разобраные данные <Ключ-Значение> принятые с сайта
 * \todo Исправить хардкод obj["russian"]
 */
/*! \~english
 * \brief Parsing answer JSON from the site, on search of a specific manga
 * \param data - JSON data recieved from the website
 * \return QMap<QString,QVariant> - parsed data <key-value> recieved from the website
 * \todo Fix a hardcode obj["russian"]
 */
QMap<QString,QVariant> ShikimoriApi::jsonParse_mangaData(QByteArray data)
{
    QMap<QString,QVariant> map;

    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonObject obj = doc.object();

    using namespace Tables::Manga::Fields;
    map[Title] = obj["name"].toString();
    map[AltTitle] = obj["russian"].toString();

    //map["director"] = obj["director"].toString();
    //map["PostScoring"] = obj["PostScoring"].toString();

    QDate date = QDate::fromString( obj["aired_on"].toString(), Qt::ISODate );
    map[Year] = date.year();
    //map["Season"] = obj["Season"].toInt();

    map[Vol] = obj["volumes"].toInt();
    map[Ch] = obj["chapters"].toInt();

    QJsonArray tagArray = obj["genres"].toArray();
    QString tags;

    bool ruLang = (_lang == "ru")? true : false;
    for( int i = 0; i < tagArray.size(); ++i ){
        QJsonObject tagObj = tagArray.at(i).toObject();
        if( i > 0)
            tags += ", ";

        if( ruLang ){
            tags += tagObj["russian"].toString();
        }else{
            tags += tagObj["name"].toString();
        }
    }
    map[Tags] = tags;
    map[Description] = obj["description_html"].toString();
    map[Url] = ShikimoriApi::getShikimoriUrl() + obj["url"].toString();
    map[ImagePath] = ( obj["image"].toObject() )["original"].toString();

    return map;
}

/*! \~russian
 * \brief Метод для поиска аниме по названию
 * \param title - искомое название
 * \param limit - ограничение на количество получаемых данных
 */
/*! \~english
 * \brief Search of anime on the website by title
 * \param title - searching title
 * \param limit - is a limit of the response
 */
void ShikimoriApi::searchAnime(QString title, short limit)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyAnimeSearch);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/animes?search=" + title + "&limit=" + QString::number(limit);

    manager->get( request(url) );
}

/*! \~russian
 * \brief Метод для поиска манги по названию
 * \param title - искомое название
 * \param limit - ограничение на количество получаемых данных
 */
/*! \~english
 * \brief Search of manga on the website by title
 * \param title - searching title
 * \param limit - is a limit of the response
 */
void ShikimoriApi::searchManga(QString title, short limit)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyMangaSearch);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/mangas?search=" + title + "&limit=" + QString::number(limit);

    manager->get( request(url) );
}

/*! \~russian
 * \brief Метод для получения id аниме на сайте, по полному названию
 * \param title - искомое название
 * \return результат приходит в слот ShikimoriApi::replyGetAnimeId
 * который излучает сигнал dataRecived_animeId(quint64 id)
 */
/*! \~english
* \brief the Method for receiving id of an anime on the site, according to the full name
 * \param title - the required name
 * \return the result comes to ShikimoriApi slot:: replyGetAnimeId
 * which radiates a signal of dataRecived_animeId(quint64 id)
 */
void ShikimoriApi::getAnimeId(QString title)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyGetAnimeId);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/animes?search=" + title + "&limit=1";

    manager->get( request(url) );
}

/*! \~russian
 * \brief Метод для получения id манги на сайте, по полному названию
 * \param title - искомое название
 * \return результат приходит в слот ShikimoriApi::replyGetMangaId
 * который излучает сигнал dataRecived_mangaId(quint64 id)
 */
/*! \~english
* \brief the Method for receiving id of an manga on the website, according to the full name
 * \param title - the required name
 * \return the result comes to ShikimoriApi slot:: replyGetMangaId
 * which radiates a signal of dataRecived_mangaId(quint64 id)
 */
void ShikimoriApi::getMangaId(QString title)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyGetMangaId);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/mangas?search=" + title + "&limit=1";

    manager->get( request(url) );
}

/*! \~russian
 * \brief Метод для получения данных об аниме по его id
 * \param id - идентификатор требуемого аниме
 * \return результат приходит в слот ShikimoriApi::replyAnimeData
 * который излучает сигнал ShikimoriApi::dataRecived_animeData(QMap<QString,QVariant> data)
 */
/*! \~english
* \brief the Method for data acquisition about an anime on its id
 * \param id - the identifier of the requirement anime
 * \return the result comes to ShikimoriApi slot::replyAnimeData
 * which radiates signal ShikimoriApi::dataRecived_animeData(QMap<QString,QVariant> data)
 */
void ShikimoriApi::pullAnimeData(quint64 id)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyAnimeData);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/animes/" + QString::number(id);

    manager->get( request(url) );
}

/*! \~russian
 * \brief Метод для получения данных об аниме по его id
 * \param id - идентификатор требуемого аниме
 * \return результат приходит в слот ShikimoriApi::replyAnimeData
 * который излучает сигнал ShikimoriApi::dataRecived_animeData(QMap<QString,QVariant> data)
 */
/*! \~english
* \brief the Method for data acquisition about an manga on its id
 * \param id - the identifier of the requirement manga
 * \return the result comes to ShikimoriApi slot::replyMangaData
 * which radiates signal ShikimoriApi::dataRecived_mangaData(QMap<QString,QVariant> data)
 */
void ShikimoriApi::pullMangaData(quint64 id)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this,    &ShikimoriApi::replyMangaData);

    QUrl url = ShikimoriApi::getShikimoriUrl() + "/api/mangas/" + QString::number(id);

    manager->get( request(url) );
}

void ShikimoriApi::replyAnimeSearch(QNetworkReply* reply)
{
//    Redirect ???
//    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
//    QUrl newUrl = redirectionTarget.toUrl();

    TitleLists lists = jsonParse_search( reply->readAll() );
    emit dataRecived_animeSearch( lists.eng, lists.rus );
    reply->deleteLater();
}

void ShikimoriApi::replyMangaSearch(QNetworkReply* reply)
{
    TitleLists lists = jsonParse_search( reply->readAll() );
    emit dataRecived_mangaSearch( lists.eng, lists.rus );
    reply->deleteLater();
}

void ShikimoriApi::replyGetAnimeId(QNetworkReply* reply)
{
    emit dataRecived_animeId( jsonParse_getId( reply->readAll() ) );
    reply->deleteLater();
}

void ShikimoriApi::replyGetMangaId(QNetworkReply* reply)
{
    emit dataRecived_mangaId( jsonParse_getId( reply->readAll() ) );
    reply->deleteLater();
}

void ShikimoriApi::replyAnimeData(QNetworkReply* reply)
{
    emit dataRecived_animeData( jsonParse_animeData( reply->readAll() ) );
    reply->deleteLater();
}

void ShikimoriApi::replyMangaData(QNetworkReply* reply)
{
    emit dataRecived_mangaData( jsonParse_mangaData( reply->readAll() ) );
    reply->deleteLater();
}
