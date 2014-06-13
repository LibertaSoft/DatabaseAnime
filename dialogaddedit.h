#ifndef DIALOGADDEDIT_H
#define DIALOGADDEDIT_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>

namespace Ui {
class DialogAddEdit;
}

class DialogAddEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEdit(QWidget *parent = 0);
    ~DialogAddEdit();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);

    void on_BtnBox_reset();
    void on_BtnBox_accepted();

    void on_BtnBox_rejected();

private:
    Ui::DialogAddEdit *ui;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;
};

#endif // DIALOGADDEDIT_H
