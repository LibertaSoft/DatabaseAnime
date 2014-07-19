#ifndef DIALOGADDAMV_H
#define DIALOGADDAMV_H

#include <QDialog>

namespace Ui {
class DialogAddAmv;
}

class DialogAddAmv : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddAmv(QWidget *parent = 0);
    ~DialogAddAmv();

private:
    Ui::DialogAddAmv *ui;
};

#endif // DIALOGADDAMV_H
