#ifndef STACKWGT_MOUSEHIDEOVER_H
#define STACKWGT_MOUSEHIDEOVER_H

#include <QStackedWidget>
#include <QEvent>

class StackWgt_MouseHideOver : public QStackedWidget
{
    Q_OBJECT
private:
    bool opt_switch;
    bool disabledSwitch;
public:
    explicit StackWgt_MouseHideOver(QWidget *parent = 0);
    void setOptSwitch(bool);
    void setDisabledSwitch(bool);
signals:

public slots:

protected:
    virtual void leaveEvent(QEvent*);
    virtual void enterEvent(QEvent*);
};

#endif // STACKWGT_MOUSEHIDEOVER_H
