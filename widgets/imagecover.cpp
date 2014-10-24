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

ImageCover::ImageCover(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);

    actionChooseImage = new QAction( QIcon("://images/list-add.png"), tr("&Choose image"),this);
    actionSetNoImage  = new QAction( QIcon("://images/list-remove.png"), tr("Clea&n image"),this);
    cMenu.addAction(actionChooseImage);
    cMenu.addAction(actionSetNoImage);

    QObject::connect(actionChooseImage, SIGNAL(triggered()),
                    this, SLOT(chooseImage())  );
    QObject::connect(actionSetNoImage, SIGNAL(triggered()),
                    this, SLOT(noImage())  );

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

void ImageCover::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open picture"), QStandardPaths::writableLocation( QStandardPaths::PicturesLocation ),
                                                    tr("Images")+" (*.png *.PNG *.jpg *.JPG *.jpeg *.JPEG)");
    if( !fileName.isEmpty() ){
        setImagePath( fileName );
        setPixmap( QPixmap( fileName ) );
    }
}

void ImageCover::noImage()
{
    imagePath.clear();
    QPixmap p( "://images/DropHere.png" );
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
    QPixmap pix( urlList.at(0).toLocalFile() );
    if( !pix.isNull() ){
        this->setPixmap( pix );
        setImagePath( urlList.at(0).toLocalFile() );
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
