#include "Main.h"

#include "Momo.h"

#include "ApplicationTimer.h"
#include "Game.h"
#include "Point.h"
#include "esUtil.h"

#include <cstdlib>
#include <windows.h>

GLint kDefaultWidth = 480;
GLint kDefaultHeight = 800;

GLint gWindowWidth = kDefaultWidth;
GLint gWindowHeight = kDefaultHeight;

Momo::Point gMousePos;
bool gMouseDown;

bool gAppPaused = false;

Game gGame;

int Init(ESContext *esContext);
void Resize(ESContext *esContext, int width, int height);
void Update(ESContext * esContext, float frameDelta);
void Draw(ESContext *esContext);
void OnKey(ESContext* esContext, unsigned char key, int keyCode, int unicode);
void OnMouse(ESContext* esContext, unsigned char type, int x, int y);

int Init(ESContext *esContext)
{
    gGame.Init();

    Resize(esContext, gWindowWidth, gWindowHeight);

    return TRUE;
}

void Resize(ESContext *esContext, int width, int height)
{
    UNUSED(esContext); // Not needed for this platform
    gWindowWidth = width;
    gWindowHeight = height;
    gGame.Resize((unsigned int)gWindowWidth, (unsigned int)gWindowHeight);
}

void Update(ESContext *esContext, float frameDelta)
{
    UNUSED(esContext); // Not needed for this platform
    UNUSED(frameDelta); // Not needed for this platform

    gGame.Update();
}

void Draw(ESContext *esContext)
{
    gGame.Draw();
    gGame.DrawDebug();

    eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void OnKey(ESContext* esContext, unsigned char key, int keyCode, int unicode)
{
    UNUSED(keyCode);
    UNUSED(unicode);

    switch (key)
    {
        case 'p':
        case 'P':
        {
            gAppPaused = !gAppPaused;
            if (gAppPaused)
            {
                gGame.Pause();
            }
            else
            {
                gGame.UnPause();
            }
        }
        break;

        case 'r':
        case 'R':
        {
            // Rotate the display
            GLint temp = gWindowWidth;
            gWindowWidth = gWindowHeight;
            gWindowHeight = temp;

            RECT clientRect, windowRect;
            GetClientRect(esContext->hWnd, &clientRect);
            GetWindowRect(esContext->hWnd, &windowRect);

            POINT diff;
            diff.x = (windowRect.right - windowRect.left) - clientRect.right;
            diff.y = (windowRect.bottom - windowRect.top) - clientRect.bottom;

            MoveWindow(esContext->hWnd, windowRect.left, windowRect.top, gWindowWidth + diff.x, gWindowHeight + diff.y, true);
        }
        break;

        default:
            // Do nothing
            break;
    }
}

void OnMouse(ESContext* esContext, unsigned char type, int x, int y)
{
    Momo::Point delta = { 0, 0 };

    switch (type)
    {
        case ES_MOUSE_DOWN:
            gMousePos.Set(x, y);
            gGame.OnTouchEvent(Momo::Input::Event::Type::Down, 0, gMousePos, delta);
            gMouseDown = true;
            break;

        case ES_MOUSE_LEAVE:
            if (esContext->mouseLeaveTimer == 0)
            {
                break;
            }
            // Follow through into...
        case ES_MOUSE_UP:
            if (gMouseDown)
            {
                gMousePos.Set(x, y);
                gGame.OnTouchEvent(Momo::Input::Event::Type::Up, 0, gMousePos, delta);
                gMouseDown = false;
            }
            break;

        case ES_MOUSE_MOVE:
            if (gMouseDown)
            {
                delta.Set(x - gMousePos.mX, y - gMousePos.mY);
                gMousePos.Set(x, y);

                gGame.OnTouchEvent(Momo::Input::Event::Type::Move, 0, gMousePos, delta);
            }
            break;
    }
}

void ShutDown(ESContext *esContext)
{
    UNUSED(esContext);
    gGame.Destroy();
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    ESContext esContext;

    esInitContext(&esContext);

    esCreateWindow(&esContext, TEXT("Momo Windows"), gWindowWidth, gWindowHeight, ES_WINDOW_RGB);

    if (!Init(&esContext))
        return FALSE;

    esRegisterDrawFunc(&esContext, Draw);
    esRegisterKeyFunc(&esContext, OnKey);
    esRegisterUpdateFunc(&esContext, Update);
    esRegisterMouseFunc(&esContext, OnMouse);
    esRegisterResizeFunc(&esContext, Resize);

    esMainLoop(&esContext);

    ShutDown(&esContext);

    return TRUE;
}
