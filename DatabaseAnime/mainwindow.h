#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/lookprogressbar.h"
#include "dialogs/settings.h"
#include "mngrconnection.h"
#include "mngrquerys.h"

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTranslator qtTr;
    QTranslator dbaTr;

    Ui::MainWindow *ui;
    MngrConnection mngrConnection;

    LookProgressBar *pbTV, *pbOVA, *pbONA, *pbSpecial, *pbMovie;

    QListWidget* ListWidget_Dir;
    QPushButton* _btnPlay;
    QScrollArea* _ScrArea_propertyes;
    bool _restoreDefSettings;

    sections::section _activeTable;
    Tables::UniformField::field _displayedField;
    Sort::sort _sort;

    QModelIndex currentItem;
    unsigned long long _currentItemId;
    QString _currentItemDir;

    QSqlQueryModel* QueryModel_ListItemsSection;

    QString getActiveTableName() const;
    QString getTableName( sections::section ) const;
    sections::section getActiveTable();
    void reloadSectionsList();
    void reloadFiltersList();
    void loadAnimeFilters();
    void loadMangaFilters();
    void loadAmvFilters();
    void loadDoramaFilters();
    void setActiveTable( sections::section );

    void selectAnimeData();
    void selectMangaData();
    void selectAmvData();
    void selectDoramaData();

    void execOptionsDialog();
    void deleteLookProgressBars();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);
    ~MainWindow();

private slots:
    void on_PButton_Options_clicked();
    void on_TButton_Add_clicked();
    void on_TButton_Edit_clicked();
    void on_TButton_Delete_clicked();
    void on_TreeView_List_activated(const QModelIndex &index);
    void saveLookValueChanges(int, QString);
//    void saveLookValueChanges(int, int, QString, QString);
    void openFileClicked();
    void openFile(QString&);
    void on_lineEdit_Search_textChanged(const QString &arg1);
    void on_TreeView_List_clicked(const QModelIndex &index);
    void on_CB_Section_currentIndexChanged(int index);
    void on_CB_Filter_currentIndexChanged(int index);
    void on_TreeView_Dir_activated(const QModelIndex &index);
    void on_PBtn_url_vk_clicked();
    void on_PBtn_url_github_clicked();
    void replyVersionVerificationFinished(QNetworkReply*);
};

#endif // MAINWINDOW_H
