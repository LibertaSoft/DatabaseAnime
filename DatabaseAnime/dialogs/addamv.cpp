#include "dialogs/addamv.h"
#include "ui_addamv.h"
#include "mngrquerys.h"
#include "definespath.h"

#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QDebug>

void DialogAddAmv::initTags()
{
    _tags.setStringList( MngrQuerys::getAmvTags() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->LView_Tags->setModel( &_tags );
    ui->LView_Tags->setWrapping( true );
    ui->LView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddAmv::setDataInField()
{
    QSqlRecord record = MngrQuerys::selectData( sections::amv, _recordId );

    ui->CheckBox_Editing->setChecked( record.value( Tables::Amv::Fields::isEditingDone ).toBool() == false );
    ui->LineEdit_Title->setText( record.value( Tables::Amv::Fields::Title ).toString() );
    ui->LineEdit_Author->setText( record.value( Tables::Amv::Fields::Author ).toString() );
    ui->LineEdit_Contestant->setText( record.value( Tables::Amv::Fields::Contestant ).toString() );
    if( record.value( Tables::Amv::Fields::Year ).toInt() != 0 )
        ui->SpinBox_Year->setValue( record.value( Tables::Amv::Fields::Year ).toInt() );
    ui->LineEdit_Tags->setText( record.value( Tables::Amv::Fields::Tags ).toString() );
    ui->PlainTextEdit_AuthorComment->setPlainText( record.value( Tables::Amv::Fields::AuthorComment ).toString() );
    ui->plainTextEdit_ContAnime->setPlainText( record.value( Tables::Amv::Fields::ContainingAnime ).toString() );
    ui->plainTextEdit_ContMusic->setPlainText( record.value( Tables::Amv::Fields::ContainingMusic ).toString() );
    ui->LineEdit_Dir->setText( record.value( Tables::Amv::Fields::Dir ).toString() );
    ui->LineEdit_URL->setText( record.value( Tables::Amv::Fields::Url ).toString() );

    _oldCover = record.value( Tables::Amv::Fields::ImagePath ).toString();

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
    using namespace Tables::Amv::Fields;
    QMap<QString, QVariant> data;

    data[id]            = _recordId;
    data[isEditingDone] = !ui->CheckBox_Editing->isChecked();
    data[isAdult]       =  false;

    QRegExp rx("<.*>"); rx.setMinimal(true);
    data[Title]          = ui->LineEdit_Title->text().remove(rx);

    data[Author]        =  ui->LineEdit_Author->text();
    data[Contestant]    =  ui->LineEdit_Contestant->text();
    data[Score]         =  0;
    data[Year]          = (ui->CBox_Year->isChecked())? ui->SpinBox_Year->value() : 0;

    QString tagsList;
    QStringList list;
    QModelIndexList mlist = ui->LView_Tags->selectionModel()->selectedIndexes();
    for(int i = 0; i < mlist.count(); i++){
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

    data[Tags]            = tagsList;
    data[ContainingMusic] = ui->plainTextEdit_ContMusic->toPlainText();
    data[ContainingAnime] = ui->plainTextEdit_ContAnime->toPlainText();
    data[AuthorComment]   = ui->PlainTextEdit_AuthorComment->toPlainText();

    data[Url] = ui->LineEdit_URL->text();
    data[Dir] = ui->LineEdit_Dir->text();

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::amvCovers() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( DefinesPath::amvCovers() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() ){
            dir.remove( DefinesPath::amvCovers() + _oldCover );
    }
    data[ImagePath] = coverName;

    if( !_isEditRole ){
        if( MngrQuerys::insertAmv(data) == false ){
            QMessageBox::critical( this, tr("Critical"), tr("Cannot insert data.") );
            return false;
        }
    }else{
        if( MngrQuerys::updateAmv(data) == false ){
            QMessageBox::critical( this, tr("Critical"), tr("Cannot update data.") );
            return false;
        }
    }
    return true;
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
