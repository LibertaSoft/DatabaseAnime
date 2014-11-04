#include "shikimoriapi.h"

shikimoriApi::shikimoriApi(QString lang, QObject *parent)
    :QObject(parent)
{
    _lang = lang;
}

void shikimoriApi::setLang(QString lang)
{
    _lang = lang;
}

QStringList shikimoriApi::jsonParse_search(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();

    int k(0);
    QStringList animeList;
    for( QJsonArray::iterator i = arr.begin(); i != arr.end(); ++i ){
        QJsonObject obj = arr.at(k).toObject();
        animeList.append( obj["name"].toString() );
        if( obj["russian"].toString().isEmpty() == false )
            animeList.append( obj["russian"].toString() );
        ++k;
    }
    qDebug() << animeList;
    return animeList;
}

unsigned long long shikimoriApi::jsonParse_getId(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();
    QJsonObject obj = arr.at(0).toObject();
    return obj["id"].toInt();
    #pragma int2ulonglong;
}

QMap<QString,QVariant> shikimoriApi::jsonParse_animeData(QByteArray data)
{
    QMap<QString,QVariant> map;

	QJsonDocument doc = QJsonDocument::fromJson( data );
	QJsonObject obj = doc.object();
	map["Title"] = obj["name"].toString();
	map["AltTitle"] = obj["russian"].toString();
	
	//map["director"] = obj["director"].toString();
	//map["PostScoring"] = obj["PostScoring"].toString();
	
	QDate date = QDate::fromString( obj["aired_on"].toString(), Qt::ISODate );
    map["Year"] = date.year();
	//map["Season"] = obj["Season"].toInt();
	
	QJsonArray studiosArray = obj["studios"].toArray();
	QJsonObject studioObj = studiosArray.at(0).toObject();
	map["Studio"] = studioObj["name"].toString();

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
	map["Tags"] = tags;
	map["Description"] = obj["description_html"].toString();
	map["URL"] = "http://shikimori.org" + obj["url"].toString();
    map["ImagePath"] = (obj["image"].toObject())["original"].toString();

    return map;
}

QMap<QString,QVariant> shikimoriApi::jsonParse_mangaData(QByteArray data)
{
    QMap<QString,QVariant> map;

	QJsonDocument doc = QJsonDocument::fromJson( data );
	QJsonObject obj = doc.object();
	
	map["Title"] = obj["name"].toString();
	map["AltTitle"] = obj["russian"].toString();
	
	//map["director"] = obj["director"].toString();
	//map["PostScoring"] = obj["PostScoring"].toString();
	
	QDate date = QDate::fromString( obj["aired_on"].toString(), Qt::ISODate );
	map["Year"] = date.year();
	//map["Season"] = obj["Season"].toInt();
	
	map["Vol"] = obj["volumes"].toInt();
	map["Ch"] = obj["chapters"].toInt();
		
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
	map["Tags"] = tags;
	map["Description"] = obj["description_html"].toString();
	map["URL"] = "http://shikimori.org" + obj["url"].toString();
    map["ImagePath"] = (obj["image"].toObject())["original"].toString();

    return map;
}

void shikimoriApi::searchAnime(QString title, short limit)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyAnimeSearch(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/animes?search=" + title + "&limit=" + QString::number(limit);
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::searchManga(QString title, short limit)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyMangaSearch(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/mangas?search=" + title + "&limit=" + QString::number(limit);
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::getAnimeId(QString title)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyGetAnimeId(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/animes?search=" + title + "&limit=1";
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::getMangaId(QString title)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyGetMangaId(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/mangas?search=" + title + "&limit=1";
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::pullAnimeData(unsigned long long id)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyAnimeData(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/animes/" + QString::number(id);
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::pullMangaData(unsigned long long id)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this,    SLOT(replyMangaData(QNetworkReply*)));

    QUrl url = "http://shikimori.org/api/mangas/" + QString::number(id);
    manager->get( QNetworkRequest( url ) );
}

void shikimoriApi::replyAnimeSearch(QNetworkReply* reply)
{
    emit dataRecived_animeSearch( jsonParse_search( reply->readAll() ) );
    reply->deleteLater();
}

void shikimoriApi::replyMangaSearch(QNetworkReply* reply)
{
    emit dataRecived_mangaSearch( jsonParse_search( reply->readAll() ) );
    reply->deleteLater();
}

void shikimoriApi::replyGetAnimeId(QNetworkReply* reply)
{
    emit dataRecived_animeId( jsonParse_getId( reply->readAll() ) );
    reply->deleteLater();
}

void shikimoriApi::replyGetMangaId(QNetworkReply* reply)
{
    emit dataRecived_mangaId( jsonParse_getId( reply->readAll() ) );
    reply->deleteLater();
}

void shikimoriApi::replyAnimeData(QNetworkReply* reply)
{
    emit dataRecived_animeData( jsonParse_animeData( reply->readAll() ) );
    reply->deleteLater();
}

void shikimoriApi::replyMangaData(QNetworkReply* reply)
{
    emit dataRecived_mangaData( jsonParse_mangaData( reply->readAll() ) );
    reply->deleteLater();
}
