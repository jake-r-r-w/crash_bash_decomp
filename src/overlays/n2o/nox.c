#include "common.h"
#include "bash/main.h"
#include "bash/n2o_ovl.h"
#include "bash/n2o_nox.h"
#include "bash/n2o.h"

/* rodata: the strings (const arrays keep their retail order; the empty one is the word
 * before NoxBoxToSphere's jump table, which nothing names) */
const char NoxNoCollBoxMsg[] = "No Coll box on subengine in XZBoxToSphere\n";
const char NoxEmptyString[] = "";

/* data, in address order */
s32 NoxBossSteps[3] = { (s32)0x80110000, 0x20200001, 0x8000 };
s32 NoxBossAnimList[3] = { (s32)&NoxBossSteps, 0, 0 };
ScriptInit NoxBossScriptInit = { 0, (void *)&NoxBossAnimList, 0x401 };
s32 NoxSpeedScale = 0x100;
s32 NoxSmokeTimer = 0x96;
s32 NoxBossHits = 0;
s32 NoxBossHitTimer = 0;
s32 NoxHeroesDown = 0;
s32 NoxHeroesAlive = 1;
NAS *NoxCameraNas = 0;
NAS *NoxCameraNasB = 0;
s32 NoxBossDead = 0;
u8 *NoxLasers = 0;
s32 NoxSmokeSteps[17] = {
    0x2022, 0x2023, 0x2024, 0x2025, 0x2026, 0x2027, 0x2028, 0x2029, 0x202A, 0x202B, 0x202C,
    0x202D, 0x202E, 0x202F, 0x2030, 0x2031, 0x8003,
};
s32 NoxSmokeAnimList[3] = { (s32)&NoxSmokeSteps, 0, 0 };
ScriptInit NoxSmokeScriptInit = { 0, (void *)&NoxSmokeAnimList, 0x402 };
s32 NoxHeroTiltA = 0;
s32 NoxHeroTiltB = 0;
s32 NoxHeroTiltC = 0;
s32 NoxHeroTiltD = 0;
s32 NoxAsteroidTimer = 0x3C;
s32 NoxUnusedMatrix[8] = { 0x1000, 0, 0x1000, 0, 0x1000, 0, 0, 0 };
s32 NoxBossPathPos = 0;
s32 NoxUnusedSizes[32] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 2, 2, 2, 2, 2, 2,
};
s32 NoxSparkAlpha = 0;
s32 NoxSparkScale = 0x100;
MATRIX NoxSparkMatrix = { { { 0x1000, 0, 0 }, { 0, 0x1000, 0 }, { 0, 0, 0x1000 } }, { 0, 0, 0 } };
NoxSparkFx NoxSparkFxDef = { 0x80, 0xA00, 0xC00, NoxInitSparks, NoxDrawSparks };
s32 NoxStarted = 0;
s32 NoxHeroBoost[4] = {0};
s32 NoxHeroSteer[4] = {0};
AstSlot NoxAsteroids[256] = {0};
AstMineSlot NoxMines[128] = {0};
SetupCam NoxCamera = {0};
s32 NoxUnusedState[8] = {0};
SLONG_XYZ NoxHeroStartA = {0};
SLONG_XYZ NoxHeroStartB = {0};
SVECTOR NoxSparkAngles = {0};


s32 NoxBoxToSphere(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *e) {
    VECTOR d;
    VECTOR n;
    MATRIX m;
    SVECTOR ang;
    SVECTOR sv[2];
    BoxHalf lo;
    BoxHalf hi;
    Box2 *src;
    SVECTOR *p;
    s32 dist;
    s32 side;
    s32 *ap;

    ap = (s32 *)a;
    dist = 0x7FFFFFF;
    side = 0;
    src = (Box2 *)func_8008CE20((subengine *)a, side, side);
    p = sv;
    if (src == NULL) {
        printf(NoxNoCollBoxMsg);
        return side;
    }
    p->pad = 0x180;
    if (c == 2) {
        p->pad = 0x200;
    }
    sv[0].vx = 0;
    p->vy = 0;
    p->vz = 0;
    lo = src->lo;
    hi = src->hi;
    d.vx = b->x - ap[1];
    d.vy = b->y - ap[2];
    d.vz = b->z - ap[3];
    ang.vx = 0;
    ang.vy = 0;
    ang.vz = -((u16 *)a)[10];
    RotMatrixZYX(&ang, &m);
    ApplyMatrixLV(&m, &d, &d);
    if (d.vx < lo.v[0]) {
        side = 2;
    }
    if (d.vy < hi.v[1]) {
        side |= 0x10;
    }
    if (lo.v[1] < d.vy) {
        return -1;
    }
    if (d.vz < lo.v[2]) {
        return -1;
    }
    if (hi.v[0] < d.vx) {
        side |= 1;
    }
    if (hi.v[2] < d.vz) {
        return -1;
    }
    if (side == 0) {
        return 1;
    }
    n.vy = 0;
    switch (side) {
    case 20:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 22:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 21:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 4:
        dist = abs(d.vz - lo.v[2]);
        n.vx = 0;
        n.vz = -0x1000;
        break;
    case 6:
        dist = csqrt((d.vx - lo.v[0]) * (d.vx - lo.v[0]) + (d.vz - lo.v[2]) * (d.vz - lo.v[2])) >> 6;
        n.vx = -0x1000;
        n.vz = -0x1000;
        break;
    case 5:
        dist = csqrt((d.vx - hi.v[0]) * (d.vx - hi.v[0]) + (d.vz - lo.v[2]) * (d.vz - lo.v[2])) >> 6;
        n.vx = 0x1000;
        n.vz = -0x1000;
        break;
    case 24:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 26:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 25:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 8:
        dist = abs(d.vz - hi.v[2]);
        n.vx = 0;
        n.vz = 0x1000;
        break;
    case 10:
        dist = csqrt((d.vx - lo.v[0]) * (d.vx - lo.v[0]) + (d.vz - hi.v[2]) * (d.vz - hi.v[2])) >> 6;
        n.vx = -0x1000;
        n.vz = 0x1000;
        break;
    case 9:
        dist = csqrt((d.vx - hi.v[0]) * (d.vx - hi.v[0]) + (d.vz - hi.v[2]) * (d.vz - hi.v[2])) >> 6;
        n.vx = 0x1000;
        n.vz = 0x1000;
        break;
    case 2:
        dist = abs(d.vx - lo.v[0]);
        n.vx = -0x1000;
        n.vz = 0;
        break;
    case 16:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 1:
        dist = abs(d.vx - hi.v[0]);
        n.vx = 0x1000;
        n.vz = 0;
        break;
    }
    if (dist < p->pad) {
        if (c == 0) {
            s32 push;

            push = p->pad - dist;
            push = BoxPushRot(&ang, (u16 *)a, abs(push));
            RotMatrix(&ang, &m);
            ApplyMatrixLV(&m, &n, &n);
            push += 8;
            e->x += (push * n.vx) >> 12;
            e->y += (push * n.vy) >> 12;
            e->z += (push * n.vz) >> 12;
        }
        return csqrt((((hi.v[0] + lo.v[0]) >> 1) - d.vx) * (((hi.v[0] + lo.v[0]) >> 1) - d.vx) +
                     (((hi.v[1] + lo.v[1]) >> 1) - (d.vy + 0x80)) * (((hi.v[1] + lo.v[1]) >> 1) - (d.vy + 0x80))) >> 6;
    }
    return -1;
}

void NoxInitAsteroids(void) {
    AstSceneObj *o;
    AstKind *k;
    AstSlot *a;
    AstMineSlot *m;
    TrigEntry *trig;
    s32 nAst;
    s32 nMine;
    s32 nBig;
    s32 r;
    u16 id;
    /* Arrays rather than literals: the retail rodata carries 3 stray bytes
     * after the last string's NUL, and the arrays keep the strings' order. */
    static const char NoxMsgInitAsteroids[] = "init asteroids\n";
    static const char NoxMsgArkSubCount[] = "ark sub count %d\n";
    static const char NoxMsgMineCount[] = "mine count %d\n";
    static const char NoxMsgSpinFireCount[] = "spin/fire count %d\n";
    static const char NoxMsgBigMineCount[] = "big mine count %d\n";
    static const char NoxMsgMineArrayError[0x44] ="mine array error"
        "!!!!!!!!!!" "!!!!!!!!!!" "!!!!!!!!!!" "!!!!!!!!!!" "!!!!!!!\n"
        "\0\x0B\x80\x82";

    o = (AstSceneObj *)((GfxScene *)MapScene)->objs;
    printf(NoxMsgInitAsteroids);
    nAst = 0;
    nMine = 0;
    nBig = 0;
    trig = rcossin_tbl;
    a = NoxAsteroids;
    NoxAsteroidTimer = 0x3C;
loop:
    if (o != NULL) {
        do {
            do {
                k = &o->k;
            } while (0);
        } while (0);
        if (k->mode == 0x63) {
            o->flags &= ~0x8000;
        } else if (k->mode != 0x64) {
            if ((u32)(o->k.kind - 1) < 3) {
                o->unk51 = 0;
                o->rx = Random(0x1000);
                o->ry = Random(0x1000);
                o->rz = Random(0x1000);
                o->unk52 = 1;
            }
            if (k->kind == 5 || k->kind == 8) {
                r = Random(0x200);
                o->rz = r - Random(0x200);
                a->ang = Random(0x1000);
                a->pos = o->pos;
                a->unk18 = 3;
                a->unk10 = 0;
                a->unk14 = 0;
                a->unk20 = 0x800;
                a->unk1C = 0;
                a->unk24 = 0xE4C40;
                id = o->id;
                if (id == 0x5002) {
                    a->unk1C = 1;
                    a->unk24 = 0x49DA40;
                    o->rx = Random(0x1000);
                    a->unk20 = 0x2AA;
                    nBig++;
                    if (o->id == id) {
                        o->pos.y = a->pos.y + (trig[(a->ang - 0x400) & 0xFFF].unk2 * 512 >> 12);
                        o->pos.x = a->pos.x + (trig[a->ang & 0xFFF].unk2 * 512 >> 12);
                    } else {
                        goto plain;
                    }
                } else {
                plain:
                    o->pos.y = a->pos.y;
                    o->pos.x = a->pos.x;
                }
                a++;
                nAst++;
            }
            if (k->kind == 6 || k->kind == 9 || k->kind == 10) {
                NoxMines[nMine].unk0 = 0;
                NoxMines[nMine].unkC = 0;
                NoxMines[nMine].ang = Random(0x1000);
                NoxMines[nMine].y = o->pos.y;
                o->rx = 0;
                o->ry = 0;
                o->rz = 0;
                o->unk51 = 0;
                do {
                    nMine++;
                } while (0);
            }
        }
        o = o->next;
        goto loop;
    }
    printf(NoxMsgArkSubCount, 0);
    printf(NoxMsgMineCount, nAst);
    printf(NoxMsgSpinFireCount, nMine);
    printf(NoxMsgBigMineCount, nBig);
    if (nAst > 0x100) {
        printf(NoxMsgMineArrayError);
    }
}

void NoxAsteroidSfx(void) {
    s32 temp_v0;

    temp_v0 = Random(6);
    if (temp_v0 == 0) {
        PlaySfx(0x2BC, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 1) {
        PlaySfx(0x2BD, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 2) {
        PlaySfx(0x2BE, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 3) {
        PlaySfx(0x2BF, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 4) {
        PlaySfx(0x2C0, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 5) {
        PlaySfx(0x2C1, 0, 0, 0x1000, 0x1E00, 0);
    }
}

s32 NoxLaserHitBox(s32 *arg0, LaserPos p) {
    LaserNode *n;
    LaserTgt *t;

    n = (LaserNode *)NoxLasers;
loop:
    if (n != NULL) {
        t = n->tgt;
        if (t != NULL) {
            if (LaserInBox(arg0, (s32 *)&t->pos)) {
                LaserBurst(n);
                return 1;
            }
        }
        n = n->next;
        goto loop;
    }
    return 0;
}

void NoxAsteroidHitHero(s32 *arg0, s32 arg1) {
    AstSlot *a;
    AstSlot *b;
    AstSlot *tb;
    AstSlot *tb1;
    HeroInfo *h;
    BossGameView *g;
    s32 d;
    s32 r;

    h = Heroes;
    if (!(*(s32 *)&h->hero_teammate & 0x22000000)) {
        d = AstDist(arg0, h->hero_eng);
        tb = NoxAsteroids;
        a = &tb[arg1];
        if (d < a->unk24 && d > 0) {
            if (a->unk1C != 1) {
                a->unk10 = 1;
                AstBurst(arg0, 0);
                AstSfx();
                if ((BGV->unk4 & 0x80000001) != 0x80000001 && Heroes[0].hero_mode != 0x31) {
                    Heroes[0].hero_health -= MineDamage(BGV) >> 2;
                    SetHeroAction(0, 0x31);
                }
            } else if ((BGV->unk4 & 0x80000001) != 0x80000001) {
                AstSfx();
                a->unk10 = 1;
                AstBurst(arg0, 4);
                if (Heroes[0].hero_mode != 0x31) {
                    Heroes[0].hero_health -= MineDamage(BGV);
                    SetHeroAction(0, 0x31);
                }
            }
        }
    }
    g = (BossGameView *)GameInfo;
    if (g->unk6C != 0 && (h = Heroes, !(*(s32 *)&h[1].hero_teammate & 0x22000000))) {
        d = AstDist(arg0, h[1].hero_eng);
        tb1 = NoxAsteroids;
        b = &tb1[arg1];
        if (d < b->unk24 && d > 0) {
            if (b->unk1C != 1) {
                b->unk10 = 1;
                AstBurst(arg0, 0);
                AstSfx();
                if ((BGV->unk4 & 0x80000001) != 0x80000001 && Heroes[1].hero_mode != 0x31) {
                    Heroes[1].hero_health -= MineDamage(BGV) >> 2;
                    SetHeroAction(1, 0x31);
                }
            } else if ((g->unk4 & 0x80000001) != 0x80000001) {
                b->unk10 = 1;
                AstBurst(arg0, 4);
                AstSfx();
                if (Heroes[1].hero_mode != 0x31) {
                    Heroes[1].hero_health -= MineDamage(BGV);
                    SetHeroAction(1, 0x31);
                }
            }
        }
    }
    r = 0x90000;
    if (NoxAsteroids[arg1].unk1C == 1) {
        r = 0x57E400;
    }
    if (MineHitLaser(*(LaserPos *)(arg0 + 1), r)) {
        AstSlot *tb2 = NoxAsteroids;
        AstSlot *s = &tb2[arg1];
        if (s->unk1C != 1) {
            if (s->unk18-- < 0) {
                s->unk10 = 1;
                AstBurst(arg0, 0);
                AstSfx();
            }
        } else {
            NoxAsteroidSfx();
        }
    }
}

void NoxTurnAsteroid(AngObj *o) {
    if (o->state == 0x5003) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x500B) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0xC00 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x5019) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x500C) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x501A) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x5004) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0xC00 - (s16)o->ax) << 20) >> 23;
    }
}

void NoxMineHitHero(s32 *arg0, s32 arg1) {
    AstMineSlot *sb;
    AstMineSlot *slot;
    HeroInfo *h;
    BossGameView *g;
    HeroInfo *h2;
    BossGameView *g2;
    s32 r;
    s32 dmg;

    sb = NoxMines;
    slot = &sb[arg1];
    if (slot->unkC == 0) {
        h = Heroes;
        if (!(*(s32 *)&h->hero_teammate & 0x22000000) &&
            NoxBoxToSphere((SLONG_XYZ *)arg0, &h->hero_eng->unk10, 1, &h->hero_eng->unk10) != -1) {
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
            g = (BossGameView *)GameInfo;
            slot->unkC = 1;
            if ((g->unk4 & 0x80000001) != 0x80000001 && h->hero_mode != 0x31) {
                dmg = MineDamage(g);
                Heroes[0].hero_health -= dmg;
                SetHeroAction(0, 0x31);
            }
        }
        g2 = (BossGameView *)GameInfo;
        if (g2->unk6C != 0) {
            h2 = Heroes;
            if (!(*(s32 *)&h2[1].hero_teammate & 0x22000000) &&
            NoxBoxToSphere((SLONG_XYZ *)arg0, &h2[1].hero_eng->unk10, 1, &h2[1].hero_eng->unk10) != -1) {
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
            NoxMines[arg1].unkC = 1;
            if ((g2->unk4 & 0x80000001) != 0x80000001 && h2[1].hero_mode != 0x31) {
                dmg = MineDamage(g2);
                Heroes[1].hero_health -= dmg;
                SetHeroAction(1, 0x31);
            }
            }
        }
        if (NoxLaserHitBox(arg0, *(LaserPos *)(arg0 + 1)) != 0) {
            NoxAsteroidSfx();
        }
    } else {
        NoxTurnAsteroid((AngObj *)arg0);
    }
}

void NoxStartPhase1(void)
{
  unsigned char one = 1;
  void (*new_var)(s32 arg0);
  N2OBallPhase = one;
  GameAdventure.unkAB[0x1F] = 0;
  N2OShowPhaseObjects(one);
  Heroes[0].hero_eng->unk2C.x = 0x1000;
  Heroes[0].hero_eng->unk2C.y = 0x1000;
  Heroes[0].hero_eng->unk2C.z = 0x1000;
  Heroes[1].hero_eng->unk2C.x = 0x1000;
  (&(*(Heroes + 1)).hero_eng->unk2C)->y = 0x1000;
  Heroes[1].hero_eng->unk2C.z = 0x1000;
  Heroes[0].hero_eng->unk5E = 1;
  Heroes[1].hero_eng->unk5E = 1;
  func_80026F48();
  N2OPlaceHero(0);
  N2OPlaceHero(1);
  N2OPlaceHero(3);
  HeroHooks.PreMoveHero = N2OPreMoveHero;
  HeroHooks.KeepInBounds = N2O_KeepInBounds;
  new_var = (void (*)(s32))N2OMoveHero;
  HeroHooks.MoveHero = new_var;
  HeroHooks.CreateShadow = N2OInitShadow;
  HeroHooks.UpdateShadow = N2OUpdateShadow;
 ;
  RenderHook = func_80093038;
  N2OInitBoss();
}

void NoxUpdateAsteroids(void) {
    AstSceneObj *o;
    AstKind *k;
    EXTengine *e;
    u8 kind;
    s32 lim;
    s32 zl;
    s32 nMine;
    s32 nAst;

    nMine = 0;
    o = (AstSceneObj *)((GfxScene *)MapScene)->objs;
    nAst = 0;
    if (GameTeams[0] != 0) {
        if ((Heroes[0].hero_status & 0x2000) && (Heroes[1].hero_status & 0x2000)) {
            return;
        }
    } else if (Heroes[0].hero_status & 0x2000) {
        return;
    }
    for (;;) {
        if (o == NULL) {
            return;
        }
        k = &o->k;
        if (k->mode == 0x63) {
            o->flags &= ~0x8000;
        } else if (k->mode != 0x64) {
            if (o->k.kind == 7 && (o->flags & 0x8000)) {
                o->unk51 = 0;
                o->unk50 = 1;
                if (o->unk20 < 0x7C18) {
                    o->unk20 += 0x80;
                    o->unk24 += 0x80;
                    o->unk28 += 0x80;
                }
                if (Heroes[0].hero_eng->unk2C.x > 0) {
                    Heroes[0].hero_eng->unk2C.x -= 0x30;
                }
                if (Heroes[0].hero_eng->unk2C.x < 0) {
                    Heroes[0].hero_eng->unk2C.x = 0;
                }
                Heroes[0].hero_eng->unk2C.y = Heroes[0].hero_eng->unk2C.x;
                Heroes[0].hero_eng->unk2C.z = Heroes[0].hero_eng->unk2C.x;
                if (((BossGameView *)GameInfo)->unk6C != 0) {
                    Heroes[1].hero_eng->unk2C.x = Heroes[0].hero_eng->unk2C.x;
                    Heroes[1].hero_eng->unk2C.y = Heroes[0].hero_eng->unk2C.x;
                    Heroes[1].hero_eng->unk2C.z = Heroes[0].hero_eng->unk2C.x;
                }
                NoxAsteroidTimer--;
                if (NoxAsteroidTimer == -1) {
                    set_fade(&ScreenFade, 0x100, 0);
                }
                if (NoxAsteroidTimer == -0x3C) {
                    set_fade(&ScreenFade, -0x100, 0);
                    NoxStartPhase1();
                    return;
                }
                if (NoxAsteroidTimer < 0x1F) {
                    NoxHeroesDown = 1;
                    NoxHeroesAlive = 0;
                }
                o->unk52 = 1;
                NoxSparkAlpha = 0x100 - (o->unk20 >> 7);
            }
            if (k->kind == 6 || k->kind == 10 || k->kind == 9) {
                if (o->flags & 0x8000) {
                    if (k->kind != 7) {
                        NoxMineHitHero((s32 *)o, nMine);
                        if (k->kind == 6) {
                            o->rz += 0x20;
                        }
                        if (k->kind == 9) {
                            o->rz -= 0x20;
                        }
                        o->unk52 = 1;
                    }
                }
            }
            if ((k->kind == 5 || k->kind == 8) && (o->flags & 0x8000)) {
                o->unk51 = 0;
                o->ry += 0x20;
                if (NoxAsteroids[nAst].unk1C != 1) {
                    o->pos.y = NoxAsteroids[nAst].pos.y;
                    o->pos.x = NoxAsteroids[nAst].pos.x;
                } else {
                    o->pos.y = NoxAsteroids[nAst].pos.y + (rcossin_tbl[(NoxAsteroids[nAst].ang - 0x400) & 0xFFF].unk2 * 512 >> 12);
                    o->pos.x = NoxAsteroids[nAst].pos.x + (rcossin_tbl[NoxAsteroids[nAst].ang & 0xFFF].unk2 * 512 >> 12);
                }
                NoxAsteroids[nAst].ang = (NoxAsteroids[nAst].ang + 0x20) & 0xFFF;
                if (o->flags & 0x8000) {
                    NoxAsteroidHitHero((s32 *)o, nAst);
                }
                o->unk52 = 1;
            }
            AstSpin(o, k, k->kind);
            kind = k->kind;
            if (kind < 4) {
                o->flags |= 0x8000;
                AST_TRACKED_HERO(e,
                    if (o->pos.z < e->unk10.z - 0x800) {
                        o->flags &= ~0x8000;
                    })
                AST_TRACKED_HERO(e,
                    if (e->unk10.z + 0x6000 < o->pos.z) {
                        o->flags &= ~0x8000;
                    })
            } else if ((u8)(kind - 5) < 6) {
                if (kind == 5 || kind == 8) {
                    if (NoxAsteroids[nAst].unk10 == 1) {
                        o->flags &= ~0x8000;
                    } else {
                        o->flags |= 0x8000;
                        if (k->kind == 8 && ((BossGameView *)GameInfo)->unk6C != 0) {
                            o->flags &= ~0x8000;
                        }
                    }
                    AST_TRACKED_HERO(e,
                        lim = NoxAsteroids[nAst].unk20;
                        zl = e->unk10.z - lim;
                        if (o->pos.z < zl) {
                            o->flags &= ~0x8000;
                            if (NoxAsteroids[nAst].unk14 == 0) {
                                NoxAsteroids[nAst].unk14 = 1;
                            }
                        })
                } else {
                    o->flags |= 0x8000;
                    if (k->kind == 6 || k->kind == 9 || k->kind == 10) {
                        AST_TRACKED_HERO(e,
                            if (o->pos.z < e->unk10.z - 0x800) {
                                o->flags &= ~0x8000;
                            })
                    }
                }
                AST_TRACKED_HERO(e,
                    if (e->unk10.z + 0x6000 < o->pos.z) {
                        o->flags &= ~0x8000;
                    })
            } else {
                AST_TRACKED_HERO(e,
                    if (o->pos.z < e->unk10.z - 0x1000) {
                        o->flags &= ~0x8000;
                    })
                AST_TRACKED_HERO(e,
                    if (e->unk10.z + 0x6000 < o->pos.z) {
                        o->flags &= ~0x8000;
                    })
            }
            if (o->flags & 0x8000) {
                if (((BossGameView *)GameInfo)->unk6C == 0) {
                    if (!(Heroes[0].hero_status & 0x2000)) {
                        AST_NEAR(o, Heroes[0].hero_eng)
                    }
                } else if (!(Heroes[0].hero_status & 0x2000)) {
                    AST_NEAR(o, Heroes[0].hero_eng)
                } else if (!(Heroes[1].hero_status & 0x2000)) {
                    AST_NEAR(o, Heroes[1].hero_eng)
                }
            } else {
                o->flags &= ~0x40000000;
                o->unk76 = 0;
            }
            if (k->kind == 5 || k->kind == 8) {
                nAst++;
            }
            if (k->kind == 6 || k->kind == 9 || k->kind == 10) {
                nMine++;
            }
        }
        o = o->next;
    }
}

void NoxUpdate(void) {
    NAS *first;
    NAS *nas;

    if (((BossGameView *)GameInfo)->unk75 != 0 && (((BossGameView *)GameInfo)->unk30 & 1)) {
        ((BossGameView *)GameInfo)->unk75 = 0;
    }
    if (((BossGameView *)GameInfo)->unk4 & 8) {
        D_8009D6DC[2] |= 0x8000;
    }
    if (NoxStarted == 0) {
        NoxCameraNasB = NULL;
        NoxCameraNas = NULL;
        NoxHeroStartA = Heroes[0].hero_eng->unk10;
        NoxHeroStartB = Heroes[1].hero_eng->unk10;
        first = CreateNAS(0, 0, 0, 0x1C03, 0x11);
        NoxCameraNas = first;
        if (first != NULL) {
            first->UserInfo.user1 = 0;
            func_80026A04((u8 *)first);
        }
        if (((BossGameView *)GameInfo)->unk6C != 0) {
            nas = CreateNAS(0, 0, 0, 0x1C03, 0x11);
            NoxCameraNasB = nas;
            if (nas != NULL) {
                nas->UserInfo.user1 = 1;
                func_80026A04((u8 *)nas);
            }
        }
        func_80026980(Heroes[0].hero);
        func_80026980(Heroes[1].hero);
        NoxStarted = 1;
    }
    if (GamePause == 0) {
        NoxUpdateAsteroids();
    }
}

void NoxHeroCollide(s32 arg0) {
    HeroInfo *h;
    HeroInfo *me;
    EXTheromove *om;
    EXTheromove *mm;
    s32 i;
    s32 total;
    s32 dx;
    s32 dz;
    s32 dy;
    s32 dist;
    s32 ang;
    s32 push;
    s32 d;
    s32 q;
    s32 r;
    TrigEntry *trig;
    HeroInfo *base;

    base = Heroes;
    i = 0;
    total = 0x200;
    trig = rcossin_tbl;
    while (1) {
        if (i >= 4) {
            break;
        }
        h = &base[i];
        if (i != arg0 && !(h->hero_status & 0xFA00)) {
            me = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
            dx = h->hero_eng->unk10.x - me->hero_eng->unk10.x;
            dz = h->hero_eng->unk10.y - me->hero_eng->unk10.y;
            dy = h->hero_eng->unk10.z - me->hero_eng->unk10.z;
            dist = csqrt(dx * dx + dz * dz + dy * dy) >> 6;
            if (dist < 0x300) {
                om = h->hero_move;
                mm = me->hero_move;
                mm->mass = 0x100;
                om->mass = 0x100;
                if (mm->mass > 0) {
                    d = -((om->x_vel - mm->x_vel) << 7);
                    q = (mm->x_vel + om->x_vel) << 16;
                    q = ((d << 8) + q) / total;
                    r = q - d;
                    om->x_vel = q >> 8;
                    mm->x_vel = r >> 8;
                    d = -((om->jump_vel - mm->jump_vel) << 7);
                    q = (mm->jump_vel + om->jump_vel) << 16;
                    q = ((d << 8) + q) / total;
                    r = q - d;
                    om->jump_vel = q >> 8;
                    mm->jump_vel = r >> 8;
                }
                ang = (u16)get_angle(dx, dz) & 0xFFF;
                push = 0x304 - dist;
                me->hero_eng->unk10.x -= push * trig[(u16)(ang - 0x400) & 0xFFF].unk2 >> 12;
                me->hero_eng->unk10.y -= push * trig[ang].unk2 >> 12;
            }
        }
        i++;
    }
}

void NoxPreMoveHero(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTheromove *m;
    BossPad *pad;
    BossPad *pad2;
    BossPad *pb;
    BossPad *pb2;
    s32 *turn;
    s32 spd;
    s32 b;
    EXTengine *e;
    EXTengine *e2;
    HeroInfo *h2;
    HeroInfo *base2;
    HeroInfo *base3;
    HeroInfo *base4;
    HeroInfo *hh;

    if (h->hero_frozen != 0) {
        pb = (BossPad *)Pads;
        pad = &pb[arg0];
        pad->unk50 = 0;
        pad->unk54 = 0;
    }
    m = h->hero_move;
    if (m != NULL) {
        m->forward_angle = 0x800;
        m->x_max = 0;
        m->z_max = 0;
        m->jump_max = 0;
        NoxHeroBoost[arg0] = 0;
        turn = &NoxHeroSteer[arg0];
        pb2 = (BossPad *)Pads;
        pad2 = &pb2[arg0];
        *turn = 0;
        if ((pad2->unk0 & 1) || h->hero_ai->AImode != 0) {
            b = pad2->unk50;
            if (b & 0x80F0) {
                if (b & 0x20) {
                    m->forward_angle = 0x980;
                }
                if (pad2->unk50 & 0x80) {
                    m->forward_angle = 0x680;
                }
                if (pad2->unk50 & 0x10) {
                    *turn = -0x180;
                }
                if (pad2->unk50 & 0x40) {
                    *turn = 0x180;
                }
            }
        }
    }
    if (((BossGameView *)GameInfo)->unk10 == 0) {
        spd = NoxSpeedScale;
        base2 = Heroes;
        hh = &base2[arg0];
        m->x_max = (spd * rcossin_tbl[(hh->hero_eng->unk1E + 0x400) & 0xFFF].unk2) >> 11;
        m->jump_max = (spd * rcossin_tbl[(hh->hero_eng->unk1C + 0x400) & 0xFFF].unk2) >> 11;
        if (NoxBossHitTimer == 0) {
            m->z_max = spd;
        } else {
            m->forward_angle = 0x800;
            m->z_max = 0;
            m->jump_max = 0;
            m->x_max = 0;
            NoxHeroSteer[arg0] = 0;
            e = hh->hero_eng;
            e->unk10.y += (-0xAF0 - e->unk10.y) >> 5;
            if (((BossGameView *)GameInfo)->unk6C == 0) {
                e2 = hh->hero_eng;
                e2->unk10.x -= e2->unk10.x >> 5;
            } else {
                if (NoxHeroTiltA == 0) {
                    if (!(base2[0].hero_status & 0x2000) && !(base2[1].hero_status & 0x2000)) {
                        NoxHeroTiltD = 0;
                        if (base2[0].hero_eng->unk10.x <= base2[1].hero_eng->unk10.x) {
                            NoxHeroTiltB = 0;
                            NoxHeroTiltC = 1;
                        } else {
                            NoxHeroTiltB = 1;
                            NoxHeroTiltC = 0;
                        }
                    } else {
                        NoxHeroTiltD = 1;
                    }
                    NoxHeroTiltA = 1;
                }
                if (NoxHeroTiltD == 0) {
                    if (arg0 == NoxHeroTiltB) {
                        base3 = Heroes;
                        h2 = &base3[arg0];
                        if (!(h2->hero_status & 0x2000)) {
                            e = h2->hero_eng;
                            e->unk10.x -= (NoxHeroesAlive * 0x302 + e->unk10.x) >> 5;
                        }
                    }
                    if (arg0 == NoxHeroTiltC) {
                        base4 = Heroes;
                        h2 = &base4[arg0];
                        if (!(h2->hero_status & 0x2000)) {
                            e = h2->hero_eng;
                            e->unk10.x -= (e->unk10.x - NoxHeroesAlive * 0x302) >> 5;
                        }
                    }
                } else {
                    e = Heroes[arg0].hero_eng;
                    e->unk10.x -= e->unk10.x >> 5;
                }
            }
        }
    }
    BossHeroSteer(arg0);
    m->x_acc = 0x10;
    m->z_acc = 0x10;
    m->jump_acc = 0x10;
    m->z_dec = 1;
    m->x_dec = 1;
    m->jump_dec = 1;
}

void NoxMoveHero(s32 arg0) {
    EXTheromove *m;
    EXTengine *eng;
    HeroInfo *h;
    HeroInfo *hb;
    s32 v;
    s32 step;

    m = Heroes[arg0].hero_move;
    eng = Heroes[arg0].hero_eng;
    HERO_ACCEL(m, x_vel, x_max, x_acc, x_dec);
    HERO_ACCEL(m, z_vel, z_max, z_acc, z_dec);
    HERO_ACCEL(m, jump_vel, jump_max, jump_acc, jump_dec);
    hb = Heroes;
    h = hb + arg0;
    if (!(h->hero_status & 0x200)) {
        eng->unk10.x += m->x_vel;
        eng->unk10.z += m->z_vel;
        eng->unk10.y += m->jump_vel;
    }
    if (h->hero_eng->unk10.y < -0xB00) {
        h->hero_eng->unk10.y = -0xB00;
        m->jump_vel = 0;
    }
    if (h->hero_eng->unk10.y > 0) {
        h->hero_eng->unk10.y = 0;
        m->jump_vel = 0;
    }
    if (h->hero_eng->unk10.x < -0xA00) {
        h->hero_eng->unk10.x = -0xA00;
        m->x_vel = 0;
    }
    if (h->hero_eng->unk10.x > 0xA00) {
        h->hero_eng->unk10.x = 0xA00;
        m->x_vel = 0;
    }
    N2OHeroSpeed[arg0] = m->x_vel;
    if (NoxHeroesDown == 0 && (GameTeams[0] & 1)) {
        NoxHeroCollide(arg0);
    }
}

void NoxHeroFly(s32 arg0) {
    HeroInfo *hb;
    HeroInfo *hb2;
    HeroInfo *h;
    subengine *sub;
    NAS *nas;
    s32 r;
    s16 ang;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;

    hb = Heroes;
    h = &hb[arg0];
    h->hero_eng->unk5E = 1;
    if (N2OHeroUpdateSpeed() != 0) {
        func_8008502C(arg0);
    }
    ang = h->hero_eng->unk1E;
    x1 = (rcossin_tbl[ang & 0xFFF].unk2 * 0x67) >> 10;
    z1 = (rcossin_tbl[(ang + 0x400) & 0xFFF].unk2 * 0x67) >> 10;
    x2 = (rcossin_tbl[(ang - 0x800) & 0xFFF].unk2 * 0x67) >> 10;
    z2 = (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 0x67) >> 10;
    sub = h->hero_sub;
    if (sub->unk74 < 0x401D) {
        sub->unk74++;
    }
    if (sub->unk74 == 0x4007) {
        PlaySfx(0x2B7, 0, 0, 0x1000, 0x1E00, 0);
    }
    if ((Pads[arg0].unk54 & 0x8000) && NoxBossDead == 0) {
        nas = CreateNAS(h->hero_eng->unk10.x + x1, h->hero_eng->unk10.y + 0x80,
                           h->hero_eng->unk10.z + z1, 0x1C07, 0x11);
        if (nas != NULL) {
            nas->Engine->unk1E = h->hero_eng->unk1E + 0x810;
            nas->Engine->unk1C = h->hero_eng->unk1C;
            if (arg0 == 0) {
                r = Random(4);
                if (r == 0) {
                    PlaySfx(0x2AB, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 1) {
                    PlaySfx(0x2AC, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 2) {
                    PlaySfx(0x2AD, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 3) {
                    PlaySfx(0x2AE, 0, 0, 0x1000, 0x1E00, 0);
                }
            }
            nas->UserInfo.user2 = arg0;
        }
        hb2 = Heroes;
        h = &hb2[arg0];
        nas = CreateNAS(h->hero_eng->unk10.x + x2, h->hero_eng->unk10.y + 0x80,
                           h->hero_eng->unk10.z + z2, 0x1C07, 0x11);
        if (nas != NULL) {
            nas->Engine->unk1E = h->hero_eng->unk1E + 0x7F0;
            nas->Engine->unk1C = h->hero_eng->unk1C;
            if (arg0 == 1) {
                r = Random(4);
                if (r == 0) {
                    PlaySfx(0x2AF, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 1) {
                    PlaySfx(0x2B0, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 2) {
                    PlaySfx(0x2B1, 0, 0, 0x1000, 0x1E00, 0);
                }
                if (r == 3) {
                    PlaySfx(0x2B2, 0, 0, 0x1000, 0x1E00, 0);
                }
            }
            nas->UserInfo.user2 = arg0;
        }
    }
    TestNoxHeroWon(arg0);
}

void SetNoxHeroDead(s32 arg0) {
    HeroInfo *h;
    NAS *nas;
    NAS *ret;
    LaserExt *ext;
    subengine *s;
    SLONG_XYZ *p;
    HeroInfo *base;

    func_80028018(arg0, 0xA);
    ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xA, 0xFF);
    base = Heroes;
    h = &base[arg0];
    h->hero_status |= 0xA000;
    if (arg0 == 0) {
        func_80026980(NoxCameraNas);
    }
    if (arg0 == 1) {
        func_80026980(NoxCameraNasB);
    }
    ret = NULL;
    p = &h->hero_eng->unk10;
    nas = CreateNAS(p->x, p->y, p->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 5;
            ext->unk18 = 0x100;
            ext->unk20 = 1;
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
        ret = nas;
    }
    if (ret != NULL) {
        s = ret->Engine->sub;
        if (s != NULL) {
            do {
                s->flags = 0x02000000;
                s->unk68 = -0x200;
                s = s->next;
            } while (s != NULL);
        }
    }
}

void InitAsteroidRoll(NAS *m) {
    AsteroidExt *ext;
    EXTengine *e;

    ext = AddExt(m, 0x80, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = *(void **)MapScene;
        m->Engine->unk5D = 1;
        ext->arc[0] = 0xC;
        ext->bounces = 0;
        ext->unk50 = 0xA0;
        ext->arc[1] = 0;
        ext->floor = 0;
        ext->to.x = Heroes->hero_eng->unk10.x;
        ext->to.z = (rcossin_tbl[0].unk2 * 9) >> 4;
        m->Engine->unk1E = 0;
        ext->to.y = 0;
        ext->from.x = Heroes->hero_eng->unk10.x;
        ext->from.z = 0x1B00;
        ext->from.y = -0x100;
        m->Engine->unk10 = ext->from;
        m->Engine->unk5E = 1;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->unk6C = PowerupsModel;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            e->unk2C.z = 0x4000;
            e->unk2C.y = 0x4000;
            e->unk2C.x = 0x4000;
            ext->shadow->unk5E = 1;
        }
    }
}

void PlotAsteroidRoll(NAS *nas) {
    AsteroidExt *ext;
    s32 n;

    ext = FindExtension(nas, 0x10000000);
    if (ext != NULL) {
        nas->Engine->unk1C += 0x200;
        if (func_8008C5C4(&ext->from.x, &ext->to.x, ext->arc, &nas->Engine->unk10.x) != 0) {
            ext->vy = -((ext->vy * 200) >> 8);
            nas->Engine->unk10.y = ext->floor;
            n = ext->bounces + 1;
            ext->vx *= 200;
            ext->bounces = n;
            ext->vz *= 200;
            ext->vx >>= 8;
            ext->vz >>= 8;
            if (n == 4 ||
                nas->Engine->unk10.x * nas->Engine->unk10.x +
                        nas->Engine->unk10.z * nas->Engine->unk10.z > 0x6400000) {
                KillNAS(nas);
                return;
            }
        }
        ext->shadow->unk10 = nas->Engine->unk10;
        ext->shadow->unk10.y = 0x100;
    }
    nas->Engine->unk5E = 1;
    ext->shadow->unk5E = 1;
}

void PlotNitroMine(NAS *nas) {
    EXTengine *eng;
    EXTengine *me;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 dy;
    s32 z;

    nas->Engine->unk1C += 0x20;
    nas->Engine->unk1E += 0x20;
    nas->Engine->unk5E = 1;
    eng = nas->Engine;
    dx = eng->unk10.x - Heroes[0].hero_eng->unk10.x;
    dz = eng->unk10.z - Heroes[0].hero_eng->unk10.z;
    dy = eng->unk10.y - Heroes[0].hero_eng->unk10.y;
    if (MineHitLaser(*(LaserPos *)&eng->unk10, 0x90000)) {
        MineBurst(nas);
        MineSfx();
        goto kill;
    }
    if (!(*(s32 *)&Heroes[0].hero_teammate & 0x22000000) && dx * dx + dy * dy + dz * dz < 0x90000) {
        MineBurst(nas);
        if ((((BossGameView *)GameInfo)->unk4 & 0x80000001) != 0x80000001 && Heroes[0].hero_mode != 0x31) {
            Heroes[0].hero_health -= MineDamage((BossGameView *)GameInfo) >> 2;
            SetHeroAction(0, 0x31);
        }
        MineSfxV();
        goto kill;
    }
    if (*GameTeams != 0 && (h = Heroes, !(*(s32 *)&h[1].hero_teammate & 0x22000000))) {
        me = nas->Engine;
        if ((me->unk10.x - h[1].hero_eng->unk10.x) * (me->unk10.x - h[1].hero_eng->unk10.x)
            + (me->unk10.y - h[1].hero_eng->unk10.y) * (me->unk10.y - h[1].hero_eng->unk10.y)
            + (me->unk10.z - h[1].hero_eng->unk10.z) * (me->unk10.z - h[1].hero_eng->unk10.z) < 0x40000) {
            MineBurst(nas);
            if ((((BossGameView *)GameInfo)->unk4 & 0x80000001) != 0x80000001
                && !(Heroes[1].hero_status & 0x2000) && Heroes[1].hero_mode != 0x31) {
                Heroes[1].hero_health -= MineDamage((BossGameView *)GameInfo) >> 2;
                SetHeroAction(1, 0x31);
            }
            MineSfxH();
            goto kill;
        }
    }
    z = nas->Engine->unk10.z;
    if (z < Heroes[0].hero_eng->unk10.z - 0x11F4 ||
        (*GameTeams != 0 && z < Heroes[1].hero_eng->unk10.z - 0x11F4)) {
    kill:
        KillNAS(nas);
    }
}

void PlotNitroMissile(NAS *nas) {
    NitroShotExt *ext;
    NitroShotExt *e2;
    NAS *n;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *he;
    SLONG_XYZ *pos;
    BossGameView *g;
    s32 dx;
    s32 dz;
    s32 dy;
    s32 xx;
    s32 zz;
    s32 d;
    s32 a;
    s32 b;
    s32 r;

    if (*GameTeams != 0) {
        if ((Heroes[0].hero_status & 0x2000) && (Heroes[1].hero_status & 0x2000)) {
            ((void (*)(s32, s32, s32, u16, u16))CreateNAS)(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1C09, 0x11);
            goto kill;
        }
    } else if (Heroes[0].hero_status & 0x2000) {
        CreateNAS(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1C09, 0x11);
        goto kill;
    }
    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    nas->Engine->unk5E = 1;
    if (ext->unk18-- < 0) {
        CreateNAS(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1C09, 0x11);
        ext->unk18 = 1;
    }
    switch (ext->unkC) {
    case 0:
        he = Heroes[ext->unk14].hero_eng;
        dx = nas->Engine->unk10.x - he->unk10.x;
        xx = dx * dx;
        dz = nas->Engine->unk10.z - he->unk10.z;
        zz = dz * dz;
        dy = nas->Engine->unk10.y - he->unk10.y + 0x100;
        d = csqrt(xx + zz) >> 6;
        a = get_angle(dx, dz) & 0xFFF;
        b = get_angle(-dy, d);
        nas->Engine->unk1E += ((a - nas->Engine->unk1E) << 20) >> 20;
        nas->Engine->unk1C += ((b - nas->Engine->unk1C) << 20) >> 20;
        nas->Engine->unk10.x -= rcossin_tbl[(nas->Engine->unk1E - 0x400) & 0xFFF].unk2 * 55 >> 10;
        nas->Engine->unk10.z -= rcossin_tbl[nas->Engine->unk1E & 0xFFF].unk2 * 55 >> 10;
        nas->Engine->unk10.y += rcossin_tbl[(nas->Engine->unk1C - 0x400) & 0xFFF].unk2 * 55 >> 10;
        eng = nas->Engine;
        h = &Heroes[ext->unk14];
        if (eng->unk10.z < h->hero_eng->unk10.z) {
            ext->unkC = 1;
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x29B, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x29C, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x29D, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x29E, 0, 0, 0x1000, 0x1E00, 0);
            }
            return;
        }
        if (h->hero_status & 0x2000) {
            return;
        }
        if (h->hero_status & 0x200) {
            return;
        }
        if (xx + dy * dy + zz > 0x3FFFF) {
            return;
        }
        pos = &eng->unk10;
        n = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            e2 = FindExtension(n, 0x10000000);
            if (e2 != NULL) {
                e2->unk18 = 0x100;
                e2->unkC = 0;
                e2->unk20 = 1;
                e2->unk28 = 0;
                n->InitDelete(n);
            }
        }
        g = (BossGameView *)GameInfo;
        if ((g->unk4 & 0x80000001) != 0x80000001) {
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x2A7, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x2A8, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x2A9, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x2AA, 0, 0, 0x1000, 0x1E00, 0);
            }
            if (Heroes[ext->unk14].hero_mode != 0x31) {
                Heroes[ext->unk14].hero_health -= MineDamage(g) >> 2;
                SetHeroAction(ext->unk14, 0x31);
            }
        }
        goto kill;
    case 1:
        nas->Engine->unk10.x -= rcossin_tbl[(nas->Engine->unk1E - 0x400) & 0xFFF].unk2 * 55 >> 10;
        nas->Engine->unk10.z -= rcossin_tbl[nas->Engine->unk1E & 0xFFF].unk2 * 55 >> 10;
        nas->Engine->unk10.y += rcossin_tbl[(nas->Engine->unk1C - 0x400) & 0xFFF].unk2 * 55 >> 10;
        if (nas->Engine->unk10.z < Heroes[ext->unk14].hero_eng->unk10.z - 0x1000) {
            goto kill;
        }
        return;
    }
    return;
kill:
    KillNAS(nas);
}

void NoxBossFollowPath(BossPath *arg0, BossObj *obj) {
    BossObj *arg1 = obj;
    BossGameView *g = (BossGameView *)GameInfo;
    s32 d;
    EXTengine *target;
    EXTengine *e;
    s32 tz;
    s32 dist;
    s32 ang[3];
    SLONG_XYZ pt;
    s32 *p;
    s32 *q;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 *out;

    if (g->unk10 != 0) {
        return;
    }
    do {
        if (g->unk6C == 0) {
            target = Heroes[0].hero_eng;
        } else if (!(Heroes[0].hero_status & 0x2000)) {
            target = Heroes[0].hero_eng;
        } else {
            target = Heroes[1].hero_eng;
        }
        e = arg1->eng;
        do {
            tz = target->unk10.z;
        } while (0);
        d = abs(tz - e->unk10.z);
    } while (0);
    if ((Heroes[0].hero_status & 0x2000) && (Heroes[1].hero_status & 0x2000)) {
        arg0->pos += NoxSpeedScale;
    } else if (d < 0x2000) {
        arg0->pos += 0x400;
    }
    func_8001E21C(arg0->keys, arg0->pos, &pt);
    arg1->eng->unk10.x += (pt.x - arg1->eng->unk10.x) >> 4;
    arg1->eng->unk10.z += (pt.z - arg1->eng->unk10.z) >> 4;
    arg1->eng->unk10.y += (pt.y - arg1->eng->unk10.y) >> 4;
    p = (s32 *)func_8001E41C(arg0->keys, arg0->pos);
    out = ang;
    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz);
    ang[0] = -get_angle(dy, dist);
    arg1->eng->unk1E += ((ang[1] - arg1->eng->unk1E) << 20) >> 23;
    arg1->eng->unk1C += ((ang[0] - arg1->eng->unk1C) << 20) >> 23;
}

void InitNitroBoss(NAS *nas) {
    BossExt *ext = AddExt(nas, 0x3C, 0x10000000);
    BossScene *scene;
    u8 *p;

    if (ext != NULL) {
        nas->Engine->sub->unk6C = *(void **)MapScene;
        nas->Engine->unk2C.x = 0x1830;
        nas->Engine->unk2C.y = 0x1830;
        nas->Engine->unk2C.z = 0x1830;
        func_80086868();
        D_8009D6E2 = 0xFF;
        NoxBossPathPos = 0;
        ext->cursor = func_80024E30(nas, &NoxBossScriptInit);
        nas->Engine->unk5D = 2;
        nas->Engine->unk5C = 1;
        func_800250B0(nas);
        nas->UserInfo.user1 = 0;
        ext->unk14 = 0x3200;
        scene = MapScene;
        p = scene->unk2C;
        ext->keys = (KeyBlock *)(p + (*(s32 *)(p + 4) + 4));
        func_8001E21C(ext->keys, ext->unk14, &nas->Engine->unk10);
        ext->unk18 = 0;
        ext->unk1C = 0;
        ext->unk20 = 0;
        ext->unk38 = 0;
        ext->unk30 = 0;
        ext->unk34 = 0;
    }
}

void PlotNitroBoss(NAS *nas) {
    BossExt *ext;
    EXTengine *e;
    NAS *n;
    NAS *m;
    NitroShotExt *s;
    NitroShotExt *t;
    NitroShotExt *r;
    NitroShotExt *q;
    SLONG_XYZ *pos;
    u16 *fl;
    s32 k;
    s32 k2;
    s32 k3;
    EXTengine *e2;

    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    fl = (u16 *)D_8009D6DC;
    if (fl[2] & 0x8000) {
        pos = &nas->Engine->unk10;
        n = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            s = FindExtension(n, 0x10000000);
            if (s != NULL) {
                s->unk18 = 0x100;
                s->unkC = 0;
                s->unk20 = 1;
                s->unk28 = 0;
                n->InitDelete(n);
            }
        }
        PlaySfx(0x2E5, 0, 0, 0x1000, 0x1E00, 0);
        KillNAS(nas);
        N2OBossNas = NULL;
        return;
    }
    nas->Engine->unk5E = 1;
    switch (ext->unk18) {
    case 0:
        NoxBossFollowPath((BossPath *)ext, (BossObj *)nas);
        k = ((u16 *)func_8001E41C(ext->keys, ext->unk14))[0xB];
        if (k == 2) {
            ext->unk18 = 1;
        }
        if (k == 1) {
            ext->unk18 = 2;
        }
        if (k == 4) {
            NoxBossDead = 1;
            NoxBossHitTimer = 1;
            NoxSparkAlpha = 0x400;
            if (NoxBossPathPos == 0) {
                PlaySfx(0x2B9, 0, 0, 0x1000, 0x1E00, 0);
                NoxBossPathPos = 1;
            }
        }
        if (k == 5) {
            nas->Engine->unk2C.x -= 0x300;
            if (nas->Engine->unk2C.x < 0) {
                nas->Engine->unk2C.x = 0;
            }
            nas->Engine->unk2C.y = nas->Engine->unk2C.x;
            nas->Engine->unk2C.z = nas->Engine->unk2C.x;
        }
        if (k == 6) {
            NoxBossHits = 1;
        }
        break;
    case 1:
        NoxBossFollowPath((BossPath *)ext, (BossObj *)nas);
        if (ext->unk1C-- < 0) {
            if (GameTeams[0] == 0) {
                if (!(Heroes[0].hero_status & 0x2000)) {
                    e = nas->Engine;
                    m = CreateNAS(e->unk10.x, e->unk10.y, e->unk10.z, 0x1C04, 0x11);
                    if (m != NULL) {
                        q = FindExtension(m, 0x10000000);
                        if (q != NULL) {
                            q->unk14 = 0;
                        }
                    }
                }
            } else {
                if (!(Heroes[0].hero_status & 0x2000)) {
                    e = nas->Engine;
                    m = CreateNAS(e->unk10.x, e->unk10.y, e->unk10.z, 0x1C04, 0x11);
                    if (m != NULL) {
                        r = FindExtension(m, 0x10000000);
                        if (r != NULL) {
                            r->unk14 = 0;
                        }
                    }
                }
                if (!(Heroes[1].hero_status & 0x2000)) {
                    e = nas->Engine;
                    m = CreateNAS(e->unk10.x, e->unk10.y, e->unk10.z, 0x1C04, 0x11);
                    if (m != NULL) {
                        t = FindExtension(m, 0x10000000);
                        if (t != NULL) {
                            t->unk14 = 1;
                        }
                    }
                }
            }
            ext->unk1C = 0x1E;
        }
        k2 = ((u16 *)func_8001E41C(ext->keys, ext->unk14))[0xB];
        if (k2 == 3) {
            ext->unk18 = 0;
        }
        if (k2 == 4) {
            NoxBossDead = 1;
            ext->unk18 = 0;
        }
        break;
    case 2:
        NoxBossFollowPath((BossPath *)ext, (BossObj *)nas);
        if (ext->unk20-- < 0) {
            e2 = nas->Engine;
            CreateNAS(e2->unk10.x, e2->unk10.y, e2->unk10.z, 0x1C05, 0x11);
            ext->unk20 = 3;
        }
        k3 = ((u16 *)func_8001E41C(ext->keys, ext->unk14))[0xB];
        if (k3 == 3) {
            ext->unk18 = 0;
        }
        if (k3 == 4) {
            NoxBossDead = 1;
            ext->unk18 = 0;
        }
        break;
    case 3:
        fl[2] |= 0x8000;
        return;
    }
    func_800250B0(nas);
}

void InitSpiralSat(NAS *nas) {
    SpiralExt *ext = AddExt(nas, 0x54, 0x10000000);
    EXTengine *e;
    NAS *s;
    LaserExt *le;
    NAS *ret;
    SLONG_XYZ *q;
    s32 i, mode;

    if (ext != NULL) {
        ext->c = nas->Engine->unk10;
        ext->unk3C = 0;
        ext->unk40 = Random(0x1000);
        i = 0;
        for (;; i++) {
            if (i >= 4) {
                goto done;
            }
            ext->sat[i] = NULL;
            e = AddExtEngine(nas, i + 2);
            if (e != NULL) {
                ext->ang[i] = i << 10;
                ext->eng[i] = e;
                ext->radius = 0x800;
                e->unk10.x = ext->c.x + ((ext->radius * rcossin_tbl[(ext->ang[i] - 0x400) & 0xFFF].unk2) >> 12);
                e->unk10.y = ext->c.y + ((ext->radius * rcossin_tbl[ext->ang[i] & 0xFFF].unk2) >> 12);
                e->unk10.z = ext->c.z;
                e->sub->unk6C = *(s32 *)MapScene;
                e->sub->unk74 = 0x201A;
                e->sub->flags = 0x8000;
                q =&e->unk10;
                s = CreateNAS(q->x, q->y, q->z, 0x1605, 0x11);
                mode = 2;
                if (s != NULL) {
                    le = FindExtension(s, 0x10000000);
                    if (le != NULL) {
                        le->unkC = mode;
                        le->unk18 = 0x100;
                        le->unk20 = 0;
                        le->unk28 = 0;
                        s->InitDelete(s);
                    }
                    ret = s;
                } else {
                    ret = NULL;
                }
                ext->sat[i] = ret;
            }
        }
    }
done:;
}

void PlotSpiralSat(NAS *nas) {
    SpiralExt *ext;
    EXTengine *f;
    EXTengine *e;
    EXTengine *g;
    NAS *s;
    LaserExt *le;
    SLONG_XYZ *q;
    SLONG_XYZ *r;
    s32 i, j, mode;
    SpiralExt *p;
    s32 dx, dy, dz;
    s32 z;

    ext = FindExtension(nas, 0x10000000);
    if (ext != NULL && ext->unk3C == 0) {
        for (i = 0;; i++) {
            if (i >= 4) {
                goto done;
            }
            f = ext->eng[i];
            if (f == NULL) {
                continue;
            }
            if (((BossGameView *)GameInfo)->unk6C == 0) {
                if (Heroes[0].hero_eng->unk10.z + 0x6000 < f->unk10.z) {
                    f->sub->flags &= ~0x8000;
                    if (ext->sat[i] != NULL) {
                        ext->sat[i]->Engine->flags &= ~0x8000;
                    }
                } else {
                    goto show;
                }
            } else if (!(Heroes[0].hero_status & 0x2000)) {
                if (Heroes[0].hero_eng->unk10.z + 0x6000 < f->unk10.z) {
                    f->sub->flags &= ~0x8000;
                    if (ext->sat[i] != NULL) {
                        ext->sat[i]->Engine->flags &= ~0x8000;
                    }
                } else {
                    goto show;
                }
            } else if (Heroes[1].hero_eng->unk10.z + 0x6000 < f->unk10.z) {
                f->sub->flags &= ~0x8000;
                if (ext->sat[i] != NULL) {
                    ext->sat[i]->Engine->flags &= ~0x8000;
                }
            } else {
            show:
                f->sub->flags |= 0x8000;
                if (ext->sat[i] != NULL) {
                    ext->sat[i]->Engine->flags |= 0x8000;
                }
            }
            do {
                e = ext->eng[i];
            } while (0);
            if (!(e->sub->flags & 0x8000)) {
                continue;
            }
            dx = e->unk10.x - Heroes[0].hero_eng->unk10.x;
            dz = e->unk10.z - Heroes[0].hero_eng->unk10.z;
            dy = e->unk10.y - Heroes[0].hero_eng->unk10.y;
            if (!(Heroes[0].hero_status & 0x2000) && dx * dx + dy * dy + dz * dz < 0x90000) {
                r = &e->unk10;
                s = CreateNAS(e->unk10.x, r->y, r->z, 0x1605, 0x11);
                mode = 0;
                if (s != NULL) {
                    le = FindExtension(s, 0x10000000);
                    if (le != NULL) {
                        le->unk18 = 0x100;
                        le->unkC = mode;
                        le->unk20 = 1;
                        le->unk28 = 0;
                        s->InitDelete(s);
                    }
                }
                if ((((BossGameView *)GameInfo)->unk4 & 0x80000001) != 0x80000001 && Heroes[0].hero_mode != 0x30) {
                    extern int SetHeroAction();
                    ((void (*)(s32, s32))SetHeroAction)(0, 0x30);
                }
                goto kill;
            }
            if (((BossGameView *)GameInfo)->unk6C != 0 && !(Heroes[1].hero_status & 0x2000)) {
                g = ext->eng[i];
                if ((g->unk10.x - Heroes[1].hero_eng->unk10.x) * (g->unk10.x - Heroes[1].hero_eng->unk10.x)
                    + (g->unk10.y - Heroes[1].hero_eng->unk10.y) * (g->unk10.y - Heroes[1].hero_eng->unk10.y)
                    + (g->unk10.z - Heroes[1].hero_eng->unk10.z) * (g->unk10.z - Heroes[1].hero_eng->unk10.z) < 0x90000) {
                    q = &g->unk10;
                    s = CreateNAS(g->unk10.x, q->y, q->z, 0x1605, 0x11);
                    mode = 0;
                    if (s != NULL) {
                        le = FindExtension(s, 0x10000000);
                        if (le != NULL) {
                            le->unk18 = 0x100;
                            le->unkC = mode;
                            le->unk20 = 1;
                            le->unk28 = 0;
                            s->InitDelete(s);
                        }
                    }
                    if ((((BossGameView *)GameInfo)->unk4 & 0x80000001) != 0x80000001
                        && !(Heroes[1].hero_status & 0x2000) && Heroes[1].hero_mode != 0x30) {
                        SetHeroAction(1, 0x30);
                    }
                    goto kill;
                }
            }
            ext->eng[i]->unk1C += 0x40;
            ext->eng[i]->unk1E += 0x40;
            ext->eng[i]->unk5E = 1;
            ext->eng[i]->unk10.x = ext->c.x + ((ext->radius * rcossin_tbl[(ext->ang[i] - 0x400) & 0xFFF].unk2) >> 12);
            ext->eng[i]->unk10.y = ext->c.y + ((ext->radius * rcossin_tbl[ext->ang[i] & 0xFFF].unk2) >> 12);
            ext->eng[i]->unk10.z = ext->c.z;
            if (ext->sat[i] != NULL) {
                ext->sat[i]->Engine->unk10 = ext->eng[i]->unk10;
            }
            if (ext->unk40 < 0x800) {
                ext->ang[i] += 0x30;
            } else {
                ext->ang[i] -= 0x30;
            }
        }
    done:
        ext->radius = abs((rcossin_tbl[(ext->unk40 - 0x400) & 0xFFF].unk2 * 3) >> 3) + 0x200;
        ext->unk40 = (ext->unk40 + 0x18) & 0xFFF;
    }
    z = nas->Engine->unk10.z;
    j = 0;
    if (z < Heroes[0].hero_eng->unk10.z - 0x800) {
        p = ext;
        for (; j < 4; j++) {
            if (p->sat[0] != NULL) {
                KillNAS(p->sat[0]);
            }
            p = (SpiralExt *)((s32 *)p + 1);
        }
        goto kill;
    }
    if (*GameTeams != 0 && z < Heroes[1].hero_eng->unk10.z - 0x800) {
        p = ext;
        for (mode = 0; mode < 4; mode++) {
            if (p->sat[0] != NULL) {
                KillNAS(p->sat[0]);
            }
            p = (SpiralExt *)((s32 *)p + 1);
        }
    kill:
        KillNAS(nas);
    }
}

void PlotMineSFX(NAS *nas) {
    s32 z;

    nas->Engine->unk10.z = nas->UserInfo.user1;
    if (GameTeams[0] == 0) {
        if (Heroes[0].hero_eng->unk10.z + 0x6000 < nas->Engine->unk10.z) {
            nas->Engine->sub->flags &= ~0x8000;
        } else {
            nas->Engine->sub->flags |= 0x8000;
        }
    } else if (!(Heroes[0].hero_status & 0x2000)) {
        if (Heroes[0].hero_eng->unk10.z + 0x6000 < nas->Engine->unk10.z) {
            nas->Engine->sub->flags &= ~0x8000;
        } else {
            nas->Engine->sub->flags |= 0x8000;
        }
    } else {
        if (Heroes[1].hero_eng->unk10.z + 0x6000 < nas->Engine->unk10.z) {
            nas->Engine->sub->flags &= ~0x8000;
        } else {
            nas->Engine->sub->flags |= 0x8000;
        }
    }
    nas->UserInfo.user2++;
    if (nas->Engine->sub->flags & 0x8000) {
        z = nas->Engine->unk10.z;
        if (z < Heroes[0].hero_eng->unk10.z + 0xC00 ||
            (GameTeams[0] != 0 && z < Heroes[1].hero_eng->unk10.z + 0xC00)) {
            if (nas->UserInfo.status == 0) {
                PlaySfx(0x29B, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (nas->UserInfo.status == 1) {
                PlaySfx(0x29C, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (nas->UserInfo.status == 2) {
                PlaySfx(0x29D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (nas->UserInfo.status == 3) {
                PlaySfx(0x29E, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            KillNAS(nas);
        }
    }
}

void PlotLaserShoot(NAS *nas) {
    EXTengine *e;
    EXTengine *t;
    SLONG_XYZ *p;
    NAS *s;
    LaserExt *ext;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 r;

    nas->Engine->unk10.x += rcossin_tbl[(nas->Engine->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 12;
    nas->Engine->unk10.z += rcossin_tbl[nas->Engine->unk1E & 0xFFF].unk2 * 1024 >> 12;
    nas->Engine->unk10.y -= rcossin_tbl[(nas->Engine->unk1C - 0x400) & 0xFFF].unk2 * 1024 >> 12;
    if (nas->UserInfo.user2 == 0) {
        nas->Engine->sub->unk74 = 0x201D;
    }
    if (nas->UserInfo.user2 == 1) {
        nas->Engine->sub->unk74 = 0x201E;
    }
    nas->Engine->unk5E = 1;
    nas->UserInfo.user1++;
    if (N2OBossNas != NULL) {
        e = nas->Engine;
        t = N2OBossNas->Engine;
        dx = e->unk10.x - t->unk10.x;
        dy = e->unk10.y - t->unk10.y;
        dz = e->unk10.z - t->unk10.z;
        if (dx * dx + dy * dy + dz * dz <= 0x8FFFF) {
            p = &e->unk10;
            s = CreateNAS(p->x, p->y, p->z, 0x1605, 0x11);
            if (s != NULL) {
                ext = FindExtension(s, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = 3;
                    ext->unk18 = 0x100;
                    ext->unk20 = 1;
                    ext->unk28 = 0;
                    s->InitDelete(s);
                }
            }
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x2B3, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x2B4, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x2B5, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x2B6, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (D_8009D6DC[3] <= 0) {
                D_8009D6DC[3] = 0;
                D_8009D6DC[2] |= 0x8000;
            }
            goto kill;
        }
    }
    if (nas->UserInfo.user1 >= 0x10) {
kill:
        KillNAS(nas);
    }
}

void NoxInitSparks(SparkObj *arg0) {
    SparkSet *set = &arg0->set;
    s32 n = set->count;
    Spark *s = arg0->set.sparks;
    SparkLine *a;
    SparkLine *b;

    NoxSparkScale = 0x100;
    NoxSparkAlpha = 0x100;
    a = set->lines[0];
    b = set->lines[1];
    if (n != 0) do {
        s->x = Random(0x2000) - Random(0x2000);
        s->y = Random(0x1000) - Random(0x1000);
        s->z = Random(0x4000);
        s->x2 = s->x + 0x40;
        s->y2 = s->y + 0x40;
        a->x0 = 0;
        b->x0 = 0;
        a->y0 = 0;
        b->y0 = 0;
        a->r0 = 0x80;
        a->g0 = 0x80;
        a->b0 = 0x80;
        b->r0 = 0x80;
        b->g0 = 0x80;
        b->b0 = 0x80;
        a->len = 4;
        a->code = 0x50;
        b->len = 4;
        b->code = 0x50;
        a->code |= 2;
        b->code |= 2;
        a++;
        b++;
        s++;
        n--;
    } while (n != 0);
}

void NoxDrawSparks(SparkObj *arg0) {
    SparkSet *set = &arg0->set;
    Spark *s = set->sparks;
    SparkLine *l = set->lines[FrameCount & 1];
    Spark *v;
    s32 n;

    if (NoxSparkScale != NoxSparkAlpha) {
        if (NoxSparkScale < NoxSparkAlpha) {
            NoxSparkScale += 0x20;
        }
        if (NoxSparkAlpha < NoxSparkScale) {
            NoxSparkScale -= 0x20;
        }
    }
    if (NoxSparkAlpha >= 0x400) {
        NoxSparkAngles.vx = 0;
        NoxSparkAngles.vy = 0;
        NoxSparkAngles.vz += 0x10;
        RotMatrix(&NoxSparkAngles, &NoxSparkMatrix);
    }
    gte_SetRotMatrix(&NoxSparkMatrix);
    gte_SetTransMatrix(&NoxSparkMatrix);
    gte_SetGeomOffset(ScreenRect->w / 2, 120);
    n = set->count;
    calc_rgbcode(0x10, 0x10, 0x10);
    if (n != 0) do {
        v = s;
        gte_ldv0(v);
        gte_rtps();
        gte_stsxy(&l->x0);
        if (GamePause == 0) {
            s->z -= NoxSparkScale;
        }
        if (s->z < 0) {
            s->x = Random(0x2000) - Random(0x2000);
            s->y = Random(0x1000) - Random(0x1000);
            s->z = 0x4000;
            s->x2 = s->x + 0x40;
            s->y2 = s->y + 0x40;
        } else {
            s->z2 = s->z;
            if (RenderState.modelDp != 0) {
                l->r1 = l->r1 * ((0x1000 - RenderState.modelDp) >> 12);
                l->g1 = l->g1 * ((0x1000 - RenderState.modelDp) >> 12);
                l->b1 = l->b1 * ((0x1000 - RenderState.modelDp) >> 12);
            } else {
                l->r1 = -(s->z >> 6);
                l->g1 = -(s->z >> 6);
                l->b1 = -(s->z >> 6);
            }
            if (NoxSparkAlpha >= 0x300) {
                l->r0 = 0;
                l->g0 = 0;
                l->b0 = 0;
                gte_ldv0(v);
                gte_rtps();
                gte_stsxy(&l->x1);
            } else {
                if (RenderState.modelDp != 0) {
                    l->r1 = l->r1 * ((0x1000 - RenderState.modelDp) >> 12);
                    l->g1 = l->g1 * ((0x1000 - RenderState.modelDp) >> 12);
                    l->b1 = l->b1 * ((0x1000 - RenderState.modelDp) >> 12);
                }
                l->r0 = l->r1;
                l->g0 = l->g1;
                l->b0 = l->b1;
                l->x1 = l->x0;
                l->y1 = l->y0;
            }
            l->len = 4;
            l->code = 0x50;
            addPrim(RenderState.ot + RenderState.otLen - 4, l);
        }
        l++;
        s++;
    } while (--n != 0);
}

void NoxCameraFollow(void) {
    FollowCam *cam;
    EXTengine *e = Heroes->hero_eng;
    s32 y;

    cam = (FollowCam *)MainCamera;
    cam->x += (((e->unk10.x + Heroes[1].hero_eng->unk10.x) >> 1) - cam->x) >> 3;
    cam->z = e->unk10.z;
    y = e->unk10.y;
    cam->unk8 = 0x990;
    cam->unk0 = 0;
    cam->unk54 = 0x1A8;
    cam->y = y;
    cam->unk58 = e->unk20 >> 3;
}

void InitLaserShoot(LaserShoot *arg0) {
    arg0->obj->sub->unk6C = *(s32 *)MapScene;
    arg0->obj->sub->unk74 = 0x201D;
    arg0->obj->unk1E = 0;
    arg0->obj->unk5D = 2;
    arg0->unk60 = 0;
    func_80026B70(&NoxLasers, (u8 *)arg0);
}

void InitMineSFX(MineSFX *arg0) {
    arg0->unk5C = Random(4);
    arg0->unk64 = Random(0x1000);
    arg0->unk60 = arg0->src->unk18;
}

void NoxPathAngles(s32 *arg0, s32 flag, s32 *out) {
    s32 *p = arg0;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 *q;

    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz) + (flag << 11);
    if (flag != 0) {
        out[0] = get_angle(dy, dist);
        return;
    }
    out[0] = -get_angle(dy, dist);
}

void InitNitroMissile(MissileHolder *arg0) {
    MissileExt *ext = AddExt((NAS *)arg0, 0x1C, 0x10000000);
    s32 r;

    if (ext != NULL) {
        arg0->obj->model->unk6C = *(s32 *)MapScene;
        arg0->obj->model->unk74 = 0x2019;
        arg0->obj->unk5D = 2;
        ext->unk10 = 0;
        ext->unkC = 0;
        ext->unk14 = 0;
        ext->unk18 = 0;
        r = Random(4);
        if (r == 0) {
            PlaySfx(0x2A3, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x2A4, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x2A5, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x2A6, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void InitNitroMine(MineHolder *arg0) {
    arg0->obj->model->unk6C = *(s32 *)MapScene;
    arg0->obj->model->unk74 = 0x2012;
    arg0->obj->unk1C = Random(0x1000);
    arg0->obj->unk1E = Random(0x1000);
    arg0->obj->unk20 = Random(0x1000);
    arg0->obj->unk5E = 1;
}

void NoxSpawnSmokeBoost(void) {
    if (NoxSmokeTimer-- < 0) {
        CreateNAS(0, 0, 0, 0x1C01, 0x11);
        NoxSmokeTimer = 0xF;
    }
}

void NoxHeroWinner(s32 arg0) {
    func_80085890(arg0);
    Heroes[arg0].hero_eng->unk1E = 0x800;
    Heroes[arg0].hero_move->forward_angle = 0x800;
    Heroes[arg0].hero_eng->unk5E = 1;
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void SetNoxHeroWinner(s32 arg0) {
    func_80089FA4(arg0);
    func_80085718(arg0, 0);
    Heroes[arg0].hero_eng->unk20 = 0;
    NoxSpeedScale = 0;
    Heroes[arg0].hero_eng->unk1C = 0;
    Heroes[arg0].hero_eng->unk1E = 0x800;
    Heroes[arg0].hero_eng->unk5E = 1;
}

s32 TestNoxHeroWon(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x32) {
            SetHeroAction(arg0, 0x32);
        }
        return 1;
    }
    return 0;
}

void NoxHeroHit(s32 arg0) {
    HeroInfo *h;
    s32 act;

    HeroInfo *base = Heroes;
    h = &base[arg0];
    act = 0x2F;
    if (h->hero_health <= 0) {
        h->hero_health = 0;
        act = 0x30;
    }
    SetHeroAction(arg0, act);
}

void SetNoxHeroHit(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;

    CreateNAS(e->unk10.x, e->unk10.y, e->unk10.z + 0x400, 0x1C09, 0x11);
    func_80028018(arg0, 0xA);
    ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xA, 0xFF);
}

void NoxHeroDead(void) {
}

void SetNoxHeroFly(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTheromove *m = h->hero_move;

    SetHeroSeq(arg0, 0x22);
    N2OHeroSetSpeed(arg0);
    m->x_acc = 0x10;
    m->z_acc = 0x10;
    m->jump_acc = 0x10;
    m->mass = 0x100;
    m->z_dec = 1;
    m->x_dec = 1;
    m->jump_dec = 1;
    h->hero->UserInfo.user1 = 0;
}

void NoxHeroFacing(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
    h->hero_eng->unk5E = 1;
}

s32 Nox_KeepInBounds(void) {
    return 0;
}

void NoxMixVelocities(MixObj *arg0, MixObj *arg1)
{
  MixObj *p = arg0;
  MixObj *q = arg1;
  s32 d;
  s32 s;
  int new_var;
  s32 t;
  s32 u;
  s32 n;
  q->w = 0x100;
  p->w = 0x100;
  if (q->w > 0)
  {
    d = -((p->a - q->a) << 7);
    s = (q->a + p->a) << 16;
    new_var = (d << 8) + s;
    n = 0x200;
    t = new_var / n;
    u = t - d;
    p->a = t >> 8;
    q->a = u >> 8;
    d = -((p->b - q->b) << 7);
    s = (q->b + p->b) << 16;
    t = ((d << 8) + s) / n;
    u = t - d;
    p->b = t >> 8;
    q->b = u >> 8;
  }
}

void PlotDummyCamera(NAS *nas) {
    nas->Camera.width = Heroes[nas->UserInfo.user1].hero->Camera.width;
    nas->Camera.height = Heroes[nas->UserInfo.user1].hero->Camera.height;
    nas->Camera.look_ahead = Heroes[nas->UserInfo.user1].hero->Camera.look_ahead;
    nas->Engine->unk10 = Heroes[nas->UserInfo.user1].hero_eng->unk10;
    if (nas->Engine->unk10.y < -0x900) {
        nas->Engine->unk10.y = -0x900;
    }
}

void InitDummyCamera(NAS *nas) {
    *(s32 *)((u8 *)nas->Engine + 0x14) = -0x480;
}

void NoxStubA(void) {
}

void NoxInitCamera(s32 arg0) {
    NoxCamera.sx = 0x1000;
    NoxCamera.sy = 0x1000;
    NoxCamera.sz = 0x1000;
    NoxCamera.unk8 = 0x8000;
    NoxCamera.unk18 = 0x140;
    NoxCamera.unk54 = 0;
    NoxCamera.unk56 = 0;
    NoxCamera.unk58 = 0;
    NoxCamera.x = 0;
    NoxCamera.y = 0;
    NoxCamera.z = 0;
    NoxCamera.unk95 = 0;
    NoxCamera.unk94 = 1;
    NoxCamera.unk0 = 0;
    NoxCamera.unk4 = 0;
    NoxCamera.unk8 = 0;
    NoxCamera.unk3C = 0;
    NoxCamera.unk38 = 0;
    NoxCamera.unk40 = 0;
    func_80093038();
    func_80018B08(&GameViewport, (GfxCamera *)&NoxCamera);
    RenderState.modelDp = ScreenFade.amount;
    func_8009440C();
}

void NoxStubB(void) {
}

s32 NoxLaserHitSphere(LaserPos p, s32 r) {
    LaserNode *n;
    LaserTgt *t;
    LaserExt *ext;
    NAS *nas;
    s32 dx, dy, dz;

    LaserPos *tp;
    s32 mode;

    n = (LaserNode *)NoxLasers;
loop:
    if (n != NULL) {
        t = n->tgt;
        if (t != NULL) {
            dx = t->pos.x - p.x;
            dy = t->pos.y - p.y;
            dz = t->pos.z - p.z;
            if (dx * dx + dy * dy + dz * dz < r) {
                tp = &t->pos;
                nas = CreateNAS(t->pos.x, tp->y, tp->z, 0x1605, 0x11);
                mode = 3;
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
                return 1;
            }
        }
        n = n->next;
        goto loop;
    }
    return 0;
}

void NoxMineSfx(void) {
    s32 temp_v0;

    temp_v0 = Random(4);
    if (temp_v0 == 0) {
        PlaySfx(0x28B, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 1) {
        PlaySfx(0x28C, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 2) {
        PlaySfx(0x28D, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 3) {
        PlaySfx(0x28E, 0, 0, 0x1000, 0x1E00, 0);
    }
}

void NoxResetHeroes(void) {
    NoxBossHits = 0;
    NoxHeroesDown = 0;
    NoxHeroTiltA = 0;
    NoxHeroTiltB = 0;
    NoxHeroTiltC = 0;
    NoxHeroTiltD = 0;
    Heroes->hero_eng->unk10.y = -0x480;
    NoxHeroesAlive = 1;
    Heroes[1].hero_eng->unk10.y = -0x480;
}

void NoxInit(void) {
    NAS *nas;
    u8 *arg;

    func_8001E0A8((u8 *)MapScene);
    NoxInitAsteroids();
    NoxUpdateAsteroids();
    nas = CreateNAS(0, 0, 0, 0x1625, 0x11);
    if (nas != NULL) {
        nas->UserInfo.user2 = 1;
    }
    NoxSpeedScale = 0x100;
    NoxBossDead = 0;
    NoxStarted = 0;
    RenderHook = (void (*)(void))NoxInitCamera;
    arg = *(u8 **)MapScene;
    NoxLasers = NULL;
    func_80095258(arg);
}

s32 NoxPointInBox(s32 *arg0, s32 *arg1) {
    Box2 *src;
    s32 d[3];
    BoxHalf lo;
    BoxHalf hi;

    src = (Box2 *)func_8008CE20((subengine *)arg0, 0, 0);
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

s32 NoxFadeLevel(void) {
    u8 *g = GameInfo;

    s32 r;

    if (g[0xCA] < 10) {
        r = 230;
    } else if (g[0xCA] < 30) {
        return 230 - (g[0xCA] - 10) * 179 / 20;
    } else {
        r = 51;
    }
    return r;
}

void NoxUpdateShadow(void) {
}

void NoxInitShadow(void) {
}

void PlotNitroSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

void InitNitroSmokeBoost(NAS *nas) {
    ScriptCursor *cur;

    cur = func_80024E30(nas, &NoxSmokeScriptInit);
    if (GameMode == 0x1F) {
        if (cur != NULL) {
            cur->unk1E = 4;
        }
    }
    nas->Engine->sub->unk6C = *(GfxModel **)MapScene;
    nas->UserInfo.user1 = 0;
}
