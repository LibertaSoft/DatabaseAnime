#include "stackwgt_mousehideover.h"

StackWgt_MouseHideOver::StackWgt_MouseHideOver(QWidget *parent) :
    QStackedWidget(parent), opt_switch(true), disabledSwitch(false)
{}

void StackWgt_MouseHideOver::setOptSwitch(bool v)
{
    opt_switch = v;
}

void StackWgt_MouseHideOver::setDisabledSwitch(bool v)
{
    disabledSwitch = v;
}

void StackWgt_MouseHideOver::leaveEvent(QEvent *)
{
    if( disabledSwitch && opt_switch )
        setCurrentIndex(0);
}

void StackWgt_MouseHideOver::enterEvent(QEvent *)
{
    if( disabledSwitch && opt_switch )
        setCurrentIndex(1);
}
