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
    bool hover;
public:
    explicit StackWgt_MouseHideOver(QWidget *parent = nullptr);
    void setOptSwitch(bool);
    void setDisabledSwitch(bool);
signals:

public slots:
    void showDir();
    void showCover();

protected:
    virtual void leaveEvent(QEvent*);
    virtual void enterEvent(QEvent*);
};

#endif // STACKWGT_MOUSEHIDEOVER_H
