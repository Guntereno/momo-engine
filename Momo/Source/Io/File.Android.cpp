#ifdef ANDROID

#include "Io/File.h"
#include "AssertBreak.h"
#include "Logger.h"

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <cstring>

namespace Momo
{
namespace Io
{
static AAssetManager* gAssetManager = NULL;

static const char* kDebugPrefix = "Momo::Io::File";

void File::Init(void* pData)
{
	LOGI("%s: Initialising", kDebugPrefix);

	ASSERT(pData != NULL);
	gAssetManager = static_cast<AAssetManager*>(pData);
}

File::Handle File::Open(const char* fileName, Mode mode)
{
	ASSERT(gAssetManager != NULL);
	ASSERT(fileName != NULL);

	switch(mode)
	{
	case kModeRead:
		{
			LOGI("%s: Opening file %s with manager 0x%x", kDebugPrefix, fileName, (u32)gAssetManager);

			AAsset* pAsset = AAssetManager_open(gAssetManager, fileName, AASSET_MODE_UNKNOWN);

			LOGI("%s: File %s has handle 0x%x", kDebugPrefix, fileName, (u32)pAsset);

			return reinterpret_cast<Handle>(pAsset);
		}

	case kModeWrite:
		BREAK_MSG("File writing currently unsupported on Android!");
		break;

	default:
		BREAK_MSG("Unhandled file mode %d!", mode);
		break;
	}

	return NULL;
}

void File::Close(File::Handle file)
{
	if(file == NULL)
		return;

	ASSERT(gAssetManager != NULL);

	AAsset* pAsset = reinterpret_cast<AAsset*>(file);

	LOGI("%s: Closing file handle 0x%x", kDebugPrefix, (u32)pAsset);

	AAsset_close(pAsset);
}

size_t File::Read(File::Handle file, void* pBuf, size_t size)
{
	if(file == NULL)
		return 0;

	ASSERT(gAssetManager != NULL);

	ASSERT(pBuf != NULL);

	AAsset* pAsset = reinterpret_cast<AAsset*>(file);

	//LOGI("%s: Reading up to %d bytes from handle 0x%x", kDebugPrefix, size, pAsset);

	size_t read = AAsset_read(pAsset, pBuf, size);

	//LOGI("%s: Read %d bytes from handle 0x%x", kDebugPrefix, read, pAsset);

	return read;
}

size_t File::Write(const void* pBuf, size_t size, Handle file)
{
	BREAK_MSG("File writing not supported on Android!");
	return 0;
}

size_t File::GetSize(Handle file)
{
	ASSERT(gAssetManager != NULL);

	AAsset* pAsset = reinterpret_cast<AAsset*>(file);

	ASSERT(pAsset != NULL);

	return AAsset_getLength(pAsset);
}
}
}

#endif // ANDROID