#include "Ui/TextBoxView.h"

#include "AssertBreak.h"
#include "Graphics/SpriteBatch.h"
#include "Text/TextWrapper.h"

#include <cstring>


namespace Momo
{
	namespace Ui
	{

		TextBoxView::TextBoxView(size_t maxLength, unsigned maxLines) :
			View(),
			mpBuffer(NULL),
			mBufferLen(maxLength),
			mpFont(NULL),
			mColor(Color::White()),
			mAlignment(Text::Alignment::HLeft | Text::Alignment::VTop),
			mWrappingDirty(true)
		{
			mpBuffer = new char[mBufferLen];
			memset(mpBuffer, 0, mBufferLen);

			mpTextWrapper = new Momo::Text::TextWrapper(maxLines);
		}

		TextBoxView::~TextBoxView()
		{
			delete[] mpBuffer;
		}

		void TextBoxView::Arrange(const Rectangle& parentRect, bool force)
		{
			bool arrangementDirty = GetArrangementDirty();

			View::Arrange(parentRect, force);

			if (force || arrangementDirty || mWrappingDirty)
			{
				ReWrap();
			}
		}

		void TextBoxView::SetFont(const Text::Font& font)
		{
			mpFont = &font;
			mWrappingDirty = true;
		}

		void TextBoxView::DrawInternal(Graphics::SpriteBatch& spriteBatch)
		{
			View::DrawInternal(spriteBatch);

			if (mpFont == NULL)
			{
				BREAK_MSG("Font unitialised!");
				return;
			}

			for (unsigned i = 0; i < mpTextWrapper->GetLineCount(); ++i)
			{
				const Text::TextWrapper::LineData* pData = mpTextWrapper->GetLine(i);
				if (pData)
				{
					spriteBatch.DrawString(*mpFont, pData->pStart,
						(size_t)(pData->pEnd - pData->pStart), pData->position, mColor);
				}
			}
		}

		void TextBoxView::SetText(const char* pText)
		{
			ASSERT(mpBuffer != NULL);
			ASSERT(pText != NULL);
			strncpy(mpBuffer, pText, mBufferLen);
			mWrappingDirty = true;
		}

		void TextBoxView::ReWrap()
		{
			if (mpFont == NULL)
			{
				BREAK_MSG("Font has not been initialised!");
				return;
			}

			mpTextWrapper->Wrap(mpFont, mpBuffer, strlen(mpBuffer), GetArrangedArea(), mAlignment);

			mWrappingDirty = false;
		}

		void TextBoxView::SetAlignment(Text::Alignment alignment)
		{
			mAlignment = alignment;
			mWrappingDirty = true;
		}

	}
}
