#include "dialogs/addanime.h"
#include "ui_addanime.h"
//#include "mngrquerys.h"
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
#include <QSettings>


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
    _tags.setStringList( AnimeModel::getGanresList() );
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
    ui->CheckBox_LookLater->setChecked( _model->wantToLook() );
    ui->CheckBox_Editing->setChecked( _model->editing() );

    ui->LineEdit_Title->setText( _model->title() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( _model->altTitle() );
    if( this->LineEdit_Director )
        this->LineEdit_Director->setText( _model->director() );
    if( this->LineEdit_PostScoring )
        this->LineEdit_PostScoring->setText( _model->postScoring() );
    if( _model->date().year() != 0 )
        ui->SpinBox_Year->setValue( _model->date().year() );
    ui->SpinBox_Season->setValue( _model->seasone() );
    ui->ComboBox_Studio->setCurrentText( _model->studio() );

    ui->SpinBox_aTV->setValue(    _model->series_total_tv() );
    ui->SpinBox_aOVA->setValue(   _model->series_total_ova() );
    ui->SpinBox_aONA->setValue(   _model->series_total_ona() );
    ui->SpinBox_aSpec->setValue(  _model->series_total_special() );
    ui->SpinBox_aMovie->setValue( _model->series_total_movie() );

    ui->SpinBox_vTV->setValue(    _model->series_viewed_tv() );
    ui->SpinBox_vOVA->setValue(   _model->series_viewed_ova() );
    ui->SpinBox_vONA->setValue(   _model->series_viewed_ona() );
    ui->SpinBox_vSpec->setValue(  _model->series_viewed_special() );
    ui->SpinBox_vMovie->setValue( _model->series_viewed_movie() );

    ui->LineEdit_Tags->setText( _model->ganres().join(", ") );
    ui->PlainTextEdit_Description->setPlainText( _model->description() );
    ui->LineEdit_Dir->setText( _model->localPath() );
    ui->LineEdit_URL->setText( _model->url() );

    _oldCover = _model->cover();
    QPixmap pm( DefinesPath::animeCovers() + _oldCover );

    if( ! pm.isNull() ){
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
    _model = new AnimeModel( QString::number(record_id) );

    setWindowTitle( tr("Editing anime") );
    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Anime::Geometry).toByteArray() );
    api.setLang("ru");
    _autoSearchOnShikimori = cfg.value( Options::Network::LIVE_SEARCH, true ).toBool();
    setSearchLimit( cfg.value( Options::Network::SEARCH_LIMIT, 10 ).toInt() ); /// \todo default value
    int searchOutType = cfg.value( Options::Network::SEARCH_OUTPUT, SearchOutput::MIX ).toInt();
    setSearchOutput( static_cast<SearchOutput>(searchOutType) );

    connectSlots();

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

void DialogAddAnime::connectSlots()
{
    connect( & _imageLoader, SIGNAL(imageLoaded(QImage)),
             this,           SLOT(coverLoaded(QImage)) );
    connect( ui->Lbl_ImageCover, SIGNAL( reloadCover() ),
             this,               SLOT( reloadCover() ) );
}

DialogAddAnime::DialogAddAnime(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddAnime), _isEditRole(false),
    LineEdit_OrigTitle(NULL), LineEdit_Director(NULL), LineEdit_PostScoring(NULL), TitleCompliter(NULL)
{
    ui->setupUi(this);
    _model = new AnimeModel();

    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Anime::Geometry).toByteArray() );
    api.setLang("ru");
    _autoSearchOnShikimori = cfg.value( Options::Network::LIVE_SEARCH, true ).toBool();
    setSearchLimit( cfg.value( Options::Network::SEARCH_LIMIT, 10 ).toInt() ); /// \todo default value
    int searchOutType = cfg.value( Options::Network::SEARCH_OUTPUT, SearchOutput::MIX ).toInt();
    setSearchOutput( static_cast<SearchOutput>(searchOutType) );

    connectSlots();

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

void DialogAddAnime::btnBox_reset( bool clearImage = true )
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

    if( clearImage )
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
    /*
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
    */
    _model->setWantToLook( ! ui->CheckBox_LookLater->isChecked() );
    _model->setEditing( ! ui->CheckBox_Editing->isChecked() );
    _model->setId( QString::number( _recordId ) ); /// \todo
    _model->setTitle( ui->LineEdit_Title->text() );
    _model->setDirector( (LineEdit_Director)? this->LineEdit_Director->text() : "" );
    _model->setPostScoring( (LineEdit_PostScoring)?this->LineEdit_PostScoring->text() : "" );

    _model->setSeries_total_tv( ui->SpinBox_aTV->value() );
    _model->setSeries_total_ova( ui->SpinBox_aOVA->value() );
    _model->setSeries_total_ona( ui->SpinBox_aONA->value() );
    _model->setSeries_total_special( ui->SpinBox_aSpec->value() );
    _model->setSeries_total_movie( ui->SpinBox_aMovie->value() );

    _model->setSeries_viewed_tv( ui->SpinBox_vTV->value() );
    _model->setSeries_viewed_ova( ui->SpinBox_vOVA->value() );
    _model->setSeries_viewed_ona( ui->SpinBox_vONA->value() );
    _model->setSeries_viewed_special( ui->SpinBox_vSpec->value() );
    _model->setSeries_viewed_movie( ui->SpinBox_vMovie->value() );

    _model->setDate( QDate( (ui->CBox_Year->isChecked() ) ? ui->SpinBox_Year->value() : 0, 1, 1) );
    _model->setSeasone( ui->SpinBox_Season->value() );
    _model->setStudio( ui->ComboBox_Studio->currentText() );
    _model->setDescription( ui->PlainTextEdit_Description->toPlainText() );
    _model->setUrl( ui->LineEdit_URL->text() );
    _model->setLocalPath( ui->LineEdit_Dir->text() );

    {
        QStringList list;
        QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
        for(int i = 0;i < mlist.count();i++){
            list.append(mlist.at(i).data(Qt::DisplayRole).toString());
        }
        QString tagsList = ui->LineEdit_Tags->text();
        list.append( tagsList.split(",") );
        _model->setGanres( list );
    }

    QString altTitle = (LineEdit_OrigTitle  )? this->LineEdit_OrigTitle->text() : _altTitle;
    if( altTitle.isEmpty() )
        altTitle = ui->LineEdit_Title->text();
    _model->setAltTitle( altTitle );

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QDir dir;
    if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::animeCovers() ) ){
        QFile f( ui->Lbl_ImageCover->getImagePath() );
        f.copy( DefinesPath::animeCovers() + coverName );
    }
    if( _isEditRole && !_oldCover.isEmpty() ){
            dir.remove( DefinesPath::animeCovers() + _oldCover );
    }
    /*
    data[ImagePath] = coverName;
    */
    _model->setCover( coverName );

    if( ! _isEditRole ){
        if( _model->save(false) == false ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot insert data."));
            return false;
        }
    }else{
        if( _model->save(true) == false ){
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

void DialogAddAnime::reloadCover()
{
    _imageLoader.getImage( _urlCover );
    qDebug() << _urlCover;
}

void DialogAddAnime::setRecivedData(KeyValue data)
{
    _model->fromKeyValue( data );
    btnBox_reset(false);
    ui->TabWidget_Info->setCurrentIndex(2);

    ui->LineEdit_Title->setText( _model->title() );

    // Optional Fields
    if( this->LineEdit_OrigTitle )
        this->LineEdit_OrigTitle->setText( _model->altTitle() );
    else
        _altTitle = _model->altTitle();
//    if( this->LineEdit_Director )
//        this->LineEdit_Director->setText( obj["russian"].toString() );
//    if( this->LineEdit_PostScoring )
//        this->LineEdit_PostScoring->setText( obj["name"].toString() );

    if( _model->date().year() != 0 ) /// \todo \fixme
        ui->SpinBox_Year->setValue( _model->date().year() );

//    ui->SpinBox_Season->setValue( 1 ); // on any not zero

    ui->ComboBox_Studio->setCurrentText( _model->studio() );


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

    ui->LineEdit_Tags->setText( _model->ganres().join(", ") );

    ui->PlainTextEdit_Description->setPlainText( _model->description() );

    ui->LineEdit_URL->setText( _model->url() );

    QString cover = _model->cover();
    _urlCover = QUrl(ShikimoriApi::getShikimoriUrl() + cover);

    ui->Lbl_ImageCover->enableReloadButton( ! _urlCover.isEmpty() );

    QSettings cfg;
    bool hasLoadImage = ui->Lbl_ImageCover->isNullImage()
                        || ( ! _isEditRole )
                        || (cfg.value( Options::Network::RELOAD_COVERS, true ).toBool());

    if( hasLoadImage ){
        _imageLoader.getImage( _urlCover );
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
