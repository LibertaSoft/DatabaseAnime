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
//    QSqlTableModel* TableModel_Tags;
    QSqlQueryModel* model;
    bool isEditRole;
    int recordId;
    QString oldCover;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;

    QStringListModel tags;

    void initTags();
    void on_BtnBox_reset();
    void on_toolButton_clicked();
public:
    explicit DialogAddManga(bool isEditRole, QModelIndex* index, QWidget *parent = 0);
    ~DialogAddManga();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aVol_valueChanged(int);
    void on_SpinBox_aCh_valueChanged(int);
    void on_SpinBox_aPages_valueChanged(int);
    bool insert_MangaPosters();
    void on_LineEdit_Dir_textChanged(const QString &arg1);
};

#endif // DIALOGADDMANGA_H
