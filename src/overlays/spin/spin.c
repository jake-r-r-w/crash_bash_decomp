#include "common.h"
#include "bash/engine.h"
#include "bash/spin_ovl.h"
#include "bash/spin.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/bash.h"
#include "bash/xtras.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char SpinAnimBreathe[] = "BREATHE";
const char SpinAnimMedium[] = "MEDIUM";
const char SpinAnimHit[] = "HIT";
const char SpinAnimDie[] = "DIE";
const char SpinAnimJump[] = "JUMP";
const char SpinAnimAttack[] = "ATTACK";
const char SpinAnimFlip[] = "FLIP";
const char SpinAnimTauntA[] = "TAUNT_A";

/* data, in address order */
u16 SpinAiSkill[10] = {
    0x16, 0x13, 0x10, 0xD, 0x13, 0x16, 0x13, 0x10, 0x10, 0x17,
};
u16 SpinAiSkillAlt[10] = {
    0x17, 0x14, 0x11, 0xE, 0x14, 0x16, 0x15, 0x10, 0x10, 0x17,
};
u16 SpinAiTeamSkill[6] = {
    0xC, 0x14, 0x18, 0xC, 0x14, 0x18,
};
u16 SpinAiHumanRoundTweak[12] = {
    0x14, 0x19, 0x1D, 0x12, 0x17, 0x1B, 0x10, 0x15, 0x19, 0xE, 0x12, 0x16,
};
u16 SpinAiBotTweak[10] = {
    0xD, 0xD, 0xD, 0xD, 0x10, 3, 0xE, 0x10, 0x10, 3,
};
u16 SpinAiHumanTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x13, 0x10, 0x10, 0x20,
};
/* the shell settings (engine world record 0x0103) */
shell_setup spin_shell_info = {
    0x2000, 0x10, 0x1000, 0xC8, 0x2BC, 0x200, VramAreas, VramNodePool, 0x20, 0x400,
};
HeroJumpEnt SpinJumpEnts[10] = {
    { 0x88, 0x88, 9, 0xB },
    { 0x80, 0x80, 6, 0xB },
    { 0x80, 0x80, 6, 0xB },
    { 0x80, 0x80, 6, 0xB },
    { 0x80, 0x80, 6, 0xB },
    { 0x80, 0x80, 8, 0xD },
    { 0x80, 0x80, 8, 0xD },
    { 0x80, 0x80, 8, 0xD },
    { 0x80, 0x80, 8, 0xD },
    { 0, 0, 0, 0 },
};
u16 SpinRunSpeeds[30] = {
    0, 0, 0, 0x32, 0x32, 0x37, 0x32, 0x32, 0x37, 0x32, 0x32, 0x37, 0x32, 0x32, 0x37, 0x32, 0x32,
    0x37, 0x32, 0x32, 0x37, 0x32, 0x32, 0x37, 0x32, 0x32, 0x37, 0, 0, 0,
};
u16 SpinBalloonWeights[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 9, 8, 7, 6,
    5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u16 SpinHazardKinds[2] = { 0x17, 0x13 };
s16 SpinDropKinds[4] = { 0xB, 8, 9, 0xA };
u16 SpinBalloonModels[4] = { 0x2004, 0x2003, 0x2001, 0x2002 };
u16 SpinBalloonAnims[6] = { 3, 2, 0, 1, 4, 0 };
s32 SpinBalloonSfx0[4] = { 0x5DC, 0x5DD, 0x5DE, 0x5DF };
s32 SpinBalloonSfx1[4] = { 0x5E0, 0x5E1, 0x5E2, 0x5E3 };
s32 SpinBalloonSfx2[4] = { 0x5E4, 0x5E5, 0x5E6, 0x5E7 };
s32 SpinBalloonSfx3[4] = { 0x5E8, 0x5E9, 0x5EA, 0x5EB };
s32 *SpinBalloonSfx[4] = { SpinBalloonSfx0, SpinBalloonSfx1, SpinBalloonSfx2, SpinBalloonSfx3 };
hero_seq CRA_seq_long_hit[20] = {
    0, 0x8009, 3, 0x4180, 0x8003, 9, 0x8007, 0xA, 0x4181, 0x8003, 8, 0x8007, 4, 0x418B, 0x8018,
    0x16, 0x418F, 0x8004, 0, 0,
};
hero_seq CRA_SP_seq_die[10] = {
    0, 0x8007, 0x20, 0x4400, 0x8018, 0xC, 0x441F, 0x8004, 0x38, 0,
};
hero_seq CRA_seq_flip_jump[18] = {
    0, 0x800A, 0xC, 0x8007, 0x13, 0x4604, 0x8018, 2, 0x4617, 0x8018, 4, 0x4618, 0x8018, 0x28,
    0x4619, 0x8004, 7, 0,
};
hero_seq COC_seq_long_hit[20] = {
    0, 0x8009, 0x35, 0x4080, 0x8003, 9, 0x8007, 0xD, 0x4081, 0x8003, 8, 0x408E, 0x8007, 5, 0x408F,
    0x8018, 0x11, 0x4093, 0x8004, 0,
};
hero_seq COC_SP_seq_die[10] = {
    0, 0x8007, 0x15, 0x4580, 0x8018, 0x14, 0x4594, 0x8004, 0x38, 0,
};
hero_seq COC_seq_flip_jump[12] = {
    0, 0x800A, 0x3E, 0x8007, 0x1C, 0x4605, 0x8018, 0x28, 0x4621, 0x8004, 7, 0,
};
hero_seq BRO_seq_long_hit[20] = {
    0, 0x8009, 0x99, 0x4100, 0x8003, 9, 0x8007, 0xE, 0x4101, 0x8003, 8, 0x8007, 5, 0x410F, 0x8018,
    0x11, 0x4113, 0x8004, 0, 0,
};
hero_seq BRO_SP_seq_die[10] = {
    0, 0x8007, 0x23, 0x4400, 0x8018, 5, 0x4422, 0x8004, 0x38, 0,
};
hero_seq BRO_seq_flip_jump[12] = {
    0, 0x800A, 0xA2, 0x8007, 0x17, 0x4684, 0x8018, 0x28, 0x469B, 0x8004, 7, 0,
};
hero_seq CTX_seq_long_hit[20] = {
    0, 0x8009, 0x67, 0x4000, 0x8003, 9, 0x8007, 0x12, 0x4001, 0x8003, 8, 0x8007, 7, 0x4013, 0x8018,
    0xB, 0x4019, 0x8004, 0, 0,
};
hero_seq CTX_SP_seq_die[10] = {
    0, 0x8007, 0x15, 0x4580, 0x8018, 0xF, 0x4594, 0x8004, 0x38, 0,
};
hero_seq CTX_seq_flip_jump[12] = {
    0, 0x800A, 0x70, 0x8007, 0x17, 0x4604, 0x8018, 0x28, 0x461B, 0x8004, 7, 0,
};
hero_seq DNG_seq_long_hit[20] = {
    0, 0x8009, 0xFD, 0x4200, 0x8003, 9, 0x8007, 0xA, 0x4201, 0x8003, 8, 0x8007, 0x13, 0x420B,
    0x8018, 7, 0x421D, 0x8004, 0, 0,
};
hero_seq DNG_SP_seq_die[10] = {
    0, 0x8007, 0x1F, 0x4480, 0x8018, 0xD, 0x449E, 0x8004, 0x38, 0,
};
hero_seq DNG_seq_flip_jump[12] = {
    0, 0x800A, 0x106, 0x8007, 0x12, 0x4684, 0x8018, 0x28, 0x4696, 0x8004, 7, 0,
};
hero_seq TIN_seq_long_hit[20] = {
    0, 0x8009, 0x12F, 0x4080, 0x8003, 9, 0x8007, 0x12, 0x4081, 0x8003, 8, 0x8007, 0xB, 0x4093,
    0x8018, 7, 0x409D, 0x8004, 0, 0,
};
hero_seq TIN_SP_seq_die[10] = {
    0, 0x8007, 0x15, 0x4600, 0x8018, 0xF, 0x4614, 0x8004, 0x38, 0,
};
hero_seq TIN_seq_flip_jump[14] = {
    0, 0x800A, 0x138, 0x8007, 0x13, 0x4684, 0x8018, 2, 0x469A, 0x8018, 0x28, 0x469B, 0x8004, 7,
};
hero_seq KAN_seq_long_hit[20] = {
    0, 0x8009, 0xCB, 0x4200, 0x8003, 9, 0x8007, 0x12, 0x4201, 0x8003, 8, 0x8007, 8, 0x4212, 0x8018,
    9, 0x421A, 0x8004, 0, 0,
};
hero_seq KAN_SP_seq_die[10] = {
    0, 0x8007, 0x17, 0x4280, 0x8018, 0x17, 0x4296, 0x8004, 0x38, 0,
};
hero_seq KAN_seq_flip_jump[14] = {
    0, 0x800A, 0xD4, 0x8007, 0x16, 0x4684, 0x8018, 2, 0x469A, 0x8018, 0x28, 0x469B, 0x8004, 7,
};
hero_seq KON_seq_long_hit[20] = {
    0, 0x8009, 0x161, 0x4180, 0x8003, 9, 0x8007, 0x12, 0x4181, 0x8003, 8, 0x8007, 9, 0x4193, 0x8018,
    9, 0x419C, 0x8004, 0, 0,
};
hero_seq KON_SP_seq_die[10] = {
    0, 0x8007, 0x19, 0x4200, 0x8018, 0xB, 0x4218, 0x8004, 0x38, 0,
};
hero_seq KON_seq_flip_jump[14] = {
    0, 0x800A, 0x16A, 0x8007, 0x18, 0x4684, 0x8018, 2, 0x469C, 0x8018, 0x28, 0x469D, 0x8004, 7,
};
s16 SpinPreLoadDB[6] = { 0x1C, 0x17, 0x18, 0x19, -1, 0 };
char *SpinAnims[9] = {
    (char *)SpinAnimBreathe, (char *)SpinAnimMedium, (char *)SpinAnimHit, (char *)SpinAnimDie,
    (char *)SpinAnimJump, (char *)SpinAnimAttack, (char *)SpinAnimFlip, (char *)SpinAnimTauntA,
    NULL,
};
s16 SpinLevelDB0[2] = { -1, 0 };
s16 SpinLevelDB1[2] = { 0x1A, -1 };
WWInf SpinWorldInfo[2] = {
    { 0x7D, SpinAnims, SpinLevelDB0, 7 },
    { 0x7E, SpinAnims, SpinLevelDB1, 7 },
};
s32 SpinFiles[21] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 363], (s32)&DatIndex[2 * 362], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 359],
    (s32)&DatIndex[2 * 358], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 361], (s32)&DatIndex[2 * 360], (s32)&HeroModelBank[DB_Powerups],
    (s32)&DatIndex[2 * 357], (s32)&DatIndex[2 * 356], (s32)&HeroModelBank[DB_CrateObjects], (s32)&DatIndex[2 * 355], (s32)&DatIndex[2 * 354],
    (s32)&HeroModelBank[DB_RingObjects], (s32)&DatIndex[2 * 353], (s32)&DatIndex[2 * 352], 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 0x1B's) */
file_group_load_info file_group_RING = { &FileGroups[10], SpinFiles };
u16 SpinLUPtab[4] = { 3, 0x201, 0x20D, 0x201 };
TrigLookUp SpinLUP = { SpinLUPtab, 4 };
PreMap SPIN_PREMAP = { SpinPreLoadDB, SpinWorldInfo, &SpinLUP };
SpinSlot SpinSlots[10] = {
    { 0, { 0, 0, 0 }, { 0, 0x14, 0 } }, { 0, { 0, 0, 0 }, { 0, 0x14, 0 } },
    { 0, { 0, 0x300, 0 }, { 0, 0, 5 } }, { 0, { 0, 0, 0 }, { 0, 0, 0xA } },
    { 0, { 0, 0, 0 }, { 0, 0, -0xF } }, { 0, { 0, 0, 0 }, { 0, 0, 0 } },
    { 0, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, { 0, 0, 0 }, { 0, 0, 0 } },
    { 0, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, { 0, 0, 0 }, { 0, 0, 0 } },
};
s16 SpinUnused0[1] = {0};
u16 SpinUnused1 = 0;
u16 SpinDropTimer = 0;
u16 SpinDropDelay = 0;
u16 SpinBigDrop = 0;
u16 SpinSlotPos = 0;
u16 SpinDropCount = 0;
u16 SpinDropPos = 0;
s16 SpinUnused2[1] = {0};
u16 SpinBalloonOwner = 0;
u16 SpinBalloonTimer = 0;
s32 SpinBalloonFlag = 0;
EXTengine SpinSfxEngine = {0};
s32 SpinBestScore = 0;
s32 SpinBestHero = 0;
SpinCell SpinDropPoints[4][16] = {0};
s32 SpinTurn = 0;
s32 SpinTurnTarget = 0;
s16 SpinAngle = 0;
s16 SpinAngle2 = 0;
s16 SpinSpeed = 0;
SpinObj *SpinArenaObjs[10] = {0};
s32 SpinASTBase = 0;
u16 SpinShadowModels[4] = { 0x2013, 0x2012, 0x2010, 0x2011 };
u16 SpinHitSfx[4] = { 0x446, 0x447, 0x448, 0x449 };
u16 SpinHitDone[4] = {0};

void InitialiseSPINFirst() {
    void *p;
    void **q;

    HeroHooks.PreMoveHero = SpinPreMoveHero;
    HeroHooks.KeepInBounds = SPIN_KeepInBounds;
    HeroHooks.TestJump = SpinHeroTestFlip;
    HeroHooks.TestAttack = SpinNoAttack;
    HeroHooks.RunSpeeds = SpinRunSpeeds;
    HeroHooks.HeroBot = Spin_HeroBot;
    HeroHooks.ChooseWinner = SpinChooseWinner;
    HeroHooks.Hero2ObjectCollision = SpinHero2ObjectCollision;
    HeroHooks.HeroGrabObject = SpinHeroGrabObject;
    HeroHooks.HeroKickCollision = SpinHeroKickCollision;
    HeroHooks.HeroCarryCollision = SpinHeroCarryCollision;
    HeroHooks.HeroExtraEffects = SpinHero_ExtraEffects;
    HeroHooks.HeroesTouched = SpinTouched;
    HeroHooks.CreateShadow = SpinCreateShadow;
    HeroHooks.JumpEnts = (HeroJumpEnt *)SpinJumpEnts;
    HeroActDead = 0x38;
    HeroActWinner = 0x36;
    HeroActLoser = 0x37;
    HeroActHit = 0x33;
    SpinInitASTIndices();
    AiBotTweakTable = SpinAiBotTweak;
    AiHumanTweakTable = SpinAiHumanTweak;
    AiHumanRoundTweakTable = SpinAiHumanRoundTweak;
    q = &AiSkillTable;
    if (GameTeams[0] != 0) {
        p = SpinAiSkillAlt;
    } else {
        p = SpinAiSkill;
    }
    *q = p;
    AiTeamSkillTable = SpinAiTeamSkill;
}

void InitialiseSPIN() {
    s32 i;
    EXTheromove *m;
    s16 a;

    if (GameLevelFlags & 0x40) {
        Init_Clock(0x140, 0x28, 0);
        Set_Clock(0x125, 0x81D0);
    } else {
        Init_Clock(0x140, 0x28, 0);
        Set_Clock(0x130, 0x81D0);
    }
    World_floor_level = -0x10;
    SpinUnused1 = 0x1F4;
    SpinDropTimer = 0x12C;
    SpinDropDelay = 0x190;
    SpinBigDrop = 0x1E;
    SpinSlotPos = 0;
    SpinDropCount = 0;
    SpinDropPos = 0;
    SpinBalloonOwner = 0;
    SpinBalloonTimer = 0;
    SpinBalloonFlag = 0;
    SpinSfxEngine = EngineTemplate;
    for (i = 0; i < 4; i++) {
        a = (get_angle(Heroes[i].hero_eng->XYZ.x, Heroes[i].hero_eng->XYZ.z) + 0x800) & 0xFFF;
        m = Heroes[i].hero_move;
        Heroes[i].hero_eng->xform.rot.vy = a;
        m->facing_angle = a;
        m->forward_angle = a;
        Heroes[i].hero->Camera.width = 0;
        Heroes[i].hero->Camera.look_ahead = 0;
        if (((GameState *)GameInfo)->heroOut[i] != 0) {
            Heroes[i].hero_move->run_walk = 2;
        }
    }
    if (((GameState *)GameInfo)->levelFlags & 0x80) {
        dbase_load_all_mesh_anims(&HeroModelBank[DB_Angel]);
    }
    if (((GameState *)GameInfo)->levelFlags & 0x40) {
        if (((GameState *)GameInfo)->teams != 0) {
            Heroes[0].hero_score = 0x28;
            Heroes[1].hero_score = 0;
            Heroes[2].hero_score = 0x1E;
            Heroes[3].hero_score = 0;
            InitSharedScoreIcons();
        } else {
            Heroes[0].hero_score = 0x1E;
            Heroes[1].hero_score = 0x19;
            Heroes[2].hero_score = 0x19;
            Heroes[3].hero_score = 0x19;
        }
    }
    SpinShuffleDropPoints();
    MarkHeroesOnGrid();
    SpinInitScene();
    PlaySfx(0x5EC, &SpinSfxEngine.sfx, NULL, 0x1000, 0x1E00, 0);
}

void SpinShuffleDropPoints(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 y;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    SpinCell *p;
    SpinCell *base;
    s32 t0;
    s32 t1;

    i = 0;
    base = SpinDropPoints[0];
    y = 0x500;
    for (; i < 4; i++) {
        j = 0;
        d = i << 7;
        for (; j < 16; j++) {
            t0 = j << 8;
            p = (SpinCell *)(d + (s32)base);
            p->x = t0;
            p->y = y;
            d += 8;
        }
        y += 0x140;
    }
    for (k = 0; k < 300; k++) {
        i = Random(4);
        j = Random(16);
        c = Random(4);
        d = Random(16);
        t0 = SpinDropPoints[i][j].x;
        t1 = SpinDropPoints[i][j].y;
        SpinDropPoints[i][j].x = SpinDropPoints[c][d].x;
        SpinDropPoints[i][j].y = SpinDropPoints[c][d].y;
        SpinDropPoints[c][d].x = t0;
        SpinDropPoints[c][d].y = t1;
    }
}

void SpinInitScene(void) {
    s32 i;
    SpinObj *obj;
    s16 *p;

    SpinTurn = 0;
    SpinTurnTarget = 0;
    SpinSpeed = 1;
    SpinAngle = 0;
    SpinAngle2 = 0;
    obj = ((SpinObj *)MapScene->subs);
    for (i = 9; i >= 0; i--) {
        SpinArenaObjs[i] = NULL;
    }
    for (; obj != NULL; obj = obj->next) {
        p = (s16 *)model_entry_data(obj->model, obj->index);
        i = *p - 10;
        if ((u32)i < 10) {
            SpinArenaObjs[i] = obj;
            obj->unk51 = 0;
        }
        i = *p - 20;
        if ((u32)i < 10) {
            SpinSlots[i].obj = obj;
            obj->unk51 = 0;
        }
    }
}

void SpinTurnArena(void) {
    SpinObj *obj;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *e;
    NAS *n;
    EXTobject *f;
    s32 i;
    s32 x;
    s32 z;
    s32 d;
    s32 a;

    if (GameExitFlags & 0x801) {
        SpinSpeed = 0;
    }
    a = SpinAngle - SpinSpeed * 13;
    if (a != 0) {
        if (a < 0) {
            SpinAngle++;
        } else {
            SpinAngle--;
        }
    }
    a = SpinAngle2 - SpinSpeed * 24;
    if (a != 0) {
        if (a < 0) {
            SpinAngle2++;
        } else {
            SpinAngle2--;
        }
    }
    SpinTurn += SpinAngle;
    SpinTurnTarget -= SpinAngle2;
    obj = SpinArenaObjs[0];
    if (obj != NULL) {
        obj->dirty = 1;
        obj->ry = SpinTurn;
    }
    obj = SpinArenaObjs[1];
    if (obj != NULL) {
        obj->dirty = 1;
        obj->ry = SpinTurn;
    }
    for (i = 0; i < 10; i++) {
        obj = SpinSlots[i].obj;
        if (obj != NULL) {
            if (SpinSlots[i].fixed[0] != 0) {
                obj->rx = SpinSlots[i].fixed[0];
            } else {
                obj->rx = (u32)(SpinTurn * SpinSlots[i].rate[0]) >> 4;
            }
            if (SpinSlots[i].fixed[1] != 0) {
                obj->ry = SpinSlots[i].fixed[1];
            } else {
                obj->ry = (u32)(SpinTurn * SpinSlots[i].rate[1]) >> 4;
            }
            if (SpinSlots[i].fixed[2] != 0) {
                obj->rz = SpinSlots[i].fixed[2];
            } else {
                obj->rz = (u32)(SpinTurn * SpinSlots[i].rate[2]) >> 4;
            }
            obj->dirty = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_status & 0xFA00) continue;
        eng = h->hero_eng;
        if (eng->XYZ.y < World_floor_level) continue;
        if (h->hero_pu == 0x13) continue;
        x = eng->XYZ.x;
        z = eng->XYZ.z;
        d = x * x + z * z;
        if (d > 0x640000) continue;
        if (d <= 0xC3FFF) continue;
        d = (csqrt(d) + 0x20) >> 6;
        a = get_angle(x, z) + SpinAngle;
        a &= 0xFFF;
        h->hero_eng->XYZ.x = d * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12;
        h->hero_eng->XYZ.z = d * rcossin_tbl[a].unk2 >> 12;
        h->hero_move->forward_angle += SpinAngle;
        h->hero_move->forward_angle &= 0xFFF;
    }
    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        f = (EXTobject *)n->FUX;
        if (f == NULL) continue;
        e = n->Engine;
        if (e == NULL) continue;
        x = e->XYZ.x;
        z = e->XYZ.z;
        d = x * x + z * z;
        if (d > 0x640000) continue;
        if (e->XYZ.y < World_floor_level) continue;
        if (d <= 0xC3FFF) continue;
        d = f->coord.dist;
        a = f->coord.angle;
        a += SpinAngle;
        a &= 0xFFF;
        f->coord.angle = a;
        e->XYZ.x = d * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12;
        e->XYZ.z = d * rcossin_tbl[a].unk2 >> 12;
        e->xform.rot.vy = (e->xform.rot.vy + SpinAngle) & 0xFFF;
        e->xform.dirty = 1;
    }
}

s32 SpinRankHeroes(void) {
    GameState *g;
    HeroInfo *h;
    s32 j;
    s32 i;
    GameState *gt;
    s32 a;
    s32 b;
    s32 mate;
    s32 score;
    s32 team0;
    s32 team1;
    s32 mate1;
    s32 score1;
    u8 *p;
    s32 k;
    s32 m;

    j = 3;
    p = GameInfo + j;
    do {
        p[0xA7] = j;
        j--;
        p--;
    } while (j >= 0);
    i = 0;
    g = (GameState *)GameInfo;
    h = Heroes;
    while (1) {
        if (!(i < 4)) break;
        j = 0;
        while (1) {
            if (!(j < 3)) break;
            if (g->levelFlags & 0x40) {
                if (h[g->adventure.rank[j]].hero_score > h[((u8 *)((k = j + 1) + (s32)g))[0xA7]].hero_score) {
                    a = g->adventure.rank[j];
                    g->adventure.rank[j] = g->adventure.rank[k];
                    g->adventure.rank[k] = a;
                }
            } else {
                if (h[g->adventure.rank[j]].hero_score < h[((u8 *)((m = j + 1) + (s32)g))[0xA7]].hero_score) {
                    a = g->adventure.rank[j];
                    g->adventure.rank[j] = g->adventure.rank[m];
                    g->adventure.rank[m] = a;
                }
            }
            j++;
        }
        i++;
    }
    gt = (GameState *)GameInfo;
    if (gt->teams != 0) {
        team0 = gt->teamLeader[0];
        mate = Heroes[team0].hero_teammate;
        score = Heroes[team0].hero_score;
        SpinBestScore = score;
        if (mate != 0) {
            SpinBestScore = score + Heroes[mate].hero_score;
        }
        team1 = gt->teamLeader[1];
        mate1 = Heroes[team1].hero_teammate;
        score1 = Heroes[team1].hero_score;
        SpinBestHero = score1;
        if (mate1 != 0) {
            SpinBestHero = score1 + Heroes[mate1].hero_score;
        }
        if (gt->levelFlags & 0x40) {
            if (SpinBestScore < SpinBestHero) {
            ra:
                return team0;
            }
        rb:
            return team1;
        }
        if (SpinBestScore > SpinBestHero) {
            goto ra;
        }
        goto rb;
    }
    return gt->adventure.rank[0];
}

void SpinChooseWinner(s32 arg0, u16 arg1) {
    s32 found;
    GameState *g;
    GameState *game;
    HeroInfo *hb;
    HeroInfo *heroes;
    HeroInfo *h;
    GameState *p;
    s32 i;
    s32 mate;

    if (SPIN_GAME->exitFlags & 0x801) {
        return;
    }
    found = 0;
    if (SPIN_GAME->levelFlags & 0x80) {
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00) && (i == 0 || (SPIN_GAME->teams != 0 && i == 1))) {
                found++;
            }
        }
        if (found == 0) {
            SPIN_DRAW(SPIN_GAME, arg1, i);
            return;
        }
    }
    if (SPIN_GAME->levelFlags & 0x40) {
        if (arg0 == -1) {
            SPIN_DRAW(SPIN_GAME, arg1, i);
            return;
        }
        found = -1;
        if (SPIN_GAME->teams != 0) {
            if (Heroes[SPIN_GAME->teamLeader[0]].hero_score == 0) {
                found = SPIN_GAME->teamLeader[0];
            }
            if (Heroes[SPIN_GAME->teamLeader[1]].hero_score == 0) {
                found = SPIN_GAME->teamLeader[1];
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (Heroes[i].hero_score == 0) {
                    found = i;
                }
            }
        }
        if (found != -1) {
            if (found == 0 || (SPIN_GAME->teams != 0 && found == 1)) {
                Heroes[found].hero_status |= 0x200;
                if (SPIN_GAME->teams != 0 && (mate = Heroes[found].hero_teammate) != -1) {
                    Heroes[mate].hero_status |= 0x200;
                }
                SPIN_GAME->winner[0] = Heroes[found].hero_name;
                SPIN_GAME->exitFlags |= arg1;
                for (i = 0; i < 4; i++) {
                    if (!(Heroes[i].hero_status & 0xFA00)) {
                        Heroes[i].hero_status |= 0x400;
                    }
                }
                return;
            }
            SPIN_DRAW(SPIN_GAME, arg1, i);
            return;
        }
    }
    if (arg0 != -1) {
        return;
    }
    found = SpinRankHeroes();
    g = (GameState *)GameInfo;
    if ((g->levelFlags & 0xE0) && found != 0) {
        if (g->teams != 0) {
            if (found == 1) {
                goto decide;
            }
            InitBigScore();
        }
        SPIN_DRAW(g, arg1, i);
        return;
    }
decide:
    game = (GameState *)GameInfo;
    if (game->teams != 0) {
        InitBigScore();
        if (SpinBestScore == SpinBestHero) {
            game->winner[0] = 0;
            game->exitFlags |= arg1;
            for (i = 0; i < 4; i++) {
                Heroes[i].hero_status |= 0x100;
            }
            return;
        }
        game->exitFlags |= 0x800;
        if (found == game->teamLeader[0]) {
            hb = Heroes;
            h = &hb[found];
            mate = h->hero_teammate;
            if (h->hero_status & 0xFA00) {
                game->winner[0] = mate;
                hb[mate].hero_status |= 0x200;
            } else if (hb[mate].hero_status & 0xFA00) {
                game->winner[0] = h->hero_name;
                h->hero_status |= 0x200;
            } else {
                game->winner[0] = h->hero_name;
                h->hero_status |= 0x200;
                game->winner[1] = hb[mate].hero_name;
                hb[mate].hero_status |= 0x200;
            }
            Heroes[SPIN_GAME->teamLeader[1]].hero_status |= 0x400;
            mate = Heroes[SPIN_GAME->teamLeader[1]].hero_teammate;
            if (mate != 0) {
                Heroes[mate].hero_status |= 0x400;
            }
        } else {
            game->winner[0] = Heroes[game->teamLeader[1]].hero_name;
            Heroes[game->teamLeader[1]].hero_status |= 0x200;
            mate = Heroes[game->teamLeader[1]].hero_teammate;
            if (mate != 0) {
                game->winner[1] = Heroes[mate].hero_name;
                Heroes[mate].hero_status |= 0x200;
            }
            Heroes[game->teamLeader[0]].hero_status |= 0x400;
            mate = Heroes[game->teamLeader[0]].hero_teammate;
            if (mate != 0) {
                Heroes[mate].hero_status |= 0x400;
            }
        }
    } else {
        heroes = Heroes;
        h = &heroes[game->adventure.rank[0]];
        if (h->hero_score == heroes[game->adventure.rank[1]].hero_score) {
            game->winner[0] = 0;
            game->exitFlags |= arg1;
            for (i = 0; i < 4; i++) {
                Heroes[i].hero_status |= 0x100;
            }
            return;
        }
        p = game;
        p->winner[0] = h->hero_name;
        p->exitFlags |= 0x800;
        for (i = 0; i < 4; i++) {
            if (p->adventure.rank[0] == i) {
                heroes[i].hero_status |= 0x200;
            } else {
                heroes[i].hero_status |= 0x400;
            }
        }
    }
}

void SpinDropMaster(void) {
    s32 i;
    s32 hero;
    s32 max;
    s32 d;

    if (GameExitFlags & 0x801) {
        return;
    }
    if (SpinBalloonTimer != 0) {
        SpinBalloonTimer--;
    }
    hero = -1;
    max = 8;
    i = 0;
    do {
        if (Heroes[i].hero_pu == 0x17) {
            hero = i;
            if (SpinBigDrop >= 0x1F) {
                SpinBigDrop = 0x1E;
            }
            max = 10;
        }
        i++;
    } while (i < 4);
    if (CrateCounts[2] == 0 && hero == -1) {
        if (SpinDropTimer != 0) {
            SpinDropTimer--;
        } else {
            SpinDropObject(1, 2);
            SpinDropTimer = Random(0x3C) + 0x5A;
        }
    }
    if (CrateCounts[12] == 0) {
        if (SpinDropDelay != 0) {
            SpinDropDelay--;
        } else {
            SpinDropObjects(1, 0xC);
            SpinDropTimer = Random(0x3C) + 0x5A;
        }
    }
    SpinDropCount = CrateCounts[8] + CrateCounts[9] + CrateCounts[10] + CrateCounts[11];
    if (SpinBigDrop != 0) {
        SpinBigDrop--;
    } else if (SpinDropCount < max) {
        d = max - SpinDropCount;
        if (d < 4) {
            i = Random(d) + 1;
            SpinBigDrop = Random(0x14) + 0x1E;
        } else {
            i = 4;
            SpinBigDrop = Random(0xA) + 0x14;
        }
        if (hero != -1) {
            SpinDropObjects(i, SpinDropKinds[hero]);
        } else {
            SpinDropObjects(i, -1);
        }
    } else {
        SpinBigDrop = Random(0x1E) + 0x3C;
    }
}


void SpinCountObjects(void) {
    s32 i;
    u16 *p;
    s32 j;
    NAS *n;
    EXTengine *e;
    EXTobject *f;
    s32 t;
    s32 x0, x1, z0, z1;

    i = 12;
    p = CrateCounts;
    p += 12;
    do {
        *p = 0;
        i--;
        p--;
    } while (i >= 0);
    for (j = 12; j >= 0; j--) {
        PickupCounts[j] = 0;
    }
    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        e = n->Engine;
        if (e == NULL) continue;
        f = (EXTobject *)n->FUX;
        if (f == NULL) continue;
        t = f->whatami;
        x0 = ((e->XYZ.x - 0x80) >> 8) + 10;
        x1 = ((e->XYZ.x + 0x80) >> 8) + 10;
        z0 = 9 - ((e->XYZ.z - 0x80) >> 8);
        z1 = 9 - ((e->XYZ.z + 0x80) >> 8);
        if ((u32)x0 < 20) {
            if ((u32)z0 < 20) {
                ArenaGrid[z0 * 20 + x0].flags |= t;
            }
            if ((u32)z1 < 20) {
                ArenaGrid[z1 * 20 + x0].flags |= t;
            }
        }
        if ((u32)x1 < 20) {
            if ((u32)z0 < 20) {
                ArenaGrid[z0 * 20 + x1].flags |= t;
            }
            if ((u32)z1 < 20) {
                ArenaGrid[z1 * 20 + x1].flags |= t;
            }
        }
        if ((u16)(f->whatami - 1) < 12) {
            switch (f->status) {
            case 0:
            case 1:
            case 3:
            case 6:
            case 7:
                CrateCounts[0]++;
                CrateCounts[f->whatami]++;
                break;
            }
        }
    }
}

void SpinDropObjects(u16 count, s16 arg1) {
    SpinPair *p;
    s32 a;
    s32 b;
    s32 i;
    s16 id;
    u16 kind;
    u16 idx;
    s32 n;
    s32 t;
    s32 r;

    id = arg1;
    if (arg1 > 0) {
        p = SpinFindDropPoint(&SpinDropPos);
        if (p != NULL) {
            CreateBalloon(p->a, p->b, id, -1);
        }
    } else if (count != 0) {
        i = 0;
        do {
            p = SpinFindDropPoint(&SpinDropPos);
            if (p == NULL) {
                continue;
            }
            a = p->a;
            idx = SpinNextSlot();
            t = SpinDropKinds[idx];
            b = p->b;
            kind = CrateCounts[t];
            id = t;
            if (kind >= 2) {
                continue;
            }
            if (((GameState *)GameInfo)->heroOut[idx] == 0
                && (n = AiPlayerSkill[idx].level, n++, (u32)Clock_Time >> 8 < 0x120)) {
                if (kind == 0) {
                    r = (u32)Random(n) < 9;
                } else {
                    r = (u32)Random(n) < 7;
                }
                if (!r) {
                    break;
                }
            }
            CreateBalloon(a, b, id, -1);
        } while (++i < count);
    }
}

SpinPair *SpinFindDropPoint(u16 *head) {
    SpinCell *p;
    SpinCell *bestp;
    s32 i;
    u16 besti;
    u16 best;
    u16 tries;
    u16 n;
    s32 x;
    s32 z;
    s32 a;
    s32 b;

    p = SpinDropPoints[0];
    best = 27;
    tries = 9;
    i = *head;
    bestp = p = p + i;
    besti = i++;
    i &= 63;
    x = (p->x >> 8) + 10;
    z = 9 - (p->y >> 8);
    do {
        while (ArenaGrid[z * 20 + x].flags != 0) {
            if (i == *head) {
                goto fail;
            }
            p = SpinDropPoints[0];
            p += i++;
            x = (p->x >> 8) + 10;
            z = 9 - (p->y >> 8);
            i &= 63;
        }
        n = 0;
        for (a = x - 3; a < x + 3; a++) {
            for (b = z - 3; b < z + 3; b++) {
                if ((u32)a < 20 && b >= 0 && b < 20 && ArenaGrid[b * 20 + a].flags != 0) {
                    n++;
                }
            }
        }
        if (n == 0) {
            *head = i;
            return (SpinPair *)p;
        }
        if (n < best) {
            best = n;
            bestp = p;
            besti = i;
        }
        if (i == *head) {
        fail:
            return NULL;
        }
        p = SpinDropPoints[0];
        p += i++;
        i &= 63;
        x = (p->x >> 8) + 10;
        z = 9 - (p->y >> 8);
    } while (tries-- != 0);
    *head = besti;
    return (SpinPair *)bestp;
}

NAS *CreateHazard(s32 a, s32 b, u16 c, s16 d)
{
  NAS *nas;
  EXTengine *eng;
  EXTobject *fux;
  s32 x;
  EXTobject *new_var;
  s32 z;
  u16 mask;
  x = (b * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
  z = (b * rcossin_tbl[a & 0xFFF].unk2) >> 12;
  nas = CreateNAS(x, World_floor_level, z, 0x1801, 0x11);
  if (nas != 0)
  {
    eng = nas->Engine;
    if (eng != 0)
    {
      fux = (EXTobject *) AddObject(nas);
      if (fux != 0)
      {
        fux->coord.angle = a;
        fux->coord.dist = b;
        if (AddMotion(nas) != 0)
        {
          LinkObject(nas);
          AddSolid(nas, 0x300);
          nas->FUX = (EXT *) fux;
          eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
          eng->xform.order = 1;
          fux->whatami = c;
          CrateCounts[fux->whatami]++;
          if (d >= 0)
          {
            fux->status = d;
          }
          else
          {
            fux->status = 6;
          }
          switch (c)
          {
            case 2:
              eng->sub->entity = 0x2014;
              fux->contains = SpinHazardKinds[Random(2)];
              break;

            case 5:
            {

              eng->sub->entity = 0x2009;
            }
              z = 0x5A;
              fux->countdown = z;
              new_var = fux;
              mask = new_var->can_do;
              goto set_mask;


            case 6:
            {

              eng->sub->entity = 0x2001;
            }
              mask = fux->can_do;
              set_mask:
            fux->can_do = mask | 0x100;

              break;


            case 4:

            default:
              eng->sub->entity = 0x2013;
              break;

          }

          switch (fux->status)
          {
            case 3:
              eng->XYZ.y = World_floor_level;
              break;

            case 7:
              eng->xform.scale.vx = 0x40;
              eng->xform.scale.vy = 0x40;
              eng->xform.scale.vz = 0x40;
              eng->xform.order = 1;
              eng->xform.hasScale = 1;
              eng->xform.dirty = 1;
              eng->flags |= 1;
              eng->XYZ.y = World_floor_level;
              PlaySfx(0x3FA, 0, &eng->XYZ, 0x1000, 0x1E00, 0);
              break;

            case 6:
              eng->xform.scale.vy = 0x40;
              eng->xform.order = 1;
              eng->xform.hasScale = 1;
              eng->xform.dirty = 1;
              eng->XYZ.y = World_floor_level;
              PlaySfx(0x3FA, 0, &eng->XYZ, 0x1000, 0x1E00, 0);
              break;

          }

        }
      }
    }
    if (nas->Failure != 0)
    {
      KillNAS(nas);
      return 0;
    }
  }
  return nas;
}

void PlotHazardType(NAS *nas) {
    EXTengine *eng;
    EXTobject *fux;
    s32 kind;
    SpinGlow *glow;
    s32 v;
    s16 owner;
    HeroInfo *h;
    HeroInfo *hb;
    s32 a;
    s32 b;
    s16 w;
    s16 o;
    u16 t;
    u32 r;
    s32 done;
    VECTOR pos;
    EXTengine *he;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    HeroHooks.KeepInBounds(eng);
    fux = (EXTobject *)nas->FUX;
    if (fux == NULL) {
        return;
    }
    kind = fux->whatami;
    glow = FindExtension(nas, 0x800);
    if (glow == NULL) {
        return;
    }
    switch (fux->status) {
    case 0:
        fux->status = 1;
        fux->can_do = (fux->can_do & 0xFFD0) | 0xA;
        eng->xform.dirty = 1;
        fux->player = -1;
        FetchDefaultColBox(eng, 0x50);
        break;
    case 6:
        v = eng->xform.scale.vy;
        v += 0x200;
        eng->xform.rot.vy = 0;
        if (v >= 0x1000) {
            v = 0x1000;
            fux->status = 0;
        }
        eng->xform.scale.vy = v;
        eng->xform.dirty = 1;
        break;
    case 7:
        v = eng->xform.scale.vx;
        v += 0x100;
        if (v >= 0x1000) {
            v = 0x1000;
            fux->status = 0;
            eng->flags &= ~0x21;
        }
        eng->xform.scale.vx = v;
        eng->xform.scale.vy = v;
        eng->xform.scale.vz = v;
        eng->sub->unk76 = v;
        eng->xform.dirty = 1;
        break;
    case 9:
        switch (kind) {
        case 2:
            SpinDropHazard(fux);
            PlayHitEffect((s32 *)&eng->XYZ, fux, 0);
            fux->status = 0xC;
            break;
        case 6:
            SpinDropHazard(fux);
            PlayHitEffect((s32 *)&eng->XYZ, fux, 0);
            KillTriggerNas(nas);
            break;
        case 5:
            fux->countdown = 0x5A;
            fux->whatami = 7;
            PlaySfx(0x3F0, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            break;
        default:
            fux->status = 0;
            break;
        }
        break;
    case 3:
        fux->status = 0;
        break;
    case 12:
        fux->countdown = 0x78;
        fux->can_do &= 0xFFF4;
        eng->sub->u6C.dbase = &HeroModelBank[DB_RingObjects];
        glow->unk18 = 0x60;
        glow->unk1E = 0xF0;
        glow->unk1C = 4;
        glow->unk1A = 0;
        glow->unkC = 0xFF;
        eng->xform.rot.vx = 0;
        eng->xform.rot.vz = 0;
        owner = fux->player;
        if (owner != -1) {
            a = AiPlayerSkill[owner].level;
            b = 0x20 - a;
            if (GAME->heroOut[owner] != 0) {
                w = a;
                o = b;
            } else {
                w = b;
                o = a;
            }
            t = w + o;
            if (t == 0) {
                return;
            }
            r = (u16)Random(t);
            if ((u16)w < r) {
                fux->contains = 0x13;
            } else {
                fux->contains = 0x17;
            }
        }
        switch (fux->contains) {
        case 0x17:
            eng->sub->entity = 0x2015;
            fux->status = 0xD;
            break;
        case 0x13:
            eng->sub->entity = 0x2014;
            fux->status = 0xD;
            break;
        }
        break;
    case 13:
        if (HeroHooks.KeepInBounds(eng) != 0) {
            KillTriggerNas(nas);
            break;
        }
        done = 0;
        owner = fux->player;
        if (owner != -1) {
            if (glow->unkC == 0) {
                s32 dx;
                s32 dy;
                s32 dz;

                fux->can_do |= 8;
                hb = Heroes;
                h = &hb[owner];
                if (sub_find_point(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
                    dx = eng->XYZ.x - pos.vx;
                    dy = eng->XYZ.y - pos.vy;
                    dz = eng->XYZ.z - pos.vz;
                } else {
                    s32 ty;

                    dx = eng->XYZ.x - hb[fux->player].hero_eng->XYZ.x;
                    ty = eng->XYZ.y + 0x180;
                    dy = ty - hb[fux->player].hero_eng->XYZ.y;
                    dz = eng->XYZ.z - hb[fux->player].hero_eng->XYZ.z;
                }
                dx >>= 2;
                dy >>= 2;
                dz >>= 2;
                eng->XYZ.x -= dx;
                eng->XYZ.y -= dy;
                eng->XYZ.z -= dz;
                dx >>= 1;
                dz >>= 1;
                if (dx == 0 && dz == 0) {
                    done = 1;
                }
                SpinHitHeroes(eng, (SpinHitMask *)fux, 8, fux->player);
                if (fux->will_do & 8) {
                    done = 1;
                }
            }
        } else {
            KillNAS(nas);
            break;
        }
        if (GravitateNas(nas) != 0) {
            done = 1;
        }
        if (done) {
            owner = fux->player;
            if (owner != -1 && owner < 5) {
                Heroes[owner].hero_pu = fux->contains;
                switch (fux->contains) {
                case 0x17:
                    he = Heroes[fux->player].hero_powerup;
                    PlaySfx(0x5F2, &he->sfx, &he->XYZ, 0x1000, 0x1E00, 0);
                    Heroes[fux->player].hero_pu_timer = 0x12C;
                    Heroes[fux->player].hero_powerup->sub->entity = 0x2015;
                    Heroes[fux->player].hero_powerup->sub->u6C.dbase = &HeroModelBank[DB_RingObjects];
                    break;
                case 0x13:
                    he = Heroes[fux->player].hero_powerup;
                    PlaySfx(0x5F3, &he->sfx, &he->XYZ, 0x1000, 0x1E00, 0);
                    Heroes[fux->player].hero_pu_timer = 0xF0;
                    Heroes[fux->player].hero_powerup->sub->entity = 0x2014;
                    Heroes[fux->player].hero_powerup->sub->u6C.dbase = &HeroModelBank[DB_RingObjects];
                    PlaySfx(0x41C, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                    break;
                }
            }
            KillTriggerNas(nas);
        }
        eng->xform.dirty = 1;
        eng->xform.rot.vy += 0x71;
        break;
    default:
        if (fux->will_do & 2) {
            if (fux->player != -1) {
                fux->status = 9;
                fux->will_do &= 0xFFF4;
                fux->can_do &= 0xFFF4;
            } else {
                fux->will_do &= 0xFFFC;
            }
        }
        if ((fux->will_do & 8) && kind == 6) {
            PlayHitEffect((s32 *)&eng->XYZ, fux, 0);
            KillTriggerNas(nas);
        }
        break;
    }
}

NAS *CreateBalloon(s32 a, s32 b, u16 c, s16 d) {
    NAS *nas;
    EXTengine *eng;
    EXTengine *shadow;
    EXTobject *fux;
    EXTmotion *mot;
    s32 x;
    s32 y;
    s32 z;
    s32 r;

    x = b * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12;
    z = b * rcossin_tbl[a & 0xFFF].unk2 >> 12;
    r = Random(0x80);
    y = World_floor_level - 0x600;
    nas = CreateNAS(x, y - r, z, 0x1802, 0x11);
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            eng->flags |= 0x20000000;
            fux = (EXTobject *)AddObject(nas);
            if (fux != NULL) {
                fux->coord.angle = a;
                fux->coord.dist = b;
                mot = AddMotion(nas);
                if (mot != NULL) {
                    LinkObject(nas);
                    AddSolid(nas, 0x300);
                    nas->FUX = (EXT *)fux;
                    eng->sub->u6C.dbase = &HeroModelBank[DB_RingObjects];
                    eng->xform.order = 1;
                    fux->whatami = c;
                    CrateCounts[fux->whatami]++;
                    if (d >= 0) {
                        fux->status = d;
                    } else {
                        fux->status = 3;
                    }
                    switch (c) {
                    case 8:
                        eng->sub->entity = 0x2003;
                        fux->contains = 1;
                        break;
                    case 9:
                        eng->sub->entity = 0x2001;
                        fux->contains = 2;
                        break;
                    case 10:
                        eng->sub->entity = 0x2002;
                        fux->contains = 3;
                        break;
                    case 11:
                        eng->sub->entity = 0x2004;
                        fux->contains = 0;
                        break;
                    case 12:
                    default:
                        eng->sub->entity = 0x2005;
                        fux->contains = Random(3) + 4;
                        break;
                    }
                    mot->Xvel = Random(8) + 4;
                    mot->Xa = a + 0x800;
                    mot->Xa = (mot->Xa - SpinSpeed * 0x200) & 0xFFF;
                }
            }
            shadow = AddExtEngine(nas, 2);
            if (shadow != NULL) {
                shadow->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                shadow->sub->entity = 0x2001;
                shadow->XYZ = eng->XYZ;
                shadow->flags |= 0x41;
                shadow->sub->unk76 = 0x800;
                shadow->sub->flags |= 0x40000000;
            }
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
            return NULL;
        }
    }
    return nas;
}

void PlotBalloon(NAS *nas) {
    EXTengine *eng;
    EXTobject *fux;
    EXTmotion *mot;
    EXTengine *shadow;
    s32 kind;
    s32 owner;
    s32 s;
    s32 a;
    u16 t;
    u16 uo;
    s32 o;
    s32 g;
    s32 y;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    HeroHooks.KeepInBounds(eng);
    fux = (EXTobject *)nas->FUX;
    if (fux != NULL) {
        kind = fux->whatami;
        if ((mot = FindExtension(nas, 0x800)) != NULL) {
            switch (fux->status) {
            case 0:
                fux->status = 1;
                fux->can_do &= 0xFFF5;
                if (kind < 0xD) if (kind >= 8) {
                    fux->can_do |= 0xA;
                }
                eng->xform.dirty = 1;
                fux->player = -1;
                FetchDefaultColBox(eng, 0x50);
                break;
            case 9:
                switch (kind) {
                case 8:
                case 9:
                case 10:
                case 11:
                    uo = fux->player;
                    owner = (s16)uo;
                    if (owner != -1 && owner < 4) {
                        if (SpinBalloonTimer != 0) {
                            if (SpinBalloonOwner == owner) {
                                if (GameLevelFlags & 0x40) {
                                    Heroes[uo & 2].hero_score--;
                                } else {
                                    Heroes[owner].hero_score++;
                                }
                            } else {
                                SetHeroAction(owner, (u16)HeroActHit);
                                if (SPIN_GAME->levelFlags & 0x80) {
                                    owner = fux->player;
                                    if (owner == 0 || (SPIN_GAME->teams != 0 && owner == 1)) {
                                        Heroes[owner].hero_health = 0;
                                        HeroTakeHit(fux->player, NULL, -1, 1, 8);
                                    }
                                }
                            }
                            SpawnModelFx(SpinBalloonAnims[SpinBalloonOwner], &eng->XYZ.x, (s32)&HeroModelBank[DB_RingObjects]);
                            goto sfx;
                        }
                        if (fux->contains == owner) {
                            if (GameLevelFlags & 0x40) {
                                Heroes[uo & 2].hero_score--;
                            } else {
                                Heroes[owner].hero_score++;
                            }
                        } else {
                            SetHeroAction(owner, (u16)HeroActHit);
                            if (SPIN_GAME->levelFlags & 0x80) {
                                owner = fux->player;
                                if (owner == 0 || (SPIN_GAME->teams != 0 && owner == 1)) {
                                    Heroes[owner].hero_health = 0;
                                    HeroTakeHit(fux->player, NULL, -1, 1, 8);
                                }
                            }
                        }
                    }
                    SpawnModelFx(SpinBalloonAnims[fux->contains], &eng->XYZ.x, (s32)&HeroModelBank[DB_RingObjects]);
                sfx:
                    PlaySfx(SpinBalloonSfx[kind - 8][Random(4)], NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                    o = fux->player;
                    if (o != -1 && o < 5 && Heroes[o].hero_object == (EXTobject *)fux) {
                        Heroes[o].hero_object = NULL;
                        if (Heroes[fux->player].hero_eng->sfx != NULL) {
                            StopSfx(Heroes[fux->player].hero_eng->sfx);
                        }
                    }
                    goto kill;
                case 12:
                    SpinScoreBalloon(fux);
                    o = fux->player;
                    if (o != -1 && o < 5 && Heroes[o].hero_object == (EXTobject *)fux) {
                        Heroes[o].hero_object = NULL;
                        if (Heroes[fux->player].hero_eng->sfx != NULL) {
                            StopSfx(Heroes[fux->player].hero_eng->sfx);
                        }
                    }
                    SpawnModelFx(4, &eng->XYZ.x, (s32)&HeroModelBank[DB_RingObjects]);
                    goto kill;
                }
            case 3:
                fux->status = 0;
                break;
            case 0x63:
                if (GravitateNas(nas) != 0) {
                    goto kill;
                }
                s = eng->xform.scale.vx;
                s -= 0x100;
                if (s < 0x101) {
                kill:
                    KillTriggerNas(nas);
                    return;
                }
                eng->xform.scale.vx = s;
                eng->xform.scale.vy = s;
                eng->xform.scale.vz = s;
                eng->sub->unk76 = s;
                eng->xform.dirty = 1;
                break;
            default:
                SPIN_CHECK_POP(fux);
                break;
            }
            if (GameExitFlags & 0x801) {
                fux->player = -1;
                fux->will_do |= 8;
            }
            a = get_angle(-eng->XYZ.x, -eng->XYZ.z) + SpinAngle;
            mot->Zvel = 4;
            a &= 0xFFF;
            mot->Za = a;
            eng->XYZ.x -= (mot->Xvel * rcossin_tbl[(mot->Xa - 0x400) & 0xFFF].unk2) >> 12;
            eng->XYZ.z -= (mot->Xvel * rcossin_tbl[mot->Xa & 0xFFF].unk2) >> 12;
            SPIN_BOUNCE(eng, mot);
            SpinBalloonCollide(eng, mot, fux);
            eng->XYZ.x -= (mot->Zvel * rcossin_tbl[(mot->Za - 0x400) & 0xFFF].unk2) >> 12;
            eng->XYZ.z -= (mot->Zvel * rcossin_tbl[mot->Za & 0xFFF].unk2) >> 12;
        }
        if ((u16)fux->contains < 4) {
            t = SpinBalloonTimer;
            if (t != 0) {
                eng->sub->entity = SpinBalloonModels[SpinBalloonOwner];
                if (t < 0x1E) {
                    if (t & 2) {
                        eng->sub->entity = SpinBalloonModels[fux->contains];
                        if (SpinBalloonFlag == 0) {
                            PlaySfx(0x5EF, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        SpinBalloonFlag = 1;
                    } else {
                        SpinBalloonFlag = 0;
                    }
                }
            } else {
                eng->sub->entity = SpinBalloonModels[fux->contains];
            }
        }
        a = fux->coord.angle + GameFrameCount * 16;
        eng->sub->XYZ.y = (rcossin_tbl[(a * 4 - 0x400) & 0xFFF].unk2 + rcossin_tbl[a & 0xFFF].unk2) >> 6;
        SPIN_FLOAT(eng, g, y, s);
    }
    SPIN_SHADOW(eng, shadow);
}

void SpinScoreBalloon(EXTobject *arg0) {
    NAS *n;
    EXTobject *f;
    s16 idx;
    u32 a;
    u32 b;
    u16 w0;
    u16 w1;
    u16 w2;
    u16 t;
    u32 r;
    s32 k;
    u16 *p;

    n = HeadOfObjects;
    idx = arg0->player;
    if (idx != -1) {
        a = AiPlayerSkill[idx].level;
        p = SpinBalloonWeights;
        b = 0x20 - a;
        a = p[a];
        b = p[b];
        if (GAME->heroOut[idx] != 0) {
            w0 = a;
            w1 = a >> 1;
            w2 = b;
        } else {
            w0 = b;
            w1 = b >> 1;
            w2 = a;
        }
        w2++;
        t = w2 + ((w0 = w0 + 1) + (w1 = w1 + 1));
        if (t == 0) {
            return;
        }
        r = (u16)Random(t);
        arg0->contains = 4;
        if ((u16)w0 < r) {
            arg0->contains = 5;
        }
        if ((u16)w0 + (u16)w1 < (s32)r) {
            arg0->contains = 6;
        }
    }
    switch (arg0->contains) {
    case 5:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        for (; n != NULL; n = n->Object.Child) {
            f = (EXTobject *)n->FUX;
            if (f != NULL && (k = f->whatami) < 12 && k >= 8 && f->contains != arg0->player) {
                f->player = -1;
                f->will_do |= 8;
            }
        }
        PlaySfx(0x5F1, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        if (SpinSfxEngine.sfx != NULL) {
            StopSfx(SpinSfxEngine.sfx);
        }
        PlaySfx(0x5EC, &SpinSfxEngine.sfx, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x5F0, NULL, NULL, 0x1000, 0x1E00, 0);
        SpinSpeed = ~SpinSpeed + 1;
        break;
    default:
        PlaySfx(0x5ED, NULL, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x5EE, NULL, NULL, 0x1000, 0x1E00, 0);
        SpinBalloonTimer = 0x96;
        SpinBalloonOwner = arg0->player;
        break;
    }
}

void SpinHero_ExtraEffects(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    VECTOR pos;
    s32 y;
    s32 top;

    base = Heroes;
    h = base + arg0;
    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
    if (h->hero_pu != 0) {
        pu = h->hero_powerup;
        sub = pu->sub;
        if (sub_find_point(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
            top = pos.vy - 0x40;
            y = top - h->hero_eng->XYZ.y;
        } else {
            switch (h->hero_name) {
            case 1:
            case 2:
                y = -0x180;
                break;
            case 3:
            case 4:
                y = -0x200;
                break;
            case 5:
            case 6:
            case 7:
            default:
                y = -0x280;
                break;
            }
        }
        pu->XYZ = Heroes[arg0].hero_eng->XYZ;
        sub->XYZ.y = y;
        if (Heroes[arg0].hero_pu != 0x13) {
            if (Heroes[arg0].hero_pu == 0x17 && SpinBigDrop > 30) {
                SpinBigDrop = 30;
            }
        }
        if (Heroes[arg0].hero_weapon_count == 0) {
            Heroes[arg0].hero_status &= 0xFFEF;
        }
        if (Heroes[arg0].hero_pu_timer != 0) {
            sub->flags |= 0x8000;
        } else {
            Heroes[arg0].hero_pu = 0;
            sub->flags &= ~0x8000;
            if (pu->sfx != NULL) {
                StopSfx(pu->sfx);
            }
        }
        pu->XYZ = Heroes[arg0].hero_eng->XYZ;
        pu->xform.dirty = 1;
        pu->xform.rot.vy += 0xAA;
    }
}

s32 SpinBalloonCollide(EXTengine *eng, EXTmotion *mot, EXTobject *fux) {
    FlagRecord *box;
    HeroInfo *h;
    NAS *it;
    EXTengine *other;
    EXTobject *f;
    s32 hit;
    s32 i;
    s32 bot;
    s32 top;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 dy;
    s32 r;
    s32 t;
    s32 lo;
    s32 kind;

    hit = 0;
    box = FindCollisionBox(eng, 0x40);
    if (box != NULL) {
        i = 0;
        top = eng->XYZ.y + box->dy;
        bot = eng->XYZ.y + box->height;
        do {
            h = &Heroes[i];
            if (!(h->hero_status & 0xF800) && h->hero_pu == 0x17) {
                if (SpinBalloonTimer != 0 && SpinBalloonOwner == i) {
                    if (SpinBalloonTimer < 0x1E) {
                        goto next;
                    }
                } else {
                    do {
                        do {
                            if (fux->contains != i) {
                                goto next;
                            }
                        } while (0);
                    } while (0);
                }
                other = h->hero_eng;
                dx = other->XYZ.x - eng->XYZ.x;
                dz = other->XYZ.z - eng->XYZ.z;
                d = dx * dx + dz * dz;
                if (d <= 0x01E3FFFF) {
                    r = csqrt(d);
                    ang = get_angle(dx, dz) & 0xFFF;
                    d = r >> 9;
                    eng->XYZ.x += (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                    eng->XYZ.z += (d * rcossin_tbl[ang].unk2) >> 12;
                    if (d < 0x280) {
                        t = eng->XYZ.y - 0x200;
                        dy = t - h->hero_powerup->XYZ.y;
                        if (abs(dy) < 0x10) {
                            eng->XYZ.y += dy >> 3;
                        }
                    }
                }
            }
        next:
            i++;
        } while (i < 4);
        it = HeadOfSolids;
        while (it != NULL) {
            if (it->Class & 0x200) {
                other = it->Engine;
                if (other != NULL && other != eng) {
                    f = (EXTobject *)it->FUX;
                    if (f != NULL) {
                        kind = f->whatami;
                        if (kind < 0xD) if (kind >= 8) {
                            d = other->XYZ.y;
                            lo = d - 0x100;
                            if (bot < d && lo < top) {
                                dx = other->XYZ.x - eng->XYZ.x;
                                dz = other->XYZ.z - eng->XYZ.z;
                                d = csqrt(dx * dx + dz * dz) >> 6;
                                if (d < 0x100) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    d = 0xFF - d;
                                    eng->XYZ.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                    hit = 1;
                                    eng->XYZ.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                                } else if (d < 0x400) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    mot->Zvel = (0x400 - d) >> 6;
                                    mot->Za = ang;
                                }
                            }
                        }
                    }
                }
            }
            it = it->Solid.Child;
        }
    }
    return hit;
}

s32 SpinHitTest(x, y, z, size, skip, mask)
    s32 x;
    s32 y;
    s32 z;
    u16 size;
    s32 skip;
    u16 mask;
{
    s32 ret;
    u32 i;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    FlagRecord *rec;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 dist;

    ret = -1;
    for (i = 0; i < 4; i++) {
        if (skip == i) {
            continue;
        }
        if (Heroes[i].hero_status & 0xFA00) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = FindCollisionBox(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->XYZ.y + rec->dy + size;
        bot = eng->XYZ.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->XYZ.x - x;
            dist = dx * dx;
            dz = eng->XYZ.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                ret = i;
            }
        }
    }
    for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200)) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL) {
            continue;
        }
        rec = FindCollisionBox(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        top = eng->XYZ.y + rec->dy + size;
        bot = eng->XYZ.y + rec->height - size;
        if (y < top && bot < y) {
            dx = eng->XYZ.x - x;
            dist = dx * dx;
            dz = eng->XYZ.z - z;
            dist += dz * dz;
            rr = rec->radius + size;
            rr *= rr;
            if (dist < rr) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    obj->player = skip;
                    obj->will_do |= mask;
                }
                if (ret != -1) {
                    ret = -1;
                }
            }
        }
    }
    return ret;
}

/* the AI's debug printf format, which nothing prints with in retail, and an empty string;
 * their place in .rodata is between functions' jump tables, so they are defined here */
const char SpinAiDebugFormat[] = "%d ai mode %d target %x player %d angle %x dist %x- %x\n";
const char SpinEmptyString[] = "";

void Spin_HeroBot(s32 arg0) {
    s32 unused[4];
    EXTobject *nf;
    NAS *nas;
    s32 kind;
    s32 lvl;
    s32 dist2;
    s32 turn;
    s32 mate;
    u16 r;
    EXTbotai *ai;
    EXTengine *e;
    EXTobject *f;
    PadState *pad;
    s32 inv;
    s32 found;
    s32 x;
    s32 z;
    s32 ang;
    s32 ang2;
    s32 target;
    s32 a;
    s32 d;
    s32 p;
    s32 k;

    if (SPIN_GAME->pause & 0x04000000) {
        return;
    }
    lvl = AiPlayerSkill[arg0].level;
    inv = 0x21 - lvl;
    ai = Heroes[arg0].hero_ai;
    if (ai->AIinitial_delay != 0) {
        ai->AIinitial_delay--;
    }
    if (ai->AIsleep != 0) {
        ai->AIsleep--;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    if (ai->AItimer2 != 0) {
        ai->AItimer2--;
    }
    ai->AIpad = 0;
    mate = -1;
    if (SPIN_GAME->teams != 0) {
        mate = arg0 ^ 1;
    }
    if (Heroes[arg0].hero_mode == (u16)HeroActHit || Heroes[arg0].hero_mode == (u16)HeroActDead) {
        ai->TargetPlayer = -1;
        ai->AIsleep = 5;
        ai->pTargetNas = NULL;
        ai->AImode = 6;
    }
    dist2 = 0;
    nas = ai->pTargetNas;
    ang = Heroes[arg0].hero_move->facing_angle;
    r = Random(0x100);
    found = 0;
    kind = -1;
    turn = 0;
    x = 0;
    z = 0;
    if (nas != NULL && nas->NASid != 0 && (e = nas->Engine) != NULL &&
        (nf = (EXTobject *)nas->FUX) != NULL) {
        kind = nf->whatami;
        if (nf->status == 1) {
            found = 1;
            x = e->XYZ.x;
            z = e->XYZ.z;
            x += inv * 16 - Random(inv << 5);
            z += inv * 16 - Random(inv << 5);
        }
    }
    if (found) {
        ai->TargetPlayer = -1;
    }
    target = ai->TargetPlayer;
    if (target != -1) {
        if (target >= 4) {
            ai->TargetPlayer = -1;
            target = -1;
            ai->AIsleep = inv;
            ai->AImode = 1;
        } else {
            if (!(Heroes[target].hero_status & 0xFA00) && (Heroes[target].hero_immunity | Heroes[target].hero_hit) == 0) {
                found = 1;
                x = Heroes[target].hero_eng->XYZ.x;
                z = Heroes[target].hero_eng->XYZ.z;
                x += inv * 16 - Random(inv << 5);
                z += inv * 16 - Random(inv << 5);
            }
        }
    }
    if (found) {
        a = get_angle(x, z);
        turn = angle_diff(a, get_angle(Heroes[arg0].hero_eng->XYZ.x, Heroes[arg0].hero_eng->XYZ.z)) * SpinSpeed;
        if (Heroes[arg0].hero_pu != 0x13 && turn >= 0x29 && Heroes[arg0].hero_move->jump_max == 0) {
            s32 b;

            d = (csqrt(x * x + z * z) + 0x20) >> 6;
            b = (get_angle(x, z) - inv * SpinSpeed) & 0xFFF;
            x = (d * rcossin_tbl[(b - 0x400) & 0xFFF].unk2) >> 12;
            z = (d * rcossin_tbl[b].unk2) >> 12;
        }
        get_angle(x, z);
        x -= Heroes[arg0].hero_eng->XYZ.x;
        z -= Heroes[arg0].hero_eng->XYZ.z;
        dist2 = x * x + z * z;
        ang = get_angle(x, z);
    } else {
        nas = NULL;
        ai->pTargetNas = NULL;
        ai->AIsleep = inv;
        ai->AImode = 1;
    }
    x = Heroes[arg0].hero_eng->XYZ.x;
    z = Heroes[arg0].hero_eng->XYZ.z;
    ang2 = ang;
    if (dist2 != 0) {
        get_angle(x, z);
        if (turn > 0) {
            ang2 = ang - (inv << 13) / 360;
            ang2 &= 0xFFF;
        } else {
            ang2 = ang;
        }
    }
    if (Heroes[arg0].hero_pu == 0x13) {
        ang2 = ang;
    }
    SpinAI_ScanAhead(arg0);
    switch (ai->AImode) {
    case 21:
        if (ai->AIsleep != 0) {
            break;
        }
        {
            s32 n;

            if (Heroes[arg0].hero_mode != 2) {
                ai->TargetPlayer = -1;
                ai->AIsleep = inv;
                ai->AImode = 1;
                break;
            }
            for (n = 0; n < 4; n++) {
                if (n != arg0 && Heroes[n].hero_ai->TargetPlayer == arg0) {
                    x = Heroes[n].hero_eng->XYZ.x - Heroes[arg0].hero_eng->XYZ.x;
                    z = Heroes[n].hero_eng->XYZ.z - Heroes[arg0].hero_eng->XYZ.z;
                    if (x * x + z * z < 0x240000) {
                        ai->TargetPlayer = -1;
                        ai->AIsleep = inv;
                        ai->AImode = 1;
                    }
                }
            }
        }
        ai->AIang = ang;
        ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        break;
    case 6:
        if (ai->AIsleep == 0 && Heroes[arg0].hero_mode == 0) {
            ai->AIsleep = 5;
            ai->AImode = 1;
        }
        break;
    case 3:
        if (Heroes[arg0].hero_move->jump_max != 0) {
            ai->AIang = ang;
            ai->AIpad |= DirPadBits[(ang >> 9) & 7];
            break;
        }
        if (nas != NULL) {
            ai->AImode = 2;
            break;
        }
        if (target == -1) {
            ai->AIsleep = inv;
            ai->AImode = 1;
            break;
        }
        if (SpinAlwaysZero(arg0) == 0) {
            p = HeroTestBox(arg0, 0x40);
            if (p == target) {
                ai->AImode = 1;
                ai->TargetPlayer = -1;
                ai->AIsleep = inv;
                if ((Heroes[p].hero_immunity | Heroes[p].hero_frozen) == 0) {
                    ai->AIpad |= 0x8000;
                }
            } else if (p != -1 && (Heroes[p].hero_immunity | Heroes[p].hero_frozen) == 0) {
                if (GameTeams[0] == 0 || p != mate) {
                    if (GameTeams[0] != 0 && Heroes[p].hero_object != NULL) {
                        ai->AIpad |= 0x8000;
                    } else if (!(r & 0x10)) {
                        ai->AIpad |= 0x8000;
                    }
                }
            }
            f = SpinHeroObjectNear(arg0, 0xA, 0x400);
            if (f != NULL) {
                switch (f->whatami) {
                case 8:
                case 9:
                case 10:
                case 11:
                    if (f->contains != arg0) {
                        break;
                    }
                    goto sleep3;
                case 2:
                    ai->AIpad |= 0x8000;
                    break;
                case 12:
                default:
                sleep3:
                    ai->AImode = 0xD;
                    ai->AIsleep = 8;
                    ai->AIpad |= 0x4000;
                    ai->pTargetNas = ((NAS **)f)[-1];
                    ai->TargetPlayer = -1;
                    break;
                }
            }
        }
        ai->AIang = ang;
        ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        break;
    case 2:
        if (Heroes[arg0].hero_move->jump_max != 0) {
            ai->AIang = ang;
            ai->AIpad |= DirPadBits[(ang >> 9) & 7];
            break;
        }
        if (target != -1) {
            ai->AImode = 3;
            break;
        }
        if (nas == NULL) {
            ai->AIsleep = inv;
            ai->AImode = 1;
            break;
        }
        if (Heroes[arg0].hero_eng->platform == nas->Engine) {
            ai->AIsleep = inv;
            ai->AImode = 1;
            break;
        }
        if (SpinAlwaysZero(arg0) == 0) {
            p = HeroTestBox(arg0, 0x40);
            if (p != target && (Heroes[p].hero_immunity | Heroes[p].hero_frozen) == 0) {
                if (GameTeams[0] == 0 || p != mate) {
                    if (Random(3) == 0) {
                        ai->AImode = 1;
                        ai->AIsleep = inv;
                        ai->AIpad |= 0x8000;
                    }
                }
            }
            f = SpinHeroObjectNear(arg0, 0xA, 0x400);
            if (f != NULL) {
                switch (k = f->whatami) {
                case 8:
                case 9:
                case 10:
                case 11:
                    if (f->contains != arg0) {
                        break;
                    }
                    if (k == kind) {
                        goto sleep2;
                    }
                case 12:
                    ai->AImode = 0xD;
                    ai->AIpad |= 0x4000;
                    ai->pTargetNas = ((NAS **)f)[-1];
                    ai->TargetPlayer = -1;
                    break;
                case 2:
                    ai->AIpad |= 0x8000;
                    break;
                default:
                sleep2:
                    ai->AImode = 0xD;
                    ai->AIsleep = 8;
                    ai->AIpad |= 0x4000;
                    ai->pTargetNas = ((NAS **)f)[-1];
                    ai->TargetPlayer = -1;
                    break;
                }
            }
        }
        ai->AIang = ang2;
        ai->AIpad |= DirPadBits[(ang2 >> 9) & 7];
        break;
    case 13:
        if (ai->AIsleep == 0 && (u32)(Heroes[arg0].hero_mode - 0x34) >= 2) {
            ai->AImode = 1;
            ai->AIsleep = inv;
            ai->TargetPlayer = -1;
            ai->pTargetNas = NULL;
        } else {
            ai->AIpad |= 0x4000;
            ai->AIang = ang;
            ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        }
        break;
    default:
        if (Heroes[arg0].hero_mode != 0) {
            break;
        }
        if (Heroes[arg0].hero_pu == 0 && ai->AIinitial_delay == 0 && ai->AItimer2 == 0 &&
            SpinAI_NearestEnemy(arg0) > (lvl << 5) + 0x400 && (p = SpinAI_PickPlayer(arg0)) != -1) {
            ai->AImode = 0x15;
            ai->TargetPlayer = p;
            ai->AIsleep = 4;
            ai->AItimer2 = (lvl + 8) * 30;
            ai->AIpad |= 0x1000;
        } else if (SpinAI_ChooseTarget(arg0) != 0) {
            if (ai->pTargetNas == NULL) {
                ai->AImode = 3;
            } else {
                ai->AImode = 2;
            }
            ai->AIaborted_mode = 0;
        } else {
            ai->TargetPlayer = SpinAI_PickPlayer(arg0);
            if ((u32)Random(inv) >= 9) {
                ai->AImode = 0x15;
                ai->AIsleep = 4;
                ai->AItimer2 = 300;
                ai->AIpad |= 0x1000;
            } else {
                ai->AImode = 3;
            }
        }
        break;
    }
    pad = &Pads[arg0];
    set_joypad((u8 *)pad, ai->AIpad);
    pad->angle.w = ai->AIang;
}

void SpinAI_ScanAhead(s32 arg0) {
    EXTengine *eng;
    EXTheromove *mv;
    s32 rx;
    s32 rz;
    s32 lx;
    s32 lz;
    s32 dx;
    s32 dz;
    s32 ang;
    s32 c;
    s32 s;
    s32 gx;
    s32 gz;
    s32 id;
    s32 i;
    u8 v;

    eng = Heroes[arg0].hero_eng;
    rx = eng->XYZ.x;
    rz = eng->XYZ.z;
    mv = Heroes[arg0].hero_move;
    dx = rcossin_tbl[mv->forward_angle & 0xFFF].unk2 * 15 >> 9;
    ang = mv->forward_angle;
    c = rcossin_tbl[(ang + 0x400) & 0xFFF].unk2;
    s = rcossin_tbl[(ang + 0x800) & 0xFFF].unk2;
    dz = c * 15 >> 9;
    lx = rx + dx;
    rx = rx - dx;
    lz = rz + dz;
    rz = rz - dz;
    dx = c * 11 >> 7;
    dz = s * 11 >> 7;
    lx += dx;
    lz += dz;
    rx += dx;
    rz += dz;
    dx = c * 256 >> 12;
    dz = s * 256 >> 12;
    id = (arg0 << 5) | 0x80;
    i = 0;
    while ((u32)(lx + 0x7FF) < 0xFFF && lz < 0x800 && lz >= -0x7FF && i < 8) {
        gx = (lx >> 8) + 10;
        gz = 9 - (lz >> 8);
        v = ArenaGrid[gz * 20 + gx].flags;
        if ((v & 0xE0) == id) {
            v &= 0x1F;
        }
        lx += dx;
        lz += dz;
        Heroes[arg0].hero_ai->AIforward_left[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_left[i] = 0xFE;
    }
    i = 0;
    while ((u32)(rx + 0x7FF) < 0xFFF && rz < 0x800 && rz >= -0x7FF && i < 8) {
        gx = (rx >> 8) + 10;
        gz = 9 - (rz >> 8);
        v = ArenaGrid[gz * 20 + gx].flags;
        if ((v & 0xE0) == id) {
            v &= 0x1F;
        }
        rx += dx;
        rz += dz;
        Heroes[arg0].hero_ai->AIforward_right[i] = v;
        i++;
    }
    for (; i < 8; i++) {
        Heroes[arg0].hero_ai->AIforward_right[i] = 0xFE;
    }
}


s32 SpinAI_ChooseTarget(s32 player) {
    NAS *bestNas;
    s32 bestHeroScore;
    s32 r;
    s32 bestHero;
    s32 px;
    s32 pz;
    s32 mate;
    s16 heroW;
    EXTbotai *ai;
    EXTengine *e;
    EXTengine *he;
    NAS *n;
    EXTobject *f;
    s32 lvl;
    s32 inv;
    s16 w0;
    s16 w1;
    s16 w2;
    s32 dx;
    s32 dz;
    s32 d;
    s32 d2;
    s32 cnt;
    s32 bestNasScore;
    s32 i;
    s32 k;

    n = HeadOfObjects;
    bestNas = NULL;
    bestHero = -1;
    bestHeroScore = 0;
    lvl = AiPlayerSkill[player].level;
    inv = 0x21 - lvl;
    mate = Heroes[player].hero_teammate;
    he = Heroes[player].hero_eng;
    ai = Heroes[player].hero_ai;
    px = he->XYZ.x;
    pz = he->XYZ.z;
    Random(0x100);
    bestNasScore = 0;
    w1 = 0x1E;
    w2 = w1;
    r = Random(0x1000);
    w0 = (lvl + 0x14) * 4;
    heroW = lvl * 8;
    if (SpinBalloonTimer != 0 && SpinBalloonOwner != player) {
        w0 = 0;
        heroW = lvl << 5;
        w1 = lvl * 0x10;
    }
    if (Heroes[player].hero_pu != 0) {
        if (Heroes[player].hero_pu == 0x17) {
            if (SpinBalloonTimer != 0) {
                if (SpinBalloonOwner == player) {
                    w0 = lvl << 5;
                    w1 = inv * 2;
                    w2 = inv * 4;
                } else {
                    heroW = lvl * 8;
                    w0 = inv * 4;
                    w1 = inv * 2;
                    w2 = lvl << 5;
                }
            } else {
                w0 = lvl << 5;
                w1 = inv * 4;
                w2 = inv * 2;
            }
        } else if (SpinBalloonTimer != 0) {
            if (SpinBalloonOwner == player) {
                w0 = lvl << 5;
                w1 = inv * 4;
                w2 = lvl << 6;
            } else {
                heroW = lvl * 0x10;
                d2 = inv * 4;
                w0 = d2;
                w1 = d2;
                w2 = lvl << 5;
            }
        } else {
            w0 = lvl << 5;
            d2 = inv * 8;
            w1 = d2;
            w2 = d2;
        }
    }
    for (; n != NULL; n = n->Object.Child) {
        e = n->Engine;
        if (e == NULL) continue;
        f = (EXTobject *)n->FUX;
        if (f == NULL) continue;
        switch (f->whatami) {
        case 8: case 9: case 10: case 11:
            if (SpinBalloonTimer != 0 && SpinBalloonOwner != player) continue;
            if (f->contains != player) continue;
            dx = px - e->XYZ.x;
            dz = pz - e->XYZ.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w0;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 2:
            dx = px - e->XYZ.x;
            dz = pz - e->XYZ.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w1;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt++;
            }
            if (cnt != 0) d >>= cnt;
            if (bestNasScore < d) {
                bestNas = n;
                bestNasScore = d;
            }
            continue;
        case 12:
            dx = px - e->XYZ.x;
            dz = pz - e->XYZ.z;
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * (s16)w2;
            cnt = 0;
            for (i = 0; i < 4; i++) {
                if (ai->pTargetNas == n) cnt++;
            }
            break;
        default:
            continue;
        }
        if (cnt != 0) d >>= cnt;
        if (bestNasScore < d) {
            bestNas = n;
            bestNasScore = d;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i == player) continue;
        if (((GameState *)GameInfo)->teams != 0 && i == mate) continue;
        if (Heroes[i].hero_status & 0xFA00) continue;
        dx = px - Heroes[i].hero_eng->XYZ.x;
        dz = pz - Heroes[i].hero_eng->XYZ.z;
        if (Heroes[i].hero_mode == 2 || Heroes[i].hero_pu != 0) {
            d = csqrt(dx * dx + dz * dz) >> 6;
            d = abs(d - r) * heroW;
            cnt = 0;
            for (k = 0; k < 4; k++) {
                if (ai->TargetPlayer == k) cnt += 4;
            }
            if (cnt != 0) d >>= cnt;
            if (bestHeroScore < d) {
                bestHero = i;
                bestHeroScore = d;
            }
        }
    }
    if (bestHeroScore < bestNasScore) {
        ai->pTargetNas = bestNas;
        ai->TargetPlayer = -1;
        return 1;
    }
    if (bestHeroScore != 0) {
        ai->TargetPlayer = bestHero;
        ai->pTargetNas = NULL;
        return 1;
    }
    return 0;
}

s32 SpinAI_PickPlayer(s32 arg0) {
    s32 other;
    s32 sum;
    s32 i;
    s32 r;

    other = -1;
    if (GameTeams[0] != 0) {
        other = arg0 ^ 1;
    }
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].quality;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != other && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].quality;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 SpinAI_NearestEnemy(s32 arg0) {
    s32 flip = arg0 ^ 1;
    s32 best = 0x7FFFFF;
    u16 i = 0;
    u8 *g = GameInfo;
    HeroInfo *base = Heroes;
    HeroInfo *me = base + arg0;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 d;
    EXTengine *a;
    EXTengine *b;

    for (; i < 4; i++) {
        if (i == arg0) {
            continue;
        }
        if (*(volatile s32 *)(g + 0x6C) != 0 && i == flip) {
            continue;
        }
        h = (HeroInfo *)(i * sizeof(HeroInfo) + (s32)base);
        if (h->hero_status & 0xFA00) {
            continue;
        }
        a = me->hero_eng;
        b = h->hero_eng;
        dx = a->XYZ.x - b->XYZ.x;
        dz = a->XYZ.z - b->XYZ.z;
        d = dx * dx + dz * dz;
        if (d < best) {
            best = d;
        }
    }
    return best != 0x7FFFFF ? csqrt(best) >> 6 : 0;
}

s32 SpinAI_HazardAhead(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;
    s32 i;
    s32 l;
    s32 r;

    for (i = 0; i < 3; i++) {
        l = ai->AIforward_left[i] & 0x3F;
        r = ai->AIforward_right[i] & 0x3F;
        if (ai->AIforward_left[i] == 0xFF || ai->AIforward_right[i] == 0xFF) {
        ret1:
            return 1;
        }
        if (l >= 6) {
            if (l < 8) {
                goto ret1;
            }
            if (l < 0x16) {
                if (l >= 0x14) {
                    goto ret1;
                }
            }
        }
        if (r >= 6) {
            if (r < 8) {
                goto ret1;
            }
            if (r < 0x16) {
                if (r >= 0x14) {
                    goto ret1;
                }
            }
        }
    }
    return 0;
}

s32 SpinAlwaysZero(s32 player) {
    return 0;
}

s32 SpinNoAttack(void) {
    return 0;
}

void SpinTouched(void) {
}

void SpinDropObject(u16 arg0, u16 arg1) {
    s32 i;
    SpinPair *p;

    for (i = 0; i < arg0; i++) {
        p = SpinFindDropPoint(&SpinDropPos);
        if (p != NULL) {
            CreateHazard(p->a, p->b, arg1, -1);
        }
    }
}

void SpinInitASTIndices(void) {
    s32 key;
    s32 i;
    s32 r;

    key = 0x1801;
    for (i = 0; i < ASTCount; i++) {
        if (ASTTable[i].id == key) {
            r = i;
            goto found;
        }
    }
    r = 0;
found:
    SpinASTBase = r;
    HitHooks.break_tile = 0;
    HitHooks.explo_range = 0;
}

s32 SPIN_KeepInBounds(EXTengine *eng) {
    s32 x = eng->XYZ.x;
    s32 z = eng->XYZ.z;
    s32 limit;
    s32 ret = 0;
    s32 ang;

    if (eng->collision_box.flags & 0x40) {
        limit = 0x980 - eng->collision_box.radius;
    } else {
        limit = 0x980;
    }
    if (x * x + z * z >= limit * limit) {
        ang = get_angle(x, z);
        eng->XYZ.x = limit * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        eng->XYZ.z = limit * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
        ret = 1;
    }
    return ret;
}

void FirstTriggersSPIN(void) {
}

void SpinRestartLevel(void) {
}

void UpdateSPIN(void) {
    ArenaGridClear();
    SpinCountObjects();
    MarkHeroesOnGrid();
    SpinDropMaster();
    SpinTurnArena();
}

void SPINSetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseSPINFirst;
    LevelSpecs.LevelInit = InitialiseSPIN;
    LevelSpecs.LevelRestartInit = InitialiseSPIN;
    LevelSpecs.LevelRoutine = UpdateSPIN;
    LevelSpecs.EarliestTriggers = FirstTriggersSPIN;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
}

void SpinPreMoveHero(s32 arg0) {
    HeroInfo *h;
    EXTheromove *m;
    PadState *pad;
    HeroInfo *hb;
    HeroInfo *hb2;
    HeroInfo *h2;
    PadState *pb;
    u16 *row;
    s32 speed;
    u16 rw;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    speed = (HeroHooks.RunSpeeds + h->hero_name * 3)[1] + (AiPlayerSkill[arg0].level >> 2);
    (HeroHooks.RunSpeeds + h->hero_name * 3)[2] = speed;
    if (h->hero_frozen != 0) {
        ((PadState *)Pads)[arg0].dir = 0;
        ((PadState *)Pads)[arg0].dirPressed = 0;
    }
    pb = (PadState *)Pads;
    pad = pb + arg0;
    if ((pad->flags & 1) || h->hero_ai->AImode != 0) {
        if (pad->dir & 0xF0) {
            m->forward_angle = *(u16 *)&pad->angle + 0x800;
            if (m->velocity_enable != 0) {
                rw = m->run_walk;
                if (h->hero_object != NULL) {
                    if (rw != 0) {
                        rw--;
                    }
                }
                row = HeroHooks.RunSpeeds;
                hb2 = Heroes;
                h2 = hb2 + arg0;
                speed = (row + h2->hero_name * 3)[rw];
                m->forward_speed = speed;
            }
        } else {
            if (pad->dir & 0x100) {
                m->forward_angle -= 0x100;
            }
            if (pad->dir & 0x200) {
                m->forward_angle += 0x100;
            }
            m->x_max = 0;
            m->z_max = 0;
            m->facing_angle = m->forward_angle = m->forward_angle & 0xFFF;
        }
    }
    speed = m->forward_angle & 0xFFF;
    m->forward_angle = speed;
    m->facing_angle = speed;
}

void SetSpinHeroHit(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    SpinHitDone[arg0] = 0;
    base = Heroes;
    h = base + arg0;
    h->hero_hit = 0x1E;
    h->hero_pu_timer = 0;
    h->hero_move->velocity_enable = 0;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq(arg0, 0x2B);
}

void SpinHeroHit(s32 arg0) {
    HeroInfo *h;
    HeroInfo *h2;
    u16 *flag;
    u16 *fbase;
    s32 r;
    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_move->reactive_speed != 0) {
        HeroHooks.HeroKickCollision();
    }
    r = ProcessHeroAnim(arg0);
    if (r != 8) {
        if (r == 9) {
            PlaySfx(SpinHitSfx[arg0], NULL, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
        }
    } else {
        SpinHitDone[arg0] = 1;
        if (h->hero_health <= 0 && !(GameCheats & 2)) {
            h->hero_immunity = 0;
            h->hero_hit += 0x28;
            SetHeroSeq(arg0, 0x2D);
        }
        Heroes[arg0].hero_health = 0x14;
    }
    fbase = SpinHitDone;
    flag = fbase + arg0;
    if (*flag != 0) {
        HeroInfo *base2 = Heroes;

        h2 = base2 + arg0;
        if (abs(((s32 (*)(EXTengine *))ApplyContour)(h2->hero_eng)) < 0x20) {
            *flag = 0;
            switch (h2->hero_name) {
            case 1:
                PlaySfx(0x11, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x43, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x75, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xA7, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x10B, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xD9, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x13D, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x16F, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            }
        }
    }
    if (HeroHooks.HeroCarryCollision != NULL) {
        HeroHooks.HeroCarryCollision(arg0);
    }
    HeroTestFall(arg0);
}

void SetSpinHeroFlip(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq(arg0, 0x2C);
    idx = 0;
    if (GAME->heroOut[arg0] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = HeroHooks.JumpEnts[idx].vel;
    h->hero_move->jump_max = HeroHooks.JumpEnts[idx].max;
    h->hero_move->jump_acc = HeroHooks.JumpEnts[idx].acc;
    h->hero_move->jump_dec = HeroHooks.JumpEnts[idx].dec;
    h->hero_move->jump_dir = -1;
}

void SpinHeroFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *eng;

    if (ApplyContourMoved(h->hero_eng) != 0) {
        SetHeroAction(arg0, 7);
    } else {
        eng = h->hero_eng;
        if (World_floor_level < eng->XYZ.y + h->hero_sub->XYZ.y) {
            if (ContourHeight == 0x7FFFFFFF) {
                SetHeroAction(arg0, 6);
            } else {
                eng->XYZ.y = World_floor_level;
                h->hero_sub->XYZ.y = 0;
            }
        }
    }
    HeroHooks.HeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SetSpinHeroDead(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 2;
    h->hero_health = 0;
    h->hero_status |= 0x2000;
    h->hero_powerup->flags = 0;
    h->hero_fx->flags = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    HeroBecomeAngel(arg0);
}

void SpinHeroDead(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    u16 st;
    s32 ret;

    base = Heroes;
    h = &base[arg0];
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    st = h->hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x36);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x37);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base2 = Heroes;
        h2 = &base2[arg0];
        if ((u32)(h2->hero_name - 1) < 8) {
            h2->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[*(volatile u16 *)&h2->hero_name]];
        } else {
            h2->hero_sub->u6C.dbase = 0;
        }
    } else {
        HeroGhost(arg0);
    }
}

void SetSpinHeroWinner(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *b2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[h->hero_name]];
    h->hero_eng->XYZ.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->XYZ.y -= dy;
    } else {
        h->hero_eng->XYZ.y = World_floor_level;
    }
    {
        HeroInfo *h2;
        b2 = Heroes;
        h2 = b2 + arg0;
        h2->hero_sub->XYZ.y = 0;
        h2->hero_pu = 0;
        h2->hero_pu_timer = 0;
    }
    RequestHeroAnim(arg0, 0);
    SetHeroSeq(arg0, 0xC);
    StartHeroVictory(arg0);
    CameraMode = 1;
}

void SpinHeroWinner(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->XYZ.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->XYZ.y -= dy;
    } else {
        h->hero_eng->XYZ.y = World_floor_level;
    }
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        FinishLevel();
    }
    ProcessHeroAnim(arg0);
}

void SetSpinHeroLose(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 8;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->u6C.dbase = (s32)HeroModelBank + HeroHooks.HeroModels[h->hero_name] * 0x38;
    h->hero_eng->XYZ.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->XYZ.y -= dy;
    } else {
        h->hero_eng->XYZ.y = World_floor_level;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_eng->xform.hasScale = 1;
    h2->hero_eng->xform.scale.vx = 0x1000;
    h2->hero_eng->xform.scale.vy = 0x1000;
    h2->hero_eng->xform.scale.vz = 0x1000;
    h2->hero_sub->XYZ.y = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    SetHeroSeq(arg0, 1);
    RemoveCamera(h2->hero);
}

void SpinHeroLose(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;
    s32 dy;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->XYZ.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->XYZ.y -= dy;
    } else {
        h->hero_eng->XYZ.y = World_floor_level;
    }
    ProcessHeroAnim(arg0);
    {
        HeroInfo *b2 = Heroes;
        h = b2 + arg0;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    if (GameExitFlags & 0x800) {
        EXTengine *e = h->hero_eng;
        s32 v = e->xform.scale.vx;
        v -= 0x60;
        if (v > 0x300) {
            e->xform.scale.vx = v;
            h->hero_eng->xform.scale.vy = v;
            h->hero_eng->xform.scale.vz = v;
        } else {
            SpawnModelFx(SpinBalloonAnims[arg0], &e->XYZ.x, (s32)&HeroModelBank[DB_RingObjects]);
            h->hero_status |= 0x8000;
        }
    }
}

/* The HeroHooks.Hero2ObjectCollision hook: claim the first object in reach of player's
 * cylinder that can take mask (within cone of the hero's heading, 0: any).
 * Each arm claims in its own do-while (0); jump2 merges the copies. */
NAS *SpinHero2ObjectCollision(player, mask, unused, cone)
    s32 player;
    s32 mask;
    s32 unused;
    u16 cone;
{
    VECTOR pos;
    s32 top;
    s32 bot;
    HeroInfo *heroes;
    HeroInfo *h;
    EXTengine *me;
    EXTengine *eng;
    FlagRecord *rec;
    FlagRecord *rec2;
    EXTobject *obj;
    NAS *nas;
    NAS *self;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 dx;
    s32 dz;
    s32 r;

    heroes = Heroes;
    h = &heroes[player];
    me = h->hero_eng;
    if (me == NULL) {
        return NULL;
    }
    rec = FindCollisionBox(me, 0x400);
    if (rec == NULL) {
        return NULL;
    }
    if (me->xform.dirty != 0) {
        rotscale_update(&me->xform);
    }
    gfx_load_xform(me->sub, &me->xform.mtx, &me->XYZ);
    bot = me->XYZ.y + rec->dy;
    top = me->XYZ.y + rec->height;
    rad = rec->radius;
    gte_rotate_point((SVECTOR *)rec, &pos);
    nas = HeadOfSolids;
    self = NULL;
    if (h->hero_object != NULL) {
        self = ((NAS **)h->hero_object)[-1];
    }
    for (; nas != NULL; nas = nas->Solid.Child) {
        if (!(nas->Class & 0x200) || nas == self) {
            continue;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec2 = FindCollisionBox(eng, 0x40);
        if (rec2 == NULL) {
            continue;
        }
        lo = eng->XYZ.y + rec2->dy;
        hi = eng->XYZ.y + rec2->height;
        if (top < lo && hi < bot) {
            dx = eng->XYZ.x - pos.vx;
            dz = eng->XYZ.z - pos.vz;
            r = rec2->radius + rad;
            if (dx * dx + dz * dz < r * r) {
                obj = FindExtension(nas, 0x1000);
                if (obj != NULL && (obj->can_do & mask)) {
                    if (cone == 0) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    } else if (angle_diff_abs(Heroes[player].hero_move->forward_angle,
                                             get_angle(eng->XYZ.x - me->XYZ.x, eng->XYZ.z - me->XYZ.z) ^ 0x800) < cone) {
                        do {
                            obj->player = player;
                            obj->will_do |= mask;
                            return nas;
                        } while (0);
                    }
                }
            }
        }
    }
    return NULL;
}

NAS *SpinHeroGrabObject(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXTobject *fux;
    HeroInfo *h;
    HeroInfo *hb;
    s32 hx, hz, dx, dz, m, lim;
    u16 ang;
    u16 um;

    um = (u16)mask;
    lim = 0x10000;
    hb = Heroes;
    n = HeadOfObjects;
    h = &hb[idx];
    he = h->hero_eng;
    ang = Heroes[idx].hero_move->facing_angle;
    hx = he->XYZ.x - (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->XYZ.z - (rcossin_tbl[ang & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = (EXTobject *)n->FUX;
        if (fux == NULL) continue;
        if (!(fux->can_do & 1)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        if (e->platform != 0) continue;
        dx = e->XYZ.x - hx;
        dz = e->XYZ.z - hz;
        if (dx * dx + dz * dz >= lim) continue;
        if (um != 0 && angle_diff_abs(Heroes[idx].hero_move->forward_angle,
                get_angle(e->XYZ.x - he->XYZ.x, e->XYZ.z - he->XYZ.z) ^ 0x800) >= um) continue;
        do {
            fux->player = idx;
            fux->will_do |= 1;
            return n;
        } while (0);
    }
    return 0;
}

s32 SpinHeroKickCollision(s32 arg0) {
    TrigEntry *tbl;
    VECTOR pos;
    NAS *own;
    EXTengine *obj;
    s32 hit;
    s32 top;
    s32 push;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 rad2;
    s32 hi2;
    s32 lo2;
    s32 rad3;
    s32 lo3;
    s32 hi3;
    u32 i;
    HeroInfo *self;
    HeroInfo *other;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *rec;
    FlagRecord *box;
    NAS *n;
    EXTobject *t;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 rr;
    s32 ang;
    s32 a;
    s32 tmp;

    hit = 0;
    lo2 = 0;
    hi2 = 0;
    rad2 = 0;
    hi3 = 0;
    lo3 = 0;
    rad3 = 0;
    eng = Heroes[arg0].hero_eng;
    obj = NULL;
    if (eng != NULL) {
        box = FindCollisionBox(eng, 0x40);
        if (box != NULL) {
            rad = box->radius;
            hi = eng->XYZ.y + box->dy;
            lo = eng->XYZ.y + box->height;
            box = FindCollisionBox(eng, 1);
            if (box != NULL) {
                if (eng->xform.dirty != 0) {
                    rotscale_update(&eng->xform);
                }
                gfx_load_xform(eng->sub, &eng->xform.mtx, &eng->XYZ);
                lo2 = eng->XYZ.y + box->dy;
                hi2 = eng->XYZ.y + box->height;
                if (lo2 < hi2) {
                    tmp = hi2;
                    hi2 = lo2;
                    lo2 = tmp;
                }
                rad2 = box->radius;
                gte_rotate_point((SVECTOR *)box, &pos);
            }
            if (Heroes[arg0].hero_object != NULL) {
                n = ((NAS **)Heroes[arg0].hero_object)[-1];
                obj = n->Engine;
                if (obj != NULL) {
                    rad3 = 0x80;
                    hi3 = obj->XYZ.y;
                    lo3 = hi3 - 0x100;
                }
            }
            other = Heroes;
            i = 0;
            self = &Heroes[arg0];
            do {
                tbl = rcossin_tbl;
                if (arg0 != i && !(other[i].hero_status & 0xF800)) {
                    if (self->hero_move->forward_speed != 0) {
                        push = 1;
                    } else {
                        push = other[i].hero_move->forward_speed == 0;
                    }
                    if (VelocityCalc < 4) {
                        if (VelocityCalc >= 2) {
                            push = 1;
                        }
                    }
                    e = other[i].hero_eng;
                    if (e != NULL) {
                        rec = FindCollisionBox(e, 0x40);
                        if (rec != NULL) {
                            bot = e->XYZ.y + rec->dy;
                            top = e->XYZ.y + rec->height;
                            if (push != 0) {
                                r = rec->radius;
                                r += rad;
                                rr = r * r;
                                if (lo < bot && top < hi) {
                                    dx = e->XYZ.x - eng->XYZ.x;
                                    dz = e->XYZ.z - eng->XYZ.z;
                                    d = dx * dx + dz * dz;
                                    if (d < rr) {
                                        d = csqrt(d) >> 6;
                                        if (d < r) {
                                            ang = get_angle(dx, dz) & 0xFFF;
                                            d = r - d + 1;
                                            eng->XYZ.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->XYZ.z -= (d * tbl[ang].unk2) >> 12;
                                            hit = 1;
                                            if (self->hero_pu != 0) {
                                                (*(void (**)(s32, s32))&HeroHooks.HeroesTouched)(arg0, i);
                                            }
                                            if (other[i].hero_pu != 0) {
                                                (*(void (**)(s32, s32))&HeroHooks.HeroesTouched)(i, arg0);
                                            }
                                        }
                                    }
                                }
                            }
                            if (rad2 != 0 && hi2 < bot && top < lo2) {
                                dx = e->XYZ.x - pos.vx;
                                dz = e->XYZ.z - pos.vz;
                                r = rec->radius;
                                r += rad2;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->XYZ.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->XYZ.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->XYZ.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->XYZ.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                            if (rad3 != 0 && lo3 < bot && top < hi3) {
                                dx = e->XYZ.x - obj->XYZ.x;
                                dz = e->XYZ.z - obj->XYZ.z;
                                r = rec->radius;
                                r += rad3;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->XYZ.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->XYZ.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->XYZ.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->XYZ.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                i++;
            } while (i < 4);
            own = NULL;
            n = HeadOfSolids;
            if (Heroes[arg0].hero_object != NULL) {
                own = ((NAS **)Heroes[arg0].hero_object)[-1];
            }
            rad = 0x78;
            push = Heroes[arg0].hero_move->forward_speed != 0;
            if (VelocityCalc < 4) {
                if (VelocityCalc >= 2) {
                    push = 1;
                }
            }
            for (; n != NULL; n = n->Solid.Child) {
                if (!(n->Class & 0x200)) continue;
                if (n == own) continue;
                e = n->Engine;
                if (e == NULL) continue;
                if (e == eng) continue;
                rec = FindCollisionBox(e, 0x40);
                if (rec == NULL) continue;
                bot = e->XYZ.y + rec->dy;
                top = e->XYZ.y + rec->height;
                if (lo < bot && top < hi) {
                    dx = e->XYZ.x - eng->XYZ.x;
                    dz = e->XYZ.z - eng->XYZ.z;
                    r = rec->radius;
                    r += rad;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            a = get_angle(dx, dz);
                            ang = a & 0xFFF;
                            if (!(a & 0xFF)) {
                                ang -= 8 - Random(0x10);
                            }
                            d = r - d + 1;
                            if (push != 0) {
                                eng->XYZ.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->XYZ.z -= (d * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
                            }
                            t = FindExtension(n, 0x1000);
                            if (t != NULL && (t->can_do & 8)) {
                                t->player = arg0;
                                t->will_do |= 8;
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

s32 SpinHeroCarryCollision(s32 idx) {
    VECTOR v;
    subengine *sub;
    s32 hit;
    s32 top;
    s32 bot;
    EXTengine *eng;
    EXTengine *other;
    FlagRecord *box;
    FlagRecord *rec;
    NAS *nas;
    NAS *self;
    EXTobject *ext;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 ang;
    s32 lo;
    s32 hi;

    eng = Heroes[idx].hero_eng;
    hit = 0;
    if (eng != NULL) {
        box = FindCollisionBox(eng, 0x40);
        if (box != NULL) {
            bot = eng->XYZ.y + box->dy;
            top = eng->XYZ.y + box->height;
            box = FindCollisionBox(eng, 1);
            if (box != NULL) {
                if (eng->xform.dirty != 0) {
                    rotscale_update(&eng->xform);
                    sub = eng->sub;
                    eng->sub = sub;
                    gfx_load_xform(sub, &eng->xform.mtx, &eng->XYZ);
                } else {
                    sub = eng->sub;
                    eng->sub = sub;
                    gfx_load_xform(sub, &eng->xform.mtx, &eng->XYZ);
                }
                gte_rotate_point((SVECTOR *)box, &v);
            }
            nas = HeadOfSolids;
            self = NULL;
            if (Heroes[idx].hero_object != NULL) {
                self = ((NAS **)Heroes[idx].hero_object)[-1];
            }
            if (nas != NULL) {
                do {
                    if ((nas->Class & 0x200) && nas != self) {
                        other = nas->Engine;
                        if (other != NULL && other != eng) {
                            ext = FindExtension(nas, 0x1000);
                            if (ext != NULL && (ext->status == 1 || ext->status == 0xB)) {
                                rec = FindCollisionBox(other, 0x40);
                                if (rec != NULL) {
                                    lo = other->XYZ.y + rec->dy;
                                    hi = other->XYZ.y + rec->height;
                                    if (top < lo && hi < bot) {
                                        dx = other->XYZ.x - eng->XYZ.x;
                                        dz = other->XYZ.z - eng->XYZ.z;
                                        r = rec->radius;
                                        r += 0x78;
                                        d = dx * dx + dz * dz;
                                        if (d < r * r) {
                                            d = csqrt(d) >> 6;
                                            if (d < r) {
                                                ang = get_angle(dx, dz) & 0xFFF;
                                                d = r - d + 1;
                                                eng->XYZ.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->XYZ.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                                                hit = 1;
                                                if (ext->can_do & 8) {
                                                    ext->player = idx;
                                                    ext->will_do |= 8;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    nas = nas->Solid.Child;
                } while (nas != NULL);
            }
        }
    }
    return hit;
}

EXTobject *SpinHeroObjectNear(s32 hero, u16 mask, s32 r) {
    EXTobject *best = NULL;
    NAS *n = HeadOfObjects;
    EXTengine *eng = Heroes[hero].hero_eng;
    s32 cx = eng->XYZ.x;
    s32 cz = eng->XYZ.z;
    s32 px = cx - (rcossin_tbl[(eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    s32 pz = cz - (rcossin_tbl[eng->xform.rot.vy & 0xFFF].unk2 * 1024 >> 14);
    s32 bestd;
    s32 lim, dx, dz, d;
    EXTobject *f;

    r = r * r;
    bestd = r * 2;

    for (; n != NULL; n = n->Object.Child) {
        f = (EXTobject *)n->FUX;
        if (f != NULL && (f->can_do & mask) && n->Engine != NULL) {
            if (f->whatami == 2) {
                lim = 0x10000;
                dx = n->Engine->XYZ.x - px;
                dz = n->Engine->XYZ.z - pz;
            } else {
                lim = r;
                dx = n->Engine->XYZ.x - cx;
                dz = n->Engine->XYZ.z - cz;
            }
            d = dx * dx + dz * dz;
            if (d < lim && d < bestd) {
                bestd = d;
                best = f;
            }
        }
    }
    return best;
}

void SpinHitHeroes(EXTengine *eng, SpinHitMask *m, u32 flag, s32 hero) {
    FlagRecord *r;
    FlagRecord *r2;
    HeroInfo *h;
    EXTengine *e2;
    s32 top, bot, dx, dz, rr, y, lo, hi;
    s32 rad;

    r = FindCollisionBox(eng, 0x40);
    if (r != NULL) {
        top = eng->XYZ.y + r->dy;
        rad = r->radius;
        h = &Heroes[hero];
        bot = eng->XYZ.y + r->height;
        if (!(h->hero_status & 0xF800)) {
            e2 = h->hero_eng;
            if (e2 != NULL) {
                r2 = FindCollisionBox(e2, 0x40);
                if (r2 != NULL) {
                    lo = e2->XYZ.y + r2->dy;
                    hi = e2->XYZ.y + r2->height;
                    if (bot < lo && hi < top) {
                        dx = e2->XYZ.x - eng->XYZ.x;
                        dz = e2->XYZ.z - eng->XYZ.z;
                        rr = r2->radius + rad;
                        rr *= rr;
                        if (dx * dx + dz * dz < rr && m != NULL && (m->mask & flag)) {
                            m->hit |= flag;
                        }
                    }
                }
            }
        }
    }
}

void SetSpinHeroFall(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
}

void SpinHeroFlip(s32 arg0) {
    HeroHooks.HeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SpinCreateShadow(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *shadow = h->hero_shadow;
    EXTengine *eng;

    shadow->sub->u6C.dbase = &HeroModelBank[DB_RingObjects];
    shadow->sub->entity = SpinShadowModels[arg0];
    eng = h->hero_eng;
    shadow->XYZ = eng->XYZ;
    shadow->flags |= 0x8021;
    shadow->sub->unk76 = 0xE00;
}

s32 SpinHeroTestFlip(s32 arg0) {
    if (!(Pads[arg0].dirPressed & 0x4000)) {
        return 0;
    }
    SetHeroAction(arg0, 0x34);
    return 1;
}

s32 SpinTestEnd(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction(arg0, act);
    return 1;
}

s32 SpinTestEndB(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x36;
    if (!(st & 0x200)) {
        act = 0x37;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction(arg0, act);
    return 1;
}
