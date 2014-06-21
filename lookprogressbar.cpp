#include "lookprogressbar.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QtSql>

LookProgressBar::LookProgressBar(QWidget *parent) :
    QFrame(parent), _value(0), _maxValue(0), _minValue(0), _btnAddActive(false), _btnSubActive(false)
{
    _format = "[%1/%2]";
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->setMouseTracking( true );
}

QSize LookProgressBar::sizeHint() const
{
    return QSize(200, 30);
}

void LookProgressBar::setValue(int n)
{
    _value = n;
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
        emit progressChanged(_value, _type);
    }
}

void LookProgressBar::progressDec()
{
    if( _value > _minValue ){
        _value--;
        repaint();
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
    p.fillRect(height()*2, 0, (width() - height()*2 )*f, height(), gradient);

    // Кнопки
    QRect rbtnPls(1,1,height()-1,height()-2);
    QRect rbtnSub(height()+2,1,height()-1,height()-2);

    p.fillRect( rbtnPls, QColor(200,200,200,255) );
    p.fillRect( rbtnSub, QColor(200,200,200,255) );

    QPixmap pUp;
    if( _btnAddActive ){
        pUp.load("://images/list-add-active.png");
    }else{
        pUp.load("://images/list-add.png");
    }
    QPixmap pSub;
    if( _btnSubActive ){
        pSub.load("://images/list-remove-active.png");
    }else{
        pSub.load("://images/list-remove.png");
    }


    QRect xrbtnPls(4,4,height()-8,height()-8);
    QRect xrbtnSub(height()+4,4,height()-8,height()-8);

    p.drawPixmap(xrbtnPls, pUp);
    p.drawPixmap(xrbtnSub, pSub);

    // Текст
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

void LookProgressBar::leaveEvent(QEvent*)
{
    _btnSubActive = false;
    _btnAddActive = false;
    repaint();
}

void LookProgressBar::mouseMoveEvent(QMouseEvent *pe)
{
    pe->pos();
    if( pe->pos().x() >= 1 && pe->pos().x() <= height()-1){
        _btnAddActive = true;
        repaint();
    }else{
        _btnAddActive = false;
        repaint();
    }
    if( pe->pos().x() >= height()+3 && pe->pos().x() <= height()*2-3){
        _btnSubActive = true;
        repaint();
    }else{
        _btnSubActive = false;
        repaint();
    }
}
