#include "lookprogressbar.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QtSql>

LookProgressBar::LookProgressBar(QWidget *parent) :
    QFrame(parent), _value(0), _maxValue(0), _minValue(0)
{
//    setLineWidth(0);
//    setFrameStyle( Box | Sunken );
    _format = "[%1/%2]";
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

QSize LookProgressBar::sizeHint() const
{
    return QSize(200, 30);
}

void LookProgressBar::setValue(int n)
{
    _value = n;
//    emit progressChanged(_value);
    emit progressChanged(_value, _type);
}

void LookProgressBar::setMaximum(int n)
{
    _maxValue = n;
}

void LookProgressBar::setMinimum(int n)
{
    _minValue = n;
}

void LookProgressBar::setFormat(QString f)
{
    _format = f.replace("%v", "%1").replace("%m", "%2");
}

void LookProgressBar::setType(QString str)
{
    _type = str;
}

void LookProgressBar::setProgress(int n)
{
    if( n >= _minValue && n <= _maxValue ){
        _value = n;
        repaint();
    }
}

void LookProgressBar::progressInc()
{
    if( _value < _maxValue ){
        _value++;
        repaint();
//        emit progressChanged(_value);
        emit progressChanged(_value, _type);
    }
}

void LookProgressBar::progressDec()
{
    if( _value > _minValue ){
        _value--;
        repaint();
//        emit progressChanged(_value);
        emit progressChanged(_value, _type);
    }
}

void LookProgressBar::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setStart( width(), height()/2 );

    float f = (float)_value / _maxValue;

    gradient.setColorAt(0.0f, QColor(30,170,235,255) );
    gradient.setColorAt(0.5f, QColor(10,130,255,255) );
    gradient.setColorAt(1.0f, QColor(30,170,235,255) );

    p.fillRect(rect(), QColor(184,184,184,255) );
    p.fillRect(0, 0, width()*f, height(), gradient);

    QRect rbtnPls(0,0,height(),height());
    QRect rbtnSub(height(),0,height(),height());

    p.fillRect( rbtnPls, QColor(200,255,200,175));
    p.fillRect(rbtnSub, QColor(255,200,200,175));
    p.drawText(rbtnPls, Qt::AlignCenter, "+");
    p.drawText(rbtnSub, Qt::AlignCenter, "-");


    p.setPen(QPen(Qt::black));
    QString str = _format.arg(_value).arg(_maxValue);
    p.drawText(rect(), Qt::AlignCenter, str);

    drawFrame(&p);
}

void LookProgressBar::mousePressEvent(QMouseEvent *pe)
{
    pe->pos();
    if( pe->pos().x() >= 0 && pe->pos().x() <= height()){
        emit progressInc();
    }
    if( pe->pos().x() >= height() && pe->pos().x() <= height()*2){
        emit progressDec();
    }
}
