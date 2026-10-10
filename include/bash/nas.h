#ifndef BASH_NAS_H
#define BASH_NAS_H

/* The engine object model (NAS and its extensions) and the other types used by
 * the minigame headers (bash/tank.h, pogo.h, arkenoid.h,
 * polar.h). The leak names them but does not define them. Layouts are recovered
 * from code; evidence is in docs/ARCHITECTURE.md (Engine types).
 *
 * A type whose layout is not known yet stays incomplete, and the minigame
 * declarations that need it complete are guarded by BASH_HAVE_<TYPE>. When a
 * layout is recovered, define the type here and define its guard next to it.
 * Members named unkXX are known only by offset and width. */

#include "common.h"

/* Provisional: the leak uses bool but never defines it, and its size is not
 * confirmed from code yet. */
typedef s32 bool;

/* Compile-time size check (C89): fails to compile if the size is wrong. */
#define BASH_SIZE_CHECK(type, size) typedef char type##_size_check[(sizeof(type) == (size)) ? 1 : -1]

typedef struct NAS NAS;
typedef struct EXTengine EXTengine;

/* Three s32 coordinates (x, y, z); polar.h reads x and z. */
typedef struct SLONG_XYZ {
    s32 x;
    s32 y;
    s32 z;
} SLONG_XYZ;
BASH_SIZE_CHECK(SLONG_XYZ, 0xC);

/* Link of an object's extension chain. Every EXT* extension starts with
 * `s32 EXTid; EXTnode Extend;`. The chain starts at NAS.ext; prev is the
 * object or extension this one was appended to (AllocExtension). */
typedef struct EXTnode {
    void *prev;
    void *next;
} EXTnode;
BASH_SIZE_CHECK(EXTnode, 0x8);
#define BASH_HAVE_EXTNODE

/* An engine object instance: an entry of the AST table ({id, Init, Plot}),
 * spawned by SpawnNASFromRecord from a pool of copies of a template. */
struct NAS {
    /* 0x00 */ u32 flags;               /* 1 when spawned; 0x100 once it has an engine */
    /* 0x04 */ u8 unk04[4];
    /* 0x08 */ void *ext;               /* first extension (EXTid, EXTnode, ...) */
    /* 0x0C */ NAS *prev;               /* active list */
    /* 0x10 */ NAS *next;               /* active list, or the free list */
    /* 0x14 */ u8 unk14[0x48 - 0x14];
    /* 0x48 */ void (*Init)(NAS *);     /* AST Init: called once when spawned */
    /* 0x4C */ void (*Plot)(NAS *);     /* AST Plot */
    /* 0x50 */ void *spawn;             /* spawn record: +0x00 position (to eng), +0x18 AST id */
    /* 0x54 */ EXTengine *eng;          /* first engine extension (AddEngine) */
    /* 0x58 */ void *unk58;
    /* 0x5C */ u16 unk5C;
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ u16 unk68;
    /* 0x6A */ u16 unk6A;               /* bit 0: an extension allocation failed */
    /* 0x6C */ s16 ast;                 /* index into the AST table */
    /* 0x6E */ u8 unk6E[2];
};
BASH_SIZE_CHECK(NAS, 0x70);

/* Engine extension, made by AddEngine(nas, id) from a template: EXTid is
 * id | 0x100. The first one is NAS.eng; later ones are linked through next. */
struct EXTengine {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ u32 flags;
    /* 0x10 */ SLONG_XYZ unk10;          /* set from the spawn record's first 12 bytes */
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 unk22[0x2C - 0x22];
    /* 0x2C */ SLONG_XYZ unk2C;
    /* 0x38 */ u8 unk38[0x5C - 0x38];
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ void *unk6C;
    /* 0x70 */ u8 unk70[0x7C - 0x70];
    /* 0x7C */ EXTengine *next;          /* next engine extension of the same NAS */
    /* 0x80 */ u8 unk80[0x9C - 0x80];
};
BASH_SIZE_CHECK(EXTengine, 0x9C);

/* Not recovered yet: incomplete, usable through pointers only. */
typedef struct NASTrigger NASTrigger;
typedef struct EXTobject EXTobject;
typedef struct EXTobject_TA EXTobject_TA;
typedef struct EXTmotion EXTmotion;
typedef struct EXTheromove EXTheromove;
typedef struct EXTbotai EXTbotai;
typedef struct subengine subengine;
typedef struct hero_seq hero_seq;           /* hero animation sequence */
typedef struct TA_player TA_player;         /* Tank player state */
typedef struct PreMap PreMap;
typedef struct LevSpec LevSpec;
typedef struct shell_setup shell_setup;
typedef struct file_group_load_info file_group_load_info;

#endif /* BASH_NAS_H */
