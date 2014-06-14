#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    FormSettings *formSettings;

    QPushButton* btnAnime;
    QPushButton* btnManga;
    QPushButton* btnAMV;
    QPushButton* btnDorama;
    QPushButton* btnEditable;
    QPushButton* btnLookLater;
};

#endif // MAINWINDOW_H
