#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class shikimoriApi : public QObject {
    Q_OBJECT
private:
    QString _lang;

    QStringList jsonParse_search(QByteArray data);
    unsigned long long jsonParse_getId(QByteArray data);
	
    QMap<QString,QVariant> jsonParse_animeData(QByteArray data);
	QMap<QString,QVariant> jsonParse_mangaData(QByteArray data);

public:
    explicit shikimoriApi(QString lang = "en", QObject* parent = 0);

    void searchAnime(QString title, short limit = 10);
    void searchManga(QString title, short limit = 10);

    void getAnimeId(QString title);
    void getMangaId(QString title);

    void pullAnimeData(unsigned long long id);
    void pullMangaData(unsigned long long id);

signals:
    void dataRecived_animeSearch(QStringList animeList);
    void dataRecived_mangaSearch(QStringList mangaList);

    void dataRecived_animeId(unsigned long long id);
    void dataRecived_mangaId(unsigned long long id);
    
    void dataRecived_animeData(QMap<QString,QVariant> data);
    void dataRecived_mangaData(QMap<QString,QVariant> data);

private slots:
    /*внутренние слоты класса излучают сигналы с данными*/
    void replyAnimeSearch(QNetworkReply*);
    void replyMangaSearch(QNetworkReply*);

    void replyGetAnimeId(QNetworkReply*);
    void replyGetMangaId(QNetworkReply*);

    void replyAnimeData(QNetworkReply*);
    void replyMangaData(QNetworkReply*);
};
