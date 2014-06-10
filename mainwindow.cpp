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
    bool set_enableBtnAnime     = settings.value("enableElem/BtnSwitchSection/Anime",     true).toBool();
    bool set_enableBtnManga     = settings.value("enableElem/BtnSwitchSection/Manga",     true).toBool();
    bool set_enableBtnAMV       = settings.value("enableElem/BtnSwitchSection/AMV",      false).toBool();
    bool set_enableBtnDorea     = settings.value("enableElem/BtnSwitchSection/Dorea",    false).toBool();
    bool set_enableBtnEditable  = settings.value("enableElem/BtnSwitchSection/Editable",  true).toBool();
    bool set_enableBtnLookLater = settings.value("enableElem/BtnSwitchSection/LookLater", true).toBool();

    if( set_enableBtnAnime ){
        btnAnime = new QPushButton( tr("Аниме") );
        ui->VLay_SectionSwapBtns->addWidget(btnAnime);
    }
    if( set_enableBtnManga ){
        btnManga = new QPushButton( tr("Манга") );
        ui->VLay_SectionSwapBtns->addWidget(btnManga);
    }
    if( set_enableBtnAMV ){
        btnAMV = new QPushButton( tr("AMV") );
        ui->VLay_SectionSwapBtns->addWidget(btnAMV);
    }
    if( set_enableBtnDorea ){
        btnDorea = new QPushButton( tr("Дореа") );
        ui->VLay_SectionSwapBtns->addWidget(btnDorea);
    }
    if( set_enableBtnEditable ){
        btnEditable = new QPushButton( tr("Ещё редактируется") );
        ui->VLay_SectionSwapBtns->addWidget(btnEditable);
    }
    if( set_enableBtnLookLater ){
        btnLookLater = new QPushButton( tr("Хочу посмотреть") );
        ui->VLay_SectionSwapBtns->addWidget(btnLookLater);
    }

    move(settings.value("MainWindow/Position", QPoint(100, 100)).toPoint());
    resize(settings.value("MainWindow/Size", QSize(868, 586)).toSize());
}

void MainWindow::closeEvent(QCloseEvent* ){
    QSettings settings;

    settings.setValue("MainWindow/Position", this->geometry().topLeft() );
    settings.setValue("MainWindow/Size", this->geometry().size() );
}

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
    FormSettings formSettings(this);
    formSettings.setModal(true);
    formSettings.exec();
}
