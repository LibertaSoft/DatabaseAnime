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
    namespace AnimeField{
        enum field{all, id, isHaveLooked, isEditingDone, isAdult, Title, AltTitle, Director,
                   PostScoring, SeriesTV, SeriesOVA, SeriesONA, SeriesSpecial, SeriesMovie,
                   vSeriesTV, vSeriesOVA, vSeriesONA, vSeriesSpecial, vSeriesMovie, Score,
                   Year, Season, Studios, Tags, Description, Url, Dir, ImagePath
                  };
    }
    namespace MangaField{
        enum field{all, id, isHaveLooked, isEditingDone, isAdult, Title, AltTitle, Author,
                   Translation, Vol, Ch, Pages, vVol, vCh, vPages, Score, Year,
                   Tags, Description, Url, Dir, ImagePath
                  };
    }
    namespace AmvField{
        enum field{all, id, isEditingDone, isAdult, Title, Author, Contestant, Score, Year, Tags,
                   ContainingMusic, ContainingAnime, AuthorComment, Url, Dir, ImagePath
                  };
    }
    namespace DoramaField{
        enum field{all, id, isHaveLooked, isEditingDone, isAdult, Title, AltTitle, Director,
                   SeriesTV, SeriesSpecial, SeriesMovie, vSeriesTV, vSeriesSpecial, vSeriesMovie,
                   Score, Year, Season, Tags, Description, Actors, Url, Dir, ImagePath
                  };
    }
}

namespace Sort{
    enum sort{none = 0, asc, desc, year};
}

#endif // GLOBALENUM_H
