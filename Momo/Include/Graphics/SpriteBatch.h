#ifndef MOMO_GRAPHICS_SPRITEBATCH_INCLUDED
#define MOMO_GRAPHICS_SPRITEBATCH_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Matrix.h"
#include "Rectangle.h"
#include "Vector.h"

#include "Graphics/Texture.h"

#include <limits>
#include <type_traits>

namespace Momo
{
	namespace Graphics
	{

		class SpriteBatch
		{
		public:
			enum class DrawFlags : u32
			{
				None = 0,
				FlipX = 1 << 0,
				FlipY = 1 << 1
			};
			
			enum class TechniqueId : u32
			{
				Sprite,
				FontNoOutline,
				FontOutline,

				Count,

				Invalid
			};

			SpriteBatch();

			void Load();

			void Begin();

			void Draw(const Texture* texture, const Rectangle& dest, const Color& color);
			void Draw(const Texture* texture, const Rectangle& dest, const Color& color, DrawFlags flags);
			void Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color);
			void Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color, DrawFlags flags);

			void DrawString(const Text::Font& font, const char* pUtf8String, size_t strLen, const Point& point, const Color& color);

			void End(const Camera& camera);

		private:
			DISALLOW_COPY_AND_ASSIGN(SpriteBatch);

			static constexpr size_t kVertsPerSprite = 4;
			static constexpr size_t kIndicesPerSprite = 6;

			static constexpr size_t kSpriteMax = std::numeric_limits<u16>::max();
			static constexpr size_t kVertexMax = kSpriteMax * kVertsPerSprite;
			static constexpr size_t kIndexMax = kSpriteMax * kIndicesPerSprite;

			static constexpr size_t kTrianglesPerSprite = 2;

			struct Vertex
			{
				static constexpr int kFloatsPerPosition = 2;
				static constexpr int kFloatsPerUv = 2;
				static constexpr int kFloatsPerChannel = 4;
				static constexpr int kBytesPerColor = 4;

				Color color;
				Vector2 position;
				Vector2 uv;
				Vector4 channel;
			};

			struct BatchInfo
			{
				const Texture* pTexture;
				size_t count;
				TechniqueId technique;
			};

			bool LoadTechniques();

			void DrawInternal(TechniqueId techniqueId, const Vector4& channel, const Texture* pTexture, const Rectangle& dest, const Rectangle* src, const Color& color, DrawFlags flags);

			GLuint mVertexBufferHandle;
			GLuint mIndexBufferHandle;

			Vertex mVertexData[kVertexMax];
			BatchInfo mBatchData[kSpriteMax];

			bool mInBeginEndBlock;
			size_t mSpriteCount;
			size_t mBatchCount;
		};

		FLAG_OPS(SpriteBatch::DrawFlags)
	}
}

#endif //MOMO_GRAPHICS_SPRITEBATCH_INCLUDED
