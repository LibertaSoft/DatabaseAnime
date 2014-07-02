#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaddanime.h"
#include "lookprogressbar.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pbTV(NULL), pbOVA(NULL), pbONA(NULL), pbSpecial(NULL), pbMovie(NULL), ListWidget_Dir(NULL)
{
    ui->setupUi(this);
    qDebug() << "ui is set!";

    ui->stackedWidget->setCurrentIndex(0);
    ui->lbl_AppTitle->setText( QApplication::applicationDisplayName() );
    ui->Lbl_VVersion->setText( QApplication::applicationVersion() );

    QSettings settings;
    bool set_enableBtnAnime     = settings.value("enableElem/BtnSwitchSection/Anime",   true).toBool();
    bool set_enableBtnManga     = settings.value("enableElem/BtnSwitchSection/Manga",  false).toBool();
    bool set_enableBtnAMV       = settings.value("enableElem/BtnSwitchSection/AMV",    false).toBool();
    bool set_enableBtnDorama    = settings.value("enableElem/BtnSwitchSection/Dorama", false).toBool();
    sections::section set_select
            = static_cast<sections::section>( settings.value("btnSwitchSection/selected", sections::none).toInt() );

    this->restoreGeometry( settings.value("MainWindow/Geometry").toByteArray() );
    this->restoreState( settings.value("MainWindow/State").toByteArray() );

    QFile fileDB( QDir::homePath() + "/." + QApplication::organizationName() + "/" + QApplication::applicationName() + "/DatabaseAnime.sqlite" );
    bool firstRun = !fileDB.exists();
    mngrConnection.open();
    if( firstRun ){
        MngrQuerys::createTable_AnimeSerials();
        MngrQuerys::createTable_AnimeTags();
        MngrQuerys::insert_defaultAnimeTags();
    }

    ui->lineEdit_Search->setFocus();
    QueryModel_ListItemsSection = new QSqlQueryModel(this);
    ui->listView_ListItemsSection->setModel(QueryModel_ListItemsSection);
    ui->listView_ListItemsSection->setModelColumn(1);

    if( set_enableBtnAnime ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-anime.png"),
                                 tr("Anime"),  sections::anime );
        if( set_select == sections::anime )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnManga ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-manga.png"),
                                 tr("Manga"),  sections::manga );
        if( set_select == sections::manga )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnAMV ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-amv.png"),
                                 tr("AMV"),    sections::amv );
        if( set_select == sections::amv )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnDorama ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-dorama.png"),
                                 tr("Dorama"), sections::dorama );
        if( set_select == sections::dorama )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    ui->CB_Filter->addItem( QIcon("://images/list-add-active.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_look.png"),
                            tr("Want to look"), Filter::wanttolook );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_tv.png"),
                            tr("TV"),           Filter::tv );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_ova.png"),
                            tr("OVA"),          Filter::ova );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_ona.png"),
                            tr("ONA"),          Filter::ona );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_special.png"),
                            tr("Special"),      Filter::special );
    ui->CB_Filter->addItem( QIcon(":/icon-filters/images/icon-filters/filter_movie.png"),
                            tr("Movie"),        Filter::movie );

}

void MainWindow::closeEvent(QCloseEvent *e){
    mngrConnection.close();

    sections::section select = static_cast<sections::section>( ui->CB_Section->currentData().toInt() );;
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

//  #Bug : Дублирующийся код ... брр, ужасная реализация.
    QSettings settings;
    bool set_enableBtnAnime     = settings.value("enableElem/BtnSwitchSection/Anime",     true).toBool();
    bool set_enableBtnManga     = settings.value("enableElem/BtnSwitchSection/Manga",    false).toBool();
    bool set_enableBtnAMV       = settings.value("enableElem/BtnSwitchSection/AMV",      false).toBool();
    bool set_enableBtnDorama    = settings.value("enableElem/BtnSwitchSection/Dorama",   false).toBool();

    sections::section set_select
            = static_cast<sections::section>( settings.value("btnSwitchSection/selected", sections::none).toInt() );
    ui->CB_Section->clear();
    if( set_enableBtnAnime ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-anime.png"),
                                 tr("Anime"),  sections::anime );
        if( set_select == sections::anime )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnManga ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-manga.png"),
                                 tr("Manga"),  sections::manga );
        if( set_select == sections::manga )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnAMV ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-amv.png"),
                                 tr("AMV"),    sections::amv );
        if( set_select == sections::amv )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnDorama ){
        ui->CB_Section->addItem( QIcon(":/icon-section/images/icon-section/section-dorama.png"),
                                 tr("Dorama"), sections::dorama );
        if( set_select == sections::dorama )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
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
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable() );
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
//    QueryModel_ListItemsSection->setQuery( QString("SELECT id,Title FROM %1").arg( getActiveTableName() ) );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable() );
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

void MainWindow::on_lineEdit_Search_textChanged(const QString &strSearch)
{
    // #Bug : Выставить фильтр в All
    // #ToDo : Реализовать поиск по секциям (Хочу посмотреть / Ещё редактируется)
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

void MainWindow::setActiveTable(sections::section table)
{
    _activeTable = table;
}

void MainWindow::selectAnimeData(const QModelIndex&)
{
    QSqlQueryModel m1;

    int i = ui->listView_ListItemsSection->selectionModel()->selectedIndexes().at(0).data().toInt();

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( i )
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
    if( m1.record(0).value("SeriesMovie").toInt() > 0 ){
        pbMovie = new LookProgressBar(this);
        pbMovie->setTargetFieldDB("vSeriesMovie");
        pbMovie->setValue( m1.record(0).value("vSeriesMovie").toInt() );
        pbMovie->setMaximum( m1.record(0).value("SeriesMovie").toInt() );
        pbMovie->setFormat("Movie [%v/%m]");
        ui->HLay_WBRow2->addWidget( pbMovie );
        QObject::connect(pbMovie, SIGNAL(progressChanged(int,QString)), this, SLOT(saveLookValueChanges(int,QString)) );
    }

    ui->Lbl_svTitle->setText( m1.record(0).value("Title").toString() );
    ui->Lbl_svOrigTitle->setText( m1.record(0).value("OrigTitle").toString() );
    ui->Lbl_svDirector->setText( m1.record(0).value("Director").toString() );
    ui->Lbl_svYear->setText( m1.record(0).value("Year").toString() );
    ui->Lbl_svTags->setText( m1.record(0).value("Tags").toString() );
    ui->Lbl_svStudio->setText( m1.record(0).value("Studios").toString() );
    ui->Lbl_svURL->setText( m1.record(0).value("URL").toString() );
    ui->Lbl_VAnimeDescr->setText( m1.record(0).value("Description").toString() );
    QString imgPath = m1.record(0).value("ImagePath").toString();

    QPixmap pic( imgPath );
    if( pic.isNull() ){
        pic.load( "://images/DBA_NoImage.png" );
    }
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
//        ListWidget_Dir->setSelectionMode( QListView::MultiSelection );
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


void MainWindow::on_listView_ListItemsSection_clicked(const QModelIndex &index)
{
    on_listView_ListItemsSection_activated(index);
}

void MainWindow::on_CB_Section_currentIndexChanged(int)
{
    sections::section sec = static_cast<sections::section>( ui->CB_Section->currentData().toInt() );
    Filter::filter    fil = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    setActiveTable( sec );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), fil );
    ui->listView_ListItemsSection->setModelColumn(1);
}

void MainWindow::on_CB_Filter_currentIndexChanged(int)
{
    on_CB_Section_currentIndexChanged(0);
}
