#include "formsettings.h"
#include "ui_formsettings.h"
#include "mngrconnection.h"

#include <QSettings>
#include <QVariant>

#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

#include <QtXml>

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings), restoreDefault(false)
{
    ui->setupUi(this);
    QSettings settings;
    this->restoreGeometry( settings.value("WindowSettings/Geometry").toByteArray() );
    ui->tabWidget_General->setCurrentIndex(0);
    ui->tabWidget_ImpExp->setCurrentIndex(0);
    ui->LineEdit_ExDir->setText( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) );

    bool b1 = settings.value( "enableSection/Anime",   true ).toBool();
    bool b2 = settings.value( "enableSection/Manga",  false ).toBool();
    bool b3 = settings.value( "enableSection/AMV",    false ).toBool();
    bool b4 = settings.value( "enableSection/Dorama", false ).toBool();

    bool a1 = settings.value( "optionalField/anime/OrigTitle",   false ).toBool();
    bool a2 = settings.value( "optionalField/anime/Director",    false ).toBool();
    bool a3 = settings.value( "optionalField/anime/PostScoring", false ).toBool();

    bool m1 = settings.value( "optionalField/manga/AltTitle",    false ).toBool();
    bool m2 = settings.value( "optionalField/manga/Author",      false ).toBool();
    bool m3 = settings.value( "optionalField/manga/Translation", false ).toBool();

    bool d1 = settings.value( "optionalField/dorama/AltTitle",   false ).toBool();
    bool d2 = settings.value( "optionalField/dorama/Director",   false ).toBool();

    bool c1 = settings.value( "SwitchToDirOnHoverACover", true ).toBool();

    ui->CheckBox_EnableAnime->setChecked( b1 );
    ui->CheckBox_EnableManga->setChecked( b2 );
    ui->CheckBox_EnableAMV->setChecked( b3 );
    ui->CheckBox_EnableDorama->setChecked( b4 );

    ui->CBox_Anime_AltTitle->setChecked( a1 );
    ui->CBox_Anime_Director->setChecked( a2 );
    ui->CBox_Anime_PostScoring->setChecked( a3 );

    ui->CBox_Manga_AltTitle->setChecked( m1 );
    ui->CBox_Manga_Author->setChecked( m2 );
    ui->CBox_Manga_Translation->setChecked( m3 );

    ui->CBox_Dorama_AltTitle->setChecked( d1 );
    ui->CBox_Dorama_Director->setChecked( d2 );

    ui->CBox_SwitchToDirOnHoverCover->setChecked( c1 );

    QString set_language = settings.value( "Application/l10n", tr("<System>") ).toString();

    ui->CB_Language->addItem( tr("<System>"), 0 );
    QDir dir( QApplication::applicationDirPath() );
    QStringList l10n = dir.entryList( QStringList("DatabaseAnime_*.qm") );
    for(int i = 0; i < l10n.count(); ++i){
        ui->CB_Language->addItem( l10n.at(i).right(5).left(2) );
        if( set_language == l10n.at(i).right(5).left(2) )
            ui->CB_Language->setCurrentIndex(i+1);
    }
    Sort::sort sort = static_cast<Sort::sort>( settings.value( "Sorting", Sort::asc ).toInt() );
    ui->CBox_Sort->setCurrentIndex( sort );

    const QString homeDir(QDir::homePath()
                          + QDir::separator() + "." + QApplication::organizationName()
                          + QDir::separator() + QApplication::applicationName() + QDir::separator());
    ui->LineEdit_WorkDirectory->setText( settings.value( "WorkDirectory", homeDir ).toString() );
}

FormSettings::~FormSettings()
{
    QSettings settings;
    settings.setValue("WindowSettings/Geometry", this->saveGeometry() );
    delete ui;
}

Sort::sort FormSettings::getSort()
{
    return static_cast<Sort::sort>( ui->CBox_Sort->currentIndex() );
}

bool FormSettings::getSwitchToDir()
{
    return ui->CBox_SwitchToDirOnHoverCover->isChecked();
}

bool FormSettings::getRestoreDefault()
{
    return restoreDefault;
}

void FormSettings::on_BtnBox_accepted()
{
    QSettings settings;

    settings.setValue( "enableSection/Anime",     ui->CheckBox_EnableAnime->isChecked() );
    settings.setValue( "enableSection/Manga",     ui->CheckBox_EnableManga->isChecked() );
    settings.setValue( "enableSection/AMV",       ui->CheckBox_EnableAMV->isChecked() );
    settings.setValue( "enableSection/Dorama",    ui->CheckBox_EnableDorama->isChecked() );

    settings.setValue( "optionalField/anime/OrigTitle",   ui->CBox_Anime_AltTitle->isChecked() );
    settings.setValue( "optionalField/anime/Director",    ui->CBox_Anime_Director->isChecked() );
    settings.setValue( "optionalField/anime/PostScoring", ui->CBox_Anime_PostScoring->isChecked() );

    settings.setValue( "optionalField/manga/AltTitle",    ui->CBox_Manga_AltTitle->isChecked() );
    settings.setValue( "optionalField/manga/Author",      ui->CBox_Manga_Author->isChecked() );
    settings.setValue( "optionalField/manga/Translation", ui->CBox_Manga_Translation->isChecked() );

    settings.setValue( "optionalField/dorama/AltTitle",   ui->CBox_Dorama_AltTitle->isChecked() );
    settings.setValue( "optionalField/dorama/Director",   ui->CBox_Dorama_Director->isChecked() );

    settings.setValue( "Application/l10n", ui->CB_Language->currentText() );
    settings.setValue( "Application/l10n_index", ui->CB_Language->currentIndex() );
    settings.setValue( "Sorting", ui->CBox_Sort->currentIndex() );

    settings.setValue( "SwitchToDirOnHoverACover", ui->CBox_SwitchToDirOnHoverCover->isChecked() );

    if( QDir::isAbsolutePath(ui->LineEdit_WorkDirectory->text()) )
        settings.setValue( "WorkDirectory", QDir(ui->LineEdit_WorkDirectory->text()).path() );
    else
        settings.remove("WorkDirectory");
}

void FormSettings::BtnBox_resetDefaults(){
    restoreDefault = true;
    ui->CheckBox_EnableAnime->setChecked( true );
    ui->CheckBox_EnableManga->setChecked( false );
    ui->CheckBox_EnableAMV->setChecked( false );
    ui->CheckBox_EnableDorama->setChecked( false );

    ui->CBox_Anime_AltTitle->setChecked( false );
    ui->CBox_Anime_Director->setChecked( false );
    ui->CBox_Anime_PostScoring->setChecked( false );

    ui->CBox_Manga_AltTitle->setChecked( false );
    ui->CBox_Manga_Author->setChecked( false );
    ui->CBox_Manga_Translation->setChecked( false );

    ui->CBox_Dorama_AltTitle->setChecked( false );
    ui->CBox_Dorama_Director->setChecked( false );

    ui->CB_Language->setCurrentIndex(0);
    ui->CBox_Sort->setCurrentIndex(1);

    const QString homeDir(QDir::homePath()
                          + QDir::separator() + "." + QApplication::organizationName()
                          + QDir::separator() + QApplication::applicationName() + QDir::separator());
    ui->LineEdit_WorkDirectory->setText( homeDir );
}

void FormSettings::on_BtnBox_clicked(QAbstractButton *button)
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
/*[Export]*/

QDomElement makeElement(      QDomDocument& domDoc,
                        const QString&      strName,
                        const QString&      strAttr = QString::null,
                        const QString&      strText = QString::null
                       )
{
    QDomElement domElement = domDoc.createElement(strName);

    if (!strAttr.isEmpty()) {
        QDomAttr domAttr = domDoc.createAttribute("number");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }

    if (!strText.isEmpty()) {
        QDomText domText = domDoc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}

namespace Export{
QDomElement anime(  QDomDocument& domDoc,
                    const QMap<QString, QString> &data
                    )
{
    QDomElement domElement = makeElement(domDoc,
                                         "item"
                                         );
    domElement.appendChild(makeElement(domDoc, "isHaveLooked",  "", data["isHaveLooked"]));
    domElement.appendChild(makeElement(domDoc, "isEditingDone", "", data["isEditingDone"]));
    domElement.appendChild(makeElement(domDoc, "isAdult",       "", data["isAdult"]));
    domElement.appendChild(makeElement(domDoc, "Title",         "", data["Title"]));
    domElement.appendChild(makeElement(domDoc, "OrigTitle",     "", data["OrigTitle"]));
    domElement.appendChild(makeElement(domDoc, "Director",      "", data["Director"]));
    domElement.appendChild(makeElement(domDoc, "PostScoring",   "", data["PostScoring"]));
    domElement.appendChild(makeElement(domDoc, "SeriesTV",      "", data["SeriesTV"]));
    domElement.appendChild(makeElement(domDoc, "SeriesOVA",     "", data["SeriesOVA"]));
    domElement.appendChild(makeElement(domDoc, "SeriesONA",     "", data["SeriesONA"]));
    domElement.appendChild(makeElement(domDoc, "SeriesSpecial", "", data["SeriesSpecial"]));
    domElement.appendChild(makeElement(domDoc, "SeriesMovie",   "", data["SeriesMovie"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesTV",     "", data["vSeriesTV"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesOVA",    "", data["vSeriesOVA"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesONA",    "", data["vSeriesONA"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesSpecial","", data["vSeriesSpecial"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesMovie",  "", data["vSeriesMovie"]));
    domElement.appendChild(makeElement(domDoc, "Score",         "", data["Score"]));
    domElement.appendChild(makeElement(domDoc, "Year",          "", data["Year"]));
    domElement.appendChild(makeElement(domDoc, "Season",        "", data["Season"]));
    domElement.appendChild(makeElement(domDoc, "Studios",       "", data["Studios"]));
    domElement.appendChild(makeElement(domDoc, "Tags",          "", data["Tags"]));
    domElement.appendChild(makeElement(domDoc, "Description",   "", data["Description"]));
    domElement.appendChild(makeElement(domDoc, "URL",           "", data["URL"]));
    domElement.appendChild(makeElement(domDoc, "Dir",           "", data["Dir"]));
    domElement.appendChild(makeElement(domDoc, "ImagePath",     "", data["ImagePath"]));

    return domElement;
}
QDomElement manga(  QDomDocument& domDoc,
                    const QMap<QString, QString> &data
                    )
{
    QDomElement domElement = makeElement(domDoc,
                                         "item"
                                         );
    domElement.appendChild(makeElement(domDoc, "isHaveLooked",  "", data["isHaveLooked"]));
    domElement.appendChild(makeElement(domDoc, "isEditingDone", "", data["isEditingDone"]));
    domElement.appendChild(makeElement(domDoc, "isAdult",       "", data["isAdult"]));
    domElement.appendChild(makeElement(domDoc, "Title",         "", data["Title"]));
    domElement.appendChild(makeElement(domDoc, "AltTitle",      "", data["AltTitle"]));
    domElement.appendChild(makeElement(domDoc, "Author",        "", data["Author"]));
    domElement.appendChild(makeElement(domDoc, "Translation",   "", data["Translation"]));
    domElement.appendChild(makeElement(domDoc, "Vol",           "", data["Vol"]));
    domElement.appendChild(makeElement(domDoc, "Ch",            "", data["Ch"]));
    domElement.appendChild(makeElement(domDoc, "Pages",         "", data["Pages"]));
    domElement.appendChild(makeElement(domDoc, "vVol",          "", data["vVol"]));
    domElement.appendChild(makeElement(domDoc, "vCh",           "", data["vCh"]));
    domElement.appendChild(makeElement(domDoc, "vPages",        "", data["vPages"]));
    domElement.appendChild(makeElement(domDoc, "Score",         "", data["Score"]));
    domElement.appendChild(makeElement(domDoc, "Year",          "", data["Year"]));
    domElement.appendChild(makeElement(domDoc, "Season",        "", data["Season"]));
    domElement.appendChild(makeElement(domDoc, "Studios",       "", data["Studios"]));
    domElement.appendChild(makeElement(domDoc, "Tags",          "", data["Tags"]));
    domElement.appendChild(makeElement(domDoc, "Description",   "", data["Description"]));
    domElement.appendChild(makeElement(domDoc, "URL",           "", data["URL"]));
    domElement.appendChild(makeElement(domDoc, "Dir",           "", data["Dir"]));
    domElement.appendChild(makeElement(domDoc, "ImagePath",     "", data["ImagePath"]));

    return domElement;
}
QDomElement amv(  QDomDocument& domDoc,
                    const QMap<QString, QString> &data
                    )
{
    QDomElement domElement = makeElement(domDoc,
                                         "item"
                                         );
    domElement.appendChild(makeElement(domDoc, "isEditingDone",   "", data["isEditingDone"]));
    domElement.appendChild(makeElement(domDoc, "isAdult",         "", data["isAdult"]));
    domElement.appendChild(makeElement(domDoc, "Title",           "", data["Title"]));
    domElement.appendChild(makeElement(domDoc, "Author",          "", data["Author"]));
    domElement.appendChild(makeElement(domDoc, "Сontestant",      "", data["Сontestant"]));
    domElement.appendChild(makeElement(domDoc, "Score",           "", data["Score"]));
    domElement.appendChild(makeElement(domDoc, "Year",            "", data["Year"]));
    domElement.appendChild(makeElement(domDoc, "Tags",            "", data["Tags"]));
    domElement.appendChild(makeElement(domDoc, "ContainingMusic", "", data["ContainingMusic"]));
    domElement.appendChild(makeElement(domDoc, "ContainingAnime", "", data["ContainingAnime"]));
    domElement.appendChild(makeElement(domDoc, "AuthorComment",   "", data["AuthorComment"]));
    domElement.appendChild(makeElement(domDoc, "URL",             "", data["URL"]));
    domElement.appendChild(makeElement(domDoc, "Dir",             "", data["Dir"]));
    domElement.appendChild(makeElement(domDoc, "ImagePath",       "", data["ImagePath"]));

    return domElement;
}

QDomElement dorama(  QDomDocument& domDoc,
                    const QMap<QString, QString> &data
                    )
{
    QDomElement domElement = makeElement(domDoc,
                                         "item"
                                         );
    domElement.appendChild(makeElement(domDoc, "isHaveLooked",  "", data["isHaveLooked"]));
    domElement.appendChild(makeElement(domDoc, "isEditingDone", "", data["isEditingDone"]));
    domElement.appendChild(makeElement(domDoc, "isAdult",       "", data["isAdult"]));
    domElement.appendChild(makeElement(domDoc, "Title",         "", data["Title"]));
    domElement.appendChild(makeElement(domDoc, "AltTitle",      "", data["AltTitle"]));
    domElement.appendChild(makeElement(domDoc, "Director",      "", data["Director"]));
    domElement.appendChild(makeElement(domDoc, "SeriesTV",      "", data["SeriesTV"]));
    domElement.appendChild(makeElement(domDoc, "SeriesSpecial", "", data["SeriesSpecial"]));
    domElement.appendChild(makeElement(domDoc, "SeriesMovie",   "", data["SeriesMovie"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesTV",     "", data["vSeriesTV"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesSpecial","", data["vSeriesSpecial"]));
    domElement.appendChild(makeElement(domDoc, "vSeriesMovie",  "", data["vSeriesMovie"]));
    domElement.appendChild(makeElement(domDoc, "Score",         "", data["Score"]));
    domElement.appendChild(makeElement(domDoc, "Year",          "", data["Year"]));
    domElement.appendChild(makeElement(domDoc, "Season",        "", data["Season"]));
    domElement.appendChild(makeElement(domDoc, "Tags",          "", data["Tags"]));
    domElement.appendChild(makeElement(domDoc, "Description",   "", data["Description"]));
    domElement.appendChild(makeElement(domDoc, "Actors",        "", data["Actors"]));
    domElement.appendChild(makeElement(domDoc, "URL",           "", data["URL"]));
    domElement.appendChild(makeElement(domDoc, "Dir",           "", data["Dir"]));
    domElement.appendChild(makeElement(domDoc, "ImagePath",     "", data["ImagePath"]));

    return domElement;
}
}

void FormSettings::on_PBtn_Export_clicked()
{
    if( ui->LineEdit_ExDir->text().isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The directory for export isn't choose"));
        ui->LineEdit_ExDir->setFocus();
        return;
    }

    QDomDocument doc("DatabaseAnime");
    QDomElement  dom = doc.createElement("DatabaseAnime");
    doc.appendChild(dom);

    QSqlQuery query;

    if( ui->CBox_ExAnime->isChecked() ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        QDomElement  domAnime = doc.createElement("Anime");
        dom.appendChild(domAnime);
        while (query.next()) {
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

            domAnime.appendChild( Export::anime(doc, data) );
            QCoreApplication::processEvents();
        }
    }
    if( ui->CBox_ExManga->isChecked() ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::manga)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::manga) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        QDomElement  domManga = doc.createElement("Manga");
        dom.appendChild(domManga);
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
            data["Season"]          = query.value("Season"          ).toString();
            data["Studios"]         = query.value("Studios"         ).toString();
            data["Tags"]            = query.value("Tags"            ).toString();
            data["Description"]     = query.value("Description"     ).toString();
            data["URL"]             = query.value("URL"             ).toString();
            data["Dir"]             = query.value("Dir"             ).toString();
            data["ImagePath"]       = query.value("ImagePath"       ).toString();

            domManga.appendChild( Export::manga(doc, data) );
            QCoreApplication::processEvents();
        }
    }

    if( ui->CBox_ExAmv->isChecked() ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::amv)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::amv) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        QDomElement  domAmv = doc.createElement("Amv");
        dom.appendChild(domAmv);
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

            domAmv.appendChild( Export::amv(doc, data) );
            QCoreApplication::processEvents();
        }
    }

    if( ui->CBox_ExDorama->isChecked() ){
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::dorama)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::dorama) );
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        QDomElement  domDorama = doc.createElement("Dorama");
        dom.appendChild(domDorama);
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

            domDorama.appendChild( Export::dorama(doc, data) );
            QCoreApplication::processEvents();
        }
    }

    QDir().mkpath( ui->LineEdit_ExDir->text() );
    QFile file( QDir(ui->LineEdit_ExDir->text()).path() + QDir::separator() + "DatabaseAnime.xml" );
    if( file.open(QIODevice::WriteOnly) ) {
        QTextStream(&file) << doc.toString(4);
        file.close();

        if( ui->CBox_ExAnime->isChecked() ){
            QDirIterator it( MngrQuerys::getAnimeCoversPath() );
            QString animeCoversExportPath( ui->LineEdit_ExDir->text() + QDir::separator() + "animeCovers" + QDir::separator() );
            QDir().mkpath( animeCoversExportPath );
            while( it.hasNext() ){
                it.next();
                QFile( it.filePath() ).copy( animeCoversExportPath + it.fileName() );
                QCoreApplication::processEvents();
            }
        }
        if( ui->CBox_ExManga->isChecked() ){
            QDirIterator it( MngrQuerys::getMangaCoversPath() );
            QString mangaCoversExportPath( ui->LineEdit_ExDir->text() + QDir::separator() + "mangaCovers" + QDir::separator() );
            QDir().mkpath( mangaCoversExportPath );
            while( it.hasNext() ){
                it.next();
                QFile( it.filePath() ).copy( mangaCoversExportPath + it.fileName() );
                QCoreApplication::processEvents();
            }
        }
        if( ui->CBox_ExAmv->isChecked() ){
            QDirIterator it( MngrQuerys::getAmvCoversPath() );
            QString amvCoversExportPath( ui->LineEdit_ExDir->text() + QDir::separator() + "amvCovers" + QDir::separator() );
            QDir().mkpath( amvCoversExportPath );
            while( it.hasNext() ){
                it.next();
                QFile( it.filePath() ).copy( amvCoversExportPath + it.fileName() );
                QCoreApplication::processEvents();
            }
        }
        if( ui->CBox_ExDorama->isChecked() ){
            QDirIterator it( MngrQuerys::getDoramaCoversPath() );
            QString doramaCoversExportPath( ui->LineEdit_ExDir->text() + QDir::separator() + "doramaCovers" + QDir::separator() );
            QDir().mkpath( doramaCoversExportPath );
            while( it.hasNext() ){
                it.next();
                QFile( it.filePath() ).copy( doramaCoversExportPath + it.fileName() );
                QCoreApplication::processEvents();
            }
        }

    }else{
        qCritical() << file.errorString()
                    << "\nFileName: " << file.fileName();
        QMessageBox::critical(this, tr("Critical"), tr("File is not created"));
    }
}

void FormSettings::on_TBtn_ChooseDir_clicked()
{
    ui->LineEdit_ExDir->setText(
                QFileDialog::getExistingDirectory(this,
                                                  tr("Choose a directory for export file"),
                                                  QStandardPaths::writableLocation( QStandardPaths::HomeLocation )
                                                  ) );
}
