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
    QPalette stylePalette;


    static const int INDEX_OF_SYSTEM_STYLE = 0;

    void initColorPickers(QPalette pallete);
    QPalette paletteFromColorPicker();
    bool duplicateStyleName(QString name);
    void setApplicationStyle(QPalette palette);
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    Sort::sort getSort();
    Tables::UniformField::field getDisplaydField();
    bool getSwitchToDir();
    bool getRestoreDefault();
    QLocale::Language getLanguage();

    void connectColorPicker();
private slots:
    void colorPicked(QColor);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_BtnBox_clicked(QAbstractButton *button);
    void on_BtnBox_accepted();
    void BtnBox_resetDefaults();
    void on_TBtn_WorkDir_Choose_clicked();

    void on_ComboBox_CurrentStyle_currentIndexChanged(int index);
    void on_TButton_RemoveStyle_clicked();
    void on_TButton_CopyStyle_clicked();
    void on_PButton_Style_SaveChanges_clicked();
};

#endif // SETTINGS_H
