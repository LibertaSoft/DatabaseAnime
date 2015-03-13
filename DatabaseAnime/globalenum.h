#ifndef GLOBALENUM_H
#define GLOBALENUM_H

#include <QString>
/*#include <QMetaType>*/

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}
/* #Note : Problems with QVariant */
/*enum class Section : int {None, Anime, Manga, Amv, Dorama};*/
/*Q_DECLARE_METATYPE(Section)*/

namespace Filter {
    enum filter{all = 0, editing, wanttolook, tv, ova, ona, special, movie, looked};
}

namespace Sort{
    enum sort{none = 0, asc, desc, year};
}

namespace Options{
    namespace General{
        const QString DisplayedField("DisplayedField");
        const QString Language("Language");
        const QString Sorting("Sorting");
        const QString SwitchCoverOrDir("SwitchCoverOrDir");
        const QString WorkDirectory("WorkDirectory");
        const QString ActiveSection("ActiveSection");
    }
    namespace ActiveSections{
        const QString SectionPrefix("ActiveSections/");
        const QString Anime(SectionPrefix+"Anime");
        const QString Manga(SectionPrefix+"Manga");
        const QString Amv(SectionPrefix+"Amv");
        const QString Dorama(SectionPrefix+"Dorama");
    }
    namespace Network{
        const QString SectionPrefix("Network/");
        const QString CheckUpdates(SectionPrefix+"CheckUpdates");
        const QString AutoSearchOnShikimori(SectionPrefix+"AutoSearchOnShikimori");
    }
    namespace Style{
        const QString SectionPrefix("Style/");
        const QString CurrentStyle(SectionPrefix+"Current");
        const QString CurrentStyleName(SectionPrefix+"CurrentName");
    }
    namespace OptionalFields{
        const QString SectionPrefix("OptionalFields/");
        namespace Anime{
            const QString SectionPrefix(OptionalFields::SectionPrefix+"Anime/");
            const QString AltTitle(SectionPrefix+"AltTitle");
            const QString Director(SectionPrefix+"Director");
            const QString Postscoring(SectionPrefix+"Postscoring");
        }
        namespace Manga{
            const QString SectionPrefix(OptionalFields::SectionPrefix+"Manga/");
            const QString AltTitle(SectionPrefix+"AltTitle");
            const QString Author(SectionPrefix+"Author");
            const QString Translator(SectionPrefix+"Translation");
        }
        namespace Dorama{
            const QString SectionPrefix(OptionalFields::SectionPrefix+"Dorama/");
            const QString AltTitle(SectionPrefix+"AltTitle");
            const QString Director(SectionPrefix+"Director");
        }
    }
    namespace Dialogs{
        const QString SectionPrefix("Dialogs/");
        namespace Anime{
            const QString SectionPrefix(Dialogs::SectionPrefix+"AddAnime/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
        }
        namespace Manga{
            const QString SectionPrefix(Dialogs::SectionPrefix+"AddManga/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
        }
        namespace Amv{
            const QString SectionPrefix(Dialogs::SectionPrefix+"AddAmv/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
        }
        namespace Dorama{
            const QString SectionPrefix(Dialogs::SectionPrefix+"AddDorama/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
        }
        namespace Config{
            const QString SectionPrefix(Dialogs::SectionPrefix+"Config/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
                namespace Splitter{
                    const QString SectionPrefix(Config::SectionPrefix+"Splitter/");
                    const QString Geometry(SectionPrefix+"Geometry");
                    const QString State(SectionPrefix+"State");
                }
        }
        namespace MainWindow {
            const QString SectionPrefix(Dialogs::SectionPrefix+"MainWindow/");
            const QString Geometry(SectionPrefix+"Geometry");
            const QString State(SectionPrefix+"State");
            namespace Splitter{
                const QString SectionPrefix(MainWindow::SectionPrefix+"Splitter/");
                const QString Geometry(SectionPrefix+"Geometry");
                const QString State(SectionPrefix+"State");
            }
        }
    }
}

#endif // GLOBALENUM_H