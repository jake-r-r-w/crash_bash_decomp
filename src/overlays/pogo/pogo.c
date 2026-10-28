#include "common.h"
#include "bash/main.h"
#include "bash/pogo_ovl.h"
#include "bash/pogo.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char PogoAnimBreathe[] = "BREATHE";
const char PogoAnimJump[] = "JUMP";
const char PogoAnimDazed[] = "DAZED";
const char PogoAnimWin[] = "WIN";
const char PogoAnimTazing[] = "TAZING";
const char PogoEmptyString[] = "";

/* data, in address order */
PogoModeEntry PogoPainterAiSkill = { { 9, 0, 6, 0, 3, 0, 1, 0, 8, 0, 0x1A, 0, 0xC, 0 }, 0x10, 0x10, { 0x1C, 0 } };
PogoModeEntry PogoPainterAiSkillTeams = { { 5, 0, 2, 0, 1, 0, 1, 0, 8, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
u16 PogoPainterAiTeamSkill[6] = { 1, 0xC, 0x14, 1, 0xC, 0x14 };
u16 PogoPainterAiQuality[6] = { 1, 0xC, 0x14, 1, 0xC, 0x14 };
u16 PogoPainterAiRound[12] = { 9, 9, 9, 6, 6, 6, 3, 3, 3, 1, 1, 1 };
u16 PogoPainterAiRoundAlt[12] = { 5, 5, 5, 2, 2, 2, 1, 1, 1, 1, 1, 1 };
PogoModeEntry PogoPainterAiHuman = { { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0x1A, 0, 0xC, 0 }, 0x10, 0x10, { 0x1C, 0 } };
PogoModeEntry PogoPainterAiHumanAlt = { { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoAiBotTweak = { { 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0, 0x10, 0 }, 0x10, 0x10, { 2, 0 } };
PogoModeEntry PogoAgogoAiSkill = { { 9, 0, 6, 0, 3, 0, 1, 0, 5, 0, 0x20, 0, 0x14, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoAgogoAiSkillTeams = { { 0xD, 0, 9, 0, 5, 0, 1, 0, 9, 0, 0x20, 0, 0x18, 0 }, 0x10, 0x10, { 0x20, 0 } };
u16 PogoAgogoAiTeamSkill[6] = { 1, 0x10, 0x18, 1, 0x10, 0x18 };
u16 PogoAgogoAiQuality[6] = { 1, 0x10, 0x18, 1, 0x10, 0x18 };
u16 PogoAgogoAiRound[12] = { 9, 9, 9, 6, 6, 6, 3, 3, 3, 1, 1, 1 };
u16 PogoAgogoAiRoundAlt[12] = { 0xD, 0xD, 0xD, 9, 9, 9, 5, 5, 5, 1, 1, 1 };
PogoModeEntry PogoAgogoAiHuman = { { 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0x20, 0, 0x14, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoAgogoAiHumanAlt = { { 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0x20, 0, 0x18, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoLocoAiSkill = { { 0x1B, 0, 0x18, 0, 0x15, 0, 0x12, 0, 0x10, 0, 0x1C, 0, 1, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoLocoAiSkillTeams = { { 0x1C, 0, 0x18, 0, 0x14, 0, 0x10, 0, 0x14, 0, 0x1C, 0, 1, 0 }, 0x10, 0x10, { 0x20, 0 } };
u16 PogoLocoAiTeamSkill[6] = { 1, 0x10, 0x1C, 1, 0x10, 0x1C };
u16 PogoLocoAiQuality[6] = { 1, 0x10, 0x1C, 1, 0x10, 0x1C };
u16 PogoLocoAiRound[12] = { 0x1B, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x15, 0x15, 0x15, 0x12, 0x12, 0x12 };
u16 PogoLocoAiRoundAlt[12] = { 0x1C, 0x1C, 0x1C, 0x18, 0x18, 0x18, 0x14, 0x14, 0x14, 0x10, 0x10, 0x10 };
PogoModeEntry PogoLocoAiHuman = { { 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0, 0x1C, 0, 1, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoLocoAiHumanAlt = { { 0, 0, 0, 0, 0, 0, 0, 0, 0x14, 0, 0x1C, 0, 1, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoBouncerAiSkill = { { 0x14, 0, 0x11, 0, 0xE, 0, 0xB, 0, 0xA, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoBouncerAiSkillTeams = { { 0x17, 0, 0x14, 0, 0x11, 0, 0xE, 0, 0xA, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
u16 PogoBouncerAiTeamSkill[6] = { 1, 0x10, 0x18, 1, 0x10, 0x18 };
u16 PogoBouncerAiQuality[6] = { 1, 0x10, 0x18, 1, 0x10, 0x18 };
u16 PogoBouncerAiRound[12] = { 0x14, 0x14, 0x14, 0x11, 0x11, 0x11, 0xE, 0xE, 0xE, 0xB, 0xB, 0xB };
u16 PogoBouncerAiRoundAlt[12] = { 0x17, 0x17, 0x17, 0x14, 0x14, 0x14, 0x11, 0x11, 0x11, 0xE, 0xE, 0xE };
PogoModeEntry PogoBouncerAiHuman = { { 0, 0, 0, 0, 0, 0, 0, 0, 0xA, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
PogoModeEntry PogoBouncerAiHumanAlt = { { 0, 0, 0, 0, 0, 0, 0, 0, 0xA, 0, 0x1C, 0, 0xA, 0 }, 0x10, 0x10, { 0x20, 0 } };
s32 PowerupsWanted[4] = { 2, 2, 1, 1 };
s32 PowerupsMax[4] = { 3, 3, 1, 2 };
s32 PickupsWanted[4] = { 2, 2, 2, 0 };
s32 PickupsMax[4] = { 2, 2, 2, 0 };
s32 PogoObjectTimes[40] = {
    0x60, 0x5D, 0x5A, 0x57, 0x54, 0x51, 0x4E, 0x4B, 0x48, 0x45, 0x42, 0x3F, 0x3C, 0x39, 0x36,
    0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0xF, 0xC, 9, 6, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
};
u16 TileLockTimes[40][2] = {
    { 0x28, 0 }, { 0x27, 0 }, { 0x26, 0 }, { 0x25, 0 }, { 0x24, 0 }, { 0x23, 0 }, { 0x22, 0 },
    { 0x21, 0 }, { 0x20, 0 }, { 0x1F, 0 }, { 0x1E, 0 }, { 0x1D, 0 }, { 0x1C, 0 }, { 0x1B, 0 },
    { 0x1A, 0 }, { 0x19, 0 }, { 0x18, 0 }, { 0x17, 0 }, { 0x16, 0 }, { 0x14, 0 }, { 0x13, 0 },
    { 0x12, 0 }, { 0x11, 0 }, { 0x10, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 },
    { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 },
    { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 }, { 0xF, 0 },
};
u16 PogoAiTimers[55][2] = {
    { 0x60, 0 }, { 0x5D, 0 }, { 0x5A, 0 }, { 0x57, 0 }, { 0x54, 0 }, { 0x51, 0 }, { 0x4E, 0 },
    { 0x4B, 0 }, { 0x48, 0 }, { 0x45, 0 }, { 0x42, 0 }, { 0x3F, 0 }, { 0x3C, 0 }, { 0x39, 0 },
    { 0x36, 0 }, { 0x33, 0 }, { 0x30, 0 }, { 0x2D, 0 }, { 0x2A, 0 }, { 0x27, 0 }, { 0x24, 0 },
    { 0x21, 0 }, { 0x1E, 0 }, { 0x1B, 0 }, { 0x18, 0 }, { 0x15, 0 }, { 0x12, 0 }, { 0xF, 0 },
    { 0xC, 0 }, { 9, 0 }, { 6, 0 }, { 3, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0x800, 0 }, { 0x800, 0 },
    { 0, 0 }, { 0x800, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0x800, 0 }, { 0, 0 },
    { 0x800, 0 }, { 0, 0 },
};
s32 KerbIndexA[32] = {
    0x5056, 0x200C, 0x200D, 0x200E, 0x200F, 0x2010, 0x2011, 0x2012, 0x2013, 0x2014, 0x2015,
    0x2016, 0x2017, 0x2018, 0x2019, 0x201A, 0x201B, 0x201C, 0x201D, 0x201E, 0x201F, 0x2020,
    0x2021, 0x2022, 0x2023, 0x2024, 0x2025, 0x2026, 0x2027, 0x2028, 0x2029, 0x202A,
};
s32 KerbIndexB[32] = {
    0x5041, 0x200C, 0x200D, 0x200E, 0x200F, 0x2010, 0x2011, 0x2012, 0x2013, 0x2014, 0x2015,
    0x2016, 0x2017, 0x2018, 0x2019, 0x201A, 0x201B, 0x201C, 0x201D, 0x201E, 0x201F, 0x2020,
    0x2021, 0x2022, 0x2023, 0x2024, 0x2025, 0x2026, 0x2027, 0x2028, 0x2029, 0x202A,
};
u8 KerbData[32][4][8] = {
    {
        { 0, 0, 0, 0, 1, 1, 1, 1 }, { 1, 1, 1, 1, 2, 2, 2, 2 }, { 2, 2, 2, 2, 3, 3, 3, 3 },
        { 3, 3, 3, 3, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 1, 1, 1 }, { 1, 1, 1, 1, 1, 2, 2, 2 }, { 2, 2, 2, 2, 2, 3, 3, 3 },
        { 3, 3, 3, 3, 3, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 2, 2 }, { 2, 2, 2, 2, 2, 2, 3, 3 },
        { 3, 3, 3, 3, 3, 3, 0, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 2 }, { 2, 2, 2, 2, 2, 2, 2, 3 },
        { 3, 3, 3, 3, 3, 3, 3, 0 },
    },
    {
        { 0, 0, 0, 0, 0, 0, 0, 0 }, { 1, 1, 1, 1, 1, 1, 1, 1 }, { 2, 2, 2, 2, 2, 2, 2, 2 },
        { 3, 3, 3, 3, 3, 3, 3, 3 },
    },
    {
        { 3, 0, 0, 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 1, 1, 1, 1 }, { 1, 2, 2, 2, 2, 2, 2, 2 },
        { 2, 3, 3, 3, 3, 3, 3, 3 },
    },
    {
        { 3, 3, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 1, 1, 1, 1 }, { 1, 1, 2, 2, 2, 2, 2, 2 },
        { 2, 2, 3, 3, 3, 3, 3, 3 },
    },
    {
        { 3, 3, 3, 0, 0, 0, 0, 0 }, { 0, 0, 0, 1, 1, 1, 1, 1 }, { 1, 1, 1, 2, 2, 2, 2, 2 },
        { 2, 2, 2, 3, 3, 3, 3, 3 },
    },
    {
        { 3, 3, 3, 3, 0, 0, 0, 0 }, { 0, 0, 0, 0, 1, 1, 1, 1 }, { 1, 1, 1, 1, 2, 2, 2, 2 },
        { 2, 2, 2, 2, 3, 3, 3, 3 },
    },
    {
        { 3, 3, 3, 3, 3, 0, 0, 0 }, { 0, 0, 0, 0, 0, 1, 1, 1 }, { 1, 1, 1, 1, 1, 2, 2, 2 },
        { 2, 2, 2, 2, 2, 3, 3, 3 },
    },
    {
        { 3, 3, 3, 3, 3, 3, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1, 1 }, { 1, 1, 1, 1, 1, 1, 2, 2 },
        { 2, 2, 2, 2, 2, 2, 3, 3 },
    },
    {
        { 3, 3, 3, 3, 3, 3, 3, 0 }, { 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 2 },
        { 2, 2, 2, 2, 2, 2, 2, 3 },
    },
    {
        { 3, 3, 3, 3, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 1, 1, 1, 1, 1, 1, 1, 1 },
        { 2, 2, 2, 2, 2, 2, 2, 2 },
    },
    {
        { 2, 3, 3, 3, 3, 3, 3, 3 }, { 3, 0, 0, 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2 },
    },
    {
        { 2, 2, 3, 3, 3, 3, 3, 3 }, { 3, 3, 0, 0, 0, 0, 0, 0 }, { 0, 0, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 2, 2, 2, 2, 2, 2 },
    },
    {
        { 2, 2, 2, 3, 3, 3, 3, 3 }, { 3, 3, 3, 0, 0, 0, 0, 0 }, { 0, 0, 0, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 2, 2, 2, 2, 2 },
    },
    {
        { 2, 2, 2, 2, 3, 3, 3, 3 }, { 3, 3, 3, 3, 0, 0, 0, 0 }, { 0, 0, 0, 0, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 2, 2, 2, 2 },
    },
    {
        { 2, 2, 2, 2, 2, 3, 3, 3 }, { 3, 3, 3, 3, 3, 0, 0, 0 }, { 0, 0, 0, 0, 0, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 2, 2, 2 },
    },
    {
        { 2, 2, 2, 2, 2, 2, 3, 3 }, { 3, 3, 3, 3, 3, 3, 0, 0 }, { 0, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 2, 2 },
    },
    {
        { 2, 2, 2, 2, 2, 2, 2, 3 }, { 3, 3, 3, 3, 3, 3, 3, 0 }, { 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 2 },
    },
    {
        { 2, 2, 2, 2, 2, 2, 2, 2 }, { 3, 3, 3, 3, 3, 3, 3, 3 }, { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 1, 1 },
    },
    {
        { 1, 2, 2, 2, 2, 2, 2, 2 }, { 2, 3, 3, 3, 3, 3, 3, 3 }, { 3, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 1, 1, 1 },
    },
    {
        { 1, 1, 2, 2, 2, 2, 2, 2 }, { 2, 2, 3, 3, 3, 3, 3, 3 }, { 3, 3, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 2, 2, 2, 2, 2 }, { 2, 2, 2, 3, 3, 3, 3, 3 }, { 3, 3, 3, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 1, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 1, 2, 2, 2, 2 }, { 2, 2, 2, 2, 3, 3, 3, 3 }, { 3, 3, 3, 3, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 1, 1, 2, 2, 2 }, { 2, 2, 2, 2, 2, 3, 3, 3 }, { 3, 3, 3, 3, 3, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 1, 1 },
    },
    {
        { 1, 1, 1, 1, 1, 1, 2, 2 }, { 2, 2, 2, 2, 2, 2, 3, 3 }, { 3, 3, 3, 3, 3, 3, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 1, 1 },
    },
    {
        { 1, 1, 1, 1, 1, 1, 1, 2 }, { 2, 2, 2, 2, 2, 2, 2, 3 }, { 3, 3, 3, 3, 3, 3, 3, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 1 },
    },
    {
        { 1, 1, 1, 1, 1, 1, 1, 1 }, { 2, 2, 2, 2, 2, 2, 2, 2 }, { 3, 3, 3, 3, 3, 3, 3, 3 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 1, 1, 1, 1, 1, 1, 1 }, { 1, 2, 2, 2, 2, 2, 2, 2 }, { 2, 3, 3, 3, 3, 3, 3, 3 },
        { 3, 0, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 1, 1, 1, 1, 1, 1 }, { 1, 1, 2, 2, 2, 2, 2, 2 }, { 2, 2, 3, 3, 3, 3, 3, 3 },
        { 3, 3, 0, 0, 0, 0, 0, 0 },
    },
    {
        { 0, 0, 0, 1, 1, 1, 1, 1 }, { 1, 1, 1, 2, 2, 2, 2, 2 }, { 2, 2, 2, 3, 3, 3, 3, 3 },
        { 3, 3, 3, 0, 0, 0, 0, 0 },
    },
};
s16 KerbTiles[28][2] = {
    { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 1, 7 },
    { 2, 7 }, { 3, 7 }, { 4, 7 }, { 5, 7 }, { 6, 7 }, { 7, 7 }, { 7, 6 }, { 7, 5 }, { 7, 4 },
    { 7, 3 }, { 7, 2 }, { 7, 1 }, { 7, 0 }, { 6, 0 }, { 5, 0 }, { 4, 0 }, { 3, 0 }, { 2, 0 },
    { 1, 0 },
};
s32 KerbPathA[32][4] = {
    { 3, 0xA, 0x11, 0x18 }, { 4, 0xB, 0x12, 0x19 }, { 5, 0xC, 0x13, 0x1A },
    { 6, 0xD, 0x14, 0x1B }, { 7, 0xE, 0x15, 0 }, { 7, 0xE, 0x15, 0 }, { 8, 0xF, 0x16, 1 },
    { 9, 0x10, 0x17, 2 }, { 0xA, 0x11, 0x18, 3 }, { 0xB, 0x12, 0x19, 4 },
    { 0xC, 0x13, 0x1A, 5 }, { 0xD, 0x14, 0x1B, 6 }, { 0xE, 0x15, 0, 7 }, { 0xE, 0x15, 0, 7 },
    { 0xF, 0x16, 1, 8 }, { 0x10, 0x17, 2, 9 }, { 0x11, 0x18, 3, 0xA }, { 0x12, 0x19, 4, 0xB },
    { 0x13, 0x1A, 5, 0xC }, { 0x14, 0x1B, 6, 0xD }, { 0x15, 0, 7, 0xE }, { 0x15, 0, 7, 0xE },
    { 0x16, 1, 8, 0xF }, { 0x17, 2, 9, 0x10 }, { 0x18, 3, 0xA, 0x11 }, { 0x19, 4, 0xB, 0x12 },
    { 0x1A, 5, 0xC, 0x13 }, { 0x1B, 6, 0xD, 0x14 }, { 0, 7, 0xE, 0x15 }, { 0, 7, 0xE, 0x15 },
    { 1, 8, 0xF, 0x16 }, { 2, 9, 0x10, 0x17 },
};
s32 KerbPathB[32][4] = {
    { 4, 0xB, 0x12, 0x19 }, { 5, 0xC, 0x13, 0x1A }, { 6, 0xD, 0x14, 0x1B },
    { 7, 0xE, 0x15, 0 }, { 7, 0xE, 0x15, 0 }, { 8, 0xF, 0x16, 1 }, { 9, 0x10, 0x17, 2 },
    { 0xA, 0x11, 0x18, 3 }, { 0xB, 0x12, 0x19, 4 }, { 0xC, 0x13, 0x1A, 5 },
    { 0xD, 0x14, 0x1B, 6 }, { 0xE, 0x15, 0, 7 }, { 0xE, 0x15, 0, 7 }, { 0xF, 0x16, 1, 8 },
    { 0x10, 0x17, 2, 9 }, { 0x11, 0x18, 3, 0xA }, { 0x12, 0x19, 4, 0xB },
    { 0x13, 0x1A, 5, 0xC }, { 0x14, 0x1B, 6, 0xD }, { 0x15, 0, 7, 0xE }, { 0x15, 0, 7, 0xE },
    { 0x16, 1, 8, 0xF }, { 0x17, 2, 9, 0x10 }, { 0x18, 3, 0xA, 0x11 }, { 0x19, 4, 0xB, 0x12 },
    { 0x1A, 5, 0xC, 0x13 }, { 0x1B, 6, 0xD, 0x14 }, { 0, 7, 0xE, 0x15 }, { 0, 7, 0xE, 0x15 },
    { 1, 8, 0xF, 0x16 }, { 2, 9, 0x10, 0x17 }, { 3, 0xA, 0x11, 0x18 },
};
s32 PogoHeroHeights[9] = { 0, 0x50, 0x50, 0x70, 0x60, 0x70, 0x70, 0x70, 0x70 };
u8 PogoHookTableC[48] = {
    0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0,
    0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0,
    0x38, 0, 0x38, 0, 0x38, 0, 0x38, 0,
};
s16 PogoBounceHeights[22] = {
    0x1000, 0x1100, 0x1200, 0x1400, 0x1600, 0x1800, 0x1E00, 0x2000, 0x1F00, 0x1E00, 0x1F00,
    0x2000, 0x1A00, 0x1200, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x800, 0,
};
u16 TileFrames[4][4] = {
    { 0x3038, 0x3035, 0x3036, 0x3037 }, { 0x304D, 0x304A, 0x304B, 0x304C },
    { 0x3044, 0x3043, 0x3041, 0x3042 }, { 0x3029, 0x3026, 0x3027, 0x3028 },
};
u16 TileFramesAlt[4][4] = {
    { 0x302B, 0x3028, 0x3029, 0x302A }, { 0x3016, 0x3013, 0x3014, 0x3015 },
    { 0x304E, 0x304D, 0x304B, 0x304C }, { 0x303C, 0x3039, 0x303A, 0x303B },
};
s32 TileStepFrames[7] = { 0x303D, 0x303E, 0x303F, 0x3036, 0x3036, 0x3036, 0x3036 };
s32 TileStepFramesAlt[7] = { 0x3047, 0x3048, 0x3049, 0x3040, 0x3040, 0x3040, 0x3040 };
PogoOffset EncloseStepsOdd[8] = { { 1, 0 }, { 1, 0 }, { 0, -1 }, { 0, -1 }, { -1, 0 }, { -1, 0 }, { 0, 1 }, { 0, 1 } };
PogoOffset EncloseStepsEven[8] = { { -1, 0 }, { -1, 0 }, { 0, -1 }, { 0, -1 }, { 1, 0 }, { 1, 0 }, { 0, 1 }, { 0, 1 } };
s32 MushroomScales[16] = {
    0, 0x80, 0x100, 0x200, 0x380, 0x500, 0x600, 0x700, 0x900, 0x1100, 0x1100, 0x900, 0x880,
    0x980, 0x1080, 0x980,
};
s32 RooScale[3] = { 0x1000, 0x1000, 0x1000 };
s32 RooSteps0[3] = { (s32)0x80110000, 0x4080001A, 0x8000 };
s32 RooSteps1[3] = { (s32)0x80110000, 0x4100000E, 0x8000 };
s32 RooSteps2[3] = { (s32)0x80110000, 0x40000015, 0x8000 };
s32 RooAnimList[5] = { (s32)&RooSteps0, (s32)&RooSteps1, (s32)&RooSteps2, 0, 0 };
ScriptInit RooScriptInit = { 0, (void *)&RooAnimList, 0x401 };
s32 RooHopFrames[15] = { 6, 6, 6, 7, 6, 7, 6, 7, 6, 6, 8, 8, 6, 8, 0xC };
s32 RooFrames[4] = {
    0xA, 0xA, 0xA, 0xC,
};
hero_seq CRA_seq_pogo_tazing[24] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x8003, 0x14, 0x4206, 0x4207,
    0x4208, 0x4209, 0x420A, 0x8003, 0x14, 0x420B, 0x420C, 0x420D, 0x420E, 0x8000, 0,
};
hero_seq CRA_seq_pogo_idle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x8003, 0x14, 0x400E, 0x400F, 0x4010, 0x8003, 0x12, 0x8000, 0,
};
hero_seq CRA_seq_pogo_quick_idle[16] = {
    0, 0x4004, 0x4006, 0x4008, 0x400A, 0x8003, 0x12, 0x400C, 0x8003, 0x14, 0x400E, 0x4010, 0x4000,
    0x4002, 0x8000, 0,
};
s32 PogoFramesCrash[19] = {
    0x4107, 0x4108, 0x4109, 0x410A, 0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112,
    0x4113, 0x4114, 0x4115, 0x4116, 0x4117, 0x4117, 0x4117,
};
hero_seq CRA_seq_pogo_dazed[6] = {
    0, 0x8007, 0x18, 0x4180, 0x8000, 0,
};
hero_seq CRA_seq_pogo_winner[36] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x8003, 0x12, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089,
    0x408A, 0x8008, 0x566, 0x8007, 0xF, 0x408B, 0x409B, 0x8003, 0x12, 0x409C, 0x409D, 0x409E,
    0x409F, 0x8003, 0x14, 0x40A0, 0x40A1, 0x40A2, 0x40A3, 0x40A4, 0x40A5, 0x8000, 0,
};
hero_seq COC_seq_pogo_tazing[24] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x4204, 0x8003, 0x14, 0x4205, 0x4206, 0x4207,
    0x4208, 0x8003, 0x14, 0x4209, 0x420A, 0x420B, 0x420C, 0x420D, 0x8000, 0, 0,
};
hero_seq COC_seq_pogo_idle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x8003, 0x14, 0x400D, 0x400E, 0x400F, 0x4010, 0x8003, 0x12, 0x8000, 0,
};
hero_seq COC_seq_pogo_quick_idle[16] = {
    0, 0x4000, 0x4002, 0x4004, 0x4006, 0x4008, 0x400A, 0x400C, 0x8003, 0x14, 0x400E, 0x8003, 0x12,
    0x8000, 0, 0,
};
s32 PogoFramesCoco[22] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A, 0x408B,
    0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4091, 0x4091, 0x4091, 0x4091,
};
hero_seq COC_seq_pogo_dazed[6] = {
    0, 0x8007, 0x17, 0x4100, 0x8000, 0,
};
hero_seq COC_seq_pogo_winner[78] = {
    0, 0x8008, 0x567, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188,
    0x4189, 0x418A, 0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x4194,
    0x4195, 0x4196, 0x8003, 0x12, 0x4197, 0x4198, 0x4199, 0x8003, 0x14, 0x4000, 0x4001, 0x4002,
    0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A, 0x400B, 0x400C, 0x8003, 0x12,
    0x400D, 0x400E, 0x400F, 0x4010, 0x8003, 0x14, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005,
    0x4006, 0x4007, 0x4008, 0x4009, 0x400A, 0x400B, 0x400C, 0x8003, 0x12, 0x400D, 0x400E, 0x400F,
    0x4010, 0x8003, 0x14, 0x8000, 0, 0,
};
hero_seq BRO_seq_pogo_tazing[10] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq BRO_seq_pogo_idle[24] = {
    0, 0x4080, 0x8003, 0x13, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089,
    0x408A, 0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8003, 0x12, 0x8000, 0, 0,
};
hero_seq BRO_seq_pogo_quick_idle[16] = {
    0, 0x4080, 0x8003, 0x13, 0x4082, 0x4084, 0x4086, 0x4088, 0x408A, 0x408C, 0x408E, 0x8003, 0x12,
    0x8000, 0, 0,
};
s32 PogoFramesBrio[19] = {
    0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A, 0x410B, 0x410C, 0x410D, 0x410E,
    0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115,
};
hero_seq BRO_seq_pogo_dazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_pogo_winner[12] = {
    0, 0x800B, 0x800A, 0x569, 0x8007, 0x22, 0x4180, 0x8007, 0x3B, 0x41A1, 0x8000, 0,
};
hero_seq CTX_seq_pogo_tazing[10] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq CTX_seq_pogo_idle[24] = {
    0, 0x4000, 0x8003, 0x13, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009,
    0x400A, 0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x8003, 0x12, 0x8000, 0, 0,
};
hero_seq CTX_seq_pogo_quick_idle[16] = {
    0, 0x4000, 0x8003, 0x13, 0x4002, 0x4004, 0x4006, 0x4008, 0x400A, 0x400C, 0x400E, 0x8003, 0x12,
    0x8000, 0, 0,
};
s32 PogoFramesCortex[19] = {
    0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A, 0x408B, 0x408C, 0x408D, 0x408E,
    0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095,
};
hero_seq CTX_seq_pogo_dazed[6] = {
    0, 0x8007, 0x14, 0x4100, 0x8000, 0,
};
hero_seq CTX_seq_pogo_winner[12] = {
    0, 0x8008, 0x568, 0x8007, 0x37, 0x4180, 0x8007, 0x20, 0x41B7, 0x8000, 0, 0,
};
hero_seq DNG_seq_pogo_tazing[24] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x8003, 0x14, 0x4206, 0x4207,
    0x4208, 0x4209, 0x420A, 0x8003, 0x14, 0x420B, 0x420C, 0x420D, 0x420E, 0x8000, 0,
};
hero_seq DNG_seq_pogo_idle[24] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x8003, 0x11, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x410C, 0x410D, 0x8003, 0x12, 0x410E, 0x410F, 0x4110, 0x8000, 0,
};
hero_seq DNG_seq_pogo_quick_idle[16] = {
    0, 0x4100, 0x4102, 0x8003, 0x11, 0x4104, 0x4106, 0x4108, 0x410A, 0x410C, 0x410E, 0x8003, 0x12,
    0x4110, 0x8000, 0,
};
s32 PogoFramesDingo[22] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A, 0x408B,
    0x408C, 0x408D, 0x408E, 0x408F, 0x408F, 0x4090, 0x4090, 0x4090, 0x4090, 0x4090,
};
hero_seq DNG_seq_pogo_dazed[6] = {
    0, 0x8007, 0x18, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_pogo_winner[52] = {
    0, 0x8008, 0x56C, 0x4180, 0x8003, 0x12, 0x4181, 0x4182, 0x8003, 0x11, 0x4183, 0x4184, 0x4185,
    0x4186, 0x4187, 0x4188, 0x4189, 0x418A, 0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191,
    0x4192, 0x8003, 0x12, 0x4100, 0x4101, 0x4102, 0x4103, 0x8003, 0x11, 0x4104, 0x4105, 0x4106,
    0x4107, 0x4108, 0x4109, 0x410A, 0x410B, 0x410C, 0x410D, 0x8003, 0x12, 0x410E, 0x410F, 0x4110,
    0x8000, 0, 0,
};
hero_seq KAN_seq_pogo_tazing[28] = {
    0, 0x4000, 0x8003, 0x14, 0x4001, 0x4002, 0x4003, 0x4000, 0x8003, 0x14, 0x4001, 0x4002, 0x4003,
    0x4000, 0x4001, 0x8003, 0x14, 0x4002, 0x4003, 0x4000, 0x4001, 0x4002, 0x8003, 0x14, 0x4003,
    0x8000, 0, 0,
};
hero_seq KAN_seq_pogo_idle[24] = {
    0, 0x4080, 0x4081, 0x8003, 0x11, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089,
    0x408A, 0x408B, 0x408C, 0x408D, 0x408E, 0x8003, 0x12, 0x408F, 0x8000, 0, 0,
};
hero_seq KAN_seq_pogo_quick_idle[16] = {
    0, 0x4080, 0x4082, 0x8003, 0x11, 0x4084, 0x4086, 0x4088, 0x408A, 0x408C, 0x408E, 0x8003, 0x12,
    0x408E, 0x8000, 0,
};
s32 PogoFramesKangaroo[23] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A, 0x410B,
    0x410C, 0x410D, 0x410E, 0x410F, 0x410F, 0x410F, 0x410F, 0x410F, 0x410F, 0x410F, 0x410F,
};
hero_seq KAN_seq_pogo_dazed[6] = {
    0, 0x8007, 0x18, 0x4180, 0x8000, 0,
};
hero_seq KAN_seq_pogo_winner[48] = {
    0, 0x4200, 0x8003, 0x12, 0x4201, 0x8008, 0x56D, 0x4202, 0x4203, 0x8003, 0x11, 0x4204, 0x4205,
    0x4206, 0x4207, 0x4208, 0x4209, 0x420A, 0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211,
    0x4212, 0x8003, 0x12, 0x4080, 0x4081, 0x8003, 0x11, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086,
    0x4087, 0x4088, 0x4089, 0x408A, 0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0,
};
hero_seq TIN_seq_pogo_tazing[24] = {
    0, 0x4200, 0x8003, 0x14, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x8003, 0x14, 0x4206, 0x4207,
    0x4208, 0x4209, 0x420A, 0x8003, 0x14, 0x420B, 0x420C, 0x420D, 0x420E, 0x8000, 0,
};
hero_seq TIN_seq_pogo_idle[26] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x8003, 0x14, 0x400D, 0x400E, 0x400F, 0x8003, 0x14, 0x8003, 0x12, 0x8000, 0, 0,
};
hero_seq TIN_seq_pogo_quick_idle[16] = {
    0, 0x4000, 0x4002, 0x4004, 0x4006, 0x4008, 0x400A, 0x400C, 0x8003, 0x14, 0x400E, 0x400E, 0x8003,
    0x12, 0x8000, 0,
};
s32 PogoFramesTiny[22] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A, 0x418B,
    0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4192, 0x4192, 0x4192,
};
hero_seq TIN_seq_pogo_dazed[6] = {
    0, 0x8007, 0x13, 0x4080, 0x8000, 0,
};
hero_seq TIN_seq_pogo_winner[10] = {
    0, 0x8008, 0x56A, 0x8003, 0x12, 0x8007, 0x13, 0x4100, 0x8000, 0,
};
hero_seq KON_seq_pogo_tazing[24] = {
    0, 0x4180, 0x8003, 0x14, 0x4181, 0x4182, 0x4183, 0x4184, 0x8003, 0x14, 0x4185, 0x4186, 0x4187,
    0x4188, 0x8003, 0x14, 0x4189, 0x418A, 0x418B, 0x418C, 0x418D, 0x8000, 0, 0,
};
hero_seq KON_seq_pogo_idle[26] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x8003, 0x14, 0x408E, 0x408F, 0x4090, 0x4091, 0x8003, 0x12, 0x8000, 0,
    0,
};
hero_seq KON_seq_pogo_quick_idle[16] = {
    0, 0x4080, 0x4082, 0x4084, 0x4086, 0x4088, 0x408A, 0x408C, 0x8003, 0x14, 0x408E, 0x408E, 0x8003,
    0x12, 0x8000, 0,
};
s32 PogoFramesKong[18] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A, 0x410B,
    0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111,
};
hero_seq KON_seq_pogo_dazed[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq KON_seq_pogo_winner[78] = {
    0, 0x8008, 0x56B, 0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208,
    0x4209, 0x420A, 0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211, 0x4212, 0x8003, 0x14,
    0x4213, 0x8003, 0x12, 0x4214, 0x4215, 0x4216, 0x4217, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084,
    0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A, 0x408B, 0x408C, 0x408D, 0x8003, 0x14, 0x408E,
    0x8003, 0x12, 0x408F, 0x4090, 0x4091, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086,
    0x4087, 0x4088, 0x4089, 0x408A, 0x408B, 0x408C, 0x408D, 0x8003, 0x14, 0x408E, 0x8003, 0x12,
    0x408F, 0x4090, 0x4091, 0x8000, 0, 0,
};
s32 PogoTilesLow[9] = { 0, 1, 1, 3, 3, 3, 2, 1, 1 };
s32 PogoTilesHigh[9] = { 0, 7, 7, 1, 1, 3, 3, 7, 2 };
s32 PogoTilesMid[9] = { 0, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x10 };
s16 PogoPreLoadDB[4] = { 0x1C, 0x4F, -1, 0 };
char *PogoAnims[6] = {
    (char *)PogoAnimBreathe, (char *)PogoAnimJump, (char *)PogoAnimDazed, (char *)PogoAnimWin,
    (char *)PogoAnimTazing, NULL,
};
s16 PogoLevelDB0[2] = { -1, 0 };
s16 PogoLevelDB1[2] = { 0x50, -1 };
WWInf PogoWorldInfo[8] = {
    { 0x47, PogoAnims, PogoLevelDB0 },
    { 0x48, PogoAnims, PogoLevelDB0 },
    { 0x49, PogoAnims, PogoLevelDB1 },
    { 0x4A, PogoAnims, PogoLevelDB0 },
    { 0x4B, PogoAnims, PogoLevelDB0 },
    { 0x4C, PogoAnims, PogoLevelDB0 },
    { 0x4D, PogoAnims, PogoLevelDB1 },
    { 0x4E, PogoAnims, PogoLevelDB0 },
};
s32 PogoFiles[15] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 535], (s32)&DatIndex[2 * 534], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 531],
    (s32)&DatIndex[2 * 530], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 533], (s32)&DatIndex[2 * 532], (s32)&HeroModelBank[DB_PogoObjects],
    (s32)&DatIndex[2 * 529], (s32)&DatIndex[2 * 528], 0, 0, 0,
};
s32 PogoLocoFiles[18] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 527], (s32)&DatIndex[2 * 526], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 523],
    (s32)&DatIndex[2 * 522], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 525], (s32)&DatIndex[2 * 524], (s32)&HeroModelBank[DB_PogoObjects],
    (s32)&DatIndex[2 * 521], (s32)&DatIndex[2 * 520], (s32)&HeroModelBank[DB_Roo], (s32)&DatIndex[2 * 519], (s32)&DatIndex[2 * 518],
    0, 0, 0,
};
/* the four levels' files (modes 10..13; the engine's table at 0x800A0030): the exe's file group
 * and the model list; Loco's has one more model */
file_group_load_info file_group_POGOPAINTER = { &FileGroups[23], PogoFiles };
file_group_load_info file_group_POGOAGOGO = { &FileGroups[23], PogoFiles };
file_group_load_info file_group_POGOLOCO = { &FileGroups[22], PogoLocoFiles };
file_group_load_info file_group_POGOBOUNCER = { &FileGroups[23], PogoFiles };
u16 PogoLUPtab[4] = { 3, 0x20D, 0x201, 0 };
TrigLookUp PogoLUP = { PogoLUPtab, 3 };
PreMap POGO_PREMAP = { PogoPreLoadDB, PogoWorldInfo, &PogoLUP };
shell_setup POGO_shell_info = {
    0x2100, 0x10, 0x1000, 0x100, 0x200, 0x4B0, VramAreas, VramNodePool, 0x20, 0x400,
};
process_info PogoProcess = { pogo_init, 0, 0, pogo_kill, 0 };
u16 PogoHeroModels[10] = { 0, 9, 0xA, 0xB, 0xC, 0xD, 0xF, 0xE, 0x10, 0 };
s32 BrioSmokeFrame = 0;
s32 CortexSmokeFrame = 0;
s16 pickup_spawn_time = -1;
s16 powerup_spawn_time = -1;
s32 number_of_pickups = 0;
s32 number_of_powerups = 0;
s32 MushroomCount = 0;
s32 PogoUnused0 = 0;
s32 PogoUnused1 = 0;
s32 KerbMode = 0;
s32 RooBounce = 0;
s32 RooFrame = 0;
s32 PadlockFlags[10] = { 0x12, 0x10, 0xE, 0xD, 0xC, 0xA, 8, 7, 7, 7 };
s32 PadlockTimes[10] = { 1, 4, 7, 0xA, 0xE, 0x13, 0x17, 0x1C, 0x1C, 0x1C };
PogoPlayer PogoPlay[4] = {0};
s32 PogoBounceCount[4] = {0};
PogoCell PogoGrid[8][8] = {0};
PogoMove PogoAIPlay[4] = {0};
s32 TileScan0[8] = {0};
s32 TileScan1[8] = {0};
s32 TileScan2[8] = {0};
s32 TileScan3[8] = {0};
s32 PogoRooNas = 0;
NAS *PogoPadlockNas = 0;
PogoObj *PogoKerbObj = 0;
s32 PogoLevelIndex = 0;
s32 PogoAiQualityIndex = 0;
s32 KerbStep = 0;
s32 KerbDelay = 0;
s32 KerbTimer = 0;
s32 KerbPeriod = 0;
s32 KerbCount = 0;
s32 KerbClockSecond = 0;
s32 RooTileX = 0;
s32 RooTileZ = 0;
s32 RooSpeedX = 0;
s32 RooSpeedZ = 0;
s32 RooAngle = 0;
s32 RooDir = 0;
s32 RooSteps = 0;
s32 RooWait = 0;

void PogoInit_AIQuality(void) {
    AiBotTweakTable = &PogoAiBotTweak;
    switch (((PogoModeView *)GameInfo)->mode) {
    case 10:
        AiSkillTable = ((PogoModeView *)GameInfo)->teams != 0 ? &PogoPainterAiSkillTeams : &PogoPainterAiSkill;
        AiHumanRoundTweakTable = GameTeams[0] != 0 ? PogoPainterAiRoundAlt : PogoPainterAiRound;
        AiHumanTweakTable = GameTeams[0] != 0 ? &PogoPainterAiHumanAlt : &PogoPainterAiHuman;
        AiTeamSkillTable = PogoPainterAiTeamSkill;
        break;
    case 11:
        AiSkillTable = ((PogoModeView *)GameInfo)->teams != 0 ? &PogoAgogoAiSkillTeams : &PogoAgogoAiSkill;
        AiHumanRoundTweakTable = GameTeams[0] != 0 ? PogoAgogoAiRoundAlt : PogoAgogoAiRound;
        AiHumanTweakTable = GameTeams[0] != 0 ? &PogoAgogoAiHumanAlt : &PogoAgogoAiHuman;
        AiTeamSkillTable = PogoAgogoAiTeamSkill;
        break;
    case 12:
        AiSkillTable = ((PogoModeView *)GameInfo)->teams != 0 ? &PogoLocoAiSkillTeams : &PogoLocoAiSkill;
        AiHumanRoundTweakTable = GameTeams[0] != 0 ? PogoLocoAiRoundAlt : PogoLocoAiRound;
        AiHumanTweakTable = GameTeams[0] != 0 ? &PogoLocoAiHumanAlt : &PogoLocoAiHuman;
        AiTeamSkillTable = PogoLocoAiTeamSkill;
        break;
    case 13:
        AiSkillTable = ((PogoModeView *)GameInfo)->teams != 0 ? &PogoBouncerAiSkillTeams : &PogoBouncerAiSkill;
        AiHumanRoundTweakTable = GameTeams[0] != 0 ? PogoBouncerAiRoundAlt : PogoBouncerAiRound;
        AiHumanTweakTable = GameTeams[0] != 0 ? &PogoBouncerAiHumanAlt : &PogoBouncerAiHuman;
        AiTeamSkillTable = PogoBouncerAiTeamSkill;
        break;
    }
}

void PogoInit_AllHeros(void) {
    s32 i;
    s16 a;
    HeroInfo *h;
    EXTengine *e;
    subengine *sub;
    PogoPlayer *slot;
    PogoMove *mv;
    EXTheromove *m;

    i = 0;
    h = Heroes;
    mv = PogoAIPlay;
    slot = PogoPlay;
loop:
    if (i < 4) {
        h->hero->Camera.width = 0x500;
        switch (h->hero_name) {
        case 5:
        case 6:
        case 7:
            h->hero->Camera.height = 0x700;
            break;
        case 8:
            h->hero->Camera.height = 0x800;
            break;
        default:
            h->hero->Camera.height = 0x500;
            break;
        }
        Player_SnapToTile(i);
        Set_Tile_Under_Player(i, 0);
        slot->forward_speed = Player_CalcForwardSpeed(i, 0);
        mv->unk4 = 0;
        e = h->hero_eng;
        if (e != NULL) {
            if (GameTeams[0] != 0) {
                switch (i) {
                case 0:
                case 1:
                    a = 0xC00;
                    break;
                case 2:
                case 3:
                    a = 0x400;
                    break;
                default:
                    a = 0;
                    break;
                }
            } else {
                switch (i) {
                case 0:
                    a = 0x800;
                    break;
                case 1:
                    a = 0xC00;
                    break;
                case 2:
                    a = 0;
                    break;
                case 3:
                    a = 0x400;
                    break;
                default:
                    a = 0;
                    break;
                }
            }
            e->unk1E = a;
            m = h->hero_move;
            if (m != NULL) {
                m->forward_angle = a;
                m->facing_angle = a;
            }
            e->unk5E = 1;
        }
        e = h->hero_powerup;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                u32 f = sub->flags;

                f &= ~0x8000;
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x200D;
                sub->flags = f;
                e->flags &= ~0x8000;
            }
        }
        e = h->hero_shadow;
        if (e != NULL) {
            e->unk2C.z = 0x1A00;
            e->unk2C.y = 0x1A00;
            e->unk2C.x = 0x1A00;
            e->unk5E = 1;
        }
        PogoAI_InitHero(i);
        h++;
        mv++;
        slot++;
        i++;
        goto loop;
    }
}

void PogoInit_RelicChallenge(void) {
    s32 i;
    s16 v;
    u32 snd;

    if (SCORE60V->flags & 0x40) {
        switch (SCORE60V->mode) {
        case 10:
            for (i = 0; i < 4; i++) {
                v = 60;
                if (SCORE60V->out[i] == 0) {
                    v = 80;
                    if (SCORE60V->teams != 0) {
                        v = 90;
                    }
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x100;
            if (GameTeams[0] != 0) {
                snd = 0x120;
            }
            break;
        case 11:
            for (i = 0; i < 4; i++) {
                v = 150;
                if (SCORE60V->out[i] != 0) {
                    v = 90;
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        case 12:
            for (i = 0; i < 4; i++) {
                if (SCORE60V->out[i] != 0) {
                    v = 80;
                    if (SCORE60V->teams != 0) {
                        v = 90;
                    }
                } else {
                    v = 120;
                    if (SCORE60V->teams != 0) {
                        v = 135;
                    }
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        case 13:
            for (i = 0; i < 4; i++) {
                v = 120;
                if (SCORE60V->out[i] != 0) {
                    v = 90;
                }
                Heroes[i].hero_score_displayed = v;
                Heroes[i].hero_score = v;
            }
            snd = 0x130;
            break;
        default:
            goto skip;
        }
        Set_Clock(snd, 0x81D0);
    skip:
        InitSharedScoreIcons();
        if (GameTeams[0] != 0) {
            PanelCtrl |= 0x2000;
        }
    }
}

void InitPogo_Tiles(void) {
    s32 i;
    s32 j;
    PogoCell *p;
    PogoObj *obj;
    u16 *id;
    s16 v;
    PogoCell *base;
    PogoCell *base2;
    u8 *g;
    s32 rowOff;
    s32 off;

    j = 0;
    base = PogoGrid[0];
    for (rowOff = 0; j < 8; j++) {
        i = 0;
        off = rowOff;
        do {
            PogoCell *c;

            Tile_Clear(i, j);
            c = (PogoCell *)(off + (u32)base);
            c->unk4 = NULL;
            *(s16 *)c->unk8 = 0;
            *(s16 *)c->unkC = 0;
            c->hero = NULL;
            c->unkE = 0;
            c->unk10 = 0;
            c->unk18[0] = 0;
            off += sizeof(PogoGrid[0]);
        } while (++i < 8);
        rowOff += sizeof(PogoCell);
    }
    obj = (PogoObj *)((u8 *)MapScene->subs);
    PogoKerbObj = NULL;
    if (obj != NULL) {
    base2 = PogoGrid[0];
    g = GameInfo;
    do {
        id = (u16 *)model_entry_data(obj->model, obj->index);
        if ((u16)(*id - 10) < 78) {
            v = *id;
            j = 7 - (s16)(v % 10);
            i = (s16)(v / 10) - 1;
            p = (PogoCell *)(j * sizeof(PogoCell) + i * sizeof(PogoGrid[0]) + (u32)base2);
            p->unk4 = (PogoCellPos *)obj;
            obj->flags |= 0x20000;
            psxpogo_init_tile((subengine *)obj);
        }
        if (*(s16 *)(g + 0x34) == 11 && *(s16 *)id == 100) {
            PogoKerbObj = obj;
        }
        obj = obj->next;
    } while (obj != NULL);
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            Tile_Update(i, j);
        }
    }
}

void UpdatePOGO(void) {
    PogoPlayer *slot;
    s32 i;
    s32 ok;
    u8 *g;

    PogoQuality_Update();
    Update_Occupancy();
    Update_Tiles();
    if (GameExitFlags & 0x801) {
        return;
    }
    i = 0;
    slot = PogoPlay;
loop:
    if (slot->status == 5) {
        goto tick;
    }
    if (slot->status == 6) {
    tick:
        if (--slot->timer < 2) {
            Pickup_Lose(i);
        }
    }
    i++;
    slot++;
    if (i < 4) {
        goto loop;
    }
    PogoPickups_Poll();
    PogoPowerups_Poll();
    g = GameInfo;
    switch ((PogoMode)*(s16 *)(g + 0x34)) {
    case POGO_MODE_12:
        break;
    case POGO_MODE_10:
        if (*(u16 *)(g + 0x32) & 0x80) {
            switch ((u32)Clock_Time >> 8) {
            case 0x115:
                ok = MushroomCount < 3;
                break;
            case 0x50:
                ok = MushroomCount < 4;
                break;
            case 0x30:
                ok = MushroomCount < 5;
                break;
            case 0x20:
                ok = MushroomCount < 6;
                break;
            case 0x10:
                ok = MushroomCount < 7;
                break;
            default:
                return;
            }
            if (ok) {
                CreateNAS(0, 0, 0, 0x1407, 0x11);
            }
        }
        break;
    case POGO_MODE_11:
        PogoKerb_Poll();
        break;
    }
}

void PogoQuality_Init(void) {
    PogoModeView *g;
    PogoModeView *h;
    PogoModeEntry *e;
    s32 i;

    e = NULL;
    switch (((PogoModeView *)GameInfo)->mode) {
    case 10:
        if (((PogoModeView *)GameInfo)->teams != 0) {
            e = &PogoPainterAiHumanAlt;
        } else {
            e = &PogoPainterAiHuman;
        }
        break;
    case 11:
        if (((PogoModeView *)GameInfo)->teams != 0) {
            e = &PogoAgogoAiHumanAlt;
        } else {
            e = &PogoAgogoAiHuman;
        }
        break;
    case 12:
        if (((PogoModeView *)GameInfo)->teams != 0) {
            e = &PogoLocoAiHumanAlt;
        } else {
            e = &PogoLocoAiHuman;
        }
        break;
    case 13:
        if (((PogoModeView *)GameInfo)->teams != 0) {
            e = &PogoBouncerAiHumanAlt;
        } else {
            e = &PogoBouncerAiHuman;
        }
        break;
    }
    if (e != NULL) {
        g = (PogoModeView *)GameInfo;
        switch (g->mode) {
        case 10:
            e->unkE = PogoPainterAiQuality[g->unk38];
            e->unk10 = PogoPainterAiQuality[g->unk38 + 3];
            break;
        case 11:
            e->unkE = PogoAgogoAiQuality[g->unk38];
            e->unk10 = PogoAgogoAiQuality[g->unk38 + 3];
            break;
        case 12:
            e->unkE = PogoLocoAiQuality[g->unk38];
            e->unk10 = PogoLocoAiQuality[g->unk38 + 3];
            break;
        case 13:
            e->unkE = PogoBouncerAiQuality[g->unk38];
            e->unk10 = PogoBouncerAiQuality[g->unk38 + 3];
            break;
        }
        g = (PogoModeView *)GameInfo;
        if ((g->flags & 1) || (g->flags & 2)) {
            i = 0;
            h = g;
            for (; i < 4; i++) {
                if (((u8 *)h)[i + 0x5C] == 0) {
                    AiPlayerSkill[i].quality = *((h->flags & 2) ? &e->unk10 : &e->unkE);
                }
            }
        }
    }
    PogoQuality_Update();
}

void Update_Occupancy(void) {
    s32 x;
    s32 z;
    EXTengine *eng;
    s32 k;
    s32 hx;
    s32 hz;
    EXTengine *he;

    for (hx = 0; hx < 8; hx++) {
        for (hz = 0; hz < 8; hz++) {
            PogoGrid[hx][hz].unkE = 0;
        }
    }
    for (k = 0; k < 4; k++) {
        he = Heroes[k].hero_eng;
        hx = (he->unk10.x + 0x800) >> 9;
        hz = (he->unk10.z + 0x800) >> 9;
        if ((u32)hx < 8 && (u32)hz < 8) {
            PogoGrid[hx][hz].unkE = k + 1;
        }
    }
    if (PogoRooNas != 0) {
        eng = *(EXTengine **)(PogoRooNas + 0x54);
        if (eng != NULL) {
            x = (eng->unk10.x + 0x800) >> 9;
            z = (eng->unk10.z + 0x800) >> 9;
            if ((u32)x < 8 && z >= 0 && z < 8) {
                PogoGrid[x][z].unkE = 5;
            }
        }
    }
}

void PogoWinner_Crystal(s32 arg0, u16 arg1) {
    u8 *g;
    s32 i;
    s32 found;

    switch (GameMode) {
    case 10:
    case 12:
        found = -1;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_mode == 0x3B && (PogoPlay[i].flags & 1)) {
                found = i;
            }
        }
        if (found == -1) {
            break;
        }
        if (GameInfo[found + 0x5C] != 0) {
            return;
        }
        g = GameInfo;
        g[0x60] = 0;
        ((D5A614View *)g)->unk30 |= arg1;
        for (i = 0; i < 4; i++) {
            if (g[i + 0x5C] != 0) {
                RemoveCamera(Heroes[i].hero);
                Heroes[i].hero_status |= 0x400;
            } else if (i != found) {
                Heroes[i].hero_status |= 0x100;
            }
        }
        CameraMode = 1;
        return;
    case 11:
    case 13:
        break;
    default:
        return;
    }
    PogoWinner_CrystalOther(arg0, arg1);
}

void PogoWinner_CrystalOther(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

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
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
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

            InitBigScore();
            CameraMode = 1;
            ia = *(s16 *)(g + 0x48);
            ib = *(s16 *)(g + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
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
                for (k = 0; k < 4; k++) {
                    if (GameInfo[k + 0x5C] != 0) {
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

void PogoWinner_Relic(s32 arg0, u16 arg1) {
    PogoTeamView *g;
    PogoTeamView *p;
    PogoTeamView *q;
    s32 sa;
    s32 sb;
    s32 found;
    s32 done;
    s32 i;

    found = -1;
    done = 0;
    g = (PogoTeamView *)GameInfo;
    if (g->teams != 0) {
        sa = PogoTeamScore(g->team0);
        sb = PogoTeamScore(g->team1);
        if (sa == 0) {
            if (g->out[g->team0] == 0) {
                POGO_TEAM_LOST(g, g->team0, i, 0);
            } else {
                done = 1;
            }
        } else if (sb == 0) {
            if (g->out[g->team1] == 0) {
                POGO_TEAM_LOST_LATE(g, g->team1, i, 0);
            } else {
                done = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_score == 0) {
                found = i;
            }
        }
        if (found != -1) {
            p = (PogoTeamView *)GameInfo;
            if (p->out[found] == 0) {
                POGO_TEAM_LOST(p, found, i, 0);
            } else {
                done = 1;
            }
        }
    }
    if (done || arg0 == -1) {
        i = 0;
        q = (PogoTeamView *)GameInfo;
        p = q;
        q->winner = 0;
        q->unk30 |= arg1;
        Clock_flags |= 0x200;
        for (; i < 4; i++) {
            if (GameInfo[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x400;
                RemoveCamera(Heroes[i].hero);
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
        CameraMode = 1;
    }
}

void PogoWinner_Gem(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

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
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
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

            InitBigScore();
            CameraMode = 1;
            ia = *(s16 *)(g + 0x48);
            ib = *(s16 *)(g + 0x4A);
            ita = Heroes[ia].hero_teammate;
            itb = Heroes[ib].hero_teammate;
            sa = Heroes[ia].hero_score + Heroes[ita].hero_score;
            sb = Heroes[ib].hero_score + Heroes[itb].hero_score;
            if (sa == sb) {
                GameInfo[0x60] = 0;
                *(u16 *)(GameInfo + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (GameInfo[k + 0x5C] != 0) {
                        RemoveCamera(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
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
                for (k = 0; k < 4; k++) {
                    if (GameInfo[k + 0x5C] != 0) {
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

void PogoWinner_Normal(s32 arg0, u16 arg1) {
    PogoRankView *g;
    PogoRankView *p;
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
        RANK60V->order[i] = best;
        used |= 1 << best;
    }
    g = RANK60V;
    if (g->teams == 0) {
        h = Heroes;
        top = &h[g->order[0]];
        if (top->hero_score == h[g->order[1]].hero_score) {
            g->winner[0] = 0;
            g->unk30 |= arg1;
            for (k = 0; k < 4; k++) {
                Heroes[k].hero_status |= 0x100;
            }
            return;
        }
        p = g;
        p->winner[0] = top->hero_name;
        p->unk30 |= 0x800;
        for (n = 0; n < 4; n++) {
            if (p->order[0] == n) {
                h[n].hero_status |= 0x200;
            } else {
                h[n].hero_status |= 0x400;
            }
        }
        return;
    }
    InitBigScore();
    hb = Heroes;
    ta = &hb[g->team[0]];
    b = &hb[g->team[1]];
    am = &hb[ta->hero_teammate];
    bm = &hb[b->hero_teammate];
    sa = ta->hero_score + am->hero_score;
    sb = b->hero_score + bm->hero_score;
    if (sa == sb) {
        g->winner[0] = 0;
        g->unk30 |= arg1;
        for (k = 0; k < 4; k++) {
            Heroes[k].hero_status |= 0x100;
        }
        return;
    }
    g->unk30 |= 0x800;
    if (sb < sa) {
        g->winner[0] = ta->hero_name;
        g->winner[1] = am->hero_name;
        ta->hero_status |= 0x200;
        am->hero_status |= 0x200;
        b->hero_status |= 0x400;
        bm->hero_status |= 0x400;
    } else {
        RANK60V->winner[0] = b->hero_name;
        RANK60V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void PreMovePOGO(u32 arg0) {
    HeroInfo *hb;
    HeroInfo *h;
    EXTheromove *move;
    PadState *pb;
    PadState *pad;
    s32 x;
    s32 z;
    s32 bits;
    s32 speed;

    hb = Heroes;
    h = &hb[arg0];
    x = (h->hero_eng->unk10.x + 0x800) >> 9;
    z = (h->hero_eng->unk10.z + 0x800) >> 9;
    if (Pickup_IsClaimable(arg0, x, z) != 0) {
        Pickup_Claim(x, z, arg0);
    }
    move = h->hero_move;
    move->forward_speed = 0;
    if (h->hero_frozen == 0) {
    pb = Pads;
    pad = &pb[arg0];
    if ((pad->unk0 & 1) || GameInfo[arg0 + 0x5C] != 0) {
        bits = ((PogoPad *)pad)->unk50;
        if (PogoPlay[arg0].status == 5 && !(bits & 0xF0)) {
            switch ((u16)move->forward_angle >> 10) {
            case 0:
                bits |= 0x40;
                break;
            case 1:
                bits |= 0x80;
                break;
            case 2:
                bits |= 0x10;
                break;
            case 3:
                bits |= 0x20;
                break;
            }
        }
        if ((bits & 0xF0) && move->velocity_enable != 0) {
            speed = Player_CalcForwardSpeed(arg0, PogoPlay[arg0].status == 5);
            Player_RemoveReserve(arg0);
            if (((bits & 0x80) && x > 0) || ((bits & 0x20) && x < 7)
                || ((bits & 0x10) && z < 7) || ((bits & 0x40) && z > 0)) {
                if ((bits & 0x80) && PogoAI_IsSquareOKForPlayer(arg0, x, z, -1, 0, 0) != 0) {
                    Player_SetReserve(arg0, x - 1, z);
                    move->forward_angle = 0x400;
                    move->forward_speed = speed;
                    PogoPlay[arg0].spin = 0;
                }
                if ((bits & 0x20) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 1, 0, 0) != 0) {
                    Player_SetReserve(arg0, x + 1, z);
                    move->forward_angle = 0xC00;
                    move->forward_speed = speed;
                    PogoPlay[arg0].spin = 0;
                }
                if ((bits & 0x10) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, 1, 0) != 0) {
                    Player_SetReserve(arg0, x, z + 1);
                    move->forward_angle = 0x800;
                    move->forward_speed = speed;
                    PogoPlay[arg0].spin = 0;
                }
                if ((bits & 0x40) && PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, -1, 0) != 0) {
                    Player_SetReserve(arg0, x, z - 1);
                    move->forward_angle = 0;
                    move->forward_speed = speed;
                    PogoPlay[arg0].spin = 0;
                }
            }
        }
    }
    move->facing_angle = move->forward_angle;
    } else {
        Player_RemoveReserve(arg0);
    }
}

void PostMovePOGO(u32 i) {
    VECTOR v;
    subengine *sub;
    s32 name;
    s32 n;

    name = Heroes[i].hero_name;
    if ((u32)(name - 3) < 2) {
        sub = Heroes[i].hero_fx->sub;
        if (sub != NULL && sub_find_point(Heroes[i].hero_eng, Heroes[i].hero_sub, 0x200, &v) != NULL) {
            sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            sub->unk50 = 1;
            sub->flags |= 0x8000;
            Heroes[i].hero_fx->unk5D = 1;
            Heroes[i].hero_fx->unk10 = *(SLONG_XYZ *)&v;
            Heroes[i].hero_fx->unk1E = Heroes[i].hero_eng->unk1E;
            Heroes[i].hero_fx->unk5E = 1;
            if (Heroes[i].hero_mode == 0x3A) {
                PogoBounceCount[i] += 5;
                if (PogoBounceCount[i] > 45) {
                    PogoBounceCount[i] = 45;
                }
            } else {
                PogoBounceCount[i] -= 2;
                if (PogoBounceCount[i] < 5) {
                    PogoBounceCount[i] = 5;
                }
            }
            Heroes[i].hero_fx->unk1C = (PogoBounceCount[i] << 12) / 360;
            switch (name) {
            case 3:
                n = CortexSmokeFrame;
                sub->entity = 0x2021;
                break;
            case 4:
                n = BrioSmokeFrame;
                sub->entity = 0x2021;
                break;
            default:
                sub->entity = 0;
                n = 0;
                break;
            }
            if (n > 20) {
                n = 0;
            }
            if (n != 20 && n != 0) {
                n = Random(16);
            }
            sub->unk50 = 1;
            sub->unk20.x = 0x1000;
            sub->unk20.z = 0x1000;
            sub->unk51 = 0;
            sub->unk20.y = PogoBounceHeights[n];
            sub->unk52 = 1;
        }
    }
}

bool TestHeroPogo(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTheromove *m = h->hero_move;
    EXTengine *e;
    s32 angle;
    s32 t;
    s32 u;
    s16 sx;
    s16 sz;
    s32 x;
    s32 z;

    if (m->jump_ctrl & 2) {
        if (m->forward_speed != 0) {
            angle = m->forward_angle;
            e = h->hero_eng;
            sx = rcossin_tbl[(angle - 0x400) & 0xFFF].unk2;
            sz = rcossin_tbl[angle & 0xFFF].unk2;
            t = e->unk10.x;
            t -= sx * 512 >> 12;
            t += 0x800;
            x = t >> 9;
            u = e->unk10.z - (sz * 512 >> 12);
            u += 0x800;
            z = u >> 9;
            if ((u32)x >= 8 || z < 0 || z >= 8) {
                Heroes[arg0].hero_move->forward_speed = 0;
                return 0;
            }
            if (PogoAI_IsSquareOKForPlayer(arg0, x, z, 0, 0, 0) != 0) {
                SetHeroAction(arg0, 0x3A);
                h->hero_move->forward_speed = 0;
                return 1;
            }
            h->hero_move->forward_speed = 0;
        }
    }
    return 0;
}


void HeroPogo(u32 arg0) {
    HeroInfo *h;
    HeroInfo *hero;
    HeroInfo *hb;
    HeroInfo *hb2;
    s32 *tbl;
    PogoPlayer *slot;
    PogoPlayer *sb;
    EXTengine *shadow;
    s32 n;
    s32 max;
    s32 name;

    hb = Heroes;
    h = &hb[arg0];
    if (h->hero_name == 4) {
        BrioSmokeFrame = 0;
    }
    if (h->hero_name == 3) {
        CortexSmokeFrame = 0;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    if (TestHeroPogoShoot(arg0) != 0) {
        return;
    }
    if (h->hero_move->jump_ctrl & 0x40) {
        h->hero_move->forward_speed = Player_CalcForwardSpeed(arg0, PogoPlay[arg0].forward_quick);
    } else {
        h->hero_move->forward_speed = 0;
    }
    sb = PogoPlay;
    slot = &sb[arg0];
    if (slot->forward_quick != 0) {
        slot->forward_frame++;
    }
    tbl = PogoTilesMid;
    slot->forward_frame++;
    n = (u16)slot->forward_frame;
    hb2 = Heroes;
    hero = &hb2[arg0];
    max = tbl[hero->hero_name];
    if (n < 0) {
        n = 0;
    }
    if (n >= max) {
        n = max;
    }
    shadow = hero->hero_shadow;
    if (shadow != NULL) {
        shadow->unk2C.x = shadow->unk2C.y = shadow->unk2C.z = (abs(max / 2 - n) << 8) + 0xC00;
        shadow->unk5E = 1;
    }
    if (n <= PogoTilesLow[hero->hero_name] || max - PogoTilesHigh[hero->hero_name] < n) {
        hero->hero_move->jump_ctrl &= 0xBF;
    } else {
        hero->hero_move->jump_ctrl |= 0x40;
    }
    if ((u16)PogoPlay[arg0].forward_frame >= max) {
        Player_SnapToTile(arg0);
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        Heroes[arg0].hero_move->velocity_enable = 0;
        Heroes[arg0].hero_move->forward_speed = 0;
        Heroes[arg0].hero_move->jump_max = 0;
        if (TestHeroPogoWinner(arg0) != 0) {
            return;
        }
        if (TestHeroPogoLoser(arg0) != 0) {
            return;
        }
        if (TestHeroPogoTied(arg0) != 0) {
            return;
        }
        SetHeroAction(arg0, 0x39);
    }
    switch (Heroes[arg0].hero_name) {
    case 1:
        n = PogoFramesCrash[n];
        if (n == 0x4111) {
            AddSomeDust(arg0);
        }
        break;
    case 2:
        n = PogoFramesCoco[n];
        if (n == 0x408E) {
            AddSomeDust(arg0);
        }
        break;
    case 5:
        n = PogoFramesTiny[n];
        if (n == 0x418E) {
            AddSomeDust(arg0);
        }
        break;
    case 6:
        n = PogoFramesKong[n];
        if (n == 0x410E) {
            AddSomeDust(arg0);
        }
        break;
    case 8:
        n = PogoFramesKangaroo[n];
        if (n == 0x4102) {
            AddDingoSmoke(arg0);
        }
        break;
    case 7:
        n = PogoFramesDingo[n];
        if (n == 0x4082) {
            AddDingoSmoke(arg0);
        }
        break;
    case 4:
        BrioSmokeFrame = n;
        n = PogoFramesBrio[n];
        if (n == 0x4105) {
            AddBrioSmoke(arg0);
        }
        break;
    case 3:
        CortexSmokeFrame = n;
        n = PogoFramesCortex[n];
        if (n == 0x4085) {
            AddBrioSmoke(arg0);
        }
        break;
    }
    Heroes[arg0].hero_eng->sub->entity = n;
}

void SetHeroPogoIdle(u32 arg0) {
    s32 seq;

    Player_RemoveReserve(arg0);
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_move->jump_ctrl |= 0x22;
    Heroes[arg0].hero_move->jump_ctrl &= 0xEF;
    Player_SnapToTile(arg0);
    if (PogoPlay[arg0].status == 5) {
        SetHeroSeq(arg0, 0x2F);
    } else {
        SetHeroSeq(arg0, 0x2E);
    }
}

void HeroPogoIdle(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 v;

    if (TestHeroPogoWinner(arg0) != 0) {
        return;
    }
    if (TestHeroPogoLoser(arg0) != 0) {
        return;
    }
    if (TestHeroPogoTied(arg0) != 0) {
        return;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    if (TestHeroPogoShoot(arg0) != 0) {
        return;
    }
    if (TestHeroPogo(arg0) != 0) {
        return;
    }
    switch (ProcessHeroAnim(arg0)) {
    case 0x12:
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        if (PogoPlay[arg0].status != 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroSeq(arg0, 0x2E);
        }
        break;
    case 0x11:
        AddDingoSmoke(arg0);
        break;
    case 0x14:
        AddSomeDust(arg0);
        break;
    case 0x13:
        AddBrioSmoke(arg0);
        break;
    }
    base = Heroes;
    h = &base[arg0];
    switch (h->hero_name) {
    case 4:
        v = h->hero_sub->entity - 0x4080;
        BrioSmokeFrame = v;
        if (v > 0x10) {
            BrioSmokeFrame = 0;
        }
        break;
    case 3:
        v = h->hero_sub->entity - 0x4000;
        CortexSmokeFrame = v;
        if (v > 0x10) {
            CortexSmokeFrame = 0;
        }
        break;
    }
    HeroTestFall(arg0);
}

void HeroPogoTazing(u32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
    EXTheromove *mv;
    HeroInfo *base;
    s32 d;

    if (TestHeroPogoWinner(arg0) != 0) {
        return;
    }
    if (TestHeroPogoLoser(arg0) != 0) {
        return;
    }
    if (TestHeroPogoTied(arg0) != 0) {
        return;
    }
    if (TestHeroPogoDazed(arg0) != 0) {
        return;
    }
    Player_RampToTile(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_frozen = 10;
    if (ProcessHeroAnim(arg0) == 0x14) {
        AddTazerRing(arg0);
    }
    eng = h->hero_eng;
    if (eng != NULL) {
        mv = h->hero_move;
        if (mv != NULL) {
            d = angle_diff(eng->unk1E, mv->facing_angle);
            eng->unk5E = 1;
            eng->unk1E += d >> 3;
        }
    }
    if (--PogoPlay[arg0].timer <= 0) {
        Heroes[arg0].hero_frozen = 0;
        HeroPogoTazingCancel(arg0);
        SetHeroAction(arg0, 0x39);
    }
    HeroTestFall(arg0);
}

void HeroPogoWinner(u32 arg0) {
    EXTengine *eng;
    u8 *g;

    Heroes[arg0].hero_frozen = 2;
    Pickup_Lose(arg0);
    if (UpdateHeroVictory(arg0) != 0) {
        g = GameInfo;
        if (*(s32 *)(g + 4) & 0x800) {
            *(u16 *)(g + 0x30) |= 0x201;
        } else {
            *(u16 *)(g + 0x30) |= 1;
        }
    }
    switch (ProcessHeroAnim(arg0)) {
    case 0x12:
        PlayBoingNoise(arg0);
        Set_Tile_Under_Player(arg0, 1);
        break;
    case 0x11:
        AddDingoSmoke(arg0);
        break;
    case 0x14:
        AddSomeDust(arg0);
        break;
    case 0x13:
        AddBrioSmoke(arg0);
        break;
    }
    switch (Heroes[arg0].hero_name) {
    case 4:
        BrioSmokeFrame = Random(0x10);
        break;
    case 3:
        CortexSmokeFrame = Random(0x10);
        break;
    }
    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        s32 r = angle_diff_abs(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
}

void SetHeroPogoLoser(u32 arg0) {
    s32 *pos;
    NAS *nas;
    s32 *ext;
    u8 *p;

    Player_RemoveReserve(arg0);
    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_frozen = 2;
    Pickup_Lose(arg0);
    Player_SnapToTile(arg0);
    Heroes[arg0].hero_move->jump_vel = 0x30;
    Heroes[arg0].hero_move->jump_max = 0x90;
    Heroes[arg0].hero_move->jump_acc = 0;
    Heroes[arg0].hero_move->jump_dec = 0;
    Heroes[arg0].hero_move->jump_dir = -1;
    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = &HeroModelBank[DB_PogoObjects];
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 0;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    RemoveCamera(Heroes[arg0].hero);
    Heroes[arg0].hero_shadow->flags &= ~0x8000;
}

void HeroPogoLoser(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *eng;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    u8 *p;

    Pickup_Lose(arg0);
    base = Heroes;
    h = &base[arg0];
    eng = h->hero_eng;
    h->hero_frozen = 2;
    if (eng != NULL) {
        eng->unk1E = (eng->unk1E + 0xE3) % 4096;
        eng->unk5E = 1;
        if (((GameFrameCount + arg0) & 3) == 0) {
            pos = (s32 *)&eng->unk10;
            p = &HeroModelBank[DB_PogoObjects];
            nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    ext[3] = 2;
                    ext[6] = 0x100;
                    if (p != NULL) {
                        ext[8] = 5;
                        ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
                    } else {
                        ext[8] = 1;
                    }
                    ext[10] = 0;
                    nas->InitDelete(nas);
                }
            }
        }
    }
}

bool TestHeroPogoDazed(u32 arg0) {
    HeroInfo *h;
    s32 x;
    s32 z;
    PogoCell *c;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    if (!(h->hero_status & 0x800) && !(PogoPlay[arg0].flags & 1)) {
        return 0;
    }
    x = (h->hero_eng->unk10.x + 0x800) >> 9;
    z = (h->hero_eng->unk10.z + 0x800) >> 9;
    if (h->hero_mode != 0x3B && h->hero_last_mode != 0x3B) {
        switch (h->hero_name) {
        case 1:
            PlaySfx(3, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x67, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x35, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 5:
            PlaySfx(0x12F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 6:
            PlaySfx(0x161, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 7:
            PlaySfx(0xFD, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PlaySfx(0x99, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 8:
            PlaySfx(0xCB, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
    SetHeroAction(arg0, 0x3B);
    if ((u32)x < 8 && z >= 0 && z < 8) {
        PogoCell *cb = PogoGrid[0];
        c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    return 1;
}

void SetHeroPogoDazed(u32 arg0) {
    s32 x;
    s32 z;
    PogoCell *c;
    HeroInfo *h;
    HeroInfo *base;

    Player_RemoveReserve(arg0);
    if (PogoPlay[arg0].tazer != NULL) {
        HeroPogoTazingCancel(arg0);
    }
    Pickup_Lose(arg0);
    pad_rumble_small(arg0, 0x1E);
    Player_SnapToTile(arg0);
    x = (Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9;
    z = (Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9;
    if ((u32)x < 8 && z >= 0 && z < 8) {
        PogoCell *cb = PogoGrid[0];
        c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 10;
    PogoPlay[arg0].spin = 0;
    SetHeroSeq(arg0, 0x31);
}

void HeroPogoDazed(u32 arg0) {
    s32 x;
    s32 z;
    PogoCell *c;
    HeroInfo *h;
    HeroInfo *base;
    PogoPlayer *s;
    EXTengine *eng;

    x = (Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9;
    z = (Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9;
    if ((u32)x < 8 && z >= 0 && z < 8) {
        PogoCell *cb = PogoGrid[0];
        c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    if (Pogo_StatusAction(arg0, 0x200, 0x3C, 0x32)) {
        return;
    }
    if (Pogo_StatusAction(arg0, 0x400, 0x3D, 0x2E)) {
        return;
    }
    if (Pogo_StatusAction(arg0, 0x100, 0x3E, 0x2E)) {
        return;
    }
    base = Heroes;
    h = &base[arg0];
    h->hero_frozen = 10;
    s = &PogoPlay[arg0];
    if (!(s->flags & 1)) {
        s->spin++;
    }
    if ((u32)s->spin > 0x32) {
        h->hero_status &= ~0x800;
        SetHeroAction(arg0, 0x39);
    }
    if (s->flags & 1) {
        eng = h->hero_eng;
        if (eng != NULL) {
            s32 r = angle_diff_abs(0, eng->unk1E);
            eng->unk5E = 1;
            eng->unk1E += r >> 3;
        }
    }
    HeroTestFall(arg0);
    ProcessHeroAnim(arg0);
    switch (Heroes[arg0].hero_name) {
    case 4:
        if (BrioSmokeFrame == 0) {
            BrioSmokeFrame = 0x14;
        } else {
            BrioSmokeFrame = 0;
        }
        break;
    case 3:
        if (CortexSmokeFrame == 0) {
            CortexSmokeFrame = 0x14;
        } else {
            CortexSmokeFrame = 0;
        }
        break;
    }
}

void AddDingoSmoke(u32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = &HeroModelBank[DB_PogoObjects];
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 6;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                if (s->entity != 0x2022) {
                    s->flags |= 0x20000000;
                }
            }
        }
    }
}

void AddSomeDust(u32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = &HeroModelBank[DB_PogoObjects];
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void AddBrioSmoke(u32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = &HeroModelBank[DB_PogoObjects];
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void PogoHero_ExtraEffects(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    s32 state;
    s32 dy;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    pu = h->hero_powerup;
    if (pu != NULL) {
        sub = pu->sub;
        if (sub != NULL) {
            state = (u16)PogoPlay[arg0].status;
            if (state >= 7) {
                goto clear;
            }
            if (state != 0) {
                if (sub_find_point(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
                    pu->unk10 = *(SLONG_XYZ *)&pos;
                    dy = PogoHeroHeights[h->hero_name];
                } else {
                    dy = 0x280;
                    pu->unk10 = h->hero_eng->unk10;
                }
                pu->unk5E = 1;
                pu->unk10.y -= dy;
                pu->unk1E += 0x71;
                if ((u16)PogoPlay[arg0].status != 6 || PogoPlay[arg0].timer >= 0x5B ||
                    !((GameFrameCount >> 2) & 1)) {
                    sub->flags |= 0x8000;
                    pu->flags |= 0x8000;
                    return;
                }
            }
        clear:
            sub->flags &= ~0x8000;
            pu->flags &= ~0x8000;
        }
    }
}

s32 Bank_Tiles(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;
    EXTengine *eng;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (PogoGrid[i][j].unkA == arg0) {
                Tile_FizzleOut(i, j, PogoGrid[i][j].unkA, 10);
                count++;
            }
        }
    }
    if (count != 0) {
        eng = Heroes[arg0].hero_eng;
        Handle_Score(arg0, (eng->unk10.x + 0x800) >> 9, (eng->unk10.z + 0x800) >> 9, count);
    }
    return count;
}

void Handle_Score(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    PogoScoreView *g = SCORE60V;
    s32 score;

    if (g->unk30 & 0x801) {
        return;
    }
    if (arg3 != 0) {
        CreateFloatyPogoScore(arg0, arg1, arg2, arg3);
    }
    if (g->flags & 0x40) {
        if (g->teams != 0) {
            score = Heroes[arg0].hero_score;
            if (score < arg3) {
                Heroes[arg0].hero_score = 0;
                Heroes[Heroes[arg0].hero_teammate].hero_score -= arg3 - score;
                return;
            }
        }
        Heroes[arg0].hero_score -= arg3;
    } else if ((g->flags & 0x80) && g->mode == 0xB) {
        if (Heroes[arg0].hero_score < arg3) {
            Heroes[arg0].hero_score = arg3;
        }
    } else {
        Heroes[arg0].hero_score += arg3;
    }
}

void Tile_Claim(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *p;
    PogoCell *base;

    if (PogoPlay[arg2].status == 6) {
        base = PogoGrid[0];
        p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
        p->unk12 = 10;
        p->unkA = arg2;
        p->unk14 = 0xF0;
        return;
    }
    {
        PogoCell *b2 = PogoGrid[0];
        p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)b2);
    }
    p->unk12 = 1;
    p->unkA = arg2;
    if (GameMode == 0xB) {
        if (arg0 > 0) {
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
        }
    }
}


void Tile_Update(s32 arg0, s32 arg1) {
    s32 rnd;
    s32 anim;
    PogoModeView *g = (PogoModeView *)GameInfo;
    s32 row = g->mode - 10;
    u16 *tbl;
    PogoCell *c;
    PogoCell *cell;
    PogoCell *cell5;
    PogoCell *cell2;
    PogoCell *cell3;
    PogoCell *c0;
    PogoCell *base;
    PogoCell *base1;
    PogoCell *base10;
    PogoCell *base0;
    PogoCell *base2;
    PogoCell *base3;
    PogoCell *base5;
    s32 fr;
    s32 fr5;
    PogoCellObj *o;
    s32 n;

    if ((u32)arg0 < 8 && (u32)arg1 < 8) {
        rnd = LevelSpecific_UnclaimedTile(arg0, arg1);
        anim = LevelSpecific_ClaimedTile(arg0, arg1);
        if (g->flags & 0x80) {
            tbl = TileFramesAlt[row];
        } else {
            tbl = TileFrames[row];
        }
        base = PogoGrid[0];
        c0 = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
        o = (PogoCellObj *)c0->unk4;
        if (o == NULL) {
            return;
        }
        switch ((u16)c0->unk12) {
        case 0:
            base0 = PogoGrid[0];
            c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base0);
            if (c->unk16 == -1) {
                if (GameFrameCount & 1) {
                    CELL_SET_T(o, rnd, rnd);
                } else {
                    CELL_SET_T(o, anim, tbl[c->unkA]);
                }
            } else {
                o->anim = anim;
                if (c->unk16 != c->unkA) {
                    if (GameFrameCount & 1) {
                        o->frame = CELL_FRAME(o, tbl[c->unk16] & 0xFFF)->unk26;
                    } else {
                        o->frame = CELL_FRAME(o, tbl[c->unkA] & 0xFFF)->unk26;
                    }
                }
            }
            break;
        case 1:
            base1 = PogoGrid[0];
            c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base1);
            if (c->unkA == -1) {
                CELL_SET_T(o, rnd, rnd);
            } else {
                CELL_SET_T(o, anim, tbl[c->unkA]);
            }
            break;
        case 10:
            base10 = PogoGrid[0];
            cell = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base10);
            if (cell->unkA != -1) {
                CELL_SET_T(o, (MV->flags & 0x80) ? 0x3045 : 0x3031, tbl[cell->unkA]);
            }
            break;
        case 2:
            base2 = PogoGrid[0];
            cell2 = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base2);
            CELL_SET_T(o, anim, tbl[cell2->unk16]);
            break;
        case 3:
            if (GameFrameCount & 1) {
                base3 = PogoGrid[0];
            cell3 = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base3);
                CELL_SET_T(o, anim, tbl[cell3->unk16]);
            } else {
                CELL_SET_T(o, rnd, rnd);
            }
            break;
        case 5:
            base5 = PogoGrid[0];
            cell5 = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base5);
            n = (s16)(cell5->unk14 / 30);
            if (n >= 4) {
                n = 3;
            }
            fr5 = (MV->flags & 0x80) ? TileStepFramesAlt[n] : TileStepFrames[n];
            o->anim = fr5;
            fr5 = (MV->flags & 0x80) ? TileStepFramesAlt[n] : TileStepFrames[n];
            o->frame = CELL_FRAME(o, fr5 & 0xFFF)->unk26;
            break;
        case 7:
            CELL_SET(o, (MV->flags & 0x80) ? 0x304A : 0x3040, (MV->flags & 0x80) ? 0x304A : 0x3040);
            break;
        case 6:
            CELL_SET(o, (MV->flags & 0x80) ? 0x304F : 0x3045, (MV->flags & 0x80) ? 0x304F : 0x3045);
            break;
        }
    }
}

void Tile_FireTnt(s32 arg0, s32 arg1) {
    HeroInfo *h;
    PogoCell *c;
    PogoCell *base;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    s32 i;
    s32 near;
    s32 one;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        near = 0;
        if (abs((arg0 << 9) - (((h->hero_eng->unk10.x + 0x800) >> 9) << 9)) < 0x300) {
            near = abs((arg1 << 9) - (((h->hero_eng->unk10.z + 0x800) >> 9) << 9)) < 0x300;
        }
        if (near && !(h->hero_status & 0x800)) {
            Player_SetDazed(i);
        }
    }
    base = PogoGrid[0];
    c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    c->unk12 = 1;
    c->unkA = -1;
    c->unk14 = 0;
    pos = &c->unk4->x;
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    one = 1;
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = one;
            ext[6] = 0x100;
            ext[8] = one;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    switch ((u32)Random(4)) {
    case 0:
        PlaySfx(0x3EC, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x3ED, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x3EE, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x3EF, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    }
}

void Tile_FireNitro(s32 arg0, s32 arg1) {
    HeroInfo *h;
    PogoCell *c;
    PogoCell *base;
    s32 *pos;
    NAS *nas;
    s32 *ext;
    s32 i;
    s32 near;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        near = 0;
        if (abs((arg0 << 9) - (((h->hero_eng->unk10.x + 0x800) >> 9) << 9)) < 0x300) {
            near = abs((arg1 << 9) - (((h->hero_eng->unk10.z + 0x800) >> 9) << 9)) < 0x300;
        }
        if (near && !(h->hero_status & 0x800)) {
            if (((PogoModeView *)GameInfo)->mode == 0xC
                && (((PogoModeView *)GameInfo)->flags & 0x80)
                && ((PogoModeView *)GameInfo)->out[i] == 0) {
                Player_SetPoisoned(i);
                PlaySfx(0x529, NULL, (SLONG_XYZ *)&h->hero_eng->unk10, 0x1000, 0x1E00, 0);
            } else {
                Player_SetDazed(i);
            }
        }
    }
    base = PogoGrid[0];
    c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    c->unk12 = 1;
    c->unkA = -1;
    c->unk14 = 0;
    Tile_Update(arg0, arg1);
    pos = &c->unk4->x;
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 2;
            ext[6] = 0x100;
            ext[8] = 1;
            ext[10] = 0;
            nas->InitDelete(nas);
        }
    }
    switch ((u32)Random(4)) {
    case 0:
        PlaySfx(0x3F1, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x3F2, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x3F3, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x3F4, NULL, CELL_POS(arg0, arg1), 0x1000, 0x1E00, 0);
        break;
    }
}

void ZapInDir(s32 arg0, s32 arg1, s32 dx, s32 dz, const s32 arg4) {
    s32 z;
    s32 x;
    PogoCell *c;
    u8 n;
    s16 t;
    s16 old;
    u8 *g;

    x = arg0;
    z = arg1;
    n = 2;
    t = 6;
    for (;;) {
        if ((u32)x >= 8 || (u32)z >= 8) {
            goto end;
        }
        c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)PogoGrid);
        if ((u16)c->unk12 - 5 >= 3u) {
            old = c->unkA;
            c->unk14 = t;
            c->unk12 = 0;
            c->unkA = arg4;
            c->unk16 = old;
            g = GameInfo;
            if (*(s16 *)(g + 0x34) == 0xB) {
                if (x > 0) {
                    Tile_Update(x - 1, z);
                }
                if (x < 7) {
                    Tile_Update(x + 1, z);
                }
                if (z > 0) {
                    Tile_Update(x, z - 1);
                }
                if (z < 7) {
                    Tile_Update(x, z + 1);
                }
            }
            ((PogoCell *)(x * sizeof(PogoGrid[0]) + z * sizeof(PogoCell) + (u32)PogoGrid))->unk18[0] = n;
            n++;
            t += 4;
        }
        x += dx;
        z += dz;
    }
end:;
}

void Update_Tiles(void) {
    s32 off;
    PogoCell *base;
    s32 j;
    s32 i;
    s32 vol;
    s32 rowOff;
    s32 sfx;
    s16 t;

    i = 0;
    base = PogoGrid[0];
    vol = 0x1E00;
    rowOff = 0;
loop_i:
    j = 0;
    if (i < 8) {
        off = rowOff;
loop_j:
        if (j < 8) {
            if (GameMode == 13) {
                if ((*(u16 *)CELL_AT(off)->unkC & 0x10) && CELL_AT(off)->unkE == 0) {
                    *(u16 *)CELL_AT(off)->unkC &= ~0x10;
                }
            }
            switch ((u16)CELL_AT(off)->unk12) {
            case 5:
                t = --CELL_AT(off)->unk14;
                if (CELL_AT(off)->unk18[0] != 0) {
                    switch (t) {
                    case 10:
                    case 20:
                    case 30:
                    case 60:
                    case 90:
                        do {
                            do {
                                PlaySfx(0x3F0, NULL, (SLONG_XYZ *)&CELL_AT(off)->unk4->x, 0x1000, vol, 0);
                            } while (0);
                        } while (0);
                        break;
                    }
                }
                if (CELL_AT(off)->unk14 < 2) {
                    Tile_FireTnt(i, j);
                }
                Tile_Update(i, j);
                break;
            case 6:
                if (--CELL_AT(off)->unk14 < 2) {
                    Tile_FireNitro(i, j);
                }
                break;
            case 0:
                do {
                if (--CELL_AT(off)->unk14 < 2) {
                    CELL_AT(off)->unk12 = 1;
                    switch (CELL_AT(off)->unk18[0]) {
                    case 2:
                        sfx = 0x516;
                        goto play;
                    case 3:
                        sfx = 0x517;
                        goto play;
                    case 4:
                        sfx = 0x518;
                        goto play;
                    case 5:
                        sfx = 0x519;
                        goto play;
                    case 6:
                        sfx = 0x51A;
                        goto play;
                    case 7:
                        sfx = 0x51B;
                        goto play;
                    case 8:
                        sfx = 0x51C;
                    play:
                        do {
                            do {
                                PlaySfx(sfx, NULL, (SLONG_XYZ *)&CELL_AT(off)->unk4->x, 0x1000, vol, 0);
                            } while (0);
                        } while (0);
                        break;
                    }
                    CELL_AT(off)->unk18[0] = 0;
                }
                } while (0);
                Tile_Update(i, j);
                break;
            case 2:
                do {
                    if (--CELL_AT(off)->unk14 < 2) {
                        CELL_AT(off)->unk12 = 3;
                        CELL_AT(off)->unk14 = 10;
                    }
                } while (0);
                Tile_Update(i, j);
                break;
            case 3:
                do {
                    if (--CELL_AT(off)->unk14 < 2) {
                        CELL_AT(off)->unk12 = 1;
                        CELL_AT(off)->unkA = -1;
                        CELL_AT(off)->unk14 = 0;
                    }
                } while (0);
                Tile_Update(i, j);
                break;
            case 1:
            case 4:
            case 7:
            case 8:
            case 9:
            case 10:
                break;
            }
            off += sizeof(PogoCell);
            j++;
            goto loop_j;
        }
        rowOff += sizeof(PogoGrid[0]);
        i++;
        goto loop_i;
    }
}

void PlayBoingNoise(u32 arg0) {
    switch (Heroes[arg0].hero_name) {
    case 4:
        PlaySfx(0x561, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x560, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x562, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x563, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x55E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x55F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 7:
        PlaySfx(0x564, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 8:
        PlaySfx(0x565, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void Set_Tile_Under_Player(u32 arg0, bool arg1) {
    EXTengine *eng;
    PogoCell *c;
    PogoCell *p;
    s32 x;
    s32 z;
    s32 owner;

    eng = Heroes[arg0].hero_eng;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    if ((u32)x >= 8) {
        return;
    }
    if ((u32)z >= 8) {
        return;
    }
    {
        PogoCell *cb = PogoGrid[0];
        c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
    }
    c->unk10 = 0;
    if (arg1 != 0) {
        PogoAIPlay[arg0].unk4 = 1;
    }
    if (c->unkA != arg0) {
        if (arg1 != 0) {
            PlaySquareChangeNoise(arg0);
        }
        *(u16 *)c->unkC |= 0x10;
    }
    if (SetTile_PogoThree(arg0, x, z, arg1) != 0) {
        return;
    }
    if (SetTile_PogoFour(arg0, x, z, arg1) != 0) {
        return;
    }
    {
        PogoCell *cb = PogoGrid[0];
        p = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
    }
    owner = p->unkA;
    if (owner == -1) {
        Tile_Claim(x, z, arg0);
        Tile_Update(x, z);
        Tiles_CheckForIslands(arg0);
        return;
    }
    if (owner == arg0) {
        return;
    }
    switch ((u16)p->unk12) {
    case 1:
    case 3:
        Tile_Claim(x, z, arg0);
        Tile_Update(x, z);
        Tiles_CheckForIslands(arg0);
        break;
    case 5:
        Tile_FireTnt(x, z);
        Tile_Update(x, z);
        break;
    case 6:
        Tile_FireNitro(x, z);
        Tile_Update(x, z);
        break;
    }
}

void PlaySquareChangeNoise(u32 arg0) {
    switch (arg0) {
    case 0:
        PlaySfx(0x55A, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x55B, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x55C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x55D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void PlaySquareLockedNoise(u32 arg0) {
    switch (arg0) {
    case 0:
        PlaySfx(0x54E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x54F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x550, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x551, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void PogoFour_PlayerHasStoodOnOwnTile(s32 arg0) {
    s32 i;
    s32 j;
    s32 n;
    PogoCell *c;

    n = 0;
    for (i = 0;; i++) {
        if (i >= 8) goto done;
        for (j = 0;; j++) {
            if (j >= 8) goto next;
            c = (PogoCell *)(i * sizeof(PogoGrid[0]) + j * sizeof(PogoCell) + (u32)PogoGrid);
            if (c->unkA == arg0) {
                c->unk12 = 1;
                c->unkA = -1;
                c->unk14 = 0;
                Tile_Update(i, j);
                n++;
            }
        }
    next:;
    }
done:
    if (n != 0) {
        PlaySfx(0x54B, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void PogoFour_FreezeMyTiles(s32 arg0) {
    NAS *nas;
    s32 i;
    s32 j;
    s32 n;
    u16 st;
    PogoCell *c;

    nas = PogoPadlockNas;
    if (nas == NULL) {
        return;
    }
    n = 0;
    if (nas->UserInfo.status < 3) {
        return;
    }
    nas->UserInfo.status = 6;
    i = 0;
    for (;;) {
        j = 0;
        if (i >= 8) {
            goto done;
        }
        for (;;) {
            if (j >= 8) {
                goto next;
            }
            c = (PogoCell *)(j * sizeof(PogoCell) + i * sizeof(PogoGrid[0]) + (u32)PogoGrid);
            st = c->unk12;
            if (st == 1 && c->unkA == arg0) {
                c->unk12 = 10;
                c->unkA = arg0;
                c->unk14 = 0xF0;
                Tile_Update(i, j);
                if (n == 0) {
                    c->unk18[0] = st;
                } else {
                    c->unk18[0] = 0;
                }
                n++;
            }
            j++;
        }
    next:
        i++;
    }
done:
    if (n != 0) {
        PlaySfx(0x54C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void PogoFour_UnfreezeMyTiles(s32 arg0) {
    s32 i;
    s32 j;
    s32 n;
    PogoCell *c;

    n = 0;
    i = 0;
    for (;;) {
        j = 0;
        if (i >= 8) {
            goto done;
        }
        for (;;) {
            if (j >= 8) {
                goto next;
            }
            c = (PogoCell *)(j * sizeof(PogoCell) + i * sizeof(PogoGrid[0]) + (u32)PogoGrid);
            if ((u16)c->unk12 == 10 && c->unkA == arg0) {
                c->unk12 = 1;
                c->unkA = arg0;
                if (((D5A614View *)GameInfo)->mode == 0xB) {
                    if (i > 0) {
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
                    }
                }
                Tile_Update(i, j);
                n++;
            }
            j++;
        }
    next:
        i++;
    }
done:
    if (n != 0) {
        PlaySfx(0x54D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

void Pickup_Claim(s32 arg0, s32 arg1, s32 arg2) {
    NAS *nas;
    HeroInfo *h;
    PogoPlayer *slot;

    if (PogoPadlock_IsHere(arg0, arg1)) {
        PogoPlayer *s;

        Pickup_Lose(arg2);
        s = PogoPlay;
        s[arg2].status = 6;
        s[arg2].timer = 0xF0;
        Pogo_PowerupAnim(arg2, 0x2042);
        PogoFour_FreezeMyTiles(arg2);
        return;
    }
    nas = (NAS *)PogoGrid[arg0][arg1].hero;
    if (nas == NULL) {
        return;
    }
    if (GameMode != 0xD) {
        Set_Tile_Under_Player(arg2, 1);
    }
    switch (nas->UserInfo.flags) {
    case 10:
        PlaySfx(0x514, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        Bank_Tiles(arg2);
        nas->UserInfo.status = 4;
        break;
    case 11:
        nas->UserInfo.status = 5;
        if (GameInfo[arg2 + 0x5C] == 0 && !(*(u16 *)(GameInfo + 0x30) & 0x801)) {
            Player_SetPoisoned(arg2);
            PlaySfx(0x529, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
        break;
    case 5: {
        PogoPlayer *b = PogoPlay;

        slot = &b[arg2];
    }
        if (slot->status != 5) {
            Pickup_Lose(arg2);
        }
        {
            HeroInfo *hb = Heroes;

            h = &hb[arg2];
        }
        PlaySfx(0x41C, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        slot->status = 5;
        slot->timer = 0xF0;
        Pogo_PowerupAnim(arg2, 0x200C);
        number_of_powerups--;
        break;
    case 7:
        Pogo_ZapArrow(arg0, arg1, arg2, 1);
        number_of_pickups--;
        break;
    case 8:
        Pogo_ZapArrow(arg0, arg1, arg2, 2);
        Tiles_CheckForIslands(arg2);
        number_of_pickups--;
        break;
    case 1:
        Pogo_PickupSet(arg2, 1);
        Pogo_PowerupAnim(arg2, 0x200D);
        number_of_powerups--;
        PlaySfx(0x51F, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (GameInfo[arg2 + 0x5C] != 0) {
            PogoAI_HeroClaimedPickup(arg2);
        }
        break;
    case 2:
        Pogo_PickupSet(arg2, 2);
        Pogo_PowerupAnim(arg2, 0x201B);
        number_of_powerups--;
        PlaySfx(0x438, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (GameInfo[arg2 + 0x5C] != 0) {
            PogoAI_HeroClaimedPickup(arg2);
        }
        break;
    case 4:
        Pogo_PickupSet(arg2, 4);
        Pogo_PowerupAnim(arg2, 0x2040);
        PlaySfx(0x51F, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (GameInfo[arg2 + 0x5C] != 0) {
            PogoAI_HeroClaimedPickup(arg2);
        }
        number_of_powerups--;
        break;
    case 3:
        Pogo_PickupSet(arg2, 3);
        Pogo_PowerupAnim(arg2, 0x2002);
        number_of_powerups--;
        PlaySfx(0x51E, NULL, &Heroes[arg2].hero_eng->unk10, 0x1000, 0x1E00, 0);
        if (GameInfo[arg2 + 0x5C] != 0) {
            PogoAI_HeroClaimedPickup(arg2);
        }
        break;
    }
    Tiles_CheckForIslands(arg2);
    if (nas->Engine != NULL) {
        if (nas->UserInfo.flags == 10) {
            Pogo_SpawnPuff(&nas->Engine->unk10.x, 3, &HeroModelBank[DB_PogoObjects]);
        } else {
            Pogo_SpawnPuff(&nas->Engine->unk10.x, 1, &HeroModelBank[DB_PogoObjects]);
        }
    }
    if (nas->UserInfo.flags < 10) {
        KillNAS(nas);
    }
    PogoGrid[arg0][arg1].hero = NULL;
}

void PogoPickups_Poll(void) {
    D5A614View *g = (D5A614View *)GameInfo;
    s32 idx = g->mode - 10;
    s32 x, z;

    if (pickup_spawn_time == -1) {
        if (number_of_pickups < PickupsWanted[idx]) {
            pickup_spawn_time = Random(70) + 40;
        }
    } else {
        pickup_spawn_time--;
        if (pickup_spawn_time < 5) {
            if (FindFreeSquare(&x, &z) != 0) {
                if (g->mode == 10) {
                    goto seven;
                }
                if (g->mode == 11) {
                    if (Random(3) == 0) {
                        CreatePickupOnTile(x, z, 8);
                    } else {
                    seven:
                        CreatePickupOnTile(x, z, 7);
                    }
                }
            }
            if (number_of_pickups > PickupsMax[idx]) {
                pickup_spawn_time = -1;
            } else {
                pickup_spawn_time = Random(70) + 40;
            }
        }
    }
}

void PogoPowerups_Poll(void) {
    D5A614View *g = (D5A614View *)GameInfo;
    s32 idx = g->mode - 10;
    s32 x, z;
    u32 r;
    D5A614View *g2;

    if (powerup_spawn_time == -1) {
        if (g->mode == 13 && (g->flags & 0x80)) {
            if (number_of_powerups < 4) {
                powerup_spawn_time = Random(60) + 60;
            }
        } else if (number_of_powerups < PowerupsWanted[idx]) {
            goto rearm;
        }
    } else {
        powerup_spawn_time--;
        if (powerup_spawn_time < 2) {
            if (FindFreeSquare(&x, &z) != 0) {
                switch (g->mode) {
                case 10:
                    if (Random(3) != 0) {
                        CreatePowerupOnTile(x, z, 1);
                    } else {
                        goto five;
                    }
                    break;
                case 11:
                    r = Random(5);
                    if (r == 0) {
                        CreatePowerupOnTile(x, z, 3);
                    } else if (r < 3) {
                        goto five;
                    } else {
                        CreatePowerupOnTile(x, z, 2);
                    }
                    break;
                case 12:
                    switch (Random(2)) {
                    case 0:
                        CreatePowerupOnTile(x, z, 1);
                        break;
                    case 1:
                        goto five;
                    }
                    break;
                case 13:
                    if (g->flags & 0x80) {
                        CreatePowerupOnTile(x, z, 4);
                    } else if (Random(2) == 0) {
                        CreatePowerupOnTile(x, z, 4);
                    } else {
                    five:
                        CreatePowerupOnTile(x, z, 5);
                    }
                    break;
                }
            }
            g2 = (D5A614View *)GameInfo;
            if (g2->mode == 13 && (g2->flags & 0x80)) {
                if (number_of_powerups >= 7) {
                    powerup_spawn_time = -1;
                } else {
                    goto rearm;
                }
            } else if (number_of_powerups > PowerupsMax[idx]) {
                powerup_spawn_time = -1;
            } else {
            rearm:
                powerup_spawn_time = Random(60) + 60;
            }
        }
    }
}

bool Pickup_IsClaimable(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *c;
    PogoCell *base;
    HeroInfo *h;
    s32 lock;

    if ((u32)arg1 < 8 && (u32)arg2 < 8) {
        if (PogoPadlock_IsHere(arg1, arg2) != 0) {
            return 1;
        }
        base = PogoGrid[0];
        c = (PogoCell *)(arg2 * sizeof(PogoCell) + arg1 * sizeof(PogoGrid[0]) + (u32)base);
        h = c->hero;
        if (h == NULL) {
            return 0;
        }
        lock = h->hero_pu_lock;
        if (h->hero_pu_action == 10) {
            if (lock < 3) {
                return 0;
            }
        } else if (lock < 3) {
            return 0;
        }
        if (PogoGrid[arg1][arg2].unk10 == 0 || PogoGrid[arg1][arg2].unk10 == arg0 + 1) {
            goto ok;
        }
    }
    return 0;
ok:
    return 1;
}

bool Pickup_IsClaimableByAI(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *c;
    PogoCell *base;
    HeroInfo *h;
    s32 action;
    s32 lock;
    s32 t;

    if ((u32)arg1 >= 8 || (u32)arg2 >= 8) {
        return 0;
    }
    if (PogoPadlock_IsHere(arg1, arg2) != 0) {
        return 1;
    }
    base = PogoGrid[0];
    c = (PogoCell *)(arg2 * sizeof(PogoCell) + arg1 * sizeof(PogoGrid[0]) + (u32)base);
    if (c->unkE != 0) {
        return 0;
    }
    if (c->unk10 != 0 && c->unk10 != arg0 + 1) {
        return 0;
    }
    {
        PogoCell *b2 = PogoGrid[0];
        c = (PogoCell *)(arg2 * sizeof(PogoCell) + arg1 * sizeof(PogoGrid[0]) + (u32)b2);
    }
    h = c->hero;
    if (h != NULL) {
        action = h->hero_pu_action;
        lock = h->hero_pu_lock;
        t = *(s32 *)&h->hero_weapon_timer;
        if (action == 11) {
            return 0;
        }
        if (t != 0) {
            return 0;
        }
        if (action == 10) {
            if (lock < 3) {
                return 0;
            }
        } else if (lock < 3) {
            return 0;
        }
        return 1;
    }
    return (u16)c->unk12 == 7;
}

void CreatePowerupOnTile(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *c;
    PogoCellPos *pos;
    NAS *nas;
    EXTengine *e;
    PogoCell *base;

    base = PogoGrid[0];
    c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    if (c->hero != NULL || PogoPadlock_IsHere(arg0, arg1) != 0) {
        return;
    }
    pos = c->unk4;
    if (pos == NULL) {
        return;
    }
    nas = CreateNAS(pos->x, World_floor_level, pos->z, 0x1402, 0x11);
    if (nas == NULL) {
        return;
    }
    c->hero = (HeroInfo *)nas;
    e = nas->Engine;
    if (e != NULL) {
        nas->UserInfo.flags = arg2;
        switch (arg2) {
        case 1:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x200D;
            break;
        case 2:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x201B;
            break;
        case 4:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x2040;
            break;
        case 5:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x200C;
            break;
        case 3:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x2002;
            break;
        }
    }
    number_of_powerups++;
}

void CreatePickupOnTile(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *c;
    PogoCellPos *pos;
    NAS *nas;
    EXTengine *e;
    PogoCell *base;

    base = PogoGrid[0];
    c = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    if (c->hero != NULL || PogoPadlock_IsHere(arg0, arg1) != 0) {
        return;
    }
    pos = c->unk4;
    if (pos == NULL) {
        return;
    }
    nas = CreateNAS(pos->x, World_floor_level, pos->z, 0x1401, 0x11);
    if (nas == NULL) {
        return;
    }
    c->hero = (HeroInfo *)nas;
    e = nas->Engine;
    nas->UserInfo.flags = arg2;
    if (e != NULL) {
        switch (arg2) {
        case 7:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x2001;
            nas->UserInfo.user1 = Random(4);
            nas->UserInfo.user2 = 0;
            break;
        case 8:
            e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
            e->sub->entity = 0x200E;
            nas->UserInfo.user1 = Random(4);
            nas->UserInfo.user2 = 0;
            break;
        }
    }
    number_of_pickups++;
}

s32 Fire_Tazer(s32 arg0, s32 arg1) {
    EXTheromove *mv;
    EXTengine *eng;
    EXTengine *e;
    NAS *nas;
    subengine *sub;
    s32 pos[3];
    s32 x;
    s32 z;
    s32 n;
    s32 dx;
    s32 dz;
    s32 t;

    mv = Heroes[arg0].hero_move;
    n = 0;
    if (mv != NULL) {
        dz = 0;
        eng = Heroes[arg0].hero_eng;
        x = (eng->unk10.x + 0x800) >> 9;
        z = (eng->unk10.z + 0x800) >> 9;
        dx = 0;
        switch (mv->facing_angle >> 10) {
        case 0:
            n = z;
            break;
        case 1:
            n = x;
            break;
        case 2:
            n = 7 - z;
            break;
        case 3:
            n = 7 - x;
            break;
        }
        if (n > 0) {
            goto spawn;
        }
    }
    return n;
spawn:
    x = Heroes[arg0].hero_eng->unk10.x;
    z = Heroes[arg0].hero_eng->unk10.z;
    *(SLONG_XYZ *)pos = Heroes[arg0].hero_eng->unk10;
    pos[1] -= 0x80;
    switch (mv->facing_angle >> 10) {
    case 0:
        z -= 0x100;
        dz = -0x200;
        pos[2] -= 0x100;
        break;
    case 1:
        x -= 0x100;
        dx = -0x200;
        pos[0] -= 0x100;
        break;
    case 2:
        z += 0x100;
        dz = 0x200;
        pos[2] += 0x100;
        break;
    case 3:
        x += 0x100;
        dx = 0x200;
        pos[0] += 0x100;
        break;
    }
    if (PogoPlay[arg0].tazer != NULL) {
        return n;
    }
    nas = CreateNAS(x, World_floor_level - 0x40, z, 0x1405, 0x11);
    if (nas == NULL) {
        return n;
    }
    e = nas->Engine;
    if (e == NULL) {
        return n;
    }
    e->unk1E = (mv->facing_angle + 0x400) % 0x1000;
    if ((mv->facing_angle >> 10) & 1) {
        e->unk1C = 0x400;
    }
    e->unk5E = 1;
    if (n >= 2) {
        nas->UserInfo.flags = eng_add_subs(e, n - 1);
    }
    sub = e->sub;
    t = 0;
    while (sub != NULL) {
        pos[0] += dx;
        pos[2] += dz;
        sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        sub->entity = 0x2005;
        sub->unk04 = t;
        sub->flags |= 0x21;
        sub = sub->next;
        t += 0x200;
    }
    Pogo_SpawnPuff(pos, 8, &HeroModelBank[DB_PogoObjects]);
    do {
        do {
            PogoPlay[arg0].tazer = nas;
            PlaySfx(0x520, NULL, (SLONG_XYZ *)&Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        } while (0);
    } while (0);
    if (PogoPlay[arg0].status == 5 && Heroes[arg0].hero_aseq == 0x2F) {
        SetHeroAction(arg0, 0x39);
    }
    if (PogoPlay[arg0].status == 6) {
        PogoFour_UnfreezeMyTiles(arg0);
    }
    PogoPlay[arg0].status = 0;
    return n;
}

void Plot_PogoTazer(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    subengine *sub;
    PogoPlayer *slot;
    HeroInfo *h;
    s32 n;
    s32 i;
    s32 x;
    s32 z;
    s32 hx;
    s32 hz;
    s32 dir;
    s32 hit;
    PogoPlayer *slots;
    HeroInfo *heroes;

    eng = nas->Engine;
    n = 0;
    if (eng == NULL) {
        return;
    }
    slots = PogoPlay;
    heroes = Heroes;
    sub = eng->sub;
next_sub:
    i = 0;
    if (sub != NULL) {
        slot = slots;
        h = heroes;
        sub->entity = ((GameFrameCount + n++) & 7) + 0x2003;
        x = (eng->unk10.x + 0x800) >> 9;
        z = (eng->unk10.z + 0x800) >> 9;
    next_hero:
        if (i < 4) {
            if (slot->tazer == nas) {
                goto skip;
            }
            hit = 0;
            if (h->hero_mode == 0x3B) {
                goto skip;
            }
            he = h->hero_eng;
            hx = (he->unk10.x + 0x800) >> 9;
            hz = (he->unk10.z + 0x800) >> 9;
            dir = eng->unk1E >> 10;
            switch (dir) {
            case 0:
                if (hz == z && !(hx < x)) {
                    hit = 1;
                }
                break;
            case 1:
                if (hx == x && !(z < hz)) {
                    hit = 1;
                }
                break;
            case 2:
                if (hz == z && !(x < hx)) {
                    hit = 1;
                }
                break;
            case 3:
                if (hx == x && !(hz < z)) {
                    hit = 1;
                }
                break;
            }
            if (hit) {
                Player_SetDazed(i);
                if (slot->status == 5 && h->hero_aseq == 0x2F) {
                    SetHeroAction(i, 0x39);
                }
                if (slot->status == 6) {
                    PogoFour_UnfreezeMyTiles(i);
                }
                slot->status = 0;
            }
        skip:
            slot++;
            h++;
            i++;
            goto next_hero;
        }
        sub = sub->next;
        goto next_sub;
    }
}

void Plot_PogoPickup(NAS *nas) {
    EXTengine *eng;
    s32 dir;
    s32 scale;
    s32 d;
    s32 state;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    state = nas->UserInfo.flags;
    if (state >= 9) {
        return;
    }
    if (state < 7) {
        return;
    }
    dir = nas->UserInfo.user1 & 3;
    switch (nas->UserInfo.status) {
    case 0:
    case 1:
        PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        eng->unk2C.x = 0x100;
        eng->unk2C.y = 0x100;
        eng->unk2C.z = 0x100;
        eng->flags |= 0x21;
        eng->sub->unk76 = 0x100;
        nas->UserInfo.status = 2;
        eng->unk1E = dir << 10;
        eng->unk5E = 1;
        break;
    case 2:
        scale = eng->unk2C.x;
        scale += 0x100;
        if (scale >= 0x1000) {
            nas->UserInfo.status = 3;
            scale = 0x1000;
            nas->UserInfo.user2 = PogoObjectTimes[PogoLevelIndex];
            eng->flags &= ~0x21;
        }
        eng->unk2C.x = scale;
        eng->unk2C.y = scale;
        eng->unk2C.z = scale;
        eng->sub->unk76 = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        d = angle_diff_abs(dir << 10, eng->unk1E) >> 3;
        eng->unk5E = 1;
        eng->unk1E += d;
        if (d < 2) {
            nas->UserInfo.user1 = (nas->UserInfo.user1 + 1) & 3;
        }
        break;
    case 6:
        KillNAS(nas);
        number_of_pickups--;
        break;
    }
}

void Init_PogoPowerup(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    EXTmotion *m;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        e->sub->entity = 0x200B;
        e->sub->unk68 = -0x10;
        e->sub->flags |= 0x02000000;
        e->unk10.y -= 0x1000;
        m = AddMotion(nas);
        if (m != NULL) {
            m->Ymax = 0xA0;
            m->Ydec = 0xE;
            m->Yacc = 0;
            m->Ydir = 0xFF;
            m->Yvel = 0;
        } else {
            nas->Failure = 1;
        }
        e2 = AddExtEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = e->unk10.y;
            e2->unk10.z = e->unk10.z;
            if (sub != NULL) {
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x2029;
                sub->unk51 = 2;
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40008041;
            }
        }
        PlaySfx(0x41F, NULL, &e->unk10, 0x1000, 0x1E00, 0);
    }
    nas->UserInfo.flags = 0;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0xF0;
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Plot_PogoPowerup(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;
    subengine *sub;
    EXTmotion *mot;
    s32 scale;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.flags) {
    case 1:
    case 2:
    case 4:
        eng->unk5E = 1;
        eng->unk1E += 0x38;
        break;
    case 5:
        break;
    }
    switch (nas->UserInfo.flags) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    default:
        return;
    }
    e2 = eng->Engine;
    if (e2 != NULL) {
        sub = e2->sub;
        e2->unk10.x = eng->unk10.x;
        e2->unk10.z = eng->unk10.z;
        e2->unk10.y = World_floor_level;
        if (sub != NULL) {
            scale = 0;
            if (abs(eng->unk10.y - World_floor_level) < 0x200) {
                scale = 0x1000 - abs(eng->unk10.y - World_floor_level) * 6;
            }
            sub->unk20.z = scale;
            sub->unk20.y = scale;
            sub->unk20.x = scale;
            sub->unk52 = 1;
        }
    }
    switch (nas->UserInfo.status) {
    case 0:
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        if (GravitateNas(nas) == 0) {
            break;
        }
        if (nas->UserInfo.status == 0) {
            nas->UserInfo.user2 = PogoObjectTimes[PogoLevelIndex];
            nas->UserInfo.status = 3;
            PlaySfx(0x420, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        mot = FindExtension(nas, 0x800);
        if (mot != NULL) {
            mot->Ymax = 0x200;
            mot->Ydec = 8;
            mot->Ydir = 0xFF;
            mot->Yacc = 0;
            mot->Yvel = 0x40;
        }
        break;
    case 6:
        KillNAS(nas);
        number_of_powerups--;
        break;
    }
}

void Plot_PogoBanker(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    PogoCellPos *pos;
    PogoCell *c;
    s32 scale;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        if (FindFreeSquare(&x, &z) != 0) {
            PogoCell *cb = PogoGrid[0];
            c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)cb);
            pos = c->unk4;
            c->hero = (HeroInfo *)nas;
            if (pos != NULL) {
                eng->unk10.x = pos->x;
                eng->unk10.y = World_floor_level;
                eng->unk10.z = pos->z;
                eng->unk2C.x = 0;
                eng->unk2C.y = 0;
                eng->unk2C.z = 0;
                eng->flags |= 0x8021;
                eng->unk5E = 1;
                nas->UserInfo.status = 2;
                nas->UserInfo.user2 = PogoObjectTimes[PogoLevelIndex];
                PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        scale = eng->unk2C.x + 0x100;
        if (scale >= 0x1000) {
            scale = 0x1000;
            eng->flags &= ~0x21;
            nas->UserInfo.status = 3;
        }
        eng->unk2C.z = scale;
        eng->unk2C.y = scale;
        eng->unk2C.x = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        break;
    case 4:
        eng->flags &= ~0x8000;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = Random(0x28) + 0x28;
        break;
    }
}

void Plot_PogoMushroom(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    PogoCellPos *pos;
    PogoCell *c;
    s32 cx;
    s32 cz;
    s32 scale;
    s32 i;
    s32 j;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 > 0) {
            break;
        }
        if (GameExitFlags & 0x801) {
            break;
        }
        if (FindFreeSquareForMushroom(&x, &z) == 0) {
            break;
        }
        pos = PogoGrid[x][z].unk4;
        if (pos == NULL) {
            break;
        }
        eng->unk10.x = pos->x;
        eng->unk10.y = World_floor_level;
        eng->unk10.z = pos->z;
        Pogo_SpawnPuff((s32 *)&eng->unk10, 0, &HeroModelBank[DB_ArenaPogoPainterCrystalarena]);
        nas->UserInfo.status = 6;
        nas->UserInfo.user1 = 0x12;
        break;
    case 6:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        cx = (eng->unk10.x + 0x800) >> 9;
        cz = (eng->unk10.z + 0x800) >> 9;
        nas->UserInfo.status = 0;
        if (SquareIsFreeForMushroom(cx, cz) != 0) {
            PogoCell *cb = PogoGrid[0];
            c = (PogoCell *)(cz * sizeof(PogoCell) + cx * sizeof(PogoGrid[0]) + (u32)cb);
            c->hero = (HeroInfo *)nas;
            if (c->unk4 != NULL) {
                eng->unk2C.x = 0x1000;
                eng->unk2C.z = 0x1000;
                eng->unk2C.y = 0;
                eng->unk5E = 1;
                eng->flags |= 0x8000;
                nas->UserInfo.status = 2;
                nas->UserInfo.user1 = 0;
                PlaySfx(0x528, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        scale = MushroomScales[nas->UserInfo.user1];
        if (++nas->UserInfo.user1 >= 0x10) {
            scale = 0x1000;
            nas->UserInfo.user1 = Random(0x28) + 0x28;
            nas->UserInfo.status = 3;
        }
        eng->unk2C.y = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 5;
        }
        break;
    case 5:
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (PogoGrid[i][j].hero == (HeroInfo *)nas) {
                    PogoGrid[i][j].hero = NULL;
                }
            }
        }
        Pogo_SpawnPuff((s32 *)&eng->unk10, 1, &HeroModelBank[DB_PogoObjects]);
        eng->flags &= ~0x8000;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = Random(0x14) + 0x1E;
        break;
    }
}

void Tiles_PlayerIsStealing(s32 arg0, s32 arg1) {
    s32 i;
    s32 j;
    PogoCell *c;
    s16 old;
    s16 k;

    for (i = 0;; i++) {
        if (i >= 8) goto done;
        for (j = 0, k = 10;; j++) {
            if (j >= 8) goto next;
            c = (PogoCell *)(i * sizeof(PogoGrid[0]) + j * sizeof(PogoCell) + (u32)PogoGrid);
            if (c->unkA == arg1) {
                old = c->unkA;
                c->unk16 = old;
                c->unkA = arg0;
                c->unk14 = k;
                c->unk12 = 0;
                if (GameMode == 0xB) {
                    if (i > 0) {
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
                    }
                }
            }
        }
    next:;
    }
done:;
}

void Tiles_FloodFillIslandMap(s32 *seen, s32 id) {
    s32 i;

    if (GameMode == 0xB) {
        for (i = 0; i < 8; i++) {
            if (TileScan0[i] != id) {
                Island_FloodFill(0, i, id, seen);
            } else {
                if (i > 0 && TileScan0[i - 1] != id) {
                    Island_FloodFill(0, i, id, seen);
                }
                if (i < 7 && TileScan0[i + 1] != id) {
                    Island_FloodFill(0, i, id, seen);
                }
            }
            if (TileScan1[i] != id) {
                Island_FloodFill(7, i, id, seen);
            } else {
                if (i > 0 && TileScan1[i - 1] != id) {
                    Island_FloodFill(7, i, id, seen);
                }
                if (i < 7 && TileScan1[i + 1] != id) {
                    Island_FloodFill(7, i, id, seen);
                }
            }
            if (TileScan2[i] != id) {
                Island_FloodFill(i, 7, id, seen);
            } else {
                if (i > 0 && TileScan2[i - 1] != id) {
                    Island_FloodFill(i, 7, id, seen);
                }
                if (i < 7 && TileScan2[i + 1] != id) {
                    Island_FloodFill(i, 7, id, seen);
                }
            }
            if (TileScan3[i] != id) {
                Island_FloodFill(i, 0, id, seen);
            } else {
                if (i > 0 && TileScan3[i - 1] != id) {
                    Island_FloodFill(i, 0, id, seen);
                }
                if (i < 7 && TileScan3[i + 1] != id) {
                    Island_FloodFill(i, 0, id, seen);
                }
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            Island_FloodFill(0, i, id, seen);
            Island_FloodFill(7, i, id, seen);
            Island_FloodFill(i, 0, id, seen);
            Island_FloodFill(i, 7, id, seen);
        }
    }
}

s32 Tiles_CountUnflagged(s32 *arg0, const s32 arg1, s32 arg2) {
    s32 i;
    s32 j;
    s32 n;
    PogoCell *c;
    u16 t;
    s32 k;
    s32 r;

    n = 0;
    for (j = 0;; j++) {
        if (j >= 8) goto done;
        for (i = 0;; i++) {
            r = j * 8;
            k = 10;
            if (i >= 8) goto next;
            if (arg0[r + i] != 0) continue;
            c = (PogoCell *)(i * sizeof(PogoGrid[0]) + j * sizeof(PogoCell) + (u32)PogoGrid);
            if (c->unkE != 0) continue;
            t = c->unk12;
            if (t == 6) continue;
            if (t == 5) continue;
            if (t == 7) continue;
            if (arg2 != 0) {
                c->unk14 = k;
                c->unk12 = 0;
                c->unk16 = c->unkA;
                c->unkA = arg1;
                if (GameMode == 0xB) {
                    if (i > 0) {
                        Tile_Update(i - 1, j);
                    }
                    if (i < 7) {
                        Tile_Update(i + 1, j);
                    }
                    if (j > 0) {
                        Tile_Update(i, j - 1);
                    }
                    if (j < 7) {
                        Tile_Update(i, j + 1);
                    }
                }
            }
            n++;
        }
    next:;
    }
done:
    return n;
}

s32 LevelSpecific_ClaimedTile(s32 x, s32 z) {
    D5A614View *g = (D5A614View *)GameInfo;
    s32 id;
    s32 mask;
    s32 n;
    s32 r;

    if (g->mode == 11) {
        id = PogoGrid[x][z].unkA;
        mask = 0;
        if (id != -1) {
            if (x > 0) {
                n = PogoGrid[x - 1][z].unkA;
                mask = n == id;
            } else if (TileScan0[z] == id) {
                mask = 1;
            }
            if (x < 7) {
                if (PogoGrid[x + 1][z].unkA == id) {
                    mask |= 4;
                }
            } else if (TileScan1[z] == id) {
                mask |= 4;
            }
            if (z > 0) {
                if (PogoGrid[x][z - 1].unkA == id) {
                    mask |= 2;
                }
            } else if (TileScan3[x] == id) {
                mask |= 2;
            }
            if (z < 7) {
                if (PogoGrid[x][z + 1].unkA == id) {
                    mask |= 8;
                }
            } else if (TileScan2[x] == id) {
                mask |= 8;
            }
        }
        if (GameLevelFlags & 0x80) {
            return mask + 0x301B;
        }
        return mask + 0x3052;
    }
    switch (g->mode) {
    case 12:
        r = 0x304E;
        if (g->flags & 0x80) {
            r = 0x3058;
        }
        return r;
    case 13:
        r = 0x3026;
        if (g->flags & 0x80) {
            r = 0x3044;
        }
        return r;
    default:
        r = 0x303C;
        if (GameLevelFlags & 0x80) {
            r = 0x3033;
        }
        return r;
    }
}

bool FindFreeSquareForMushroom(s32 *arg0, s32 *arg1) {
    s32 ok;

    if (!Pogo_FindFreeSquare(arg0, arg1)) {
        return 0;
    }
    ok = 1;
    if (*arg0 > 0) {
        ok = SquareContainsMushroom(*arg0 - 1, *arg1) == 0;
    }
    if (*arg0 < 7 && SquareContainsMushroom(*arg0 + 1, *arg1)) {
        ok = 0;
    }
    if (*arg1 > 0 && SquareContainsMushroom(*arg0, *arg1 - 1)) {
        ok = 0;
    }
    if (*arg1 < 7 && SquareContainsMushroom(*arg0, *arg1 + 1)) {
        ok = 0;
    }
    return ok;
}

bool SquareIsFreeForMushroom(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    s32 s;

    if (p->unkE != 0 || p->unk10 != 0 || p->hero != NULL) {
        return 0;
    }
    s = (u16)p->unk12;
    if (s == 5) {
        return 0;
    }
    if (s == 6) {
        return 0;
    }
    return PogoPadlock_IsHere(arg0, arg1) == 0;
}

void Player_RampToTile(u32 arg0) {
    s32 x = Heroes[arg0].hero_eng->unk10.x;
    s32 tx = (((x + 0x800) >> 9) << 9) - 0x700;
    s32 tz = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;

    Heroes[arg0].hero_eng->unk10.x = x - (x - tx) / 4;
    Heroes[arg0].hero_eng->unk10.z -= (Heroes[arg0].hero_eng->unk10.z - tz) / 4;
}

void Fire_Rocket(s32 arg0, s32 arg1) {
    EXTheromove *move;
    EXTengine *eng;
    s32 buttons;
    s32 angle;
    s32 mode;

    PlayThrowNoise(arg0);
    move = Heroes[arg0].hero_move;
    if (move != NULL) {
        buttons = ((PogoPad *)Pads)[arg0].unk50;
        angle = move->facing_angle;
        if (buttons & 0xF0) {
            if (buttons & 0x80) {
                angle = 0x400;
            }
            if (buttons & 0x40) {
                angle = 0;
            }
            if (buttons & 0x20) {
                angle = 0xC00;
            }
            if (buttons & 0x10) {
                angle = 0x800;
            }
        }
        mode = 1;
        if (arg1 != 0) {
            mode = 3;
        }
        Fire_RocketInDir(arg0, mode, angle, 0x180);
        eng = Heroes[arg0].hero_eng;
        PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
        if (PogoPlay[arg0].status == 5 && Heroes[arg0].hero_aseq == 0x2F) {
            SetHeroAction(arg0, 0x39);
        }
        if (PogoPlay[arg0].status == 6) {
            PogoFour_UnfreezeMyTiles(arg0);
        }
        PogoPlay[arg0].status = 0;
    }
}

void Fire_RocketFourWays(s32 arg0, s32 arg1) {
    HeroInfo *h;
    EXTengine *eng;
    s32 x;
    s32 z;
    PogoPlayer *base;
    PogoPlayer *slot;
    HeroInfo *heroes;

    heroes = Heroes;
    h = &heroes[arg0];
    eng = h->hero_eng;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    PlayThrowNoise(arg0);
    if (x > 0) {
        Fire_RocketInDir(arg0, 2, 0x400, 0x180);
    }
    if (x < 7) {
        Fire_RocketInDir(arg0, 2, 0xC00, 0x180);
    }
    if (z > 0) {
        Fire_RocketInDir(arg0, 2, 0, 0x180);
    }
    if (z < 7) {
        Fire_RocketInDir(arg0, 2, 0x800, 0x180);
    }
    PlaySfx(0x43C, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
    if (PogoPlay[arg0].status == 5 && h->hero_aseq == 0x2F) {
        SetHeroAction(arg0, 0x39);
    }
    base = PogoPlay;
    slot = &base[arg0];
    if (slot->status == 6) {
        PogoFour_UnfreezeMyTiles(arg0);
    }
    slot->status = 0;
}

void PlayThrowNoise(u32 arg0) {
    switch (Heroes[arg0].hero_name) {
    case 1:
        PlaySfx(0x10, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x74, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x42, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x13C, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 6:
        PlaySfx(0x16E, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 7:
        PlaySfx(0x10A, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 4:
        PlaySfx(0xA6, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 8:
        PlaySfx(0xD8, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void Plot_PogoProjectile(NAS *arg0) {
    EXTengine *e;
    subengine *first;
    subengine *sub;
    HeroInfo *h;
    PogoPlayer *base;
    PogoPlayer *slot;
    EXTengine *he;
    NAS *cell;
    NAS *nas;
    s32 *ext;
    u8 *p;
    s32 *pos;
    s32 i;
    s32 j;
    s32 n;
    s32 done;
    s32 k;
    s32 off;
    s32 hit;
    s32 lim;
    s32 px;
    s32 pz;
    s32 me;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;
    u16 f;

    e = arg0->Engine;
    if (e == NULL || (first = e->sub) == NULL) {
        return;
    }
    if (arg0->UserInfo.flags & 2) {
        e->unk10.x -= (rcossin_tbl[(arg0->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 8;
        e->unk10.z -= (rcossin_tbl[arg0->UserInfo.status & 0xFFF].unk2 * 9) >> 8;
    }
    j = 0;
    n = 0;
    done = 0;
    sub = first->next;
    for (;;) {
        if (sub == NULL || done) {
            break;
        }
        if (sub->flags & 0x8000) {
            if (((s32)arg0->UserInfo.user1 >> j) & 1) {
                k = sub->unk20.x;
                if (arg0->UserInfo.flags & 2) {
                    sub->unk04 += 0x90;
                }
                k -= 0x100;
                if (k <= 0) {
                    k = 0;
                    sub->flags &= ~0x8000;
                }
                n++;
                sub->unk20.z = k;
                sub->unk20.y = k;
                sub->unk20.x = k;
                sub->unk52 = 1;
            }
            if (sub->flags & 0x8000) {
                goto next;
            }
        }
        if (!(((s32)arg0->UserInfo.user1 >> j) & 1)) {
            done = 1;
        } else {
        next:
            sub = sub->next;
            j++;
        }
    }
    if (arg0->UserInfo.flags & 2) {
        k = (arg0->UserInfo.flags >> 4) & 3;
        hit = 0;
        if (GameFrameCount & 1) {
            k++;
        }
        if (k & 2) {
            if (sub != NULL) {
                sub->entity = 0x2018;
                sub->flags |= 0x8000;
                arg0->UserInfo.user1 |= 1 << j;
            }
            k = 0;
        }
        i = 0;
        base = PogoPlay;
        h = Heroes;
        slot = base;
        off = i;
        f = (arg0->UserInfo.flags & 0xFFCF) | (k << 4);
        me = (f >> 8) & 7;
        arg0->UserInfo.flags = f;
    heroes:
        if (i < 4) {
            if (i != me) {
                he = h->hero_eng;
                if (he != NULL) {
                    dx = he->unk10.x - e->unk10.x;
                    dz = he->unk10.z - e->unk10.z;
                    if ((csqrt(dx * dx + dz * dz) >> 6) < 0xA0) {
                        hit = 1;
                        if (!(h->hero_status & 0x800)) {
                            if (arg0->UserInfo.user2 == 3) {
                                do {
                                    Tiles_PlayerIsStealing(me, i);
                                    PlaySfx(0x552, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                                } while (0);
                            }
                            do {
                                do {
                                    if (((PogoPlayer *)(off + (u32)base))->status == 5 && h->hero_aseq == 0x2F) {
                                        SetHeroAction(i, 0x39);
                                    }
                                } while (0);
                            } while (0);
                            do {
                                if (slot->status == 6) {
                                    PogoFour_UnfreezeMyTiles(i);
                                }
                            } while (0);
                            slot->status = 0;
                            h->hero_status |= 0x800;
                        }
                    }
                }
            }
            h++;
            do {
                slot++;
            } while (0);
            off += sizeof(PogoPlayer);
            i++;
            goto heroes;
        }
        if (GameMode == 0xA) {
            x = (e->unk10.x + 0x800) >> 9;
            z = (e->unk10.z + 0x800) >> 9;
            if ((u32)x < 8 && z >= 0 && z < 8) {
                cell = (NAS *)PogoGrid[x][z].hero;
                if (cell != NULL && cell->UserInfo.flags == 0xB) {
                    cell->UserInfo.status = 5;
                    hit = 1;
                }
            }
        }
        if (!hit) {
            px = e->unk10.x;
            pz = e->unk10.z;
            lim = 0x800;
            if (px < -0xA00 || px > lim || pz < -0xA00 || pz > lim) {
                hit = 1;
            }
            if (!hit) {
                return;
            }
        }
        pos = (s32 *)&e->unk10;
        p = &HeroModelBank[DB_PogoObjects];
        nas = CreateNAS(e->unk10.x, pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext[3] = 0;
                ext[6] = 0x100;
                if (p != NULL) {
                    ext[8] = 5;
                    ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
                } else {
                    ext[8] = 1;
                }
                ext[10] = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x521, NULL, &e->unk10, 0x1000, 0x1E00, 0);
        arg0->UserInfo.flags &= 0xFFFC;
        first->flags &= ~0x8000;
    } else if (arg0->UserInfo.user1 != 0 && n == 0) {
        KillNAS(arg0);
    }
}

void PogoKerb_Poll(void) {
    s32 buf[0x40];
    u8 *row;
    s32 *pos;
    s32 i;
    s32 j;
    HeroInfo *h;
    PogoModeView *g;

    if (PogoKerbObj == NULL) {
        return;
    }
    if (KerbDelay != 0) {
        KerbDelay--;
    }
    if (KerbCount == 0) {
        switch ((u32)Clock_Time >> 8) {
        case 0x16:
        case 0x17:
        case 0x31:
        case 0x32:
        case 0x46:
        case 0x47:
        case 0x101:
        case 0x102:
        case 0x116:
        case 0x117:
            if (((u32)Clock_Time >> 8) != KerbClockSecond) {
                PlaySfx(0x532, NULL, NULL, 0x1000, 0x1E00, 0);
                KerbClockSecond = (u32)Clock_Time >> 8;
            }
            break;
        case 0x15:
        case 0x30:
        case 0x45:
        case 0x100:
        case 0x115:
            KerbCount = Random(4) + 1;
            PlaySfx(0x534, NULL, NULL, 0x1000, 0x1E00, 0);
            PogoKerb_SetMode(KerbCount);
            break;
        }
        return;
    }
    if (KerbDelay != 0) {
        return;
    }
    if (KerbMode != 0) {
        if (--KerbStep < 0) {
            KerbStep = 0x1F;
        }
    } else {
        if (++KerbStep >= 0x20) {
            KerbStep = 0;
        }
    }
    if (GameLevelFlags & 0x80) {
        PogoKerbObj->index = KerbIndexB[KerbStep];
    } else {
        PogoKerbObj->index = KerbIndexA[KerbStep];
    }
    j = 0;
    row = KerbData[KerbStep][0];
    for (; j < 8; j++) {
        TileScan0[j] = row[j];
        TileScan2[j] = row[j + 8];
        TileScan1[7 - j] = row[j + 0x10];
        TileScan3[7 - j] = row[j + 0x18];
    }
    if (KerbMode != 0) {
        pos = KerbPathB[KerbStep];
    } else {
        pos = KerbPathA[KerbStep];
    }
    Tile_Update(KerbTiles[pos[0]][0], KerbTiles[pos[0]][1]);
    Tile_Update(KerbTiles[pos[1]][0], KerbTiles[pos[1]][1]);
    Tile_Update(KerbTiles[pos[2]][0], KerbTiles[pos[2]][1]);
    Tile_Update(KerbTiles[pos[3]][0], KerbTiles[pos[3]][1]);
    i = 0;
    g = (PogoModeView *)GameInfo;
    h = Heroes;
loop:
    if (i < 4) {
        if (g->mode != 10 && g->mode != 13) {
            Tiles_MakeUpIslandMap(buf, i);
            Tiles_FloodFillIslandMap(buf, i);
            if (Tiles_CountUnflagged(buf, i, 1)) {
                PlaySfx(0x51D, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                if (g->mode == 11) {
                    Bank_Tiles(i);
                }
            }
        }
        h++;
        i++;
        goto loop;
    }
    if (--KerbTimer <= 0) {
        if (!(GameLevelFlags & 0x80)) {
            KerbDelay = Random(0xC8) + 0xC8;
        }
        KerbCount = 0;
    } else {
        KerbDelay = KerbPeriod;
    }
    PlaySfx(0x533, NULL, NULL, 0x1000, 0x1E00, 0);
}

void PogoKerb_SetMode(s32 arg0) {
    KerbCount = arg0;
    switch (arg0) {
    case 1:
        KerbMode = 0;
        KerbPeriod = 8;
        KerbDelay = 8;
        KerbTimer = 8;
        break;
    case 2:
        KerbMode = 0;
        KerbPeriod = 8;
        KerbDelay = 8;
        KerbTimer = 0x10;
        break;
    case 3:
        KerbMode = 1;
        KerbPeriod = 8;
        KerbDelay = 8;
        KerbTimer = 8;
        break;
    case 4:
        KerbMode = 1;
        KerbPeriod = 8;
        KerbDelay = 8;
        KerbTimer = 0x10;
        break;
    case 6:
        KerbPeriod = 0x18;
        KerbDelay = 0x18;
        KerbMode = GameFrameCount & 1;
        KerbTimer = (Random(2) + 2) * 8;
        break;
    }
}

void PogoAI_Poll(u32 arg0) {
    EXTbotai *ai;
    s32 r;

    D5A614View *g = (D5A614View *)GameInfo;

    if (!(*(s32 *)&g->unkC[4] & 0x04000000)) {
        ai = Heroes[arg0].hero_ai;
        if (ai != NULL) {
            ai->AIpad = 0;
            if (ai->AItimer1 != 0) {
                ai->AItimer1--;
            }
            if (PogoAIPlay[arg0].unk4 != 0 &&
                (u16)(g->mode - 0xB) < 2U) {
                if (ai->AItimer2 != 0) {
                    ai->AItimer2--;
                }
            }
            switch (ai->AImode) {
            case 1:
                r = PogoAI_PrimeDirective(arg0) != 1;
                if (r) {
                    ai->TargetXYZ.x = Random(8);
                    ai->TargetXYZ.z = Random(8);
                    ai->AImode = 3;
                }
                break;
            case 3:
                r = PogoAI_PrimeDirective(arg0) != 1;
                if (r) {
                    PogoAI_MoveTowardsTarget(arg0);
                }
                break;
            case 4:
                r = PogoAI_PrimeDirective(arg0) != 1;
                if (r) {
                    ai->AImode = 1;
                    break;
                }
            case 5:
            case 6:
            case 7:
                PogoAI_MoveTowardsTarget(arg0);
                break;
            }
            if (ai->AItimer1 == 0 || (GameLevelFlags & 0x20)) {
                switch (PogoPlay[arg0].status) {
                case 1:
                case 3:
                case 4:
                    r = PogoAI_CanPlayerShootAnyone(arg0);
                    goto check;
                case 2:
                    r = PogoAI_CanPlayerShootAnyoneFourWays(arg0);
                check:
                    if (r != 0) {
                        ai->AIpad |= 0x8000;
                    }
                    break;
                }
            }
            set_joypad((u8 *)&Pads[arg0], ai->AIpad);
        }
        PogoAIPlay[arg0].unk4 = 0;
    }
}

bool PogoAI_CanPlayerShootAnyone(u32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z, ox, oz;
    s32 dir;
    s32 hit;

    if (GameTeams[0] != 0) {
        mate = Heroes[hero].hero_teammate;
    } else {
        mate = 1000;
    }
    i = 0;
    eng = Heroes[hero].hero_eng;
    g = GameInfo;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    dir = Heroes[hero].hero_move->facing_angle >> 10;
    p = Heroes;
loop:
    {
        hit = 0;
        if (i == hero || i == mate) {
            goto next;
        }
        ox = (p->hero_eng->unk10.x + 0x800) >> 9;
        oz = (p->hero_eng->unk10.z + 0x800) >> 9;
        switch (dir) {
        case 0:
            if (oz < z && x == ox) {
                hit = 1;
            }
            break;
        case 1:
            if (ox < x && z == oz) {
                hit = 1;
            }
            break;
        case 2:
            if (z < oz && x == ox) {
                hit = 1;
            }
            break;
        case 3:
            if (x < ox && z == oz) {
                hit = 1;
            }
            break;
        }
        if (hit) {
            if ((*(u16 *)(g + 0x32) & 0x20) && g[i + 0x5C] != 0) {
                return 0;
            }
            return 1;
        }
    }
next:
    i++;
    p++;
    if (i < 4) {
        goto loop;
    }
    return 0;
}

bool PogoAI_CanPlayerShootAnyoneFourWays(u32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z, ox, oz;

    if (GameTeams[0] != 0) {
        mate = Heroes[hero].hero_teammate;
        i = 0;
    } else {
        mate = 1000;
        i = 0;
    }
    eng = Heroes[hero].hero_eng;
    g = GameInfo;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    p = Heroes;
loop:
    if (i == hero) {
        goto next;
    }
    do {
        if (i == mate) {
            goto next;
        }
    } while (0);
    ox = (p->hero_eng->unk10.x + 0x800) >> 9;
    oz = (p->hero_eng->unk10.z + 0x800) >> 9;
    if (x != ox) {
        do {
            if (z != oz) {
                goto next;
            }
        } while (0);
    }
    if (*(u16 *)(g + 0x32) & 0x20) {
        do {
            if (g[i + 0x5C] != 0) {
                return 0;
            }
        } while (0);
    }
    return 1;
next:
    i++;
    p++;
    if (i < 4) {
        goto loop;
    }
    return 0;
}

bool PogoAI_CanPlayerShockAnyone(u32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z;

    if (GameTeams[0] != 0) {
        mate = Heroes[hero].hero_teammate;
        i = 0;
    } else {
        mate = 1000;
        i = 0;
    }
    eng = Heroes[hero].hero_eng;
    g = GameInfo;
    p = Heroes;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    for (;;) {
        if (i >= 4) {
            goto end;
        }
        if (i != hero && i != mate) {
            if ((u32)(csqrt(PogoSquaredDistanceToDestination(x, z, p->hero_eng->unk10.x, p->hero_eng->unk10.z)) >> 6) < 0x381) {
                if ((*(u16 *)(g + 0x32) & 0x20) && g[i + 0x5C] != 0) {
                    return 0;
                }
                return 1;
            }
        }
        p++;
        i++;
    }
end:
    return 0;
}

void PogoAI_MoveTowardsTarget(u32 hero) {
    s32 dir;
    EXTbotai *ai;
    PogoMove *m;
    PogoMove *mb;
    PogoOffset *tbl;
    s32 tx, tz;
    s32 x;
    s32 z;
    s32 i;
    s32 ax, az, bz;
    s32 xm;
    s32 xp;
    s32 gx, gz;
    s32 dx, dz;
    u32 best;
    s32 flags;
    u32 n;
    s32 k;
    s32 r;

    ai = Heroes[hero].hero_ai;
    if (ai == NULL) {
        return;
    }
    dir = -1;
    do {
        ai->AIpad = 0;
        tx = ai->TargetXYZ.x;
        tz = ai->TargetXYZ.z;
        x = (Heroes[hero].hero_eng->unk10.x + 0x800) >> 9;
        z = (Heroes[hero].hero_eng->unk10.z + 0x800) >> 9;
    } while (0);
    if (x == tx && z == tz) {
        if (ai->AImode == 3 || ai->AImode == 7) {
            ai->AImode = 1;
            ai->AItimer2 = Random(3) + 3;
            return;
        }
        if (ai->AImode == 6) {
            ai->AImode = 1;
            ai->AItimer2 = Random(3) + 3;
            return;
        }
        if (ai->AImode == 5) {
            mb = PogoAIPlay;
            m = &mb[hero];
            n = ++m->unkC;
            if (n < (u32)m->unk10) {
                k = (n + m->unk8) & 7;
                if (m->unk0 != 0) {
                    tbl = EncloseStepsOdd;
                } else {
                    tbl = EncloseStepsEven;
                }
                ai->TargetXYZ.x = x + tbl[k].x;
                r = z + tbl[k].z;
                do {
                } while (0);
                ai->TargetXYZ.z = r;
                r = PogoAI_IsSquareOKForPlayer(hero, ai->TargetXYZ.x, ai->TargetXYZ.z, 0, 0, 0) != 1;
                if (r) {
                    ai->AImode = 1;
                    ai->AItimer2 = Random(3) + 3;
                    return;
                }
            } else {
                ai->AImode = 1;
                ai->AItimer2 = Random(3) + 3;
                return;
            }
        }
    }
    flags = 2;
    i = 0;
    ax = (x << 9) - 0x800;
    az = ((z + 1) << 9) - 0x800;
    do {
        best = -1;
        gx = (tx << 9) - 0x800;
        gz = (tz << 9) - 0x800;
        bz = (z << 9) - 0x800;
        if (PogoAI_IsSquareOKForPlayer(hero, x, z, 0, 1, flags)) {
            dx = abs(ax - gx);
            dz = abs(az - gz);
            dx = dx * dx + dz * dz;
            if ((u32)dx < best) {
                best = dx;
                dir = 2;
            }
        }
        if (PogoAI_IsSquareOKForPlayer(hero, x, z, 0, -1, flags)) {
            dx = (z - 1) << 9;
            dx -= 0x800;
            dz = abs(ax - gx);
            dx = abs(dx - gz);
            dx = dz * dz + dx * dx;
            if ((u32)dx < best) {
                best = dx;
                dir = 0;
            }
        }
        if (PogoAI_IsSquareOKForPlayer(hero, x, z, 1, 0, flags)) {
            xp = ((x + 1) << 9) - 0x800;
            dx = abs(xp - gx);
            dz = abs(bz - gz);
            dx = dx * dx + dz * dz;
            if ((u32)dx < best) {
                best = dx;
                dir = 3;
            }
        }
        if (PogoAI_IsSquareOKForPlayer(hero, x, z, -1, 0, flags)) {
            xm = ((x - 1) << 9) - 0x800;
            dx = abs(xm - gx);
            dz = abs(bz - gz);
            dx = dx * dx + dz * dz;
            if ((u32)dx < best) {
                best = dx;
                dir = 1;
            }
        }
        flags = 0;
        i++;
    } while (dir == -1 && i < 2);
    switch (dir) {
    case 2:
        ai->AIpad |= DirPadBits[0];
        break;
    case 0:
        ai->AIpad |= DirPadBits[4];
        break;
    case 3:
        ai->AIpad |= DirPadBits[2];
        break;
    case 1:
        ai->AIpad |= DirPadBits[6];
        break;
    }
    if (PogoAIPlay[hero].unk4 != 0 && dir != Heroes[hero].hero_move->facing_angle >> 10) {
        ai->AItimer2 = Random(2) + 2;
    }
}

bool PogoAI_IsSquareOKForPlayer(u32 arg0, s32 x, s32 z, s32 dx, s32 dz, u32 flags) {
    PogoCell *p;
    HeroInfo *h;
    u8 *g;
    u16 t;
    u16 a;

    x += dx;
    z += dz;
    if ((u32)x >= 8 || (u32)z >= 8) {
        return 0;
    }
    p = &PogoGrid[x][z];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    if (p->unk10 != 0 && p->unk10 != arg0 + 1) {
        return 0;
    }
    g = GameInfo;
    if (g[arg0 + 0x5C] == 0 && !(flags & 1)) {
        goto one;
    }
    t = p->unk12;
    if (t == 5) {
        return 0;
    }
    if (t == 6) {
        return 0;
    }
    h = p->hero;
    if (h != NULL) {
        a = h->hero_pu_action;
        if (a == 0xB) {
            return 0;
        }
        if (*(s16 *)(g + 0x34) != 0xD) {
            goto one;
        }
        if (a == 0xA) {
            return 1;
        }
    }
    if (GameMode == 0xD && (flags & 2) && p->unkA == arg0 && PogoPlay[arg0].status != 6) {
        return 0;
    }
one:
    return 1;
}

bool PogoAI_PrimeDirective(u32 arg0) {
    s32 x;
    s32 z;
    EXTbotai *ai;

    if ((u16)GameMode - 0xB < 2U) {
        ai = Heroes[arg0].hero_ai;
        if (ai != NULL && ai->AItimer2 == 0) {
            ai->AItimer2 = Random(3) + 3;
            if (PogoAI_GoForMyOwn(arg0, ai) != 0) {
                return 1;
            }
            if (PogoAI_StartEncircling(arg0, ai) != 0) {
                return 1;
            }
            if (PogoAI_StartEnclosing(arg0, ai) != 0) {
                return 1;
            }
        }
    }
    x = -1;
    z = -1;
    PogoAI_ScanForClosestPickup(arg0, &x, &z);
    if (x != -1) {
        EXTbotai *ai2 = Heroes[arg0].hero_ai;
        if (ai2 != NULL) {
            ai2->TargetXYZ.x = x;
            ai2->AImode = 4;
            ai2->TargetXYZ.z = z;
            return 1;
        }
    }
    return 0;
}

void PogoAI_ScanForClosestPickup(u32 arg0, s32 *arg1, s32 *arg2) {
    s32 hx;
    s32 hz;
    u32 best;
    EXTengine *eng;
    PogoCell *c;
    s32 i;
    s32 j;
    s32 ok;
    s32 dx;
    s32 dz;
    s32 cx;
    s32 cz;
    s32 px;
    s32 pz;
    u32 d;

    best = -1;
    eng = Heroes[arg0].hero_eng;
    hx = (eng->unk10.x + 0x800) >> 9;
    hz = (eng->unk10.z + 0x800) >> 9;
    i = 0;
    while (1) {
        if (i >= 8) {
            break;
        }
        j = 0;
        while (1) {
            if (j >= 8) {
                break;
            }
            if (Pickup_IsClaimableByAI(arg0, i, j) != 0) {
                HeroInfo *h;
                c = (PogoCell *)(j * sizeof(PogoCell) + i * sizeof(PogoGrid[0]) + (u32)PogoGrid);
                h = c->hero;
                ok = 0;
                if (h != NULL) {
                    do {
                        ok = PogoPlay[arg0].status < h->hero_pu_action;
                    } while (0);
                } else if ((u16)c->unk12 != 7 || PogoPlay[arg0].status < 9) {
                    ok = 1;
                }
                if (ok) {
                    cx = (i << 9) - 0x800;
                    px = (hx << 9) - 0x800;
                    cz = (j << 9) - 0x800;
                    pz = (hz << 9) - 0x800;
                    dx = abs(cx - px);
                    dz = abs(cz - pz);
                    d = dx * dx + dz * dz;
                    if (d < best) {
                        best = d;
                        *arg1 = i;
                        *arg2 = j;
                    }
                }
            }
            j++;
        }
        i++;
    }
}

bool PogoAI_RadialSearch(u32 i, s32 x, s32 z, s32 angle, s32 *outX, s32 *outZ) {
    s32 dx;
    s32 dz;
    s32 side0, side1, side2, side3;
    s32 off0, off1, off2, off3, off4, off5;
    s32 f0, f1, f2;
    s32 miss;
    s32 px, pz;

    dz = dx = 0;
    side0 = 0;
    side1 = 0;
    side2 = 0;
    side3 = 0;
    switch (angle) {
    case 2:
        dz = 1;
        do {} while (0);
        dz = 1;
        side0 = -1;
        side1 = 0;
        side2 = dz;
        break;
    case 0:
        dz = -1;
        do {} while (0);
        dz = -1;
        side0 = 1;
        side1 = 0;
        side2 = dz;
        break;
    case 1:
        dx = -1;
        do {} while (0);
        dx = -1;
        side0 = 0;
        side1 = dx;
        side2 = 0;
        side3 = 1;
        break;
    case 3:
        dx = 1;
        do {} while (0);
        dx = 1;
        side0 = 0;
        side1 = dx;
        side2 = 0;
        side3 = -1;
        break;
    }
    f0 = 0;
    f1 = 0;
    f2 = 0;
    off0 = side3;
    off1 = side2;
    off2 = side1;
    off3 = side0;
    off4 = dz;
    off5 = dx;
    for (;;) {
        miss = 0;
        px = x + off5;
        pz = z + off4;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f0 && PogoGrid[px][pz].unkA != i) {
                    f0 = 1;
                }
                if (f0 && PogoGrid[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((PogoModeView *)GameInfo)->mode == 11 && f0 &&
                       PogoAI_RadialSearch_CheckKerbs(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        px = x + off3;
        pz = z + off2;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f1 && PogoGrid[px][pz].unkA != i) {
                    f1 = 1;
                }
                if (f1 && PogoGrid[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((PogoModeView *)GameInfo)->mode == 11 && f1 &&
                       PogoAI_RadialSearch_CheckKerbs(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        px = x + off1;
        pz = z + off0;
        if (px >= -1 && px < 9 && pz >= -1 && pz < 9) {
            if (px >= 0 && px < 8 && pz >= 0 && pz < 8) {
                if (!f2 && PogoGrid[px][pz].unkA != i) {
                    f2 = 1;
                }
                if (f2 && PogoGrid[px][pz].unkA == i) {
                    goto found;
                }
            } else if (((PogoModeView *)GameInfo)->mode == 11 && f2 &&
                       PogoAI_RadialSearch_CheckKerbs(i, px, pz)) {
                goto found;
            }
        } else {
            miss++;
        }
        if (miss >= 3) {
            return 0;
        }
        off0 += side3;
        off1 += side2;
        off2 += side1;
        off3 += side0;
        off4 += dz;
        off5 += dx;
    }
found:
    if (px < 0) {
        px = 0;
    }
    if (px >= 8) {
        px = 7;
    }
    if (pz < 0) {
        pz = 0;
    }
    if (pz >= 8) {
        pz = 7;
    }
    *outX = px;
    *outZ = pz;
    return PogoGrid[px][pz].unkE == 0;
}

bool PogoAI_StartEnclosing(u32 arg0, EXTbotai *ai) {
    PogoMove *m2;
    EXTengine *eng;
    PogoMove *m;
    PogoMove *base;
    PogoMove *base2;
    s32 x;
    s32 z;
    s32 odd;
    s32 dir;

    if ((u16)GameMode - 0xB >= 2U) {
        return 0;
    }
    if (PogoAI_CountMyTiles(arg0) < 9) {
        return 0;
    }
    {
        eng = Heroes[arg0].hero_eng;
        if (eng != NULL) {
            base = PogoAIPlay;
            m = (PogoMove *)(arg0 * sizeof(PogoMove) + (u32)base);
            x = (eng->unk10.x + 0x800) >> 9;
            z = (eng->unk10.z + 0x800) >> 9;
            odd = GameFrameCount & 1;
            m->unkC = 0;
            m->unk10 = 8;
            m->unk0 = odd;
            if (x & 4) {
                if (z & 4) {
                    dir = odd * 2;
                } else if (odd != 0) {
                    dir = 4;
                } else {
                    dir = 6;
                }
            } else if (z & 4) {
                dir = (odd == 0) * 2;
            } else if (odd != 0) {
                dir = 6;
            } else {
                dir = 4;
            }
            m->unk8 = dir;
            base2 = PogoAIPlay;
            m2 = (PogoMove *)(arg0 * sizeof(PogoMove) + (u32)base2);
            if (m2->unk0 != 0) {
                ai->TargetXYZ.x = x + EncloseStepsOdd[m2->unk8].x;
                ai->TargetXYZ.z = z + EncloseStepsOdd[m2->unk8].z;
            } else {
                ai->TargetXYZ.x = x + EncloseStepsEven[m2->unk8].x;
                ai->TargetXYZ.z = z + EncloseStepsEven[m2->unk8].z;
            }
            ai->AImode = 5;
            return 1;
        }
    }
    return 0;
}

bool PogoAI_StartEncircling(u32 hero, EXTbotai *goal) {
    EXTengine *eng;
    s32 ox;
    s32 oz;
    s32 best;
    s32 px;
    s32 pz;
    s32 bx;
    s32 bz;
    s32 i;
    s32 j;
    s32 ax;
    s32 cx;
    s32 az;
    s32 cz;
    s32 dx;
    s32 dy;
    s32 d;

    if (GameMode == 0xC) {
        return 0;
    }
    eng = Heroes[hero].hero_eng;
    if (eng == NULL) {
        return 0;
    }
    best = 0x0FFFFFFF;
    ox = (eng->unk10.x + 0x800) >> 9;
    oz = (eng->unk10.z + 0x800) >> 9;
    bx = ox;
    bz = oz;
    px = ox << 9;
    pz = oz << 9;
    for (i = 0; ; i++) {
        if (i >= 8) {
            goto done;
        }
        for (j = 0; ; j++) {
            if (j >= 8) {
                goto next;
            }
            if (PogoAI_ClaimingThisTileWillEncircle(i, j, hero) != 0) {
                ax = px - 0x800;
                az = pz - 0x800;
                cx = (i << 9) - 0x800;
                cz = (j << 9) - 0x800;
                dx = abs(ax - cx);
                dy = abs(az - cz);
                d = dx * dx + dy * dy;
                if (d < best) {
                    best = d;
                    bx = i;
                    bz = j;
                }
            }
        }
    next:;
    }
done:
    if (bx != ox || bz != oz) {
        goal->TargetXYZ.x = bx;
        goal->TargetXYZ.z = bz;
        goal->AImode = 6;
        return 1;
    }
    return 0;
}

bool PogoAI_ClaimingThisTileWillEncircle(s32 x, s32 z, s32 hero) {
    s32 grid[64];
    s32 *g;
    PogoCell *base;
    PogoCell *c;
    s32 i;
    s32 j;

    if (PogoAI_CountNeighbours(x, z, hero) < 2) {
        return 0;
    }
    base = PogoGrid[0];
    c = (PogoCell *)(z * sizeof(PogoCell) + x * sizeof(PogoGrid[0]) + (u32)base);
    if (c->unkE != 0) {
        return 0;
    }
    if (c->unkA == hero) {
        return 0;
    }
    g = grid;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            s32 *p = (s32 *)((j * 8 + i) * 4 + (u32)g);
            *p = 0;
            if (PogoGrid[i][j].unkA == hero) {
                *p = 1;
            }
        }
    }
    grid[z * 8 + x] = 1;
    Tiles_FloodFillIslandMap(grid, hero);
    return Tiles_CountUnflagged(grid, hero, 0) != 0;
}

s32 PogoAI_CountNeighbours(s32 arg0, s32 arg1, s32 arg2) {
    s32 n;

    n = 0;
    if (arg0 > 0) {
        s32 v = PogoGrid[arg0 - 1][arg1].unkA;
        n = v == arg2;
    }
    if (arg0 < 7 && PogoGrid[arg0 + 1][arg1].unkA == arg2) {
        n++;
    }
    if (arg1 > 0 && PogoGrid[arg0][arg1 - 1].unkA == arg2) {
        n++;
    }
    if (arg1 < 7 && PogoGrid[arg0][arg1 + 1].unkA == arg2) {
        n++;
    }
    if (arg0 == 0 && TileScan0[arg1] == arg2) {
        n++;
    }
    if (arg0 == 7 && TileScan1[arg1] == arg2) {
        n++;
    }
    if (arg1 == 0 && TileScan3[arg0] == arg2) {
        n++;
    }
    if (arg1 == 7 && TileScan2[arg0] == arg2) {
        n++;
    }
    return n;
}

void Init_PogoRipperRoo(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    EXTmotion *m;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        ScriptCursorFirst(nas, &RooScriptInit);
        dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_Roo]);
        e->sub->u6C.dbase = &HeroModelBank[DB_Roo];
        e->sub->entity = 0x2001;
        e->unk5D = 0;
        nas->UserInfo.status = 0;
        e->unk10.x = -0x100;
        e->unk10.z = 0x900;
        m = AddMotion(nas);
        if (m != NULL) {
            m->Ymax = 0xA0;
            m->Ydec = 0xE;
            m->Yacc = 0;
            m->Ydir = 0xFF;
            m->Yvel = 0;
        } else {
            nas->Failure = 1;
        }
        e2 = AddExtEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = e->unk10.y;
            e2->unk10.z = e->unk10.z;
            if (sub != NULL) {
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x2029;
                sub->unk51 = 2;
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40000041;
            }
        }
    }
    RooFrame = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
        PogoRooNas = 0;
    } else {
        PogoRooNas = (s32)nas;
    }
}

void Plot_PogoRipperRoo(NAS *nas) {
    EXTengine *e;
    EXTanimator *anim;
    EXTengine *e2;
    subengine *sub;
    u8 *g;
    s32 x;
    s32 z;
    s32 tx;
    s32 tz;
    s32 ez;
    s32 scale;
    s32 v;
    s32 i;
    s32 j;
    s32 k;
    s32 near;
    s32 sfx;

    e = nas->Engine;
    if (e == NULL) {
        return;
    }
    anim = FindExtension(nas, 0x400);
    if (anim == NULL) {
        return;
    }
    RunNasAnim(nas);
    if (e->flags & 0x8000) {
        e2 = e->Engine;
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            ez = e->unk10.z;
            e2->unk10.y = World_floor_level;
            e2->unk10.z = ez;
            if (sub != NULL) {
                scale = 0x1600 - abs(e->unk10.y - World_floor_level) * 4;
                sub->unk20.z = scale;
                sub->unk20.y = scale;
                sub->unk20.x = scale;
                sub->unk52 = 1;
            }
        }
        x = (e->unk10.x + 0x800) >> 9;
        z = (e->unk10.z + 0x800) >> 9;
        if ((u32)x < 8 && z >= 0 && z < 8) {
            PogoGrid[x][z].unkE = 5;
        }
    }
    switch (nas->UserInfo.status) {
    case 0:
        RooTileX = -1;
        e->unk10.x = -0x100;
        e->unk10.z = 0x900;
        RooTileX = -1; /* set twice in the original: its registers need this store */
        RooSpeedX = 0;
        RooSpeedZ = 0;
        RooAngle = 0;
        nas->UserInfo.user1 = 0xA0;
        nas->UserInfo.status = 1;
        GotoLinkAnim(anim, 0);
        break;
    case 1:
        if (--nas->UserInfo.user1 < 2) {
            RooDir = (GameFrameCount & 1) ? 1 : 3;
            nas->UserInfo.user1 = Random(0x3C) + 0x64;
            nas->UserInfo.status = 2;
            PlaySfx(0x542, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            GotoLinkAnim(anim, 1);
            RooBounce = 0;
        }
        break;
    case 2:
        if (nas->UserInfo.user1 > 0 && !(GameExitFlags & 0x801)) {
            nas->UserInfo.user1--;
        }
        Roo_UpdatePosition(e);
        if (!Roo_Bounce(nas)) {
            break;
        }
        g = GameInfo;
        if ((*(s32 *)(g + 0x30) & 0x800801) == 0x800000) {
            for (i = 0; i < 4; i++) {
                near = 0;
                if (GameInfo[i + 0x5C] == 0) {
                    if (abs(e->unk10.x - Heroes[i].hero_eng->unk10.x) < 0x580) {
                        near = abs(e->unk10.z - Heroes[i].hero_eng->unk10.z) < 0x580;
                    }
                    if (near && !(Heroes[i].hero_status & 0x800)) {
                        nas->UserInfo.user1 = 0;
                    }
                }
            }
        }
        if (nas->UserInfo.user1 > 0) {
            RooTileX = (e->unk10.x + 0x800) >> 9;
            RooTileZ = (e->unk10.z + 0x800) >> 9;
            switch (RooDir) {
            case 0:
                RooTileZ -= 2;
                break;
            case 2:
                RooTileZ += 2;
                break;
            case 3:
                RooTileX += 2;
                break;
            case 1:
                RooTileX -= 2;
                break;
            }
            Roo_ChangeDirOnEdge();
        } else {
            RooTileX = (e->unk10.x + 0x800) >> 9;
            RooTileZ = (e->unk10.z + 0x800) >> 9;
            if (RooTileX == 8) {
                RooDir = 1;
            } else if (RooTileX == -1) {
                RooDir = 3;
            } else if (RooTileZ == 8) {
                RooDir = 0;
            } else if (RooTileZ == -1) {
                RooDir = 2;
            }
            if (GameLevelFlags & 0x80) {
                v = RooFrames[RooFrame];
            } else {
                v = RooHopFrames[RooFrame];
            }
            if (v == 12) {
                RooFrame = 0;
                if (GameLevelFlags & 0x80) {
                    v = RooFrames[0];
                } else {
                    v = RooHopFrames[0];
                }
            } else {
                RooFrame++;
            }
            switch (v - 6) {
            case 1:
                nas->UserInfo.status = 3;
                Roo_FindNewSquare(e);
                RooWait = Random(3) + 1;
                break;
            case 0:
                nas->UserInfo.status = 3;
                Roo_FindNewSquare(e);
                RooWait = 0x32;
                break;
            case 2:
                nas->UserInfo.status = 4;
                nas->UserInfo.user2 = 0x19;
                Roo_FindNewSquare(e);
                RooWait = Random(3) + 1;
                break;
            case 3:
            case 4:
                nas->UserInfo.status = v;
                Roo_FindNewSquare(e);
                RooWait = 0x32;
                break;
            }
            RooSteps = 0;
            sfx = 0x543;
            if (GameFrameCount & 1) {
                sfx = 0x542;
            }
            PlaySfx(sfx, NULL, &e->unk10, 0x1000, 0x1E00, 0);
        }
        Roo_UpdateVelocities(e);
        break;
    case 3:
    case 4:
    case 9:
    case 10:
        Roo_UpdatePosition(e);
        if (!Roo_Bounce(nas)) {
            break;
        }
        tx = (e->unk10.x + 0x800) >> 9;
        tz = (e->unk10.z + 0x800) >> 9;
        if ((u32)tx < 8 && tz >= 0 && tz < 8 && RooSteps < 100) {
            if (nas->UserInfo.status == 3) {
                if (RooSteps == RooWait) {
                    PogoGrid[tx][tz].unkA = 5;
                    PogoGrid[tx][tz].unk12 = 7;
                } else {
                    PogoGrid[tx][tz].unkA = 5;
                    PogoGrid[tx][tz].unk12 = 5;
                    PogoGrid[tx][tz].unk14 = 0x78;
                }
                Tile_Update(tx, tz);
                if (RooSteps == 0) {
                    PogoGrid[tx][tz].unk18[0] = 1;
                } else {
                    PogoGrid[tx][tz].unk18[0] = 0;
                }
            } else if (nas->UserInfo.status == 4) {
                if (RooSteps == RooWait) {
                    nas->UserInfo.status = 5;
                    nas->UserInfo.user1 = 0x8C;
                    GotoLinkAnim(anim, 2);
                }
            } else if (nas->UserInfo.status == 9) {
                Roo_ShootAtHumans(nas);
            } else if (nas->UserInfo.status == 10) {
                i = 0;
                for (;;) {
                    s32 cx;
                    s32 cz;

                    if (i >= 4) {
                        break;
                    }
                    near = 0;
                    if (GameInfo[i + 0x5C] == 0) {
                        if (abs(e->unk10.x - Heroes[i].hero_eng->unk10.x) < 0x480) {
                            near = abs(e->unk10.z - Heroes[i].hero_eng->unk10.z) < 0x480;
                        }
                        if (near && !(Heroes[i].hero_status & 0x800)) {
                            cx = (e->unk10.x + 0x800) >> 9;
                            cz = (e->unk10.z + 0x800) >> 9;
                            PogoGrid[cx][cz].unkA = 5;
                            PogoGrid[cx][cz].unk12 = 6;
                            PogoGrid[cx][cz].unk14 = TileLockTimes[PogoLevelIndex][0];
                            Tile_Update((e->unk10.x + 0x800) >> 9, (e->unk10.z + 0x800) >> 9);
                        }
                    }
                    i++;
                }
            }
            RooSteps++;
        }
        if (Roo_FindNewSquare(e)) {
            nas->UserInfo.status = 2;
            for (k = 0; k < 8; k++) {
                for (j = 0; j < 8; j++) {
                    if (PogoGrid[k][j].unk10 == 5) {
                        PogoGrid[k][j].unk10 = 0;
                    }
                }
            }
            nas->UserInfo.user1 = Random(0x50) + 0x64;
            GotoLinkAnim(anim, 1);
            RooBounce = 0;
        }
        Roo_UpdateVelocities(e);
        break;
    case 5:
        if (nas->UserInfo.user1 > 0) {
            nas->UserInfo.user1--;
        }
        e->unk5E = 1;
        e->unk1E += 0x16;
        if (e->sub->entity == 0x4012) {
            PlaySfx(0x522, NULL, &e->unk10, 0x1000, 0x1E00, 0);
            Roo_FireRocket(e);
        }
        if (nas->UserInfo.user1 <= 0 || (GameExitFlags & 0x801)) {
            nas->UserInfo.status = 4;
            RooSteps = 100;
            GotoLinkAnim(anim, 1);
            RooBounce = 0;
        }
        break;
    }
}

void Roo_UpdatePosition(EXTengine *eng) {
    u32 x;
    u32 z;
    s32 i;
    s32 j;
    s32 a;

    x = RooTileX;
    z = RooTileZ;
    if (x < 8 && z < 8) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (PogoGrid[i][j].unk10 == 5) {
                    PogoGrid[i][j].unk10 = 0;
                }
            }
        }
        PogoGrid[x][z].unk10 = 5;
    }
    if ((u32)(RooBounce - 2) < 10) {
        eng->unk10.x += RooSpeedX;
        eng->unk10.z += RooSpeedZ;
    }
    a = angle_diff(RooAngle, eng->unk1E);
    eng->unk5E = 1;
    eng->unk1E -= a >> 1;
}

bool Roo_ChangeDirOnEdge(void) {
    if ((u32)(RooTileZ + 1) >= 10) {
        if (RooTileZ < -1) {
            RooTileZ = -1;
        }
        if (RooTileZ > 8) {
            RooTileZ = 8;
        }
        if (RooTileX == 8) {
            RooTileX = 7;
            RooDir = 1;
        } else if (RooTileX == -1) {
            RooTileX = 0;
            RooDir = 3;
        }
        return 1;
    }
    if (RooTileZ == 8 || RooTileZ == -1) {
        if (RooTileX == 8) {
            RooDir = (RooTileZ == 8) ? (RooDir == 2)
                                           : (RooDir == 0 ? 1 : 2);
            return 1;
        }
        if (RooTileX == -1) {
            RooDir = (RooTileZ == 8) ? (RooDir == 2 ? 3 : 0)
                                           : (RooDir == 0 ? 3 : 2);
            return 1;
        }
    }
    if ((u32)(RooTileX + 1) >= 10) {
        if (RooTileX < -1) {
            RooTileX = -1;
        }
        if (RooTileX > 8) {
            RooTileX = 8;
        }
        if (RooTileZ == 8) {
            RooTileZ = 7;
            RooDir = 0;
        } else if (RooTileZ == -1) {
            RooTileZ = 0;
            RooDir = 2;
        }
        return 1;
    }
    if (RooTileX == -1 || RooTileX == 8) {
        if (RooTileZ == 8) {
            RooDir = (RooTileX == 8) ? (RooDir == 2 ? 3 : 0)
                                           : (RooDir != 3);
            return 1;
        }
        if (RooTileZ == -1) {
            RooDir = (RooTileX == 8) ? ((RooDir == 1) * 2)
                                           : (RooDir == 3 ? 2 : 1);
            return 1;
        }
    }
    return 0;
}

bool Roo_FindNewSquare(EXTengine *eng) {
    s32 count;
    s32 ok;
    s32 x;
    s32 z;
    s32 v;
    s32 *pz;
    s32 *px;

    count = 0;
    do {
        ok = 1;
        count++;
        RooTileX = (eng->unk10.x + 0x800) >> 9;
        RooTileZ = (eng->unk10.z + 0x800) >> 9;
        z = RooDir;
        px = &RooTileX;
        if (count > 5) {
            RooDir = Random(4);
            count = 0;
        }
        switch (RooDir) {
        case 0:
            RooTileZ -= 2;
            RooTileX -= 1 - Random(3);
            break;
        case 2:
            RooTileZ += 2;
            RooTileX -= 1 - Random(3);
            break;
        case 3:
            RooTileX += 2;
            RooTileZ -= 1 - Random(3);
            break;
        case 1:
            RooTileX -= 2;
            RooTileZ -= 1 - Random(3);
            break;
        }
        x = RooTileX;
        if ((u32)x >= 8) {
            RooDir = (GameFrameCount & 1) ? 0 : 2;
            if ((u32)(x + 1) >= 10) {
                pz = &RooTileZ;
                v = *pz;
                if (v < 4) {
                    v++;
                } else {
                    v--;
                }
                *pz = v;
                if (RooTileX < -1) {
                    RooTileX = -1;
                }
                if (RooTileX > 8) {
                    RooTileX = 8;
                }
            }
            return 1;
        }
        z = RooTileZ;
        if ((u32)z >= 8) {
            RooDir = (GameFrameCount & 1) ? 1 : 3;
            if ((u32)(z + 1) >= 10) {
                if (x < 4) {
                    x++;
                } else {
                    x--;
                }
                *px = x;
                if (RooTileZ < -1) {
                    RooTileZ = -1;
                }
                if (RooTileZ > 8) {
                    RooTileZ = 8;
                }
            }
            return 1;
        }
        if ((SquareIsFreeForRoo(x, z) ^ 1) != 0) {
            ok = 0;
        }
        if (PogoGrid[RooTileX][RooTileZ].hero != NULL) {
            ok = 0;
        }
    } while (!ok);
    return 0;
}

void Roo_GoBoing(EXTengine *eng) {
    switch (Random(6)) {
    case 0:
        PlaySfx(0x53C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x53D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x53E, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x53F, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 4:
        PlaySfx(0x540, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x541, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

void Roo_FireRocket(EXTengine *eng) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;
    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;
    s32 angle = eng->unk1E;

    nas = CreateNAS(x, World_floor_level - 0x200, z, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user2 = 1;
                nas->UserInfo.flags = 0x502;
                nas->UserInfo.user1 = 0;
                nas->UserInfo.status = angle;
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x200D;
                eng_add_subs(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                sub->entity = 0x200D;
                e->unk1E = (angle + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
}

void Roo_ShootAtHumans(NAS *nas) {
    EXTengine *eng;
    EXTengine *he;
    s32 x, z, hx, hz;
    s32 i;

    eng = nas->Engine;
    i = 0;
    if (eng == NULL) {
        return;
    }
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    for (; i < 4; i++) {
        if (GameInfo[i + 0x5C] != 0 || Heroes[i].hero_mode == 0x3B) {
            continue;
        }
        he = Heroes[i].hero_eng;
        hx = (he->unk10.x + 0x800) >> 9;
        hz = (he->unk10.z + 0x800) >> 9;
        if (x != hx && z != hz) {
            continue;
        }
        if (x == hx) {
            if (z < hz) {
                Roo_FireProjectile(eng, 0x800);
            }
            if (hz < z) {
                Roo_FireProjectile(eng, 0);
            }
        }
        if (z == hz) {
            if (x < hx) {
                Roo_FireProjectile(eng, 0xC00);
            }
            if (hx < x) {
                Roo_FireProjectile(eng, 0x400);
            }
        }
    }
}

void Roo_FireProjectile(EXTengine *eng, s32 angle) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;

    nas = CreateNAS(x, World_floor_level - 0x280, z, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user2 = 1;
                nas->UserInfo.flags = 0x502;
                nas->UserInfo.user1 = 0;
                nas->UserInfo.status = angle;
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x200D;
                eng_add_subs(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                sub->entity = 0x200D;
                e->unk1E = (angle + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
    PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
}

void Init_PogoPadlock(NAS *nas) {
    EXTengine *e;
    EXTengine *e2;
    subengine *sub;

    e = nas->Engine;
    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        e->sub->entity = 0x2041;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        e->unk5C = 1;
        e->flags &= ~0x8000;
        e2 = AddExtEngine(nas, 1);
        if (e2 != NULL) {
            sub = e2->sub;
            e2->unk10.x = e->unk10.x;
            e2->unk10.y = World_floor_level;
            e2->unk10.z = e->unk10.z;
            e2->flags &= ~0x8000;
            if (sub != NULL) {
                sub->entity = 0x2029;
                sub->unk51 = 2;
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->unk50 = 1;
                sub->unk76 = 0x800;
                sub->flags |= 0x40008041;
            }
        }
    } else {
        nas->Failure = 1;
    }
    nas->UserInfo.user1 = 0xF0;
    nas->UserInfo.status = 0;
    nas->UserInfo.flags = PadlockFlags[0];
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Plot_PogoPadlock(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    EXTengine *eng2;
    s32 cx;
    s32 cz;
    s32 tries;
    s32 dir;
    s32 ok;
    s32 nx;
    s32 nz;
    s32 tx;
    s32 tz;
    s32 cell;
    s32 dx;
    s32 dz;
    u16 init;
    u32 flags;
    s32 count;
    s32 level;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    flags = nas->UserInfo.flags;
    count = flags & 0xFF;
    level = flags >> 8;
    eng->unk5E = 1;
    eng->unk1E += 0x38;
    eng2 = eng->Engine;
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 > 0) {
            break;
        }
        if (!Pogo_FindFreeSquare(&x, &z)) {
            break;
        }
        nas->UserInfo.status = 1;
        nas->UserInfo.user1 = 0xA;
        init = PadlockFlags[0];
        count = init & 0xFF;
        nas->UserInfo.flags = init;
        level = init >> 8;
        eng->unk2C.y = 0;
        eng->unk5E = 1;
        eng->unk10.x = (x << 9) - 0x700;
        eng->unk10.z = (z << 9) - 0x700;
        eng->flags |= 0x8000;
        if (eng2 == NULL) {
            break;
        }
        eng2->flags |= 0x8000;
        goto follow;
    case 1:
        eng->unk2C.y += 0x100;
        if (eng->unk2C.y > 0x1000) {
            eng->unk2C.y = 0x1000;
            nas->UserInfo.status = 3;
            nas->UserInfo.user1 = PadlockTimes[level];
        }
        eng->unk5E = 1;
        break;
    case 3:
        if (--nas->UserInfo.user1 > 0) {
            break;
        }
        if (level < 7) {
            if (--count <= 0) {
                count = Pogo_PadlockMoves(++level);
            }
        }
        nas->UserInfo.status = 4;
        break;
    case 4:
        tries = 0;
        cz = (eng->unk10.z + 0x800) >> 9;
        cx = (eng->unk10.x + 0x800) >> 9;
        do {
            do {
                ok = 1;
                dir = Random(4);
                if (dir == 1) {
                    ok = cx > 0;
                }
                if (dir == 0 && cz <= 0) {
                    ok = 0;
                }
                if (dir == 3 && cx >= 7) {
                    ok = 0;
                }
                if (dir == 2 && cz >= 7) {
                    ok = 0;
                }
            } while (ok == 0);
            nx = cx;
            nz = cz;
            switch (dir) {
            case 1:
                nx--;
                break;
            case 3:
                nx++;
                break;
            case 2:
                nz++;
                break;
            case 0:
                nz--;
                break;
            }
            tries++;
            if (tries >= 5) {
                goto fail;
            }
        } while ((PadlockAI_IsTileOKForPadlock(nas, nx, nz) ^ 1) != 0);
        nas->UserInfo.user2 = ((nz & 7) << 3) | (nx & 7);
        nas->UserInfo.status = 5;
        nas->UserInfo.user1 = 0x12;
        break;
    fail:
        nas->UserInfo.status = 3;
        nas->UserInfo.user1 = 5;
        break;
    case 5:
        cell = nas->UserInfo.user2;
        tx = ((cell & 7) << 9) - 0x700;
        tz = (((cell >> 3) & 7) << 9) - 0x700;
        dx = (tx - eng->unk10.x) / 6;
        dz = (tz - eng->unk10.z) / 6;
        eng->unk10.x += dx;
        eng->unk10.z += dz;
        if (--nas->UserInfo.user1 <= 0) {
            eng->unk10.x = tx;
            eng->unk10.z = tz;
            nas->UserInfo.status = 3;
            nas->UserInfo.user1 = PadlockTimes[level];
        }
        if (eng2 == NULL) {
            break;
        }
    follow:
        eng2->unk10.x = eng->unk10.x;
        eng2->unk10.z = eng->unk10.z;
        break;
    case 6:
        eng->flags &= ~0x8000;
        if (eng2 != NULL) {
            eng2->flags &= ~0x8000;
        }
        nas->UserInfo.user1 = 0x78;
        nas->UserInfo.status = 0;
        Pogo_SpawnPuff((s32 *)&eng->unk10, 1, &HeroModelBank[DB_PogoObjects]);
        break;
    }
    nas->UserInfo.flags = ((level & 0xFF) << 8) | (count & 0xFF);
}

bool PogoPadlock_IsHere(s32 arg0, s32 arg1) {
    EXTengine *e;

    if (PogoPadlockNas != NULL) {
        e = PogoPadlockNas->Engine;
        if (e != NULL) {
            if ((u16)PogoPadlockNas->UserInfo.status >= 3 && (e->unk10.x + 0x800) >> 9 == arg0
                && (e->unk10.z + 0x800) >> 9 == arg1) {
                return 1;
            }
        }
    }
    return 0;
}

bool PadlockAI_IsTileOKForPadlock(NAS *arg0, s32 arg1, s32 arg2) {
    PogoCell *p;

    if ((u32)arg1 >= 8 || (u32)arg2 >= 8) {
        return 0;
    }
    if (PogoGrid[arg1][arg2].hero != NULL) {
        return 0;
    }
    p = &PogoGrid[arg1][arg2];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    return p->unk10 == 0;
}

void Create_PogoPadlock(s32 arg0, s32 arg1) {
    PogoPadlockNas = CreateNAS((arg0 << 9) - 0x700, World_floor_level, (arg1 << 9) - 0x700, 0x1408, 0x11);
}

void CreateFloatyPogoScore(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    CreateFloatyScore((arg1 << 9) - 0x700, World_floor_level - 0x100, (arg2 << 9) - 0x800, arg3, 0);
}

bool SquareIsFreeForRoo(s32 arg0, s32 arg1) {
    PogoCell *p;
    u16 t;

    if ((u32)arg0 >= 8 || (u32)arg1 >= 8) {
        return 1;
    }
    p = &PogoGrid[arg0][arg1];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    t = p->unk10;
    if (t != 0 && t != 5) {
        return 0;
    }
    return 1;
}

bool Roo_Bounce(NAS *nas) {
    if (++RooBounce < 14) {
        return 0;
    }
    RooBounce = 0;
    if (nas->Engine != NULL) {
        Roo_GoBoing(nas->Engine);
    }
    return 1;
}

void Roo_UpdateVelocities(EXTengine *eng) {
    s32 dx;
    s32 dz;

    dx = (RooTileX << 9) - eng->unk10.x - 0x700;
    dz = (RooTileZ << 9) - eng->unk10.z - 0x700;
    RooSpeedX = dx / 10;
    RooSpeedZ = dz / 10;
    RooAngle = get_angle(dx, dz) + 0x800;
}

s32 PogoAI_CountMyTiles(u32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (PogoGrid[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

bool PogoAI_RadialSearch_CheckKerbs(s32 arg0, s32 arg1, s32 arg2) {
    if ((u32)arg2 < 8) {
        if (arg1 == -1 && TileScan0[arg2] == arg0) {
            return 1;
        }
        if (arg1 == 8 && TileScan1[arg2] == arg0) {
            return 1;
        }
    }
    if ((u32)arg1 < 8) {
        if (arg2 == -1 && TileScan3[arg1] == arg0) {
            return 1;
        }
        if (arg2 == 8 && TileScan2[arg1] == arg0) {
            return 1;
        }
    }
    return 0;
}

bool PogoAI_GoForMyOwn(u32 arg0, EXTbotai *ai)
{
    s32 x;
    s32 z;

    EXTengine *e = Heroes[arg0].hero_eng;
    s32 a = Heroes[arg0].hero_move->facing_angle >> 10;

    if (PogoAI_RadialSearch(arg0, (e->unk10.x + 0x800) >> 9, (e->unk10.z + 0x800) >> 9,
                      a, &x, &z) != 0) {
        ai->AImode = 7;
        ai->TargetXYZ.x = x;
        ai->TargetXYZ.z = z;
        return 1;
    }
    return 0;
}

void PogoAI_HeroClaimedPickup(u32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AItimer1 = PogoAiTimers[PogoAiQualityIndex][0];
    }
}

void PogoAI_InitHero(u32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
        ai->AItimer2 = Random(3) + 3;
    }
}

void PogoKerb_CopyData(void) {
    s32 i;
    u8 (*p)[8] = KerbData[KerbStep];

    for (i = 0; i < 8; i++) {
        TileScan0[i] = p[0][i];
        TileScan2[i] = p[1][i];
        TileScan1[7 - i] = p[2][i];
        TileScan3[7 - i] = p[3][i];
    }
}

void PogoKerb_Init(void) {
    KerbStep = 0;
    KerbDelay = Random(0xC8) + 0xC8;
    KerbCount = 0;
    KerbPeriod = 8;
    KerbClockSecond = 0;
    KerbMode = 0;
    if (GameLevelFlags & 0x80) {
        KerbDelay = 0x78;
    }
    PogoKerb_CopyData();
}

void Init_PogoProjectile(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void Create_PogoProjectile(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    nas = CreateNAS(arg1, World_floor_level - arg3, arg2, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user1 = 0;
                nas->UserInfo.user2 = arg0;
                nas->UserInfo.status = arg5;
                nas->UserInfo.flags = ((arg4 & 7) << 8) | 2;
                sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                sub->entity = 0x200D;
                eng_add_subs(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->u6C.dbase = &HeroModelBank[DB_PogoObjects];
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                switch (arg0) {
                case 1:
                    sub->entity = 0x200D;
                    break;
                case 2:
                    sub->entity = 0x201B;
                    break;
                case 3:
                    sub->entity = 0x2040;
                    break;
                }
                e->unk1E = (arg5 + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
}

void Fire_RocketInDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;

    eng = Heroes[arg0].hero_eng;
    Create_PogoProjectile(arg1, (((eng->unk10.x + 0x800) >> 9) << 9) - 0x700,
                  (((eng->unk10.z + 0x800) >> 9) << 9) - 0x700, arg3, arg0 & 7, arg2);
}

void Player_SetPoisoned(u32 arg0) {
    PogoPlay[arg0].flags |= 1;
}

void Player_SetDazed(u32 arg0) {
    Heroes[arg0].hero_status |= 0x800;
}

s32 Player_CountTiles(u32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (PogoGrid[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

s32 Player_CalcForwardSpeed(u32 arg0, s32 arg1)
{
    s32 h;
    s32 n;
    s32 r;
    s32 *a = PogoTilesMid;

    h = Heroes[arg0].hero_name;
    n = a[h] - PogoTilesHigh[h] - PogoTilesLow[h];
    r = 0x200 / n;
    if (arg1 != 0) {
        if (n & 1) {
            r = (0x200 / (n + 1)) * 2;
        } else {
            r *= 2;
        }
    }
    return r;
}

void Player_SnapToTile(u32 arg0) {
    Heroes[arg0].hero_eng->unk10.x = (((Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9) << 9) - 0x700;
    Heroes[arg0].hero_eng->unk10.z = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;
}

bool SquareContainsMushroom(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    HeroInfo *hero = ((PogoCell *)(arg0 * sizeof(PogoGrid[0]) + arg1 * sizeof(PogoCell) + (u32)base))->hero;

    if (hero != NULL && hero->hero_pu_action == 0xB) {
        return 1;
    }
    return 0;
}

bool FindFreeSquare(s32 *px, s32 *pz) {
    s32 x;
    s32 z;
    s32 i;
    s32 taken;

    i = 0;
    do {
        x = Random(8);
        z = Random(8);
        taken = SquareIsFreeForMushroom(x, z) != 1;
        i++;
    } while (taken && i < 10);
    if (i >= 10) {
        return 0;
    }
    *px = x;
    *pz = z;
    return 1;
}

u32 PogoSquaredDistanceToDestination(x0, y0, x1, y1)
s32 x0;
s32 y0;
s32 x1;
s32 y1;
{
    s32 dx;
    s32 dy;
    s32 a;
    s32 b;

    a = (y0 << 9) - 0x800;
    b = (y1 << 9) - 0x800;
    dx = abs((x0 - x1) << 9);
    dy = abs(a - b);
    return dx * dx + dy * dy;
}

u32 PogoDistanceToDestination(s32 x0, s32 y0, s32 x1, s32 y1) {
    return csqrt(PogoSquaredDistanceToDestination(x0, y0, x1, y1)) >> 6;
}

s32 LevelSpecific_UnclaimedTile(s32 x, s32 z) {
    D5A614View *g = (D5A614View *)GameInfo;
    s32 v;

    switch (g->mode) {
    case 10:
        v = 0x303C;
        if (g->flags & 0x80) {
            v = 0x3033;
        }
        return v;
    case 11:
        v = 0x3052;
        if (g->flags & 0x80) {
            v = 0x301B;
        }
        return v;
    case 12:
        v = 0x3035;
        if (g->flags & 0x80) {
            v = 0x303F;
        }
        return v;
    case 13:
        return 0x3000;
    }
    return 0;
}

void Island_FloodFill(s32 x, s32 z, u32 id, s32 *seen) {
    if ((u32)x < 8 && (u32)z < 8 && PogoGrid[x][z].unkA != id && seen[z * 8 + x] == 0) {
        seen[z * 8 + x] = 1;
        Island_FloodFill(x - 1, z - 1, id, seen);
        Island_FloodFill(x, z - 1, id, seen);
        Island_FloodFill(x + 1, z - 1, id, seen);
        Island_FloodFill(x - 1, z, id, seen);
        Island_FloodFill(x + 1, z, id, seen);
        Island_FloodFill(x - 1, z + 1, id, seen);
        Island_FloodFill(x, z + 1, id, seen);
        Island_FloodFill(x + 1, z + 1, id, seen);
    }
}

void Tiles_MakeUpIslandMap(s32 *arg0, s32 arg1) {
    s32 i;
    s32 j;
    s32 *p;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            p = (s32 *)((i * 8 + j) * sizeof(s32) + (u32)arg0);
            *p = 0;
            if (PogoGrid[j][i].unkA == arg1) {
                *p = 1;
            }
        }
    }
}

void Tiles_CheckForIslands(s32 arg0) {
    s32 grid[64];

    if (((D5A614View *)GameInfo)->mode != 10 && ((D5A614View *)GameInfo)->mode != 13) {
        Tiles_MakeUpIslandMap(grid, arg0);
        Tiles_FloodFillIslandMap(grid, arg0);
        if (Tiles_CountUnflagged(grid, arg0, 1) != 0) {
            PlaySfx(0x51D, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            if (((D5A614View *)GameInfo)->mode == 11) {
                Bank_Tiles(arg0);
            }
        }
    }
}

void Init_PogoMushroom(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_ArenaPogoPainterCrystalarena];
        e->sub->entity = 0x2007;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 11;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        MushroomCount++;
    }
}

void Init_PogoBanker(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        e->sub->entity = 0x200B;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 10;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = Random(60) + 60;
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoPickup(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        e->sub->entity = 0x200B;
        e->sub->unk68 = -0x10;
        e->sub->flags |= 0x02000000;
    }
    nas->UserInfo.flags = 0;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0xF0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoTazer(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        e->sub->entity = 0x2005;
        e->unk5D = 2;
    }
    nas->UserInfo.flags = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Pickup_Lose(arg0)
s32 arg0;
{
    PogoPlayer *base;
    PogoPlayer *slot;

    if (PogoPlay[arg0].status == 5 && Heroes[arg0].hero_aseq == 0x2F) {
        SetHeroAction(arg0, 0x39);
    }
    base = PogoPlay;
    slot = &base[arg0];
    if (slot->status == 6) {
        PogoFour_UnfreezeMyTiles(arg0);
    }
    slot->status = 0;
}

bool SetTile_PogoFour(s32 arg0, s32 arg1, s32 arg2, bool arg3) {
    u8 *g = GameInfo;
    PogoCell *base;
    PogoCell *p;

    if (((D5A614View *)g)->mode != 13) {
        return 0;
    }
    if ((((D5A614View *)g)->flags & 0x80) && g[arg0 + 0x5C] == 0) {
        return 1;
    }
    base = PogoGrid[0];
    p = (PogoCell *)(arg2 * sizeof(PogoCell) + arg1 * sizeof(PogoGrid[0]) + (u32)base);
    if ((u16)p->unk12 == 10) {
        if (arg3 != 0) {
            PlaySquareLockedNoise(arg0);
        }
        return 1;
    }
    if (p->unkA != arg0) {
        return 0;
    }
    if (*(u16 *)p->unkC & 0x10) {
        return 0;
    }
    PogoFour_PlayerHasStoodOnOwnTile(arg0);
    Tile_Update(arg1, arg2);
    return 1;
}

bool SetTile_PogoThree(s32 arg0, s32 arg1, s32 arg2, bool arg3) {
    PogoCell *base;
    PogoCell *c;

    if (GameMode == 0xC) {
        base = PogoGrid[0];
        c = (PogoCell *)(arg2 * sizeof(PogoCell) + arg1 * sizeof(PogoGrid[0]) + (u32)base);
        if ((u16)c->unk12 == 7) {
            Tile_Claim(arg1, arg2, arg0);
            if (arg1 > 0) {
                ZapInDir(arg1, arg2, -1, 0, arg0);
            }
            if (arg1 < 7) {
                ZapInDir(arg1, arg2, 1, 0, arg0);
            }
            if (arg2 > 0) {
                ZapInDir(arg1, arg2, 0, -1, arg0);
            }
            if (arg2 < 7) {
                ZapInDir(arg1, arg2, 0, 1, arg0);
            }
            Tiles_CheckForIslands(arg0);
            if (arg3 != 0) {
                PlaySfx(0x515, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

void ZapFourWays(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 > 0) {
        ZapInDir(arg0, arg1, -1, 0, arg2);
    }
    if (arg0 < 7) {
        ZapInDir(arg0, arg1, 1, 0, arg2);
    }
    if (arg1 > 0) {
        ZapInDir(arg0, arg1, 0, -1, arg2);
    }
    if (arg1 < 7) {
        ZapInDir(arg0, arg1, 0, 1, arg2);
    }
}

void ZapArrow(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas = (NAS *)PogoGrid[arg0][arg1].hero;
    s32 dir;
    s32 i;
    s32 dx;
    s32 dz;

    if (nas != NULL) {
        if (nas->Engine != NULL) {
            PlaySfx(0x515, NULL, &nas->Engine->unk10, 0x1000, 0x1E00, 0);
        }
        dir = nas->UserInfo.user1 & 3;
        for (i = 0; i < arg3; i++) {
            switch (dir) {
            case 0:
                dx = 0;
                dz = 1;
                break;
            case 1:
                dx = 1;
                dz = 0;
                break;
            case 2:
                dx = 0;
                dz = -1;
                break;
            case 3:
                dx = -1;
                dz = 0;
                break;
            default:
                dx = 0;
                dz = 0;
                break;
            }
            ZapInDir(arg0, arg1, dx, dz, arg2);
            dir ^= 2;
        }
    }
}

void Tile_FireAllNitros(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if ((u16)PogoGrid[i][j].unk12 == 6) {
                Tile_FireNitro(i, j);
            }
        }
    }
}

void Tile_OnFire(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 4;
    p->unk14 = arg3;
    p->unk16 = arg2;
    p->unkA = arg2;
}

void Tile_DelayedFizzleOut(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 2;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void Tile_FizzleOut(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 3;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void Tile_DelayedSet(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);
    s16 old = p->unkA;

    p->unkA = arg2;
    p->unk14 = arg3;
    p->unk12 = 0;
    p->unk16 = old;
    if (GameMode == 0xB) {
        if (arg0 > 0) {
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
        }
    }
}

void Tile_SetFourWay(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 7;
}

void Tile_SetNitro(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 6;
    p->unk14 = TileLockTimes[PogoLevelIndex][0];
}

void Tile_SetTnt(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 5;
    p->unk14 = 0x78;
}

void Tile_SetLocked(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 10;
    p->unkA = arg2;
    p->unk14 = 0xF0;
}

void Tile_Set(s32 arg0, s32 arg1, s32 arg2) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = arg2;
    if (GameMode == 0xB) {
        if (arg0 > 0) {
            Tile_Update(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            Tile_Update(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            Tile_Update(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            Tile_Update(arg0, arg1 + 1);
        }
    }
}

void Tile_Clear(s32 arg0, s32 arg1) {
    PogoCell *base = PogoGrid[0];
    PogoCell *p = (PogoCell *)(arg1 * sizeof(PogoCell) + arg0 * sizeof(PogoGrid[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
}

s32 GetPogoContour(EXTengine *arg0) {
    return arg0->unk10.y - World_floor_level;
}

void AddTazerRing(u32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = &HeroModelBank[DB_PogoObjects];
    nas = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 7;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    r->Engine->unk10.y -= 0x40;
}

void Player_SetReserve(u32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 j;

    if ((u32)arg1 < 8 && (u32)arg2 < 8) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (PogoGrid[i][j].unk10 == arg0 + 1) {
                    PogoGrid[i][j].unk10 = 0;
                }
            }
        }
        PogoGrid[arg1][arg2].unk10 = arg0 + 1;
    }
}

void Player_RemoveReserve(u32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (PogoGrid[i][j].unk10 == arg0 + 1) {
                PogoGrid[i][j].unk10 = 0;
            }
        }
    }
}

void HeroPogoTied(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *eng;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_frozen = 2;
    Pickup_Lose(arg0);
    eng = h->hero_eng;
    if (eng != NULL) {
        s32 r = angle_diff_abs(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
    ProcessHeroAnim(arg0);
}

void SetHeroPogoTied(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    Player_RemoveReserve(arg0);
    Pickup_Lose(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
}

bool TestHeroPogoTied(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        if (PogoPlay[arg0].tazer != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction(arg0, 0x3E);
        SetHeroSeq(arg0, 0x2E);
        return 1;
    }
    return 0;
}

bool TestHeroPogoLoser(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        if (PogoPlay[arg0].tazer != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction(arg0, 0x3D);
        SetHeroSeq(arg0, 0x2E);
        return 1;
    }
    return 0;
}

void SetHeroPogoWinner(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    Player_RemoveReserve(arg0);
    StartHeroVictory(arg0);
    Pickup_Lose(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    CameraMode = 1;
}

bool TestHeroPogoWinner(u32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        if (PogoPlay[arg0].tazer != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction(arg0, 0x3C);
        SetHeroSeq(arg0, 0x32);
        return 1;
    }
    return 0;
}

void HeroPogoTazingCancel(u32 arg0) {
    PogoPlayer *base = PogoPlay;
    PogoPlayer *slot = (PogoPlayer *)(arg0 * sizeof(PogoPlayer) + (u32)base);

    if (slot->tazer != NULL) {
        KillNAS(slot->tazer);
        slot->tazer = NULL;
    }
}

void SetHeroPogoTazing(u32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *move;

    Player_RemoveReserve(arg0);
    SetHeroSeq(arg0, 0x30);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    move = h->hero_move;
    if (move != NULL) {
        move->velocity_enable = 0;
        move->forward_angle = move->facing_angle;
    }
    h->hero_frozen = 0xA;
    PogoPlay[arg0].timer = 0x14;
}

bool TestHeroPogoShootCancel(u32 arg0) {
    PogoPlayer *base = PogoPlay;
    PogoPlayer *slot = (PogoPlayer *)(arg0 * sizeof(PogoPlayer) + (u32)base);

    if (slot->tazer != NULL) {
        HeroPogoTazingCancel(arg0);
        return 1;
    }
    return 0;
}

bool TestHeroPogoShoot(u32 arg0) {
    if (Pads[arg0].unk54 & 0x8000) {
        switch (PogoPlay[arg0].status) {
        case 1:
            Fire_Rocket(arg0, 0);
            break;
        case 2:
            Fire_RocketFourWays(arg0, 0);
            break;
        case 4:
            Fire_Rocket(arg0, 1);
            break;
        case 3:
            if (Fire_Tazer(arg0, 0) != 0) {
                SetHeroAction(arg0, 0x3F);
                return 1;
            }
            break;
        }
    }
    return 0;
}

void SetHeroPogo(u32 arg0) {
    PogoPlayer *slot;

    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_move->jump_ctrl &= 0xFE;
    slot = &PogoPlay[arg0];
    *(s16 *)((u8 *)slot + 0x16) = 0;
    slot->forward_quick = (u16)slot->status == 5;
    Set_Tile_Under_Player(arg0, 0);
}

void PogoHeroChooseWinner(s32 arg0, u16 arg1) {
    u8 *g = GameInfo;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            PogoWinner_Crystal(arg0, (u16)arg1);
        } else if (f & 0x20) {
            PogoWinner_Gem(arg0, (u16)arg1);
        } else if (f & 0x40) {
            PogoWinner_Relic(arg0, (u16)arg1);
        } else {
            PogoWinner_Normal(arg0, (u16)arg1);
        }
    }
}

void PogoQuality_Update(void) {
    s32 i;
    u8 *p;

    for (i = 0; i < 4; i++) {
        p = GameInfo + i;
        if (p[0x5C] != 0) {
            PogoLevelIndex = AiPlayerSkill[i].level;
        }
        if (p[0x5C] == 0) {
            PogoAiQualityIndex = AiPlayerSkill[i].quality;
        }
    }
}

void PogoInit_TrophyChallenge(void) {
    if (!(GameLevelFlags & 0xE0)) {
        Set_Clock(0x130, 0x81D0);
    }
}

void PogoInit_GemChallenge(void) {
    if (GameLevelFlags & 0x20) {
        Set_Clock(0x130, 0x81D0);
    }
}

void PogoInit_CrystalChallenge(void) {
    u8 *g = GameInfo;
    s32 i;

    if (*(u16 *)(g + 0x32) & 0x80) {
        if (*(s16 *)(g + 0x34) == 10) {
            MushroomCount = 0;
            for (i = 0; i < 2; i++) {
                CreateNAS(0, 0, 0, 0x1407, 0x11);
            }
        }
        Set_Clock(0x130, 0x81D0);
    }
}

void PogoInit_AllChallenges(void) {
    s32 i;
    s32 n;

    n = 0;
    switch (GameMode) {
    case 10:
        n = 2;
        break;
    case 11:
        PogoKerb_Init();
        break;
    case 12:
        n = 3;
        CreateNAS(0, 0, 0, 0x1406, 0x11);
        break;
    case 13:
        n = 2;
        Create_PogoPadlock(4, 4);
        break;
    }
    if (n != 0) {
        for (i = 0; i < n; i++) {
            CreateNAS(0, 0, 0, 0x1403, 0x11);
        }
    }
}

void PogoInit_Camera(void) {
    MainCamera.eye.x = 0;
    MainCamera.eye.z = 0xE98;
    MainCamera.xform.angle.vx = 0x1BE;
    MainCamera.dist = 0x200;
    MainCamera.eye.y = 0;
    MainCamera.xform.angle.vy = 0;
    MainCamera.xform.angle.vz = 0;
    MainCamera.at.x = 0;
    MainCamera.at.y = 0;
    MainCamera.at.z = -0x650;
}

void PogoInit_SetClock(u32 arg0) {
    Set_Clock(arg0, 0x81D0);
}

void InitialisePOGO(void) {
    Init_Clock(0x140, 0x28, 0);
    PogoPadlockNas = 0;
    PogoRooNas = 0;
    PogoUnused1 = 0;
    BrioSmokeFrame = 0;
    CortexSmokeFrame = 0;
    World_floor_level = -0x10;
    number_of_pickups = 0;
    number_of_powerups = 0;
    PogoUnused0 = 0;
    PogoLevelIndex = 0x10;
    PogoAiQualityIndex = 0x10;
    PogoQuality_Init();
    PogoInit_Camera();
    InitPogo_Tiles();
    PogoInit_AllChallenges();
    PogoInit_AllHeros();
    PogoInit_CrystalChallenge();
    PogoInit_GemChallenge();
    PogoInit_RelicChallenge();
    PogoInit_TrophyChallenge();
}

void POGO_InitHero(u32 arg0) {
    PogoPlayer *base = PogoPlay;
    PogoPlayer *p = (PogoPlayer *)(arg0 * sizeof(PogoPlayer) + (u32)base);

    p->flags = 0;
    p->status = 0;
    p->tazer = NULL;
    PogoBounceCount[arg0] = 5;
    SetHeroAction(arg0, 0x39);
    PogoAI_InitHero(arg0);
}

void PogoCreateShadow(u32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    if (s != NULL) {
        s->sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        s->sub->entity = 0x2029;
        s->unk10 = h->hero_eng->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x800;
        s->sub->flags |= 0x40000000;
    }
}

void FirstTriggersPOGO(void) {
}

void POGOriggers(NASTrigger *trig, s32 arg1) {
}

void InitialisePOGOFirst(void) {
    HeroHooks.InitHero = (s32)POGO_InitHero;
    HeroHooks.TestJump = &HeroNoJump;
    HeroHooks.HeroModels = &PogoHeroModels;
    HeroHooks.RunSpeeds = &PogoHookTableC;
    HeroHooks.PreMoveHero = PreMovePOGO;
    HeroHooks.PostMoveHero = PostMovePOGO;
    HeroHooks.ChooseWinner = PogoHeroChooseWinner;
    HeroHooks.HeroExtraEffects = PogoHero_ExtraEffects;
    HeroHooks.CreateShadow = PogoCreateShadow;
    HeroHooks.HeroBot = PogoAI_Poll;
    PogoInit_AIQuality();
}

void POGOSetUp(void) {
    LevelSpecs.LevelPreInit = InitialisePOGOFirst;
    LevelSpecs.LevelInit = InitialisePOGO;
    LevelSpecs.LevelRestartInit = InitialisePOGO;
    LevelSpecs.LevelRoutine = UpdatePOGO;
    LevelSpecs.EarliestTriggers = FirstTriggersPOGO;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    LevelSpecs.GetContour = GetPogoContour;
    process_control_init(&MenuProcessControl, &PogoProcess);
}

void pogo_kill(void) {
    primpool_free();
}

void pogo_init(void) {
    primpool_init(0x80);
}

void psxpogo_init_tile_heap(s32 arg0) {
    primpool_init(arg0 * 2);
}

void psxpogo_init_tile(subengine *arg0) {
    (*(s32*)((u8*)(arg0) + 84)) = psxpogo_render_tile;
    (*(s32*)((u8*)(arg0) + 88)) = kill_sprim_sub;
    (*(s32*)((u8*)(arg0) + 0)) = (s32) ((*(s32*)((u8*)(arg0) + 0)) | 0x01000000);
}

void psxpogo_render_tile(ModelObj *obj, void *arg1, s32 flags) {
    POLY_GT4 *p0;
    POLY_GT4 *p1;
    GfxTexInfo *ti;

    draw_model_quad(obj, arg1, flags);
    p0 = obj->slots[0].prim;
    p1 = obj->slots[1].prim;
    if (p0 != NULL && p1 != NULL) {
        if (obj->unk8E != 0) {
            ti = &obj->model->texs.tex[obj->unk8E & 0xFFF].info;
            *(u16 *)&p0->u0 = ti->uv;
            *(u16 *)&p0->u1 = ti->uv1;
            *(u16 *)&p0->u2 = ti->uv2;
            *(u16 *)&p0->u3 = ti->uv3;
            p0->tpage = ti->tpage;
            *(u16 *)&p1->u0 = ti->uv;
            *(u16 *)&p1->u1 = ti->uv1;
            *(u16 *)&p1->u2 = ti->uv2;
            *(u16 *)&p1->u3 = ti->uv3;
            p1->tpage = ti->tpage;
            obj->unk8E = 0;
        }
        if (obj->unk8C != 0) {
            p0->clut = obj->unk8C;
            p1->clut = obj->unk8C;
            obj->unk8C = 0;
        }
    }
}
