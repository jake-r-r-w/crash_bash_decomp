#include "common.h"
#include "bash/engine.h"
#include "bash/chase_ovl.h"
#include "bash/chase.h"
#include "bash/pogo.h"
#include "bash/bash.h"
#include "bash/xtras.h"
#include "bash/bonk.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char ChaseAnimBreathe[] = "BREATHE";
const char ChaseAnimMedium[] = "MEDIUM";
const char ChaseAnimHit[] = "HIT";
const char ChaseAnimJump[] = "JUMP";
const char ChaseAnimWin[] = "WIN";

/* data, in address order */
s16 ChasePreLoadDB[6] = { 0x1C, 0x17, 0x61, 0x1A, -1, 0 };
char *ChaseAnims[5] = {
    (char *)ChaseAnimBreathe, (char *)ChaseAnimMedium, (char *)ChaseAnimHit, (char *)ChaseAnimJump,
    NULL,
};
s16 ChaseLevelDB[2] = { -1, 0 };
WWInf ChaseWorldInfo[2] = {
    { 0x5F, ChaseAnims, ChaseLevelDB, 0 },
    { 0x60, ChaseAnims, ChaseLevelDB, 0 },
};
GroupFile ChaseFiles[7] = {
    &HeroModelBank[DB_Font], &DatIndex[315], &DatIndex[314], &HeroModelBank[DB_Loading], &DatIndex[311],
    &DatIndex[310], &HeroModelBank[DB_Interface], &DatIndex[313], &DatIndex[312], &HeroModelBank[DB_Powerups],
    &DatIndex[309], &DatIndex[308], &HeroModelBank[DB_Angel], &DatIndex[307], &DatIndex[306],
    &HeroModelBank[DB_KegObjects], &DatIndex[305], &DatIndex[304], 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 0x1E's) */
file_group_load_info file_group_CHASE = { &FileGroups[5], ChaseFiles };
u16 ChaseLUPtab[8] = { 3, 0x20D, 0x201, 0x20D, 0x1301, 0x1306, 0x1309, 0x130A };
TrigLookUp ChaseLUP = { ChaseLUPtab, 8 };
PreMap CHASE_PREMAP = { ChasePreLoadDB, ChaseWorldInfo, &ChaseLUP };
u16 ChaseHeroModels[10] = {
    0, 0x62, 0x63, 0x64, 0x66, 0x65, 0x68, 0x67, 0x69, 0,
};
VictorySeq ChaseVictorySeqs[4] = {
    { &ChaseAnimWin, 0x6A },
    { &ChaseAnimWin, 0x6A },
    { &ChaseAnimWin, 0x6A },
    { &ChaseAnimWin, 0x6A },
};
s32 ChaseHitSfx[9][4] = {
    { 3, 4, 5, 6 }, { 0x35, 0x36, 0x37, 0x38 }, { 0x67, 0x68, 0x69, 0x6A },
    { 0x99, 0x9A, 0x9B, 0x9C }, { 0x12F, 0x130, 0x131, 0x132 }, { 0x161, 0x162, 0x163, 0x164 },
    { 0xFD, 0xFE, 0xFF, 0x100 }, { 0xCB, 0xCC, 0xCD, 0xCE }, { 0x749, 0x748, 0x747, 0x746 },
};
s32 ChaseBarrelSfx[4] = { 0x745, 0x744, 0x743, 0x742 };
s32 ChaseFlameSfx[4] = { 0x741, 0x740, 0x73F, 0x73E };
s32 ChaseChainSfx[4] = { 0x750, 0x751, 0x752, 0x753 };
/* the shell settings (engine world record 0x010D) */
shell_setup chase_shell_info = {
    0x4000, 0x20, 0x1800, 0x80, 0x200, 0x100, VramAreas, VramNodePool, 0x20, 0x5DC,
};
ChaseSpawnOfs ChaseSpawnOffsets[8] = {
    { 0x82, -0x384, 0xDC },
    { 0x64, -0x4B0, 0xFA },
    { 0x64, 0x44C, 0xC8 },
    { 0x64, 0x44C, 0xC8 },
    { 0x82, -0x5DC, 0xFA },
    { 0x82, -0x5DC, 0x12C },
    { 0x82, -0x44C, 0x12C },
    { 0x82, -0x44C, 0x12C },
};
hero_seq CRA_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq CRA_seq_kegbreathe[6] = {
    0, 0x8007, 0x16, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq CRA_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq CRA_seq_kegmedium[14] = {
    0, 0x4093, 0x8003, 8, 0x8007, 0xA, 0x4080, 0x8003, 9, 0x8007, 9, 0x408A, 0x8000, 0,
};
hero_seq CRA_seq_kegjump[12] = {
    0, 0x800A, 0xC, 0x8007, 7, 0x4105, 0x8018, 0x28, 0x410C, 0x8004, 0x7A, 0,
};
hero_seq CRA_seq_kegfall[16] = {
    2, 0x410D, 0x8018, 4, 0x410E, 0x410F, 0x8018, 0x28, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110,
    0x8004, 0x7B, 0,
};
hero_seq CRA_seq_kegland[8] = {
    0, 0x8009, 0xD, 0x8007, 0x1B, 0x4111, 0x8004, 0x71,
};
hero_seq CRA_seq_keghit[12] = {
    0, 0x8007, 9, 0x4200, 0x8003, 8, 0x8007, 0xA, 0x4209, 0x8004, 0x71, 0,
};
hero_seq CRA_seq_kegvictory[6] = {
    0, 0x8007, 0x16, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_kegwin[6] = {
    0, 0x8007, 0x2A, 0x4180, 0x8000, 0,
};
hero_seq CHASE_seq_kegdead[6] = {
    0, 0x8007, 0x14, 0x4180, 0x8000, 0,
};
hero_seq COC_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq COC_seq_kegbreathe[6] = {
    0, 0x8007, 0x1B, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq COC_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq COC_seq_kegmedium[14] = {
    0, 0x4095, 0x8003, 8, 0x8007, 0xB, 0x4080, 0x8003, 9, 0x8007, 0xA, 0x408A, 0x8000, 0,
};
hero_seq COC_seq_kegjump[12] = {
    0, 0x800A, 0x3E, 0x8007, 7, 0x4106, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq COC_seq_kegfall[14] = {
    2, 0x410E, 0x410F, 0x4110, 0x8018, 4, 0x4111, 0x4112, 0x8018, 0x28, 0x4113, 0x8004, 0x7B, 0,
};
hero_seq COC_seq_kegland[8] = {
    0, 0x8009, 0x3F, 0x8007, 9, 0x4114, 0x8004, 0x71,
};
hero_seq COC_seq_keghit[12] = {
    0, 0x8007, 0xE, 0x4180, 0x8003, 8, 0x8007, 5, 0x418E, 0x8004, 0x71, 0,
};
hero_seq COC_seq_kegwin[6] = {
    0, 0x8007, 0x32, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_kegvictory[6] = {
    0, 0x8007, 0x1B, 0x4000, 0x8004, 0x71,
};
hero_seq TIN_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq TIN_seq_kegbreathe[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq TIN_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq TIN_seq_kegmedium[14] = {
    0, 0x4093, 0x8003, 8, 0x8007, 0xA, 0x4080, 0x8003, 9, 0x8007, 9, 0x408A, 0x8000, 0,
};
hero_seq TIN_seq_kegjump[12] = {
    0, 0x800A, 0x138, 0x8007, 7, 0x4106, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq TIN_seq_kegfall[18] = {
    2, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x8018, 4, 0x4113, 0x4114, 0x4115, 0x8018, 0x28,
    0x4116, 0x4116, 0x8004, 0x7B, 0,
};
hero_seq TIN_seq_kegland[8] = {
    0, 0x8009, 0x139, 0x8007, 0xB, 0x4117, 0x8004, 0x71,
};
hero_seq TIN_seq_keghit[12] = {
    0, 0x8007, 0x13, 0x4180, 0x8003, 8, 0x8007, 0xA, 0x4193, 0x8004, 0x71, 0,
};
hero_seq TIN_seq_kegwin[10] = {
    0, 0x8007, 0x26, 0x4200, 0x8007, 0x27, 0x4226, 0x8000, 3, 0,
};
hero_seq TIN_seq_kegvictory[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq CTX_seq_kegbreathe[6] = {
    0, 0x8007, 0x1E, 0x4000, 0x8000, 0,
};
hero_seq CTX_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq CTX_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq CTX_seq_kegmedium[14] = {
    0, 0x4193, 0x8003, 8, 0x8007, 0xB, 0x4180, 0x8003, 9, 0x8007, 0xA, 0x418A, 0x8000, 0,
};
hero_seq CTX_seq_kegjump[12] = {
    0, 0x800A, 0x138, 0x8007, 7, 0x4106, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq CTX_seq_kegfall[18] = {
    2, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x8018, 4, 0x4113, 0x4114, 0x4115, 0x8018, 0x28,
    0x4116, 0x4116, 0x8004, 0x7B, 0,
};
hero_seq CTX_seq_kegland[8] = {
    0, 0x8009, 0x71, 0x8007, 5, 0x4116, 0x8004, 0x71,
};
hero_seq CTX_seq_keghit[12] = {
    0, 0x8007, 0x14, 0x4080, 0x8003, 8, 0x8007, 0xF, 0x4093, 0x8004, 0x71, 0,
};
hero_seq CTX_seq_kegwin[10] = {
    0, 0x8007, 9, 0x4200, 0x8007, 0x14, 0x4209, 0x8000, 3, 0,
};
hero_seq CTX_seq_kegvictory[6] = {
    0, 0x8007, 0x1E, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq BRO_seq_kegbreathe[6] = {
    0, 0x8007, 0x13, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq BRO_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq BRO_seq_kegmedium[14] = {
    0, 0x4095, 0x8003, 8, 0x8007, 0xB, 0x4080, 0x8003, 9, 0x8007, 9, 0x408B, 0x8000, 0,
};
hero_seq BRO_seq_kegjump[12] = {
    0, 0x800A, 0xA2, 0x8007, 6, 0x4106, 0x8018, 0x28, 0x410C, 0x8004, 0x7A, 0,
};
hero_seq BRO_seq_kegfall[14] = {
    2, 0x410D, 0x410E, 0x8018, 4, 0x410F, 0x4110, 0x4111, 0x8018, 0x28, 0x4112, 0x4113, 0x8004,
    0x7B,
};
hero_seq BRO_seq_kegland[8] = {
    0, 0x8009, 0xA3, 0x8007, 2, 0x4111, 0x8004, 0x71,
};
hero_seq BRO_seq_keghit[12] = {
    0, 0x8007, 0xF, 0x4180, 0x8003, 8, 0x8007, 5, 0x418E, 0x8004, 0x71, 0,
};
hero_seq BRO_seq_kegwin[6] = {
    0, 0x8007, 0x18, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_kegvictory[6] = {
    0, 0x8007, 0x13, 0x4200, 0x8000, 0,
};
hero_seq DNG_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq DNG_seq_kegbreathe[6] = {
    0, 0x8007, 0x16, 0x4080, 0x8000, 0,
};
hero_seq DNG_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq DNG_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq DNG_seq_kegmedium[14] = {
    0, 0x4011, 0x8003, 8, 0x8007, 9, 0x4000, 0x8003, 9, 0x8007, 8, 0x4009, 0x8000, 0,
};
hero_seq DNG_seq_kegjump[12] = {
    0, 0x800A, 0x106, 0x8007, 7, 0x4106, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq DNG_seq_kegfall[18] = {
    2, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x8018, 4, 0x4113, 0x4114, 0x4115, 0x8018, 0x28,
    0x4116, 0x4116, 0x8004, 0x7B, 0,
};
hero_seq DNG_seq_kegland[8] = {
    0, 0x8009, 0x139, 0x8007, 0xB, 0x4117, 0x8004, 0x71,
};
hero_seq DNG_seq_keghit[12] = {
    0, 0x8007, 0x13, 0x4180, 0x8003, 8, 0x8007, 0xD, 0x4193, 0x8004, 0x71, 0,
};
hero_seq DNG_seq_kegwin[10] = {
    0, 0x8007, 0x15, 0x4200, 0x8007, 0xD, 0x4216, 0x8000, 3, 0,
};
hero_seq DNG_seq_kegvictory[6] = {
    0, 0x8007, 0x16, 0x4080, 0x8000, 0,
};
hero_seq KON_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq KON_seq_kegbreathe[6] = {
    0, 0x8007, 0x20, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq KON_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq KON_seq_kegmedium[14] = {
    0, 0x4093, 0x8003, 8, 0x8007, 0xA, 0x4080, 0x8003, 9, 0x8007, 9, 0x408A, 0x8000, 0,
};
hero_seq KON_seq_kegjump[12] = {
    0, 0x800A, 0x16A, 0x8007, 4, 0x4109, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq KON_seq_kegfall[18] = {
    2, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x8018, 4, 0x4113, 0x4114, 0x4115, 0x8018, 0x28,
    0x4116, 0x4116, 0x8004, 0x7B, 0,
};
hero_seq KON_seq_kegland[8] = {
    0, 0x8009, 0x16B, 0x8007, 0xB, 0x4117, 0x8004, 0x71,
};
hero_seq KON_seq_keghit[12] = {
    0, 0x8007, 0x12, 0x4180, 0x8003, 8, 0x8007, 0xA, 0x4192, 0x8004, 0x71, 0,
};
hero_seq KON_seq_kegwin[10] = {
    0, 0x8007, 0x21, 0x4000, 0x8007, 0x2D, 0x4021, 0x8000, 3, 0,
};
hero_seq KON_seq_kegvictory[6] = {
    0, 0x8007, 0x20, 0x4200, 0x8000, 0,
};
hero_seq KAN_seq_kegnone[4] = {
    2, 0, 0x8000, 0,
};
hero_seq KAN_seq_kegbreathe[6] = {
    0, 0x8007, 0x14, 0x4200, 0x8000, 0,
};
hero_seq KAN_seq_kegidle_a[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq KAN_seq_kegidle_b[4] = {
    0, 0x8005, 0x61, 0,
};
hero_seq KAN_seq_kegmedium[14] = {
    0, 0x4093, 0x8003, 8, 0x8007, 0xA, 0x4080, 0x8003, 9, 0x8007, 9, 0x408A, 0x8000, 0,
};
hero_seq KAN_seq_kegjump[12] = {
    0, 0x800A, 0xD4, 0x8007, 7, 0x4106, 0x8018, 0x28, 0x410D, 0x8004, 0x7A, 0,
};
hero_seq KAN_seq_kegfall[16] = {
    2, 0x410E, 0x410F, 0x4110, 0x8018, 4, 0x4111, 0x4112, 0x4113, 0x8018, 0x28, 0x4114, 0x4115,
    0x8004, 0x7B, 0,
};
hero_seq KAN_seq_kegland[8] = {
    0, 0x8009, 0xD5, 0x8007, 9, 0x4115, 0x8004, 0x71,
};
hero_seq KAN_seq_keghit[12] = {
    0, 0x8007, 0x10, 0x4180, 0x8003, 8, 0x8007, 0xA, 0x4191, 0x8004, 0x71, 0,
};
hero_seq KAN_seq_kegwin[6] = {
    0, 0x8007, 0xC, 0x4000, 0x8000, 0,
};
hero_seq KAN_seq_kegvictory[6] = {
    0, 0x8007, 0x14, 0x4200, 0x8000, 0,
};
u16 ChaseAiSkill[10] = {
    0x15, 0x13, 0x11, 0x10, 0x16, 0x18, 0x15, 0x10, 0x10, 0x19,
};
u16 ChaseAiSkillAlt[10] = {
    0x16, 0x14, 0x12, 0x10, 0x16, 0x18, 0x15, 0x10, 0x10, 0x19,
};
u16 ChaseAiTeamSkill[6] = {
    0xE, 0x13, 0x16, 0xE, 0x13, 0x16,
};
u16 ChaseAiHumanRoundTweak[12] = {
    0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14,
};
u16 ChaseAiBotTweak[10] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 ChaseAiHumanTweak[72] = {
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0, 0, 1, 0, 2, 0, 3, 0, 0, 0, 1, 0,
    2, 0, 0, 0, 0xFEE8, 0xFFFF, 0xB4, 0, 0, 0, 0xFEE7, 0xFFFF, 0xB4, 0, 0, 0, 0xFE82, 0xFFFF, 0xC8,
    0, 0, 0, 0xFE81, 0xFFFF, 0xC8, 0, 0, 0, 0xFF02, 0xFFFF, 0xC8, 0, 0, 0, 0xFF01, 0xFFFF, 0xC8, 0,
    0, 0, 0xFE06, 0xFFFF, 0xA0, 0, 0, 0, 0xFE05, 0xFFFF, 0xA0, 0,
};
u16 ChaseHeroShadowModels[10] = { 0, 0x2002, 0x2003, 0x2001, 0x2003, 0x2001, 0x2002, 0x2001, 0x2002, 0 };
u16 ChaseHeroShadowSizes[10] = { 0, 0x4000, 0x4000, 0x4000, 0x4200, 0x4000, 0x4200, 0x4080, 0x4200, 0 };
CircleFireOfs CircleFireOffsets[8] = {
    { -0x80, -0x80 }, { -0x80, 0 }, { -0x80, 0x80 }, { 0, -0x80 }, { 0, 0x80 },
    { 0x80, -0x80 }, { 0x80, 0 }, { 0x80, 0x80 },
};
s32 ChaseUnused0[8] = { -0x100, 0, 0x100, 0, 0, 0x100, 0, -0x100 };
u8 ChaseHeroSlots[48] = {0};
s32 ChaseUpdateState[36] = {0};
NAS *ChaseHeroCoal[4] = {0};
s32 ChaseHeroState[4][21] = {0};
NAS *ChasePickup[1] = {0};
NAS *ChaseBarrels[8] = {0};
s32 ChaseRoundOver = 0;
s32 ChaseArenaRadius = 0;
s32 ChaseArenaShrink = 0;
s32 ChaseBarrelCount = 0;
NAS *ChaseGenerator = 0;
NAS *ChaseGeneratorB = 0;
EXTengine *ChaseFlameTimer[4] = {0};
s32 ChaseFlameState[4] = {0};
s32 ChaseFlameSfxHandle[4] = {0};
s32 ChaseFlameObj[4] = {0};
s32 ChaseHeroSpeed[4] = {0};
u16 ChaseRunSpeeds[30] = {0};
NAS *ChaseGrid[1600] = {0};

void ChaseUpdateStub(s32 *arg0) {
}

void ChaseAI_AngleTo(s32 arg0, EXTengine *arg1) {
    HeroInfo *h;
    s32 hx, hz;
    s32 a;
    s32 dx, dz;
    ChaseAiState *p;

    hx = Heroes[arg0].hero_eng->XYZ.x;
    hz = Heroes[arg0].hero_eng->XYZ.z;
    h = &Heroes[arg0];
    a = get_angle(arg1->XYZ.x - hx, arg1->XYZ.z - hz);
    switch (h->hero_name) {
    case 3:
    case 4:
        a -= 0x5A;
        break;
    default:
        a += 0x5A;
        break;
    }
    a &= 0xFFF;
    dx = (rcossin_tbl[(a - 0x400) & 0xFFF].unk2 * 256) >> 12;
    dz = (rcossin_tbl[a].unk2 * 256) >> 12;
    get_angle(arg1->XYZ.x + dx - hx, arg1->XYZ.z + dz - hz);
    p = (ChaseAiState *)((arg0 * 0x30) + ChaseHeroSlots);
    p->x = arg1->XYZ.x + dx;
    p->z = arg1->XYZ.z + dz;
    p->angle = get_angle(p->x - hx, p->z - hz);
}

void ChaseAI_ChooseTarget(s32 arg0, EXTengine *arg1, ChaseAiState *arg2) {
    NAS *nas;
    NAS *best;
    EXTengine *eng;
    s32 x, z;
    s32 i;
    s32 d, dx, dz;
    s32 bestd;
    u32 r;
    ChaseCoalFux *fux;

    if (Random(0x40) <= AiPlayerSkill[arg0].level + 0x20) {
        x = arg1->XYZ.x;
        z = arg1->XYZ.z;
        if ((arg2->pickup = ChaseNearPickup(x, z)) != NULL) {
            arg2->state = 5;
            arg2->timer = 0x96;
            ((void (*)(s32, EXTengine *, s32))ChaseAI_AngleTo)(arg0, arg2->pickup->Engine, 0);
            return;
        }
        bestd = 0x8000;
        if (arg2->timer2 <= 0) {
            arg2->state = 7;
            arg2->timer = 0x3C;
            arg2->angle = get_angle(arg1->XYZ.x, arg1->XYZ.z);
            return;
        }
        best = NULL;
        for (i = 0; i < 8; i++) {
            nas = ChaseBarrels[i];
            if (nas != NULL) {
                eng = nas->Engine;
                dx = eng->XYZ.x - x;
                dz = eng->XYZ.z - z;
                dx *= dx;
                dz *= dz;
                d = csqrt(dx + dz) >> 6;
                fux = (ChaseCoalFux *)nas->FUX;
                if (fux->owner >= 0 && fux->owner != arg0) {
                    d *= 2;
                }
                if (eng->sub->entity == 0x2017) {
                    d = d * 2 / 3;
                } else if (eng->sub->entity == 0x2016) {
                    d /= 2;
                }
                if (d < bestd) {
                    bestd = d;
                    best = nas;
                }
            }
        }
        if (best != NULL) {
            arg2->state = 6;
            arg2->target = best;
            arg2->timer = 0x3C;
            ((void (*)(s32, EXTengine *, s32))ChaseAI_AngleTo)(arg0, best->Engine, 0x100);
            fux = (ChaseCoalFux *)best->FUX;
            fux->owner = arg0;
            return;
        }
    }
    arg2->timer = 0x1E;
    r = Random(8);
    switch (r) {
    case 0:
    case 1:
    case 2:
    default:
        arg2->timer = Random(0x1E) + 0xF;
        arg2->flags = 0;
        arg2->state = 1;
        return;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    arg2->state = 4;
    arg2->x = Random(0x1100) - 0x880;
    arg2->z = Random(0x1100) - 0x880;
    arg2->angle = get_angle(arg2->x - arg1->XYZ.x, arg2->z - arg1->XYZ.z);
}


void Chase_HeroBot(s32 arg0) {
    u32 off;
    ChaseAiState *ai;
    EXTengine *eng;
    EXTengine *e;
    NAS *nas;
    NAS *best;
    NAS **pp;
    s32 i;
    s32 n;
    s32 d, dx, dz;
    s32 bestd;
    s32 x, z;
    ChaseCoalFux *fux;

    if (GamePause & 0x04000000) {
        return;
    }
    eng = Heroes[arg0].hero->Engine;
    ai = (ChaseAiState *)(ChaseHeroSlots + arg0 * 0x30);
    switch ((ai->timer >> 4) & 3) {
    case 1:
    default:
        off = 0;
        break;
    case 0:
        off = -200;
        break;
    case 2:
        off = 200;
        break;
    }
    ai->flags = 0;
    if (ai->timer2 > 0) {
        ai->timer2--;
    }
    if (--ai->timer <= 0) {
        ChaseAI_ChooseTarget(arg0, eng, ai);
    } else {
        ChaseNearPickup(eng->XYZ.x, eng->XYZ.z);
    }
    switch (ai->state) {
    case 4:
        ai->flags |= DirPadBits[((ai->angle + off) >> 9) & 7];
        if (ai->timer < 5 || ((d = ai->x - eng->XYZ.x, (u32)(d + 99) < 199) &&
                              (dz = ai->z - eng->XYZ.z, (u32)(dz + 99) < 199))) {
            ai->state = 1;
            ai->timer = 1;
            ai->flags |= 0x2000;
        }
        break;
    case 5:
        if (ai->pickup->Type != ChaseASTIndex(0x1D04) || ai->pickup->NASid == 0 || ai->flag != 0) {
            goto rethink;
        }
        ((void (*)(s32, EXTengine *, s32))ChaseAI_AngleTo)(arg0, ai->pickup->Engine, 0);
        ai->flags |= DirPadBits[((ai->angle + off) >> 9) & 7];
        break;
    case 6:
        if (ai->timer2 <= 0) {
            ChaseAI_ChooseTarget(arg0, eng, ai);
            break;
        }
        if (ai->target->Type != ChaseASTIndex(0x1D01) || ai->target->NASid == 0) {
            goto rethink;
        }
        if (Random(0x20) < AiPlayerSkill[arg0].level) {
            nas = ai->target;
            fux = (ChaseCoalFux *)nas->FUX;
            x = eng->XYZ.x;
            z = eng->XYZ.z;
            if (fux->owner == arg0) {
                fux->owner = -21;
            }
            bestd = 0x8000;
            best = NULL;
            for (i = 0; i < 8; i++) {
                nas = ChaseBarrels[i];
                if (nas != NULL) {
                    e = nas->Engine;
                    dx = e->XYZ.x - x;
                    dz = e->XYZ.z - z;
                    dx *= dx;
                    dz *= dz;
                    d = csqrt(dx + dz) >> 6;
                    fux = (ChaseCoalFux *)nas->FUX;
                    if (fux->owner >= 0 && fux->owner != arg0) {
                        d *= 2;
                    }
                    if (e->sub->entity == 0x2017) {
                        d = d * 2 / 3;
                    } else if (e->sub->entity == 0x2016) {
                        d /= 2;
                    }
                    if (d < bestd) {
                        bestd = d;
                        best = nas;
                    }
                }
            }
            if (best != NULL) {
                ai->state = 6;
                ai->target = best;
                ai->timer = 0x3C;
                ((void (*)(s32, EXTengine *, s32))ChaseAI_AngleTo)(arg0, best->Engine, 0x100);
                fux = (ChaseCoalFux *)best->FUX;
                fux->owner = arg0;
            }
        }
        ((void (*)(s32, EXTengine *, s32))ChaseAI_AngleTo)(arg0, ai->target->Engine, 0);
        ai->flags |= DirPadBits[((ai->angle + off) >> 9) & 7];
        break;
    case 7:
        if (ai->timer2 > 0) {
            goto rethink;
        }
        dx = eng->XYZ.x;
        dz = eng->XYZ.z;
        dx *= dx;
        dz *= dz;
        if ((csqrt(dx + dz) >> 6) >= ChaseArenaRadius - 200) {
            ai->state = 8;
            ai->timer = 0x3C;
            ai->angle -= 0x400;
        }
        ai->flags |= DirPadBits[(ai->angle >> 9) & 7];
        break;
    case 8:
        if (ai->timer2 > 0) {
        rethink:
            ChaseAI_ChooseTarget(arg0, eng, ai);
            break;
        }
        ai->flags |= DirPadBits[(ai->angle >> 9) & 7];
        break;
    }
    if (ai->timer & 7) {
        n = 0;
        for (i = 0; i < 8; i++) {
            nas = ChaseBarrels[i];
            if (nas != NULL) {
                e = nas->Engine;
                dx = e->XYZ.x - eng->XYZ.x;
                dz = e->XYZ.z - eng->XYZ.z;
                dx *= dx;
                dz *= dz;
                d = csqrt(dx + dz) >> 6;
                if (d < 0x380) {
                    n++;
                }
            }
        }
        if (n >= 3) {
            ai->flags |= 0x2000;
        }
    }
    switch (ai->state) {
    case 1:
    case 4:
    case 7:
        break;
    case 5:
    default:
        ai->flags |= 0x8000;
        break;
    }
    set_joypad((u8 *)&Pads[arg0], ai->flags);
}

void InitialiseCHASEFirst(void) {
    HeroHooks.HeroModels = ChaseHeroModels;
    HeroHooks.HeroBot = (void *)Chase_HeroBot;
    HeroHooks.TestRun = (void *)ChaseHeroTestRun;
    HeroHooks.InitHero = (void *)ChaseInitHero;
    HeroHooks.KeepInBounds = (void *)CHASE_KeepInBounds;
    HeroHooks.VictorySeqs = ChaseVictorySeqs;
    HeroHooks.PreMoveHero = (void *)ChasePreMoveHero;
    HeroHooks.MoveHero = (void *)ChaseMoveHero;
    HeroHooks.HeroKickCollision = (void *)ChaseHeroKickCollision;
    HeroHooks.TestJump = (void *)ChaseHeroTestJump;
    HeroHooks.TestKick = (void *)ChaseNoKick;
    HeroHooks.TestRun = (void *)ChaseHeroTestRun;
    HeroHooks.HeroExtraEffects = (void *)ChaseHero_ExtraEffects;
    HeroActDead = 0x77;
    HeroActWinner = 0x75;
    HeroActLoser = 0x74;
    HeroActHit = 0x73;
    HeroHooks.ChooseWinner = (void *)ChaseChooseWinner;
    AiBotTweakTable = ChaseAiBotTweak;
    AiHumanTweakTable = ChaseAiHumanTweak;
    AiHumanRoundTweakTable = ChaseAiHumanRoundTweak;
    AiSkillTable = GameTeams[0] != 0 ? ChaseAiSkillAlt : ChaseAiSkill;
    AiTeamSkillTable = ChaseAiTeamSkill;
}

/* Steering in the tail: `t = v` survives cse2 and combine only because v is
   set again before the loop, and the always-zero `(t * 60) & 3` is a use that
   flow counts before combine drops it. That lifts the *60 chain's local-alloc
   priority above the x chain's (they tie otherwise), so it takes v1 as in
   retail. */
void InitialiseCHASE(void) {
    s32 i;
    s32 a;
    s32 x;
    s32 v;
    s32 t;
    s32 y;
    u8 *g;
    u8 *p;
    GameState *q;
    HeroInfo *h;
    camera_info_tag *cam;
    ChaseScreen *scr;

    ChaseRoundOver = 0;
    World_floor_level = 0;
    Init_Clock(0x140, 0x1E, 0);
    if (((GameState *)GameInfo)->levelFlags & 0x40) {
        a = 0x40;
        if (((GameState *)GameInfo)->teams == 0) {
            a = 0x100;
        }
    } else {
        a = 0x130;
    }
    Set_Clock(a, 0x81C0);
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_KegObjects]);
    dbase_load_all_mesh_anims((s32 *)(((u8 *)&HeroModelBank[DB_KegObjects]) - 0xF88));
    dbase_load_all_mesh_anims((s32 *)MapScene->dbase);
    scr = (ChaseScreen *)&GameViewport;
    scr->unk10 = 0x140;
    scr->unk14 = 0x15E;
    cam = &MainCamera;
    cam->eye.x = 0;
    cam->eye.z = 0xE98;
    cam->xform.angle.vx = 0x1A8;
    cam->dist = 0x200;
    cam->at.y = -0x110;
    cam->eye.y = 0;
    cam->xform.angle.vy = 0;
    cam->xform.angle.vz = 0;
    cam->at.x = 0;
    cam->at.z = -0x770;
    for (i = 7; i >= 0; i--) {
        ChaseBarrels[i] = NULL;
    }
    ChaseArenaRadius = 0x938;
    ChaseArenaShrink = 0x684;
    ChaseBarrelCount = 0;
    for (i = 0x63F; i >= 0; i--) {
        ChaseGrid[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        ChasePickup[i] = NULL;
    }
    CreateNAS(0, 0, 0, 0x1D06, 0x11);
    i = Random(0x1000);
    ChaseGenerator = CreateNAS(i, 0, 0x700, 0x1D07, 0x11);
    ChaseGeneratorB = CreateNAS((i + 0x800) & 0xFFF, 0, 0x700, 0x1D07, 0x11);
    CreateNAS(0, 0, 0, 0x1D03, 0x11);
    g = GameInfo;
    if (((GameState *)g)->levelFlags & 0x80) {
        CreateNAS(0, 0, 0, 0x1D08, 0x11);
    }
    if (((GameState *)g)->levelFlags & 0x40) {
        for (i = 0; i < 4; i++) {
            p = GameInfo;
            h = &Heroes[i];
            v = 0x78;
            if (p[i + 0x5C] == 0) {
                v = 0x82;
                if (((GameState *)p)->teams != 0) {
                    v = 0x8C;
                }
            }
            h->hero_score_displayed = v;
            h->hero_score = v;
        }
        InitSharedScoreIcons();
        if (GameTeams[0] != 0) {
            PanelCtrl |= 0x2000;
        }
    }
    t = AiPlayerSkill[3].level * 2 + 0x44;
    v = t & 0xFFFE;
    x = v * 35 / 100;
    t = v;
    HeroHooks.RunSpeeds = ChaseRunSpeeds;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        y = t * 60 / 100 + ((t * 60) & 3);
        q = GAME;
        if (q->heroCpu[i] == 0) {
            ((ChasePct *)ChaseRunSpeeds)[h->hero_name].unk0 = 0x23;
            ((ChasePct *)ChaseRunSpeeds)[h->hero_name].unk2 = 0x3C;
        } else {
            ((ChasePct *)ChaseRunSpeeds)[h->hero_name].unk0 = x;
            ((ChasePct *)ChaseRunSpeeds)[h->hero_name].unk2 = y;
        }
    }
}

void UpdateCHASE(void) {
    s32 i;
    s16 c;
    s32 n;
    u8 *h;
    NAS **p;

    c = 0;
    i = 0;
    p = ChaseBarrels;
loop:
    if (i < 8) {
        if (*p != NULL) {
            c++;
        }
        p++;
        i++;
        goto loop;
    }
    if (HeroModelBank[DB_KegObjects].data != 0) {
        texture_update_anims(&HeroModelBank[DB_KegObjects].textures);
    }
    n = 3;
    do {
        n--;
    } while (n >= 0);
    h = &HeroModelBank[DB_KegObjects];
    texture_update_anims(h + 0x10);
    ChaseUpdateStub(ChaseUpdateState);
}

void FinishChaseLevel(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                HeroTakeHit(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

s32 CHASE_KeepInBounds(s32 *p) {
    s32 d;
    s32 a;
    if (p[4] < -ChaseArenaShrink || ChaseArenaShrink < p[4] || p[6] < -ChaseArenaShrink || ChaseArenaShrink < p[6]) {
        d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - ChaseArenaRadius;
        if (d > 0) {
            a = get_angle(p[4], p[6]);
            p[4] -= (d * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
            p[6] -= (d * rcossin_tbl[a & 0xFFF].unk2) >> 12;
            return 0;
        }
    }
    return 1;
}

void ChaseInitHero(s32 arg0) {
    HeroInfo *b;
    HeroInfo *h;
    EXTengine *eng;
    subengine *sub;
    s32 *st;
    u8 *g;
    ChaseAiState *p;
    NAS *nas;

    Pads[arg0].dir = 0;
    Pads[arg0].dirPressed = 0;
    b = Heroes;
    h = b + arg0;
    if (h->hero_name == 9) {
        return;
    }
    g = &GameInfo[arg0];
    if (g[0x50] == 9) {
        return;
    }
    SetHeroAction(arg0, 0x71);
    eng = h->hero_eng;
    eng->xform.order = 1;
    eng->xform.rot.vy = 0x400;
    st = ChaseHeroState[arg0];
    eng_add_subs(eng, 1);
    sub = eng->sub;
    sub->xform.order = 1;
    sub->entity = ChaseHeroShadowModels[h->hero_name];
    FetchDefaultColBox(eng, 0x50);
    sub->entity = ChaseHeroShadowSizes[h->hero_name];
    h->hero_score = 0;
    st[0] = 1;
    st[2] = 0xB4;
    ChaseHeroCoal[arg0] = NULL;
    ChaseFlameState[arg0] = 0;
    ChaseFlameSfxHandle[arg0] = 0;
    nas = CreateNAS(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, 0x1D09, 0x11);
    nas->UserInfo.user2 = arg0;
    ChaseFlameTimer[arg0] = nas->Engine;
    ChaseFlameObj[arg0] = 0;
    if (g[0x5C] != 0) {
        p = (ChaseAiState *)((arg0 * 0x30) + ChaseHeroSlots);
        p->state = 1;
        p->timer = 1;
        p->flags = 0;
        p->target = 0;
        p->pickup = 0;
        p->timer2 = 0;
        p->flag = 0;
        p->reach = 0x8000;
        p->reach2 = 0x8000;
    }
}

void ChasePreMoveHero(s32 arg0) {
    HeroInfo *hb;
    HeroInfo *hb2;
    PadState *pb;
    HeroInfo *h;
    HeroInfo *h2;
    EXTheromove *m;
    PadState *pad;
    u16 *row;
    u16 speed;
    u16 rw;
    s32 vc;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    if (h->hero_frozen != 0) {
        Pads[arg0].dir = 0;
        Pads[arg0].dirPressed = 0;
    }
    pb = Pads;
    pad = pb + arg0;
    if ((pad->flags & 1) || GameHeroCpu != NULL) {
        if (pad->dir & 0xF0) {
            m->forward_angle = pad->angle.lo + 0x800;
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
                if (h2->hero_pu == 0x13) {
                    speed += (row + h2->hero_name * 3)[2];
                }
                vc = m->velocity_calc;
                if (vc != 1) {
                    if (vc != 0) {
                        if (vc < 4) {
                            m->x_max = (speed * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                            m->z_max = (speed * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
                        }
                    }
                }
                m->forward_speed = speed;
            }
        } else {
            m->x_max = 0;
            m->z_max = 0;
            m->facing_angle = m->forward_angle = m->forward_angle & 0xFFF;
        }
    }
    m->forward_angle &= 0xFFF;
    if (abs(angle_diff(m->forward_angle, m->facing_angle) & 0xFFFF) > 0x110) {
        m->facing_angle = (m->forward_angle + 0x100) & 0xE00;
    }
}

void ChaseMoveHero(s32 arg0) {
    HeroInfo *h;
    EXTengine *e;
    EXTengine *e0;
    EXTengine *ne;
    s32 dx;
    s32 dz;
    EXTheromove *m;
    s32 step;
    HeroInfo *base;
    s32 *st;
    NAS *t;
    SLONG_XYZ *v;
    u8 *anim;
    NAS *nas;
    EXTscriptdata *ext;

    base = Heroes;
    h = base + arg0;
    e0 = h->hero_eng;
    h->hero_immunity = 0;
    m = h->hero_move;
    e = e0;
    if (Pads[arg0].dir & 0x8000) {
        m->run_walk = 0;
    } else {
        m->run_walk = 1;
    }
    ((void (*)(s32))HeroHooks.HeroFacing)(arg0);
    HeroApplyJump(arg0);
    switch (m->velocity_calc) {
    case 2:
        m->x_vel += m->x_acc - m->x_dec;
        if (m->x_vel < 0) {
            m->x_vel = 0;
        }
        if (m->x_vel > m->x_max) {
            m->x_vel = m->x_max;
        }
        if (m->x_dir != 0) {
            e->XYZ.x -= m->x_vel;
        } else {
            e->XYZ.x += m->x_vel;
        }
        m->z_vel += m->z_acc - m->z_dec;
        if (m->z_vel < 0) {
            m->z_vel = 0;
        }
        if (m->z_vel > m->z_max) {
            m->z_vel = m->z_max;
        }
        if (m->z_dir != 0) {
            e->XYZ.z -= m->z_vel;
        } else {
            e->XYZ.z += m->z_vel;
        }
        break;
    case 3:
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
        e->XYZ.z += m->z_vel;
        e->XYZ.x += m->x_vel;
        break;
    case 9:
        break;
    case 1:
    default:
        e->XYZ.x -= (m->forward_speed * rcossin_tbl[(m->forward_angle - 0x400) & 0xFFF].unk2) >> 12;
        e->XYZ.z -= (m->forward_speed * rcossin_tbl[m->forward_angle & 0xFFF].unk2) >> 12;
        if (m->reactive_speed != 0) {
            e->XYZ.x -= (m->reactive_speed * rcossin_tbl[(m->reactive_angle - 0x400) & 0xFFF].unk2) >> 12;
            e->XYZ.z -= (m->reactive_speed * rcossin_tbl[m->reactive_angle & 0xFFF].unk2) >> 12;
            m->reactive_speed -= 4;
            if (m->reactive_speed < 0) {
                m->reactive_speed = 0;
            }
        }
        break;
    }
    if ((Pads[arg0].dir & 0x8000) && e0->XYZ.y >= -0x31) {
        ChaseHeroSpeed[arg0] = 0xF;
        e0 = Heroes[arg0].hero_eng;
        st = ChaseHeroState[arg0];
        t = NULL;
        if (ChaseHeroCoal[arg0] == NULL
            || (ne = ChaseHeroCoal[arg0]->Engine, dx = e0->XYZ.x - ne->XYZ.x, dx = dx * dx,
                dz = e0->XYZ.z - ne->XYZ.z, dz = dz * dz, dx + dz >= 0x400)) {
            t = ChaseSpawnCoal(arg0);
        }
        if (t != NULL) {
            ChaseHeroCoal[arg0] = t;
        }
    } else {
        if (--ChaseHeroSpeed[arg0] <= 0) {
            ChaseFlameObj[arg0] = 0;
        }
    }
    if ((Pads[arg0].dir & 0x2A00) && e0->XYZ.y >= -0x31 && ChaseHeroState[arg0][0x14] != 0) {
        v = &e0->XYZ;
        PlaySfx(0x43A, NULL, v, 0x1000, 0x1E00, 0);
        ChaseHidePowerup(arg0);
        CoalChainHit(e0->XYZ.x, e0->XYZ.z, arg0, 0, 0x380, 0);
        ChaseHitHeroesNear(e0->XYZ.x, e0->XYZ.z, arg0, 0x380);
        anim = &HeroModelBank[DB_KegObjects];
        nas = CreateNAS(e0->XYZ.x, v->y, v->z, 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 3;
                ext->speed = 0x100;
                if (anim != NULL) {
                    ext->flags = 5;
                    nas->Engine->sub->u6C.dbase = anim;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                nas->InitDelete(nas);
            }
        }
    }
}

s32 ChaseHeroTestDrop(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_move->jump_max != 0) {
        if (ApplyContourMoved(h->hero_eng) != 0) {
            h->hero_move->jump_max = 0;
            h->hero_sub->XYZ.y = 0;
        } else {
            if (h->hero_ctrl & 8) {
                return 0;
            }
            if (World_floor_level < h->hero_eng->XYZ.y + h->hero_sub->XYZ.y) {
                if (ContourHeight == 0x7FFFFFFF) {
                    SetHeroAction(arg0, 0x79);
                    return 1;
                }
                h->hero_eng->XYZ.y = World_floor_level;
                h->hero_sub->XYZ.y = 0;
            }
        }
    } else {
        HeroMovedDist(arg0);
    }
    return 0;
}

s32 ChaseHeroKickCollision(s32 arg0) {
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

static inline s32 Chk5CD0_A(s32 arg0) {
    s32 ret;
    u16 st;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk5CD0_B(s32 arg0) {
    s32 ret;

    if (Pads[arg0].dirPressed & 0x4000) {
        SetHeroAction(arg0, 0x78);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk5CD0_C(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_move->forward_speed != 0) {
        SetHeroAction(arg0, 0x72);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void ChaseHeroIdle(s32 arg0) {
    if (!Chk5CD0_A(arg0)) {
        if (!Chk5CD0_B(arg0)) {
            if (!Chk5CD0_C(arg0)) {
                if (Heroes[arg0].hero_move->velocity_calc == 3) {
                    SetHeroSeq(arg0, 0x61);
                }
                ProcessHeroAnim(arg0);
                if (VelocityCalc < 4) {
                    if (VelocityCalc >= 2) {
                        ChaseHeroKickCollision(arg0);
                    }
                }
                ChaseHeroTestDrop(arg0);
                Heroes[arg0].hero_hit = 0;
            }
        }
    }
}

static inline s32 Chk5E84_C(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction(arg0, 0x71);
        return 1;
    }
    return 0;
}

void ChaseHeroRun(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *e;

    if (!Chk5CD0_A(arg0)) {
        if (!Chk5CD0_B(arg0)) {
            if (!Chk5E84_C(arg0)) {
                base = Heroes;
                h = base + arg0;
                h->hero_move->velocity_enable = 1;
                ChaseHeroKickCollision(arg0);
                switch (ProcessHeroAnim(arg0)) {
                case 8:
                    e = h->hero_eng;
                    if (h->hero_pu == 0x15) {
                        PlaySfx(0x41E, NULL, NULL, 0x1000, 0x1E00, 0);
                        break;
                    }
                    switch (h->hero_name) {
                    case 1: PlaySfx(0xA, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 2: PlaySfx(0x3C, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 3: PlaySfx(0x6E, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 4: PlaySfx(0xA0, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 7: PlaySfx(0x104, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 8: PlaySfx(0xD2, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 5: PlaySfx(0x136, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    case 6: PlaySfx(0x168, NULL, &e->XYZ, 0x1000, 0x1E00, 0); break;
                    }
                    break;
                case 9:
                    if (h->hero_pu == 0x15) {
                        break;
                    }
                    switch (h->hero_name) {
                    case 1: PlaySfx(0xB, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 2: PlaySfx(0x3D, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 3: PlaySfx(0x6F, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 4: PlaySfx(0xA1, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 7: PlaySfx(0x105, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 8: PlaySfx(0xD3, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 5: PlaySfx(0x137, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 6: PlaySfx(0x169, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    }
                    break;
                }
                ChaseHeroTestDrop(arg0);
            }
        }
    }
}

void SetChaseHeroJump(s32 arg0) {
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
    SetHeroSeq(arg0, 0x65);
    idx = 0;
    if (GAME->heroCpu[arg0] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = HeroHooks.JumpEnts[idx].vel;
    h->hero_move->jump_max = HeroHooks.JumpEnts[idx].max;
    h->hero_move->jump_acc = HeroHooks.JumpEnts[idx].acc;
    h->hero_move->jump_dec = HeroHooks.JumpEnts[idx].dec;
    h->hero_move->jump_dir = -1;
}

void SetChaseHeroFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;
    EXTengine *p;

    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq(arg0, 0x66);
}

void ChaseHeroFall(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;

    if (ApplyContourMoved(h->hero_eng) != 0) {
        SetHeroAction(arg0, 0x7B);
    } else if (!(h->hero_ctrl & 8)) {
        e = h->hero_eng;
        if (World_floor_level < e->XYZ.y + h->hero_sub->XYZ.y) {
            if (ContourHeight == 0x7FFFFFFF) {
                SetHeroAction(arg0, 0x79);
            } else {
                e->XYZ.y = World_floor_level;
                h->hero_sub->XYZ.y = 0;
            }
        }
    }
    ChaseHeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SetChaseHeroDrop(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *p;
    EXTengine *e;

    h = base + arg0;
    h->hero_eng->XYZ.x &= ~0x1FF;
    h->hero_eng->XYZ.x += 0x100;
    h->hero_eng->XYZ.z &= ~0x1FF;
    h->hero_eng->XYZ.z += 0x100;
    h->hero_move->velocity_enable = 0;
    h->hero_move->forward_speed = 0;
    h->hero_move->jump_ctrl |= 0x10;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    h->hero_status |= 0x800;
    if (h->hero_last_mode != 0x73) {
        SetHeroSeq(arg0, 0x66);
    }
    PlaySfx(0x410, NULL, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
}

void SetChaseHeroLand(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *o;
    EXTengine *t;
    EXTobject *x;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    h->hero_move->jump_max = 0;
    h->hero_sub->XYZ.y = 0;
    SetHeroSeq(arg0, 0x67);
    if (ContourPlatform != NULL) {
        e = h->hero_eng;
        o = ContourPlatform->Engine;
        t = e->platform;
        if (t != NULL) {
            t->platform = NULL;
            e->platform = NULL;
        }
        t = o->platform;
        if (t != NULL) {
            t->platform = NULL;
            o->platform = NULL;
        }
        e->platform = o;
        o->platform = e;
        x = FindExtension(ContourPlatform, 0x1000);
        if (x != NULL && (x->can_do & 8)) {
            x->will_do |= 8;
        }
    }
}

static inline s32 Chk66A4_A(s32 arg0) {
    s32 ret;
    u16 st;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_B(s32 arg0) {
    s32 ret;

    if (Pads[arg0].dirPressed & 0x4000) {
        SetHeroAction(arg0, 0x78);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_C(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_move->forward_speed != 0) {
        SetHeroAction(arg0, 0x72);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void ChaseHeroLand(s32 arg0) {
    if (!Chk66A4_A(arg0)) {
        if (!Chk66A4_B(arg0)) {
            if (!Chk66A4_C(arg0)) {
                ChaseHeroKickCollision(arg0);
                ProcessHeroAnim(arg0);
                ChaseHeroTestDrop(arg0);
            }
        }
    }
}

void SetChaseHeroHit(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    ((void (*)())pad_rumble_big)(arg0, 0xA, 0xF0);  /* matching: Chase called it unprototyped, so 0xF0 goes as an int, not the s8 -16 */
    ChaseHidePowerup(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq(arg0, 0x68);
    PlaySfx(ChaseHitSfx[h->hero_name - 1][Random(4)], 0, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
}

void ChaseHeroHit(s32 arg0) {
    GameState *g;

    if (ProcessHeroAnim(arg0) == 8 && ((g = (GameState *)GameInfo), (g->levelFlags & 0x80)) && g->heroCpu[arg0] == 0 &&
        !(g->cheats & 2)) {
        HeroInfo *base = Heroes;
        HeroInfo *h = &base[arg0];

        h->hero_immunity = 0;
        h->hero_hit += 0x28;
        SetHeroSeq(arg0, 0x61);
        if (ChaseHeroState[arg0][0] == 0) {
            SetHeroAction(arg0, 0x76);
        } else {
            SetHeroAction(arg0, 0x71);
        }
    }
    ChaseHeroTestDrop(arg0);
}

void SetChaseHeroDie(s32 arg0) {
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
    h->hero_sub->XYZ.y = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x61);
    h->hero_hit = 0x3C;
    h->hero_status |= 0x2000;
}

void SetChaseHeroDead(s32 arg0) {
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
    ChaseHeroState[arg0][0] = 0;
    HeroBecomeAngel(arg0);
}

void ChaseHeroDead(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction(arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction(arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->u6C.dbase = (&HeroModelBank[HeroHooks.HeroModels[*(volatile u16 *)&h->hero_name]]);
        } else {
            h->hero_sub->u6C.dbase = 0;
        }
    } else {
        HeroGhost(arg0);
    }
}

void SetChaseHeroWinner(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

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
        HeroInfo *b2 = Heroes;
        HeroInfo *g = b2 + arg0;
        g->hero_sub->XYZ.y = 0;
        g->hero_pu = 0;
        g->hero_pu_timer = 0;
    }
    RequestHeroAnim(arg0, 0);
    SetHeroSeq(arg0, 0x6B);
    StartHeroVictory(arg0);
    CameraMode = 1;
    ChaseRoundOver = 1;
}

void ChaseHeroWinner(s32 arg0) {
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

void SetChaseHeroLose(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

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
    h->hero_sub->u6C.dbase = &HeroModelBank[HeroHooks.HeroModels[h->hero_name]];
    h->hero_eng->XYZ.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))ApplyContour)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->XYZ.y -= dy;
    } else {
        h->hero_eng->XYZ.y = World_floor_level;
    }
    {
        HeroInfo *b2 = Heroes;
        HeroInfo *g = b2 + arg0;
        SLONG_XYZ *v;
        void *anim;
        NAS *nas;
        EXTscriptdata *ext;

        g->hero_sub->XYZ.y = 0;
        g->hero_pu = 0;
        g->hero_pu_timer = 0;
        SetHeroSeq(arg0, 0x61);
        v = &g->hero_eng->XYZ;
        anim = &HeroModelBank[DB_KegObjects];
        nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 0;
                ext->speed = 0x100;
                if (anim != NULL) {
                    ext->flags = 5;
                    nas->Engine->sub->u6C.dbase = anim;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                nas->InitDelete(nas);
            }
        }
    }
    {
        HeroInfo *b3 = Heroes;
        HeroInfo *k = b3 + arg0;
        EXTengine *eng = k->hero_eng;

        eng->xform.hasScale = 1;
        eng->xform.scale.vx = 0x1000;
        eng->xform.scale.vy = 0x1000;
        eng->xform.scale.vz = 0x1000;
        eng->Engine->sub->flags = 0;
        RemoveCamera(k->hero);
    }
}

void ChaseHeroLose(s32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
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
    ProcessHeroAnim(arg0);
    eng = Heroes[arg0].hero_eng;
    eng->xform.hasScale = 1;
    if (eng->xform.scale.vx > 0x12C) {
        eng->xform.scale.vx -= 0x12C;
        eng->xform.scale.vy -= 0x12C;
        eng->xform.scale.vz -= 0x12C;
    } else {
        eng->xform.scale.vx = 3;
        eng->xform.scale.vy = 3;
        eng->xform.scale.vz = 3;
    }
}

static inline s32 BarrelNearHero(s32 x, s32 z, s32 r) {
    s32 i;
    s32 d;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name == 9) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        d = x - Heroes[i].hero_eng->XYZ.x;
        if (d >= -r) {
            if (d <= r) {
                d = z - Heroes[i].hero_eng->XYZ.z;
                if (d > -r) {
                    if (d < r) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

void PlotBarrelGenerator(NAS *nas) {
    s32 i;
    s32 n;
    s32 x;
    s32 z;

    if (ChaseRoundOver != 0) {
        KillNAS(nas);
        return;
    }
    if (nas->UserInfo.flags != 0) {
        nas->UserInfo.flags--;
        return;
    }
    n = 0;
    for (i = 0; i < 8; i++) {
        if (ChaseBarrels[i] != NULL) {
            n++;
        }
    }
    nas->UserInfo.flags = n * 5 + 1;
    if (n >= 8) {
        return;
    }
again:
    do {
        x = Random(8) - 4;
        x <<= 9;
        z = Random(8) - 4;
        z <<= 9;
        if ((csqrt(x * x + z * z) >> 6) > 0x800) {
            goto again;
        }
    } while (BarrelNearHero(x, z, 0x200) >= 0);
    for (i = 0; i < 8; i++) {
        if (ChaseBarrels[i] != NULL && ChaseBarrels[i]->Engine->XYZ.x == x &&
            ChaseBarrels[i]->Engine->XYZ.z == z) {
            goto again;
        }
    }
    CreateNAS(x, 0, z, 0x1D01, 0x11);
}

void InitBarrelCH(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;
    s32 i;
    s32 n;

    ext = AddExt(nas, sizeof(CoalFlameExt), 0x10000001);
    nas->FUX = &ext->ext;
    EXT_W(ext, 0x30 / 4) = 1;
    EXT_W(ext, 0x40 / 4) = -0x17;
    ext->unk1C = 0x3E8;
    eng = nas->Engine;
    sub = eng->sub;
    eng->xform.scale.vx = 0x3E8;
    eng->xform.scale.vy = 0x3E8;
    eng->xform.scale.vz = 0x3E8;
    eng->xform.hasScale = 1;
    eng->XYZ.y = 0;
    sub->u6C.dbase = &HeroModelBank[DB_KegObjects];
    n = ++ChaseBarrelCount;
    if ((n & 0x1F) == 0) {
        sub->entity = 0x2016;
    } else if ((n & 7) == 0) {
        sub->entity = 0x2017;
    } else {
        sub->entity = 0x2001;
    }
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (ChaseBarrels[i] == NULL) {
                ChaseBarrels[i] = nas;
                break;
            }
        }
    }
}

void PlotBarrelCH(NAS *nas) {
    EXTengine *eng;
    BarrelCHExt *fux;
    EXTengine *he;
    s32 i;
    s32 j;
    s32 bx, bz;
    s32 dx, dz;
    s32 d;
    s32 a;
    SLONG_XYZ *v;
    void *anim;
    NAS *n;
    EXTscriptdata *ext;

    i = 0;
    eng = nas->Engine;
    fux = (BarrelCHExt *)nas->FUX;
    bx = eng->XYZ.x;
    bz = eng->XYZ.z;
    do {
        he = Heroes[i].hero_eng;
        dx = he->XYZ.x - bx;
        dz = he->XYZ.z - bz;
        if ((u32)(dx + 0xFF) < 0x1FF && dz >= -0xFF && dz < 0x100) {
            d = (csqrt(dx * dx + dz * dz) >> 6) - 0x70;
            if (d < 0x70) {
                a = get_angle(dx, dz);
                he->XYZ.x += (d * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
                he->XYZ.z += (d * rcossin_tbl[a & 0xFFF].unk2) >> 12;
            }
        }
        i++;
    } while (i < 4);
    if (fux->growing == 1) {
        fux->scale += 0x3E8;
        if (fux->scale >= 0x1770) {
            fux->scale = 0x1770;
            fux->growing = 0;
        }
        eng->xform.scale.vx = fux->scale;
        eng->xform.scale.vy = fux->scale;
        eng->xform.scale.vz = fux->scale;
        eng->xform.dirty = 1;
        return;
    }
    if (nas->UserInfo.status != 0) {
        v = &eng->XYZ;
        PlaySfx(ChaseBarrelSfx[Random(4)], 0, v, 0x1000, 0x1E00, 0);
        anim = &HeroModelBank[DB_KegObjects];
        n = CreateNAS(eng->XYZ.x, v->y, v->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->scriptnumber = 0;
                ext->speed = 0x100;
                if (anim != NULL) {
                    ext->flags = 5;
                    n->Engine->sub->u6C.dbase = anim;
                } else {
                    ext->flags = 1;
                }
                ext->maxloops = 0;
                n->InitDelete(n);
            }
        }
        for (j = 0; j < 8; j++) {
            if (ChaseBarrels[j] == nas) {
                ChaseBarrels[j] = NULL;
                break;
            }
        }
        KillNAS(nas);
    }
}

NAS *ChaseSpawnCoal(s32 arg0) {
    u8 *g = GameInfo + arg0;
    EXTengine *he = Heroes[arg0].hero_eng;
    s32 lv = g[0x50];
    ChaseSpawnOfs *o = &ChaseSpawnOffsets[--lv];
    s32 r = o->r;
    s32 a = (he->xform.rot.vy + o->ang) & 0xFFF;
    s32 dx = (r * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
    s32 dz = (r * rcossin_tbl[a].unk2) >> 12;
    s32 x = he->XYZ.x + dx;
    s32 y = -o->y;
    s32 z;
    NAS *old;
    NAS *nas;
    EXTengine *e;

    if (x < -0x9FF) {
        x = -0x9FF;
    }
    if (x >= 0xA00) {
        x = 0x9FF;
    }
    z = he->XYZ.z + dz;
    if (z < -0x9FF) {
        z = -0x9FF;
    }
    if (z >= 0xA00) {
        z = 0x9FF;
    }
    nas = CreateNAS(x, he->XYZ.y + y, z, 0x1D02, 0x11);
    if (nas != NULL) {
        {
            CoalPosInner *p = ((CoalGridObj *)nas)->pos;
            s32 gx = p->x;
            s32 gz = p->z;
            s32 idx;

            gx += 0xA00;
            gz += 0xA00;
            gx >>= 7;
            gz >>= 7;
            idx = gz * 40 + gx;
            old = ChaseGrid[idx];
            ChaseGrid[idx] = nas;
            if (old != 0) {
                KillNAS(old);
            }
        }
        if (g[0x5C] != 0) {
            nas->UserInfo.status = (nas->UserInfo.status * AiPlayerSkill[arg0].level) / 16;
        }
        e = nas->Engine;
        e->xform.rot.vy = he->xform.rot.vy;
        e->xform.rot.vx = 0x400;
        e->xform.dirty = 1;
        EXT_W(nas->FUX, 0x2C / 4) = arg0;
    }
    return nas;
}

void CoalFlameSpread(EXTengine *eng, CoalFlameFux *fux, s32 id) {
    NAS **grid;
    s32 cx;
    s32 cz;
    s32 i;
    CircleFireOfs *ofs;

    if (ChaseRoundOver != 0) {
        return;
    }
    i = 0;
    grid = ChaseGrid;
    ofs = CircleFireOffsets;
    cx = eng->XYZ.x;
    cz = eng->XYZ.z;
    for (;;) {
        NAS *cell;
        CoalFlamePos *pos;
        CoalFlameFux *cf;
        NAS *fire;
        CoalFlameFux *ff;
        s32 x;
        s32 z;
        s32 s;

        if (i >= 8) {
            break;
        }
        x = cx + ofs->x;
        if (x >= -0x9FF) if (x < 0xA00) {
            z = cz + ofs->z;
            if (z >= -0x9FF) if (z < 0xA00) {
                cell = (NAS *)CircleFireGridCell(grid, x, z);
                if (cell != NULL) {
                    pos = cell->Engine;
                    cf = cell->FUX;
                    if (fux->unk1C < 0) {
                        fux->unk1C = 0;
                        fux->unk20 = pos->x;
                        fux->unk24 = pos->z;
                        fux->unk2C = cf->unk2C;
                        s = ((cell->UserInfo.status + 0xF) << 12) / 60;
                        if (s > 0x1000) {
                            s = 0x1000;
                        } else if (s < 0x258) {
                            s = 0x258;
                        }
                        eng->xform.scale.vx = s;
                        eng->xform.scale.vy = s;
                        eng->xform.scale.vz = s;
                        eng->xform.dirty = 1;
                        grid[CIRCLEFIRE_CELL(x, z)] = 0;
                        KillNAS((NAS *)cell);
                    } else {
                        fire = CreateNAS(cx, 0, cz, 0x1D05, 0x11);
                        if (fire != NULL) {
                            ff = (CoalFlameFux *)fire->FUX;
                            ff->unk2C = cf->unk2C;
                            ff->unk1C = -7;
                            ff->unk20 = pos->x;
                            ff->unk24 = pos->z;
                            if (cf->unk2C == id) {
                                ff->unk28 = fux->unk28;
                            }
                        }
                    }
                }
            }
        }
        ofs++;
        i++;
    }
}

s32 CoalChainHit(s32 x, s32 z, s32 id, s32 cur, s32 range, s32 delay) {
    NAS *nas;
    EXTengine *eng;
    HeroInfo *h;
    s32 total;
    s32 i;
    s32 d;
    s32 pts;
    u16 kind;

    total = 0;
    for (i = 0; i < 8; i++) {
        nas = (NAS *)ChaseBarrels[i];
        if (nas == NULL || nas->UserInfo.status == 1) {
            continue;
        }
        eng = nas->Engine;
        d = eng->XYZ.x - x;
        if (-range < d && d < range) {
            d = eng->XYZ.z - z;
            if (-range < d && d < range && ((CoalChainFux *)nas->FUX)->unk30 != 1) {
                nas->UserInfo.status = 1;
                kind = eng->sub->entity;
                total += 2;
                if (kind == 0x2016) {
                    pts = 20;
                    PlaySfx(0x755, NULL, (SLONG_XYZ *)&eng->XYZ.x, 0x1000, 0x1E00, 0);
                } else if (kind == 0x2017) {
                    pts = 10;
                    PlaySfx(0x754, NULL, (SLONG_XYZ *)&eng->XYZ.x, 0x1000, 0x1E00, 0);
                } else {
                    pts = cur + total;
                    if (pts > 10) {
                        pts = 10;
                    }
                    PlaySfx(ChaseChainSfx[id], NULL, (SLONG_XYZ *)&eng->XYZ.x, 0x1000, 0x1E00, 0);
                }
                if (((GameState *)GameInfo)->levelFlags & 0x40) {
                    if (((GameState *)GameInfo)->teams != 0) {
                        h = &ChaseHeroes()[id];
                        if (h->hero_score >= pts) {
                            h->hero_score -= pts;
                        } else {
                            Heroes[h->hero_teammate].hero_score -= pts - (u16)h->hero_score;
                            h->hero_score = 0;
                        }
                    } else {
                        ChaseHeroes()[id].hero_score -= pts;
                    }
                } else {
                    ChaseHeroes()[id].hero_score += pts;
                }
                CreateFloatyScore(eng->XYZ.x, -300, eng->XYZ.z, pts, delay);
                delay += 10;
                total += CoalChainHit(eng->XYZ.x, eng->XYZ.z, id, pts, 0x120, delay);
            }
        }
    }
    return total;
}

void PlotCoalFlame(NAS *nas) {
    CoalFlameFux *fux;
    EXTengine *eng;
    s32 t;

    fux = (CoalFlameFux *)nas->FUX;
    eng = (EXTengine *)nas->Engine;
    ((CoalPlayerRec *)ChaseHeroSlots)[fux->unk2C].unk20 = 2;
    if (fux->unk1C == 0) {
        if (eng->XYZ.x < fux->unk20) {
            eng->XYZ.x += 0x14;
        } else if (eng->XYZ.x > fux->unk20) {
            eng->XYZ.x -= 0x14;
        }
        if (eng->XYZ.z < fux->unk24) {
            eng->XYZ.z += 0x14;
        } else if (eng->XYZ.z > fux->unk24) {
            eng->XYZ.z -= 0x14;
        }
        if ((u32)(fux->unk20 - eng->XYZ.x + 0x27) < 0x4F
            && (u32)((t = fux->unk24) - eng->XYZ.z + 0x27) < 0x4F) {
            eng->XYZ.x = fux->unk20;
            eng->XYZ.z = fux->unk24;
            fux->unk1C = -7;
        }
    } else if ((t = --nas->UserInfo.user2) <= 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        KillNAS(nas);
        return;
    }
    if (fux->unk1C < 0) {
        CoalFlameSpread(eng, fux, fux->unk2C);
    }
    ChaseFlameObj[fux->unk2C] += CoalChainHit(eng->XYZ.x, eng->XYZ.z, fux->unk2C,
                                            ChaseFlameObj[fux->unk2C], 0x12C, 0);
    if (ChaseFlameObj[fux->unk2C] > 10) {
        ChaseFlameObj[fux->unk2C] = 10;
    }
    ((EXTengine *)ChaseFlameTimer[fux->unk2C])->XYZ.x = eng->XYZ.x;
    ((EXTengine *)ChaseFlameTimer[fux->unk2C])->XYZ.z = eng->XYZ.z;
    ChaseFlameState[fux->unk2C] = 10;
}

void PlotCircleFire(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 cx;
    s32 cz;
    s32 i;
    NAS **grid;

    eng = nas->Engine;
    sub = eng->sub;
    if (++sub->entity >= 0x4014) {
        sub->entity = 0x4000;
    }
    nas->UserInfo.status = (nas->UserInfo.status + 0xC) & 0xFFF;
    eng->XYZ.x = (rcossin_tbl[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 4;
    eng->XYZ.z = (rcossin_tbl[nas->UserInfo.status & 0xFFF].unk2 * 9) >> 4;
    cx = eng->XYZ.x;
    eng->xform.rot.vy = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->xform.dirty = 1;
    cz = eng->XYZ.z;
    i = 0;
    grid = ChaseGrid;
    for (;;) {
        NAS *cell;
        NAS *fire;
        CircleFireFux *fux;
        s32 x;
        s32 z;

        if (i >= 8) {
            break;
        }
        x = cx + CircleFireOffsets[i].x;
        if (x >= -0x9FF) if (x < 0xA00) {
            z = cz + CircleFireOffsets[i].z;
            if (z >= -0x9FF) if (z < 0xA00) {
                cell = CircleFireGridCell(grid, x, z);
                if (cell != NULL) {
                    fux = (CircleFireFux *)cell->FUX;
                    fire = CreateNAS(x, 0, z, 0x1D05, 0x11);
                    if (fire != NULL) {
                        ((CircleFireFux *)fire->FUX)->unk2C = fux->unk2C;
                    }
                    fire = CreateNAS(x, 0, z, 0x1D05, 0x11);
                    if (fire != NULL) {
                        ((CircleFireFux *)fire->FUX)->unk2C = fux->unk2C;
                    }
                    grid[CIRCLEFIRE_CELL(x, z)] = 0;
                    KillNAS(cell);
                }
            }
        }
        i++;
    }
}

void PlotEgg(NAS *nas) {
    EXTengine *eng;
    EggExt *fux;
    subengine *sub;
    SLONG_XYZ *pos;
    SLONG_XYZ *at;
    NAS *n;
    EggPuffExt *ext;
    obj_dbase *p;
    s32 i;
    s32 skip;
    s32 lim;
    s32 x;
    s32 z;
    HeroInfo *h;

    eng = nas->Engine;
    sub = eng->sub;
    fux = (EggExt *)nas->FUX;
    if (Clock_Time == 0 || (GameExitFlags & 0x801)) {
        PlaySfx(0x75A, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
        pos = &eng->XYZ;
        p = MapScene->dbase;
        n = CreateNAS(eng->XYZ.x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0;
                ext->unk18 = 0x100;
                if (p != 0) {
                    ext->unk20 = 5;
                    n->Engine->sub->u6C.dbase = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                n->InitDelete(n);
            }
        }
        KillNAS(nas);
        return;
    }
    switch (fux->state) {
    case 0x26:
    default:
        if (--nas->UserInfo.user2 > 0) {
            return;
        }
        if (nas->UserInfo.user2 == 0) {
            PlaySfx(0x756, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
        }
        eng->XYZ.x += fux->xvel;
        fux->y += 0xC;
        eng->XYZ.y += fux->y;
        if (++sub->entity == 0x4091) {
            fux->state = 0x29;
            sub->entity = 0x4000;
            eng->XYZ.x = 0;
            eng->XYZ.y = -0x12C;
        }
        break;
    case 0x29:
        nas->UserInfo.status = (nas->UserInfo.status - 0xA) & 0xFFF;
        eng->XYZ.x = (rcossin_tbl[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 0x53) >> 7;
        eng->XYZ.z = (rcossin_tbl[nas->UserInfo.status & 0xFFF].unk2 * 0x53) >> 7;
        eng->xform.rot.vy = (nas->UserInfo.status + 0x400) & 0xFFF;
        eng->xform.dirty = 1;
        if (eng->XYZ.z > 0) {
            sub->otz = 0x100;
        } else {
            sub->otz = -0x100;
        }
        if (++sub->entity == 0x4006) {
            PlaySfx(0x757, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
        }
        if (sub->entity == 0x400F) {
            PlaySfx(0x758, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
        }
        if (sub->entity == 0x4012) {
            sub->entity = 0x4000;
            i = BarrelNearHero(eng->XYZ.x, eng->XYZ.z, 0x600);
            if (i >= 0 && GAME->heroCpu[i] == 0) {
                PlaySfx(0x759, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
                fux->angle = get_angle(Heroes[i].hero_eng->XYZ.x - eng->XYZ.x,
                                       Heroes[i].hero_eng->XYZ.z - eng->XYZ.z);
                fux->state = 0x27;
                fux->y = -0x64;
                sub->entity = 0x4080;
                eng->xform.dirty = 1;
            }
        }
        break;
    case 0x27:
        if (++sub->entity == 0x4091) {
            sub->entity = 0x4090;
        }
        fux->y += 0xC;
        eng->XYZ.y += fux->y;
        eng->XYZ.x += (rcossin_tbl[(fux->angle - 0x400) & 0xFFF].unk2 * 5) >> 8;
        eng->XYZ.z += (rcossin_tbl[fux->angle & 0xFFF].unk2 * 5) >> 8;
        if (eng->XYZ.y > 0) {
            PlaySfx(0x75A, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
            i = BarrelNearHero(eng->XYZ.x, eng->XYZ.z, 0x280);
            if (i >= 0) {
                if (ChaseRoundOver == 0) {
                    HeroInfo *hb = Heroes;

                    h = &hb[i];
                    if (h->hero_immunity == 0 && !(h->hero_status & 0xFA00)) {
                        SetHeroAction(i, (u16)HeroActHit);
                        ((void (*)())pad_rumble_big)(i, 0xA, 0xF0);  /* matching: Chase called it unprototyped, so 0xF0 goes as an int, not the s8 -16 */
                    }
                }
                if (GAME->heroCpu[i] == 0) {
                    ChaseHeroState[i][0] = 0;
                }
            }
            skip = 0x63;
            lim = 0x280;
            i = 0;
            x = eng->XYZ.x;
            z = eng->XYZ.z;
            for (;;) {
                s32 d;

                if (i >= 4) {
                    break;
                }
                if (skip != i) {
                    d = x - Heroes[i].hero_eng->XYZ.x;
                    if (d >= -0x280 && d <= lim) {
                        d = z - Heroes[i].hero_eng->XYZ.z;
                        if (d >= -0x280 && d <= lim && ChaseRoundOver == 0 && Heroes[i].hero_immunity == 0 &&
                            !(Heroes[i].hero_status & 0xFA00)) {
                            SetHeroAction(i, (u16)HeroActHit);
                            ((void (*)())pad_rumble_big)(i, 0xA, 0xF0);  /* matching: Chase called it unprototyped, so 0xF0 goes as an int, not the s8 -16 */
                        }
                    }
                }
                i++;
            }
            at = &eng->XYZ;
            p = MapScene->dbase;
            n = CreateNAS(eng->XYZ.x, at->y, at->z, 0x1605, 0x11);
            if (n != NULL) {
                ext = FindExtension(n, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = 0;
                    ext->unk18 = 0x100;
                    if (p != 0) {
                        ext->unk20 = 5;
                        n->Engine->sub->u6C.dbase = p;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    n->InitDelete(n);
                }
            }
            KillNAS(nas);
            CreateNAS(0, 0, 0, 0x1D08, 0x11);
        }
        break;
    }
}

void PlotCoalFlameSFX(CoalSfxObj *obj) {
    CoalSfxInner *in = obj->inner;

    if (ChaseFlameState[obj->idx] > 0) {
        if (ChaseFlameSfxHandle[obj->idx] == 0) {
            PlaySfx(ChaseFlameSfx[Random(4)], &in->voice, &in->pos, 0x1000, 0x1E00, 0);
        } else {
            ChaseFlameState[obj->idx]--;
            if (ChaseFlameState[obj->idx] <= 0) {
                if (in->voice != 0) {
                    StopSfx(in->voice);
                }
                ChaseFlameObj[obj->idx] = 0;
            }
        }
    }
    ChaseFlameSfxHandle[obj->idx] = ChaseFlameState[obj->idx];
}

void PlotPUGenerator_CH(NAS *nas) {
    s32 i;
    s32 j;
    s32 x;
    s32 z;
    NAS *pu;
    subengine *sub;

    if (ChaseRoundOver != 0) {
        KillNAS(nas);
        return;
    }
    if (nas->UserInfo.flags != 0) {
        nas->UserInfo.flags--;
        return;
    }
    nas->UserInfo.flags = 0x258;
    for (i = 0;; i++) {
        if (i >= 1) {
            return;
        }
        if (ChasePickup[i] != NULL) {
            continue;
        }
    again:
        do {
            x = (Random(0xE) - 7) << 8;
            z = (Random(0xE) - 7) << 8;
        } while (BarrelNearHero(x, z, 0x200) >= 0);
        for (j = 0; j < 8; j++) {
            if (ChaseBarrels[j] != NULL && ChaseBarrels[j]->Engine->XYZ.x == x &&
                ChaseBarrels[j]->Engine->XYZ.z == z) {
                goto again;
            }
        }
        pu = CreateNAS(x, 0, z, 0x1D04, 0x11);
        if (pu != NULL) {
            ChasePickup[i] = pu;
            sub = pu->Engine->sub;
            sub->u6C.dbase = &HeroModelBank[DB_Powerups];
            sub->entity = 0x200C;
            pu->UserInfo.status = Random(2) + 1;
            return;
        }
    }
}

void InitPickupCH(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    PickupCHExt *ext;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExt(nas, sizeof(PickupCHExt), 0x10000006);
    ext->y = 0;
    eng = nas->Engine;
    nas->FUX = (EXT *)ext;
    eng->xform.order = 1;
    e = AddExtEngine(nas, 1);
    if (e != NULL) {
        e->XYZ.x = eng->XYZ.x;
        e->XYZ.y = 0;
        e->XYZ.z = eng->XYZ.z;
        e->sub->xform.order = 2;
        e->sub->u6C.dbase = &HeroModelBank[DB_Powerups];
        e->sub->entity = 0x2001;
        e->sub->flags |= 0x40000041;
        e->sub->scale = 0x800;
        e->sub->xform.hasScale = 1;
        e->sub->xform.dirty = 1;
    }
    PlaySfx(0x3FA, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
}

void PlotPickupCH(NAS *nas) {
    PickupCHExt *ext = (PickupCHExt *)nas->FUX;
    EXTengine *eng = nas->Engine;
    HeroInfo *h;
    EXTengine *he;
    EXTengine *p;
    subengine *sub;
    s32 i;
    s32 hero;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;
    s32 lim;
    s32 status;

    ext->y += 8;
    eng->XYZ.y += ext->y;
    if (eng->XYZ.y >= 0) {
        eng->XYZ.y = -1;
        ext->y = -0x28;
        eng->XYZ.y -= 0x28;
    }
    lim = 0x100;
    x = eng->XYZ.x;
    z = eng->XYZ.z;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        he = h->hero_eng;
        dx = x - he->XYZ.x;
        if (dx < -0x100) {
            continue;
        }
        if (dx > lim) {
            continue;
        }
        dz = z - he->XYZ.z;
        if (dz > -lim) {
            if (dz < 0x100) {
                hero = i;
                goto found;
            }
        }
    }
    hero = -1;
found:
    if (hero < 0) {
        return;
    }
    if (ChaseRoundOver != 0) {
        for (i = 0; i < 1; i++) {
            if (ChasePickup[i] == nas) {
                ChasePickup[i] = NULL;
                break;
            }
        }
    } else {
        PlaySfx(0x439, NULL, &eng->XYZ, 0x1000, 0x1E00, 0);
        status = nas->UserInfo.status;
        sub = NULL;
        ChaseHidePowerup(hero);
        p = Heroes[hero].hero_powerup;
        if (p != NULL) {
            sub = p->sub;
        }
        if (status < 3) {
            if (status != 0) {
                if (sub != NULL) {
                    sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                    sub->entity = 0x200C;
                    sub->flags |= 0x8000;
                    p->flags |= 0x8000;
                }
                ChaseHeroState[hero][0x14] = status;
            }
        }
        for (i = 0; i < 1; i++) {
            if (ChasePickup[i] == nas) {
                ChasePickup[i] = NULL;
                break;
            }
        }
    }
    KillNAS(nas);
}

void ChaseChooseWinnerScore(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    s32 free;
    s32 m;
    GameState *g;

    free = -1;
    for (i = 0; i < 4; i++) {
        if (ChaseHeroState[i][0] == 0) {
            free = i;
        }
    }
    if (free != -1) {
        GAME->winner[0] = 0;
        GAME->exitFlags |= arg1;
        for (i = 0; i < 4; i++) {
            if (GAME->heroCpu[i] != 0) {
                RemoveCamera(Heroes[i].hero);
                Heroes[i].hero_status |= 0x400;
            } else if (i != free) {
                Heroes[i].hero_status |= 0x100;
            }
        }
        CameraMode = 1;
    } else if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (m = 0; m < 4; m++) {
                if (!((taken >> m) & 1)) {
                    if (Heroes[m].hero_score > Heroes[best].hero_score || best == -1) {
                        best = m;
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
                    if (GAME->heroCpu[k] != 0) {
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
                    if (GAME->heroCpu[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                g->exitFlags |= 0x800;
                if (g->heroCpu[g->teamLeader[0]] == 0) {
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
                    if (GAME->heroCpu[k] != 0) {
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

void ChaseChooseWinnerScoreB(s32 arg0, u16 arg1) {
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

            if (g->heroCpu[g->adventure.rank[0]] != 0
                || (hb = Heroes, a = &hb[g->adventure.rank[0]], a->hero_score == hb[g->adventure.rank[1]].hero_score)) {
                GAME->winner[0] = 0;
                GAME->exitFlags |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GAME->heroCpu[k] != 0) {
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
                    if (GAME->heroCpu[k] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                g->exitFlags |= 0x800;
                if (g->heroCpu[g->teamLeader[0]] == 0) {
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
                    if (GAME->heroCpu[k] != 0) {
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

void ChaseChooseWinnerTimed(s32 arg0, u16 arg1) {
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
            if (g->heroCpu[g->teamLeader[0]] == 0) {
                GAME->winner[0] = a->hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroCpu[i] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        } else if (sb == 0) {
            if (g->heroCpu[g->teamLeader[1]] == 0) {
                GAME->winner[0] = b->hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroCpu[i] != 0) {
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
            if (GAME->heroCpu[found] == 0) {
                GAME->winner[0] = Heroes[found].hero_name;
                GAME->exitFlags |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (GAME->heroCpu[i] != 0) {
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
            if (GAME->heroCpu[i] != 0) {
                Heroes[i].hero_status |= 0x400;
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void ChaseChooseWinnerRank(s32 arg0, u16 arg1) {
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
        RANK68V->adventure.rank[i] = best;
        used |= 1 << best;
    }
    g = RANK68V;
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
        RANK68V->winner[0] = b->hero_name;
        RANK68V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void ChaseChooseWinner(s32 arg0, s32 arg1) {
    GameState *g = GAME;

    if (!(g->exitFlags & 0x801)) {
        u16 f = g->levelFlags;

        if (f & 0x80) {
            ChaseChooseWinnerScore(arg0, (u16)arg1);
        } else if (f & 0x20) {
            ChaseChooseWinnerScoreB(arg0, (u16)arg1);
        } else if (f & 0x40) {
            ChaseChooseWinnerTimed(arg0, (u16)arg1);
        } else {
            ChaseChooseWinnerRank(arg0, (u16)arg1);
        }
    }
}

void KillPickupCH(NAS *nas) {
    s32 i;

    for (i = 0; i < 1; i++) {
        if (ChasePickup[i] == nas) {
            ChasePickup[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPUGenerator_CH(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x258;
}

void ChaseHidePowerup(s32 arg0) {
    EXTengine *p = Heroes[arg0].hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    ChaseHeroState[arg0][0x14] = 0;
}

void ChaseHero_ExtraEffects(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *p = h->hero_powerup;
    EXTengine *e;

    if (p != NULL) {
        e = h->hero_eng;
        p->XYZ = e->XYZ;
        p->xform.dirty = 1;
        p->XYZ.y -= 0x300;
        p->xform.rot.vy = p->xform.rot.vy + 0x38;
    }
}

void ChaseHeroHitFlash(s32 arg0, s32 arg1) {
    EXTengine *p;
    subengine *sub;

    sub = NULL;
    ChaseHidePowerup(arg0);
    p = Heroes[arg0].hero_powerup;
    if (p != NULL) {
        sub = p->sub;
    }
    if (arg1 < 3) {
        if (arg1 > 0) {
            if (sub != NULL) {
                sub->u6C.dbase = &HeroModelBank[DB_Powerups];
                sub->entity = 0x200C;
                sub->flags |= 0x8000;
                p->flags |= 0x8000;
            }
            ChaseHeroState[arg0][0x14] = arg1;
        }
    }
}

void InitCoalFlameSFX(CoalFlameObj *arg0) {
    arg0->inner->unkC = 0;
}

void InitEgg(NAS *nas) {
    EggExt *ext;
    EXTengine *eng;
    subengine *sub;
    obj_dbase *scene;

    ext = AddExt(nas, sizeof(EggExt), 0x10000005);
    nas->FUX = &ext->ext;
    ext->state = 8;
    ext->xvel = -0x55;
    ext->y = -0xA0;
    eng = nas->Engine;
    nas->UserInfo.user2 = 0x78;
    sub = eng->sub;
    nas->UserInfo.status = eng->XYZ.x;
    eng->XYZ.x = 0x600;
    eng->XYZ.y = 0x12C;
    eng->XYZ.z = 0xA60;
    eng->xform.rot.vy = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->xform.dirty = 1;
    scene = MapScene->dbase;
    sub->entity = 0x4080;
    sub->otz = 0x3E7;
    sub->u6C.dbase = scene;
    sub->flags |= 0x2000000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCircleFire(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CircleFireExt *ext;

    ext = AddExt(nas, sizeof(CircleFireExt), 0x10000004);
    nas->FUX = &ext->ext;
    ext->unk30 = 1;
    eng = nas->Engine;
    nas->UserInfo.status = eng->XYZ.x;
    eng->XYZ.x = (rcossin_tbl[((u16)nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 4;
    eng->XYZ.z = (rcossin_tbl[nas->UserInfo.status & 0xFFF].unk2 * 9) >> 4;
    sub = eng->sub;
    eng->xform.rot.vy = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->xform.dirty = 1;
    eng->XYZ.y = 0;
    sub->u6C.dbase = &HeroModelBank[DB_KegObjects];
    sub->entity = 0x4000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCoalFlame(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;

    eng = nas->Engine;
    nas->UserInfo.user2 = 0xF;
    ext = AddExt(nas, sizeof(CoalFlameExt), 0x10000003);
    nas->FUX = &ext->ext;
    ext->unk1C = -0xD;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_KegObjects];
    sub->entity = 0x200E;
    eng->xform.hasScale = 1;
    eng->xform.scale.vx = 0x1000;
    eng->xform.scale.vy = 0x1000;
    eng->xform.scale.vz = 0x1000;
    eng->xform.dirty = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotCoal(NAS *nas) {
    EXTengine *eng = nas->Engine;

    if (nas->UserInfo.flags == 5) {
        eng->xform.dirty = 1;
        eng->xform.rot.vx += 0x44;
        nas->UserInfo.user1 += 0xC;
        eng->XYZ.y += nas->UserInfo.user1;
        if (eng->XYZ.y > 0) {
            eng->XYZ.y = 0;
            eng->sub->entity = 0x2002;
            eng->xform.rot.vx = 0;
            eng->xform.dirty = 1;
            nas->UserInfo.flags = 0;
        }
    }
    nas->UserInfo.status--;
    if (nas->UserInfo.status == 0) {
        s32 xi = (eng->XYZ.x + 0xA00) >> 7;
        s32 zi = (eng->XYZ.z + 0xA00) >> 7;
        s32 idx = zi * 40 + xi;
        ChaseGrid[idx] = 0;
        KillNAS((NAS *)nas);
    }
}

void InitCoal(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    nas->FUX = AddExt(nas, sizeof(EXT) + 0x38, 0x10000002);
    eng = nas->Engine;
    eng->xform.order = 1;
    nas->UserInfo.flags = 5;
    nas->UserInfo.user1 = 0;
    sub = eng->sub;
    sub->u6C.dbase = &HeroModelBank[DB_KegObjects];
    sub->entity = 0x200F;
    nas->UserInfo.status = 0x3C;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void KillPickupCHB(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (ChaseBarrels[i] == nas) {
            ChaseBarrels[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitBarrelGenerator(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x3C;
}

void ChaseHeroDie(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 5;
    SetHeroAction(arg0, 0x77);
    if (hero->hero_hit != 0) {
        hero->hero_hit--;
    } else {
        SetHeroAction(arg0, 0x77);
    }
    ProcessHeroAnim(arg0);
}

void ChaseHeroDrop(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;
    s32 y;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 2;
    y = hero->hero_eng->XYZ.y + hero->hero_sub->XYZ.y;
    if (World_floor_level + 0x180 < y) {
        SetHeroAction(arg0, 0x77);
    }
    ChaseHeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void ChaseHeroJump(s32 arg0) {
    ChaseHeroKickCollision(arg0);
    ProcessHeroAnim(arg0);
}

void SetChaseHeroRun(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x64);
}

void ChaseUnusedStub0(void) {
}

void ChaseUnusedStub1(void) {
}

void SetChaseHeroIdle(s32 arg0) {
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq(arg0, 0x61);
}

s32 ChaseNoKick(void) {
    return 0;
}

s32 ChaseAlwaysOne(void) {
    return 1;
}

s32 ChaseTestEnd(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x75;
    if (!(st & 0x200)) {
        act = 0x74;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction(arg0, act);
    return 1;
}

s32 ChaseHeroTestJump(s32 arg0) {
    if (!(Pads[arg0].dirPressed & 0x4000)) {
        return 0;
    }
    SetHeroAction(arg0, 0x78);
    return 1;
}

s32 ChaseHeroToIdle(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction(arg0, 0x71);
        return 1;
    }
    return 0;
}

s32 ChaseHeroTestRun(s32 arg0) {
  if (Heroes[arg0].hero_move->forward_speed != 0)
  {
    SetHeroAction(arg0, 0x72);
    return 1;
  }
  return 0;
}

void ChaseHitHeroesNear(s32 x, s32 z, s32 skip, s32 range) {
    HeroInfo *h;
    s32 d;
    s32 i;
    s32 neg;

    i = 0;
    neg = -range;
    h = Heroes;
loop:
    if (i < 4) {
        if (skip != i) {
            d = x - h->hero_eng->XYZ.x;
            do {
                if (d < neg) {
                    goto next;
                }
            } while (0);
            if (d <= range) {
                d = z - h->hero_eng->XYZ.z;
                if (d >= neg && d <= range && ChaseRoundOver == 0 &&
                    h->hero_immunity == 0 && !(h->hero_status & 0xFA00)) {
                    SetHeroAction(i, (u16)HeroActHit);
                    ((void (*)())pad_rumble_big)(i, 0xA, 0xF0);  /* matching: Chase called it unprototyped, so 0xF0 goes as an int, not the s8 -16 */
                }
            }
        }
    next:
        h++;
        i++;
        goto loop;
    }
}

void ChaseHitHero(s32 arg0) {
  HeroInfo *h;
  HeroInfo *new_var;
  if (ChaseRoundOver == 0)
  {
    new_var = Heroes;
    h = new_var;
    h += arg0;
    if ((h->hero_immunity == 0) && (!(h->hero_status & 0xFA00)))
    {
      SetHeroAction(arg0, (u16) HeroActHit);
      ((void (*)())pad_rumble_big)(arg0, 0xA, 0xF0);  /* matching: Chase called it unprototyped, so 0xF0 goes as an int, not the s8 -16 */
    }
  }
}

s32 ChaseFindHero(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->XYZ.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->XYZ.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

s32 ChasePadHook(s32 arg0) {
    u32 m = Pads[arg0].dirPressed & 0x2000;
    return m != 0;
}

void ChaseClampToArena(s32 *p, s32 arg1) {
    s32 d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - arg1;
    s32 a;
    if (d > 0) {
        a = get_angle(p[4], p[6]);
        p[4] -= (d * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 12;
        p[6] -= (d * rcossin_tbl[a & 0xFFF].unk2) >> 12;
    }
}

void FirstTriggersCHASE(void) {
}

void ChaseTriggers(void) {
}

void ChaseStopHeroSfx(void) {
}

void CHASESetUp(void) {
    LevelSpecs.LevelPreInit = InitialiseCHASEFirst;
    LevelSpecs.LevelInit = InitialiseCHASE;
    LevelSpecs.LevelRestartInit = InitialiseCHASE;
    LevelSpecs.LevelRoutine = UpdateCHASE;
    LevelSpecs.LevelExit = ChaseStopHeroSfx;
    LevelSpecs.PostTriggers = ChaseTriggers;
    LevelSpecs.EarliestTriggers = FirstTriggersCHASE;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.FinishQuick = FinishChaseLevel;
}

void ChaseGridClear(s32 x, s32 y) {
    s32 xi = (x + 0xA00) >> 7;
    s32 yi = (y + 0xA00) >> 7;
    s32 idx = yi * 40 + xi;
    ChaseGrid[idx] = 0;
}

NAS *ChaseGridAt(s32 x, s32 z) {
    s32 xi;
    s32 zi;
    s32 idx;

    if ((u32)(x + 0x9FF) >= 0x13FF || z >= 0xA00 || z < -0x9FF) {
        return 0;
    }
    xi = (x + 0xA00) >> 7;
    zi = (z + 0xA00) >> 7;
    idx = zi * 40 + xi;
    return ChaseGrid[idx];
}

void ChaseKillGridObj(CoalGridObj *obj) {
    CoalPosInner *p = obj->pos;
    s32 x = p->x;
    s32 z = p->z;
    s32 idx;
    CoalGridObj *old;

    x += 0xA00;
    z += 0xA00;
    x >>= 7;
    z >>= 7;
    idx = z * 40 + x;
    old = ChaseGrid[idx];

    ChaseGrid[idx] = obj;
    if (old != NULL) {
        KillNAS((NAS *)old);
    }
}

s32 ChaseFindHeroNear(s32 x, s32 z, s32 range, s32 skip) {
    s32 i;
    s32 neg;
    s32 dx;
    s32 dz;
    HeroInfo *h;
    EXTengine *eng;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (i == skip || h->hero_name == 9 || (h->hero_status & 0xC000)) {
            continue;
        }
        eng = h->hero_eng;
        dx = x - eng->XYZ.x;
        if (dx < -range || range < dx) {
            continue;
        }
        dz = z - eng->XYZ.z;
        if (-range < dz && dz < range) {
            return i;
        }
    }
    return -1;
}

s32 ChaseFindHeroAt(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->XYZ.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->XYZ.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

void ChaseResetSlot(s32 arg0) {
    ChaseAiState *temp_v0;

    temp_v0 = (ChaseAiState *)((arg0 * 0x30) + ChaseHeroSlots);
    (temp_v0->state) = 1;
    (temp_v0->timer) = 1;
    (temp_v0->flags) = 0;
    (temp_v0->target) = 0;
    (temp_v0->pickup) = 0;
    (temp_v0->timer2) = 0;
    (temp_v0->flag) = 0;
    (temp_v0->reach) = 0x8000;
    (temp_v0->reach2) = 0x8000;
}

NAS *ChaseFindPickupNear(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 1; i++) {
        nas = ChasePickup[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->XYZ.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->XYZ.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

void ChaseAimAt(s32 arg0, s32 arg1, s32 arg2, EXTengine *arg3, ChaseOut *arg4) {
    s32 ang;
    s32 n;

    ang = get_angle(arg3->XYZ.x - arg1, arg3->XYZ.z - arg2);
    arg4->unk0 = 2;
    n = ChaseHeroState[arg0][17];
    arg4->unk4 = (n / 2 + 0x400) / n;
    arg4->unk8 = *(u32 *)FILE_AT(DirPadBits, (ang >> 7) & 0x1C);
    if (HeroesAllOut == 0) {
        Random(4);
    }
}

s32 ChaseDist(ChaseObj *a, ChasePos *b) {
    ChasePos *p = a->pos;
    s32 dx = b->x - p->x;
    s32 dz;
    dx = dx * dx;
    dz = b->z - p->z;
    dz = dz * dz;
    return csqrt(dx + dz) >> 6;
}
