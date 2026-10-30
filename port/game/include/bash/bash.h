#ifndef BASH_BASH_H
#define BASH_BASH_H

/* Bash, the crate-throwing game (overlay bash).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct MapGrid_XZ MapGrid_XZ;
typedef struct BashSquare BashSquare;

enum BASH_AST_types {
    BASH_AST_zero = 0x1100,
    AST_SnowGlint,
    AST_BashPenguin,
    AST_Bash8way,
    AST_BashHomer,
    AST_BashShox,
    AST_MissileExplode,
    AST_Tumbler,
    AST_Boing
};

struct MapGrid_XZ {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 z;
};
BASH_SIZE_CHECK(MapGrid_XZ, 0x8);

struct BashSquare {
    /* 0x00 */ subengine *sub;
    /* 0x04 */ subengine *base;
    /* 0x08 */ u16 flags;
    /* 0x0A */ u16 status;
};
BASH_SIZE_CHECK(BashSquare, 0xC);

#endif /* BASH_BASH_H */
