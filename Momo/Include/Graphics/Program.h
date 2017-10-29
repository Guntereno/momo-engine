#ifndef MOMO_GRAPHICS_PROGRAM_INCLUDED
#define MOMO_GRAPHICS_PROGRAM_INCLUDED

#include "Momo.h"

#include <GLES2/gl2.h>


namespace Momo
{
	namespace Graphics
	{

		class Program
		{
		public:
			Program() :
				mHandle(0)
			{}

			inline GLuint Handle() const { return mHandle; }

			bool Load(const char* pVertexSource, const char* pFragmentSource);
			bool LoadFiles(const char* pVertexFileName, const char* pFragmentFileName);

		private:
			DISALLOW_COPY_AND_ASSIGN(Program);

			GLuint LoadShader(GLenum shaderType, const char* pSource);

			GLuint mHandle;
		};

	}
}

#endif //MOMO_GRAPHICS_PROGRAM_INCLUDED
