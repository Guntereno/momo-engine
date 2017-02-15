#include "Text/TextWrapper.h"

#include "Text/Alignment.h"
#include "Text/Font.h"

#include <cstring>

namespace Momo
{
namespace Text
{

TextWrapper::TextWrapper(unsigned maxLines):
	mMaxLines(maxLines)
{
	mpLines = new LineData[maxLines];
}

TextWrapper::~TextWrapper()
{
	delete [] mpLines;
}

void TextWrapper::Wrap(const Font* pFont, const char* pText, size_t textLen, const Momo::Rectangle& area, u32 flags)
{
	ASSERT(pFont != NULL);
	ASSERT(pText != NULL);

	mpFont = pFont;
	mpText = pText;
	mTextLen = textLen;
	mArea = area;
	mFlags = flags;
	mLineCount = 0;

	Wrap();
	Position();
}

void TextWrapper::Wrap()
{
	const char* pStart = mpText;
	const char* pEnd = pStart;
	int previousLength = 0;

	for(const char* pCurrent = pStart; pCurrent < (mpText + mTextLen); ++pCurrent)
	{
		bool isFinal = pCurrent == (mpText + (mTextLen - 1));
		if(*pCurrent == ' ' || isFinal)
		{
			if(isFinal)
			{
				++pCurrent;
			}

			int length = mpFont->MeasureString(pStart, (pCurrent - pStart));
			if(length > mArea.width)
			{
				AddLine(pStart, pEnd, previousLength);
				pStart = pEnd = pEnd + 1;
			}
			else
			{
				pEnd = pCurrent;
				previousLength = length;
			}
		}
		else if(*pCurrent == '\n')
		{
			pEnd = pCurrent;
			int length = mpFont->MeasureString(pStart, (pEnd - pStart));
			AddLine(pStart, pEnd, length);
			pStart = pCurrent + 1;
		}
	}

	// Add the remainder
	pEnd = mpText + mTextLen;
	int length = mpFont->MeasureString(pStart, (pEnd - pStart));
	AddLine(pStart, pEnd, length);
}

void TextWrapper::Position()
{
	int lineHeight = mpFont->GetCommon()->lineHeight;
	Point center = mArea.Center();

	int yPos;
	if((mFlags & kVAlignCenter) == kVAlignCenter)
	{
		yPos = center.y + ((mLineCount * lineHeight) / 2) - lineHeight;
	}
	else if((mFlags & kVAlignTop) == kVAlignTop)
	{
		yPos = mArea.Top() - lineHeight;
	}
	else // bottom align
	{
		yPos = mArea.y + ((mLineCount - 1) * lineHeight);
	}

	for(unsigned i=0; i<mLineCount; ++i)
	{
		int xPos;
		int lineLen = mpLines[i].length;
		if((mFlags & kHAlignCenter) == kHAlignCenter)
		{
			xPos = center.x - (lineLen / 2);
		}
		else if((mFlags & kHAlignRight) == kHAlignRight)
		{
			xPos = mArea.Right() - lineLen;
		}
		else // left align
		{
			xPos = mArea.Left();
		}

		mpLines[i].position.Set(xPos, yPos);

		yPos -= lineHeight;
	}
}

void TextWrapper::AddLine(const char* pStart, const char* pEnd, int length)
{
	if(mLineCount < mMaxLines)
	{
		mpLines[mLineCount].pStart = pStart;
		mpLines[mLineCount].pEnd = pEnd;
		mpLines[mLineCount].length = length;
		++mLineCount;
	}
	else
	{
		BREAK_MSG("Exceeded maximum lines in TextWrapper!");
	}
}

const TextWrapper::LineData* TextWrapper::GetLine(unsigned line)
{
	if(line < mLineCount)
	{
		return &(mpLines[line]);
	}

	BREAK_MSG("Line number %d is out of bounds!", line);
	return NULL;
}

}
}