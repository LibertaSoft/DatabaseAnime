#ifndef DIALOGADDDORAMA_H
#define DIALOGADDDORAMA_H

#include <QDialog>

namespace Ui {
class DialogAddDorama;
}

class DialogAddDorama : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddDorama(QWidget *parent = 0);
    ~DialogAddDorama();

private:
    Ui::DialogAddDorama *ui;
};

#endif // DIALOGADDDORAMA_H
