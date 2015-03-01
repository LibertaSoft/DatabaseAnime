#ifndef DBASETTINGS_H
#define DBASETTINGS_H

#include <QSettings>

namespace Configs{
    namespace General{
        enum key{DisplayedField, Language, Sorting, SwitchCoverOrDir, WorkDirectory, ActiveSection};
    }
    namespace ActiveSections{
        enum key{Anime, Manga, Amv, Dorama};
    }
    namespace OptionalFields{
        namespace Anime{
            enum key{AltTitle, Director, Postscoring};
        }
        namespace Manga{
            enum key{AltTitle, Author, Translation};
        }
        namespace Dorama{
            enum key{AltTitle, Director};
        }
    }
    namespace DialogsSettings{
        enum key{AnimeGeometry, AnimeState,
             MangaGeometry, MangaState,
             AmvGeometry, AmvState,
             DoramaGeomety, DoramaState,
             ConfigGeometry, ConfigState, ConfigSplitterState, ConfigSplitterGeometry,
             MainGeometry, MainState, MainSplitterState, MainSplitterGeometry};
    }
    namespace Network{
        enum key{CheckUpdates, AutoSearchOnShikimori};
    }
}

class DbaSettings : public QSettings
{
    Q_OBJECT
private:
    static QString keyToString(Configs::ActiveSections::key _key);
    static QString keyToString(Configs::DialogsSettings::key _key);
    static QString keyToString(Configs::General::key _key);
    static QString keyToString(Configs::Network::key _key);
    static QString keyToString(Configs::OptionalFields::Anime::key _key);
    static QString keyToString(Configs::OptionalFields::Manga::key _key);
    static QString keyToString(Configs::OptionalFields::Dorama::key _key);
public:
    QVariant value(const Configs::ActiveSections::key &key,         const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::DialogsSettings::key &key,        const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::General::key &key,                const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::Network::key &key,                const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::OptionalFields::Anime::key &key,  const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::OptionalFields::Manga::key &key,  const QVariant &defaultValue = QVariant() ) const;
    QVariant value(const Configs::OptionalFields::Dorama::key &key, const QVariant &defaultValue = QVariant() ) const;

    void setValue(const Configs::ActiveSections::key         &key, const QVariant &value );
    void setValue(const Configs::DialogsSettings::key        &key, const QVariant &value );
    void setValue(const Configs::General::key                &key, const QVariant &value );
    void setValue(const Configs::Network::key                &key, const QVariant &value );
    void setValue(const Configs::OptionalFields::Anime::key  &key, const QVariant &value );
    void setValue(const Configs::OptionalFields::Manga::key  &key, const QVariant &value );
    void setValue(const Configs::OptionalFields::Dorama::key &key, const QVariant &value );

    void remove(const Configs::ActiveSections::key         &key);
    void remove(const Configs::DialogsSettings::key        &key);
    void remove(const Configs::General::key                &key);
    void remove(const Configs::Network::key                &key);
    void remove(const Configs::OptionalFields::Anime::key  &key);
    void remove(const Configs::OptionalFields::Manga::key  &key);
    void remove(const Configs::OptionalFields::Dorama::key &key);

signals:

public slots:

};

#endif // DBASETTINGS_H
