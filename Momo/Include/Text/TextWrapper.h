#ifndef MOMO_TEXT_TEXTWRAPPER_INCLUDED
#define MOMO_TEXT_TEXTWRAPPER_INCLUDED

#include "Momo.h"

#include "Point.h"
#include "Rectangle.h"
#include "Text/Alignment.h"

namespace Momo
{
namespace Text
{

class TextWrapper
{
public:
    struct LineData
    {
        const char* pStart;
        const char* pEnd;
        Point position;
        int length;
    };

    TextWrapper(unsigned maxLines);
    ~TextWrapper();

    void Wrap(const Font* pFont, const char* pText, size_t textLen, const Rectangle& area,
        Alignment alignment);

    unsigned GetLineCount() { return mLineCount; }
    const LineData* GetLine(unsigned line);

private:
    void Wrap();
    void Position();
    void AddLine(const char* pStart, const char* pEnd, int length);

    unsigned mMaxLines;

    const Font* mpFont;
    const char* mpText;
    size_t mTextLen;
    Momo::Rectangle mArea;
    Alignment mAlignment;

    LineData* mpLines;
    unsigned mLineCount;
};

}
}

#endif //MOMO_TEXT_TEXTWRAPPER_INCLUDED