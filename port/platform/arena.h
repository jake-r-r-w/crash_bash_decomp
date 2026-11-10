/* The game's heap arena: one block that every heap and heap reference of the game's is in (game/include/common.h). It's the platform's
 * (overlays.c), not in the game's own data, as that's saved whole and put back when the game starts again, and this is only emptied. */
#ifndef ARENA_H
#define ARENA_H

#define HEAP_ARENA_BYTES (64 * 1024 * 1024)
extern unsigned char HeapArena[];

#endif
