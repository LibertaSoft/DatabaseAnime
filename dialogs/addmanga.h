#ifndef DIALOGADDMANGA_H
#define DIALOGADDMANGA_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QNetworkAccessManager>
#include <QCompleter>
#include "shikimoriapi.h"

namespace Ui {
class DialogAddManga;
}

class DialogAddManga : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddManga *ui;
    QSqlQueryModel* model;
    shikimoriApi api;

    bool _isEditRole;
    unsigned long long _recordId;
    QString _oldCover;


    QLineEdit* LineEdit_AltTitle;
    QLineEdit* LineEdit_Author;
    QLineEdit* LineEdit_Translation;

    QStringListModel _titleCompliterModel;
    QCompleter *TitleCompliter;
    QStringListModel _completerModel;

    bool _autoSearchOnShikimori;

    QStringListModel _tags;

    void initTitleCompleter();
    void initTags();
    void initOptionalFields();
    void setDataInFields();
    void createOptionalFields();
    bool insert_Manga();
    void setTabOrders();

public:
    explicit DialogAddManga(QWidget *parent, unsigned long long record_id);
    explicit DialogAddManga(QWidget *parent);
    ~DialogAddManga();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aVol_valueChanged(int);
    void on_SpinBox_aCh_valueChanged(int);
    void on_SpinBox_aPages_valueChanged(int);
    void on_LineEdit_Dir_textChanged(const QString&);
    void btnBox_reset();
    void on_TBtn_ChooseDir_clicked();
    void on_SpinBox_Year_valueChanged(int);

    void replyDownloadPictureFinished(QNetworkReply*);
    void on_LineEdit_Title_textEdited(const QString &title);
    void on_TBtn_Search_clicked();
    void setRecivedData(QMap<QString,QVariant> data);
};

#endif // DIALOGADDMANGA_H
