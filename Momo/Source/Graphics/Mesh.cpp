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
			mpMesh(nullptr),
			mpTechnique(nullptr),
			mVertexBufferHandle(-1),
			mIndexBufferHandle(-1)
		{}

		void Mesh::Renderer::Load(const Technique& technique, const Mesh& mesh, const Texture* pTexture)
		{
			mpMesh = &mesh;
			mpTechnique = &technique;
			mpTexture = pTexture;

			// Declare the vertex stream buffer
			glGenBuffers(1, &mVertexBufferHandle);
			glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
			glBufferData(GL_ARRAY_BUFFER, mesh.mVertexCount * sizeof(Vertex), mesh.mpVertexData, GL_STATIC_DRAW);

			// Declare the index stream buffer
			glGenBuffers(1, &mIndexBufferHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.mIndexCount * sizeof(GLushort), mesh.mpIndexData, GL_STATIC_DRAW);

			// Unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void Mesh::Renderer::Draw(const Camera& camera, const Matrix& world)
		{
			ASSERT(mpMesh != NULL);
			ASSERT(mpTechnique != NULL);
			ASSERT(mVertexBufferHandle != -1);
			ASSERT(mIndexBufferHandle != -1);

			GL_CHECK(glUseProgram(mpTechnique->program.Handle()))

				// Set the transform
				Matrix transform;
			//Matrix::Multiply(camera.GetViewProjection(), world, transform);
			transform = camera.GetViewProjection() * world;
			GL_CHECK(glUniformMatrix4fv(mpTechnique->uniforms.transform, 1, false, (GLfloat*)(&transform)))

				// Send vertex data
				GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle))
				GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle))

				GL_CHECK(glEnableVertexAttribArray(mpTechnique->attributes.color))
				GL_CHECK(glEnableVertexAttribArray(mpTechnique->attributes.position))
				GL_CHECK(glEnableVertexAttribArray(mpTechnique->attributes.textureCoord))

				// Enable the vertex attributes
				GL_CHECK(glVertexAttribPointer(
					mpTechnique->attributes.color,
					Vertex::kBytesPerColor,
					GL_UNSIGNED_BYTE,
					GL_TRUE,
					sizeof(Vertex),
					(void*)offsetof(struct Vertex, color)))

				GL_CHECK(glVertexAttribPointer(
					mpTechnique->attributes.position,
					Vertex::kFloatsPerPosition,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					(void*)offsetof(struct Vertex, position)))

				GL_CHECK(glVertexAttribPointer(
					mpTechnique->attributes.textureCoord,
					Vertex::kFloatsPerUv,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					(void*)offsetof(struct Vertex, uv)))

				// Set the active texture unit to texture unit 0.
				glActiveTexture(GL_TEXTURE0);
			glUniform1i(mpTechnique->uniforms.texture, 0);

			// Bind the texture for this batch
			glBindTexture(GL_TEXTURE_2D, mpTexture->Handle());

			// Draw indexed primitives
			GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle))
				GL_CHECK(glDrawElements(
					GL_TRIANGLES,
					mpMesh->mIndexCount,
					GL_UNSIGNED_SHORT,
					NULL))

				glDisableVertexAttribArray(mpTechnique->attributes.color);
			glDisableVertexAttribArray(mpTechnique->attributes.position);
			glDisableVertexAttribArray(mpTechnique->attributes.textureCoord);
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