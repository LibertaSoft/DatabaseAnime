#ifndef DIALOGADDMANGA_H
#define DIALOGADDMANGA_H

#include <QDialog>

namespace Ui {
class DialogAddManga;
}

class DialogAddManga : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddManga(QWidget *parent = 0);
    ~DialogAddManga();

private:
    Ui::DialogAddManga *ui;
};

#endif // DIALOGADDMANGA_H
