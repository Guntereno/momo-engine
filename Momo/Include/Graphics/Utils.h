#ifndef MOMO_GRAPHICS_UTILS_INCLUDED
#define MOMO_GRAPHICS_UTILS_INCLUDED

#include "Momo.h"

#include "Logger.h"
#include "AssertBreak.h"

#include <GLES2/gl2.h>
#include <cstdlib>

namespace Momo
{
namespace Graphics
{
class Utils
{
public:
	static void CheckGlError(const char* op) {
		for (GLint error = glGetError(); error; error
				= glGetError()) {
			BREAK_MSG("after %s() glError (0x%x)\n", op, error);
		}
	}
};
}
}

#endif //MOMO_GRAPHICS_UTILS_INCLUDED
