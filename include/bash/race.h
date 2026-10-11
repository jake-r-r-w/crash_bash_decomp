#ifndef BASH_RACE_H
#define BASH_RACE_H

/* Race (overlay ovl63).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct EXTdinoegg EXTdinoegg;
typedef struct RaceBarrel RaceBarrel;

enum RACE_AST_types {
    RACE_AST_zero = 0x1700,
    AST_RaceMissile,
    AST_RacePickUp,
    AST_TNTCrate,
    AST_DinoEgg,
    AST_RaceFire,
    AST_RaceTag,
    AST_RaceVolcano,
    AST_RaceShield,
    AST_RaceSmokePoof,
    AST_RaceSmokeBoost,
    AST_Glob,
    AST_BarrelRoll,
    AST_TrackGlob,
    AST_Dac,
    AST_RaceGreenSmokePoof,
    AST_RaceGreenSmokeBoost,
    AST_RaceSmokeBoostWater
};

struct EXTdinoegg {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 owner;
    /* 0x10 */ s32 NasStatus;
    /* 0x14 */ s32 angcounter;
    /* 0x18 */ s32 yang;
    /* 0x1C */ EXTengine *shadow;
    /* 0x20 */ s32 vel;
    /* 0x24 */ s32 acc;
    /* 0x28 */ s32 dec;
    /* 0x2C */ s32 timer;
    /* 0x30 */ EXTanimator *anim;
};
BASH_SIZE_CHECK(EXTdinoegg, 0x34);

struct RaceBarrel {
    /* 0x00 */ EXTheromove move;
    /* 0x54 */ subengine *sub;
    /* 0x58 */ s32 bStatus;
    /* 0x5C */ SLONG_XYZ startpos;
    /* 0x68 */ s32 waittime;
};
BASH_SIZE_CHECK(RaceBarrel, 0x6C);

#endif /* BASH_RACE_H */
