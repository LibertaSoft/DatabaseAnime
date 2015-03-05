#include "stackwgt_mousehideover.h"

#include <QtSql>

StackWgt_MouseHideOver::StackWgt_MouseHideOver(QWidget *parent) :
    QStackedWidget(parent), opt_switch(true), disabledSwitch(false), hover(false)
{}

void StackWgt_MouseHideOver::setOptSwitch(bool v)
{
    opt_switch = v;
}

void StackWgt_MouseHideOver::setDisabledSwitch(bool v)
{
    disabledSwitch = v;
}

void StackWgt_MouseHideOver::showDir()
{
    if( hover && disabledSwitch && opt_switch )
        setCurrentIndex(1);
}

void StackWgt_MouseHideOver::showCover()
{
    if( !hover && disabledSwitch && opt_switch )
        setCurrentIndex(0);
}

void StackWgt_MouseHideOver::leaveEvent(QEvent *e)
{
    hover = false;
    showCover();
    e->accept();
}

void StackWgt_MouseHideOver::enterEvent(QEvent *e)
{
    hover = true;
    QTimer::singleShot(300, this, SLOT( showDir() ) );
    e->accept();
}
