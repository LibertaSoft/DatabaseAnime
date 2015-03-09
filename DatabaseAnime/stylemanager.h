#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QStringList>
#include <QSet>

class StyleManager
{
public:
    static QSet<QString> getExistsStyles();
    static QPalette getPaletteOfStyle(QString styleName);
    static bool saveStyle(QString styleName, QPalette palette);
    static bool removeStyle(QString styleName);
};

#endif // STYLEMANAGER_H
