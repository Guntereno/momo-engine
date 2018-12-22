#include "Ui/ImageDrawable.h"

#include "AssertBreak.h"
#include "Graphics/SpriteBatch.h"

#include <cstring>


namespace Momo
{
	namespace Ui
	{

		ImageDrawable::ImageDrawable() :
			Drawable()
		{
			memset(mStates, 0, sizeof(mStates));
		}

		ImageDrawable::~ImageDrawable()
		{
			for (int i = 0; i < (int)StateId::Count; ++i)
			{
				if (mStates[i] != NULL)
				{
					delete mStates[i];
				}
			}
		}

		void ImageDrawable::Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state, const Color& color) const
		{
			const State* pState = NULL;
			if (mStates[(int)state] != NULL)
			{
				pState = mStates[(int)state];
			}
			else
			{
				pState = mStates[(int)StateId::Default];
			}

			if (pState == NULL)
			{
				BREAK_MSG("No state data defined!");
				return;
			}

			if (pState->src.mWidth == pState->src.mHeight == 0)
			{
				spriteBatch.Draw(pState->pTexture, dest, color);
			}
			else
			{
				spriteBatch.Draw(pState->pTexture, dest, color);
			}
		}

		void ImageDrawable::SetState(StateId id, const Graphics::Texture* pTexture, const Rectangle& src)
		{
			if (mStates[(int)id] != NULL)
			{
				BREAK_MSG("State %d already assigned!", id);
				delete (mStates[(int)id]);
			}

			State* pState = new State();
			pState->pTexture = pTexture;
			pState->src = src;

			mStates[(int)id] = pState;
		}

	}
}