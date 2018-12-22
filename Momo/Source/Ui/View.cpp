#include "Ui/View.h"

#include "AssertBreak.h"
#include "Logger.h"
#include "Graphics/LineBatch.h"
#include "Graphics/SpriteBatch.h"
#include "Ui/Drawable.h"


namespace Momo
{
	namespace Ui
	{

		View::View() :
			mState(StateId::Default),
			mpSibling(NULL),
			mpChild(NULL),
			mColor(Color::White()),
			mFlags(0),
			mMargin(Offset::Zero()),
			mPadding(Offset::Zero()),
			mArea(Rectangle::Zero()),
			mpBackground(NULL),
			mArrangementDirty(true)
		{
		}

		View::~View()
		{
		}

		void View::Arrange(const Rectangle& parentRect, bool force)
		{
			bool shouldRearrange = force || mArrangementDirty;

			Rectangle area;
			if (shouldRearrange)
			{
				// If we've an empty area, default to fill behaviour
				if
					(
					(mArea == Rectangle::Zero()) ||
						((mFlags & kFlagFill) == kFlagFill)
						)
				{
					area = parentRect;
				}
				else
				{
					area = AlignChildRectangle(parentRect, mArea, mFlags);
				}

				area.Deflate(mMargin);
				mArrangedArea = area;
			}

			mArrangementDirty = false;

			// Apply the padding before drawing children
			area.Deflate(mPadding);
			for (View* pChild = mpChild; pChild != NULL; pChild = pChild->GetSibling())
			{
				pChild->Arrange(area, shouldRearrange);
			}
		}

		void View::Draw(Graphics::SpriteBatch& spriteBatch)
		{
			DrawInternal(spriteBatch);

			for (View* pChild = mpChild; pChild != NULL; pChild = pChild->GetSibling())
			{
				pChild->Draw(spriteBatch);
			}
		}

		void View::DrawDebug(Graphics::LineBatch& lineBatch)
		{
			DrawDebugInternal(lineBatch);

			for (View* pChild = mpChild; pChild != NULL; pChild = pChild->GetSibling())
			{
				pChild->DrawDebug(lineBatch);
			}
		}

		bool View::RecieveInputEvent(const Input::Event& event)
		{
			for (View* pChild = mpChild; pChild != NULL; pChild = pChild->GetSibling())
			{
				bool handled = pChild->RecieveInputEvent(event);
				if (handled)
				{
					return true;
				}
			}

			return HandleInputEvent(event);
		}

		void View::DrawInternal(Graphics::SpriteBatch& spriteBatch)
		{
			if (mpBackground != NULL)
			{
				mpBackground->Draw(spriteBatch, mArrangedArea, mState, mColor);
			}
		}

		void View::DrawDebugInternal(Graphics::LineBatch& lineBatch)
		{
			Color color = { 0xBF00FF00 };
			lineBatch.DrawRectangle(mArrangedArea, color);
		}

		void View::AddChild(View* pChild)
		{
			View** ppView = &mpChild;
			while (*ppView != NULL)
			{
				ppView = &((*ppView)->mpSibling);
			}
			*ppView = pChild;

			// Re-arrange the children here
			Rectangle area = mArrangedArea;
			area.Deflate(mPadding);
		}

		void View::RemoveChild(View* pChild)
		{
			View* pPrevious = NULL;
			for (View* pCheck = mpSibling; pCheck != NULL; pCheck = pCheck->GetSibling())
			{
				if (pCheck == pChild)
				{
					if (pPrevious != NULL)
					{
						pPrevious->mpSibling = pCheck;
						return;
					}
				}

				pPrevious = pCheck;
			}

			BREAK_MSG("Child 0x%x not found!", pChild);
		}

		int View::GetChildCount()
		{
			int count = 0;
			for (View* pChild = mpChild; pChild != NULL; pChild = pChild->GetSibling())
			{
				++count;
			}

			return count;
		}

		Rectangle View::AlignChildRectangle(const Rectangle& parent, const Rectangle& child, u16 flags)
		{
			Rectangle result = child;

			// Move relative to parent
			result.mX += parent.mX;
			result.mY += parent.mY;

			// Align horizontally
			bool touchLeft = ((flags & kFlagTouchLeft) != 0);
			bool touchRight = ((flags & kFlagTouchRight) != 0);
			if (touchLeft || touchRight)
			{
				if (touchLeft && touchRight)
				{
					result.mWidth = parent.mWidth;
					result.mX = parent.Left();
				}
				else if (touchRight)
				{
					result.mX = parent.Right() - child.mWidth;
				}
				else // touchLeft
				{
					result.mX = parent.Left();
				}
			}

			// Align vertically
			bool touchTop = ((flags & kFlagTouchTop) != 0);
			bool touchBottom = ((flags & kFlagTouchBottom) != 0);
			if (touchTop || touchBottom)
			{
				if (touchTop && touchBottom)
				{
					result.mHeight = parent.mHeight;
					result.mY = parent.Top();
				}
				else if (touchTop)
				{
					result.mY = parent.Top() - child.mHeight;
				}
				else // touchBottom
				{
					result.mY = parent.Bottom();
				}
			}

			return result;
		}

		void View::SetFlags(u16 flags)
		{
			mFlags = flags;
			mArrangementDirty = true;
		}

		void View::SetMargin(const Offset& offset)
		{
			mMargin = offset;
			mArrangementDirty = true;
		}

		void View::SetPadding(const Offset& offset)
		{
			mPadding = offset;
			mArrangementDirty = true;
		}

		void View::SetArea(const Rectangle& area)
		{
			mArea = area;
			mArrangementDirty = true;
		}

	}
}