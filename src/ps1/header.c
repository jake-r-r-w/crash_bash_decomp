/* The PS-X EXE header: the first 0x800 bytes of SCUS_945.70, which the BIOS reads to load the
 * body (at t_addr, t_size bytes) and start it at pc0 with the stack at s_addr. The header itself
 * isn't loaded. */
#include "common.h"

extern u8 __SN_ENTRY_POINT[];  /* the startup code (2MBYTE.OBJ) */

typedef struct ExeHeader {
    /* 0x00 */ char id[8];
    /* 0x08 */ u32 text;
    /* 0x0C */ u32 data;
    /* 0x10 */ u32 pc0;        /* the entry point */
    /* 0x14 */ u32 gp0;
    /* 0x18 */ u32 t_addr;     /* where the body loads */
    /* 0x1C */ u32 t_size;     /* the body's size: the file runs to 0x69800 (the Makefile's EXE_FILE_SIZE) */
    /* 0x20 */ u32 d_addr;
    /* 0x24 */ u32 d_size;
    /* 0x28 */ u32 b_addr;
    /* 0x2C */ u32 b_size;
    /* 0x30 */ u32 s_addr;     /* the stack's top */
    /* 0x34 */ u32 s_size;
    /* 0x38 */ u32 saved[5];   /* the BIOS's saved sp, fp, gp, ra and s0 */
    /* 0x4C */ char marker[0x7B4];  /* the licence region text, then zeros */
} ExeHeader;

ExeHeader ExeHeaderData = {
    "PS-X EXE",
    0, 0,
    (u32)__SN_ENTRY_POINT, 0,
    0x80010000, 0x69000,
    0, 0, 0, 0,
    0x801FFFF0, 0,
    { 0, 0, 0, 0, 0 },
    "Sony Computer Entertainment Inc. for North America area",
};
