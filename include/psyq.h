#ifndef PSYQ_H
#define PSYQ_H

/* Sony's PsyQ 4.7 headers (ext/psyq47/include), for the library calls the game
 * makes. Included from common.h, so every source gets the real prototypes. */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <libapi.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <libspu.h>
#include <libsnd.h>
#include <libpad.h>
#include <libmcrd.h>

/* GTE macros (DMPSX version 3). Their command words are placeholders that
 * tools/scripts/dmpsx.py turns into cop2 instructions after cc1. */
#include <inline_c.h>
#include <gtemac.h>

#endif
