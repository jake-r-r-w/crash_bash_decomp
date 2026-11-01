/* Texture packs (texpack.c). */
#ifndef TEXPACK_H
#define TEXPACK_H

#include <stdint.h>

#define TEXPACK_NAME 64

/* The pack in use: a folder's name under packs/, or a path. NULL or empty: none. */
void texpack_select(const char *name);
int texpack_active(void);
int texpack_has(uint64_t hash);
/* A replacement that didn't load isn't asked for again. */
void texpack_drop(uint64_t hash);
/* The replacement's texels, to free, or NULL. */
uint32_t *texpack_load(uint64_t hash, int *width, int *height);
/* The packs there are to choose from. */
int texpack_list(char (*names)[TEXPACK_NAME], int max);

/* For making a pack: each texture as it's first drawn is saved to this folder, named as a pack needs it. */
void texpack_dump_to(const char *folder);
int texpack_dumping(void);
void texpack_dump(uint64_t hash, const uint32_t *texels, int w, int h, int pitch);

#endif
