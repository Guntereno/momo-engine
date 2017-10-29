#ifdef WIN32

#include "Io/File.h"
#include "AssertBreak.h"
#include "Logger.h"

#include <cstdio>


namespace Momo
{
	namespace Io
	{
		static const char* kDebugPrefix = "Momo::Io::File";

		void File::Init(void* pData)
		{
			LOGI("%s: Initialising", kDebugPrefix);

			// Do nothing
		}

		File::Handle File::Open(const char* fileName, Mode mode)
		{
			ASSERT(fileName != NULL);

			LOGI("%s: Opening file %s", kDebugPrefix, fileName);

			const char* modeString = NULL;
			switch (mode)
			{
			case kModeRead:
				modeString = "rb";
				break;

			case kModeWrite:
				modeString = "wb";
				break;

			default:
				BREAK_MSG("Unhandled file mode %d!", mode);
			}

			FILE* pFile = fopen(fileName, modeString);

			if (pFile == NULL)
			{
				perror(fileName);
			}

			return reinterpret_cast<Handle>(pFile);
		}

		void File::Close(File::Handle file)
		{
			if (file == NULL)
				return;

			FILE* pFile = reinterpret_cast<FILE*>(file);

			LOGI("%s: Closing file handle 0x%x", kDebugPrefix, pFile);

			int result = fclose(pFile);
			ASSERT(result == 0);
		}

		size_t File::Read(File::Handle file, void* pBuf, size_t size)
		{
			if (file == NULL)
				return 0;

			ASSERT(pBuf != NULL);

			FILE* pFile = reinterpret_cast<FILE*>(file);

			const size_t kNumBytes = 1;
			size_t read = fread(pBuf, kNumBytes, size, pFile);

			return read;
		}

		size_t File::Write(const void* pBuf, size_t size, Handle file)
		{
			ASSERT(pBuf != NULL);
			ASSERT(size > 0);
			ASSERT(file != NULL);

			FILE* pFile = reinterpret_cast<FILE*>(file);

			const size_t kNumBytes = 1;
			size_t written = fwrite(pBuf, kNumBytes, size, pFile);

			return written;
		}

		size_t File::GetSize(Handle file)
		{
			int result;

			if (file == NULL)
				return 0;

			FILE* pFile = reinterpret_cast<FILE*>(file);

			// store current pos
			size_t initial = ftell(pFile);

			// seek to the end
			result = fseek(pFile, 0L, SEEK_END);
			ASSERT(result == 0);

			size_t size = ftell(pFile);

			// seek back to the initial
			result = fseek(pFile, initial, SEEK_SET);
			ASSERT(result == 0);

			return size;
		}
	}
}

#endif // WIN32