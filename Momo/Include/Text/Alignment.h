#ifndef MOMO_TEXT_ALIGNMENT_INCLUDED
#define MOMO_TEXT_ALIGNMENT_INCLUDED

#include "Momo.h"

namespace Momo
{
namespace Text
{

enum Alignment
{
	kHAlignLeft = 0x1,
	kHAlignRight = 0x2,
	kHAlignCenter = kHAlignLeft | kHAlignRight,

	kVAlignBottom = 0x4,
	kVAlignTop = 0x8,
	kVAlignCenter = kVAlignBottom | kVAlignTop,
};

}
}

#endif //MOMO_TEXT_ALIGNMENT_INCLUDED