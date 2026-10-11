#ifndef BASH_PAPU_H
#define BASH_PAPU_H

/* Papu Papu, the first boss (overlay ovl5e).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct PapuFloor_info PapuFloor_info;
typedef struct PapuMinion_info PapuMinion_info;
typedef struct PapuSquare PapuSquare;

enum PAPU_AST_types {
    PAPU_AST_zero = 0x1200,
    AST_PapuBoss,
    AST_PapuMinion,
    AST_PapuColumn
};

struct PapuFloor_info {
    /* 0x00 */ u16 x_master;
    /* 0x02 */ u16 z_master;
    /* 0x04 */ u16 delay_master;
    /* 0x06 */ u16 wait;
    /* 0x08 */ s16 xstep;
    /* 0x0A */ s16 zstep;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 z;
    /* 0x10 */ u16 delay;
    /* 0x12 */ u16 status;
};
BASH_SIZE_CHECK(PapuFloor_info, 0x14);

struct PapuMinion_info {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ s32 z;
    /* 0x0C */ s16 delay;
};
BASH_SIZE_CHECK(PapuMinion_info, 0x10);

struct PapuSquare {
    /* 0x00 */ subengine *sub;
    /* 0x04 */ subengine *base;
    /* 0x08 */ u16 flags;
    /* 0x0A */ u16 status;
    /* 0x0C */ u16 dist;
    /* 0x0E */ u16 anim;
    /* 0x10 */ u16 frame;
    /* 0x12 */ s16 yoffset;
};
BASH_SIZE_CHECK(PapuSquare, 0x14);

#endif /* BASH_PAPU_H */
