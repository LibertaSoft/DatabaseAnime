#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddedit.h"
#include "lookprogressbar.h"

#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>
#include <QDir>
#include <QtSql>
//#include <QProgressBar>

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

bool insertDefaultTags_AnimeTags()
{
    QSqlQuery query;
    /*
    query.prepare("TRUNCATE TABLE AnimeTags");
    if( !query.exec() ){
        qDebug() << "Cannot truncate table AnimeTags: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    // */
    query.prepare("INSERT INTO AnimeTags(tagName) VALUES "
                  "(:v01), (:v02), (:v03), (:v04), (:v05), "
                  "(:v06), (:v07), (:v08), (:v09), (:v10), "
                  "(:v11), (:v12), (:v13), (:v14), (:v15), "
                  "(:v16), (:v17), (:v18), (:v19), (:v20), "
                  "(:v21), (:v22), (:v23), (:v24), (:v25), "
                  "(:v26), (:v27), (:v28), (:v29), (:v30), "
                  "(:v31), (:v32), (:v33), (:v34), (:v35), "
                  "(:v36), (:v37), (:v38), (:v39), (:v40), "
                  "(:v41), (:v42), (:v43), (:v44)"
                  );
    query.bindValue(":v01", "Сёнен");
    query.bindValue(":v02", "Сёнен Ай");
    query.bindValue(":v03", "Сейнен");
    query.bindValue(":v04", "Сёдзе");
    query.bindValue(":v05", "Сёдзе Ай");
    query.bindValue(":v06", "Дзёсей");
    query.bindValue(":v07", "Комедия");
    query.bindValue(":v08", "Романтика");
    query.bindValue(":v09", "Школа");
    query.bindValue(":v10", "Безумие");
    query.bindValue(":v11", "Боевые исскуства");
    query.bindValue(":v12", "Вампиры");
    query.bindValue(":v13", "Военное");
    query.bindValue(":v14", "Гарем");
    query.bindValue(":v15", "Демоны");
    query.bindValue(":v16", "Детское");
    query.bindValue(":v17", "Драма");
    query.bindValue(":v18", "Игры");
    query.bindValue(":v19", "Исторический");
    query.bindValue(":v20", "Космос");
    query.bindValue(":v21", "Магия");
    query.bindValue(":v22", "Машины");
    query.bindValue(":v23", "Меха");
    query.bindValue(":v24", "Мистика");
    query.bindValue(":v25", "Музыка");
    query.bindValue(":v26", "Пародия");
    query.bindValue(":v27", "Повседневность");
    query.bindValue(":v28", "Полиция");
    query.bindValue(":v29", "Приключения");
    query.bindValue(":v30", "Психологическое");
    query.bindValue(":v31", "Самураи");
    query.bindValue(":v32", "Сверхъестественное");
    query.bindValue(":v33", "Смена пола");
    query.bindValue(":v34", "Спорт");
    query.bindValue(":v35", "Супер сила");
    query.bindValue(":v36", "Ужасы");
    query.bindValue(":v37", "Фантастика");
    query.bindValue(":v38", "Фэнтези");
    query.bindValue(":v39", "Экшен");
    query.bindValue(":v40", "Этти");
    query.bindValue(":v41", "Триллер");
    query.bindValue(":v42", "Хентай");
    query.bindValue(":v43", "Яой");
    query.bindValue(":v44", "Юри");

    if( !query.exec() ){
        qDebug() << "Cannot insert data in table AnimeTags: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}

bool createTable_AnimeTags()
{
    QString sql = "CREATE TABLE IF NOT EXISTS animeTags("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                  "tagName VARCHAR(32) "
                  ");";
    QSqlQuery query;
    if( !query.exec(sql) ){
        qDebug() << "Table AnimeTags is not created! Error: " << query.lastError();
//        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        QMessageBox::warning(0, QObject::tr("Внимание"), QObject::tr("Произошла ошибка при создании таблицы в БД.") );
        return false;
    }
    return true;
}

bool createTable_AnimeSerials()
{
    QString sql = "CREATE TABLE IF NOT EXISTS animeSerials( "
                  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
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
    if( !query.exec(sql) ){
        qDebug() << "Table animeSerials is not created! Error: " << query.lastError();
        QMessageBox::warning(0, QObject::tr("Внимание"), QObject::tr("Произошла ошибка при создании таблицы в БД.") );
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
    createTable_AnimeTags();

    b_pbTV = b_pbOVA = b_pbONA = b_pbSpecial = b_pbFilm = false;

    QueryModel_ListItemsSection = new QSqlQueryModel;
    QueryModel_ListItemsSection->setQuery( "SELECT Title FROM animeSerials" );
    ui->listView_ListItemsSection->setModel(QueryModel_ListItemsSection);
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
{insertDefaultTags_AnimeTags();}

void MainWindow::on_TButton_Add_clicked()
{
    DialogAddEdit dialogAdd(false, this);
    dialogAdd.setModal(true);
    dialogAdd.exec();
    QueryModel_ListItemsSection->setQuery( "SELECT Title FROM animeSerials" );
}

void MainWindow::on_TButton_Edit_clicked()
{
    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        DialogAddEdit dialogAdd(true, this);
        dialogAdd.setModal(true);
        dialogAdd.exec();
    }
}

void MainWindow::on_TButton_Delete_clicked()
{

    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        QMessageBox::information(this,"Удаление","Типа удалено");
//        QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
//        for(int i = 0; i < mlist.count(); i+=2){
//            QueryModel_ListItemsSection->setQuery( QString("DELETE FROM animeSerials WHERE Title='%1'").arg( mlist.at(i).data(Qt::DisplayRole).toString() ) );
//        }
    }else{
        QMessageBox::information(this,"Удаление","И чё теперь делать будем?");
    }
}

void MainWindow::on_listView_ListItemsSection_activated(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(1);
    QSqlQueryModel m1;
    m1.setQuery( QString("SELECT * FROM animeSerials WHERE Title='%1'").arg( index.data().toString() ) );
//    ui->ScrollArea_AnimeProperty->
//    ui->scrollAreaWidgetContents_FLay->wid
      /*
    "isHaveLooked, isEditingDone, ,"
    ", , PostScoring,"
    ", SeriesOVA, SeriesONA, SeriesSpecial, SeriesFilm,"
    ", vSeriesOVA, vSeriesONA, vSeriesSpecial, vSeriesFilm,"
    ", Season, ,"
    ", ,"
    ", Dir, " b_pbTV, b_pbOVA, b_pbONA, b_pbSpecial, b_pbFilm;
    */

    if( b_pbTV ){
        delete pbTV;
        b_pbTV = false;
    }
    if( b_pbOVA ){
        delete pbOVA;
        b_pbOVA = false;
    }
    if( b_pbONA ){
        delete pbONA;
        b_pbONA = false;
    }
    if( b_pbSpecial ){
        delete pbSpecial;
        b_pbSpecial = false;
    }
    if( b_pbFilm ){
        delete pbFilm;
        b_pbFilm = false;
    }

    if( m1.record(0).value("SeriesTV").toInt() > 0 ){
        b_pbTV = true;
        pbTV = new LookProgressBar(this);
        pbTV->setValue( m1.record(0).value("vSeriesTV").toInt() );
        pbTV->setMaximum( m1.record(0).value("SeriesTV").toInt() );
        pbTV->setFormat("TV [%v/%m]");
        ui->VLay_WatchedSeriesBars->addWidget( pbTV );
    }
    if( m1.record(0).value("SeriesOVA").toInt() > 0 ){
        b_pbOVA = true;
        pbOVA = new LookProgressBar(this);
        pbOVA->setValue( m1.record(0).value("vSeriesOVA").toInt() );
        pbOVA->setMaximum( m1.record(0).value("SeriesOVA").toInt() );
        pbOVA->setFormat("OVA [%v/%m]");
        ui->VLay_WatchedSeriesBars->addWidget( pbOVA );
    }
    if( m1.record(0).value("SeriesONA").toInt() > 0 ){
        b_pbONA = true;
        pbONA = new LookProgressBar(this);
        pbONA->setValue( m1.record(0).value("vSeriesONA").toInt() );
        pbONA->setMaximum( m1.record(0).value("SeriesONA").toInt() );
        pbONA->setFormat("ONA [%v/%m]");
        ui->VLay_WatchedSeriesBars->addWidget( pbONA );
    }
    if( m1.record(0).value("SeriesSpecial").toInt() > 0 ){
        b_pbSpecial = true;
        pbSpecial = new LookProgressBar(this);
        pbSpecial->setValue( m1.record(0).value("vSeriesSpecial").toInt() );
        pbSpecial->setMaximum( m1.record(0).value("SeriesSpecial").toInt() );
        pbSpecial->setFormat("Special [%v/%m]");
        ui->VLay_WatchedSeriesBars->addWidget( pbSpecial );
    }
    if( m1.record(0).value("SeriesFilm").toInt() > 0 ){
        b_pbFilm = true;
        pbFilm = new LookProgressBar(this);
        pbFilm->setValue( m1.record(0).value("vSeriesFilm").toInt() );
        pbFilm->setMaximum( m1.record(0).value("SeriesFilm").toInt() );
        pbFilm->setFormat("Film [%v/%m]");
        ui->VLay_WatchedSeriesBars->addWidget( pbFilm );
    }

    ui->Lbl_svTitle->setText( m1.record(0).value("Title").toString() );
    ui->Lbl_svOrigTitle->setText( m1.record(0).value("OrigTitle").toString() );
    ui->Lbl_svDirector->setText( m1.record(0).value("Director").toString() );
    ui->Lbl_svYear->setText( m1.record(0).value("Year").toString() );
    ui->Lbl_svTags->setText( m1.record(0).value("Tags").toString() );
    ui->Lbl_svStudio->setText( m1.record(0).value("Studios").toString() );
    ui->Lbl_svURL->setText( m1.record(0).value("URL").toString() );
    ui->Lbl_VAnimeDescr->setText( m1.record(0).value("Description").toString() );
    QPixmap pic( m1.record(0).value("ImagePath").toString() );
    ui->Lbl_ImageCover->setPixmap( pic );
}
