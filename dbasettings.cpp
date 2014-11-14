#include "dbasettings.h"

/// Методы keyToString преобразуют перечисления описанные в пространствах имён к строке
/// Отвечающей за указанную опцию

QString DbaSettings::keyToString(Configs::ActiveSections::key _key)
{
    using namespace Configs::ActiveSections;
    const QString prefix("ActiveSections/");
    switch( _key ){
    case Anime :
        return prefix + "Anime";
    case Manga :
        return prefix + "Manga";
    case Amv :
        return prefix + "Amv";
    case Dorama :
        return prefix + "Dorama";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::DialogsSettings::key _key)
{
    using namespace Configs::DialogsSettings;
    const QString prefix("DialogsSettings/");
    switch( _key ){
    case AnimeGeometry :
        return prefix + "AnimeGeometry";
    case AnimeState :
        return prefix + "AnimeState";
    case MangaGeometry :
        return prefix + "MangaGeometry";
    case MangaState :
        return prefix + "MangaState";
    case AmvGeometry :
        return prefix + "AmvGeometry";
    case AmvState :
        return prefix + "AmvState";
    case DoramaGeomety :
        return prefix + "DoramaGeomety";
    case DoramaState :
        return prefix + "DoramaState";
    case MainGeometry :
        return prefix + "MainGeometry";
    case MainState :
        return prefix + "MainState";
    case MainSplitterGeometry :
        return prefix + "MainSplitterGeometry";
    case MainSplitterState :
        return prefix + "MainSplitterState";
    case ConfigGeometry :
        return prefix + "ConfigGeometry";
    case ConfigState :
        return prefix + "ConfigState";
    case ConfigSplitterGeometry :
        return prefix + "ConfigSplitterGeometry";
    case ConfigSplitterState :
        return prefix + "ConfigSplitterState";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::General::key _key)
{
    using namespace Configs::General;
    const QString prefix("General/");
    switch( _key ){
    case ActiveSection :
        return prefix + "ActiveSection";
    case DisplayedField :
        return prefix + "DisplayedField";
    case Language :
        return prefix + "Language";
    case Sorting :
        return prefix + "Sorting";
    case SwitchCoverOrDir :
        return prefix + "SwitchCoverOrDir";
    case WorkDirectory :
        return prefix + "WorkDirectory";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::Network::key _key)
{
    using namespace Configs::Network;
    const QString prefix("Network/");
    switch( _key ){
    case AutoSearchOnShikimori :
        return prefix + "AutoSearchOnShikimori";
    case CheckUpdates :
        return prefix + "CheckUpdates";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::OptionalFields::Anime::key _key)
{
    using namespace Configs::OptionalFields::Anime;
    const QString prefix("OptionalFields/Anime/");
    switch( _key ){
    case AltTitle :
        return prefix + "AltTitle";
    case Director :
        return prefix + "Director";
    case Postscoring :
        return prefix + "Postscoring";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::OptionalFields::Manga::key _key)
{
    using namespace Configs::OptionalFields::Manga;
    const QString prefix("OptionalFields/Manga/");
    switch( _key ){
    case AltTitle :
        return prefix + "AltTitle";
    case Author :
        return prefix + "Author";
    case Translation :
        return prefix + "Translation";
    }
    return QString::null;
}

QString DbaSettings::keyToString(Configs::OptionalFields::Dorama::key _key)
{
    using namespace Configs::OptionalFields::Dorama;
    const QString prefix("OptionalFields/Dorama/");
    switch( _key ){
    case AltTitle :
        return prefix + "AltTitle";
    case Director :
        return prefix + "Director";
    }
    return QString::null;
}

/// Конструкторы представляют интерфейс для инициализации базового класса QSettings

DbaSettings::DbaSettings(const QString &organization, const QString &application, QObject *parent)
    :QSettings(organization, application, parent)
{}

DbaSettings::DbaSettings(QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
    :QSettings(scope, organization, application, parent)
{}

DbaSettings::DbaSettings(QSettings::Format format, QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent)
    :QSettings(format, scope, organization, application, parent)
{}

DbaSettings::DbaSettings(const QString &fileName, QSettings::Format format, QObject *parent)
    :QSettings(fileName, format, parent)
{}

DbaSettings::DbaSettings(QObject *parent) :
    QSettings(parent)
{}

/// Функции value представляют интерфейс для функции QSettings::value, но принимающий как параметр
/// Значения перечислений, заместо строки отвечающей за указанное свойство
/// Затем этот параметр преобразуется в строку при помощи приватных функций keyToString
/// После чего отправляется в стандартную функцию QSettings::value

QVariant DbaSettings::value(const Configs::ActiveSections::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::DialogsSettings::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::General::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::Network::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::OptionalFields::Anime::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::OptionalFields::Manga::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

QVariant DbaSettings::value(const Configs::OptionalFields::Dorama::key &key, const QVariant &defaultValue) const
{
    return QSettings::value(keyToString(key), defaultValue);
}

/// Функции setValue предоставляют интерфейс к функции класса QSettings::setValue
/// Но принимают не строку-ключ, а перечисление, которое в последствии
/// Преобразовывается к строке и отправляется в функцию QSettings::setValue

void DbaSettings::setValue(const Configs::ActiveSections::key &key, const QVariant &value)
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::DialogsSettings::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::General::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::Network::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::OptionalFields::Anime::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::OptionalFields::Manga::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::setValue(const Configs::OptionalFields::Dorama::key &key, const QVariant &value )
{
    QSettings::setValue(keyToString(key), value);
}

void DbaSettings::remove(const Configs::ActiveSections::key &key)
{
    QSettings::remove(keyToString(key));
}
void DbaSettings::remove(const Configs::DialogsSettings::key &key)
{
    QSettings::remove(keyToString(key));
}

void DbaSettings::remove(const Configs::General::key &key)
{
    QSettings::remove(keyToString(key));
}

void DbaSettings::remove(const Configs::Network::key &key)
{
    QSettings::remove(keyToString(key));
}

void DbaSettings::remove(const Configs::OptionalFields::Anime::key &key)
{
    QSettings::remove(keyToString(key));
}

void DbaSettings::remove(const Configs::OptionalFields::Manga::key &key)
{
    QSettings::remove(keyToString(key));
}

void DbaSettings::remove(const Configs::OptionalFields::Dorama::key &key)
{
    QSettings::remove(keyToString(key));
}
