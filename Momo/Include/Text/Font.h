#ifndef MOMO_TEXT_FONT_INCLUDED
#define MOMO_TEXT_FONT_INCLUDED

#include "Momo.h"

#include "Graphics/SpriteBatch.h"
#include "Rectangle.h"
#include "Vector.h"


namespace Momo
{
	namespace Text
	{

		class Font
		{
		public:
			friend class FontLoader;

			static constexpr int kMaxPages = 8;

			struct Glyph
			{
				u16 id;
				Rectangle source;
				s16 xOffset;
				s16 yOffset;
				s16 xAdvance;
				u16 page;
				Vector4 channel;
			};

			struct Kerning
			{
				u16 first;
				u16 second;
				s16 amount;
			};

			struct Page
			{
				u16 id;
				Graphics::Texture* pTexture;
			};

			struct Info
			{
				static constexpr size_t kStrLen = 32;
				static constexpr int kPaddingCount = 4;
				static constexpr int kSpacingCount = 2;

				enum Flags
				{
					kFlagBold = 0x1,
					kFlagItalic = 0x2,
					kFlagUnicode = 0x4,
					kFlagSmooth = 0x8
				};

				char pName[kStrLen];
				u8 flags;
				u16 size;
				char pCharset[kStrLen];
				u16 stretchH;
				u16 antiAliasing;
				u16 padding[kPaddingCount];
				u16 spacing[kSpacingCount];
				u16 outlineThickness;
			};

			struct Common
			{
				enum ChannelContents
				{
					kChannelGlyph = 0,
					kChannelOutline = 1,
					kChannelGlyphAndOutline = 2,
					kChannelZero = 3,
					kChannel1 = 4
				};

				enum Channel
				{
					kChannelBlue,
					kChannelGreen,
					kChannelRed,
					kChannelAlpha,

					kChannelCount
				};

				u16 lineHeight;
				s16 base;
				u16 scaleW;
				u16 scaleH;
				u16 pages;
				bool packed;
				u8 channelContents[kChannelCount];
			};

			Font();
			virtual ~Font();

			bool Load(const char* pFileName);

			const Info* GetInfo() const { return &mInfo; }
			const Common* GetCommon() const { return &mCommon; }
			const Page* GetPage(unsigned index) const;
			const Glyph* GetGlyph(u16 unicode) const;
			const Kerning* GetKerning(u16 first, u16 second) const;
			Graphics::SpriteBatch::TechniqueId GetTechniqueId() const { return mTechniqueId; }

			int MeasureString(const char* pString, size_t length) const;

		private:
			Info mInfo;
			Common mCommon;

			Page* mpPages;
			u32 mPageCount;

			Glyph* mpGlyphs;
			u32 mGlyphCount;

			Kerning* mpKernings;
			u32 mKerningCount;

			Graphics::SpriteBatch::TechniqueId mTechniqueId;
		};

	}
}

#endif // MOMO_TEXT_FONT_INCLUDED
