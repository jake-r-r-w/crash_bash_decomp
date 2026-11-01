/* Vblanks are the one clock, and everything the game's interrupts did on the PS1 happens here, once per vblank: the sound tick, the pads, the audio. */
#include "platform.h"

void pad_vblank(void);
void snd_vblank(void);

static int VBlanks;
static int LastWait;

static void vblank(void) {
    VBlanks++;
    pad_vblank();
    snd_vblank();
    platform_vblank();
}

/* Until this many vblanks after the last wait, and at least to the next one. */
void platform_wait_vblanks(int count) {
    int target = LastWait + (count == 0 ? 1 : count);

    if (target <= VBlanks) {
        target = VBlanks + 1;
    }
    while (VBlanks < target) {
        vblank();
    }
    LastWait = VBlanks;
}
