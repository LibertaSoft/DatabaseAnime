#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/lookprogressbar.h"
#include "definespath.h"
#include "dbalocalization.h"

#include "dialogs/addanime.h"
#include "dialogs/addmanga.h"
#include "dialogs/addamv.h"
#include "dialogs/adddorama.h"

#include <QDesktopServices>
#include <QScrollArea>
#include <QDirModel>
#include <QDir>

#include <QMessageBox>
#include <QDebug>
#include <QFontDatabase>

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

//#include <QSvgWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pbTV(NULL), pbOVA(NULL), pbONA(NULL), pbSpecial(NULL), pbMovie(NULL), ListWidget_Dir(NULL),
    _btnPlay(NULL), _ScrArea_propertyes(NULL), _restoreDefSettings(false)
{
    DbaSettings settings;
    QLocale::Language language = static_cast<QLocale::Language>(settings.value( Configs::General::Language, QLocale::English ).toInt());
    if( language == 0 ){
        language = QLocale::system().language();
    }
    qtTr.load( DbaLocalization::getQtBaseFileOfLocalization( language, DefinesPath::share() ) );
    dbaTr.load( DbaLocalization::getFileOfLocalization( language, DefinesPath::share() ) );
    qApp->installTranslator(&qtTr);
    qApp->installTranslator(&dbaTr);
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->StackWgt_CoverOrDir->setCurrentIndex(0);
    ui->lbl_AppTitle->setText( qApp->applicationDisplayName() );

    int font_id = QFontDatabase::addApplicationFont("://fonts/URWChanceryL-MediItal.ttf");
    if(font_id >= 0)
        ui->lbl_AppTitle->setFont( QFont( QFontDatabase::applicationFontFamilies(font_id).at(0), 30 ) );

    ui->Lbl_VVersion->setText( qApp->applicationVersion() );

    _displayedField = static_cast<Tables::UniformField::field>( settings.value( Configs::General::DisplayedField, Tables::UniformField::Title ).toInt() );

//    [svg logo]
//    int fAppName_id = QFontDatabase::addApplicationFont("./urw-chancery-l-medium-italic.ttf");
//    ui->lbl_AppTitle->setFont(QFont(QFontDatabase::applicationFontFamilies(fAppName_id).first(),28));

//    ui->Lbl_logo->setVisible( false );
//    QSvgWidget *logo = new QSvgWidget("/tmp/DBA_logo.svg");
//    logo->setFixedSize(600,500);
//    ui->VLay_logoSvg->addWidget( logo );
//    ui->VLay_logoSvg->setAlignment(logo, Qt::AlignCenter);


    // Verification of the new version
    if( settings.value(Configs::Network::CheckUpdates, true).toBool() ){
        QUrl url("https://api.github.com/repos/LibertaSoft/DatabaseAnime/releases");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyVersionVerificationFinished(QNetworkReply*)));
        manager->get( QNetworkRequest(url) );
    }

    this->restoreGeometry( settings.value(Configs::DialogsSettings::MainGeometry).toByteArray() );
    this->restoreState( settings.value(Configs::DialogsSettings::MainState).toByteArray() );
    ui->splitter->restoreGeometry( settings.value(Configs::DialogsSettings::MainSplitterGeometry).toByteArray() );
    ui->splitter->restoreState( settings.value(Configs::DialogsSettings::MainSplitterState).toByteArray() );
    _sort = static_cast<Sort::sort>( settings.value(Configs::General::Sorting, Sort::asc).toInt() );
    bool c1 = settings.value( Configs::General::SwitchCoverOrDir, true ).toBool();
    ui->StackWgt_CoverOrDir->setOptSwitch( c1 );

    mngrConnection.open();
    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();
    MngrQuerys::createTable_Amv();
    MngrQuerys::createTable_Dorama();

    ui->lineEdit_Search->setFocus();
    QueryModel_ListItemsSection = new QSqlQueryModel(this);
    ui->TreeView_List->setModel(QueryModel_ListItemsSection);

    reloadSectionsList();
    reloadFiltersList();
}

void MainWindow::replyVersionVerificationFinished(QNetworkReply* r){
    QJsonDocument doc = QJsonDocument::fromJson( r->readAll() );

    QJsonArray arr  = doc.array();
    QJsonObject obj = arr.at(0).toObject();

    QString verFromGit = obj["tag_name"].toString();
    verFromGit = verFromGit.replace(".", "");// split '.' from "vX.X.X"
    verFromGit = verFromGit.right( verFromGit.length() - 1 );// split 'v' from "vXXX"

    QString appVer = qApp->applicationVersion();
    appVer = appVer.replace(".", "");// split '.' from "X.X.X"
    appVer = appVer.left(3);// split Pre-Alpha/Alpha/Beta/etc

    if( verFromGit.toInt() > appVer.toInt() ){
        ui->Lbl_VVersion->setStyleSheet("color: #f00");
        ui->Lbl_VVersion->setToolTip( tr("Older version") );
    }

    r->deleteLater();
}

void MainWindow::closeEvent(QCloseEvent *e){
    mngrConnection.close();

    DbaSettings settings;
    if( !_restoreDefSettings ){
        settings.setValue(Configs::DialogsSettings::MainGeometry, this->saveGeometry() );
        settings.setValue(Configs::DialogsSettings::MainState,    this->saveState() );
        settings.setValue(Configs::General::ActiveSection, _activeTable);
    }else{
        settings.remove(Configs::DialogsSettings::MainGeometry);
        settings.remove(Configs::DialogsSettings::MainState);
        settings.remove(Configs::General::ActiveSection);
    }

    settings.setValue(Configs::DialogsSettings::MainSplitterGeometry, ui->splitter->saveGeometry() );
    settings.setValue(Configs::DialogsSettings::MainSplitterState, ui->splitter->saveState() );
    e->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PButton_Options_clicked()
{
    sections::section currentSection = static_cast<sections::section>(ui->CB_Section->currentData().toInt());
    DbaSettings set;
    set.setValue(Configs::General::ActiveSection, currentSection);

    Settings formSettings(mngrConnection, this);
    formSettings.setModal(true);
    formSettings.exec();

    QLocale::Language language = formSettings.getLanguage();
    if( language == 0 )
        language = QLocale::system().language();
    qtTr.load( DbaLocalization::getQtBaseFileOfLocalization( language, DefinesPath::share() ) );
    dbaTr.load( DbaLocalization::getFileOfLocalization( language, DefinesPath::share() ) );
    ui->retranslateUi(this);

    _sort = formSettings.getSort();
    _displayedField = formSettings.getDisplaydField();
    ui->StackWgt_CoverOrDir->setOptSwitch( formSettings.getSwitchToDir() );
    _restoreDefSettings = formSettings.getRestoreDefault();

    reloadSectionsList();
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
            DialogAddManga dialogAddManga(this);
            dialogAddManga.setModal(true);
            dialogAddManga.exec();
            break;
        }
        case sections::amv :{
            DialogAddAmv dialogAddAmv(this);
            dialogAddAmv.setModal(true);
            dialogAddAmv.exec();
            break;
        }
        case sections::dorama :{
            DialogAddDorama dialogAddDorama(this);
            dialogAddDorama.setModal(true);
            dialogAddDorama.exec();
            break;
        }
        case sections::none :
        default:
            return;
    }
    QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
}

void MainWindow::on_TButton_Edit_clicked()
{
    if( ui->TreeView_List->selectionModel()->selectedIndexes().isEmpty() == false ){
        switch( getActiveTable() ){
            case sections::anime :{
                DialogAddAnime dialogAddAnime(this, _currentItemId);
                dialogAddAnime.setModal(true);
                dialogAddAnime.exec();
                break;
            }
            case sections::manga :{
                DialogAddManga dialogAddManga(this, _currentItemId);
                dialogAddManga.setModal(true);
                dialogAddManga.exec();
                break;
            }
            case sections::amv :{
                DialogAddAmv dialogAddAmv(this, _currentItemId);
                dialogAddAmv.setModal(true);
                dialogAddAmv.exec();
                break;
            }
            case sections::dorama :{
                DialogAddDorama dialogAddDorama(this, _currentItemId);
                dialogAddDorama.setModal(true);
                dialogAddDorama.exec();
                break;
            }
            case sections::none :
            default:
                return;
        }
        QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
    }else{
        QMessageBox::information(this, tr("Warning"), tr("Section is don't selected"));
    }
}

void MainWindow::on_TButton_Delete_clicked()
{
    if( ui->TreeView_List->selectionModel()->selectedIndexes().isEmpty() == false ){
        QMessageBox* pmbx =
        new QMessageBox(QMessageBox::Question,
            tr("Warning"),
            tr("<b>Delete</b> selected item?"),
            QMessageBox::Yes | QMessageBox::No
        );
        int n = pmbx->exec();
        delete pmbx;
        if (n == QMessageBox::No) {
            return;
        }

        QString coverFolder;

        switch( _activeTable ){
        case sections::anime :
            coverFolder = DefinesPath::animeCovers();
        break;
        case sections::manga :
            coverFolder = DefinesPath::mangaCovers();
        break;
        case sections::amv :
            coverFolder = DefinesPath::amvCovers();
        break;
        case sections::dorama :
            coverFolder = DefinesPath::doramaCovers();
        break;
        case sections::none :
        default:
            return;
        }

        QString coverPath( coverFolder + MngrQuerys::getImagePath(_activeTable, _currentItemId) );

        quint64 tmpId = ui->TreeView_List->selectionModel()->selectedIndexes().at(0).data().toULongLong();


        if( ! MngrQuerys::deleteRecord(_activeTable, tmpId) ){
            QMessageBox::critical(this, tr("Critical"), tr("It was not succeeded to remove record") );
        }else{
            if( coverPath.isNull() == false ){
                QDir().remove( coverPath );
            }
            QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
            ui->stackedWidget->setCurrentIndex(0);
        }
    }
}

void MainWindow::on_TreeView_List_activated(const QModelIndex&)
{
    _currentItemId = ui->TreeView_List->selectionModel()->selectedIndexes().at(0).data().toULongLong();
    ui->stackedWidget->setCurrentIndex(1);
    switch( getActiveTable() ){
        case sections::anime :
            selectAnimeData();
            break;
        case sections::manga :
            selectMangaData();
            break;
        case sections::amv :
            selectAmvData();
            break;
        case sections::dorama :
            selectDoramaData();
            break;
        case sections::none :
        default:
            return;
    }
}

void MainWindow::saveLookValueChanges(int value, int max, QString type)
{
    MngrQuerys::updateRecord(getActiveTable(), _currentItemId, type, QString::number(value) );
    if( value == max && type == "vSeriesTV" )
        MngrQuerys::updateRecord(getActiveTable(), _currentItemId, QString("isHaveLooked"), QString::number(true) );
}

void MainWindow::saveLookValueChanges(int value, int max, QString type, QString nextField)
{
    MngrQuerys::updateRecord(getActiveTable(), _currentItemId, type, QString::number(value) );
    if( value == max && (nextField.isEmpty() == false) ){
        mngrConnection.transaction();
        MngrQuerys::updateRecord(getActiveTable(), _currentItemId, nextField, nextField+"+1" );
        MngrQuerys::updateRecord(getActiveTable(), _currentItemId, type, "0" );
        mngrConnection.commit();
    }
    return;
}

void MainWindow::openFileClicked()
{
    openFile( _currentItemDir );
}

void MainWindow::openFile(QString &file)
{
    QDesktopServices::openUrl( QUrl::fromLocalFile( file ) );
}

void MainWindow::on_lineEdit_Search_textChanged(const QString &strSearch)
{
    Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    MngrQuerys::selectSection(QueryModel_ListItemsSection, getActiveTable(),
                                   _displayedField, filter, _sort, strSearch);
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
    DbaSettings settings;
    bool set_enableBtnAnime  = settings.value(Configs::ActiveSections::Anime,   true).toBool();
    bool set_enableBtnManga  = settings.value(Configs::ActiveSections::Manga,  false).toBool();
    bool set_enableBtnAMV    = settings.value(Configs::ActiveSections::Amv,    false).toBool();
    bool set_enableBtnDorama = settings.value(Configs::ActiveSections::Dorama, false).toBool();

    sections::section set_select
            = static_cast<sections::section>(
                settings.value(Configs::General::ActiveSection, sections::none).toInt() );
    ui->CB_Section->clear();
    ui->CB_Section->addItem( QIcon("://images/icon-section/Main.png"),
                             tr("Main"), sections::none );
    if( set_enableBtnAnime ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/Anime.png"),
                                 tr("Anime"),  sections::anime );
        if( set_select == sections::anime )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnManga ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/Manga.png"),
                                 tr("Manga"),  sections::manga );
        if( set_select == sections::manga )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnAMV ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/Amv.png"),
                                 tr("AMV"),    sections::amv );
        if( set_select == sections::amv )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
    if( set_enableBtnDorama ){
        ui->CB_Section->addItem( QIcon("://images/icon-section/Dorama.png"),
                                 tr("Dorama"), sections::dorama );
        if( set_select == sections::dorama )
            ui->CB_Section->setCurrentIndex( ui->CB_Section->count()-1 );
    }
}

void MainWindow::loadAnimeFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/All.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/WantLook.png"),
                            tr("Want to look"), Filter::wanttolook );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Looked.png"),
                            tr("Looked"), Filter::looked );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Tv.png"),
                            tr("TV"),           Filter::tv );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/MonitorOVA.png"),
                            tr("OVA"),          Filter::ova );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/MonitorONA.png"),
                            tr("ONA"),          Filter::ona );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Special.png"),
                            tr("Special"),      Filter::special );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Movie.png"),
                            tr("Movie"),        Filter::movie );
}
void MainWindow::loadMangaFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/All.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/WantLook.png"),
                            tr("Want to read"), Filter::wanttolook );
}
void MainWindow::loadAmvFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/All.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Edit.png"),
                            tr("Editing"),      Filter::editing );
}
void MainWindow::loadDoramaFilters(){
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/All.png"),
                            tr("All"),          Filter::all );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Edit.png"),
                            tr("Editing"),      Filter::editing );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/WantLook.png"),
                            tr("Want to look"), Filter::wanttolook );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Looked.png"),
                            tr("Looked"), Filter::looked );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Tv.png"),
                            tr("TV"),           Filter::tv );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Special.png"),
                            tr("Special"),      Filter::special );
    ui->CB_Filter->addItem( QIcon("://images/icon-filters/Movie.png"),
                            tr("Movie"),        Filter::movie );
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

void MainWindow::selectAnimeData()
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( _currentItemId )
                );
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

    if( _ScrArea_propertyes ){
        _ScrArea_propertyes->deleteLater();
        _ScrArea_propertyes = NULL;
    }
    if( _btnPlay ){
        _btnPlay->deleteLater();
        _btnPlay = NULL;
    }

    _ScrArea_propertyes = new QScrollArea;
    _ScrArea_propertyes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ScrArea_propertyes->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _ScrArea_propertyes->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _ScrArea_propertyes->setMinimumWidth(300);

    QFormLayout *FLay_propertyes = new QFormLayout(_ScrArea_propertyes);
    _ScrArea_propertyes->setLayout(FLay_propertyes);
    ui->VLay_AnimeDescrFull->addWidget(_ScrArea_propertyes);

    //pbTV->setValue( m1.record(0).value("vSeriesTV").toInt() );
    //pbTV->setMaximum( m1.record(0).value("SeriesTV").toInt() );
    // Title
    QLabel *lblTitle = new QLabel(
                "<a href='"
                + m1.record(0).value("URL").toString().replace("%v", m1.record(0).value("vSeriesTV").toString()).replace("%m", m1.record(0).value("SeriesTV").toString())
                + "'>"
                + m1.record(0).value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( !m1.record(0).value("OrigTitle").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("OrigTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Director").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Season").toInt() > 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Season").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Season:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("PostScoring").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("PostScoring").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Postscoring:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Studios").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Studios").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Studio:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Tags").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Description").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::animeCovers() + m1.record(0).value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = m1.record(0).value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel;
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(_currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::selectMangaData()
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( _currentItemId )
                );
    if( pbTV ){
        pbTV->deleteLater();
        pbTV=NULL;
    }
    if( pbOVA ){
        pbOVA->deleteLater();
        pbOVA=NULL;
    }
    if( pbONA ){
        pbONA->deleteLater();
        pbONA=NULL;
    }
    if( pbSpecial ){
        pbSpecial->deleteLater();
        pbSpecial=NULL;
    }
    if( pbMovie ){
        pbMovie->deleteLater();
        pbMovie=NULL;
    }
    if( _btnPlay ){
        _btnPlay->deleteLater();
        _btnPlay=NULL;
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
        pbOVA->setTargetOverflowFieldDB("vVol");
        pbOVA->setValue( m1.record(0).value("vCh").toInt() );
        pbOVA->setMaximum( m1.record(0).value("Ch").toInt() );
        pbOVA->setFormat("Charapter [%v/%m]");
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }
    if( m1.record(0).value("Pages").toInt() > 0 ){
        pbONA = new LookProgressBar(this);
        pbONA->setTargetFieldDB("vPages");
        pbONA->setTargetOverflowFieldDB("vCh");
        pbONA->setValue( m1.record(0).value("vPages").toInt() );
        pbONA->setMaximum( m1.record(0).value("Pages").toInt() );
        pbONA->setFormat("Pages [%v/%m]");
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, SIGNAL(progressChanged(int,int,QString)), this, SLOT(saveLookValueChanges(int,int,QString)) );
    }

    if( _ScrArea_propertyes ){
        _ScrArea_propertyes->deleteLater();
        _ScrArea_propertyes = NULL;
    }
    _ScrArea_propertyes = new QScrollArea;
    _ScrArea_propertyes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ScrArea_propertyes->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    _ScrArea_propertyes->setMinimumWidth(300);

    QFormLayout *FLay_propertyes = new QFormLayout(_ScrArea_propertyes);
    _ScrArea_propertyes->setLayout(FLay_propertyes);
    ui->VLay_AnimeDescrFull->addWidget(_ScrArea_propertyes);

    // Title
    QLabel *lblTitle = new QLabel(
                "<a href='"
                + m1.record(0).value("URL").toString().replace("%v", m1.record(0).value("vVol").toString()).replace("%m", m1.record(0).value("Vol").toString())
                + "'>"
                + m1.record(0).value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( !m1.record(0).value("AltTitle").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("AltTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Director").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Translation").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Translation").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Translator:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Tags").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Description").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::mangaCovers() + m1.record(0).value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = m1.record(0).value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel;
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(_currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::selectAmvData()
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( _currentItemId )
                );

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

    if( _ScrArea_propertyes ){
        _ScrArea_propertyes->deleteLater();
        _ScrArea_propertyes = NULL;
    }
    _ScrArea_propertyes = new QScrollArea;
    #ifdef QT_DEBUG
        _ScrArea_propertyes->setStyleSheet("border:1px solid black"); // #Bug : Убрать
    #endif

    QFormLayout *FLay_propertyes = new QFormLayout(_ScrArea_propertyes);
    _ScrArea_propertyes->setLayout(FLay_propertyes);
    _ScrArea_propertyes->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->VLay_AnimeDescrFull->addWidget(_ScrArea_propertyes);

    // Title
    QLabel *lblTitle = new QLabel(
                "<a href='"
                + m1.record(0).value("URL").toString()
                + "'>"
                + m1.record(0).value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( !m1.record(0).value("Author").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Author").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Author:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Сontestant").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Сontestant").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Сontestant:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Tags").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("AuthorComment").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("AuthorComment").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Author comment:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( !m1.record(0).value("ContainingMusic").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("ContainingMusic").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Containing music:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( !m1.record(0).value("ContainingAnime").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("ContainingAnime").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Containing anime:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::amvCovers() + m1.record(0).value("ImagePath").toString());
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = m1.record(0).value("Dir").toString();
    ui->StackWgt_CoverOrDir->setDisabledSwitch( false );

    if( _btnPlay ){
        _btnPlay->deleteLater();
        _btnPlay = NULL;
    }
    if( !_currentItemDir.isEmpty() ){
        _btnPlay = new QPushButton( QIcon("://images/play.png"), tr("Play") );
        ui->VLay_BtnPlay->addWidget( _btnPlay );
        QObject::connect(_btnPlay, SIGNAL(clicked()),
                         this,SLOT(openFileClicked()) );
    }
}

void MainWindow::selectDoramaData()
{
    QSqlQueryModel m1;

    m1.setQuery(
                QString("SELECT * FROM '%1' WHERE id='%2'").arg( getActiveTableName() ).arg( _currentItemId )
                );
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
    if( _btnPlay ){
        _btnPlay->deleteLater();
        _btnPlay = NULL;
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

    if( _ScrArea_propertyes ){
        _ScrArea_propertyes->deleteLater();
        _ScrArea_propertyes = NULL;
    }
    _ScrArea_propertyes = new QScrollArea;
    _ScrArea_propertyes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    _ScrArea_propertyes->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _ScrArea_propertyes->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _ScrArea_propertyes->setMinimumWidth(300);

    QFormLayout *FLay_propertyes = new QFormLayout(_ScrArea_propertyes);
    _ScrArea_propertyes->setLayout(FLay_propertyes);
    ui->VLay_AnimeDescrFull->addWidget(_ScrArea_propertyes);

    // Title
    QLabel *lblTitle = new QLabel(
                "<a href='"
                + m1.record(0).value("URL").toString().replace("%v", m1.record(0).value("vSeriesTV").toString()).replace("%m", m1.record(0).value("SeriesTV").toString())
                + "'>"
                + m1.record(0).value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( !m1.record(0).value("OrigTitle").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("OrigTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Director").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( m1.record(0).value("Season").toInt() > 0 ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Season").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Season:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("PostScoring").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("PostScoring").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Postscoring:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Tags").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( !m1.record(0).value("Actors").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Actors").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("In roles:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( !m1.record(0).value("Description").toString().isEmpty() ){
        QLabel *lblValue = new QLabel(m1.record(0).value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::doramaCovers() + m1.record(0).value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = m1.record(0).value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel;
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(_currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::on_TreeView_List_clicked(const QModelIndex &index)
{
    on_TreeView_List_activated(index);
}

void MainWindow::on_CB_Section_currentIndexChanged(int = 0)
{
    sections::section sec = static_cast<sections::section>( ui->CB_Section->currentData().toInt() );
    setActiveTable( sec );
    reloadFiltersList();

    Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), _displayedField, filter, _sort );
    ui->TreeView_List->hideColumn(0);
    if(sec == sections::none){
        ui->stackedWidget->setCurrentIndex(0);
        ui->CB_Filter->setHidden( true );
    }else{
        ui->CB_Filter->setVisible( true );
    }
}

void MainWindow::on_CB_Filter_currentIndexChanged(int = 0)
{
    Filter::filter filter = static_cast<Filter::filter>( ui->CB_Filter->currentData().toInt() );
    MngrQuerys::selectSection( QueryModel_ListItemsSection, getActiveTable(), _displayedField, filter, _sort );
}

void MainWindow::on_TreeView_Dir_activated(const QModelIndex &index)
{
    QDirModel *m = (QDirModel*)index.model();
    QDesktopServices::openUrl( QUrl::fromLocalFile( m->fileInfo(index).absolutePath()
                                                    + QDir::separator() + index.data().toString()
                                                    )  );
}


void MainWindow::on_PBtn_url_vk_clicked()
{
    QDesktopServices::openUrl( QUrl("https://vk.com/db_anime") );
}

void MainWindow::on_PBtn_url_github_clicked()
{
    QDesktopServices::openUrl( QUrl("https://github.com/LibertaSoft/DatabaseAnime") );
}
