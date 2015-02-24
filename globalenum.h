#ifndef GLOBALENUM_H
#define GLOBALENUM_H

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

#endif // GLOBALENUM_H
