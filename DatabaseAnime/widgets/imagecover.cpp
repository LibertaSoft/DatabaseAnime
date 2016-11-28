#include "imagecover.h"

#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>

#include <QApplication>
#include <QStandardPaths>
#include <QPixmap>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>

#include <QMessageBox>
#include <QDebug>

ImageCover::ImageCover(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);

    actionChooseImage = new QAction( QIcon("://images/list-add.png"), tr("&Choose image"),this);
    actionSetNoImage  = new QAction( QIcon("://images/list-remove.png"), tr("Clea&n image"),this);
    actionReloadCover = new QAction( QIcon("://images/refresh.png"), tr("&Reload image"),this);
    cMenu.addAction(actionChooseImage);
    cMenu.addAction(actionSetNoImage);

    QObject::connect(actionChooseImage, &QAction::triggered,
                    this, &ImageCover::chooseImage  );
    QObject::connect(actionSetNoImage,  &QAction::triggered,
                    this, &ImageCover::noImage  );
    QObject::connect(actionReloadCover, &QAction::triggered,
                    this, &ImageCover::slotReloadCover  );

    noImage();
}

QString ImageCover::getImagePath() const{
    return imagePath;
}
void ImageCover::setImagePath( QString path ){
    imagePath = path;
}

QSize ImageCover::sizeHint() const
{
    return QSize( 194 , 582 );
}

bool ImageCover::isNullImage()
{
    return _emptyImage;
}

void ImageCover::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open picture"), QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ),
                                                    tr("Images")+" (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG)");

    if( ! fileName.isEmpty() ){
        _emptyImage = false;
        setImagePath( fileName );
        setPixmap( QPixmap( fileName ) );
    }
}

void ImageCover::noImage()
{
    imagePath.clear();
    QPixmap p( "://images/DropHere.png" );
    this->setPixmap(p);
    _emptyImage = true;
}

void ImageCover::setPixmap(const QPixmap &pixmap)
{
    QLabel::setPixmap( pixmap );
    _emptyImage = false;
}

void ImageCover::enableReloadButton(const bool enable)
{
    if( enable ){
        cMenu.addAction( actionReloadCover );
    } else {
        cMenu.removeAction( actionReloadCover );
    }
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
    QPixmap pix( urlList.at(0).toLocalFile() );
    if( !pix.isNull() ){
        this->setPixmap( pix );
        setImagePath( urlList.at(0).toLocalFile() );
        _emptyImage = false;
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("It was not succeeded to load the picture") );
    }
}

void ImageCover::mouseDoubleClickEvent(QMouseEvent *)
{
    chooseImage();
}

void ImageCover::contextMenuEvent(QContextMenuEvent *pe)
{
    cMenu.exec( pe->globalPos() );
}

void ImageCover::resizeEvent(QResizeEvent *)
{
//    e->oldSize()
//    QMessageBox::information(this, windowTitle(), QString::number(this->height()) );
    //    resize( height()/3, height() );
}

void ImageCover::slotReloadCover()
{
    emit reloadCover();
}
