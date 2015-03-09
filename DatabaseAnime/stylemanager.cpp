#include "definespath.h"
#include "stylemanager.h"

#include <QDebug>
#include <QPalette>
#include <QRgb>
#include <QSettings>

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

QPalette StyleManager::getPaletteOfStyle(QString styleName)
{
    QString fileName = DefinesPath::styleLocation() + QDir::separator() + styleName + ".ini";
    QSettings ini(fileName, QSettings::IniFormat);

    QPalette palette;

    QColor Window, WindowText, Base, AlternateBase, ToolTipBase,
            ToolTipText, Text, Button, ButtonText, BrightText,
            Link, Highlight, HighlightedText;


    Window.setNamedColor( ini.value("Window", "#000000" ).toString() );
    WindowText.setNamedColor( ini.value("WindowText", "#000000" ).toString() );
    Base.setNamedColor( ini.value("Base", "#000000" ).toString() );
    AlternateBase.setNamedColor( ini.value("AlternateBase", "#000000" ).toString() );
    ToolTipBase.setNamedColor( ini.value("ToolTipBase", "#000000" ).toString() );
    ToolTipText.setNamedColor( ini.value("ToolTipText", "#000000" ).toString() );
    Text.setNamedColor( ini.value("Text", "#000000" ).toString() );
    Button.setNamedColor( ini.value("Button", "#000000" ).toString() );
    ButtonText.setNamedColor( ini.value("ButtonText", "#000000" ).toString() );
    BrightText.setNamedColor( ini.value("BrightText", "#000000" ).toString() );
    Link.setNamedColor( ini.value("Link", "#000000" ).toString() );
    Highlight.setNamedColor( ini.value("Highlight", "#000000" ).toString() );
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

bool StyleManager::removeStyle(QString styleName)
{
    if( styleName.isEmpty() )
        return false;

    QString fileName = DefinesPath::styleLocation() + QDir::separator() + styleName + ".ini";
    return QFile().remove(fileName);
}
