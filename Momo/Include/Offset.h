#ifndef MOMO_OFFSET_INCLUDED
#define MOMO_OFFSET_INCLUDED

#include "Momo.h"


namespace Momo
{

// POD type representing a region offset (e.g. margin/padding information)
struct Offset
{
    enum class Side : u8
    {
        Top,
        Right,
        Bottom,
        Left,

        Count
    };

    static Offset Zero()
    {
        Offset zero;
        zero.Set(0);
        return zero;
    }

    void Set(s16 top, s16 right, s16 bottom, s16 left)
    {
        side[(int)Side::Top] = top;
        side[(int)Side::Right] = right;
        side[(int)Side::Bottom] = bottom;
        side[(int)Side::Left] = left;
    }

    void Set(s16 vertical, s16 horizontal)
    {
        side[(int)Side::Top] = side[(int)Side::Bottom] = vertical;
        side[(int)Side::Right] = side[(int)Side::Left] = horizontal;
    }

    void Set(s16 value)
    {
        side[(int)Side::Top] =
            side[(int)Side::Bottom] =
            side[(int)Side::Right] =
            side[(int)Side::Left] =
            value;
    }

    s16 side[(int)Side::Count];
};

POD_CHECK(Offset);

}

#endif //MOMO_OFFSET_INCLUDED
