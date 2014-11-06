#include "settings.h"
#include "ui_settings.h"

Settings::Settings(MngrConnection &MngrCon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings), MngrConnect(MngrCon)
{
    ui->setupUi(this);
    QSettings settings;
    this->restoreGeometry( settings.value("WindowSettings/Geometry").toByteArray() );
    ui->LineEdit_Export_FilePath->setText( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
    ui->LineEdit_Import_FilePath->setText( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );

    settings.beginGroup("enableSection");
        bool b1 = settings.value( "Anime",   true ).toBool();
        bool b2 = settings.value( "Manga",  false ).toBool();
        bool b3 = settings.value( "AMV",    false ).toBool();
        bool b4 = settings.value( "Dorama", false ).toBool();
    settings.endGroup();

    settings.beginGroup("optionalField");
        settings.beginGroup("anime");
            bool a1 = settings.value( "OrigTitle",   false ).toBool();
            bool a2 = settings.value( "Director",    false ).toBool();
            bool a3 = settings.value( "PostScoring", false ).toBool();
        settings.endGroup();

        settings.beginGroup("manga");
            bool m1 = settings.value( "AltTitle",    false ).toBool();
            bool m2 = settings.value( "Author",      false ).toBool();
            bool m3 = settings.value( "Translation", false ).toBool();
        settings.endGroup();

        settings.beginGroup("dorama");
            bool d1 = settings.value( "AltTitle",   false ).toBool();
            bool d2 = settings.value( "Director",   false ).toBool();
        settings.endGroup();
    settings.endGroup();

    bool checkUpdates = settings.value("General/VerUpdate", false).toBool();

    bool c1 = settings.value( "SwitchCoverOrDir", true ).toBool();

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
    ui->ChBox_SwitchCoverOrDir->setChecked( c1 );
    ui->ChBox_SearchOnShikimori->setChecked( c1 ); // #ToDo: to settings

    QLocale::Language set_language = static_cast<QLocale::Language>(settings.value( "Language", QLocale::English ).toInt());

    ui->ComboBox_Language->addItem( tr("<System>"), 0 );

    QMap<QLocale::Language,QString> langList = DbaLocalization::readExistsLocalizations( DefinesPath::share() );
    int i(0);
    foreach (QString langName, langList) {
        ++i;
        ui->ComboBox_Language->addItem(langName, langList.key(langName));
        if( set_language == langList.key(langName) )
            ui->ComboBox_Language->setCurrentIndex(i);
    }


    Sort::sort sort = static_cast<Sort::sort>( settings.value( "Sorting", Sort::asc ).toInt() );
    ui->ComboBox_ItemList_Sorting->setCurrentIndex( sort );

    ui->LineEdit_WorkDir->setText( QDir::toNativeSeparators( DefinesPath::appData() ) );
}

Settings::~Settings()
{
    QSettings settings;
    settings.setValue("WindowSettings/Geometry", this->saveGeometry() );
    delete ui;
}

Sort::sort Settings::getSort()
{
    return static_cast<Sort::sort>( ui->ComboBox_ItemList_Sorting->currentIndex() );
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

    settings.beginGroup("enableSection");
        settings.setValue( "Anime",  ui->ChBox_AS_Anime->isChecked() );
        settings.setValue( "Manga",  ui->ChBox_AS_Manga->isChecked() );
        settings.setValue( "AMV",    ui->ChBox_AS_Amv->isChecked() );
        settings.setValue( "Dorama", ui->ChBox_AS_Dorama->isChecked() );
    settings.endGroup();

    settings.beginGroup("optionalField");
        settings.beginGroup("anime");
            settings.setValue( "OrigTitle",   ui->ChBox_OptField_Anime_AltTitle->isChecked() );
            settings.setValue( "Director",    ui->ChBox_OptField_Anime_Director->isChecked() );
            settings.setValue( "PostScoring", ui->ChBox_OptField_Anime_Postscoring->isChecked() );
        settings.endGroup();

        settings.beginGroup("manga");
            settings.setValue( "AltTitle",    ui->ChBox_OptField_Manga_AltTitle->isChecked() );
            settings.setValue( "Author",      ui->ChBox_OptField_Manga_Author->isChecked() );
            settings.setValue( "Translation", ui->ChBox_OptField_Manga_Translation->isChecked() );
        settings.endGroup();

        settings.beginGroup("dorama");
            settings.setValue( "AltTitle",   ui->ChBox_OptField_Dorama_AltTitle->isChecked() );
            settings.setValue( "Director",   ui->ChBox_OptField_Dorama_Director->isChecked() );
        settings.endGroup();
    settings.endGroup();

    settings.setValue( "Language", ui->ComboBox_Language->currentData() );
    settings.setValue( "Sorting", ui->ComboBox_ItemList_Sorting->currentIndex() );

    settings.setValue( "VerUpdate", ui->ChBox_CheckForUpdate->isChecked() );
    settings.setValue( "SwitchCoverOrDir", ui->ChBox_SwitchCoverOrDir->isChecked() );

    if( QDir::isAbsolutePath( ui->LineEdit_WorkDir->text() ) )
        settings.setValue( "WorkDirectory", QDir(ui->LineEdit_WorkDir->text()).path() );
    else
        settings.remove("WorkDirectory");
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

    ui->ChBox_CheckForUpdate->setChecked(false);
    ui->ChBox_SwitchCoverOrDir->setChecked( true ); ;

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

    this->setDisabled( true );

    QDir().mkpath( exportDir );
    QFile file( exportDir + "DatabaseAnime.xml" );
    if( ! file.open( QIODevice::WriteOnly | QIODevice::Text ) ){
        return;
    }
    XmlDbaWriter writer(&file);
    QSqlQuery query;
    if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime)) ) ){
        qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
        QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
        return;
    }

    writer.startDocument();
    writer.startElement("DatabaseAnime");

    if( exAnime ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime) ) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::anime);
        while( query.next() ){
            QMap<QString, QString> data;

            data["isHaveLooked"]    = query.value("isHaveLooked"    ).toString();
            data["isEditingDone"]   = query.value("isEditingDone"   ).toString();
            data["isAdult"]         = query.value("isAdult"         ).toString();
            data["Title"]           = query.value("Title"           ).toString();
            data["OrigTitle"]       = query.value("OrigTitle"       ).toString();
            data["Director"]        = query.value("Director"        ).toString();
            data["PostScoring"]     = query.value("PostScoring"     ).toString();
            data["SeriesTV"]        = query.value("SeriesTV"        ).toString();
            data["SeriesOVA"]       = query.value("SeriesOVA"       ).toString();
            data["SeriesONA"]       = query.value("SeriesONA"       ).toString();
            data["SeriesSpecial"]   = query.value("SeriesSpecial"   ).toString();
            data["SeriesMovie"]     = query.value("SeriesMovie"     ).toString();
            data["vSeriesTV"]       = query.value("vSeriesTV"       ).toString();
            data["vSeriesOVA"]      = query.value("vSeriesOVA"      ).toString();
            data["vSeriesONA"]      = query.value("vSeriesONA"      ).toString();
            data["vSeriesSpecial"]  = query.value("vSeriesSpecial"  ).toString();
            data["vSeriesMovie"]    = query.value("vSeriesMovie"    ).toString();
            data["Score"]           = query.value("Score"           ).toString();
            data["Year"]            = query.value("Year"            ).toString();
            data["Season"]          = query.value("Season"          ).toString();
            data["Studios"]         = query.value("Studios"         ).toString();
            data["Tags"]            = query.value("Tags"            ).toString();
            data["Description"]     = query.value("Description"     ).toString();
            data["URL"]             = query.value("URL"             ).toString();
            data["Dir"]             = query.value("Dir"             ).toString();
            data["ImagePath"]       = query.value("ImagePath"       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Anime*/);
    }
    if( exManga ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::manga)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::manga) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::manga);
        while (query.next()) {
            QMap<QString, QString> data;

            data["isHaveLooked"]    = query.value("isHaveLooked"    ).toString();
            data["isEditingDone"]   = query.value("isEditingDone"   ).toString();
            data["isAdult"]         = query.value("isAdult"         ).toString();
            data["Title"]           = query.value("Title"           ).toString();
            data["AltTitle"]        = query.value("AltTitle"        ).toString();
            data["Author"]          = query.value("Author"          ).toString();
            data["Translation"]     = query.value("Translation"     ).toString();
            data["Vol"]             = query.value("Vol"             ).toString();
            data["Ch"]              = query.value("Ch"              ).toString();
            data["Pages"]           = query.value("Pages"           ).toString();
            data["vVol"]            = query.value("vVol"            ).toString();
            data["vCh"]             = query.value("vCh"             ).toString();
            data["vPages"]          = query.value("vPages"          ).toString();
            data["Score"]           = query.value("Score"           ).toString();
            data["Year"]            = query.value("Year"            ).toString();
            data["Tags"]            = query.value("Tags"            ).toString();
            data["Description"]     = query.value("Description"     ).toString();
            data["URL"]             = query.value("URL"             ).toString();
            data["Dir"]             = query.value("Dir"             ).toString();
            data["ImagePath"]       = query.value("ImagePath"       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Manga*/);
    }
    if( exAmv ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::amv)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::amv) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::amv);
        while (query.next()) {
            QMap<QString, QString> data;

            data["isEditingDone"]   = query.value("isEditingDone"   ).toString();
            data["isAdult"]         = query.value("isAdult"         ).toString();
            data["Title"]           = query.value("Title"           ).toString();
            data["Author"]          = query.value("Author"          ).toString();
            data["Сontestant"]      = query.value("Сontestant"      ).toString();
            data["Score"]           = query.value("Score"           ).toString();
            data["Year"]            = query.value("Year"            ).toString();
            data["Tags"]            = query.value("Tags"            ).toString();
            data["ContainingMusic"] = query.value("ContainingMusic" ).toString();
            data["ContainingAnime"] = query.value("ContainingAnime" ).toString();
            data["AuthorComment"]   = query.value("AuthorComment"   ).toString();
            data["URL"]             = query.value("URL"             ).toString();
            data["Dir"]             = query.value("Dir"             ).toString();
            data["ImagePath"]       = query.value("ImagePath"       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Amv*/);
    }
    if( exDorama ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::dorama)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::dorama) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::dorama);
        while (query.next()) {
            QMap<QString, QString> data;

            data["isHaveLooked"]    = query.value("isHaveLooked"    ).toString();
            data["isEditingDone"]   = query.value("isEditingDone"   ).toString();
            data["isAdult"]         = query.value("isAdult"         ).toString();
            data["Title"]           = query.value("Title"           ).toString();
            data["AltTitle"]        = query.value("AltTitle"        ).toString();
            data["Director"]        = query.value("Director"        ).toString();
            data["SeriesTV"]        = query.value("SeriesTV"        ).toString();
            data["SeriesSpecial"]   = query.value("SeriesSpecial"   ).toString();
            data["SeriesMovie"]     = query.value("SeriesMovie"     ).toString();
            data["vSeriesTV"]       = query.value("vSeriesTV"       ).toString();
            data["vSeriesSpecial"]  = query.value("vSeriesSpecial"  ).toString();
            data["vSeriesMovie"]    = query.value("vSeriesMovie"    ).toString();
            data["Score"]           = query.value("Score"           ).toString();
            data["Year"]            = query.value("Year"            ).toString();
            data["Season"]          = query.value("Season"          ).toString();
            data["Tags"]            = query.value("Tags"            ).toString();
            data["Description"]     = query.value("Description"     ).toString();
            data["Actors"]          = query.value("Actors"          ).toString();
            data["URL"]             = query.value("URL"             ).toString();
            data["Dir"]             = query.value("Dir"             ).toString();
            data["ImagePath"]       = query.value("ImagePath"       ).toString();

            writer.writeNext(data);
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
    QMessageBox::information( this, tr("Export"), tr("Export is successfully finished") );
}


void Settings::on_TBtn_Export_Path_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for export file"),
                                                    ui->LineEdit_Export_FilePath->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_Export_FilePath->setText( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
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
        ui->LineEdit_Import_FilePath->setText( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
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
    this->setDisabled(true);

    int countImportRecords = on_actionImport_triggered();
    if( countImportRecords > 0 ){
        QMessageBox::information(this, tr("Import"),"<b>" + tr("Import is successfully finished") + "</b><br>"
                                                    + tr("Records it is imported:")+ " " + QString::number(countImportRecords) + "   "
                                                        );
    }else{
        QMessageBox::critical(this, tr("Import"),"<b>" + tr("Import failed") + "</b>");
    }

    this->setEnabled(true);
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
    this->setDisabled(true);

    if (n == QMessageBox::Yes) {
        on_actionDeleteRecords_triggered();
        on_PBtn_Import_Append_clicked();
    }

    this->setCursor( QCursor(Qt::ArrowCursor) );
    this->setEnabled(true);
}

unsigned long long Settings::on_actionImport_triggered()
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

    unsigned long long n(0);
    XmlDbaReader reader(&file);
    QMap<QString, QVariant> data;

    MngrConnect.transaction();
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
            MngrQuerys::insertAnime(data);
            break;
        case sections::manga :
            MngrQuerys::insertManga(data);
            break;
        case sections::amv :
            MngrQuerys::insertAmv(data);
            break;
        case sections::dorama :
            MngrQuerys::insertDorama(data);
            break;
        default:
            qCritical() << "[FormSettings::importAppend] uncorrect section: " << reader.currentSection();
        }
        n++;
        QCoreApplication::processEvents();
    }
    if( reader.hasError() )
        MngrConnect.rollback();
    else
        MngrConnect.commit();

    QString importPath( QFileInfo( filePath ).path() );

    if( imAnime  && imImages ){
        QDirIterator it( DefinesPath::animeCovers( importPath ) );
        QDir().mkpath( DefinesPath::animeCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::animeCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imManga  && imImages ){
        QDirIterator it( DefinesPath::mangaCovers( importPath ) );
        QDir().mkpath( DefinesPath::mangaCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::mangaCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imAmv  && imImages ){
        QDirIterator it( DefinesPath::amvCovers( importPath ) );
        QDir().mkpath( DefinesPath::amvCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::amvCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imDorama && imImages ){
        QDirIterator it( DefinesPath::doramaCovers( importPath ) );
        QDir().mkpath( DefinesPath::doramaCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::doramaCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }

    file.close();

    return n;
}

bool Settings::on_actionDeleteRecords_triggered()
{
    bool imAnime  = ui->ChBox_Import_Anime->isChecked();
    bool imManga  = ui->ChBox_Import_Manga->isChecked();
    bool imAmv    = ui->ChBox_Import_Amv->isChecked();
    bool imDorama = ui->ChBox_Import_Dorama->isChecked();
    bool imImages = ui->ChBox_Import_Images->isChecked();

    MngrConnect.transaction();
    QSqlQuery query;
    if( imAnime ){
        if( false == query.exec( QString("DROP TABLE IF EXISTS %1").arg( MngrQuerys::getTableName( sections::anime ) ) ) ){
            qCritical() << QString("Error when deleting a table %1").arg( MngrQuerys::getTableName( sections::anime ) )
                        << query.lastError();
            MngrConnect.rollback();
            return false;
        }

        if( imImages ){
            QDirIterator it( DefinesPath::animeCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }
    }
    if( imManga ){
        if( false == query.exec( QString("DROP TABLE IF EXISTS %1").arg( MngrQuerys::getTableName( sections::manga ) ) ) ){
            qCritical() << QString("Error when deleting a table %1").arg( MngrQuerys::getTableName( sections::manga ) )
                        << query.lastError();
            MngrConnect.rollback();
            return false;
        }

        if( imImages ){
            QDirIterator it( DefinesPath::mangaCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }
    }
    if( imAmv ){
        if( false == query.exec( QString("DROP TABLE IF EXISTS %1").arg( MngrQuerys::getTableName( sections::amv ) ) ) ){
            qCritical() << QString("Error when deleting a table %1").arg( MngrQuerys::getTableName( sections::amv ) )
                        << query.lastError();
            MngrConnect.rollback();
            return false;
        }

        if( imImages ){
            QDirIterator it( DefinesPath::amvCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }
    }
    if( imDorama ){
        if( false == query.exec( QString("DROP TABLE IF EXISTS %1").arg( MngrQuerys::getTableName( sections::dorama ) ) ) ){
            qCritical() << QString("Error when deleting a table %1").arg( MngrQuerys::getTableName( sections::dorama ) )
                        << query.lastError();
            MngrConnect.rollback();
            return false;
        }

        if( imImages ){
            QDirIterator it( DefinesPath::doramaCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }
    }

    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();
    MngrQuerys::createTable_Amv();
    MngrQuerys::createTable_Dorama();
    MngrConnect.commit();
    return true;
}
