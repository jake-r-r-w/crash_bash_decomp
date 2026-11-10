/* The game's fingerprint (main/fingerprint.c): its own values folded into a number after every step, the same on every machine given the
 * same presses. */
#ifndef BASH_FINGERPRINT_H
#define BASH_FINGERPRINT_H

#include "common.h"

/* Before the first step of a game that every machine starts together. */
void fingerprint_start(void);
/* After a step: every step's fingerprint so far. */
u32 fingerprint_step(void);
/* How many steps it's had. */
s32 fingerprint_steps(void);

#endif
