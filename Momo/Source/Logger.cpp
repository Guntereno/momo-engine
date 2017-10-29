#include "Logger.h"

#include <cstdio>

#ifdef ANDROID

	// Nothing

#else

	#include <cstdarg>

	void LoggerPrint(const char* fmt, ...)
	{
		va_list arg;
		va_start(arg, fmt);
		vprintf(fmt, arg);
		va_end(arg);
		printf("\n");
	};

#endif
