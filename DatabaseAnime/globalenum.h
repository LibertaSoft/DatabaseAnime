#ifndef GLOBALENUM_H
#define GLOBALENUM_H

#include <QString>
#include "core/searchoutputtype.h"
#include "core/sorttype.h"
#include "core/filtertype.h"
#include "core/section.h"
/*#include <QMetaType>*/

//namespace sections {
//    enum section{none = 0, anime, manga, amv, dorama};
//}
/* #Note : Problems with QVariant */
/*enum class Section : int {None, Anime, Manga, Amv, Dorama};*/
/*Q_DECLARE_METATYPE(Section)*/



namespace Options{
    namespace General{
        const QString DISPLAYED_FIELD("DisplayedField");
        const QString LANGUAGE("Language");
        const QString LOCALE("Locale");
        const QString SORTING("Sorting");
        const QString SwitchCoverOrDir("SwitchCoverOrDir");
        const QString WorkDirectory("WorkDirectory");
        const QString ActiveSection("ActiveSection");
    }
    namespace ActiveSections{
        const QString SectionPrefix("ActiveSections/");
        const QString ANIME(SectionPrefix+"Anime");
        const QString MANGA(SectionPrefix+"Manga");
        const QString AMV(SectionPrefix+"Amv");
        const QString DORAMA(SectionPrefix+"Dorama");
    }
    namespace Network{
        const QString SectionPrefix("Network/");
        const QString CHECK_UPDATES(SectionPrefix+"CheckUpdates");
        const QString USE_SSL(SectionPrefix+"USE_SSL");
        const QString LIVE_SEARCH(SectionPrefix+"AutoSearchOnShikimori");
        const QString SEARCH_LIMIT(SectionPrefix+"SearchLimit");
        const QString SEARCH_OUTPUT(SectionPrefix+"SearchOutput");
        const QString RELOAD_COVERS(SectionPrefix+"DownloadCovers");
    }
    namespace Style{
        const QString SectionPrefix("Style/");
        const QString SystemStyle(SectionPrefix+"ActiveIsSystem");
        const QString CurrentStyleName(SectionPrefix+"ActiveStyleName");
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
