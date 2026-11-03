/* BPS patches (bps.c), for mods: a patch holds only what it changes of a file, with checksums of the file it's for and of the one it
 * makes, so a mod can change the game's files without having them. */
#ifndef BPS_H
#define BPS_H

#include <stddef.h>
#include <stdint.h>

/* The file the patch makes of source, to free, of no more than limit bytes; or NULL with *problem saying why: in capitals, for the
   launcher. */
uint8_t *bps_apply(const uint8_t *patch, size_t patch_size, const uint8_t *source, size_t source_size, size_t limit, size_t *size,
                   const char **problem);

#endif
