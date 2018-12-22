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
				GLint position;
				GLint textureCoord;
				GLint color;
				GLint channel;
			};

			struct Uniforms
			{
				GLint transform;
				GLint texture;
			};

			Technique();

			bool Load(const char* pVertexFileName, const char* pFragmentFileName);

			inline const Attributes& GetAttributes() const
			{
				return attributes;
			}

			inline const Uniforms& GetUniforms() const
			{
				return uniforms;
			}

			inline const Program& GetProgram() const
			{
				return program;
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(Technique);

			Program program;
			Attributes attributes;
			Uniforms uniforms;
		};

	}
}

#endif //MOMO_GRAPHICS_TECHNIQUE_INCLUDED
