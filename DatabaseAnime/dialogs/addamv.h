#ifndef DIALOGADDAMV_H
#define DIALOGADDAMV_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>

#include "models/amvmodel.h"

namespace Ui {
class DialogAddAmv;
}

/*! \~russian
 * \brief Класс диалога добавления AMV
 *
 * Диалог добавления и/или редактирования AMV записей.
 * \todo Делает слишком много. В планах, разбиение задач
 * чтобы у диалога осталась только задача редактирования.
 */
class DialogAddAmv : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddAmv *ui;
    AmvModel *_model;

    QSqlQueryModel* model;

    bool _isEditRole;
    unsigned long long _recordId;
    QString _oldCover;
    QStringListModel _tags;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;

    void initTags();
    void initOptionalFields();
    void setDataInField();

public:
    explicit DialogAddAmv(QWidget *parent, unsigned long long id);
    explicit DialogAddAmv(QWidget *parent);
    ~DialogAddAmv();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_toolButton_clicked();
    bool insert_Amv();
    void btnBox_reset();

    void on_LineEdit_Dir_textChanged(const QString &value);
    void on_SpinBox_Year_valueChanged(int value);
};

#endif // DIALOGADDAMV_H
