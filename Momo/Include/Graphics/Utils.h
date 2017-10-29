#ifndef MOMO_GRAPHICS_UTILS_INCLUDED
#define MOMO_GRAPHICS_UTILS_INCLUDED

#include "Momo.h"

#include "Logger.h"
#include "AssertBreak.h"

#include <GLES2/gl2.h>
#include <cstdlib>


#define GL_CHECK(_statement) _statement;Graphics::Utils::CheckGlError(__FILE__, __LINE__, #_statement);


namespace Momo
{
	namespace Graphics
	{

		class Utils
		{
		public:
			static void CheckGlError(const char* pFile, int pLine, const char* statement) 
			{
				for (GLint error = glGetError(); (error != 0); error = glGetError())
				{
					BreakPrintMessage(pFile, pLine, "after '%s' glError (0x%x) in file '%s' on line '%d'.\n", statement, error, pFile, pLine);
					DEBUG_BREAK();
				}
			}
		};

	}
}

#endif //MOMO_GRAPHICS_UTILS_INCLUDED
