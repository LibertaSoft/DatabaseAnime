#include "dialogs/addmanga.h"
#include "ui_addmanga.h"
//#include "mngrquerys.h"
#include "definespath.h"

#include <QFileDialog>
#include <QDesktopServices>

#include <QImageReader>
//#include <QPicture>

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>

#include <QMessageBox>
#include <QDebug>
#include <QSettings>

void DialogAddManga::initTitleCompleter()
{
    // Init QCompliter
    TitleCompliter = new QCompleter( this );
    TitleCompliter->setModel(&_titleCompliterModel);
    TitleCompliter->setCaseSensitivity(Qt::CaseInsensitive);
    TitleCompliter->setCompletionMode(QCompleter::UnfilteredPopupCompletion); // PopupCompletion
    ui->LineEdit_Title->setCompleter( TitleCompliter );
    connect(&api, &ShikimoriApi::dataRecived_mangaSearch,
            this, &DialogAddManga::setCompletionModel );
    connect(&api, &ShikimoriApi::dataRecived_mangaId,
            &api, &ShikimoriApi::pullMangaData);
    connect(&api, &ShikimoriApi::dataRecived_mangaData,
            this, &DialogAddManga::setRecivedData);
}

void DialogAddManga::initTags()
{
    _tags.setStringList( MangaModel::getGanresList() );
    _tags.sort(1, Qt::AscendingOrder);
    ui->ListView_Tags->setModel( &_tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

void DialogAddManga::setDataInFields()
{
    _model->setId( QString::number( _recordId ) );
    _model->loadFromDatabase();

    ui->CheckBox_LookLater->setChecked( _model->wantToLook() == false );
    ui->CheckBox_Editing->setChecked( _model->editing() == false );

    ui->LineEdit_Title->setText( _model->title() );

    // Optional Fields
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->setText( _model->altTitle() );
    if( this->LineEdit_Author )
        this->LineEdit_Author->setText( _model->author() );
    if( this->LineEdit_Translation )
        this->LineEdit_Translation->setText( _model->translator() );

    if( _model->date().year() != 0 )
        ui->SpinBox_Year->setValue( _model->date().year() );

    ui->SpinBox_aVol->setValue( _model->series_total_volumes() );
    ui->SpinBox_aCh->setValue( _model->series_total_charapters() );
    ui->SpinBox_aPages->setValue( _model->series_total_pages() );

    ui->SpinBox_vVol->setValue( _model->series_viewed_volumes() );
    ui->SpinBox_vCh->setValue( _model->series_viewed_charapters() );
    ui->SpinBox_vPages->setValue( _model->series_viewed_pages() );

    ui->LineEdit_Tags->setText( _model->ganres().join(",") );
    ui->PlainTextEdit_Description->setPlainText( _model->description() );
    ui->LineEdit_Dir->setText( _model->localPath() );
    ui->LineEdit_URL->setText( _model->url() );

    _oldCover = _model->cover();
    QPixmap pm( DefinesPath::mangaCovers() + _oldCover );

    if( ! pm.isNull() ){
        ui->Lbl_ImageCover->setPixmap( pm );
        ui->Lbl_ImageCover->setImagePath( DefinesPath::mangaCovers() + _oldCover );
    }else{
        ui->Lbl_ImageCover->noImage();
    }
}

void DialogAddManga::createOptionalFields()
{
    QSettings settings;
    if( settings.value( Options::OptionalFields::Manga::AltTitle, false ).toBool() ){
        this->LineEdit_AltTitle = new QLineEdit(this);
        this->LineEdit_AltTitle->setMaxLength(128);
        this->LineEdit_AltTitle->setDragEnabled(true);
        this->LineEdit_AltTitle->setPlaceholderText( tr("Alternative title") );
        ui->VLay_AltTitle->addWidget( this->LineEdit_AltTitle );
    }
    if( settings.value( Options::OptionalFields::Manga::Author, false ).toBool() ){
        this->LineEdit_Author = new QLineEdit(this);
        this->LineEdit_Author->setMaxLength(32);
        this->LineEdit_Author->setDragEnabled(true);
        this->LineEdit_Author->setPlaceholderText( tr("Author") );
        ui->HLay_AuthorAndSound->addWidget( this->LineEdit_Author );
    }
    if( settings.value( Options::OptionalFields::Manga::Translator, false ).toBool() ){
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
        if( !this->LineEdit_Translation && !this->LineEdit_Author ){
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

    if( !this->LineEdit_Translation && !this->LineEdit_Author && !this->LineEdit_AltTitle ){
        setTabOrder(ui->SpinBox_Year, ui->SpinBox_aVol);
    }

    setTabOrder(ui->SpinBox_aVol, ui->SpinBox_aCh);
    setTabOrder(ui->SpinBox_aCh,  ui->SpinBox_aPages);

    setTabOrder(ui->tab_lookSeries, ui->SpinBox_vVol);
    setTabOrder(ui->SpinBox_vVol,   ui->SpinBox_vCh);
    setTabOrder(ui->SpinBox_vCh,    ui->SpinBox_vPages);
}

void DialogAddManga::connectSlots()
{
    connect( & _imageLoader, SIGNAL( imageLoaded(QImage) ),
             this,           SLOT( coverLoaded(QImage) ) );
    connect( ui->Lbl_ImageCover, SIGNAL( reloadCover() ),
             this,               SLOT( reloadCover() ) );
}

DialogAddManga::DialogAddManga(QWidget *parent, unsigned long long record_id ) :
    QDialog(parent), ui(new Ui::DialogAddManga), _isEditRole( true ), _recordId( record_id ),
    LineEdit_AltTitle(NULL), LineEdit_Author(NULL), LineEdit_Translation(NULL)
{
    ui->setupUi(this);
    _model = new MangaModel( QString::number( record_id ) );

    setWindowTitle( tr("Editing manga") );
    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Manga::Geometry).toByteArray() );
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
    createOptionalFields();
    setTabOrders();
    setDataInFields();
    initTags();
}

DialogAddManga::DialogAddManga(QWidget *parent):
    QDialog(parent), ui(new Ui::DialogAddManga), _isEditRole( false ),
    LineEdit_AltTitle(NULL), LineEdit_Author(NULL), LineEdit_Translation(NULL), _model( new MangaModel )
{
    ui->setupUi(this);
//    _model = new MangaModel;

    QSettings cfg;
    this->restoreGeometry( cfg.value(Options::Dialogs::Manga::Geometry).toByteArray() );
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
    createOptionalFields();
    setTabOrders();
    initTags();
}

DialogAddManga::~DialogAddManga()
{
    QSettings settings;
    settings.setValue(Options::Dialogs::Manga::Geometry, this->saveGeometry() );
    delete ui;
}

void DialogAddManga::btnBox_reset(bool clearImage = true)
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

    if( clearImage )
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

    _model->setWantToLook( ! ui->CheckBox_LookLater->isChecked() );
    _model->setEditing( ! ui->CheckBox_Editing->isChecked() );
    _model->setId( QString::number(_recordId) );
    _model->setTitle( ui->LineEdit_Title->text() );

    _model->setAltTitle( (LineEdit_AltTitle) ? LineEdit_AltTitle->text() : _altTitle );
    if( _model->altTitle().isEmpty() )
        _model->setAltTitle( ui->LineEdit_Title->text() );
    _model->setAuthor( (LineEdit_Author) ? LineEdit_Author->text() : "" );
    _model->setTranslator( (LineEdit_Translation) ? LineEdit_Translation->text() : "" );

    _model->setSeries_total_volumes( ui->SpinBox_aVol->value() );
    _model->setSeries_total_charapters( ui->SpinBox_aCh->value() );
    _model->setSeries_total_pages( ui->SpinBox_aPages->value() );

    _model->setSeries_viewed_volumes( ui->SpinBox_vVol->value() );
    _model->setSeries_viewed_charapters( ui->SpinBox_vCh->value() );
    _model->setSeries_viewed_pages( ui->SpinBox_vPages->value() );

    _model->setDate( QDate((ui->CBox_Year->isChecked()) ? ui->SpinBox_Year->value() : 0, 1, 1) );

    {
        QStringList list;
        QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
        for(int i = 0;i < mlist.count();i++){
            list.append(mlist.at(i).data(Qt::DisplayRole).toString());
        }

        _model->setGanres( list + ui->LineEdit_Tags->text().split(",") );
    }
    _model->setDescription( ui->PlainTextEdit_Description->toPlainText() );
    _model->setUrl( ui->LineEdit_URL->text() );
    _model->setLocalPath( ui->LineEdit_Dir->text() );

    {
        QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
        QDir dir;
        if( !ui->Lbl_ImageCover->getImagePath().isEmpty() && dir.mkpath( DefinesPath::mangaCovers() ) ){
            QFile f;
            f.setFileName( ui->Lbl_ImageCover->getImagePath() );
            f.copy( DefinesPath::mangaCovers() + coverName );
        }
        if( _isEditRole && !_oldCover.isEmpty() )
            dir.remove( DefinesPath::mangaCovers() + _oldCover );

        _model->setCover( coverName );
    }

    /// \todo rewraite to use logger
    bool success = _model->save( _isEditRole );
    if( ( ! success ) && ( ! _isEditRole ) ){
        QMessageBox::critical(this, tr("Critical"), tr("Cannot insert data"));
    }
    if( ! success ){
        QMessageBox::critical(this, tr("Critical"), tr("Cannot update data"));
    }
    return success;
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

void DialogAddManga::reloadCover()
{
    _imageLoader.getImage( _urlCover );
}

void DialogAddManga::on_LineEdit_Title_textEdited(const QString &title)
{
    if( _autoSearchOnShikimori == false )
        return;
    if( title.count() < 3 )
        return;
    foreach ( QString name, _titleCompliterModel.stringList() ) {
        if( name.toUpper().contains( title.toUpper() ) )
            return;
    }

    api.searchManga( title, _searchLimit );
}

void DialogAddManga::coverLoaded(QImage image)
{
    if( image.isNull() )
        qDebug() << "image is null";

    QString coverName( QString::number( QDateTime::currentMSecsSinceEpoch() ) );
    QString tmpImagePath( QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() );

    bool okSave = image.save( tmpImagePath + coverName, "png" );
    if( ! okSave )
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

void DialogAddManga::on_TBtn_Search_clicked()
{
    QString title = ui->LineEdit_Title->text();
    api.getMangaId( title );
}

/// \todo this is need use &data
void DialogAddManga::setRecivedData(KeyValue data)
{
    _model->fromKeyValue( data );

    btnBox_reset(false);
    ui->TabWidget_Info->setCurrentIndex(2);

    ui->LineEdit_Title->setText( _model->title() );

    // Optional Fields
    if( this->LineEdit_AltTitle )
        this->LineEdit_AltTitle->setText( _model->altTitle() );
    else
        _altTitle = _model->altTitle();
//    if( this->LineEdit_Author )
//        this->LineEdit_Author->setText( obj["russian"].toString() );
//    if( this->LineEdit_Translator )
//        this->LineEdit_Translator->setText( obj["name"].toString() );

    if( _model->date().year() != 0 )
        ui->SpinBox_Year->setValue( _model->date().year() );

    ui->SpinBox_aVol->setValue( _model->series_total_volumes() );
    ui->SpinBox_aCh->setValue( _model->series_total_charapters() );

    ui->LineEdit_Tags->setText( _model->ganres().join(",") );

    ui->PlainTextEdit_Description->setPlainText( _model->description() );

    ui->LineEdit_URL->setText( _model->url() );

    QString cover = _model->cover();
    _urlCover = QUrl(shikimoriUrl + cover);
    ui->Lbl_ImageCover->enableReloadButton( ! _urlCover.isEmpty() );

    QSettings cfg;
    bool hasLoadImage = ui->Lbl_ImageCover->isNullImage()
                        || ( ! _isEditRole )
                        || (cfg.value( Options::Network::RELOAD_COVERS, true ).toBool());

    qDebug() << "hasLoadImage:"
             << ui->Lbl_ImageCover->isNullImage()
             << ( ! _isEditRole )
             << (cfg.value( Options::Network::RELOAD_COVERS, true ).toBool());

    if( hasLoadImage ){
        _imageLoader.getImage( _urlCover );
    }
}

bool DialogAddManga::setSearchLimit(const int limit)
{
    if(limit > 0){
        _searchLimit = limit;
        return true;
    } else {
        _searchLimit = 10; /// \note default value 10
        return false;
    }
}

void DialogAddManga::setSearchOutput(const SearchOutput outType)
{
    _searchOutput = outType;
}

void DialogAddManga::setCompletionModel(QStringList eng, QStringList rus)
{
    QStringList model;

    switch ( _searchOutput ) {
        case SearchOutput::ENG :
            model = eng;
            break;
        case SearchOutput::RUS :
            model = rus;
            break;
        case SearchOutput::MIX :
        default:
            model = eng+rus;
    }

    _titleCompliterModel.setStringList( model );
}
