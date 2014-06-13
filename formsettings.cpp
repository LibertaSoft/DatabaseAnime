#include "formsettings.h"
#include "ui_formsettings.h"

#include <QAbstractButton>
#include <QSettings>
#include <QDialogButtonBox>

#include <iostream>

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);

    QSettings settings;

    bool a1 = settings.value( "enableElem/BtnSwitchSection/Anime",     true ).toBool();
    bool a2 = settings.value( "enableElem/BtnSwitchSection/Manga",     true ).toBool();
    bool a3 = settings.value( "enableElem/BtnSwitchSection/AMV",      false ).toBool();
    bool a4 = settings.value( "enableElem/BtnSwitchSection/Dorama",   false ).toBool();
    bool a5 = settings.value( "enableElem/BtnSwitchSection/Editable",  true ).toBool();
    bool a6 = settings.value( "enableElem/BtnSwitchSection/LookLater", true ).toBool();

    bool b1 = settings.value( "enableElem/FieldsForEdit/OrigTitle",   true ).toBool();
    bool b2 = settings.value( "enableElem/FieldsForEdit/Director",    true ).toBool();
    bool b3 = settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool();

    ui->CheckBox_EnableAnime->setChecked( a1 );
    ui->CheckBox_EnableManga->setChecked( a2 );
    ui->CheckBox_EnableAMV->setChecked( a3 );
    ui->CheckBox_EnableDorama->setChecked( a4 );
    ui->CheckBox_EnableEditing->setChecked( a5 );
    ui->CheckBox_EnableLookLater->setChecked( a6 );

    ui->CheckBox_OrigTitle->setChecked( b1 );
    ui->CheckBox_Director->setChecked( b2 );
    ui->CheckBox_PostScoring->setChecked( b3 );
}

FormSettings::~FormSettings()
{
    delete ui;
}

void FormSettings::on_BtnBox_accepted()
{
    QSettings settings;

    settings.setValue( "configExist", true );
    settings.setValue( "enableElem/BtnSwitchSection/Anime",     ui->CheckBox_EnableAnime->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Manga",     ui->CheckBox_EnableManga->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/AMV",       ui->CheckBox_EnableAMV->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Dorama",    ui->CheckBox_EnableDorama->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Editable",  ui->CheckBox_EnableEditing->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/LookLater", ui->CheckBox_EnableLookLater->isChecked() );

    settings.setValue( "enableElem/FieldsForEdit/OrigTitle",   ui->CheckBox_OrigTitle->isChecked() );
    settings.setValue( "enableElem/FieldsForEdit/Director",    ui->CheckBox_Director->isChecked() );
    settings.setValue( "enableElem/FieldsForEdit/PostScoring", ui->CheckBox_PostScoring->isChecked() );

    //save tags in database

}

void FormSettings::on_BtnBox_resetDefaults(){
    ui->CheckBox_EnableAnime->setChecked( true );
    ui->CheckBox_EnableManga->setChecked( true );
    ui->CheckBox_EnableAMV->setChecked( false );
    ui->CheckBox_EnableDorama->setChecked( false );
    ui->CheckBox_EnableEditing->setChecked( true );
    ui->CheckBox_EnableLookLater->setChecked( true );

    ui->CheckBox_OrigTitle->setChecked( true );
    ui->CheckBox_Director->setChecked( true );
    ui->CheckBox_PostScoring->setChecked( true );
}

void FormSettings::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case  QDialogButtonBox::ApplyRole:
            emit on_BtnBox_accepted();
            this->close();
            break;
        case QDialogButtonBox::ResetRole:
            emit on_BtnBox_resetDefaults();
            break;
        default:
            this->close();
    }
//    std::cout << "[Pressed:]" << ui->BtnBox->buttonRole( button ) << std::endl;
//    std::cout << "[Need:]" << QDialogButtonBox::ApplyRole << std::endl;
}
