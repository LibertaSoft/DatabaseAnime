#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lookprogressbar.h"

#include <formsettings.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    QPushButton* btnAnime, *btnManga, *btnAMV, *btnDorama, *btnEditable, *btnLookLater;
    LookProgressBar *pbTV, *pbOVA, *pbONA, *pbSpecial, *pbFilm;
    QListWidget* ListWidget_Dir;

    sections::section _activeTable;
    QSqlQueryModel* QueryModel_ListItemsSection;

    QString getActiveTableName() const;
    QString getTableName( sections::section ) const;
    sections::section getActiveTable();
    void setActiveTable( sections::section );

    void selectAnimeData(const QModelIndex&);
    void selectMangaData(const QModelIndex&);
    void selectAmvData(const QModelIndex&);
    void selectDoramaData(const QModelIndex&);

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *);
    ~MainWindow();

private slots:
    void on_PButton_Options_clicked();
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

    void on_lineEdit_Search_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
