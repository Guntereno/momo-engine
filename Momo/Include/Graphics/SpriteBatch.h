#ifndef MOMO_GRAPHICS_SPRITEBATCH_INCLUDED
#define MOMO_GRAPHICS_SPRITEBATCH_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Matrix.h"
#include "Rectangle.h"
#include "Vector.h"

#include "Graphics/Texture.h"

namespace Momo
{
namespace Graphics
{
class SpriteBatch
{
public:
	enum DrawFlags
	{
		kFlagFlipX = 1,
		kFlagFlipY = 2
	};

	enum TechniqueId
	{
		kTechniqueInvalid = -1,

		kTechniqueSprite,
		kTechniqueFontNoOutline,
		kTechniqueFontOutline,

		kTechniqueCount
	};

	SpriteBatch();

	inline void SetTransform(const Matrix& transform)
	{
		mTransform = transform;
	}

	void Load();

	void Begin();

	void Draw(const Texture* texture, const Rectangle& dest, const Color& color);
	void Draw(const Texture* texture, const Rectangle& dest, const Color& color, unsigned int flags);
	void Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color);
	void Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color, unsigned int flags);

	void DrawString(const Text::Font& font, const char* pUtf8String, size_t strLen, const Point& point, const Color& color);

	void End();

private:
	DISALLOW_COPY_AND_ASSIGN(SpriteBatch);

	static const size_t kVertsPerSprite = 4;
	static const size_t kIndicesPerSprite = 6;

	static const size_t kSpriteMax = 8192;
	static const size_t kVertexMax = kSpriteMax * kVertsPerSprite;
	static const size_t kIndexMax = kSpriteMax * kIndicesPerSprite;

	static const size_t kTrianglesPerSprite = 2;

	struct Vertex
	{
		static const int kFloatsPerPosition = 2;
		static const int kFloatsPerUv = 2;
		static const int kFloatsPerChannel = 4;
		static const int kBytesPerColor = 4;

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

	void DrawInternal(TechniqueId techniqueId, const Vector4& channel, const Texture* pTexture, const Rectangle& dest, const Rectangle* src, const Color& color, unsigned int flags);

	GLuint mVertexBufferHandle;
	GLuint mIndexBufferHandle;

	Vertex mVertexData[kVertexMax];
	BatchInfo mBatchData[kSpriteMax];

	bool mInBeginEndBlock;
	size_t mSpriteCount;
	size_t mBatchCount;

	Matrix mTransform;
};
}
}

#endif //MOMO_GRAPHICS_SPRITEBATCH_INCLUDED
