#include "Memory/DefaultHeap.h"

#include "Logger.h"

#include <cstdlib>
#include <new>

namespace Momo
{
namespace Memory
{
void OutOfMemory()
{
	LOGE("Unable to allocate memory block! Aborting process.");
	abort();
}

DefaultHeap::DefaultHeap()
{
	//std::set_new_handler(OutOfMemory);
}

void* DefaultHeap::Allocate( size_t size )
{
	return malloc(size);
}

void DefaultHeap::Free( void* pBlock )
{
	free(pBlock);
}
}
}