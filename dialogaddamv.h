#ifndef DIALOGADDAMV_H
#define DIALOGADDAMV_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>

namespace Ui {
class DialogAddAmv;
}

class DialogAddAmv : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddAmv *ui;
    QSqlQueryModel* model;

    bool _isEditRole;
    unsigned int _recordId;
    QString _oldCover;
    QStringListModel _tags;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;

    void initTags();
    void initOptionalFields();
    void setDataInField();

public:
    explicit DialogAddAmv(QWidget *parent, unsigned int id);
    explicit DialogAddAmv(QWidget *parent);
    ~DialogAddAmv();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_toolButton_clicked();
    bool insert_Amv();
    void btnBox_reset();

    void on_LineEdit_Dir_textChanged(const QString &arg1);
    void on_SpinBox_Year_valueChanged(int arg1);
};

#endif // DIALOGADDAMV_H
