/* Dino's overlay id, the first word of every slot overlay (0x5D..0x6A by overlay). It is a unit
 * of its own: a slot overlay's first C file has 8-aligned rodata and the slot starts at 4 mod 8, so
 * the id can't share its object (ARCHITECTURE.md, Slot overlays). */
#include "common.h"

const s32 DinoOverlayId = 0x65;
