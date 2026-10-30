#ifndef BASH_MODELBANK_H
#define BASH_MODELBANK_H

/* The model bank: 283 dbase records (obj_dbase), one per model a game can load, each
 * with the archive's .mdl and .tex it loads by default. HeroModelBank[DB_x]; the hero
 * tables (HeroHooks.HeroModels) and the scripts hold the indices.
 * owner: src/main/dbase.c */

#include "common.h"

/* An archive entry (DatIndex): the demos' C++ class `file` (asload_permanent_callback(file *, ...)). */
typedef struct file {
    /* 0x00 */ s32 sector;   /* its first sector in CRASHBSH.DAT */
    /* 0x04 */ s32 size;     /* bytes */
} file;

/* A texture set (a dbase's, at +0x10): its textures and their scroll
 * animations. The name is the C++ symbols'
 * (texture_init_mem_textureset__FP14obj_textureset). */
typedef struct obj_textureset {
    /* 0x00 */ s32 nTex;
    /* 0x04 */ s32 nEnt;
    /* 0x08 */ struct obj_texture *tex; /* nTex obj_textures */
    /* 0x0C */ struct TexClut *ent;
    /* 0x10 */ struct TexFile *base; /* the texture file, until texture_release_loading_mem */
    /* 0x14 */ s32 f14;                /* the file header's word at 0x14 */
    /* 0x18 */ struct AnimData *anims; /* in base, or NULL */
    /* 0x1C */ s32 nAnim;
    /* 0x20 */ u8 *anim;
} obj_textureset;

/* A dbase: one of the 0x38-byte records of HeroModelBank, a loaded file and its textures
 * (dbase_asload_with_anims loads it, the callbacks fill it). The name is the C++ symbols'
 * (dbase_mesh_anim_name_loaded__FP10_obj_dbasePc; the typedef is in bash/core.h). */
struct obj_dbase {
    /* 0x00 */ file *file;            /* &DatIndex[N]: loaded into data once the textures are in */
    /* 0x04 */ file *textureFile;     /* &DatIndex[N]: loaded first, into textures */
    /* 0x08 */ void *load;             /* the load in flight, or 0 */
    /* 0x0C */ struct ModelFile *data;
    /* 0x10 */ obj_textureset textures;
    /* 0x34 */ s32 *keep;             /* name hashes of the blocks to keep (dbase_setup_blocks) */
};

/* The records, by index: the ones the code names by slot keep the slot's name, the rest
 * are named from their default .mdl. */
enum DbaseIds {
    DB_Empty, /* 0: (none) */
    DB_CharsCrateCrash, /* 1: models\chars\crate\crash.mdl */
    DB_CharsCrateCoco, /* 2: models\chars\crate\coco.mdl */
    DB_CharsCrateCortex, /* 3: models\chars\crate\cortex.mdl */
    DB_CharsCrateBrio, /* 4: models\chars\crate\brio.mdl */
    DB_CharsCrateTiny, /* 5: models\chars\crate\tiny.mdl */
    DB_CharsCrateKong, /* 6: models\chars\crate\kong.mdl */
    DB_CharsCrateDingo, /* 7: models\chars\crate\dingo.mdl */
    DB_CharsCrateRilla, /* 8: models\chars\crate\rilla.mdl */
    DB_CharsPogoCrash, /* 9: models\chars\pogo\crash.mdl */
    DB_CharsPogoCoco, /* 10: models\chars\pogo\coco.mdl */
    DB_CharsPogoCortex, /* 11: models\chars\pogo\cortex.mdl */
    DB_CharsPogoBrio, /* 12: models\chars\pogo\brio.mdl */
    DB_CharsPogoTiny, /* 13: models\chars\pogo\tiny.mdl */
    DB_CharsPogoDingo, /* 14: models\chars\pogo\dingo.mdl */
    DB_CharsPogoKong, /* 15: models\chars\pogo\kong.mdl */
    DB_CharsPogoRilla, /* 16: models\chars\pogo\rilla.mdl */
    DB_CharsPolar2Crash, /* 17: models\chars\polar2\crash.mdl */
    DB_CharsPolar2Coco, /* 18: models\chars\polar2\coco.mdl */
    DB_CharsPolar2Cortex, /* 19: models\chars\polar2\cortex.mdl */
    DB_CharsPolar2Tiny, /* 20: models\chars\polar2\tiny.mdl */
    DB_CharsPolar2Dingo, /* 21: models\chars\polar2\dingo.mdl */
    DB_CharsPolar2Brio, /* 22: models\chars\polar2\brio.mdl */
    DB_Powerups, /* 23: models\powerups15.mdl */
    DB_CrateObjects, /* 24: models\arena\crate_snow\objects.mdl */
    DB_RingObjects, /* 25: models\arena\medieval_ring\objects.mdl */
    DB_Angel, /* 26: models\angel10.mdl */
    DB_Penguin, /* 27: models\arena\crate_snow\penguin2.mdl */
    DB_Interface, /* 28: models\interface\interface22.mdl */
    DB_InterfaceInterface22, /* 29: models\interface\interface22.mdl */
    DB_InterfaceInterface22_2, /* 30: models\interface\interface22.mdl */
    DB_InterfaceInterface22_3, /* 31: models\interface\interface22.mdl */
    DB_InterfaceInterface22_4, /* 32: models\interface\interface22.mdl */
    DB_InterfaceInterface22_5, /* 33: models\interface\interface22.mdl */
    DB_ArenaCrateSpaceArena, /* 34: models\arena\crate_space\arena.mdl */
    DB_ArenaCrateSpaceCrystalarena, /* 35: models\arena\crate_space\crystalarena.mdl */
    DB_ArenaCrateJungleArena, /* 36: models\arena\crate_jungle\arena.mdl */
    DB_ArenaCrateJungleCrystalarena, /* 37: models\arena\crate_jungle\crystalarena.mdl */
    DB_ArenaCrateSnowArena, /* 38: models\arena\crate_snow\arena.mdl */
    DB_ArenaCrateSnowCrystalarena, /* 39: models\arena\crate_snow\crystalarena.mdl */
    DB_ArenaCrateDrainArena, /* 40: models\arena\crate_drain\arena.mdl */
    DB_ArenaCrateDrainCrystalarena, /* 41: models\arena\crate_drain\crystalarena.mdl */
    DB_ArenaCrateTiebreakerArena, /* 42: models\arena\crate_tiebreaker\arena.mdl */
    DB_ArenaBossPapuArena, /* 43: models\arena\boss_papu\arena.mdl */
    DB_Papu, /* 44: models\arena\boss_papu\papu2.mdl */
    DB_Minion, /* 45: models\arena\boss_papu\minion2.mdl */
    DB_ArenaTankSwampArena, /* 46: models\arena\tank_swamp\arena.mdl */
    DB_ArenaTankDesertArena, /* 47: models\arena\tank_desert\arena.mdl */
    DB_ArenaTankJungleArena, /* 48: models\arena\tank_jungle\arena.mdl */
    DB_ArenaTankMetalArena, /* 49: models\arena\tank_metal\arena.mdl */
    DB_ArenaBossKomodoArena, /* 50: models\arena\boss_komodo\arena.mdl */
    DB_ArenaTankSwampCrystalarena, /* 51: models\arena\tank_swamp\crystalarena.mdl */
    DB_ArenaTankDesertCrystalarena, /* 52: models\arena\tank_desert\crystalarena.mdl */
    DB_ArenaTankJungleCrystalarena, /* 53: models\arena\tank_jungle\crystalarena.mdl */
    DB_ArenaTankMetalCrystalarena, /* 54: models\arena\tank_metal\crystalarena.mdl */
    DB_TankObjects, /* 55: models\arena\tank_swamp\objects.mdl */
    DB_TankObjects2, /* 56: models\arena\tank_unk4\objects.mdl */
    DB_ArenaBossKomodoArenaobjects, /* 57: models\arena\boss_komodo\arenaobjects.mdl */
    DB_ArenaBossKomodoKomodo, /* 58: models\arena\boss_komodo\komodo.mdl */
    DB_ArenaBossKomodoKomodotank, /* 59: models\arena\boss_komodo\komodotank.mdl */
    DB_TankVehicle, /* 60: models\arena\tank_swamp\vehicle2.mdl */
    DB_ArenaTankJungleObjects, /* 61: models\arena\tank_jungle\objects.mdl */
    DB_CharsTankCrash, /* 62: models\chars\tank\crash.mdl */
    DB_CharsTankCoco, /* 63: models\chars\tank\coco.mdl */
    DB_CharsTankCortex, /* 64: models\chars\tank\cortex.mdl */
    DB_CharsTankTiny, /* 65: models\chars\tank\tiny.mdl */
    DB_CharsTankBrio, /* 66: models\chars\tank\brio.mdl */
    DB_CharsTankDingo, /* 67: models\chars\tank\dingo.mdl */
    DB_CharsTankKong, /* 68: models\chars\tank\kong.mdl */
    DB_CharsTankRilla, /* 69: models\chars\tank\rilla.mdl */
    DB_ArenaMedievalChickenChicken, /* 70: models\arena\medieval_chicken\chicken.mdl */
    DB_ArenaPogoPainterArena, /* 71: models\arena\pogo_painter\arena.mdl */
    DB_ArenaPogoGogoArena, /* 72: models\arena\pogo_gogo\arena.mdl */
    DB_ArenaPogoEplArena, /* 73: models\arena\pogo_epl\arena.mdl */
    DB_ArenaPogoPadlockArena, /* 74: models\arena\pogo_padlock\arena.mdl */
    DB_ArenaPogoPainterCrystalarena, /* 75: models\arena\pogo_painter\crystalarena.mdl */
    DB_ArenaPogoGogoCrystalarena, /* 76: models\arena\pogo_gogo\crystalarena.mdl */
    DB_ArenaPogoEplCrystalarena, /* 77: models\arena\pogo_epl\crystalarena.mdl */
    DB_ArenaPogoPadlockCrystalarena, /* 78: models\arena\pogo_padlock\crystalarena.mdl */
    DB_PogoObjects, /* 79: models\arena\pogo_unk\powerups.mdl */
    DB_Roo, /* 80: models\arena\pogo_epl\roo2.mdl */
    DB_ArenaMedievalMalletArena, /* 81: models\arena\medieval_mallet\arena.mdl */
    DB_ArenaMedievalMalletCrystalarena, /* 82: models\arena\medieval_mallet\crystalarena.mdl */
    DB_MalletObjects, /* 83: models\arena\medieval_mallet\modeobjects2.mdl */
    DB_CharsMalletCrash, /* 84: models\chars\mallet\crash.mdl */
    DB_CharsMalletCoco, /* 85: models\chars\mallet\coco.mdl */
    DB_CharsMalletCortex, /* 86: models\chars\mallet\cortex.mdl */
    DB_CharsMalletBrio, /* 87: models\chars\mallet\brio.mdl */
    DB_CharsMalletDingo, /* 88: models\chars\mallet\dingo.mdl */
    DB_CharsMalletRilla, /* 89: models\chars\mallet\rilla.mdl */
    DB_CharsMalletTiny, /* 90: models\chars\mallet\tiny.mdl */
    DB_CharsMalletKong, /* 91: models\chars\mallet\kong.mdl */
    DB_ArenaBossOxideArena, /* 92: models\arena\boss_oxide\arena.mdl */
    DB_Oxide, /* 93: models\arena\boss_oxide\oxidevehicle.mdl */
    DB_PlayerVehicle, /* 94: models\arena\boss_oxide\playervehicle2.mdl */
    DB_ArenaMedievalKegArena, /* 95: models\arena\medieval_keg\arena.mdl */
    DB_ArenaMedievalKegCrystalarena, /* 96: models\arena\medieval_keg\crystalarena.mdl */
    DB_KegObjects, /* 97: models\arena\medieval_keg\objects.mdl */
    DB_CharsKegsCrash, /* 98: models\chars\kegs\crash.mdl */
    DB_CharsKegsCoco, /* 99: models\chars\kegs\coco.mdl */
    DB_CharsKegsCortex, /* 100: models\chars\kegs\cortex.mdl */
    DB_CharsKegsTiny, /* 101: models\chars\kegs\tiny.mdl */
    DB_CharsKegsBrio, /* 102: models\chars\kegs\brio.mdl */
    DB_CharsKegsDingo, /* 103: models\chars\kegs\dingo.mdl */
    DB_CharsKegsKong, /* 104: models\chars\kegs\kong.mdl */
    DB_CharsKegsRilla, /* 105: models\chars\kegs\rilla.mdl */
    DB_ArenaBallsCrashArena, /* 106: models\arena\balls_crash\arena.mdl */
    DB_ArenaBallsCrashCrystalarena, /* 107: models\arena\balls_crash\crystalarena.mdl */
    DB_ArenaBallsBeachArena, /* 108: models\arena\balls_beach\arena.mdl */
    DB_ArenaBallsBeachCrystalarena, /* 109: models\arena\balls_beach\crystalarena.mdl */
    DB_ArenaBallsNginArena, /* 110: models\arena\balls_ngin\arena.mdl */
    DB_ArenaBallsNginCrystalarena, /* 111: models\arena\balls_ngin\crystalarena.mdl */
    DB_ArenaBallsSkyArena, /* 112: models\arena\balls_sky\arena.mdl */
    DB_ArenaBallsSkyCrystalarena, /* 113: models\arena\balls_sky\crystalarena.mdl */
    DB_CharsBallCrash, /* 114: models\chars\ball\crash.mdl */
    DB_CharsBallCoco, /* 115: models\chars\ball\coco.mdl */
    DB_CharsBallCortex, /* 116: models\chars\ball\cortex.mdl */
    DB_CharsBallBrio, /* 117: models\chars\ball\brio.mdl */
    DB_CharsBallDingo, /* 118: models\chars\ball\dingo.mdl */
    DB_CharsBallRilla, /* 119: models\chars\ball\rilla.mdl */
    DB_CharsBallTiny, /* 120: models\chars\ball\tiny.mdl */
    DB_CharsBallKong, /* 121: models\chars\ball\kong.mdl */
    DB_BallsEffects, /* 122: models\arena\balls_unk2\objects.mdl */
    DB_BallsVehicle, /* 123: models\arena\balls_unk2\vehicle.mdl */
    DB_Ngin, /* 124: models\arena\balls_ngin\ngin.mdl */
    DB_ArenaMedievalRingArena, /* 125: models\arena\medieval_ring\arena.mdl */
    DB_ArenaMedievalRingCrystalarena, /* 126: models\arena\medieval_ring\crystalarena.mdl */
    DB_Unkscenery, /* 127: models\unkscenery.mdl */
    DB_Polar, /* 128: models\arena\polar\bear_reflection.mdl */
    DB_Masks, /* 129: models\masks.mdl */
    DB_PolarObjects, /* 130: models\arena\polar_unk5\objects.mdl */
    DB_ArenaPolarPolarArena, /* 131: models\arena\polar_polar\arena.mdl */
    DB_ArenaPolarMeltArena, /* 132: models\arena\polar_melt\arena.mdl */
    DB_ArenaPolarTiltSeal, /* 133: models\arena\polar_tilt\seal.mdl */
    DB_ArenaPolarManicArena, /* 134: models\arena\polar_manic\arena.mdl */
    DB_CharsCrate2Crash, /* 135: models\chars\crate2\crash.mdl */
    DB_CharsCrate2Coco, /* 136: models\chars\crate2\coco.mdl */
    DB_CharsCrate2Cortex, /* 137: models\chars\crate2\cortex.mdl */
    DB_CharsCrate2Tiny, /* 138: models\chars\crate2\tiny.mdl */
    DB_CharsCrate2Dingo, /* 139: models\chars\crate2\dingo.mdl */
    DB_CharsCrate2Brio, /* 140: models\chars\crate2\brio.mdl */
    DB_ArenaBossBearArena, /* 141: models\arena\boss_bear\arena.mdl */
    DB_ArenaBossBearBigbear, /* 142: models\arena\boss_bear\bigbear.mdl */
    DB_Modelmix, /* 143: models\modelmix.mdl */
    DB_CharsPolar2Kong, /* 144: models\chars\polar2\kong.mdl */
    DB_RoboPolar, /* 145: models\arena\boss_bear\robopolar2.mdl */
    DB_CharsPolar2Rilla, /* 146: models\chars\polar2\rilla.mdl */
    DB_ArenaPolarTiltCrystalarena, /* 147: models\arena\polar_tilt\crystalarena.mdl */
    DB_CharsCrate2Rilla, /* 148: models\chars\crate2\rilla.mdl */
    DB_CharsCrate2Kong, /* 149: models\chars\crate2\kong.mdl */
    DB_ArenaPolarManicCrystalarena, /* 150: models\arena\polar_manic\crystalarena.mdl */
    DB_ArenaPolarMeltCrystalarena, /* 151: models\arena\polar_melt\crystalarena.mdl */
    DB_ArenaPolarPolarCrystalarena, /* 152: models\arena\polar_polar\crystalarena.mdl */
    DB_CharsPolarCrash, /* 153: models\chars\polar\crash.mdl */
    DB_CharsPolarCoco, /* 154: models\chars\polar\coco.mdl */
    DB_CharsPolarCortex, /* 155: models\chars\polar\cortex.mdl */
    DB_CharsPolarTiny, /* 156: models\chars\polar\tiny.mdl */
    DB_CharsPolarDingo, /* 157: models\chars\polar\dingo.mdl */
    DB_CharsPolarBrio, /* 158: models\chars\polar\brio.mdl */
    DB_CharsPolarRilla, /* 159: models\chars\polar\rilla.mdl */
    DB_CharsPolarKong, /* 160: models\chars\polar\kong.mdl */
    DB_ArenaDashDotArena, /* 161: models\arena\dash_dot\arena.mdl */
    DB_CharsRaceCrash, /* 162: models\chars\race\crash.mdl */
    DB_CharsRaceCortex, /* 163: models\chars\race\cortex.mdl */
    DB_CharsRaceTiny, /* 164: models\chars\race\tiny.mdl */
    DB_CharsRaceDingo, /* 165: models\chars\race\dingo.mdl */
    DB_CharsRaceBrio, /* 166: models\chars\race\brio.mdl */
    DB_ArenaDashToxicArena, /* 167: models\arena\dash_toxic\arena.mdl */
    DB_ArenaDashSplashArena, /* 168: models\arena\dash_splash\arena.mdl */
    DB_ArenaDashDanteArena, /* 169: models\arena\dash_dante\arena.mdl */
    DB_CrateEffects, /* 170: models\unkrocket.mdl */
    DB_CharsRaceCoco, /* 171: models\chars\race\coco.mdl */
    DB_CharsRaceRilla, /* 172: models\chars\race\rilla.mdl */
    DB_CharsRaceKong, /* 173: models\chars\race\kong.mdl */
    DB_CharsSplashCrash, /* 174: models\chars\splash\crash.mdl */
    DB_CharsSplashCortex, /* 175: models\chars\splash\cortex.mdl */
    DB_CharsSplashTiny, /* 176: models\chars\splash\tiny.mdl */
    DB_CharsSplashDingo, /* 177: models\chars\splash\dingo.mdl */
    DB_CharsSplashKong, /* 178: models\chars\splash\kong.mdl */
    DB_CharsSplashBrio, /* 179: models\chars\splash\brio.mdl */
    DB_CharsSplashCoco, /* 180: models\chars\splash\coco.mdl */
    DB_CharsSplashRilla, /* 181: models\chars\splash\rilla.mdl */
    DB_ArenaDashToxicCrystalarena, /* 182: models\arena\dash_toxic\crystalarena.mdl */
    DB_ArenaDashDanteCrystalarena, /* 183: models\arena\dash_dante\crystalarena.mdl */
    DB_ArenaDashDotCrystalarena, /* 184: models\arena\dash_dot\crystalarena.mdl */
    DB_ArenaDashSplashCrystalarena, /* 185: models\arena\dash_splash\crystalarena.mdl */
    DB_ArenaMedievalDragonCrystalarena, /* 186: models\arena\medieval_dragon\crystalarena.mdl */
    DB_CharsDinoCrash, /* 187: models\chars\dino\crash.mdl */
    DB_CharsDinoCortex, /* 188: models\chars\dino\cortex.mdl */
    DB_CharsDinoTiny, /* 189: models\chars\dino\tiny.mdl */
    DB_CharsDinoDingo, /* 190: models\chars\dino\dingo.mdl */
    DB_CharsDinoCoco, /* 191: models\chars\dino\coco.mdl */
    DB_CharsDinoRilla, /* 192: models\chars\dino\rilla.mdl */
    DB_CharsDinoBrio, /* 193: models\chars\dino\brio.mdl */
    DB_CharsDinoKong, /* 194: models\chars\dino\kong.mdl */
    DB_ArenaMedievalDragonArena, /* 195: models\arena\medieval_dragon\arena.mdl */
    DB_DragonObjects, /* 196: models\arena\test\arena.mdl */
    DB_ArenaTestObjects, /* 197: models\arena\test\objects.mdl */
    DB_Loading, /* 198: models\loading\level20.mdl */
    DB_MainmenuModels, /* 199: models\mainmenu\models.mdl */
    DB_MainmenuArenaModels2, /* 200: models\mainmenu\arena_models2.mdl */
    DB_MainmenuArenaModels1, /* 201: models\mainmenu\arena_models1.mdl */
    DB_Font, /* 202: fonts\font22.mdl */
    DB_CutsceneIntroEurocom, /* 203: models\cutscene\intro_eurocom.mdl */
    DB_CutsceneIntroLogo2, /* 204: models\cutscene\intro_logo2.mdl */
    DB_MainmenuModels2, /* 205: models\mainmenu\models2.mdl */
    DB_WarpRoom1Level, /* 206: models\warp_room1\level.mdl */
    DB_WarpRoom2Level, /* 207: models\warp_room2\level.mdl */
    DB_WarpRoom3Level, /* 208: models\warp_room3\level.mdl */
    DB_WarpRoom4Level, /* 209: models\warp_room4\level.mdl */
    DB_WarpRoom5Level, /* 210: models\warp_room5\level.mdl */
    DB_DemoHub2Level, /* 211: models\demo_hub2\level.mdl */
    DB_DemoHub1Level, /* 212: models\demo_hub1\level.mdl */
    DB_AdventureItems, /* 213: models\adventure_items_void.mdl */
    DB_CharsWarpCrash, /* 214: models\chars\warp\crash.mdl */
    DB_CharsWarpCoco, /* 215: models\chars\warp\coco.mdl */
    DB_CharsWarpCortex, /* 216: models\chars\warp\cortex.mdl */
    DB_CharsWarpBrio, /* 217: models\chars\warp\brio.mdl */
    DB_CharsWarpTiny, /* 218: models\chars\warp\tiny.mdl */
    DB_CharsWarpKong, /* 219: models\chars\warp\kong.mdl */
    DB_CharsWarpDingo, /* 220: models\chars\warp\dingo.mdl */
    DB_CharsWarpRilla, /* 221: models\chars\warp\rilla.mdl */
    DB_CutsceneIntroLogo, /* 222: models\cutscene\intro_logo.mdl */
    DB_CutsceneLevelShot19, /* 223: models\cutscene\level_shot19.mdl */
    DB_CutsceneLevelShot18, /* 224: models\cutscene\level_shot18.mdl */
    DB_CutsceneLevelShot17, /* 225: models\cutscene\level_shot17.mdl */
    DB_CutsceneLevelShot16, /* 226: models\cutscene\level_shot16.mdl */
    DB_CutsceneLevelShot15, /* 227: models\cutscene\level_shot15.mdl */
    DB_CutsceneLevelShot14, /* 228: models\cutscene\level_shot14.mdl */
    DB_CutsceneLevelIntroCrashplain, /* 229: models\cutscene\level_intro_crashplain.mdl */
    DB_CutsceneLevelIntroCortexlab, /* 230: models\cutscene\level_intro_cortexlab.mdl */
    DB_CutsceneLevelIntroShotGroupTeamgood2, /* 231: models\cutscene\level_intro_shot_group_teamgood2.mdl */
    DB_CutsceneLevelIntroShotWelcome3, /* 232: models\cutscene\level_intro_shot_welcome3.mdl */
    DB_CutsceneLevelIntroShotWelcome2, /* 233: models\cutscene\level_intro_shot_welcome2.mdl */
    DB_CutsceneLevelIntroShotWelcome, /* 234: models\cutscene\level_intro_shot_welcome.mdl */
    DB_CutsceneLevelIntroShotGroupTeamevil4, /* 235: models\cutscene\level_intro_shot_group_teamevil4.mdl */
    DB_CutsceneLevelIntroShotGroupTeamevil3, /* 236: models\cutscene\level_intro_shot_group_teamevil3.mdl */
    DB_CutsceneLevelShot13, /* 237: models\cutscene\level_shot13.mdl */
    DB_CutsceneLevelIntroShotGroupTeamevil2, /* 238: models\cutscene\level_intro_shot_group_teamevil2.mdl */
    DB_CutsceneLevelShot12, /* 239: models\cutscene\level_shot12.mdl */
    DB_CutsceneLevelIntroShotGroupTeamevil, /* 240: models\cutscene\level_intro_shot_group_teamevil.mdl */
    DB_CutsceneLevelIntroShotGroupTeamgood, /* 241: models\cutscene\level_intro_shot_group_teamgood.mdl */
    DB_CutsceneLevel4, /* 242: models\cutscene\level4.mdl */
    DB_CutsceneLevel3, /* 243: models\cutscene\level3.mdl */
    DB_CutsceneLevel2, /* 244: models\cutscene\level2.mdl */
    DB_CutsceneLevel, /* 245: models\cutscene\level.mdl */
    DB_CutsceneLevelEndingGoodShot3, /* 246: models\cutscene\level_ending_good_shot3.mdl */
    DB_CutsceneLevelShot8, /* 247: models\cutscene\level_shot8.mdl */
    DB_CutsceneLevelShot7, /* 248: models\cutscene\level_shot7.mdl */
    DB_CutsceneLevelShot6, /* 249: models\cutscene\level_shot6.mdl */
    DB_CutsceneLevelEndingGoodShot2, /* 250: models\cutscene\level_ending_good_shot2.mdl */
    DB_CutsceneLevelShot5, /* 251: models\cutscene\level_shot5.mdl */
    DB_CutsceneLevelShot4, /* 252: models\cutscene\level_shot4.mdl */
    DB_CutsceneLevelShot3, /* 253: models\cutscene\level_shot3.mdl */
    DB_CutsceneLevelShot2, /* 254: models\cutscene\level_shot2.mdl */
    DB_CutsceneLevelEndingEvilShot1, /* 255: models\cutscene\level_ending_evil_shot1.mdl */
    DB_CutsceneLevelEndingGoodShot1, /* 256: models\cutscene\level_ending_good_shot1.mdl */
    DB_CutsceneLevelIntro, /* 257: models\cutscene\level_intro.mdl */
    DB_CutsceneLevelEndingEvilShot8, /* 258: models\cutscene\level_ending_evil_shot8.mdl */
    DB_CutsceneLevelShot11, /* 259: models\cutscene\level_shot11.mdl */
    DB_CutsceneLevelShot10, /* 260: models\cutscene\level_shot10.mdl */
    DB_CutsceneLevelEndingEvilShot7, /* 261: models\cutscene\level_ending_evil_shot7.mdl */
    DB_CutsceneLevelEndingEvilShot6, /* 262: models\cutscene\level_ending_evil_shot6.mdl */
    DB_CutsceneLevelShot9, /* 263: models\cutscene\level_shot9.mdl */
    DB_CutsceneLevelEndingEvilShot5, /* 264: models\cutscene\level_ending_evil_shot5.mdl */
    DB_CutsceneLevelEndingEvilShot4, /* 265: models\cutscene\level_ending_evil_shot4.mdl */
    DB_CutsceneLevelEndingEvilShot3, /* 266: models\cutscene\level_ending_evil_shot3.mdl */
    DB_CutsceneLevelEndingEvilShot2, /* 267: models\cutscene\level_ending_evil_shot2.mdl */
    DB_CutsceneTextGoodwins, /* 268: models\cutscene\text_goodwins.mdl */
    DB_CutsceneTextEvilwins, /* 269: models\cutscene\text_evilwins.mdl */
    DB_CutsceneTextMixteamwins, /* 270: models\cutscene\text_mixteamwins.mdl */
    DB_CutsceneTextCredits, /* 271: models\cutscene\text_credits.mdl */
    DB_CutsceneTextIntromovie, /* 272: models\cutscene\text_intromovie.mdl */
    DB_CutsceneUkaData4, /* 273: models\cutscene\uka\data4.mdl */
    DB_CutsceneUkaData3, /* 274: models\cutscene\uka\data3.mdl */
    DB_CutsceneUkaData2, /* 275: models\cutscene\uka\data2.mdl */
    DB_CutsceneUkaData, /* 276: models\cutscene\uka\data.mdl */
    DB_CutsceneAkuData4, /* 277: models\cutscene\aku\data4.mdl */
    DB_CutsceneAkuData3, /* 278: models\cutscene\aku\data3.mdl */
    DB_CutsceneAkuData2, /* 279: models\cutscene\aku\data2.mdl */
    DB_CutsceneAkuData, /* 280: models\cutscene\aku\data.mdl */
    DB_CutsceneAkuUkaData2, /* 281: models\cutscene\aku_uka\data2.mdl */
    DB_CutsceneAkuUkaData, /* 282: models\cutscene\aku_uka\data.mdl */
    DB_COUNT
};

extern struct obj_dbase HeroModelBank[DB_COUNT];

#endif
