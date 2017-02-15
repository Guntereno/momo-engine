#ifndef MOMO_GRAPHICS_TEXTURE_INCLUDED
#define MOMO_GRAPHICS_TEXTURE_INCLUDED

#include "Momo.h"

#include <GLES2/gl2.h>

namespace Momo
{
namespace Graphics
{
class Texture
{
public:
	Texture():
		mHandle(0),
		mWidth(0),
		mHeight(0)
		{}

	inline int Width() const { return mWidth; }
	inline int Height() const { return mHeight; }
	inline GLuint Handle() const { return mHandle; }
	inline GLenum Format() const { return mFormat; }

	void Load(GLsizei width, GLsizei height, GLenum format, const void* data);
	bool LoadTga(const char* pFileName);

private:
	DISALLOW_COPY_AND_ASSIGN(Texture);

	GLuint mHandle;
	int mWidth;
	int mHeight;
	GLenum mFormat;
};
}
}

#endif //MOMO_GRAPHICS_TEXTURE_INCLUDED
