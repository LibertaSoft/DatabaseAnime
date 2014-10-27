#ifndef GLOBALENUM_H
#define GLOBALENUM_H

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}
namespace Filter {
    enum filter{all = 0, editing, wanttolook, tv, ova, ona, special, movie, looked};
}

namespace Tables {
    enum table{none = 0, vAnime, vManga, vAmv, vDorama};
}

namespace Sort{
    enum sort{none = 0, asc, desc, year};
}

#endif // GLOBALENUM_H
