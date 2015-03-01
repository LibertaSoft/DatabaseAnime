#include "settings.h"
#include "ui_settings.h"

Settings::Settings(MngrConnection &MngrCon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings), MngrConnect(MngrCon)
{
    ui->setupUi(this);
    ui->ProgressBar_Export->setVisible(false);
    ui->ProgressBar_Import->setVisible(false);

    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Config::Geometry).toByteArray() );
    ui->splitter->restoreGeometry( settings.value(Options::Dialogs::Config::Splitter::Geometry).toByteArray() );
    ui->splitter->restoreState( settings.value(Options::Dialogs::Config::Splitter::State).toByteArray() );
    ui->LineEdit_Export_FilePath->setText( DefinesPath::home() );
    ui->LineEdit_Import_FilePath->setText( DefinesPath::home() );

    bool b1 = settings.value( Options::ActiveSections::Anime,   true ).toBool();
    bool b2 = settings.value( Options::ActiveSections::Manga,  false ).toBool();
    bool b3 = settings.value( Options::ActiveSections::Amv,    false ).toBool();
    bool b4 = settings.value( Options::ActiveSections::Dorama, false ).toBool();


    bool a1 = settings.value( Options::OptionalFields::Anime::AltTitle,    false ).toBool();
    bool a2 = settings.value( Options::OptionalFields::Anime::Director,    false ).toBool();
    bool a3 = settings.value( Options::OptionalFields::Anime::Postscoring, false ).toBool();

    bool m1 = settings.value( Options::OptionalFields::Manga::AltTitle,    false ).toBool();
    bool m2 = settings.value( Options::OptionalFields::Manga::Author,      false ).toBool();
    bool m3 = settings.value( Options::OptionalFields::Manga::Translator, false ).toBool();

    bool d1 = settings.value( Options::OptionalFields::Dorama::AltTitle, false ).toBool();
    bool d2 = settings.value( Options::OptionalFields::Dorama::Director, false ).toBool();

    bool checkUpdates      = settings.value(Options::Network::CheckUpdates,          true).toBool();
    bool searchOnShikimori = settings.value(Options::Network::AutoSearchOnShikimori, true).toBool();

    bool SwitchCoverOrDir = settings.value( Options::General::SwitchCoverOrDir, true ).toBool();

    ui->ChBox_AS_Anime->setChecked( b1 );
    ui->ChBox_AS_Manga->setChecked( b2 );
    ui->ChBox_AS_Amv->setChecked( b3 );
    ui->ChBox_AS_Dorama->setChecked( b4 );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( a1 );
    ui->ChBox_OptField_Anime_Director->setChecked( a2 );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( a3 );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( m1 );
    ui->ChBox_OptField_Manga_Author->setChecked( m2 );
    ui->ChBox_OptField_Manga_Translation->setChecked( m3 );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( d1 );
    ui->ChBox_OptField_Dorama_Director->setChecked( d2 );

    ui->ChBox_CheckForUpdate->setChecked( checkUpdates );
    ui->ChBox_SwitchCoverOrDir->setChecked( SwitchCoverOrDir );
    ui->ChBox_SearchOnShikimori->setChecked( searchOnShikimori );

    QLocale::Language set_language = static_cast<QLocale::Language>(settings.value( Options::General::Language, QLocale::English ).toInt());

    ui->ComboBox_Language->addItem( tr("<System>"), 0 );

    QMap<QLocale::Language,QString> langList = DbaLocalization::readExistsLocalizations( DefinesPath::share() );
    int i(0);
    foreach (QString langName, langList) {
        ++i;
        ui->ComboBox_Language->addItem(langName, langList.key(langName));
        if( set_language == langList.key(langName) )
            ui->ComboBox_Language->setCurrentIndex(i);
    }
    // Sorting
    {  // Sorting
        ui->ComboBox_ItemList_Sorting->addItem(tr("None"), Sort::none);
        ui->ComboBox_ItemList_Sorting->addItem(tr("ASC"),  Sort::asc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("DESC"), Sort::desc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("Year"), Sort::year);

        Sort::sort sort = static_cast<Sort::sort>( settings.value( Options::General::Sorting, Sort::asc ).toInt() );
        ui->ComboBox_ItemList_Sorting->setCurrentIndex( sort );
    }
    // Work dir
    ui->LineEdit_WorkDir->setText( QDir::toNativeSeparators( DefinesPath::appData() ) );

    // Displayed field
    {   // Displayed field
        Tables::UniformField::field displayedField =
                static_cast<Tables::UniformField::field>(
                    settings.value( Options::General::DisplayedField, Tables::UniformField::Title ).toInt() );
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Title"), Tables::UniformField::Title);
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Alternative title"), Tables::UniformField::AltTitle);
        if( displayedField == Tables::UniformField::Title )
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( 0 );
        else
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( 1 );
    }
}

Settings::~Settings()
{
    QSettings settings;
    settings.setValue(Options::Dialogs::Config::Geometry, this->saveGeometry() );

    settings.setValue(Options::Dialogs::Config::Splitter::Geometry, ui->splitter->saveGeometry() );
    settings.setValue(Options::Dialogs::Config::Splitter::State,    ui->splitter->saveState() );
    delete ui;
}

Sort::sort Settings::getSort()
{
    return static_cast<Sort::sort>( ui->ComboBox_ItemList_Sorting->currentIndex() );
}

Tables::UniformField::field Settings::getDisplaydField()
{
    return static_cast<Tables::UniformField::field>(ui->ComboBox_ItemList_DisplayedField->currentData().toInt());
}

bool Settings::getSwitchToDir()
{
    return ui->ChBox_SwitchCoverOrDir->isChecked();
}

bool Settings::getRestoreDefault()
{
    return restoreDefault;
}

QLocale::Language Settings::getLanguage()
{
    return static_cast<QLocale::Language>( ui->ComboBox_Language->currentData().toInt() );
}

void Settings::on_listWidget_currentRowChanged(int currentRow)
{
    ui->StackedWidget->setCurrentIndex(currentRow);
}

void Settings::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case  QDialogButtonBox::ApplyRole:
            on_BtnBox_accepted();
            this->close();
            break;
        case QDialogButtonBox::ResetRole:
            BtnBox_resetDefaults();
            break;
        default:
            this->close();
    }
}

void Settings::on_BtnBox_accepted()
{
    QSettings settings;
    {
        using namespace Options::ActiveSections;
        settings.setValue( Anime,  ui->ChBox_AS_Anime->isChecked() );
        settings.setValue( Manga,  ui->ChBox_AS_Manga->isChecked() );
        settings.setValue( Amv,    ui->ChBox_AS_Amv->isChecked() );
        settings.setValue( Dorama, ui->ChBox_AS_Dorama->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Anime;
        settings.setValue( AltTitle,   ui->ChBox_OptField_Anime_AltTitle->isChecked() );
        settings.setValue( Director,    ui->ChBox_OptField_Anime_Director->isChecked() );
        settings.setValue( Postscoring, ui->ChBox_OptField_Anime_Postscoring->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Manga;
        settings.setValue( AltTitle,    ui->ChBox_OptField_Manga_AltTitle->isChecked() );
        settings.setValue( Author,      ui->ChBox_OptField_Manga_Author->isChecked() );
        settings.setValue( Translator, ui->ChBox_OptField_Manga_Translation->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Dorama;
        settings.setValue( AltTitle,   ui->ChBox_OptField_Dorama_AltTitle->isChecked() );
        settings.setValue( Director,   ui->ChBox_OptField_Dorama_Director->isChecked() );
    }
    {
        using namespace Options::General;
        settings.setValue( Language, ui->ComboBox_Language->currentData() );
        settings.setValue( Sorting, ui->ComboBox_ItemList_Sorting->currentIndex() );
    }
    {
        using namespace Options::Network;
        settings.setValue( CheckUpdates, ui->ChBox_CheckForUpdate->isChecked() );
        settings.setValue( AutoSearchOnShikimori, ui->ChBox_SearchOnShikimori->isChecked() );
    }

    settings.setValue( Options::General::SwitchCoverOrDir, ui->ChBox_SwitchCoverOrDir->isChecked() );

    if( QDir::isAbsolutePath( ui->LineEdit_WorkDir->text() ) )
        settings.setValue( Options::General::WorkDirectory, QDir(ui->LineEdit_WorkDir->text()).path() );
    else
        settings.remove(Options::General::WorkDirectory);

    // Displayed field
    Tables::UniformField::field displayedField = static_cast<Tables::UniformField::field>( ui->ComboBox_ItemList_DisplayedField->currentData().toInt() );
    settings.setValue( Options::General::DisplayedField, displayedField );
}

void Settings::BtnBox_resetDefaults()
{
    restoreDefault = true;
    ui->ChBox_AS_Anime->setChecked( true );
    ui->ChBox_AS_Manga->setChecked( false );
    ui->ChBox_AS_Amv->setChecked( false );
    ui->ChBox_AS_Dorama->setChecked( false );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( false );
    ui->ChBox_OptField_Anime_Director->setChecked( false );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( false );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( false );
    ui->ChBox_OptField_Manga_Author->setChecked( false );
    ui->ChBox_OptField_Manga_Translation->setChecked( false );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( false );
    ui->ChBox_OptField_Dorama_Director->setChecked( false );

    ui->ComboBox_Language->setCurrentIndex(0);
    ui->ComboBox_ItemList_Sorting->setCurrentIndex(1);

    ui->ChBox_CheckForUpdate->setChecked( true );
    ui->ChBox_SwitchCoverOrDir->setChecked( true );
    ui->ChBox_SearchOnShikimori->setChecked( true );

    ui->LineEdit_WorkDir->setText( DefinesPath::appData(true) );
}

void Settings::on_PBtn_Action_Export_clicked()
{
    bool exAnime  = ui->ChBox_Export_Anime->isChecked();
    bool exManga  = ui->ChBox_Export_Manga->isChecked();
    bool exAmv    = ui->ChBox_Export_Amv->isChecked();
    bool exDorama = ui->ChBox_Export_Dorama->isChecked();
    bool exImages = ui->ChBox_Export_Images->isChecked();

    QString exportDir = ui->LineEdit_Export_FilePath->text();
            exportDir = QDir( exportDir ).path() + QDir::separator();

    if( exportDir.isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The directory for export isn't choose"));
        ui->LineEdit_Export_FilePath->setFocus();
        return;
    }

    if( QDir::isAbsolutePath( exportDir ) == false ){
        QMessageBox::warning( this, tr("Warning"), tr("The path shall be absolute") );
        return;
    }

    on_actionShowExportProgressBar_triggered(true);
    this->setDisabled( true );

    QDir().mkpath( exportDir );
    QFile file( exportDir + "DatabaseAnime.xml" );
    if( ! file.open( QIODevice::WriteOnly | QIODevice::Text ) ){
        return;
    }
    XmlDbaWriter writer(&file);
    /*
    QSqlQuery query;
    if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime)) ) ){
        qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
        QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
        return;
    }

    */
    writer.startDocument();
    writer.writeDTD();
    writer.startElement("DatabaseAnime");

    quint64 countAnime, countManga, countAmv, countDorama, allCount;
    countAnime  = ( exAnime  )? MngrQuerys::count(sections::anime)  : 0;
    countManga  = ( exManga  )? MngrQuerys::count(sections::manga)  : 0;
    countAmv    = ( exAmv    )? MngrQuerys::count(sections::amv)    : 0;
    countDorama = ( exDorama )? MngrQuerys::count(sections::dorama) : 0;
    allCount = countAnime + countManga + countAmv + countDorama;

    writer.writeAttribute("CountAnime"  , QString::number( countAnime  ) );
    writer.writeAttribute("CountManga"  , QString::number( countManga  ) );
    writer.writeAttribute("CountAmv"    , QString::number( countAmv    ) );
    writer.writeAttribute("CountDorama" , QString::number( countDorama ) );

    QSqlQuery query;

    ui->ProgressBar_Export->setMinimum(0);
    ui->ProgressBar_Export->setValue(0);
    ui->ProgressBar_Export->setMaximum(allCount);

    quint64 progress(0);
    if( exAnime ){
        query = MngrQuerys::selectAll(sections::anime);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::anime);
        while( query.next() ){
            using namespace Tables::Anime::Fields;
            QMap<QString, QString> data;

            data[isHaveLooked]   = query.value(isHaveLooked   ).toString();
            data[isEditingDone]  = query.value(isEditingDone  ).toString();
            data[isAdult]        = query.value(isAdult        ).toString();
            data[Title]          = query.value(Title          ).toString();
            data[AltTitle]       = query.value(AltTitle       ).toString();
            data[Director]       = query.value(Director       ).toString();
            data[PostScoring]    = query.value(PostScoring    ).toString();
            data[SeriesTV]       = query.value(SeriesTV       ).toString();
            data[SeriesOVA]      = query.value(SeriesOVA      ).toString();
            data[SeriesONA]      = query.value(SeriesONA      ).toString();
            data[SeriesSpecial]  = query.value(SeriesSpecial  ).toString();
            data[SeriesMovie]    = query.value(SeriesMovie    ).toString();
            data[vSeriesTV]      = query.value(vSeriesTV      ).toString();
            data[vSeriesOVA]     = query.value(vSeriesOVA     ).toString();
            data[vSeriesONA]     = query.value(vSeriesONA     ).toString();
            data[vSeriesSpecial] = query.value(vSeriesSpecial ).toString();
            data[vSeriesMovie]   = query.value(vSeriesMovie   ).toString();
            data[Score]          = query.value(Score          ).toString();
            data[Year]           = query.value(Year           ).toString();
            data[Season]         = query.value(Season         ).toString();
            data[Studios]        = query.value(Studios        ).toString();
            data[Tags]           = query.value(Tags           ).toString();
            data[Description]    = query.value(Description    ).toString();
            data[Url]            = query.value(Url            ).toString();
            data[Dir]            = query.value(Dir            ).toString();
            data[ImagePath]      = query.value(ImagePath      ).toString();

            writer.writeNext(data);
            ui->ProgressBar_Export->setValue( progress++ );
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Anime*/);
    }
    if( exManga ){
        query = MngrQuerys::selectAll(sections::manga);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::manga);
        while (query.next()) {
            using namespace Tables::Manga::Fields;
            QMap<QString, QString> data;

            data[isHaveLooked]    = query.value(isHaveLooked    ).toString();
            data[isEditingDone]   = query.value(isEditingDone   ).toString();
            data[isAdult]         = query.value(isAdult         ).toString();
            data[Title]           = query.value(Title           ).toString();
            data[AltTitle]        = query.value(AltTitle        ).toString();
            data[Author]          = query.value(Author          ).toString();
            data[Translation]     = query.value(Translation     ).toString();
            data[Vol]             = query.value(Vol             ).toString();
            data[Ch]              = query.value(Ch              ).toString();
            data[Pages]           = query.value(Pages           ).toString();
            data[vVol]            = query.value(vVol            ).toString();
            data[vCh]             = query.value(vCh             ).toString();
            data[vPages]          = query.value(vPages          ).toString();
            data[Score]           = query.value(Score           ).toString();
            data[Year]            = query.value(Year            ).toString();
            data[Tags]            = query.value(Tags            ).toString();
            data[Description]     = query.value(Description     ).toString();
            data[Url]             = query.value(Url             ).toString();
            data[Dir]             = query.value(Dir             ).toString();
            data[ImagePath]       = query.value(ImagePath       ).toString();

            writer.writeNext(data);
            ui->ProgressBar_Export->setValue( progress++ );
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Manga*/);
    }
    if( exAmv ){
        query = MngrQuerys::selectAll(sections::amv);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::amv);
        while (query.next()) {
            using namespace Tables::Amv::Fields;
            QMap<QString, QString> data;

            data[isEditingDone]   = query.value(isEditingDone  ).toString();
            data[isAdult]         = query.value(isAdult        ).toString();
            data[Title]           = query.value(Title          ).toString();
            data[Author]          = query.value(Author         ).toString();
            data[Contestant]      = query.value(Contestant     ).toString();
            data[Score]           = query.value(Score          ).toString();
            data[Year]            = query.value(Year           ).toString();
            data[Tags]            = query.value(Tags           ).toString();
            data[ContainingMusic] = query.value(ContainingMusic).toString();
            data[ContainingAnime] = query.value(ContainingAnime).toString();
            data[AuthorComment]   = query.value(AuthorComment  ).toString();
            data[Url]             = query.value(Url            ).toString();
            data[Dir]             = query.value(Dir            ).toString();
            data[ImagePath]       = query.value(ImagePath      ).toString();

            writer.writeNext(data);
            ui->ProgressBar_Export->setValue( progress++ );
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Amv*/);
    }
    if( exDorama ){
        query = MngrQuerys::selectAll(sections::dorama);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::dorama);
        while (query.next()) {
            using namespace Tables::Dorama::Fields;
            QMap<QString, QString> data;

            data[isHaveLooked]    = query.value(isHaveLooked   ).toString();
            data[isEditingDone]   = query.value(isEditingDone  ).toString();
            data[isAdult]         = query.value(isAdult        ).toString();
            data[Title]           = query.value(Title          ).toString();
            data[AltTitle]        = query.value(AltTitle       ).toString();
            data[Director]        = query.value(Director       ).toString();
            data[SeriesTV]        = query.value(SeriesTV       ).toString();
            data[SeriesSpecial]   = query.value(SeriesSpecial  ).toString();
            data[SeriesMovie]     = query.value(SeriesMovie    ).toString();
            data[vSeriesTV]       = query.value(vSeriesTV      ).toString();
            data[vSeriesSpecial]  = query.value(vSeriesSpecial ).toString();
            data[vSeriesMovie]    = query.value(vSeriesMovie   ).toString();
            data[Score]           = query.value(Score          ).toString();
            data[Year]            = query.value(Year           ).toString();
            data[Season]          = query.value(Season         ).toString();
            data[Tags]            = query.value(Tags           ).toString();
            data[Description]     = query.value(Description    ).toString();
            data[Actors]          = query.value(Actors         ).toString();
            data[Url]             = query.value(Url            ).toString();
            data[Dir]             = query.value(Dir            ).toString();
            data[ImagePath]       = query.value(ImagePath      ).toString();

            writer.writeNext(data);
            ui->ProgressBar_Export->setValue( progress++ );
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Dorama*/);
    }
    writer.endElement(/*DatabaseAnime*/);

    file.close();

    if( exAnime  && exImages ){
        QDirIterator it( DefinesPath::animeCovers() );
        QString animeCoversExportPath( DefinesPath::animeCovers( exportDir ) );
        QDir().mkpath( animeCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( animeCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exManga  && exImages ){
        QDirIterator it( DefinesPath::mangaCovers() );
        QString mangaCoversExportPath( DefinesPath::mangaCovers( exportDir ) );
        QDir().mkpath( mangaCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( mangaCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exAmv && exImages ){
        QDirIterator it( DefinesPath::amvCovers() );
        QString amvCoversExportPath( DefinesPath::amvCovers( exportDir ) );
        QDir().mkpath( amvCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( amvCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exDorama && exImages ){
        QDirIterator it( DefinesPath::doramaCovers() );
        QString doramaCoversExportPath( DefinesPath::doramaCovers( exportDir ) );
        QDir().mkpath( doramaCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( doramaCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }

    this->setEnabled( true );
    on_actionShowExportProgressBar_triggered(false);
    QMessageBox::information( this, tr("Export"), tr("Export is successfully finished") );
}


void Settings::on_TBtn_Export_Path_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for export file"),
                                                    ui->LineEdit_Export_FilePath->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_Export_FilePath->setText( DefinesPath::home() );
    else
        ui->LineEdit_Export_FilePath->setText( dir );
}

void Settings::on_TBtn_Import_Path_Choose_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Choose a xml file for import"),
                                                ui->LineEdit_Import_FilePath->text()
                                                );
    if( file.isEmpty() )
        ui->LineEdit_Import_FilePath->setText( DefinesPath::home() );
    else
        ui->LineEdit_Import_FilePath->setText( file );
}

void Settings::on_PBtn_Import_Append_clicked()
{
    if( ui->LineEdit_Import_FilePath->text().isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The imported file isn't selected"));
        ui->LineEdit_Import_FilePath->setFocus();
        this->setEnabled( true );
        return;
    }

    this->setCursor( QCursor(Qt::WaitCursor) );
    on_actionShowImportProgressBar_triggered(true);
    this->setDisabled(true);

    int countImportRecords = import();
    if( countImportRecords > 0 ){
        QMessageBox::information(this, tr("Import"),"<b>" + tr("Import is successfully finished") + "</b><br>"
                                                    + tr("Records it is imported:")+ " " + QString::number(countImportRecords) + "   "
                                                        );
    }else{
        QMessageBox::critical(this, tr("Import"),"<b>" + tr("Import failed") + "</b>");
    }

    this->setEnabled(true);
    on_actionShowImportProgressBar_triggered(false);
    this->setCursor( QCursor(Qt::ArrowCursor) );
}

void Settings::on_PBtn_Import_Replace_clicked()
{
    QMessageBox* pmbx =
    new QMessageBox(QMessageBox::Question,
        tr("Warning"),
        tr("<b>Delete</b> your database?<br>"
           "<i>Fields marked by a checkbox will be deleted</i>"),
        QMessageBox::Yes | QMessageBox::No
    );
    int n = pmbx->exec();
    delete pmbx;

    this->setCursor( QCursor(Qt::WaitCursor) );
    on_actionShowImportProgressBar_triggered(true);
    this->setDisabled(true);

    if (n == QMessageBox::Yes) {
        deleteRecords();
        on_PBtn_Import_Append_clicked();
    }

    this->setCursor( QCursor(Qt::ArrowCursor) );
    on_actionShowImportProgressBar_triggered(false);
    this->setEnabled(true);
}

quint64 Settings::import()
{
    bool imAnime  = ui->ChBox_Import_Anime->isChecked();
    bool imManga  = ui->ChBox_Import_Manga->isChecked();
    bool imAmv    = ui->ChBox_Import_Amv->isChecked();
    bool imDorama = ui->ChBox_Import_Dorama->isChecked();
    bool imImages = ui->ChBox_Import_Images->isChecked();
    QString filePath = ui->LineEdit_Import_FilePath->text();

    QFile file( QDir(filePath).path() );
    if( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qCritical() << file.errorString()
                    << "\nFileName: " << file.fileName()
                    << "\nFileError: " << file.error();
        QMessageBox::critical(this, tr("Critical"), tr("File is not open"));
        return 0;
    }

    XmlDbaReader reader(&file);
    quint64 countAnime, countManga, countAmv, countDorama, allCount;
    reader.readHeader(countAnime, countManga, countAmv, countDorama);
    countAnime  = ( imAnime  )? countAnime  : 0;
    countManga  = ( imManga  )? countManga  : 0;
    countAmv    = ( imAmv    )? countAmv    : 0;
    countDorama = ( imDorama )? countDorama : 0;
    allCount = countAnime + countManga + countAmv + countDorama;

    QMap<QString, QVariant> data;

    MngrConnect.transaction();
    quint64 progress(0);
    ui->ProgressBar_Import->setMinimum(0);
    ui->ProgressBar_Import->setValue(0);
    ui->ProgressBar_Import->setMaximum(allCount);
    ui->ProgressBar_Import->setFormat( tr("Import of records") + " %p%" );

    while( ! reader.atEnd() && ! reader.hasError() ){
        data = reader.readNext();
        if( data.isEmpty() )
            continue;

        if( reader.currentSection() == sections::anime
            && imAnime == false)
            continue;
        if( reader.currentSection() == sections::manga
            && imManga == false)
            continue;
        if( reader.currentSection() == sections::amv
            && imAmv == false)
            continue;
        if( reader.currentSection() == sections::dorama
            && imDorama == false)
            continue;

        switch ( reader.currentSection() ) {
        case sections::anime :
            MngrQuerys::insertAnime( data );
            progress++;
            break;
        case sections::manga :
            MngrQuerys::insertManga( data );
            progress++;
            break;
        case sections::amv :
            MngrQuerys::insertAmv( data );
            progress++;
            break;
        case sections::dorama :
            MngrQuerys::insertDorama( data );
            progress++;
            break;
        default:
            qCritical() << "[FormSettings::importAppend] uncorrect section: " << reader.currentSection();
        }
        ui->ProgressBar_Import->setValue(progress);
        QCoreApplication::processEvents();
    }
    if( reader.hasError() )
        MngrConnect.rollback();
    else
        MngrConnect.commit();

    file.close();

    ui->ProgressBar_Import->setFormat( tr("Copying of covers")/* + " %p%" */);
    QString importPath( QFileInfo( filePath ).path() );

    if( imAnime  && imImages )
        copyFolder( DefinesPath::animeCovers( importPath ), DefinesPath::animeCovers() );
    if( imManga  && imImages )
        copyFolder( DefinesPath::mangaCovers( importPath ), DefinesPath::mangaCovers() );
    if( imAmv  && imImages )
        copyFolder( DefinesPath::amvCovers( importPath ), DefinesPath::amvCovers() );
    if( imDorama && imImages )
        copyFolder( DefinesPath::doramaCovers( importPath ), DefinesPath::doramaCovers() );

    return progress;
}

quint64 Settings::removeFilesFromFolder(QString folder)
{
    QDirIterator it( folder );
    quint64 n(0);
    while( it.hasNext() ){
        it.next();
        if( it.fileName() == "." || it.fileName() == ".." )
            continue;
        QFile( it.filePath() ).remove();
        ++n;
        QCoreApplication::processEvents();
    }
    return n;
}

quint64 Settings::copyFolder(QString folder1, QString folder2)
{
    QDirIterator it( folder1 );
    QDir().mkpath( folder2 );
    quint64 n(0);
    while( it.hasNext() ){
        it.next();
        if( it.fileName() == "." || it.fileName() == ".." )
            continue;
        QFile( it.filePath() ).copy( DefinesPath::animeCovers() + it.fileName() );
        ++n;
        QCoreApplication::processEvents();
    }
    return n;
}

bool Settings::deleteRecords()
{
    bool imAnime  = ui->ChBox_Import_Anime->isChecked();
    bool imManga  = ui->ChBox_Import_Manga->isChecked();
    bool imAmv    = ui->ChBox_Import_Amv->isChecked();
    bool imDorama = ui->ChBox_Import_Dorama->isChecked();
    bool imImages = ui->ChBox_Import_Images->isChecked();

    MngrConnect.transaction();
    if( imAnime ){
        if( MngrQuerys::dropTable( sections::anime ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imManga ){
        if( MngrQuerys::dropTable( sections::manga ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imAmv ){
        if( MngrQuerys::dropTable( sections::amv ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imDorama ){
        if( MngrQuerys::dropTable( sections::dorama ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }

    if( imImages ){
        if( imAnime )
            removeFilesFromFolder( DefinesPath::animeCovers() );
        if( imManga )
            removeFilesFromFolder( DefinesPath::mangaCovers() );
        if( imAmv )
            removeFilesFromFolder( DefinesPath::amvCovers() );
        if( imDorama )
            removeFilesFromFolder( DefinesPath::doramaCovers() );
    }

    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();
    MngrQuerys::createTable_Amv();
    MngrQuerys::createTable_Dorama();
    MngrConnect.commit();
    return true;
}

void Settings::on_TBtn_WorkDir_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for application data"),
                                                    ui->LineEdit_WorkDir->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_WorkDir->setText( DefinesPath::appData() );
    else
        ui->LineEdit_WorkDir->setText( dir );
}

void Settings::on_actionShowImportProgressBar_triggered(bool checked)
{
    ui->PBtn_Import_Append->setVisible(!checked);
    ui->PBtn_Import_Replace->setVisible(!checked);
    ui->ProgressBar_Import->setVisible(checked);
}

void Settings::on_actionShowExportProgressBar_triggered(bool checked)
{
    ui->PBtn_Action_Export->setVisible(!checked);
    ui->ProgressBar_Export->setVisible(checked);
}
