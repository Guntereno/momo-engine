#ifndef MOMO_GRAPHICS_TARGA_INCLUDED
#define MOMO_GRAPHICS_TARGA_INCLUDED

#include "Momo.h"

#include "Utils.h"


namespace Momo
{
	namespace Graphics
	{

		class Targa
		{
		public:
			struct Header
			{
				u8  mIdLength;
				u8  mColourMapType;
				u8  mDataTypeCode;
				u16 mColourMapOrigin;
				u16 mColourMapLength;
				u8  mColourMapDepth;
				u16 mXOrigin;
				u16 mYOrigin;
				u16 mWidth;
				u16 mHeight;
				u8  mBitsPerPixel;
				u8  mImageDescriptor;
			};

			enum DataType : u8
			{
				NoData = 0,										// No image data included.
				UncompressedColorMapped = 1,						// Uncompressed, color-mapped images.
				UncompressedRgb = 2,								// Uncompressed, RGB images.
				UncompressedGray = 3,								// Uncompressed, black and white images.
				RleColorMapped = 9,								// Runlength encoded color-mapped images.
				RleRgb = 10,										// Runlength encoded RGB images.
				CompressedGray = 11,								// Compressed, black and white images.
			};

			enum class Interleaving : u8
			{
				None,
				TwoWay,
				FourWay
			};

			Targa();
			virtual ~Targa();

			bool Load(const char* pFileName);
			bool Save(const char* pFileName);

			bool Create(GLenum format, u16 mWidth, u16 mHeight, const u8* pRgbaData);

			const Header& GetHeader() { return mHeader; }

			const char* GetDescriptor() { return mpDescriptor; }

			int GetNumChannels();
			size_t GetDataSize();
			const u8* GetData() { return mpData; }

		private:
			DISALLOW_COPY_AND_ASSIGN(Targa);

			u8 ImageDescriptorByte(u8 bitsPerPixel, bool originInUpperLeft, Interleaving interleaving);

			Header mHeader;
			char* mpDescriptor;
			u8* mpColorMap;
			u8* mpData;
		};

	}
}

#endif //MOMO_GRAPHICS_TARGA_INCLUDED
