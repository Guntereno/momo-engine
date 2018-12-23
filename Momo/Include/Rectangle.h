#ifndef MOMO_RECTANGLE_INCLUDED
#define MOMO_RECTANGLE_INCLUDED

#include "Momo.h"

#include "Point.h"
#include "Offset.h"


namespace Momo
{

// POD type representing a 2D rectangle
struct Rectangle
{
    static Rectangle Zero()
    {
        Rectangle zero;
        zero.Set(0, 0, 0, 0);
        return zero;
    }

    s32 mX;
    s32 mY;
    s32 mWidth;
    s32 mHeight;

    void Set(s32 x, s32 y, s32 width, s32 height)
    {
        this->mX = x;
        this->mY = y;
        this->mWidth = width;
        this->mHeight = height;
    }

    s32 Left() const { return mX; }
    s32 Right() const { return mX + mWidth; }
    s32 Top() const { return mY + mHeight; }
    s32 Bottom() const { return mY; }

    Point Location()
    {
        Point location;
        location.Set(this->mX, this->mY);
        return location;
    }

    void Location(Point& out)
    {
        out.Set(this->mX, this->mY);
    }

    Point Center()
    {
        Point center;
        center.Set(this->mX + this->mWidth / 2, this->mY + this->mHeight / 2);
        return center;
    }

    void Center(Point& out)
    {
        out.Set(this->mX + this->mWidth / 2, this->mY + this->mHeight / 2);
    }

    bool Contains(int x, int y)
    {
        return
            this->mX <= x &&
            x < (this->mX + this->mWidth) &&
            this->mY <= y &&
            y < (this->mY + this->mHeight);
    }

    bool Contains(const Point& value)
    {
        return
            this->mX <= value.mX &&
            value.mX < (this->mX + this->mWidth) &&
            this->mY <= value.mY &&
            value.mY < this->mY + this->mHeight;
    }

    bool Contains(const Rectangle& value)
    {
        return
            this->mX <= value.mX &&
            value.mX + value.mWidth <= this->mX + this->mWidth &&
            this->mY <= value.mY &&
            value.mY + value.mHeight <= this->mY + this->mHeight;
    }

    bool Intersects(const Rectangle& value)
    {
        return
            value.mX < this->mX + this->mWidth &&
            this->mX < value.mX + value.mWidth &&
            value.mY < this->mY + this->mHeight &&
            this->mY < value.mY + value.mHeight;
    }

    void Translate(const Point& amount)
    {
        this->mX += amount.mX;
        this->mY += amount.mY;
    }

    void Translate(s32 offsetX, s32 offsetY)
    {
        this->mX += offsetX;
        this->mY += offsetY;
    }

    void Inflate(s32 horizontalAmount, s32 verticalAmount)
    {
        this->mX -= horizontalAmount;
        this->mY -= verticalAmount;
        this->mWidth += horizontalAmount * 2;
        this->mHeight += verticalAmount * 2;
    }

    void Inflate(const Offset& offset)
    {
        this->mX -= offset.side[(int)Offset::Side::Left];
        this->mY -= offset.side[(int)Offset::Side::Bottom];
        this->mWidth += (offset.side[(int)Offset::Side::Left] + offset.side[(int)Offset::Side::Right]);
        this->mHeight += (offset.side[(int)Offset::Side::Top] + offset.side[(int)Offset::Side::Bottom]);
    }

    void Deflate(const Offset& offset)
    {
        this->mX += offset.side[(int)Offset::Side::Left];
        this->mY += offset.side[(int)Offset::Side::Bottom];
        this->mWidth -= (offset.side[(int)Offset::Side::Left] + offset.side[(int)Offset::Side::Right]);
        this->mHeight -= (offset.side[(int)Offset::Side::Top] + offset.side[(int)Offset::Side::Bottom]);
    }

    static void Union(const Rectangle& value1, const Rectangle& value2, Rectangle& result)
    {
        s32 right1 = value1.mX + value1.mWidth;
        s32 right2 = value2.mX + value2.mWidth;
        s32 bottom1 = value1.mY + value1.mHeight;
        s32 bottom2 = value2.mY + value2.mHeight;

        s32 minLeft = (value1.mX < value2.mX) ? value1.mX : value2.mX;
        s32 minTop = (value1.mY < value2.mY) ? value1.mY : value2.mY;
        s32 maxRight = (right1 > right2) ? right1 : right2;
        s32 maxBottom = (bottom1 > bottom2) ? bottom1 : bottom2;

        result.mX = minLeft;
        result.mY = minTop;
        result.mWidth = maxRight - minLeft;
        result.mHeight = minTop - maxBottom;
    }

    bool operator==(const Rectangle& rhs)
    {
        return
            (this->mX == rhs.mX) &&
            (this->mY == rhs.mY) &&
            (this->mWidth == rhs.mWidth) &&
            (this->mHeight == rhs.mHeight);
    }

    bool operator!=(const Rectangle& rhs)
    {
        return
            (this->mX != rhs.mX) ||
            (this->mY != rhs.mY) ||
            (this->mWidth != rhs.mWidth) ||
            (this->mHeight != rhs.mHeight);
    }
};

POD_CHECK(Rectangle);
}

#endif //MOMO_RECTANGLE_INCLUDED
