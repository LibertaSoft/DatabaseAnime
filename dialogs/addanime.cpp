#include "dialogs/addanime.h"
#include "ui_addanime.h"
#include "mngrquerys.h"
#include "definespath.h"

#include <QSettings>
#include <QFileDialog>
#include <QDesktopServices>

#include <QImageReader>
//#include <QPicture>

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>

#include <QCompleter>
#include <QMessageBox>
#include <QDebug>


void DialogAddAnime::initTags()
{
    _tags.setStringList( MngrQuerys::getAnimeTags() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->ListView_Tags->setModel( &_tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddAnime::initOptionalFields()
{
    QSettings settings;
    if( settings.value( "optionalField/anime/OrigTitle", false ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setMaxLength(128);
        this->LineEdit_OrigTitle->setDragEnabled(true);
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( "optionalField/anime/Director", false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setDragEnabled(true);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( "optionalField/anime/PostScoring", false ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setMaxLength(128);
        this->LineEdit_PostScoring->setDragEnabled(true);
        this->LineEdit_PostScoring->setPlaceholderText( tr("Postscoring") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }
}

void DialogAddAnime::setDataInField()
{
    model = new QSqlQueryModel(this);
    model->setQuery( QString("SELECT * FROM %1 WHERE id = '%2'").arg(
                         MngrQuerys::getTableName( sections::anime ) ).arg( _recordId ) );

    ui->CheckBox_LookLater->setChecked( !model->record(0).value("isHaveLooked").toBool() );
    ui->CheckBox_Editing->setChecked( !model->record(0).value("isEditingDone").toBool() );

    ui->LineEdit_Title->setText( model->record(0).value("Title").toString() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( model->record(0).value("OrigTitle").toString() );
    if( this->LineEdit_Director )
        this->LineEdit_Director->setText( model->record(0).value("Director").toString() );
    if( this->LineEdit_PostScoring )
        this->LineEdit_PostScoring->setText( model->record(0).value("PostScoring").toString() );
    if( model->record(0).value("Year").toInt() != 0 )
        ui->SpinBox_Year->setValue( model->record(0).value("Year").toInt() );
    ui->SpinBox_Season->setValue( model->record(0).value("Season").toInt() );
    ui->ComboBox_Studio->setCurrentText( model->record(0).value("Studios").toString() );

    ui->SpinBox_aTV->setValue(    model->record(0).value("SeriesTV"     ).toInt() );
    ui->SpinBox_aOVA->setValue(   model->record(0).value("SeriesOVA"    ).toInt() );
    ui->SpinBox_aONA->setValue(   model->record(0).value("SeriesONA"    ).toInt() );
    ui->SpinBox_aSpec->setValue(  model->record(0).value("SeriesSpecial").toInt() );
    ui->SpinBox_aMovie->setValue( model->record(0).value("SeriesMovie"  ).toInt() );

    ui->SpinBox_vTV->setValue(    model->record(0).value("vSeriesTV"     ).toInt() );
    ui->SpinBox_vOVA->setValue(   model->record(0).value("vSeriesOVA"    ).toInt() );
    ui->SpinBox_vONA->setValue(   model->record(0).value("vSeriesONA"    ).toInt() );
    ui->SpinBox_vSpec->setValue(  model->record(0).value("vSeriesSpecial").toInt() );
    ui->SpinBox_vMovie->setValue( model->record(0).value("vSeriesMovie"  ).toInt() );

    ui->LineEdit_Tags->setText( model->record(0).value("Tags").toString() );
    ui->PlainTextEdit_Description->setPlainText( model->record(0).value("Description").toString() );
    ui->LineEdit_Dir->setText( model->record(0).value("Dir").toString() );
    ui->LineEdit_URL->setText( model->record(0).value("URL").toString() );

    _oldCover = model->record(0).value("ImagePath").toString();

    QPixmap pm( DefinesPath::animeCovers() + _oldCover );

    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( DefinesPath::animeCovers() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

void DialogAddAnime::setTabOrders()
{
    if( this->LineEdit_OrigTitle  ){
        setTabOrder(ui->LineEdit_Title, this->LineEdit_OrigTitle);
    }
    if( this->LineEdit_Director  ){
        if( this->LineEdit_OrigTitle  ){
            setTabOrder(this->LineEdit_OrigTitle, this->LineEdit_Director);
        }else{
            setTabOrder(ui->LineEdit_Title, this->LineEdit_Director);
        }
    }
    if( this->LineEdit_PostScoring ){
        if( this->LineEdit_Director ){
            setTabOrder(this->LineEdit_Director, this->LineEdit_PostScoring);
        }else{
            if( this->LineEdit_OrigTitle ){
                setTabOrder(this->LineEdit_OrigTitle, this->LineEdit_PostScoring);
            }else{
                setTabOrder(ui->LineEdit_Title, this->LineEdit_PostScoring);
            }
        }
    }
}

DialogAddAnime::DialogAddAnime(QWidget *parent, unsigned long long record_id) :
    QDialog(parent), ui(new Ui::DialogAddAnime), _isEditRole(true), _recordId(record_id),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL), TitleCompliter(NULL)
{
    ui->setupUi(this);
    QSettings settings;
    this->restoreGeometry( settings.value("DialogAddAnime/Geometry").toByteArray() );

    // Reset tabs
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    // Init QCompliter
    TitleCompliter = new QCompleter( this );
    TitleCompliter->setModel(&_titleCompliterModel);
    TitleCompliter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->LineEdit_Title->setCompleter( TitleCompliter );

    initTags();
    initOptionalFields();
    setTabOrders();
    setDataInField();
}

DialogAddAnime::DialogAddAnime(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddAnime), _isEditRole(false),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL), TitleCompliter(NULL)
{
    ui->setupUi(this);
    QSettings settings;
    this->restoreGeometry( settings.value("DialogAddAnime/Geometry").toByteArray() );

    // Reset tabs
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    // Init QCompliter
    TitleCompliter = new QCompleter( this );
    TitleCompliter->setModel(&_titleCompliterModel);
    TitleCompliter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->LineEdit_Title->setCompleter( TitleCompliter );

    initTags();
    initOptionalFields();
    setTabOrders();
}

DialogAddAnime::~DialogAddAnime()
{
    QSettings settings;
    settings.setValue("DialogAddAnime/Geometry", this->saveGeometry() );
    delete ui;
}

void DialogAddAnime::btnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();
    // optional
    if( this->LineEdit_OrigTitle ){
        this->LineEdit_OrigTitle->clear();
    }
    if( this->LineEdit_Director ){
        this->LineEdit_Director->clear();
    }
    if( this->LineEdit_PostScoring ){
        this->LineEdit_PostScoring->clear();
    }

    ui->SpinBox_Year->setValue(2000);
    ui->SpinBox_Season->setValue(0);
    ui->ComboBox_Studio->setCurrentIndex(0);

    ui->SpinBox_aTV->setValue(0);
    ui->SpinBox_aOVA->setValue(0);
    ui->SpinBox_aONA->setValue(0);
    ui->SpinBox_aSpec->setValue(0);
    ui->SpinBox_aMovie->setValue(0);

    ui->SpinBox_vTV->setValue(0);
    ui->SpinBox_vOVA->setValue(0);
    ui->SpinBox_vONA->setValue(0);
    ui->SpinBox_vSpec->setValue(0);
    ui->SpinBox_vMovie->setValue(0);

    ui->LineEdit_Tags->clear();
    ui->ListView_Tags->clearSelection();
    ui->PlainTextEdit_Description->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();

    ui->Lbl_ImageCover->noImage();
}

void DialogAddAnime::on_BtnBox_clicked(QAbstractButton *button)
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

bool DialogAddAnime::insert_Anime(){
    QMap<QString, QVariant> data;
    data["isHaveLooked"]   = !ui->CheckBox_LookLater->isChecked();
    data["isEditingDone"]  = !ui->CheckBox_Editing->isChecked();
    data["isAdult"]        = false;
    data["id"]             = _recordId;

    QRegExp rx("<.*>"); rx.setMinimal(true);
    data["Title"]          = ui->LineEdit_Title->text().remove(rx);

    data["OrigTitle"]      = (LineEdit_OrigTitle  )?  this->LineEdit_OrigTitle->text() : "";
    data["Director"]       = (LineEdit_Director   )?   this->LineEdit_Director->text() : "";
    data["PostScoring"]    = (LineEdit_PostScoring)?this->LineEdit_PostScoring->text() : "";
    data["SeriesTV"]       = ui->SpinBox_aTV->value();
    data["SeriesOVA"]      = ui->SpinBox_aOVA->value();
    data["SeriesONA"]      = ui->SpinBox_aONA->value();
    data["SeriesSpecial"]  = ui->SpinBox_aSpec->value();
    data["SeriesMovie"]    = ui->SpinBox_aMovie->value();
    data["vSeriesTV"]      = ui->SpinBox_vTV->value();
    data["vSeriesOVA"]     = ui->SpinBox_vOVA->value();
    data["vSeriesONA"]     = ui->SpinBox_vONA->value();
    data["vSeriesSpecial"] = ui->SpinBox_vSpec->value();
    data["vSeriesMovie"]   = ui->SpinBox_vMovie->value();
    data["Score"]          = 0;
    data["Year"]           = (ui->CBox_Year->isChecked()  )? ui->SpinBox_Year->value() :0;
    data["Season"]         = ui->SpinBox_Season->value();
    data["Studios"]        = ui->ComboBox_Studio->currentText();

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

    data["Tags"]        = tagsList;
    data["Description"] = ui->PlainTextEdit_Description->toPlainText();
    data["URL"]         = ui->LineEdit_URL->text();
    data["Dir"]         = ui->LineEdit_Dir->text();

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::animeCovers() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( DefinesPath::animeCovers() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() ){
            dir.remove( DefinesPath::animeCovers() + _oldCover );
    }

    data["ImagePath"] = coverName;

    if( !_isEditRole ){
        if( MngrQuerys::insertAnime(data) == false ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot insert data."));
            return false;
        }
    }else{
        if( MngrQuerys::updateAnime(data) == false ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot update data."));
            return false;
        }
    }
    return true;
}

void DialogAddAnime::on_BtnBox_accepted()
{
    if( ui->LineEdit_Title->text().isEmpty() == false ){
        if( QDir( ui->LineEdit_Dir->text() ).exists() == false ){
            QMessageBox::warning( this, tr("Warning"), tr("The field 'Directory' is uncorrect") );
            ui->LineEdit_Dir->setFocus();
        }else{
            insert_Anime();
            this->close();
        }
    }else{
        QMessageBox::warning( this, tr("Warning"), tr("The field 'Title' is not filled") );
        ui->LineEdit_Title->setFocus();
    }
}

void DialogAddAnime::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddAnime::on_SpinBox_aTV_valueChanged(int value)
{
    ui->SpinBox_vTV->setMaximum(value);
}

void DialogAddAnime::on_SpinBox_aOVA_valueChanged(int value)
{
    ui->SpinBox_vOVA->setMaximum(value);
}

void DialogAddAnime::on_SpinBox_aONA_valueChanged(int value)
{
    ui->SpinBox_vONA->setMaximum(value);
}

void DialogAddAnime::on_SpinBox_aSpec_valueChanged(int value)
{
    ui->SpinBox_vSpec->setMaximum(value);
}

void DialogAddAnime::on_SpinBox_aMovie_valueChanged(int value)
{
    ui->SpinBox_vMovie->setMaximum(value);
}

void DialogAddAnime::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText(
                QFileDialog::getExistingDirectory(this,
                                                  tr("Choose a directory with video files"),
                                                  QStandardPaths::writableLocation( QStandardPaths::MoviesLocation )
                                                  ) );
}


void DialogAddAnime::on_LineEdit_Dir_textChanged(const QString &path)
{
    if( QDir(path).exists() == false ){
        ui->LineEdit_Dir->setStyleSheet("color:red");
    }else{
        ui->LineEdit_Dir->setStyleSheet("color:black");
    }
}

void DialogAddAnime::on_SpinBox_Year_valueChanged(int = 0)
{
    ui->CBox_Year->setChecked( true );
}

void DialogAddAnime::on_TBtn_Search_clicked()
{
    QUrl url("http://shikimori.org/api/animes?limit=1&search="+ui->LineEdit_Title->text() );
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyLastSearchFinished(QNetworkReply*)));

    manager->get( QNetworkRequest(url) );
}

void DialogAddAnime::on_LineEdit_Title_textEdited(const QString &title)
{
    QUrl url("http://shikimori.org/api/animes?limit=10&search="+title);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replySearchFinished(QNetworkReply*)));

    manager->get( QNetworkRequest(url) );
}

void DialogAddAnime::replySearchFinished(QNetworkReply *r)
{
    QByteArray data = r->readAll();

    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();

    int k(0);
    QStringList animeList;
    for( QJsonArray::iterator i = arr.begin(); i != arr.end(); ++i ){
        QJsonObject obj = arr.at(k).toObject();
        animeList.append( obj["name"].toString() );
        ++k;
    }

    _titleCompliterModel.setStringList( animeList );

    r->deleteLater();
}

void DialogAddAnime::replyLastSearchFinished(QNetworkReply *r)
{
    ui->TabWidget_Info->setCurrentIndex(2);
    QByteArray data = r->readAll();

    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonArray arr = doc.array();

    QJsonObject obj = arr.at(0).toObject();

    // Запрос на получение данных
    QUrl url( "http://shikimori.org/api/animes/" + QString::number(obj["id"].toInt()) );

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyPullDataFinished(QNetworkReply*)));

    manager->get( QNetworkRequest(url) );
    r->deleteLater();
}

void DialogAddAnime::replyPullDataFinished(QNetworkReply *r)
{
    btnBox_reset();
    QByteArray data = r->readAll();

    QJsonDocument doc = QJsonDocument::fromJson( data );
    QJsonObject obj = doc.object();

    ui->LineEdit_Title->setText( obj["name"].toString() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( obj["russian"].toString() );
//    if( this->LineEdit_Director )
//        this->LineEdit_Director->setText( obj["russian"].toString() );
//    if( this->LineEdit_PostScoring )
//        this->LineEdit_PostScoring->setText( obj["name"].toString() );

    QDate date = QDate::fromString( obj["aired_on"].toString(), Qt::ISODate );
    if( date.year() != 0 )
        ui->SpinBox_Year->setValue( date.year() );


//    ui->SpinBox_Season->setValue( 1 ); // on any not zero

    QJsonArray studiosArray = obj["studios"].toArray();
    QJsonObject studioObj = studiosArray.at(0).toObject();
    ui->ComboBox_Studio->setCurrentText( studioObj["name"].toString() );

    if( obj["kind"].toString() == "TV" )
        ui->SpinBox_aTV->setValue( obj["episodes"].toInt() );
    else if( obj["kind"].toString() == "OVA" )
        ui->SpinBox_aOVA->setValue( obj["episodes"].toInt() );
    else if( obj["kind"].toString() == "ONA" )
        ui->SpinBox_aONA->setValue( obj["episodes"].toInt() );
    else if( obj["kind"].toString() == "Special" )
        ui->SpinBox_aSpec->setValue( obj["episodes"].toInt() );
    else if( obj["kind"].toString() == "Movie" )
        ui->SpinBox_aMovie->setValue( obj["episodes"].toInt() );

    QJsonArray tagArray = obj["genres"].toArray();

    QString tags;

    QSettings settings;
    QString lang( settings.value("Application/l10n", "en").toString() );
    bool ruLang = (lang == "ru")? true : false;

    for( int i = 0; i < tagArray.size(); ++i ){
        QJsonObject tagObj = tagArray.at(i).toObject();
        if( i > 0)
            tags += ", ";
        if( ruLang ){
            tags += tagObj["russian"].toString();
        }else{
            tags += tagObj["name"].toString();
        }
    }

    ui->LineEdit_Tags->setText( tags );

    ui->PlainTextEdit_Description->setPlainText( obj["description_html"].toString() );

    ui->LineEdit_URL->setText( "http://shikimori.org" + obj["url"].toString() );

    QString cover = (obj["image"].toObject())["original"].toString();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyDownloadPictureFinished(QNetworkReply*)));

    QUrl urlCover("http://shikimori.org" + cover);
    manager->get( QNetworkRequest(urlCover) );

    r->deleteLater();
}

void DialogAddAnime::replyDownloadPictureFinished(QNetworkReply *r)
{
    QImageReader imageReader(r);
    QImage image;
    if (r->error() == QNetworkReply::NoError)
        imageReader.read(&image);
    else
        qDebug() << "network error";

    if( image.isNull() )
        qDebug() << "image is null";

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QString tmpImagePath( QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() );

    bool okSave = image.save( tmpImagePath + coverName, "png" );
    if( !okSave )
        qCritical() << "Image is not save as: "
                    << tmpImagePath + coverName;

    QPixmap pm( tmpImagePath + coverName );
    if( !pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( tmpImagePath + coverName );
    }else{
        ui->Lbl_ImageCover->noImage();
        qCritical() << "Pixmap is null";
    }

    r->deleteLater();
}

void DialogAddAnime::on_LineEdit_Title_returnPressed()
{
    on_TBtn_Search_clicked();
}
