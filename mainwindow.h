#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lookprogressbar.h"

#include <formsettings.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *);
    ~MainWindow();

private slots:
    void on_PButton_Options_clicked();
    void on_PBtn_Create_clicked();
    void on_TButton_Add_clicked();
    void on_TButton_Edit_clicked();
    void on_TButton_Delete_clicked();
    void on_listView_ListItemsSection_activated(const QModelIndex &index);
    void saveLookValueChanges(int, QString);

    void on_PBtnAnime_toggled(bool);
    void on_PBtnManga_toggled(bool);
    void on_PBtnAMV_toggled(bool);
    void on_PBtnDorama_toggled(bool);
    void on_PBtnIsLook_toggled(bool);
    void on_PBtnIsEditing_toggled(bool);

private:
    Ui::MainWindow *ui;

    QPushButton* btnAnime, *btnManga, *btnAMV, *btnDorama, *btnEditable, *btnLookLater;
    bool b_btnAnime, b_btnManga, b_btnAMV, b_btnDorama, b_btnEditable, b_btnLookLater;
    LookProgressBar *pbTV, *pbOVA, *pbONA, *pbSpecial, *pbFilm;
    bool b_pbTV, b_pbOVA, b_pbONA, b_pbSpecial, b_pbFilm;

    QSqlQueryModel* QueryModel_ListItemsSection;
};

#endif // MAINWINDOW_H
