#include "dialogaddamv.h"
#include "ui_dialogaddamv.h"
#include "mngrquerys.h"

#include <QSettings>
#include <QFileDialog>
#include <QDesktopServices>

#include <QMessageBox>
#include <QDebug>

void DialogAddAmv::initTags()
{
    _tags.setStringList( MngrQuerys::getAmvTags() );
    ui->LView_Tags->setModel( &_tags );
    ui->LView_Tags->setWrapping( true );
    ui->LView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddAmv::setDataInField()
{
    model = new QSqlQueryModel(this);
    model->setQuery( QString("SELECT * FROM %1 WHERE id = '%2'").arg(
                         MngrQuerys::getTableName( sections::amv ) ).arg( _recordId ) );

    ui->CheckBox_Editing->setChecked( !model->record(0).value("isEditingDone").toBool() );
    ui->LineEdit_Title->setText( model->record(0).value("Title").toString() );
    ui->LineEdit_Author->setText( model->record(0).value("Author").toString() );
    ui->LineEdit_Contestant->setText( model->record(0).value("Сontestant").toString() );
    if( model->record(0).value("Year").toInt() != 0 )
        ui->SpinBox_Year->setValue( model->record(0).value("Year").toInt() );
    ui->LineEdit_Tags->setText( model->record(0).value("Tags").toString() );
    ui->PlainTextEdit_AuthorComment->setPlainText( model->record(0).value("AuthorComment").toString() );
    ui->plainTextEdit_ContAnime->setPlainText( model->record(0).value("ContainingAnime").toString() );
    ui->plainTextEdit_ContMusic->setPlainText( model->record(0).value("ContainingMusic").toString() );
    ui->LineEdit_Dir->setText( model->record(0).value("Dir").toString() );
    ui->LineEdit_URL->setText( model->record(0).value("URL").toString() );

    _oldCover = model->record(0).value("ImagePath").toString();

    QPixmap pm( MngrQuerys::getAmvCoversPath() + _oldCover );
    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( MngrQuerys::getAmvCoversPath() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

DialogAddAmv::DialogAddAmv(QWidget *parent, unsigned int record_id) :
    QDialog(parent), ui(new Ui::DialogAddAmv), _isEditRole(true), _recordId(record_id),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL)
{
    ui->setupUi(this);
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
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    initTags();
}

DialogAddAmv::~DialogAddAmv()
{
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
    QSqlQuery query;
    if( !_isEditRole ){
        query.prepare( QString("INSERT INTO %1("
                      "isEditingDone, Title,"
                      "Author, Сontestant,"
                      "Year, Tags,"
                      "ContainingMusic, ContainingAnime, AuthorComment,"
                      "URL, Dir, ImagePath"
                      ") VALUES ("
                      ":isEditingDone, :Title,"
                      ":Author, :Concursant,"
                      ":Year, :Tags,"
                      ":ContainingMusic, :ContainingAnime, :AuthorComment,"
                      ":URL, :Dir, :ImagePath)"
                      ).arg( MngrQuerys::getTableName( sections::amv ) ) );
    }else{
        query.prepare( QString("UPDATE %1 SET "
                      "isEditingDone = :isEditingDone, Title = :Title,"
                      "Author = :Author, Сontestant = :Concursant,"
                      "Year = :Year, Tags = :Tags,"
                      "ContainingMusic = :ContainingMusic, ContainingAnime = :ContainingAnime, AuthorComment = :AuthorComment,"
                      "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;"
                      ).arg( MngrQuerys::getTableName( sections::amv ) ) );
    }
    query.bindValue( ":id",             _recordId );
    query.bindValue( ":isEditingDone", !ui->CheckBox_Editing->isChecked() );
    query.bindValue( ":Title",          ui->LineEdit_Title->text() );

    query.bindValue( ":Author",         ui->LineEdit_Author->text() );
    query.bindValue( ":Concursant",     ui->LineEdit_Contestant->text() );
    query.bindValue( ":Year",          (ui->CBox_Year->isChecked())? ui->SpinBox_Year->value() : 0);

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

    query.bindValue( ":Tags",           tagsList );
    query.bindValue( ":ContainingMusic",ui->plainTextEdit_ContMusic->toPlainText() );
    query.bindValue( ":ContainingAnime",ui->plainTextEdit_ContAnime->toPlainText() );
    query.bindValue( ":AuthorComment",  ui->PlainTextEdit_AuthorComment->toPlainText() );

    query.bindValue( ":URL",           ui->LineEdit_URL->text() );
    query.bindValue( ":Dir",           ui->LineEdit_Dir->text() );

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( dir.mkpath( MngrQuerys::getAmvCoversPath() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( MngrQuerys::getAmvCoversPath() + coverName );
    }
    if( _isEditRole ){
            dir.remove( MngrQuerys::getAmvCoversPath() + _oldCover );
    }
    query.bindValue(":ImagePath", coverName );


    if( !query.exec() ){
        qDebug() << QString("Cannot insert data in table %1: ").arg(
                        MngrQuerys::getTableName( sections::amv ) ) << query.lastError();
        QMessageBox::warning( this, tr("Warning"), tr("Cannot insert data.") );
        return false;
    }
    return true;
}

void DialogAddAmv::on_BtnBox_accepted()
{
    QFile amvFile( ui->LineEdit_Dir->text() );
    if( !ui->LineEdit_Title->text().isEmpty() ){
        if( !ui->LineEdit_Dir->text().isEmpty() && !amvFile.exists() ){
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
                                             ) );
}


void DialogAddAmv::on_LineEdit_Dir_textChanged(const QString &path)
{
    QFile amvFile( path );
    if( !amvFile.exists() ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}

void DialogAddAmv::on_SpinBox_Year_valueChanged(int = 0)
{
    ui->CBox_Year->setChecked( true );
}
