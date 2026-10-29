#ifndef BASH_N2O_NOX_H
#define BASH_N2O_NOX_H

/* Functions and data defined in src/overlays/n2o/nox.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/n2o/nox.c */

#include "common.h"
#include "bash/n2o_ovl.h"

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

extern SetupCam NoxCamera;

typedef struct MineSrc {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ s32 unk18;
} MineSrc;

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
    /* 0x3C */ s32 flag;  /* tested for zero */
    /* 0x40 */ s32 angle;
    /* 0x44 */ NAS *sat[4];
} SpiralExt;

extern u8 *NoxLasers;
extern NAS *NoxCameraNas;
extern NAS *NoxCameraNasB;
extern s32 NoxSpeedScale;
extern void (*RenderHook)(void);

typedef struct MineModel {
    /* 0x00 */ u8 pad00[0x6C];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} MineModel;


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
    /* 0x54 */ EXTengine *obj;
} MineHolder;
extern s32 NoxBossHits;
extern s32 NoxHeroesDown;
extern s32 NoxHeroesAlive;
extern s32 NoxHeroTiltA;
extern s32 NoxHeroTiltB;
extern s32 NoxHeroTiltC;
extern s32 NoxHeroTiltD;
extern s32 NoxHeroBoost[];
extern s32 NoxHeroSteer[];
extern ScriptInit NoxSmokeScriptInit;

/* Path follower of NoxBossFollowPath: a key table and a position along it. */
typedef struct BossPath {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ KeyBlock *keys;
    /* 0x14 */ s32 pos;
} BossPath;

typedef struct BossObj {
    /* 0x00 */ u8 pad00[0x54];
    /* 0x54 */ EXTengine *eng;
} BossObj;


s32 NoxBoxToSphere(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *d);

/* Mine damage by game-view level, as NoxFadeLevel. */
static inline s32 MineDamage(GameState *g) {
    s32 r;

    if (g->adventure.modeSkill[31] < 10) {
        r = 230;
    } else if (g->adventure.modeSkill[31] >= 30) {
        r = 51;
    } else {
        r = 230 - (g->adventure.modeSkill[31] - 10) * 179 / 20;
    }
    return r;
}

extern SLONG_XYZ NoxHeroStartA;
extern SLONG_XYZ NoxHeroStartB;


void NoxAsteroidSfx(void);

void InitMineSFX(NAS *arg0);

void InitLaserShoot(LaserShoot *arg0);

void NoxPathAngles(s32 *arg0, s32 flag, s32 *out);

s32 TestNoxHeroWon(s32 arg0);

void NoxHeroHit(s32 arg0);

void NoxHeroDead(void);

s32 Nox_KeepInBounds(void);

void InitDummyCamera(NAS *nas);

void NoxStubA(void);

void NoxStubB(void);

void NoxMineSfx(void);


void NoxUpdateShadow(void);

void NoxInitShadow(void);

void PlotNitroSmokeBoost(NAS *nas);

void NoxInitCamera(s32 arg0);

void InitNitroSmokeBoost(NAS *nas);
extern s32 NoxSmokeTimer;

void NoxCameraFollow(void);

void InitNitroMine(MineHolder *arg0);

void NoxSpawnSmokeBoost(void);

void NoxHeroWinner(s32 arg0);

void SetNoxHeroWinner(s32 arg0);

void SetNoxHeroHit(s32 arg0);

void SetNoxHeroFly(s32 arg0);

void NoxHeroFacing(s32 arg0);

void NoxMixVelocities(MixObj *arg0, MixObj *arg1);

void PlotDummyCamera(NAS *nas);

s32 NoxFadeLevel(void);

typedef struct BoxHalf {
    s16 v[3];
} BoxHalf;

typedef struct Box2 {
    BoxHalf lo;
    BoxHalf hi;
} Box2;


extern const char NoxNoCollBoxMsg[];

void NoxTurnAsteroid(subengine *o);

void InitNitroMissile(MissileHolder *arg0);

s32 NoxLaserHitSphere(LaserPos p, s32 r);

s32 NoxLaserHitBox(s32 *arg0, LaserPos p);

/* Bursts a laser target: spawns 0x1605 at its position, kills the node. */
static inline void LaserBurst(LaserNode *n) {
    LaserExt *ext;
    NAS *nas;
    LaserPos *tp;
    s32 mode;

    do {
        tp = &n->tgt->pos;
        nas = CreateNAS(n->tgt->pos.x, tp->y, tp->z, 0x1605, 0x11);
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

/* Inline copy of NoxLaserHitSphere (defined after PlotNitroMine): bursts the
 * first laser target whose squared distance to p is under r. */
static inline s32 MineHitLaser(LaserPos p, s32 r) {
    LaserNode *n;
    LaserTgt *t;
    LaserExt *ext;
    NAS *nas;
    s32 dx, dy, dz;

    LaserPos *tp;
    s32 mode;

    n = (LaserNode *)NoxLasers;
    for (;;) {
        s32 t_;
        if (n == NULL) {
            break;
        }
        t = n->tgt;
        if (t != NULL) {
            dx = t->pos.x - p.x;
            dy = t->pos.y - p.y;
            dz = t->pos.z - p.z;
            if (dx * dx + dy * dy + dz * dz < r) {
                tp = &t->pos;
                nas = CreateNAS(t->pos.x, tp->y, tp->z, 0x1605, 0x11);
                mode = 3;
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
                KillNAS((NAS *)n);
                return 1;
            }
        }
        n = n->next;
    }
    return 0;
}

/* Spawns the 0x1605 burst at pos with mode 1. */
static inline void MineBurst(NAS *e) {
    LaserExt *ext;
    NAS *nas;
    SLONG_XYZ *tp;
    s32 mode;

    tp = &e->Engine->XYZ;
    nas = CreateNAS(e->Engine->XYZ.x, tp->y, tp->z, 0x1605, 0x11);
    mode = 1;
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = mode;
            ext->unk18 = 0x100;
            ext->unk20 = 1;
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

/* PlotNitroMine's explosion sound. */
static inline void MineSfx(void) {
    s32 r;

    r = Random(4);
    if (r == 0) {
        PlaySfx(0x3F1, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 1) {
        PlaySfx(0x3F2, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 2) {
        PlaySfx(0x3F3, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 3) {
        PlaySfx(0x3F4, 0, 0, 0x1000, 0x1E00, 0);
    }
}

/* MineSfx with the last PlaySfx called as void: keeps the copy apart from
 * MineSfx's, which cross-jumping would otherwise merge. */
static inline void MineSfxV(void) {
    s32 r;

    r = Random(4);
    if (r == 0) {
        PlaySfx(0x3F1, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 1) {
        PlaySfx(0x3F2, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 2) {
        PlaySfx(0x3F3, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 3) {
        ((void (*)(s32, _sfx_handler **, SLONG_XYZ *, s32, s32, s32))PlaySfx)(0x3F4, 0, 0, 0x1000, 0x1E00, 0);
    }
}

/* As MineSfxV, with the last PlaySfx returning s16. */
static inline void MineSfxH(void) {
    s32 r;

    r = Random(4);
    if (r == 0) {
        PlaySfx(0x3F1, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 1) {
        PlaySfx(0x3F2, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 2) {
        PlaySfx(0x3F3, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 3) {
        ((s16 (*)(s32, _sfx_handler **, SLONG_XYZ *, s32, s32, s32))PlaySfx)(0x3F4, 0, 0, 0x1000, 0x1E00, 0);
    }
}

/* NoxBoxToSphere's push: sets up the object's rotation and passes v through. */
static inline s32 BoxPushRot(SVECTOR *ang, u16 *a, s32 v) {
    ang->vx = a[8];
    ang->vy = a[9];
    ang->vz = a[10];
    return v;
}

/* NoxPointInBox's box test, inlined into NoxLaserHitBox. */
static inline s32 LaserInBox(s32 *arg0, s32 *arg1) {
    Box2 *src;
    s32 d[3];
    BoxHalf lo;
    BoxHalf hi;

    src = (Box2 *)SubFindFlagPoint((subengine *)arg0, 0, 0);
    if (src == NULL) {
        printf(NoxNoCollBoxMsg);
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
    /* 0x00 */ s32 pad0;
    /* 0x04 */ u8 pad04[0x28];
    /* 0x2C */ u8 *data;
} BossScene;

typedef struct BossExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ ScriptCursor *cursor;
    /* 0x10 */ KeyBlock *keys;
    /* 0x14 */ s32 frame;  /* its key */
    /* 0x18 */ s32 state;  /* switched on */
    /* 0x1C */ s32 timer;  /* counted down */
    /* 0x20 */ s32 timer2;  /* counted down */
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
} BossExt;

extern ScriptInit NoxBossScriptInit;
extern s32 NoxBossPathPos;
void N2OPlaceHero(s32 arg0);

void NoxStartPhase1(void);

void NoxHeroCollide(s32 arg0);

void SetNoxHeroDead(s32 arg0);

void NoxBossFollowPath(BossPath *arg0, BossObj *obj);

void InitNitroBoss(NAS *nas);

/* Extension of the objects PlotNitroBoss spawns. */
typedef struct NitroShotExt {
    /* 0x00 */ u8 pad00[0xC];
    /* 0x0C */ s32 state;  /* switched on */
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 player;  /* a hero's index */
    /* 0x18 */ s32 timer;  /* counted down */
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 unk28;
} NitroShotExt;

extern s32 NoxBossHitTimer;

s32 NoxPointInBox(s32 *arg0, s32 *arg1);

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

/* Spark, SparkSet and SparkObj: pause.h's (the engine's PlotSparks). */

extern s32 NoxSparkAlpha;
extern s32 NoxSparkScale;


void NoxInitSparks(SparkObj *arg0);

extern MATRIX NoxSparkMatrix;  /* spark view matrix */
extern SVECTOR NoxSparkAngles; /* spark view rotation */

void NoxDrawSparks(SparkObj *arg0);

/* Asteroid roll extension (id 0x10000000): a ballistic arc (ArcUpdate's
 * Arc, declared in bash/pause.h) at 0x0C. */
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



void NoxUpdate(void);

void InitAsteroidRoll(NAS *m);

void PlotAsteroidRoll(NAS *nas);

void PlotMineSFX(NAS *nas);

void PlotLaserShoot(NAS *nas);

/* A scene object as NoxInitAsteroids walks the list at MapScene->subs. */
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
    /* 0x16 */ u8 pad16[0x20 - 0x16];
    /* 0x20 */ s32 fade;  /* counts up 0x80 a frame; the sparks' alpha */
    /* 0x24 */ s32 fade2;
    /* 0x28 */ s32 fade3;
    /* 0x2C */ u8 pad2C[0x50 - 0x2C];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct AstSceneObj *next;
    /* 0x60 */ u8 pad60[4];
    /* 0x64 */ AstKind k;
    /* 0x66 */ u8 pad66[0x74 - 0x66];
    /* 0x74 */ u16 id;
    /* 0x76 */ s16 unk76;
} AstSceneObj;
typedef struct AstSlot {
    /* 0x00 */ SLONG_XYZ pos;
    /* 0x0C */ s32 ang;
    /* 0x10 */ s32 state;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 timer;  /* counted down */
    /* 0x1C */ s32 state2;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 radius;
} AstSlot;
typedef struct AstMineSlot {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 ang;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 flag;  /* tested for zero */
} AstMineSlot;
extern s32 NoxAsteroidTimer;
extern AstSlot NoxAsteroids[];
extern AstMineSlot NoxMines[];

void NoxInitAsteroids(void);

#define BGV ((GameState *)GameInfo)

/* Squared distance from the asteroid's position (o + 1) to q. */
static inline s32 AstDist(s32 *o, EXTengine *q) {
    s32 dx, dy, dz;

    dx = o[1] - q->XYZ.x;
    dy = o[2] - q->XYZ.y;
    dz = o[3] - q->XYZ.z;
    return dx * dx + dy * dy + dz * dz;
}

/* NoxAsteroidHitHero's 0x1605 burst at the asteroid's position (o + 1). */
static inline void AstBurst(s32 *o, s32 mode) {
    LaserExt *ext;
    NAS *nas;
    LaserPos *tp;

    tp = (LaserPos *)(o + 1);
    nas = CreateNAS(o[1], tp->y, tp->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = mode;
            ext->unk18 = 0x100;
            ext->unk20 = 1;
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

/* NoxAsteroidHitHero's asteroid hit sound. */
static inline void AstSfx(void) {
    s32 r;

    r = Random(4);
    if (r == 0) {
        PlaySfx(0x28B, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 1) {
        PlaySfx(0x28C, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 2) {
        PlaySfx(0x28D, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (r == 3) {
        PlaySfx(0x28E, 0, 0, 0x1000, 0x1E00, 0);
    }
}

/* NoxUpdateAsteroids: flags 0x40000000 and sets the fade (unk76) of an object
 * between 0x5000 and 0x6000 ahead of hero engine e. */
#define AST_NEAR(o, e)                                       \
    if (o->pos.z < e->XYZ.z + 0x6000) {                    \
        o->flags |= 0x40000000;                              \
        o->unk76 = abs(e->XYZ.z - o->pos.z) - 0x5000;      \
        if (o->unk76 >= 0x1000) {                            \
            o->unk76 = 0xFFF;                                \
        }                                                    \
        if (o->unk76 < 0) {                                  \
            o->unk76 = 0xFFF;                                \
        }                                                    \
        if (o->pos.z < e->XYZ.z + 0x5000) {                \
            o->unk76 = 0;                                    \
            o->flags &= ~0x40000000;                         \
        }                                                    \
    } else {                                                 \
        o->unk76 = 0;                                        \
        o->flags &= ~0x40000000;                             \
    }

/* NoxUpdateAsteroids: spins the small asteroids (kinds 1-3) and moves the
 * others but 7 down the track. */
static inline void AstSpin(AstSceneObj *o, AstKind *k, u8 kind) {
    if (k->kind >= 1 && k->kind <= 3) {
        if (o->flags & 0x8000) {
            o->pos.z -= 0x40;
            if (k->kind == 1) {
                o->rx += 0x18;
                o->ry += 0x18;
                o->rz += 0x18;
            }
            if (k->kind == 2) {
                o->rx += 0x10;
                o->ry += 0x10;
                o->rz += 0x10;
            }
            if (k->kind == 3) {
                o->rx += 8;
                o->ry += 8;
                o->rz += 8;
            }
            o->unk52 = 1;
        }
    } else if (kind != 7 && (o->flags & 0x8000)) {
        o->pos.z -= 0x14;
    }
}

/* Runs body with e set to the engine of the hero NoxUpdateAsteroids tracks:
 * hero 0 in one-player games, else the first hero not out (status 0x2000). */
#define AST_TRACKED_HERO(e, body)                          \
    if (((GameState *)GameInfo)->teams == 0) {        \
        e = Heroes[0].hero_eng;                            \
        body                                               \
    } else if (!(Heroes[0].hero_status & 0x2000)) {        \
        e = Heroes[0].hero_eng;                            \
        body                                               \
    } else if (!(Heroes[1].hero_status & 0x2000)) {        \
        e = Heroes[1].hero_eng;                            \
        body                                               \
    }

void NoxMineHitHero(s32 *arg0, s32 arg1);

void NoxHeroFly(s32 arg0);

void PlotNitroBoss(NAS *nas);


/* NoxPreMoveHero's tail: steer the hero's pitch and yaw toward their targets. */
static inline void BossHeroSteer(s32 i) {
    s32 *tb = NoxHeroSteer;
    s32 *t = &tb[i];
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[i];
    EXTengine *e = h->hero_eng;

    e->xform.rot.vx += ((*t - e->xform.rot.vx) << 20) >> 23;
    h->hero_eng->xform.rot.vy += angle_diff(h->hero_eng->xform.rot.vy, h->hero_move->forward_angle) >> 2;
    h->hero_eng->xform.dirty = 1;
}

void NoxAsteroidHitHero(s32 *arg0, s32 arg1);

void NoxUpdateAsteroids(void);

void PlotNitroMine(NAS *nas);

void PlotNitroMissile(NAS *nas);

void PlotSpiralSat(NAS *nas);


#endif /* BASH_N2O_NOX_H */
