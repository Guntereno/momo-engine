#ifndef MOMO_UI_BUTTONVIEW_INCLUDED
#define MOMO_UI_BUTTONVIEW_INCLUDED

#include "Momo.h"

#include "View.h"
#include "ICallback.h"

#include "Input/InputEvent.h"

namespace Momo
{
namespace Ui
{

class ButtonView : public View
{
public:
    ButtonView();
    virtual ~ButtonView();

    virtual bool HandleInputEvent(const Input::Event& event);

    void RegisterCallback(ICallback* pCallback) { mpCallback = pCallback; }

private:
    DISALLOW_COPY_AND_ASSIGN(ButtonView);

    ICallback* mpCallback;
    Input::Event::Id mTouchId;
};

}
}

#endif //MOMO_UI_BUTTONVIEW_INCLUDED
