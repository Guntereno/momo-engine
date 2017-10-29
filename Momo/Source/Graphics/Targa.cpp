#include "Graphics/Targa.h"

#include "AssertBreak.h"
#include "Graphics/Texture.h"
#include "Io/File.h"

#include <cstring>


namespace Momo
{
	namespace Graphics
	{

		Targa::Targa() :
			mpDescriptor(NULL),
			mpColorMap(NULL),
			mpData(NULL)
		{
			memset(&mHeader, 0, sizeof(Header));
		}

		Targa::~Targa()
		{
			if (mpDescriptor != NULL)
			{
				delete[] mpDescriptor;
			}

			if (mpColorMap != NULL)
			{
				delete[] mpColorMap;
			}

			if (mpData != NULL)
			{
				delete[] mpData;
			}
		}

		u8 Targa::ImageDescriptorByte(u8 bitsPerPixel, bool originInUpperLeft, Interleaving interleaving)
		{
			ASSERT(bitsPerPixel <= 8);

			u8 byte = 0;

			byte |= (bitsPerPixel & 0x7); // First three bits

			if (originInUpperLeft)
			{
				byte |= 0x20;
			}

			switch (interleaving)
			{
			case kInterleaveNone:
				// Nothing
				break;

			case kInterleaveTwoWay:
				byte |= 0x64;
				break;

			case kInterleaveFourWay:
				byte |= 0x128;
				break;
			}

			return byte;
		}

		bool Targa::Load(const char* pFileName)
		{
			using namespace Io;

			ASSERT(pFileName != NULL);

			// Open the file
			File::Handle file = File::Open(pFileName, Io::File::kModeRead);
			if (file == NULL)
			{
				BREAK_MSG("Unable to load file %s!", pFileName);
				return false;
			}

			// Read the header info
			File::Read(file, &(mHeader.idLength), sizeof(u8));
			File::Read(file, &(mHeader.colourMapType), sizeof(u8));
			File::Read(file, &(mHeader.dataTypeCode), sizeof(u8));
			File::Read(file, &(mHeader.colourMapOrigin), sizeof(u16));
			File::Read(file, &(mHeader.colourMapLength), sizeof(u16));
			File::Read(file, &(mHeader.colourMapDepth), sizeof(u8));
			File::Read(file, &(mHeader.xOrigin), sizeof(u16));
			File::Read(file, &(mHeader.yOrigin), sizeof(u16));
			File::Read(file, &(mHeader.width), sizeof(u16));
			File::Read(file, &(mHeader.height), sizeof(u16));
			File::Read(file, &(mHeader.bitsPerPixel), sizeof(u8));
			File::Read(file, &(mHeader.imageDescriptor), sizeof(u8));

			switch (mHeader.dataTypeCode)
			{
			case 2:
			case 3:
				//valid
				break;

			default:
				// Currently only supports 32bit uncompressed textures
				BREAK_MSG("Unsupported TGA datatype %d in file %s!", mHeader.dataTypeCode, pFileName);
				return false;
			}

			// Read the descriptor
			if (mpDescriptor != NULL)
			{
				delete[] mpDescriptor;
			}
			if (mHeader.idLength > 0)
			{
				mpDescriptor = new char[mHeader.idLength];
				if (mpDescriptor == NULL)
				{
					BREAK_MSG("Unable to allocate %d bytes for tga %s!", mHeader.idLength, pFileName);
					return false;
				}

				File::Read(file, mpDescriptor, mHeader.idLength);
			}

			// Calculate the size
			size_t dataSize = GetDataSize();

			// Read the data
			if (mpData != NULL)
			{
				delete[] mpData;
			}
			mpData = new u8[dataSize];
			if (mpData == NULL)
			{
				BREAK_MSG("Unable to allocate %d bytes for tga %s!", dataSize, pFileName);
				return false;
			}
			File::Read(file, mpData, dataSize);

			File::Close(file);

			return true;
		}

		bool Targa::Save(const char* pFileName)
		{
			using namespace Io;

			ASSERT(pFileName != NULL);

			File::Handle file = Io::File::Open(pFileName, Io::File::kModeWrite);

			File::Write(&mHeader.idLength, sizeof(u8), file);
			File::Write(&mHeader.colourMapType, sizeof(u8), file);
			File::Write(&mHeader.dataTypeCode, sizeof(u8), file);
			File::Write(&mHeader.colourMapOrigin, sizeof(u16), file);
			File::Write(&mHeader.colourMapLength, sizeof(u16), file);
			File::Write(&mHeader.colourMapDepth, sizeof(u8), file);
			File::Write(&mHeader.xOrigin, sizeof(u16), file);
			File::Write(&mHeader.yOrigin, sizeof(u16), file);
			File::Write(&mHeader.width, sizeof(u16), file);
			File::Write(&mHeader.height, sizeof(u16), file);
			File::Write(&mHeader.bitsPerPixel, sizeof(u8), file);
			File::Write(&mHeader.imageDescriptor, sizeof(u8), file);

			if (mHeader.idLength > 0)
			{
				ASSERT(mpDescriptor != NULL);
				File::Write(mpDescriptor, mHeader.idLength, file);
			}

			File::Write(mpData, GetDataSize(), file);

			File::Close(file);

			return true;
		}

		bool Targa::Create(GLenum format, int width, int height, const u8* pRgbaData)
		{
			ASSERT(width > 0);
			ASSERT(height > 0);
			ASSERT(pRgbaData != NULL);

			memset(&mHeader, 0, sizeof(Header));

			switch (format)
			{
			case GL_RGBA:
			{
				mHeader.idLength = 0;
				mHeader.dataTypeCode = kTypeUncompressedRgb;
				mHeader.width = width;
				mHeader.height = height;
				mHeader.yOrigin = height;
				mHeader.bitsPerPixel = 32;
				mHeader.imageDescriptor = ImageDescriptorByte(0, true, kInterleaveNone);
			}
			break;

			default:
				BREAK_MSG("Saving textures of format %d currently unsupported.", format);
				return false;
			}

			if (mpDescriptor != NULL)
			{
				delete[] mpDescriptor;
			}

			if (mpData != NULL)
			{
				delete[] mpData;
			}
			size_t dataSize = GetDataSize();
			mpData = new u8[dataSize];

			for (int i = 0; i < (width * height); ++i)
			{
				// Convert pixels to BGRA format
				const u8* src = pRgbaData + (i * 4);
				u8* dest = mpData + (i * 4);

				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
				dest[3] = src[3];
			}

			return true;
		}

		int Targa::GetNumChannels()
		{
			const int kBitsPerByte = 8;
			return mHeader.bitsPerPixel / kBitsPerByte;
		}

		size_t Targa::GetDataSize()
		{
			return (GetNumChannels() * mHeader.width * mHeader.height);
		}

	}
}