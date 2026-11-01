#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/n2o_ovl.h"
#include "bash/n2o.h"
#include "bash/arkenoid.h"

/* the exe's data the tables point at, not declared in the headers yet */

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char N2OAnimWin[] = "WIN";
const char N2OAnimBreathe[] = "BREATHE";
const char N2OAnimBank[] = "BANK";
const char N2OAnimDie[] = "DIE";

/* data, in address order */
VictorySeq N2OVictorySeqs[4] = {
    { &N2OAnimWin, 0x28 },
    { &N2OAnimWin, 0x28 },
    { &N2OAnimWin, 0x28 },
    { &N2OAnimWin, 0x28 },
};
SLONG_XYZ N2OBallStarts[6] = {
    { -0x800, -0x20, -0x800 }, { 0x800, -0x20, -0x800 }, { -0x800, -0x20, -0x800 },
    { 0x800, -0x20, -0x800 }, { -0x800, -0x20, 0x800 }, { 0x800, -0x20, 0x800 },
};
N2OCircle N2OPillars[4] = {
    { 0x800, 0x800, 0x280 },
    { 0x800, -0x880, 0x280 },
    { -0x800, 0x800, 0x280 },
    { -0x800, -0x880, 0x280 },
};
SLONG_XYZ N2OCorners[4] = {
    { -0x800, -0x20, -0x800 },
    { 0x800, -0x20, -0x800 },
    { -0x800, -0x20, 0x800 },
    { 0x800, -0x20, 0x800 },
};
s16 N2OUnusedTable[124] = {
    0xA, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 1, 3, 0, 2, 0x40, 0x40, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x40, 0x40, 0x40, 0x40, -0x20, 0, 0, 0, 0, -0x20, 0x40, 0x40,
    -0x20, -0x20, -0x10, 0, 0, 0, 0, -0x10, -0x20, -0x20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x20, 0x20, 0x10,
    0, 0, 0, 0, 0x10, 0x20, 0x20, 0x40, 0x40, 0x20, 0, 0, 0, 0, 0x20, 0x40, 0x40, -0x40, -0x40,
    -0x20, -0x20, -0x20, -0x20, -0x20, -0x20, -0x40, -0x40,
};
N2OStep N2OBossSteps[22] = {
    { 0, 0, 0x168 }, { 0xA, 2, 0x78 }, { 4, 0, 0x78 }, { 0, 0, 0x1E }, { 0, 1, 0x12C },
    { 0xA, 2, 0x3C }, { 4, 0, 0xB4 }, { 0, 0, 0x1E }, { 0, 1, 0x12C }, { 0xA, 2, 0x5A },
    { 3, 0, 0x78 }, { 0, 0, 0x1E }, { 0, 1, 0x1C2 }, { 0xA, 2, 0x3C }, { 4, 0, 0x78 },
    { 0, 0, 0x1E }, { 0, 1, 0x12C }, { 0xA, 2, 0x5A }, { 3, 0, 0x78 }, { 0, 0, 0x1E },
    { 0, 1, 0x12C }, { 0x63, 0, 5 },
};
u16 N2OKickSfx[3] = { 0x581, 0x582, 0x2C2 };
u16 N2OBossShotSfx = 0x2C2;
u16 N2OBallBounceSfx[4] = { 0x58D, 0x58E, 0x58F, 0x590 };
u16 N2OBallHitSfx[4] = { 0x57C, 0x57D, 0x57E, 0x57F };
u16 N2OBallSfx[10] = { 0x587, 0x588, 0x589, 0x58A, 0x58B, 0x58C, 0x58D, 0x58E, 0x58F, 0x590 };
u16 N2OMissileSfx[4] = { 0x2A3, 0x2A4, 0x2A5, 0x2A6 };
u16 N2OHitSfx[4] = { 0x2A7, 0x2A8, 0x2A9, 0x2AA };
u16 N2OExplosionSfx[8] = { 0x2AB, 0x2AC, 0x2AD, 0x2AE, 0x578, 0x579, 0x57A, 0x57B };
u16 N2OUnusedRows[10][5] = {
    { 0, 0, 0, 0, 0 }, { 0x18, 0xA, 0xF, 0x1C, 0x417F }, { 0x18, 0xA, 0xF, 0x1C, 0x41FF },
    { 0x18, 0xA, 0xF, 0x1C, 0x41FF }, { 0x18, 0xA, 0xF, 0x1C, 0x41FF },
    { 0x15, 7, 0xE, 0x1C, 0x407F }, { 0x15, 7, 0xE, 0x1C, 0x417F },
    { 0x15, 7, 0xE, 0x1C, 0x41FF }, { 0x18, 0xA, 0xF, 0x1C, 0x407F }, { 0, 0, 0, 0, 0 },
};
s32 N2OUnusedSteps[2] = { 0x40000063, 0x8000 };
void *N2OUnusedAnimList[1] = { &N2OUnusedSteps };
ScriptInit N2OUnusedScriptInit = { 0, (void *)&N2OUnusedAnimList, 0x401 };
hero_seq CRA_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4180, 0x8000, 0,
};
hero_seq CRA_seq_n2oidle[6] = {
    0, 0x8007, 0x6D, 0x4080, 0x8005, 0x21,
};
hero_seq CRA_seq_n2otaunt[8] = {
    0, 0x800A, 7, 0x8007, 0x34, 0x4200, 0x8004, 0x27,
};
hero_seq CRA_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq CRA_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_n2owin[6] = {
    0, 0x8007, 0x6D, 0x4200, 0x8000, 0,
};
hero_seq CRA_seq_n2ovictory[6] = {
    0, 0x8007, 0x44, 0x4100, 0x8000, 0,
};
hero_seq CRA_seq_n2olose[10] = {
    0, 0x8007, 0x13, 0x4280, 0x8007, 0x14, 0x4293, 0x8000, 3, 0,
};
hero_seq COC_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_n2oidle[6] = {
    0, 0x8007, 0x29, 0x4080, 0x8005, 0x21,
};
hero_seq COC_seq_n2otaunt[8] = {
    0, 0x800A, 0x39, 0x8007, 0x31, 0x4180, 0x8004, 0x27,
};
hero_seq COC_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq COC_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_n2owin[6] = {
    0, 0x8007, 0x31, 0x4180, 0x8000, 0,
};
hero_seq COC_seq_n2ovictory[6] = {
    0, 0x8007, 0x40, 0x4100, 0x8000, 0,
};
hero_seq COC_seq_n2olose[10] = {
    0, 0x8007, 0x1E, 0x4280, 0x8007, 0x14, 0x429E, 0x8000, 3, 0,
};
hero_seq TIN_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4080, 0x8000, 0,
};
hero_seq TIN_seq_n2oidle[6] = {
    0, 0x8007, 0x2E, 0x4200, 0x8005, 0x21,
};
hero_seq TIN_seq_n2otaunt[8] = {
    0, 0x800A, 0x12E, 0x8007, 0x2A, 0x4180, 0x8004, 0x27,
};
hero_seq TIN_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq TIN_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_n2owin[6] = {
    0, 0x8007, 0x2A, 0x4200, 0x8000, 0,
};
hero_seq TIN_seq_n2ovictory[10] = {
    0, 0x8007, 0xE, 0x4100, 0x8007, 0x1F, 0x410E, 0x8000, 3, 0,
};
hero_seq TIN_seq_n2olose[10] = {
    0, 0x8007, 0x1E, 0x4280, 0x8007, 0x13, 0x429E, 0x8000, 3, 0,
};
hero_seq CTX_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq CTX_seq_n2oidle[6] = {
    0, 0x8007, 0x3C, 0x4080, 0x8005, 0x21,
};
hero_seq CTX_seq_n2otaunt[8] = {
    0, 0x800A, 0x66, 0x8007, 0x34, 0x4180, 0x8004, 0x27,
};
hero_seq CTX_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq CTX_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_n2owin[6] = {
    0, 0x8007, 0x34, 0x4180, 0x8000, 0,
};
hero_seq CTX_seq_n2ovictory[6] = {
    0, 0x8007, 0x51, 0x4100, 0x8000, 0,
};
hero_seq CTX_seq_n2olose[10] = {
    0, 0x8007, 0x23, 0x4280, 0x8018, 0x63, 0x42A2, 0x8000, 3, 0,
};
hero_seq BRO_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_n2oidle[6] = {
    0, 0x8007, 0x38, 0x4080, 0x8005, 0x21,
};
hero_seq BRO_seq_n2otaunt[8] = {
    0, 0x800A, 0x98, 0x8007, 0x32, 0x4180, 0x8004, 0x27,
};
hero_seq BRO_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq BRO_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_n2owin[6] = {
    0, 0x8007, 0x3C, 0x4180, 0x8000, 0,
};
hero_seq BRO_seq_n2ovictory[6] = {
    0, 0x8007, 0x4B, 0x4100, 0x8000, 0,
};
hero_seq BRO_seq_n2olose[10] = {
    0, 0x8007, 0x23, 0x4280, 0x8018, 0x63, 0x42A2, 0x8000, 3, 0,
};
hero_seq DNG_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq DNG_seq_n2oidle[6] = {
    0, 0x8007, 0x1F, 0x4080, 0x8005, 0x21,
};
hero_seq DNG_seq_n2otaunt[8] = {
    0, 0x800A, 0xFC, 0x8007, 0x3D, 0x4180, 0x8004, 0x27,
};
hero_seq DNG_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq DNG_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_n2owin[6] = {
    0, 0x8007, 0x3D, 0x4180, 0x8000, 0,
};
hero_seq DNG_seq_n2ovictory[6] = {
    0, 0x8007, 0x1E, 0x4100, 0x8000, 0,
};
hero_seq DNG_seq_n2olose[10] = {
    0, 0x8007, 0xC, 0x4280, 0x8007, 0xE, 0x428B, 0x8000, 3, 0,
};
hero_seq KON_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4180, 0x8000, 0,
};
hero_seq KON_seq_n2oidle[6] = {
    0, 0x8007, 0x3C, 0x4100, 0x8005, 0x21,
};
hero_seq KON_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq KON_seq_n2otaunt[8] = {
    0, 0x800A, 0x160, 0x8007, 0x2A, 0x4200, 0x8004, 0x27,
};
hero_seq KON_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_n2owin[6] = {
    0, 0x8007, 0x2A, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_n2ovictory[6] = {
    0, 0x8007, 0x44, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_n2olose[10] = {
    0, 0x8007, 0xC, 0x4280, 0x8007, 0x26, 0x428C, 0x8000, 3, 0,
};
hero_seq KAN_seq_n2obreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_n2omove[6] = {
    0, 0x8007, 0x1C, 0x4080, 0x8000, 0,
};
hero_seq KAN_seq_n2oidle[6] = {
    0, 0x8007, 0x4B, 0x4180, 0x8005, 0x21,
};
hero_seq KAN_seq_n2otaunt[8] = {
    0, 0x800A, 0xCA, 0x8007, 0x28, 0x4200, 0x8004, 0x27,
};
hero_seq KAN_seq_n2ohit[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8004, 0x27,
};
hero_seq KAN_seq_n2okick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_n2owin[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_n2ovictory[6] = {
    0, 0x8007, 0x40, 0x4280, 0x8000, 0,
};
hero_seq KAN_seq_n2olose[10] = {
    0, 0x8007, 0x23, 0x4100, 0x8018, 0xA, 0x4122, 0x8000, 3, 0,
};
u16 N2OAiSkill[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 N2OAiSkillAlt[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 N2OBossAiSkill[32] = {
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
    0x11, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10,
};
u16 N2OBossAiSkillAlt[32] = {
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
    0x11, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10,
};
s16 N2OPreLoadDB[4] = { 0x1C, 0x17, 0x1A, -1 };
char *N2OAnims[4] = {
    (char *)N2OAnimBreathe, (char *)N2OAnimBank, (char *)N2OAnimDie, NULL,
};
s16 N2OLevelDB[4] = { 0x5E, 0x5D, 0x7A, -1 };
WWInf N2OWorldInfo[1] = {
    { 0x5C, N2OAnims, N2OLevelDB, 1 },
};
GroupFile N2OFiles[9] = {
    &HeroModelBank[DB_Font], &DatIndex[331], &DatIndex[330], &HeroModelBank[DB_Loading], &DatIndex[327],
    &DatIndex[326], &HeroModelBank[DB_Interface], &DatIndex[329], &DatIndex[328], &HeroModelBank[DB_Powerups],
    &DatIndex[325], &DatIndex[324], &HeroModelBank[DB_Angel], &DatIndex[323], &DatIndex[322],
    &HeroModelBank[DB_PlayerVehicle], &DatIndex[319], &DatIndex[318], &HeroModelBank[DB_BallsEffects], &DatIndex[321],
    &DatIndex[320], &HeroModelBank[DB_Oxide], &DatIndex[317], &DatIndex[316], 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 0x1F's) */
file_group_load_info file_group_N2O = { &FileGroups[6], N2OFiles };
u16 N2OLUPtab[6] = { 3, 0x1605, 0x1508, 0x1C06, 0x1C08, 0 };
TrigLookUp N2OLUP = { N2OLUPtab, 5 };
PreMap N2O_PREMAP = { N2OPreLoadDB, N2OWorldInfo, &N2OLUP };
/* the shell settings (engine world record 0x010C) */
shell_setup n2o_shell_info = { { 0xC8, 0x2BC, 0x200 } };
u16 N2OHeroModels[10] = {
    0, 0x72, 0x73, 0x74, 0x75, 0x78, 0x79, 0x76, 0x77, 0,
};
s32 N2OHeroStarts[4][3] = { { 0, 0x1C, -0xA00 }, { -0x300, 0x1C, -0xA00 }, { 0x300, 0x1C, -0xA00 }, { 0, 0x1C, 0xA00 } };
NAS *N2OHeroList = NULL;
NAS *N2OBossNas = 0;
s32 N2OUnusedPair[2] = { 1, 2 };
N2OEdge N2OAimEdgesA[4] = {
    { -0x800, -1, -0x7A0, -1, 0x800, 0, -0x7A0, -1 },
    { -0x800, -1, -0x7A0, -1, 0x800, 0, -0x7A0, -1 },
    { -0x800, -1, 0x7A0, 0, 0x800, 0, 0x7A0, 0 }, { -0x800, -1, 0x7A0, 0, 0x800, 0, 0x7A0, 0 },
};
N2OEdge N2OAimEdgesB[4] = {
    { -0x500, -1, -0x800, -1, 0x500, 0, -0x800, -1 },
    { -0x500, -1, -0x800, -1, 0x500, 0, -0x800, -1 },
    { -0x500, -1, 0x800, 0, 0x500, 0, 0x800, 0 }, { -0x500, -1, 0x800, 0, 0x500, 0, 0x800, 0 },
};
u16 N2OAimAngles[32] = {
    0x100, 0x140, 0x180, 0x1C0, 0x280, 0x240, 0x280, 0x2C0, 0x300, 0x340, 0x380, 0x3C0, 0x480,
    0x440, 0x480, 0x4C0, 0x500, 0x540, 0x580, 0x5C0, 0x680, 0x640, 0x680, 0x6C0, 0x700, 0x740,
    0x780, 0x7C0, 0x880, 0x840, 0x880, 0x8C0,
};
s16 N2OExplodeScales[7][3] = {
    { 0x5000, 0x5000, 0x5000 }, { 0x4000, 0x2000, 0x4000 }, { 0x1800, 0x1000, 0x1800 },
    { 0x800, 0xA00, 0x800 }, { 0x200, 0x200, 0x200 }, { 0, 0, 0 }, { 0, 0, 0 },
};
u16 N2OBallPhase = 0;
#define N2O_BALL_WALL 0x800
u16 N2OHeroPlaced[4] = {0};
s16 N2OBossAnimTimer = 0;
s16 N2OHeroSpeed[4] = {0};
N2OCircle *N2OPillarList = 0;
N2OHeroBox N2OHeroBoxes[4] = {0};
N2ORec N2OSlotRecs[4] = {0};
N2OSlot N2OSlots[4] = {0};
s16 N2OBallClock = 0;
s16 N2OBallCount = 0;
s16 N2OBallDelay = 0;
s16 N2OBossStepKind = 0;
s16 N2OBossPad = 0;
s16 N2OBossStepFlag = 0;
s16 N2OBossStepTimer = 0;
s16 N2OBossStep = 0;
s16 N2OMissileCount = 0;
s16 N2OMissileDelay = 0;
u16 N2OBossAimDelay = 0;
u16 N2OSkillBias = 0;
s16 N2OUnusedHalf[1] = {0};
u16 N2OWallsHit = 0;
s32 N2ODeadWallUp = 0;
s32 N2OBossShooting = 0;
u16 N2OHeroPushTimer = 0;
N2ORound N2OBossRound = {0};
u32 *N2OPhaseObject = 0;
s16 N2OBallsLost = 0;
s32 N2OBossClock = 0;
s32 N2OUnusedWords[4] = {0};
hero_seq CRA_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_n2ofly[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};

void InitialiseN2OFirst(void) {
    NoxStarted = 0;
    if (!(GameExitFlags & 0x200)) {
        N2OBallPhase = 0;
    }
    if (N2OBallPhase == 1) {
        HeroHooks.PreMoveHero = (void *)N2OPreMoveHero;
        HeroHooks.KeepInBounds = (void *)N2O_KeepInBounds;
        HeroHooks.MoveHero = (void *)N2OMoveHero;
        HeroHooks.CreateShadow = (void *)N2OInitShadow;
        HeroHooks.UpdateShadow = (void *)N2OUpdateShadow;
    } else {
        HeroHooks.PreMoveHero = (void *)NoxPreMoveHero;
        HeroHooks.MoveHero = (void *)NoxMoveHero;
        HeroHooks.KeepInBounds = (void *)Nox_KeepInBounds;
        HeroHooks.HeroFacing = (void *)NoxHeroFacing;
        HeroHooks.CreateShadow = (void *)NoxInitShadow;
        HeroHooks.UpdateShadow = (void *)NoxUpdateShadow;
        NoxBossDead = 0;
    }
    HeroHooks.InitHero = N2OInitHero;
    HeroHooks.HeroModels = N2OHeroModels;
    HeroHooks.VictorySeqs = N2OVictorySeqs;
    HeroActHit = 0x2A;
    HeroActDead = 0x2E;
    LevelSpecs.GetContour = (void *)GetN2OContour;
    OVG->levelFlags |= 0x100;
    if (OVG->teams == 0) {
        OVG->heroes[1] = 9;
    }
    OVG->heroes[2] = 9;
    OVG->heroes[3] = 9;
    AiSkillTable = OVG->teams != 0 ? N2OAiSkillAlt : N2OAiSkill;
}

void InitialiseN2O(void) {
    HeroInfo *h;
    EXTheromove *mv;
    s32 i;

    i = 0;
    World_floor_level = 0x2800;
    HeadOfBalls = NULL;
    HeadOfBalls2 = 0;
    N2OBallClock = 0;
    N2OBallCount = 0;
    N2OBallDelay = 50;
    N2OHeroList = 0;
    N2OBossStepKind = 0;
    N2OBossStepFlag = 0;
    N2OBossPad = 0;
    N2OMissileCount = 0;
    N2OMissileDelay = 50;
    N2OBossStepTimer = 0;
    N2OBossStep = 0;
    N2OBallsLost = 0;
    N2OWallsHit = 0;
    N2ODeadWallUp = 0;
    N2OBossShooting = 0;
    N2OHeroPushTimer = 0;
    N2OBossAimDelay = 6;
    MapScene->map->clear = 1;
    ((GameState *)GameInfo)->levelFlags &= ~0x800;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        mv = h->hero_move;
        h->hero_eng->xform.rot.vy = 0x800;
        mv->facing_angle = 0x800;
        mv->forward_angle = 0x800;
        if (N2OBallPhase == 1) {
            RemoveCamera(h->hero);
        }
        N2OPillarList = N2OPillars;
    }
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_BallsEffects]);
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_Angel]);
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_Oxide]);
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_PlayerVehicle]);
    SetPanelHeroIconB(0x5C, 0x3088);
    N2OBossRound.unk0 = 0;
    N2OBossRound.unk22 = i + 10;
    N2OBossRound.eng = 0;
    N2OBossRound.unk8 = 0;
    N2OBossRound.unkC = 0;
    N2OBossRound.unk10 = 0;
    N2OBossRound.unk14 = 0;
    N2OBossRound.unk18 = 0;
    N2OBossRound.unk1C = 0;
    N2OBossRound.unk20 = 0;
    N2OBossRound.unk24 = 0;
    N2OBossRound.unk26 = 0xFF;
    N2OBossRound.unk28 = 0;
    N2OBossRound.unk2A = 0;
    N2OBossRound.unk2C = 0;
    N2OBossRound.unk2E = 0;
    N2OBossRound.unk30 = 0;
    N2OBossRound.unk32 = 0;
    N2OBossRound.unk34 = 0;
    N2OBossRound.unk38 = 0;
    N2OBossRound.unk3C = 0;
    N2OShowPhaseObjects(N2OBallPhase);
    if (N2OBallPhase != 1) {
        N2OBossNas = CreateNAS(0, -0x200, 0xA00, 0x1C02, 0x11);
        NoxInit();
    } else {
        N2OInitBoss();
    }
    N2OInitSlots();
}

s32 N2O_KeepInBounds(EXTengine *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    N2OCircle *c;

    for (i = 0; i < 4; i++) {
        c = (N2OCircle *)((u8 *)N2OPillarList + i * 6);
        dx = arg0->XYZ.x - c->x;
        dz = arg0->XYZ.z - c->z;
        if (arg0->collision_box.flags & 0x40) {
            r = c->r + arg0->collision_box.radius;
        } else {
            r = c->r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            arg0->XYZ.x = N2OPillarList[i].x - ((r * sin_cos_table[(get_angle(-dx, -dz) - 0x400) & 0xFFF].unk2) >> 12);
            return 1;
        }
    }
    return 0;
}

s32 N2O_BA_BounceOnBounds(EXTengine *arg0, s32 arg1) {
    N2OCircle *base;
    SLONG_XYZ v;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 z;
    s32 ret;
    s32 cx;
    s32 cz;

    base = N2OPillarList;
    ret = 0;
    for (i = 0; i < 4; i++) {
        if (arg0->collision_box.flags & 0x40) {
            r = base[i].r + arg0->collision_box.radius;
        } else {
            r = base[i].r + 0x80;
        }
        cx = N2OPillarList[i].x;
        x = arg0->XYZ.x;
        dx = x - cx;
        cz = N2OPillarList[i].z;
        z = arg0->XYZ.z;
        dz = z - cz;
        if (dx * dx + dz * dz < r * r) {
            goto found;
        }
    }
    ret = -1;
    if (N2OWallsHit == 0) {
        goto fail;
    }
    dx = x;
    dz = z;
    if (arg0->collision_box.flags & 0x40) {
        r = 0x800 - arg0->collision_box.radius;
    } else {
        r = 0x800;
    }
    if (dx > r && (N2OWallsHit & 0x88)) {
        arg0->XYZ.x = r;
        ret = 0x400;
    }
    if (dx < -r && (N2OWallsHit & 0x44)) {
        arg0->XYZ.x = -r;
        do { ret = 0xC00; } while (0);
    }
    if (dz > r && (N2OWallsHit & 0x22)) {
        arg0->XYZ.z = r;
        ret = 0;
    }
    if (dz < -r && (N2OWallsHit & 0x11)) {
        arg0->XYZ.z = -r;
        ret = 0x800;
    }
    if (ret != -1) {
        PlaySfx(0x592, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    return ret;
found:
    v.x = cx;
    v.y = 0;
    v.z = N2OPillarList[i].z;
    N2OLineToCircleTest((SLONG_XYZ *)&arg0->XYZ.x, arg1, &v, r);
    return get_angle(N2OPillarList[i].x - arg0->XYZ.x, N2OPillarList[i].z - arg0->XYZ.z);
fail:
    return -1;
}

s32 N2O_BA_TestLaunchBounds(EXTengine *arg0) {
    N2OCircle *base = N2OPillarList;
    N2OCircle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 lim;
    s32 z;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->collision_box.flags & 0x40) {
            r = c->r + arg0->collision_box.radius;
        } else {
            r = c->r + 0x80;
        }
        dx = c->x;
        x = arg0->XYZ.x;
        dx = x - dx;
        z = arg0->XYZ.z;
        dz = z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    lim = 0x900;
    if (x > lim || x < -lim) {
        return 1;
    }
    if (z > lim) {
        return 1;
    }
    return z < -lim;
}

void N2OHeroEngineUpdate(s32 arg0) {
    VECTOR sp10;
    VECTOR *v;
    EXTengine *e = (&Heroes[arg0])->hero_eng;
    FlagRecord *r;
    N2OHeroBox *b;
    N2OHeroBox *base;
    s32 t;

    if (e->xform.dirty != 0) {
        rotscale_update(&e->xform);
    }
    gfx_load_xform(e->sub, &e->xform.mtx, &e->XYZ);
    r = FindCollisionBox(e, 1);
    if (r == NULL) goto next;
    {
        base = N2OHeroBoxes;
        b = (N2OHeroBox *)((u8 *)base + arg0 * 40);
        t = r->dy;
        b->a = t;
        b->b = r->height;
        b->c = r->radius;
        view_transform((SVECTOR *)r, &sp10);
        b->x0 = sp10.vx - (&Heroes[arg0])->hero_eng->XYZ.x;
        b->y0 = sp10.vy - (&Heroes[arg0])->hero_eng->XYZ.y;
        b->z0 = sp10.vz - (&Heroes[arg0])->hero_eng->XYZ.z;
    }
next:
    r = FindCollisionBox(e, 2);
    v = &sp10;
    if (r != NULL) {
        base = N2OHeroBoxes;
        t = r->radius;
        b = (N2OHeroBox *)((u8 *)base + arg0 * 40);
        b->d = t;
        view_transform((SVECTOR *)r, v);
        b->x1 = sp10.vx - (&Heroes[arg0])->hero_eng->XYZ.x;
        b->y1 = sp10.vy - (&Heroes[arg0])->hero_eng->XYZ.y;
        b->z1 = sp10.vz - (&Heroes[arg0])->hero_eng->XYZ.z;
    }
}

void N2OInitHero(s32 i) {
    HeroInfo *h;
    HeroInfo *hb;
    HeroInfo *hb2;
    HeroInfo *h2;
    s32 one;
    EXTengine *eng = Heroes[i].hero_eng;
    subengine *sub;
    subengine *n;
    s32 k;
    void *scene;

    Heroes[i].hero_health = 0xFF;
    switch (i) {
    case 0:
        if (GameTeams[0] != 0) {
            Heroes[0].hero_eng->XYZ.x = N2OHeroStarts[1][0];
            Heroes[0].hero_eng->XYZ.z = N2OHeroStarts[1][2];
        } else {
            Heroes[0].hero_eng->XYZ.x = N2OHeroStarts[0][0];
            Heroes[0].hero_eng->XYZ.z = N2OHeroStarts[0][2];
        }
        break;
    case 1:
        Heroes[i].hero_eng->XYZ.x = N2OHeroStarts[2][0];
        Heroes[i].hero_eng->XYZ.z = N2OHeroStarts[2][2];
        break;
    }
    k = 2;
    hb = Heroes;
    h = hb + i;
    eng->xform.order = 0;
    sub = h->hero_sub;
    eng_add_subs_n(eng, k);
    n = sub->next;
    if (n != NULL) {
        n->u6C.dbase = sub->u6C.dbase;
        n->flags |= 0x8000;
        h->hero_anim->sub = n;
        n = n->next;
        if (n != NULL) {
            n->u6C.dbase = &HeroModelBank[DB_PlayerVehicle];
            n->entity = 0x2005;
            n->xform.hasScale = 1;
            n->xform.order = 0;
            n->flags &= ~0x8000;
        }
    }
    k = 1;
    if (N2OBallPhase == k) {
        ((void (*)(s32, s32))N2OPlaceHero)(i, k);
        return;
    }
    N2OHeroPlaced[i] = 0xF;
    sub->u6C.dbase = MapScene->dbase;
    sub->entity = 0x4000;
    if ((u32)(i - 2) < 2) {
        sub->flags &= ~0x8000;
    }
    if (GameTeams[0] == 0 && i == k) {
        sub->flags &= ~0x8000;
    }
    sub->xform.hasScale = 1;
    sub->xform.order = 0;
    sub->xform.scale.vx = 0x1201;
    sub->xform.scale.vy = 0x1201;
    sub->xform.scale.vz = 0x1201;
    one = 1;
    sub->xform.dirty = one;
    FetchDefaultColBox(eng, 0x50);
    hb2 = Heroes;
    h2 = hb2 + i;
    AddToGroup(&N2OHeroList, h2->hero);
    SetHeroAction(i, 0x2F);
    h2->hero_eng->XYZ.y = -0x480;
    h2->hero_weapon_count = one;
    if (i == 0) {
        NoxResetHeroes();
    }
}

void N2OPlaceHero(s32 i) {
    HeroInfo *h;
    EXTengine *eng;
    subengine *sub;
    subengine *n;
    s32 z;
    HeroInfo *h0;
    HeroInfo *hb;
    HeroInfo *h1;
    HeroInfo *h3;
    HeroInfo *hp;
    s32 mode;

    hb = Heroes;
    h0 = &hb[i];
    RemoveCamera(h0->hero);
    if (h0->hero_status & 0x8000) {
        return;
    }
    eng = h0->hero_eng;
    mode = GameTeams[0];
    if (i == 3) {
        h3 = &Heroes[3];
        h3->hero_status = (h3->hero_status & ~0x1000) | 0x40;
        h3->hero_eng->flags |= 0x8000;
        sub = h3->hero_sub;
        sub->flags |= 0x8000;
        sub = sub->next;
        sub->flags |= 0x8000;
        return;
    }
    switch (i) {
    case 0:
        if (mode != 0) {
            Heroes[0].hero_eng->XYZ.x = N2OHeroStarts[1][0];
            Heroes[0].hero_eng->XYZ.z = N2OHeroStarts[1][2];
        } else {
            Heroes[0].hero_eng->XYZ.x = N2OHeroStarts[0][0];
            Heroes[0].hero_eng->XYZ.z = N2OHeroStarts[0][2];
        }
        break;
    case 1:
        h1 = &Heroes[1];
        h1->hero_eng->XYZ.x = N2OHeroStarts[2][0];
        h1->hero_eng->XYZ.z = N2OHeroStarts[2][2];
        break;
    }
    hp = Heroes;
    h = &hp[i];
    h->hero_eng->XYZ.y = World_floor_level;
    eng->xform.order = 0;
    eng->xform.rot.vz = 0;
    eng->xform.rot.vy = 0;
    eng->xform.rot.vx = 0;
    sub = h->hero_sub;
    n = sub->next;
    if (n != NULL) {
        z = ((u16 *)N2OHeroModels)[h->hero_name];
        n->flags |= 0x8000;
        n->XYZ.z = 0;
        n->XYZ.y = 0;
        n->XYZ.x = 0;
        n->u6C.dbase = &HeroModelBank[z];
        h->hero_anim->sub = n;
        n = n->next;
        if (n != NULL) {
            n->u6C.dbase = &HeroModelBank[DB_PlayerVehicle];
            n->entity = 0x2005;
            n->xform.hasScale = 1;
            n->xform.order = 0;
            n->XYZ.z = 0;
            n->XYZ.y = 0;
            n->XYZ.x = 0;
            n->flags &= ~0x8000;
        }
    }
    sub->u6C.dbase = &HeroModelBank[DB_PlayerVehicle];
    N2OHeroPlaced[i] = 0xF;
    sub->entity = 0x2003;
    sub->xform.scale.vx = 0x1000;
    sub->xform.scale.vy = 0x1000;
    sub->xform.scale.vz = 0x1000;
    sub->XYZ.z = 0;
    sub->XYZ.y = 0;
    sub->XYZ.x = 0;
    sub->xform.dirty = 1;
    N2OHeroEngineUpdate(i);
    FetchDefaultColBox(eng, 0x50);
    if (i == 1) {
        Heroes[1].hero_eng->XYZ.z = -0x2800;
    } else {
        Heroes[i].hero_eng->XYZ.z = -0x2200;
    }
    SetHeroAction(i, 0x26);
    Heroes[i].hero_weapon_count = 1;
}

void N2OPreMoveHero(s32 arg0) {
    EXTheromove *m;
    HeroInfo *h;
    HeroInfo *c;
    PadState *p;
    PadState *pads;
    s32 d;
    s32 max;
    s32 acc;
    s32 fast;
    s32 b;
    s32 v;

    c = Heroes;
    h = c + arg0;
    m = h->hero_move;
    h->hero_eng->flags |= 0x8000;
    pads = (PadState *)Pads;
    p = pads + arg0;
    max = 0x68;
    acc = 0x22;
    fast = 0;
    if ((p->flags & 1) || h->hero_ai->AImode != 0) {
        if (p->dir & 0xF00) {
            max = 0xA0;
            acc = 0x38;
        }
        if (p->dir & 0xA0) {
            if (m->velocity_enable != 0) {
                d = 0;
                if (HeroHooks.KeepInBounds(h->hero_eng) != 0) {
                    fast = 1;
                }
                b = p->dir;
                if (b & 0x80) {
                    d = -acc;
                }
                if (b & 0x20) {
                    d += acc;
                }
                if (fast != 0) {
                    if (d < 0) {
                        d -= max >> 1;
                    }
                    if (d > 0) {
                        d += max >> 1;
                    }
                }
                m->forward_speed += d;
                if (m->forward_speed > max) {
                    m->forward_speed = max;
                }
                if (m->forward_speed < -max) {
                    m->forward_speed = -max;
                }
                if (m->forward_speed == 0) {
                    if (d != 0) {
                        v = 1;
                        if (d < 0) {
                            v = -1;
                        }
                        m->forward_speed = v;
                    }
                }
            }
            N2OHeroSpeed[arg0] = m->forward_speed;
        } else {
            N2OHeroSpeed[arg0] = 0;
            if (HeroHooks.KeepInBounds(h->hero_eng) != 0) {
                m->forward_speed = 0;
            }
            if (m->forward_speed > 0) {
                v = m->forward_speed - 0x12;
                m->forward_speed = v;
                if (v >= 0) {
                    goto end;
                }
                m->forward_speed = 0;
            }
            if (m->forward_speed < 0) {
                v = m->forward_speed + 0x12;
                m->forward_speed = v;
                if (v > 0) {
                    m->forward_speed = 0;
                }
            }
        }
    end:
        m->forward_angle = m->facing_angle;
    }
}

void N2OMoveHero(s32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *f;
    s32 a1;
    s32 sp;
    s32 t;

    HeroInfo *c;
    HeroInfo *b;
    HeroInfo *e;

    c = Heroes;
    g = c + arg0;
    a1 = 0;
    if (!(g->hero_status & 0xFA00)) {
        s32 y;
        s32 d;
        t = ((GameFrameCount + arg0) << 6) & 0xFFF;
        a1 = sin_cos_table[(t - 0x400) & 0xFFF].unk2 * 17 >> 10;
        y = World_floor_level;
        d = (sin_cos_table[(t * 2 - 0x400) & 0xFFF].unk2 * 15 >> 11) - 0x80;
        g->hero_eng->XYZ.y = y + d;
    } else {
        g->hero_eng->XYZ.y = World_floor_level - 0x80;
    }
    ((void (*)(s32))HeroHooks.HeroFacing)(arg0);
    b = Heroes;
    f = b + arg0;
    sp = f->hero_move->forward_speed;
    if (sp != 0) {
        a1 = -sp;
        f->hero_eng->XYZ.x += sp;
    }
    sp = angle_diff(f->hero_eng->xform.rot.vz, a1);
    {
        EXTengine *e2 = f->hero_eng;
        e2->xform.rot.vz = (u16)e2->xform.rot.vz + (sp >> 2);
        N2OHeroBallCollision(arg0, e2);
    }
    if (N2OHeroPushTeammate(arg0) != 0) {
        if ((u16)N2OHeroPushTimer == 0) {
            PlaySfx(0x28A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        N2OHeroPushTimer = 4;
    } else if ((u16)N2OHeroPushTimer != 0) {
        N2OHeroPushTimer = (u16)N2OHeroPushTimer - 1;
    }
    e = Heroes;
    h = e + arg0;
    h->hero_eng->xform.dirty = 1;
    if (h->hero_mode == 0x26) {
        h->hero_eng->XYZ.y = World_floor_level - 0x80;
        return;
    }
    if (!(h->hero_status & 0xF800)) {
        if (h->hero_attack_timer != 0) {
            h->hero_sub->entity = 0x2001;
            return;
        }
        h->hero_sub->entity = 0x2003;
    }
}

void N2OHeroSetSpeed(s32 arg0) {
    u16 *p;
    u16 *b;
    u16 t;
    s32 v;

    v = N2OHeroSpeed[arg0];
    do {
        do {
            if (v >= 0) {
                goto pos;
            }
        } while (0);
    } while (0);
    t = N2OUnusedRows[Heroes[arg0].hero_name][2];
    goto done;
pos:
    t = N2OUnusedRows[Heroes[arg0].hero_name][3];
done:
    b = (u16 *)N2OHeroPlaced;
    p = b + arg0;
    if ((u32)(abs(*p - N2OUnusedRows[Heroes[arg0].hero_name][2]) - 2) >= 0x19U) {
        *p = t;
    }
    if ((u32)(abs(*p - N2OUnusedRows[Heroes[arg0].hero_name][3]) - 2) >= 0x19U) {
        *p = t;
    }
}

s32 N2OHeroUpdateSpeed(s32 arg0) {
    s32 v;
    s32 t;
    u32 u;
    u16 *p;
    u16 *d;
    subengine *s;

    v = N2OHeroSpeed[arg0];
    if (v == 0) {
        d = &N2OHeroPlaced[arg0];
        do {
        } while (0);
        p = (u16 *)N2OUnusedRows;
        p += Heroes[arg0].hero_name * 5;
        if (*d <= p[2]) {
            t = p[2];
        } else {
            t = p[3];
        }
        if (N2OHeroPlaced[arg0] == t) {
            if (p || Heroes[arg0].hero_name) {
                return 1;
            } else {
                return 1;
            }
        }
    } else {
        if (v < 0) {
            u = N2OUnusedRows[Heroes[arg0].hero_name][0];
        } else {
            u = N2OUnusedRows[Heroes[arg0].hero_name][1];
        }
        if (N2OHeroPlaced[arg0] == u) {
            goto skip;
        }
    }
    N2OHeroPlaced[arg0] += 1;
skip:
    if (N2OHeroPlaced[arg0] >= 0x1D) {
        N2OHeroPlaced[arg0] = 1;
    }

    s = Heroes[arg0].hero_sub;
    s = s->next;
    if (s != 0) {
        s->entity = N2OUnusedRows[Heroes[arg0].hero_name][4] + N2OHeroPlaced[arg0];
        return 0;
    }
    return 1;
}

void N2OHeroEnter(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 z;
    s32 s;
    s32 n;

    if (N2OHeroUpdateSpeed(arg0) != 0) {
        ProcessHeroAnim(arg0);
    }
    base = Heroes;
    h = &base[arg0];
    if (h->hero_eng->XYZ.z < -0xA00) {
        h->hero_eng->XYZ.z += 0x20;
        z = h->hero_eng->XYZ.z;
        s = z + 0xA00;
        n = -(s >> 6);
        s >>= 3;
        if (s > 0x40) {
            s = 0x40;
        }
        if (s != 0) {
            h->hero_eng->XYZ.z = z - s;
        } else {
            h->hero_eng->XYZ.z = -0xA00;
        }
        n = 0x1E - n;
        if (n <= 0) {
            n = 1;
        }
        if (n > 0x1E) {
            n = 0x1E;
        }
        Heroes[arg0].hero_sub->entity = n + 0x3FFF;
    } else {
        if (N2OPhaseObject != NULL) {
            *N2OPhaseObject &= 0xFFFF7FFF;
        }
        h->hero_eng->XYZ.z = -0xA00;
        SetHeroAction(arg0, 0x27);
        h->hero_sub->entity = 0x2003;
    }
}

void N2OHeroIdle(s32 arg0) {
    s32 ret;
    u16 st;
    PadState *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    if (N2OHeroUpdateSpeed(arg0) != 0) {
        ProcessHeroAnim(arg0);
    }
    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        PadState *base = (PadState *)Pads;
        pad = &base[arg0];
        if (pad->dirPressed & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->lastDir &= ~0x8000;
            } else {
                SetHeroAction(arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed != 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction(arg0, 0x28);
        }
    }
}

void SetN2OHeroHit(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    subengine *s;

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    N2OHeroSetSpeed(arg0);
    SetHeroAction(arg0, 0x27);
    h->hero_idle_count = Random(0x96) + 0xB4;
    s = h->hero_sub;
    s = s->next;
    if (s != NULL) {
        s = s->next;
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
}

void N2OHeroHit(s32 arg0) {
    s32 r;
    u16 st;
    PadState *pad;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x2C);
        r = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x2D);
        r = 1;
    } else {
        r = 0;
    }
    if (r == 0) {
        PadState *base = (PadState *)Pads;
        pad = &base[arg0];
        if (pad->dirPressed & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->lastDir &= ~0x8000;
            } else {
                SetHeroAction(arg0, 0x2B);
            }
        }
        ProcessHeroAnim(arg0);
    }
}

void N2OHeroTaunt(s32 arg0) {
    s32 ret;
    u16 st;
    PadState *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        PadState *base = (PadState *)Pads;
        pad = &base[arg0];
        if (pad->dirPressed & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->lastDir &= ~0x8000;
            } else {
                SetHeroAction(arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed != 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction(arg0, 0x28);
        }
        ProcessHeroAnim(arg0);
    }
}

void N2OHeroMove(s32 arg0) {
    s32 ret;
    u16 st;
    PadState *pad;
    HeroInfo *h;
    HeroInfo *hb;
    s32 mode;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x2C);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x2D);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        PadState *base = (PadState *)Pads;
        pad = &base[arg0];
        if (pad->dirPressed & 0x8000) {
            if (Heroes[arg0].hero_attack_timer != 0) {
                pad->lastDir &= ~0x8000;
            } else {
                SetHeroAction(arg0, 0x2B);
            }
        }
        hb = Heroes;
        h = hb + arg0;
        if (h->hero_move->forward_speed == 0 &&
            ((mode = h->hero_mode, (s32)mode < 0x2C) == 0 || (s32)mode < 0x2A)) {
            SetHeroAction(arg0, 0x27);
        }
        if (N2OHeroUpdateSpeed(arg0) != 0) {
            ProcessHeroAnim(arg0);
        }
    }
}

void SetN2OHeroWinner(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    AddCamera(h->hero);
    h->hero_sub->XYZ.y = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    RequestHeroAnim(arg0, 0);
    StartHeroVictory(arg0);
    GameAdventure.creditsDone = 0;
}

void N2OHeroWinner(s32 arg0) {
    EXTengine *e;
    s32 flag;
    s32 t;

    e = Heroes[arg0].hero_eng;
    t = e->XYZ.z;
    flag = 0;
    if (t < -0x500) {
        e->XYZ.z = t + 0x20;
    } else {
        flag = 1;
    }
    if (GameTeams[0] != 0) {
        if (arg0 == 0) {
            e = Heroes[0].hero_eng;
            t = e->XYZ.x;
            if (t >= -0x27F) {
                e->XYZ.x = t - 0x40;
            }
        } else {
            e = Heroes[arg0].hero_eng;
            t = e->XYZ.x;
            if (t < 0x280) {
                e->XYZ.x = t + 0x40;
            }
        }
    }
    if (flag != 0) {
        CameraMode = 1;
    }
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        FinishLevel();
    }
    ProcessHeroAnim(arg0);
}

void SetN2OHeroDie(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 5;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    RemoveCamera(h->hero);
    h->hero_sub->XYZ.y = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x29);
    h->hero_hit = 0x3C;
}

void SetN2OHeroKick(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    N2OHeroSetSpeed(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x25);
    h->hero_attack_delay += 0x1E;
    if (h->hero_attack_delay > 0x5A) {
        h->hero_attack_delay = 0x5A;
    }
    h->hero_attack_counter = 0;
    h->hero_attack_timer = h->hero_attack_delay;
    PlaySfx(N2OKickSfx[arg0], NULL, NULL, 0x1000, 0x1E00, 0);
}

void N2OHeroKick(s32 arg0) {
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;
    subengine *s;
    subengine *sub;
    u16 cnt;
    s32 scale;
    s32 ang;

    s = 0;
    base = Heroes;
    h = &base[arg0];
    cnt = h->hero_attack_counter;
    sub = h->hero_sub;
    scale = (cnt * 0x600) + 0x1000;
    ang = (cnt << 9) + 0x800;
    sub = sub->next;
    if (sub != 0) {
        s = sub->next;
        if (s != 0) {
            s->entity = 0x2005;
            s->xform.scale.vx = scale & 0xFFFF;
            s->xform.scale.vy = scale & 0xFFFF;
            s->xform.scale.vz = scale & 0xFFFF;
            s->scale = ang;
            s->xform.dirty = 1;
            s->flags |= 0x40008021;
        }
    }
    if (N2OHeroRad2ObjectCollision(arg0, (scale & 0xFFFF) >> 3, 0)) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    base2[arg0].hero_attack_counter++;
    if (base2[arg0].hero_attack_counter >= 4) {
        SetHeroAction(arg0, 0x27);
        if (s != 0) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (N2OHeroUpdateSpeed(arg0) != 0) {
        ProcessHeroAnim(arg0);
    }
}

void SetN2OHeroDead(s32 arg0) {
    SLONG_XYZ sp18;
    EXTengine *temp_v0;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *temp_s0;
    HeroInfo *temp_s0_2;
    _sfx_handler *temp_a0;
    subengine *temp_v1;

    base = Heroes;
    temp_s0 = &base[arg0];
    temp_s0->hero_hit = 0;
    temp_s0->hero_immunity = 0;
    temp_s0->hero_move->velocity_enable = 0;
    temp_s0->hero_move->jump_max = 0;
    temp_s0->hero_health = 0;
    temp_s0->hero_frozen = 2;
    temp_s0->hero_powerup->flags = 0;
    temp_s0->hero_fx->flags = 0;
    temp_s0->hero_pu = 0;
    temp_s0->hero_pu_timer = 0;
    temp_s0->hero_anim->sub = NULL;
    temp_a0 = temp_s0->hero_powerup->sfx;
    if (temp_a0 != NULL) {
        StopSfx(temp_a0);
    }
    temp_s0->hero_status |= 0x2000;
    temp_v1 = temp_s0->hero_sub;
    temp_v1 = temp_v1->next;
    if (temp_v1 != NULL) {
        temp_v1->flags = 0;
        temp_v1 = temp_v1->next;
        if (temp_v1 != NULL) {
            temp_v1->flags = 0;
        }
    }
    base2 = Heroes;
    temp_s0_2 = &base2[arg0];
    temp_v0 = temp_s0_2->hero_eng;
    sp18 = temp_v0->XYZ;
    SpawnModelFx(0, (s32 *)&sp18, &HeroModelBank[DB_PlayerVehicle]);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->XYZ, 0x1000, 0x1E00, 0);
    HeroBecomeAngel(arg0);
}

void N2OHeroDead(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x2C);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[*(volatile u16 *)&h->hero_name]];
        } else {
            h->hero_sub->u6C.dbase = NULL;
        }
    } else {
        HeroGhost(arg0);
    }
}

void N2OBallMaster(void) {
    s32 n;
    N2ONode *p;

    if (GameExitFlags & 0x801) {
        return;
    }
    if (N2OBallDelay != 0) {
        N2OBallDelay--;
        return;
    }
    N2OBallClock++;
    n = (u16)((u16)N2OBallClock / 240);
    n++;
    if (n >= 5) {
        n = 4;
    }
    p = HeadOfBalls;
    N2OBallCount = 0;
    if (p != NULL) {
        do {
            p = p->next;
            N2OBallCount++;
        } while (p != NULL);
    }
    if ((u16)N2OBallCount < n) {
        N2OFreeBallSlot();
        N2OBallDelay = 30;
    }
}

void InitN2OBall(NAS *nas) {
    EXTengine *eng;
    EXTengine *e1;
    EXTobject *obj;
    EXTmotion *mot;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->flags |= 0x02000000;
        eng->flags &= 0xFFFF7FFF;
        obj = (EXTobject *)AddObject(nas);
        if (obj != NULL) {
            mot = AddMotion(nas);
            if (mot != NULL) {
                LinkObject(nas);
                AddSolid(nas, 0x300);
                nas->FUX = (EXT *)obj;
                obj->whatami = 1;
                obj->owner = 0x63;
                obj->status = 0;
                mot->Mass = 0x100;
                mot->Za = 0;
                mot->Zvel = 0;
                mot->Zmax = 0;
                mot->Zacc = 3;
                mot->Zdec = 3;
            }
        }
        eng->xform.order = 2;
        eng->sub->u6C.dbase = &HeroModelBank[DB_BallsEffects];
        eng->sub->entity = 0x2009;
        AddToGroup((NAS **)&HeadOfBalls, nas);
        e1 = AddExtEngine(nas, 1);
        if (e1 != NULL) {
            sub = e1->sub;
            sub->u6C.dbase = &HeroModelBank[DB_BallsEffects];
            sub->entity = 0x200B;
            e1->flags |= 0x22000021;
            sub->flags |= 0x22000021;
            e1->flags &= 0xFFFF7FFF;
        }
    }
}


void PlotN2OBall(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    EXTengine *e2;
    s32 skill;
    s32 done;
    s32 idx;
    s32 ang;
    s16 d;
    s16 a;
    s16 k;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 px;
    s32 pz;

    skill = AiPlayerSkill[3].level + (u16)N2OBallsLost;
    if (skill > 0x20) {
        skill = 0x20;
    }
    eng = nas->Engine;
    done = 0;
    if (eng == NULL) {
        return;
    }
    obj = (EXTobject *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    mot = FindExtension(nas, 0x800);
    if (mot != NULL) {
        if (mot->Xvel < mot->Xmax) {
            mot->Xvel += mot->Xacc;
            if (mot->Xvel > mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
        }
        if (mot->Xvel > mot->Xmax) {
            mot->Xvel -= mot->Xdec;
            if (mot->Xvel < mot->Xmax) {
                mot->Xvel = mot->Xmax;
            }
        }
        eng->XYZ.x -= (mot->Xvel * sin_cos_table[(mot->Xa - 0x400) & 0xFFF].unk2) >> 12;
        eng->XYZ.z -= (mot->Xvel * sin_cos_table[mot->Xa & 0xFFF].unk2) >> 12;
        if (obj->timer != 0) {
            obj->timer--;
        }
        switch (obj->status) {
        case 0:
            if ((((GameState *)GameInfo)->pause & 0x04000000) ||
                (((GameState *)GameInfo)->exitFlags & 0x1000)) {
                mot->Xvel = 0;
                mot->Xacc = 0;
                mot->Xdec = 0;
                eng->XYZ.x = 0x2000;
                eng->XYZ.y = 0x2000;
                eng->XYZ.z = 0x2000;
                eng->flags &= ~0x8000;
                break;
            }
            idx = Random(2);
            if (N2OSlotRecs[idx].f16 != 0) {
                break;
            }
            eng->XYZ = N2OBallStarts[idx];
            N2OSlotRecs[idx].f16 = 0x1F;
            obj->countdown = 0xF;
            obj->status = 12;
            if (N2OBossAimDelay != 0) {
                N2OBossAimDelay--;
                break;
            }
            N2OBossAimDelay = 0x26 - skill;
            eng->sub->entity = 0x2018;
            obj->whatami = 2;
            break;
        case 12:
            if (obj->countdown != 0) {
                obj->countdown--;
                break;
            }
            e2 = eng->Engine;
            eng->flags |= 0x8000;
            if (e2 != NULL) {
                e2->flags |= 0x8000;
            }
            px = Heroes[3].hero_eng->XYZ.x;
            pz = Heroes[3].hero_eng->XYZ.z;
            dx = eng->XYZ.x - px;
            dz = eng->XYZ.z - pz;
            ang = get_angle(dx, dz);
            mot->Xa = ang;
            mot->Za = ang & 0xFFF;
            mot->Xacc = 4;
            mot->Xdec = 1;
            mot->Zvel = 0;
            obj->timer = 0x14;
            PlaySfx(0x5A0, &eng->sfx, &eng->XYZ, 0x1000, 0x1E00, 0);
            eng->XYZ.y = World_floor_level - 0x100;
            mot->Yvel = 0x30;
            mot->Yacc = 0;
            mot->Ydec = 6;
            mot->Ymax = 0x60;
            mot->Ydir = 0xFF;
            mot->Xvel = 0x30;
            obj->status = 6;
            mot->Xmax = 0x50;
            obj->player = -1;
            obj->owner = -1;
            break;
        case 6:
            if (((s32 (*)(EXTengine *, EXTobject *))N2O_BA_TestLaunchBounds)(eng, obj) == 0) {
                mot->Xa = mot->Za;
                mot->Za = 0;
                obj->status = 8;
            }
        case 8:
            mot->Yvel += mot->Yacc - mot->Ydec;
            if (mot->Yvel < 0) {
                mot->Yvel = 0;
            }
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            if (mot->Ydir != 0) {
                eng->XYZ.y -= mot->Yvel;
            } else {
                eng->XYZ.y += mot->Yvel;
            }
            if (mot->Yvel == 0) {
                mot->Yacc = mot->Ydec;
                mot->Ydec = 0;
                mot->Ydir = 0;
            }
            if (mot->Ydir != 0) {
                break;
            }
            eng->XYZ.y += 0x60;
            if (ApplyContourMoved(eng) != 0) {
                obj->can_do |= 0x400;
                if (nas->UserInfo.flags == 0) {
                    PlaySfx(N2OBallBounceSfx[Random(4)], &eng->sfx, &eng->XYZ, 0x1000, 0x1E00, 0);
                    nas->UserInfo.flags |= 1;
                }
                mot->Yvel = mot->Yvel >> 2;
                mot->Ydec = mot->Yacc;
                mot->Yacc = 0;
                mot->Ydir = 0xFF;
                if (mot->Yvel < 8) {
                    obj->status = 1;
                    obj->can_do |= 0x400;
                }
            }
            if (obj->can_do & 0x400) {
                d = (s16)N2O_BA_BounceOnBounds((EXTengine *)eng, mot->Xa);
                if (d != -1) {
                    a = mot->Xa;
                    if (angle_diff_abs(d, a) < 0x200) {
                        a += 0x800;
                    }
                    k = (s16)angle_diff(a & 0xFFF, d);
                    if (abs(k) < 8) {
                        k = Random(0x10) + 8;
                    }
                    mot->Xa = (d + k) & 0xFFF;
                    mot->Zmax = 0;
                    PlaySfx(N2OBallSfx[Random(6)], NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                }
                if (N2OBallOut(eng, obj, 0)) {
                    obj->countdown = 4;
                    break;
                }
            }
            eng->XYZ.y -= 0x60;
            break;
        case 1:
            d = (s16)N2O_BA_BounceOnBounds((EXTengine *)eng, mot->Xa);
            if (d != -1) {
                a = mot->Xa;
                if (angle_diff_abs(d, a) > 0x200) {
                    a += 0x800;
                }
                k = (s16)angle_diff(a & 0xFFF, d);
                if (abs(k) < 8) {
                    k = Random(0x10) + 8;
                }
                mot->Xa = (d + k) & 0xFFF;
                mot->Zmax = 0;
                PlaySfx(N2OBallSfx[Random(6)], NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            }
            if (N2OBallOut(eng, obj, 0)) {
                obj->countdown = 4;
                obj->status = 10;
            }
            N2OBlendDirection((EXTmotion *)mot);
            eng->XYZ.y = eng->XYZ.y - ((s32 (*)(EXTengine *))ApplyContour)(eng) - 0x60;
            break;
        case 0x11:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->scale = 0x400;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->scale = 0x400;
                    }
                }
            } else {
                eng->sub->scale += 0x200;
                e2 = eng->Engine;
                if (e2 != NULL) {
                    e2->sub->scale += 0x200;
                    if ((s16)e2->sub->scale >= 0x1000) {
                        e2->sub->scale = 0xFFF;
                        done = 1;
                    }
                }
            }
            if (N2OBallOut(eng, obj, 1) || done != 0) {
                obj->status = 0x12;
            }
            eng->XYZ.y = eng->XYZ.y - ((s32 (*)(EXTengine *))ApplyContour)(eng) - 0x60;
            break;
        case 4:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->scale = 0x400;
                    eng->sub->otz = 0x100;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->scale = 0x400;
                    }
                }
            } else {
                eng->sub->scale += 0x200;
                e2 = eng->Engine;
                if (e2 != NULL) {
                    e2->sub->scale += 0x200;
                    if ((s16)e2->sub->scale >= 0x1000) {
                        e2->sub->scale = 0xFFF;
                        done = 1;
                    }
                }
            }
            mot->Xvel >>= 1;
            mot->Xmax >>= 1;
            mot->Yvel += mot->Yacc - mot->Ydec;
            if (mot->Yvel < 0) {
                mot->Yvel = 0;
            }
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            eng->XYZ.y += mot->Yvel;
            if (N2OBallOut(eng, obj, 1) || done != 0) {
                obj->status = 0x12;
            }
            break;
        case 0x12:
            obj->can_do &= 0xFF7F;
            if (obj->whatami == 2) {
                goto explode;
            }
            if (obj->player != 0) {
                if (obj->player != 3) {
                    goto explode;
                }
                SpawnModelFx(0, &eng->XYZ.x, 0);
                PlaySfx(HitSfx.hit[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
                BossState.health -= 0x20;
                N2OBallsLost++;
                if (BossState.health <= 0) {
                    BossState.health = 0;
                    Heroes[3].hero_attack_timer = 0x1E;
                    BossState.flags |= 0x2000;
                }
            } else {
                ((void (*)(s32, s32, s32, s32, s32))N2OSpawnExplosion)(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, -1, 0);
                if (!(GameCheats & 1) && !(Heroes[obj->player].hero_status & 0xFA00)) {
                    Heroes[0].hero_health -= 0x10;
                    if (Heroes[0].hero_health < 0) {
                        Heroes[0].hero_health = 0;
                    }
                    Heroes[0].hero_hit++;
                    Heroes[1].hero_health -= 0x10;
                    if (Heroes[1].hero_health < 0) {
                        Heroes[1].hero_health = 0;
                    }
                    Heroes[1].hero_hit++;
                }
            }
            KillNAS(nas);
            return;
        case 10:
            obj->can_do &= 0xFF7F;
            if (obj->whatami == 2) {
                goto explode;
            }
            if (obj->player != 0) {
                if (obj->player == 3) {
                    SpawnModelFx(0, (s32 *)&eng->XYZ, 0);
                    PlaySfx(N2OHitSfx[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
                    BossState.health -= 0x20;
                    if (BossState.health <= 0) {
                        BossState.health = 0;
                        Heroes[3].hero_attack_timer = 0x1E;
                        BossState.flags |= 0x2000;
                    }
                }
            } else {
                ((void (*)(s32, s32, s32, s32, s32))N2OSpawnExplosion)(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, -1, 0);
                if (!(GameCheats & 1) && obj->player == 0) {
                    if (!(Heroes[0].hero_status & 0xFA00)) {
                        Heroes[0].hero_health -= 0x10;
                        if (Heroes[0].hero_health < 0) {
                            Heroes[0].hero_health = 0;
                        }
                        Heroes[0].hero_hit++;
                    }
                    if (!(Heroes[1].hero_status & 0xFA00)) {
                        Heroes[1].hero_health -= 0x10;
                        if (Heroes[1].hero_health < 0) {
                            Heroes[1].hero_health = 0;
                        }
                        Heroes[1].hero_hit++;
                    }
                }
            }
            KillNAS(nas);
            return;
        }
        mot->Zvel = 0;
    }
    dx = eng->XYZ.x - eng->OldXYZ.x;
    dz = eng->XYZ.z - eng->OldXYZ.z;
    eng->xform.rot.vy += (((get_angle(dx, dz) & 0xFFF) - eng->xform.rot.vy) << 20) >> 21;
    dist = int_sqrt(dx * dx + dz * dz);
    eng->xform.dirty = 1;
    eng->xform.rot.vx -= dist >> 5;
    if (obj->will_do & 0x100) {
    explode:
        ((void (*)(s32, s32, s32, s32, s32))N2OSpawnExplosion)(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, -1, 0);
        KillNAS(nas);
        return;
    }
    switch (obj->status) {
    case 4:
    case 2:
    case 0x12:
    case 0x63:
        break;
    default:
        TestN2OObjectObstacles(eng, (N2OFuxF *)obj);
        break;
    }
    e2 = eng->Engine;
    if (e2 != NULL) {
        e2->XYZ = eng->XYZ;
        ((N2OHook *)&LevelSpecs.GetContour)->fn(eng);
    }
}

void N2OBlendDirection(EXTmotion *p) {
    s32 x1;
    s32 z1;
    s16 vel;
    s32 speed;

    speed = p->Zvel;
    if (speed != 0) {
        u32 da = (u16)p->Xa;  /* matching: the angles read unsigned (N2OBlend had them u16) */
        u32 db = (u16)p->Za;
        s32 a, b, c, d;
        vel = p->Xvel;
        a = vel * sin_cos_table[(da - 0x400) & 0xFFF].unk2;
        b = vel * sin_cos_table[da & 0xFFF].unk2;
        c = speed * sin_cos_table[(db - 0x400) & 0xFFF].unk2;
        d = speed * sin_cos_table[db & 0xFFF].unk2;
        x1 = a >> 12;
        z1 = b >> 12;
        x1 += c >> 12;
        z1 += d >> 12;
        p->Xa = get_angle(x1, z1) & 0xFFF;
    }
}

void N2OInitBoss(void) {
    EXTengine *eng;
    subengine *sub;
    subengine *sub2;
    EXTheromove *mv;
    u8 *anim;
    s32 *pos;
    s32 i;
    s32 ang;
    s32 t;

    InitBossHealthIcons();
    AiSkillTable = GameTeams[0] != 0 ? N2OBossAiSkillAlt : N2OBossAiSkill;
    for (i = 0; i < 4; i++) {
        if (OVT->levelFlags & 0x100) {
            t = OVT->adventure.modeSkill[OVT->mode];
            OVT->adventure.aiSkill[i] = t;
            AiPlayerSkill[i].level = ((u16 *)AiSkillTable)[t];
            AiPlayerSkill[i].quality = 0x10;
        }
    }
    ResetHeroGlobals();
    sub = Heroes[3].hero_sub;
    eng = Heroes[3].hero_eng;
    pos = &eng->XYZ.x;
    *pos = N2OHeroStarts[3][0];
    eng->XYZ.y = World_floor_level;
    eng->XYZ.z = *(s32 *)((u8 *)N2OHeroStarts + 0x2C);  /* matching: N2OHeroStarts[3][2], an array read, can't be scheduled above the store before it */
    ang = get_angle(Heroes[3].hero_eng->XYZ.x, Heroes[3].hero_eng->XYZ.z) + 0x7F;
    ang &= 0xC00;
    mv = Heroes[3].hero_move;
    eng->xform.rot.vy = ang;
    mv->facing_angle = ang;
    mv->forward_angle = ang;
    eng->xform.order = 0;
    anim = &HeroModelBank[DB_Oxide];
    *(u8 **)&sub->u6C.dbase = anim;
    N2OBossAnimTimer = 0xF;
    sub->entity = 0x2006;
    sub->XYZ.z = 0;
    sub->XYZ.y = 0;
    sub->XYZ.x = 0;
    sub->flags |= 0x8000;
    FetchDefaultColBox(eng, 0x50);
    N2OHeroEngineUpdate(3);
    if (eng->XYZ.x > 0x880) {
        eng->XYZ.x = 0x880;
    }
    if (eng->XYZ.x < -0x880) {
        eng->XYZ.x = -0x880;
    }
    if (eng->XYZ.z > 0x880) {
        eng->XYZ.z = 0x880;
    }
    if (eng->XYZ.z < -0x880) {
        eng->XYZ.z = -0x880;
    }
    N2OBossClock = 0;
    N2OBossStepTimer = 0;
    N2OBossRound.eng = eng;
    N2OBossRound.unk8 = sub;
    N2OBossRound.unk0 = Heroes[3].hero;
    sub = sub->next;
    N2OBossStep = -1;
    if (sub != NULL) {
        sub->u6C.dbase = anim;
        sub->entity = 0x4000;
        sub->XYZ.z = 0;
        sub->XYZ.y = 0;
        sub->XYZ.x = 0;
        sub->flags |= 0x8000;
    }
    sub2 = sub->next;
    if (sub2 != NULL) {
        sub2->u6C.dbase = anim + 0x38;
        sub2->entity = 0x2005;
        sub2->xform.hasScale = 1;
        sub2->xform.order = 0;
        sub2->flags &= ~0x8000;
        sub->XYZ.z = 0;
        sub->XYZ.y = 0;
        sub2->XYZ.x = 0;
    }
    BossState.health = 0xFF;
    CreateNAS(4, World_floor_level - 0x20, 0, 0x1B05, 0x11);
    CreateNAS(5, World_floor_level - 0x20, 0, 0x1B05, 0x11);
    CameraMode = 8;
}

void N2OUpdateBoss(void) {
    EXTengine *eng;
    N2OStep *st;
    subengine *sub;
    s32 lim;
    s32 step;
    s32 d;
    s32 v;
    u16 k;
    u32 sc;
    N2OPan *p;
    u16 fl;
    u16 n;
    s16 i;
    s32 ang;
    N2OStep *tbl;
    subengine *hs;
    s32 sg;
    s32 nx;
    s32 dx;
    s32 *q;
    s32 tm;

    if (BossState.flags & 0x8000) {
        return;
    }
    if (Heroes[3].hero_attack_timer != 0) {
        Heroes[3].hero_attack_timer--;
    }
    eng = Heroes[3].hero_eng;
    eng->flags |= 0x8000;
    if (N2OBossStepTimer != 0) {
        N2OBossStepTimer--;
    } else {
        tbl = N2OBossSteps;
        nx = (u16)N2OBossStep + 1;
        ((N2OHalf *)&N2OBossStep)->v = nx;
        nx <<= 16;
        nx >>= 16;
        st = &tbl[nx];
        N2OBossStepKind = st->kind;
        if ((u16)N2OBossStepKind == 99) {
            N2OBossStep = st->time;
            N2OBossStepKind = tbl[N2OBossStep].kind;
        }
        k = tbl[N2OBossStep].slot;
        N2OBossStepFlag = k;
        N2OBossStepTimer = tbl[N2OBossStep].time;
        if (k != 0) {
            N2OSlots[0].kind = k;
            N2OSlots[2].kind = k;
        }
    }
    N2OBossPad = 0;
    if (BossState.flags & 0x2000) {
        N2OBossStepFlag = 1;
        N2OSlots[0].kind = 1;
        N2OSlots[2].kind = 1;
        N2OBossStepKind = 0;
        if (Heroes[3].hero_attack_timer == 0) {
            Heroes[3].hero_eng->flags = 0;
            PlaySfx(0x586, NULL, &Heroes[3].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            SpawnModelFx(0, &eng->XYZ.x, &HeroModelBank[DB_BallsEffects]);
            BossState.flags |= 0x8000;
        }
    } else {
        N2O_HeroBot((u16 *)&N2OBossPad);
        if (GameExitFlags & 0x801) {
            N2OBossPad = 0;
        }
        lim = 0x68;
        p = (N2OPan *)Heroes[3].hero_move;
        if (p != NULL) {
            fl = N2OBossPad;
            step = 0x22;
            if (fl & 0xF00) {
                lim = 0xA0;
                step = 0x38;
            }
            if (fl & 0xA0) {
                d = 0;
                if (fl & 0x80) {
                    d = -step;
                }
                if (fl & 0x20) {
                    d += step;
                }
                p->flag += d;
                if (p->flag > lim) {
                    p->flag = lim;
                }
                if (p->flag < -lim) {
                    p->flag = -lim;
                }
                if (p->flag == 0 && d != 0) {
                    sg = 1;
                    if (d < 0) {
                        sg = -1;
                    }
                    p->flag = sg;
                }
                N2OHeroSpeed[3] = p->flag;
            } else {
                N2OHeroSpeed[3] = 0;
                if (p->flag > 0) {
                    p->flag -= 0x12;
                    if (p->flag < 0) {
                        p->flag = 0;
                    }
                }
                if (p->flag < 0) {
                    p->flag += 0x12;
                    if (p->flag > 0) {
                        p->flag = 0;
                    }
                }
            }
            if ((u16)N2OBossPad & 0x8000) {
                Heroes[3].hero_attack_timer = 10;
                Heroes[3].hero_attack_counter = 0;
                PlaySfx(N2OBossShotSfx, NULL, NULL, 0x1000, 0x1E00, 0);
                N2OBossShooting = 1;
            }
        }
        sub = NULL;
        if (N2OBossShooting != 0) {
            n = Heroes[3].hero_attack_counter;
            hs = Heroes[3].hero_sub;
            sc = n * 0x600 + 0x1000;
            ang = (n << 9) + 0x800;
            hs = hs->next;
            if (hs != NULL) {
                sub = hs->next;
                if (sub != NULL) {
                    sub->entity = 0x2005;
                    sub->xform.scale.vx = sc & 0xFFFF;
                    sub->xform.scale.vy = sc & 0xFFFF;
                    sub->xform.scale.vz = sc & 0xFFFF;
                    sub->scale = ang;
                    sub->xform.dirty = 1;
                    sub->flags |= 0x40008021;
                }
            }
            if (N2OHeroRad2ObjectCollision(3, (sc & 0xFFFF) >> 3, 0) != 0) {
                Heroes[3].hero_attack_timer = 0;
            }
            Heroes[3].hero_attack_counter++;
            if (Heroes[3].hero_attack_counter >= 4) {
                N2OBossShooting = 0;
                if (sub != NULL) {
                    sub->flags &= ~0x8000;
                }
            }
        }
        N2O_KeepInBounds((EXTengine *)eng);
        tm = N2OBossClock;
        q = &World_floor_level;
        v = (sin_cos_table[(((tm << 7) & 0xFFF) - 0x400) & 0xFFF].unk2 * 15 >> 11) - 0x80;
        eng->XYZ.y = *q + v;
        dx = ((N2OPanRef *)&Heroes[3].hero_move)->p->flag;
        eng->XYZ.x += dx;
        v = angle_diff(eng->xform.rot.vz, -dx);
        eng->xform.dirty = 1;
        eng->xform.rot.vz += v >> 2;
    }
    if (GameExitFlags & 0x801) {
        return;
    }
    switch ((u16)N2OBossStepKind) {
    case 0:
        break;
    case 10:
        if (N2OSlots[0].nas == NULL) {
            N2OSpawnSlotMissile(0);
        }
        if (N2OSlots[2].nas == NULL) {
            N2OSpawnSlotMissile(2);
        }
        break;
    case 3:
        n = N2OMissileDelay;
        if (n != 0) {
            N2OMissileDelay = n - 1;
        } else {
            N2OSpawnSlotMissile(((u16)N2OMissileCount * 2) & 2);
            N2OMissileDelay = 15;
            N2OMissileCount++;
        }
        break;
    case 4:
        n = N2OMissileDelay;
        if (n != 0) {
            N2OMissileDelay = n - 1;
        } else {
            N2OSpawnSlotMissile(0);
            N2OSpawnSlotMissile(2);
            N2OMissileDelay = 30;
            N2OMissileCount++;
        }
        break;
    }
    N2OBossClock++;
    ((s32 (*)(s32))N2OHeroBallCollision)(3);
}

s32 N2OBotAimA(s32 arg0, N2OPos *from, N2OPos *to, s32 angle) {
    s32 i;
    s32 n;
    u16 ang;
    s16 x0;
    s16 z0;
    s16 x1;
    s16 z1;
    s32 dz;
    s32 ox;
    s32 oz;
    s32 dx;
    s32 ez;
    s32 ex;
    s32 num;
    s32 den;
    s32 num2;
    s32 t;

    if (arg0 == 0) {
        i = 0;
    } else if (arg0 == 1) {
        i = 0;
    } else {
        i = 3;
    }
    n = AiPlayerSkill[arg0].level + (u16)N2OBallsLost;
    x0 = N2OAimEdgesA[i].x0;
    z0 = N2OAimEdgesA[i].z0;
    x1 = N2OAimEdgesA[i].x1;
    z1 = N2OAimEdgesA[i].z1;
    if (n > 0x20) {
        n = 0x20;
    }
    n = 0x20 - n;
    n -= N2OSkillBias;
    if (n < 0) {
        n = 0;
    }
    n = N2OAimAngles[n];
    /* The empty loops raise angle's ref count (flow.c weights refs by loop depth) so it gets s1. */
    do {
        do {
            do {
                do {
                    ang = angle + (Random(n) - n);
                } while (0);
            } while (0);
        } while (0);
    } while (0);
    to->x = from->x - sin_cos_table[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - sin_cos_table[ang & 0xFFF].unk2;
    if (x0 < from->x && x1 < from->x && x0 < to->x && x1 < to->x) {
        return 0;
    }
    if (from->x < x0 && from->x < x1 && to->x < x0 && to->x < x1) {
        return 0;
    }
    if (z0 < from->z && z1 < from->z && z0 < to->z && z1 < to->z) {
        return 0;
    }
    if (from->z < z0 && from->z < z1 && to->z < z0 && to->z < z1) {
        return 0;
    }
    dz = (from->z - to->z) >> 4;
    ox = (x0 - from->x) >> 4;
    oz = (z0 - from->z) >> 4;
    dx = (from->x - to->x) >> 4;
    ez = (z1 - z0) >> 4;
    ex = (x1 - x0) >> 4;
    num = dz * ox - dx * oz;
    den = ez * dx - ex * dz;
    if (den > 0) {
        if (den < num || num < 0) {
            return 0;
        }
    } else {
        if (num < den || num > 0) {
            return 0;
        }
    }
    num2 = ex * oz - ez * ox;
    if (den > 0) {
        if (den < num2 || num2 < 0) {
            return 0;
        }
    } else {
        if (num2 < den || num2 > 0) {
            return 0;
        }
    }
    if (den == 0) {
        return 0;
    }
    t = (num << 8) / den;
    to->x = x0 + ((x1 - x0) * t >> 8);
    to->z = z0 + ((z1 - z0) * t >> 8);
    return 1;
}

s32 N2OBotAimB(s32 arg0, N2OPos *from, N2OPos *to, s32 angle) {
    s32 i;
    s32 n;
    u16 ang;
    s16 x0;
    s16 z0;
    s16 x1;
    s16 z1;
    s32 dz;
    s32 ox;
    s32 oz;
    s32 dx;
    s32 ez;
    s32 ex;
    s32 num;
    s32 den;
    s32 num2;
    s32 t;

    if (arg0 == 0) {
        i = 0;
    } else if (arg0 == 1) {
        i = 0;
    } else {
        i = 3;
    }
    n = AiPlayerSkill[arg0].level + (u16)N2OBallsLost;
    x0 = N2OAimEdgesB[i].x0;
    z0 = N2OAimEdgesB[i].z0;
    x1 = N2OAimEdgesB[i].x1;
    z1 = N2OAimEdgesB[i].z1;
    if (n > 0x20) {
        n = 0x20;
    }
    n = 0x20 - n;
    n -= N2OSkillBias;
    if (n < 0) {
        n = 0;
    }
    n = N2OAimAngles[n];
    /* The empty loops raise angle's ref count (flow.c weights refs by loop depth) so it gets s1. */
    do {
        do {
            do {
                do {
                    ang = angle + (Random(n) - n);
                } while (0);
            } while (0);
        } while (0);
    } while (0);
    to->x = from->x - sin_cos_table[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - sin_cos_table[ang & 0xFFF].unk2;
    if (x0 < from->x && x1 < from->x && x0 < to->x && x1 < to->x) {
        return 0;
    }
    if (from->x < x0 && from->x < x1 && to->x < x0 && to->x < x1) {
        return 0;
    }
    if (z0 < from->z && z1 < from->z && z0 < to->z && z1 < to->z) {
        return 0;
    }
    if (from->z < z0 && from->z < z1 && to->z < z0 && to->z < z1) {
        return 0;
    }
    dz = (from->z - to->z) >> 4;
    ox = (x0 - from->x) >> 4;
    oz = (z0 - from->z) >> 4;
    dx = (from->x - to->x) >> 4;
    ez = (z1 - z0) >> 4;
    ex = (x1 - x0) >> 4;
    num = dz * ox - dx * oz;
    den = ez * dx - ex * dz;
    if (den > 0) {
        if (den < num || num < 0) {
            return 0;
        }
    } else {
        if (num < den || num > 0) {
            return 0;
        }
    }
    num2 = ex * oz - ez * ox;
    if (den > 0) {
        if (den < num2 || num2 < 0) {
            return 0;
        }
    } else {
        if (num2 < den || num2 > 0) {
            return 0;
        }
    }
    if (den == 0) {
        return 0;
    }
    t = (num << 8) / den;
    to->x = x0 + ((x1 - x0) * t >> 8);
    to->z = z0 + ((z1 - z0) * t >> 8);
    return 1;
}

s32 PickN2OPlayer(s32 arg0) {
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].quality;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].quality;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 N2OBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, void *arg5) {
    N2OPos buf;
    s32 a;
    EXTengine *t;
    N2ONode *n;
    N2ONode *best;
    HeroInfo *h;
    HeroInfo *h2;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;

    arg2 *= arg2;
    if (Heroes[arg0].hero_attack_timer != 0) {
        goto ret0;
    }
    best = NULL;
    n = HeadOfBalls;
    if (n != NULL) {
        h = &Heroes[arg0];
    loop:
        do {
            t = n->target;
            if (t != NULL) {
                dx = h->hero_eng->XYZ.x - t->XYZ.x;
                dz = h->hero_eng->XYZ.z - t->XYZ.z;
                if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->whatami != 2) {
                    a = get_angle(dx, dz);
                    d = angle_diff(a, h->hero_move->forward_angle);
                    if (arg1 == -1) {
                        if ((s16)arg3 < d && d < (s16)arg4) {
                            best = n;
                        }
                    } else if (FindExtension((NAS *)n, 0x800) != NULL) {
                        r = N2OBotAimB(arg1, (N2OPos *)&t->XYZ.x, &buf, (u16)a);
                        best = n;
                        if (r != 0) {
                            goto found;
                        }
                    }
                }
            }
        } while (0);
        do {
            n = n->next;
        } while (0);
        if (n != NULL) {
            goto loop;
        }
    }
    if (best == NULL) {
        return 0;
    }
    if (arg5 == 0) {
        goto ret0;
    }
    if (GameTeams[0] != 0) {
        HeroInfo *base = Heroes;

        h2 = &base[arg0];
        arg1 = h2->hero_teammate;
        t = best->target;
        a = get_angle(h2->hero_eng->XYZ.x - t->XYZ.x, h2->hero_eng->XYZ.z - t->XYZ.z);
        if (FindExtension((NAS *)best, 0x800) == NULL) {
            goto ret0;
        }
        if ((N2OBotAimB(arg1, (N2OPos *)&t->XYZ.x, &buf, (u16)a) ^ 1) == 0) {
            goto ret0;
        }
    }
found:
    return 1;
ret0:
    return 0;
}

/* For a test run's player: the ball that reaches the hero's end first, or with bomb, the bomb that does. Fills where it'll
   cross his line, with the walls folded in, and where it is now; 0 if nothing's on its way. */
s32 N2ONextBall(s32 bomb, s32 *cross, s32 *bx, s32 *bz) {
    N2ONode *n;
    s32 soonest = 0x7FFFFFFF;
    s32 hz = Heroes[0].hero_eng->XYZ.z;

    for (n = HeadOfBalls; n != NULL; n = n->next) {
        EXTmotion *mot = FindExtension((NAS *)n, 0x800);
        EXTengine *t = n->target;

        if (mot != NULL && t != NULL && n->unk58 != NULL && (n->unk58->status == 1 || n->unk58->status == 8)
            && (n->unk58->whatami == 2) == (bomb != 0)) {
            s32 vx = -sin_cos_table[(mot->Xa - 0x400) & 0xFFF].unk2;
            s32 vz = -sin_cos_table[mot->Xa & 0xFFF].unk2;

            if (vz < -0x40 && t->XYZ.z > hz) {
                s32 run = ((hz - t->XYZ.z) << 8) / vz;
                s32 x = t->XYZ.x + ((vx * run) >> 8);

                while (x > N2O_BALL_WALL || x < -N2O_BALL_WALL) {
                    x = x > 0 ? N2O_BALL_WALL * 2 - x : -N2O_BALL_WALL * 2 - x;
                }
                if (run < soonest) {
                    soonest = run;
                    *cross = x;
                    *bx = t->XYZ.x;
                    *bz = t->XYZ.z;
                }
            }
        }
    }
    return soonest != 0x7FFFFFFF;
}

void N2O_HeroBot(u16 *arg0) {
    N2OPos best;
    N2OPos pos;
    EXTbotai *ai;
    N2ONode *n;
    EXTengine *t;
    void *ext;
    s32 skill;
    s32 dist;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 r;
    s32 tgt;
    s32 k;

    if (GamePause & 0x04000000) {
        return;
    }
    ai = Heroes[3].hero_ai;
    do {
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
        skill = AiPlayerSkill[3].level + (u16)N2OBallsLost;
        if (skill > 0x20) {
            skill = 0x20;
        }
    } while (0);
    if (Heroes[3].hero_mode == (u16)HeroActHit || Heroes[3].hero_mode == (u16)HeroActDead) {
        ai->AIsleep = 5;
        ai->AImode = 0x14;
    }
    n = HeadOfBalls;
    dist = 0x7FFFFF;
    if (ai->TargetPlayer == -1) {
        ai->TargetPlayer = PickN2OPlayer(3);
    }

    for (; n != NULL; n = n->next) {
        t = n->target;
        if (t != NULL && n->unk58 != NULL && n->unk58->status == 1) {
            ext = FindExtension((NAS *)n, 0x800);
            if (ext != NULL && N2OBotAimA(3, (N2OPos *)&t->XYZ.x, &pos, (u16)((EXTmotion *)ext)->Xa) != 0) {
                dx = t->XYZ.x - pos.x;
                dz = t->XYZ.z - pos.z;
                d = dx * dx + dz * dz;
                if (d < dist) {
                    dist = d;
                    best = pos;
                }
            }
        }
    }
    if (dist != 0x7FFFFF) {
        ang = get_angle(best.x - Heroes[3].hero_eng->XYZ.x, best.z - Heroes[3].hero_eng->XYZ.z);
        dist = int_sqrt(dist) >> 6;
    } else {
        dist = 0;
        ang = get_angle(ai->TargetXYZ.x - Heroes[3].hero_eng->XYZ.x, ai->TargetXYZ.z - Heroes[3].hero_eng->XYZ.z);
    }
    switch (ai->AImode) {
    case 3:
        r = skill * 16;
        r -= N2OSkillBias;
        if (r <= 0) {
            r = 1;
        }
        {
            s32 q = Random(r) + 0x100;

            do {
                r += q;
            } while (0);
        }
        tgt = ai->TargetPlayer;
        n = (N2ONode *)0;
        if (tgt != -1) {
            if (ai->AItimer1 == 0) {
                if (skill < (s16)Random(0x20)) {
                    ai->AItimer1 = ((0x20 - skill) >> 1) + 0x20;
                    if (ai->AItimer1 == 0) {
                        ai->AItimer1 = 1;
                    }
                } else {
                    n = (N2ONode *)1;
                }
            }
            if (HeroesAllOut == 0) {
                n = (N2ONode *)1;
            }
            if (N2OBotCanHit(3, tgt, r, -0x200, 0x200, n) != 0) {
                ai->TargetPlayer = -1;
                ai->AIpad |= 0x8000;
            } else if (ai->AItimer1 != 0) {
                ai->AItimer1--;
            }
        }
        if (dist != 0) {
            r = abs(Heroes[3].hero_eng->XYZ.x - best.x) + ((AiPlayerSkill[3].level + N2OSkillBias) << 5);
            if (abs(r) > 0x280) {
                if (dist < r) {
                    ai->AIpad |= 0xF00;
                }
                goto set_ang;
            }
        } else {
            ai->AImode = 1;
        }
        break;
    case 4:
        if (dist != 0) {
            goto abort;
        }
        dx = Heroes[3].hero_eng->XYZ.x - ai->TargetXYZ.x;
        dz = Heroes[3].hero_eng->XYZ.z - ai->TargetXYZ.z;
        d = int_sqrt(dx * dx + dz * dz) >> 6;
        if (d < 0x80) {
            ai->AIsleep = Random(0x3C) + 0x1E;
            ai->AImode = 5;
            break;
        }
    set_ang:
        ai->AIang = ang;
        ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        break;
    case 21:
        if (dist != 0) {
            goto abort;
        }
        if (Heroes[3].hero_mode != 0x1E) {
            ai->AImode = 1;
        }
        break;
    case 5:
        if (dist != 0) {
            goto abort;
        }
        if (ai->AIsleep == 0) {
            ai->AImode = 0x15;
            ai->AIpad |= 0x1000;
        }
        break;
    abort:
        ai->AImode = 3;
        ai->AIaborted_mode = 0;
        break;
    case 20:
        break;
    default:
        ai->TargetXYZ = Heroes[3].hero_eng->XYZ;
        ai->TargetXYZ.x = Random(0x400) - 0x200;
        ai->AImode = 4;
        break;
    }
    *arg0 |= ai->AIpad;
}

void N2OSpawnSlotMissile(s32 a) {
    s16 (*tab)[3] = (s16 (*)[3])N2OPillars;
    s32 arg0 = a & 0xFFFF;
    NAS *n;
    N2OSlot *base;
    s32 x = tab[arg0][0];
    s32 z = tab[arg0][1];
    N2OSlot *s;
    s32 y;

    base = N2OSlots;
    s = &base[arg0];
    y = s->src->XYZ.y;

    if (s->kind != 3) {
        n = s->nas;
        if (n != NULL) {
            return;
        }
        n = CreateNAS(x, y, z, 0x1B03, 0x11);
    } else {
        n = s->nas;
        if (n != NULL) {
            n->UserInfo.flags = 1;
            return;
        }
        n = CreateNAS(x, y, z, 0x1B02, 0x11);
    }
    if (n != NULL) {
        if (n->Failure == 0) {
            s->nas = n;
        }
        n->UserInfo.user1 = arg0;
    }
}

void PlotN2Omissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 top;
    s32 x;
    s32 y;
    s32 z;
    s32 vel;
    s32 ang;
    s32 hit;
    SLONG_XYZ *pos;
    HeroInfo *h;
    s16 hp;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    sub = eng->sub;
    if (sub == NULL) {
        return;
    }
    mot = FindExtension(nas, 0x800);
    if (mot == NULL) {
        return;
    }
    top = World_floor_level - 0xE0;
    if (eng->XYZ.y > top) {
        eng->XYZ.y -= 0x20;
        if (eng->XYZ.y < top) {
            eng->XYZ.y = top;
        }
    } else {
        sub->otz = 0;
        mot->Xvel += mot->Xacc;
        if (mot->Xvel > mot->Xmax) {
            mot->Xvel = mot->Xmax;
        }
    }
    vel = mot->Xvel;
    if (vel < 0) {
        vel = 0;
    }
    if (vel < 0x10) {
        ang = (s16)N2OSlots[nas->UserInfo.user1].f18;
        nas->UserInfo.user2 = ang;
        eng->xform.rot.vy = ang - 0x400;
    }
    if (vel > 0x20 && eng->boomshanka != 0) {
        N2OSlots[nas->UserInfo.user1].nas = NULL;
        PlaySfx(N2OMissileSfx[Random(4)], &eng->sfx, &eng->XYZ, 0x1000, 0x1E00, 0);
        eng->boomshanka = 0;
    }
    ang = nas->UserInfo.user2;
    if (sub->flags != 0) {
        eng->XYZ.x -= (vel * sin_cos_table[(ang - 0x400) & 0xFFF].unk2) >> 12;
        eng->XYZ.z -= (vel * sin_cos_table[ang & 0xFFF].unk2) >> 12;
        x = eng->XYZ.x;
        y = eng->XYZ.y += mot->Yvel;
        z = eng->XYZ.z;
        if (z >= -0x1100) {
            hit = N2OMissileHitTest(x, y, z, 0x20, 0xA);
            if (hit == -1) {
                goto alive;
            }
            pos = &eng->XYZ;
            SpawnModelFx(0, (s32 *)pos, 0);
            PlaySfx(N2OHitSfx[Random(4)], NULL, pos, 0x1000, 0x1E00, 0);
            if (!(OVM->cheats & 1)) {
                if (!(Heroes[hit].hero_status & 0xFA00)) {
                    h = &Heroes[hit];
                    if (OVM->teams != 0) {
                        hp = h->hero_health - 0x28;
                    } else {
                        hp = h->hero_health - 0x18;
                    }
                    h->hero_health = hp;
                    if (hp < 0) {
                        h->hero_health = 0;
                    }
                    h->hero_hit++;
                }
            }
        }
        KillNAS(nas);
        return;
    }
alive:
    eng->xform.dirty = 1;
}

void PlotN2Ofirstmissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 x;
    s32 y;
    s32 z;
    s32 vel;
    s32 ang;
    s32 hit;
    SLONG_XYZ *pos;
    HeroInfo *h;
    s16 hp;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    sub = eng->sub;
    if (sub == NULL) {
        return;
    }
    mot = FindExtension(nas, 0x800);
    if (mot == NULL) {
        return;
    }
    if (nas->UserInfo.flags == 0) {
        eng->XYZ.y = N2OSlots[nas->UserInfo.user1].src->XYZ.y - 0x80;
    } else {
        sub->otz = 0;
        mot->Xvel += mot->Xacc;
        if (mot->Xvel > mot->Xmax) {
            mot->Xvel = mot->Xmax;
        }
    }
    vel = mot->Xvel;
    if (vel < 0) {
        vel = 0;
    }
    if (vel < 0x10) {
        ang = (s16)N2OSlots[nas->UserInfo.user1].f18;
        nas->UserInfo.user2 = ang;
        eng->xform.rot.vy = ang - 0x400;
    }
    if (vel > 0x20 && eng->boomshanka != 0) {
        N2OSlots[nas->UserInfo.user1].nas = NULL;
        PlaySfx(N2OMissileSfx[Random(4)], &eng->sfx, &eng->XYZ, 0x1000, 0x1E00, 0);
        eng->boomshanka = 0;
    }
    ang = nas->UserInfo.user2;
    if (vel != 0) {
        eng->XYZ.x -= (vel * sin_cos_table[(ang - 0x400) & 0xFFF].unk2) >> 12;
        eng->XYZ.z -= (vel * sin_cos_table[ang & 0xFFF].unk2) >> 12;
        x = eng->XYZ.x;
        y = eng->XYZ.y += mot->Yvel;
        z = eng->XYZ.z;
        if (z >= -0x1100) {
            hit = N2OMissileHitTest(x, y, z, 0x20, 0xA);
            if (hit == -1) {
                goto alive;
            }
            pos = &eng->XYZ;
            SpawnModelFx(0, (s32 *)pos, 0);
            PlaySfx(N2OHitSfx[Random(4)], NULL, pos, 0x1000, 0x1E00, 0);
            if (!(OVM->cheats & 1)) {
                if (!(Heroes[hit].hero_status & 0xFA00)) {
                    h = &Heroes[hit];
                    if (OVM->teams != 0) {
                        hp = h->hero_health - 0x28;
                    } else {
                        hp = h->hero_health - 0x18;
                    }
                    h->hero_health = hp;
                    if (hp < 0) {
                        h->hero_health = 0;
                    }
                    h->hero_hit++;
                }
            }
        }
        KillNAS(nas);
        return;
    }
alive:
    eng->xform.dirty = 1;
}

void N2OInitSlots(void) {
    s32 i;
    N2OSlot *s;
    N2OEnt *e;
    s16 *t;
    u32 k;

    for (i = 0; i < 4; i++) {
        N2OSlotRecs[i].ent = NULL;
        N2OSlotRecs[i].f4 = 0;
        N2OSlotRecs[i].f8 = 0;
        N2OSlotRecs[i].f12 = 0;
        N2OSlotRecs[i].fC = 0;
        N2OSlotRecs[i].f12 = 0;
        N2OSlotRecs[i].fC = 0;
        N2OSlotRecs[i].f16 = 0;
        N2OSlotRecs[i].f18 = 0;
        N2OSlotRecs[i].f1A = 0;
        N2OSlotRecs[i].f1C = 0;
        N2OSlotRecs[i].f20 = 0;
        N2OSlotRecs[i].f24 = 0;
        N2OSlots[i].src = NULL;
        N2OSlots[i].unk4 = 0;
        N2OSlots[i].nas = NULL;
        N2OSlots[i].f12 = 0;
        N2OSlots[i].f0C = 0;
        N2OSlots[i].f12 = 0;
        N2OSlots[i].f0C = 0;
        N2OSlots[i].f16 = 0;
        N2OSlots[i].f18 = 0;
        N2OSlots[i].kind = 3;
        N2OSlots[i].f1C = 0;
        N2OSlots[i].f20 = 0;
        N2OSlots[i].f24 = 0;
    }
    e = (N2OEnt *)((u8 *)MapScene->subs);
    N2OPhaseObject = NULL;
    for (; e != NULL; e = e->next) {
        t = (s16 *)model_entry_data(e->key, e->id);
        if (t[1] == 0x63) {
            k = t[0] - 10;
            if (k < 4) {
                N2OSlotRecs[k].ent = e;
                N2OSlotRecs[k].fC = e->id;
            }
            k = t[0] - 10;
            if (k < 4) {
                s = &N2OSlots[k];
                s->src = (void *)e;
                s->f1C = e->y;
                if (!(k & 1)) {
                    e->y += 0x200;
                    s->kind = 0;
                }
                s->f20 = e->y;
            }
            if (t[0] == 0x61) {
                N2OPhaseObject = (u32 *)e;
            }
            if (t[0] == 0x62) {
                e->flags = 0;
            }
        }
    }
}

void N2OUpdateSlots(void) {
    GameState *pad;
    HeroInfo *heroes;
    N2OSlot *s;
    N2ORec *r;
    subengine *o;
    s32 i;
    s32 y;
    s32 p0;
    s32 p1;
    s32 x;
    s32 z;

    i = 0;
    pad = (GameState *)GameInfo;
    heroes = Heroes;
    s = N2OSlots;
    r = N2OSlotRecs;
loop:
    {
        if (r->f16 != 0) {
            r->f16--;
        }
        if (pad->exitFlags & 0x801) {
            if (s->kind != 0) {
                s->kind = 1;
            }
            s->f24 = 1;
        }
        o = s->src;
        if (o != NULL) {
            y = s->kind;
            switch (y) {
            case 2:
                if (s->f24 == 0) {
                    PlaySfx(0x2C3, NULL, NULL, 0x1000, 0x1E00, 0);
                    s->f24 = 1;
                }
                p0 = o->XYZ.y;
                if (s->f1C < p0) {
                    p0 -= 0x10;
                    o->XYZ.y = p0;
                    if (s->f1C < p0) {
                        goto aim;
                    }
                }
                o->XYZ.y = s->f1C;
                s->kind = 3;
                s->f24 = 0;
            case 3:
            aim:
                x = o->XYZ.x;
                z = o->XYZ.z;
                p1 = 1;
                if (pad->teams == 0 || (p0 = heroes[0].hero_status, p0 &= 0xFA00, p0 = p0 != 0, heroes[1].hero_status & 0xFA00)) {
                    p0 = p1 = 0;
                }
                if (!(i & 1)) {
                    if (pad->teams != 0) {
                        if (i != 0) {
                            x -= heroes[(u16)p1].hero_eng->XYZ.x;
                            z -= heroes[(u16)p1].hero_eng->XYZ.z;
                        } else {
                            x -= heroes[(u16)p0].hero_eng->XYZ.x;
                            z -= heroes[(u16)p0].hero_eng->XYZ.z;
                        }
                    } else {
                        x -= heroes[(u16)p0].hero_eng->XYZ.x;
                        z -= heroes[(u16)p0].hero_eng->XYZ.z;
                    }
                } else {
                    x -= heroes[3].hero_eng->XYZ.x;
                    z -= heroes[3].hero_eng->XYZ.z;
                }
                s->f18 = get_angle(x, z);
                break;
            case 1:
                if (s->f24 == 0) {
                    PlaySfx(0x2C3, NULL, NULL, 0x1000, 0x1E00, 0);
                    s->f24 = y;
                }
                if (o->XYZ.y < s->f20) {
                    o->XYZ.y += 0x10;
                } else {
                    o->XYZ.y = s->f20;
                    s->kind = 0;
                    s->f24 = 0;
                }
                break;
            }
            o->xform.order = 2;
            y = angle_diff(o->xform.rot.vy, (s16)s->f18);
            o->xform.dirty = 1;
            o->xform.rot.vy += y >> 1;
        }
        s++;
        i++;
        r++;
    }
    if (i < 4) {
        goto loop;
    }
}

void InitN2ODeadWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    u32 dir;
    s32 off;

    if (GameExitFlags & 0x801) {
        N2OWallsHit &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->XYZ.x;
        eng->flags &= 0xFFFF7FFF;
        if (dir == 3) {
            eng->XYZ.x = -0x600;
            eng->XYZ.z = 0x880;
            eng->xform.rot.vx = 0x38E;
            eng->xform.rot.vy = 0;
            nas->UserInfo.flags = 2;
            N2OWallsHit |= 2;
        }
        if (dir < 2) {
            eng->XYZ.x = -0x600;
            eng->XYZ.z = -0x880;
            eng->xform.rot.vx = 0x200;
            eng->xform.rot.vy = 0;
            nas->UserInfo.flags = 1;
            N2OWallsHit |= 1;
        }
        if (dir == 4) {
            eng->XYZ.x = -0x880;
            eng->XYZ.z = -0x600;
            eng->xform.rot.vy = 0xC00;
            eng->xform.rot.vz = 0;
            nas->UserInfo.flags = dir;
            N2OWallsHit |= 4;
        }
        if (dir == 5) {
            eng->XYZ.x = 0x880;
            eng->XYZ.z = -0x600;
            eng->xform.rot.vy = 0xC00;
            eng->xform.rot.vz = 0;
            nas->UserInfo.flags = 8;
            N2OWallsHit |= 8;
        }
        eng->xform.order = 2;
        eng->xform.dirty = 1;
        if (N2ODeadWallUp == 0) {
            PlaySfx(0x591, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        }
        N2ODeadWallUp = 1;
        eng_add_subs(eng, 5);
        sub = eng->sub;
        off = 0;
        if (sub != 0) {
            do {
                sub->u6C.dbase = &HeroModelBank[DB_BallsEffects];
                sub->entity = 0;
                sub->XYZ.x = off;
                sub->flags |= 0x21;
                sub = sub->next;
                off += 0x200;
            } while (sub != 0);
        }
    }
    nas->UserInfo.user1 = 4;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void N2OLineToCircleTest(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r) {
    SLONG_XYZ start;
    SLONG_XYZ d;
    SLONG_XYZ dir;
    s32 a;
    s32 b;
    s32 c;
    s32 s;
    s32 t1;
    s32 t2;

    arg1 = (arg1 + 0x800) & 0xFFF;
    start = *pos;
    start.x -= (0x200 * sin_cos_table[(arg1 - 0x400) & 0xFFF].unk2) >> 12;
    start.z -= (0x200 * sin_cos_table[arg1].unk2) >> 12;
    dir.x = start.x - pos->x;
    dir.y = 0;
    dir.z = start.z - pos->z;
    if (dir.x == 0) {
        return;
    }
    d.x = target->x - start.x;
    d.y = 0;
    d.z = target->z - start.z;
    a = (dir.x * dir.x + dir.z * dir.z) >> 4;
    b = ((dir.x * d.x + dir.z * d.z) * 2) >> 4;
    c = (d.x * d.x + d.z * d.z - r * r) >> 4;
    s = b * b - a * c * 4;
    if (s < 0) {
        return;
    }
    s = int_sqrt(s) >> 6;
    t1 = ((-b + s) << 8) / (a * 2);
    t2 = ((-b - s) << 8) / (a * 2);
    if (t1 < 0 && t2 < 0) {
        return;
    }
    if (t1 < t2) {
        if (t1 < 0) {
            pos->x = start.x - ((dir.x * t2) >> 8);
            pos->y = start.y - ((dir.y * t2) >> 8);
            pos->z = start.z - ((dir.z * t2) >> 8);
        } else {
            pos->x = start.x - ((dir.x * t1) >> 8);
            pos->y = start.y - ((dir.y * t1) >> 8);
            pos->z = start.z - ((dir.z * t1) >> 8);
        }
    } else if (t2 >= 0) {
        pos->x = start.x - ((dir.x * t2) >> 8);
        pos->y = start.y - ((dir.y * t2) >> 8);
        pos->z = start.z - ((dir.z * t2) >> 8);
    } else {
        pos->x = start.x - ((dir.x * t1) >> 8);
        pos->y = start.y - ((dir.y * t1) >> 8);
        pos->z = start.z - ((dir.z * t1) >> 8);
    }
}

s32 N2OHeroPushTeammate(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *t;
    EXTengine *eng;
    EXTengine *teng;
    s32 mate;
    s32 dx;
    s32 dz;
    s32 r;

    base = Heroes;
    h = &base[arg0];
    if (!(h->hero_status & 0xFA00) && GameTeams[0] != 0) {
        mate = h->hero_teammate;
        t = &base[mate];
        if (!(t->hero_status & 0xFA00)) {
            eng = h->hero_eng;
            if (eng != NULL && (r = h->hero_move->forward_speed) != 0) {
                teng = t->hero_eng;
                dx = eng->XYZ.x - teng->XYZ.x;
                dz = eng->XYZ.z - teng->XYZ.z;
                if (eng->collision_box.flags & 0x40) {
                    r = eng->collision_box.radius;
                } else {
                    r = 0x100;
                }
                r *= 2;
                r += 0x90;
                if (dx * dx + dz * dz < r * r) {
                    eng->XYZ.x = Heroes[mate].hero_eng->XYZ.x - ((r * sin_cos_table[(get_angle(-dx, -dz) - 0x400) & 0xFFF].unk2) >> 12);
                    return 1;
                }
                return 0;
            }
        }
    }
    return 0;
}

s32 N2OHeroBallCollision(s32 arg0, EXTengine *unused) {
    SLONG_XYZ tgt;
    SLONG_XYZ nearp;
    SLONG_XYZ farp;
    VECTOR pos;
    EXTengine *me;
    s32 speed;
    s32 top;
    s32 bot;
    s32 rnear;
    s32 rfar;
    HeroInfo *hero;
    N2OHeroBox *box;
    NAS *nas;
    EXTengine *eng;
    FlagRecord *rec;
    EXTobject *obj;
    EXTmotion *puck;
    s32 r;
    s32 rad;
    s32 d;
    s32 dist;
    s32 y;
    s32 hy;
    s32 x1;
    s32 x2;
    s32 z2;
    s32 dz;
    s32 fa;
    s16 sp;
    TrigEntry *tbl;
    s32 cz;
    s32 fb;
    u16 ang;
    u16 ang2;
    u16 fc;
    u16 fc2;
    N2OHeroBox *base;

    if (Heroes[arg0].hero_status & 0xF800) {
        return 0;
    }
    me = Heroes[arg0].hero_eng;
    if (me == NULL) {
        return 0;
    }
    speed = Heroes[arg0].hero_move->forward_speed;
    if (arg0 == 3) {
        speed = -Heroes[3].hero_move->forward_speed;
    }
    if (sub_find_point(me, me->sub, 1, &pos) == NULL) {
        *(SLONG_XYZ *)&pos = me->XYZ;
    }
    if (FindCollisionBox(me, 2) != NULL) {
        if (me->xform.dirty != 0) {
            rotscale_update(&me->xform);
        }
        gfx_load_xform(me->sub, &me->xform.mtx, &me->XYZ);
    }
    hy = Heroes[arg0].hero_eng->XYZ.y;
    base = N2OHeroBoxes;
    box = &base[arg0];
    top = hy + box->b;
    bot = hy + box->a;
    rnear = box->c;
    rfar = box->d;
    nearp.x = Heroes[arg0].hero_eng->XYZ.x + box->x0;
    nearp.y = Heroes[arg0].hero_eng->XYZ.y + box->y0;
    nearp.z = Heroes[arg0].hero_eng->XYZ.z + box->z0;
    hero = &Heroes[arg0];
    farp.x = hero->hero_eng->XYZ.x + box->x1;
    farp.y = hero->hero_eng->XYZ.y + box->y1;
    farp.z = hero->hero_eng->XYZ.z + box->z1;
    tbl = sin_cos_table;
    for (nas = HeadOfSolids;; nas = nas->Solid.Child) {
        if (nas == NULL) {
            return 0;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec = FindCollisionBox(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        dz = rec->dy;
        y = eng->XYZ.y;
        x1 = rec->height;
        if (!(top < y + dz)) {
            continue;
        }
        y += x1;
        if (!(y < bot)) {
            continue;
        }
        rad = rec->radius;
        if (angle_diff_abs(hero->hero_move->forward_angle,
                          get_angle(-eng->OldXYZ.x + pos.vx, -eng->OldXYZ.z + pos.vz)) >= 0x38F) {
            r = rnear;
            tgt = nearp;
        } else {
            r = rfar;
            tgt = farp;
        }
        get_angle(eng->XYZ.x - pos.vx, eng->XYZ.z - pos.vz);
        obj = FindExtension(nas, 0x1000);
        if (obj == NULL) {
            continue;
        }
        rad += r;
        y = eng->XYZ.x - tgt.x;
        dist = y * y;
        y = eng->XYZ.z - tgt.z;
        dist += y * y;
        if (!(dist < rad * rad)) {
            continue;
        }
        dist = int_sqrt(dist) >> 6;
        if (!(dist < rad)) {
            continue;
        }
        puck = FindExtension(nas, 0x800);
        if (puck != NULL) {
            r = puck->Xa;
        } else {
            r = get_angle(eng->OldXYZ.x - eng->XYZ.x, eng->OldXYZ.z - eng->XYZ.z);
        }
        N2OLineToCircleTest(&eng->XYZ, r, &tgt, rad + 8);
        r = get_angle(-eng->XYZ.x + tgt.x, -eng->XYZ.z + tgt.z) & 0xFFF;
        puck = FindExtension(nas, 0x800);
        if (puck == NULL) {
            continue;
        }
        obj->player = arg0;
        rad = puck->Xa;
        rad += 0x800;
        rad &= 0xFFF;
        if (obj->whatami == 2 && !(hero->hero_status & 0xFA00) &&
            (arg0 == 0 || (GameTeams[0] != 0 && arg0 == 1))) {
            if (!(GameCheats & 1)) {
                hero->hero_score = 0;
                hero->hero_health = 0;
                ((void (*)(s32, s32, s32))pad_rumble_big)(arg0, 0xC, 0xFF);
                SetHeroAction(arg0, 0x2E);
            }
            KillNAS(nas);
            return 0;
        }
        PlaySfx(N2OBallHitSfx[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
        fa = hero->hero_move->forward_angle & 0xFFF;
        ang = r;
        fc = fa;
        sp = abs(speed);
        x1 = puck->Xvel * tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
        z2 = puck->Xvel * tbl[r & 0xFFF].unk2 >> 12;
        dz = sp * tbl[(fc - 0x400) & 0xFFF].unk2 >> 12;
        cz = sp * tbl[fa].unk2 >> 12;
        r = get_angle(x1 + dz, z2 + cz) & 0xFFF;
        d = angle_diff(rad, r);
        if (abs(d) > 0x400) {
            angle_diff(rad ^ 0x800, r);
            fb = hero->hero_move->forward_angle & 0xFFF;
            ang2 = r;
            fc2 = fb;
            sp = abs(speed);
            x1 = puck->Xvel * tbl[(ang2 - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->Xvel * tbl[r].unk2 >> 12;
            dz = sp * tbl[(fc2 - 0x400) & 0xFFF].unk2 >> 12;
            x2 = sp * tbl[fb].unk2 >> 12;
            r = get_angle(x1 + dz, z2 + x2) & 0xFFF;
        } else {
            if (abs(d) > 0x3F0) {
                if (d >= 0) {
                    d = 0x3F0;
                } else {
                    d = -0x3F0;
                }
            }
            r += d;
            r &= 0xFFF;
        }
        puck->Xa = r;
        if (speed != 0) {
            puck->Xvel += abs(speed);
            if (puck->Xvel > puck->Xmax + 0x40) {
                puck->Xvel = puck->Xmax + 0x40;
            }
        }
        if (puck->Ftime == 0 && puck->Xmax < 0x90) {
            puck->Xmax += 4;
        }
    }
    return 0;
}

s32 N2OHeroRad2ObjectCollision(u32 arg0, s32 arg1, bool arg2) {
    EXTengine *hero;
    EXTengine *o;
    FlagRecord *rec;
    NAS *n;
    EXTobject *t;
    EXTmotion *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 ang;
    s32 hit;

    hero = Heroes[arg0].hero_eng;
    hit = 0;
    if (hero != NULL) {
        n = (NAS *)HeadOfBalls;
        if (n != NULL) {
            do {
                o = n->Engine;
                if (o != NULL && o != hero) {
                    rec = FindCollisionBox(o, 0x40);
                    if (rec != NULL) {
                        dx = o->XYZ.x - hero->XYZ.x;
                        dz = o->XYZ.z - hero->XYZ.z;
                        r = rec->radius;
                        r += arg1;
                        d = dx * dx + dz * dz;
                        if (d < r * r) {
                            d = int_sqrt(d) >> 6;
                            if (d < r) {
                                t = FindExtension(n, 0x1000);
                                if (t != NULL
                                    && (t->status == 1 || t->status == 8)
                                    && t->whatami == 1 && t->timer == 0) {
                                    t->can_do |= 8;
                                    ang = get_angle(-dx, -dz) & 0xFFF;
                                    d = r - d + 1;
                                    o->XYZ.x -= (d * sin_cos_table[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                    o->XYZ.z -= (d * sin_cos_table[ang].unk2) >> 12;
                                    t->player = arg0;
                                    e = FindExtension(n, 0x800);
                                    if (e != NULL) {
                                        hit = 1;
                                        PlaySfx(0x585, NULL, NULL, 0x1000, 0x1E00, 0);
                                        t->timer = 4;
                                        e->Xa = ang;
                                        e->Xvel = e->Xmax + 0x40;
                                        e->Xmax = 0x90;
                                    }
                                }
                            }
                        }
                    }
                }
                n = n->Solid.Child;
            } while (n != NULL);
        }
    }
    return hit;
}

s32 TestN2OObjectObstacles(EXTengine *eng, N2OFuxF *self) {
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *other;
    NAS *it;
    EXTmotion *mine;
    EXTmotion *m;
    N2OFuxF *fux;
    s32 hit;
    s32 rad;
    s32 bot;
    s32 top;
    s32 lo;
    s32 hi;
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;
    s32 ang;

    hit = 0;
    box = FindCollisionBox(eng, 0x40);
    if (box != NULL) {
        top = eng->XYZ.y + box->dy;
        bot = eng->XYZ.y + box->height;
        rad = box->radius;
        mine = FindExtension(((NAS **)eng)[-1], 0x800);
        it = HeadOfSolids;
        if (it != NULL) {
            do {
                if (it->Class & 0x200) {
                    other = it->Engine;
                    if (other != NULL && other != eng) {
                        rec = FindCollisionBox(other, 0x40);
                        if (rec != NULL) {
                            lo = other->XYZ.y + rec->dy;
                            hi = other->XYZ.y + rec->height;
                            if (bot < lo && hi < top) {
                                dx = other->XYZ.x - eng->XYZ.x;
                                dz = other->XYZ.z - eng->XYZ.z;
                                r = rec->radius;
                                r += rad;
                                if ((d = dx * dx + dz * dz) < r * r) {
                                    d = int_sqrt(d) >> 6;
                                    do {
                                    if (d < r) {
                                        ang = get_angle(-dx, -dz) & 0xFFF;
                                        d = r - d + 1;
                                        fux = (N2OFuxF *)it->FUX;
                                        if (fux != NULL) {
                                            if (self->flags & 0x80) {
                                                m = FindExtension(it, 0x800);
                                                if (m != NULL) {
                                                    other->XYZ.x -= (d * sin_cos_table[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                    other->XYZ.z -= (d * sin_cos_table[ang].unk2) >> 12;
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += angle_diff((m->Xa + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    m->Xa = ang;
                                                    PlaySfx(0x580, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                                                }
                                            } else {
                                                if (r / 2 < d) {
                                                    d = r;
                                                }
                                                eng->XYZ.x += (d * sin_cos_table[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->XYZ.z += (d * sin_cos_table[ang].unk2) >> 12;
                                                if (mine != NULL) {
                                                    m = FindExtension(it, 0x800);
                                                    if (m != NULL) {
                                                        if (!(fux->flags & 0x80)) {
                                                            m->Xa = ang;
                                                            dx = (mine->Xvel + m->Xvel) / 2;
                                                            m->Xvel = dx;
                                                            mine->Xvel = dx;
                                                        }
                                                        PlaySfx(0x580, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                                                    }
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += angle_diff((mine->Xa + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    mine->Xa = ang;
                                                }
                                            }
                                            hit = 1;
                                        }
                                    }
                                    } while (0);
                                }
                            }
                        }
                    }
                }
                it = it->Solid.Child;
            } while (it != NULL);
        }
    }
    return hit;
}

NAS *N2OBossPushObjects(void) {
    SLONG_XYZ sp10;
    s32 r;
    EXTengine *me;
    EXTengine *eng;
    FlagRecord *rec;
    NAS *nas;
    EXTengine *ext;
    s32 r0;
    s32 dx;
    s32 dz;

    me = N2OBossRound.eng;
    if (me != NULL) {
        rec = FindCollisionBox(me, 0x40);
        if (rec != NULL) {
            if (me->xform.dirty != 0) {
                rotscale_update(&me->xform);
            }
            gfx_load_xform(me->sub, &me->xform.mtx, &me->XYZ);
            r0 = rec->radius;
            view_transform((SVECTOR *)rec, (VECTOR *)&sp10);
            for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
                eng = nas->Engine;
                if (eng != NULL && eng != me) {
                    FlagRecord *rec2;
                    do { rec2 = FindCollisionBox(eng, 0x40); } while (0);
                    if (rec2 != NULL) {
                        r = rec2->radius;
                        ext = FindExtension(nas, 0x1000);
                        if (ext != NULL) {
                            r += r0;
                            if (ext->xform.rot.vx & 8) {
                                dx = eng->XYZ.x - sp10.x;
                                dz = eng->XYZ.z - sp10.z;
                                if (r * r > dx * dx + dz * dz && FindExtension(nas, 0x800) != NULL) {
                                    ext->xform.rot.vy = 0x100;
                                    return nas;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

s32 N2OMissileHitTest(s32 x, s32 y, s32 z, u16 size, u16 mask) {
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
                    obj->player = -1;
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

s32 TestN2OObject2Hero(N2OVel *arg0, s32 arg1) {
    s32 i;
    FlagRecord *rec;
    s32 r;
    s32 dz;
    s32 dx;
    EXTengine *eng;
    HeroInfo *hero;
    HeroInfo *base = Heroes;

    for (i = 0; i < 4; i++) {
        hero = &base[i];
        if (hero->hero_status & 0xF800) {
            continue;
        }
        eng = hero->hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = FindCollisionBox(eng, 0x40);
        if (rec == NULL) {
            goto next;
        }
        dx = eng->XYZ.x - arg0->x;
        dz = eng->XYZ.z - arg0->z;
        r = rec->radius + (arg1 & 0xFFFF);
        r = r * r;
        if ((dx * dx) + (dz * dz) < r) {
            return i;
        }
    next:;
    }
    return -1;
}

void PlotN2ODeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (GameExitFlags & 0x801) {
        N2OWallsHit &= ~arg0->UserInfo.flags;
        KillNAS(arg0);
        return;
    }
    eng = arg0->Engine;
    if (eng != NULL) {
        if (arg0->UserInfo.user1 != 0) {
            arg0->UserInfo.user1 = arg0->UserInfo.user1 - 1;
        } else {
            eng->flags |= 0x8000;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->entity = (GameFrameCount & 7) + 0x2001;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

s32 GetN2OContour(EXTengine *arg0) {
    return (arg0->XYZ.y) - World_floor_level;
}

void N2OShowPhaseObjects(u16 arg0) {
    N2OFlagNode *n = ((N2OFlagNode *)MapScene->subs);
    s16 *p;

    if (arg0) {
        while (n != NULL) {
            p = (s16 *)model_entry_data(n->dbase, n->entity);
            if (p[1] == 0x63) {
                if (p[0] != 0x62) {
                    n->flags |= 0x8000;
                } else {
                    n->flags &= ~0x8000;
                }
            } else {
                n->flags &= ~0x8000;
            }
            n = n->next;
        }
    } else {
        while (n != NULL) {
            p = (s16 *)model_entry_data(n->dbase, n->entity);
            if (p[1] == 0x63) {
                n->flags &= ~0x8000;
            } else {
                n->flags |= 0x8000;
            }
            n = n->next;
        }
    }
}

void N2OCameraFollow(camera_info_tag *unused) {
    s32 *t;
    s32 z;
    camera_info_tag *cam;
    EXTengine *eng;
    s32 d;

    t = path_get(0x63);
    eng = Heroes[0].hero_eng;
    z = eng->XYZ.z;
    if (GameTeams[0] != 0) {
        if (Heroes[0].hero_status & 0xFA00) {
            z = Heroes[1].hero_eng->XYZ.z;
        }
        if (Heroes[1].hero_status & 0xFA00) {
            z = eng->XYZ.z;
        }
    }
    if ((!(Heroes[0].hero_status & 0xFA00) || !(Heroes[1].hero_status & 0xFA00)) && t != NULL) {
        cam = &MainCamera;
        cam->eye.x = 0;
        cam->eye.y = 0;
        cam->eye.z = get_dist_angles(t + 0x13, t + 0xC, &cam->xform.angle.vx);
        cam->at = *(CamXYZ *)&((MapPath *)t)->keys[0].pos;
        cam->dist = 0x200;
        d = -0xA00 - cam->at.z;
        cam->at.z = z - d;
    }
}

void PlotN2OmissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s16 *p;
    s16 (*tab)[3];

    eng = nas->Engine;
    if (eng != NULL) {
        tab = N2OExplodeScales;
        p = tab[nas->UserInfo.flags];
        sub = eng->sub;
        if (p[0] != 0) {
            sub->xform.scale.vx = p[0];
            sub->xform.scale.vy = p[1];
            sub->xform.scale.vx = p[2];
            sub->xform.dirty = 1;
            nas->UserInfo.flags++;
            return;
        }
        KillNAS(nas);
    }
}

void InitN2OmissileExplode(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        sub->u6C.dbase = &HeroModelBank[DB_Oxide];
        sub->entity = 0x2005;
        sub->xform.hasScale = 1;
        sub->xform.order = 0;
        eng->flags |= 0x21;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void N2OSpawnExplosion(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas;

    nas = CreateNAS(arg0, arg1 - 0x80, arg2, 0x1B04, 0x11);
    if (nas != NULL) {
        nas->UserInfo.user1 = arg3;
        PlaySfx(N2OExplosionSfx[Random(4)], NULL, &nas->Engine->XYZ, 0x1000, 0x1E00, 0);
    }
}

void InitN2Ofirstmissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 ang;
    s32 d;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        eng->xform.order = 0;
        sub->u6C.dbase = &HeroModelBank[DB_Oxide];
        sub->entity = 0x2004;
        sub->xform.order = 0;
        sub->otz = 0x100;
        sub->flags |= 0x02000000;
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = -0x28;
            mot->Xmax = 0x100;
            mot->Xacc = 8;
            mot->Xdec = 0;
        }
        d = Heroes[0].hero_eng->XYZ.x;
        ang = get_angle(eng->XYZ.x - d, eng->XYZ.z - d);
        nas->UserInfo.user2 = ang;
        eng->xform.rot.vy = ang - 0x400;
        eng->boomshanka = 1;
        nas->UserInfo.flags = 0;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitN2Omissile(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTmotion *mot;
    s32 ang;

    eng = nas->Engine;
    if (eng != NULL) {
        sub = eng->sub;
        eng->xform.order = 0;
        sub->u6C.dbase = &HeroModelBank[DB_Oxide];
        sub->entity = 0x2004;
        sub->xform.order = 0;
        sub->flags |= 0x02000000;
        mot = AddMotion(nas);
        if (mot != NULL) {
            mot->Xvel = -0x28;
            mot->Xmax = 0x100;
            mot->Xacc = 8;
            mot->Xdec = 0;
        }
        ang = get_angle(eng->XYZ.x - ((s32 *)Heroes[0].hero_eng)[4], eng->XYZ.z - ((s32 *)Heroes[0].hero_eng)[4]);
        nas->UserInfo.user2 = ang;
        eng->xform.rot.vy = ang - 0x400;
        eng->boomshanka = 1;
        eng->XYZ.y += 0x200;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void N2OSpawnFirstMissile(void) {
    CreateNAS(0, 0, 0, 0x1B01, 0x11);
}

NAS *N2OFreeBallSlot(void) {
    s32 var_v0;
    NAS *temp_v0;
    EXTobject *temp_v0_2;
    NAS *var_v1;

    var_v1 = HeadOfBalls;
    var_v0 = 0;
    if (var_v1 != NULL) {
        do {
            var_v1 = (var_v1->Group.Child);
            var_v0 += 1;
        } while (var_v1 != NULL);
    }
    if (var_v0 >= 9) {
        return NULL;
    }
    temp_v0 = CreateNAS(0x1000, 0x1000, 0x1000, 0x1B01, 0x11);
    if ((temp_v0 != NULL) && ((temp_v0->Engine) != 0)) {
        temp_v0_2 = (EXTobject *)temp_v0->FUX;
        if (temp_v0_2 != NULL) {
            (temp_v0_2->status) = 0;
        }
    }
    return temp_v0;
}

void N2OHeroDie(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction(arg0, 0x2E);
    }
    ProcessHeroAnim(arg0);
}

void SetN2OHeroMove(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    N2OHeroSetSpeed(arg0);
    SetHeroSeq(arg0, 0x22);
}

void SetN2OHeroTaunt(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x24);
}

void SetN2OHeroIdle(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_move != NULL) {
        h->hero_move->velocity_enable = 1;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    N2OHeroSetSpeed(arg0);
    SetHeroSeq(arg0, 0x21);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

void SetN2OHeroEnter(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    /* a retry from the ball game places him before InitialiseHeroes has made this */
    if (h->hero_move != NULL) {
        h->hero_move->velocity_enable = 0;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    N2OHeroSetSpeed(arg0);
    SetHeroSeq(arg0, 0x21);
}

s32 N2OHeroTestIdle(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        SetHeroAction(arg0, 0x2C);
        return 1;
    }
    return 0;
}

s32 N2OHeroTestMove(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x2C;
    if (!(st & 0x200)) {
        act = 0x2D;
        if (!(st & 0x2000)) {
            return 0;
        }
    }
    SetHeroAction(arg0, act);
    return 1;
}

s32 N2OHeroTestKick(s32 arg0) {
    PadState *base = (PadState *)Pads;
    PadState *pad = &base[arg0];

    if (pad->dirPressed & 0x8000) {
        if (Heroes[arg0].hero_attack_timer == 0) {
            SetHeroAction(arg0, 0x2B);
            return 1;
        }
        pad->lastDir &= 0xFFFF7FFF;
    }
    return 0;
}

void N2OUpdateShadow(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *shadow = h->hero_shadow;

    shadow->XYZ = h->hero_eng->XYZ;
    shadow->XYZ.y -= ((s32 (*)(EXTengine *))ApplyContour)(shadow);
    shadow->xform.rot.vy = h->hero_eng->xform.rot.vy;
    shadow->xform.dirty = 1;
    if (h->hero_status & 0xF800) {
        shadow->sub->flags &= ~0x8000;
    }
}

void N2OInitShadow(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->u6C.dbase = &HeroModelBank[DB_PlayerVehicle];
    s->sub->entity = 0x2006;
    s->XYZ = h->hero_eng->XYZ;
    s->xform.order = 0;
    s->flags |= 0x8041;
    s->sub->scale = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->otz = -0x10;
}

s32 N2OAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
    s32 a = arg1 * sin_cos_table[((arg0 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 b = arg1 * sin_cos_table[arg0 & 0xFFF].unk2;
    s32 c = arg3 * sin_cos_table[((arg2 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 d = arg3 * sin_cos_table[arg2 & 0xFFF].unk2;
    s32 x = a >> 12;
    s32 z = b >> 12;
    x += c >> 12;
    z += d >> 12;
    return get_angle(x, z) & 0xFFFF;
}

s32 N2OTestWalls(N2OVel *arg0, N2OOut *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;

    if (arg2 != 0) {
        lim = 0x1200;
    } else {
        lim = 0xB00;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = -1;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = -1;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = 3;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = 0;
        return 1;
    }
    if (arg2 != 0 && z < -0xA00) {
        if (x > 0x600) {
            arg0->x = 0x600;
        }
        if (arg0->x < -0x600) {
            arg0->x = -0x600;
        }
    }
    return 0;
}

void UpdateN2O(void) {
    if (N2OBallPhase == 1) {
        N2OUpdateSlots();
        N2OBallMaster();
        N2OUpdateBoss();
        return;
    }
    NoxUpdate();
}

void N2OSetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseN2OFirst;
    LevelSpecs.LevelInit = InitialiseN2O;
    LevelSpecs.LevelRestartInit = InitialiseN2O;
    LevelSpecs.GetContour = GetN2OContour;
    LevelSpecs.PostTriggers = 0;
    LevelSpecs.EarliestTriggers = 0;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.LevelRoutine = UpdateN2O;
}
