// esUtil_win.h
//
//   API-neutral interface for creating windows.  Implementation needs to be provided per-platform.

#ifndef ESUTIL_WIN_H
#define ESUTIL_WIN_H

///
//  Includes
//

#ifdef __cplusplus

extern "C" {
#endif

///
//  Macros
//

///
// Types
//

///
//  Public Functions
//

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate ( ESContext *esContext, LPCTSTR title );

///
//  WinLoop()
//
//      Start main windows loop
//
void WinLoop ( ESContext *esContext );

///
//  WinTGALoad()
//
//      TGA loader win32 implementation
//
int WinTGALoad ( const char *fileName, char **buffer, int *width, int *height );

#ifdef __cplusplus
}
#endif

#endif // ESUTIL_WIN_H
