#ifndef BASH_OVL65_H
#define BASH_OVL65_H

/* Functions and data defined in the ovl65 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl65/ovl65.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/main.h"

s32 func_800B3760(s32 arg0);
s32 func_800B38BC(s32 arg0);
s32 func_800B33D8(s32 arg0);
s32 func_800B485C(s32 arg0, s32 arg1);

s32 func_800B4C70(s32 *arg0, s32 arg1);

typedef struct Ovl65Ext {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Ovl65Ext;

typedef struct Ovl65Chase {
    u8 pad[0xC];
    void (*fn)(); /* 0xC */
    SLONG_XYZ pos;    /* 0x10 */
    SLONG_XYZ tgt;    /* 0x1C */
    s32 unk28;
    u8 pad2[4];
    s32 unk30;
    u8 pad2b[4];
    s32 state; /* 0x38 */
    u8 pad3[4];
    s32 timer; /* 0x40 */
} Ovl65Chase;

typedef struct Ovl65DinoGem {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x6C];
    s32 unk80;
    EXTengine *unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    u8 pad94[0x20];
    s32 unkB4;
    u8 padB8[0x1C];
    s32 unkD4;
    u8 padD8[0xA];
    s16 unkE2;
} Ovl65DinoGem;
typedef struct Ovl65PadAng {
    u8 pad0[0x38];
    s32 angle;
    u8 pad3C[0x50 - 0x3C];
    s32 buttons;
    u8 pad54[4];
} Ovl65PadAng;
typedef struct Ovl65DinoPhys {
    u8 pad0[0x20];
    s32 unk20;
    u8 pad24[0x1C];
    s32 unk40;
    u8 pad44[0xA];
    s16 unk4E;
} Ovl65DinoPhys;

typedef struct Ovl65Pos {
    u8 pad[4];
    SLONG_XYZ pos; /* 0x4 */
} Ovl65Pos;

typedef struct Ovl65PosH {
    u8 pad[4];
    u16 x;
    u8 pad2[6];
    u16 z;
} Ovl65PosH;

typedef struct Ovl65EngH {
    u8 pad[0x10];
    u16 x;
    u8 pad2[6];
    u16 z;
} Ovl65EngH;

s32 func_800B49AC(s32 id);
void func_800B3538(s32 id);

/* Hero NAS extension 0x10000000 func_800B9C10 ticks: a 26-frame timer. */
typedef struct Ovl65HeroExt {
    u8 pad[0xC];
    s32 unkC;  /* 0x0C */
    u8 pad10[0xC];
    s32 timer; /* 0x1C */
} Ovl65HeroExt;

s32 func_800BA810(s32 hero);
s32 func_800BA960(s32 hero);
s32 func_800BC820(s32 hero);
typedef struct Ovl65ExtB {
    u8 pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    NAS *unk18;
} Ovl65ExtB;

typedef struct Ovl65Pad {
    u8 pad[0x50];
    s32 unk50;
    u8 pad2[4];
} Ovl65Pad;

extern NAS *D_800BDA08[];

/* func_800B9930: move speeds (normal, carrying, by D_8009D87C[id].unk0 / 2). */
extern s32 D_800BDC14;
extern s32 D_800BDC18;
extern s32 D_800BDC1C[];

void PlotDinoSmoke(NAS *m);

void func_800BC948(HeroInfo *hero);

void func_800BCA18(HeroInfo *hero);

s32 func_800BCDF0(u8 *arg0);

void func_800BCE6C(void);

/* A scene object in the list at D_80058AD0 + 0x1C (func_800B714C). */
typedef struct Ovl65Obj {
    u32 flags;             /* 0x00 */
    u8 pad4[0x58];
    struct Ovl65Obj *next; /* 0x5C */
    u8 pad60[0x14];
    u16 index;             /* 0x74 */
    s16 scale;             /* 0x76 */
    s32 r;                 /* 0x78 */
    s32 g;                 /* 0x7C */
    s32 b;                 /* 0x80 */
} Ovl65Obj;

typedef struct Ovl65Game {
    u8 pad0[0x32];
    u16 flags; /* 0x32 */
} Ovl65Game;

/* func_800B69F4: D_8005A614's team-game view. */
typedef struct Ovl65TeamView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team0;
    /* 0x4A */ s16 team1;
    /* 0x4C */ u8 unk4C[0x5C - 0x4C];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 winner;
    /* 0x61 */ u8 unk61[0x6C - 0x61];
    /* 0x6C */ s32 teams;
} Ovl65TeamView;

/* func_800B69F4: the 0x10000000 extension of the spawned 0x1605 effect. */
typedef struct Ovl65Puff {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10[0x18 - 0x10];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x20 - 0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x28 - 0x24];
    /* 0x28 */ s32 unk28;
} Ovl65Puff;

/* func_800B69F4: a puff of smoke at hero h. */
#define OVL65_PUFF(h)                                                       \
    {                                                                       \
        NAS *nas_;                                                          \
        Ovl65Puff *p_;                                                      \
        SLONG_XYZ *v_;                                                      \
                                                                            \
        v_ = &(h)->hero_eng->unk10;                                         \
        nas_ = SpawnNASById(v_->x, v_->y, v_->z, 0x1605, 0x11);             \
        if (nas_ != NULL) {                                                 \
            p_ = FindExtension(nas_, 0x10000000);                           \
            if (p_ != NULL) {                                               \
                p_->unk18 = 0x100;                                          \
                p_->unkC = 0;                                               \
                p_->unk20 = 1;                                              \
                p_->unk28 = 0;                                              \
                nas_->InitDelete(nas_);                                     \
            }                                                               \
        }                                                                   \
    }

extern Ovl65Obj *D_800BDB54;
extern Ovl65Obj *D_800BDB7C;
extern Ovl65Obj *D_800BDB80;
extern Ovl65Obj *D_800BDB84;
extern Ovl65Obj *D_800BDC6C[3];
extern Ovl65Obj *D_800BDC78[3];

void func_800B5424();
void func_800B6FE4();
void func_800B7664();
void func_800B85DC();

void func_800BCE74(void);

void func_800BCF44(s32 arg0);

void func_800BD08C(void);

s32 func_800BD1B8(s32 *arg0, s32 arg1);

void func_800BD1FC(void);

s32 func_800BD2B8(s32 arg0);

s32 func_800BD408(s32 arg0);

s32 func_800BD488(u8 *arg0, u8 *arg1);

extern s32 D_800BD9FC;
extern NAS *D_800BDA00;
extern NAS *D_800BDA04;
extern s32 D_800BDBA8;
extern s32 D_800BDBA4;
extern s32 D_800BDBAC;

void func_800B7300(void);
void func_800B7AA0(void);

s32 func_800B475C(s32 a, s32 b);

void func_800B9C10(s32 id);

void func_800BC5B0(void);

s32 func_800BC774(s32 arg0);

void func_800BC8D0(s32 arg0);

void func_800BCA38(s32 arg0);

void func_800BCB4C(s32 arg0);

void func_800BCE7C(s32 arg0, s32 arg1);

void func_800BCEFC(void);

void func_800BCF88(s32 arg0);

s32 func_800BD01C(s32 id);

s32 func_800BD258(s32 arg0);

s32 func_800BD320(s32 arg0, s32 arg1);
extern ScriptInit D_800BDC08;

s32 func_800B33D8(s32 arg0);

void func_800B3C1C(s32 id);

void func_800B3D90(s32 id);

s32 func_800B4474(s32 *a, s32 *b, s16 *c, s32 *d);

void func_800B4B14(s32 id);
void func_800B3A38(s32 id);
void func_800B3C1C(s32 id);
void func_800B3D90(s32 id);
void func_800B3FDC(s32 id);
void func_800B4D6C(s32 id);

void func_800B714C(void);

s32 func_800B86C8(s32 arg0);

void func_800B9534(s32 arg0, s32 arg1);

void func_800B96A0(s32 arg0);

void func_800BA468(s32 id);

void func_800BA574(s32 id);

void func_800BA724(s32 id);

s32 func_800BAD18(NAS *m);

void InitDinoGem(NAS *m);

void PlotDinoScratch(NAS *m);

void InitDinoScratch(NAS *m);

s32 func_800BC684(s32 *p, s32 arg1);

s32 func_800BC878(s32 arg0);

void func_800BCAC0(s32 arg0);

void func_800BCBB8(EXTheromove *a, EXTheromove *b);

s32 func_800BCCE8(s32 arg0);

void InitDinoSmoke(NAS *arg0);

s32 func_800BD0AC(s32 self, s32 *dist);

s32 func_800BD204(s32 arg0);

extern s32 D_800BDB58[];
extern u16 D_800BDB78;

typedef struct Ovl65BoxHalf {
    s16 v[3];
} Ovl65BoxHalf;

typedef struct Ovl65Box2 {
    Ovl65BoxHalf lo;
    Ovl65BoxHalf hi;
} Ovl65Box2;

s32 csqrt(s32 x);

/* XZBoxToSphere's push: sets up the object's rotation and passes v through. */
static inline s32 Ovl65BoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

/* Hero NAS extension 0x20000000: the AI chase state func_800B4D6C runs. */
typedef struct Ovl65AiExt {
    u8 pad[0xC];
    void (*func)(); /* 0x0C */
    SLONG_XYZ pos;    /* 0x10 */
    SLONG_XYZ target; /* 0x1C */
    s32 unk28;        /* 0x28 */
    s32 victim;       /* 0x2C: nearest other hero */
    s32 unk30;
    s32 unk34;
    s32 unk38;        /* 0x38 */
    s32 angle;        /* 0x3C */
    s32 timer;        /* 0x40 */
} Ovl65AiExt;

void func_800B5DCC(s32 arg0, u16 arg1);

void func_800BAE94(NAS *arg0);

/* plays one of four sfx at random (r: the caller's s32) */
#define SFX4(a, b, c, d)                                    \
    r = Random(4);                                          \
    if (r == 0) {                                           \
        PlaySfx(a, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 1) {                                           \
        PlaySfx(b, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 2) {                                           \
        PlaySfx(c, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }                                                       \
    if (r == 3) {                                           \
        PlaySfx(d, NULL, NULL, 0x1000, 0x1E00, 0);          \
    }

void func_800B62D8(s32 arg0, u16 arg1);

s32 XZBoxToSphere(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *e);

void func_800B9E68(s32 id);

#endif /* BASH_OVL65_H */

extern u8 D_800BDA7C[], D_800BDA5C[], D_8008B29C[], D_8008B418[];
extern u8 D_800BD948[], D_800BD95C[], D_800BD970[], D_800BD97C[], D_800BD994[], D_800BD9A8[];
extern Ovl65Obj *D_800BDB88, *D_800BDB8C;
extern s32 D_800BDB90, D_800BDB94, D_800BDB98, D_800BDB9C, D_800BDBA0;
void func_800B69F4(s32 arg0, u16 arg1);
void func_800B9930();
void func_800B9778();
void func_800B96A0();

typedef struct Ovl65Vec {
    s32 vx, vy, vz;
} Ovl65Vec;

/* A wall segment of a scene collision object (func_800B9020). */
typedef struct Ovl65Seg {
    Ovl65Vec p0; /* 0x0 */
    Ovl65Vec p1; /* 0xC */
    s32 len;      /* 0x18 */
    s32 ang;      /* 0x1C */
    u8 pad20[0x18];
} Ovl65Seg;

typedef struct Ovl65SegObj {
    u8 pad0[0x38];
    s32 count; /* 0x38 */
    u8 pad3C[0xC];
    Ovl65Seg segs[1]; /* 0x48 */
} Ovl65SegObj;

typedef struct Ovl65SceneData {
    u8 pad0[0x14];
    s32 off14;
} Ovl65SceneData;

typedef struct Ovl65Scene {
    s32 unk0;
    u8 pad4[0xC];
    u8 *data; /* 0x10 */
} Ovl65Scene;

s32 func_800B9020(Ovl65Vec *pos, Ovl65Vec *out, s32 radius, s32 idx, s32 flags);
