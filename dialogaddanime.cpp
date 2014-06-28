#include "dialogaddanime.h"
#include "ui_dialogaddanime.h"

#include <QSettings>
#include <QFileDialog>
#include <QtSql>

#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>

DialogAddEdit::DialogAddEdit(bool isEditRole, QModelIndex* index, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::DialogAddEdit)
{

    ui->setupUi(this);
    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", false ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setMaxLength(128);
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Original title") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", false ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setMaxLength(128);
        this->LineEdit_PostScoring->setPlaceholderText( tr("Postscoring") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }
    TableModel_Tags = new QSqlTableModel;
    TableModel_Tags->setTable("AnimeTags");
    TableModel_Tags->select();

    ui->ListView_Tags->setModel( TableModel_Tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setModelColumn(1);
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );

    if( isEditRole ){
        this->isEditRole = isEditRole;
        this->recordId   = index->data().toInt();

        model = new QSqlQueryModel;
        model->setQuery( QString("SELECT * FROM animeSerials WHERE id = '%1'").arg( index->data().toInt() ) );

        ui->CheckBox_LookLater->setChecked( !model->record(0).value("isHaveLooked").toBool() );
        ui->CheckBox_Editing->setChecked( !model->record(0).value("isEditingDone").toBool() );

        ui->LineEdit_Title->setText( model->record(0).value("Title").toString() );
        // optional
        QSettings settings;
        if( settings.value( "enableElem/FieldsForEdit/OrigTitle", false ).toBool() ){
            this->LineEdit_OrigTitle->setText( model->record(0).value("OrigTitle").toString() );
        }
        if( settings.value( "enableElem/FieldsForEdit/Director",  false ).toBool() ){
            this->LineEdit_Director->setText( model->record(0).value("Director").toString() );
        }
        if( settings.value( "enableElem/FieldsForEdit/PostScoring", false ).toBool() ){
            this->LineEdit_PostScoring->setText( model->record(0).value("PostScoring").toString() );
        }

        ui->SpinBox_Year->setValue( model->record(0).value("Year").toInt() );
        ui->SpinBox_Season->setValue( model->record(0).value("Season").toInt() );
        ui->ComboBox_Studio->setCurrentIndex( model->record(0).value("Studios").toInt() );

        ui->SpinBox_aTV->setValue( model->record(0).value("SeriesTV").toInt() );
        ui->SpinBox_aOVA->setValue( model->record(0).value("SeriesOVA").toInt() );
        ui->SpinBox_aONA->setValue( model->record(0).value("SeriesONA").toInt() );
        ui->SpinBox_aSpec->setValue( model->record(0).value("SeriesSpecial").toInt() );
        ui->SpinBox_aFilm->setValue( model->record(0).value("SeriesFilm").toInt() );

        ui->SpinBox_vTV->setValue( model->record(0).value("vSeriesTV").toInt() );
        ui->SpinBox_vOVA->setValue( model->record(0).value("vSeriesOVA").toInt() );
        ui->SpinBox_vONA->setValue( model->record(0).value("vSeriesONA").toInt() );
        ui->SpinBox_vSpec->setValue( model->record(0).value("vSeriesSpecial").toInt() );
        ui->SpinBox_vFilm->setValue( model->record(0).value("vSeriesFilm").toInt() );

        ui->LineEdit_Tags->setText( model->record(0).value("Tags").toString() );
        ui->ListView_Tags->clearSelection(); // Bug, не используется, нет возможности получить модель выделения
        ui->PlainTextEdit_Description->setPlainText( model->record(0).value("Description").toString() );
        ui->LineEdit_Dir->setText( model->record(0).value("Dir").toString() );
        ui->LineEdit_URL->setText( model->record(0).value("URL").toString() );

        QPixmap pm( model->record(0).value("ImagePath").toString() );
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( model->record(0).value("ImagePath").toString() );

        oldCover = model->record(0).value("ImagePath").toString();
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
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle",   false ).toBool() ){
        this->LineEdit_OrigTitle->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/Director",    false ).toBool() ){
        this->LineEdit_Director->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", false ).toBool() ){
        this->LineEdit_PostScoring->clear();
    }

    ui->SpinBox_Year->setValue(2000);
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
        case QDialogButtonBox::AcceptRole:
            //emit on_BtnBox_accepted();
            break;
        default:
            this->close();
    }
}

bool DialogAddEdit::insert_AnimeSerials(){
    QSqlQuery query;
    if( !this->isEditRole ){
        query.prepare("INSERT INTO animeSerials("
                      "isHaveLooked, isEditingDone, Title,"
                      "OrigTitle, Director, PostScoring,"
                      "SeriesTV, SeriesOVA, SeriesONA, SeriesSpecial, SeriesFilm,"
                      "vSeriesTV, vSeriesOVA, vSeriesONA, vSeriesSpecial, vSeriesFilm,"
                      "Year, Season, Studios,"
                      "Tags, Description,"
                      "URL, Dir, ImagePath"
                      ") VALUES "
                      "(:isHaveLooked, :isEditingDone, :Title,"
                      ":OrigTitle, :Director, :PostScoring,"
                      ":SeriesTV, :SeriesOVA, :SeriesONA, :SeriesSpecial, :SeriesFilm,"
                      ":vSeriesTV, :vSeriesOVA, :vSeriesONA, :vSeriesSpecial, :vSeriesFilm,"
                      ":Year, :Season, :Studios,"
                      ":Tags, :Description,"
                      ":URL, :Dir, :ImagePath)"
                      );
    }else{
        query.prepare("UPDATE animeSerials SET "
                      "isHaveLooked = :isHaveLooked, isEditingDone = :isEditingDone, Title = :Title,"
                      "OrigTitle = :OrigTitle, Director = :Director, PostScoring = :PostScoring,"
                      "SeriesTV = :SeriesTV, SeriesOVA = :SeriesOVA, SeriesONA = :SeriesONA, SeriesSpecial = :SeriesSpecial, SeriesFilm = :SeriesFilm,"
                      "vSeriesTV = :vSeriesTV, vSeriesOVA = :vSeriesOVA, vSeriesONA = :vSeriesONA, vSeriesSpecial = :vSeriesSpecial, vSeriesFilm = :vSeriesFilm,"
                      "Year = :Year, Season = :Season, Studios = :Studios,"
                      "Tags = :Tags, Description = :Description,"
                      "URL = :URL, Dir = :Dir, ImagePath = :ImagePath WHERE id = :id;"
                      );
    }
    query.bindValue(":isHaveLooked",  !ui->CheckBox_LookLater->isChecked() );
    query.bindValue(":isEditingDone", !ui->CheckBox_Editing->isChecked() );
    query.bindValue(":id",             this->recordId );
    query.bindValue(":Title",          ui->LineEdit_Title->text() );

    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", false ).toBool() ){
        query.bindValue(":OrigTitle", this->LineEdit_OrigTitle->text() );
    }else{
        query.bindValue(":OrigTitle", "" );
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", false ).toBool() ){
        query.bindValue(":Director", this->LineEdit_Director->text() );
    }else{
        query.bindValue(":Director", "" );
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", false ).toBool() ){
        query.bindValue(":PostScoring", this->LineEdit_PostScoring->text() );
    }else{
        query.bindValue(":PostScoring", "" );
    }

    query.bindValue(":SeriesTV",      ui->SpinBox_aTV->value()   );
    query.bindValue(":SeriesOVA",     ui->SpinBox_aOVA->value()  );
    query.bindValue(":SeriesONA",     ui->SpinBox_aONA->value()  );
    query.bindValue(":SeriesSpecial", ui->SpinBox_aSpec->value() );
    query.bindValue(":SeriesFilm",    ui->SpinBox_aFilm->value() );
    query.bindValue(":vSeriesTV",     ui->SpinBox_vTV->value()   );
    query.bindValue(":vSeriesOVA",    ui->SpinBox_vOVA->value()  );
    query.bindValue(":vSeriesONA",    ui->SpinBox_vONA->value()  );
    query.bindValue(":vSeriesSpecial",ui->SpinBox_vSpec->value() );
    query.bindValue(":vSeriesFilm",   ui->SpinBox_vFilm->value() );
    query.bindValue(":Year",          ui->SpinBox_Year->value() );
    query.bindValue(":Season",        ui->SpinBox_Season->value()   );
    query.bindValue(":Studios",       ui->ComboBox_Studio->currentIndex() );

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
    if( !ui->LineEdit_Tags->text().isEmpty() ){
        tagsList += ", ";
    }
    tagsList += ui->LineEdit_Tags->text();


    query.bindValue(":Tags",          tagsList );
    query.bindValue(":Description",   ui->PlainTextEdit_Description->toPlainText() );
    query.bindValue(":URL",           ui->LineEdit_URL->text() );
    query.bindValue(":Dir",           ui->LineEdit_Dir->text() );

    QDir objQdir;
    QString coverPath( QDir::homePath() + "/."+QApplication::organizationName()+"/"+QApplication::applicationName() + "/animeCovers/" );
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
        qDebug() << "Cannot insert data in table animeSerials: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}

void DialogAddEdit::on_BtnBox_accepted()
{
    if( !ui->LineEdit_Title->text().isEmpty() ){
        insert_AnimeSerials();
        this->close();
    }else{
        QMessageBox::information( this, tr("Warning"), tr("The field 'Title' is not filled") );
    }
}

void DialogAddEdit::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddEdit::on_SpinBox_aTV_valueChanged(int arg1)
{
    ui->SpinBox_vTV->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aOVA_valueChanged(int arg1)
{
    ui->SpinBox_vOVA->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aONA_valueChanged(int arg1)
{
    ui->SpinBox_vONA->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aSpec_valueChanged(int arg1)
{
    ui->SpinBox_vSpec->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aFilm_valueChanged(int arg1)
{
    ui->SpinBox_vFilm->setMaximum(arg1);
}

void DialogAddEdit::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText( QFileDialog::getExistingDirectory(this, tr("Choose a directory with video files"), ui->LineEdit_Dir->text() ) );
}
