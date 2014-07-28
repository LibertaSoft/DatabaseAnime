#ifndef DialogAddAnime_H
#define DialogAddAnime_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>

namespace Ui {
class DialogAddAnime;
}

class DialogAddAnime : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddAnime *ui;
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
    explicit DialogAddAnime(QWidget *parent, unsigned int id);
    explicit DialogAddAnime(QWidget *parent);
    ~DialogAddAnime();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aTV_valueChanged(int arg1);
    void on_SpinBox_aOVA_valueChanged(int arg1);
    void on_SpinBox_aONA_valueChanged(int arg1);
    void on_SpinBox_aSpec_valueChanged(int arg1);
    void on_SpinBox_aMovie_valueChanged(int arg1);
    void on_toolButton_clicked();
    bool insert_Anime();
    void btnBox_reset();

    void on_LineEdit_Dir_textChanged(const QString &arg1);
};

#endif // DialogAddAnime_H
