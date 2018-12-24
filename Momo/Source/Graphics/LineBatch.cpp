#include "Graphics/LineBatch.h"

#include "Logger.h"
#include "AssertBreak.h"
#include "Graphics/Camera.h"
#include "Graphics/Program.h"
#include "Graphics/Technique.h"
#include "Graphics/Utils.h"
#include "Point.h"

#include <cstddef>
#include <limits>


//#define OUTPUT_VERTEX_INFO


namespace Momo
{
namespace Graphics
{

static constexpr char* kShaderNames[2] =
{ "shaders/vpLineBatch.vp", "shaders/fpLineBatch.fp" };


static bool gStaticsInitialised = false;
static Technique gTechnique;

LineBatch::LineBatch() :
    mVertexBufferHandle(0),
    mIndexBufferHandle(0),
    mInBeginEndBlock(false),
    mLineCount(0)
{
}

void LineBatch::Load()
{
    LOGI("LineBatch Constructor");

    if (!gStaticsInitialised)
    {
        bool result = LoadTechnique();
        UNUSED(result); // Unused in Release
        ASSERT(result);

        gStaticsInitialised = true;
    }

    // Declare the vertex stream buffer
    glGenBuffers(1, &mVertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, kVertexMax * sizeof(Vertex), mVertexData, GL_STREAM_DRAW);

    // Generate the index buffer
    GLushort indices[kIndexMax];
    for (size_t line = 0; line < kLineMax; ++line)
    {
        size_t startIndex = (line * kIndicesPerLine);
        ASSERT((startIndex + 1) < kIndexMax);

        size_t startVert = (line * kVertsPerLine);
        ASSERT((startVert + 1) < std::numeric_limits<GLushort>::max());

        indices[startIndex + 0] = (GLushort)(startVert + 0);
        indices[startIndex + 1] = (GLushort)(startVert + 1);
    }

    glGenBuffers(1, &mIndexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, kIndexMax * sizeof(GLushort), indices, GL_STATIC_DRAW);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void LineBatch::End(const Camera& camera)
{
    //LOGI("LineBatch::End()");
    ASSERT(mInBeginEndBlock);

    GL_CHECK(glUseProgram(gTechnique.GetProgram().Handle()));

    // Set the transform
    glUniformMatrix4fv(gTechnique.GetUniforms().transform, 1, false,
        (GLfloat*)(&camera.GetViewProjection()));

    // Send vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
    GL_CHECK(glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        (GLsizeiptr)(mLineCount * kVertsPerLine * sizeof(Vertex)),
        mVertexData));

    // Enable the vertex attributes
    GL_CHECK(glVertexAttribPointer(
        (GLuint)(gTechnique.GetAttributes().color), Vertex::kBytesPerColor,
        GL_UNSIGNED_BYTE, GL_TRUE,
        sizeof(Vertex),
        (void*)offsetof(struct Vertex, color)));

    GL_CHECK(glVertexAttribPointer(
        (GLuint)(gTechnique.GetAttributes().position), Vertex::kFloatsPerPosition,
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(struct Vertex, position)));

    GL_CHECK(glEnableVertexAttribArray((GLuint)(gTechnique.GetAttributes().color)));
    GL_CHECK(glEnableVertexAttribArray((GLuint)(gTechnique.GetAttributes().position)));;

    // Draw indexed primitives
    size_t indexCount = mLineCount * kIndicesPerLine;
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle));
    GL_CHECK(glDrawElements(
        GL_LINES,
        (GLsizei)indexCount,
        GL_UNSIGNED_SHORT,
        (void*)(0)
    ));

    mInBeginEndBlock = false;
}

bool LineBatch::LoadTechnique()
{
    LOGI("LoadTechniques()\n");

    // Create the program
    bool result = gTechnique.Load
    (
        kShaderNames[0],
        kShaderNames[1]
    );

    return result;
}

void LineBatch::DrawInternal(const Point& from, const Point& to, const Color& colorFrom, const Color& colorTo)
{
    ASSERT(mInBeginEndBlock);

    if (mLineCount >= kLineMax)
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

    const size_t kFirstVert = mLineCount * kVertsPerLine;
    for (int i = 0; i < kVertsPerLine; ++i)
    {
        size_t vertIdx = kFirstVert + i;

        mVertexData[vertIdx].position = positions[i];
        mVertexData[vertIdx].color = colors[i];

#ifdef OUTPUT_VERTEX_INFO

        LOGI("mVertexData[%d].color = 0x%x\n",
            vertIdx,
            mVertexData[vertIdx].color
        );
        LOGI("mVertexData[%d].position = { %.2f, %.2f }\n",
            vertIdx,
            mVertexData[vertIdx].position.mX,
            mVertexData[vertIdx].position.mY
        );
#endif
    }

    ++mLineCount;
}
}
}
