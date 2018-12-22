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
						mState = StateId::Pressed;
					}
				}
			}
			break;

			case Input::Event::Type::Move:
			{
				// Moved out of region
				if (isMyTouch && (!containsPos))
				{
					mState = StateId::Default;
					consumed = true;
				}
			}
			break;

			case Input::Event::Type::Up:
			{
				if (isMyTouch)
				{
					// Only trigger when up occurs within the region
					if ((mState == StateId::Pressed) && containsPos)
					{
						if (mpCallback != NULL)
						{
							mpCallback->Call(this);
						}
					}

					mTouchId = Input::Event::IdInvalid;
					mState = StateId::Default;
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
