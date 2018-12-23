// esUtil_win32.c
//
//    This file contains the Win32 implementation of the windowing functions.

#if defined(_MSC_VER)
#pragma warning(disable: 4255) // 'EnableMouseInPointerForThread': no function prototype given : converting '()' to '(void)'
#endif

//
// Includes
//

#include <windows.h>
#include "esUtil.h"

#define MOUSELEAVE 1

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//

///
//  ESWindowProc()
//
//      Main window procedure
//
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT  lRet = 1;

    switch (uMsg)
    {
        case WM_CREATE:
            break;

        case WM_SIZE:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (esContext) {
                esContext->width = LOWORD(lParam);
                esContext->height = HIWORD(lParam);

                if (esContext && esContext->resizeFunc)
                    esContext->resizeFunc(esContext, esContext->width, esContext->height);

                InvalidateRect(esContext->hWnd, NULL, FALSE);
            }
        }

        case WM_PAINT:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            if (esContext && esContext->drawFunc)
                esContext->drawFunc(esContext);

            if (esContext)
                ValidateRect(esContext->hWnd, NULL);
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_CHAR:
        {
            POINT      point;
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            GetCursorPos(&point);

            if (esContext && esContext->keyFunc)
                esContext->keyFunc(esContext, (unsigned char)wParam,
                (int)point.x, (int)point.y);
        }
        break;

        case WM_LBUTTONDOWN:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            WORD x = LOWORD(lParam);
            WORD y = HIWORD(lParam);

            if (esContext && esContext->mouseFunc)
                esContext->mouseFunc(esContext, ES_MOUSE_DOWN,
                (int)x, (int)y);
        }
        break;

        case WM_MOUSEMOVE:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            WORD x = LOWORD(lParam);
            WORD y = HIWORD(lParam);

            if (esContext && esContext->mouseFunc)
                esContext->mouseFunc(esContext, ES_MOUSE_MOVE,
                (int)x, (int)y);

            // Start timer for handling mouse leaves
            if (esContext->mouseLeaveTimer == 0)
                esContext->mouseLeaveTimer = SetTimer(esContext->hWnd, MOUSELEAVE, 250, NULL);
        }
        break;

        case WM_LBUTTONUP:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            WORD x = LOWORD(lParam);
            WORD y = HIWORD(lParam);

            if (esContext && esContext->mouseFunc)
                esContext->mouseFunc(esContext, ES_MOUSE_UP,
                (int)x, (int)y);
        }
        break;

        case WM_TIMER:
        {
            ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA);

            if (wParam == MOUSELEAVE)
            {
                POINT pt;
                RECT rect;
                GetCursorPos(&pt);
                GetWindowRect(esContext->hWnd, &rect);
                if (!PtInRect(&rect, pt))
                {
                    if (esContext && esContext->mouseFunc)
                        esContext->mouseFunc(esContext, ES_MOUSE_LEAVE,
                        (int)pt.x, (int)pt.y);

                    if (esContext->mouseLeaveTimer != 0)
                    {
                        KillTimer(esContext->hWnd, esContext->mouseLeaveTimer);
                        esContext->mouseLeaveTimer = 0;
                    }
                }
            }
        }
        break;

        default:
            lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
    }

    return lRet;
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate(ESContext *esContext, LPCTSTR title)
{
    WNDCLASS wndclass = { 0 };
    DWORD    wStyle = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.style = CS_OWNDC;
    wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName = TEXT("opengles2.0");

    if (!RegisterClass(&wndclass))
        return FALSE;

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = esContext->width;
    windowRect.bottom = esContext->height;

    AdjustWindowRect(&windowRect, wStyle, FALSE);

    esContext->hWnd = CreateWindow(
        TEXT("opengles2.0"),
        title,
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    // Set the ESContext* to the GWL_USERDATA so that it is available to the
    // ESWindowProc
    SetWindowLongPtr(esContext->hWnd, GWLP_USERDATA, (LONG_PTR)esContext);

    if (esContext->hWnd == NULL)
        return GL_FALSE;

    ShowWindow(esContext->hWnd, TRUE);

    return GL_TRUE;
}

///
//  winLoop()
//
//      Start main windows loop
//
void WinLoop(ESContext *esContext)
{
    MSG msg = { 0 };
    int done = 0;
    DWORD lastTime = GetTickCount();

    while (!done)
    {
        int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
        DWORD curTime = GetTickCount();
        float deltaTime = (float)(curTime - lastTime) / 1000.0f;
        lastTime = curTime;

        if (gotMsg)
        {
            if (msg.message == WM_QUIT)
            {
                done = 1;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
            SendMessage(esContext->hWnd, WM_PAINT, 0, 0);

        // Call update function if registered
        if (esContext->updateFunc != NULL)
            esContext->updateFunc(esContext, deltaTime);
    }

    if (esContext->mouseLeaveTimer != 0)
    {
        KillTimer(esContext->hWnd, esContext->mouseLeaveTimer);
        esContext->mouseLeaveTimer = 0;
    }
}
