#ifndef MOMO_UI_NINEPATCHDRAWABLE_INCLUDED
#define MOMO_UI_NINEPATCHDRAWABLE_INCLUDED

#include "Momo.h"

#include "Drawable.h"

#include "Rectangle.h"
#include "States.h"

namespace Momo
{
namespace Ui
{

class NinePatchDrawable: public Drawable
{
public:
	NinePatchDrawable();
	~NinePatchDrawable();

	// override
	virtual void Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state, const Color& color) const;

	// Nine patch will be arranged such that the outside rect will fill the dest and areas
	// outside of the inside rect will not be scaled
	void SetState(StateId id, const Graphics::Texture* pTexture, const Rectangle& outside, const Rectangle& inside);

private:
	DISALLOW_COPY_AND_ASSIGN(NinePatchDrawable);

	struct State
	{
		const Graphics::Texture* pTexture;
		Rectangle outside;
		Rectangle inside;
	};

	const State* mStates[kStateCount];
};

}
}

#endif //MOMO_UI_NINEPATCHDRAWABLE_INCLUDED
