#ifndef SAVES_H
#define SAVES_H

/* The folder saves go in: a given one, none (NULL: nothing is kept), or the player's own. */
void saves_set_folder(const char *folder);
void saves_use_default_folder(void);
/* Slot 0 to 3. 1: it's there and whole. */
int saves_read(int slot, void *data, int size);
/* 1: it's safely written. */
int saves_write(int slot, const void *data, int size);

#endif
