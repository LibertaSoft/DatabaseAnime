#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "definespath.h"
#include "dbalocalization.h"
#include "mngrconnection.h"
#include "mngrquerys.h"

#include "xmldbareader.h"
#include "xmldbawriter.h"

#include <QSettings>
#include <QLocale>
#include <QDir>
#include <QFileDialog>
//#include <QVariant>

//#include <QMessageBox>
//#include <QDebug>

//#include <QProgressDialog>
//#include <QAbstractButton>


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
public:
    explicit Settings(MngrConnection &MngrCon, QWidget *parent = 0);
    ~Settings();

    Sort::sort getSort();
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
    unsigned long long on_actionImport_triggered();
    bool on_actionDeleteRecords_triggered();
};

#endif // SETTINGS_H
