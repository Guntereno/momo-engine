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
		const char *v = (const char *)glGetString(s);
		LOGI("GL %s = %s\n", name, v);
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

	void GameBase::Resize(unsigned int width, unsigned int height)
	{
		LOGI("GameBase::Resize(): w=%d h=%d", width, height);

		mViewport.Set(0, 0, width, height);

		mpViewContainer->SetFlags(Ui::View::kFlagFill);
		mpViewContainer->Arrange(mViewport, true);

		GL_CHECK(glViewport(0, 0, width, height))

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
			0.0f - (float)(mViewport.width / 2),
			0.0f - (float)(mViewport.height / 2),
			0.0f);
		mCamera.SetView(view);
		mCamera.SetProjectionOrtho(mViewport, -1.0f, 1.0f);
	}

	void GameBase::OnTouchEvent(Input::Event::Type type, Input::Event::Id id, const Point& pos, const Point& delta)
	{
		LOGI("type:%s id:%d pos:%d,%d delta:%d,%d", Input::Event::ToString(type), id, pos.x, pos.y, delta.x, delta.y);

		Point screenPos = pos;
		screenPos.y = mViewport.height - pos.y;

		Input::Event event = { type, id, screenPos, delta };
		mpInputQueue->Push(event);
	}

}