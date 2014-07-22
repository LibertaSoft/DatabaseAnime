#include "dialogaddmanga.h"
#include "ui_dialogaddmanga.h"
#include "mngrquerys.h"

#include <QSettings>
#include <QFileDialog>
#include <QtSql>

#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>
#include <QDesktopServices>

#include <QtNetwork/QNetworkRequest>

void DialogAddManga::initTags()
{
    tags.setStringList( MngrQuerys::getMangaTags() );
}

DialogAddManga::DialogAddManga(bool _isEditRole, QModelIndex* index, QWidget *parent ) :
    QDialog(parent), ui(new Ui::DialogAddManga), isEditRole( _isEditRole )
{
    ui->setupUi(this);
    QSettings settings;
    initTags();

    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);

    if( settings.value( "optionalField/manga/AltTitle", false ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setMaxLength(128);
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( "optionalField/manga/Director", false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( "optionalField/manga/Translation", false ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setMaxLength(128);
        this->LineEdit_PostScoring->setPlaceholderText( tr("Translation") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }

    ui->ListView_Tags->setModel( &tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );

    if( isEditRole ){
        this->isEditRole = isEditRole;
        this->recordId   = index->data().toInt();

        model = new QSqlQueryModel;
        model->setQuery( QString("SELECT * FROM %1 WHERE id = '%2'").arg(
                             MngrQuerys::getTableName( sections::manga ) ).arg( this->recordId ) );

        ui->CheckBox_LookLater->setChecked( !model->record(0).value("isHaveLooked").toBool() );
        ui->CheckBox_Editing->setChecked( !model->record(0).value("isEditingDone").toBool() );

        ui->LineEdit_Title->setText( model->record(0).value("Title").toString() );

        // Optional Fields
        QSettings settings;
        if( settings.value( "optionalField/manga/AltTitle", false ).toBool() ){
            this->LineEdit_OrigTitle->setText( model->record(0).value("AltTitle").toString() );
        }
        if( settings.value( "optionalField/manga/Director",  false ).toBool() ){
            this->LineEdit_Director->setText( model->record(0).value("Director").toString() );
        }
        if( settings.value( "optionalField/manga/Translation", false ).toBool() ){
            this->LineEdit_PostScoring->setText( model->record(0).value("Translation").toString() );
        }

        ui->SpinBox_Year->setValue( model->record(0).value("Year").toInt() );

        ui->SpinBox_aVol->setValue( model->record(0).value("Vol").toInt() );
        ui->SpinBox_aCh->setValue( model->record(0).value("Ch").toInt() );
        ui->SpinBox_aPages->setValue( model->record(0).value("Pages").toInt() );

        ui->SpinBox_vVol->setValue( model->record(0).value("vVol").toInt() );
        ui->SpinBox_vCh->setValue( model->record(0).value("vCh").toInt() );
        ui->SpinBox_vPages->setValue( model->record(0).value("vPages").toInt() );

        ui->LineEdit_Tags->setText( model->record(0).value("Tags").toString() );
        ui->ListView_Tags->clearSelection(); // Bug, не используется, нет возможности получить модель выделения
        ui->PlainTextEdit_Description->setPlainText( model->record(0).value("Description").toString() );
        ui->LineEdit_Dir->setText( model->record(0).value("Dir").toString() );
        ui->LineEdit_URL->setText( model->record(0).value("URL").toString() );


        QString imgPath = model->record(0).value("ImagePath").toString();
        QPixmap pm( imgPath );
        bool imageCrash = false;
        if( pm.isNull() ){
            pm.load( "://images/NoImage.png" );
            imageCrash = true;
        }
        if( !imageCrash ){
            ui->Lbl_ImageCover->setPixmap( pm );
            ui->Lbl_ImageCover->setImagePath( model->record(0).value("ImagePath").toString() );
        }else{
            ui->Lbl_ImageCover->noImage();
        }

        oldCover = model->record(0).value("ImagePath").toString();
    }
}

DialogAddManga::~DialogAddManga()
{
    delete ui;
}

void DialogAddManga::on_BtnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();
    // optional
    QSettings settings;
    if( settings.value( "optionalField/manga/AltTitle",   false ).toBool() ){
        this->LineEdit_OrigTitle->clear();
    }
    if( settings.value( "optionalField/manga/Director",    false ).toBool() ){
        this->LineEdit_Director->clear();
    }
    if( settings.value( "optionalField/manga/Translation", false ).toBool() ){
        this->LineEdit_PostScoring->clear();
    }

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
}

void DialogAddManga::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case 7:
            emit on_BtnBox_reset();
            break;
        case QDialogButtonBox::AcceptRole:
            //emit on_BtnBox_accepted();
            break;
        default:
            this->close();
    }
}

bool DialogAddManga::insert_MangaPosters(){
    QSqlQuery query;
    if( !this->isEditRole ){
        query.prepare( QString("INSERT INTO %1("
                      "isHaveLooked, isEditingDone, Title,"
                      "AltTitle, Director, Translation,"
                      "Vol, Ch, Pages,"
                      "vVol, vCh, vPages,"
                      "Year,"
                      "Tags, Description,"
                      "URL, Dir, ImagePath"
                      ") VALUES "
                      "(:isHaveLooked, :isEditingDone, :Title,"
                      ":AltTitle, :Director, :Translation,"
                      ":Vol, :Ch, :Pages,"
                      ":vVol, :vCh, :vPages,"
                      ":Year,"
                      ":Tags, :Description,"
                      ":URL, :Dir, :ImagePath)"
                      ).arg( MngrQuerys::getTableName( sections::manga ) ) );
    }else{
        query.prepare( QString("UPDATE %1 SET "
                      "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, Title = :Title,"
                      "AltTitle = :AltTitle, Director = :Director, Translation = :Translation,"
                      "Vol = :Vol, Ch = :Ch, Pages = :Pages,"
                      "vVol = :vVol, vCh = :vCh, vPages = :vPages,"
                      "Year = :Year,"
                      "Tags = :Tags, Description = :Description,"
                      "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;").arg(
                           MngrQuerys::getTableName( sections::manga ) )
                      );
    }
    query.bindValue( ":isHaveLooked",  !ui->CheckBox_LookLater->isChecked() );
    query.bindValue( ":isEditingDone", !ui->CheckBox_Editing->isChecked() );
    query.bindValue( ":id",            this->recordId );
    query.bindValue( ":Title",         ui->LineEdit_Title->text() );

    QSettings settings;
    if( settings.value( "optionalField/manga/AltTitle", false ).toBool() ){
        query.bindValue( ":AltTitle", this->LineEdit_OrigTitle->text() );
    }else{
        query.bindValue( ":AltTitle", "" );
    }
    if( settings.value( "optionalField/manga/Director", false ).toBool() ){
        query.bindValue( ":Director", this->LineEdit_Director->text() );
    }else{
        query.bindValue( ":Director", "" );
    }
    if( settings.value( "optionalField/manga/Translation", false ).toBool() ){
        query.bindValue( ":Translation", this->LineEdit_PostScoring->text() );
    }else{
        query.bindValue( ":Translation", "" );
    }

    query.bindValue(":Vol",    ui->SpinBox_aVol->value()   );
    query.bindValue(":Ch",     ui->SpinBox_aCh->value()  );
    query.bindValue(":Pages",  ui->SpinBox_aPages->value()  );
    query.bindValue(":vVol",   ui->SpinBox_vVol->value()   );
    query.bindValue(":vCh",    ui->SpinBox_vCh->value()  );
    query.bindValue(":vPages", ui->SpinBox_vPages->value()  );
    query.bindValue(":Year",   ui->SpinBox_Year->value() );

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
    query.bindValue( ":URL",           ui->LineEdit_URL->text() );
    query.bindValue( ":Dir",           ui->LineEdit_Dir->text() );

    QDir objQdir;
    QString coverPath( QDir::homePath() + "/."+QApplication::organizationName()+"/"+QApplication::applicationName() + "/mangaCovers/" );
    if( objQdir.mkpath( coverPath ) ){
        QDateTime dt;
        coverPath += "/" + QString::number( dt.currentMSecsSinceEpoch() );
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( coverPath );
    }
    if( isEditRole ){
            objQdir.remove( oldCover );
    }
    query.bindValue(":ImagePath", coverPath );
    if( !query.exec() ){
        qDebug() << "Cannot insert data in table mangaPosters: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}

void DialogAddManga::on_BtnBox_accepted()
{
    QDir dir( ui->LineEdit_Dir->text() );
    if( !ui->LineEdit_Title->text().isEmpty() ){
        if( !dir.exists() ){
            QMessageBox::warning( this, tr("Warning"), tr("The field 'Dir' is uncorrect") );
            ui->LineEdit_Dir->setFocus();
        }else{
            insert_MangaPosters(); // #Bug : what is it? //298
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

void DialogAddManga::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText(
                QFileDialog::getExistingDirectory(this,
                                                  tr("Choose a directory with video files"),
                                                  QStandardPaths::writableLocation( QStandardPaths::PicturesLocation )
                                                  ) );
}


void DialogAddManga::on_LineEdit_Dir_textChanged(const QString &path)
{
    QDir dir( path );
    if( !dir.exists() ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}
