#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddanime.h"
#include "lookprogressbar.h"
#include "dialogaddmanga.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QtSql>
#include <QAbstractItemModel>
#include <QDirModel>
//#include <QSvgWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pbTV(NULL), pbOVA(NULL), pbONA(NULL), pbSpecial(NULL), pbMovie(NULL), ListWidget_Dir(NULL)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->StackWgt_CoverOrDir->setCurrentIndex(0);
    ui->lbl_AppTitle->setText( QApplication::applicationDisplayName() );
    ui->Lbl_VVersion->setText( QApplication::applicationVersion() );

//    ui->Lbl_logo->setVisible( false );
//    QSvgWidget *logo = new QSvgWidget("/tmp/DBA_logo.svg");
//    logo->setFixedSize(600,500);
//    ui->VLay_logoSvg->addWidget( logo );
//    ui->VLay_logoSvg->setAlignment(logo, Qt::AlignCenter);


    QSettings settings;

    this->restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );
    this->restoreState( settings.value("MainWindow/State").toByteArray() );
    bool c1 = settings.value( "SwitchToDirOnHoverACover", true ).toBool();
    ui->StackWgt_CoverOrDir->setOptSwitch( c1 );

    mngrConnection.open();
    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();

    ui->lineEdit_Search->setFocus();
    QueryModel_ListItemsSection = new QSqlQueryModel(this);
    ui->listView_ListItemsSection->setModel(QueryModel_ListItemsSection);
    ui->listView_ListItemsSection->setModelColumn(1);

    reloadSectionsList();
    reloadFiltersList();
}

void MainWindow::closeEvent(QCloseEvent *e){
    mngrConnection.close();

    sections::section select = static_cast<sections::section>( ui->CB_Section->currentData().toInt() );
    QSettings settings;
    settings.setValue("MainWindow/Geometry", this->saveGeometry() );
    settings.setValue("MainWindow/State",    this->saveState() );
    settings.setValue("btnSwitchSection/selected", select);

    ui->dockMenu->close();
    e->accept();
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

    reloadSectionsList();
    QSettings settings;
    bool c1 = settings.value( "SwitchToDirOnHoverACover", true ).toBool();
    ui->StackWgt_CoverOrDir->setOptSwitch( c1 );
}

void MainWindow::on_TButton_Add_clicked()
{
    switch( getActiveTable() ){
        case sections::anime :{
            DialogAddAnime dialogAddAnime(this);
            dialogAddAnime.setModal(true);
            dialogAddAnime.exec();
            break;
        }
        case sections::manga :{
            QModelIndex null;
            DialogAddManga dialogAddManga(false, &null, this);
            dialogAddManga.setModal(true);
            dialogAddManga.exec();
            break;
        }
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
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable() );
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_TButton_Edit_clicked()
{
    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        QModelIndex i = ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0);
        switch( getActiveTable() ){
            case sections::anime :{
                DialogAddAnime dialogAddAnime(this, currentItemId);
                dialogAddAnime.setModal(true);
                dialogAddAnime.exec();
                break;
            }
            case sections::manga :{
                QModelIndex null;
                DialogAddManga dialogAddManga(true, &i, this);
                dialogAddManga.setModal(true);
                dialogAddManga.exec();
                break;
            }
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
        Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentIndex() );
        MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), filter );
        ui->listView_ListItemsSection->setModelColumn(1);
    }
}

void MainWindow::on_TButton_Delete_clicked()
{
    if( !ui->listView_ListItemsSection->selectionModel()->selectedIndexes().isEmpty() ){
        QSqlQueryModel model;
        model.setQuery( QString( "SELECT ImagePath FROM %1 WHERE id = %2").arg( getActiveTableName() ).arg( currentItemId ) );
        QDir dir;
        dir.remove( model.record(0).value("ImagePath").toString() );
        QSqlQuery query;
        query.prepare( QString("DELETE FROM '%1' WHERE id = :id;").arg( getActiveTableName() ) );
        query.bindValue(":id",
                        ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt());
        if( !query.exec() ){
            qDebug() << "It was not succeeded to remove record. Error: " << query.lastError();
            QMessageBox::warning(this, "Warning", "It was not succeeded to remove record");
        }else{
            QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}

void MainWindow::on_listView_ListItemsSection_activated(const QModelIndex &index)
{
    currentItemId = ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt();
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

void MainWindow::saveLookValueChanges(int value, int max, QString type)
{
    QSqlQuery query;
    bool looked = false;
    if( value == max && type == "vSeriesTV" ){
        looked = true;
    }
    if( looked ){
        query.prepare( QString("UPDATE %1 SET %2 = :vNum, isHaveLooked = :isLook WHERE id = :id;").arg( getActiveTableName() ).arg(type) );
        query.bindValue(":isLook", true);
    }else{
        query.prepare( QString("UPDATE %1 SET %2 = :vNum WHERE id = :id;").arg( getActiveTableName() ).arg(type) );
    }
    query.bindValue(":vNum", value);
    query.bindValue(":id", currentItemId );
    if( !query.exec() ){
        qDebug() << QString("Cannot update data in table %1: ").arg( getActiveTableName() ) << query.lastError();
    }
}

void MainWindow::on_lineEdit_Search_textChanged(const QString &strSearch)
{
    MngrQuerys::selectSection(
                QueryModel_ListItemsSection, getActiveTable(),
                QString("Title LIKE '%2'").arg("%"+strSearch+"%") );
    ui->listView_ListItemsSection->setModelColumn(1);
}

QString MainWindow::getActiveTableName() const
{
    return getTableName( _activeTable );
}

QString MainWindow::getTableName(sections::section table) const
{
    return MngrQuerys::getTableName( table );
}

sections::section MainWindow::getActiveTable()
{
    return _activeTable;
}

void MainWindow::reloadSectionsList()
{
    QSettings settings;
    bool set_enableBtnAnime  = settings.value("enableSection/Anime",   true).toBool();
    bool set_enableBtnManga  = settings.value("enableSection/Manga",  false).toBool();
    bool set_enableBtnAMV    = settings.value("enableSection/AMV",    false).toBool();
    bool set_enableBtnDorama = settings.value("enableSection/Dorama", false).toBool();

    sections::section set_select
            = static_cast<sections::section>(
                settings.value("btnSwitchSection/selected", sections::none).toInt() );
    ui->CB_Section->clear();
    ui->CB_Section->addItem( QIcon("://images/icon-filters/filter_edit.png"),
                             tr("Main"), sections::none );
    if( set_enableBtnAnime ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/section-anime.png"),
                                 tr("Anime"),  sections::anime );
        if( set_select == sections::anime )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnManga ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/section-manga.png"),
                                 tr("Manga"),  sections::manga );
        if( set_select == sections::manga )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnAMV ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/section-amv.png"),
                                 tr("AMV"),    sections::amv );
        if( set_select == sections::amv )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnDorama ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/section-dorama.png"),
                                 tr("Dorama"), sections::dorama );
        if( set_select == sections::dorama )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
}

void MainWindow::loadAnimeFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_all.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_look.png"),
                            tr("Want to look"), Filter::wanttolook );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_tv.png"),
                            tr("TV"),           Filter::tv );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_ova.png"),
                            tr("OVA"),          Filter::ova );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_ona.png"),
                            tr("ONA"),          Filter::ona );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_special.png"),
                            tr("Special"),      Filter::special );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_movie.png"),
                            tr("Movie"),        Filter::movie );
}
void MainWindow::loadMangaFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_all.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_look.png"),
                            tr("Want to read"), Filter::wanttolook );
}
void MainWindow::loadAmvFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_all.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_edit.png"),
                            tr("Editing"),      Filter::editing );
}
void MainWindow::loadDoramaFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_all.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/filter_look.png"),
                            tr("Want to look"), Filter::wanttolook );
}

void MainWindow::reloadFiltersList()
{
    ui->CB_Filter->clear();
    switch ( getActiveTable() ) {
    case sections::anime :
        loadAnimeFilters();
        break;
    case sections::manga :
        loadMangaFilters();
        break;
    case sections::amv :
        loadAmvFilters();
        break;
    case sections::dorama :
        loadDoramaFilters();
        break;
    case sections::none :
    default:
        break;
    }
}

void MainWindow::setActiveTable(sections::section table)
{
    _activeTable = table;
}

void MainWindow::selectAnimeData(const QModelIndex&)
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( currentItemId )
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
    if( pbMovie ){
        delete pbMovie;
        pbMovie = NULL;
    }

    if( m1.record(0).value("SeriesTV").toInt() > 0 ){
        pbTV = new LookProgressBar(this);
        pbTV->setTargetFieldDB("vSeriesTV");
        pbTV->setValue( m1.record(0).value("vSeriesTV").toInt() );
        pbTV->setMaximum( m1.record(0).value("SeriesTV").toInt() );
        pbTV->setFormat("TV [%v/%m]");
        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("SeriesOVA").toInt() > 0 ){
        pbOVA = new LookProgressBar(this);
        pbOVA->setTargetFieldDB("vSeriesOVA");
        pbOVA->setValue( m1.record(0).value("vSeriesOVA").toInt() );
        pbOVA->setMaximum( m1.record(0).value("SeriesOVA").toInt() );
        pbOVA->setFormat("OVA [%v/%m]");
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("SeriesONA").toInt() > 0 ){
        pbONA = new LookProgressBar(this);
        pbONA->setTargetFieldDB("vSeriesONA");
        pbONA->setValue( m1.record(0).value("vSeriesONA").toInt() );
        pbONA->setMaximum( m1.record(0).value("SeriesONA").toInt() );
        pbONA->setFormat("ONA [%v/%m]");
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("SeriesSpecial").toInt() > 0 ){
        pbSpecial = new LookProgressBar(this);
        pbSpecial->setTargetFieldDB("vSeriesSpecial");
        pbSpecial->setValue( m1.record(0).value("vSeriesSpecial").toInt() );
        pbSpecial->setMaximum( m1.record(0).value("SeriesSpecial").toInt() );
        pbSpecial->setFormat("Special [%v/%m]");
        ui->HLay_WBRow2->addWidget( pbSpecial );
        QObject::connect(pbSpecial, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("SeriesMovie").toInt() > 0 ){
        pbMovie = new LookProgressBar(this);
        pbMovie->setTargetFieldDB("vSeriesMovie");
        pbMovie->setValue( m1.record(0).value("vSeriesMovie").toInt() );
        pbMovie->setMaximum( m1.record(0).value("SeriesMovie").toInt() );
        pbMovie->setFormat("Movie [%v/%m]");
        ui->HLay_WBRow2->addWidget( pbMovie );
        QObject::connect(pbMovie, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }

    if( !m1.record(0).value("URL").toString().isEmpty() ){
        ui->Lbl_svTitle->setText( "<a href='"
                                  + m1.record(0).value("URL").toString()
                                  + "'>"
                                  + m1.record(0).value("Title").toString()
                                  + "</a>");
        ui->Lbl_svTitle->setOpenExternalLinks( true );
    }else{
        ui->Lbl_svTitle->setText( m1.record(0).value("Title").toString() );
    }
    if( m1.record(0).value("OrigTitle").toString().isEmpty() ){
        ui->Lbl_svOrigTitle->setVisible( false );
        ui->Lbl_sOrigTitle->setVisible( false );
    }else{
        ui->Lbl_svOrigTitle->setText( m1.record(0).value("OrigTitle").toString() );
        ui->Lbl_svOrigTitle->setVisible( true );
        ui->Lbl_sOrigTitle->setVisible( true );
    }
    if( m1.record(0).value("Director").toString().isEmpty() ){
        ui->Lbl_svDirector->setVisible( false );
        ui->Lbl_sDirector->setVisible( false );
    }else{
        ui->Lbl_svDirector->setText( m1.record(0).value("Director").toString() );
        ui->Lbl_svDirector->setVisible( true );
        ui->Lbl_sDirector->setVisible( true );
    }
    if( m1.record(0).value("Year").toString().isEmpty() ){
        ui->Lbl_svYear->setVisible( false );
        ui->Lbl_sYear->setVisible( false );
    }else{
        ui->Lbl_svYear->setText( m1.record(0).value("Year").toString() );
        ui->Lbl_svYear->setVisible( true );
        ui->Lbl_sYear->setVisible( true );
    }
    if( m1.record(0).value("Tags").toString().isEmpty() ){
        ui->Lbl_svTags->setVisible( false );
        ui->Lbl_sTags->setVisible( false );
    }else{
        ui->Lbl_svTags->setText( m1.record(0).value("Tags").toString() );
        ui->Lbl_svTags->setVisible( true );
        ui->Lbl_sTags->setVisible( true );
    }
    if( m1.record(0).value("Studios").toString().isEmpty() ){
        ui->Lbl_svStudio->setVisible( false );
        ui->Lbl_sStudio->setVisible( false );
    }else{
        ui->Lbl_svStudio->setText( m1.record(0).value("Studios").toString() );
        ui->Lbl_svStudio->setVisible( true );
        ui->Lbl_sStudio->setVisible( true );
    }
    if( m1.record(0).value("Description").toString().isEmpty() ){
        ui->Lbl_VAnimeDescr->setVisible( false );
        ui->Lbl_sAnimeDescr->setVisible( false );
    }else{
        ui->Lbl_VAnimeDescr->setText( m1.record(0).value("Description").toString() );
        ui->Lbl_VAnimeDescr->setVisible( true );
        ui->Lbl_sAnimeDescr->setVisible( true );
    }

    QString imgPath = m1.record(0).value("ImagePath").toString();

    QPixmap pic( imgPath );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    currentItemDir = m1.record(0).value("Dir").toString();

    if( currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setOptSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setOptSwitch( true );
    QDirModel *dirModel = new QDirModel;
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::selectMangaData(const QModelIndex&)
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( currentItemId )
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

    if( m1.record(0).value("Vol").toInt() > 0 ){
        pbTV = new LookProgressBar(this);
        pbTV->setTargetFieldDB("vVol");
        pbTV->setValue( m1.record(0).value("vVol").toInt() );
        pbTV->setMaximum( m1.record(0).value("Vol").toInt() );
        pbTV->setFormat("Volume [%v/%m]");
        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("Ch").toInt() > 0 ){
        pbOVA = new LookProgressBar(this);
        pbOVA->setTargetFieldDB("vCh");
        pbOVA->setValue( m1.record(0).value("vCh").toInt() );
        pbOVA->setMaximum( m1.record(0).value("Ch").toInt() );
        pbOVA->setFormat("Charapter [%v/%m]");
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("Pages").toInt() > 0 ){
        pbONA = new LookProgressBar(this);
        pbONA->setTargetFieldDB("vPages");
        pbONA->setValue( m1.record(0).value("vPages").toInt() );
        pbONA->setMaximum( m1.record(0).value("Pages").toInt() );
        pbONA->setFormat("Pages [%v/%m]");
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }

    if( !m1.record(0).value("URL").toString().isEmpty() ){
        ui->Lbl_svTitle->setText( "<a href='"
                                  + m1.record(0).value("URL").toString()
                                  + "'>"
                                  + m1.record(0).value("Title").toString()
                                  + "</a>");
        ui->Lbl_svTitle->setOpenExternalLinks( true );
    }else{
        ui->Lbl_svTitle->setText( m1.record(0).value("Title").toString() );
    }
    if( m1.record(0).value("AltTitle").toString().isEmpty() ){
        ui->Lbl_svOrigTitle->setVisible( false );
        ui->Lbl_sOrigTitle->setVisible( false );
    }else{
        ui->Lbl_svOrigTitle->setText( m1.record(0).value("AltTitle").toString() );
        ui->Lbl_svOrigTitle->setVisible( true );
        ui->Lbl_sOrigTitle->setVisible( true );
    }
    if( m1.record(0).value("Director").toString().isEmpty() ){
        ui->Lbl_svDirector->setVisible( false );
        ui->Lbl_sDirector->setVisible( false );
    }else{
        ui->Lbl_svDirector->setText( m1.record(0).value("Director").toString() );
        ui->Lbl_svDirector->setVisible( true );
        ui->Lbl_sDirector->setVisible( true );
    }
    if( m1.record(0).value("Year").toString().isEmpty() ){
        ui->Lbl_svYear->setVisible( false );
        ui->Lbl_sYear->setVisible( false );
    }else{
        ui->Lbl_svYear->setText( m1.record(0).value("Year").toString() );
        ui->Lbl_svYear->setVisible( true );
        ui->Lbl_sYear->setVisible( true );
    }
    if( m1.record(0).value("Tags").toString().isEmpty() ){
        ui->Lbl_svTags->setVisible( false );
        ui->Lbl_sTags->setVisible( false );
    }else{
        ui->Lbl_svTags->setText( m1.record(0).value("Tags").toString() );
        ui->Lbl_svTags->setVisible( true );
        ui->Lbl_sTags->setVisible( true );
    }
    ui->Lbl_svStudio->setVisible( false );
    ui->Lbl_sStudio->setVisible( false );

    if( m1.record(0).value("Description").toString().isEmpty() ){
        ui->Lbl_VAnimeDescr->setVisible( false );
        ui->Lbl_sAnimeDescr->setVisible( false );
    }else{
        ui->Lbl_VAnimeDescr->setText( m1.record(0).value("Description").toString() );
        ui->Lbl_VAnimeDescr->setVisible( true );
        ui->Lbl_sAnimeDescr->setVisible( true );
    }

    QString imgPath = m1.record(0).value("ImagePath").toString();

    QPixmap pic( imgPath );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    currentItemDir = m1.record(0).value("Dir").toString();

    if( currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setOptSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setOptSwitch( true );
    QDirModel *dirModel = new QDirModel;
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::selectAmvData(const QModelIndex&)
{
    return;
}

void MainWindow::selectDoramaData(const QModelIndex&)
{
    return;
}


void MainWindow::on_listView_ListItemsSection_clicked(const QModelIndex &index)
{
    on_listView_ListItemsSection_activated(index);
}

void MainWindow::on_CB_Section_currentIndexChanged(int = 0)
{

    sections::section sec = static_cast<sections::section>( ui->CB_Section->currentData().toInt() );
    setActiveTable( sec );
    reloadFiltersList();

    Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), filter );
    ui->listView_ListItemsSection->setModelColumn(1);
    if(sec == sections::none)
        ui->stackedWidget->setCurrentIndex(0);
    // ToDo : Проверить соответствие версии БД
    // */
}

void MainWindow::on_CB_Filter_currentIndexChanged(int = 0)
{
    Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), filter );
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_TreeView_Dir_activated(const QModelIndex &index)
{
    QDirModel *m = (QDirModel*)index.model();
    QDesktopServices::openUrl( QUrl::fromLocalFile( m->fileInfo(index).absolutePath()
                                                    + "/" + index.data().toString()
                                                    )  );
}
