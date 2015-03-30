#include "seriesrange.h"


int SeriesRange::current() const
{
    return _current;
}

void SeriesRange::setCurrent(int current)
{
    if( current < 0 )
        return;
    if( current > _all )
        return;

    _current = current;
}

int SeriesRange::all() const
{
    return _all;
}

void SeriesRange::setAll(int all)
{
    if( all < 0 )
        return;

    _all = all;
}
SeriesRange::SeriesRange()
{}

SeriesRange::SeriesRange(int current, int all)
{
    if( all < 0 || current < 0 )
        return;
    if( current > all )
        return;

    _all     = all;
    _current = current;
}
