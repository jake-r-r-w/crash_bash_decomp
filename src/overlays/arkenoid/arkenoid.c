#include "common.h"
#include "bash/main.h"
#include "bash/arkenoid_ovl.h"
#include "bash/arkenoid.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_80050100[];
extern u8 D_8005010C[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char ArkAnimBreathe[] = "BREATHE";
const char ArkAnimBank[] = "BANK";
const char ArkAnimIdleA[] = "IDLE_A";
const char ArkAnimTauntA[] = "TAUNT_A";
const char ArkAnimDie[] = "DIE";
const char ArkAnimWin[] = "WIN";

/* data, in address order */
s16 ArkPreLoadDB[6] = { 0x1C, 0x17, 0x1A, 0x7B, 0x7A, -1 };
char *ArkAnims[7] = {
    (char *)ArkAnimBreathe, (char *)ArkAnimBank, (char *)ArkAnimIdleA, (char *)ArkAnimTauntA,
    (char *)ArkAnimDie, (char *)ArkAnimWin, NULL,
};
s16 ArkLevelDB0[2] = { -1, 0 };
s16 ArkLevelDB1[2] = { 0x7C, -1 };
WWInf ArkWorldInfo[8] = {
    { 0x6A, ArkAnims, ArkLevelDB0, 7 },
    { 0x6C, ArkAnims, ArkLevelDB0, 7 },
    { 0x6E, ArkAnims, ArkLevelDB1, 7 },
    { 0x70, ArkAnims, ArkLevelDB0, 7 },
    { 0x6B, ArkAnims, ArkLevelDB0, 7 },
    { 0x6D, ArkAnims, ArkLevelDB0, 7 },
    { 0x6F, ArkAnims, ArkLevelDB1, 7 },
    { 0x71, ArkAnims, ArkLevelDB0, 7 },
};
s32 ArkFiles[24] = {
    (s32)&FontModel, (s32)&DatIndex[2 * 517], (s32)&DatIndex[2 * 516], (s32)&LoadingModel, (s32)&DatIndex[2 * 513],
    (s32)&DatIndex[2 * 512], (s32)&InterfaceModel, (s32)&DatIndex[2 * 515], (s32)&DatIndex[2 * 514], (s32)&PowerupsModel,
    (s32)&DatIndex[2 * 511], (s32)&DatIndex[2 * 510], (s32)&AngelModel, (s32)&DatIndex[2 * 509], (s32)&DatIndex[2 * 508],
    (s32)&BallsVehicleModel, (s32)&DatIndex[2 * 505], (s32)&DatIndex[2 * 504], (s32)&BallsEffectsModel, (s32)&DatIndex[2 * 507],
    (s32)&DatIndex[2 * 506], 0, 0, 0,
};
s32 ArkNginFiles[27] = {
    (s32)&FontModel, (s32)&DatIndex[2 * 503], (s32)&DatIndex[2 * 502], (s32)&LoadingModel, (s32)&DatIndex[2 * 499],
    (s32)&DatIndex[2 * 498], (s32)&InterfaceModel, (s32)&DatIndex[2 * 501], (s32)&DatIndex[2 * 500], (s32)&PowerupsModel,
    (s32)&DatIndex[2 * 497], (s32)&DatIndex[2 * 496], (s32)&AngelModel, (s32)&DatIndex[2 * 495], (s32)&DatIndex[2 * 494],
    (s32)&BallsVehicleModel, (s32)&DatIndex[2 * 493], (s32)&DatIndex[2 * 492], (s32)&BallsEffectsModel, (s32)&DatIndex[2 * 491],
    (s32)&DatIndex[2 * 490], (s32)&NginModel, (s32)&DatIndex[2 * 489], (s32)&DatIndex[2 * 488], 0, 0,
};
/* the four levels' files (modes 14..17; the engine's table at 0x800A0030): the exe's file group
 * and the model list; N. Gin's has more models */
file_group_load_info file_group_BALLS = { D_8005010C, ArkFiles };
file_group_load_info file_group_BEACHBALL = { D_8005010C, ArkFiles };
file_group_load_info file_group_NGINBALL = { D_80050100, ArkNginFiles };
file_group_load_info file_group_SKYBALL = { D_8005010C, ArkFiles };
u16 ArkLUPtab[4] = { 3, 0x1605, 0x1508, 0 };
TrigLookUp ArkLUP = { ArkLUPtab, 3 };
PreMap ARKENOID_PREMAP = { ArkPreLoadDB, ArkWorldInfo, &ArkLUP };
shell_setup arkenoid_shell_info = {
    0x4000, 0x20, 0x1800, 0xC8, 0x2BC, 0x200, D_80063EC8, D_80063BB8, 0x20, 0x5DC,
};
u16 ArkHeroModels[10] = { 0, 0x72, 0x73, 0x74, 0x75, 0x78, 0x79, 0x76, 0x77, 0 };
u16 ArkBallsAiSkill[10] = { 9, 6, 3, 1, 8, 0x12, 0x10, 0x10, 0x10, 0x14 };
u16 ArkBallsAiSkillAlt[10] = { 9, 6, 3, 1, 0xE, 0x15, 0x12, 0x10, 0x10, 0x15 };
u16 ArkBallsAiTeamSkill[6] = { 2, 0xA, 0x12, 2, 0xA, 0x12 };
u16 ArkBallsAiHumanRoundTweak[12] = { 8, 0x10, 0x18, 7, 0xE, 0x15, 6, 0xC, 0x13, 5, 0xA, 0x10 };
u16 ArkBallsAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkBallsAiHumanTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkBeachAiSkill[10] = { 0xD, 0xA, 7, 4, 9, 0x13, 0x10, 0x10, 0x10, 0x18 };
u16 ArkBeachAiSkillAlt[10] = { 0x10, 0xD, 0xA, 7, 0x11, 0x16, 0x10, 0x10, 0x10, 0x1A };
u16 ArkBeachAiTeamSkill[6] = { 2, 0xB, 0x13, 2, 0xB, 0x13 };
u16 ArkBeachAiHumanRoundTweak[12] = { 0xE, 0x15, 0x1C, 0xC, 0x13, 0x19, 0xA, 0xF, 0x15, 8, 0xD, 0x12 };
u16 ArkBeachAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkBeachAiHumanTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkNginAiSkill[10] = { 0x10, 0xD, 0xA, 7, 0xE, 0x12, 0x14, 0x10, 0x10, 0x15 };
u16 ArkNginAiSkillAlt[10] = { 0x10, 0xD, 0xA, 7, 0xE, 0x14, 0x14, 0x10, 0x10, 0x15 };
u16 ArkNginAiTeamSkill[6] = { 2, 0xB, 0x12, 2, 0xB, 0x12 };
u16 ArkNginAiHumanRoundTweak[12] = { 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14 };
u16 ArkNginAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkNginAiHumanTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkSkyAiSkill[10] = { 0x12, 0xF, 0xC, 8, 0x13, 0x14, 0x12, 0x10, 0x10, 0x16 };
u16 ArkSkyAiSkillAlt[10] = { 0x14, 0x11, 0xE, 0xA, 0x15, 0x15, 0x12, 0x10, 0x10, 0x18 };
u16 ArkSkyAiTeamSkill[6] = { 6, 0xE, 0x14, 6, 0xE, 0x14 };
u16 ArkSkyAiHumanRoundTweak[12] = { 0x14, 0x16, 0x20, 0x12, 0x15, 0x1C, 0x10, 0x14, 0x18, 0xC, 0x12, 0x14 };
u16 ArkSkyAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 ArkSkyAiHumanTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
s32 ArkVictorySeqs[16] = {
    (s32)&ArkAnimWin, 0x1F, (s32)&ArkAnimWin, 0x1F, (s32)&ArkAnimWin, 0x1F, (s32)&ArkAnimWin,
    0x1F, 0x20001, 0x10002, 0x10002, 0x20001, 0x20001, 0x10002, 0x10002, 0x20001,
};
ArkenoidCircle ArkBallsPosts[4] = {
    { 0x800, 0x800, 0x200 }, { 0x800, -0x800, 0x200 }, { -0x800, 0x800, 0x200 },
    { -0x800, -0x800, 0x200 },
};
ArkenoidCircle ArkBallsCircles[4] = { { 0, -0x1000, 0x200 }, { 0, 0x1000, 0x200 }, { 0x1000, 0, 0x200 }, { -0x1000, 0, 0x200 } };
ArkenoidCircle ArkBallsBumpers[8] = {
    { -0x800, -1, -0x20 }, { -1, -0x800, -1 }, { 0x800, 0, -0x20 }, { -1, -0x800, -1 },
    { -0x800, -1, -0x20 }, { -1, 0x800, 0 }, { 0x800, 0, -0x20 }, { -1, 0x800, 0 },
};
ArkenoidCircle ArkBeachPosts[4] = {
    { 0x800, 0x800, 0x200 }, { 0x800, -0x800, 0x200 }, { -0x800, 0x800, 0x200 },
    { -0x800, -0x800, 0x200 },
};
ArkenoidCircle ArkBeachCircles[4] = { { 0, -0x1000, 0x200 }, { 0, 0x1000, 0x200 }, { 0x1000, 0, 0x200 }, { -0x1000, 0, 0x200 } };
ArkenoidCircle ArkBeachBumpers[8] = {
    { -0x800, -1, -0x20 }, { -1, -0x800, -1 }, { 0x800, 0, -0x20 }, { -1, -0x800, -1 },
    { -0x800, -1, -0x20 }, { -1, 0x800, 0 }, { 0x800, 0, -0x20 }, { -1, 0x800, 0 },
};
ArkenoidCircle ArkNginPosts[4] = {
    { 0x850, 0x850, 0x1A0 }, { 0x850, -0x850, 0x1A0 }, { -0x850, 0x850, 0x1A0 },
    { -0x850, -0x850, 0x1A0 },
};
ArkenoidCircle ArkNginCircles[4] = { { 0, -0x1000, 0x200 }, { 0, 0x1000, 0x200 }, { 0x1000, 0, 0x200 }, { -0x1000, 0, 0x200 } };
ArkenoidCircle ArkNginBumpers[8] = {
    { -0x900, -1, -0x500 }, { -1, -0x900, -1 }, { 0x900, 0, -0x500 }, { -1, -0x900, -1 },
    { -0x900, -1, -0x500 }, { -1, 0x900, 0 }, { 0x900, 0, -0x500 }, { -1, 0x900, 0 },
};
ArkenoidCircle ArkSkyPosts[4] = {
    { 0x74D, 0x74D, 0x1D0 }, { 0x74D, -0x74D, 0x1D0 }, { -0x74D, 0x74D, 0x1D0 },
    { -0x74D, -0x74D, 0x1D0 },
};
ArkenoidCircle ArkSkyCircles[4] = { { 0, -0x1000, 0x1B3 }, { 0, 0x1000, 0x1B3 }, { 0x1000, 0, 0x1B3 }, { -0x1000, 0, 0x1B3 } };
ArkenoidCircle ArkSkyBumpers[8] = {
    { -0x800, -1, -0x800 }, { -1, -0x800, -1 }, { 0x800, 0, -0x800 }, { -1, -0x800, -1 },
    { -0x800, -1, -0x800 }, { -1, 0x800, 0 }, { 0x800, 0, -0x800 }, { -1, 0x800, 0 },
};
u16 BallSpeedFrames[4][2] = { { 0, 1 }, { 2, 3 }, { 1, 3 }, { 0, 2 } };
u16 BallSpeedTable[10][10] = {
    { 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x40, 0x40 },
    { 0x40, 0x40, 0xFFE0, 0, 0, 0, 0, 0xFFE0, 0x40, 0x40 },
    { 0xFFE0, 0xFFE0, 0xFFF0, 0, 0, 0, 0, 0xFFF0, 0xFFE0, 0xFFE0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x20, 0x20, 0x10, 0, 0, 0, 0, 0x10, 0x20, 0x20 },
    { 0x40, 0x40, 0x20, 0, 0, 0, 0, 0x20, 0x40, 0x40 },
    { 0xFFC0, 0xFFC0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFE0, 0xFFC0, 0xFFC0 },
};
s16 ArkUnusedShorts[30] = {
    0x12, 0x17, 0x12, 0x17, 0x12, 0x17, 0x12, 0x17, 0x5000, 0x5000, 0x5000, 0x4000, 0x2000,
    0x4000, 0x1800, 0x1000, 0x1800, 0x800, 0xA00, 0x800, 0x200, 0x200, 0x200, 0, 0, 0, 0, 0, 0,
    0,
};
u16 CrateMasterTimes[16] = { 0xA, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
ArkenoidRiseCfg FloorRiseCfg[8] = {
    { 2, -0x3800, 0x4000 }, { 4, -0x3800, 0x3800 }, { 0, -0x1000, 0x1000 },
    { 8, -0x3800, 0x3800 }, { 0xF, -0x2800, 0x2800 }, { 0, -0x1000, 0x1000 },
    { 0, -0x1000, 0x1000 }, { 0, -0x1000, 0x1000 },
};
u16 BallFramesA[4] = { 0x58D, 0x58E, 0x58F, 0x590 };
u16 ArkGrabFrames[4] = { 0x57C, 0x57D, 0x57E, 0x57F };
u16 BallFramesB[10] = { 0x587, 0x588, 0x589, 0x58A, 0x58B, 0x58C, 0x58D, 0x58E, 0x58F, 0x590 };
u16 CornerFailFrames[4] = { 0x5CA, 0x5CB, 0x5CC, 0x5CD };
u16 BallFramesC[4] = { 0x578, 0x579, 0x57A, 0x57B };
u16 ArkHeroSpeeds[10][5] = {
    { 0, 0, 0, 0, 0 }, { 0x18, 0xA, 0xF, 0x1C, 0x417F }, { 0x18, 0xA, 0xF, 0x1C, 0x41FF },
    { 0x18, 0xA, 0xF, 0x1C, 0x41FF }, { 0x18, 0xA, 0xF, 0x1C, 0x41FF },
    { 0x15, 7, 0xE, 0x1C, 0x407F }, { 0x15, 7, 0xE, 0x1C, 0x417F },
    { 0x15, 7, 0xE, 0x1C, 0x41FF }, { 0x18, 0xA, 0xF, 0x1C, 0x407F }, { 0, 0, 0, 0, 0 },
};
u16 FloorFramesA[8] = { 0x2010, 0x2012, 0x200F, 0x2011, 0x2016, 0x2014, 0x2015, 0x2013 };
u16 FloorFramesB[8] = { 0x200C, 0x200C, 0x200E, 0x200D, 0x201B, 0x2019, 0x201A, 0x2018 };
u16 FloorFramesC[8] = { 0x2022, 0x2020, 0x2021, 0x201F, 0x2026, 0x2024, 0x2025, 0x2023 };
s16 ArkPickupSpots[18] = { 0, 1, 2, 3, 1, 3, 0, 1, 0, 3, 2, 1, 2, 0, 2, 3, -1, 0 };
u16 FloorLayout[6][3] = {
    { 0x201E, 0x201F, 0x2020 }, { 0x2021, 0x2022, 0x2023 }, { 0x201E, 0x201F, 0x2020 },
    { 0x2021, 0x2022, 0x2023 }, { 0, 0x8011, 0x14 }, { 0x4000, 0x8001, 0 },
};
s32 NGinSteps[4] = { (s32)0x80110000, 0x4080003C, 0x88005, 0x8001 };
s32 NGinAnimList[4] = { (s32)&NGinSteps, (s32)&NGinSteps, 0, 0 };
ScriptInit NGinScriptInit = { 0, (void *)&NGinAnimList, 0x401 };
s32 SeaweedSteps0[7] = { 0x41000000, 0x41024101, 0x41044103, 0x41064105, 0x41084107, 0x458011, (s32)0x80004109 };
s32 SeaweedSteps1[7] = { 0x41800000, 0x41824181, 0x41844183, 0x41864185, 0x41884187, 0x458011, (s32)0x80004189 };
s32 SeaweedSteps2[7] = { 0x42800000, 0x42824281, 0x42844283, 0x42864285, 0x42884287, 0x458011, (s32)0x80004289 };
s32 SeaweedSteps3[3] = { (s32)0x80110000, 0x4200004F, 0x8000 };
s32 SeaweedSteps4[3] = { (s32)0x80110000, 0x4300004F, 0x8000 };
s32 SeaweedSteps5[3] = { (s32)0x80110000, 0x40800020, 0x8000 };
s32 SeaweedSteps6[3] = { (s32)0x80110000, 0x40000020, 0x8000 };
s32 SeaweedAnimList[7] = {
    (s32)&SeaweedSteps0, (s32)&SeaweedSteps1, (s32)&SeaweedSteps2, (s32)&SeaweedSteps3, (s32)&SeaweedSteps4,
    (s32)&SeaweedSteps5, (s32)&SeaweedSteps6,
};
ScriptInit SeaweedScriptInit = { 0, (void *)&SeaweedAnimList, 0x401 };
hero_seq CRA_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4180, 0x8000, 0,
};
hero_seq CRA_seq_arkidle[6] = {
    0, 0x8007, 0x6D, 0x4080, 0x8005, 0x19,
};
hero_seq CRA_seq_arktaunt[6] = {
    0, 0x8007, 0x34, 0x4200, 0x8004, 0x1C,
};
hero_seq CRA_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_arkwin[6] = {
    0, 0x8007, 0x6D, 0x4200, 0x8000, 0,
};
hero_seq CRA_seq_arkvictory[6] = {
    0, 0x8007, 0x44, 0x4100, 0x8000, 0,
};
hero_seq CRA_seq_arklose[10] = {
    0, 0x8007, 0x13, 0x4280, 0x8007, 0x14, 0x4293, 0x8000, 3, 0,
};
hero_seq COC_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_arkidle[6] = {
    0, 0x8007, 0x29, 0x4080, 0x8005, 0x19,
};
hero_seq COC_seq_arktaunt[6] = {
    0, 0x8007, 0x31, 0x4180, 0x8004, 0x1C,
};
hero_seq COC_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_arkwin[6] = {
    0, 0x8007, 0x31, 0x4180, 0x8000, 0,
};
hero_seq COC_seq_arkvictory[6] = {
    0, 0x8007, 0x40, 0x4100, 0x8000, 0,
};
hero_seq COC_seq_arklose[10] = {
    0, 0x8007, 0x1E, 0x4280, 0x8007, 0x14, 0x429E, 0x8000, 3, 0,
};
hero_seq TIN_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4080, 0x8000, 0,
};
hero_seq TIN_seq_arkidle[6] = {
    0, 0x8007, 0x2E, 0x4200, 0x8005, 0x19,
};
hero_seq TIN_seq_arktaunt[6] = {
    0, 0x8007, 0x2A, 0x4180, 0x8004, 0x1C,
};
hero_seq TIN_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_arkwin[6] = {
    0, 0x8007, 0x2A, 0x4200, 0x8000, 0,
};
hero_seq TIN_seq_arkvictory[10] = {
    0, 0x8007, 0xE, 0x4100, 0x8007, 0x1F, 0x410E, 0x8000, 3, 0,
};
hero_seq TIN_seq_arklose[10] = {
    0, 0x8007, 0x1E, 0x4280, 0x8007, 0x13, 0x429E, 0x8000, 3, 0,
};
hero_seq CTX_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq CTX_seq_arkidle[6] = {
    0, 0x8007, 0x3C, 0x4080, 0x8005, 0x19,
};
hero_seq CTX_seq_arktaunt[6] = {
    0, 0x8007, 0x34, 0x4180, 0x8004, 0x1C,
};
hero_seq CTX_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_arkwin[6] = {
    0, 0x8007, 0x34, 0x4180, 0x8000, 0,
};
hero_seq CTX_seq_arkvictory[6] = {
    0, 0x8007, 0x51, 0x4100, 0x8000, 0,
};
hero_seq CTX_seq_arklose[10] = {
    0, 0x8007, 0x23, 0x4280, 0x8018, 0x63, 0x42A2, 0x8000, 3, 0,
};
hero_seq BRO_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_arkidle[6] = {
    0, 0x8007, 0x38, 0x4080, 0x8005, 0x19,
};
hero_seq BRO_seq_arktaunt[6] = {
    0, 0x8007, 0x32, 0x4180, 0x8004, 0x1C,
};
hero_seq BRO_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_arkwin[6] = {
    0, 0x8007, 0x3C, 0x4180, 0x8000, 0,
};
hero_seq BRO_seq_arkvictory[6] = {
    0, 0x8007, 0x4B, 0x4100, 0x8000, 0,
};
hero_seq BRO_seq_arklose[10] = {
    0, 0x8007, 0x23, 0x4280, 0x8018, 0x63, 0x42A2, 0x8000, 3, 0,
};
hero_seq DNG_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4200, 0x8000, 0,
};
hero_seq DNG_seq_arkidle[6] = {
    0, 0x8007, 0x1F, 0x4080, 0x8005, 0x19,
};
hero_seq DNG_seq_arktaunt[6] = {
    0, 0x8007, 0x3D, 0x4180, 0x8004, 0x1C,
};
hero_seq DNG_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_arkwin[6] = {
    0, 0x8007, 0x3D, 0x4180, 0x8000, 0,
};
hero_seq DNG_seq_arkvictory[6] = {
    0, 0x8007, 0x1E, 0x4100, 0x8000, 0,
};
hero_seq DNG_seq_arklose[10] = {
    0, 0x8007, 0xC, 0x4280, 0x8007, 0xE, 0x428B, 0x8000, 3, 0,
};
hero_seq KON_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4180, 0x8000, 0,
};
hero_seq KON_seq_arkidle[6] = {
    0, 0x8007, 0x3C, 0x4100, 0x8005, 0x19,
};
hero_seq KON_seq_arktaunt[6] = {
    0, 0x8007, 0x2A, 0x4200, 0x8004, 0x1C,
};
hero_seq KON_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_arkwin[6] = {
    0, 0x8007, 0x2A, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_arkvictory[6] = {
    0, 0x8007, 0x44, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_arklose[10] = {
    0, 0x8007, 0xC, 0x4280, 0x8007, 0x26, 0x428C, 0x8000, 3, 0,
};
hero_seq KAN_seq_arkbreathe[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_arkmove[6] = {
    0, 0x8007, 0x1C, 0x4080, 0x8000, 0,
};
hero_seq KAN_seq_arkidle[6] = {
    0, 0x8007, 0x4B, 0x4180, 0x8005, 0x19,
};
hero_seq KAN_seq_arktaunt[6] = {
    0, 0x8007, 0x28, 0x4200, 0x8004, 0x1C,
};
hero_seq KAN_seq_arkkick[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_arkwin[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_arkvictory[6] = {
    0, 0x8007, 0x40, 0x4280, 0x8000, 0,
};
hero_seq KAN_seq_arklose[10] = {
    0, 0x8007, 0x23, 0x4100, 0x8018, 0xA, 0x4122, 0x8000, 3, 0,
};
s32 ArkHeroStarts[4][3] = { { 0, 0x1C, -0x880 }, { 0, 0x1C, 0x880 }, { 0x880, 0x1C, 0 }, { -0x880, 0x1C, 0 } };
s32 ArkUnused2[2] = { 0x20001, 0x80004 };
u16 ArkKickFrames[4] = { 0x581, 0x582, 0x583, 0x584 };
u16 ArkGrabTimes[12] = { 0x5AB, 0x5AC, 0x5AD, 0x5AE, 0x5AF, 0x5B0, 0x5B1, 0x5B2, 0x5B3, 0x5B4, 0x5B5, 0x5B6 };
ArkenoidEdge ArkBotEdgesA[4] = {
    { -0x800, -1, -0x7A0, -1, 0x800, 0, -0x7A0, -1 },
    { -0x800, -1, 0x7A0, 0, 0x800, 0, 0x7A0, 0 }, { 0x7A0, 0, -0x800, -1, 0x7A0, 0, 0x800, 0 },
    { -0x7A0, -1, -0x800, -1, -0x7A0, -1, 0x800, 0 },
};
ArkenoidEdge ArkBotEdgesB[4] = {
    { -0x500, -1, -0x800, -1, 0x500, 0, -0x800, -1 },
    { -0x500, -1, 0x800, 0, 0x500, 0, 0x800, 0 }, { 0x800, 0, -0x500, -1, 0x800, 0, 0x500, 0 },
    { -0x800, -1, -0x500, -1, -0x800, -1, 0x500, 0 },
};
u16 ArkBotAimAngles[32] = {
    0x100, 0x140, 0x180, 0x1C0, 0x280, 0x240, 0x280, 0x2C0, 0x300, 0x340, 0x380, 0x3C0, 0x480,
    0x440, 0x480, 0x4C0, 0x500, 0x540, 0x580, 0x5C0, 0x680, 0x640, 0x680, 0x6C0, 0x700, 0x740,
    0x780, 0x7C0, 0x880, 0x840, 0x880, 0x8C0,
};
s32 *ArkWallTargets[4][3] = {
    { (void *)&ArkSidePlayer1, (void *)&ArkSidePlayer0, (void *)&ArkSidePlayer2 },
    { (void *)&ArkSidePlayer2, (void *)&ArkSidePlayer3, (void *)&ArkSidePlayer1 },
    { (void *)&ArkSidePlayer3, (void *)&ArkSidePlayer1, (void *)&ArkSidePlayer0 },
    { (void *)&ArkSidePlayer0, (void *)&ArkSidePlayer2, (void *)&ArkSidePlayer3 },
};
s16 ArkBotWaits[6] = { -0x40, 0, 0x40, 0, 0x20, 0 };
u16 CrateMasterSpotsX[18] = { 1, 2, 3, 2, 3, 1, 3, 1, 2, 1, 2, 3, 2, 3, 1, 3, 1, 2 };
u16 CrateMasterSpotsZ[18] = { 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3 };
ArkenoidCircle CameraFlashCircles[9] = {
    { 0x5000, 0x5000, 0x5000 }, { 0x4000, 0x2000, 0x4000 }, { 0x1800, 0x1000, 0x1800 },
    { 0x800, 0xA00, 0x800 }, { 0x200, 0x200, 0x200 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
    { 0, 0, 0 },
};
ArkenoidCircle *ArkPosts = 0;
ArkenoidCircle *ArkCircles = 0;
ArkenoidCircle *ArkBumpers = 0;
ArkenoidHeroBox ArkHeroBoxes[4] = {0};
EXTengine *ArkBallSpawner = 0;
ArkenoidGin *ArkNGin = 0;
EXTengine ArkCornerEngine = {0};
EXTengine ArkDieEngine = {0};
s32 ArkFloorTiltX = 0;
s32 ArkFloorTiltZ = 0;
s32 CrateMasterState = 0;
s32 CrateMasterTimer = 0;
s32 CrateMasterCount = 0;
s32 CrateMasterNext = 0;
s32 ArkBallTimer = 0;
s32 ArkDeadWalls = 0;
u16 ArkFloorState = 0;
void *ArkBounceOnBounds = 0;
void *ArkTestOutOfBounds = 0;
void *ArkTestLaunchBounds = 0;
ArkenoidScnObj *FloorObjectsA[10] = {0};
ArkenoidScnObj *FloorObjectsB[10] = {0};
ArkenoidLink FloorLinks[4] = {0};
ArkenoidDoor ArkDoorsA[4] = {0};
ArkenoidDoor ArkDoorsB[4] = {0};
ArkenoidWallObj ArkWallObjects[4] = {0};
ArkenoidSlot ArkGoals[4] = {0};
ArkenoidWall ArkWalls[4] = {0};
u16 ArkHeroSpeed[4] = {0};
s16 ArkHeroSpeedDelta[4] = {0};
u16 CrateMasterWordA = 0;
u16 CrateMasterWordB = 0;
u16 NGinState = 0;
u16 ArkPickupBounce = 0;
s16 ArkBotSkill = 0;
s16 ArkInitWord0 = 0;
s16 ArkInitWord1 = 0;
s16 ArkInitWord2 = 0;
s16 ArkInitWord3 = 0;
u16 CrateMasterWordC = 0;
u16 ArkRedKickTimer = 0;
s16 ArkUnused3[2] = {0};
u16 ArkWallFlags = 0;
s16 ArkBallCount = 0;
s16 ArkUnusedBuffer[1024] = {0};
u16 ArkPickupCount = 0;
s32 ArkSidePlayer1 = 0;
s32 ArkSidePlayer2 = 0;
s32 ArkSidePlayer3 = 0;
s32 ArkSidePlayer0 = 0;

void ArkHeroEngineUpdate(s32 arg0) {
    VECTOR sp10;
    VECTOR *v;
    HeroInfo *h;
    EXTengine *e = (&Heroes[arg0])->hero_eng;
    FlagRecord *r;
    ArkenoidHeroBox *b;
    ArkenoidHeroBox *base;
    s32 t;

    if (e->unk5E != 0) {
        func_800142D4(&e->unk1C);
    }
    func_8001CD04(e->sub, e->unk3C, &e->unk10);
    r = func_80025A48(e, 1);
    if (r == NULL) goto next;
    {
        base = ArkHeroBoxes;
        b = (ArkenoidHeroBox *)(arg0 << 2);
        b = (ArkenoidHeroBox *)((s32)b + arg0);
        b = (ArkenoidHeroBox *)((s32)b << 3);
        b = (ArkenoidHeroBox *)((s32)base + (s32)b);
        t = r->unk2;
        b->a = t;
        b->b = r->height;
        b->c = r->radius;
        func_80015758((SVECTOR *)r, &sp10);
        b->x0 = sp10.vx - (&Heroes[arg0])->hero_eng->unk10.x;
        b->y0 = sp10.vy - (&Heroes[arg0])->hero_eng->unk10.y;
        b->z0 = sp10.vz - (&Heroes[arg0])->hero_eng->unk10.z;
    }
next:
    r = func_80025A48(e, 2);
    v = &sp10;
    if (r != NULL) {
        base = ArkHeroBoxes;
        b = (ArkenoidHeroBox *)(arg0 << 2);
        b = (ArkenoidHeroBox *)((s32)b + arg0);
        b = (ArkenoidHeroBox *)((s32)b << 3);
        t = r->radius;
        b = (ArkenoidHeroBox *)((s32)base + (s32)b);
        b->d = t;
        func_80015758((SVECTOR *)r, v);
        b->x1 = sp10.vx - (&Heroes[arg0])->hero_eng->unk10.x;
        b->y1 = sp10.vy - (&Heroes[arg0])->hero_eng->unk10.y;
        b->z1 = sp10.vz - (&Heroes[arg0])->hero_eng->unk10.z;
    }
}

void AR_PreInitLevel(void) {
    u8 *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void *t;

    HeroHooks.unk4 = ArkHeroModels;
    HeroHooks.unk0 = (s32)AR_InitHero;
    HeroHooks.unk18 = AR_InitShadow;
    HeroHooks.unk1C = AR_UpdateShadow;
    HeroHooks.unk38 = Ark_HeroBot;
    HeroHooks.unk14 = ArkVictorySeqs;
    if (GameTeams[0] != 0) {
        ArkSidePlayer1 = 1;
        ArkSidePlayer2 = 3;
        ArkSidePlayer3 = 2;
    } else {
        ArkSidePlayer1 = 2;
        ArkSidePlayer2 = 3;
        ArkSidePlayer3 = 1;
    }
    ArkSidePlayer0 = 0;
    g = GameInfo;
    switch (*(s16 *)(g + 0x34)) {
    case 15:
        HeroHooks.unk68 = AR_KeepInBounds;
        HeroHooks.unk20 = AR_PreMoveHero;
        HeroHooks.unk24 = AR_MoveHero;
        ArkBounceOnBounds = AR_BA_BounceOnBounds;
        ArkTestOutOfBounds = AR_BA_TestOutOfBounds;
        ArkTestLaunchBounds = AR_BA_TestLaunchBounds;
        LevelSpecs.GetContour = (void *)GetArkContour;
        AiBotTweakTable = ArkBeachAiBotTweak;
        AiHumanTweakTable = ArkBeachAiHumanTweak;
        AiHumanRoundTweakTable = ArkBeachAiHumanRoundTweak;
        p = &AiSkillTable;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = ArkBeachAiSkillAlt;
        } else {
            t = ArkBeachAiSkill;
        }
        *p = t;
        AiTeamSkillTable = ArkBeachAiTeamSkill;
        return;
    case 16:
        HeroHooks.unk68 = AR_KeepInBounds;
        HeroHooks.unk20 = AR_PreMoveHero;
        HeroHooks.unk24 = AR_MoveHero;
        ArkBounceOnBounds = AR_BA_BounceOnBounds;
        ArkTestOutOfBounds = AR_NG_TestOutOfBounds;
        ArkTestLaunchBounds = AR_BA_TestLaunchBounds;
        LevelSpecs.GetContour = (void *)GetArkContour;
        AiBotTweakTable = ArkNginAiBotTweak;
        AiHumanTweakTable = ArkNginAiHumanTweak;
        AiHumanRoundTweakTable = ArkNginAiHumanRoundTweak;
        p2 = &AiSkillTable;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = ArkNginAiSkillAlt;
        } else {
            t = ArkNginAiSkill;
        }
        *p2 = t;
        AiTeamSkillTable = ArkNginAiTeamSkill;
        return;
    case 17:
        HeroHooks.unk68 = AR_PI_KeepInBounds;
        HeroHooks.unk20 = AR_PI_PreMoveHero;
        HeroHooks.unk24 = AR_PI_MoveHero;
        ArkBounceOnBounds = AR_PI_BounceOnBounds;
        ArkTestOutOfBounds = AR_PI_TestOutOfBounds;
        ArkTestLaunchBounds = AR_PI_TestLaunchBounds;
        LevelSpecs.GetContour = (void *)GetArkPIContour;
        AiBotTweakTable = ArkSkyAiBotTweak;
        AiHumanTweakTable = ArkSkyAiHumanTweak;
        AiHumanRoundTweakTable = ArkSkyAiHumanRoundTweak;
        p3 = &AiSkillTable;
        if (*(s32 *)(g + 0x6C) != 0) {
            t = ArkSkyAiSkillAlt;
        } else {
            t = ArkSkyAiSkill;
        }
        *p3 = t;
        AiTeamSkillTable = ArkSkyAiTeamSkill;
        return;
    case 0:
    default:
        HeroHooks.unk68 = AR_KeepInBounds;
        HeroHooks.unk20 = AR_PreMoveHero;
        HeroHooks.unk24 = AR_MoveHero;
        ArkBounceOnBounds = AR_BA_BounceOnBounds;
        ArkTestOutOfBounds = AR_BA_TestOutOfBounds;
        ArkTestLaunchBounds = AR_BA_TestLaunchBounds;
        LevelSpecs.GetContour = (void *)GetArkContour;
        AiBotTweakTable = ArkBallsAiBotTweak;
        AiHumanTweakTable = ArkBallsAiHumanTweak;
        AiHumanRoundTweakTable = ArkBallsAiHumanRoundTweak;
        p4 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = ArkBallsAiSkillAlt;
        } else {
            t = ArkBallsAiSkill;
        }
        *p4 = t;
        AiTeamSkillTable = ArkBallsAiTeamSkill;
        return;
    }
}

void AR_InitLevel(void) {
    ArkenoidModeView *m;
    ArkenoidModeView *m2;
    HeroInfo *h;
    EXTengine *e;
    EXTheromove *mv;
    s32 i;
    s32 a;

    World_floor_level = 0;
    HeadOfBalls = 0;
    D_8005AA3C = 0;
    ArkInitWord0 = 0;
    ArkInitWord1 = 0;
    ArkInitWord2 = 0;
    ArkInitWord3 = 0;
    CrateMasterWordC = 0xB4;
    ArkRedKickTimer = 0xB4;
    ArkWallFlags = 0;
    ArkBallCount = -1;
    ArkBallSpawner = 0;
    ArkNGin = 0;
    ArkDeadWalls = 0;
    ArkPickupCount = Random(15);
    CrateMasterState = 150;
    CrateMasterTimer = Random(18);
    CrateMasterCount = 270;
    CrateMasterNext = Random(18);
    ArkBallTimer = -1;
    CrateMasterWordA = 0;
    CrateMasterWordB = 0;
    NGinState = 10;
    ArkBotSkill = 0;
    ArkCornerEngine = EngineTemplate;
    ArkDieEngine = EngineTemplate;
    ArkFloorState = 30;
    ArkPickupBounce = 0;

    for (i = 0, m = (ArkenoidModeView *)GameInfo; i < 4; i++) {
        h = &Heroes[i];
        a = get_angle(h->hero_eng->unk10.x, h->hero_eng->unk10.z) + 0x7F;
        mv = h->hero_move;
        e = h->hero_eng;
        a &= 0xC00;
        e->unk1E = a;
        mv->facing_angle = a;
        mv->forward_angle = a;
        func_80026980(h->hero);
        switch (m->unk34) {
        case 15:
            ArkPosts = ArkBeachPosts;
            ArkCircles = ArkBeachCircles;
            h->hero_score = 12;
            h->hero_health = 12;
            ArkBumpers = ArkBeachBumpers;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 18;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 30;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 9;
                    Heroes[1].hero_score = 15;
                    Heroes[2].hero_score = 15;
                    Heroes[3].hero_score = 15;
                }
            }
            break;
        case 16:
            ArkPosts = ArkNginPosts;
            ArkCircles = ArkNginCircles;
            h->hero_score = 20;
            h->hero_health = 20;
            ArkBumpers = ArkNginBumpers;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 30;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 40;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 15;
                    Heroes[1].hero_score = 20;
                    Heroes[2].hero_score = 20;
                    Heroes[3].hero_score = 20;
                }
            }
            break;
        case 17:
            ArkPosts = ArkSkyPosts;
            ArkCircles = ArkSkyCircles;
            ArkBumpers = ArkSkyBumpers;
            if (m->unk32 & 0x80) {
                h->hero_score = 10;
                h->hero_health = 10;
            } else {
                h->hero_score = 20;
                h->hero_health = 20;
            }
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = 18;
                    Heroes[1].hero_score = 0;
                    Heroes[2].hero_score = 25;
                    Heroes[3].hero_score = 0;
                } else {
                    Heroes[0].hero_score = 9;
                    Heroes[1].hero_score = 15;
                    Heroes[2].hero_score = 15;
                    Heroes[3].hero_score = 15;
                }
            }
            break;
        case 0:
        default:
            ArkPosts = ArkBallsPosts;
            ArkCircles = ArkBallsCircles;
            h->hero_score = 15;
            h->hero_health = 15;
            ArkBumpers = ArkBallsBumpers;
            if (m->unk32 & 0x40) {
                if (m->unk6C != 0) {
                    Heroes[0].hero_score = Heroes[0].hero_score_displayed = 18;
                    Heroes[1].hero_score = Heroes[1].hero_score_displayed = 0;
                    Heroes[2].hero_score = Heroes[2].hero_score_displayed = 30;
                    Heroes[3].hero_score = Heroes[3].hero_score_displayed = 0;
                } else {
                    Heroes[0].hero_score = Heroes[0].hero_score_displayed = 9;
                    Heroes[1].hero_score = Heroes[1].hero_score_displayed = 15;
                    Heroes[2].hero_score = Heroes[2].hero_score_displayed = 15;
                    Heroes[3].hero_score = Heroes[3].hero_score_displayed = 15;
                }
            }
            break;
        }
        Heroes[0].hero_score_displayed = Heroes[0].hero_score;
        Heroes[1].hero_score_displayed = Heroes[1].hero_score;
        Heroes[2].hero_score_displayed = Heroes[2].hero_score;
        Heroes[3].hero_score_displayed = Heroes[3].hero_score;
        ArkHeroEngineUpdate(i);
    }
    m2 = (ArkenoidModeView *)GameInfo;
    if ((m2->unk32 & 0x40) && m2->unk6C != 0) {
        InitSharedScoreIcons();
    }
    switch (GameMode) {
    case 0x10:
        CreateNAS(0, World_floor_level, 0, 0x1507, 0x11);
        break;
    case 0x11:
        PlaySfx(0x5C8, &ArkCornerEngine.sfx, 0, 0x1000, 0x1E00, 0);
        break;
    }
    InitARKfloor();
}

void AR_UpdateLevel(void) {
    s32 i;
    s32 best;
    s32 k;
    u8 *ao;
    u16 *c2;
    NAS *n;
    ArkenoidFuxType *f;
    u16 *cnt;

    func_80086F78();
    if ((GameExitFlags & 0x801) && ArkFloorState != 0) {
        ArkFloorState = ArkFloorState - 1;
    }
    i = 12;
    if (HeroesAllOut != 0) {
        if (ArkBallTimer == -1) {
            best = 99;
            for (i = 0; i < 4; i++) {
                AiPlayerSkill[i].unk4 = 4;
                AiPlayerSkill[i].unk0 = 32;
                if (!(Heroes[i].hero_status & 0xFA00) && Heroes[i].hero_score < best) {
                    best = Heroes[i].hero_score;
                    ArkBallTimer = i;
                }
            }
            if (ArkBallTimer != -1 && ArkBallTimer < 4) {
                AiPlayerSkill[ArkBallTimer].unk4 = 32;
                AiPlayerSkill[ArkBallTimer].unk0 = 0;
            }
            i = 12;
        }
    }
    {
        u16 *p = (u16 *)&D_8009E25C;
        p += 12;
        do {
            *p = 0;
            i--;
            p--;
        } while (i >= 0);
    }
    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        f = (ArkenoidFuxType *)n->FUX;
        if (f != NULL && (u16)(f->type - 0x11) < 0xC) {
            D_8009E25C.n++;
            k = f->type - 0x10;
            cnt = (u16 *)&D_8009E25C;
            cnt[k]++;
            if ((u16)(f->type - 0x12) < 4) {
                c2 = D_8009E46C;
                c2[1]++;
            }
        }
    }
    ArkCrateMaster();
    UpdateArkFloor();
    if (GameMode != 0x10) {
        if (GameMode >= 0x11) {
            if (GameMode == 0x11) {
                HandleCornerFailure();
            }
        }
    }
    ao = (u8 *)BallsEffectsModel;
    ao += 0x10;
    func_80029100((obj_textureset *)ao);
}

bool AR_KeepInBounds(EXTengine *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 a;

    for (i = 0; i < 4; i++) {
        dx = arg0->unk10.x - ArkPosts[i].x;
        dz = arg0->unk10.z - ArkPosts[i].z;
        if (arg0->collision_box.flags & 0x40) {
            r = ArkPosts[i].r + arg0->collision_box.radius;
        } else {
            r = ArkPosts[i].r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            a = get_angle(-dx, -dz);
            if ((void *)arg0 != (void *)Heroes[ArkSidePlayer2].hero_eng &&
                (void *)arg0 != (void *)Heroes[ArkSidePlayer1].hero_eng) {
                arg0->unk10.x = ArkPosts[i].x - (r * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12);
            }
            if ((void *)arg0 != (void *)Heroes[ArkSidePlayer3].hero_eng &&
                (void *)arg0 != (void *)Heroes[ArkSidePlayer0].hero_eng) {
                arg0->unk10.z = ArkPosts[i].z - (r * rcossin_tbl[a & 0xFFF].unk2 >> 12);
            }
            return 1;
        }
    }
    return 0;
}

bool AR_PI_KeepInBounds(EXTengine *arg0) {
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 a;

    if (GameExitFlags & 0x801) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if ((Heroes[i].hero_status & 0xFA00) && (void *)Heroes[i].hero_eng == (void *)arg0) {
            return 0;
        }
    }
    a = arg0->unk1E;
    arg0->unk10.x = rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
    arg0->unk10.z = rcossin_tbl[a & 0xFFF].unk2 * 9 >> 4;
    for (i = 0; i < 4; i++) {
        dx = arg0->unk10.x - ArkPosts[i].x;
        dz = arg0->unk10.z - ArkPosts[i].z;
        if (arg0->collision_box.flags & 0x40) {
            r = ArkPosts[i].r + arg0->collision_box.radius;
        } else {
            r = ArkPosts[i].r + 0x80;
        }
        if (dx * dx + dz * dz < r * r) {
            a = get_angle(-dx, -dz);
            if ((void *)arg0 != (void *)Heroes[ArkSidePlayer2].hero_eng &&
                (void *)arg0 != (void *)Heroes[ArkSidePlayer1].hero_eng) {
                arg0->unk10.x = ArkPosts[i].x - (r * rcossin_tbl[(a - 0x400) & 0xFFF].unk2 >> 12);
            }
            if ((void *)arg0 != (void *)Heroes[ArkSidePlayer3].hero_eng &&
                (void *)arg0 != (void *)Heroes[ArkSidePlayer0].hero_eng) {
                arg0->unk10.z = ArkPosts[i].z - (r * rcossin_tbl[a & 0xFFF].unk2 >> 12);
            }
            a = get_angle(arg0->unk10.x, arg0->unk10.z);
            arg0->unk1E = a;
            arg0->unk10.x = rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 9 >> 4;
            arg0->unk10.z = rcossin_tbl[a & 0xFFF].unk2 * 9 >> 4;
            return 1;
        }
    }
    return 0;
}

s32 AR_BA_BounceOnBounds(EXTengine *arg0, s32 arg1) {
    ArkenoidCircle *c;
    ArkenoidCircle *base;
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

    for (i = 0; i < 4; i++) {
        if (arg0->collision_box.flags & 0x40) {
            r = ArkPosts[i].r + arg0->collision_box.radius;
        } else {
            r = ArkPosts[i].r + 0x80;
        }
        base = ArkPosts;
        c = &base[i];
        cx = c->x;
        x = arg0->unk10.x;
        dx = x - cx;
        cz = c->z;
        z = arg0->unk10.z;
        dz = z - cz;
        if (dx * dx + dz * dz < r * r) {
            goto found;
        }
    }
    ret = -1;
    if (ArkWallFlags == 0) {
        goto fail;
    }
    dx = x;
    dz = z;
    if (arg0->collision_box.flags & 0x40) {
        r = 0x800 - arg0->collision_box.radius;
    } else {
        r = 0x800;
    }
    if (dx > r && (ArkWallFlags & 0x88)) {
        arg0->unk10.x = r;
        ret = 0x400;
    }
    if (dx < -r && (ArkWallFlags & 0x44)) {
        arg0->unk10.x = -r;
        ret = 0xC00;
    }
    if (dz > r && (ArkWallFlags & 0x22)) {
        arg0->unk10.z = r;
        ret = 0;
    }
    if (dz < -r && (ArkWallFlags & 0x11)) {
        arg0->unk10.z = -r;
        ret = 0x800;
    }
    if (ret != -1) {
        PlaySfx(0x592, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    return ret;
found:
    v.x = cx;
    v.y = 0;
    v.z = base[i].z;
    LineToCircleTest(&arg0->unk10, arg1, &v, r);
    ret = get_angle(ArkPosts[i].x - arg0->unk10.x, ArkPosts[i].z - arg0->unk10.z);
    ArkGoals[i].state = 3;
    return ret;
fail:
    return -1;
}

s32 AR_PI_BounceOnBounds(EXTengine *arg0, s32 arg1) {
    SLONG_XYZ v;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 cx;
    s32 ret;
    s32 a;
    s32 t;

    a = ret = -1;
    for (i = 0; i < 4; i++) {
        if (arg0->collision_box.flags & 0x40) {
            r = ArkPosts[i].r + arg0->collision_box.radius;
        } else {
            r = ArkPosts[i].r + 0x80;
        }
        cx = ArkPosts[i].x;
        dx = arg0->unk10.x - cx;
        dz = arg0->unk10.z - ArkPosts[i].z;
        if (dx * dx + dz * dz < r * r) {
            v.x = cx;
            v.y = 0;
            v.z = ArkPosts[i].z;
            LineToCircleTest(&arg0->unk10, arg1, &v, r);
            a = get_angle(ArkPosts[i].x - arg0->unk10.x, ArkPosts[i].z - arg0->unk10.z);
            ArkGoals[i].state = 3;
            ret = a;
        }
        if (ArkWallFlags != 0) {
            dx = arg0->unk10.x;
            dz = arg0->unk10.z;
            if (arg0->collision_box.flags & 0x40) {
                r = 0x800 - arg0->collision_box.radius;
            } else {
                r = 0x800;
            }
            if (dx > r && (ArkWallFlags & 0x88)) {
                arg0->unk10.x = r;
                a = 0x400;
            }
            if (dx < -r && (ArkWallFlags & 0x44)) {
                arg0->unk10.x = -r;
                a = 0xC00;
            }
            if (dz > r && (ArkWallFlags & 0x22)) {
                arg0->unk10.z = r;
                a = 0;
            }
            if (dz < -r && (ArkWallFlags & 0x11)) {
                arg0->unk10.z = -r;
                a = 0x800;
            }
            if (a != -1) {
                if (ArkPickupBounce != 0) {
                    t = ArkPickupBounce - 1;
                } else {
                    PlaySfx(0x592, NULL, NULL, 0x1000, 0x1E00, 0);
                    t = 3;
                }
                ArkPickupBounce = t;
                ret = a;
            }
        }
    }
    return ret;
}

bool AR_PI_TestOutOfBounds(EXTengine *arg0, EXTobject *arg1, bool arg2) {
    s32 lim = 0x980;
    s32 x, z;
    s32 a;

    if (arg2 != 0) {
        lim = 0xC00;
    }
    x = arg0->unk10.x;
    z = arg0->unk10.z;
    if (x * x + z * z > lim * lim) {
        a = get_angle(-x, -z) + 0x200;
        a &= 0xC00;
        if (a == 0x400) {
            arg1->player = ArkSidePlayer2;
            return 1;
        } else if (a == 0x800) {
            arg1->player = ArkSidePlayer3;
            return 1;
        } else if (a == 0xC00) {
            arg1->player = ArkSidePlayer1;
            return 1;
        } else {
            arg1->player = ArkSidePlayer0;
            return 1;
        }
    }
    if (arg2 != 0) {
        s16 d = arg1->player;
        if (d == ArkSidePlayer3 || d == ArkSidePlayer0) {
            if (x >= 0x601) {
                arg0->unk10.x = 0x600;
            }
            if (arg0->unk10.x < -0x600) {
                arg0->unk10.x = -0x600;
            }
        } else {
            if (z >= 0x601) {
                arg0->unk10.z = 0x600;
            }
            if (arg0->unk10.z < -0x600) {
                arg0->unk10.z = -0x600;
            }
        }
    }
    return 0;
}

bool AR_BA_TestLaunchBounds(EXTengine *arg0, EXTobject *obj) {
    ArkenoidCircle *base = ArkPosts;
    ArkenoidCircle *c;
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
        x = arg0->unk10.x;
        dx = x - dx;
        z = arg0->unk10.z;
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

bool GetIntersect(SLONG_XYZ *p0, SLONG_XYZ *p1, SLONG_XYZ *q) {
    s32 denom, num, u, t;
    s32 dqz;
    s32 dpx;
    s32 dpz, dqx, ez, ex;
    s32 a, b, c;

    a = p0->x;
    if (a < 0 && (c = p1->x) < 0 && a < (b = q->x) && c < b) {
        return 0;
    }
    if ((a = p0->x) > 0 && (c = p1->x) > 0 && (b = q->x) < a && b < c) {
        return 0;
    }
    a = p0->z;
    if (a < 0 && (c = p1->z) < 0 && a < (b = q->z) && c < b) {
        return 0;
    }
    if ((a = p0->z) > 0 && (c = p1->z) > 0 && (b = q->z) < a && b < c) {
        return 0;
    }
    dqz = -q->z >> 4;
    dpx = p0->x >> 4;
    dqx = -q->x >> 4;
    dpz = p0->z >> 4;
    ez = (p1->z - p0->z) >> 4;
    ex = (p1->x - p0->x) >> 4;
    num = dqz * dpx - dqx * dpz;
    denom = ez * dqx - ex * dqz;
    if (denom > 0) {
        if (denom < num) {
            return 0;
        }
        if (num < 0) {
            return 0;
        }
    } else {
        if (num < denom) {
            return 0;
        }
        if (num > 0) {
            return 0;
        }
    }
    u = ex * dpz - ez * dpx;
    if (denom > 0) {
        if (denom < u) {
            return 0;
        }
        if (u < 0) {
            return 0;
        }
    } else {
        if (u < denom) {
            return 0;
        }
        if (u > 0) {
            return 0;
        }
    }
    if (denom == 0) {
        return 0;
    }
    t = (num << 8) / denom;
    q->x = p0->x + (((p1->x - p0->x) * t) >> 8);
    q->z = p0->z + (((p1->z - p0->z) * t) >> 8);
    return 1;
}

void AR_InitHero(u32 arg0) {
    HeroInfo *h;
    HeroInfo *c;
    HeroInfo *g;
    HeroInfo *b;
    EXTengine *e;
    subengine *s;
    subengine *n;
    s32 sp;
    s32 y;
    s32 d;
    s32 a;

    c = Heroes;
    h = c + arg0;
    e = h->hero_eng;
    if (arg0 == ArkSidePlayer0) {
        h->hero_eng->unk10.x = ArkHeroStarts[0][0];
        h->hero_eng->unk10.z = ArkHeroStarts[0][2];
    }
    if (arg0 == ArkSidePlayer3) {
        h->hero_eng->unk10.x = ArkHeroStarts[1][0];
        h->hero_eng->unk10.z = ArkHeroStarts[1][2];
    }
    if (arg0 == ArkSidePlayer1) {
        h->hero_eng->unk10.x = ArkHeroStarts[2][0];
        h->hero_eng->unk10.z = ArkHeroStarts[2][2];
    }
    if (arg0 == ArkSidePlayer2) {
        h->hero_eng->unk10.x = ArkHeroStarts[3][0];
        h->hero_eng->unk10.z = ArkHeroStarts[3][2];
    }
    e->unk5D = 0;
    s = h->hero_sub;
    func_8001D390(e, 2);
    n = s->next;
    if (n != NULL) {
        n->unk6C = s->unk6C;
        n->flags |= 0x8000;
        h->hero_anim->sub = n;
        n = n->next;
        if (n != NULL) {
            n->unk6C = BallsVehicleModel;
            n->unk74 = 0x2001;
            n->unk50 = 1;
            n->unk51 = 0;
            n->flags &= ~0x8000;
        }
    }
    s->unk6C = BallsVehicleModel;
    if (arg0 == ArkSidePlayer0) {
        switch (Heroes[arg0].hero_name) {
        case 5:
        case 6:
        case 8:
            s->unk68 = -0x10;
            s->flags |= 0x02000000;
            break;
        }
    }
    ArkHeroSpeed[arg0] = 0xF;
    s->unk74 = 0x2004;
    FetchDefaultColBox(e, 0x50);
    if (e->unk10.x > 0x880) {
        e->unk10.x = 0x880;
    }
    if (e->unk10.x < -0x880) {
        e->unk10.x = -0x880;
    }
    if (e->unk10.z > 0x880) {
        e->unk10.z = 0x880;
    }
    if (e->unk10.z < -0x880) {
        e->unk10.z = -0x880;
    }
    SetHeroAction(arg0, 0x1C);
    b = Heroes;
    g = b + arg0;
    a = GameFrameCount;
    g->hero_weapon_count = 1;
    sp = ((((((a + arg0) << 6) & 0xFFF) - 0x400) & 0xFFF) + rcossin_tbl)->unk2;
    y = World_floor_level;
    d = (sp * 15 >> 11) - 0x80;
    g->hero_eng->unk10.y = y + d;
}

void AR_PreMoveHero(u32 arg0) {
    EXTheromove *m;
    HeroInfo *h;
    HeroInfo *c;
    PadButtons *p;
    PadButtons *pads;
    s32 mask;
    s32 d;
    s32 max;
    s32 acc;
    s32 fast;
    s32 b;
    s32 v;

    max = 0x68;
    acc = 0x22;
    c = Heroes;
    h = c + arg0;
    m = h->hero_move;
    fast = 0;
    if (h->hero_frozen != 0) {
        ((PadButtons *)Pads)[arg0].unk50 = 0;
        ((PadButtons *)Pads)[arg0].unk54 = 0;
    }
    if ((Pads[arg0].unk0 & 1) || h->hero_ai->AImode != 0) {
        if (arg0 == ArkSidePlayer3) {
            acc = 0x2A;
        }
        if (((PadButtons *)Pads)[arg0].unk50 & 0xF00) {
            max = 0xA0;
            acc = 0x38;
        }
        if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) {
            mask = 0xA0;
        } else {
            mask = 0x50;
        }
        pads = (PadButtons *)Pads;
        p = pads + arg0;
        if (p->unk50 & mask) {
            if (m->velocity_enable != 0) {
                d = 0;
                if (((s32 (*)(EXTengine *))HeroHooks.unk68)(Heroes[arg0].hero_eng) != 0) {
                    fast = 1;
                }
                if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) {
                    b = p->unk50;
                    if (b & 0x80) {
                        d = -acc;
                    }
                    v = b & 0x20;
                } else {
                    b = p->unk50;
                    if (b & 0x40) {
                        d = -acc;
                    }
                    v = b & 0x10;
                }
                if (v != 0) {
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
                do {
                    if (m->forward_speed > max) {
                        m->forward_speed = max;
                    }
                    if (m->forward_speed < -max) {
                        m->forward_speed = -max;
                    }
                } while (0);
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
            ArkHeroSpeedDelta[arg0] = m->forward_speed;
        } else {
            ArkHeroSpeedDelta[arg0] = 0;
            if (((s32 (*)(EXTengine *))HeroHooks.unk68)(Heroes[arg0].hero_eng) != 0) {
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

void AR_PI_PreMoveHero(u32 arg0) {
    EXTheromove *mv;
    HeroInfo *h;
    PadButtons *p;
    s32 mask;
    s32 d;
    s32 hit;
    s32 step;
    s32 max;
    s32 t;

    hit = 0;
    max = 0x28;
    {
        HeroInfo *hb = Heroes;
        h = &hb[arg0];
    }
    mv = h->hero_move;
    step = 0xD;
    if (h->hero_frozen != 0) {
        HPADS[arg0].unk50 = 0;
        HPADS[arg0].unk54 = 0;
    }
    if ((*(s32 *)HPADS[arg0].unk0 & 1) || h->hero_ai->AImode != 0) {
        if (arg0 == ArkSidePlayer3) {
            step = 0x15;
        }
        if (HPADS[arg0].unk50 & 0xF00) {
            max = 0x48;
            step = 0x12;
        }
        mask = (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) ? 0xA0 : 0x50;
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        if (p->unk50 & mask) {
            if (mv->velocity_enable != 0) {
                d = 0;
                if (HeroHooks.unk68(Heroes[arg0].hero_eng) != 0) {
                    hit = 1;
                }
                if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) {
                    if (p->unk50 & 0x80) {
                        d = -step;
                    }
                    if (p->unk50 & 0x20) {
                        d += step;
                    }
                } else {
                    if (p->unk50 & 0x40) {
                        d = -step;
                    }
                    if (p->unk50 & 0x10) {
                        d += step;
                    }
                }
                if (hit != 0) {
                    do {
                        if (d < 0) {
                            d -= max >> 1;
                        }
                        if (d > 0) {
                            d += max >> 1;
                        }
                    } while (0);
                }
                mv->forward_speed += d;
                if (mv->forward_speed > max) {
                    mv->forward_speed = max;
                }
                if (mv->forward_speed < -max) {
                    mv->forward_speed = -max;
                }
                if (mv->forward_speed == 0) {
                    mv->forward_speed = (d < 0) ? -1 : 1;
                }
            }
            ArkHeroSpeedDelta[arg0] = mv->forward_speed;
        } else {
            ArkHeroSpeedDelta[arg0] = 0;
            if (HeroHooks.unk68(Heroes[arg0].hero_eng) != 0) {
                mv->forward_speed = 0;
            }
            if (mv->forward_speed > 0) {
                mv->forward_speed -= 6;
                if (mv->forward_speed < 0) {
                    mv->forward_speed = 0;
                }
            }
            if (mv->forward_speed < 0) {
                mv->forward_speed += 6;
                if (mv->forward_speed > 0) {
                    mv->forward_speed = 0;
                }
            }
        }
    }
}

void AR_MoveHero(u32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *c;
    HeroInfo *f;
    HeroInfo *b;
    EngineHooks *hooks;
    s32 a1;
    s32 sp;

    c = Heroes;
    g = c + arg0;
    a1 = 0;
    if (!(g->hero_status & 0xFA00)) {
        s32 d;
        s32 y;
        sp = ((((((GameFrameCount + arg0) << 6) & 0xFFF) - 0x400) & 0xFFF) + rcossin_tbl)->unk2;
        a1 = sp * 17 >> 10;
        y = World_floor_level;
        d = (sp * 15 >> 11) - 0x80;
        g->hero_eng->unk10.y = y + d;
    } else {
        g->hero_eng->unk10.y = -0x80;
    }
    hooks = &HeroHooks;
    ((void (*)(s32))hooks->unk2C)(arg0);
    f = Heroes;
    g = f + arg0;
    sp = g->hero_move->forward_speed;
    if (sp != 0) {
        if (arg0 == ArkSidePlayer3) {
            a1 = sp;
            g->hero_eng->unk10.x += sp;
        }
        if (arg0 == ArkSidePlayer0) {
            a1 = -sp;
            g->hero_eng->unk10.x += sp;
        }
        if (arg0 == ArkSidePlayer2) {
            a1 = sp;
            g->hero_eng->unk10.z += sp;
        }
        if (arg0 == ArkSidePlayer1) {
            a1 = -sp;
            g->hero_eng->unk10.z += sp;
        }
        ((void (*)(EXTengine *))hooks->unk68)(g->hero_eng);
    }
    b = Heroes;
    h = b + arg0;
    sp = func_800154A0(h->hero_eng->unk20, a1);
    {
        EXTengine *e2 = h->hero_eng;
        e2->unk20 = (u16)e2->unk20 + (sp >> 2);
        ArkHero2ObjectCollision(arg0);
    }
    h->hero_eng->unk5E = 1;
    if (!(h->hero_status & 0xF800)) {
        if (h->hero_mode == 0x21) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        if (h->hero_attack_timer != 0) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        h->hero_sub->unk74 = 0x2004;
    }
}

void AR_PI_MoveHero(u32 arg0) {
    HeroInfo *h;
    HeroInfo *g;
    HeroInfo *b;
    HeroInfo *c;
    EXTengine *e;
    s32 a0;
    s32 a1;
    s32 sp;
    s32 y;
    s32 r;

    r =((s32 (*)(EXTengine *))func_80025F44)(Heroes[arg0].hero_eng);
    e = Heroes[arg0].hero_eng;
    y = e->unk10.y - r;
    if (!(Heroes[arg0].hero_status & 0xFA00)) {
        s32 t;
        s32 d;
        a0 = ((GameFrameCount + arg0) << 6) & 0xFFF;
        t = rcossin_tbl[(a0 - 0x400) & 0xFFF].unk2;
        a1 = t * 17 >> 10;
        d = (t * 15 >> 11) - 0x80;
        e->unk10.y = y + d;
    } else {
        e->unk10.y = y - 0x80;
        a1 = 0;
    }
    c = Heroes;
    g = c + arg0;
    sp = g->hero_move->forward_speed;
    a0 = g->hero_eng->unk1E;
    if (sp != 0) {
        if (arg0 == ArkSidePlayer3) {
            a0 += sp;
            a1 = sp;
        }
        if (arg0 == ArkSidePlayer0) {
            a0 -= sp;
            a1 = -sp;
        }
        if (arg0 == ArkSidePlayer2) {
            a0 += sp;
            a1 = sp;
        }
        if (arg0 == ArkSidePlayer1) {
            a0 -= sp;
            a1 = -sp;
        }
    }
    b = Heroes;
    h = b + arg0;
    a0 &= 0xFFF;
    h->hero_eng->unk1E = a0;
    h->hero_move->forward_angle = a0;
    sp = func_800154A0(h->hero_eng->unk20, a1);
    {
        EXTengine *e2 = h->hero_eng;
        e2->unk20 = (u16)e2->unk20 + (sp >> 2);
        ArkHero2ObjectCollision(arg0);
    }
    h->hero_eng->unk5E = 1;
    if (!(h->hero_status & 0xF800)) {
        if (h->hero_mode == 0x21) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        if (h->hero_attack_timer != 0) {
            h->hero_sub->unk74 = 0x2006;
            return;
        }
        h->hero_sub->unk74 = 0x2004;
    }
}

s32 ArkHeroModeCheck(s32 arg0) {
    PadButtons *p;
    s32 act;
    s32 t;
    s32 f;

    switch (MODEV->unk34) {
    break;
    case 14:
        if (MODEV->unk32 & 0x80) {
            if (arg0 == 0 || (MODEV->unk6C != 0 && arg0 == 1)) {
                Heroes[arg0].hero_attack_timer = 10;
            }
            goto l17;
        }
    case 17:
l17:
        if (MODEV->unk32 & 0x80) {
            if (arg0 != 0 && (MODEV->unk6C == 0 || arg0 != 1)) {
                if (HPADS[arg0].unk54 & 0x2000) {
                    act = 0x20;
                    goto call;
                }
            }
        }
    case 16:
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        f = p->unk54 & 0x8000;
        goto chk;
    case 15:
        {
            PadButtons *b = HPADS;
            p = &b[arg0];
        }
        t = p->unk54;
        if (t & 0x4000) {
            act = 0x21;
            goto call;
        }
        f = t & 0x8000;
chk:
        if (f) {
            if (Heroes[arg0].hero_attack_timer == 0) {
                act = 0x1F;
call:
                SetHeroAction(arg0, act);
                return 1;
            }
            p->unk4C &= ~0x8000;
        }
        return 0;
    default:
        return 0;
    }
}

void ArkHeroSetSpeed(arg0)
s32 arg0;
{
    u16 *p;
    u16 *b;
    s32 v;
    u16 t;

    v = ArkHeroSpeedDelta[arg0];
    if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer2) {
        v = ~v + 1;
    }
    if (v < 0) {
        t = ArkHeroSpeeds[Heroes[arg0].hero_name][2];
    } else {
        t = ArkHeroSpeeds[Heroes[arg0].hero_name][3];
    }
    b = ArkHeroSpeed;
    p = b + arg0;
    if ((u32)(abs(*p - ArkHeroSpeeds[Heroes[arg0].hero_name][2]) - 2) >= 0x19U) {
        *p = t;
    }
    if ((u32)(abs(*p - ArkHeroSpeeds[Heroes[arg0].hero_name][3]) - 2) >= 0x19U) {
        *p = t;
    }
}

s32 ArkHeroUpdateSpeed(arg0)
s32 arg0;
{
  s32 v;
  s32 t;
  u32 u;
  u16 *p;
  u16 *d;
  subengine *s;
  v = ArkHeroSpeedDelta[arg0];
  if (v == 0)
  {
    d = &ArkHeroSpeed[arg0];
    do
    {
    }
    while (0);
    p = (u16 *) ArkHeroSpeeds;
    p += Heroes[arg0].hero_name * 5;
    if ((*d) <= p[2])
    {
      t = p[2];
    }
    else
    {
      t = p[3];
    }
    if (ArkHeroSpeed[arg0] == t)
    {
      if (p || Heroes[arg0].hero_name)
      {
        return 1;
      }
      else
      {
        return 1;
      }
    }
  }
  else
  {
    if ((arg0 == ArkSidePlayer3) || (arg0 == ArkSidePlayer2))
    {
      v = (~v) + 1;
    }
    if (v < 0)
    {
      u = ArkHeroSpeeds[Heroes[arg0].hero_name][0];
    }
    else
    {
      u = ArkHeroSpeeds[Heroes[arg0].hero_name][1];
    }
    if (ArkHeroSpeed[arg0] == u)
    {
      goto skip;
    }
  }
  ArkHeroSpeed[arg0] += 1;
  skip:
  if (ArkHeroSpeed[arg0] >= 0x1D)
  {
    ArkHeroSpeed[arg0] = 1;
  }

  s = Heroes[arg0].hero_sub;
  s = s->next;
  if (s != 0)
  {
    s->unk74 = ArkHeroSpeeds[Heroes[arg0].hero_name][4] + ArkHeroSpeed[arg0];
    return 0;
  }
  return 1;
}

static inline s32 HeroCheckPad54(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_status & 0xFA00) {
        return 0;
    }
    if (((PadButtons *)Pads)[arg0].unk54 & 0x1000) {
        SetHeroAction(arg0, 0x1E);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 HeroCheckStatus2600(s32 arg0) {
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void ArkHeroIdle(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *base2;
    HeroInfo *h2;
    u16 mode;

    if (HeroCheckPad54(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_aseq == 0x19) {
            if (h->hero_idle_count != 0) {
                h->hero_idle_count = h->hero_idle_count - 1;
            } else {
                h->hero_idle_count = Random(0x258) + 0x12C;
                SetHeroSeq(arg0, 0x1B);
            }
        }
        if (ArkHeroUpdateSpeed(arg0) != 0) {
            func_8008502C(arg0);
        }
        if (HeroCheckStatus2600(arg0) == 0) {
            ArkHeroModeCheck(arg0);
            base2 = Heroes;
            h2 = base2 + arg0;
            if (h2->hero_move->forward_speed != 0) {
                mode = h2->hero_mode;
                if (mode != 0x1F && mode != 0x21) {
                    SetHeroAction(arg0, 0x1D);
                }
            }
        }
    }
}


void SetArkHeroTaunt(u32 arg0) {
    s32 id;
    SLONG_XYZ *pos;
    HeroInfo *base;

    SetHeroSeq(arg0, 0x1C);
    if (Random(3) == 0 || GameInfo[arg0 + 0x5C] == 0) {
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(7, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x39, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x6B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PlaySfx(0x9D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 5:
            PlaySfx(0x133, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 6:
            PlaySfx(0x165, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 7:
            PlaySfx(0x101, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 8:
            PlaySfx(0xCF, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
}

void ArkHeroTaunt(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 act;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        ArkHeroModeCheck(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction(arg0, 0x1D);
            }
        }
        func_8008502C(arg0);
    }
}

void ArkHeroMove(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction(arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        ArkHeroModeCheck(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed == 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction(arg0, 0x1C);
            }
        }
        if (ArkHeroUpdateSpeed(arg0) != 0) {
            func_8008502C(arg0);
        }
    }
}

void SetArkHeroWinner(u32 arg0) {
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
    func_80026A04((u8 *)h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    func_80085718(arg0, 0);
    func_80089FA4(arg0);
}

void ArkHeroWinner(u32 arg0) {
    EXTengine *e;
    EXTengine *f;
    s32 v;
    s32 done;

    done = 0;
    if (arg0 == ArkSidePlayer3) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.z;
        if (v > 0x500) {
            e->unk10.z = v - 0x20;
        } else {
            done = 1;
        }
        if (GameTeams[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.x < 0) {
                f->unk10.x += 0x20;
            }
        }
    }
    if (arg0 == ArkSidePlayer0) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.z;
        if (v < -0x500) {
            e->unk10.z = v + 0x20;
        } else {
            done = 1;
        }
        if (GameTeams[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.x > 0) {
                f->unk10.x -= 0x20;
            }
        }
    }
    if (arg0 == ArkSidePlayer1) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.x;
        if (v > 0x500) {
            e->unk10.x = v - 0x20;
        } else {
            done = 1;
        }
        if (GameTeams[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.z < 0) {
                f->unk10.z += 0x20;
            }
        }
    }
    if (arg0 == ArkSidePlayer2) {
        e = Heroes[arg0].hero_eng;
        v = e->unk10.x;
        if (v < -0x500) {
            e->unk10.x = v + 0x20;
        } else {
            done = 1;
        }
        if (GameTeams[0] != 0) {
            f = Heroes[arg0].hero_eng;
            if (f->unk10.z > 0) {
                f->unk10.z -= 0x20;
            }
        }
    }
    if (done != 0) {
        CameraMode = 1;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void SetArkHeroLose(u32 arg0) {
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
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x19);
}

void SetArkHeroDie(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    subengine *s;
    ArkenoidModeView *m;
    s32 i;

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
    func_80026980(h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x20);
    func_80028018(arg0, 0x3C);
    s = h->hero_sub->next;
    if (s != NULL) {
        s = s->next;
        if (s != NULL) {
            s->flags = 0;
        }
    }
    if (arg0 == ArkSidePlayer0) {
        i = 0;
    } else if (arg0 == ArkSidePlayer3) {
        i = 1;
    } else if (arg0 == ArkSidePlayer2) {
        i = 3;
    } else {
        i = (arg0 == ArkSidePlayer1) * 2;
    }
    if (ArkDoorsA[i].sub != NULL) {
        ArkDoorsA[i].sub->unk74 = ArkDoorsA[i].unkE;
    }
    m = (ArkenoidModeView *)GameInfo;
    switch (m->unk34) {
    case 16:
        CreateNAS(arg0, World_floor_level - 0x50, -0x20, 0x1506, 0x11);
        break;
    case 17:
        if (m->unk30 & 0x801) {
            ArkWallFlags = 0;
            if (ArkDieEngine.sfx != NULL) {
                StopSfx(ArkDieEngine.sfx);
            }
        } else {
            if (ArkDoorsB[i].flags != NULL) {
                *ArkDoorsB[i].flags |= 0x8000;
            }
            if (ArkDeadWalls == 0) {
                PlaySfx(0x591, &ArkDieEngine.sfx, NULL, 0x1000, 0x1E00, 0);
                ArkDeadWalls = 1;
            }
            if (arg0 == ArkSidePlayer0) {
                ArkWallFlags |= 1;
            }
            if (arg0 == ArkSidePlayer3) {
                ArkWallFlags |= 2;
            }
            if (arg0 == ArkSidePlayer2) {
                ArkWallFlags |= 4;
            }
            if (arg0 == ArkSidePlayer1) {
                ArkWallFlags |= 8;
            }
        }
        break;
    default:
        CreateNAS(arg0, World_floor_level - 0x50, 0, 0x1505, 0x11);
        break;
    }
    Heroes[arg0].hero_hit = 0x3C;
}

void SetArkHeroKick(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    ArkHeroSetSpeed();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x1D);
    h->hero_attack_delay += 0x1E;
    if (h->hero_attack_delay > 0x5A) {
        h->hero_attack_delay = 0x5A;
    }
    h->hero_attack_counter = 0;
    h->hero_attack_timer = h->hero_attack_delay;
    PlaySfx(ArkKickFrames[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void ArkHeroKick(u32 arg0) {
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
            s->unk74 = 0x2001;
            s->unk20.x = scale & 0xFFFF;
            s->unk20.y = scale & 0xFFFF;
            s->unk20.z = scale & 0xFFFF;
            s->unk76 = ang;
            s->unk52 = 1;
            s->flags |= 0x40008021;
        }
    }
    if (ArkHeroRad2ObjectCollision(arg0, (scale & 0xFFFF) >> 3, 0)) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    base2[arg0].hero_attack_counter++;
    if (base2[arg0].hero_attack_counter >= 4) {
        SetHeroAction(arg0, 0x1C);
        if (s != 0) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (ArkHeroUpdateSpeed(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void ArkHeroRedKick(u32 arg0) {
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;
    subengine *a;
    subengine *s;
    u16 cnt;
    s32 v;
    s32 r;

    s = NULL;
    base = Heroes;
    cnt = base[arg0].hero_attack_counter;
    a = base[arg0].hero_sub;
    v = cnt * 0x600 + 0x1000;
    r = (cnt << 9) + 0x800;
    a = a->next;
    if (a != NULL) {
        s = a->next;
        if (s != NULL) {
            s->unk74 = 0x2007;
            s->unk20.x = v & 0xFFFF;
            s->unk20.y = v & 0xFFFF;
            s->unk20.z = v & 0xFFFF;
            s->unk76 = r;
            s->unk52 = 1;
            s->flags |= 0x40008021;
        }
    }
    if (ArkHeroRad2ObjectCollision(arg0, (v & 0xFFFF) >> 3, 1) != 0) {
        Heroes[arg0].hero_attack_timer = 0;
    }
    base2 = Heroes;
    h = &base2[arg0];
    cnt = h->hero_attack_counter + 1;
    h->hero_attack_counter = cnt;
    if (cnt >= 4) {
        SetHeroAction(arg0, 0x1C);
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (ArkHeroUpdateSpeed(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void ArkHeroGrab(u32 arg0) {
    subengine *a;
    subengine *s;
    HeroInfo *h;
    HeroInfo *b;
    s32 t;
    s32 f;
    s16 w;

    s = NULL;
    a = Heroes[arg0].hero_sub;
    a = a->next;
    if (a != NULL) {
        s = a->next;
        if (s != NULL) {
            s->unk74 = 0x2003;
            s->flags |= 0x40008021;
            t = rcossin_tbl[(((GameFrameCount << 7) & 0xFFF) - 0x400) & 0xFFF].unk2 * 1024 >> 13;
            s->unk52 = 1;
            s->unk20.x = t + 0x1000;
            s->unk20.y = t + 0x1000;
            s->unk20.z = t + 0x1000;
            s->unk76 = 0xA00 - t;
        }
    }
    b = Heroes;
    h = b + arg0;
    h->hero_attack_counter += 1;
    if (h->hero_weapon_timer != 0) {
        w = h->hero_weapon_timer - 1;
        h->hero_weapon_timer = w;
        if (w < 0x91) {
            if (!(HPADS[arg0].unk50 & 0x4000)) {
                f = 1;
                if (HLV->out[arg0] != 0) {
                    f = h->hero_ai->AItimer3 < 1U;
                }
                if (f != 0) {
                    FreeGrabbedObject(arg0, 1);
                    goto act;
                }
            } else {
                h->hero_ai->AItimer3 = 2;
            }
        }
    } else {
        FreeGrabbedObject(arg0, 0);
act:
        SetHeroAction(arg0, 0x1C);
        if (s != NULL) {
            s->flags &= 0xFFFF7FFF;
        }
    }
    if (ArkHeroUpdateSpeed(arg0) != 0) {
        func_8008502C(arg0);
    }
}

void SetArkHeroDead(u32 arg0) {
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
    sp18 = temp_v0->unk10;
    func_8007A524(0, (s32 *)&sp18, (s32) &BallsEffectsModel);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->unk10, 0x1000, 0x1E00, 0);
    func_800898EC(arg0);
}

void ArkHeroDead(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x22);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = HeroModelBank + HeroHooks.unk4[*(volatile u16 *)&h->hero_name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
    } else {
        HeroGhost(arg0);
    }
}

s32 ArkBotAimA(s32 arg0, ArkenoidPos *from, ArkenoidPos *to, s32 angle) {
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

    if (arg0 == ArkSidePlayer0) {
        i = 0;
    } else if (arg0 == ArkSidePlayer3) {
        i = 1;
    } else if (arg0 == ArkSidePlayer2) {
        i = 3;
    } else {
        i = (arg0 == ArkSidePlayer1) * 2;
    }
    n = 0x20 - AiPlayerSkill[arg0].unk0;
    n -= ArkBotSkill;
    x0 = ArkBotEdgesA[i].x0;
    z0 = ArkBotEdgesA[i].z0;
    x1 = ArkBotEdgesA[i].x1;
    z1 = ArkBotEdgesA[i].z1;
    if (n < 0) {
        n = 0;
    }
    n = ArkBotAimAngles[n];
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
    to->x = from->x - rcossin_tbl[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - rcossin_tbl[ang & 0xFFF].unk2;
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

s32 ArkBotAimB(s32 arg0, ArkenoidPos *from, ArkenoidPos *to, s32 angle) {
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

    if (arg0 == ArkSidePlayer0) {
        i = 0;
    } else if (arg0 == ArkSidePlayer3) {
        i = 1;
    } else if (arg0 == ArkSidePlayer2) {
        i = 3;
    } else {
        i = (arg0 == ArkSidePlayer1) * 2;
    }
    n = 0x20 - AiPlayerSkill[arg0].unk0;
    n -= ArkBotSkill;
    x0 = ArkBotEdgesB[i].x0;
    z0 = ArkBotEdgesB[i].z0;
    x1 = ArkBotEdgesB[i].x1;
    z1 = ArkBotEdgesB[i].z1;
    if (n < 0) {
        n = 0;
    }
    n = ArkBotAimAngles[n];
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
    to->x = from->x - rcossin_tbl[(ang - 0x400) & 0xFFF].unk2;
    to->z = from->z - rcossin_tbl[ang & 0xFFF].unk2;
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

s32 PickArkPlayer(u32 arg0) {
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += AiPlayerSkill[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 ArkBotCanHit(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5) {
    ArkenoidPos buf;
    ArkenoidNode *n;
    ArkenoidNode *t;
    ArkenoidNode *best;
    HeroInfo *h;
    HeroInfo *h1;
    HeroInfo *h2;
    s32 dx;
    s32 dz;
    s32 a;
    s32 d;
    s32 r;

    arg2 *= arg2;
    if (MODEV->unk34 < 0xE) {
        goto ret0;
    }
    if (MODEV->unk34 >= 0x11) {
        if (MODEV->unk34 != 0x11) {
            return 0;
        }
        if (MODEV->unk32 & 0x80) {
            n = HeadOfBalls;
            if (n != NULL) {
                HeroInfo *base = Heroes;

                h1 = &base[arg0];
                do {
                    t = n->target;
                    if (t != NULL) {
                        dx = h1->hero_eng->unk10.x - t->x;
                        dz = h1->hero_eng->unk10.z - t->z;
                        if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->unk14 == 2) {
                            goto set_ai;
                        }
                    }
                    n = n->next;
                } while (n != NULL);
            }
        }
    }
    if (Heroes[arg0].hero_attack_timer != 0) {
        return 0;
    }
    best = NULL;
    n = HeadOfBalls;
    if (n != NULL) {
        h = &Heroes[arg0];
    loop:
        /* the loop notes weight these refs for global-alloc as a loop's would, without loop.c
           hoisting the (s16) casts out as it does from a real loop */
        do {
            t = n->target;
            if (t != NULL) {
                dx = h->hero_eng->unk10.x - t->x;
                dz = h->hero_eng->unk10.z - t->z;
                if (dx * dx + dz * dz < arg2 && n->unk58 != NULL && n->unk58->unk14 != 2) {
                    a = get_angle(dx, dz);
                    d = func_800154A0(a, h->hero_move->forward_angle);
                    if (arg1 == -1) {
                        if ((s16)arg3 < d) {
                            if (d < (s16)arg4) {
                                best = n;
                            }
                        }
                    } else if (FindExtension((NAS *)n, 0x800) != NULL) {
                        r = ArkBotAimB(arg1, (ArkenoidPos *)&t->x, &buf, (u16)a);
                        best = n;
                        if (r != 0) {
                            goto found;
                        }
                    }
                }
            }
        } while (0);
        n = n->next;
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
        a = get_angle(h2->hero_eng->unk10.x - t->x, h2->hero_eng->unk10.z - t->z);
        if (FindExtension((NAS *)best, 0x800) == NULL) {
            goto ret0;
        }
        if ((ArkBotAimB(arg1, (ArkenoidPos *)&t->x, &buf, (u16)a) ^ 1) == 0) {
            goto ret0;
        }
    }
found:
    return 1;
set_ai:
    h1->hero_ai->AIpad |= 0x2000;
    return 0;
ret0:
    return 0;
}

s32 ArkBallComingAtHero(s32 arg0) {
    ArkenoidNode *n;
    ArkenoidNode *t;
    ArkenoidNode *e;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 lim;
    HeroInfo *base;

    if (GameMode == 0xF) {
        lim = 0x64000;
        n = HeadOfBalls;
        if (n != NULL) {
            base = Heroes;
            h = &base[arg0];
            do {
                t = n->target;
                if (t != NULL) {
                    e = (ArkenoidNode *)h->hero_eng;
                    dx = e->x - t->x;
                    dz = e->z - t->z;
                    if (dx * dx + dz * dz < lim) {
                        if (func_80015480(get_angle(dx, dz), h->hero_move->forward_angle) < 0x2AA) {
                            return 1;
                        }
                    }
                }
                n = n->next;
            } while (n != NULL);
        }
    }
    return 0;
}

/* a debug printf format nothing prints with in retail; its place in .rodata is between two
 * functions' jump tables, so it is defined here */
const char ArkAiDebugFormat[] = "%d ai mode %d target %x %x  angle %x dist %x\n";

void Ark_HeroBot(u32 arg0) {
    s32 unused[2];
    ArkenoidPos best;
    ArkenoidPos pos;
    s32 dist;
    s32 ang;
    s32 mate;
    EXTbotai *ai;
    ArkenoidAiNode *n;
    ArkenoidAiNode *node;
    EXTengine *e;
    EXTengine *obj;
    EXTengine *eng;
    ArkenoidAiExt *x;
    ArkenoidAiExt *ext;
    s32 min;
    s32 target;
    s32 row;
    s32 col;
    s32 k;
    ArkenoidPos *at;
    s32 range;
    s32 spread;
    s32 dx;
    s32 d;
    s32 r;
    s32 flag;
    s32 v;
    PadState *pad;
    D_8009D87C_t *sk;

    if (GamePause & 0x04000000) {
        return;
    }
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
    if (ai->AItimer3 != 0) {
        ai->AItimer3--;
    }
    ai->AIpad = 0;
    mate = Heroes[arg0].hero_teammate;
    if (Heroes[arg0].hero_mode == (u16)HeroActHit || Heroes[arg0].hero_mode == (u16)HeroActDead) {
        ai->AIsleep = 5;
        ai->AImode = 0x14;
    }
    min = 0x7FFFFF;
    n = HeadOfBalls;
    if (ai->TargetPlayer == -1) {
        ai->TargetPlayer = PickArkPlayer(arg0);
    }
    target = ai->TargetPlayer;
    k = (0x20 - AiPlayerSkill[arg0].unk0) << 4;
    for (; n != NULL; n = n->next) {
        e = n->eng;
        if (e != NULL && (ext = (ArkenoidAiExt *)n->fux) != NULL && ext->unk1A == 1) {
            x = FindExtension((NAS *)n, 0x800);
            if (x != NULL && ArkBotAimA(arg0, (ArkenoidPos *)&e->unk10, &pos, x->unk16) != 0) {
                dx = (e->unk10.x + k) - (pos.x + k);
                row = e->unk10.z - pos.z;
                dx = dx * dx;
                d = dx + row * row;
                if (d < min) {
                    min = d;
                    best = pos;
                }
            }
        }
    }
    if (target != -1) {
        row = arg0 == ArkSidePlayer0;
        if (arg0 == ArkSidePlayer2) {
            row = 2;
        }
        if (arg0 == ArkSidePlayer1) {
            row = 3;
        }
        if (target == *ArkWallTargets[row][0]) {
            col = 0;
        } else if (target == *ArkWallTargets[row][1]) {
            col = 1;
        } else if (target == *ArkWallTargets[row][2]) {
            col = 2;
        } else {
            col = 3;
        }
        v = ArkBotWaits[col];
        if (arg0 == ArkSidePlayer3) {
            best.x -= v;
        }
        if (arg0 == ArkSidePlayer0) {
            best.x += v;
        }
        if (arg0 == ArkSidePlayer2) {
            best.z -= v;
        }
        if (arg0 == ArkSidePlayer1) {
            best.x -= v;
        }
    }
    if (GameMode == 0x10) {
        k = (0x20 - AiPlayerSkill[arg0].unk0) << 5;
        if (k <= 0) {
            k = 1;
        }
        for (node = HeadOfObjects; node != NULL; node = node->child) {
            obj = node->eng;
            if (obj != NULL && (ext = (ArkenoidAiExt *)node->fux) != NULL &&
                ((ArkenoidAiFux *)ext)->unk24 == 0x12) {
                eng = Heroes[arg0].hero_eng;
                dx = eng->unk10.x - obj->unk10.x;
                dx += k;
                row = eng->unk10.z - obj->unk10.z;
                row += k;
                d = dx * dx + row * row;
                if (d < min) {
                    min = d;
                    best = *(ArkenoidPos *)&obj->unk10;
                }
            }
        }
    }
    if (min != 0x7FFFFF) {
        ang = get_angle(best.x - Heroes[arg0].hero_eng->unk10.x,
                        best.z - Heroes[arg0].hero_eng->unk10.z);
        dist = csqrt(min) >> 6;
    } else {
        dist = 0;
        ang = get_angle(ai->TargetXYZ.x - Heroes[arg0].hero_eng->unk10.x,
                        ai->TargetXYZ.z - Heroes[arg0].hero_eng->unk10.z);
    }
    if (ai->AItimer2 != 0) {
        if (ArkBallComingAtHero(arg0) != 0) {
            ai->AIpad |= 0x4000;
        } else {
            ai->AItimer2 = 0;
        }
        if (ai->AItimer2 < 0x1E) {
            for (node = HeadOfBalls; node != NULL; node = node->next) {
                ext = FindExtension((NAS *)node, 0x1000);
                if (ext != NULL && ext->unk1A == 2 && arg0 == (s16)ext->unk16) {
                    target = ai->TargetPlayer;
                    if (target != -1) {
                        obj = node->eng;
                        if (obj != NULL) {
                            x = FindExtension((NAS *)node, 0x800);
                            if (x != NULL) {
                                at = (ArkenoidPos *)&obj->unk10;
                                range = x->unk16;
                                if (ArkBotAimA(target, at, &pos, range) != 0) {
                                    ai->AItimer2 = 0;
                                    ai->TargetPlayer = -1;
                                    ai->AIpad &= ~0x4000;
                                } else if ((target = GameFrameCount & 3) != arg0 && target != mate &&
                                           ArkBotAimA(target, at, &pos, range) != 0) {
                                    ai->AItimer2 = 0;
                                    ai->TargetPlayer = -1;
                                    ai->AIpad &= ~0x4000;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    switch (ai->AImode) {
    case 3:
        k = AiPlayerSkill[arg0].unk0 * 0x10;
        sk = &AiPlayerSkill[arg0];
        k -= ArkBotSkill;
        if (k <= 0) {
            k = 1;
        }
        spread = Random(k) + 0x100;
        k += spread;
        if (GameMode == 0xF) {
            if (ai->AItimer2 == 0) {
                target = ai->TargetPlayer;
                flag = 0;
                if (target != -1) {
                    if (ai->AItimer1 == 0) {
                        if ((s16)Random(0x18) > sk->unk0) {
                            ai->AItimer1 = ((0x20 - sk->unk0) >> 1) + 0x40;
                            if (ai->AItimer1 == 0) {
                                ai->AItimer1 = 1;
                            }
                        } else {
                            flag = 1;
                        }
                    }
                    if (HeroesAllOut == 0) {
                        flag = 1;
                    }
                    if (((s32 (*)(s32, s32, s32, s32, s32, s32))ArkBotCanHit)(arg0, target, k, -0x200, 0x200, 0) != 0) {
                        ai->TargetPlayer = -1;
                        ai->AIpad |= 0x8000;
                    } else {
                        if (ai->AItimer1 != 0) {
                            ai->AItimer1--;
                        }
                        if (ArkBallComingAtHero(arg0) != 0) {
                            ai->AItimer2 = 0x5A;
                            ai->AIpad |= 0x4000;
                        } else if (((s32 (*)(s32, s32, s32, s32, s32, s32))ArkBotCanHit)(arg0, target, k, -0x200, 0x200, flag) != 0) {
                            ai->TargetPlayer = -1;
                            ai->AIpad |= 0x8000;
                        }
                    }
                } else {
                    if (ai->AItimer1 != 0) {
                        ai->AItimer1--;
                    }
                    if (ArkBallComingAtHero(arg0) != 0) {
                        ai->AItimer2 = 0x5A;
                        ai->AIpad |= 0x4000;
                    }
                }
            }
        } else {
            target = ai->TargetPlayer;
            flag = 0;
            if (target != -1) {
                if (ai->AItimer1 == 0) {
                    if ((s16)Random(0x20) > sk->unk0) {
                        ai->AItimer1 = ((0x20 - sk->unk0) >> 1) + 0x20;
                        if (ai->AItimer1 == 0) {
                            ai->AItimer1 = 1;
                        }
                    } else {
                        flag = 1;
                    }
                }
                if (HeroesAllOut == 0) {
                    flag = 1;
                }
                if (((s32 (*)(s32, s32, s32, s32, s32, s32))ArkBotCanHit)(arg0, target, k, -0x200, 0x200, flag) != 0) {
                    if ((GameLevelFlags & 0x80) && (u16)ArkRedKickTimer == 0) {
                        ai->AIpad |= 0x2000;
                    } else {
                        ai->TargetPlayer = -1;
                        ai->AIpad |= 0x8000;
                    }
                } else if (ai->AItimer1 != 0) {
                    ai->AItimer1--;
                }
            }
        }
        if (dist != 0) {
            eng = Heroes[arg0].hero_eng;
            dx = eng->unk10.x - best.x;
            r = eng->unk10.z - best.z;
            if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) {
                v = abs(dx);
            } else {
                v = abs(r);
            }
            v += (AiPlayerSkill[arg0].unk0 + ArkBotSkill) << 5;
            if (abs(v) > 0x280) {
                if (dist < v) {
                    ai->AIpad |= 0xF00;
                }
                ai->AIang = ang;
                ai->AIpad |= DirPadBits[(ang >> 9) & 7];
            }
        } else {
            ai->AImode = 1;
        }
        break;
    case 4:
        if (dist != 0) {
            goto abort;
        }
        eng = Heroes[arg0].hero_eng;
        dx = eng->unk10.x - ai->TargetXYZ.x;
        row = eng->unk10.z - ai->TargetXYZ.z;
        d = csqrt(dx * dx + row * row) >> 6;
        if (d < 0x80) {
            ai->AIsleep = Random(0x3C) + 0x1E;
            ai->AImode = 5;
        } else {
            ai->AIang = ang;
            ai->AIpad |= DirPadBits[(ang >> 9) & 7];
        }
        break;
    case 21:
        if (dist != 0) {
            goto abort;
        }
        if (Heroes[arg0].hero_mode != 0x1E) {
            ai->AImode = 1;
        }
        break;
    case 5:
        if (dist == 0) {
            if (ai->AIsleep == 0) {
                ai->AImode = 0x15;
                ai->AIpad |= 0x1000;
            }
            break;
        }
    abort:
        ai->AImode = 3;
        ai->AIaborted_mode = 0;
        break;
    case 20:
        break;
    default:
        ai->TargetXYZ = Heroes[arg0].hero_eng->unk10;
        if (arg0 == ArkSidePlayer3 || arg0 == ArkSidePlayer0) {
            ai->TargetXYZ.x = Random(0x400) - 0x200;
        } else {
            ai->TargetXYZ.z = Random(0x400) - 0x200;
        }
        ai->AImode = 4;
        break;
    }
    pad = &Pads[arg0];
    set_joypad((u8 *)pad, ai->AIpad);
    *(s32 *)((u8 *)pad + 0x38) = ai->AIang;
}

void FinishArkLevel(void) {
    s32 best = 0;
    s32 idx = -1;
    s32 i;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if ((h->hero_status & 0xFA00) == 0 && best < h->hero_health) {
            best = h->hero_score;
            idx = i;
        }
    }
    if (idx != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if ((h->hero_status & 0xFA00) == 0 && i != idx) {
                h->hero_score = 0;
                h->hero_health = 0;
                h->hero_status |= 0x2000;
            }
        }
    }
}

NAS *CreateBall(NAS *arg0) {
    NAS *nas;
    ArkenoidEng *src;
    ArkenoidEng *eng;
    ArkenoidFux *fux;
    ArkenoidMot *dst;
    ArkenoidMot *srcm;
    s32 dir;

    nas = NULL;
    if (arg0 != NULL) {
        src = (ArkenoidEng *)arg0->Engine;
        if (src != NULL) {
            nas = CreateNAS(src->x, src->pad14, src->z, 0x1501, 0x11);
            if (nas != NULL) {
                eng = (ArkenoidEng *)nas->Engine;
                if (eng != NULL) {
                    eng->flags |= 0x8000;
                    if (eng->sub != NULL) {
                        eng->sub->flags |= 0x8000;
                    }
                    fux = (ArkenoidFux *)nas->FUX;
                    if (fux != NULL) {
                        fux->flag = 1;
                        dst = FindExtension(nas, 0x800);
                        if (dst != NULL) {
                            srcm = FindExtension(arg0, 0x800);
                            if (srcm != NULL) {
                                dst->b = 4;
                                dst->c = 1;
                                dst->a = srcm->a;
                                dst->lim = srcm->lim;
                                dir = srcm->dir;
                                srcm->dir = (dir + 0x2AA) & 0xFFF;
                                src->x -= rcossin_tbl[(srcm->dir - 0x400) & 0xFFF].unk2 * 128 >> 12;
                                src->z -= rcossin_tbl[srcm->dir & 0xFFF].unk2 * 128 >> 12;
                                if (dst->lim < 0x50) {
                                    dst->lim = 0x50;
                                }
                                if (dst->a < dst->lim) {
                                    dst->a = dst->lim;
                                }
                                dst->dir = (dir - 0x2AA) & 0xFFF;
                                eng->x -= rcossin_tbl[(dst->dir - 0x400) & 0xFFF].unk2 * 128 >> 12;
                                eng->z -= rcossin_tbl[dst->dir & 0xFFF].unk2 * 128 >> 12;
                            }
                        }
                    }
                }
            }
        }
    }
    return nas;
}

void ArkSpawnBall(void) {
    NAS *nas;
    EXTengine *eng;
    EXTobject *fux;
    EXTmotion *mot;
    s32 dir;
    s32 n;

    nas = HeadOfBalls;
    n = 0;
    if (nas != NULL) {
        do {
            nas = *(NAS **)((u8 *)nas + 0x20);
            n++;
        } while (nas != NULL);
    }
    if (n < 8) {
        nas = CreateNAS(((ArkenoidSpawn *)ArkBallSpawner)->x, ((ArkenoidSpawn *)ArkBallSpawner)->y, ((ArkenoidSpawn *)ArkBallSpawner)->z, 0x1501, 0x11);
        if (nas != NULL) {
            eng = nas->Engine;
            if (eng != NULL) {
                eng->flags |= 0x8000;
                if (eng->Engine != NULL) {
                    eng->Engine->flags |= 0x8000;
                }
                fux = (EXTobject *)nas->FUX;
                if (fux != NULL) {
                    fux->status = 1;
                    fux->can_do = 0x40;
                    mot = FindExtension(nas, 0x800);
                    if (mot != NULL) {
                        mot->Xacc = 4;
                        mot->Xdec = 1;
                        mot->Xvel = 0x80;
                        mot->Xmax = 0x80;
                        dir = ((ArkenoidSpawn *)ArkBallSpawner)->dir;
                        mot->Xa = dir;
                        eng->unk10.x = rcossin_tbl[(dir - 0x400) & 0xFFF].unk2 * 13 >> 7;
                        eng->unk10.z = rcossin_tbl[dir & 0xFFF].unk2 * 13 >> 7;
                    }
                }
                PlaySfx(0x5C1, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
}

void InitBall(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    EXTengine *e1;
    EXTobject *obj;
    EXTmotion *mot;
    subengine *sub;
    u8 *ext;
    s32 i;

    eng = nas->Engine;
    if (eng != NULL) {
    eng->sub->flags |= 0x02000000;
    eng->flags &= 0xFFFF7FFF;
    nas->Class |= 0x2000;
    obj = (EXTobject *)AddObject(nas);
    if (obj != NULL) {
        mot = AddMotion(nas);
        if (mot != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x300);
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
            mot->Ftime = 0;
        }
    }
    eng->unk10.y = World_floor_level - 0x80;
    eng->unk5D = 2;
    eng->sub->unk6C = BallsEffectsModel;
    if (GameMode == 0xF) {
        eng->sub->unk74 = 0x2017;
    } else {
        eng->sub->unk74 = 0x2009;
    }
    func_80026B70((u8 **)&HeadOfBalls, (u8 *)nas);
    e1 = AddExtEngine(nas, 1);
    if (e1 != NULL) {
        sub = e1->sub;
        sub->unk6C = BallsEffectsModel;
        if (GameMode == 0xF) {
            sub->unk74 = 0x2016;
        } else {
            sub->unk74 = 0x200B;
        }
        e1->flags |= 0x22000021;
        sub->flags |= 0x22000021;
        e1->flags &= 0xFFFF7FFF;
    }
    e = AddExtEngine(nas, 1);
    if (e != NULL) {
        sub = e->sub;
        e->unk5D = 0;
        e->unk5C = 0;
        e->flags &= 0xFFFF7FFF;
        sub->unk6C = BallsEffectsModel;
        if (GameMode == 0xE) {
            sub->unk74 = 0x200C;
            sub->unk76 = 0xB00;
            sub->flags |= 0x42000021;
            ext = AddExt(nas, 0x74, 0x10000000);
            if (ext != NULL) {
                i = 7;
                ext += i;
                do {
                    ext[0x6C] = 0;
                    i--;
                    ext--;
                } while (i >= 0);
                func_8001D390(e, 6);
                for (sub = sub->next; sub != NULL; sub = sub->next) {
                    sub->unk51 = 0;
                    sub->unk50 = 1;
                    sub->unk6C = BallsEffectsModel;
                    sub->unk74 = 0x200D;
                    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x42000021;
                    sub->unk76 = 0xC00;
                }
            }
        } else {
            sub->unk74 = 0;
        }
    }
    }
}

/* The side (0-3) a player defends. */
static inline s32 ArkPlayerSide(s32 player) {
    if (player == ArkSidePlayer0) {
        return 0;
    }
    if (player == ArkSidePlayer3) {
        return 1;
    }
    if (player == ArkSidePlayer2) {
        return 3;
    }
    return (player == ArkSidePlayer1) * 2;
}

void PlotBall(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    EXTengine *e2;
    subengine *sub;
    ArkenoidTrail *trail;
    s32 hide;
    s32 done;
    s16 k;
    s32 ang;
    s16 d;
    s32 dx;
    s32 dz;
    s32 y;
    s32 dist;
    s32 sc;
    s32 i;
    s32 idx;
    s32 yv;
    s16 shade;
    s16 pl;

    hide = 0;
    eng = nas->Engine;
    done = 0;
    if (eng == NULL) {
        return;
    }
    eng->OldXYZ = eng->unk10;
    obj = (EXTobject *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    mot = FindExtension(nas, 0x800);
    if (mot != NULL) {
        if (mot->Ftime != 0) {
            mot->Ftime--;
        }
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
        eng->unk10.x -= (mot->Xvel * rcossin_tbl[(mot->Xa - 0x400) & 0xFFF].unk2) >> 12;
        eng->unk10.z -= (mot->Xvel * rcossin_tbl[mot->Xa & 0xFFF].unk2) >> 12;
        if (obj->timer != 0) {
            obj->timer--;
        }
        switch (obj->status) {
        case 0:
            hide = 1;
            if ((((ArkenoidBallView *)GameInfo)->unk10 & 0x04000000) ||
                (((ArkenoidBallView *)GameInfo)->unk30 & 0x1000)) {
                mot->Xvel = 0;
                mot->Xacc = 0;
                mot->Xdec = 0;
                eng->unk10.x = 0x2000;
                eng->unk10.y = 0x2000;
                eng->unk10.z = 0x2000;
                eng->flags &= ~0x8000;
                break;
            }
            if ((u16)ArkBallCount < 4) {
                idx = BallSpeedFrames[ArkBallCount][Random(2)];
            } else {
                idx = Random(4);
            }
            if (FloorLinks[idx].unk12 == 0) {
                ArkBallCount = -1;
                eng->unk10 = ((SLONG_XYZ *)ArkBumpers)[idx];
                FloorLinks[idx].unk12 = 0x1F;
                obj->countdown = 0xF;
                obj->status = 12;
            }
            break;
        case 12:
            hide = 1;
            if (obj->countdown != 0) {
                obj->countdown--;
                break;
            }
            e2 = eng->Engine;
            eng->flags |= 0x8000;
            if (e2 != NULL) {
                e2->flags |= 0x8000;
            }
            ang = get_angle(eng->unk10.x, eng->unk10.z);
            mot->Xa = ang;
            mot->Za = ang;
            ang = Random(0x1C7);
            mot->Xacc = 4;
            mot->Zvel = 0;
            mot->Xdec = hide;
            mot->Za = (mot->Za + ((u16)ang - 0xE3)) & 0xFFF;
            obj->timer = 0x14;
            PlaySfx(0x5A0, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            switch (((ArkenoidBallView *)GameInfo)->unk34) {
            case 17:
                mot->Yvel = 0x40;
                mot->Ymax = 0x50;
                mot->Yacc = 6;
                mot->Ydec = 0;
                mot->Ydir = 0;
                mot->Xvel = 0x50;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            case 16:
                mot->Ymax = 0x60;
                mot->Yacc = 3;
                mot->Xvel = 0x20;
                mot->Yvel = 0x10;
                mot->Ydec = 0;
                mot->Ydir = 0;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            case 15:
                eng->unk10.y = World_floor_level - 0x100;
                mot->Ymax = 0x60;
                mot->Ydir = 0xFF;
                mot->Yvel = 0x30;
                mot->Yacc = 0;
                mot->Ydec = 6;
                mot->Xvel = 0x20;
                obj->status = 6;
                mot->Xmax = 0x30;
                break;
            default:
                eng->unk10.y = World_floor_level - 0x100;
                mot->Yvel = 0x30;
                mot->Ymax = 0x60;
                mot->Ydir = 0xFF;
                mot->Yacc = 0;
                mot->Ydec = 6;
                mot->Xvel = 0x20;
                obj->status = 6;
                mot->Xmax = 0x50;
                break;
            }
            obj->player = -1;
            obj->owner = -1;
            break;
        case 6:
            if (((s32 (*)(EXTengine *, EXTobject *))ArkTestLaunchBounds)(eng, obj) != 0) {
                hide = 1;
            } else {
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
                eng->unk10.y -= mot->Yvel;
            } else {
                eng->unk10.y += mot->Yvel;
            }
            if (mot->Yvel == 0) {
                mot->Yacc = mot->Ydec;
                mot->Ydec = 0;
                mot->Ydir = 0;
            }
            if (mot->Ydir != 0) {
                break;
            }
            eng->unk10.y += 0x60;
            if (func_80025BBC(eng) != 0) {
                obj->can_do |= 0x400;
                if (nas->UserInfo.flags == 0) {
                    PlaySfx(BallFramesA[Random(4)], &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
                    nas->UserInfo.flags |= 1;
                }
                if (((ArkenoidBallView *)GameInfo)->unk34 == 0x10) {
                    mot->Yvel = mot->Yvel >> 2;
                } else {
                    mot->Yvel = mot->Yvel >> 1;
                }
                mot->Ydec = mot->Yacc;
                mot->Yacc = 0;
                mot->Ydir = 0xFF;
                if (mot->Yvel < 8) {
                    obj->status = 1;
                    obj->can_do |= 0x400;
                }
            }
            if (obj->can_do & 0x400) {
                d = ((s16 (*)(EXTengine *, s32))ArkBounceOnBounds)(eng, mot->Xa);
                if (d != -1) {
                    k = func_800154A0(((u16)mot->Xa + 0x800) & 0xFFF, d);
                    if (abs(k) < 8) {
                        k = Random(0x10) + 8;
                    }
                    mot->Xa = (d + k) & 0xFFF;
                    mot->Zmax = 0;
                    PlaySfx(BallFramesB[Random(6)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (((s32 (*)(EXTengine *, EXTobject *, s32))ArkTestOutOfBounds)(eng, obj, 0) != 0) {
                    obj->countdown = 4;
                    switch (((ArkenoidBallView *)GameInfo)->unk34) {
                    case 17:
                        obj->status = 0x63;
                        break;
                    case 15:
                    case 16:
                        obj->status = 4;
                        mot->Yvel = 0x80;
                        mot->Ymax = 0x60;
                        mot->Yacc = 8;
                        mot->Ydec = 0;
                        break;
                    default:
                        obj->status = 0x11;
                        break;
                    }
                    i = ArkPlayerSide(obj->player);
                    BLINK_D38[i].unk12 = 4;
                    BLINK_D38[i].unk10 = 0x20;
                    break;
                }
            }
            eng->unk10.y -= 0x60;
            break;
        case 3:
            if (((s32 (*)(EXTengine *, EXTobject *))ArkTestLaunchBounds)(eng, obj) != 0) {
                hide = 1;
            } else {
                mot->Xa = mot->Za;
                mot->Za = 0;
                PlaySfx(BallFramesA[Random(4)], &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
                obj->status = 1;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 1:
            d = ((s16 (*)(EXTengine *, s32))ArkBounceOnBounds)(eng, mot->Xa);
            if (d != -1) {
                k = func_800154A0(((u16)mot->Xa + 0x800) & 0xFFF, d);
                if (abs(k) < 8) {
                    k = Random(0x10) + 8;
                }
                mot->Xa = (d + k) & 0xFFF;
                mot->Zmax = 0;
                PlaySfx(BallFramesB[Random(6)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (((s32 (*)(EXTengine *, EXTobject *, s32))ArkTestOutOfBounds)(eng, obj, 0) != 0) {
                obj->countdown = 8;
                switch (((ArkenoidBallView *)GameInfo)->unk34) {
                case 17:
                    obj->countdown = 0x10;
                    obj->status = 0x63;
                    break;
                case 15:
                case 16:
                    obj->status = 4;
                    mot->Yvel = 0x80;
                    mot->Ymax = 0x60;
                    mot->Yacc = 8;
                    mot->Ydec = 0;
                    break;
                default:
                    obj->status = 0x11;
                    break;
                }
                i = ArkPlayerSide(obj->player);
                BLINK_D38[i].unk12 = 4;
                BLINK_D38[i].unk10 = 0x20;
                break;
            }
            CalcArkInfluence(mot);
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 0x63:
            if (obj->countdown != 0) {
                obj->countdown--;
            } else {
                done = 1;
            }
            if ((((s32 (*)(EXTengine *, EXTobject *, s32))ArkTestOutOfBounds)(eng, obj, 0) ^ 1) != 0) {
                obj->status = 1;
                break;
            }
            mot->Xa = get_angle(-eng->unk10.x, -eng->unk10.z);
            if (((s32 (*)(EXTengine *, EXTobject *, s32))ArkTestOutOfBounds)(eng, obj, 1) != 0 || done != 0) {
                obj->status = 4;
                mot->Yvel = 0x20;
                mot->Ymax = 0x60;
                mot->Yacc = 6;
                mot->Ydec = 0;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 0x11:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->unk76 = 0x400;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->unk76 = 0x400;
                    }
                }
            } else {
                eng->sub->unk76 += 0x200;
                e2 = eng->Engine;
                hide = 1;
                if (e2 != NULL) {
                    e2->sub->unk76 += 0x200;
                    if ((s16)e2->sub->unk76 >= 0x1000) {
                        e2->sub->unk76 = 0xFFF;
                        done = hide;
                    }
                }
            }
            if (((s32 (*)(EXTengine *, EXTobject *, s32))ArkTestOutOfBounds)(eng, obj, 1) != 0 || done != 0) {
                obj->status = 0x12;
            }
            eng->unk10.y = eng->unk10.y - ((s32 (*)(EXTengine *))func_80025F44)(eng) - 0x60;
            break;
        case 4:
            if (obj->countdown != 0) {
                if (--obj->countdown == 0) {
                    eng->flags |= 0x40000021;
                    eng->sub->flags |= 0x40000021;
                    eng->sub->unk76 = 0x400;
                    eng->sub->unk68 = 0x100;
                    e2 = eng->Engine;
                    if (e2 != NULL) {
                        e2->flags |= 0x40000000;
                        e2->sub->flags |= 0x40000000;
                        e2->sub->unk76 = 0x400;
                    }
                }
            } else {
                eng->sub->unk76 += 0x200;
                e2 = eng->Engine;
                hide = 1;
                if (e2 != NULL) {
                    e2->sub->unk76 += 0x200;
                    if ((s16)e2->sub->unk76 >= 0x1000) {
                        e2->sub->unk76 = 0xFFF;
                        done = hide;
                    }
                }
            }
            if (((ArkenoidBallView *)GameInfo)->unk34 != 17) {
                mot->Xvel >>= 1;
                mot->Xmax >>= 1;
            }
            mot->Yvel += mot->Yacc - mot->Ydec;
            if (mot->Yvel < 0) {
                mot->Yvel = 0;
            }
            if (mot->Yvel > mot->Ymax) {
                mot->Yvel = mot->Ymax;
            }
            yv = mot->Yvel;
            eng->unk10.y += yv;
            if (((s32 (*)(EXTengine *, EXTobject *, s32, s32))ArkTestOutOfBounds)(eng, obj, 1, yv) != 0 ||
                done != 0) {
                obj->status = 0x12;
            }
            break;
        case 2:
            obj->can_do |= 0x80;
            if ((u16)obj->player >= 4) {
                mot->Xvel = 0x30;
                mot->Xmax = 0x30;
                obj->status = 1;
                break;
            }
            AlignGrabBall(eng, mot, obj);
            break;
        case 0x12:
            if (obj->whatami != 2) {
                obj->can_do &= 0xFF7F;
                pl = obj->player;
                if (!(Heroes[pl].hero_status & 0xFA00)) {
                    PlaySfx(BallFramesC[pl], NULL, NULL, 0x1000, 0x1E00, 0);
                    pl = obj->player;
                    if (pl != -1) {
                        ArkBallCount = ArkPlayerSide(pl);
                    }
                    if ((((ArkenoidBallView *)GameInfo)->unk4 & 0x80000001) != 0x80000001) {
                        if ((((ArkenoidBallView *)GameInfo)->unk32 & 0x40) &&
                            ((ArkenoidBallView *)GameInfo)->unk6C != 0) {
                            if (((ArkenoidBallView *)GameInfo)->unk5C[obj->player] != 0) {
                                obj->player = ((ArkenoidBallView *)GameInfo)->unk4A;
                            } else {
                                obj->player = ((ArkenoidBallView *)GameInfo)->unk48;
                            }
                            if (Heroes[obj->player].hero_score != 0) {
                                Heroes[obj->player].hero_score--;
                                Heroes[obj->player].hero_score_displayed = Heroes[obj->player].hero_score;
                            }
                        } else if (Heroes[obj->player].hero_score != 0) {
                            Heroes[obj->player].hero_score--;
                            Heroes[obj->player].hero_health--;
                            Heroes[obj->player].hero_score_displayed = Heroes[obj->player].hero_score;
                        }
                        if ((((ArkenoidBallView *)GameInfo)->unk4 & 0x80000002) != 0x80000002 &&
                            Heroes[obj->player].hero_score == 0) {
                            if (obj->player == ArkBallTimer) {
                                ArkBallTimer = -1;
                            }
                            Heroes[obj->player].hero_status |= 0x2000;
                            if (((ArkenoidBallView *)GameInfo)->unk32 & 0x40) {
                                pl = Heroes[obj->player].hero_teammate;
                                if (pl != -1) {
                                    Heroes[pl].hero_status |= 0x2000;
                                }
                            }
                        }
                    }
                }
            }
            KillNAS(nas);
            return;
        }
        mot->Zvel = 0;
    }
    dx = eng->unk10.x - eng->OldXYZ.x;
    dz = eng->unk10.z - eng->OldXYZ.z;
    eng->unk1E += (((get_angle(dx, dz) & 0xFFF) - eng->unk1E) << 20) >> 21;
    dist = csqrt(dx * dx + dz * dz) >> 5;
    eng->unk5E = 1;
    dx = ((eng->unk10.x + 0x800) >> 9) + 1;
    dz = ((eng->unk10.z + 0x800) >> 9) + 1;
    eng->unk1C -= dist;
    if (dx >= 10) {
        dx = 9;
    }
    if (dx < 0) {
        dx = 0;
    }
    if (dz >= 10) {
        dz = 9;
    }
    if (dz < 0) {
        dz = 0;
    }
    shade = BallSpeedTable[9 - dz][dx];
    switch (obj->status) {
    case 4:
    case 0x12:
        shade = 0x200;
        break;
    case 0x63:
    case 2:
        break;
    default:
        TestArkObjectObstacles(eng, obj);
        break;
    }
    eng->sub->unk68 = shade;
    e2 = eng->Engine;
    if (e2 == NULL) {
        return;
    }
    e2->unk10 = eng->unk10;
    e2->sub->unk68 = shade;
    /* the level's GetContour hook */
    y = eng->unk10.y - LevelSpecs.GetContour(eng);
    e2 = e2->Engine;
    if (e2 == NULL) {
        return;
    }
    e2->unk10 = eng->unk10;
    e2->unk10.y = y;
    e2->sub->unk68 = shade;
    sub = e2->sub;
    if (hide != 0) {
        e2->flags &= ~0x8000;
    } else {
        e2->flags |= 0x8000;
    }
    sub->unk08 = y - eng->unk10.y;
    trail = FindExtension(nas, 0x10000000);
    if (trail == NULL) {
        return;
    }
    k = 0;
    sub = sub->next;
    sc = 0x1000;
    while (sub != NULL) {
        if (trail->on[k] != 0) {
            sub->flags |= 0x8000;
        } else {
            sub->flags &= ~0x8000;
        }
        sub->unk04 = trail->pos[k].x - e2->unk10.x;
        sub->unk08 = trail->pos[k].y - (e2->unk10.y + e2->sub->unk08);
        sub->unk0C = trail->pos[k].z - e2->unk10.z;
        sub->unk20.z = sc;
        sub->unk20.x = sc;
        sc -= 0x180;
        sub->unk52 = 1;
        if (sc < 0x100) {
            sc = 0x100;
        }
        sub->unk76 = 0x1000 - (sc >> 1);
        sub = sub->next;
        k++;
    }
    for (k = 7; k > 0; k--) {
        trail->pos[k].x = trail->pos[k - 1].x;
        trail->pos[k].y = trail->pos[k - 1].y;
        trail->pos[k].z = trail->pos[k - 1].z;
        trail->on[k] = trail->on[k - 1];
    }
    if (hide == 0 && (nas->UserInfo.flags & 1)) {
        trail->on[0] = 1;
        trail->pos[0] = e2->unk10;
    }
}

void CalcArkInfluence(EXTmotion *m) {
    s32 sx;
    s32 sz;
    s32 ax;
    s32 az;
    s32 speed;
    s32 angle;
    s32 t;
    s32 x1;
    s32 z1;
    s16 vel;
    s32 scale;

    if (GameMode == 0x11) {
        sx = abs((rcossin_tbl[(ArkFloorTiltX - 0x400) & 0xFFF].unk2 * 5) >> 3);
        sz = abs((rcossin_tbl[(ArkFloorTiltZ - 0x400) & 0xFFF].unk2 * 5) >> 3);
        az = 0;
        ax = (ArkFloorTiltX < 0) ? 0x800 : 0;
        if (ArkFloorTiltX > 0) {
            ax = 0;
        }
        if (ArkFloorTiltZ < 0) {
            az = 0x400;
        }
        if (ArkFloorTiltZ > 0) {
            az = 0xC00;
        }
        scale = (s16)sx;
        x1 = (scale * rcossin_tbl[(ax - 0x400) & 0xFFF].unk2) >> 12;
        z1 = (scale * rcossin_tbl[ax].unk2) >> 12;
        scale = (s16)sz;
        t = (scale * rcossin_tbl[(az - 0x400) & 0xFFF].unk2) >> 12;
        x1 += t;
        t = (scale * rcossin_tbl[az].unk2) >> 12;
        angle = get_angle(x1, z1 + t);
        m->Zvel = (sx + sz) >> 6;
        m->Za = angle;
    }
    speed = m->Zvel;
    if (speed != 0) {
        vel = m->Xvel;
        x1 = (vel * rcossin_tbl[((u16)m->Xa - 0x400) & 0xFFF].unk2) >> 12;
        z1 = (vel * rcossin_tbl[(u16)m->Xa & 0xFFF].unk2) >> 12;
        t = (speed * rcossin_tbl[((u16)m->Za - 0x400) & 0xFFF].unk2) >> 12;
        x1 += t;
        t = (speed * rcossin_tbl[(u16)m->Za & 0xFFF].unk2) >> 12;
        m->Xa = get_angle(x1, z1 + t) & 0xFFF;
    }
}

void FreeGrabbedObject(u32 arg0, bool arg1) {
    VECTOR sp18;
    HeroInfo *hs;
    HeroInfo *h;
    EXTengine *hero;
    EXTmotion *mot;
    EXTengine *eng;
    NAS *nas;
    EXTobject *obj;
    s32 dir;
    s32 a;
    s32 spd;

    hs = Heroes;
    h = &hs[arg0];
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    hero = h->hero_eng;
    if (hero != NULL) {
        nas = HeadOfSolids;
        if (nas != NULL) {
            do {
                eng = nas->Engine;
                if (eng != NULL) {
                    if (eng != hero) {
                        obj = FindExtension(nas, 0x1000);
                        if (obj != NULL && obj->status == 2 && arg0 == obj->player) {
                            mot = FindExtension(nas, 0x800);
                            if (mot != NULL) {
                                spd = 0x30;
                                if (arg1) {
                                    PlaySfx(ArkGrabTimes[arg0 + 8], NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                                    spd = 0xE0;
                                }
                                mot->Xvel = spd;
                                mot->Xmax = spd;
                                if (func_80015780(hero, hero->sub, 4, &sp18) != NULL) {
                                    dir = get_angle(-eng->unk10.x + sp18.vx, -eng->unk10.z + sp18.vz);
                                } else {
                                    dir = get_angle(-eng->unk10.x + hero->unk10.x, -eng->unk10.z + hero->unk10.z);
                                }
                                a = func_800154A0(hero->unk1E, dir);
                                if (a > 0x220) {
                                    a = 0x220;
                                }
                                if (a < -0x220) {
                                    a = -0x220;
                                }
                                a += hero->unk1E;
                                a &= 0xFFF;
                                *(volatile s16 *)&mot->Xa = a;
                                eng->unk10.x -= mot->Xvel * rcossin_tbl[(mot->Xa - 0x400) & 0xFFF].unk2 >> 12;
                                eng->unk10.z -= mot->Xvel * rcossin_tbl[(u16)mot->Xa & 0xFFF].unk2 >> 12;
                                obj->status = 1;
                            }
                        }
                    }
                    nas = nas->Solid.Child;
                }
            } while (nas != NULL);
        }
    }
}

void AlignGrabBall(EXTengine *arg0, EXTmotion *arg1, EXTobject *arg2) {
    SLONG_XYZ v;
    VECTOR target;
    HeroInfo *hero;
    EXTengine *o;
    NAS *n;
    void *t;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 rr;
    HeroInfo *base;
    s32 a1;
    s32 a2;
    s32 d;

    base = Heroes;
    hero = &base[arg2->player];
    if (func_80015780(hero->hero_eng, hero->hero_sub, 4, &target) == NULL) {
        *(SLONG_XYZ *)&target = hero->hero_eng->unk10;
    }
    for (i = 0; i < 4; i++) {
        if (arg0->collision_box.flags & 0x40) {
            r = ArkPosts[i].r + arg0->collision_box.radius;
        } else {
            r = ArkPosts[i].r + 0x80;
        }
        dx = arg0->unk10.x - ArkPosts[i].x;
        dz = arg0->unk10.z - ArkPosts[i].z;
        if (dx * dx + dz * dz < r * r) {
            a1 = get_angle(-dx, -dz);
            v.x = ArkPosts[i].x;
            v.y = 0;
            v.z = ArkPosts[i].z;
            LineToCircleTest(&arg0->unk10, a1, &v, r);
            dx = -arg0->unk10.x;
            arg1->Xa = get_angle(dx + target.vx, (dz = -arg0->unk10.z) + target.vz);
        }
    }
    r = 0x80;
    if (arg0->collision_box.flags & 0x40) {
        r = arg0->collision_box.radius;
    }
    r *= 2;
    rr = r * r;
    for (n = HeadOfSolids; n != NULL; n = n->Solid.Child) {
        o = n->Engine;
        if (o != NULL && o != arg0) {
            t = FindExtension(n, 0x1000);
            if (t != NULL && *(u16 *)((u8 *)t + 0x1A) == 2) {
                dx = o->unk10.x - arg0->unk10.x;
                dz = o->unk10.z - arg0->unk10.z;
                if (dx * dx + dz * dz < rr) {
                    a1 = get_angle(target.vx - arg0->unk10.x, target.vz - arg0->unk10.z);
                    dx = -o->unk10.x;
                    a2 = get_angle(dx + target.vx, (dz = -o->unk10.z) + target.vz);
                    d = (0xE3 - func_80015480(a1, a2)) >> 1;
                    if (func_800154A0(a1, a2) < 0) {
                        a1 += d;
                        a2 -= d;
                    } else {
                        a2 += d;
                        a1 -= d;
                    }
                    arg1->Xa = a1;
                    t = FindExtension(n, 0x800);
                    if (t != NULL) {
                        *(s16 *)((u8 *)t + 0x16) = a2;
                    }
                }
            }
        }
    }
}

void Plot_N_Gin(NAS *arg0) {
    EXTengine *eng;
    ArkenoidGin *gin;
    s32 i;
    u32 s;

    eng = arg0->Engine;
    if (eng == NULL) {
        return;
    }
    if (FindExtension(arg0, 0x400) != NULL && (gin = ArkNGin) != NULL) {
        if (GameExitFlags & 0x801) {
            if ((u16)gin->unk1A != 0) {
                gin->unk1A = 5;
                if (eng->sfx != NULL) {
                    StopSfx(eng->sfx);
                }
            }
            func_800250B0(arg0);
        }
        s = (u16)gin->unk1A;
        switch (s) {
        case 1:
            eng->unk10.y = World_floor_level;
            gin->unk1A = 2;
            gin->unk22 = 0xA;
            gin->unk1C |= 0x90;
            PlaySfx(0x5BE, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8007A524(0, &eng->unk10.x, (s32)NginModel);
            eng->sub->unk74 = 0x2001;
            break;
        case 2:
            eng->unk1E += 0x20;
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                arg0->Class |= 0x200;
                eng->flags |= 0x8000;
                gin->unk1A = 3;
                PlaySfx(0x5BF, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
                gin->unk22 = 0x5A;
                gin->unk20 = 0x10;
            }
            func_800250B0(arg0);
            eng->unk5E = 1;
            return;
        case 3:
            eng->unk1E += gin->unk20;
            gin->unk20 += 4;
            func_800250B0(arg0);
            if (gin->unk22 != 0) {
                gin->unk22--;
                break;
            }
            eng->sub->unk74 = 0x2006;
            gin->unk1A = 4;
            gin->unk22 = 0x96;
            gin->unk20 = Random(0xA) + 0xA;
            if (((ArkenoidGinView *)GameInfo)->unk32 == 0) {
                eng->unk5E = 1;
                return;
            }
            if (((ArkenoidGinView *)GameInfo)->unk6C != 0) {
                gin->unk22 = 0xD2;
            }
            break;
        case 4:
            eng->unk1E += 0x1C7;
            if (((ArkenoidGinView *)GameInfo)->unk32 & 0x80) {
                if (gin->unk20 != 0) {
                    gin->unk20--;
                } else {
                    for (i = 0; i < 4; i++) {
                        if ((i == 0 || (((ArkenoidGinView *)GameInfo)->unk6C != 0 && i == 1)) && !(Heroes[i].hero_status & 0xFA00)) {
                            if (func_80015480(eng->unk1E, Heroes[i].hero_move->facing_angle & 0xFFF) < 0xE3) {
                                ArkSpawnBall();
                                gin->unk20 = Random(0xB) + 6;
                            } else {
                                gin->unk20 = Random(5);
                            }
                        }
                    }
                }
            } else {
                if (gin->unk20 != 0) {
                    gin->unk20--;
                } else {
                    ArkSpawnBall();
                    gin->unk20 = 0xF;
                }
            }
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                gin->unk1A = 5;
            }
            break;
        case 5:
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5C0, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8007A524(0, &eng->unk10.x, (s32)NginModel);
            gin->unk1C = 0;
            eng->unk10.y = -0x1000;
            eng->flags &= 0xFFFF7FFF;
            NGinState = 0x19;
            arg0->Class &= 0xFDFF;
            gin->unk1A = 0;
            gin->unk22 = (Random(0xF) + 8) * 0x1E;
            break;
        case 0:
        default:
            gin->unk1C = 0;
            eng->unk10.y = -0x1000;
            arg0->Class &= 0xFDFF;
            eng->flags &= 0xFFFF7FFF;
            if (((ArkenoidModeView *)GameInfo)->unk30 & 0x801) {
                eng->unk5E = 1;
                return;
            }
            if (gin->unk22 != 0) {
                gin->unk22--;
            } else {
                gin->unk1A = 1;
            }
        }
    }
end:
    eng->unk5E = 1;
}

void ArkCrateMaster(void) {
    ArkenoidGameView *g;
    ArkenoidNode *n;
    NAS *nas;
    s32 gin;
    s32 cnt;
    s32 i;
    s32 lim;
    s32 x;
    s32 y;
    s32 hero;
    s32 total;

    g = (ArkenoidGameView *)GameInfo;
    if (g->flags & 0x801) {
        return;
    }
    gin = 0;
    if (ArkNGin != NULL) {
        gin = (u16)ArkNGin->unk1A != 0;
    }
    if (g->mode == 0x10 && D_8009E25C.n == 0) {
        if (CrateMasterWordC != 0) {
            CrateMasterWordC--;
        } else {
            for (i = cnt = 0; i < 4; i++) {
                if (Heroes[i].hero_pu != 0) {
                    cnt++;
                }
            }
            if (cnt == 0) {
                Create_ArkPickup();
                CrateMasterWordC = Random(0x5A) + 0x1E;
            }
        }
    }
    if (GameMode == 0xE && Random(0xF) == 0) {
        x = Random(0x1800) - 0xC00;
        y = -(Random(0x200) + 0x80);
        CreateNAS(x, y, Random(0x800) + 0x1000, 0x1504, 0x11);
    }
    if (NGinState != 0) {
        NGinState--;
    } else {
        CrateMasterWordA++;
        lim = (u16)(CrateMasterWordA / 300);
        lim++;
        if (GameMode == 0xF) {
            if (lim >= 5) {
                lim = 4;
            }
        } else if (lim >= 6) {
            lim = 5;
        }
        if (gin) {
            lim = 0;
        }
        n = HeadOfBalls;
        CrateMasterWordB = 0;
        while (n != NULL) {
            n = n->next;
            CrateMasterWordB++;
        }
        if (CrateMasterWordB < lim) {
            total = 0;
            for (n = HeadOfBalls; n != NULL; n = n->next) {
                total++;
            }
            if (total < 9) {
                nas = CreateNAS(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
                if (nas != NULL && nas->Engine != NULL && nas->FUX != NULL) {
                    ((ArkenoidFux *)nas->FUX)->flag = 0;
                }
            }
            NGinState = 0x1E;
        }
    }
    ArkBotSkill = CrateMasterTimes[CrateMasterWordB];
    if (O61GV->mode == 0xF && (O61GV->pad_buttons & 0x80)) {
        if (CrateMasterState != 0) {
            CrateMasterState--;
        } else {
            if (++CrateMasterTimer >= 0x12) {
                CrateMasterTimer = 0;
            }
            if (O61GV->unk6C != 0) {
                hero = CrateMasterSpotsZ[CrateMasterTimer];
            } else {
                hero = CrateMasterSpotsX[CrateMasterTimer];
            }
            if (!(Heroes[hero].hero_status & 0xFA00)) {
                CreateNAS(hero, 0, 0, 0x1509, 0x11);
            }
            CrateMasterState = Random(6) * 0x1E + 0x5A;
        }
    }
    if (O61GV->mode == 0x11) {
        if (ArkRedKickTimer != 0) {
            ArkRedKickTimer--;
        }
        if (CrateMasterCount != 0) {
            CrateMasterCount--;
        } else {
            if (++CrateMasterNext >= 0x12) {
                CrateMasterNext = 0;
            }
            if (O61GV->unk6C != 0) {
                i = CrateMasterSpotsZ[CrateMasterNext];
            } else {
                i = CrateMasterSpotsX[CrateMasterNext];
            }
            SetCornerFailure((u16)i);
            CrateMasterCount = Random(9) * 0x1E + 0x10E;
        }
    }
}

#define LINK(x) ((ArkenoidLink *)(x))

void InitARKfloor(void) {
    s32 i;
    ArkenoidScnObj *obj;
    s16 *p;
    s32 t;
    s32 t60;
    u32 k;

    for (i = 9; i >= 0; i--) {
        FloorObjectsA[i] = 0;
    }
    for (i = 9; i >= 0; i--) {
        FloorObjectsB[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        FloorLinks[i].a = 0;
        FloorLinks[i].unkA = 0;
        FloorLinks[i].unk12 = 0;
        FloorLinks[i].unk10 = 0;
        LINK(ArkGoals)[i].a = 0;
        LINK(ArkGoals)[i].b = 0;
        LINK(ArkGoals)[i].unkE = 0;
        LINK(ArkGoals)[i].unk8 = 0;
        LINK(ArkGoals)[i].unkE = 0;
        LINK(ArkGoals)[i].unk8 = 0;
        LINK(ArkGoals)[i].unk12 = 0;
        LINK(ArkDoorsA)[i].a = 0;
        LINK(ArkDoorsA)[i].b = 0;
        LINK(ArkDoorsA)[i].unkE = 0;
        LINK(ArkDoorsA)[i].unk8 = 0;
        LINK(ArkDoorsA)[i].unkE = 0;
        LINK(ArkDoorsA)[i].unk8 = 0;
        LINK(ArkDoorsA)[i].unk12 = 0;
        LINK(ArkDoorsB)[i].a = 0;
        LINK(ArkDoorsB)[i].b = 0;
        LINK(ArkDoorsB)[i].unkE = 0;
        LINK(ArkDoorsB)[i].unk8 = 0;
        LINK(ArkDoorsB)[i].unkE = 0;
        LINK(ArkDoorsB)[i].unk8 = 0;
        LINK(ArkDoorsB)[i].unk12 = 0;
        LINK(ArkWallObjects)[i].a = 0;
        LINK(ArkWallObjects)[i].b = 0;
        LINK(ArkWallObjects)[i].unkE = 0;
        LINK(ArkWallObjects)[i].unk8 = 0;
        LINK(ArkWallObjects)[i].unkE = 0;
        LINK(ArkWallObjects)[i].unk8 = 0;
        LINK(ArkWallObjects)[i].unk12 = 0;
        ArkWalls[i].pad0 = 0;
        ArkWalls[i].f4 = 0;
        ArkWalls[i].f8 = 0x60;
        ArkWalls[i].fC = 0;
        ArkWalls[i].f10 = 0;
        ArkWalls[i].f14 = 0;
        ArkWalls[i].f18 = 0;
        ArkWalls[i].f1A = 0;
    }
    for (obj = (ArkenoidScnObj *)((GfxScene *)MapScene)->objs; obj != 0; obj = obj->next) {
        p = (s16 *)func_80015870(obj->unk6C, obj->unk74);
        k = *p;
        if (k - 1 < 9) {
            FloorObjectsA[k] = obj;
        }
        k = *p - 10;
        if (k < 4) {
            FloorLinks[k].a = obj;
            FloorLinks[k].unkA = obj->unk74;
            obj->unk68 = 0x44;
            obj->flags |= 0x2000000;
        }
        k = *p - 20;
        if (k < 4) {
            LINK(ArkGoals)[k].a = obj;
            LINK(ArkGoals)[k].unk8 = obj->unk74;
            switch (((ArkenoidModeView *)GameInfo)->unk34) {
            case 14:
                LINK(ArkGoals)[k].unkA = FloorFramesA[k];
                break;
            case 16:
                LINK(ArkGoals)[k].unkA = FloorFramesC[k];
                break;
            }
        }
        t = *p;
        k = t - 30;
        if (k < 4) {
            LINK(ArkGoals)[k].b = obj;
            LINK(ArkGoals)[k].unkC = obj->unk74;
            switch (((ArkenoidModeView *)GameInfo)->unk34) {
            case 14:
                LINK(ArkGoals)[k].unkE = FloorFramesA[t - 26];
                break;
            case 16:
                LINK(ArkGoals)[k].unkE = FloorFramesC[t - 26];
                break;
            }
        }
        k = *p - 50;
        if (k < 4) {
            LINK(ArkDoorsA)[k].a = obj;
            LINK(ArkDoorsA)[k].unk8 = obj->unk74;
            LINK(ArkDoorsA)[k].unkA = FloorFramesB[k];
        }
        t60 = *p;
        k = t60 - 60;
        if (k < 4) {
            LINK(ArkDoorsA)[k].b = obj;
            LINK(ArkDoorsA)[k].unkC = obj->unk74;
            LINK(ArkDoorsA)[k].unkE = FloorFramesB[t60 - 56];
        }
        k = *p - 80;
        if (k < 4) {
            LINK(ArkWallObjects)[k].a = obj;
            LINK(ArkWallObjects)[k].unkA = obj->unk74;
            LINK(ArkWallObjects)[k].unkE = FloorLayout[k][0];
        }
        k = *p - 90;
        if (k < 4) {
            LINK(ArkDoorsB)[k].a = obj;
            LINK(ArkDoorsB)[k].unkA = obj->unk74;
            obj->flags &= ~0x8000;
        }
        k = *p - 100;
        if (k < 10) {
            FloorObjectsB[k] = obj;
        }
    }
}

void UpdateArkFloor(void)
{
    s32 a;
    s32 i;
    s32 j;
    ArkenoidScnObj *o;
    ArkenoidScnObj *p;
    ArkenoidSpr **sp;
    ArkenoidSpr *s;

    a = (GameFrameCount << 5) & 0xFFF;
    if (GameMode == 0x11) {
        ArkFloorTiltZ = ((rcossin_tbl[(a * 2 - 0x400) & 0xFFF].unk2 + rcossin_tbl[a].unk2) * 11) >> 12;
        ArkFloorTiltX = (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 5) >> 10;
        World_floor_level = (rcossin_tbl[a].unk2 * 3) >> 7;
    }
    i = 0;
    do {
        if (ArkWalls[i].pad0 != 0) {
            a = get_angle(0xA00, ArkWalls[i].pad0 >> 4) - 0x400;
            switch (i) {
            case 1:
                ArkFloorTiltZ += a;
                ArkFloorTiltX += a;
                break;
            case 2:
                ArkFloorTiltZ -= a;
                ArkFloorTiltX += a;
                break;
            case 3:
                ArkFloorTiltZ -= a;
                ArkFloorTiltX -= a;
                break;
            default:
                ArkFloorTiltZ += a;
                ArkFloorTiltX -= a;
                break;
            }
        }
        i++;
    } while (i < 4);

    for (j = 0; j < 4; j++) {
        if (FloorLinks[j].unk12 != 0) {
            FloorLinks[j].unk12--;
        }
        if (BLINK_E28[j].unk12 != 0) {
            BLINK_E28[j].unk12--;
        }
        if (BLINK_D38[j].unk12 != 0) {
            BLINK_D38[j].unk12--;
        }
        if ((p = FloorLinks[j].a) != NULL && ((ArkenoidModeView *)GameInfo)->unk34 == 14) {
            if (FloorLinks[j].unk12 != 0) {
                FloorLinks[j].unk10++;
                p->unk74 = FloorLinks[j].unkA + (u16)(((FloorLinks[j].unk10 >> 2) & 3) + 1);
            } else {
                p->unk74 = FloorLinks[j].unkA;
                FloorLinks[j].unk10 = 0;
            }
        }
        if ((p = BLINK_E28[j].a) != NULL) {
            switch (((ArkenoidModeView *)GameInfo)->unk34) {
            case 14:
                if (BLINK_E28[j].unk12 != 0) {
                    p->unk74 = BLINK_E28[j].unkA;
                    if (BLINK_E28[j].b != NULL) {
                        BLINK_E28[j].b->unk74 = BLINK_E28[j].unkE;
                    }
                } else {
                    p->unk74 = BLINK_E28[j].unk8;
                    if (BLINK_E28[j].b != NULL) {
                        BLINK_E28[j].b->unk74 = BLINK_E28[j].unkC;
                    }
                }
                break;
            case 16:
                if (BLINK_E28[j].unk10 == 1) {
                    p->unk74 = BLINK_E28[j].unkA;
                    BLINK_E28[j].b->unk74 = BLINK_E28[j].unkE;
                } else {
                    p->unk74 = BLINK_E28[j].unk8;
                    BLINK_E28[j].b->unk74 = BLINK_E28[j].unkC;
                }
                break;
            }
        }
        if ((o = BLINK_D38[j].a) != NULL && BLINK_D38[j].unk12 == 0) {
            if (BLINK_D38[j].unk10 != 0 && (--BLINK_D38[j].unk10 & 4)) {
                o->unk74 = BLINK_D38[j].unkA;
            } else {
                BLINK_D38[j].a->unk74 = BLINK_D38[j].unk8;
            }
        }
    }

    if (GameMode == 0x11) {
        if (ArkFloorState == 0) {
            ArkWallFlags = 0;
        }
        for (i = 1; i < 10; i++) {
            sp = (ArkenoidSpr **)&FloorObjectsA[i];
            if (*sp != NULL) {
                (*sp)->unk51 = 2;
                (*sp)->unk10 = ArkFloorTiltX;
                (*sp)->unk14 = ArkFloorTiltZ;
                (*sp)->unk8 = World_floor_level;
                (*sp)->unk52 = 1;
            }
        }
        for (i = 0; i < 10; i++) {
            if (FloorObjectsB[i] != NULL) {
                ((ArkenoidRise *)FloorObjectsB[i])->unk4 += FloorRiseCfg[i].step;
                if (FloorRiseCfg[i].max < ((ArkenoidRise *)FloorObjectsB[i])->unk4) {
                    ((ArkenoidRise *)FloorObjectsB[i])->unk4 = FloorRiseCfg[i].reset;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            s = (ArkenoidSpr *)ArkDoorsB[i].flags;
            if (s != NULL) {
                if (ArkFloorState == 0) {
                    s->flags &= ~0x8000;
                }
                ((ArkenoidSpr *)ArkDoorsB[i].flags)->unk51 = 2;
                ((ArkenoidSpr *)ArkDoorsB[i].flags)->unk10 = ArkFloorTiltX;
                ((ArkenoidSpr *)ArkDoorsB[i].flags)->unk14 = ArkFloorTiltZ;
                ((ArkenoidSpr *)ArkDoorsB[i].flags)->unk8 = World_floor_level;
                ((ArkenoidSpr *)ArkDoorsB[i].flags)->unk52 = 1;
            }
            if (ArkWallObjects[i].sub != NULL) {
                ((ArkenoidSpr *)ArkWallObjects[i].sub)->unk51 = 2;
                ((ArkenoidSpr *)ArkWallObjects[i].sub)->unk10 = ArkFloorTiltX;
                ((ArkenoidSpr *)ArkWallObjects[i].sub)->unk14 = ArkFloorTiltZ;
                ((ArkenoidSpr *)ArkWallObjects[i].sub)->unk8 = World_floor_level;
                ((ArkenoidSpr *)ArkWallObjects[i].sub)->unk52 = 1;
            }
        }
    }
}

void Init_ArkDeadWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    s32 dir;
    s32 off;

    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->unk10.x;
        eng->flags &= 0xFFFF7FFF;
        if (dir == ArkSidePlayer3) {
            eng->unk10.x = -0x600;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x38E;
            eng->unk1E = 0;
            nas->UserInfo.flags = 2;
            ArkWallFlags |= 2;
        }
        if (dir == ArkSidePlayer0) {
            eng->unk10.x = -0x600;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            nas->UserInfo.flags = 1;
            ArkWallFlags |= 1;
        }
        if (dir == ArkSidePlayer2) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 4;
            ArkWallFlags |= 4;
        }
        if (dir == ArkSidePlayer1) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 8;
            ArkWallFlags |= 8;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        if (ArkDeadWalls == 0) {
            PlaySfx(0x591, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        }
        ArkDeadWalls = 1;
        func_80018F3C(eng, 5);
        sub = eng->sub;
        off = 0;
        if (sub != 0) {
            do {
                sub->unk6C = BallsEffectsModel;
                sub->unk74 = 0;
                sub->unk04 = off;
                sub->flags |= 0x21;
                sub = sub->next;
                off += 0x200;
            } while (sub != 0);
        }
    }
    nas->UserInfo.user1 = 4;
    nas->UserInfo.user2 = 0x1E;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_NGArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;
    s32 x;
    s32 off;

    eng = arg0->Engine;
    if (eng != NULL) {
        eng->flags &= 0xFFFF7FFF;
        x = eng->unk10.x;
        if (x == ArkSidePlayer3) {
            eng->unk10.x = -0x700;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x38E;
            eng->unk1E = 0;
            arg0->UserInfo.flags = 2;
            ArkWallFlags |= 2;
        }
        if (x == ArkSidePlayer0) {
            eng->unk10.x = -0x700;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            arg0->UserInfo.flags = 1;
            ArkWallFlags |= 1;
        }
        if (x == ArkSidePlayer2) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x700;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            arg0->UserInfo.flags = 4;
            ArkWallFlags |= 4;
        }
        if (x == ArkSidePlayer1) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x700;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            arg0->UserInfo.flags = 8;
            ArkWallFlags |= 8;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        if (ArkDeadWalls == 0) {
            PlaySfx(0x591, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
        }
        ArkDeadWalls = 1;
        func_80018F3C(eng, 6);
        sub = eng->sub;
        off = 0;
        if (sub != NULL) {
            do {
                sub->unk6C = BallsEffectsModel;
                sub->unk74 = 0;
                sub->unk04 = off;
                sub->flags |= 0x21;
                sub = sub->next;
                off += 0x200;
            } while (sub != NULL);
        }
    }
    arg0->UserInfo.user1 = 4;
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void SetCornerFailure(u16 arg0) {
    ArkenoidWall *base;
    ArkenoidWall *w;
    s32 r = Random(0x1E);

    base = ArkWalls;
    w = &base[arg0];
    w->f1A = r + 0x3C;
    w->f18 = 1;
    w->f10 = 0xA0;
    w->f8 = 0x300;
    w->f4 = 0;
    w->fC = 0;
    w->f14 = -1;
    PlaySfx(0x5C9, &ArkCornerEngine.sfx, NULL, 0x1000, 0x1E00, 0);
    PlaySfx(CornerFailFrames[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
}

void HandleCornerFailure(void) {
    ArkenoidWall *base;
    ArkenoidWall *w;
    ArkenoidWallObj *o;
    s32 t;
    s32 lim;
    ArkenoidWallObj *ob;
    s32 two;
    s32 three;
    s32 neg;
    s32 s;
    s32 phase;

    two = 2;
    three = 3;
    neg = -1;
    base = ArkWalls;
    ob = ArkWallObjects;
    w = base;
    phase = (GameFrameCount >> 1) & 7;
    o = ob;
loop:
    {
        if (w->f1A != 0) {
            w->f1A = w->f1A - 1;
        }
        s = w->f18;
        if (s == two) {
            goto c2;
        }
        if (s < 3) {
            if (s == 1) {
                goto c1;
            }
            goto end;
        }
        if (s == three) {
            goto c3;
        }
        goto end;
        {
        c1:
            if (o->sub != NULL) {
                o->sub->unk74 = o->unkE + phase;
            }
            if (w->f1A == 0) {
                do { do { w->f18 = two; } while (0); } while (0);
            }
            if (w->f14 == neg) {
                if (w->pad0 < -0x1500) {
                    w->f14 = 0;
                    w->fC = 0x60;
                    w->f10 = 0;
                }
            } else {
                if (w->f4 > 0) {
                    w->fC = 0x10;
                }
                if (w->pad0 >= -0x12FF) {
                    do { w->f14 = neg; } while (0);
                    w->fC = 0;
                    w->f10 = 0x40;
                }
            }
            goto end;
        c2:
            w->f8 = 0x300;
            w->f14 = 0;
            w->fC = 4;
            w->f10 = 0;
            do { do { w->f18 = three; } while (0); } while (0);
            if (o->sub != NULL) {
                o->sub->unk74 = o->unkA;
            }
            goto end;
        c3:
            if (w->f14 == 0) {
                w->fC += 2;
                if (w->pad0 > 0) {
                    w->f10 = w->fC;
                    w->fC = 0;
                    w->f14 = neg;
                    w->f8 = w->f8 >> 1;
                }
            } else {
                w->f10 += 2;
                if (w->pad0 < 0) {
                    if (w->f8 > 0x40) {
                        w->fC = w->f10;
                        w->f10 = 0;
                        w->f14 = 0;
                        w->f8 = w->f8 >> 1;
                    } else {
                        w->pad0 = 0;
                        w->f4 = 0;
                        w->fC = 0;
                        w->f10 = 0;
                        w->f18 = 0;
                    }
                }
            }
        }
    end:
        w->f4 += w->fC;
        w->f4 -= w->f10;
        lim = w->f8;
        if (lim < w->f4) {
            w->f4 = lim;
        }
        t = -w->f8;
        if (w->f4 < t) {
            w->f4 = t;
        }
        w->pad0 += w->f4;
        w++;
        o++;
    }
    do {
        if ((s32)w < (s32)(base + 4)) {
            goto loop;
        }
    } while (0);
}

void Create_ArkPickup(void)
{
  SLONG_XYZ pos;
  HeroInfo *base;
  HeroInfo *h1;
  HeroInfo *h2;
  EXTengine *eng;
  s32 s;
  ArkenoidSpark *obj;
  NAS *nas;
  NAS *tgt;
  s32 i;
  s32 k;
  i = 0;
  if (((u16) ArkNGin->unk1A) != 0)
  {
    return;
  }
  base = Heroes;
  h1 = &base[ArkSidePlayer0];
  h2 = &base[ArkSidePlayer2];
  for (;;)
  {
    if (i >= 5)
    {
      return;
    }
    ArkPickupCount++;
    s = ArkPickupSpots[ArkPickupCount];
    i++;
    switch (s)
    {
      case 0:
        if ((h1->hero_status & 0xFA00) && (h2->hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 1:
        if ((h1->hero_status & 0xFA00) && (base[ArkSidePlayer1].hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 2:
        if ((base[ArkSidePlayer3].hero_status & 0xFA00) && (h2->hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      case 3:
        if ((base[ArkSidePlayer3].hero_status & 0xFA00) && (base[ArkSidePlayer1].hero_status & 0xFA00))
      {
        do
        {
          s = -1;
        }
        while (0);
      }
        break;

      default:
        ArkPickupCount = 0;
        break;

    }

    if (s >= 0)
    {
      nas = CreateNAS(ArkPosts[s].x, 0, ArkPosts[s].z, 0x1502, 0x11);
      if (nas == 0)
      {
        return;
      }
      eng = nas->Engine;
      if (eng != 0)
      {

        k = 0x12;
        Random(8);
        obj = (ArkenoidSpark *) AddObject(nas);
        if (obj != 0)
        {
          nas->FUX = (EXT *) obj;
          obj->unk14 = k;
          obj->unk1A = 7;
        }

        eng->flags = 0;
        D_8009E25C.n++;
        obj->unk24 = k;
        nas->UserInfo.user1 = s;
        nas->UserInfo.user2 = 0xB4;
      }
      break;
    }
  }

  pos = MapPoints[s + 4].pos;
  ArkGoals[nas->UserInfo.user1].unk10 = 1;
  tgt = func_8007A464(1, (s32 *) (&pos), *((s32 *) MapScene));
  if (tgt != 0)
  {
    func_80026C38((u8 *) nas, (u8 *) tgt);
  }
  if (nas->Failure != 0)
  {
    tgt = nas->Family.Child;
    if (tgt != 0)
    {
      KillNAS(tgt);
    }
    KillNAS(nas);
  }
}

void Plot_Ark_Pickup(NAS *arg0) {
    EXTobject *fux;
    EXTengine *e;
    HeroInfo *h;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    if (arg0->Engine != NULL && (fux = (EXTobject *)arg0->FUX) != NULL) {
        switch (fux->status) {
        case 0:
            fux->status = 1;
            fux->can_do = (fux->can_do & 0xFFD0) | 0x12;
            func_80026AD8((u8 *)arg0);
            fux->player = -1;
            break;
        case 7:
            fux->status = 0;
            break;
        default:
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                e = h->hero_eng;
                dx = e->unk10.x - ArkPosts[arg0->UserInfo.user1].x;
                dz = e->unk10.z - ArkPosts[arg0->UserInfo.user1].z;
                if (e->collision_box.flags & 0x40) {
                    r = ArkPosts[arg0->UserInfo.user1].r + e->collision_box.radius;
                } else {
                    r = ArkPosts[arg0->UserInfo.user1].r + 0x80;
                }
                r += 0x40;
                if (dx * dx + dz * dz < r * r) {
                    D_8009D754 = i;
                    h->hero_pu = fux->contains;
                    if (fux->contains == 0x12 || fux->contains == 0x17) {
                        CreateNAS(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1503, 0x11);
                    }
                    ArkGoals[arg0->UserInfo.user1].unk10 = 0;
                    PlaySfx(0x5DA, NULL, NULL, 0x1000, 0x1E00, 0);
                    goto kill;
                }
            }
            break;
        }
    }
    if ((u16)ArkNGin->unk1A != 0) {
        arg0->UserInfo.user2 = 0;
    }
    if (arg0->UserInfo.user2 != 0) {
        arg0->UserInfo.user2--;
        return;
    }
    PlaySfx(0x5D8, NULL, NULL, 0x1000, 0x1E00, 0);
    ArkGoals[arg0->UserInfo.user1].unk10 = 0;
kill:
    if (arg0->Family.Child != NULL) {
        KillNAS(arg0->Family.Child);
    }
    KillNAS(arg0);
}

void ArkHeroPowerupTimers(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_action_timer != 0) {
        h->hero_pu_action_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
}

void Plot_RepulseRing(NAS *arg0) {
    EXTengine *e;
    HeroInfo *h;
    subengine *s;
    s32 t;
    HeroInfo *base;

    base = Heroes;
    h = &base[arg0->UserInfo.user1];
    if (h->hero_status & 0xFA00) {
        KillNAS(arg0);
        return;
    }
    e = arg0->Engine;
    if (e == NULL) {
        return;
    }
    e->unk10 = h->hero_eng->unk10;
    e->unk1E = base[arg0->UserInfo.user1].hero_eng->unk1E;
    e->unk1C = base[arg0->UserInfo.user1].hero_eng->unk1C;
    e->unk20 = base[arg0->UserInfo.user1].hero_eng->unk20;
    s = e->sub;
    s->flags |= 0x40008000;
    t = rcossin_tbl[(((GameFrameCount << 7) & 0xFFF) - 0x400) & 0xFFF].unk2 * 1024 >> 12;
    s->unk52 = 1;
    s->unk20.x = t + 0x1200;
    s->unk20.y = t + 0x1200;
    s->unk20.z = t + 0x1200;
    s->unk76 = t + 0x600;
    e->unk5E = 1;
    ArkHeroRad2ObjectRepel(arg0->UserInfo.user1, 0x800);
    D_8009E25C.n += 1;
    if (arg0->UserInfo.user2 != 0) {
        arg0->UserInfo.user2 = arg0->UserInfo.user2 - 1;
        return;
    }
    base[arg0->UserInfo.user1].hero_pu = 0;
    PlaySfx(0x5D8, NULL, NULL, 0x1000, 0x1E00, 0);
    KillNAS(arg0);
}

void Init_ArkLaserWall(NAS *nas)
{
    EXTengine *eng;
    subengine *sub;
    s32 dir;
    s32 off;

    if (GameExitFlags & 0x801) {
        ArkWallFlags &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != 0) {
        dir = eng->unk10.x;
        if (dir == ArkSidePlayer3) {
            eng->unk10.x = -0x600;
            eng->unk10.z = 0x880;
            eng->unk1C = 0x71;
            eng->unk1E = 0;
            nas->UserInfo.flags = 0x20;
            ArkWallFlags |= 0x20;
        }
        if (dir == ArkSidePlayer0) {
            eng->unk10.x = -0x600;
            eng->unk10.z = -0x880;
            eng->unk1C = 0x200;
            eng->unk1E = 0;
            nas->UserInfo.flags = 0x10;
            ArkWallFlags |= 0x10;
        }
        if (dir == ArkSidePlayer2) {
            eng->unk10.x = -0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 0x40;
            ArkWallFlags |= 0x40;
        }
        if (dir == ArkSidePlayer1) {
            eng->unk10.x = 0x880;
            eng->unk10.z = -0x600;
            eng->unk1E = 0xC00;
            eng->unk20 = 0;
            nas->UserInfo.flags = 0x80;
            ArkWallFlags |= 0x80;
        }
        eng->unk5D = 2;
        eng->unk5E = 1;
        PlaySfx(0x5B7, &eng->sfx, 0, 0x1000, 0x1E00, 0);
        func_80018F3C(eng, 5);
        sub = eng->sub;
        off = 0;
        if (sub != 0) {
            do {
                sub->unk6C = *(s32 *)MapScene;
                sub->flags |= 0x21;
                sub->unk74 = 0x2019;
                sub->unk04 = off;
                sub = sub->next;
                off += 0x200;
            } while (sub != 0);
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void LineToCircleTest(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r) {
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
    start.x -= (0x200 * rcossin_tbl[(arg1 - 0x400) & 0xFFF].unk2) >> 12;
    start.z -= (0x200 * rcossin_tbl[arg1].unk2) >> 12;
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
    s = csqrt(s) >> 6;
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

NAS *ArkHero2ObjectCollision(u32 arg0) {
    SLONG_XYZ tgt;
    SLONG_XYZ nearp;
    SLONG_XYZ farp;
    VECTOR pos;
    SVECTOR spot;
    EXTengine *me;
    s32 speed;
    FlagRecord *grab;
    s32 top;
    s32 bot;
    s32 rnear;
    s32 rfar;
    HeroInfo *hero;
    ArkenoidHeroBox *box;
    NAS *nas;
    EXTengine *eng;
    FlagRecord *rec;
    FlagRecord *fr;
    EXTobject *obj;
    ArkenoidPuck *puck;
    s32 r;
    s32 rad;
    s32 back;
    s32 d;
    s32 dist;
    s32 y;
    s32 x1;
    s32 z1;
    s32 x2;
    s32 z2;
    s32 dz;
    s32 fa;
    s16 sp;
    s32 idx;
    TrigEntry *tbl;
    s32 cz;
    s32 fb;
    u16 ang;
    u16 ang2;
    u16 fc;
    u16 fc2;
    ArkenoidHeroBox *base;

    if (Heroes[arg0].hero_status & 0xF800) {
        return 0;
    }
    me = Heroes[arg0].hero_eng;
    if (me == NULL) {
        return 0;
    }
    speed = Heroes[arg0].hero_move->forward_speed;
    if (arg0 == ArkSidePlayer2) {
        speed = -speed;
    }
    if (arg0 == ArkSidePlayer1) {
        speed = Heroes[arg0].hero_move->forward_speed;
    }
    if (arg0 == ArkSidePlayer3) {
        speed = -Heroes[arg0].hero_move->forward_speed;
    }
    if (func_80015780(me, me->sub, 1, &pos) == NULL) {
        *(SLONG_XYZ *)&pos = me->unk10;
    }
    fr = func_80025A48(me, 2);
    if (fr != NULL) {
        if (me->unk5E != 0) {
            func_800142D4(&me->unk1C);
        }
        func_8001CD04(me->sub, me->unk3C, &me->unk10);
        grab = fr;
    } else {
        grab = NULL;
    }
    base = ArkHeroBoxes;
    box = &base[arg0];
    top = box->b;
    bot = box->a;
    rnear = box->c;
    rfar = box->d;
    nearp.x = Heroes[arg0].hero_eng->unk10.x + box->x0;
    nearp.y = Heroes[arg0].hero_eng->unk10.y + box->y0;
    nearp.z = Heroes[arg0].hero_eng->unk10.z + box->z0;
    farp.x = Heroes[arg0].hero_eng->unk10.x + box->x1;
    hero = &Heroes[arg0];
    farp.y = hero->hero_eng->unk10.y + box->y1;
    farp.z = hero->hero_eng->unk10.z + box->z1;
    tbl = rcossin_tbl;
    for (nas = HeadOfSolids;; nas = nas->Solid.Child) {
        if (nas == NULL) {
            return 0;
        }
        eng = nas->Engine;
        if (eng == NULL || eng == me) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        dz = rec->unk2;
        y = eng->unk10.y;
        x1 = rec->height;
        if (!(top < y + dz)) {
            continue;
        }
        y += x1;
        if (!(y < bot)) {
            continue;
        }
        rad = rec->radius;
        if (func_80015480(hero->hero_move->forward_angle,
                          get_angle(-eng->OldXYZ.x + pos.vx, -eng->OldXYZ.z + pos.vz)) >= 0x38F) {
            r = rnear;
            tgt = nearp;
            back = 0;
        } else {
            r = rfar;
            tgt = farp;
            back = 1;
            if (hero->hero_mode == 0x21) {
                r += 0x80;
            }
        }
        get_angle(eng->unk10.x - pos.vx, eng->unk10.z - pos.vz);
        obj = FindExtension(nas, 0x1000);
        if (obj == NULL) {
            continue;
        }
        if (obj->status == 2) {
            if (arg0 == obj->player) {
                puck = FindExtension(nas, 0x800);
                if (puck != NULL && grab != NULL) {
                    rad += rfar;
                    r = func_800154A0(me->unk1E, puck->dir);
                    if (r > 0x220) {
                        r = 0x220;
                    }
                    if (r < -0x220) {
                        r = -0x220;
                    }
                    r &= 0xFFF;
                    spot.vx = ((ArkenoidSpot *)grab)->x - (rad * tbl[(r - 0x400) & 0xFFF].unk2 >> 12);
                    z1 = rad * tbl[r].unk2 >> 12;
                    spot.vz = ((ArkenoidSpot *)grab)->z - z1;
                    spot.vy = 0;
                    func_80015758(&spot, (VECTOR *)&eng->unk10);
                }
            }
            continue;
        }
        y = eng->unk10.x - tgt.x;
        dist = y * y;
        y = eng->unk10.z - tgt.z;
        rad += r;
        dist += y * y;
        if (!(dist < rad * rad)) {
            continue;
        }
        dist = csqrt(dist) >> 6;
        if (!(dist < rad)) {
            continue;
        }
        puck = FindExtension(nas, 0x800);
        if (puck != NULL) {
            r = puck->dir;
        } else {
            r = get_angle(eng->OldXYZ.x - eng->unk10.x, eng->OldXYZ.z - eng->unk10.z);
        }
        LineToCircleTest(&eng->unk10, r, &tgt, rad + 8);
        r = get_angle(-eng->unk10.x + tgt.x, -eng->unk10.z + tgt.z) & 0xFFF;
        puck = FindExtension(nas, 0x800);
        if (puck == NULL) {
            continue;
        }
        obj->player = arg0;
        if (hero->hero_mode != 0x21) {
            back = 0;
        }
        if (back) {
            r = get_angle(-eng->unk10.x + farp.x, -eng->unk10.z + farp.z) & 0xFFF;
            puck->dir = r;
            puck->speed = 0;
            puck->max = 0;
            obj->status = 2;
            PlaySfx(ArkGrabTimes[arg0 + 4], NULL, &me->unk10, 0x1000, 0x1E00, 0);
            continue;
        }
        rad = puck->dir;
        rad += 0x800;
        rad &= 0xFFF;
        if (obj->whatami == 2) {
            if (!(hero->hero_status & 0xFA00) &&
                (arg0 == 0 || (GameTeams[0] != 0 && arg0 == 1))) {
                hero->hero_score = 0;
                if (arg0 == ArkSidePlayer0) {
                    idx = 0;
                } else if (arg0 == ArkSidePlayer3) {
                    idx = 1;
                } else if (arg0 == ArkSidePlayer2) {
                    idx = 3;
                } else {
                    idx = (arg0 == ArkSidePlayer1) * 2;
                }
                if (ArkDoorsB[idx].flags != NULL) {
                    *ArkDoorsB[idx].flags |= 0x8000;
                }
                if (ArkDeadWalls == 0) {
                    PlaySfx(0x591, &ArkDieEngine.sfx, NULL, 0x1000, 0x1E00, 0);
                    ArkDeadWalls = 1;
                }
                if (arg0 == ArkSidePlayer0) {
                    ArkWallFlags |= 1;
                }
                if (arg0 == ArkSidePlayer3) {
                    ArkWallFlags |= 2;
                }
                if (arg0 == ArkSidePlayer2) {
                    ArkWallFlags |= 4;
                }
                if (arg0 == ArkSidePlayer1) {
                    ArkWallFlags |= 8;
                }
                ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xC, 0xFF);
                SetHeroAction(arg0, 0x25);
                return 0;
            }
        } else {
            PlaySfx(ArkGrabFrames[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
            fa = hero->hero_move->forward_angle & 0xFFF;
            ang = r;
            fc = fa;
            sp = abs(speed);
            x1 = puck->speed * tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->speed * tbl[(u16)r].unk2 >> 12;
            dz = sp * tbl[(fc - 0x400) & 0xFFF].unk2 >> 12;
            cz = sp * tbl[fa].unk2 >> 12;
            r = (u16)get_angle(x1 + dz, z2 + cz);
        }
        r &= 0xFFF;
        d = func_800154A0(rad, r);
        if (abs(d) > 0x400) {
            func_800154A0(rad ^ 0x800, r);
            fb = hero->hero_move->forward_angle & 0xFFF;
            ang2 = r;
            fc2 = fb;
            sp = abs(speed);
            x1 = puck->speed * tbl[(ang2 - 0x400) & 0xFFF].unk2 >> 12;
            z2 = puck->speed * tbl[r].unk2 >> 12;
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
        puck->dir = r;
        if (speed != 0) {
            puck->speed += abs(speed);
            if (puck->speed > puck->max + 0x40) {
                puck->speed = puck->max + 0x40;
            }
        }
        if (puck->unk32 == 0 && puck->max < 0x90) {
            puck->max += 4;
        }
    }
    return 0;
}

bool ArkHeroRad2ObjectCollision(u32 arg0, s32 arg1, bool arg2) {
    EXTengine *hero;
    EXTengine *o;
    FlagRecord *rec;
    u8 *n;
    u8 *t;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 ang;
    u16 v;
    u16 old;
    bool hit;

    hero = Heroes[arg0].hero_eng;
    hit = 0;
    if (hero != NULL) {
        n = HeadOfBalls;
        if (n != NULL) {
            do {
                o = *(EXTengine **)(n + 0x54);
                if (o != NULL && o != hero) {
                    rec = func_80025A48(o, 0x40);
                    if (rec != NULL) {
                        dx = o->unk10.x - hero->unk10.x;
                        dz = o->unk10.z - hero->unk10.z;
                        r = rec->radius;
                        r += arg1;
                        d = dx * dx + dz * dz;
                        if (d < r * r) {
                            d = csqrt(d) >> 6;
                            if (d < r) {
                                t = FindExtension((NAS *)n, 0x1000);
                                if (t != NULL
                                    && (*(u16 *)(t + 0x1A) == 1 || *(u16 *)(t + 0x1A) == 8)
                                    && *(u16 *)(t + 0x14) == 1 && *(s16 *)(t + 0x20) == 0) {
                                    ang = get_angle(-dx, -dz) & 0xFFF;
                                    d = r - d + 1;
                                    o->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                    o->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                                    *(u16 *)(t + 0x16) = arg0;
                                    e = FindExtension((NAS *)n, 0x800);
                                    if (e != NULL) {
                                        hit = 1;
                                        PlaySfx(0x585, NULL, NULL, 0x1000, 0x1E00, 0);
                                        *(s16 *)(t + 0x20) = 4;
                                        *(s16 *)(e + 0x16) = ang;
                                        switch (GameMode) {
                                        case 0xF:
                                            *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                            *(u16 *)(e + 0x14) = 0x40;
                                            break;
                                        case 0x11:
                                            if (arg2) {
                                                o->sub->unk74 = 0x2018;
                                                *(u16 *)(t + 0x14) = 2;
                                                *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                                *(u16 *)(e + 0x14) = 0x80;
                                            } else {
                                                *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                                *(u16 *)(e + 0x14) = 0x80;
                                            }
                                            break;
                                        default:
                                            *(s16 *)(e + 0xE) = *(u16 *)(e + 0x14) + 0x40;
                                            *(u16 *)(e + 0x14) = 0x90;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x34);
            } while (n != NULL);
        }
    }
    return hit;
}

void ArkHeroRad2ObjectRepel(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *t;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 q;
    s32 r;
    s32 lim;
    s32 ang;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 1);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = HeadOfBalls;
        if (n != NULL) {
            do {
                t = *(u8 **)(n + 0x58);
                if (t != NULL && *(u16 *)(t + 0x1A) == 1) {
                    o = *(u8 **)(n + 0x54);
                    if (o != NULL) {
                        dx = *(s32 *)(o + 0x10) - sp10.x;
                        dz = *(s32 *)(o + 0x18) - sp10.z;
                        d = dx * dx + dz * dz;
                        if (d < lim) {
                            d = csqrt(d) >> 6;
                            ang = get_angle(-dx, -dz) & 0xFFF;
                            d = r - d;                            e = FindExtension((NAS *)n, 0x800);
                            if (e != NULL) {
                                *(s16 *)(e + 0x2A) = ang;
                                *(s16 *)(e + 0x22) = d >> 4;
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

void ArkHeroRad2ObjectAttract(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 lim;
    s32 ang;
    s32 a;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 2);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = HeadOfBalls;
        if (n != NULL) {
            do {
                o = *(u8 **)(n + 0x54);
                if (o != NULL) {
                    dx = *(s32 *)(o + 0x10) - sp10.x;
                    dz = *(s32 *)(o + 0x18) - sp10.z;
                    d = dx * dx + dz * dz;
                    if (d < lim) {
                        e = FindExtension((NAS *)n, 0x800);
                        if (e != NULL) {
                            ang = get_angle(dx, dz) & 0xFFF;
                            a = func_80015480(ang, *(s16 *)(e + 0x16));
                            if (a < 0x300) {
                                s32 s = csqrt(d);
                                d = s >> 6;
                                if (a < (s >> 8)) {
                                    d = r - d;
                                    *(s16 *)(e + 0x2A) = ang;
                                    *(s16 *)(e + 0x22) = d >> 4;
                                }
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

bool TestArkObjectObstacles(EXTengine *eng, EXTobject *self) {
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *other;
    NAS *it;
    ArkenoidMot *mine;
    ArkenoidMot *m;
    ArkenoidFuxF *fux;
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
    s32 avg;

    hit = 0;
    box = func_80025A48(eng, 0x40);
    if (box != NULL) {
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        rad = box->radius;
        mine = FindExtension(((NAS **)eng)[-1], 0x800);
        it = HeadOfSolids;
        if (it != NULL) {
            do {
                if (it->Class & 0x200) {
                    other = it->Engine;
                    if (other != NULL && other != eng) {
                        rec = func_80025A48(other, 0x40);
                        if (rec != NULL) {
                            lo = other->unk10.y + rec->unk2;
                            hi = other->unk10.y + rec->height;
                            if (bot < lo && hi < top) {
                                dx = other->unk10.x - eng->unk10.x;
                                dz = other->unk10.z - eng->unk10.z;
                                r = rec->radius;
                                r += rad;
                                if ((d = dx * dx + dz * dz) < r * r) {
                                    d = csqrt(d) >> 6;
                                    do {
                                    if (d < r) {
                                        ang = get_angle(-dx, -dz) & 0xFFF;
                                        d = r - d + 1;
                                        fux = (ArkenoidFuxF *)it->FUX;
                                        if (fux != NULL) {
                                            if (self->can_do & 0x80) {
                                                m = FindExtension(it, 0x800);
                                                if (m != NULL) {
                                                    other->unk10.x -= (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                    other->unk10.z -= (d * rcossin_tbl[ang].unk2) >> 12;
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += func_800154A0((m->dir + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    m->dir = ang;
                                                    PlaySfx(0x580, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                                                }
                                            } else {
                                                if (r / 2 < d) {
                                                    d = r;
                                                }
                                                eng->unk10.x += (d * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                                eng->unk10.z += (d * rcossin_tbl[ang].unk2) >> 12;
                                                if (mine != NULL) {
                                                    m = FindExtension(it, 0x800);
                                                    if (m != NULL) {
                                                        if (!(fux->flags & 0x80)) {
                                                            m->dir = ang;
                                                            dx = (mine->a + m->a) / 2;
                                                            m->a = dx;
                                                            mine->a = dx;
                                                        }
                                                        PlaySfx(0x580, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                                                    }
                                                    ang += 0x800;
                                                    ang &= 0xFFF;
                                                    ang += func_800154A0((mine->dir + 0x800) & 0xFFF, ang);
                                                    ang &= 0xFFF;
                                                    mine->dir = ang;
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

s32 TestArkObject2Hero(EXTengine *arg0, u16 arg1) {
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
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            goto next;
        }
        dx = eng->unk10.x - arg0->unk10.x;
        dz = eng->unk10.z - arg0->unk10.z;
        r = rec->radius + arg1;
        r = r * r;
        if ((dx * dx) + (dz * dz) < r) {
            return i;
        }
    next:;
    }
    return -1;
}

void Plot_ArkLaserWall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    if (GameExitFlags & 0x801) {
        ArkWallFlags &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != NULL) {
        if (CrateMasterState == 0) {
            ArkWallFlags &= ~nas->UserInfo.flags;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            KillNAS(nas);
            return;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Init_RepulseRing(NAS *nas)
{
  EXTengine *eng;
  char new_var;
  subengine *sub;
  eng = nas->Engine;
  if (eng != 0)
  {
    nas->UserInfo.user1 = D_8009D754;
    eng->unk5D = 0;
    eng->unk5C = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->unk51 = (new_var = 0);
    sub->unk6C = &BallsEffectsModel;
    sub->unk74 = 0x2014;
    sub->unk50 = 1;
    sub->unk08 = 0x80;
    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x21;
    PlaySfx(0x5D6, 0, 0, 0x1000, 0x1E00, new_var);
    PlaySfx(0x5D7, &eng->sfx, 0, 0x1000, 0x1E00, 0);
    nas->UserInfo.user2 = 0xF0;
    D_8009E25C.n += 1;
  }
  if (nas->Failure != 0)
  {
    KillNAS(nas);
  }
}

s32 GetArkPIContour(EXTengine *v) {
    return v->unk10.y - ((World_floor_level - (v->unk10.z * rcossin_tbl[(ArkFloorTiltX - 0x400) & 0xFFF].unk2 >> 12)) +
                       (v->unk10.x * rcossin_tbl[(ArkFloorTiltZ - 0x400) & 0xFFF].unk2 >> 12));
}

s32 GetArkContour(EXTengine *arg0) {
    return arg0->unk10.y - World_floor_level;
}

void Plot_ArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (ArkFloorState == 0) {
        ArkWallFlags &= ~arg0->UserInfo.flags;
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
                sub->unk74 = (GameFrameCount & 7) + 0x2001;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Plot_ArkCameraFlash(NAS *arg0)
{
  long new_var;
  EXTengine *eng = arg0->Engine;
  if (eng != 0)
  {
    ArkenoidCircle *base = CameraFlashCircles;
    subengine *sub = eng->sub;
    ArkenoidCircle *c = &base[arg0->UserInfo.user1];
    if (c->x != 0)
    {
      sub->unk20.x = c->x;
      sub->unk20.y = c->z;
      new_var = c->r;
      sub->flags |= 0x8000;
      sub->unk52 = 1;
      sub->unk20.x = new_var;
      arg0->UserInfo.user1 += 1;
    }
    else
    {
      KillNAS(arg0);
    }
  }
}

void Init_ArkCameraFlash(NAS *arg0) {
    u8 *eng = (u8*)(*(s32*)((u8*)(arg0) + 84));
    u8 *sub;
    if (eng != NULL) {
        s32 val;
        sub = (u8*)(*(s32*)(eng + 108));
        val = *(s32*)MapScene;
        (*(u16*)(sub + 116)) = 0x2017;
        (*(u8*)(sub + 80)) = 1;
        (*(u8*)(sub + 81)) = 0;
        (*(s32*)(sub + 108)) = val;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS(arg0);
    }
}

void Plot_seaweed(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0(arg0);
    }
}

void Init_seaweed(NAS *arg0) {
    EXTengine *eng = arg0->Engine;
    if (eng != NULL) {
        ScriptCursor *cur;
        eng->sub->unk6C = *(s32*)MapScene;
        cur = func_80024E30(arg0, &SeaweedScriptInit);
        if (cur != NULL) {
            u8 *trig = (u8*)arg0->trigger;
            if (trig != NULL) {
                u8 *info = (u8*)*(s32*)(trig + 16);
                s32 n = *(s32*)(info + 4);
                ((void (*)(EXTanimator*, s32))GotoLinkAnim)((EXTanimator*)cur, n);
                if (n < 2) {
                    func_800250B0(arg0);
                    *(u16*)((u8*)cur + 30) += Random(8);
                }
                eng->sub->flags |= 0x02000000;
                eng->sub->unk68 = *(u16*)(info + 8);
            }
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void Init_N_Gin(NAS *nas) {
    EXTengine *eng;
    ArkenoidGin *obj;

    eng = nas->Engine;
    if (eng != NULL) {
        func_80024E30(nas, &NGinScriptInit);
        ArkBallSpawner = eng;
        eng->sub->unk6C = NginModel;
        eng->sub->unk74 = 0x2001;
        ArkNGin = obj = (ArkenoidGin *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            nas->FUX = (EXT *)obj;
            obj->unk14 = 0xE;
            obj->unk1A = 0;
            obj->unk22 = 0x186;
            obj->unk1C = 0;
        }
        eng->unk10.y = -0x1000;
        nas->Class &= 0xFDFF;
        eng->unk5D = 1;
        eng->flags &= 0xFFFF7FFF;
        FetchDefaultColBox(eng, 0x50);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

u8 *ArkFreeBallSlot(void) {
    s32 var_v0;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_v1;

    var_v1 = HeadOfBalls;
    var_v0 = 0;
    if (var_v1 != NULL) {
        do {
            var_v1 = (*(s32*)((u8*)(var_v1) + 32));
            var_v0 += 1;
        } while (var_v1 != NULL);
    }
    if (var_v0 >= 9) {
        return NULL;
    }
    temp_v0 = CreateNAS(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

s32 ArkWallIndex(s32 arg0, s32 arg1) {
    s32 row;

    row = arg0 == ArkSidePlayer0;
    if (arg0 == ArkSidePlayer2) {
        row = 2;
    }
    if (arg0 == ArkSidePlayer1) {
        row = 3;
    }
    if (arg1 == *ArkWallTargets[row][0]) {
        return 0;
    }
    if (arg1 == *ArkWallTargets[row][1]) {
        return 1;
    }
    if (arg1 == *ArkWallTargets[row][2]) {
        return 2;
    }
    return 3;
}

void SetArkHeroGrab(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    ArkHeroSetSpeed();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x1D);
    h->hero_weapon_timer = 0x96;
    h->hero_attack_counter = 0;
    PlaySfx(ArkGrabTimes[arg0], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
}

void SetArkHeroRedKick(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    ArkHeroSetSpeed();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x1D);
    ArkRedKickTimer = Random(0x3C) + 0x3C;
    h->hero_attack_counter = 0;
    PlaySfx(ArkKickFrames[arg0], NULL, NULL, 0x1000, 0x1E00, 0);
}

void ArkHeroDie(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction(arg0, 0x25);
    }
    func_8008502C(arg0);
}

void ArkHeroLose(u32 arg0) {
    Heroes[arg0].hero_frozen = 5;
    func_8008502C();
}

void SetArkHeroMove(u32 arg0) {
    ArkHeroSetSpeed();
    SetHeroSeq(arg0, 0x1A);
}

void SetArkHeroIdle(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    ArkHeroSetSpeed(arg0);
    SetHeroSeq(arg0, 0x19);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

bool TestForMove(u32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x22;
    if (!(st & 0x200)) {
        act = 0x23;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction(arg0, act);
    return 1;
}

bool TestArkenoidStop(u32 arg0) {
    HeroInfo *base = Heroes;
    u16 st = base[arg0].hero_status;
    s32 act;

    if (st & 0x200) {
        act = 0x22;
    } else if (st & 0x2000) {
        act = 0x24;
    } else if (st & 0x400) {
        act = 0x23;
    } else {
        return 0;
    }
    SetHeroAction(arg0, act);
    return 1;
}

void AR_UpdateShadow(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *shadow = h->hero_shadow;

    shadow->unk10 = h->hero_eng->unk10;
    shadow->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(shadow);
    shadow->unk1E = h->hero_eng->unk1E;
    shadow->unk5E = 1;
    if (h->hero_status & 0xF800) {
        shadow->sub->flags &= ~0x8000;
    }
}

extern char BallsVehicleModel[];

void AR_InitShadow(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->unk6C = BallsVehicleModel;
    s->sub->unk74 = 0x2002;
    s->unk10 = h->hero_eng->unk10;
    s->unk5D = 0;
    s->flags |= 0x8041;
    s->sub->unk76 = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->unk68 = -0x10;
}

s32 ArkAddPolar(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
    s32 a = arg1 * rcossin_tbl[((arg0 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 b = arg1 * rcossin_tbl[arg0 & 0xFFF].unk2;
    s32 c = arg3 * rcossin_tbl[((arg2 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 d = arg3 * rcossin_tbl[arg2 & 0xFFF].unk2;
    s32 x = a >> 12;
    s32 z = b >> 12;
    x += c >> 12;
    z += d >> 12;
    return get_angle(x, z) & 0xFFFF;
}

bool AR_PI_TestLaunchBounds(EXTengine *arg0, EXTobject *obj) {
    ArkenoidCircle *base = ArkPosts;
    ArkenoidCircle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->collision_box.flags & 0x40) {
            r = c->r + arg0->collision_box.radius;
        } else {
            r = c->r + 0x80;
        }
        dx = arg0->unk10.x - c->x;
        dz = arg0->unk10.z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    return 0;
}

bool AR_NG_TestOutOfBounds(EXTengine *arg0, EXTobject *arg1, bool arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0x1000;
    }
    x = arg0->unk10.x;
    if (x > lim) {
        arg1->player = ArkSidePlayer1;
        return 1;
    }
    if (x < -lim) {
        arg1->player = ArkSidePlayer2;
        return 1;
    }
    z = arg0->unk10.z;
    if (z > lim) {
        arg1->player = ArkSidePlayer3;
        return 1;
    }
    if (z < -lim) {
        arg1->player = ArkSidePlayer0;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->player;
        if (d == ArkSidePlayer3 || d == ArkSidePlayer0) {
            if (x > 0x680) {
                arg0->unk10.x = 0x680;
            }
            if (arg0->unk10.x < -0x680) {
                arg0->unk10.x = -0x680;
            }
        } else {
            if (z > 0x680) {
                arg0->unk10.z = 0x680;
            }
            if (arg0->unk10.z < -0x680) {
                arg0->unk10.z = -0x680;
            }
        }
    }
    return 0;
}

bool AR_BA_TestOutOfBounds(EXTengine *arg0, EXTobject *arg1, bool arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0xB80;
    }
    x = arg0->unk10.x;
    if (x > lim) {
        arg1->player = ArkSidePlayer1;
        return 1;
    }
    if (x < -lim) {
        arg1->player = ArkSidePlayer2;
        return 1;
    }
    z = arg0->unk10.z;
    if (z > lim) {
        arg1->player = ArkSidePlayer3;
        return 1;
    }
    if (z < -lim) {
        arg1->player = ArkSidePlayer0;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->player;
        if (d == ArkSidePlayer3 || d == ArkSidePlayer0) {
            if (x > 0x600) {
                arg0->unk10.x = 0x600;
            }
            if (arg0->unk10.x < -0x600) {
                arg0->unk10.x = -0x600;
            }
        } else {
            if (z > 0x600) {
                arg0->unk10.z = 0x600;
            }
            if (arg0->unk10.z < -0x600) {
                arg0->unk10.z = -0x600;
            }
        }
    }
    return 0;
}

void AR_Triggers(NASTrigger *arg0, s32 arg1) {
}

void AR_EarliestTriggers(void) {
}

void AR_RestartLevel(void) {
}

void AR_SetUp(void) {
    LevelSpecs.LevelPreInit = AR_PreInitLevel;
    LevelSpecs.LevelInit = AR_InitLevel;
    LevelSpecs.LevelRestartInit = AR_InitLevel;
    LevelSpecs.LevelRoutine = AR_UpdateLevel;
    LevelSpecs.PostTriggers = AR_Triggers;
    LevelSpecs.EarliestTriggers = AR_EarliestTriggers;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.GetContour = GetArkContour;
    LevelSpecs.FinishQuick = FinishArkLevel;
}
