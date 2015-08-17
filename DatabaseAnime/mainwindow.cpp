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
    pbTV(nullptr), pbOVA(nullptr), pbONA(nullptr), pbSpecial(nullptr), pbMovie(nullptr),
    ListWidget_Dir(nullptr), _btnPlay(nullptr), _ScrArea_propertyes(nullptr), _restoreDefSettings(false)
{
    QSettings settings;
    QLocale::Language language = static_cast<QLocale::Language>(settings.value( Options::General::LANGUAGE, QLocale::English ).toInt());
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
        ui->lbl_AppTitle->setFont( QFont( QFontDatabase::applicationFontFamilies(font_id).first(), 30 ) );

    ui->Lbl_VVersion->setText( qApp->applicationVersion() );

    _displayedField = static_cast<Tables::UniformField::field>( settings.value( Options::General::DISPLAYED_FIELD, Tables::UniformField::TITLE ).toInt() );

//    [svg logo]
//    ui->Lbl_logo->setVisible( false );
//    QSvgWidget *logo = new QSvgWidget("/tmp/DBA_logo.svg");
//    logo->setFixedSize(600,500);
//    ui->VLay_logoSvg->addWidget( logo );
//    ui->VLay_logoSvg->setAlignment(logo, Qt::AlignCenter);

    // Verification of the new version
    if( settings.value(Options::Network::CHECK_UPDATES, true).toBool() ){
        QUrl url("https://api.github.com/repos/LibertaSoft/DatabaseAnime/releases");
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished,
                this, &MainWindow::replyVersionVerificationFinished);
        manager->get( QNetworkRequest(url) );
    }

    this->restoreGeometry( settings.value(Options::Dialogs::MainWindow::Geometry).toByteArray() );
    this->restoreState( settings.value(Options::Dialogs::MainWindow::State).toByteArray() );
    ui->splitter->restoreGeometry( settings.value(Options::Dialogs::MainWindow::Splitter::Geometry).toByteArray() );
    ui->splitter->restoreState( settings.value(Options::Dialogs::MainWindow::Splitter::State).toByteArray() );
    _sort = static_cast<Sort::sort>( settings.value(Options::General::SORTING, Sort::asc).toInt() );
    bool c1 = settings.value( Options::General::SwitchCoverOrDir, true ).toBool();
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
    // split '.' from "vX.X.X"
    verFromGit = verFromGit.replace(".", "");
    // split 'v' from "vXXX"
    verFromGit = verFromGit.right( verFromGit.length() - 1 );

    QString appVer = qApp->applicationVersion();
    // split '.' from "X.X.X"
    appVer = appVer.replace(".", "");
    // split Pre-Alpha/Alpha/Beta/etc
    appVer = appVer.left(3);

    if( verFromGit.toInt() > appVer.toInt() ){
        ui->Lbl_VVersion->setStyleSheet("color: #f00");
        ui->Lbl_VVersion->setToolTip( tr("Older version") );
    }

    r->deleteLater();
}

void MainWindow::closeEvent(QCloseEvent *e){
    mngrConnection.commit();
    mngrConnection.close();

    QSettings settings;
    if( !_restoreDefSettings ){
        settings.setValue(Options::Dialogs::MainWindow::Geometry, this->saveGeometry() );
        settings.setValue(Options::Dialogs::MainWindow::State,    this->saveState() );
        settings.setValue(Options::General::ActiveSection, _activeTable);
    }else{
        settings.remove(Options::Dialogs::MainWindow::Geometry);
        settings.remove(Options::Dialogs::MainWindow::State);
        settings.remove(Options::General::ActiveSection);
    }

    settings.setValue(Options::Dialogs::MainWindow::Splitter::Geometry, ui->splitter->saveGeometry() );
    settings.setValue(Options::Dialogs::MainWindow::Splitter::State, ui->splitter->saveState() );
    e->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PButton_Options_clicked()
{
    sections::section currentSection = static_cast<sections::section>(ui->CB_Section->currentData().toInt());
    QSettings settings;
    settings.setValue(Options::General::ActiveSection, currentSection);

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
    QDialog *dialog;
    switch( getActiveTable() ){
        case sections::anime :{
            dialog = new DialogAddAnime(this);
            break;
        }
        case sections::manga :{
            dialog = new DialogAddManga(this);
            break;
        }
        case sections::amv :{
            dialog = new DialogAddAmv(this);
            break;
        }
        case sections::dorama :{
            dialog = new DialogAddDorama(this);
            break;
        }
        case sections::none :
        default:
            return;
    }
    dialog->setModal(true);
    dialog->exec();
    QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
}

void MainWindow::on_TButton_Edit_clicked()
{
    if( ui->TreeView_List->selectionModel()->selectedIndexes().isEmpty() == false ){
        QDialog *dialog;
        switch( getActiveTable() ){
            case sections::anime :
                dialog = new DialogAddAnime(this, _currentItemId);
                break;
            case sections::manga :
                dialog = new DialogAddManga(this, _currentItemId);
                break;
            case sections::amv :
                dialog = new DialogAddAmv(this, _currentItemId);
                break;
            case sections::dorama :
                dialog = new DialogAddDorama(this, _currentItemId);
                break;
            case sections::none :
            default:
                return;
        }
        dialog->setModal(true);
        dialog->exec();
        QueryModel_ListItemsSection->setQuery( QueryModel_ListItemsSection->query().executedQuery() );
    }else{
        QMessageBox::information(this, tr("Warning"), tr("Item isn't selected"));
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

        QString coverFolder = DefinesPath::getCoversFolder(_activeTable);
        if( coverFolder == QString::null )
            return;

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
    }else{
        QMessageBox::information(this, tr("Warning"), tr("Item isn't selected"));
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

void MainWindow::saveLookValueChanges(int value, QString field)
{
    MngrQuerys::updateRecord(getActiveTable(), _currentItemId, field, QString::number(value) );
}
/*
void MainWindow::saveLookValueChanges(int value, int max, QString field, QString nextField)
{
    MngrQuerys::updateRecord(getActiveTable(), _currentItemId, field, QString::number(value) );
    if( value == max && (nextField.isEmpty() == false) ){
        mngrConnection.transaction();
        MngrQuerys::updateRecord(getActiveTable(), _currentItemId, nextField, nextField+"+1" );
        MngrQuerys::updateRecord(getActiveTable(), _currentItemId, field, "0" );
        mngrConnection.commit();
    }
    return;
}
*/
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
    QSettings settings;
    bool set_enableBtnAnime  = settings.value(Options::ActiveSections::ANIME,   true).toBool();
    bool set_enableBtnManga  = settings.value(Options::ActiveSections::MANGA,  false).toBool();
    bool set_enableBtnAMV    = settings.value(Options::ActiveSections::AMV,    false).toBool();
    bool set_enableBtnDorama = settings.value(Options::ActiveSections::DORAMA, false).toBool();

    sections::section set_select
            = static_cast<sections::section>(
                settings.value(Options::General::ActiveSection, sections::none).toInt() );
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
    ui->CB_Filter->blockSignals( true );
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
    ui->CB_Filter->blockSignals( false );
}

void MainWindow::setActiveTable(sections::section table)
{
    _activeTable = table;
}

void MainWindow::selectAnimeData()
{
    mngrConnection.commit();
    mngrConnection.transaction();

    QSqlRecord record = MngrQuerys::selectData(_activeTable, _currentItemId);

    deleteLookProgressBars();

    if( record.value("SeriesTV").toInt() > 0 ){
        const QString fieldName_ValueTv = Tables::Anime::Fields::vSeriesTV;
        const QString fieldName_AllTv = Tables::Anime::Fields::SeriesTV;
        pbTV = new LookProgressBar(0,
                                   record.value(fieldName_ValueTv).toInt(),
                                   record.value(fieldName_AllTv).toInt(),
                                   "TV [%v/%m]", fieldName_ValueTv,
                                   this);

        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, &LookProgressBar::progressChanged,
                         this, &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesOVA").toInt() > 0 ){
        pbOVA = new LookProgressBar(0,
                                    record.value("vSeriesOVA").toInt(),
                                    record.value("SeriesOVA").toInt(),
                                    "OVA [%v/%m]", "vSeriesOVA",
                                    this);
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, &LookProgressBar::progressChanged,
                         this,  &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesONA").toInt() > 0 ){
        pbONA = new LookProgressBar(0,
                                    record.value("vSeriesONA").toInt(),
                                    record.value("SeriesONA").toInt(),
                                    "ONA [%v/%m]", "vSeriesONA",
                                    this);
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, &LookProgressBar::progressChanged,
                         this,  &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesSpecial").toInt() > 0 ){
        pbSpecial = new LookProgressBar(0,
                                        record.value("vSeriesSpecial").toInt(),
                                        record.value("SeriesSpecial").toInt(),
                                        "Special [%v/%m]", "vSeriesSpecial",
                                        this);
        ui->HLay_WBRow2->addWidget( pbSpecial );
        QObject::connect(pbSpecial, &LookProgressBar::progressChanged,
                         this,      &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesMovie").toInt() > 0 ){
        pbMovie = new LookProgressBar(0,
                                      record.value("vSeriesMovie").toInt(),
                                      record.value("SeriesMovie").toInt(),
                                      "Movie [%v/%m]", "vSeriesMovie",
                                      this);
        ui->HLay_WBRow2->addWidget( pbMovie );
        QObject::connect(pbMovie, &LookProgressBar::progressChanged,
                         this,    &MainWindow::saveLookValueChanges );
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
                + record.value("URL").toString().replace("%v", record.value("vSeriesTV").toString()).replace("%n", QString::number(record.value("vSeriesTV").toInt()+1)).replace("%m", record.value("SeriesTV").toString())
                + "'>"
                + record.value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( record.value("OrigTitle").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("OrigTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( record.value("Director").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( record.value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(record.value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( record.value("Season").toInt() > 0 ){
        QLabel *lblValue = new QLabel(record.value("Season").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Season:") + "</b>", lblValue );
    }
    if( record.value("PostScoring").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("PostScoring").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Postscoring:") + "</b>", lblValue );
    }
    if( record.value("Studios").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Studios").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Studio:") + "</b>", lblValue );
    }
    if( record.value("Tags").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( record.value("Description").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::animeCovers() + record.value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = record.value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel( _ScrArea_propertyes );
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
    mngrConnection.commit();
    mngrConnection.transaction();

    QSqlRecord record = MngrQuerys::selectData(_activeTable, _currentItemId);

    deleteLookProgressBars();

    if( record.value( Tables::Manga::Fields::Vol ).toInt() > 0 ){
        pbTV = new LookProgressBar(0,
                                   record.value( Tables::Manga::Fields::vVol ).toInt(),
                                   record.value( Tables::Manga::Fields::Vol ).toInt(),
                                   "Volume [%v/%m]", Tables::Manga::Fields::vVol,
                                   this);
        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, &LookProgressBar::progressChanged,
                         this, &MainWindow::saveLookValueChanges );
    }
    if( record.value( Tables::Manga::Fields::Ch ).toInt() > 0 ){
        pbOVA = new LookProgressBar(0,
                                    record.value( Tables::Manga::Fields::vCh ).toInt(),
                                    record.value( Tables::Manga::Fields::Ch ).toInt(),
                                    "Charapter [%v/%m]",  Tables::Manga::Fields::vCh ,
                                    this);
        ui->HLay_WBRow1->addWidget(pbOVA);
        QObject::connect(pbOVA, &LookProgressBar::progressChanged,
                         this,  &MainWindow::saveLookValueChanges );
    }
    if( record.value( Tables::Manga::Fields::Pages ).toInt() > 0 ){
        pbONA = new LookProgressBar(0,
                                    record.value( Tables::Manga::Fields::vPages ).toInt(),
                                    record.value( Tables::Manga::Fields::Pages ).toInt(),
                                    "Pages [%v/%m]", Tables::Manga::Fields::vPages,
                                    this);
        ui->HLay_WBRow1->addWidget( pbONA );
        QObject::connect(pbONA, &LookProgressBar::progressChanged,
                         this,  &MainWindow::saveLookValueChanges );
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
                + record.value("URL").toString().replace("%v", record.value("vSeriesTV").toString()).replace("%n", record.value("vVol").toString()).replace("%m", record.value("Vol").toString())
                + "'>"
                + record.value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( record.value("AltTitle").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("AltTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( record.value("Director").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( record.value("Translation").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Translation").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Translator:") + "</b>", lblValue );
    }
    if( record.value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(record.value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( record.value("Tags").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( record.value("Description").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::mangaCovers() + record.value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = record.value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel(_ScrArea_propertyes);
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
    mngrConnection.commit();
    mngrConnection.transaction();

    QSqlRecord record = MngrQuerys::selectData(_activeTable, _currentItemId);

    deleteLookProgressBars();

    _ScrArea_propertyes = new QScrollArea;
    QFormLayout *FLay_propertyes = new QFormLayout(_ScrArea_propertyes);
    _ScrArea_propertyes->setLayout(FLay_propertyes);
    _ScrArea_propertyes->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->VLay_AnimeDescrFull->addWidget(_ScrArea_propertyes);

    // Title
    QLabel *lblTitle = new QLabel(
                "<a href='"
                + record.value("URL").toString()
                + "'>"
                + record.value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( record.value("Author").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Author").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Author:") + "</b>", lblValue );
    }
    if( record.value("Сontestant").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Сontestant").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Сontestant:") + "</b>", lblValue );
    }
    if( record.value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(record.value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( record.value("Tags").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( record.value("AuthorComment").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("AuthorComment").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Author comment:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( record.value("ContainingMusic").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("ContainingMusic").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Containing music:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( record.value("ContainingAnime").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("ContainingAnime").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Containing anime:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::amvCovers() + record.value("ImagePath").toString());
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = record.value("Dir").toString();
    ui->StackWgt_CoverOrDir->setDisabledSwitch( false );

    if( ! _currentItemDir.isEmpty() ){
        _btnPlay = new QPushButton( QIcon("://images/play.png"), tr("Play") );

        ui->VLay_BtnPlay->addWidget( _btnPlay );
        ui->VLay_BtnPlay->setContentsMargins(6, 0, 0, 0);
        QObject::connect(_btnPlay, &QAbstractButton::clicked,
                         this,     &MainWindow::openFileClicked );
    }
}

void MainWindow::selectDoramaData()
{
    mngrConnection.commit();
    mngrConnection.transaction();

    QSqlRecord record = MngrQuerys::selectData(_activeTable, _currentItemId);

    deleteLookProgressBars();

    if( record.value("SeriesTV").toInt() > 0 ){
        pbTV = new LookProgressBar(0,
                                   record.value("vSeriesTV").toInt(),
                                   record.value("SeriesTV").toInt(),
                                   "TV [%v/%m]", "vSeriesTV",
                                   this);
        ui->HLay_WBRow0->addWidget( pbTV );
        QObject::connect(pbTV, &LookProgressBar::progressChanged,
                         this, &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesSpecial").toInt() > 0 ){
        pbSpecial = new LookProgressBar(0,
                                        record.value("vSeriesSpecial").toInt(),
                                        record.value("SeriesSpecial").toInt(),
                                        "Special [%v/%m]", "vSeriesSpecial",
                                        this);
        ui->HLay_WBRow2->addWidget( pbSpecial );
        QObject::connect(pbSpecial, &LookProgressBar::progressChanged,
                         this,      &MainWindow::saveLookValueChanges );
    }
    if( record.value("SeriesMovie").toInt() > 0 ){
        pbMovie = new LookProgressBar(0,
                                      record.value("vSeriesMovie").toInt(),
                                      record.value("SeriesMovie").toInt(),
                                      "Movie [%v/%m]", "vSeriesMovie",
                                      this);
        ui->HLay_WBRow2->addWidget( pbMovie );
        QObject::connect(pbMovie, &LookProgressBar::progressChanged,
                         this,    &MainWindow::saveLookValueChanges );
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
                + record.value("URL").toString().replace("%v", record.value("vSeriesTV").toString()).replace("%n", record.value("vSeriesTV").toString()).replace("%m", record.value("SeriesTV").toString())
                + "'>"
                + record.value("Title").toString()
                + "</a>", _ScrArea_propertyes);
    lblTitle->setWordWrap( true );
    lblTitle->setOpenExternalLinks( true );
    FLay_propertyes->addRow( "<b>" + tr("Title:") + "</b>", lblTitle);
    if( record.value("OrigTitle").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("OrigTitle").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Alt title:") + "</b>", lblValue );
    }
    if( record.value("Director").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Director").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Director:") + "</b>", lblValue );
    }
    if( record.value("Year").toInt() != 0 ){
        QLabel *lblValue = new QLabel(record.value("Year").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Year:") + "</b>", lblValue );
    }
    if( record.value("Season").toInt() > 0 ){
        QLabel *lblValue = new QLabel(record.value("Season").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Season:") + "</b>", lblValue );
    }
    if( record.value("PostScoring").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("PostScoring").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Postscoring:") + "</b>", lblValue );
    }
    if( record.value("Tags").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Tags").toString(), _ScrArea_propertyes);
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow( "<b>" + tr("Ganres:") + "</b>", lblValue );
    }
    if( record.value("Actors").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Actors").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("In roles:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }
    if( record.value("Description").toString().isEmpty() == false ){
        QLabel *lblValue = new QLabel(record.value("Description").toString(), _ScrArea_propertyes);
        QLabel *lblTitle = new QLabel( "<b>" + tr("Description:") + "</b>", _ScrArea_propertyes );
        lblValue->setWordWrap( true );
        lblValue->setOpenExternalLinks( true );
        FLay_propertyes->addRow(lblTitle);
        FLay_propertyes->addRow(lblValue);
    }

    QPixmap pic( DefinesPath::doramaCovers() + record.value("ImagePath").toString() );
    if( pic.isNull() ){
        pic.load( "://images/NoImage.png" );
    }
    ui->Lbl_ImageCover->setPixmap( pic );

    _currentItemDir = record.value("Dir").toString();

    if( _currentItemDir.isEmpty() )
        ui->StackWgt_CoverOrDir->setDisabledSwitch( false );
    else
        ui->StackWgt_CoverOrDir->setDisabledSwitch( true );
    QDirModel *dirModel = new QDirModel(_ScrArea_propertyes);
//    dirModel->setNameFilters( QStringList() << "*ona*" << "*ova*" << "*special*" << "*tv*" );
    dirModel->setSorting( QDir::DirsFirst | QDir::Type | QDir::Name );

    ui->TreeView_Dir->setModel( dirModel );
    ui->TreeView_Dir->setRootIndex( dirModel->index(_currentItemDir) );
    ui->TreeView_Dir->setColumnHidden(1, true);
    ui->TreeView_Dir->setColumnHidden(2, true);
    ui->TreeView_Dir->setColumnHidden(3, true);
}

void MainWindow::deleteLookProgressBars()
{
    if( pbTV ){
        delete pbTV;
        pbTV = nullptr;
    }
    if( pbOVA ){
        delete pbOVA;
        pbOVA = nullptr;
    }
    if( pbONA ){
        delete pbONA;
        pbONA = nullptr;
    }
    if( pbSpecial ){
        delete pbSpecial;
        pbSpecial = nullptr;
    }
    if( pbMovie ){
        delete pbMovie;
        pbMovie = nullptr;
    }
    if( _btnPlay ){
        _btnPlay->deleteLater();
        _btnPlay = nullptr;
    }
    if( _ScrArea_propertyes ){
        _ScrArea_propertyes->deleteLater();
        _ScrArea_propertyes = nullptr;
    }
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
//    Show column 'Year'
//    if( _sort != Sort::year ){
//        ui->TreeView_List->hideColumn(2);
//    }else{
//        ui->TreeView_List->setColumnWidth(2, 50);
//        ui->TreeView_List->showColumn(2);
//    }
    bool enableButtons(true);
    if(sec == sections::none){
        enableButtons = false;
        ui->lineEdit_Search->clear();
        ui->stackedWidget->setCurrentIndex(0);
    }
    ui->CB_Filter->setVisible( enableButtons );
    ui->TButton_Add->setEnabled( enableButtons );
    ui->TButton_Edit->setEnabled( enableButtons );
    ui->TButton_Delete->setEnabled( enableButtons );
    ui->lineEdit_Search->setEnabled( enableButtons );
    ui->TreeView_List->setEnabled( enableButtons );
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
