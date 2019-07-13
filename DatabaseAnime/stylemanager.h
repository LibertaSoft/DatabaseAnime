#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QStringList>
#include <QPalette>
#include <QSet>

/*! \~russian
 * \brief Класс управления стилями приложения
 */
/*! \~english
 * \brief Class for management of application styles
 */
class StyleManager
{
public:
    static QSet<QString> getExistsStyles();
    static QString getFilePath(QString styleName);
    static QPalette getPaletteOfStyle(QString styleName);
    static bool saveStyle(QString styleName, QPalette palette);
    static bool removeStyle(QString styleName);
};

#endif // STYLEMANAGER_H
