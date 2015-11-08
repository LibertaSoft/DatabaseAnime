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

    bool _emptyImage = true;
public:
    explicit ImageCover(QWidget *parent = 0);
    QString getImagePath() const;
    void setImagePath(QString);
    QSize sizeHint() const;
    bool isNullImage();

    // Actions context menu
    QAction *actionChooseImage;
    QAction *actionSetNoImage;
    QAction *actionReloadCover;
signals:
    void reloadCover();
public slots:
    void chooseImage();
    void noImage();
    void setPixmap(const QPixmap &pixmap);
    void enableReloadButton(const bool enable = true);

protected:
    virtual void dragEnterEvent(QDragEnterEvent*);
    virtual void dropEvent(QDropEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void contextMenuEvent(QContextMenuEvent*);
    virtual void resizeEvent(QResizeEvent*);

private slots:
    void slotReloadCover();
};

#endif // IMAGECOVER_H
