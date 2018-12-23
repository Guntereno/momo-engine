#ifndef MOMO_GAMEBASE_INCLUDED
#define MOMO_GAMEBASE_INCLUDED

#include "Momo.h"

#include "ApplicationTimer.h"
#include "GameTime.h"
#include "Rectangle.h"
#include "Matrix.h"
#include "Graphics/Camera.h"
#include "Graphics/LineBatch.h"
#include "Graphics/SpriteBatch.h"
#include "Input/InputEvent.h"


namespace Momo
{

class GameBase
{
public:
    GameBase();
    virtual ~GameBase();

    virtual void Init();
    virtual void Resize(u32 mWidth, u32 mHeight);
    virtual void Update();
    virtual void Draw() = 0;
    virtual void DrawDebug() = 0;
    virtual void Destroy();
    virtual void Pause();
    virtual void UnPause();
    virtual bool HandleTouchEvent(const Input::Event& event)
    {
        UNUSED(event);
        return false;
    };

    void OnTouchEvent(Input::Event::Type type, Input::Event::Id id, const Point& pos, const Point& delta);

protected:
    void InitCamera();

    ApplicationTimer mAppTimer;
    GameTime mGameTime;

    Rectangle mViewport;
    Graphics::Camera mCamera;
    Graphics::SpriteBatch mSpriteBatch;
    Graphics::LineBatch mLineBatch;

    Ui::View* mpViewContainer;
    Input::InputQueue* mpInputQueue;

private:
    DISALLOW_COPY_AND_ASSIGN(GameBase);
};

}

#endif // MOMO_GAMEBASE_INCLUDED
