#ifndef BASH_MAIN_H
#define BASH_MAIN_H

/* Functions and data defined in main.exe's game code. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/main.c */

#include "common.h"
#include "bash/nas.h"
#include "bash/core.h"   /* the Oct 9 records' types (map_enviroment, script_thread_handler, ...) */

/* A model a mode loads (ModeDbaseLists, 16 bytes; queue_dbase_load): the file group it comes in,
 * the bank record it loads into, its model's and its textures' archive entries. */
typedef struct ModeDbase {
    FileGroup *group;
    obj_dbase *dbase;
    s32 *file;
    s32 *textureFile;
} ModeDbase;

typedef struct AnimData {
    s32 tracksOff; /* 0x00 */
    s32 subOff;    /* 0x04 */
} AnimData;

/* One texture's frame animation, in a texture file's anims block (texture_update_anims,
 * texture_scroll_step). */
typedef struct TexFrameAnim {
    s32 frames; /* 0x00: offset from here to a table of offsets to each frame's image */
    s32 tex;    /* 0x04: the texture set's texture it loads into */
    s32 count;  /* 0x08: frames */
    s32 speed;  /* 0x0C: added to pos each update (8.8 frames; negative runs backwards) */
    s32 pos;    /* 0x10: the current frame, 8.8 */
} TexFrameAnim;


/* A scrolling track (texture_update_anims's sub list, 16 bytes each); the name is
 * the C++ symbols' (texture_update_scroll__FP14texture_scrollP14obj_textureset). */
typedef struct texture_scroll {
    s32 idx;        /* 0x00: entry in obj_textureset.tex */
    s32 speed;      /* 0x04 */
    s32 pos;        /* 0x08: 24.8 */
    s16 *dst;       /* 0x0C: x, y */
} texture_scroll;




typedef union {
    u16 h;
    u8 b[2];
    struct { u16 u : 8; u16 v : 8; } f;
} TexUV;

extern s32 GameLanguage;

/* A viewpoint: its position (+0xC) is subtracted from world positions before
 * they go through the GTE (view_project_volume, view_project). */

typedef struct {
    s32 v[3];
} CamVec3;

/* A 0x20-byte block play_script copies whole. */
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

s32 interp(s32 arg0, s32 arg1, s32 arg2, s32 arg3);



/* A 0x4C-byte animation key of the track update_entity_thread walks (at track+0x30). */
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

/* A 0x50-byte key of the object track update_model_thread walks (from track + 0x24);
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


/* The pose update_entity_thread writes (at obj+8). */
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





/* Bounding box tested for visibility by bbox_visible: corners min/max and a
 * centre whose pad is a radius. */
typedef struct {
    s16 min[3];     /* 0x00 */
    s16 max[3];     /* 0x06 */
    SVECTOR center; /* 0x0C: center.pad = radius */
} BoundBox;


/* A block holding a table at a byte offset from its field at +0x20. */


/* A 0x28-byte primitive slot of faces_write_colors_dq: a textured triangle, or (run flag
 * bit 0) a draw-mode packet followed by an untextured gouraud triangle. */
typedef union {
    POLY_GT3 gt;
    struct {
        DR_TPAGE tp;
        POLY_G3 g;
    } g;
} FacePrim;

/* A mesh's header in a model file (docs/ASSETS.md, Model files): the header table's 0x34-byte
 * records (ModelFile.meshes) and the entities'. Offsets are counted from their own field. */
typedef struct MeshHeader {
    /* 0x00 */ struct PrimSet *set; /* its prims while drawn (primset_alloc), marked dead on release */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 faces;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s32 verts;   /* a bounding box, then s16 x, y, z and a u16 a vertex */
    /* 0x14 */ s32 runs;    /* the strips: a flags byte and a count byte each, ended by count 0xFF */
    /* 0x18 */ s32 uvIdx;   /* a u16 UV table index a face */
    /* 0x1C */ s32 texs;    /* texture runs: bits 9-14 repeat count, bit 15 last texture, bits 0-8 texture */
    /* 0x20 */ s32 colors;  /* a u16 a face: its first colour, bit 15 semi-transparent, bits 13-14 blend */
    /* 0x24 */ s32 end;
    /* 0x28 */ s32 normals; /* 0: none */
    /* 0x2C */ s32 points;  /* a PointList (sub_point_list), 0: none */
    /* 0x30 */ u32 unk30;
} MeshHeader;

/* A model file's header (docs/ASSETS.md, Model files): the offsets are counted from their own
 * field. A model id names a part of it: 0x2000 | i mesh i (from 1), 0x4000 | n << 7 | f frame f
 * of mesh animation n, 0x5000 | i entity i (from 1). */
typedef struct ModelFile {
    /* 0x00 */ u32 tag;        /* 0x0C160029 */
    /* 0x04 */ u32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 mapAnims;   /* the map's animations (map_enviroment.map_animations, from 8 past it) */
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 maps;       /* a map_list: the level's maps (an arena's) */
    /* 0x1C */ s32 entities;   /* the map directory: s32 count, then ModelEntity each */
    /* 0x20 */ s32 colors;     /* u32 RGB + code, 3 a face */
    /* 0x24 */ s32 uvs;        /* u16 u | v << 8 */
    /* 0x28 */ s32 verts;      /* the animations' default vertex pool */
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u32 unk30[3];
    /* 0x3C */ s32 anims;      /* s32 count, then DbaseAnim: chunks loaded from the file on demand */
    /* 0x40 */ s32 nMeshAnims;
    /* 0x44 */ s32 meshAnims;  /* FileEntry each (by name hash) */
    /* 0x48 */ s32 nScripts;
    /* 0x4C */ s32 scripts;    /* s32 each, from itself to a script_header */
    /* 0x50 */ u32 size;       /* the part kept in memory; on-demand data starts at the next 2 KB sector */
    /* 0x54 */ s32 nMeshes;
    /* 0x58 */ MeshHeader meshes[1];
} ModelFile;

/* ModelFile.entities: a count, then one of these each. */
typedef struct ModelEntity {
    /* 0x00 */ s32 mesh;  /* from here to its MeshHeader, 0: none */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 group; /* entry_block_value */
} ModelEntity;

typedef struct EntityTable {
    s32 count;
    ModelEntity e[1];
} EntityTable;

/* A frame of a mesh animation (FileEntry's data: an s32, then 16 bytes a frame). */
typedef struct MeshAnimFrame {
    /* 0x00 */ s32 list;   /* from here to an index list */
    /* 0x04 */ s32 list2;  /* from here to the one it blends towards */
    /* 0x08 */ s32 blend;  /* 0..0x1000, 0: a keyframe */
    /* 0x0C */ s32 points; /* from here to a PointList, 0: none */
} MeshAnimFrame;

/* A texture entry's VRAM placement and the primitive words drawn with it: its own
 * struct at obj_texture +0x18 (prim_create_entity_single and Pogo's psxpogo_render_tile
 * take a pointer to it). */
typedef struct GfxTexInfo {
    /* 0x00 */ s16 vramX;    /* its VRAM rect: x, y, vramW, vramH */
    /* 0x02 */ s16 vramY;
    /* 0x04 */ u16 vramW;    /* in VRAM units */
    /* 0x06 */ s16 vramH;
    /* 0x08 */ struct FreeHdr *vram; /* its VRAM node (vram_take), freed with vram_free_node */
    /* 0x0C */ u16 tpage;
    /* 0x0E */ u16 clut;
    /* 0x10 */ u16 uv0;      /* ORed into each vertex's uv */
    /* 0x12 */ u16 uv1;      /* uv of vertices 1-3 */
    /* 0x14 */ u16 uv2;
    /* 0x16 */ u16 uv3;
} GfxTexInfo;





/* An object drawn as one textured quad (draw_model_quad, sub_use_quad_model,
 * kill_sprim_sub): one primitive per frame buffer. */
typedef struct {
    POLY_GT4 *prim; /* from new_sprim, freed with release_sprim */
    u8 unk4[8];
} ModelObjSlot;

typedef struct {
    u32 flags;               /* 0x00: 0x8000 visible */
    u8 unk4[0x68];
    struct obj_dbase *model; /* 0x6C */
    u8 unk70[4];
    u16 index;               /* 0x74: passed to entity_address */
    u8 unk76[0x16];
    u16 unk8C;               /* 0x8C: clut to set on the next draw */
    u16 unk8E;               /* 0x8E: texture index to set on the next draw */
    ModelObjSlot slots[2];  /* 0x90: indexed by FrameCount & 1 */
} ModelObj;


/* The block at GfxState+0x28 (passed to gfx_setup_xform). */
typedef struct {
    s32 x, y;  /* 0x0: screen offset */
    u16 otz;   /* 0x8: becomes GfxState.otzAdd */
} GfxScreen;

/* Graphics state at 0x80056998 (placeholder name; 161 references). Only the
 * fields gfx_load_xform and project_quad use are known: two pointers it aims
 * at buffers inside the struct itself, a block at +0x28 it passes to
 * gfx_setup_xform, and the ordering table that primitives are added to. */
typedef struct {
    struct obj_dbase *dbase; /* 0x00: the dbase being drawn */
    void *unk4;
    s32 *farColor; /* 0x08: r, g, b for gte_ldfcdir */
    s32 dp;        /* 0x0C: depth-cue factor for gte_lddp */
    void *viewport; /* 0x10: GfxViewport (gfx_set_viewport) */
    s32 modelDp;   /* 0x14: dp for ModelObj quads (draw_model_quad); the screen fade */
    u8 unk18[4];
    s16 clipX1;    /* 0x1C: viewport clip right/bottom on screen */
    s16 clipY1;
    s32 ofx;   /* 0x20: geometry offset x (scaled by the screen width / 640) */
    s32 ofy;   /* 0x24: geometry offset y (halved) */
    u8 unk28[0xC]; /* 0x28: a GfxScreen */
    SVECTOR gteIn; /* 0x34: GTE scratch, a position relative to the view (view_project_volume) */
    u8 unk3C[4];
    u32 *ot;   /* 0x40 */
    s16 otzAdd; /* 0x44: added to the average Z before it is halved */
    s16 otLen;  /* 0x46: entries in ot */
    struct camera_info_tag *cam; /* 0x48 */
    u8 unk4C[0x28];
    VECTOR gteOut; /* 0x74: gteIn rotated and translated */
    u8 *unk84; /* 0x84: = unkCC */
    u8 *unk88; /* 0x88: = unk10C */
    MATRIX unk8C; /* 0x8C: set to identity by draw_init */
    MATRIX unkAC; /* 0xAC: object's local-to-screen matrix (gfx_object_matrix) */
    u8 unkCC[0x40];
    u8 unk10C[0x20]; /* the struct ends at 0x12C: PrimDepthLimit is the next global */
} GfxState;

/* A 0x1C-byte entry of a sorted table at +0x4C of a block (key_find). */
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
 * gte_stsxy3_* macros uses these offsets; draw_verts and draw_gouraud_quad
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
    s32 unk2C;               /* 0x2C: -1 when allocated; sfx_key_off stops it */
    s32 unk30;
    s32 id;                  /* 0x34: the SfxTable id */
};
BASH_SIZE_CHECK(_sfx_handler, 0x38);

/* 24 voices (sfx_voices_init links them into the free list). */
typedef struct {
    _sfx_handler voices[24];
    _sfx_handler *free;         /* 0x540 */
    _sfx_handler *active;       /* 0x544 */
} SfxPool;

/* A VAB file in memory: the header and body as byte offsets from its start
 * (vab_loaded opens it with SsVabOpenHead / SsVabTransBody). */
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

/* Points drawn as one-pixel tiles by plot_point_sprites (an engine API entry). */
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

/* Filled by gfx_setup_xform for gfx_load_xform, which loads it into the GTE. */
typedef struct {
    MATRIX *rot;
    VECTOR *trans; /* may be NULL: translation left as it is */
    s16 ofx;       /* scaled by the screen width / 640 */
    s16 ofy;       /* halved */
} GteSetup;

/* Returned by sub_point_list: points with flag bits (sub_find_point picks the
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

/* A mesh's vertex block (MeshHeader.verts): its bounds, then the vertices. */
typedef struct MeshVerts {
    BoundBox box;
    FlagPoint v[1];
} MeshVerts;

/* Header of a heap block, 0x18 bytes before the pointer the allocator hands
 * out; the next block follows its size words of data. Free blocks are on the
 * heap's free list, sorted by size (mal_free frees and merges). The names
 * mal and _mal_area are the C++ symbols' (mal_unlink__FP3malP9_mal_area). */
typedef struct mal {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 ticks;                /* frames left on the deferred list (mal_free_later) */
    /* 0x04 */ void **owner;             /* cleared on free; (void **)-1 for none; NULL when free */
    /* 0x08 */ u32 size;                 /* in words, header not included */
    /* 0x0C */ struct mal *prev;    /* the block before it in memory */
    /* 0x10 */ struct mal *prevFree;
    /* 0x14 */ struct mal *nextFree;
} mal;

#define MEMBLOCK_NEXT(b) ((mal *)((u8 *)(b) + (b)->size * 4 + sizeof(mal)))
/* The block after B if B held WORDS words, written as the allocator sums it (the words first). */
#define MAL_AFTER(b, words) ((mal *)((u8 *)(b) + ((words) * 4 + sizeof(mal))))
/* B's data words, just after its header, and the last of WORDS of them. */
#define MAL_DATA(b) ((s32 *)((u8 *)(b) + sizeof(mal)))
#define MAL_LAST(b, words) ((u32 *)((u8 *)(b) + ((words) * 4 + sizeof(mal) - 4)))
/* The same sums left to right (the pointer first), and back from a block's last word. */
#define MAL_END(b, words) ((mal *)((u8 *)(b) + (words) * 4 + sizeof(mal)))
#define MAL_OF_LAST(last, words) ((mal *)((u8 *)(last) - (words) * 4 - (sizeof(mal) - 4)))

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
    void *mem;                        /* heap block, freed by primpool_free */
    PoolNode *done;
} NodePool;

/* Data with a table of self-relative offsets at +0x1C: entry i points to
 * &offs[i] + offs[i] (offset_table_entry). */
typedef struct OffsetTable {
    /* 0x00 */ u8 unk0[0x1C];
    /* 0x1C */ s32 offs[1];
} OffsetTable;

/* 0x18-byte entry of an id-keyed table ended by id 0x3E7 (FindIdEntry). */
typedef struct IdEntry {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 unk2[0x16];
} IdEntry;

/* A pad's motors (PadState.rumble; pad_set_rumble_big and _small, pad_rumble_send). */
typedef struct PadRumble {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 act[2];  /* motor values passed to PadSetAct */
    /* 0x06 */ u16 dirty;
    /* 0x08 */ s16 time0;
    /* 0x0A */ s16 time1;
    /* 0x0C */ s16 reset;
    /* 0x0E */ s16 on;      /* PlayersMenuUpdate: the pad's player takes part */
} PadRumble;
/* Per-pad state, 8 of them at Pads (pads_init_mtap resets them; pad_read fills it). */
typedef struct PadState {
    /* 0x00 */ s32 flags;          /* 1: connected */
    /* 0x04 */ s32 buttons;
    /* 0x08 */ s32 rx;             /* sticks, 0..0xFF (the d-pad's 0, 0x80, 0xFF without) */
    /* 0x0C */ s32 ry;
    /* 0x10 */ s32 lx;
    /* 0x14 */ s32 ly;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ PadRumble rumble;
    /* 0x2C */ s32 forced;         /* set_joypad: 1 */
    /* 0x30 */ s32 forcedButtons;  /* set_joypad: the buttons */
    /* 0x34 */ s32 unk34;
    /* 0x38 */ union { s32 w; s16 lo; } angle;  /* the left stick's (some games read the low half) */
    /* 0x3C */ s32 dist;
    /* 0x40 */ s32 lastHeld;       /* the buttons, for the next frame's edges */
    /* 0x44 */ s32 held;           /* buttons down */
    /* 0x48 */ s32 pressed;        /* buttons down this frame and not the last */
    /* 0x4C */ s32 lastDir;
    /* 0x50 */ s32 dir;            /* the d-pad, or the stick as one */
    /* 0x54 */ s32 dirPressed;
} PadState;

/* Loaded sound data and the sequence opened from it (CurrentMusic); the name is
 * the C++ symbols' (music_dload_midi__FP4fileP10music_midil). */
typedef struct music_midi {
    /* 0x00 */ s32 data;                 /* heap block, freed by music_free */
    /* 0x04 */ s32 seq;                  /* SsSeq access number, or -1 */
    /* 0x08 */ s32 songs[5];             /* each song's SEQ data: data + its header offset (music_setup_slot) */
    /* 0x1C */ s32 unk1C;                /* set to -1 on close */
} music_midi;

/* Set both channels of slot's sequence volume (music_update). */

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

/* MusicTable: what music_load_track passes to music_load for an index. */
typedef struct MusicEntry {
    /* 0x00 */ s32 file;
    /* 0x04 */ s16 reverb;               /* SsUtSetReverbType */
    /* 0x06 */ u8 unk6[2];
    /* 0x08 */ s32 unk8;                 /* music_play_level: music file */
    /* 0x0C */ s32 unkC;                 /* music_play_level: alt music file (flag 0x80) */
    /* 0x10 */ s16 tempo;
    /* 0x12 */ u8 unk12[0x14 - 0x12];
} MusicEntry;

/* Loader: file loader state (loader_reset resets it), with its queue of loads
 * (loadlist_push / loadlist_pop, nodes from LoadNodePool). */
typedef struct LoaderState {
    /* 0x00 */ s32 state;                /* 3 after a reset */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ struct LoadNode *head;
    /* 0x10 */ struct LoadNode *tail;
} LoaderState;

/* A process (a game mode), its control block and the screen fade, from the
 * Jampack Winter 2000 demo's debug types (docs/PROTOTYPES.md).
 * A process's init runs when it starts (process_control_update), update every frame,
 * render from process_control_render and quit when it ends; a non-NULL heap resets the
 * heap to start there. */
typedef struct process_info {
    /* 0x00 */ void (*init)(void);
    /* 0x04 */ void (*update)(void);
    /* 0x08 */ void (*render)(void);
    /* 0x0C */ void (*quit)(void);
    /* 0x10 */ u32 *heap;
} process_info;
BASH_SIZE_CHECK(process_info, 0x14);

/* process_control_update switches to next_process once the fade is done. */
typedef struct process_control_info {
    /* 0x00 */ process_info *process;
    /* 0x04 */ process_info *next_process;
    /* 0x08 */ process_info *last_process;
    /* 0x0C */ s32 flags;   /* 1: skip quit, 2: skip init, 4: skip render, 8: skip update, 0x10: clear after a switch */
} process_control_info;
BASH_SIZE_CHECK(process_control_info, 0x10);

/* amount steps by speed towards 0x1000 or 0; maxed counts the frames at the
 * end (fade_update). */
typedef struct fade_info {
    /* 0x00 */ s32 amount;
    /* 0x04 */ s32 speed;
    /* 0x08 */ s32 maxed;
    /* 0x0C */ s32 delay;
} fade_info;
BASH_SIZE_CHECK(fade_info, 0x10);

typedef struct LoadNode {
    /* 0x00 */ s32 *file;                /* archive entry {sector, size}; NULL once cancelled mid-load (loader_cancel) */
    /* 0x04 */ s32 offset;               /* bytes into the file */
    /* 0x08 */ s32 size;                 /* bytes */
    /* 0x0C */ s32 mode;                 /* 1: mal_set_owner after the load */
    /* 0x10 */ void (*fn)(u8 *, u8 *);   /* called with (buffer, arg) when done */
    /* 0x14 */ u8 *arg;
    /* 0x18 */ u8 *buf;                  /* preset buffer (loader_alloc) */
    /* 0x1C */ struct LoadNode *next;
    /* 0x20 */ struct LoadNode *prev;
} LoadNode;

/* A heap (MainHeap is the main one). */
typedef struct _mal_area {
    /* 0x00 */ mal *first;          /* its first block (mal_init) */
    /* 0x04 */ mal *end;            /* the zero-size block that ends it */
    /* 0x08 */ mal *free;           /* free list, smallest first */
    /* 0x0C */ mal *lowest;         /* lowest free block, or NULL */
    /* 0x10 */ mal *deferred;       /* freed once unk2 ticks run out (mal_free_later, mal_update_deferred) */
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

/* A node from the free list smallnode_alloc hands out (smallnode_free returns
 * it): the heap block it owns and the load filling it. */
typedef struct BlockRef {
    /* 0x00 */ u8 *ptr;
    /* 0x04 */ s32 load;
    /* 0x08 */ struct BlockRef *next;   /* on the free list */
} BlockRef;

/* An entry of the 0x18-byte table at +0x44 (+ offset at +0x44) of a file
 * loaded by dbase_asload_with_anims_callback (dbase_setup_blocks). */
typedef struct FileEntry {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
    /* 0x08 */ s32 frames;
    /* 0x0C */ s32 mesh;   /* from here to the MeshHeader it moves */
    /* 0x10 */ s32 hash;   /* string_hash of its name */
    /* 0x14 */ BlockRef *block;
} FileEntry;

/* A per-frame-buffer allocation slot used by sub_alloc_prims. */
typedef struct {
    /* 0x0 */ void *ptr;
    /* 0x4 */ u16 size;
} Func8001CC10Slot;

void mal_print_free(void);
s32 mal_is_unowned(void *p);
void mal_disown(void *p);
void mal_set_owner(void *p, void **owner);
s16 mal_get_flags(void *p);
u8 *mal_alloc(s32 arg0, _mal_area *arg1);
u8 *mal_alloc_nocompact(s32 arg0, _mal_area *arg1);
void mal_unlink(mal *blk, _mal_area *area);
void mal_compact_ptr(s32 arg0);
void mal_compact_step(_mal_area *heap);
u8 *mal_update_deferred(_mal_area *arg0);
void sortlist_sort(s32 arg0);
void loader_unused_a(void);
void smallnode_free(BlockRef *node);
void loader_cancel(LoadNode *arg0);
s32 string_hash(u8 *s);
s32 load_file_cb(s32 arg0, void (*done)(), void *ctx);
s32 loader_pending(void);
s32 loader_update(void);
void mal_compact(_mal_area *arg0);
mal *mal_first_free(u32 words, mal *blk);
s32 loader_queue(s32 arg0, s32 arg1, s32 arg2, void (*done)(u8 *, u8 *), u8 *arg4, s32 arg5, s32 arg6);
s32 asload_permanent_callback(s32 arg0, void (*done)(), void *ctx);
void loader_unused_b(void);
void loadnode_free(LoadNode *node);
struct LoadNode *loadlist_push(LoaderState *ls);
struct LoadNode *loadlist_head(LoaderState *ls);
void set_joypad(PadState *pad, u32 buttons);
void cam_init(camera_info_tag *cam);
void cam_ramp_override(s32 arg0, s32 arg1);
s32 *get_exe_exports(void);
s32 angle_diff_abs(s32 arg0, s32 arg1);
s32 angle_diff(s32 arg0, s32 arg1);
void SetRandomSeed(s32 arg0, s32 arg1);
/* An effect run on a subengine (fx_alloc_buffers): the buffers it needs, and its Init (called
 * with the subengine) and the subengine's hook. The weather's, the hub's sparks, N2O's stars. */
typedef struct FxDef {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 size;              /* bytes of each of the two state buffers */
    /* 0x08 */ s32 bufSize;           /* bytes of the work buffer */
    /* 0x0C */ void (*init)();
    /* 0x10 */ void (*hook)();
} FxDef;

/* An effect's view of its subengine's +0x6C (subengine.u6C on): the buffers fx_alloc_buffers
 * takes and fx_free_buffers gives back. */
typedef struct SubFx {
    /* 0x00 */ void *buf;
    /* 0x04 */ void *a;
    /* 0x08 */ void *b;
    /* 0x0C */ s32 count;
} SubFx;

void fx_free_buffers(subengine *sub);
s32 fx_alloc_buffers_b(FxDef *def, SubFx *fx);
void quat_to_matrix(SVECTOR *rot, MATRIX *m);
void rotscale_update(void *arg0);
PointList *sub_point_list(subengine *sub);
FlagPoint *sub_find_point(EXTengine *eng, subengine *sub, u16 flag, VECTOR *out);

extern SVECTOR MorphVerts[];
typedef struct { s16 unk0; s16 unk2; } TrigEntry;
extern TrigEntry rcossin_tbl[]; /* libgte's sine/cosine table (cstbl.o) */
/* A 4.12 fixed-point quaternion (quat_slerp slerps two of them). */
typedef struct { s32 v[4]; } Quat;
extern s32 SfxDistanceCurve[];
extern s32 SavedSp;
extern s32 LoadPermanentBuf;
extern u8 ExportTable[];
extern s32 *LangModeTypeTexts[];
extern s32 SoundCritical;
extern s32 MusicNext;
extern s32 MusicCur;
extern s32 MusicStopDelay;
extern s32 MusicVolCur;
extern s32 MusicDuck;
extern BlockRef *SmallNodeFree;
/* A module's entry table, the first bytes of its image: the engine's (entry.c) is what the
 * shell runs (EngineModule), and RunModules runs one until it hands over (CurrentModule). */
typedef struct ModuleEntry {
    void (*init)(void);
    void (*update)(void);
    void (*render)(void);
    void (*quit)(void);
    u8 *heap; /* where the heap starts once it is loaded */
} ModuleEntry;
s32 rand_step(void);
extern ModuleEntry *EngineModule;
extern ModuleEntry *CurrentModule;
extern s32 VSyncRate;
extern s32 LastFlipVSync;
void mal_update(_mal_area *heap);
extern u8 HeapStart[];
mal *mal_init(_mal_area *heap, u8 *start, s32 size);
extern ModuleEntry ShellModules;
s32 load_file_direct(s32 *file, u8 *buf);
void init_file(void);
extern s32 DcacheReady;
extern void *LoadNodeFree;
extern BlockRef SmallNodePool[]; /* 63 of them, free list in SmallNodeFree */
extern u8 LoadNodePool[]; /* 63 x 0x24 pool, free list in LoadNodeFree */
extern s32 cam_ramp_overide_count;
extern s32 CamEaseOverride;
extern s32 RandomSeedA;
extern s32 RandomSeedB;
extern script_thread_handler *ScriptList;
/* What each kind of script thread runs (ScriptCommands, by thread_type): init (the thread's
 * definition, the script's control info, the thread's number), update, render (the thread,
 * the render setup), and the release, if it has one. */
typedef struct ScriptCommand {
    void (*init)();
    void (*update)(script_thread_handler *, script_control_info *);
    void (*render)();
    void (*kill)();
} ScriptCommand;
extern ScriptCommand ScriptCommands[];
extern s32 World_floor_level;
extern s32 MusicRestart;
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
extern char *CardSaveTitle;
extern u8 CardIconFrames[]; /* the header's icon frames (CardBuffer + 0x80) */
extern const char CardSaveTitleFormat[];
s32 count_completion(u8 *arg0);

/* Writes up to 32 Shift-JIS characters of src (ASCII) to dst, padding with
 * full-width spaces (card_make_header). */
extern s32 CardSaveSum;
extern s32 CardRequest;
extern s32 CardCommand;
extern s32 CardDirCount;

/* 0x28-byte entries starting at 0x800675D4; CardDirCount is the count. */
typedef struct {
    u8 unk0[0x18];
    s32 size; /* 0x18 */
    u8 unk1C[0xC];
} SizeEntry;
extern SizeEntry CardDir[];
extern const char CardFileName[];
extern s32 CardSyncCmd;
extern s32 CardSyncResult;

/* The directory entry named CardFileName, or NULL (card_find_file). */

/* Clears the save's slots and flags and re-sums it (card_clear_slots). */

/* McResetSave with the checksum in its own variable (allocates it apart from i). */

/* Zeroes the 0x2000-byte save buffer. */

/* Free bytes on the card (card_count_files). */
extern _mal_area MainHeap;
extern s32 LoadDirectBusy;
extern u8 *LoadWaitBuf;
extern NAS *HeadOfCameras;
extern NAS *HeadOfObjects;
extern music_midi CurrentMusic;
extern LoaderState Loader;
extern const char MsgTooManyLoadsInQueue[];
void load_dbase_done(u8 *arg0, ModeDbase *a1);
void load_group_done(u8 *arg0, file_group_load_info *a1);
extern MusicEntry MusicTable[];
extern sfx_machine_bank_info SfxBanks[6];
extern NodePool PrimPool;
extern NAS *HeadOfSolids;
extern NAS *ContourPlatform;
extern void *HeadOfMissiles;            /* tail of the HeadOfObjects list */
extern void *HeadOfUnused;            /* tail of the HeadOfSolids list */
extern s32 VelocityCalc;
extern s32 ACMD[16][2];       /* entries 1..15 cleared by ResetNasLists */
extern s32 PadCount;
extern s32 first_joypad;            /* first pad with flag 1 set, or -1 */
extern s32 PadRunEnd;            /* pads 0..n-1 all have flag 1 */
extern s32 joypad_idle_count;            /* frames with no button down */
extern u8 PadMtapBuf[2][0x34];    /* PadInitMtap buffers */
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

/* vram_init's VRAM areas: a list head and the area's position; x = -1
 * ends the list. */
typedef struct {
    BufNode **head;
    FreeHdr hdr;
} VramArea;
extern u8 *VramBufNodes; /* the BufNode pool (vram_init) */
typedef struct FreePool {
    FreeNode *head;
    u16 w;
    u16 pad6;
    u16 h;
    u16 padA;
} FreePool;
extern FreeNode *FreeBufNodes;
extern u8 VramSmallReq[];
extern u8 VramReqEnd[];
extern FreeNode *ClutFreeSmall;
extern FreeNode *ClutUsedSmall;
extern FreeNode *ClutLargeList;    /* the large cluts' free list (clut_alloc_large) */
extern s32 MalFreeBytes;            /* mal_count_free: free bytes */
extern s32 MalFreeBlocks;            /* mal_count_free: free blocks */
extern s32 DatIndex[];         /* the archive's {sector, size} entries, file N at [2 * N] */
extern s32 MalLargestFree;            /* mal_count_free: largest free block, bytes */
extern s32 SfxBankCount;            /* slots of SfxBanks in use */
extern char SsTable[];
/* A save slot as the card menus show it (CardSlots): the heroes picked and out (GameInfo +0x4C
 * and +0x5C), then the saved game state. */
typedef struct CardSlot {
    u8 summary[8];
    GameState state;
} CardSlot;
extern CardSlot CardSlots[];
/* A panel's record (0x24 bytes; PlaceEngineSlot's lists, ending at a type below 2), read by
 * ApplyPlaceRecs: what to place (type), where, and its text table or image (data). The pause
 * screens' records and the text panels' items are the same record. */
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
    /* 0x14 */ void *data;   /* a text table (by language), or a sprite's image */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20;
} PlaceRec;
extern u8 GameInfo[];
extern AdventureState GameAdventure;   /* GameInfo + 0x70 */
extern u8 GamePlayers;           /* GameInfo + 0x26: the players in play (views: numPlayers) */
extern s8 GameArena;             /* GameInfo + 0x36: the BattleArenas entry */
extern s16 GameTeamLeader[2];    /* GameInfo + 0x48: each team's leading hero */
extern u8 GameHeroOut[4];        /* GameInfo + 0x5C: each hero out of the game */
extern u8 GameWinner[2];         /* GameInfo + 0x60: the winners' hero names */

extern s32 DirPadBits[24];         /* pad direction bits by angle octant */
extern void *HeadOfBalls2;       /* reset with HeadOfBalls (Arkenoid, N2O) */
extern u16 ModeTypes[];          /* music_play_level: 6-byte entries indexed by GameInfo+0x34 */
/* ModeTypes as 6-byte entries indexed by level (SetupLevelSpecs). */
typedef struct LevelInfo {
    /* 0x00 */ u16 world;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
} LevelInfo;
/* An arena definition (SetupLevelSpecs): 0x18 bytes, a table ends at id 999. */
typedef struct ArenaDef {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 nasSlots;       /* NasPoolSlots, less the 4 heroes */
    /* 0x04 */ u16 firstNas;       /* FirstNas, less 4 */
    /* 0x06 */ u16 nasHeapKB;      /* NasHeapSize in KB */
    /* 0x08 */ u16 subPool;        /* SubPoolCount */
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ void *triggers;     /* LevelTriggerData */
    /* 0x10 */ s32 config;         /* LevelConfig */
    /* 0x14 */ s32 boot;           /* GameState.boot */
} ArenaDef;
extern LevSpec LevelSpecs; /* the level's hooks */
extern s32 NasHeapSize;
extern s32 SubPoolCount;
extern s32 LevelConfig;
extern const char MsgFatalMetricsError[];         /* "FATAL METRICS ERROR ..." */
extern s32 LevelSfxBanks[][8];       /* music_play_level */
extern GameOptionSet GameOptions;
extern s32 card_command_delay;
extern s32 GameFrames;
extern s32 GlobalNAStatus;
extern camera_info_tag MainCamera;
extern s16 GameMode;
extern u16 GameLevelFlags;
extern s32 FontPage;
extern s8 SysFontWidth[];
extern u8 SysFontFlags[];
extern obj_dbase *sysfont_dbase;
extern s16 SysFontLookUp[];
extern s32 NasHeapMem;
extern u32 NasHeapBytes;
extern s32 ExtHeap[];
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
extern ExtBlk *ExtFreeList;
extern VabFile *VabLoading;  /* VAB being transferred to the SPU, or NULL */
extern sfx_machine_bank_info *SfxBankLoading;  /* where its id goes */
extern s32 VabHeadId;       /* the id from SsVabOpenHead */
extern u8 SpuKeyStatus[];
extern s32 xa_count;
extern s32 SfxVoiceMask;
extern s32 SfxVoiceBits[];       /* voice bit masks, by voice */
extern s32 MusicDuckCur;
extern s32 ASTCount;            /* entries in ASTTable */
extern ASTEntry *ASTTable;      /* the AST table in use */
extern s32 SubPool;
extern SfxPool SfxVoicePool;
extern s8 SfxTable[][2];
extern s32 CdReadStatus;

/* 32-byte records of LevelRecs, indexed by LevelRecIndex (SetLevelCameraMode). */
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 heroLoadArg;  /* passed to queue_dbase_load by QueueHeroModels */
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ u16 cameraMode;  /* CameraMode for the level (SetLevelCameraMode) */
    /* 0x0E */ u8 unkE[0x12];
} EngineRec32;
extern EngineRec32 *LevelRecs;
extern s32 ArchiveSector;
extern u8 CdControlResult[8];
extern CdlLOC CdReadLoc;
extern NASTrigger *HeadOfTriggers;
extern s32 TriggerCount;
extern script_handler *active_scripts;
extern RECT VramRect;
extern script_handler *free_scripts;
extern map_enviroment *MapScene;
/* A map trigger record (InitTriggers): position, then a word whose low 2
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
/* The level's trigger file (LevelTriggerData): its records (LevelRecs) and its trigger table. */
typedef struct LevelTriggerHead {
    /* 0x00 */ s16 *unk0;
    /* 0x04 */ EngineRec32 *recs;
    /* 0x08 */ U16Table *tbl;
} LevelTriggerHead;
extern struct LevelTriggerHead *LevelTriggerData;
extern MapPoint MapPoints[32];
/* A button-combo sequence: steps[] are pad masks, 0-terminated. */
typedef struct ComboSeq {
    /* 0x00 */ void (*fn)(void);
    /* 0x04 */ s32 maxTime;
    /* 0x08 */ s32 step;
    /* 0x0C */ s32 startTime;
    /* 0x10 */ s32 steps[32];    /* 0-terminated */
} ComboSeq;

extern MATRIX MapSceneMatrix;
extern SVECTOR MapSceneRot;
extern s32 ComboSeqs[]; /* ComboSeq table, 0x90 bytes per entry */
extern CdlLOC CdSeekLoc;
extern RECT *ScreenRect;
extern s32 ScreenSetFrame;
extern s32 ScreenSetFlag;
extern s32 FntLoadPatch;
extern u32 DebugFontClutData[];
extern s32 DebugFontStream;
extern s32 DebugFontTPage;
extern s32 DebugFontClut;
void *mal_trim_front(void *base, s32 offset, _mal_area *arg2);
void cam_build_view(camera_info_tag *cam, RECT *rect);
extern GfxState RenderState;
extern VECTOR ScreenTrans; /* screen-space translation (gfx_setup_xform) */

/* The current draw buffer: primitives are allocated from the pointer at
 * +0x4008 (texture_draw). */
/* A frame's draw buffer (DrawBuffers, two; cdb is the one being built): its ordering table,
 * drawn from the last entry (DrawOTag), and its primitive area (AllocPrimBuffers). */
typedef struct DrawBuffer {
    /* 0x0000 */ u32 ot[0x1000];
    /* 0x4000 */ u8 *primStart;
    /* 0x4004 */ u8 *primEnd;
    /* 0x4008 */ u8 *prim;        /* the next free primitive */
} DrawBuffer;
extern DrawBuffer *cdb;

/* A screen offset pair, copied as one struct. */
typedef struct {
    s32 x, y;
} GfxOffset;

/* A viewport gfx_set_viewport makes current (coordinates in 640-wide units). */
typedef struct {
    s16 x0, y0, x1, y1;     /* 0x00: draw area */
    s16 cx0, cy0, cx1, cy1; /* 0x08: clip area (cam_build_view) */
    s32 ofx, ofy;           /* 0x10: geometry offset */
    s32 otIdx;              /* 0x18: first ot entry in cdb */
    s32 otLen;              /* 0x1C */
    s32 unk20;
    UBYTE_RGB bg;           /* 0x24: background (the map's clear colour) */
    u8 isbg;                /* 0x27: clear to it (the map's clear) */
} GfxViewport;




/* A VRAM slot class (VramSlots). */
typedef struct { u8 pad[0xC]; } Slot12;
/* A texture: the 0x38-byte entries of a texture set's table, which
 * texture_draw draws as one POLY_FT4 (the name is the C++ symbols',
 * texture_draw__FP11obj_textureG4xy_wiUl). */
typedef struct obj_texture {
    /* 0x00 */ u16 flag;     /* 1: 8-bit (texture_init_entry) */
    /* 0x02 */ u16 clut;     /* its clut: an entry in the set's ent, 0x7FFF none */
    /* 0x04 */ Slot12 *slot; /* its VRAM slot class */
    /* 0x08 */ s16 w;        /* in pixels */
    /* 0x0A */ s16 h;
    /* 0x0C */ u32 fC;
    /* 0x10 */ u8 advance;   /* w - 1; returned scaled to the screen width */
    /* 0x11 */ u8 dv;        /* h - 1 */
    /* 0x12 */ u16 flags;     /* 2: one frame per language from here (texture_upload_anim) */
    /* 0x14 */ s32 frames;
    /* 0x18 */ GfxTexInfo info;
    /* 0x30 */ s32 pix;      /* its pixels: an offset into the set's base */
    /* 0x34 */ s32 time;     /* FrameCount when last used, -1 when loaded (texanims_release) */
} obj_texture;

/* A screen position as one word (x low, y high). */
typedef union {
    s32 w;
    struct {
        s16 x, y;
    } s;
} ScreenXY;

/* The same packed position built with bitfields (DrawSpriteTriple). */
typedef union {
    struct {
        s32 x : 16;
        s32 y : 16;
    } f;
    s32 w;
} SprPos;


/* calc_rgbcode: fades r, g, b towards black by RenderState.modelDp / 0x1000. */
/* FadeColor reading the fade from gs->modelDp (texture_draw_3d). */

typedef struct {
    struct {
        s16 x;
        u16 y;
        u16 z;
        u16 pad;
    } v[4];
    s32 c[4];
} QuadVerts;

/* draw_gouraud_quad's input: four vertices and four colours (bytes r, g, b). */
typedef struct {
    SVECTOR v[4];
    u8 c[4][4];
} ColorQuad;

/* draw_gouraud_quad's primitive: a draw-mode word, then a POLY_G4 sharing the
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
extern s32 MalMoveVSyncs; /* VSync ticks spent compacting the heap (mal_update) */
extern s32 MalMoveWords; /* words moved compacting the heap (mal_update) */
extern s32 PrimDepthLimit; /* draw_object_model: the object's +0x8A */
extern s32 PrimDepthTest;    /* model_prims_begin: with PrimDepthLimit, forces the depth-cued path */
extern s32 FarColor[3]; /* model_prims_begin: default far colour */

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
/* A step of a kind 2 animator's Atable (DoAnimCommand2): cmd >= 0 is a frame
 * shown for arg ticks, 0x8000 | n is command n with operand arg. */
typedef struct AnimStep {
    s16 cmd;
    u16 arg;
} AnimStep;
extern s32 ContourHeight;
extern void (*SsTickSaved)();
extern NAS *NASActiveTail;
extern s32 *PrimNodeFree;
extern s32 *PrimsConfig;            /* pool sizes (prims_init) */
extern u8 *PrimSetPool;             /* 0x58-byte nodes (prims_init) */
extern u8 *PrimNodePool;             /* 0x54-byte nodes, free list in PrimNodeFree */
extern u8 *PrimLinkPool;             /* 8-byte nodes, free list in PrimLinkFree */
typedef struct PrimList {
    s32 f0;
    s32 f4;
    s32 **f8;
    s32 *fC;
    s32 **f10;
} PrimList;

typedef struct {
    PrimList b; /* 0x00 */
    PrimList a; /* 0x14 */
} PrimListPair;
typedef struct PrimSet {
    struct PrimSet *next;  /* 0x00 */
    PrimListPair pairs[2]; /* 0x04 */
    u16 mode;              /* 0x54 */
    u16 dead;              /* 0x56 */
} PrimSet;
extern PrimSet *PrimSetList;
extern PrimSet *PrimSetFree;
extern s32 *PrimLinkFree;
extern NAS *NASActiveHead;
extern s32 num_sfx_banks;
extern s32 VabCueTimer;
extern s32 VabCuePending;
extern s32 GameFrameCount;
extern s32 CompletionBcd; /* BCD of count_completion's count */
extern s32 FirstBootDone;
extern subengine *next_sub;
extern s32 ScriptFade;
extern s32 ScriptTimeStep;
extern s32 ScriptThreadPool;
extern s32 ScriptPool;
extern u8 GameCup;
extern void *HeadOfBalls;
/* An ordering table and the primitive area drawn through it (0x400C bytes;
 * DrawBuffers holds two). */
typedef struct {
    u_long ot[0x1000];
    u8 *prim; /* 0x4000 */
    u8 *end;  /* 0x4004 */
    u8 *cur;  /* 0x4008 */
} OtBuf;

extern DrawBuffer DrawBuffers[2];
extern s32 VideoMode;
extern u8 *ScriptTrackDbase;
extern s32 ScriptTrackIndex;
extern s32 ScriptTrackThread;

/* update_emitter_thread: per-particle state, 0x28 bytes */
typedef struct EmitPart {
    s32 age;
    s32 unk4;
    s32 vel[3];
    u8 pad14[0x14];
} EmitPart;


extern NAS *NASFreeList;
extern u16 FirstNas;            /* first pool slot BuildNASFreeList frees */
extern u16 NasPoolSlots;            /* pool slot count */
extern u16 NasPoolUsed;            /* pool slots in use (KillEveryNas kills them all) */
extern NAS *NasPool;           /* NAS pool */
extern NAS NASTemplate;            /* copied over each spawned NAS */
extern EXTengine EngineTemplate;   /* copied over each new engine */
extern s16 AtanTable[];            /* get_angle */
extern u16 GameExitFlags;
extern s32 WideScreen;
extern s32 screen_aspect[2];





void gte_rotate_point(SVECTOR *v, VECTOR *out);






void plot_point_sprites(PointSprites *ps, MATRIX *m);



void smallnode_init(void);

void loadnode_init(void);

void palette_set_top_bits(u16 *arg0, u32 arg1);


void clear_bit15(u16 *arg0, s32 arg1);





s32 Random(s32 n);




s32 get_angle(s32 x, s32 z);





void mal_free(void *ptr, _mal_area *heap);












s32 mal_count_free(_mal_area *heap);



void shell_run_spyro_demo(void);




void cheat_menu_enable(void);

s32 bit_count(u32 x);

s32 lerp_fix12(s32 a, s32 b, s32 t);

void *dbase_anim_node(s32 *arg0, FileEntry *rec);





void pad_dpad_to_stick(s32 pad, PadState *out);








typedef struct {
    u16 x;
    u16 y;
} ClutPos;
/* One of a texture set's cluts (obj_textureset.ent): its colours are at base + off in the
 * texture file until textureset_upload_cluts loads them into VRAM at pos. */
typedef struct TexClut {
    /* 0x00 */ u16 w;       /* colours */
    /* 0x02 */ u16 clut;    /* GetClut's word for pos */
    /* 0x04 */ ClutPos *pos;
    /* 0x08 */ s32 off;
} TexClut;


MeshAnimFrame *dbase_anim_frame(s32 i, FileEntry *rec);

void BSwapEndian(u8 *p, u32 n);





void WSwapEndian(u16 *p, u32 n);



void loader_reset(void);





void *smallnode_alloc(void);














void *mal_split_ptr(void *ptr, s32 bytes);

void pads_update(void);

s32 asload_chunk_relocatable(s32 arg0, s32 arg1, u32 **arg2, s32 arg3);


LoadNode *loadlist_pop(LoaderState *ls);

s32 load_file_to(s32 *arg0, u8 *arg1);

mal *mal_next_free_block(mal *blk);

s32 load_file(s32 *arg0, u8 *arg1);

mal *mal_split(mal *blk, s32 words);



/* Per-note pitch data: a signed note and a signed fine nibble in the top of +1. */
typedef struct {
    s8 note;
    u8 fine; /* 0x01: high nibble is a signed fine value */
} PitchInfo;

typedef struct {
    s16 voice;       /* 0x00 */
    PitchInfo *info; /* 0x04 */
} PitchVoice;


s32 entry_block_value(s32 i, struct ModelFile *blk);

void mal_free_later(u8 *arg0, _mal_area *heap, s32 arg2);





void shell_render(void);

mal *mal_after_lowest(_mal_area *heap);

void toggle_widescreen(void);

void pads_init(void);

MeshHeader *map_entity_address(u16 arg0, ModelFile *arg1);










u8 *entity_address(u32 arg0, ModelFile *arg1);



s32 lerp_angle(s32 arg0, s32 arg1, s32 arg2, s32 arg3);



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
/* A text object draw_text_nas draws: a NAS-like header with the position at
 * +4 (x keeps DrawSysText's 0xC000 centring flags) and the text at +0x6C. */
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
extern PosEntry PosFlashes[];
void shell_init(void);

void shell_render_dcache(void);

void shell_init_heap(void);

void shell_update(void);

void shell_start_engine(void);

void mal_insert_free(mal *blk, _mal_area *heap);

mal *mal_prev_movable(mal *arg0);

mal *mal_next_movable_now(_mal_area *heap);

void group_split_buffer(void *arg0, s32 *arg1);

s32 group_file_offset(s32 *arg0, s32 *arg1);

void loadnode_unlink(LoadNode *arg0);

s32 load_file_permanent(s32 *arg0);

s32 load_file_to_wait(s32 arg0, s32 arg1, s32 arg2);

void itoa_div(char *dst, s32 val, s32 div);

char *itoa_div_end(char *arg0, s32 arg1, s32 arg2);

s32 lerp_snap(s32 a, s32 b, s32 t);



































s32 msg_glyph_width(u8 *arg0);

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

u8 *mal_trim(u8 *arg0, s32 arg1, _mal_area *arg2);

void *mal_alloc_owned_low(s32 arg0, void * *arg1, s32 arg2, s32 arg3);

struct obj_textureset;


/* texture_upload_list: count at +0, then 0x10-byte entries from +4. */
typedef struct {
    s32 idx;      /* 0x0: record index into the 0x38-byte table */
    s32 unk4;
    s32 unk8;
    FreeHdr *pos; /* 0xC: VRAM slot from vram_take */
} TexUploadEntry;

typedef struct {
    s32 count;
    TexUploadEntry e[1];
} TexUploadList;

SortNode *sortlist_bubble(SortList *arg0);


LoadNode *loadlist_sort_by_sector(LoaderState *ls);

s32 mal_can_alloc(s32 arg0);

s32 load_file_wait(s32 *arg0, u8 * *arg1);

s32 load_file_part_wait(s32 arg0, s32 arg1, u8 * *arg2, s32 arg3);

s32 get_dist_angles(s32 *a, s32 *b, s16 *out);

u8 *model_entry_data(obj_dbase *arg0, s32 arg1);

s32 model_entry_texture(obj_dbase *arg0, s32 arg1);




















void *mal_take_low(s32 bytes, _mal_area *heap);
































extern u8 PadActAlign[];


/* Raw controller packet read by pad_read_packet (status byte, type in the top nibble). */
typedef struct PadPacket {
    /* 0x00 */ u8 status;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u16 buttons;
    /* 0x04 */ u8 rx;
    /* 0x05 */ u8 ry;
    /* 0x06 */ u8 lx;
    /* 0x07 */ u8 ly;
} PadPacket;



extern s32 PadLastRead;

/* pad_rumble_send's body, inlined into pad_read_packet. */

mal *mal_take_low_from(mal *blk, s32 words, _mal_area *heap);

void mal_take_high_from(mal *arg0, s32 arg1, _mal_area *arg2);

void mal_move_up(mal *arg0, _mal_area *arg1);

u8 *queue_dbase_load(ModeDbase *arg0, s32 arg1);

u8 *queue_group_load(file_group_load_info *arg0);

void dbase_files_loaded(s32 * *arg0, s32 *arg1, u8 *arg2);

void combo_check(ComboSeq *c);







/* A script cursor: kind 1 or 2 steps idx through a table of entries (0 entries
 * jump to the index in their low half) and stores the entry in cur. */
typedef struct ScriptCursor {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1[0xB];
    /* 0x0C */ s32 cur;
    /* 0x10 */ s32 *tbl;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
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

void mal_move_down(mal *arg0, _mal_area *arg1);

u8 *loader_alloc(LoadNode *arg0);

void pad_read(PadState *pad);

void cam_set_projection(camera_info_tag *cam, RECT *rect);

s32 fx_alloc_buffers(FxDef *def, subengine *sub);





typedef struct ScriptInit {
    s32 cur;
    s32 *tbl;
    u32 flags;
} ScriptInit;

void init_dots(subengine *arg0);









/* Inlined copies of view_project_volume, view_project and sfx_pan (used by
 * sfx_position_volume). */




extern Slot12 VramSlots[];

/* A texture file's header (docs/ASSETS.md, Texture sets): the CLUTs (each a u32 count and its
 * colours), the images (TexImage, each followed by its pixels), then the animated textures. */
typedef struct TexFile {
    /* 0x00 */ u32 type;   /* 8 */
    /* 0x04 */ u32 size;
    /* 0x08 */ u16 nTex;
    /* 0x0A */ u16 nClut;
    /* 0x0C */ s32 images; /* from here to the first TexImage */
    /* 0x10 */ s32 cluts;  /* from here to the CLUTs */
    /* 0x14 */ s32 f14;    /* copied to obj_textureset.f14 */
    /* 0x18 */ s32 anims;  /* from the file's start to its AnimData, 0: none */
    /* 0x1C */ s32 extra;  /* from here to a count and words for obj_textureset.anim, 0 in every file */
} TexFile;

/* An image's header in a texture file (docs/ASSETS.md, Texture sets); its pixels follow. */
typedef struct TexImage {
    /* 0x00 */ u16 w;          /* in VRAM units */
    /* 0x02 */ u16 h;
    /* 0x04 */ u16 a;
    /* 0x06 */ u16 b;
    /* 0x08 */ u32 c;          /* copied to obj_texture.fC */
    /* 0x0C */ u16 eight : 1;  /* 8-bit (else 4-bit) */
               u16 clut : 15;  /* its CLUT, 0x7FFF none */
    /* 0x0E */ u16 flags;      /* 2: one frame per language from here */
    /* 0x10 */ s32 frames;
} TexImage;




void mal_compact_from(mal *arg0, _mal_area *arg1);

void mal_compact_high(_mal_area *heap);

void combo_check_all(void);























/* cam_ease_to: camera eased towards a target view (CamXYZ: core.h) */

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

void cam_ease_to(CamView *cam, CamView *dst, CamEase *ease);








/* Map placement record (map_enviroment.placements, 0xA0 bytes each),
 * copied into a subengine by mapenv_place_subs. */
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




/* DrawSpriteTriple: three 0x38-byte sprite records drawn side by side */
extern u16 SpriteTripleUVs[][3];

mal *mal_next_movable(_mal_area *heap);

void export_exe_functions(void);














#include "bash/card.h"

#include "bash/music.h"

#include "bash/sound.h"

#include "bash/nascalls.h"

#include "bash/sfx.h"

#include "bash/prims.h"

#include "bash/dbase.h"

/* The boss's record in a boss arena, after Heroes: flags as a hero's (0x8000 out, 0x2000
 * beaten), health as the panel shows it (pHeroHealth[3]). */
typedef struct BossRecord {
    s16 unk0;
    s16 unk2;
    u16 flags;
    s16 health;
    s16 unk8;
    s16 unkA;
} BossRecord;
extern BossRecord BossState;

#include "bash/aiskill.h"

/* DrawHudEntries: one 0x18-byte HUD script entry; the list ends at type -1. */
typedef struct HudEntry {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 idx;
    /* 0x08 */ s32 a;
    /* 0x0C */ s32 b;
    /* 0x10 */ s32 c;
    /* 0x14 */ s32 d;
} HudEntry;

/* A screen mode for one TV standard: its two display buffers' areas. */
typedef struct ScreenMode {
    RECT buf0;
    RECT buf1;
    s32 unk10[2];
} ScreenMode;

#endif /* BASH_MAIN_H */
