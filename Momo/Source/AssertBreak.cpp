#include "AssertBreak.h"

#ifdef USE_ASSERT

#include "Logger.h"

#include <cstdio>

#ifdef WIN32
#include <cstdarg>
#endif // WIN32

extern "C"
void AssertPrint (const char* file, unsigned line, const char* expression)
{
	LOGE("\n%s(%d): ASSERT (%s)\n\n", file, line, expression);
}

extern "C"
void AssertPrintMessage (const char* file, unsigned line, const char* expression, const char* message, ...)
{
	const size_t kBuffLen = 256;
	char buff[kBuffLen];
	va_list args;
	va_start(args, message);
	vsprintf(buff, message, args);
	va_end(args);

	LOGE("\n%s(%d): ASSERT_MSG (%s) %s\n\t", file, line, expression, buff);
}

extern "C"
void BreakPrintMessage (const char* file, unsigned line, const char* message, ...)
{
	const size_t kBuffLen = 256;
	char buff[kBuffLen];
	va_list args;
	va_start(args, message);
	vsprintf(buff, message, args);
	va_end(args);

	LOGE("\n%s(%d): BREAK_MSG\n\t %s", file, line, buff);
}

#endif // USE_ASSERT