#ifndef DIALOGADDMANGA_H
#define DIALOGADDMANGA_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QStringListModel>

namespace Ui {
class DialogAddManga;
}

class DialogAddManga : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddManga *ui;
    QSqlQueryModel* model;
    bool _isEditRole;
    unsigned int _recordId;
    QString _oldCover;

    QLineEdit* LineEdit_AltTitle;
    QLineEdit* LineEdit_Author;
    QLineEdit* LineEdit_Translation;

    QStringListModel _tags;

    void initTags();
    void initOptionalFields();
    void setDataInFields();
    void createOptionalFields();
    bool insert_Manga();
public:
    explicit DialogAddManga(QWidget *parent, unsigned int record_id);
    explicit DialogAddManga(QWidget *parent);
    ~DialogAddManga();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aVol_valueChanged(int);
    void on_SpinBox_aCh_valueChanged(int);
    void on_SpinBox_aPages_valueChanged(int);
    void on_LineEdit_Dir_textChanged(const QString &arg1);
    void on_BtnBox_reset();
    void on_TBtn_ChooseDir_clicked();
};

#endif // DIALOGADDMANGA_H
