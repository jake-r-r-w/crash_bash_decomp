#ifndef BASH_MAIN_H
#define BASH_MAIN_H

/* Functions and data defined in main.exe's game code. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/main.c */

#include "common.h"
#include "bash/nas.h"

typedef struct {
    s32 tracksOff; /* 0x00 */
    s32 subOff;    /* 0x04 */
} AnimData;

typedef struct {
    u8 unk0[8];
    u8 *table;      /* 0x08 */
    u8 unkC[0xC];
    AnimData *data; /* 0x18 */
} AnimOwner;

/* An AnimOwner table entry (0x38 bytes) scrolled by func_80028F28. */
typedef struct {
    u8 unk0[0x1C];
    u16 w;          /* 0x1C */
    s16 h;          /* 0x1E */
    s16 *src;       /* 0x20: x, y */
} AnimEntry;

/* A scrolling track (func_80029100's sub list, 16 bytes each). */
typedef struct {
    s32 idx;        /* 0x00: entry in AnimOwner.table */
    s32 speed;      /* 0x04 */
    s32 pos;        /* 0x08: 24.8 */
    s16 *dst;       /* 0x0C: x, y */
} AnimScroll;

void func_80028F28(AnimScroll *, AnimOwner *);

/* The same 0x38-byte entry as func_80028D40 uploads it to VRAM. */
typedef struct AnimTex {
    u16 mode;       /* 0x00: 0 = 4-bit, else 8-bit */
    u16 clut;       /* 0x02: 0x7FFF = none */
    u8 *src;        /* 0x04 */
    u8 unk8[8];
    u8 du;          /* 0x10 */
    u8 dv;          /* 0x11 */
    u16 flags;      /* 0x12 */
    s32 frames;     /* 0x14 */
    RECT rect;      /* 0x18 */
    struct FreeHdr *pos; /* 0x20 */
    u16 tpage;      /* 0x24 */
    u16 clutId;     /* 0x26 */
    u16 uv0;        /* 0x28 */
    u16 uv1;        /* 0x2A */
    u16 uv2;        /* 0x2C */
    u16 uv3;        /* 0x2E */
    u8 *pix;        /* 0x30 */
    u8 unk34[4];
} AnimTex;

typedef struct {
    u16 unk0;
    u16 id;         /* 0x02 */
    u8 unk4[8];
} AnimClut;

typedef struct {
    u8 unk0[0xC];
    AnimClut *cluts; /* 0x0C */
} AnimTexSet;

typedef union {
    u16 h;
    u8 b[2];
    struct { u16 u : 8; u16 v : 8; } f;
} TexUV;

extern s32 D_8005A640;

/* A viewpoint: its position (+0xC) is subtracted from world positions before
 * they go through the GTE (func_8002B6D0, func_8002B760). */
typedef struct {
    s32 unk0[3];
    VECTOR pos; /* 0x0C */
} View;

typedef struct {
    s32 v[3];
} CamVec3;

/* A 0x20-byte block func_8001FE80 copies whole. */
typedef struct {
    s32 v[8];
} Blk32;

/* A 0x28-byte camera key in the track func_8001EDFC walks; it interpolates
 * from one key to the next. */
typedef struct CamKey {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 len;   /* 0 ends the track */
    /* 0x08 */ CamVec3 at;
    /* 0x14 */ CamVec3 eye;
    /* 0x20 */ u8 unk20[8];
} CamKey;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ CamVec3 pos;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x38];
    /* 0x54 */ s16 rot[3];
} CamState;

s32 func_80015304(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* out = a + (b - a) * t / len, per component. */
static inline void CamVec3_Lerp(CamVec3 *out, CamVec3 *a, CamVec3 *b, s32 len, s32 t) {
    out->v[0] = func_80015304(a->v[0], b->v[0], len, t);
    out->v[1] = func_80015304(a->v[1], b->v[1], len, t);
    out->v[2] = func_80015304(a->v[2], b->v[2], len, t);
}

/* The key of the camera track that covers frame v (or the terminating one). */
static inline CamKey *CamTrack_FindKey(u8 *track, s32 v) {
    CamKey *key = (CamKey *)(track + 0x1C);

    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* A 0x4C-byte animation key of the track func_8001F0D4 walks (at track+0x30). */
typedef struct AnimKey {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 len;   /* 0 ends the track */
    /* 0x08 */ CamVec3 pos;
    /* 0x14 */ s32 unk14[3];
    /* 0x20 */ s32 rot[4];
    /* 0x30 */ s32 unk30[3];
    /* 0x3C */ CamVec3 scale;
    /* 0x48 */ s32 alpha;
} AnimKey;

typedef struct {
    s32 v[4];
} AnimRot;

static inline AnimKey *AnimTrack_FindKey(AnimKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

typedef struct {
    s32 v[4];
} ObjQuat;

/* A 0x50-byte key of the object track func_8001EAA4 walks (from track + 0x24);
 * it interpolates from one key to the next. */
typedef struct ObjKey {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 len; /* 0 ends the track */
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ CamVec3 pos;
    /* 0x18 */ s32 unk18[3];
    /* 0x24 */ ObjQuat rot;
    /* 0x34 */ s32 unk34[3];
    /* 0x40 */ CamVec3 scale;
    /* 0x4C */ s32 unk4C;
} ObjKey;

/* The key of the object track that covers frame v (or the terminating one). */
static inline ObjKey *ObjTrack_FindKey(ObjKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* Frame range of an animation track (at track+0x14). */
typedef struct {
    /* 0x00 */ u16 base;
    /* 0x04 */ s32 lo;
    /* 0x08 */ s32 hi;
    /* 0x0C */ s32 off;
    /* 0x10 */ s32 clamp;
} AnimRange;

/* Frame t (16.16) of the range, clamped or looped, relative to its first frame. */
static inline s32 AnimRange_Frame(AnimRange *range, s32 t) {
    s32 frame;

    t -= range->off << 16;
    if (t < 0) {
        frame = range->lo << 16;
    } else if (range->clamp != 0) {
        if (t < (range->hi - range->lo) << 16) {
            frame = t + (range->lo << 16);
        } else {
            frame = (range->hi - 1) << 16;
        }
    } else {
        frame = t % ((range->hi - range->lo + 1) << 16);
        frame += range->lo << 16;
        if ((range->hi - 1) << 16 < frame) {
            frame = (range->hi - 1) << 16;
        }
    }
    frame -= range->lo << 16;
    return frame;
}

/* The pose func_8001F0D4 writes (at obj+8). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ CamVec3 pos;
    /* 0x10 */ s32 rot[4];
    /* 0x20 */ CamVec3 scale;
    /* 0x2C */ u8 unk2C[0x24];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54[0x1C];
    /* 0x70 */ s32 frame;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ s16 unk76;
} AnimPose;

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);


/* A 0x38-byte record in the table at +8 of the block func_8002A9AC frees
 * (count at +0). */
typedef struct TexAnimEntry {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ s32 handle; /* freed with func_80028AE4 */
    /* 0x24 */ u8 unk24[0x10];
    /* 0x34 */ s32 time;   /* frame stamp, compared to D_8005B690 */
} TexAnimEntry;

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

/* A 0x28-byte primitive slot of func_80017B08: a textured triangle, or (run flag
 * bit 0) a draw-mode packet followed by an untextured gouraud triangle. */
typedef union {
    POLY_GT3 gt;
    struct {
        DR_TPAGE tp;
        POLY_G3 g;
    } g;
} FacePrim;

/* Face data func_80017B08 reads: a run list (as MorphModel's: pairs of a flags
 * byte and a count, ending in a count of 0xFF) and one u16 colour index per face
 * (bit 15: semi-transparent, bits 13-14: blend mode). */
typedef struct {
    u8 unk0[0x14];
    s32 runs;  /* 0x14: byte offset from this field */
    u8 unk18[8];
    s32 table; /* 0x20: byte offset from this field */
} FaceRuns;

/* The part of a 0x38-byte texture entry from +0x18 (func_80017EE8). */
typedef struct {
    u8 unk0[0xC];
    u16 tpage; /* 0x0C */
    u16 clut;  /* 0x0E */
    u16 uv;    /* 0x10: ORed into each vertex's uv */
} GfxTexInfo;

typedef struct {
    u8 unk0[0x18];
    GfxTexInfo info; /* 0x18 */
    u8 unk2A[0xE];
} GfxTexEntry;

typedef struct {
    u8 unk0[2];
    u16 clut; /* 0x02 */
    u8 unk4[8];
} GfxClutEntry;

/* At +0x10 of GfxModel (func_80017EE8). */
typedef struct {
    s32 count;            /* 0x00: entries in tex */
    u8 unk4[4];
    GfxTexEntry *tex;     /* 0x08 */
    GfxClutEntry *cluts;  /* 0x0C */
} GfxTexSet;

/* A block with a table at a byte offset from its field at +0x24. */
typedef struct {
    u8 unk0[0x24];
    s32 uvs; /* 0x24: byte offset from this field */
} UvTable;

typedef struct GfxModel {
    u8 unk0[0xC];
    RelTable *colors; /* 0x0C: u32 colours, indexed by a face's 13-bit indices */
    GfxTexSet texs;   /* 0x10 */
} GfxModel;

/* Face data func_80017EE8 reads: per face a uv index (0x18), a run-length
 * texture index list (0x1C: bits 9-14 repeat count, bit 15 last texture, bits 0-8
 * texture), and a blend index (0x20). */
typedef struct {
    u8 unk0[0x14];
    s32 runs;  /* 0x14: byte offset from this field */
    s32 uvIdx; /* 0x18 */
    s32 texs;  /* 0x1C */
    s32 table; /* 0x20 */
} TexFaceRuns;

/* An object drawn as one textured quad (func_8001AA48, func_8001C3D0,
 * func_8001C378): one primitive per frame buffer. */
typedef struct {
    POLY_GT4 *prim; /* from func_800183B0, freed with func_80018388 */
    u8 unk4[8];
} ModelObjSlot;

typedef struct {
    u32 flags;               /* 0x00: 0x8000 visible */
    u8 unk4[0x68];
    struct GfxModel *model;  /* 0x6C */
    u8 unk70[4];
    u16 index;               /* 0x74: passed to func_80015A18 */
    u8 unk76[0x1A];
    ModelObjSlot slots[2];   /* 0x90: indexed by D_8005B690 & 1 */
} ModelObj;

/* At +0x54 of GfxCamera; func_8001ACC0 takes a pointer to it. */
typedef struct {
    u8 unk0[0x10];
    VECTOR scale; /* 0x10: diagonal of the screen-aligned matrix */
} GfxCamScale;

/* The camera GfxState.cam points at (func_8001D894). */
typedef struct GfxCamera {
    u8 unk0[0xC];
    s32 x, y, z;    /* 0x0C */
    u8 unk18[0x3C];
    GfxCamScale unk54; /* 0x54 */
    MATRIX view;    /* 0x74 */
    u8 unk94[4];
    MATRIX rot;     /* 0x98: rotation for screen-aligned objects */
} GfxCamera;

/* The transform block func_8001ACC0 reads (at +0xC or +0x1C of its owner). */
typedef struct {
    u32 flags;      /* 0x00: 0x10000000 screen space, 0x20000000 screen-aligned */
    s32 x, y, z;    /* 0x04 */
    u8 unk10[0x10]; /* 0x10: passed to func_800142D4 (0x20 bytes) */
    s32 sx, sy, sz; /* 0x20: scale for screen-aligned (func_8001965C) */
    u8 unk2C[4];
    MATRIX rot;     /* 0x30 */
    u8 unk50;       /* 0x50: nonzero: scale the screen-aligned matrix */
    u8 unk51;       /* 0x51: 4: no transform */
    u16 unk52;      /* 0x52: nonzero: rot needs rebuilding */
} GfxXform;

/* The block at GfxState+0x28 (passed to func_8001965C). */
typedef struct {
    s32 x, y;  /* 0x0: screen offset */
    u16 otz;   /* 0x8: becomes GfxState.otzAdd */
} GfxScreen;

/* Graphics state at 0x80056998 (placeholder name; 161 references). Only the
 * fields func_8001CD04 and func_8001D048 use are known: two pointers it aims
 * at buffers inside the struct itself, a block at +0x28 it passes to
 * func_8001965C, and the ordering table that primitives are added to. */
typedef struct {
    struct GfxModel *unk0;
    u8 unk4[4];
    s32 *farColor; /* 0x08: r, g, b for gte_ldfcdir */
    s32 dp;        /* 0x0C: depth-cue factor for gte_lddp */
    void *viewport; /* 0x10: GfxViewport (func_80018B08) */
    s32 modelDp;   /* 0x14: dp for ModelObj quads (func_8001AA48) */
    u8 unk18[4];
    s16 clipX1;    /* 0x1C: viewport clip right/bottom on screen */
    s16 clipY1;
    s32 ofx;   /* 0x20: geometry offset x (scaled by the screen width / 640) */
    s32 ofy;   /* 0x24: geometry offset y (halved) */
    u8 unk28[0x18];
    u32 *ot;   /* 0x40 */
    s16 otzAdd; /* 0x44: added to the average Z before it is halved */
    s16 otLen;  /* 0x46: entries in ot */
    struct GfxCamera *cam; /* 0x48 */
    u8 unk4C[0x38];
    u8 *unk84; /* 0x84: = unkCC */
    u8 *unk88; /* 0x88: = unk10C */
    MATRIX unk8C; /* 0x8C: set to identity by func_8001D5D0 */
    MATRIX unkAC; /* 0xAC: object's local-to-screen matrix (func_8001ACC0) */
    u8 unkCC[0x40];
    u8 unk10C[0x40];
} GfxState;

/* A 0x1C-byte entry of a sorted table at +0x4C of a block (func_8001E41C). */
typedef struct {
    SLONG_XYZ pos; /* 0x00 */
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

/* Loads GTE vector 0 from memory through $t0/$t1 (lw + mtc2), where Sony's
 * gte_ldv0 uses lwc2. */
#define gte_ldv0_t(r0) __asm__ volatile (				"lw	$t0, %0;"					"lw	$t1, %1;"					"mtc2	$t0, $0;"					"mtc2	$t1, $1"					:								: "m"(((s32 *)(r0))[0]), "m"(((s32 *)(r0))[1])	: "$8", "$9" )

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

/* Analog/edge view of a PadState (func_800136BC). */
typedef struct PadStick {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 buttons;
    /* 0x08 */ s32 rx;
    /* 0x0C */ s32 ry;
    /* 0x10 */ s32 lx;
    /* 0x14 */ s32 ly;
    /* 0x18 */ u8 unk18[0x2C - 0x18];
    /* 0x2C */ s32 forced;
    /* 0x30 */ s32 forcedButtons;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 angle;
    /* 0x3C */ s32 dist;
    /* 0x40 */ s32 held;
    /* 0x44 */ s32 held2;
    /* 0x48 */ s32 pressed;
    /* 0x4C */ s32 dirHeld;
    /* 0x50 */ s32 dirHeld2;
    /* 0x54 */ s32 dirPressed;
} PadStick;

/* Loaded sound data and the sequence opened from it (D_8006557C). */
typedef struct SeqSlot {
    /* 0x00 */ s32 data;                 /* heap block, freed by func_8002C44C */
    /* 0x04 */ s32 seq;                  /* SsSeq access number, or -1 */
    /* 0x08 */ s32 p8;                   /* data + header offset (func_8002C2C4) */
    /* 0x0C */ s32 pC;
    /* 0x10 */ s32 p10;
    /* 0x14 */ s32 p14;
    /* 0x18 */ s32 p18;
    /* 0x1C */ s32 unk1C;                /* set to -1 on close */
} SeqSlot;

/* Set both channels of slot's sequence volume (func_8002BEA8). */
static inline void SeqSlot_SetVol(SeqSlot *slot, s32 vol) {
    SsSeqSetVol((s16)slot->seq, (s16)vol, (s16)vol);
}

/* The offsets at the start of the loaded sound data. */
typedef struct {
    s32 body;
    s32 head;
    s32 o8;
    s32 oC;
    s32 o10;
    s32 o14;
    s32 o18;
} SeqHdr;

/* D_80058B88: what func_800229FC passes to func_8002C488 for an index. */
typedef struct MusicEntry {
    /* 0x00 */ s32 file;
    /* 0x04 */ s16 reverb;               /* SsUtSetReverbType */
    /* 0x06 */ u8 unk6[2];
    /* 0x08 */ s32 unk8;                 /* func_80022140: music file */
    /* 0x0C */ s32 unkC;                 /* func_80022140: alt music file (flag 0x80) */
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

/* A game mode (func_8001E610): called on entry, every frame and on exit;
 * heapStart non-NULL resets the heap to start there. */
typedef struct GameModeDef {
    /* 0x00 */ void (*init)(void);
    /* 0x04 */ void (*update)(void);
    /* 0x08 */ void *unk8;
    /* 0x0C */ void (*exit)(void);
    /* 0x10 */ u8 *heapStart;
} GameModeDef;

/* The mode switcher (func_8001E610, D_8009F658). */
typedef struct ModeMgr {
    /* 0x00 */ GameModeDef *cur;
    /* 0x04 */ GameModeDef *next;
    /* 0x08 */ GameModeDef *prev;
    /* 0x0C */ s32 flags;   /* 1: skip exit, 2: skip init, 8: skip update, 0x10: clear after a switch */
} ModeMgr;

typedef struct LoadNode {
    /* 0x00 */ u8 *file;                 /* file table entry; NULL once cancelled mid-load (func_80012F04) */
    /* 0x04 */ s32 offset;               /* bytes into the file */
    /* 0x08 */ s32 size;                 /* bytes */
    /* 0x0C */ s32 mode;                 /* 1: func_80011544 after the load */
    /* 0x10 */ void (*fn)(u8 *, u8 *);   /* called with (buffer, arg) when done */
    /* 0x14 */ u8 *arg;
    /* 0x18 */ u8 *buf;                  /* preset buffer (func_800121F8) */
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

/* A node from the free list func_80012E28 hands out (func_80012E14 returns
 * it); ptr is the heap block it owns. */
typedef struct {
    /* 0x00 */ u8 *ptr;
} BlockRef;

/* An entry of the 0x18-byte table at +0x44 (+ offset at +0x44) of a file
 * loaded by func_80016234 (func_80015F60). */
typedef struct {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
    /* 0x08 */ u8 unk8[8];
    /* 0x10 */ s32 hash;   /* func_8001534C of its name */
    /* 0x14 */ BlockRef *block;
} FileEntry;

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
void func_80012F04(LoadNode *arg0);
s32 func_8001534C(u8 *s);
void func_8001828C(void);
s32 func_80013650(s32 arg0, void (*done)(), void *ctx);
s32 func_80012FFC(void);
s32 func_8001231C(void);
void func_80010AE8(Heap *arg0);
void func_8002BAE8(void);
void func_8002C894(void);
MemBlock *func_80011C5C(u32 words, MemBlock *blk);
s32 func_80013034(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6);
void func_80029560(u8 *arg0);
void func_8002A5A4(u8 *arg0, u8 *arg1);
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
void func_80019A60(s32 a, u8 *set, s32 c, void *d);
typedef struct RotScale {
    /* 0x00 */ SVECTOR rot;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ VECTOR scale;
    /* 0x20 */ MATRIX mtx;
    /* 0x40 */ u8 hasScale;
    /* 0x41 */ u8 order;
    /* 0x42 */ s16 dirty;
} RotScale;
void func_80013D54(SVECTOR *rot, MATRIX *m);
void func_800142D4(void *arg0);
PointList *func_8001562C(subengine *sub);
FlagPoint *func_80015780(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out);
void func_8001C2C0(u8 *arg0);
void func_8001C378(u8 *arg0);
void func_8001AA48(ModelObj *obj, void *arg1, s32 flags);
void func_800177F4(RelTable *face, POLY_GT4 *poly, s32 arg2, s32 mode);
void func_8001795C(u8 *face, POLY_GT4 *poly);
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
s32 func_8002C4B0(u8 arg0);
void func_8002D134(s32 arg0);
void func_8002D15C(void);
void func_8002D254(void);
void func_8002D274(void);
void func_8002D33C(u8 *arg0);

extern GteScratch D_800569CC;
extern SVECTOR D_80056AC8[];
typedef struct { s16 unk0; s16 unk2; } TrigEntry;
extern TrigEntry D_80068BD4[];
/* A 4.12 fixed-point quaternion (func_80020344 slerps two of them). */
typedef struct { s32 v[4]; } Quat;
static inline s32 QuatDot(Quat *a, Quat *b) {
    return (a->v[0] * b->v[0] + a->v[1] * b->v[1] + a->v[2] * b->v[2] + a->v[3] * b->v[3] + 0x800) >> 12;
}
extern s32 D_80064478[];
extern s32 D_8006E9F8;
extern u8 D_800527C4[];
extern u8 D_80052A54[];
extern u8 D_8004DDBC[];
extern u8 D_80050010[];
void func_8002D488(void);
void func_8002D4F4();
extern s32 D_80064414;
extern s32 D_800569AC;
extern s32 D_8006559C;
extern s32 D_800655A4;
extern s32 D_800655A0;
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
extern VTable8 *D_8005B648;
extern s32 D_8004E0E0;
extern s32 D_8004E0E4;
void func_80010488(Heap *heap);
void func_80017388(void);
extern u8 D_80078C90[];
MemBlock *func_80011BF8(Heap *heap, u8 *start, s32 size);
extern u8 D_80050008[];
extern u8 D_8004E0B8[];
s32 func_800134FC(u8 *a, u8 *b);
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
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    void (*unkC)(u8 *, u8 *);
} D_80058B00_Entry;
extern D_80058B00_Entry D_80058B00[];
extern s32 D_8005A5D8;
extern s32 D_800655A8;
extern s32 D_800655B8;
extern s32 D_800655C0;
extern u8 D_800655C8[];
/* The memory card save file header at D_800655C8 (PsyQ _CARD layout). */
typedef struct {
    u32 magic : 16;  /* 0x00 "SC" */
    u32 frames : 4;  /* 0x02 icon frames */
    u32 type : 4;
    u32 blocks : 8;  /* 0x03 */
    s16 title[32];   /* 0x04 Shift-JIS */
} McHeader;
extern char *D_800655BC;
extern u8 D_80065648[]; /* the header's icon frames (D_800655C8 + 0x80) */
extern u8 D_8004CD8C[];
s32 func_80010000(u8 *arg0);
s32 func_8002C4B0(u8 arg0);

/* Writes up to 32 Shift-JIS characters of src (ASCII) to dst, padding with
 * full-width spaces (func_8002C5BC). */
static inline void McSetTitle(u8 *src, s16 *dst) {
    s16 fill;
    s32 i;
    s32 n;
    s32 done;
    u8 c;
    s32 r;
    s32 r2;

    i = 0;
    done = 0;
    n = 0;
    r2 = func_8002C4B0(0x20);
    fill = ((r2 & 0xFF) << 8) | ((u32)(func_8002C4B0(0x20) & 0xFFFF) >> 8);
    do {
        if (done == 0) {
            c = src[i];
            i++;
            if (c >= 0x20U) {
                r = func_8002C4B0(c);
                dst[n] = ((r & 0xFF) << 8) | ((u32)(func_8002C4B0(c) & 0xFFFF) >> 8);
            } else {
                done = 1;
            }
            if (done != 0) {
                goto fillit;
            }
        } else {
fillit:
            dst[n] = fill;
        }
        n++;
    } while (n < 0x20);
}
typedef struct {
    u8 b[8];
    u8 pad[0x18C];
} D_800657C8_Slot;
extern D_800657C8_Slot D_800657C8[];
extern s32 D_80065E24;
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
extern s32 D_800637B0;
s32 func_80027790(u8 *a, s32 b, u8 *c, s32 d);
extern u8 *D_8006E9FC;
extern u8 D_80055684[];
extern void *D_8005AA34;
extern void *D_8005AA24;
extern SeqSlot D_8006557C;
extern LoaderState D_80050628;
extern char D_8004C83C[];
extern void func_80012840(u8 *, u8 *);
extern void func_80012678(u8 *, u8 *);
extern MusicEntry D_80058B88[];
extern VabSlot D_800644C0[6];
extern NodePool D_80056844;
extern NAS *D_8005AA2C;
extern NAS *D_8005AA9C;
extern void *D_8005AA28;            /* tail of the D_8005AA24 list */
extern void *D_8005AA30;            /* tail of the D_8005AA2C list */
extern s32 D_8005A5DC;
extern s32 D_8005B5BC[16][2];       /* entries 1..15 cleared by func_80026D9C */
extern s32 D_800515F8;
extern s32 D_800515FC;            /* first pad with flag 1 set, or -1 */
extern s32 D_80051600;            /* pads 0..n-1 all have flag 1 */
extern s32 D_80051604;            /* frames with no button down */
extern u8 D_80063A90[2][0x34];    /* PadInitMtap buffers */
extern u8 D_80063AC4[0x34];       /* D_80063A90[1] */
extern PadState D_80051338[8];
typedef struct FreeHdr { u16 f0; u16 f2; } FreeHdr;
typedef struct FreeNode {
    FreeHdr hdr;
    struct FreeNode *next;
    s32 f8;
    void *fC;
    void *f10;
    s32 f14;
} FreeNode;
typedef struct BufNode {
    s32 f0;
    struct BufNode *next;
    struct BufNode *prev;
    struct BufNode *parent;
    struct BufNode *child;
    struct BufNode **head;
} BufNode;

/* func_80028774's VRAM areas: a list head and the area's position; x = -1
 * ends the list. */
typedef struct {
    BufNode **head;
    FreeHdr hdr;
} VramArea;
extern u8 *D_800643F4; /* the BufNode pool (func_80028774) */
typedef struct FreePool {
    FreeNode *head;
    u16 w;
    u16 pad6;
    u16 h;
    u16 padA;
} FreePool;
extern FreeNode *D_80063B18;
extern u8 D_80063B58[];
extern u8 D_80063BA0[];
extern FreeNode *D_80063BAC;
extern FreeNode *D_80063BB0;
extern FreeHdr *func_800282F8(u8 *);
extern void *D_80063BB4;        /* list walked by func_80028A00 (next at +4) */
extern s32 D_8004E104;            /* func_80011410: free bytes */
extern s32 D_8004E108;            /* func_80011410: free blocks */
extern s32 D_8004E110[];         /* func_80012D80: 8-byte entries */
extern s32 D_8004E10C;            /* func_80011410: largest free block, bytes */
extern s32 D_800654BC;            /* slots of D_800644C0 in use */
extern char D_800654CC[];
extern s32 D_8005A608;
extern u8 D_8005A614[];
/* D_8005A614 as seen by func_80022140. */
typedef struct D5A614View {
    /* 0x00 */ s32 unk0;    /* func_800235BC: the arena's unk14 */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;    /* func_800235BC: a LevelInfo field by flags 0x80 */
    /* 0x0C */ u8 unkC[0x30 - 0xC];
    /* 0x30 */ u16 unk30;   /* bit 0 cleared on a mode change (func_8001E610) */
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x50 - 0x36];
    /* 0x50 */ u8 unk50[4];
} D5A614View;
extern u16 D_8004DDD0[];          /* func_80022140: 6-byte entries indexed by D_8005A614+0x34 */
/* D_8004DDD0 as 6-byte entries indexed by level (func_800235BC). */
typedef struct LevelInfo {
    /* 0x00 */ u16 world;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
} LevelInfo;
/* An arena definition (func_800235BC): 0x18 bytes, a table ends at id 999. */
typedef struct ArenaDef {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ void *unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} ArenaDef;
extern s32 D_8005AA70[11];
extern s32 D_8005AA50;
extern s32 D_8005AA54;
extern s32 D_8005A5E4;
extern char D_8004C8D0[];         /* "FATAL METRICS ERROR ..." */
extern u8 D_8004E9C0[];           /* func_80022140: passed (-0x10/-8/+0) to func_8002BD0C */
extern s32 D_80058F0C[][8];       /* func_80022140 */
extern s8 D_8005A604[];
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
extern s8 D_8005AEB4[];
extern u8 D_8005B0B4[];
extern u8 *D_8005B63C;
extern s16 D_8005AAB4[];
typedef struct GlyphPos {
    s16 v[3];
} GlyphPos;
extern s32 D_8006EA00;
extern u32 D_8006EA04;
extern s32 D_8005A7A0[];
typedef struct ExtBlk ExtBlk;
struct ExtBlk {
    ExtBlk *prev;
    ExtBlk *next;
    ExtBlk *fprev;
    ExtBlk *fnext;
    u32 size;
    EXT *owner;
    EXT ext;
};
extern ExtBlk *D_8005A7AC;
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
extern s32 D_800637B8;
void func_80034C4C(s32, s32);
extern u8 D_8006EA08;
extern CdlLOC D_8006EA10;
void func_80034AEC(void);
extern NASTrigger *D_8005AA48;
extern s32 D_8005AA4C;
extern void (*D_8005AA84)(void);
extern void *D_80058B78;
extern RECT D_8005B640;
extern void *D_80058B74;
extern void *D_80058AD0;
/* A map trigger record (func_80024AD8): position, then a word whose low 2
 * bits are the cast and the rest the offset from +0xC to its info. */
typedef struct MapTrigRec {
    /* 0x00 */ SLONG_XYZ pos;
    /* 0x0C */ s32 info;
} MapTrigRec;
typedef struct MapTrigInfo {
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 flags;   /* 0x4000: a camera/point, not a NAS */
    /* 0x04 */ s32 slot;
    /* 0x08 */ SLONG_XYZ rot;
} MapTrigInfo;
typedef struct MapPoint {
    /* 0x00 */ SLONG_XYZ pos;
    /* 0x0C */ SLONG_XYZ rot;
} MapPoint;
typedef struct U16Table {
    /* 0x00 */ u16 *data;
    /* 0x04 */ u16 count;
} U16Table;
extern void *D_8005A5E0;
extern void (*D_8005AA88)(NASTrigger *, s32);
extern MapPoint D_8005B2B8[32];
/* A button-combo sequence: steps[] are pad masks, 0-terminated. */
typedef struct ComboSeq {
    /* 0x00 */ void (*fn)(void);
    /* 0x04 */ s32 maxTime;
    /* 0x08 */ s32 step;
    /* 0x0C */ s32 startTime;
    /* 0x10 */ s32 steps[1];
} ComboSeq;

extern MATRIX D_80058AD4;
extern SVECTOR D_80058AF4;
extern s32 D_80051380;
extern u8 D_800512A8[]; /* ComboSeq table, 0x90 bytes per entry */
extern u8 D_800536CC[];
extern CdlLOC D_8006EA14;
void func_80016D98();
void func_80016188();
extern RECT *D_8005B698;
extern s32 D_8005B694;
extern s32 D_8005B78C;
extern s32 D_800335BC;
extern u32 D_80063AF8[];
extern s32 D_800643E8;
extern s32 D_800643EC;
extern s32 D_800643F0;
void *func_800112A4(void *base, s32 offset, Heap *arg2);
void func_80014540(u8 *arg0, RECT *rect);
void func_8002ACA0(s32 id, s32 arg1);
extern GfxState D_80056998;
extern VECTOR D_8005688C; /* screen-space translation (func_8001965C) */

/* Adds v to the translation GfxState.unk88 points at (func_8001965C). */
static inline void GfxAddTrans(VECTOR *v) {
    ((VECTOR *)D_80056998.unk88)->vx += v->vx;
    ((VECTOR *)D_80056998.unk88)->vy += v->vy;
    ((VECTOR *)D_80056998.unk88)->vz += v->vz;
}
/* The current draw buffer: primitives are allocated from the pointer at
 * +0x4008 (func_80029D28). */
extern u8 *D_8005B68C;

/* A screen offset pair, copied as one struct. */
typedef struct {
    s32 x, y;
} GfxOffset;

/* A viewport func_80018B08 makes current (coordinates in 640-wide units). */
typedef struct {
    s16 x0, y0, x1, y1;     /* 0x00: draw area */
    s16 cx0, cy0, cx1, cy1; /* 0x08: clip area (func_80014540) */
    s32 ofx, ofy;           /* 0x10: geometry offset */
    s32 otIdx;              /* 0x18: first ot entry in D_8005B68C */
    s32 otLen;              /* 0x1C */
    s32 unk20;
    u8 r, g, b, isbg;       /* 0x24: background */
} GfxViewport;

/* GfxViewport's background colour, copied as a block (func_8001DAF8). */
typedef struct {
    s8 r, g, b;
} GfxRgb8;

/* The scene func_8001DAF8 sets up (D_80058AD0 points at it). */
typedef struct {
    u8 unk0[0x10];
    GfxViewport *viewport; /* 0x10 */
    u8 unk14[8];
    u8 *objs;              /* 0x1C: list linked at +0x5C, callback at +0x54 */
} GfxScene;

/* Copies a scene's background into the current viewport, faded by modelDp
 * (func_8001DAF8). */
static inline void GfxSetSceneBg(GfxState *gs, GfxScene *scene) {
    GfxViewport *vp;

    vp = gs->viewport;
    vp->isbg = scene->viewport->isbg;
    if (gs->modelDp != 0) {
        vp->isbg = 1;
    }
    if (vp->isbg != 0) {
        if (gs->modelDp != 0) {
            vp->r = scene->viewport->r * (0x1000 - gs->modelDp) / 4096;
            vp->g = scene->viewport->g * (0x1000 - gs->modelDp) / 4096;
            vp->b = scene->viewport->b * (0x1000 - gs->modelDp) / 4096;
        } else {
            *(GfxRgb8 *)&vp->r = *(GfxRgb8 *)&scene->viewport->r;
        }
    }
}

/* Sets the clip area c (640-wide units) for cam. */
static inline void GfxSetClip(GfxState *gs, RECT *c, GfxCamera *cam) {
    RECT clip;

    clip.x = c->x * D_8005B698->w / 640;
    clip.y = c->y / 2;
    clip.w = (c->w - c->x) * D_8005B698->w / 640;
    clip.h = (c->h - c->y) / 2;
    gs->clipX1 = c->w * D_8005B698->w / 640;
    gs->clipY1 = c->h / 2;
    gs->cam = cam;
    func_80014540((u8 *)cam, &clip);
}

/* A sprite frame func_80029D28 draws as one POLY_FT4. */
typedef struct {
    u8 unk0[8];
    s16 w;       /* 0x08 */
    s16 h;       /* 0x0A */
    u8 unkC[4];
    u8 advance;  /* 0x10: returned scaled to the screen width */
    u8 unk11[0x13];
    u16 tpage;   /* 0x24 */
    u16 clut;    /* 0x26 */
    u16 uv0;     /* 0x28 */
    u16 uv1;     /* 0x2A */
    u16 uv2;     /* 0x2C */
    u16 uv3;     /* 0x2E */
} SpriteFrame;

/* A screen position as one word (x low, y high). */
typedef union {
    s32 w;
    struct {
        s16 x, y;
    } s;
} ScreenXY;

/* The same packed position built with bitfields (func_800248A0). */
typedef union {
    struct {
        s32 x : 16;
        s32 y : 16;
    } f;
    s32 w;
} SprPos;

/* Draws f at xy (640-wide units) as a POLY_FT4 in ot[otz]; color bit 31 makes
 * it semi-transparent, bits 29-30 its blend mode. Returns f's advance. */
s32 func_80029D28(SpriteFrame *f, s32 xy, s32 otz, u32 color);
/* func_80029D28 as a POLY_GT4 with one colour per vertex (c0 also sets the
 * semi-transparency and blend mode). */
s32 func_8002992C(SpriteFrame *f, s32 xy, s32 otz, u32 c0, u32 c1, u32 c2, u32 c3);
/* Draws f at the 3D point pos as a POLY_GT4 in the OT at D_8005B68C, c0 on
 * the top vertices and c1 on the bottom ones; returns f's advance. */
s32 func_80029F9C(SpriteFrame *f, GlyphPos *pos, s32 otz, u32 c0, u32 c1);

/* func_8002A2EC: fades r, g, b towards black by D_800569AC / 0x1000. */
static inline s32 FadeColor(s32 r, s32 g, s32 b) {
    s32 t;
    s32 d;

    d = D_800569AC;
    if (d != 0) {
        t = 0x1000 - d;
        r *= t;
        g *= t;
        b *= t;
        r >>= 12;
        r &= 0xFF;
        g >>= 12;
        g &= 0xFF;
        b >>= 12;
        b &= 0xFF;
    }
    return (r << 16) | (g << 8) | b;
}
/* FadeColor reading the fade from gs->modelDp (func_80029F9C). */
static inline s32 FadeColorGs(GfxState *gs, s32 r, s32 g, s32 b) {
    s32 t;

    if (gs->modelDp != 0) {
        t = 0x1000 - gs->modelDp;
        r *= t;
        g *= t;
        b *= t;
        r >>= 12;
        r &= 0xFF;
        g >>= 12;
        g &= 0xFF;
        b >>= 12;
        b &= 0xFF;
    }
    return (r << 16) | (g << 8) | b;
}
extern u16 D_800569DC;
extern s32 D_800569C0;

typedef struct {
    struct {
        s16 x;
        u16 y;
        u16 z;
        u16 pad;
    } v[4];
    s32 c[4];
} QuadVerts;

/* func_8001A0D8's input: four vertices and four colours (bytes r, g, b). */
typedef struct {
    SVECTOR v[4];
    u8 c[4][4];
} ColorQuad;

/* func_8001A0D8's primitive: a draw-mode word, then a POLY_G4 sharing the
 * tag (len 0xA, code 0x38 at +0xF). */
typedef struct {
    u32 tag;
    u32 mode;  /* 0x04: 0xE1000200 | abr bits */
    u32 unk8;
    struct {
        u32 c;
        s16 x, y;
    } v[4];    /* 0x0C */
} ModePolyG4;
extern s32 D_8005B690; /* frame counter: its low bit picks the frame buffer */
extern s32 D_8004E0E8; /* VSync ticks spent compacting the heap (func_80010488) */
extern s32 D_8004E0EC; /* words moved compacting the heap (func_80010488) */
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
extern s32 D_80056AC4; /* func_80019A60: the object's +0x8A */

/* func_80019A60: draw model entry e with vertices v. */
static inline void ModelEntry_Draw(u8 *e, s32 c, SVECTOR *v) {
    void *p = func_80019094(e, c);

    if (p != NULL) {
        func_800193A8(p, v, e + (*(s32 *)(e + 0x14) + 0x14));
    }
}
extern u8 D_80053694[];
typedef struct Words3 {
    s32 w0;
    s32 w4;
    s32 w8;
} Words3;

typedef struct SortKey {
    s32 unk0;
    s32 unk4;
} SortKey;
typedef struct SortNode {
    SortKey *key;      /* 0x00 */
    u8 pad4[0x18];
    struct SortNode *next; /* 0x1C */
    struct SortNode *prev; /* 0x20 */
} SortNode;
typedef struct SortList {
    u8 pad0[0xC];
    SortNode *head;    /* 0x0C */
    SortNode *tail;    /* 0x10 */
} SortList;

void *AllocExtension(void *owner, s32 size, s32 id, void *tmpl);
extern EXTmotion D_8005A9CC;
typedef struct Ext400 {
    s32 w[11];
} Ext400;
extern Ext400 D_8005A9A0;
typedef struct Ext1000 {
    s32 w[11];
} Ext1000;
extern Ext1000 D_8005A974;
extern subengine D_8005A830;
typedef struct Words4 {
    s32 w[4];
} Words4;
void FreeExtension(void *ext);
s32 abs(s32);         /* LIBC2 ABS_0.OBJ (labs in ABS_1.OBJ is identical); a call, not inlined: -fno-builtin */
s32 func_80025E00(EXTengine *eng, s32 y);
s16 func_800251A8(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
s16 func_80025534(NAS *nas, EXTengine *eng, subengine *sub, EXTanimator *anim);
/* A step of a kind 2 animator's Atable (func_80025534): cmd >= 0 is a frame
 * shown for arg ticks, 0x8000 | n is command n with operand arg. */
typedef struct AnimStep {
    s16 cmd;
    u16 arg;
} AnimStep;
extern s32 (*D_8005AA94)(EXTengine *eng); /* optional hook: replaces func_80025E00 */
extern s32 D_8005B5B8;
extern void (*D_800643F8)();
extern NAS *NASActiveTail;
extern s32 *D_80056850;
extern s32 *D_8005683C;            /* pool sizes (func_80017164) */
extern u8 *D_8005685C;             /* 0x58-byte nodes (func_80017164) */
extern u8 *D_80056840;             /* 0x54-byte nodes, free list in D_80056850 */
extern u8 *D_80056854;             /* 8-byte nodes, free list in D_80056858 */
extern s32 D_80056848;
void func_80017164(s32 *cfg);
typedef struct ListNode18938 {
    s32 f0;
    s32 f4;
    s32 **f8;
    s32 *fC;
    s32 **f10;
} ListNode18938;

/* Takes the node at the cursor fC of a list (func_800185F8's code). */
static inline s32 *ListNode18938_Take(ListNode18938 *l) {
    s32 *n;
    s32 **tail;

    if (l == NULL || (n = l->fC) == NULL) {
        return NULL;
    }
    tail = l->f10;
    l->fC = (s32 *)*n;
    if (tail != NULL) {
        *tail = (s32 *)*n;
        l->fC = (s32 *)*n;
        if (*n == 0) {
            l->f8 = tail;
        }
    } else {
        if (l->f8 == (s32 **)n) {
            l->f8 = NULL;
        }
        l->f4 = *n;
        l->fC = (s32 *)*n;
        *n = 0;
    }
    return n;
}
typedef struct {
    ListNode18938 b; /* 0x00 */
    ListNode18938 a; /* 0x14 */
} PrimListPair;
typedef struct PrimSet {
    struct PrimSet *next;  /* 0x00 */
    PrimListPair pairs[2]; /* 0x04 */
    u16 mode;              /* 0x54 */
    u16 dead;              /* 0x56 */
} PrimSet;
extern s32 *D_80056858;
extern NAS *NASActiveHead;
extern s32 D_800654AC;
extern s32 D_800654B0;
extern s32 D_800654B4;
extern s32 D_8004E0D8;
extern s32 D_8004E0D0; /* BCD of func_80010000's count */
extern s32 D_800590CC;
extern subengine *D_80058ACC;
extern s32 D_80058B60;
extern s32 D_80058B64;
extern s32 D_80058B68;
extern s32 D_80058B6C;
void func_8001DFF8();
extern s8 D_8005A63C;
extern void *D_8005699C;
extern void *D_8005AA38;
extern void *D_80056864;
extern s32 *D_80056860;
/* An ordering table and the primitive area drawn through it (0x400C bytes;
 * D_8005B790 holds two). */
typedef struct {
    u_long ot[0x1000];
    u8 *prim; /* 0x4000 */
    u8 *end;  /* 0x4004 */
    u8 *cur;  /* 0x4008 */
} OtBuf;

extern u8 D_8005B790[];
extern s32 D_800637A8;
extern u8 *D_80058B7C;
extern s32 D_80058B80;
extern s32 D_80058B84;

/* func_8001F828: per-particle state, 0x28 bytes */
typedef struct EmitPart {
    s32 age;
    s32 unk4;
    s32 vel[3];
    u8 pad14[0x14];
} EmitPart;
void func_8001F604(u8 *obj, EmitPart *part, u8 *def);

/* fade from `from` to `to` over [0,in), hold `to`, back to `from` over (out,end] */
static inline s32 EmitPart_Fade(s32 in, s32 out, s32 n, s32 end, s32 from, s32 to) {
    s32 ret;

    if (n < 0) {
        ret = from;
    } else if (n < in) {
        ret = func_80015304(from, to, in, n);
    } else if (out < n) {
        if (end < n) {
            ret = from;
        } else {
            ret = func_80015304(to, from, end - out, n - out);
        }
    } else {
        ret = to;
    }
    return ret;
}

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
typedef struct {
    u16 x;
    u16 y;
} ClutPos;
typedef struct {
    u16 w;
    u16 clut;
    ClutPos *pos;
    s32 off;
} ClutUpload;
ClutPos *func_80028420(void);

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
/* A text object func_8001A82C draws: a NAS-like header with the position at
 * +4 (x keeps func_800243A0's 0xC000 centring flags) and the text at +0x6C. */
typedef struct {
    s32 NASid;   /* 0x00: 0x8000 drawn, 0x10000000 2D (no GTE setup) */
    s32 x;       /* 0x04 */
    s32 y;       /* 0x08 */
    s32 z;       /* 0x0C */
    u8 unk10[0x5C];
    u8 *str;     /* 0x6C */
    u32 c0;      /* 0x70 */
    u32 c1;      /* 0x74 */
    s32 font;    /* 0x78: into D_8005B2B4 */
} TextNAS;
extern s16 D_800569C4;
void func_8001A82C(TextNAS *t, s32 arg1, s32 arg2);
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

void func_800201BC(u8 *arg0, GteSetup *arg1);

void func_80022070(void);

_sfx_handler *func_80022924(void);

void func_80024214(u8 *a, u8 *b);

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

u8 *func_80011498(u8 *arg0, s32 arg1, Heap *arg2);

void *func_800115D8(s32 arg0, void * *arg1, s32 arg2, s16 arg3);

void func_8002926C(void *arg0);

typedef struct {
    s32 f0;
    s32 f4;
    u8 *f8;
    u8 *fC;
    u8 *src;
    s32 f14;
} Func2A5A4Obj;

/* func_8002A4C0: count at +0, then 0x10-byte entries from +4. */
typedef struct {
    s32 idx;      /* 0x0: record index into the 0x38-byte table */
    s32 unk4;
    s32 unk8;
    FreeHdr *pos; /* 0xC: VRAM slot from func_800282F8 */
} TexUploadEntry;

typedef struct {
    s32 count;
    TexUploadEntry e[1];
} TexUploadList;

SortNode *func_8001251C(SortList *arg0);

typedef struct PosNode {
    s32 *base;         /* 0x00 */
    s32 off;           /* 0x04 */
    u8 pad8[0x14];
    struct PosNode *next; /* 0x1C */
    struct PosNode *prev; /* 0x20 */
} PosNode;
typedef struct PosList {
    u8 pad0[0xC];
    PosNode *head;     /* 0x0C */
    PosNode *tail;     /* 0x10 */
} PosList;

PosNode *func_800125B4(PosList *arg0);

s32 func_800131B0(s32 arg0);

s32 func_80013290(s32 *arg0, u8 * *arg1);

s32 func_80013334(s32 arg0, s32 arg1, u8 * *arg2, s32 arg3);

s32 func_800153B4(s32 *a, s32 *b, s16 *out);

u8 *func_80015870(u8 *arg0, s32 arg1);

s32 func_800158E4(u8 *arg0, s32 arg1);

u8 *func_80016478(u8 *arg0, u8 *arg1);

s32 func_80016558(u8 *arg0, u8 *arg1);

u8 *func_800165F8(u8 *arg0, u8 *arg1);

s32 func_800166FC(u8 *arg0, u8 *arg1);

u8 *func_80016A20(u8 *arg0, s32 arg1);

u8 *func_80016B48(u8 *arg0, s32 arg1);

void func_80018430(s32 count);

s32 *func_80018694(s32 * *arg0, s32 arg1);

void func_800187D4(ListNode18938 *arg0, s32 arg1);

void func_80018938(ListNode18938 *arg0, s32 arg1);

void func_8001C1E0(u8 *arg0, u16 *src, u8 *tab, u16 *dst);

void *func_8001CF50(ModelEntry *e, s32 arg1);

s32 *func_8001E48C(s32 arg0);

void func_8001E984(s32 arg0, s32 arg1);

void func_80020DF8(u8 *arg0);

void func_80020E9C(NAS *nas, u8 *arg1);

void func_80020F5C(u8 *arg0);

void func_80021054(u8 *arg0);

void func_8002128C(u8 *arg0, u8 *arg1, s32 arg2);

void func_8002147C(VelObj *obj, VelParams *p, VelState *s);
void *func_80010E8C(s32 bytes, Heap *heap);

void func_80021604(u8 *arg0, u8 *arg1, s32 arg2);

void func_80021798(u8 *arg0, u8 *arg1, s32 arg2);

void func_80021A1C(u8 *arg0, u8 *arg1, s32 arg2);

s32 *func_80021CB8(s32 *arg0, s32 arg1, s32 arg2);

u8 *func_80021D1C(u8 *arg0, s32 arg1);

void func_80022504(void);

void func_80023158(s32 arg0);

void func_8002321C(void);

void func_80025B10(EXTengine *eng, u32 flag);

s32 func_80025C80(EXTengine *eng);

s32 func_80025D40(EXTengine *eng);

void func_8002617C(void);

s16 func_80026210(s32 x, u8 *s);

s32 func_8002633C(u8 *s);

Ext400 *func_800265DC(NAS *nas);

Ext1000 *AddObject(NAS *nas);

void func_80026E2C(void);

s32 func_800274FC(s32 *arg0);  /* func_800276C4, then D_8005B698 = NULL; returns the second end */
void func_800276C4(s32 *arg0);

void func_800279A4(void);

void func_80027F7C(s32 pad, s32 arg1, s8 arg2);

void func_80028018(s32 pad, s32 arg1);

FreeNode *func_80028A28(void);

void func_80028AE4(BufNode *arg0);

void func_8002A3D0(u8 *arg0);

void func_8002A684(u8 *arg0, u8 *arg1);

void func_8002A77C(ClutUpload *arg0, s32 arg1);

void func_8002A9AC(s32 *arg0, s32 arg1);

void func_8002BBEC(void);

void func_8002C2C4(SeqSlot *slot);

void func_8002D294(void);

void func_8002D394(u8 *arg0, s16 *arg1);

extern u8 D_80063A84[];

/* Pad rumble state (func_800281C0). */
typedef struct PadRumble {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 act[2];  /* motor values passed to PadSetAct */
    /* 0x06 */ u16 dirty;
    /* 0x08 */ s16 time0;
    /* 0x0A */ s16 time1;
    /* 0x0C */ s16 reset;
} PadRumble;

/* Raw controller packet read by func_80027AD4 (status byte, type in the top nibble). */
typedef struct PadPacket {
    /* 0x00 */ u8 status;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u16 buttons;
    /* 0x04 */ u8 rx;
    /* 0x05 */ u8 ry;
    /* 0x06 */ u8 lx;
    /* 0x07 */ u8 ly;
} PadPacket;

/* PadState as written by func_80027AD4. */
typedef struct PadInput {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 buttons;
    /* 0x08 */ s32 rx;
    /* 0x0C */ s32 ry;
    /* 0x10 */ s32 lx;
    /* 0x14 */ s32 ly;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ PadRumble rumble;
} PadInput;

void func_80027AD4(PadPacket *p, PadInput *in, s32 n);

extern s32 D_80063A8C;

/* func_800281C0's body, inlined into func_80027AD4. */
static inline s32 PadRumble_update(PadRumble *r, s32 port) {
    if (r->reset != 0) {
        if (r->time0 != 0) {
            r->time0 = 1;
        }
        if (r->time1 != 0) {
            r->time1 = 1;
        }
    }
    r->reset = 0;
    if (r->time0 != 0) {
        r->time0 = r->time0 - 1;
        if ((r->time0 << 16) == 0) {
            r->act[0] = 0;
            r->dirty |= 1;
        }
    }
    if (r->time1 != 0) {
        r->time1 = r->time1 - 1;
        if ((r->time1 << 16) == 0) {
            r->act[1] = 0;
            r->dirty |= 2;
        }
    }
    if (PadGetState(port) != 6) {
        return 0;
    }
    if (r->dirty != 0) {
        PadSetAct(port, r->act, 2);
        if (PadSetActAlign(port, D_80063A84) == 1) {
            r->dirty = 0;
        }
    }
    return 8;
}

MemBlock *func_800118BC(MemBlock *blk, s32 words, Heap *heap);

void func_800119D4(MemBlock *arg0, s32 arg1, Heap *arg2);

void func_800120E8(MemBlock *arg0, Heap *arg1);

u8 *func_800129D0(u8 *arg0, s32 arg1);

u8 *func_80012AC4(u8 *arg0);

void func_80012BC8(s32 * *arg0, s32 *arg1, u8 *arg2);

void func_80013B0C(ComboSeq *c);

void func_8001CAD4(u16 *arg0, s32 arg1, u32 arg2);

void func_8001D894(void *sub, void *arg1, s32 flags, s32 arg3);

void func_8001F4F8(u8 *arg0, u8 *arg1);

s32 func_800281C0(PadRumble *r, s32 port);

void func_80029100(AnimOwner *arg0);

void func_8002A4C0(TexUploadList *arg0, u8 *arg1);

void func_8002BD98(SeqSlot *slot, s32 idx);
/* A script cursor: kind 1 or 2 steps idx through a table of entries (0 entries
 * jump to the index in their low half) and stores the entry in cur. */
typedef struct ScriptCursor {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1[0xB];
    /* 0x0C */ s32 cur;
    /* 0x10 */ s32 *tbl;
    /* 0x14 */ u8 pad14[8];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 idx;
    /* 0x22 */ u8 pad22[6];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
} ScriptCursor;

typedef struct {
    s16 a;
    s16 b;
} S16Pair;

void func_80011FC8(MemBlock *arg0, Heap *arg1);

u8 *func_800121F8(u8 *arg0);

void func_800136BC(PadState *pad);

void func_800143F0(s32 arg0, s32 arg1);

s32 func_80015D70(u8 *arg0, u8 *arg1);

MATRIX *func_8001ACC0(GfxXform *obj);

s32 func_8001D48C(subengine * *out_first, subengine * *last, s32 n);

void func_80020CC4(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3);

void func_800210F8(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 * *arg4);

typedef struct ScriptInit {
    s32 cur;
    s32 *tbl;
    u32 flags;
} ScriptInit;

void func_80015AD4(void *arg0);

u8 *func_800189F4(u8 *arg0, s32 w, s32 h);

s32 func_80020064(u8 *arg0);

ScriptCursor *func_80024E30(NAS *nas, ScriptInit *init);

void func_80024FA4(ScriptCursor *c);

ScriptCursor *func_80026048(NAS *nas, s32 arg1, s32 arg2, ScriptInit *init);

s32 func_8002B5B0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

S16Pair func_8002B7C8(s32 x, s32 v);

extern s32 D_8005A60C;

/* Inlined copies of func_8002B6D0, func_8002B760 and func_8002B7C8 (used by
 * func_8002B210). */
static inline void ViewProject(VECTOR *pos, s32 *psxy) {
    VECTOR *out = &D_800569CC.out;
    SVECTOR *in = &D_800569CC.in;
    View *view = (View *)D_80051640;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
}

static inline S16Pair PanVolume(s32 x, s32 v) {
    S16Pair r;
    s32 lo = 0;
    s32 hi = 0x200;

    if (x < lo) {
        r.a = v;
    } else if (x > hi) {
        r.a = 0;
    } else {
        s32 d = 0x40000;
        r.a = ((d - x * x) * v) / d;
    }
    if (0x200 - lo < x) {
        r.b = v;
    } else if (x < 0x200 - hi) {
        r.b = 0;
    } else {
        r.b = ((hi * hi - (0x200 - x) * (0x200 - x)) * v) / (hi * hi - lo);
    }
    return r;
}

static inline s32 ViewVolumeXY(VECTOR *pos, View *view, s32 *psxy) {
    VECTOR *out = &D_800569CC.out;
    SVECTOR *in = &D_800569CC.in;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return func_8002B020(out, D_80064478);
}

S16Pair func_8002B210(VECTOR *pos);

typedef struct { u8 pad[0xC]; } Slot12;
extern Slot12 D_80063B1C[];

typedef struct {
    /* 0x00 */ u16 w0;
    /* 0x02 */ u16 w2;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ u32 w8;
    /* 0x0C */ u32 wC;
    /* 0x10 */ u32 w10;
} TexSrc2A84C;

typedef struct {
    /* 0x00 */ u16 flag;
    /* 0x02 */ s16 f2;
    /* 0x04 */ Slot12 *slot;
    /* 0x08 */ s16 f8;
    /* 0x0A */ s16 fA;
    /* 0x0C */ u32 fC;
    /* 0x10 */ s8 f10;
    /* 0x11 */ s8 f11;
    /* 0x12 */ u16 f12;
    /* 0x14 */ u32 f14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ u16 f1C;
    /* 0x1E */ u16 f1E;
    /* 0x20 */ s32 f20;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ s32 f30;
    /* 0x34 */ s32 f34;
} TexDst2A84C;

void func_800107C8(MemBlock *arg0, Heap *arg1);

void func_8001097C(Heap *heap);

void func_800139A8(void);

void func_80015F60(u8 *arg0, s32 *arg1);

void func_80017640(void);

void func_80017B08(FaceRuns *faces, FacePrim *prim, s32 arg2, s32 mode);

void func_80017D90(FaceRuns *faces, FacePrim *prim);

void func_80019F1C(u8 *arg0, u8 *arg1, s32 arg2);

void func_8001A43C(u16 *arg0, s32 arg1, u32 arg2, s32 arg3, s32 arg4);

void func_8001E0A8(u8 *arg0);

void func_8001EDFC(u8 *arg0, u8 *arg1);

void func_8001FCDC(u8 *arg0, u8 *arg1);

void func_80020344(Quat *a, Quat *b, s32 t, Quat *out);

void func_80022140(s32 arg0, s32 arg1);

s32 func_8002235C(_sfx_handler *voice);

NAS *func_80022C3C(s32 x, s32 y, s32 z, u16 id, s32 slot);

s16 func_80024008(s16 x, u8 *s);

void func_800246D0(GlyphPos *arg0, u8 *s, s32 arg2, s32 arg3);

void func_80024AD8(void);

s32 func_8002578C(NAS *nas);

void func_80027408(void);

void func_80027D90(void);

void func_80029790(u8 *arg0);

void func_8002A84C(TexSrc2A84C *s, TexDst2A84C *d, s32 n);

void func_8002C5BC(void);

/* func_80013F44: camera eased towards a target view */
typedef struct { s32 x, y, z; } CamXYZ;

typedef struct CamView {
    /* 0x00 */ CamXYZ eye;
    /* 0x0C */ CamXYZ at;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ SVECTOR rot;
} CamView;

typedef struct CamEase {
    /* 0x00 */ CamXYZ at;
    /* 0x0C */ CamXYZ rot;
    /* 0x18 */ CamXYZ eye;
} CamEase;

void func_80013F44(CamView *cam, CamView *dst, CamEase *ease);

FacePrim *func_800180BC(s32 arg0, ListNode18938 *list, ListNode18938 *pool, FaceRuns *faces, s32 arg4);

void func_80018B08(GfxViewport *vp, GfxCamera *cam);

void func_8001DAF8(GfxScene *scene);

void func_8001F0D4(u8 *arg0, u8 *arg1);

void func_8001F828(u8 *arg0, u8 *arg1);

void func_800243A0(s16 arg0, s32 arg1, u8 *s, u32 arg3, u32 arg4, s32 arg5);

/* Map placement record (map_enviroment.placements, 0xA0 bytes each),
 * copied into a subengine by func_8001D6B4. */
typedef struct { s32 w[8]; } PlacementBlk;
typedef struct Placement {
    /* 0x00 */ u32 flags;
    /* 0x04 */ SLONG_XYZ pos;
    /* 0x10 */ u8 unk10[0x28 - 0x10];
    /* 0x28 */ PlacementBlk blk;
    /* 0x48 */ u8 unk48[0x74 - 0x48];
    /* 0x74 */ u8 unk74[4];
    /* 0x78 */ u8 unk78[0x88 - 0x78];
    /* 0x88 */ u16 unk88;
    /* 0x8A */ u8 unk8A[0x9C - 0x8A];
    /* 0x9C */ u16 unk9C;
    /* 0x9E */ u16 unk9E;
} Placement;
/* Head of map_enviroment (core.h) as func_8001D6B4 uses it. */
typedef struct MapEnvHead {
    /* 0x00 */ void *dbase;
    /* 0x04 */ u8 unk04[0x14 - 0x04];
    /* 0x14 */ Placement *placements;
    /* 0x18 */ s32 num_placements;
    /* 0x1C */ subengine *subs;
} MapEnvHead;
/* func_8001D6B4: reuse the map's subengine array or allocate it. */
static inline subengine *MapEnv_GetSubs(MapEnvHead *env) {
    subengine *sub;
    subengine *t;

    t = env->subs;
    if (t != NULL) {
        sub = t;
        func_8001D334(sub);
    } else {
        sub = (subengine *)func_80011654(env->num_placements * 0xA8, &D_8004E0F0);
    }
    return sub;
}

void func_8001D6B4(MapEnvHead *env);

u8 *func_8001FE80(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 * *arg5);

void func_80028D40(AnimTex *t, s32 base, AnimTexSet *set);

/* func_800248A0: three 0x38-byte sprite records drawn side by side */
extern u8 D_80053074[];
extern u16 D_8005A5C0[][3];

void func_8001492C(void);

void func_8001C7FC(NAS *nas, u8 *arg1, u32 flags);

KeyEntry *func_8001E21C(KeyBlock *arg0, s32 arg1, SLONG_XYZ *out);

void func_8001E610(u8 *mgr, u8 *fadeState);

void func_8001EAA4(u8 *arg0, u8 *arg1);

void func_800235BC(ArenaDef *arg0);

void func_8002854C(FreeNode *n);

void func_80028774(VramArea *areas, FreeNode *pool, s32 npool, s32 nbuf);

#endif /* BASH_MAIN_H */
