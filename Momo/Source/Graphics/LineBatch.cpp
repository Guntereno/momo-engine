#include "Graphics/LineBatch.h"

#include "Logger.h"
#include "AssertBreak.h"
#include "Graphics/Program.h"
#include "Graphics/Utils.h"
#include "Point.h"

#include <cstddef>

//#define OUTPUT_VERTEX_INFO

namespace Momo
{
namespace Graphics
{

static const char* kShaderNames[2] =
{ "shaders/vpLineBatch.vp", "shaders/fpLineBatch.fp" };

struct Technique
{
	struct Attributes
	{
		GLuint position;
		GLuint color;
	};

	struct Uniforms
	{
		GLuint transform;
	};

	Program program;
	Attributes attributes;
	Uniforms uniforms;
};

static Technique gTechnique;

LineBatch::LineBatch():
	mVertexBufferHandle(0),
	mIndexBufferHandle(0),
	mInBeginEndBlock(false),
	mLineCount(0)
{
}

void LineBatch::Load()
{
	LOGI("LineBatch Constructor");

	bool result = LoadTechnique();
	ASSERT(result);

	// Declare the vertex stream buffer
	glGenBuffers(1, &mVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, kVertexMax * sizeof(Vertex), mVertexData, GL_STREAM_DRAW);

	// Generate the index buffer
	GLushort indices[kIndexMax];
	for(int line=0; line<kLineMax; ++line)
	{
		GLushort startIndex = (line * kIndicesPerLine);
		GLushort startVert = (line * kVertsPerLine);

		indices[startIndex + 0] = startVert + 0;
		indices[startIndex + 1] = startVert + 1;
	}

	glGenBuffers(1, &mIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, kIndexMax * sizeof(GLushort), indices, GL_STATIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mTransform.SetIdentity();
}

void LineBatch::Begin()
{
	//LOGI("LineBatch::Begin()");
	ASSERT(!mInBeginEndBlock);

	mInBeginEndBlock = true;
	mLineCount = 0;
}

void LineBatch::Draw(const Point& from, const Point& to, const Color& color)
{
	DrawInternal(from, to, color, color);
}

void LineBatch::Draw(const Point& from, const Point& to, const Color& colorFrom, const Color& colorTo)
{
	DrawInternal(from, to, colorFrom, colorTo);
}

void LineBatch::DrawRectangle(const Rectangle& rect, const Color& color)
{
	Point points[] =
	{
		{ rect.Left(), rect.Bottom() },
		{ rect.Left(), rect.Top() },
		{ rect.Right(), rect.Top() },
		{ rect.Right(), rect.Bottom() }
	};

	DrawInternal(points[0], points[1], color, color);
	DrawInternal(points[1], points[2], color, color);
	DrawInternal(points[2], points[3], color, color);
	DrawInternal(points[3], points[0], color, color);
}

void LineBatch::End()
{
	//LOGI("LineBatch::End()");
	ASSERT(mInBeginEndBlock);

	glUseProgram(gTechnique.program.Handle());
	Graphics::Utils::CheckGlError("glUseProgram");

	// Set the transform
	glUniformMatrix4fv(gTechnique.uniforms.transform, 1, false, (GLfloat*)(&mTransform));

	// Send vertex data
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferSubData (
		GL_ARRAY_BUFFER,
		0,
		mLineCount * kVertsPerLine * sizeof(Vertex),
		mVertexData);
	Graphics::Utils::CheckGlError("glBufferSubData");

	// Enable the vertex attributes
	glVertexAttribPointer(
		gTechnique.attributes.color, Vertex::kBytesPerColor,
		GL_UNSIGNED_BYTE, GL_TRUE,
		sizeof(Vertex),
		(void*)offsetof(struct Vertex, color));

	glVertexAttribPointer(
		gTechnique.attributes.position, Vertex::kFloatsPerPosition,
		GL_FLOAT, GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(struct Vertex, position));
	
	Graphics::Utils::CheckGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(gTechnique.attributes.color);
	glEnableVertexAttribArray(gTechnique.attributes.position);

	Graphics::Utils::CheckGlError("glEnableVertexAttribArray");

	// Draw indexed primitives
	int indexCount = mLineCount * kIndicesPerLine;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	Graphics::Utils::CheckGlError("glBindBuffer");
	glDrawElements(
		GL_LINES,
		indexCount,
		GL_UNSIGNED_SHORT,
		(void*)(0)
		);
	Graphics::Utils::CheckGlError("glDrawElements");

	mInBeginEndBlock = false;
}

bool LineBatch::LoadTechnique()
{
	LOGI("LoadTechniques()\n");

	// Create the program
	bool result = gTechnique.program.LoadFiles
	(
		kShaderNames[0],
		kShaderNames[1]
	);

	if (!result) {
		LOGE("Could not create sprite batch program.\n");
		return false;
	}

	GLuint programHandle = gTechnique.program.Handle();
	ASSERT(programHandle != 0);

	// Get the attribute names
	Technique::Attributes& attributes = gTechnique.attributes;

	attributes.color = glGetAttribLocation(programHandle, "aColor");
	Graphics::Utils::CheckGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"aColor\") = %d\n",
			attributes.color);

	attributes.position = glGetAttribLocation(programHandle, "aPosition");
	Graphics::Utils::CheckGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"aPosition\") = %d\n",
			attributes.position);

	// Get the uniform handles
	Technique::Uniforms& uniforms = gTechnique.uniforms;

	uniforms.transform = glGetUniformLocation(programHandle, "uTransform");
	Graphics::Utils::CheckGlError("glGetUniformLocation");
	LOGI("glGetUniformLocation(\"uTransform\") = %d\n",
			uniforms.transform);

	return true;
}

void LineBatch::DrawInternal(const Point& from, const Point& to, const Color& colorFrom, const Color& colorTo)
{
	ASSERT(mInBeginEndBlock);

	if(mLineCount >= kLineMax)
	{
		BREAK();
		return;
	}

	// Setup the vertices
	Vector2 positions[kVertsPerLine];
	{
		positions[0].Set(from);
		positions[1].Set(to);
	}

	Color colors[kVertsPerLine];
	{
		colors[0] = colorFrom;
		colors[1] = colorTo;
	}

	const int kFirstVert = mLineCount * kVertsPerLine;
	for(int i=0; i<kVertsPerLine; ++i)
	{
		int vertIdx = kFirstVert + i;

		mVertexData[vertIdx].position = positions[i];
		mVertexData[vertIdx].color = colors[i];

#ifdef OUTPUT_VERTEX_INFO

		LOGI("mVertexData[%d].color = 0x%x\n",
			vertIdx,
			mVertexData[vertIdx].color
			);
		LOGI("mVertexData[%d].position = { %.2f, %.2f }\n",
			vertIdx,
			mVertexData[vertIdx].position.x,
			mVertexData[vertIdx].position.y
			);
#endif
	}

	++mLineCount;
}
}
}