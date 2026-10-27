#ifndef BASH_MAIN_H
#define BASH_MAIN_H

/* Functions and data defined in main.exe's game code. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/main.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/core.h"   /* the Oct 9 records' types (map_enviroment, script_thread_handler, ...) */

typedef struct {
    s32 tracksOff; /* 0x00 */
    s32 subOff;    /* 0x04 */
} AnimData;

/* An obj_texture as texture_update_scroll scrolls it. */
typedef struct {
    u8 unk0[0x1C];
    u16 w;          /* 0x1C */
    s16 h;          /* 0x1E */
    s16 *src;       /* 0x20: x, y */
} AnimEntry;

/* A scrolling track (func_80029100's sub list, 16 bytes each); the name is
 * the C++ symbols' (texture_update_scroll__FP14texture_scrollP14obj_textureset). */
typedef struct texture_scroll {
    s32 idx;        /* 0x00: entry in obj_textureset.tex */
    s32 speed;      /* 0x04 */
    s32 pos;        /* 0x08: 24.8 */
    s16 *dst;       /* 0x0C: x, y */
} texture_scroll;


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

extern s32 GameLanguage;

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

/* A 0x28-byte camera key in the track update_camera_thread walks; it interpolates
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

s32 interp(s32 arg0, s32 arg1, s32 arg2, s32 arg3);



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


/* Frame range of an animation track (at track+0x14). */
typedef struct {
    /* 0x00 */ u16 base;
    /* 0x04 */ s32 lo;
    /* 0x08 */ s32 hi;
    /* 0x0C */ s32 off;
    /* 0x10 */ s32 clamp;
} AnimRange;


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



/* A 0x38-byte record in the table at +8 of the block func_8002A9AC frees
 * (count at +0). */
typedef struct TexAnimEntry {
    /* 0x00 */ u8 unk0[0x20];
    /* 0x20 */ s32 handle; /* freed with func_80028AE4 */
    /* 0x24 */ u8 unk24[0x10];
    /* 0x34 */ s32 time;   /* frame stamp, compared to FrameCount */
} TexAnimEntry;


/* Bounding box tested for visibility by func_8001AF2C: corners min/max and a
 * centre whose pad is a radius. */
typedef struct {
    s16 min[3];     /* 0x00 */
    s16 max[3];     /* 0x06 */
    SVECTOR center; /* 0x0C: center.pad = radius */
} BoundBox;

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

/* The part of a 0x38-byte texture entry from +0x18 (prim_write_uvs). */
typedef struct {
    u8 unk0[0xC];
    u16 tpage; /* 0x0C */
    u16 clut;  /* 0x0E */
    u16 uv;    /* 0x10: ORed into each vertex's uv */
    u16 uv1;   /* 0x12: uv of vertices 1-3 (Pogo's psxpogo_render_tile) */
    u16 uv2;   /* 0x14 */
    u16 uv3;   /* 0x16 */
} GfxTexInfo;

typedef struct {
    u8 unk0[0x18];
    GfxTexInfo info; /* 0x18 */
    u8 unk30[0x8];
} GfxTexEntry;

typedef struct {
    u8 unk0[2];
    u16 clut; /* 0x02 */
    u8 unk4[8];
} GfxClutEntry;

/* At +0x10 of GfxModel (prim_write_uvs). */
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

/* Face data prim_write_uvs reads: per face a uv index (0x18), a run-length
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
 * kill_sprim_sub): one primitive per frame buffer. */
typedef struct {
    POLY_GT4 *prim; /* from new_sprim, freed with release_sprim */
    u8 unk4[8];
} ModelObjSlot;

typedef struct {
    u32 flags;               /* 0x00: 0x8000 visible */
    u8 unk4[0x68];
    struct GfxModel *model;  /* 0x6C */
    u8 unk70[4];
    u16 index;               /* 0x74: passed to entity_address */
    u8 unk76[0x16];
    u16 unk8C;               /* 0x8C: clut to set on the next draw */
    u16 unk8E;               /* 0x8E: texture index to set on the next draw */
    ModelObjSlot slots[2];  /* 0x90: indexed by FrameCount & 1 */
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
    void *unk4;
    s32 *farColor; /* 0x08: r, g, b for gte_ldfcdir */
    s32 dp;        /* 0x0C: depth-cue factor for gte_lddp */
    void *viewport; /* 0x10: GfxViewport (func_80018B08) */
    s32 modelDp;   /* 0x14: dp for ModelObj quads (func_8001AA48); the screen fade */
    u8 unk18[4];
    s16 clipX1;    /* 0x1C: viewport clip right/bottom on screen */
    s16 clipY1;
    s32 ofx;   /* 0x20: geometry offset x (scaled by the screen width / 640) */
    s32 ofy;   /* 0x24: geometry offset y (halved) */
    u8 unk28[0xC]; /* 0x28: a GfxScreen */
    SVECTOR gteIn; /* 0x34: GTE scratch, a position relative to the view (func_8002B6D0) */
    u8 unk3C[4];
    u32 *ot;   /* 0x40 */
    s16 otzAdd; /* 0x44: added to the average Z before it is halved */
    s16 otLen;  /* 0x46: entries in ot */
    struct GfxCamera *cam; /* 0x48 */
    u8 unk4C[0x28];
    VECTOR gteOut; /* 0x74: gteIn rotated and translated */
    u8 *unk84; /* 0x84: = unkCC */
    u8 *unk88; /* 0x88: = unk10C */
    MATRIX unk8C; /* 0x8C: set to identity by draw_init */
    MATRIX unkAC; /* 0xAC: object's local-to-screen matrix (func_8001ACC0) */
    u8 unkCC[0x40];
    u8 unk10C[0x20]; /* the struct ends at 0x12C: D_80056AC4 is the next global */
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
 * gte_stsxy3_* macros uses these offsets; draw_verts and func_8001A0D8
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
    _sfx_handler *active;       /* 0x544 */
} SfxPool;

/* A VAB file in memory: the header and body as byte offsets from its start
 * (func_8002ADD4 opens it with SsVabOpenHead / SsVabTransBody). */
typedef struct {
    s32 body;
    s32 head;
} VabFile;

/* A loaded sfx bank (machine_load_sfx); the name is the C++ symbols'
 * (machine_load_sfx__FP4fileP21sfx_machine_bank_info). */
typedef struct sfx_machine_bank_info {
    u8 *head;
    s32 id;   /* set once the transfer completes */
    s32 handle; /* asload_permanent_callback's result (machine_load_sfx) */
} sfx_machine_bank_info;

/* Points drawn as one-pixel tiles by func_80015C1C (an engine API entry). */
typedef struct {
    SVECTOR v;
    u8 unk8[0x10];
} PointSpriteSrc;

typedef struct {
    PointSpriteSrc *src;
    TILE_1 *prims[2]; /* one per frame buffer (FrameCount & 1) */
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
 * heap's free list, sorted by size (mal_free frees and merges). The names
 * mal and _mal_area are the C++ symbols' (mal_unlink__FP3malP9_mal_area). */
typedef struct mal {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u8 unk2[2];
    /* 0x04 */ void **owner;             /* cleared on free; (void **)-1 for none; NULL when free */
    /* 0x08 */ u32 size;                 /* in words, header not included */
    /* 0x0C */ struct mal *prev;    /* the block before it in memory */
    /* 0x10 */ struct mal *prevFree;
    /* 0x14 */ struct mal *nextFree;
} mal;

#define MEMBLOCK_NEXT(b) ((mal *)((u8 *)(b) + (b)->size * 4 + sizeof(mal)))

/* Header of a pool entry; the data follows it. */
typedef struct PoolNode {
    struct PoolNode *next;
    s32 state;                        /* 2 once handed back (release_sprim) */
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

/* PrimPool: entries come from free (new_sprim) and go back on done. */
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

/* Per-pad state, 8 of them at Pads (func_80027F00 resets them). */
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
    /* 0x2A */ u8 unk2A[0x44 - 0x2A];
    /* 0x44 */ s32 held;      /* buttons down */
    /* 0x48 */ s32 pressed;   /* buttons down this frame and not the last */
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
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

/* Loaded sound data and the sequence opened from it (CurrentMusic); the name is
 * the C++ symbols' (music_dload_midi__FP4fileP10music_midil). */
typedef struct music_midi {
    /* 0x00 */ s32 data;                 /* heap block, freed by func_8002C44C */
    /* 0x04 */ s32 seq;                  /* SsSeq access number, or -1 */
    /* 0x08 */ s32 p8;                   /* data + header offset (func_8002C2C4) */
    /* 0x0C */ s32 pC;
    /* 0x10 */ s32 p10;
    /* 0x14 */ s32 p14;
    /* 0x18 */ s32 p18;
    /* 0x1C */ s32 unk1C;                /* set to -1 on close */
} music_midi;

/* Set both channels of slot's sequence volume (func_8002BEA8). */

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

/* MusicTable: what func_800229FC passes to func_8002C488 for an index. */
typedef struct MusicEntry {
    /* 0x00 */ s32 file;
    /* 0x04 */ s16 reverb;               /* SsUtSetReverbType */
    /* 0x06 */ u8 unk6[2];
    /* 0x08 */ s32 unk8;                 /* func_80022140: music file */
    /* 0x0C */ s32 unkC;                 /* func_80022140: alt music file (flag 0x80) */
    /* 0x10 */ s16 tempo;
    /* 0x12 */ u8 unk12[0x14 - 0x12];
} MusicEntry;

/* Loader: file loader state (func_80013238 resets it). Only these fields are known. */
typedef struct LoaderState {
    /* 0x00 */ s32 state;                /* 3 after a reset */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
} LoaderState;

/* A process (a game mode), its control block and the screen fade, from the
 * Jampack Winter 2000 demo's debug types (docs/PROTOTYPES.md).
 * A process's init runs when it starts (func_8001E610), update every frame,
 * render from func_8001E898 and quit when it ends; a non-NULL heap resets the
 * heap to start there. */
typedef struct process_info {
    /* 0x00 */ void (*init)(void);
    /* 0x04 */ void (*update)(void);
    /* 0x08 */ void (*render)(void);
    /* 0x0C */ void (*quit)(void);
    /* 0x10 */ u32 *heap;
} process_info;
BASH_SIZE_CHECK(process_info, 0x14);

/* func_8001E610 switches to next_process once the fade is done. */
typedef struct process_control_info {
    /* 0x00 */ process_info *process;
    /* 0x04 */ process_info *next_process;
    /* 0x08 */ process_info *last_process;
    /* 0x0C */ s32 flags;   /* 1: skip quit, 2: skip init, 4: skip render, 8: skip update, 0x10: clear after a switch */
} process_control_info;
BASH_SIZE_CHECK(process_control_info, 0x10);

/* amount steps by speed towards 0x1000 or 0; maxed counts the frames at the
 * end (func_8001E598). */
typedef struct fade_info {
    /* 0x00 */ s32 amount;
    /* 0x04 */ s32 speed;
    /* 0x08 */ s32 maxed;
    /* 0x0C */ s32 delay;
} fade_info;
BASH_SIZE_CHECK(fade_info, 0x10);

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

/* A heap (MainHeap is the main one). Only these fields are known. */
typedef struct _mal_area {
    /* 0x00 */ u8 unk0[8];
    /* 0x08 */ mal *free;           /* free list, smallest first */
    /* 0x0C */ mal *lowest;         /* lowest free block, or NULL */
    /* 0x10 */ mal *deferred;       /* freed once unk2 ticks run out (func_80011578, func_80011DB4) */
} _mal_area;

/* The used block right after the heap's lowest free block, or NULL. */
static inline mal *Heap_AfterLowest(_mal_area *heap) {
    mal *nx;
    mal *lo = heap->lowest;

    if (lo == NULL) {
        return NULL;
    }
    nx = (mal *)((u8 *)lo + (lo->size * 4 + 0x18));
    if (nx->owner == (void **)-1 || nx->size == 0) {
        return NULL;
    }
    return nx;
}


extern s32 FrameCount;

/* Heap_AfterLowest, unless it is flagged busy for this frame buffer. */
static inline mal *Heap_NextToMove(_mal_area *heap) {
    mal *p;

    p = Heap_AfterLowest(heap);
    if (p != NULL && (p->unk0 & 2) && (p->unk0 & 1) == (FrameCount & 1)) {
        return NULL;
    }
    return p;
}

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

void mal_print_free(void);
s32 func_8001151C(u8 *arg0);
void func_80011530(u8 *arg0);
void func_80011544(u8 *arg0, u8 * *arg1);
s16 func_8001156C(u8 *arg0);
u8 *func_80011654(s32 arg0, _mal_area *arg1);
u8 *func_80011748(s32 arg0, u8 *arg1);
void mal_unlink(mal *blk, _mal_area *area);
void func_80011D00(s32 arg0);
void func_80011D28(u8 *arg0);
u8 *func_80011DB4(_mal_area *arg0);
void func_80012DDC(s32 arg0);
void func_80012E0C(void);
void func_80012E14(u8 *arg0);
void func_80012F04(LoadNode *arg0);
s32 func_8001534C(u8 *s);
s32 func_80013650(s32 arg0, void (*done)(), void *ctx);
s32 func_80012FFC(void);
s32 func_8001231C(void);
void func_80010AE8(_mal_area *arg0);
mal *mal_first_free(u32 words, mal *blk);
s32 func_80013034(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6);
s32 asload_permanent_callback(s32 arg0, void (*done)(), void *ctx);
void func_80012E88(void);
void func_80012F90(u8 *arg0);
u8 *func_80013110(u8 *arg0);
s32 func_80013160(u8 *arg0);
void set_joypad(u8 *arg0, u32 arg1);
void cam_init(u8 *arg0);
void func_8001452C(s32 arg0, s32 arg1);
s32 *func_800153A8(void);
s32 func_80015480(s32 arg0, s32 arg1);
s32 func_800154A0(s32 arg0, s32 arg1);
void SetRandomSeed(s32 arg0, s32 arg1);
void func_80015D08(u8 *arg0);
s32 func_80015E94(u8 *arg0, u8 *arg1);
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
void func_800369FC(void);

extern SVECTOR D_80056AC8[];
typedef struct { s16 unk0; s16 unk2; } TrigEntry;
extern TrigEntry rcossin_tbl[]; /* libgte's sine/cosine table (cstbl.o) */
/* A 4.12 fixed-point quaternion (func_80020344 slerps two of them). */
typedef struct { s32 v[4]; } Quat;
extern s32 D_80064478[];
extern s32 D_8006E9F8;
extern u8 D_800527C4[];
extern u8 HeroModelBank[];
extern u8 D_8004DDBC[];
extern u8 D_80050010[];
void func_8002D488(void);
void func_8002D4F4();
extern s32 D_80064414;
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
s32 func_800155FC(void);
extern VTable8 *D_8004E0DC;
extern VTable8 *D_8005B648;
extern s32 D_8004E0E0;
extern s32 D_8004E0E4;
void mal_update(_mal_area *heap);
extern u8 HeapStart[];
mal *func_80011BF8(_mal_area *heap, u8 *start, s32 size);
extern u8 D_8004E0B8[];
s32 func_800134FC(u8 *a, u8 *b);
void init_file(void);
extern s32 D_8004E0B4;
extern s32 D_8006E9F4;
s32 func_800320CC(void);
void func_800320DC(s32 arg0);
extern void *LoadNodeFree;
extern u8 D_80050F40[]; /* 63 x 0xC pool, free list in D_80051240 */
extern u8 LoadNodePool[]; /* 63 x 0x24 pool, free list in LoadNodeFree */
extern s32 cam_ramp_overide_count;
extern s32 D_800517B4;
extern s32 RandomSeedA;
extern s32 RandomSeedB;
extern void *ScriptList;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    void (*unkC)(u8 *, u8 *);
} D_80058B00_Entry;
extern D_80058B00_Entry ScriptCommands[];
extern s32 World_floor_level;
extern s32 D_800655A8;
extern s32 CardChannel;
extern s32 CardStatus;
extern u8 CardBuffer[];
/* The memory card save file header at CardBuffer (PsyQ _CARD layout). */
typedef struct {
    u32 magic : 16;  /* 0x00 "SC" */
    u32 frames : 4;  /* 0x02 icon frames */
    u32 type : 4;
    u32 blocks : 8;  /* 0x03 */
    s16 title[32];   /* 0x04 Shift-JIS */
} McHeader;
extern char *D_800655BC;
extern u8 D_80065648[]; /* the header's icon frames (CardBuffer + 0x80) */
extern const char CardSaveTitleFormat[];
s32 func_80010000(u8 *arg0);

/* Writes up to 32 Shift-JIS characters of src (ASCII) to dst, padding with
 * full-width spaces (func_8002C5BC). */
typedef struct {
    u8 b[8];
    u8 unk8[0x18C]; /* a copy of GameInfo from +0x60 (the hub's save), with the arena results */
} D_800657C8_Slot;
extern D_800657C8_Slot CardSlots[];
extern s32 D_80065E24;
extern s32 CardRequest;
extern s32 CardCommand;
extern s32 D_800675D0;

/* 0x28-byte entries starting at 0x800675D4; D_800675D0 is the count. */
typedef struct {
    u8 unk0[0x18];
    s32 size; /* 0x18 */
    u8 unk1C[0xC];
} SizeEntry;
extern SizeEntry D_800675D4[];
extern const char CardFileName[];
extern s32 D_8006782C;
extern s32 D_80067830;

/* The directory entry named CardFileName, or NULL (func_8002D1D4). */

/* Clears the save's slots and flags and re-sums it (func_8002D294). */

/* McResetSave with the checksum in its own variable (allocates it apart from i). */

/* Zeroes the 0x2000-byte save buffer. */

/* Free bytes on the card (func_8002D178). */
extern _mal_area MainHeap;
extern s32 D_800637B0;
extern u8 *D_8006E9FC;
extern u8 FontModel[];
extern void *D_8005AA34;
extern void *HeadOfObjects;
extern music_midi CurrentMusic;
extern LoaderState Loader;
extern const char MsgTooManyLoadsInQueue[];
extern void func_80012840(u8 *, u8 *);
extern void func_80012678(u8 *, u8 *);
extern MusicEntry MusicTable[];
extern sfx_machine_bank_info SfxBanks[6];
extern NodePool PrimPool;
extern NAS *HeadOfSolids;
extern NAS *D_8005AA9C;
extern void *HeadOfMissiles;            /* tail of the HeadOfObjects list */
extern void *D_8005AA30;            /* tail of the HeadOfSolids list */
extern s32 VelocityCalc;
extern s32 ACMD[16][2];       /* entries 1..15 cleared by func_80026D9C */
extern s32 PadCount;
extern s32 first_joypad;            /* first pad with flag 1 set, or -1 */
extern s32 PadRunEnd;            /* pads 0..n-1 all have flag 1 */
extern s32 joypad_idle_count;            /* frames with no button down */
extern u8 D_80063A90[2][0x34];    /* PadInitMtap buffers */
extern PadState Pads[8];
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
extern FreeNode *FreeBufNodes;
extern u8 D_80063B58[];
extern u8 D_80063BA0[];
extern FreeNode *D_80063BAC;
extern FreeNode *D_80063BB0;
extern void *D_80063BB4;        /* list walked by func_80028A00 (next at +4) */
extern s32 D_8004E104;            /* func_80011410: free bytes */
extern s32 D_8004E108;            /* func_80011410: free blocks */
extern s32 DatIndex[];         /* the archive's {sector, size} entries, file N at [2 * N] */
extern s32 D_8004E10C;            /* func_80011410: largest free block, bytes */
extern s32 SfxBankCount;            /* slots of SfxBanks in use */
extern char D_800654CC[];
extern s32 D_8005A608;
extern u8 GameInfo[];
/* GameInfo as seen by func_80022140. */
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
extern u16 ModeTypes[];          /* func_80022140: 6-byte entries indexed by GameInfo+0x34 */
/* ModeTypes as 6-byte entries indexed by level (func_800235BC). */
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
extern LevSpec LevelSpecs; /* the level's hooks */
extern s32 D_8005AA50;
extern s32 D_8005AA54;
extern s32 D_8005A5E4;
extern const char MsgFatalMetricsError[];         /* "FATAL METRICS ERROR ..." */
extern s32 D_80058F0C[][8];       /* func_80022140 */
extern s8 GameOptions[];
extern s32 card_command_delay;
extern s32 D_8005A620;
extern u8 D_800556F4[];
extern s32 GlobalNAStatus;
extern u8 MainCamera[];
extern u8 LoadingModel[];
extern s16 GameMode;
extern u16 GameLevelFlags;
extern s32 FontPage;
extern s8 SysFontWidth[];
extern u8 SysFontFlags[];
extern u8 *sysfont_dbase;
extern s16 SysFontLookUp[];
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
extern VabFile *VabLoading;  /* VAB being transferred to the SPU, or NULL */
extern sfx_machine_bank_info *SfxBankLoading;  /* where its id goes */
extern s32 D_800654C8;       /* the id from SsVabOpenHead */
extern u8 D_800644A8[];
extern u8 D_8005A688;
extern s32 xa_count;
extern s32 D_800654B8;
extern s32 D_80064418[];       /* voice bit masks, by voice */
extern s32 D_800655B0;
extern s32 ASTCount;            /* entries in ASTTable */
extern ASTEntry *ASTTable;      /* the AST table in use */
extern s32 D_80058AC8;
extern SfxPool SfxVoicePool;
extern s8 SfxTable[][2];
extern s32 D_800637B4;

/* 32-byte records of D_8005A5E8, indexed by D_8005A61C (func_8008688C). */
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4; /* passed to func_800129D0 by func_80093854 */
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u8 unkE[0x12];
} EngineRec32;
extern EngineRec32 *D_8005A5E8;
extern s32 D_800637B8;
void func_80034C4C(s32, s32);
extern u8 D_8006EA08;
extern CdlLOC D_8006EA10;
void func_80034AEC(void);
extern NASTrigger *HeadOfTriggers;
extern s32 TriggerCount;
extern void *D_80058B78;
extern RECT D_8005B640;
extern void *free_scripts;
extern void *MapScene;
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
extern MapPoint MapPoints[32];
/* A button-combo sequence: steps[] are pad masks, 0-terminated. */
typedef struct ComboSeq {
    /* 0x00 */ void (*fn)(void);
    /* 0x04 */ s32 maxTime;
    /* 0x08 */ s32 step;
    /* 0x0C */ s32 startTime;
    /* 0x10 */ s32 steps[1];
} ComboSeq;

extern MATRIX MapSceneMatrix;
extern SVECTOR MapSceneRot;
extern u8 D_800512A8[]; /* ComboSeq table, 0x90 bytes per entry */
extern u8 D_800536CC[];
extern CdlLOC D_8006EA14;
extern RECT *ScreenRect;
extern s32 D_8005B694;
extern s32 D_8005B78C;
extern s32 D_800335BC;
extern u32 D_80063AF8[];
extern s32 D_800643E8;
extern s32 D_800643EC;
extern s32 D_800643F0;
void *func_800112A4(void *base, s32 offset, _mal_area *arg2);
void func_80014540(u8 *arg0, RECT *rect);
extern GfxState RenderState;
extern VECTOR D_8005688C; /* screen-space translation (func_8001965C) */

/* The current draw buffer: primitives are allocated from the pointer at
 * +0x4008 (texture_draw). */
extern u8 *cdb;

/* A screen offset pair, copied as one struct. */
typedef struct {
    s32 x, y;
} GfxOffset;

/* A viewport func_80018B08 makes current (coordinates in 640-wide units). */
typedef struct {
    s16 x0, y0, x1, y1;     /* 0x00: draw area */
    s16 cx0, cy0, cx1, cy1; /* 0x08: clip area (func_80014540) */
    s32 ofx, ofy;           /* 0x10: geometry offset */
    s32 otIdx;              /* 0x18: first ot entry in cdb */
    s32 otLen;              /* 0x1C */
    s32 unk20;
    u8 r, g, b, isbg;       /* 0x24: background */
} GfxViewport;

/* GfxViewport's background colour, copied as a block (func_8001DAF8). */
typedef struct {
    s8 r, g, b;
} GfxRgb8;

/* The scene func_8001DAF8 sets up (MapScene points at it). */
typedef struct {
    u8 unk0[0x10];
    GfxViewport *viewport; /* 0x10 */
    u8 unk14[8];
    u8 *objs;              /* 0x1C: list linked at +0x5C, callback at +0x54 */
} GfxScene;



/* A texture: the 0x38-byte entries of a texture set's table, which
 * texture_draw draws as one POLY_FT4 (the name is the C++ symbols',
 * texture_draw__FP11obj_textureG4xy_wiUl). AnimEntry and TexDst2A84C are other
 * views of the same entry, kept apart until their fields are merged. */
typedef struct obj_texture {
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
} obj_texture;

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


/* calc_rgbcode: fades r, g, b towards black by RenderState.modelDp / 0x1000. */
/* FadeColor reading the fade from gs->modelDp (func_80029F9C). */

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
extern s32 FrameCount; /* frame counter: its low bit picks the frame buffer */
extern s32 D_8004E0E8; /* VSync ticks spent compacting the heap (mal_update) */
extern s32 D_8004E0EC; /* words moved compacting the heap (mal_update) */
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
extern s32 D_80056AC4; /* func_80019A60: the object's +0x8A */
extern s32 D_80056878;    /* func_80019094: with D_80056AC4, forces the depth-cued path */
extern s32 D_80056868[3]; /* func_80019094: default far colour */

extern u8 TankObjects2Model[];
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

extern EXTmotion ExtMotionTemplate;
typedef struct Ext400 {
    s32 w[11];
} Ext400;
extern Ext400 ExtTemplate400;
typedef struct Ext1000 {
    s32 w[11];
} Ext1000;
extern Ext1000 ExtTemplate1000;
extern subengine SubEngineTemplate;
typedef struct Words4 {
    s32 w[4];
} Words4;
s32 abs(s32);         /* LIBC2 ABS_0.OBJ (labs in ABS_1.OBJ is identical); a call, not inlined: -fno-builtin */
/* A step of a kind 2 animator's Atable (func_80025534): cmd >= 0 is a frame
 * shown for arg ticks, 0x8000 | n is command n with operand arg. */
typedef struct AnimStep {
    s16 cmd;
    u16 arg;
} AnimStep;
extern s32 D_8005B5B8;
extern void (*D_800643F8)();
extern NAS *NASActiveTail;
extern s32 *D_80056850;
extern s32 *D_8005683C;            /* pool sizes (func_80017164) */
extern u8 *D_8005685C;             /* 0x58-byte nodes (func_80017164) */
extern u8 *D_80056840;             /* 0x54-byte nodes, free list in D_80056850 */
extern u8 *D_80056854;             /* 8-byte nodes, free list in D_80056858 */
typedef struct ListNode18938 {
    s32 f0;
    s32 f4;
    s32 **f8;
    s32 *fC;
    s32 **f10;
} ListNode18938;

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
extern s32 num_sfx_banks;
extern s32 D_800654B0;
extern s32 D_800654B4;
extern s32 GameFrameCount;
extern s32 D_8004E0D0; /* BCD of func_80010000's count */
extern s32 FirstBootDone;
extern subengine *next_sub;
extern s32 D_80058B60;
extern s32 D_80058B64;
extern s32 D_80058B68;
extern s32 D_80058B6C;
extern s8 D_8005A63C;
extern void *HeadOfBalls;
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


extern NAS *NASFreeList;
extern u16 FirstNas;            /* first pool slot func_80026D08 frees */
extern u16 D_8005AA12;            /* pool slot count */
extern u16 NasPoolUsed;            /* pool slots in use (func_80026FCC kills them all) */
extern NAS *NasPool;           /* NAS pool */
extern NAS NASTemplate;            /* copied over each spawned NAS */
extern EXTengine EngineTemplate;   /* copied over each new engine */
extern s16 AtanTable[];            /* get_angle */
extern u16 GameExitFlags;
extern s32 D_800512A4;
extern s32 screen_aspect[2];





void func_80015758(SVECTOR *v, VECTOR *out);






void func_80015C1C(PointSprites *ps, MATRIX *m);



void func_80012E54(void);

void func_8001325C(void);

void palette_set_top_bits(u16 *arg0, u32 arg1);


void func_800154C0(u16 *arg0, s32 arg1);





s32 Random(s32 n);




s32 get_angle(s32 x, s32 z);





void mal_free(void *ptr, _mal_area *heap);












s32 func_80011410(_mal_area *heap);



void func_800101E0(void);




void func_80013C78(void);

s32 func_8001538C(u32 x);

s32 func_80015284(s32 a, s32 b, s32 t);

void *func_80015AB0(s32 *arg0, u8 *rec);





void func_80013CFC(s32 pad, u8 *out);








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


u8 *func_80015A78(s32 i, u8 *rec);

void BSwapEndian(u8 *p, u32 n);





void WSwapEndian(u16 *p, u32 n);



void func_80013238(void);





void *func_80012E28(void);














void *func_80011828(void *ptr, s32 bytes);

void func_800138A4(void);

s32 asload_chunk_relocatable(s32 arg0, s32 arg1, u32 **arg2, s32 arg3);


u8 *func_8001316C(u8 *arg0);

s32 func_8001361C(s32 *arg0, u8 *arg1);

mal *func_80011BC0(mal *blk);

s32 func_800135E4(s32 *arg0, u8 *arg1);

mal *func_80011878(mal *blk, s32 words);

s32 interp(s32 arg0, s32 arg1, s32 arg2, s32 arg3);


/* Per-note pitch data: a signed note and a signed fine nibble in the top of +1. */
typedef struct {
    s8 note;
    u8 fine; /* 0x01: high nibble is a signed fine value */
} PitchInfo;

typedef struct {
    s16 voice;       /* 0x00 */
    PitchInfo *info; /* 0x04 */
} PitchVoice;


s32 func_80015984(s32 i, EntryBlock *blk);

void func_80011578(u8 *arg0, _mal_area *heap, s32 arg2);





void func_80010354(void);

mal *func_80011F74(_mal_area *heap);

void func_80013C24(void);

void func_80013CA4(void);

s32 map_entity_address(u16 arg0, u8 *arg1);










u8 *entity_address(u32 arg0, u8 *arg1);



s32 func_800152A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);



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
    s32 font;    /* 0x78: into FontPage */
} TextNAS;
extern PosEntry D_8005689C[];
void func_80010158(void);

void shell_render_dcache(void);

void func_800102F8(void);

void func_80010394(void);

void func_80010410(void);

void func_80011AE8(mal *blk, _mal_area *heap);

mal *func_80011C98(mal *arg0);

mal *func_80011EF0(_mal_area *heap);

void func_80012CD4(void *arg0, s32 *arg1);

s32 func_80012D80(s32 *arg0, s32 *arg1);

void func_80012FA4(LoadNode *arg0);

s32 load_file_permanent(s32 *arg0);

s32 func_80013488(s32 arg0, s32 arg1, s32 arg2);

void func_800150E8(char *dst, s32 val, s32 div);

char *func_8001517C(char *arg0, s32 arg1, s32 arg2);

s32 func_80015210(s32 a, s32 b, s32 t);



































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

u8 *func_80011498(u8 *arg0, s32 arg1, _mal_area *arg2);

void *func_800115D8(s32 arg0, void * *arg1, s32 arg2, s32 arg3);

struct obj_textureset;

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




















void *func_80010E8C(s32 bytes, _mal_area *heap);
































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


extern s32 D_80063A8C;

/* func_800281C0's body, inlined into func_80027AD4. */

mal *func_800118BC(mal *blk, s32 words, _mal_area *heap);

void func_800119D4(mal *arg0, s32 arg1, _mal_area *arg2);

void func_800120E8(mal *arg0, _mal_area *arg1);

u8 *func_800129D0(u8 *arg0, s32 arg1);

u8 *func_80012AC4(u8 *arg0);

void func_80012BC8(s32 * *arg0, s32 *arg1, u8 *arg2);

void func_80013B0C(ComboSeq *c);







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

void func_80011FC8(mal *arg0, _mal_area *arg1);

u8 *func_800121F8(u8 *arg0);

void func_800136BC(PadState *pad);

void func_800143F0(s32 arg0, s32 arg1);

s32 func_80015D70(u8 *arg0, u8 *arg1);





typedef struct ScriptInit {
    s32 cur;
    s32 *tbl;
    u32 flags;
} ScriptInit;

void init_dots(subengine *arg0);








extern s32 D_8005A60C;

/* Inlined copies of func_8002B6D0, func_8002B760 and func_8002B7C8 (used by
 * func_8002B210). */




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

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 f4;
    /* 0x08 */ s32 off;
} TexEnt12;

/* A texture set (a dbase's, at +0x10): its textures and their scroll
 * animations. The name is the C++ symbols'
 * (texture_init_mem_textureset__FP14obj_textureset). */
typedef struct obj_textureset {
    /* 0x00 */ s32 nTex;
    /* 0x04 */ s32 nEnt;
    /* 0x08 */ TexDst2A84C *tex;      /* nTex obj_textures */
    /* 0x0C */ TexEnt12 *ent;
    /* 0x10 */ u8 *base;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ AnimData *anims;       /* in base, or NULL */
    /* 0x1C */ s32 nAnim;
    /* 0x20 */ u8 *anim;
} obj_textureset;

/* A dbase: one of the 0x38-byte records at HeroModelBank (load_initial_dbases'
 * pool), a loaded file and its textures (func_8001612C loads it, the callbacks
 * below fill it). The name is the C++ symbols'
 * (dbase_mesh_anim_name_loaded__FP10_obj_dbasePc; the Oct 9 records' tag is
 * obj_dbase, whose typedef is in bash/core.h); the fields are from the code. */
struct obj_dbase {
    /* 0x00 */ s32 file;              /* loaded into data once the textures are in */
    /* 0x04 */ s32 textureFile;       /* loaded first, into textures (func_8001612C) */
    /* 0x08 */ s32 load;              /* the load in flight, or 0 */
    /* 0x0C */ u8 *data;
    /* 0x10 */ obj_textureset textures;
    /* 0x34 */ s32 *keep;             /* name hashes of the blocks to keep (func_80015F60) */
};
BASH_SIZE_CHECK(obj_dbase, 0x38);

void func_800107C8(mal *arg0, _mal_area *arg1);

void func_8001097C(_mal_area *heap);

void func_800139A8(void);























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




/* func_800248A0: three 0x38-byte sprite records drawn side by side */
extern u8 InterfaceModel[];
extern u16 D_8005A5C0[][3];

mal *func_80011E80(_mal_area *heap);

void func_8001492C(void);













#include "bash/card.h"

#include "bash/music.h"

#include "bash/sound.h"

#include "bash/nascalls.h"

#include "bash/sfx.h"

#include "bash/prims.h"

#include "bash/dbase.h"

#endif /* BASH_MAIN_H */
