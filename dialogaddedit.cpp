#include "dialogaddedit.h"
#include "ui_dialogaddedit.h"

#include <QSettings>
#include <QMessageBox>

DialogAddEdit::DialogAddEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEdit)
{
    ui->setupUi(this);
    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", true ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setObjectName("LineEdit_OrigTitle");
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Оригинальное название", "Placeholder text, dialogAddEdit") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", true ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setObjectName("LineEdit_Director");
        this->LineEdit_Director->setPlaceholderText( tr("Режиссёр", "Placeholder text, dialogAddEdit") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setObjectName("LineEdit_PostScoring");
        this->LineEdit_PostScoring->setPlaceholderText( tr("Озвучка", "Placeholder text, dialogAddEdit") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }
}

DialogAddEdit::~DialogAddEdit()
{
    delete ui;
}

void DialogAddEdit::on_BtnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();
    // optional
    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", true ).toBool() ){
        this->LineEdit_OrigTitle->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", true ).toBool() ){
        this->LineEdit_Director->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool() ){
        this->LineEdit_PostScoring->clear();
    }

    ui->DateTimeEdit_Year->setDate( QDate(2000,1,1) );
    ui->SpinBox_Season->setValue(0);
    ui->ComboBox_Studio->clear();

    ui->SpinBox_aTV->setValue(0);
    ui->SpinBox_aOVA->setValue(0);
    ui->SpinBox_aONA->setValue(0);
    ui->SpinBox_aSpec->setValue(0);
    ui->SpinBox_aFilm->setValue(0);

    ui->SpinBox_vTV->setValue(0);
    ui->SpinBox_vOVA->setValue(0);
    ui->SpinBox_vONA->setValue(0);
    ui->SpinBox_vSpec->setValue(0);
    ui->SpinBox_vFilm->setValue(0);

    ui->LineEdit_Tags->clear();
    ui->ListView_Tags->clearSelection();
    ui->PlainTextEdit_Description->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();
}

void DialogAddEdit::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case 7:
            emit on_BtnBox_reset();
            break;
        default:
            this->close();
    }
}

void DialogAddEdit::on_BtnBox_accepted()
{
    QMessageBox::information(this,"Accept","Типа добавлено.");
    this->close();
}

void DialogAddEdit::on_BtnBox_rejected()
{
    this->close();
}
