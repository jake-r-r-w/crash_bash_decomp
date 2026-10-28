#include "common.h"
#include "bash/main.h"
#include "bash/tank_ovl.h"
#include "bash/engine.h"
#include "bash/tank.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char TankAnimBreathe[] = "BREATHE";
const char TankAnimIdle0[] = "IDLE0";
const char TankAnimIdle1[] = "IDLE1";
const char TankAnimHit[] = "HIT";
const char TankAnimDie[] = "DIE";
const char TankAnimFire1[] = "FIRE1";
const char TankAnimFire2[] = "FIRE2";
const char TankAnimFire3[] = "FIRE3";
const char TankAnimMine[] = "MINE";
const char TankAnimWin[] = "WIN";
const char TankEmptyString[] = "";

/* data, in address order */
s16 TankPreLoadDB[4] = { 0x1C, 0x1A, 0x17, -1 };
char *TankAnims[10] = {
    (char *)TankAnimBreathe, (char *)TankAnimIdle0, (char *)TankAnimIdle1, (char *)TankAnimHit,
    (char *)TankAnimDie, (char *)TankAnimFire1, (char *)TankAnimFire2, (char *)TankAnimFire3,
    (char *)TankAnimMine, NULL,
};
s16 TankLevelDB0[4] = { 0x37, 0x3C, -1, 0 };
s16 TankLevelDB1[4] = { 0x37, 0x38, -1, 0 };
s16 TankLevelDB2[4] = { 0x37, 0x38, 0x3D, -1 };
s16 TankLevelDB3[4] = { 0x37, 0x38, -1, 0 };
s16 TankLevelDB4[4] = { 0x37, 0x38, -1, 0 };
WWInf TankWorldInfo[9] = {
    { 0x2E, TankAnims, TankLevelDB0 },
    { 0x2F, TankAnims, TankLevelDB1 },
    { 0x30, TankAnims, TankLevelDB2 },
    { 0x31, TankAnims, TankLevelDB3 },
    { 0x32, TankAnims, TankLevelDB4 },
    { 0x33, TankAnims, TankLevelDB0 },
    { 0x34, TankAnims, TankLevelDB1 },
    { 0x35, TankAnims, TankLevelDB2 },
    { 0x36, TankAnims, TankLevelDB3 },
};
s32 TankFiles[24] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 563], (s32)&DatIndex[2 * 562], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 559],
    (s32)&DatIndex[2 * 558], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 561], (s32)&DatIndex[2 * 560], (s32)&HeroModelBank[DB_Powerups],
    (s32)&DatIndex[2 * 557], (s32)&DatIndex[2 * 556], (s32)&HeroModelBank[DB_Angel], (s32)&DatIndex[2 * 555], (s32)&DatIndex[2 * 554],
    (s32)&HeroModelBank[DB_TankObjects], (s32)&DatIndex[2 * 553], (s32)&DatIndex[2 * 552], (s32)&HeroModelBank[DB_TankObjects2], (s32)&DatIndex[2 * 551],
    (s32)&DatIndex[2 * 550], 0, 0, 0,
};
s32 TankSwampFiles[24] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 549], (s32)&DatIndex[2 * 548], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 545],
    (s32)&DatIndex[2 * 544], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 547], (s32)&DatIndex[2 * 546], (s32)&HeroModelBank[DB_Powerups],
    (s32)&DatIndex[2 * 543], (s32)&DatIndex[2 * 542], (s32)&HeroModelBank[DB_Angel], (s32)&DatIndex[2 * 541], (s32)&DatIndex[2 * 540],
    (s32)&HeroModelBank[DB_TankObjects], (s32)&DatIndex[2 * 539], (s32)&DatIndex[2 * 538], (s32)&HeroModelBank[DB_TankVehicle], (s32)&DatIndex[2 * 537],
    (s32)&DatIndex[2 * 536], 0, 0, 0,
};
/* The five arenas' files (modes 5..9; the engine's table at 0x800A0030): the exe's file group
 * (24 or 25) and the model list; the swamp's has the hover vehicle. */
file_group_load_info file_group_TANKLAST = { &FileGroups[24], TankSwampFiles };
file_group_load_info file_group_TANKDESERT = { &FileGroups[25], TankFiles };
file_group_load_info file_group_TANKJUNGLE = { &FileGroups[25], TankFiles };
file_group_load_info file_group_TANKMETAL = { &FileGroups[25], TankFiles };
file_group_load_info file_group_TANKBOSS = { &FileGroups[25], TankFiles };
u16 TankLUPtab[8] = { 3, 0x20D, 0x201, 0x20D, 0x1301, 0x1306, 0x1309, 0x130A };
TrigLookUp TankLUP = { TankLUPtab, 8 };
PreMap TANK_PREMAP = { TankPreLoadDB, TankWorldInfo, &TankLUP };
u16 TankHeroModels[10] = {
    0, 0x3E, 0x3F, 0x40, 0x42, 0x41, 0x44, 0x43, 0x45, 0,
};
s32 TankVictorySeqs[8] = {
    (s32)&TankAnimWin, 0x5C, (s32)&TankAnimWin, 0x5C, (s32)&TankAnimWin, 0x5C, (s32)&TankAnimWin,
    0x5C,
};
Ambient_data TankJungleAmbience = { 2, 6, 0x3FC, 0, { 0x3FD, 0x3FE, 0x3FF, 0x400, 0x401, 0x402, 0x403, 0x3FD } };
shell_setup tank_shell_info = {
    0x4000, 0x20, 0x1800, 0xB0, 0x200, 0x100, VramAreas, VramNodePool, 0x20, 0x5DC,
};
s32 TankHeroSpeeds[16] = {
    0x1C, 0x1C, 0x18, 0x18, 0x1A, 0x1A, 0x18, 0x18, 0x2028, 0x2037, 0x2038, 0x2039, 0x2029,
    0x202A, 0x202B, 0x202C,
};
s32 MoeScript0[5] = { 0x2028, 0x2037, 0x2038, 0x2039, 0 };
s32 MoeScript1[16] = {
    0x2028, 0x2037, 0x2038, 0x2039, 0x203A, 0x203B, 0x203C, 0x203D, 0x203E, 0x203F, 0x2040,
    0x2041, 0x2042, 0x2043, 0x2044, 0,
};
s32 MoeScript2[16] = {
    0x2028, 0x2037, 0x2038, 0x2039, 0x203A, 0x203B, 0x203A, 0x2039, 0x2038, 0x2039, 0x203A,
    0x2039, 0x2038, 0x2037, 0x2028, 0,
};
s32 MoeScript3[24] = {
    0x2028, 0x2037, 0x2038, 0x2039, 0x203A, 0x203B, 0x203C, 0x203D, 0x203C, 0x203B, 0x203A,
    0x203B, 0x203C, 0x203B, 0x203A, 0x2039, 0x2038, 0x2039, 0x203A, 0x2039, 0x2038, 0x2037,
    0x2028, 0,
};
s32 MoeScript4[14] = {
    0x2028, 0x2037, 0x2038, 0x2039, 0x2038, 0x2037, 0x2038, 0x2039, 0x203A, 0x2039, 0x2038,
    0x2037, 0x2028, 0,
};
s32 MoeScript5[14] = {
    0x2028, 0x2037, 0x2038, 0x2037, 0x2028, 0x2037, 0x2038, 0x2037, 0x2028, 0x2037, 0x2038,
    0x2037, 0x2028, 0,
};
s32 MoeScript6[25] = {
    0x2028, 0x2038, 0x203A, 0x203C, 0x203D, 0x203E, 0x203D, 0x203C, 0x203B, 0x203E, 0x203B,
    0x203C, 0x203C, 0x203C, 0x203C, 0x203D, 0x203D, 0x203C, 0x203B, 0x203A, 0x2039, 0x2038,
    0x2037, 0x2028, 0,
};
s32 MoeScript7[9] = { 0x2028, 0x2038, 0x203A, 0x203C, 0x203E, 0x2040, 0x2042, 0x2044, 0 };
s32 JoeScript0[9] = { 0x2027, 0x2029, 0x202A, 0x202B, 0x202C, 0x202D, 0x202E, 0x202F, 0 };
s32 JoeScript1[16] = {
    0x2027, 0x2029, 0x202A, 0x202B, 0x202C, 0x202D, 0x202E, 0x202F, 0x2030, 0x2031, 0x2032,
    0x2033, 0x2034, 0x2035, 0x2036, 0,
};
s32 JoeScript2[16] = {
    0x2027, 0x2029, 0x202A, 0x202B, 0x202C, 0x202D, 0x202C, 0x202B, 0x202A, 0x202B, 0x202C,
    0x202B, 0x202A, 0x2029, 0x2027, 0,
};
s32 JoeScript3[24] = {
    0x2027, 0x2029, 0x202A, 0x202B, 0x202C, 0x202D, 0x202E, 0x202F, 0x202E, 0x202D, 0x202C,
    0x202D, 0x202E, 0x202D, 0x202C, 0x202B, 0x202A, 0x202B, 0x202C, 0x202B, 0x202A, 0x2029,
    0x2027, 0,
};
s32 JoeScript4[14] = {
    0x2027, 0x2029, 0x202A, 0x202B, 0x202A, 0x2029, 0x202A, 0x202B, 0x202C, 0x202B, 0x202A,
    0x2029, 0x2027, 0,
};
s32 JoeScript5[14] = {
    0x2027, 0x2029, 0x202A, 0x2029, 0x2027, 0x2029, 0x202A, 0x2029, 0x2027, 0x2029, 0x202A,
    0x2029, 0x2027, 0,
};
s32 JoeScript6[25] = {
    0x2027, 0x202A, 0x202C, 0x202E, 0x202F, 0x2030, 0x202F, 0x202E, 0x202D, 0x2030, 0x202D,
    0x202E, 0x202E, 0x202E, 0x202E, 0x202F, 0x202F, 0x202E, 0x202D, 0x202C, 0x202B, 0x202A,
    0x2029, 0x2027, 0,
};
s32 JoeScript7[17] = { 0x2027, 0x202A, 0x202C, 0x202E, 0x2030, 0x2032, 0x2034, 0x2036, 0, 8, 8, 8, 8, 8, 8, 8, 8 };
MineOffset MineShrapnelOffsets[4] = { { 0xFA, 0, 0x2021 }, { -0xFA, 0, 0x201F }, { 0, 0xFA, 0x201E }, { 0, -0xFA, 0x201B } };
RockTblEnt MineFrames[4] = { { 0x2002, 0 }, { 0x2003, 0 }, { 0x2004, 0 }, { 0x2005, 0 } };
RockTblEnt MineFramesBig[4] = { { 0x2017, 0 }, { 0x2016, 0 }, { 0x2015, 0 }, { 0x2014, 0 } };
s32 TankRocketAngles[19] = {
    -0x104, 0x104, 0x2044, 0x2043, 0x2042, 0x2041, 0x2040, 0x203F, 0x203E, 0x203D, 0x203C,
    0x203D, 0x203E, 0x203F, 0x2040, 0x2041, 0x2042, 0x2043, 0x2044,
};
s32 TankForceFieldSfx[4] = { 0x510, 0x511, 0x512, 0x513 };
s32 TankHoverSfx[4] = { 0x50B, 0x50C, 0x50D, 0x50E };
s32 TankMineSfx[4] = { 0x4EC, 0x4ED, 0x4EE, 0x4EF };
s32 TankRecoilSfx[8][4] = {
    { 3, 4, 5, 6 }, { 0x35, 0x36, 0x37, 0x38 }, { 0x67, 0x68, 0x69, 0x6A },
    { 0x99, 0x9A, 0x9B, 0x9C }, { 0x12F, 0x130, 0x131, 0x132 }, { 0x161, 0x162, 0x163, 0x164 },
    { 0xFD, 0xFE, 0xFF, 0x100 }, { 0xCB, 0xCC, 0xCD, 0xCE },
};
s32 TankTauntSfx[8] = { 2, 0x34, 0x66, 0x98, 0x12E, 0x160, 0xFC, 0xCA };
s32 TankHullSfx[4] = { 0x4C2, 0x4C3, 0x4C4, 0x4C5 };
s32 TankTurretSfx[4] = { 0x4BE, 0x4BF, 0x4C0, 0x4C1 };
s32 TankTurnSfx[4] = { 0x4FD, 0x4FE, 0x4FF, 0x500 };
hero_seq CRA_seq_tankidle[6] = { 0, 0x8007, 0x13, 0x4000, 0x8000, 0 };
hero_seq CRA_seq_tankvictory[6] = { 0, 0x8007, 0x50, 0x4200, 0x8000, 0 };
hero_seq CRA_seq_tankrecoil[6] = { 0, 0x8007, 0x1B, 0x4280, 0x8000, 0 };
hero_seq CRA_seq_tankdie[6] = { 0, 0x8007, 0x28, 0x4180, 0x41A7, 0x41A7 };
/* three sequences no table points at */
hero_seq CRA_seq_tankunused[18] = {
    0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x41A7,
    0x41A7, 0x41A7, 0x41A7, 0x41A7, 0x8000, 0,
};
hero_seq CRA_seq_tankidle1[6] = { 0, 0x8007, 0x29, 0x4100, 0x8000, 0 };
hero_seq CRA_seq_tankminelay[6] = { 0, 0x8007, 0x32, 0x4080, 0x8000, 0 };
hero_seq CRA_seq_tankfire1[6] = { 0, 0x8007, 0x13, 0x4000, 0x8000, 0 };
hero_seq CRA_seq_tankfire2[6] = { 0, 0x8007, 0x13, 0x4000, 0x8000, 0 };
hero_seq CRA_seq_tankfire3[6] = { 0, 0x8007, 0x13, 0x4000, 0x8000, 0 };
hero_seq COC_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq COC_seq_tankvictory[6] = { 0, 0x8007, 0x40, 0x4000, 0x8000, 0 };
hero_seq COC_seq_tankrecoil[6] = { 0, 0x8007, 0x16, 0x4180, 0x8000, 0 };
hero_seq COC_seq_tankdie[6] = { 0, 0x8007, 0x32, 0x4200, 0x8000, 0 };
hero_seq COC_seq_tankidle1[6] = { 0, 0x8007, 0x28, 0x4100, 0x8000, 0 };
hero_seq COC_seq_tankminelay[6] = { 0, 0x8007, 0x2C, 0x4080, 0x8000, 0 };
hero_seq COC_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq COC_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq COC_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq TIN_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq TIN_seq_tankvictory[6] = { 0, 0x8007, 0x50, 0x4000, 0x8000, 0 };
hero_seq TIN_seq_tankrecoil[6] = { 0, 0x8007, 0x1E, 0x4180, 0x8000, 0 };
hero_seq TIN_seq_tankdie[6] = { 0, 0x8007, 0x29, 0x4200, 0x8000, 0 };
hero_seq TIN_seq_tankidle1[6] = { 0, 0x8007, 0x29, 0x4080, 0x8000, 0 };
hero_seq TIN_seq_tankminelay[6] = { 0, 0x8007, 0x28, 0x4100, 0x8000, 0 };
hero_seq TIN_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq TIN_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq TIN_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4280, 0x8000, 0 };
hero_seq CTX_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq CTX_seq_tankvictory[6] = { 0, 0x8007, 0x52, 0x4000, 0x8000, 0 };
hero_seq CTX_seq_tankrecoil[6] = { 0, 0x8007, 0x1E, 0x4100, 0x8000, 0 };
hero_seq CTX_seq_tankdie[6] = { 0, 0x8007, 0x29, 0x4180, 0x8000, 0 };
hero_seq CTX_seq_tankidle1[6] = { 0, 0x8007, 0x28, 0x4280, 0x8000, 0 };
hero_seq CTX_seq_tankminelay[6] = { 0, 0x8007, 0x27, 0x4080, 0x8000, 0 };
hero_seq CTX_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq CTX_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq CTX_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq BRO_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq BRO_seq_tankvictory[6] = { 0, 0x8007, 0x39, 0x4280, 0x8000, 0 };
hero_seq BRO_seq_tankrecoil[6] = { 0, 0x8007, 0x1E, 0x4100, 0x8000, 0 };
hero_seq BRO_seq_tankdie[6] = { 0, 0x8007, 0x29, 0x4080, 0x8000, 0 };
hero_seq BRO_seq_tankidle1[6] = { 0, 0x8007, 0x3C, 0x4200, 0x8000, 0 };
hero_seq BRO_seq_tankminelay[6] = { 0, 0x8007, 0x39, 0x4180, 0x8000, 0 };
hero_seq BRO_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq BRO_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq BRO_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq DNG_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq DNG_seq_tankvictory[6] = { 0, 0x8007, 0x1E, 0x4080, 0x8000, 0 };
hero_seq DNG_seq_tankrecoil[6] = { 0, 0x8007, 0x1C, 0x4180, 0x8000, 0 };
hero_seq DNG_seq_tankdie[6] = { 0, 0x8007, 0x33, 0x4200, 0x8000, 0 };
hero_seq DNG_seq_tankidle1[6] = { 0, 0x8007, 0x1F, 0x4280, 0x8000, 0 };
hero_seq DNG_seq_tankminelay[6] = { 0, 0x8007, 0x3E, 0x4100, 0x8000, 0 };
hero_seq DNG_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq DNG_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq DNG_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4000, 0x8000, 0 };
hero_seq KON_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4080, 0x8000, 0 };
hero_seq KON_seq_tankvictory[6] = { 0, 0x8007, 0x40, 0x4100, 0x8000, 0 };
hero_seq KON_seq_tankrecoil[6] = { 0, 0x8007, 0x1E, 0x4000, 0x8000, 0 };
hero_seq KON_seq_tankdie[6] = { 0, 0x8007, 0x29, 0x4280, 0x8000, 0 };
hero_seq KON_seq_tankidle1[6] = { 0, 0x8007, 0x2D, 0x4180, 0x8000, 0 };
hero_seq KON_seq_tankminelay[6] = { 0, 0x8007, 0x32, 0x4200, 0x8000, 0 };
hero_seq KON_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4080, 0x8000, 0 };
hero_seq KON_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4080, 0x8000, 0 };
hero_seq KON_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4080, 0x8000, 0 };
hero_seq KAN_seq_tankidle[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq KAN_seq_tankvictory[6] = { 0, 0x8007, 0x50, 0x4000, 0x8000, 0 };
hero_seq KAN_seq_tankrecoil[6] = { 0, 0x8007, 0x1E, 0x4100, 0x8000, 0 };
hero_seq KAN_seq_tankdie[6] = { 0, 0x8007, 0x29, 0x4180, 0x8000, 0 };
hero_seq KAN_seq_tankidle1[6] = { 0, 0x8007, 0x2D, 0x4280, 0x8000, 0 };
hero_seq KAN_seq_tankminelay[6] = { 0, 0x8007, 0x32, 0x4080, 0x8000, 0 };
hero_seq KAN_seq_tankfire1[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq KAN_seq_tankfire2[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
hero_seq KAN_seq_tankfire3[6] = { 0, 0x8007, 0x14, 0x4200, 0x8000, 0 };
s32 TankSwirlyFireAnim[13] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0,
};
s32 TankLaserFireAnim[13] = {
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0,
};
s32 TankFlubberFireAnim[13] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0,
};
s32 TankFlamerFireAnim[26] = {
    0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009,
    0x400A, 0x400B, 0,
};
s32 TankShellFireAnim[13] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0,
};
s32 TankRocketFireAnim[40] = {
    0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C,
    0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C,
    0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C,
    0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0x200C, 0,
};
s32 TankSwampSwirlyTurret[7] = { 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0 };
s32 TankSwampSwirlyFireAnim[13] = {
    0x4380, 0x4381, 0x4382, 0x4383, 0x4384, 0x4385, 0x4386, 0x4387, 0x4388, 0x4389, 0x438A,
    0x438B, 0,
};
s32 TankSwampLaserTurret[7] = { 0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0 };
s32 TankSwampLaserFireAnim[13] = {
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0,
};
s32 TankSwampFlubberTurret[7] = { 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0 };
s32 TankSwampFlubberFireAnim[13] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0,
};
s32 TankSwampFlamerTurret[7] = { 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0 };
s32 TankSwampFlamerFireAnim[13] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0,
};
s32 TotemTargets[16][2] = {
    { 0x400, 0x400 }, { 0x180, 0x700 }, { 0x500, 0xB00 }, { 0x300, 0xE00 }, { 0x200, 0xA00 },
    { 0x900, 0xE00 }, { 0xD00, 0x900 }, { 0xE00, 0xD00 }, { 0xB00, 0x700 }, { 0xB00, 0x200 },
    { 0xF00, 0x700 }, { 0x900, 0x400 }, { 0xC00, 0x180 }, { 0xD00, 0x700 }, { 0x800, 0x200 },
    { 0x300, 0x800 },
};
s32 TotemAnim0D[32] = {
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0x428C, 0x428D, 0x428D, -1, 0x428D, 0x428D, 0x428C, 0x428B, 0x428A, 0x4289, 0x4288,
    0x4287, 0x4286, 0x4285, 0x4284, 0x4283, 0x4282, 0x4281, 0x4280, 0,
};
s32 TotemAnim29[20] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0,
};
s32 TotemAnim25[29] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x4194, 0x4195,
    0x4196, 0x4197, 0x4198, 0x4199, 0x419A, 0x419B, 0,
};
s32 TotemAnim27[30] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, -1, 0x4011, 0x4012, 0x4013, 0x4014, 0x4015,
    0x4016, 0x4017, 0x4018, 0x4019, 0x401A, 0x401B, 0,
};
s32 TotemAnim26[22] = {
    0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420D, 0x420D, 0x420D, 0x420D, 0x420D, 0x420D, 0x420D, 0,
};
u16 TankDesertAiSkill[10] = {
    0x12, 0xF, 0xC, 9, 0x15, 0x19, 0x1A, 0x10, 0x10, 0x1B,
};
u16 TankDesertAiSkillAlt[10] = {
    0x15, 0x12, 0xF, 0xD, 0x1C, 0x1E, 0x1E, 0x10, 0x10, 0x20,
};
u16 TankDesertAiTeamSkill[6] = {
    0x10, 0x18, 0x20, 0x10, 0x18, 0x20,
};
u16 TankDesertAiHumanRoundTweak[12] = {
    0xE, 0x15, 0x1C, 0xC, 0x13, 0x19, 0xA, 0xF, 0x15, 8, 0xD, 0x12,
};
u16 TankDesertAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 2,
};
u16 TankDesertAiHumanTweak[10] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 TankMetalAiSkill[10] = {
    0x16, 0x13, 0x10, 0xD, 0x1A, 0x1C, 0x1E, 0x10, 0x10, 0x1C,
};
u16 TankMetalAiSkillAlt[10] = {
    0x19, 0x16, 0x13, 0x10, 0x1C, 0x1E, 0x20, 0x10, 0x10, 0x20,
};
u16 TankMetalAiTeamSkill[6] = {
    0x10, 0x18, 0x20, 0x10, 0x18, 0x20,
};
u16 TankMetalAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 TankMetalAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 2,
};
u16 TankMetalAiHumanTweak[10] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 TankJungleAiSkill[10] = {
    0x18, 0x15, 0x12, 0xF, 0x15, 0x16, 0x1A, 0x10, 0x10, 0x16,
};
u16 TankJungleAiSkillAlt[10] = {
    0x1A, 0x17, 0x14, 0x11, 0x1C, 0x1D, 0x1C, 0x10, 0x10, 0x20,
};
u16 TankJungleAiTeamSkill[6] = {
    0x10, 0x18, 0x20, 0x10, 0x18, 0x20,
};
u16 TankJungleAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 TankJungleAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 2,
};
u16 TankJungleAiHumanTweak[10] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 TankSwampAiSkill[10] = {
    0x15, 0x12, 0xF, 0xC, 0x15, 0x18, 0x14, 0x10, 0x10, 0x18,
};
u16 TankSwampAiSkillAlt[10] = {
    0x15, 0x12, 0xF, 0xD, 0x15, 0x18, 0x18, 0x10, 0x10, 0x1C,
};
u16 TankSwampAiTeamSkill[6] = {
    1, 0x10, 0x1A, 1, 0x10, 0x1A,
};
u16 TankSwampAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 TankSwampAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 TankSwampAiHumanTweak[10] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 TankBossAiSkill[74] = {
    0x12, 0xF, 0xC, 9, 0x15, 0x15, 0x14, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 TankBossAiSkillAlt[74] = {
    0x15, 0x12, 0xF, 0xD, 0x1C, 0x1C, 0x1C, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 TankBossAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 TankBossAiHumanTweak[10] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
u16 TankBossAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
s32 TankBotTargetOrder[7] = { 0, 1, 2, 3, 0, 1, 2 };
s32 TankBotWanderPoints[9][2] = {
    { 0x200, 0x200 }, { 0x800, 0x200 }, { 0xE00, 0x200 }, { 0x200, 0x800 }, { 0x800, 0x800 },
    { 0xE00, 0x800 }, { 0x200, 0xE00 }, { 0x800, 0xE00 }, { 0xE00, 0xE00 },
};
u8 TankDesertMap[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0, 0x73, 0x93, 0, 0xD3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x37, 0, 0x77, 0x97, 0, 0xD7, 0, 0, 0x39, 0, 0x79, 0x99, 0, 0xD9, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x3D, 0, 0x7D, 0x9D, 0, 0xDD, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 TankDesertMapAlt[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0, 0x73, 0x93, 0, 0xD3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x37, 0, 0, 0, 0, 0xD7, 0, 0, 0x39, 0, 0, 0, 0, 0xD9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3D,
    0, 0x7D, 0x9D, 0, 0xDD, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 TankJungleMap[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x77,
    0x97, 0, 0, 0, 0, 0, 0, 0x79, 0x99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 TankMetalMap[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x33, 0, 0x73, 0, 0xB3, 0, 0xF3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0x37, 0, 0x77, 0, 0xB7, 0, 0xF7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x3B, 0, 0x7B, 0,
    0xBB, 0, 0xFB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
u8 TankBossMap[72] = {0};
u8 TankSwampMap[64] = {0};
s32 TankMapNas[64] = {0};
s32 TankWaterBob[16] = { -3, -2, -1, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, -1, -2, -3 };
u32 MineBallFrames[8] = { 0x201A, 0x201B, 0x201C, 0x201D, 0x201E, 0x201F, 0x2020, 0x2021 };
RockTblEnt ShrapnelFrames[3] = { { 0x2001, 0 }, { 0x2002, 0 }, { 0x2003, 0 } };
s32 PickupSpots[82] = {
    0x100, 0x100, 0x100, 0x300, 0x100, 0x500, 0x100, 0x700, 0x100, 0x900, 0x100, 0xB00, 0x100,
    0xD00, 0x500, 0x100, 0x500, 0x300, 0x500, 0x500, 0x500, 0x700, 0x500, 0x900, 0x500, 0xB00,
    0x500, 0xD00, 0x900, 0x100, 0x900, 0x300, 0x900, 0x500, 0x900, 0x700, 0x900, 0x900, 0x900,
    0xB00, 0x900, 0xD00, 0xD00, 0x100, 0xD00, 0x300, 0xD00, 0x500, 0xD00, 0x700, 0xD00, 0x900,
    0xD00, 0xB00, 0xD00, 0xD00, 0x1100, 0x100, 0x1100, 0x300, 0x1100, 0x500, 0x100, 0x700,
    0x100, 0x900, 0x100, 0xB00, 0x100, 0xD00, 0x100, 0x300, 0x100, 0x700, 0x100, 0xB00, 0x1100,
    0x300, 0x1100, 0x700, 0x1100, 0xB00,
};
s32 HelicopterZSwamp[4] = { 0x100, 0x500, 0xB00, 0xF00 };
s32 HelicopterZDesert[4] = { 0x100, 0x500, 0xB00, 0xF00 };
s32 HelicopterZJungle[4] = { 0x100, 0x500, 0xB00, 0xF00 };
s32 HelicopterZMetal[4] = { 0x100, 0x500, 0x900, 0xD00 };
s32 HelicopterZBoss[4] = { 0x300, 0x500, 0x900, 0xD00 };
TankU16Pad HelicopterFrames[33] = {
    { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 }, { 0x4084, 0 }, { 0x4085, 0 },
    { 0x4086, 0 }, { 0x4087, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 },
};
RockTblEnt RockFrames[6] = { { 0x2015, 0 }, { 0x2010, 0 }, { 0x2011, 0 }, { 0x2012, 0 }, { 0x2013, 0 }, { 0x2014, 0 } };
s32 PeriscopeFrames[17] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0,
};
s32 PillarPattern0[29] = {
    0xB4, 2, 1, 0x2014, 1, 2, 0x2012, 2, 5, 0x2016, 1, 4, 0x2013, 6, 1, 0x201E, 7, 2, 0x2021,
    6, 5, 0x2020, 7, 4, 0x2022, 4, 3, 0x201A, -1,
};
s32 PillarPattern1[17] = { 0xB4, 3, 2, 0x2017, 3, 4, 0x2018, 5, 2, 0x201C, 5, 4, 0x201D, 4, 3, 0x201A, -1 };
s32 PillarPattern2[26] = {
    0xB4, 1, 2, 0x2012, 1, 4, 0x2013, 3, 2, 0x2017, 3, 4, 0x2018, 5, 2, 0x201C, 5, 4, 0x201D,
    7, 2, 0x2021, 7, 4, 0x2022, -1,
};
s32 PillarPattern3[29] = {
    0xB4, 2, 1, 0x2014, 4, 1, 0x2019, 6, 1, 0x201E, 2, 3, 0x2015, 4, 3, 0x201A, 6, 3, 0x201F,
    2, 5, 0x2016, 4, 5, 0x201B, 6, 5, 0x2020, -1,
};
s32 *PillarWaves[32] = {
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
    (void *)&PillarPattern2, (void *)&PillarPattern3, (void *)&PillarPattern0, (void *)&PillarPattern1,
};
TankU16Pad PillarCoverFrames[4] = { { 0x4003, 0 }, { 0x4007, 0 }, { 0x400B, 0 }, { 0x400F, 0 } };
s32 TankBossBob[16] = { 0xA, -0x14, 0xA, -0x14, 0xA, 0, 0x14, -0xA, 0x14, -0x14, 0x1E, 0, -0x14, 0x14, -0xA, 0 };
u16 CrashTurretFrames[20] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0, 0,
};
u16 CrashTurretFramesAlt[20] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0, 0,
};
TankPoint TankBossTargets[5] = { { 0x700, 0 }, { 0x700, 0 }, { 0, 0x700 }, { 0, 0x700 }, { 0x1200, 0x700 } };
s32 MoeMoveAnim[21] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0,
};
s32 MoeDieAnim[31] = {
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x4294, 0x4295,
    0x4296, 0x4297, 0x4298, 0x4299, 0x429A, 0x429B, 0x429C, 0x429D, 0,
};
s32 MoeRecoilAnim[31] = {
    0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211, 0x4212, 0x4213, 0x4214, 0x4215,
    0x4216, 0x4217, 0x4218, 0x4219, 0x421A, 0x421B, 0x421C, 0x421D, 0,
};
s32 JoeMoveAnim[21] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0,
};
s32 JoeDieAnim[35] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115,
    0x4116, 0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120,
    0x4121, 0,
};
s32 JoeRecoilAnim[31] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095,
    0x4096, 0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0,
};
u8 TankLevelsGridMapMiss[72] = {0};
u8 TankLevelsGridMapTank[72] = {0};
NAS *PU_TA_list[3] = {0};
TA_player TA_players[4] = {0};
/* TankUnused0..4, TankUnusedTable and TankUnusedGrid: TA_InitLevel clears all but the table and
 * nothing reads any of them, in retail or in Tank's copy on any prototype or demo disc back to
 * Aug 23. TankUnusedGrid is a 9 x 7 tile grid, the size of the metal arena's map. */
s32 TankUnused0 = 0;
s32 TankUnused1 = 0;
s32 TankUnused2 = 0;
s32 TankUnused3 = 0;
s32 TankUnused4 = 0;
s32 TankMapWidth = 0;
s32 TankMapHeight = 0;
TA_Bot TankBotAI[4] = {0};
s32 TankUnusedTable[5] = {0};
NAS *TankMines[8] = {0};
s32 TankCountdown = 0;
s32 TankPendingSfx = 0;
EXTengine *TankCountdownEng = 0;
s32 TankPillarSfxDelay = 0;
s32 TankLevelOver = 0;
s32 HelicopterCount = 0;
s32 TankDropDelay = 0;
s32 TankShotBounce = 0;
s32 TankArenaMaxX = 0;
s32 TankArenaMaxZ = 0;
s32 TankUnusedGrid[9][7] = {0};
s32 PillarWave = 0;
NAS *TankBossNas = 0;
EXTengine *TankBossEng = 0;
s32 TankBossStatus = 0;
NAS *PeriscopePuff = 0;
NAS *TotemPole = 0;
NAS *TotemTop = 0;
NAS *TotemBot = 0;
NAS *TankBossCrashTurrets[3] = {0};
NAS *TankBossCortexTurrets[4] = {0};
NAS *TankBossRocketTurrets[2] = {0};
s32 TankBossRocketDelay = 0;
s32 *MoeAnimPtr = 0;
s32 *JoeAnimPtr = 0;
s32 MoeState = 0;
s32 JoeState = 0;
s32 TankBossStartStatus = 0;
s32 TankBossSightedHero = 0;
NAS *TankObjects[8] = {0};
s32 TankBotForceField[4] = {0};

s32 TankBotPickTarget(arg0)
s32 arg0;
{
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && TA_players[i].Energy > 0) {
            sum += AiPlayerSkill[i].quality;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00) && TA_players[i].Energy > 0) {
            sum += AiPlayerSkill[i].quality;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

s32 TankBotPickLinedUpTarget(arg0)
s32 arg0;
{
    s32 sum;
    s32 i;
    s32 j;
    s32 r;
    s32 start;
    s32 mate;
    HeroInfo *h;
    EXTengine *me;
    EXTengine *te;
    s32 ex;

    mate = Heroes[arg0].hero_teammate;
    start = Random(4);
    sum = 0;
    for (i = start; i < start + 4; i++) {
        j = TankBotTargetOrder[i];
        if (j != arg0 && j != mate && !((h = &Heroes[j])->hero_status & 0xFA00) && TA_players[j].Energy > 0) {
            sum += AiPlayerSkill[j].quality;
            if (GameInfo[j + 0x5C] != 0 && (u32)Random(0x100) < 0xD3) {
                continue;
            }
            me = Heroes[arg0].hero_eng;
            te = h->hero_eng;
            if (TankNear(ex = me->unk10.x, te->unk10.x) || TankNear(ex = me->unk10.z, te->unk10.z)) {
                return j;
            }
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = start; i < start + 4; i++) {
        j = TankBotTargetOrder[i];
        if (j != arg0 && j != mate && !(Heroes[j].hero_status & 0xFA00) && TA_players[j].Energy > 0) {
            sum += AiPlayerSkill[j].quality;
            if (r < sum) {
                return j;
            }
        }
    }
    return -1;
}

s32 TankBotTryFire(s32 player, EXTengine *eng, TA_Bot *ai) {
    EXTengine *te;
    EXTengine *me;
    HeroInfo *hb;
    s32 d;
    s32 dir;
    s32 ex;
    s32 ez;
    s32 tx;
    s32 tz;
    s32 dx;
    s32 dz;
    s32 a;
    s32 i;
    s32 t;

    if (HeroesAllOut == 0) {
        i = Random(0x65);
        if (ai->FirePercentChance < (u32)i) {
            return 0;
        }
    }
    do {
        do {
            i = ai->Victim;
        } while (0);
    } while (0);
    if (i >= 0) {
    if (((TankModeView *)GameInfo)->mode == 5) {
        te = Heroes[ai->Victim].hero_eng;
        a = get_angle(eng->unk10.x - te->unk10.x, eng->unk10.z - te->unk10.z);
        if ((u32)(((eng->unk1E - a) & 0xFFF) - 100) < 0xF39) {
            goto ret0;
        }
    } else {
    d = eng->unk1E;
    dir = (d + ((TA_players[player].TurretReqdDir - d) << 20 >> 22)) & 0xFFF;
    if (GetSandbagValueTank(eng->unk10.x + (-(rcossin_tbl[(dir - 0x400) & 0xFFF].unk2 << 9) >> 12),
                      eng->unk10.z + (-(rcossin_tbl[dir].unk2 << 9) >> 12)) != 0) {
        return 0;
    }
    if (((TankGameView *)GameInfo)->w6C != 0 || ((TankModeView *)GameInfo)->mode == 9) {
        hb = Heroes;
        me = hb[hb[player].hero_teammate].hero_eng;
        ex = eng->unk10.x;
        tx = me->unk10.x;
        player = tx - ex;
        if (player < 0) {
            player = -player;
        }
        tz = me->unk10.z;
        ez = eng->unk10.z;
        dz = tz - ez;
        if (dz < 0) {
            dz = -dz;
        }
        player = (player + dz) >> 8;
        if (player == 0) {
            player = 1;
        }
        do {
            do {
                te = hb[ai->Victim].hero_eng;
            } while (0);
        } while (0);
        dx = te->unk10.x - ex;
        if (dx < 0) {
            dx = -dx;
        }
        dz = te->unk10.z - ez;
        if (dz < 0) {
            dz = -dz;
        }
        dx = (dx + dz) >> 8;
        if (dx >= player) {
        t = dir - 0x800;
        a = ((t - get_angle(tx - ex, tz - ez)) << 20) >> 20;
        if (a < 0) {
            a = -a;
        }
        if (a * player < 0x800) {
            return 0;
        }
        }
    }
    }
    ai->AIpad |= 0x8000;
    return 1;
    }
ret0:
    return 0;
}

void TankBotDodgeFrom(s32 player, s32 x, s32 z, EXTengine *eng, TA_Bot *ai) {
    s32 ex;
    s32 ez;
    s32 a;
    s32 r;
    s32 speed;
    s32 px;
    s32 p;

    ex = eng->unk10.x;
    ez = eng->unk10.z;
    a = get_angle(ex - x, ez - z);
    ai->Doing = 2;
    speed = TA_players[player].Speed;
    ai->DoingTimer = (speed / 2 + 0x400) / speed;
    ai->AIpad = DirPadBits[(a >> 9) & 7];
    if (HeroesAllOut != 0) {
        return;
    }
    r = Random(4);
    switch (r) {
    case 0:
    case 1:
        if (ai->AIpad & 0xA0) {
            goto doz;
        }
    dox:
        if (ai->AIpad & 0xA0) {
            break;
        }
        px = eng->unk10.x;
        if (px - 0x400 < 0) {
            goto left;
        }
        if (TankArenaMaxX < px + 0x400) {
            goto right;
        }
        if (r & 1) {
            if (GetSandbagValueMiss(ex - 0x100, ez) != 0) {
                goto left;
            }
        right:
            ai->AIpad |= 0x80;
            break;
        }
        if (GetSandbagValueMiss(ex + 0x100, ez) != 0) {
            goto right;
        }
    left:
        ai->AIpad |= 0x20;
        break;
    case 2:
    case 3:
        if (ai->AIpad & 0x50) {
            goto dox;
        }
    doz:
        if (ai->AIpad & 0x50) {
            break;
        }
        p = eng->unk10.z;
        if (TankArenaMaxZ < p + 0x400) {
            goto up;
        }
        if (p - 0x400 < 0) {
            goto down;
        }
        if (r & 1) {
            if (GetSandbagValueMiss(ex, ez + 0x100) != 0) {
                goto up;
            }
        down:
            ai->AIpad |= 0x10;
            break;
        }
        if (GetSandbagValueMiss(ex, ez - 0x100) != 0) {
            goto down;
        }
    up:
        ai->AIpad |= 0x40;
        break;
    }
}

s32 TankBotAvoidMines(s32 player, EXTengine *eng, TA_Bot *ai) {
    NAS *nas;
    EXTengine *te;
    s32 i;
    s32 r;
    u16 type;
    NAS *cur;
    s32 ex;

    if (HeroesAllOut != 0) {
        return 0;
    }
    if ((u32)Random(AiPlayerSkill[player].level + 1) < 8) {
        return 0;
    }
    if (ai->Doing == 2) {
        return 0;
    }
    cur = ai->pMine;
    if (cur != NULL) {
        nas = cur;
        type = nas->Type;
        if ((type != TankASTIndex(0x1303)) | (nas->NASid == 0)) {
            ai->pMine = NULL;
            return 0;
        }
        te = nas->Engine;
        if (TankNear(ex = eng->unk10.x, te->unk10.x) || TankNear(ex = eng->unk10.z, te->unk10.z)) {
            goto hit;
        }
        return 0;
    }
    if (++ai->nMine >= 8) {
        ai->nMine = 0;
    }
    nas = TankMines[ai->nMine];
    if (nas == NULL) {
        return 0;
    }
    te = nas->Engine;
    if (TankNear(ex = eng->unk10.x, te->unk10.x) || TankNear(ex = eng->unk10.z, te->unk10.z)) {
    hit:
        ai->pMine = nas;
        TankBotDodgeFrom(player, te->unk10.x, te->unk10.z, eng, ai);
        return 1;
    }
    return 0;
}

void TankBotPlanSteerMove(s32 player, EXTengine *eng, TA_Bot *ai) {
    NAS **pp;
    NAS *nas;
    NAS *tgt;
    EXTengine *te;
    EXTengine *e;
    s32 j;
    s32 i;
    s32 ax;
    s32 az;
    s32 n;
    s32 sx;
    s32 sz;
    s32 ex;
    s32 ez;
    s32 d;
    s32 r;

    switch (GameMode) {
    case 6:
    case 8:
        r = TankBotPickLinedUpTarget(player);
        ai->Victim = r;
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = TankBotPickTarget(player);
        ai->Victim = r;
        break;
    }
    if (r < 0) {
        goto avg;
    }
    if (TA_players[player].TurretFiring < 0) {
        switch (Random(8)) {
        case 0:
        default:
            if (HeroesAllOut == 0 && (u32)Random(AiPlayerSkill[player].level + 1) < 13) {
                ai->DoingTimer = Random(30) + 15;
                ai->AIpad = 0;
                ai->Doing = 0;
                return;
            }
            goto retry;
        case 1:
        case 2:
        case 3:
        case 4:
            goto retry;
        case 5:
        case 6:
            goto avg;
        case 7:
            break;
        }
    }
find:
    j = 0;
    pp = PU_TA_list;
    ex = eng->unk10.x;
    ez = eng->unk10.z;
    for (; j < 3; j++, pp++) {
        nas = *pp;
        if (nas != NULL) {
            te = nas->Engine;
            d = ex - te->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (ez - te->unk10.z) < 0x600) {
                tgt = nas;
                goto found;
            }
        }
    }
    tgt = NULL;
found:
    if (tgt != NULL) {
        e = tgt->Engine;
        ai->TargetX = e->unk10.x;
        ai->TargetZ = e->unk10.z;
    } else {
        r = Random(9);
        ai->TargetX = TankBotWanderPoints[r][0];
        ai->TargetZ = TankBotWanderPoints[r][1];
    }
    ai->Doing = 4;
    ai->DoingTimer = 0x5A;
    return;

avg:
    n = 0;
    sz = 0;
    sx = 0;
    for (i = 0; i < 4; i++) {
        if (i != player && !(Heroes[player].hero_status & 0xB000)) {
            n++;
            sx += Heroes[i].hero_eng->unk10.x;
            sz += Heroes[i].hero_eng->unk10.z;
        }
    }
    if (n < 2) {
        if ((u32)Random(100) < 30) {
            goto find;
        }
        goto retry;
    }
    ax = sx / n;
    az = sz / n;
    ai->TargetX = ax < 0x800 ? 0xE00 : 0x200;
    ai->TargetZ = az < 0x800 ? 0xE00 : 0x200;
    ai->Doing = 7;
    ai->DoingTimer = 0xB4;
    return;

retry:
    if (ai->Victim >= 0) {
        ai->Doing = 6;
        ai->DoingTimer = 0x5A;
        return;
    }
    if ((u32)Random(100) < 30) {
        goto find;
    }
    goto avg;
}

void TankBotPlanGridMove(s32 player, EXTengine *eng, TA_Bot *ai) {
    EXTengine *te;
    EXTengine *e;
    NAS *nas;
    NAS **pp;
    NAS *tgt;
    TA_player *ta;
    TA_player *tab;
    TA_player *tab2;
    s32 j;
    s32 ex;
    s32 ez;
    s32 d;
    s32 x;
    s32 z;
    s32 delay;
    s32 speed;
    s32 aspeed;
    s32 sspeed;
    s32 a;
    s32 r;

    switch (GameMode) {
    case 6:
    case 8:
        r = TankBotPickLinedUpTarget(player);
        break;
    case 5:
    case 7:
    case 9:
    default:
        r = TankBotPickTarget(player);
        break;
    }
    ai->Victim = r;
    ai->DoingTimer = 30;
    if (ai->pMine != NULL && TankBotAvoidMines(player, eng, ai) != 0) {
        return;
    }
    switch (Random(8)) {
    case 0:
        if (HeroesAllOut == 0 && (u32)Random(AiPlayerSkill[player].level + 1) < 13) {
            ai->DoingTimer = Random(30) + 15;
            ai->AIpad = 0;
            ai->Doing = 0;
            return;
        }
        goto chase;
    case 1:
    find:
        j = 0;
        pp = PU_TA_list;
        ex = eng->unk10.x;
        ez = eng->unk10.z;
        for (; j < 3; j++, pp++) {
            nas = *pp;
            if (nas != NULL) {
                te = nas->Engine;
                d = ex - te->unk10.x;
                if (d < 0) {
                    d = -d;
                }
                if (d + (ez - te->unk10.z) < 0x600) {
                    tgt = nas;
                    goto found;
                }
            }
        }
        tgt = NULL;
    found:
        if (tgt != NULL) {
            e = tgt->Engine;
            d = e->unk10.x - ex;
            if (d > 0x70) {
                goto px;
            }
            if (d < -0x70) {
                goto nx;
            }
            d = e->unk10.z - ez;
            if (d > 0x70) {
                goto pz;
            }
            if (d < -0x70) {
                goto nz;
            }
        }
        speed = TA_players[player].Speed;
        ai->Doing = 5;
        ai->DoingTimer = (speed / 2 + 0x200) / speed;
        for (;;) {
            switch (Random(4)) {
            case 0:
            px:
                x = ex + 0x200;
                if (TankArenaMaxX < x || GetSandbagValueMiss(x, ez) != 0) {
                    continue;
                }
                ai->AIpad = 0x20;
                return;
            case 1:
            nx:
                x = ex - 0x200;
                if (x < 0 || GetSandbagValueMiss(x, ez) != 0) {
                    continue;
                }
                ai->AIpad = 0x80;
                return;
            case 2:
            pz:
                z = ez + 0x200;
                if (TankArenaMaxZ < z || GetSandbagValueMiss(ex, z) != 0) {
                    continue;
                }
                ai->AIpad = 0x10;
                return;
            case 3:
            nz:
                z = ez - 0x200;
                if (z < 0 || GetSandbagValueMiss(ex, z) != 0) {
                    continue;
                }
                ai->AIpad = 0x40;
                return;
            }
            return;
        }
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    chase:
        if (ai->Victim < 0) {
            goto find;
        }
        tab = TA_players;
        ta = &tab[player];
        delay = ta->MainWeaponFireDelay;
        if (delay < 5) {
        aim:
            e = Heroes[ai->Victim].hero_eng;
            a = get_angle(e->unk10.x - eng->unk10.x, e->unk10.z - eng->unk10.z);
            ai->Doing = 4;
            aspeed = TA_players[player].Speed;
            ai->DoingTimer = (aspeed / 2 + 0x200) / aspeed;
            ai->AIpad = DirPadBits[(a >> 9) & 7];
            return;
        }
        if (HeroesAllOut != 0) {
            if (delay != 0) {
                ta->MainWeaponFireDelay = delay - 1;
            }
            if ((u32)Random(0x100) > 0x40) {
                goto aim;
            }
        }
        e = Heroes[ai->Victim].hero_eng;
        TankBotDodgeFrom(player, e->unk10.x, e->unk10.z, eng, ai);
        return;
    case 2:
        tab2 = TA_players;
        ta = &tab2[player];
        if (ta->SubWeaponRounds <= 0) {
            goto chase;
        }
        sspeed = ta->Speed;
        ai->AIpad |= 0x2000;
        ai->DoingTimer = (sspeed / 2 + 0x200) / sspeed;
        return;
    }
}

void TankBotSteer(s32 player) {
    TA_Bot *ai;
    EXTengine *eng;
    EXTengine *te;
    s32 buttons;
    s32 d;
    s32 a;

    eng = Heroes[player].hero_eng;
    ai = &TankBotAI[player];
    if (ai->FirePercentChance == 0) {
        ai->FirePercentChance = AiPlayerSkill[player].level * 0x55 / 32 + 5;
    }
    if (--ai->DoingTimer <= 0) {
        TankBotPlanSteerMove(player, eng, ai);
    }
    buttons = 0;
    switch (ai->Doing) {
    case 4:
    case 7:
        d = eng->unk10.x - ai->TargetX;
        if ((u32)(d + 99) < 199) {
            ai->DoingTimer = 0;
        }
        a = get_angle(d, eng->unk10.z - ai->TargetZ) - eng->unk1E;
        a = (a << 20) >> 20;
        if (a > 40) {
            buttons = 0x820;
        }
        if (a < -40) {
            buttons = 0x880;
        }
        break;
    case 6:
        te = Heroes[ai->Victim].hero_eng;
        a = get_angle(eng->unk10.x - te->unk10.x, eng->unk10.z - te->unk10.z) - eng->unk1E;
        a = (a << 20) >> 20;
        if (a > 70) {
            buttons = 0x20;
        }
        if (a < -70) {
            buttons = 0x80;
        }
        break;
    }
    ai->AIpad &= ~0x8000;
    TankBotTryFire(player, eng, ai);
    if (ai->AIpad & 0x8000) {
        buttons |= 0x8000;
    }
    if (TankBotForceField[player] != 0) {
        buttons |= 0x4000;
        TankBotForceField[player] = 0;
    }
    set_joypad((u8 *)&Pads[player], buttons);
}

void TA_HeroBot(u32 player) {
    TA_Bot *ai;
    EXTengine *eng;
    EXTengine *te;
    EXTengine *tgt;
    NAS *nas;
    s32 i;
    s32 d;
    s32 ex;
    s32 ez;
    u8 *g;

    g = GameInfo;
    if (*(s32 *)(g + 0x10) & 0x04000000) {
        return;
    }
    if (((TankModeView *)g)->mode == 5) {
        TankBotSteer(player);
        return;
    }
    eng = Heroes[player].hero->Engine;
    ai = &TankBotAI[player];
    if (ai->FirePercentChance == 0) {
        ai->FirePercentChance = AiPlayerSkill[player].level * 0x55 / 32 + 5;
    }
    if (GameMode != 5 && ai->Victim >= 0) {
        tgt = Heroes[ai->Victim].hero_eng;
        TA_players[player].TurretReqdDir =
            (get_angle(tgt->unk10.x - eng->unk10.x, tgt->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
    }
    if (ai->Doing == 5 && !(eng->unk10.x & 0x1FF) && !(eng->unk10.z & 0x1FF)) {
        TankBotPlanGridMove(player, eng, ai);
    }
    if (--ai->DoingTimer <= 0) {
        TankBotPlanGridMove(player, eng, ai);
    } else {
        TankBotAvoidMines(player, eng, ai);
        TankBotTryFire(player, eng, ai);
    }
    if (HeroesAllOut == 0 && (u32)Random(AiPlayerSkill[player].level + 1) >= 9) {
        ex = eng->unk10.x;
        ez = eng->unk10.z;
        for (i = 0; i < 8; i++) {
            nas = TankMines[i];
            if (nas != NULL) {
                te = nas->Engine;
                d = ex - te->unk10.x;
                if (d > 250) {
                    if (d < 500) {
                        ai->AIpad &= ~0x80;
                    }
                } else if (d < -250 && d > -500) {
                    ai->AIpad &= ~0x20;
                }
                d = ez - te->unk10.z;
                if (d > 250) {
                    if (d < 500) {
                        ai->AIpad &= ~0x40;
                    }
                } else if (d < -250 && d > -500) {
                    ai->AIpad &= ~0x10;
                }
            }
        }
    }
    set_joypad((u8 *)&Pads[player], ai->AIpad);
}

void TA_PreInitLevel(void) {
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void **p5;
    void *t;
    TankGameView *g;

    HeroHooks.HeroModels = TankHeroModels;
    HeroHooks.HeroBot = TA_HeroBot;
    HeroHooks.InitHero = (s32)TA_InitHero;
    HeroHooks.KeepInBounds = TA_KeepInBounds;
    HeroHooks.VictorySeqs = TankVictorySeqs;
    HeroHooks.CreateShadow = TA_CreateShadow;
    HeroHooks.PreMoveHero = TA_PreMoveHero;
    HeroHooks.MoveHero = TA_MoveHero;
    switch (GameMode) {
    case 6:
    default:
        AiBotTweakTable = TankDesertAiBotTweak;
        AiHumanTweakTable = TankDesertAiHumanTweak;
        AiHumanRoundTweakTable = TankDesertAiHumanRoundTweak;
        p = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = TankDesertAiSkillAlt;
        } else {
            t = TankDesertAiSkill;
        }
        *p = t;
        AiTeamSkillTable = TankDesertAiTeamSkill;
        return;
    case 8:
        AiBotTweakTable = TankMetalAiBotTweak;
        AiHumanTweakTable = TankMetalAiHumanTweak;
        AiHumanRoundTweakTable = TankMetalAiHumanRoundTweak;
        p2 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = TankMetalAiSkillAlt;
        } else {
            t = TankMetalAiSkill;
        }
        *p2 = t;
        AiTeamSkillTable = TankMetalAiTeamSkill;
        return;
    case 7:
        AiBotTweakTable = TankJungleAiBotTweak;
        AiHumanTweakTable = TankJungleAiHumanTweak;
        AiHumanRoundTweakTable = TankJungleAiHumanRoundTweak;
        p3 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = TankJungleAiSkillAlt;
        } else {
            t = TankJungleAiSkill;
        }
        *p3 = t;
        AiTeamSkillTable = TankJungleAiTeamSkill;
        return;
    case 5:
        AiBotTweakTable = TankSwampAiBotTweak;
        AiHumanTweakTable = TankSwampAiHumanTweak;
        AiHumanRoundTweakTable = TankSwampAiHumanRoundTweak;
        p4 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = TankSwampAiSkillAlt;
        } else {
            t = TankSwampAiSkill;
        }
        *p4 = t;
        AiTeamSkillTable = TankSwampAiTeamSkill;
        return;
    case 9:
        AiBotTweakTable = TankBossAiBotTweak;
        AiHumanTweakTable = TankBossAiHumanTweak;
        AiHumanRoundTweakTable = TankBossAiHumanRoundTweak;
        p5 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = TankBossAiSkillAlt;
        } else {
            t = TankBossAiSkill;
        }
        *p5 = t;
        AiTeamSkillTable = TankSwampAiTeamSkill;
        Heroes[2].hero_name = 9;
        Heroes[3].hero_name = 9;
        g = (TankGameView *)GameInfo;
        g->flags |= 0x100;
        if (g->w6C == 0) {
            Heroes[1].hero_name = 9;
            g->b51 = 9;
        }
        g->b52 = 9;
        g->b53 = 9;
        return;
    }
}

void TA_InitLevel(void) {
    TankSetupView *g;
    u8 *src1;
    u8 *src2;
    u8 *p;
    u8 *q;
    s32 i;
    s32 j;
    s32 k;

    TankUnused4 = 0;
    TankUnused3 = 0;
    TankUnused2 = 0;
    TankUnused1 = 0;
    TankUnused0 = 0;
    for (j = 7; j >= 0; j--) {
        TankObjects[j] = NULL;
    }
    TankLevelOver = 0;
    TankBossSightedHero = -1;
    TankCountdown = 0;
    TankPillarSfxDelay = 0;
    World_floor_level = 0;
    Init_Clock(0x140, 0x1E, 0);
    g = (TankSetupView *)GameInfo;
    if (g->flags & 0x40) {
        if (g->w6C != 0) {
            switch (g->mode) {
            case 6:
                Set_Clock(0x30, 0x81C0);
                break;
            case 8:
                Set_Clock(0x30, 0x81C0);
                break;
            case 7:
                Set_Clock(0x25, 0x81C0);
                break;
            case 5:
                Set_Clock(0x25, 0x81C0);
                break;
            case 9:
                Clock_Off();
                break;
            }
        } else {
            switch (g->mode) {
            case 6:
                Set_Clock(0x40, 0x81C0);
                break;
            case 8:
                Set_Clock(0x40, 0x81C0);
                break;
            case 7:
                Set_Clock(0x30, 0x81C0);
                break;
            case 5:
                Set_Clock(0x40, 0x81C0);
                break;
            case 9:
                Clock_Off();
                break;
            }
        }
    } else if (g->mode == 9) {
        Clock_Off();
    } else {
        Set_Clock(0x130, 0x81C0);
    }
    if (GameMode == 5) {
        dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_TankVehicle]);
    } else {
        dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_TankObjects2]);
    }
    if (GameMode == 7) {
        dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_ArenaTankJungleObjects]);
    }
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_TankObjects]);
    dbase_load_all_mesh_anims((s32 *)(((u8 *)&HeroModelBank[DB_TankObjects]) - 0x658));
    dbase_load_all_mesh_anims(((s32 *)MapScene->dbase));
    GameViewport.ofx = 0x140;
    GameViewport.ofy = 0x15E;
    CAM5F->eye.x = 0;
    CAM5F->eye.z = 0xE98;
    CAM5F->xform.angle.vx = 0x200;
    CAM5F->dist = 0x200;
    CAM5F->eye.y = 0;
    CAM5F->xform.angle.vy = 0;
    CAM5F->xform.angle.vz = 0;
    CAM5F->at.y = 0x90;
    for (i = 0; i < 4; i++) {
        SetHeroAction(i, 0x6C);
    }
    for (i = 2; i >= 0; i--) {
        PU_TA_list[i] = NULL;
    }
    for (i = 7; i >= 0; i--) {
        TankMines[i] = NULL;
    }
    switch (GameMode) {
    default:
    case 5:
        src1 = src2 = TankSwampMap;
        TankMapWidth = 9;
        TankMapHeight = 8;
        CAM5F->at.x = 0x930;
        CAM5F->at.z = 0x1C0;
        TankArenaMaxX = 0x1100;
        TankArenaMaxZ = 0xF00;
        PlaySfx(0x50F, NULL, NULL, 0x1000, 0x1E00, 0);
        break;
    case 6:
        src2 = TankDesertMap;
        TankMapWidth = 8;
        TankMapHeight = 8;
        CAM5F->at.x = 0x800;
        CAM5F->at.z = 0x2A0;
        TankArenaMaxX = 0xF00;
        TankArenaMaxZ = 0xF00;
        if (GameLevelFlags & 0x80) {
            CreateNAS(0x800, -0x3C, 0x800, 0x1318, 0x11);
            src1 = TankDesertMapAlt;
        } else {
            src1 = src2;
        }
        break;
    case 7:
        InitAmbience(&TankJungleAmbience);
        CreateNAS(0x800, 0, 0x800, 0x131A, 0x11);
        src1 = src2 = TankJungleMap;
        TankMapWidth = 8;
        TankMapHeight = 8;
        CAM5F->at.x = 0x800;
        CAM5F->at.z = 0x220;
        CAM5F->xform.angle.vx = 0x1EA;
        CAM5F->at.y = 0;
        TankArenaMaxX = 0xF00;
        TankArenaMaxZ = 0xF00;
        break;
    case 8:
        TankMapWidth = 9;
        TankMapHeight = 7;
        CAM5F->at.x = 0x930;
        CAM5F->at.z = 0x1C0;
        PillarWave = 0;
        CreateNAS(0, 0, 0, 0x1310, 0x11);
        CreateNAS(0, 0, 0, 0x1309, 0x11);
        src1 = src2 = TankMetalMap;
        TankArenaMaxX = 0x1100;
        TankArenaMaxZ = 0xD00;
        break;
    case 9:
        g = (TankSetupView *)GameInfo;
        if (g->w6C == 0) {
            g->b51 = 9;
        }
        g->b52 = 9;
        g->b53 = 9;
        Heroes[2].hero_name = 9;
        Heroes[3].hero_name = 9;
        if (g->w6C == 0) {
            Heroes[1].hero_name = 9;
        }
        CAM5F->xform.angle.vx = 0x192;
        TankMapWidth = 9;
        TankMapHeight = 8;
        CAM5F->at.x = 0x930;
        CAM5F->at.y = -0x70;
        CAM5F->at.z = 0x1C0;
        TankArenaMaxX = 0x1100;
        TankArenaMaxZ = 0xF00;
        CreateNAS(0x600, 0, 0x900, 0x131F, 0x11);
        CreateNAS(0xC00, 0, 0x900, 0x131E, 0x11);
        CreateNAS(0x900, 0x7D0, 0x10E0, 0x1313, 0x11);
        src1 = src2 = TankBossMap;
        MoeState = 0;
        JoeState = 0;
        break;
    }
    p = TankLevelsGridMapMiss;
    q = TankLevelsGridMapTank;
    for (i = 0; i < 0x48; i++) {
        *p++ = *src2++;
        *q++ = *src1++;
    }
    for (k = 0; k < 7; k++) {
        for (j = 0; j < 9; j++) {
            TankUnusedGrid[j][k] = 0;
        }
    }
    TankPendingSfx = 0;
    TankDropDelay = 0x50;
    TankBossRocketDelay = 0x1E0 / (AiPlayerSkill[3].level + 1);
}

void TA_UpdateLevel(void) {
    s32 i;
    TA_player *ta;
    HeroInfo *hero;
    EXTengine *eng;
    u8 *base;
    u8 *b2;
    s32 e;
    s32 j;

    if (GameMode == 8) {
        TankDropDelay--;
    }
    for (i = 0; i < 4; i++) {
        ta = &TA_players[i];
        eng = Heroes[i].hero_eng;
        e = ta->Energy;
        ta->PosX = eng->unk10.x;
        ta->PosZ = eng->unk10.z;
        if (e <= 0) {
            SetHullSFX(i, eng, 0);
            SetTurretSFX(i, (s32)eng, 0);
            SetFireBreatherSFX(i, (s32)eng, 0);
        }
    }
    if (TankCountdown <= 0) {
        if (TankCountdown != 0) {
            PlaySfx(0x4B9, NULL, &TankCountdownEng->unk10, 0x1000, 0x1E00, 0);
            TankCountdown = 60;
        }
    } else {
        TankCountdown--;
    }
    if (TankPillarSfxDelay > 0) {
        if (--TankPillarSfxDelay == 0) {
            PlaySfx(0x501, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (TankPendingSfx != 0) {
        PlaySfx(TankPendingSfx, NULL, NULL, 0x1000, 0x1E00, 0);
        TankPendingSfx = 0;
    }
    base = (u8 *)HeroModelBank;
    if (TANK_MODEL_ON(base, 56)) {
        texture_update_anims((obj_textureset *)(TANK_MODEL(56) + 0x10));
    }
    if (GameMode == 5 && TANK_MODEL_ON(base, 60)) {
        texture_update_anims((obj_textureset *)(TANK_MODEL(60) + 0x10));
    }
    if (GameMode == 9 && TANK_MODEL_ON(b2 = (u8 *)HeroModelBank, 57)) {
        texture_update_anims((obj_textureset *)(b2 + 57 * 0x38 + 0x10));
    }
    if (HeroesAllOut == 0) {
        HeroesAllOut = 1;
        for (j = 0; j < 4; j++) {
            if (!(Heroes[j].hero_status & 0xFA00)) {
                if (GameInfo[j + 0x5C] == 0) {
                    HeroesAllOut = 0;
                }
                if (((TankGameView *)GameInfo)->w6C != 0 && GameInfo[Heroes[j].hero_teammate + 0x5C] == 0) {
                    HeroesAllOut = 0;
                }
            }
        }
        if (HeroesAllOut != 0) {
            for (j = 0; j < 4; j++) {
                AiPlayerSkill[j].quality = 0x10;
                AiPlayerSkill[j].level = 0x1E;
            }
        }
    }
}

void FinishTankLevel(void) {
    s32 i;
    s32 best;
    s32 bestHealth;
    s32 h;

    bestHealth = 0;
    best = -1;
    i = bestHealth;
    for (; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xFA00)) {
            h = Heroes[i].hero_health;
            if (bestHealth < h) {
                bestHealth = h;
                best = i;
            }
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00) && i != best) {
                TA_players[i].RequestQuickFinish = 1;
            }
        }
    }
}

void DamageTankHero(s32 player, s32 damage) {
    HeroInfo *h;
    HeroInfo *hb;
    EXTengine *eng;
    TA_player *ta;
    u8 *g;
    s32 t;
    s32 u;

    if (TankLevelOver != 0) {
        return;
    }
    if (Heroes[player].hero_immunity != 0) {
        return;
    }
    if (Heroes[player].hero_status & 0xFA00) {
        return;
    }
    g = GameInfo;
    if (((TankModeView *)g)->mode == 9 && (BossState.flags & 0x8000)) {
        return;
    }
    if (damage < 400) {
        ((void (*)(s32, s32, s32))pad_rumble_big)(player, 0xA, 0xF0);
        if (HeroesAllOut != 0) {
            damage *= 3;
        } else if (g[player + 0x5C] != 0) {
            t = (0x20 - AiPlayerSkill[player].level) * 150;
            u = t / 32; /* dead: keeps t live for the divide's copy */
            t = t / 32 + 50;
            damage = damage * t / 100;
        }
    }
    hb = Heroes;
    h = &hb[player];
    eng = h->hero_eng;
    if (h->hero_mode == 0x69) {
        return;
    }
    h->hero->UserInfo.status = 0x20;
    ta = &TA_players[player];
    if (GameMode == 9 && (u32)(player - 2) < 2) {
        SetMoeJoeMode(player, 0x69);
    } else {
        SetHeroAction(player, 0x69);
    }
    if (GameInfo[player + 0x5C] != 0 || (((TankGameView *)GameInfo)->w4 & 0x80000001) != 0x80000001) {
        ta->Energy -= damage;
    }
    if (ta->Energy <= 0) {
        SetHullSFX(player, eng, 0);
        SetTurretSFX(player, (s32)eng, 0);
        SetFireBreatherSFX(player, (s32)eng, 0);
        ta->Energy = 0;
        if ((((TankGameView *)GameInfo)->w4 & 0x80000002) == 0x80000002) {
            ta->Energy = 2;
        } else {
            Heroes[player].hero_status |= 0x2000;
        }
    }
    Heroes[player].hero_health = ta->Energy;
}

void TA_InitHero(u32 player) {
    HeroInfo *h;
    HeroInfo *hh;
    HeroInfo *hb;
    EXTengine *eng;
    subengine *sub;
    TA_player *ta;
    TA_Bot *bot;
    EngPtr3 *pp;
    EngPtr3 *pb;
    PanelPrim *pnl;
    NAS *nas;
    u8 *g;
    u8 *g1;
    u8 *g2;
    s32 n;

    ((PadButtons *)Pads)[player].unk50 = 0;
    ((PadButtons *)Pads)[player].unk54 = 0;
    hb = Heroes;
    h = &hb[player];
    if (h->hero_name == 9) {
        return;
    }
    g = GameInfo;
    if (g[player + 0x50] == 9) {
        return;
    }
    eng = h->hero_eng;
    eng->unk10.x = (eng->unk10.x + 0x80) & ~0xFF;
    eng->unk10.z = (eng->unk10.z + 0x80) & ~0xFF;
    SetHeroAction(player, 0x6C);
    eng_add_subs(eng, 2);
    sub = eng->sub;
    eng->unk5D = 1;
    eng->unk1E = 0x400;
    sub->unk51 = 1;
    if (((TankModeView *)g)->mode == 5) {
        sub->unk08 = 0x48;
    }
    sub = sub->next;
    sub->unk51 = 1;
    if (((TankModeView *)g)->mode == 5) {
        sub->u6C.dbase = &HeroModelBank[DB_TankVehicle];
        sub->entity = 0x2005;
        sub->unk08 = -0x100;
    } else {
        sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
        sub->entity = 0x2009;
    }
    sub = sub->next;
    sub->unk51 = 1;
    if (GameMode == 5) {
        sub->u6C.dbase = &HeroModelBank[DB_TankVehicle];
        sub->entity = 0x2001;
    } else {
        sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
        sub->entity = 0x4300;
    }
    SetTankHeroIdle(player);
    switch ((u32)player) {
    case 0:
        TA_players[player].MoveDir = 2;
        TA_players[player].FaceDir = 2;
        TA_players[player].RunnerMoving = 2;
        TA_players[player].RunnerFacing = 2;
        break;
    case 1:
        TA_players[player].MoveDir = 3;
        TA_players[player].FaceDir = 3;
        TA_players[player].RunnerMoving = 3;
        TA_players[player].RunnerFacing = 3;
        break;
    case 2:
        TA_players[player].MoveDir = 0;
        TA_players[player].FaceDir = 0;
        TA_players[player].RunnerMoving = 0;
        TA_players[player].RunnerFacing = 0;
        break;
    case 3:
        TA_players[player].MoveDir = 1;
        TA_players[player].FaceDir = 1;
        TA_players[player].RunnerMoving = 1;
        TA_players[player].RunnerFacing = 1;
        break;
    }
    ta = &TA_players[player];
    ta->Speed = TankHeroSpeeds[Heroes[player].hero_name - 1];
    hh = &Heroes[player];
    ta->SpeedX = 0;
    ta->SpeedZ = 0;
    g1 = GameInfo;
    if (((TankModeView *)g1)->mode == 5) {
        ta->Speed = ta->Speed * 3 / 2;
    }
    if (g1[player + 0x5C] != 0) {
        ta->Speed = ta->Speed * (AiPlayerSkill[player].level + 0x54) / 100;
    }
    ta->Energy = 0x63;
    hh->hero_health = ta->Energy;
    ta->PosX = eng->unk10.x;
    ta->PosZ = eng->unk10.z;
    ta->RequestQuickFinish = 0;
    ta->SubWeaponFireDelay = 0;
    if (((TankModeView *)g1)->mode == 5 && (((TankModeView *)g1)->flags & 0x80)) {
        ta->SubWeaponRounds = 0;
    } else {
        ta->SubWeaponRounds = 3;
    }
    ta->FireBreatherSFXDuration = 0;
    ta->TurretSFXDuration = 0;
    ta->TurretReqdDir = -1;
    ta->HangaboutTime = -0x5A;
    ta->IdleAnimCt = Random(0x1E);
    ta->TurretFiring = -1;
    ta->TauntDelay = 0;
    ta->ForceFieldDelay = 0;
    ResetToBasicWeapon(player);
    if (GameInfo[player + 0x5C] != 0) {
        ta->MainWeaponFireDelay = 0x96;
    } else {
        ta->MainWeaponFireDelay = 0;
    }
    ta->MainWeaponNas[0] = NULL;
    n = Heroes[player].hero_name;
    if (n == 0 || n >= 7) {
        ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    } else {
        ta->MainWeaponNas[1] = (NAS *)0xFFEEEEFF;
        ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    }
    if (GameMode == 5) {
        nas = CreateNAS(eng->unk10.x, 0, eng->unk10.z, 0x1320, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = player;
        }
    }
    g2 = GameInfo;
    if (g2[player + 0x5C] != 0) {
        bot = &TankBotAI[player];
        bot->Doing = 0;
        bot->DoingTimer = 0;
        bot->Victim = 0;
        bot->pMine = NULL;
        bot->pMissile = NULL;
        bot->AIpad = 0;
        bot->FirePercentChance = 0;
    }
    pnl = (PanelPrim *)&IconParts[player][5];
    pnl->unk0 |= 0x8000;
    pb = TankHudWeapons;
    pp = &pb[player];
    pp->a = &ta->MainWeaponType;
    pp->b = &ta->MainWeaponRounds;
    pp->c = &ta->SubWeaponRounds;
    if (((TankModeView *)g2)->mode == 5 && g2[player + 0x5C] == 0 && (((TankModeView *)g2)->flags & 0x80)) {
        if (player == 0) {
            nas = CreateNAS(0x880, 0, 0x880, 0x1322, 0x11);
        } else {
            nas = CreateNAS(0x980, 0, 0x880, 0x1322, 0x11);
        }
        if (nas != NULL) {
            nas->UserInfo.user1 = player;
        }
    }
}

void TA_MoveHero(u32 player) {
    HeroInfo *hb;
    HeroInfo *hero;
    EXTengine *eng;
    TA_player *ta;
    NAS *nas;
    s32 x;
    s32 dz;

    hb = Heroes;
    hero = &hb[player];
    eng = hero->hero_eng;
    ta = &TA_players[player];
    AnimateTankTurret(player);
    if (GameMode != 5) {
        x = eng->unk10.x;
        dz = eng->unk10.z - eng->OldXYZ.z;
        if (x != eng->OldXYZ.x || dz != 0) {
            if (ta->HangaboutTime != 0) {
                ta->HangaboutTime--;
            }
            SetHullSFX(player, eng, TankHullSfx[player]);
        } else {
            if (!(hero->hero_status & 0xFA00)) {
                ta->HangaboutTime++;
            }
            SetHullSFX(player, eng, 0);
        }
        if (ta->TurretSFXDuration != 0) {
            if (--ta->TurretSFXDuration == 0) {
                SetTurretSFX(player, (s32)eng, 0);
            } else {
                SetTurretSFX(player, (s32)eng, TankTurretSfx[player]);
            }
        }
    } else {
        if (((PadButtons *)Pads)[player].unk50 & 0x800) {
            SetHullSFX(player, eng, TankHoverSfx[player]);
        } else {
            SetHullSFX(player, eng, 0);
        }
    }
    if (((TankModeView *)GameInfo)->mode == 8 && GameInfo[player + 0x5C] == 0 &&
        (((TankGameView *)GameInfo)->flags & 0x80) && ta->HangaboutTime >= 30 && TankLevelOver == 0 &&
        TankDropDelay <= 0 && (eng->unk10.z > 0x400 || (u32)(eng->unk10.x - 0x301) < 0xDFF)) {
        nas = CreateNAS(eng->unk10.x, -0x8FC, eng->unk10.z, 0x1308, 0x11);
        if (nas != NULL) {
            nas->UserInfo.status = 6;
            TankDropDelay = 60;
            nas->Engine->sub->entity = 0x202B;
            ta->HangaboutTime = 0;
        }
    }
}

void AnimateTank(u32 player) {
    if (GameInfo[player + 0x5C] != 0) {
        if (++TA_players[player].IdleAnimCt >= 0x50 && TankLevelOver == 0) {
            TA_players[player].IdleAnimCt = Random(0x1E);
            SetHeroSeq(player, 0x5A);
            TA_players[player].IdleAnimCt = 0;
            PlaySfx(TankTauntSfx[Heroes[player].hero_name - 1], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00,
                    0);
            return;
        }
    }
    SetHeroSeq(player, 0x59);
}

void ResetToBasicWeapon(s32 player) {
    TA_player *ta;
    subengine *sub;

    ta = &TA_players[player];
    ta->MainWeaponRounds = 999;
    ta->MainWeaponFireDelay = 30;
    sub = Heroes[player].hero_sub->next;
    switch (Heroes[player].hero_name) {
    case 1:
    case 2:
        ta->MainWeaponType = 9;
        if (GameMode == 5) {
            ta->TurretAnimPtr = TankSwampSwirlyTurret;
            sub->next->entity = 0x4300;
        } else {
            sub->entity = 0x2010;
        }
        break;
    case 7:
    case 8:
        ta->MainWeaponType = 8;
        if (GameMode == 5) {
            ta->TurretAnimPtr = TankSwampFlamerTurret;
            sub->next->entity = 0x4100;
        } else {
            sub->entity = 0x2009;
        }
        break;
    default:
        ta->MainWeaponType = 11;
        if (GameMode == 5) {
            ta->TurretAnimPtr = TankSwampLaserTurret;
            sub->next->entity = 0x4200;
        } else {
            sub->entity = 0x2011;
        }
        break;
    case 5:
    case 6:
        ta->MainWeaponType = 10;
        if (GameMode == 5) {
            ta->TurretAnimPtr = TankSwampFlubberTurret;
            sub->next->entity = 0x4000;
        } else {
            sub->entity = 0x2012;
        }
        break;
    }
    if (((TankModeView *)GameInfo)->mode == 5) {
        sub->entity = 0x2005;
    }
    if (((TankModeView *)GameInfo)->mode == 9) {
        ta->MainWeaponRounds = 99999;
        if (GameInfo[player + 0x5C] != 0) {
            ta->MainWeaponType = 11;
            sub->entity = 0x2011;
            return;
        }
        if (TankBossStartStatus == 0x3A) {
            ta->MainWeaponType = 2;
            sub->entity = 0x200B;
            return;
        }
        ta->MainWeaponType = 3;
        sub->entity = 0x2008;
    }
}

void AnimateTankTurret(s32 player) {
    TA_player *ta;
    TA_player *ta2;
    TA_player *tab;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *se;
    EXTengine *se2;
    subengine *sub;
    subengine *sub2;
    NAS *n;
    NAS *puff;
    EXTobject_TA *fux;
    s32 a;
    s32 dx;
    s32 dz;
    s32 pad;
    u16 anim;

    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    sub = Heroes[player].hero_sub->next;
    sub2 = sub->next;
    if (GameMode == 5) {
        pad = ((PadButtons *)Pads)[player].unk50;
        if (pad & 0x800) {
            goto right;
        } else if (pad & 0x200) {
        right:
            sub->entity = 0x2006;
            sub->unk14 -= 0x2A7;
        } else {
            sub->entity = 0x2005;
            sub->unk14 -= 0xB3;
        }
        sub->unk52 = 1;
    }
    if (ta->TurretFiring >= 0) {
        a = *ta->TurretAnimPtr++;
        if (a != 0) {
            if (GameMode == 5) {
                sub2->entity = a;
            } else {
                sub->entity = a;
            }
        } else {
            if (GameMode == 5) {
                switch (ta->MainWeaponType) {
                case 9:
                default:
                    sub2->entity = 0x4300;
                    ta->TurretAnimPtr = TankSwampSwirlyTurret;
                    break;
                case 11:
                    sub2->entity = 0x4200;
                    ta->TurretAnimPtr = TankSwampLaserTurret;
                    break;
                case 10:
                    sub2->entity = 0x4000;
                    ta->TurretAnimPtr = TankSwampFlubberTurret;
                    break;
                case 8:
                    sub2->entity = 0x4100;
                    ta->TurretAnimPtr = TankSwampFlamerTurret;
                    break;
                }
            } else {
                switch (ta->TurretFiring) {
                default:
                    anim = 0x2010;
                    break;
                case 11:
                    anim = 0x2011;
                    break;
                case 10:
                    anim = 0x2012;
                    break;
                case 8:
                    anim = 0x2009;
                    break;
                case 0:
                case 3:
                    anim = 0x2008;
                    break;
                case 2:
                    anim = 0x200B;
                    break;
                }
                sub->entity = anim;
            }
            ta->TurretFiring = -1;
            if (ta->MainWeaponRounds <= 0) {
                ResetToBasicWeapon(player);
            }
        }
        if (sub->entity == 0x4004) {
            e = Heroes[player].hero_eng;
            puff = TankSpawnBit1605Nas(&e->unk10, 0);
            if (puff != NULL) {
                TankPuffSetAngle(puff, eng->unk1E);
            }
            tab = TA_players;
            ta2 = &tab[player];
            if (ta2->MainWeaponType == 3) {
                n = CreateNAS(e->unk10.x, e->unk10.y - 0x12C, e->unk10.z, 0x130B, 0x11);
                if (n != NULL) {
                    fux = AddExt(n, 0x38, 0x10000000);
                    if (fux == NULL) {
                        KillNAS(n);
                        return;
                    }
                    n->FUX = (EXT *)fux;
                    se = n->Engine;
                    dx = -(rcossin_tbl[(e->unk1E - 0x400) & 0xFFF].unk2 * 250);
                    se->unk10.x += dx >> 12;
                    dz = -(rcossin_tbl[(u16)e->unk1E & 0xFFF].unk2 * 250);
                    se->unk10.z += dz >> 12;
                    fux->SpeedX = dx >> 13;
                    fux->SpeedY = -0x5B;
                    fux->SpeedZ = dz >> 13;
                    fux->Owner = player;
                    fux->Status = 1;
                    ta2->MainWeaponRounds--;
                    se2 = n->Engine;
                    PlaySfx(0x4F3, &se2->sfx, &se2->unk10, 0x1000, 0x1E00, 0);
                }
            }
        }
    } else if (GameMode == 5) {
    loop:
        a = *ta->TurretAnimPtr++;
        if (a == 0) {
            switch (ta->MainWeaponType) {
            case 9:
            default:
                ta->TurretAnimPtr = TankSwampSwirlyTurret;
                break;
            case 11:
                ta->TurretAnimPtr = TankSwampLaserTurret;
                break;
            case 10:
                ta->TurretAnimPtr = TankSwampFlubberTurret;
                break;
            case 8:
                ta->TurretAnimPtr = TankSwampFlamerTurret;
                break;
            }
            goto loop;
        }
        sub2->entity = a;
    }
    if (GameMode != 5) {
        if (++sub2->entity == 0x4306) {
            sub2->entity = 0x4300;
        }
    }
}

void UpdateTankHeroWeapons(s32 player) {
    EXTengine *eng;
    TA_player *ta;
    subengine *sub;
    subengine *sub2;
    HeroInfo *h;
    HeroInfo *hb;
    NAS *n;
    NAS *mine;
    NAS *obj;
    EXTobject_TA *fux;
    EXTengine *e;
    s32 d;
    s32 i;
    s32 a;
    s32 dir;
    u32 pad;

    if (Heroes[player].hero_status & 0xFA00) {
        return;
    }
    if (TankLevelOver != 0) {
        return;
    }
    eng = Heroes[player].hero_eng;
    ta = &TA_players[player];
    sub = eng->sub->next;
    sub2 = sub->next;
    if (ta->TurretReqdDir != -1) {
        d = ta->TurretReqdDir - eng->unk1E;
        d = (d << 20) >> 22;
        eng->unk5E = 1;
        eng->unk1E += d;
        sub2->unk52 = 1;
        sub2->unk12 -= d;
        if ((u32)(d + 3) < 7) {
            ta->TurretReqdDir = -1;
        }
    } else if (GameMode != 5) {
        pad = ((PadButtons *)Pads)[player].unk50;
        if (pad & 0xA00) {
            eng->unk5E = 1;
            eng->unk1E = (eng->unk1E + 0x40) & 0xFFF;
            sub2->unk52 = 1;
            sub2->unk12 = (sub2->unk12 - 0x40) & 0xFFF;
            ta->TurretSFXDuration = 2;
        } else if (pad & 0x500) {
            eng->unk5E = 1;
            eng->unk1E = (eng->unk1E - 0x40) & 0xFFF;
            sub2->unk52 = 1;
            sub2->unk12 = (sub2->unk12 + 0x40) & 0xFFF;
            ta->TurretSFXDuration = 2;
        }
    }
    if (ta->MainWeaponFireDelay == 0) {
        if (ta->TurretFiring < 0) {
            for (i = 0; i < 3; i++) {
                if (ta->MainWeaponNas[i] == NULL) {
                    break;
                }
            }
            if (i < 3 && (((PadButtons *)Pads)[player].unk54 & 0x8000)) {
                switch (Random(4)) {
                default:
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                    SetHeroSeq(player, 0x5D);
                    break;
                case 5:
                case 6:
                    SetHeroSeq(player, 0x5E);
                    break;
                case 7:
                    SetHeroSeq(player, 0x5F);
                    break;
                }
                if (ta->MainWeaponType == 3) {
                    ta->MainWeaponFireDelay = 0x3C;
                } else {
                    ta->MainWeaponFireDelay = 0xA;
                }
                switch (ta->MainWeaponType) {
                default:
                case 11:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x113, eng->unk10.z, 0x130C, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 0xB;
                    if (GameMode == 5) {
                        sub->entity = 0x427F;
                        ta->TurretAnimPtr = TankSwampLaserFireAnim;
                    } else {
                        sub->entity = 0x427F;
                        ta->TurretAnimPtr = TankLaserFireAnim;
                    }
                    if (Heroes[player].hero_name == 3) {
                        PlaySfx(0x4CC, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CD, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    n->Engine->unk10.x += (-(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux = (EXTobject_TA *)n->FUX;
                    fux->Owner = player;
                    fux->SpeedX = -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x4A) >> 12;
                    fux->SpeedZ = -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 0x4A) >> 12;
                    if (GameMode == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    obj = TankSpawnBit1605Nas(&n->Engine->unk10, 7);
                    if (obj != NULL) {
                        TankPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 0:
                    sub->entity = 0x3FFF;
                    ta->TurretFiring = 0;
                    ta->TurretAnimPtr = TankShellFireAnim;
                    break;
                case 3:
                    sub->entity = 0x3FFF;
                    ta->TurretFiring = 3;
                    ta->TurretAnimPtr = TankShellFireAnim;
                    break;
                case 8:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xFA, eng->unk10.z, 0x130D, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 8;
                    if (GameMode == 5) {
                        sub->entity = 0x417F;
                        ta->TurretAnimPtr = TankSwampFlamerFireAnim;
                    } else {
                        sub->entity = 0x41FF;
                        ta->TurretAnimPtr = TankFlamerFireAnim;
                    }
                    if (Heroes[player].hero_name == 2) {
                        PlaySfx(0x4CB, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4D0, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x36) >> 12;
                    fux->SpeedZ = -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 0x36) >> 12;
                    if (GameMode == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    ta->FireBreatherSFXDuration = 4;
                    obj = TankSpawnBit1605Nas(&n->Engine->unk10, 8);
                    if (obj != NULL) {
                        TankPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 9:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x104, eng->unk10.z, 0x130F, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 9;
                    if (GameMode == 5) {
                        sub->entity = 0x437F;
                        ta->TurretAnimPtr = TankSwampSwirlyFireAnim;
                    } else {
                        ta->TurretAnimPtr = TankSwirlyFireAnim;
                        sub->entity = 0x417F;
                    }
                    if (Heroes[player].hero_name == 1) {
                        PlaySfx(0x4CA, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CB, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x2C) >> 12;
                    fux->SpeedZ = -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 0x2C) >> 12;
                    if (GameMode == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    obj = TankSpawnBit1605Nas(&n->Engine->unk10, 0xF);
                    if (obj != NULL) {
                        TankPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 10:
                    n = CreateNAS(eng->unk10.x, eng->unk10.y - 0x14A, eng->unk10.z, 0x130E, 0x11);
                    if (n == NULL) {
                        break;
                    }
                    ta->MainWeaponNas[i] = n;
                    ta->TurretFiring = 0xA;
                    if (GameMode == 5) {
                        sub->entity = 0x407F;
                        ta->TurretAnimPtr = TankSwampFlubberFireAnim;
                    } else {
                        sub->entity = 0x40FF;
                        ta->TurretAnimPtr = TankFlubberFireAnim;
                    }
                    if (Heroes[player].hero_name == 5) {
                        PlaySfx(0x4CE, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    } else {
                        PlaySfx(0x4CF, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    }
                    fux = (EXTobject_TA *)n->FUX;
                    n->Engine->unk10.x += (-(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12) + ta->SpeedX;
                    n->Engine->unk10.z += (-(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 400) >> 12) + ta->SpeedZ;
                    n->Engine->unk1E = eng->unk1E;
                    fux->SpeedX = -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 0x2C) >> 12;
                    fux->SpeedZ = -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 0x2C) >> 12;
                    if (GameMode == 5) {
                        BumpSpeedsForSwamp(fux);
                    }
                    fux->Status = 1;
                    fux->Owner = player;
                    obj = TankSpawnBit1605Nas(&n->Engine->unk10, 6);
                    if (obj != NULL) {
                        TankPuffSetAngle(obj, eng->unk1E);
                    }
                    break;
                case 2:
                    dir = eng->unk1E;
                    for (i = 0; i < 2; i++) {
                        n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xE6, eng->unk10.z, 0x1302, 0x11);
                        if (n != NULL) {
                            PlaySfx(0x522, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                            e = n->Engine;
                            a = dir - TankRocketAngles[i];
                            e->unk10.x -= (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 0x19) >> 8;
                            e->unk10.z -= (rcossin_tbl[a & 0xFFF].unk2 * 0x19) >> 8;
                            e->unk1E = eng->unk1E;
                            fux = (EXTobject_TA *)n->FUX;
                            fux->SpeedX = -(rcossin_tbl[(dir - 0x400) & 0xFFF].unk2 * 200) >> 12;
                            fux->SpeedZ = -(rcossin_tbl[dir & 0xFFF].unk2 * 200) >> 12;
                            fux->Status = 1;
                            fux->Owner = player;
                        }
                    }
                    sub->entity = 0x200C;
                    ta->TurretFiring = 2;
                    ta->TurretAnimPtr = TankRocketFireAnim;
                    ta->MainWeaponRounds--;
                    break;
                }
            } else if (GameInfo[player + 0x5C] == 0 &&
                       (((PadButtons *)Pads)[player].unk54 & 0x8000)) {
                PlaySfx(0x4F1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
    } else {
        ta->MainWeaponFireDelay--;
        if (HeroesAllOut != 0 && ta->MainWeaponFireDelay != 0) {
            ta->MainWeaponFireDelay--;
        }
    }
    if (ta->SubWeaponFireDelay != 0) {
        ta->SubWeaponFireDelay--;
    }
    if ((((PadButtons *)Pads)[player].unk54 & 0x2000) &&
        ((TankModeView *)GameInfo)->mode < 10 && ((TankModeView *)GameInfo)->mode >= 5) {
        if (ta->SubWeaponRounds != 0) {
            if (ta->SubWeaponFireDelay == 0) {
                if (((TankModeView *)GameInfo)->mode == 5) {
                    for (i = 0; i < 8; i++) {
                        if (TankMines[i] != NULL && TankMines[i]->UserInfo.user1 == player) {
                            goto subdone;
                        }
                    }
                    for (i = 0; i < 8; i++) {
                        if (TankMines[i] == NULL) {
                            break;
                        }
                    }
                    mine = CreateNAS(eng->unk10.x, eng->unk10.y + 0x14, eng->unk10.z, 0x131D, 0x11);
                    if (mine != NULL) {
                        TankMines[i] = mine;
                        mine->UserInfo.user1 = player;
                    }
                    ta->SubWeaponFireDelay = 0x1E;
                    PlaySfx(0x50A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    PlaySfx(TankMineSfx[player], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    ta->SubWeaponRounds--;
                    SetHeroSeq(player, 0x5B);
                } else {
                    TankTriggerNear(eng->unk10.x, eng->unk10.z, 0x100);
                    CreateNAS(eng->unk10.x, eng->unk10.y + 0x14, eng->unk10.z, 0x1303, 0x11);
                    ta->SubWeaponFireDelay = 0x78;
                    PlaySfx(0x4E9, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    PlaySfx(TankMineSfx[player], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    ta->SubWeaponRounds--;
                    SetHeroSeq(player, 0x5B);
                }
            } else if (GameInfo[player + 0x5C] == 0) {
                PlaySfx(0x4F1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        } else if (GameInfo[player + 0x5C] == 0) {
            PlaySfx(0x4F2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
subdone:
    if (((TankModeView *)GameInfo)->mode == 5) {
        if (ta->ForceFieldDelay != 0) {
            ta->ForceFieldDelay--;
        } else if (((PadButtons *)Pads)[player].unk54 & 0x4000) {
            obj = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1321, 0x11);
            PlaySfx(TankForceFieldSfx[player], NULL, NULL, 0x1000, 0x1E00, 0);
            if (obj != NULL) {
                obj->UserInfo.user1 = player;
            }
            Heroes[player].hero_immunity = 0xA;
            if (GameInfo[player + 0x5C] == 0) {
                ta->ForceFieldDelay = 0x3C;
            } else {
                ta->ForceFieldDelay = (0x20 - AiPlayerSkill[player].level) * 0x3C / 32 + 0xF;
            }
        }
    }
    if (ta->TauntDelay <= 0) {
        if (((PadButtons *)Pads)[player].unk50 & 0x1000) {
            hb = Heroes;
            h = &hb[player];
            if (!(h->hero_status & 0xFA00)) {
                switch (h->hero_mode) {
                case 0x6C:
                case 0x6E:
                case 0x6F:
                    ta->TauntDelay = 0x5A;
                    SetHeroSeq(player, 0x5A);
                    PlaySfx(TankTauntSfx[h->hero_name - 1], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                    break;
                }
            }
        }
    } else {
        ta->TauntDelay--;
    }
}

void SetTankHeroIdle(u32 player) {
    s32 done;
    u8 unused[8];

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        TankLevelOver = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        EXTengine *eng;

        Heroes[player].hero_move->jump_max = 0;
        Heroes[player].hero_move->velocity_enable = 1;
        Heroes[player].hero_idle_count = Random(0x96) + 0x5A;
        SetHeroSeq(player, 0x59);
        eng = Heroes[player].hero_eng;
        if (TA_players[player].HullSFX != 0) {
            TA_players[player].HullSFX = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        if (TA_players[player].TurretSFX != 0) {
            TA_players[player].TurretSFX = 0;
            StopSfx(TA_players[player].TurretSFXHandler);
        }
        if (TA_players[player].FireBreatherSFX != 0) {
            TA_players[player].FireBreatherSFX = 0;
            StopSfx(TA_players[player].FireBreatherSFXHandler);
        }
        AnimateTank(player);
    }
}

void SetTankHeroTurn(u32 player) {
    s32 done;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        TankLevelOver = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        SetHeroSeq(player, 0x59);
        PlaySfx(TankTurnSfx[player], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void TankHeroTurn(u32 player) {
    s32 done;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        TankLevelOver = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        EXTengine *eng;
        TA_player *ta;
        subengine *sub;
        HeroInfo *h;
        HeroInfo *hb;

        UpdateTankHeroWeapons(player);
        if (ProcessHeroAnim(player) != 0) {
            AnimateTank(player);
        }
        hb = Heroes;
        h = &hb[player];
        ta = &TA_players[player];
        eng = h->hero_eng;
        sub = h->hero_sub->next->next;
        sub->unk12 += ta->ModeSpeed;
        sub->unk52 = 1;
        if (--ta->ModeCounter == 0) {
            SetHeroAction(player, 0x6E);
            switch (ta->MoveDir) {
            case 2:
                if (((PadButtons *)Pads)[player].unk50 & 0x10) {
                    eng->unk10.z += ta->Speed;
                }
                break;
            case 0:
                if (((PadButtons *)Pads)[player].unk50 & 0x40) {
                    eng->unk10.z -= ta->Speed;
                }
                break;
            case 3:
                if (((PadButtons *)Pads)[player].unk50 & 0x20) {
                    eng->unk10.x += ta->Speed;
                }
                break;
            case 1:
                if (((PadButtons *)Pads)[player].unk50 & 0x80) {
                    eng->unk10.x -= ta->Speed;
                }
                break;
            }
        }
    }
}

void TankHeroSteerMove(s32 player) {
    TA_player *ta;
    TA_player *o;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *h;
    s32 pad;
    s32 v;
    s32 px;
    s32 pz;
    s32 c;
    s32 x;
    s32 z;
    s32 d;
    s32 lim;
    s32 n;
    s32 bank;
    s32 sp;
    s32 i;
    s32 hit;
    u16 name;

    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    bank = ta->BankAngle;
    pad = ((PadButtons *)Pads)[player].unk50 & 0xF0;
    if (bank != 0) {
        eng->unk5E = 1;
        if (ta->BankAngle > 0) {
            ta->BankAngle -= 10;
            if (ta->BankAngle < 3) {
                ta->BankAngle = 0;
            }
        } else {
            ta->BankAngle += 10;
            if (ta->BankAngle >= -2) {
                ta->BankAngle = 0;
            }
        }
    }
    if (pad != 0) {
        if (pad & 0x20) {
            ta->BankAngle -= 20;
            if (ta->BankAngle < -200) {
                ta->BankAngle = -200;
            }
            eng->unk1E += ta->Speed * 3 / 2;
        }
        if (pad & 0x80) {
            ta->BankAngle += 20;
            if (ta->BankAngle > 200) {
                ta->BankAngle = 200;
            }
            eng->unk1E -= ta->Speed * 3 / 2;
        }
        eng->unk5E = 1;
        eng->unk1E &= 0xFFF;
    }
    eng->unk20 = ta->BankAngle & 0xFFF;
    c = ((PadButtons *)Pads)[player].unk50;
    if (c & 0x800) {
        goto thrust;
    }
    if (c & 0x200) {
    thrust:
        v = ta->Speed;
        px = v * rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2;
        pz = v * rcossin_tbl[(u16)eng->unk1E & 0xFFF].unk2;
        ta->SpeedX += -(px >> 12) >> 2;
        ta->SpeedZ += -(pz >> 12) >> 2;
    }
    sp = ta->SpeedX;
    c = ta->Speed;
    if (c < sp || sp < (c = -c)) {
        ta->SpeedX = c;
    }
    sp = ta->SpeedZ;
    c = ta->Speed;
    if (c < sp || sp < (c = -c)) {
        ta->SpeedZ = c;
    }
    x = eng->unk10.x + ta->SpeedX;
    if (ta->SpeedX > 0) {
        ta->SpeedX--;
        if (TankArenaMaxX < x) {
            x = TankArenaMaxX;
            ta->SpeedX = -ta->SpeedX;
        }
    } else {
        if (ta->SpeedX != 0) {
            ta->SpeedX++;
        }
        if (x < 0x100) {
            x = 0x100;
            ta->SpeedX = -ta->SpeedX;
        }
    }
    z = eng->unk10.z + ta->SpeedZ;
    if (ta->SpeedZ > 0) {
        ta->SpeedZ--;
        if (TankArenaMaxZ < z) {
            z = TankArenaMaxZ;
            ta->SpeedZ = -ta->SpeedZ;
        }
    } else {
        if (ta->SpeedZ != 0) {
            ta->SpeedZ++;
        }
        if (z < 0x100) {
            z = 0x100;
            ta->SpeedZ = -ta->SpeedZ;
        }
    }
    lim = 0x1C0;
    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -0x1C0) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x1C0;
        if (n < d && d < 0x1C0) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (hit < 0) {
        eng->unk10.x = x;
        eng->unk10.z = z;
        return;
    }
    if (TankCountdown == 0) {
        TankCountdownEng = eng;
        TankCountdown = -1;
    }
    o = &TA_players[hit];
    o->SpeedX += ta->SpeedX / 2;
    ta->SpeedX /= 4;
    o->SpeedZ += ta->SpeedZ / 2;
    ta->SpeedZ /= 4;
}

void TankHeroGridMove(s32 player) {
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    HeroInfo *h;
    s32 pad;
    s32 speed;
    s32 dir;
    s32 angle;
    s32 dx;
    s32 dz;
    s32 m;
    s32 a;
    s32 sn;
    s32 cs;
    u16 old;
    s32 x;
    s32 z;
    s32 fx;
    s32 fz;
    s32 lim;
    s32 n;
    s32 d;
    s32 t;
    s32 df;
    u16 name;
    TA_player *ta;
    s32 hit;
    s32 i;

    pad = ((PadButtons *)Pads)[player].unk50;
    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    speed = ta->Speed;
    dir = pad & 0xF0;
    sub = eng->sub->next->next;
    if (dir == 0) {
        return;
    }
    if (((TankModeView *)GameInfo)->mode == 7 && !(((TankModeView *)GameInfo)->flags & 0x80)) {
        dx = TotemTop->Engine->unk10.x - eng->unk10.x;
        dz = TotemTop->Engine->unk10.z - eng->unk10.z;
        if (dx >= -0x15D) {
            if (dx < 0x15E) {
                if (dz >= -0x15D) {
                    if (dz < 0x15E) {
                        if (dx < 0) {
                            m = pad & 0x70;
                        } else {
                            m = pad & 0xD0;
                        }
                        if (dz > 0) {
                            m &= ~0x10;
                        } else {
                            m &= ~0x40;
                        }
                        dir = m;
                    }
                }
            }
        }
        dx = TotemBot->Engine->unk10.x - eng->unk10.x;
        dz = TotemBot->Engine->unk10.z - eng->unk10.z;
        if (dx >= -0x15D) if (dx < 0x15E) if (dz >= -0x15D) if (dz < 0x15E) {
            if (dx < 0) {
                m = dir & ~0x80;
            } else {
                m = dir & ~0x20;
            }
            if (dz > 0) {
                m &= ~0x10;
            } else {
                m &= ~0x40;
            }
            dir = m;
        }
    }
    if (dir == 0) {
        return;
    }
    switch (dir) {
    case 0x40:
        angle = 0x800;
        break;
    case 0x80:
        angle = 0xC00;
        break;
    case 0x20:
        angle = 0x400;
        break;
    case 0x90:
        angle = 0xE00;
        break;
    case 0x30:
        angle = 0x200;
        break;
    case 0xC0:
        angle = 0xA00;
        break;
    case 0x60:
        angle = 0x600;
        break;
    case 0x10:
    default:
        angle = 0;
        break;
    }
loop:
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    df = ((angle - a) << 20) >> 20;
    a = (u16)sub->unk12;
    if ((u32)(df + 0x400) > 0x800) {
        sub->unk12 = (a + 0x800) & 0xFFF;
        goto loop;
    }
    df = df / 4;
    sub->unk12 = (a + df) & 0xFFF;
    sub->unk52 = 1;
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    sn = rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
    cs = rcossin_tbl[a].unk2;
    x = eng->unk10.x + ((speed * sn) >> 12);
    fx = x + ((sn * 256) >> 12);
    z = eng->unk10.z + ((speed * cs) >> 12);
    fz = z + ((cs * 256) >> 12);
    if (GameMode == 7 && (u32)(x - 0x581) < 0x4FF && z >= 0x581 && z < 0xA80) {
        if (x < 0x800) {
            if (x < 0x5C0) {
                x = 0x580;
            } else if (x >= 0xA41) {
                x = 0xA80;
            }
        }
        if (z < 0x800) {
            if (z < 0x5C0) {
                z = 0x580;
            } else if (z >= 0xA41) {
                z = 0xA80;
            }
        }
    }
    lim = 0x1C0;
    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -0x1C0) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x1C0;
        if (n < d && d < 0x1C0) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (hit < 0) {
        t = ((fz >> 9) * TankMapWidth) + (fx >> 9);
        if (TankLevelsGridMapMiss[t] == 0) {
            eng->unk10.x = x;
            eng->unk10.z = z;
        }
    } else if (TankCountdown == 0) {
        TankCountdownEng = eng;
        TankCountdown = -1;
    }
}

void TankHeroMove(u32 player) {
    s32 done;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        TankLevelOver = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        TA_player *ta;
        EXTengine *eng;
        s32 speed;
        s32 pad;
        s32 x;
        s32 z;
        s32 dir;
        s32 half;
        s32 off;
        s32 pos;
        s32 frac;
        s32 hit;

        UpdateTankHeroWeapons(player);
        if (ProcessHeroAnim(player) != 0) {
            AnimateTank(player);
        }
        if (TankLevelOver != 0) {
            return;
        }
        ta = &TA_players[player];
        eng = Heroes[player].hero_eng;
        if (ta->RequestQuickFinish != 0) {
            DamageTankHero(player, 9999);
            return;
        }
        if (GameMode == 5) {
            TankHeroSteerMove(player);
            return;
        }
        if (GameMode == 7 || GameMode == 9 || GameMode == 5) {
            TankHeroGridMove(player);
            return;
        }
        half = (speed = ta->Speed) / 2;
        pad = ((PadButtons *)Pads)[player].unk50;
        x = eng->unk10.x;
        z = eng->unk10.z;
        dir = pad & 0xF0;
        if (dir == 0) {
            return;
        }
        if (!(x & 0xFF) && !(z & 0xFF) && (x & 0x100) && (z & 0x100)) {
            if ((pad & 0x20) && x != 0xF00 && TankMapCell(x + 0x100, z) == 0) {
                switch (ta->MoveDir) {
                case 0:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 3;
                    return;
                case 2:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 3;
                    return;
                case 1:
                    ta->MoveDir = 3;
                    return;
                }
            }
            if ((dir & 0x80) && eng->unk10.x != 0x100 &&
                (off = ta->Speed + 0x100, TankMapCell(x - off, z) == 0)) {
                switch (ta->MoveDir) {
                case 1:
                    break;
                case 3:
                    ta->MoveDir = 1;
                    return;
                case 0:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 1;
                    return;
                case 2:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 1;
                    return;
                }
            }
            if ((dir & 0x10) && eng->unk10.z != 0xF00 && TankMapCell(x, z + 0x100) == 0) {
                switch (ta->MoveDir) {
                case 2:
                    break;
                case 1:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 2;
                    return;
                case 0:
                    ta->MoveDir = 2;
                    return;
                case 3:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 2;
                    return;
                }
            }
            if ((dir & 0x40) && eng->unk10.z != 0x100 &&
                (off = ta->Speed + 0x100, TankMapCell(x, z - off) == 0)) {
                switch (ta->MoveDir) {
                case 1:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = -0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 0;
                    return;
                case 3:
                    SetHeroAction(player, 0x6F);
                    ta->ModeSpeed = 0x80;
                    ta->ModeCounter = 8;
                    ta->MoveDir = 0;
                    return;
                case 2:
                    ta->MoveDir = 0;
                    return;
                case 0:
                    break;
                }
            }
        } else {
            switch (ta->MoveDir) {
            case 0:
                if (dir & 0x10) {
                    ta->MoveDir = 2;
                }
                break;
            case 2:
                if (dir & 0x40) {
                    ta->MoveDir = 0;
                }
                break;
            case 1:
                if (dir & 0x20) {
                    ta->MoveDir = 3;
                }
                break;
            case 3:
                if (dir & 0x80) {
                    ta->MoveDir = 1;
                }
                break;
            }
        }
        switch (ta->MoveDir) {
        case 2:
            hit = TankMoveFindHero(player, eng->unk10.x, eng->unk10.z + speed, 0x1C0 - speed);
            if (hit < 0) {
                if (TankMapCell(x, z + 0x100) == 0) {
                    pos = eng->unk10.z + ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.z = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.z = pos - frac;
                    }
                }
            } else if (TankCountdown == 0) {
                TankCountdownEng = eng;
                TankCountdown = -1;
            }
            break;
        case 0:
            hit = TankMoveFindHero(player, eng->unk10.x, eng->unk10.z - speed, 0x1C0);
            if (hit < 0) {
                off = half + 0x100;
                if (TankMapCell(x, z - off) == 0) {
                    pos = eng->unk10.z - ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.z = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.z = pos - frac;
                    }
                }
            } else if (TankCountdown == 0) {
                TankCountdownEng = eng;
                TankCountdown = -1;
            }
            break;
        case 3:
            hit = TankMoveFindHero(player, eng->unk10.x + speed, eng->unk10.z, 0x1C0 - speed);
            if (hit < 0) {
                if (TankMapCell(x + 0x100, z) == 0) {
                    pos = eng->unk10.x + ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.x = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.x = pos - frac;
                    }
                }
            } else if (TankCountdown == 0) {
                TankCountdownEng = eng;
                TankCountdown = -1;
            }
            break;
        case 1:
            hit = TankMoveFindHero(player, eng->unk10.x - speed, eng->unk10.z, 0x1C0);
            if (hit < 0) {
                off = half + 0x100;
                if (TankMapCell(x - off, z) == 0) {
                    pos = eng->unk10.x - ta->Speed;
                    frac = pos & 0xFF;
                    eng->unk10.x = pos;
                    if (frac > 0x80) {
                        frac -= 0x100;
                    }
                    if (frac >= -half && frac <= half) {
                        eng->unk10.x = pos - frac;
                    }
                }
            } else if (TankCountdown == 0) {
                TankCountdownEng = eng;
                TankCountdown = -1;
            }
            break;
        }
    }
}

void SetTankHeroRecoil(u32 player) {
    s32 done;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction(player, 0x70);
        TankLevelOver = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        TA_players[player].SpeedY = -0x64;
        SetHeroSeq(player, 0x57);
        Heroes[player].hero_immunity = 0x3C;
        PlaySfx(TankRecoilSfx[Heroes[player].hero_name - 1][Random(4)], NULL, &Heroes[player].hero_eng->unk10, 0x1000,
                0x1E00, 0);
    }
}

void TankHeroRecoil(u32 arg0) {
    EXTengine *eng;
    HeroInfo *h;
    TA_player *p;

    eng = Heroes[arg0].hero_eng;
    h = &Heroes[arg0];
    TA_players[arg0].SpeedY += 0xC;
    p = &TA_players[arg0];
    eng->unk10.y += p->SpeedY;
    if (eng->unk10.y > 0) {
        p->SpeedY = -p->SpeedY / 2;
        eng->unk10.y = 0;
    }
    eng->sub->next->next->unk08 = -eng->unk10.y / 2;
    if (ProcessHeroAnim(arg0) != 0) {
        if (h->hero_status & 0x2000) {
            SetHeroAction(arg0, 0x6A);
        } else {
            SetHeroAction(arg0, h->hero_last_mode);
            SetHeroSeq(arg0, 0x59);
        }
    }
}

void SetTankHeroVictory(u32 arg0) {
    u32 i;
    subengine *sub;

    if (GameTeams[0] == 0) {
        CameraMode = 1;
        for (i = 0; i < 4; i++) {
            if (i != arg0 && !(Heroes[i].hero_status & 0x200)) {
                RemoveCamera(Heroes[i].hero);
            }
        }
    }
    Heroes[arg0].hero_ctrl |= 2;
    SetHeroSeq(arg0, 0x5A);
    StartHeroVictory(arg0);
    RequestHeroAnim(arg0, 0);
    Heroes[arg0].hero_eng->unk10.y = 0;
    sub = Heroes[arg0].hero_eng->sub;
    sub = sub->next;
    sub = sub->next;
    sub->unk08 = 0;
}

void TankHeroVictory(u32 arg0) {
    EXTengine *eng;
    subengine *sub;

    eng = Heroes[arg0].hero_eng;
    sub = Heroes[arg0].hero_sub->next->next;
    eng->unk1E += 0x15;
    eng->unk5E = 1;
    if (GAME->mode != 5) {
        sub->unk52 = 1;
        sub->unk12 -= 0x15;
    }
    ProcessHeroAnim(arg0);
    CheckHeroAnim(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        if (GAME->cheats & 0x800) {
            GAME->exitFlags |= 0x201;
        } else {
            GAME->exitFlags |= 1;
        }
    }
}

void TankHeroDie(u32 arg0) {
    s32 ok;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        TankLevelOver = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0 && ProcessHeroAnim(arg0) != 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = &HeroModelBank[DB_TankObjects];
        nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 0xE;
                ext->speed = 0x100;
                if (p != NULL) {
                    ext->flags = 5;
                    nas->Engine->sub->u6C.dbase = p;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        SetHeroAction(arg0, 0x6B);
    }
}

void SetTankHeroDead(u32 arg0) {
    subengine *s;

    Heroes[arg0].hero_hit = 0;
    Heroes[arg0].hero_immunity = 0;
    Heroes[arg0].hero_health = 0;
    Heroes[arg0].hero_status |= 0x6000;
    HeroBecomeAngel(arg0);
    s = Heroes[arg0].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
}

s32 MoveTankShot(NAS *nas, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;
    EXTobject_TA *fux;
    s32 owner;
    s32 ret;
    s32 x;
    s32 z;
    s32 tile;
    s32 row;
    s32 col;
    s32 dx;
    s32 dz;
    s32 v;
    s32 a;
    s32 hit;
    s32 sfx;

retry:
    TankShotBounce = 0;
    eng = nas->Engine;
    fux = (EXTobject_TA *)nas->FUX;
    owner = fux->Owner;
    ret = 0;
    if (GameMode == 8 &&
        *(((eng->unk10.z >> 9) * TankMapWidth) + (eng->unk10.x >> 9) + TankLevelsGridMapTank) != 0 &&
        fux->Status != 1) {
        x = eng->unk10.x;
        z = eng->unk10.z;
        goto dead;
    }
    x = eng->unk10.x + fux->SpeedX;
    z = eng->unk10.z + fux->SpeedZ;
    if (x < 0 || TankArenaMaxX + 0x100 < x) {
        if (fux->Status == 1) {
        back:
            eng->unk10.x -= fux->SpeedX;
            eng->unk10.z -= fux->SpeedZ;
            goto retry;
        }
        x += fux->SpeedX = -fux->SpeedX;
        eng->unk5E = 1;
        TankShotBounce = x;
        eng->unk1E = 0x1000 - eng->unk1E;
    } else
    if (z < 0 || TankArenaMaxZ + 0x100 < z) {
        if (fux->Status == 1) {
            goto back;
        }
        z += fux->SpeedZ = -fux->SpeedZ;
        eng->unk5E = 1;
        TankShotBounce = z;
        eng->unk1E = 0x800 - eng->unk1E;
    } else if (eng->unk10.y >= -400) {
        v = ((z >> 9) * TankMapWidth) + (x >> 9);
        tile = *(v + TankLevelsGridMapTank);
        if (tile != 0) {
            TankShotBounce = tile;
            if (fux->Status == 1) {
                goto back;
            }
            dx = eng->unk10.x - ((tile & 0xF0) << 4);
            if (dx < 0) {
                dx = -dx;
            }
            dz = eng->unk10.z - ((tile & 0xF) << 8);
            if (dz < 0) {
                dz = -dz;
            }
            if (dz < dx) {
                a = fux->SpeedX;
                x += fux->SpeedX = -a;
                if (TankLevelsGridMapTank[((z >> 9) * TankMapWidth) + (x >> 9)] != 0) {
                    x += a;
                    fux->SpeedX = a;
                    z += fux->SpeedZ = -fux->SpeedZ;
                    eng->unk1E = 0x800 - eng->unk1E;
                } else {
                    eng->unk1E = 0x1000 - eng->unk1E;
                }
            } else {
                a = (sfx = fux->SpeedZ);
                z += fux->SpeedZ = -a;
                if (TankLevelsGridMapTank[((z >> 9) * TankMapWidth) + (x >> 9)] != 0) {
                    z += a;
                    fux->SpeedZ = a;
                    x += fux->SpeedX = -fux->SpeedX;
                    eng->unk1E = 0x1000 - eng->unk1E;
                } else {
                    eng->unk1E = 0x800 - eng->unk1E;
                }
            }
            eng->unk5E = 1;
        }
    }
    eng->unk10.x = x;
    eng->unk10.z = z;
    MissileCollideWithPickup(eng);
    hit = TankRocketFindHero(x, z, 0x10E);
    if (hit >= 0) {
        switch ((u32)Random(4)) {
        case 0:
            sfx = 0x4BA;
            break;
        case 1:
            sfx = 0x4BB;
            break;
        case 2:
            sfx = 0x4BC;
            break;
        case 3:
            sfx = 0x4BD;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:
        if (((TankModeView *)GameInfo)->mode == 5 && GameInfo[hit + 0x5C] != 0 &&
            TA_players[hit].ForceFieldDelay == 0 && HeroesAllOut == 0 &&
            (u32)Random(0x20) < AiPlayerSkill[hit].level) {
            TankBotForceField[hit] = 1;
            goto hit2;
        }
        if (owner == hit) {
            DamageTankHero(hit, arg1 / 2);
        } else {
            DamageTankHero(hit, arg1);
        }
        goto hit2;
    }
    if (TankRocketNear(eng->unk10.x, eng->unk10.z, 0xE0) == NULL) {
        if (--fux->Counter != 0) {
            return ret;
        }
    dead:
        ret = 1;
        goto spawn;
    }
hit2:
    ret = 2;
spawn:
    if (arg3 != 0) {
        TankSpawnBit1605(&eng->unk10, arg3);
    }
    if (arg2 != 0) {
        CreateNAS(x, eng->unk10.y, z, arg2, 0x11);
    }
    return ret;
}

void PlotLaserBitTA(NAS *nas) {
    EXTengine *eng;
    EXTobject_TA *fux;
    subengine *sub;
    s32 hit;
    s32 sfx;
    s32 i;
    s16 rot;

    eng = nas->Engine;
    fux = (EXTobject_TA *)nas->FUX;
    sub = eng->sub;
    sub = sub->next;
    sub->unk51 = 0;
    sub->unk14 += 0x32;
    rot = -eng->unk1E;
    sub->unk10 = 0;
    sub->unk52 = 1;
    sub->unk12 = rot;
    hit = MoveTankShot(nas, 0x14, 0, 0xA);
    if (TankShotBounce != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4D8;
            break;
        case 1:
            sfx = 0x4D9;
            break;
        case 2:
            sfx = 0x4DA;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (hit != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E7, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (TA_players[fux->Owner].MainWeaponNas[i] == nas) {
                TA_players[fux->Owner].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (fux->Status == 1) {
        fux->Status = 0;
    }
}

void PlotFlamerBitTA(NAS *nas) {
    EXTengine *eng;
    EXTobject_TA *fux;
    s32 i;

    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    eng->unk5E = 1;
    eng->unk20 += 0x96;
    if (!(GameFrameCount & 7)) {
        TankSpawnBit1605(&eng->unk10, 2);
    }
    if (MoveTankShot(nas, 0x14, 0x1306, 0xC) != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E4, NULL, NULL, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (TA_players[fux->Owner].MainWeaponNas[i] == nas) {
                TA_players[fux->Owner].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (TankShotBounce != 0) {
        switch ((u32)Random(3)) {
        case 0:
            PlaySfx(0x4E1, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 1:
            PlaySfx(0x4E2, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x4E3, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
    if (fux->Status == 1) {
        fux->Status = 0;
    }
}

void PlotFlubberTA(NAS *nas) {
    EXTengine *eng;
    EXTobject_TA *fux;
    EXTengine *e;
    s32 sfx;
    s32 i;

    eng = nas->Engine;
    fux = (EXTobject_TA *)nas->FUX;
    eng->unk5E = 1;
    eng->unk1C += 0x55;
    fux->SpeedY += 0xC;
    eng->unk10.y += fux->SpeedY;
    if (eng->unk10.y >= -0x64) {
        eng->unk10.y = -0x64;
        fux->SpeedY = -0x3C;
        eng->unk10.y -= 0x3C;
        if (((TankModeView *)GameInfo)->mode == 5 ||
            (((TankModeView *)GameInfo)->mode == 6 && (((TankModeView *)GameInfo)->flags & 0x80) &&
             TankLevelsGridMapMiss[((eng->unk10.z >> 9) * TankMapWidth) + (eng->unk10.x >> 9)] != 0)) {
            switch ((u32)Random(4)) {
            case 0:
                sfx = 0x4F7;
                break;
            case 1:
                sfx = 0x4F8;
                break;
            case 2:
                sfx = 0x4F9;
                break;
            case 3:
                sfx = 0x4FA;
                break;
            default:
                goto skip1;
            }
            PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        skip1:
            TankSpawnBit1605(&eng->unk10, 0x12);
        } else {
            switch ((u32)Random(6)) {
            case 0:
                sfx = 0x4D2;
                break;
            case 1:
                sfx = 0x4D3;
                break;
            case 2:
                sfx = 0x4D4;
                break;
            case 3:
                sfx = 0x4D5;
                break;
            case 4:
                sfx = 0x4D6;
                break;
            case 5:
                sfx = 0x4D7;
                break;
            default:
                goto skip2;
            }
            PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        skip2:
            TankSpawnBit1605(&eng->unk10, 9);
        }
    }
    if (MoveTankShot(nas, 0x1E, 0x1306, 0xB) != 0) {
        do {
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x4E5, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        } while (0);
        for (i = 0; i < 3; i++) {
            if (TA_players[fux->Owner].MainWeaponNas[i] == nas) {
                TA_players[fux->Owner].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (TankShotBounce != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4DE;
            break;
        case 1:
            sfx = 0x4DF;
            break;
        case 2:
            sfx = 0x4E0;
            break;
        default:
            goto skip3;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip3:;
    }
    if (fux->Status == 1) {
        fux->Status = 0;
    }
    do {
        e = eng->Engine;
        if (e != NULL) {
            e->unk1E = eng->unk1E;
            e->unk10 = eng->unk10;
            e->unk10.y = 0;
        }
    } while (0);
}

void PlotSwirlyTA(NAS *nas) {
    EXTengine *eng;
    EXTobject_TA *fux;
    subengine *sub;
    s32 sfx;
    s32 i;

    eng = nas->Engine;
    eng->unk5E = 1;
    eng->unk1C += 0x17C;
    eng->unk1E += 0x118;
    eng->unk20 += 0xB4;
    sub = eng->sub->next;
    sub->unk52 = 1;
    sub->unk10 += 0x46;
    sub->unk12 += 0x8C;
    sub->unk14 += 0xD2;
    fux = (EXTobject_TA *)nas->FUX;
    if (MoveTankShot(nas, 0x14, 0x1306, 0xD) != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E6, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (TA_players[fux->Owner].MainWeaponNas[i] == nas) {
                TA_players[fux->Owner].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (TankShotBounce != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4DB;
            break;
        case 1:
            sfx = 0x4DC;
            break;
        case 2:
            sfx = 0x4DD;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (fux->Status == 1) {
        fux->Status = 0;
    }
}

void PlotShell(NAS *nas) {
    EXTobject_TA *fux;
    EXTengine *eng;
    EXTengine *e2;
    SLONG_XYZ *pos;
    u8 *p;
    NAS *puff;
    EXTscriptdata *ext;
    s32 sfx;

    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    fux->SpeedY += 0xC;
    eng->unk10.y += fux->SpeedY;
    if (eng->unk10.y > 0) {
        eng->unk10.y = 0;
        fux->SpeedY = -0x54;
        eng->unk10.y -= 0x54;
        pos = &eng->unk10;
        p = &HeroModelBank[DB_TankObjects];
        puff = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (puff != NULL) {
            ext = FindExtension(puff, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 9;
                ext->speed = 0x100;
                if (p != NULL) {
                    ext->flags = 5;
                    puff->Engine->sub->u6C.dbase = p;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                puff->InitDelete(puff);
            }
        }
    }
    if (MoveTankShot(nas, 0x3C, 0x1306, 4) != 0) {
        switch ((u32)Random(4)) {
        case 0:
        case 1:
            sfx = 0x490;
            break;
        case 2:
        case 3:
            sfx = 0x491;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:
        KillNAS(nas);
        return;
    }
    if (fux->Status == 1) {
        fux->Status = 0;
    }
    e2 = eng->Engine;
    if (e2 != NULL) {
        e2->unk1E = eng->unk1E;
        e2->unk10 = eng->unk10;
        e2->unk10.y = 0;
    }
}

void PlotLobber_TA(NAS *nas)
{
  EXTengine *eng;
  EXTobject_TA *fux;
  EXTengine *e;
  EXTengine *e2;
  s32 x;
  s32 z;
  s32 d;
  s32 n;
  s32 lim;
  s32 i;
  s32 j;
  s32 lo;
  s32 fx;
  s32 fz;
  s32 flim;
  s32 hit;
  u16 name;

  eng = nas->Engine;
  fux = (EXTobject_TA *) nas->FUX;
  eng->unk10.x += fux->SpeedX;
  eng->unk10.z += fux->SpeedZ;
  fux->SpeedY += 0xC;
  eng->unk10.y += fux->SpeedY;
  fx = eng->unk10.x;
  fz = eng->unk10.z;
  flim = 0x180;
  for (j = 0; j < 4; j++)
  {
    if (j == fux->Owner)
    {
      continue;
    }
    name = (*(&Heroes[j])).hero_name;
    if ((name == 9) && (((((TankModeView *) GameInfo)->mode != name) || (TankBossStatus != 0)) || (((u32) (j - 2)) >= 2)))
    {
      continue;
    }
    if (Heroes[j].hero_status & 0xC000)
    {
      continue;
    }
    e = Heroes[j].hero_eng;
    d = fx - e->unk10.x;
    if (d < (-0x180))
    {
      continue;
    }
    if (flim < d)
    {
      continue;
    }
    d = fz - e->unk10.z;
    lo = -0x180;
    if ((lo < d) && (d < 0x180))
    {
      hit = j;
      goto found;
    }
  }


  hit = -1;
  found:
  if (hit >= 0)
  {
    s32 hx;
    s32 hz;
    s32 r;
    s32 hd;
    r = 0x180;

    hx = eng->unk10.x;
    hz = eng->unk10.z;
    for (i = 0; i < 4; i++)
    {
      EXTengine *he;
      he = Heroes[i].hero_eng;
      hd = hx - he->unk10.x;
      if (hd < (-0x180))
      {
        continue;
      }
      if (r < hd)
      {
        continue;
      }
      hd = hz - he->unk10.z;
      if (hd < (-0x180))
      {
        continue;
      }
      if (r < hd)
      {
        continue;
      }
      DamageTankHero(i, 0x3C);
    }


    goto boom;
  }


  if (((fux->SpeedY > 0) && (eng->unk10.y >= (-0x103))) || ((GameMode == 9) && (eng->unk10.z > 0x1000)))
  {
    eng->unk10.y = -0x104;
    if ((GameMode == 9) && (fux->Owner != (-1)))
    {
      CheckMineToBoss(eng->unk10.x, eng->unk10.y, eng->unk10.z);
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    lim = 0x180;
    for (hit = 0; hit < 4; hit++)
    {
      name = Heroes[hit].hero_name;
      if ((name == 9) && (((((TankModeView *) GameInfo)->mode != name) || (TankBossStatus != 0)) || (((u32) (hit - 2)) >= 2)))
      {
        continue;
      }
      if (Heroes[hit].hero_status & 0xC000)
      {
        continue;
      }
      e = Heroes[hit].hero_eng;
      d = x - e->unk10.x;
      if (d < (-0x180))
      {
        continue;
      }
      if (lim < d)
      {
        continue;
      }
      d = z - e->unk10.z;
      n = -0x180;
      if ((n < d) && (d < 0x180))
      {
        goto found2;
      }
    }

    hit = -1;
    found2:
    if (hit < 0)
    {
      goto puff;
    }

    DamageTankHero(hit, 0x3C);
    boom:
    if (eng->sfx != 0)
    {
      StopSfx(eng->sfx);
    }

    switch ((u32) Random(4))
    {
      case 0:
        PlaySfx(0x4BA, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 1:
        PlaySfx(0x4BB, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 2:
        PlaySfx(0x4BC, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 3:
        PlaySfx(0x4BD, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

    }

    puff:
    switch ((u32) Random(4))
    {
      case 0:

      case 1:
        PlaySfx(0x490, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

      case 2:

      case 3:
        PlaySfx(0x491, 0, &eng->unk10, 0x1000, 0x1E00, 0);
        break;

    }


    TankSpawnBit1605(&eng->unk10, 1);

    KillNAS(nas);
    return;
  }

  if (fux->Status == 1)
  {
    fux->Status = 0;
  }

  e2 = eng->Engine;
  if (e2 != 0)
  {
    e2->unk1E = eng->unk1E;
    e2->unk10 = eng->unk10;
    if (TankLevelsGridMapTank[((eng->unk10.z >> 9) * TankMapWidth) + (eng->unk10.x >> 9)] != 0)
    {
      e2->unk10.y = -0x100;
    }
    else
    {
      e2->unk10.y = 0;
    }
  }

}

void PlotRocketTA(NAS *nas)
{
    EXTengine *eng;
    EXTobject_TA *fux;
    NAS *n;
    NAS *puff;
    EXTscriptdata *ext;
    void *p;
    SLONG_XYZ *pos;
    s32 x;
    s32 z;
    s32 hit;
    s32 dmg;
    s32 ok;

    eng = nas->Engine;
    fux = (EXTobject_TA *) nas->FUX;
    x = eng->unk10.x + fux->SpeedX;
    z = eng->unk10.z + fux->SpeedZ;
    if (x < 0 || x >= TankArenaMaxX + 0x100) {
        goto boom;
    }
    if (TankRocketNear(x, z, 0xE0) != NULL) {
        goto boom;
    }
    if (GameMode == 9) {
        ok = (u32) z < 0x1280;
    } else {
        if (z < 0) {
            goto boom;
        }
        ok = z < TankArenaMaxZ + 0x100;
    }
    if (!ok) {
        goto boom;
    }
    if (TankRocketTile(x, z) != 0) {
        if (GameMode == 7 || GameMode == 9) {
            if (z < TankArenaMaxZ) {
                n = (NAS *) *(((z >> 9) * TankMapWidth) + (x >> 9) + TankMapNas);
                if (n != NULL) {
                    pos = &n->Engine->unk10;
                    p = MapScene->dbase;
                    puff = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
                    if (puff != NULL) {
                        ext = FindExtension(puff, 0x10000000);
                        if (ext != NULL) {
                            ext->scriptnumber = 0;
                            ext->speed = 0x100;
                            if (p != NULL) {
                                ext->flags = 5;
                                puff->Engine->sub->u6C.dbase = p;
                            } else {
                                ext->flags = 1;
                            }
                            ext->maxloops = 0;
                            puff->InitDelete(puff);
                        }
                    }
                    TankRocketKillCell(x, z);
                }
            }
        }
        goto boom;
    }
    eng->unk10.x = x;
    eng->unk10.z = z;
    MissileCollideWithPickup(eng);
    hit = TankRocketFindHero(x, z, 0x10E);
    if (hit < 0 || hit == fux->Owner) {
        goto puff;
    }
    if (((TankModeView *) GameInfo)->mode != 9 || fux->Owner == -1) {
        dmg = 0x3C;
    } else {
        dmg = (((GameInfo[0xB4] * 190) / 32 + 10) * 60) / 100;
    }
    DamageTankHero(hit, dmg);
boom:
    CreateNAS(x, eng->unk10.y, z, 0x1306, 0x11);
    KillNAS(nas);
    if (eng->sfx != NULL) {
        StopSfx(eng->sfx);
    }
    PlaySfx(0x43D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    return;
puff:
    if (!(GameFrameCount & 1)) {
        TankSpawnBit1605(&eng->unk10, 0x11);
    }
}

void PlotMine(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    s32 i;
    s32 d;
    s32 x;
    s32 z;
    s32 mode;
    s32 t;
    s32 r;
    s32 n;
    u16 name;
    u8 *g;

    g = GameInfo;
    eng = nas->Engine;
    if ((((TankModeView *)g)->mode == 8 &&
         TankLevelsGridMapMiss[((eng->unk10.z >> 9) * TankMapWidth) + (eng->unk10.x >> 9)] != 0) ||
        --nas->UserInfo.status == 0) {
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        CreateMineShrapnelBits(nas);
        for (i = 0; i < 8; i++) {
            if (TankMines[i] == nas) {
                TankMines[i] = NULL;
                break;
            }
        }
        r = 250;
        x = eng->unk10.x;
        z = eng->unk10.z;
        for (i = 0; i < 4; i++) {
            name = Heroes[i].hero_name;
            if (name == 9 &&
                (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
                 (u32)(i - 2) >= 2)) {
                continue;
            }
            if (Heroes[i].hero_status & 0xC000) {
                continue;
            }
            e = Heroes[i].hero_eng;
            d = x - e->unk10.x;
            if (d < -250) {
                continue;
            }
            if (r < d) {
                continue;
            }
            d = z - e->unk10.z;
            n = -250;
            if (n < d && d < 250) {
                goto found;
            }
        }
        i = -1;
    found:
        if (i >= 0) {
            if (((TankModeView *)GameInfo)->mode == 8 && GameInfo[i + 0x5C] == 0 &&
                (((TankGameView *)GameInfo)->flags & 0x80)) {
                DamageTankHero(i, 0x4B0);
            } else {
                DamageTankHero(i, 0x3C);
            }
        }
        KillNAS(nas);
        return;
    }
    t = (u16)(nas->UserInfo.status / 30);
    if (t >= 4) {
        t = 3;
    }
    mode = ((TankModeView *)g)->mode;
    if ((mode == 8 && (((TankGameView *)g)->flags & 0x80)) || mode == 9) {
        nas->Engine->sub->entity = MineFramesBig[t].v;
    } else {
        nas->Engine->sub->entity = MineFrames[t].v;
    }
    if ((u16)(nas->UserInfo.status % 30) == 0) {
        PlaySfx(0x4EB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void PlotWaterMine(NAS *nas) {
    s32 i;
    s32 j;
    TA_player *ta;
    EXTengine *eng;
    EXTengine *e;
    s32 t;
    s32 d;
    s32 hit;
    s32 x;
    s32 z;
    s32 r;
    s32 n;
    u16 name;

    t = nas->UserInfo.user2;
    eng = nas->Engine;
    eng->unk10.y += TankWaterBob[t & 0xF];
    nas->UserInfo.user2 = t + 1;
    nas->UserInfo.status = (nas->UserInfo.status + 4) & 0xFFF;
    eng->unk10.x += (rcossin_tbl[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 7) >> 12;
    eng->unk10.z += (rcossin_tbl[nas->UserInfo.status & 0xFFF].unk2 * 7) >> 12;
    if (eng->unk10.x > TankArenaMaxX) {
        eng->unk10.x = TankArenaMaxX;
    }
    if (eng->unk10.x < 0x100) {
        eng->unk10.x = 0x100;
    }
    if (eng->unk10.z > TankArenaMaxZ) {
        eng->unk10.z = TankArenaMaxZ;
    }
    if (eng->unk10.z < 0x100) {
        eng->unk10.z = 0x100;
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    r = 400;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -400) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -400;
        if (n < d && d < 400) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (nas->UserInfo.flags == 1) {
        if (hit < 0) {
            nas->UserInfo.flags = 0;
        }
    } else if (hit >= 0) {
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (j = 0; j < 8; j++) {
            if (TankMines[j] == nas) {
                TankMines[j] = NULL;
                break;
            }
        }
        if (GameInfo[hit + 0x5C] == 0) {
            if (((TankGameView *)GameInfo)->flags & 0x80) {
                DamageTankHero(hit, 0x4B0);
            } else {
                DamageTankHero(hit, 0x3C);
            }
        } else {
            DamageTankHero(hit, 0x3C);
        }
        ta = &TA_players[hit];
        ta->SpeedX = 0;
        ta->SpeedZ = 0;
        CreateMineShrapnelBits(nas);
        KillNAS(nas);
    }
}

void PlotTorpedo(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    HeroInfo *hero;
    HeroInfo *h0;
    HeroInfo *hb;
    HeroInfo *h2;
    NAS *n;
    SLONG_XYZ *pos;
    u8 *p;
    EXTscriptdata *ext;
    s32 player;
    s32 angle;
    s32 t;
    s32 dx;

    player = nas->UserInfo.user1;
    h0 = Heroes;
    hero = &h0[player];
    eng = nas->Engine;
    if (!(hero->hero_status & 0xA400) && hero->hero_health > 0) {
        t = nas->UserInfo.user2;
        eng->unk10.y += TankWaterBob[t & 0xF];
        nas->UserInfo.user2 = t + 1;
        he = hero->hero_eng;
        angle = get_angle(he->unk10.x - eng->unk10.x, he->unk10.z - eng->unk10.z);
        eng->unk1E = angle;
        eng->unk5E = 1;
        if (nas->UserInfo.status != 0) {
            nas->UserInfo.status--;
        } else {
            eng->unk10.x += (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 9) >> 12;
            eng->unk10.z += (rcossin_tbl[angle & 0xFFF].unk2 * 9) >> 12;
        }
        hb = Heroes;
        h2 = &hb[player];
        if (h2->hero_status & 0xFA00) {
            return;
        }
        if (h2->hero_immunity != 0) {
            return;
        }
        dx = eng->unk10.x;
        if ((u32)(dx - he->unk10.x + 0x15F) > 0x2BE) {
            return;
        }
        dx = eng->unk10.z;
        if ((u32)(dx - he->unk10.z + 0x15F) > 0x2BE) {
            return;
        }
        h2->hero_health = 1;
        TA_players[player].Energy = 1;
        h2->hero_immunity = 0;
        DamageTankHero(player, 0x4B0);
    }
    pos = &eng->unk10;
    PlaySfx(0x4EA, NULL, pos, 0x1000, 0x1E00, 0);
    p = &HeroModelBank[DB_TankObjects];
    n = CreateNAS(eng->unk10.x, pos->y, pos->z, 0x1605, 0x11);
    if (n != NULL) {
        ext = FindExtension(n, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = 0x10;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                n->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            n->InitDelete(n);
        }
    }
    KillNAS(nas);
}

void CreateMineShrapnelBits(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    NAS *n;
    NAS *m;
    SLONG_XYZ *pos;
    u8 *p;
    EXTscriptdata *ext;
    s32 i;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;

    eng = nas->Engine;
    switch ((s16)(GameMode - 5)) {
    default:
    case 1:
    case 3:
        for (i = 0; i < 4; i++) {
            dx = MineShrapnelOffsets[i].x;
            dz = MineShrapnelOffsets[i].z;
            m = CreateNAS(eng->unk10.x + dx, eng->unk10.y, eng->unk10.z + dz, 0x1304, 0x11);
            if (m != NULL) {
                m->UserInfo.user1 = dx;
                m->UserInfo.user2 = dz;
            }
        }
        break;
    case 0:
    case 2:
    case 4:
        pos = &eng->unk10;
        p = &HeroModelBank[DB_TankObjects];
        n = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 0x10;
                ext->speed = 0x100;
                if (p != NULL) {
                    ext->flags = 5;
                    n->Engine->sub->u6C.dbase = p;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                n->InitDelete(n);
            }
        }
        x = eng->unk10.x;
        z = eng->unk10.z;
        if (GameMode == 9) {
            CheckMineToBoss(x, eng->unk10.y, z);
        }
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                he = Heroes[i].hero_eng;
                if ((u32)(x - he->unk10.x + 0x300) <= 0x600 && (u32)(z - he->unk10.z + 0x2FF) < 0x5FF) {
                    DamageTankHero(i, 0x3C);
                }
            }
        }
        break;
    }
}

static inline s32 ShrapnelFindHero(s32 x, s32 z) {
    s32 i;
    s32 d;
    s32 r;
    s32 n;
    u16 name;
    EXTengine *e;

    r = 250;
    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -250) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -250;
        if (n < d && d < 250) {
            return i;
        }
    }
    return -1;
}

static inline u8 ShrapnelTile(s32 x, s32 z) {
    s32 i;

    i = ((z >> 9) * TankMapWidth) + (x >> 9);
    return TankLevelsGridMapMiss[i];
}

void PlotShrapnelBit(NAS *nas) {
    EXTengine *eng;
    s32 x;
    s32 z;
    s32 dx;
    s32 dz;
    s32 hit;
    s32 sfx;

    eng = nas->Engine;
    CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
    dz = nas->UserInfo.user2;
    z = eng->unk10.z + dz;
    dx = nas->UserInfo.user1;
    x = eng->unk10.x + dx;
    if (ShrapnelTile(x, z) != 0) {
        goto kill;
    }
    hit = ShrapnelFindHero(x, z);
    z += dz;
    if (hit < 0) {
        goto step;
    }
found:
    switch ((u32)Random(4)) {
    case 0:
        sfx = 0x4BA;
        goto play;
    case 1:
        sfx = 0x4BB;
        goto play;
    case 2:
        sfx = 0x4BC;
        goto play;
    case 3:
        sfx = 0x4BD;
    play:
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
    if (((TankModeView *)GameInfo)->mode == 8 && GameInfo[hit + 0x5C] == 0 &&
        (((TankGameView *)GameInfo)->flags & 0x80)) {
        DamageTankHero(hit, 0x4B0);
    } else {
        DamageTankHero(hit, 0x3C);
    }
    goto kill;
step:
    x += dx;
    if (ShrapnelTile(x, z) != 0) {
        goto kill;
    }
    hit = ShrapnelFindHero(x, z);
    if (hit >= 0) {
        goto found;
    }
    eng->unk10.z = z;
    eng->unk10.x = x;
    nas->UserInfo.status++;
    if (TankArenaMaxX + 0x100 < eng->unk10.x || eng->unk10.x < 0x100 ||
        TankArenaMaxZ + 0x100 < eng->unk10.z || eng->unk10.z < 0x100) {
        goto kill;
    }
    return;
kill:
    CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1305, 0x11);
    KillNAS(nas);
}

void PlotPickupGenerator_TA(NAS *nas)
{
  s32 i;
  s32 j;
  s32 x;
  s32 z;
  s32 r;
  s32 n;
  s32 new_var;
  NAS *p;
  EXTengine *eng;
  if (TankLevelOver != 0)
  {
    KillNAS(nas);
    return;
  }
  if (nas->UserInfo.flags == 0)
  {
    nas->UserInfo.flags = Random(0x78) + 0xB4;
    switch (GameMode)
    {
      case 7:

      default:
        CreateNAS(0x600, -0x500, 0x600, 0x1307, 0x11);
        return;

      case 8:
        for (i = 0;; i++)
      {
        s32 r;
        if (i >= 3)
        {
          break;
        }
        if (PU_TA_list[i] != 0)
        {
          continue;
        }

        n = Random(0x29);
        x = PickupSpots[n * 2];
        new_var = x;
        z = PickupSpots[(n * 2) | 1];
        if (PickupFindHero(new_var, z) >= 0)
        {
          return;
        }
        for (j = 0; j < 3; j++)
        {
          p = PU_TA_list[j];
          if (p != 0)
          {
            if ((p->Engine->unk10.x != new_var) || (p->Engine->unk10.z != z))
            {
              break;
            }
            return;
          }
        }


        p = CreateNAS(new_var, 0, z, 0x1308, 0x11);
        if (p == 0)
        {
          continue;
        }
        eng = p->Engine;
        PU_TA_list[i] = p;
        r = Random(3);
        switch (r)
        {
          case 0:
            p->Engine->sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
            p->Engine->sub->entity = 0x2013;
            r = 5;
            break;

          case 1:
            eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            eng->sub->entity = 0x2018;
            r = 4;
            break;

          case 2:
            p->Engine->sub->entity = 0x200C;
            r = 3;
            break;

        }

        p->UserInfo.status = r;
      }

        break;

      case 9:
        for (i = 0;; i++)
      {
        s32 k;
        if (i >= 3)
        {
          break;
        }
        if (PU_TA_list[i] != 0)
        {
          continue;
        }
        x = nas->Engine->unk10.x;
        z = nas->Engine->unk10.z;
        if (PickupFindHero(x, z) >= 0)
        {
          return;
        }
        for (k = 0; k < 3; k++)
        {
          p = PU_TA_list[k];
          if (((p != 0) && (p->Engine->unk10.x == x)) && (p->Engine->unk10.z == z))
          {
            return;
          }
        }

        p = CreateNAS(x, 0, z, 0x1308, 0x11);
        if (p == 0)
        {
          return;
        }
        PU_TA_list[i] = p;
        nas->UserInfo.flags = ((AiPlayerSkill[3].level * 2) - GameInfo[0xB4]) * 30;
        if (((u16) (nas->UserInfo.flags & 0xFFFE)) < 60)
        {
          nas->UserInfo.flags = 60;
        }
        if (nas->UserInfo.flags > 1200)
        {
          nas->UserInfo.flags = 1200;
        }
        if (GameInfo[0xB4] > 10)
        {
          nas->UserInfo.flags >>= 1;
        }
        if (((u32) Random(0x20)) > 8)
        {
          eng = p->Engine;
          p->UserInfo.status = 4;
          eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
          eng->sub->entity = 0x2018;
          eng->sub->flags |= 0x20000000;
          eng = eng->Engine;
          eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
          eng->sub->entity = 0x2001;
          return;
        }
        p->UserInfo.status = 5;
        p->Engine->sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
        p->Engine->sub->entity = 0x2013;
        return;
      }

        break;

    }

  }
  else
  {
    nas->UserInfo.flags--;
  }
}

void InitHelicopterTA(NAS *nas) {
    EXTobject_TA *ext;
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    u8 *p;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->Status = 3;
        eng = nas->Engine;
        if (++HelicopterCount & 1) {
            eng->unk1E = 0x400;
            eng->unk10.x = 0x1800;
            ext->SpeedX = -0x40;
            eng->unk10.y = -0x400;
        } else {
            eng->unk1E = 0xC00;
            eng->unk10.x = -0x800;
            ext->SpeedX = 0x40;
            eng->unk10.y = -0x400;
        }
        switch ((s16)(GameMode - 5)) {
        case 0:
            eng->unk10.z = HelicopterZSwamp[Random(4)];
            break;
        case 1:
            eng->unk10.z = HelicopterZDesert[Random(4)];
            break;
        case 2:
            eng->unk10.z = HelicopterZJungle[Random(4)];
            break;
        case 3:
            eng->unk10.z = HelicopterZMetal[Random(4)];
            break;
        case 4:
            eng->unk10.z = HelicopterZBoss[Random(4)];
            break;
        }
        p = &HeroModelBank[DB_TankObjects2];
        sub = eng->sub;
        sub->u6C.dbase = p;
        sub->entity = 0x4080;
        eng->unk5D = 1;
        eng->unk5E = 1;
        nas->UserInfo.user2 = 0;
        PlaySfx(0x4F6, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        e = AddExtEngine(nas, 1);
        if (e != NULL) {
            e->unk10.x = eng->unk10.x;
            e->unk10.y = 0;
            e->unk10.z = eng->unk10.z;
            e->sub->unk51 = 2;
            e->sub->u6C.dbase = p;
            e->sub->entity = 0x200F;
            e->sub->flags |= 0x40000041;
            e->sub->unk76 = 0x800;
            e->sub->unk50 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void PlotHelicopterTA(NAS *nas) {
    NAS **slot;
    TankModeView *g;
    EXTobject_TA *fux;
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    NAS *p;
    s32 i;
    s32 n;
    s32 x;
    u32 chance;
    u16 st;
    NAS **s;

    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    sub = eng->sub;
    if (TankLevelOver != 0) {
        eng->unk10.y -= 0x32;
    }
    eng->unk10.x += fux->SpeedX;
    if (--nas->UserInfo.user2 < 0) {
        nas->UserInfo.user2 = 7;
    }
    sub->entity = HelicopterFrames[nas->UserInfo.user2].v;
    e2 = eng->Engine;
    if (e2 != NULL) {
        e2->unk1E = eng->unk1E;
        e2->unk10 = eng->unk10;
        e2->unk10.y = 0;
    }
    x = eng->unk10.x;
    if ((u32)(x + 0x800) > 0x2000) {
        KillNAS(nas);
        return;
    }
    if (fux->Status != 3 || TankLevelOver != 0 || (x & 0x1FF) != 0x100) {
        return;
    }
    for (i = 0, s = PU_TA_list; i < 3; i++, s++) {
        p = *s;
        if (p != NULL) {
            if (i) {
                if (p->Engine->unk10.x != eng->unk10.x || p->Engine->unk10.z != eng->unk10.z) {
                    break;
                }
            } else {
                if (p->Engine->unk10.x != eng->unk10.x || p->Engine->unk10.z != eng->unk10.z) {
                    break;
                }
            }
            return;
        }
    }
    if ((u32)(eng->unk10.x - 2) >= 0xFFD) {
        return;
    }
    if ((u32)Random(0x8000) >= 0x2800) {
        return;
    }
    chance = 0;
    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name != 9 && TA_players[i].Energy <= 0) {
            chance += 2000;
        }
    }
    n = 0;
    g = (TankModeView *)GameInfo;
    slot = PU_TA_list;
loop:
    if (n < 3) {
        if (*slot == NULL) {
            p = CreateNAS(eng->unk10.x, eng->unk10.y - 0xC8, eng->unk10.z + 0x3C, 0x1308, 0x11);
            if (p == NULL) {
                return;
            }
            fux->Status = 0;
            if (g->mode) {
                *slot = p;
            } else if (*slot) {
                *slot = p;
            } else {
                *slot = p;
            }
            if (g->mode != 9 && (u32)Random(7000) < chance) {
                st = 6;
            } else {
                switch (g->mode) {
                case 5:
                default:
                    if ((u32)Random(1000) >= 0x1F5) {
                        st = 5;
                    } else {
                        st = 4;
                    }
                    break;
                case 7:
                    switch ((u32)Random(3)) {
                    case 0:
                        st = 5;
                        break;
                    case 1:
                        st = 4;
                        break;
                    case 2:
                        st = 2;
                        break;
                    case 3:
                        st = 2;
                        break;
                    default:
                        st = 5;
                        break;
                    }
                    break;
                case 9:
                    switch ((u32)Random(2)) {
                    default:
                        st = 5;
                        break;
                    case 0:
                        st = 5;
                        break;
                    case 1:
                        st = 3;
                        break;
                    }
                    break;
                }
            }
            p->UserInfo.status = st;
            return;
        }
        slot++;
        n++;
        goto loop;
    }
}

void InitPickupTA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    EXTobject_TA *ext;
    subengine *sub;
    u8 *p;
    s32 y;

    eng = nas->Engine;
    switch ((u32)Random(4)) {
    default:
    case 0:
        PlaySfx(0x4B0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x4B1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x4B2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x4B3, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        ext->SpeedY = 0;
        nas->FUX = ext;
        p = &HeroModelBank[DB_TankObjects];
        eng = nas->Engine;
        sub = eng->sub;
        sub->u6C.dbase = p;
        sub->entity = 0x202B;
        eng->unk5D = 1;
        e = AddExtEngine(nas, 1);
        if (e != NULL) {
            e->unk10.x = eng->unk10.x;
            e->unk10.y = 0;
            e->unk10.z = eng->unk10.z;
            e->sub->unk51 = 2;
            e->sub->u6C.dbase = p;
            e->sub->entity = 0x202A;
            e->sub->flags |= 0x40000041;
            e->sub->unk76 = 0x800;
            e->sub->unk50 = 1;
            y = eng->unk10.y + 0x1000;
            e->sub->unk20.x = y;
            e->sub->unk20.z = y;
            e->sub->unk52 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void PlotPickupTA(NAS *nas) {
    EXTengine *eng;
    EXTobject_TA *fux;
    NAS *n;
    EXTengine *e;
    EXTengine *fe;
    subengine *sub;
    s32 dx;
    s32 fx;
    s32 fz;
    s32 d;
    s32 j;
    s32 i;
    s32 hit;
    s32 k;
    s32 lim;
    s32 v;
    s32 x;
    s32 sc;
    s32 anim;
    s32 sn;
    u16 st;

    eng = nas->Engine;
    fux = (EXTobject_TA *)nas->FUX;
    dx = 0;
    if (TankMapCell(d = eng->unk10.x, eng->unk10.z) != 0) {
        goto puff;
    }
    switch (nas->UserInfo.user1) {
    case 0:
    default:
        if (nas->UserInfo.status == 4) {
            eng->sub->entity++;
            if (eng->sub->entity >= 0x2026) {
                eng->sub->entity = 0x2018;
            }
        }
        if (nas->UserInfo.status == 2) {
            eng->unk5E = 1;
            eng->unk1E += 0x28;
        }
        fux->SpeedY += 8;
        eng->unk10.y += fux->SpeedY;
        if (eng->unk10.y >= 0) {
            eng->unk10.y = -1;
            fux->SpeedY = -0x28;
            eng->unk10.y -= 0x28;
        }
        break;
    case 5:
        eng->unk10.y += 0x14;
        nas->UserInfo.user2 = (nas->UserInfo.user2 + 0x78) & 0xFFF;
        sn = rcossin_tbl[(nas->UserInfo.user2 - 0x400) & 0xFFF].unk2;
        eng->unk5E = 1;
        eng->unk20 = (sn * 25) >> 9;
        dx = -(rcossin_tbl[(nas->UserInfo.user2 - 0x400) & 0xFFF].unk2 * 120) >> 12;
        if (eng->unk10.y < -0x12B) {
            break;
        }
        fx = eng->unk10.x;
        fz = eng->unk10.z;
        lim = 0x100;
        for (j = 0; j < 8; j++) {
            n = TankMines[j];
            if (n == NULL) {
                continue;
            }
            e = n->Engine;
            d = fx - e->unk10.x;
            if (d < -0x100) {
                continue;
            }
            if (lim < d) {
                continue;
            }
            d = fz - e->unk10.z;
            if (d < -0x100) {
                continue;
            }
            if (lim < d) {
                continue;
            }
            n->UserInfo.status = 1;
        }
        if ((u32)((u16)GameMode - 8) >= 2) {
            TankSpawnBit1605(&eng->unk10, 3);
            switch ((u32)Random(4)) {
            case 0:
                PlaySfx(0x4B4, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 1:
                PlaySfx(0x4B5, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x4B6, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x4B7, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        }
        eng->unk10.y = 0;
        fux->SpeedY = -0x28;
        nas->UserInfo.user1 = 0;
        eng->unk20 = 0;
        switch (nas->UserInfo.status) {
        case 2:
            eng->sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
            eng->sub->entity = 0x2007;
            eng->Engine->sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
            eng->Engine->sub->entity = 0x200F;
            eng->Engine->unk1E = eng->unk1E + 0x400;
            eng->Engine->sub->unk20.x = 0x20;
            eng->Engine->sub->unk20.z = 0x60;
            eng->Engine->unk5E = 1;
            break;
        case 6:
            CreateNAS(eng->unk10.x, -0x14, eng->unk10.z, 0x1303, 0x11);
            for (i = 0; i < 3; i++) {
                if (PU_TA_list[i] == nas) {
                    PU_TA_list[i] = NULL;
                    break;
                }
            }
            KillNAS(nas);
            return;
        case 5:
        default:
            eng->sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
            eng->sub->entity = 0x2013;
            eng->Engine->sub->u6C.dbase = ((u8 *)&HeroModelBank[DB_TankObjects2]) - 0x738;
            eng->Engine->sub->entity = 0x2001;
            break;
        case 3:
            eng->sub->entity = 0x200C;
            eng->Engine->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            eng->Engine->sub->entity = 0x2001;
            break;
        case 4:
            eng->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            eng->sub->entity = 0x2018;
            eng->sub->flags |= 0x20000000;
            eng->Engine->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            eng->Engine->sub->entity = 0x2001;
            break;
        }
        break;
    }
    eng->unk5E = 1;
    if (eng->unk10.y < -0x2ED) {
        goto follow;
    }
    hit = TankRocketFindHero(eng->unk10.x, eng->unk10.z, 0x12C);
    if (hit < 0) {
        goto follow;
    }
    if (TankLevelOver != 0) {
    puff:
        TankSpawnBit1605(&eng->unk10, 3);
        PlaySfx(0x4EA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (k = 0; k < 3; k++) {
            if (PU_TA_list[k] == nas) {
                PU_TA_list[k] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    switch (nas->UserInfo.status) {
    case 6:
        if (((TankModeView *)GameInfo)->mode != 8 || GameInfo[hit + 0x5C] != 0 ||
            !(((TankModeView *)GameInfo)->flags & 0x80)) {
            v = 0x1E;
        } else {
            v = 0x4B0;
        }
        DamageTankHero(hit, v);
        CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1306, 0x11);
        goto puff;
    case 5:
        TA_players[hit].SubWeaponRounds++;
        break;
    case 4:
        TA_players[hit].Energy += 0x14;
        if (TA_players[hit].Energy >= 0x64) {
            TA_players[hit].Energy = 0x63;
        }
        Heroes[hit].hero_health = TA_players[hit].Energy;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
        sub = Heroes[hit].hero_sub->next;
        TA_players[hit].TurretFiring = -1;
        st = nas->UserInfo.status;
        if (st != 2) {
            if (st != TA_players[hit].MainWeaponType) {
                TA_players[hit].MainWeaponRounds = 2;
            } else {
                TA_players[hit].MainWeaponRounds += 2;
            }
            anim = 0x2008;
        } else {
            if (TA_players[hit].MainWeaponType != st) {
                TA_players[hit].MainWeaponRounds = st;
            } else {
                TA_players[hit].MainWeaponRounds += 2;
            }
            anim = 0x200B;
        }
        sub->entity = anim;
        TA_players[hit].MainWeaponType = nas->UserInfo.status;
        break;
    case 7:
        sub = Heroes[hit].hero_sub->next;
        sub->entity = 0x2008;
        if (hit < 2) {
            TA_players[hit].SubWeaponRounds++;
        }
        if (TA_players[hit].MainWeaponType != 3) {
            TA_players[hit].MainWeaponType = 3;
            TA_players[hit].MainWeaponRounds = 1;
        } else {
            TA_players[hit].MainWeaponRounds++;
        }
        break;
    }
    for (i = 0; i < 3; i++) {
        if (PU_TA_list[i] == nas) {
            PU_TA_list[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
    switch (nas->UserInfo.status) {
    case 4:
    default:
        PlaySfx(0x1C2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 3:
        PlaySfx(0x48E, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 2:
        PlaySfx(0x51F, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    case 5:
        PlaySfx(0x4E8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        return;
    }
follow:
    fe = eng->Engine;
    if (fe != NULL) {
        fe->unk1E = eng->unk1E;
        fe->unk10 = eng->unk10;
        x = eng->unk10.x;
        fe->unk10.y = 0;
        fe->unk10.x = x + dx;
        sc = ((eng->unk10.y + 0x1000) * 0xAF0) / 0x1000;
        fe->sub->unk20.x = sc;
        fe->sub->unk20.z = sc;
        fe->sub->unk52 = 1;
    }
    if (nas->UserInfo.status == 2) {
        fe->unk1E = eng->unk1E + 0x400;
        fe->sub->unk20.x = 0x3E8;
        fe->sub->unk20.z = 0xAF0;
        eng->Engine->unk5E = 1;
    }
}

static inline void SpawnPuff1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;

    p = &HeroModelBank[DB_TankObjects];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = kind;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
    }
}

bool MissileCollideWithPickup(EXTengine *eng) {
    s32 i;
    s32 j;
    NAS *pickup;
    EXTengine *pe;
    NAS **pp;
    NAS **tbl;
    s32 x;
    s32 y;
    s32 z;

    x = eng->unk10.x;
    y = eng->unk10.y;
    z = eng->unk10.z;
    for (i = 0;; i++) {
        if (i >= 3) {
            return 0;
        }
        pickup = PU_TA_list[i];
        if (pickup == NULL) {
            continue;
        }
        pe = pickup->Engine;
        if ((u32)(pe->unk10.x - x + 0xFF) < 0x1FF
            && (u32)(pe->unk10.z - z + 0xFF) < 0x1FF
            && (u32)(pe->unk10.y - y + 0xFF) < 0x1FF) {
            SpawnPuff1605(&pe->unk10, 0x10);
            PlaySfx(0x4B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            for (j = 0; j < 3; j++) {
                if (PU_TA_list[j] == pickup) {
                    PU_TA_list[j] = NULL;
                    break;
                }
            }
            KillNAS(pickup);
            return 1;
        }
    }
}

void PlotRock_TA(NAS *nas) {
    EXTengine *e;

    if (nas->UserInfo.status == 1) {
        e = nas->Engine;
        nas->UserInfo.status = 0;
        {
            s32 z = e->unk10.z >> 9;
            s32 x = e->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            TankLevelsGridMapMiss[(z * TankMapWidth) + x] = v;
            TankLevelsGridMapTank[(z * TankMapWidth) + x] = v;
        }
        {
            s32 z = nas->Engine->unk10.z >> 9;
            s32 x = nas->Engine->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            s32 i = (z * TankMapWidth) + x;
            TankLevelsGridMapMiss[i] = v;
            TankLevelsGridMapTank[i] = v;
            TankMapNas[i] = (s32)nas;
        }
    }
}

void PlotPeriscope(NAS *nas) {
    EXTobject_TA *fux;
    EXTengine *eng;
    EXTengine *e;
    NAS *n;
    subengine *sub;
    s32 i;
    s32 j;
    s32 frame;

    if (TankLevelOver != 0) {
        KillNAS(nas);
        return;
    }
    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    switch (fux->Status) {
    case 1:
        if (--fux->Counter == 30) {
            PeriscopePuff = TankPeriscopePuff(&nas->Engine->unk10, 1);
            PlaySfx(0x4FB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (fux->Counter <= 0) {
            fux->Status = 4;
            fux->Counter = 4;
        }
        break;
    case 4:
        TankPeriscopePuff(&nas->Engine->unk10, 0);
        fux->Counter--;
        switch (fux->Counter) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->entity = 0x2001;
            break;
        case 2:
            eng->sub->next->entity = 0x2002;
            break;
        case 1:
            eng->sub->next->entity = 0x2003;
            break;
        case 0:
            for (j = 0; j < 8; j++) {
                if (TankObjects[j] == NULL) {
                    TankObjects[j] = nas;
                    break;
                }
            }
            eng->sub->next->entity = 0x2004;
            fux->Status = 0;
            fux->Counter = 210;
            break;
        }
        break;
    case 0:
        if (!(GameFrameCount & 0x3F)) {
            TankPeriscopePuff(&nas->Engine->unk10, 0);
        }
        sub = eng->sub;
        eng->unk5E = 1;
        eng->unk1E += 30;
        sub = sub->next;
        sub->unk51 = 0;
        sub->unk52 = 1;
        if (--fux->Counter <= 0) {
            fux->Status = 5;
            fux->Counter = 4;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name != 9 && !(Heroes[i].hero_status & 0xFA00) && GameInfo[i + 0x5C] == 0) {
                e = Heroes[i].hero_eng;
                if ((u32)(((eng->unk1E - get_angle(eng->unk10.x - e->unk10.x, eng->unk10.z - e->unk10.z)) & 0xFFF) - 0x15) >= 0xFD7U) {
                    fux->Status = 10;
                    fux->Counter = 0;
                    PlaySfx(0x4FC, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                }
            }
        }
        break;
    case 10:
        if (!(GameFrameCount & 0x1F)) {
            TankPeriscopePuff(&nas->Engine->unk10, 0);
        }
        frame = PeriscopeFrames[fux->Counter];
        if (++fux->Counter == 9) {
            n = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1319, 0x11);
            if (n != NULL) {
                n->Engine->unk1E = eng->unk1E;
            }
        }
        if (frame == 0) {
            fux->Status = 11;
            fux->Counter = 4;
            break;
        }
        eng->sub->next->entity = frame;
        break;
    case 11:
        TankPeriscopePuff(&nas->Engine->unk10, 0);
        fux->Counter--;
        switch (fux->Counter) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->entity = 0x2003;
            break;
        case 2:
            for (j = 0; j < 8; j++) {
                if (TankObjects[j] == nas) {
                    TankObjects[j] = NULL;
                    break;
                }
            }
            eng->sub->next->entity = 0x2002;
            break;
        case 1:
            eng->sub->next->entity = 0x2001;
            break;
        case 0:
            fux->Status = 12;
            fux->Counter = 1;
            eng->sub->next->flags = 0;
            break;
        }
        break;
    case 12:
        if (--fux->Counter <= 0) {
            fux->Status = 1;
            fux->Counter = 120;
        }
        break;
    case 5:
        TankPeriscopePuff(&nas->Engine->unk10, 0);
        fux->Counter--;
        switch (fux->Counter) {
        case 3:
            eng->sub->next->flags = 0x8000;
            eng->sub->next->entity = 0x2003;
            break;
        case 2:
            eng->sub->next->entity = 0x2002;
            break;
        case 1:
            eng->sub->next->entity = 0x2001;
            break;
        case 0:
            fux->Status = 1;
            fux->Counter = 150;
            eng->sub->next->flags = 0;
            break;
        }
        break;
    }
}

void PlotPeriscopeGun(NAS *nas) {
    EXTengine *e;
    EXTengine *se;
    NAS *shell;
    EXTobject_TA *ext;
    s32 dx;
    s32 dz;

    if (TankLevelOver != 0) {
        KillNAS(nas);
        return;
    }
    e = nas->Engine;
    e->sub->entity++;
    switch (nas->Engine->sub->entity) {
    case 0x4081:
        shell = CreateNAS(e->unk10.x, e->unk10.y - 200, e->unk10.z, 0x130B, 0x11);
        if (shell == NULL) {
            return;
        }
        PlaySfx(0x4F3, &shell->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        ext = AddExt(shell, 0x38, 0x10000000);
        if (ext == NULL) {
            KillNAS(shell);
            return;
        }
        shell->FUX = (EXT *)ext;
        se = shell->Engine;
        dx = -(rcossin_tbl[(e->unk1E - 0x400) & 0xFFF].unk2 * 620);
        se->unk10.x += dx >> 12;
        dz = -(rcossin_tbl[(u16)e->unk1E & 0xFFF].unk2 * 620);
        se->unk10.z += dz >> 12;
        ext->SpeedY = -150;
        ext->Owner = -1;
        ext->SpeedX = dx >> 16;
        ext->SpeedZ = dz >> 16;
        ext->Status = 1;
        break;
    case 0x4087:
        KillNAS(nas);
        break;
    }
}

void PlotBoatField(NAS *nas) {
    EXTengine *src;
    EXTengine *dst;
    s32 *pos;
    u8 *p;
    NAS *n;
    EXTscriptdata *ext;
    u32 t;

    src = Heroes[nas->UserInfo.user1].hero_eng;
    dst = nas->Engine;
    dst->unk10.x = src->unk10.x;
    dst->unk10.z = src->unk10.z;
    t = nas->UserInfo.user2;
    nas->UserInfo.user2 = t + 1;
    switch (t) {
    case 0:
    case 4:
    case 9:
    case 14:
        pos = (s32 *)&dst->unk10;
        p = &HeroModelBank[DB_TankVehicle];
        n = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 4;
                ext->speed = 0x100;
                if (p != NULL) {
                    ext->flags = 5;
                    n->Engine->sub->u6C.dbase = p;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                n->InitDelete(n);
            }
        }
        break;
    case 29:
        KillNAS(nas);
        break;
    }
}

void PlotPillarControl(NAS *nas) {
    s32 *p;
    s32 delay;
    s32 x;
    s32 z;
    s32 v;
    NAS *obj;

    if (TankLevelOver == 0 && --nas->UserInfo.flags == 0) {
        PlaySfx(0x501, NULL, NULL, 0x1000, 0x1E00, 0);
        p = PillarWaves[PillarWave++];
        delay = *p++;
        while ((v = *p++) >= 0) {
            x = (v << 9) + 0x100;
            z = (*p++ << 9) + 0x100;
            obj = CreateNAS(x, 0, z, 0x1311, 0x11);
            if (obj != NULL) {
                obj->Engine->sub->entity = *p++;
                obj->UserInfo.user2 = delay;
                obj = CreateNAS(x, 0, z, 0x1312, 0x11);
                if (obj != NULL) {
                    obj->Engine->sub->entity = 0x4000;
                    obj->UserInfo.user1 = 6;
                    obj->UserInfo.status = 0;
                }
            }
        }
        if (PillarWave < 0x20) {
            nas->UserInfo.flags = delay + 0x46;
        } else {
            KillNAS(nas);
        }
    }
}

void PlotPillar(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    NAS *obj;
    s32 i;
    s32 d;
    s32 x;
    s32 z;
    s32 r;
    s32 n;
    s32 hit;
    u16 name;

    eng = nas->Engine;
    switch (nas->UserInfo.user1) {
    case 1:
        if (--nas->UserInfo.status != 0) {
            return;
        }
        r = 0x1FF;
        x = eng->unk10.x;
        z = eng->unk10.z;
        for (i = 0; i < 4; i++) {
            name = Heroes[i].hero_name;
            if (name == 9 &&
                (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
                 (u32)(i - 2) >= 2)) {
                continue;
            }
            if (Heroes[i].hero_status & 0xC000) {
                continue;
            }
            e = Heroes[i].hero_eng;
            d = x - e->unk10.x;
            if (d < -0x1FF) {
                continue;
            }
            if (r < d) {
                continue;
            }
            d = z - e->unk10.z;
            n = -0x1FF;
            if (n < d && d < 0x1FF) {
                hit = i;
                goto found;
            }
        }
        hit = -1;
    found:
        if (hit < 0) {
            if (TankLevelsGridMapMiss[((eng->unk10.z >> 9) * TankMapWidth) + (eng->unk10.x >> 9)] != 0) {
                KillNAS(nas);
                return;
            }
            nas->UserInfo.user1 = 4;
            {
                s32 z = eng->unk10.z >> 9;
                s32 x = eng->unk10.x >> 9;
                u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
                TankLevelsGridMapMiss[(z * TankMapWidth) + x] = v;
                TankLevelsGridMapTank[(z * TankMapWidth) + x] = v;
            }
            eng->sub->unk68 = 0;
            eng->sub->flags &= ~0x02000000;
            MissileCollideWithPickup(eng);
            return;
        }
        if (nas->UserInfo.user2 >= 2) {
            nas->UserInfo.user2--;
        }
        nas->UserInfo.status++;
        return;
    case 4:
        eng->unk10.y -= 0x1E;
        if (eng->unk10.y <= 0) {
            eng->unk10.y = 0;
            nas->UserInfo.user1 = 0;
        }
        return;
    case 0:
    case 3:
    default:
        if (TankLevelOver == 0 && --nas->UserInfo.user2 <= 0) {
            nas->UserInfo.user1 = 5;
            eng->sub->unk68 = 0x20;
            TankPillarSfxDelay = 2;
            eng->sub->flags |= 0x02000000;
        }
        return;
    case 5:
        eng->unk10.y += 0x1E;
        if (eng->unk10.y >= 0xB8) {
            obj = CreateNAS(eng->unk10.x, 0, eng->unk10.z, 0x1312, 0x11);
            if (obj != NULL) {
                obj->UserInfo.user1 = 7;
                obj->UserInfo.status = 3;
                obj->Engine->sub->entity = 0x400F;
            }
            {
                s32 z = eng->unk10.z >> 9;
                s32 x = eng->unk10.x >> 9;
                TankLevelsGridMapMiss[(z * TankMapWidth) + x] = 0;
                TankLevelsGridMapTank[(z * TankMapWidth) + x] = 0;
            }
            nas->UserInfo.user1 = 2;
            nas->UserInfo.status = 4;
        }
        return;
    case 2:
        if (--nas->UserInfo.status == 0) {
            KillNAS(nas);
        }
        return;
    }
}

static inline s32 TotemFindHero(s32 x, s32 z, s32 r) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = x - e->unk10.x;
        if (d < -r) {
            continue;
        }
        if (r < d) {
            continue;
        }
        d = z - e->unk10.z;
        if (-r < d && d < r) {
            return i;
        }
    }
    return -1;
}

void PlotTotemPole(NAS *nas) {
    EXTobject_TA *fux;
    EXTengine *eng;
    EXTengine *e;
    s32 i;

    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    switch (fux->Status) {
    case 1:
        if (--fux->Counter > 0) {
            return;
        }
        if (((TankGameView *)GameInfo)->flags & 0x80) {
            fux->Status = 0x24;
            PlaySfx(0x502, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            nas->UserInfo.user2 = 0x1E;
            return;
        }
        if (TotemFindHero(0x590, 0x400, 0x200) < 0) {
            fux->Counter = 0x12C;
            fux->Status = 0x11;
            return;
        }
        fux->Counter++;
        return;
    case 0x11:
        if (--fux->Counter > 0) {
            return;
        }
        if (TotemFindHero(0xA40, 0x440, 0x200) >= 0) {
            fux->Counter++;
            return;
        }
        fux->Status = 0;
        return;
    case 0:
    case 2:
    default:
        return;
    case 0x24:
        if (nas->UserInfo.user2 > 0) {
            nas->UserInfo.user2--;
            return;
        }
        eng->unk5E = 1;
        eng->unk1E += 0x22;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == 9) {
                continue;
            }
            if (Heroes[i].hero_status & 0xFA00) {
                continue;
            }
            if (GameInfo[i + 0x5C] != 0) {
                continue;
            }
            e = Heroes[i].hero_eng;
            if ((u32)(((eng->unk1E - get_angle(eng->unk10.x - e->unk10.x, eng->unk10.z - e->unk10.z)) & 0xFFF) - 0x15) < 0xFD7) {
                continue;
            }
            fux->Status = 0xD;
            fux->Counter = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x503, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        return;
    }
}

void InitTotemBit(NAS *nas, s32 id) {
    EXTobject_TA *ext;
    EXTengine *eng;
    subengine *sub;
    s32 i;
    NAS **p;

    nas->UserInfo.user1 = 1;
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        goto kill;
    }
    nas->FUX = (EXT *)ext;
    ext->Status = 1;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk1E = 0;
    sub->u6C.dbase = &HeroModelBank[DB_ArenaTankJungleObjects];
    if (id == 0x131B) {
        sub->entity = 0x4180;
        ext->TargetIndex = 0;
    } else {
        sub->entity = 0x4180;
        ext->TargetIndex = 0xD;
    }
    ext->TargetX = TotemTargets[ext->TargetIndex][0];
    ext->TargetZ = *(s32 *)((u8 *)TotemTargets + ((ext->TargetIndex * 8) | 4));
    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    for (i = 0; i < 8; i++) {
        if (TankObjects[i] == NULL) {
            TankObjects[i] = nas;
            break;
        }
    }
    if (nas->Failure != 0) {
kill:
        KillNAS(nas);
    }
}

void TotemBitExplode(EXTengine *arg0) {
    NAS *nas;
    EXTscriptdata *ext;
    SLONG_XYZ *pos;
    u8 *p;
    s32 saved;
    s32 x;
    s32 z;
    s32 d;
    s32 i;
    s32 lim;
    HeroInfo *h;

    saved = arg0->unk10.y;
    arg0->unk10.y = 0x12C;
    pos = &arg0->unk10;
    p = &HeroModelBank[DB_ArenaTankJungleObjects];
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->scriptnumber = 0;
            ext->speed = 0x100;
            if (p != NULL) {
                ext->flags = 5;
                nas->Engine->sub->u6C.dbase = p;
            } else {
                ext->flags = 1;
            }
            ext->maxloops = 0;
            nas->InitDelete(nas);
        }
    }
    lim = 0x300;
    i = 0;
    h = Heroes;
    x = arg0->unk10.x;
    z = arg0->unk10.z;
    for (; i < 4; i++, h++) {
        d = x - h->hero_eng->unk10.x;
        if (d >= -0x300 && d <= lim) {
            d = z - h->hero_eng->unk10.z;
            if (d >= -0x300 && d <= lim) {
                DamageTankHero(i, 0x3C);
            }
        }
    }
    arg0->unk10.y = saved;
    PlaySfx(0x505, NULL, NULL, 0x1000, 0x1E00, 0);
}

void PlotTotemBit(NAS *nas, s32 id) {
    EXTengine *eng;
    s32 v;
    NAS *pole;
    EXTobject_TA *pf;
    EXTobject_TA *ext;
    NAS *n;
    EXTengine *ne;
    EXTobject_TA *nx;
    s32 x;
    s32 z;
    s32 k;
    s32 t;
    s32 a;

    eng = nas->Engine;
    ext = (EXTobject_TA *)nas->FUX;
    x = eng->unk10.x;
    z = eng->unk10.z;
    switch ((u32)ext->Status) {
    case 1:
        pole = TotemPole;
        pf = (EXTobject_TA *)pole->FUX;
        switch (pf->Status) {
        case 0xD:
            if (id == 0x131B) {
                ext->Status = 0xD;
                ext->pAnimList = TotemAnim0D;
            }
            break;
        case 0x24:
            if (eng->unk1E != pole->Engine->unk1E) {
                eng->unk1E = pole->Engine->unk1E;
                eng->unk5E = 1;
            }
            break;
        case 0x11:
            if (id == 0x131B) {
                ext->Status = 5;
                ext->SpeedY = -0x5A;
                ext->SpeedX = -0x1E;
                ext->SpeedZ = -0x2D;
                PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            break;
        case 0:
            if (id == 0x131C) {
                ext->Status = 5;
                ext->SpeedY = -0x5A;
                ext->SpeedX = 0x1E;
                ext->SpeedZ = -0x2D;
                PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            break;
        }
        break;
    case 13:
        v = *ext->pAnimList++;
        if (v != 0) {
            if (v == -1) {
            n = CreateNAS(eng->unk10.x, eng->unk10.y - 100, eng->unk10.z, 0x130E, 0x11);
            if (n != NULL) {
                ne = n->Engine;
                nx = (EXTobject_TA *)n->FUX;
                ne->sub->u6C.dbase = &HeroModelBank[DB_ArenaTankJungleObjects];
                ne->sub->entity = 0x2006;
                nx->Owner = v;
                ne->unk10.x += -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 300) >> 12;
                ne->unk10.z += -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 300) >> 12;
                ne->unk1E = eng->unk1E;
                nx->SpeedX = -(rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 * 60) >> 12;
                nx->SpeedZ = -(rcossin_tbl[eng->unk1E & 0xFFF].unk2 * 60) >> 12;
            }
            } else {
                eng->sub->entity = v;
            }
        } else {
            pole = TotemPole;
            ext->Status = 1;
            eng->sub->entity = 0x2005;
            pf = (EXTobject_TA *)pole->FUX;
            pf->Status = 0x24;
            PlaySfx(0x502, &pole->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            pole->UserInfo.user2 = 0x1E;
        }
        break;
    case 5:
        ext->SpeedY += 12;
        eng->unk10.y += ext->SpeedY;
        x = eng->unk10.x + ext->SpeedX;
        z = eng->unk10.z + ext->SpeedZ;
        if (eng->unk10.y < 0) {
            break;
        }
        eng->unk10.y = 0;
        if (ext->SpeedY >= 0x25) {
            TotemBitExplode(eng);
        } else {
            PlaySfx(0x506, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (ext->SpeedY < 13) {
            ext->Status = 0;
            ext->Counter = 0x5A;
        } else {
            ext->SpeedY = -(ext->SpeedY * 2) / 3;
            ext->SpeedX = (ext->SpeedX * 2) / 3;
            ext->SpeedZ = (ext->SpeedZ * 2) / 3;
        }
        break;
    case 0:
        if (--ext->Counter > 0) {
            break;
        }
        ext->Status = 0x25;
        ext->pAnimList = TotemAnim25;
        nas->UserInfo.status = 0;
        ext->TargetIndex = (ext->TargetIndex + 1) & 0xF;
        ext->TargetX = TotemTargets[ext->TargetIndex][0];
        ext->TargetZ = *(s32 *)((u8 *)TotemTargets + ((ext->TargetIndex * 8) | 4));
        ext->SpeedX = (ext->TargetX - eng->unk10.x) / 31;
        ext->SpeedZ = (ext->TargetZ - eng->unk10.z) / 31;
        a = get_angle(ext->SpeedX, ext->SpeedZ) + 0x800;
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        a /= 12;
        ext->Speed = a;
        PlaySfx(0x507, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 0x25:
        v = *ext->pAnimList++;
        if (v != 0) {
            eng->sub->entity = v;
            k = nas->UserInfo.status++;
            k -= 16;
            if (k > 0) {
                eng->unk1E += ext->Speed;
                eng->unk5E = 1;
                x = eng->unk10.x + ext->SpeedX * k / 12;
                z = eng->unk10.z + ext->SpeedZ * k / 12;
            }
        } else {
            ext->pAnimList = TotemAnim29;
            ext->Status = 0x29;
        }
        break;
    case 0x29:
        v = *ext->pAnimList++;
        if (v == 0) {
            ext->Status = 0x26;
            ext->pAnimList = TotemAnim26;
            nas->UserInfo.status = 0;
            a = -eng->unk1E;
            a = (a << 20) >> 20;
            a /= 12;
            ext->Speed = a;
            break;
        }
        eng->sub->entity = v;
        x = eng->unk10.x + ext->SpeedX;
        z = eng->unk10.z + ext->SpeedZ;
        break;
    case 0x26:
        v = *ext->pAnimList++;
        if (v != 0) {
            eng->sub->entity = v;
            t = nas->UserInfo.status++;
            if (t < 12) {
                t = 12 - t;
                x = eng->unk10.x + ext->SpeedX * t / 12;
                z = eng->unk10.z + ext->SpeedZ * t / 12;
                eng->unk1E += ext->Speed;
                eng->unk5E = 1;
            }
        } else if (TankLevelOver != 0) {
            ext->Status = 0;
            eng->sub->entity = 0x2004;
            ext->Counter = 0xB9A;
        } else {
            ext->pAnimList = TotemAnim27;
            ext->Status = 0x27;
            PlaySfx(0x504, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x27:
    next:
        v = *ext->pAnimList++;
        if (v != 0) {
            eng->sub->entity = v;
            if (v == -1) {
                TotemBitExplode(eng);
                goto next;
            }
        } else {
            ext->Status = 0;
            eng->sub->entity = 0x2004;
            ext->Counter = 0x96;
        }
        break;
    }
    if (TankRocketFindHero(x, z, 0x180) < 0) {
        eng->unk10.x = x;
        eng->unk10.z = z;
    }
    if (eng->unk10.x > TankArenaMaxX) {
        eng->unk10.x = TankArenaMaxX;
    }
    if (eng->unk10.x < 0x100) {
        eng->unk10.x = 0x100;
    }
    if (eng->unk10.z > TankArenaMaxZ) {
        eng->unk10.z = TankArenaMaxZ;
    }
    if (eng->unk10.z < 0x100) {
        eng->unk10.z = 0x100;
    }
    MissileCollideWithPickup(eng);
}

void CheckMineToBoss(s32 x, s32 y, s32 z) {
    s32 i;

    switch (TankBossStatus) {
    case 0x2F:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 3) {
                break;
            }
            nas = TankBossCrashTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (TankWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2006;
                    TankSpawnBit1605B(&eng->unk10, 0);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2008;
                    TankSpawnBit1605B(&eng->unk10, 0);
                }
            }
        }
        break;
    case 0x33:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 4) {
                break;
            }
            nas = TankBossCortexTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (TankWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2007;
                    TankSpawnBit1605B(&eng->unk10, 1);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2005;
                    TankSpawnBit1605B(&eng->unk10, 1);
                }
            }
        }
        break;
    case 0x37:
        for (i = 0;; i++) {
            NAS *nas;
            EXTengine *eng;

            if (i >= 2) {
                break;
            }
            nas = TankBossRocketTurrets[i];
            if (nas == NULL || nas->UserInfo.flags == 2) {
                continue;
            }
            eng = nas->Engine;
            if (TankWithin(eng, x, y, z, 0x250, 0x200, 0x500)) {
                if (++nas->UserInfo.flags == 1) {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2025;
                    TankSpawnBit1605B(&eng->unk10, 2);
                } else {
                    nas->UserInfo.user2 = 0x3C;
                    nas->UserInfo.status = 0;
                    eng->sub->entity = 0x2026;
                    TankSpawnBit1605B(&eng->unk10, 2);
                }
            }
        }
        break;
    }
}

void SpawnTankBossTurrets(void) {
    TankBossCrashTurrets[0] = CreateNAS(0x369, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCrashTurrets[1] = CreateNAS(0x900, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCrashTurrets[2] = CreateNAS(0xE97, 0x7D0, 0x10E0, 0x1315, 0x11);
    TankBossCortexTurrets[0] = CreateNAS(0x300, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[1] = CreateNAS(0x700, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[2] = CreateNAS(0xB00, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossCortexTurrets[3] = CreateNAS(0xF00, 0x7D0, 0x10E0, 0x1316, 0x11);
    TankBossRocketTurrets[0] = CreateNAS(0x300, 0x7D0, 0x1200, 0x1317, 0x11);
    TankBossRocketTurrets[1] = CreateNAS(0xF00, 0x7D0, 0x1200, 0x1317, 0x11);
}

void InitTankBoss(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    EXTobject_TA *ext;
    u8 *base;
    u8 *model;

    ResetHeroGlobals();
    TankBossNas = nas;
    eng = nas->Engine;
    TankBossEng = eng;
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    model = TANK_MODEL(57);
    nas->FUX = (EXT *)ext;
    sub = eng->sub;
    sub->u6C.dbase = model;
    sub->entity = 0x2002;
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    eng->unk5D = 0;
    eng->unk10.y = 0x7D0;
    ext->Status = TankBossStartStatus;
    TankBossStatus = TankBossStartStatus;
    switch (TankBossStartStatus) {
    case 1:
        base = (u8 *)HeroModelBank;
        if (TANK_MODEL_ON(base, 59)) {
            dbase_release(TANK_MODEL(59));
        }
        if (!TANK_MODEL_ON(base, 58)) {
            dbase_asload_with_anims(&HeroModelBank[DB_ArenaBossKomodoKomodo], 0);
        }
        if (!TANK_MODEL_ON(base, 57)) {
            dbase_asload_with_anims(&HeroModelBank[DB_ArenaBossKomodoArenaobjects], 0);
        }
        ext->Counter = 0x5A;
        TankBossStartStatus = 0x2C;
        SpawnTankBossTurrets();
        break;
    case 0x2C:
        base = (u8 *)HeroModelBank;
        if (TANK_MODEL_ON(base, 59)) {
            dbase_release(TANK_MODEL(59));
        }
        if (TANK_MODEL_ON(base, 58)) {
            dbase_release(TANK_MODEL(58));
        }
        if (!TANK_MODEL_ON(base, 57)) {
            dbase_asload_with_anims(&HeroModelBank[DB_ArenaBossKomodoArenaobjects], 0);
        }
        ext->Counter = 0x3C;
        SpawnTankBossTurrets();
        break;
    case 0x3A:
        base = (u8 *)HeroModelBank;
        if (TANK_MODEL_ON(base, 58)) {
            dbase_release(TANK_MODEL(58));
        }
        if (TANK_MODEL_ON(base, 57)) {
            dbase_release(TANK_MODEL(57));
        }
        if (!TANK_MODEL_ON(base, 59)) {
            dbase_asload_with_anims(&HeroModelBank[DB_ArenaBossKomodoKomodotank], 0);
        }
        ext->Counter = 0x3C;
        break;
    }
    if (nas->Failure) {
        KillNAS(nas);
    }
}

void PlotTankBoss(NAS *nas) {
    EXTobject_TA *fux;
    EXTengine *eng;

    if (TankLevelOver != 0) {
        return;
    }
    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    switch (fux->Status) {
    case 1:
        if (--fux->Counter <= 0) {
            fux->Status = 0x2C;
            fux->Counter = 0x96;
        }
        break;
    case 0x2C:
        if (--fux->Counter <= 0) {
            fux->Status = 0x2D;
            PlaySfx(0x258, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x2D:
        if ((eng->unk10.y -= 0x1E) <= 0) {
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x259, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            eng->unk10.y = 0;
            fux->Status = 0x2E;
            fux->Counter = 0x1E;
            PlaySfx(0x25B, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x2E:
        if (--fux->Counter <= 0) {
            fux->Status = 0x2F;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        break;
    case 0x2F:
        if (TankBossCrashTurrets[0]->UserInfo.flags == 2 &&
            TankBossCrashTurrets[1]->UserInfo.flags == TankBossCrashTurrets[0]->UserInfo.flags &&
            TankBossCrashTurrets[2]->UserInfo.flags == TankBossCrashTurrets[1]->UserInfo.flags) {
            fux->Status = 0x30;
            fux->Counter = 0x2D;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x30:
        eng->unk10.y += TankBossBob[fux->Counter & 0xF];
        if (--fux->Counter <= 0) {
            fux->Status = 0x31;
        }
        break;
    case 0x31:
        if ((eng->unk10.y += 10) >= 0x2B6) {
            KillNAS(TankBossCrashTurrets[0]);
            KillNAS(TankBossCrashTurrets[1]);
            KillNAS(TankBossCrashTurrets[2]);
            eng->unk10.y = 0x2B6;
            fux->Status = 0x32;
            fux->Counter = 0x33;
            PlaySfx(0x25C, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x32:
        if (--fux->Counter <= 0) {
            fux->Status = 0x33;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            TankPendingSfx = 0x25D;
        }
        break;
    case 0x33:
        if (TankBossCortexTurrets[0]->UserInfo.flags == 2 &&
            TankBossCortexTurrets[1]->UserInfo.flags == TankBossCortexTurrets[0]->UserInfo.flags &&
            TankBossCortexTurrets[2]->UserInfo.flags == TankBossCortexTurrets[1]->UserInfo.flags &&
            TankBossCortexTurrets[3]->UserInfo.flags == TankBossCortexTurrets[2]->UserInfo.flags) {
            fux->Status = 0x34;
            fux->Counter = 0x2D;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x34:
        eng->unk10.y += TankBossBob[fux->Counter & 0xF];
        if (--fux->Counter <= 0) {
            fux->Status = 0x35;
        }
        break;
    case 0x35:
        if ((eng->unk10.y += 10) >= 0x4BF) {
            KillNAS(TankBossCortexTurrets[0]);
            KillNAS(TankBossCortexTurrets[1]);
            KillNAS(TankBossCortexTurrets[2]);
            KillNAS(TankBossCortexTurrets[3]);
            eng->unk10.y = 0x4BF;
            fux->Status = 0x36;
            fux->Counter = 0x3C;
            PlaySfx(0x25C, &eng->sfx, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x36:
        if (--fux->Counter <= 0) {
            fux->Status = 0x37;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            TankPendingSfx = 0x25D;
        }
        break;
    case 0x37:
        if (TankBossRocketTurrets[0]->UserInfo.flags == 2 &&
            TankBossRocketTurrets[1]->UserInfo.flags == TankBossRocketTurrets[0]->UserInfo.flags) {
            fux->Status = 0x38;
            fux->Counter = 0x5A;
            PlaySfx(0x25A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 0x38:
        eng->unk10.y += TankBossBob[fux->Counter & 0xF];
        if (--fux->Counter <= 0) {
            fux->Status = 0x39;
            fux->Counter = 0x5A;
        }
        break;
    case 0x39:
        if ((eng->unk10.y += 10) >= 0x7D0) {
            KillNAS(TankBossRocketTurrets[0]);
            KillNAS(TankBossRocketTurrets[1]);
            TankBossStartStatus = 0x3A;
            GameAdventure.unkAB[9] = 0;
            fux->Counter = 0x5A;
            fux->Status = 0x3A;
            PlaySfx(0x25D, NULL, NULL, 0x1000, 0x1E00, 0);
            dbase_release(&HeroModelBank[DB_ArenaBossKomodoArenaobjects]);
            dbase_asload_with_anims(&HeroModelBank[DB_ArenaBossKomodoKomodotank], 0);
        }
        break;
    case 0x3A:
        if (--fux->Counter == 0) {
            eng->flags &= ~0x8000;
            fux->Status = 0;
            ResetToBasicWeapon(0);
            if (GameTeams[0] != 0) {
                ResetToBasicWeapon(1);
            }
        }
        break;
    case 0:
        if (MoeState == 0x26 && JoeState == MoeState) {
            BossState.flags |= 0x8000;
        }
        break;
    }
    TankBossStatus = fux->Status;
}

s32 TankBossTurretFindTarget(EXTengine *eng) {
    s32 x;
    s32 z;
    s32 ang;

    if (TankLevelOver != 0) {
        return -1;
    }
    x = eng->unk10.x;
    z = eng->unk10.z;
    ang = eng->unk1E;
    if ((u32)Random(0x20) < (u32)AiPlayerSkill[3].level) {
        return -1;
    }
    if (Heroes[0].hero_health <= 0 || (Heroes[0].hero_status & 0xFA00)) {
        TankBossTargets[0].z = -1;
    } else {
        TankBossTargets[0].x = Heroes[0].hero_eng->unk10.x;
        TankBossTargets[0].z = Heroes[0].hero_eng->unk10.z;
    }
    if (GameTeams[0] == 0 || Heroes[1].hero_health <= 0 || (Heroes[1].hero_status & 0xFA00)) {
        TankBossTargets[1].z = -1;
    } else {
        TankBossTargets[1].x = Heroes[1].hero_eng->unk10.x;
        TankBossTargets[1].z = Heroes[1].hero_eng->unk10.z;
    }
    if (TankBossStatus == 0x37) {
        if (TankTurretSees(0, 0, x, z, ang)) {
            return 0;
        }
        if (TankTurretSees(1, 1, x, z, ang)) {
            return 1;
        }
        return -1;
    }
    if (TankBossTargets[0].z > 0x600 || TankBossTargets[1].z > 0x600) {
        if (TankBossTargets[0].z > 0x600 && TankTurretSees(0, 0, x, z, ang)) {
            return 0;
        }
        if (TankBossTargets[1].z > 0x600 && TankTurretSees(1, 1, x, z, ang)) {
            return 1;
        }
        return -1;
    }
    switch ((u32)Random(3)) {
    case 0:
        if (TankBossSightedHero != 2 && TankTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (TankBossSightedHero != 3 && TankTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (TankBossSightedHero != 4 && TankTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    case 1:
        if (TankBossSightedHero != 4 && TankTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (TankBossSightedHero != 3 && TankTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (TankBossSightedHero != 2 && TankTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    case 2:
        if (TankBossSightedHero != 3 && TankTurretSees(3, 3, x, z, ang)) {
            return 3;
        }
        if (TankBossSightedHero != 2 && TankTurretSees(2, 2, x, z, ang)) {
            return 2;
        }
        if (TankBossSightedHero != 4 && TankTurretSees(4, 4, x, z, ang)) {
            return 4;
        }
        break;
    }
    return -1;
}

void TankBossCrashTurretFire(EXTengine *arg0) {
    NAS *shell;
    EXTobject_TA *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = CreateNAS(arg0->unk10.x, arg0->unk10.y - 0xF0, arg0->unk10.z, 0x130E, 0x11);
    if (shell == NULL) {
        return;
    }
    ext = (EXTobject_TA *)shell->FUX;
    se = shell->Engine;
    ext->Counter *= 2;
    PlaySfx(0x4CE, &se->sfx, NULL, 0x1000, 0x1E00, 0);
    se->unk10.x += -(rcossin_tbl[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12;
    se->unk10.z += -(rcossin_tbl[(u16)arg0->unk1E & 0xFFF].unk2 * 400) >> 12;
    se->unk1E = arg0->unk1E;
    ext->SpeedX = -(rcossin_tbl[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 58) >> 12;
    ext->SpeedY = -0x10;
    dz = rcossin_tbl[(u16)arg0->unk1E & 0xFFF].unk2;
    ext->Status = 1;
    ext->Owner = -1;
    ext->SpeedZ = -(dz * 58) >> 12;
loop:
    if (se->unk10.z > TankArenaMaxZ + 0x100) {
        se->unk10.z -= 0x10;
        goto loop;
    }
    n = TankSpawnBit1605Nas(&shell->Engine->unk10, 6);
    if (n != NULL) {
        ang = arg0->unk1E;
        n->Engine->unk1E = ang;
        n->Engine->unk5E = 1;
        for (sub = n->Engine->sub; sub != NULL; sub = sub->next) {
            sub->flags |= 0x20000000;
        }
    }
}

void PlotTankBossCrashTurret(NAS *nas) {
    EXTengine *eng;
    s32 a;
    s32 t;
    s32 u;
    s32 r;
    s32 v;
    s32 k;
    u16 *tbl;

    eng = nas->Engine;
    eng->unk10.y = TankBossEng->unk10.y - 0x46;
    switch (nas->UserInfo.status) {
    case 4:
        nas->UserInfo.user2--;
        if (TankBossStatus == 0x2E) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        eng->unk10.z -= 0x30;
        nas->UserInfo.flags -= 0x30;
        if (nas->UserInfo.flags > 1000) {
            nas->UserInfo.flags = 0;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 39;
            nas->UserInfo.user1 = 0;
        }
        break;
    case 39:
        a = eng->unk10.x;
        switch (a) {
        case 0x300:
        default:
            a = 200;
            break;
        case 0xE97:
            a = -200;
            break;
        case 0x900:
            a = 0x200;
            break;
        }
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        if (a >= 0) {
            if (a < 20) {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 10, -1);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E += 20;
        } else {
            if (a >= -19) {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 10, -1);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E -= 20;
        }
        eng->unk5E = 1;
        break;
    case 13:
        k = nas->UserInfo.user2;
        u = k + 1;
        nas->UserInfo.user2 = u;
        if (nas->UserInfo.flags != 0) {
            tbl = CrashTurretFramesAlt;
        } else {
            tbl = CrashTurretFrames;
        }
        u *= 2;
        u += (s32)tbl;
        v = *(u16 *)u;
        nas->UserInfo.user2 = k + 2;
        if (v == 0) {
            if (nas->UserInfo.flags != 0) {
                eng->sub->entity = 0x2006;
            } else {
                eng->sub->entity = 0x2003;
            }
            nas->UserInfo.user2 = 60;
            nas->UserInfo.status = 0;
            eng->unk1E &= 0xFFF;
        } else {
            eng->sub->entity = v;
        }
        break;
    case 0:
        if (nas->UserInfo.flags != 2) {
            if (eng->unk1E <= 16) {
                nas->UserInfo.user2 = 0;
                nas->UserInfo.status = 14;
            } else {
                if (eng->unk1E < 0x800) {
                    eng->unk1E -= 16;
                } else {
                    eng->unk1E += 16;
                }
                eng->unk5E = 1;
                eng->unk1E &= 0xFFF;
            }
        }
        break;
    case 14:
        if (nas->UserInfo.user1 & 1) {
            eng->unk1E += 16;
        } else {
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        t = eng->unk1E &= 0xFFF;
        if ((u32)(t - 900) < 0x8F9U) {
            nas->UserInfo.user2 = 0;
            if (nas->UserInfo.user1 & 1) {
                nas->UserInfo.status = 15;
            } else {
                nas->UserInfo.status = 16;
            }
        }
        break;
    case 15:
        do {
            eng->unk1E = (eng->unk1E - 16) & 0xFFF;
        } while (0);
        u = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(u - 900) < 0x8F9U) {
            eng->unk1E = u + 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 16;
            break;
        }
        r = TankBossTurretFindTarget(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 10, r);
            } else {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 3, r);
            }
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    case 16:
        do {
            eng->unk1E = (eng->unk1E + 16) & 0xFFF;
        } while (0);
        t = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(t - 900) < 0x8F9U) {
            eng->unk1E = t - 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 15;
            break;
        }
        r = TankBossTurretFindTarget(eng);
        if (r >= 0) {
            nas->UserInfo.user1++;
            if (nas->UserInfo.user1 & 1) {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 10, r);
            } else {
                ((TankTurretFireFn)TankBossCrashTurretFire)(eng, 3, r);
            }
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void TankBossCortexTurretFire(EXTengine *arg0) {
    NAS *shell;
    EXTobject_TA *ext;
    EXTengine *se;
    NAS *n;
    subengine *sub;
    s32 dz;
    s32 ang;

    shell = CreateNAS(arg0->unk10.x, arg0->unk10.y - 0x113, arg0->unk10.z, 0x130C, 0x11);
    if (shell == NULL) {
        return;
    }
    ext = (EXTobject_TA *)shell->FUX;
    ext->Counter = ext->Counter * 5 / 3;
    se = shell->Engine;
    PlaySfx(0x4CC, &se->sfx, NULL, 0x1000, 0x1E00, 0);
    se->unk10.x += -(rcossin_tbl[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 400) >> 12;
    se->unk10.z += -(rcossin_tbl[(u16)arg0->unk1E & 0xFFF].unk2 * 400) >> 12;
    se->unk1E = arg0->unk1E;
    ext->SpeedX = -(rcossin_tbl[(arg0->unk1E - 0x400) & 0xFFF].unk2 * 74) >> 12;
    dz = rcossin_tbl[(u16)arg0->unk1E & 0xFFF].unk2;
    ext->Status = 1;
    ext->Owner = -1;
    ext->SpeedZ = -(dz * 74) >> 12;
loop:
    if (se->unk10.z > TankArenaMaxZ + 0x100) {
        se->unk10.z -= 0x10;
        goto loop;
    }
    n = TankSpawnBit1605Nas(&shell->Engine->unk10, 7);
    if (n != NULL) {
        ang = arg0->unk1E;
        n->Engine->unk1E = ang;
        n->Engine->unk5E = 1;
        for (sub = n->Engine->sub; sub != NULL; sub = sub->next) {
            sub->flags |= 0x20000000;
        }
    }
}

void PlotTankBossCortexTurret(NAS *nas) {
    EXTengine *eng;
    s32 a;
    s32 t;
    s32 u;

    eng = nas->Engine;
    eng->unk10.y = TankBossEng->unk10.y - 0x2B6;
    switch (nas->UserInfo.status) {
    case 17:
        if (TankBossStatus == 0x32) {
            eng->unk5E = 1;
            eng->unk1C += 20;
        } else if (TankBossStatus == 0x33) {
            nas->UserInfo.flags = 0;
            nas->UserInfo.status = 39;
            nas->UserInfo.user1 = 0;
        }
        break;
    case 39:
        a = eng->unk10.x;
        switch (a) {
        case 0x300:
        default:
            a = 300;
            break;
        case 0xF00:
            a = -300;
            break;
        case 0x700:
            a = -200;
            break;
        case 0xB00:
            a = 200;
            break;
        }
        a -= eng->unk1E;
        a = (a << 20) >> 20;
        if (a >= 0) {
            if (a < 20) {
                TankBossCortexTurretFire(eng);
                nas->UserInfo.status = 13;
                nas->UserInfo.user2 = 0;
                break;
            }
            eng->unk1E += 16;
        } else {
            if (a >= -19) {
                TankBossCortexTurretFire(eng);
                nas->UserInfo.status = 13;
                break;
            }
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        break;
    case 13:
        nas->UserInfo.user2 = 60;
        nas->UserInfo.status = 0;
        break;
    case 0:
        if (nas->UserInfo.flags != 2) {
            if (eng->unk1E <= 16) {
                nas->UserInfo.user2 = 0;
                nas->UserInfo.status = 14;
            } else {
                if (eng->unk1E < 0x800) {
                    eng->unk1E -= 16;
                } else {
                    eng->unk1E += 16;
                }
                eng->unk5E = 1;
                eng->unk1E &= 0xFFF;
            }
        }
        break;
    case 14:
        if (nas->UserInfo.user1 & 1) {
            eng->unk1E += 16;
        } else {
            eng->unk1E -= 16;
        }
        eng->unk5E = 1;
        t = eng->unk1E &= 0xFFF;
        if ((u32)(t - 900) < 0x8F9U) {
            nas->UserInfo.user2 = 0;
            if (nas->UserInfo.user1 & 1) {
                nas->UserInfo.status = 15;
            } else {
                nas->UserInfo.status = 16;
            }
        }
        break;
    case 15:
        do {
            eng->unk1E = (eng->unk1E - 16) & 0xFFF;
        } while (0);
        u = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(u - 900) < 0x8F9U) {
            eng->unk1E = u + 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 16;
            break;
        }
        goto fire;
    case 16:
        do {
            eng->unk1E = (eng->unk1E + 16) & 0xFFF;
        } while (0);
        t = eng->unk1E;
        eng->unk5E = 1;
        if ((u32)(t - 900) < 0x8F9U) {
            eng->unk1E = t - 16;
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 15;
            break;
        }
    fire:
        if (TankBossTurretFindTarget(eng) >= 0) {
            nas->UserInfo.user1++;
            TankBossCortexTurretFire(eng);
            nas->UserInfo.user2 = 0;
            nas->UserInfo.status = 13;
        }
        break;
    }
}

void TankBossRocketTurretFire(NAS *nas, EXTengine *eng) {
    s32 i;
    s32 dir;
    s32 a;
    NAS *n;
    NAS *puff;
    EXTobject_TA *fux;
    EXTengine *e;

    dir = eng->unk1E;
    for (i = 0; ; i++) {
        {
            s32 k;
            if (i >= 2) {
                break;
            }
        }
        n = CreateNAS(eng->unk10.x, eng->unk10.y - 0xFA, eng->unk10.z, 0x1302, 0x11);
        if (n != NULL) {
            PlaySfx(0x522, &n->Engine->sfx, (fux = (EXTobject_TA *)n->FUX, (SLONG_XYZ *)NULL), 0x1000, 0x1E00, 0);
            e = n->Engine;
            a = dir - TankShellAngleOffsets()[i];
            e->unk10.x -= (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 245) >> 11;
            e->unk10.z -= (rcossin_tbl[a & 0xFFF].unk2 * 245) >> 11;
            e->unk1E = eng->unk1E;
            fux->SpeedX = -(rcossin_tbl[(dir - 0x400) & 0xFFF].unk2 * 200) >> 12;
            fux->SpeedZ = -(rcossin_tbl[dir & 0xFFF].unk2 * 200) >> 12;
            fux->Status = 1;
            fux->Owner = -1;
        }
        puff = TankSpawnBit1605Nas(&n->Engine->unk10, 7);
        if (puff != NULL) {
            TankPuffSetAngle(puff, eng->unk1E);
        }
    }
    nas->UserInfo.user2 = 0x78;
    nas->UserInfo.status = 0xD;
    if (nas->UserInfo.flags != 0) {
        eng->sub->entity = 0x2025;
    } else {
        eng->sub->entity = 0x2024;
    }
}

void PlotTankBossRocketTurret(NAS *nas) {
    EXTengine *eng;
    s32 t;

    eng = nas->Engine;
    eng->unk10.y = TankBossEng->unk10.y - 0x4BF;
    if (TankBossStatus < 0x36) {
        return;
    }
    if (TankBossStatus >= 0x38) {
        return;
    }
    ((void (*)(char *, s32))DebugNasStatus)("RocketTurret", nas->UserInfo.status);
    if (nas->UserInfo.flags == 2) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 17:
        if (TankBossStatus == 0x36) {
            eng->flags = 0x8000;
            eng->unk5E = 1;
            eng->unk10.z -= 10;
            eng->unk10.y -= 3;
            nas->UserInfo.user2 = 30;
        } else if (TankBossStatus == 0x37) {
            if (--nas->UserInfo.user2 <= 0) {
                nas->UserInfo.flags = 0;
                nas->UserInfo.status = 19;
                nas->UserInfo.user2 = 0;
            }
        }
        break;
    case 19:
        t = ++nas->UserInfo.user2 & 0x3F;
        if (eng->unk10.x > 0x900) {
            if (t < 0x20) {
                eng->unk1E += 30;
            } else {
                eng->unk1E -= 30;
            }
        } else if (t >= 0x20) {
            eng->unk1E += 30;
        } else {
            eng->unk1E -= 30;
        }
        eng->unk5E = 1;
        if (nas->UserInfo.user2 > 30 && TankBossTurretFindTarget(eng) >= 0) {
            nas->UserInfo.status = 10;
            nas->UserInfo.user2 = TankBossRocketDelay;
        }
        break;
    case 10:
        if (--nas->UserInfo.user2 == TankBossRocketDelay / 2) {
            if (nas->UserInfo.flags != 0) {
                eng->sub->entity = 0x2022;
            } else {
                eng->sub->entity = 0x2021;
            }
        }
        if (nas->UserInfo.user2 <= 0) {
            TankBossRocketTurretFire(nas, eng);
            nas->UserInfo.status = 13;
            nas->UserInfo.user2 = 30;
        }
        break;
    case 13:
        if (--nas->UserInfo.user2 <= 0) {
            nas->UserInfo.status = 18;
        }
        break;
    case 18:
        eng->unk5E = 1;
        if (eng->unk10.x > 0x900) {
            if (eng->unk1E < 0xFCF) {
                eng->unk1E = (eng->unk1E - 20) & 0xFFF;
                break;
            }
        } else if (eng->unk1E < 0) {
            eng->unk1E = (eng->unk1E + 20) & 0xFFF;
            break;
        }
        eng->unk1E = 0;
        nas->UserInfo.status = 19;
        nas->UserInfo.user2 = 0;
        break;
    case 0:
        nas->UserInfo.status = 18;
        nas->UserInfo.user2--;
        break;
    }
}

void InitMoeJoe(NAS *nas, s32 id) {
    EXTengine *eng;
    subengine *sub;
    subengine *s;
    subengine *s2;
    EXTobject_TA *ext;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    nas->UserInfo.flags = 0;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoKomodo];
    if (id == 0x131F) {
        sub->entity = 0x4180;
    } else {
        sub->entity = 0x4080;
    }
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    ext = AddExt(nas, 0x38, 0x10000000);
    nas->FUX = (EXT *)ext;
    ext->Counter = 3;
    eng_add_subs(eng, 2);
    sub = eng->sub;
    s = sub->next;
    sub->unk51 = 1;
    s2 = s->next;
    s->unk51 = 1;
    s->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    s->entity = 0x2011;
    s->unk68 = 0x100;
    s->flags = (s->flags & 0xFFFF7FFF) | 0x02000000;
    s2->unk51 = 1;
    s2->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    s2->entity = 0x4300;
    s2->unk68 = 0x100;
    s2->flags = (s2->flags & 0xFFFF7FFF) | 0x02000000;
    switch (TankBossStartStatus) {
    case 1:
        ext->Status = TankBossStartStatus;
        break;
    case 0x2C:
        eng->unk10.y = 0x1001;
        eng->unk10.z = 0x1001;
        sub->unk68 = 0x100;
        sub->flags |= 0x02000000;
        ext->Status = 5;
        break;
    case 0x3A:
        ext->Status = TankBossStartStatus;
        eng->unk10.y = 0x480;
        eng->unk10.z = 0x1001;
        sub->unk68 = 0x100;
        sub->flags |= 0x02000000;
        break;
    }
}

void LoopMoeJoeMoveAnim(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        MoeAnimPtr = MoeMoveAnim;
        val = *MoeAnimPtr++;
        TankMoeSub->entity = val;
    } else {
        s32 val;
        JoeAnimPtr = JoeMoveAnim;
        val = *JoeAnimPtr++;
        TankJoeSub->entity = val;
    }
}

void AnimateMoeJoeTurret(s32 player) {
    TA_player *ta;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *se;
    subengine *sub;
    subengine *sub2;
    NAS *n;
    NAS *puff;
    EXTobject_TA *fux;
    s32 a;
    s32 dx;
    s32 dz;
    u16 anim;

    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    sub = Heroes[player].hero_sub->next;
    sub2 = sub->next;
    if (ta->TurretFiring >= 0) {
        a = *ta->TurretAnimPtr++;
        if (a != 0) {
            if (GameMode == 5) {
                sub2->entity = a;
            } else {
                sub->entity = a;
            }
        } else {
            switch (ta->TurretFiring) {
            default:
                anim = 0x2010;
                break;
            case 11:
                anim = 0x2011;
                break;
            case 10:
                anim = 0x2012;
                break;
            case 8:
                anim = 0x2009;
                break;
            case 0:
            case 3:
                anim = 0x2008;
                break;
            case 2:
                anim = 0x200B;
                break;
            }
            sub->entity = anim;
            ta->TurretFiring = -1;
            if (ta->MainWeaponRounds <= 0) {
                ResetToBasicWeapon(player);
            }
        }
        if (sub->entity == 0x4004) {
            e = Heroes[player].hero_eng;
            puff = TankSpawnBit1605Nas(&e->unk10, 0);
            if (puff != NULL) {
                TankPuffSetAngle(puff, eng->unk1E);
            }
            if (TA_players[player].MainWeaponType == 3) {
                n = CreateNAS(e->unk10.x, e->unk10.y - 0x12C, e->unk10.z, 0x130B, 0x11);
                if (n != NULL) {
                    PlaySfx(0x4F3, &n->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                    fux = AddExt(n, 0x38, 0x10000000);
                    n->FUX = fux;
                    se = n->Engine;
                    dx = -(rcossin_tbl[(e->unk1E - 0x400) & 0xFFF].unk2 * 250);
                    se->unk10.x += dx >> 12;
                    dz = -(rcossin_tbl[(u16)e->unk1E & 0xFFF].unk2 * 250);
                    se->unk10.z += dz >> 12;
                    fux->SpeedX = dx >> 13;
                    fux->SpeedY = -0x5B;
                    fux->SpeedZ = dz >> 13;
                    fux->Owner = player;
                    fux->Status = 1;
                    TA_players[player].MainWeaponRounds--;
                }
            }
        }
    } else if (GameMode == 5) {
    loop:
        a = *ta->TurretAnimPtr++;
        if (a == 0) {
            switch (ta->MainWeaponType) {
            case 9:
            default:
                ta->TurretAnimPtr = TankSwampSwirlyTurret;
                break;
            case 11:
                ta->TurretAnimPtr = TankSwampLaserTurret;
                break;
            case 10:
                ta->TurretAnimPtr = TankSwampFlubberTurret;
                break;
            case 8:
                ta->TurretAnimPtr = TankSwampFlamerTurret;
                break;
            }
            goto loop;
        }
        sub2->entity = a;
    }
    if (GameMode != 5) {
        if (++sub2->entity == 0x4306) {
            sub2->entity = 0x4300;
        }
    }
}

void SetMoeJoeIdle(s32 arg0) {
    EXTengine *eng;
    s32 pad[2];

    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
    eng = Heroes[arg0].hero_eng;
    if (TA_players[arg0].HullSFX != 0) {
        TA_players[arg0].HullSFX = 0;
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
    }
    if (TA_players[arg0].TurretSFX != 0) {
        TA_players[arg0].TurretSFX = 0;
        StopSfx(TA_players[arg0].TurretSFXHandler);
    }
    if (TA_players[arg0].FireBreatherSFX != 0) {
        TA_players[arg0].FireBreatherSFX = 0;
        StopSfx(TA_players[arg0].FireBreatherSFXHandler);
    }
    if (arg0 == 3) {
        s32 v;
        MoeAnimPtr = MoeMoveAnim;
        v = *MoeAnimPtr++;
        TankMoeSub->entity = v;
    } else {
        s32 v;
        JoeAnimPtr = JoeMoveAnim;
        v = *JoeAnimPtr++;
        TankJoeSub->entity = v;
    }
}

void SetMoeJoeMove(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        MoeAnimPtr = MoeMoveAnim;
        val = *MoeAnimPtr++;
        TankMoeSub->entity = val;
    } else {
        s32 val;
        JoeAnimPtr = JoeMoveAnim;
        val = *JoeAnimPtr++;
        TankJoeSub->entity = val;
    }
}

void SetMoeJoeRecoil(s32 arg0) {
    s32 val;
    s32 val2;
    s32 *p;
    s32 *p2;
    s32 z;

    if (TankLevelOver == 0) {
        if (arg0 == 3) {
        again:
            p = MoeRecoilAnim;
            MoeAnimPtr = p;
            val = *MoeAnimPtr++;
            TankMoeSub->entity = val;
            PlaySfx(0x261, 0, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
        } else {
            p2 = JoeRecoilAnim;
            JoeAnimPtr = p2;
            val2 = *JoeAnimPtr++;
            Heroes[2].hero_sub->entity = val2;
            PlaySfx(0x260, 0, (SLONG_XYZ *)((u8 *)Heroes[arg0].hero_eng + 0x10), 0x1000, 0x1E00, 0);
        }
        TA_players[arg0].SpeedY = -0x64;
        Heroes[arg0].hero_immunity = 0x3C;
        do {
            z = 0;
        } while (0);
        if (z) {
            goto again;
        }
    }
}

void SetMoeJoeDead(s32 hero, s32 last_mode) {
    subengine *s;

    Heroes[hero].hero_hit = 0;
    Heroes[hero].hero_immunity = 0;
    Heroes[hero].hero_health = 0;
    Heroes[hero].hero_status |= 0x6000;
    HeroBecomeAngel(hero);
    s = Heroes[hero].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    if (hero == 3) {
        MoeState = 0x26;
    } else {
        JoeState = 0x26;
    }
}

void MoeJoeMove(s32 player) {
    EXTengine *eng;
    EXTengine *e;
    subengine *sub;
    HeroInfo *h;
    TA_Bot *ai;
    subengine *u;
    TA_player *ta;
    s32 ex;
    s32 ez;
    s32 v;
    s32 pad;
    s32 speed;
    s32 dir;
    s32 angle;
    s32 a;
    s32 sn;
    s32 cs;
    s32 x;
    s32 z;
    s32 fx;
    s32 fz;
    s32 lim;
    s32 n;
    s32 d;
    s32 t;
    s32 df;
    s32 saved;
    u16 name;
    s32 hit;
    s32 i;

    UpdateTankHeroWeapons(player);
    if (player == 3) {
        v = *MoeAnimPtr++;
        TankMoeSub->entity = v;
    } else {
        v = *JoeAnimPtr++;
        TankJoeSub->entity = v;
    }
    if (v == 0) {
        LoopMoeJoeMoveAnim(player);
    }
    if (TankLevelOver != 0) {
        return;
    }
    ta = &TA_players[player];
    eng = Heroes[player].hero_eng;
    speed = ta->Speed;
    sub = eng->sub->next->next;
    saved = ((TankGameView *)GameInfo)->w6C;
    ((TankGameView *)GameInfo)->w6C = 1;
    ai = &TankBotAI[player];
    TA_HeroBot(player);
    ((TankGameView *)GameInfo)->w6C = saved;
    pad = ai->AIpad;
    dir = pad & 0xF0;
    if (dir == 0) {
        return;
    }
    if ((pad & 0x20) && dir != 0x20) {
        ex = eng->unk10.x;
        if (TankArenaMaxX - speed < ex ||
            TankMapCell(ex + speed + 0x100, eng->unk10.z) != 0) {
            dir = pad & 0xD0;
        }
    }
    if ((dir & 0x80) && dir != 0x80) {
        ex = eng->unk10.x;
        if (ex < speed + 0x100 ||
            TankMapCell(ex - speed - 0x100, eng->unk10.z) != 0) {
            dir &= ~0x80;
        }
    }
    if ((dir & 0x10) && dir != 0x10) {
        ez = eng->unk10.z;
        if (TankArenaMaxZ - speed < ez ||
            TankMapCell(eng->unk10.x, ez + speed + 0x100) != 0) {
            dir &= ~0x10;
        }
    }
    if ((dir & 0x40) && dir != 0x40) {
        ez = eng->unk10.z;
        if (ez < speed + 0x100 ||
            TankMapCell(eng->unk10.x, ez - speed - 0x100) != 0) {
            dir &= ~0x40;
        }
    }
    switch (dir) {
    case 0x40:
        angle = 0x800;
        break;
    case 0x80:
        angle = 0xC00;
        break;
    case 0x20:
        angle = 0x400;
        break;
    case 0x90:
        angle = 0xE00;
        break;
    case 0x30:
        angle = 0x200;
        break;
    case 0xC0:
        angle = 0xA00;
        break;
    case 0x60:
        angle = 0x600;
        break;
    case 0x10:
    default:
        angle = 0;
        break;
    }
loop:
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    df = ((angle - a) << 20) >> 20;
    a = (u16)sub->unk12;
    if ((u32)(df + 0x400) > 0x800) {
        sub->unk12 = (a + 0x800) & 0xFFF;
        goto loop;
    }
    df = df / 4;
    sub->unk12 = (a + df) & 0xFFF;
    sub->unk52 = 1;
    a = (eng->unk1E + sub->unk12) & 0xFFF;
    sn = rcossin_tbl[(a - 0x400) & 0xFFF].unk2;
    cs = rcossin_tbl[a].unk2;
    x = eng->unk10.x + ((speed * sn) >> 12);
    fx = x + ((sn * 256) >> 12);
    z = eng->unk10.z + ((speed * cs) >> 12);
    fz = z + ((cs * 256) >> 12);
    lim = 0x1C0;
    for (i = 0; i < 4; i++) {
        if (i == player) {
            continue;
        }
        h = &Heroes[i];
        name = h->hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        e = h->hero_eng;
        d = x - e->unk10.x;
        if (d < -0x1C0) {
            continue;
        }
        if (lim < d) {
            continue;
        }
        d = z - e->unk10.z;
        n = -0x1C0;
        if (n < d && d < 0x1C0) {
            hit = i;
            goto found;
        }
    }
    hit = -1;
found:
    if (hit < 0) {
        t = ((fz >> 9) * TankMapWidth) + (fx >> 9);
        if (TankLevelsGridMapMiss[t] == 0) {
            eng->unk10.x = x;
            eng->unk10.z = z;
        }
    } else if (TankCountdown == 0) {
        TankCountdownEng = eng;
        TankCountdown = -1;
    }
}

void MoeJoeRecoil(s32 arg0) {
    EXTengine *eng;
    TA_player *p;
    HeroInfo *h;
    HeroInfo *base;
    s32 val;
    s32 mode;
    s32 last;

    eng = Heroes[arg0].hero_eng;
    TA_players[arg0].SpeedY += 0xC;
    p = &TA_players[arg0];
    eng->unk10.y += p->SpeedY;
    if (eng->unk10.y > 0) {
        p->SpeedY = -p->SpeedY / 2;
        eng->unk10.y = 0;
    }
    eng->sub->next->next->unk08 = -eng->unk10.y / 2;
    if (arg0 == 3) {
        val = *MoeAnimPtr++;
        Heroes[3].hero_sub->entity = val;
    } else {
        val = *JoeAnimPtr++;
        Heroes[2].hero_sub->entity = val;
    }
    if (val == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_status & 0x2000) {
            u16 old = h->hero_mode;

            h->hero_mode = 0x6A;
            h->hero_last_mode = old;
            if (arg0 == 3) {
                subengine *s;
                s32 v;
                v = MoeDieAnim[0];
                MoeAnimPtr = MoeDieAnim;
                s = Heroes[3].hero_sub;
                MoeAnimPtr++;
                s->entity = v;
            } else {
                subengine *s;
                s32 v;
                v = JoeDieAnim[0];
                JoeAnimPtr = JoeDieAnim;
                s = Heroes[2].hero_sub;
                JoeAnimPtr++;
                s->entity = v;
            }
            return;
        }
        mode = h->hero_last_mode;
        last = h->hero_mode;
        h->hero_mode = mode;
        h->hero_last_mode = last;
        switch (mode) {
        case 0x6C:
            SetMoeJoeIdle(arg0);
            break;
        case 0x6E:
            SetMoeJoeMove(arg0);
            break;
        case 0x69:
            SetMoeJoeRecoil(arg0);
            break;
        case 0x6B:
            ((void (*)(s32))SetMoeJoeDead)(arg0);
            break;
        case 0x6A:
        case 0x6D:
            break;
        }
        if (arg0 == 3) {
            s32 v;
            MoeAnimPtr = MoeMoveAnim;
            v = *MoeAnimPtr++;
            TankMoeSub->entity = v;
        } else {
            s32 v;
            JoeAnimPtr = JoeMoveAnim;
            v = *JoeAnimPtr++;
            TankJoeSub->entity = v;
        }
    }
}

void MoeJoeDie(s32 arg0) {
    s32 val;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    EXTscriptdata *ext;
    s32 mode;

    if (arg0 == 3) {
        TankMoeSub->entity = val = *MoeAnimPtr++;
    } else {
        TankJoeSub->entity = val = *JoeAnimPtr++;
    }
    if (val == 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = &HeroModelBank[DB_TankObjects];
        nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 0xE;
                ext->speed = 0x100;
                if (p != NULL) {
                    ext->flags = 5;
                    nas->Engine->sub->u6C.dbase = p;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        mode = Heroes[arg0].hero_mode;
        Heroes[arg0].hero_mode = 0x6B;
        Heroes[arg0].hero_last_mode = mode;
        SetMoeJoeDead(arg0, mode);
    }
}

void MoeJoeBecomeHero(NAS *nas, s32 id) {
    EXTengine *eng;
    subengine *sub;
    HeroInfo *h;
    HeroInfo *hb;
    EXTbotai *ai;
    TA_player *ta;
    TA_player *tb;
    TA_Bot *t;
    s32 p;
    u16 mode;
    u8 *g;

    eng = nas->Engine;
    sub = eng->sub;
    if (id == 0x131E) {
        p = 3;
        TankMoeTeammate = 2;
        MoeState = 0x3B;
    } else {
        p = 2;
        TankJoeTeammate = 3;
        JoeState = 0x3B;
    }
    g = GameInfo + p;
    g[0x5C] = 1;
    hb = Heroes;
    h = &hb[p];
    h->hero_status = 0x80;
    h->hero_strength = 0x14;
    h->hero_idle_count = 0x258;
    h->hero_harm_player = -1;
    h->hero = nas;
    h->hero_eng = eng;
    h->hero_sub = sub;
    h->hero_immunity = 0;
    h->hero_immunity_level = 0;
    h->hero_hit = 0;
    h->hero_last_mode = 0;
    h->hero_mode = 0;
    h->hero_pick_delay = 0;
    h->hero_aseq = 0;
    h->hero_harm_Nas = NULL;
    h->hero_harm_value = 0;
    h->hero_harm_fraction = 0;
    h->hero_harm_flags = 0;
    h->hero_harm_sfx = 0;
    h->hero_ctrl = 0;
    h->hero_score = 0;
    h->hero_score_displayed = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    h->hero_pu_lock = 0;
    h->hero_attack_timer = 0;
    h->hero_attack_delay = 0;
    h->hero_pu_action = 0;
    h->hero_pu_action_timer = 0;
    h->hero_weapon_count = 0;
    h->hero_weapon_timer = 0;
    h->hero_attack_counter = 0;
    h->hero_name = 9;
    g[0x50] = 9;
    AiPlayerSkill[p].level = 0x10;
    AiPlayerSkill[p].unk2 = 0x10;
    AiPlayerSkill[p].quality = 0x10;
    if (h->hero_ai == NULL) {
        ai = AddExt(h->hero, 0x44, 0x400000F4);
        if (ai != NULL) {
            h->hero_ai = ai;
            ai->AImode = 0;
            ai->AIaborted_mode = 0;
            ai->AItimer1 = 0;
            ai->AItimer2 = 0;
            ai->AIang = 0;
            ai->AIpad = 0;
            ai->pTargetNas = NULL;
            ai->TargetPlayer = -1;
            ai->TargetXYZ.z = 0;
            ai->TargetXYZ.y = 0;
            ai->TargetXYZ.x = 0;
            ai->AIinitial_delay = Random(0x3C) + 0x1E;
            ai->AIsleep = 8;
        }
    }
    ((PadButtons *)Pads)[p].unk50 = 0;
    ((PadButtons *)Pads)[p].unk54 = 0;
    eng = Heroes[p].hero_eng;
    eng->unk10.z = 0xF00;
    eng->unk10.x = (eng->unk10.x + 0x80) & ~0xFF;
    mode = Heroes[p].hero_mode;
    Heroes[p].hero_mode = 0x6E;
    Heroes[p].hero_last_mode = mode;
    SetMoeJoeMove(p);
    sub = eng->sub;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    sub = sub->next;
    sub->unk51 = 1;
    sub->unk68 = 0;
    SetMoeJoeIdle(p);
    tb = TA_players;
    ta = &tb[p];
    ta->Speed = 0x1A;
    ta->MoveDir = 0;
    ta->FaceDir = 0;
    ta->RunnerMoving = 0;
    ta->RunnerFacing = 0;
    ta->SpeedX = 0;
    ta->SpeedZ = 0;
    ta->Speed = ta->Speed * (AiPlayerSkill[p].level + 0x54) / 100;
    ta->Energy = 0x63;
    Heroes[p].hero_health = ta->Energy;
    ta->PosX = eng->unk10.x;
    ta->PosZ = eng->unk10.z;
    ta->SubWeaponFireDelay = 0;
    ta->SubWeaponRounds = 0;
    ta->FireBreatherSFXDuration = 0;
    ta->TurretSFXDuration = 0;
    ta->TurretReqdDir = -1;
    ta->HangaboutTime = -0x5A;
    ta->IdleAnimCt = Random(0x1E);
    ta->TurretFiring = -1;
    ta->TauntDelay = 0;
    ResetToBasicWeapon(p);
    ta->MainWeaponFireDelay = 0x1E;
    ta->MainWeaponNas[0] = (NAS *)0xFFEEEEFF;
    ta->MainWeaponNas[2] = (NAS *)0xFFEEEEFF;
    t = &TankBotAI[p];
    t->Doing = 0;
    t->DoingTimer = 0;
    t->Victim = 0;
    t->pMine = NULL;
    t->pMissile = 0;
    t->AIpad = 0;
    t->FirePercentChance = 0;
    if (MoeState == 0x3B && JoeState == MoeState) {
        SetPanelHeroIconA(0x3B, 0x300E, 0x300F);
        InitPowerupIcons((s32)&Heroes[2].hero_health, (s32)&Heroes[3].hero_health);
    }
}

void PlotMoeJoeHero(NAS *nas, s32 id) {
    EXTengine *eng;
    TA_player *ta;
    s32 p;
    s32 x;
    s32 dz;
    u16 mode;

    p = 2;
    if (id == 0x131E) {
        p = 3;
    }
    eng = nas->Engine;
    eng->flags = 0x8000;
    ta = &TA_players[p];
    switch (Heroes[p].hero_mode) {
    case 0x6C:
        mode = Heroes[p].hero_mode;
        Heroes[p].hero_mode = 0x6E;
        Heroes[p].hero_last_mode = mode;
        SetMoeJoeMove(p);
        break;
    case 0x6E:
        MoeJoeMove(p);
        break;
    case 0x69:
        MoeJoeRecoil(p);
        break;
    case 0x6A:
        MoeJoeDie(p);
        break;
    case 0x6B:
        HeroGhost(p);
        break;
    }
    if (eng->unk10.x > TankArenaMaxX) {
        eng->unk10.x = TankArenaMaxX;
    }
    if (eng->unk10.x < 0x100) {
        eng->unk10.x = 0x100;
    }
    if (eng->unk10.z > TankArenaMaxZ) {
        eng->unk10.z = TankArenaMaxZ;
    }
    if (eng->unk10.z < 0x100) {
        eng->unk10.z = 0x100;
    }
    AnimateMoeJoeTurret(p);
    if (GameMode != 5) {
        x = eng->unk10.x;
        dz = eng->unk10.z - eng->OldXYZ.z;
        if (x != eng->OldXYZ.x || dz != 0) {
            if (ta->HangaboutTime != 0) {
                ta->HangaboutTime--;
            }
            TANK_SET_HULL_SFX(p, eng, TankHullSfx[p]);
        } else {
            if (!(Heroes[p].hero_status & 0xFA00)) {
                ta->HangaboutTime++;
            }
            TANK_SET_HULL_SFX(p, eng, 0);
        }
        if (ta->TurretSFXDuration != 0) {
            if (--ta->TurretSFXDuration == 0) {
                TANK_SET_TURRET_SFX(p, 0);
            } else {
                TANK_SET_TURRET_SFX(p, TankTurretSfx[p]);
            }
        }
    }
    if (((PadButtons *)Pads)[p].unk50 & 0x4800) {
        TANK_SET_HULL_SFX(p, eng, TankHoverSfx[p]);
    } else {
        TANK_SET_HULL_SFX(p, eng, 0);
    }
    if (Heroes[p].hero_immunity != 0) {
        Heroes[p].hero_immunity--;
    }
}

void PlotMoeJoe(NAS *nas, s32 id) {
    EXTobject_TA *fux;
    EXTengine *eng;
    subengine *sub;
    subengine *s;
    s32 *p;
    ModelRec *m;
    s32 anim;
    s32 next;
    s32 z;

    fux = (EXTobject_TA *)nas->FUX;
    eng = nas->Engine;
    sub = eng->sub;
    if (fux->Status == 0x3B) {
        PlotMoeJoeHero(nas, id);
        return;
    }
    if (TankLevelOver != 0) {
        return;
    }
    if (TankBossStatus == 0) {
        sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoKomodotank];
        eng->unk2C.x = 0x1266;
        eng->unk2C.y = 0x1266;
        eng->unk2C.z = 0x1266;
        sub->entity++;
        if (id == 0x131E) {
            if ((u16)(sub->entity - 0x4180) >= 20) {
                sub->entity = 0x4180;
            }
        } else if ((u16)(sub->entity - 0x4000) >= 20) {
            sub->entity = 0x4000;
        }
        s = sub->next;
        s->flags |= 0x8000;
        s->next->flags |= 0x8000;
        fux = (EXTobject_TA *)nas->FUX;
        switch (fux->Status) {
        case 4:
            eng->unk10.y -= 0x14;
            if (eng->unk10.y < 0) {
                fux->Status = 0x29;
                fux->Counter = 1;
            }
            break;
        case 0x29:
            eng->unk10.z -= 0x3C;
            if ((u32)(Heroes[0].hero_eng->unk10.x - eng->unk10.x + 0x1FF) < 0x3FF) {
                z = eng->unk10.z - 0x200;
                if (z < Heroes[0].hero_eng->unk10.z) {
                    Heroes[0].hero_eng->unk10.z = z;
                }
            }
            if (GameTeams[0] != 0 &&
                (u32)(Heroes[1].hero_eng->unk10.x - eng->unk10.x + 0x1FF) < 0x3FF) {
                z = eng->unk10.z - 0x200;
                if (z < Heroes[1].hero_eng->unk10.z) {
                    Heroes[1].hero_eng->unk10.z = z;
                }
            }
            if (eng->unk10.z <= 0xF00) {
                MoeJoeBecomeHero(nas, id);
                fux->Status = 0x3B;
            }
            break;
        }
        return;
    }
    if (TankBossStatus == 0x3A) {
        fux->Status = 4;
        sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoKomodotank];
        eng->unk10.y = 0x480;
        if (id == 0x131E) {
            eng->unk10.x = 0xD00;
            sub->entity = 0x4180;
            TankBossCrashTurrets[0] = nas;
        } else {
            eng->unk10.x = 0x400;
            sub->entity = 0x400A;
            TankBossCrashTurrets[1] = nas;
        }
        return;
    }
    anim = sub->entity + 1;
    next = 0;
    switch (anim) {
    case 0x419E:
        anim = 0x4180;
        next = anim;
        break;
    case 0x411E:
        anim = 0x4100;
        next = anim;
        break;
    case 0x4014:
        anim = 0x4000;
        next = anim;
        break;
    case 0x4214:
        anim = 0x4200;
        next = anim;
        break;
    case 0x40A3:
        anim = 0x4080;
        next = anim;
        break;
    }
    switch (fux->Status) {
    case 1:
        if (next != 0) {
            anim = 0x4180;
            if (id != 0x131F) {
                anim = 0x4080;
                if (fux->Counter & 1) {
                    anim = 0x4100;
                }
            }
            if (--fux->Counter <= 0) {
                fux->Status = 0x24;
                fux->Counter = 0xD;
                TankPendingSfx = 0x25E;
                anim = 0x4000;
                if (id == 0x131F) {
                    anim = 0x4200;
                }
            }
        }
        break;
    case 0x24:
        if (id == 0x131E) {
            eng->unk1E += 0x96;
        } else {
            eng->unk1E -= 0x96;
        }
        eng->unk5E = 1;
        if (--fux->Counter <= 0) {
            fux->Status = 9;
            if (id == 0x131F) {
                sub->entity = 0x4200;
            } else {
                sub->entity = 0x4000;
            }
        }
        break;
    case 9:
        eng->unk10.z += 0x46;
        if (id == 0x131E) {
            eng->unk10.x -= 0xD;
        } else {
            eng->unk10.x += 0xD;
        }
        if (eng->unk10.z > 0x1100) {
            fux->Status = 5;
            fux->SpeedY = -0x46;
            sub->unk68 = 0x100;
            sub->flags |= 0x02000000;
        }
        break;
    case 5:
        fux->SpeedY += 0xC;
        eng->unk10.y += fux->SpeedY;
        if (eng->unk10.y > 0x1000) {
            eng->unk1E = 0;
            eng->unk5E = 1;
            eng->unk10.z = TankBossEng->unk10.z + 0x100;
            fux->Status = 0;
            fux->Counter = Random(0x5A) + 0x1E;
            eng->unk2C.x = 0x14CC;
            eng->unk2C.y = 0x14CC;
            eng->unk2C.z = 0x14CC;
            eng->unk5C = 1;
            if (id == 0x131E) {
                anim = 0x2028;
                fux->pAnimList = MoeScript0;
                eng->unk10.x = 0xA00;
            } else {
                anim = 0x2027;
                fux->pAnimList = JoeScript0;
                eng->unk10.x = 0x810;
            }
            m = (ModelRec *)&HeroModelBank[DB_ArenaBossKomodoArenaobjects];
            sub->u6C.dbase = m;
            sub->entity = anim;
            if (m[1].on == 0) {
                return;
            }
            dbase_release((u8 *)&m[1]);
        }
        break;
    case 0:
        eng->unk10.y = TankBossEng->unk10.y - 0x528;
        if (--fux->Counter <= 0) {
            fux->Status = 0x27;
            switch (Random(8)) {
            default:
            case 0:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript0;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript0;
                    anim = 0x2027;
                }
                break;
            case 1:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript1;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript1;
                    anim = 0x2027;
                }
                break;
            case 2:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript2;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript2;
                    anim = 0x2027;
                }
                break;
            case 3:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript3;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript3;
                    anim = 0x2027;
                }
                break;
            case 4:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript4;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript4;
                    anim = 0x2027;
                }
                break;
            case 5:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript5;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript5;
                    anim = 0x2027;
                }
                break;
            case 6:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript6;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript6;
                    anim = 0x2027;
                }
                break;
            case 7:
                if (id == 0x131E) {
                    fux->pAnimList = MoeScript7;
                    anim = 0x2028;
                } else {
                    fux->pAnimList = JoeScript7;
                    anim = 0x2027;
                }
                break;
            }
        } else {
            anim = sub->entity;
        }
        break;
    case 0x27:
        eng->unk10.y = TankBossEng->unk10.y - 0x528;
        p = fux->pAnimList;
        anim = *p;
        if (GameFrameCount & 1) {
            fux->pAnimList = p + 1;
        }
        if (anim == 0) {
            anim = 0x2027;
            if (id == 0x131E) {
                anim = 0x2028;
            }
            fux->Status = 0;
            fux->Counter = Random(0x1E) + 0xF;
        }
        break;
    case 0x3A:
        break;
    }
    sub->entity = anim;
}

void PlotMoeJoe(NAS *nas, s32 id);

void PlotJoe(NAS *nas) {
    PlotMoeJoe(nas, 0x131F);
}

void InitMoeJoe(NAS *nas, s32 id);

void InitJoe(NAS *nas) {
    InitMoeJoe(nas, 0x131F);
}

void PlotMoe(NAS *nas) {
    PlotMoeJoe(nas, 0x131E);
}

void InitMoe(NAS *nas) {
    InitMoeJoe(nas, 0x131E);
}

void MoeJoeDead(void) {
    HeroGhost();
}

void MoeJoeIdle(s32 arg0) {
    u16 prev = Heroes[arg0].hero_mode;
    Heroes[arg0].hero_mode = 0x6E;
    Heroes[arg0].hero_last_mode = prev;
    SetMoeJoeMove(arg0);
}

void SetMoeJoeMode(s32 hero, s32 mode) {
    u16 last = Heroes[hero].hero_mode;

    Heroes[hero].hero_mode = mode;
    Heroes[hero].hero_last_mode = last;
    switch (mode) {
    case 0x6C:
        ((void (*)())SetMoeJoeIdle)();
        break;
    case 0x6E:
        ((void (*)())SetMoeJoeMove)();
        break;
    case 0x69:
        ((void (*)())SetMoeJoeRecoil)();
        break;
    case 0x6B:
        SetMoeJoeDead(hero, mode);
        break;
    case 0x6A:
    case 0x6D:
        break;
    }
}

/* Moe and Joe's minelay updater (hero mode 0x6D), empty like TankHeroMinelay. */
void MoeJoeMinelay(void) {
}

void StartMoeJoeAnim(s32 arg0, s32 *arg1) {
    s32 val;

    if (arg0 == 3) {
        MoeAnimPtr = arg1;
    } else {
        JoeAnimPtr = arg1;
    }
    if (arg0 == 3) {
        val = *MoeAnimPtr++;
        TankMoeSub->entity = val;
    } else {
        val = *JoeAnimPtr++;
        TankJoeSub->entity = val;
    }
}

s32 StepMoeJoeAnim(s32 arg0) {
    s32 val;

    if (arg0 == 3) {
        TankMoeSub->entity = val = *MoeAnimPtr++;
    } else {
        TankJoeSub->entity = val = *JoeAnimPtr++;
    }
    return val;
}

void InitTankBossRocketTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk10.z += 0x258;
    eng->unk10.y += 0xC8;
    sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoArenaobjects];
    if (nas->UserInfo.flags == 0) {
        sub->entity = 0x2024;
    } else {
        sub->entity = 0x2025;
    }
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
    eng->flags = 0;
}

void InitTankBossCortexTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoArenaobjects];
    sub->entity = 0x2004;
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
}

void InitTankBossCrashTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 v;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk10.z += 0x100;
    v = 0x100;
    nas->UserInfo.flags = v;
    nas->UserInfo.user2 = 0x168;
    nas->UserInfo.status = 4;
    sub->u6C.dbase = &HeroModelBank[DB_ArenaBossKomodoArenaobjects];
    sub->entity = 0x2003;
    sub->unk68 = v;
    sub->unk51 = 0;
    sub->flags |= 0x02000000;
}

s32 TankBossTurretCheckSight(s32 id, s32 x, s32 z, s32 angle) {
    s32 d;

    if (TankLevelOver == 0) {
        if (TankBossTargets[id].z >= 0) {
            d = (angle - get_angle(x - TankBossTargets[id].x, z - TankBossTargets[id].z)) & 0xFFF;
            if (d < 0x15 || d > 0xFEB) {
                TankBossSightedHero = id;
                return 1;
            }
        }
    }
    return 0;
}

/* The boss level's human target: the surviving one of heroes 0 and 1, else either at random;
 * hero 0 in a one-player game. */
s32 TankBossPickHero(void) {
    if (GameTeams[0] != 0) {
        if (Heroes[0].hero_health == 0) {
            return 1;
        }
        if (Heroes[1].hero_health == 0) {
            return 0;
        }
        return Random(2);
    }
    return 0;
}

s32 TankWithinBox(EXTengine *eng, s32 x, s32 y, s32 z) {
    s32 d;

    d = eng->unk10.x - x;
    if (d > 0x250) {
        return 0;
    }
    if (d < -0x250) {
        return 0;
    }
    d = eng->unk10.y - y;
    if (d > 0x200) {
        return 0;
    }
    if (d < -0x200) {
        return 0;
    }
    d = eng->unk10.z - z;
    if (d > 0x500) {
        return 0;
    }
    return d >= -0x500;
}

void PlotTotemBot(NAS *nas) {
    PlotTotemBit(nas, 0x131C);
}

void InitTotemBot(NAS *nas) {
    InitTotemBit(nas, 0x131C);
}

void PlotTotemTop(NAS *nas) {
    PlotTotemBit(nas, 0x131B);
}

void InitTotemTop(NAS *nas) {
    InitTotemBit(nas, 0x131B);
}

void InitTotemPole(NAS *nas) {
    EXTobject_TA *ext;
    EXTengine *e;

    TotemPole = nas;
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        if (!(((u16 *)&GameExitFlags)[1] & 0x80)) {
            ext->Counter = 0x384;
        } else {
            ext->Counter = 0x96;
        }
        ext->Status = 1;
        e = nas->Engine;
        e->flags = 0;
        TotemTop = CreateNAS(e->unk10.x, e->unk10.y - 0x294, e->unk10.z, 0x131B, 0x11);
        TotemBot = CreateNAS(e->unk10.x, e->unk10.y - 0x14A, e->unk10.z, 0x131C, 0x11);
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void PlotPillarCover(NAS *nas) {
    switch (nas->UserInfo.user1) {
    case 6:
        nas->UserInfo.status++;
        if (nas->UserInfo.status >= 4) {
            KillNAS(nas);
            return;
        }
        break;
    case 7:
        if (nas->UserInfo.status == 0) {
            KillNAS(nas);
            return;
        }
        nas->UserInfo.status--;
        break;
    }
    nas->Engine->sub->entity = PillarCoverFrames[nas->UserInfo.status].v;
}

void InitPillarCover(NAS *nas) {
    EXTengine *e = nas->Engine;
    e->unk10.y = 0;
    e->sub->u6C.dbase = MapScene->dbase;
}

void InitPillar(NAS *nas) {
    EXTengine *e = nas->Engine;
    subengine *s;
    nas->UserInfo.user1 = 1;
    nas->UserInfo.status = 6;
    e->unk10.y = 0xB8;
    s = e->sub;
    s->u6C.dbase = MapScene->dbase;
    s->unk68 = 0x20;
    s->flags |= 0x02000000;
}

void InitPillarControl(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x5A;
}

void InitBoatField(NAS *nas) {
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBoatWake(NAS *nas) {
    EXTengine *src;
    EXTengine *dst;
    u16 t;

    src = Heroes[nas->UserInfo.user1].hero_eng;
    dst = nas->Engine;
    t = src->unk1E;
    dst->unk5E = 1;
    dst->unk1E = t;
    dst->unk10.x = src->unk10.x;
    dst->unk10.z = src->unk10.z;
    if (nas->UserInfo.status == 0) {
        nas->UserInfo.status = 0;
    }
    if (Heroes[nas->UserInfo.user1].hero_health <= 0) {
        KillNAS(nas);
    }
}

void InitBoatWake(NAS *nas) {
    subengine *sub;

    nas->UserInfo.status = 0;
    sub = nas->Engine->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankVehicle];
    sub->entity = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitPeriscopeGun(NAS *nas) {
    nas->Engine->sub->u6C.dbase = MapScene->dbase;
    nas->Engine->sub->entity = 0x4080;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitPeriscope(NAS *nas) {
    EXTengine *eng;
    subengine *s;
    s32 one;
    void *p;
    EXTobject_TA *ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->Counter = 0x12C;
        one = 1;
        ext->Status = one;
        eng = nas->Engine;
        eng->unk5D = 0;
        p = MapScene->dbase;
        s = eng->sub;
        s->flags = 0;
        s->u6C.dbase = p;
        if (eng_add_subs(eng, one) != 0) {
            s = s->next;
            s->flags = 0;
            s->u6C.dbase = MapScene->dbase;
            s->entity = 0x2001;
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
        }
    } else {
        KillNAS(nas);
    }
}

void InitRock_TA(NAS *nas) {
    MapTriggerInfo *info;
    EXT *ext;
    subengine *sub;
    u32 val;

    info = nas->trigger->InfoStruct;
    val = info->val1;
    nas->UserInfo.status = 1;
    nas->UserInfo.user2 = val;
    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = ext;
    sub = nas->Engine->sub;
    sub->u6C.dbase = MapScene->dbase;
    sub->entity = RockFrames[info->val1].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBigApple(NAS *nas) {
}

void InitBigApple(NAS *nas) {
}

void KillPickupTA(NAS *nas) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (PU_TA_list[i] == nas) {
            PU_TA_list[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPickupGenerator_TA(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0xF0;
}

void PlotShellExplosion(NAS *nas) {
    subengine *sub = nas->Engine->sub;

    sub->entity++;
    if ((u16)sub->entity >= 0x2022) {
        KillNAS(nas);
    }
}

void InitShellExplosion(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    sub->entity = 0x2019;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitShrapnelBit(NAS *nas) {
    subengine *sub = nas->Engine->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
    sub->entity = ShrapnelFrames[Random(3)].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotMineBall(NAS *nas) {
    if (++nas->UserInfo.status >= 8) {
        KillNAS(nas);
        return;
    }
    nas->Engine->sub->entity = MineBallFrames[nas->UserInfo.status];
}

void InitMineBall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    sub->entity = 0x2019;
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitTorpedo(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    nas->UserInfo.status = 0x5A;
    nas->UserInfo.flags = 0x11;
    nas->UserInfo.user2 = 0;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->u6C.dbase = MapScene->dbase;
    sub->entity = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (TankMines[i] == NULL) {
                TankMines[i] = nas;
                break;
            }
        }
    }
}

void InitWaterMine(NAS *nas) {
    EXTengine *e;
    subengine *s;
    nas->UserInfo.flags = 1;
    nas->UserInfo.user1 = -1;
    nas->UserInfo.status = GameFrameCount;
    e = nas->Engine;
    nas->UserInfo.user2 = 0;
    e->unk10.y = -0x14;
    s = e->sub;
    s->u6C.dbase = MapScene->dbase;
    s->entity = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitMine(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    sub->entity = 0x2005;
    nas->UserInfo.status = 0x78;
    nas->UserInfo.user1 = -1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (TankMines[i] == NULL) {
                TankMines[i] = nas;
                break;
            }
        }
    }
}

void InitRocketTA(NAS *nas) {
    EXTobject_TA *ext;
    subengine *sub;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = (EXT *)ext;
    sub = nas->Engine->sub;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects2];
    sub->entity = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLobber_TA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
    eng->sub->entity = 0x200C;
    eng->unk5D = 1;
    e2 = AddExtEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->u6C.dbase = ((u8 *)&HeroModelBank[DB_TankObjects]) - 0x700;
        e2->sub->entity = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitShell(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
    eng->sub->entity = 0x200C;
    eng->unk5D = 1;
    e2 = AddExtEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->u6C.dbase = ((u8 *)&HeroModelBank[DB_TankObjects]) - 0x700;
        e2->sub->entity = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitSwirlyTA(NAS *nas) {
    EXTscriptdata *ext;
    EXTengine *eng;
    subengine *sub;
    void *p;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        p = &HeroModelBank[DB_TankObjects];
        ext->maxloops = 0x87;
        eng = nas->Engine;
        sub = eng->sub;
        eng->unk5D = 0;
        sub->u6C.dbase = p;
        sub->entity = 0x2038;
        sub->flags |= 0x20000000;
        eng->flags |= 0x21;
        if (eng_add_subs(eng, 2) == 2) {
            sub = sub->next;
            sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
            sub->entity = 0x2030;
            sub = sub->next;
            sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
            sub->entity = 0x2030;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void InitFlubberTA(NAS *nas) {
    EXTscriptdata *ext;
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    void *p;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        ext->maxloops = 0x4B;
        ext->timer = -0x1E;
        p = &HeroModelBank[DB_TankObjects];
        eng = nas->Engine;
        sub = eng->sub;
        sub->u6C.dbase = p;
        sub->entity = 0x2016;
        eng->unk5D = 1;
        e2 = AddExtEngine(nas, 1);
        if (e2 != NULL) {
            e2->unk10.x = eng->unk10.x;
            e2->unk10.y = 0;
            e2->unk10.z = eng->unk10.z;
            e2->sub->unk51 = 2;
            e2->sub->u6C.dbase = ((u8 *)&HeroModelBank[DB_TankObjects]) - 0x700;
            e2->sub->entity = 0x2001;
            e2->sub->flags |= 0x40000041;
            e2->sub->unk76 = 0x800;
            e2->sub->unk50 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

void InitFlamerBitTA(NAS *nas) {
    EXTobject_TA *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = (EXT *)ext;
    ext->Counter = 0x3F;
    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
    sub->entity = 0x202F;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLaserBitTA(NAS *nas) {
    EXTobject_TA *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExt(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = (EXT *)ext;
    ext->Counter = 0x5A;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
    sub->entity = 0x2021;
    eng->flags |= 0x21;
    if (eng_add_subs(eng, 1) != 0) {
        sub = sub->next;
        sub->u6C.dbase = &HeroModelBank[DB_TankObjects];
        sub->entity = 0x2023;
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void BumpSpeedsForSwamp(EXTobject_TA *pObject) {
    pObject->SpeedX = pObject->SpeedX * 3 / 2;
    pObject->SpeedZ = pObject->SpeedZ * 3 / 2;
    pObject->Counter = pObject->Counter * 2 / 3;
}

void TankHeroMinelay(u32 player) {
}

void SetTankHeroMinelay(u32 arg0) {
    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        TankLevelOver = arg0 + 1;
    }
}

void TankHeroDead(u32 player) {
    HeroGhost(player);
}

void SetTankHeroDie(u32 player) {
    SetHeroSeq(player, 0x58);
}

s32 TankBlockPadNear(s32 flags, NAS *nas, EXTengine *eng) {
    s32 dx = nas->Engine->unk10.x - eng->unk10.x;
    s32 dz = nas->Engine->unk10.z - eng->unk10.z;

    if (dx < -0x15D) {
        return flags;
    }
    if (dx >= 0x15E || dz < -0x15D || dz >= 0x15E) {
        return flags;
    }
    flags &= (dx < 0) ? -0x81 : -0x21;
    flags &= (dz > 0) ? -0x11 : -0x41;
    return flags;
}

void SetTankHeroMove(u32 arg0) {
    s32 ok;

    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        TankLevelOver = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0) {
        SetHeroSeq(arg0, 0x59);
    }
}

/* Start a 90-degree turn to MoveDir dir: hero mode 0x6F (turn), ModeSpeed -0x80 a frame for
 * 8 frames (up to right is -0x80, so clockwise). TankHeroGridMove does the same inline. */
void TankHeroTurnClockwise(s32 player, TA_player *ta, s32 dir) {
    SetHeroAction(player, 0x6F);
    ta->ModeSpeed = -0x80;
    ta->ModeCounter = 8;
    ta->MoveDir = dir;
}

/* The same turn the other way (+0x80 a frame). */
void TankHeroTurnAnticlockwise(s32 player, TA_player *ta, s32 dir) {
    SetHeroAction(player, 0x6F);
    ta->ModeSpeed = 0x80;
    ta->ModeCounter = 8;
    ta->MoveDir = dir;
}

void TankHeroIdle(u32 player) {
    SetHeroAction(player, 0x6E);
}

void KillTankMainWeaponNas(NAS *nas, s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (TA_players[player].MainWeaponNas[i] == nas) {
            TA_players[player].MainWeaponNas[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void SetFireBreatherSFX(s32 player, EXTengine *unused, s32 ReqdSFX) {
    if (TA_players[player].FireBreatherSFX != ReqdSFX) {
        TA_players[player].FireBreatherSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(TA_players[player].FireBreatherSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &TA_players[player].FireBreatherSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void SetTurretSFX(s32 player, EXTengine *unused, s32 ReqdSFX) {
    if (TA_players[player].TurretSFX != ReqdSFX) {
        TA_players[player].TurretSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(TA_players[player].TurretSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &TA_players[player].TurretSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void SetHullSFX(s32 player, EXTengine *pEngine, s32 ReqdSFX) {
    if (TA_players[player].HullSFX != ReqdSFX) {
        TA_players[player].HullSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            if (pEngine->sfx != NULL) {
                StopSfx(pEngine->sfx);
            }
        } else {
            PlaySfx(ReqdSFX, &pEngine->sfx, &pEngine->unk10, 0x1000, 0x1E00, 0);
        }
    }
}

/* The check every SetTankHero* starts with: a quit (pad 0x801) ends the level, and a hero
 * whose status has 0x200 (won) gets the victory action (0x70) and TankLevelOver. */
s32 CheckTankHeroWon(s32 arg0) {
    if (GameExitFlags & 0x801) {
        TankLevelOver = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction(arg0, 0x70);
        TankLevelOver = arg0 + 1;
        return 1;
    }
    return 0;
}

void TA_PreMoveHero(u32 player) {
}

void DamageTankHeroesNear(s32 x, s32 z, s32 range, s32 arg3) {
    HeroInfo *h;
    s32 d;
    s32 i;

    s32 neg;

    i = 0;
    neg = -range;
    for (h = Heroes; i < 4; i++, h++) {
        d = x - h->hero_eng->unk10.x;
        if (d >= neg && d <= range) {
            d = z - h->hero_eng->unk10.z;
            if (d >= neg && d <= range) {
                DamageTankHero(i, arg3);
            }
        }
    }
}

bool TA_KeepInBounds(EXTengine *p) {
    s32 r;
    s32 lim;

    r = 0;
    if (p->unk10.x > TankArenaMaxX) {
        lim = TankArenaMaxX;
        p->unk10.x = lim;
        r = 1;
    }
    if (p->unk10.x < 0x100) {
        p->unk10.x = 0x100;
        r = 1;
    }
    if (p->unk10.z > TankArenaMaxZ) {
        lim = TankArenaMaxZ;
        p->unk10.z = lim;
        r = 1;
    }
    if (p->unk10.z < 0x100) {
        p->unk10.z = 0x100;
        r = 1;
    }
    return r;
}

/* An empty function nothing references; its place in the file's inline tail is between
 * TA_Triggers and TA_KeepInBounds, and no leaked name fits it. */
void TankEmptyStub(void) {
}

void TA_Triggers(NASTrigger *arg0, s32 arg1) {
}

void TA_EarliestTriggers(void) {
}

void TA_CreateShadow(u32 arg0) {
    Heroes[arg0].hero_shadow->flags = 0;
}

void TA_StopHeroSfx(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (TA_players[i].TurretSFXHandler != NULL) {
            StopSfx(TA_players[i].TurretSFXHandler);
        }
        if (TA_players[i].FireBreatherSFXHandler != NULL) {
            StopSfx(TA_players[i].FireBreatherSFXHandler);
        }
    }
}

/* Empty, defined right after TA_SetUp, where Arkenoid's leaked header puts AR_RestartLevel
 * (its empty func_800C0B8C sits in the same place). */
void TA_RestartLevel(void) {
}

void TA_SetUp(void) {
    LevelSpecs.LevelPreInit = TA_PreInitLevel;
    LevelSpecs.LevelInit = TA_InitLevel;
    LevelSpecs.LevelRestartInit = TA_InitLevel;
    LevelSpecs.LevelRoutine = TA_UpdateLevel;
    LevelSpecs.LevelExit = TA_StopHeroSfx;
    LevelSpecs.PostTriggers = TA_Triggers;
    LevelSpecs.EarliestTriggers = TA_EarliestTriggers;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.FinishQuick = FinishTankLevel;
    TankBossStartStatus = 1;
}

void TriggerTankMinesNear(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = TankMines[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    nas->UserInfo.status = 1;
                }
            }
        }
    }
}

NAS *FindTankObjectNear(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = TankObjects[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    return nas;
                }
            }
        }
    }
    return NULL;
}


void RemoveTankObject(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (TankObjects[i] == nas) {
            TankObjects[i] = NULL;
            return;
        }
    }
}

void AddTankObject(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (TankObjects[i] == NULL) {
            TankObjects[i] = nas;
            return;
        }
    }
}

void ClearTankObjects(void) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        TankObjects[i] = NULL;
    }
}

void SetPuffAngle(NAS *puff, s16 angle) {
    subengine *sub;

    puff->Engine->unk1E = angle;
    puff->Engine->unk5E = 1;
    sub = puff->Engine->sub;
    if (sub != NULL) {
        do {
            sub->flags |= 0x20000000;
            sub = sub->next;
        } while (sub != NULL);
    }
}

s32 TankFindHeroNear(s32 x, s32 z, s32 r, s32 skip) {
    s32 i;
    s32 dx;
    s32 dz;
    EXTengine *eng;
    u16 name;
    u8 *g;

    s32 nine;

    for (i = 0, nine = 9, g = GameInfo; i < 4; i++) {
        if (i == skip) {
            continue;
        }
        name = Heroes[i].hero_name;
        if (name == nine &&
            (((TankModeView *)g)->mode != name ||TankBossStatus != 0 || (u32)(i - 2) >= 2U)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        eng = Heroes[i].hero_eng;
        dx = x - eng->unk10.x;
        if (dx < -r || r < dx) {
            continue;
        }
        dz = z - eng->unk10.z;
        if (dz <= -r || dz >= r) {
            continue;
        }
        return i;
    }
    return -1;
}

s32 RocketFindHero(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((TankModeView *)GameInfo)->mode != name || TankBossStatus != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = arg0 - e->unk10.x;
        if (d < -arg2) {
            continue;
        }
        if (arg2 < d) {
            continue;
        }
        d = arg1 - e->unk10.z;
        if (-arg2 < d && d < arg2) {
            return i;
        }
    }
    return -1;
}

void KillTankMapCell(s32 arg0, s32 arg1) {
    s32 idx;
    NAS *nas;

    idx = ((arg1 >> 9) * TankMapWidth) + (arg0 >> 9);
    nas = (NAS *)TankMapNas[idx];
    TankMapNas[idx] = 0;
    TankLevelsGridMapMiss[idx] = 0;
    TankLevelsGridMapTank[idx] = 0;
    KillNAS(nas);
}

s32 GetTankMapCell(s32 arg0, s32 arg1) {
    return TankMapNas[((arg1 >> 9) * TankMapWidth) + (arg0 >> 9)];
}

void SetTankMapCellNas(NAS *nas) {
    s32 temp_a0;
    s32 temp_a1;
    s32 idx;
    u8 temp_a2;

    temp_a1 = nas->Engine->unk10.z >> 9;
    temp_a0 = nas->Engine->unk10.x >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    idx = (temp_a1 * TankMapWidth) + temp_a0;
    TankLevelsGridMapMiss[idx] = temp_a2;
    TankLevelsGridMapTank[idx] = temp_a2;
    TankMapNas[idx] = (s32)nas;
}

u32 GetSandbagValueTank(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * TankMapWidth) + (arg0 >> 9) + TankLevelsGridMapTank);
}

u32 GetSandbagValueMiss(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * TankMapWidth) + (arg0 >> 9) + TankLevelsGridMapMiss);
}

void ClearTankMapCell(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    TankLevelsGridMapMiss[(temp_a1 * TankMapWidth) + temp_a0] = 0;
    TankLevelsGridMapTank[(temp_a1 * TankMapWidth) + temp_a0] = 0;
}

void SetTankMapCell(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;
    u8 temp_a2;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    TankLevelsGridMapMiss[(temp_a1 * TankMapWidth) + temp_a0] = temp_a2;
    TankLevelsGridMapTank[(temp_a1 * TankMapWidth) + temp_a0] = temp_a2;
}

void TankBotReset(s32 player) {
    TA_Bot *bot;

    bot = &TankBotAI[player];
    bot->Doing = 0;
    bot->DoingTimer = 0;
    bot->Victim = 0;
    bot->pMine = NULL;
    bot->pMissile = NULL;
    bot->AIpad = 0;
    bot->FirePercentChance = 0;
}

NAS *FindTankPickupNear(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 3; i++) {
        nas = PU_TA_list[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

void TankBotAimTurret(s32 player, EXTengine *eng, s32 *arg2) {
    s32 hero;
    EXTengine *target;

    if (GameMode != 5) {
        hero = arg2[2];
        if (hero >= 0) {
            target = Heroes[hero].hero_eng;
            TA_players[player].TurretReqdDir =
                (get_angle(target->unk10.x - eng->unk10.x, target->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
        }
    }
}

void TankBotChooseTarget(void) {
    switch ((s16)(GameMode - 5)) {
    case 1:
    case 3:
        TankBotPickLinedUpTarget();
        break;
    case 0:
    case 2:
    case 4:
    default:
        TankBotPickTarget();
        break;
    }
}
