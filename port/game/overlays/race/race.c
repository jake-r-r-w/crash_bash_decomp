#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/race_ovl.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char RaceAnimRun[] = "RUN";
const char RaceAnimBreathe[] = "BREATHE";
const char RaceAnimHit[] = "HIT";
const char RaceAnimFall[] = "FALL";
const char RaceAnimSwim[] = "SWIM";
const char RaceAnimTrans[] = "TRANS";
const char RaceAnimWin[] = "WIN";

/* data, in address order */
hero_seq CRA_seq_raceidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_racewin[6] = {
    0, 0x8007, 0x48, 0x4200, 0x8000, 0,
};
hero_seq CRA_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq CRA_seq_racerun[8] = {
    0, 0x8007, 0x19, 0x4080, 0x8003, 1, 0x4099, 0,
};
hero_seq CRA_seq_racehit[10] = {
    0, 0x8007, 0x16, 0x4100, 0x8001, 0x4116, 0x4117, 0x8000, 0, 0,
};
hero_seq CRA_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4080, 0x8000, 0,
};
hero_seq CRA_seq_raceswimwin[6] = {
    0, 0x8007, 0x48, 0x4180, 0x8000, 0,
};
hero_seq CRA_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4200, 0x8003, 1, 0x4220, 0,
};
hero_seq CRA_seq_raceswimrun[12] = {
    0, 0x8007, 0x10, 0x4100, 0x8001, 0x4080, 0x8007, 0xF, 0x4081, 0x8000, 0, 0,
};
hero_seq KON_seq_raceidle[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_racewin[6] = {
    0, 0x8007, 0x3F, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq KON_seq_racerun[8] = {
    0, 0x8007, 0x10, 0x4000, 0x8003, 1, 0x4010, 0,
};
hero_seq KON_seq_racehit[10] = {
    0, 0x8007, 0xF, 0x4100, 0x8001, 0x410F, 0x4110, 0x8000, 0, 0,
};
hero_seq KON_seq_raceswimidle[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_raceswimwin[6] = {
    0, 0x8007, 0x24, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4180, 0x8003, 1, 0x41A0, 0,
};
hero_seq KON_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4100, 0x8001, 0x4080, 0x8007, 0x12, 0x4081, 0x8000, 0, 0,
};
hero_seq COC_seq_raceidle[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq COC_seq_racewin[6] = {
    0, 0x8007, 0x40, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq COC_seq_racerun[8] = {
    0, 0x8007, 0x10, 0x4000, 0x8003, 1, 0x4010, 0,
};
hero_seq COC_seq_racehit[10] = {
    0, 0x8007, 0xF, 0x4100, 0x8001, 0x410F, 0x4110, 0x8000, 0, 0,
};
hero_seq COC_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4100, 0x8000, 0,
};
hero_seq COC_seq_raceswimwin[6] = {
    0, 0x8007, 0x42, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4080, 0x8003, 1, 0x40A0, 0,
};
hero_seq COC_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4180, 0x8001, 0x4100, 0x8007, 0xF, 0x4101, 0x8000, 0, 0,
};
hero_seq BRO_seq_raceidle[6] = {
    0, 0x8007, 0xA, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_racewin[6] = {
    0, 0x8007, 0x4B, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq BRO_seq_racerun[8] = {
    0, 0x8007, 0x11, 0x4080, 0x8003, 1, 0x4091, 0,
};
hero_seq BRO_seq_racehit[10] = {
    0, 0x8007, 0x1C, 0x4100, 0x8001, 0x411C, 0x411D, 0x8000, 0, 0,
};
hero_seq BRO_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4100, 0x8000, 0,
};
hero_seq BRO_seq_raceswimwin[6] = {
    0, 0x8007, 0x40, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4080, 0x8003, 1, 0x40A0, 0,
};
hero_seq BRO_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4180, 0x8001, 0x4100, 0x8007, 0xF, 0x4101, 0x8000, 0, 0,
};
hero_seq CTX_seq_raceidle[6] = {
    0, 0x8007, 0xA, 0x4080, 0x8000, 0,
};
hero_seq CTX_seq_racewin[6] = {
    0, 0x8007, 0x52, 0x4200, 0x8000, 0,
};
hero_seq CTX_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq CTX_seq_racerun[8] = {
    0, 0x8007, 0x11, 0x4000, 0x8003, 1, 0x4011, 0,
};
hero_seq CTX_seq_racehit[10] = {
    0, 0x8007, 0x1C, 0x4100, 0x8001, 0x411C, 0x411D, 0x8000, 0, 0,
};
hero_seq CTX_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4080, 0x8000, 0,
};
hero_seq CTX_seq_raceswimwin[6] = {
    0, 0x8007, 0x25, 0x4180, 0x8000, 0,
};
hero_seq CTX_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4200, 0x8003, 1, 0x4220, 0,
};
hero_seq CTX_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4100, 0x8001, 0x4080, 0x8007, 0xF, 0x4081, 0x8000, 0, 0,
};
hero_seq DNG_seq_raceidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_racewin[6] = {
    0, 0x8007, 0x1E, 0x4200, 0x8000, 0,
};
hero_seq DNG_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq DNG_seq_racerun[8] = {
    0, 0x8007, 0x13, 0x4080, 0x8003, 1, 0x4093, 0,
};
hero_seq DNG_seq_racehit[10] = {
    0, 0x8007, 0x18, 0x4100, 0x8001, 0x4118, 0x4119, 0x8000, 0, 0,
};
hero_seq DNG_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4080, 0x8000, 0,
};
hero_seq DNG_seq_raceswimwin[6] = {
    0, 0x8007, 0x20, 0x4180, 0x8000, 0,
};
hero_seq DNG_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4200, 0x8003, 1, 0x4220, 0,
};
hero_seq DNG_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4100, 0x8001, 0x4080, 0x8007, 0xF, 0x4081, 0x8000, 0, 0,
};
hero_seq KAN_seq_raceidle[6] = {
    0, 0x8007, 0xA, 0x4080, 0x8000, 0,
};
hero_seq KAN_seq_racewin[6] = {
    0, 0x8007, 0x40, 0x4200, 0x8000, 0,
};
hero_seq KAN_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq KAN_seq_racerun[8] = {
    0, 0x8007, 0x11, 0x4000, 0x8003, 1, 0x4011, 0,
};
hero_seq KAN_seq_racehit[10] = {
    0, 0x8007, 0x1C, 0x4100, 0x8001, 0x411C, 0x411D, 0x8000, 0, 0,
};
hero_seq KAN_seq_raceswimidle[6] = {
    0, 0x8007, 0x10, 0x4100, 0x8000, 0,
};
hero_seq KAN_seq_raceswimwin[6] = {
    0, 0x8007, 0x40, 0x4200, 0x8000, 0,
};
hero_seq KAN_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4080, 0x8003, 1, 0x40A0, 0,
};
hero_seq KAN_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4180, 0x8001, 0x4100, 0x8007, 0xF, 0x4101, 0x8000, 0, 0,
};
hero_seq TIN_seq_raceidle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_racewin[6] = {
    0, 0x8007, 0x20, 0x4200, 0x8000, 0,
};
hero_seq TIN_seq_racefall[8] = {
    0, 0x8007, 0x18, 0x4180, 0x8003, 1, 0x4198, 0,
};
hero_seq TIN_seq_racerun[8] = {
    0, 0x8007, 0x10, 0x4080, 0x8003, 1, 0x4090, 0,
};
hero_seq TIN_seq_racehit[10] = {
    0, 0x8007, 0x1C, 0x4100, 0x8001, 0x411C, 0x411D, 0x8000, 0, 0,
};
hero_seq TIN_seq_raceswimidle[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq TIN_seq_raceswimwin[6] = {
    0, 0x8007, 0x36, 0x4180, 0x8000, 0,
};
hero_seq TIN_seq_raceswimfall[8] = {
    0, 0x8007, 0x20, 0x4200, 0x8003, 1, 0x4220, 0,
};
hero_seq TIN_seq_raceswimrun[12] = {
    0, 0x8007, 0xE, 0x4100, 0x8001, 0x4080, 0x8007, 0x13, 0x4081, 0x8000, 0, 0,
};
u16 RaceDashAiSkill[10] = {
    4, 1, 1, 1, 0xA, 0x1A, 0x14, 0x10, 0x10, 0x1C,
};
u16 RaceDashAiSkillAlt[10] = {
    6, 3, 1, 1, 0xE, 0x1C, 0x16, 5, 5, 0x1E,
};
u16 RaceDashAiTeamSkill[6] = {
    1, 0xB, 0x16, 1, 0xB, 0x16,
};
u16 RaceDashAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 RaceDashAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2,
};
u16 RaceDashAiHumanTweak[42] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C,
    0xE, 0x13, 0x18, 0xC, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 RaceToxicAiSkill[10] = {
    0x10, 0xD, 0xA, 7, 0xA, 0x1A, 0x14, 0x10, 0x10, 0x1A,
};
u16 RaceToxicAiSkillAlt[10] = {
    0x14, 0x11, 0xE, 0xB, 0x10, 0x1D, 0x18, 0x20, 0x20, 0x20,
};
u16 RaceToxicAiTeamSkill[6] = {
    1, 0xA, 0x14, 1, 0xA, 0x14,
};
u16 RaceToxicAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 RaceToxicAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2,
};
u16 RaceToxicAiHumanTweak[42] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C,
    0xE, 0x13, 0x18, 0xC, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 RaceLavaAiSkill[10] = {
    0x14, 0x11, 0xE, 0xB, 0x12, 0x18, 0x16, 0x10, 0x10, 0x18,
};
u16 RaceLavaAiSkillAlt[10] = {
    0x16, 0x13, 0x10, 0xD, 0x14, 0x18, 0x18, 0x20, 0x20, 0x1A,
};
u16 RaceLavaAiTeamSkill[6] = {
    1, 0xB, 0x16, 1, 0xB, 0x16,
};
u16 RaceLavaAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 RaceLavaAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2,
};
u16 RaceLavaAiHumanTweak[42] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C,
    0xE, 0x13, 0x18, 0xC, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 RaceWaterAiSkill[10] = {
    0x14, 0x11, 0xE, 0xB, 0x13, 0x1A, 0x18, 0x10, 0x10, 0x1C,
};
u16 RaceWaterAiSkillAlt[10] = {
    0x14, 0x11, 0xE, 0xB, 0x13, 0x1B, 0x18, 0x20, 0x20, 0x1D,
};
u16 RaceWaterAiTeamSkill[6] = {
    1, 0xA, 0x14, 1, 0xA, 0x14,
};
u16 RaceWaterAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 RaceWaterAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2,
};
u16 RaceWaterAiHumanTweak[42] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C,
    0xE, 0x13, 0x18, 0xC, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
s32 RaceAccel = 0x300;
s32 RaceMoveTune1 = 0x800;
s32 RaceMoveTune2 = 0x1000;
s32 RaceDecel = 0xA0;
s32 RaceMoveTune3 = 0x200;
s32 RaceMoveTune4 = 0x80;
s32 RaceJumpVel = 0x50;
s32 RaceMoveTune5 = 0xA0;
u8 *RaceObjects = 0;
s32 RaceBarrelCount = 0;
s16 RacePreLoadDB[6] = { 0x1C, 0x17, 0x82, 0xAA, -1, 0 };
char *RaceAnims[6] = {
    (char *)RaceAnimRun, (char *)RaceAnimBreathe, (char *)RaceAnimHit, (char *)RaceAnimFall,
    (char *)RaceAnimSwim, NULL,
};
char *RaceAnimsB[5] = {
    (char *)RaceAnimBreathe, (char *)RaceAnimFall, (char *)RaceAnimSwim, (char *)RaceAnimTrans,
    NULL,
};
s16 RaceLevelDB[4] = { 0x18, 0x81, -1, 0 };
WWInf RaceWorldInfo[8] = {
    { 0xA1, RaceAnims, RaceLevelDB, 1 },
    { 0xA7, RaceAnims, RaceLevelDB, 1 },
    { 0xA8, RaceAnimsB, RaceLevelDB, 1 },
    { 0xA9, RaceAnims, RaceLevelDB, 1 },
    { 0xB6, RaceAnims, RaceLevelDB, 1 },
    { 0xB7, RaceAnims, RaceLevelDB, 1 },
    { 0xB8, RaceAnims, RaceLevelDB, 1 },
    { 0xB9, RaceAnimsB, RaceLevelDB, 1 },
};
GroupFile RaceFiles[9] = {
    &HeroModelBank[DB_Font], &DatIndex[413], &DatIndex[412], &HeroModelBank[DB_Loading], &DatIndex[409],
    &DatIndex[408], &HeroModelBank[DB_Interface], &DatIndex[411], &DatIndex[410], &HeroModelBank[DB_Powerups],
    &DatIndex[407], &DatIndex[406], &HeroModelBank[DB_PolarObjects], &DatIndex[403], &DatIndex[402],
    &HeroModelBank[DB_CrateEffects], &DatIndex[405], &DatIndex[404], &HeroModelBank[DB_CrateObjects], &DatIndex[401],
    &DatIndex[400], &HeroModelBank[DB_Masks], &DatIndex[399], &DatIndex[398], 0, 0,
};
/* the levels' files (the engine's per-mode table points at modes 0x17..0x1A's) */
file_group_load_info file_group_DASH = { &FileGroups[15], RaceFiles };
file_group_load_info file_group_TOXIC = { &FileGroups[15], RaceFiles };
file_group_load_info file_group_WATER = { &FileGroups[15], RaceFiles };
file_group_load_info file_group_LAVA = { &FileGroups[15], RaceFiles };
u16 RaceLUPtab[10] = { 3, 0x20D, 0x1601, 0x1603, 0x1605, 0x1607, 0x160A, 0x1619, 0x1703, 0 };
TrigLookUp RaceLUP = { RaceLUPtab, 9 };
PreMap RACE_PREMAP = { RacePreLoadDB, RaceWorldInfo, &RaceLUP };
u16 RaceHeroModels[10] = {
    0, 0xA2, 0xAB, 0xA3, 0xA6, 0xA4, 0xAD, 0xA5, 0xAC, 0,
};
u16 RaceHeroModelsWater[10] = {
    0, 0xAE, 0xB4, 0xAF, 0xB3, 0xB0, 0xB2, 0xB1, 0xB5, 0,
};
NAS *RacePickups = 0;
s32 RaceEggCount = 0;
s32 RaceTagTimer = 0;
s32 RaceTimer = 0;
s32 RaceFloorLevel = -0x100;
/* the shell settings (engine world records 0x0017 and 0x0018) */
shell_setup race_shell_info = {
    0x2000, 0x10, 0x2000, 0xC8, 0x100, 0x200, VramAreas, VramNodePool, 0x20, 0x708,
};
s32 RaceOver = 0;
VictorySeq RaceVictorySeqs[4] = {
    { &RaceAnimWin, 0x4C },
    { &RaceAnimWin, 0x55 },
    { &RaceAnimWin, 0xD },
    { &RaceAnimWin, 0xD },
};
s32 RaceAISpeeds[9] = { 0, 5, 5, 0, 0xA, 0x1E, 0x1E, 0xA, 0xA };
RaceSpinObj *RaceSpinner = 0;
s32 RacePatternTime = 0;
s32 RacePatternIdx = 0;
s32 RaceUnused0[1] = {0};
RaceObj74 *RaceGlowObj = 0;
s32 RaceGlowPhase = 0;
s32 RaceGlowMode = 0;
s32 RacePistonSteps0[36] = {
    0xA, 0xA, 0x1D, 0xA, 0x14, 0x1D, 0xA, 0x1D, 0x1D, 0xA, 0x28, 0x1D, 0xA, 0x32, 0x1D, 0xA,
    0x3C, 0x1D, 0x78, 0, 0x1D, 0x78, 0, 0x1D, 0x78, 0, 0x1D, 0x78, 0, 0x1D, 0x78, 0, 0x1D,
    0x78, 0, 0x1D,
};
s32 RacePistonSteps1[18] = {
    0x78, 0, 0x1D, 0x78, -1, 0x1D, 0x78, -1, 0x1D, 0x78, -1, 0x1D, 0x78, -1, 0x1D, 0x78, -1,
    0x1D,
};
s32 RacePistonSteps2[36] = {
    0x78, 0, 0x1D, 0, -1, 0, 0, -1, 0, 0x78, 0, 0x1D, 0, -1, 0, 0, -1, 0, 0x78, 0, 0x1D, 0, -1,
    0, 0x78, 0, 0x1D, 0, -1, 0, 0x78, 0, 0x1D, 0, -1, 0,
};
s32 RacePistonSteps3[18] = { 0x4B0, 0, 0, 0x4B0, 0, 0, 0x4B0, 0, 0, 0x4B0, 0, 0, 0x4B0, 0, 0, 0x4B0, 0, 0 };
RacePattern RacePistonPatterns[6] = {
    { (void *)&RacePistonSteps0, 0xB4 }, { (void *)&RacePistonSteps1, 0xB4 },
    { (void *)&RacePistonSteps1, 0xB4 }, { (void *)&RacePistonSteps2, 0xB4 },
    { (void *)&RacePistonSteps2, 0xB4 }, { (void *)&RacePistonSteps3, 0x708 },
};
s32 RacePistonLimit = 0x11;
s32 RacePickupTimer = 0;
s32 RaceUnused1 = 0;
s32 RaceBarrelRollTimer = 0xB4;
s32 RaceStartDelay = 0;
RaceShort3 RaceJumpTable[8] = {0};
s32 RaceSteerSpeeds[11] = { 0x180, 0x19E, 0x1BC, 0x1DA, 0x1F8, 0x248, 0x27A, 0x2AC, 0x2DE, 0x310, 0x380 };
s32 RaceSteerAngles[11] = { 0, 0x19, 0x32, 0x4B, 0x64, 0x7D, 0x96, 0xAF, 0xC8, 0xE1, 0x100 };
s32 RaceSmokeBoostSteps[17] = {
    0x2029, 0x202A, 0x202B, 0x202C, 0x202D, 0x202E, 0x202F, 0x2030, 0x2031, 0x2032, 0x2033,
    0x2034, 0x2035, 0x2036, 0x2037, 0x2038, 0x8003,
};
void *RaceSmokeBoostAnimList[3] = { &RaceSmokeBoostSteps, 0, 0 };
ScriptInit RaceSmokeBoostScriptInit = { 0, (void *)&RaceSmokeBoostAnimList, 0x402 };
s32 RacePickupTypes[6] = { 1, 2, 0, 0, 4, 5 };
s32 RacePickupCount = 0;
s32 RacePickupHeroCount = 0;
s32 DinoEggSteps[20] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x8000,
};
void *DinoEggAnimList[3] = { &DinoEggSteps, 0, 0 };
ScriptInit DinoEggScriptInit = { 0, (void *)&DinoEggAnimList, 0x402 };
s32 RaceTagTime = 0x12C;
s32 RaceSmokePoofSteps[17] = {
    0x201B, 0x1201B, 0x201D, 0x201E, 0x201F, 0x2020, 0x2021, 0x2022, 0x2023, 0x2024, 0x2025,
    0x2026, 0x2027, 0x2028, 0x2029, 0x202A, 0x8003,
};
void *RaceSmokePoofAnimList[3] = { &RaceSmokePoofSteps, 0, 0 };
ScriptInit RaceSmokePoofScriptInit = { 0, (void *)&RaceSmokePoofAnimList, 0x402 };
s32 RaceGreenSmokePoofSteps[17] = {
    0x2019, 0x12019, 0x201B, 0x201C, 0x201D, 0x201E, 0x201F, 0x2020, 0x2021, 0x2022, 0x2023,
    0x2024, 0x2025, 0x2026, 0x2027, 0x2028, 0x8003,
};
void *RaceGreenSmokePoofAnimList[3] = { &RaceGreenSmokePoofSteps, 0, 0 };
ScriptInit RaceGreenSmokePoofScriptInit = { 0, (void *)&RaceGreenSmokePoofAnimList, 0x402 };
s32 RaceGreenSmokeBoostSteps[17] = {
    0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008, 0x2009, 0x200A, 0x200B,
    0x200C, 0x200D, 0x200E, 0x200F, 0x2010, 0x8003,
};
void *RaceGreenSmokeBoostAnimList[3] = { &RaceGreenSmokeBoostSteps, 0, 0 };
ScriptInit RaceGreenSmokeBoostScriptInit = { 0, (void *)&RaceGreenSmokeBoostAnimList, 0x402 };
s32 GlobSteps0[3] = { (s32)0x80110000, 0x4007006F, 0x18005 };
s32 GlobSteps1[3] = { (s32)0x80110000, 0x4080002B, 0x18005 };
s32 GlobSteps2[3] = { (s32)0x80110000, 0x41800020, 0x18005 };
s32 GlobSteps3[3] = { (s32)0x80110000, 0x42000001, 0x8000 };
void *GlobAnimList[6] = { &GlobSteps0, &GlobSteps1, &GlobSteps3, &GlobSteps2, 0, 0 };
ScriptInit GlobScriptInit = { 0, (void *)&GlobAnimList, 0x401 };
s32 TrackGlobSteps[3] = { (s32)0x80110000, 0x4100001C, 0x8000 };
void *TrackGlobAnimList[3] = { &TrackGlobSteps, 0, 0 };
ScriptInit TrackGlobScriptInit = { 0, (void *)&TrackGlobAnimList, 0x401 };
s32 RaceTrackGlobCount = 0;
s32 DacSteps[3] = { (s32)0x80110000, 0x40800012, 0x8000 };
void *DacAnimList[3] = { &DacSteps, 0, 0 };
ScriptInit DacScriptInit = { 0, (void *)&DacAnimList, 0x401 };
s32 RaceSmokeBoostWaterSteps[17] = {
    0x2039, 0x203A, 0x203B, 0x203C, 0x203D, 0x203E, 0x203F, 0x2040, 0x2041, 0x2042, 0x2043,
    0x2044, 0x2045, 0x2046, 0x2047, 0x2048, 0x8003,
};
void *RaceSmokeBoostWaterAnimList[3] = { &RaceSmokeBoostWaterSteps, 0, 0 };
ScriptInit RaceSmokeBoostWaterScriptInit = { 0, (void *)&RaceSmokeBoostWaterAnimList, 0x402 };
RaceBarrel RaceBarrels[128] = {0};
s32 RaceSteer[4] = {0};
void *RaceSpinnerBoxes[6] = {0};
RacePiston RacePistons[6] = {0};
s32 RaceSpinTimer = 0;
s32 RaceSpinDelay = 0;
s32 RaceUnused2[16] = {0};
RacePickUpSlot RacePickupSlots[16] = {0};
void *RaceJumpHooks[1] = { RaceResetJumps };

s32 RaceAI_DodgeSpinners(s32 player) {
    s32 i;
    HeroInfo *h;
    void **p;
    SLONG_XYZ pos;

    i = 0;
    pos.x = Heroes[player].hero_eng->XYZ.x - (0x40 * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12);
    h = &Heroes[player];
    pos.y = 0;
    p = RaceSpinnerBoxes;
    pos.z = h->hero_eng->XYZ.z - (0x40 * rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2 >> 12);
loop:
    if (i < 6) {
        if (RaceXZBoxToSphere(*p, &pos, 1, &pos) != -1) {
            h->hero_ai->AIpad &= ~0x20;
            h->hero_ai->AIpad |= 0x2000;
            h->hero_ai->AIpad |= 0x80;
            ((PadState *)Pads)[player].dir = h->hero_ai->AIpad;
            return 1;
        }
        h++;
        h--;
        p++;
        p++;
        p--;
        i++;
        goto loop;
    }
    return 0;
}

s32 RaceAI_PickVictim(s32 arg0) {
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

void RaceAI_SpinnerBrake(s32 player) {
    SLONG_XYZ pos;
    void **p;
    s32 i;
    s32 r;
    HeroInfo *h;

    pos.x = Heroes[player].hero_eng->XYZ.x - (0x40 * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12);
    pos.y = 0;
    pos.z = Heroes[player].hero_eng->XYZ.z - (0x40 * rcossin_tbl[Heroes[player].hero_eng->xform.rot.vy & 0xFFF].unk2 >> 12);
    i = 0;
    if (Heroes[player].hero->FUX != NULL) {
        do {
            h = &Heroes[player];
        } while (0);
        p = RaceSpinnerBoxes;
    loop:
        if (i < 6) {
            r = RaceXZBoxToSphere(*p, &pos, 1, &pos);
            if (r != -1) {
                p++;
                if (!(r & 4)) {
                    h->hero_ai->AIpad |= 0x4000;
                    ((PadState *)Pads)[player].dir = h->hero_ai->AIpad;
                    return;
                }
            } else {
                p++;
            }
            i++;
            goto loop;
        }
    }
}

void RaceAI_AvoidBarrels(s32 player) {
    s32 b[3];
    s32 a[3];
    s16 c[8];
    RaceBarrel *p;
    subengine *o;
    RaceBarrel *best;
    HeroInfo *hp;
    HeroInfo *h;
    HeroInfo *base;
    s32 x;
    s32 z;
    s32 d;
    s32 i;
    s32 ang;
    s32 bestd;
    s32 besti;

    bestd = 0x7FFFFFFF;
    besti = -1;
    i = 0;
    p = RaceBarrels;
    ang = get_angle(Heroes[player].hero_eng->XYZ.x, Heroes[player].hero_eng->XYZ.z) & 0xFFF;
    ang -= 0x400;
    ang &= 0xFFF;
    hp = &Heroes[player];
    do {
loop:
    if (i < RaceBarrelCount) {
        o = p->sub;
        if (p->bStatus == 1) {
            x = o->XYZ.x;
            z = o->XYZ.z;
            if ((((get_angle(x, z) & 0xFFF) - 0x400) & 0xFFF) >= ang) {
                x -= hp->hero_eng->XYZ.x;
                z -= hp->hero_eng->XYZ.z;
                d = x * x + z * z;
                if (d < bestd) {
                    bestd = d;
                    besti = i;
                }
            }
        }
        i++;
        p++;
        goto loop;
    }
    } while (0);
    if (besti != -1) {
        base = Heroes;
        h = &base[player];
        a[0] = 0;
        a[2] = 0;
        b[0] = rcossin_tbl[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2;
        b[1] = 0;
        b[2] = rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2;
        best = &RaceBarrels[besti];
        c[0] = best->sub->XYZ.x - h->hero_eng->XYZ.x;
        c[2] = best->sub->XYZ.z - h->hero_eng->XYZ.z;
        c[3] = 0x180;
        if (RaceRayHit(a, b, c, 0) != 0) {
            x = best->sub->XYZ.x;
            z = best->sub->XYZ.z;
            d = x * x + z * z;
            if (d < 0x400000) {
                h->hero_ai->AIpad &= ~0x20;
                h->hero_ai->AIpad |= 0x80;
            } else {
                h->hero_ai->AIpad &= ~0x80;
                h->hero_ai->AIpad |= 0x20;
            }
            h->hero_ai->AIpad &= ~0x8000;
            ((PadState *)Pads)[player].dir = h->hero_ai->AIpad;
        }
    }
}

void RaceAI_AvoidObjects(s32 player) {
    s32 b[3];
    s32 a[3];
    s16 c[8];
    NAS *n;
    NAS *best;
    EXTengine *e;
    EXTengine *he;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;
    s32 d;
    s32 bestd;
    HeroInfo *h;
    HeroInfo *base;

    n = (NAS *)RaceObjects;
    best = NULL;
    bestd = 0x7FFFFFFF;
    while (1) {
        if (n == NULL) {
            break;
        }
        e = n->Engine;
        if (e != NULL) {
            he = Heroes[player].hero_eng;
            dx = he->XYZ.x - e->XYZ.x;
            dz = he->XYZ.z - e->XYZ.z;
            d = dx * dx + dz * dz;
            if (d < bestd) {
                best = n;
                bestd = d;
            }
        }
        n = n->Group.Child;
    }
    if (best != NULL) {
        base = Heroes;
        h = &base[player];
        a[0] = 0;
        a[2] = 0;
        b[0] = rcossin_tbl[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2;
        b[1] = 0;
        b[2] = rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2;
        c[0] = best->Engine->XYZ.x - h->hero_eng->XYZ.x;
        c[2] = best->Engine->XYZ.z - h->hero_eng->XYZ.z;
        c[3] = 0x180;
        if (RaceRayHit(a, b, c, 0) != 0) {
            d = best->Engine->XYZ.x;
            z = best->Engine->XYZ.z;
            x = d * d + z * z;
            d = h->hero_eng->XYZ.x;
            z = h->hero_eng->XYZ.z;
            if (x < d * d + z * z) {
                h->hero_ai->AIpad &= ~0x20;
                h->hero_ai->AIpad |= 0x80;
            } else {
                h->hero_ai->AIpad &= ~0x80;
                h->hero_ai->AIpad |= 0x20;
            }
            h->hero_ai->AIpad &= ~0x8000;
            ((PadState *)Pads)[player].dir = h->hero_ai->AIpad;
        }
    }
}


s32 RaceRayHit(s32 *a, s32 *b, s16 *c, void *unused) {
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

s32 RaceAI_UseCloseWeapon(s32 player) {
  HeroInfo *h;
  HeroInfo *o;
  int new_var;
  s32 i;
  s32 dx;
  s32 dz;
  PadState *pad;
  HeroInfo *t;
  HeroInfo *base;
  base = Heroes;
  t = &base[player];
  if (t->hero->FUX == 0)
  {
    return 0;
  }
  if (((RaceFux *) t->hero->FUX)->mode != 0x20)
  {
    return 0;
  }
  i = 0;
  h = t;
  pad = (PadState *) Pads;
  o = base;
  loop:
  if (i < 4)
  {
    if (((i == player) || (i == h->hero_teammate)) || (RaceAI_PickVictim(player) != i))
    {
      goto next;
    }
    do
    {
      dx = h->hero_eng->XYZ.x - o->hero_eng->XYZ.x;
      dz = h->hero_eng->XYZ.z - o->hero_eng->XYZ.z;
      new_var = (dx * dx) + (dz * dz);
      if (new_var < 0x90000)
      {
        do
        {
          h->hero_ai->AIpad |= 0x2000;
          pad[player].dir = h->hero_ai->AIpad;
          return 1;
        }
        while (0);
      }
      next:
      o++;
      i++;
    }
    while (0);
    goto loop;
  }

  return 0;
}

s32 RaceAI_UseAimedWeapon(s32 player) {
    TrigEntry *trig;
    PadState *pad;
    HeroInfo *o;
    HeroInfo *h;
    HeroInfo *base;
    s32 i;
    s32 a;
    s32 to[4];
    s32 from[4];
    s16 rel[8];

    if (Heroes[player].hero->FUX == 0) {
        return 0;
    }
    if (((RaceFux *) Heroes[player].hero->FUX)->mode != 1) {
        return 0;
    }
    i = 0;
    trig = rcossin_tbl;
    base = Heroes;
    pad = (PadState *) Pads;
    o = base;
loop:
    if (i < 4) {
        if (i == player) {
            goto next;
        }
        h = (HeroInfo *)((u8 *)base + player * sizeof(HeroInfo));
        if (i == h->hero_teammate) {
            goto next;
        }
        if (RaceAI_PickVictim(player) != i) {
            goto next;
        }
        a = get_angle(o->hero_eng->XYZ.x, o->hero_eng->XYZ.z) & 0xFFF;
        if ((((a - (get_angle(h->hero_eng->XYZ.x, h->hero_eng->XYZ.z) & 0xFFF)) << 20) >> 20) < 100) {
            goto next;
        }
        from[0] = 0;
        from[2] = 0;
        to[0] = trig[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 2048 >> 12;
        to[1] = 0;
        do { to[2] = trig[h->hero_eng->xform.rot.vy & 0xFFF].unk2 * 2048 >> 12; } while (0);
        rel[0] = o->hero_eng->XYZ.x - h->hero_eng->XYZ.x;
        rel[2] = o->hero_eng->XYZ.z - h->hero_eng->XYZ.z;
        rel[3] = 0x180;
        if (RaceRayHit(from, to, rel, 0) != 0) {
            h->hero_ai->AIpad |= 0x2000;
            pad[player].dir = h->hero_ai->AIpad;
            return 1;
        }
    next:
        do { o++; i++; } while (0);
        goto loop;
    }
    return 0;
}

s32 RaceAI_SteerAtVictim(s32 player) {
    s32 i;
    s32 b[4];
    s32 a[4];
    s16 c[8];
    TrigEntry *trig;
    HeroInfo *o;
    HeroInfo *base;
    HeroInfo *h;
    PadState *pad;
    PadState *pads;
    s32 ang;
    s32 x;
    s32 z;

    if (Heroes[player].hero->FUX != NULL) {
        i = 0;
        trig = rcossin_tbl;
        pads = (PadState *)Pads;
        base = Heroes;
        pad = pads + player;
        o = base;
    loop:
        if (i < 4) {
            do {
                if (i == player) {
                    goto next;
                }
            } while (0);
            ang = get_angle(o->hero_eng->XYZ.x, o->hero_eng->XYZ.z) & 0xFFF;
            h = (HeroInfo *)((u8 *)base + player * (s32)sizeof(HeroInfo));
            if ((((ang - (get_angle(h->hero_eng->XYZ.x, h->hero_eng->XYZ.z) & 0xFFF)) << 20) >> 20) < 100) {
                goto next;
            }
            a[0] = 0;
            a[2] = 0;
            b[0] = trig[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x17D >> 10;
            b[1] = 0;
            do {
                b[2] = trig[h->hero_eng->xform.rot.vy & 0xFFF].unk2 * 0x17D >> 10;
            } while (0);
            c[0] = o->hero_eng->XYZ.x - h->hero_eng->XYZ.x;
            c[2] = o->hero_eng->XYZ.z - h->hero_eng->XYZ.z;
            c[3] = 0x180;
            if (RaceRayHit(a, b, c, 0) != 0 && o->hero->FUX != NULL) {
                x = o->hero_eng->XYZ.x;
                z = o->hero_eng->XYZ.z;
                if (x * x + z * z > 0x310000) {
                    h->hero_ai->AIpad |= 0x20;
                    h->hero_ai->AIpad &= ~0x80;
                } else {
                    h->hero_ai->AIpad |= 0x80;
                    h->hero_ai->AIpad &= ~0x20;
                }
                do {
                    do {
                        pad->dir = h->hero_ai->AIpad;
                    } while (0);
                } while (0);
            }
        next:
            o++;
            do { i++; } while (0);
            goto loop;
        }
    }
    return 0;
}

void RaceAI_Think(s32 arg0) {
    HeroInfo *hero;
    EXTraceai *ext;
    NAS *nas;
    NAS *found;
    s32 angle;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    ext = FindExtension(hero->hero, 0x20000000);
    if (ext != NULL) {
        hero->hero_ai->AIpad = 0;
        hero->hero_ai->AImode = 1;
        ext->unk18 = RaceAI_SteerAhead;
        nas = RaceNearestPickup(arg0);
        found = NULL;
        if (nas != NULL) {
            if (DistXZSq((SLONG_XYZ *)&nas->Engine->XYZ, (SLONG_XYZ *)&hero->hero_eng->XYZ) <= 0x270FFF) {
                angle = get_angle(nas->Engine->XYZ.x, nas->Engine->XYZ.z) & 0xFFF;
                if ((get_angle(hero->hero_eng->XYZ.x, hero->hero_eng->XYZ.z) & 0xFFF) < angle) {
                    found = nas;
                    goto done;
                }
            }
            found = NULL;
        }
    done:
        if (found != NULL) {
            ext->unk18 = RaceAI_ChasePickup;
            ext->unk14 = RaceAISpeeds[Heroes[arg0].hero_name];
        }
        if (G63->mode != 0x17) {
            RaceAI_SteerAtVictim(arg0);
        }
        ext->unk18(arg0);
        if (G63->mode == 0x19) {
            RaceAI_SpinnerBrake(arg0);
        } else {
            RaceAI_UseAimedWeapon(arg0);
        }
        if (GameMode == 0x18) {
            RaceAI_AvoidBarrels(arg0);
            RaceAI_AvoidObjects(arg0);
            RaceAI_UseCloseWeapon(arg0);
        }
    }
}

void RaceInitHeroAngle(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->xform.rot.vy += angle_diff(h->hero_eng->xform.rot.vy, h->hero_move->forward_angle) >> 2;
            if (GAME->heroCpu[player] != 0) {
                h->hero_eng->xform.rot.vz += angle_diff(h->hero_eng->xform.rot.vz, RaceSteer[player]) >> 3;
            } else {
                h->hero_eng->xform.rot.vz += angle_diff(h->hero_eng->xform.rot.vz, RaceSteer[player]) >> 2;
            }
        }
        Heroes[player].hero_ctrl &= ~1;
        Heroes[player].hero_eng->xform.dirty = 1;
    }
}

void InitialiseRACEFirst(void) {
    EngineHooks *h;
    GameState *g;
    GameState *g2;
    u16 flags;
    void **p;
    void *t;

    RaceOver = 0;
    h = &HeroHooks;
    h->HeroBot = Race_HeroBot;
    h->HeroModels = RaceHeroModels;
    do {} while (0);
    g = GAME;
    if (g->mode == 0x19) {
        h->HeroModels = RaceHeroModelsWater;
    }
    h->InitHero = RaceInitHero;
    h->PreMoveHero = RacePreMoveHero;
    h->MoveHero = RaceMoveHero;
    h->RunSpeeds = RaceJumpTable;
    h->CreateShadow = ModelCreateShadow;
    h->UpdateShadow = ModelUpdateShadow;
    flags = g->levelFlags;
    h->ChooseWinner = RaceChooseWinner;
    if (flags & 0x40) {
        h->ChooseWinner = RaceChooseWinnerTeam;
    }
    if (flags & 0x20) {
        h->ChooseWinner = RaceChooseWinnerTeam;
    }
    if (flags & 0x80) {
        h->ChooseWinner = RaceChooseWinnerTeam;
    }
    h->VictorySeqs = RaceVictorySeqs;
    g->levelFlags = flags | 0x800;
    do {} while (0);
    p = &AiSkillTable;
    if (g->teams != 0) {
        t = RaceDashAiSkillAlt;
    } else {
        t = RaceDashAiSkill;
    }
    *p = t;
    AiBotTweakTable = RaceDashAiBotTweak;
    AiHumanTweakTable = RaceDashAiHumanTweak;
    AiHumanRoundTweakTable = RaceDashAiHumanRoundTweak;
    AiTeamSkillTable = RaceDashAiTeamSkill;
    MainCamera.dist = 0x22B;
    do {} while (0);
    g2 = GAME;
    if (g2->mode == 0x18) {
        p = &AiSkillTable;
        if (g2->teams != 0) {
            t = RaceToxicAiSkillAlt;
        } else {
            t = RaceToxicAiSkill;
        }
        *p = t;
        AiBotTweakTable = RaceToxicAiBotTweak;
        AiHumanTweakTable = RaceToxicAiHumanTweak;
        AiHumanRoundTweakTable = RaceToxicAiHumanRoundTweak;
        AiTeamSkillTable = RaceToxicAiTeamSkill;
    }
    g2 = GAME;
    if (g2->mode == 0x1A) {
        p = &AiSkillTable;
        if (g2->teams != 0) {
            t = RaceLavaAiSkillAlt;
        } else {
            t = RaceLavaAiSkill;
        }
        *p = t;
        AiBotTweakTable = RaceLavaAiBotTweak;
        AiHumanTweakTable = RaceLavaAiHumanTweak;
        AiHumanRoundTweakTable = RaceLavaAiHumanRoundTweak;
        AiTeamSkillTable = RaceLavaAiTeamSkill;
    }
    g2 = GAME;
    if (g2->mode == 0x19) {
        p = &AiSkillTable;
        if (g2->teams != 0) {
            t = RaceWaterAiSkillAlt;
        } else {
            t = RaceWaterAiSkill;
        }
        *p = t;
        AiBotTweakTable = RaceWaterAiBotTweak;
        AiHumanTweakTable = RaceWaterAiHumanTweak;
        AiHumanRoundTweakTable = RaceWaterAiHumanRoundTweak;
        AiTeamSkillTable = RaceWaterAiTeamSkill;
        MainCamera.dist = 0x252;
    }
    RacePickups = NULL;
    RaceEggCount = 0;
}

s32 RaceFindNode(s32 *arg0, s32 arg1, s32 *arg2)
{
  u8 *base;
  s32 off;
  u8 *p;
  u8 *a2;
  u8 *a3;
  s32 found;
  u8 *new_var;
  s32 n;
  s32 i;
  s32 ang;
  s32 a;
  s32 b;
  s32 r;
  s32 c;
  found = -1;
  if (MapScene->dbase != 0)
  {
    u8 *q = ((u8 *)MapScene->map);
    c = arg1 >= (*((s32 *) (((*(s32 *)&((map_map *)q)->contour_grids) + 0x14) + q)));  /* the contours' count */
  }
  else
  {
    if (1)
    {
    }
    c = arg1 >= 0;
  }
  if (c)
  {
    return 0;
  }
  base = ((u8 *)MapScene->map);
  off = *(s32 *)&((map_map *)base)->contour_grids;
  p = ((u8 *) (&((s32 *) ((base + 0x14) + off))[arg1 + 1])) + ((s32 *) ((base + 0x14) + off))[arg1 + 1];
  if ((new_var = p) != 0)
  {
    i = 0;
    a2 = p + 0x5C;
    a3 = p;
    ang = get_angle(arg0[0], arg0[2]) & 0xFFF;
    n = ((MapContour *)a3)->nEdges;
    loop:
    if (i < n)
    {
 do { r = ((MapContour *)a3)->edges[0].angle; a = get_angle(*((s32 *) (a2 - 0x14)), *((s32 *) (a2 - 0xC))) & 0xFFF; b = get_angle(*((s32 *) (a2 - 8)), *((s32 *) a2)) & 0xFFF; if ((a < ang) && (ang < b)) { *arg2 = -r; found = i; } a3 += 0x38; i++; a2 += 0x38; } while (0);
      goto loop;
    }

  }
  return found;
}

u32 RaceAI_FollowTrack(s32 player) {
    HeroInfo *h;
    s32 *ext;
    RaceFux *fux;
    s32 d;
    SLONG_XYZ pos;
    HeroInfo *base;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        fux = (RaceFux *)h->hero->FUX;
        if (fux != NULL) {
            d = (((csqrt((fux->vx * fux->vx >> 8) + (fux->vz * fux->vz >> 8)) >> 14) - 0x50) * 0x180) / 80;
            if (d < 0) {
                d = 0;
            }
            d += 0x200;
            pos.x = h->hero_eng->XYZ.x - (d * rcossin_tbl[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12);
            pos.y = 0;
            pos.z = h->hero_eng->XYZ.z - (d * rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2 >> 12);
            if (PointInZone(&pos.x, 0) != 0) {
                h->hero_ai->AIpad |= 0x80;
                h->hero_ai->AIpad &= ~0x8000;
            }
            pos.x = h->hero_eng->XYZ.x - (rcossin_tbl[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x79 >> 8);
            pos.y = 0;
            pos.z = h->hero_eng->XYZ.z - (rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2 * 0x79 >> 8);
            if (GameMode == 0x17 && (d = PointInZone(&pos.x, 3) != 1)) {
                h->hero_ai->AIpad |= 0x20;
                h->hero_ai->AIpad &= ~0x8000;
            }
            if (GameMode == 0x19) {
                if (pos.x * pos.x + pos.z * pos.z > 0x640000) {
                    Heroes[player].hero_ai->AIpad |= 0x20;
                    Heroes[player].hero_ai->AIpad &= ~0x8000;
                }
            }
            if (GameMode == 0x18) {
                if (pos.x * pos.x + pos.z * pos.z > 0x510000) {
                    Heroes[player].hero_ai->AIpad |= 0x20;
                    Heroes[player].hero_ai->AIpad &= ~0x8000;
                }
            }
            if (GameMode == 0x1A) {
                if (pos.x * pos.x + pos.z * pos.z > 0x640000) {
                    Heroes[player].hero_ai->AIpad |= 0x20;
                    Heroes[player].hero_ai->AIpad &= ~0x8000;
                }
            }
            EXT_W(ext, 7) = 0x20 - AiPlayerSkill[player].level;
        }
    }
    return 0x8000;
}

void RaceAI_SteerAhead(s32 player) {
    RaceFux *fux;
    SLONG_XYZ pos;
    s32 d;

    if (FindExtension(Heroes[player].hero, 0x20000000) != NULL) {
        fux = (RaceFux *)Heroes[player].hero->FUX;
        if (fux != NULL) {
            d = (((csqrt((fux->vx * fux->vx >> 8) + (fux->vz * fux->vz >> 8)) >> 14) - 0x50) << 10) / 80;
            if (d < 0) {
                d = 0;
            }
            d += 0xBE8;
            pos.x = Heroes[player].hero_eng->XYZ.x - (d * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12);
            pos.y = 0;
            pos.z = Heroes[player].hero_eng->XYZ.z - (d * rcossin_tbl[Heroes[player].hero_eng->xform.rot.vy & 0xFFF].unk2 >> 12);
            Heroes[player].hero_ai->AIpad |= 0x8000;
            Heroes[player].hero_ai->AIpad |= RaceAI_FollowTrack(player);
            Heroes[player].hero_ai->AIpad |= 0x800;
            ((PadState *)Pads)[player].dir = Heroes[player].hero_ai->AIpad;
        }
    }
}

void RaceAI_SteerTo(s32 player, SLONG_XYZ *pos) {
    HeroInfo *base;
    HeroInfo *hero;
    s32 d;

    base = Heroes;
    hero = base + player;
    d = (((get_angle(hero->hero_eng->XYZ.x - pos->x, hero->hero_eng->XYZ.z - pos->z) & 0xFFF) -
          hero->hero_eng->xform.rot.vy) << 20) >> 20;
    if (d > 500) {
        hero->hero_ai->AIpad |= 0x20;
        hero->hero_ai->AIpad &= ~0x80;
    }
    if (d < -500) {
        hero->hero_ai->AIpad |= 0x80;
        hero->hero_ai->AIpad &= ~0x20;
    }
    hero->hero_ai->AIpad |= 0x800;
    ((PadState *)Pads)[player].dir = hero->hero_ai->AIpad;
}

static inline NAS *RacePickupAheadInline(s32 arg0) {
    NAS *nas;
    HeroInfo *hero;
    s32 angle;

    nas = RaceNearestPickup(arg0);
    if (nas != NULL) {
        hero = &Heroes[arg0];
        if (DistXZSq((SLONG_XYZ *)&nas->Engine->XYZ, (SLONG_XYZ *)&hero->hero_eng->XYZ) <= 0x270FFF) {
            angle = get_angle(nas->Engine->XYZ.x, nas->Engine->XYZ.z) & 0xFFF;
            if ((get_angle(hero->hero_eng->XYZ.x, hero->hero_eng->XYZ.z) & 0xFFF) < angle) {
                return nas;
            }
        }
    }
    return NULL;
}

void RaceAI_ChasePickup(s32 arg0) {
    RaceAiExt *ext;
    NAS *nas;

    ext = FindExtension(Heroes[arg0].hero, 0x20000000);
    if (ext != NULL) {
        nas = RacePickupAheadInline(arg0);
        if (nas != NULL) {
            RaceAI_SteerTo(arg0, &nas->Engine->XYZ);
            return;
        }
        ext->unk18 = RaceAI_SteerAhead;
    }
}

void RaceInitSpinners(void) {
    subengine *obj;
    s32 i;
    u8 *r;

    RaceGlowObj = NULL;
    RaceGlowPhase = 0;
    RaceGlowMode = 0;
    obj = ((subengine *)MapScene->subs);
    for (i = 0; i < 6; i++) {
        RaceSpinnerBoxes[i] = 0;
        RacePistons[i].unk8 = 1;
        RacePistons[i].pos = 0;
        RacePistons[i].timer = 0;
        RacePistons[i].hold = 0;
        RacePistons[i].state = 0;
        RacePistons[i].active = 0;
        RacePatternTime = 0;
        RacePatternIdx = Random(5) + 0;
        RaceSpinTimer = 150;
        RaceSpinDelay = 180;
    }
    i = 0;
    for (;;) {
        if (obj == NULL) {
            return;
        }
        if (G63->mode == 0x17) {
            if (!(G63->levelFlags & 0x80)) {
                if (obj->entity == 0x5001) {
                    obj->flags &= ~0x8000;
                }
                if (obj->entity == 0x5003) {
                    obj->flags &= ~0x8000;
                }
            } else {
                if (obj->entity == 0x5001) {
                    obj->flags &= ~0x8000;
                }
                if (obj->entity == 0x5002) {
                    obj->flags &= ~0x8000;
                }
            }
        }
        if (G63->mode == 0x19 && obj->entity == 0x5003) {
            obj->flags &= ~0x8000;
        }
        if (G63->mode == 0x1A) {
            if (obj->entity == 0x5003) {
                obj->flags &= ~0x8000;
            }
            if (obj->entity == 0x4000) {
                printf("found lava\n");
                RaceGlowObj = (RaceObj74 *)obj;
            }
        }
        if (G63->mode == 0x18 && obj->u6C.dbase->data != NULL) {
            r = entity_address(obj->entity, obj->u6C.dbase->data);
            if (r != NULL && ((MeshHeader *)r)->normals != 0) {
                obj->flags &= ~0x8000;
            }
        }
        if (G63->mode == 0x19) {
            if (obj->entity == 0x5005) {
                RaceSpinner = (RaceSpinObj *)obj;
            }
            if (obj->entity == 0x4000) {
                RaceSpinnerBoxes[i] = obj;
                RacePistons[i].unkC = obj;
                obj->xform.rot.vy = i * 0x2AA;
                i++;
            }
        }
        obj = obj->next;
    }
}

s32 RacePistonPattern(RacePattern *pats, s32 idx) {
    RacePattern *pat;
    RacePatStep *step;
    RacePiston *p;
    s32 i;
    s32 j;
    s32 speed;
    s32 base;
    s32 one;
    s32 two;
    s32 st;
    s32 act;
    s32 v;

    speed = 2;
    pat = &pats[idx];
    step = pat->steps;
    base = 0x4000;
    if (GameLevelFlags & 0x80) {
        speed = 4;
    }
    RacePatternTime++;
    if (RaceOver != 0) {
        RacePatternTime = 0;
    }
    if (RacePatternTime > pat->length) {
        RacePatternTime = 0;
        RacePistonLimit = Random(0xC) + 0x11;
        for (j = 0; j < 6; j++) {
            RacePistons[j].active = 0;
            RacePistons[j].timer = 0;
        }
        return 1;
    }
    i = 0;
    one = 1;
    two = 2;
    p = RacePistons;
loop:
    if (p->active == 0 && step->delay != -1) {
        if (p->timer >= step->delay) {
            p->active = one;
            p->state = one;
            PlaySfx(0x673, NULL, NULL, 0x1000, 0x1E00, 0);
        } else {
            if (GameLevelFlags & 0x80) {
                v = p->timer + 4;
            } else {
                v = p->timer + 1;
            }
            p->timer = v;
        }
    }
    act = p->active;
    if (act == one) {
        if (RaceOver != 0) {
            p->state = two;
        }
        st = p->state;
        if (st == two) {
            goto down;
        }
        if (st < 3) {
            if (st == act) {
                goto up;
            }
            goto next;
        }
        if (st == 3) {
            goto hold;
        }
        goto next;
    up:
        p->pos += speed;
        if (RacePistonLimit < p->pos) {
            p->pos = RacePistonLimit;
            p->state = 3;
        }
        goto next;
    hold:
        if (step->hold < p->hold++) {
            p->hold = 0;
            p->state = two;
            PlaySfx(0x674, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        goto next;
    down:
        p->pos -= speed;
        if (p->pos < 0) {
            p->pos = 0;
            p->state = 0;
        }
    }
next:
    step++;
    ((RaceObj74 *)RaceSpinnerBoxes[i])->unk74 = p->pos + base;
    p++;
    if (++i < 6) {
        goto loop;
    }
    return 0;
}

void MaintainCentre(void) {
    MATRIX m;
    SVECTOR v;
    RaceSpinObj **pp;
    RaceSpinObj *o;
    s32 i;
    s32 spd;

    spd = 4;
    if ((u32)(RacePatternIdx - 1) < 2) {
        spd = -4;
    }
    if (GameLevelFlags & 0x80) {
        spd = -0x20;
    }
    if (RaceSpinner != NULL) {
        RaceSpinner->rot.vy -= spd;
        v.vx = RaceSpinner->rot.vx;
        v.vy = RaceSpinner->rot.vy;
        v.vz = RaceSpinner->rot.vz;
        RotMatrix(&v, &m);
        RaceSpinner->mat = m;
    }
    i = 0;
    pp = (RaceSpinObj **)RaceSpinnerBoxes;
loop:
    if (i < 6) {
        o = *pp;
        if (o != NULL) {
            o->rot.vy -= spd;
            v.vx = o->rot.vx;
            v.vy = o->rot.vy;
            v.vz = o->rot.vz;
            RotMatrix(&v, &m);
            o->mat = m;
        }
        pp++;
        i++;
        goto loop;
    }
    if (RaceSpinDelay < 0) {
        if (RaceOver != 0) {
            RacePatternIdx = 5;
        }
        if (RacePistonPattern(RacePistonPatterns, RacePatternIdx) != 0) {
            RacePatternIdx = Random(5);
        }
    } else {
        RaceSpinDelay--;
    }
}

void RaceInitBarrels(void) {
    RaceBarrel *b;
    subengine *obj;
    s32 n;

    n = 0;
    obj = ((subengine *)MapScene->subs);
    memset(RaceBarrels, 0x3600, 0);
    while (obj != NULL) {
        if (obj->entity == 0x5006) {
            b = &RaceBarrels[n];
            b->sub = obj;
            obj->xform.order = 2;
            b->move.mass = 0x80;
            b->move.x_vel = 0;
            b->move.z_vel = 0;
            b->move.z_acc = 0x500;
            b->move.x_acc = 0x500;
            b->move.z_dec = 0x500;
            b->move.x_dec = 0x500;
            b->move.x_max = 0;
            b->move.z_max = 0;
            b->startpos = *(SLONG_XYZ *)&obj->XYZ.x;
            b->bStatus = 1;
            n++;
        }
        obj = obj->next;
    }
    RaceBarrelCount = n;
    printf("%d  barrels found\n", n);
}

void RaceEaseCamStep(RaceEaseCam *c, s32 *pos) {
    s32 step;

    if (c->x.cur != c->x.target) {
        if (c->x.target != 0) {
            step = c->x.step;
        } else {
            step = c->x.stepZero;
        }
        if (c->x.cur < c->x.target) {
            c->x.cur += step;
            if (c->x.cur > c->x.target) {
                c->x.cur = c->x.target;
            }
        } else {
            c->x.cur -= step;
            if (c->x.cur < c->x.target) {
                c->x.cur = c->x.target;
            }
        }
    }
    if (c->z.cur != c->z.target) {
        if (c->z.target != 0) {
            step = c->z.step;
        } else {
            step = c->z.stepZero;
        }
        if (c->z.cur < c->z.target) {
            c->z.cur += step;
            if (c->z.cur > c->z.target) {
                c->z.cur = c->z.target;
            }
        } else {
            c->z.cur -= step;
            if (c->z.cur < c->z.target) {
                c->z.cur = c->z.target;
            }
        }
    }
    pos[1] += c->x.cur >> 8;
    pos[3] += c->z.cur >> 8;
}

s32 RaceBarrelHitHero(RaceBarrel *b) {
    EXTengine *e;
    subengine *obj;
    s32 hit;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 angle;
    s32 r;

    hit = 0;
    obj = b->sub;
    for (i = 0;; i++) {
        if (i >= 4) {
            return hit;
        }
        e = Heroes[i].hero_eng;
        dx = e->XYZ.x - obj->XYZ.x;
        dz = e->XYZ.z - obj->XYZ.z;
        d = dx * dx + dz * dz;
        if (d < 0x24000) {
            d = csqrt(d) >> 6;
            angle = get_angle(dx, dz) & 0xFFF;
            d = 0x180 - d;
            e->XYZ.x += d * rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 >> 12;
            e->XYZ.z += d * rcossin_tbl[angle].unk2 >> 12;
            RaceBarrelBump(i, b);
            if (Heroes[i].hero_mode != 0x61) {
                r = Random(2);
                if (r == 0) {
                    PlaySfx(0x663, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (r == 1) {
                    PlaySfx(0x664, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction(i, 0x5D);
                }
            }
            hit = 1;
        }
    }
}

s32 RaceBarrelSpaceFree(RaceBarrel *self) {
    RaceBarrel *b;
    subengine *obj;
    subengine *o;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 angle;

    b = RaceBarrels;
    i = 0;
    obj = self->sub;
    for (;; i++, b++) {
        if (i >= RaceBarrelCount) {
            return 1;
        }
        o = b->sub;
        if (b != self && b->bStatus == 1) {
            dx = obj->XYZ.x - o->XYZ.x;
            dz = obj->XYZ.z - o->XYZ.z;
            d = dx * dx + dz * dz;
            if (d < 0x24000) {
                d = csqrt(d) >> 6;
                angle = get_angle(dx, dz) & 0xFFF;
                d = 0x180 - d;
                obj->XYZ.x += d * rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 >> 12;
                obj->XYZ.z += d * rcossin_tbl[angle].unk2 >> 12;
            }
        }
    }
}

void RaceUpdateBarrels(void) {
    RaceBarrel *b;
    SLONG_XYZ pos;
    s32 i;

    b = RaceBarrels;
    for (i = 0;; b++, i++) {
        subengine *obj;

        if (i >= RaceBarrelCount) {
            break;
        }
        obj = b->sub;
        switch (b->bStatus) {
        case 1:
            b->move.x_max = 0;
            b->move.z_max = 0;
            if (RaceBarrelHitHero(b) != 0) {
                pos = *(SLONG_XYZ *)&obj->XYZ.x;
                pos.y = -0xD2;
                b->bStatus = 0;
                b->waittime = Random(0x3C) + 0xB4;
                obj->flags &= ~0x8000;
                RaceSpawnPuff(&pos, 1, &HeroModelBank[DB_CrateEffects]);
            }
            break;
        case 2:
            pos = *(SLONG_XYZ *)&obj->XYZ.x;
            pos.y = 0;
            if (b->waittime == 4) {
                RaceSpawnPuff(&pos, 9, NULL);
            }
            if (b->waittime-- < 0) {
                obj->flags |= 0x8000;
                b->bStatus = 1;
            }
            break;
        }
    }
}

s32 RaceTriHeight(SLONG_XYZ *pos, s16 *tri) {
    s32 d[3];
    s32 base[3];
    s32 e0[3];
    s32 e1[3];
    s32 a, b, c, dd, e, f, det, n;
    s32 t;
    s32 s;
    s32 *p;

    base[0] = tri[0];
    base[1] = tri[1];
    base[2] = tri[2];
    e1[0] = tri[4] - tri[0];
    e1[1] = tri[5] - tri[1];
    e1[2] = tri[6] - tri[2];
    e0[0] = tri[8] - tri[0];
    e0[1] = tri[9] - tri[1];
    e0[2] = tri[10] - tri[2];
    a = (e0[0] * e0[0] >> 8) + (e0[1] * e0[1] >> 8) + (e0[2] * e0[2] >> 8);
    b = (e0[0] * e1[0] >> 8) + (e0[1] * e1[1] >> 8) + (e0[2] * e1[2] >> 8);
    c = (e1[0] * e1[0] >> 8) + (e1[1] * e1[1] >> 8) + (e1[2] * e1[2] >> 8);
    d[0] = base[0] - pos->x;
    d[1] = base[1] - pos->y;
    d[2] = base[2] - pos->z;
    p = d;
    dd = (e0[0] * p[0] >> 8) + (e0[1] * p[1] >> 8) + (e0[2] * p[2] >> 8);
    e = (e1[0] * p[0] >> 8) + (e1[1] * p[1] >> 8) + (e1[2] * p[2] >> 8);
    det = (a * c >> 8) - (b * b >> 8);
    s = (b * e - c * dd) / det;
    t = (b * dd - a * e) / det;
    f = (p[0] * p[0] >> 8) + (p[1] * p[1] >> 8) + (p[2] * p[2] >> 8);
    if (s + t < 0x100) {
        if (s < 0) {
            if (t < 0) {
                if (dd < 0) {
                    s = -(dd << 8) / a;
                    t = 0;
                    if (s >= 0x100) {
                        s = 0xFF;
                    }
                } else {
                    s = 0;
                    if (e < 0) {
                        do {
                            t = -(e << 8) / c;
                        } while (0);
                        if (t >= 0x100) {
                            t = 0xFF;
                        }
                    } else {
                    t_zero:
                        t = 0;
                    }
                }
            } else {
                goto region3;
            }
        } else if (t < 0) {
            s = -(dd << 8) / a;
            t = 0;
            if (s < 0) {
                s = 0;
            } else if (s >= 0x100) {
                s = 0xFF;
            }
        }
    } else if (s < 0) {
        n = b - c + dd - e;
        if (n < 0) {
            s = -(n << 8) / (a - b * 2 + c);
            if (s < 0) {
                s = 0;
            } else if (s >= 0x100) {
                s = 0xFF;
            }
            t = 0xFF - s;
        } else if (c + e > 0) {
        region3:
            s = 0;
            t = -(e << 8) / c;
            if (t < 0) {
                goto t_zero;
            } else if (t >= 0x100) {
                t = 0xFF;
            }
        } else {
            s = 0;
            t = 0xFF;
        }
    } else if (t < 0) {
        n = a - b + dd - e;
        if (n > 0) {
            t = (n << 8) / (a - b * 2 + c);
            if (t < 0) {
                t = 0;
            } else if (t >= 0x100) {
                t = 0xFF;
            }
            s = 0xFF - t;
        } else if (a + dd > 0) {
            s = -(dd << 8) / a;
            t = 0;
            if (s < 0) {
                s = 0;
            } else if (s >= 0x100) {
                s = 0xFF;
            }
        } else {
            s = 0xFF;
            goto t_zero;
        }
    } else {
        s = -((b - c + dd - e) << 8) / (a - b * 2 + c);
        if (s < 0) {
            s = 0;
        } else if (s >= 0x100) {
            s = 0xFF;
        }
        t = 0xFF - s;
    }
    {
        s32 g;

        g = f << 8;
        return abs(s * ((a * s >> 8) + (b * t >> 8) + dd * 2)
                   + t * ((b * s >> 8) + (c * t >> 8) + e * 2) + g);
    }
}

s32 RaceFloorHeight(SLONG_XYZ *arg0, s32 *arg1) {
    RaceNorm out;
    SLONG_XYZ pos;
    RaceNorm n;
    s32 hit;
    subengine *obj;
    u8 *grp;
    s16 *tri;
    RaceNorm *nrm;
    s32 rad;
    s32 found;
    s32 d;
    s32 k;
    s32 dx;
    s32 dz;

    rad = 0xDC;
    found = 0;
    obj = ((subengine *)MapScene->subs);
    hit = 0;
    for (;;) {
        u8 *r;

        if (obj == NULL) {
            break;
        }
        if (!(obj->flags & 0x8000) && obj->u6C.dbase->data != NULL) {
            r = entity_address(obj->entity, obj->u6C.dbase->data);
            if (r != NULL && ((MeshHeader *)r)->normals != 0) {
                pos = *arg0;
                grp = r + (((MeshHeader *)r)->runs + 0x14);  /* its runs' self offset */
                tri = (s16 *)(r + ((MeshHeader *)r)->verts + 0x24);
                nrm = (RaceNorm *)(r + (((MeshHeader *)r)->normals + 0x28));  /* its normals' self offset */
                for (;;) {
                    s32 count;

                    count = grp[1];
                    if (count == 0xFF) {
                        break;
                    }
                    for (;;) {
                        s32 lim;

                        count--;
                        if (count == -1) {
                            break;
                        }
                        lim = pos.z + rad;
                        if (lim < tri[2] && lim < tri[6] && lim < tri[10]) {
                            goto next;
                        }
                        lim = pos.x - rad;
                        if (tri[0] < lim && tri[4] < lim && tri[8] < lim) {
                            goto next;
                        }
                        lim = pos.z - rad;
                        if (tri[2] < lim && tri[6] < lim && tri[10] < lim) {
                            goto next;
                        }
                        lim = pos.x + rad;
                        if (lim < tri[0] && lim < tri[4] && lim < tri[8]) {
                            goto next;
                        }
                        d = RaceTriHeight(&pos, tri);
                        if (rad * rad < d) {
                            goto next;
                        }
                        n = nrm[2];
                        k = (rad << 6) - csqrt(d) + 2;
                        dx = (k * n.x) >> 18;
                        dz = (k * n.z) >> 18;
                        arg0->x += dx;
                        arg0->z += dz;
                        hit = 1;
                        pos.x += dx;
                        pos.z += dz;
                        out = n;
                        if (!(G63->levelFlags & 0x80)) {
                            if (G63->mode == 0x17 && obj->entity == 0x5003) {
                                found = 1;
                            }
                        } else {
                            if (G63->mode == 0x17 && obj->entity == 0x5002) {
                                found = 1;
                            }
                        }
                    next:
                        tri += 4;
                        nrm++;
                    }
                    tri += 8;
                    nrm += 2;
                    grp += 2;
                }
            }
        }
        obj = obj->next;
    }
    if (found) {
        *arg1 = get_angle(out.x, out.z) & 0xFFF;
    } else {
        *arg1 = -1;
    }
    return hit;
}

s32 RaceTrackCollide(SLONG_XYZ *pos, RaceVec4 *out, s32 radius, s32 idx, s32 flags) {
    SLONG_XYZ p;
    SLONG_XYZ hit;
    SLONG_XYZ d;
    SLONG_XYZ e;
    RaceSegObj *obj;
    RaceSegObj *w;
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
    bad = idx >= (MapScene->dbase ?
                *(s32 *)(((u8 *)MapScene->map) + 0x14 + *(s32 *)(((u8 *)MapScene->map) + 0x14)) :
                  0);
    if (bad) {
        return 0;
    }
    data = ((u8 *)MapScene->map);
    obj = (RaceSegObj *)SELF_REL(&((s32 *)(data + (*(s32 *)&((map_map *)data)->contour_grids + 0x14)))[idx + 1]);
    if (obj != NULL) {
        i = 0;
        dist = 0;
        t = 0;
        w = obj;
        n = obj->count;
        r2 = rad * rad;
        for (;; i++, w = (RaceSegObj *)((RaceSeg *)w + 1)) {
            s32 lt;

            if (i >= n) {
                break;
            }
            ang = obj->segs[i].ang;
            if (flags & 1) {
                ang += 0x800;
            }
            d.x = pos->x - w->segs[0].p0.x;
            d.z = pos->z - w->segs[0].p0.z;
            if (((d.x * rcossin_tbl[-ang & 0xFFF].unk2) >> 12) -
                    ((d.z * rcossin_tbl[(-ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                continue;
            }
            d.y = w->segs[0].p1.y - w->segs[0].p0.y;
            e.x = w->segs[0].p1.x - w->segs[0].p0.x;
            e.y = w->segs[0].p1.y - w->segs[0].p0.y;
            e.z = w->segs[0].p1.z - w->segs[0].p0.z;
            len = (w->segs[0].len * w->segs[0].len) >> shift;
            if (len != 0) {
                t = (e.x * d.x + e.y * d.y + e.z * d.z) / len;
            }
            lt = t < 0x1000;
            if (t <= 0) {
                p = w->segs[0].p0;
                dx = p.x - pos->x;
                dz = p.z - pos->z;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (!lt) {
                p = w->segs[0].p1;
                dx = p.x - pos->x;
                dz = p.z - pos->z;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (t > 0 && lt) {
                p.x = ((t * e.x) >> shift) + w->segs[0].p0.x;
                p.y = ((t * e.y) >> shift) + w->segs[0].p0.y;
                p.z = ((t * e.z) >> shift) + w->segs[0].p0.z;
                dx = p.x - pos->x;
                dz = p.z - pos->z;
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
            hit.x = p.x;
            hit.y = p.y;
            hit.z = p.z;
            if (best > r2) {
                continue;
            }
            s = csqrt(best) >> 6;
            out->y = ang;
            a = -ang;
            v = rad - s + 2;
            out->x = (v * rcossin_tbl[a & 0xFFF].unk2) >> 12;
            ang = a + 0x400;
            out->z = (v * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
            pos->x += out->x;
            pos->z += out->z;
            found = 1;
            hit.y = obj->segs[i].ang;
        }
    }
    return found;
}

s32 RaceTrackCollideB(VECTOR *pos, RaceVec *out, s32 radius, s32 idx, s32 flags, RaceSegBlend *bl) {
    RaceVec p;
    RaceVec hit;
    RaceVec d;
    RaceVec e;
    SVECTOR rv;
    MATRIX m;
    VECTOR lp;
    RaceSegEnds a;
    RaceSegEnds b;
    RaceSegObj *w;
    RaceSegObj *w2;
    RaceSegObj *obj;
    u8 *data;
    RaceSegTable *tbl;
    RaceSegTable *tbl2;
    s32 rad;
    s32 best;
    s32 found;
    s32 n;
    s32 i;
    s32 one;
    s32 t;
    s32 dist;
    s32 ang;
    s32 r2;
    s32 len;
    s32 dx;
    s32 dz;
    s32 s;
    s32 v;
    s32 side;
    s32 bad;

    best = 0x7FFFFFFF;
    found = 0;
    rad = radius;
    one = 0x1000;
    bad = idx > (MapScene->dbase ?
                *(s32 *)(((u8 *)MapScene->map) + 0x14 + *(s32 *)(((u8 *)MapScene->map) + 0x14)) :
                 0);
    if (bad) {
        return 0;
    }
    data = ((u8 *)MapScene->map);
    tbl = (RaceSegTable *)(data + (*(s32 *)&((map_map *)data)->contour_grids + 0x14));
    w = (RaceSegObj *)((u8 *)&tbl->offs[idx] +
                        (tbl2 = (RaceSegTable *)((u8 *)data + *(s32 *)&((map_map *)data)->contour_grids + 0x14))->offs[idx]);
    if (w != NULL) {
        dist = 0;
        t = 0;
        w2 = (RaceSegObj *)((u8 *)&tbl->offs[bl->idx] + tbl2->offs[bl->idx]);
        obj = w;
        i = 0;
        n = w->count;
        r2 = rad * rad;
        rv.vx = 0;
        rv.vy = -bl->rot->ang;
        rv.vz = 0;
        RotMatrix(&rv, &m);
        ApplyMatrixLV(&m, pos, &lp);
        for (;; i++, w = (RaceSegObj *)((RaceSeg *)w + 1),
                w2 = (RaceSegObj *)((RaceSeg *)w2 + 1)) {
            s32 gt;

            if (i >= n) {
                break;
            }
            a.p0.vx = w->segs[0].p0.x;
            a.p0.vz = w->segs[0].p0.z;
            a.p1.vx = w->segs[0].p1.x;
            a.p1.vz = w->segs[0].p1.z;
            b.p0.vx = w2->segs[0].p0.x;
            b.p0.vz = w2->segs[0].p0.z;
            b.p1.vx = w2->segs[0].p1.x;
            b.p1.vz = w2->segs[0].p1.z;
            dx = b.p0.vx - a.p0.vx;
            a.p0.vx += (dx * (bl->t << 8)) / 30 >> 8;
            dz = b.p0.vz - a.p0.vz;
            a.p0.vz += (dz * (bl->t << 8)) / 30 >> 8;
            dx = b.p1.vx - a.p1.vx;
            a.p1.vx += (dx * (bl->t << 8)) / 30 >> 8;
            dz = b.p1.vz - a.p1.vz;
            a.p1.vz += (dz * (bl->t << 8)) / 30 >> 8;
            ang = obj->segs[i].ang;
            if (flags & 1) {
                ang += 0x800;
            }
            d.vx = lp.vx - a.p0.vx;
            d.vz = lp.vz - a.p0.vz;
            side = ((d.vx * rcossin_tbl[-ang & 0xFFF].unk2) >> 12) -
                ((d.vz * rcossin_tbl[(-ang - 0x400) & 0xFFF].unk2) >> 12);
            if (side < 0 || side > rad) {
                continue;
            }
            d.vy = 0;
            e.vx = a.p1.vx - a.p0.vx;
            e.vy = 0;
            e.vz = a.p1.vz - a.p0.vz;
            len = (e.vx * e.vx + e.vz * e.vz) >> 12;
            if (len != 0) {
                t = (e.vx * d.vx + e.vz * d.vz) / len;
            }
            if (t < 0) {
                p = a.p0;
                dx = p.vx - lp.vx;
                dz = p.vz - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            gt = one < t;
            if (gt) {
                p = a.p1;
                dx = p.vx - lp.vx;
                dz = p.vz - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (t >= 0 && !gt) {
                p.vx = ((t * e.vx) >> 12) + a.p0.vx;
                p.vy = ((t * e.vy) >> 12) + a.p0.vy;
                p.vz = ((t * e.vz) >> 12) + a.p0.vz;
                dx = p.vx - lp.vx;
                dz = p.vz - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (dist < 0) {
                dist = 0x7FFFFFFF;
                printf(RaceMsgError);
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
            s = csqrt(best);
            out->vy = ang + bl->rot->ang;
            ang = (-ang + 0x400) & 0xFFF;
            v = (rad << 6) - s + 2;
            out->vx = (v * rcossin_tbl[(((ang + bl->rot->ang) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 18;
            out->vz = (v * rcossin_tbl[(ang + bl->rot->ang) & 0xFFF].unk2) >> 18;
            pos->vx += out->vx;
            pos->vz += out->vz;
            found = 1;
            hit.vy = obj->segs[i].ang;
        }
    }
    return found;
}

s32 RaceTrackCollideC(VECTOR *pos, RaceVec *out, s32 radius, s32 idx, s32 flags, SVECTOR *rot) {
    SLONG_XYZ p;
    SLONG_XYZ hit;
    RaceVec d;
    RaceVec e;
    SVECTOR rv;
    MATRIX m;
    VECTOR lp;
    RaceSegObj *w;
    RaceSegObj *obj;
    u8 *data;
    RaceSegTable *tbl;
    s32 rad;
    s32 best;
    s32 found;
    s32 n;
    s32 i;
    s32 one;
    s32 t;
    s32 dist;
    s32 ang;
    s32 r2;
    s32 len;
    s32 dx;
    s32 dz;
    s32 s;
    s32 v;
    s32 side;
    s32 bad;

    best = 0x7FFFFFFF;
    found = 0;
    rad = radius;
    one = 0x1000;
    bad = idx > (MapScene->dbase ?
                *(s32 *)(((u8 *)MapScene->map) + 0x14 + *(s32 *)(((u8 *)MapScene->map) + 0x14)) :
                 0);
    if (bad) {
        return 0;
    }
    data = ((u8 *)MapScene->map);
    tbl = (RaceSegTable *)(data + (*(s32 *)&((map_map *)data)->contour_grids + 0x14));
    w = (RaceSegObj *)((u8 *)&tbl->offs[idx] +
                        ((RaceSegTable *)((u8 *)data + *(s32 *)&((map_map *)data)->contour_grids + 0x14))->offs[idx]);
    if (w != NULL) {
        i = 0;
        dist = 0;
        t = 0;
        obj = w;
        n = w->count;
        r2 = rad * rad;
        rv.vx = -rot->vx;
        rv.vy = -rot->vy;
        rv.vz = -rot->vz;
        RotMatrix(&rv, &m);
        ApplyMatrixLV(&m, pos, &lp);
        for (;; i++, w = (RaceSegObj *)((RaceSeg *)w + 1)) {
            s32 gt;

            if (i >= n) {
                break;
            }
            ang = obj->segs[i].ang;
            if (flags & 1) {
                ang += 0x800;
            }
            d.vx = lp.vx - w->segs[0].p0.x;
            d.vz = lp.vz - w->segs[0].p0.z;
            side = ((d.vx * rcossin_tbl[-ang & 0xFFF].unk2) >> 12) -
                ((d.vz * rcossin_tbl[(-ang - 0x400) & 0xFFF].unk2) >> 12);
            if (side < 0 || side > rad) {
                continue;
            }
            d.vy = 0;
            e.vx = w->segs[0].p1.x - w->segs[0].p0.x;
            e.vy = 0;
            e.vz = w->segs[0].p1.z - w->segs[0].p0.z;
            len = (w->segs[0].len * w->segs[0].len) >> 12;
            if (len != 0) {
                t = (e.vx * d.vx + e.vz * d.vz) / len;
            }
            if (t < 0) {
                p = w->segs[0].p0;
                dx = p.x - lp.vx;
                dz = p.z - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            gt = one < t;
            if (gt) {
                p = w->segs[0].p1;
                dx = p.x - lp.vx;
                dz = p.z - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (t >= 0 && !gt) {
                p.x = ((t * e.vx) >> 12) + w->segs[0].p0.x;
                p.y = ((t * e.vy) >> 12) + w->segs[0].p0.y;
                p.z = ((t * e.vz) >> 12) + w->segs[0].p0.z;
                dx = p.x - lp.vx;
                dz = p.z - lp.vz;
                dist = dx * dx + dz * dz;
                if (dist > r2) {
                    continue;
                }
            }
            if (dist < 0) {
                dist = 0x7FFFFFFF;
                printf(RaceMsgError);
            }
            if (dist > best) {
                continue;
            }
            best = dist;
            hit.x = p.x;
            hit.y = p.y;
            hit.z = p.z;
            if (best > r2) {
                continue;
            }
            s = csqrt(best);
            out->vy = ang;
            pos->vx += out->vx;
            pos->vz += out->vz;
            ang = (-ang + 0x400) & 0xFFF;
            v = (rad << 6) - s + 2;
            out->vx = (v * rcossin_tbl[(((ang + rot->vy) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 18;
            out->vz = (v * rcossin_tbl[(ang + rot->vy) & 0xFFF].unk2) >> 18;
            found = 1;
            hit.y = obj->segs[i].ang;
            if (flags & 2) {
                return 1;
            }
        }
    }
    return found;
}

/* a debug printf format; its place in .rodata is between functions' jump tables, so it is
 * defined here */
const char RaceMsgError[] = "error\n";

s32 RaceXZBoxToSphere(void *arg0, SLONG_XYZ *arg1, s32 arg2, SLONG_XYZ *arg3) {
    VECTOR d;
    VECTOR n;
    MATRIX m;
    SVECTOR ang;
    SVECTOR sv[2];
    RaceBoxHalf lo;
    RaceBoxHalf hi;
    RaceBox *src;
    SVECTOR *p;
    s32 dist;
    s32 side;
    s32 *ap;

    ap = (s32 *)arg0;
    dist = 0x7FFFFFF;
    side = 0;
    src = (RaceBox *)SubFindFlagPoint((subengine *)arg0, side, side);
    p = sv;
    if (src == NULL) {
        printf("No Coll box on subengine in RaceXZBoxToSphere\n");
        return side;
    }
    p->pad = 0x100;
    if (arg2 == 2) {
        p->pad = 0x200;
    }
    sv[0].vx = 0;
    p->vy = 0;
    p->vz = 0;
    lo = src->lo;
    hi = src->hi;
    d.vx = arg1->x - ap[1];
    d.vy = arg1->y - ap[2];
    d.vz = arg1->z - ap[3];
    ang.vx = -((u16 *)arg0)[8];
    ang.vy = -((u16 *)arg0)[9];
    ang.vz = -((u16 *)arg0)[10];
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
        if (arg2 == 0) {
            s32 push;

            push = p->pad - dist;
            push = RaceBoxPushRot(&ang, (u16 *)arg0, abs(push));
            RotMatrix(&ang, &m);
            ApplyMatrixLV(&m, &n, &n);
            arg3->x += (push * n.vx) >> 12;
            arg3->y += (push * n.vy) >> 12;
            arg3->z += (push * n.vz) >> 12;
        }
        return side;
    }
    return -1;
}

void InitialiseRACE(void) {
    s32 i;
    RaceAiExt *ai;
    EXTengine *eng;
    subengine *sub;
    GameState *g;
    EXTheromove *move;
    EXTengine *he;
    s32 *scene;

    RaceStartDelay = 0x3C;
    Init_Clock(0x140, 0x28, 0);
    if (G63->mode == 0x18) {
        Set_Clock((G63->levelFlags & 0x40) ? 0x115 : 0x130, 0x81C0);
    }
    if (G63->mode == 0x17) {
        Set_Clock((G63->levelFlags & 0x40) ? 0x100 : 0x130, 0x81C0);
    }
    if (G63->mode == 0x19) {
        Set_Clock((G63->levelFlags & 0x40) ? 0x100 : 0x130, 0x81C0);
    }
    if (G63->mode == 0x1A) {
        Set_Clock((G63->levelFlags & 0x40) ? 0x100 : 0x130, 0x81C0);
    }
    RaceObjects = NULL;
    World_floor_level = -0x10;
    RaceInitStub();
    scene = ((s32 *)MapScene->dbase);
    RaceTagTimer = 0;
    dbase_load_all_mesh_anims(scene);
    RaceInitSpinners();
    RaceOver = 0;
    g = G63B;
    if (g->mode == 0x18) {
        RaceInitBarrels();
        CreateNAS(0, 0xC4, 0, 0x170B, 0x11);
        PlaySfx(0x42E, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (g->mode == 0x19) {
        PlaySfx(0x672, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    RaceBarrelRollTimer = 0xB4;
    i = 0;
    for (;; i++) {
        RaceFux *fux;

        if (i >= 4) {
            break;
        }
        printf("hero status %d\n", Heroes[i].hero_status);
        RaceInitHeroMove(i);
        Heroes[i].hero_eng->xform.order = 1;
        fux = AddExt(Heroes[i].hero, sizeof(RaceFux), 0x10000000);
        if (fux == NULL) {
            continue;
        }
        he = Heroes[i].hero_eng;
        move = Heroes[i].hero_move;
        he->xform.rot.vy = 0;
        move->forward_angle = 0;
        Heroes[i].hero_eng->xform.dirty = 1;
        sub = Heroes[i].hero_eng->sub;
        sub->flags |= 0x8000;
        ai = AddExt(Heroes[i].hero, sizeof(RaceAiExt), 0x20000000);
        if (ai != NULL) {
            ai->unkC = 0;
            ai->angle = Heroes[i].hero_move->forward_angle;
            ai->unk18 = RaceAI_SteerAhead;
            ai->unk1C = 0;
            ai->speed = RaceAISpeeds[Heroes[i].hero_name];
        }
        Heroes[i].hero->FUX = (EXT *)fux;
        fux->unkC = 0;
        fux->unk14 = 0;
        fux->mode = 0;
        fux->eng = NULL;
        fux->unk20 = 0;
        fux->timer2 = 0;
        fux->timer = 0;
        fux->cross = 0;
        fux->unk38 = 0;
        fux->laps = 0;
        fux->timer4 = 0;
        if ((G63B->levelFlags & 0x40) && G63B->heroCpu[i] != 0) {
            fux->laps = 2;
        }
        fux->side = -1;
        RacePickupTimer = 0;
        fux->hits = 1;
        fux->count = 0;
        fux->unk44 = 0;
        fux->timer22 = 0;
        fux->timer3 = 0;
        fux->unk50 = 0;
        fux->steer = 0;
        fux->flags = 0;
        fux->shield = 0;
        fux->vx = 0;
        fux->vz = 0;
        fux->started = 0;
        RaceUnused1 = 0;
        Heroes[i].hero_eng->XYZ.y = RaceFloorLevel;
        fux->decel = RaceDecel;
        fux->maxSpeed = RaceMoveTune5;
        fux->jumpVel = RaceJumpVel;
        fux->tune1 = RaceMoveTune1;
        fux->accel = RaceAccel;
        fux->turnRate = RaceMoveTune4;
        fux->bounce2 = 0x100;
        RaceTimer = 0;
        eng = AddExtEngine(Heroes[i].hero, 8);
        if (eng != NULL) {
            fux->eng = eng;
            eng->sub->flags &= ~0x8000;
            eng->xform.hasScale = 1;
            eng->xform.order = 0;
            eng->xform.scale.vx = 0x800;
            eng->xform.scale.vy = 0x800;
            eng->xform.scale.vz = 0x800;
        }
    }
    if (G63->mode == 0x1A) {
        if (G63->levelFlags & 0x80) {
            CreateNAS(0, -0x400, 0, 0x170E, 0x11);
        } else {
            CreateNAS(0, 0, 0, 0x1707, 0x11);
        }
    }
}

void UpdateRACE(void) {
    RaceTimer++;
    if (G63->exitFlags & 0x800) {
        RaceOver = 1;
    }
    if (G63->exitFlags & 1) {
        RaceOver = 1;
    }
    if (G63->pause == 0) {
        RaceStartDelay--;
    }
    if (RaceOver == 0) {
        CameraMode = 0;
    } else {
        CameraMode = 1;
    }
    RacePickupMaster();
    if (G63->mode != 0x19) {
        RacePickupRespawn();
    }
    if (G63->mode == 0x18) {
        RaceUpdateBarrels();
    }
    if (G63->mode == 0x19) {
        MaintainCentre();
    }
    if (G63->mode == 0x1A && !(G63->levelFlags & 0x80)) {
        RaceGlowObj->unk74 = (RaceGlowPhase >> 8) + 0x4000;
        if (RaceGlowMode == 0) {
            RaceGlowPhase += 0x80;
            if (RaceGlowPhase >= 0x6000) {
                RaceGlowPhase = 0;
            }
        }
        if (RaceGlowMode == 1) {
            RaceGlowPhase -= 0x19C;
            if (RaceGlowPhase <= 0) {
                RaceGlowPhase = 0;
                RaceGlowMode = 0;
            }
        }
    }
}

void RaceInitHeroMove(s32 arg0) {
    EXTheromove *m;

    m = Heroes[arg0].hero_move;
    if (m != NULL) {
        if (GameMode == 0x19) {
            RaceAccel = 0x258;
            RaceMoveTune1 = 0x640;
            RaceMoveTune2 = 0xC80;
            RaceDecel = 0x7D;
            RaceMoveTune3 = 0x190;
            RaceMoveTune4 = 0x64;
            RaceJumpVel = 0x3E;
            RaceMoveTune5 = 0x7D;
        }
        m->mass = 0x100;
        m->x_acc = RaceAccel;
        m->z_acc = RaceAccel;
        m->z_dec = RaceDecel;
        m->x_dec = RaceDecel;
        m->velocity_calc = 3;
    }
}

void RacePreMoveHero(s32 player) {
    HeroInfo *heroes;
    HeroInfo *h;
    RaceFux *fux;
    EXTheromove *m;
    s32 *steers;
    s32 *steer;
    PadState *pad;
    PadState *pads;
    s32 speed;
    s32 boost;

    heroes = Heroes;
    h = heroes + player;
    fux = (RaceFux *)h->hero->FUX;
    if (fux != NULL) {
        m = h->hero_move;
        if (m != NULL) {
            steers = RaceSteer;
            steer = steers + player;
            m->z_max = 0;
            m->x_max = 0;
            *steer = 0;
            if (h->hero_mode != 0x60 && (m = h->hero_move) != NULL && h->hero_frozen == 0) {
                pads = Pads;
                pad = pads + player;
                if ((pad->flags & 1) || h->hero_ai->AImode != 0) {
                    if (((PadState *)pad)->dir & 0xA0) {
                        speed = ((csqrt(fux->vx * fux->vx + fux->vz * fux->vz) * 4 >> 8) * 0x3C8) / 100 >> 8;
                        if (speed > 0x3C8) {
                            speed = 0x3C8;
                        }
                        if (((PadState *)pad)->dir & 0x80) {
                            m->forward_angle -= fux->turnRate;
                            *steer = speed;
                        }
                        if (((PadState *)pad)->dir & 0x20) {
                            m->forward_angle += fux->turnRate;
                            *steer = -speed;
                        }
                    }
                    boost = 0;
                    if ((((PadState *)Pads)[player].dir & 0x8000) && fux->steer > 0) {
                        if (G63B->mode == 0x18 && (G63B->levelFlags & 0x80) && G63B->heroCpu[player] == 0 && fux->timer5 == 0 && fux->timer4 > 0) {
                            fux->vx >>= 3;
                            fux->vz >>= 3;
                        }
                        m->x_acc = fux->tune1;
                        m->z_acc = fux->tune1;
                        m->x_max = fux->maxSpeed * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy + 0x400) & 0xFFF].unk2 >> 12;
                        m->z_max = fux->maxSpeed * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy + 0x800) & 0xFFF].unk2 >> 12;
                        if (AiPlayerSkill[player].level >= 0x17 && G63B->heroCpu[player] != 0) {
                            fux->steer -= 0x180 - RaceSteerAngles[AiPlayerSkill[player].level - 0x16];
                        } else if (Heroes[player].hero_mode != 0x61) {
                            fux->steer -= 0x180;
                        }
                        boost = 1;
                        if (fux->steer < 0) {
                            fux->steer = 0;
                            boost = 0;
                        }
                    }
                    if (boost == 0 && (((PadState *)Pads)[player].dir & 0x800)) {
                        m->x_acc = fux->accel;
                        m->z_acc = fux->accel;
                        if (G63B->heroCpu[player] != 0) {
                            if (AiPlayerSkill[player].level < 0x16) {
                                m->x_acc = RaceAccel - ((0x10 - AiPlayerSkill[player].level) << 6);
                                m->z_acc = RaceAccel - ((0x10 - AiPlayerSkill[player].level) << 6);
                                if (m->x_acc < 0x200) {
                                    m->x_acc = 0x200;
                                }
                                if (m->z_acc < 0x200) {
                                    m->z_acc = 0x200;
                                }
                            } else {
                                m->x_acc = RaceAccel + RaceSteerSpeeds[AiPlayerSkill[player].level - 0x16];
                                m->z_acc = RaceAccel + RaceSteerSpeeds[AiPlayerSkill[player].level - 0x16];
                            }
                        }
                        if (G63B->mode == 0x18 && (G63B->levelFlags & 0x80) && G63B->heroCpu[player] == 0 && fux->timer5 == 0 && fux->timer4 > 0) {
                            fux->vx >>= 3;
                            fux->vz >>= 3;
                        }
                        m->x_max = fux->jumpVel * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy + 0x400) & 0xFFF].unk2 >> 12;
                        m->z_max = fux->jumpVel * rcossin_tbl[(Heroes[player].hero_eng->xform.rot.vy + 0x800) & 0xFFF].unk2 >> 12;
                    }
                }
            }
            RaceHeroCollide(player);
            if (Heroes[player].hero_status & 0x200) {
                m->z_dir = 0;
                m->x_max = 0;
                m->z_max = 0;
                fux->vx = 0;
                fux->vz = 0;
            }
            angle_diff(m->forward_angle, m->facing_angle);
            m->facing_angle = m->forward_angle;
        }
    }
    RaceInitHeroAngle(player);
}

s32 RaceKeepOnTrack(s32 arg0, s32 arg1) {
    EXTengine *eng;
    s32 dist;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        dist = csqrt(eng->XYZ.x * eng->XYZ.x + eng->XYZ.z * eng->XYZ.z) >> 6;
        if (arg1 == 0) {
            if (dist > 0xB00) {
                s32 ang = get_angle(eng->XYZ.x, eng->XYZ.z) & 0xFFF;
                s32 c = rcossin_tbl[(ang - 0x400) & 0xFFF].unk2;
                s32 d = dist - 0xAFC;
                eng->XYZ.x -= d * c >> 12;
                eng->XYZ.z -= d * rcossin_tbl[ang].unk2 >> 12;
                return 1;
            }
        } else {
            if (dist < 0x464) {
                s32 ang = get_angle(eng->XYZ.x, eng->XYZ.z) & 0xFFF;
                s32 c = rcossin_tbl[(ang - 0x400) & 0xFFF].unk2;
                s32 d = dist - 0x460;
                eng->XYZ.x -= d * c >> 12;
                eng->XYZ.z -= d * rcossin_tbl[ang].unk2 >> 12;
                return 1;
            }
        }
    }
    return 0;
}

void RaceMoveHero(s32 player) {
    SLONG_XYZ pos;
    RaceVec4 out;
    s32 ang0;
    s32 ang;
    s32 zacc;
    s32 step;
    RaceFux *fux;
    EXTheromove *move;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *h;
    HeroInfo *h2;
    HeroInfo *hb;
    HeroInfo *hb2;
    s32 pz;
    s32 vx;
    s32 vz;
    s32 a; /* x target speed, then the bounce angle, then the sfx pick */
    s32 b; /* z target speed, then the hit angle, then the pass counter */
    s32 xacc;
    s32 i;
    s32 d;
    s32 dist;
    s32 dx;
    s32 dz;
    s32 ga;
    s32 lim;
    s32 hit;

    hb = Heroes;
    h = hb + player;
    fux = (RaceFux *)h->hero->FUX;
    if (fux != NULL) {
        pos = h->hero_eng->XYZ;
        vx = fux->vx;
        vz = fux->vz;
        eng = h->hero_eng;
        move = h->hero_move;
        a = move->x_max << 8;
        b = move->z_max << 8;
        xacc = move->x_acc;
        zacc = move->z_acc;
        step = fux->decel;
        pos.y = RaceFloorLevel;
        RaceFloorHeight(&pos, &ang0);
        if (move->velocity_calc != 3) {
            return;
        }
        if (vx != a) {
            d = step;
            if (a != 0) {
                d = xacc;
            }
            if (vx < a) {
                vx += d;
                if (vx > a) {
                    vx = a;
                }
            } else {
                vx -= d;
                if (vx < a) {
                    vx = a;
                }
            }
        }
        if (vz != b) {
            d = step;
            if (b != 0) {
                d = zacc;
            }
            if (vz < b) {
                vz += d;
                if (vz > b) {
                    vz = b;
                }
            } else {
                vz -= d;
                if (vz < b) {
                    vz = b;
                }
            }
        }
        eng->OldXYZ = eng->XYZ;
        if (G63->mode == 0x19) {
            i = 0;
            while (1) {
                if (i >= 6) {
                    break;
                }
                if (RaceXZBoxToSphere(RaceSpinnerBoxes[i], &Heroes[player].hero_eng->XYZ, 0, &pos) == 4 &&
                    RacePistons[i].state == 1) {
                    ga = get_angle(eng->OldXYZ.x - pos.x, eng->OldXYZ.z - pos.z) & 0xFFF;
                    vx = -(rcossin_tbl[(ga - 0x400) & 0xFFF].unk2 * 6);
                    vz = -(rcossin_tbl[ga].unk2 * 6);
                }
                i++;
            }
        }
        if (vx < -0xC800) {
            vx = -0xC800;
        }
        if (vx > 0xC800) {
            vx = 0xC800;
        }
        if (vz < -0xC800) {
            vz = -0xC800;
        }
        if (vz > 0xC800) {
            vz = 0xC800;
        }
        fux->vx = vx;
        fux->vz = vz;
        dx = vx >> 8;
        pos.x += dx;
        dz = vz >> 8;
        pos.z += dz;
        if (RaceFloorHeight(&pos, &ang) != 0) {
            move->x_max = 0;
            move->z_max = 0;
            if (ang != -1) {
                b = ((get_angle(dx, dz) & 0xFFF) - ang) & 0xFFF;
                if (b > 0x800) {
                    a = ang - 0x338;
                } else {
                    a = ang + 0x338;
                }
                dist = csqrt(vx * vx + vz * vz) >> 6;
                dist = dist * 200 >> 8;
                fux->vx = dist * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12;
                fux->vz = dist * rcossin_tbl[a & 0xFFF].unk2 >> 12;
                Heroes[player].hero_move->forward_angle = a + 0x800;
                a = Random(4);
                if (fux->vx != 0 || fux->vz != 0) {
                    lim = 0x600;
                    if (b > lim) {
                        if (b < 0xA00) {
                            if (a == 0) {
                                PlaySfx(0x64C, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (a == 1) {
                                PlaySfx(0x64D, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (a == 2) {
                                PlaySfx(0x64E, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (a == 3) {
                                PlaySfx(0x64F, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                        }
                    }
                }
            }
        }
        do {
            hb2 = Heroes;
            h2 = hb2 + player;
        } while (0);
        e = h2->hero_eng;
        pz = pos.z;
        h2->hero_eng->XYZ.x += pos.x - e->XYZ.x;
        h2->hero_eng->XYZ.z += pz - e->XYZ.z;
        if (!(h2->hero_status & 0x200)) {
            b = 0;
            while (1) {
                if (b >= 2) {
                    break;
                }
                if (b == 1) {
                    if (G63->mode != 0x18 && G63->mode != 0x1A) {
                        hit = RaceTrackCollide(&pos, &out, 200, 1, 0);
                    } else {
                        hit = Heroes[player].hero_eng->XYZ.x * Heroes[player].hero_eng->XYZ.x +
                                  Heroes[player].hero_eng->XYZ.z * Heroes[player].hero_eng->XYZ.z >
                            (G63->mode == 0x1A ? 0xA90000 : 0x900000);
                    }
                    if (hit && Heroes[player].hero_mode != 0x61) {
                        SetHeroAction(player, 0x61);
                    }
                }
                b++;
            }
        }
        RaceCountLaps(player);
    }
}

void RaceQuatNormalize(Quat *q) {
    s32 n;
    s32 sw;
    s32 sz;
    s32 sx;
    s32 sy;

    sw = q->v[3] * q->v[3];
    sz = q->v[2] * q->v[2];
    sx = q->v[0] * q->v[0];
    sy = q->v[1] * q->v[1];
    n = ((sx >= 0 ? sx + 0x800 : sx - 0x800) >> 12)
      + ((q->v[1] * q->v[1] >= 0 ? sy + 0x800 : sy - 0x800) >> 12)
      + ((q->v[2] * q->v[2] >= 0 ? sz + 0x800 : sz - 0x800) >> 12)
      + ((q->v[3] * q->v[3] >= 0 ? sw + 0x800 : sw - 0x800) >> 12);
    q->v[0] = (q->v[0] << 12) / n;
    q->v[1] = (q->v[1] << 12) / n;
    q->v[2] = (q->v[2] << 12) / n;
    q->v[3] = (q->v[3] << 12) / n;
}

void RaceEulerToQuat(VECTOR *ang, Quat *q) {
    s32 x, y, z;
    s32 sx, cx, sy, cy, sz, cz;
    s32 p, a, b, c, d;
    s32 m0, m1, m2, m3;
    s32 t;

    x = ang->vx >> 1;
    y = ang->vy >> 1;
    z = ang->vz >> 1;
    sx = rcossin_tbl[x & 0xFFF].unk2;
    cx = rcossin_tbl[(x - 0x400) & 0xFFF].unk2;
    sy = rcossin_tbl[y & 0xFFF].unk2;
    cy = rcossin_tbl[(y - 0x400) & 0xFFF].unk2;
    sz = rcossin_tbl[z & 0xFFF].unk2;
    cz = rcossin_tbl[(z - 0x400) & 0xFFF].unk2;
    a = RaceFixMul(sx, sz);
    b = RaceFixMul(sx, cz);
    c = RaceFixMul(cx, sz);
    d = RaceFixMul(cx, cz);
    p = sy * c;
    m0 = cy * b;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t -= (cy * b >= 0 ? m0 + 0x800 : m0 - 0x800) >> 12;
    q->v[0] = t;
    p = sy * d;
    m1 = cy * a;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t += (cy * a >= 0 ? m1 + 0x800 : m1 - 0x800) >> 12;
    q->v[1] = t;
    p = sy * b;
    m2 = cy * c;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t -= (cy * c >= 0 ? m2 + 0x800 : m2 - 0x800) >> 12;
    q->v[2] = t;
    p = sy * a;
    m3 = cy * d;
    p = ((p >= 0 ? p + 0x800 : p - 0x800) >> 12) + ((cy * d >= 0 ? m3 + 0x800 : m3 - 0x800) >> 12);
    q->v[3] = p;
    RaceQuatNormalize(q);
}

void RaceHeroIdle(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    PadState *pad;
    PadState *pb;
    EXTengine *e;
    SLONG_XYZ pos;
    HeroInfo *base;
    HeroInfo *h1;
    HeroInfo *hb;
    HeroInfo *hn;
    HeroInfo *h2;
    s32 spawned;

    base = Heroes;
    h1 = &base[player];
    h1->hero_eng->xform.rot.vx -= h1->hero_eng->xform.rot.vx >> 1;
    f = (RaceFux *)h1->hero->FUX;
    if (f == NULL) {
        return;
    }
    if (G63B->mode == 0x18) {
        if (f->timer4 > 0) {
            f->timer4--;
        }
        if ((G63B->levelFlags & 0x80) && G63B->heroCpu[player] == 0) {
            if (f->timer5-- < 0) {
                f->timer5 = (150 - f->timer4) / 5;
            }
        }
    }
    hb = Heroes;
    h2 = &hb[player];
    if (h2->hero_move == NULL) {
        return;
    }
    h2->hero_eng->xform.dirty = 1;
    spawned = 0;
    if (f->timer2 == 0) {
        pb = (PadState *)Pads;
        pad = &pb[player];
        if (pad->dir & 0x8000) {
            if (f->steer > 0) {
                if (f->flag == 0) {
                    if (GameMode == 0x19) {
                        if (player == 0) {
                            PlaySfx(0x679, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (player == 1) {
                            PlaySfx(0x67A, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (player == 2) {
                            PlaySfx(0x67B, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (player == 3) {
                            PlaySfx(0x67C, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                    }
                    SetHeroSeq(player, 0x49);
                    if (GameMode == 0x19) {
                        SetHeroSeq(player, 0x53);
                        hn = Heroes;
                        h = &hn[player];
                        if (h->hero_name == 1) {
                            PlaySfx(0x67D, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (h->hero_name == 3) {
                            PlaySfx(0x67F, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (h->hero_name == 5) {
                            PlaySfx(0x681, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (h->hero_name == 7) {
                            PlaySfx(0x684, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                    }
                    if (player == 0) {
                        PlaySfx(0x648, &f->sfx, &f->sfxPos, 0x1000, 0x1E00, 0);
                    }
                    if (player == 1) {
                        PlaySfx(0x649, &f->sfx, &f->sfxPos, 0x1000, 0x1E00, 0);
                    }
                    if (player == 2) {
                        PlaySfx(0x64A, &f->sfx, &f->sfxPos, 0x1000, 0x1E00, 0);
                    }
                    if (player == 3) {
                        PlaySfx(0x64B, &f->sfx, &f->sfxPos, 0x1000, 0x1E00, 0);
                    }
                    f->flag = 1;
                }
                {
                    HeroInfo *hb;
                    HeroInfo *h;

                    hb = Heroes;
                    h = &hb[player];
                    e = h->hero_eng;
                    pos.x = e->XYZ.x + (rcossin_tbl[(e->xform.rot.vy - 0x400) & 0xFFF].unk2 * 25 >> 9);
                    e = h->hero_eng;
                    pos.z = e->XYZ.z + (rcossin_tbl[e->xform.rot.vy & 0xFFF].unk2 * 25 >> 9);
                    pos.y = h->hero_eng->XYZ.y;
                }
                if (--f->timer22 == -1) {
                    if (G63B->mode == 0x18 && (G63B->levelFlags & 0x80) && G63B->heroCpu[player] == 0) {
                        if (f->timer4 > 0) {
                            CreateNAS(pos.x, pos.y, pos.z, 0x1710, 0x11);
                        } else {
                            CreateNAS(pos.x, pos.y, pos.z, 0x170A, 0x11);
                        }
                    } else if (GameMode != 0x19) {
                        CreateNAS(pos.x, pos.y, pos.z, 0x170A, 0x11);
                    } else {
                        CreateNAS(pos.x, pos.y, pos.z, 0x1711, 0x11);
                    }
                    spawned = 1;
                    f->timer22 = 4;
                }
            } else {
                if (f->sfx != NULL) {
                    StopSfx(f->sfx);
                }
                pad->dirPressed &= ~0x8000;
            }
        } else {
            if (f->sfx != NULL) {
                StopSfx(f->sfx);
            }
            f->flag = 0;
        }
        if (spawned == 0) {
            if (((PadState *)Pads)[player].dir & 0x800) {
                if (f->flag2 == 0) {
                    if (player == 0) {
                        PlaySfx(0x644, &Heroes[0].hero_eng->sfx, &Heroes[0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 1) {
                        PlaySfx(0x645, &Heroes[1].hero_eng->sfx, &Heroes[1].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 2) {
                        PlaySfx(0x646, &Heroes[2].hero_eng->sfx, &Heroes[2].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 3) {
                        PlaySfx(0x647, &Heroes[3].hero_eng->sfx, &Heroes[3].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    f->flag2 = 1;
                }
                {
                    HeroInfo *hb;
                    HeroInfo *h;

                    hb = Heroes;
                    h = &hb[player];
                    e = h->hero_eng;
                    pos.x = e->XYZ.x + (rcossin_tbl[(e->xform.rot.vy - 0x400) & 0xFFF].unk2 * 25 >> 9);
                    e = h->hero_eng;
                    pos.z = e->XYZ.z + (rcossin_tbl[e->xform.rot.vy & 0xFFF].unk2 * 25 >> 9);
                    pos.y = h->hero_eng->XYZ.y;
                }
                if (--f->timer3 == -1) {
                    if (G63B->mode == 0x18 && (G63B->levelFlags & 0x80) && G63B->heroCpu[player] == 0) {
                        if (f->timer4 > 0) {
                            CreateNAS(pos.x, pos.y, pos.z, 0x170F, 0x11);
                        } else {
                            CreateNAS(pos.x, pos.y, pos.z, 0x1709, 0x11);
                        }
                    } else if (GameMode != 0x19) {
                        CreateNAS(pos.x, pos.y, pos.z, 0x1709, 0x11);
                    } else {
                        CreateNAS(pos.x, pos.y, pos.z, 0x1711, 0x11);
                    }
                    f->timer3 = 6;
                }
            } else {
                if (Heroes[player].hero_eng->sfx != NULL) {
                    StopSfx(Heroes[player].hero_eng->sfx);
                }
                f->flag2 = 0;
            }
        }
    } else {
        if (--f->timer3 == -1) {
            EXTengine *e2;

            e2 = h2->hero_eng;
            pos.x = e2->XYZ.x + (rcossin_tbl[(e2->xform.rot.vy - 0x400) & 0xFFF].unk2 * 25 >> 9);
            e2 = h2->hero_eng;
            pos.z = e2->XYZ.z + (rcossin_tbl[e2->xform.rot.vy & 0xFFF].unk2 * 25 >> 9);
            pos.y = h2->hero_eng->XYZ.y;
            if (GameMode != 0x19) {
                CreateNAS(pos.x, pos.y, pos.z, 0x170A, 0x11);
            } else {
                CreateNAS(pos.x, pos.y, pos.z, 0x1711, 0x11);
            }
            f->timer3 = 3;
        }
    }
    if (G63B->mode == 0x1A) {
        RaceFloorLevel = -0xB4;
    }
    Heroes[player].hero_eng->XYZ.y = RaceFloorLevel;
    RaceHeroUsePickup(player);
    if (G63B->mode == 0x19) {
        RaceHeroTestJump(player);
    }
    TestRaceHeroWon(player);
    if (G63B->mode != 0x19) {
        if (ProcessHeroAnim(player) != 0) {
            SetHeroSeq(player, 0x48);
        }
    } else {
        ProcessHeroAnim(player);
    }
}

void SetRaceHeroOffTrack(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    h->hero_eng->xform.rot.vx = 0;
    h->hero_eng->xform.dirty = 1;
    TestRaceHeroWon(player);
    f = (RaceFux *)h->hero->FUX;
    if (f != NULL) {
        f->angle = get_angle(h->hero_eng->XYZ.x, h->hero_eng->XYZ.z) & 0xFFF;
        f->bounce.accel = 0x300;
        f->bounce.vel = 0;
        f->bounce.flags = 2;
        f->bounce.damp = 100;
        f->bounces = 0;
        f->timer2 = 0;
        f->flag2 = 0;
        f->flag3 = 0;
        f->bounce.limit = RaceFloorLevel;
        if (h->hero_eng->sfx != NULL) {
            StopSfx(h->hero_eng->sfx);
        }
        if (f->sfx != NULL) {
            StopSfx(f->sfx);
        }
        f->mode = 0;
        if (player == 0) {
            PlaySfx(0x650, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 1) {
            PlaySfx(0x651, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 2) {
            PlaySfx(0x652, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 3) {
            PlaySfx(0x653, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        h->hero_eng->xform.dirty = 1;
        SetHeroSeq(player, 0x4B);
        if (G63->mode == 0x19) {
            SetHeroSeq(player, 0x54);
        }
        h->hero_shadow->sub->flags &= ~0x8000;
        h->hero_eng->sub->otz = 0x200;
        h->hero_eng->sub->flags |= 0x02000000;
        if (G63->mode == 0x18 && (G63->levelFlags & 0x80)) {
            f->timer4 = 150;
        }
    }
}

/* The splash a dive leaves in the water (RaceHeroOffTrack). */
static inline void RaceDiveSplash(SLONG_XYZ *pos, s32 kind) {
    NAS *fx;
    RaceFux *x;

    fx = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (fx != NULL) {
        x = FindExtension(fx, 0x10000000);
        if (x != NULL) {
            x->unkC = kind;
            x->mode = 0x100;
            x->unk20 = 1;
            x->unk28 = 0;
            fx->InitDelete(fx);
        }
    }
}

void RaceHeroOffTrack(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    EXTheromove *m;
    EXTengine *e;
    SLONG_XYZ sp;
    HeroInfo *base;
    HeroInfo *hb;
    HeroInfo *h2;

    if (TestRaceHeroWon(player) != 0) {
        return;
    }
    base = Heroes;
    h = &base[player];
    f = (RaceFux *)h->hero->FUX;
    if (f == NULL) {
        return;
    }
    m = h->hero_move;
    if (m != NULL) {
        m->x_max = 0;
        m->z_max = 0;
        m->x_vel = 0;
        m->z_vel = 0;
        f->vx = 0;
        f->vz = 0;
    }
    if (f->flag3 != 0) {
        h->hero_shadow->sub->flags |= 0x8000;
        h->hero_eng->sub->otz = 0;
        ProcessHeroAnim(player);
        h->hero_move->forward_angle = ((f->angle - 0x200) << 20) >> 20;
        h->hero_eng->xform.rot.vy = ((f->angle - 0x200) << 20) >> 20;
        h->hero_eng->xform.rot.vz = 0;
        h->hero_frozen = 2;
        h->hero_eng->xform.rot.vx -= h->hero_eng->xform.rot.vx >> 1;
        m = h->hero_move;
        if (m != NULL) {
            m->x_max = 0;
            m->z_max = 0;
            m->x_vel = 0;
            m->z_vel = 0;
            f->vx = 0;
            f->vz = 0;
        }
        if (RaceBounceObject((RaceBounceObj *)h->hero_eng, &f->bounce) != 0 && ++f->bounces >= 2) {
            SetHeroAction(player, 0x5B);
            if (G63B->mode == 0x18 && G63B->heroCpu[player] == 0 && (G63B->levelFlags & 0x80)) {
                PlaySfx(0x66A, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            return;
        }
        if (f->bounces > 0) {
            if (((PadState *)Pads)[player].dir & 0x8000) {
                if (f->flag2 == 0) {
                    if (player == 0) {
                        PlaySfx(0x644, &Heroes[0].hero_eng->sfx, &Heroes[0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 1) {
                        PlaySfx(0x645, &Heroes[1].hero_eng->sfx, &Heroes[1].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 2) {
                        PlaySfx(0x646, &Heroes[2].hero_eng->sfx, &Heroes[2].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    if (player == 3) {
                        PlaySfx(0x647, &Heroes[3].hero_eng->sfx, &Heroes[3].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                    }
                    f->flag2 = 1;
                }
                if (--f->timer22 == -1) {
                    hb = Heroes;
                    h2 = &hb[player];
                    e = h2->hero_eng;
                    sp.x = e->XYZ.x + (rcossin_tbl[(e->xform.rot.vy - 0x400) & 0xFFF].unk2 * 256 >> 12);
                    e = h2->hero_eng;
                    sp.z = e->XYZ.z + (rcossin_tbl[e->xform.rot.vy & 0xFFF].unk2 * 256 >> 12);
                    sp.y = h2->hero_eng->XYZ.y;
                    CreateNAS(sp.x, sp.y, sp.z, 0x1709, 0x11);
                    f->timer22 = 4;
                }
            } else {
                if (Heroes[player].hero_eng->sfx != NULL) {
                    StopSfx(Heroes[player].hero_eng->sfx);
                }
                f->flag2 = 0;
            }
        }
    } else {
        m = h->hero_move;
        if (m != NULL) {
            m->x_max = 0;
            m->z_max = 0;
            m->x_vel = 0;
            m->z_vel = 0;
            f->vx = 0;
            f->vz = 0;
        }
        if (ProcessHeroAnim(player) != 0) {
            f->flag3 = 1;
            SetHeroSeq(player, 0x48);
            if (G63->mode == 0x19) {
                SetHeroSeq(player, 0x52);
            }
            h->hero_eng->XYZ.x -= rcossin_tbl[(f->angle - 0x400) & 0xFFF].unk2 * 512 >> 12;
            h->hero_eng->XYZ.z -= rcossin_tbl[f->angle & 0xFFF].unk2 * 512 >> 12;
            h->hero_eng->XYZ.y = -0x578;
            h->hero_eng->xform.rot.vz = 0;
            if (G63->mode == 0x17) {
                RaceDiveSplash(&h->hero_eng->XYZ, 9);
            }
            if (GameMode == 0x18) {
                RaceDiveSplash(&Heroes[player].hero_eng->XYZ, 9);
            }
            if (GameMode == 0x19) {
                RaceDiveSplash(&Heroes[player].hero_eng->XYZ, 5);
            }
            if (GameMode == 0x1A) {
                RaceDiveSplash(&Heroes[player].hero_eng->XYZ, 9);
            }
            PlaySfx(0x655, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    Heroes[player].hero_eng->xform.dirty = 1;
}

void SetRaceHeroRun(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    EXTheromove *m;
    HeroInfo *base;
    u16 spd;

    base = Heroes;
    h = &base[player];
    f = (RaceFux *)h->hero->FUX;
    if (f != NULL) {
        m = h->hero_move;
        if (m != NULL) {
            spd = HeroHooks.RunSpeeds[h->hero_name * 3];
            m->x_vel = (spd * rcossin_tbl[(h->hero_eng->xform.rot.vy + 0x400) & 0xFFF].unk2) >> 12;
            m->z_vel = (spd * rcossin_tbl[(h->hero_eng->xform.rot.vy + 0x800) & 0xFFF].unk2) >> 12;
        }
        f->count = 0;
    }
}

void RaceHeroRun(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    EXTheromove *m;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    h->hero_eng->XYZ.y = 0;
    f = (RaceFux *)h->hero->FUX;
    if (f == NULL) {
        return;
    }
    m = h->hero_move;
    if (m == NULL) {
        return;
    }
    h->hero_eng->XYZ.y = 0;
    if (++f->count > 20) {
        SetHeroAction(player, 0x5B);
        if (GameMode == 0x19) {
            SetHeroSeq(player, 0x52);
        }
        return;
    }
    if (((PadState *)Pads)[player].dir & 0x8000) {
        m->x_acc = 0xC;
        m->z_acc = 0xC;
        m->x_max = (RaceJumpVel * rcossin_tbl[(h->hero_eng->xform.rot.vy + 0x400) & 0xFFF].unk2) >> 12;
        m->z_max = (RaceJumpVel * rcossin_tbl[(h->hero_eng->xform.rot.vy + 0x800) & 0xFFF].unk2) >> 12;
    }
    ProcessHeroAnim(player);
    RaceHeroUsePickup(player);
}

void SetRaceHeroDazed(s32 player) {
    HeroInfo *h;
    RaceSpawnExt *f;
    EXTheromove *m;
    HeroInfo *base;
    s32 r;

    TestRaceHeroWon(player);
    if (G63->mode != 0x19) {
        SetHeroSeq(player, 0x4A);
    }
    pad_rumble_small(player, 0xA);
    ((void (*)(s32, s32, s32))pad_rumble_big)(player, 0xA, 0xFF);
    base = Heroes;
    h = &base[player];
    f = (RaceSpawnExt *)h->hero->FUX;
    if (f != NULL) {
        f->unkC = 0x3C;
        m = h->hero_move;
        if (m != NULL) {
            m->jump_acc = 0xA;
            m->jump_dec = 0x14;
            m->jump_vel = -0xA0;
        }
        if (G63->mode != 0x19) {
            switch (h->hero_name) {
            case 1:
                RACE_SFX4(3, 4, 5, 6);
                break;
            case 2:
                RACE_SFX4(0x35, 0x36, 0x37, 0x38);
                break;
            case 3:
                RACE_SFX4(0x67, 0x68, 0x69, 0x6A);
                break;
            case 4:
                RACE_SFX4(0x99, 0x9A, 0x9B, 0x9C);
                break;
            case 5:
                RACE_SFX4(0x12F, 0x130, 0x131, 0x132);
                break;
            case 7:
                RACE_SFX4(0xFD, 0xFE, 0xFF, 0x100);
                break;
            case 8:
                RACE_SFX4(0xCB, 0xCC, 0xCD, 0xCE);
                break;
            case 6:
                RACE_SFX4(0x161, 0x162, 0x163, 0x164);
                break;
            }
        } else {
            switch (h->hero_name) {
            case 1:
                PlaySfx(0x687, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x686, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x688, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0x685, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x68C, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x68B, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0x689, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x68A, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            }
        }
    }
}

void RaceHeroDazed(s32 arg0) {
    HeroInfo *hero;
    EXTraceshield *fux;
    PadState *pad;
    PadState *pb;
    HeroInfo *base;

    TestRaceHeroWon(arg0);
    base = Heroes;
    hero = &base[arg0];
    fux = (EXTraceshield *)hero->hero->FUX;
    if (fux == NULL) {
        return;
    }
    pb = (PadState *)Pads;
    pad = &pb[arg0];
    pad->dir = 0;
    pad->dirPressed = 0;
    if (GameMode != 0x19) {
        ProcessHeroAnim(arg0);
    }
    hero->hero_frozen = 2;
    hero->hero_eng->xform.dirty = 1;
    if (RaceJumpAbove(arg0)) {
        Heroes[arg0].hero_eng->XYZ.y -= abs(RaceFloorLevel - Heroes[arg0].hero_eng->XYZ.y);
        Heroes[arg0].hero_eng->xform.rot.vx -= Heroes[arg0].hero_eng->xform.rot.vx >> 1;
        Heroes[arg0].hero_eng->xform.rot.vy += 0x100;
    } else {
        Heroes[arg0].hero_eng->xform.rot.vx += 0x80;
    }
    if (fux->state-- < 0) {
        SetHeroAction(arg0, 0x5B);
    }
}

void SetRaceHeroJump(s32 player) {
    HeroInfo *h;
    RaceFux *f;
    EXTheromove *m;
    SLONG_XYZ pos;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    f = (RaceFux *)h->hero->FUX;
    if (f != NULL) {
        f->phase = 0;
        m = h->hero_move;
        if (m != NULL) {
            pos.x = h->hero_eng->XYZ.x + (rcossin_tbl[(h->hero_eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 25 >> 9);
            pos.z = h->hero_eng->XYZ.z + (rcossin_tbl[h->hero_eng->xform.rot.vy & 0xFFF].unk2 * 25 >> 9);
            pos.y = h->hero_eng->XYZ.y;
            m->jump_acc = 5;
            m->jump_dec = 10;
            m->jump_vel = -0x44;
            if (GameMode == 0x19) {
                if (player == 0) {
                    PlaySfx(0x68D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 1) {
                    PlaySfx(0x68E, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 2) {
                    PlaySfx(0x68F, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 3) {
                    PlaySfx(0x690, NULL, NULL, 0x1000, 0x1E00, 0);
                }
            }
        }
    }
}

void RaceHeroJump(s32 player) {
    RaceFux *fux;
    EXTheromove *move;

    ProcessHeroAnim(player);
    move = Heroes[player].hero_move;
    if (move == NULL) {
        return;
    }
    fux = (RaceFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (fux->phase == 0 && RaceJumpAbove(player)) {
        move->jump_vel = -0x20;
        fux->phase = 1;
    }
    if (fux->phase == 1 && RaceJumpAbove(player)) {
        Heroes[player].hero_eng->XYZ.y -= abs(RaceFloorLevel - Heroes[player].hero_eng->XYZ.y);
        SetHeroAction(player, 0x5B);
    }
}

void RaceHeroOiled(s32 arg0) {
    RaceFux *fux;
    HeroInfo *hero;
    EXTheromove *move;
    HeroInfo *base;

    base = Heroes;
    hero = &base[arg0];
    fux = (RaceFux *)hero->hero->FUX;
    if (fux == NULL) {
        return;
    }
    ProcessHeroAnim(arg0);
    RaceHeroUsePickup(arg0);
    move = hero->hero_move;
    if (move == NULL) {
        return;
    }
    hero->hero_frozen = 1000;
    hero->hero_eng->xform.dirty = 1;
    if (fux->timer2-- < 0) {
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = 0;
        move->z_vel = 0;
        if (fux->timer2 < -0xF) {
            SetHeroAction(arg0, 0x5B);
            hero->hero_frozen = 0;
        }
    } else {
        do {
            hero->hero_eng->xform.rot.vy += 0x100;
        } while (0);
    }
}

void RaceHeroKick(s32 arg0) {
    HeroInfo *hero;
    EXTraceshield *ext;
    s32 lim;
    EXTengine *eng;
    EXTengine *eng2;
    EXTheromove *move;
    HeroInfo *other;
    u32 i;
    s32 found;
    s32 t;
    s32 angle;
    HeroInfo *base;
    SLONG_XYZ *pos;

    base = Heroes;
    hero = &base[arg0];
    ext = (EXTraceshield *)hero->hero->FUX;
    if (ext == NULL) {
        return;
    }
    eng = hero->hero_eng;
    t = ext->players[0]--;
    if (t < 0) {
        SetHeroAction(arg0, 0x5B);
        return;
    }
    pos = &eng->XYZ;
    i = 0;
    lim = 0x27100;
    other = base;
loop:
    found = -1;
    if (i < 4) {
        if (i == arg0 || DistXZSq((SLONG_XYZ *)pos, (SLONG_XYZ *)&other->hero_eng->XYZ) >= lim) {
            other++;
            i++;
            goto loop;
        }
        found = i;
    }
    if (found != -1) {
        move = Heroes[found].hero_move;
        if (move != NULL) {
            eng2 = Heroes[found].hero_eng;
            angle = get_angle(eng->XYZ.x - eng2->XYZ.x, eng->XYZ.z - eng2->XYZ.z) & 0xFFF;
            if ((u32)(angle - 0x201) < 0xBFF) {
                move->x_vel = -((rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 3) >> 7);
                move->z_vel = -((rcossin_tbl[angle].unk2 * 3) >> 7);
            }
        }
    }
    ProcessHeroAnim(arg0);
}

s32 RaceHeroUsePickup(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;
    HeroInfo *other;
    RaceFux *fux;
    PadState *pb;
    PadState *pad;
    EXTengine *eng;
    NAS *nas;
    RaceSpawnExt *ext;
    SLONG_XYZ pos;
    SLONG_XYZ *pp;
    s32 st;
    s32 lim;
    u32 i;
    s32 found;
    s32 t;

    base = Heroes;
    hero = &base[player];
    fux = (RaceFux *)hero->hero->FUX;
    if (fux != NULL) {
        pb = (PadState *)Pads;
        pad = &pb[player];
        st = pad->dir;
        eng = hero->hero_eng;
        if (st & 0x2200) {
            t = fux->mode;
            if (t == 1) {
                RaceFireMissile(player);
                fux->mode &= ~1;
                return 1;
            } else if (t == 0x20) {
                nas = CreateNAS(eng->XYZ.x, eng->XYZ.y + 0x60, eng->XYZ.z, 0x1708, 0x11);
                if (nas != NULL) {
                    ext = FindExtension(nas, 0x10000000);
                    if (ext != NULL) {
                        fux->flags = 1;
                        ext->unk14 = player;
                    }
                }
                fux->mode &= ~0x20;
                return 1;
            } else if (t == 2) {
                pp = &pos;
                pos.y = 0;
                pos.x = eng->XYZ.x + ((rcossin_tbl[(eng->xform.rot.vy - 0x400) & 0xFFF].unk2 * 75) >> 11);
                pos.z = eng->XYZ.z + ((rcossin_tbl[eng->xform.rot.vy & 0xFFF].unk2 * 75) >> 11);
                i = 0;
                lim = 0x1DE84;
                other = base;
            loop:
                found = -1;
                if (i < 4) {
                    if (i == player || DistXZSq((SLONG_XYZ *)pp, (SLONG_XYZ *)&other->hero_eng->XYZ) >= lim) {
                        other++;
                        i++;
                        goto loop;
                    }
                    do {
                        found = i;
                    } while (0);
                }
                if (found == -1) {
                    CreateNAS(pos.x, 0, pos.z, 0x1703, 0x11);
                    fux->mode &= ~2;
                    return 1;
                }
            } else if (t == 4) {
                nas = CreateNAS(eng->XYZ.x, -0x20, eng->XYZ.z, 0x1704, 0x11);
                if (nas != NULL) {
                    ext = FindExtension(nas, 0x10000000);
                    if (ext != NULL) {
                        ext->unkC = player;
                    }
                }
                fux->mode &= ~4;
                return 1;
            }
        } else {
            pad->dir = st & ~0x2000;
        }
    }
    return 0;
}

s32 RaceHeroTestJump(s32 arg0) {
    PadState *base;
    PadState *pad;
    PadState *b1;
    PadState *p1;
    s32 st;

    if (Heroes[arg0].hero->FUX != NULL) {
        if (GAME->heroCpu[arg0] != 0) {
            b1 = (PadState *)Pads;
            p1 = &b1[arg0];
            st = p1->dir;
            if (!(st & 0x4000)) {
                p1->dir = st & ~0x4000;
                return 0;
            }
            goto act;
        }
        base = (PadState *)Pads;
        pad = &base[arg0];
        if (pad->dirPressed & 0x4000) {
act:
            SetHeroAction(arg0, 0x5E);
            return 1;
        }
        pad->dir &= ~0x4000;
        return 0;
    }
    return 0;
}

s32 RaceHeroTestOil(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + arg0;

    if (PointInZone((s32 *)&hero->hero_eng->XYZ, 2) == 0 &&
        PointInZone((s32 *)&hero->hero_eng->XYZ, 3) == 0) {
        return 0;
    }
    if (hero->hero_mode != 0x60) {
        SetHeroAction(arg0, 0x60);
    }
    return 1;
}

void SetRaceHeroWinner(s32 player) {
    HeroInfo *hero;
    HeroInfo *base;

    RaceOver = 1;
    StartHeroVictory(player);
    SetHeroSeq(player, 0x48);
    base = Heroes;
    hero = base + player;
    hero->hero_eng->xform.rot.vz = 0;
    hero->hero_eng->xform.rot.vx = 0;
    hero->hero_eng->XYZ.y = RaceFloorLevel;
    hero->hero_eng->xform.dirty = 1;
    if (GameMode != 0x19) {
        RequestHeroAnim(player, 0);
        return;
    }
    RequestHeroAnim(player, 1);
    prims_empty_back();
    mal_compact_step(&MainHeap);
}

void RaceChooseWinnerTeam(s32 arg0, s32 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    GameState *g;

    if (!(GameExitFlags & 0x801) && arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score < Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            GAME->adventure.rank[j] = best;
            taken |= 1 << best;
        }
        g = GameInfo;
        if (g->teams == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g->heroCpu[g->adventure.rank[0]] != 0
                || (hb = Heroes, a = &hb[g->adventure.rank[0]], a->hero_score == hb[g->adventure.rank[1]].hero_score)) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    Heroes[k].hero_status |= 0x100;
                    if (GAME->heroCpu[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                    }
                }
            } else {
                GAME->winner[0] = a->hero_name;
                GAME->exitFlags |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (GAME->adventure.rank[0] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x8400;
                        hb[n].hero_shadow->sub->flags &= ~0x8000;
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

            ia = g->teamLeader[0];
            ib = g->teamLeader[1];
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            if (Heroes[ia].hero_score == 0 && GAME->heroCpu[ia] == 0) {
                GAME->exitFlags |= 0x800;
                RACE_TEAM_WIN(ia, ita, ib, itb);
            } else if (Heroes[ita].hero_score == 0 && GAME->heroCpu[ia] == 0) {
                GAME->exitFlags |= 0x800;
                RACE_TEAM_WIN(ia, ita, ib, itb);
            } else if (Heroes[ib].hero_score == 0 && GAME->heroCpu[ib] == 0) {
                GAME->exitFlags |= 0x800;
                RACE_TEAM_WIN(ib, itb, ia, ita);
            } else if (Heroes[itb].hero_score == 0 && GAME->heroCpu[ib] == 0) {
                GAME->exitFlags |= 0x800;
                RACE_TEAM_WIN(ib, itb, ia, ita);
            } else if (Heroes[ia].hero_score == 0 && GAME->heroCpu[ia] != 0) {
                RACE_DRAW(p, arg1);
            } else if (Heroes[ita].hero_score == 0 && GAME->heroCpu[ia] != 0) {
                RACE_DRAW(p, arg1);
            } else if (Heroes[ib].hero_score == 0 && GAME->heroCpu[ib] != 0) {
                RACE_DRAW(p, arg1);
            } else if (Heroes[itb].hero_score == 0 && GAME->heroCpu[ib] != 0) {
                RACE_DRAW(p, arg1);
            } else {
                sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
                sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
                if (sa == sb) {
                    RACE_DRAW(p, arg1);
                } else if ((sa < sb && GAME->heroCpu[ia] == 0)
                           || (sb < sa && GAME->heroCpu[ib] == 0)) {
                    GameState *w = GAME;

                    w->exitFlags |= 0x800;
                    if (w->heroCpu[ia] == 0) {
                        RACE_TEAM_WIN(ia, ita, ib, itb);
                    } else {
                        RACE_TEAM_WIN(ib, itb, ia, ita);
                    }
                } else {
                    RACE_DRAW(p, arg1);
                }
            }
        }
    }
}

void RaceChooseWinner(s32 arg0, u16 arg1) {
    s32 i;
    GameState *g;
    HeroInfo *hb;
    GameState *g0;

    g0 = RV;
    if (!(g0->exitFlags & 0x801) && arg0 == -1) {
        for (i = 0; ; i++) {
            RaceFux *f;

            if (i >= 4) {
                break;
            }
            RV->adventure.rank[i] = i;
            f = (RaceFux *)Heroes[i].hero->FUX;
            if (f != NULL) {
                if (f->sfx != NULL) {
                    StopSfx(f->sfx);
                }
                if (Heroes[i].hero_eng->sfx != NULL) {
                    StopSfx(Heroes[i].hero_eng->sfx);
                }
            }
        }
        {
        HeroInfo *hp;

        i = 0;
        hp = Heroes;
        for (; ; i++) {
            s32 j;

            if (i >= 4) {
                break;
            }
            for (j = 1; ; j++) {
                s32 a;
                s32 b;

                if (j >= 4) {
                    break;
                }
                a = RV->adventure.rank[j - 1];
                b = RV->adventure.rank[j];
                if (hp[a].hero_score > hp[b].hero_score) {
                    RV->adventure.rank[j - 1] = b;
                    RV->adventure.rank[j] = a;
                }
            }
        }
        }
        g = RV;
        if (g->teams == 0) {
            HeroInfo *a;

            hb = Heroes;
            a = &hb[g->adventure.rank[0]];
            if (a->hero_score == hb[g->adventure.rank[1]].hero_score) {
                RACE_DRAW_I(g, i, arg1);
            } else {
                RV->winner[0] = a->hero_name;
                RV->exitFlags |= 0x800;
                for (i = 0; i < 4; i++) {
                    if (RV->adventure.rank[0] == i) {
                        hb[i].hero_status |= 0x200;
                    } else {
                        hb[i].hero_status |= 0x8400;
                        hb[i].hero_shadow->sub->flags &= ~0x8000;
                    }
                }
            }
        } else {
            s32 ia;
            s32 ib;
            s32 ita;
            s32 itb;
            s32 sa;
            s32 sta;
            s32 sb;
            s32 stb;
            s32 mb;
            s32 ma;
            s32 n;

            ia = g->teamLeader[0];
            ib = g->teamLeader[1];
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            if (Heroes[ia].hero_score == 0) {
                g->exitFlags |= 0x800;
                RACE_TEAM_WIN(ia, ita, ib, itb);
            } else if (Heroes[ita].hero_score == 0) {
                g->exitFlags |= 0x800;
                RACE_TEAM_WIN(ia, ita, ib, itb);
            } else if (Heroes[ib].hero_score == 0) {
                g->exitFlags |= 0x800;
                RACE_TEAM_WIN(ib, itb, ia, ita);
            } else if (Heroes[itb].hero_score == 0) {
                g->exitFlags |= 0x800;
                RACE_TEAM_WIN(ib, itb, ia, ita);
            } else {
                sa = (Heroes[g->teamLeader[0]].hero_score << 12) + RaceFacing(ia);
                sta = (Heroes[Heroes[g->teamLeader[0]].hero_teammate].hero_score << 12)
                    + RaceFacing(Heroes[g->teamLeader[0]].hero_teammate);
                sb = (Heroes[g->teamLeader[1]].hero_score << 12) + RaceFacing(g->teamLeader[1]);
                stb = (Heroes[Heroes[g->teamLeader[1]].hero_teammate].hero_score << 12)
                    + RaceFacing(Heroes[g->teamLeader[1]].hero_teammate);
                for (n = 0; ; n++) {
                    RaceFux *f;

                    if (n >= 4) {
                        break;
                    }
                    f = (RaceFux *)Heroes[n].hero->FUX;
                    if (f != NULL) {
                        if (n == 0) {
                            sa -= f->started << 12;
                        }
                        if (n == 1) {
                            sta -= f->started << 12;
                        }
                        if (n == 2) {
                            sb -= f->started << 12;
                        }
                        if (n == 3) {
                            stb -= f->started << 12;
                        }
                    }
                }
                ma = (sa < sta) ? sa : sta;
                mb = (sb < stb) ? sb : stb;
                if (ma == mb) {
                    RACE_DRAW_I(RV, i, arg1);
                } else if (ma < mb) {
                    RACE_TEAM_WIN_BY(RV, Heroes, RV->teamLeader[0], RV->teamLeader[1]);
                } else {
                    RACE_TEAM_WIN_BY(RV, Heroes, RV->teamLeader[1], RV->teamLeader[0]);
                }
            }
        }
    }
}

void RaceBarrelBump(s32 player, RaceBarrel *b) {
    RaceFux *f;
    s32 ma;
    s32 mb;
    s32 xa;
    s32 xb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = Heroes[player].hero_move->mass;
    mb = b->move.mass;
    if (ma > 0 && mb > 0) {
        f = (RaceFux *)Heroes[player].hero->FUX;
        if (f != NULL) {
            xa = f->vx >> 8;
            xb = b->move.x_vel >> 8;
            d = -((xa - xb) << 8);
            xb = (ma * xa + mb * xb) << 8;
            q = (d * mb + xb) / (total = mb + ma);
            xa = f->vz >> 8;
            xb = b->move.z_vel >> 8;
            d = -((xa - xb) << 8);
            r = (d * mb + ((ma * xa + mb * xb) << 8)) / total;
            f->vx = q;
            b->move.x_vel = q;
            f->vz = r;
            b->move.z_vel = r - d;
        }
    }
}

#define RACE_CLAMP_SPEED(v, lim)                                           \
    if ((v) < 0) {                                                          \
        if ((v) < ((lim) = -(RaceMoveTune5 << 8))) {                           \
            (v) = (lim);                                                    \
        }                                                                   \
    } else if (((lim) = RaceMoveTune5 << 8) < (v)) {                           \
        (v) = (lim);                                                        \
    }

void RaceHeroesBump(s32 pa, s32 pb) {
    RaceFux *fa;
    RaceFux *fb;
    s32 ma;
    s32 mb;
    s32 e;
    s32 az;
    s32 bz;
    s32 x;
    s32 d;
    s32 q;
    s32 r;
    s32 lim;
    s32 t;
    s32 k;

    ma = Heroes[pa].hero_move->mass;
    mb = Heroes[pb].hero_move->mass;
    if (ma > 0 && mb > 0) {
        fa = (RaceFux *)Heroes[pa].hero->FUX;
        if (fa != NULL) {
            fb = (RaceFux *)Heroes[pb].hero->FUX;
            e = fa->bounce2;
            if (fb != NULL) {
                pa = fa->vx;
                az = fa->vz;
                pb = fb->vx;
                bz = fb->vz;
                RACE_CLAMP_SPEED(pa, lim);
                RACE_CLAMP_SPEED(az, lim);
                RACE_CLAMP_SPEED(pb, lim);
                RACE_CLAMP_SPEED(bz, lim);
                t = pa;
                pa = mb + ma;
                x = t >> 8;
                t = pb;
                pb = t >> 8;
                d = -(e * (x - pb + ((t << 8) & 0xFF)));
                r = (ma * x + mb * pb) << 8;
                q = (d * mb + r) / pa;
                t = az;
                x = t >> 8;
                k = (t << 8) & 0xFF;
                t = bz;
                pb = t >> 8;
                az = e * (x - pb + k + ((t << 8) & 0xFF));
                r = (ma * x + mb * pb) << 8;
                pb = q - d;
                d = -az;
                r = (d * mb + r) / pa;
                fa->vx = q;
                fb->vx = pb;
                pb = r - d;
                fa->vz = r;
                fb->vz = pb;
            }
        }
    }
}

s32 RaceHeroCollide(s32 player) {
  s32 other;
  EXTengine *eng;
  RaceFux *a;
  RaceFux *b;
  s32 dist;
  s32 dz;
  s32 dx;
  s32 r;
  s32 ang;
  u32 i;
  s32 hit;
  s32 speed;
  s32 done;
  s32 lim;
  s32 dy;
  lim = 0x180;
  eng = Heroes[player].hero_eng;
  hit = 0;
  if (eng != 0)
  {
    for (i = 0;; i++)
    {
      EXTengine *oeng;
      if (i >= 4)
      {
        break;
      }
      if (player == i)
      {
        continue;
      }
      if (Heroes[i].hero_status & 0x2000)
      {
        continue;
      }
      oeng = Heroes[i].hero_eng;
      if (oeng == 0)
      {
        continue;
      }
      dy = abs(oeng->XYZ.y - eng->XYZ.y);
      if (dy > lim)
      {
        continue;
      }
      if (Heroes[player].hero_move == 0)
      {
        continue;
      }
      dx = oeng->XYZ.x - eng->XYZ.x;
      dz = oeng->XYZ.z - eng->XYZ.z;
      dist = csqrt((dx * dx) + (dz * dz)) >> 6;
      if (dist >= 0x180)
      {
        continue;
      }
      r = Random(4);
      ang = get_angle(dx, dz) & 0xFFF;
      dist = (lim - dist) + 8;
      eng->XYZ.x -= (dist * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
      eng->XYZ.z -= (dist * rcossin_tbl[ang].unk2) >> 12;
      RaceHeroesBump(player, i);
      a = (RaceFux *) Heroes[player].hero->FUX;
      hit = 1;
      if (a == 0)
      {
        continue;
      }
      b = (RaceFux *) Heroes[i].hero->FUX;
      if (b == 0)
      {
        continue;
      }
      speed = ((a->vx * a->vx) >> 8) + ((a->vz * a->vz) >> 8);
      if (GameMode == 0x19)
      {
        if (speed > 0x10D2A4)
        {
          if (r == 0)
          {
            PlaySfx(0x675, 0, 0, 0x1000, 0x1E00, 0);
          }
          if (r == 1)
          {
            PlaySfx(0x676, 0, 0, 0x1000, 0x1E00, 0);
          }
          if (r == 2)
          {
            PlaySfx(0x677, 0, 0, 0x1000, 0x1E00, 0);
          }
          if (r == 3)
          {
            PlaySfx(0x678, 0, 0, 0x1000, 0x1E00, 0);
          }
        }
      }
      else
        if (speed > 0x62E08)
      {
        if (r == 0)
        {
          PlaySfx(0x640, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (r == 1)
        {
          PlaySfx(0x641, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (r == 2)
        {
          PlaySfx(0x642, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (r == 3)
        {
          PlaySfx(0x643, 0, 0, 0x1000, 0x1E00, 0);
        }
      }

      speed = a->vx * a->vx;
      speed >>= 8;
      speed += (a->vz * a->vz) >> 8;
      other = (b->vx * b->vx) >> 8;
      other += (b->vz * b->vz) >> 8;
      speed >>= 1;
      if ((other < speed) && (speed > 0xC5C10))
      {
        u16 mode = Heroes[i].hero_mode;
        if (((mode != 0x61) && (mode != 0x5D)) && (Heroes[player].hero_mode != 0x5D))
        {
          PlaySfx(0x654, 0, 0, 0x1000, 0x1E00, 0);
          SetHeroAction(player, 0x5D);
        }
      }

      done = 0;
      if (((a->mode == 0x10) && (RaceTagTime > 0)) && (b->timer == 0))
      {
        b->mode = a->mode;
        a->mode = 0;
        a->timer = 0xF;
        b->timer = 0;
        PlaySfx(0x454, 0, 0, 0x1000, 0x1E00, 0);
        done = 1;
      }
      if ((((done == 0) && (b->mode == 0x10)) && (RaceTagTime > 0)) && (a->timer == 0))
      {
        a->mode = b->mode;
        b->mode = 0;
        b->timer = 0xF;
        PlaySfx(0x454, 0, 0, 0x1000, 0x1E00, 0);
        a->timer = 0;
      }
    }

  }
  return hit;
}

s32 RaceMissileHitHeroes(u8 *arg0, u32 arg1) {
    u32 i;
    s32 d;
    s32 j;
    u16 f;

    for (i = 0; ; i++) {
        if (i >= 4) {
            return -1;
        }
        if (i != arg1 && Heroes[i].hero_mode != 0x61) {
            d = DistXZSq(arg0, &Heroes[i].hero_eng->XYZ);
            if (abs(((SLONG_XYZ *)arg0)->y - Heroes[i].hero_eng->XYZ.y) > 0x180) {
                break;
            }
            if (d < 0x24000) {
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction(i, 0x5D);
                }
                if (G63B->mode == 0x17 && (G63B->levelFlags & 0x80) && G63B->heroCpu[i] == 0 && !(G63B->exitFlags & 0x801)) {
                    f = 1;
                    if (G63B->cheats & 0x800) {
                        f = 0x201;
                    }
                    G63B->exitFlags |= f;
                    G63B->winner[0] = 0;
                    for (j = 0; j < 4; j++) {
                        Heroes[j].hero_status |= 0x100;
                        if (G63B->heroCpu[j] != 0) {
                            RemoveCamera(Heroes[j].hero);
                        }
                    }
                    CameraMode = 1;
                }
                return i;
            }
        }
    }
    return -1;
}

void InitRaceMissile(NAS *m) {
    RaceMissileExt *ext;
    EXTengine *e;

    ext = AddExt(m, sizeof(RaceMissileExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2011;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_Powerups + 0x2028 / 0x38];
        m->Engine->xform.order = 2;
        m->Engine->xform.hasScale = 1;
        m->Engine->sub->xform.order = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->speed = 0x8C;
        ext->speedMax = 0x3000;
        ext->unk40 = 0x700;
        ext->unk54 = 4;
        ext->state = 0;
        ext->unk20 = 0;
        ext->count = 0;
        ext->unk4C = 0;
        ext->unk50 = 0x100;
        ext->shadow = NULL;
        ext->timer = 0;
        ext->unk58 = ext->speedMax;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
        }
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->XYZ, 0x1000, 0x1E00, 0);
    }
}

void PlotRaceMissile(NAS *m) {
    RaceMissileExt *ext;
    RaceVec4 hit;
    RaceVec4 n;
    RaceVec4 d;
    RaceVec4 r;
    s32 dot;
    s32 i;
    s32 count;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->state) {
    case 0:
        RaceToPolar(&m->Engine->XYZ, &ext->radius);
        if ((Heroes[ext->owner].hero_eng->xform.rot.vy - ext->angle) << 20 > 0) {
            ext->angVel = -(ext->speed << 8) / ext->radius;
        } else {
            ext->angVel = (ext->speed << 8) / ext->radius;
        }
        m->Engine->xform.rot.vy = Heroes[ext->owner].hero_eng->xform.rot.vy;
        ext->pos = Heroes[ext->owner].hero_eng->XYZ;
        ext->state = 1;
        break;
    case 1:
        if (--ext->timer == -1) {
            RaceSpawnPuff(&m->Engine->XYZ, 0, &HeroModelBank[DB_CrateEffects]);
            ext->timer = 2;
        }
        break;
    }
    ext->pos.x -= (ext->speed * rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 12;
    ext->pos.z -= (ext->speed * rcossin_tbl[(u16)m->Engine->xform.rot.vy & 0xFFF].unk2) >> 12;
    if (RaceMissileHitHeroes((u8 *)&ext->pos, ext->owner) != -1) {
        RaceSpawnPuff(&m->Engine->XYZ, 1, &HeroModelBank[DB_CrateEffects]);
        PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
        if (m->Engine->sfx != NULL) {
            StopSfx(m->Engine->sfx);
        }
    } else {
        m->Engine->xform.dirty = 1;
        if (csqrt(m->Engine->XYZ.x * m->Engine->XYZ.x
                  + m->Engine->XYZ.z * m->Engine->XYZ.z) >> 6 > 0xF00) {
            goto kill;
        }
        count = 1;
        if (GameMode == 0x17) {
            count = 3;
        }
        for (i = 0; ; i++) {
            s32 snd;

            if (i >= count) {
                break;
            }
            if (i != 1 && RaceTrackCollide(&ext->pos, &hit, 0x96, i, 1) != 0) {
                n.x = rcossin_tbl[hit.y & 0xFFF].unk2;
                n.z = rcossin_tbl[(hit.y - 0x400) & 0xFFF].unk2;
                d.x = -rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2;
                d.z = -rcossin_tbl[(u16)m->Engine->xform.rot.vy & 0xFFF].unk2;
                dot = (d.x * n.x >> 12) + (d.z * n.z >> 12);
                r.x = d.x - (dot * n.x >> 11);
                r.z = d.z - (dot * n.z >> 11);
                m->Engine->xform.rot.vy = get_angle(r.x, r.z) + 0x800;
                ext->count++;
                snd = Random(3);
                if (snd == 0) {
                    PlaySfx(0x65E, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (snd == 1) {
                    PlaySfx(0x65F, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (snd == 2) {
                    PlaySfx(0x660, NULL, NULL, 0x1000, 0x1E00, 0);
                }
            }
        }
        m->Engine->XYZ = ext->pos;
        if (ext->shadow != NULL) {
            ext->shadow->XYZ = m->Engine->XYZ;
            ext->shadow->XYZ.y = 0;
            ext->shadow->xform.scale.vx = m->Engine->xform.scale.vx;
            ext->shadow->xform.scale.vy = m->Engine->xform.scale.vy;
            ext->shadow->xform.scale.vz = m->Engine->xform.scale.vz;
            if (GameMode == 0x1A) {
                ext->shadow->xform.rot.vy = m->Engine->xform.rot.vy;
                ext->shadow->xform.dirty = 1;
                ext->shadow->XYZ.y = -m->Engine->XYZ.y;
            }
        }
        if (ext->count < 3) {
            return;
        }
        RaceSpawnPuff(&m->Engine->XYZ, 1, &HeroModelBank[DB_CrateEffects]);
        PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
kill:
    KillNAS(m);
}

void InitRacePickUp(NAS *m) {
    RacePickUpExt *ext;
    EXTengine *e;

    ext = AddExt(m, sizeof(RacePickUpExt), 0x10000000);
    if (ext != NULL) {
        AddToGroup((NAS **)&RacePickups, m);
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
        m->Engine->xform.order = 2;
        m->Engine->xform.hasScale = 1;
        ext->unk10 = -0x2000;
        ext->unk1C = 0x200;
        ext->flag = 0;
        ext->count = 0;
        ext->unk4C = 0;
        ext->unk30 = 1;
        ext->unk28 = 0;
        ext->unk2C = 0x100;
        ext->shadow = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects - 0x2028 / 0x38];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
        }
    }
}

/* debug printf formats and an empty string, in place in .rodata as above */
const char RaceMsgPlayerAngle[] = "player %d ang %d\n";
const char RaceMsgFucked[] = "fucked\n";
const char RaceEmptyString[] = "";

void PlotRacePickUp(NAS *m) {
    RacePickUpExt *ext;
    RaceFux *fux;
    HeroInfo *h;
    SLONG_XYZ *pos;
    EXTengine *e;
    u32 i;
    s32 found;
    s32 lim;
    s32 d;
    s32 s;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (GameMode != 0x19) {
        RaceBounceObject((RaceBounceObj *)m->Engine, (RaceBouncer *)ext->padC);
    }
    m->Engine->xform.rot.vy += 0x80;
    m->Engine->xform.dirty = 1;
    switch (ext->state) {
    case 1:
        s = 0xC00;
        m->Engine->xform.scale.vx = s;
        m->Engine->xform.scale.vy = s;
        m->Engine->xform.scale.vz = s;
        m->Engine->sub->entity = 0x2009;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
        break;
    case 2:
        m->Engine->sub->entity = 0x2005;
        break;
    case 3:
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
        m->Engine->sub->flags |= 0x20000000;
        m->Engine->sub->entity = ext->count + 0x2049;
        if (++ext->count >= 0xE) {
            ext->count = 0;
        }
        break;
    case 5:
        m->Engine->sub->entity = 0x2017;
        break;
    case 0:
    case 4:
    case 6:
        m->Engine->sub->entity = 0x2011;
        break;
    }
    if (ext->flag == 0) {
        pos = &m->Engine->XYZ;
        i = 0;
        h = Heroes;
    loop:
        if (i < 4) {
            do {
                d = DistXZSq((SLONG_XYZ *)pos, (SLONG_XYZ *)&h->hero_eng->XYZ);
                lim = 0x24000;
                if (d < lim) {
                    if (abs(pos->y - h->hero_eng->XYZ.y) < 0x200) {
                        found = i;
                        goto done;
                    }
                }
                h++;
                i++;
            } while (0);
            goto loop;
        }
        found = -1;
    done:
        if (found != -1) {
            fux = (RaceFux *)Heroes[found].hero->FUX;
            if (fux != NULL && fux->flags == 0 && fux->mode != 0x10) {
                switch (ext->state) {
                case 0:
                    fux->mode = 1;
                    PlaySfx(0x51F, NULL, NULL, 0x1000, 0x1E00, 0);
                    RacePickupSlots[ext->slot].used = 0;
                    break;
                case 1:
                    fux->mode = 2;
                    break;
                case 2:
                    fux->mode = 4;
                    break;
                case 5:
                    fux->mode = 0x20;
                    PlaySfx(0x439, NULL, NULL, 0x1000, 0x1E00, 0);
                    RacePickupSlots[ext->slot].used = 0;
                    break;
                case 6:
                    fux->mode = 0;
                    fux->timer2 = 0x5A;
                    RacePickupSlots[ext->slot].used = 0;
                    break;
                case 3:
                    fux->steer += 0x2000;
                    if (fux->steer > 0x10000) {
                        fux->steer = 0x10000;
                    }
                    RacePickupSlots[ext->slot].used = 0;
                    PlaySfx(0x1C2, NULL, NULL, 0x1000, 0x1E00, 0);
                    break;
                case 4:
                    fux->mode = 0x10;
                    CreateNAS(Heroes[found].hero_eng->XYZ.x, Heroes[found].hero_eng->XYZ.y,
                                 Heroes[found].hero_eng->XYZ.z, 0x1706, 0x11);
                    break;
                }
            }
            KillNAS(m);
            return;
        }
    }
    e = ext->shadow;
    if (e != NULL) {
        e->XYZ = m->Engine->XYZ;
        ext->shadow->XYZ.y = 0;
        ext->shadow->xform.scale.vx = m->Engine->xform.scale.vx;
        ext->shadow->xform.scale.vy = m->Engine->xform.scale.vy;
        ext->shadow->xform.scale.vz = m->Engine->xform.scale.vz;
    }
}

void RaceCountLaps(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;
    RaceFux *fux;
    EXTengine *e;
    s32 one;
    s32 flags;
    s32 unused[2];

    if (!(GameExitFlags & 0x801)) {
        base = Heroes;
        hero = base + player;
        fux = (RaceFux *)hero->hero->FUX;
        if (fux != NULL) {
            get_angle(hero->hero_eng->XYZ.x, hero->hero_eng->XYZ.z);
            e = hero->hero_eng;
            if (e->XYZ.x > 0) {
                if (e->OldXYZ.z >= -0x1ED) {
                    if (e->XYZ.z < -0x1ED) {
                        fux->cross++;
                        fux->side = 1;
                        if (fux->cross >= 2) {
                            fux->cross = 1;
                        }
                    }
                    if (Heroes[player].hero_eng->OldXYZ.z >= -0x1ED) {
                        goto check;
                    }
                }
                if (Heroes[player].hero_eng->XYZ.z >= -0x1ED) {
                    fux->side = 0;
                    if (--fux->cross < -1) {
                        fux->cross = -1;
                    }
                }
            }
check:
            one = fux->cross;
            if (one == 1) {
                if (fux->started == 0) {
                    fux->started = one;
                } else {
                    if (player == 0) {
                        PlaySfx(0x656, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    if (player == one) {
                        PlaySfx(0x657, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    if (player == 2) {
                        PlaySfx(0x658, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    if (player == 3) {
                        PlaySfx(0x659, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux->laps++;
                }
                fux->cross = 0;
            }
            Heroes[player].hero_score_displayed = Heroes[player].hero_score = 10 - fux->laps;
            if (Heroes[player].hero_score == 0) {
                flags = 1;
                if (GameCheats & 0x800) {
                    flags = 0x201;
                }
                HeroHooks.ChooseWinner(-1, flags);
            }
        }
    }
}

void RacePickupMaster(void) {
    NAS *nas;
    NAS *spawned;
    RacePickUpExt *pick;
    RacePickUpExt *ext;
    RacePickUpSlot *s;
    s32 k;
    RaceFux *fux;
    RaceFux *hfux;
    EXTengine *eng;
    GameState *g;
    s32 n;
    s32 big;
    s32 count;
    s32 extra;
    s32 state;
    s32 i;
    s32 j;
    s32 p;
    s32 slot;
    s32 dist;
    s32 angle;
    s32 r;

    if (RaceStartDelay >= 0) {
        return;
    }
    big = 0;
    nas = RacePickups;
    count = 0;
    if (RaceTagTimer != 0) {
        RaceTagTimer--;
        big = 1;
    }
    if (RacePickupTimer == 0) {
        big = 1;
        RacePickupTimer = big;
        RacePickupCount = 30;
        for (k = 0; k < 16; k++) {
            RacePickupSlots[k].used = 0;
            if (k == 2) {
                RacePickupSlots[k].used = 1;
            }
            RacePickupSlots[k].angle = k << 8;
        }
    }
    for (;; nas = nas->Group.Child) {
        if (nas == NULL) {
            goto heroes_loop;
        }
        pick = FindExtension(nas, 0x10000000);
        if (pick != NULL) {
            state = pick->state;
            if (state == 4) {
                big = 1;
            }
            if (state == 2 && RaceEggCount == 0) {
                RaceEggCount = 1;
            }
            if (pick->state == 3) {
                count++;
            }
        }
    }
heroes_loop:
    for (p = 0;; p++) {
        if (p >= 4) {
            goto heroes_done;
        }
        hfux = (RaceFux *)Heroes[p].hero->FUX;
        if (hfux != NULL) {
            state = hfux->mode;
            if (state != 0) {
                if (state == 0x10) {
                    big = 1;
                }
                if (state == 4 && RaceEggCount == 0) {
                    RaceEggCount = 1;
                }
            }
        }
    }
heroes_done:
    extra = 0;
    if (RaceTimer > 0) {
        extra = 3;
    }
    if (RaceTimer >= 0x385) {
        extra = 2;
    }
    if (RaceTimer >= 0x709) {
        extra = 1;
    }
    n = 6 - (count + extra);
    if (n != 0) {
        if (--RacePickupCount < 0) {
            RacePickupCount = 0;
        }
    }
    i = 0;
    g = G63;
    for (;; i++) {
        dist = 0x898;
        if (i > 0) {
            goto held;
        }
        slot = Random(16);
        j = 0;
        if (g->mode == 0x19) {
            dist = 0x5DC;
        }
        if (RacePickupCount == 0) {
            RacePickupCount = 30;
            for (; j < 16;) {
                j++;
                if (RacePickupSlots[slot].used == 0) {
                    break;
                }
                slot++;
                slot &= 0xF;
            }
            if (j == 16) {
                return;
            }
            s = &RacePickupSlots[slot];
            angle = s->angle;
            spawned = CreateNAS(dist * rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 >> 12, 0,
                                   dist * rcossin_tbl[angle & 0xFFF].unk2 >> 12, 0x1702, 0x11);
            if (spawned != NULL) {
                s->used = 1;
                ext = FindExtension(spawned, 0x10000000);
                if (ext != NULL) {
                    ext->slot = slot;
                    Random(big ? 4 : 5);
                    r = Random(0x1000);
                    if (r < 0x800) {
                        ext->state = 3;
                    }
                    if (r > 0x800) {
                        ext->state = 5;
                    }
                    if (r > 0xC00) {
                        ext->state = 0;
                    }
                    ext->state = 3;
                }
            }
        }
    }
held:
    for (p = 0;; p++) {
        if (p >= 4) {
            return;
        }
        fux = (RaceFux *)Heroes[p].hero->FUX;
        if (fux == NULL) {
            continue;
        }
        eng = fux->eng;
        if (eng == NULL) {
            continue;
        }
        eng->sub->flags &= ~0x8000;
        if (fux->mode == 1) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2011;
            eng->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 2) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2009;
            eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 4) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2005;
            eng->sub->u6C.dbase = MapScene->dbase;
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 8) {
            eng->sub->entity = 0x2002;
            eng->sub->u6C.dbase = MapScene->dbase;
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 0x10) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2011;
            eng->sub->xform.scale.vx = eng->sub->xform.scale.vy = eng->sub->xform.scale.vz = 0x200;
            eng->sub->u6C.dbase = MapScene->dbase;
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 0x20) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2017;
            eng->sub->xform.scale.vx = eng->sub->xform.scale.vy = eng->sub->xform.scale.vz = 0x200;
            eng->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (fux->mode == 0x40) {
            eng->sub->flags |= 0x8000;
            eng->sub->entity = 0x2011;
            eng->sub->xform.scale.vx = eng->sub->xform.scale.vy = eng->sub->xform.scale.vz = 0x200;
            eng->sub->u6C.dbase = MapScene->dbase;
            eng->xform.dirty = 1;
            eng->xform.rot.vy += 0x80;
            eng->XYZ = Heroes[p].hero_eng->XYZ;
        }
        if (eng->sub->flags & 0x8000) {
            eng->XYZ.y -= 0x200;
        }
    }
}

void RacePickupRespawn(void) {
    NAS *nas;
    NAS *spawned;
    RacePickUpExt *ext;
    RacePickUpExt *pick;
    RaceFux *fux;
    s32 count;
    s32 heroes;
    s32 extra;
    s32 n;
    s32 p;
    s32 i;
    s32 j;
    s32 slot;
    s32 dist;
    s32 angle;
    s32 state;

    if (RaceStartDelay >= 0) {
        return;
    }
    heroes = 0;
    count = 0;
    for (nas = RacePickups;; nas = nas->Group.Child) {
        if (nas == NULL) {
            goto heroes_loop;
        }
        pick = FindExtension(nas, 0x10000000);
        if (pick != NULL) {
            state = pick->state;
            if (state == 0) {
                count++;
            }
            if (state == 5) {
                count++;
            }
        }
    }
heroes_loop:
    for (p = 0;; p++) {
        if (p >= 4) {
            goto heroes_done;
        }
        fux = (RaceFux *)Heroes[p].hero->FUX;
        if (fux != NULL) {
            state = fux->mode;
            if (state != 0) {
                if (state == 1) {
                    heroes++;
                }
                if (state == 0x20) {
                    heroes++;
                }
            }
        }
    }
heroes_done:
    extra = RaceTimer > 0;
    if (RaceTimer >= 0x385) {
        extra = 1;
    }
    if (RaceTimer >= 0x709) {
        extra = 0;
    }
    n = 3 - (count + extra + heroes);
    if (n != 0) {
        if (--RacePickupHeroCount < 0) {
            RacePickupHeroCount = 0;
        }
    }
    for (i = 0;; i++) {
        if (i >= n) {
            return;
        }
        dist = Random(100) + 0x898;
        slot = Random(16);
        j = 0;
        if (RacePickupHeroCount == 0) {
            RacePickupHeroCount = 30;
            for (; j < 16;) {
                j++;
                if (RacePickupSlots[slot].used == 0) {
                    break;
                }
                slot++;
                slot &= 0xF;
            }
            if (j == 16) {
                return;
            }
            RacePickupSlots[slot].used = 1;
            angle = RacePickupSlots[slot].angle;
            spawned = CreateNAS(dist * rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 >> 12, 0,
                                   dist * rcossin_tbl[angle & 0xFFF].unk2 >> 12, 0x1702, 0x11);
            if (spawned != NULL) {
                ext = FindExtension(spawned, 0x10000000);
                if (ext != NULL) {
                    ext->slot = slot;
                    if (G63->mode != 0x18) {
                        ext->state = 0;
                    } else {
                        ext->state = 5;
                    }
                }
            }
        }
    }
}

void RaceInitStub(void) {
    u8 unused[24];
}

void PlotTNTCrate(NAS *nas) {
    s32 status;
    s32 next;
    s32 i;
    s32 *pos;
    u8 *p;
    NAS *spawned;
    EXTscriptdata *ext;
    HeroInfo *h;
    subengine *sub;

    status = nas->UserInfo.status;
    switch (status) {
    case 0:
        next = 1;
        nas->UserInfo.status = next;
        break;
    case 1:
        i = 0;
        if (nas->UserInfo.user1 < 90) {
            if (nas->UserInfo.user1 == 0) {
                nas->Engine->sub->entity = 0x2012;
            }
            if (nas->UserInfo.user1 == 30) {
                nas->Engine->sub->entity = 0x2011;
            }
            if (nas->UserInfo.user1 == 60) {
                nas->Engine->sub->entity = 0x2010;
            }
            for (; i < 4; i++) {
            }
            if (nas->UserInfo.user2 == 0) {
                nas->UserInfo.flags++;
                PlaySfx(0x3F0, NULL, NULL, 0x1000, 0x1E00, 0);
                nas->UserInfo.user2 = 30 / nas->UserInfo.flags;
                if (nas->UserInfo.user2 < 4) {
                    nas->UserInfo.user2 = 4;
                }
            }
            nas->UserInfo.user2--;
        } else {
            if (nas->UserInfo.user1 == 90) {
                i = Random(4);
                if (i == 0) {
                    PlaySfx(0x3EC, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (i == 1) {
                    PlaySfx(0x3ED, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (i == 2) {
                    PlaySfx(0x3EE, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (i == 3) {
                    PlaySfx(0x3EF, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                pos = (s32 *)&nas->Engine->XYZ;
                p = &HeroModelBank[DB_CrateObjects];
                spawned = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
                if (spawned != NULL) {
                    ext = FindExtension(spawned, 0x10000000);
                    if (ext != NULL) {
                        ext->scriptnumber = status;
                        ext->speed = 0x100;
                        if (p != NULL) {
                            ext->flags = 5;
                            spawned->Engine->sub->u6C.dbase = p;
                        } else {
                            ext->flags = status;
                        }
                        ext->maxloops = 0;
                        spawned->InitDelete(spawned);
                    }
                }
                nas->Engine->sub->flags &= ~0x8000;
            }
            i = 0;
            h = Heroes;
        loop:
            if (i < 4) {
                if (DistXZSq((SLONG_XYZ *)&nas->Engine->XYZ, (SLONG_XYZ *)&h->hero_eng->XYZ) < 0x40000 &&
                    h->hero_mode != 0x5D) {
                    SetHeroAction(i, 0x5D);
                }
                h++;
                i++;
                goto loop;
            }
            if (nas->UserInfo.user1 == 100) {
                KillNAS(nas);
                return;
            }
        }
        nas->UserInfo.user1++;
        break;
    }
}

void InitDinoEgg(NAS *m) {
    EXTdinoegg *ext;
    EXTengine *e;

    ext = AddExt(m, sizeof(EXTdinoegg), 0x10000000);
    if (ext != NULL) {
        ext->anim = (EXTanimator *)ScriptCursorFirst(m, &DinoEggScriptInit);
        m->Engine->xform.order = 0;
        m->Engine->xform.hasScale = 1;
        m->Engine->sub->entity = 0x2005;
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        ext->NasStatus = 0;
        ext->angcounter = 0;
        ext->yang = 0;
        ext->shadow = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
        }
        RaceEggCount++;
    }
}

void PlotDinoEgg(NAS *m) {
    EXTdinoegg *ext;
    EXTengine *h;
    RaceVec4 hit;
    s32 n[4];
    s32 d[4];
    s32 r[4];
    s32 status;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->xform.dirty = 1;
    status = ext->NasStatus;
    switch (status) {
    case 0:
        h = Heroes[ext->owner].hero_eng;
        ext->NasStatus = 2;
        ext->yang = m->Engine->xform.rot.vy = h->xform.rot.vy + 0x800;
        ext->vel = 0x80;
        ext->dec = 4;
        RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 1, 0);
        RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 0, 1);
        m->Engine->XYZ.x += (rcossin_tbl[(h->xform.rot.vy - 0x400) & 0xFFF].unk2 * 75) >> 11;
        m->Engine->XYZ.z += (rcossin_tbl[(u16)h->xform.rot.vy & 0xFFF].unk2 * 75) >> 11;
        break;
    case 2:
        if (ext->vel > 0) {
            ext->vel -= ext->dec;
        }
        m->Engine->xform.rot.vz = (rcossin_tbl[(ext->angcounter - 0x400) & 0xFFF].unk2 * 3) >> 5;
        ext->angcounter += 0x80;
        if (RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 1, 0) != 0) {
            RACE_BOUNCE(ext->yang, n, d, r, m->Engine->xform.rot.vy, hit.y);
        }
        if (RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 0, 1) != 0) {
            RACE_BOUNCE(ext->yang, n, d, r, m->Engine->xform.rot.vy, hit.y);
        }
        m->Engine->XYZ.x -= (ext->vel * rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 12;
        m->Engine->XYZ.z -= (ext->vel * rcossin_tbl[(u16)m->Engine->xform.rot.vy & 0xFFF].unk2) >> 12;
        m->Engine->xform.rot.vy += ((ext->yang - m->Engine->xform.rot.vy) << 20) >> 21;
        if (RaceHitHero((u8 *)&m->Engine->XYZ) != -1) {
            KillNAS(m);
            RaceEggCount = 0;
            return;
        }
        if (ext->vel <= 0) {
            ext->vel = 0;
            ext->NasStatus = 1;
            GotoLinkAnim(ext->anim, 0);
            RunNasAnim(m);
        }
        break;
    case 1:
        m->Engine->XYZ.x -= (rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x20) >> 12;
        m->Engine->XYZ.z -= (rcossin_tbl[(u16)m->Engine->xform.rot.vy & 0xFFF].unk2 * 0x20) >> 12;
        ext->angcounter += 0x80;
        RunNasAnim(m);
        if (RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 1, 0) != 0)
            ext->yang = RaceBounce(m->Engine->xform.rot.vy, hit.y);
        if (RaceTrackCollide(&m->Engine->XYZ, &hit, 0x9E, 0, 1) != 0) {
            RACE_BOUNCE(ext->yang, n, d, r, m->Engine->xform.rot.vy, hit.y);
        }
        m->Engine->xform.rot.vy += ((ext->yang - m->Engine->xform.rot.vy) << 20) >> 21;
        if (RaceHitHero((u8 *)&m->Engine->XYZ) != -1) {
            RaceEggCount = 0;
            KillNAS(m);
            return;
        }
        break;
    }
    if (ext->shadow != NULL) {
        ext->shadow->XYZ = m->Engine->XYZ;
        ext->shadow->XYZ.y = 0;
        ext->shadow->xform.scale.vx = m->Engine->xform.scale.vx;
        ext->shadow->xform.scale.vy = m->Engine->xform.scale.vy;
        ext->shadow->xform.scale.vz = m->Engine->xform.scale.vz;
    }
}

static inline NAS *SpawnShieldFx(SLONG_XYZ *pos) {
    NAS *fx;
    RaceShieldFxExt *fxe;

    fx = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (fx != NULL) {
        fxe = FindExtension(fx, 0x10000000);
        if (fxe != NULL) {
            fxe->unkC = 8;
            fxe->unk18 = 0x100;
            fxe->unk20 = 1;
            fxe->unk28 = 0;
            fx->InitDelete(fx);
        }
        return fx;
    }
    return NULL;
}

void PlotRaceShield(NAS *nas) {
    EXTraceshield *ext;
    HeroInfo *hero;
    RaceFux *fux;
    RaceFux *fux2;
    SLONG_XYZ *pos;
    HeroInfo *other;
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *hb;
    EXTengine *eng;
    EXTengine *eng2;
    u32 i;
    s32 found;
    s32 me;
    s32 lim;
    s32 angle;
    s32 c;
    s32 s;

    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    base = Heroes;
    hero = &base[ext->players[0]];
    fux = (RaceFux *)hero->hero->FUX;
    if (fux == NULL) {
        return;
    }
    switch (ext->state) {
    case 0:
        fux->shield = SpawnShieldFx(&nas->Engine->XYZ);
        PlaySfx(0x43A, NULL, NULL, 0x1000, 0x1E00, 0);
        ext->state = 1;
        break;
    case 1:
        i = 0;
        if (fux->shield != NULL) {
            fux->shield->Engine->XYZ = hero->hero_eng->XYZ;
        }
        lim = 0xC5C10;
        ext->unk1C = 0x384;
        other = base;
        me = ext->players[0];
        pos = &fux->shield->Engine->XYZ;
        while (1) {
            if (i >= 4) {
                found = -1;
                break;
            }
            if (i != me && DistXZSq((SLONG_XYZ *)pos, (SLONG_XYZ *)&other->hero_eng->XYZ) < lim) {
                found = i;
                break;
            }
            other++;
            i++;
        }
        if (found != -1) {
            hb = Heroes;
            h = &hb[found];
            eng = nas->Engine;
            eng2 = h->hero_eng;
            angle = get_angle(eng->XYZ.x - eng2->XYZ.x, eng->XYZ.z - eng2->XYZ.z);
            c = rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 8;
            s = rcossin_tbl[angle & 0xFFF].unk2 * 8;
            fux2 = (RaceFux *)h->hero->FUX;
            if (fux2 != NULL) {
                fux2->vx = -c;
                fux2->vz = -s;
                h->hero_move->x_max = 0;
                h->hero_move->z_max = 0;
                h->hero_move->x_max = 0;
                h->hero_move->z_max = 0;
            }
        }
        if (--ext->timer < 0) {
            fux->flags &= ~1;
            KillNAS(fux->shield);
            KillNAS(nas);
        }
        break;
    }
}

void PlotRaceTag(NAS *m) {
    s32 i;
    HeroInfo *h1;
    HeroInfo *h;
    RaceFux *fux1;
    RaceFux *fux;
    SLONG_XYZ pos;
    u8 *tex;
    SLONG_XYZ *pp;
    NAS *tag;
    RaceTagExt *ext;

    RaceTagTime--;
    i = 0;
    h1 = Heroes;
loop1:
    if (i < 4) {
        fux1 = (RaceFux *)h1->hero->FUX;
        if (fux1 != NULL && fux1->timer > 0) {
            fux1->timer--;
        }
        h1++;
        i++;
        goto loop1;
    }
    if (m->Engine->sfx != NULL) {
        sfx_set_pitch(m->Engine->sfx, (0x12C - RaceTagTime) * 0xF00 / 300 + 0x1E00);
    }
    i = 0;
    if (RaceTagTime == 0) {
        pp = &pos;
        h = Heroes;
loop2:
        if (i < 4) {
            fux = (RaceFux *)h->hero->FUX;
            if (fux == NULL) {
                goto next;
            }
            if (fux->mode == 0x10) {
                goto clear;
            }
            pos = h->hero_eng->XYZ;
            tex = &HeroModelBank[DB_PolarObjects];
            tag = CreateNAS(pos.x, pp->y, pp->z, 0x1605, 0x11);
            if (tag != NULL) {
                ext = FindExtension(tag, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = 0;
                    ext->unk18 = 0x100;
                    if (tex != NULL) {
                        ext->unk20 = 5;
                        tag->Engine->sub->u6C.dbase = tex;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    tag->InitDelete(tag);
                }
            }
            if (h->hero_mode != 0x5D) {
                SetHeroAction(i, 0x5D);
            }
            fux->unkC = 0x78;
            PlaySfx(0x466, NULL, NULL, 0x1000, 0x1E00, 0);
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            goto next;
        clear:
            fux->mode = 0;
        next:
            h++;
            i++;
            goto loop2;
        }
        RaceTagTimer = 0x12C;
        KillNAS(m);
    }
}

void InitRaceVolcano(NAS *nas) {
    s32 i;
    RaceVolcanoExt *v;
    RaceVolcanoExt *ext;
    EXTengine *eng;

    v = AddExt(nas, sizeof(RaceVolcanoExt), 0x10000000);
    i = 0;
    if (v != NULL) {
        ext = v;
        ext->timer = 0x1C2;
        ext->state = 0;
    loop:
        if (i < 3) {
            eng = AddExtEngine(nas, i + 2);
            if (eng != NULL) {
                ext->eng[i] = eng;
                eng->sub->flags &= ~0x8000;
                eng->xform.hasScale = 1;
                eng->xform.order = 2;
                eng->sub->entity = 0x2019;
                eng->sub->u6C.dbase = MapScene->dbase;
                ext->eng[i]->xform.rot.vy = Random(0x1000);
                ext->eng[i]->xform.rot.vz = 0;
                ext->nas[i] = 0;
                ext->delay[i] = 0;
            }
            i++;
            goto loop;
        }
        i = 2;
    loop2:
        i--;
        if (i >= 0) goto loop2;
    }
}

void PlotRaceVolcano(NAS *nas) {
    RaceVolcanoExt *ext;
    SLONG_XYZ zero;
    s32 count;
    s32 i;
    s32 rock;
    s32 n;
    s32 j;
    s32 ang;
    s32 d;
    s32 r;
    NAS *fx;

    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    memset(&zero, 0, sizeof(zero));
    if (RaceOver != 0) {
        for (i = 0; i < 3; i++) {
            if (ext->eng[i]->sub->flags & 0x8000) {
                ext->eng[i]->sub->flags &= ~0x8000;
            }
        }
        return;
    }
    switch (ext->state) {
    case 0:
        if (--ext->timer == 0x3C) {
            PlaySfx(0x69A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (ext->timer < 0) {
            ext->state = 1;
        }
        break;
    case 1:
        ang = Random(0x1000);
        for (rock = 0; ; rock++) {
            s32 x;
            s32 z;

            if (rock >= 3) {
                break;
            }
            x = rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 75 >> 9;
            z = rcossin_tbl[ang & 0xFFF].unk2 * 75 >> 9;
            if (ext->eng[rock] != NULL) {
                ext->eng[rock]->xform.rot.vy = ang;
                ext->vel[rock].x = x;
                ext->vel[rock].y = 0;
                ext->vel[rock].z = z;
                ext->arc[rock].unk0 = 4;
                ext->arc[rock].unk44 = 0xC - Random(3);
                ext->arc[rock].unk4 = 0;
                ext->arc[rock].unk18 = 0x100;
                ext->eng[rock]->sub->flags |= 0x8000;
                ext->eng[rock]->XYZ = zero;
                ang += 0x555 + Random(0x200);
                fx = RaceSpawnPuffRet(&ext->eng[rock]->XYZ, 0xA, 0);
                ext->nas[rock] = fx;
                if (fx != NULL) {
                    if (z < 0) {
                        fx->Engine->sub->flags |= 0x02000000;
                        ext->nas[rock]->Engine->sub->otz = -0x400;
                    } else {
                        fx->Engine->sub->flags |= 0x02000000;
                        ext->nas[rock]->Engine->sub->otz = 0x200;
                    }
                }
            }
        }
        PlaySfx(0x69B, NULL, NULL, 0x1000, 0x1E00, 0);
        RaceSpawnPuff(&nas->Engine->XYZ, 4, NULL);
        ext->timer = 0x14;
        ext->y = -0x80;
        ext->state = 2;
        break;
    case 2:
        count = 0;
        for (n = 0; ; n++) {
            EXTengine *cur;
            EXTengine *eng;

            if (n >= 3) {
                break;
            }
            ext->eng[n]->xform.dirty = 1;
            cur = ext->eng[n];
            if (cur->sub->flags & 0x8000) {
                cur->xform.rot.vx -= 0x80;
                eng = ext->eng[n];
                d = eng->XYZ.x * eng->XYZ.x + eng->XYZ.z * eng->XYZ.z;
                ArcUpdate(&zero.x, &ext->vel[n].x, &ext->arc[n], &eng->XYZ.x);
                if (d < 0x90000) {
                    if (ext->nas[n] != NULL) {
                        ext->nas[n]->Engine->XYZ.y = 0;
                    }
                    if (ext->eng[n]->XYZ.y >= -0xFF && ext->arc[n].yvel > 0) {
                        ext->arc[n].yvel = -(ext->arc[n].yvel * 0x94 >> 8);
                        if (ext->arc[n].xdir < 0) {
                            ext->arc[n].xdir = -0x14;
                        }
                        if (ext->arc[n].xdir > 0) {
                            ext->arc[n].xdir = 0x14;
                        }
                        if (ext->arc[n].zdir < 0) {
                            ext->arc[n].zdir = -0x14;
                        }
                        if (ext->arc[n].zdir > 0) {
                            ext->arc[n].zdir = 0x14;
                        }
                    }
                } else {
                    if (ext->nas[n] != NULL) {
                        ext->nas[n]->Engine->XYZ.y = 0;
                    }
                    if (ext->y == -0x80 && ext->y < ext->eng[n]->XYZ.y
                        && ext->arc[n].yvel > 0) {
                        ext->arc[n].yvel = -(ext->arc[n].yvel * 0x94 >> 8);
                        ext->eng[n]->XYZ.y = ext->y;
                    }
                }
                if (ext->nas[n] != NULL) {
                    ext->nas[n]->Engine->XYZ.x = ext->eng[n]->XYZ.x;
                    ext->nas[n]->Engine->XYZ.z = ext->eng[n]->XYZ.z;
                }
                if (d > 0x900000) {
                    if (ext->eng[n] != NULL) {
                        RaceSpawnPuff(&ext->eng[n]->XYZ, 1, NULL);
                        RACE_SFX4(0x69C, 0x69D, 0x69E, 0x69F);
                        ext->eng[n]->sub->flags &= ~0x8000;
                        ext->arc[n].unk4 = 0;
                        ext->eng[n]->XYZ.y = 0;
                        KillNAS(ext->nas[n]);
                        ext->nas[n] = NULL;
                    }
                } else if (ext->delay[n]-- < 0) {
                    RaceSpawnPuff(&ext->eng[n]->XYZ, 3, NULL);
                    ext->delay[n] = Random(3) + 8;
                }
            }
        }
        for (n = 0; ; n++) {
            s32 dummy;

            if (n >= 3) {
                break;
            }
            j = 0;
            if (!(ext->eng[n]->sub->flags & 0x8000)) {
                count++;
            } else {
                for (; ; j++) {
                    s32 dummy;

                    if (j >= 4) {
                        break;
                    }
                    if (RaceDistSq(&ext->eng[n]->XYZ, &Heroes[j].hero_eng->XYZ) < 0x27100) {
                        if (Heroes[j].hero_mode != 0x5D) {
                            SetHeroAction(j, 0x5D);
                        }
                        RaceSpawnPuff(&ext->eng[n]->XYZ, 1, NULL);
                        ext->eng[n]->sub->flags &= ~0x8000;
                        ext->arc[n].unk4 = 0;
                        ext->eng[n]->XYZ.y = 0;
                        if (ext->nas[n] != NULL) {
                            KillNAS(ext->nas[n]);
                            ext->nas[n] = NULL;
                        }
                    }
                }
            }
        }
        if (count == 3 && ext->timer-- < 0) {
            ext->timer = 0x12C;
            ext->state = 0;
        }
        break;
    }
}

void PlotGlob(NAS *m) {
    EXTglob *ext;
    s32 state;
    s32 i;
    NAS *tag;
    RaceTagExt *text;
    SLONG_XYZ pos;
    SLONG_XYZ *pp;
    s32 x;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        state = ext->state;
        switch (state) {
        case 0:
            if (++ext->count == 0x3C) {
                PlaySfx(0x666, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (RunNasAnim(m) != 0) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 1);
                ext->state = 1;
                PlaySfx(0x667, NULL, NULL, 0x1000, 0x1E00, 0);
                ext->count = 0;
                RunNasAnim(m);
            }
            break;
        case 1:
            x = 0x16;
            if (++ext->count == x) {
                PlaySfx(0x668, NULL, NULL, 0x1000, 0x1E00, 0);
                pp = &pos;
                x = -0x200;
                pos.y = -0x29C;
                pos.x = x;
                pos.z = x;
                tag = CreateNAS(pp->x, pp->y, pp->z, 0x1605, 0x11);
                if (tag != NULL) {
                    text = FindExtension(tag, 0x10000000);
                    if (text != NULL) {
                        text->unkC = 0xA;
                        text->unk18 = 0x100;
                        text->unk20 = state;
                        text->unk28 = 0;
                        tag->InitDelete(tag);
                    }
                }
                for (i = 0; i < 3; i++) {
                    CreateNAS(pos.x, pos.y, pos.z, 0x170D, 0x11);
                }
            }
            if (RunNasAnim(m) != 0) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                RunNasAnim(m);
                ext->state = 2;
                PlaySfx(0x669, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            break;
        case 2:
            if (RunNasAnim(m) != 0) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 2);
                RunNasAnim(m);
                ext->state = 3;
            }
            break;
        case 3:
            if (ext->timer-- < 0 && RaceOver == 0) {
                ext->timer = Random(0x96) + 0x1C2;
                GotoLinkAnim((EXTanimator *)ext->cursor, 0);
                RunNasAnim(m);
                PlaySfx(0x665, NULL, NULL, 0x1000, 0x1E00, 0);
                ext->state = 0;
                ext->count = 0;
            }
            break;
        }
    }
}

s32 RaceExplodeHitHeroes(SLONG_XYZ *pos) {
    s32 hit;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 dz;
    u16 bits;

    hit = 0;
    for (i = 0;; i++) {
        if (i >= 4) {
            return hit;
        }
        dx = Heroes[i].hero_eng->XYZ.x - pos->x;
        dz = Heroes[i].hero_eng->XYZ.z - pos->z;
        dy = Heroes[i].hero_eng->XYZ.y - pos->y;
        if (dx * dx + dz * dz + dy * dy < 0x90000) {
            if (Heroes[i].hero_mode != 0x61) {
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction(i, 0x5D);
                }
                if (MODE63->levelFlags & 0x80) {
                    if (MODE63->heroCpu[i] != 0) {
                        hit = 1;
                        continue;
                    }
                    bits = 1;
                    if (MODE63->cheats & 0x800) {
                        bits = 0x201;
                    }
                    MODE63->winner[0] = 0;
                    MODE63->exitFlags |= bits;
                    for (j = 0; j < 4; j++) {
                        Heroes[j].hero_status |= 0x100;
                        if (MODE63->heroCpu[j] != 0) {
                            RemoveCamera(Heroes[j].hero);
                        }
                    }
                    CameraMode = 1;
                }
            }
            hit = 1;
        }
    }
}

void InitBarrelRoll(NAS *m) {
    BarrelRollExt *ext;
    EXTengine *e;
    s32 r;

    ext = AddExt(m, sizeof(BarrelRollExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        m->Engine->sub->entity = 0x2037;
        m->Engine->xform.order = 1;
        ext->arc = 0xC;
        ext->step = 0;
        ext->unk50 = 0xA0;
        ext->unk10 = 0;
        ext->y = 0;
        r = Random(0x400);
        ext->vel.x = (rcossin_tbl[(r + 0xA00) & 0xFFF].unk2 * 9) >> 4;
        r += 0xE00;
        ext->vel.z = (rcossin_tbl[r & 0xFFF].unk2 * 9) >> 4;
        m->Engine->xform.rot.vy = -r;
        ext->pos.z = 0x1B00;
        ext->vel.y = 0;
        ext->pos.x = 0;
        ext->pos.y = -0x100;
        m->Engine->XYZ = ext->pos;
        m->Engine->xform.dirty = 1;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
            e->xform.scale.vz = 0x4000;
            e->xform.scale.vy = 0x4000;
            e->xform.scale.vx = 0x4000;
            ext->shadow->xform.dirty = 1;
        }
    }
}

void PlotBarrelRoll(NAS *nas) {
    BarrelRollExt *ext;
    s32 n;

    ext = FindExtension(nas, 0x10000000);
    if (ext != NULL) {
        if (RaceExplodeHitHeroes(&nas->Engine->XYZ) != 0) {
            RaceSpawnPuff(&nas->Engine->XYZ, 1, &HeroModelBank[DB_CrateEffects]);
            KillNAS(nas);
            return;
        }
        nas->Engine->xform.rot.vx += 0x200;
        if (ArcUpdate(&ext->pos.x, &ext->vel.x, &ext->arc,&nas->Engine->XYZ.x) != 0) {
            ext->yvel = -((ext->yvel * 200) >> 8);
            nas->Engine->XYZ.y = ext->y;
            n = ext->step + 1;
            ext->xvel *= 200;
            ext->zvel *= 200;
            ext->step = n;
            ext->xvel >>= 8;
            ext->zvel >>= 8;
            if (nas->Engine->XYZ.x * nas->Engine->XYZ.x +
                    nas->Engine->XYZ.z * nas->Engine->XYZ.z > 0x900000) {
                RaceSpawnPuff(&nas->Engine->XYZ, 1, &HeroModelBank[DB_CrateEffects]);
                KillNAS(nas);
                return;
            }
        }
        ext->shadow->XYZ = nas->Engine->XYZ;
        ext->shadow->XYZ.y = 0x100;
    }
    nas->Engine->xform.dirty = 1;
    ext->shadow->xform.dirty = 1;
}

void InitTrackGlob(NAS *m) {
    EXTtrackglob *ext;
    EXTengine *e;
    s32 angle;

    ext = AddExt(m, sizeof(EXTtrackglob), 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &TrackGlobScriptInit);
        AddToGroup((NAS **)&RaceObjects, (NAS *)m);
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        m->Engine->sub->entity = 0x4100;
        m->Engine->xform.order = 1;
        ext->arc = 0x10;
        ext->unk54 = 0x40;
        ext->state = 0;
        ext->unk14 = 0;
        ext->unk28 = 0;
        angle = 0xA00 - (RaceTrackGlobCount << 9);
        if (++RaceTrackGlobCount == 3) {
            RaceTrackGlobCount = 0;
        }
        ext->vel = (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 7) >> 4;
        ext->unk6C = (rcossin_tbl[angle & 0xFFF].unk2 * 7) >> 4;
        ext->unk68 = 0;
        ext->timer = 0x5A;
        m->Engine->xform.rot.vy = 0;
        ext->pos = m->Engine->XYZ;
        m->Engine->xform.dirty = 1;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->eng = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
            ext->eng->xform.dirty = 1;
        }
    }
}

void PlotTrackGlob(NAS *nas) {
    EXTtrackglob *ext;
    EXTengine *e;
    s32 angle;
    HeroInfo *h;
    s32 i;
    s32 dx;
    s32 dz;

    if (RaceOver != 0) {
        KillNAS(nas);
        return;
    }
    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->state) {
    case 0:
        if (ArcUpdate(&ext->pos.x, &ext->vel, &ext->arc, &nas->Engine->XYZ.x) != 0) {
            ext->state = 1;
            GotoLinkAnim((EXTanimator *)ext->cursor, 0);
            RunNasAnim(nas);
            nas->Engine->XYZ.y = 0;
        }
        break;
    case 1:
        i = 0;
        h = Heroes;
    loop1:
        if (i < 4) {
            do {
                dx = nas->Engine->XYZ.x - h->hero_eng->XYZ.x;
                dz = nas->Engine->XYZ.z - h->hero_eng->XYZ.z;
                if (dx * dx + dz * dz < 0x10000) {
                    if (h->hero_mode != 0x5D) {
                        SetHeroAction(i, 0x5D);
                    }
                    KillNAS(nas);
                    return;
                }
            } while (0);
            h++;
            i++;
            goto loop1;
        }
        RunNasAnim(nas);
        if (ext->timer-- < 0) {
            ext->state = 2;
            ext->angle = Random(0x1000);
        }
        break;
    case 2:
        RunNasAnim(nas);
        angle = get_angle(nas->Engine->XYZ.x, nas->Engine->XYZ.z) & 0xFFF;
        angle += 0x400;
        angle &= 0xFFF;
        i = 0;
        ext->angle = angle;
        nas->Engine->XYZ.x -= 0x20 * rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 >> 12;
        h = Heroes;
        nas->Engine->XYZ.z -= 0x20 * rcossin_tbl[ext->angle & 0xFFF].unk2 >> 12;
    loop2:
        if (i < 4) {
            do {
                if ((nas->Engine->XYZ.x - h->hero_eng->XYZ.x) * (nas->Engine->XYZ.x - h->hero_eng->XYZ.x) +
                        (nas->Engine->XYZ.z - h->hero_eng->XYZ.z) * (nas->Engine->XYZ.z - h->hero_eng->XYZ.z) < 0x10000) {
                    if (h->hero_mode != 0x5D) {
                        SetHeroAction(i, 0x5D);
                    }
                    KillNAS(nas);
                    return;
                }
            } while (0);
            h++;
            i++;
            goto loop2;
        }
        e = nas->Engine;
        e->xform.rot.vy -= (s32)((e->xform.rot.vy - ext->angle) << 20) >> 22;
        if (angle < 0x400) {
            ext->state = 3;
        }
        break;
    case 3:
        RunNasAnim(nas);
        ext->angle = 0x800;
        nas->Engine->XYZ.x -= 0x20 * rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2 >> 12;
        nas->Engine->XYZ.z -= 0x20 * rcossin_tbl[ext->angle & 0xFFF].unk2 >> 12;
        e = nas->Engine;
        e->xform.rot.vy -= (s32)((e->xform.rot.vy - ext->angle) << 20) >> 22;
        if (nas->Engine->XYZ.x * nas->Engine->XYZ.x + nas->Engine->XYZ.z * nas->Engine->XYZ.z > 0x790000) {
            KillNAS(nas);
            return;
        }
        break;
    }
    ext->eng->XYZ = nas->Engine->XYZ;
    ext->eng->XYZ.y = 0x40;
    nas->Engine->xform.dirty = 1;
}

void InitDac(NAS *m) {
    DacExt *ext;
    EXTengine *e;

    ext = AddExt(m, sizeof(DacExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.order = 2;
        m->Engine->xform.scale.vx = 0x2000;
        m->Engine->xform.scale.vy = 0x2000;
        m->Engine->xform.scale.vz = 0x2000;
        m->Engine->XYZ.x = (rcossin_tbl[0x800].unk2 * 7) >> 4;
        m->Engine->XYZ.z = (rcossin_tbl[0xC00].unk2 * 7) >> 4;
        m->Engine->sub->otz = -0x300;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk10 = ScriptCursorFirst(m, &DacScriptInit);
        RunNasAnim(m);
        m->Engine->xform.rot.vy = 0x800;
        m->Engine->xform.dirty = 1;
        ext->unkC = 0;
        ext->unk18 = 0xF0;
        m->Engine->XYZ.y = -0x600;
        ext->unk1C = -0x600;
        ext->unk24 = 0;
        ext->shadow = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            e->sub->entity = 0x2001;
            e->xform.hasScale = 1;
            e->xform.scale.vx = 0x2000;
            e->xform.scale.vy = 0x2000;
            e->xform.scale.vz = 0x2000;
            e->sub->otz = -0x300;
            e->sub->flags |= 0x02000000;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->sub->scale = 0x800;
            e->sub->flags |= 0x40008041;
            ext->shadow->xform.dirty = 1;
        }
    }
}

void PlotDac(NAS *nas) {
    EXTracedac *ext;
    EXTengine *eng;
    HeroInfo *h;
    HeroInfo *base;
    s32 dx, dz, dy, dd;
    s32 a, b, i, p;
    u8 *g;

    ext = FindExtension(nas, 0x10000000);
    if (ext == NULL) {
        return;
    }
    RunNasAnim(nas);
    nas->Engine->xform.dirty = 1;
    switch (ext->state) {
    found:
        ext->player = p;
        goto sfx;
    case 0:
        nas->Engine->XYZ.y -= (nas->Engine->XYZ.y - ext->height) >> 4;
        ext->angle = ((get_angle(nas->Engine->XYZ.x, nas->Engine->XYZ.z) & 0xFFF) + 0x400) & 0xFFF;
        nas->Engine->XYZ.x += rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2 * 32 >> 12;
        nas->Engine->XYZ.z += rcossin_tbl[ext->angle & 0xFFF].unk2 * 32 >> 12;
        {
            s32 r;
            r = nas->Engine->xform.rot.vy - 0x800;
            nas->Engine->xform.rot.vy -= ((r - ext->angle) << 20) >> 22;
        }
        nas->Engine->xform.rot.vx -= (nas->Engine->xform.rot.vx << 20) >> 22;
        if (ext->timer-- < 0) {
            i = 0;
            p = Random(4);
            ext->state = 1;
            g = GameInfo;
        search:
            i++;
            if (g[p + 0x5C] == 0) {
                goto found;
            }
            p++;
            p &= 3;
            if (i < 4) {
                goto search;
            }
        sfx:
            PlaySfx(0x6A0, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->height = -0x200;
            ext->timer = 0x78;
        }
        break;
    case 1:
        if (RaceOver != 0) {
            ext->height = -0x600;
            ext->state = 0;
            ext->timer = 0xF3C;
            return;
        }
        dy = nas->Engine->XYZ.y - ext->height;
        nas->Engine->XYZ.y -= dy >> 4;
        base = Heroes;
        h = &base[ext->player];
        dx = nas->Engine->XYZ.x - h->hero_eng->XYZ.x;
        dz = nas->Engine->XYZ.z - h->hero_eng->XYZ.z;
        dy = nas->Engine->XYZ.y - h->hero_eng->XYZ.y;
        dd = dx * dx + dz * dz;
        if (dd + dy * dy < 0x40000) {
            PlaySfx(0x6A1, NULL, NULL, 0x1000, 0x1E00, 0);
            if (base[ext->player].hero_mode != 0x5D) {
                SetHeroAction(ext->player, 0x5D);
            }
            ext->height = -0x600;
            ext->state = 0;
            ext->timer = 0x168;
            return;
        }
        if (h->hero_mode == 0x61) {
            ext->height = -0x600;
            ext->state = 0;
            ext->timer = 0xF0;
            return;
        }
        a = get_angle(dx, dz) & 0xFFF;
        b = get_angle(dy, csqrt(dd) >> 6);
        ext->angle = a;
        nas->Engine->XYZ.x -= (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 5) >> 9;
        nas->Engine->XYZ.z -= (rcossin_tbl[ext->angle & 0xFFF].unk2 * 5) >> 9;
        eng = nas->Engine;
        eng->xform.rot.vy -= ((eng->xform.rot.vy - ext->angle) << 20) >> 22;
        nas->Engine->xform.rot.vx -= ((nas->Engine->xform.rot.vx - (-b & 0xFFF)) << 20) >> 22;
        if (nas->Engine->xform.rot.vx > 0x200) {
            nas->Engine->xform.rot.vx = 0x200;
        }
        if (ext->timer-- < 0) {
            ext->height = -0x600;
            ext->state = 0;
            ext->timer = 0x168;
        }
        break;
    }
    ext->shadow->xform.scale.vx = (nas->Engine->XYZ.y + 0x600) * 5 + 0x3000;
    ext->shadow->xform.scale.vy = (nas->Engine->XYZ.y + 0x600) * 5 + 0x3000;
    ext->shadow->xform.scale.vz = (nas->Engine->XYZ.y + 0x600) * 5 + 0x3000;
    ext->shadow->xform.dirty = 1;
    ext->shadow->XYZ = nas->Engine->XYZ;
    ext->shadow->XYZ.y = 0x40;
}

void RaceResetJumps(void) {
    RaceJumpTable[0] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[1] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[2] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[3] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[4] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[5] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[6] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
    RaceJumpTable[7] = (RaceShort3){ RaceJumpVel, 0x28, 0x38 };
}

void PlotRaceSmokeBoostWater(NAS *nas) {
    RunNasAnim(nas);
}

void InitRaceSmokeBoostWater(NAS *nas) {
    ScriptCursorFirst(nas, &RaceSmokeBoostWaterScriptInit);
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
    nas->UserInfo.user1 = 0;
}

s32 RaceBounceObject(RaceBounceObj *obj, RaceBouncer *b) {
    s32 pos;

    b->vel += b->accel;
    pos = obj->pos += b->vel >> 8;
    if (b->limit < pos) {
        obj->pos -= abs(pos - b->limit);
        if (b->flags & 1) {
            b->vel = -b->vel;
        }
        if (b->flags & 2) {
            b->vel = -((b->vel * b->damp) >> 8);
        }
        if (b->flags & 4) {
            b->vel = -b->rebound;
        }
        return 1;
    }
    return 0;
}

void InitGlob(NAS *m) {
    EXTglob *ext;

    ext = AddExt(m, sizeof(EXTglob), 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &GlobScriptInit);
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        GotoLinkAnim((EXTanimator *)ext->cursor, 2);
        RunNasAnim(m);
        ext->state = 3;
        ext->timer = Random(0x5A) + 0xF0;
        ext->count = 0;
    }
}

void PlotInnerRipple(NAS *m) {
    EXTinnerripple *ext;
    subengine *sub;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        ext->scale += ext->speed;
        if (ext->max < ext->scale) {
            ext->scale = ext->max;
        }
        m->Engine->xform.scale.vx = ext->scale + 0x400;
        m->Engine->xform.scale.vy = ext->scale + 0x400;
        m->Engine->xform.scale.vz = ext->scale + 0x400;
        m->Engine->xform.dirty = 1;
        sub = m->Engine->sub;
        if ((s16)sub->scale >= 0xFFF) {
            KillTriggerNas(m);
            return;
        }
        sub->scale += ext->spin;
    }
}

void InitInnerRipple(NAS *m) {
    EXTinnerripple *ext;

    ext = AddExt(m, sizeof(EXTinnerripple), 0x10000000);
    if (ext != NULL) {
        ext->speed = 0x240;
        ext->max = 0x4000;
        ext->scale = 0;
        *(s32 *)&ext->spin = 0xC8;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
        m->Engine->sub->entity = 0x2018;
        m->Engine->sub->flags |= 0x40000000;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.order = 0;
        m->Engine->sub->scale = 0;
        m->Engine->xform.scale.vx = 0x400;
        m->Engine->xform.scale.vy = 0x400;
        m->Engine->xform.scale.vz = 0x400;
        m->Engine->xform.dirty = 1;
    }
}

void PlotRippled(NAS *m) {
    EXTripple *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        ext->scale += ext->step;
        if (ext->max < ext->scale) {
            ext->scale = ext->max;
        }
        m->Engine->xform.scale.vx = ext->scale + 0x400;
        m->Engine->xform.scale.vy = ext->scale + 0x400;
        m->Engine->xform.scale.vz = ext->scale + 0x400;
        m->Engine->sub->scale += ext->fade;
        m->Engine->xform.dirty = 1;
        if ((s16)m->Engine->sub->scale >= 0xFFF) {
            KillTriggerNas(m);
        }
    }
}

void InitRippled(NAS *m) {
    EXTrippled *ext;

    ext = AddExt(m, sizeof(EXTrippled), 0x10000000);
    if (ext != NULL) {
        ext->speed = 0x480;
        ext->max = 0x4000;
        ext->scale = 0;
        ext->unk18 = 0x190;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.scale.vx = 0x400;
        m->Engine->xform.scale.vy = 0x400;
        m->Engine->xform.scale.vz = 0x400;
        m->Engine->sub->entity = 0x2018;
        m->Engine->sub->flags = 0x40008000;
        m->Engine->xform.order = 0;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.dirty = 1;
        m->Engine->sub->scale = 0;
    }
}

void PlotRaceGreenSmokeBoost(NAS *nas) {
    RunNasAnim(nas);
}

void InitRaceGreenSmokeBoost(NAS *nas) {
    ScriptCursorFirst(nas, &RaceGreenSmokeBoostScriptInit);
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
    nas->UserInfo.user1 = 0;
}

void PlotRaceGreenSmokePoof(NAS *nas) {
    RunNasAnim(nas);
}

void InitRaceGreenSmokePoof(NAS *nas) {
    ScriptCursorFirst(nas, &RaceGreenSmokePoofScriptInit);
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
    nas->Engine->sub->flags |= 0x21;
    nas->UserInfo.user1 = 0;
}

void PlotRaceSmokePoof(NAS *nas) {
    RunNasAnim(nas);
}

void InitRaceSmokePoof(NAS *nas) {
    ScriptCursorFirst(nas, &RaceSmokePoofScriptInit);
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    nas->Engine->sub->flags |= 0x21;
    nas->UserInfo.user1 = 0;
}

s32 RaceDist2(SLONG_XYZ *arg0, SLONG_XYZ *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    temp_v0 = (arg0->x) - (arg1->x);
    temp_v0_2 = (arg0->z) - (arg1->z);
    temp_v0_3 = (arg0->y) - (arg1->y);
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2) + (temp_v0_3 * temp_v0_3);
}

void InitRaceTag(NAS *arg0) {
    RaceTagTime = 0x12C;
    PlaySfx(0x452, &arg0->Engine->sfx, 0, 0x1000, 0x1E00, 0);
}

void InitRaceShield(NAS *nas) {
    EXTraceshield *ext;

    ext = AddExt(nas, sizeof(EXTraceshield), 0x10000000);
    if (ext != NULL) {
        ext->timer = 0xE;
        ext->state = 0;
        ext->unk1C = 0x100;
    }
}

void PlotRaceFire(NAS *nas) {
    FindExtension(nas, 0x10000000);
}

void InitRaceFire(NAS *nas) {
    EXTracefire *ext;

    ext = AddExt(nas, sizeof(EXTracefire), 0x10000000);
    if (ext != NULL) {
        ext->unk10 = 0;
        ext->unk18 = 0;
        ext->unk1C = 0;
        ext->unk14 = 0;
    }
}

static inline s32 DotXZ(s32 *u, s32 *v) {
    return (u[0] * v[0] >> 12) + (u[2] * v[2] >> 12);
}

s32 RaceReflectAngle(s32 a, s32 b) {
    s32 n[4];
    s32 d[4];
    s32 r[4];
    s32 dot;

    n[0] = rcossin_tbl[b & 0xFFF].unk2;
    n[2] = rcossin_tbl[(b - 0x400) & 0xFFF].unk2;
    d[0] = -rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
    d[2] = -rcossin_tbl[a & 0xFFF].unk2;
    dot = DotXZ(d, n);
    r[0] = d[0] - (dot * n[0] >> 11);
    r[2] = d[2] - (dot * n[2] >> 11);
    return get_angle(r[0], r[2]) + 0x800;
}

s32 RaceDazeHeroNear(u8 *arg0) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (DistXZSq(arg0, &hero->hero_eng->XYZ) < 0x6184) {
            if (hero->hero_mode != 0x5D) {
                SetHeroAction(i, 0x5D);
            }
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void InitTNTCrate(NAS *m) {
    m->Engine->xform.order = 2;
    m->Engine->xform.hasScale = 1;
    m->Engine->sub->entity = 0x2009;
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
    m->UserInfo.status = 0;
    m->UserInfo.user1 = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.flags = 0;
}

s32 RaceToCell(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if (var_a0 < 0) {
        var_a0 += 0x1FF;
    }
    return var_a0 >> 9;
}

void PlotRaceSmokeBoost(NAS *nas) {
    RunNasAnim(nas);
}

void InitRaceSmokeBoost(NAS *nas) {
    ScriptCursor *cur;

    cur = ScriptCursorFirst(nas, &RaceSmokeBoostScriptInit);
    if (GameMode == 0x1F) {
        if (cur != NULL) {
            cur->frame = 4;
        }
    }
    nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CrateEffects];
    nas->UserInfo.user1 = 0;
}

s32 RaceHeroNear(u8 *arg0, s32 arg1) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (DistXZSq(arg0, &hero->hero_eng->XYZ) < arg1 * arg1) {
            if (abs(((s32 *)arg0)[1] - hero->hero_eng->XYZ.y) < 0x200) {
                return i;
            }
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void RaceFireMissile(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    SLONG_XYZ pos;
    NAS *nas;
    RaceMissileExt *ext;

    pos = hero->hero_eng->XYZ;
    nas = CreateNAS(pos.x, pos.y - 0x80, pos.z, 0x1701, 0x11);
    if (nas != NULL) {
        nas->Engine->xform.rot.vy = hero->hero_eng->xform.rot.vy;
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->owner = player;
        }
    }
}

void RaceMovesSwap(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        d = -((a->x_vel - b->x_vel) << 8);
        q = (d * mb + ((ma * a->x_vel + mb * b->x_vel) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        d = -((a->z_vel - b->z_vel) << 8);
        q = (d * mb + ((ma * a->z_vel + mb * b->z_vel) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

s32 RaceTrackAngle(s32 arg0) {
    EXTengine *eng;
    s32 angle;

    eng = Heroes[arg0].hero_eng;
    angle = get_angle(eng->XYZ.x, eng->XYZ.z) & 0xFFF;
    angle = 0x152C - angle;
    angle &= 0xFFF;
    printf(RaceMsgPlayerAngle, arg0, angle);
    return angle;
}

void RaceHeroWinner(s32 arg0) {
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        FinishLevel();
    }
    ProcessHeroAnim(arg0);
}

s32 TestRaceHeroWon(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x62) {
            SetHeroAction(arg0, 0x62);
        }
        return 1;
    }
    return 0;
}

s32 RaceHeroBreatheFire(s32 arg0) {
    HeroInfo *hero;
    RaceFux *fux;
    EXTraceshield *ext;
    NAS *nas;
    HeroInfo *base;

    base = Heroes;
    hero = &base[arg0];
    fux = (RaceFux *)hero->hero->FUX;
    if (fux != NULL && fux->hits > 0 &&
        (((PadState *)Pads)[arg0].dirPressed & 0x8000)) {
        nas = CreateNAS(hero->hero_eng->XYZ.x, hero->hero_eng->XYZ.y,
                           hero->hero_eng->XYZ.z, 0x1705, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->state = arg0;
            }
        }
        return 1;
    }
    return 0;
}

s32 RaceHeroTestIdle(s32 arg0) {
    if (((PadState *)Pads)[arg0].dir & 0xF0) {
        return 0;
    }
    SetHeroAction(arg0, 0x5B);
    return 1;
}

s32 RaceHeroTestRun(s32 arg0) {
    if (!(((PadState *)Pads)[arg0].dirPressed & 0x8000)) {
        return 0;
    }
    SetHeroAction(arg0, 0x5C);
    return 1;
}

void SetRaceHeroKick(s32 player) {
    EXTraceshield *ext;

    ext = (EXTraceshield *)Heroes[player].hero->FUX;
    if (ext != NULL) {
        ext->players[0] = 8;
    }
}

s32 RaceOtherHeroNear(u8 *arg0, u32 arg1, s32 arg2) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (i != arg1) {
            if (DistXZSq(arg0, &hero->hero_eng->XYZ) < arg2 * arg2) {
                return i;
            }
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void SetRaceHeroOiled(s32 player) {
    HeroInfo *hero;
    RaceFux *fux;
    EXTheromove *move;
    s32 a;
    s32 b;
    HeroInfo *base = Heroes;

    hero = &base[player];
    fux = (RaceFux *)hero->hero->FUX;
    if (fux != NULL) {
        move = hero->hero_move;
        if (move != NULL) {
            a = Random(0x300);
            b = Random(0x300);
            b = hero->hero_eng->xform.rot.vy + a - b;
            fux->timer2 = 0x2D;
            fux->unk20 = b;
            fux->unk28 = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        }
    }
}

void SetRaceHeroIdle(s32 player) {
    EXTheromove *move;
    RaceFux *fux;

    SetHeroSeq(player, 0x48);
    if (GameMode == 0x19) {
        SetHeroSeq(player, 0x52);
    }
    move = Heroes[player].hero_move;
    if (move != NULL) {
        move->z_max = 0;
        move->x_max = 0;
    }
    fux = (RaceFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->flag = 0;
        fux->flag2 = 0;
    }
}

void RaceAxisAngleToQuat(s32 *arg0, s32 *arg1)
{
    s32 a;
    s32 c;
    s32 t;

    a = arg0[3] >> 1;
    arg1[3] = rcossin_tbl[a & 0xFFF].unk2;
    c = rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
    t = c * arg0[0];
    arg1[0] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
    t = c * arg0[1];
    arg1[1] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
    t = c * arg0[2];
    arg1[2] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
}

void RaceInitHero(s32 arg0) {
    SetHeroAction(arg0, 0x5B);
    Heroes[arg0].hero_eng->xform.rot.vy = 0;
    Heroes[arg0].hero_eng->xform.dirty = 1;
}

s32 RaceFallToFloor(s32 player) {
    EXTheromove *move;
    EXTengine *eng;
    s32 y;

    move = Heroes[player].hero_move;
    eng = Heroes[player].hero_eng;
    if (move != NULL && eng != NULL) {
        if (move->jump_vel < 0) {
            move->jump_vel += move->jump_acc;
        } else {
            move->jump_vel += move->jump_dec;
        }
        y = eng->XYZ.y += move->jump_vel;
        if (move->jump_vel < 0 && y < RaceFloorLevel) {
            eng->XYZ.y += abs(RaceFloorLevel - y);
            return 1;
        }
    }
    return 0;
}

s32 RaceJumpUp(s32 player) {
    EXTheromove *move;
    EXTengine *eng;
    s32 y;

    move = Heroes[player].hero_move;
    eng = Heroes[player].hero_eng;
    if (move != NULL && eng != NULL) {
        if (move->jump_vel < 0) {
            move->jump_vel += move->jump_acc;
        } else {
            move->jump_vel += move->jump_dec;
        }
        y = eng->XYZ.y += move->jump_vel;
        if (move->jump_vel > 0 && y > RaceFloorLevel) {
            return 1;
        }
    }
    return 0;
}

s32 RaceFloorY(void) {
    return RaceFloorLevel;
}

void RacePolarOf(s32 *pos, s32 *polar) {
    polar[0] = csqrt(pos[0] * pos[0] + pos[2] * pos[2]) >> 6;
    polar[1] = get_angle(pos[0], pos[2]) & 0xFFF;
}

s32 RaceAlwaysZero(void) {
    return 0;
}

void FirstTriggersRACE(void) {
}

void RaceRestartLevel(void) {
}

void RaceSpawnBarrelRolls(void) {
    if (RaceBarrelRollTimer-- < 0) {
        CreateNAS(0, 0, 0, 0x170C, 0x11);
        RaceBarrelRollTimer = 0xB4;
    }
}

void RaceCycleModel(void) {
    RaceGlowObj->unk74 = (RaceGlowPhase >> 8) + 0x4000;
    if (RaceGlowMode == 0) {
        RaceGlowPhase += 0x80;
        if (RaceGlowPhase >= 0x6000) {
            RaceGlowPhase = 0;
        }
    }
    if (RaceGlowMode == 1) {
        RaceGlowPhase -= 0x19C;
        if (RaceGlowPhase <= 0) {
            RaceGlowPhase = 0;
            RaceGlowMode = 0;
        }
    }
}

NAS *RaceNearestPickup(s32 arg0) {
    HeroInfo *base;
    HeroInfo *hero;
    NAS *nas;
    NAS *best;
    s32 dist;
    s32 min;

    best = NULL;
    min = 0x7FFFFFFF;
    nas = RacePickups;
    base = Heroes;
    hero = base + arg0;
loop:
    if (nas != NULL) {
        dist = DistXZSq((SLONG_XYZ *)&nas->Engine->XYZ, (SLONG_XYZ *)&hero->hero_eng->XYZ);
        do {
            if (dist < min) {
                min = dist;
                best = nas;
            }
        } while (0);
        nas = nas->Group.Child;
        goto loop;
    }
    return best;
}

s32 RacePadAngle(s32 arg0) {
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

void Race_HeroBot(s32 player) {
    RaceAI_Think(player);
}

NAS *RacePickupAhead(s32 arg0) {
    NAS *nas;
    HeroInfo *base;
    HeroInfo *hero;
    s32 angle;

    nas = RaceNearestPickup(arg0);
    if (nas != NULL) {
        base = Heroes;
        hero = base + arg0;
        if (DistXZSq((SLONG_XYZ *)&nas->Engine->XYZ, (SLONG_XYZ *)&hero->hero_eng->XYZ) <= 0x270FFF) {
            angle = get_angle(nas->Engine->XYZ.x, nas->Engine->XYZ.z) & 0xFFF;
            if ((get_angle(hero->hero_eng->XYZ.x, hero->hero_eng->XYZ.z) & 0xFFF) < angle) {
                return nas;
            }
        }
    }
    return NULL;
}

s32 RaceAI_TargetShare(s32 arg0, s32 arg1) {
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

void RACESetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseRACEFirst;
    LevelSpecs.LevelInit = InitialiseRACE;
    LevelSpecs.LevelRestartInit = InitialiseRACE;
    LevelSpecs.LevelRoutine = UpdateRACE;
    LevelSpecs.EarliestTriggers = FirstTriggersRACE;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
}
