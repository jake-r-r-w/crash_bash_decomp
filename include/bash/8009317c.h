#ifndef BASH_8009317C_H
#define BASH_8009317C_H

/* Functions and data defined in src/overlays/engine/8009317c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8009317c.c */

#include "common.h"
#include "bash/engine.h"

extern u16 D_8009F994[];

/* 0x38-byte records at D_80052A54 (load_initial_dbases). */
/* D_8005A614 as seen by load_initial_dbases. */
typedef struct View939FC {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x20];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x20];
    /* 0x50 */ u8 unk50[4];
} View939FC;

typedef struct Blk8 {
    s32 w0;
    s32 w4;
} Blk8;
typedef struct Blk38 {
    Blk8 head;
    u8 pad8[0x30];
} Blk38;

typedef struct Tmpl1C {
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
} Tmpl1C;

extern Tmpl1C D_800A0D44[];
extern u8 D_8001C690[];
extern u8 *D_800A001C[];

void func_80094058(void);

void func_8009407C(void);

void func_80094120(void);

void func_8009414C(void);

void func_800941BC(void);

void func_800941EC(void);

void func_80094274(void);

void func_80094D88(s32 arg0, process_info *arg1);

s32 func_80094F30(void);

void func_80094F60(void);

void func_80094FA8(void);

void func_80094FE0(void);

void func_80095A04(s32 arg0);

/* 0x24-byte placement record read by func_800952F8. */
typedef struct PlaceRec {
    /* 0x00 */ s32 type;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ s16 z;
    /* 0x0A */ s16 scale;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20;
} PlaceRec;

/* subengine as filled by func_800952F8. */
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

/* D_8005A614 as seen by func_800952F8. */
typedef struct PlaceGameView {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x2C - 0x14];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u16 pad30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 pad34[0x6C - 0x34];
    /* 0x6C */ s32 unk6C;
} PlaceGameView;

static inline void PlaceSprite(PlaceSub *s, s32 x, s32 y, s32 img, s32 clut) {
    if (s != NULL) {
        s->flags = 0x10008000;
        s->hook = D_8001C448;
        s->x = x;
        s->y = y;
        s->unk6C = img;
        s->unk7C = clut;
    }
}

static inline void PlaceTmpl(PlaceSub *s, s32 x, s32 y, s32 z, s32 scale, s32 idx) {
    Tmpl1C *t;

    if (s != NULL) {
        t = &D_800A0D44[idx];
        s->flags = t->f0 | 0x12008000;
        s->hook = D_8001C690;
        if ((x & 0xC000) == 0x8000) {
            y = y * 640 / D_8005B698->w;
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

void func_800952F8(subengine *sub, s32 *recs);

void func_80095BEC(s32 arg0, s32 *recs);

EXTengine *GetEngineSlot(s32 arg0);

void func_80095258(u8 *arg0);

s32 func_8009317C(void);
extern s32 *D_8009F954[];
extern s32 D_8009F8E4[];
extern u8 *D_80098598;

void func_80093588(void);

void func_800940B0(s32 arg0, s32 arg1);

void func_8009421C(void);

s32 func_80095D88(s32 *rec);

void func_800936C4(void);

void func_800945C0(EngineStartArgs *arg0);

void func_8009342C(void);

void func_80095020(void);

void func_800943B8(void);

void func_8009440C(void);

void func_80095E5C(u8 *arg0, u8 *arg1, s32 arg2);

s32 func_80095F48(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4);

void func_80094DD0(void);

void func_80093270(void);

void realign_panel_dbase(void);

void level_load_map(void);

void func_80094EB8(void);

s32 func_800950FC(void);

void func_80095160(EngineStartArgs *arg0);

void func_80095DBC(void);

void func_80094754(void);

void func_80094860(void);

s32 func_800951D0(s32 arg0);

s32 func_80095F88(u8 *p, s16 a1, s32 a2, s16 a3, s32 a4, s32 idx);

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
extern Blk32 D_800A0BA0;
extern CamVec3 D_800A0BC0;
extern u8 D_800A0BF3;
extern u8 *D_800A0D34;

/* D_8005A614 as seen by func_80093BE8. */
typedef struct EngStateView {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x30 - 0x14];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 pad32;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x42 - 0x36];
    /* 0x42 */ s16 unk42;
    /* 0x44 */ u8 pad44[0xCC - 0x44];
    /* 0xCC */ u8 unkCC[4];
} EngStateView;
typedef struct EngRgb {
    u8 r, g, b;
} EngRgb;
typedef struct EngColor {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ EngRgb rgb;
    /* 0x27 */ u8 code;
} EngColor;
typedef struct EngColorSrc {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ EngColor *col;
} EngColorSrc;
extern process_info D_8009F998;
extern process_info D_8009FA00;
extern u8 D_8004E998[];
extern process_info D_8009F720;
void func_80093854(void);
void load_initial_dbases(void);
void func_80092AF4(process_info *process, s32 speed);

void func_80093BE8(void);

void func_80094A28(void);

void func_80094B38(void);

void func_80095A60(s32 arg0, s32 arg1);

#endif /* BASH_8009317C_H */
