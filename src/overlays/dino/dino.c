#include "common.h"
#include "bash/dino_ovl.h"
#include "bash/engine.h"
#include "bash/dino.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char DinoAnimRun[] = "RUN";
const char DinoAnimBreathe[] = "BREATHE";
const char DinoAnimRecoil[] = "RECOIL";
const char DinoAnimBarge[] = "BARGE";
const char DinoAnimWin[] = "WIN";

/* data, in address order */
hero_seq CRA_seq_dinowin[18] = {
    0, 0x8007, 0xC, 0x8008, 0x6C5, 0x4180, 0x8008, 0x6A6, 0x8001, 0, 0x8008, 0x6C5, 0x418C, 0x8007,
    0x12, 0x418C, 0x8000, 0,
};
hero_seq CRA_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq CRA_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq CRA_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq CRA_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq COC_seq_dinowin[8] = {
    0, 0x8007, 0x24, 0x8008, 0x6C5, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq COC_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq COC_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4180, 0x8004, 0x63,
};
hero_seq COC_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq CTX_seq_dinowin[16] = {
    0, 0x8007, 0xC, 0x8008, 0x6C4, 0x4180, 0x8001, 0, 0x418C, 0x8008, 0x6C4, 0x8007, 0x12, 0x418C,
    0x8000, 0,
};
hero_seq CTX_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq CTX_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq CTX_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq CTX_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8004, 0x63,
};
hero_seq BRO_seq_dinowin[20] = {
    0, 0x8008, 0x6C7, 0x8007, 0xC, 0x4175, 0x8008, 0x6A7, 0x8001, 0, 0x8008, 0x6C7, 0x4181, 0x8007,
    0x12, 0x4181, 0x8008, 0x6A7, 0x8000, 0,
};
hero_seq BRO_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq BRO_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq BRO_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq BRO_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq KAN_seq_dinowin[16] = {
    0, 0x8007, 0xC, 0x8008, 0x6C7, 0x4180, 0x8001, 0, 0x418C, 0x8008, 0x6C7, 0x8007, 0x12, 0x418C,
    0x8000, 0,
};
hero_seq KAN_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq KAN_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq KAN_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq KAN_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq DNG_seq_dinowin[16] = {
    0, 0x8007, 0xC, 0x8008, 0x6C4, 0x4180, 0x8001, 0, 0x418C, 0x8008, 0x6C4, 0x8007, 0x12, 0x418C,
    0x8000, 0,
};
hero_seq DNG_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq DNG_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq DNG_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq DNG_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq TIN_seq_dinowin[16] = {
    0, 0x8007, 0xA, 0x4180, 0x8008, 0x6C7, 0x8001, 0, 0x418A, 0x8008, 0x6C7, 0x8007, 0x24, 0x418B,
    0x8000, 0,
};
hero_seq TIN_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq TIN_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq TIN_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq TIN_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq KON_seq_dinoidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_dinorun[6] = {
    0, 0x8007, 0x1A, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_dinoslide[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x8004, 0x63, 0,
};
hero_seq KON_seq_dinoshoot[6] = {
    0, 0x8007, 0x10, 0x4200, 0x8004, 0x63,
};
hero_seq KON_seq_dinodazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x63,
};
hero_seq KON_seq_dinowin[8] = {
    0, 0x8007, 0x24, 0x8008, 0x6C6, 0x4180, 0x8000, 0,
};
u16 DinoAiSkill[10] = {
    0x13, 0x10, 0xE, 0xC, 0x1C, 0x1E, 0x1E, 0x10, 0x10, 0x20,
};
u16 DinoAiSkillAlt[10] = {
    0x1A, 0x17, 0x14, 0x11, 0x1C, 0x1E, 0x1E, 0x10, 0x10, 0x20,
};
u16 DinoAiTeamSkill[6] = {
    8, 0x10, 0x14, 8, 0x10, 0x14,
};
u16 DinoAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 DinoAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 8, 7, 8, 0x10, 0x10, 2,
};
u16 DinoAiHumanTweak[42] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C,
    0xE, 0x13, 0x18, 0xC, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 8, 7, 8, 0x10, 0x10, 2, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
s32 DinoGem0 = 0;
NAS *DinoGem1 = 0;
NAS *DinoGem2 = 0;
NAS *DinoGems[3] = { NULL, NULL, NULL };
/* the shell settings (engine world record 0x010B) */
shell_setup dino_shell_info = {
    0x1000, 0x10, 0x2000, 0xB4, 0xCE, 0x9C, VramAreas, VramNodePool, 0x20, 0x514,
};
s16 DinoPreLoadDB[4] = { 0x1C, 0xC4, -1, 0 };
char *DinoAnims[6] = {
    (char *)DinoAnimRun, (char *)DinoAnimBreathe, (char *)DinoAnimRecoil, (char *)DinoAnimBarge,
    (char *)DinoAnimWin, NULL,
};
s32 DinoVictorySeqs[8] = {
    (s32)&DinoAnimWin, 0x56, (s32)&DinoAnimWin, 0x56, (s32)&DinoAnimWin, 0x56, (s32)&DinoAnimWin,
    0x56,
};
u16 DinoHeroModels[10] = {
    0, 0xBB, 0xBF, 0xBC, 0xC1, 0xBD, 0xC2, 0xBE, 0xC0, 0,
};
s16 DinoLevelDB[2] = { -1, 0 };
WWInf DinoWorldInfo[2] = {
    { 0xC3, DinoAnims, DinoLevelDB, 1 },
    { 0xBA, DinoAnims, DinoLevelDB, 1 },
};
s32 DinoFiles[15] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 351], (s32)&DatIndex[2 * 350], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 347],
    (s32)&DatIndex[2 * 346], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 349], (s32)&DatIndex[2 * 348], (s32)&HeroModelBank[DB_DragonObjects],
    (s32)&DatIndex[2 * 345], (s32)&DatIndex[2 * 344], 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 0x1C's) */
file_group_load_info file_group_DINO = { &FileGroups[9], DinoFiles };
u16 DinoLUPtab[10] = { 3, 0x20D, 0x1601, 0x1603, 0x1605, 0x1607, 0x160A, 0x1619, 0x1703, 0 };
TrigLookUp DinoLUP = { DinoLUPtab, 9 };
PreMap DINO_PREMAP = { DinoPreLoadDB, DinoWorldInfo, &DinoLUP };
s32 DinoArenaRadius = 0x658;
DinoObj *DinoGoal = 0;
s32 DinoWanderAngles[8] = { 0, 0x400, 0x800, 0xC00, 0x600, 0xA00, 0xE00, 0x200 };
u16 DinoWanderTime = 0xFA;
DinoObj *DinoTargetA = 0;
DinoObj *DinoTargetB = 0;
DinoObj *DinoTargetC = 0;
DinoObj *DinoTargetD = 0;
DinoObj *DinoTargetE = 0;
s32 DinoTargetTimer = 0;
s32 DinoTargetDelay = 0;
s32 DinoTargetStep = 0;
s32 DinoScoreTimer = 0;
s32 DinoTargetFlag = 0;
s32 DinoScoreFlag = 0;
s32 DinoGemTimer = 0;
s32 DinoScoreDelay = 0;
s32 DinoScoreA = 0;
s32 DinoScoreB = 0;
s32 DinoSmokeSteps[17] = {
    0x2004, 0x12004, 0x2006, 0x2007, 0x2008, 0x2009, 0x200A, 0x200B, 0x200C, 0x200D, 0x200E,
    0x200F, 0x2010, 0x2011, 0x2012, 0x2013, 0x8003,
};
s32 DinoSmokeAnimList[3] = { (s32)&DinoSmokeSteps, 0, 0 };
ScriptInit DinoSmokeScriptInit = { 0, (void *)&DinoSmokeAnimList, 0x402 };
s32 DinoSteerMin = 0x54;
s32 DinoSteerMax = 0x6A;
s32 DinoSteerTable[20] = {
    0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x4E, 0x52, 0x56, 0x5A, 0x5E, 0x62,
    0x66, 0x6A, 0, 0, 0,
};
DinoObj *DinoTargetObjs[3] = {0};
DinoObj *DinoTargetObjsB[3] = {0};
s32 DinoGemHooks[1] = { (s32)DinoInitGems };

s32 DinoAI_PickVictim(s32 arg0) {
    s32 start;
    s32 r;
    s32 i;
    s32 j;
    s32 k;
    s32 total;
    s32 result;
    s32 v;
    s32 neg;

    start = Random(4);
    r = Random(0x1000);
    neg = -1;
    i = start;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        if ((i & 3) == arg0) {
            goto next;
        }
        j = i & 3;
        result = 0;
        total = 0;
        if (Heroes[j].hero_status & 0x2000) {
            v = -1;
        } else {
            for (k = 0; k < 4; k++) {
                if (k != arg0 && !(Heroes[k].hero_status & 0x2000)) {
                    total += AiPlayerSkill[k].quality << 7;
                }
            }
            if (total != 0) {
                result = (AiPlayerSkill[j].quality << 19) / total;
            }
            v = result;
        }
        if (r < v) {
            return neg = i & 3;
        }
    next:
        i++;
    }
    return neg;
}

void DinoAI_SteerToTarget(id)
s32 id;
{
    HeroInfo *h;
    HeroInfo *h2;
    DinoChase *c;
    EXTbotai *ai;
    u32 pad;
    u32 dir;
    u16 ang;
    u16 t;
    s32 a;
    s32 b;
    SLONG_XYZ *p;
    SLONG_XYZ *q;
    HeroInfo *base;
    s32 off;

    base = Heroes;
    off = id * sizeof(HeroInfo);
    h = (HeroInfo *)(off + (u8 *)base);
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        p = &c->pos;
        q = &c->tgt;
        dir = (p->x > q->x) << 7;
        if (p->x < q->x) {
            dir |= 0x20;
        }
        a = p->z;
        b = q->z;
        if (b < a) {
            dir |= 0x40;
        }
        if (a < b) {
            dir |= 0x10;
        }
        h->hero_ai->AIpad |= dir;
        if (h->hero_frozen == 0) {
            ((DinoPadAng *)Pads)[id].buttons = h->hero_ai->AIpad;
        }
        ai = h->hero_ai;
        t = ai->AItimer1--;
        if (t == 0) {
            pad = h->hero_ai->AIpad;
            ang = 0;
            if (pad & 0x80) {
                ang = 0xC00;
            }
            if (pad & 0x20) {
                ang = 0x400;
            }
            if (pad & 0x10) {
                ang = 0;
            }
            if (pad & 0x40) {
                ang = 0x800;
            }
            if ((pad & 0x90) == 0x90) {
                ang = 0xE00;
            }
            if ((pad & 0xC0) == 0xC0) {
                ang = 0xA00;
            }
            if ((pad & 0x30) == 0x30) {
                ang = 0x200;
            }
            if ((pad & 0x60) == 0x60) {
                ang = 0x600;
            }
            h2 = (HeroInfo *)(off + (s32)base);
            h2->hero_ai->AIang = ang;
            h2->hero_ai->AItimer1 = 0;
        }
        if (Heroes[id].hero_frozen == 0) {
            ((DinoPadAng *)Pads)[id].angle = Heroes[id].hero_ai->AIang;
        }
    }
}

s32 DinoAI_NearestGem(s32 arg0) {
    s32 best = -1;
    s32 bestd = 0x7FFFFFFF;
    s32 start = Random(3);
    s32 i = start;
    s32 off;
    s32 idx;
    s32 dx;
    s32 dz;
    s32 d;
    DinoExt *ext;
    EXTengine *a;
    EXTengine *b;

    for (;;) {
        if (i >= start + 3) {
            goto done;
        }
        off = 0;
        if (i >= 3) {
            off = 3;
        }
        ext = FindExtension(DinoGems[i - off], 0x10000000);
        if (ext != NULL) {
            s32 v = ext->unkC;
            if (v == 0 || v == 3 || v == 4) {
                idx = i - off;
                b = Heroes[arg0].hero_eng;
                a = DinoGems[idx]->Engine;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                d = dx * dx + dz * dz;
                if (d < bestd) {
                    bestd = d;
                    best = idx;
                }
            }
        }
        i++;
    }
done:
    return best;
}

s32 DinoAI_PickGem(s32 arg0) {
    s32 best = -1;
    s32 bestd = 0x7FFFFFFF;
    s32 start = Random(3);
    s32 i = start;
    s32 off;
    s32 idx;
    s32 dx;
    s32 dz;
    s32 d;
    s32 t;
    DinoExt *ext;
    EXTengine *a;
    EXTengine *b;

    for (;;) {
        if (i >= start + 3) {
            goto done;
        }
        off = 0;
        if (i >= 3) {
            off = 3;
        }
        idx = i - off;
        ext = FindExtension(DinoGems[idx], 0x10000000);
        if (ext != NULL && ext->unkC == 1) {
            t = DinoAI_PickVictim(arg0);
            if (t == ext->unk10 && DinoTestTeamMate(arg0, t) == 0) {
                b = Heroes[arg0].hero_eng;
                a = DinoGems[idx]->Engine;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                d = dx * dx + dz * dz;
                if (d < bestd) {
                    bestd = d;
                    best = idx;
                }
            }
        }
        i++;
    }
done:
    return best;
}

void DinoAI_ChaseGem(s32 id) {
    HeroInfo *h;
    HeroInfo *hb;
    DinoChase *c;
    DinoExt *e;
    EXTengine *b;
    EXTengine *a;
    s32 near;
    s32 far;
    s32 ok;
    s32 t;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        t = DinoAI_NearestGem(id);
        if (t != -1) {
            c->unk28 = t;
        }
        e = FindExtension(DinoGems[c->unk28], 0x10000000);
        if (e != NULL) {
            t = e->unkC;
            if (t == 0 || t == 3 || t == 4) {
                near = 1;
            } else {
                near = 0;
            }
        } else {
            near = 0;
        }
        if ((near ^ 1) != 0) {
            t = DinoAI_NearestGem(id);
            if (t != -1) {
                ok = 1;
                c->unk28 = t;
            } else {
                ok = 0;
            }
            if ((ok ^ 1) != 0) {
                c->unk28 = -1;
                c->fn = DinoUnusedStub;
                return;
            }
        }
        if (DinoAI_ShotBlocked(id) != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
        }
        b = Heroes[id].hero_eng;
        c->pos = b->unk10;
        a = DinoGems[c->unk28]->Engine;
        c->tgt = a->unk10;
        DinoAI_SteerToTarget(id);
    }
}

void DinoAI_GuardGem(s32 id) {
    HeroInfo *h;
    HeroInfo *hb;
    DinoChase *c;
    DinoExt *e;
    EXTengine *b;
    EXTengine *a;
    s32 near;
    s32 far;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        e = FindExtension(DinoGems[c->unk28], 0x10000000);
        near = 0;
        if (e != NULL) {
            near = e->unkC == 1;
        }
        far = near ^ 1;
        if (far != 0) {
            c->unk28 = -1;
            c->fn = DinoUnusedStub;
            return;
        }
        b = h->hero_eng;
        c->pos = b->unk10;
        a = DinoGems[c->unk28]->Engine;
        c->tgt = a->unk10;
        if (DinoAI_ShotBlocked(id) != 0) {
            h->hero_ai->AIpad |= 0x8000;
        }
        ((PadButtons *)Pads)[id].unk50 = h->hero_ai->AIpad;
        DinoAI_SteerToTarget(id);
    }
}

s32 DinoRayHit();

void DinoAI_ShootGem(s32 id) {
    DinoChase *c;
    DinoExt *e;
    s32 near;
    s32 hit;
    s32 yaw;
    s32 sc;
    DinoPosH *p;
    s32 v1[3];
    s32 v2[3];
    s16 d[8];

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        hit = 0;
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        if ((near ^ 1) != 0) {
            c->unk28 = -1;
            c->fn = DinoUnusedStub;
            return;
        }
        if (DinoAI_ShotBlocked(id) != 0) {
            c->timer = Random(0xF) + 0x3C;
            c->state = 4;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = ((DinoPos *)DinoGoal)->pos;
        DinoAI_SteerToTarget(id);
        yaw = Heroes[id].hero_eng->unk1E;
        sc = DinoArenaRadius * 4;
        v1[0] = (sc * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2) >> 12;
        v1[2] = (sc * rcossin_tbl[yaw & 0xFFF].unk2) >> 12;
        v2[0] = 0;
        v2[2] = 0;
        p = (DinoPosH *)DinoGoal;
        d[0] = p->x - ((DinoEngH *)Heroes[id].hero_eng)->x;
        d[2] = p->z - ((DinoEngH *)Heroes[id].hero_eng)->z;
        d[1] = 0;
        d[3] = 0x300;
        if (DinoRayHit(v2, v1, d) == 1) {
            hit = 1;
        }
        if (hit != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
            ((DinoPad *)Pads)[id].unk50 = Heroes[id].hero_ai->AIpad;
            c->unk28 = -1;
            c->unk30 = 8;
            c->fn = DinoUnusedStub;
            c->state = 0;
        }
    }
}

void DinoAI_CarryGem(s32 id) {
    DinoChase *c;
    DinoExt *e;
    s32 near;
    s32 hit;
    s32 yaw;
    s32 sc;
    s32 r;
    DinoPosH *p;
    s32 v1[3];
    s32 v2[3];
    s16 d[8];

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        hit = 0;
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        if ((near ^ 1) != 0) {
            c->unk28 = -1;
            c->fn = DinoUnusedStub;
            return;
        }
        r = DinoFloorKind(id);
        if (r != 1) {
            c->timer = 0x3C;
            c->state = 4;
            c->fn = DinoAI_Wander;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = ((DinoPos *)DinoGoal)->pos;
        if (c->unk30-- > 0) {
            DinoAI_SteerToTarget(id);
        }
        if (c->unk30 < -2) {
            c->unk30 = 8;
        }
        yaw = Heroes[id].hero_eng->unk1E;
        sc = DinoArenaRadius * 4;
        v1[0] = (sc * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2) >> 12;
        v1[2] = (sc * rcossin_tbl[yaw & 0xFFF].unk2) >> 12;
        v2[0] = 0;
        v2[2] = 0;
        p = (DinoPosH *)DinoGoal;
        d[0] = p->x - ((DinoEngH *)Heroes[id].hero_eng)->x;
        d[2] = p->z - ((DinoEngH *)Heroes[id].hero_eng)->z;
        d[1] = 0;
        d[3] = 0x300;
        if (DinoRayHit(v2, v1, d) == r) {
            hit = 1;
        }
        if (hit != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
            ((DinoPad *)Pads)[id].unk50 = Heroes[id].hero_ai->AIpad;
            c->unk28 = -1;
            c->unk30 = 8;
            c->fn = DinoUnusedStub;
            c->state = 0;
        }
    }
}


s32 DinoRayHit(a, b, c, unused)
    s32 *a;
    s32 *b;
    s16 *c;
    void *unused;
{
    s32 q[3];
    s32 p[3];
    s32 *qp;
    s32 A;
    s32 B;
    s32 C;
    s32 disc;
    s32 s;
    s32 t1;
    s32 nc;
    s32 den;
    s32 r1;
    s32 r2;
    s32 t0;

    p[0] = b[0] - a[0];
    p[1] = 0;
    p[2] = b[2] - a[2];
    A = (p[0] * p[0] + p[2] * p[2]) >> 12;
    q[0] = c[0] - a[0];
    q[1] = 0;
    q[2] = c[2] - a[2];
    qp = q;
    t0 = p[0] * q[0];
    B = (q[0] * q[0] + qp[1] * qp[1] + qp[2] * qp[2] - c[3] * c[3]) >> 12;
    C = ((t0 + p[2] * qp[2]) * 2) >> 12;
    s = C * C - A * B * 4;
    if (C * C < 0) {
        printf("error in b\n");
    }
    if (s < 0) {
        return 0;
    }
    s = csqrt(s) >> 6;
    nc = -C;
    t1 = (nc + s) << 8;
    den = A * 2;
    r1 = t1 / den;
    r2 = ((nc - s) << 8) / den;
    if (r1 < 0) {
        if (r2 < 0) {
            return 0;
        }
    }
    if (r1 < r2) {
        if (r1 < 0) {
            if (r2 < 0x100) {
                return 1;
            }
        }
    } else if (r2 > 0) {
        if (r2 < 0x100) {
            return 1;
        }
    }
    r2 = r1 < 0x100;
    if (r2) {
        goto one;
    }
    t0 = 0;
    return t0;
one:
    return 1;
}

s32 DinoRayHitB(s32 *a, s32 *b, s16 *c, s32 *d) {
    s32 rel[3];
    s32 dir[3];
    s32 qa;
    s32 qb;
    s32 qc;
    s32 disc;
    s32 t0;
    s32 t1;
    s32 *p;

    dir[0] = b[0] - a[0];
    dir[1] = 0;
    dir[2] = b[2] - a[2];
    rel[0] = c[0] - a[0];
    rel[1] = 0;
    rel[2] = c[2] - a[2];
    qa = (dir[0] * dir[0] + dir[2] * dir[2]) >> 8;
    p = rel;
    qb = ((dir[0] * p[0] + dir[2] * p[2]) * 2) >> 8;
    qc = (p[0] * p[0] + p[1] * p[1] + p[2] * p[2] - c[3] * c[3]) >> 8;
    disc = qb * qb - qa * qc * 4;
    if (disc >= 0) {
        disc = csqrt(disc) >> 6;
        t0 = ((-qb + disc) << 8) / (qa * 2);
        t1 = ((-qb - disc) << 8) / (qa * 2);
        if (t0 < 0 && t1 < 0) {
            return 0;
        }
        if (t0 < t1) {
            if (t0 < 0) {
                d[0] = a[0] + ((dir[0] * t1) >> 8);
                d[1] = a[1] + ((dir[1] * t1) >> 8);
                d[2] = a[2] + ((dir[2] * t1) >> 8);
                return 1;
            } else {
                d[0] = a[0] + ((dir[0] * t0) >> 8);
                d[1] = a[1] + ((dir[1] * t0) >> 8);
                d[2] = a[2] + ((dir[2] * t0) >> 8);
                return 1;
            }
        } else if (t1 > 0) {
            d[0] = a[0] + ((dir[0] * t1) >> 8);
            d[1] = a[1] + ((dir[1] * t1) >> 8);
            d[2] = a[2] + ((dir[2] * t1) >> 8);
            return 1;
        } else {
            d[0] = a[0] + ((dir[0] * t0) >> 8);
            d[1] = a[1] + ((dir[1] * t0) >> 8);
            d[2] = a[2] + ((dir[2] * t0) >> 8);
            return 1;
        }
    }
    return 0;
}

s32 DinoAI_RayToPlayer(s32 a, s32 b) {
    s32 v1[3];
    s32 v2[3];
    s16 d[8];
    EXTengine *ea;
    EXTengine *eb;
    s32 yaw;

    ea = Heroes[a].hero_eng;
    yaw = ea->unk1E;
    eb = Heroes[b].hero_eng;
    v1[0] = (DinoArenaRadius * rcossin_tbl[(yaw - 0x400) & 0xFFF].unk2) >> 12;
    v1[2] = (DinoArenaRadius * rcossin_tbl[yaw & 0xFFF].unk2) >> 12;
    v2[0] = 0;
    v2[2] = 0;
    d[0] = *(u16 *)((u8 *)eb + 0x10) - *(u16 *)((u8 *)ea + 0x10);
    d[2] = *(u16 *)((u8 *)eb + 0x18) - *(u16 *)((u8 *)ea + 0x18);
    d[1] = 0;
    d[3] = 0xC8;
    return DinoRayHit(v2, v1, d, ea) == 1;
}

s32 DinoTestTeamMate(s32 arg0, s32 arg1) {
    u8 *g = GameInfo;

    if (*(s32 *)(g + 0x6C) != 0) {
        if (Heroes[arg1].hero_status & 0x2000) {
            return 0;
        }
        if (arg0 == *(s16 *)(g + 0x48) && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == GameTeamLeader[0] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
        if (arg0 == GameTeamLeader[1] && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == GameTeamLeader[1] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
    }
    return 0;
}

s32 DinoAI_ShotBlocked(s32 id)
{
  Vec3W pos;
  s32 best;
  s32 bestd;
  s32 i;
  s32 d;
  u16 st;
  s32 dist;
  s32 sel;
  s32 *pd = &dist;
  best = -1;
  bestd = 0x07FFFFFF;
  i = 0;
  for (;;)
  {
    if (i >= 4)
    {
      goto done;
    }
    if (i != id)
    {
      st = Heroes[i].hero_status;
      sel = st & 0x2000;
      if (!sel)
      {
        if (!(st & 0x8000))
        {
          pos = *((Vec3W *) (&Heroes[i].hero_eng->unk10));
          d = DistXZSq((SLONG_XYZ *)(&pos), (SLONG_XYZ *)(&Heroes[id].hero_eng->unk10));
          if (d < bestd)
          {
            bestd = d;
            best = i;
          }
        }
      }
    }
    i++;
  }

  done:
  *pd = bestd;

  sel = best;
  if (sel != (-1))
  {
    if (DinoAI_PickVictim(id) == sel)
    {
      if (DinoTestTeamMate(id, sel) == 0)
      {
        if (dist <= 0xFFFFF)
        {
          if (DinoAI_RayToPlayer(id, sel) != 0)
          {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

void DinoAI_ReturnGem(s32 id) {
    DinoChase *c;
    DinoExt *e;
    s32 near;
    s32 dist;
    s32 far;
    SLONG_XYZ sp;
    s32 dx;
    s32 dz;

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        sp.x = 0;
        sp.y = 0;
        sp.z = -0xB00;
        if (DinoAI_ShotBlocked(id) != 0) {
            c->timer = 0x5A;
            c->state = 4;
            return;
        }
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        far = near ^ 1;
        if (far != 0) {
            c->unk28 = -1;
            c->fn = DinoUnusedStub;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = sp;
        dx = c->pos.x - c->tgt.x;
        dz = c->pos.z - c->tgt.z;
        dist = dx * dx + dz * dz;
        DinoAI_SteerToTarget(id);
        if (dist <= 0x3FFFF) {
            c->state = 3;
        }
    }
}

s32 DinoGetLevelPoint(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

void DinoAI_Wander(s32 arg0) {
    s32 v1[4];
    s32 sp20[4];
    s32 v2[4];
    s16 d[8];
    struct {
        SLONG_XYZ v;
        s32 pad;
    } pos;
    s16 d2[8];
    s16 d3[8];
    s32 rnd;
    s32 best;
    s32 bestd;
    s32 i;
    s32 j;
    s32 found;
    s32 found2;
    s32 ang;
    s32 dx;
    s32 dz;
    HeroInfo *h;
    EXTheromove *mv;
    EXTengine *my;
    EXTengine *ve;
    DinoAiExt *ext;
    DinoExt *ext2;
    u32 ok;
    u32 bad;

    bestd = 0x7FFFFFFF;
    best = 0;
    ext = FindExtension(Heroes[arg0].hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    ext2 = FindExtension(Heroes[arg0].hero, 0x10000000);
    ok = 0;
    if (ext2 != NULL) {
        ok = 0 < (u32)ext2->unk14;
    }
    bad = ok ^ 1;
    if (bad != 0) {
        ext->unk28 = -1;
        ext->func = DinoUnusedStub;
        return;
    }
    i = 0;
    mv = Heroes[arg0].hero_move;
    ext->timer--;
    for (;;) {
        s32 dist;
        if (i >= 4) {
            break;
        }
        if (i != arg0 && !(Heroes[i].hero_status & 0x2000)) {
            pos.v = Heroes[i].hero_eng->unk10;
            dist = DistXZSq((SLONG_XYZ *)&pos.v, (SLONG_XYZ *)&Heroes[arg0].hero_eng->unk10);
            if (dist < bestd) {
                bestd = dist;
                best = i;
            }
        }
        i++;
    }
    ext->victim = best;
    if (mv != NULL) {
        mv->forward_angle = get_angle(Heroes[arg0].hero_eng->unk10.x - Heroes[best].hero_eng->unk10.x,
                                      Heroes[arg0].hero_eng->unk10.z - Heroes[best].hero_eng->unk10.z) & 0xFFF;
    }
    my = Heroes[arg0].hero_eng;
    ve = Heroes[ext->victim].hero_eng;
    d[0] = *(u16 *)&ve->unk10.x - *(u16 *)&my->unk10.x;
    d[2] = *(u16 *)&ve->unk10.z - *(u16 *)&my->unk10.z;
    d[1] = 0;
    d[3] = DinoWanderTime;
    dx = ve->unk10.x - my->unk10.x;
    dz = ve->unk10.z - my->unk10.z;
    if (dx * dx + dz * dz > 0x283E3F) {
        goto chase;
    }
    rnd = Random(8);
    v1[0] = (DinoArenaRadius * rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2) >> 12;
    v1[2] = (DinoArenaRadius * rcossin_tbl[ext->angle & 0xFFF].unk2) >> 12;
    best = 0;
    v2[0] = 0;
    v2[2] = 0;
    if (DinoRayHitB(v2, v1, d, sp20) == 0) {
        found2 = 0;
        j = 0;
        for (;;) {
            EXTengine *e2;
            if (j >= 4) {
                break;
            }
            e2 = Heroes[j].hero_eng;
            if (j != arg0 && j != ext->victim && !(Heroes[j].hero_status & 0x2000)) {
                d2[0] = *(u16 *)&e2->unk10.x - *(u16 *)&my->unk10.x;
                d2[2] = *(u16 *)&e2->unk10.z - *(u16 *)&my->unk10.z;
                d2[1] = 0;
                d2[3] = DinoWanderTime;
                if (DinoRayHitB(v2, v1, d2, sp20) == 1) {
                    found2 = 1;
                }
            }
            j++;
        }
        if (!found2) {
            pos.v.x = Heroes[arg0].hero_eng->unk10.x - v1[0];
            pos.v.z = Heroes[arg0].hero_eng->unk10.z - v1[2];
            if (DinoGetLevelPoint(&pos.v.x, 0) != 0) {
                ext->target = pos.v;
                ext->pos = Heroes[arg0].hero_eng->unk10;
                DinoAI_SteerToTarget(arg0);
                best = 1;
            }
        }
    }
    if (best != 1) {
        for (j = rnd; ; j++) {
            if (j >= rnd + 8) {
                goto end;
            }
            ang = DinoWanderAngles[j & 7];
            v1[0] = (DinoArenaRadius * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
            v1[2] = (DinoArenaRadius * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
            v2[0] = 0;
            v2[2] = 0;
            if (DinoRayHitB(v2, v1, d, sp20) != 0) {
                continue;
            }
            found = 0;
            i = 0;
            pos.v.x = Heroes[arg0].hero_eng->unk10.x - v1[0];
            h = Heroes;
            pos.v.z = Heroes[arg0].hero_eng->unk10.z - v1[2];
            for (;;) {
                EXTengine *e3;
                if (i >= 4) {
                    break;
                }
                e3 = h->hero_eng;
                if (i != arg0 && i != ext->victim) {
                    d3[0] = *(u16 *)&e3->unk10.x - *(u16 *)&my->unk10.x;
                    d3[2] = *(u16 *)&e3->unk10.z - *(u16 *)&my->unk10.z;
                    d3[1] = 0;
                    d3[3] = DinoWanderTime;
                    if (DinoRayHitB(v2, v1, d3, sp20) == 1) {
                        found = 1;
                    }
                }
                h++;
                i++;
            }
            if (!found && DinoGetLevelPoint(&pos.v.x, 0) != 0) {
                break;
            }
        }
        ext->target = pos.v;
        ext->pos = Heroes[arg0].hero_eng->unk10;
        DinoAI_SteerToTarget(arg0);
        ext->angle = ang;
    }
end:
    if (ext->timer > 0) {
        return;
    }
chase:
    ext->func = DinoAI_ShootGem;
    ext->unk38 = 1;
}

void DinoAI_Think(id)
s32 id;
{
    DinoChase *c;
    DinoExt *e;
    s32 near;
    s32 r;
    s32 r2;
    HeroInfo *h;
    HeroInfo *hb;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        h->hero_ai->AIpad = 0;
        h->hero_ai->AImode = 1;
        if (c->fn != DinoAI_ShootGem) {
            e = FindExtension(h->hero, 0x10000000);
            near = 0;
            if (e != NULL) {
                near = (u32)e->unk14 > 0;
            }
            if (near != 0) {
                if (c->state == 0) {
                    if (Random(0x1000) < 0x800) {
                        r = 2;
                    } else {
                        c->timer = 0x3C;
                        r = 4;
                    }
                    c->state = r;
                }
                if (c->state == 2) {
                    c->fn = DinoAI_ReturnGem;
                }
                if (c->state == 1) {
                    c->fn = DinoAI_ShootGem;
                }
                if (c->state == 4) {
                    c->fn = DinoAI_Wander;
                }
                if (c->state == 3) {
                    c->fn = DinoAI_CarryGem;
                }
            }
        }
        if (c->fn == DinoUnusedStub) {
            c->state = 0;
            r = DinoAI_NearestGem(id);
            if (r != -1) {
                c->unk28 = r;
                c->fn = DinoAI_ChaseGem;
            }
            r2 = DinoAI_PickGem(id);
            if (r2 != -1) {
                c->unk28 = r2;
                c->fn = DinoAI_GuardGem;
            }
        }
        c->fn(id);
    }
}

void DinoChooseWinnerScore(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (!(GameExitFlags & 0x801) && arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            GameInfo[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = GameInfo;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    Heroes[k].hero_status |= 0x100;
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                    }
                }
            } else {
                GameInfo[0x60] = a->hero_name;
                *(u16 *)(GameInfo + 0x30) |= 0x800;
                n = 0;
                for (;; n++) {
                    s32 t_;

                    if (n >= 4) {
                        break;
                    }
                    if (GameInfo[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x8400;
                        hb[n].hero_shadow->sub->flags &= ~0x8000;
                        DINO_PUFF(&hb[n]);
                    }
                }
            }
        } else {
            s32 ia;
            s32 ib;
            s32 ita;
            s32 itb;
            s32 sa;
            s32 sb;
            s32 p;
            s32 q;

            u8 *t;

            t = GameInfo;
            ia = *(s16 *)(t + 0x48);
            ib = *(s16 *)(t + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                t[0x60] = 0;
                *(u16 *)(t + 0x30) |= arg1;
                for (p = 0; p < 4; p++) {
                    Heroes[p].hero_status |= 0x100;
                }
            } else if ((sb < sa && t[ia + 0x5C] == 0) || (sa < sb && t[ib + 0x5C] == 0)) {
                u8 *w = GameInfo;

                *(u16 *)(w + 0x30) |= 0x800;
                if (w[ia + 0x5C] == 0) {
                    w[0x60] = Heroes[ia].hero_name;
                    w[0x61] = Heroes[ita].hero_name;
                    Heroes[ia].hero_status |= 0x200;
                    Heroes[ita].hero_status |= 0x200;
                    Heroes[ib].hero_status |= 0x400;
                    Heroes[itb].hero_status |= 0x400;
                    Heroes[ib].hero_status |= 0x8000;
                    DINO_PUFF(&Heroes[ib]);
                    Heroes[ib].hero_shadow->sub->flags &= ~0x8000;
                    Heroes[itb].hero_status |= 0x8000;
                    Heroes[itb].hero_shadow->sub->flags &= ~0x8000;
                    DINO_PUFF(&Heroes[itb]);
                } else {
                    w[0x60] = Heroes[ib].hero_name;
                    w[0x61] = Heroes[itb].hero_name;
                    Heroes[ib].hero_status |= 0x200;
                    Heroes[itb].hero_status |= 0x200;
                    Heroes[ia].hero_status |= 0x400;
                    Heroes[ita].hero_status |= 0x400;
                    Heroes[ia].hero_status |= 0x8000;
                    DINO_PUFF(&Heroes[ia]);
                    Heroes[ia].hero_shadow->sub->flags &= ~0x8000;
                    Heroes[ita].hero_status |= 0x8000;
                    Heroes[ita].hero_shadow->sub->flags &= ~0x8000;
                    DINO_PUFF(&Heroes[ita]);
                }
            } else {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (q = 0; q < 4; q++) {
                    Heroes[q].hero_status |= 0x100;
                }
            }
        }
    }
}

void DinoChooseWinnerOther(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (!(GameExitFlags & 0x801) && arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            GameInfo[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = GameInfo;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    Heroes[k].hero_status |= 0x100;
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                    }
                }
                CameraMode = 1;
            } else {
                GameInfo[0x60] = a->hero_name;
                *(u16 *)(GameInfo + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (GameInfo[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            s32 ia;
            s32 ib;
            s32 ita;
            s32 itb;
            s32 sa;
            s32 sb;
            s32 p;
            s32 q;

            ia = *(s16 *)(g + 0x48);
            ib = *(s16 *)(g + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (p = 0; p < 4; p++) {
                    Heroes[p].hero_status |= 0x100;
                }
            } else if ((sb < sa && g[ia + 0x5C] == 0) || (sa < sb && g[ib + 0x5C] == 0)) {
                u8 *w = GameInfo;

                *(u16 *)(w + 0x30) |= 0x800;
                if (w[ia + 0x5C] == 0) {
                    w[0x60] = Heroes[ia].hero_name;
                    w[0x61] = Heroes[ita].hero_name;
                    Heroes[ia].hero_status |= 0x200;
                    Heroes[ita].hero_status |= 0x200;
                    Heroes[ib].hero_status |= 0x400;
                    Heroes[itb].hero_status |= 0x400;
                } else {
                    w[0x60] = Heroes[ib].hero_name;
                    w[0x61] = Heroes[itb].hero_name;
                    Heroes[ib].hero_status |= 0x200;
                    Heroes[itb].hero_status |= 0x200;
                    Heroes[ia].hero_status |= 0x400;
                    Heroes[ita].hero_status |= 0x400;
                }
            } else {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (q = 0; q < 4; q++) {
                    Heroes[q].hero_status |= 0x100;
                }
            }
        }
    }
}

void DinoChooseWinnerPuffs(s32 arg0, u16 arg1)
{
  s32 best;
  s32 i;
  s32 j;
  s32 taken;
  NAS *new_var;
  u8 *e;
  s32 k;
  u8 *g;
  if ((!(GameExitFlags & 0x801)) && (arg0 == (-1)))
  {
    taken = 0;

    for (j = 0; j < 4; j++)
    {
      best = -1;
      for (i = 0; i < 4; i++)
      {
        if ((!((taken >> i) & 1)) && ((Heroes[i].hero_score > Heroes[best].hero_score) || (best == (-1))))
        {
          best = i;
        }
      }

      GameInfo[0xA7 + j] = best;
      taken |= 1 << best;
    }

    e = GameInfo;
    if ((*((s32 *) (e + 0x6C))) == 0)
    {
      HeroInfo *hb;
      HeroInfo *a;
      u8 *w;
      s32 n;
      hb = Heroes;
      a = &hb[e[0xA7]];
      if (a->hero_score == hb[e[0xA8]].hero_score)
      {
        e[0x60] = 0;
        *((u16 *) (e + 0x30)) |= arg1;
        for (k = 0; k < 4; k++)
        {
          hb[k].hero_status |= 0x100;
        }

      }
      else
      {
        n = 0;
        ;
        e[0x60] = a->hero_name;
        *((u16 *) (GameInfo + 0x30)) |= 0x800;
        for (;; n++)
        {
          s32 t_;
          if (n >= 4)
          {
            break;
          }
          if (GameInfo[0xA7] == n)
          {
            hb[n].hero_status |= 0x200;
          }
          else
          {
            hb[n].hero_status |= 0x8400;
            {
              NAS *nas_;
              DinoPuff *p_;
              SLONG_XYZ *v_;
              v_ = &(&hb[n])->hero_eng->unk10;
              nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
              if (nas_ != 0)
              {
                p_ = FindExtension(nas_, 0x10000000);
                if (p_ != 0)
                {
                  p_->unk18 = 0x100;
                  p_->unkC = 0;
                  p_->unk20 = 1;
                  p_->unk28 = 0;
                  nas_->InitDelete(nas_);
                }
              }
            }
            ;
            hb[n].hero_shadow->sub->flags &= ~0x8000;
          }
        }

      }
    }
    else
    {
      s32 ia;
      s32 ib;
      s32 ita;
      s32 itb;
      s32 sa;
      s32 sb;
      s32 p;
      HeroInfo *ha;
      HeroInfo *hb;
      HeroInfo *ta;
      HeroInfo *tb;
      HeroInfo *hh;
      g = GameInfo;
      hh = Heroes;
      ia = *((s16 *) (g + 0x48));
      ib = *((s16 *) (g + 0x4A));
      ha = &hh[ia];
      hb = &hh[ib];
      ita = ha->hero_teammate;
      itb = hb->hero_teammate;
      ta = &hh[ita];
      tb = &hh[itb];
      sa = ha->hero_score + (ta->hero_score ^ 0);
      sb = hb->hero_score + tb->hero_score;
      if (sa == sb)
      {
        g[0x60] = 0;
        *((u16 *) (g + 0x30)) |= arg1;
        for (p = 0; p < 4; p++)
        {
          Heroes[p].hero_status |= 0x100;
        }

      }
      else
      {
        *((u16 *) (g + 0x30)) |= 0x800;
        if (sb < sa)
        {
          g[0x60] = ha->hero_name;
          g[0x61] = ta->hero_name;
          ha->hero_status |= 0x200;
          ta->hero_status |= 0x200;
          hb->hero_status |= 0x400;
          tb->hero_status |= 0x400;
          hb->hero_status |= 0x8000;
          {
            NAS *nas_;
            DinoPuff *p_;
            SLONG_XYZ *v_;
            v_ = &hb->hero_eng->unk10;
            new_var = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
            nas_ = new_var;
            if (nas_ != 0)
            {
              p_ = FindExtension(nas_, 0x10000000);
              if (p_ != 0)
              {
                p_->unk18 = 0x100;
                p_->unkC = 0;
                p_->unk20 = 1;
                p_->unk28 = 0;
                nas_->InitDelete(nas_);
              }
            }
          }
          ;
          Heroes[itb].hero_status |= 0x8000;
          Heroes[ib].hero_shadow->sub->flags &= ~0x8000;
          {
            NAS *nas_;
            DinoPuff *p_;
            SLONG_XYZ *v_;
            v_ = &(&Heroes[itb])->hero_eng->unk10;
            nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
            if (nas_ != 0)
            {
              p_ = FindExtension(nas_, 0x10000000);
              if (p_ != 0)
              {
                p_->unk18 = 0x100;
                p_->unkC = 0;
                p_->unk20 = 1;
                p_->unk28 = 0;
                nas_->InitDelete(nas_);
              }
            }
          }
          ;
          Heroes[itb].hero_shadow->sub->flags &= ~0x8000;
        }
        else
        {
          g[0x60] = hb->hero_name;
          g[0x61] = tb->hero_name;
          hb->hero_status |= 0x200;
          tb->hero_status |= 0x200;
          ha->hero_status |= 0x400;
          ta->hero_status |= 0x400;
          ha->hero_status |= 0x8000;
          {
            NAS *nas_;
            DinoPuff *p_;
            SLONG_XYZ *v_;
            v_ = &ha->hero_eng->unk10;
            nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
            if (nas_ != 0)
            {
              p_ = FindExtension(nas_, 0x10000000);
              if (p_ != 0)
              {
                p_->unk18 = 0x100;
                p_->unkC = 0;
                p_->unk20 = 1;
                p_->unk28 = 0;
                nas_->InitDelete(nas_);
              }
            }
          }
          ;
          Heroes[ita].hero_status |= 0x8000;
          Heroes[ia].hero_shadow->sub->flags &= ~0x8000;
          Heroes[ita].hero_shadow->sub->flags &= ~0x8000;
          {
            NAS *nas_;
            DinoPuff *p_;
            SLONG_XYZ *v_;
            v_ = &(&Heroes[ita])->hero_eng->unk10;
            nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
            if (nas_ != 0)
            {
              p_ = FindExtension(nas_, 0x10000000);
              if (p_ != 0)
              {
                p_->unk18 = 0x100;
                p_->unkC = 0;
                p_->unk20 = 1;
                p_->unk28 = 0;
                nas_->InitDelete(nas_);
              }
            }
          }
          ;
        }
      }
    }

  }
}

void DinoChooseWinnerTeam(s32 arg0, u16 arg1) {
    HeroInfo *ha;
    DinoTeamView *w;
    DinoTeamView *q;
    DinoTeamView *p;
    HeroInfo *hb;
    HeroInfo *hc;
    HeroInfo *ta;
    HeroInfo *tc;
    s32 ia;
    s32 ib;
    s32 sa;
    s32 sb;
    s32 found;
    s32 done;
    s32 i;
    DinoTeamView *g;

    g = (DinoTeamView *)GameInfo;
    found = -1;
    done = 0;
    if (g->teams != 0) {
        hb = Heroes;
        ia = g->team0;
        ha = &hb[ia];
        ta = &hb[ha->hero_teammate];
        ib = g->team1;
        hc = &hb[ib];
        tc = &hb[hc->hero_teammate];
        sa = ha->hero_score + ta->hero_score;
        sb = hc->hero_score + tc->hero_score;
        if (sa <= 0) {
            i = done;
            if (g->out[ia] != 0) {
                goto out;
            }
            q = g;
            q->winner = ha->hero_name;
            q->unk30 |= 0x800;
            Clock_flags |= 0x200;
            for (;; i++) {
                s32 t_;

                if (i >= 4) {
                    break;
                }
                if (((u8 *)q)[i + 0x5C] != 0) {
                    Heroes[i].hero_status |= 0x8400;
                    Heroes[i].hero_shadow->sub->flags &= ~0x8000;
                    DINO_PUFF(&Heroes[i]);
                    RemoveCamera(Heroes[i].hero);
                } else {
                    Heroes[i].hero_status |= 0x200;
                }
            }
        } else if (sb <= 0) {
            i = 0;
            if (((u8 *)g)[ib + 0x5C] != 0) {
                goto out;
            }
            p = g;
            p->winner = hc->hero_name;
            p->unk30 |= 0x800;
            Clock_flags |= 0x200;
            for (;; i++) {
                s32 t_;

                if (i >= 4) {
                    break;
                }
                if (((u8 *)p)[i + 0x5C] != 0) {
                    hb[i].hero_status |= 0x8100;
                    DINO_PUFF(&hb[i]);
                    hb[i].hero_shadow->sub->flags &= ~0x8000;
                    RemoveCamera(hb[i].hero);
                } else {
                    hb[i].hero_status |= 0x200;
                }
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_score <= 0) {
                found = i;
            }
        }
        if (found != -1) {
            i = 0;
            w = (DinoTeamView *)GameInfo;
            if (w->out[found] != 0) {
                goto out;
            }
            p = w;
            p->winner = Heroes[found].hero_name;
            p->unk30 |= 0x800;
            Clock_flags |= 0x200;
            for (;; i++) {
                s32 t_;

                if (i >= 4) {
                    break;
                }
                if (((u8 *)p)[i + 0x5C] != 0) {
                    Heroes[i].hero_status |= 0x8400;
                    DINO_PUFF(&Heroes[i]);
                    Heroes[i].hero_shadow->sub->flags &= ~0x8000;
                    RemoveCamera(Heroes[i].hero);
                } else {
                    Heroes[i].hero_status |= 0x200;
                }
            }
        }
    }
    goto end;
out:
    done = 1;
end:
    if (done || arg0 == -1) {
        DinoTeamView *r;
        DinoTeamView *x;

        i = 0;
        r = (DinoTeamView *)GameInfo;
        x = r;
        x->winner = 0;
        x->unk30 |= arg1;
        Clock_flags |= 0x200;
        for (;; i++) {
            s32 t_;

            if (i >= 4) {
                break;
            }
            if (GameInfo[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x8400;
                DINO_PUFF(&Heroes[i]);
                Heroes[i].hero_shadow->sub->flags &= ~0x8000;
                RemoveCamera(Heroes[i].hero);
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void InitialiseDINOFirst(void) {
    void *v;
    void *f;
    void **p;
    u8 *g;
    HeroHooks.HeroBot = Dino_HeroBot;
    HeroHooks.HeroModels = DinoHeroModels;
    HeroHooks.InitHero = DinoInitHero;
    HeroHooks.PreMoveHero = DinoPreMoveHero;
    HeroHooks.MoveHero = DinoMoveHero;
    HeroHooks.HeroFacing = DinoHeroFacing;
    f = DinoChooseWinner;
    HeroHooks.ChooseWinner = f;
    g = GameInfo;
    if (*(u16 *)(g + 0x32) & 0x40) {
        HeroHooks.ChooseWinner = DinoChooseWinnerTeam;
    }
    HeroHooks.VictorySeqs = DinoVictorySeqs;
    HeroHooks.CreateShadow = ModelCreateShadow;
    HeroHooks.UpdateShadow = ModelUpdateShadow;
    p = &AiSkillTable;
    if (*(s32 *)(g + 0x6C) != 0) {
        v = DinoAiSkillAlt;
    } else {
        v = DinoAiSkill;
    }
    *p = v;
    AiBotTweakTable = DinoAiBotTweak;
    AiHumanTweakTable = DinoAiHumanTweak;
    AiHumanRoundTweakTable = DinoAiHumanRoundTweak;
    AiTeamSkillTable = DinoAiTeamSkill;
    DinoTargetD = 0;
    DinoTargetE = 0;
    DinoTargetObjs[0] = 0;
    DinoTargetObjs[1] = 0;
    DinoTargetObjs[2] = 0;
    DinoTargetObjsB[0] = 0;
    DinoTargetObjsB[1] = 0;
    DinoTargetObjsB[2] = 0;
    DinoTargetA = 0;
    DinoTargetB = 0;
    DinoTargetC = 0;
    DinoGoal = 0;
}

void DinoFindTargets(void) {
    DinoObj *o;

    for (o = ((DinoObj *)MapScene->subs); o != NULL; o = o->next) {
        if (((DinoGame *)GameInfo)->flags & 0x80) {
            if (o->index == 0x5002) {
                DinoGoal = o;
                printf("found target\n");
            }
            if (o->index == 0x5009) {
                DinoTargetA = o;
                printf("found target\n");
            }
            if (o->index == 0x500B) {
                DinoTargetB = o;
                printf("found target\n");
            }
            if (o->index == 0x500A) {
                DinoTargetC = o;
                printf("found target\n");
            }
            if (o->index == 0x500C) {
                DinoTargetObjsB[0] = o;
                printf("1\n");
            }
            if (o->index == 0x500D) {
                DinoTargetObjsB[1] = o;
                printf("2\n");
            }
            if (o->index == 0x500E) {
                DinoTargetObjsB[2] = o;
                printf("3\n");
            }
        } else if (o->index == 0x5002) {
            DinoGoal = o;
            printf("found target\n");
        }
    }
    DinoTargetObjs[0] = DinoTargetA;
    DinoTargetObjs[1] = DinoTargetB;
    DinoTargetObjs[2] = DinoTargetC;
}

void DinoUpdateTargets(void) {
    DinoObj *cur;
    DinoObj **p;
    DinoObj **q;
    s32 i;
    s32 j;

    if (!(GameLevelFlags & 0x80)) {
        return;
    }
    if (DinoTargetDelay == 0xF) {
        PlaySfx(0x6CF, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (DinoTargetDelay-- < 0) {
        DinoTargetDelay = 0xB4;
        DinoTargetFlag = 0x1E;
        DinoScoreTimer = DinoTargetTimer;
        DinoTargetTimer = Random(3);
        if (DinoTargetTimer == DinoScoreTimer) {
            DinoTargetTimer++;
        }
        if (DinoTargetTimer >= 3) {
            DinoTargetTimer = 0;
        }
        if (DinoTargetTimer == 0) {
            DinoTargetD = DinoTargetA;
        }
        if (DinoTargetTimer == 1) {
            DinoTargetD = DinoTargetB;
        }
        if (DinoTargetTimer == 2) {
            DinoTargetD = DinoTargetC;
        }
        if (DinoTargetTimer == 0) {
            DinoTargetE = DinoTargetObjsB[0];
        }
        if (DinoTargetTimer == 1) {
            DinoTargetE = DinoTargetObjsB[1];
        }
        if (DinoTargetTimer == 2) {
            DinoTargetE = DinoTargetObjsB[2];
        }
        DinoTargetStep = 0;
        return;
    }
    if (DinoTargetFlag-- < 0) {
        DinoScoreTimer = DinoTargetTimer;
    }
    if (DinoTargetD != NULL) {
        DinoTargetD->flags |= 0x40000000;
        DinoTargetD->scale = abs((rcossin_tbl[((DinoTargetStep & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 3);
        if (DinoTargetTimer == 2) {
            DinoTargetD->r = 0xFFF;
            DinoTargetD->g = 0;
            DinoTargetD->b = 0;
        }
        if (DinoTargetTimer == 1) {
            DinoTargetD->r = 0xFFF;
            DinoTargetD->g = 0xFFF;
            DinoTargetD->b = 0;
        }
        if (DinoTargetTimer == 0) {
            DinoTargetD->r = 0;
            DinoTargetD->g = 0xFFF;
            DinoTargetD->b = 0;
        }
        DinoTargetStep += 0x30;
        for (i = 0, p = DinoTargetObjs; i < 3; i++, p++) {
            if (DinoTargetD != *p) {
                (*p)->scale -= (*p)->scale >> 1;
            }
        }
    }
    if (DinoTargetE != NULL) {
        DinoTargetE->flags |= 0x40000000;
        DinoTargetE->scale = abs(rcossin_tbl[((DinoTargetStep & 0xFFF) - 0x400) & 0xFFF].unk2);
        cur = DinoTargetE;
        cur->r = 0;
        cur->g = 0;
        cur->b = 0;
        for (j = 0, q = DinoTargetObjsB; j < 3; j++, q++) {
            if (cur != *q) {
                (*q)->scale -= (*q)->scale >> 1;
            }
        }
    }
}

void InitialiseDINO(void) {
    NAS *nas;
    NAS *o;
    DinoChase *c;
    DinoHeroExt *x;
    DinoExt *e;
    EXTheromove *mv;
    s32 i;
    s32 a;
    u16 ang;
    s32 sc;
    s16 acc;
    u8 *g;

    Init_Clock(0x140, 0x28, 0);
    InitSharedScoreIcons();
    g = GameInfo;
    if (*(s32 *)(g + 0x6C) != 0) {
        PanelCtrl |= 0x2000;
    }
    if (*(s32 *)(g + 0x6C) == 0) {
        sc = *(u16 *)(g + 0x32) & 0x40;
        do {
            a = 0x100;
        } while (0);
    } else {
        sc = *(u16 *)(g + 0x32) & 0x40;
        do {
            a = 0x45;
        } while (0);
    }
    if (!sc) {
        a = 0x130;
    }
    Set_Clock(a, 0x81C0);
    World_floor_level = -0x10;
    DinoFindTargets();
    DinoScoreFlag = 0;
    DinoGemTimer = 0;
    DinoTargetTimer = 0;
    DinoTargetDelay = 0;
    DinoTargetD = NULL;
    DinoScoreTimer = 0;
    DinoTargetFlag = 0;
    DinoScoreDelay = 0;
    o = CreateNAS(0, 0, 0, 0x1902, 0x11);
    if (o != NULL) {
        o->UserInfo.user1 = 0;
    }
    o = CreateNAS(0, 0, 0, 0x1902, 0x11);
    if (o != NULL) {
        o->UserInfo.user1 = 1;
    }
    o = CreateNAS(0, 0, 0, 0x1902, 0x11);
    if (o != NULL) {
        o->UserInfo.user1 = 2;
    }
    o = CreateNAS(0, 0, 0, 0x1902, 0x11);
    if (o != NULL) {
        o->UserInfo.user1 = 3;
    }
    i = 0;
    for (;;) {
        HeroInfo *h;

        if (i >= 4) {
            break;
        }
        h = &Heroes[i];
        c = AddExt(h->hero, 0x44, 0x20000000);
        if (c != NULL) {
            c->unk28 = -1;
            c->fn = DinoUnusedStub;
            c->unk30 = 8;
            c->state = 0;
        }
        x = AddExt(h->hero, 0x20, 0x10000000);
        if (x != NULL) {
            acc = 6;
            mv = h->hero_move;
            mv->x_acc = acc;
            mv->z_acc = acc;
            mv->z_dec = acc;
            mv->x_dec = acc;
            mv->mass = 0x100;
            ang = ((get_angle(h->hero_eng->unk10.x, h->hero_eng->unk10.z) + 0x800) & 0xFFF) + 0x800;
            mv->forward_angle = ang;
            h->hero_eng->unk1E = ang;
            h->hero_eng->unk5E = 1;
            h->hero_shadow->unk1E = h->hero_eng->unk1E;
            x->unkC = 0x1E;
            x->timer = 0;
            h->hero_score = 0;
            if (((DinoGame *)GameInfo)->flags & 0x40) {
                sc = GameInfo[i + 0x5C];
                if (sc != 0) {
                    sc = 0xF;
                } else {
                    sc = 0x14;
                }
                h->hero_score = sc;
            }
            h->hero_score_displayed = h->hero_score;
            SetHeroAction(i, 0x63);
        }
        i++;
    }
    nas = CreateNAS(0x400, -0x40, 0, 0x1901, 0x11);
    DinoGem0 = (s32)nas;
    e = FindExtension(nas, 0x10000000);
    if (e != NULL) {
        e->unkC = 7;
    }
    nas = CreateNAS(-0x400, -0x40, 0, 0x1901, 0x11);
    DinoGem1 = nas;
    e = FindExtension(nas, 0x10000000);
    if (e != NULL) {
        e->unkC = 7;
    }
    DinoGem1->Engine->sub->entity = 0x2002;
    if (((DinoGame *)GameInfo)->flags & 0x80) {
        DinoGem1->Engine->sub->entity = 0x2002;
    }
    o = CreateNAS(0, -0x40, 0x200, 0x1901, 0x11);
    DinoGem2 = o;
    o->Engine->sub->entity = 0x2003;
    if (((DinoGame *)GameInfo)->flags & 0x80) {
        o->Engine->sub->entity = 0x2003;
    }
    DinoGems[0] = (NAS *)DinoGem0;
    DinoGems[1] = DinoGem1;
    DinoGems[2] = DinoGem2;
}

void DinoScoreGems(void) {
    VECTOR n;
    VECTOR d;
    VECTOR r;
    NAS *nas;
    DinoGem *ext;
    s32 i;
    s32 kind;
    s32 sign;
    s32 spd;
    s32 a;
    s32 out;
    s32 dot;

    if (DinoGoal == NULL || (GameExitFlags & 0x801)) {
        return;
    }
    if (DinoScoreDelay-- < 0) {
        DinoScoreDelay = 0;
    }
    ((DinoTarget *)DinoGoal)->unk51 = 0;
    ((DinoTarget *)DinoGoal)->x = (rcossin_tbl[(DinoScoreB - 0x400) & 0xFFF].unk2 * 11) >> 4;
    ((DinoTarget *)DinoGoal)->z = (rcossin_tbl[DinoScoreB & 0xFFF].unk2 * 11) >> 4;
    {
        s32 h = (u16)DinoScoreB;

        ((DinoTarget *)DinoGoal)->ang = h;
        ((DinoTarget *)DinoGoal)->ang2 = h * 2;
    }
    DinoScoreB = ((rcossin_tbl[(DinoScoreFlag - 0x400) & 0xFFF].unk2 * 3) >> 4) & 0xFFF;
    if (DinoScoreB >= 0x2F8 && DinoScoreDelay == 0 && DinoScoreB <= 0x400) {
        PlaySfx(0x6BF, NULL, NULL, 0x1000, 0x1E00, 0);
        DinoScoreDelay = 0x5A;
    }
    if (DinoScoreB <= 0xD08 && DinoScoreDelay == 0 && DinoScoreB >= 0x400) {
        PlaySfx(0x6BF, NULL, NULL, 0x1000, 0x1E00, 0);
        DinoScoreDelay = 0x5A;
    }
    DinoScoreFlag = (DinoScoreFlag + 0xC) & 0xFFF;
    ((DinoTarget *)DinoGoal)->unk52 = 1;
    for (i = 0;; i++) {
        s32 t_;

        if (i >= 3) {
            break;
        }
        nas = NULL;
        if (i == 0) {
            nas = (NAS *)DinoGem0;
        }
        if (i == 1) {
            nas = DinoGem1;
        }
        if (i == 2) {
            nas = DinoGem2;
        }
        ext = FindExtension(nas, 0x10000000);
        if (ext == NULL) {
            continue;
        }
        kind = ext->unkC;
        if (kind != 3 || ext->unk80 >= 2 || ext->unk8C > 0) {
            continue;
        }
        if (XZBoxToSphere((SLONG_XYZ *)DinoGoal, &nas->Engine->unk10, 0, &nas->Engine->unk10) == -1) {
            continue;
        }
        DinoScoreA++;
        spd = csqrt(ext->unkB4 * ext->unkB4 + ext->unkD4 * ext->unkD4) >> 6;
        a = (((DinoTarget *)DinoGoal)->ang + 0x800) & 0xFFF;
        ext->unk8C = 0x14;
        n.vx = rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
        n.vz = rcossin_tbl[a].unk2;
        d.vx = -rcossin_tbl[(nas->Engine->unk1E - 0x400) & 0xFFF].unk2;
        d.vz = -rcossin_tbl[nas->Engine->unk1E & 0xFFF].unk2;
        dot = DinoDot(&d, &n);
        r.vx = d.vx - (dot * n.vx >> 11);
        r.vz = d.vz - (dot * n.vz >> 11);
        out = get_angle(r.vx, r.vz) + 0x800;
        nas->Engine->unk1E = out;
        ext->unkB4 = -(spd * rcossin_tbl[(out - 0x400) & 0xFFF].unk2 >> 12);
        ext->unkD4 = -(spd * rcossin_tbl[out & 0xFFF].unk2 >> 12);
        nas->Engine->unk10.x += ext->unkB4;
        nas->Engine->unk10.z += ext->unkD4;
        sign = 1;
        if (((DinoGame *)GameInfo)->flags & 0x40) {
            sign = -1;
        }
        if (((DinoGame *)GameInfo)->flags & 0x80) {
            if (GameInfo[ext->unk10 + 0x5C] == 0) {
                PlaySfx(0x6B1, NULL, NULL, 0x1000, 0x1E00, 0);
                if (ext->unk90 == 5 && (DinoTargetTimer == 2 || DinoScoreTimer == 2)) {
                    PlaySfx(0x6BE, NULL, NULL, 0x1000, 0x1E00, 0);
                    Heroes[ext->unk10].hero_score += ext->unk90 * sign;
                    if (Heroes[ext->unk10].hero_score <= 0) {
                        Heroes[ext->unk10].hero_score = 0;
                    }
                    Heroes[ext->unk10].hero_score_displayed = Heroes[ext->unk10].hero_score;
                }
                if (ext->unk90 == 3 && (DinoTargetTimer == 1 || DinoScoreTimer == 1)) {
                    PlaySfx(0x6BD, NULL, NULL, 0x1000, 0x1E00, 0);
                    Heroes[ext->unk10].hero_score += ext->unk90 * sign;
                    if (Heroes[ext->unk10].hero_score <= 0) {
                        Heroes[ext->unk10].hero_score = 0;
                    }
                    Heroes[ext->unk10].hero_score_displayed = Heroes[ext->unk10].hero_score;
                }
                if (ext->unk90 == 2 && (DinoTargetTimer == 0 || DinoScoreTimer == 0)) {
                    PlaySfx(0x6BC, NULL, NULL, 0x1000, 0x1E00, 0);
                    Heroes[ext->unk10].hero_score += ext->unk90 * sign;
                    if (Heroes[ext->unk10].hero_score <= 0) {
                        Heroes[ext->unk10].hero_score = 0;
                    }
                    Heroes[ext->unk10].hero_score_displayed = Heroes[ext->unk10].hero_score;
                }
            } else {
                PlaySfx(0x6B1, NULL, NULL, 0x1000, 0x1E00, 0);
                Heroes[ext->unk10].hero_score += ext->unk90 * sign;
                if (Heroes[ext->unk10].hero_score <= 0) {
                    Heroes[ext->unk10].hero_score = 0;
                }
                Heroes[ext->unk10].hero_score_displayed = Heroes[ext->unk10].hero_score;
                if (ext->unk90 == 1) {
                    PlaySfx(0x6BB, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (ext->unk90 == 2) {
                    PlaySfx(0x6BC, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (ext->unk90 == 3) {
                    PlaySfx(0x6BD, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (ext->unk90 == 5) {
                    PlaySfx(0x6BE, NULL, NULL, 0x1000, 0x1E00, 0);
                    PlaySfx(0x6B2, NULL, NULL, 0x1000, 0x1E00, 0);
                }
            }
        } else {
            PlaySfx(0x6B1, NULL, NULL, 0x1000, 0x1E00, 0);
            Heroes[ext->unk10].hero_score += ext->unk90 * sign;
            if (Heroes[ext->unk10].hero_score < 0) {
                Heroes[ext->unk10].hero_score = 0;
            }
            if (((DinoTeamView *)GameInfo)->teams != 0 && (((DinoGame *)GameInfo)->flags & 0x40) &&
                Heroes[ext->unk10].hero_score == 0) {
                Heroes[Heroes[ext->unk10].hero_teammate].hero_score += ext->unk90 * sign;
                if (Heroes[Heroes[ext->unk10].hero_teammate].hero_score < 0) {
                    Heroes[Heroes[ext->unk10].hero_teammate].hero_score = 0;
                }
            }
            Heroes[ext->unk10].hero_score_displayed = Heroes[ext->unk10].hero_score;
            if (ext->unk90 == 1) {
                PlaySfx(0x6BB, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (ext->unk90 == 2) {
                PlaySfx(0x6BC, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (ext->unk90 == 3) {
                PlaySfx(0x6BD, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (ext->unk90 == 5) {
                PlaySfx(0x6BE, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x6B2, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
}

void UpdateDINO(void) {
    s32 i;
    DinoExt *ext;
    HeroInfo *hero;

    DinoScoreGems();
    i = 0;
    DinoUpdateTargets();
    hero = Heroes;
loop:
    if (i < 4) {
        ext = FindExtension(hero->hero, 0x10000000);
        if (ext != NULL && ext->unk10 > 0) {
            ext->unk10--;
        }
        hero++;
        i++;
        goto loop;
    }
    if (GamePause == 0) {
        DinoGemTimer++;
    }
    if (DinoGemTimer == 0x1C2) {
        FindExtension(DinoGem1, 0x10000000);
    }
    if (DinoGemTimer == 0x546) {
        ext = FindExtension(DinoGem2, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 7;
        }
    }
}

s32 DinoKeepInArena(s32 arg0) {
    s32 dist;
    s32 ang;
    s32 d;
    s32 r;

    dist = csqrt(Heroes[arg0].hero_eng->unk10.x * Heroes[arg0].hero_eng->unk10.x +
                 Heroes[arg0].hero_eng->unk10.z * Heroes[arg0].hero_eng->unk10.z) >> 6;
    if (dist < 0xB01) {
        return 0;
    }
    ang = get_angle(Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z) & 0xFFF;
    d = dist - 0xAFE;
    Heroes[arg0].hero_eng->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
    Heroes[arg0].hero_eng->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
    if (Heroes[arg0].hero_move->x_vel * Heroes[arg0].hero_move->x_vel +
            Heroes[arg0].hero_move->z_vel * Heroes[arg0].hero_move->z_vel > 0x1000 &&
        Heroes[arg0].hero_mode == 0x66) {
        r = Random(4);
        if (r == 0) {
            PlaySfx(0x6D0, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x6D1, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x6D2, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        do {
            if (r == 3) {
                PlaySfx(0x6D3, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        } while (0);
    }
    return ang;
}

void PlotDinoSmoke(NAS *m) {
    s32 w = m->UserInfo.user1;
    EXTengine **u;
    EXTengine *e;
    s32 x;
    s32 z;

    if (w != 0) {
        u = (EXTengine **)w;
        if (GameMode != 0x1C) {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) >> 4;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) >> 4;
            x = e->unk10.z;
        } else {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) / 6;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) / 6;
            x = e->unk10.z;
        }
        x += z;
        e->unk10.z = x;
        m->Engine->unk10.y = u[1]->unk10.y - ((u[1]->unk2C.x * 0x1A4) >> 12);
        m->Engine->unk2C.x = u[1]->unk2C.x;
        m->Engine->unk2C.y = u[1]->unk2C.y;
        m->Engine->unk2C.z = u[1]->unk2C.z;
    }
    RunNasAnim(m);
}

s32 XZBoxToSphere(SLONG_XYZ *a, SLONG_XYZ *b, s32 c, SLONG_XYZ *e) {
    VECTOR d;
    VECTOR n;
    MATRIX m;
    SVECTOR ang;
    SVECTOR sv[2];
    DinoBoxHalf lo;
    DinoBoxHalf hi;
    DinoBox2 *src;
    SVECTOR *p;
    s32 dist;
    s32 side;
    s32 *ap;

    ap = (s32 *)a;
    dist = 0x7FFFFFF;
    side = 0;
    src = (DinoBox2 *)SubFindFlagPoint((subengine *)a, side, side);
    p = sv;
    if (src == NULL) {
        printf("No Coll box on subengine in XZBoxToSphere\n");
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
    ang.vx = -((u16 *)a)[8];
    ang.vy = -((u16 *)a)[9];
    ang.vz = 0;
    RotMatrixZYX(&ang, &m);
    ApplyMatrixLV(&m, &d, &d);
    if (d.vx < lo.v[0]) {
        side = 2;
    }
    if (d.vy < hi.v[1]) {
        side |= 0x10;
    }
    if (d.vz < lo.v[2]) {
        side |= 4;
    }
    if (hi.v[0] < d.vx) {
        side |= 1;
    }
    if (hi.v[2] < d.vz) {
        side |= 8;
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
            push = DinoBoxPushRot(&ang, (u16 *)a, abs(push));
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

s32 DinoTrackCollide(DinoVec *pos, DinoVec *out, s32 radius, s32 idx, s32 flags) {
    DinoVec p;
    DinoVec hit;
    DinoVec d;
    DinoVec e;
    DinoSegObj *obj;
    DinoSegObj *w;
    u8 *data;
    s32 *ent;
    s32 best;
    s32 found;
    s32 shift;
    s32 i;
    s32 dist;
    s32 t;
    s32 ang;
    s32 r2;
    s32 len;
    s32 bad;
    s32 a;
    s32 dx;
    s32 dz;
    s32 s;
    s32 v;
    s32 n;
    s32 rad;

    best = 0x7FFFFFFF;
    found = 0;
    rad = radius;
    shift = 12;
    bad = idx >= (((DinoScene *)MapScene)->unk0 ?
                  *(s32 *)(*(s32 *)(((DinoScene *)MapScene)->data + 0x14) +
                           (s32)((DinoScene *)MapScene)->data + 0x14) :
                  0);
    if (bad) {
        return 0;
    }
    data = ((DinoScene *)MapScene)->data;
    obj = (DinoSegObj *)((u8 *)&((s32 *)(data + (*(s32 *)(data + 0x14) + 0x14)))[idx + 1] +
                          ((s32 *)(data + (*(s32 *)(data + 0x14) + 0x14)))[idx + 1]);
    if (obj != NULL) {
        i = 0;
        dist = 0;
        t = 0;
        w = obj;
        n = obj->count;
        r2 = rad * rad;
        for (;; i++, w = (DinoSegObj *)((u8 *)w + sizeof(DinoSeg))) {
            s32 lt;

            if (i >= n) {
                break;
            }
            ang = obj->segs[i].ang;
            if (flags & 1) {
                ang += 0x800;
            }
            d.vx = pos->vx - w->segs[0].p0.vx;
            d.vz = pos->vz - w->segs[0].p0.vz;
            if (((d.vx * rcossin_tbl[-ang & 0xFFF].unk2) >> 12) -
                    ((d.vz * rcossin_tbl[(-ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                continue;
            }
            d.vy = w->segs[0].p1.vy - w->segs[0].p0.vy;
            e.vx = w->segs[0].p1.vx - w->segs[0].p0.vx;
            e.vy = w->segs[0].p1.vy - w->segs[0].p0.vy;
            e.vz = w->segs[0].p1.vz - w->segs[0].p0.vz;
            len = (w->segs[0].len * w->segs[0].len) >> shift;
            if (len != 0) {
                t = (e.vx * d.vx + e.vy * d.vy + e.vz * d.vz) / len;
            }
            lt = t < 0x1000;
            if (t <= 0) {
                p = w->segs[0].p0;
                dx = p.vx - pos->vx;
                dz = p.vz - pos->vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (!lt) {
                p = w->segs[0].p1;
                dx = p.vx - pos->vx;
                dz = p.vz - pos->vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (t > 0 && lt) {
                p.vx = ((t * e.vx) >> shift) + w->segs[0].p0.vx;
                p.vy = ((t * e.vy) >> shift) + w->segs[0].p0.vy;
                p.vz = ((t * e.vz) >> shift) + w->segs[0].p0.vz;
                dx = p.vx - pos->vx;
                dz = p.vz - pos->vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (dist < 0) {
                dist = 0x7FFFFFFF;
            }
            if (dist > best) {
                continue;
            }
            best = dist;
            hit.vx = p.vx;
            hit.vy = p.vy;
            hit.vz = p.vz;
            if (best > r2) {
                continue;
            }
            s = csqrt(best) >> 6;
            out->vy = ang;
            a = -ang;
            v = rad - s + 2;
            out->vx = (v * rcossin_tbl[a & 0xFFF].unk2) >> 12;
            ang = a + 0x400;
            out->vz = (v * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
            pos->vx += out->vx;
            pos->vz += out->vz;
            found = 1;
            hit.vy = obj->segs[i].ang;
        }
    }
    return found;
}

void DinoMovesSwap(s32 arg0, s32 arg1) {
    EXTheromove *a;
    EXTheromove *b;
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;
    s32 av;
    s32 bv;

    a = Heroes[arg0].hero_move;
    b = Heroes[arg1].hero_move;
    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        av = a->x_vel;
        bv = b->x_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        av = a->z_vel;
        bv = b->z_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

void DinoHeroFacing(s32 arg0) {
    if (Heroes[arg0].hero_frozen == 0) {
        if (!(Heroes[arg0].hero_ctrl & 1)) {
            Heroes[arg0].hero_eng->unk1E += angle_diff(Heroes[arg0].hero_eng->unk1E, Heroes[arg0].hero_move->forward_angle) >> 2;
        }
        Heroes[arg0].hero_ctrl &= 0xFFFE;
    }
    Heroes[arg0].hero_eng->unk5E = 1;
}

void DinoMoveHero(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    ((void (*)(s32))HeroHooks.HeroFacing)(arg0);
    if (m->x_vel != m->x_max) {
        if (m->x_max != 0) {
            step = m->x_acc;
        } else {
            step = m->x_dec;
        }
        if (m->x_vel < m->x_max) {
            m->x_vel += step;
            if (m->x_vel > m->x_max) {
                m->x_vel = m->x_max;
            }
        } else {
            m->x_vel -= step;
            if (m->x_vel < m->x_max) {
                m->x_vel = m->x_max;
            }
        }
    }
    if (m->z_vel != m->z_max) {
        if (m->z_max != 0) {
            step = m->z_acc;
        } else {
            step = m->z_dec;
        }
        if (m->z_vel < m->z_max) {
            m->z_vel += step;
            if (m->z_vel > m->z_max) {
                m->z_vel = m->z_max;
            }
        } else {
            m->z_vel -= step;
            if (m->z_vel < m->z_max) {
                m->z_vel = m->z_max;
            }
        }
    }
    e->unk10.x += m->x_vel;
    e->unk10.z += m->z_vel;
    if (DinoKeepInArena(arg0) != 0 && Heroes[arg0].hero_mode == 0x66) {
        m->z_vel = 0;
        m->x_vel = 0;
        m->z_max = 0;
        m->x_max = 0;
    }
}

void DinoPreMoveHero(s32 id) {
    HeroInfo *h;
    DinoExt *ext;
    EXTheromove *m;
    EXTheromove *m2;
    s32 sc;

    m = NULL;
    ext = FindExtension(Heroes[id].hero, 0x10000000);
    h = &Heroes[id];
    if (ext != NULL) {
        if (h->hero_frozen != 0) {
            ((PadButtons *)Pads)[id].unk50 = 0;
            ((PadButtons *)Pads)[id].unk54 = 0;
        }
        m = h->hero_move;
        if (m != NULL) {
            m->x_max = 0;
            m->z_max = 0;
            if ((Pads[id].unk0 & 1) || h->hero_ai->AImode != 0) {
                if (h->hero_mode != 0x66 && (((PadButtons *)Pads)[id].unk50 & 0xF0)) {
                    if (ext->unk14 == 0) {
                        m->forward_angle = (((DinoPadAng *)Pads)[id].angle + 0x800) & 0xFFF;
                        sc = DinoSteerMin;
                        if (GameInfo[id + 0x5C] != 0) {
                            sc = DinoSteerTable[(s16)(u16)AiPlayerSkill[id].level >> 1];
                        }
                        m->x_max = (sc * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                        m->z_max = (sc * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
                    } else {
                        sc = DinoSteerMax;
                        m->forward_angle = (((DinoPadAng *)Pads)[id].angle + 0x800) & 0xFFF;
                        m->x_max = (sc * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                        m->z_max = (sc * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
                    }
                }
                if (Heroes[id].hero_mode == 0x65) {
                    m->x_max = 0;
                    m->z_max = 0;
                }
            }
        }
        if (Heroes[id].hero_status & 0x200) {
            m2 = Heroes[id].hero_move;
            if (m2 != NULL) {
                m2->z_dir = 0;
                m2->x_max = 0;
                m2->z_max = 0;
                m2->x_vel = 0;
                m2->z_vel = 0;
            }
        }
    }
    if (abs(angle_diff(m->forward_angle, m->facing_angle)) > 0x110) {
        m->facing_angle = (m->forward_angle + 0x100) & 0xE00;
    }
}

void DinoHeroRun(s32 id) {
    DinoHeroExt *ext;

    ext = FindExtension(Heroes[id].hero, 0x10000000);
    if (ext != NULL) {
        if (++ext->timer == 6) {
            if (id == 0) {
                PlaySfx(0x6A4, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 1) {
                PlaySfx(0x6A6, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 2) {
                PlaySfx(0x6A8, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 3) {
                PlaySfx(0x6AA, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (ext->timer == 0x13) {
            if (id == 0) {
                PlaySfx(0x6A5, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 1) {
                PlaySfx(0x6A7, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 2) {
                PlaySfx(0x6A9, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 3) {
                PlaySfx(0x6AB, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (ext->timer >= 0x1A) {
            ext->timer = 0;
        }
        DinoHeroCollide(id);
        DinoHeroTestIdle(id);
        DinoHeroTestShoot(id);
        ProcessHeroAnim(id);
        if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
            SetHeroAction(id, 0x68);
        }
    }
}

void SetDinoHeroDazed(s32 id) {
    DinoExtB *ext;
    EXTheromove *mv;
    s32 r;

    ext = FindExtension(Heroes[id].hero, 0x10000000);
    if (ext != NULL) {
        mv = Heroes[id].hero_move;
        mv->x_max = 0;
        mv->z_max = 0;
        mv->x_acc = 9;
        mv->z_acc = 9;
        mv->z_dec = 9;
        mv->x_dec = 9;
        SetHeroSeq(id, 0x4D);
        ext->unkC = 7;
        ext->unk14 = 0;
        pad_rumble_small(id, 0xA);
        ((void (*)(s32, s32, s32))pad_rumble_big)(id, 0xA, 0xFF);
        switch (Heroes[id].hero_name) {
        case 1:
            SFX4(3, 4, 5, 6);
            break;
        case 2:
            SFX4(0x35, 0x36, 0x37, 0x38);
            break;
        case 3:
            SFX4(0x67, 0x68, 0x69, 0x6A);
            break;
        case 4:
            SFX4(0x99, 0x9A, 0x9B, 0x9C);
            break;
        case 5:
            SFX4(0x12F, 0x130, 0x131, 0x132);
            break;
        case 7:
            SFX4(0xFD, 0xFE, 0xFF, 0x100);
            break;
        case 8:
            SFX4(0xCB, 0xCC, 0xCD, 0xCE);
            break;
        case 6:
            SFX4(0x161, 0x162, 0x163, 0x164);
            break;
        }
    }
}

void DinoHeroDazed(s32 id) {
    DinoExt *ext;

    ext = FindExtension(Heroes[id].hero, 0x10000000);
    if (ext != NULL) {
        Heroes[id].hero_eng->unk1E += 0x80;
        DinoHeroCollide(id);
        Heroes[id].hero_frozen = 2;
        ((PadButtons *)Pads)[id].unk50 = 0;
        ((PadButtons *)Pads)[id].unk54 = 0;
        if (ext->unkC-- < 0) {
            SetHeroAction(id, 0x63);
        }
        ProcessHeroAnim(id);
        if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
            SetHeroAction(id, 0x68);
        }
    }
}

void SetDinoHeroSlide(s32 id) {
    EXTheromove *m;
    s32 v;

    m = Heroes[id].hero_move;
    if (id == 0) {
        PlaySfx(0x6C4, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 1) {
        PlaySfx(0x6C5, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 2) {
        PlaySfx(0x6C6, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 3) {
        PlaySfx(0x6C7, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    m->x_acc = 0xA;
    m->z_acc = 0xA;
    m->z_dec = 6;
    m->x_dec = 6;
    m->forward_angle = Heroes[id].hero_eng->unk1E;
    v = (rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2 * 0x59) >> 0xB;
    m->x_vel = v;
    m->x_max = v;
    v = (rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2 * 0x59) >> 0xB;
    m->z_vel = v;
    m->z_max = v;
    SetHeroSeq(id, 0x50);
}

void DinoHeroSlide(s32 id) {
    NAS *nas;

    if (Heroes[id].hero_anim->Aindex == 2) {
        nas = CreateNAS(Heroes[id].hero_eng->unk10.x, Heroes[id].hero_eng->unk10.y, Heroes[id].hero_eng->unk10.z, 0x1903, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = (s32)&Heroes[id];
        }
    }
    DinoHeroCollide(id);
    DinoHeroTestStop(id);
    ProcessHeroAnim(id);
    if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
        SetHeroAction(id, 0x68);
    }
}

s32 DinoHeroTestShoot(s32 arg0) {
    DinoExtB *a;
    DinoExtB *b;

    a = FindExtension(Heroes[arg0].hero, 0x10000000);
    if (a != NULL) {
        if (a->unk14 != 0) {
            b = FindExtension(a->unk18, 0x10000000);
            if (b != NULL && b->unk10 == arg0) {
                if (((DinoPad *)Pads)[arg0].unk50 & 0x8000) {
                    SetHeroAction(arg0, 0x65);
                    b->unkC = 2;
                    a->unk14 = 0;
                    return 1;
                }
            }
            return 0;
        }
        if (a->unk10 <= 0) {
            if (((DinoPad *)Pads)[arg0].unk50 & 0x8000) {
                a->unk10 = 0x1E;
                SetHeroAction(arg0, 0x66);
                return 1;
            }
        }
    }
    return 0;
}

s32 DinoHeroCollide(s32 hero) {
    s32 dx;
    s32 dz;
    HeroInfo *me;
    HeroInfo *hb;
    HeroInfo *o;
    EXTengine *eng;
    EXTengine *oe;
    DinoExtB *mx;
    DinoExtB *ox;
    DinoExtB *x;
    DinoExtB *x1;
    u32 i;
    s32 dist;
    s32 ang;
    s32 r;
    s32 d;
    s32 hit;
    s32 lim;

    lim = 0x19C;
    hb = Heroes;
    hit = 0;
    eng = hb[hero].hero_eng;
    i = 0;
    if (eng != NULL) {
        do {
            me = &hb[hero];
        } while (0);
        o = hb;
    loop:
        if (i < 4) {
            if (hero != i && !(o->hero_status & 0x2000)) {
                oe = o->hero_eng;
                if (oe != NULL) {
                    do {
                        dx = oe->unk10.x - eng->unk10.x;
                        dz = oe->unk10.z - eng->unk10.z;
                        dist = csqrt(dx * dx + dz * dz) >> 6;
                    } while (0);
                    if (dist < 0x19C) {
                        if (me->hero_move->x_vel * me->hero_move->x_vel +
                                me->hero_move->z_vel * me->hero_move->z_vel > 0x1000) {
                            r = Random(4);
                            if (r == 0) {
                                PlaySfx(0x6D0, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (r == 1) {
                                PlaySfx(0x6D1, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (r == 2) {
                                PlaySfx(0x6D2, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (r == 3) {
                                PlaySfx(0x6D3, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                        }
                        Random(4);
                        hit = 1;
                        ang = get_angle(dx, dz) & 0xFFF;
                        dist = lim - dist;
                        DinoMovesSwap(hero, i);
                        do {
                            mx = FindExtension(me->hero, 0x10000000);
                            ox = FindExtension(o->hero, 0x10000000);
                        } while (0);
                        if (me->hero_mode != 0x67 && o->hero_mode == 0x66) {
                            if (mx->unk14 != 0) {
                                x1 = FindExtension(mx->unk18, 0x10000000);
                                if (x1 != NULL && x1->unkC == 1) {
                                    x1->unkC = 5;
                                }
                            }
                            SetHeroAction(hero, 0x67);
                            do {
                                SetHeroAction(i, 0x63);
                            } while (0);
                        }
                        if (o->hero_mode != 0x67 && me->hero_mode == 0x66) {
                            if (ox->unk14 != 0) {
                                x = FindExtension(ox->unk18, 0x10000000);
                                if (x != NULL && x->unkC == 1) {
                                    x->unkC = 5;
                                }
                            }
                            SetHeroAction(i, 0x67);
                            SetHeroAction(hero, 0x63);
                        }
                        d = dist + 2;
                        eng->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                        eng->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                    }
                }
            }
            do {
                o++;
                i++;
            } while (0);
            goto loop;
        }
    }
    return hit;
}

s32 DinoGemCarrier(NAS *m) {
    DinoExtB *me;
    DinoExtB *he;
    s32 i;
    s32 dx;
    s32 dz;

    me = FindExtension(m, 0x10000000);
    if (me == NULL) {
        return 0;
    }
    i = 0;
    for (;;) {
        if (i >= 4) {
            goto done;
        }
        he =FindExtension(Heroes[i].hero, 0x10000000);
        if (he != NULL && he->unk14 == 0 && Heroes[i].hero_mode != 0x65 && me->unkC != 1
            && abs(m->Engine->unk10.y - Heroes[i].hero_eng->unk10.y) < 0x201
            && (me->unkC == 0 || me->unkC == 3 || me->unkC == 4)) {
            dx = Heroes[i].hero_eng->unk10.x - m->Engine->unk10.x;
            dz = Heroes[i].hero_eng->unk10.z - m->Engine->unk10.z;
            if (dx * dx + dz * dz <= 0x3FFFF) {
                me->unk10 = i;
                he = FindExtension(Heroes[i].hero, 0x10000000);
                if (he != NULL) {
                    he->unk14 = 1;
                    he->unk18 = m;
                }
                return 1;
            }
        }
        i++;
    }
done:
    return 0;
}

void DinoGemPhysics(NAS *arg0) {
    DinoExt *e;
    DinoExt *o;
    EXTengine *a;
    EXTengine *b;
    DinoPhys *pa;
    DinoPhys *pb;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 ma;
    s32 mb;
    s32 sum;
    s32 diff;
    s32 q;

    e = FindExtension(arg0, 0x10000000);
    i = 0;
    if (e != NULL) {
        for (;;) {
            s32 r;

            if (i >= 3) {
                break;
            }
            if (arg0 == DinoGems[i] || DinoGems[i] == NULL) {
                goto next;
            }
            o = FindExtension(DinoGems[i], 0x10000000);
            if (o == NULL || o->unkC == 6 || e->unkC == 6) {
                goto next;
            }
            a = arg0->Engine;
            b = DinoGems[i]->Engine;
            dx = a->unk10.x - b->unk10.x;
            dz = a->unk10.z - b->unk10.z;
            if (abs(a->unk10.y - b->unk10.y) >= 0x200) {
                goto next;
            }
            d = csqrt(dx * dx + dz * dz) >> 6;
            if (d >= 0x200) {
                goto next;
            }
            ang = get_angle(dx, dz) & 0xFFF;
            d = 0x200 - d;
            arg0->Engine->unk10.x += ((d + 8) * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
            arg0->Engine->unk10.z += ((d + 8) * rcossin_tbl[ang].unk2) >> 12;
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x6C0, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x6C1, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x6C2, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x6C3, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            pa = (DinoPhys *)((u8 *)e + 0x94);
            pb = (DinoPhys *)((u8 *)o + 0x94);
            ma = pa->unk4E;
            mb = pb->unk4E;
            if (ma > 0 && mb > 0) {
                diff = -((pa->unk20 - pb->unk20) << 8);
                q = (diff * mb + ((ma * pa->unk20 + mb * pb->unk20) << 8)) / (mb + ma);
                sum = q - diff;
                pa->unk20 = q >> 8;
                pb->unk20 = sum >> 8;
                diff = -((pa->unk40 - pb->unk40) << 8);
                q = (diff * mb + ((ma * pa->unk40 + mb * pb->unk40) << 8)) / (mb + ma);
                sum = q - diff;
                pa->unk40 = q >> 8;
                pb->unk40 = sum >> 8;
            }
        next:
            i++;
        }
    }
}

void InitDinoGem(NAS *m) {
    DinoGem *ext;
    EXTengine *e;

    ext = AddExt(m, 0xE8, 0x10000000);
    if (ext != NULL) {
        do {
            m->Engine->sub->entity = 0x2001;
        } while (0);
        if (GameLevelFlags & 0x80) {
            m->Engine->sub->entity = 0x2001;
        }
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_DragonObjects];
        ext->unkC = 6;
        ext->unk10 = 5;
        ext->unk80 = 0;
        ext->unk88 = 0;
        ext->unk8C = 0;
        ext->unk90 = 1;
        ext->unkE2 = 0x100;
        ext->unkB4 = 0;
        ext->unkD4 = 0;
        AddCamera(m);
        m->Engine->sub->flags &= 0xFFFF7FFF;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->unk84 = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_DragonObjects];
            e->sub->entity = 0x2014;
            e->unk5C = 1;
            e->unk2C.x = 0x23E8;
            e->unk2C.y = 0x23E8;
            e->unk2C.z = 0x23E8;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            ext->unk84->sub->flags &= 0xFFFF7FFF;
        }
    }
}

void PlotDinoGem(NAS *m) {
    DinoGem *ext;
    SLONG_XYZ pos;
    s32 n[4];
    s32 d[4];
    s32 rv[4];
    SLONG_XYZ *p;
    s32 i;
    s32 found;
    s32 r;
    s32 dist;
    s32 ang;
    s32 spd;
    s32 dot;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (GameExitFlags & 0x801) {
        KillNAS(m);
        return;
    }
    m->Engine->unk5E = 1;
    if (ext->unk8C > 0) {
        ext->unk8C--;
    }
    switch (ext->unkC) {
    case 7:
        ext->unkC = 0;
        m->Engine->sub->flags |= 0x8000;
        pos = m->Engine->unk10;
        pos.y += 0x100;
        {
            NAS *nas_;
            DinoPuff *p_;
            SLONG_XYZ *v_;

            v_ = &pos;
            nas_ = CreateNAS(v_->x, v_->y, v_->z, 0x1605, 0x11);
            if (nas_ != NULL) {
                p_ = FindExtension(nas_, 0x10000000);
                if (p_ != NULL) {
                    p_->unk18 = 0x100;
                    p_->unkC = 0;
                    p_->unk20 = 1;
                    p_->unk28 = 0;
                    nas_->InitDelete(nas_);
                }
            }
        }
        PlaySfx(0x6CE, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 0:
        m->Engine->unk1E += 0x20;
        ext->unk84->sub->flags |= 0x8000;
        DinoGemPhysics(m);
        m->Engine->unk10.x += ext->unkB4;
        m->Engine->unk10.z += ext->unkD4;
        if (ext->unkB4 < 0) {
            ext->unkB4 += 2;
            if (ext->unkB4 > 0) {
                ext->unkB4 = 0;
            }
        }
        if (ext->unkB4 > 0) {
            ext->unkB4 -= 2;
            if (ext->unkB4 < 0) {
                ext->unkB4 = 0;
            }
        }
        if (ext->unkD4 < 0) {
            ext->unkD4 += 2;
            if (ext->unkD4 > 0) {
                ext->unkD4 = 0;
            }
        }
        if (ext->unkD4 > 0) {
            ext->unkD4 -= 2;
            if (ext->unkD4 < 0) {
                ext->unkD4 = 0;
            }
        }
        goto check;
    case 1:
        ext->unk84->sub->flags &= 0xFFFF7FFF;
        m->Engine->unk1E += 0x40;
        m->Engine->unk10 = Heroes[ext->unk10].hero_eng->unk10;
        m->Engine->unk10.y -= 0x400;
        break;
    case 2: {
        HeroInfo *hb;
        s32 a;

        ext->unk68 = Heroes[ext->unk10].hero_eng->unk10;
        ext->unk68.y -= 0x200;
        m->Engine->unk10 = ext->unk68;
        a = Heroes[ext->unk10].hero_eng->unk1E;
        m->Engine->unk2C.x = 0x1000;
        m->Engine->unk2C.y = 0x1000;
        m->Engine->unk2C.z = 0x1000;
        ext->unk14 = 0x800;
        ext->unk28 = 0;
        ext->unk2C = 0;
        ext->unk48 = -(rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 0xD2) >> 12;
        ext->unk4C = -(rcossin_tbl[a & 0xFFF].unk2 * 0xD2) >> 12;
        ext->unk80 = 0;
        ext->unkB4 = ext->unk48;
        ext->unkD4 = ext->unk4C;
        ext->unk84->sub->flags |= 0x8000;
        AddCamera(m);
        found = -1;
        ext->unkC = 3;
        hb = &Heroes[ext->unk10];
        for (i = 3; i > 0; i--) {
            if (PointInZone((s32 *)&hb->hero_eng->unk10, i) != 0) {
                found = i;
            }
        }
        if (found == 1) {
            ext->unk90 = 5;
        }
        if (found == 2) {
            ext->unk90 = 3;
        }
        if (found == 3) {
            ext->unk90 = 2;
        }
        if (found == -1) {
            ext->unk90 = 1;
        }
        m->Engine->unk1E = Heroes[ext->unk10].hero_eng->unk1E;
        ext->unk88 = 4;
        break;
    }
    case 5: {
        s32 a;

        a = Random(0x1000);
        m->Engine->unk2C.x = 0x1000;
        m->Engine->unk2C.y = 0x1000;
        m->Engine->unk2C.z = 0x1000;
        ext->unk28 = -0x60;
        ext->unk14 = 8;
        ext->unk2C = 0;
        ext->unk48 = (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 0x40) >> 12;
        ext->unk4C = (rcossin_tbl[a & 0xFFF].unk2 * 0x40) >> 12;
        ext->unk80 = 0;
        ext->unkB4 = ext->unk48;
        ext->unkD4 = ext->unk4C;
        ext->unk84->sub->flags |= 0x8000;
        ext->unkC = 4;
        SFX4(0x6C0, 0x6C1, 0x6C2, 0x6C3);
        AddCamera(m);
        break;
    }
    case 4:
        m->Engine->unk10.x += ext->unkB4;
        m->Engine->unk10.z += ext->unkD4;
        m->Engine->unk10.y += ext->unk28;
        ext->unk28 += ext->unk14;
        if (ext->unk2C < m->Engine->unk10.y) {
            m->Engine->unk10.y -= abs(m->Engine->unk10.y - ext->unk2C);
            ext->unk28 = -ext->unk28 >> 1;
            ext->unkB4 >>= 1;
            ext->unkD4 >>= 1;
            ext->unk80++;
            SFX4(0x6B7, 0x6B8, 0x6B9, 0x6BA);
            if (ext->unk80 >= 5) {
                ext->unk80 = 0;
                ext->unkC = 0;
                ext->unkB4 = 0;
                ext->unkD4 = 0;
                ext->unk10 = 5;
                return;
            }
        }
        goto check;
    case 3:
        ext->unk84->sub->flags |= 0x8000;
        if (ext->unk88-- > 0
            && (GameInfo[ext->unk10 + 0x5C] != 0 || (((DinoPad *)Pads)[ext->unk10].unk50 & 0x8000))) {
            ext->unk28 -= 0x1800;
            if (ext->unk28 < -0x6000) {
                ext->unk28 = -0x6000;
            }
        }
        m->Engine->unk10.x += ext->unkB4;
        m->Engine->unk10.z += ext->unkD4;
        m->Engine->unk10.y += ext->unk28 >> 8;
        ext->unk28 += ext->unk14;
        if (ext->unk2C < m->Engine->unk10.y) {
            m->Engine->unk10.y -= abs(m->Engine->unk10.y - ext->unk2C);
            ext->unk28 = -ext->unk28 >> 1;
            ext->unkB4 >>= 1;
            ext->unkD4 >>= 1;
            ext->unk80++;
            SFX4(0x6B7, 0x6B8, 0x6B9, 0x6BA);
            if (ext->unk80 >= 5) {
                ext->unk80 = 0;
                RemoveCamera(m);
                ext->unkC = 0;
                ext->unkB4 = 0;
                ext->unkD4 = 0;
                ext->unk10 = 5;
                return;
            }
        }
    check:
        if (DinoGemCarrier(m) != 0) {
            ext->unkC = 1;
            RemoveCamera(m);
            PlaySfx(0x6CD, NULL, NULL, 0x1000, 0x1E00, 0);
            m->Engine->unk2C.x = 0x800;
            m->Engine->unk2C.y = 0x800;
            m->Engine->unk2C.z = 0x800;
            return;
        }
        break;
    }
    if (ext->unkC != 1) {
        EXTengine *e;
        s32 lim;

        e = m->Engine;
        p = &e->unk10;
        dist = csqrt(e->unk10.x * e->unk10.x + p->z * p->z) >> 6;
        lim = 0xB00;
        if (dist > lim) {
            s32 a;

            a = get_angle(e->unk10.x, p->z) & 0xFFF;
            e->unk10.x -= ((dist - 0xAFC) * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
            p->z -= ((dist - 0xAFC) * rcossin_tbl[a].unk2) >> 12;
            ang = a;
        } else {
            ang = -1;
        }
        if (ang != -1) {
            s32 w;
            s32 h;

            PlaySfx(0x6CC, NULL, NULL, 0x1000, 0x1E00, 0);
            spd = csqrt(ext->unkB4 * ext->unkB4 + ext->unkD4 * ext->unkD4) >> 6;
            w = (ang + 0x800) & 0xFFF;
            n[0] = rcossin_tbl[(w - 0x400) & 0xFFF].unk2;
            n[2] = rcossin_tbl[w].unk2;
            d[0] = -rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2;
            d[2] = -rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2;
            dot = (d[0] * n[0] >> 12) + (d[2] * n[2] >> 12);
            rv[0] = d[0] - (dot * n[0] >> 11);
            rv[2] = d[2] - (dot * n[2] >> 11);
            h = get_angle(rv[0], rv[2]) + 0x800;
            m->Engine->unk1E = h;
            ext->unkB4 = -(spd * rcossin_tbl[(h - 0x400) & 0xFFF].unk2 >> 12);
            ext->unkD4 = -(spd * rcossin_tbl[h & 0xFFF].unk2 >> 12);
        }
    }
    ext->unk84->unk10 = m->Engine->unk10;
    ext->unk84->unk10.y = 0x80;
}

void PlotDinoScratch(NAS *m) {
    subengine *s;
    EXTengine *e;
    s32 dx;
    s32 v;
    u16 ang;

    s = m->Engine->sub;
    while (s != NULL) {
        if (!(s->flags & 0x8000)) {
            break;
        }
        s = s->next;
    }
    e = Heroes[m->UserInfo.user1].hero_shadow;
    if (e != NULL) {
        if ((e->unk10.x - e->OldXYZ.x) * (e->unk10.x - e->OldXYZ.x)
            + (e->unk10.z - e->OldXYZ.z) * (e->unk10.z - e->OldXYZ.z) > 0 && s != NULL
            && Heroes[m->UserInfo.user1].hero_mode == 0x66
            && Heroes[m->UserInfo.user1].hero_anim->Aindex >= 0xE) {
            if (Heroes[m->UserInfo.user1].hero_anim->Aindex == 0xE) {
                if (m->UserInfo.user1 == 0) {
                    PlaySfx(0x6AC, &Heroes[0].hero_eng->sfx, &Heroes[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 1) {
                    PlaySfx(0x6AD, &Heroes[1].hero_eng->sfx, &Heroes[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 2) {
                    PlaySfx(0x6AE, &Heroes[2].hero_eng->sfx, &Heroes[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 3) {
                    PlaySfx(0x6AF, &Heroes[3].hero_eng->sfx, &Heroes[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
            }
            m->UserInfo.user2++;
            if (!(s->flags & 0x8000)) {
                *(SLONG_XYZ *)&s->unk04 = Heroes[m->UserInfo.user1].hero_eng->unk10;
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x > 0x1000) {
                    s->unk50 = 1;
                    dx = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = dx;
                    s->unk20.y = dx;
                    s->unk20.x = dx;
                }
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x < 0x1000) {
                    s->unk50 = 1;
                    v = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = v;
                    s->unk20.y = v;
                    s->unk20.x = v;
                }
                s->flags = 0x40008041;
                ang = Heroes[m->UserInfo.user1].hero_eng->unk1E;
                s->unk52 = 1;
                s->unk76 = 0x400;
                s->unk12 = ang + 0x800;
            }
        }
    }
    for (s = m->Engine->sub; s != NULL; s = s->next) {
        if (s->flags & 0x8000) {
            s->unk76 += 0x40;
            if ((s16)s->unk76 > 0xC00) {
                s->flags = 0;
                s->unk20.z = 0x1000;
                s->unk20.y = 0x1000;
                s->unk20.x = 0x1000;
            }
        }
    }
}

void DinoInitGems(void) {
    DinoGems[0] = DinoGem0;
    DinoGems[1] = DinoGem1;
    DinoGems[2] = DinoGem2;
}

void InitDinoScratch(NAS *m) {
    EXTengine *eng;
    subengine *sub;

    if (eng_add_subs(m->Engine, 0x10) != 0) {
        eng = m->Engine;
        sub = eng->sub;
        eng->flags |= 0x200000;
        m->UserInfo.user1 = 0;
        m->UserInfo.user2 = 0;
        if (sub != NULL) do {
            sub->flags = 0x40000000;
            sub->u6C.dbase = MapScene->dbase;
            sub->entity = 0x2003;
            sub->unk51 = 2;
            sub->unk14 = 0;
            sub->unk12 = 0;
            sub->unk10 = 0;
            sub->unk76 = 0x800;
            sub = sub->next;
        } while (sub != NULL);
    }
}

s32 DinoClampToArena(s32 *p, s32 arg1) {
    s32 d;
    s32 ang;

    d = csqrt(p[0] * p[0] + p[2] * p[2]) >> 6;
    if (0xB00 - arg1 < d) {
        d -= 0xB00;
        d += arg1;
        ang = get_angle(p[0], p[2]) & 0xFFF;
        d += 4;
        p[0] -= d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        p[2] -= d * rcossin_tbl[ang].unk2 >> 12;
        return ang;
    }
    return -1;
}

s32 DinoHeroTestStop(s32 arg0) {
    if ((csqrt(Heroes[arg0].hero_move->x_vel * Heroes[arg0].hero_move->x_vel +
               Heroes[arg0].hero_move->z_vel * Heroes[arg0].hero_move->z_vel) >> 6) < 0x20) {
        if (Heroes[arg0].hero_mode != 0x63) {
            SetHeroAction(arg0, 0x63);
        }
        return 1;
    }
    return 0;
}

s32 DinoHeroTestIdle(s32 arg0) {
    if (((PadButtons *)Pads)[arg0].unk50 & 0xF0) {
        return 0;
    }
    SetHeroAction(arg0, 0x63);
    return 1;
}

s32 DinoHeroTestRun(s32 arg0) {
    if (!(((PadButtons *)Pads)[arg0].unk50 & 0xF0)) {
        return 0;
    }
    SetHeroAction(arg0, 0x64);
    return 1;
}

void DinoHeroShoot(s32 arg0) {
    DinoHeroCollide(arg0);
    ProcessHeroAnim(arg0);
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x68) {
        SetHeroAction(arg0, 0x68);
    }
}

void SetDinoHeroShoot(s32 player) {
    SetHeroSeq(player, 0x4F);
    if (player == 0) {
        PlaySfx(0x6B3, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (player == 1) {
        PlaySfx(0x6B4, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (player == 2) {
        PlaySfx(0x6B5, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (player == 3) {
        PlaySfx(0x6B6, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void SetDinoHeroRun(s32 player) {
    SetHeroSeq(player, 0x4E);
}

void DinoHeroIdle(s32 arg0) {
    DinoHeroCollide(arg0);
    DinoHeroTestRun(arg0);
    DinoHeroTestShoot(arg0);
    ProcessHeroAnim(arg0);
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x68) {
        SetHeroAction(arg0, 0x68);
    }
}

void SetDinoHeroIdle(s32 arg0) {
    EXTheromove *mv;

    mv = Heroes[arg0].hero_move;
    SetHeroSeq(arg0, 0x4D);
    if (Heroes[arg0].hero_eng->sfx != NULL) {
        StopSfx(Heroes[arg0].hero_eng->sfx);
    }
    mv->x_acc = 9;
    mv->z_acc = 9;
    mv->z_dec = 9;
    mv->x_dec = 9;
    mv->mass = 0x100;
}

void DinoInitHero(s32 arg0) {
    SetHeroAction(arg0, 0x63);
    Heroes[arg0].hero_eng->unk1E = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
    Heroes[arg0].hero->Camera.height = 0x600;
}

void DinoMovesSwapB(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        s32 av = a->x_vel;
        s32 bv = b->x_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        av = a->z_vel;
        bv = b->z_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

s32 DinoFloorKind(s32 arg0) {
    s32 i;
    s32 best;

    best = -1;
    i = 3;
    do {
        if (PointInZone((s32 *)&Heroes[arg0].hero_eng->unk10, i) != 0) {
            best = i;
        }
        i--;
    } while (i > 0);
    return best;
}

void InitDinoSmoke(NAS *arg0) {
    s16 *p;

    ScriptCursorFirst(arg0, &DinoSmokeScriptInit);
    arg0->Engine->sub->u6C.dbase = &HeroModelBank[DB_DragonObjects];
    arg0->Engine->sub->flags |= 0x40000041;
    p = &GameMode;
    do {
        arg0->UserInfo.user1 = 0;
    } while (0);
    if (*p == 0x1C) {
        arg0->Engine->sub->flags = 0x40008021;
    }
}

s32 DinoKeepInBounds(u8 *arg0) {
    s32 var_v1;

    var_v1 = 0;
    if ((*(s32*)((u8*)(arg0) + 16)) >= 0x801) {
        (*(s32*)((u8*)(arg0) + 16)) = 0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 16)) < -0x800) {
        (*(s32*)((u8*)(arg0) + 16)) = -0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 24)) >= 0x801) {
        (*(s32*)((u8*)(arg0) + 24)) = 0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 24)) < -0x800) {
        (*(s32*)((u8*)(arg0) + 24)) = -0x800;
        var_v1 = 1;
    }
    return var_v1;
}

void FirstTriggersDINO(void) {
}

void DinoRestartLevel(void) {
}

void DinoChooseWinner(s32 arg0, s32 arg1) {
    u8 *g = GameInfo;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x40) {
            DinoChooseWinnerTeam(arg0, (u16)arg1);
        } else if (f & 0x20) {
            DinoChooseWinnerScore(arg0, (u16)arg1);
        } else if (f & 0x80) {
            DinoChooseWinnerScore(arg0, (u16)arg1);
        } else {
            DinoChooseWinnerPuffs(arg0, (u16)arg1);
        }
    }
}

void DINOSetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseDINOFirst;
    LevelSpecs.LevelInit = InitialiseDINO;
    LevelSpecs.LevelRestartInit = InitialiseDINO;
    LevelSpecs.LevelRoutine = UpdateDINO;
    LevelSpecs.EarliestTriggers = FirstTriggersDINO;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
}

void DinoHeroWinner(s32 arg0) {
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        FinishLevel();
    }
    ProcessHeroAnim(arg0);
}

void SetDinoHeroWinner(s32 arg0) {
    StartHeroVictory(arg0);
    SetHeroSeq(arg0, 0x4D);
    Heroes[arg0].hero_eng->unk20 = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
    if (Heroes[arg0].hero_eng->sfx != NULL) {
        StopSfx(Heroes[arg0].hero_eng->sfx);
    }
    RequestHeroAnim(arg0, 0);
}

s32 TestDinoHeroWon(s32 id) {
    if (Heroes[id].hero_status & 0x200) {
        if (Heroes[id].hero_mode != 0x68) {
            SetHeroAction(id, 0x68);
        }
        return 1;
    }
    return 0;
}

void Dino_HeroBot(void) {
    DinoAI_Think();
}

s32 DinoFindClosestPlayer(s32 self, s32 *dist) {
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
        if (i != self &&!((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                do {
                    best = i;
                } while (0);
            }
        }
        do {
            h++;
        } while (0);
        i++;
        goto loop;
    }
    *dist = min;
    return best;
}

s32 DinoAI_SetNearestGem(s32 *arg0, s32 arg1) {
    s32 v = DinoAI_NearestGem(arg1);

    if (v != -1) {
        arg0[10] = v;
        return 1;
    }
    return 0;
}

void DinoUnusedStub(void) {
}

s32 DinoGemFree(s32 arg0) {
    DinoExt *ext;
    s32 ret;

    ext = FindExtension(DinoGems[arg0], 0x10000000);
    if (ext != NULL) {
        ret = 1;
        if (ext->unkC == ret) {
            return ret;
        }
    }
    return 0;
}

s32 DinoHeroHasGem(s32 arg0) {
    DinoExt *ext;

    ext = FindExtension(Heroes[arg0].hero, 0x10000000);
    if (ext != NULL && ext->unk14 != 0) {
        return 1;
    }
    return 0;
}

s32 DinoGemLoose(s32 arg0) {
    DinoExt *ext;

    ext = FindExtension(DinoGems[arg0], 0x10000000);
    if (ext != NULL) {
        s32 v = ext->unkC;
        if (v == 0 || v == 3 || v == 4) {
            return 1;
        }
        return 0;
    }
    return 0;
}

s32 DinoAI_TargetShare(s32 arg0, s32 arg1) {
    s32 total;
    s32 result;
    s32 i;

    result = 0;
    total = 0;
    if (Heroes[arg1].hero_status & 0x2000) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0x2000)) {
            total += AiPlayerSkill[i].quality << 7;
        }
    }
    if (total != 0) {
        result = (AiPlayerSkill[arg1].quality << 19) / total;
    }
    return result;
}

s32 DinoPadAngle(s32 arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0 & 0x80) {
        var_a1 = 0xC00;
    }
    if (arg0 & 0x20) {
        var_a1 = 0x400;
    }
    if (arg0 & 0x10) {
        var_a1 = 0;
    }
    if (arg0 & 0x40) {
        var_a1 = 0x800;
    }
    if ((arg0 & 0x90) == 0x90) {
        var_a1 = 0xE00;
    }
    if ((arg0 & 0xC0) == 0xC0) {
        var_a1 = 0xA00;
    }
    if ((arg0 & 0x30) == 0x30) {
        var_a1 = 0x200;
    }
    if ((arg0 & 0x60) == 0x60) {
        var_a1 = 0x600;
    }
    return var_a1;
}

s32 DinoCompareXZ(u8 *arg0, u8 *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0));
    temp_a0 = (*(s32*)((u8*)(arg1) + 0));
    var_v1 = (temp_a0 < temp_v0) << 7;
    if (temp_v0 < temp_a0) {
        var_v1 |= 0x20;
    }
    temp_a2 = (*(s32*)((u8*)(arg0) + 8));
    temp_a0_2 = (*(s32*)((u8*)(arg1) + 8));
    if (temp_a0_2 < temp_a2) {
        var_v1 |= 0x40;
    }
    if (temp_a2 < temp_a0_2) {
        var_v1 |= 0x10;
    }
    return var_v1;
}
