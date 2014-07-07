#include "stackwgt_mousehideover.h"

StackWgt_MouseHideOver::StackWgt_MouseHideOver(QWidget *parent) :
    QStackedWidget(parent), opt_switch(true)
{}

void StackWgt_MouseHideOver::setOptSwitch(bool v)
{
    opt_switch = v;
}

void StackWgt_MouseHideOver::leaveEvent(QEvent *)
{
    if( opt_switch )
        setCurrentIndex(0);
}

void StackWgt_MouseHideOver::enterEvent(QEvent *)
{
    if( opt_switch )
        setCurrentIndex(1);
}
