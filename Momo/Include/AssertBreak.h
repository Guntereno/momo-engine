#ifndef MOMO_ASSERT_INCLUDED
#define MOMO_ASSERT_INCLUDED

#include "Momo.h"

#ifdef MOMO_DEBUG
#define USE_ASSERT
#endif

#ifdef USE_ASSERT

#if defined (WIN32)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK()
#endif

// handle both C and C++ linkage
#ifdef  __cplusplus
extern "C"
{
#endif

void AssertPrint (const char* file, unsigned line, const char* expression);
void AssertPrintMessage (const char* file, unsigned line, const char* expression, const char* message, ...);
void BreakPrintMessage (const char* file, unsigned line, const char* message, ...);

#ifdef __cplusplus
}
#endif

#define Assert( expression ) \
	if (expression) {} \
		else \
{ \
	AssertPrint(__FILE__, __LINE__, #expression); \
	DEBUG_BREAK(); \
}

#define AssertMessage( expression, ... ) \
	if (expression) {} \
		else \
{ \
	AssertPrintMessage(__FILE__, __LINE__, #expression, __VA_ARGS__); \
	DEBUG_BREAK(); \
}

#define DebugBreakMessage( ... ) \
{ \
	BreakPrintMessage(__FILE__, __LINE__, __VA_ARGS__); \
	DEBUG_BREAK(); \
}

#else  // USE_ASSERT

#define Assert( expression )              ((void)0)
#define AssertMessage( expression, ... )  ((void)0)
#define DEBUG_BREAK()                      ((void)0)
#define DebugBreakMessage( ... )          ((void)0)

#endif  // USE_ASSERT

#define BREAK()                     DEBUG_BREAK()
#define BREAK_MSG(...)              DebugBreakMessage(__VA_ARGS__)
#define ASSERT(expression)          Assert(expression)
#define ASSERT_MSG(expression, ...) AssertMessage(expression, __VA_ARGS__)

// TODO: Implement compile time asserts
#define COMPILE_TIME_ASSERT(expression)

#endif //MOMO_ASSERT_INCLUDED
