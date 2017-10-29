#ifndef MOMO_OFFSET_INCLUDED
#define MOMO_OFFSET_INCLUDED

#include "Momo.h"


namespace Momo
{

	// POD type representing a region offset (e.g. margin/padding information)
	struct Offset
	{
		enum Side
		{
			kSideTop,
			kSideRight,
			kSideBottom,
			kSideLeft,

			kSideCount
		};

		static Offset Zero()
		{
			Offset zero;
			zero.Set(0);
			return zero;
		}

		void Set(s16 top, s16 right, s16 bottom, s16 left)
		{
			side[kSideTop] = top;
			side[kSideRight] = right;
			side[kSideBottom] = bottom;
			side[kSideLeft] = left;
		}

		void Set(s16 vertical, s16 horizontal)
		{
			side[kSideTop] = side[kSideBottom] = vertical;
			side[kSideRight] = side[kSideLeft] = horizontal;
		}

		void Set(s16 value)
		{
			side[kSideTop] =
				side[kSideBottom] =
				side[kSideRight] =
				side[kSideLeft] =
				value;
		}

		s16 side[kSideCount];
	};

}

#endif //MOMO_OFFSET_INCLUDED
