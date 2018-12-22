#if defined(_MSC_VER)
#pragma warning(disable: 4204) // nonstandard extension used : non-constant aggregate initializer
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4668) // '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning(disable: 4255) // 'EnableMouseInPointerForThread': no function prototype given : converting '()' to '(void)'
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "esUtil.h"
#include "esUtil_win.h"

///
//  Extensions
//

PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;

PFNEGLPOSTSUBBUFFERNVPROC eglPostSubBufferNV;

/*

PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;

PFNGLDELETEFENCESNVPROC glDeleteFencesNV;
PFNGLGENFENCESNVPROC glGenFencesNV;
PFNGLGETFENCEIVNVPROC glGetFenceivNV;
PFNGLISFENCENVPROC glIsFenceNV;
PFNGLFINISHFENCENVPROC glFinishFenceNV;
PFNGLSETFENCENVPROC glSetFenceNV;
PFNGLTESTFENCENVPROC glTestFenceNV;

*/

///
// CreateEGLContext()
//
//    Creates an EGL rendering context and all associated elements
//
EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
	EGLContext* eglContext, EGLSurface* eglSurface,
	EGLint* configAttribList, EGLint* surfaceAttribList)
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if (display == EGL_NO_DISPLAY)
	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if (!eglInitialize(display, &majorVersion, &minorVersion))
	{
		return EGL_FALSE;
	}

	// Bind to extensions
	eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
	eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");

	eglPostSubBufferNV = (PFNEGLPOSTSUBBUFFERNVPROC)eglGetProcAddress("eglPostSubBufferNV");

	/*
	   glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) eglGetProcAddress("glEGLImageTargetTexture2DOES");

	   glDeleteFencesNV = (PFNGLDELETEFENCESNVPROC) eglGetProcAddress("glDeleteFencesNV");
	   glGenFencesNV = (PFNGLGENFENCESNVPROC) eglGetProcAddress("glGenFencesNV");
	   glGetFenceivNV = (PFNGLGETFENCEIVNVPROC) eglGetProcAddress("glGetFenceivNV");
	   glIsFenceNV = (PFNGLISFENCENVPROC) eglGetProcAddress("glIsFenceNV");
	   glFinishFenceNV = (PFNGLFINISHFENCENVPROC) eglGetProcAddress("glFinishFenceNV");
	   glSetFenceNV = (PFNGLSETFENCENVPROC) eglGetProcAddress("glSetFenceNV");
	   glTestFenceNV = (PFNGLTESTFENCENVPROC) eglGetProcAddress("glTestFenceNV");

	*/

	// Get configs
	if (!eglGetConfigs(display, NULL, 0, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Choose config
	if (!eglChooseConfig(display, configAttribList, &config, 1, &numConfigs))
	{
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, surfaceAttribList);
	if (surface == EGL_NO_SURFACE)
	{
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT)
	{
		return EGL_FALSE;
	}

	// Make the context current
	if (!eglMakeCurrent(display, surface, surface, context))
	{
		return EGL_FALSE;
	}

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
}

///
// DestroyEGLContext()
//
//    Destroys an EGL rendering context and all associated elements
//
EGLBoolean DestroyEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
	EGLContext* eglContext, EGLSurface* eglSurface,
	EGLint* configAttribList, EGLint* surfaceAttribList)
{
	if (!eglDestroyContext(eglDisplay, eglContext))
	{
		return EGL_FALSE;
	}

	if (!eglDestroySurface(eglDisplay, eglSurface))
	{
		return EGL_FALSE;
	}

	if (!eglTerminate(eglDisplay))
	{
		return EGL_FALSE;
	}

	return EGL_TRUE;
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.
//
void ESUTIL_API esInitContext(ESContext *esContext)
{
	if (esContext != NULL)
	{
		memset(esContext, 0, sizeof(ESContext));
	}
}

///
//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
//          ES_WINDOW_POST_SUB_BUFFER_SUPPORTED - specifies that EGL_POST_SUB_BUFFER_NV is supported.
//
GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, LPCTSTR title, GLint width, GLint height, GLuint flags)
{
	EGLint configAttribList[] =
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};
	EGLint surfaceAttribList[] =
	{
		EGL_POST_SUB_BUFFER_SUPPORTED_NV, flags & (ES_WINDOW_POST_SUB_BUFFER_SUPPORTED) ? EGL_TRUE : EGL_FALSE,
		EGL_NONE, EGL_NONE
	};

	if (esContext == NULL)
	{
		return GL_FALSE;
	}

	esContext->width = width;
	esContext->height = height;

	if (!WinCreate(esContext, title))
	{
		return GL_FALSE;
	}

	if (!CreateEGLContext(esContext->hWnd,
		&esContext->eglDisplay,
		&esContext->eglContext,
		&esContext->eglSurface,
		configAttribList,
		surfaceAttribList))
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

///
//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application
//
void ESUTIL_API esMainLoop(ESContext *esContext)
{
	WinLoop(esContext);
}

///
//  esRegisterDrawFunc()
//
void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext*))
{
	esContext->drawFunc = drawFunc;
}

///
//  esRegisterDrawFunc()
//
void ESUTIL_API esRegisterResizeFunc(ESContext *esContext, void (ESCALLBACK *resizeFunc) (ESContext*, int, int))
{
	esContext->resizeFunc = resizeFunc;
}

///
//  esRegisterUpdateFunc()
//
void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext, void (ESCALLBACK *updateFunc) (ESContext*, float))
{
	esContext->updateFunc = updateFunc;
}

///
//  esRegisterKeyFunc()
//
void ESUTIL_API esRegisterKeyFunc(ESContext *esContext,
	void (ESCALLBACK *keyFunc) (ESContext*, unsigned char, int, int))
{
	esContext->keyFunc = keyFunc;
}

///
//  esRegisterMouseFunc()
//
void ESUTIL_API esRegisterMouseFunc(ESContext *esContext,
	void (ESCALLBACK *mouseFunc) (ESContext*, unsigned char, int, int))
{
	esContext->mouseFunc = mouseFunc;
}
