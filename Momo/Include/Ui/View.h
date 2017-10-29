#ifndef MOMO_UI_VIEW_INCLUDED
#define MOMO_UI_VIEW_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Offset.h"
#include "Rectangle.h"
#include "Ui/States.h"


namespace Momo
{
	namespace Ui
	{

		class View
		{
		public:
			View();
			virtual ~View();

			virtual void Arrange(const Rectangle& parentRect, bool force = false);
			void Draw(Graphics::SpriteBatch& spriteBatch);
			void DrawDebug(Graphics::LineBatch& lineBatch);
			bool RecieveInputEvent(const Input::Event& event);

			void AddChild(View* pChild);
			void RemoveChild(View* pChild);

			int GetChildCount();
			View* GetSibling() { return mpSibling; }

			StateId GetState() const { return mState; }

			void SetColor(const Color& color) { mColor = color; }
			const Color& GetColor() const { return mColor; }

			void SetFlags(u16 flags);
			u16 GetFlags() const { return mFlags; }

			void SetMargin(const Offset& offset);
			const Offset& GetMargin() const { return mMargin; }

			void SetPadding(const Offset& offset);
			const Offset& GetPadding() const { return mPadding; }

			void SetArea(const Rectangle& area);
			const Rectangle& GetArea() { return mArea; }

			void SetBackground(const Drawable* pDrawable) { mpBackground = pDrawable; }
			const Drawable* GetBackground() const { return mpBackground; }

			const Rectangle& GetArrangedArea() const { return mArrangedArea; }

			bool GetArrangementDirty() const { return mArrangementDirty; }

			enum Flag
			{
				kFlagTouchTop = 0x1,
				kFlagTouchRight = 0x2,
				kFlagTouchBottom = 0x4,
				kFlagTouchLeft = 0x08,
				kFlagFill = kFlagTouchTop | kFlagTouchRight | kFlagTouchBottom | kFlagTouchLeft
			};

		protected:
			virtual void DrawInternal(Graphics::SpriteBatch& spriteBatch);
			virtual void DrawDebugInternal(Graphics::LineBatch& lineBatch);
			virtual bool HandleInputEvent(const Input::Event& event) { return false; };

			StateId mState;

		private:
			static Rectangle AlignChildRectangle(const Rectangle& parent, const Rectangle& child, u16 flags);

			DISALLOW_COPY_AND_ASSIGN(View);

			View* mpSibling;
			View* mpChild;

			Color mColor;

			u16 mFlags;
			Offset mMargin;
			Offset mPadding;
			Rectangle mArea;

			Rectangle mArrangedArea;

			const Drawable* mpBackground;

			bool mArrangementDirty;
		};

	}
}

#endif //MOMO_UI_VIEW_INCLUDED
