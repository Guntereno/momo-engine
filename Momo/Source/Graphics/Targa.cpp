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
				case Interleaving::None:
				// Nothing
				break;

				case Interleaving::TwoWay:
					byte |= 0x64;
				break;

				case Interleaving::FourWay:
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
			File::Handle file = File::Open(pFileName, Io::File::Mode::Read);
			if (file == NULL)
			{
				BREAK_MSG("Unable to load file %s!", pFileName);
				return false;
			}

			// Read the header info
			File::Read(file, &(mHeader.mIdLength), sizeof(u8));
			File::Read(file, &(mHeader.mColourMapType), sizeof(u8));
			File::Read(file, &(mHeader.mDataTypeCode), sizeof(u8));
			File::Read(file, &(mHeader.mColourMapOrigin), sizeof(u16));
			File::Read(file, &(mHeader.mColourMapLength), sizeof(u16));
			File::Read(file, &(mHeader.mColourMapDepth), sizeof(u8));
			File::Read(file, &(mHeader.mXOrigin), sizeof(u16));
			File::Read(file, &(mHeader.mYOrigin), sizeof(u16));
			File::Read(file, &(mHeader.mWidth), sizeof(u16));
			File::Read(file, &(mHeader.mHeight), sizeof(u16));
			File::Read(file, &(mHeader.mBitsPerPixel), sizeof(u8));
			File::Read(file, &(mHeader.mImageDescriptor), sizeof(u8));

			switch (mHeader.mDataTypeCode)
			{
			case 2:
			case 3:
				//valid
				break;

			default:
				// Currently only supports 32bit uncompressed textures
				BREAK_MSG("Unsupported TGA datatype %d in file %s!", mHeader.mDataTypeCode, pFileName);
				return false;
			}

			// Read the descriptor
			if (mpDescriptor != NULL)
			{
				delete[] mpDescriptor;
			}
			if (mHeader.mIdLength > 0)
			{
				mpDescriptor = new char[mHeader.mIdLength];
				if (mpDescriptor == NULL)
				{
					BREAK_MSG("Unable to allocate %d bytes for tga %s!", mHeader.mIdLength, pFileName);
					return false;
				}
				File::Read(file, mpDescriptor, mHeader.mIdLength);
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

			File::Handle file = Io::File::Open(pFileName, Io::File::Mode::Write);

			File::Write(&mHeader.mIdLength, sizeof(u8), file);
			File::Write(&mHeader.mColourMapType, sizeof(u8), file);
			File::Write(&mHeader.mDataTypeCode, sizeof(u8), file);
			File::Write(&mHeader.mColourMapOrigin, sizeof(u16), file);
			File::Write(&mHeader.mColourMapLength, sizeof(u16), file);
			File::Write(&mHeader.mColourMapDepth, sizeof(u8), file);
			File::Write(&mHeader.mXOrigin, sizeof(u16), file);
			File::Write(&mHeader.mYOrigin, sizeof(u16), file);
			File::Write(&mHeader.mWidth, sizeof(u16), file);
			File::Write(&mHeader.mHeight, sizeof(u16), file);
			File::Write(&mHeader.mBitsPerPixel, sizeof(u8), file);
			File::Write(&mHeader.mImageDescriptor, sizeof(u8), file);

			if (mHeader.mIdLength > 0)
			{
				ASSERT(mpDescriptor != NULL);
				File::Write(mpDescriptor, mHeader.mIdLength, file);
			}

			File::Write(mpData, GetDataSize(), file);

			File::Close(file);

			return true;
		}

		bool Targa::Create(GLenum format, u16 width, u16 height, const u8* pRgbaData)
		{
			ASSERT(width > 0);
			ASSERT(height > 0);
			ASSERT(pRgbaData != NULL);

			memset(&mHeader, 0, sizeof(Header));

			switch (format)
			{
			case GL_RGBA:
			{
				mHeader.mIdLength = 0;
				mHeader.mDataTypeCode = DataType::UncompressedRgb;
				mHeader.mWidth = width;
				mHeader.mHeight = height;
				mHeader.mYOrigin = height;
				mHeader.mBitsPerPixel = 32;
				mHeader.mImageDescriptor = ImageDescriptorByte(0, true, Interleaving::None);
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
			return mHeader.mBitsPerPixel / kBitsPerByte;
		}

		size_t Targa::GetDataSize()
		{
			return static_cast<size_t>(GetNumChannels() * mHeader.mWidth * mHeader.mHeight);
		}

	}
}