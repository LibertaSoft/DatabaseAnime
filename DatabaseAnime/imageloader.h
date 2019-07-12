#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>

#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QImageReader>
#include <QImage>
#include <QUrl>

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = 0);

signals:
    void imageLoaded( QImage image );

public slots:
    void getImage(const QUrl &url);

private slots:
    void loadFinished(QNetworkReply*r);
};

#endif // IMAGELOADER_H
