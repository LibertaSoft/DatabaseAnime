#ifndef DIALOGADDEDIT_H
#define DIALOGADDEDIT_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlTableModel>

namespace Ui {
class DialogAddEdit;
}

class DialogAddEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddEdit(bool isEditRole, QWidget *parent = 0);
    ~DialogAddEdit();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);

    void on_BtnBox_reset();
    void on_BtnBox_accepted();

    void on_BtnBox_rejected();

    void on_SpinBox_aTV_valueChanged(int arg1);

    void on_SpinBox_aOVA_valueChanged(int arg1);

    void on_SpinBox_aONA_valueChanged(int arg1);

    void on_SpinBox_aSpec_valueChanged(int arg1);

    void on_SpinBox_aFilm_valueChanged(int arg1);

    void on_toolButton_clicked();
    bool insert_AnimeSerials();
private:
    Ui::DialogAddEdit *ui;
    QSqlTableModel* TableModel_Tags;
//    QSqlTableModel*

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;
};

#endif // DIALOGADDEDIT_H
