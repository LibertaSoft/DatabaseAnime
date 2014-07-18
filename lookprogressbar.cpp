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
    emit progressChanged( getValue(), getMaximum(), getTargetField() );
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

void LookProgressBar::setTargetFieldDB(QString str)
{
    _targetFieldDB = str;
}

int LookProgressBar::getValue() const
{
    return _value;
}

int LookProgressBar::getMaximum() const
{
    return _maxValue;
}

int LookProgressBar::getMinimum() const
{
    return _minValue;
}

bool LookProgressBar::isActiveBtnAdd() const
{
    return _btnAddActive;
}

bool LookProgressBar::isActiveBtnSub() const
{
    return _btnSubActive;
}

bool LookProgressBar::setActiveBtnAdd(bool f)
{
    if( _btnAddActive != f ){
        _btnAddActive = f;
        return true;
    }else
        return false;
}

bool LookProgressBar::setActiveBtnSub(bool f)
{
    if( _btnSubActive != f ){
        _btnSubActive = f;
        return true;
    }else
        return false;
}

QString LookProgressBar::getFormat() const
{
    return _format;
}

QString LookProgressBar::getTargetField() const
{
    return _targetFieldDB;
}

void LookProgressBar::setProgress(int n)
{
    if( n >= getMinimum() && n <= getMaximum() ){
        setValue( n );
        repaint();
    }
}

void LookProgressBar::progressInc()
{
    if( getValue() < getMaximum() ){
        _value++;
        repaint();
        emit progressChanged( getValue(), getMaximum(), getTargetField() );
    }
}

void LookProgressBar::progressDec()
{
    if( getValue() > getMinimum() ){
        _value--;
        repaint();
        emit progressChanged( getValue(), getMaximum(), getTargetField() );
    }
}

void LookProgressBar::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setStart( width(), height()/2 );

    float f = static_cast<float>( getValue() / getMaximum() );

    gradient.setColorAt(0.0f, QColor(30,170,235,255) );
    gradient.setColorAt(0.4f, QColor(10,130,255,255) );
    gradient.setColorAt(1.0f, QColor(30,170,235,255) );

    p.fillRect(rect(), QColor(184,184,184,255) );
    p.fillRect(height()*2, 0, (width() - height()*2 )*f, height(), gradient);

    // Кнопки
    QRect rbtnPls(1,1,height()-1,height()-2);
    QRect rbtnSub(height()+2,1,height()-1,height()-2);

    p.fillRect( rbtnPls, QColor(200,200,200,255) );
    p.fillRect( rbtnSub, QColor(200,200,200,255) );

    QPixmap pUp;
    if( isActiveBtnAdd() ){
        pUp.load("://images/list-add-active.png");
    }else{
        pUp.load("://images/list-add.png");
    }
    QPixmap pSub;
    if( isActiveBtnSub() ){
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
    QString str = getFormat().arg( getValue() ).arg( getMaximum() );
    p.drawText(rect(), Qt::AlignCenter, str);

    drawFrame(&p);
}

void LookProgressBar::mousePressEvent(QMouseEvent*)
{
    if( isActiveBtnAdd() ){
        emit progressInc();
    }else if( isActiveBtnSub() ){
        emit progressDec();
    }
}

void LookProgressBar::leaveEvent(QEvent*)
{
    if( setActiveBtnAdd( false ) || setActiveBtnSub( false ) )
        repaint();
}

void LookProgressBar::mouseMoveEvent(QMouseEvent *pe)
{
    const int height2 = height()*2;
    if( pe->pos().x() < height2 ){
        if( pe->pos().x() < height2>>1 ){
            if( setActiveBtnAdd( true ) || setActiveBtnSub( false ) )
                repaint();
        }else{
            if( setActiveBtnAdd( false ) || setActiveBtnSub( true ) )
                repaint();
        }
    }else{
        if( setActiveBtnSub( false ) )
            repaint();
    }
}
