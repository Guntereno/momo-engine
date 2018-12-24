#include "GameBase.h"

#include "Logger.h"

#include "Graphics\Utils.h"
#include "Input\InputEvent.h"
#include "Input\InputQueue.h"
#include "Ui\View.h"

namespace Momo
{

static void PrintGLString(const char *name, GLenum s)
{
#if defined MOMO_DEBUG
    const char *v = (const char *)glGetString(s);
    LOGI("GL %s = %s\n", name, v);
#else
    UNUSED(name);
    UNUSED(s);
#endif
}

GameBase::GameBase() :
    mAppTimer(),
    mGameTime(),
    mViewport(),
    mSpriteBatch()
{
    mpViewContainer = new Ui::View();
    mpInputQueue = new Input::InputQueue();
}

GameBase::~GameBase()
{
    delete(mpInputQueue);
    delete(mpViewContainer);
}

void GameBase::Init()
{
    LOGI("GameBase::Init()");

    PrintGLString("Version", GL_VERSION);
    PrintGLString("Vendor", GL_VENDOR);
    PrintGLString("Renderer", GL_RENDERER);
    PrintGLString("Extensions", GL_EXTENSIONS);

    mAppTimer.Init();
    mLineBatch.Load();
    mSpriteBatch.Load();
}

void GameBase::Resize(u32 width, u32 height)
{
    LOGI("GameBase::Resize(): w=%d h=%d", width, height);

    mViewport.Set(0, 0, (s32)width, (s32)height);

    mpViewContainer->SetFlags(Ui::View::Flags::Fill);
    mpViewContainer->Arrange(mViewport, true);

    GL_CHECK(glViewport(0, 0, (GLsizei)width, (GLsizei)height));

    // Setup the view projection matrix
    InitCamera();
}

void GameBase::Update()
{
    mAppTimer.Update();
    mGameTime.Update(mAppTimer.GetCurrentTick());

    while (mpInputQueue->Count() > 0)
    {
        Input::Event event = mpInputQueue->Pop();

        bool handled = mpViewContainer->RecieveInputEvent(event);

        if (!handled)
        {
            handled = HandleTouchEvent(event);
        }
    }

    mpViewContainer->Arrange(mViewport);
}

void GameBase::Destroy()
{
}

void GameBase::Pause()
{
    mAppTimer.SetPaused(true);
}

void GameBase::UnPause()
{
    mAppTimer.SetPaused(false);
}

void GameBase::InitCamera()
{
    Matrix view;
    view.SetTranslation(
        0.0f - (float)(mViewport.mWidth / 2),
        0.0f - (float)(mViewport.mHeight / 2),
        0.0f);
    mCamera.SetView(view);
    mCamera.SetProjectionOrtho(mViewport, -1.0f, 1.0f);
}

void GameBase::OnTouchEvent(Input::Event::Type type, Input::Event::Id id, const Point& pos, const Point& delta)
{
    LOGI("type:%s id:%d pos:%d,%d delta:%d,%d", Input::Event::ToString(type), id, pos.mX, pos.mY, delta.mX, delta.mY);

    Point screenPos = pos;
    screenPos.mY = mViewport.mHeight - pos.mY;

    Input::Event event = { type, id, screenPos, delta };
    mpInputQueue->Push(event);
}

}
