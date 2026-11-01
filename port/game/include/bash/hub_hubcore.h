#ifndef BASH_HUB_HUBCORE_H
#define BASH_HUB_HUBCORE_H

/* Functions and data defined in src/overlays/hub/hubcore.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/hub/hubcore.c */

#include "common.h"
#include "bash/hub_ovl.h"

extern s32 PodiumTies;
extern u8 *PodiumAnimName;
extern u8 PodiumSpawns[];

typedef struct HubPt {
    /* 0x0 */ u16 x;
    /* 0x2 */ u16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 w;
} HubPt;
typedef struct HubTrail {
    /* 0x000 */ HubPt pos[8];
    /* 0x040 */ s32 flag[8];
    /* 0x060 */ HubPt prev[8];
    /* 0x0A0 */ u8 padA0[0xE0 - 0xA0];
    /* 0x0E0 */ HubPt ang[8];
    /* 0x120 */ s32 yvel;  /* damped by 0xE6/0x100 */
    /* 0x124 */ s32 pad124;
    /* 0x128 */ s32 gravity;  /* added to yvel */
    /* 0x12C */ s32 xvel;  /* damped by 0xE6/0x100 */
    /* 0x130 */ s32 pad130;
    /* 0x134 */ s32 pad134;
    /* 0x138 */ s32 flag2;  /* tested for zero */
} HubTrail;
typedef struct HubTrailDst {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[offsetof(subengine, scale) - (offsetof(subengine, XYZ.z) + 4)];
    /* 0x76 */ u16 angle;  /* turned 0x200 a frame */
} HubTrailDst;

extern s32 HubDoorFade;
extern s32 HubHeroPressed;
extern EXTengine HubNameEngine;
extern s32 HubNameNext;
extern s32 HubNamePage;
extern s32 HubNameLen;
typedef struct HubEntry12 {
    struct HubKey *keys;  /* the page's keys */
    s32 pad4;
    s32 next;  /* the next name entry (HubNameNext) */
} HubEntry12;
extern HubEntry12 HubNamePages[];
/* name-entry key, 0x10 bytes, at HubNamePages[].unk0 */
typedef struct HubKey {
    /* 0x0 */ s32 pad0;
    /* 0x4 */ u8 *chars;
    /* 0x8 */ s16 state;  /* switched on */
    /* 0xA */ s8 left;  /* the moves to the next key */
    /* 0xB */ s8 right;
    /* 0xC */ s8 up;
    /* 0xD */ s8 down;
    /* 0xE */ s16 padE;
} HubKey;
typedef struct PodiumTrophy {
    /* 0x00 */ u8 pad0[offsetof(NAS, Engine)];
    /* 0x54 */ EXTengine *inner;
    /* 0x58 */ u8 pad58[offsetof(NAS, UserInfo) - (offsetof(NAS, Engine) + sizeof(void *))];
    /* 0x5C */ u16 state;
} PodiumTrophy;
#define PODIUM_SIN(k) rcossin_tbl[((((FrameCount * (k)) & 0xFFF) - 0x400) & 0xFFF)].unk2
extern u32 PodiumState;
extern s32 PodiumCamState;
extern s32 PodiumCamTimer;
extern CamXYZ PodiumCamAt;
void PodiumRankPlayers(void);
extern s32 PodiumReady;
extern s32 PodiumRanks[];
extern u8 HubNameCursor[];
extern GfxViewport HubSaveViewport;
extern u8 HubSaveCamera[];

typedef struct KeyedPair {
    s32 key;
    void *val[1];
} KeyedPair;
extern KeyedPair HubLockPanels[];
/* Per-mode record tables for PlaceEngineSlot, indexed by language/player. */
extern s32 *HubSavePanels[];
extern s32 *HubStartPanels[];
extern s32 *HubChallengePanels[];
extern s32 *HubWarpPanels[];
extern s32 *HubPapuPanels[];
extern s32 HubDoorAngles[];
/* A POLY_FT4 as HubInitSparks fills it. */
typedef struct HubFt4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0, g0, b0, code;
    /* 0x08 */ s16 x0, y0;
    /* 0x0C */ u16 uv0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 x1, y1;
    /* 0x14 */ u16 uv1;
    /* 0x16 */ u16 tpage;
    /* 0x18 */ s16 x2, y2;
    /* 0x1C */ u16 uv2;
    /* 0x1E */ u16 pad2;
    /* 0x20 */ s16 x3, y3;
    /* 0x24 */ u16 uv3;
    /* 0x26 */ u16 pad3;
} HubFt4;

/* One row of 8 particles HubInitSparks sets up (0x140 bytes). */
typedef struct HubSparkRow {
    /* 0x000 */ SVECTOR rot[8];
    /* 0x040 */ s32 live[8];  /* per spark, nonzero while it shows */
    /* 0x060 */ u8 pad60[0x40];
    /* 0x0A0 */ SVECTOR vel[8];  /* per spark */
    /* 0x0E0 */ SVECTOR angle[8];
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 unk124;
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
    /* 0x130 */ s32 unk130;
    /* 0x134 */ s32 count;  /* counted up */
    /* 0x138 */ s32 flag;  /* tested for zero */
    /* 0x13C */ s32 brightness;  /* faded by 220/256 */
} HubSparkRow;

typedef struct HubSparkSet {
    /* 0x0 */ HubSparkRow *rows;
    /* 0x4 */ HubFt4 *src;
    /* 0x8 */ HubFt4 *dst;
    /* 0xC */ s32 count;
} HubSparkSet;

typedef struct HubSparks {
    /* 0x00 */ u8 pad0[offsetof(subengine, next)];
    /* 0x5C */ struct HubSparks *next;
    /* 0x60 */ u8 pad60[offsetof(subengine, u6C.ptr) - (offsetof(subengine, next) + sizeof(void *))];
    /* 0x6C */ HubSparkSet set;
} HubSparks;

/* A texture entry: the POLY_FT4 fields HubInitSparks copies. */
typedef struct HubTex {
    /* 0x00 */ u8 pad0[offsetof(obj_texture, info.tpage)];
    /* 0x24 */ u16 tpage;
    /* 0x26 */ u16 clut;
    /* 0x28 */ u16 uv0;
    /* 0x2A */ u16 uv1;
    /* 0x2C */ u16 uv2;
    /* 0x2E */ u16 uv3;
} HubTex;

void HubInitSparks(HubSparks *arg0);

extern s32 PodiumCheerTimer;
extern s32 PodiumCheerKind;
extern s32 HubHeroTouchCount;
extern CamEase HubCamEase;

typedef struct DistPos {
    u8 pad0[offsetof(EXTengine, XYZ)];
    s32 x;
    u8 pad14[4];
    s32 z;
} DistPos;
extern PlaceRec HubDoorMenuRecs[];
extern s32 HubDoorChoices;
extern u8 PodiumScore1[];
extern u8 PodiumScore2[];
extern u8 PodiumScore3[];
extern u8 PodiumScore4[];
extern s32 PodiumScoreCount;

void HubOpenDoorMenu(s32 arg0);


typedef struct PodiumObj {
    u8 pad0[offsetof(NAS, Engine)];
    EXTengine *child;
    u8 pad58[offsetof(NAS, UserInfo.status) - (offsetof(NAS, Engine) + sizeof(void *))];
    u16 f5C;
} PodiumObj;

extern PodiumObj *PodiumTrophyObj;

typedef struct HubSpawnEnt {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 z;
    /* 0x8 */ u8 kind;
    /* 0x9 */ u8 flags;
} HubSpawnEnt;
extern NAS *PodiumHeroNas[];
extern u16 PodiumHeroAnims[];
extern s32 PodiumHeroX[];

#define G6A ((GameState *)GameInfo)

/* HubHeroRing as seen by InitialiseHUB. */
typedef struct {
    /* 0x00 */ s32 pad0;
    /* 0x04 */ s32 angle;
    /* 0x08 */ u32 radius;
    /* 0x0C */ u32 radiusMin;
    /* 0x10 */ u32 radiusMax;
} HubCamState;
#define CAM6A ((HubCamState *)&HubHeroRing)


extern s32 HubSetupFlag;
extern _sfx_handler *HubSelectSfx;
extern u16 HubHeroSizes[10];
extern char *HubHaveTexts[]; /* the "you already have" messages */
extern process_info HubDoorMenu;
extern process_info HubDoorMenu2;

void PlotCrystalFlash(void);


typedef struct FxMid {
    u8 pad0[offsetof(EXTengine, sub)];
    subengine *inner;
} FxMid;

typedef struct FxObj {
    u8 pad0[offsetof(NAS, Engine)];
    FxMid *mid;
    u8 pad58[offsetof(NAS, UserInfo.user1) - (offsetof(NAS, Engine) + sizeof(void *))];
    s32 unk60;
} FxObj;

void HubPathAngles(s32 *arg0, s32 flag, s32 *out);

void HubInitLevelSelect(void);

s32 HubGatewayIsKind(s32 arg0);

u16 HubGatewayKind(void);

void HubDoorMenu2Quit(void);

void HubDoorMenu2Render(void);

void HubDoorMenu2Update(void);

void HubDoorMenu2Init(void);

void HubDoorMenuRender(void);

void HubDoorMenuInit(void);

void HubDoorMenuQuit(void);

void HubShowSub(NAS *arg0, s32 arg1);

void PlotGatewayTrophy(void);

void InitGatewayTrophy(void);

s32 HubDist(DistPos *a, DistPos *b);

s32 HubDistXZ(EXTengine *arg0, EXTengine *arg1);

void HubGoToMode(s32 arg0);

void HubCamFromPath(camera_info_tag *cam, s32 arg1);

void HubHeroSetIdle(s32 arg0);

s32 HubHeroTestJump(void);

s32 HubHeroTestKick(void);

void HubHeroTouched(s32 arg0);
void HubPreMoveHero(s32 arg0);

void HubCamTarget(void);

void HubShowShadow(s32 arg0);

void HubHideShadow(s32 arg0);

void HubCreateShadow(s32 arg0);

void HubInitHero(s32 arg0);

s32 HubNearestGateway(void);

void HubRenderScene(void);

void HubUpdateRoom(void);

void HubCullObjects(void);

void *HubLookup(s32 arg0);

void HubCreditsEffects(void);

void PodiumCheer(void);

void PlotPodiumOdo(NAS *arg0);

void PlotSparkGFX(NAS *arg0);

void PodiumScoresUp(void);

void PodiumScoresDown(void);

void PodiumReleaseTrophy(void);

void InitPodiumTrophy(NAS *arg0);

void InitPodiumOdo(NAS *arg0);


extern ScriptInit HubPathScriptInit;

s32 HubSelectedLevel(void);

void InitPathEffect(NAS *nas);

void InitCrystalFlash(FxObj *arg0);

void HubSpawnFlash(s32 *arg0, s32 arg1);

s32 HubHeroTestButton(s32 arg0);

void PodiumFindTies(void);

void PodiumRenderScene(void);

void UpdatePodium(void);

void HubSaveMenuRender(void);

void HubSaveMenuQuit(void);

void HubSaveMenuUpdate(void);

void HubNameEntryKey(s8 arg0);
typedef struct SubInit {
    s16 a;
    s16 b;
    void *c;
    s32 pad[2];
} SubInit;

typedef struct SubInitList {
    SubInit *items;
    s32 count;
} SubInitList;

extern subengine HubNameSubs[];

void HubSaveMenuSubs(SubInitList *list);

extern PlaceRec HubNameLetters[];
extern PlaceRec HubNamePanel[];

void HubSaveMenuReset(void);

void HubStub(void);

typedef struct HubRenderObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[offsetof(subengine, otz) - 0x4];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[offsetof(subengine, u6C) - (offsetof(subengine, otz) + 2)];
    /* 0x6C */ obj_dbase *sub;
    /* 0x70 */ u8 pad70[offsetof(subengine, entity) - offsetof(subengine, u70)];
    /* 0x74 */ u16 entity;
} HubRenderObj;

void HubInitGateways(void);

void HubProjectObj(HubRenderObj *obj, s16 *out);

/* HubProject's body, inlined into HubProjectObj */
#define HUB_BOUND_XY(v, b, p, xy)     \
    {                                   \
        s16 *q_ = (p);                  \
        gte_ldv0(v);                    \
        gte_rtps();                     \
        gte_stsxy(q_);                  \
        if ((xy)[0] < (b)[0]) {         \
            (b)[0] = (xy)[0];           \
        }                               \
        if ((xy)[0] > (b)[2]) {         \
            (b)[2] = (xy)[0];           \
        }                               \
        if ((xy)[1] < (b)[1]) {         \
            (b)[1] = (xy)[1];           \
        }                               \
        if ((xy)[1] > (b)[3]) {         \
            (b)[3] = (xy)[1];           \
        }                               \
    }
void HubDrawObj(HubRenderObj *obj, void *arg1);

void HubProject(SVECTOR *v, s16 *b);

typedef struct PodiumInner {
    u8 pad0[offsetof(subengine, u6C)];
    u8 *script;
} PodiumInner;

typedef struct PodiumMid {
    u8 pad0[offsetof(EXTengine, flags)];
    u32 flags;
    u8 pad10[offsetof(EXTengine, sub) - (offsetof(EXTengine, flags) + 4)];
    PodiumInner *inner;
} PodiumMid;

typedef struct PodiumScriptObj {
    u8 pad0[offsetof(NAS, Engine)];
    PodiumMid *mid;
    u8 pad58[offsetof(NAS, UserInfo.user2) - (offsetof(NAS, Engine) + sizeof(void *))];
    s32 unk64;
} PodiumScriptObj;

extern ScriptInit PodiumScript0;
extern ScriptInit PodiumScript1;
extern ScriptInit PodiumScript2;
extern ScriptInit PodiumScript3;
extern ScriptInit PodiumScript4;
extern ScriptInit PodiumScript5;
extern ScriptInit PodiumScript6;
extern ScriptInit PodiumScript7;

void PodiumInitScript(s32 arg0, s32 arg1);
extern u8 HubSelectFlag;
extern s32 HubSelectTimer;
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
} HubPTag;

void HubDoorMenuUpdate(void);

void HubLevelSelect(void);

void HubSpawnEffects(void);

void PlotPodiumTrophy(PodiumTrophy *arg0);

void PodiumShowScores(void);

void InitSparkGFX(NAS *arg0);

void HubTrailPoint(HubTrailDst *d, HubTrail *t, s32 idx);

void HubDrawSparks(HubSparks *arg0, MATRIX *m);

extern FxDef HubSparkFxDef; /* the warp room's sparks */

void PodiumSpawnHero(s32 arg0, HubSpawnEnt *arg1);

void HubSaveMenuInput(void);

void HubSaveMenuInit(void);
/* Extension 0x10000000 of the path-following objects HubSpawnEffects spawns
 * (0x1626); PlotPathEffect is their Plot. */
typedef struct PathExt {
    /* 0x00 */ EXT base;
    /* 0x0C */ KeyBlock *path;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 state;
    /* 0x18 */ s32 pos;
    /* 0x1C */ u32 keyFlags;
    /* 0x20 */ s32 dir;
    /* 0x24 */ s32 speed;
    /* 0x28 */ KeyEntry *key;
    /* 0x2C */ EXTanimator *anim;
    /* 0x30 */ s32 timer;
    /* 0x34 */ s32 kind;
    /* 0x38 */ NAS *fx;
    /* 0x3C */ s32 absAngle;
} PathExt;

/* PlotPathEffect's rotation buffer (x pitch, y yaw). */
typedef struct PathRot {
    s32 x, y, z, pad;
} PathRot;


void HubUpdateGateways(void);

void PlotPathEffect(NAS *nas);

#endif /* BASH_HUB_HUBCORE_H */
