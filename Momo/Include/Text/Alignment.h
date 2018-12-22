#ifndef MOMO_TEXT_ALIGNMENT_INCLUDED
#define MOMO_TEXT_ALIGNMENT_INCLUDED

#include "Momo.h"

namespace Momo
{
	namespace Text
	{

		enum class Alignment : u8
		{
			HLeft = 0x1,
			HRight = 0x2,
			HCenter = HLeft | HRight,

			VBottom = 0x4,
			VTop = 0x8,
			VCenter = VBottom | VTop,
		};

		FLAG_OPS(Alignment)

	}
}

#endif //MOMO_TEXT_ALIGNMENT_INCLUDED