#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/titles_ovl.h"
#include "bash/titles.h"

extern u8 GamePlayers;

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

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
void *MainMenuHints[4] = { &TextAdventureHint, &TextBattleHint, &TextTournamentHint, &TextOptionsHint };
PlaceRec MainMenuPlace[9] = {
    { 4, -0x20, 0x12C, -0x46, 0x72, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x2C, 0, 0, 0, 0, 0, 0, &TextAdventureMode, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0xA, 0, 0, 0, 0, 0, 0, &TextBattleMode, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x18, 0, 0, 0, 0, 0, 0, &TextTournament, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x3A, 0, 0, 0, 0, 0, 0, &TextOptions, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x82, 0, 0, 0, 0, 0, 0, &TextEmpty, 4, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &TextSelectGameType, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec PlayersMenuPlace[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x42, 0, 0, 0, 0, 0, 0, &Text1Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x1C, 0, 0, 0, 0, 0, 0, &Text2Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 8, 0, 0, 0, 0, 0, 0, &Text3Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0x2C, 0, 0, 0, 0, 0, 0, &Text4Player, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &TextSelectNumberOfPlayers, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec PlayersMenuAdventurePlace[8] = {
    { 4, -0x20, 0x12C, -0x46, 0x50, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, &Text1Player, 0, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, &Text2Player, 0, 0, 0, 0 },
    { 3, 0x3A, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x3A, 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &TextSelectNumberOfPlayers, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec NewLoadMenuPlace[6] = {
    { 3, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &TextAdventureMode, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x20, 0x12C, -0x46, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x7F7A, -0x22, 0, 0, 0, 0, 0, 0, &TextNewGame, 2, 0, 0, 0 },
    { 3, -0x7F7A, 0, 0, 0, 0, 0, 0, 0, &TextLoadGame, 3, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec BattleOptionsPlace[12] = {
    { 4, -0x8000, 0x190, -0x68, -0xA, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xA, 0xB4, -0x14, 0, 0xCCC, 0, 0x28, 0, 0, 0, 0, 0xC7, 0x2009, 2 },
    { 3, 0x3C, -0x46, 0, 0, 0, 0, 0, 0, &TextCupsCount, 0, 0, 0, 0 },
    { 3, -0xDC, -0x55, 0, 0, 0, 0, 0, 0, &TextSelectCups, 4, 0, 0, 0 },
    { 4, -0x8000, 0x190, 0x28, 0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x46, 0x37, 0, 0, 0, 0, 0, 0, &TextEmpty, 0, 0, 0, 0 },
    { 3, -0xDC, 0x32, 0, 0, 0, 0, 0, 0, &TextSelectDifficulty, 4, 0, 0, 0 },
    { 4, -0x8000, 0x258, 0x9A, 0xC0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0xA5, 0, 0, 0, 0, 0, 0, &TextBeginBattle, 4, 0, 0, 0 },
    { 4, -0x15E, 0x258, -0xD0, -0xA6, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, -0xCA, 0, 0, 0, 0, 0, 0, &TextGameOptions, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
GroupFile TitlesFiles[3] = {
    &HeroModelBank[DB_Font], &DatIndex[397], &DatIndex[396], &HeroModelBank[DB_Interface], &DatIndex[395],
    &DatIndex[394], 0, 0, 0,
};
/* the engine's table at 0x800A0030 points at it for the front end's modes */
file_group_load_info file_group_TITLES = { &FileGroups[14], TitlesFiles };
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
    0x1800, 0x10, 0x100, 0x40, 0x100, 0x40, VramAreas, VramNodePool, 0x20, 0x76C,
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
s32 TitlesBackdropSteps[9] = {
    0x388011, (s32)0x80114100, 0x41800054, 0x168011, (s32)0x80114000, 0x40000016, 0x388011,
    (s32)0x80004100, 0,
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
void *TextDifficulties[1][3] = {
    {
        &TextEasy, &TextMedium,
        &TextHard,
    },
};
void *TextBattleKinds[1][2] = { { &TextPressButtonToStart, &TextPressButtonToStart } };
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
void *LangOptions[1] = { &TextOptions };
void *LangExit[1] = { &TextExit };
void *LangSound[1] = { &TextSound };
void *LangAudioMode[1] = { &TextAudioMode };
void *LangStereo[1] = { &TextStereo };
void *LangMono[1] = { &TextMono };
void *LangMusicVolume[1] = { &TextMusicVolume };
void *LangFxVolume[1] = { &TextFxVolume };
void *LangController[1] = { &TextController };
void *LangVibration[1] = { &TextVibration };
void *LangOn[1] = { &TextOn };
void *LangOff[1] = { &TextOff };
void *LangPressButtonToChange[1] = { &TextPressButtonToChange };
void *LangPressButtonToAccept[1] = { &TextPressButtonToAccept };
PlaceRec OptionsPlace[7] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &LangOptions, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x5A, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x32, 0, 0, 0, 0, 0, 0, &LangSound, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xA, 0, 0, 0, 0, 0, 0, &LangController, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x1E, 0, 0, 0, 0, 0, 0, &LangExit, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec OptionsSoundPlace[12] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &LangSound, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xFA, 0xFA, -0x40, 0x8C, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0xD2, -0x1E, 0, 0, 0, 0, 0, 0, &LangAudioMode, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, &LangStereo, 0, 0, 0, 0 },
    { 0xE, 0x46, -0x1E, 0, 0, 0, 0, 0, 0, &LangMono, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0xA, 0, 0, 0, 0, 0, 0, &LangMusicVolume, 0, 0, 0, 0 },
    { 3, 0x46, 0xA, 0, 0, 0, 0, 0, 0, &TextPercent100, 0, 0, 0, 0 },
    { 0xE, -0xD2, 0x32, 0, 0, 0, 0, 0, 0, &LangFxVolume, 0, 0, 0, 0 },
    { 3, 0x46, 0x32, 0, 0, 0, 0, 0, 0, &TextPercent90, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, &LangPressButtonToAccept, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
PlaceRec OptionsControllerPlace[23] = {
    { 0xE, -0x8000, -0x8C, 0, 0, 0, 0, 0, 0, &LangController, 0, 0, 0, 0 },
    { 4, -0x15E, 0x15E, -0x94, -0x64, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0xBE, 0xBE, -0x4A, 0x82, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0x3C, 0, 0, 0, 0, 0, 0, &LangVibration, 0, 0, 0, 0 },
    { 0xE, -0x8F, 0, 0, 0, 0, 0, 0, 0, &LangOn, 7, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, &LangOff, 0, 0, 0, 0 },
    { 0xE, -0x96, 0, 0, 0, 0, 0, 0, 0, &LangOff, 1, 0, 0, 0 },
    { 0xE, -0x35, 0, 0, 0, 0, 0, 0, 0, &LangOn, 7, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, &LangOff, 0, 0, 0, 0 },
    { 0xE, -0x3C, 0, 0, 0, 0, 0, 0, 0, &LangOff, 1, 0, 0, 0 },
    { 0xE, 0x25, 0, 0, 0, 0, 0, 0, 0, &LangOn, 7, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, &LangOff, 0, 0, 0, 0 },
    { 0xE, 0x1E, 0, 0, 0, 0, 0, 0, 0, &LangOff, 1, 0, 0, 0 },
    { 0xE, 0x75, 0, 0, 0, 0, 0, 0, 0, &LangOn, 7, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, &LangOff, 0, 0, 0, 0 },
    { 0xE, 0x6E, 0, 0, 0, 0, 0, 0, 0, &LangOff, 1, 0, 0, 0 },
    { 3, -0x87, -0xF, 0, 0, 0, 0, 0, 0, &TextP1, 4, 0, 0, 0 },
    { 3, -0x2D, -0xF, 0, 0, 0, 0, 0, 0, &TextP2, 4, 0, 0, 0 },
    { 3, 0x2D, -0xF, 0, 0, 0, 0, 0, 0, &TextP3, 4, 0, 0, 0 },
    { 3, 0x7D, -0xF, 0, 0, 0, 0, 0, 0, &TextP4, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x3C, 0, 0, 0, 0, 0, 0, &LangPressButtonToChange, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0x64, 0, 0, 0, 0, 0, 0, &LangPressButtonToAccept, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s32 OptionsCursor = 0;
s32 OptionsPage = 0;
process_info OptionsScreen = { OptionsInit, OptionsUpdate, OptionsRender, OptionsQuit, 0 };
process_info RunGame = { RunGameInit, RunGameUpdate, RunGameRender, RunGameQuit, 0 };
camera_info_tag TitlesCamera = {0};
s32 MainMenuCursor = 0;
Ext400 TitlesBackdropAnimator = {0};
process_info **MenuChainPos = 0;
s32 MainMenuHintTimer = 0;
s32 NewLoadMenuCursor = 0;
s32 BattleOptionsState = 0;
void *LangCharacterSelect[1] = { &TextCharacterSelect };
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
    { 0xE, -0x8000, 0x116, 0, 0, 0, 0, 0, 0, &LangCharacterSelect, 0, 0, 0, 0 },
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
    { 0, 1, { 0 }, 0x8E, 0x20, 0xCCCC, 0xC7, 0, &CharSelectSteps2, 0x38E, 0 },
    { 0, 2, { 0 }, 0xE8, 0x20, 0xCCCC, 0xC7, 0, &CharSelectSteps1, 0x38F, 0 },
    { 0, 5, { 0 }, 0x142, 0x20, 0x9999, 0xC7, 0, &CharSelectSteps6, 0x394, 0 },
    { 0, 7, { 0 }, 0x19C, 0x20, 0x9999, 0xC7, 0, &CharSelectSteps4, 0x392, 0 },
    { 0, 3, { 0 }, 0x8E, 0x84, 0xCCCC, 0xC7, 0, &CharSelectSteps3, 0x391, 0 },
    { 0, 4, { 0 }, 0xE8, 0x84, 0x9999, 0xC7, 0, &CharSelectSteps0, 0x390, 0 },
    { 0, 6, { 0 }, 0x142, 0x84, 0x9999, 0xC7, 0, CharSelectSteps7, 0x395, 0 },
    { 0, 8, { 0 }, 0x19C, 0x84, 0x9999, 0xC7, 0, &CharSelectSteps5, 0x393, 0 },
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
void *CharSelectAnim0[3] = { CharSelectSteps0, 0, 0 };
ScriptInit CharSelectInit0 = { 0, CharSelectAnim0, 0x401 };
s32 CharSelectSteps1[3] = { (s32)0x80110000, 0x42000014, 0x8000 };
void *CharSelectAnim1[3] = { CharSelectSteps1, 0, 0 };
ScriptInit CharSelectInit1 = { 0, CharSelectAnim1, 0x401 };
s32 CharSelectSteps2[3] = { (s32)0x80110000, 0x42800016, 0x8000 };
void *CharSelectAnim2[3] = { CharSelectSteps2, 0, 0 };
ScriptInit CharSelectInit2 = { 0, CharSelectAnim2, 0x401 };
s32 CharSelectSteps3[3] = { (s32)0x80110000, 0x43000020, 0x8000 };
void *CharSelectAnim3[3] = { CharSelectSteps3, 0, 0 };
ScriptInit CharSelectInit3 = { 0, CharSelectAnim3, 0x401 };
s32 CharSelectSteps4[3] = { (s32)0x80110000, 0x44000016, 0x8000 };
void *CharSelectAnim4[3] = { CharSelectSteps4, 0, 0 };
ScriptInit CharSelectInit4 = { 0, CharSelectAnim4, 0x401 };
s32 CharSelectSteps5[3] = { (s32)0x80110000, 0x44800014, 0x8000 };
void *CharSelectAnim5[3] = { CharSelectSteps5, 0, 0 };
ScriptInit CharSelectInit5 = { 0, CharSelectAnim5, 0x401 };
s32 CharSelectSteps6[3] = { (s32)0x80110000, 0x45000020, 0x8000 };
void *CharSelectAnim6[3] = { CharSelectSteps6, 0, 0 };
ScriptInit CharSelectInit6 = { 0, CharSelectAnim6, 0x401 };
s32 CharSelectSteps7[3] = { (s32)0x80110000, 0x45800020, 0x8000 };
void *CharSelectAnim7[3] = { CharSelectSteps7, 0, 0 };
camera_info_tag CharSelectCamera = {0};
TitlesSlot CharSelectors[4] = {0};
void *LangPressToUnlockLevels[1] = { &TextPressToUnlockLevels };
void *LangNoMoreUnlocked[1] = { &TextNoMoreUnlocked };
void *LangInsertCard[1] = { &TextInsertCard };
void *LangLocked[1] = { &TextLocked };
BattleLevel BattleLevelsBash[4] = {
    { 1, 0, &HeroModelBank[DB_MainmenuArenaModels2], 0x2005 },
    { 1, 1, &HeroModelBank[DB_MainmenuArenaModels2], 0x2006 },
    { 1, 2, &HeroModelBank[DB_MainmenuArenaModels2], 0x2007 },
    { 1, 3, &HeroModelBank[DB_MainmenuArenaModels2], 0x2008 },
};
BattleLevel BattleLevelsPolar[4] = {
    { 1, 0x13, &HeroModelBank[DB_MainmenuArenaModels1], 0x2005 },
    { 1, 0x12, &HeroModelBank[DB_MainmenuArenaModels1], 0x2006 },
    { 1, 0x14, &HeroModelBank[DB_MainmenuArenaModels1], 0x2007 },
    { 1, 0x15, &HeroModelBank[DB_MainmenuArenaModels1], 0x2008 },
};
BattleLevel BattleLevelsPogo[4] = {
    { 1, 0xA, &HeroModelBank[DB_MainmenuArenaModels1], 0x2001 },
    { 1, 0xB, &HeroModelBank[DB_MainmenuArenaModels1], 0x2002 },
    { 1, 0xC, &HeroModelBank[DB_MainmenuArenaModels1], 0x2003 },
    { 1, 0xD, &HeroModelBank[DB_MainmenuArenaModels1], 0x2004 },
};
BattleLevel BattleLevelsArkenoid[4] = {
    { 1, 0xE, &HeroModelBank[DB_MainmenuArenaModels2], 0x2001 },
    { 1, 0xF, &HeroModelBank[DB_MainmenuArenaModels2], 0x2002 },
    { 1, 0x10, &HeroModelBank[DB_MainmenuArenaModels2], 0x2003 },
    { 1, 0x11, &HeroModelBank[DB_MainmenuArenaModels2], 0x2004 },
};
BattleLevel BattleLevelsTank[4] = {
    { 2, 6, &HeroModelBank[DB_MainmenuArenaModels1], 0x2009 },
    { 2, 8, &HeroModelBank[DB_MainmenuArenaModels1], 0x200A },
    { 2, 7, &HeroModelBank[DB_MainmenuArenaModels1], 0x200B },
    { 2, 5, &HeroModelBank[DB_MainmenuArenaModels1], 0x200C },
};
BattleLevel BattleLevelsRace[4] = {
    { 2, 0x17, &HeroModelBank[DB_MainmenuArenaModels2], 0x2009 },
    { 2, 0x18, &HeroModelBank[DB_MainmenuArenaModels2], 0x200A },
    { 2, 0x1A, &HeroModelBank[DB_MainmenuArenaModels2], 0x200B },
    { 2, 0x19, &HeroModelBank[DB_MainmenuArenaModels2], 0x200C },
};
BattleLevel BattleLevelsDino[4] = {
    { 2, 0x1B, &HeroModelBank[DB_MainmenuArenaModels2], 0x200D },
    { 2, 0x1C, &HeroModelBank[DB_MainmenuArenaModels2], 0x200E },
    { 2, 0x1D, &HeroModelBank[DB_MainmenuArenaModels2], 0x200F },
    { 2, 0x1E, &HeroModelBank[DB_MainmenuArenaModels2], 0x2010 },
};
BattleArena BattleArenas[7] = {
    { &BattleLevelsBash, 4 }, { &BattleLevelsPolar, 4 }, { &BattleLevelsPogo, 4 },
    { &BattleLevelsArkenoid, 4 }, { &BattleLevelsTank, 4 }, { &BattleLevelsRace, 4 },
    { &BattleLevelsDino, 4 },
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
void *LangChangeArena[1] = { &TextChangeArena };
void *LangChangeLevel[1] = { &TextChangeLevel };
void *LangPressButtonToSelect[1] = { &TextPressButtonToSelect };
void *LangChooseLevel[1] = { &TextChooseLevel };
PlaceRec LevelSelectPlace[11] = {
    { 3, 0x7FA1, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 0xD4, 0, 0, 0x266, 0, 0, 0, 0, 0, 0, 0xC7, 0x2008, 0 },
    { 0xE, 0x7FA1, -0x96, 0, 0, 0, 0, 0, 0, &LangChangeArena, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x42, 0, 0, 0, 0, 0, 0, &LangChangeLevel, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0x28, 0, 0, 0, 0, 0, 0, &LangPressButtonToSelect, 4, 0, 0, 0 },
    { 0xE, 0x7FA1, -0xC8, 0, 0, 0, 0, 0, 0, &LangChooseLevel, 0, 0, 0, 0 },
    { 3, 0x7FA1, -0x6E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x14A, 0x88, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x120, 0x28A, -0xCC, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 0x7FA1, 0x4E, 0, 0, 0, 0, 0, 0, &TextCheckCardForSave, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void *LangCheckingCard[1] = { &TextCheckingCard };
GfxViewport LevelSelectView0 = { 0x28, 0xEE, 0x19A, 0x1B0, 0, 0, 0x280, 0x1E0, 0xE1, 0x198, 0x200, 0x1FF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView1 = { 0x1CC, 0x24, 0x25C, 0x84, 0, 0, 0x280, 0x1E0, 0x214, 0x88, 0x400, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView2 = { 0x1CC, 0x88, 0x25C, 0xE8, 0, 0, 0x280, 0x1E0, 0x214, 0xEC, 0x4CC, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView3 = { 0x1CC, 0xEC, 0x25C, 0x14C, 0, 0, 0x280, 0x1E0, 0x214, 0x150, 0x600, 0xFF, 0, 0, 0, 0, 0 };
GfxViewport LevelSelectView4 = { 0x1CC, 0x150, 0x25C, 0x1B0, 0, 0, 0x280, 0x1E0, 0x214, 0x1B4, 0x700, 0xFF, 0, 0, 0, 0, 0 };
process_info LevelSelect = { LevelSelectInit, LevelSelectUpdate, LevelSelectRender, LevelSelectQuit, 0 };
s32 LevelSelectBusy = 0;
s32 LevelSelectReload = 0;
void *LangSelectBattleType[1] = { &TextSelectBattleType };
void *LangChangeBattleType[1] = { &TextChangeBattleType };
void *LangStartBattle[1] = { &TextStartBattle };
PlaceRec TeamModePlace[19] = {
    { 6, -0xFA, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, -0x6A, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0x38, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 6, 0xDC, -0x5A, 0, 0x1000, 0, 0, 0, 0, 0, 0, 0xC7, 0, 0 },
    { 4, -0x122, -0xAA, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, -0x88, -0x10, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0x10, 0x88, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 4, 0xAA, 0x122, -0x6E, 0xA, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0xAA, -0x46, 0, 0, 0, 0, 0, 0, &TextVs, 0, 0, 0, 0 },
    { 3, -0x8000, -0x46, 0, 0, 0, 0, 0, 0, &TextVs, 0, 0, 0, 0 },
    { 3, 0x88, -0x46, 0, 0, 0, 0, 0, 0, &TextVs, 0, 0, 0, 0 },
    { 0xE, -0x8000, -0xC8, 0, 0, 0, 0, 0, 0, &LangSelectBattleType, 0, 0, 0, 0 },
    { 4, -0x8000, 0x258, -0xCA, -0xA6, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, -0x8000, 0x42, 0, 0, 0, 0, 0, 0, &TextVsBattle, 2, 0, 0, 0 },
    { 4, -0x78, 0x78, 0x3A, 0x6C, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0xE, -0x8000, 0x8C, 0, 0, 0, 0, 0, 0, &LangChangeBattleType, 4, 0, 0, 0 },
    { 0xE, -0x8000, 0xA6, 0, 0, 0, 0, 0, 0, &LangStartBattle, 4, 0, 0, 0 },
    { 4, -0x10E, 0x10E, 0x87, 0xBE, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
void *LangVsBattle[1] = { &TextVsBattle };
void *LangTeamBattle[1] = { &TextTeamBattle };
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
    { 3, 0, 0, 0, 0, 0, 0, 0, 0, &TextSelectSlot, 2, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
/* two screens nothing in the front end chains to (the engine's own menus may) */
process_info TitlesExtraScreen = { TitlesExtraScreenInit, 0, TitlesExtraScreenRender, 0, 0 };
process_info TitlesShellScreen = { TitlesShellScreenInit, TitlesShellScreenUpdate, TitlesShellScreenRender, 0, 0 };

void TitlesInitLevel(void) {
    GameState *g;
    process_info **p;
    u8 *a1;

    ResetWarpRooms();
    g = GAME;
    g->warpRoom = 0;
    cam_init(&TitlesCamera);
    *(s32 *)((u8 *)&TitlesCamera + 8) = 0x400;
    *(s32 *)((u8 *)&TitlesCamera + 0x18) = 0x190;
    PlaceEngineSlot(1, (PlaceRec *)TitlesBackdropPlace);
    GetEngineSlot(1)->sub[4].flags &= ~0x8000;
    GetEngineSlot(1)->sub[5].flags &= ~0x8000;
    GetEngineSlot(1)->sub[6].flags &= ~0x8000;
    GetEngineSlot(1)->sub[7].flags &= ~0x8000;
    GetEngineSlot(1)->sub[3].flags &= ~0x8000;
    joypad_idle_count = 0;
    switch (g->mode) {
    case 0x28:
        core_process = 0;
        ResetGameState();
        g->levelFlags |= 0x200;
        process_control_init(&MenuProcessControl, &MainMenu);
        break;
    case 0x29:
        p = BattleChainResume;
        g->maxPlayers = 4;
        a1 = (u8 *)*p;
        MenuChainPos = p;
        process_control_init(&MenuProcessControl, (process_info *)a1);
        break;
    case 0x2A:
        p = TournamentChainResume;
        g->maxPlayers = 4;
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
        f = IdleFrames;
        if (f & 1) {
            GameMode = 0x28;
            GotoScriptScene(IntroMovieMsgs, &LevelLoadProcess);
        } else {
            GameState *g = GAME;
            u8 *t = (u8 *)TitlesDemoModes;
            u16 v = *(u16 *)(t + ((f * 2) & 0xC));
            g->demo = 1;
            g->demoTimer = 0x384;
            ProcessControl.next_process = &LevelLoadProcess;
            g->mode = v;
        }
        IdleFrames += 1;
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
    sub->xform.rot.vz = FrameCount * 0x14;
    sub->xform.rot.vy = 0x293;
    sub->xform.hasScale = 1;
    sub->xform.order = 0;
    sub->xform.scale.vz = 0x2800;
    sub->xform.scale.vx = 0x2800;
    sub->xform.dirty = 1;
    eng = GetEngineSlot(1);
    tbl = rcossin_tbl;
    pos = TitlesBackdropPos;
    d = GameFrameCount;
    pos.x += (tbl[(((d * 0x14) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x8F) >> 0xF;
    pos.y += (tbl[(((d * 0x2F) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x17) >> 0xC;
    ang = (tbl[(((d * 0x13) & 0xFFF) - 0x400) & 0xFFF].unk2 * 0x333) >> 0x10;
    p2 = &eng->sub[1];
    *(SLONG_XYZ *)&p2->XYZ.x = pos;
    p2->xform.rot.vy = -0x1C7;
    p2->xform.order = 0;
    p2->xform.rot.vz = ang;
    p2->xform.dirty = 1;
    DoAnimCommand(NULL, eng, p2, &TitlesBackdropAnimator);
    p2 = &eng->sub[2];
    *(SLONG_XYZ *)&p2->XYZ.x = pos;
    p2->xform.order = 0;
    p2->xform.rot.vy = -0x155;
    p2->xform.rot.vz = ang;
    p2->flags |= 0x02000000;
    p2->otz = 0x80;
    p2->xform.dirty = 1;
}

void MainMenuUpdate(void) {
    subengine *s;
    GameState *g;

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
            g->maxPlayers = 2;
            g->teams = 1;
            g->teamLeader[0] = 0;
            g->teamLeader[1] = 2;
            g->teammateChoice[0] = 1;
            g->teammateChoice[1] = 0;
            g->teammateChoice[2] = 3;
            g->teammateChoice[3] = 2;
            g->levelFlags |= 4;
            MenuChainStart(&AdventureChain[1]);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 1:
            g->maxPlayers = 4;
            g->teamLeader[1] = 2;
            ((s8 *)g)[0x54] = -1;
            ((s8 *)g)[0x55] = -1;
            ((s8 *)g)[0x56] = -1;
            ((s8 *)g)[0x57] = -1;
            g->teams = 0;
            g->teamLeader[0] = 0;
            g->levelFlags |= 1;
            MenuChainStart(&BattleChain[1]);
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            g->maxPlayers = 4;
            g->levelFlags |= 2;
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
        s[5].u6C.text = (void *)MainMenuHints[MainMenuCursor];
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
    if (GameMaxPlayers == 2) {
        s[0].XYZ.y = -0x22;
        s[1].XYZ.y = 0;
    } else {
        s[0].XYZ.y = -0x40;
        s[1].XYZ.y = -0x1C;
    }
    if (O69CV->maxPlayers == 2) {
        s[2].flags &= ~0x8000;
        s[3].flags &= ~0x8000;
    }
    n = PadRunEnd;
    if (n > 4) {
        n = 4;
    }
    if (O69CV->maxPlayers < n) {
        n = O69CV->maxPlayers;
    }
    for (i = 0; i < n; i++) {
        s[i].unk7C = 0;
    }
    if (n < O69CV->players) {
        O69CV->players = n;
    }
    if (O69CV->players != 0) {
        s[O69CV->players - 1].unk7C = 2;
    } else {
        O69CV->players = n;
    }
    if (Pads[0].pressed & 0x10) {
        if (O69CV->players >= 2) {
            O69CV->players--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if ((Pads[0].pressed & 0x40) && O69CV->players < n) {
        O69CV->players++;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((O69CV->levelFlags & 4) && O69CV->players == 2) {
        O69CV->teams = 1;
        O69CV->teammateChoice[0] = 1;
        O69CV->teammateChoice[1] = 0;
        O69CV->teammateChoice[2] = 3;
        O69CV->teammateChoice[3] = O69CV->players;
    } else {
        O69CV->teams = 0;
        O69CV->teammateChoice[0] = -1;
        O69CV->teammateChoice[1] = -1;
        O69CV->teammateChoice[2] = -1;
        O69CV->teammateChoice[3] = -1;
    }
    c = O69CV->players;
    O69CV->heroCpu[0] = c < 1;
    O69CV->heroCpu[1] = c < 2;
    O69CV->heroCpu[2] = c < 3;
    O69CV->heroCpu[3] = c < 4;
    O69PAD[0].rumble.on = c < 1;
    O69PAD[1].rumble.on = c < 2;
    O69PAD[2].rumble.on = c < 3;
    O69PAD[3].rumble.on = c < 4;
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
            GotoProcess(&SaveScreenProcess2, 0x200);
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
    sub->xform.rot.vz = FrameCount * 0x14;
    sub->xform.rot.vy = 0x293;
    sub->xform.hasScale = 1;
    sub->xform.scale.vz = 0x2800;
    sub->xform.scale.vx = 0x2800;
    sub->xform.order = 0;
    sub->xform.dirty = 1;
    if (((PadState *)Pads)->pressed & 0x1000) {
        MenuChainBack();
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (((PadState *)Pads)->pressed & 0x80) {
        if (O69OV->battleWins >= 3) {
            PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
            O69OV->battleWins--;
        }
    }
    if ((Pads[0].pressed & 0x20) && O69OV->battleWins < 7) {
        O69OV->battleWins++;
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((Pads[0].pressed & 0x40) && O69OV->aiLevel > 0) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->aiLevel--;
    }
    if ((Pads[0].pressed & 0x10) && O69OV->aiLevel < 2) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        O69OV->aiLevel++;
    }
    *(u8 *)GetEngineSlot(0)->sub[2].u6C.dbase = O69OV->battleWins + '0';
    GetEngineSlot(0)->sub[5].u6C.text = (void *)TextDifficulties[O69OV->language][O69OV->aiLevel];
    p = &GetEngineSlot(0)->sub[8];
    if (O69OV->levelFlags & 1) {
        p->u6C.text = (void *)TextBattleKinds[O69OV->language][0];
    } else {
        p->u6C.text = (void *)TextBattleKinds[O69OV->language][1];
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
    ((subengine *)p)[1].flags &= 0xFFFF7FFF;
    p = (u8 *)GetEngineSlot(1)->sub;
    ((subengine *)p)[2].flags &= 0xFFFF7FFF;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(1));
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(0));
}

void TitlesSetHeroMasks(void) {
    GameState *g = GameInfo;
    s32 i;
    s32 k;

    if (g->teams == 1) {
        i = g->teamLeader[0];
        k = g->teammateChoice[i];
        g->teamSide[0] = (s8)((u8)HeroSide[g->heroChoice[i]] | (u8)HeroSide[g->heroChoice[k]]);
        i = g->teamLeader[1];
        k = g->teammateChoice[i];
        g->teamSide[1] = (s8)((u8)HeroSide[g->heroChoice[i]] | (u8)HeroSide[g->heroChoice[k]]);
        return;
    }
    i = g->teamLeader[0];
    g->teamSide[0] = (s8)HeroSide[g->heroChoice[i]];
}

void OptionsUpdate(void) {
    subengine *sub;
    s32 v;
    s32 n;
    s32 i;
    subengine *cam;

    cam = GetEngineSlot(1)->sub;
    cam->xform.rot.vz = FrameCount * 0x14;
    cam->xform.rot.vy = 0x293;
    cam->xform.hasScale = 1;
    cam->xform.scale.vz = 0x2800;
    cam->xform.scale.vx = 0x2800;
    cam->xform.order = 0;
    cam->xform.dirty = 1;
    sub = GetEngineSlot(4)->sub;
    switch (OptionsPage) {
    case 0:
        sub[OptionsCursor + 3].unk7C = 0;
        if ((((PadState *)Pads)->pressed & 0x40) && OptionsCursor < 2) {
            OptionsCursor++;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if ((Pads[0].pressed & 0x10) && OptionsCursor > 0) {
            OptionsCursor--;
            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        sub[OptionsCursor + 3].unk7C = 2;
        if (((PadState *)Pads)->pressed & 0x4000) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            switch (OptionsCursor) {
            case 0:
                OptionsPage = 1;
                PlaceEngineSlot(4, (PlaceRec *)OptionsSoundPlace);
                break;
            case 1:
                OptionsPage = 2;
                PlaceEngineSlot(4, (PlaceRec *)OptionsControllerPlace);
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
                GameOptionSet *opt = &GameOptions;

                opt->stereo ^= 1;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                if (opt->stereo != 0) {
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
            v = GameOptions.music;
            if (v > 0 && (Pads[0].pressed & 0x80)) {
                v -= 5;
            }
            if (v < 100 && (Pads[0].pressed & 0x20)) {
                v += 5;
            }
            GameOptions.music = v;
            sub[6].unk7C = 2;
            sub[7].unk7C = 2;
            break;
        case 2:
            v = GameOptions.sfx;
            if (v > 0 && (Pads[0].pressed & 0x80)) {
                v -= 5;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (v < 100 && (Pads[0].pressed & 0x20)) {
                v += 5;
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            GameOptions.sfx = v;
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
            if (((PadState *)Pads)[i].pressed & 0x8000) {
                GameOptions.vibrate[i] ^= 1;
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
        PlaceEngineSlot(4, (PlaceRec *)OptionsPlace);
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
        FormatPercent(sub[7].u6C.dbase, GameOptions.music);
        if (GameOptions.stereo != 0) {
            sub[5].flags &= ~0x8000;
            sub[4].flags |= 0x8000;
        } else {
            sub[5].flags |= 0x8000;
            sub[4].flags &= ~0x8000;
        }
        FormatPercent(sub[9].u6C.dbase, GameOptions.sfx);
        break;
    case 2:
        n = PadCount;
        if (n >= 5) {
            n = 4;
        }
        for (i = 0; i < n; i++) {
            if (Pads[i].flags & 8) {
                sub[i * 3 + 5].unk7C = 0;
                if (GameOptions.vibrate[i] != 0) {
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
    eng_render_subs(GetEngineSlot(4));
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(1));
}

void RunGameQuit(void) {
    if (IntroProcess.quit != NULL) {
        IntroProcess.quit();
    }
}

void RunGameRender(void) {
    if (IntroProcess.render != NULL) {
        IntroProcess.render();
    }
}

void RunGameUpdate(void) {
    if (IntroProcess.update != NULL) {
        IntroProcess.update();
    }
}

void RunGameInit(void) {
    init_scripts(0x20, 0x20);
    if (IntroProcess.init != NULL) {
        IntroProcess.init();
    }
    sound_init();
    music_load_track(GameMode);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
    LoadScreenImage(&DatIndex[139]);
    sfx_table_clear();
    sfx_bank_load(&DatIndex[274]);
    while (loader_pending() != 0) {
        loader_update();
        mal_compact(&MainHeap);
        vab_transfer_check();
    }
}

void OptionsQuit(void) {
}

void OptionsInit(void) {
    PlaceEngineSlot(4, (PlaceRec *)OptionsPlace);
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
    PlaceEngineSlot(0, (PlaceRec *)BattleOptionsPlace);
    BattleOptionsState = 2;
}

void TitlesLeaveRender(void) {
    if (CharSelect.render != NULL) {
        CharSelect.render();
    }
}

void TitlesLeaveInit(void) {
    set_fade(&ScreenFade, 0x200, 0);
    GotoScriptScene(IntroMovieMsgs, &WarpReturnProcess);
}

void NewLoadMenuInit(void) {
    GameState *g;
    PlaceEngineSlot(0, (PlaceRec *)NewLoadMenuPlace);
    NewLoadMenuCursor = 0;
    MainMenuCursor = 0;
    g = GAME;
    g->maxPlayers = 2;
    g->teams = 1;
    g->teammateChoice[0] = 1;
    g->teamLeader[0] = 0;
    g->teamLeader[1] = 2;
    g->teammateChoice[1] = 0;
    g->teammateChoice[2] = 3;
    g->teammateChoice[3] = 2;
    g->levelFlags |= 4;
}

void num_players_render(void) {
    RenderState.modelDp = ScreenFade.amount;
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    eng_render_subs(GetEngineSlot(0));
}


void PlayersMenuInit(void) {
    if (GameLevelFlags & 4) {
        PlaceEngineSlot(0, (PlaceRec *)PlayersMenuAdventurePlace);
    } else {
        PlaceEngineSlot(0, (PlaceRec *)PlayersMenuPlace);
    }
    GamePlayers = PadRunEnd != 0;
}

void MainMenuInit(void) {
    GameState *g;
    MainMenuCursor = 0;
    ResetGameState();
    g = GAME;
    g->mode = 0x28;
    MainMenuHintTimer = 0;
    g->levelFlags |= 0x200;
    PlaceEngineSlot(0, (PlaceRec *)MainMenuPlace);
}

void RenderTitlesBackdropAndMenu(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(1));
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(0));
}

void RenderTitlesBackdrop(void) {
    GetEngineSlot(1)->sub[1].flags &= ~0x8000;
    GetEngineSlot(1)->sub[2].flags &= ~0x8000;
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(1));
}

void RenderTitlesMenu(void) {
    GetEngineSlot(1)->sub[1].flags |= 0x8000;
    GetEngineSlot(1)->sub[2].flags |= 0x8000;
    gfx_set_viewport(&TitlesMenuView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(0));
    gfx_set_viewport(&TitlesBackdropView, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(1));
}

void SetupTitlesBackdrop(void) {
    PlaceSub *s = (PlaceSub *)GetEngineSlot(1)->sub;

    s->rz = FrameCount * 20;
    s->ry = 0x293;
    s->unk50 = 1;
    s->sz = 0x2800;
    s->sx = 0x2800;
    s->unk51 = 0;
    s->unk52 = 1;
}

void MenuConfirmOrBack(void) {
    if (((PadState *)Pads)->pressed & 0x4000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainNext();
    }
    if (((PadState *)Pads)->pressed & 0x1000) {
        PlaySfx(0x384, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
}

void TitlesCamFromPath(camera_info_tag *cam, s32 arg1) {
    MapPath *o = (MapPath *)path_get(arg1);
    s32 *p;
    s32 t;

    if (o != NULL) {
        cam->xform.angle.vx = (get_angle(o->keys[1].key - o->keys[0].key,
                                          o->keys[1].pos.y - o->keys[0].pos.y) - 0x400) & 0xFFF;
        cam->xform.angle.vy = 0;
        cam->xform.angle.vz = 0;
        cam->dist = (((s32)(*(u16 *)&o->keys[0].pad10[0] << 16)) >> 18) + 0x200;
        cam->at.x = o->keys[0].pos.x;
        p = (s32 *)&o->keys[0].pos;
        t = p[1];
        cam->at.y = t;
        t = p[2];
        cam->at.z = t;
        cam->eye.y = -p[1];
        cam->eye.x = 0;
        cam->eye.z = o->keys[1].key;
    }
}

void TitlesCameraHook(camera_info_tag *cam, s32 arg1) {
    if (arg1 != 0) {
        TitlesCamFromPath((u8 *)cam, 1);
    }
    GameViewport.ofy = 0xF0;
}

void TitlesEmptyHook(void) {
}

void TitlesUpdateLevel(void) {
    combo_check_all();
}

void TitlesPreInitLevel(void) {
    GameState *g = GAME;

    g->heroes[0] = 9;
    g->heroes[1] = 9;
    g->heroes[2] = 9;
    g->heroes[3] = 9;
    g->levelFlags |= 0x200;
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
    o->sub = s;
    t = e0->sub + arg0;
    o->sub2 = t;
    o->sub3 = e2->sub + (arg0 + 4);
    o->ext = ExtTemplate400;
    o->flags = 0;
    o->flags2 = arg0;
    o->flags3 = 0;
    o->y = Random(0x1E) + 0x1E;
    o->value = Random(0x200) - 0x100;
    o->value2 = Random(0x200) - 0x100;
    s->xform.hasScale = 1;
    s->otz = -0x64;
    s->xform.order = 0;
    s->xform.scale.vx = 0;
    s->xform.scale.vy = 0;
    s->flags |= 0x02000000;
    g = &GamePlayers;
    o->card = arg0;
    do {
    } while (0);
    o->count = 0;
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

    s = o->sub;
    t = o->sub2;
    s2 = o->sub3;
    x = CharCards[o->card].x + CharSelectorOffsets[o->flags2].x;
    y = CharCards[o->card].y + CharSelectorOffsets[o->flags2].y;
    t->XYZ.x = lerp_snap(t->XYZ.x, x, 0x800);
    t->XYZ.y = lerp_snap(t->XYZ.y, y, 0x800);
    still = abs(t->XYZ.x - x) + abs(t->XYZ.y - y);
    s->XYZ.y = s2->XYZ.y = ((TitlesTrig *)rcossin_tbl)[(FrameCount * o->y - 0x400) & 0xFFF].hi + 0x1A4;
    still = still == 0;
    if (o->flags & 1) {
        tgt = CharCards[o->card].target;
        v = o->value;
        v = TitlesSteer(o->scaleX, tgt, v, 0x2BC);
        o->value = v;
        o->scaleX += v;
        s->xform.scale.vx = o->scaleX >> 4;
        v = o->value2;
        v = TitlesSteer(o->scaleY, tgt, v, 0x3E8);
        o->value2 = v;
        o->scaleY += v;
        s->xform.scale.vy = o->scaleY >> 4;
        {
            EXTengine *e = GetEngineSlot(2);
            DoAnimCommand(NULL, e, o->sub, (EXTanimator *)&o->ext);
        }
        s->xform.dirty = 1;
        o->count++;
        return;
    }
    if (o->flags2 < GamePlayers) {
        if (O69PAD[o->flags2].pressed & 0x80) {
            PlaySfx(CharMoveSfx[o->flags2], NULL, NULL, 0x1000, 0x1E00, 0);
            o->card--;
        }
        if (O69PAD[o->flags2].pressed & 0x20) {
            PlaySfx(CharMoveSfx[o->flags2], NULL, NULL, 0x1000, 0x1E00, 0);
            o->card++;
        }
        if (O69PAD[o->flags2].pressed & 0x10) {
            PlaySfx(CharMoveSfx[o->flags2], NULL, NULL, 0x1000, 0x1E00, 0);
            o->card -= 4;
        }
        if (O69PAD[o->flags2].pressed & 0x40) {
            PlaySfx(CharMoveSfx[o->flags2], NULL, NULL, 0x1000, 0x1E00, 0);
            o->card += 4;
        }
    }
    o->card &= 7;
    if ((o->flags2 < ((GameState *)GameInfo)->players && (O69PAD[o->flags2].pressed & 0x4000) && still) || o->flags3 != 0) {
        GetEngineSlot(0)->sub[20].flags &= 0xFFFF7FFF;
        GetEngineSlot(0)->sub[21].flags &= 0xFFFF7FFF;
        card = CharCards;
        if (card[o->card].flags & 1) {
            PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
            return;
        }
        if (o->flags3 == 0) {
            PlaySfx(card[o->card].sfx, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        card[o->card].flags |= 1;
        o->scaleX = 0;
        o->scaleY = 0;
        o->value = 0;
        o->value2 = 0;
        o->flags |= 1;
        {
            s->u6C.dbase = &HeroModelBank[card[o->card].model];
            s->flags |= 0x8000;
        }
        GAME->heroes[o->flags2] = card[o->card].hero;
        GAME->heroChoice[o->flags2] = card[o->card].hero;
        PlaySfx(CharPickSfx[o->flags2], NULL, NULL, 0x1000, 0x1E00, 0);
        o->ext = ExtTemplate400;
        {
            s32 w = CharCards[o->card].anim;
            o->count++;
            o->ext.w[3] = w;
        }
    }
}

void CharSelectInit(void) {
    EXTengine *eng;
    CharCard *rec;
    s32 i;
    s32 off;

    cam_init(&CharSelectCamera);
    *(s32 *)((u8 *)&CharSelectCamera + 8) = 0x400;
    *(s32 *)((u8 *)&CharSelectCamera + 0x18) = 0x190;
    eng = GetEngineSlot(0);
    PlaceEngineSlot(0, (PlaceRec *)CharSelectPlace);
    PlaceEngineSlot(2, (PlaceRec *)CharSelectModelsPlace);
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
        } while (i < ((GameState *)GameInfo)->players);
    }
}

s32 CpuPickCharacter(s32 a0) {
    GameState *g = GAME;
    s32 r;
    s32 i;
    s8 *t;
    s32 v;
    s32 w;
    CharCard *rec;

    if (g->levelFlags & 4) {
        if (g->players == 1) {
            s8 *t = AdventureHeadsSolo;
            s32 v = Random(2) + (a0 - 1) * 2;
            s32 w = (g->heroChoice[0] - 1) * 6;
            r = t[v + w];
        } else {
            s8 *t = AdventureHeadsTeam;
            s32 v = Random(2) + (a0 - 2) * 2 + (g->heroChoice[1] - 1) * 4;
            s32 w = (g->heroChoice[0] - 1) << 5;
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
    s->XYZ.y = ((rcossin_tbl[(((GameFrameCount * 50) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0x6E;
    s->xform.rot.vy = (rcossin_tbl[(((GameFrameCount * 25) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s->xform.rot.vz = (rcossin_tbl[(((GameFrameCount * 36) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s->xform.dirty = 1;
    s2 = &GetEngineSlot(0)->sub[25];
    s2->XYZ.y = ((rcossin_tbl[(((GameFrameCount * 41) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10) + 0xBE;
    s2->xform.rot.vy = (rcossin_tbl[(((GameFrameCount * 35) & 0xFFF) - 0x400) & 0xFFF].unk2 * 75) >> 11;
    s2->xform.rot.vz = (rcossin_tbl[(((GameFrameCount * 26) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 9;
    s2->xform.dirty = 1;
    min = 0x7FFFFFFF;
    for (i = 0; i < ((GameState *)GameInfo)->players; i++) {
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
    eng->XYZ.x = -0x140;
    eng->XYZ.y = -0xF0;
    RenderTitlesBackdropAndMenu();
    gfx_set_viewport(&TitlesMenuView, &CharSelectCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(2));
}

s8 adventure_mode_head(s32 a0) {
    GameState *g = GAME;

    if (g->players == 1) {
        s8 *t = AdventureHeadsSolo;
        s32 v = Random(2) + (a0 - 1) * 2;
        s32 w = (g->heroChoice[0] - 1) * 6;
        return t[v + w];
    } else {
        s8 *t = AdventureHeadsTeam;
        s32 v = Random(2) + (a0 - 2) * 2 + (g->heroChoice[1] - 1) * 4;
        s32 w = (g->heroChoice[0] - 1) << 5;
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
    tbl = (u8 *)&CardSlots[0].state;
outer:
    if (i < 7) {
        j = 0;
        p = (s32 *)e->levels;
        q = p + 1;
        g = GameInfo;
    inner:
        if (j < e->count) {
            if (p[0] == 2) {
                s32 n;
                s32 idx = q[0];
                u8 *gg;
                do { do { do { do {
                    gg = (u8 *)((u8 *)g + idx);  /* matching: an int sum, like PickCpuHeroes' */
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
    PlaceEngineSlot(0, (PlaceRec *)LevelSelectPlace);
    PlaceEngineSlot(2, (PlaceRec *)LevelSelectPlace0);
    PlaceEngineSlot(3, (PlaceRec *)LevelSelectPlace1);
    PlaceEngineSlot(4, (PlaceRec *)LevelSelectPlace2);
    PlaceEngineSlot(5, (PlaceRec *)LevelSelectPlace3);
    PlaceEngineSlot(6, (PlaceRec *)LevelSelectPlace4);
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
    GameState *g;
    EXTengine *e;
    s32 c;

    if ((Pads[0].pressed & 0x80) && (s8)GAME->arena > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        LevelSelectReload = 4;
        GAME->arena--;
    }
    if ((Pads[0].pressed & 0x20) && (s8)GAME->arena < 6) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        LevelSelectReload = 4;
        GAME->arena++;
    }
    if ((Pads[0].pressed & 0x10) && (s8)GAME->arenaLevel > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        GAME->arenaLevel--;
    }
    if ((Pads[0].pressed & 0x40)
        && (s8)GAME->arenaLevel < BattleArenas[(s8)GAME->arena].count - 1) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        GAME->arenaLevel++;
    }
    if (((PadState *)Pads)->pressed & 0x1000) {
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        MenuChainBack();
    }
    if (((PadState *)Pads)->pressed & 0x4000) {
        if (TitlesSlotOn()) {
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            MenuChainNext();
        } else {
            PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
    if (TitlesSlotOn()) {
        GetEngineSlot(0)->sub->u6C.dbase = (void *)TitlesSlotRowI((s8)GAME->arena, (s8)GAME->arenaLevel);
        GetEngineSlot(0)->sub->XYZ.y = 8;
        GetEngineSlot(2)->sub->u6C.dbase = (void *)TitlesSlotModelI((s8)GAME->arena, (s8)GAME->arenaLevel);
        GetEngineSlot(2)->sub->entity = TitlesSlotCI((s8)GAME->arena, (s8)GAME->arenaLevel);
        GetEngineSlot(0)->sub[4].flags |= 0x8000;
        GetEngineSlot(0)->sub[9].flags &= ~0x8000;
        return;
    }
    e = GetEngineSlot(0);
    tbl = LangLocked;
    g = GAME;
    e->sub->u6C.text = (void *)tbl[g->language];
    GetEngineSlot(2)->sub->entity = 0;
    GetEngineSlot(0)->sub[4].flags &= ~0x8000;
    GetEngineSlot(0)->sub->XYZ.y = 0x1C;
    GetEngineSlot(0)->sub[9].flags |= 0x8000;
    if ((c = ((BattleLevel *)((u8 *)BattleArenas[(s8)GAME->arena].levels + ((s8)GAME->arenaLevel << 4)))->type == 2)) {
        if (CardStatus & 2) {
            if (CardStatus & 0x40) {
                GetEngineSlot(0)->sub[9].u6C.text = (void *)LangCheckingCard[GAME->language];
            } else if (CardStatus & 0x10) {
                if (UnlockCardLevels(0) != 0) {
                    *(s32 *)&GetEngineSlot(0)->sub[9].u6C.text = LangPressToUnlockLevels[GAME->language];
                    if (Pads[0].pressed & 0x2000) {
                        PlaySfx(0x396, NULL, NULL, 0x1000, 0x1E00, 0);
                        UnlockCardLevels(1);
                    }
                } else {
                    GetEngineSlot(0)->sub[9].u6C.text = (void *)LangNoMoreUnlocked[GAME->language];
                }
            } else {
                GetEngineSlot(0)->sub[9].u6C.text = (void *)LangInsertCard[GAME->language];
            }
        } else {
            GetEngineSlot(0)->sub[9].u6C.text = (void *)LangInsertCard[GAME->language];
        }
    } else {
        GetEngineSlot(0)->sub[9].u6C.dbase = NULL;
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
    GameState *h;
    BattleArena *tbl;
    TrigEntry *trig;

    if (LevelSelectReload != 0) {
        prims_empty_back();
        mal_compact_step(&MainHeap);
        LevelSelectReload--;
    }
    AnimateTitlesBackdrop();
    a = &HeroModelBank[DB_MainmenuModels];
    texture_update_anims(&((obj_dbase *)a)->textures);
    texture_update_anims(&((obj_dbase *)a)[1].textures);
    val = LevelSelectBusy;
    if (val == 0) {
        LevelSelectInput();
    }
    e0 = GetEngineSlot(0);
    boff = ((BattleLevel *)((u8 *)BattleArenas[(s8)GAME->arena].levels + ((s8)GAME->arenaLevel << 4)))->idx;
    w = ModeTypes[boff * 3] - 0x101;
    off = ((s32 **)LangModeTypeTexts)[GAME->language][w];
    e0->sub[6].u6C.dbase = (void *)off;
    t = &GetEngineSlot(0)->sub[1];
    t->XYZ.y = lerp_fix12(t->XYZ.y,
        *(s16 *)((u8 *)&GetEngineSlot(1)->sub[(s8)GAME->arenaLevel + 4] + 0x72) + 0x32, 0x800);
    i = 0;
    off = 0;
    for (;;) {
        s32 type;
        s32 idx;
        s32 on;
        s32 fl;
        EXTengine *e;

        if (i >= (BattleArenas + (s8)GAME->arena)->count) {
            break;
        }
        GetEngineSlot(1)->sub[off + 4].flags |= 0x8000;
        e = GetEngineSlot(i + 3);
        fl = GAME->cheats & 0x2000;
        slot = &((BattleLevel *)BattleArenas[(s8)GAME->arena].levels)[off];
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
            s->u6C.dbase = ((BattleLevel *)((u8 *)BattleArenas[(s8)GAME->arena].levels + (off << 4)))->model;
            c = ((BattleLevel *)((u8 *)BattleArenas[(s8)GAME->arena].levels + (off << 4)))->anim;
            s->entity = c;
            s->xform.rot.vx = 0x7D;
            s->xform.rot.vy = 0;
            s->xform.rot.vz = 0;
            s->XYZ.x = rcossin_tbl[(((GameFrameCount * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;
            s->XYZ.y = (rcossin_tbl[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
        } else {
            s->u6C.dbase = &HeroModelBank[DB_MainmenuModels];
            s->entity = 0x2006;
            s->xform.rot.vx = 0;
            s->xform.rot.vy = FrameCount * 50 + (i << 10);
            val = rcossin_tbl[(((GameFrameCount * 100 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2;
            s->XYZ.x = 0;
            s->XYZ.y = 0;
            s->xform.rot.vz = (val * 25) >> 9;
        }
        off++;
        i++;
        s->xform.order = 0;
        s->xform.dirty = 1;
    }
    for (i = (GameArena + BattleArenas)->count; i < 4; i++) {
        GetEngineSlot(1)->sub[i + 4].flags &= ~0x8000;
        GetEngineSlot(i + 3)->sub->entity = 0;
    }
    h = GAME;
    NextGameMode = ((BattleLevel *)((u8 *)BattleArenas[(s8)h->arena].levels + ((s8)h->arenaLevel << 4)))->idx;
    q = GetEngineSlot(2)->sub;
    q->xform.rot.vx = 0x7D;
    q->xform.order = 0;
    q->XYZ.x = rcossin_tbl[(((GameFrameCount * 20 + (i << 11)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 128 >> 12;    q->XYZ.y = (rcossin_tbl[(((GameFrameCount * 70 + (i << 9)) & 0xFFF) - 0x400) & 0xFFF].unk2 * 5) >> 10;
    q->xform.dirty = 1;
}

void LevelSelectRender(void) {
    s32 *cam = (s32 *)&TitlesCamera;
    s8 *m;

    gfx_set_viewport(&LevelSelectView0, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(2));
    RenderTitlesBackdropAndMenu();
    m = (s8 *)GameInfo;
    cam[2] = (m[0x37] != 0) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView1, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(3));
    cam[2] = (m[0x37] != 1) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView2, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(4));
    cam[2] = (m[0x37] != 2) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView3, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(5));
    cam[2] = (m[0x37] != 3) ? 0x400 : 0x578;
    gfx_set_viewport(&LevelSelectView4, &TitlesCamera);
    RenderState.modelDp = ScreenFade.amount;
    eng_render_subs(GetEngineSlot(6));
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
    s32 v = *(s16 *)((u8 *)GetEngineSlot(1)->sub + GameArenaLevel * 0xA8 + 0x312) + 0x32;

    if (arg0 != 0) {
        ((subengine *)p)->XYZ.y = lerp_fix12(((subengine *)p)->XYZ.y, v, 0x800);
    } else {
        ((subengine *)p)->XYZ.y = v;
    }
}

s32 IsLevelUnlocked(s32 arg0) {
    u8 *g = GameInfo;
    u8 *e = (u8 *)BattleArenas[(s8)g[0x36]].levels + (arg0 << 4);
    s32 off = ((BattleLevel *)e)->idx;
    s32 type = *(s32 *)e;
    u8 *p;

    if ((((GameState *)g)->cheats & 0x2000) || type == 1
        || ((p = g + off), type == 2)
            && (p[0xE4] != 0 || p[0x168] != 0)) {
        return 1;
    }
    return 0;
}

s32 IsCardLevel(s32 arg0) {
    s32 i = GameArena;
    u8 *s = (u8 *)BattleArenas[i].levels;
    return *(s32 *)(s + (arg0 << 4)) == 2;
}

void TitlesEmptyStub(void) {
}

s32 LevelListName(s32 arg0, s32 arg1) {
    s32 idx = *(s32 *)(BattleArenas[arg0].levels + (arg1 << 4) + 4);
    s32 w = ModeTypes[idx * 3] - 0x101;
    s32 **tbl = (s32 **)LangModeTypeTexts;
    return tbl[GameLanguage][w];
}

s32 LevelListModel(s32 arg0, s32 arg1) {
    return *(s32 *)(BattleArenas[arg0].levels + (arg1 << 4) + 8);
}

s32 LevelListAnim(s32 a, s32 b) {
    return *(s32 *)(BattleArenas[a].levels + (b << 4) + 0xC);
}

s32 LevelListCount(s32 arg0) {
    return BattleArenas[arg0].count;
}

s32 LevelListText(s32 a, s32 b) {
    s32 base = BattleArenas[a].levels;
    s32 idx = *(s32 *)(FILE_AT(base, b << 4) + 4);
    s32 *row = LangLevelNames[GameLanguage];
    return row[idx];
}

s32 LevelListType(s32 a, s32 b) {
    return *(s32 *)((u8 *)BattleArenas[a].levels + (b << 4));
}

s32 LevelListLevel(s32 a, s32 b) {
    return *(s32 *)((u8 *)BattleArenas[a].levels + (b << 4) + 4);
}

s32 AnyCardHasLevel(s32 idx) {
    u8 *p;
    s32 i;

    i = 0;
    p = (u8 *)&CardSlots[0].state;
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
    subengine *s;     /* the slot's subs, one after another */

    s = GetEngineSlot(0)->sub;
    p = &TeamModePresets[TeamModePreset];
    s[0].entity = TeamModeHeroFrames[O69PV->heroChoice[p->hero[0]]];
    s[1].entity = TeamModeHeroFrames[O69PV->heroChoice[p->hero[1]]];
    s[2].entity = TeamModeHeroFrames[O69PV->heroChoice[p->hero[2]]];
    s[3].entity = TeamModeHeroFrames[O69PV->heroChoice[p->hero[3]]];
    s[8].flags &= ~0x8000;
    s[9].flags &= ~0x8000;
    s[10].flags &= ~0x8000;
    s[8].flags |= p->flags[0];
    s[9].flags |= p->flags[1];
    s[10].flags |= p->flags[2];

    TITLES_SETBAR(0, p->val[0]);
    TITLES_SETBAR(1, p->val[1]);
    TITLES_SETBAR(2, p->val[2]);
    TITLES_SETBAR(3, p->val[3]);

    s[13].u6C.ptr = (void *)p->tbl[O69PV->language];
    O69PV->teams = p->flag != 0;
    O69PV->teamLeader[0] = p->leader0;
    O69PV->teamLeader[1] = p->leader1;
    O69PV->teammateChoice[0] = p->mate0;
    O69PV->teammateChoice[1] = p->mate1;
    O69PV->teammateChoice[2] = p->mate2;
    O69PV->teammateChoice[3] = p->mate3;
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
    PlaceEngineSlot(0, (PlaceRec *)TeamModePlace);
    TeamModeRow = 0;
    TeamModePreset = 0;
}

void MoveTeamBar(s32 arg0, s32 arg1) {
    EXTengine *eng = GetEngineSlot(0);
    s32 off = arg0 * 0xA8;
    u8 *p = (u8 *)eng->sub + (off + 0x2A0);
    s16 v;

    v = lerp_fix12(*(s16 *)&((subengine *)p)->u70.w, arg1, 0x800);
    *(s16 *)&((subengine *)p)->u70.w = v;
    ((subengine *)p)->scale = v + 0x78;
    ((subengine *)((u8 *)eng->sub + off))->XYZ.x = *(s16 *)&((subengine *)p)->u70.w + 0x1A;
}

void TitlesShellScreenRender(void) {
    RenderSaveMenu();
}

void TitlesShellScreenUpdate(void) {
}

void TitlesShellScreenInit(void) {
    InitCardMenu();
}

void TitlesExtraScreenRender(void) {
    eng_render_subs(GetEngineSlot(2));
}

void TitlesExtraScreenInit(void) {
    PlaceEngineSlot(2, (PlaceRec *)TitlesExtraPlace);
}
