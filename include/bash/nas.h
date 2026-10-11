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

/* The leak uses bool but never defines it. It is 32 bits: the callees of the
 * leak's bool parameters (ArkHeroRad2ObjectCollision, FreeGrabbedObject) read
 * them as whole words, with no truncation to a char or short. */
typedef s32 bool;

/* Compile-time size check (C89): fails to compile if the size is wrong. */
#define BASH_SIZE_CHECK(type, size) typedef char type##_size_check[(sizeof(type) == (size)) ? 1 : -1]

typedef struct NAS NAS;
typedef struct EXTengine EXTengine;
typedef struct subengine subengine;

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

/* The prefix every extension shares, for code that walks a chain of mixed
 * types (FindExtension). Placeholder name: the leak has no such struct. */
typedef struct EXTheader {
    s32 EXTid;
    EXTnode Extend;
} EXTheader;
BASH_SIZE_CHECK(EXTheader, 0xC);

/* A spawn record (placement data) that SpawnNASFromRecord builds a NAS from.
 * Placeholder names; only these fields are known. */
typedef struct NASSpawn {
    /* 0x00 */ SLONG_XYZ pos;           /* copied to the engine (EXTengine.unk10) */
    /* 0x0C */ u8 unkC[0xC];
    /* 0x18 */ u16 id;                  /* AST id */
    /* 0x1A */ u16 flags;               /* bit 0: spawned; 0x10: KillNAS leaves the NAS alive */
} NASSpawn;

/* An engine object instance: an entry of the AST table ({id, Init, Plot}),
 * spawned by SpawnNASFromRecord from a pool of copies of a template. */
struct NAS {
    /* 0x00 */ u32 flags;               /* 1 when spawned; 0x100 once it has an engine */
    /* 0x04 */ u8 unk04[4];
    /* 0x08 */ void *ext;               /* first extension (EXTid, EXTnode, ...) */
    /* 0x0C */ NAS *prev;               /* active list */
    /* 0x10 */ NAS *next;               /* active list, or the free list */
    /* 0x14 */ u8 unk14[0x28 - 0x14];
    /* 0x28 */ NAS *prev28;             /* list headed by D_8005AA24 (func_80026A54 unlinks) */
    /* 0x2C */ NAS *next28;
    /* 0x30 */ u8 unk30[0x38 - 0x30];
    /* 0x38 */ NAS *prev38;             /* list headed by D_8005AA34 (func_80026980 unlinks) */
    /* 0x3C */ NAS *next38;
    /* 0x40 */ u8 unk40[0x48 - 0x40];
    /* 0x48 */ void (*Init)(NAS *);     /* AST Init: called once when spawned */
    /* 0x4C */ void (*Plot)(NAS *);     /* AST Plot */
    /* 0x50 */ NASSpawn *spawn;         /* the spawn record it was built from, or NULL */
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

/* An AST table entry: an object type's id and its Init/Plot. The engine's
 * table is EngineASTTable; ASTTable points at the one in use. */
typedef struct ASTEntry {
    s32 id;
    void (*Init)(NAS *);
    void (*Plot)(NAS *);
} ASTEntry;
BASH_SIZE_CHECK(ASTEntry, 0xC);

/* A 16-byte record with flag bits, found by func_80025A48: an engine has one
 * of its own (EXTengine.rec), and a PointList carries more after its points. */
typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u32 flags;
} FlagRecord;
BASH_SIZE_CHECK(FlagRecord, 0x10);

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
    /* 0x38 */ u8 unk38[0x3C - 0x38];
    /* 0x3C */ u8 unk3C[0x5C - 0x3C];   /* passed to func_8001CD04 (GTE setup) */
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ subengine *sub;           /* first of the engine's subengines */
    /* 0x70 */ subengine *subLast;       /* last of them */
    /* 0x74 */ u8 unk74[0x7C - 0x74];
    /* 0x7C */ EXTengine *next;          /* next engine extension of the same NAS */
    /* 0x80 */ EXTengine *link;          /* paired engine (its link points back); cleared both ways by KillNAS */
    /* 0x84 */ FlagRecord rec;            /* checked first by func_80025A48 */
    /* 0x94 */ s32 unk94;                /* a PlaySfx voice: StopSfx'd when the subengines are freed */
    /* 0x98 */ u8 unk98[2];
    /* 0x9A */ u16 subCount;             /* number of subengines */
};
BASH_SIZE_CHECK(EXTengine, 0x9C);

/* A node of an engine's subengine chain (EXTengine.sub .. subLast, linked
 * through next/prev). They come from one pool: func_80018F3C moves n of them
 * from the free list (D_80058ACC) onto an engine, copying the 0xA8-byte
 * template D_8005A830 over each; func_8001D174 returns them. Pogo's
 * PogoSquare.sub (subengine * in the leak) points at these nodes. */
struct subengine {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C[0x20 - 0x1C];
    /* 0x20 */ SLONG_XYZ unk20;
    /* 0x2C */ u8 unk2C[0x50 - 0x2C];
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ void (*hook)(subengine *, s32, u32); /* func_80021330 calls it on each node */
    /* 0x58 */ void (*release)(subengine *); /* called when the node is freed */
    /* 0x5C */ subengine *next;
    /* 0x60 */ subengine *prev;
    /* 0x64 */ u8 unk64[0x68 - 0x64];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A[2];
    /* 0x6C */ void *unk6C;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ u8 unk72[2];
    /* 0x74 */ u16 unk74;
    /* 0x76 */ u16 unk76;
    /* 0x78 */ u8 unk78[0xA8 - 0x78];
};
BASH_SIZE_CHECK(subengine, 0xA8);

/* Extension id 0x800, 0x34 bytes, made by AddMotion(nas) from the template
 * D_8005A9CC (it also sets 0x800 in NAS.flags). The leak's
 * CalcArkInfluence(EXTmotion *) is passed FindExtension(nas, 0x800). */
typedef struct EXTmotion {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 unk0D;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 unk20[2];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E[0x32 - 0x2E];
    /* 0x32 */ u16 unk32;
} EXTmotion;
BASH_SIZE_CHECK(EXTmotion, 0x34);

/* Extension id 0x400 | kind, 0x2C bytes, made by func_800265DC (which also
 * sets 0x400 in NAS.flags). It plays a hero_seq (+0x0C, set by SetHeroSeq);
 * func_800250B0 runs it: kind 1 with func_800251A8, kind 2 with
 * func_80025534. Placeholder name. */
typedef struct EXTanim {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ void *seq;               /* the one playing: seqs[seqId] */
    /* 0x10 */ void **seqs;             /* table func_80025FAC picks from, or NULL */
    /* 0x14 */ EXTengine *eng;           /* or NULL for NAS.eng */
    /* 0x18 */ subengine *sub;           /* or NULL for the engine's first */
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 seqId;
    /* 0x22 */ u8 unk22[0x28 - 0x22];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
} EXTanim;
BASH_SIZE_CHECK(EXTanim, 0x2C);

/* Extension id 0x1000, 0x2C bytes, made by AddObject(nas) from the template
 * D_8005A974 (it also sets 0x1000 in NAS.flags). InitBall stores it in
 * NAS.unk58, and PlotBall passes that to the leak's AR_*_TestOutOfBounds and
 * AlignGrabBall as their EXTobject *. */
typedef struct EXTobject {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ u8 unk0C[0x14 - 0x0C];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;               /* a player index (AlignGrabBall) */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A[2];
} EXTobject;
BASH_SIZE_CHECK(EXTobject, 0x2C);

/* A hero animation sequence is a stream of halfwords: the interpreter
 * (func_8008502C) reads seq[pos] and treats 0x8000 | n as command n (27 of
 * them, some taking halfword operands) and anything else as a frame. The leak
 * declares the sequences as `hero_seq NAME[]`. */
typedef u16 hero_seq;
#define BASH_HAVE_HERO_SEQ

/* A player's hero: Heroes[4] in the engine overlay, set up by func_80083708.
 * Functions take the player index (the leak's `unsigned int player`). */
typedef struct Hero {
    /* 0x00 */ NAS *nas;                /* from the pool at D_8005AA1C, AST index 3 */
    /* 0x04 */ EXTengine *eng;          /* AddEngine(nas, 1) */
    /* 0x08 */ subengine *sub;          /* eng->sub */
    /* 0x0C */ EXTengine *unk0C;        /* AddEngine(nas, 2) */
    /* 0x10 */ EXTengine *unk10;        /* AddEngine(nas, 3) */
    /* 0x14 */ EXTengine *unk14;        /* AddEngine(nas, 3) */
    /* 0x18 */ void *unk18;             /* AddExtension(nas, 0x54, 0x400000F0) */
    /* 0x1C */ void *unk1C;             /* func_800265DC(nas): 0x2C bytes, id 0x400; plays the hero_seq (+0x0C) */
    /* 0x20 */ void *unk20;             /* AddExtension(nas, 0x44, 0x400000F4) */
    /* 0x24 */ void *unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u16 action;              /* SetHeroAction */
    /* 0x2E */ u16 character;           /* 1-8, selects the HeroSeqTables entry */
    /* 0x30 */ s16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ u16 unk38;
    /* 0x3A */ u16 unk3A;
    /* 0x3C */ u16 unk3C;
    /* 0x3E */ u16 unk3E;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42;
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u16 unk46;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u16 unk4A;
    /* 0x4C */ u16 prevAction;          /* SetHeroAction keeps the previous action here */
    /* 0x4E */ u16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u16 seq;                 /* SetHeroSeq: index into the character's table */
    /* 0x58 */ u16 unk58;
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u16 unk5C;
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ u16 unk60;
    /* 0x62 */ u16 unk62;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u16 unk66;
    /* 0x68 */ u16 unk68;
    /* 0x6A */ u16 unk6A;
} Hero;
BASH_SIZE_CHECK(Hero, 0x6C);

/* Not recovered yet: incomplete, usable through pointers only. These are used
 * by the minigame overlays only. */
typedef struct NASTrigger NASTrigger;
typedef struct EXTobject_TA EXTobject_TA;
typedef struct EXTheromove EXTheromove;
typedef struct EXTbotai EXTbotai;
typedef struct TA_player TA_player;         /* Tank player state */
typedef struct PreMap PreMap;
typedef struct LevSpec LevSpec;
typedef struct shell_setup shell_setup;
typedef struct file_group_load_info file_group_load_info;

#endif /* BASH_NAS_H */
