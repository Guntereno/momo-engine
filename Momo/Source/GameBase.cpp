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
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

GameBase::GameBase():
	mAppTimer(),
	mGameTime(),
	mViewport(),
	mProjectionMatrix(),
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

	glViewport(0, 0, width, height);
	Graphics::Utils::CheckGlError("glViewport");

	// Setup the view projection matrix
	SetViewProjectionMatrix();
}

void GameBase::Update()
{
	mAppTimer.Update();
	mGameTime.Update(mAppTimer.GetCurrentTick());

	while(mpInputQueue->Count() > 0)
	{
		Input::Event event = mpInputQueue->Pop();

		bool handled = mpViewContainer->RecieveInputEvent(event);

		if(!handled)
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

void GameBase::SetViewProjectionMatrix()
{
	Matrix view, projection;
	view.SetTranslation(
		0.0f - (float)(mViewport.width/2),
		0.0f - (float)(mViewport.height/2),
		0.0f);
	projection.SetOrthographicProjection((float)mViewport.width, (float)mViewport.height, -1.0f, 1.0f);
	Matrix::Multiply(projection, view, mProjectionMatrix);

	mSpriteBatch.SetTransform(mProjectionMatrix);
	mLineBatch.SetTransform(mProjectionMatrix);
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