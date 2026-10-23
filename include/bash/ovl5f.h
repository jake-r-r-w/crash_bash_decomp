#ifndef BASH_OVL5F_H
#define BASH_OVL5F_H

/* Functions and data defined in the ovl5f overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl5f/ovl5f.c */

#include "common.h"
#include "bash/nas.h"

typedef struct Ovl5fModeView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
} Ovl5fModeView;

extern s32 D_800CB210;
extern NAS *D_800CB224[3];

typedef struct TotemFux {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s32 timer;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ u32 state;
} TotemFux;
extern NAS *D_800CB230[4];
extern NAS *D_800CB240[2];
extern u32 D_800CA48C[];
extern s32 D_800CA44C[];
typedef struct { u16 v, pad; } RockTblEnt;
extern RockTblEnt D_800CA6D4[];
extern RockTblEnt D_800C938C[]; /* PlotMine: frame by countdown */
extern RockTblEnt D_800C939C[]; /* PlotMine: frame by countdown, modes 8/9 */

extern u8 D_80053774[];
extern s32 D_800C8F14[]; /* TA_InitHero: tank speed by hero_name - 1 */
extern u8 D_800C9EE8[];
extern u8 D_800C9EFC[];
extern u8 D_800C9ED0[];
extern u8 D_800C9EB0[];
extern u8 D_800C9E9C[];
extern u8 D_800C9EC4[];

void func_800C0C68(NAS *nas, s32 id);
void func_800C0F24(NAS *nas, s32 id);

void func_800C68A8(void);
void func_800C69BC(void);
void func_800C831C(void);
void func_800C83D0(void);
extern s32 D_800CB25C;
extern s32 D_800C93AC[];
extern s32 D_800C9A54[];
extern s32 D_800C9AA4[];
extern s32 D_800C9AF4[];
extern s32 D_800C9B44[];
extern s32 D_800CB254;
extern s32 D_800CB258;
extern s16 D_8009D634;
extern s16 D_8009D6A0;
extern s32 D_800CB210;
extern u8 D_800C8EA0[];
extern u8 D_800C8EB4[];
extern u8 D_800C9E74[];
extern u8 D_800C9E88[];
extern u8 D_800C9E5C[];
extern u8 D_800C9E3C[];
extern u8 D_800C9E28[];
extern u8 D_800C9E50[];
void TA_InitHero(u32 player);
void TA_MoveHero(u32 player);
void func_800B5588(void);
void func_800B58B0(void);
void func_800B5FC4(void);
void func_800B62A4(void);
void func_800C8984(s32 arg0, s32 arg1);
void func_800C89D8(s32 arg0, s32 arg1);
void func_800C8A40(s32 arg0);
NAS *func_800C8A80(s32 x, s32 z, s32 range);

/* Object func_800C7E68/func_800C7EB0 set up alongside a hero action. */
typedef struct Ovl5fActObj {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x50 - 0xC];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl5fActObj;
/* InitFlamerBitTA's 0x38-byte extension (id 0x10000000). */
typedef struct FlamerBitExt {
    /* 0x00 */ EXT ext;
    /* 0x0C */ u8 pad0C[0x1C];
    /* 0x28 */ s32 timer;
    /* 0x2C */ u8 pad2C[0xC];
} FlamerBitExt;

/* InitTotemPole's 0x38-byte extension (id 0x10000000). */
typedef struct TotemPoleExt {
    /* 0x00 */ EXT ext;
    /* 0x0C */ u8 pad0C[0x1C];
    /* 0x28 */ s32 timer;
    /* 0x2C */ u8 pad2C[0x4];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0x4];
} TotemPoleExt;
extern NAS *D_800CB218;
extern NAS *D_800CB21C;
extern NAS *D_800CB220;

extern u8 D_8005365C[];
extern u8 D_800537AC[];
extern s32 D_800C9B94[][2];

/* Extension 0x10000000 of the totem NAS func_800C0C68 sets up. */
typedef struct TotemExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;  /* turn per frame */
    /* 0x10 */ s32 unk10; /* velocity x */
    /* 0x14 */ s32 unk14; /* velocity y */
    /* 0x18 */ s32 unk18; /* velocity z */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 timer;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 *anim; /* animation script: frames, 0 ends, -1 is an event */
} TotemExt;
extern s32 D_800C9C14[];
extern s32 D_800C9C94[];
extern s32 D_800C9CE4[];
extern s32 D_800C9D58[];
extern s32 D_800C9DD0[];

/* State block func_800C7E68/func_800C7EB0 set up alongside a hero action. */
typedef struct Ovl5fActState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC[17];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl5fActState;

extern s32 D_800CA160[];
extern s32 D_800CA34C[];
extern struct { u16 v, pad; } D_800CA4AC[];
extern u8 D_800CACFC[];
extern s32 D_800CAFF4;
extern u8 D_800CACB4[];
extern u8 D_800CAFFC[];
typedef struct {
    u8 pad0[0x74];
    s16 unk74;
} Ovl5fUnk74;

extern Ovl5fUnk74 *D_8009D60C;
extern Ovl5fUnk74 *D_8009D678;

/* Extension 0x10000000 of the NAS 0x1605 object func_800C5620 spawns. */
typedef struct Ovl5fExt1605 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} Ovl5fExt1605;
typedef struct MineOffset {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 z;
    /* 0x08 */ s32 unk8;
} MineOffset;
extern MineOffset D_800C935C[4];
void func_800C4D04(s32 hero, s32 last_mode);
s32 func_8008502C();
extern s32 *D_800CB24C;
extern s32 *D_800CB250;
extern s32 D_800CAA0C[];
extern s32 D_800CAB58[];
extern s32 D_800CAA60[];
extern s32 D_800CABAC[];
extern s32 D_800CAADC[];
extern s32 D_800CAC38[];
extern s32 D_800CB0FC;

typedef struct Ovl5fLink {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x5C - 0x4];
    /* 0x5C */ struct Ovl5fLink *next;
} Ovl5fLink;

typedef struct {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[0x5E - 0x20];
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ Ovl5fLink *list;
} Ovl5fObj;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} Ovl5fPos;

typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ Ovl5fPos *pos;
} Ovl5fHolder;

/* PlotLaserBitTA's NAS.FUX. */
typedef struct LaserBitFux {
    /* 0x00 */ u8 unk0[0x2C];
    /* 0x2C */ s32 player;
    /* 0x30 */ s32 unk30;
} LaserBitFux;

/* func_800BAFFC's view of a shot's NAS.FUX. */
typedef struct Ovl5fShotFux {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 vx;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 vz;
    /* 0x1C */ u8 pad1C[0x28 - 0x1C];
    /* 0x28 */ s32 life;
    /* 0x2C */ s32 player;
    /* 0x30 */ s32 unk30;
} Ovl5fShotFux;

s32 func_800BAFFC(NAS *nas, s32 arg1, s32 arg2, s32 arg3);

/* PlotShell's NAS.FUX. */
typedef struct ShellFux {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 vy;
    /* 0x18 */ u8 unk18[0x30 - 0x18];
    /* 0x30 */ s32 unk30;
} ShellFux;

/* PlotFlubberTA's NAS.FUX. */
typedef struct FlubberFux {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 vy;
    /* 0x18 */ u8 unk18[0x2C - 0x18];
    /* 0x2C */ s32 player;
    /* 0x30 */ s32 unk30;
} FlubberFux;

/* PlotLobber_TA's NAS.FUX. */
typedef struct LobberFux {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 vx;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 vz;
    /* 0x1C */ u8 unk1C[0x2C - 0x1C];
    /* 0x2C */ s32 player;
    /* 0x30 */ s32 unk30;
} LobberFux;

/* InitPickupTA's extension 0x10000000 (0x38 bytes), its NAS.FUX. */
typedef struct PickupTAExt {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x38 - 0x18];
} PickupTAExt;
/* The 0x38-byte extension (id 0x10000000) of the shell PlotPeriscopeGun fires. */
typedef struct PeriscopeShellExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 vx;
    /* 0x14 */ s32 vy;
    /* 0x18 */ s32 vz;
    /* 0x1C */ u8 pad1C[0x28 - 0x1C];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[4];
} PeriscopeShellExt;

typedef struct {
    s32 x;
    s32 z;  /* negative: unused */
} Ovl5fPoint;

extern Ovl5fPoint D_800CA9E4[];
extern s32 D_800CB0F0;
extern s32 D_800CB260;
s32 get_angle(s32 x, s32 z);

/* func_800C2940: can the turret at (x, z) facing ang see target i? Records state. */
static inline s32 Ovl5fTurretSees(s32 i, s32 state, s32 x, s32 z, s32 ang) {
    if (D_800CB0F0 != 0) {
        return 0;
    }
    if (D_800CA9E4[i].z < 0) {
        return 0;
    }
    if ((u32)(((ang - get_angle(x - D_800CA9E4[i].x, z - D_800CA9E4[i].z)) & 0xFFF) - 0x15) < 0xFD7U) {
        return 0;
    }
    D_800CB260 = state;
    return 1;
}

extern Ovl5fPoint D_800CA9E4[];
typedef struct {
    u16 v, pad;
} Ovl5fU16Pad;
extern Ovl5fU16Pad D_800CA944[];
extern u16 D_800CA994[]; /* crash turret anim frames (flags == 0) */
extern u16 D_800CA9BC[]; /* crash turret anim frames (flags != 0) */
typedef void (*Ovl5fTurretFireFn)(EXTengine *eng, s32 a, s32 b);
extern s32 D_800CB0F0;
extern s32 D_800C94E8[];
extern s32 D_800C9428[][4];
extern NAS *D_800CAD44[];
/* func_800B4688's fallback roam points: {x, z}. */
extern s32 D_800CA17C[][2];
/* PlotHelicopterTA's NAS.FUX. */
typedef struct HeliFux {
    /* 0x00 */ u8 unk0[0x10];
    /* 0x10 */ s32 vx;
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ s32 unk30;
} HeliFux;
extern Ovl5fU16Pad D_800CA650[];    /* PlotHelicopterTA's rotor frames, by UserInfo.user2 */
/* PlotPillarControl's waves: delay, then {x, z, angle} until x < 0. */
extern s32 *D_800CA8C4[];
extern s32 D_800CB204;      /* next wave */
extern s32 D_800CB260;
extern NAS *D_800CB264[8];    /* objects func_800C8598 adds and func_800C8560 removes; func_800C84D8 tests their distance */
extern NAS *D_800CB0C0[8];

/* InitJoe/InitMoe's 0x38-byte extension (id 0x10000000), their NAS.FUX. */
typedef struct JoeExt {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[4];
} JoeExt;
extern u8 D_80053704[];
extern NAS *D_800CB208;       /* InitTankBoss's NAS */
extern EXTengine *D_800CB20C; /* and its engine */
extern s32 D_800CB248;
s32 func_800C2940(EXTengine *eng);
void func_800C3778(EXTengine *eng);
void func_800C3D48(NAS *nas, EXTengine *eng);
/* Field 0xC of the 0x38-byte model record N at D_80052A54 (D_800536CC is record 57). */
#define TANK_MODEL(n) (&D_80052A54[(n) * 0x38])
#define TANK_MODEL_ON(b, n) (*(s32 *)((b) + (n) * 0x38 + 0xC))
extern s32 D_800CA954[16]; /* PlotTankBoss: per-frame height wobble */
extern s8 D_8005A6C8;









void func_800C5620(s32 arg0);

void func_800C69C4(s32 arg0, s32 *arg1);

s32 func_800C6B9C(s32 id, s32 x, s32 z, s32 angle);



s32 func_800C7D44(s32 flags, Ovl5fHolder *a, Ovl5fPos *b);

void func_800C7E68(HeroInfo *hero, Ovl5fActObj *obj, s32 arg2);

void func_800C7EB0(HeroInfo *hero, Ovl5fActState *state, s32 arg2);


void SetHullSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX);

void func_800C81C8(s32 x, s32 z, s32 range, s32 arg3);

void func_800C8450(s32 x, s32 z, s32 dist);

NAS *func_800C84D8(s32 x, s32 z, s32 dist);

void func_800C8560(NAS *nas);

void func_800C8598(NAS *nas);

void func_800C85D0(void);

s32 func_800C8870(s32 arg0, s32 arg1);

void func_800C88A8(NAS *nas);

void func_800C8BA0(void);

extern u8 D_800C9F5C[];
extern u8 D_800C9F70[];
extern u8 D_800C9F44[];
extern u8 D_800C9F24[];
extern u8 D_800C9F10[];
extern u8 D_800C9F38[];

extern u8 D_800C9FD0[];
extern u8 D_800C9FE4[];
extern u8 D_800C9FB8[];
extern u8 D_800C9F98[];
extern u8 D_800C9F84[];
extern u8 D_800C9FAC[];
extern u8 D_800CA120[];
extern u8 D_800CA134[];
extern u8 D_800CA148[];
extern u8 D_800CA08C[];
extern u8 D_800C9FF8[];
/* D_8005A614 as seen by func_800B5588. */
typedef struct Ovl5fGameView {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u32 w4;
    /* 0x08 */ u8 pad8[0x32 - 8];
    /* 0x32 */ u16 flags;
    /* 0x34 */ u8 pad34[0x51 - 0x34];
    /* 0x51 */ u8 b51;
    /* 0x52 */ u8 b52;
    /* 0x53 */ u8 b53;
    /* 0x54 */ u8 pad54[0x6C - 0x54];
    /* 0x6C */ s32 w6C;
} Ovl5fGameView;

extern s32 D_800C94A8[];
extern s32 D_800C94C8[]; /* TA_MoveHero: hull sfx while moving, by player */
extern s32 D_800C94D8[]; /* turret sfx, by player */
extern s32 D_800C9408[]; /* hull sfx in mode 5, by player */
extern s32 D_800CB0F8;    /* delay before the next hangabout spawn */
extern s32 D_800CB0E0;    /* func_800B5FC4: countdown, plays sfx 0x4B9 when negative */
extern s32 D_800CB0E4;    /* func_800B5FC4: sfx id to play once */
extern EXTengine *D_800CB0E8;
extern s32 D_800CB0EC;    /* func_800B5FC4: countdown to sfx 0x501 */
void func_800B71F8(s32 player);

typedef struct OvlClampPos {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 x;
    /* 0x14 */ s32 pad14;
    /* 0x18 */ s32 z;
} OvlClampPos;

extern s32 D_800CB100;
extern s32 D_800CB104;











void func_800C0DA0(EXTengine *arg0);

void func_800C1F44(void);

void func_800C4320(NAS *nas, s32 id);

void func_800C44E8(s32 arg0);

void func_800C49F8(s32 arg0);

void func_800C4B60(s32 arg0);

void func_800C68C8(s32 arg0);


s32 func_800C6A30(s32 arg0);

s32 func_800C6C30(void);

s32 func_800C6C90(OvlClampPos *p, s32 x, s32 y, s32 z);

void func_800C7298(NAS *nas);

void func_800C7F18(NAS *nas, s32 player);


void SetTurretSFX(s32 player, EXTengine *unused, s32 ReqdSFX);

s32 func_800C8120(s32 arg0);



void func_800C8360(void);

void func_800C83D8(void);

void func_800C85F8(Ovl5fHolder *h, s16 v);

s32 func_800C8650(s32 x, s32 z, s32 r, s32 skip);

s32 func_800C872C(s32 arg0, s32 arg1, s32 arg2);

void func_800C8800(s32 arg0, s32 arg1);

void func_800C8AF8(s32 player, EXTengine *eng, s32 *arg2);

s32 func_800B3B24();

/* AI target state func_800B4490 fills (target NAS, scan cursor). */
typedef struct Ovl5fAITarget {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ NAS *target;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 scan;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 flags;
    /* 0x20 */ s32 x;
    /* 0x24 */ s32 z;
    /* 0x28 */ u16 timer;
    /* 0x2A */ u16 unk2A;
} Ovl5fAITarget;
extern s32 D_800CB284[];
s32 func_800B3F44(s32 player, EXTengine *eng, Ovl5fAITarget *ai);
void func_800B4688(s32 player, EXTengine *eng, Ovl5fAITarget *ai);
void func_800B4A28(s32 player, EXTengine *eng, Ovl5fAITarget *ai);
static inline s32 Ovl5fASTIndex(s32 id) {
    s32 i;

    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == id) {
            return i;
        }
    }
    return 0;
}
/* PlotRocketTA: func_800C84D8 inlined. */
static inline NAS *Ovl5fRocketNear(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = D_800CB264[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    return nas;
                }
            }
        }
    }
    return NULL;
}
extern HeroInfo Heroes[4];
/* PlotRocketTA: func_800C872C inlined. */
static inline s32 Ovl5fRocketFindHero(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = arg0 - e->unk10.x;
        if (d < -arg2) {
            continue;
        }
        if (arg2 < d) {
            continue;
        }
        d = arg1 - e->unk10.z;
        if (-arg2 < d && d < arg2) {
            return i;
        }
    }
    return -1;
}
/* PlotRocketTA: func_800C8800 inlined. */
static inline void Ovl5fRocketKillCell(s32 arg0, s32 arg1) {
    s32 idx;
    NAS *nas;

    idx = ((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9);
    nas = (NAS *)D_800CA34C[idx];
    D_800CA34C[idx] = 0;
    D_800CACB4[idx] = 0;
    D_800CACFC[idx] = 0;
    KillNAS(nas);
}
/* PlotRocketTA: the D_800CACFC tile under (x, z), 0 off the map in mode 9. */
static inline s32 Ovl5fRocketTile(s32 x, s32 z) {
    if (D_8005A648 == 9) {
        if (z >= D_800CB104 || z < 0 || x >= D_800CB100 || x < 0) {
            return 0;
        }
    }
    return *(((z >> 9) * D_800CAFF4) + (x >> 9) + D_800CACFC);
}
/* func_800C4DCC: the D_800CACB4 cell under (x, z). */
static inline s32 Ovl5fMapCell(s32 x, s32 z) {
    return *(((z >> 9) * D_800CAFF4) + (x >> 9) + D_800CACB4);
}
/* TankHeroMove: the other hero within lim of (x, z), or -1. */
static inline s32 TankMoveFindHero(s32 player, s32 x, s32 z, s32 lim) {
    s32 i;
    s32 d;
    s32 n;
    u16 name;
    HeroInfo *h;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -lim) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -lim;
        if (n < d && d < lim) {
            return i;
        }
    }
    return -1;
}
static inline s32 Ovl5fNear(s32 a, s32 b) {
    s32 d;

    d = a - b;
    return d >= -999 && d <= 999;
}
static inline void Ovl5fSpawnBit1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = D_8005365C;
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = kind;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}
static inline NAS *Ovl5fSpawnBit1605Nas(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = D_8005365C;
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = kind;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}
/* PlotPeriscope: a 0x1605 puff with the scene's first model. */
static inline NAS *Ovl5fPeriscopePuff(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = *(u8 **)D_80058AD0;
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = kind;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
        return nas;
    }
    return NULL;
}
extern NAS *D_800CB214;  /* PlotPeriscope's last puff */
extern s32 D_800CA6EC[]; /* PlotPeriscope: frame by timer in state 10, 0 ends */
/* func_800C3D48: per-shell angle offsets. */
static inline s32 *Ovl5fShellAngleOffsets(void) {
    return D_800C93AC;
}
/* func_800C3D48: face a spawned puff along `angle` and flag its subengines. */
static inline void Ovl5fPuffSetAngle(NAS *puff, s32 angle) {
    subengine *s;

    puff->Engine->unk1E = angle;
    puff->Engine->unk5E = 1;
    for (s = puff->Engine->sub; s != NULL; s = s->next) {
        s->flags |= 0x20000000;
    }
}
extern s32 D_80051244[]; /* flags by angle octant */
void func_800B4240(s32 player, s32 x, s32 z, EXTengine *eng, Ovl5fAITarget *ai);
s32 func_800B4490(s32 player, EXTengine *eng, Ovl5fAITarget *ai);

void func_800B4FAC(s32 player);

void func_800C4BC0(s32 arg0);

void func_800C53B0(s32 arg0);

/* Extension 0x10000000 of the TA helicopter (InitHelicopterTA). */
typedef struct HelicopterTAExt {
    /* 0x00 */ u8 pad[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x30 - 0x14];
    /* 0x30 */ s32 unk30;
} HelicopterTAExt;
extern s32 D_800CB0F4;
extern s32 D_800CA600[4];
extern s32 D_800CA610[4];
extern s32 D_800CA620[4];
extern s32 D_800CA630[4];
extern s32 D_800CA640[4];
extern s32 D_800CA4B8[]; /* (x, z) pairs */
/* PlotPickupGenerator_TA: hero within 0x200 of (x, z). */
static inline s32 PickupFindHero(s32 x, s32 z) {
    s32 i;
    s32 d;
    s32 r;
    s32 n;
    u16 name;
    EXTengine *e;

    r = 0x200;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -0x200) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x200;
        if (n < d && d < 0x200) {
            return i;
        }
    }
    return -1;
}

void func_800B6354(s32 player, s32 damage);


void func_800C30C0(EXTengine *arg0);

void func_800C4548(s32 player);

void func_800C57B0(NAS *nas, s32 id);

/* SetHullSFX inlined by func_800C5BCC. */
#define OVL5F_SET_HULL_SFX(player, pEngine, ReqdSFX)                        \
    {                                                                       \
        s32 sfx_ = (ReqdSFX);                                               \
        TA_player *tb_ = D_800CAD50;                                        \
        TA_player *ta_ = &tb_[player];                                      \
        if (ta_->HullSFX != sfx_) {                                         \
            ta_->HullSFX = sfx_;                                            \
            if (sfx_ != 0) {                                                \
                PlaySfx(sfx_, &(pEngine)->sfx, &(pEngine)->unk10, 0x1000,   \
                        0x1E00, 0);                                         \
            } else if ((pEngine)->sfx != NULL) {                            \
                StopSfx((pEngine)->sfx);                                    \
            }                                                               \
        }                                                                   \
    }

/* SetTurretSFX inlined by func_800C5BCC. */
#define OVL5F_SET_TURRET_SFX(player, ReqdSFX)                               \
    {                                                                       \
        s32 sfx_ = (ReqdSFX);                                               \
        TA_player *tb_ = D_800CAD50;                                        \
        TA_player *ta_ = &tb_[player];                                      \
        if (ta_->TurretSFX != sfx_) {                                       \
            ta_->TurretSFX = sfx_;                                          \
            if (sfx_ == 0) {                                                \
                StopSfx(ta_->TurretSFXHandler);                             \
            } else {                                                        \
                PlaySfx(sfx_, &D_800CAD50[player].TurretSFXHandler, NULL,   \
                        0x1000, 0x1E00, 0);                                 \
            }                                                               \
        }                                                                   \
    }

void func_800B9430(s32 player);

void func_800B993C(s32 player);

void func_800C4DCC(s32 player);

void func_800C5BCC(NAS *nas, s32 id);

/* func_800B58B0: level setup */
typedef struct Ovl5fSetupView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x51 - 0x36];
    /* 0x51 */ u8 b51;
    /* 0x52 */ u8 b52;
    /* 0x53 */ u8 b53;
    /* 0x54 */ u8 pad54[0x6C - 0x54];
    /* 0x6C */ s32 w6C;
} Ovl5fSetupView;
typedef struct Ovl5fCam {
    /* 0x00 */ s32 w0;
    /* 0x04 */ s32 w4;
    /* 0x08 */ s32 w8;
    /* 0x0C */ s32 wC;
    /* 0x10 */ s32 w10;
    /* 0x14 */ s32 w14;
    /* 0x18 */ s32 w18;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 h54;
    /* 0x56 */ s16 h56;
    /* 0x58 */ s16 h58;
} Ovl5fCam;
#define CAM5F ((Ovl5fCam *)D_80051640)
typedef struct Ovl5fScreen {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 w10;
    /* 0x14 */ s32 w14;
} Ovl5fScreen;
extern s32 D_800CAFE0;
extern s32 D_800CAFE4;
extern s32 D_800CAFE8;
extern s32 D_800CAFEC;
extern s32 D_800CAFF0;
extern s32 D_800CAFF8;
extern s32 D_800CB108[9][7];
extern s16 D_8009D88E;
extern u8 D_800C8ED4[];
extern u8 D_800CA1C4[];
extern u8 D_800CA204[];
extern u8 D_800CA244[];
extern u8 D_800CA284[];
extern u8 D_800CA2C4[];
extern u8 D_800CA30C[];
extern u8 D_8005365C[];
void func_8007EB04(void);
void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2);
void func_8007B7FC(u32 a, u32 b);

/* func_800C60AC (PlotJoe/PlotMoe): the boss NAS's FUX extension. */
typedef struct JoeMoeFux {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18[0x28 - 0x18];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 *unk34; /* animation script: zero-terminated anim ids */
} JoeMoeFux;

/* A 0x38-byte model record of D_80052A54 (see TANK_MODEL). */
typedef struct ModelRec {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 on;
    /* 0x10 */ u8 unk10[0x38 - 0x10];
} ModelRec;

extern u8 D_8005373C[];
extern NAS *D_800CB228;
/* func_800C60AC: anim scripts, Moe (0x131E) and Joe */
extern s32 D_800C8F54[];
extern s32 D_800C8F68[];
extern s32 D_800C8FA8[];
extern s32 D_800C8FE8[];
extern s32 D_800C9048[];
extern s32 D_800C9080[];
extern s32 D_800C90B8[];
extern s32 D_800C911C[];
extern s32 D_800C9140[];
extern s32 D_800C9164[];
extern s32 D_800C91A4[];
extern s32 D_800C91E4[];
extern s32 D_800C9244[];
extern s32 D_800C927C[];
extern s32 D_800C92B4[];
extern s32 D_800C9318[];

void func_800C60AC(NAS *nas, s32 id);

#endif /* BASH_OVL5F_H */
