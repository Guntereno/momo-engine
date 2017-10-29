#ifndef MOMO_IO_FILE_INCLUDED
#define MOMO_IO_FILE_INCLUDED

#include "Momo.h"

#include <cstdio>


namespace Momo
{
	namespace Io
	{

		class File
		{
		public:
			typedef void* Handle;

			enum Mode
			{
				kModeRead,
				kModeWrite
			};

			// pData is platform specific:
			//  on Android pass a pointer to the AssetManager
			static void Init(void *pData);

			static Handle Open(const char* pFileName, Mode);
			static void Close(Handle file);

			static size_t Read(Handle file, void* pBuf, size_t size);
			static size_t Write(const void* pBuf, size_t size, Handle file);

			static size_t GetSize(Handle file);
		};

	}
}

#endif //MOMO_IO_FILE_INCLUDED
