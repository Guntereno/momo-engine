#include "Io/FileInMemory.h"

#include "Io/File.h"

#include <cstring>
#include <cstdlib>


namespace Momo
{
namespace Io
{
FileInMemory::FileInMemory(const char* pFileName) :
    mpData(NULL),
    mDataSize(0)
{
    File::Handle file = File::Open(pFileName, File::Mode::Read);
    ASSERT(file != NULL);

    if (file != NULL)
    {
        mDataSize = File::GetSize(file);
        mpData = (u8*)malloc(mDataSize + 1);
        memset(mpData, 0, mDataSize + 1);

        File::Read(file, mpData, mDataSize);
        mpData[mDataSize] = '\0';

        File::Close(file);
    }
}

FileInMemory::~FileInMemory()
{
    free(mpData);
}
}
}