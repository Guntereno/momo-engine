#ifndef MOMO_POINT_INCLUDED
#define MOMO_POINT_INCLUDED

#include "Momo.h"


namespace Momo
{

// POD type representing a 2D Point
struct Point
{
    static Point Zero()
    {
        Point zero;
        zero.Set(0, 0);
        return zero;
    }

    int mX;
    int mY;

    void Set(int x, int y)
    {
        this->mX = x;
        this->mY = y;
    }

    bool operator==(const Point &rhs)
    {
        return
            (this->mX == rhs.mX) &&
            (this->mY == rhs.mY);
    }

    bool operator!=(const Point &rhs)
    {
        return
            (this->mX != rhs.mX) ||
            (this->mY != rhs.mY);
    }
};


POD_CHECK(Point);
}

#endif //MOMO_POINT_INCLUDED
