#include "Text\Font.h"

#include "Graphics\Texture.h"
#include "Text\FontLoader.h"

#include <cstring>


namespace Momo
{
	namespace Text
	{
		Font::Font() :
			mpPages(NULL),
			mPageCount(0),
			mpGlyphs(NULL),
			mGlyphCount(0),
			mpKernings(NULL),
			mKerningCount(0),
			mTechniqueId(Graphics::SpriteBatch::TechniqueId::FontNoOutline)
		{
			memset(&mInfo, 0, sizeof(Info));
			memset(&mCommon, 0, sizeof(Common));
		}

		Font::~Font()
		{
			for (unsigned i = 0; i < mPageCount; ++i)
			{
				delete mpPages[i].pTexture;
			}
			delete[](mpPages);
			delete[](mpGlyphs);
			delete[](mpKernings);
		}

		bool Font::Load(const char* pFileName)
		{
			FontLoader loader;
			return loader.Load(pFileName, this);
		}

		const Font::Page* Font::GetPage(unsigned index) const
		{
			if (index < mPageCount)
			{
				return mpPages + index;
			}
			else
			{
				BREAK_MSG("Invalid index!");
				return NULL;
			}
		}

		const Font::Glyph* Font::GetGlyph(u16 unicode) const
		{
			for (unsigned i = 0; i < mGlyphCount; ++i)
			{
				Glyph* pGlyph = &(mpGlyphs[i]);
				if (pGlyph->id == unicode)
				{
					return pGlyph;
				}
			}

			return NULL;
		}

		const Font::Kerning* Font::GetKerning(u16 first, u16 second) const
		{
			for (unsigned i = 0; i < mKerningCount; ++i)
			{
				Kerning* pKerning = &(mpKernings[i]);
				if ((pKerning->first == first) && (pKerning->second == second))
				{
					return pKerning;
				}
			}

			return NULL;
		}

		int Font::MeasureString(const char* pString, size_t length) const
		{
			ASSERT(pString != NULL);

			int longestLength = 0;
			int currentLength = 0;
			char prevChar = 0, curChar = 0;

			for (unsigned i = 0; i < length; ++i)
			{
				prevChar = curChar;
				curChar = pString[i];

				if (i > 0)
				{
					const Font::Kerning* pKerning = GetKerning((u16)prevChar, (u16)curChar);
					if (pKerning != NULL)
					{
						currentLength += pKerning->amount;
					}
				}

				if (curChar == '\n')
				{
					if (currentLength > longestLength)
					{
						longestLength = currentLength;
					}
					currentLength = 0;
				}
				else
				{
					const Font::Glyph* pGlyph = GetGlyph((u16)curChar);
					if (pGlyph == NULL)
					{
						pGlyph = GetGlyph('?');
					}
					ASSERT(pGlyph != NULL);

					currentLength += pGlyph->xAdvance;
				}
			}

			if (currentLength > longestLength)
			{
				longestLength = currentLength;
			}

			return longestLength;
		}
	}
}