#ifndef BASH_MAIN_H
#define BASH_MAIN_H

/* Functions and data defined in main.exe's game code. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/main.c */

#include "common.h"
#include "bash/nas.h"

/* A viewpoint: its position (+0xC) is subtracted from world positions before
 * they go through the GTE (func_8002B6D0, func_8002B760). */
typedef struct {
    s32 unk0[3];
    VECTOR pos; /* 0x0C */
} View;

/* Scratch for one GTE transform (0x800569CC, 0x50 bytes; the next symbol is
 * 0x80056A1C). func_8002B6D0 addresses `in` as `&out - 0x40`, so both are one
 * object. */
typedef struct {
    SVECTOR in;       /* 0x00: position relative to the view */
    u8 unk8[0x38];
    VECTOR out;       /* 0x40: rotated and translated position */
} GteScratch;

/* A model whose vertices are interpolated between two sets (func_8001C008,
 * func_8001C0F0). Only the run list is known. */
typedef struct {
    u8 unk0[0x14];
    s32 runs; /* 0x14: byte offset from this field to a list of (u8, u8 count)
               * pairs ending in a count of 0xFF */
} MorphModel;

/* A block holding a table at a byte offset from its field at +0x20. */
typedef struct {
    u8 unk0[0x20];
    s32 table; /* 0x20: byte offset from this field */
} RelTable;

typedef struct GfxModel {
    u8 unk0[0xC];
    RelTable *colors; /* 0x0C: u32 colours, indexed by a face's 13-bit indices */
} GfxModel;

/* Graphics state at 0x80056998 (placeholder name; 161 references). Only the
 * fields func_8001CD04 and func_8001D048 use are known: two pointers it aims
 * at buffers inside the struct itself, a block at +0x28 it passes to
 * func_8001965C, and the ordering table that primitives are added to. */
typedef struct {
    struct GfxModel *unk0;
    u8 unk4[4];
    s32 *farColor; /* 0x08: r, g, b for gte_ldfcdir */
    s32 dp;        /* 0x0C: depth-cue factor for gte_lddp */
    u8 unk10[0x18];
    u8 unk28[0x18];
    u32 *ot;   /* 0x40 */
    s16 otzAdd; /* 0x44: added to the average Z before it is halved */
    s16 otLen;  /* 0x46: entries in ot */
    u8 unk48[0x3C];
    u8 *unk84; /* 0x84: = unkCC */
    u8 *unk88; /* 0x88: = unk10C */
    u8 unk8C[0x40];
    u8 unkCC[0x40];
    u8 unk10C[0x40];
} GfxState;

/* Loads GTE vector 0 from two words (vx|vy, vz) in general registers
 * (lw + mtc2), where Sony's gte_ldv0 loads straight from memory (lwc2). */
#define gte_ldv0_w(xy, z) __asm__ volatile (				"mtc2	%0, $0;"						"mtc2	%1, $1"							:								: "r"(xy), "r"(z) )

/* A VAB file in memory: the header and body as byte offsets from its start
 * (func_8002ADD4 opens it with SsVabOpenHead / SsVabTransBody). */
typedef struct {
    s32 body;
    s32 head;
} VabFile;

typedef struct {
    u8 *head;
    s32 id;   /* set once the transfer completes */
    s32 unk8;
} VabSlot;

/* Points drawn as one-pixel tiles by func_80015C1C (an engine API entry). */
typedef struct {
    SVECTOR v;
    u8 unk8[0x10];
} PointSpriteSrc;

typedef struct {
    PointSpriteSrc *src;
    TILE_1 *prims[2]; /* one per frame buffer (D_8005B690 & 1) */
    s32 count;
} PointSpriteSet;

typedef struct {
    u8 unk0[0x6C];
    PointSpriteSet set; /* 0x6C */
} PointSprites;

/* Filled by func_8001965C for func_8001CD04, which loads it into the GTE. */
typedef struct {
    MATRIX *rot;
    VECTOR *trans; /* may be NULL: translation left as it is */
    s16 ofx;       /* scaled by the screen width / 640 */
    s16 ofy;       /* halved */
} GteSetup;

/* Returned by func_8001562C: points with flag bits (func_80015780 picks the
 * first point with a given flag). */
typedef struct {
    s16 vx, vy, vz;
    u16 flags;
} FlagPoint;

typedef struct {
    s16 count;
    s16 unk2;
    FlagPoint pts[1]; /* [count] */
} PointList;

void func_80011474(void);
s32 func_8001151C(u8 *arg0);
void func_80011530(u8 *arg0);
void func_80011544(u8 *arg0, u8 * *arg1);
s16 func_8001156C(u8 *arg0);
u8 *func_80011654(s32 arg0, u8 *arg1);
u8 *func_80011748(s32 arg0, u8 *arg1);
void func_80011B74(u8 *arg0, u8 *arg1);
void func_80011D00(s32 arg0);
void func_80011D28(u8 *arg0);
u8 *func_80011DB4(u8 *arg0);
void func_80012DDC(s32 arg0);
void func_80012E0C(void);
void func_80012E14(u8 *arg0);
void func_80012E88(void);
void func_80012F90(u8 *arg0);
u8 *func_80013110(u8 *arg0);
s32 func_80013160(u8 *arg0);
void func_80013C94(u8 *arg0, s32 arg1);
void func_80014388(u8 *arg0);
void func_8001452C(s32 arg0, s32 arg1);
s32 *func_800153A8(void);
s32 func_80015480(s32 arg0, s32 arg1);
s32 func_800154A0(s32 arg0, s32 arg1);
void SetRandomSeed(s32 arg0, s32 arg1);
void func_80015D08(u8 *arg0);
s32 func_80015E94(u8 *arg0, u8 *arg1);
s32 func_800160F8(u8 *arg0, s32 arg1);
void func_8001612C(u8 *arg0, s32 arg1);
void func_80016234(u8 *arg0, u8 *arg1);
void func_800162E4(u8 *arg0, s32 arg1);
void func_800163E0(u8 *arg0, s32 arg1);
u8 *func_80016C18(s32 *arg0, u8 *arg1);
void func_80016D44(u8 *arg0);
void func_80016EF4(u8 *arg0);
void func_80017070(void);
void func_80017094(u8 *arg0, s32 arg1, s32 arg2);
void func_80017138(u8 *arg0);
void func_800184D4(u8 * *arg0);
s32 *func_80018570(u8 *arg0);
s32 *func_800185F8(u8 *arg0);
void func_80018728(s32 arg0, s32 arg1);
void func_80018764(s32 *arg0);
void func_8001888C(void);
void func_800189E0(u8 *arg0);
void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);
void func_8001C0F0(s32 p, MorphModel *model, u16 *ia, u16 *ib, s16 (*verts)[3]);
void func_8001965C(void *arg0, void *arg1, void *arg2, u8 *arg3, GteSetup *out);
void func_8001CD04(void *arg0, void *arg1, void *arg2);
void func_800142D4(void *arg0);
PointList *func_8001562C(subengine *sub);
FlagPoint *func_80015780(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out);
void func_8001C2C0(u8 *arg0);
void func_8001C378(u8 *arg0);
void func_8001CBD4(u32 arg0);
void func_8001CC08(void);
void func_8001D174(EXTengine *eng);
void func_8001D220(EXTengine *eng);
void func_8001D2B4(subengine *first, subengine *last);
void func_8001D334(subengine *first);
s32 func_80018F3C(EXTengine *eng, s32 n);
s32 func_8001D390(EXTengine *eng, s32 n);
void func_8001D604(void);
void func_8001DDB4(s32 *arg0);
void func_8001DDD8(u8 *arg0);
void func_8001DF74(u8 *arg0, u8 * *arg1);
void func_8001E054(u8 *arg0, s32 arg1);
void func_8001E588(u8 *arg0, s32 arg1, s32 arg2);
void func_8001E824(u8 *arg0);
void func_8001E838(u8 *arg0, s32 arg1);
void func_8001E848(u8 *arg0, s32 arg1);
void func_8001E898(u8 *arg0);
void func_8001E934(u8 *arg0, s32 arg1);
void func_8001E948(void);
void func_8002141C(u8 *arg0);
void func_80021770(u8 *arg0, s32 *arg1);
void func_80021990(u8 *arg0, s32 *arg1);
void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4);
void func_80021ED8(u8 *arg0);
void func_80021F14(u8 *arg0);
u8 *func_80021F6C(void);
void func_80021FB0(void);
void func_80022030(void);
void func_800220E8(void);
void func_800222BC(void);
void func_800222DC(void);
void func_800222FC(void);
void func_8002232C(void);
void func_800227C0(void);
void func_800227FC(void);
void func_80022838(void);
void func_800228A4(u8 *arg0, s32 arg1);
void func_800229D4(void);
void func_80025AE4(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80025F8C(void);
void func_80025F94(void);
void func_80025F9C(void);
void func_80025FA4(void);
void func_80026308(void);
void func_80026418(void);
void func_80026420(void);
void func_80026428(void);
void func_800264B4(void);
void func_80026A04(u8 *arg0);
void func_80026AD8(u8 *arg0);
void func_80026B70(u8 * *arg0, u8 *arg1);
void func_80026BF4(u8 *arg0);
void func_80026C38(u8 *arg0, u8 *arg1);
void func_80026C6C(u8 *arg0, u8 *arg1);
void func_80026C94(u8 *arg0, u8 *arg1);
void func_80026D00(void);
void func_80027040(s32 arg0);
void func_800270B0(u8 *arg0);
s32 func_8002718C(void);
void func_8002726C(void);
void func_8002759C(void);
void func_800276A4(void);
void func_800278BC(s32 *arg0, s32 arg1);
void func_80027908(void);
void func_80027A74(void);
void func_80027A94(void);
void func_80027A9C(void);
void func_80028184(u8 *arg0, s32 arg1, s8 arg2);
void func_800281A0(u8 *arg0, s32 arg1);
void func_800282D8(u8 *arg0);
void func_80028D38(void);
void func_8002A2DC(void);
void func_8002A2E4(void);
void func_8002A35C(u8 *arg0);
void func_8002A4B8(void);
void func_8002A67C(void);
void func_8002A740(u8 *arg0);
void func_8002AA50(u8 *arg0);
void func_8002AA8C(u8 *arg0);
void func_8002AAA0(void);
void func_8002AAAC(void);
void func_8002AB44(void);
void func_8002ABA8(void);
void func_8002AC18(void);
void func_8002AC40(void);
void func_8002AC70(void);
void func_8002B4B8(void);
s32 func_8002B020(VECTOR *v, s32 *range);
s32 func_8002B6D0(VECTOR *pos, View *view);
void func_8002B760(VECTOR *pos, View *view, VECTOR *out, s32 *sxy);
void func_8002B96C(void);
s32 func_8002BB80(void);
void func_8002C284(s32 arg0);
void func_8002C290(void);
void func_8002C29C(void);
void func_8002C2AC(void);
void func_8002C3F0(s32 arg0, s32 *arg1);
void func_8002C428(void);
void func_8002D134(s32 arg0);
void func_8002D15C(void);
void func_8002D254(void);
void func_8002D274(void);
void func_8002D33C(u8 *arg0);

extern GteScratch D_800569CC;
extern SVECTOR D_80056AC8[];
extern s32 D_80064478[];
extern u8 D_800527C4[];
extern s32 D_80064414;
extern s32 D_8006559C;
extern s32 D_800655AC;
extern void *D_80051240;
extern void *D_80050F3C;
extern u8 D_80050F40[]; /* 63 x 0xC pool, free list in D_80051240 */
extern u8 D_8005063C[]; /* 63 x 0x24 pool, free list in D_80050F3C */
extern s32 D_800517B0;
extern s32 D_800517B4;
extern s32 RandomSeedA;
extern s32 RandomSeedB;
extern void *D_80058B70;
extern s32 D_8005A5D8;
extern s32 D_800655A8;
extern s32 D_800655C0;
extern s32 D_800655C4;
extern s32 D_800675CC;
extern u8 D_8004E0F0[];
extern u8 D_80055684[];
extern void *D_8005AA34;
extern void *D_8005AA24;
extern u8 D_8006557C[];
extern u8 D_8005A614[];
extern s32 D_800675C8;
extern s32 D_8005A620;
extern u8 D_800556F4[];
extern u8 D_8001C378[];
extern s32 D_8005AA58;
extern u8 D_80051640[];
extern u8 D_800555A4[];
extern s16 D_8005A648;
extern u16 D_8005A646;
extern s32 D_8005B2B4;
extern u8 *D_8005B63C;
extern s32 D_8006EA00;
extern VabFile *D_800654C0;  /* VAB being transferred to the SPU, or NULL */
extern VabSlot *D_800654C4;  /* where its id goes */
extern s32 D_800654C8;       /* the id from SsVabOpenHead */
extern u8 D_800644A8[];
extern u8 D_8005A688;
extern s32 D_80063A7C;
extern s32 D_800654B8;
extern s32 D_800655B0;
extern s32 ASTCount;            /* entries in ASTTable */
extern ASTEntry *ASTTable;      /* the AST table in use */
extern s32 D_80058AC8;
extern void *SfxActiveVoices;
extern s32 D_800637B4;
extern s32 D_8005AA48;
extern void *D_80058B78;
extern RECT D_8005B640;
extern void *D_80058B74;
extern void *D_80058AD0;
extern s32 D_80051380;
extern u8 D_800536CC[];
extern CdlLOC D_8006EA14;
void func_80016D98();
void func_80016188();
extern RECT *D_8005B698;
void *func_800112A4(void *base, s32 offset, u8 *arg2);
void func_80014540(u8 *arg0, RECT *rect);
void func_8002ACA0(s32 id, s32 arg1);
extern GfxState D_80056998;
extern u16 D_800569DC;
extern s32 D_8005B690; /* frame counter: its low bit picks the frame buffer */
void func_8001A6D4(void *arg0, u32 hidden);
s32 func_8001A0D8(void *arg0, u32 flags);
extern u8 D_80053694[];
void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl);
extern EXTmotion D_8005A9CC;
extern subengine D_8005A830;
void FreeExtension(void *ext);
s32 func_80032F3C(s32);         /* abs or labs (LIBC2: identical objects) */
s32 func_80025E00(EXTengine *eng, s32 y);
extern s32 (*D_8005AA94)(EXTengine *eng); /* optional hook: replaces func_80025E00 */
extern s32 D_8005B5B8;
extern void (*D_800643F8)();
extern NAS *NASActiveTail;
extern s32 *D_80056858;
extern NAS *NASActiveHead;
extern s32 D_800654AC;
extern s32 D_800654B0;
extern s32 D_800654B4;
extern s32 D_8004E0D8;
extern s32 D_800590CC;
extern subengine *D_80058ACC;
extern s32 D_80058B68;
extern s32 D_80058B6C;
void func_8001DFF8();
extern s8 D_8005A63C;
extern void *D_8005699C;
extern void *D_8005AA38;
extern void *D_80056864;
extern u8 D_8005B790[];
extern s32 D_800637A8;

extern NAS *NASFreeList;
extern NAS NASTemplate;            /* copied over each spawned NAS */
extern EXTengine EngineTemplate;   /* copied over each new engine */
extern s16 AtanTable[];            /* get_angle */

EXTmotion *AddMotion(NAS *nas);

void func_8001D3B0(EXTengine *eng, s32 n, subengine *sub);

EXTengine *AddEngine(NAS *nas, s32 id);

s32 func_80025BBC(EXTengine *eng);

void func_80015758(SVECTOR *v, VECTOR *out);

void func_8001C008(s32 p, MorphModel *model, SVECTOR *src, SVECTOR *fc);

void func_8001D048(SVECTOR *v, POLY_GT4 *poly);

void func_8002AEE0(void);

u32 func_8001C974(NAS *nas, u8 *arg1, u32 flags);

void func_8001C690(NAS *nas, u8 *arg1);

void func_80015C1C(PointSprites *ps, MATRIX *m);

void func_80016F34(u8 *arg0, u8 *arg1);

void *AddExtension(void *owner, s32 size, s32 id);

void func_80012E54(void);

void func_8001325C(void);

#endif /* BASH_MAIN_H */
