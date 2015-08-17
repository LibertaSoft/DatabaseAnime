#include "imageloader.h"

ImageLoader::ImageLoader(QObject *parent) : QObject(parent)
{}

void ImageLoader::getImage(const QUrl &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(loadFinished(QNetworkReply*)));

    manager->get( QNetworkRequest(url) );
}

void ImageLoader::loadFinished(QNetworkReply *r)
{
    QImageReader imageReader(r);
    QImage image;
    if (r->error() == QNetworkReply::NoError)
        imageReader.read(&image);
    else
        qDebug() << "network error";

    emit imageLoaded( image );
    r->deleteLater();
}

