#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lookprogressbar.h"
#include "mngrconnection.h"
#include "mngrquerys.h"

#include <formsettings.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    MngrConnection mngrConnection;

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
    void on_listView_ListWidget_Dir_activated(const QModelIndex &index);
    void saveLookValueChanges(int, QString);

    void on_lineEdit_Search_textChanged(const QString &arg1);
    void on_listView_ListItemsSection_clicked(const QModelIndex &index);
    void on_CB_Section_currentIndexChanged(int index);
    void on_CB_Filter_currentIndexChanged(int index);
};

#endif // MAINWINDOW_H
