#include "Graphics\Mesh.h"

#include "Graphics\Camera.h"
#include "Graphics\Program.h"
#include "Graphics\Technique.h"
#include "Graphics\Utils.h"
#include "Matrix.h"


namespace Momo
{
namespace Graphics
{
Mesh::Renderer::Renderer() :
    mpTechnique(nullptr),
    mpMesh(nullptr),
    mpTexture(nullptr),
    mVertexBufferHandle(0),
    mIndexBufferHandle(0)
{}

void Mesh::Renderer::Load(const Technique& technique, const Mesh& mesh, const Texture* pTexture)
{
    mpMesh = &mesh;
    mpTechnique = &technique;
    mpTexture = pTexture;

    // Declare the vertex stream buffer
    glGenBuffers(1, &mVertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(mesh.mVertexCount * sizeof(Vertex)), mesh.mpVertexData, GL_STATIC_DRAW);

    // Declare the index stream buffer
    glGenBuffers(1, &mIndexBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(mesh.mIndexCount * sizeof(GLushort)), mesh.mpIndexData, GL_STATIC_DRAW);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Renderer::Draw(const Camera& camera, const Matrix& world)
{
    ASSERT(mpMesh != NULL);
    ASSERT(mpTechnique != NULL);
    ASSERT(mVertexBufferHandle != 0);
    ASSERT(mIndexBufferHandle != 0);

    GL_CHECK(glUseProgram(mpTechnique->GetProgram().Handle()));

    // Set the transform
    Matrix transform;
    //Matrix::Multiply(camera.GetViewProjection(), world, transform);
    transform = camera.GetViewProjection() * world;
    GL_CHECK(glUniformMatrix4fv(mpTechnique->GetUniforms().transform, 1, false,
        (GLfloat*)(&transform)));

    // Send vertex data
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle));

    GL_CHECK(glEnableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().color)));
    GL_CHECK(glEnableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().position)));
    GL_CHECK(glEnableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().textureCoord)));

    // Enable the vertex attributes
    GL_CHECK(glVertexAttribPointer(
        (GLuint)(mpTechnique->GetAttributes().color),
        Vertex::kBytesPerColor,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(Vertex),
        (void*)offsetof(struct Vertex, color)));

    GL_CHECK(glVertexAttribPointer(
        (GLuint)(mpTechnique->GetAttributes().position),
        Vertex::kFloatsPerPosition,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(struct Vertex, position)));

    GL_CHECK(glVertexAttribPointer(
        (GLuint)(mpTechnique->GetAttributes().textureCoord),
        Vertex::kFloatsPerUv,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(struct Vertex, uv)));

    // Set the active texture unit to texture unit 0.
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glUniform1i(mpTechnique->GetUniforms().texture, 0));

    // Bind the texture for this batch
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, mpTexture->Handle()));

    // Draw indexed primitives
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle));
    GL_CHECK(glDrawElements(
        GL_TRIANGLES,
        (GLsizei)(mpMesh->mIndexCount),
        GL_UNSIGNED_SHORT,
        NULL));

    GL_CHECK(glDisableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().color)));
    GL_CHECK(glDisableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().position)));
    GL_CHECK(glDisableVertexAttribArray((GLuint)(mpTechnique->GetAttributes().textureCoord)));
}


Mesh::Mesh() :
    mpVertexData(nullptr),
    mVertexCount(0),
    mpIndexData(nullptr),
    mIndexCount(0)
{}

Mesh::Mesh(const Vertex* pVertexData, size_t vertexCount, const GLushort* pIndexData, size_t indexCount) :
    mpVertexData(pVertexData),
    mVertexCount(vertexCount),
    mpIndexData(pIndexData),
    mIndexCount(indexCount)
{}

Mesh::~Mesh()
{
    DeleteBuffers();
}

void Mesh::Load(const Vertex* pVertexData, size_t vertexCount, const GLushort* pIndexData, size_t indexCount)
{
    DeleteBuffers();

    mpVertexData = pVertexData;
    mVertexCount = vertexCount;
    mpIndexData = pIndexData;
    mIndexCount = indexCount;
}

void Mesh::DeleteBuffers()
{
    if (mpVertexData != nullptr)
    {
        delete[] mpVertexData;
        mpVertexData = nullptr;
    }

    if (mpIndexData != nullptr)
    {
        delete[] mpIndexData;
        mpIndexData = nullptr;
    }
}
}
}
