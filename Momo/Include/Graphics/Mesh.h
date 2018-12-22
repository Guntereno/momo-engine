#ifndef MOMO_GRAPHICS_MESH_INCLUDED
#define MOMO_GRAPHICS_MESH_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Vector.h"

#include <GLES2/gl2.h>


namespace Momo
{
	namespace Graphics
	{

		class Mesh
		{
		public:
			class Renderer
			{
			public:
				Renderer();

				void Load(const Technique& technique, const Mesh& mesh, const Texture* pTexture);
				void Draw(const Camera& camera, const Matrix& world);

			private:
				const Technique* mpTechnique;
				const Mesh* mpMesh;
				const Texture* mpTexture;

				GLuint mVertexBufferHandle;
				GLuint mIndexBufferHandle;
			};

			struct Vertex
			{
				static constexpr int kFloatsPerPosition = 4;
				static constexpr int kFloatsPerUv = 2;
				static constexpr int kBytesPerColor = 4;

				Color color;
				Vector4 position;
				Vector2 uv;
			};

			Mesh();
			Mesh(const Vertex* pVertexData, size_t vertexCount, const GLushort* pIndexData, size_t indexCount);

			virtual ~Mesh();

			void Load(const Vertex* pVertexData, size_t vertexCount, const GLushort* pIndexData, size_t indexCount);

			void DeleteBuffers();

		private:
			DISALLOW_COPY_AND_ASSIGN(Mesh);

			const Vertex* mpVertexData;
			size_t mVertexCount;
			
			const GLushort* mpIndexData;
			size_t mIndexCount;
		};

	}
}

#endif //MOMO_GRAPHICS_MESH_INCLUDED
