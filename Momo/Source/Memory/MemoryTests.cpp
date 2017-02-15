#include "Memory/MemoryTests.h"

#include "Memory/StackHeap.h"

#include <cstring>

namespace Momo
{
namespace Memory
{
bool PerformTests()
{
	bool success = true;

	const size_t kHeapSize = 256;
	char heap[kHeapSize];
	Momo::Memory::StackHeap stackHeap(heap, kHeapSize);

	const u8 kTestValue = 0xFF;
	const size_t kTestValue1Len = 16;
	u8* pMem1 = (u8*)stackHeap.Allocate(kTestValue1Len);
	memset(pMem1, kTestValue, kTestValue1Len);

	const char* kHeap1Name = "Heap 1";
	stackHeap.PushHeap(kHeap1Name);

	return success;
}
}
}