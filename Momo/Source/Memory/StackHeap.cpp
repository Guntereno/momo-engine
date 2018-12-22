#include "Memory/StackHeap.h"

#include "AssertBreak.h"

#include <cstddef>
#include <cstring>


namespace Momo
{
	namespace Memory
	{
		StackHeap::StackHeap(void* pBlock, size_t size) :
			mpHead((u8*)pBlock),
			mpTail((u8*)pBlock),
			mSize(size),
			mpCurrentHeap(NULL)
		{
			ASSERT(pBlock != NULL);
			ASSERT(size > 0);

			PushHeap("Head");
		}

		void* StackHeap::Allocate(size_t size)
		{
			u8* newTail = mpTail + size;
			if (newTail > (mpHead + mSize))
			{
				BREAK_MSG("Out of memory! Requested %d bytes have %d.",
					size,
					BytesRemaining());
				return NULL;
			}

			void* mem = mpTail;
			mpTail = newTail;
			return mem;
		}

		void StackHeap::Free(void* pBlock)
		{
			UNUSED(pBlock);
			// Do nothing, memory is freed when we pop the containing heap
		}

		void StackHeap::PushHeap(const char* pName)
		{
			// Allocate the header
			HeapHeader* newHeap = (HeapHeader*)mpTail;
			mpTail += sizeof(HeapHeader);

			// Allocate the name at the start of the heap
			size_t nameLen = strlen(pName);
			newHeap->pName = (char*)Allocate(nameLen + 1);
			if (newHeap->pName == NULL)
			{
				return;
			}
			strncpy(newHeap->pName, pName, BytesRemaining());

			// Link the heap
			newHeap->pPrevious = mpCurrentHeap;
			mpCurrentHeap = newHeap;
		}

		void StackHeap::PopHeap()
		{
			if (mpCurrentHeap->pPrevious == NULL)
			{
				BREAK_MSG("Attempting to pop head heap!");
				return;
			}

			mpTail = (u8*)mpCurrentHeap;
			mpCurrentHeap = mpCurrentHeap->pPrevious;
		}

		size_t StackHeap::BytesAllocated()
		{
			return (size_t)(mpTail - mpHead);
		}

		size_t StackHeap::BytesRemaining()
		{
			return mSize - BytesAllocated();
		}
	}
}