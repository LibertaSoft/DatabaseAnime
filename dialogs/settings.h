#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "definespath.h"
#include "dbalocalization.h"
#include "mngrconnection.h"
#include "mngrquerys.h"

#include "share/xml/xmldbareader.h"
#include "share/xml/xmldbawriter.h"

#include <QLocale>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
private:
    Ui::Settings *ui;
    bool restoreDefault;
    MngrConnection MngrConnect;

    bool deleteRecords();
    quint64 import();
    quint64 removeFilesFromFolder(QString folder);
    quint64 copyFolder(QString folder1, QString folder2);
public:
    explicit Settings(MngrConnection &MngrCon, QWidget *parent = 0);
    ~Settings();

    Sort::sort getSort();
    Tables::UniformField::field getDisplaydField();
    bool getSwitchToDir();
    bool getRestoreDefault();
    QLocale::Language getLanguage();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void BtnBox_resetDefaults();
    void on_PBtn_Action_Export_clicked();
    void on_TBtn_Export_Path_Choose_clicked();
    void on_TBtn_Import_Path_Choose_clicked();
    void on_PBtn_Import_Append_clicked();
    void on_PBtn_Import_Replace_clicked();
    void on_TBtn_WorkDir_Choose_clicked();
    void on_actionShowImportProgressBar_triggered(bool checked);
    void on_actionShowExportProgressBar_triggered(bool checked);
};

#endif // SETTINGS_H
