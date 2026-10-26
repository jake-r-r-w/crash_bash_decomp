/* Tank's overlay id, the first word of every slot overlay (0x5D..0x6A by overlay). It is a unit of
 * its own: the first C file's jump tables are 8-aligned and the slot starts at 4 mod 8, so the id
 * can't share their object (ARCHITECTURE.md, Slot overlays). */
#include "common.h"

const s32 TankOverlayId = 0x5F;
