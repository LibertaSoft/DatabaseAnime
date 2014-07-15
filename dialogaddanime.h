#ifndef DIALOGADDEDIT_H
#define DIALOGADDEDIT_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QStringListModel>

namespace Ui {
class DialogAddEdit;
}

class DialogAddEdit : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddEdit *ui;
    QSqlTableModel* TableModel_Tags;
    QSqlQueryModel* model;
    bool isEditRole;
    int recordId;
    QString oldCover;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;

    QStringListModel tags;

    void initTags();
public:
    explicit DialogAddEdit(bool isEditRole, QModelIndex* index, QWidget *parent);
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
    void on_SpinBox_aMovie_valueChanged(int arg1);
    void on_toolButton_clicked();
    bool insert_AnimeSerials();

    void on_LineEdit_Dir_textChanged(const QString &arg1);
};

#endif // DIALOGADDEDIT_H
