#ifndef BASH_OVL62_H
#define BASH_OVL62_H

/* Functions and data defined in the ovl62 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl62/ovl62.c */

#include "common.h"
#include "bash/nas.h"

extern u8 D_800546C4[];
extern u8 D_80052EEC[];
extern s16 D_8005A65C[2];

/* Per-character tables func_800D2044 hangs on hero_eng->sub->unk6C. */
extern u8 D_800547DC[];
extern u8 D_80054814[];
extern u8 D_8005484C[];
extern u8 D_80054884[];
extern u8 D_800548BC[];
extern u8 D_800548F4[];
extern u8 D_80054AB4[];
extern u8 D_80054AEC[];

void func_800B5AC4();
void func_800BAFA0();
void func_800BB810();
void func_800CAD0C(void);
void func_800CAD14(void);
void func_800CAD1C(void);
s32 func_800CAFF0(void);
void func_800CB124(void);
void func_800CB12C(void);
void func_800CB6B4(void);
void func_800CB6BC(void);
s32 func_800CB6C4(s32 arg0);
s32 func_800CB744(u8 *arg0, u8 *arg1);
u32 func_800CBA28(u8 *arg0);
s32 func_800CBA5C(void);
void func_800CF9EC(s32 player);
s32 func_800D2D2C();
void func_800D3170(void);
void func_800D34F0(void);
void func_800D34F8(void);
void func_800D3500(void);
void func_800D3508(void);

extern NAS *D_800D5294;
extern subengine *D_800D5298;

/* InitBombGFX's extension (0x1C bytes). */
typedef struct BombGFXExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
} BombGFXExt;

/* A scene's model object as func_800CA770 walks the list at D_80058AD0 + 0x1C. */
typedef struct SceneObj {
    /* 0x00 */ u32 flags; /* 0x8000 visible */
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x5C - 0xC];
    /* 0x5C */ struct SceneObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ GfxModel *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} SceneObj;

/* Visible scene objects func_800CA770 collects. */
extern SceneObj *D_800D6010[80];
extern char D_800B3498[];

/* Scene objects func_800B38A0 picks out by index. */
extern s32 D_8009E1F4;
extern s32 D_8009E1F8;
extern SceneObj *D_800D529C;
extern SceneObj *D_800D52A0;
extern s32 D_800D5FC4;
extern s32 D_800D5FC8;

/* D_80058AD0 as seen by func_800B38A0. */
typedef struct Ovl62Scene {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ SceneObj *objs;
} Ovl62Scene;

/* D_8005A614 as seen by func_800B38A0. */
typedef struct Ovl62ModeView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
} Ovl62ModeView;

/* D_80051640 as the camera func_800CACC0 sets up. */
typedef struct Ovl62Cam {
    /* 0x00 */ CamXYZ eye;
    /* 0x0C */ CamXYZ at;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 rotX;
    /* 0x56 */ s16 rotY;
    /* 0x58 */ s16 rotZ;
} Ovl62Cam;

/* A value func_800C5EEC eases toward its target by a fixed step. */
typedef struct Ovl62Ease {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;     /* used while target != 0 */
    /* 0x0A */ s16 stepZero; /* used while target == 0 */
    /* 0x0C */ u8 padC[0x20 - 0xC];
} Ovl62Ease;

/* The camera offsets func_800C5EEC eases and subtracts from a position. */
typedef struct Ovl62EaseCam {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Ovl62Ease x;
    /* 0x40 */ Ovl62Ease z;
} Ovl62EaseCam;

typedef struct Ext10000000 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
} Ext10000000;

/* Extension 0x10000000 of the object func_800B5910 spawns (NAS 0x1605). */
typedef struct ExtSpawn5910 {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} ExtSpawn5910;

/* Extension 0x10000000 of the smoke trail PlotSmokeTrail moves. */
typedef struct ExtSmokeTrail {
    /* 0x00 */ u8 pad[0xC];
    /* 0x0C */ s32 fadeIn;
    /* 0x10 */ s32 fadeOut;
    /* 0x14 */ s32 shrink;
    /* 0x18 */ s32 flags;
    /* 0x1C */ s32 rise;
    /* 0x20 */ s32 baseX;
    /* 0x24 */ s32 amp;
    /* 0x28 */ s32 ampDecay;
    /* 0x2C */ s32 angle;
} ExtSmokeTrail;

extern s32 D_800D4D08;

typedef struct Ext20 {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[0x8];
    /* 0x20 */ s32 val;
    /* 0x24 */ s32 state;
} Ext20;

/* PadState (D_80051338) as func_800D1C14 and func_800D307C use it: the
 * buttons at 0x50, a word at 0x54. */
typedef struct Ovl62Pad {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl62Pad;

void func_800D1C14(s32 player);

/* PadState (D_80051338) as func_800B5EBC writes it: stick angle at 0x38,
 * buttons at 0x50. */
typedef struct Ovl62PadAng {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 angle;
    /* 0x3C */ u8 pad3C[0x50 - 0x3C];
    /* 0x50 */ s32 buttons;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
} Ovl62PadAng;

/* Inlined into func_800B5EBC: the d-pad bits that steer from v toward p. */
static inline u32 Ovl62PadToward(SLONG_XYZ *p, SLONG_XYZ *v) {
    u32 flags;

    flags = 0;
    if (p->x > v->x) {
        flags = 0x80;
    }
    if (p->x < v->x) {
        flags |= 0x20;
    }
    if (p->z > v->z) {
        flags |= 0x40;
    }
    if (p->z < v->z) {
        flags |= 0x10;
    }
    return flags;
}

extern HeroInfo Heroes[4];

/* Extension 0x10000000 made by InitSimpleExplosion: a velocity per sub-engine. */
typedef struct ExtSimpleExplosion {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ SLONG_XYZ vel[20];
    /* 0x104 */ s32 unk104;
} ExtSimpleExplosion;

extern u8 D_800546C4[];

/* The object PlotBrioPack follows (its NAS.UserInfo.user1). */
typedef struct BrioAnim {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ s16 frame;
} BrioAnim;

typedef struct BrioOwner {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ EXTengine *engine;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ BrioAnim *anim;
    /* 0x20 */ u8 pad20[0x2C - 0x20];
    /* 0x2C */ u16 type;
} BrioOwner;

/* Head of a list linked through +0x20 (counted by func_800C9B5C). */
extern u8 *D_800D5454;
extern HeroInfo Heroes[4];
extern s32 D_8005A680[];

s32 func_800C9B5C(void);
s32 func_800C2ECC(EXTengine *a, s32 b, EXTengine *c, s32 d);

void func_800BFD38();

/* A gravity object's motion block (GravityObject.target). */
typedef struct GravityMotion {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 flags;
    /* 0x0E */ s16 vx;
    /* 0x10 */ s16 ax;
    /* 0x12 */ u8 pad12[0x16 - 0x12];
    /* 0x16 */ s16 rx;
    /* 0x18 */ s16 vy;
    /* 0x1A */ s16 ay;
    /* 0x1C */ u8 pad1C[0x20 - 0x1C];
    /* 0x20 */ s16 ry;
    /* 0x22 */ s16 vz;
    /* 0x24 */ s16 az;
    /* 0x26 */ u8 pad26[0x2A - 0x26];
    /* 0x2A */ s16 rz;
    /* 0x2C */ u8 pad2C[0x2E - 0x2C];
    /* 0x2E */ s16 floor;
} GravityMotion;

typedef struct GravityObject {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 target;
} GravityObject;

void PlotGravityObject(GravityObject *obj);
/* A hero's FUX extension as func_800B9714 sets it up. */
typedef struct Ovl62Fux {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ SLONG_XYZ vel;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 target;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 state;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x44 - 0x40];
    /* 0x44 */ void *func;
    /* 0x48 */ u8 pad48[0x4C - 0x48];
    /* 0x4C */ s32 flags;
    /* 0x50 */ u8 pad50[0x54 - 0x50];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x5C - 0x58];
    /* 0x5C */ s32 timer;
} Ovl62Fux;

extern s32 D_800D4E0C[];
void func_800B7520();
void func_800B767C();
void func_800B7F70();
void func_800B6FEC();
void func_800B6D74();
extern s32 D_800D5FEC[];

/* Inlined into func_800B9994. */
static inline void Ovl62StartChase(s32 player) {
    Ovl62Fux *f;
    s32 j;
    s32 n;

    f = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    for (n = j = 0; j < 4; j++) {
        if (Heroes[j].hero_status & 0x2000) {
            n++;
        }
        f->state = 4;
        f->func = func_800CB12C;
        if (n >= 3) {
            return;
        }
    }
    f->unk3C = 1;
    f->unk24 = Random(0x3C) + 5;
}
s32 func_800B9714(s32 player);

s32 func_800CD118(s32 *arg0, s32 arg1);

void func_800B9D48(s32 player);

void func_800D2EB0(s32 player);

void func_800D325C(s32 arg0);

/* Bomb maker extension (0x1C bytes, id 0x10000000). */
typedef struct BombMakerExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} BombMakerExt;

extern s32 D_800D4DEC;
extern VECTOR D_800D4DD4;
/* func_800CB56C, inlined into func_800B782C. */
static inline s32 Ovl62HasBomb(s32 player) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;
    s32 found;

    nas = D_800D5294;
    one = 1;
    found = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        do {
            if (ext != NULL && ext->unkC == one && ext->unk14 == player) {
                goto done;
            }
        } while (0);
        nas = nas->Group.Child;
        goto loop;
    }
    found = 0;
done:
    return found;
}

/* func_800CB56C as inlined into func_800B6FEC. */
static inline s32 Ovl62HasBomb2(s32 player) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;

    nas = D_800D5294;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == one && ext->unk14 == player) {
            return 1;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return 0;
}

/* func_800CB5F4, inlined into func_800B6FEC. */
static inline NAS *Ovl62FreeBomb(void) {
    NAS *nas;
    BombMakerExt *ext;

    nas = D_800D5294;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == 0) {
            return nas;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return NULL;
}

void func_800B649C(s32 player);
void func_800B6708(void);

/* D_8009D87C (declared in bash/8007ee2c.h, which this unit does not include). */
typedef struct Ovl62Odds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Ovl62Odds;

extern Ovl62Odds D_8009D87C[];
extern s32 D_800D52E0[];
extern s32 D_800D5364[];
extern s32 D_8009D908;

extern s32 D_800D4E04;
s32 func_800BFEE0(VECTOR *a, VECTOR *b, SVECTOR *c, VECTOR *out);

s32 func_800B7A58(s32 player, s32 target);

/* Roll the odds in D_800D5364 (by D_8009D87C[player].unk0) and set ext->unk14. */
static inline void Ovl62RollOdds(s32 player, Ext20 *ext) {
    s32 r;
    s32 limit;

    r = Random(0x1000);
    if (D_8009D87C[player].unk0 > 0x30) {
        D_8009D87C[player].unk0 = 0x30;
    }
    limit = D_800D5364[D_8009D87C[player].unk0] << 7;
    if (D_8005A648 == 0x12) {
        limit += 0x400;
    }
    if (r < limit) {
        ext->unk14 = 8;
    }
    if (D_8009D908 != 0) {
        ext->unk14 = 0;
    }
}

s32 func_8008CEB0(u8 *arg0, u8 *arg1);
s32 func_800B7B88(s32 arg0);
s32 func_800BCF08(s32 arg0, s32 arg1);
s32 func_800B9B6C(s32 player);
s32 func_800D05A8(s32 arg0);

/* The NAS that owns the bomb maker. */
extern NAS *D_800D52B4;

s32 func_800C9C98(u8 *arg0);

NAS *func_800CB5F4(void);

void func_800D2C38(s32 player);

void func_800D2F48(s32 player);

void func_800D3018(s32 player);

extern s32 D_800D5FEC[4];
extern s32 D_800D5FB0[4];

s32 func_800B4A1C(NAS *nas);

void func_800B5910(void);

/* One of the 16 angular sectors func_800B4260 checks. */
typedef struct Ovl62Sector {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 val;
    /* 0x08 */ s32 pad8[2];
} Ovl62Sector;

/* Read by func_800CB0A4 and func_800B4260. */
typedef struct Ovl62Link {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x6C - 0x14];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[0xBC - 0x70];
    /* 0xBC */ Ovl62Sector sectors[16];
} Ovl62Link;

typedef struct Ovl62Owner {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *unk54;
    /* 0x58 */ Ovl62Link *unk58;
} Ovl62Owner;

extern Ovl62Owner *D_800D5254;
extern s32 D_800D5FFC;
extern s32 D_8009D908;
s32 func_800B7CDC();

/* Read by func_800CAEB0. */
typedef struct Ovl62Mode {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 mode;
} Ovl62Mode;

typedef struct Ovl62Prox {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ EXTengine *eng;
    /* 0x58 */ Ovl62Mode *unk58;
} Ovl62Prox;

extern Ovl62Prox *D_800D5494;

void func_800B60B8(s32 player);

void func_800B767C(s32 player);

s32 func_800B9994(s32 player);

s32 func_800C9EC4(EXTengine *eng, s32 player);

s32 func_800C9F70(void);

void func_800CACC0(void);

s32 func_800CAF94(s32 state);

void func_800CB0A4(void);

void func_800CB134(s32 player);
void func_800B7224();
void func_800B90C0();

s32 func_800CB284(void);
s32 func_8008502C();

s32 func_800CB2C8(s32 arg0);

void func_800CB314(s32 player);

void func_800CB508(s32 player);

void func_800CB650(s32 player);

void func_800CBA64(void);

s32 func_800CBAA4(s32 arg0, s32 arg1);

void func_800CBB8C(void);

s32 func_800CD560(s32 arg0);

s32 func_800D0650(s32 player);

s32 func_800D0750(s32 player);

s32 func_800D10A0(s32 player);

void func_800D2D9C(s32 player);

void func_800D307C(s32 player);

/* A rising slot as func_800CA6E8 steps it: 16 of them at +0xB8. */
typedef struct Ovl62Riser {
    /* 0x0 */ s32 active;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 height;
    /* 0xC */ s16 base;
    /* 0xE */ s16 padE;
} Ovl62Riser;

typedef struct Ovl62RiserSet {
    /* 0x00 */ u8 pad0[0xB8];
    /* 0xB8 */ Ovl62Riser slot[16];
} Ovl62RiserSet;

/* func_800C0FE8's ring of 16 mask segments at +0xB8 of the Uka polar ext. */
typedef struct UkaMaskSeg {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 state;
    /* 0x8 */ s32 t;
    /* 0xC */ s32 angle;
} UkaMaskSeg;

typedef struct UkaMaskSet {
    /* 0x00 */ u8 pad0[0xB8];
    /* 0xB8 */ UkaMaskSeg seg[16];
} UkaMaskSet;

void func_800C0FE8(s32 i, UkaMaskSet *m);

typedef struct Ovl62RiserObj {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ s16 y;
} Ovl62RiserObj;

extern Ovl62RiserObj *D_800D6150[16];

/* Scene objects with index 0x4000, func_800C0E94 collects. */
extern SceneObj *D_800D6190[16];

void func_800CA6E8(Ovl62RiserSet *arg0);

s32 func_800CB56C(s32 arg0);
/* Walrus fish extension (id 0x10000000): a ballistic arc (func_8008C5C4's
 * Arc, declared in bash/8007ee2c.h) at 0x10. */
typedef struct WalrusFishExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 arc[0xD];
    /* 0x44 */ s32 vx; /* arc vx */
    /* 0x48 */ s32 vz; /* arc vz */
    /* 0x4C */ u8 pad4C[0x54 - 0x4C];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x64 - 0x58];
    /* 0x64 */ SLONG_XYZ to;
    /* 0x70 */ SLONG_XYZ from;
    /* 0x7C */ s32 timer;
    /* 0x80 */ s32 spin;
    /* 0x84 */ ScriptCursor *cursor;
} WalrusFishExt;

extern ScriptInit D_800D5690;
extern s32 D_800D563C;
extern u8 D_8005476C[];

s32 func_8008C5C4(s32 *from, s32 *to, void *arc, s32 *pos);

/* Big missile extension (id 0x10000000) as PlotBigMissile reads it: a ballistic
 * arc at 0xC (the same block as BigMissileExt, InitBigMissile's view). */
typedef struct BigMissileArcExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 arc[0x15];
    /* 0x60 */ SLONG_XYZ from;
    /* 0x6C */ SLONG_XYZ to;
    /* 0x78 */ s32 state;
} BigMissileArcExt;

/* Polar boss extension (id 0x10000000), as PlotBigMissile uses it. */
typedef struct PolarBossExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ u8 pad10[0x70 - 0x10];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ u8 pad74[4];
    /* 0x78 */ s32 phase;
} PolarBossExt;

extern NAS *D_800D5448;

s32 func_800B3740(s32 arg0);

s32 func_800CB178(s32 self, s32 *dist);
/* Weight death extension (0x28 bytes, id 0x10000000). */
typedef struct WeightDeathExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} WeightDeathExt;

/* Sniper missile extension (0x38 bytes, id 0x10000000). */
typedef struct SniperMissileExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x30 - 0x10];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} SniperMissileExt;

/* Polar missile extension (0x70 bytes, id 0x10000000). */
typedef struct PolarMissileExt {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x2C - 0x1C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x38 - 0x30];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ u8 pad3C[0x44 - 0x3C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ u8 pad64[0x70 - 0x64];
} PolarMissileExt;

/* Polar bomb extension (0x84 bytes, id 0x10000000). */
typedef struct PolarBombExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x30 - 0x14];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x48 - 0x38];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s32 unk74;
    /* 0x78 */ u8 pad78[0x84 - 0x78];
} PolarBombExt;

extern u8 D_80054A0C[];

/* Big missile extension (0x7C bytes, id 0x10000000). */
typedef struct BigMissileExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x24 - 0x14];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x50 - 0x28];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x60 - 0x54];
    /* 0x60 */ SLONG_XYZ pos;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ s32 unk78;
} BigMissileExt;

extern EXTengine *D_8009D530[];

/* Value easing towards a target by a step (func_800C7ED4). */
typedef struct SniperEase {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 target;
    /* 0x08 */ s16 step;
    /* 0x0A */ s16 stepZero;
    /* 0x0C */ u8 padC[0x20 - 0xC];
} SniperEase;

typedef struct SniperAim {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ SniperEase x;
    /* 0x40 */ SniperEase y;
} SniperAim;

/* Bear death extension (0x88 bytes, id 0x10000000). */
typedef struct BearDeathExt {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SLONG_XYZ to;
    /* 0x20 */ SLONG_XYZ from;
    /* 0x2C */ s32 unk2C; /* start of the arc passed to func_8008C5C4 */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x28];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ u8 pad74[0xC];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s32 unk84;
} BearDeathExt;

extern ScriptInit D_800D56D4;

void func_800CA930(EXTheromove *a, EXTheromove *b);

void func_800CAE10(s32 player, s32 target);

void func_800D2CB8(s32 arg0);

/* Per-hero speed table (declared in bash/8007ee2c.h, not included here). */
extern u16 *D_8009D6F4;
extern s32 D_800D6924[];
extern s32 D_800D6934[];
extern u8 D_80054654[];

s32 func_800CAC20(s32 i);

s32 func_800CAFF8(s32 player);

void func_800D3460(s32 arg0);
/* D_8005A614 as seen by func_800B9B6C: the two team leaders. */
typedef struct Ovl62TeamView {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ s16 team0;
    /* 0x4A */ s16 team1;
} Ovl62TeamView;

s32 func_800B9B6C(s32 hero);

void func_800CAD60(s32 player);

void func_800CB368(s32 player);

void func_800D2E00(s32 player);

extern ScriptInit D_800D5DB8;

void func_800B38A0(void);

s32 func_800B4094(s32 *arg0, s32 arg1);

s32 func_800B4260(s32 *arg0, s32 arg1);

void func_800B5570(s32 player);

void func_800B5684(s32 player);

void func_800B6258(s32 player);

s32 func_800B6EB4(s32 player, s32 target);

void func_800B782C(s32 player);

s32 func_800B9398(s32 player);

s32 func_800B94B8(s32 player);

s32 func_800B9600(s32 player);

s32 func_800B98AC(s32 player);

void func_800BD058(s32 arg0, s32 arg1, s32 arg2);

void func_800C0E94(void);

s32 func_800C5CD0(s32 *arg0, s32 arg1);

s32 func_800C5DCC(NAS *m);

void func_800C5EEC(Ovl62EaseCam *c, s32 *pos);

void func_800C7ED4(SniperAim *a, s32 *out);

void func_800C9D74(NAS *m);

void func_800CA770(void);

void InitGravityObject(NAS *m);

void func_800CAEB0(s32 player, s32 idx);

void func_800CB41C(s32 player);

void func_800CC2BC(s32 player);

void func_800CDA54(s32 player);

void func_800CFB7C(s32 player);

void func_800CFF44(s32 player);

void func_800D034C(s32 player);

void func_800D0470(s32 player);

void func_800D0808(s32 player);

void func_800D17E8(s32 player);

void func_800D2044(s32 player);

void func_800D2248(s32 arg0);

void func_800D2334(s32 player);

void func_800D2B58(s32 player);

void func_800D30C0(s32 player);

void func_800D31A0(s32 player);

s32 func_800D32CC(s32 player);

s32 func_800D3394(s32 player);

#endif /* BASH_OVL62_H */
