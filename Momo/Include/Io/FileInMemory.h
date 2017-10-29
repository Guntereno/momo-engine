#ifndef MOMO_IO_FILEINMEMORY_INCLUDED
#define MOMO_IO_FILEINMEMORY_INCLUDED

#include "Momo.h"

namespace Momo
{
namespace Io
{

class FileInMemory
{
public:
	FileInMemory(const char* pFileName);
	virtual ~FileInMemory();

	const u8* GetData() { return mpData; }
	const size_t GetSize() { return mDataSize; }

private:
	DISALLOW_COPY_AND_ASSIGN(FileInMemory);

	u8* mpData;
	size_t mDataSize;
};

}
}

#endif //MOMO_IO_FILEINMEMORY_INCLUDED
