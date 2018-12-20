#ifndef MOMO_ASSERT_INCLUDED
#define MOMO_ASSERT_INCLUDED

#include "Momo.h"


#ifdef MOMO_DEBUG
# define USE_ASSERT
#endif


// Define implementation macros. Don't call these directly, see definitions at the bottom.
#ifdef USE_ASSERT

# if defined (WIN32)
#  define DEBUG_BREAK() __debugbreak()
# else
#  define DEBUG_BREAK()
# endif

// handle both C and C++ linkage
# ifdef  __cplusplus
extern "C"
{
# endif

	void AssertPrint(const char* file, unsigned line, const char* expression);
	void AssertPrintMessage(const char* file, unsigned line, const char* expression, const char* message, ...);
	void BreakPrintMessage(const char* file, unsigned line, const char* message, ...);

# ifdef __cplusplus
}
# endif

# define ASSERT_IMPL( expression ) \
	if (!(expression)) \
	{ \
		AssertPrint(__FILE__, __LINE__, #expression); \
		DEBUG_BREAK(); \
	}

# define ASSERT_MSG_IMPL( expression, ... ) \
	if (!(expression)) \
	{ \
		AssertPrintMessage(__FILE__, __LINE__, #expression, __VA_ARGS__); \
		DEBUG_BREAK(); \
	}

# define BREAK_MSG_IMPL( ... ) \
	{ \
		BreakPrintMessage(__FILE__, __LINE__, __VA_ARGS__); \
		DEBUG_BREAK(); \
	}

#else  // USE_ASSERT

# define ASSERT_IMPL( expression )          ((void)0)
# define ASSERT_MSG_IMPL( expression, ... ) ((void)0)
# define BREAK_MSG_IMPL( ... )              ((void)0)

# define DEBUG_BREAK()                      ((void)0)

#endif  // USE_ASSERT

// External debugging macros
#define BREAK()                     DEBUG_BREAK()
#define BREAK_MSG(...)              BREAK_MSG_IMPL(__VA_ARGS__)
#define ASSERT(expression)          ASSERT_IMPL(expression)
#define ASSERT_MSG(expression, ...) ASSERT_MSG_IMPL(expression, __VA_ARGS__)

#endif //MOMO_ASSERT_INCLUDED
