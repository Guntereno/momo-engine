#ifndef MOMO_MEMORY_DEFAULTHEAP_INCLUDED
#define MOMO_MEMORY_DEFAULTHEAP_INCLUDED

#include "Momo.h"

#include "Memory/Heap.h"

namespace Momo
{
namespace Memory
{

// DefaultHeap provides an interface to the default new and delete operators
class DefaultHeap: public Heap
{
public:
	DefaultHeap();

	virtual void* Allocate(size_t size);
	virtual void Free(void* pBlock);

private:
	DISALLOW_COPY_AND_ASSIGN(DefaultHeap);
};

}
}

#endif //MOMO_MEMORY_DEFAULTHEAP_INCLUDED
