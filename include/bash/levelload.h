#ifndef BASH_LEVELLOAD_H
#define BASH_LEVELLOAD_H

/* Functions and data defined in src/overlays/engine/levelload.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/levelload.c */

#include "common.h"
#include "bash/engine.h"

extern u16 LoadingEntities[];



/* A text panel's template (TextTemplates; the Oct 9 prototype's sub_string_info, 0x1C bytes):
 * PlaceTextSub copies it into the panel's subengine. */
struct sub_string_info {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    u16 f10;
    u16 pad12;
    u16 f14;
    u16 pad16;
    u16 f18;
    u16 pad1A;
};

extern sub_string_info TextTemplates[];
extern u8 *ModeFileGroups[];

void QuitLevelLoad(void);

void RenderLevelLoad(void);

void QuitLoadingScene(void);

void RenderLoadingScene(void);

void InitLoadingCamera(void);

void QuitLoadingScreen(void);

void InitLoadingScreen(void);

void GotoScriptScene(ScriptScene *arg0, process_info *arg1);

s32 SceneModelLoaded(void);

void ReleaseOldSceneModel(void);

void LoadSceneModel(void);

void SceneNextProcess(void);

void KillEngineSlot(s32 arg0);


/* subengine as filled by ApplyPlaceRecs. */
typedef struct PlaceSub {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x20 - 0x16];
    /* 0x20 */ s32 sx;
    /* 0x24 */ s32 sy;
    /* 0x28 */ s32 sz;
    /* 0x2C */ u8 pad2C[0x50 - 0x2C];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ void *hook;
    /* 0x58 */ void *release;
    /* 0x5C */ struct PlaceSub *next;
    /* 0x60 */ void *prev;
    /* 0x64 */ PlaceRec *rec;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ u16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ u8 pad7A[2];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u16 unk84;
    /* 0x86 */ u16 unk86;
    /* 0x88 */ s32 unk88;
} PlaceSub;


static inline void PlaceSprite(PlaceSub *s, s32 x, s32 y, s32 img, s32 clut) {
    if (s != NULL) {
        s->flags = 0x10008000;
        s->hook = posdst_flash;
        s->x = x;
        s->y = y;
        s->unk6C = img;
        s->unk7C = clut;
    }
}

static inline void PlaceTmpl(PlaceSub *s, s32 x, s32 y, s32 z, s32 scale, s32 idx) {
    sub_string_info *t;

    if (s != NULL) {
        t = &TextTemplates[idx];
        s->flags = t->f0 | 0x12008000;
        s->hook = draw_nas_model;
        if ((x & 0xC000) == 0x8000) {
            y = y * 640 / ScreenRect->w;
        }
        s->unk6C = t->f0;
        s->unk70 = x;
        s->unk76 = y;
        s->unk72 = z;
        s->unk78 = scale;
        s->unk7C = t->f4;
        s->unk84 = t->f10;
        s->unk86 = t->f14;
        s->unk80 = t->fC;
        s->unk88 = t->f8;
        s->unk68 = t->f18;
    }
}

void ApplyPlaceRecs(subengine *sub, PlaceRec *recs);

void PlaceEngineSlot(s32 arg0, PlaceRec *recs);

EXTengine *GetEngineSlot(s32 arg0);

void SetupSceneMarks(u8 *arg0);

s32 LoadModeText(void);
extern s32 *ModeTextTables[];
extern s32 *ModeTextFiles[];
extern u8 *ModeTextLoadAddr;

void LoadModeFiles(void);

void LoadModeDbases(s32 arg0, s32 arg1);

void RenderLoadingScreen(void);

s32 CountPlaceRecs(s32 *rec);

void InitLevelLoad(void);

void SetupScriptScene(ScriptScene *arg0);

void UpdateLoadingScene(void);

void UnloadScene(void);

void LoadLoadingModel(void);

void DrawSceneMarks(void);

void draw_animated_sub(struct PlaceSub *arg0, u8 *arg1, s32 arg2);

s32 PlaceFlashSub(struct PlaceSub *p, s32 a1, s32 a2, s32 a3, s32 a4);

void QuitScriptScene(void);

void AnimateLoadingIcon(void);

void realign_panel_dbase(void);

void level_load_map(void);

void RenderScriptScene(void);

s32 SceneVabReady(void);

void SceneSoundInit(ScriptScene *arg0);

void ResetEnginePool(void);

void InitScriptScene(void);

void UpdateSceneVabCues(void);

ScriptScene *PickModeVariant(s32 arg0);

s32 PlaceTextSub(struct PlaceSub *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx);

typedef struct MarkSrcEnt {
    /* 0x00 */ s16 val;
    /* 0x02 */ u8 pad2[0x32];
} MarkSrcEnt;
typedef struct MarkSrc {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ s32 count;
    /* 0x58 */ u8 pad58[0xE];
    /* 0x66 */ MarkSrcEnt ent[1];
} MarkSrc;
typedef struct MarkObj {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ MarkSrc *src;
} MarkObj;
typedef struct MarkCtx {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ MarkObj *obj;
    /* 0x8 */ s32 unk8;
    /* 0xC */ u8 unkC;
} MarkCtx;
extern Blk32 SceneScriptRot;
extern CamVec3 SceneScriptPos;
extern script_handler *SceneScript;

extern process_info LoadingProcess;
extern process_info LoadingProcess2;
extern process_info RetryProcess;
void QueueHeroModels(void);
void load_initial_dbases(void);
void GotoProcess(process_info *process, s32 speed);

void UpdateLevelLoad(void);

void StartSceneScript(void);

void UpdateScriptScene(void);

void InitEngineSlot(s32 arg0, s32 arg1);

#endif /* BASH_LEVELLOAD_H */
