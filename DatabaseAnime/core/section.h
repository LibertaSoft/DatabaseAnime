#ifndef SECTION_H
#define SECTION_H

/* #Note : Problems with QVariant */
/*enum class Section : int {None, Anime, Manga, Amv, Dorama};*/
/*Q_DECLARE_METATYPE(Section)*/

namespace sections {
    enum section{none = 0, anime, manga, amv, dorama};
}

#endif // SECTION_H
