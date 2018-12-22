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
			enum class Flags : u8
			{
				TouchTop = 1 << 0,
				TouchRight = 1 << 1,
				TouchBottom = 1 << 2,
				TouchLeft = 1 << 3,

				Fill = TouchTop | TouchRight | TouchBottom | TouchLeft,
				None = 0
			};

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

			void SetFlags(Flags flags);
			Flags GetFlags() const { return mFlags; }

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

		protected:
			virtual void DrawInternal(Graphics::SpriteBatch& spriteBatch);
			virtual void DrawDebugInternal(Graphics::LineBatch& lineBatch);
			virtual bool HandleInputEvent(const Input::Event& event)
			{
				UNUSED(event);
				return false;
			};

			StateId mState;

		private:
			static Rectangle AlignChildRectangle(const Rectangle& parent, const Rectangle& child,
				Flags flags);

			DISALLOW_COPY_AND_ASSIGN(View);

			View* mpSibling;
			View* mpChild;

			Color mColor;

			Flags mFlags;
			Offset mMargin;
			Offset mPadding;
			Rectangle mArea;

			Rectangle mArrangedArea;

			const Drawable* mpBackground;

			bool mArrangementDirty;
		};

		FLAG_OPS(View::Flags)
	}
}

#endif //MOMO_UI_VIEW_INCLUDED
