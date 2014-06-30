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
#include <QFileDialog>

ImageCover::ImageCover(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);

    actionChooseImage = new QAction( tr("&Choose image"),this);
    actionSetNoImage  = new QAction( tr("Clea&n image"),this);
    cMenu.addAction(actionChooseImage);
    cMenu.addAction(actionSetNoImage);

    QObject::connect(actionChooseImage, SIGNAL(triggered()),
                    this, SLOT(chooseImage())  );
    QObject::connect(actionSetNoImage, SIGNAL(triggered()),
                    this, SLOT(noImage())  );

    emit noImage();
}

QString ImageCover::getImagePath() const{
    return imagePath;
}
void ImageCover::setImagePath( QString path ){
    imagePath = path;
}

QSize ImageCover::sizeHint() const
{
//    return QSize( 194 , 582 );
    return QSize( heightForWidth( width()/3 ), width() );
}

void ImageCover::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr(""), "Open picture", "*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG");
    if( !fileName.isEmpty() ){
        setImagePath( fileName );
        setPixmap( QPixmap( fileName ) );
    }
}

void ImageCover::noImage()
{
    setImagePath("://images/DBA_NoImage.png");
    QPixmap p( getImagePath() );
    this->setPixmap(p);
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
        setImagePath( urlList.at(0).toLocalFile() );
        this->setPixmap( QPixmap( urlList.at(0).toLocalFile() ) );
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("It was not succeeded to load the picture") );
    }
}

void ImageCover::mouseDoubleClickEvent(QMouseEvent *)
{
    emit chooseImage();
}

void ImageCover::contextMenuEvent(QContextMenuEvent *pe)
{
    cMenu.exec( pe->globalPos() );
}
