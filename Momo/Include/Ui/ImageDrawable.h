#ifndef MOMO_UI_IMAGEDRAWABLE_INCLUDED
#define MOMO_UI_IMAGEDRAWABLE_INCLUDED

#include "Momo.h"

#include "Drawable.h"

#include "Rectangle.h"
#include "States.h"

namespace Momo
{
namespace Ui
{

class ImageDrawable: public Drawable
{
public:
	ImageDrawable();
	virtual ~ImageDrawable();

	// override
	virtual void Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state, const Color& color) const;

	// pTexture - Can specify NULL to use no texture
	// src - Use Rectangle::Zero() to render the whole texture
	void SetState(StateId id, const Graphics::Texture* pTexture, const Rectangle& src);

private:
	DISALLOW_COPY_AND_ASSIGN(ImageDrawable);

	struct State
	{
		const Graphics::Texture* pTexture;
		Rectangle src;
	};

	const State* mStates[kStateCount];
};

}
}

#endif //MOMO_UI_IMAGEDRAWABLE_INCLUDED
