#ifndef MOMO_GRAPHICS_TECHNIQUE_INCLUDED
#define MOMO_GRAPHICS_TECHNIQUE_INCLUDED

#include "Momo.h"

#include "Graphics\Program.h"

#include <GLES2/gl2.h>


namespace Momo
{
	namespace Graphics
	{

		class Technique
		{
		public:
			struct Attributes
			{
				GLuint position;
				GLuint textureCoord;
				GLuint color;
				GLuint channel;
			};

			struct Uniforms
			{
				GLuint transform;
				GLuint texture;
			};

			Technique();

			bool Load(const char* pVertexFileName, const char* pFragmentFileName);

			Program program;
			Attributes attributes;
			Uniforms uniforms;

		private:
			DISALLOW_COPY_AND_ASSIGN(Technique);
		};

	}
}

#endif //MOMO_GRAPHICS_TECHNIQUE_INCLUDED
