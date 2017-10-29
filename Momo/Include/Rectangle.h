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

	int x;
	int y;
	int width;
	int height;

	void Set(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	int Left() const { return x; }
	int Right() const { return x + width; }
	int Top() const { return y + height; }
	int Bottom() const { return y; }

	Point Location()
	{
		Point location;
		location.Set(this->x, this->y);
		return location;
	}

	void Location(Point& out)
	{
		out.Set(this->x, this->y);
	}

	Point Center()
	{
		Point center;
		center.Set(this->x + this->width / 2, this->y + this->height / 2);
		return center;
	}

	void Center(Point& out)
	{
		out.Set(this->x + this->width / 2, this->y + this->height / 2);
	}

	bool Contains(int x, int y)
	{
		return
			this->x <= x &&
			x < (this->x + this->width) &&
			this->y <= y &&
			y < (this->y + this->height);
	}

	bool Contains(const Point& value)
	{
		return
			this->x <= value.x &&
			value.x < (this->x + this->width) &&
			this->y <= value.y &&
			value.y < this->y + this->height;
	}

	bool Contains(const Rectangle& value)
	{
		return
			this->x <= value.x &&
			value.x + value.width <= this->x + this->width &&
			this->y <= value.y &&
			value.y + value.height <= this->y + this->height;
	}

	bool Intersects(const Rectangle& value)
	{
		return
			value.x < this->x + this->width &&
			this->x < value.x + value.width &&
			value.y < this->y + this->height &&
			this->y < value.y + value.height;
	}

	void Translate(const Point& amount)
	{
		this->x += amount.x;
		this->y += amount.y;
	}

	void Translate(int offsetX, int offsetY)
	{
		this->x += offsetX;
		this->y += offsetY;
	}

	void Inflate(int horizontalAmount, int verticalAmount)
	{
		this->x -= horizontalAmount;
		this->y -= verticalAmount;
		this->width += horizontalAmount * 2;
		this->height += verticalAmount * 2;
	}

	void Inflate(const Offset& offset)
	{
		this->x -= offset.side[Offset::kSideLeft];
		this->y -= offset.side[Offset::kSideBottom];
		this->width += (offset.side[Offset::kSideLeft] + offset.side[Offset::kSideRight]);
		this->height += (offset.side[Offset::kSideTop] + offset.side[Offset::kSideBottom]);
	}

	void Deflate(const Offset& offset)
	{
		this->x += offset.side[Offset::kSideLeft];
		this->y += offset.side[Offset::kSideBottom];
		this->width -= (offset.side[Offset::kSideLeft] + offset.side[Offset::kSideRight]);
		this->height -= (offset.side[Offset::kSideTop] + offset.side[Offset::kSideBottom]);
	}

	static void Union(const Rectangle& value1, const Rectangle& value2, Rectangle& result)
	{
		int right1 = value1.x + value1.width;
		int right2 = value2.x + value2.width;
		int bottom1 = value1.y + value1.height;
		int bottom2 = value2.y + value2.height;

		int minLeft = (value1.x < value2.x) ? value1.x : value2.x;
		int minTop = (value1.y < value2.y) ? value1.y : value2.y;
		int maxRight = (right1 > right2) ? right1 : right2;
		int maxBottom = (bottom1 > bottom2) ? bottom1 : bottom2;

		result.x = minLeft;
		result.y = minTop;
		result.width = maxRight - minLeft;
		result.height = minTop - maxBottom;
	}

	bool operator==(const Rectangle& rhs)
	{
		return
			(this->x == rhs.x) &&
			(this->y == rhs.y) &&
			(this->width == rhs.width) &&
			(this->height == rhs.height);
	}

	bool operator!=(const Rectangle& rhs)
	{
		return
			(this->x != rhs.x) ||
			(this->y != rhs.y) ||
			(this->width != rhs.width) ||
			(this->height != rhs.height);
	}
};

}

#endif //MOMO_RECTANGLE_INCLUDED
