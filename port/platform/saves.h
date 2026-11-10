#ifndef SAVES_H
#define SAVES_H

#include <stddef.h>

/* The folder saves go in: a given one, none (NULL: nothing is kept), or the player's own. */
void saves_set_folder(const char *folder);
void saves_use_default_folder(void);
/* The folder they go in, ending in its separator, or "" for none. */
const char *saves_folder(void);
/* Slot 0 to 3. 1: it's there and whole. */
int saves_read(int slot, void *data, int size);
/* 1: it's safely written. */
int saves_write(int slot, const void *data, int size);

/* Online play (net.c). A machine that joins plays with the host's saves, in memory, and its own files are left alone: saves_put for
   each slot the host has (size 0: an empty one), and saves_in_memory from the game's start till it's over. saves_get is what the host
   sends: a slot's bytes, however many it has (0: empty, or more than there's room for). */
#define SAVES_SLOTS 4
#define SAVES_MOST 1024
void saves_in_memory(int on);
void saves_put(int slot, const void *data, int size);
int saves_get(int slot, void *data, int room);
/* Whether a slot's bytes are a save the game could have made, and so one it can load (game/main/card.c): for a save from another
   machine, an online game's host's, or a recording's, which the game would otherwise take as they are. Size 0 is an empty slot, and
   fine. 0 if not, with what's wrong in why. */
int card_check(const void *data, int size, char *why, size_t room);

#endif
