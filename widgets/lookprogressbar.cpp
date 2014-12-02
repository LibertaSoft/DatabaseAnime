#include "lookprogressbar.h"

#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QtSql>
#include <QStylePainter>

#include <QPushButton>

//void LookProgressBar::wheelEvent(QWheelEvent *pe)
//{
//    if( pe->delta() > 0 ){
//        progressInc();
//    }else{
//        progressDec();
//    }
//}

void LookProgressBar::initCreate()
{
    _btnAdd = new QToolButton(this);
    _btnSub = new QToolButton(this);
    _lookProgressBar = new QProgressBar(this);
    _hLay = new QHBoxLayout(this);

    this->setLayout(_hLay);
    _hLay->addWidget(_btnSub);
    _hLay->addWidget(_btnAdd);
    _hLay->addWidget(_lookProgressBar);
    _hLay->setMargin(0);
    _hLay->setSpacing(3);

    _btnSub->setIcon( QIcon("://images/list-remove.png") );
    _btnAdd->setIcon( QIcon("://images/list-add.png") );
    _lookProgressBar->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
}

void LookProgressBar::initConnect()
{
    if( !_btnAdd || !_btnSub || !_lookProgressBar || !_hLay ){
        qCritical() << "initConnect: widgets is not created!";
        return;
    }
    connect(this, SIGNAL(progressChanged(int)), _lookProgressBar, SLOT(setValue(int)) );
    connect(_btnAdd, &QToolButton::clicked, [=](){
        int value = getValue() + 1;

        if( value == getMaximum() ){
            emit progressOverflow();
            if( revertWhenOverflow )
                value = getMinimum();
        }

        emit progressChanged( value );
        emit progressChanged( value, getTargetField() );
    });
    connect(_btnSub, &QToolButton::clicked, [=](){
        emit progressChanged( getValue() - 1 ); // decrement
        emit progressChanged( getValue(), getTargetField() );
    });
}

LookProgressBar::LookProgressBar(QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    revertWhenOverflow = false;
    _lookProgressBar->setMinimum( 0 );
    _lookProgressBar->setValue( 0 );
    _lookProgressBar->setMaximum( 100 );
    _lookProgressBar->setFormat( "[%v/%m]" );
    _targetFieldDB = QString::null;
}

LookProgressBar::LookProgressBar(int minimum, int value, int maximum, QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    revertWhenOverflow = false;
    _lookProgressBar->setMinimum( minimum );
    _lookProgressBar->setValue( value );
    _lookProgressBar->setMaximum( maximum );
    _lookProgressBar->setFormat( "[%v/%m]" );
    _targetFieldDB = QString::null;
}

LookProgressBar::LookProgressBar(int minimum, int value, int maximum, QString format, QString targetField, QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    revertWhenOverflow = false;
    _lookProgressBar->setMinimum( minimum );
    _lookProgressBar->setValue( value );
    _lookProgressBar->setMaximum( maximum );
    _lookProgressBar->setFormat( format );
    _targetFieldDB = targetField;
}

void LookProgressBar::setValue(int value)
{
    _lookProgressBar->setValue( value );
}

void LookProgressBar::setMaximum(int value)
{
    _lookProgressBar->setMaximum( value );
}

void LookProgressBar::setMinimum(int value)
{
    _lookProgressBar->setMinimum( value );
}

void LookProgressBar::setFormat(QString format)
{
    _lookProgressBar->setFormat( format );
}

void LookProgressBar::setRevertWhenOverflow(bool flag)
{
    revertWhenOverflow = flag;
}

void LookProgressBar::setTargetFieldDB(QString fieldName)
{
    _targetFieldDB = fieldName;
}

int LookProgressBar::getValue() const
{
    return _lookProgressBar->value();
}

int LookProgressBar::getMaximum() const
{
    return _lookProgressBar->maximum();
}

int LookProgressBar::getMinimum() const
{
    return _lookProgressBar->minimum();
}

QString LookProgressBar::getFormat() const
{
    return _lookProgressBar->format();
}

QString LookProgressBar::getTargetField() const
{
    return _targetFieldDB;
}
