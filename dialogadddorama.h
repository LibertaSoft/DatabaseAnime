#ifndef DIALOGADDDORAMA_H
#define DIALOGADDDORAMA_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>

namespace Ui {
class DialogAddDorama;
}

class DialogAddDorama : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddDorama *ui;
    QSqlQueryModel* model;

    bool _isEditRole;
    unsigned int _recordId;
    QString _oldCover;
    QStringListModel _tags;

    QLineEdit* LineEdit_AltTitle;
    QLineEdit* LineEdit_Director;

    void initTags();
    void initOptionalFields();
    void setDataInField();
    void setTabOrders();

public:
    explicit DialogAddDorama(QWidget *parent, unsigned int id);
    explicit DialogAddDorama(QWidget *parent);
    ~DialogAddDorama();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aTV_valueChanged(int arg1);
    void on_SpinBox_aSpec_valueChanged(int arg1);
    void on_SpinBox_aMovie_valueChanged(int arg1);
    void on_toolButton_clicked();
    bool insert_Dorama();
    void btnBox_reset();

    void on_LineEdit_Dir_textChanged(const QString &arg1);
};

#endif // DIALOGADDDORAMA_H
