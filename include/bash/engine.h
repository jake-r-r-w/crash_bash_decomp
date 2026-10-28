#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Functions and data defined in the engine overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * The engine is several files; this one owns engine.c's and includes the others' headers.
 * owner: src/overlays/engine/engine.c */

#include "common.h"
#include "bash/main.h"
#include "bash/nas.h"
#include "bash/shell.h"   /* ClockInfo, ScoreInfo */


extern u16 LangPanelModels[];
extern s32 CamBoundTop;

extern s32 CamBoundRight;
typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} CamFrameEntry;  /* CameraFrameList: a thing the camera keeps in frame */
extern s32 camera_frame_list_size;
extern CamFrameEntry CameraFrameList[];
extern s32 HealthAst;
extern s32 CratePickupAst;

void RequirementMet(s32 arg0);
void RequirementMetAny(s32 arg0);
void ModeRequirementMet(s32 arg0);
void RunLevelTriggers(void);
void RenderLevel(void);
void InitAmbience(Ambient_data *arg0);
void RenderNas(void);
void LevelUnused(void);
void UpdateLevelTriggers(void);
void ResetASTTable(void);
void ClearLevelNas(void);
extern s32 SaveScreenPanel;
extern s32 CardSlotsPanel;
extern s32 CardSlotIndex;
extern s32 CardBufferStamp;
extern s32 SaveMenuStage;
extern s32 SaveMenuPanel;
extern s32 CardPromptPanel;
extern subengine EngineSlotSubs[][32]; /* 32 subengines per EnginePool slot */

typedef struct { u16 a, b; } U16Pair;
extern ScoreInfo Score[4];
extern SWORD_XY ScoreIcon3Pos[4];
extern SWORD_XY ScoreIcon2Pos[4];

extern s32 ScreenLabelState;
extern s32 PanelCtrl;
typedef struct EngObj {
    s32 flags;
    u8 pad[0x48];
    void (*update)(struct EngObj *);
} EngObj;
extern EngObj **NasUpdateList;
typedef struct IconSet { s32 *c; u8 pad[4]; s32 *a; s32 *b; } IconSet;
extern IconSet Icon;
extern s16 IconPos[];
extern s32 GameTeams[];
extern u32 Clock_Time;
extern u16 Clock_flags;
extern U16Pair Clock_offsets[];
extern ClockInfo Clock[4];
extern s32 Clock_XYZ[3];
extern u16 Clock_last_ping;
extern s16 CompletionShown;
extern s32 CompletionPos[];
extern s32 HeroesOutTimer;
void draw_nas_model_flags(NAS *nas, u8 *arg1, u32 flags);
typedef struct EngineBox {
    s32 unk0;
    u8 pad4[0x50];
    void *unk54;
    u8 pad58[0x10];
    s16 unk68, pad6A, unk6C, unk6E, unk70, pad72, unk74, unk76, unk78, pad7A, unk7C, unk7E, unk80, pad82, unk84, unk86, unk88, pad8A;
    s32 unk8C, unk90, unk94, unk98;
} EngineBox;
extern u16 CupMode;
extern s32 CupTimer;
extern u16 Trophy_Cups;
extern process_control_info ProcessControl;
extern u8 ModeDbaseLists[];
extern process_control_info LoadingControl;
extern EXTengine EnginePool[];
extern HeroInfo Heroes[4];
/* A hero action (SetHeroAction): what starts it and what runs it each frame. */
typedef struct HeroActionEntry {
    void (*fn)(void);
    void (*update)(s32);
} HeroActionEntry;
extern HeroActionEntry HeroActionTable[];
extern hero_seq **HeroSeqTables[]; /* by HeroInfo.hero_name (1-8): each character's sequences, by id */
extern s32 ElapsedStart;
extern s32 LoadingScenePlaying;
extern s32 LoadingSceneDelay;
extern EXTengine LoadingIconEngine;
extern struct MarkCtx *SceneMark; /* the scene's current mark record (levelload.h) */
extern process_control_info MenuProcessControl;
extern void (*RenderHook)(void);
extern GfxViewport LoadingViewport;   /* the loading screen's (RenderLoadingScreen) */
extern camera_info_tag LoadingCamera;
extern s32 LevelLoadStage;
extern Blk32 LoadingScriptRot;
extern CamVec3 LoadingScriptPos;
extern ASTEntry EngineASTTable[];
extern fade_info ScreenFade;
extern s32 ScreenLabelShown;
extern process_info LevelLoadProcess;
extern s32 GamePause;
extern u8 PauseMenuPanel[];
extern ScriptInit BigPoofInit;
extern ScriptInit SmokePoofInit;
extern GfxViewport HudViewport;
extern GfxViewport SceneViewport;
extern camera_info_tag HudCamera;
extern process_info *SceneSkipProcess;
extern GfxViewport SaveViewport;
extern GfxViewport SaveBackViewport;
extern camera_info_tag SaveCamera;
extern camera_info_tag SaveBackCamera;
extern s32 SaveMenuStep;
extern s32 SaveMenuYesNo;
extern s32 CardPromptStep;
extern s32 SaveMenuState;

/* InitLevelLoad's level setup config (LevelConfig, or DefaultLevelConfig if unset). */
typedef struct EngineCfg {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 pools[3];       /* prims_init */
    /* 0x18 */ VramArea *areas;    /* vram_init */
    /* 0x1C */ FreeNode *pool;
    /* 0x20 */ s32 npool;
    /* 0x24 */ s32 nbuf;
} EngineCfg;

extern EngineCfg DefaultLevelConfig;
extern EngineCfg SceneLevelConfig;
extern s32 GraphicsMode;
extern s32 IdleFrames;
extern s16 core_process;
extern u8 *SlotBase;
extern s32 ModeTextLoaded;
extern u8 SlotHeapStart[];
extern ArenaDef DefaultArenaDef;
extern process_info RunGame;
extern ScreenMode vmode_512_ntsc;
extern ScreenMode vmode_512_pal;
extern s32 SceneStep;
extern GfxViewport MainViewport;
extern camera_info_tag MainGfxCamera;
extern process_info SaveScreenProcess;
extern struct Ambient_data *AmbienceDef; /* the level's (InitAmbience) */
extern u16 AmbienceTimer;
extern process_info ScriptSceneProcess;
extern struct ScriptScene *SceneArgs;
extern s32 SceneOldDbase;
extern s32 SceneMarkCount;
/* DrawSceneMarks's entries (SceneMarks, 0xC bytes) and the table they index (0x14 bytes). */
typedef struct EngineMarker {
    /* 0x0 */ u16 unk0;
    /* 0x4 */ s32 idx;
    /* 0x8 */ void *unk8;
} EngineMarker;
typedef struct EngineMarkerPos {
    /* 0x00 */ s32 x, y, z;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 unkE[6];
} EngineMarkerPos;
extern EngineMarker SceneMarks[];
extern EngineMarkerPos SceneMarkPos[];
extern subengine SceneMarkSub;
extern s32 SceneVabSlot[]; /* arrays: their stores must alias struct loads (gcc 2.7 sched) */
extern s32 SceneSoundHandle[];
extern s32 SceneStarted;
extern s32 SceneStarting;
extern u8 IntroViewport[];
/* GameInfo as seen by InitRetry. */
typedef struct EngineGameView {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[8];
    /* 0x10 */ s32 pause;  /* GamePause */
    /* 0x14 */ u8 unk14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0xE0 - 0x36];
    /* 0xE0 */ s32 unkE0;
} EngineGameView;
extern GfxViewport GameViewport;
extern s32 GameSceneFade;
typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; } EngineEntry16Head;
typedef struct { s32 unk0; EngineEntry16Head *unk4; s32 unk8; } EngineTable12;
extern EngineTable12 CupTable[];
/* Also in main.h; repeated for overlays that include engine.h first. */
extern u8 GameInfo[];
extern s32 LevelUnlockedIn(s32, u8 *);
extern u16 ModeTypes[];
void process_init(process_control_info *pc);
void InitialiseLevel(void);
s32 CountTrophies(u8 *arg0);
void HubStartCredits(void);

extern s32 IntroDbase;
extern char *HeroAnimNameA;
extern obj_dbase *HeroAnimDbaseA;
extern char *HeroAnimNameB;
extern obj_dbase *HeroAnimDbaseB;
extern u8 LoadingSceneViewport[];
extern s32 IntroStepIndex;
extern s32 IntroStepTimer;
extern s32 IntroNextSfx;
extern s32 IntroSfxDelay;
extern s32 IntroScriptHandle;
extern u8 CupMenuPanel[];
/* A text-panel record PlaceEngineSlot builds from (0x24 bytes): kind at 0, the text at 0x14. */
typedef struct EngRec24 {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ void *text;
    /* 0x18 */ u8 pad18[6];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[4];
} EngRec24;

extern EngRec24 CupMenuLabels[];
extern EngRec24 CupSlotPanel[2];
extern process_info TournamentSelect;   /* after CupSlotPanel's two records: the tournament select menu (TournamentChain) */
extern s32 CupMenuCompact;
typedef struct { u16 a, b, c, flags; } EngEntry;
typedef struct { s32 unk0; EngEntry *entries; s32 unk8, unkC, unk10; } EngTable;
/* The hub hero's place on the warp room's ring (HubPreMoveHero): its angle and its distance from
 * the centre, kept between path 2's limits (HubInitHero). */
typedef struct { u16 unk0; s32 angle; s32 radius; s32 radiusMin; s32 radiusMax; } HubRing;
extern HubRing HubHeroRing;
extern s32 NextGameMode;
extern u8 GameWarpRoom;
extern s32 WarpRooms[][5];
extern u16 ArenaRequirements[];
extern u16 CpuHeroChoices[];
typedef struct { u16 unk0; u16 unk2; u16 unk4; u16 unk6; } EngSub90248;
typedef struct { s32 *unk0; s32 *unk4; s32 unk8; s32 unkC; s32 unk10; EngSub90248 *unk14; u8 *unk18; s32 unk1C; s32 unk20; s32 unk24; } EngRec90248;
extern s8 GameGateway;
extern EngRec90248 HubGateways[];

void ArenaRequirementMetAny(s32 arg0);

typedef struct {
    u8 unk0;
    u8 unk1;
} D_8009D90C_t;
extern D_8009D90C_t ArenaGrid[400];

int ASTTableCount(void);

/* One of the camera frame's extreme points (CalcCameraFrame): the bottom and top ones set y
 * and z, the left and right ones x and z. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
} CamFramePoint;

extern CamFramePoint camera_frame_bottom;
extern s32 CamBoundBottom;
extern CamFramePoint camera_frame_left;
extern CamFramePoint camera_frame_top;
extern CamFramePoint camera_frame_right;
extern s32 CamBoundLeft;

/* PadState (Pads) seen by PauseChooseYesNo: button flags at 0x44/0x48. */
typedef struct PadButtons {
    /* 0x00 */ u8 unk0[0x44];
    /* 0x44 */ s32 held;
    /* 0x48 */ s32 pressed;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} PadButtons;

extern s32 PauseChoice;
extern s32 PausePad;
void ArenaRequirementMet(s32 arg0);


/* An entry of the list SceneVabCue walks (UpdateSceneVabCues), 12 bytes. */
typedef struct {
    s32 vab;   /* machine_load_sfx's first argument; 0 ends the list */
    s32 sfx;   /* PlaySfx id */
    s32 unk8;  /* -> VabCueTimer */
} EngineVabCue;
extern EngineVabCue *SceneVabCue;

/* A script scene (GotoScriptScene, SetupScriptScene): its marks (-> SceneMark), its sound cue
 * list (-> SceneVabCue) and its frame rate (16.16 frames a tick). The ending's are lists of
 * them; ModeScenes picks one per mode and team side. */
typedef struct ScriptScene {
    struct MarkCtx *marks;
    EngineVabCue *cues;
    s32 rate;
} ScriptScene;
/* A mode's scenes by GameInfo +0x44 (1, 2, 3: ModeScenes.scene[0..2]). */
typedef struct {
    s32 mode;
    ScriptScene *scene[3];
} ModeScenes;
extern ModeScenes ModeVariants[];

typedef struct Vec3W {
    s32 a, b, c;
} Vec3W;

typedef struct { s32 unk0; EngineRec32 *unk4; } EngineBootSrc;
typedef struct {
    /* 0x00 */ void (*unk0)();
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 unkC[0x20];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 unk30[0xC];
    /* 0x3C */ u16 unk3C;
} EngineBootView;
extern NAS *NasPoolLast;
extern u8 *HeroNasList;
extern s32 PauseMenuSets;
extern s32 PauseMenuSetsByLang[];
extern s32 LevelRecIndex;
extern u16 CameraMode;

typedef struct EngineRec24 {
    s32 kind;
    s16 val;
    char pad[0x1E];
} EngineRec24;
extern EngineRec24 *PauseRec;
extern EngineRec24 **PauseRecSet;
extern s32 PauseRecIndex;

/* Hero jump parameters, 8 bytes per hero (EngineHooks.unk10). */
typedef struct HeroJumpEnt {
    u16 vel, max, acc, dec;
} HeroJumpEnt;

/* Engine callback table filled by SetDefaultHeroHooks (0x54 is HeroHooks.HeroKickCollision). */
typedef struct EngineHooks {
    /* 0x00 */ s32 InitHero;
    /* 0x04 */ u16 *HeroModels;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u16 *RunSpeeds;
    /* 0x10 */ void *JumpEnts;
    /* 0x14 */ void *VictorySeqs;     /* the victory sequences (D_8009D6FC_t, pause.h) */
    /* 0x18 */ void *CreateShadow;
    /* 0x1C */ void (*UpdateShadow)(void);
    /* 0x20 */ void *PreMoveHero;
    /* 0x24 */ void *MoveHero;
    /* 0x28 */ void *PostMoveHero;
    /* 0x2C */ void (*HeroFacing)(void);
    /* 0x30 */ void (*HeroSetIdle)();
    /* 0x34 */ void (*HeroTouched)(s32);
    /* 0x38 */ void *HeroBot;
    /* 0x3C */ s32 (*TestAttack)(s32);
    /* 0x40 */ s32 (*TestRun)(s32);
    /* 0x44 */ s32 (*TestJump)(s32);
    /* 0x48 */ s32 (*TestKick)(s32);
    /* 0x4C */ void *Hero2ObjectCollision;
    /* 0x50 */ s32 (*HeroGrabObject)(s32, s32);
    /* 0x54 */ void (*HeroKickCollision)();
    /* 0x58 */ void (*HeroCarryCollision)(s32);
    /* 0x5C */ void *HeroExtraEffects;
    /* 0x60 */ void *HeroesTouched;
    /* 0x64 */ void (*ChooseWinner)(s32, s32);
    /* 0x68 */ s32 (*KeepInBounds)(EXTengine *);
} EngineHooks;
extern EngineHooks HeroHooks;
extern u8 DefaultHeroModels[];
extern s32 DefaultVictorySeqs[];   /* {name, id} pairs: the default victory sequences (HeroHooks.VictorySeqs) */
extern u8 DefaultRunSpeeds[];
extern u8 DefaultJumpEnts[];
extern u8 DefaultAiBotTweaks[];
extern u8 DefaultAiHumanTweaks[];
extern u8 DefaultAiSkills[];
extern u8 DefaultAiTeamSkills[];
extern u8 DefaultAiHumanRoundTweaks[];
extern void *AiBotTweakTable;
extern void *AiHumanTweakTable;
extern void *AiSkillTable;
extern void *AiTeamSkillTable;
extern void *AiHumanRoundTweakTable;
extern s32 HeroesAllOut;
extern s16 HeroActHit;
extern s16 HeroActPickup;
extern s16 HeroActHold;
extern s16 HeroActCarry;
extern s16 HeroActThrow;
extern s16 HeroActDead;
extern s16 HeroActWinner;
extern s16 HeroActLoser;
void QuitLevel(void);


extern SWORD_XY ScoreIcon1Pos[4];
extern s16 LevelLoadModel;
extern s32 map[];

void PreInitLevel(void);

/* N2O's star field (nox.c's NoxSparkFx; the engine's link takes it from the overlay's ELF) */
extern FxDef NoxSparkFxDef;

extern FxDef WeatherFxDef; /* the weather's (InitWeatherNas) */

extern HitSfxTable HitSfx;
extern s32 CrateAstType;
extern s32 CrateAstType2;
extern u16 CrateCounts[];
NAS *SpawnModelFx(s32 arg0, s32 *arg1, s32 arg2);

void RenderAllNas(void);


NAS *SpawnModelFxOnce(s32 arg0, s32 *arg1, s32 arg2);

void DoAmbience(void);

/* The engine's other files (ARCHITECTURE.md, Files inside the engine). */
#include "bash/panel.h"
#include "bash/pause.h"
#include "bash/heroes.h"
#include "bash/gameflow.h"
#include "bash/levelload.h"
#include "bash/camera.h"

/* Needs prototypes from the unit headers above. */
/* InitRetry's tail: resets state, then starts the mode by its ModeTypes world. */
static inline void EngineBeginMode(s32 arg) {
    EngineGameView *g = (EngineGameView *)GameInfo;
    s32 mode;
    u16 world;

    GameViewport.r = 0;
    GameViewport.g = 0;
    GameViewport.b = 0;
    GameViewport.isbg = 0;
    g->unk30 &= 0xFDFF;
    UpdateCamera(arg);
    process_init(&MenuProcessControl);
    ResetScreenLabels();
    mode = g->mode;
    world = ModeTypes[mode * 3];
    if (world != 0x101) {
        if (world == 0x102) {
            if (mode == 0x25) {
                HubStartCredits();
            }
        } else {
            g->unk30 |= 0x1000;
            StartWinSequence(mode);
        }
    }
}

void InitialiseEngine(void);

void UpdateNasObjects(void);

/* LevelRequirements rows (10 bytes) and the object LevelRequirementMet totals over. */
typedef struct { u16 min0, min2, min4, min6, off; } EngineReq;
extern EngineReq LevelRequirements[];
typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0xE4 - 8];
    u8 a[0x21];
    u8 pad105[0x105 - 0xE4 - 0x21];
    u8 b[0x21];
    u8 pad126[0x126 - 0x105 - 0x21];
    u8 c[0x21];
    u8 pad147[0x147 - 0x126 - 0x21];
    u8 d[0x21];
    u8 pad168[0x168 - 0x147 - 0x21];
    u8 e[1];
} EngineReqObj;

void InitialiseNasSystem(void);

void UpdateAllNas(void);

void PickCpuHeroes(void);

s32 GravitateNas(NAS *nas);

/* GameInfo as seen by CheckControllers. */
typedef struct EngMenuGame {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x26 - 0x14];
    /* 0x26 */ u8 count;
    /* 0x27 */ u8 pad27[0x34 - 0x27];
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x68 - 0x36];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 pad6C[0x74 - 0x6C];
    /* 0x74 */ u8 unk74;
} EngMenuGame;
extern HudEntry PauseUnpluggedMenu[];
extern HudEntry PausePadMessage[];

void CheckControllers(void);

/* GameInfo as seen by DrawLevelHud. */
typedef struct EngHudGame {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x32 - 0x14];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
} EngHudGame;
#define HUDGAME ((EngHudGame *)GameInfo)
#define HUDPLAYERS ((EXTengine **)&Icon)
extern s32 PauseFlashItem;
void DrawLevelHud(void);


#endif /* BASH_ENGINE_H */
