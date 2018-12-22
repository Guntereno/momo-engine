#ifndef MOMO_UI_DRAWABLE_INCLUDED
#define MOMO_UI_DRAWABLE_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Ui/States.h"


namespace Momo
{
	namespace Ui
	{

		class Drawable
		{
		public:
			Drawable() {}

			virtual void Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state, const Color& color) const = 0;

			void Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest, StateId state)
			{
				Draw(spriteBatch, dest, state, Color::White());
			}

			void Draw(Graphics::SpriteBatch& spriteBatch, const Rectangle& dest)
			{
				Draw(spriteBatch, dest, StateId::Default, Color::White());
			}

		private:
			DISALLOW_COPY_AND_ASSIGN(Drawable);
		};

	}
}

#endif //MOMO_UI_DRAWABLE_INCLUDED
