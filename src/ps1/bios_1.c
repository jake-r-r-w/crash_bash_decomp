#include "common.h"

/* libcd's bios_1.o .data as the game has it (CD_cbsync at +0x8 .. CD_comstr at +0x2C, the
 * command and interrupt name tables, the CD registers at +0x2CC ..). Sony's PsyQ 4.7 object
 * has 1 where the game has 0 at +0x2B0 (configs/USA/psyq/data_keep.txt): an older libcd, so
 * the object's own .data can't be linked. The object's .data still sits here NOLOAD, for its
 * own references (configs/USA/psyq/sections.ld); these are its bytes. The pointers are into
 * the object's .rdata and .bss, whose addresses undefined_syms_manual.txt gives. */
extern u8 libcd_bios_1_rdata[];
extern u8 libcd_bios_1_bss[];

s32 libcd_bios_1_data[0x310 / 4] = {
    0x47350, 0x470000, 0, 0,  /* +0x000 */
    0, 0, 0, 0,  /* +0x010 */
    2, 0, 0, (s32)(libcd_bios_1_rdata + 0xF4),  /* +0x020 */
    (s32)(libcd_bios_1_rdata + 0xEC), (s32)(libcd_bios_1_rdata + 0xE0), (s32)(libcd_bios_1_rdata + 0xD8), (s32)(libcd_bios_1_rdata + 0xCC),  /* +0x030 */
    (s32)(libcd_bios_1_rdata + 0xC0), (s32)(libcd_bios_1_rdata + 0xB4), (s32)(libcd_bios_1_rdata + 0xA8), (s32)(libcd_bios_1_rdata + 0xA0),  /* +0x040 */
    (s32)(libcd_bios_1_rdata + 0x94), (s32)(libcd_bios_1_rdata + 0x88), (s32)(libcd_bios_1_rdata + 0x80), (s32)(libcd_bios_1_rdata + 0x74),  /* +0x050 */
    (s32)(libcd_bios_1_rdata + 0x64), (s32)(libcd_bios_1_rdata + 0x58), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x48),  /* +0x060 */
    (s32)(libcd_bios_1_rdata + 0x3C), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x30), (s32)(libcd_bios_1_rdata + 0x24),  /* +0x070 */
    (s32)(libcd_bios_1_rdata + 0x18), (s32)(libcd_bios_1_rdata + 0xC), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x54),  /* +0x080 */
    (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x0), (s32)(libcd_bios_1_rdata + 0x54),  /* +0x090 */
    (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x134),  /* +0x0A0 */
    (s32)(libcd_bios_1_rdata + 0x128), (s32)(libcd_bios_1_rdata + 0x11C), (s32)(libcd_bios_1_rdata + 0x110), (s32)(libcd_bios_1_rdata + 0x108),  /* +0x0B0 */
    (s32)(libcd_bios_1_rdata + 0xFC), (s32)(libcd_bios_1_rdata + 0x54), (s32)(libcd_bios_1_rdata + 0x54), 0,  /* +0x0C0 */
    0, 0, 0, 0,  /* +0x0D0 */
    0, 0, 1, 1,  /* +0x0E0 */
    1, 1, 0, 0,  /* +0x0F0 */
    0, 0, 0, 0,  /* +0x100 */
    0, 1, 0, 0,  /* +0x110 */
    1, 1, 0, 0,  /* +0x120 */
    0, 1, 0, 0,  /* +0x130 */
    0, 0, 0, 0,  /* +0x140 */
    0, 0, 1, 1,  /* +0x150 */
    1, 1, 0, 0,  /* +0x160 */
    0, 0, 0, 0,  /* +0x170 */
    0, 0, 0, 0,  /* +0x180 */
    0, 0, 0, 0,  /* +0x190 */
    1, 1, 0, 0,  /* +0x1A0 */
    0, 0, 1, 0,  /* +0x1B0 */
    0, 0, 0, 1,  /* +0x1C0 */
    1, 1, 1, 1,  /* +0x1D0 */
    1, 1, 1, 1,  /* +0x1E0 */
    1, 1, 1, 1,  /* +0x1F0 */
    1, 1, 1, 0,  /* +0x200 */
    0, 1, 1, 1,  /* +0x210 */
    1, 1, 1, 1,  /* +0x220 */
    1, 1, 1, 1,  /* +0x230 */
    1, 1, 1, 0,  /* +0x240 */
    0, 3, 0, 0,  /* +0x250 */
    0, 0, 0, 0,  /* +0x260 */
    0, 0, 0, 0,  /* +0x270 */
    2, 1, 0, 0,  /* +0x280 */
    0, 1, 0, 1,  /* +0x290 */
    0, 0, 0, 0,  /* +0x2A0 */
    0, 0, 0, 0,  /* +0x2B0 */
    0, 0, 0, 0x1F801800,  /* +0x2C0 */
    0x1F801803, 0x1F801020, 0x1F801801, 0x1F801802,  /* +0x2D0 */
    0x1F801C00, 0, (s32)((u8 *)libcd_bios_1_data + 0x2E4), (s32)(libcd_bios_1_bss + 0x0),  /* +0x2E0 */
    (s32)((u8 *)libcd_bios_1_data + 0x25), (s32)((u8 *)libcd_bios_1_data + 0x14), (s32)((u8 *)libcd_bios_1_data + 0x20), (s32)(libcd_bios_1_rdata + 0x1F8),  /* +0x2F0 */
    0x1F8010B8, 0, 0, 0,  /* +0x300 */
};
