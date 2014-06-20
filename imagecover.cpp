#include "imagecover.h"
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include <QPixmap>
#include <QMessageBox>
#include <QString>

ImageCover::ImageCover(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);
}

QString ImageCover::getImagePath(){
    return imagePath;
}
void ImageCover::setImagePath( QString& path ){
    imagePath = path;
}

/*virtual*/ void ImageCover::dragEnterEvent(QDragEnterEvent* pe)
{
    if( pe->mimeData()->hasUrls() ){
        pe->acceptProposedAction();
    }
}

/*virtual*/ void ImageCover::dropEvent(QDropEvent* pe)
{
    QList<QUrl> urlList = pe->mimeData()->urls();
    this->imagePath = urlList.at(0).toLocalFile();
//    QMessageBox::information(this, "", pe->mimeData()->data("image/png").toBase64() );
//    this->setPixmap( pe->mimeData()->text() );
//    QImage m(pe->mimeData()->data("image/png").toBase64() );
    this->setPixmap( QPixmap( imagePath ) );
}
