#ifndef FORMSETTINGS_H
#define FORMSETTINGS_H

#include <QWidget>
#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class FormSettings;
}

class FormSettings : public QDialog
{
    Q_OBJECT

public:
    explicit FormSettings(QWidget *parent = 0);
    ~FormSettings();

private slots:
    void on_BtnBox_accepted();
    void on_BtnBox_resetDefaults();
    void on_BtnBox_clicked(QAbstractButton *button);
private:
    Ui::FormSettings *ui;
};

#endif // FORMSETTINGS_H
