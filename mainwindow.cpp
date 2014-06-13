#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QtSql>

bool connectDB(){
    const QString dbUser("");
    const QString dbHost("");
    const QString dbPass("");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QDir objQdir;
    QString DBPath( QDir::homePath() + "/."+QApplication::organizationName()+"/"+QApplication::applicationName() );
    bool isOk = objQdir.mkpath( DBPath );
    if( !isOk ){
        qDebug() << "Cannot createed app directory in home path";
        QMessageBox::warning(0, QObject::tr("Внимание"), QObject::tr("Не удалось создать директорию для базы данных.") );
        return false;
    }

    db.setDatabaseName( DBPath +"/DatabaseAnime.sqlite");
    db.setUserName( dbUser );
    db.setHostName( dbHost );
    db.setPassword( dbPass );
    if( !db.open() ){
        qDebug() << "Cannot open database: " << db.lastError().text();
        QMessageBox::warning(0, QObject::tr("Внимание"), QObject::tr("Не удалось подключится к БД.") );
        return false;
    }
    return true;
}

bool createTable_AnimeSerials(){
    QString str = "CREATE TABLE IF NOT EXISTS animeSerials( "
                  "id INTEGER PRIMARY KEY NOT NULL,"
                  "isHaveLooked   INTEGER, "
                  "isEditingDone  INTEGER, "
                  "Title          VARCHAR(128), "
                  "OrigTitle      VARCHAR(128), "
                  "Director       VARCHAR(32), "
                  "PostScoring    VARCHAR(128), "
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
                  "URL            VARCHAR(256), "
                  "Dir            VARCHAR(256), "
                  "ImagePath      VARCHAR(256) "
                  ");";
    QSqlQuery query;
    if( !query.exec(str) ){
        qDebug() << "Table AnimeSerials is not created! Error: " << query.lastError();
        QMessageBox::warning(0, QObject::tr("Внимание"), QObject::tr("Извините. Произошла ошибка при создании таблицы в БД.") );
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

    this->restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );
    this->restoreState( settings.value("MainWindow/State").toByteArray() );

    connectDB();
    createTable_AnimeSerials();
}

void MainWindow::closeEvent(QCloseEvent* ){
    QSettings settings;
    settings.setValue("MainWindow/Geometry", this->saveGeometry() );
    settings.setValue("MainWindow/State",    this->saveState() );
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

}
