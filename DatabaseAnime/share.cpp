#include "share.h"

Share::Share(MngrConnection &MngrCon, QWidget *parent) :
    QDialog(parent), MngrConnect(MngrCon)
{
    ui.setupUi(this);

    ui.ProgressBar_Export->setVisible(false);
    ui.ProgressBar_Import->setVisible(false);
}

void Share::setTab(Share::Tabs tab)
{
    ui.tabWidget->setCurrentIndex( tab );
}

void Share::on_PBtn_Action_Export_clicked()
{
    bool exAnime  = ui.ChBox_Export_Anime->isChecked();
    bool exManga  = ui.ChBox_Export_Manga->isChecked();
    bool exAmv    = ui.ChBox_Export_Amv->isChecked();
    bool exDorama = ui.ChBox_Export_Dorama->isChecked();
    bool exImages = ui.ChBox_Export_Images->isChecked();

    QString exportDir = ui.LineEdit_ExportFolder->text();
            exportDir = QDir( exportDir ).path() + QDir::separator();

    if( exportDir.isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The directory for export isn't choose"));
        ui.LineEdit_ExportFolder->setFocus();
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

    ui.ProgressBar_Export->setMinimum(0);
    ui.ProgressBar_Export->setValue(0);
    ui.ProgressBar_Export->setMaximum(allCount);

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
            ui.ProgressBar_Export->setValue( progress++ );
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
            ui.ProgressBar_Export->setValue( progress++ );
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
            ui.ProgressBar_Export->setValue( progress++ );
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
            ui.ProgressBar_Export->setValue( progress++ );
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


void Share::on_TButton_ChooseExportFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for export file"),
                                                    ui.LineEdit_ExportFolder->text()
                                                    );
    if( dir.isEmpty() )
        ui.LineEdit_ExportFolder->setText( DefinesPath::home() );
    else
        ui.LineEdit_ExportFolder->setText( dir );
}

void Share::on_TButton_ChooseImportFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Choose a xml file for import"),
                                                ui.LineEdit_ImportFile->text()
                                                );
    if( file.isEmpty() )
        ui.LineEdit_ImportFile->setText( DefinesPath::home() );
    else
        ui.LineEdit_ImportFile->setText( file );
}

void Share::on_PBtn_Import_Append_clicked()
{
    if( ui.LineEdit_ImportFile->text().isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The imported file isn't selected"));
        ui.LineEdit_ImportFile->setFocus();
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

void Share::on_PBtn_Import_Replace_clicked()
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

quint64 Share::import()
{
    bool imAnime  = ui.ChBox_Import_Anime->isChecked();
    bool imManga  = ui.ChBox_Import_Manga->isChecked();
    bool imAmv    = ui.ChBox_Import_Amv->isChecked();
    bool imDorama = ui.ChBox_Import_Dorama->isChecked();
    bool imImages = ui.ChBox_Import_Images->isChecked();
    QString filePath = ui.LineEdit_ImportFile->text();

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
    ui.ProgressBar_Import->setMinimum(0);
    ui.ProgressBar_Import->setValue(0);
    ui.ProgressBar_Import->setMaximum(allCount);
    ui.ProgressBar_Import->setFormat( tr("Import of records") + " %p%" );

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
        ui.ProgressBar_Import->setValue(progress);
        QCoreApplication::processEvents();
    }
    if( reader.hasError() )
        MngrConnect.rollback();
    else
        MngrConnect.commit();

    file.close();

    ui.ProgressBar_Import->setFormat( tr("Copying of covers")/* + " %p%" */);
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

quint64 Share::removeFilesFromFolder(QString folder)
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

quint64 Share::copyFolder(QString folder1, QString folder2)
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

bool Share::deleteRecords()
{
    bool imAnime  = ui.ChBox_Import_Anime->isChecked();
    bool imManga  = ui.ChBox_Import_Manga->isChecked();
    bool imAmv    = ui.ChBox_Import_Amv->isChecked();
    bool imDorama = ui.ChBox_Import_Dorama->isChecked();
    bool imImages = ui.ChBox_Import_Images->isChecked();

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
 * \brief Метод - отображения/скрытия прогресс бара импорта
 * \param checked - отобразить или скрыть прогресс бар
 *
 * Метод при аттрибуте cheched равном true - скрывает кнопки действий
 * и отображает прогресс бар процесса импорта
 */
void Share::on_actionShowImportProgressBar_triggered(bool checked)
{
    ui.PBtn_Import_Append->setVisible(!checked);
    ui.PBtn_Import_Replace->setVisible(!checked);
    ui.ProgressBar_Import->setVisible(checked);
}
/*! \~russian
 * \brief Метод - отображения/скрытия прогресс бара экспорта
 * \param checked - отобразить или скрыть прогресс бар
 *
 * Метод при аттрибуте cheched равном true - скрывает кнопки действий
 * и отображает прогресс бар процесса экспорта
 */
void Share::on_actionShowExportProgressBar_triggered(bool checked)
{
    ui.PBtn_Action_Export->setVisible(!checked);
    ui.ProgressBar_Export->setVisible(checked);
}
