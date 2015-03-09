#include "colorpicker.h"

#include <QColorDialog>
#include <QPainter>

ColorPicker::ColorPicker(QWidget *parent)
: QFrame(parent)
{
    this->setFrameShape( Shape::WinPanel );
    this->setFrameStyle( QFrame::WinPanel );
}

/*! ~russian
 * \brief Метод для получения текущего установленного цвета в ColorPicker'е
 * \return Текущий цвет ColorPicker'а
 */
QColor ColorPicker::color()
{
    return _color;
}

QSize ColorPicker::sizeHint() const
{
    return QSize(24,24);
}

void ColorPicker::setColor(QColor color)
{
    _color = color;
    this->repaint();
}

void ColorPicker::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.setBrush( QBrush(_color) );
    QRect shape = rect();
    shape.adjust(0,0,-1,-1);

    p.drawRect( shape );

    p.end();
}

void ColorPicker::mouseReleaseEvent(QMouseEvent*)
{
    QColor newColor = QColorDialog::getColor( _color, this );

    if( newColor.isValid() ){
        setColor( newColor );
        emit colorChanged( newColor );
    }
}
