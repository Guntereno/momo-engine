//
/// \file ESUtil.h
/// \brief A utility library for OpenGL ES.  This library provides a
///        basic common framework for the example applications in the
///        OpenGL ES 2.0 Programming Guide.
//
#ifndef ESUTIL_H
#define ESUTIL_H

///
//  Includes
//
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifdef __cplusplus

extern "C" {
#endif

///
//  Macros
//
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flag - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8
/// esCreateWindow flag - EGL_POST_SUB_BUFFER_NV supported.
#define ES_WINDOW_POST_SUB_BUFFER_SUPPORTED 16

#define ES_MOUSE_DOWN           1
#define ES_MOUSE_UP             2
#define ES_MOUSE_MOVE           3
#define ES_MOUSE_LEAVE          4

///
// Types
//

typedef struct
{
    GLfloat   m[4][4];
} ESMatrix;

typedef struct
{
   /// Put your user data here...
   void*       userData;

   /// Window width
   GLint       width;

   /// Window height
   GLint       height;

   /// Window handle
   EGLNativeWindowType  hWnd;

   /// EGL display
   EGLDisplay  eglDisplay;

   /// EGL context
   EGLContext  eglContext;

   /// EGL surface
   EGLSurface  eglSurface;

   /// Callbacks
   void (ESCALLBACK *drawFunc) ( void* );
   void (ESCALLBACK *keyFunc) ( void*, unsigned char, int, int );
   void (ESCALLBACK *mouseFunc) ( void*, unsigned char, int, int );
   void (ESCALLBACK *updateFunc) ( void*, float deltaTime );
   void (ESCALLBACK *resizeFunc) ( void*, int, int );

   UINT_PTR mouseLeaveTimer;
} ESContext;

///
//  Extensions
//

extern PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
extern PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;

extern PFNEGLPOSTSUBBUFFERNVPROC eglPostSubBufferNV;

/*

extern PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;

extern PFNGLDELETEFENCESNVPROC glDeleteFencesNV;
extern PFNGLGENFENCESNVPROC glGenFencesNV;
extern PFNGLGETFENCEIVNVPROC glGetFenceivNV;
extern PFNGLISFENCENVPROC glIsFenceNV;
extern PFNGLFINISHFENCENVPROC glFinishFenceNV;
extern PFNGLSETFENCENVPROC glSetFenceNV;
extern PFNGLTESTFENCENVPROC glTestFenceNV;

*/

///
//  Public Functions
//

//
///
/// \brief Initialize ES framework context.  This must be called before calling any other functions.
/// \param esContext Application context
//
void ESUTIL_API esInitContext ( ESContext *esContext );

//
/// \brief Create a window with the specified parameters
/// \param esContext Application context
/// \param title Name for title bar of window
/// \param width Width in pixels of window to create
/// \param height Height in pixels of window to create
/// \param flags Bitfield for the window creation flags
///         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
///         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
///         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
///         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
///         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
///         ES_WINDOW_POST_SUB_BUFFER_SUPPORTED - specifies that EGL_POST_SUB_BUFFER_NV is supported.
/// \return GL_TRUE if window creation is succesful, GL_FALSE otherwise
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, LPCTSTR title, GLint width, GLint height, GLuint flags );

//
/// \brief Start the main loop for the OpenGL ES application
/// \param esContext Application context
//
void ESUTIL_API esMainLoop ( ESContext *esContext );

//
/// \brief Register a draw callback function to be used to render each frame
/// \param esContext Application context
/// \param drawFunc Draw callback function that will be used to render the scene
//
void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) ( ESContext* ) );

//
/// \brief Register an update callback function to be used to update on each time step
/// \param esContext Application context
/// \param updateFunc Update callback function that will be used to render the scene
//
void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void (ESCALLBACK *updateFunc) ( ESContext*, float ) );

//
/// \brief Register an keyboard input processing callback function
/// \param esContext Application context
/// \param keyFunc Key callback function for application processing of keyboard input
//
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext, void (ESCALLBACK *keyFunc) ( ESContext*, unsigned char, int, int ) );

//
/// \brief Register a mouse input processing callback function
/// \param esContext Application context
/// \param mouseFunc Mouse callback function for application processing of keyboard input
//
void ESUTIL_API esRegisterMouseFunc ( ESContext *esContext, void (ESCALLBACK *mouseFunc) ( ESContext*, unsigned char, int, int ) );

//
/// \brief Register a draw callback function to be used whenever the window is resized
/// \param esContext Application context
/// \param drawFunc Resize callback function that will be used when window is resized
//
void ESUTIL_API esRegisterResizeFunc ( ESContext *esContext, void (ESCALLBACK *resizeFunc) ( ESContext*, int, int ) );

#ifdef __cplusplus
}
#endif

#endif // ESUTIL_H
