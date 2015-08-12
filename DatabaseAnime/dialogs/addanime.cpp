#include "dialogs/addanime.h"
#include "ui_addanime.h"
#include "mngrquerys.h"
#include "definespath.h"

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


void DialogAddAnime::initTitleCompleter()
{
    // Init QCompliter
    TitleCompliter = new QCompleter( this );
    TitleCompliter->setModel(&_titleCompliterModel);
    TitleCompliter->setCaseSensitivity(Qt::CaseInsensitive);
    TitleCompliter->setCompletionMode(QCompleter::UnfilteredPopupCompletion); // PopupCompletion
    ui->LineEdit_Title->setCompleter( TitleCompliter );
    connect(&api, &ShikimoriApi::dataRecived_animeSearch,
            this, &DialogAddAnime::setCompletionModel );
    connect(&api, &ShikimoriApi::dataRecived_animeId,
            &api, &ShikimoriApi::pullAnimeData);
    connect(&api, &ShikimoriApi::dataRecived_animeData,
            this, &DialogAddAnime::setRecivedData);
}

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
    if( settings.value( Options::OptionalFields::Anime::AltTitle, false ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setMaxLength(128);
        this->LineEdit_OrigTitle->setDragEnabled(true);
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( Options::OptionalFields::Anime::Director, false ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setMaxLength(32);
        this->LineEdit_Director->setDragEnabled(true);
        this->LineEdit_Director->setPlaceholderText( tr("Director") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( Options::OptionalFields::Anime::Postscoring, false ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setMaxLength(128);
        this->LineEdit_PostScoring->setDragEnabled(true);
        this->LineEdit_PostScoring->setPlaceholderText( tr("Postscoring") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }
}

void DialogAddAnime::setDataInField()
{
    QSqlRecord record = MngrQuerys::selectData( sections::anime, _recordId );

    ui->CheckBox_LookLater->setChecked( record.value( Tables::Anime::Fields::isHaveLooked ).toBool() == false );
    ui->CheckBox_Editing->setChecked( record.value( Tables::Anime::Fields::isEditingDone ).toBool() == false );

    ui->LineEdit_Title->setText( record.value( Tables::Anime::Fields::Title ).toString() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( record.value( Tables::Anime::Fields::AltTitle ).toString() );
    if( this->LineEdit_Director )
        this->LineEdit_Director->setText( record.value( Tables::Anime::Fields::Director ).toString() );
    if( this->LineEdit_PostScoring )
        this->LineEdit_PostScoring->setText( record.value( Tables::Anime::Fields::PostScoring ).toString() );
    if( record.value("Year").toInt() != 0 )
        ui->SpinBox_Year->setValue( record.value( Tables::Anime::Fields::Year ).toInt() );
    ui->SpinBox_Season->setValue( record.value( Tables::Anime::Fields::Season ).toInt() );
    ui->ComboBox_Studio->setCurrentText( record.value( Tables::Anime::Fields::Studios ).toString() );

    ui->SpinBox_aTV->setValue(    record.value( Tables::Anime::Fields::SeriesTV      ).toInt() );
    ui->SpinBox_aOVA->setValue(   record.value( Tables::Anime::Fields::SeriesOVA     ).toInt() );
    ui->SpinBox_aONA->setValue(   record.value( Tables::Anime::Fields::SeriesONA     ).toInt() );
    ui->SpinBox_aSpec->setValue(  record.value( Tables::Anime::Fields::SeriesSpecial ).toInt() );
    ui->SpinBox_aMovie->setValue( record.value( Tables::Anime::Fields::SeriesMovie   ).toInt() );

    ui->SpinBox_vTV->setValue(    record.value( Tables::Anime::Fields::vSeriesTV      ).toInt() );
    ui->SpinBox_vOVA->setValue(   record.value( Tables::Anime::Fields::vSeriesOVA     ).toInt() );
    ui->SpinBox_vONA->setValue(   record.value( Tables::Anime::Fields::vSeriesONA     ).toInt() );
    ui->SpinBox_vSpec->setValue(  record.value( Tables::Anime::Fields::vSeriesSpecial ).toInt() );
    ui->SpinBox_vMovie->setValue( record.value( Tables::Anime::Fields::vSeriesMovie   ).toInt() );

    ui->LineEdit_Tags->setText( record.value( Tables::Anime::Fields::Tags ).toString() );
    ui->PlainTextEdit_Description->setPlainText( record.value( Tables::Anime::Fields::Description ).toString() );
    ui->LineEdit_Dir->setText( record.value( Tables::Anime::Fields::Dir ).toString() );
    ui->LineEdit_URL->setText( record.value( Tables::Anime::Fields::Url ).toString() );

    _oldCover = record.value( Tables::Anime::Fields::ImagePath ).toString();

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

int DialogAddAnime::kindOf(const QString &kind)
{
    const QString TV("TV");
    const QString OVA("OVA");
    const QString ONA("ONA");
    const QString SPECIAL("SPECIAL");
    const QString MOVIE("MOVIE");

    QString _kind = kind.toUpper();
    if( _kind == TV )
        return Kind::TV;
    if( _kind == OVA )
        return Kind::OVA;
    if( _kind == ONA )
        return Kind::ONA;
    if( _kind == SPECIAL )
        return Kind::Special;
    if( _kind == MOVIE )
        return Kind::Movie;

    return Kind::TV;
}

DialogAddAnime::DialogAddAnime(QWidget *parent, unsigned long long record_id) :
    QDialog(parent), ui(new Ui::DialogAddAnime), _isEditRole(true), _recordId(record_id),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL), TitleCompliter(NULL)
{
    ui->setupUi(this);
    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Anime::Geometry).toByteArray() );
    api.setLang("ru");
    _autoSearchOnShikimori = cfg.value( Options::Network::LIVE_SEARCH, true ).toBool();
    setSearchLimit( cfg.value( Options::Network::SEARCH_LIMIT, 10 ).toInt() ); /// \todo default value
    int searchOutType = cfg.value( Options::Network::SEARCH_OUTPUT, SearchOutput::MIX ).toInt();
    setSearchOutput( static_cast<SearchOutput>(searchOutType) );

    connect( & _imageLoader, SIGNAL(imageLoaded(QImage)),
             this,           SLOT(coverLoaded(QImage)) );

    // Reset tabs
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    initTitleCompleter();
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
    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Anime::Geometry).toByteArray() );
    api.setLang("ru");
    _autoSearchOnShikimori = cfg.value( Options::Network::LIVE_SEARCH, true ).toBool();
    setSearchLimit( cfg.value( Options::Network::SEARCH_LIMIT, 10 ).toInt() ); /// \todo default value
    int searchOutType = cfg.value( Options::Network::SEARCH_OUTPUT, SearchOutput::MIX ).toInt();
    setSearchOutput( static_cast<SearchOutput>(searchOutType) );

    connect( & _imageLoader, SIGNAL(imageLoaded(QImage)),
             this,           SLOT(coverLoaded(QImage)) );

    // Reset tabs
    ui->TabWidget_Series->setCurrentIndex(0);
    ui->TabWidget_Info->setCurrentIndex(0);
    ui->LineEdit_Title->setFocus();

    initTitleCompleter();
    initTags();
    initOptionalFields();
    setTabOrders();
}

DialogAddAnime::~DialogAddAnime()
{
    QSettings settings;
    settings.setValue(Options::Dialogs::Anime::Geometry, this->saveGeometry() );
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
    using namespace Tables::Anime::Fields;
    QMap<QString, QVariant> data;

    data[isHaveLooked]   = !ui->CheckBox_LookLater->isChecked();
    data[isEditingDone]  = !ui->CheckBox_Editing->isChecked();
    data[isAdult]        = false;
    data[id]             = _recordId;

    QRegExp rx("<.*>"); rx.setMinimal(true);
    data[Title]          = ui->LineEdit_Title->text().remove(rx);

    data[AltTitle]      = (LineEdit_OrigTitle  )?   this->LineEdit_OrigTitle->text() : _altTitle;
    if( data[AltTitle].toString().isEmpty() )
        data[AltTitle]  = ui->LineEdit_Title->text();
    data[Director]       = (LineEdit_Director  )?    this->LineEdit_Director->text() : "";
    data[PostScoring]    = (LineEdit_PostScoring)?this->LineEdit_PostScoring->text() : "";
    data[SeriesTV]       = ui->SpinBox_aTV->value();
    data[SeriesOVA]      = ui->SpinBox_aOVA->value();
    data[SeriesONA]      = ui->SpinBox_aONA->value();
    data[SeriesSpecial]  = ui->SpinBox_aSpec->value();
    data[SeriesMovie]    = ui->SpinBox_aMovie->value();
    data[vSeriesTV]      = ui->SpinBox_vTV->value();
    data[vSeriesOVA]     = ui->SpinBox_vOVA->value();
    data[vSeriesONA]     = ui->SpinBox_vONA->value();
    data[vSeriesSpecial] = ui->SpinBox_vSpec->value();
    data[vSeriesMovie]   = ui->SpinBox_vMovie->value();
    data[Score]          = 0;
    data[Year]           = (ui->CBox_Year->isChecked()  )? ui->SpinBox_Year->value() :0;
    data[Season]         = ui->SpinBox_Season->value();
    data[Studios]        = ui->ComboBox_Studio->currentText();

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

    data[Tags]        = tagsList;
    data[Description] = ui->PlainTextEdit_Description->toPlainText();
    data[Url]         = ui->LineEdit_URL->text();
    data[Dir]         = ui->LineEdit_Dir->text();

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::animeCovers() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( DefinesPath::animeCovers() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() ){
            dir.remove( DefinesPath::animeCovers() + _oldCover );
    }

    data[ImagePath] = coverName;

    if( ! _isEditRole ){
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
    QString title = ui->LineEdit_Title->text();
    api.getAnimeId( title );
}

void DialogAddAnime::on_LineEdit_Title_textEdited(const QString &title)
{
    if( _autoSearchOnShikimori == false )
        return;
    if( title.count() < 3 )
        return;
    foreach ( QString name, _titleCompliterModel.stringList() ) {
        if( name.toUpper().contains( title.toUpper() ) )
            return;
    }

    api.searchAnime( title, _searchLimit );
}

void DialogAddAnime::coverLoaded(QImage image)
{
    if( image.isNull() )
        qCritical() << "image is null";

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QString tmpImagePath( QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() );

    bool okSave = image.save( tmpImagePath + coverName, "png" );
    if( !okSave )
        qCritical() << "Image is not save as: "
                    << tmpImagePath + coverName;

    QPixmap pm( tmpImagePath + coverName );
    if( ! pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( tmpImagePath + coverName );
    }else{
        ui->Lbl_ImageCover->noImage();
        qCritical() << "Pixmap is null";
    }
}

void DialogAddAnime::setRecivedData(QMap<QString, QVariant> data)
{
    using namespace Tables::Anime::Fields;
    btnBox_reset();
    ui->TabWidget_Info->setCurrentIndex(2);

    ui->LineEdit_Title->setText( data[Title].toString() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( data[AltTitle].toString() );
    else
        _altTitle = data[AltTitle].toString();
//    if( this->LineEdit_Director )
//        this->LineEdit_Director->setText( obj["russian"].toString() );
//    if( this->LineEdit_PostScoring )
//        this->LineEdit_PostScoring->setText( obj["name"].toString() );

    if( data[Year].toInt() != 0 )
        ui->SpinBox_Year->setValue( data[Year].toInt() );

//    ui->SpinBox_Season->setValue( 1 ); // on any not zero

    ui->ComboBox_Studio->setCurrentText( data[Studios].toString() );


    {
        int type = kindOf( data["Type"].toString() );
        switch (type) {
            case Kind::TV :
                ui->SpinBox_aTV->setValue( data["Series"].toInt() );
                break;
            case Kind::OVA :
                ui->SpinBox_aOVA->setValue( data["Series"].toInt() );
                break;
            case Kind::ONA :
                ui->SpinBox_aONA->setValue( data["Series"].toInt() );
                break;
            case Kind::Special :
                ui->SpinBox_aSpec->setValue( data["Series"].toInt() );
                break;
            case Kind::Movie :
                ui->SpinBox_aMovie->setValue( data["Series"].toInt() );
                break;
            default:
                break;
        }
    }

    ui->LineEdit_Tags->setText( data[Tags].toString() );

    ui->PlainTextEdit_Description->setPlainText( data[Description].toString() );

    ui->LineEdit_URL->setText( data[Url].toString() );

    QSettings cfg;

    if( cfg.value( Options::Network::DOWNLOAD_COVERS, true ).toBool() ){
        QString cover = data[ImagePath].toString();
        QUrl urlCover(shikimoriUrl + cover);
        _imageLoader.getImage( urlCover );
    }
}

bool DialogAddAnime::setSearchLimit(const int limit)
{
    if(limit > 0){
        _searchLimit = limit;
        return true;
    } else {
        _searchLimit = 10; /// \note default value 10
        return false;
    }
}

void DialogAddAnime::setSearchOutput(SearchOutput outputType)
{
    _searchOutput = outputType;
}

void DialogAddAnime::setCompletionModel(QStringList eng, QStringList rus)
{
    QStringList model;

    switch( _searchOutput ){
        case SearchOutput::ENG :
            model = eng;
            break;
        case SearchOutput::RUS :
            model = rus;
            break;
        case SearchOutput::MIX :
        default:
            model = eng + rus;
    }

    _titleCompliterModel.setStringList( model );
}
