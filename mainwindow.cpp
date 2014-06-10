#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->lbl_AppTitle->setText( QApplication::applicationName() );

    QSettings settings;

    // Если настроек нету - создаём их
    if( !settings.contains("configExist") ){
        settings.setValue("configExist", true);
        settings.setValue("enableElem/BtnSwitchSection/Anime", true);
        settings.setValue("enableElem/BtnSwitchSection/Manga", true);
        settings.setValue("enableElem/BtnSwitchSection/AMV", false);
        settings.setValue("enableElem/BtnSwitchSection/Dorea", false);
        settings.setValue("enableElem/BtnSwitchSection/Editable", true);
        settings.setValue("enableElem/BtnSwitchSection/LookLater", true);
    }

    bool set_enableBtnAnime            = settings.value("enableElem/BtnSwitchSection/Anime", true).toBool();
    bool set_enableBtnManga            = settings.value("enableElem/BtnSwitchSection/Manga", true).toBool();
    bool set_enableBtnAMV                = settings.value("enableElem/BtnSwitchSection/AMV", false).toBool();
    bool set_enableBtnDorea             = settings.value("enableElem/BtnSwitchSection/Dorea", false).toBool();
    bool set_enableBtnEditable         = settings.value("enableElem/BtnSwitchSection/Editable", true).toBool();
    bool set_enableBtnLookLater      = settings.value("enableElem/BtnSwitchSection/LookLater", true).toBool();

    if( set_enableBtnAnime )
        btnAnime             = new QPushButton( trUtf8("Аниме") );
    if( set_enableBtnManga )
        btnManga            = new QPushButton( trUtf8("Манга") );
    if( set_enableBtnAMV )
        btnAMV                = new QPushButton( trUtf8("AMV") );
    if( set_enableBtnDorea )
        btnDorea             = new QPushButton( trUtf8("Дореа") );
    if( set_enableBtnEditable )
        btnEditable         = new QPushButton( trUtf8("Ещё редактируется") );
    if( set_enableBtnLookLater )
        btnLookLater = new QPushButton( trUtf8("Хочу посмотреть") );

    if( set_enableBtnAnime )
        ui->VLay_SectionSwapBtns->addWidget(btnAnime);
    if( set_enableBtnManga )
        ui->VLay_SectionSwapBtns->addWidget(btnManga);
    if( set_enableBtnAMV )
        ui->VLay_SectionSwapBtns->addWidget(btnAMV);
    if( set_enableBtnDorea )
        ui->VLay_SectionSwapBtns->addWidget(btnDorea);
    if( set_enableBtnEditable )
        ui->VLay_SectionSwapBtns->addWidget(btnEditable);
    if( set_enableBtnLookLater )
        ui->VLay_SectionSwapBtns->addWidget(btnLookLater);

}
/*
void MainWindow::closeEvent(QCloseEvent* ){
    std::cout << "Application Terminate!" << std::endl;
}
*/
MainWindow::~MainWindow()
{
    delete ui;
    delete btnAnime;
    delete btnManga;
    delete btnAMV;
    delete btnDorea;
    delete btnEditable;
    delete btnLookLater;
}

void MainWindow::on_PButton_Options_clicked()
{
    FormSettings fs(this);
    fs.setModal(true);
    fs.exec();
}
