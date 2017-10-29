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

		int x;
		int y;

		void Set(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		bool operator==(const Point &rhs)
		{
			return
				(this->x == rhs.x) &&
				(this->y == rhs.y);
		}

		bool operator!=(const Point &rhs)
		{
			return
				(this->x != rhs.x) ||
				(this->y != rhs.y);
		}
	};

}

#endif //MOMO_POINT_INCLUDED
