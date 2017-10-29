#include "Ui/NinePatchDrawable.h"

#include "AssertBreak.h"
#include "Graphics/SpriteBatch.h"

#include <cstring>

namespace Momo
{
	namespace Ui
	{

		NinePatchDrawable::NinePatchDrawable() :
			Drawable()
		{
			memset(mStates, 0, sizeof(mStates));
		}

		NinePatchDrawable::~NinePatchDrawable()
		{
			for (int i = 0; i < kStateCount; ++i)
			{
				if (mStates[i] != NULL)
				{
					delete mStates[i];
				}
			}
		}

		void NinePatchDrawable::Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state, const Color& color) const
		{
			const State* pState = NULL;
			if (mStates[state] != NULL)
			{
				pState = mStates[state];
			}
			else
			{
				pState = mStates[kStateDefault];
			}

			if (pState == NULL)
			{
				BREAK_MSG("No state data defined!");
				return;
			}

			int srcRowHeights[] =
			{
				pState->pTexture->Height() - pState->inside.Top(),
				pState->inside.height,
				pState->inside.Bottom()
			};
			int srcColumnWidths[] =
			{
				pState->pTexture->Width() - pState->inside.Right(),
				pState->inside.width,
				pState->inside.Left()
			};

			int margin[] =
			{
				pState->outside.Top() - pState->inside.Top(),
				pState->outside.Right() - pState->inside.Right(),
				pState->inside.Bottom() - pState->outside.Bottom(),
				pState->inside.Left() - pState->outside.Left()
			};

			int destInsideHeight = dest.height - (margin[0] + margin[2]);
			int destInsideWidth = dest.width - (margin[1] + margin[3]);

			const int kNumRows = 3;
			const int kNumColumns = 3;

			int destLeft = dest.Left() - pState->outside.Left();
			int destTop = dest.Top() - (pState->pTexture->Height() - pState->outside.Top());

			Rectangle curSrc = { 0, 0, 0, 0 };
			Rectangle curDest = { destLeft, destTop, 0, 0 };

			for (int x = 0; x < kNumColumns; ++x)
			{
				curSrc.width = srcColumnWidths[x];
				curDest.width = (x == 1) ? destInsideWidth : curSrc.width;

				for (int y = 0; y < kNumRows; ++y)
				{
					curSrc.height = srcRowHeights[y];
					curDest.height = (y == 1) ? destInsideHeight : curSrc.height;
					curDest.y -= curDest.height;

					spriteBatch.Draw(pState->pTexture, curDest, curSrc, color);

					curSrc.y += curSrc.height;
				}

				curSrc.y = pState->pTexture->Height();
				curDest.y = destTop;

				curSrc.x += curSrc.width;
				curDest.x += curDest.width;
			}
		}

		void NinePatchDrawable::SetState(StateId id, const Graphics::Texture* pTexture, const Rectangle& outside, const Rectangle& inside)
		{
			if (mStates[id] != NULL)
			{
				BREAK_MSG("State %d already assigned!", id);
				delete (mStates[id]);
			}

			State* pState = new State();
			pState->pTexture = pTexture;
			pState->outside = outside;
			pState->inside = inside;

			mStates[id] = pState;
		}

	}
}