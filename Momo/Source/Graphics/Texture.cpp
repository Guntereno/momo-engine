#include "Graphics/Texture.h"

#include "Graphics/Targa.h"

#include <cstddef>

namespace Momo
{
namespace Graphics
{
void Texture::Load(GLsizei width, GLsizei height, GLenum format, const void* data)
{
	GLuint textureHandle = 0;

	mWidth = width;
	mHeight = height;
	mFormat = format;

	glGenTextures(1, &textureHandle);
	Utils::CheckGlError("glGenTextures");

	if (textureHandle != 0)
	{
		// Use tightly packed data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		// Bind to the texture in OpenGL
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		// Set filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Load the bitmap into the bound texture.
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, mFormat, GL_UNSIGNED_BYTE, data);
	}

	mHandle = textureHandle;
}

bool Texture::LoadTga(const char* pFileName)
{
	Targa targa;
	bool result = targa.Load(pFileName);
	if(!result)
	{
		return false;
	}

	const Targa::Header& header = targa.GetHeader();

	switch(header.dataTypeCode)
	{
	case Targa::kTypeUncompressedRgb:
	case Targa::kTypeUncompressedGray:
		// valid
		break;

	default:
		BREAK_MSG("Unsupported TGA format!");
		return false;
	}

	int numChannels = targa.GetNumChannels();
	GLenum format;
	switch(numChannels)
	{
	case 1:
		format = GL_LUMINANCE;
		break;

	case 4:
		format = GL_RGBA;
		break;

	default:
		BREAK_MSG("Unsupported pixel format!");
		return false;
	}

	u8* pTextureData = NULL;
	pTextureData = new u8[targa.GetDataSize()];

	int numPixels = header.width * header.height;

	for(int i=0; i<numPixels; ++i)
	{
		// Convert pixels to RGBA format
		const u8* src = targa.GetData() + (i*numChannels);
		u8* dest = pTextureData + (i*numChannels);

		switch(numChannels)
		{
		case 4:
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];
			dest[3] = src[3];
			break;

		case 1:
			dest[0] = src[0];
			break;
		}
	}

	Load(header.width, header.height, format, pTextureData);

	return true;
}
}
}