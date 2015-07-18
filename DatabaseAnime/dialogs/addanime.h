#ifndef DialogAddAnime_H
#define DialogAddAnime_H

#include <QDialog>
#include <QAbstractButton>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QNetworkAccessManager>
#include <QCompleter>
#include "shikimoriapi.h"

namespace Ui {
class DialogAddAnime;
}

enum Kind{TV, OVA, ONA, Special, Movie};

class DialogAddAnime : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogAddAnime *ui;
    QSqlQueryModel* model;
    ShikimoriApi api;


    bool _isEditRole;
    unsigned long long _recordId;
    QString _oldCover;
    QStringListModel _tags;
    int _animeId;
    QString _altTitle;

    QLineEdit* LineEdit_OrigTitle;
    QLineEdit* LineEdit_Director;
    QLineEdit* LineEdit_PostScoring;

    QStringListModel _titleCompliterModel;
    QCompleter *TitleCompliter;
    QStringListModel _completerModel;

    bool _autoSearchOnShikimori;

    void initTitleCompleter();
    void initTags();
    void initOptionalFields();
    void setDataInField();
    void setTabOrders();
    int  kindOf(const QString &kind);

public:
    explicit DialogAddAnime(QWidget *parent, unsigned long long id);
    explicit DialogAddAnime(QWidget *parent);
    ~DialogAddAnime();

private slots:
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void on_BtnBox_rejected();
    void on_SpinBox_aTV_valueChanged(int value);
    void on_SpinBox_aOVA_valueChanged(int value);
    void on_SpinBox_aONA_valueChanged(int value);
    void on_SpinBox_aSpec_valueChanged(int value);
    void on_SpinBox_aMovie_valueChanged(int value);
    void on_toolButton_clicked();
    bool insert_Anime();
    void btnBox_reset();
    void on_LineEdit_Dir_textChanged(const QString &value);
    void on_SpinBox_Year_valueChanged(int value);
    void on_TBtn_Search_clicked();

    void replyDownloadPictureFinished(QNetworkReply*);
    void on_LineEdit_Title_textEdited(const QString&title);

    void setRecivedData(QMap<QString,QVariant>);
};

#endif // DialogAddAnime_H
