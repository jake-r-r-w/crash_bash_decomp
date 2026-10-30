#ifndef BASH_POGO_OVL_H
#define BASH_POGO_OVL_H

/* Functions and data defined in the pogo overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/pogo/pogo.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/pogo.h" /* PogoPlayer, PogoPlay */

extern u16 PogoHeroModels[];
extern u8 PogoHookTableC[];

extern process_info PogoProcess;

void psxpogo_render_tile(ModelObj *obj, void *arg1, s32 flags);


/* What a board cell's unk4 points to (Plot_PogoBanker reads its x and z). */
typedef struct {
    /* 0x00 */ s32 pad0;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 pad8;
    /* 0x0C */ s32 z;
} PogoCellPos;

extern PogoSquare PogoGrid[8][8];
/* The board cell at byte offset off from base (PogoGrid). */
#define CELL_AT(off) ((PogoSquare *)((u8 *)base + (off)))
/* The position a board cell's unk4 points to, as PlaySfx takes it. */
#define CELL_POS(cx, cz) (&((PogoSquare *)((u8 *)PogoGrid + (cx) * sizeof(PogoGrid[0]) + (cz) * sizeof(PogoSquare)))->sub->XYZ)

/* A scene object in the list at MapScene->subs (InitPogo_Tiles). */
typedef struct PogoObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x5C - 0x4];
    /* 0x5C */ struct PogoObj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} PogoObj;
extern PogoObj *PogoKerbObj;

extern s32 PogoBounceCount[];
extern s16 PogoBounceHeights[];
extern s32 PogoHeroHeights[];
void Roo_FireProjectile(EXTengine *eng, s32 angle);
/* Plot_PogoMushroom's 16-step y scale bounce. */
extern s32 MushroomScales[16];
/* Plot_PogoMushroom's puff of smoke at pos (type 0x1605, flags 0x11). */
static inline void Pogo_SpawnPuff(s32 *pos, s32 mode, u8 *p) {
    NAS *obj;
    s32 *ext;

    obj = CreateNAS(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (obj != NULL) {
        ext = FindExtension(obj, 0x10000000);
        if (ext != NULL) {
            ext[3] = mode;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                obj->Engine->sub->u6C.ptr = p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            obj->InitDelete(obj);
        }
    }
}
/* Drops hero i's pickup and sets its slot state (Pickup_Claim). */
#define Pogo_PickupSet(i, state) \
    {                             \
        PogoPlayer *s_;            \
                                  \
        Pickup_Lose(i);           \
        s_ = PogoPlay;          \
        s_[i].status = (state);     \
    }
/* Plays anim on a hero's powerup sub-engine (Pickup_Claim). */
static inline void Pogo_PowerupAnim(s32 i, s32 anim) {
    subengine *sub = Heroes[i].hero_powerup->sub;

    if (sub != NULL) {
        sub->u6C.dbase = &HeroModelBank[DB_PogoObjects];
        sub->entity = anim;
    }
}
/* ZapArrow, inlined into Pickup_Claim. */
static inline void Pogo_ZapArrow(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas = (NAS *)PogoGrid[arg0][arg1].pickup;
    s32 dir;
    s32 i;
    s32 dx;
    s32 dz;

    if (nas != NULL) {
        if (nas->Engine != NULL) {
            PlaySfx(0x515, NULL, &nas->Engine->XYZ, 0x1000, 0x1E00, 0);
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
extern ScriptInit RooScriptInit;
extern s32 RooFrame;
extern NAS *PogoPadlockNas;
extern s32 BrioSmokeFrame;
/* The padlock's moves at each level before it levels up (Plot_PogoPadlock;
 * entry 0 is also its first UserInfo.flags, level 0 in the high byte). */
extern s32 PadlockFlags[];
/* PadlockFlags's entry for a level, inlined into Plot_PogoPadlock. */
static inline s32 Pogo_PadlockMoves(s32 level) {
    return PadlockFlags[level];
}
/* FindFreeSquare (the same C), inlined into FindFreeSquareForMushroom and
 * Plot_PogoPadlock: up to 10 random tries for a free square. */
static inline bool Pogo_FindFreeSquare(s32 *px, s32 *pz) {
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
extern s32 CortexSmokeFrame;
extern s32 PickupsWanted[];
extern s32 PickupsMax[];
extern s32 PogoObjectTimes[];
extern s32 PowerupsWanted[];
extern s32 PowerupsMax[];
extern s32 PogoUnused0;
extern s32 PogoUnused1;
extern s32 RooBounce;
extern NAS *PogoRooNas;
extern s32 PogoLevelIndex;
extern u16 TileLockTimes[][2];
extern s32 PogoAiQualityIndex;
extern u16 PogoAiTimers[][2];
extern s32 PogoTilesMid[];
extern s32 PogoTilesHigh[];
extern s32 PogoTilesLow[];
/* HeroPogo's per-hero frame tables */
extern s32 PogoFramesCrash[];
extern s32 PogoFramesCoco[];
extern s32 PogoFramesBrio[];
extern s32 PogoFramesCortex[];
extern s32 PogoFramesDingo[];
extern s32 PogoFramesKangaroo[];
extern s32 PogoFramesTiny[];
extern s32 PogoFramesKong[];
extern s32 KerbMode;
extern s32 KerbStep;
extern s32 KerbDelay;
extern s32 KerbTimer;
extern s32 KerbPeriod;
extern s32 KerbCount;
extern s32 KerbClockSecond;
extern s32 TileScan0[8];
extern s32 TileScan1[8];
extern s32 TileScan2[8];
extern s32 TileScan3[8];
extern u8 KerbData[][4][8];
extern s32 KerbIndexA[];
extern s32 KerbIndexB[];
extern s16 KerbTiles[][2];
extern s32 KerbPathA[][4];
extern s32 KerbPathB[][4];


typedef struct {
    s32 x;
    s32 z;
} PogoOffset;

extern PogoAIPlayer PogoAIPlay[];
extern PogoOffset EncloseStepsOdd[];
extern PogoOffset EncloseStepsEven[];







































extern s32 MushroomCount;
extern s32 RooTileX;
extern s32 RooTileZ;
extern s32 RooSpeedX;
extern s32 RooSpeedZ;
extern s32 RooAngle;
extern s32 RooDir;

/* GameInfo+0x34 as UpdatePOGO switches on it: an enum, so gcc's
 * switch tree skips its text cost table. */
typedef enum PogoMode {
    POGO_MODE_NONE = -1,
    POGO_MODE_10 = 10,
    POGO_MODE_11 = 11,
    POGO_MODE_12 = 12
} PogoMode;




































#define SCORE60V ((GameState *)GameInfo)

#define RANK60V ((GameState *)GameInfo)

/* PogoQuality_Init: per-mode entries picked by teams, with two values at 0xE/0x10. */
typedef struct PogoModeEntry {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u16 quality;  /* the AI quality */
    /* 0x10 */ u16 qualityAlt;  /* with levelFlags 2 */
    /* 0x12 */ u8 pad12[2];
} PogoModeEntry;
extern PogoModeEntry PogoPainterAiHuman;
extern PogoModeEntry PogoPainterAiHumanAlt;
extern PogoModeEntry PogoAgogoAiHuman;
extern PogoModeEntry PogoAgogoAiHumanAlt;
extern PogoModeEntry PogoLocoAiHuman;
extern PogoModeEntry PogoLocoAiHumanAlt;
extern PogoModeEntry PogoBouncerAiHuman;
extern PogoModeEntry PogoBouncerAiHumanAlt;
extern u16 PogoPainterAiQuality[];
extern u16 PogoAgogoAiQuality[];
extern u16 PogoLocoAiQuality[];
extern u16 PogoBouncerAiQuality[];
/* PogoWinner_Relic: hero idx's team is out of points (i starts at i0). */
#define POGO_TEAM_LOST(g, idx, i, i0)                                   \
    {                                                                    \
        GameState *g_;                                               \
                                                                         \
        i = (i0);                                                        \
        g_ = (g);                                                        \
        g_->winner[0] = Heroes[idx].hero_name;                              \
        g_->exitFlags |= 0x800;                                              \
        Clock_flags |= 0x200;                                             \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                RemoveCamera(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x200;                          \
            }                                                            \
        }                                                                \
    }
/* PogoWinner_Relic: POGO_TEAM_LOST, g copied for the loop after the stores. */
#define POGO_TEAM_LOST_LATE(g, idx, i, i0)                              \
    {                                                                    \
        GameState *g_;                                               \
                                                                         \
        i = (i0);                                                        \
        (g)->winner[0] = Heroes[idx].hero_name;                             \
        (g)->exitFlags |= 0x800;                                             \
        Clock_flags |= 0x200;                                             \
        g_ = (g);                                                        \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                RemoveCamera(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x200;                          \
            }                                                            \
        }                                                                \
    }
/* PogoWinner_Relic: the round ends with no winner. */
#define POGO_ROUND_OVER(g, flag, i)                                     \
    do {                                                                 \
        GameState *g_;                                               \
                                                                         \
        i = 0;                                                           \
        g_ = (g);                                                        \
        g_->winner = 0;                                                  \
        g_->unk30 |= (flag);                                             \
        Clock_flags |= 0x200;                                             \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                RemoveCamera(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x100;                          \
            }                                                            \
        }                                                                \
    } while (0)
/* PogoWinner_Relic: a hero's score plus its teammate's. */
static inline s32 PogoTeamScore(s32 idx) {
    HeroInfo *hb = Heroes;
    HeroInfo *h = &hb[idx];

    return h->hero_score + hb[h->hero_teammate].hero_score;
}
/* PogoInit_AIQuality: the engine's per-mode table pointers. */
extern PogoModeEntry PogoAiBotTweak;
extern PogoModeEntry PogoPainterAiSkill;
extern PogoModeEntry PogoPainterAiSkillTeams;
extern u16 PogoPainterAiTeamSkill[6];
extern u16 PogoPainterAiRound[12];
extern u16 PogoPainterAiRoundAlt[12];
extern PogoModeEntry PogoAgogoAiSkill;
extern PogoModeEntry PogoAgogoAiSkillTeams;
extern u16 PogoAgogoAiTeamSkill[6];
extern u16 PogoAgogoAiRound[12];
extern u16 PogoAgogoAiRoundAlt[12];
extern PogoModeEntry PogoLocoAiSkill;
extern PogoModeEntry PogoLocoAiSkillTeams;
extern u16 PogoLocoAiTeamSkill[6];
extern u16 PogoLocoAiRound[12];
extern u16 PogoLocoAiRoundAlt[12];
extern PogoModeEntry PogoBouncerAiSkill;
extern PogoModeEntry PogoBouncerAiSkillTeams;
extern u16 PogoBouncerAiTeamSkill[6];
extern u16 PogoBouncerAiRound[12];
extern u16 PogoBouncerAiRoundAlt[12];

























/* HeroPogoDazed: a status bit sends the hero to an action and seq. */
static inline s32 Pogo_StatusAction(s32 arg0, s32 bit, s32 action, s32 seq)
{
    if (Heroes[arg0].hero_status & bit) {
        if (PogoPlay[arg0].tazer != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction(arg0, action);
        SetHeroSeq(arg0, seq);
        return 1;
    }
    return 0;
}




extern s32 TileStepFrames[];
extern s32 TileStepFramesAlt[];

/* A 0x38-byte frame of a cell object's model; its +0x26 is the frame CELL_SET_T shows. */
typedef struct {
    /* 0x00 */ u8 pad0[0x26];
    /* 0x26 */ u16 frame;
    /* 0x28 */ u8 pad28[0x38 - 0x28];
} PogoFrame;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ PogoFrame *frames;
} PogoModel;

/* The object a board cell's unk4 points to, as Tile_Update sets it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ PogoModel *model;
    /* 0x70 */ u8 pad70[0x8C - 0x70];
    /* 0x8C */ u16 frame;
    /* 0x8E */ s16 anim;
} PogoCellObj;
extern u16 TileFrames[][4];
extern u16 TileFramesAlt[][4];
#define MV ((GameState *)GameInfo)
/* Set a cell object's animation and its frame (fr: an s32 temp of the caller). */
#define CELL_SET(o, a, f) \
    ((o)->anim = (a), fr = (f), (o)->frame = CELL_FRAME(o, fr & 0xFFF)->frame)
#define CELL_SET_T(o, a, f) \
    ((o)->anim = (a), (o)->frame = CELL_FRAME(o, (f) & 0xFFF)->frame)
/* &(o)->model->frames[i], as (i * 7) << 3 bytes (sizeof(PogoFrame) is 0x38) */
#define CELL_FRAME(o, i) ((PogoFrame *)((u8 *)(o)->model->frames + (((i) * 7) << 3)))

extern s32 PadlockTimes[];

/* Plot_PogoRipperRoo: the move-pattern tables (normal, alt), the tile counter and its target */
extern s32 RooHopFrames[];
extern s32 RooFrames[];
extern s32 RooSteps;
extern s32 RooWait;

#endif /* BASH_POGO_OVL_H */
