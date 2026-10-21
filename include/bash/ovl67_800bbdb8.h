#ifndef BASH_OVL67_800BBDB8_H
#define BASH_OVL67_800BBDB8_H

/* Functions and data defined in src/overlays/ovl67/800bbdb8.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/800bbdb8.c */

#include "common.h"
#include "bash/ovl67.h"

typedef struct FollowCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ u8 pad18[0x3C];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 pad56;
    /* 0x58 */ s16 unk58;
} FollowCam;

typedef struct SetupCam {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 z;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x1C];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[0x10];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ u8 pad5A[0xA];
    /* 0x64 */ s32 sx, sy, sz;
    /* 0x70 */ u8 pad70[0x24];
    /* 0x94 */ u8 unk94;
    /* 0x95 */ u8 unk95;
} SetupCam;

typedef struct MixObj {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ s32 a;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 b;
    /* 0x34 */ u8 pad34[0x1A];
    /* 0x4E */ s16 w;
} MixObj;

extern SetupCam D_800C7614;
extern u8 D_8009F644[];
extern u8 D_8009F6F4[];

typedef struct MineSrc {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ s32 unk18;
} MineSrc;

typedef struct MineSFX {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MineSrc *src;
    /* 0x58 */ u8 pad58[4];
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
} MineSFX;
typedef struct LaserSub {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} LaserSub;

typedef struct LaserObj {
    /* 0x00 */ u8 pad00[0x1E];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[0x3D];
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 pad5E[0x0E];
    /* 0x6C */ LaserSub *sub;
} LaserObj;

typedef struct LaserShoot {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ LaserObj *obj;
    /* 0x58 */ u8 pad58[8];
    /* 0x60 */ s32 unk60;
} LaserShoot;
typedef struct LaserPos {
    s32 x;
    s32 y;
    s32 z;
} LaserPos;

typedef struct LaserTgt {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ LaserPos pos;
} LaserTgt;

typedef struct LaserNode {
    /* 0x00 */ u8 pad00[0x20];
    /* 0x20 */ struct LaserNode *next;
    /* 0x24 */ u8 pad24[0x30];
    /* 0x54 */ LaserTgt *tgt;
} LaserNode;

typedef struct LaserExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} LaserExt;

typedef struct SpiralExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ EXTengine *eng[4];
    /* 0x1C */ s32 radius;
    /* 0x20 */ s32 ang[4];
    /* 0x30 */ SLONG_XYZ c;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ NAS *sat[4];
} SpiralExt;

extern u8 *D_800C449C;
extern NAS *D_800C4490;
extern NAS *D_800C4494;
extern s32 D_800C4478;
extern void (*D_8009F8B4)(void);
extern s32 D_800C4498;
extern s32 D_800C45F0;

typedef struct MineModel {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} MineModel;

typedef struct MineObj {
    /* 0x00 */ u8 pad00[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 pad22[0x3C];
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ u8 pad60[0xC];
    /* 0x6C */ MineModel *model;
} MineObj;

typedef struct MissileObj {
    /* 0x00 */ u8 pad00[0x5D];
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 pad5E[0xE];
    /* 0x6C */ MineModel *model;
} MissileObj;

typedef struct MissileHolder {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MissileObj *obj;
} MissileHolder;

typedef struct MissileExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} MissileExt;

typedef struct MineHolder {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ MineObj *obj;
} MineHolder;
extern s32 D_800C4480;
extern s32 D_800C4488;
extern s32 D_800C448C;
extern s32 D_800C44FC;
extern s32 D_800C4500;
extern s32 D_800C4504;
extern s32 D_800C4508;
/* PadState (D_80051338) as seen by func_800BEB68. */
typedef struct BossPad {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad04[0x50 - 0x04];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} BossPad;
extern s32 D_800C45F4[];
extern s32 D_800C4604[];
extern ScriptInit D_800C44F0;

/* Path follower of func_800C0C28: a key table and a position along it. */
typedef struct BossPath {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ KeyBlock *keys;
    /* 0x14 */ s32 pos;
} BossPath;

typedef struct BossObj {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ EXTengine *eng;
} BossObj;

/* D_8005A614 as seen by func_800C0C28. */
typedef struct BossGameView {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad08[8];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad32[0x6C - 0x32];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[5];
    /* 0x75 */ u8 unk75;
    /* 0x76 */ u8 pad76[0xCA - 0x76];
    /* 0xCA */ u8 unkCA;
} BossGameView;

s32 func_800BBDB8(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *d);

/* Mine damage by game-view level, as func_800C365C. */
static inline s32 MineDamage(BossGameView *g) {
    s32 r;

    if (g->unkCA < 10) {
        r = 230;
    } else if (g->unkCA >= 30) {
        r = 51;
    } else {
        r = 230 - (g->unkCA - 10) * 179 / 20;
    }
    return r;
}

extern SLONG_XYZ D_800C76CC;
extern SLONG_XYZ D_800C76D8;
extern s16 D_8009D6DC[];
extern s32 D_8005A624;

extern EXTengine *D_8009D530;
extern s32 D_800C4478;

void func_800C0C28(BossPath *arg0, BossObj *arg1);void func_800BC6B8(void);

void InitMineSFX(MineSFX *arg0);

void InitLaserShoot(LaserShoot *arg0);

void func_800C2904(s32 *arg0, s32 flag, s32 *out);

s32 func_800C2D10(s32 arg0);

void func_800C2D80(s32 arg0);

void func_800C2E58(void);

s32 func_800C2F70(void);

void InitDummyCamera(NAS *nas);

void func_800C3184(void);

void func_800C3234(void);

void func_800C3370(void);

void func_800C3440(void);

void func_800C36D4(void);

void func_800C36DC(void);

void PlotNitroSmokeBoost(NAS *nas);

void func_800C318C(s32 arg0);

void InitNitroSmokeBoost(NAS *nas);
extern s32 D_800C447C;

void func_800C27D4(void);

void InitNitroMine(MineHolder *arg0);

void func_800C2BB0(void);

void func_800C2C08(s32 arg0);

void func_800C2C8C(s32 arg0);

void func_800C2DD8(s32 arg0);

void func_800C2E60(s32 arg0);

void func_800C2EF8(s32 arg0);

void func_800C2F78(MixObj *arg0, MixObj *arg1);

void PlotDummyCamera(NAS *nas);

s32 func_800C365C(void);

typedef struct BoxHalf {
    s16 v[3];
} BoxHalf;

typedef struct Box2 {
    BoxHalf lo;
    BoxHalf hi;
} Box2;

typedef struct AngObj {
    /* 0x00 */ char pad0[0x10];
    /* 0x10 */ u16 ax;
    /* 0x12 */ u16 ay;
    /* 0x14 */ char pad14[0x3D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ char pad54[0x20];
    /* 0x74 */ u16 state;
} AngObj;

extern char D_800B336C[];

void func_800BD584(AngObj *o);

void InitNitroMissile(MissileHolder *arg0);

s32 func_800C323C(LaserPos p, s32 r);

s32 func_800BC7E0(s32 *arg0, LaserPos p);

/* Bursts a laser target: spawns 0x1605 at its position, kills the node. */
static inline void LaserBurst(LaserNode *n) {
    LaserExt *ext;
    NAS *nas;
    LaserPos *tp;
    s32 mode;

    do {
        tp = &n->tgt->pos;
        nas = SpawnNASById(n->tgt->pos.x, tp->y, tp->z, 0x1605, 0x11);
        mode = 3;
    } while (0);
    do {
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unk18 = 0x100;
                ext->unkC = mode;
                ext->unk20 = 1;
                ext->unk28 = 0;
                nas->InitDelete(nas);
            }
        }
    } while (0);
    KillNAS((NAS *)n);
}

/* func_800BBDB8's push: sets up the object's rotation and passes v through. */
static inline s32 BoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

/* func_800C3544's box test, inlined into func_800BC7E0. */
static inline s32 LaserInBox(s32 *arg0, s32 *arg1) {
    Box2 *src;
    s32 d[3];
    BoxHalf lo;
    BoxHalf hi;

    src = (Box2 *)func_8008CE20((subengine *)arg0, 0, 0);
    if (src == NULL) {
        printf(D_800B336C);
        return 0;
    }
    lo = src->lo;
    hi = src->hi;
    d[0] = arg1[0] - arg0[1];
    d[1] = arg1[1] - arg0[2];
    d[2] = arg1[2] - arg0[3];
    if (lo.v[0] < d[0]) {
        if (d[0] < hi.v[0]) {
            if (hi.v[1] < d[1]) {
                if (d[1] < lo.v[1]) {
                    if (lo.v[2] < d[2] + 0x400) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
typedef struct BossScene {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad04[0x28];
    /* 0x2C */ u8 *unk2C;
} BossScene;

typedef struct BossExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ ScriptCursor *cursor;
    /* 0x10 */ KeyBlock *keys;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
} BossExt;

extern s16 D_8009D6E2;
extern ScriptInit D_800C446C;
extern s32 D_800C4530;
extern s8 D_8005A6DE;
void func_80093038(void);
void func_800B4138(s32 arg0);

void func_800BDB00(void);

void func_800BE8C0(s32 arg0);

void func_800BF7C8(s32 arg0);

void func_800C0C28(BossPath *arg0, BossObj *obj);

void InitNitroBoss(NAS *nas);

/* Extension of the objects PlotNitroBoss spawns. */
typedef struct NitroShotExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} NitroShotExt;

extern s32 D_800C4484;

s32 func_800C3544(s32 *arg0, s32 *arg1);

/* Moves one EXTheromove velocity toward its max by acc (max != 0) or dec,
 * clamping at max. Uses the caller's s32 locals step and v. */
#define HERO_ACCEL(m, vel, max, acc, dec)        \
    if (m->vel != m->max) {                      \
        step = m->max != 0 ? m->acc : m->dec;    \
        if (m->vel < m->max) {                   \
            v = m->vel + step;                   \
            m->vel = v;                          \
            if (v > m->max) {                    \
                m->vel = m->max;                 \
            }                                    \
        } else {                                 \
            v = m->vel - step;                   \
            m->vel = v;                          \
            if (v < m->max) {                    \
                m->vel = m->max;                 \
            }                                    \
        }                                        \
    }

void InitSpiralSat(NAS *nas);

/* A 2-point gouraud line primitive (LINE_G2 layout). */
typedef struct SparkLine {
    /* 0x00 */ u8 tag[3];
    /* 0x03 */ u8 len;
    /* 0x04 */ u8 r0, g0, b0, code;
    /* 0x08 */ s16 x0, y0;
    /* 0x0C */ u8 r1, g1, b1, p1;
    /* 0x10 */ s16 x1, y1;
} SparkLine;

typedef struct Spark {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 pad06;
    /* 0x08 */ s16 x2;
    /* 0x0A */ s16 y2;
    /* 0x0C */ s16 z2;
    /* 0x0E */ u8 pad0E[0xA];
} Spark;

typedef struct SparkSet {
    /* 0x00 */ Spark *sparks;
    /* 0x04 */ SparkLine *lines[2];
    /* 0x0C */ s32 count;
} SparkSet;

typedef struct SparkObj {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ SparkSet set;
} SparkObj;

extern s32 D_800C45B4;
extern s32 D_800C45B8;

void func_800C2218(SparkObj *arg0);

extern MATRIX D_800C45BC;  /* spark view matrix */
extern SVECTOR D_800C76E4; /* spark view rotation */

void func_800C237C(SparkObj *arg0);

/* Asteroid roll extension (id 0x10000000): a ballistic arc (func_8008C5C4's
 * Arc, declared in bash/8007ee2c.h) at 0x0C. */
typedef struct AsteroidExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 arc[5];
    /* 0x20 */ s32 vy;    /* arc vy */
    /* 0x24 */ s32 floor; /* arc floor */
    /* 0x28 */ s32 arc28[6];
    /* 0x40 */ s32 vx;    /* arc vx */
    /* 0x44 */ s32 vz;    /* arc vz */
    /* 0x48 */ u8 pad48[0x50 - 0x48];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x60 - 0x54];
    /* 0x60 */ SLONG_XYZ from;
    /* 0x6C */ SLONG_XYZ to;
    /* 0x78 */ s32 bounces;
    /* 0x7C */ EXTengine *shadow;
} AsteroidExt;

extern u8 D_80052F5C[];

s32 func_8008C5C4(s32 *from, s32 *to, void *arc, s32 *pos);

void func_800BE70C(void);

void InitAsteroidRoll(NAS *m);

void PlotAsteroidRoll(NAS *nas);

void PlotMineSFX(NAS *nas);

void PlotLaserShoot(NAS *nas);

/* A scene object as func_800BC3A4 walks the list at D_80058AD0 + 0x1C. */
typedef struct AstKind {
    /* 0x0 */ u8 kind;
    /* 0x1 */ u8 mode;
} AstKind;
typedef struct AstSceneObj {
    /* 0x00 */ s32 flags;
    /* 0x04 */ SLONG_XYZ pos;
    /* 0x10 */ s16 rx;
    /* 0x12 */ s16 ry;
    /* 0x14 */ s16 rz;
    /* 0x16 */ u8 pad16[0x51 - 0x16];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct AstSceneObj *next;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ AstKind k;
    /* 0x66 */ u8 pad66[0x74 - 0x66];
    /* 0x74 */ u16 id;
} AstSceneObj;
typedef struct AstSlot {
    /* 0x00 */ SLONG_XYZ pos;
    /* 0x0C */ s32 ang;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} AstSlot;
typedef struct AstMineSlot {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 ang;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 unkC;
} AstMineSlot;
extern s32 D_800C450C;
extern AstSlot D_800C4614[];
extern AstMineSlot D_800C6E14[];

void func_800BC3A4(void);

void func_800BD6E8(s32 *arg0, s32 arg1);

void func_800BF37C(s32 arg0);

void PlotNitroBoss(NAS *nas);

extern HeroInfo Heroes[4];

/* func_800BEB68's tail: steer the hero's pitch and yaw toward their targets. */
static inline void BossHeroSteer(s32 i) {
    s32 *tb = D_800C4604;
    s32 *t = &tb[i];
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[i];
    EXTengine *e = h->hero_eng;

    e->unk1C += ((*t - e->unk1C) << 20) >> 23;
    h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
    h->hero_eng->unk5E = 1;
}

#endif /* BASH_OVL67_800BBDB8_H */
