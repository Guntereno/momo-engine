#ifndef MOMO_LOGGER_INCLUDED
#define MOMO_LOGGER_INCLUDED

#if defined MOMO_DEBUG

# ifdef ANDROID

#  include <android/log.h>

#  define  LOG_TAG    "momo"
#  define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#  define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

# else

void LoggerPrint(const char* fmt, ...);

#  define  LOGI(fmt,...) LoggerPrint(fmt,__VA_ARGS__)
#  define  LOGE(fmt,...) LoggerPrint(fmt,__VA_ARGS__)

# endif

#else // MOMO_DEBUG

# define  LOGI(fmt,...)
# define  LOGE(fmt,...)

#endif

#endif // MOMO_LOGGER_INCLUDED
