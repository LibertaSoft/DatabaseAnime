#ifndef IMAGECOVER_H
#define IMAGECOVER_H

#include <QLabel>
#include <QString>
#include <QMenu>

class ImageCover : public QLabel
{
    Q_OBJECT
private:
    QString imagePath;
    QMenu cMenu;
public:
    explicit ImageCover(QWidget *parent = 0);
    QString getImagePath() const;
    void setImagePath(QString);
    QSize sizeHint() const;

    // Actions context menu
    QAction *actionChooseImage;
    QAction *actionSetNoImage;
signals:
public slots:
    void chooseImage();
    void noImage();
protected:
    virtual void dragEnterEvent(QDragEnterEvent*);
    virtual void dropEvent(QDropEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void contextMenuEvent(QContextMenuEvent*);
    virtual void resizeEvent(QResizeEvent*);
};

#endif // IMAGECOVER_H
