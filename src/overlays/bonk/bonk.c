#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/bonk_ovl.h"
#include "bash/bonk.h"
#include "bash/pogo.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char BonkAnimBreathe[] = "BREATHE";
const char BonkAnimMedium[] = "MEDIUM";
const char BonkAnimAttack[] = "ATTACK";
const char BonkAnimHit[] = "HIT";
const char BonkAnimWin[] = "WIN";

/* data, in address order */
u16 BonkAiSkill[10] = {
    0x19, 0x16, 0x13, 0x10, 0xC, 0x1C, 0x18, 0x10, 0x10, 0x1E,
};
u16 BonkAiSkillAlt[10] = {
    0x19, 0x16, 0x13, 0x10, 8, 0x1C, 0x18, 0x10, 0x10, 0x1E,
};
u16 BonkAiTeamSkill[6] = {
    0x16, 0x19, 0x1E, 0x16, 0x19, 0x1E,
};
u16 BonkAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 BonkAiHumanRoundTweakAlt[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
s32 BonkStoolFrames[40] = {
    0x60, 0x5D, 0x5A, 0x57, 0x54, 0x51, 0x4E, 0x4B, 0x48, 0x45, 0x42, 0x3F, 0x3C, 0x39, 0x36,
    0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0xF, 0xC, 9, 6, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 BonkCrateFrames[40] = {
    0x60, 0x5D, 0x5A, 0x57, 0x54, 0x51, 0x4E, 0x4B, 0x48, 0x45, 0x42, 0x3F, 0x3C, 0x39, 0x36,
    0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0xF, 0xC, 9, 6, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
};
s32 BonkSteerA[40] = {
    0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D,
    0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E,
};
s32 BonkSteerB[40] = {
    0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
    0x5F, 0x60, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61,
};
u16 BonkAIRange[10] = { 0x1000, 0x200, 0x200, 0x220, 0x220, 0x320, 0x320, 0x300, 0x300, 0 };
u16 BonkPowerupTimes[10] = { 0x300, 0x240, 0x280, 0x200, 0x260, 0x280, 0x2C0, 0x340, 0x300, 0 };
s32 BonkStoolCount = 6;
s32 BonkUnused1[6] = { 5, 5, 4, 3, 2, 1 };
s32 BonkStoolHeights[7] = { 0x50, 0x50, 0x46, 0x46, 0x3C, 0x32, 0x28 };
s32 BonkStoolTable[21] = {
    0x3C, 0x32, 0x28, 0x2D, 0x28, 0x23, 0x1E, 0x5A, 0x5A, 0x5A, 0x5A, 0x3C, 0x3C, 0x3C, 0x1E,
    0x1E, 0x19, 0x19, 0x14, 0xF, 0xF,
};
s32 BonkEntryCount = 0xF;
/* six words nothing points at, before the sequences */
s32 BonkUnused0[6] = { 0xF, 0xD, 0xA, 0xA, 0x5A, 0x5A };
hero_seq CRA_seq_bonk_idle[6] = {
    0, 0x8007, 0x16, 0x4100, 0x8000, 0,
};
hero_seq COC_seq_bonk_idle[6] = {
    0, 0x8007, 0x1C, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_bonk_idle[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_bonk_idle[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_bonk_idle[6] = {
    0, 0x8007, 0x16, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_bonk_idle[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_bonk_idle[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_bonk_idle[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_bonk_run[28] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x8003, 8, 0x4008, 0x4009,
    0x400A, 0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x8003, 9, 0x4012, 0x4013,
    0x8000, 0, 0,
};
hero_seq COC_seq_bonk_run[30] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x8003, 8, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x410C, 0x410D, 0x8003, 9, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113,
    0x4114, 0x4115, 0x8000, 0, 0,
};
hero_seq CTX_seq_bonk_run[30] = {
    0, 0x4080, 0x4081, 0x8003, 8, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089,
    0x408A, 0x408B, 0x8003, 9, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093,
    0x4094, 0x4095, 0x8000, 0, 0,
};
hero_seq BRO_seq_bonk_run[30] = {
    0, 0x4100, 0x4101, 0x8003, 8, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x8003, 9, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113,
    0x4114, 0x4115, 0x8000, 0, 0,
};
hero_seq DNG_seq_bonk_run[26] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x8003, 8, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x410C, 0x8003, 9, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x8000, 0, 0,
};
hero_seq KAN_seq_bonk_run[28] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x8003, 8, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x410C, 0x410D, 0x8003, 9, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113,
    0x8000, 0, 0,
};
hero_seq TIN_seq_bonk_run[28] = {
    0, 0x4100, 0x4101, 0x8003, 8, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x8003, 9, 0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113,
    0x8000, 0, 0,
};
hero_seq KON_seq_bonk_run[28] = {
    0, 0x4100, 0x4101, 0x8003, 8, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x8003, 9, 0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113,
    0x8000, 0, 0,
};
hero_seq CRA_seq_bonk_hit[6] = {
    0, 0x8007, 0x13, 0x4180, 0x8004, 0x40,
};
hero_seq COC_seq_bonk_hit[6] = {
    0, 0x8007, 0x15, 0x4080, 0x8004, 0x40,
};
hero_seq CTX_seq_bonk_hit[6] = {
    0, 0x8007, 0x1A, 0x4100, 0x8004, 0x40,
};
hero_seq BRO_seq_bonk_hit[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8004, 0x40,
};
hero_seq DNG_seq_bonk_hit[6] = {
    0, 0x8007, 0x20, 0x4080, 0x8004, 0x40,
};
hero_seq KAN_seq_bonk_hit[6] = {
    0, 0x8007, 0x1B, 0x4080, 0x8004, 0x40,
};
hero_seq TIN_seq_bonk_hit[6] = {
    0, 0x8007, 0x1E, 0x4180, 0x8004, 0x40,
};
hero_seq KON_seq_bonk_hit[6] = {
    0, 0x8007, 0x1E, 0x4080, 0x8004, 0x40,
};
hero_seq CRA_seq_bonk_winner[6] = {
    0, 0x8007, 0x1A, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_bonk_winner[6] = {
    0, 0x8007, 0x5A, 0x4180, 0x8000, 0,
};
hero_seq CTX_seq_bonk_winner[10] = {
    0, 0x8007, 0xE, 0x4280, 0x8007, 0x29, 0x428E, 0x8000, 3, 0,
};
hero_seq BRO_seq_bonk_winner[10] = {
    0, 0x8007, 0x26, 0x4280, 0x8007, 0x29, 0x42A6, 0x8000, 3, 0,
};
hero_seq DNG_seq_bonk_winner[10] = {
    0, 0x8007, 0x25, 0x4280, 0x8007, 0xF, 0x4296, 0x8000, 3, 0,
};
hero_seq KAN_seq_bonk_winner[6] = {
    0, 0x8007, 0xD, 0x4200, 0x8000, 0,
};
hero_seq TIN_seq_bonk_winner[10] = {
    0, 0x8007, 0x21, 0x4280, 0x8007, 0x2E, 0x42A1, 0x8000, 3, 0,
};
hero_seq KON_seq_bonk_winner[10] = {
    0, 0x8007, 0x21, 0x4200, 0x8007, 0x2F, 0x4221, 0x8000, 3, 0,
};
hero_seq CRA_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4080, 0x8003, 8, 0x4088, 0x4088, 0x4088, 0x8000, 0, 0,
};
hero_seq COC_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4200, 0x8003, 8, 0x4208, 0x4208, 0x4208, 0x8000, 0, 0,
};
hero_seq CTX_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4180, 0x8003, 8, 0x4188, 0x4188, 0x4188, 0x8000, 0, 0,
};
hero_seq BRO_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4180, 0x8003, 8, 0x4188, 0x4188, 0x4188, 0x8000, 0, 0,
};
hero_seq DNG_seq_bonk_backswing[12] = {
    0, 0x8007, 0xA, 0x4180, 0x8003, 8, 0x4189, 0x4189, 0x4189, 0x8000, 0, 0,
};
hero_seq KAN_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4180, 0x8003, 8, 0x4188, 0x4188, 0x4188, 0x8000, 0, 0,
};
hero_seq TIN_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4200, 0x8003, 8, 0x4208, 0x4208, 0x4208, 0x8000, 0, 0,
};
hero_seq KON_seq_bonk_backswing[12] = {
    0, 0x8007, 9, 0x4280, 0x8003, 8, 0x4288, 0x4288, 0x4288, 0x8000, 0, 0,
};
hero_seq CRA_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 7, 0x4089, 0x8003, 8, 0x4090, 0x4090, 0x4090, 0x8000, 0, 0,
};
hero_seq COC_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 7, 0x4209, 0x8003, 8, 0x4210, 0x4210, 0x4210, 0x8000, 0, 0,
};
hero_seq CTX_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 6, 0x4189, 0x8003, 8, 0x418F, 0x418F, 0x418F, 0x8000, 0, 0,
};
hero_seq BRO_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 6, 0x4189, 0x8003, 8, 0x418F, 0x418F, 0x418F, 0x8000, 0, 0,
};
hero_seq DNG_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 9, 0x418A, 0x8003, 8, 0x4193, 0x4193, 0x4193, 0x8000, 0, 0,
};
hero_seq KAN_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 8, 0x4189, 0x8003, 8, 0x4191, 0x4191, 0x4191, 0x8000, 0, 0,
};
hero_seq TIN_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 8, 0x4209, 0x8003, 8, 0x4211, 0x4211, 0x4211, 0x8000, 0, 0,
};
hero_seq KON_seq_bonk_extended_backswing[12] = {
    0, 0x8007, 8, 0x4289, 0x8003, 8, 0x4291, 0x4291, 0x4291, 0x8000, 0, 0,
};
hero_seq CRA_seq_bonk_swing[30] = {
    0, 0x4091, 0x4092, 0x8003, 0xC, 0x4093, 0x4094, 0x4095, 0x4096, 0x4097, 0x8003, 8, 0x4098,
    0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x409E, 0x409F, 0x40A0, 0x40A1, 0x40A2, 0x8003, 9,
    0x40A2, 0x40A2, 0x8000, 0, 0,
};
hero_seq COC_seq_bonk_swing[32] = {
    0, 0x4211, 0x4212, 0x8003, 0xC, 0x4213, 0x4214, 0x8003, 8, 0x4215, 0x4216, 0x4217, 0x4218,
    0x4219, 0x421A, 0x421B, 0x421C, 0x421D, 0x421E, 0x421F, 0x4220, 0x4221, 0x4222, 0x4223, 0x4224,
    0x4225, 0x8003, 9, 0x4225, 0x4225, 0x8000, 0,
};
hero_seq BRO_seq_bonk_swing[28] = {
    0, 0x4190, 0x4191, 0x8003, 0xC, 0x4192, 0x4193, 0x8003, 8, 0x4194, 0x4195, 0x4196, 0x4197,
    0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D, 0x419E, 0x419F, 0x8003, 9, 0x419F, 0x419F,
    0x8000, 0, 0,
};
hero_seq CTX_seq_bonk_swing[28] = {
    0, 0x4190, 0x4191, 0x8003, 0xC, 0x4192, 0x4193, 0x8003, 8, 0x4194, 0x4195, 0x4196, 0x4197,
    0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D, 0x419E, 0x419F, 0x8003, 9, 0x419F, 0x419F,
    0x8000, 0, 0,
};
hero_seq DNG_seq_bonk_swing[28] = {
    0, 0x4194, 0x4195, 0x4196, 0x4197, 0x8003, 8, 0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D,
    0x419E, 0x419F, 0x41A0, 0x41A1, 0x41A2, 0x41A3, 0x41A4, 0x8003, 9, 0x41A4, 0x41A4, 0x41A4,
    0x8000, 0, 0,
};
hero_seq KAN_seq_bonk_swing[32] = {
    0, 0x4191, 0x4192, 0x4193, 0x4194, 0x4195, 0x4196, 0x8003, 8, 0x4197, 0x4198, 0x4199, 0x419A,
    0x419B, 0x419C, 0x419D, 0x419E, 0x419F, 0x41A0, 0x41A1, 0x41A2, 0x41A3, 0x41A4, 0x41A5, 0x41A6,
    0x8003, 9, 0x41A6, 0x41A6, 0x8000, 0, 0,
};
hero_seq TIN_seq_bonk_swing[34] = {
    0, 0x4212, 0x4213, 0x8003, 0xC, 0x4214, 0x4215, 0x4216, 0x4217, 0x8003, 8, 0x4218, 0x4219,
    0x421A, 0x421B, 0x421C, 0x421D, 0x421E, 0x421F, 0x4220, 0x4221, 0x4222, 0x4223, 0x4224, 0x4225,
    0x4226, 0x4227, 0x8003, 9, 0x4227, 0x4227, 0x8000, 0, 0,
};
hero_seq KON_seq_bonk_swing[34] = {
    0, 0x4292, 0x4293, 0x8003, 0xC, 0x4294, 0x4295, 0x4296, 0x8003, 8, 0x4297, 0x4298, 0x4299,
    0x429A, 0x429B, 0x429C, 0x429D, 0x429E, 0x429F, 0x42A0, 0x42A1, 0x42A2, 0x42A3, 0x42A4, 0x42A5,
    0x42A6, 0x42A7, 0x8003, 9, 0x42A7, 0x42A7, 0x8000, 0, 0,
};
s32 BonkFlagSteps[3] = { (s32)0x80110000, 0x40000010, 0x8001 };
s32 BonkFlagAnimList[3] = { (s32)&BonkFlagSteps, 0, 0 };
ScriptInit BonkFlagScriptInit = { 0, (void *)&BonkFlagAnimList, 0x401 };
s16 BonkPreLoadDB[4] = { 0x1C, 0x17, -1, 0 };
char *BonkAnims[6] = {
    (char *)BonkAnimBreathe, (char *)BonkAnimMedium, (char *)BonkAnimAttack, (char *)BonkAnimHit,
    (char *)BonkAnimWin, NULL,
};
s16 BonkLevelDB[2] = { 0x53, -1 };
WWInf BonkWorldInfo[2] = {
    { 0x51, BonkAnims, BonkLevelDB, 0 },
    { 0x52, BonkAnims, BonkLevelDB, 0 },
};
GroupFile BonkFiles[6] = {
    &HeroModelBank[DB_Font], &DatIndex[341], &DatIndex[340], &HeroModelBank[DB_Loading], &DatIndex[337],
    &DatIndex[336], &HeroModelBank[DB_Interface], &DatIndex[339], &DatIndex[338], &HeroModelBank[DB_Powerups],
    &DatIndex[335], &DatIndex[334], &HeroModelBank[DB_MalletObjects], &DatIndex[333], &DatIndex[332],
    0, 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 0x1D's) */
file_group_load_info file_group_BONK = { &FileGroups[7], BonkFiles };
u16 BonkLUPtab[4] = { 3, 0x20D, 0x201, 0x1A02 };
TrigLookUp BonkLUP = { BonkLUPtab, 4 };
PreMap BONK_PREMAP = { BonkPreLoadDB, BonkWorldInfo, &BonkLUP };
/* the shell settings (engine world record 0x010E) */
shell_setup bonk_shell_info = {
    0x2000, 0x10, 0x1000, 0x80, 0x200, 0x190, VramAreas, VramNodePool, 0x20, 0x400,
};
u16 BonkHeroModels[10] = {
    0, 0x54, 0x55, 0x56, 0x57, 0x5A, 0x5B, 0x58, 0x59, 0,
};
s32 BonkRoundTime = 0x258;
s32 BonkUnused2[4] = { 0x258, 0x258, 0x1C2, 0x12C };
s32 BonkStoolSfx[4] = { 0x733, 0x734, 0x735, 0x736 };
s32 BonkTargetSfx[4] = { 0x71D, 0x71E, 0x71F, 0x720 };
s32 BonkObjectSfx[4] = { 0x3F5, 0x3F6, 0x3F7, 0x3F8 };
BonkEntry BonkEntries[10] = {0};
BonkEnt BonkEnts[3] = {0};
s32 BonkSkill = 0;
s32 BonkEntryCur = 0;
s32 BonkCrateTimer = 0;
s32 BonkCrateDelay = 0;
s32 BonkCrateCount = 0;
s32 BonkRoundTimer = 0;
s32 BonkRoundState = 0;
s32 BonkEntCur = 0;

void InitialiseBONKFirst() {
    HeroHooks.HeroModels = BonkHeroModels;
    HeroHooks.HeroFacing = BonkHeroFacing;
    HeroHooks.KeepInBounds = BONK_KeepInBounds;
    HeroHooks.PreMoveHero = BonkPreMoveHero;
    HeroHooks.PostMoveHero = BonkHeroPostMove;
    HeroHooks.HeroKickCollision = BonkHeroKickCollision;
    HeroHooks.TestAttack = BonkHeroTestSwing;
    HeroHooks.TestKick = HeroNoKick;
    HeroHooks.TestJump = HeroNoJump;
    HeroHooks.HeroExtraEffects = BonkHero_ExtraEffects;
    HeroHooks.ChooseWinner = BonkChooseWinner;
    HeroHooks.HeroBot = Bonk_HeroBot;
    HeroActHit = 0x42;
    HeroActWinner = 0x47;
    HeroActLoser = 0x48;
    BonkInitTweakAi();
}

void BonkInitEntries(void) {
    s32 i;

    BonkEntryCur = 0;
    BonkCrateCount = 0;
    BonkRoundState = 0;
    BonkRoundTimer = BonkRoundTime;
    for (i = 0; i < 10; i++) {
        BonkEntries[i].unk0 = 0x100;
        BonkEntries[i].flag = 0;
        BonkEntries[i].timer = 0;
    }
    BonkCrateTimer = 0;
    BonkCrateDelay = BonkEntryCount * 30;
}

void BonkCrateMaster(void) {
    s32 i;
    s32 *new_var;

    if (BonkCrateCount > 0) {
        BonkCrateCount -= 1;
    }
    if (BonkRoundTimer > 0 && BonkRoundState < 4) {
        BonkRoundTimer -= 1;
    }
    if (BonkEntryCur != 0) {
        for (i = 0; i < 10; i++) {
            NAS *n = (NAS *)BonkEntries[i].flag;
            if (n != 0 && n->UserInfo.status == 1) {
                KillNAS(n);
                BonkEntries[i].flag = 0;
            }
        }
    }
    if (BonkCrateTimer < 4) {
        if (BonkCrateDelay != 0) {
            BonkCrateDelay--;
        } else {
            BonkCrateTimer++;
            BonkCrateDelay = (&BonkEntryCount)[BonkCrateTimer] * 30;
        }
    }
    if (BonkEntryCur < (new_var = &BonkStoolCount)[BonkCrateTimer] && BonkCrateCount == 0) {
        BonkSpawnCrate();
        BonkCrateCount = 0x14;
    }
}


void PlotBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    SLONG_XYZ pos;
    SLONG_XYZ *v;
    s32 *p;
    s32 *q;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (BonkAI_PickEntry(&x, &z) != 0) {
            BonkEntries[nas->UserInfo.user2].timer = 1000;
            e->XYZ.x = x;
            e->XYZ.z = z;
            e->XYZ.y = World_floor_level - 0x2000;
            BonkKeepInBoundsB(e);
            nas->UserInfo.status = 2;
            if (nas->UserInfo.user1 == 0) {
                BonkEntries[nas->UserInfo.user2].timer = BonkStoolFrames[BonkSkill];
                nas->UserInfo.user1 = Random(0x14) + 10;
            } else {
                BonkEntries[nas->UserInfo.user2].timer = 0;
            }
            e->flags &= 0xFFFF7FFF;
            if (BonkRoundTimer == 0) {
                e->sub->entity = 0x2015;
                p = &BonkRoundTime;
                BonkRoundTimer = p[++BonkRoundState];
            } else {
                e->sub->entity = 0x2002;
            }
        }
        break;
    case 2:
        if (!(GamePause & 0x4000000)) {
            nas->UserInfo.user1--;
        }
        if (nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 10;
            e->xform.dirty = 1;
            e->xform.scale.vy = 0;
            e->flags |= 0x8000;
            e->XYZ.y = World_floor_level;
            if (e->sub->entity == 0x2015) {
                PlaySfx(0x72A, NULL, &e->XYZ, 0x1000, 0x1E00, 0);
            } else {
                PlaySfx(0x528, NULL, &e->XYZ, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 10:
        e->xform.scale.vy += 0x100;
        if (e->xform.scale.vy > 0xC00) {
            e->xform.scale.vy = 0xC00;
            nas->UserInfo.status = 12;
            nas->UserInfo.user1 = BonkStoolHeights[BonkCrateTimer] + Random(BonkStoolTable[BonkCrateTimer]);
            if (e->sub->entity == 0x2015) {
                pos = e->XYZ;
                pos.y -= 0x100;
                Bonk_SpawnPuff((s32 *)&pos, 3, &HeroModelBank[DB_MalletObjects]);
            }
        }
        e->xform.dirty = 1;
        break;
    case 12:
        if (BonkEntries[nas->UserInfo.user2].timer != 0) {
            BonkEntries[nas->UserInfo.user2].timer--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 13;
            nas->UserInfo.user1 = 70;
        }
        break;
    case 13:
    case 14:
        if (BonkEntries[nas->UserInfo.user2].timer != 0) {
            BonkEntries[nas->UserInfo.user2].timer--;
        }
        if (--nas->UserInfo.user1 <= 0) {
            if (nas->UserInfo.status == 13) {
                nas->UserInfo.status = 14;
                nas->UserInfo.user1 = 40;
            } else {
                nas->UserInfo.status = (GamePause & 0x4000000) ? 4 : 5;
            }
            break;
        }
        if (nas->UserInfo.status == 13) {
            e->xform.scale.vy = Random(0x200) + 0xB00;
        } else {
            e->xform.scale.vy = Random(0x800) + 0x800;
        }
        e->xform.dirty = 1;
        break;
    case 5:
        Bonk_SpawnPuff((s32 *)&e->XYZ, 0, &HeroModelBank[DB_MalletObjects]);
        v = &e->XYZ;
        BonkMalletHit((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(BonkStoolSfx[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Bonk_SpawnPuff((s32 *)&e->XYZ, 1, &HeroModelBank[DB_MalletObjects]);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        p = &BonkEntryCur;
        q = &BonkStoolCount;
        if (*p > q[BonkCrateTimer]) {
            nas->UserInfo.status = 1;
            (*p)--;
            e->XYZ.y = World_floor_level - 0x2000;
        } else {
            nas->UserInfo.status = 0;
            nas->UserInfo.user1 = 0;
        }
        break;
    }
}

s32 BonkAI_PickEntry(s32 *arg0, s32 *arg1) {
    s32 tries;
    s32 ok;
    s32 x;
    s32 z;
    s32 i;
    HeroInfo *h;
    BonkEntry *e;
    BonkEnt *f;
    EXTengine *eng;

    HeroInfo *heroes;

    tries = 0;
    heroes = Heroes;
    do {
        ok = 1;
        x = Random(0x1000) - 0x800;
        z = Random(0x1000) - 0x800;
        for (i = 0, h = heroes; i < 4; i++, h++) {
            eng = h->hero_eng;
            if ((csqrt(BonkDist2XZ(eng->XYZ.x, eng->XYZ.z, x, z)) >> 6) < 0x400) {
                ok = 0;
            }
        }
        if (ok) {
            if (BonkEntryCur != 0) {
                for (i = 0, e = BonkEntries; i < 10; i++, e++) {
                    if (e->flag != 0) {
                        eng = ((NAS *)e->flag)->Engine;
                        if ((csqrt(BonkDist2XZ(eng->XYZ.x, eng->XYZ.z, x, z)) >> 6) < 0x400) {
                            ok = 0;
                        }
                    }
                }
            }
            if (ok && BonkEntCur != 0) {
                for (i = 0, f = BonkEnts; i < 3; i++, f++) {
                    if (f->b != 0) {
                        eng = ((NAS *)f->b)->Engine;
                        if ((csqrt(BonkDist2XZ(eng->XYZ.x, eng->XYZ.z, x, z)) >> 6) < 0x300) {
                            ok = 0;
                        }
                    }
                }
            }
        }
        tries++;
        if (ok) {
            *arg0 = x;
            *arg1 = z;
            return 1;
        }
    } while (tries < 10);
    return 0;
}

void InitBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->xform.hasScale = 1;
        e->xform.scale.vz = 0;
        e->xform.scale.vy = 0;
        e->xform.scale.vx = 0;
        e->xform.dirty = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = BonkEntCur * 0x5A + 0x1E;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->u6C.dbase = &HeroModelBank[DB_MalletObjects];
            s->entity = 0x200F;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            LinkObject(nas);
            AddSolid(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        nas->UserInfo.user2 = BonkEntCur;
        BonkEnts[BonkEntCur].b = (s32)nas;
        BonkEntCur += 1;
    }
}

void PlotBonkCrate(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 x;
    s32 z;
    s32 s;
    SLONG_XYZ *v;

    if (e == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        e->XYZ.y = World_floor_level - 0x2000;
        if (nas->UserInfo.user1 == 0) {
            BonkEnts[nas->UserInfo.user2].c = BonkCrateFrames[BonkSkill];
            nas->UserInfo.user1 = Random(0x5A) + 0x5A;
        } else {
            BonkEnts[nas->UserInfo.user2].c = 0;
        }
        e->flags &= 0xFFFF7FFF;
        e->xform.rot.vy = Random(0x1000);
        nas->UserInfo.status = 2;
        break;
    case 2:
        if (!(GamePause & 0x4000000)) {
            if (nas->UserInfo.user1 > 0) {
                nas->UserInfo.user1--;
            } else {
                goto spawn;
            }
        }
        if (nas->UserInfo.user1 > 0) {
            break;
        }
    spawn:
        if (BonkAI_PickEntry(&x, &z) != 0) {
            e->XYZ.x = x;
            e->XYZ.z = z;
            e->XYZ.y = World_floor_level;
            BonkKeepInBoundsB(e);
            nas->UserInfo.status = 10;
            e->xform.scale.vz = 0;
            e->xform.scale.vx = 0;
            e->xform.scale.vy = 0;
            e->xform.dirty = 1;
            e->flags |= 0x8021;
        }
        break;
    case 10:
        e->xform.scale.vx += 0x100;
        if (e->xform.scale.vy >= 0x1000) {
            e->xform.scale.vx = 0x1000;
            nas->UserInfo.status = 12;
            e->flags &= ~0x21;
        }
        s = e->xform.scale.vx;
        e->xform.dirty = 1;
        e->xform.scale.vy = s;
        e->xform.scale.vz = s;
        break;
    case 12:
        if (BonkEnts[nas->UserInfo.user2].c > 0) {
            BonkEnts[nas->UserInfo.user2].c--;
        }
        break;
    case 5:
        Bonk_SpawnPuff((s32 *)&e->XYZ, 0, &HeroModelBank[DB_MalletObjects]);
        v = &e->XYZ;
        BonkMalletHit((VECTOR *)v, 0x380, -1, 0);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        PlaySfx(BonkStoolSfx[Random(4)], NULL, v, 0x1000, 0x1E00, 0);
        break;
    case 4:
        Bonk_SpawnPuff((s32 *)&e->XYZ, 2, &HeroModelBank[DB_MalletObjects]);
        e->flags &= 0xFFFF7FFF;
        nas->UserInfo.status = 6;
        break;
    case 6:
        nas->UserInfo.user1 = 0;
        e->XYZ.y = World_floor_level - 0x2000;
        nas->UserInfo.status = 0;
        break;
    }
}

s32 BonkAI_FindClosest(s32 arg0, s32 *arg1, s32 arg2) {
    BonkEntry *found;
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 d;
    s32 best;
    s32 i;
    BonkEntry *e;
    BonkEnt *f;

    found = 0;
    if (arg2 & 2) {
        best = BonkAIRange[Heroes[arg0].hero_name];
    } else {
        best = 0xFFFFFF;
    }
    x = Heroes[arg0].hero_eng->XYZ.x;
    z = Heroes[arg0].hero_eng->XYZ.z;
    if ((arg2 & 0x100) && BonkEntryCur != 0) {
        for (i = 0, e = BonkEntries; i < 10; i++, e++) {
            if (e->flag == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((BonkPad *)e->flag)->mode < 9) {
                    continue;
                }
                if (e->timer != 0) {
                    continue;
                }
            }
            if (((BonkPad *)e->flag)->mode < 0xB) {
                continue;
            }
            ex = ((BonkPad *)e->flag)->eng->XYZ.x;
            ez = ((BonkPad *)e->flag)->eng->XYZ.z;
            d = csqrt(BonkDist2XZ(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || angle_diff_abs(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    BONK_SET_CLOSEST(best, found, d, e);
                }
            }
        }
    }
    if (arg2 & 0x200) {
        i = 0;
        if (BonkEntCur != 0)
        for (f = BonkEnts; i < 3; i++, f++) {
            if (f->b == 0) {
                continue;
            }
            if (arg2 & 1) {
                if (((BonkPad *)f->b)->mode < 9) {
                    continue;
                }
                if (f->c != 0) {
                    continue;
                }
            }
            if (((BonkPad *)f->b)->mode < 0xB) {
                continue;
            }
            ex = ((BonkPad *)f->b)->eng->XYZ.x;
            ez = ((BonkPad *)f->b)->eng->XYZ.z;
            d = csqrt(BonkDist2XZ(x, z, ex, ez));
            d >>= 6;
            if (d < best) {
                if (!(arg2 & 4) || angle_diff_abs(get_angle(x - ex, z - ez), Heroes[arg0].hero_move->forward_angle) < 0x320) {
                    BONK_SET_CLOSEST(best, found, d, (BonkEntry *)f);
                }
            }
        }
    }
    if (found != 0) {
        *arg1 = (s32)found;
        return 1;
    }
    return 0;
}

void BonkHitTarget(s32 arg0, s32 arg1, BonkTgt2 *t) {
    BonkPad *p;
    EXTengine *e;
    s32 big;

    p = t->obj;
    if (p != NULL) {
        e = p->eng;
        big = 0;
        if (e != NULL) {
            big = ((subengine *)e->sub)->entity == 0x2015;
        }
        if (arg0 >= 0) {
            switch (p->mode) {
            case 0xE:
                BonkKnockBack(arg0, e, big ? 0x14 : 5);
                break;
            case 0xD:
                BonkKnockBack(arg0, e, big ? 0xF : 3);
                break;
            default:
                BonkKnockBack(arg0, e, big ? 0xA : 2);
                break;
            }
            PlaySfx(BonkTargetSfx[arg0], NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            if (big) {
                PlaySfx(0x72B, NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
        }
        p->mode = 4;
    }
}

void BonkMalletHit(VECTOR *pos, s32 a, s32 b, s32 c) {
    s32 x;
    s32 z;
    s32 ex;
    s32 ez;
    s32 ang;
    s32 i;
    HeroInfo *h;
    BonkEntry *e;
    EXTengine *eng;
    EXTheromove *mv;
    s32 vc;

    x = pos->vx;
    z = pos->vz;
    if (GameExitFlags & 0x801) {
        return;
    }
    i = 0;
    do {
        h = &Heroes[i];
        if (i == b) {
            goto next;
        }
        ex = h->hero_eng->XYZ.x;
        ez = h->hero_eng->XYZ.z;
        if ((csqrt(BonkDist2XZ(x, z, ex, ez)) >> 6) > a) {
            goto next;
        }
        if ((h->hero_immunity | h->hero_hit) != 0) {
            goto next;
        }
        ang = get_angle(x - ex, z - ez);
        mv = h->hero_move;
        vc = mv->velocity_calc;
        if (vc != 1) {
            if (vc != 0) {
                if (vc < 4) {
                    mv->x_vel -= (rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 5) >> 8;
                    h->hero_move->z_vel -= (rcossin_tbl[ang & 0xFFF].unk2 * 5) >> 8;
                }
            }
        }
        h->hero_move->reactive_angle = ang;
        h->hero_move->reactive_speed = 0x50;
        HeroTakeHit(i, NULL, -1, 0x8001, 0x446);
        h->hero_health = 0x14;
        pad_rumble_small(i, 0xF);
        BonkHidePowerup(i);
        PlaySfx(HeroHitSfx[h->hero_name][Random(4)], NULL, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
    next:
        i++;
    } while (i < 4);
    if ((c & 0x100) && BonkEntryCur != 0) {
        i = 0;
        do {
            e = &BonkEntries[i];
            if (e->flag != 0) {
                eng = ((NAS *)e->flag)->Engine;
                if ((csqrt(BonkDist2XZ(x, z, eng->XYZ.x, eng->XYZ.z)) >> 6) <= a
                    && ((BonkPad *)e->flag)->mode >= 0xB) {
                    BonkHitTarget(b, 1, (BonkTgt2 *)e);
                }
            }
            i++;
        } while (i < 10);
    }
}

void BonkChooseWinnerScore(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    GameState *g;

    if (arg0 == -1) {
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
            GAME->adventure.rank[j] = best;
            taken |= 1 << best;
        }
        g = GameInfo;
        if (g->teams == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g->heroOut[g->adventure.rank[0]] != 0
                || (hb = Heroes, a = &hb[g->adventure.rank[0]], a->hero_score == hb[g->adventure.rank[1]].hero_score)) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                GAME->winner[0] = a->hero_name;
                GAME->exitFlags |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (GAME->adventure.rank[0] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[g->teamLeader[0]];
            b = &hb[g->teamLeader[1]];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                g->exitFlags |= 0x800;
                if (g->heroOut[g->teamLeader[0]] == 0) {
                    g->winner[0] = a->hero_name;
                    g->winner[1] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g->winner[0] = b->hero_name;
                    g->winner[1] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void BonkChooseWinnerScoreB(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    GameState *g;

    if (arg0 == -1) {
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
            GAME->adventure.rank[j] = best;
            taken |= 1 << best;
        }
        g = GameInfo;
        if (g->teams == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g->heroOut[g->adventure.rank[0]] != 0
                || (hb = Heroes, a = &hb[g->adventure.rank[0]], a->hero_score == hb[g->adventure.rank[1]].hero_score)) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                GAME->winner[0] = a->hero_name;
                GAME->exitFlags |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (GAME->adventure.rank[0] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[g->teamLeader[0]];
            b = &hb[g->teamLeader[1]];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                g->exitFlags |= 0x800;
                if (g->heroOut[g->teamLeader[0]] == 0) {
                    g->winner[0] = a->hero_name;
                    g->winner[1] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g->winner[0] = b->hero_name;
                    g->winner[1] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroOut[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void BonkChooseWinnerTimed(s32 arg0, u16 arg1) {
    s32 found;
    s32 fail;
    GameState *g;
    s32 i;

    found = -1;
    fail = 0;
    g = GameInfo;
    if (g->teams != 0) {
        HeroInfo *hb = Heroes;
        HeroInfo *a;
        HeroInfo *ta;
        HeroInfo *b;
        s32 sa;
        s32 sb;

        a = &hb[g->teamLeader[0]];
        ta = &hb[a->hero_teammate];
        b = &hb[g->teamLeader[1]];
        sa = a->hero_score + ta->hero_score;
        sb = b->hero_score + hb[b->hero_teammate].hero_score;
        if (sa == 0) {
            if (g->heroOut[g->teamLeader[0]] == 0) {
                GAME->winner[0] = a->hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroOut[i] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        } else if (sb == 0) {
            if (g->heroOut[g->teamLeader[1]] == 0) {
                GAME->winner[0] = b->hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroOut[i] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (!Heroes[i].hero_score) {
                found = i;
            }
        }
        if (found != -1) {
            if (GAME->heroOut[found] == 0) {
                GAME->winner[0] = Heroes[found].hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroOut[i] != 0) {
                        Heroes[i].hero_status |= 0x400;
                    } else {
                        Heroes[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    }
    if (fail || arg0 == -1) {
        GAME->winner[0] = 0;
        GAME->exitFlags |= arg1;
        Clock_flags |= 0x200;
        for (i = 0; i < 4; i++) {
            if (GAME->heroOut[i] != 0) {
                Heroes[i].hero_status |= 0x400;
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void BonkChooseWinnerRank(s32 arg0, u16 arg1) {
    GameState *g;
    GameState *p;
    HeroInfo *hb;
    HeroInfo *h;
    HeroInfo *top;
    HeroInfo *ta;
    HeroInfo *b;
    HeroInfo *am;
    HeroInfo *bm;
    s32 used;
    s32 i;
    s32 k;
    s32 n;
    s32 j;
    s32 best;
    s32 sa;
    s32 sb;

    if (arg0 != -1) {
        return;
    }
    used = 0;
    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((used >> j) & 1) && (Heroes[j].hero_score > Heroes[best].hero_score || best == -1)) {
                best = j;
            }
        }
        RANK66V->adventure.rank[i] = best;
        used |= 1 << best;
    }
    g = RANK66V;
    if (g->teams == 0) {
        h = Heroes;
        top = &h[g->adventure.rank[0]];
        if (top->hero_score == h[g->adventure.rank[1]].hero_score) {
            g->winner[0] = 0;
            g->exitFlags |= arg1;
            for (k = 0; k < 4; k++) {
                Heroes[k].hero_status |= 0x100;
            }
            return;
        }
        p = g;
        p->winner[0] = top->hero_name;
        p->exitFlags |= 0x800;
        for (n = 0; n < 4; n++) {
            if (p->adventure.rank[0] == n) {
                h[n].hero_status |= 0x200;
            } else {
                h[n].hero_status |= 0x400;
            }
        }
        return;
    }
    InitBigScore();
    hb = Heroes;
    ta = &hb[g->teamLeader[0]];
    b = &hb[g->teamLeader[1]];
    am = &hb[ta->hero_teammate];
    bm = &hb[b->hero_teammate];
    sa = ta->hero_score + am->hero_score;
    sb = b->hero_score + bm->hero_score;
    if (sa == sb) {
        g->winner[0] = 0;
        g->exitFlags |= arg1;
        for (k = 0; k < 4; k++) {
            Heroes[k].hero_status |= 0x100;
        }
        return;
    }
    g->exitFlags |= 0x800;
    if (sb < sa) {
        g->winner[0] = ta->hero_name;
        g->winner[1] = am->hero_name;
        ta->hero_status |= 0x200;
        am->hero_status |= 0x200;
        b->hero_status |= 0x400;
        bm->hero_status |= 0x400;
    } else {
        RANK66V->winner[0] = b->hero_name;
        RANK66V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void Bonk_HeroBot(s32 arg0)
{
    GameState *g = GAME;
    HeroInfo *h;
    HeroInfo *heroes;
    EXTbotai *ai;
    s32 sx;
    s32 sz;
    HeroInfo *me;
    u32 i;
    BonkHit *hit;
    u16 mode;
    u16 ang;

    if (g->pause & 0x04000000) {
        return;
    }
    heroes = Heroes;
    h = &heroes[arg0];
    ai = h->hero_ai;
    if (ai == NULL) {
        return;
    }
    if (ai->AItimer1 != 0) {
        ai->AItimer1--;
    }
    ai->AIpad = 0;
    if (BonkAI_Closest(arg0, (int *)&hit) != 0) {
        if (hit->nas != ai->pTargetNas && ai->AItimer1 == 0) {
            ai->pTargetNas = hit->nas;
            ai->TargetXYZ = hit->nas->Engine->XYZ;
            ai->AItimer1 = 10;
            if (hit->kind == 0x100) {
                if (BonkSlots[arg0].f8 == 1 || (g->levelFlags & 0x80) ||
                    BonkAI_NearestEnemy(arg0) != 0) {
                    ai->AImode = 2;
                } else {
                    mode = 3;
                    if (GameFrameCount & 1) {
                        mode = 2;
                    }
                    ai->AImode = mode;
                }
            } else {
                if (BonkSlots[arg0].f8 == 1 || (g->levelFlags & 0x80) ||
                    BonkAI_NearestEnemy(arg0) != 0) {
                    ai->AImode = 4;
                } else {
                    mode = 5;
                    if (GameFrameCount & 1) {
                        mode = 4;
                    }
                    ai->AImode = mode;
                }
            }
        }
    } else {
        sz = 0;
        sx = 0;
        i = 0;
        me = h;
        while (1) {
            if (i < 4) {
                if (i != arg0) {
                    if ((csqrt(bonk_sq(me->hero_eng->XYZ.x, me->hero_eng->XYZ.z,
                                        heroes[i].hero_eng->XYZ.x, heroes[i].hero_eng->XYZ.z)) >> 6) < 0x1000) {
                        sx += me->hero_eng->XYZ.x - heroes[i].hero_eng->XYZ.x;
                        sz += me->hero_eng->XYZ.z - heroes[i].hero_eng->XYZ.z;
                    }
                }
                i++;
                continue;
            }
            break;
        }
        if (sx != 0 || sz != 0) {
            ang = get_angle(sx, sz);
            ai->AIang = ang;
            ai->AIpad |= DirPadBits[(ang >> 9) & 7];
            ai->AImode = 1;
        }
    }
    switch (ai->AImode) {
    case 1:
        break;
    case 2:
    case 3:
        if ((csqrt(bonk_sq(ai->pTargetNas->Engine->XYZ.x, ai->pTargetNas->Engine->XYZ.z,
                            Heroes[arg0].hero_eng->XYZ.x, Heroes[arg0].hero_eng->XYZ.z)) >> 6) <
            BonkAIRange[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 3 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 2 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
        goto face;
    case 4:
    case 5:
        if ((csqrt(bonk_sq(ai->pTargetNas->Engine->XYZ.x, ai->pTargetNas->Engine->XYZ.z,
                            Heroes[arg0].hero_eng->XYZ.x, Heroes[arg0].hero_eng->XYZ.z)) >> 6) <
            BonkAIRange[Heroes[arg0].hero_name]) {
            if ((ai->AImode == 5 && Heroes[arg0].hero_mode != 0x44) ||
                (ai->AImode == 4 && Heroes[arg0].hero_mode != 0x43)) {
                ai->AIpad |= 0x8000;
            }
        }
    face:
        ang = get_angle(ai->pTargetNas->Engine->XYZ.x - Heroes[arg0].hero_eng->XYZ.x,
                        ai->pTargetNas->Engine->XYZ.z - Heroes[arg0].hero_eng->XYZ.z);
        ai->AIang = ang;
        ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        break;
    }
    set_joypad((u8 *)&Pads[arg0], ai->AIpad);
}

s32 BonkAI_NearestEnemy(s32 arg0)
{
  s32 new_var2;
  EXTengine *b;
  EXTengine *a;
  s32 dx;
  s32 dz;
  s32 new_var;
  s32 r;
  if ((*GameTeams) != 0)
  {
    HeroInfo *t = &Heroes[Heroes[arg0].hero_teammate];
    a = Heroes[arg0].hero_eng;
    b = t->hero_eng;
    new_var = a->XYZ.z;
    new_var2 = a->XYZ.x;
    dx = b->XYZ.x - new_var2;
    dz = b->XYZ.z - new_var;
    r = (csqrt((dx * dx) + (dz * dz)) >> 6) < 0x501;
  }
  else
  {
    r = 0;
  }
  return r;
}

int BonkAI_Closest(int a0, int *out)
{
    int v;

    if (BonkAI_FindClosest(a0, &v, 0x301) != 0) {
        *out = v;
        return 1;
    }
    return 0;
}

void BonkAI_Reset(s32 arg0) {
    EXTbotai *ai = Heroes[arg0].hero_ai;

    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
    }
}

void BonkChooseWinner(s32 arg0, s32 arg1) {
    GameState *g = GAME;

    if (!(g->exitFlags & 0x801)) {
        u16 f = g->levelFlags;

        if (f & 0x80) {
            BonkChooseWinnerScore(arg0, (u16)arg1);
        } else if (f & 0x20) {
            BonkChooseWinnerScoreB(arg0, (u16)arg1);
        } else if (f & 0x40) {
            BonkChooseWinnerTimed(arg0, (u16)arg1);
        } else {
            BonkChooseWinnerRank(arg0, (u16)arg1);
        }
    }
}

s32 BonkDist2XZ(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a0 = arg2 - arg0;
    temp_a1 = arg3 - arg1;
    return (temp_a0 * temp_a0) + (temp_a1 * temp_a1);
}

void PlotBonkFlag(NAS *nas) {
    RunNasAnim(nas);
}

void InitBonkFlag(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        ScriptCursorFirst(nas, &BonkFlagScriptInit);
        e->sub->u6C.dbase = &HeroModelBank[DB_ArenaMedievalMalletArena];
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;
    s32 v;

    if (e != NULL) {
        s32 a = nas->UserInfo.user1;
        s32 b = nas->UserInfo.user2;

        if (a >= b) {
            KillNAS(nas);
        } else {
            s32 t = a - 0x800;

            nas->UserInfo.user1 = a + ((b - t) >> 1);
        }
        e->xform.rot.vy += 0x71;
        v = nas->UserInfo.user1;
        e->xform.dirty = 1;
        e->xform.scale.vz = v;
        e->xform.scale.vx = v;
    }
}

void InitBonkShockwave(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_MalletObjects];
        e->sub->entity = 0x2014;
        e->xform.hasScale = 1;
        e->xform.scale.vy = 0x1000;
        e->xform.order = 0;
        e->xform.scale.vz = 0;
        e->xform.scale.vx = 0;
        e->xform.dirty = 1;
        e->flags |= 0x8021;
        nas->UserInfo.user1 = 0;
        nas->UserInfo.user2 = 0x4000;
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BonkHitObject(s32 arg0, s32 arg1, BonkObj *obj) {
    BonkPad *p = obj->pad;

    if (p != NULL) {
        BonkHeroHitFlash(arg0, Random(2));
        p->mode = 4;
        PlaySfx(BonkObjectSfx[Random(4)], NULL, &Heroes[arg0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
    }
}

void BonkUpdateStub(void) {
}

void BonkSpawnStool(void) {
    s32 i;

    BonkEntCur = 0;
    for (i = 0; i < 3; i++) {
        BonkEnts[i].a = 0x200;
        BonkEnts[i].b = 0;
        BonkEnts[i].c = 0;
    }
    for (i = 0; i < 2; i++) {
        CreateNAS(0, 0, 0, 0x1A04, 0x11);
    }
}

void InitBonkStool(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        subengine *s;

        e->xform.hasScale = 1;
        e->xform.scale.vz = 0xC00;
        e->xform.scale.vy = 0xC00;
        e->xform.scale.vx = 0xC00;
        e->xform.dirty = 1;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = 0;
        s = e->sub;
        e->flags &= 0xFFFF7FFF;
        if (s != NULL) {
            s->u6C.dbase = &HeroModelBank[DB_MalletObjects];
            s->entity = 0x2002;
        } else {
            nas->Failure = 1;
        }
        if (AddObject(nas) != NULL) {
            LinkObject(nas);
            AddSolid(nas, 0x200);
        } else {
            nas->Failure = 1;
        }
    } else {
        nas->Failure = 1;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BonkSpawnCrate(void) {
    NAS *n;

    if (BonkEntryCur < 10) {
        n = CreateNAS(0, World_floor_level - 0x2000, 0, 0x1A01, 0x11);
        if (n != NULL) {
            n->UserInfo.user1 = Random(10) + BonkEntryCur * 5;
            n->UserInfo.user2 = BonkEntryCur;
            BonkEntries[BonkEntryCur].flag = (s32)n;
            BonkEntryCur += 1;
        }
    }
}

void BonkUpdateSkill(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (GAME->heroOut[i] != 0) {
            BonkSkill = AiPlayerSkill[i].level;
        }
    }
}

void UpdateBONK(void) {
    BonkCrateMaster();
    BonkUpdateStub();
    BonkUpdateSkill();
}

void BonkInitTweakAi(void) {
    AiHumanRoundTweakTable = GameTeams[0] != 0 ? BonkAiHumanRoundTweakAlt : BonkAiHumanRoundTweak;
    AiSkillTable = GameTeams[0] != 0 ? BonkAiSkillAlt : BonkAiSkill;
    AiTeamSkillTable = BonkAiTeamSkill;
}

void BonkSetClock(s32 arg0) {
    Set_Clock(arg0 & 0xFFFF, 0x81D0);
}

void BonkSetClockA(void) {
    if (!(GameLevelFlags & 0xE0)) {
        BonkSetClock(0x130);
    }
}

void BonkSetClockB(void) {
    if (GameLevelFlags & 0x20) {
        BonkSetClock(0x130);
    }
}

void BonkInitScores(void) {
    s32 i;
    s16 v;
    GameState *g = GameInfo;

    GameState *h;

    if (g->levelFlags & 0x40) {
        i = 0;
        h = g;
        for (; i < 4; i++) {
            v = 0x3C;
            if (((u8 *)h)[i + 0x5C] == 0) {  /* matching: h + i first; heroOut[i] adds the other way round */
                v = 0x50;
                if (h->teams != 0) {
                    v = 0x5A;
                }
            }
            Heroes[i].hero_score_displayed = v;
            Heroes[i].hero_score = v;
        }
        BonkSetClock(0x130);
        InitSharedScoreIcons();
        if (GameTeams[0] != 0) {
            PanelCtrl |= 0x2000;
        }
    }
}

void BonkSetClockC(void) {
    if (GameLevelFlags & 0x80) {
        BonkSetClock(0x130);
    }
}

void BonkInitCamera(void) {
    camera_info_tag *cam = &MainCamera;
    cam->eye.x = 0;
    cam->eye.z = 0xE98;
    cam->xform.angle.vx = 0x192;
    cam->dist = 0x200;
    cam->at.x = 0x60;
    cam->at.y = 0x20;
    cam->eye.y = 0;
    cam->xform.angle.vy = 0;
    cam->xform.angle.vz = 0;
    cam->at.z = -0x660;
}

void InitialiseBONK(void) {
    World_floor_level = -0x10;
    Init_Clock(0x140, 0x28, 0);
    Set_Clock(0x130, 0x81D0);
    BonkInitCamera();
    BonkSkill = 0x10;
    BonkUpdateSkill();
    BonkSetClockC();
    BonkInitScores();
    BonkSetClockB();
    BonkSetClockA();
    BonkBounceAll();
    BonkInitEntries();
    BonkSpawnStool();
}

void BONKSetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseBONKFirst;
    LevelSpecs.LevelInit = InitialiseBONK;
    LevelSpecs.LevelRestartInit = InitialiseBONK;
    LevelSpecs.LevelRoutine = UpdateBONK;
    LevelSpecs.EarliestTriggers = FirstTriggersBONK;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
}

void BonkRestartLevel(void) {
}

void FirstTriggersBONK(void) {
}

void BonkKnockBack(s32 arg0, EXTengine *p, s32 arg2) {
    HeroInfo *h;
    HeroInfo *t;
    HeroInfo *base, *b2, *b3, *h2;
    s32 v;
    s32 s;
    s32 x, y, z;
    GameState *g;

    if (p != NULL) {
        x = p->XYZ.x;
    } else {
        x = Heroes[arg0].hero_eng->XYZ.x;
    }
    if (p != NULL) {
        y = p->XYZ.y;
    } else {
        y = Heroes[arg0].hero_eng->XYZ.y;
    }
    if (p != NULL) {
        z = p->XYZ.z;
    } else {
        z = Heroes[arg0].hero_eng->XYZ.z;
    }
    CreateFloatyScore(x, y, z, arg2, 0);
    g = GAME;
    if (g->levelFlags & 0x40) {
        if (g->teams != 0) {
            base = Heroes;
            h = base + arg0;
            s = h->hero_score;
            if (s < arg2) {
                h->hero_score = 0;
                t = base + h->hero_teammate;
                t->hero_score = t->hero_score - (arg2 - s);
                return;
            }
        }
        b2 = Heroes;
        h2 = arg0 + b2;
        v = (u16)h2->hero_score - arg2;
    } else {
        b3 = Heroes;
        h2 = arg0 + b3;
        v = (u16)h2->hero_score + arg2;
    }
    h2->hero_score = v;
}

void BonkHeroHitFlash(s32 arg0, s32 arg1) {
    EXTengine *e;
    subengine *s;
    HeroInfo *h;
    HeroInfo *base;

    s = NULL;
    BonkHidePowerup(arg0);
    base = Heroes;
    h = &base[arg0];
    e = h->hero_powerup;
    if (e != NULL) {
        s = e->sub;
    }
    switch (arg1) {
    case 0:
        BonkSlots[arg0].fC = 0xB4;
        if (s != NULL) {
            s->u6C.dbase = &HeroModelBank[DB_MalletObjects];
            s->entity = 0x2013;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        PlaySfx(0x41C, NULL, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
        break;
    case 1:
        if (s != NULL) {
            s->u6C.dbase = &HeroModelBank[DB_Powerups];
            s->entity = 0x200C;
            s->flags |= 0x8000;
            e->flags |= 0x8000;
        }
        PlaySfx(0x72E, NULL, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
        break;
    default:
        return;
    }
    BonkSlots[arg0].f8 = arg1;
}

void BonkHero_ExtraEffects(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    EXTengine *p = h->hero_powerup;

    if (p != NULL) {
        EXTengine *e = h->hero_eng;
        p->XYZ = e->XYZ;
        p->XYZ.y -= BonkPowerupTimes[h->hero_name];
        p->xform.rot.vy += 0x38;
        p->xform.dirty = 1;
    }
    if (BonkSlots[player].f8 == 0) {
        if (--BonkSlots[player].fC <= 0) {
            BonkHidePowerup(player);
        }
    }
}
