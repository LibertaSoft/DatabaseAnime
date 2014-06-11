#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
//#include <iostream>
#include <QDebug>
#include <QtSql>

bool connectDB(){
    const QString dbUser("");
    const QString dbHost("");
    const QString dbPass("");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( QDir::homePath() +"/DatabaseAnime.sqlite");
    db.setUserName( dbUser );
    db.setHostName( dbHost );
    db.setPassword( dbPass );
    if( !db.open() ){
        qDebug() << "Cannot Open DataBase: " << db.lastError().text();
        QMessageBox::information(0,"Cannot Open DataBase", db.lastError().text());
        return false;
    }

    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lbl_AppTitle->setText( QApplication::applicationDisplayName() );
    ui->Lbl_VVersion->setText( QApplication::applicationVersion() );
    //ui->Lbl_VVersion->setText( QApplication::platformName() );

    QSettings settings;
    bool set_enableBtnAnime     = settings.value("enableElem/BtnSwitchSection/Anime",     true).toBool();
    bool set_enableBtnManga     = settings.value("enableElem/BtnSwitchSection/Manga",     true).toBool();
    bool set_enableBtnAMV       = settings.value("enableElem/BtnSwitchSection/AMV",      false).toBool();
    bool set_enableBtnDorama    = settings.value("enableElem/BtnSwitchSection/Dorama",   false).toBool();
    bool set_enableBtnEditable  = settings.value("enableElem/BtnSwitchSection/Editable",  true).toBool();
    bool set_enableBtnLookLater = settings.value("enableElem/BtnSwitchSection/LookLater", true).toBool();

    if( set_enableBtnAnime ){
        btnAnime = new QPushButton( tr("Аниме"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnAnime);
    }
    if( set_enableBtnManga ){
        btnManga = new QPushButton( tr("Манга"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnManga);
    }
    if( set_enableBtnAMV ){
        btnAMV = new QPushButton( tr("AMV"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnAMV);
    }
    if( set_enableBtnDorama ){
        btnDorama = new QPushButton( tr("Дорама"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnDorama);
    }
    if( set_enableBtnEditable ){
        btnEditable = new QPushButton( tr("Ещё редактируется"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnEditable);
    }
    if( set_enableBtnLookLater ){
        btnLookLater = new QPushButton( tr("Хочу посмотреть"), this );
        ui->VLay_SectionSwapBtns->addWidget(btnLookLater);
    }

    move(settings.value("MainWindow/Position", QPoint( 100, 100)).toPoint());
    resize(settings.value("MainWindow/Size", QSize(868, 586)).toSize());

    connectDB();
}

void MainWindow::closeEvent(QCloseEvent* ){
    QSettings settings;
    settings.setValue("MainWindow/Position", this->geometry().topLeft() );
    settings.setValue("MainWindow/Size", this->geometry().size() );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PButton_Options_clicked()
{
    FormSettings formSettings(this);
    formSettings.setModal(true);
    formSettings.exec();
}

void MainWindow::on_PBtn_Create_clicked()
{
    QMessageBox::information(0,"Click","Create DB Begin");
    qDebug() << "test";
    QSqlQuery query;
    /* */
    QString str = "CREATE TABLE animeSerials( "
                  "id INTEGER PRIMARY KEY NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "Title          VARCHAR(15), "
                  "OrigTitle      VARCHAR(20), "
                  "Director       VARCHAR(20), "
                  "PostScoring    VARCHAR(20), "
                  "SeriesTV       INTEGER, "
                  "SeriesOVA      INTEGER, "
                  "SeriesONA      INTEGER, "
                  "SeriesSpecial  INTEGER, "
                  "SeriesFilm     INTEGER, "
                  "vSeriesTV      INTEGER, "
                  "vSeriesOVA     INTEGER, "
                  "vSeriesONA     INTEGER, "
                  "vSeriesSpecial INTEGER, "
                  "vSeriesFilm    INTEGER, "
                  "Year           INTEGER, "
                  "Season         INTEGER, "
                  "Studios        INTEGER, "
                  "Tags           VARCHAR(256), "
                  "Description    TEXT, "
                  "URL            VARCHAR(20), "
                  "Dir            VARCHAR(20), "
                  "ImageParh      VARCHAR(20) "
                  ");";
    if( !query.exec(str) ){
        qDebug() << "Create DB is not created! Error: " << query.lastError();
    }
}
