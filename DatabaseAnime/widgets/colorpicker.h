#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QFrame>

class ColorPicker : public QFrame
{
    Q_OBJECT
private:
    QColor _color;
public:
    explicit ColorPicker(QWidget* parent = nullptr);
    QColor color();
    QSize sizeHint() const;
protected:
    virtual void paintEvent(QPaintEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

signals:
    void colorChanged( QColor );

public slots:
    void setColor( QColor color );
};

#endif // COLORPICKER_H
