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

/* A block with 12-byte entries at a byte offset from its field at +0x1C + 0x1C
 * (func_80015984). */
typedef struct {
    u8 unk0[0x1C];
    s32 entries; /* 0x1C */
} EntryBlock;

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
    MATRIX unk8C; /* 0x8C: set to identity by func_8001D5D0 */
    u8 unkAC[0x20];
    u8 unkCC[0x40];
    u8 unk10C[0x40];
} GfxState;

/* A 0x1C-byte entry of a sorted table at +0x4C of a block (func_8001E41C). */
typedef struct {
    u8 unk0[0xC];
    s32 key;   /* 0x0C */
    u8 unk10[4];
    u32 flags; /* 0x14 */
    u8 unk18[4];
} KeyEntry;

typedef struct {
    u8 unk0[4];
    s32 count; /* 0x04 */
    u8 unk8[0x44];
    KeyEntry entries[1]; /* 0x4C */
} KeyBlock;

/* Loads GTE vector 0 from two words (vx|vy, vz) in general registers
 * (lw + mtc2), where Sony's gte_ldv0 loads straight from memory (lwc2). */
#define gte_ldv0_w(xy, z) __asm__ volatile (				"mtc2	%0, $0;"						"mtc2	%1, $1"							:								: "r"(xy), "r"(z) )

/* Stores SXY0..2 at +0x10, +0x18, +0x20 of a primitive. None of Sony's
 * gte_stsxy3_* macros uses these offsets; func_800193A8 and func_8001A0D8
 * store them straight off the primitive pointer. */
#define gte_stsxy3_10(r0) __asm__ volatile (				"swc2	$12, 16( %0 );"					"swc2	$13, 24( %0 );"					"swc2	$14, 32( %0 )"					:								: "r"(r0)							: "memory" )

/* A playing sound (PlaySfx), linked into SfxVoicePool's active or free list.
 * The type name is the prototype's (EXTengine.sfx, TA_player's handlers);
 * the field names are placeholders, from PlaySfx and StopSfx. */
struct _sfx_handler {
    struct _sfx_handler **owner; /* 0x00: the caller's handle, cleared on stop */
    SLONG_XYZ posCopy;       /* 0x04: the position, when no handle is given */
    SLONG_XYZ *pos;          /* 0x10: the caller's position, posCopy, or NULL */
    struct _sfx_handler *prev;   /* 0x14: active list */
    struct _sfx_handler *next;   /* 0x18: active list, or the free list */
    s32 flags;               /* 0x1C: PlaySfx's 6th argument */
    s32 unk20;               /* 0x20: PlaySfx's 4th argument (0x1000) */
    s32 unk24;               /* 0x24: PlaySfx's 5th argument (0x1E00) */
    s32 unk28;               /* 0x28: -1 when started */
    s32 unk2C;               /* 0x2C: -1 when allocated; func_8002B8F4 stops it */
    s32 unk30;
    s32 id;                  /* 0x34: the SfxTable id */
};
BASH_SIZE_CHECK(_sfx_handler, 0x38);

/* 24 voices (func_80022984 links them into the free list). */
typedef struct {
    _sfx_handler voices[24];
    _sfx_handler *free;         /* 0x540 */
    _sfx_handler *active;       /* 0x544 (the symbol SfxActiveVoices, used by asm) */
} SfxPool;

/* A VAB file in memory: the header and body as byte offsets from its start
 * (func_8002ADD4 opens it with SsVabOpenHead / SsVabTransBody). */
typedef struct {
    s32 body;
    s32 head;
} VabFile;

typedef struct {
    u8 *head;
    s32 id;   /* set once the transfer completes */
    s32 handle; /* func_80013688's result (func_8002BA10) */
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
    s16 nRecs;
    FlagPoint pts[1]; /* [count], then nRecs FlagRecords */
} PointList;

/* Header of a heap block, 0x18 bytes before the pointer the allocator hands
 * out; the next block follows its size words of data. Free blocks are on the
 * heap's free list, sorted by size (func_800110A8 frees and merges). */
typedef struct MemBlock {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u8 unk2[2];
    /* 0x04 */ void **owner;             /* cleared on free; (void **)-1 for none; NULL when free */
    /* 0x08 */ u32 size;                 /* in words, header not included */
    /* 0x0C */ struct MemBlock *prev;    /* the block before it in memory */
    /* 0x10 */ struct MemBlock *prevFree;
    /* 0x14 */ struct MemBlock *nextFree;
} MemBlock;

#define MEMBLOCK_NEXT(b) ((MemBlock *)((u8 *)(b) + (b)->size * 4 + sizeof(MemBlock)))

/* Header of a pool entry; the data follows it. */
typedef struct PoolNode {
    struct PoolNode *next;
    s32 state;                        /* 2 once handed back (func_80018388) */
} PoolNode;

typedef struct SlotTarget {
    /* 0x00 */ u8 unk0[0x56];
    /* 0x56 */ u16 flag;
} SlotTarget;

typedef struct SlotEntry {
    /* 0x00 */ SlotTarget *target;
    /* 0x04 */ u8 unk4[0x30];
} SlotEntry;

typedef struct SlotList {
    /* 0x00 */ u8 unk0[0x54];
    /* 0x54 */ s32 count;
    /* 0x58 */ SlotEntry entries[1];
} SlotList;

/* D_80056844: entries come from free (func_800183B0) and go back on done. */
typedef struct NodePool {
    PoolNode *free;
    void *mem;                        /* heap block, freed by func_800183DC */
    PoolNode *done;
} NodePool;

/* Data with a table of self-relative offsets at +0x1C: entry i points to
 * &offs[i] + offs[i] (func_80021E44). */
typedef struct OffsetTable {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ s32 offs[1];
} OffsetTable;

/* 0x18-byte entry of an id-keyed table ended by id 0x3E7 (func_800263D8). */
typedef struct IdEntry {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 unk2[0x16];
} IdEntry;

/* Per-pad state, 8 of them at D_80051338 (func_80027F00 resets them). */
typedef struct PadState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x18 - 0x08];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20[2];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A[0x58 - 0x2A];
} PadState;

/* Loaded sound data and the sequence opened from it (D_8006557C). */
typedef struct SeqSlot {
    /* 0x00 */ s32 data;                 /* heap block, freed by func_8002C44C */
    /* 0x04 */ s32 seq;                  /* SsSeq access number, or -1 */
    /* 0x08 */ u8 unk8[0x1C - 0x08];
    /* 0x1C */ s32 unk1C;                /* set to -1 on close */
} SeqSlot;

/* D_80058B88: what func_800229FC passes to func_8002C488 for an index. */
typedef struct MusicEntry {
    /* 0x00 */ s32 file;
    /* 0x04 */ s16 reverb;               /* SsUtSetReverbType */
    /* 0x06 */ u8 unk6[0x10 - 0x06];
    /* 0x10 */ s16 tempo;
    /* 0x12 */ u8 unk12[0x14 - 0x12];
} MusicEntry;

/* D_80050628: file loader state (func_80013238 resets it). Only these fields are known. */
typedef struct LoaderState {
    /* 0x00 */ s32 state;                /* 3 after a reset */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
} LoaderState;

typedef struct LoadNode {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ struct LoadNode *next;
    /* 0x20 */ struct LoadNode *prev;
} LoadNode;

/* A heap (D_8004E0F0 is the main one). Only these fields are known. */
typedef struct Heap {
    /* 0x00 */ u8 unk0[8];
    /* 0x08 */ MemBlock *free;           /* free list, smallest first */
    /* 0x0C */ MemBlock *lowest;         /* lowest free block, or NULL */
    /* 0x10 */ MemBlock *deferred;       /* freed once unk2 ticks run out (func_80011578, func_80011DB4) */
} Heap;

/* A per-frame-buffer allocation slot used by func_8001CC10. */
typedef struct {
    /* 0x0 */ void *ptr;
    /* 0x4 */ u16 size;
} Func8001CC10Slot;

void func_80011474(void);
s32 func_8001151C(u8 *arg0);
void func_80011530(u8 *arg0);
void func_80011544(u8 *arg0, u8 * *arg1);
s16 func_8001156C(u8 *arg0);
u8 *func_80011654(s32 arg0, Heap *arg1);
u8 *func_80011748(s32 arg0, u8 *arg1);
void func_80011B74(u8 *arg0, u8 *arg1);
void func_80011D00(s32 arg0);
void func_80011D28(u8 *arg0);
u8 *func_80011DB4(Heap *arg0);
void func_80012DDC(s32 arg0);
void func_80012E0C(void);
void func_80012E14(u8 *arg0);
void func_80012F04(void *arg0);
s32 func_8001534C(u8 *s);
void func_8001828C(void);
s32 func_80013650(s32 arg0, void (*done)(), void *ctx);
s32 func_80012FFC(void);
void func_8001231C(void);
void func_80010AE8(Heap *arg0);
void func_8002BAE8(void);
void func_8002C894(void);
MemBlock *func_80011C5C(u32 words, MemBlock *blk);
s32 func_80013034(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6);
void func_80029560(u8 *arg0);
void func_8002A5A4(u8 *arg0, u8 *arg1);
void func_80027AD4(u8 *src, u8 *dst, s32 n);
void func_8002C394(s32 arg0, SeqSlot *slot, s32 type);
s32 func_80013688(s32 arg0, void (*done)(), void *ctx);
void func_8002BA10(s32 arg0, VabSlot *slot);
void func_8002ADD4(void *data, VabSlot *slot);
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
void func_80016EF4(u8 *arg0, u8 *arg1);
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
void func_8001AA48(u8 *arg0);
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
void func_800237BC(void);
void func_8002232C(void);
void func_800227C0(void);
void func_800227FC(void);
void func_80022838(void);
void func_800228A4(_sfx_handler *voice, s32 arg1);
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
void func_8001C270(EXTengine *eng);
void func_80026D08(void);
void func_80026430(NAS *nas);
void func_80026AFC(NAS *nas);
void func_80026980(NAS *nas);
void func_80026A54(NAS *nas);
void func_800268BC(NAS *nas);
void func_80026C38(u8 *arg0, u8 *arg1);
void func_80026C6C(u8 *arg0, u8 *arg1);
void func_80026C94(u8 *arg0, u8 *arg1);
void func_80026D00(void);
void func_80027040(s32 arg0);
void func_800270B0(NAS *nas);
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
void func_800369FC(void);
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
typedef struct { s16 unk0; s16 unk2; } TrigEntry;
extern TrigEntry D_80068BD4[];
extern s32 D_80064478[];
extern s32 D_8006E9F8;
extern u8 D_800527C4[];
extern u8 D_80052A54[];
extern s32 D_80064414;
extern s32 D_800569AC;
extern s32 D_8006559C;
extern s32 D_800655A4;
extern s32 D_800655B4;
extern s32 D_800655AC;
extern void *D_80051240;
typedef struct {
    void (*unk0)(void);
    void (*unk4)(void);
    void (*unk8)(void);
    u8 unkC[4];
    u8 *unk10;
} VTable8;
void func_800155FC(void);
void func_8002BEA8(void);
extern VTable8 *D_8004E0DC;
extern s32 D_8004E0E0;
extern u8 D_80078C90[];
MemBlock *func_80011BF8(Heap *heap, u8 *start, s32 size);
extern u8 D_80050008[];
extern u8 D_8004E0B8[];
void func_800134FC(u8 *a, u8 *b);
void func_800270F0(u8 *a);
void func_80012E90(void);
void func_800272AC(s32 arg0);
extern s32 D_8004E0B4;
extern s32 D_8006E9F4;
s32 func_800320CC(void);
void func_800320DC(s32 arg0);
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
extern s32 D_800675D0;

/* 0x28-byte entries starting at 0x800675D4; D_800675D0 is the count. */
typedef struct {
    u8 unk0[0x18];
    s32 size; /* 0x18 */
    u8 unk1C[0xC];
} SizeEntry;
extern SizeEntry D_800675D4[];
extern char D_8004CDAC[];
extern Heap D_8004E0F0;
extern u8 *D_8006E9FC;
extern u8 D_80055684[];
extern void *D_8005AA34;
extern void *D_8005AA24;
extern SeqSlot D_8006557C;
extern LoaderState D_80050628;
extern MusicEntry D_80058B88[];
extern VabSlot D_800644C0[6];
extern NodePool D_80056844;
extern NAS *D_8005AA2C;
extern void *D_8005AA28;            /* tail of the D_8005AA24 list */
extern void *D_8005AA30;            /* tail of the D_8005AA2C list */
extern s32 D_8005A5DC;
extern s32 D_8005B5BC[16][2];       /* entries 1..15 cleared by func_80026D9C */
extern s32 D_800515F8;
extern s32 D_800515FC;            /* first pad with flag 1 set, or -1 */
extern s32 D_80051600;            /* pads 0..n-1 all have flag 1 */
extern s32 D_80051604;            /* frames with no button down */
extern u8 D_80063A90[2][0x34];    /* PadInitMtap buffers */
extern PadState D_80051338[8];
extern void *D_80063BB4;          /* list walked by func_80028A00 (next at +4) */
extern s32 D_8004E104;            /* func_80011410: free bytes */
extern s32 D_8004E108;            /* func_80011410: free blocks */
extern s32 D_8004E110[];         /* func_80012D80: 8-byte entries */
extern s32 D_8004E10C;            /* func_80011410: largest free block, bytes */
extern s32 D_800654BC;            /* slots of D_800644C0 in use */
extern char D_800654CC[];
extern s32 D_8005A608;
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
extern s32 D_80064418[];       /* voice bit masks, by voice */
extern s32 D_800655B0;
extern s32 ASTCount;            /* entries in ASTTable */
extern ASTEntry *ASTTable;      /* the AST table in use */
extern s32 D_80058AC8;
extern SfxPool SfxVoicePool;
extern s8 SfxTable[][2]; /* by sound id; [1] == -1: no sound */
void func_8002B8F4(s32 *voice);
extern s32 D_800637B4;
extern u8 D_8006EA08;
void func_80034AEC(void);
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
extern s32 D_800335BC;
extern u32 D_80063AF8[];
extern s32 D_800643E8;
extern s32 D_800643EC;
extern s32 D_800643F0;
void *func_800112A4(void *base, s32 offset, Heap *arg2);
void func_80014540(u8 *arg0, RECT *rect);
void func_8002ACA0(s32 id, s32 arg1);
extern GfxState D_80056998;
extern u16 D_800569DC;
extern s32 D_8005B690; /* frame counter: its low bit picks the frame buffer */
void func_8001A6D4(void *arg0, u32 hidden);
s32 func_8001A0D8(void *arg0, u32 flags);
void *func_80019094(void *arg0, s32 arg1);
s32 func_8001AF2C(void *arg0);

typedef struct {
    u8 unk0[0x10];
    s32 off; /* 0x10: byte offset from this field to a block */
    s32 runs; /* 0x14: see MorphModel */
    u8 unk18[0x1C];
} ModelEntry;

typedef struct {
    u8 unk0[0x24];
    ModelEntry entries[1]; /* 0x24, stride 0x34 */
} ModelSet;
void func_800193A8(POLY_GT3 *poly, SVECTOR *v, u8 *runs);
extern u8 D_80053694[];
typedef struct Words3 {
    s32 w0;
    s32 w4;
    s32 w8;
} Words3;

void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl);
extern EXTmotion D_8005A9CC;
extern subengine D_8005A830;
void FreeExtension(void *ext);
s32 func_80032F3C(s32);         /* abs or labs (LIBC2: identical objects) */
s32 func_80025E00(EXTengine *eng, s32 y);
s16 func_800251A8(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
s16 func_80025534(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
extern s32 (*D_8005AA94)(EXTengine *eng); /* optional hook: replaces func_80025E00 */
extern s32 D_8005B5B8;
extern void (*D_800643F8)();
extern NAS *NASActiveTail;
extern s32 *D_80056850;
extern s32 *D_80056858;
extern NAS *NASActiveHead;
extern s32 D_800654AC;
extern s32 D_800654B0;
extern s32 D_800654B4;
extern s32 D_8004E0D8;
extern s32 D_800590CC;
extern subengine *D_80058ACC;
extern s32 D_80058B60;
extern s32 D_80058B68;
extern s32 D_80058B6C;
void func_8001DFF8();
extern s8 D_8005A63C;
extern void *D_8005699C;
extern void *D_8005AA38;
extern void *D_80056864;
extern u8 D_8005B790[];
extern s32 D_800637A8;
extern u8 *D_80058B7C;
extern s32 D_80058B80;
extern s32 D_80058B84;

extern NAS *NASFreeList;
extern u16 D_8005AA10;            /* first pool slot func_80026D08 frees */
extern u16 D_8005AA12;            /* pool slot count */
extern u16 D_8005AA14;            /* pool slots in use (func_80026FCC kills them all) */
extern NAS *D_8005AA1C;           /* NAS pool */
extern NAS NASTemplate;            /* copied over each spawned NAS */
extern EXTengine EngineTemplate;   /* copied over each new engine */
extern s16 AtanTable[];            /* get_angle */
extern u16 D_8005A644;
extern s32 D_800512A4;
extern s32 D_80051638[2];

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

void func_800154EC(u16 *arg0, s32 arg1);

void func_8002775C(u8 *arg0);

void func_800154C0(u16 *arg0, s32 arg1);

void *func_8001CFE8(MorphModel *model, void *arg1, s32 arg2);

s32 func_80028BC8(u8 * *arg0);

void func_8002D0C8(void);

void func_8002AABC(void);

s32 Random(s32 n);
void *FindExtension(NAS *nas, s32 id);
NAS *SpawnNASById(s32 x, s32 y, s32 z, u16 id, u16 flags);

void StopSfx(_sfx_handler *voice);

_sfx_handler *PlaySfx(s32 id, _sfx_handler **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags);

s32 KillNAS(NAS *nas);

s32 get_angle(s32 x, s32 z);

FlagRecord *func_80025A48(EXTengine *eng, u32 flag);

s16 func_800250B0(NAS *nas);

void func_8002C44C(void);

void func_80025FAC(EXTanimator *anim, s16 n);

void func_800110A8(void *ptr, Heap *heap);

u8 *func_80016AD4(s32 *arg0, u8 *rec);

void func_8002C488(s32 arg0, s32 type);

void func_8002D108(void);

u8 *func_800160C4(u8 *arg0, s32 i);

void func_8002BD0C(s32 arg0);

void func_80021330(u8 *arg0, s32 *arg1);

s32 func_8002A650(u8 *arg0);

void func_8002BD68(void);

void func_8002BBB4(void);

void func_8001E854(void *arg0);

void func_8001E8F0(void *arg0);

s32 func_80011410(Heap *heap);

void func_8002B9A4(void);

s32 func_8001E598(u8 *arg0);

void func_800101E0(void);

subengine *func_8001DCD4(s32 i, s32 val);

s32 func_80016274(s32 *list, u8 *arg1);

s32 func_800280C0(u8 *src, u8 *dst, s32 n);

void func_80013C78(void);

s32 func_8001538C(u32 x);

s32 func_80015284(s32 a, s32 b, s32 t);

void *func_80015AB0(s32 *arg0, u8 *rec);

void *func_800183B0(void);

void func_80018388(void *data);

void func_800183DC(void);

s32 func_80026A28(void);

void func_80013CFC(s32 pad, u8 *out);

void func_80026B94(NAS *list, NAS *nas);

void func_8001682C(s32 *arg0);

void func_8002C158(u8 *arg0);

void func_8002C18C(u8 *arg0);

u8 *func_80021E44(OffsetTable *tbl, s32 i);

void func_8002A830(u8 *src, u8 *out, s32 len);

void *func_80021EEC(void);

void *func_80028A00(void);

void func_8002694C(NAS *nas, s32 bits);

u8 *func_80015A78(s32 i, u8 *rec);

void func_80015518(u8 *p, s32 n);

void func_800228C4(_sfx_handler *voice);

void func_80027F00(void);

void func_8002C0E0(void);

void func_8002C11C(void);

void func_80015554(u16 *p, s32 n);

void func_800229FC(s32 i);

void func_80016CA0(u8 *rec);

void func_80013238(void);

NAS *SpawnNASFromRecord(NASTrigger *trig);

void func_8001636C(u8 *arg0, s32 arg1);

s32 func_80016450(u8 *arg0, s32 arg1);

void func_8001C298(EXTengine *eng);

void *func_80012E28(void);

subengine *func_8001C34C(s32 n, subengine *sub);

void func_80028158(void);

void func_80026FCC(void);

void func_80026D9C(void);

void func_8002120C(u8 *arg0);

void func_8001D5D0(void);

s32 func_8002D094(void);

u8 *func_80021E60(u8 *arg0, s32 arg1);

void func_80022984(void);

void func_80016E5C(u8 *arg0, u8 *arg1);

s32 func_8001DD20(s32 *arg0);

void func_8002C060(void);

u8 *func_80021E98(u8 *arg0);

void *func_80011828(void *ptr, s32 bytes);

void func_800138A4(void);

s32 func_800133C8(s32 arg0, s32 arg1, u8 *arg2, s32 arg3);

s32 func_8002D360(u8 *arg0);

u8 *func_8001316C(u8 *arg0);

s32 func_8001361C(s32 *arg0, u8 *arg1);

MemBlock *func_80011BC0(MemBlock *blk);

s32 func_800135E4(s32 *arg0, u8 *arg1);

MemBlock *func_80011878(MemBlock *blk, s32 words);

s32 func_80015304(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_8002C248(s32 arg0);

/* Per-note pitch data: a signed note and a signed fine nibble in the top of +1. */
typedef struct {
    s8 note;
    u8 fine; /* 0x01: high nibble is a signed fine value */
} PitchInfo;

typedef struct {
    s16 voice;       /* 0x00 */
    PitchInfo *info; /* 0x04 */
} PitchVoice;

IdEntry *func_800263D8(IdEntry *p, s32 id);

s32 func_80015984(s32 i, EntryBlock *blk);

void func_80011578(u8 *arg0, Heap *heap, s32 arg2);

void func_8001702C(SlotList *arg0);

void func_80027A30(s16 vol);

void func_80021940(s32 arg0, u8 *arg1, s32 arg2);

s32 func_8002D178(void);

void func_80010354(void);

MemBlock *func_80011F74(Heap *heap);

void func_80013C24(void);

void func_80013CA4(void);

s32 func_800159C4(s32 arg0, u8 *arg1);

void func_80016F54(u8 *arg0);

void func_8001CE00(s32 arg0, ModelSet *set, s32 arg2);

void func_80025F44(EXTengine *eng);

void func_80026858(void *owner, s32 size, s32 id, Words3 *tmpl);

void func_80028BF0(void);

void func_80028C94(void);

void func_8002B4D8(PitchVoice *arg0, s32 arg1);

void func_8002C090(SeqSlot *slot);

void func_8001CEB4(u16 arg0, u8 *arg1, s32 arg2);

u8 *func_80015A18(s32 arg0, u8 *arg1);

void func_80021238(u8 *arg0, s32 *arg1);

void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);

void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
s32 func_800152A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_80021708(u8 *arg0, u8 *arg1, s32 arg2);

void func_800170D4(void);

typedef struct {
    s32 a[3]; /* 0x00 */
    s32 mode; /* 0x0C */
    s32 b[3]; /* 0x10 */
} PosEntry;
typedef struct {
    u8 unk0[0x70];
    s32 y; /* 0x70 */
    s32 z; /* 0x74 */
    s32 x; /* 0x78 */
    s32 idx; /* 0x7C */
} PosDst;
void func_8001A82C(PosDst *arg0, s32 arg1, s32 arg2);
extern PosEntry D_8005689C[];
void func_80010158(void);

void func_80010278(void);

void func_800102F8(void);

void func_80010394(void);

void func_80010410(void);

void func_80011AE8(MemBlock *blk, Heap *heap);

MemBlock *func_80011C98(MemBlock *arg0);

MemBlock *func_80011EF0(Heap *heap);

void func_80012CD4(void *arg0, s32 *arg1);

s32 func_80012D80(s32 *arg0, s32 *arg1);

void func_80012FA4(LoadNode *arg0);

s32 func_800133FC(s32 *arg0);

s32 func_80013488(s32 arg0, s32 arg1, s32 arg2);

void func_800150E8(char *dst, s32 val, s32 div);

char *func_8001517C(char *arg0, s32 arg1, s32 arg2);

s32 func_80015210(s32 a, s32 b, s32 t);

u8 *func_800167A8(u8 *arg0, u8 *arg1);

void func_8001830C(void);

s32 *func_800184F0(s32 arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_8001C3D0(u8 *arg0);

void func_8001C448(PosDst *arg0, s32 arg1, s32 arg2);

void func_8001C584(PosDst *arg0, s32 arg1);

s32 func_8001CC10(void *arg0, s32 arg1);

void func_8001D640(s32 n);

void func_8001DD50(void *arg0, void *arg1);

void func_8001DEF4(s32 *arg0, s32 *arg1);

KeyEntry *func_8001E41C(KeyBlock *arg0, s32 arg1);

s32 func_8001E51C(s32 arg0);

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

s16 func_80020C4C(s32 arg0);

s32 func_800213A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

s32 *func_80021878(u8 *arg0, s32 arg1);

s32 *func_800218DC(u8 *arg0, s32 arg1);

s32 *func_800219B8(u8 *arg0, s32 arg1);

u8 *func_80021DD4(u8 *arg0, s32 arg1, s32 arg2);

void func_80022070(void);

_sfx_handler *func_80022924(void);

void func_80024214(s32 *a, s32 *b);

void func_80026ECC(void);

void func_80026F48(void);

s32 func_80027944(void);

s32 func_80028994(s32 a, s32 b, s32 c);

s32 func_8002A2EC(s32 r, s32 g, s32 b);

void func_8002B52C(s16 *arg0, s32 arg1, s32 arg2);

void func_8002BA44(void);

void func_8002BCB0(s32 *arg0);

void func_8002C1C0(s32 arg0, s32 arg1);

SizeEntry *func_8002D1D4(void);

s32 func_8002E584(u8 *arg0);

typedef struct VelObj {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ u16 angle;
    /* 0x16 */ u8 pad16[0x3C];
    /* 0x52 */ s16 dirty;
} VelObj;

typedef struct VelParams {
    /* 0x00 */ u8 pad00[0x18];
    /* 0x18 */ s32 limit;
    /* 0x1C */ u8 pad1C[0x24];
    /* 0x40 */ s32 addX;
    /* 0x44 */ s32 mulX;
    /* 0x48 */ s32 addY;
    /* 0x4C */ s32 mulY;
    /* 0x50 */ s32 addZ;
    /* 0x54 */ s32 mulZ;
    /* 0x58 */ u8 pad58[0x10];
    /* 0x68 */ u16 dAngle;
} VelParams;

typedef struct VelState {
    /* 0x00 */ s32 frame;
    /* 0x04 */ u8 pad04[4];
    /* 0x08 */ s32 vx;
    /* 0x0C */ s32 vy;
    /* 0x10 */ s32 vz;
} VelState;

s32 func_80013334(s32 arg0, s32 arg1, u8 * *arg2, s32 arg3);

void func_8002147C(VelObj *obj, VelParams *p, VelState *s);

#endif /* BASH_MAIN_H */
