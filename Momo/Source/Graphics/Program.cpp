#include "Graphics/Program.h"

#include "Logger.h"
#include "Graphics\Utils.h"
#include "Io\FileInMemory.h"

#include <cstdlib>

namespace Momo
{
namespace Graphics
{
GLuint Program::LoadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader)
	{
		glShaderSource(shader, 1, &pSource, NULL);
		Utils::CheckGlError("glShaderSource");

		glCompileShader(shader);
		Utils::CheckGlError("glCompileShader");

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = (char*) malloc(infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

bool Program::Load(const char* pVertexSource, const char* pFragmentSource)
{
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader)
	{
		return false;
	}

	GLuint pixelShader = LoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader)
	{
		return false;
	}

	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vertexShader);
		Graphics::Utils::CheckGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		Graphics::Utils::CheckGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength)
			{
				char* buf = (char*) malloc(bufLength);
				if (buf)
				{
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}

	mHandle = program;
	return true;
}

bool Program::LoadFiles(const char* pVertexFileName, const char* pFragmentFileName)
{
	Io::FileInMemory vertexFile(pVertexFileName);
	if(vertexFile.GetData() == NULL)
	{
		return false;
	}

	Io::FileInMemory fragmentFile(pFragmentFileName);
	if(fragmentFile.GetData() == NULL)
	{
		return false;
	}

	return Load(reinterpret_cast<const char*>(vertexFile.GetData()), reinterpret_cast<const char*>(fragmentFile.GetData()));
}
}
}