#ifndef SERIESRANGE_H
#define SERIESRANGE_H


class SeriesRange
{
    int _current = 0;
    int _all     = 0;
public:
    SeriesRange();
    SeriesRange(int current, int all);
    int current() const;
    void setCurrent(int current);
    int all() const;
    void setAll(int all);
};

#endif // SERIESRANGE_H
