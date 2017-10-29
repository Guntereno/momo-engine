#ifndef MOMO_TYPES_INCLUDED
#define MOMO_TYPES_INCLUDED

#include "AssertBreak.h"

#include <cstddef>


typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

#ifdef ANDROID

typedef long s64;
typedef unsigned long u64;

#else

typedef __int64 s64;
typedef unsigned __int64 u64;

#endif

//COMPILE_TIME_ASSERT(sizeof(s8) == 1);
//COMPILE_TIME_ASSERT(sizeof(u8) == 1);
//COMPILE_TIME_ASSERT(sizeof(s16) == 2);
//COMPILE_TIME_ASSERT(sizeof(u16) == 2);
//COMPILE_TIME_ASSERT(sizeof(s32) == 4);
//COMPILE_TIME_ASSERT(sizeof(u32) == 4);
//COMPILE_TIME_ASSERT(sizeof(f32) == 4);
//COMPILE_TIME_ASSERT(sizeof(s64) == 8);
//COMPILE_TIME_ASSERT(sizeof(u64) == 8);
//COMPILE_TIME_ASSERT(sizeof(f64) == 8);

#endif //MOMO_TYPES_INCLUDED
