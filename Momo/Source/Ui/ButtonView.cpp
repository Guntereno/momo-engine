#include "Ui\ButtonView.h"

namespace Momo
{
	namespace Ui
	{

		ButtonView::ButtonView() :
			View(),
			mTouchId(Input::Event::IdInvalid)
		{

		}

		ButtonView::~ButtonView()
		{

		}

		bool ButtonView::HandleInputEvent(const Input::Event& event)
		{
			Rectangle rect = GetArrangedArea();

			bool consumed = false;

			bool containsPos = rect.Contains(event.pos);
			if (containsPos)
			{
				consumed = true;
			}

			bool isMyTouch = (mTouchId == event.id);

			switch (event.type)
			{
			case Input::Event::Type::Down:
			{
				if (containsPos)
				{
					if (mTouchId == Input::Event::IdInvalid)
					{
						mTouchId = event.id;
						mState = kStatePressed;
					}
				}
			}
			break;

			case Input::Event::Type::Move:
			{
				// Moved out of region
				if (isMyTouch && (!containsPos))
				{
					mState = kStateDefault;
					consumed = true;
				}
			}
			break;

			case Input::Event::Type::Up:
			{
				if (isMyTouch)
				{
					// Only trigger when up occurs within the region
					if ((mState == kStatePressed) && containsPos)
					{
						if (mpCallback != NULL)
						{
							mpCallback->Call(this);
						}
					}

					mTouchId = Input::Event::IdInvalid;
					mState = kStateDefault;
				}
			}
			break;

			case Input::Event::Type::None:
				// Do nothing
				break;
			}

			return consumed;
		}

	}
}
