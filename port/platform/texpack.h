/* Textures in place of the game's, from mods (texpack.c). */
#ifndef TEXPACK_H
#define TEXPACK_H

#include <stdint.h>

/* Whether any are in use, and whether this one is. */
int texpack_active(void);
int texpack_has(uint64_t hash);
/* A replacement that didn't load isn't asked for again. */
void texpack_drop(uint64_t hash);
/* The replacement's texels, to free, or NULL. */
uint32_t *texpack_load(uint64_t hash, int *width, int *height);
/* Changes whenever the textures in use do, as mods are turned on and off. */
unsigned texpack_generation(void);

/* For making them: each texture as it's first drawn is saved to this folder, named as a mod needs it. */
void texpack_dump_to(const char *folder);
int texpack_dumping(void);
/* Its rows are rotated by top: it's saved as it is at rest. */
void texpack_dump(uint64_t hash, const uint32_t *texels, int w, int h, int pitch, int top);

#endif
