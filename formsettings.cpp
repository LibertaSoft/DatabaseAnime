#include "formsettings.h"
#include "ui_formsettings.h"
#include "mngrconnection.h"

#include <QSettings>
#include <QVariant>

#include <QMessageBox>
#include <QDebug>

#include <QtXml>

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings), restoreDefault(false)
{
    ui->setupUi(this);
    ui->tabWidget_General->setCurrentIndex(0);
    ui->tabWidget_ImpExp->setCurrentIndex(0);

    QSettings settings;

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


QDomElement anime(  QDomDocument& domDoc,
                    const QMap<QString, QString> &data
                    )
{
    QDomElement domElement = makeElement(domDoc,
                                         "anime"
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


void FormSettings::on_PBtn_Export2xml_clicked()
{
        QDomDocument doc("DatabaseAnime");
        QDomElement  dom = doc.createElement("DatabaseAnime");
        doc.appendChild(dom);

        QSqlQuery query;
        if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime)) ) ){
            qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
            QMessageBox::critical(0, tr("Critical"), tr("Cannot select data in database") );
            return;
        }
        QDomElement  domAnime = doc.createElement("Section");
        domAnime.setAttribute("type", "anime");
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

            domAnime.appendChild( anime(doc, data) );
        }


        QFile file("/tmp/DatabaseAnime.xml");
        if( file.open(QIODevice::WriteOnly) ) {
            QTextStream(&file) << doc.toString(4);
            file.close();
        }else{
            qCritical() << file.errorString()
                        << "\nFileName: " << file.fileName();
            QMessageBox::critical(0, tr("Critical"), tr("File is not created"));
        }
}
