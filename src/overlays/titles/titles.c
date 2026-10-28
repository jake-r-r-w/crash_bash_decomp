#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/titles_ovl.h"
#include "bash/titles.h"

extern u8 GamePlayers;

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_800500B8[];
extern u8 D_80055614[];
extern u8 D_8005564C[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char TextAdventureHint[] = "complete each warp room to\nunlock further levels";
const char TextBattleHint[] = "select a battle\nfrom unlocked levels";
const char TextTournamentHint[] = "play a tournament";
const char TextOptionsHint[] = "change the options";
const char TextAdventureMode[] = "ADVENTURE MODE";
const char TextBattleMode[] = "BATTLE MODE";
const char TextTournament[] = "TOURNAMENT";
const char TextOptions[] = "OPTIONS";
const char TextEmpty[] = "";
const char TextSelectGameType[] = "SELECT GAME TYPE";
const char Text1Player[] = "1 PLAYER";
const char Text2Player[] = "2 PLAYER";
const char Text3Player[] = "3 PLAYER";
const char Text4Player[] = "4 PLAYER";
const char TextSelectNumberOfPlayers[] = "SELECT NUMBER OF PLAYERS";
const char TextNewGame[] = "NEW GAME";
const char TextLoadGame[] = "LOAD GAME";
const char TextCupsCount[] = "5  X";
const char TextSelectCups[] = "PRESS LEFT - RIGHT TO\nSELECT CUPS NEEDED\nTO WIN ROUND";
const char TextSelectDifficulty[] = "PRESS UP - DOWN TO\nSELECT DIFFICULTY";
const char TextBeginBattle[] = "BEGIN BATTLE";
const char TextGameOptions[] = "GAME OPTIONS";
const char TitlesAnimBreathe[] = "BREATHE";
const char TitlesAnimTauntA[] = "TAUNT_A";
const char TitlesAnimSlide[] = "SLIDE";
const char TitlesAnimWin[] = "WIN";
const char TitlesAnimWin2[] = "WIN2";
/* the backdrop's position (AnimateTitlesBackdrop), then the menus' texts */
const SLONG_XYZ TitlesBackdropPos = { -0x17C, 0x190, -0x64 };
const char TextEasy[] = "EASY";
const char TextMedium[] = "MEDIUM";
const char TextHard[] = "HARD";
const char TextPressButtonToStart[] = "PRESS \xFC BUTTON TO START";
const char TextExit[] = "EXIT";
const char TextSound[] = "SOUND";
const char TextAudioMode[] = "AUDIO MODE";
const char TextStereo[] = "<STEREO>";
const char TextMono[] = "< MONO >";
const char TextMusicVolume[] = "MUSIC VOLUME";
const char TextFxVolume[] = "FX VOLUME";
const char TextController[] = "CONTROLLER";
const char TextVibration[] = "VIBRATION";
const char TextOn[] = "ON";
const char TextOff[] = "OFF";
const char TextPressButtonToChange[] = "PRESS \xFF BUTTON TO CHANGE";
const char TextPressButtonToAccept[] = "PRESS \xFC BUTTON TO ACCEPT";
const char TextPercent100[] = "<100%>";
const char TextPercent90[] = "<090%>";
const char TextP1[] = "P1";
const char TextP2[] = "P2";
const char TextP3[] = "P3";
const char TextP4[] = "P4";
const char TextCharacterSelect[20] = "CHARACTER SELECT\x00\x14";
const char TextPressToUnlockLevels[] = "PRESS \xFD BUTTON\r14TO UNLOCK LEVELS";
const char TextNoMoreUnlocked[] = "THERE ARE NO MORE\nUNLOCKED LEVELS\nON THIS MEMORY CARD\n";
const char TextInsertCard[] = "INSERT MEMORY CARD\nWITH CRASH BASH DATA\nTO CHECK FOR\nUNLOCKED LEVELS";
const char TextLocked[] = "LOCKED";
const char TextChangeArena[] = "PRESS LEFT - RIGHT TO CHANGE ARENA";
const char TextChangeLevel[] = "PRESS UP - DOWN TO CHANGE LEVEL";
const char TextPressButtonToSelect[] = "PRESS \xFC BUTTON TO SELECT";
const char TextChooseLevel[] = "CHOOSE LEVEL";
const char TextCheckCardForSave[] = "PRESS \xFF TO\nCHECK MEMORY CARD\nFOR SAVED GAME";
const char TextCheckingCard[64] = "CHECKING MEMORY CARD\nIN MEMORY CARD SLOT 1\nFOR UNLOCKED\nLEVELS\x00\x14";
const char TextSelectBattleType[] = "SELECT BATTLE TYPE";
const char TextChangeBattleType[] = "PRESS LEFT - RIGHT TO CHANGE BATTLE TYPE";
const char TextStartBattle[] = "PRESS \xFC BUTTON TO START BATTLE";
const char TextVs[] = "VS";
const char TextVsBattle[] = "VS BATTLE";
const char TextTeamBattle[] = "TEAM BATTLE";
const char TextSelectSlot[] = "SELECT SLOT";

/* data, in address order */
s32 MainMenuHints[4] = { (s32)&TextAdventureHint, (s32)&TextBattleHint, (s32)&TextTournamentHint, (s32)&TextOptionsHint };
PlaceRec MainMenuPlace[9] = {
    { 4, -0x20, 0x12C, -0x46, 0x72, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x2C, 0, 0, 0, 0, 0, 0, (s32)&TextAdventureMode, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0xA, 0, 0, 0, 0, 0, 0, (s32)&TextBattleMode, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x18, 0, 0, 0, 0, 0, 0, (s32)&TextTournament, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x3A, 0, 0, 0, 0, 0, 0, (s32)&TextOptions, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x82, 0, 0, 0, 0, 0, 0, (s32)&TextEmpty, 4, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&TextSelectGameType, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec PlayersMenuPlace[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x42, 0, 0, 0, 0, 0, 0, (s32)&Text1Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x1C, 0, 0, 0, 0, 0, 0, (s32)&Text2Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 8, 0, 0, 0, 0, 0, 0, (s32)&Text3Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x2C, 0, 0, 0, 0, 0, 0, (s32)&Text4Player, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&TextSelectNumberOfPlayers, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec PlayersMenuAdventurePlace[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, (s32)&Text1Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, (s32)&Text2Player, 0, 0, 0, 0 },
    { 3, 0x3A, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x3A, 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&TextSelectNumberOfPlayers, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec NewLoadMenuPlace[6] = {
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&TextAdventureMode, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x20, 0x12C, -0x46, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, (s32)&TextNewGame, 2, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, (s32)&TextLoadGame, 3, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec BattleOptionsPlace[12] = {
    { 4, -0x8000, 0x190, -0x68, -0xA, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xA, 0xB4, -0x14, 0, 0xCCC, 0, 0x28, 0, 0, 0, 0, 0xC7, 0x2009, 2 },
    { 3, 0x3C, -0x46, 0, 0, 0, 0, 0, 0, (s32)&TextCupsCount, 0, 0, 0, 0 },
    { 3, -0xDC, -0x55, 0, 0, 0, 0, 0, 0, (s32)&TextSelectCups, 4, 0, 0, 0 },
    { 4, -0x8000, 0x190, 0x28, 0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x46, 0x37, 0, 0, 0, 0, 0, 0, (s32)&TextEmpty, 0, 0, 0, 0 },
    { 3, -0xDC, 0x32, 0, 0, 0, 0, 0, 0, (s32)&TextSelectDifficulty, 4, 0, 0, 0 },
    { 4, -0x8000, 0x258, 0x9A, 0xC0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0xA5, 0, 0, 0, 0, 0, 0, (s32)&TextBeginBattle, 4, 0, 0, 0 },
    { 4, -0x15E, 0x258, -0xD0, -0xA6, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0xCA, 0, 0, 0, 0, 0, 0, (s32)&TextGameOptions, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 TitlesFiles[9] = {
    (s32)&FontModel, (s32)&DatIndex[2 * 397], (s32)&DatIndex[2 * 396], (s32)&InterfaceModel, (s32)&DatIndex[2 * 395],
    (s32)&DatIndex[2 * 394], 0, 0, 0,
};
/* the engine's table at 0x800A0030 points at it for the front end's modes */
file_group_load_info file_group_TITLES = { D_800500B8, TitlesFiles };
s16 TitlesPreLoadDB[2] = { -1, 0 };
char *TitlesAnims[6] = {
    (char *)TitlesAnimBreathe, (char *)TitlesAnimTauntA, (char *)TitlesAnimSlide, (char *)TitlesAnimWin,
    (char *)TitlesAnimWin2, NULL,
};
s16 TitlesLevelDB[6] = { 0xC7, 0xC8, 0xC9, 0x1C, -1, 0 };
WWInf TitlesWorldInfo[3] = {
    { 0xC5, TitlesAnims, TitlesLevelDB, 3 },
    { 0xC5, TitlesAnims, TitlesLevelDB, 3 },
    { 0xC5, TitlesAnims, TitlesLevelDB, 3 },
};
shell_setup titles_shell_info = {
    0x1800, 0x10, 0x100, 0x40, 0x100, 0x40, D_80063EC8, D_80063BB8, 0x20, 0x76C,
};
u16 TitlesLUPtab[4] = { 3, 0x20D, 0x1001, 0x1002 };
TrigLookUp TitlesLUP = { TitlesLUPtab, 4 };
PreMap TITLES_PREMAP = { TitlesPreLoadDB, TitlesWorldInfo, &TitlesLUP };
Ext400 TitlesBackdropAtable = {
    {
        (s32)0x80110000, 0x40000016, 0x168011, (s32)0x80114000, 0x40000016, 0x168011,
        (s32)0x80114000, 0x40800038, 0x168011, (s32)0x80114000, 0x40000016,
    },
};
s32 TitlesBackdropSteps[11] = {
    0x388011, (s32)0x80114100, 0x41800054, 0x168011, (s32)0x80114000, 0x40000016, 0x388011,
    (s32)0x80004100, 0, (s32)&TitlesAnimBreathe, 0,
};
PlaceRec TitlesBackdropPlace[9] = {
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
u16 TitlesDemoModes[8] = { 0xA, 0, 0xE, 0, 0xA, 0, 0xE, 0 };
GfxViewport TitlesMenuView = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x10, 0x1EF, 0, 0, 0, 0, 0 };
GfxViewport TitlesBackdropView = { 0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0, 0x140, 0xF0, 0x800, 0x7FF, -0x80, 0, 0, 0, 1 };
process_info MainMenu = { MainMenuInit, MainMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info PlayersMenu = { PlayersMenuInit, PlayersMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info NewLoadMenu = { NewLoadMenuInit, NewLoadMenuUpdate, RenderTitlesMenu, 0, 0 };
process_info TitlesLeave = { TitlesLeaveInit, 0, TitlesLeaveRender, 0, 0 };
s32 TextDifficulties[1][3] = {
    {
        (s32)&TextEasy, (s32)&TextMedium,
        (s32)&TextHard,
    },
};
s32 TextBattleKinds[1][2] = { { (s32)&TextPressButtonToStart, (s32)&TextPressButtonToStart } };
process_info BattleOptions = { BattleOptionsInit, BattleOptionsUpdate, BattleOptionsRender, BattleOptionsQuit, 0 };
/* The menu chains MenuChainNext/Back walk (MenuChainPos): each starts at its second entry, so
 * backing out of the first screen reaches the main menu before it. A Resume pointer is where a
 * finished battle or tournament picks up again (TitlesInitLevel, modes 0x29 and 0x2A). */
process_info *BattleChain[6] = {
    &MainMenu, &PlayersMenu, &CharSelect, &LevelSelect, &TeamMode, &BattleOptions,
};
process_info **BattleChainResume = &BattleChain[3];
process_info *AdventureChain[6] = {
    &MainMenu, &NewLoadMenu, &PlayersMenu, &CharSelect, &TitlesLeave, &MainMenu,
};
process_info *TournamentChain[4] = { &PlayersMenu, &CharSelect, &TournamentSelect, &BattleOptions };
process_info **TournamentChainResume = &TournamentChain[2];
s32 LangOptions[1] = { (s32)&TextOptions };
s32 LangExit[1] = { (s32)&TextExit };
s32 LangSound[1] = { (s32)&TextSound };
s32 LangAudioMode[1] = { (s32)&TextAudioMode };
s32 LangStereo[1] = { (s32)&TextStereo };
s32 LangMono[1] = { (s32)&TextMono };
s32 LangMusicVolume[1] = { (s32)&TextMusicVolume };
s32 LangFxVolume[1] = { (s32)&TextFxVolume };
s32 LangController[1] = { (s32)&TextController };
s32 LangVibration[1] = { (s32)&TextVibration };
s32 LangOn[1] = { (s32)&TextOn };
s32 LangOff[1] = { (s32)&TextOff };
s32 LangPressButtonToChange[1] = { (s32)&TextPressButtonToChange };
s32 LangPressButtonToAccept[1] = { (s32)&TextPressButtonToAccept };
PlaceRec OptionsPlace[7] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&LangOptions, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x5A, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x32, 0, 0, 0, 0, 0, 0, (s32)&LangSound, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xA, 0, 0, 0, 0, 0, 0, (s32)&LangController, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x1E, 0, 0, 0, 0, 0, 0, (s32)&LangExit, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec OptionsSoundPlace[12] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&LangSound, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xFA, 0xFA, -0x40, 0x8C, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0xD2, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&LangAudioMode, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&LangStereo, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, (s32)&LangMono, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0xA, 0, 0, 0, 0, 0, 0, (s32)&LangMusicVolume, 0, 0, 0, 0 },
    { 3, 0x46, 0xA, 0, 0, 0, 0, 0, 0, (s32)&TextPercent100, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0x32, 0, 0, 0, 0, 0, 0, (s32)&LangFxVolume, 0, 0, 0, 0 },
    { 3, 0x46, 0x32, 0, 0, 0, 0, 0, 0, (s32)&TextPercent90, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, (s32)&LangPressButtonToAccept, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec OptionsControllerPlace[23] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, (s32)&LangController, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x82, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x3C, 0, 0, 0, 0, 0, 0, (s32)&LangVibration, 0, 0, 0, 0 },
    { 0xE, -0x8F, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOn, 7, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 0, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 1, 0, 0, 0 },
    { 0xE, -0x35, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOn, 7, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 0, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 1, 0, 0, 0 },
    { 0xE, 0x25, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOn, 7, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 0, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 1, 0, 0, 0 },
    { 0xE, 0x75, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOn, 7, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 0, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, (s32)&LangOff, 1, 0, 0, 0 },
    { 3, -0x87, -0xF, 0, 0, 0, 0, 0, 0, (s32)&TextP1, 4, 0, 0, 0 },
    { 3, -0x2D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&TextP2, 4, 0, 0, 0 },
    { 3, 0x2D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&TextP3, 4, 0, 0, 0 },
    { 3, 0x7D, -0xF, 0, 0, 0, 0, 0, 0, (s32)&TextP4, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x3C, 0, 0, 0, 0, 0, 0, (s32)&LangPressButtonToChange, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, (s32)&LangPressButtonToAccept, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 OptionsCursor = 0;
s32 OptionsPage = 0;
process_info OptionsScreen = { OptionsInit, OptionsUpdate, OptionsRender, OptionsQuit, 0 };
process_info RunGame = { RunGameInit, RunGameUpdate, RunGameRender, RunGameQuit, 0 };
GfxCamera TitlesCamera = {0};
s32 MainMenuCursor = 0;
Ext400 TitlesBackdropAnimator = {0};
process_info **MenuChainPos = 0;
s32 MainMenuHintTimer = 0;
s32 NewLoadMenuCursor = 0;
s32 BattleOptionsState = 0;
s32 LangCharacterSelect[1] = { (s32)&TextCharacterSelect };
PlaceRec CharSelectPlace[27] = {
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
    { 0xE, -0x8000, 0x116, 0, 0, 0, 0, 0, 0, (s32)&LangCharacterSelect, 0, 0, 0, 0 },
    { 4, -0xA, 0x28A, 0x112, 0x136, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0, 0x2BC, 0x28, 0x6E, 7, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0, 0x2BC, 0x8C, 0xD2, 8, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 0x50, 0x6E, 0, 0x4CC, 0, 0, 0, 0, 0, 0, 0xC7, 0x200B, 0 },
    { 6, 0x230, 0xBE, 0, 0x4CC, 0, 0, 0, 0, 0, 0, 0xC7, 0x200C, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec CharSelectModelsPlace[9] = {
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
CharCard CharCards[8] = {
    { 0, 1, { 0 }, 0x8E, 0x20, 0xCCCC, 0xC7, 0, (s32)&CharSelectSteps2, 0x38E, 0 },
    { 0, 2, { 0 }, 0xE8, 0x20, 0xCCCC, 0xC7, 0, (s32)&CharSelectSteps1, 0x38F, 0 },
    { 0, 5, { 0 }, 0x142, 0x20, 0x9999, 0xC7, 0, (s32)&CharSelectSteps6, 0x394, 0 },
    { 0, 7, { 0 }, 0x19C, 0x20, 0x9999, 0xC7, 0, (s32)&CharSelectSteps4, 0x392, 0 },
    { 0, 3, { 0 }, 0x8E, 0x84, 0xCCCC, 0xC7, 0, (s32)&CharSelectSteps3, 0x391, 0 },
    { 0, 4, { 0 }, 0xE8, 0x84, 0x9999, 0xC7, 0, (s32)&CharSelectSteps0, 0x390, 0 },
    { 0, 6, { 0 }, 0x142, 0x84, 0x9999, 0xC7, 0, (s32)&CharSelectAnim7, 0x395, 0 },
    { 0, 8, { 0 }, 0x19C, 0x84, 0x9999, 0xC7, 0, (s32)&CharSelectSteps5, 0x393, 0 },
};
TitlesOff CharSelectorOffsets[4] = { { 7, 4 }, { 0x2D, 4 }, { 7, 0x30 }, { 0x2D, 0x30 } };
s32 CharPickSfx[4] = { 0x389, 0x38A, 0x38B, 0x38C };
s32 CharMoveSfx[4] = { 0x385, 0x386, 0x387, 0x388 };
s8 AdventureHeadsSolo[48] = {
    3, 4, 6, 6, 8, 8, 3, 4, 6, 6, 8, 8, 1, 2, 5, 5, 7, 7, 1, 2, 5, 5, 7, 7, 3, 4, 6, 6, 8, 8,
    1, 2, 5, 5, 7, 7, 3, 4, 6, 6, 8, 8, 1, 2, 5, 5, 7, 7,
};
s8 AdventureHeadsTeam[256] = {
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
process_info CharSelect = { CharSelectInit, CharSelectUpdate, CharSelectRender, 0, 0 };
s32 CharSelectSteps0[3] = { (s32)0x80110000, 0x43800020, 0x8000 };
s32 CharSelectAnim0[6] = { (s32)&CharSelectSteps0, 0, 0, 0, (s32)&CharSelectAnim0, 0x401 };
s32 CharSelectSteps1[3] = { (s32)0x80110000, 0x42000014, 0x8000 };
s32 CharSelectAnim1[6] = { (s32)&CharSelectSteps1, 0, 0, 0, (s32)&CharSelectAnim1, 0x401 };
s32 CharSelectSteps2[3] = { (s32)0x80110000, 0x42800016, 0x8000 };
s32 CharSelectAnim2[6] = { (s32)&CharSelectSteps2, 0, 0, 0, (s32)&CharSelectAnim2, 0x401 };
s32 CharSelectSteps3[3] = { (s32)0x80110000, 0x43000020, 0x8000 };
s32 CharSelectAnim3[6] = { (s32)&CharSelectSteps3, 0, 0, 0, (s32)&CharSelectAnim3, 0x401 };
s32 CharSelectSteps4[3] = { (s32)0x80110000, 0x44000016, 0x8000 };
s32 CharSelectAnim4[6] = { (s32)&CharSelectSteps4, 0, 0, 0, (s32)&CharSelectAnim4, 0x401 };
s32 CharSelectSteps5[3] = { (s32)0x80110000, 0x44800014, 0x8000 };
s32 CharSelectAnim5[6] = { (s32)&CharSelectSteps5, 0, 0, 0, (s32)&CharSelectAnim5, 0x401 };
s32 CharSelectSteps6[3] = { (s32)0x80110000, 0x45000020, 0x8000 };
s32 CharSelectAnim6[6] = { (s32)&CharSelectSteps6, 0, 0, 0, (s32)&CharSelectAnim6, 0x401 };
s32 CharSelectAnim7[6] = { (s32)0x80110000, 0x45800020, 0x8000, (s32)&CharSelectAnim7, 0, 0 };
GfxCamera CharSelectCamera = {0};
TitlesSlot CharSelectors[4] = {0};
s32 LangPressToUnlockLevels[1] = { (s32)&TextPressToUnlockLevels };
s32 LangNoMoreUnlocked[1] = { (s32)&TextNoMoreUnlocked };
s32 LangInsertCard[1] = { (s32)&TextInsertCard };
s32 LangLocked[1] = { (s32)&TextLocked };
s32 BattleLevelsBash[16] = {
    1, 0, (s32)&D_80055614, 0x2005, 1, 1, (s32)&D_80055614, 0x2006, 1, 2, (s32)&D_80055614,
    0x2007, 1, 3, (s32)&D_80055614, 0x2008,
};
s32 BattleLevelsPolar[16] = {
    1, 0x13, (s32)&D_8005564C, 0x2005, 1, 0x12, (s32)&D_8005564C, 0x2006, 1, 0x14,
    (s32)&D_8005564C, 0x2007, 1, 0x15, (s32)&D_8005564C, 0x2008,
};
s32 BattleLevelsPogo[16] = {
    1, 0xA, (s32)&D_8005564C, 0x2001, 1, 0xB, (s32)&D_8005564C, 0x2002, 1, 0xC,
    (s32)&D_8005564C, 0x2003, 1, 0xD, (s32)&D_8005564C, 0x2004,
};
s32 BattleLevelsArkenoid[16] = {
    1, 0xE, (s32)&D_80055614, 0x2001, 1, 0xF, (s32)&D_80055614, 0x2002, 1, 0x10,
    (s32)&D_80055614, 0x2003, 1, 0x11, (s32)&D_80055614, 0x2004,
};
s32 BattleLevelsTank[16] = {
    2, 6, (s32)&D_8005564C, 0x2009, 2, 8, (s32)&D_8005564C, 0x200A, 2, 7, (s32)&D_8005564C,
    0x200B, 2, 5, (s32)&D_8005564C, 0x200C,
};
s32 BattleLevelsRace[16] = {
    2, 0x17, (s32)&D_80055614, 0x2009, 2, 0x18, (s32)&D_80055614, 0x200A, 2, 0x1A,
    (s32)&D_80055614, 0x200B, 2, 0x19, (s32)&D_80055614, 0x200C,
};
s32 BattleLevelsDino[16] = {
    2, 0x1B, (s32)&D_80055614, 0x200D, 2, 0x1C, (s32)&D_80055614, 0x200E, 2, 0x1D,
    (s32)&D_80055614, 0x200F, 2, 0x1E, (s32)&D_80055614, 0x2010,
};
BattleArena BattleArenas[7] = {
    { (s32)&BattleLevelsBash, 4 }, { (s32)&BattleLevelsPolar, 4 }, { (s32)&BattleLevelsPogo, 4 },
    { (s32)&BattleLevelsArkenoid, 4 }, { (s32)&BattleLevelsTank, 4 }, { (s32)&BattleLevelsRace, 4 },
    { (s32)&BattleLevelsDino, 4 },
};
PlaceRec LevelSelectPlace0[2] = {
    { 7, 0, 0, 0, 0xB33, 0, 0, 0, 0, 0, 0, 0xC8, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec LevelSelectPlace1[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2005, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec LevelSelectPlace2[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2006, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec LevelSelectPlace3[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2007, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec LevelSelectPlace4[2] = {
    { 7, 0, 0, 0, 0x666, 0, 0, 0, 0, 0, 0, 0xC8, 0x2008, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 LangChangeArena[1] = { (s32)&TextChangeArena };
s32 LangChangeLevel[1] = { (s32)&TextChangeLevel };
s32 LangPressButtonToSelect[1] = { (s32)&TextPressButtonToSelect };
s32 LangChooseLevel[1] = { (s32)&TextChooseLevel };
PlaceRec LevelSelectPlace[11] = {
    { 3, 0x7FA1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 0xD4, 0, 0, 0x266, 0, 0, 0, 0, 0, 0, 0xC7, 0x2008, 0 },
    { 0xE, 0x7FA1, -0x96, 0, 0, 0, 0, 0, 0, (s32)&LangChangeArena, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x42, 0, 0, 0, 0, 0, 0, (s32)&LangChangeLevel, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x28, 0, 0, 0, 0, 0, 0, (s32)&LangPressButtonToSelect, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0xC8, 0, 0, 0, 0, 0, 0, (s32)&LangChooseLevel, 0, 0, 0, 0 },
    { 3, 0x7FA1, -0x6E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x14A, 0x88, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x120, 0x28A, -0xCC, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x7FA1, 0x4E, 0, 0, 0, 0, 0, 0, (s32)&TextCheckCardForSave, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 LangCheckingCard[1] = { (s32)&TextCheckingCard };
GfxViewport LevelSelectView0 = { 0x28, 0xEE, 0x19A, 0x1B0, 0, 0, 0x280, 0x1E0, 0xE1, 0x198, 0x200, 0x1FF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView1 = { 0x1CC, 0x24, 0x25C, 0x84, 0, 0, 0x280, 0x1E0, 0x214, 0x88, 0x400, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView2 = { 0x1CC, 0x88, 0x25C, 0xE8, 0, 0, 0x280, 0x1E0, 0x214, 0xEC, 0x4CC, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView3 = { 0x1CC, 0xEC, 0x25C, 0x14C, 0, 0, 0x280, 0x1E0, 0x214, 0x150, 0x600, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView4 = { 0x1CC, 0x150, 0x25C, 0x1B0, 0, 0, 0x280, 0x1E0, 0x214, 0x1B4, 0x700, 0xFF, 0, 0, 0, 0, 0 };
process_info LevelSelect = { LevelSelectInit, LevelSelectUpdate, LevelSelectRender, LevelSelectQuit, 0 };
s32 LevelSelectBusy = 0;
s32 LevelSelectReload = 0;
s32 LangSelectBattleType[1] = { (s32)&TextSelectBattleType };
s32 LangChangeBattleType[1] = { (s32)&TextChangeBattleType };
s32 LangStartBattle[1] = { (s32)&TextStartBattle };
PlaceRec TeamModePlace[19] = {
    { 6, -0xFA, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, -0x6A, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0x38, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0xDC, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 4, -0x122, -0xAA, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x88, -0x10, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x10, 0x88, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0xAA, 0x122, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0xAA, -0x46, 0, 0, 0, 0, 0, 0, (s32)&TextVs, 0, 0, 0, 0 },
    { 3, -0x8000, -0x46, 0, 0, 0, 0, 0, 0, (s32)&TextVs, 0, 0, 0, 0 },
    { 3, 0x88, -0x46, 0, 0, 0, 0, 0, 0, (s32)&TextVs, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xC8, 0, 0, 0, 0, 0, 0, (s32)&LangSelectBattleType, 0, 0, 0, 0 },
    { 4, -0x8000, 0x258, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0x42, 0, 0, 0, 0, 0, 0, (s32)&TextVsBattle, 2, 0, 0, 0 },
    { 4, -0x78, 0x78, 0x3A, 0x6C, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x8C, 0, 0, 0, 0, 0, 0, (s32)&LangChangeBattleType, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0xA6, 0, 0, 0, 0, 0, 0, (s32)&LangStartBattle, 4, 0, 0, 0 },
    { 4, -0x10E, 0x10E, 0x87, 0xBE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 LangVsBattle[1] = { (s32)&TextVsBattle };
s32 LangTeamBattle[1] = { (s32)&TextTeamBattle };
TitlesPreset TeamModePresets[2] = {
    {
        { 0, 1, 2, 3 }, { 0x8000, 0x8000, 0x8000 }, { -0x122, -0x88, 0x10, 0xAA },
        (void *)&LangVsBattle, 0, 0, 2, -1, -1, -1, -1,
    },
    {
        { 0, 1, 2, 3 }, { 0, 0x8000, 0 }, { -0x116, -0x96, 0x1F, 0x9E }, (void *)&LangTeamBattle,
        1, 0, 2, 1, 0, 3, 2,
    },
};
u16 TeamModeHeroFrames[10] = { 0, 0x306B, 0x3067, 0x3069, 0x307B, 0x3073, 0x3071, 0x306D, 0x306F, 0 };
process_info TeamMode = { team_mode_init, TeamModeUpdate, RenderTitlesBackdropAndMenu, 0, 0 };
s32 TeamModeRow = 0;
s32 TeamModePreset = 0;
PlaceRec TitlesExtraPlace[3] = {
    { 4, 0, 0x64, 0, 0x64, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0, 0, 0, 0, 0, 0, 0, 0, (s32)&TextSelectSlot, 2, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/* two screens nothing in the front end chains to (the engine's own menus may) */
process_info TitlesExtraScreen = { TitlesExtraScreenInit, 0, TitlesExtraScreenRender, 0, 0 };
process_info TitlesShellScreen = { TitlesShellScreenInit, TitlesShellScreenUpdate, TitlesShellScreenRender, 0, 0 };

void TitlesInitLevel(void) {
    u8 *g;
    process_info **p;
    u8 *a1;

    func_8009029C();
    g = GameInfo;
    g[0x62] = 0;
    cam_init((u8 *)&TitlesCamera);
    *(s32 *)((u8 *)&TitlesCamera + 8) = 0x400;
    *(s32 *)((u8 *)&TitlesCamera + 0x18) = 0x190;
    func_80095BEC(1, (s32 *)TitlesBackdropPlace);
    GetEngineSlot(1)->sub[4].flags &= ~0x8000;
    GetEngineSlot(1)->sub[5].flags &= ~0x8000;
    GetEngineSlot(1)->sub[6].flags &= ~0x8000;
    GetEngineSlot(1)->sub[7].flags &= ~0x8000;
    GetEngineSlot(1)->sub[3].flags &= ~0x8000;
    joypad_idle_count = 0;
    switch (*(s16 *)(g + 0x34)) {
    case 0x28:
        core_process = 0;
        func_80092C54();
        *(u16 *)(g + 0x32) |= 0x200;
        process_control_init(&MenuProcessControl, &MainMenu);
        break;
    case 0x29:
        p = BattleChainResume;
        g[0x27] = 4;
        a1 = (u8 *)*p;
        MenuChainPos = p;
        process_control_init(&MenuProcessControl, (process_info *)a1);
        break;
    case 0x2A:
        p = TournamentChainResume;
        g[0x27] = 4;
        a1 = (u8 *)*p;
        MenuChainPos = p;
        process_control_init(&MenuProcessControl, (process_info *)a1);
        break;
    }
    TitlesBackdropAnimator = ExtTemplate400;
    TitlesBackdropAnimator.w[3] = (s32)&TitlesBackdropAtable;
}

void TitlesCheckIdle(void) {
    s32 f;
    if (joypad_idle_count >= 0x385) {
        joypad_idle_count = 0;
        set_fade(&ScreenFade, 0x200, 0);
        f = D_8009F654;
        if (f & 1) {
            GameMode = 0x28;
            func_80094D88((s32)D_800A0840, &LevelLoadProcess);
        } else {
            u8 *g = GameInfo;
            u8 *t = (u8 *)TitlesDemoModes;
            u16 v = *(u16 *)(t + ((f * 2) & 0xC));
            *(s32 *)(g + 0x68) = 1;
            *(s32 *)(g + 0x14) = 0x384;
            ProcessControl.next_process = &LevelLoadProcess;
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
    sub->unk14 = FrameCount * 0x14;
    sub->unk12 = 0x293;
    sub->unk50 = 1;
    sub->unk51 = 0;
    sub->unk20.z = 0x2800;
    sub->unk20.x = 0x2800;
    sub->unk52 = 1;
    eng = GetEngineSlot(1);
    tbl = rcossin_tbl;
    pos = TitlesBackdropPos;
    d = GameFrameCount;
    pos.x += (tbl[(((d * 0x14) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x8F) >> 0xF;
    pos.y += (tbl[(((d * 0x2F) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x17) >> 0xC;
    ang = (tbl[(((d * 0x13) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x333) >> 0x10;
    p2 = &eng->sub[1];
    *(SLONG_XYZ *)&p2->unk04 = pos;
    p2->unk12 = -0x1C7;
    p2->unk51 = 0;
    p2->unk14 = ang;
    p2->unk52 = 1;
    DoAnimCommand(NULL, eng, p2, &TitlesBackdropAnimator);
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
    if (Pads[0].pressed & 0x40) {
        if (MainMenuCursor < 3) {
            MainMenuCursor++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        MainMenuHintTimer = 0x5A;
    }
    if (Pads[0].pressed & 0x10) {
        if (MainMenuCursor > 0) {
            MainMenuCursor--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        MainMenuHintTimer = 0x5A;
    }
    if (Pads[0].pressed & 0x4000) {
        g = GameInfo;
        ((u16 *)g)[0x19] &= 0xFF00;
        switch (MainMenuCursor) {
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
            MenuChainStart(&AdventureChain[1]);
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
            MenuChainStart(&BattleChain[1]);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            g[0x27] = 4;
            *(u16 *)(g + 0x32) |= 2;
            MenuChainStart(TournamentChain);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            process_interrupt(&MenuProcessControl, &OptionsScreen);
            break;
        }
    }
    s[1].unk7C = 0;
    s[2].unk7C = 0;
    s[4].unk7C = 0;
    s[3].unk7C = 0;
    if (MainMenuHintTimer != 0) {
        s[5].flags |= 0x8000;
        MainMenuHintTimer--;
        s[5].unk6C = (void *)MainMenuHints[MainMenuCursor];
    } else {
        s[5].flags &= ~0x8000;
    }
    s[MainMenuCursor + 1].unk7C = 2;
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
    n = PadRunEnd;
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
    if (Pads[0].pressed & 0x10) {
        if (O69CV->unk26 >= 2) {
            O69CV->unk26--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if ((Pads[0].pressed & 0x40) && O69CV->unk26 < n) {
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
    TitlesSub *s;
    s32 *g;

    AnimateTitlesBackdrop();
    do {
        s = (TitlesSub *)GetEngineSlot(0)->sub;
        s->a = (NewLoadMenuCursor == 0) * 2;
        s->b = (NewLoadMenuCursor == 1) * 2;
        g = &Pads[0].pressed;
    } while (0);
    if ((*g & 0x10) && (NewLoadMenuCursor > 0)) {
        NewLoadMenuCursor -= 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((Pads[0].pressed & 0x40) && (NewLoadMenuCursor <= 0)) {
        NewLoadMenuCursor += 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (Pads[0].pressed & 0x4000) {
        switch (NewLoadMenuCursor) {
        case 0:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
            break;
        case 1:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            func_80092AF4(&SaveScreenProcess2, 0x200);
            break;
        }
    }
    if (Pads[0].pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void BattleOptionsUpdate(void) {
    subengine *sub;
    subengine *p;

    sub = GetEngineSlot(1)->sub;
    sub->unk14 = FrameCount * 0x14;
    sub->unk12 = 0x293;
    sub->unk50 = 1;
    sub->unk20.z = 0x2800;
    sub->unk20.x = 0x2800;
    sub->unk51 = 0;
    sub->unk52 = 1;
    if (((PadButtons *)Pads)->pressed & 0x1000) {
        MenuChainBack();
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (((PadButtons *)Pads)->pressed & 0x80) {
        if (O69OV->unk3A >= 3) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            O69OV->unk3A--;
        }
    }
    if ((Pads[0].pressed & 0x20) && O69OV->unk3A < 7) {
        O69OV->unk3A++;
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((Pads[0].pressed & 0x40) && O69OV->unk38 > 0) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->unk38--;
    }
    if ((Pads[0].pressed & 0x10) && O69OV->unk38 < 2) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->unk38++;
    }
    *(u8 *)GetEngineSlot(0)->sub[2].unk6C = O69OV->unk3A + '0';
    GetEngineSlot(0)->sub[5].unk6C = (void *)TextDifficulties[O69OV->unk2C][O69OV->unk38];
    p = &GetEngineSlot(0)->sub[8];
    if (O69OV->unk32 & 1) {
        p->unk6C = (void *)TextBattleKinds[O69OV->unk2C][0];
    } else {
        p->unk6C = (void *)TextBattleKinds[O69OV->unk2C][1];
    }
    if (Pads[0].pressed & 0x4000) {
        GameExitFlags = 0x8008;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void BattleOptionsRender(void) {
    u8 *p;

    RenderState.modelDp = ScreenFade.amount;
    p = (u8 *)GetEngineSlot(1)->sub;
    *(s32 *)(p + 0xA8) &= 0xFFFF7FFF;
    p = (u8 *)GetEngineSlot(1)->sub;
    *(s32 *)(p + 0x150) &= 0xFFFF7FFF;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(1));
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(0));
}

void TitlesSetHeroMasks(void) {
    u8 *g = GameInfo;
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
    cam->unk14 = FrameCount * 0x14;
    cam->unk12 = 0x293;
    cam->unk50 = 1;
    cam->unk20.z = 0x2800;
    cam->unk20.x = 0x2800;
    cam->unk51 = 0;
    cam->unk52 = 1;
    sub = GetEngineSlot(4)->sub;
    switch (OptionsPage) {
    case 0:
        sub[OptionsCursor + 3].unk7C = 0;
        if ((((PadButtons *)Pads)->pressed & 0x40) && OptionsCursor < 2) {
            OptionsCursor++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((Pads[0].pressed & 0x10) && OptionsCursor > 0) {
            OptionsCursor--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        sub[OptionsCursor + 3].unk7C = 2;
        if (((PadButtons *)Pads)->pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            switch (OptionsCursor) {
            case 0:
                OptionsPage = 1;
                func_80095BEC(4, (s32 *)OptionsSoundPlace);
                break;
            case 1:
                OptionsPage = 2;
                func_80095BEC(4, (s32 *)OptionsControllerPlace);
                break;
            case 2:
                process_control_back(&MenuProcessControl);
                break;
            }
            OptionsCursor = 0;
        }
        if (Pads[0].pressed & 0x1000) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            process_control_back(&MenuProcessControl);
        }
        return;
    case 1:
        if ((Pads[0].pressed & 0x40) && OptionsCursor < 2) {
            OptionsCursor++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((Pads[0].pressed & 0x10) && OptionsCursor > 0) {
            OptionsCursor--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        sub[3].unk7C = 0;
        sub[4].unk7C = 0;
        sub[5].unk7C = 0;
        sub[8].unk7C = 0;
        sub[9].unk7C = 0;
        sub[6].unk7C = 0;
        sub[7].unk7C = 0;
        switch (OptionsCursor) {
        case 0:
            if (Pads[0].pressed & 0xA0) {
                s32 *opt = (s32 *)GameOptions;

                opt[2] ^= 1;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                if (opt[2] != 0) {
                    sound_set_stereo();
                } else {
                    sound_set_mono();
                }
            }
            sub[3].unk7C = 2;
            sub[4].unk7C = 2;
            sub[5].unk7C = 2;
            break;
        case 1:
            v = *(s32 *)GameOptions;
            if (v > 0 && (Pads[0].pressed & 0x80)) {
                v -= 5;
            }
            if (v < 100 && (Pads[0].pressed & 0x20)) {
                v += 5;
            }
            *(s32 *)GameOptions = v;
            sub[6].unk7C = 2;
            sub[7].unk7C = 2;
            break;
        case 2:
            v = D_8005A608;
            if (v > 0 && (Pads[0].pressed & 0x80)) {
                v -= 5;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (v < 100 && (Pads[0].pressed & 0x20)) {
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
        n = PadCount;
        if (n >= 5) {
            n = 4;
        }
        for (i = 0; i < n; i++) {
            if (((PadButtons *)Pads)[i].pressed & 0x8000) {
                ((TitlesOpt *)GameOptions)->unkC[i] ^= 1;
                pad_rumble_small(i, 0xA);
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        break;
    default:
        return;
    }
    if (Pads[0].pressed & 0x5000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        OptionsPage = 0;
        OptionsCursor = 0;
        func_80095BEC(4, (s32 *)OptionsPlace);
    }
}

void OptionsRender(void) {
    subengine *sub;
    s32 n;
    s32 i;

    sub = GetEngineSlot(4)->sub;
    RenderState.modelDp = ScreenFade.amount;
    switch (OptionsPage) {
    case 1:
        func_8008D488(sub[7].unk6C, *(s32 *)GameOptions);
        if (((s32 *)GameOptions)[2] != 0) {
            sub[5].flags &= ~0x8000;
            sub[4].flags |= 0x8000;
        } else {
            sub[5].flags |= 0x8000;
            sub[4].flags &= ~0x8000;
        }
        func_8008D488(sub[9].unk6C, D_8005A608);
        break;
    case 2:
        n = PadCount;
        if (n >= 5) {
            n = 4;
        }
        for (i = 0; i < n; i++) {
            if (Pads[i].unk0 & 8) {
                sub[i * 3 + 5].unk7C = 0;
                if (GameOptions[i + 0xC] != 0) {
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
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    eng_render_subs((u8 *)GetEngineSlot(4));
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(1));
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
    init_scripts(0x20, 0x20);
    if (D_8009E58C != NULL) {
        D_8009E58C();
    }
    sound_init();
    music_load_track(GameMode);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    func_8008EC00(&DatIndex[2 * 139]);
    sfx_table_clear();
    sfx_bank_load((s32)&DatIndex[2 * 274]);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
}

void OptionsQuit(void) {
}

void OptionsInit(void) {
    func_80095BEC(4, (s32 *)OptionsPlace);
    OptionsCursor = 0;
    OptionsPage = 0;
}

void MenuChainStart(process_info **chain) {
    MenuChainPos = chain;
    process_control_goto(&MenuProcessControl, *chain);
}

void MenuChainStep(s32 n) {
    process_info **p;
    process_info *next;

    p = &MenuChainPos[n];
    next = *p;
    MenuChainPos = p;
    if (next != NULL) {
        process_control_goto(&MenuProcessControl, next);
    }
}

void MenuChainBack(void) {
    process_info **p;
    process_info *next;

    p = MenuChainPos - 1;
    next = *p;
    MenuChainPos = p;
    if (next != NULL) {
        process_control_goto(&MenuProcessControl, next);
    }
}

void MenuChainNext(void) {
    process_info **p;
    process_info *next;

    p = MenuChainPos;
    next = p[1];
    MenuChainPos = p + 1;
    if (next != NULL) {
        process_control_goto(&MenuProcessControl, next);
    }
}

void BattleOptionsQuit(void) {
}

void BattleOptionsInit(void) {
    func_80095BEC(0, (s32 *)BattleOptionsPlace);
    BattleOptionsState = 2;
}

void TitlesLeaveRender(void) {
    if (CharSelect.render != NULL) {
        CharSelect.render();
    }
}

void TitlesLeaveInit(void) {
    set_fade(&ScreenFade, 0x200, 0);
    func_80094D88((s32)D_800A0840, &D_8009E5C8);
}

void NewLoadMenuInit(void) {
    u8 *g;
    func_80095BEC(0, (s32 *)NewLoadMenuPlace);
    NewLoadMenuCursor = 0;
    MainMenuCursor = 0;
    g = GameInfo;
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
    RenderState.modelDp = ScreenFade.amount;
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    eng_render_subs((u8 *)GetEngineSlot(0));
}


void PlayersMenuInit(void) {
    if (GameLevelFlags & 4) {
        func_80095BEC(0, (s32 *)PlayersMenuAdventurePlace);
    } else {
        func_80095BEC(0, (s32 *)PlayersMenuPlace);
    }
    GamePlayers = PadRunEnd != 0;
}

void MainMenuInit(void) {
    u8 *g;
    MainMenuCursor = 0;
    func_80092C54();
    g = GameInfo;
    *(s16 *)(g + 0x34) = 0x28;
    MainMenuHintTimer = 0;
    *(u16 *)(g + 0x32) |= 0x200;
    func_80095BEC(0, (s32 *)MainMenuPlace);
}

void RenderTitlesBackdropAndMenu(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(1));
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(0));
}

void RenderTitlesBackdrop(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(1));
}

void RenderTitlesMenu(void) {
    GetEngineSlot(1)->sub[1].flags |= 0x8000;
    GetEngineSlot(1)->sub[2].flags |= 0x8000;
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(0));
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(1));
}

void SetupTitlesBackdrop(void) {
    PlaceSub *s = (PlaceSub *)GetEngineSlot(1)->sub;

    s->unk14 = FrameCount * 20;
    s->unk12 = 0x293;
    s->unk50 = 1;
    s->sz = 0x2800;
    s->sx = 0x2800;
    s->unk51 = 0;
    s->unk52 = 1;
}

void MenuConfirmOrBack(void) {
    if (((PadButtons *)Pads)->pressed & 0x4000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainNext();
    }
    if (((PadButtons *)Pads)->pressed & 0x1000) {
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
    GameViewport.ofy = 0xF0;
}

void TitlesEmptyHook(void) {
}

void TitlesUpdateLevel(void) {
    combo_check_all();
}

void TitlesPreInitLevel(void) {
    u8 *g = GameInfo;

    g[0x50] = 9;
    g[0x51] = 9;
    g[0x52] = 9;
    g[0x53] = 9;
    *(u16 *)(g + 0x32) |= 0x200;
}

void TitlesSetUp(void) {
    LevelSpecs.LevelPreInit = TitlesPreInitLevel;
    LevelSpecs.LevelInit = TitlesInitLevel;
    LevelSpecs.LevelRestartInit = TitlesInitLevel;
    LevelSpecs.LevelRoutine = TitlesUpdateLevel;
    LevelSpecs.EarliestTriggers = 0;
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    RenderHook = TitlesEmptyHook;
}

void InitCharSelector(s32 arg0) {
    EXTengine *e0;
    EXTengine *e2;
    TitlesObj *o;
    subengine *s;
    subengine *t;
    u8 *g;

    e0 = GetEngineSlot(0);
    e2 = GetEngineSlot(2);
    o = (TitlesObj *)&CharSelectors[arg0];
    s = e2->sub + arg0;
    o->unk8 = s;
    t = e0->sub + arg0;
    o->unkC = t;
    o->unk10 = e2->sub + (arg0 + 4);
    o->ext = ExtTemplate400;
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
    g = &GamePlayers;
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

void UpdateCharSelector(TitlesSlot *slot) {
    TitlesObj *o = (TitlesObj *)slot;
    subengine *t;
    subengine *s;
    subengine *s2;
    CharCard *card;
    TitlesOff *off;
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
    x = CharCards[o->unk30].x + CharSelectorOffsets[o->unk4].x;
    y = CharCards[o->unk30].y + CharSelectorOffsets[o->unk4].y;
    t->unk04 = lerp_snap(t->unk04, x, 0x800);
    t->unk08 = lerp_snap(t->unk08, y, 0x800);
    still = abs(t->unk04 - x) + abs(t->unk08 - y);
    s->unk08 = s2->unk08 = ((TitlesTrig *)rcossin_tbl)[(FrameCount * o->unk28 - 0x400) & 0xFFF].hi + 0x1A4;
    still = still == 0;
    if (o->unk0 & 1) {
        tgt = CharCards[o->unk30].target;
        v = o->unk14;
        v = TitlesSteer(o->unk1C, tgt, v, 0x2BC);
        o->unk14 = v;
        o->unk1C += v;
        s->unk20.x = o->unk1C >> 4;
        v = o->unk18;
        v = TitlesSteer(o->unk20, tgt, v, 0x3E8);
        o->unk18 = v;
        o->unk20 += v;
        s->unk20.y = o->unk20 >> 4;
        {
            EXTengine *e = GetEngineSlot(2);
            DoAnimCommand(NULL, e, o->unk8, (EXTanimator *)&o->ext);
        }
        s->unk52 = 1;
        o->unk24++;
        return;
    }
    if (o->unk4 < GamePlayers) {
        if (O69PAD[o->unk4].pressed & 0x80) {
            PlaySfx(CharMoveSfx[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30--;
        }
        if (O69PAD[o->unk4].pressed & 0x20) {
            PlaySfx(CharMoveSfx[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30++;
        }
        if (O69PAD[o->unk4].pressed & 0x10) {
            PlaySfx(CharMoveSfx[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30 -= 4;
        }
        if (O69PAD[o->unk4].pressed & 0x40) {
            PlaySfx(CharMoveSfx[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
            o->unk30 += 4;
        }
    }
    o->unk30 &= 7;
    if ((o->unk4 < ((TitlesModeView *)GameInfo)->unk26 && (O69PAD[o->unk4].pressed & 0x4000) && still) || o->unk2C != 0) {
        GetEngineSlot(0)->sub[20].flags &= 0xFFFF7FFF;
        GetEngineSlot(0)->sub[21].flags &= 0xFFFF7FFF;
        card = CharCards;
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
            s->unk6C = HeroModelBank + card[o->unk30].model * 0x38;
            s->flags |= 0x8000;
        }
        GameInfo[o->unk4 + 0x50] = card[o->unk30].hero;
        GameInfo[o->unk4 + 0x4C] = card[o->unk30].hero;
        PlaySfx(CharPickSfx[o->unk4], NULL, NULL, 0x1000, 0x1E00, 0);
        o->ext = ExtTemplate400;
        {
            s32 w = CharCards[o->unk30].anim;
            o->unk24++;
            o->ext.w[3] = w;
        }
    }
}

void CharSelectInit(void) {
    EXTengine *eng;
    CharCard *rec;
    s32 i;
    s32 off;

    cam_init((u8 *)&CharSelectCamera);
    *(s32 *)((u8 *)&CharSelectCamera + 8) = 0x400;
    *(s32 *)((u8 *)&CharSelectCamera + 0x18) = 0x190;
    eng = GetEngineSlot(0);
    func_80095BEC(0, (s32 *)CharSelectPlace);
    func_80095BEC(2, (s32 *)CharSelectModelsPlace);
    i = 0;
    do {
        InitCharSelector(i);
        i++;
    } while (i < 4);
    i = 0;
    rec = CharCards;
    off = 0x7E0;
    do {
        i++;
        rec->sub = (u8 *)eng->sub + off;
        rec->flags = 0;
        rec++;
        off += 0xA8;
    } while (i < 8);
    ShuffleCharSelectors();
}

void ShuffleCharSelectors(void) {
    s32 i;
    s32 j;

    i = 0;
    if (GamePlayers != 0) {
        do {
            do {
                CharSelectors[i].idx = Random(8);
                for (j = 0; j < i; j++) {
                    if (CharSelectors[j].idx == CharSelectors[i].idx) {
                        break;
                    }
                }
            } while (j != i);
            CharSelectors[i].dst->unk4 = CharCards[CharSelectors[i].idx].x + CharSelectorOffsets[i].x;
            CharSelectors[i].dst->unk8 = CharCards[CharSelectors[i].idx].y + CharSelectorOffsets[i].y;
            i++;
        } while (i < ((TitlesModeView *)GameInfo)->unk26);
    }
}

s32 CpuPickCharacter(s32 a0) {
    u8 *g = GameInfo;
    s32 r;
    s32 i;
    s8 *t;
    s32 v;
    s32 w;
    CharCard *rec;

    if (*(u16 *)(g + 0x32) & 4) {
        if (g[0x26] == 1) {
            s8 *t = AdventureHeadsSolo;
            s32 v = Random(2) + (a0 - 1) * 2;
            s32 w = (g[0x4C] - 1) * 6;
            r = t[v + w];
        } else {
            s8 *t = AdventureHeadsTeam;
            s32 v = Random(2) + (a0 - 2) * 2 + (g[0x4D] - 1) * 4;
            s32 w = (g[0x4C] - 1) << 5;
            r = t[v + w];
        }
        i = 0;
        rec = CharCards;
        do {
            if (*(s32 *)&rec->hero == r) {
                return i;
            }
            i++;
            rec++;
        } while (i < 8);
        return -1;
    }
    {
        CharCard *p = CharCards;
        s32 k;
        s32 j;
        do {
            k = Random(4) * 2;
            if (p[k].flags & 1) {
                continue;
            }
            j = k + 1;
        } while ((p[k].flags & 1) || (p[j].flags & 1));
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
    s->unk08 = ((rcossin_tbl[(((GameFrameCount * 50) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0x6E;
    s->unk12 = (rcossin_tbl[(((GameFrameCount * 25) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s->unk14 = (rcossin_tbl[(((GameFrameCount * 36) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s->unk52 = 1;
    s2 = &GetEngineSlot(0)->sub[25];
    s2->unk08 = ((rcossin_tbl[(((GameFrameCount * 41) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0xBE;
    s2->unk12 = (rcossin_tbl[(((GameFrameCount * 35) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s2->unk14 = (rcossin_tbl[(((GameFrameCount * 26) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s2->unk52 = 1;
    min = 0x7FFFFFFF;
    for (i = 0; i < ((TitlesModeView *)GameInfo)->unk26; i++) {
        if (CharSelectors[i].pad10[5] < min) {
            min = CharSelectors[i].pad10[5];
        }
    }
    if (min >= 0x10) {
        for (i = GamePlayers; ; i++) {
            s32 w;

            if (i >= 4) {
                break;
            }
            w = CharSelectors[i].pad10[5];
            if (w > 0 && w < 15) {
                min = 0;
                break;
            }
            if (w == 0) {
                CharSelectors[i].idx = CpuPickCharacter(i);
                CharSelectors[i].pad10[7]++;
                min = 0;
                break;
            }
        }
        if (min >= 0x79) {
            TitlesSetHeroMasks();
            MenuChainNext();
        }
    }
    if (Pads[0].pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    for (i = 0; i < 4; i++) {
        UpdateCharSelector(&CharSelectors[i]);
    }
}

void CharSelectRender(void) {
    EXTengine *eng;

    eng = GetEngineSlot(0);
    eng->unk10.x = -0x140;
    eng->unk10.y = -0xF0;
    RenderTitlesBackdropAndMenu();
    gfx_set_viewport(&TitlesMenuView, &CharSelectCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(2));
}

s8 adventure_mode_head(s32 a0) {
    u8 *g = GameInfo;

    if (g[0x26] == 1) {
        s8 *t = AdventureHeadsSolo;
        s32 v = Random(2) + (a0 - 1) * 2;
        s32 w = (g[0x4C] - 1) * 6;
        return t[v + w];
    } else {
        s8 *t = AdventureHeadsTeam;
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
    BattleArena *e;
    s32 count;
    u8 *g;
    s32 i;
    s32 found;
    u8 *tbl;

    e = BattleArenas;
    count = 0;
    i = count;
    tbl = CardSlots[0].unk8;
outer:
    if (i < 7) {
        j = 0;
        p = (s32 *)e->unk0;
        q = p + 1;
        g = GameInfo;
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

    LevelSelectReload = 0;
    func_80095BEC(0, (s32 *)LevelSelectPlace);
    func_80095BEC(2, (s32 *)LevelSelectPlace0);
    func_80095BEC(3, (s32 *)LevelSelectPlace1);
    func_80095BEC(4, (s32 *)LevelSelectPlace2);
    func_80095BEC(5, (s32 *)LevelSelectPlace3);
    func_80095BEC(6, (s32 *)LevelSelectPlace4);
    GetEngineSlot(1)->sub[4].flags |= 0x8000;
    GetEngineSlot(1)->sub[5].flags |= 0x8000;
    GetEngineSlot(1)->sub[6].flags |= 0x8000;
    GetEngineSlot(1)->sub[7].flags |= 0x8000;
    sub = GetEngineSlot(1)->sub;
    LevelSelectBusy = 0;
    sub[3].flags |= 0x8000;
    LevelSelectCursor(0);
}

void LevelSelectInput(void) {
    s32 *tbl;
    u8 *g;
    EXTengine *e;
    s32 c;

    if ((Pads[0].pressed & 0x80) && (s8)GameInfo[0x36] > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        LevelSelectReload = 4;
        GameInfo[0x36]--;
    }
    if ((Pads[0].pressed & 0x20) && (s8)GameInfo[0x36] < 6) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        LevelSelectReload = 4;
        GameInfo[0x36]++;
    }
    if ((Pads[0].pressed & 0x10) && (s8)GameInfo[0x37] > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        GameInfo[0x37]--;
    }
    if ((Pads[0].pressed & 0x40)
        && (s8)GameInfo[0x37] < BattleArenas[(s8)GameInfo[0x36]].unk4 - 1) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        GameInfo[0x37]++;
    }
    if (((PadButtons *)Pads)->pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    if (((PadButtons *)Pads)->pressed & 0x4000) {
        if (TitlesSlotOn()) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
        } else {
            PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (TitlesSlotOn()) {
        GetEngineSlot(0)->sub->unk6C = (void *)TitlesSlotRowI((s8)GameInfo[0x36], (s8)GameInfo[0x37]);
        GetEngineSlot(0)->sub->unk08 = 8;
        GetEngineSlot(2)->sub->unk6C = (void *)TitlesSlotModelI((s8)GameInfo[0x36], (s8)GameInfo[0x37]);
        GetEngineSlot(2)->sub->unk74 = TitlesSlotCI((s8)GameInfo[0x36], (s8)GameInfo[0x37]);
        GetEngineSlot(0)->sub[4].flags |= 0x8000;
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
        return;
    }
    e = GetEngineSlot(0);
    tbl = LangLocked;
    g = GameInfo;
    e->sub->unk6C = (void *)tbl[*(s32 *)(g + 0x2C)];
    GetEngineSlot(2)->sub->unk74 = 0;
    GetEngineSlot(0)->sub[4].flags &= ~0x8000;
    GetEngineSlot(0)->sub->unk08 = 0x1C;
    GetEngineSlot(0)->sub[9].flags |= 0x8000;
    if ((c = ((BattleLevel *)((u8 *)BattleArenas[(s8)GameInfo[0x36]].unk0 + ((s8)GameInfo[0x37] << 4)))->type == 2)) {
        if (CardStatus & 2) {
            if (CardStatus & 0x40) {
                GetEngineSlot(0)->sub[9].unk6C = (void *)LangCheckingCard[*(s32 *)(GameInfo + 0x2C)];
            } else if (CardStatus & 0x10) {
                if (UnlockCardLevels(0) != 0) {
                    *(s32 *)&GetEngineSlot(0)->sub[9].unk6C = LangPressToUnlockLevels[*(s32 *)(GameInfo + 0x2C)];
                    if (Pads[0].pressed & 0x2000) {
                        PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                        UnlockCardLevels(1);
                    }
                } else {
                    GetEngineSlot(0)->sub[9].unk6C = (void *)LangNoMoreUnlocked[*(s32 *)(GameInfo + 0x2C)];
                }
            } else {
                GetEngineSlot(0)->sub[9].unk6C = (void *)LangInsertCard[*(s32 *)(GameInfo + 0x2C)];
            }
        } else {
            GetEngineSlot(0)->sub[9].unk6C = (void *)LangInsertCard[*(s32 *)(GameInfo + 0x2C)];
        }
    } else {
        GetEngineSlot(0)->sub[9].unk6C = NULL;
    }
}

void LevelSelectUpdate(void) {
    BattleLevel *slot;
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
    BattleArena *tbl;
    TrigEntry *trig;

    if (LevelSelectReload != 0) {
        prims_empty_back();
        mal_compact_step((u8 *)&MainHeap);
        LevelSelectReload--;
    }
    AnimateTitlesBackdrop();
    a = D_800555DC;
    texture_update_anims((obj_textureset *)(a + 0x10));
    texture_update_anims((obj_textureset *)(a + 0x48));
    val = LevelSelectBusy;
    if (val == 0) {
        LevelSelectInput();
    }
    e0 = GetEngineSlot(0);
    boff = ((BattleLevel *)((u8 *)BattleArenas[(s8)GameInfo[0x36]].unk0 + ((s8)GameInfo[0x37] << 4)))->idx;
    w = ModeTypes[boff * 3] - 0x101;
    off = ((s32 **)D_8004DDBC)[*(s32 *)(GameInfo + 0x2C)][w];
    e0->sub[6].unk6C = (void *)off;
    t = &GetEngineSlot(0)->sub[1];
    t->unk08 = lerp_fix12(t->unk08,
        *(s16 *)((u8 *)&GetEngineSlot(1)->sub[(s8)GameInfo[0x37] + 4] + 0x72) + 0x32, 0x800);
    i = 0;
    off = 0;
    for (;;) {
        s32 type;
        s32 idx;
        s32 on;
        s32 fl;
        EXTengine *e;

        if (i >= (BattleArenas + (s8)GameInfo[0x36])->unk4) {
            break;
        }
        GetEngineSlot(1)->sub[off + 4].flags |= 0x8000;
        e = GetEngineSlot(i + 3);
        fl = *(s32 *)(GameInfo + 4) & 0x2000;
        slot = &((BattleLevel *)BattleArenas[(s8)GameInfo[0x36]].unk0)[off];
        idx = slot->idx;
        type = slot->type;
        s = e->sub;
        if (fl || type == 1
            || (type == 2 && ((g = GameInfo + idx), g[0xE4] != 0 || g[0x168] != 0))) {
            on = 1;
        } else {
            on = 0;
        }
        if (on) {
            s->unk6C = ((BattleLevel *)((u8 *)BattleArenas[(s8)GameInfo[0x36]].unk0 + (off << 4)))->model;
            c = ((BattleLevel *)((u8 *)BattleArenas[(s8)GameInfo[0x36]].unk0 + (off << 4)))->unkC;
            s->unk74 = c;
            s->unk10 = 0x7D;
            s->unk12 = 0;
            s->unk14 = 0;
            s->unk04 = rcossin_tbl[(((GameFrameCount * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
            s->unk08 = (rcossin_tbl[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
        } else {
            s->unk6C = D_800555DC;
            s->unk74 = 0x2006;
            s->unk10 = 0;
            s->unk12 = FrameCount * 50 + (i << 10);
            val = rcossin_tbl[(((GameFrameCount * 100 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2;
            s->unk04 = 0;
            s->unk08 = 0;
            s->unk14 = (val * 25) >> 9;
        }
        off++;
        i++;
        s->unk51 = 0;
        s->unk52 = 1;
    }
    for (i = (GameArena + BattleArenas)->unk4; i < 4; i++) {
        GetEngineSlot(1)->sub[i + 4].flags &= ~0x8000;
        GetEngineSlot(i + 3)->sub->unk74 = 0;
    }
    h = GameInfo;
    NextGameMode = ((BattleLevel *)((u8 *)BattleArenas[(s8)h[0x36]].unk0 + ((s8)h[0x37] << 4)))->idx;
    q = GetEngineSlot(2)->sub;
    q->unk10 = 0x7D;
    q->unk51 = 0;
    q->unk04 = rcossin_tbl[(((GameFrameCount * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;    q->unk08 = (rcossin_tbl[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
    q->unk52 = 1;
}

void LevelSelectRender(void) {
    s32 *cam = (s32 *)&TitlesCamera;
    s8 *m;

    gfx_set_viewport(&LevelSelectView0, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(2));
    RenderTitlesBackdropAndMenu();
    m = (s8 *)GameInfo;
    cam[2] = (m[0x37] != 0) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView1, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(3));
    cam[2] = (m[0x37] != 1) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView2, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(4));
    cam[2] = (m[0x37] != 2) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView3, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(5));
    cam[2] = (m[0x37] != 3) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView4, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs((u8 *)GetEngineSlot(6));
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
        *(s32 *)(p + 8) = lerp_fix12(*(s32 *)(p + 8), v, 0x800);
    } else {
        *(s32 *)(p + 8) = v;
    }
}

s32 IsLevelUnlocked(s32 arg0) {
    u8 *g = GameInfo;
    u8 *e = (u8 *)BattleArenas[(s8)g[0x36]].unk0 + (arg0 << 4);
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
    s32 i = GameArena;
    u8 *s = (u8 *)BattleArenas[i].unk0;
    return *(s32 *)(s + (arg0 << 4)) == 2;
}

void TitlesEmptyStub(void) {
}

s32 LevelListName(s32 arg0, s32 arg1) {
    s32 idx = *(s32 *)(BattleArenas[arg0].unk0 + (arg1 << 4) + 4);
    s32 w = ModeTypes[idx * 3] - 0x101;
    s32 **tbl = (s32 **)D_8004DDBC;
    return tbl[GameLanguage][w];
}

s32 LevelListModel(s32 arg0, s32 arg1) {
    return *(s32 *)(BattleArenas[arg0].unk0 + (arg1 << 4) + 8);
}

s32 LevelListAnim(s32 a, s32 b) {
    return *(s32 *)(BattleArenas[a].unk0 + (b << 4) + 0xC);
}

s32 LevelListCount(s32 arg0) {
    return BattleArenas[arg0].unk4;
}

s32 LevelListText(s32 a, s32 b) {
    s32 base = BattleArenas[a].unk0;
    s32 idx = *(s32 *)((u8 *)base + (b << 4) + 4);
    s32 *row = D_8004DD70[GameLanguage];
    return row[idx];
}

s32 LevelListType(s32 a, s32 b) {
    return *(s32 *)((u8 *)BattleArenas[a].unk0 + (b << 4));
}

s32 LevelListLevel(s32 a, s32 b) {
    return *(s32 *)((u8 *)BattleArenas[a].unk0 + (b << 4) + 4);
}

s32 AnyCardHasLevel(s32 idx) {
    u8 *p;
    s32 i;

    i = 0;
    p = CardSlots[0].unk8;
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
    TitlesPreset *p;
    u8 *s;

    s = (u8 *)GetEngineSlot(0)->sub;
    p = &TeamModePresets[TeamModePreset];
    *(u16 *)(s + 0x74) = TeamModeHeroFrames[O69PV->unk4C[p->hero[0]]];
    *(u16 *)(s + 0x11C) = TeamModeHeroFrames[O69PV->unk4C[p->hero[1]]];
    *(u16 *)(s + 0x1C4) = TeamModeHeroFrames[O69PV->unk4C[p->hero[2]]];
    *(u16 *)(s + 0x26C) = TeamModeHeroFrames[O69PV->unk4C[p->hero[3]]];
    *(s32 *)(s + 0x540) &= ~0x8000;
    *(s32 *)(s + 0x5E8) &= ~0x8000;
    *(s32 *)(s + 0x690) &= ~0x8000;
    *(s32 *)(s + 0x540) |= p->flags[0];
    *(s32 *)(s + 0x5E8) |= p->flags[1];
    *(s32 *)(s + 0x690) |= p->flags[2];

    TITLES_SETBAR(0, p->val[0]);
    TITLES_SETBAR(1, p->val[1]);
    TITLES_SETBAR(2, p->val[2]);
    TITLES_SETBAR(3, p->val[3]);

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
    if (TeamModeRow == 0) {
        if (Pads[0].pressed & 0x80) {
            temp_v0 = TeamModePreset - 1;
            TeamModePreset = temp_v0;
            if (temp_v0 < 0) {
                TeamModePreset = 1;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (Pads[0].pressed & 0x20) {
            temp_v0_2 = TeamModePreset + 1;
            TeamModePreset = temp_v0_2;
            if (temp_v0_2 >= 2) {
                TeamModePreset = 0;
            }
            PlaySfx(0x385, 0, 0, 0x1000, 0x1E00, 0);
        }
        TeamModeShowPreset();
        if (Pads[0].pressed & 0x4000) {
            MenuChainNext();
            PlaySfx(0x192, 0, 0, 0x1000, 0x1E00, 0);
        }
    }
    if ((Pads[0].pressed & 0x10) && (TeamModeRow > 0)) {
        TeamModeRow -= 1;
    }
    if ((Pads[0].pressed & 0x40) && (TeamModeRow < 0)) {
        TeamModeRow += 1;
    }
    if (Pads[0].pressed & 0x1000) {
        PlaySfx(0x384, 0, 0, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void team_mode_init(void) {
    func_80095BEC(0, (s32 *)TeamModePlace);
    TeamModeRow = 0;
    TeamModePreset = 0;
}

void MoveTeamBar(s32 arg0, s32 arg1) {
    EXTengine *eng = GetEngineSlot(0);
    s32 off = arg0 * 0xA8;
    u8 *p = (u8 *)eng->sub + (off + 0x2A0);
    s16 v;

    v = lerp_fix12(*(s16 *)(p + 0x70), arg1, 0x800);
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
    eng_render_subs((u8 *)GetEngineSlot(2));
}

void TitlesExtraScreenInit(void) {
    func_80095BEC(2, (s32 *)TitlesExtraPlace);
}
