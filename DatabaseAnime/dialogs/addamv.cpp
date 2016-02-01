#include "dialogs/addamv.h"
#include "ui_addamv.h"
//#include "mngrquerys.h"
#include "definespath.h"

#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QDebug>
#include <QSettings>

void DialogAddAmv::initTags()
{
    _tags.setStringList( AmvModel::getTagsList() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->LView_Tags->setModel( &_tags );
    ui->LView_Tags->setWrapping( true );
    ui->LView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddAmv::setDataInField()
{
    ui->CheckBox_Editing->setChecked( _model->editing() == false );
    ui->LineEdit_Title->setText( _model->title() );
    ui->LineEdit_Author->setText( _model->author() );
    ui->LineEdit_Contestant->setText( _model->contestant() );
    if( _model->date().year() != 0 )
        ui->SpinBox_Year->setValue( _model->date().year() );
    ui->LineEdit_Tags->setText( _model->tags().join(",") );
    ui->PlainTextEdit_AuthorComment->setPlainText( _model->authorComment() );
    ui->plainTextEdit_ContAnime->setPlainText( _model->sourceVideo() );
    ui->plainTextEdit_ContMusic->setPlainText( _model->sourceMusic() );
    ui->LineEdit_Dir->setText( _model->localPath() );
    ui->LineEdit_URL->setText( _model->url() );

    _oldCover = _model->cover();

    QPixmap pm( DefinesPath::amvCovers() + _oldCover );
    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( DefinesPath::amvCovers() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

DialogAddAmv::DialogAddAmv(QWidget *parent, unsigned long long record_id) :
    QDialog(parent), ui(new Ui::DialogAddAmv), _isEditRole(true), _recordId(record_id),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL)
{
    ui->setupUi(this);
    _model = new AmvModel( QString::number( record_id ) );

    setWindowTitle( tr("Editing AMV") );
    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Amv::Geometry).toByteArray() );

    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    initTags();
    setDataInField();
}

DialogAddAmv::DialogAddAmv(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddAmv), _isEditRole(false),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL)
{
    ui->setupUi(this);
    _model = new AmvModel();

    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Amv::Geometry).toByteArray() );

    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    initTags();
}

DialogAddAmv::~DialogAddAmv()
{
    QSettings settings;
    settings.setValue(Options::Dialogs::Amv::Geometry, this->saveGeometry() );
    delete ui;
}

void DialogAddAmv::btnBox_reset()
{
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();
    ui->LineEdit_Author->clear();
    ui->LineEdit_Contestant->clear();

    ui->SpinBox_Year->setValue(2000);
    ui->LView_Tags->clearSelection();
    ui->PlainTextEdit_AuthorComment->clear();
    ui->plainTextEdit_ContAnime->clear();
    ui->plainTextEdit_ContMusic->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();

    ui->Lbl_ImageCover->noImage();
}

void DialogAddAmv::on_BtnBox_clicked(QAbstractButton *button)
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

bool DialogAddAmv::insert_Amv(){
    _model->setId( QString::number( _recordId ) );
    _model->setEditing( ! ui->CheckBox_Editing->isChecked() );
    _model->setTitle( ui->LineEdit_Title->text() );
    _model->setAuthor( ui->LineEdit_Author->text() );
    _model->setContestant( ui->LineEdit_Contestant->text() );
    _model->setScore( 0 );
    _model->setDate( QDate((ui->CBox_Year->isChecked())? ui->SpinBox_Year->value() : 0, 1, 1) );

    {
        QStringList list;
        QModelIndexList mlist = ui->LView_Tags->selectionModel()->selectedIndexes();
        for(int i = 0; i < mlist.count(); i++){
            list.append(mlist.at(i).data(Qt::DisplayRole).toString());
        }
        _model->setTags( list + ui->LineEdit_Tags->text().split(",") );
    }

    _model->setSourceMusic( ui->plainTextEdit_ContMusic->toPlainText() );
    _model->setSourceVideo( ui->plainTextEdit_ContAnime->toPlainText() );
    _model->setAuthorComment( ui->PlainTextEdit_AuthorComment->toPlainText() );
    _model->setUrl( ui->LineEdit_URL->text() );
    _model->setLocalPath( ui->LineEdit_Dir->text() );

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::amvCovers() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( DefinesPath::amvCovers() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() ){
            dir.remove( DefinesPath::amvCovers() + _oldCover );
    }
    _model->setCover( coverName );

    bool success = _model->save( ! _isEditRole );
    if( ! success ){
        QMessageBox::critical( this, tr("Critical"), tr("Cannot insert data.") );
    }
    return success;
}

void DialogAddAmv::on_BtnBox_accepted()
{
    if( ui->LineEdit_Title->text().isEmpty() == false ){
        if( !ui->LineEdit_Dir->text().isEmpty() && !QFile::exists( ui->LineEdit_Dir->text() ) ){
            QMessageBox::warning( this, tr("Warning"), tr("The field 'File' is uncorrect") );
            ui->LineEdit_Dir->setFocus();
        }else{
            insert_Amv();
            this->close();
        }
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("The field 'Title' is not filled") );
        ui->LineEdit_Title->setFocus();
    }
}

void DialogAddAmv::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddAmv::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText(
                QFileDialog::getOpenFileName(this,
                                             tr("Choose a video file"),
                                             QStandardPaths::writableLocation( QStandardPaths::MoviesLocation )
                                             )
                             );
}


void DialogAddAmv::on_LineEdit_Dir_textChanged(const QString &path)
{
    if( QFile::exists(path) == false ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}

void DialogAddAmv::on_SpinBox_Year_valueChanged(int = 0)
{
    ui->CBox_Year->setChecked( true );
}
