#include "lookprogressbar.h"

#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QtSql>
#include <QStylePainter>

#include <QPushButton>

LookProgressBar::LookProgressBar(QWidget *parent) :
    QFrame(parent), _value(0), _maxValue(0), _minValue(0), _btnAddActive(false), _btnSubActive(false)
{
    _format = "[%1/%2]";
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->setMouseTracking( true );
}

QSize LookProgressBar::sizeHint() const
{
    return QSize(200, 32);
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

void LookProgressBar::setTargetOverflowFieldDB(QString str)
{
    _targetOverflowFieldDB = str;
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

QString LookProgressBar::getTargetOverflowFieldDB() const
{
    return _targetOverflowFieldDB;
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
    QStylePainter sp(this);

    QStyleOptionButton subBtnOpt;
    subBtnOpt.icon = QIcon("://images/list-remove.png");
    subBtnOpt.iconSize = QSize(24,24);
    subBtnOpt.rect = QRect(0,0,32,32);
    subBtnOpt.state = QStyle::State_Enabled;
    if( _btnSubActive )
        subBtnOpt.state |= QStyle::State_MouseOver;

    sp.drawControl( QStyle::CE_PushButton, subBtnOpt);

    QStyleOptionButton addBtnOpt;
    addBtnOpt.icon = QIcon("://images/list-add.png");
    addBtnOpt.iconSize = QSize(24,24);
    addBtnOpt.rect = QRect(32,0,32,32);
    addBtnOpt.state = QStyle::State_Enabled;
    if( _btnAddActive )
        addBtnOpt.state |= QStyle::State_MouseOver;

    sp.drawControl( QStyle::CE_PushButton, addBtnOpt);

    QStyleOptionProgressBar opt;
    opt.maximum = getMaximum();
    opt.minimum = getMinimum();
    opt.progress = getValue();
    opt.text = getFormat().arg( getValue() ).arg( getMaximum() );
    opt.textVisible = true;
    opt.textAlignment = Qt::AlignCenter | Qt::AlignVCenter;
    QRect r = rect();
    r.setX(64);
    opt.rect = r;

    sp.drawControl( QStyle::CE_ProgressBar, opt);
}

void LookProgressBar::mousePressEvent(QMouseEvent*)
{
    if( isActiveBtnAdd() ){
        progressInc();
    }else if( isActiveBtnSub() ){
        progressDec();
    }
}

void LookProgressBar::leaveEvent(QEvent*)
{
    if( setActiveBtnAdd( false ) || setActiveBtnSub( false ) )
        repaint();
}

void LookProgressBar::mouseMoveEvent(QMouseEvent *pe)
{
    /// such approach:
    /// "if (setActiveBtnAdd(false) || setActiveBtnSub(true))"
    /// It is applied to optimization since it isn't required to redraw the button if it it isn't required

    const int height2 = height()*2;
    if( pe->pos().x() < height2 ){
        // Mouse is to the on buttons
        if( pe->pos().x() < height2>>1 ){
            // Mouse on left button
            if( setActiveBtnAdd( false ) || setActiveBtnSub( true ) )
                repaint();
        }else{
            // Mouse on right button
            if( setActiveBtnAdd( true ) || setActiveBtnSub( false ) )
                repaint();
        }
    }else{
        // Mouse is to the right of buttons
        if( setActiveBtnAdd( false ) )
            repaint();
    }
}

void LookProgressBar::wheelEvent(QWheelEvent *pe)
{
    if( pe->delta() > 0 ){
        progressInc();
    }else{
        progressDec();
    }
}


void LookProgressBar2::initCreate()
{
    _btnAdd = new QToolButton(this);
    _btnSub = new QToolButton(this);
    _lookProgressBar = new QProgressBar(this);
    _hLay = new QHBoxLayout(this);

    this->setLayout(_hLay);
    _hLay->addWidget(_btnSub);
    _hLay->addWidget(_btnAdd);
    _hLay->addWidget(_lookProgressBar);

    _btnSub->setIcon( QIcon("://images/list-remove.png") );
    _btnAdd->setIcon( QIcon("://images/list-add.png") );
    _lookProgressBar->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
}

void LookProgressBar2::initConnect()
{
    if( !_btnAdd || !_btnSub || !_lookProgressBar || !_hLay ){
        qCritical() << "initConnect: widgets is not created!";
        return;
    }
    connect(this, SIGNAL(progressChanged(int)), _lookProgressBar, SLOT(setValue(int)) );
    connect(_btnAdd, &QToolButton::clicked, [=](){
        if( getValue() == getMaximum() ){
            emit progressOverflow();
            if( revertWhenOverflow )
                emit progressChanged( getMinimum() );
        }else{
            emit progressChanged( getValue() + 1 );
            emit progressChanged( getValue(), getTargetField() );
        }
    });
    connect(_btnSub, &QToolButton::clicked, [=](){
        emit progressChanged( getValue() - 1 );
        emit progressChanged( getValue(), getTargetField() );
    });
}

LookProgressBar2::LookProgressBar2(QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    _lookProgressBar->setMinimum( 0 );
    _lookProgressBar->setValue( 0 );
    _lookProgressBar->setMaximum( 100 );
    _lookProgressBar->setFormat( "[%v/%m]" );
    _targetFieldDB = QString::null;
}

LookProgressBar2::LookProgressBar2(int minimum, int value, int maximum, QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    _lookProgressBar->setMinimum( minimum );
    _lookProgressBar->setValue( value );
    _lookProgressBar->setMaximum( maximum );
    _lookProgressBar->setFormat( "[%v/%m]" );
    _targetFieldDB = QString::null;
}

LookProgressBar2::LookProgressBar2(int minimum, int value, int maximum, QString format, QString targetField, QWidget *parent)
    :QWidget(parent)
{
    initCreate();
    initConnect();

    _lookProgressBar->setMinimum( minimum );
    _lookProgressBar->setValue( value );
    _lookProgressBar->setMaximum( maximum );
    _lookProgressBar->setFormat( format );
    _targetFieldDB = targetField;
}

void LookProgressBar2::setValue(int value)
{
    _lookProgressBar->setValue( value );
}

void LookProgressBar2::setMaximum(int value)
{
    _lookProgressBar->setMaximum( value );
}

void LookProgressBar2::setMinimum(int value)
{
    _lookProgressBar->setMinimum( value );
}

void LookProgressBar2::setFormat(QString format)
{
    _lookProgressBar->setFormat( format );
}

void LookProgressBar2::setTargetFieldDB(QString fieldName)
{
    _targetFieldDB = fieldName;
}

void LookProgressBar2::setTargetOverflowFieldDB(QString fieldName)
{
    _targetOverflowFieldDB = fieldName;
}

int LookProgressBar2::getValue() const
{
    return _lookProgressBar->value();
}

int LookProgressBar2::getMaximum() const
{
    return _lookProgressBar->maximum();
}

int LookProgressBar2::getMinimum() const
{
    return _lookProgressBar->minimum();
}

QString LookProgressBar2::getFormat() const
{
    return _lookProgressBar->format();
}

QString LookProgressBar2::getTargetField() const
{
    return _targetFieldDB;
}

QString LookProgressBar2::getTargetOverflowFieldDB() const
{
    return _targetOverflowFieldDB;
}
