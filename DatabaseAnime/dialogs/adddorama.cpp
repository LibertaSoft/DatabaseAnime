#include "dialogs/adddorama.h"
#include "ui_adddorama.h"
//#include "mngrquerys.h"
#include "definespath.h"

#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QDebug>
#include <QSettings>

void DialogAddDorama::initTags()
{
    _tags.setStringList( DoramaModel::getGanresList() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->ListView_Tags->setModel( &_tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddDorama::initOptionalFields()
{
    QSettings settings;
    if( settings.value( Options::OptionalFields::Dorama::AltTitle, false ).toBool() ){
        this->LineEdit_AltTitle = new QLineEdit(this);
        this->LineEdit_AltTitle->setMaxLength(128);
        this->LineEdit_AltTitle->setDragEnabled(true);
        this->LineEdit_AltTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_AltTitle );
    }
    if( settings.value( Options::OptionalFields::Dorama::Director, false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setDragEnabled(true);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_Director );
    }
}

void DialogAddDorama::setDataInField()
{
    ui->CheckBox_LookLater->setChecked( _model->wantToLook() == false );
    ui->CheckBox_Editing->setChecked( _model->editing() == false );

    ui->LineEdit_Title->setText( _model->title() );

    // Optional Fields
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->setText( _model->altTitle() );
    if( this->LineEdit_Director )
        this->LineEdit_Director->setText( _model->director() );

    if( _model->date().year() != 0 )
        ui->SpinBox_Year->setValue( _model->date().year() );
    ui->SpinBox_Season->setValue( _model->season() );

    ui->SpinBox_aTV->setValue( _model->series_all_tv() );
    ui->SpinBox_aSpec->setValue( _model->series_all_special() );
    ui->SpinBox_aMovie->setValue( _model->series_all_movie() );

    ui->SpinBox_vTV->setValue( _model->series_viewed_tv() );
    ui->SpinBox_vSpec->setValue( _model->series_viewed_special() );
    ui->SpinBox_vMovie->setValue( _model->series_viewed_movie() );

    ui->LineEdit_Tags->setText( _model->ganres().join(",") );
    ui->PlainTextEdit_Description->setPlainText( _model->description() );
    ui->PlainTextEdit_Actors->setPlainText( _model->roles() );
    ui->LineEdit_Dir->setText( _model->localPath() );
    ui->LineEdit_URL->setText( _model->url() );

    _oldCover = _model->cover();

    QPixmap pm( DefinesPath::doramaCovers() + _oldCover );
    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( DefinesPath::doramaCovers() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

void DialogAddDorama::setTabOrders()
{
    ui->LineEdit_Title->setFocus();

    if( this->LineEdit_AltTitle ){
        setTabOrder(ui->LineEdit_Title, this->LineEdit_AltTitle);
        if( this->LineEdit_Director ){
            setTabOrder(this->LineEdit_AltTitle, this->LineEdit_Director);
        }
    }else{
        if( this->LineEdit_Director ){
            setTabOrder(ui->LineEdit_Title, this->LineEdit_Director);
        }
    }

    setTabOrder(ui->SpinBox_Season, ui->SpinBox_aTV);
    setTabOrder(ui->SpinBox_aTV,    ui->SpinBox_aSpec);
    setTabOrder(ui->SpinBox_aSpec,  ui->SpinBox_aMovie);
    setTabOrder(ui->SpinBox_aMovie, ui->TabWidget_Series);
}

DialogAddDorama::DialogAddDorama(QWidget *parent, unsigned long long record_id) :
    QDialog(parent), ui(new Ui::DialogAddDorama), _isEditRole(true), _recordId(record_id),
    LineEdit_AltTitle(NULL), LineEdit_Director(NULL)
{
    ui->setupUi(this);
    _model = new DoramaModel( QString::number( record_id ) );

    setWindowTitle( tr("Editing dorama") );
    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Dorama::Geometry).toByteArray() );

    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);

    initTags();
    initOptionalFields();
    setTabOrders();
    setDataInField();
}

DialogAddDorama::DialogAddDorama(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddDorama), _isEditRole(false),
    LineEdit_AltTitle(NULL), LineEdit_Director(NULL)
{
    ui->setupUi(this);
    _model = new DoramaModel();

    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Dorama::Geometry).toByteArray() );

    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);

    initTags();
    initOptionalFields();
    setTabOrders();
}

DialogAddDorama::~DialogAddDorama()
{
    QSettings settings;
    settings.setValue(Options::Dialogs::Dorama::Geometry, this->saveGeometry() );
    delete ui;
}

void DialogAddDorama::btnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();

    // optional
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->clear();
    if( this->LineEdit_Director )
        this->LineEdit_Director->clear();

    ui->SpinBox_Year->setValue(2000);
    ui->SpinBox_Season->setValue(0);

    ui->SpinBox_aTV->setValue(0);
    ui->SpinBox_aSpec->setValue(0);
    ui->SpinBox_aMovie->setValue(0);

    ui->SpinBox_vTV->setValue(0);
    ui->SpinBox_vSpec->setValue(0);
    ui->SpinBox_vMovie->setValue(0);

    ui->LineEdit_Tags->clear();
    ui->ListView_Tags->clearSelection();
    ui->PlainTextEdit_Description->clear();
    ui->PlainTextEdit_Actors->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();

    ui->Lbl_ImageCover->noImage();
}

void DialogAddDorama::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case 7:
            btnBox_reset();
            break;
        case QDialogButtonBox::AcceptRole:
            //on_BtnBox_accepted();
            break;
        default:
            this->close();
    }
}

bool DialogAddDorama::insert_Dorama(){
    _model->setWantToLook( ! ui->CheckBox_LookLater->isChecked() );
    _model->setEditing( ! ui->CheckBox_Editing->isChecked() );
    _model->setTitle( ui->LineEdit_Title->text() );
    _model->setAltTitle( (LineEdit_AltTitle)?this->LineEdit_AltTitle->text():ui->LineEdit_Title->text() );
    _model->setDirector( (LineEdit_Director)?this->LineEdit_Director->text():"" );
    _model->setSeries_all_tv( ui->SpinBox_aTV->value() );
    _model->setSeries_all_special( ui->SpinBox_aSpec->value() );
    _model->setSeries_all_movie( ui->SpinBox_aMovie->value() );
    _model->setSeries_viewed_tv( ui->SpinBox_vTV->value() );
    _model->setSeries_viewed_special( ui->SpinBox_vSpec->value() );
    _model->setSeries_viewed_movie( ui->SpinBox_vMovie->value() );
    _model->setScore( 0 );
    _model->setDate( QDate((ui->CBox_Year->isChecked())? ui->SpinBox_Year->value() : 0, 1, 1) );
    _model->setSeason( ui->SpinBox_Season->value() );

    {
        QStringList list;
        QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
        for(int i = 0;i < mlist.count();i++){
            list.append(mlist.at(i).data(Qt::DisplayRole).toString());
        }

        _model->setGanres( list + ui->LineEdit_Tags->text().split(",") );
    }

    _model->setDescription( ui->PlainTextEdit_Description->toPlainText() );
    _model->setRoles( ui->PlainTextEdit_Actors->toPlainText() );
    _model->setUrl( ui->LineEdit_URL->text() );
    _model->setLocalPath( ui->LineEdit_Dir->text() );

    {
        QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
        QDir dir;
        if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::doramaCovers() ) ){
            QFile f( ui->Lbl_ImageCover->getImagePath() );
            f.copy( DefinesPath::doramaCovers() + coverName );
        }
        if( _isEditRole && !_oldCover.isEmpty() ){
                dir.remove( DefinesPath::doramaCovers() + _oldCover );
        }

        _model->setCover( coverName );
    }

    if( ! _model->save( _isEditRole ) ){
        QMessageBox::critical(this, tr("Critical"), tr("Cannot insert data."));
        return false;
    }
    return true;
}

void DialogAddDorama::on_BtnBox_accepted()
{
    if( ui->LineEdit_Title->text().isEmpty() == false ){
        if( QDir( ui->LineEdit_Dir->text() ).exists() == false ){
            QMessageBox::warning( this, tr("Warning"), tr("The field 'Directory' is uncorrect") );
            ui->LineEdit_Dir->setFocus();
        }else{
            insert_Dorama();
            this->close();
        }
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("The field 'Title' is not filled") );
        ui->LineEdit_Title->setFocus();
    }
}

void DialogAddDorama::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddDorama::on_SpinBox_aTV_valueChanged(int value)
{
    ui->SpinBox_vTV->setMaximum(value);
}

void DialogAddDorama::on_SpinBox_aSpec_valueChanged(int value)
{
    ui->SpinBox_vSpec->setMaximum(value);
}

void DialogAddDorama::on_SpinBox_aMovie_valueChanged(int value)
{
    ui->SpinBox_vMovie->setMaximum(value);
}

void DialogAddDorama::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText(
                QFileDialog::getExistingDirectory(this,
                                                  tr("Choose a directory with video files"),
                                                  QStandardPaths::writableLocation( QStandardPaths::MoviesLocation )
                                                  ) );
}


void DialogAddDorama::on_LineEdit_Dir_textChanged(const QString &path)
{
    if( QDir(path).exists() == false ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}

void DialogAddDorama::on_SpinBox_Year_valueChanged(int)
{
    ui->CBox_Year->setChecked( true );
}
