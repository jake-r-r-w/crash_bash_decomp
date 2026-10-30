#ifndef COMMON_H
#define COMMON_H

/* maspsx --macro-inc already includes macro.inc (which has an include guard);
 * use it here too instead of labels.inc, which would redefine the macros. */
#define INCLUDE_ASM_USE_MACRO_INC 1
#include "include_asm.h"
#include "psyq.h"

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

/* BASE plus a byte offset OFF: where a file's header or a variable-size record puts its parts. And a
 * self-relative entry: the word at P is the offset from P to what it names. */
#define FILE_AT(base, off) ((u8 *)(base) + (off))
#define SELF_REL(p) FILE_AT(p, *(p))

/* Arguments the original's call doesn't pass, because they're already in the callee's registers: the
 * caller's own, untouched. `f(PASSED(arg0))`. Only a PC build's calling convention needs them said. */
#ifdef BASH_PC
#define PASSED(args...) args
/* Where the game uses a null pointer. The PS1 has RAM at address 0, so it reads and writes the kernel's
 * low memory and carries on. A PC can't, so those few places say it, and get a block of their own. */
extern unsigned char PcLowRam[];
#define LOW_RAM(p) ((p) != NULL ? (p) : (__typeof__(p))PcLowRam)
#else
#define PASSED(args...)
#define LOW_RAM(p) (p)
#endif

/* Game-specific structs, globals and prototypes go in their own headers under include/,
 * included from here or from the .c files that need them. */

#endif
