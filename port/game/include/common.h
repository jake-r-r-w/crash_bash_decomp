#ifndef COMMON_H
#define COMMON_H

#include "psyq.h"
#include <stddef.h>
#include <stdint.h>

/* A slot that holds a number in some uses and an address in others. */
typedef intptr_t iptr;

/* A pointer in 4 bytes, for where a file's format or a heap block's header has no room for more: an offset from the heap
 * arena. The arena is static, so every heap and static address the game holds is within 2 GB of it. 0 is NULL. */
typedef struct ref32 { int off; } ref32;
#define HEAP_ARENA_BYTES (64 * 1024 * 1024)
extern unsigned char HeapArena[];
#define HEAP_START (HeapArena + 8)
#define HEAP_END (HeapArena + HEAP_ARENA_BYTES)
static inline void *ref_get(ref32 r) { return r.off == 0 ? (void *)0 : r.off == -1 ? (void *)-1 : (void *)(HeapArena + r.off); }
static inline ref32 ref_of(const void *p) { ref32 r; r.off = p == (void *)0 ? 0 : p == (void *)-1 ? -1 : (int)((const unsigned char *)p - HeapArena); return r; }

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

/* Game-specific structs, globals and prototypes go in their own headers under include/,
 * included from here or from the .c files that need them. */

#endif
