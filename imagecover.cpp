#include "imagecover.h"
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include <QApplication>
#include <QPixmap>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QDateTime>

ImageCover::ImageCover(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);
}

QString ImageCover::getImagePath(){
    return imagePath;
}
void ImageCover::setImagePath( QString path ){
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

    QDir objQdir;
    QString coverPath( QDir::homePath() + "/."+QApplication::organizationName()+"/"+QApplication::applicationName() + "/animeCovers/" );
    if( objQdir.mkpath( coverPath ) ){
        this->imagePath = urlList.at(0).toLocalFile();
        this->setPixmap( QPixmap( urlList.at(0).toLocalFile() ) );
    }else{
        QMessageBox::warning(this, "Внимание", "Не удалось загрузить картинку");
    }


}
