#ifndef IMAGECOVER_H
#define IMAGECOVER_H

#include <QLabel>
#include <QString>

class ImageCover : public QLabel
{
    Q_OBJECT
private:
    QString imagePath;
public:
    explicit ImageCover(QWidget *parent = 0);
    QString getImagePath() const;
    void setImagePath(QString);
    QSize sizeHint() const;
signals:

public slots:
protected:
    virtual void dragEnterEvent(QDragEnterEvent*);
    virtual void dropEvent(QDropEvent*);
};

#endif // IMAGECOVER_H
