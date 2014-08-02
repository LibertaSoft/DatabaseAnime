#include "formsettings.h"
#include "ui_formsettings.h"

#include <QSettings>

#include <QMessageBox>
#include <QDebug>

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

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
}

void FormSettings::BtnBox_resetDefaults(){
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
