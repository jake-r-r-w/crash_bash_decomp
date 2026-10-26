#ifndef BASH_CHASE_H
#define BASH_CHASE_H

/* Chase (overlay chase).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct CH_player CH_player;
typedef struct CH_Bot CH_Bot;
typedef struct EXTobject_CH EXTobject_CH;

enum CHASE_AST_types {
    CHASE_AST_zero = 0x1D00,
    AST_BarrelCH,
    AST_Coal,
    AST_PUGenerator_CH,
    AST_PickupCH,
    AST_CoalFlame,
    AST_BarrelGenerator,
    AST_CircleFire,
    AST_Egg,
    AST_CoalFlameSFX
};

enum CHASE_PU_TYPES {
    CHASEPOWERUP_NONE,
    CHASEPOWERUP_FIREBALL,
    CHASEPOWERUP_LIGHTNING,
    CHASEPOWERUP_NUM_TYPES
};

struct CH_player {
    /* 0x00 */ s32 Energy;
    /* 0x04 */ s32 MainWeaponType;
    /* 0x08 */ s32 MainWeaponRounds;
    /* 0x0C */ s32 MainWeaponFireDelay;
    /* 0x10 */ NAS *SubWeaponNas[3];
    /* 0x1C */ s32 SubWeaponType;
    /* 0x20 */ s32 SubWeaponRounds;
    /* 0x24 */ s32 SubWeaponFireDelay;
    /* 0x28 */ s32 PosX;
    /* 0x2C */ s32 PosY;
    /* 0x30 */ s32 PosZ;
    /* 0x34 */ _sfx_handler *HulFSXHandler;
    /* 0x38 */ _sfx_handler *TurretSFXHandler;
    /* 0x3C */ _sfx_handler *FireBreatherSFXHandler;
    /* 0x40 */ s32 IdleAnimCt;
    /* 0x44 */ s32 Speed;
    /* 0x48 */ NAS *pLastCoalNas;
    /* 0x4C */ NAS *pProtectingNas;
    /* 0x50 */ s32 powerup;
};
BASH_SIZE_CHECK(CH_player, 0x54);

struct CH_Bot {
    /* 0x00 */ s32 Doing;
    /* 0x04 */ s32 DoingTimer;
    /* 0x08 */ u32 AIpad;
    /* 0x0C */ s32 TargetX;
    /* 0x10 */ s32 TargetZ;
    /* 0x14 */ s32 TargetAngle;
    /* 0x18 */ NAS *pBarrelNas;
    /* 0x1C */ NAS *pPickupNas;
    /* 0x20 */ s32 HaveFlame;
    /* 0x24 */ s32 HavePowerup;
    /* 0x28 */ s32 RecentX;
    /* 0x2C */ s32 RecentZ;
};
BASH_SIZE_CHECK(CH_Bot, 0x30);

struct EXTobject_CH {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 Speed;
    /* 0x10 */ s32 SpeedX;
    /* 0x14 */ s32 SpeedY;
    /* 0x18 */ s32 SpeedZ;
    /* 0x1C */ s32 TargetIndex;
    /* 0x20 */ s32 TargetX;
    /* 0x24 */ s32 TargetZ;
    /* 0x28 */ s32 Counter;
    /* 0x2C */ s32 Owner;
    /* 0x30 */ s32 Status;
    /* 0x34 */ s32 *pAnimList;
    /* 0x38 */ NAS *AddrPrevNas;
    /* 0x3C */ NAS *AddrNextNas;
    /* 0x40 */ s32 Earmarked;
};
BASH_SIZE_CHECK(EXTobject_CH, 0x44);

#endif /* BASH_CHASE_H */
