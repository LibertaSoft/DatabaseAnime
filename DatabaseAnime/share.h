#ifndef SHARE_H
#define SHARE_H

#include "ui_share.h"

#include "mngrconnection.h"
#include "mngrquerys.h"

#include "xmldbareader.h"
#include "xmldbawriter.h"

#include <QFileDialog>

class Share : public QDialog
{
    Q_OBJECT
private:
    Ui::Share ui;
    MngrConnection &MngrConnect;

public:
    enum Tabs{Import,Export};
    explicit Share(MngrConnection &MngrCon, QWidget *parent = 0);
    void setTab(Tabs tab);

private slots:
    void on_actionShowExportProgressBar_triggered(bool checked);
    void on_actionShowImportProgressBar_triggered(bool checked);
    void on_PBtn_Import_Replace_clicked();
    void on_PBtn_Import_Append_clicked();
    void on_TButton_ChooseImportFile_clicked();
    void on_TButton_ChooseExportFolder_clicked();
    void on_PBtn_Action_Export_clicked();

private:
    bool deleteRecords();
    quint64 import();
    quint64 removeFilesFromFolder(QString folder);
    quint64 copyFolder(QString folder1, QString folder2);
};

#endif // SHARE_H
