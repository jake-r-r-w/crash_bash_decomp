#ifndef BASH_OVL62_H
#define BASH_OVL62_H

/* Functions and data defined in the ovl62 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl62/ovl62.c */

#include "common.h"
#include "bash/nas.h"

extern void (*D_8009D714)(void);
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

/* Per-character tables func_800CD594 hangs on hero_eng->sub->unk6C. */
extern u8 D_80052E0C[];
extern u8 D_80052E44[];
extern u8 D_80052E7C[];
extern u8 D_80052EB4[];
extern u8 D_80052F24[];
extern u8 D_800549D4[];
extern u8 D_80054A44[];
extern u8 D_80054BCC[];
extern u8 D_80054C04[];
extern u8 D_80054C3C[];
extern u8 D_80054C74[];
extern u8 D_80054CAC[];
extern u8 D_80054CE4[];
extern u8 D_80054D1C[];
extern u8 D_80054D54[];

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

/* D_8005A614 as func_800B649C reads it. */
typedef struct Ovl62GameView {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 unk6C;
} Ovl62GameView;

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

/* The 0x10000000 extension of the object func_800D1188 spawns (0x160D). */
typedef struct Ovl62Thrown {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 owner;
    /* 0x18 */ SLONG_XYZ pos;
    /* 0x24 */ SLONG_XYZ start;
    /* 0x30 */ u8 pad30[0x74 - 0x30];
    /* 0x74 */ s32 unk74;
} Ovl62Thrown;

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
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x4];
    /* 0x20 */ s32 val;
    /* 0x24 */ s32 state;
    /* 0x28 */ u8 pad28[0xAC - 0x28];
    /* 0xAC */ s32 unkAC;
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
    /* 0x2E */ u16 subtype;
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
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x5C - 0x58];
    /* 0x5C */ s32 timer;
} Ovl62Fux;

extern s32 D_800D4E0C[];
extern s32 D_800D4E08;
extern s32 D_800D3BB4[];
extern s32 D_800D3C74[];
extern s32 D_800D3EDC[];
extern s32 D_800D4174[];
extern s32 D_800D4410[];
extern s32 D_800D4820[];
extern s32 D_800D4A50[];
extern s32 D_800D4B64[];
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

/* Inlined twice into func_800B7CDC. */
static inline void Ovl62Grab(s32 player) {
    Ovl62Fux *f;

    f = (Ovl62Fux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    if (f->state != 6 || f->unk24 == 0) {
        f->unk24 = 0x3C;
    }
    f->state = 6;
    f->func = func_800B7F70;
    f->unk2C = 0;
    f->unk3C = 1;
    f->unk34 |= 1;
}

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

/* func_800CB178, inlined into func_800B90C0. */
static inline s32 Ovl62Closest(s32 self, s32 *dist) {
    SLONG_XYZ pos;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        do {
            if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
                pos = h->hero_eng->unk10;
                d = func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10);
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
            h++;
            i++;
        } while (0);
        goto loop;
    }
    *dist = min;
    return best;
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

/* InitUkaPolarMask's extension (id 0x40000000, 0x1E0 bytes). */
typedef struct UkaPolarExt {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ u8 pad18[0x64 - 0x18];
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ u8 pad70[4];
    /* 0x074 */ EXTengine *eng;
    /* 0x078 */ u8 pad78[0x88 - 0x78];
    /* 0x088 */ SLONG_XYZ pos;
    /* 0x094 */ u8 pad94[0xA8 - 0x94];
    /* 0x0A8 */ NAS *bit;
    /* 0x0AC */ s32 unkAC;
    /* 0x0B0 */ NAS *mask;
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ UkaMaskSeg seg[16];
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 unk1BC;
    /* 0x1C0 */ s32 angle;
    /* 0x1C4 */ s32 unk1C4;
    /* 0x1C8 */ s32 unk1C8;
    /* 0x1CC */ s32 unk1CC;
    /* 0x1D0 */ s32 unk1D0;
    /* 0x1D4 */ s32 unk1D4;
    /* 0x1D8 */ s32 unk1D8;
    /* 0x1DC */ s32 unk1DC;
} UkaPolarExt;

/* The 0x1605 bit's extension (id 0x10000000) InitUkaPolarMask sets up. */
typedef struct UkaBitExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
} UkaBitExt;

/* InitTransmuterBeam's extension (id 0x40000000, 0x1E0 bytes). */
typedef struct TransBeamExt {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18[4];
    /* 0x028 */ s32 unk28[4];
    /* 0x038 */ s32 unk38[4];
    /* 0x048 */ s32 unk48[4];
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64;
    /* 0x068 */ s32 unk68;
    /* 0x06C */ s32 unk6C;
    /* 0x070 */ u8 pad70[4];
    /* 0x074 */ EXTengine *eng;
    /* 0x078 */ EXTengine *engs[4];
    /* 0x088 */ SLONG_XYZ pos;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ s32 unk98;
    /* 0x09C */ s32 unk9C;
    /* 0x0A0 */ s32 unkA0;
    /* 0x0A4 */ NAS *bit1;
    /* 0x0A8 */ NAS *bit2;
    /* 0x0AC */ s32 unkAC;
    /* 0x0B0 */ u8 padB0[0x1DC - 0xB0];
    /* 0x1DC */ s32 unk1DC;
} TransBeamExt;

extern s32 D_800D5474;
extern s32 D_800D5478;
extern s32 D_800D547C;
extern s32 D_800D6008;
extern s32 D_800D600C;

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

/* Polar walrus extension (0x94 bytes, id 0x10000000): arc at 0x14 like
 * WalrusFishExt's at 0x10. */
typedef struct PolarWalrusExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ s32 timer;
    /* 0x14 */ s32 arc[0xD];
    /* 0x48 */ u8 pad48[0x58 - 0x48];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x68 - 0x5C];
    /* 0x68 */ SLONG_XYZ to;
    /* 0x74 */ SLONG_XYZ from;
    /* 0x80 */ ScriptCursor *cursor;
    /* 0x84 */ u8 pad84[0x90 - 0x84];
    /* 0x90 */ s32 unk90;
} PolarWalrusExt;

/* Polar walrus second extension (0x54 bytes, id 0x20000000). */
typedef struct PolarWalrusExt2 {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x40 - 0x24];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ u8 pad50[4];
} PolarWalrusExt2;

extern ScriptInit D_800D5630;
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

/* Polar boss extension (0x94 bytes, id 0x10000000; InitPolarBoss, PlotBigMissile). */
typedef struct PolarBossExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 state;
    /* 0x10 */ ScriptCursor *cursor;
    /* 0x14 */ u8 pad14[0x34 - 0x14];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ u8 pad40[0x54 - 0x40];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ u8 pad60[0x68 - 0x60];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 ang;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 phase;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ s32 unk80;
    /* 0x84 */ NAS *unk84;
    /* 0x88 */ NAS *unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
} PolarBossExt;

extern NAS *D_800D5DC4;
extern ScriptInit D_800D5A34;
extern NAS *D_8009D670;
extern s16 D_8009D6E2;
void func_80086868(void);

extern NAS *D_800D5448;

s32 func_800B3740(s32 arg0);

s32 func_800CB178(s32 self, s32 *dist);
/* Weight death extension (0x28 bytes, id 0x10000000). */
typedef struct WeightDeathExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
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
    /* 0x64 */ SLONG_XYZ pos;
} PolarMissileExt;

extern u8 D_80054A0C[];

/* Index of the first live hero within 0x7BC4 of pos (knocked back and a
 * sound played), or -1 (PlotPolarMissile). */
static inline s32 PolarMissileHitHero(SLONG_XYZ *pos) {
    HeroInfo *h;
    u32 i;
    u32 n;

    n = 1;
    if (D_8005A680[0] != 0) {
        n = 2;
    }
    i = 0;
    while (1) {
        if (i >= n) {
            return -1;
        }
        h = &Heroes[i];
        if (!(h->hero_mode & 0x2000)
            && func_8008CEB0((u8 *)pos, (u8 *)&h->hero_eng->unk10) < 0x7BC4) {
            if (h->hero_mode != 0x4B) {
                SetHeroAction((HeroInfo *)i, 0x4B);
            }
            PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
            return i;
        }
        i++;
    }
}

/* Spawns the 0x1605 blast at m's position (PlotPolarMissile). */
static inline void PolarMissileBlast(NAS *m, s32 kind) {
    SLONG_XYZ *pos;
    u8 *anim;
    NAS *nas;
    ExtSpawn5910 *x;

    pos = &m->Engine->unk10;
    anim = D_80054A0C;
    nas = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        x = FindExtension(nas, 0x10000000);
        if (x != NULL) {
            x->unkC = kind;
            x->unk18 = 0x100;
            if (anim != NULL) {
                x->unk20 = 5;
                nas->Engine->sub->unk6C = anim;
            } else {
                x->unk20 = 1;
            }
            x->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

/* Polar bomb extension (0x84 bytes, id 0x10000000). */
typedef struct PolarBombExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ s32 to[3];   /* func_8008C5C4 target */
    /* 0x24 */ s32 from[3]; /* func_8008C5C4 start */
    /* 0x30 */ s32 unk30;   /* arc state from here */
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x48 - 0x38];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s32 unk74;
    /* 0x78 */ u8 pad78[0x84 - 0x78];
} PolarBombExt;

s32 func_800B44B4(NAS *nas, s32 dist);

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

/* Sniper extension (0x9C bytes, id 0x10000000; InitSniper). */
typedef struct SniperExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ EXTengine *unk10;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ EXTengine *unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x50 - 0x2C];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 pad5C[0x70 - 0x5C];
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ u8 pad7C[0x84 - 0x7C];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ EXTengine *unk94;
    /* 0x98 */ s32 unk98;
} SniperExt;

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

void func_800B5788(s32 arg0);

void func_800B6258(s32 player);

s32 func_800B6EB4(s32 player, s32 target);

void func_800B782C(s32 player);

s32 func_800B9398(s32 player);

s32 func_800B94B8(s32 player);

s32 func_800B9600(s32 player);

s32 func_800B98AC(s32 player);

s32 func_800BCC9C(s32 player);

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

void func_800CC024(s32 player, EXTheromove *other);

void func_800CC2BC(s32 player);

void func_800CDA54(s32 player);

void func_800CDBD0(s32 player);

void func_800CFB7C(s32 player);

void func_800CFF44(s32 player);

void func_800D034C(s32 player);

void func_800D0470(s32 player);

void func_800D0808(s32 player);

void func_800D17E8(s32 player);

void func_800D1DB4(s32 player);

void func_800D2044(s32 player);

void func_800D2248(s32 arg0);

void func_800D2334(s32 player);

void func_800D2B58(s32 player);

void func_800D30C0(s32 player);

void func_800D31A0(s32 player);

s32 func_800D32CC(s32 player);

s32 func_800D3394(s32 player);

/* A hero's 0x20000000 extension as func_800CFC58 uses it. */
typedef struct Ovl62HitExt {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 speed;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0xA8 - 0x28];
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
} Ovl62HitExt;

void func_800CF6F8(s32 player);

void func_800CFC58(s32 player);

void func_800D0060(s32 player);

void func_800D1188(s32 player);

void func_800D14CC(s32 player);

void func_800D191C(s32 player);

/* InitBearminator's second extension (0x54 bytes, id 0x20000000). */
typedef struct BearminatorExt2 {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0x28 - 0x24];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ u8 pad2C[0x40 - 0x2C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x48 - 0x44];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ u8 pad50[4];
} BearminatorExt2;

/* InitBearminator's extension (0xB8 bytes, id 0x10000000). */
typedef struct BearminatorExt {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ ScriptCursor *cursor;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x38 - 0x20];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x50 - 0x40];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x7C - 0x54];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x8C - 0x80];
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ u8 pad90[0x98 - 0x90];
    /* 0x98 */ NAS *unk98;
    /* 0x9C */ EXTengine *unk9C;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ BearminatorExt2 *unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
} BearminatorExt;

extern ScriptInit D_800D5FA4;
/* D_8005A614 as seen by func_800B5AC4. */
typedef struct Ovl62InitView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x51 - 0x36];
    /* 0x51 */ u8 b51;
    /* 0x52 */ u8 b52;
    /* 0x53 */ u8 b53;
    /* 0x54 */ u8 pad54[0x6C - 0x54];
    /* 0x6C */ s32 w6C;
} Ovl62InitView;

/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct Ovl62Hooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ void *unk34;
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl62Hooks;
extern Ovl62Hooks D_8009D6E8;
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern s32 D_800D4D0C;
extern s32 D_800D4DD0;
extern u8 D_8008B29C[], D_8008B418[];
extern u8 D_800D5258[], D_800D526C[], D_800D5280[], D_800D52B8[];
extern u8 D_800D3510[], D_800D3524[], D_800D3538[], D_800D3544[], D_800D355C[], D_800D3570[];
extern u8 D_800D35C4[], D_800D35D8[], D_800D35EC[], D_800D35F8[], D_800D3610[], D_800D3624[];
extern u8 D_800D3678[], D_800D368C[], D_800D36A0[], D_800D36AC[], D_800D36C4[], D_800D36D8[];
extern u8 D_800D372C[], D_800D3740[], D_800D3754[], D_800D3760[], D_800D3778[], D_800D378C[];
extern u8 D_800D37E0[], D_800D3830[], D_800D3874[], D_800D388C[], D_800D38A4[];
void func_800CBBE8(s32 p1, s32 p2);

void func_800CC3B4(void);
void func_800CCCB8(void);

void func_800CD594(s32 player);

#endif /* BASH_OVL62_H */
