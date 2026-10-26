#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl69.h"
#include "bash/titles.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_8004ED60[];
extern u8 D_8004ED68[];
extern u8 D_8004ED70[];
extern u8 D_8004ED78[];
extern u8 D_800500B8[];
extern u8 D_80055614[];
extern u8 D_8005564C[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];
extern u8 D_8009EB84[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char D_800B32B8[] = "complete each warp room to\nunlock further levels";
const char D_800B32EC[] = "select a battle\nfrom unlocked levels";
const char D_800B3314[] = "play a tournament";
const char D_800B3328[] = "change the options";
const char D_800B333C[] = "ADVENTURE MODE";
const char D_800B334C[] = "BATTLE MODE";
const char D_800B3358[] = "TOURNAMENT";
const char D_800B3364[] = "OPTIONS";
const char D_800B336C[] = "";
const char D_800B3370[] = "SELECT GAME TYPE";
const char D_800B3384[] = "1 PLAYER";
const char D_800B3390[] = "2 PLAYER";
const char D_800B339C[] = "3 PLAYER";
const char D_800B33A8[] = "4 PLAYER";
const char D_800B33B4[] = "SELECT NUMBER OF PLAYERS";
const char D_800B33D0[] = "NEW GAME";
const char D_800B33DC[] = "LOAD GAME";
const char D_800B33E8[] = "5  X";
const char D_800B33F0[] = "PRESS LEFT - RIGHT TO\nSELECT CUPS NEEDED\nTO WIN ROUND";
const char D_800B3428[] = "PRESS UP - DOWN TO\nSELECT DIFFICULTY";
const char D_800B3450[] = "BEGIN BATTLE";
const char D_800B3460[] = "GAME OPTIONS";
const char D_800B3470[] = "BREATHE";
const char D_800B3478[] = "TAUNT_A";
const char D_800B3480[] = "SLIDE";
const char D_800B3488[] = "WIN";
const char D_800B348C[] = "WIN2";
/* the backdrop's position (AnimateTitlesBackdrop), then the menus' texts */
const SLONG_XYZ D_800B3494 = { -0x17C, 0x190, -0x64 };
const char D_800B34A0[] = "EASY";
const char D_800B34A8[] = "MEDIUM";
const char D_800B34B0[] = "HARD";
const char D_800B34B8[] = "PRESS \xFC BUTTON TO START";
const char D_800B34D0[] = "EXIT";
const char D_800B34D8[] = "SOUND";
const char D_800B34E0[] = "AUDIO MODE";
const char D_800B34EC[] = "<STEREO>";
const char D_800B34F8[] = "< MONO >";
const char D_800B3504[] = "MUSIC VOLUME";
const char D_800B3514[] = "FX VOLUME";
const char D_800B3520[] = "CONTROLLER";
const char D_800B352C[] = "VIBRATION";
const char D_800B3538[] = "ON";
const char D_800B353C[] = "OFF";
const char D_800B3540[] = "PRESS \xFF BUTTON TO CHANGE";
const char D_800B355C[] = "PRESS \xFC BUTTON TO ACCEPT";
const char D_800B3578[] = "<100%>";
const char D_800B3580[] = "<090%>";
const char D_800B3588[] = "P1";
const char D_800B358C[] = "P2";
const char D_800B3590[] = "P3";
const char D_800B3594[] = "P4";
const char D_800B3598[20] = "CHARACTER SELECT\x00\x14";
const char D_800B35AC[] = "PRESS \xFD BUTTON\r14TO UNLOCK LEVELS";
const char D_800B35D0[] = "THERE ARE NO MORE\nUNLOCKED LEVELS\nON THIS MEMORY CARD\n";
const char D_800B3608[] = "INSERT MEMORY CARD\nWITH CRASH BASH DATA\nTO CHECK FOR\nUNLOCKED LEVELS";
const char D_800B3650[] = "LOCKED";
const char D_800B3658[] = "PRESS LEFT - RIGHT TO CHANGE ARENA";
const char D_800B367C[] = "PRESS UP - DOWN TO CHANGE LEVEL";
const char D_800B369C[] = "PRESS \xFC BUTTON TO SELECT";
const char D_800B36B8[] = "CHOOSE LEVEL";
const char D_800B36C8[] = "PRESS \xFF TO\nCHECK MEMORY CARD\nFOR SAVED GAME";
const char D_800B36F4[64] = "CHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nFOR UNLOCKED\nLEVELS\x00\x14";
const char D_800B3734[] = "SELECT BATTLE TYPE";
const char D_800B3748[] = "PRESS LEFT - RIGHT TO CHANGE BATTLE TYPE";
const char D_800B3774[] = "PRESS \xFC BUTTON TO START BATTLE";
const char D_800B3794[] = "VS";
const char D_800B3798[] = "VS BATTLE";
const char D_800B37A4[] = "TEAM BATTLE";
const char D_800B37B0[] = "SELECT SLOT";

/* data, in address order */
s32 D_800B8508[4] = { (s32)&D_800B32B8, (s32)&D_800B32EC, (s32)&D_800B3314, (s32)&D_800B3328 };
PlaceRec D_800B8518[9] = {
    { 4, -0x20, 0x12C, -0x46, 0x72, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x2C, 0, 0, 0, 0, 0, 0, (s32)&D_800B333C, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0xA, 0, 0, 0, 0, 0, 0, (s32)&D_800B334C, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x18, 0, 0, 0, 0, 0, 0, (s32)&D_800B3358, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x3A, 0, 0, 0, 0, 0, 0, (s32)&D_800B3364, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x82, 0, 0, 0, 0, 0, 0, (s32)&D_800B336C, 4, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B3370, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B865C[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x42, 0, 0, 0, 0, 0, 0, (s32)&D_800B3384, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x1C, 0, 0, 0, 0, 0, 0, (s32)&D_800B3390, 0, 0, 0, 0 },
    { 3, -0x7F7A, 8, 0, 0, 0, 0, 0, 0, (s32)&D_800B339C, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x2C, 0, 0, 0, 0, 0, 0, (s32)&D_800B33A8, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B33B4, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B877C[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, (s32)&D_800B3384, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B3390, 0, 0, 0, 0 },
    { 3, 0x3A, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x3A, 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B33B4, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B889C[6] = {
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B333C, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x20, 0x12C, -0x46, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, (s32)&D_800B33D0, 2, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B33DC, 3, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B8974[12] = {
    { 4, -0x8000, 0x190, -0x68, -0xA, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xA, 0xB4, -0x14, 0, 0xCCC, 0, 0x28, 0, 0, 0, 0, 0xC7, 0x2009, 2 },
    { 3, 0x3C, -0x46, 0, 0, 0, 0, 0, 0, (s32)&D_800B33E8, 0, 0, 0, 0 },
    { 3, -0xDC, -0x55, 0, 0, 0, 0, 0, 0, (s32)&D_800B33F0, 4, 0, 0, 0 },
    { 4, -0x8000, 0x190, 0x28, 0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x46, 0x37, 0, 0, 0, 0, 0, 0, (s32)&D_800B336C, 0, 0, 0, 0 },
    { 3, -0xDC, 0x32, 0, 0, 0, 0, 0, 0, (s32)&D_800B3428, 4, 0, 0, 0 },
    { 4, -0x8000, 0x258, 0x9A, 0xC0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0xA5, 0, 0, 0, 0, 0, 0, (s32)&D_800B3450, 4, 0, 0, 0 },
    { 4, -0x15E, 0x258, -0xD0, -0xA6, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0xCA, 0, 0, 0, 0, 0, 0, (s32)&D_800B3460, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 D_800B8B24[11] = {
    (s32)&D_80055684, (s32)&D_8004ED78, (s32)&D_8004ED70, (s32)&D_80053074, (s32)&D_8004ED68,
    (s32)&D_8004ED60, 0, 0, 0, (s32)&D_800500B8, (s32)&D_800B8B24,
};
s32 D_800B8B50[1] = { 0xFFFF };
s32 D_800B8B54[6] = { (s32)&D_800B3470, (s32)&D_800B3478, (s32)&D_800B3480, (s32)&D_800B3488, (s32)&D_800B348C, 0 };
s32 D_800B8B6C[3] = { 0xC800C7, 0x1C00C9, 0xFFFF };
s32 D_800B8B78[34] = {
    0xC5, (s32)&D_800B8B54, (s32)&D_800B8B6C, 3, 0, 0, 0, 0, 0xC5, (s32)&D_800B8B54,
    (s32)&D_800B8B6C, 3, 0, 0, 0, 0, 0xC5, (s32)&D_800B8B54, (s32)&D_800B8B6C, 3, 0, 0, 0, 0,
    0x1800, 0x10, 0x100, 0x40, 0x100, 0x40, (s32)&D_80063EC8, (s32)&D_80063BB8, 0x20, 0x76C,
};
s32 D_800B8C00[2] = { 0x20D0003, 0x10021001 };
s32 D_800B8C08[9] = { (s32)&D_800B8C00, 4, (s32)&D_800B8B50, (s32)&D_800B8B78, (s32)&D_800B8C08, 0, 0, 0, 0 };
Ext400 D_800B8C2C = {
    {
        (s32)0x80110000, 0x40000016, 0x168011, (s32)0x80114000, 0x40000016, 0x168011,
        (s32)0x80114000, 0x40800038, 0x168011, (s32)0x80114000, 0x40000016,
    },
};
s32 D_800B8C58[11] = {
    0x388011, (s32)0x80114100, 0x41800054, 0x168011, (s32)0x80114000, 0x40000016, 0x388011,
    (s32)0x80004100, 0, (s32)&D_800B3470, 0,
};
PlaceRec D_800B8C84[9] = {
    { 6, -0x64, 0, 0x4C8, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x2007, 0 },
    { 7, -0x17C, 0x190, -0x64, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 7, -0x17C, 0x190, -0x64, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x2001, 0 },
    { 4, -0x118, 0x5A, -2, 0xC0, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x8C, 0x11C, -0xCC, -0x6C, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x8C, 0x11C, -0x68, -8, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x8C, 0x11C, -4, 0x5C, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x8C, 0x11C, 0x60, 0xC0, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
u16 D_800B8DC8[8] = { 0xA, 0, 0xE, 0, 0xA, 0, 0xE, 0 };
GfxViewport D_800B8DD8 = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x10, 0x1EF, 0, 0, 0, 0, 0 };
GfxViewport D_800B8E00 = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x800, 0x7FF, -0x80, 0, 0, 0, 1 };
process_info D_800B8E28 = { MainMenuInit, MainMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info D_800B8E3C = { PlayersMenuInit, PlayersMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info D_800B8E50 = { NewLoadMenuInit, NewLoadMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info D_800B8E64 = { TitlesLeaveInit, 0, TitlesLeaveRender, 0, 0 };
s32 D_800B8E78[1][3] = {
    {
        (s32)&D_800B34A0, (s32)&D_800B34A8,
        (s32)&D_800B34B0,
    },
};
s32 D_800B8E84[1][2] = { { (s32)&D_800B34B8, (s32)&D_800B34B8 } };
process_info D_800B8E8C = { BattleOptionsInit, BattleOptionsUpdate, BattleOptionsRender, BattleOptionsQuit, 0 };
s32 D_800B8EA0[1] = { (s32)&D_800B8E28 };
s32 D_800B8EA4[2] = { (s32)&D_800B8E3C, (s32)&D_800B9DF4 };
s32 D_800B8EAC[3] = { (s32)&D_800BA72C, (s32)&D_800BAAB4, (s32)&D_800B8E8C };
s32 *D_800B8EB8 = (void *)&D_800B8EAC;
s32 D_800B8EBC[1] = { (s32)&D_800B8E28 };
s32 D_800B8EC0[5] = { (s32)&D_800B8E50, (s32)&D_800B8E3C, (s32)&D_800B9DF4, (s32)&D_800B8E64, (s32)&D_800B8E28 };
s32 D_800B8ED4[2] = { (s32)&D_800B8E3C, (s32)&D_800B9DF4 };
s32 D_800B8EDC[2] = { (s32)&D_8009EB84, (s32)&D_800B8E8C };
s32 *D_800B8EE4 = (void *)&D_800B8EDC;
s32 D_800B8EE8[1] = { (s32)&D_800B3364 };
s32 D_800B8EEC[1] = { (s32)&D_800B34D0 };
s32 D_800B8EF0[1] = { (s32)&D_800B34D8 };
s32 D_800B8EF4[1] = { (s32)&D_800B34E0 };
s32 D_800B8EF8[1] = { (s32)&D_800B34EC };
s32 D_800B8EFC[1] = { (s32)&D_800B34F8 };
s32 D_800B8F00[1] = { (s32)&D_800B3504 };
s32 D_800B8F04[1] = { (s32)&D_800B3514 };
s32 D_800B8F08[1] = { (s32)&D_800B3520 };
s32 D_800B8F0C[1] = { (s32)&D_800B352C };
s32 D_800B8F10[1] = { (s32)&D_800B3538 };
s32 D_800B8F14[1] = { (s32)&D_800B353C };
s32 D_800B8F18[1] = { (s32)&D_800B3540 };
s32 D_800B8F1C[1] = { (s32)&D_800B355C };
PlaceRec D_800B8F20[7] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EE8, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x5A, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x32, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EF0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xA, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F08, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x1E, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EEC, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B901C[12] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EF0, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xFA, 0xFA, -0x40, 0x8C, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0xD2, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EF4, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EF8, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&D_800B8EFC, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0xA, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F00, 0, 0, 0, 0 },
    { 3, 0x46, 0xA, 0, 0, 0, 0, 0, 0, (s32)&D_800B3578, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0x32, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F04, 0, 0, 0, 0 },
    { 3, 0x46, 0x32, 0, 0, 0, 0, 0, 0, (s32)&D_800B3580, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F1C, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B91CC[23] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F08, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x82, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x3C, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F0C, 0, 0, 0, 0 },
    { 0xE, -0x8F, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F10, 7, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 0, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 1, 0, 0, 0 },
    { 0xE, -0x35, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F10, 7, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 0, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 1, 0, 0, 0 },
    { 0xE, 0x25, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F10, 7, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 0, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 1, 0, 0, 0 },
    { 0xE, 0x75, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F10, 7, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 0, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F14, 1, 0, 0, 0 },
    { 3, -0x87, -0xF, 0, 0, 0, 0, 0, 0, (s32)&D_800B3588, 4, 0, 0, 0 },
    { 3, -0x2D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&D_800B358C, 4, 0, 0, 0 },
    { 3, 0x2D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&D_800B3590, 4, 0, 0, 0 },
    { 3, 0x7D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&D_800B3594, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x3C, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F18, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, (s32)&D_800B8F1C, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 D_800B9508 = 0;
s32 D_800B950C = 0;
process_info D_800B9510 = { OptionsInit, OptionsUpdate, OptionsRender, OptionsQuit, 0 };
process_info D_800B9524 = { RunGameInit, RunGameUpdate, RunGameRender, RunGameQuit, 0 };
GfxCamera D_800B9538 = {0};
s32 D_800B95F0 = 0;
Ext400 D_800B95F4 = {0};
s32 *D_800B9620 = 0;
s32 D_800B9624 = 0;
s32 D_800B9628 = 0;
s32 D_800B962C = 0;
s32 D_800B9630[1] = { (s32)&D_800B3598 };
PlaceRec D_800B9634[27] = {
    { 0x11, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3075, 0 },
    { 0x11, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3077, 0 },
    { 0x11, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3079, 0 },
    { 0x11, 0, 0, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3065, 0 },
    { 0xF, 0x96, 0x26, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x306B, 0 },
    { 0xF, 0xF0, 0x26, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3067, 0 },
    { 0xF, 0x14A, 0x26, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3073, 0 },
    { 0xF, 0x1A4, 0x26, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x306D, 0 },
    { 0xF, 0x96, 0x8A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3069, 0 },
    { 0xF, 0xF0, 0x8A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x307B, 0 },
    { 0xF, 0x14A, 0x8A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x3071, 0 },
    { 0xF, 0x1A4, 0x8A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0x306F, 0 },
    { 0x10, 0x8E, 0xE4, 0x20, 0x76, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0xE8, 0x13E, 0x20, 0x76, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0x142, 0x198, 0x20, 0x76, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0x19C, 0x1F2, 0x20, 0x76, 9, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0x8E, 0xE4, 0x84, 0xDA, 0xA, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0xE8, 0x13E, 0x84, 0xDA, 0xA, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0x142, 0x198, 0x84, 0xDA, 0xA, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0x10, 0x19C, 0x1F2, 0x84, 0xDA, 0xA, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x116, 0, 0, 0, 0, 0, 0, (s32)&D_800B9630, 0, 0, 0, 0 },
    { 4, -0xA, 0x28A, 0x112, 0x136, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0, 0x2BC, 0x28, 0x6E, 7, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0, 0x2BC, 0x8C, 0xD2, 8, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 0x50, 0x6E, 0, 0x4CC, 0, 0, 0, 0, 0, 0, 0xC7, 0x200B, 0 },
    { 6, 0x230, 0xBE, 0, 0x4CC, 0, 0, 0, 0, 0, 0, 0xC7, 0x200C, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800B9A00[9] = {
    { 7, -0x1FE, 0x1A4, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, -0xAA, 0x1A4, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 0xAA, 0x1A4, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, 0x1FE, 0x1A4, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 7, -0x1FE, 0x1A4, 0, 0x800, 0, 0, 0, 0, 0, 0, 0xC7, 0x2002, 0 },
    { 7, -0xAA, 0x1A4, 0, 0x800, 0, 0, 0, 0, 0, 0, 0xC7, 0x2003, 0 },
    { 7, 0xAA, 0x1A4, 0, 0x800, 0, 0, 0, 0, 0, 0, 0xC7, 0x2004, 0 },
    { 7, 0x1FE, 0x1A4, 0, 0x800, 0, 0, 0, 0, 0, 0, 0xC7, 0x2005, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
Ovl69Rec D_800B9B44[8] = {
    {
        0,
        {
            1, 0, 0, 0, 0x8E, 0, 0, 0, 0x20, 0, 0, 0, 0xCC, 0xCC, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0x50, 0x9E, 0xB, 0x80, 0x8E, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            2, 0, 0, 0, 0xE8, 0, 0, 0, 0x20, 0, 0, 0, 0xCC, 0xCC, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0x2C, 0x9E, 0xB, 0x80, 0x8F, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            5, 0, 0, 0, 0x42, 1, 0, 0, 0x20, 0, 0, 0, 0x99, 0x99, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0xE0, 0x9E, 0xB, 0x80, 0x94, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            7, 0, 0, 0, 0x9C, 1, 0, 0, 0x20, 0, 0, 0, 0x99, 0x99, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0x98, 0x9E, 0xB, 0x80, 0x92, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            3, 0, 0, 0, 0x8E, 0, 0, 0, 0x84, 0, 0, 0, 0xCC, 0xCC, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0x74, 0x9E, 0xB, 0x80, 0x91, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            4, 0, 0, 0, 0xE8, 0, 0, 0, 0x84, 0, 0, 0, 0x99, 0x99, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 8, 0x9E, 0xB, 0x80, 0x90, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            6, 0, 0, 0, 0x42, 1, 0, 0, 0x84, 0, 0, 0, 0x99, 0x99, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 4, 0x9F, 0xB, 0x80, 0x95, 3, 0, 0,
        },
        0,
    },
    {
        0,
        {
            8, 0, 0, 0, 0x9C, 1, 0, 0, 0x84, 0, 0, 0, 0x99, 0x99, 0, 0, 0xC7, 0, 0, 0, 0, 0, 0,
            0, 0xBC, 0x9E, 0xB, 0x80, 0x93, 3, 0, 0,
        },
        0,
    },
};
Ovl69Off D_800B9C84[4] = { { 7, 4 }, { 0x2D, 4 }, { 7, 0x30 }, { 0x2D, 0x30 } };
s32 D_800B9CA4[4] = { 0x389, 0x38A, 0x38B, 0x38C };
s32 D_800B9CB4[4] = { 0x385, 0x386, 0x387, 0x388 };
s8 D_800B9CC4[48] = {
    3, 4, 6, 6, 8, 8, 3, 4, 6, 6, 8, 8, 1, 2, 5, 5, 7, 7, 1, 2, 5, 5, 7, 7, 3, 4, 6, 6, 8, 8,
    1, 2, 5, 5, 7, 7, 3, 4, 6, 6, 8, 8, 1, 2, 5, 5, 7, 7,
};
s8 D_800B9CF4[256] = {
    0, 0, 0, 0, 3, 4, 6, 8, 7, 7, 6, 6, 7, 7, 6, 6, 3, 4, 8, 8, 7, 7, 3, 4, 3, 4, 6, 6, 5, 5,
    3, 4, 3, 4, 6, 8, 0, 0, 0, 0, 7, 7, 6, 6, 7, 7, 6, 6, 3, 4, 8, 8, 7, 7, 3, 4, 3, 4, 6, 6,
    5, 5, 3, 4, 7, 7, 6, 6, 7, 7, 6, 6, 0, 0, 0, 0, 1, 2, 5, 7, 1, 2, 8, 8, 1, 2, 7, 7, 1, 2,
    6, 6, 1, 2, 5, 5, 7, 7, 6, 6, 7, 7, 6, 6, 1, 2, 5, 7, 0, 0, 0, 0, 1, 2, 8, 8, 1, 2, 7, 7,
    1, 2, 6, 6, 1, 2, 5, 5, 3, 4, 8, 8, 3, 4, 8, 8, 1, 2, 8, 8, 1, 2, 8, 8, 0, 0, 0, 0, 1, 2,
    3, 4, 3, 4, 6, 8, 1, 2, 3, 4, 7, 7, 3, 4, 7, 7, 3, 4, 1, 2, 7, 7, 1, 2, 7, 7, 1, 2, 3, 4,
    0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 5, 7, 3, 4, 6, 6, 3, 4, 6, 6, 1, 2, 6, 6, 1, 2, 6, 6, 3, 4,
    6, 8, 1, 2, 3, 4, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 3, 4, 5, 5, 3, 4, 1, 2, 5, 5, 1, 2, 5, 5,
    1, 2, 3, 4, 1, 2, 5, 7, 1, 2, 3, 4, 0, 0, 0, 0,
};
process_info D_800B9DF4 = { CharSelectInit, CharSelectUpdate, CharSelectRender, 0, 0 };
s32 D_800B9E08[3] = { (s32)0x80110000, 0x43800020, 0x8000 };
s32 D_800B9E14[6] = { (s32)&D_800B9E08, 0, 0, 0, (s32)&D_800B9E14, 0x401 };
s32 D_800B9E2C[3] = { (s32)0x80110000, 0x42000014, 0x8000 };
s32 D_800B9E38[6] = { (s32)&D_800B9E2C, 0, 0, 0, (s32)&D_800B9E38, 0x401 };
s32 D_800B9E50[3] = { (s32)0x80110000, 0x42800016, 0x8000 };
s32 D_800B9E5C[6] = { (s32)&D_800B9E50, 0, 0, 0, (s32)&D_800B9E5C, 0x401 };
s32 D_800B9E74[3] = { (s32)0x80110000, 0x43000020, 0x8000 };
s32 D_800B9E80[6] = { (s32)&D_800B9E74, 0, 0, 0, (s32)&D_800B9E80, 0x401 };
s32 D_800B9E98[3] = { (s32)0x80110000, 0x44000016, 0x8000 };
s32 D_800B9EA4[6] = { (s32)&D_800B9E98, 0, 0, 0, (s32)&D_800B9EA4, 0x401 };
s32 D_800B9EBC[3] = { (s32)0x80110000, 0x44800014, 0x8000 };
s32 D_800B9EC8[6] = { (s32)&D_800B9EBC, 0, 0, 0, (s32)&D_800B9EC8, 0x401 };
s32 D_800B9EE0[3] = { (s32)0x80110000, 0x45000020, 0x8000 };
s32 D_800B9EEC[6] = { (s32)&D_800B9EE0, 0, 0, 0, (s32)&D_800B9EEC, 0x401 };
s32 D_800B9F04[6] = { (s32)0x80110000, 0x45800020, 0x8000, (s32)&D_800B9F04, 0, 0 };
GfxCamera D_800B9F1C = {0};
Ovl69Slot D_800B9FD4[4] = {0};
s32 D_800BA154[1] = { (s32)&D_800B35AC };
s32 D_800BA158[1] = { (s32)&D_800B35D0 };
s32 D_800BA15C[1] = { (s32)&D_800B3608 };
s32 D_800BA160[1] = { (s32)&D_800B3650 };
s32 D_800BA164[16] = {
    1, 0, (s32)&D_80055614, 0x2005, 1, 1, (s32)&D_80055614, 0x2006, 1, 2, (s32)&D_80055614,
    0x2007, 1, 3, (s32)&D_80055614, 0x2008,
};
s32 D_800BA1A4[16] = {
    1, 0x13, (s32)&D_8005564C, 0x2005, 1, 0x12, (s32)&D_8005564C, 0x2006, 1, 0x14,
    (s32)&D_8005564C, 0x2007, 1, 0x15, (s32)&D_8005564C, 0x2008,
};
s32 D_800BA1E4[16] = {
    1, 0xA, (s32)&D_8005564C, 0x2001, 1, 0xB, (s32)&D_8005564C, 0x2002, 1, 0xC,
    (s32)&D_8005564C, 0x2003, 1, 0xD, (s32)&D_8005564C, 0x2004,
};
s32 D_800BA224[16] = {
    1, 0xE, (s32)&D_80055614, 0x2001, 1, 0xF, (s32)&D_80055614, 0x2002, 1, 0x10,
    (s32)&D_80055614, 0x2003, 1, 0x11, (s32)&D_80055614, 0x2004,
};
s32 D_800BA264[16] = {
    2, 6, (s32)&D_8005564C, 0x2009, 2, 8, (s32)&D_8005564C, 0x200A, 2, 7, (s32)&D_8005564C,
    0x200B, 2, 5, (s32)&D_8005564C, 0x200C,
};
s32 D_800BA2A4[16] = {
    2, 0x17, (s32)&D_80055614, 0x2009, 2, 0x18, (s32)&D_80055614, 0x200A, 2, 0x1A,
    (s32)&D_80055614, 0x200B, 2, 0x19, (s32)&D_80055614, 0x200C,
};
s32 D_800BA2E4[16] = {
    2, 0x1B, (s32)&D_80055614, 0x200D, 2, 0x1C, (s32)&D_80055614, 0x200E, 2, 0x1D,
    (s32)&D_80055614, 0x200F, 2, 0x1E, (s32)&D_80055614, 0x2010,
};
Ovl69Entry D_800BA324[7] = {
    { (s32)&D_800BA164, 4 }, { (s32)&D_800BA1A4, 4 }, { (s32)&D_800BA1E4, 4 },
    { (s32)&D_800BA224, 4 }, { (s32)&D_800BA264, 4 }, { (s32)&D_800BA2A4, 4 },
    { (s32)&D_800BA2E4, 4 },
};
PlaceRec D_800BA35C[2] = {
    { 7, 0, 0, 0, 0xB33, 0, 0, 0, 0, 0, 0, 0xC8, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800BA3A4[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2005, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800BA3EC[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2006, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800BA434[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2007, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec D_800BA47C[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2008, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 D_800BA4C4[1] = { (s32)&D_800B3658 };
s32 D_800BA4C8[1] = { (s32)&D_800B367C };
s32 D_800BA4CC[1] = { (s32)&D_800B369C };
s32 D_800BA4D0[1] = { (s32)&D_800B36B8 };
PlaceRec D_800BA4D4[11] = {
    { 3, 0x7FA1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 0xD4, 0, 0, 0x266, 0, 0, 0, 0, 0, 0, 0xC7, 0x2008, 0 },
    { 0xE, 0x7FA1, -0x96, 0, 0, 0, 0, 0, 0, (s32)&D_800BA4C4, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x42, 0, 0, 0, 0, 0, 0, (s32)&D_800BA4C8, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x28, 0, 0, 0, 0, 0, 0, (s32)&D_800BA4CC, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0xC8, 0, 0, 0, 0, 0, 0, (s32)&D_800BA4D0, 0, 0, 0, 0 },
    { 3, 0x7FA1, -0x6E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x14A, 0x88, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x120, 0x28A, -0xCC, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x7FA1, 0x4E, 0, 0, 0, 0, 0, 0, (s32)&D_800B36C8, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 D_800BA660[1] = { (s32)&D_800B36F4 };
GfxViewport D_800BA664 = { 0x28, 0xEE, 0x19A, 0x1B0, 0, 0, 0x280, 0x1E0, 0xE1, 0x198, 0x200, 0x1FF, 0, 0, 0, 0, 0 };
GfxViewport D_800BA68C = { 0x1CC, 0x24, 0x25C, 0x84, 0, 0, 0x280, 0x1E0, 0x214, 0x88, 0x400, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport D_800BA6B4 = { 0x1CC, 0x88, 0x25C, 0xE8, 0, 0, 0x280, 0x1E0, 0x214, 0xEC, 0x4CC, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport D_800BA6DC = { 0x1CC, 0xEC, 0x25C, 0x14C, 0, 0, 0x280, 0x1E0, 0x214, 0x150, 0x600, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport D_800BA704 = { 0x1CC, 0x150, 0x25C, 0x1B0, 0, 0, 0x280, 0x1E0, 0x214, 0x1B4, 0x700, 0xFF, 0, 0, 0, 0, 0 };
process_info D_800BA72C = { LevelSelectInit, LevelSelectUpdate, LevelSelectRender, LevelSelectQuit, 0 };
s32 D_800BA740 = 0;
s32 D_800BA744 = 0;
s32 D_800BA748[1] = { (s32)&D_800B3734 };
s32 D_800BA74C[1] = { (s32)&D_800B3748 };
s32 D_800BA750[1] = { (s32)&D_800B3774 };
PlaceRec D_800BA754[19] = {
    { 6, -0xFA, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, -0x6A, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0x38, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0xDC, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 4, -0x122, -0xAA, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x88, -0x10, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x10, 0x88, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0xAA, 0x122, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0xAA, -0x46, 0, 0, 0, 0, 0, 0, (s32)&D_800B3794, 0, 0, 0, 0 },
    { 3, -0x8000, -0x46, 0, 0, 0, 0, 0, 0, (s32)&D_800B3794, 0, 0, 0, 0 },
    { 3, 0x88, -0x46, 0, 0, 0, 0, 0, 0, (s32)&D_800B3794, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xC8, 0, 0, 0, 0, 0, 0, (s32)&D_800BA748, 0, 0, 0, 0 },
    { 4, -0x8000, 0x258, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0x42, 0, 0, 0, 0, 0, 0, (s32)&D_800B3798, 2, 0, 0, 0 },
    { 4, -0x78, 0x78, 0x3A, 0x6C, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x8C, 0, 0, 0, 0, 0, 0, (s32)&D_800BA74C, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0xA6, 0, 0, 0, 0, 0, 0, (s32)&D_800BA750, 4, 0, 0, 0 },
    { 4, -0x10E, 0x10E, 0x87, 0xBE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 D_800BAA00[1] = { (s32)&D_800B3798 };
s32 D_800BAA04[1] = { (s32)&D_800B37A4 };
Ovl69Preset D_800BAA08[2] = {
    {
        { 0, 1, 2, 3 }, { 0x8000, 0x8000, 0x8000 }, { -0x122, -0x88, 0x10, 0xAA },
        (void *)&D_800BAA00, 0, 0, 2, -1, -1, -1, -1,
    },
    {
        { 0, 1, 2, 3 }, { 0, 0x8000, 0 }, { -0x116, -0x96, 0x1F, 0x9E }, (void *)&D_800BAA04,
        1, 0, 2, 1, 0, 3, 2,
    },
};
u16 D_800BAAA0[10] = { 0, 0x306B, 0x3067, 0x3069, 0x307B, 0x3073, 0x3071, 0x306D, 0x306F, 0 };
process_info D_800BAAB4 = { team_mode_init, TeamModeUpdate, RenderTitlesBackdropAndMenu, 0, 0 };
s32 D_800BAAC8 = 0;
s32 D_800BAACC = 0;
PlaceRec D_800BAAD0[4] = {
    { 4, 0, 0x64, 0, 0x64, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&D_800B37B0, 2, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {
        (s32)TitlesExtraScreenInit, 0, 0, -0x7B48, -0x7FF5, 0, 0, 0, 0,
        (s32)TitlesShellScreenInit, (s32)TitlesShellScreenUpdate, -0x7B90, -0x7FF5, 0,
    },
};
s32 D_800BAB60[1] = {0};

void TitlesInitLevel(void) {
    u8 *g;
    s32 *p;
    u8 *a1;

    func_8009029C();
    g = D_8005A614;
    g[0x62] = 0;
    cam_init((u8 *)&D_800B9538);
    *(s32 *)((u8 *)&D_800B9538 + 8) = 0x400;
    *(s32 *)((u8 *)&D_800B9538 + 0x18) = 0x190;
    func_80095BEC(1, (s32 *)D_800B8C84);
    GetEngineSlot(1)->sub[4].flags &= ~0x8000;
    GetEngineSlot(1)->sub[5].flags &= ~0x8000;
    GetEngineSlot(1)->sub[6].flags &= ~0x8000;
    GetEngineSlot(1)->sub[7].flags &= ~0x8000;
    GetEngineSlot(1)->sub[3].flags &= ~0x8000;
    D_80051604 = 0;
    switch (*(s16 *)(g + 0x34)) {
    case 0x28:
        core_process = 0;
        func_80092C54();
        *(u16 *)(g + 0x32) |= 0x200;
        func_8001E934(&D_8009F8A4, &D_800B8E28);
        break;
    case 0x29:
        p = D_800B8EB8;
        g[0x27] = 4;
        a1 = (u8 *)*p;
        D_800B9620 = p;
        func_8001E934(&D_8009F8A4, (process_info *)a1);
        break;
    case 0x2A:
        p = D_800B8EE4;
        g[0x27] = 4;
        a1 = (u8 *)*p;
        D_800B9620 = p;
        func_8001E934(&D_8009F8A4, (process_info *)a1);
        break;
    }
    D_800B95F4 = D_8005A9A0;
    D_800B95F4.w[3] = (s32)&D_800B8C2C;
}

void TitlesCheckIdle(void) {
    s32 f;
    if (D_80051604 >= 0x385) {
        D_80051604 = 0;
        set_fade(&D_8009F644, 0x200, 0);
        f = D_8009F654;
        if (f & 1) {
            D_8005A648 = 0x28;
            func_80094D88((s32)D_800A0840, &D_800A00DC);
        } else {
            u8 *g = D_8005A614;
            u8 *t = (u8 *)D_800B8DC8;
            u16 v = *(u16 *)(t + ((f * 2) & 0xC));
            *(s32 *)(g + 0x68) = 1;
            *(s32 *)(g + 0x14) = 0x384;
            D_8009F658.next_process = &D_800A00DC;
            *(u16 *)(g + 0x34) = v;
        }
        D_8009F654 += 1;
    }
}


void AnimateTitlesBackdrop(void) {
    SLONG_XYZ pos;
    subengine *sub;
    subengine *p2;
    TrigEntry *tbl;
    EXTengine *eng;
    s16 ang;
    s32 d;

    sub = GetEngineSlot(1)->sub;
    sub->unk14 = D_8005B690 * 0x14;
    sub->unk12 = 0x293;
    sub->unk50 = 1;
    sub->unk51 = 0;
    sub->unk20.z = 0x2800;
    sub->unk20.x = 0x2800;
    sub->unk52 = 1;
    eng = GetEngineSlot(1);
    tbl = D_80068BD4;
    pos = D_800B3494;
    d = D_8004E0D8;
    pos.x += (tbl[(((d * 0x14) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x8F) >> 0xF;
    pos.y += (tbl[(((d * 0x2F) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x17) >> 0xC;
    ang = (tbl[(((d * 0x13) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x333) >> 0x10;
    p2 = &eng->sub[1];
    *(SLONG_XYZ *)&p2->unk04 = pos;
    p2->unk12 = -0x1C7;
    p2->unk51 = 0;
    p2->unk14 = ang;
    p2->unk52 = 1;
    func_800251A8(NULL, eng, p2, &D_800B95F4);
    p2 = &eng->sub[2];
    *(SLONG_XYZ *)&p2->unk04 = pos;
    p2->unk51 = 0;
    p2->unk12 = -0x155;
    p2->unk14 = ang;
    p2->flags |= 0x02000000;
    p2->unk68 = 0x80;
    p2->unk52 = 1;
}

void MainMenuUpdate(void) {
    subengine *s;
    u8 *g;

    AnimateTitlesBackdrop();
    TitlesCheckIdle();
    s = GetEngineSlot(0)->sub;
    if (D_80051380 & 0x40) {
        if (D_800B95F0 < 3) {
            D_800B95F0++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        D_800B9624 = 0x5A;
    }
    if (D_80051380 & 0x10) {
        if (D_800B95F0 > 0) {
            D_800B95F0--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        D_800B9624 = 0x5A;
    }
    if (D_80051380 & 0x4000) {
        g = D_8005A614;
        ((u16 *)g)[0x19] &= 0xFF00;
        switch (D_800B95F0) {
        case 0:
            g[0x27] = 2;
            *(s32 *)(g + 0x6C) = 1;
            *(s16 *)(g + 0x48) = 0;
            *(s16 *)(g + 0x4A) = 2;
            g[0x54] = 1;
            g[0x55] = 0;
            g[0x56] = 3;
            g[0x57] = 2;
            *(u16 *)(g + 0x32) |= 4;
            MenuChainStart(D_800B8EC0);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 1:
            g[0x27] = 4;
            *(s16 *)(g + 0x4A) = 2;
            ((s8 *)g)[0x54] = -1;
            ((s8 *)g)[0x55] = -1;
            ((s8 *)g)[0x56] = -1;
            ((s8 *)g)[0x57] = -1;
            *(s32 *)(g + 0x6C) = 0;
            *(s16 *)(g + 0x48) = 0;
            *(u16 *)(g + 0x32) |= 1;
            MenuChainStart(D_800B8EA4);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            g[0x27] = 4;
            *(u16 *)(g + 0x32) |= 2;
            MenuChainStart(D_800B8ED4);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            process_interrupt(&D_8009F8A4, &D_800B9510);
            break;
        }
    }
    s[1].unk7C = 0;
    s[2].unk7C = 0;
    s[4].unk7C = 0;
    s[3].unk7C = 0;
    if (D_800B9624 != 0) {
        s[5].flags |= 0x8000;
        D_800B9624--;
        s[5].unk6C = (void *)D_800B8508[D_800B95F0];
    } else {
        s[5].flags &= ~0x8000;
    }
    s[D_800B95F0 + 1].unk7C = 2;
}

void PlayersMenuUpdate(void) {
    subengine *s;
    s32 n;
    s32 i;
    u32 c;

    s = GetEngineSlot(0)->sub;
    s++;
    AnimateTitlesBackdrop();
    s[0].unk7C = 3;
    s[1].unk7C = 3;
    s[2].unk7C = 3;
    s[3].unk7C = 3;
    if (D_8005A63B == 2) {
        s[0].unk08 = -0x22;
        s[1].unk08 = 0;
    } else {
        s[0].unk08 = -0x40;
        s[1].unk08 = -0x1C;
    }
    if (O69CV->unk27 == 2) {
        s[2].flags &= ~0x8000;
        s[3].flags &= ~0x8000;
    }
    n = D_80051600;
    if (n > 4) {
        n = 4;
    }
    if (O69CV->unk27 < n) {
        n = O69CV->unk27;
    }
    for (i = 0; i < n; i++) {
        s[i].unk7C = 0;
    }
    if (n < O69CV->unk26) {
        O69CV->unk26 = n;
    }
    if (O69CV->unk26 != 0) {
        s[O69CV->unk26 - 1].unk7C = 2;
    } else {
        O69CV->unk26 = n;
    }
    if (D_80051380 & 0x10) {
        if (O69CV->unk26 >= 2) {
            O69CV->unk26--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if ((D_80051380 & 0x40) && O69CV->unk26 < n) {
        O69CV->unk26++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((O69CV->unk32 & 4) && O69CV->unk26 == 2) {
        O69CV->unk6C = 1;
        O69CV->unk54 = 1;
        O69CV->unk55 = 0;
        O69CV->unk56 = 3;
        O69CV->unk57 = O69CV->unk26;
    } else {
        O69CV->unk6C = 0;
        O69CV->unk54 = -1;
        O69CV->unk55 = -1;
        O69CV->unk56 = -1;
        O69CV->unk57 = -1;
    }
    c = O69CV->unk26;
    O69CV->unk5C[0] = c < 1;
    O69CV->unk5C[1] = c < 2;
    O69CV->unk5C[2] = c < 3;
    O69CV->unk5C[3] = c < 4;
    O69PAD[0].unk2A = c < 1;
    O69PAD[1].unk2A = c < 2;
    O69PAD[2].unk2A = c < 3;
    O69PAD[3].unk2A = c < 4;
    if (c != 0) {
        if (O69PAD->pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
        }
        if (O69PAD->pressed & 0x1000) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainBack();
        }
    }
}

void NewLoadMenuUpdate(void) {
    Ovl69Sub *s;
    s32 *g;

    AnimateTitlesBackdrop();
    do {
        s = (Ovl69Sub *)GetEngineSlot(0)->sub;
        s->a = (D_800B9628 == 0) * 2;
        s->b = (D_800B9628 == 1) * 2;
        g = &D_80051380;
    } while (0);
    if ((*g & 0x10) && (D_800B9628 > 0)) {
        D_800B9628 -= 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((D_80051380 & 0x40) && (D_800B9628 <= 0)) {
        D_800B9628 += 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (D_80051380 & 0x4000) {
        switch (D_800B9628) {
        case 0:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
            break;
        case 1:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            func_80092AF4(&D_8009F480, 0x200);
            break;
        }
    }
    if (D_80051380 & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void BattleOptionsUpdate(void) {
    subengine *sub;
    subengine *p;

    sub = GetEngineSlot(1)->sub;
    sub->unk14 = D_8005B690 * 0x14;
    sub->unk12 = 0x293;
    sub->unk50 = 1;
    sub->unk20.z = 0x2800;
    sub->unk20.x = 0x2800;
    sub->unk51 = 0;
    sub->unk52 = 1;
    if (((PadButtons *)D_80051338)->pressed & 0x1000) {
        MenuChainBack();
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (((PadButtons *)D_80051338)->pressed & 0x80) {
        if (O69OV->unk3A >= 3) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            O69OV->unk3A--;
        }
    }
    if ((D_80051380 & 0x20) && O69OV->unk3A < 7) {
        O69OV->unk3A++;
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((D_80051380 & 0x40) && O69OV->unk38 > 0) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->unk38--;
    }
    if ((D_80051380 & 0x10) && O69OV->unk38 < 2) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->unk38++;
    }
    *(u8 *)GetEngineSlot(0)->sub[2].unk6C = O69OV->unk3A + '0';
    GetEngineSlot(0)->sub[5].unk6C = (void *)D_800B8E78[O69OV->unk2C][O69OV->unk38];
    p = &GetEngineSlot(0)->sub[8];
    if (O69OV->unk32 & 1) {
        p->unk6C = (void *)D_800B8E84[O69OV->unk2C][0];
    } else {
        p->unk6C = (void *)D_800B8E84[O69OV->unk2C][1];
    }
    if (D_80051380 & 0x4000) {
        D_8005A644 = 0x8008;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void BattleOptionsRender(void) {
    u8 *p;

    D_80056998.modelDp = D_8009F644.amount;
    p = (u8 *)GetEngineSlot(1)->sub;
    *(s32 *)(p + 0xA8) &= 0xFFFF7FFF;
    p = (u8 *)GetEngineSlot(1)->sub;
    *(s32 *)(p + 0x150) &= 0xFFFF7FFF;
    func_80018B08(&D_800B8E00, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(1));
    func_80018B08(&D_800B8DD8, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(0));
}

void TitlesSetHeroMasks(void) {
    u8 *g = D_8005A614;
    s32 i;
    s32 k;

    if (*(s32 *)(g + 0x6C) == 1) {
        i = *(s16 *)(g + 0x48);
        k = ((s8 *)(g + i))[0x54];
        *(s16 *)(g + 0x44) = (s8)((u8)D_8004E0A4[g[i + 0x4C]] | (u8)D_8004E0A4[g[k + 0x4C]]);
        i = *(s16 *)(g + 0x4A);
        k = ((s8 *)(g + i))[0x54];
        *(s16 *)(g + 0x46) = (s8)((u8)D_8004E0A4[g[i + 0x4C]] | (u8)D_8004E0A4[g[k + 0x4C]]);
        return;
    }
    i = *(s16 *)(g + 0x48);
    *(s16 *)(g + 0x44) = (s8)D_8004E0A4[g[i + 0x4C]];
}

void OptionsUpdate(void) {
    subengine *sub;
    s32 v;
    s32 n;
    s32 i;
    subengine *cam;

    cam = GetEngineSlot(1)->sub;
    cam->unk14 = D_8005B690 * 0x14;
    cam->unk12 = 0x293;
    cam->unk50 = 1;
    cam->unk20.z = 0x2800;
    cam->unk20.x = 0x2800;
    cam->unk51 = 0;
    cam->unk52 = 1;
    sub = GetEngineSlot(4)->sub;
    switch (D_800B950C) {
    case 0:
        sub[D_800B9508 + 3].unk7C = 0;
        if ((((PadButtons *)D_80051338)->pressed & 0x40) && D_800B9508 < 2) {
            D_800B9508++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((D_80051380 & 0x10) && D_800B9508 > 0) {
            D_800B9508--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        sub[D_800B9508 + 3].unk7C = 2;
        if (((PadButtons *)D_80051338)->pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            switch (D_800B9508) {
            case 0:
                D_800B950C = 1;
                func_80095BEC(4, (s32 *)D_800B901C);
                break;
            case 1:
                D_800B950C = 2;
                func_80095BEC(4, (s32 *)D_800B91CC);
                break;
            case 2:
                func_8001E824(&D_8009F8A4);
                break;
            }
            D_800B9508 = 0;
        }
        if (D_80051380 & 0x1000) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            func_8001E824(&D_8009F8A4);
        }
        return;
    case 1:
        if ((D_80051380 & 0x40) && D_800B9508 < 2) {
            D_800B9508++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((D_80051380 & 0x10) && D_800B9508 > 0) {
            D_800B9508--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        sub[3].unk7C = 0;
        sub[4].unk7C = 0;
        sub[5].unk7C = 0;
        sub[8].unk7C = 0;
        sub[9].unk7C = 0;
        sub[6].unk7C = 0;
        sub[7].unk7C = 0;
        switch (D_800B9508) {
        case 0:
            if (D_80051380 & 0xA0) {
                s32 *opt = (s32 *)D_8005A604;

                opt[2] ^= 1;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                if (opt[2] != 0) {
                    func_800222BC();
                } else {
                    func_800222DC();
                }
            }
            sub[3].unk7C = 2;
            sub[4].unk7C = 2;
            sub[5].unk7C = 2;
            break;
        case 1:
            v = *(s32 *)D_8005A604;
            if (v > 0 && (D_80051380 & 0x80)) {
                v -= 5;
            }
            if (v < 100 && (D_80051380 & 0x20)) {
                v += 5;
            }
            *(s32 *)D_8005A604 = v;
            sub[6].unk7C = 2;
            sub[7].unk7C = 2;
            break;
        case 2:
            v = D_8005A608;
            if (v > 0 && (D_80051380 & 0x80)) {
                v -= 5;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (v < 100 && (D_80051380 & 0x20)) {
                v += 5;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            D_8005A608 = v;
            sub[8].unk7C = 2;
            sub[9].unk7C = 2;
            break;
        }
        break;
    case 2:
        n = D_800515F8;
        if (n >= 5) {
            n = 4;
        }
        for (i = 0; i < n; i++) {
            if (((PadButtons *)D_80051338)[i].pressed & 0x8000) {
                ((Ovl69Opt *)D_8005A604)->unkC[i] ^= 1;
                func_80028018(i, 0xA);
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        break;
    default:
        return;
    }
    if (D_80051380 & 0x5000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800B950C = 0;
        D_800B9508 = 0;
        func_80095BEC(4, (s32 *)D_800B8F20);
    }
}

void OptionsRender(void) {
    subengine *sub;
    s32 n;
    s32 i;

    sub = GetEngineSlot(4)->sub;
    D_80056998.modelDp = D_8009F644.amount;
    switch (D_800B950C) {
    case 1:
        func_8008D488(sub[7].unk6C, *(s32 *)D_8005A604);
        if (((s32 *)D_8005A604)[2] != 0) {
            sub[5].flags &= ~0x8000;
            sub[4].flags |= 0x8000;
        } else {
            sub[5].flags |= 0x8000;
            sub[4].flags &= ~0x8000;
        }
        func_8008D488(sub[9].unk6C, D_8005A608);
        break;
    case 2:
        n = D_800515F8;
        if (n >= 5) {
            n = 4;
        }
        for (i = 0; i < n; i++) {
            if (D_80051338[i].unk0 & 8) {
                sub[i * 3 + 5].unk7C = 0;
                if (D_8005A604[i + 0xC] != 0) {
                    sub[i * 3 + 4].flags |= 0x8000;
                    sub[i * 3 + 5].flags &= ~0x8000;
                    sub[i * 3 + 6].flags &= ~0x8000;
                } else {
                    sub[i * 3 + 4].flags &= ~0x8000;
                    sub[i * 3 + 5].flags |= 0x8000;
                    sub[i * 3 + 6].flags &= ~0x8000;
                }
            } else {
                sub[i * 3 + 4].flags &= ~0x8000;
                sub[i * 3 + 5].flags &= ~0x8000;
                sub[i * 3 + 6].flags |= 0x8000;
            }
        }
        for (; i < 4; i++) {
            sub[i * 3 + 5].unk7C = 3;
            sub[i * 3 + 4].flags &= ~0x8000;
            sub[i * 3 + 5].flags &= ~0x8000;
            sub[i * 3 + 6].flags |= 0x8000;
        }
        break;
    }
    func_80018B08(&D_800B8DD8, &D_800B9538);
    func_8001C2C0((u8 *)GetEngineSlot(4));
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    func_80018B08(&D_800B8E00, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(1));
}

void RunGameQuit(void) {
    if (D_8009E598 != NULL) {
        D_8009E598();
    }
}

void RunGameRender(void) {
    if (D_8009E594 != NULL) {
        D_8009E594();
    }
}

void RunGameUpdate(void) {
    if (D_8009E590 != NULL) {
        D_8009E590();
    }
}

void RunGameInit(void) {
    void func_8008EC00();
    func_8001E984(0x20, 0x20);
    if (D_8009E58C != NULL) {
        D_8009E58C();
    }
    func_800222FC();
    func_800229FC(D_8005A648);
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
    func_8008EC00(D_8004E568);
    func_8002BBB4();
    func_8002BD0C((s32)(D_8004E568 + 0x438));
    while (func_80012FFC() != 0) {
        func_8001231C();
        func_80010AE8(&D_8004E0F0);
        func_8002BAE8();
    }
}

void OptionsQuit(void) {
}

void OptionsInit(void) {
    func_80095BEC(4, (s32 *)D_800B8F20);
    D_800B9508 = 0;
    D_800B950C = 0;
}

void MenuChainStart(s32 *arg0) {
    D_800B9620 = arg0;
    func_8001E848(&D_8009F8A4, (process_info *)*arg0);
}

void MenuChainStep(s32 arg0) {
    s32 *temp_v1;
    s32 temp_a1;

    temp_v1 = &D_800B9620[arg0];
    temp_a1 = *temp_v1;
    D_800B9620 = temp_v1;
    if (temp_a1 != 0) {
        func_8001E848(&D_8009F8A4, (process_info *)temp_a1);
    }
}

void MenuChainBack(void) {
    s32 *temp_v0;
    s32 temp_a1;

    temp_v0 = D_800B9620 - 1;
    temp_a1 = *temp_v0;
    D_800B9620 = temp_v0;
    if (temp_a1 != 0) {
        func_8001E848(&D_8009F8A4, (process_info *)temp_a1);
    }
}

void MenuChainNext(void) {
    s32 *temp_v0;
    s32 temp_a1;

    temp_v0 = D_800B9620;
    temp_a1 = temp_v0[1];
    D_800B9620 = temp_v0 + 1;
    if (temp_a1 != 0) {
        func_8001E848(&D_8009F8A4, (process_info *)temp_a1);
    }
}

void BattleOptionsQuit(void) {
}

void BattleOptionsInit(void) {
    func_80095BEC(0, (s32 *)D_800B8974);
    D_800B962C = 2;
}

void TitlesLeaveRender(void) {
    if (D_800B9DF4.render != NULL) {
        D_800B9DF4.render();
    }
}

void TitlesLeaveInit(void) {
    set_fade(&D_8009F644, 0x200, 0);
    func_80094D88((s32)D_800A0840, &D_8009E5C8);
}

void NewLoadMenuInit(void) {
    u8 *g;
    func_80095BEC(0, (s32 *)D_800B889C);
    D_800B9628 = 0;
    D_800B95F0 = 0;
    g = D_8005A614;
    g[0x27] = 2;
    *(s32 *)(g + 0x6C) = 1;
    g[0x54] = 1;
    *(s16 *)(g + 0x48) = 0;
    *(s16 *)(g + 0x4A) = 2;
    g[0x55] = 0;
    g[0x56] = 3;
    g[0x57] = 2;
    *(u16 *)(g + 0x32) |= 4;
}

void num_players_render(void) {
    D_800569AC = D_8009F644.amount;
    func_80018B08(&D_800B8DD8, &D_800B9538);
    func_8001C2C0((u8 *)GetEngineSlot(0));
}

extern u8 D_8005A63A;

void PlayersMenuInit(void) {
    if (D_8005A646 & 4) {
        func_80095BEC(0, (s32 *)D_800B877C);
    } else {
        func_80095BEC(0, (s32 *)D_800B865C);
    }
    D_8005A63A = D_80051600 != 0;
}

void MainMenuInit(void) {
    u8 *g;
    D_800B95F0 = 0;
    func_80092C54();
    g = D_8005A614;
    *(s16 *)(g + 0x34) = 0x28;
    D_800B9624 = 0;
    *(u16 *)(g + 0x32) |= 0x200;
    func_80095BEC(0, (s32 *)D_800B8518);
}

void RenderTitlesBackdropAndMenu(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    func_80018B08(&D_800B8E00, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(1));
    func_80018B08(&D_800B8DD8, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(0));
}

void RenderTitlesBackdrop(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    func_80018B08(&D_800B8E00, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(1));
}

void RenderTitlesMenu(void) {
    GetEngineSlot(1)->sub[1].flags |= 0x8000;
    GetEngineSlot(1)->sub[2].flags |= 0x8000;
    func_80018B08(&D_800B8DD8, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(0));
    func_80018B08(&D_800B8E00, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(1));
}

void SetupTitlesBackdrop(void) {
    PlaceSub *s = (PlaceSub *)GetEngineSlot(1)->sub;

    s->unk14 = D_8005B690 * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->sz = 0x2800;
    s->sx = 0x2800;
    s->unk51 = 0;
    s->unk52 = 1;
}

void MenuConfirmOrBack(void) {
    if (((PadButtons *)D_80051338)->pressed & 0x4000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainNext();
    }
    if (((PadButtons *)D_80051338)->pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void TitlesCamFromPath(u8 *cam, s32 arg1) {
    u8 *o = (u8 *)path_get(arg1);
    s32 *p;
    s32 t;

    if (o != NULL) {
        *(s16 *)(cam + 0x54) = (get_angle(*(s32 *)(o + 0x58) - *(s32 *)(o + 0x3C),
                                          *(s32 *)(o + 0x50) - *(s32 *)(o + 0x34)) - 0x400) & 0xFFF;
        *(s16 *)(cam + 0x56) = 0;
        *(s16 *)(cam + 0x58) = 0;
        *(s32 *)(cam + 0x18) = (((s32)(*(u16 *)(o + 0x40) << 16)) >> 18) + 0x200;
        *(s32 *)(cam + 0xC) = *(s32 *)(o + 0x30);
        p = (s32 *)(o + 0x30);
        t = p[1];
        *(s32 *)(cam + 0x10) = t;
        t = p[2];
        *(s32 *)(cam + 0x14) = t;
        *(s32 *)(cam + 4) = -p[1];
        *(s32 *)cam = 0;
        *(s32 *)(cam + 8) = *(s32 *)(o + 0x58);
    }
}

void TitlesCameraHook(u8 *cam, s32 arg1) {
    if (arg1 != 0) {
        TitlesCamFromPath(cam, 1);
    }
    D_8009F708 = 0xF0;
}

void TitlesEmptyHook(void) {
}

void TitlesUpdateLevel(void) {
    func_800139A8();
}

void TitlesPreInitLevel(void) {
    u8 *g = D_8005A614;

    g[0x50] = 9;
    g[0x51] = 9;
    g[0x52] = 9;
    g[0x53] = 9;
    *(u16 *)(g + 0x32) |= 0x200;
}

void TitlesSetUp(void) {
    D_8005AA70[0] = (s32)TitlesPreInitLevel;
    D_8005AA70[1] = (s32)TitlesInitLevel;
    D_8005AA70[2] = (s32)TitlesInitLevel;
    D_8005AA70[3] = (s32)TitlesUpdateLevel;
    D_8005AA70[5] = 0;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8009F8B4 = TitlesEmptyHook;
}

void InitCharSelector(s32 arg0) {
    EXTengine *e0;
    EXTengine *e2;
    Ovl69Obj *o;
    subengine *s;
    subengine *t;
    u8 *g;

    e0 = GetEngineSlot(0);
    e2 = GetEngineSlot(2);
    o = (Ovl69Obj *)&D_800B9FD4[arg0];
    s = e2->sub + arg0;
    o->unk8 = s;
    t = e0->sub + arg0;
    o->unkC = t;
    o->unk10 = e2->sub + (arg0 + 4);
    o->ext = D_8005A9A0;
    o->unk0 = 0;
    o->unk4 = arg0;
    o->unk2C = 0;
    o->unk28 = Random(0x1E) + 0x1E;
    o->unk14 = Random(0x200) - 0x100;
    o->unk18 = Random(0x200) - 0x100;
    s->unk50 = 1;
    s->unk68 = -0x64;
    s->unk51 = 0;
    s->unk20.x = 0;
    s->unk20.y = 0;
    s->flags |= 0x02000000;
    g = &D_8005A63A;
    o->unk30 = arg0;
    do {
    } while (0);
    o->unk24 = 0;
    if (arg0 < *g) {
        t->flags |= 0x8000;
    } else {
        t->flags &= 0xFFFF7FFF;
    }
}

void UpdateCharSelector(Ovl69Slot *slot) {
    Ovl69Obj *o = (Ovl69Obj *)slot;
    subengine *t;
    subengine *s;
    subengine *s2;
    Ovl69Card *card;
    Ovl69Off *off;
    s32 y;
    s32 x;
    s32 still;
    s32 tgt;
    s32 v;
    s32 a;
    s32 d;

    s = o->unk8;
    t = o->unkC;
    s2 = o->unk10;
    x = O69CARD[o->unk30].x + D_800B9C84[o->unk4].x;
    y = O69CARD[o->unk30].y + D_800B9C84[o->unk4].y;
    t->unk04 = func_80015210(t->unk04, x, 0x800);
    t->unk08 = func_80015210(t->unk08, y, 0x800);
    still = abs(t->unk04 - x) + abs(t->unk08 - y);
    s->unk08 = s2->unk08 = ((Ovl69Trig *)D_80068BD4)[(D_8005B690 * o->unk28 - 0x400) & 0xFFF].hi + 0x1A4;
    still = still == 0;
    if (o->unk0 & 1) {
        tgt = O69CARD[o->unk30].target;
        v = o->unk14;
        v = Ovl69Steer(o->unk1C, tgt, v, 0x2BC);
        o->unk14 = v;
        o->unk1C += v;
        s->unk20.x = o->unk1C >> 4;
        v = o->unk18;
        v = Ovl69Steer(o->unk20, tgt, v, 0x3E8);
        o->unk18 = v;
        o->unk20 += v;
        s->unk20.y = o->unk20 >> 4;
        {
            EXTengine *e = GetEngineSlot(2);
            func_800251A8(NULL, e, o->unk8, (EXTanimator *)&o->ext);
        }
        s->unk52 = 1;
        o->unk24++;
        return;
    }
    if (o->unk4 < D_8005A63A) {
        if (O69PAD[o->unk4].pressed & 0x80) {
            PlaySfx(D_800B9CB4[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30--;
        }
        if (O69PAD[o->unk4].pressed & 0x20) {
            PlaySfx(D_800B9CB4[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30++;
        }
        if (O69PAD[o->unk4].pressed & 0x10) {
            PlaySfx(D_800B9CB4[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30 -= 4;
        }
        if (O69PAD[o->unk4].pressed & 0x40) {
            PlaySfx(D_800B9CB4[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30 += 4;
        }
    }
    o->unk30 &= 7;
    if ((o->unk4 < ((Ovl69ModeView *)D_8005A614)->unk26 && (O69PAD[o->unk4].pressed & 0x4000) && still) || o->unk2C != 0) {
        GetEngineSlot(0)->sub[20].flags &= 0xFFFF7FFF;
        GetEngineSlot(0)->sub[21].flags &= 0xFFFF7FFF;
        card = O69CARD;
        if (card[o->unk30].flags & 1) {
            PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
            return;
        }
        if (o->unk2C == 0) {
            PlaySfx(card[o->unk30].sfx, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        card[o->unk30].flags |= 1;
        o->unk1C = 0;
        o->unk20 = 0;
        o->unk14 = 0;
        o->unk18 = 0;
        o->unk0 |= 1;
        {
            s->unk6C = D_80052A54 + card[o->unk30].unk14 * 0x38;
            s->flags |= 0x8000;
        }
        D_8005A614[o->unk4 + 0x50] = card[o->unk30].unk4;
        D_8005A614[o->unk4 + 0x4C] = card[o->unk30].unk4;
        PlaySfx(D_800B9CA4[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
        o->ext = D_8005A9A0;
        {
            s32 w = O69CARD[o->unk30].unk1C;
            o->unk24++;
            o->ext.w[3] = w;
        }
    }
}

void CharSelectInit(void) {
    EXTengine *eng;
    Ovl69Rec *rec;
    s32 i;
    s32 off;

    cam_init((u8 *)&D_800B9F1C);
    *(s32 *)((u8 *)&D_800B9F1C + 8) = 0x400;
    *(s32 *)((u8 *)&D_800B9F1C + 0x18) = 0x190;
    eng = GetEngineSlot(0);
    func_80095BEC(0, (s32 *)D_800B9634);
    func_80095BEC(2, (s32 *)D_800B9A00);
    i = 0;
    do {
        InitCharSelector(i);
        i++;
    } while (i < 4);
    i = 0;
    rec = D_800B9B44;
    off = 0x7E0;
    do {
        i++;
        rec->unk24 = (u8 *)eng->sub + off;
        rec->unk0 = 0;
        rec++;
        off += 0xA8;
    } while (i < 8);
    ShuffleCharSelectors();
}

void ShuffleCharSelectors(void) {
    s32 i;
    s32 j;

    i = 0;
    if (D_8005A63A != 0) {
        do {
            do {
                D_800B9FD4[i].idx = Random(8);
                for (j = 0; j < i; j++) {
                    if (D_800B9FD4[j].idx == D_800B9FD4[i].idx) {
                        break;
                    }
                }
            } while (j != i);
            D_800B9FD4[i].dst->unk4 = ((Ovl69Pt *)D_800B9B44)[D_800B9FD4[i].idx].x + D_800B9C84[i].x;
            D_800B9FD4[i].dst->unk8 = ((Ovl69Pt *)D_800B9B44)[D_800B9FD4[i].idx].y + D_800B9C84[i].y;
            i++;
        } while (i < ((Ovl69ModeView *)D_8005A614)->unk26);
    }
}

s32 CpuPickCharacter(s32 a0) {
    u8 *g = D_8005A614;
    s32 r;
    s32 i;
    s8 *t;
    s32 v;
    s32 w;
    Ovl69Rec *rec;

    if (*(u16 *)(g + 0x32) & 4) {
        if (g[0x26] == 1) {
            s8 *t = D_800B9CC4;
            s32 v = Random(2) + (a0 - 1) * 2;
            s32 w = (g[0x4C] - 1) * 6;
            r = t[v + w];
        } else {
            s8 *t = D_800B9CF4;
            s32 v = Random(2) + (a0 - 2) * 2 + (g[0x4D] - 1) * 4;
            s32 w = (g[0x4C] - 1) << 5;
            r = t[v + w];
        }
        i = 0;
        rec = D_800B9B44;
        do {
            if (*(s32 *)rec->unk4 == r) {
                return i;
            }
            i++;
            rec++;
        } while (i < 8);
        return -1;
    }
    {
        Ovl69Rec *p = D_800B9B44;
        s32 k;
        s32 j;
        do {
            k = Random(4) * 2;
            if (p[k].unk0 & 1) {
                continue;
            }
            j = k + 1;
        } while ((p[k].unk0 & 1) || (p[j].unk0 & 1));
        return k + Random(2);
    }
}

void CharSelectUpdate(void) {
    EXTengine *eng;
    subengine *s;
    subengine *s2;
    s32 min;
    s32 i;

    AnimateTitlesBackdrop();
    eng = GetEngineSlot(0);
    s = &eng->sub[24];
    s->unk08 = ((D_80068BD4[(((D_8004E0D8 * 50) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0x6E;
    s->unk12 = (D_80068BD4[(((D_8004E0D8 * 25) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s->unk14 = (D_80068BD4[(((D_8004E0D8 * 36) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s->unk52 = 1;
    s2 = &GetEngineSlot(0)->sub[25];
    s2->unk08 = ((D_80068BD4[(((D_8004E0D8 * 41) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0xBE;
    s2->unk12 = (D_80068BD4[(((D_8004E0D8 * 35) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s2->unk14 = (D_80068BD4[(((D_8004E0D8 * 26) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s2->unk52 = 1;
    min = 0x7FFFFFFF;
    for (i = 0; i < ((Ovl69ModeView *)D_8005A614)->unk26; i++) {
        if (D_800B9FD4[i].pad10[5] < min) {
            min = D_800B9FD4[i].pad10[5];
        }
    }
    if (min >= 0x10) {
        for (i = D_8005A63A; ; i++) {
            s32 w;

            if (i >= 4) {
                break;
            }
            w = D_800B9FD4[i].pad10[5];
            if (w > 0 && w < 15) {
                min = 0;
                break;
            }
            if (w == 0) {
                D_800B9FD4[i].idx = CpuPickCharacter(i);
                D_800B9FD4[i].pad10[7]++;
                min = 0;
                break;
            }
        }
        if (min >= 0x79) {
            TitlesSetHeroMasks();
            MenuChainNext();
        }
    }
    if (D_80051380 & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    for (i = 0; i < 4; i++) {
        UpdateCharSelector(&D_800B9FD4[i]);
    }
}

void CharSelectRender(void) {
    EXTengine *eng;

    eng = GetEngineSlot(0);
    eng->unk10.x = -0x140;
    eng->unk10.y = -0xF0;
    RenderTitlesBackdropAndMenu();
    func_80018B08(&D_800B8DD8, &D_800B9F1C);
    D_800569AC = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(2));
}

s8 adventure_mode_head(s32 a0) {
    u8 *g = D_8005A614;

    if (g[0x26] == 1) {
        s8 *t = D_800B9CC4;
        s32 v = Random(2) + (a0 - 1) * 2;
        s32 w = (g[0x4C] - 1) * 6;
        return t[v + w];
    } else {
        s8 *t = D_800B9CF4;
        s32 v = Random(2) + (a0 - 2) * 2 + (g[0x4D] - 1) * 4;
        s32 w = (g[0x4C] - 1) << 5;
        return t[v + w];
    }
}

int ApproachSpeed(int a0, int a1, int a2, int a3, int a4) {
    int v;

    if (a0 != a1) {
        if (a0 < a1) {
            if (a2 < 0) {
                a2 += a4;
            } else {
                a2 += a3;
            }
        } else if (a2 > 0) {
            a2 -= a4;
        } else {
            a2 -= a3;
        }
    }
    v = a2;
    if (a2 < 0) {
        v = -a2;
    }
    if (a3 >= v) {
        v = a1 - a0;
        if (v < 0) {
            v = -v;
        }
        if (a3 >= v) {
            a2 = 0;
        }
    }
    return a2;
}

s32 UnlockCardLevels(s32 arg0) {
    s32 *p;
    s32 *q;
    s32 j;
    Ovl69Entry *e;
    s32 count;
    u8 *g;
    s32 i;
    s32 found;
    u8 *tbl;

    e = D_800BA324;
    count = 0;
    i = count;
    tbl = D_800657D0;
outer:
    if (i < 7) {
        j = 0;
        p = (s32 *)e->unk0;
        q = p + 1;
        g = D_8005A614;
    inner:
        if (j < e->unk4) {
            if (p[0] == 2) {
                s32 n;
                s32 idx = q[0];
                u8 *gg;
                do { do { do { do {
                    gg = (u8 *)(idx + (s32)g);
                } while (0); } while (0); } while (0); } while (0);
                if (gg[0xE4] == 0) {
                    s32 k;
                    u8 *b;
                    do {
                        n = idx;
                    } while (0);
                    k = 0;
                    b = tbl;
                    while (1) {
                        if (k >= 4) {
                            break;
                        }
                        if ((b + n)[0xE4] != 0) {
                            goto hit;
                        }
                        b += 0x194;
                        k++;
                    }
                    found = 0;
                ktest:
                    if (found) {
                        do {
                            count++;
                        } while (0);
                        if (arg0 != 0) {
                            g[q[0] + 0xE4] = 1;
                        }
                    }
                }
            }
            do {
                q += 4;
                p += 4;
                j++;
            } while (0);
            goto inner;
        hit:
            found = 1;
            goto ktest;
        }
        do {
            e++;
        } while (0);
        i++;
        goto outer;
    }
    return count;
}

void LevelSelectInit(void) {
    subengine *sub;

    D_800BA744 = 0;
    func_80095BEC(0, (s32 *)D_800BA4D4);
    func_80095BEC(2, (s32 *)D_800BA35C);
    func_80095BEC(3, (s32 *)D_800BA3A4);
    func_80095BEC(4, (s32 *)D_800BA3EC);
    func_80095BEC(5, (s32 *)D_800BA434);
    func_80095BEC(6, (s32 *)D_800BA47C);
    GetEngineSlot(1)->sub[4].flags |= 0x8000;
    GetEngineSlot(1)->sub[5].flags |= 0x8000;
    GetEngineSlot(1)->sub[6].flags |= 0x8000;
    GetEngineSlot(1)->sub[7].flags |= 0x8000;
    sub = GetEngineSlot(1)->sub;
    D_800BA740 = 0;
    sub[3].flags |= 0x8000;
    LevelSelectCursor(0);
}

void LevelSelectInput(void) {
    s32 *tbl;
    u8 *g;
    EXTengine *e;
    s32 c;

    if ((D_80051380 & 0x80) && (s8)D_8005A614[0x36] > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800BA744 = 4;
        D_8005A614[0x36]--;
    }
    if ((D_80051380 & 0x20) && (s8)D_8005A614[0x36] < 6) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_800BA744 = 4;
        D_8005A614[0x36]++;
    }
    if ((D_80051380 & 0x10) && (s8)D_8005A614[0x37] > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8005A614[0x37]--;
    }
    if ((D_80051380 & 0x40)
        && (s8)D_8005A614[0x37] < D_800BA324[(s8)D_8005A614[0x36]].unk4 - 1) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8005A614[0x37]++;
    }
    if (((PadButtons *)D_80051338)->pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    if (((PadButtons *)D_80051338)->pressed & 0x4000) {
        if (Ovl69SlotOn()) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
        } else {
            PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (Ovl69SlotOn()) {
        GetEngineSlot(0)->sub->unk6C = (void *)Ovl69SlotRowI((s8)D_8005A614[0x36], (s8)D_8005A614[0x37]);
        GetEngineSlot(0)->sub->unk08 = 8;
        GetEngineSlot(2)->sub->unk6C = (void *)Ovl69SlotModelI((s8)D_8005A614[0x36], (s8)D_8005A614[0x37]);
        GetEngineSlot(2)->sub->unk74 = Ovl69SlotCI((s8)D_8005A614[0x36], (s8)D_8005A614[0x37]);
        GetEngineSlot(0)->sub[4].flags |= 0x8000;
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
        return;
    }
    e = GetEngineSlot(0);
    tbl = D_800BA160;
    g = D_8005A614;
    e->sub->unk6C = (void *)tbl[*(s32 *)(g + 0x2C)];
    GetEngineSlot(2)->sub->unk74 = 0;
    GetEngineSlot(0)->sub[4].flags &= ~0x8000;
    GetEngineSlot(0)->sub->unk08 = 0x1C;
    GetEngineSlot(0)->sub[9].flags |= 0x8000;
    if ((c = ((Ovl69ListSlot *)((u8 *)D_800BA324[(s8)D_8005A614[0x36]].unk0 + ((s8)D_8005A614[0x37] << 4)))->type == 2)) {
        if (D_800655C0 & 2) {
            if (D_800655C0 & 0x40) {
                GetEngineSlot(0)->sub[9].unk6C = (void *)D_800BA660[*(s32 *)(D_8005A614 + 0x2C)];
            } else if (D_800655C0 & 0x10) {
                if (UnlockCardLevels(0) != 0) {
                    *(s32 *)&GetEngineSlot(0)->sub[9].unk6C = D_800BA154[*(s32 *)(D_8005A614 + 0x2C)];
                    if (D_80051380 & 0x2000) {
                        PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                        UnlockCardLevels(1);
                    }
                } else {
                    GetEngineSlot(0)->sub[9].unk6C = (void *)D_800BA158[*(s32 *)(D_8005A614 + 0x2C)];
                }
            } else {
                GetEngineSlot(0)->sub[9].unk6C = (void *)D_800BA15C[*(s32 *)(D_8005A614 + 0x2C)];
            }
        } else {
            GetEngineSlot(0)->sub[9].unk6C = (void *)D_800BA15C[*(s32 *)(D_8005A614 + 0x2C)];
        }
    } else {
        GetEngineSlot(0)->sub[9].unk6C = NULL;
    }
}

void LevelSelectUpdate(void) {
    Ovl69ListSlot *slot;
    subengine *q;
    s32 val;
    u8 *a;
    subengine *s;
    subengine *t;
    EXTengine *e0;
    s32 i;
    s32 off;
    u8 *g;
    s32 boff;
    s32 w;
    s32 c;
    u8 *h;
    Ovl69Entry *tbl;
    TrigEntry *trig;

    if (D_800BA744 != 0) {
        func_80017640();
        func_80011D28((u8 *)&D_8004E0F0);
        D_800BA744--;
    }
    AnimateTitlesBackdrop();
    a = D_800555DC;
    func_80029100((obj_textureset *)(a + 0x10));
    func_80029100((obj_textureset *)(a + 0x48));
    val = D_800BA740;
    if (val == 0) {
        LevelSelectInput();
    }
    e0 = GetEngineSlot(0);
    boff = ((Ovl69ListSlot *)((u8 *)D_800BA324[(s8)D_8005A614[0x36]].unk0 + ((s8)D_8005A614[0x37] << 4)))->idx;
    w = D_8004DDD0[boff * 3] - 0x101;
    off = ((s32 **)D_8004DDBC)[*(s32 *)(D_8005A614 + 0x2C)][w];
    e0->sub[6].unk6C = (void *)off;
    t = &GetEngineSlot(0)->sub[1];
    t->unk08 = func_80015284(t->unk08,
        *(s16 *)((u8 *)&GetEngineSlot(1)->sub[(s8)D_8005A614[0x37] + 4] + 0x72) + 0x32, 0x800);
    i = 0;
    off = 0;
    for (;;) {
        s32 type;
        s32 idx;
        s32 on;
        s32 fl;
        EXTengine *e;

        if (i >= (D_800BA324 + (s8)D_8005A614[0x36])->unk4) {
            break;
        }
        GetEngineSlot(1)->sub[off + 4].flags |= 0x8000;
        e = GetEngineSlot(i + 3);
        fl = *(s32 *)(D_8005A614 + 4) & 0x2000;
        slot = &((Ovl69ListSlot *)D_800BA324[(s8)D_8005A614[0x36]].unk0)[off];
        idx = slot->idx;
        type = slot->type;
        s = e->sub;
        if (fl || type == 1
            || (type == 2 && ((g = D_8005A614 + idx), g[0xE4] != 0 || g[0x168] != 0))) {
            on = 1;
        } else {
            on = 0;
        }
        if (on) {
            s->unk6C = ((Ovl69ListSlot *)((u8 *)D_800BA324[(s8)D_8005A614[0x36]].unk0 + (off << 4)))->model;
            c = ((Ovl69ListSlot *)((u8 *)D_800BA324[(s8)D_8005A614[0x36]].unk0 + (off << 4)))->unkC;
            s->unk74 = c;
            s->unk10 = 0x7D;
            s->unk12 = 0;
            s->unk14 = 0;
            s->unk04 = D_80068BD4[(((D_8004E0D8 * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
            s->unk08 = (D_80068BD4[(((D_8004E0D8 * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
        } else {
            s->unk6C = D_800555DC;
            s->unk74 = 0x2006;
            s->unk10 = 0;
            s->unk12 = D_8005B690 * 50 + (i << 10);
            val = D_80068BD4[(((D_8004E0D8 * 100 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2;
            s->unk04 = 0;
            s->unk08 = 0;
            s->unk14 = (val * 25) >> 9;
        }
        off++;
        i++;
        s->unk51 = 0;
        s->unk52 = 1;
    }
    for (i = (D_8005A64A + D_800BA324)->unk4; i < 4; i++) {
        GetEngineSlot(1)->sub[i + 4].flags &= ~0x8000;
        GetEngineSlot(i + 3)->sub->unk74 = 0;
    }
    h = D_8005A614;
    D_8009E5DC = ((Ovl69ListSlot *)((u8 *)D_800BA324[(s8)h[0x36]].unk0 + ((s8)h[0x37] << 4)))->idx;
    q = GetEngineSlot(2)->sub;
    q->unk10 = 0x7D;
    q->unk51 = 0;
    q->unk04 = D_80068BD4[(((D_8004E0D8 * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;    q->unk08 = (D_80068BD4[(((D_8004E0D8 * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
    q->unk52 = 1;
}

void LevelSelectRender(void) {
    s32 *cam = (s32 *)&D_800B9538;
    s8 *m;

    func_80018B08(&D_800BA664, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(2));
    RenderTitlesBackdropAndMenu();
    m = (s8 *)D_8005A614;
    cam[2] = (m[0x37] != 0) ? 0x400 : 0x578;
    func_80018B08(&D_800BA68C, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(3));
    cam[2] = (m[0x37] != 1) ? 0x400 : 0x578;
    func_80018B08(&D_800BA6B4, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(4));
    cam[2] = (m[0x37] != 2) ? 0x400 : 0x578;
    func_80018B08(&D_800BA6DC, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(5));
    cam[2] = (m[0x37] != 3) ? 0x400 : 0x578;
    func_80018B08(&D_800BA704, &D_800B9538);
    D_80056998.modelDp = D_8009F644.amount;
    func_8001C2C0((u8 *)GetEngineSlot(6));
    cam[2] = 0x400;
}

void LevelSelectQuit(void) {
    GetEngineSlot(1)->sub[4].flags &= ~0x8000;
    GetEngineSlot(1)->sub[5].flags &= ~0x8000;
    GetEngineSlot(1)->sub[6].flags &= ~0x8000;
    GetEngineSlot(1)->sub[7].flags &= ~0x8000;
    GetEngineSlot(1)->sub[3].flags &= ~0x8000;
}

void LevelSelectCursor(s32 arg0) {
    u8 *p = (u8 *)GetEngineSlot(0)->sub + 0xA8;
    s32 v = *(s16 *)((u8 *)GetEngineSlot(1)->sub + D_8005A64B * 0xA8 + 0x312) + 0x32;

    if (arg0 != 0) {
        *(s32 *)(p + 8) = func_80015284(*(s32 *)(p + 8), v, 0x800);
    } else {
        *(s32 *)(p + 8) = v;
    }
}

s32 IsLevelUnlocked(s32 arg0) {
    u8 *g = D_8005A614;
    u8 *e = (u8 *)D_800BA324[(s8)g[0x36]].unk0 + (arg0 << 4);
    s32 off = *(s32 *)(e + 4);
    s32 type = *(s32 *)e;
    u8 *p;

    if ((*(s32 *)(g + 4) & 0x2000) || type == 1
        || ((p = g + off), type == 2)
            && (p[0xE4] != 0 || p[0x168] != 0)) {
        return 1;
    }
    return 0;
}

s32 IsCardLevel(s32 arg0) {
    s32 i = D_8005A64A;
    u8 *s = (u8 *)D_800BA324[i].unk0;
    return *(s32 *)(s + (arg0 << 4)) == 2;
}

void TitlesEmptyStub(void) {
}

s32 LevelListName(s32 arg0, s32 arg1) {
    s32 idx = *(s32 *)(D_800BA324[arg0].unk0 + (arg1 << 4) + 4);
    s32 w = D_8004DDD0[idx * 3] - 0x101;
    s32 **tbl = (s32 **)D_8004DDBC;
    return tbl[D_8005A640][w];
}

s32 LevelListModel(s32 arg0, s32 arg1) {
    return *(s32 *)(D_800BA324[arg0].unk0 + (arg1 << 4) + 8);
}

s32 LevelListAnim(s32 a, s32 b) {
    return *(s32 *)(D_800BA324[a].unk0 + (b << 4) + 0xC);
}

s32 LevelListCount(s32 arg0) {
    return D_800BA324[arg0].unk4;
}

s32 LevelListText(s32 a, s32 b) {
    s32 base = D_800BA324[a].unk0;
    s32 idx = *(s32 *)((u8 *)base + (b << 4) + 4);
    s32 *row = D_8004DD70[D_8005A640];
    return row[idx];
}

s32 LevelListType(s32 a, s32 b) {
    return *(s32 *)((u8 *)D_800BA324[a].unk0 + (b << 4));
}

s32 LevelListLevel(s32 a, s32 b) {
    return *(s32 *)((u8 *)D_800BA324[a].unk0 + (b << 4) + 4);
}

s32 AnyCardHasLevel(s32 idx) {
    u8 *p;
    s32 i;

    i = 0;
    p = D_800657D0;
loop:
    if (i < 4) {
        if ((p + idx)[0xE4] != 0) {
            return 1;
        }
        p += 0x194;
        i++;
        goto loop;
    }
    return 0;
}

s32 LevelUnlockedIn(s32 idx, u8 *base) {
    return (base + idx)[0xE4] != 0;
}

void TeamModeShowPreset(void) {
    Ovl69Preset *p;
    u8 *s;

    s = (u8 *)GetEngineSlot(0)->sub;
    p = &D_800BAA08[D_800BAACC];
    *(u16 *)(s + 0x74) = D_800BAAA0[O69PV->unk4C[p->hero[0]]];
    *(u16 *)(s + 0x11C) = D_800BAAA0[O69PV->unk4C[p->hero[1]]];
    *(u16 *)(s + 0x1C4) = D_800BAAA0[O69PV->unk4C[p->hero[2]]];
    *(u16 *)(s + 0x26C) = D_800BAAA0[O69PV->unk4C[p->hero[3]]];
    *(s32 *)(s + 0x540) &= ~0x8000;
    *(s32 *)(s + 0x5E8) &= ~0x8000;
    *(s32 *)(s + 0x690) &= ~0x8000;
    *(s32 *)(s + 0x540) |= p->flags[0];
    *(s32 *)(s + 0x5E8) |= p->flags[1];
    *(s32 *)(s + 0x690) |= p->flags[2];

    OVL69_SETBAR(0, p->val[0]);
    OVL69_SETBAR(1, p->val[1]);
    OVL69_SETBAR(2, p->val[2]);
    OVL69_SETBAR(3, p->val[3]);

    *(s32 *)(s + 0x8F4) = p->tbl[O69PV->unk2C];
    O69PV->unk6C = p->unk30 != 0;
    O69PV->unk48 = p->unk34;
    O69PV->unk4A = p->unk38;
    O69PV->unk54 = p->unk3C;
    O69PV->unk55 = p->unk40;
    O69PV->unk56 = p->unk44;
    O69PV->unk57 = p->unk48;
}

void TeamModeUpdate(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    SetupTitlesBackdrop();
    if (D_800BAAC8 == 0) {
        if (D_80051380 & 0x80) {
            temp_v0 = D_800BAACC - 1;
            D_800BAACC = temp_v0;
            if (temp_v0 < 0) {
                D_800BAACC = 1;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (D_80051380 & 0x20) {
            temp_v0_2 = D_800BAACC + 1;
            D_800BAACC = temp_v0_2;
            if (temp_v0_2 >= 2) {
                D_800BAACC = 0;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        TeamModeShowPreset();
        if (D_80051380 & 0x4000) {
            MenuChainNext();
            PlaySfx(0x192, 0, 0, 0x1000, 0x1E00, 0);
        }
    }
    if ((D_80051380 & 0x10) && (D_800BAAC8 > 0)) {
        D_800BAAC8 -= 1;
    }
    if ((D_80051380 & 0x40) && (D_800BAAC8 < 0)) {
        D_800BAAC8 += 1;
    }
    if (D_80051380 & 0x1000) {
        PlaySfx(0x384, 0, 0, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void team_mode_init(void) {
    func_80095BEC(0, (s32 *)D_800BA754);
    D_800BAAC8 = 0;
    D_800BAACC = 0;
}

void MoveTeamBar(s32 arg0, s32 arg1) {
    EXTengine *eng = GetEngineSlot(0);
    s32 off = arg0 * 0xA8;
    u8 *p = (u8 *)eng->sub + (off + 0x2A0);
    s16 v;

    v = func_80015284(*(s16 *)(p + 0x70), arg1, 0x800);
    *(s16 *)(p + 0x70) = v;
    *(s16 *)(p + 0x76) = v + 0x78;
    *(s32 *)((u8 *)eng->sub + off + 4) = *(s16 *)(p + 0x70) + 0x1A;
}

void TitlesShellScreenRender(void) {
    func_800917B4();
}

void TitlesShellScreenUpdate(void) {
}

void TitlesShellScreenInit(void) {
    func_8009246C();
}

void TitlesExtraScreenRender(void) {
    func_8001C2C0((u8 *)GetEngineSlot(2));
}

void TitlesExtraScreenInit(void) {
    func_80095BEC(2, (s32 *)D_800BAAD0);
}
