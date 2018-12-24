#ifndef MOMO_GRAPHICS_UTILS_INCLUDED
#define MOMO_GRAPHICS_UTILS_INCLUDED

#include "Momo.h"

#include "Logger.h"
#include "AssertBreak.h"

#include <GLES2/gl2.h>
#include <cstdlib>

#if defined MOMO_DEBUG
# define GL_CHECK(_statement)                                        \
do{                                                                 \
    _statement;                                                     \
    Graphics::Utils::CheckGlError(__FILE__, __LINE__, #_statement); \
} while(false)
#else
# define GL_CHECK(_statement) _statement;(void)0
#endif

namespace Momo
{
namespace Graphics
{

class Utils
{
public:
    static void CheckGlError(const char* pFile, int line, const char* statement)
    {
        for (GLenum error = glGetError(); (error != 0); error = glGetError())
        {
            // Unused in Release
            UNUSED(pFile);
            UNUSED(line);
            UNUSED(statement);

            BREAK_MSG(pFile, line, "after '%s' glError (0x%x).\n", statement, error);
        }
    }
};

}
}

#endif //MOMO_GRAPHICS_UTILS_INCLUDED
