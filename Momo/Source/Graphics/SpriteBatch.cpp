#include "Graphics/SpriteBatch.h"

#include "Logger.h"
#include "AssertBreak.h"
#include "Graphics/Camera.h"
#include "Graphics/Program.h"
#include "Graphics/Technique.h"
#include "Graphics/Utils.h"
#include "Text/Font.h"

#include <cstddef>

//#define OUTPUT_VERTEX_INFO

namespace Momo
{
namespace Graphics
{

static const char* kShaderNames[SpriteBatch::kTechniqueCount][2] =
{
	{ "shaders/vpSpriteBatch.vp", "shaders/fpSpriteBatchSprite.fp" },
	{ "shaders/vpSpriteBatch.vp", "shaders/fpSpriteBatchFontNoOutline.fp" },
	{ "shaders/vpSpriteBatch.vp", "shaders/fpSpriteBatchFontOutline.fp" }
};

static bool gStaticsInitialised = false;
static Technique gTechniques[SpriteBatch::kTechniqueCount];
static Texture* gWhiteTexture = NULL;

static const Vector4 kChannelAll = { 0.0f, 0.0f, 0.0f, 0.0f };

SpriteBatch::SpriteBatch():
	mVertexBufferHandle(0),
	mIndexBufferHandle(0),
	mInBeginEndBlock(false),
	mSpriteCount(0),
	mBatchCount(0)
{
}

void SpriteBatch::Load()
{
	LOGI("SpriteBatch Constructor");

	if (!gStaticsInitialised)
	{
		bool result;

		gWhiteTexture = new Texture();
		result = gWhiteTexture->LoadTga("white.tga");
		ASSERT(result);

		result = LoadTechniques();
		ASSERT(result);

		gStaticsInitialised = true;
	}
	
	// Declare the vertex stream buffer
	glGenBuffers(1, &mVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, kVertexMax * sizeof(Vertex), mVertexData, GL_STREAM_DRAW);

	// Generate the index buffer
	GLushort indices[kIndexMax];
	for(int sprite=0; sprite<kSpriteMax; ++sprite)
	{
		GLushort startIndex = (sprite * (GLushort)kIndicesPerSprite);
		GLushort startVert = (sprite * (GLushort)kVertsPerSprite);

		indices[startIndex + 0] = startVert + 0;
		indices[startIndex + 1] = startVert + 1;
		indices[startIndex + 2] = startVert + 2;

		indices[startIndex + 3] = startVert + 0;
		indices[startIndex + 4] = startVert + 2;
		indices[startIndex + 5] = startVert + 3;
	}

	glGenBuffers(1, &mIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, kIndexMax * sizeof(GLushort), indices, GL_STATIC_DRAW);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SpriteBatch::Begin()
{
	ASSERT(!mInBeginEndBlock);

	mInBeginEndBlock = true;
	mSpriteCount = mBatchCount = 0;
}

void SpriteBatch::Draw(const Texture* texture, const Rectangle& dest, const Color& color)
{
	DrawInternal(SpriteBatch::kTechniqueSprite, kChannelAll, texture, dest, NULL, color, 0);
}

void SpriteBatch::Draw(const Texture* texture, const Rectangle& dest, const Color& color, unsigned int flags)
{
	DrawInternal(SpriteBatch::kTechniqueSprite, kChannelAll, texture, dest, NULL, color, flags);
}

void SpriteBatch::Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color)
{
	DrawInternal(SpriteBatch::kTechniqueSprite, kChannelAll, texture, dest, &src, color, 0);
}

void SpriteBatch::Draw(const Texture* texture, const Rectangle& dest, const Rectangle& src, const Color& color, unsigned int flags)
{
	DrawInternal(SpriteBatch::kTechniqueSprite, kChannelAll, texture, dest, &src, color, flags);
}

void SpriteBatch::DrawString(const Text::Font& font, const char* pUtf8String, size_t strLen, const Point& point, const Color& color)
{
	using namespace Text;

	const Font::Common* pCommon = font.GetCommon();

	Point cursor = point;

	// TODO: utf8 decoding
	char prevChar = 0, curChar = 0;
	for(unsigned i=0; i<strLen; ++i)
	{
		prevChar = curChar;
		curChar = pUtf8String[i];

		if(i>0)
		{
			const Font::Kerning* pKerning = font.GetKerning(prevChar, curChar);
			if(pKerning != NULL)
			{
				cursor.x += pKerning->amount;
			}
		}

		if(curChar == '\n')
		{
			cursor.x = point.x;
			cursor.y -= pCommon->lineHeight;
		}
		else
		{
			const Font::Glyph* pGlyph = font.GetGlyph(curChar);
			if(pGlyph == NULL)
			{
				pGlyph = font.GetGlyph('?');
			}
			ASSERT(pGlyph != NULL);

			const Font::Page* pPage = font.GetPage(pGlyph->page);
			ASSERT(pPage != NULL);

			Rectangle dest =
			{
				cursor.x + pGlyph->xOffset,
				cursor.y + pCommon->lineHeight - pGlyph->yOffset - pGlyph->source.height,
				pGlyph->source.width,
				pGlyph->source.height
			};

			// Greyscale textures should only use the red channel
			static const Vector4 kRedOnly = { 1.0f, 0.0f, 0.0f, 0.0f };
			Vector4 channel = (pPage->pTexture->Format() == GL_LUMINANCE) ? kRedOnly : pGlyph->channel;

			DrawInternal((TechniqueId)font.GetTechniqueId(), channel, pPage->pTexture, dest, &pGlyph->source, color, 0);

			cursor.x += pGlyph->xAdvance;
		}
	}
}

void SpriteBatch::End(const Camera& camera)
{
	ASSERT(mInBeginEndBlock);

	unsigned int currentIndex = 0;
	for(unsigned batch=0; batch<mBatchCount; ++batch)
	{
		const BatchInfo& currentBatch = mBatchData[batch];

		Technique& technique = gTechniques[currentBatch.technique];

		GL_CHECK(glUseProgram(technique.program.Handle()))

		// Set the transform
		glUniformMatrix4fv(technique.uniforms.transform, 1, false, (GLfloat*)(&camera.GetViewProjection()));

		// Send vertex data
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
		GL_CHECK(glBufferSubData (
			GL_ARRAY_BUFFER,
			0,
			mSpriteCount * kVertsPerSprite * sizeof(Vertex),
			mVertexData))

		// Enable the vertex attributes
		GL_CHECK(glVertexAttribPointer(
			technique.attributes.color, Vertex::kBytesPerColor,
			GL_UNSIGNED_BYTE, GL_TRUE,
			sizeof(Vertex),
			(void*)offsetof(struct Vertex, color)))

		GL_CHECK(glVertexAttribPointer(
			technique.attributes.position, Vertex::kFloatsPerPosition,
			GL_FLOAT, GL_FALSE,
			sizeof(Vertex),
			(void*)offsetof(struct Vertex, position)))

		GL_CHECK(glVertexAttribPointer(
			technique.attributes.textureCoord, Vertex::kFloatsPerUv,
			GL_FLOAT, GL_FALSE,
			sizeof(Vertex),
			(void*)offsetof(struct Vertex, uv)))

		GL_CHECK(glVertexAttribPointer(
			technique.attributes.channel, Vertex::kFloatsPerChannel,
			GL_FLOAT, GL_FALSE,
			sizeof(Vertex),
			(void*)offsetof(struct Vertex, channel)))

		GL_CHECK(glEnableVertexAttribArray(technique.attributes.color))
		GL_CHECK(glEnableVertexAttribArray(technique.attributes.position))
		GL_CHECK(glEnableVertexAttribArray(technique.attributes.textureCoord))
		GL_CHECK(glEnableVertexAttribArray(technique.attributes.channel))

		// Set the active texture unit to texture unit 0.
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(technique.uniforms.texture, 0);

		// Bind the texture for this batch
		glBindTexture(GL_TEXTURE_2D, currentBatch.pTexture->Handle());

		// Draw indexed primitives
		int batchIndexCount = currentBatch.count * kIndicesPerSprite;
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle))
		GL_CHECK(glDrawElements(
			GL_TRIANGLES,
			batchIndexCount,
			GL_UNSIGNED_SHORT,
			(void*)(currentIndex * sizeof(GLushort))
			))

		currentIndex += batchIndexCount;
	}

	mInBeginEndBlock = false;
}

bool SpriteBatch::LoadTechniques()
{
	LOGI("LoadTechniques()\n");

	bool result;

	for(int i=0; i<kTechniqueCount; ++i)
	{
		Technique& technique = gTechniques[i];

		result = technique.Load
		(
			kShaderNames[i][0],
			kShaderNames[i][1]
		);

		if (!result)
		{
			return false;
		}
	}

	return true;
}

void SpriteBatch::DrawInternal(TechniqueId techniqueId, const Vector4& channel, const Texture* pTexture, const Rectangle& dest, const Rectangle* src, const Color& color, unsigned int flags)
{
	if(pTexture == NULL)
	{
		pTexture = gWhiteTexture;
	}

	ASSERT(pTexture->Handle() > 0);
	ASSERT(mInBeginEndBlock);

	if(mSpriteCount >= kSpriteMax)
	{
		BREAK();
		return;
	}

	// Create a new batch or increase batch size if needed
	if
	(
		(mBatchCount == 0) ||
		(pTexture != mBatchData[mBatchCount-1].pTexture) ||
		(techniqueId != mBatchData[mBatchCount-1].technique)
	)
	{
		BatchInfo& newBatch = mBatchData[mBatchCount];
		newBatch.pTexture = pTexture;
		newBatch.count = 1;
		newBatch.technique = techniqueId;
		++mBatchCount;
	}
	else
	{
		++(mBatchData[mBatchCount-1].count);
	}

	// Setup the vertices
	Vector2 positions[kVertsPerSprite];
	{
		float left = (float)(dest.Left());
		float right = (float)(dest.Right());
		float top = (float)(dest.Top());
		float bottom = (float)(dest.Bottom());

		positions[0].Set(left, top);
		positions[1].Set(left, bottom);
		positions[2].Set(right, bottom);
		positions[3].Set(right, top);
	}

	// Setup the uvs
	Vector2 uvs[kVertsPerSprite];
	{
		float left, right, top, bottom;

		if(src == NULL)
		{
			if(flags & kFlagFlipX)
			{
				left = 1.0f;
				right = 0.0f;
			}
			else
			{
				left = 0.0f;
				right = 1.0f;
			}

			if(flags & kFlagFlipY)
			{
				top = 1.0f;
				bottom = 0.0f;
			}
			else
			{
				top = 0.0f;
				bottom = 1.0f;
			}
		}
		else
		{
			float unflippedLeft = (float)src->Left() / pTexture->Width();
			float unflippedRight = (float)src->Right() / pTexture->Width();
			float unflippedTop = (float)src->Top() / pTexture->Height();
			float unflippedBottom = (float)src->Bottom() / pTexture->Height();

			if(flags & kFlagFlipX)
			{
				left = unflippedRight;
				right = unflippedLeft;
			}
			else
			{
				left = unflippedLeft;
				right = unflippedRight;
			}

			if(flags & kFlagFlipY)
			{
				top = unflippedBottom;
				bottom = unflippedTop;
			}
			else
			{
				top = unflippedTop;
				bottom = unflippedBottom;
			}
		}

		uvs[0].Set(left, bottom);
		uvs[1].Set(left, top);
		uvs[2].Set(right, top);
		uvs[3].Set(right, bottom);
	}

	const int kFirstVert = mSpriteCount * kVertsPerSprite;
	for(int i=0; i<kVertsPerSprite; ++i)
	{
		int vertIdx = kFirstVert + i;

		mVertexData[vertIdx].position = positions[i];
		mVertexData[vertIdx].uv = uvs[i];
		mVertexData[vertIdx].color = color;
		mVertexData[vertIdx].channel = channel;

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
		LOGI("mVertexData[%d].uv = { %.2f, %.2f }\n",
			vertIdx,
			mVertexData[vertIdx].uv.x,
			mVertexData[vertIdx].uv.y
			);
#endif
	}

	++mSpriteCount;
}

}
}