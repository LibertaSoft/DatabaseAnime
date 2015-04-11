#include "settings.h"
#include "stylemanager.h"
#include "ui_settings.h"

#include <QColorDialog>
#include <QInputDialog>
#include <QStyleFactory>

void Settings::connectColorPicker()
{
    connect( ui->Frame_Style_Window, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_WindowText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Base, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_AlternateBase, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ToolTipBase, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ToolTipText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Text, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Button, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_ButtonText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_BrightText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Link, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_Highlight, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
    connect( ui->Frame_Style_HighlightedText, &ColorPicker::colorChanged,
             this, &Settings::colorPicked );
}

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QSettings settings;
    this->restoreGeometry( settings.value(Options::Dialogs::Config::Geometry).toByteArray() );
    ui->splitter->restoreGeometry( settings.value(Options::Dialogs::Config::Splitter::Geometry).toByteArray() );
    ui->splitter->restoreState( settings.value(Options::Dialogs::Config::Splitter::State).toByteArray() );

    bool b1 = settings.value( Options::ActiveSections::Anime,   true ).toBool();
    bool b2 = settings.value( Options::ActiveSections::Manga,  false ).toBool();
    bool b3 = settings.value( Options::ActiveSections::Amv,    false ).toBool();
    bool b4 = settings.value( Options::ActiveSections::Dorama, false ).toBool();


    bool a1 = settings.value( Options::OptionalFields::Anime::AltTitle,    false ).toBool();
    bool a2 = settings.value( Options::OptionalFields::Anime::Director,    false ).toBool();
    bool a3 = settings.value( Options::OptionalFields::Anime::Postscoring, false ).toBool();

    bool m1 = settings.value( Options::OptionalFields::Manga::AltTitle,    false ).toBool();
    bool m2 = settings.value( Options::OptionalFields::Manga::Author,      false ).toBool();
    bool m3 = settings.value( Options::OptionalFields::Manga::Translator,  false ).toBool();

    bool d1 = settings.value( Options::OptionalFields::Dorama::AltTitle, false ).toBool();
    bool d2 = settings.value( Options::OptionalFields::Dorama::Director, false ).toBool();

    bool checkUpdates      = settings.value(Options::Network::CheckUpdates,          true).toBool();
    bool searchOnShikimori = settings.value(Options::Network::AutoSearchOnShikimori, true).toBool();

    bool SwitchCoverOrDir = settings.value( Options::General::SwitchCoverOrDir, true ).toBool();

    ui->ChBox_AS_Anime->setChecked( b1 );
    ui->ChBox_AS_Manga->setChecked( b2 );
    ui->ChBox_AS_Amv->setChecked( b3 );
    ui->ChBox_AS_Dorama->setChecked( b4 );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( a1 );
    ui->ChBox_OptField_Anime_Director->setChecked( a2 );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( a3 );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( m1 );
    ui->ChBox_OptField_Manga_Author->setChecked( m2 );
    ui->ChBox_OptField_Manga_Translation->setChecked( m3 );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( d1 );
    ui->ChBox_OptField_Dorama_Director->setChecked( d2 );

    ui->ChBox_CheckForUpdate->setChecked( checkUpdates );
    ui->ChBox_SwitchCoverOrDir->setChecked( SwitchCoverOrDir );
    ui->ChBox_SearchOnShikimori->setChecked( searchOnShikimori );

    QLocale::Language set_language = static_cast<QLocale::Language>(settings.value( Options::General::Language, QLocale::English ).toInt());

    ui->ComboBox_Language->addItem( tr("System"), 0 );

    /// \~russian
    /// \todo Использовать поиск языка по имени, а не по индексу, на каждой проверке.
    QMap<QLocale::Language,QString> langList = DbaLocalization::readExistsLocalizations( DefinesPath::share() );
    int i(0);
    foreach (QString langName, langList) {
        ++i;
        ui->ComboBox_Language->addItem(langName, langList.key(langName));
        if( set_language == langList.key(langName) )
            ui->ComboBox_Language->setCurrentIndex(i);
    }

    {  // Sorting
        ui->ComboBox_ItemList_Sorting->addItem(tr("None"), Sort::none);
        ui->ComboBox_ItemList_Sorting->addItem(tr("ASC"),  Sort::asc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("DESC"), Sort::desc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("Year"), Sort::year);

        Sort::sort sort = static_cast<Sort::sort>( settings.value( Options::General::Sorting, Sort::asc ).toInt() );
        ui->ComboBox_ItemList_Sorting->setCurrentIndex( sort );
    }
    // Work dir
    ui->LineEdit_WorkDir->setText( QDir::toNativeSeparators( DefinesPath::appData() ) );

    {   // Displayed field
        Tables::UniformField::field displayedField =
                static_cast<Tables::UniformField::field>(
                    settings.value( Options::General::DisplayedField, Tables::UniformField::Title ).toInt() );
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Title"), Tables::UniformField::Title);
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Alternative title"), Tables::UniformField::AltTitle);
        if( displayedField == Tables::UniformField::Title ){
            const int TITLE_INDEX = 0;
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( TITLE_INDEX );
        }else{
            const int ALT_TITLE_INDEX = 1;
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( ALT_TITLE_INDEX );
        }
    }

    { // Style
        using namespace Options::Style;

        QString styleName = settings.value( CurrentStyleName, "System" ).toString();
        bool    isSystemStyle    = settings.value( SystemStyle, true ).toBool();

        QSet<QString> styles = StyleManager::getExistsStyles();
        ui->ComboBox_CurrentStyle->addItems( styles.toList() );

        if( ( ! isSystemStyle) && styles.contains( styleName ) ){
            ui->ComboBox_CurrentStyle->setCurrentIndex( ui->ComboBox_CurrentStyle->findText( styleName ) );
            stylePalette = StyleManager::getPaletteOfStyle( styleName );
        }else{
            ui->ComboBox_CurrentStyle->setCurrentIndex( INDEX_OF_SYSTEM_STYLE );
        }

        initColorPickers( stylePalette );
        connectColorPicker();
        ui->GroupBox_Style_Colors->setEnabled( ! isSystemStyle );

        ui->PButton_Style_SaveChanges->setVisible( false );
    }
}

Settings::~Settings()
{
    using namespace Options::Dialogs::Config;
    QSettings settings;
    settings.setValue(Geometry, this->saveGeometry() );

    settings.setValue(Splitter::Geometry, ui->splitter->saveGeometry() );
    settings.setValue(Splitter::State,    ui->splitter->saveState() );
    delete ui;
}

/*! \~russian
 * \brief Метод - возвращает выбранный пользователем способ сортировки
 * \return перечисление Sort описанное в файле globalenum.h
 *
 * Метод предназначен для получения выбранного пользователем способа
 * сортировки содержимого разделов.
 */
Sort::sort Settings::getSort()
{
    return static_cast<Sort::sort>( ui->ComboBox_ItemList_Sorting->currentIndex() );
}

Tables::UniformField::field Settings::getDisplaydField()
{
    return static_cast<Tables::UniformField::field>(ui->ComboBox_ItemList_DisplayedField->currentData().toInt());
}

bool Settings::getSwitchToDir()
{
    return ui->ChBox_SwitchCoverOrDir->isChecked();
}

bool Settings::getRestoreDefault()
{
    return restoreDefault;
}

QLocale::Language Settings::getLanguage()
{
    return static_cast<QLocale::Language>( ui->ComboBox_Language->currentData().toInt() );
}

void Settings::colorPicked(QColor)
{
    stylePalette = paletteFromColorPicker();
    setApplicationStyle( stylePalette );
    ui->PButton_Style_SaveChanges->setVisible( true );
}

void Settings::on_listWidget_currentRowChanged(int currentRow)
{
    ui->StackedWidget->setCurrentIndex(currentRow);
}

void Settings::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case  QDialogButtonBox::ApplyRole:
            on_BtnBox_accepted();
            this->close();
            break;
        case QDialogButtonBox::ResetRole:
            BtnBox_resetDefaults();
            break;
        default:
            this->close();
    }
}

void Settings::on_BtnBox_accepted()
{
    QSettings settings;
    {
        using namespace Options::ActiveSections;
        settings.setValue( Anime,  ui->ChBox_AS_Anime->isChecked() );
        settings.setValue( Manga,  ui->ChBox_AS_Manga->isChecked() );
        settings.setValue( Amv,    ui->ChBox_AS_Amv->isChecked() );
        settings.setValue( Dorama, ui->ChBox_AS_Dorama->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Anime;
        settings.setValue( AltTitle,   ui->ChBox_OptField_Anime_AltTitle->isChecked() );
        settings.setValue( Director,    ui->ChBox_OptField_Anime_Director->isChecked() );
        settings.setValue( Postscoring, ui->ChBox_OptField_Anime_Postscoring->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Manga;
        settings.setValue( AltTitle,    ui->ChBox_OptField_Manga_AltTitle->isChecked() );
        settings.setValue( Author,      ui->ChBox_OptField_Manga_Author->isChecked() );
        settings.setValue( Translator,  ui->ChBox_OptField_Manga_Translation->isChecked() );
    }
    {
        using namespace Options::OptionalFields::Dorama;
        settings.setValue( AltTitle,   ui->ChBox_OptField_Dorama_AltTitle->isChecked() );
        settings.setValue( Director,   ui->ChBox_OptField_Dorama_Director->isChecked() );
    }
    {
        using namespace Options::General;
        settings.setValue( Language, ui->ComboBox_Language->currentData() );
        settings.setValue( Sorting,  ui->ComboBox_ItemList_Sorting->currentIndex() );
    }
    {
        using namespace Options::Network;
        settings.setValue( CheckUpdates,          ui->ChBox_CheckForUpdate->isChecked() );
        settings.setValue( AutoSearchOnShikimori, ui->ChBox_SearchOnShikimori->isChecked() );
    }
    { // Style
        using namespace Options::Style;
        bool isSystemStyle = ui->ComboBox_CurrentStyle->currentIndex() == INDEX_OF_SYSTEM_STYLE;
        QString styleName  = ui->ComboBox_CurrentStyle->currentText();

        settings.setValue( SystemStyle, isSystemStyle );
        settings.setValue( CurrentStyleName, styleName );

        if( ! isSystemStyle )
            StyleManager::saveStyle( styleName, paletteFromColorPicker() );
    }

    settings.setValue( Options::General::SwitchCoverOrDir, ui->ChBox_SwitchCoverOrDir->isChecked() );

    if( QDir::isAbsolutePath( ui->LineEdit_WorkDir->text() ) )
        settings.setValue( Options::General::WorkDirectory, QDir( ui->LineEdit_WorkDir->text() ).path() );
    else
        settings.remove(Options::General::WorkDirectory);

    { // Displayed field
        int displayedFieldId = ui->ComboBox_ItemList_DisplayedField->currentData().toInt();
        Tables::UniformField::field displayedField
                = static_cast<Tables::UniformField::field>( displayedFieldId );
        settings.setValue( Options::General::DisplayedField, displayedField );
    }

}

void Settings::BtnBox_resetDefaults()
{
    restoreDefault = true;
    ui->ChBox_AS_Anime->setChecked( true );
    ui->ChBox_AS_Manga->setChecked( false );
    ui->ChBox_AS_Amv->setChecked( false );
    ui->ChBox_AS_Dorama->setChecked( false );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( false );
    ui->ChBox_OptField_Anime_Director->setChecked( false );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( false );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( false );
    ui->ChBox_OptField_Manga_Author->setChecked( false );
    ui->ChBox_OptField_Manga_Translation->setChecked( false );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( false );
    ui->ChBox_OptField_Dorama_Director->setChecked( false );

    const int SYSTEM_LANGUAGE = 0;
    ui->ComboBox_Language->setCurrentIndex( SYSTEM_LANGUAGE );
    ui->ComboBox_ItemList_Sorting->setCurrentIndex( Sort::asc );

    ui->ChBox_CheckForUpdate->setChecked( true );
    ui->ChBox_SwitchCoverOrDir->setChecked( true );
    ui->ChBox_SearchOnShikimori->setChecked( true );

    ui->LineEdit_WorkDir->setText( DefinesPath::appData(true) );

    /// \todo Add reset other options here
}

/*! \~russian
 * \brief Метод инициализирует элементы настройки цвета в соответствии с переданной палитрой.
 * \param palette - палитра для инициализации
 */
void Settings::initColorPickers(QPalette palette)
{
    ui->Frame_Style_Window->setColor( palette.window().color() );
    ui->Frame_Style_WindowText->setColor( palette.windowText().color() );
    ui->Frame_Style_Base->setColor( palette.base().color() );
    ui->Frame_Style_AlternateBase->setColor( palette.alternateBase().color() );
    ui->Frame_Style_ToolTipBase->setColor( palette.toolTipBase().color() );
    ui->Frame_Style_ToolTipText->setColor( palette.toolTipText().color() );
    ui->Frame_Style_Text->setColor( palette.text().color() );
    ui->Frame_Style_Button->setColor( palette.button().color() );
    ui->Frame_Style_ButtonText->setColor( palette.buttonText().color() );
    ui->Frame_Style_BrightText->setColor( palette.brightText().color() );
    ui->Frame_Style_Link->setColor( palette.link().color() );
    ui->Frame_Style_Highlight->setColor( palette.highlight().color() );
    ui->Frame_Style_HighlightedText->setColor( palette.highlightedText().color() );
}

/*! \~russian
 * \brief Метод для получения палитры на основе ColorPicker'а
 * \return палитра с набором цветов из ColorPicker
 */
QPalette Settings::paletteFromColorPicker()
{
    QPalette palette;

    palette.setColor(QPalette::Window, ui->Frame_Style_Window->color() );
    palette.setColor(QPalette::WindowText, ui->Frame_Style_WindowText->color() );
    palette.setColor(QPalette::Base, ui->Frame_Style_Base->color());
    palette.setColor(QPalette::AlternateBase, ui->Frame_Style_AlternateBase->color());
    palette.setColor(QPalette::ToolTipBase, ui->Frame_Style_ToolTipBase->color());
    palette.setColor(QPalette::ToolTipText, ui->Frame_Style_ToolTipText->color());
    palette.setColor(QPalette::Text, ui->Frame_Style_Text->color());
    palette.setColor(QPalette::Button, ui->Frame_Style_Button->color());
    palette.setColor(QPalette::ButtonText, ui->Frame_Style_ButtonText->color());
    palette.setColor(QPalette::BrightText, ui->Frame_Style_BrightText->color());
    palette.setColor(QPalette::Link, ui->Frame_Style_Link->color());
    palette.setColor(QPalette::Highlight, ui->Frame_Style_Highlight->color());
    palette.setColor(QPalette::HighlightedText, ui->Frame_Style_HighlightedText->color());

    return palette;
}

/*! \~russian
 * \brief Метод проверки дублирования имени стиля
 * \param name - имя для проверки
 * \return true в случае если параметр совпадает с одним из пунктов в выпадающем списке стилей.
 *
 * Метод проверяет имеется ли переданное имя в списке стилей и возвращает истину в случае если
 * указанное имя уже имеется.
 */
bool Settings::duplicateStyleName(QString name)
{
    const int dontFind = -1;
    if( ui->ComboBox_CurrentStyle->findText(name) == dontFind ){
        return false;
    }
    return true;
}

void Settings::setApplicationStyle(QPalette palette)
{
    qApp->setStyle( QStyleFactory::create("Fusion") );
    qApp->setPalette( palette );
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

/*! \~russian
 * \brief Метод - нажатия на кнопку выбора рабочей директории.
 *
 * Метод отображает стандартный диалог выбора директории,
 * позволяя пользователю выбрать место в файловой системе,
 * где будут храниться файлы приложения.
 *
 * При отображении диалогового окна, инстанция задаётся в соответствии
 * с текущим значением поля ввода рабочей директории.
 *
 * При нажатии кнопки отмены, не изменяет текущее значение поля ввода
 * рабочей директории.
 */
void Settings::on_TBtn_WorkDir_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for application data"),
                                                    ui->LineEdit_WorkDir->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_WorkDir->setText( DefinesPath::appData() );
    else
        ui->LineEdit_WorkDir->setText( dir );
}

/*! \~russian
 * \brief Метод - изменение текущего активного стиля в выпадающем списке.
 * \param index - текущий выбранный индекс
 *
 * Если выбранный стиль является системным(INDEX_OF_SYSTEM_STYLE)
 * блокирует возможность изменения цветовой палитры.
 *
 * Иначе - разблокирует возможность изменения цветовой палитры.
 *
 * \todo Загружает файл в соответствии с выбранным стилем и
 * инициализирует цветовую палитру соответствующими цветами.
 */
void Settings::on_ComboBox_CurrentStyle_currentIndexChanged(int index)
{
    ui->GroupBox_Style_Colors->setEnabled( index );
    ui->PButton_Style_SaveChanges->setVisible( false );
    if( index != INDEX_OF_SYSTEM_STYLE ){
        stylePalette = StyleManager::getPaletteOfStyle( ui->ComboBox_CurrentStyle->currentText() );
        initColorPickers( stylePalette );
        setApplicationStyle( stylePalette );
    }
}

/*! \~russian
 * \brief Метод - клик на кнопку удаления выбранного стиля.
 *
 * Если выбранный стиль - системный (INDEX_OF_SYSTEM_STYLE)
 * Отображает сообщение пользователю о невозможности его удаления и
 * завершает выполнение функции.
 *
 * Иначе - удаляет выбранный пункт из выпадаюзего списка.
 * Запрашивает подтверждение у пользователя.
 * Удаляет файл с таким именем, содержащий этот стиль.
 */
void Settings::on_TButton_RemoveStyle_clicked()
{
    if( ui->ComboBox_CurrentStyle->currentIndex() == INDEX_OF_SYSTEM_STYLE ){
        QMessageBox::information(this, tr("Warning"), tr("It is impossible to delete system style"));
        return;
    }

    QMessageBox::StandardButton pressedButton
            = QMessageBox::question(this, tr("Question"),
                                    tr("You really want to remove the current style?"));

    if( pressedButton == QMessageBox::Yes ){
        bool successRemoved = StyleManager::removeStyle( ui->ComboBox_CurrentStyle->currentText() );

        if( ! successRemoved ){
            /// \todo Set here the normal text
            QMessageBox::information(this, tr("Warning"), tr("Impossible to remove this style. Maybe your not accesss to removed."));
        }else{
            ui->ComboBox_CurrentStyle->removeItem( ui->ComboBox_CurrentStyle->currentIndex() );
        }
    }
}

/*! \~russian
 * \brief Метод - клик на кнопку копирования выбранного стиля
 *
 * Отображает диалог ввода имени для нового стиля,
 * проверяет валидность нового имени - не может быть пустым,
 * не может дублировать уже имеющееся имя, не может состоять из пробелов.
 *
 * Если не валидно, обображает соответствующее сообщение пользователю
 * и выходит из функции.
 *
 * Если валидно - добавляет новый пункт в выпадающий список стилей и делает его текущим.
 * Cоздаёт новый файл со стилем.
 */
void Settings::on_TButton_CopyStyle_clicked()
{
    bool ok = false;

    QString styleName = QInputDialog::getText(this, tr("Style copying"),
                                         tr("Enter name for new style:"), QLineEdit::Normal,
                                         ui->ComboBox_CurrentStyle->currentText(), &ok);

    styleName = styleName.trimmed();

    if( ! ok ){
        return;
    }
    if( styleName.isEmpty() ){
        QMessageBox::information(this, tr("Warning"), tr("The entered name is empty") );
        return;
    }
    if ( duplicateStyleName( styleName ) ){
        QMessageBox::information(this, tr("Warning"), tr("Style with such name already exists") );
        return;
    }

    StyleManager::saveStyle(styleName, paletteFromColorPicker() );

    ui->ComboBox_CurrentStyle->addItem(styleName);
    ui->ComboBox_CurrentStyle->setCurrentIndex( ui->ComboBox_CurrentStyle->findText(styleName) );
    ui->PButton_Style_SaveChanges->setVisible( false );
}

void Settings::on_PButton_Style_SaveChanges_clicked()
{
    QString styleName  = ui->ComboBox_CurrentStyle->currentText();
    int     styleIndex = ui->ComboBox_CurrentStyle->currentIndex();

    if( styleIndex != INDEX_OF_SYSTEM_STYLE )
        StyleManager::saveStyle( styleName, paletteFromColorPicker() );

    ui->PButton_Style_SaveChanges->setVisible( false );
}
