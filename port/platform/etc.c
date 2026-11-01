/* libetc and libapi on PC (ROADMAP-B 6.2): vsync is the one clock, and everything the PS1 does in
#include <stdint.h>
typedef unsigned int u32;
 * its interrupts happens here, once per vblank: the sound tick, the pads, the audio. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <libapi.h>
#include <libetc.h>
#include "platform.h"

void pad_vblank(void);
void snd_vblank(void);

static int VBlanks;
static int LastVSync;

static void vblank(void) {
    VBlanks++;
    pad_vblank();
    snd_vblank();
    platform_vblank();
}

/* VSync(1) gives the time since the last vsync in scanlines, which only sets how much of the heap
 * mal_update compacts per frame and fills the profiling counters. A frame here takes none. */
int VSync(int mode) {
    int target;

    if (mode < 0) {
        return VBlanks;
    }
    if (mode == 1) {
        return 0;
    }
    target = LastVSync + (mode == 0 ? 1 : mode);
    if (target <= VBlanks) {
        target = VBlanks + 1;
    }
    while (VBlanks < target) {
        vblank();
    }
    LastVSync = VBlanks;
    return 0;
}

int ResetCallback(void) {
    return 0;
}
int StopCallback(void) {
    return 0;
}
int SetVideoMode(int mode) {
    return 0;
}

/* The stack is the host's: the game's move of it into the scratchpad does nothing here. */
u32 GetSp(void) {
    return 0;
}
u32 SetSp(u32 sp) {
    return 0;
}

/* The Spyro 3 demo, which is another game's exe. */
int LoadExec(char *file, u32 sp, u32 size) {
    fprintf(stderr, "The game asked to run %s, which the PC build can't.\n", file);
    exit(0);
    return 0;
}

int StopRCnt(u32 spec) {
    return 1;
}
void _96_init(void) {
}

/* The mod-chip check is PS1-only (src/ps1/modchip.c). */
void modchip_check_region(void) {
}
int modchip_step(int arg0) {
    return 0;
}
