#ifndef MOMO_UI_TEXTBOXVIEW_INCLUDED
#define MOMO_UI_TEXTBOXVIEW_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "View.h"

namespace Momo
{
namespace Ui
{

class TextBoxView: public View
{
public:
	TextBoxView(size_t maxLength = 256, unsigned maxLines = 16);
	virtual ~TextBoxView();

	// Override
	virtual void Arrange(const Rectangle& parentRect, bool force = false);

	void SetFont(const Text::Font& font);
	const Text::Font* GetFont() const { return mpFont; }

	void SetColor(const Color& color) { mColor = color; }
	const Color& GetColor() const { return mColor; }

	void SetText(const char* pText);
	const char* GetText() { return mpBuffer; }

	// Use bitwise or of TextWrapper::Flag
	void SetFlags(u32 flags);
	u32 GetFlags() { return mFlags; }

private:
	DISALLOW_COPY_AND_ASSIGN(TextBoxView);

	// Override
	virtual void DrawInternal(Graphics::SpriteBatch& spriteBatch);

	void ReWrap();

	char* mpBuffer;
	size_t mBufferLen;
	const Text::Font* mpFont;
	Color mColor;
	u32 mFlags;

	Text::TextWrapper* mpTextWrapper;

	bool mWrappingDirty;
};

}
}

#endif //MOMO_UI_TEXTBOXVIEW_INCLUDED
