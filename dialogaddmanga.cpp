#include "dialogaddmanga.h"
#include "ui_dialogaddmanga.h"
#include "mngrquerys.h"

#include <QSettings>
#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QDebug>

void DialogAddManga::initTags()
{
    _tags.setStringList( MngrQuerys::getMangaTags() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->ListView_Tags->setModel( &_tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddManga::setDataInFields()
{
    model = new QSqlQueryModel;
    model->setQuery( QString("SELECT * FROM %1 WHERE id = '%2'").arg(
                         MngrQuerys::getTableName( sections::manga ) ).arg( _recordId ) );
    QMap<QString, QVariant> data;
    data["isHaveLooked"]  = model->record(0).value("isHaveLooked");
    data["isEditingDone"] = model->record(0).value("isEditingDone");
    data["Title"]         = model->record(0).value("Title");
    data["AltTitle"]      = model->record(0).value("AltTitle");
    data["Author"]        = model->record(0).value("Author");
    data["Translation"]   = model->record(0).value("Translation");
    data["Year"]          = model->record(0).value("Year");
    data["Vol"]           = model->record(0).value("Vol");
    data["Ch"]            = model->record(0).value("Ch");
    data["Pages"]         = model->record(0).value("Pages");
    data["vVol"]          = model->record(0).value("vVol");
    data["vCh"]           = model->record(0).value("vCh");
    data["vPages"]        = model->record(0).value("vPages");
    data["Tags"]          = model->record(0).value("Tags");
    data["Description"]   = model->record(0).value("Description");
    data["Dir"]           = model->record(0).value("Dir");
    data["URL"]           = model->record(0).value("URL");
    data["ImagePath"]     = model->record(0).value("ImagePath");
    delete model;

    ui->CheckBox_LookLater->setChecked(!data["isHaveLooked" ].toBool() );
    ui->CheckBox_Editing->setChecked(  !data["isEditingDone"].toBool() );

    ui->LineEdit_Title->setText( data["Title"].toString() );

    // Optional Fields
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->setText( data["AltTitle"].toString() );
    if( this->LineEdit_Author )
        this->LineEdit_Author->setText( data["Author"].toString() );
    if( this->LineEdit_Translation )
        this->LineEdit_Translation->setText( data["Translation"].toString() );

    if( data["Year"].toInt() != 0 )
        ui->SpinBox_Year->setValue( data["Year"].toInt() );

    ui->SpinBox_aVol->setValue( data["Vol"].toInt() );
    ui->SpinBox_aCh->setValue( data["Ch"].toInt() );
    ui->SpinBox_aPages->setValue( data["Pages"].toInt() );

    ui->SpinBox_vVol->setValue( data["vVol"].toInt() );
    ui->SpinBox_vCh->setValue( data["vCh"].toInt() );
    ui->SpinBox_vPages->setValue( data["vPages"].toInt() );

    ui->LineEdit_Tags->setText( data["Tags"].toString() );
    ui->PlainTextEdit_Description->setPlainText( data["Description"].toString() );
    ui->LineEdit_Dir->setText( data["Dir"].toString() );
    ui->LineEdit_URL->setText( data["URL"].toString() );

    _oldCover = data["ImagePath"].toString();
    QPixmap pm( MngrQuerys::getMangaCoversPath() + _oldCover );
    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( MngrQuerys::getMangaCoversPath() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

void DialogAddManga::createOptionalFields()
{
    QSettings settings;
    if( settings.value( "optionalField/manga/AltTitle", false ).toBool() ){
        this->LineEdit_AltTitle = new QLineEdit(this);
        this->LineEdit_AltTitle->setMaxLength(128);
        this->LineEdit_AltTitle->setDragEnabled(true);
        this->LineEdit_AltTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_AltTitle );
    }
    if( settings.value( "optionalField/manga/Author", false ).toBool() ){
        this->LineEdit_Author = new QLineEdit(this);
        this->LineEdit_Author->setMaxLength(32);
        this->LineEdit_Author->setDragEnabled(true);
        this->LineEdit_Author->setPlaceholderText( tr("Author") );
        ui->HLay_AuthorAndSound->addWidget( this->LineEdit_Author );
    }
    if( settings.value( "optionalField/manga/Translation", false ).toBool() ){
        this->LineEdit_Translation = new QLineEdit(this);
        this->LineEdit_Translation->setMaxLength(128);
        this->LineEdit_Translation->setDragEnabled(true);
        this->LineEdit_Translation->setPlaceholderText( tr("Translation") );
        ui->HLay_AuthorAndSound->addWidget( this->LineEdit_Translation );
    }
}

void DialogAddManga::setTabOrders()
{
    if( this->LineEdit_AltTitle  ){
        setTabOrder(ui->SpinBox_Year, this->LineEdit_AltTitle);
        if( !this->LineEdit_Translation && !this->LineEdit_Translation ){
            setTabOrder(this->LineEdit_AltTitle, ui->SpinBox_aVol);
        }
    }
    if( this->LineEdit_Author  ){
        if( this->LineEdit_AltTitle  ){
            setTabOrder(this->LineEdit_AltTitle, this->LineEdit_Author);
        }else{
            setTabOrder(ui->SpinBox_Year, this->LineEdit_Author);
        }
        if( !this->LineEdit_Translation ){
            setTabOrder(this->LineEdit_Author, ui->SpinBox_aVol);
        }
    }
    if( this->LineEdit_Translation ){
        if( this->LineEdit_Author ){
            setTabOrder(this->LineEdit_Author, this->LineEdit_Translation);
        }else{
            if( this->LineEdit_AltTitle ){
                setTabOrder(this->LineEdit_AltTitle, this->LineEdit_Translation);
            }else{
                setTabOrder(ui->SpinBox_Year, this->LineEdit_Translation);
            }
        }
        setTabOrder(this->LineEdit_Translation, ui->SpinBox_aVol);
    }

    if( !this->LineEdit_Translation && !this->LineEdit_Translation && !this->LineEdit_AltTitle ){
        setTabOrder(ui->SpinBox_Year, ui->SpinBox_aVol);
    }

    setTabOrder(ui->SpinBox_aVol, ui->SpinBox_aCh);
    setTabOrder(ui->SpinBox_aCh,  ui->SpinBox_aPages);

    setTabOrder(ui->tab_lookSeries, ui->SpinBox_vVol);
    setTabOrder(ui->SpinBox_vVol,   ui->SpinBox_vCh);
    setTabOrder(ui->SpinBox_vCh,    ui->SpinBox_vPages);
}

DialogAddManga::DialogAddManga(QWidget *parent, unsigned int record_id ) :
    QDialog(parent), ui(new Ui::DialogAddManga), _isEditRole( true ), _recordId( record_id ),
    LineEdit_AltTitle(NULL), LineEdit_Author(NULL), LineEdit_Translation(NULL)
{
    ui->setupUi(this);
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    createOptionalFields();
    setTabOrders();
    setDataInFields();
    initTags();
}

DialogAddManga::DialogAddManga(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddManga), _isEditRole( false ),
    LineEdit_AltTitle(NULL), LineEdit_Author(NULL), LineEdit_Translation(NULL)
{
    ui->setupUi(this);
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    createOptionalFields();
    setTabOrders();
    initTags();
}

DialogAddManga::~DialogAddManga()
{
    delete ui;
}

void DialogAddManga::btnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();

    // Optional Fields
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->clear();
    if( this->LineEdit_Author )
        this->LineEdit_Author->clear();
    if( this->LineEdit_Translation )
        this->LineEdit_Translation->clear();

    ui->SpinBox_Year->setValue(2000);

    ui->SpinBox_aVol->setValue(0);
    ui->SpinBox_aCh->setValue(0);
    ui->SpinBox_aPages->setValue(0);

    ui->SpinBox_vVol->setValue(0);
    ui->SpinBox_vCh->setValue(0);
    ui->SpinBox_vPages->setValue(0);

    ui->LineEdit_Tags->clear();
    ui->ListView_Tags->clearSelection();
    ui->PlainTextEdit_Description->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();

    ui->Lbl_ImageCover->noImage();
}

void DialogAddManga::on_BtnBox_clicked(QAbstractButton *button)
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

bool DialogAddManga::insert_Manga(){
    QMap<QString, QVariant> data;

    data["isHaveLooked"]  = !ui->CheckBox_LookLater->isChecked();
    data["isEditingDone"] = !ui->CheckBox_Editing->isChecked();
    data["id"]            = _recordId;
    data["Title"]         = ui->LineEdit_Title->text();
    data["AltTitle"]      = (LineEdit_AltTitle   ) ?    LineEdit_AltTitle->text() : "";
    data["Author"]        = (LineEdit_Author     ) ?      LineEdit_Author->text() : "";
    data["Translation"]   = (LineEdit_Translation) ? LineEdit_Translation->text() : "";

    data["Vol"]    = ui->SpinBox_aVol->value();
    data["Ch"]     = ui->SpinBox_aCh->value();
    data["Pages"]  = ui->SpinBox_aPages->value();
    data["vVol"]   = ui->SpinBox_vVol->value();
    data["vCh"]    = ui->SpinBox_vCh->value();
    data["vPages"] = ui->SpinBox_vPages->value();
    data["Year"]   = (ui->CBox_Year->isChecked()) ? ui->SpinBox_Year->value() : 0 ;

    QString tagsList;
    QStringList list;
    QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
    for(int i = 0;i < mlist.count();i++){
        list.append(mlist.at(i).data(Qt::DisplayRole).toString());
    }

    for(int i = 0; i < list.count();i++){
        if( i != 0 ){
            tagsList += ", ";
        }
        tagsList += list.at(i);
    }
    if( !ui->LineEdit_Tags->text().isEmpty() && !tagsList.isEmpty() ){
        tagsList += ", ";
    }
    tagsList += ui->LineEdit_Tags->text();

    data["Tags"]         = tagsList;
    data["Description"]  = ui->PlainTextEdit_Description->toPlainText();
    data["URL"]          = ui->LineEdit_URL->text();
    data["Dir"]          = ui->LineEdit_Dir->text();

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( MngrQuerys::getMangaCoversPath() ) ){
        QFile f;
        f.setFileName( ui->Lbl_ImageCover->getImagePath() );
        f.copy( MngrQuerys::getMangaCoversPath() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() )
        dir.remove( MngrQuerys::getMangaCoversPath() + _oldCover );
    data["ImagePath"] = coverName;

    if( !_isEditRole ){
        if( MngrQuerys::insertManga(data) == false ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot insert data"));
            return false;
        }
    }else{
        if( MngrQuerys::updateManga(data) == false ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot update data"));
            return false;
        }
    }
    return true;
}

void DialogAddManga::on_BtnBox_accepted()
{
    if( ui->LineEdit_Title->text().isEmpty() == false ){
        if( QDir( ui->LineEdit_Dir->text() ).exists() == false ){
            QMessageBox::warning( this, tr("Warning"), tr("The field 'Directory' is uncorrect") );
            ui->LineEdit_Dir->setFocus();
        }else{
            insert_Manga();
            this->close();
        }
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("The field 'Title' is not filled") );
        ui->LineEdit_Title->setFocus();
    }
}

void DialogAddManga::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddManga::on_SpinBox_aVol_valueChanged(int value)
{
    ui->SpinBox_vVol->setMaximum(value);
}

void DialogAddManga::on_SpinBox_aCh_valueChanged(int value)
{
    ui->SpinBox_vCh->setMaximum(value);
}

void DialogAddManga::on_SpinBox_aPages_valueChanged(int value)
{
    ui->SpinBox_vPages->setMaximum(value);
}

void DialogAddManga::on_LineEdit_Dir_textChanged(const QString &path)
{
    if( QDir(path).exists() == false ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}

void DialogAddManga::on_TBtn_ChooseDir_clicked()
{
    ui->LineEdit_Dir->setText(
                QFileDialog::getExistingDirectory(this,
                                                  tr("Choose a directory with picture files"),
                                                  QStandardPaths::writableLocation( QStandardPaths::PicturesLocation )
                                                  ) );
}

void DialogAddManga::on_SpinBox_Year_valueChanged(int = 0)
{
    ui->CBox_Year->setChecked( true );
}
