#include "Ui\ButtonView.h"

#include "Input\InputEvent.h"

namespace Momo
{
namespace Ui
{

ButtonView::ButtonView():
	View(),
	mTouchId(-1)
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
	if(containsPos)
	{
		consumed = true;
	}

	bool isMyTouch = (mTouchId == event.id);
	
	switch(event.type)
	{
	case Input::Event::kDown:
		{
			if(containsPos)
			{
				if(mTouchId == -1)
				{
					mTouchId = event.id;
					mState = kStatePressed;
				}
			}
		}
		break;

	case Input::Event::kMove:
		{
			// Moved out of region
			if(isMyTouch && (!containsPos))
			{
				mState = kStateDefault;
				consumed = true;
			}
		}
		break;

	case Input::Event::kUp:
		{
			if(isMyTouch)
			{
				// Only trigger when up occurs within the region
				if((mState == kStatePressed) && containsPos)
				{
					if(mpCallback != NULL)
					{
						mpCallback->Call(this);
					}
				}

				mTouchId = -1;
				mState = kStateDefault;
			}
		}
		break;

	case Input::Event::kNone:
		// Do nothing
		break;
	}

	return consumed;
}

}
}
