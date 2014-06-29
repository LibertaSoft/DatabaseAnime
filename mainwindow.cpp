#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddanime.h"
#include "lookprogressbar.h"

#include <QDesktopServices>
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
    if( !objQdir.mkpath( DBPath ) ){
        qDebug() << "Cannot createed app directory in home path";
        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("It was not succeeded to create a directory for a database.") );
        return false;
    }

    db.setDatabaseName( DBPath +"/DatabaseAnime.sqlite");
    db.setUserName( dbUser );
    db.setHostName( dbHost );
    db.setPassword( dbPass );
    if( !db.open() ){
        qDebug() << "Cannot open database: " << db.lastError().text();
        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("It was not possible it will be connected to a database.") );
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
        qDebug() << "Table animeTags is not created! Error: " << query.lastError();
        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("There was a mistake at table creation in a database.") );
        return false;
    }
    //if( query.exec("CREATE TABLE animeTags") )
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
        QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("There was a mistake at table creation in a database.") );
        return false;
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    btnAnime(NULL), btnManga(NULL), btnAMV(NULL), btnDorama(NULL), btnEditable(NULL), btnLookLater(NULL),
    pbTV(NULL), pbOVA(NULL), pbONA(NULL), pbSpecial(NULL), pbFilm(NULL), ListWidget_Dir(NULL)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->lbl_AppTitle->setText( QApplication::applicationDisplayName() );
    ui->Lbl_VVersion->setText( QApplication::applicationVersion() );

    QSettings settings;
    bool set_enableBtnAnime     = settings.value("enableElem/BtnSwitchSection/Anime",     true).toBool();
    bool set_enableBtnManga     = settings.value("enableElem/BtnSwitchSection/Manga",    false).toBool();
    bool set_enableBtnAMV       = settings.value("enableElem/BtnSwitchSection/AMV",      false).toBool();
    bool set_enableBtnDorama    = settings.value("enableElem/BtnSwitchSection/Dorama",   false).toBool();
    bool set_enableBtnEditable  = settings.value("enableElem/BtnSwitchSection/Editable",  true).toBool();
    bool set_enableBtnLookLater = settings.value("enableElem/BtnSwitchSection/LookLater", true).toBool();
    sections::section set_chechedButton
            = (sections::section)settings.value("btnSwitchSection/selected", sections::none).toInt();

    /* #Bug : Временно? /
    bool set_enableBtnCreate     = settings.value("enableElem/BtnCreate", false).toBool();
    if( set_enableBtnCreate )
        ui->PButton_Create->close();
    // */
    if( set_enableBtnAnime ){
        btnAnime = new QPushButton( tr("Anime"), this );
        btnAnime->setCheckable( true );
        if( set_chechedButton == sections::anime ){
            btnAnime->setChecked( true );
        }
        QObject::connect(btnAnime, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnAnime_toggled(bool)) );
        ui->VLay_SectionSwapBtns->addWidget(btnAnime);
    }
    if( set_enableBtnManga ){
        btnManga = new QPushButton( tr("Manga"), this );
        btnManga->setCheckable( true );
        if( set_chechedButton == sections::manga ){
            btnManga->setChecked( true );
        }
        QObject::connect(btnManga, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnManga_toggled(bool)) );
        ui->VLay_SectionSwapBtns->addWidget(btnManga);
    }
    if( set_enableBtnAMV ){
        btnAMV = new QPushButton( tr("AMV"), this );
        btnAMV->setCheckable( true );
        if( set_chechedButton == sections::amv ){
            btnAMV->setChecked( true );
        }
        QObject::connect(btnAMV, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnAMV_toggled(bool)) );
        ui->VLay_SectionSwapBtns->addWidget(btnAMV);
    }
    if( set_enableBtnDorama ){
        btnDorama = new QPushButton( tr("Dorama"), this );
        btnDorama->setCheckable( true );
        if( set_chechedButton == sections::dorama ){
            btnDorama->setChecked( true );
        }
        QObject::connect(btnDorama, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnDorama_toggled(bool)) );
        ui->VLay_SectionSwapBtns->addWidget(btnDorama);
    }
    if( !set_enableBtnEditable && !set_enableBtnLookLater ){
        delete ui->line;
    }
    if( set_enableBtnEditable ){
        btnEditable = new QPushButton( tr("Editing"), this );
        btnEditable->setCheckable( true );
        QObject::connect(btnEditable, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnIsEditing_toggled(bool)) );
        ui->VLay_sectionOption->addWidget(btnEditable);
    }
    if( set_enableBtnLookLater ){
        btnLookLater = new QPushButton( tr("Want to look"), this );
        btnLookLater->setCheckable( true );
        QObject::connect(btnLookLater, SIGNAL(toggled(bool)) ,this, SLOT(on_PBtnIsLook_toggled(bool)) );
        ui->VLay_sectionOption->addWidget(btnLookLater);
    }



    this->restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );
    this->restoreState( settings.value("MainWindow/State").toByteArray() );

    connectDB();
    createTable_AnimeSerials();
    createTable_AnimeTags();

    ui->lineEdit_Search->setFocus();
    setActiveTable( set_chechedButton );

    QueryModel_ListItemsSection = new QSqlQueryModel;
    QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    ui->listView_ListItemsSection->setModel(QueryModel_ListItemsSection);
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::closeEvent(QCloseEvent* ){
    sections::section select;
    if( btnAnime && btnAnime->isChecked() ){
        select = sections::anime;
    }else if( btnManga && btnManga->isChecked() ){
        select = sections::manga;
    }else if( btnAMV && btnAMV->isChecked() ){
        select = sections::amv;
    }else if( btnDorama && btnDorama->isChecked() ){
        select = sections::dorama;
    }else{
        select = sections::none;
    }
    QSettings settings;
    settings.setValue("MainWindow/Geometry", this->saveGeometry() );
    settings.setValue("MainWindow/State",    this->saveState() );
    settings.setValue("btnSwitchSection/selected", select);
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

void MainWindow::on_TButton_Add_clicked()
{


    switch( getActiveTable() ){
        case sections::anime :{
            QModelIndex null;
            DialogAddEdit dialogAdd(false, &null, this);
            dialogAdd.setModal(true);
            dialogAdd.exec();
            break;
        }
        case sections::manga :
                ;
            break;
        case sections::amv :
                ;
            break;
        case sections::dorama :
                ;
            break;
        case sections::none :
        default:
            return;
    }
    QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_TButton_Edit_clicked()
{
    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        QModelIndex i = ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0);
        DialogAddEdit dialogAdd(true, &i, this);
        dialogAdd.setModal(true);
        dialogAdd.exec();
    }
    QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_TButton_Delete_clicked()
{
    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        QSqlQueryModel model;
        model.setQuery( QString( "SELECT ImagePath FROM %1 WHERE id = %2").arg( getActiveTableName() ).arg( ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt() ) );
        QDir dir;
        dir.remove( model.record(0).value("ImagePath").toString() );
        QSqlQuery query;
        query.prepare( QString("DELETE FROM '%1' WHERE id = :id;").arg( getActiveTableName() ) );
        query.bindValue(":id",
                        ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt());
        if( !query.exec() ){
            QMessageBox::warning(this, "Warning", "It was not succeeded to remove record");
        }else{
            QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}

void MainWindow::on_listView_ListItemsSection_activated(const QModelIndex &index)
{
    ui->stackedWidget->setCurrentIndex(1);
    switch( getActiveTable() ){
        case sections::anime :
            selectAnimeData(index);
            break;
        case sections::manga :
            selectMangaData(index);
            break;
        case sections::amv :
            selectAmvData(index);
            break;
        case sections::dorama :
            selectDoramaData(index);
            break;
        case sections::none :
        default:
            return;
    }
}

void MainWindow::on_listView_ListWidget_Dir_activated(const QModelIndex &index)
{
    QSqlQueryModel m1;
    m1.setQuery(
                QString("SELECT Dir FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt() )
                );
    QDesktopServices::openUrl( QUrl::fromLocalFile( m1.record(0).value("Dir").toString() + "/" + index.data().toString() ) );
}

void MainWindow::saveLookValueChanges(int value, QString type)
{
    QSqlQuery query;
    query.prepare( QString("UPDATE %1 SET %2 = :vNum WHERE id = :id;").arg( getActiveTableName() ).arg(type) );
    query.bindValue(":vNum", value);
    query.bindValue(":id", ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt() );
    if( !query.exec() ){
        qDebug() << QString("Cannot update data in table %1: ").arg( getActiveTableName() ) << query.lastError();
    }
}

void MainWindow::on_PBtnAnime_toggled(bool f)
{
    if(!f){
        ui->stackedWidget->setCurrentIndex(0);
        setActiveTable( sections::none );
        QueryModel_ListItemsSection->setQuery( "" );
    }else{
        if( btnManga )
            btnManga->setChecked( false );
        if( btnAMV )
            btnAMV->setChecked( false );
        if( btnDorama )
            btnDorama->setChecked( false );
        setActiveTable( sections::anime );
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
        ui->listView_ListItemsSection->setModelColumn(1);
    }
}

void MainWindow::on_PBtnManga_toggled(bool f)
{
    if(!f){
        ui->stackedWidget->setCurrentIndex(0);
        setActiveTable( sections::none );
        QueryModel_ListItemsSection->setQuery( "" );
    }else{
        if( btnAnime )
            btnAnime->setChecked( false );
        if( btnAMV )
            btnAMV->setChecked( false );
        if( btnDorama )
            btnDorama->setChecked( false );
        setActiveTable( sections::manga );
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
        ui->listView_ListItemsSection->setModelColumn(1);
    }
}

void MainWindow::on_PBtnAMV_toggled(bool f)
{
    if(!f){
        ui->stackedWidget->setCurrentIndex(0);
        setActiveTable( sections::none );
        QueryModel_ListItemsSection->setQuery( "" );
    }else{
        if( btnManga )
            btnManga->setChecked( false );
        if( btnAnime )
            btnAnime->setChecked( false );
        if( btnDorama )
            btnDorama->setChecked( false );
        setActiveTable( sections::amv );
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
        ui->listView_ListItemsSection->setModelColumn(1);
    }
}

void MainWindow::on_PBtnDorama_toggled(bool f)
{
    if(!f){
        ui->stackedWidget->setCurrentIndex(0);
        setActiveTable( sections::none );
        QueryModel_ListItemsSection->setQuery( "" );
    }else{
        if( btnManga )
            btnManga->setChecked( false );
        if( btnAMV )
            btnAMV->setChecked( false );
        if( btnAnime )
            btnAnime->setChecked( false );
        setActiveTable( sections::dorama );
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
        ui->listView_ListItemsSection->setModelColumn(1);
    }
}

void MainWindow::on_PBtnIsLook_toggled(bool f)
{
    if(f){
        if( btnEditable )
            btnEditable->setChecked( false );
        QueryModel_ListItemsSection->setQuery(
                    QString("SELECT id,Title FROM %1 WHERE isHaveLooked = 0").arg( getActiveTableName() ) );
    }else{
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    }
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_PBtnIsEditing_toggled(bool f)
{
    if(f){
        if( btnLookLater )
            btnLookLater->setChecked( false );
        QueryModel_ListItemsSection->setQuery(
                    QString("SELECT id,Title FROM %1 WHERE isEditingDone = 0").arg( getActiveTableName() ) );
    }else{
        QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    }
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_lineEdit_Search_textChanged(const QString &arg1)
{
    if( btnLookLater )
        btnLookLater->setChecked( false );
    if( btnEditable )
        btnEditable->setChecked( false );
    // #ToDo : Реализовать поиск по секциям (Хочу посмотреть / Ещё редактируется)
    QueryModel_ListItemsSection->setQuery(
                QString("SELECT id,Title FROM '%1' WHERE Title LIKE '%2'").arg( getActiveTableName() ).arg("%"+arg1+"%") );
    ui->listView_ListItemsSection->setModelColumn(1);
}

QString MainWindow::getActiveTableName() const
{
    return getTableName( _activeTable );
}

QString MainWindow::getTableName(sections::section table) const
{
    switch( table ){
        case sections::anime :
            return "animeSerials";
            break;
        case sections::manga :
            return "mangaPosters" ;
            break;
        case sections::amv :
            return "amvVideos" ;
            break;
        case sections::dorama :
            return "doramaSerials" ;
            break;
        case sections::none :
        default:
            return "" ;
    }
}

sections::section MainWindow::getActiveTable()
{
    return _activeTable;
}

void MainWindow::setActiveTable(sections::section table)
{
    _activeTable = table;
}

void MainWindow::selectAnimeData(const QModelIndex&)
{
    QSqlQueryModel m1;
//    index.data().toString();
//    QMessageBox::information(this, "select", index.child(1,1).data().toString() );
    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt() )
                );
    /*
    "Season, PostScoring"
    */

    if( pbTV ){
        delete pbTV;
        pbTV = NULL;
    }
    if( pbOVA ){
        delete pbOVA;
        pbOVA = NULL;
    }
    if( pbONA ){
        delete pbONA;
        pbONA = NULL;
    }
    if( pbSpecial ){
        delete pbSpecial;
        pbSpecial = NULL;
    }
    if( pbFilm ){
        delete pbFilm;
        pbFilm = NULL;
    }

    if( m1.record(0).value("SeriesTV").toInt() > 0 ){
        pbTV = new LookProgressBar(this);
        pbTV->setTargetFieldDB("vSeriesTV");
        pbTV->setValue( m1.record(0).value("vSeriesTV").toInt() );
        pbTV->setMaximum( m1.record(0).value("SeriesTV").toInt() );
        pbTV->setFormat("TV [%v/%m]");
        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
    }
    if( m1.record(0).value("SeriesOVA").toInt() > 0 ){
        pbOVA = new LookProgressBar(this);
        pbOVA->setTargetFieldDB("vSeriesOVA");
        pbOVA->setValue( m1.record(0).value("vSeriesOVA").toInt() );
        pbOVA->setMaximum( m1.record(0).value("SeriesOVA").toInt() );
        pbOVA->setFormat("OVA [%v/%m]");
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
    }
    if( m1.record(0).value("SeriesONA").toInt() > 0 ){
        pbONA = new LookProgressBar(this);
        pbONA->setTargetFieldDB("vSeriesONA");
        pbONA->setValue( m1.record(0).value("vSeriesONA").toInt() );
        pbONA->setMaximum( m1.record(0).value("SeriesONA").toInt() );
        pbONA->setFormat("ONA [%v/%m]");
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
    }
    if( m1.record(0).value("SeriesSpecial").toInt() > 0 ){
        pbSpecial = new LookProgressBar(this);
        pbSpecial->setTargetFieldDB("vSeriesSpecial");
        pbSpecial->setValue( m1.record(0).value("vSeriesSpecial").toInt() );
        pbSpecial->setMaximum( m1.record(0).value("SeriesSpecial").toInt() );
        pbSpecial->setFormat("Special [%v/%m]");
        ui->HLay_WBRow2->addWidget( pbSpecial );
        QObject::connect(pbSpecial, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
    }
    if( m1.record(0).value("SeriesFilm").toInt() > 0 ){
        pbFilm = new LookProgressBar(this);
        pbFilm->setTargetFieldDB("vSeriesFilm");
        pbFilm->setValue( m1.record(0).value("vSeriesFilm").toInt() );
        pbFilm->setMaximum( m1.record(0).value("SeriesFilm").toInt() );
        pbFilm->setFormat("Film [%v/%m]");
        ui->HLay_WBRow2->addWidget( pbFilm );
        QObject::connect(pbFilm, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
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

    // #Bug, нужна модель директории
    if( ListWidget_Dir ){
        delete ListWidget_Dir;
        ListWidget_Dir = NULL;
    }
    if( !m1.record(0).value("Dir").toString().isEmpty() ){
         ListWidget_Dir = new QListWidget(this);

        QDir dir( m1.record(0).value("Dir").toString() );
        ListWidget_Dir->setSortingEnabled( true );
        ListWidget_Dir->setIconSize( QSize(32,32) );
        ListWidget_Dir->setViewMode( QListView::IconMode );
//        ListWidget_Dir->setViewMode( QListView::ListMode );
        ListWidget_Dir->setMinimumHeight(80);
        ListWidget_Dir->setWordWrap( true );
        ListWidget_Dir->setWrapping( true );
        QStringList filters;
        filters << "*.avi" << "*.mkv" << "*.mp4" << "*.wmv";
        filters << "*.m2ts" << "*.rm";
        ListWidget_Dir->addItems( dir.entryList( filters, QDir::Files ) );
        ui->HLay_FolderVideo->addWidget( ListWidget_Dir );
        for(int i = 0; i < ListWidget_Dir->count() ; ++i){
            ListWidget_Dir->item(i)->setIcon( QIcon( "://images/video.png" ) );
            ListWidget_Dir->item(i)->setSizeHint( QSize( ListWidget_Dir->height()+ListWidget_Dir->height()/2, ListWidget_Dir->height() ) );
        }
        QObject::connect( ListWidget_Dir, SIGNAL(activated(QModelIndex)), this, SLOT(on_listView_ListWidget_Dir_activated(QModelIndex)) );
    }
}

void MainWindow::selectMangaData(const QModelIndex&)
{
    return;
}

void MainWindow::selectAmvData(const QModelIndex&)
{
    return;
}

void MainWindow::selectDoramaData(const QModelIndex&)
{
    return;
}

void MainWindow::on_PButton_Create_clicked()
{
    ui->PButton_Create->setDisabled( true );
    QSettings settings;
    settings.setValue("enableElem/BtnCreate", true);
//  #ToDo : Создать и заполнить таблицы animeTags, animeStudios, etc...

}
