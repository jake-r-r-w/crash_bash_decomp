#ifndef BASH_NAS_H
#define BASH_NAS_H

/* The engine object model (NAS and its extensions) and the other types used by
 * the minigame headers (bash/tank.h, pogo.h, arkenoid.h, polar.h). The retail
 * leak names them but does not define them. Names, tags and layouts come from
 * the Oct 9 2000 prototype's debug types where it has them, each checked
 * against retail code; the rest are recovered from code. Evidence is in
 * docs/ARCHITECTURE.md (Engine types, Prototype debug types).
 *
 * A type whose layout is not known yet stays incomplete, and the minigame
 * declarations that need it complete are guarded by BASH_HAVE_<TYPE>. When a
 * layout is recovered, define the type here and define its guard next to it.
 * Members named unkXX are known only by offset and width. */

#include "common.h"
#include "bash/modelbank.h"

/* The leak uses bool but never defines it. It is 32 bits: the callees of the
 * leak's bool parameters (ArkHeroRad2ObjectCollision, FreeGrabbedObject) read
 * them as whole words, with no truncation to a char or short. */
typedef s32 bool;

/* Compile-time size check (C89): fails to compile if the size is wrong. */
#define BASH_SIZE_CHECK(type, size) typedef char type##_size_check[(sizeof(type) == (size)) ? 1 : -1]

typedef struct NAStype NAS;
typedef struct EXTtype EXT;
typedef struct EXTengtype EXTengine;
typedef struct subengine subengine;
typedef struct NASTrigger NASTrigger;
typedef struct _sfx_handler _sfx_handler;   /* defined in bash/main.h */

/* Three s32 coordinates (x, y, z); polar.h reads x and z. The prototype's
 * debug types use an anonymous 12-byte struct here. */
typedef struct SLONG_XYZ {
    s32 x;
    s32 y;
    s32 z;
} SLONG_XYZ;
BASH_SIZE_CHECK(SLONG_XYZ, 0xC);

/* The shell's other small vector types, from the Jampack Winter 2000 demo's
 * debug types (docs/PROTOTYPES.md). */
typedef struct SLONG_XY {
    s32 x;
    s32 y;
} SLONG_XY;

typedef struct SWORD_XY {
    s16 x;
    s16 y;
} SWORD_XY;
BASH_SIZE_CHECK(SWORD_XY, 0x4);

/* An SWORD_XY seen as one word (passed in a register). */
typedef union xy_w {
    SWORD_XY w;
    u32 SLONG_XY;
} xy_w;

typedef struct UBYTE_XY {
    u8 x;
    u8 y;
} UBYTE_XY;

typedef union xy_ub {
    UBYTE_XY b;
    u16 SLONG_XY;
} xy_ub;

typedef struct SWORD_XYZ {
    s16 x;
    s16 y;
    s16 z;
} SWORD_XYZ;
BASH_SIZE_CHECK(SWORD_XYZ, 0x6);

typedef struct SWORD_XYZ_MINMAX {
    SWORD_XYZ min;
    SWORD_XYZ max;
} SWORD_XYZ_MINMAX;

typedef struct SLONG_XYZ_MINMAX {
    SLONG_XYZ min;
    SLONG_XYZ max;
} SLONG_XYZ_MINMAX;

typedef struct UBYTE_RGB {
    u8 r;
    u8 g;
    u8 b;
} UBYTE_RGB;

/* Link of an object's extension chain. Every extension starts with
 * `s32 EXTid; EXTnode Extend;`. The chain starts at NAS.Extend.Child; Parent
 * is the object or extension this one was appended to (AllocExtension). */
typedef struct EXTnode {
    EXT *Parent;
    EXT *Child;
} EXTnode;
BASH_SIZE_CHECK(EXTnode, 0x8);
#define BASH_HAVE_EXTNODE

/* The prefix every extension shares, for code that walks a chain of mixed
 * types (FindExtension). */
struct EXTtype {
    s32 EXTid;
    EXTnode Extend;
};
BASH_SIZE_CHECK(EXT, 0xC);

/* The NAS lists: Parent is the previous object, Child the next. */
typedef struct NASnode {
    NAS *Parent;
    NAS *Child;
} NASnode;
BASH_SIZE_CHECK(NASnode, 0x8);

typedef struct GRPnode {
    NAS *Parent;
    NAS *Child;
    NAS **Head;
} GRPnode;
BASH_SIZE_CHECK(GRPnode, 0xC);

typedef struct OBJnode {
    NAS *Parent;
    NAS *Child;
} OBJnode;
BASH_SIZE_CHECK(OBJnode, 0x8);

typedef struct CAMnode {
    NAS *Parent;
    NAS *Child;
    u16 flags;
    u16 width;
    u16 height;
    u16 look_ahead;
} CAMnode;
BASH_SIZE_CHECK(CAMnode, 0x10);

typedef struct NasUser {
    /* 0x00 */ u16 status;
    /* 0x02 */ u16 flags;
    /* 0x04 */ s32 user1;
    /* 0x08 */ s32 user2;
} NasUser;
BASH_SIZE_CHECK(NasUser, 0xC);

/* A map trigger's placement data (the anonymous trigtype members). */
typedef struct {
    u32 dist;
    s32 y;
    u32 num;
} trig_path_pos;

typedef struct {
    u32 placement;
    s16 entity_num;
    s16 spare[3];
} trig_entity;

typedef struct {
    u32 placement;
    s16 num_frames;
    s16 frames_per_second;
    s16 framenum;
    s16 spare;
} trig_animation;

typedef union trigtype {
    trig_path_pos path;
    trig_entity entity;
    trig_animation animation;
    SLONG_XYZ xyz;
} trigtype;
BASH_SIZE_CHECK(trigtype, 0xC);

typedef struct trigger {
    /* 0x00 */ trigtype pos;
    /* 0x0C */ u32 type;
} trigger;
BASH_SIZE_CHECK(trigger, 0x10);

typedef struct {
    u32 num : 9;
    u32 dist : 23;
} tp_path;

typedef struct trig_path {
    union {
        tp_path tp_path;
        trigger *trig_placement;
    } tp_type;
} trig_path;
BASH_SIZE_CHECK(trig_path, 0x4);

typedef struct MapTriggerInfo {
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u32 val1;
    /* 0x08 */ u32 val2;
    /* 0x0C */ u32 val3;
    /* 0x10 */ u32 val4;
} MapTriggerInfo;
BASH_SIZE_CHECK(MapTriggerInfo, 0x14);

/* A map trigger that TriggerNas builds a NAS from (NAS.trigger). */
struct NASTrigger {
    /* 0x00 */ SLONG_XYZ TrigXYZ;       /* copied to the engine (xyzrf +0x10) */
    /* 0x0C */ trig_path TrigPath;
    /* 0x10 */ MapTriggerInfo *InfoStruct;
    /* 0x14 */ u16 StartUp;
    /* 0x16 */ u16 cast;
    /* 0x18 */ u16 Type;                /* AST id */
    /* 0x1A */ u16 Status;              /* bit 0: spawned; 0x10: KillNAS leaves the NAS alive */
};
BASH_SIZE_CHECK(NASTrigger, 0x1C);

/* An engine object instance: an entry of the AST table ({id, Init, Plot}),
 * spawned by TriggerNas from a pool of copies of a template. */
struct NAStype {
    /* 0x00 */ s32 NASid;               /* 1 when spawned; 0x100 once it has an engine */
    /* 0x04 */ EXTnode Extend;          /* Child: the first extension */
    /* 0x0C */ NASnode Tag;             /* active list, or (Child) the free list */
    /* 0x14 */ NASnode Family;          /* ReAdoptChild appends, RemoveFamily unlinks */
    /* 0x1C */ GRPnode Group;
    /* 0x28 */ OBJnode Object;          /* list headed by HeadOfObjects (UnlinkObject unlinks) */
    /* 0x30 */ OBJnode Solid;           /* list headed by HeadOfSolids (AddSolid pushes) */
    /* 0x38 */ CAMnode Camera;          /* list headed by HeadOfCameras (RemoveCamera unlinks) */
    /* 0x48 */ void (*InitDelete)(NAS *); /* AST Init: called once when spawned */
    /* 0x4C */ void (*Handler)(NAS *);  /* AST Plot */
    /* 0x50 */ NASTrigger *trigger;     /* the trigger it was built from, or NULL */
    /* 0x54 */ EXTengine *Engine;       /* first engine extension (AddExtEngine) */
    /* 0x58 */ EXT *FUX;
    /* 0x5C */ NasUser UserInfo;
    /* 0x68 */ u16 Class;
    /* 0x6A */ u16 Failure;             /* bit 0: an extension allocation failed */
    /* 0x6C */ u16 Type;                /* index into the AST table */
    /* 0x6E */ u8 pad6E[2];
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

/* A 16-byte record with flag bits, found by FindCollisionBox: an engine has one
 * of its own (EXTengine.collision_box), and a PointList carries more after its
 * points. The prototype's type for it is anonymous and not in the fragment. */
typedef struct {
    /* 0x00 */ s16 unk0;                 /* x offset (SetEngineColBox) */
    /* 0x02 */ s16 unk2;                 /* bottom (y offset) for CheckSolidCollision */
    /* 0x04 */ s16 unk4;                 /* z offset */
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u32 flags;
} FlagRecord;
BASH_SIZE_CHECK(FlagRecord, 0x10);

/* Engine extension, made by AddExtEngine(nas, id) from a template: EXTid is
 * id | 0x100. The first one is NAS.Engine; later ones are linked through
 * Engine. 0x0C..0x60 is the prototype's `xyzrf` (an anonymous struct whose
 * layout is not in the fragment), so its fields keep code-derived names. */
struct EXTengtype {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ u32 flags;
    /* 0x10 */ SLONG_XYZ unk10;          /* set from the trigger's TrigXYZ */
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ u8 unk22[0x2C - 0x22];
    /* 0x2C */ SLONG_XYZ unk2C;
    /* 0x38 */ u8 unk38[0x3C - 0x38];
    /* 0x3C */ u8 unk3C[0x5C - 0x3C];   /* passed to gfx_load_xform (GTE setup) */
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u16 unk5E;
    /* 0x60 */ SLONG_XYZ OldXYZ;
    /* 0x6C */ subengine *sub;           /* first of the engine's subengines */
    /* 0x70 */ subengine *last;          /* last of them */
    /* 0x74 */ void (*render)();
    /* 0x78 */ void (*kill)();
    /* 0x7C */ EXTengine *Engine;        /* next engine extension of the same NAS */
    /* 0x80 */ EXTengine *platform;      /* paired engine (its platform points back); cleared both ways by KillNAS */
    /* 0x84 */ FlagRecord collision_box; /* checked first by FindCollisionBox */
    /* 0x94 */ _sfx_handler *sfx;        /* StopSfx'd when the subengines are freed */
    /* 0x98 */ u8 boomshanka;
    /* 0x99 */ u8 direction;
    /* 0x9A */ s16 num_subs;             /* number of subengines */
};
BASH_SIZE_CHECK(EXTengine, 0x9C);

/* A node of an engine's subengine chain (EXTengine.sub .. last, linked
 * through next/prev). They come from one pool: eng_add_subs moves n of them
 * from the free list (next_sub) onto an engine, copying the 0xA8-byte
 * template SubEngineTemplate over each; eng_release_subs_sfx returns them. Pogo's
 * PogoSquare.sub (subengine * in the leak) points at these nodes. The
 * prototype fragment names the type but does not define it. */
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
    /* 0x54 */ void (*hook)(subengine *, s32, u32); /* render_emitter_thread calls it on each node */
    /* 0x58 */ void (*release)(subengine *); /* called when the node is freed */
    /* 0x5C */ subengine *next;
    /* 0x60 */ subengine *prev;
    /* 0x64 */ u8 unk64[0x68 - 0x64];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A[2];
    /* 0x6C */ union {
        struct obj_dbase *dbase;     /* a model's: the record its entity is in */
        char *text;                  /* a text panel's */
        void *ptr;
    } u6C;                           /* a script's control info and the fx buffers start here too */
    /* 0x70 */ union {
        s32 w;
        struct {
            u16 lo;                  /* a model's morph blend (draw_object_model) */
            s16 hi;                  /* its frame, when the flags' sign bit is set */
        } h;
    } u70;
    /* 0x74 */ u16 entity;           /* the entity in the dbase (entity_address) */
    /* 0x76 */ u16 unk76;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 unk84[0x8A - 0x84];
    /* 0x8A */ s16 unk8A;
    /* 0x8C */ u8 unk8C[0x8E - 0x8C];
    /* 0x8E */ u16 unk8E;
    /* 0x90 */ s32 unk90;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ u8 unk96[0x9C - 0x96];
    /* 0x9C */ s32 unk9C;
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ u8 unkA2[0xA8 - 0xA2];
};
BASH_SIZE_CHECK(subengine, 0xA8);

/* Extension id 0x800, 0x34 bytes, made by AddMotion(nas) from the template
 * ExtMotionTemplate (it also sets 0x800 in NAS.NASid). The leak's
 * CalcArkInfluence(EXTmotion *) is passed FindExtension(nas, 0x800). */
typedef struct EXTmotion {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ u8 Ydir;
    /* 0x0D */ u8 flags;
    /* 0x0E */ s16 Xvel;
    /* 0x10 */ s16 Xacc;
    /* 0x12 */ s16 Xdec;
    /* 0x14 */ s16 Xmax;
    /* 0x16 */ s16 Xa;
    /* 0x18 */ s16 Yvel;
    /* 0x1A */ s16 Yacc;
    /* 0x1C */ s16 Ydec;
    /* 0x1E */ s16 Ymax;
    /* 0x20 */ s16 Ya;
    /* 0x22 */ s16 Zvel;
    /* 0x24 */ s16 Zacc;
    /* 0x26 */ s16 Zdec;
    /* 0x28 */ s16 Zmax;
    /* 0x2A */ s16 Za;
    /* 0x2C */ u16 Mass;
    /* 0x2E */ s16 floorheight;
    /* 0x30 */ u16 Fdist;
    /* 0x32 */ u16 Ftime;
} EXTmotion;
BASH_SIZE_CHECK(EXTmotion, 0x34);

/* Extension id 0x400 | kind, 0x2C bytes, made by AllocExt400 (which also
 * sets 0x400 in NAS.NASid). It plays a hero_seq (Atable, set by SetHeroSeq);
 * RunNasAnim runs it: kind 1 with DoAnimCommand, kind 2 with
 * DoAnimCommand2. */
typedef struct EXTanimator {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ void *Atable;            /* the one playing: Alist[ALindex] */
    /* 0x10 */ void **Alist;            /* table GotoLinkAnim picks from, or NULL (void * in the prototype) */
    /* 0x14 */ EXTengine *Engine;       /* or NULL for NAS.Engine */
    /* 0x18 */ subengine *sub;          /* or NULL for the engine's first */
    /* 0x1C */ s16 Aspeed;
    /* 0x1E */ s16 Aindex;
    /* 0x20 */ s16 ALindex;
    /* 0x22 */ s16 Acount;
    /* 0x24 */ s16 Masterspeed;
    /* 0x26 */ s16 Currentframe;
    /* 0x28 */ s16 Seq_count;
    /* 0x2A */ s16 Seq_count_down;
} EXTanimator;
BASH_SIZE_CHECK(EXTanimator, 0x2C);

typedef struct PCoord {
    s16 angle;
    s16 dist;
} PCoord;
BASH_SIZE_CHECK(PCoord, 0x4);

/* Extension id 0x1000, 0x2C bytes, made by AddObject(nas) from the template
 * ExtTemplate1000 (it also sets 0x1000 in NAS.NASid). InitBall stores it in
 * NAS.FUX, and PlotBall passes that to the leak's AR_*_TestOutOfBounds and
 * AlignGrabBall as their EXTobject *. */
typedef struct EXTobject {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ NAS *target;
    /* 0x10 */ NAS *who;
    /* 0x14 */ u16 whatami;
    /* 0x16 */ s16 player;              /* a player index (AlignGrabBall) */
    /* 0x18 */ s16 owner;
    /* 0x1A */ u16 status;
    /* 0x1C */ u16 can_do;
    /* 0x1E */ u16 will_do;
    /* 0x20 */ s16 timer;
    /* 0x22 */ s16 countdown;
    /* 0x24 */ s16 contains;
    /* 0x26 */ PCoord coord;
    /* 0x2A */ u8 pad2A[2];
} EXTobject;
BASH_SIZE_CHECK(EXTobject, 0x2C);

/* The hero's movement extension (HeroInfo.hero_move, 0x54 bytes). */
typedef struct EXTheromove {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 forward_speed;
    /* 0x10 */ s32 reactive_speed;
    /* 0x14 */ u16 forward_angle;
    /* 0x16 */ u16 reactive_angle;
    /* 0x18 */ u16 facing_angle;
    /* 0x1A */ u8 run_walk;
    /* 0x1B */ u8 jump_ctrl;
    /* 0x1C */ u8 velocity_calc;
    /* 0x1D */ u8 velocity_enable;
    /* 0x20 */ s32 x_vel;
    /* 0x24 */ s32 x_max;
    /* 0x28 */ s16 x_acc;
    /* 0x2A */ s16 x_dec;
    /* 0x2C */ s16 x_dir;
    /* 0x30 */ s32 jump_vel;
    /* 0x34 */ s32 jump_max;
    /* 0x38 */ s16 jump_acc;
    /* 0x3A */ s16 jump_dec;
    /* 0x3C */ s16 jump_dir;
    /* 0x40 */ s32 z_vel;
    /* 0x44 */ s32 z_max;
    /* 0x48 */ s16 z_acc;
    /* 0x4A */ s16 z_dec;
    /* 0x4C */ s16 z_dir;
    /* 0x4E */ s16 mass;
    /* 0x50 */ s16 HitBy;
} EXTheromove;
BASH_SIZE_CHECK(EXTheromove, 0x54);

/* The AI extension of a computer-controlled hero (HeroInfo.hero_ai, 0x44
 * bytes). */
typedef struct _EXTbotai {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ NAS *pTargetNas;
    /* 0x10 */ s32 TargetPlayer;
    /* 0x14 */ SLONG_XYZ TargetXYZ;
    /* 0x20 */ u16 AImode;
    /* 0x22 */ u16 AIaborted_mode;
    /* 0x24 */ u16 AItimer1;
    /* 0x26 */ u16 AItimer2;
    /* 0x28 */ u16 AItimer3;
    /* 0x2A */ u16 AIang;
    /* 0x2C */ u32 AIpad;
    /* 0x30 */ u16 AIinitial_delay;
    /* 0x32 */ u16 AIsleep;
    /* 0x34 */ u8 AIforward_left[8];
    /* 0x3C */ u8 AIforward_right[8];
} EXTbotai;
BASH_SIZE_CHECK(EXTbotai, 0x44);

/* A hero animation sequence is a stream of halfwords: the interpreter
 * (ProcessHeroAnim) reads seq[pos] and treats 0x8000 | n as command n (27 of
 * them, some taking halfword operands; the prototype's HEROcmds enum) and
 * anything else as a frame. The leak declares the sequences as
 * `hero_seq NAME[]`. The prototype types it `short`. */
typedef u16 hero_seq;
#define BASH_HAVE_HERO_SEQ

/* A player's hero: Heroes[4] in the engine overlay, set up by InitialiseHeroes.
 * Functions take the player index (the leak's `unsigned int player`). */
typedef struct HeroInfo {
    /* 0x00 */ NAS *hero;               /* from the pool at NasPool, AST index 3 */
    /* 0x04 */ EXTengine *hero_eng;     /* AddExtEngine(nas, 1) */
    /* 0x08 */ subengine *hero_sub;     /* hero_eng->sub */
    /* 0x0C */ EXTengine *hero_shadow;  /* AddExtEngine(nas, 2) */
    /* 0x10 */ EXTengine *hero_powerup; /* AddExtEngine(nas, 3) */
    /* 0x14 */ EXTengine *hero_fx;      /* AddExtEngine(nas, 3) */
    /* 0x18 */ EXTheromove *hero_move;  /* AddExt(nas, 0x54, 0x400000F0) */
    /* 0x1C */ EXTanimator *hero_anim;  /* AllocExt400(nas) */
    /* 0x20 */ EXTbotai *hero_ai;       /* AddExt(nas, 0x44, 0x400000F4) */
    /* 0x24 */ EXTobject *hero_object;
    /* 0x28 */ NAS *hero_harm_Nas;
    /* 0x2C */ u16 hero_mode;           /* SetHeroAction (HeroMode_nums) */
    /* 0x2E */ u16 hero_name;           /* 1-8, selects the HeroSeqTables entry */
    /* 0x30 */ s16 hero_teammate;
    /* 0x32 */ u16 hero_status;
    /* 0x34 */ u16 hero_ctrl;
    /* 0x36 */ s16 hero_health;
    /* 0x38 */ s16 hero_harm_player;
    /* 0x3A */ u16 hero_harm_value;
    /* 0x3C */ u16 hero_harm_fraction;
    /* 0x3E */ u16 hero_harm_flags;
    /* 0x40 */ u16 hero_harm_sfx;
    /* 0x42 */ s16 hero_strength;
    /* 0x44 */ u16 hero_immunity;
    /* 0x46 */ u16 hero_immunity_level;
    /* 0x48 */ u16 hero_hit;
    /* 0x4A */ u16 hero_frozen;
    /* 0x4C */ u16 hero_last_mode;      /* SetHeroAction keeps the previous mode here */
    /* 0x4E */ s16 hero_idle_count;
    /* 0x50 */ s16 hero_score;
    /* 0x52 */ s16 hero_score_displayed;
    /* 0x54 */ u16 hero_pick_delay;
    /* 0x56 */ u16 hero_aseq;           /* SetHeroSeq: index into the character's table */
    /* 0x58 */ u16 hero_pu;
    /* 0x5A */ u16 hero_pu_timer;
    /* 0x5C */ u16 hero_pu_lock;
    /* 0x5E */ u16 hero_pu_action;
    /* 0x60 */ u16 hero_pu_action_timer;
    /* 0x62 */ u16 hero_weapon_count;
    /* 0x64 */ s16 hero_weapon_timer;
    /* 0x66 */ u16 hero_attack_timer;
    /* 0x68 */ u16 hero_attack_delay;
    /* 0x6A */ u16 hero_attack_counter;
} HeroInfo;
BASH_SIZE_CHECK(HeroInfo, 0x6C);

/* A level's world data (PreMap.WorldInfo). */
typedef struct WWInf {
    /* 0x00 */ s32 MapDbase;
    /* 0x04 */ char **Anims;
    /* 0x08 */ s16 *LevelDB;
    /* 0x0C */ s32 camera;
    /* 0x10 */ void *Spare1;
    /* 0x14 */ void *Spare2;
    /* 0x18 */ void *Spare3;
    /* 0x1C */ void *Spare4;
} WWInf;
BASH_SIZE_CHECK(WWInf, 0x20);

typedef struct TrigLookUp {
    /* 0x00 */ u16 *LUPtab;
    /* 0x04 */ u16 LUPsize;
} TrigLookUp;
BASH_SIZE_CHECK(TrigLookUp, 0x8);

/* Each minigame's map data (TANK_PREMAP, POGO_PREMAP, ...). */
typedef struct PreMap {
    /* 0x00 */ s16 *PreLoadDB;
    /* 0x04 */ WWInf *WorldInfo;
    /* 0x08 */ TrigLookUp *LUPinfo;
    /* 0x0C */ void *Do_Not_Use1;
    /* 0x10 */ void *Do_Not_Use2;
    /* 0x14 */ void *Do_Not_Use3;
    /* 0x18 */ void *Do_Not_Use4;
} PreMap;
BASH_SIZE_CHECK(PreMap, 0x1C);

/* The player's options, saved with the game: volumes in percent, stereo, a pad's vibration. */
typedef struct GameOptionSet {
    /* 0x00 */ s32 music;
    /* 0x04 */ s32 sfx;
    /* 0x08 */ s32 stereo;
    /* 0x0C */ s8 vibrate[4];
} GameOptionSet;
/* GameInfo's last 0x11C bytes (0x8005A684..0x8005A7A0), the adventure's progress: per-level
 * tables of 0x21 bytes. Field names are GameInfo offsets, as gameflow.h's GameStateView. */
typedef struct AdventureState {
    /* 0x070 */ s32 unk70;
    /* 0x074 */ u8 unk74;
    /* 0x075 */ s8 unk75;
    /* 0x076 */ u8 unk76[0x21];
    /* 0x097 */ u8 pad97[4];
    /* 0x09B */ u8 unk9B[4];
    /* 0x09F */ u8 unk9F[4];
    /* 0x0A3 */ u8 padA3[4];
    /* 0x0A7 */ u8 unkA7[4];
    /* 0x0AB */ u8 unkAB[0x21];
    /* 0x0CC */ u8 unkCC[4];
    /* 0x0D0 */ GameOptionSet options; /* GameOptions when the game was saved */
    /* 0x0E0 */ s32 unkE0;
    /* 0x0E4 */ u8 unkE4[5][0x21];
} AdventureState;
BASH_SIZE_CHECK(AdventureState, 0x11C);

/* Each minigame's level hooks (TANKSpecifics, POGOSpecifics, ...). */
typedef struct LevSpec {
    /* 0x00 */ void (*LevelPreInit)();
    /* 0x04 */ void (*LevelInit)();
    /* 0x08 */ void (*LevelRestartInit)();
    /* 0x0C */ void (*LevelRoutine)();
    /* 0x10 */ void (*LevelExit)();
    /* 0x14 */ void (*EarliestTriggers)();
    /* 0x18 */ void (*PostTriggers)();
    /* 0x1C */ void (*SpecTriggerFunc)();
    /* 0x20 */ s32 (*OnOffWindowTest)(); /* the return type is not recorded */
    /* 0x24 */ s32 (*GetContour)();
    /* 0x28 */ void (*FinishQuick)();
} LevSpec;
BASH_SIZE_CHECK(LevSpec, 0x2C);

/* Each minigame's shell settings (tank_shell_info, POGO_shell_info, ...): the
 * second pointer of the engine's per-world record, after the PreMap. Its size
 * is from Tank's data (the next object starts 0x28 bytes on); the fields are
 * not known yet. */
typedef struct shell_setup {
    /* 0x00 */ s32 unk0[6];
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} shell_setup;
BASH_SIZE_CHECK(shell_setup, 0x28);

/* A level's files (file_group_TANKDESERT, ...): the engine's table at
 * 0x800A0030 points at one per mode. group is an entry of the exe's file
 * groups table (0x80050010); files a list of {model slot, mdl entry, tex
 * entry} words ending in zeros (Tank's), loaded after the group. */
typedef struct FileGroup {
    /* 0x00 */ s32 first;   /* its first file in the archive */
    /* 0x04 */ s32 count;
    /* 0x08 */ s32 size;    /* bytes */
} FileGroup;
extern FileGroup FileGroups[];

typedef struct file_group_load_info {
    /* 0x00 */ void *group;
    /* 0x04 */ void *files;
} file_group_load_info;
BASH_SIZE_CHECK(file_group_load_info, 0x8);

/* HitSfx: the hit sounds by kind, four of each (PlaySfx of one at random). */
typedef struct {
    s32 hit[4];
    s32 hit6[4];
    s32 other[4];
    s32 crate[4];
} HitSfxTable;

#endif /* BASH_NAS_H */
