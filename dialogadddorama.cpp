#include "dialogadddorama.h"
#include "ui_dialogadddorama.h"
#include "mngrquerys.h"

#include <QSettings>
#include <QFileDialog>
#include <QtSql>

#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>
#include <QDesktopServices>

#include <QtNetwork/QNetworkRequest>

void DialogAddDorama::initTags()
{
    _tags.setStringList( MngrQuerys::getDoramaTags() );
    ui->ListView_Tags->setModel( &_tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddDorama::initOptionalFields()
{
    QSettings settings;
    if( settings.value( "optionalField/dorama/AltTitle", false ).toBool() ){
        this->LineEdit_AltTitle = new QLineEdit(this);
        this->LineEdit_AltTitle->setMaxLength(128);
        this->LineEdit_AltTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_AltTitle );
    }
    if( settings.value( "optionalField/dorama/Director", false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_Director );
    }
}

void DialogAddDorama::setDataInField()
{
    model = new QSqlQueryModel(this);
    model->setQuery( QString("SELECT * FROM %1 WHERE id = '%2'").arg(
                         MngrQuerys::getTableName( sections::dorama ) ).arg( _recordId ) );

    ui->CheckBox_LookLater->setChecked( !model->record(0).value("isHaveLooked").toBool() );
    ui->CheckBox_Editing->setChecked( !model->record(0).value("isEditingDone").toBool() );

    ui->LineEdit_Title->setText( model->record(0).value("Title").toString() );

    // Optional Fields
    if( this->LineEdit_AltTitle ){
        this->LineEdit_AltTitle->setText( model->record(0).value("AltTitle").toString() );
    }
    if( this->LineEdit_Director ){
        this->LineEdit_Director->setText( model->record(0).value("Director").toString() );
    }

    ui->SpinBox_Year->setValue( model->record(0).value("Year").toInt() );
    ui->SpinBox_Season->setValue( model->record(0).value("Season").toInt() );

    ui->SpinBox_aTV->setValue( model->record(0).value("SeriesTV").toInt() );
    ui->SpinBox_aSpec->setValue( model->record(0).value("SeriesSpecial").toInt() );
    ui->SpinBox_aMovie->setValue( model->record(0).value("SeriesMovie").toInt() );

    ui->SpinBox_vTV->setValue( model->record(0).value("vSeriesTV").toInt() );
    ui->SpinBox_vSpec->setValue( model->record(0).value("vSeriesSpecial").toInt() );
    ui->SpinBox_vMovie->setValue( model->record(0).value("vSeriesMovie").toInt() );

    ui->LineEdit_Tags->setText( model->record(0).value("Tags").toString() );
    ui->PlainTextEdit_Description->setPlainText( model->record(0).value("Description").toString() );
    ui->LineEdit_Dir->setText( model->record(0).value("Dir").toString() );
    ui->LineEdit_URL->setText( model->record(0).value("URL").toString() );

    QString imgPath = model->record(0).value("ImagePath").toString();
    QPixmap pm( imgPath );
    bool imageCrash = false;
    if( pm.isNull() ){ // #FixMe
        pm.load( "://images/NoImage.png" );
        imageCrash = true;
    }
    if( !imageCrash ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( model->record(0).value("ImagePath").toString() );
    }else{
        ui->Lbl_ImageCover->noImage();
    }

    _oldCover = model->record(0).value("ImagePath").toString();
}

DialogAddDorama::DialogAddDorama(QWidget *parent, unsigned int record_id) :
    QDialog(parent), ui(new Ui::DialogAddDorama), _isEditRole(true), _recordId(record_id),
    LineEdit_AltTitle(NULL), LineEdit_Director(NULL)
{
    ui->setupUi(this);
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);

    initTags();
    initOptionalFields();
    setDataInField();
}

DialogAddDorama::DialogAddDorama(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddDorama), _isEditRole(false),
    LineEdit_AltTitle(NULL), LineEdit_Director(NULL)
{
    ui->setupUi(this);
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);

    initTags();
    initOptionalFields();
}

DialogAddDorama::~DialogAddDorama()
{
    delete ui;
}

void DialogAddDorama::on_BtnBox_reset()
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
            on_BtnBox_reset();
            break;
        case QDialogButtonBox::AcceptRole:
            //on_BtnBox_accepted();
            break;
        default:
            this->close();
    }
}

bool DialogAddDorama::insert_Dorama(){
    QSqlQuery query;
    if( !_isEditRole ){
        query.prepare( QString("INSERT INTO %1("
                      "isHaveLooked, isEditingDone, Title,"
                      "AltTitle, Director,"
                      "SeriesTV, SeriesSpecial, SeriesMovie,"
                      "vSeriesTV, vSeriesSpecial, vSeriesMovie,"
                      "Year, Season,"
                      "Tags, Description, Actors,"
                      "URL, Dir, ImagePath"
                      ") VALUES "
                      "(:isHaveLooked, :isEditingDone, :Title,"
                      ":AltTitle, :Director,"
                      ":SeriesTV, :SeriesSpecial, :SeriesMovie,"
                      ":vSeriesTV, :vSeriesSpecial, :vSeriesMovie,"
                      ":Year, :Season,"
                      ":Tags, :Description, :Actors,"
                      ":URL, :Dir, :ImagePath)"
                      ).arg( MngrQuerys::getTableName( sections::dorama ) ) );
    }else{
        query.prepare( QString("UPDATE %1 SET "
                      "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, Title = :Title,"
                      "AltTitle = :AltTitle, Director = :Director,"
                      "SeriesTV = :SeriesTV, SeriesSpecial = :SeriesSpecial, SeriesMovie = :SeriesMovie,"
                      "vSeriesTV = :vSeriesTV, vSeriesSpecial = :vSeriesSpecial, vSeriesMovie = :vSeriesMovie,"
                      "Year = :Year, Season = :Season,"
                      "Tags = :Tags, Description = :Description, Actors = :Actors,"
                      "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;").arg( MngrQuerys::getTableName( sections::dorama ) )
                      );
    }
    query.bindValue( ":isHaveLooked",  !ui->CheckBox_LookLater->isChecked() );
    query.bindValue( ":isEditingDone", !ui->CheckBox_Editing->isChecked() );
    query.bindValue( ":id",            _recordId );
    query.bindValue( ":Title",         ui->LineEdit_Title->text() );
    query.bindValue( ":AltTitle",      (LineEdit_AltTitle)?this->LineEdit_AltTitle->text():"" );
    query.bindValue( ":Director",      (LineEdit_Director)?this->LineEdit_Director->text():"" );
    query.bindValue(":SeriesTV",       ui->SpinBox_aTV->value()   );
    query.bindValue(":SeriesSpecial",  ui->SpinBox_aSpec->value() );
    query.bindValue(":SeriesMovie",    ui->SpinBox_aMovie->value());
    query.bindValue(":vSeriesTV",      ui->SpinBox_vTV->value()   );
    query.bindValue(":vSeriesSpecial", ui->SpinBox_vSpec->value() );
    query.bindValue(":vSeriesMovie",   ui->SpinBox_vMovie->value());
    query.bindValue(":Year",           ui->SpinBox_Year->value()  );
    query.bindValue(":Season",         ui->SpinBox_Season->value());

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

    query.bindValue( ":Tags",          tagsList );
    query.bindValue( ":Description",   ui->PlainTextEdit_Description->toPlainText() );
    query.bindValue( ":Actors",        ui->PlainTextEdit_Actors->toPlainText() );
    query.bindValue( ":URL",           ui->LineEdit_URL->text() );
    query.bindValue( ":Dir",           ui->LineEdit_Dir->text() );

    QString coverPath( QDir::homePath() + "/."
                       + QApplication::organizationName()
                       + "/"
                       + QApplication::applicationName()
                       + "/doramaCovers/" );
    QDir dir;
    if( dir.mkpath( coverPath ) ){
        QDateTime dt;
        coverPath += "/" + QString::number( dt.currentMSecsSinceEpoch() );
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( coverPath );
    }
    if( _isEditRole ){
            dir.remove( _oldCover );
    }
    query.bindValue(":ImagePath", coverPath );
    if( !query.exec() ){
        qDebug() << QString("Cannot insert data in table %1: ").arg(
                        MngrQuerys::getTableName( sections::dorama ) ) << query.lastError();
        QMessageBox::warning(this, tr("Warning"), tr("Cannot insert data."));
        QMessageBox::information(this, tr("Warning"), query.executedQuery() );
        return false;
    }
    return true;
}

void DialogAddDorama::on_BtnBox_accepted()
{
    QDir dir( ui->LineEdit_Dir->text() );
    if( !ui->LineEdit_Title->text().isEmpty() ){
        if( !dir.exists() ){
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
    QDir dir( path );
    if( !dir.exists() ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}
