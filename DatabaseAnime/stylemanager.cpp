#include "definespath.h"
#include "stylemanager.h"

#include <QDebug>
#include <QPalette>
#include <QRgb>
#include <QSettings>

/*! \~russian
 * \brief Метод для получения доступных стилей
 * \return QSet<QString> - список доступных стилей
 *
 * Метод читает директории со стилями и ищет .ini файлы стилей
 * после чего возвращает список найденных файлов без расширения.
 */
/*! \~english
 * \brief The method for obtaining available styles
 * \return QSet<QString> - the list of available styles
 *
 * The method reads directories with styles and looks for .ini the file of styles
 * then returns the list of the found files without expansion.
 */
QSet<QString> StyleManager::getExistsStyles()
{
    QSet<QString> styles;

    QStringList paths = DefinesPath::share();

    foreach (QString path, paths) {
        QString styleFolder = QDir(path).path() + QDir::separator() + "Style" + QDir::separator();
        foreach(QString fileName, QDir(styleFolder).entryList(QStringList() << "*.ini") ){
            fileName.replace(".ini", "");
            styles.insert( fileName );
        }
    }

    return styles;
}

/*! \~russian
 * \brief Метод для получения палитры указанного стиля
 * \param styleName - имя стиля, палитру которого требуется получить
 * \return QPalette - палитра
 */
/*! \~english
 * \brief the Method for receiving a palette of the specified style
 * \param styleName - a style name, which palette is required to be received
 * \return QPalette - a palette
 */
QPalette StyleManager::getPaletteOfStyle(QString styleName)
{
    /// \bug Read only writable locations
    QString fileName = DefinesPath::styleLocation() + styleName + ".ini";
    QSettings ini(fileName, QSettings::IniFormat);

    QPalette palette;

    QColor Window, WindowText, Base, AlternateBase, ToolTipBase,
            ToolTipText, Text, Button, ButtonText, BrightText,
            Link, Highlight, HighlightedText;


    Window.setNamedColor(          ini.value("Window",          "#353535" ).toString() );
    WindowText.setNamedColor(      ini.value("WindowText",      "#ffffff" ).toString() );
    Base.setNamedColor(            ini.value("Base",            "#191919" ).toString() );
    AlternateBase.setNamedColor(   ini.value("AlternateBase",   "#353535" ).toString() );
    ToolTipBase.setNamedColor(     ini.value("ToolTipBase",     "#ffffff" ).toString() );
    ToolTipText.setNamedColor(     ini.value("ToolTipText",     "#000000" ).toString() );
    Text.setNamedColor(            ini.value("Text",            "#ffffff" ).toString() );
    Button.setNamedColor(          ini.value("Button",          "#353535" ).toString() );
    ButtonText.setNamedColor(      ini.value("ButtonText",      "#ffffff" ).toString() );
    BrightText.setNamedColor(      ini.value("BrightText",      "#ff0000" ).toString() );
    Link.setNamedColor(            ini.value("Link",            "#2a82da" ).toString() );
    Highlight.setNamedColor(       ini.value("Highlight",       "#2a82da" ).toString() );
    HighlightedText.setNamedColor( ini.value("HighlightedText", "#000000" ).toString() );

    palette.setColor(QPalette::Window, Window);
    palette.setColor(QPalette::WindowText, WindowText);
    palette.setColor(QPalette::Base, Base);
    palette.setColor(QPalette::AlternateBase, AlternateBase);
    palette.setColor(QPalette::ToolTipBase, ToolTipBase);
    palette.setColor(QPalette::ToolTipText, ToolTipText);
    palette.setColor(QPalette::Text, Text);
    palette.setColor(QPalette::Button, Button);
    palette.setColor(QPalette::ButtonText, ButtonText);
    palette.setColor(QPalette::BrightText, BrightText);
    palette.setColor(QPalette::Link, Link);
    palette.setColor(QPalette::Highlight, Highlight);
    palette.setColor(QPalette::HighlightedText, HighlightedText);

    return palette;
}

/*! \~russian
 * \brief Метод для сохранения стиля, в пользовательскую директорию со стилями
 * \param styleName - имя сохраняемого стиля
 * \param palette - палитра для сохранения
 * \return bool - успех операции, false - если переданное имя пустое.
 */
/*! \~english
 * \brief the Method for style preservation, in the user directory with styles
 * \param styleName - a name of the kept style
 * \param palette - a palette for preservation
 * \return bool - success of operation, false - if the transferred name empty.
 */
bool StyleManager::saveStyle(QString styleName, QPalette palette)
{
    if( styleName.isEmpty() )
        return false;

    QString fileName = DefinesPath::styleLocation() + QDir::separator() + styleName + ".ini";
    QSettings ini(fileName, QSettings::IniFormat);

    ini.setValue("Window", 			palette.window().color().name() );
    ini.setValue("WindowText",		palette.windowText().color().name() );
    ini.setValue("Base", 			palette.base().color().name() );
    ini.setValue("AlternateBase", 	palette.alternateBase().color().name() );
    ini.setValue("ToolTipBase", 	palette.toolTipBase().color().name() );
    ini.setValue("ToolTipText", 	palette.toolTipText().color().name() );
    ini.setValue("Text", 			palette.text().color().name() );
    ini.setValue("Button", 			palette.button().color().name() );
    ini.setValue("ButtonText", 		palette.buttonText().color().name() );
    ini.setValue("BrightText", 		palette.brightText().color().name() );
    ini.setValue("Link", 			palette.link().color().name() );
    ini.setValue("Highlight", 		palette.highlight().color().name() );
    ini.setValue("HighlightedText", palette.highlightedText().color().name() );

    return true;
}

/*! \~russian
 * \brief Метод для удаления стиля
 * \param styleName - имя стиля который требуется удалить (без расширения)
 * \return bool - успех операции.
 */
/*! \~english
 * \brief the Method for remove of style
 * \param styleName - a name of style which is required to be removed (without expansion)
 * \return bool - success of operation.
 */
bool StyleManager::removeStyle(QString styleName)
{
    if( styleName.isEmpty() )
        return false;

    QString fileName = DefinesPath::styleLocation() + styleName + ".ini";
    return QFile().remove(fileName);
}
