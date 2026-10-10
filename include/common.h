#ifndef COMMON_H
#define COMMON_H

/* maspsx --macro-inc already includes macro.inc (which has an include guard);
 * use it here too instead of labels.inc, which would redefine the macros. */
#define INCLUDE_ASM_USE_MACRO_INC 1
#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

#ifndef NULL
#define NULL ((void*)0)
#endif

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

/* Game-specific structs, globals and prototypes go in their own headers under include/,
 * included from here or from the .c files that need them. */

#endif
