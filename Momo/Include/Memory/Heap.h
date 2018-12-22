#ifndef MOMO_MEMORY_HEAP_INCLUDED
#define MOMO_MEMORY_HEAP_INCLUDED

#include "Momo.h"


namespace Momo
{
	namespace Memory
	{

		// Base interface for all memory heaps
		class Heap
		{
		public:
			static constexpr u32 kDefaultAlignment = 32;

			virtual void* Allocate(size_t size) = 0;
			virtual void Free(void* pBlock) = 0;
		};

	}
}

#endif // MOMO_MEMORY_HEAP_INCLUDED
