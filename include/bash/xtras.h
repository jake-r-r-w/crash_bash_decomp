#ifndef BASH_XTRAS_H
#define BASH_XTRAS_H

/* Crates and pickups shared by the games (XTRAS_AST_types: ovl5d and the
 * engine).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct CrateVelocity CrateVelocity;
typedef struct CrateHookInf CrateHookInf;
typedef struct xzxz xzxz;

enum XTRAS_AST_types {
    XTRAS_AST_zero = 0x200,
    AST_Crate,
    AST_CrateType,
    AST_PACrate,
    AST_PACrateType,
    AST_CrateExplode,
    AST_CratePickup,
    AST_BigPoof,
    AST_ShieldPickup,
    AST_BootsPickup,
    AST_AnvilPickup,
    AST_SlowmoPickup,
    AST_Health,
    AST_SpecialFX
};

enum pu_types {
    ALL_PICKUPS = 0x10,
    PICKUP_HEALTH,
    PICKUP_SHIELD,
    PICKUP_BOOTS,
    PICKUP_ANVIL,
    PICKUP_SLOWMO,
    PICKUP_FAN,
    PICKUP_VAX,
    PICKUP_LIGHT,
    POWERUP_8WAY,
    POWERUP_FIRE,
    POWERUP_HOMER,
    POWERUP_SONIC,
    TYPES_OF_PICKUP,
    PICKUP_NONE
};

enum crate_type {
    ALL_CRATES,
    PICKUP_CRATE,
    POWERUP_CRATE,
    WOOD_CRATE,
    STONE_CRATE,
    TNT_CRATE,
    NITRO_CRATE,
    TICKING_CRATE,
    RED_CRATE,
    BLUE_CRATE,
    GREEN_CRATE,
    YELLOW_CRATE,
    BAD_CRATE,
    TYPES_OF_CRATE,
    PENGUIN_CRATE,
    BOSS_CRATE
};

struct CrateVelocity {
    /* 0x00 */ u16 FVel;
    /* 0x02 */ u16 FMax;
    /* 0x04 */ u16 FAcc;
    /* 0x06 */ u16 FDec;
    /* 0x08 */ u16 YVel;
    /* 0x0A */ u16 YMax;
    /* 0x0C */ u16 YAcc;
    /* 0x0E */ u16 YDec;
    /* 0x10 */ u16 dir;
    /* 0x12 */ u16 dist;
    /* 0x14 */ u16 time;
};
BASH_SIZE_CHECK(CrateVelocity, 0x16);

struct CrateHookInf {
    /* 0x00 */ CrateVelocity **KickVels;
    /* 0x04 */ CrateVelocity **ThrowVels;
    /* 0x08 */ void (*OnExplode)();
    /* 0x0C */ void (*ExploRange)();
};
BASH_SIZE_CHECK(CrateHookInf, 0x10);

struct xzxz {
    /* 0x00 */ s16 x2;
    /* 0x02 */ s16 z2;
    /* 0x04 */ s16 ang1;
    /* 0x06 */ s16 ang2;
};
BASH_SIZE_CHECK(xzxz, 0x8);

#endif /* BASH_XTRAS_H */
