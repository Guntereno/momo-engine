#include "Graphics\Technique.h"

#include "Logger.h"
#include "Graphics/Utils.h"


namespace Momo
{
	namespace Graphics
	{

		static GLint GetAttribute(GLuint programHandle, const char* pName);
		static GLint GetUniform(GLuint programHandle, const char* pName);

		Technique::Technique()
		{

		}

		bool Technique::Load(const char* pVertexFileName, const char* pFragmentFileName)
		{
			// Create the program
			bool result = program.LoadFiles(pVertexFileName, pFragmentFileName);

			if (!result) {
				LOGE("Could not create sprite batch program.\n");
				return false;
			}

			GLuint programHandle = program.Handle();
			ASSERT(programHandle != 0);

			// Get the attribute names
			attributes.color = GetAttribute(programHandle, "aColor");
			attributes.channel = GetAttribute(programHandle, "aChannel");
			attributes.position = GetAttribute(programHandle, "aPosition");
			attributes.textureCoord = GetAttribute(programHandle, "aTexCoord");

			// Get the uniform handles
			uniforms.texture = GetUniform(programHandle, "uTransform");
			uniforms.texture = GetUniform(programHandle, "uTexture");

			return true;
		}

		GLint GetAttribute(GLuint programHandle, const char* pName)
		{
			GL_CHECK(GLint result = glGetAttribLocation(programHandle, pName))
				LOGI("glGetAttribLocation(\"%s\") = %d\n", pName, result);

			return result;
		}

		GLint GetUniform(GLuint programHandle, const char* pName)
		{
			GL_CHECK(GLint result = glGetUniformLocation(programHandle, pName))
				LOGI("glGetUniformLocation(\"%s\") = %d\n", pName, result);

			return result;
		}

	}
}