#ifndef MOMO_MEMORY_STACKHEAP_INCLUDED
#define MOMO_MEMORY_STACKHEAP_INCLUDED

#include "Momo.h"

#include "Memory/Heap.h"

namespace Momo
{
namespace Memory
{

// StackHeap allocates memory contiguously and provides no delete functionality.
// Instead watermarks can be pushed and can then be popped to return to a
// previous state.
class StackHeap: public Heap
{
public:

	StackHeap(void* pBlock, size_t size);

	virtual void* Allocate(size_t size);
	virtual void Free(void* pBlock);

	void PushHeap(const char* pName);
	void PopHeap();

	size_t BytesAllocated();
	size_t BytesRemaining();

private:
	DISALLOW_COPY_AND_ASSIGN(StackHeap);

	u8* mpHead;
	u8* mpTail;
	size_t mSize;

	struct HeapHeader
	{
		char* pName;
		HeapHeader* pPrevious;
	};
	HeapHeader* mpCurrentHeap;
};

}
}

#endif //MOMO_MEMORY_STACKHEAP_INCLUDED
