#include "settings.h"
#include "stylemanager.h"
#include "ui_settings.h"

#include <QColorDialog>
#include <QInputDialog>
#include <QStyleFactory>

void Settings::connectColorPicker()
{
    connect( ui->Frame_Style_Window, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_WindowText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Base, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_AlternateBase, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ToolTipBase, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ToolTipText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Text, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Button, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ButtonText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_BrightText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Link, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Highlight, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_HighlightedText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
}

Settings::Settings(MngrConnection &MngrCon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    app(*static_cast<DbaApplication*>(qApp)),
    MngrConnect(MngrCon)
{
    ui->setupUi(this);
    ui->ProgressBar_Export->setVisible(false);
    ui->ProgressBar_Import->setVisible(false);

    QSettings &cfg = app.settings();
    this->restoreGeometry( cfg.value(Options::Dialogs::Config::Geometry).toByteArray() );
    ui->splitter->restoreGeometry( cfg.value(Options::Dialogs::Config::Splitter::Geometry).toByteArray() );
    ui->splitter->restoreState( cfg.value(Options::Dialogs::Config::Splitter::State).toByteArray() );
    ui->LineEdit_Export_FilePath->setText( DefinesPath::home() );
    ui->LineEdit_Import_FilePath->setText( DefinesPath::home() );

    // Work dir
    ui->LineEdit_WorkDir->setText( QDir::toNativeSeparators( DefinesPath::appData() ) );

    loadSettings();
}

Settings::~Settings()
{
    using namespace Options::Dialogs::Config;
    QSettings &cfg = app.settings();
    cfg.setValue(Geometry, this->saveGeometry() );

    cfg.setValue(Splitter::Geometry, ui->splitter->saveGeometry() );
    cfg.setValue(Splitter::State,    ui->splitter->saveState() );
    delete ui;
}

/*! \~russian
 * \brief Метод - возвращает выбранный пользователем способ сортировки
 * \return перечисление Sort описанное в файле globalenum.h
 *
 * Метод предназначен для получения выбранного пользователем способа
 * сортировки содержимого разделов.
 */
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

void Settings::colorPicked(QColor)
{
    stylePalette = paletteFromColorPicker();
    setApplicationStyle( stylePalette );
    ui->PButton_Style_SaveChanges->setVisible( true );
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
    saveSettings();
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

    const int SYSTEM_LANGUAGE = 0;
    ui->ComboBox_Language->setCurrentIndex( SYSTEM_LANGUAGE );
    ui->ComboBox_ItemList_Sorting->setCurrentIndex( Sort::asc );


    ui->ChBox_SwitchCoverOrDir->setChecked( true );

    ui->ChBox_CheckForUpdate->setChecked( true );
    ui->ChBox_SearchOnShikimori->setChecked( true );
    ui->ChBox_DownloadCovers->setChecked( true );
    ui->ComboBox_SearchOutput->setCurrentIndex( SearchOutput::MIX );
    ui->SpinBox_SearchLimit->setValue( 10 );

    ui->LineEdit_WorkDir->setText( DefinesPath::appData(true) );

    /// \todo Add reset other options here
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

/*! \~russian
 * \brief Метод инициализирует элементы настройки цвета в соответствии с переданной палитрой.
 * \param palette - палитра для инициализации
 */
void Settings::initColorPickers(QPalette palette)
{
    ui->Frame_Style_Window->setColor( palette.window().color() );
    ui->Frame_Style_WindowText->setColor( palette.windowText().color() );
    ui->Frame_Style_Base->setColor( palette.base().color() );
    ui->Frame_Style_AlternateBase->setColor( palette.alternateBase().color() );
    ui->Frame_Style_ToolTipBase->setColor( palette.toolTipBase().color() );
    ui->Frame_Style_ToolTipText->setColor( palette.toolTipText().color() );
    ui->Frame_Style_Text->setColor( palette.text().color() );
    ui->Frame_Style_Button->setColor( palette.button().color() );
    ui->Frame_Style_ButtonText->setColor( palette.buttonText().color() );
    ui->Frame_Style_BrightText->setColor( palette.brightText().color() );
    ui->Frame_Style_Link->setColor( palette.link().color() );
    ui->Frame_Style_Highlight->setColor( palette.highlight().color() );
    ui->Frame_Style_HighlightedText->setColor( palette.highlightedText().color() );
}

/*! \~russian
 * \brief Метод для получения палитры на основе ColorPicker'а
 * \return палитра с набором цветов из ColorPicker
 */
QPalette Settings::paletteFromColorPicker()
{
    QPalette palette;

    palette.setColor(QPalette::Window, ui->Frame_Style_Window->color() );
    palette.setColor(QPalette::WindowText, ui->Frame_Style_WindowText->color() );
    palette.setColor(QPalette::Base, ui->Frame_Style_Base->color());
    palette.setColor(QPalette::AlternateBase, ui->Frame_Style_AlternateBase->color());
    palette.setColor(QPalette::ToolTipBase, ui->Frame_Style_ToolTipBase->color());
    palette.setColor(QPalette::ToolTipText, ui->Frame_Style_ToolTipText->color());
    palette.setColor(QPalette::Text, ui->Frame_Style_Text->color());
    palette.setColor(QPalette::Button, ui->Frame_Style_Button->color());
    palette.setColor(QPalette::ButtonText, ui->Frame_Style_ButtonText->color());
    palette.setColor(QPalette::BrightText, ui->Frame_Style_BrightText->color());
    palette.setColor(QPalette::Link, ui->Frame_Style_Link->color());
    palette.setColor(QPalette::Highlight, ui->Frame_Style_Highlight->color());
    palette.setColor(QPalette::HighlightedText, ui->Frame_Style_HighlightedText->color());

    return palette;
}

/*! \~russian
 * \brief Метод проверки дублирования имени стиля
 * \param name - имя для проверки
 * \return true в случае если параметр совпадает с одним из пунктов в выпадающем списке стилей.
 *
 * Метод проверяет имеется ли переданное имя в списке стилей и возвращает истину в случае если
 * указанное имя уже имеется.
 */
bool Settings::duplicateStyleName(QString name)
{
    const int dontFind = -1;
    if( ui->ComboBox_CurrentStyle->findText(name) == dontFind ){
        return false;
    }
    return true;
}

void Settings::setApplicationStyle(QPalette palette)
{
    qApp->setStyle( QStyleFactory::create("Fusion") );
    qApp->setPalette( palette );
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

void Settings::loadSettings()
{
    QSettings cfg;
    {
        using namespace Options::ActiveSections;
        ui->ChBox_AS_Anime ->setChecked( cfg.value( ANIME , true  ).toBool() );
        ui->ChBox_AS_Manga ->setChecked( cfg.value( MANGA , false ).toBool() );
        ui->ChBox_AS_Amv   ->setChecked( cfg.value( AMV   , false ).toBool() );
        ui->ChBox_AS_Dorama->setChecked( cfg.value( DORAMA, false ).toBool() );
    }
    {
        using namespace Options::OptionalFields;

        ui->ChBox_OptField_Anime_AltTitle   ->setChecked( cfg.value( Anime::AltTitle   , false ).toBool() );
        ui->ChBox_OptField_Anime_Director   ->setChecked( cfg.value( Anime::Director   , false ).toBool() );
        ui->ChBox_OptField_Anime_Postscoring->setChecked( cfg.value( Anime::Postscoring, false ).toBool() );

        ui->ChBox_OptField_Manga_AltTitle   ->setChecked( cfg.value( Manga::AltTitle  , false ).toBool() );
        ui->ChBox_OptField_Manga_Author     ->setChecked( cfg.value( Manga::Author    , false ).toBool() );
        ui->ChBox_OptField_Manga_Translation->setChecked( cfg.value( Manga::Translator, false ).toBool() );

        ui->ChBox_OptField_Dorama_AltTitle->setChecked( cfg.value( Dorama::AltTitle, false ).toBool() );
        ui->ChBox_OptField_Dorama_Director->setChecked( cfg.value( Dorama::Director, false ).toBool() );
    }
    {
        using namespace Options::Network;

        ui->ChBox_CheckForUpdate   ->setChecked( cfg.value( CHECK_UPDATES, true ).toBool() );
        ui->ChBox_SearchOnShikimori->setChecked( cfg.value( LIVE_SEARCH  , true ).toBool() );
        /// \todo DefaultValues
        {
            ui->ComboBox_SearchOutput->addItem( tr("English"), SearchOutput::ENG );
            ui->ComboBox_SearchOutput->addItem( tr("Russian"), SearchOutput::RUS );
            ui->ComboBox_SearchOutput->addItem( tr("Mixed")  , SearchOutput::MIX );
        }
        ui->SpinBox_SearchLimit->setValue( cfg.value( SEARCH_LIMIT, 10 ).toInt() );
        ui->ComboBox_SearchOutput->setCurrentIndex( cfg.value( SEARCH_OUTPUT, SearchOutput::MIX ).toInt() );
        ui->ChBox_DownloadCovers->setChecked( cfg.value( RELOAD_COVERS, true ).toBool() );
    }
    {
        using namespace Options::General;

        ui->ChBox_SwitchCoverOrDir->setChecked(  cfg.value( SwitchCoverOrDir, true ).toBool() );

        QLocale locale = cfg.value( LOCALE, QLocale::system() ).toLocale();

        ui->ComboBox_Language->addItem( tr("System"), QLocale::system().language() );

        auto langMap = DbaLocalization::readExistsLocalizations( DefinesPath::share() );
        for( auto it = langMap.begin(); it != langMap.end(); ++it ){
            ui->ComboBox_Language->addItem( it.value(), it.key() );

            if( locale.language() == it.key() )
                ui->ComboBox_Language->setCurrentText( it.value() );
        }
    }
    {  // Sorting
        using namespace Options::General;
        ui->ComboBox_ItemList_Sorting->addItem( tr("None"), Sort::none);
        ui->ComboBox_ItemList_Sorting->addItem( tr("ASC"),  Sort::asc );
        ui->ComboBox_ItemList_Sorting->addItem( tr("DESC"), Sort::desc);
        ui->ComboBox_ItemList_Sorting->addItem( tr("Year"), Sort::year);

        auto sort = cfg.value( SORTING, Sort::asc ).toInt();
        ui->ComboBox_ItemList_Sorting->setCurrentIndex( sort );
    }
    {  // Displayed field
        using namespace Tables::UniformField;
        using namespace Options::General;

        ui->ComboBox_ItemList_DisplayedField->addItem( tr("Title")            , TITLE    );
        ui->ComboBox_ItemList_DisplayedField->addItem( tr("Alternative title"), ALT_TITLE);

        int cfgField = cfg.value( DISPLAYED_FIELD, TITLE ).toInt();
        auto displayedField = static_cast<Tables::UniformField::field>( cfgField );

        enum IndexInComboBox{ TITLE_INDEX, ALT_TITLE_INDEX };

        int indexDisplayedField = ( displayedField == TITLE )
                                  ? TITLE_INDEX
                                  : ALT_TITLE_INDEX;

        ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( indexDisplayedField );
    }
    {  // Style
        using namespace Options::Style;

        QString styleName     = cfg.value( CurrentStyleName, "System" ).toString();
        bool    isSystemStyle = cfg.value( SystemStyle     , true     ).toBool();

        QSet<QString> styles = StyleManager::getExistsStyles();
        ui->ComboBox_CurrentStyle->addItems( styles.toList() );

        if( ( ! isSystemStyle) && styles.contains( styleName ) ){
            ui->ComboBox_CurrentStyle->setCurrentIndex( ui->ComboBox_CurrentStyle->findText( styleName ) );
            stylePalette = StyleManager::getPaletteOfStyle( styleName );
        }else{
            ui->ComboBox_CurrentStyle->setCurrentIndex( INDEX_OF_SYSTEM_STYLE );
        }

        initColorPickers( stylePalette );
        connectColorPicker();
        ui->GroupBox_Style_Colors->setEnabled( ! isSystemStyle );

        ui->PButton_Style_SaveChanges->setVisible( false );
    }
}

void Settings::saveSettings()
{
    QSettings &cfg = app.settings();
    {
        using namespace Options::ActiveSections;
        cfg.setValue( ANIME,  ui->ChBox_AS_Anime->isChecked() );
        cfg.setValue( MANGA,  ui->ChBox_AS_Manga->isChecked() );
        cfg.setValue( AMV,    ui->ChBox_AS_Amv->isChecked() );
        cfg.setValue( DORAMA, ui->ChBox_AS_Dorama->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Anime;
        cfg.setValue( AltTitle,    ui->ChBox_OptField_Anime_AltTitle->isChecked() );
        cfg.setValue( Director,    ui->ChBox_OptField_Anime_Director->isChecked() );
        cfg.setValue( Postscoring, ui->ChBox_OptField_Anime_Postscoring->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Manga;
        cfg.setValue( AltTitle,   ui->ChBox_OptField_Manga_AltTitle->isChecked() );
        cfg.setValue( Author,     ui->ChBox_OptField_Manga_Author->isChecked() );
        cfg.setValue( Translator, ui->ChBox_OptField_Manga_Translation->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Dorama;
        cfg.setValue( AltTitle, ui->ChBox_OptField_Dorama_AltTitle->isChecked() );
        cfg.setValue( Director, ui->ChBox_OptField_Dorama_Director->isChecked() );
    }
    {
        using namespace Options::General;
        cfg.setValue( LOCALE, QLocale( getLanguage() ) );
        cfg.setValue( SORTING,  getSort() );
    }
    {
        using namespace Options::Network;
        cfg.setValue( CHECK_UPDATES, ui->ChBox_CheckForUpdate->isChecked() );
        cfg.setValue( LIVE_SEARCH  , ui->ChBox_SearchOnShikimori->isChecked() );
        cfg.setValue( SEARCH_OUTPUT, ui->ComboBox_SearchOutput->currentData().toInt() );
        cfg.setValue( SEARCH_LIMIT , ui->SpinBox_SearchLimit->value() );
        cfg.setValue( RELOAD_COVERS, ui->ChBox_DownloadCovers->isChecked() );
    }
    { // Style
        using namespace Options::Style;
        bool isSystemStyle = ui->ComboBox_CurrentStyle->currentIndex() == INDEX_OF_SYSTEM_STYLE;
        QString styleName  = ui->ComboBox_CurrentStyle->currentText();

        cfg.setValue( SystemStyle, isSystemStyle );
        cfg.setValue( CurrentStyleName, styleName );

        if( ! isSystemStyle )
            StyleManager::saveStyle( styleName, paletteFromColorPicker() );
    }

    cfg.setValue( Options::General::SwitchCoverOrDir, ui->ChBox_SwitchCoverOrDir->isChecked() );

    if( QDir::isAbsolutePath( ui->LineEdit_WorkDir->text() ) )
        cfg.setValue( Options::General::WorkDirectory, QDir( ui->LineEdit_WorkDir->text() ).path() );
    else
        cfg.remove(Options::General::WorkDirectory);

    { // Displayed field
        int displayedFieldId = ui->ComboBox_ItemList_DisplayedField->currentData().toInt();
        Tables::UniformField::field displayedField
                = static_cast<Tables::UniformField::field>( displayedFieldId );
        cfg.setValue( Options::General::DISPLAYED_FIELD, displayedField );
    }
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

/*! \~russian
 * \brief Метод - нажатия на кнопку выбора рабочей директории.
 *
 * Метод отображает стандартный диалог выбора директории,
 * позволяя пользователю выбрать место в файловой системе,
 * где будут храниться файлы приложения.
 *
 * При отображении диалогового окна, инстанция задаётся в соответствии
 * с текущим значением поля ввода рабочей директории.
 *
 * При нажатии кнопки отмены, не изменяет текущее значение поля ввода
 * рабочей директории.
 */
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

/*! \~russian
 * \brief Метод - отображения/скрытия прогресс бара импорта
 * \param checked - отобразить или скрыть прогресс бар
 *
 * Метод при аттрибуте cheched равном true - скрывает кнопки действий
 * и отображает прогресс бар процесса импорта
 */


/*!
 * \brief Settings::on_actionShowImportProgressBar_triggered
 * \param checked
 *
 * test
 */
void Settings::on_actionShowImportProgressBar_triggered(bool checked)
{
    ui->PBtn_Import_Append->setVisible(!checked);
    ui->PBtn_Import_Replace->setVisible(!checked);
    ui->ProgressBar_Import->setVisible(checked);
}
/*! \~russian
 * \brief Метод - отображения/скрытия прогресс бара экспорта
 * \param checked - отобразить или скрыть прогресс бар
 *
 * Метод при аттрибуте cheched равном true - скрывает кнопки действий
 * и отображает прогресс бар процесса экспорта
 */
void Settings::on_actionShowExportProgressBar_triggered(bool checked)
{
    ui->PBtn_Action_Export->setVisible(!checked);
    ui->ProgressBar_Export->setVisible(checked);
}

/*! \~russian
 * \brief Метод - изменение текущего активного стиля в выпадающем списке.
 * \param index - текущий выбранный индекс
 *
 * Если выбранный стиль является системным(INDEX_OF_SYSTEM_STYLE)
 * блокирует возможность изменения цветовой палитры.
 *
 * Иначе - разблокирует возможность изменения цветовой палитры.
 *
 * \todo Загружает файл в соответствии с выбранным стилем и
 * инициализирует цветовую палитру соответствующими цветами.
 */
void Settings::on_ComboBox_CurrentStyle_currentIndexChanged(int index)
{
    ui->GroupBox_Style_Colors->setEnabled( index );
    ui->PButton_Style_SaveChanges->setVisible( false );
    if( index != INDEX_OF_SYSTEM_STYLE ){
        stylePalette = StyleManager::getPaletteOfStyle( ui->ComboBox_CurrentStyle->currentText() );
        initColorPickers( stylePalette );
        setApplicationStyle( stylePalette );
    }
}

/*! \~russian
 * \brief Метод - клик на кнопку удаления выбранного стиля.
 *
 * Если выбранный стиль - системный (INDEX_OF_SYSTEM_STYLE)
 * Отображает сообщение пользователю о невозможности его удаления и
 * завершает выполнение функции.
 *
 * Иначе - удаляет выбранный пункт из выпадаюзего списка.
 * Запрашивает подтверждение у пользователя.
 * Удаляет файл с таким именем, содержащий этот стиль.
 */
void Settings::on_TButton_RemoveStyle_clicked()
{
    if( ui->ComboBox_CurrentStyle->currentIndex() == INDEX_OF_SYSTEM_STYLE ){
        QMessageBox::information(this, tr("Warning"), tr("It is impossible to delete system style"));
        return;
    }

    QMessageBox::StandardButton pressedButton
            = QMessageBox::question(this, tr("Question"),
                                    tr("You really want to remove the current style?"));

    if( pressedButton == QMessageBox::Yes ){
        bool successRemoved = StyleManager::removeStyle( ui->ComboBox_CurrentStyle->currentText() );

        if( ! successRemoved ){
            /// \todo Set here the normal text
            QMessageBox::information(this, tr("Warning"), tr("Impossible to remove this style. Maybe your not accesss to removed."));
        }else{
            ui->ComboBox_CurrentStyle->removeItem( ui->ComboBox_CurrentStyle->currentIndex() );
        }
    }
}

/*! \~russian
 * \brief Метод - клик на кнопку копирования выбранного стиля
 *
 * Отображает диалог ввода имени для нового стиля,
 * проверяет валидность нового имени - не может быть пустым,
 * не может дублировать уже имеющееся имя, не может состоять из пробелов.
 *
 * Если не валидно, обображает соответствующее сообщение пользователю
 * и выходит из функции.
 *
 * Если валидно - добавляет новый пункт в выпадающий список стилей и делает его текущим.
 * Cоздаёт новый файл со стилем.
 */
void Settings::on_TButton_CopyStyle_clicked()
{
    bool ok = false;

    QString styleName = QInputDialog::getText(this, tr("Style copying"),
                                         tr("Enter name for new style:"), QLineEdit::Normal,
                                         ui->ComboBox_CurrentStyle->currentText(), &ok);

    styleName = styleName.trimmed();

    if( ! ok ){
        return;
    }
    if( styleName.isEmpty() ){
        QMessageBox::information(this, tr("Warning"), tr("The entered name is empty") );
        return;
    }
    if ( duplicateStyleName( styleName ) ){
        QMessageBox::information(this, tr("Warning"), tr("Style with such name already exists") );
        return;
    }

    StyleManager::saveStyle(styleName, paletteFromColorPicker() );

    ui->ComboBox_CurrentStyle->addItem(styleName);
    ui->ComboBox_CurrentStyle->setCurrentIndex( ui->ComboBox_CurrentStyle->findText(styleName) );
    ui->PButton_Style_SaveChanges->setVisible( false );
}

void Settings::on_PButton_Style_SaveChanges_clicked()
{
    QString styleName  = ui->ComboBox_CurrentStyle->currentText();
    int     styleIndex = ui->ComboBox_CurrentStyle->currentIndex();

    if( styleIndex != INDEX_OF_SYSTEM_STYLE )
        StyleManager::saveStyle( styleName, paletteFromColorPicker() );

    ui->PButton_Style_SaveChanges->setVisible( false );
}
