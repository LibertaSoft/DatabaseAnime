#ifndef SHIKIMORI_API_H
#define SHIKIMORI_API_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDate>
#include <QMap>

//const QString shikimoriUrl("https://shikimori.org");

/*! \~russian
 * \brief Класс взаимодействия с API сайта shikimori.org
 *
 * Класс предоставляющий доступ к api сайта shikimori.org
 * интерфейс взаимоидействие которого происходит
 * посредствам сигналов и слотов В ответ на запрашиваемые действия класс излучает сигналы
 * с уже распарсенными данными, когда они будут загружены.
 */
/*! \~english
 * \brief Class interactions with site API shikimori.org
 *
 * Class the providing access to site shikimori.org api
 * the interface which vzaimoideystviye occurs
 * by means of signals and slots In response to required actions the class radiates signals
 * with already rasparsenny data when they are loaded.
 */
/*!
 * \code
   void dialog::btn_search_clecked()
   {
       shikimoriApi api;
       connect(api, SIGNAL(dataRecived_animeSearch(QStringLst)),
               this, SLOT(updateCompleter(QStringLst)) );
       api.searchAnime("Hunter x Hunter");
   }

   void dialog::updateCompleter(QStringLst list)
   {
       completerModel.setStringList(list);
   }
 * \endcode
 */
class ShikimoriApi : public QObject {
    Q_OBJECT
private:
    QString _lang;
    QNetworkRequest request( QUrl& url );

    struct TitleLists{
        QStringList eng;
        QStringList rus;
    };
    TitleLists jsonParse_search(QByteArray data);
    quint64 jsonParse_getId(QByteArray data);

    QMap<QString,QVariant> jsonParse_animeData(QByteArray data);
    QMap<QString,QVariant> jsonParse_mangaData(QByteArray data);

public:
    explicit ShikimoriApi(QString lang = "en", QObject* parent = 0);
    void setLang(QString lang);
    static QString getShikimoriUrl();

    void searchAnime(QString title, int limit = 10);
    void searchManga(QString title, int limit = 10);

    void getAnimeId(QString title);
    void getMangaId(QString title);

    void pullAnimeData(quint64 id);
    void pullMangaData(quint64 id);

signals:
//    void dataRecived_animeSearch(QStringList eng);
    void dataRecived_animeSearch(QStringList eng, QStringList rus);
//    void dataRecived_mangaSearch(QStringList eng);
    void dataRecived_mangaSearch(QStringList eng, QStringList rus);

    void dataRecived_animeId(quint64 id);
    void dataRecived_mangaId(quint64 id);

    void dataRecived_animeData(QMap<QString,QVariant> data);
    void dataRecived_mangaData(QMap<QString,QVariant> data);

private slots:
    /*внутренние слоты класса излучают сигналы с данными*/
    /// \~russian Приватные слоты класса, служат для внутренней организации класса
    /// и переводят принятые данные с сайта в готовый для использования вид
    /// \~english The private slots of the class, provided the data from response to signal
    void replyAnimeSearch(QNetworkReply*);
    void replyMangaSearch(QNetworkReply*);

    void replyGetAnimeId(QNetworkReply*);
    void replyGetMangaId(QNetworkReply*);

    void replyAnimeData(QNetworkReply*);
    void replyMangaData(QNetworkReply*);
};

#endif // SHIKIMORI_API_H
