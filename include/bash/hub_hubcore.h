#ifndef BASH_HUB_HUBCORE_H
#define BASH_HUB_HUBCORE_H

/* Functions and data defined in src/overlays/hub/hubcore.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/hub/hubcore.c */

#include "common.h"
#include "bash/hub_ovl.h"

typedef struct HubSpawnExt {
    u8 pad0[0xC];
    s32 *unkC;
    u8 pad10[0x34 - 0x10];
    s32 unk34;
} HubSpawnExt;
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
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 pad124;
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
    /* 0x130 */ s32 pad130;
    /* 0x134 */ s32 pad134;
    /* 0x138 */ s32 unk138;
} HubTrail;
typedef struct HubTrailDst {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[0x76 - 0x10];
    /* 0x76 */ u16 unk76;
} HubTrailDst;

extern s32 HubDoorFade;
extern s32 HubHeroPressed;
extern EXTengine HubNameEngine;
extern s32 HubNameNext;
extern s32 HubNamePage;
extern s32 HubNameLen;
typedef struct HubEntry12 {
    s32 unk0;
    s32 pad4;
    s32 unk8;
} HubEntry12;
extern HubEntry12 HubNamePages[];
/* name-entry key, 0x10 bytes, at HubNamePages[].unk0 */
typedef struct HubKey {
    /* 0x0 */ s32 pad0;
    /* 0x4 */ u8 *unk4;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s8 unkA;
    /* 0xB */ s8 unkB;
    /* 0xC */ s8 unkC;
    /* 0xD */ s8 unkD;
    /* 0xE */ s16 padE;
} HubKey;
typedef struct PodiumFx {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x52 - 0x16];
    /* 0x52 */ s16 unk52;
} PodiumFx;
typedef struct TrophyInner {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x5E - 0x38];
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ PodiumFx *fx;
} TrophyInner;
typedef struct PodiumTrophy {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ TrophyInner *inner;
    /* 0x58 */ u8 pad58[4];
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
    s32 val[1];
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
    /* 0x040 */ s32 unk40[8];
    /* 0x060 */ u8 pad60[0x40];
    /* 0x0A0 */ SVECTOR unkA0[8];
    /* 0x0E0 */ SVECTOR unkE0[8];
    /* 0x120 */ s32 unk120;
    /* 0x124 */ s32 unk124;
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
    /* 0x130 */ s32 unk130;
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 unk138;
    /* 0x13C */ s32 unk13C;
} HubSparkRow;

typedef struct HubSparkSet {
    /* 0x0 */ HubSparkRow *rows;
    /* 0x4 */ HubFt4 *src;
    /* 0x8 */ HubFt4 *dst;
    /* 0xC */ s32 count;
} HubSparkSet;

typedef struct HubSparks {
    /* 0x00 */ u8 pad0[0x5C];
    /* 0x5C */ struct HubSparks *next;
    /* 0x60 */ u8 pad60[0xC];
    /* 0x6C */ HubSparkSet set;
} HubSparks;

/* A texture entry: the POLY_FT4 fields HubInitSparks copies. */
typedef struct HubTex {
    /* 0x00 */ u8 pad0[0x24];
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
    u8 pad0[0x10];
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

typedef struct PodiumChild {
    u8 pad0[0xC];
    s32 flags;
    u8 pad10[0x1C];
    s32 f2C;
    s32 f30;
    s32 f34;
    u8 pad38[0x24];
    u8 f5C;
    u8 pad5D;
    s16 f5E;
} PodiumChild;

typedef struct PodiumObj {
    u8 pad0[0x54];
    PodiumChild *child;
    u8 pad58[4];
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

/* HubGateways entries (0x28 bytes) as seen by InitialiseHUB. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x14];
} HubCamEntry;
#define CAMTBL6A ((HubCamEntry *)HubGateways)

extern s32 HubSetupFlag;
extern _sfx_handler *HubSelectSfx;
extern u16 HubHeroSizes[10];
extern char *HubHaveTexts[]; /* the "you already have" messages */
extern process_info HubDoorMenu;
extern process_info HubDoorMenu2;

void PlotCrystalFlash(void);

typedef struct FxInner {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0xA];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[0x24];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x18];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ s16 unk74;
} FxInner;

typedef struct FxMid {
    u8 pad0[0x6C];
    FxInner *inner;
} FxMid;

typedef struct FxObj {
    u8 pad0[0x54];
    FxMid *mid;
    u8 pad58[8];
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

void HubCamFromPath(u8 *cam, s32 arg1);

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

s32 HubLookup(s32 arg0);

void HubCreditsEffects(void);

void PodiumCheer(void);

void PlotPodiumOdo(NAS *arg0);

void PlotSparkGFX(NAS *arg0);

void PodiumScoresUp(void);

void PodiumScoresDown(void);

void PodiumReleaseTrophy(void);

void InitPodiumTrophy(NAS *arg0);

void InitPodiumOdo(NAS *arg0);

typedef struct ScriptExt40 {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[8];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ ScriptCursor *cursor;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} ScriptExt40;

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
    s32 c;
    s32 pad[2];
} SubInit;

typedef struct SubInitList {
    SubInit *items;
    s32 count;
} SubInitList;

extern subengine HubNameSubs[];

void HubSaveMenuSubs(SubInitList *list);

extern s32 HubNameLetters[];
extern s32 HubNamePanel[];

void HubSaveMenuReset(void);

void HubStub(void);

typedef struct HubRenderSub {
    u8 padC[0xC];
    s32 unkC;
} HubRenderSub;

typedef struct HubRenderObj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x68 - 0x4];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ HubRenderSub *sub;
    /* 0x70 */ u8 pad70[4];
    /* 0x74 */ u16 unk74;
} HubRenderObj;

extern u8 HubOtBuffer[];
extern u8 HubDrawEnv[];
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
void HubSetDrawArea(s16 *a, void *b, s32 c, s32 d);
void HubDrawObj(HubRenderObj *obj, void *arg1);

void HubProject(SVECTOR *v, s16 *b);

typedef struct PodiumInner {
    u8 pad0[0x6C];
    u8 *script;
} PodiumInner;

typedef struct PodiumMid {
    u8 pad0[0xC];
    u32 flags;
    u8 pad10[0x5C];
    PodiumInner *inner;
} PodiumMid;

typedef struct PodiumScriptObj {
    u8 pad0[0x54];
    PodiumMid *mid;
    u8 pad58[0xC];
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
    /* 0x00 */ u8 pad00[0xC];
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
