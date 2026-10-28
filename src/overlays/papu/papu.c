#include "common.h"
#include "bash/main.h"
#include "bash/papu_ovl.h"
#include "bash/papu.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_80050148[];
extern u8 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char PapuAnimBreathe[] = "BREATHE";
const char PapuAnimMedium[] = "MEDIUM";
const char PapuAnimHit[] = "HIT";
const char PapuAnimDie[] = "DIE";
const char PapuAnimJump[] = "JUMP";
const char PapuAnimAttack[] = "ATTACK";
const char PapuAnimPickup[] = "PICKUP";
const char PapuAnimHold[] = "HOLD";
const char PapuAnimHoldSlow[] = "HOLD_SLOW";
const char PapuAnimHoldThrow[] = "HOLD_THROW";
const char PapuAnimTauntA[] = "TAUNT_A";

/* data, in address order */
u16 PapuAiSkill[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
u16 PapuAiSkillAlt[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
};
s16 PapuBossRates[32] = {
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x12C, 0x14A, 0x168, 0x186, 0x1A4,
    0x1C2, 0x1E0, 0x1FE, 0x21C, 0x23A, 0x258, 0x276, 0x294, 0x2B2, 0x2D0, 0x2EE, 0x30C, 0x32A,
    0x348, 0x366, 0x384, 0x384, 0x384,
};
s16 PapuPickupRates[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
};
/* sequence-shaped halfwords nothing points at (no hero's sequence table, no code) */
s16 PapuUnusedSeq[6] = { 0, -0x7FF9, 0x17, 0x4500, -0x8000, 0 };
hero_seq CRA_PA_seq_die[10] = {
    0, 0x8007, 0x20, 0x4400, 0x8018, 0xC, 0x441F, 0x8004, 0x1A, 0,
};
hero_seq CRA_PA_seq_pickup[8] = {
    0, 0x800A, 0xF, 0x8007, 0xB, 0x4202, 0x8004, 0x17,
};
hero_seq CRA_PA_seq_hold[6] = {
    0, 0x8007, 0x14, 0x4280, 0x8000, 0,
};
hero_seq CRA_PA_seq_hold_slow[16] = {
    0, 0x8007, 7, 0x4380, 0x8008, 0xA, 0x8007, 0xC, 0x4387, 0x8008, 0xB, 0x8007, 5, 0x4393, 0x8000,
    0,
};
hero_seq CRA_PA_seq_hold_throw[24] = {
    0, 0x8007, 2, 0x4300, 0x8003, 0x12, 0x8007, 6, 0x4301, 0x8008, 0x10, 0x8003, 0x13, 0x8007, 7,
    0x4310, 0x8004, 7, 0, 0x8007, 0x20, 0x4400, 0x8000, 0,
};
hero_seq COC_PA_seq_die[10] = {
    0, 0x8007, 0x15, 0x4580, 0x8018, 0x14, 0x4594, 0x8004, 0x1A, 0,
};
hero_seq COC_PA_seq_pickup[8] = {
    0, 0x800A, 0x41, 0x8007, 0xC, 0x4201, 0x8004, 0x17,
};
hero_seq COC_PA_seq_hold[6] = {
    0, 0x8007, 0x14, 0x4380, 0x8000, 0,
};
hero_seq COC_PA_seq_hold_slow[16] = {
    0, 0x8007, 9, 0x4300, 0x8008, 0x3C, 0x8007, 0xB, 0x4309, 0x8008, 0x3D, 0x4314, 0x4315, 0x8000,
    0, 0,
};
hero_seq COC_PA_seq_hold_throw[24] = {
    0, 0x8007, 2, 0x4280, 0x8003, 0x12, 0x8007, 6, 0x4281, 0x8008, 0x42, 0x8003, 0x13, 0x8007, 8,
    0x4296, 0x8004, 7, 0, 0x8007, 0x20, 0x4500, 0x8000, 0,
};
hero_seq BRO_PA_seq_die[10] = {
    0, 0x8007, 0x23, 0x4400, 0x8018, 5, 0x4422, 0x8004, 0x1A, 0,
};
hero_seq BRO_PA_seq_pickup[8] = {
    0, 0x800A, 0xA5, 0x8007, 0xD, 0x4201, 0x8004, 0x17,
};
hero_seq BRO_PA_seq_hold[6] = {
    0, 0x8007, 0x18, 0x4280, 0x8000, 0,
};
hero_seq BRO_PA_seq_hold_slow[14] = {
    0, 0x8008, 0xA0, 0x8007, 0xC, 0x4300, 0x8008, 0xA1, 0x8007, 0xC, 0x430B, 0x8000, 0, 0,
};
hero_seq BRO_PA_seq_hold_throw[22] = {
    0, 0x4380, 0x8003, 0x12, 0x4381, 0x800B, 0x8008, 0xA6, 0x8003, 0x13, 0x4382, 0x8007, 0xA,
    0x4383, 0x8004, 0, 0, 0x8007, 0x20, 0x4080, 0x8000, 0,
};
hero_seq CTX_PA_seq_die[10] = {
    0, 0x8007, 0x15, 0x4580, 0x8018, 0xF, 0x4594, 0x8004, 0x1A, 0,
};
hero_seq CTX_PA_seq_pickup[8] = {
    0, 0x800A, 0x73, 0x8007, 8, 0x4300, 0x8004, 0x17,
};
hero_seq CTX_PA_seq_hold[6] = {
    0, 0x8007, 0x18, 0x4280, 0x8000, 0,
};
hero_seq CTX_PA_seq_hold_slow[14] = {
    0, 0x8008, 0x6E, 0x8007, 0xC, 0x4400, 0x8008, 0x6F, 0x8007, 0xC, 0x440B, 0x8000, 0, 0,
};
hero_seq CTX_PA_seq_hold_throw[22] = {
    0, 0x4380, 0x8003, 0x12, 0x4381, 0x800B, 0x8008, 0x74, 0x8003, 0x13, 0x4382, 0x8007, 0xA,
    0x4383, 0x8004, 0, 0, 0x8007, 0x1F, 0x4500, 0x8000, 0,
};
hero_seq DNG_PA_seq_die[10] = {
    0, 0x8007, 0x1F, 0x4480, 0x8018, 0xD, 0x449E, 0x8004, 0x1A, 0,
};
hero_seq DNG_PA_seq_pickup[8] = {
    0, 0x800A, 0x109, 0x8007, 0x10, 0x4401, 0x8004, 0x17,
};
hero_seq DNG_PA_seq_hold[6] = {
    0, 0x8007, 0x16, 0x4280, 0x8000, 0,
};
hero_seq DNG_PA_seq_hold_slow[14] = {
    0, 0x8008, 0x104, 0x8007, 0xC, 0x430B, 0x8008, 0x105, 0x8007, 0xC, 0x4300, 0x8000, 0, 0,
};
hero_seq DNG_PA_seq_hold_throw[24] = {
    0, 0x8007, 2, 0x4380, 0x8003, 0x12, 0x8007, 6, 0x4381, 0x8008, 0x10A, 0x8003, 0x13, 0x8007,
    0x14, 0x4388, 0x8004, 0, 0, 0x8007, 0x20, 0x4480, 0x8000, 0,
};
hero_seq TIN_PA_seq_die[10] = {
    0, 0x8007, 0x15, 0x4600, 0x8018, 0xF, 0x4614, 0x8004, 0x1A, 0,
};
hero_seq TIN_PA_seq_pickup[8] = {
    0, 0x800A, 0x13B, 0x8007, 9, 0x4401, 0x8004, 0x17,
};
hero_seq TIN_PA_seq_hold[6] = {
    0, 0x8007, 0x21, 0x4300, 0x8000, 0,
};
hero_seq TIN_PA_seq_hold_slow[14] = {
    0, 0x8008, 0x136, 0x8007, 0xA, 0x4280, 0x8008, 0x137, 0x8007, 0xA, 0x4289, 0x8000, 0, 0,
};
hero_seq TIN_PA_seq_hold_throw[32] = {
    0, 0x4382, 0x8008, 0x13C, 0x8003, 0x12, 0x4383, 0x4384, 0x4385, 0x4386, 0x8003, 0x13, 0x4387,
    0x4387, 0x4388, 0x4389, 0x438A, 0x4394, 0x4395, 0x4396, 0x4397, 0x4398, 0x4399, 0x439A, 0x8004,
    0, 0, 0x8007, 0x20, 0x4500, 0x8000, 0,
};
hero_seq KAN_PA_seq_die[10] = {
    0, 0x8007, 0x17, 0x4280, 0x8018, 0x17, 0x4296, 0x8004, 0x1A, 0,
};
hero_seq KAN_PA_seq_pickup[8] = {
    0, 0x800A, 0xD7, 0x8007, 0x10, 0x4301, 0x8004, 0x17,
};
hero_seq KAN_PA_seq_hold[6] = {
    0, 0x8007, 0x15, 0x4380, 0x8000, 0,
};
hero_seq KAN_PA_seq_hold_slow[14] = {
    0, 0x8008, 0xD2, 0x8007, 0xA, 0x4409, 0x8008, 0xD3, 0x8007, 0xA, 0x4400, 0x8000, 0, 0,
};
hero_seq KAN_PA_seq_hold_throw[22] = {
    0, 0x8003, 0x12, 0x8007, 6, 0x4481, 0x8008, 0xD8, 0x8003, 0x13, 0x8007, 0x14, 0x4487, 0x8004, 0,
    0, 0, 0x8007, 0x17, 0x4500, 0x8000, 0,
};
hero_seq KON_PA_seq_die[10] = {
    0, 0x8007, 0x19, 0x4200, 0x8018, 0xB, 0x4218, 0x8004, 0x1A, 0,
};
hero_seq KON_PA_seq_pickup[8] = {
    0, 0x800A, 0x16D, 0x8007, 0xB, 0x430A, 0x8004, 0x17,
};
hero_seq KON_PA_seq_hold[6] = {
    0, 0x8007, 0x18, 0x4380, 0x8000, 0,
};
hero_seq KON_PA_seq_hold_slow[14] = {
    0, 0x8008, 0x168, 0x8007, 0xA, 0x4400, 0x8008, 0x169, 0x8007, 0xA, 0x4409, 0x8000, 0, 0,
};
hero_seq KON_PA_seq_hold_throw[18] = {
    0, 0x4482, 0x8003, 0x12, 0x4483, 0x4484, 0x4485, 0x4486, 0x8008, 0x16E, 0x8003, 0x13, 0x4487,
    0x8007, 0xE, 0x4488, 0x8004, 0,
};
/* the shell settings (engine world record 0x0105) */
shell_setup papu_shell_info = {
    0x2000, 0x10, 0x1000, 0xC8, 0x2BC, 0x200, VramAreas, VramNodePool, 0x20, 0x400,
};
s32 PapuFloorWave0[20] = {
    0x70001, 0x80006, -0x10000, 0, 0, 0x70003, 6, -0x10000, 0, 0, 0x70005, 0x80006, -0x10000,
    0, 0, 0, 0, 0, 0, 0,
};
s32 PapuFloorWave1[40] = {
    0x70000, 0x60006, -0x10000, 0, 0, 0x70003, 0xA0006, -0x10000, 0, 0, 0x70006, 0x60006,
    -0x10000, 0, 0, 0x70001, 0x2C0006, -0x10000, 0, 0, 0x70002, 0x260006, -0x10000, 0, 0,
    0x70004, 0x260006, -0x10000, 0, 0, 0x70005, 0x2C0006, -0x10000, 0, 0, 0, 0, 0, 0, 0,
};
s32 PapuFloorWave2[55] = {
    0x70001, 0x60006, -0x10000, 0, 0, 0x70003, 6, -0x10000, 0, 0, 0x70005, 0x60006, -0x10000,
    0, 0, 0x70000, 0x280006, -0x10000, 0, 0, 0x70002, 0x220006, -0x10000, 0, 0, 0x70004,
    0x220006, -0x10000, 0, 0, 0x70006, 0x280006, -0x10000, 0, 0, 0x70001, 0x560006, -0x10000,
    0, 0, 0x70003, 0x500006, -0x10000, 0, 0, 0x70005, 0x560006, -0x10000, 0, 0, 0, 0, 0, 0, 0,
};
s32 PapuFloorWave3[35] = {
    0x20000, 0xC0006, -0xFFFF, 0, 0, 0x40000, 0x60006, -0xFFFF, 0, 0, 0x60000, 6, -0xFFFF, 0,
    0, 0x70001, 6, -0xFFFF, 0, 0, 0x70003, 0x60006, -0xFFFF, 0, 0, 0x70005, 0xC0006, -0xFFFF,
    0, 0, 0, 0, 0, 0, 0,
};
PapuKey *PapuFloorWaves[4] = { (void *)&PapuFloorWave0, (void *)&PapuFloorWave1, (void *)&PapuFloorWave2, (void *)&PapuFloorWave3 };
s32 PapuMinionWave0[8] = { -0x600, -0x300, 0x300, 0, 0, 0, 0, 0xFFFF };
s32 PapuMinionWave1[8] = { 0x600, -0x300, 0x200, 0, 0, 0, 0, 0xFFFF };
s32 PapuMinionWave2[12] = { -0x600, -0x300, 0x400, 0, 0x600, -0x300, 0x200, 0x14, 0, 0, 0, 0xFFFF };
s32 PapuMinionWave3[12] = { -0x600, -0x300, 0x100, 0, 0x600, -0x300, 0x200, 0x14, 0, 0, 0, 0xFFFF };
s32 PapuMinionWave4[12] = { -0x600, -0x300, 0x100, 0, 0x600, -0x300, 0x400, 0x14, 0, 0, 0, 0xFFFF };
s32 PapuMinionWave5[12] = { -0x600, -0x300, 0x100, 0, 0x600, -0x300, 0x400, 0x1E, 0, 0, 0, 0xFFFF };
PapuStep *PapuMinionWaves[6] = {
    (void *)&PapuMinionWave0, (void *)&PapuMinionWave1, (void *)&PapuMinionWave2, (void *)&PapuMinionWave3,
    (void *)&PapuMinionWave4, (void *)&PapuMinionWave5,
};
u16 PapuAttackPlan[64] = {
    1, 0, 2, 0, 3, 0, 5, 0, 6, 0, 8, 0, 7, 1, 8, 0, 9, 0, 4, 1, 2, 1, 3, 1, 5, 0, 6, 2, 8, 0,
    7, 3, 8, 0, 9, 0, 4, 0xA, 2, 2, 3, 2, 5, 0, 6, 4, 8, 0, 7, 5, 8, 0, 7, 4, 8, 0, 9, 0, 4,
    0x13, 2, 3, 0xA, 0,
};
u16 PapuTileModels[28] = {
    0, 0x2006, 0x2007, 0x2008, 0x2009, 0x200A, 0x2005, 0x2005, 0x2005, 0x2005, 0x2004, 0x2004,
    0x2004, 0x2004, 0x2003, 0x2003, 0x2003, 0x2003, 0x2002, 0x2002, 0x2002, 0x2002, 0x2001,
    0x2001, 0x2001, 0x2001, 0, 0,
};
s32 PapuBossSteps0[10] = {
    (s32)0x80110000, 0x40000009, 0x88005, 0x98011, (s32)0x80054009, (s32)0x80110008,
    0x40130009, 0x88005, (s32)0x8001401D, 0,
};
s32 PapuBossSteps1[7] = { (s32)0x80110000, 0x42000042, 0xC8005, 0x58011, (s32)0x800F4241, 0x42460063, 0x8001 };
s32 PapuBossSteps2[3] = { (s32)0x80110000, 0x4180001E, 0x8001 };
s32 PapuBossSteps3[2] = { (s32)0x80010000, 0 };
s32 PapuBossSteps4[6] = { (s32)0x80110000, 0x4100000A, 0x1FD800D, 0x248011, (s32)0x80014109, 0 };
s32 PapuBossSteps5[7] = { (s32)0x80110000, 0x4100000A, 0x1FD800D, 0x248011, (s32)0x80054109, (s32)0x80010008, 0 };
s32 PapuBossSteps6[9] = {
    (s32)0x80110000, 0x4080000D, 0x1F4800D, 0xA8011, (s32)0x8005408C, (s32)0x8011000C,
    0x4097001A, 0x88005, 0x8001,
};
s32 PapuBossSteps7[2] = { (s32)0x80010000, 0 };
s32 PapuBossSteps8[9] = {
    (s32)0x80110000, 0x4280000A, 0x1F7800D, 0xD8011, (s32)0x80054289, (s32)0x8011000D,
    0x42960035, 0x88005, 0x8001,
};
s32 PapuBossSteps9[3] = { (s32)0x80110000, 0x4180001E, 0x8001 };
s32 PapuBossSteps10[3] = { (s32)0x80110000, 0x4180001E, 0x8000 };
s32 PapuBossSteps11[4] = { 0x43000000, 0x1FE800D, 0x268011, (s32)0x80014301 };
s32 PapuBossSteps12[2] = { 0x42460064, 0x8000 };
u16 PapuBossRandomAnims[8] = { 0xE, 0, 0xE, 0xE, 0, 0x13, 0, 0 };
s32 PapuBossAnimList[42] = {
    (s32)&PapuBossSteps0, 0, 0, (s32)&PapuBossSteps6, (s32)&PapuBossSteps4, 0, 0, (s32)&PapuBossSteps7, 0, 0,
    (s32)&PapuBossSteps5, (s32)&PapuBossSteps5, 0, 0, (s32)&PapuBossSteps2, (s32)&PapuBossSteps2,
    (s32)&PapuBossSteps2, 0, 0, (s32)&PapuBossSteps3, 0, 0, (s32)&PapuBossSteps8, 0, 0, (s32)&PapuBossSteps1,
    0, 0x29, (s32)&PapuBossSteps9, (s32)&PapuBossSteps10, (s32)&PapuBossSteps11, 0, 0, (s32)&PapuBossSteps4,
    (s32)&PapuBossSteps4, (s32)&PapuBossSteps2, (s32)&PapuBossSteps2, (s32)&PapuBossSteps3, (s32)&PapuBossSteps4,
    0, 0x21, (s32)&PapuBossSteps12,
};
ScriptInit PapuBossScriptInit = { 0, (void *)&PapuBossAnimList, 0x401 };
s32 PapuMinionSteps0[4] = { (s32)0x80110000, 0x40000016, 0x88005, 0x8000 };
s32 PapuMinionSteps1[7] = {
    (s32)0x80110000, 0x40800009, 0x205800C, 0xB8011, (s32)0x800C4089, 0x40940206,
    (s32)0x80004095,
};
s32 PapuMinionSteps2[1] = { (s32)0x80010000 };
s32 PapuMinionSteps3[7] = { 0x41000002, 0x4800F, 0x41024101, 0x28800F, 0x41034103, 0x41034103, 0x8001 };
s32 PapuMinionSteps4[4] = { (s32)0x800C0000, (s32)0x80110208, 0x4104001B, 0x8001 };
s32 PapuMinionSteps5[4] = { (s32)0x800C0000, (s32)0x8011020A, 0x4202000B, 0x8001 };
s32 PapuMinionSteps6[4] = { (s32)0x80110000, 0x42800014, 0x88005, 0x8000 };
s32 PapuMinionSteps7[9] = {
    (s32)0x80110000, 0x43800007, 0x205800C, 0xC8011, (s32)0x80054387, (s32)0x800C0008,
    (s32)0x80110206, 0x43930005, 0x8000,
};
s32 PapuMinionSteps8[8] = {
    (s32)0x80110000, 0x43000002, 0x68011, (s32)0x800C4301, (s32)0x8005020B, (s32)0x80110010,
    0x43100007, 0x8001,
};
s32 PapuMinionSteps9[8] = {
    (s32)0x80110000, 0x4180000B, 0x88011, (s32)0x8005418B, (s32)0x800F0008, 0x41920014,
    0x98005, 0x8001,
};
s32 PapuMinionSteps10[4] = { (s32)0x800C000E, 0x20060209, 0x88005, 0x8001 };
s32 PapuMinionAnimList[19] = {
    (s32)&PapuMinionSteps0, (s32)&PapuMinionSteps1, (s32)&PapuMinionSteps2, (s32)&PapuMinionSteps3, (s32)&PapuMinionSteps4,
    0, 0, (s32)&PapuMinionSteps5, (s32)&PapuMinionSteps6, (s32)&PapuMinionSteps7, (s32)&PapuMinionSteps8, 0, 0,
    (s32)&PapuMinionSteps9, 0, 0, (s32)&PapuMinionSteps10, 0, 0,
};
ScriptInit PapuMinionScriptInit = { 0, (void *)&PapuMinionAnimList, 0x401 };
PapuSpot PapuSpawnPoints[5] = {
    { -0x500, 0, 0x500 }, { -0x500, 0, 0 }, { 0x500, 0, 0x500 }, { 0x500, 0, 0 },
    { 0, 0, 0x500 },
};
s16 PapuPreLoadDB[8] = { 0x1C, 0x17, 0x1A, 0x18, 0x2C, 0x2D, -1, 0 };
char *PapuAnims[12] = {
    (char *)PapuAnimBreathe, (char *)PapuAnimMedium, (char *)PapuAnimHit, (char *)PapuAnimDie,
    (char *)PapuAnimJump, (char *)PapuAnimAttack, (char *)PapuAnimPickup, (char *)PapuAnimHold,
    (char *)PapuAnimHoldSlow, (char *)PapuAnimHoldThrow, (char *)PapuAnimTauntA, NULL,
};
s16 PapuLevelDB[2] = { -1, 0 };
s32 PapuFiles[27] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 579], (s32)&DatIndex[2 * 578], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 575],
    (s32)&DatIndex[2 * 574], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 577], (s32)&DatIndex[2 * 576], (s32)&HeroModelBank[DB_Powerups],
    (s32)&DatIndex[2 * 573], (s32)&DatIndex[2 * 572], (s32)&HeroModelBank[DB_Angel], (s32)&DatIndex[2 * 571], (s32)&DatIndex[2 * 570],
    (s32)&HeroModelBank[DB_CrateObjects], (s32)&DatIndex[2 * 569], (s32)&DatIndex[2 * 568], (s32)&HeroModelBank[DB_Papu], (s32)&DatIndex[2 * 567],
    (s32)&DatIndex[2 * 566], (s32)&HeroModelBank[DB_Minion], (s32)&DatIndex[2 * 565], (s32)&DatIndex[2 * 564], 0, 0,
};
/* the level's files (the engine's per-mode table points at mode 4's) */
file_group_load_info file_group_PAPU = { D_80050148, PapuFiles };
WWInf PapuWorldInfo[1] = {
    { 0x2B, PapuAnims, PapuLevelDB, 1 },
};
u16 PapuLUPtab[4] = { 3, 0x201, 0x20D, 0x201 };
TrigLookUp PapuLUP = { PapuLUPtab, 4 };
PreMap PAPU_PREMAP = { PapuPreLoadDB, PapuWorldInfo, &PapuLUP };
s16 PapuColumnHeights0[22] = {
    -0x15, -0x1B, -0x26, 0, 0, 0, 0, 0, 0, 0, 0, -0x13, -0x1A, -0x23, 0, 0, 0, 0, 0, 0, 0, 0,
};
s16 PapuColumnHeights1[22] = {
    -0x1D, -0x29, -0x2F, 0, 0, 0, 0, 0, 0, 0, 0, -0x18, -0x24, -0x3A, 0, 0, 0, 0, 0, 0, 0, 0,
};
s16 PapuColumnHeights2[22] = {
    -0x24, -0x32, -0x3D, 0, 0, 0, 0, 0, 0, 0, 0, -0x1E, -0x34, -0x42, 0, 0, 0, 0, 0, 0, 0, 0,
};
s16 PapuColumnHeights3[22] = {
    -0x32, -0x3E, -0x44, 0, 0, 0, 0, 0, 0, 0, 0, -0x34, -0x52, -0x70, 0, 0, 0, 0, 0, 0, 0, 0,
};
s16 *PapuColumnHeights[4] = { (void *)&PapuColumnHeights0, (void *)&PapuColumnHeights1, (void *)&PapuColumnHeights2, (void *)&PapuColumnHeights3 };
u16 PapuMinionSfx[4] = { 0x201, 0x202, 0x203, 0x204 };
s32 PapuBossFlag = 0;
Slot40 PapuMinionSlots[6] = {0};
PapuBossState PapuBoss = {0};
u16 PapuPickupActive = 0;
u16 PapuPickupDelay = 0;
u16 PapuPickupTimer = 0;
s32 PapuFloorTimer = 0;
s32 PapuFloorStarted = 0;
u16 PapuCrateTimer = 0;
s16 PapuUnused0 = 0;
u16 PapuCrateMixPos = 0;
u16 PapuDropPos = 0;
u16 PapuFirstCrates = 0;
s16 PapuUnused1 = 0;
u16 PapuMaxCrates = 0;
s16 PapuUnused2[2] = {0};
u16 PapuCrateCounts[4] = {0};
s16 PapuUnused3[8] = {0};
u8 PapuCrateMix[64] = {0};
PapuCell PapuTiles[8][8] = {0};
PapuGridPos PapuDropPoints[16][16] = {0};
PapuEntry PapuColumns[20] = {0};
EXTengine PapuSfxEngine = {0};
PapuSceneObj *PapuFloorObj = 0;
s16 PapuFloorSink = 0;
s16 PapuBossRate = 0;
PapuKey *PapuFloorWaveCur = 0;
u16 PapuAttackPos = 0;
u16 PapuAttackDelay = 0;
u16 PapuAttackWave = 0;
u16 PapuFloorState[40] = {0};
u16 PapuFloorDepth = 0;
s16 PapuUnused4 = 0;
s16 PapuUnused5 = 0;
s16 PapuBossCount = 0;
s16 PapuBossLast = 0;
s16 PapuUnused6 = 0;
s16 PapuWaveFlag = 0;
/* two zero bytes before pahero.c's data: the link packs to 2 (SUBALIGN), so the padding is written out */
s16 PapuDataPad = 0;

void InitialisePAPUFirst(void) {
    EngineGameView *g;
    u8 *v;
    void **p;

    HeroHooks.KeepInBounds = PAPU_KeepInBounds;
    HeroHooks.Hero2ObjectCollision = PapuHero2ObjectCollision;
    HeroHooks.HeroGrabObject = PapuHeroGrabObject;
    HeroHooks.HeroKickCollision = PapuHeroKickCollision;
    HeroHooks.HeroCarryCollision = PapuHeroCarryCollision;
    PapuInitCrateHooks();
    LevelSpecs.GetContour = GetPapuContour;
    HeroHooks.PostMoveHero = PapuHeroCheckPit;
    g = (EngineGameView *)GameInfo;
    g->flags |= 0x900;
    if (*(s32 *)(GameInfo + 0x6C) == 0) {
        GameInfo[0x51] = 9;
    }
    GameInfo[0x52] = 9;
    GameInfo[0x53] = 9;
    HeroActHit = 0x15;
    HeroActPickup = 0x16;
    HeroActHold = 0x17;
    HeroActCarry = 0x18;
    HeroActThrow = 0x19;
    HeroActDead = 0x1A;
    HeroActWinner = 0x1B;
    p = &AiSkillTable;
    if (*(s32 *)(GameInfo + 0x6C) != 0) {
        v = PapuAiSkillAlt;
    } else {
        v = PapuAiSkill;
    }
    *p = v;
}

void InitialisePAPU(void) {
    EXTheromove *move;
    s32 i;
    s16 angle;

    World_floor_level = 0;
    PapuCrateTimer = 0;
    PapuUnused0 = 0;
    PapuDropPos = 0;
    PapuBossFlag = 0;
    PapuFloorTimer = 0;
    PapuFloorStarted = 0;
    PapuFloorSink = 0;
    PapuBossRate = 0;
    PapuPickupActive = 0;
    PapuPickupDelay = 0;
    PapuPickupTimer = (Random(10) + 10) * 30;
    PapuSfxEngine = EngineTemplate;
    PapuShuffleDropPoints();
    MarkHeroesOnGrid();
    for (i = 0; i < 4; i++) {
        angle = get_angle(Heroes[i].hero_eng->unk10.x, Heroes[i].hero_eng->unk10.z);
        move = Heroes[i].hero_move;
        Heroes[i].hero_eng->unk1E = angle;
        move->facing_angle = angle;
        move->forward_angle = angle;
        Heroes[i].hero_ctrl |= 8;
        if (GameInfo[0x51] == 9) {
            Heroes[i].hero_shadow->flags = 0;
        }
    }
    Clock_Off();
    SetPanelHeroIconB(0x2C, 0x300A);
    InitBossHealthIcons();
    PapuInitMinionSlots();
    PapuInitTiles();
    CreateNAS(MapPoints[2].pos.x, MapPoints[2].pos.y, MapPoints[2].pos.z,
                 0x1201, 0x11);
    PapuSetCrateMix(0, 0, 0, 0, 0xC, 4, 0, 0, 0);
    PapuDropFirstCrates();
    HitHooks.unkC = (void (*)(s32, s32, s32, s32))PapuExploRange;
    PapuAttackPos = 0;
    PapuAttackDelay = 0;
}

void PapuSetCrateMix(u16 arg0, s32 a1, s32 a2, s32 a3, u16 n4, u16 n5, u16 n6, u16 n7, u16 n8) {
    s32 t;
    s32 i;
    s32 j;
    s32 k;
    u16 *p;
    u16 *e;
    u8 *a;
    u8 *b;
    u8 tmp;
    s32 x;

    x = arg0 * n6;
    p = PapuCrateCounts;
    t = x / 16;
    k = t + 1;
    p[3] = k;
    p[2] = k;
    p[1] = k;
    do {
    } while (0);
    n4 *= 4;
    n5 *= 4;
    n6 *= 4;
    n7 *= 4;
    n8 *= 4;
    do {
    } while (0);
    i = 0;
    PapuFirstCrates = arg0;
    e = &PapuMaxCrates;
    do {
    } while (0);
    p[0] = k;
    e[3] = arg0;
    e[2] = arg0;
    e[1] = arg0;
    PapuMaxCrates = arg0;
    do {
    } while (0);
    for (j = i; j < n4; j++) {
        PapuCrateMix[i++] = 4;
    }
    for (j = 0; j < n5; j++) {
        PapuCrateMix[i++] = 5;
    }
    for (j = 0; j < n6; j++) {
        PapuCrateMix[i++] = 6;
    }
    for (j = 0; j < n7; j++) {
        PapuCrateMix[i++] = 1;
    }
    x = n8;
    for (j = 0; j < x; j++) {
        PapuCrateMix[i++] = 2;
    }
    j = 0;
    do {
        i = Random(0x40);
        k = Random(0x40);
        a = &PapuCrateMix[i];
        b = &PapuCrateMix[k];
        tmp = *a;
        j++;
        *a = *b;
        *b = tmp;
    } while (j < 0x258);
    PapuCrateMixPos = Random(0x20) + 0x14;
}

void PapuCrateMaster(void) {
    PapuGridPos *p;
    s32 a1;
    s32 y;

    if (GameExitFlags & 0x801) {
        return;
    }
    if (PapuCrateTimer == 0) {
        if (CrateCounts[0] < PapuMaxCrates) {
            PapuCrateTimer = Random(15) + 15;
            PapuDropCrates(1);
        } else {
            PapuCrateTimer = Random(40) + 5;
        }
    } else {
        PapuCrateTimer = PapuCrateTimer - 1;
    }
    a1 = PapuPickupRates[GameAdventure.unkAB[4]];
    if (a1 != 0) {
        if (PapuPickupTimer != 0) {
            PapuPickupTimer -= 1;
        }
    } else {
        PapuPickupTimer = 99;
        PapuPickupActive = 0;
    }
    if (PapuPickupActive != 0) {
        if (PapuPickupDelay != 0) {
            PapuPickupDelay -= 1;
            return;
        }
        p = &PapuDropPoints[0][0];
        p += Random(256);
        a1 = p->x;
        a1 >>= 8;
        a1 += 10;
        y = 9 - (p->y >> 8);
        if (ArenaGrid[y * 20 + a1].unk0 < 16) {
            CreateNAS(p->x, -0x800, p->y, 0x20C, 0x11);
            PapuPickupActive -= 1;
            PapuPickupDelay = Random(4) + 2;
        }
    } else if (PapuPickupTimer == 0) {
        if (PickupCounts[0] < a1) {
            PapuPickupActive = a1;
            PapuPickupTimer = (Random(10) + 10) * 30;
            return;
        }
        PapuPickupTimer = 60;
    }
}

void PapuShuffleDropPoints(void) {
    s32 in, x, y;
    s32 j, i, n, k;
    PapuGridPos *a, *b;

    i = 0;
    do {
        y = (i << 8) - 0x780;
        j = 0;
        do {
            x = (j << 8) - 0x780;
            if ((u32)(i - 3) >= 8U) {
                y = (Random(9) << 8) - 0x480;
            }
            if ((u32)(j - 4) >= 8U) {
                x = (Random(9) << 8) - 0x380;
            }
            PapuDropPoints[i][j].x = x;
            PapuDropPoints[i][j].y = y;
        } while (++j < 16);
    } while (++i < 16);
    for (n = 0; n < 600; n++) {
        i = Random(16);
        j = Random(16);
        in = Random(16);
        k = Random(16);
        a = (PapuGridPos *)((u8 *)PapuDropPoints + (j * 8 + i * 128));
        b = (PapuGridPos *)((u8 *)PapuDropPoints + (k * 8 + in * 128));
        x = a->x;
        y = a->y;
        a->x = b->x;
        a->y = b->y;
        b->x = x;
        b->y = y;
    }
}

PapuMsg *PapuFindDropPoint(u16 *arg0) {
    PapuGridPos *best;
    PapuGridPos *cell;
    u16 bestn;
    u16 tries;
    u16 besti;
    s32 i;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;
    u16 n;

    cell = PapuDropPoints[0];
    bestn = 9;
    tries = 9;
    i = *arg0;
    best = cell = &cell[i];
    besti = i;
    i++;
    i &= 0xFF;
    x = (cell->x >> 8) + 10;
    z = 9 - (cell->y >> 8);
    do {
        while (ArenaGrid[z * 20 + x].unk0 != 0) {
            if (i == *arg0) {
                return NULL;
            }
            cell = PapuDropPoints[0];
            cell = &cell[i];
            i++;
            i &= 0xFF;
            x = (cell->x >> 8) + 10;
            z = 9 - (cell->y >> 8);
        }
        xhi = x;
        xlo = x;
        zhi = z;
        zlo = z;
        if (x > 0) {
            xlo = x - 1;
        }
        if (x < 19) {
            xhi = x + 1;
        }
        if (z > 0) {
            zlo = z - 1;
        }
        if (z < 19) {
            zhi = z + 1;
        }
        n = ArenaGrid[zlo * 20 + xlo].unk0 != 0;
        if (ArenaGrid[zlo * 20 + x].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[zlo * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[z * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[z * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[zhi * 20 + xlo].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[zhi * 20 + x].unk0 != 0) {
            n++;
        }
        if (ArenaGrid[zhi * 20 + xhi].unk0 != 0) {
            n++;
        }
        if (n == 0) {
            *arg0 = i;
            return (PapuMsg *)cell;
        }
        if (n < bestn) {
            bestn = n;
            best = cell;
            besti = i;
        }
        if (i == *arg0) {
            return NULL;
        }
        cell = PapuDropPoints[0];
        cell = &cell[i];
        i++;
        i &= 0xFF;
        x = (cell->x >> 8) + 10;
        z = 9 - (cell->y >> 8);
        if (--tries == 0xFFFF) {
            break;
        }
    } while (1);
    *arg0 = besti;
    return (PapuMsg *)best;
}

void PapuSetColumns(u16 arg0) {
    s32 i;
    PapuEntry *p;

    for (i = 0; i < 20; i++) {
        p = &PapuColumns[i];
        if (p->unk0 != 0) {
            s16 v = PapuColumnHeights[arg0][i];
            p->unkE = 4;
            p->unkA = v;
            p->unkC = 0x60;
            if (abs(p->unkA) >= p->unkC) {
                p->unkE = 0;
            }
        }
    }
}

void PapuMoveColumns(void) {
    PapuEntry *e;
    PapuTile *t;
    s32 i;
    s32 v;
    s32 a;
    s32 c;

    e = PapuColumns;
    i = 0;
    do {
        e = &PapuColumns[i];
        if ((u16)e->unk6 != 0) {
            t = (PapuTile *)e->unk0;
            v = e->unkA;
            c = e->unkC;
            v += e->unkE;
            if (c < v) {
                v = c;
            }
            e->unkA = v;
            a = t->unk8 + v;
            if (a < -0xE00) {
                e->unk6 = 0;
                t->unk0 &= 0xFFFF7FFF;
            }
            if (e->unk8 < a) {
                a = e->unk8;
                e->unkA = 0;
                e->unkC = 0;
                e->unkE = 0;
                e->unk6 = 0;
            }
            t->unk8 = a;
        }
        i++;
    } while (i < 20);
}

void PapuInitMinionSlots(void) {
    s32 i;
    Slot40 *p;

    for (i = 0; i < 6; i++) {
        p = &PapuMinionSlots[i];
        p->unk0 = 0;
        p->pad4[0] = 0;
        p->pad4[1] = 0;
        p->pad4[2] = 0;
        p->pad4[3] = 0;
        p->pad4[4] = 0;
        p->pad4[5] = 0;
        p->pad4[6] = 0;
        p->unk20 = 0;
        p->unk22 = i + 0x14;
        p->unk24 = 0;
        p->unk26 = 0x14;
        p->unk28 = 0;
        p->unk2A = 0;
        p->unk2C = 0;
        p->unk2E = 0;
        p->unk30 = 0;
        p->unk32 = 0;
        p->unk34[0] = 0;
        p->unk34[1] = 0;
        p->unk34[2] = 0;
    }
}


void InitPapuBoss(NAS *nas) {
    EXTengine *eng;
    ScriptCursor *cur;

    eng = nas->Engine;
    if (eng != NULL) {
        ResetHeroGlobals();
        eng->sub->u6C.dbase = &HeroModelBank[DB_Papu];
        eng->sub->entity = 0x2001;
        eng->unk5C = 1;
        eng->unk5E = 1;
        FetchDefaultColBox(eng, 0x40);
        cur = ScriptCursorFirst(nas, &PapuBossScriptInit);
        RunNasAnim(nas);
        PapuBossCount = 0;
        PapuBossLast = 0;
        PapuWaveFlag = 0;
        AddCamera((u8 *)nas);
        PapuBoss.nas = nas;
        PapuBoss.eng = eng;
        PapuBoss.sub = eng->sub;
        PapuBoss.unk22 = 3;
        PapuBoss.cursor = cur;
        PapuBoss.unkC = 0;
        PapuBoss.unk10 = 0;
        PapuBoss.unk14 = 0;
        PapuBoss.unk1C = 0;
        PapuBoss.unk20 = 0;
        PapuBoss.unk24 = 0;
        PapuBoss.unk26 = 0x14;
        PapuBoss.unk28 = 0;
        PapuBoss.unk2A = 0;
        PapuBoss.unk2C = 0;
        PapuBoss.unk2E = 0;
        PapuBoss.unk30 = 0;
        PapuBoss.unk32 = 0;
        PapuBoss.unk34 = 0;
        PapuBoss.unk38 = 0;
        PapuBoss.unk3C = 0;
    }
    if (nas->Failure != 0) {
        printf("boss failure!!!\n");
    }
}

void PlotPapuBoss(NAS *nas) {
    s32 frame;
    EXTanimator *anim;
    EXTengine *eng;
    s16 *p;
    s32 v;
    s32 i;
    s32 j;
    s32 empty;
    NAS *o;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    anim = FindExtension(nas, 0x400);
    frame = RunNasAnim(nas);
    if (BossState.flags & 0x200) {
        if (PapuBossCount != 0x1E) {
            PapuBossCount = 0x1E;
            GotoLinkAnim(anim, 0x1C);
        }
    }
    switch (PapuBossCount) {
    case 7:
        if (frame == 8) {
            PapuBossCount = 0;
        } else if (frame == 0xC) {
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 8:
        switch (frame) {
        case 0xC:
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        case 0xD:
            PapuSpawnMinions(PapuAttackWave);
            break;
        case 8:
            PapuBossCount = 0xB;
            break;
        }
        break;
    case 6:
        PapuHitHeroes(eng);
        if (frame == 8) {
            goto f8;
        }
        if (frame == 0xC) {
            PlaySfx(0x1F5, NULL, NULL, 0x1000, 0x1E00, 0);
            v = 0xB0;
            i = 0;
            p = PapuFloorState;
            PapuWaveFlag = 1;
            PapuFloorState[0] = 0x80;
            PapuFloorDepth = 0x300;
            for (; i < 0x27; i++) {
                if (v <= 0) {
                    break;
                }
                *p = v;
                v -= 0x10;
                p++;
            }
            PapuStopCrates();
            for (j = 0; j < 0x14; j++) {
                if (PapuColumns[j].unk0 != 0) {
                    PapuColumns[j].unk6 = 1;
                }
            }
        }
        break;
    f8:
        PapuFloorStarted = 1;
        PapuBossCount = 0;
        break;
    case 10:
        if (!(GamePause & 0x04000000)) {
            PapuBossCount = 0;
        }
        break;
    case 12:
        if (PapuBoss.unk28 != 0) {
            PapuBossLast = PapuBossCount;
            if (PapuBossFlag != 0) {
                PapuBossCount = 9;
                BossState.health -= 5;
                if (BossState.health < 0) {
                    BossState.health = 0;
                }
                GotoLinkAnim(anim, 0x1E);
            }
        }
        if (frame == 0xC) {
            PlaySfx(0x1F8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (PapuBossRate != 0) {
            PapuBossRate--;
        } else {
            PapuBossCount = 0;
        }
        break;
    case 11:
        empty = 1;
        for (i = 0; i < 6; i++) {
            if (PapuMinionSlots[i].unk0 != 0) {
                empty = 0;
            }
        }
        if (empty) {
            PapuBossCount = 0;
        }
        if (frame == 8) {
            empty = PapuBossRandomAnims[Random(8)];
            ((void (*)(EXTanimator *, s32))GotoLinkAnim)(anim, empty);
        }
        break;
    case 9:
        PapuBossFlag = 0;
    case 1:
        if (frame == 8) {
            PapuBossCount = 0;
        }
        break;
    case 20:
        if (frame == 0xC) {
            o = CreateNAS(eng->unk10.x, eng->unk10.y - 0x800, eng->unk10.z, 0x1203, 0x11);
            if (o != NULL) {
                AppendExtension((NAS *)nas, (NAS *)o);
            }
        }
        break;
    case 30:
        break;
    default:
        PapuAttackDelay = PapuAttackPlan[PapuAttackPos];
        PapuAttackPos++;
        PapuAttackWave = PapuAttackPlan[PapuAttackPos];
        PapuAttackPos++;
        if (PapuAttackDelay == 4) {
            if (PapuBossFlag != 0) {
                PapuAttackPos = PapuAttackWave * 2;
                PapuAttackDelay = PapuAttackPlan[PapuAttackPos];
                PapuAttackPos++;
                PapuAttackWave = PapuAttackPlan[PapuAttackPos];
                PapuAttackPos++;
            } else {
                PapuAttackDelay = PapuAttackPlan[PapuAttackPos];
                PapuAttackPos++;
                PapuAttackWave = PapuAttackPlan[PapuAttackPos];
                PapuAttackPos++;
            }
        }
        switch (PapuAttackDelay) {
        case 1:
            PapuBossCount = 0xA;
            GotoLinkAnim(anim, 0);
            break;
        case 9:
            PapuFloorTimer = 0;
            PapuBossCount = 0xC;
            PapuBossFlag = 1;
            PapuBossRate = PapuBossRates[GameAdventure.unkAB[4]];
            if (PapuFloorObj != NULL) {
                PapuFloorObj->flags |= 0x8000;
            }
            GotoLinkAnim(anim, 0x21);
            break;
        case 2:
            PapuBossFlag = 0;
            if (PapuFloorObj != NULL) {
                PapuFloorObj->flags &= ~0x8000;
            }
            PapuBossCount = 6;
            PapuMaxCrates = (&PapuMaxCrates)[1] = 0;
            GotoLinkAnim(anim, 3);
            PapuSetColumns(PapuAttackWave);
            break;
        case 3:
            PapuStartFloorWave(PapuAttackWave);
            PapuBossCount = 1;
            GotoLinkAnim(anim, 0xA);
            break;
        case 5:
            PapuCrateTimer = 0x1E;
            PapuMaxCrates = (&PapuMaxCrates)[1] = 6;
            break;
        case 6:
            PapuFloorTimer = 1;
            PapuInitMinionSlots();
            PapuBossCount = 8;
            GotoLinkAnim(anim, 0x16);
            break;
        case 7:
            PapuFloorTimer = 1;
            PapuInitMinionSlots();
            PapuBossCount = 8;
            GotoLinkAnim(anim, 0x16);
            break;
        case 8:
            PapuFloorTimer = 1;
            PapuBossCount = 0xB;
            GotoLinkAnim(anim, 0);
            break;
        case 10:
            if (BossState.flags & 0x200) {
                PapuBossCount = 0x1E;
                GotoLinkAnim(anim, 0x1C);
            } else {
                PapuBossCount = 0x14;
                GotoLinkAnim(anim, 0x19);
            }
            break;
        default:
            PapuBossCount = 1;
            GotoLinkAnim(anim, 0);
            break;
        }
        break;
    }
    eng->unk5E = 1;
    PapuBoss.unk28 = 0;
}

NAS *PapuMinionFindCrate(s32 arg0) {
    NAS *best;
    NAS *n;
    s32 bestScore;
    s32 px;
    s32 pz;
    s32 r;
    s32 h;
    EXTengine *e;
    s32 dx;
    s32 dz;
    s32 score;
    s32 mul;
    s32 cnt;
    s32 i;
    T_B4B9CFux *f;
    NAS *o;

    best = NULL;
    bestScore = 0x7FFFFF;
    n = HeadOfObjects;
    h = AiPlayerSkill[3].level;
    px = PapuMinionSlots[arg0].unk34[0];
    pz = px;
    if (n != NULL) {
        r = (0x20 - h) * 16;
        do {
            e = n->Engine;
            if (e != NULL && e->platform == NULL) {
                f = (T_B4B9CFux *)n->FUX;
                if (f != NULL && ((mul = 1, f->type == 4) || (f->type == 5 && (mul = 5))) && f->status == 1) {
                    dx = pz - e->unk10.x;
                    dz = px - e->unk10.z;
                    dx += Random(r);
                    dz += Random(r);
                    score = csqrt(dx * dx + dz * dz) >> 6;
                    score *= mul;
                    cnt = 0;
                    for (i = 0; i < 6; i++) {
                        o = (NAS *)PapuMinionSlots[i].unk0;
                        if (o != NULL && o->FUX != NULL && ((T_B4B9CFux *)o->FUX)->owner == n) {
                            cnt += 2;
                        }
                    }
                    if (cnt != 0) {
                        score <<= cnt;
                    }
                    if (score < bestScore) {
                        best = n;
                        bestScore = score;
                    }
                }
            }
            n = n->Object.Child;
        } while (n != NULL);
    }
    return best;
}

s32 PapuPickSpawnPoint(PapuPos *arg0) {
    s32 best;
    s32 bestScore;
    s32 i;
    s32 j;
    s32 r;
    s32 dx;
    s32 dz;
    s32 score;

    best = Random(5);
    bestScore = 0x90000;
    r = (0x20 - AiPlayerSkill[3].level) * 16;
    for (i = 0; i < 5; i++) {
        dx = PapuSpawnPoints[i].x - arg0->x;
        dz = PapuSpawnPoints[i].z - arg0->z;
        dx += Random(r);
        dz += Random(r);
        score = dx * dx + dz * dz;
        for (j = 0; j < 4; j++) {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                EXTengine *e = Heroes[i].hero_eng;
                dx = PapuSpawnPoints[i].x - e->unk10.x;
                dz = PapuSpawnPoints[i].x - e->unk10.z;
                if (dx * dx + dz * dz <= 0x8FFFF) {
                    score = 0;
                }
            }
        }
        if (bestScore < score) {
            bestScore = score;
            best = i;
        }
    }
    return best;
}

NAS *CreatePapuMinion(s32 a, s32 b, s32 c, u16 d)
{
  NAS *nas;
  u8 *new_var;
  EXTengine *eng;
  PapuObj *obj;
  Slot40 *s;
  s32 i;
  nas = CreateNAS(a, b, c, 0x1202, 0x11);
  if (nas != 0)
  {
    eng = nas->Engine;
    if (eng != 0)
    {
      eng->sub->u6C.dbase = &HeroModelBank[DB_Minion];
      eng->unk5D = 0;
      eng->sub->entity = 0x2001;
      eng->flags &= 0xFFFF7FFF;
      FetchDefaultColBox(eng, 0x50);
      for (i = 0; i < 6; i++)
      {
        if (PapuMinionSlots[i].unk0 == 0)
        {
          goto found;
        }
      }

      i = -1;
      found:
      if (i != (-1))
      {
        nas->UserInfo.user1 = i;
        new_var = (u8 *) PapuMinionSlots;
        s = (Slot40 *) (new_var + (i << 6));
        s->unk0 = (s32) nas;
        s->pad4[0] = (s32) eng;
        s->pad4[1] = (s32) eng->sub;
        s->pad4[4] = (s32) AddMotion(nas);
        *((SLONG_XYZ *) s->unk34) = eng->unk10;
        s->unk26 = 0xA;
        obj = (PapuObj *) AddObject(nas);
        if (obj != 0)
        {
          nas->FUX = (EXT *) obj;
          obj->unk1A = 1;
          obj->unk22 = d;
          obj->unk14 = s->unk22;
          LinkObject((u8 *) nas);
          AddSolid(nas, 0x200);
        }
        s->pad4[5] = (s32) ScriptCursorFirst(nas, &PapuMinionScriptInit);
      }
      else
      {
        KillNAS(nas);
        return 0;
      }

    }
    if (nas->Failure != 0)
    {
      KillNAS(nas);
    }
  }
  return nas;
}

void PlotPapuMinion(NAS *nas)
{
  EXTengine *target_eng;
  s32 slot;
  s32 level;
  EXTengine *eng;
  MinionMove *move;
  EXTanimator *anim;
  MinionFux *fux;
  MinionFux *other;
  NAS *target;
  Slot40 *s;
  s32 dx;
  s32 dz;
  s32 i;
  s32 hit;
  s32 dist;
  s32 angle;
  s32 pick;
  s32 who;
  s32 frame;
  s32 step;
  u16 a;

  slot = nas->UserInfo.user1;
  eng = nas->Engine;
  level = AiPlayerSkill[3].level;
  if (eng == NULL) {
    return;
  }
  target_eng = NULL;
  dist = -1;
  angle = 0;
  move = FindExtension(nas, 0x800);
  if (move == NULL) {
    return;
  }
  anim = FindExtension(nas, 0x400);
  if (anim == NULL) {
    return;
  }
  fux = (MinionFux *)nas->FUX;
  if (fux == NULL) {
    return;
  }
  target = fux->target;
  hit = 0;
  if (target != NULL) {
    for (i = 0; i < 4; i++) {
      if (Heroes[i].hero == target) {
        if (!(Heroes[i].hero_status & 0xFA00)) {
          hit = 1;
        }
        break;
      }
    }
    if (i == 4) {
      pick = (s32)target->FUX;
      if (pick != 0 && ((MinionFux *)pick)->state == 1) {
        hit = 1;
      }
      if (target->Engine->platform != NULL) {
        hit = 0;
      }
    }
    if (hit) {
      target_eng = target->Engine;
      dx = target_eng->unk10.x - eng->unk10.x;
      dz = target_eng->unk10.z - eng->unk10.z;
      angle = get_angle(-dx, -dz);
      dist = csqrt(dx * dx + dz * dz) >> 6;
    }
  }
  if (!hit) {
    fux->target = NULL;
  }
  frame = RunNasAnim(nas);
  switch (fux->state) {
  case 5:
    move->speed = 0;
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    break;
  case 1:
    move->speed = 0;
    if (fux->unk22 == 0) {
      PlaySfx(0x1FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
      SpawnModelFx(0, &eng->unk10.x, (s32)&HeroModelBank[DB_Minion]);
      fux->unk22 = 5;
      fux->state = 2;
    } else {
      fux->unk22--;
    }
    break;
  case 2:
    move->speed = 0;
    if (fux->unk22 != 0) {
      fux->unk22--;
      break;
    }
    eng->flags |= 0x8021;
    fux->state = 3;
    move->unk1E = 0x50;
    move->unk18 = 0;
    move->unk1A = 6;
    move->unk1C = 0;
    move->unkC = 0;
    GotoLinkAnim(anim, 3);
    break;
  case 3:
    move->speed = 0;
    if (GravitateNas(nas) != 0) {
      fux->unk1C |= 0x102;
      eng->flags &= ~0x21;
      fux->state = 4;
      GotoLinkAnim(anim, 4);
    }
    break;
  case 4:
    move->speed = 0;
    if (frame == 8) {
      fux->state = 0;
    }
    break;
  case 6:
    dx = ((s32 (*)(EXTengine *))ApplyContour)(eng);
    i = (s16)((s16)(((eng->unk1E -= 0x155) & 0xFFF) / 341) % 12);
    eng->unk10.y -= dx;
    if (frame == 8) {
      fux->state = 0;
      if (eng->sfx != NULL) {
        StopSfx(eng->sfx);
      }
      eng->unk5C = 0;
      eng->unk2C.x = 0x1000;
      eng->unk2C.y = 0x1000;
      eng->unk2C.z = 0x1000;
    } else {
      eng->unk5C = 1;
      eng->unk2C.x = D_8009DC38[i];
      eng->unk2C.y = D_8009DC50[i];
      eng->unk2C.z = D_8009DC38[i];
    }
    who = PapuHitHeroesFlag(eng, 0x400);
    if (who != -1) {
      if ((Heroes[who].hero_immunity | Heroes[who].hero_hit) == 0) {
        Heroes[who].hero_move->reactive_angle = angle;
        Heroes[who].hero_move->reactive_speed = 0x30;
        HeroTakeHit(who, NULL, -1, 0x8005, 0x446);
      }
    }
    PapuHitSolids(eng, 2, 0x400, (u16)nas->UserInfo.user1);
    if (target_eng != NULL) {
      move->angle = angle;
    }
    move->speed = 0x20;
    break;
  case 7:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    hit = (s32)PapuMinionObjectAhead(slot, 0x180);
    if (hit != 0) {
      for (i = 0; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xFA00) &&
            angle_diff_abs(eng->unk1E, get_angle(eng->unk10.x - Heroes[i].hero_eng->unk10.x,
                                                eng->unk10.z - Heroes[i].hero_eng->unk10.z)) < level * 4 + 0x1C0) {
          GotoLinkAnim(anim, 0x10);
          fux->state = 6;
          break;
        }
      }
      do {
        if (fux->state != 6) {
          other = (MinionFux *)((NAS *)hit)->FUX;
          if (other != NULL) {
            other->unk1E |= 1;
            other->unk16 = PapuMinionSlots[slot].unk22;
            fux->target = NULL;
            fux->state = 8;
            GotoLinkAnim(anim, 7);
          }
        }
      } while (0);
    } else if (target_eng == NULL) {
      fux->state = 0;
      GotoLinkAnim(anim, 0);
    } else {
      move->angle = angle;
      move->speed = 0x20;
    }
    break;
  case 8:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    move->speed = 0;
    if (PapuMinionSlots[slot].pad4[6] != 0) {
      if (frame == 8) {
        if (((MinionFux *)PapuMinionSlots[slot].pad4[6])->unk14 == 5) {
          fux->unk20 = 0x5A - Random(level + 0xA);
        } else {
          fux->unk20 = Random(level * 3 + 0x96) + 0xB4;
        }
        pick = 0;
        if (*GameTeams != 0) {
          pick = Random(2);
          if (Heroes[pick].hero_status & 0xFA00) {
            pick = Heroes[pick].hero_teammate;
          }
          if (pick == -1) {
            who = -1;
            goto have;
          }
        }
        if (Heroes[pick].hero_status & 0xFA00) {
          pick = -1;
        }
        who = pick;
      have:
        if (who != -1) {
          fux->target = Heroes[who].hero;
          fux->state = 9;
          GotoLinkAnim(anim, 9);
        }
      }
    } else {
      fux->unk1E |= 2;
    }
    break;
  case 9:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    if (PapuMinionSlots[slot].pad4[6] == 0) {
      fux->unk1E |= 2;
      break;
    }
    hit = (s32)((NAS **)PapuMinionSlots[slot].pad4[6])[-1];
    if (hit == 0 || ((NAS *)hit)->Engine->platform != NULL) {
      fux->target = NULL;
      fux->state = 0xA;
      GotoLinkAnim(anim, 0xA);
    }
    if (fux->unk20 != 0) {
      fux->unk20--;
      if (frame == 8 && dist < 0x600 &&
          angle_diff_abs(angle, eng->unk1E) < (0x20 - level) * 4 + 0x40) {
        fux->target = NULL;
        fux->state = 0xA;
        GotoLinkAnim(anim, 0xA);
      }
      if (target_eng != NULL) {
        move->angle = angle;
        move->speed = 0x18;
        break;
      }
      fux->target = NULL;
      fux->state = 0xA;
      GotoLinkAnim(anim, 0xA);
      break;
    }
    fux->target = NULL;
    fux->state = 0xA;
    GotoLinkAnim(anim, 0xA);
    break;
  case 13:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    dx = PapuSpawnPoints[nas->UserInfo.user2].x - eng->unk10.x;
    dz = PapuSpawnPoints[nas->UserInfo.user2].z - eng->unk10.z;
    angle = get_angle(-dx, -dz);
    dist = csqrt(dx * dx + dz * dz) >> 6;
    if (dist < 0x200) {
      fux->state = 0;
    }
    move->angle = angle;
    move->speed = 0x20;
    break;
  case 10:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    move->speed = 0;
    if (frame == 8) {
      fux->state = 0;
    } else if (frame == 0x10) {
      if (PapuMinionSlots[slot].pad4[6] != 0) {
        ((MinionFux *)PapuMinionSlots[slot].pad4[6])->unk1E |= 4;
      }
      PapuMinionSlots[slot].pad4[6] = 0;
    }
    break;
  case 12:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    move->speed = 0;
    pick = (s32)PapuMinionSlots[slot].pad4[6];
    if (pick != 0) {
      ((MinionFux *)pick)->unk16 = fux->unk16;
    }
    PapuMinionSlots[slot].pad4[6] = 0;
    fux->state = 0xB;
    fux->unk1C &= 0xFEFD;
    fux->unk1E &= 0xFEFD;
    PapuMinionSlots[slot].unk26 = 0;
    GotoLinkAnim(anim, 0xD);
    break;
  case 11:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    move->speed = 0;
    PapuMinionSlots[slot].pad4[6] = 0;
    if (frame != 8) {
      if (frame == 9 && PapuMinionSlots[slot].unk26 != 0) {
        fux->state = 0;
      }
      break;
    }
    PapuMinionSlots[slot].unk0 = 0;
    PlaySfx(0x1FB, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    SpawnModelFx(0, &eng->unk10.x, (s32)&HeroModelBank[DB_Minion]);
    KillNAS(nas);
    return;
  default:
    eng->unk10.y -= ((s32 (*)(EXTengine *))ApplyContour)(eng);
    fux->unk1C |= 0x102;
    move->speed = 0;
    move->push = 0;
    fux->target = PapuMinionFindCrate(slot);
    if (fux->target != NULL) {
      GotoLinkAnim(anim, 1);
      fux->state = 7;
    } else {
      nas->UserInfo.user2 = PapuPickSpawnPoint((PapuPos *)eng);
      GotoLinkAnim(anim, 1);
      fux->state = 0xD;
    }
    break;
  }
  if (fux->state != 6) {
    s32 turn = angle_diff(eng->unk1E, move->angle) >> 2;
    if (turn != 0) {
      eng->unk1E += turn;
    } else {
      eng->unk1E = move->angle;
    }
  }
  eng->unk10.x -= (move->speed * rcossin_tbl[(move->angle - 0x400) & 0xFFF].unk2) >> 12;
  eng->unk10.z -= (move->speed * rcossin_tbl[(u16)move->angle & 0xFFF].unk2) >> 12;
  if (move->push != 0) {
    eng->unk10.x -= (move->push * rcossin_tbl[(move->push_angle - 0x400) & 0xFFF].unk2) >> 12;
    eng->unk10.z -= (move->push * rcossin_tbl[(u16)move->push_angle & 0xFFF].unk2) >> 12;
    move->push -= 4;
    if ((s16)move->push < 0) {
      move->push = 0;
    }
  }
  PapuMinionKick(slot);
  PAPU_KeepInBounds((u8 *)eng);
  if (fux->unk1E & 0x102) {
    if (PapuMinionSlots[slot].pad4[6] != 0) {
      ((MinionFux *)PapuMinionSlots[slot].pad4[6])->unk16 = fux->unk16;
    }
    PapuMinionSlots[slot].pad4[6] = 0;
    move->push = 0x40;
    move->push_angle = (move->angle + 0x800) & 0xFFF;
    fux->state = 0xB;
    fux->unk1C &= 0xFEFD;
    fux->unk1E &= 0xFEFD;
    PlaySfx(PapuMinionSfx[Random(4)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    GotoLinkAnim(anim, 0xD);
  }
  eng->unk5E = 1;
}

void PapuExploRange(B5E44Owner *arg0, s32 arg1, s32 arg2)
{
  B5E44Pos *me;
  s32 i;
  s32 px;
  s32 pz;
  s32 dx;
  s32 dz;
  s32 d;
  B5E44Pos *new_var;
  s32 ang;
  B5E44Slot *s;
  B5E44Flag *f;
  arg2 -= 0x14;
  if (((u32) arg2) >= 7U)
  {
    arg2 = -1;
  }
  me = arg0->pos;
  if (me != 0)
  {
    px = me->x;
    pz = me->z;
    for (i = 0; i < 4; i++)
    {
      s = &((B5E44Slot *) PapuMinionSlots)[i];
      if ((arg2 != i) && ((&((B5E44Slot *) PapuMinionSlots)[i])->obj != 0))
      {
        f = s->obj->flag;
        if ((f != 0) && (f->flags & 2))
        {
          dx = s->pos->x - px;
          new_var = s->pos;
          dz = new_var->z - pz;
          d = csqrt((dx * dx) + (dz * dz)) >> 6;
          if (d < arg1)
          {
            if (d < 0x80)
            {
              ang = s->dyn->unk16;
            }
            else
            {
              ang = get_angle(dx, dz);
            }
            s->dyn->unk2A = ang ^ 0x800;
            d = (arg1 - d) >> 3;
            (&((B5E44Slot *) PapuMinionSlots)[i])->dyn->unk22 = d;
            (&((B5E44Slot *) PapuMinionSlots)[i])->dyn->unk16 = (&((B5E44Slot *) PapuMinionSlots)[i])->dyn->unk2A + 0x800;
            PlaySfx(PapuMinionSfx[Random(4)], 0, (SLONG_XYZ *) (&me->x), 0x1000, 0x1E00, 0);
            f->unk1A = 0xC;
          }
        }
      }
    }

  }
}

void *PapuMinionObjectAhead(s32 arg0, s32 arg1) {
    s32 ang;
    u8 *n = HeadOfObjects;
    s32 lim = 0x10000;
    Slot40 *b = PapuMinionSlots;
    Slot40 *s = &b[arg0];
    u8 *o = (u8*)s->pad4[0];
    u8 *a = (u8*)s->pad4[4];
    s32 px = *(s32*)(o + 0x10) - (rcossin_tbl[(*(s16*)(a + 0x16) - 0x400) & 0xFFF].unk2 * 256 >> 12);
    s32 pz = *(s32*)(o + 0x18) - (rcossin_tbl[*(u16*)(a + 0x16) & 0xFFF].unk2 * 256 >> 12);
    Slot40 *q;

    if (n != NULL) {
        ang = arg1 & 0xFFFF;
        q = s;
        do {
            u8 *f = *(u8**)(n + 0x58);
            if (f != NULL && (*(u16*)(f + 0x1C) & 1)) {
                u8 *e = *(u8**)(n + 0x54);
                if (e != NULL && *(s32*)(e + 0x80) == 0) {
                    s32 ex = *(s32*)(e + 0x10);
                    s32 ez = *(s32*)(e + 0x18);
                    s32 dx = ex - px;
                    s32 dz = ez - pz;
                    if (dx * dx + dz * dz < lim) {
                        if (ang == 0) {
                            return n;
                        }
                        if (angle_diff_abs(*(s16*)((u8*)q->pad4[4] + 0x16),
                                          get_angle(ex - *(s32*)(o + 0x10), ez - *(s32*)(o + 0x18)) ^ 0x800) < ang) {
                            return n;
                        }
                    }
                }
            }
            n = *(u8**)(n + 0x2C);
        } while (n != NULL);
    }
    return NULL;
}

s32 PapuHitHeroes(EXTengine *eng) {
    VECTOR pos;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 lo;
    s32 hi;

    box = FindCollisionBox(eng, 0x400);
    if (box != NULL) {
        if (eng->unk5E != 0) {
            rotscale_update(&eng->unk1C);
        }
        gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
        r = box->radius;
        top = eng->unk10.y + box->unk2;
        bot = eng->unk10.y + box->height;
        gte_rotate_point((SVECTOR *)box, &pos);
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_status & 0xF800) {
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
            lo = eng->unk10.y + rec->unk2;
            hi = eng->unk10.y + rec->height;
            if (bot < lo && hi < top) {
                dx = eng->unk10.x - pos.vx;
                dz = eng->unk10.z - pos.vz;
                d = dx * dx + dz * dz;
                if (d < (rec->radius + r) * (rec->radius + r)) {
                    HeroTakeHit(i, NULL, -1, 0x8008, 0x446);
                }
            }
        }
    }
    return -1;
}

s32 PapuHitHeroesFlag(EXTengine *eng, u32 flag) {
    VECTOR pos;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 lo;
    s32 hi;

    box = FindCollisionBox(eng, flag);
    if (box == NULL) {
        return -1;
    }
    if (eng->unk5E != 0) {
        rotscale_update(&eng->unk1C);
    }
    gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    gte_rotate_point((SVECTOR *)box, &pos);
    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_status & 0xF800) {
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
        lo = eng->unk10.y + rec->unk2;
        hi = eng->unk10.y + rec->height;
        if (bot < lo && hi < top) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            d = dx * dx + dz * dz;
            if (d < (rec->radius + r) * (rec->radius + r)) {
                return i;
            }
        }
    }
    return -1;
}

NAS *PapuHitSolids(EXTengine *eng, u32 mask, s32 unused, s32 add) {
    VECTOR pos;
    FlagRecord *new_var;
    FlagRecord *box;
    FlagRecord *rec;
    NAS *n;
    u8 *x;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 lo;
    EXTengine *o;
    s32 hi;

    box = FindCollisionBox(eng, 0x400);
    if (box != NULL) {
        if (eng->unk5E != 0) {
            rotscale_update(&eng->unk1C);
        }
        gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
        r = box->radius;
        new_var = box;
        top = eng->unk10.y + new_var->unk2;
        bot = eng->unk10.y + new_var->height;
        gte_rotate_point((SVECTOR *)new_var, &pos);
        for (n = HeadOfSolids; n != NULL; n = n->Solid.Child) {
            if (!(n->Class & 0x200)) {
                continue;
            }
            o = n->Engine;
            if (o == NULL || o == eng) {
                continue;
            }
            rec = FindCollisionBox(o, 0x40);
            if (rec == NULL) {
                continue;
            }
            lo = o->unk10.y + rec->unk2;
            hi = o->unk10.y + rec->height;
            if (bot < lo && hi < top) {
                dx = o->unk10.x - pos.vx;
                unused = o->unk10.z - pos.vz;
                if (dx * dx + unused * unused < (rec->radius + r) * (rec->radius + r)) {
                    x = FindExtension(n, 0x1000);
                    if (x != NULL && (*(u16 *)(x + 0x1C) & mask)) {
                        *(s16 *)(x + 0x16) = add + 0x14;
                        *(u16 *)(x + 0x1E) |= mask;
                        return n;
                    }
                }
            }
        }
    }
    return NULL;
}

NAS *PapuHitObjects(EXTengine *eng, s32 mask) {
    VECTOR pos;
    FlagRecord *box;
    FlagRecord *rec;
    EXTengine *e;
    EXTobject *obj;
    NAS *nas;
    s32 r;
    s32 top;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 rr;
    s32 lo;
    s32 hi;

    box = FindCollisionBox(eng, 0x40);
    if (box != NULL) {
    if (eng->unk5E != 0) {
        rotscale_update(&eng->unk1C);
    }
    gfx_load_xform(eng->sub, eng->unk3C, &eng->unk10);
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    gte_rotate_point((SVECTOR *)box, &pos);
    for (nas = HeadOfSolids; nas != NULL; nas = nas->Solid.Child) {
        if (nas->Class & 0x200) {
            e = nas->Engine;
            if (e != NULL && e != eng) {
                rec = FindCollisionBox(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (bot < lo && hi < top) {
                        dx = e->unk10.x - pos.vx;
                        dz = e->unk10.z - pos.vz;
                        rr = rec->radius + r;
                        if (dx * dx + dz * dz < rr * rr) {
                            obj = FindExtension(nas, 0x1000);
                            if (obj != NULL && (obj->can_do & mask)) {
                                obj->player = 3;
                                obj->will_do |= mask;
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

s32 PapuMinionKick(u32 idx) {
    s32 hit;
    s32 bot;
    s32 top;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *box;
    FlagRecord *rec;
    HeroInfo *h;
    NAS *nas;
    NAS *self;
    EXTobject *obj;
    s32 r;
    s32 can;
    s32 rr;
    s32 rr2;
    s32 dx;
    s32 dz;
    s32 d2;
    s32 d;
    s32 ang;
    s32 lo;
    s32 hi;
    s32 *p;
    s32 i;
    Slot40 *slot;

    hit = 0;
    if (idx >= 6) {
        return 0;
    }
    eng = (EXTengine *)PapuMinionSlots[idx].pad4[0];
    if (eng != NULL && (box = FindCollisionBox(eng, 0x40)) != NULL) {
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    for (i = 0; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xF800)) {
            if (((s16 *)PapuMinionSlots[idx].pad4[4])[7] == 0) {
                can = Heroes[i].hero_move->forward_speed == 0;
            } else {
                can = 1;
            }
            e = Heroes[i].hero_eng;
            if (e != NULL) {
                rec = FindCollisionBox(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (can) {
                        rr = rec->radius;
                        rr += r;
                        rr2 = rr * rr;
                        if (bot < lo && hi < top) {
                            dx = e->unk10.x - eng->unk10.x;
                            dz = e->unk10.z - eng->unk10.z;
                            d2 = dx * dx + dz * dz;
                            if (d2 < rr2) {
                                d2 = csqrt(d2) >> 6;
                                if (d2 < rr) {
                                    ang = get_angle(dx, dz) & 0xFFF;
                                    d2 = rr - d2 + 1;
                                    eng->unk10.x -= d2 * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
                                    hit = 1;
                                    eng->unk10.z -= d2 * rcossin_tbl[ang].unk2 >> 12;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    nas = HeadOfSolids;
    p = (s32 *)PapuMinionSlots[idx].pad4[6];
    slot = &PapuMinionSlots[idx];
    self = NULL;
    if (p != NULL) {
        self = (NAS *)p[-1];
    }
    can = ((s16 *)slot->pad4[4])[7] != 0;
    r = 0x78;
    for (; nas != NULL; nas = nas->Solid.Child) {
        if ((nas->Class & 0x200) && nas != self) {
            e = nas->Engine;
            if (e != NULL && e != eng) {
                rec = FindCollisionBox(e, 0x40);
                if (rec != NULL) {
                    lo = e->unk10.y + rec->unk2;
                    hi = e->unk10.y + rec->height;
                    if (bot < lo && hi < top) {
                        dx = e->unk10.x - eng->unk10.x;
                        dz = e->unk10.z - eng->unk10.z;
                        rr = rec->radius;
                        rr += r;
                        d2 = dx * dx + dz * dz;
                        rr2 = rr * rr;
                        if (d2 < rr2) {
                            d2 = csqrt(d2) >> 6;
                            if (d2 < rr) {
                                hit = 1;
                                ang = get_angle(dx, dz) & 0xFFF;
                                if (!(ang & 0xFF)) {
                                    ang += Random(0x10) - 8;
                                }
                                d2 = rr - d2 + 1;
                                if (can) {
                                    eng->unk10.x -= d2 * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
                                    eng->unk10.z -= d2 * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
                                }
                                obj = FindExtension(nas, 0x1000);
                                if (obj != NULL && (obj->can_do & 8)) {
                                    obj->player = idx;
                                    obj->will_do |= 8;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    }
    return hit;
}

void PapuInitTiles(void) {
    PapuSceneObj *obj;
    u16 *id;
    PapuEntry *p;
    s32 i;
    s32 j;
    s32 dx;
    s32 dz;

    obj = *(PapuSceneObj **)((u8 *)MapScene + 0x1C);
    primpool_init(0xC8);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            PapuTiles[j][i].tile = NULL;
        }
    }
    for (j = 0; j < 6; j++) {
        PapuColumns[j].unk0 = 0;
        PapuColumns[j].unk6 = 0;
        PapuColumns[j].unkA = 0;
        PapuColumns[j].unkC = 0;
        PapuColumns[j].unkE = 0;
        PapuColumns[j].unk8 = 0;
    }
    PapuFloorObj = NULL;
    while (obj != NULL) {
        id = (u16 *)model_entry_data(obj->model, obj->index);
        if ((u32)(*id - 10) < 78) {
            i = 7 - (s16)((s16)*id % 10);
            j = (s16)((s16)*id / 10) - 1;
            PapuTiles[j][i].tile = (PapuTile *)obj;
            sub_use_quad_model((u8 *)obj);
            obj->flags |= 0x1000000;
        }
        j = *(s16 *)id - 100;
        if ((u32)j < 20) {
            p = &PapuColumns[j];
            p->unk0 = (s32)obj;
            p->unk8 = obj->unk8;
        }
        if (j == 50) {
            PapuFloorObj = obj;
            obj->unk50 = 1;
            obj->unk51 = 0;
            obj->flags &= ~0x8000;
        }
        obj = obj->next;
    }
    for (j = 39; j >= 0; j--) {
        PapuFloorState[j] = 0;
    }
    PapuFloorDepth = 0;
    PapuUnused4 = 0;
    PapuUnused5 = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            obj = (PapuSceneObj *)PapuTiles[j][i].tile;
            if (obj != NULL) {
                PapuTiles[j][i].unkA = 0;
                PapuTiles[j][i].unk10 = obj->index;
                PapuTiles[j][i].pad8 = 0;
                PapuTiles[j][i].unk12 = obj->unk8;
                PapuTiles[j][i].unkE = 0;
                dx = obj->x;
                dz = obj->z - 0x600;
                PapuTiles[j][i].unkC = csqrt(dx * dx + dz * dz) >> 6;
            }
        }
    }
}

void PapuFloorWave(void) {
    PapuKey *k;
    s32 first;
    s32 v;
    s32 t;

    first = 1;
    if (PapuFloorWaveCur != NULL && PapuFloorWaveCur->b != 0) {
        k = PapuFloorWaveCur;
        do {
            if (k->g != 0) {
                t = k->f;
                if (t != 0) {
                    v = t - 1;
                } else {
                    if (first) {
                        PlaySfx(0x1F6, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    first = 0;
                    if (k->g != 0) {
                        PapuTiles[(s16)k->d][(s16)k->e].unkE = 1;
                    }
                    k->d += k->pad8[0];
                    k->e += k->pad8[1];
                    if (k->d >= 7U) {
                        k->g = 0;
                    }
                    if ((u16)(k->e - 1) >= 7U) {
                        k->g = 0;
                    }
                    v = k->pad4;
                }
                k->f = v;
            }
            k++;
        } while (k->b != 0);
    }
}

void PapuUpdateFloor(void) {
    s32 i;
    s32 j;
    s32 m;
    u32 a;
    s32 w;
    u32 u;
    u32 k;
    PapuTile *tile;

    if (PapuFloorObj != NULL) {
        if (PapuFloorTimer == 0) {
            if (PapuFloorSink != 0) {
                PapuFloorSink -= 0x140;
                if (PapuFloorSink < 0) {
                    PapuFloorSink = 0;
                }
                if (PapuFloorSink < 0x200 && PapuSfxEngine.sfx != NULL) {
                    StopSfx(PapuSfxEngine.sfx);
                }
                PapuFloorObj->unk24 = PapuFloorSink;
            } else {
                PapuFloorObj->flags &= ~0x8000;
            }
        } else {
            if (PapuFloorSink == 0) {
                PlaySfx(0x20D, &PapuSfxEngine.sfx, NULL, 0x1000, 0x1E00, 0);
            }
            if (PapuFloorSink < 0x1000) {
                PapuFloorSink += 0x80;
                if (PapuFloorSink > 0x1000) {
                    PapuFloorSink = 0x1000;
                }
                PapuFloorObj->unk24 = PapuFloorSink;
            }
            PapuFloorObj->flags |= 0x8000;
        }
        PapuFloorObj->unk52 = 1;
    }
    PapuFloorWave();
    PapuMoveColumns();
    m = (u16)PapuWaveFlag;
    if (m < 3 && m != 0) {
        if (PapuFloorState[0] != 0) {
            PapuFloorState[0] -= 2;
        }
        j = 0x27;
        do {
            PapuFloorState[j] = PapuFloorState[j - 1];
            j--;
        } while (j > 0);
        PapuFloorDepth += 0x20;
        for (i = 1; i < 8; i++) {
            j = 0;
            for (; j < 7; j++) {
                tile = PapuTiles[j][i].tile;
                if (tile != NULL) {
                    a = PapuTiles[j][i].unkC;
                    u = PapuFloorDepth + a;
                    a >>= 7;
                    w = PapuFloorState[a];
                    tile->unk8 = PapuTiles[j][i].unk12;
                    k = ((u & 0xFFF) - 0x400) & 0xFFF;
                    tile->unk8 += (w * rcossin_tbl[k].unk2) >> 12;
                    if (PapuTiles[j][i].unkE != 0) {
                        PapuTiles[j][i].unkA |= 0x100;
                        tile->unk74 = PapuTileModels[PapuTiles[j][i].unkE];
                        PapuTiles[j][i].unkE++;
                        if (PapuTileModels[PapuTiles[j][i].unkE] == 0) {
                            PapuTiles[j][i].unkE = 0;
                        }
                    } else {
                        tile->unk74 = PapuTiles[j][i].unk10;
                        PapuTiles[j][i].unkA &= ~0x100;
                    }
                }
            }
        }
    }
}

s32 PapuHeroOverPit(s32 arg0) {
    EXTengine *eng;
    s32 ix;
    s32 iz;
    s32 y;
    PapuTile *t;

    eng = Heroes[arg0].hero_eng;
    ix = (eng->unk10.x + 0x700) >> 9;
    iz = (eng->unk10.z + 0x700) >> 9;
    if ((u32)ix < 7U && (u32)iz < 7U) {
        y = eng->unk10.y - World_floor_level;
        t = PapuTiles[ix][iz].tile;
        if (t != NULL) {
            y -= t->unk8;
        }
        if (abs(y) < 0x20 && (PapuTiles[ix][iz + 1].unkA & 0x100)) {
            return 1;
        }
    }
    return 0;
}

s32 GetPapuContour(EXTengine *eng) {
    s32 y;
    s32 ix;
    s32 iz;
    PapuTile *t;

    y = eng->unk10.y - World_floor_level;
    ix = (eng->unk10.x + 0x700) >> 9;
    iz = (eng->unk10.z + 0x700) >> 9;
    if ((u32)ix >= 7U || (u32)iz >= 7U) {
        return y - 0x800;
    }
    t = PapuTiles[ix][iz].tile;
    if (t != NULL) {
        y -= t->unk8;
    }
    return AddPlatformContour(eng, y);
}

void PapuStartFloorWave(s32 arg0) {
    PapuKey *src;
    PapuKey *dst;

    src = PapuFloorWaves[arg0];
    PapuFloorWaveCur = src;
    if (src->b != 0) {
        dst = src;
        do {
            dst->d = src->a;
            dst->e = dst->b;
            dst->f = dst->c;
            dst->g = 1;
            dst++;
            src++;
        } while (dst->b != 0);
    }
    PapuWaveFlag = 2;
}

s32 PAPU_KeepInBounds(u8 *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    s32 var_a2;

    var_a2 = 0;
    if ((*(s32*)((u8*)(arg0) + 144)) & 0x40) {
        temp_a0 = 0x6A0 - (*(s16*)((u8*)(arg0) + 138));
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 16))) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_a0;
            var_a2 = 1;
        }
        temp_v1 = -temp_a0;
        if ((*(s32*)((u8*)(arg0) + 16)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 16)) = temp_v1;
            var_a2 = 1;
        }
        if (temp_a0 < (*(s32*)((u8*)(arg0) + 24))) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_a0;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < temp_v1) {
            (*(s32*)((u8*)(arg0) + 24)) = temp_v1;
            goto block_17;
        }
    } else {
        if ((*(s32*)((u8*)(arg0) + 16)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 16)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 16)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 16)) = -0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) >= 0x601) {
            (*(s32*)((u8*)(arg0) + 24)) = 0x600;
            var_a2 = 1;
        }
        if ((*(s32*)((u8*)(arg0) + 24)) < -0x600) {
            (*(s32*)((u8*)(arg0) + 24)) = -0x600;
block_17:
            var_a2 = 1;
        }
    }
    return var_a2;
}

void PapuStopCrates(void) {
    u8 *temp_v1;
    u8 *var_a0;

    var_a0 = HeadOfObjects;
    if (var_a0 != NULL) {
        do {
            temp_v1 = (*(s32*)((u8*)(var_a0) + 88));
            if (temp_v1 != NULL) {
                if ((*(u16*)((u8*)(temp_v1) + 26)) == 1) {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 4U;
                } else {
                    (*(u16*)((u8*)(temp_v1) + 26)) = 5U;
                }
            }
            var_a0 = (*(s32*)((u8*)(var_a0) + 44));
        } while (var_a0 != NULL);
    }
}

s32 PapuPickTarget(void) {
    s32 a;

    if (*GameTeams != 0) {
        a = Random(2);
        if (Heroes[a].hero_status & 0xFA00) {
            a = Heroes[a].hero_teammate;
        }
        if (a == -1) {
            return -1;
        }
    } else {
        a = 0;
    }
    if (Heroes[a].hero_status & 0xFA00) {
        a = -1;
    }
    return a;
}

void PapuSpawnMinions(u16 arg0) {
    PapuStep *s = PapuMinionWaves[arg0];

    while (s->d >= 0) {
        CreatePapuMinion(s->a, s->b, s->c, s->d);
        s++;
    }
}

void PapuHeroCheckPit(s32 arg0) {
    if (PapuHeroOverPit(arg0) != 0) {
        HeroTakeHit(arg0, 0, -1, 3, 0);
    }
}

s32 PapuFreeMinionSlot(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (PapuMinionSlots[i].unk0 == 0) {
            return i;
        }
    }
    return -1;
}

void PlotPapuColumn(NAS *nas) {
    EXTengine *e = nas->Engine;
    EXTengine *other;
    s32 lim;
    s32 d;
    s32 cur;

    if (e != NULL && nas->UserInfo.user1 != 0) {
        cur = e->unk10.y + 0x140;
        e->unk10.y = cur;
        lim = D_8005B2EC;
        if (nas->Family.Parent != NULL) {
            other = nas->Family.Parent->Engine;
            if (other != NULL) {
                lim = other->unk10.y;
                d = lim - cur;
                if (d < 0) {
                    d = 0;
                }
                if (d < 0x200) {
                    other->unk2C.y = d * 8;
                    BossState.flags |= 0x2000;
                }
            }
        }
        if (lim < e->unk10.y) {
            PlaySfx(0x20C, NULL, NULL, 0x1000, 0x1E00, 0);
            BossState.health = 0;
            BossState.flags |= 0x8000;
            e->unk10.y = lim;
            nas->UserInfo.user1 = 0;
        }
        e->unk5E = 1;
    }
}

void InitPapuColumn(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = *(GfxModel **)MapScene;
        e->sub->entity = 0x200C;
        e->unk5D = 0;
        e->unk5C = 1;
    }
    nas->UserInfo.user1 = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PapuDropColumns(void) {
    s32 i;
    PapuEntry *p;
    s16 one;

    i = 0;
    one = 1;
    for (p = PapuColumns; i < 20; i++, p++) {
        if (p->unk0 != 0) {
            p->unk6 = one;
        }
    }
}

void PapuMarkHoles(void) {
    s32 i, j;

    for (j = 2; j < 18; j++) {
        for (i = 2; i < 18; i++) {
            if (PapuTiles[(j - 2) >> 1][7 - ((i - 2) >> 1)].unkA == 0) {
                ArenaGrid[i * 20 + j].unk0 = 0xFF;
            }
        }
    }
}

void PapuDropFirstCrates(void)
{
  s32 i;
  s32 *p;
  s32 a;
  s32 b;
  s32 type;
  s32 n;
  short t;
  n = PapuFirstCrates;
  for (i = 0;; i++)
  {
    if (i >= n)
    {
      goto done;
    }
    p = (s32 *) PapuFindDropPoint(&PapuDropPos);
    if (p != 0)
    {
      a = p[0];
      b = p[1];
      t = PapuCrateMix[PapuCrateMixPos];
      PapuCrateMixPos = (PapuCrateMixPos + 1) & 0x3F;
      type = t;
      if ((type == 6) && (CrateCounts[6] >= 4))
      {
        type = 4;
      }
      CreatePACrate(a, 0, b, type, -1);
    }
  }

  done:
  ;

  ;
}

void PapuDropCrates(u16 arg0) {
    s32 i;
    PapuMsg *m;
    s32 v;
    short t;
    int a;
    s32 b;

    for (i = 0;; i++) {
        if (i >= (arg0 & 0xFFFF)) {
            return;
        }
        m = PapuFindDropPoint(&PapuDropPos);
        if (m != NULL) {
            a = m->unk0;
            b = m->unk4;
            t = PapuCrateMix[PapuCrateMixPos];
            PapuCrateMixPos = (PapuCrateMixPos + 1) & 0x3F;
            v = t;
            if (v == 6 && CrateCounts[6] >= 4) {
                v = 4;
            }
            CreatePACrate(a, 0, b, v, -1);        }
    }
}

void FirstTriggersPAPU(void) {
}

void PapuRestartLevel(void) {
}

void UpdatePAPU(void) {
    ArenaGridClear();
    PapuCountObjects();
    MarkHeroesOnGrid();
    PapuCrateMaster();
    PapuUpdateFloor();
}

void PAPUSetUp(void) {
    LevelSpecs.LevelPreInit = InitialisePAPUFirst;
    LevelSpecs.LevelInit = InitialisePAPU;
    LevelSpecs.LevelRestartInit = InitialisePAPU;
    LevelSpecs.LevelRoutine = UpdatePAPU;
    LevelSpecs.EarliestTriggers = FirstTriggersPAPU;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.GetContour = GetPapuContour;
}

void PapuInitCrateHooks(void) {
    CrateAstType = PapuASTIndex(0x203);
    CrateAstType2 = PapuASTIndex(0x204);
    HitHooks.unk4 = D_8009E3B4;
    HitHooks.unk8 = NULL;
    HitHooks.unkC = 0;
    HitHooks.unk0 = D_8009E3DC;
}

void PACrateAimAtGrid(T_B7F14_A *a, T_B7F14_B *b) {
    s32 z;
    s32 x;
    s32 sum = 0;
    s32 v = b->unkE;
    s32 d = b->unk10 - b->unk12;
    s32 mx, mz;

    if (v != 0) {
        do {
            v += d;
            if (v < 0) {
                v = 0;
            }
            sum += v;
        } while (v != 0);
    }
    x = a->x;
    mx = sum * rcossin_tbl[(a->ang - 0x400) & 0xFFF].unk2;
    mz = sum * rcossin_tbl[(u16)a->ang & 0xFFF].unk2;
    z = a->z;
    x -= mx >> 12;
    z -= mz >> 12;
    x &= ~0xFF;
    x += 0x80;
    z &= ~0xFF;
    z += 0x80;
    b->unk16 = x;
    b->unk2A = z;
    b->dir = DirPadBits[((u16)a->ang >> 7) & 0x1C];
    x -= a->x;
    z -= a->z;
    sum = csqrt(x * x + z * z) >> 6;
    if (sum > 0) {
        v -= d;
        do {
            sum -= v;
            v -= d;
        } while (sum > 0);
        v += d;
    }
    v -= d >> 1;
    b->unkE = v;
}

s32 PapuGridKey(T_B8080 *p) {
    s32 a, b, c, d;
    b = (p->x >> 8) + 8;
    a = 7 - (p->y >> 8);
    c = (s8)(p->h >> 8);
    d = c << 16;
    if (!(((u16)p->h) >> 16)) {
        return c;
    }
    return a + b + d;
}

void PapuCountObjects(void) {
    s32 i;
    s32 x, z;
    u16 t;
    u8 c;
    u16 *p;
    s32 j;
    NAS *n;
    EXTengine *e;
    T_B80B0Fux *f;

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
    n = HeadOfObjects;
    for (;;) {
        if (n == NULL) return;
        e = n->Engine;
        if (e != NULL) {
        f = (T_B80B0Fux *)n->FUX;
        if (f != NULL) {
            t = f->type;
            x = (e->unk10.x >> 8) + 10;
            z = 9 - (e->unk10.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            ArenaGrid[z * 20 + x].unk0 = t;
            if ((u16)(f->type - 1) < 12) {
                CrateCounts[0]++;
                CrateCounts[f->type]++;
            } else if ((u16)(f->type - 0x11) < 12) {
                PickupCounts[0]++;
                PickupCounts[f->type - 0x10]++;
                if ((u16)(f->type - 0x12) < 4) {
                    CrateCounts[1]++;
                }
            }
        } else {
            c = 0xFF;
            x = (e->unk10.x >> 8) + 10;
            z = 9 - (e->unk10.z >> 8);
            if (x >= 20) x = 19;
            if (x < 0) x = 0;
            if (z >= 20) z = 19;
            if (z < 0) z = 0;
            ArenaGrid[z * 20 + x].unk0 = c;
        }
        }
        n = n->Object.Child;
    }
}

NAS *CreatePACrate(s32 a, s32 b, s32 c, s32 d, s32 e) {
    NAS *nas;
    PapuObj *obj;
    EXTengine *eng;
    s32 x, z;

    u16 k;
    SLONG_XYZ *p;
    s32 t;

    nas = CreateNAS(a, b, c, 0x204, 0x11);
    k = d;
    if (nas != NULL) {
        eng = nas->Engine;
        if (eng != NULL) {
            obj = (PapuObj *)AddObject(nas);
            if (obj != NULL && AddMotion(nas) != NULL) {
                LinkObject((u8 *)nas);
                AddSolid(nas, 0x300);
                nas->FUX = (EXT *)obj;
                eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
                eng->unk5D = 1;
                obj->unk14 = d;
                CrateCounts[obj->unk14]++;
                obj->unk1A = 7;
                switch (k) {
                case 4:
                    eng->sub->entity = 0x2013;
                    break;
                case 5:
                    eng->sub->entity = 0x2009;
                    obj->unk22 = 0x5A;
                    obj->unk1C |= 0x100;
                    break;
                default:
                    eng->sub->entity = 0x2013;
                    break;
                }
                eng->unk2C.x = 0x40;
                eng->unk2C.y = 0x40;
                eng->unk2C.z = 0x40;
                eng->unk5D = 1;
                eng->unk5C = 1;
                eng->unk5E = 1;
                eng->flags |= 1;
                eng->unk10.y = World_floor_level;
                p = &eng->unk10;
                PlaySfx(0x3FA, NULL, p, 0x1000, 0x1E00, 0);
                t = k;
                x = (eng->unk10.x >> 8) + 10;
                z = 9 - (p->z >> 8);
                if (x >= 20) x = 19;
                if (x < 0) x = 0;
                if (z >= 20) z = 19;
                if (z < 0) z = 0;
                ArenaGrid[z * 20 + x].unk0 = t;
            }
        }
        if (nas->Failure != 0) {
            KillNAS(nas);
            return NULL;
        }
    }
    return nas;
}

void InitPACrate(NAS *arg0) {
    EXTengine *eng;
    PapuObj *obj;
    NAS *nas;
    EXTengine *other;
    s32 again;
    s32 x, z;

    eng = arg0->Engine;
    if (eng != NULL) {
        obj = (PapuObj *)AddObject(arg0);
        if (obj != NULL) {
            arg0->FUX = (EXT *)obj;
            do {
                again = 0;
                nas = HeadOfSolids;
                eng->unk10.x = (eng->unk10.x & ~0xFF) + 0x80;
                eng->unk10.z = (eng->unk10.z & ~0xFF) + 0x80;
                while (nas != NULL) {
                    if (nas->Type == CrateAstType) {
                        other = nas->Engine;
                        if (other != NULL && other->unk10.x == eng->unk10.x && other->unk10.z == eng->unk10.z) {
                            eng->unk10.x = Random(0xC00) - 0x600;
                            eng->unk10.z = Random(0xC00) - 0x600;
                            nas = NULL;
                            again = 1;
                        }
                    }
                    if (nas != NULL) {
                        nas = nas->Solid.Child;
                    }
                }
            } while (again);
            eng->unk10.y = World_floor_level;
            eng->sub->u6C.dbase = &HeroModelBank[DB_CrateObjects];
            eng->unk5D = 1;
            eng->unk5C = 1;
            obj->unk1A = 3;
            if (arg0->trigger != NULL) {
                obj->unk14 = arg0->trigger->InfoStruct->val1;
            }
            CrateCounts[obj->unk14]++;
            switch (obj->unk14) {
            case 4:
                eng->sub->entity = 0x2013;
                break;
            case 5:
                eng->sub->entity = 0x2009;
                obj->unk22 = 0x5A;
                obj->unk1C |= 0x100;
                break;
            default:
                eng->sub->entity = 0x2013;
                break;
            }
            x = (eng->unk10.x >> 8) + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x >= 20) {
                x = 19;
            }
            if (x < 0) {
                x = 0;
            }
            if (z >= 20) {
                z = 19;
            }
            if (z < 0) {
                z = 0;
            }
            ArenaGrid[z * 20 + x].unk0 = obj->unk14;
            AddMotion(arg0);
            LinkObject((u8 *)arg0);
            AddSolid(arg0, 0x300);
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}
