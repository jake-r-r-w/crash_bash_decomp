#ifndef BASH_CORE_H
#define BASH_CORE_H

/* The engine's shared types beyond the object model in bash/nas.h: scripts,
 * maps and triggers, animation tables, memory, geometry and particles.
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size, except the
 * shell's first types: the Oct 9 records number them as Jampack Winter 2000's
 * do, which define them (._1 SLONG_XY, ._2 SWORD_XY, ._10 UBYTE_RGB, sizes
 * agreeing), so those fields have their types. */

#include "bash/nas.h"

typedef struct obj_dbase obj_dbase;
typedef struct script_control_info script_control_info;
typedef struct sub_script_info sub_script_info;
typedef struct sub_string_info sub_string_info;

typedef struct script_command_subscript script_command_subscript;
typedef struct script_key_anim script_key_anim;
typedef struct script_command_anim script_command_anim;
typedef struct script_command_fade script_command_fade;
typedef struct partmotdata partmotdata;
typedef struct partdata partdata;
typedef struct script_thread script_thread;
typedef struct script_header script_header;
typedef struct script_thread_handler script_thread_handler;
typedef struct script_handler script_handler;
typedef struct level_music_info level_music_info;
typedef struct map_tree map_tree;
typedef struct trigger_list trigger_list;
typedef struct map_map map_map;
typedef struct map_list map_list;
typedef struct map_enviroment map_enviroment;
typedef struct FillNAS FillNAS;
typedef struct EXTmemory EXTmemory;
typedef struct EXTscratchpad EXTscratchpad;
typedef struct MEXTtype MEXTtype;
typedef struct MALEXTStruct MALEXTStruct;
typedef struct ANIMtab ANIMtab;
typedef struct AnimTn AnimTn;
typedef struct REL REL;
typedef struct fadescreen_info fadescreen_info;
typedef struct NasMetrics NasMetrics;
typedef struct rules_info rules_info;
typedef struct scrolling_credits scrolling_credits;
typedef struct geom_info geom_info;
typedef struct draw_window draw_window;
typedef struct matrix_stack matrix_stack;
typedef struct obj_globals obj_globals;
typedef struct MapTrigger4Longs MapTrigger4Longs;
typedef struct TrigMarker TrigMarker;
typedef struct TRGBOX TRGBOX;
typedef struct AcmdFX AcmdFX;
typedef struct particledata particledata;
typedef struct EXTparticlemaster EXTparticlemaster;
typedef struct box box;
typedef struct sphere sphere;
typedef struct pcontrol pcontrol;
typedef struct threaddata threaddata;
typedef struct EXTscriptdata EXTscriptdata;

struct script_command_subscript {
    /* 0x00 */ s32 script_num;
    /* 0x04 */ s32 sort_priority;
    /* 0x08 */ s32 spare1;
    /* 0x0C */ s32 spare2;
    /* 0x10 */ s32 key[18]; /* struct script_key_subscript (0x48 bytes), not in the fragment */
};
BASH_SIZE_CHECK(script_command_subscript, 0x58);

struct script_key_anim {
    /* 0x00 */ s32 frame[2]; /* struct ._338 (0x8 bytes), not in the fragment */
    /* 0x08 */ SLONG_XYZ pos;
    /* 0x14 */ SLONG_XYZ ang;
    /* 0x20 */ s32 quat[4]; /* struct ._316 (0x10 bytes), not in the fragment */
    /* 0x30 */ s32 quat_interp[3]; /* struct ._341 (0xC bytes), not in the fragment */
    /* 0x3C */ SLONG_XYZ scale;
    /* 0x48 */ s32 fade;
};
BASH_SIZE_CHECK(script_key_anim, 0x4C);

struct script_command_anim {
    /* 0x00 */ s32 aframe;
    /* 0x04 */ s32 start_aframe;
    /* 0x08 */ s32 end_aframe;
    /* 0x0C */ s32 delay;
    /* 0x10 */ s32 prolong;
    /* 0x14 */ s32 sort_priority;
    /* 0x18 */ s32 spare;
    /* 0x1C */ script_key_anim key[1];
};
BASH_SIZE_CHECK(script_command_anim, 0x68);

struct script_command_fade {
    /* 0x00 */ s32 start;
    /* 0x04 */ s32 end;
};
BASH_SIZE_CHECK(script_command_fade, 0x8);

struct partmotdata {
    /* 0x00 */ s32 life;
    /* 0x04 */ s32 delay;
    /* 0x08 */ s32 vel[4]; /* struct ._22 (0x10 bytes), not in the fragment */
    /* 0x18 */ s32 grav[4]; /* struct ._22 (0x10 bytes), not in the fragment */
};
BASH_SIZE_CHECK(partmotdata, 0x28);

struct partdata {
    /* 0x00 */ s32 flags;
    /* 0x04 */ SLONG_XYZ pos;
    /* 0x10 */ s32 maxparticlecount;
    /* 0x14 */ s32 particlesperframe;
    /* 0x18 */ s32 particlelifetime;
    /* 0x1C */ s32 effecttime;
    /* 0x20 */ s32 emitterspeed;
    /* 0x24 */ s32 randomspeedflag;
    /* 0x28 */ s32 frame;
    /* 0x2C */ s32 emitterradius;
    /* 0x30 */ s32 yrotmin;
    /* 0x34 */ s32 yrotmax;
    /* 0x38 */ s32 xrotmin;
    /* 0x3C */ s32 xrotmax;
    /* 0x40 */ s32 gxmin;
    /* 0x44 */ s32 gxmax;
    /* 0x48 */ s32 gymin;
    /* 0x4C */ s32 gymax;
    /* 0x50 */ s32 gzmin;
    /* 0x54 */ s32 gzmax;
    /* 0x58 */ s32 fadeinend;
    /* 0x5C */ s32 fadeoutstart;
    /* 0x60 */ s32 scaleinend;
    /* 0x64 */ s32 scaleoutstart;
    /* 0x68 */ s32 zrot;
    /* 0x6C */ s32 regenparticle;
};
BASH_SIZE_CHECK(partdata, 0x70);

struct script_thread {
    /* 0x00 */ s32 thread_type;
    /* 0x04 */ s32 start_frame;
    /* 0x08 */ s32 end_frame;
    /* 0x0C */ s32 total_frames;
    /* 0x10 */ s32 key_frame_count;
    /* 0x14 */ union {
        /* 0x00 */ partdata particle;
        /* 0x00 */ s32 entity[24]; /* struct ._343 (0x60 bytes), not in the fragment */
        /* 0x00 */ s32 camera[12]; /* struct ._340 (0x30 bytes), not in the fragment */
        /* 0x00 */ script_command_anim anim;
        /* 0x00 */ script_command_fade fade;
        /* 0x00 */ script_command_subscript subscript;
    } type;
};
BASH_SIZE_CHECK(script_thread, 0x84);

struct script_header {
    /* 0x00 */ s32 threadcount;
    /* 0x04 */ s32 subcount;
    /* 0x08 */ s32 end_frame;
    /* 0x0C */ s32 start_frame;
    /* 0x10 */ s32 num_frames;
    /* 0x14 */ u32 spare[2];
    /* 0x1C */ u32 threaddata[1];
};
BASH_SIZE_CHECK(script_header, 0x20);

struct script_thread_handler {
    /* 0x00 */ s32 thread_num;
    /* 0x04 */ s32 thread_type;
    /* 0x08 */ union {
        /* 0x00 */ struct {
            /* 0x00 */ subengine sub;
        } entity;
        /* 0x00 */ struct {
            /* 0x00 */ s32 num_subs;
            /* 0x04 */ subengine *subs;
            /* 0x08 */ subengine *last_sub;
            /* 0x0C */ partmotdata *mdata;
            /* 0x10 */ s32 first;
        } particle;
        /* 0x00 */ struct {
            /* 0x00 */ subengine sub;
            /* 0xA8 */ s32 frame_count;
        } anim;
        /* 0x00 */ struct {
            /* 0x00 */ s32 start;
            /* 0x04 */ s32 end;
        } fade;
        /* 0x00 */ struct {
            /* 0x00 */ subengine sub;
        } subscript;
    } type;
    /* 0xB4 */ script_thread_handler *next;
};
BASH_SIZE_CHECK(script_thread_handler, 0xB8);

/* A playing script (0x1C bytes; the fragment records the size, the fields
 * are from sub_play_script, which sets one up at subengine +0x6C, and the
 * threads that read it). */
struct script_control_info {
    /* 0x00 */ obj_dbase *dbase;
    /* 0x04 */ s32 script;             /* index into the dbase's script table */
    /* 0x08 */ s32 unk8;
    /* 0x0C */ union {
        s32 w;                         /* 16.16 frames */
        struct {
            u16 frac;
            s16 frame;
        } h;
    } time;
    /* 0x10 */ s32 unk10;              /* the script header's +8 */
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
};
BASH_SIZE_CHECK(script_control_info, 0x1C);

struct script_handler {
    /* 0x00 */ script_control_info info;
    /* 0x1C */ s32 xyzrf[21]; /* struct ._319 (0x54 bytes), not in the fragment */
    /* 0x70 */ script_handler *last;
    /* 0x74 */ script_handler *next;
    /* 0x78 */ script_handler **owner;
};
BASH_SIZE_CHECK(script_handler, 0x7C);

struct level_music_info {
    /* 0x00 */ void *music;
    /* 0x04 */ s16 reverb;
    /* 0x08 */ void *sfx;
    /* 0x0C */ void *sfx2;
    /* 0x10 */ s16 tempo;
};
BASH_SIZE_CHECK(level_music_info, 0x14);

struct map_tree {
    /* 0x00 */ u32 flagsplit;
    /* 0x04 */ u32 higher;
    /* 0x08 */ u32 lower;
};
BASH_SIZE_CHECK(map_tree, 0xC);

struct trigger_list {
    /* 0x00 */ u32 num;
    /* 0x04 */ trigger trig[1];
};
BASH_SIZE_CHECK(trigger_list, 0x14);

struct map_map {
    /* 0x00 */ SLONG_XYZ cell_size;
    /* 0x0C */ u32 path_list;
    /* 0x10 */ u32 trigger_list;
    /* 0x14 */ u32 contour_grids;
    /* 0x18 */ u32 xcell_list;
    /* 0x1C */ s32 num_placements;
    /* 0x20 */ u32 placements;
    /* 0x24 */ UBYTE_RGB clear_colour;
    /* 0x27 */ u8 clear;
    /* 0x28 */ u32 spare4;
    /* 0x2C */ u32 spare5;
    /* 0x30 */ u32 spare6;
    /* 0x34 */ u32 xmap_cell_list;
    /* 0x38 */ u32 xnum_cells;
    /* 0x3C */ map_tree xtree[1];
};
BASH_SIZE_CHECK(map_map, 0x48);

struct map_list {
    /* 0x00 */ u32 num_maps;
    /* 0x04 */ u32 map_offset[1];
};
BASH_SIZE_CHECK(map_list, 0x8);

struct map_enviroment {
    /* 0x00 */ obj_dbase *dbase;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u32 map_num;
    /* 0x0C */ map_list *maps;
    /* 0x10 */ map_map *map;
    /* 0x14 */ void *placements;
    /* 0x18 */ s32 num_placements;
    /* 0x1C */ subengine *subs;
    /* 0x20 */ void *map_directory;
    /* 0x24 */ void *map_animations;
    /* 0x28 */ void *path_list;
    /* 0x2C */ u32 *path;
    /* 0x30 */ trigger_list *triggers;
};
BASH_SIZE_CHECK(map_enviroment, 0x34);

enum UserStatus {
    StatusOff,
    StatusIdle,
    StatusActive,
    StatusActive2,
    StatusBusy,
    StatusWaiting,
    StatusPrompted,
    StatusActivate
};

struct FillNAS {
    /* 0x00 */ s32 userAST;
    /* 0x04 */ void (*InitDelete)();
    /* 0x08 */ void (*Handler)();
};
BASH_SIZE_CHECK(FillNAS, 0xC);

struct EXTmemory {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ void *MemStart;
    /* 0x10 */ s32 FirstByte;
};
BASH_SIZE_CHECK(EXTmemory, 0x14);

struct EXTscratchpad {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 var1;
    /* 0x10 */ s32 var2;
    /* 0x14 */ s32 var3;
    /* 0x18 */ s32 var4;
    /* 0x1C */ s32 var5;
    /* 0x20 */ s32 var6;
    /* 0x24 */ s32 var7;
    /* 0x28 */ s32 var8;
};
BASH_SIZE_CHECK(EXTscratchpad, 0x2C);

struct MEXTtype {
    /* 0x00 */ MEXTtype *slotP;
    /* 0x04 */ MEXTtype *slotC;
    /* 0x08 */ MEXTtype *freeP;
    /* 0x0C */ MEXTtype *freeC;
    /* 0x10 */ u32 length;
    /* 0x14 */ NAS *Head;
};
BASH_SIZE_CHECK(MEXTtype, 0x18);

struct MALEXTStruct {
    /* 0x00 */ MEXTtype *slotP;
    /* 0x04 */ MEXTtype *slotC;
    /* 0x08 */ MEXTtype *freeP;
    /* 0x0C */ MEXTtype *freeC;
    /* 0x10 */ u32 length;
    /* 0x14 */ NAS *Head;
    /* 0x18 */ u32 *Heapbase;
    /* 0x1C */ u32 *Heapend;
};
BASH_SIZE_CHECK(MALEXTStruct, 0x20);

struct ANIMtab {
    /* 0x00 */ void *Atable;
    /* 0x04 */ void *Alist;
    /* 0x08 */ u32 type;
};
BASH_SIZE_CHECK(ANIMtab, 0xC);

struct AnimTn {
    /* 0x00 */ u16 frame;
    /* 0x02 */ u16 delay;
};
BASH_SIZE_CHECK(AnimTn, 0x4);

struct REL {
    /* 0x00 */ s16 Dx;
    /* 0x02 */ s16 Dy;
    /* 0x04 */ s16 Dz;
    /* 0x06 */ u16 hypo;
    /* 0x08 */ u16 hypo3;
    /* 0x0A */ u16 dir;
};
BASH_SIZE_CHECK(REL, 0xC);

struct fadescreen_info {
    /* 0x00 */ s32 master[4]; /* struct ._22 (0x10 bytes), not in the fragment */
    /* 0x10 */ u16 speed;
    /* 0x12 */ u16 flags;
    /* 0x14 */ s16 fade;
};
BASH_SIZE_CHECK(fadescreen_info, 0x18);

struct NasMetrics {
    /* 0x00 */ u16 lev99;
    /* 0x02 */ u16 TNas;
    /* 0x04 */ u16 FNas;
    /* 0x06 */ u16 NasHeap;
    /* 0x08 */ u16 TotSubs;
    /* 0x0A */ u16 DoNotUse;
    /* 0x0C */ PreMap *PreMapDB;
    /* 0x10 */ void *ShellSetup;
    /* 0x14 */ void (*LevelSetup)();
};
BASH_SIZE_CHECK(NasMetrics, 0x18);

struct rules_info {
    /* 0x00 */ void **trophy_first;
    /* 0x04 */ void **trophy_ingame;
    /* 0x08 */ void **challenge_first;
    /* 0x0C */ void **challenge_ingame;
    /* 0x10 */ void **gem_first;
    /* 0x14 */ void **gem_ingame;
    /* 0x18 */ void **relicg_first;
    /* 0x1C */ void **relicg_ingame;
    /* 0x20 */ void **relicp_first;
    /* 0x24 */ void **relicp_ingame;
    /* 0x28 */ void **battle_first;
    /* 0x2C */ void **battle_ingame;
};
BASH_SIZE_CHECK(rules_info, 0x30);

struct scrolling_credits {
    /* 0x00 */ s32 type;
    /* 0x04 */ s8 *str;
    /* 0x08 */ s32 value;
};
BASH_SIZE_CHECK(scrolling_credits, 0xC);

struct geom_info {
    /* 0x00 */ void *rot;
    /* 0x04 */ void *trans;
    /* 0x08 */ SWORD_XY origin;
};
BASH_SIZE_CHECK(geom_info, 0xC);

struct draw_window {
    /* 0x00 */ s32 clip[2]; /* struct ._37 (0x8 bytes), not in the fragment */
    /* 0x08 */ s32 screen[2]; /* struct ._37 (0x8 bytes), not in the fragment */
    /* 0x10 */ SLONG_XY origin;
    /* 0x18 */ s32 zstart;
    /* 0x1C */ s32 zsize;
    /* 0x20 */ s32 zadjust;
    /* 0x24 */ UBYTE_RGB clear_colour;
    /* 0x27 */ u8 clear;
};
BASH_SIZE_CHECK(draw_window, 0x28);

struct matrix_stack {
    /* 0x00 */ s32 m[8]; /* struct ._21 (0x20 bytes), not in the fragment */
    /* 0x20 */ void *faces;
};
BASH_SIZE_CHECK(matrix_stack, 0x24);

struct obj_globals {
    /* 0x00 */ obj_dbase *dbase;
    /* 0x04 */ EXTengine *eng;
    /* 0x08 */ void *fade_colour;
    /* 0x0C */ s32 fade;
    /* 0x10 */ draw_window *window;
    /* 0x14 */ s32 global_fade;
    /* 0x18 */ s32 screen_clip[2]; /* struct ._37 (0x8 bytes), not in the fragment */
    /* 0x20 */ SLONG_XY render_origin;
    /* 0x28 */ SLONG_XY draw_origin;
    /* 0x30 */ s32 ot_adjust;
    /* 0x34 */ s32 sv[2]; /* struct ._23 (0x8 bytes), not in the fragment */
    /* 0x3C */ s16 num_palettes;
    /* 0x3E */ s16 num_textures;
    /* 0x40 */ u32 *otlist;
    /* 0x44 */ s16 ot_offset;
    /* 0x46 */ s16 otmax;
    /* 0x48 */ struct camera_info_tag *cam;
    /* 0x4C */ s32 scratch_matrix[8]; /* struct ._21 (0x20 bytes), not in the fragment */
    /* 0x6C */ s32 scratch_sv[2]; /* struct ._23 (0x8 bytes), not in the fragment */
    /* 0x74 */ s32 scratch_v[4]; /* struct ._22 (0x10 bytes), not in the fragment */
    /* 0x84 */ void *msp;
    /* 0x88 */ void *vsp;
    /* 0x8C */ s32 vap_matrix[8]; /* struct ._21 (0x20 bytes), not in the fragment */
    /* 0xAC */ s32 tm[8]; /* struct ._21 (0x20 bytes), not in the fragment */
    /* 0xCC */ s32 rot_matrix_stack[16]; /* struct ._21 (0x40 bytes), not in the fragment */
    /* 0x10C */ s32 trans_vector_stack[8]; /* struct ._22 (0x20 bytes), not in the fragment */
};
BASH_SIZE_CHECK(obj_globals, 0x12C);

struct MapTrigger4Longs {
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u32 val1;
    /* 0x08 */ u32 val2;
    /* 0x0C */ u32 val3;
    /* 0x10 */ u32 val4;
};
BASH_SIZE_CHECK(MapTrigger4Longs, 0x14);

struct TrigMarker {
    /* 0x00 */ SLONG_XYZ xyz;
    /* 0x0C */ s32 val2;
    /* 0x10 */ s32 val3;
    /* 0x14 */ s32 val4;
};
BASH_SIZE_CHECK(TrigMarker, 0x18);

struct TRGBOX {
    /* 0x00 */ s32 x1;
    /* 0x04 */ s32 x2;
    /* 0x08 */ s32 z1;
    /* 0x0C */ s32 z2;
};
BASH_SIZE_CHECK(TRGBOX, 0x10);

enum BasicAcmds {
    Acmd_restart = 0x8000,
    Acmd_rts,
    Acmd_xdir,
    Acmd_delete,
    Acmd_killme,
    Acmd_reply,
    Acmd_send,
    Acmd_xflip,
    Acmd_yflip,
    Acmd_zflip,
    Acmd_no_flip,
    Acmd_sfx,
    Acmd_sfx_pos,
    Acmd_sfx_handled,
    Acmd_sfx_omni,
    Acmd_delayspeed,
    Acmd_dbase,
    Acmd_sequence,
    Acmd_seq_down
};

enum AnimCommands {
    ACom_none,
    ACom_error,
    ACom_restart,
    ACom_rts,
    ACom_stop,
    ACom_start,
    ACom_slow,
    ACom_go,
    ACom_think,
    ACom_think_again,
    ACom_jump,
    ACom_change,
    ACom_action1,
    ACom_action2,
    ACom_action3,
    ACom_action4,
    ACom_throw,
    ACom_throw1,
    ACom_throw2,
    ACom_throw3,
    ACom_throw4
};

struct AcmdFX {
    /* 0x00 */ void (*AnimTs)();
    /* 0x04 */ void (*AnimTn)();
};
BASH_SIZE_CHECK(AcmdFX, 0x8);

enum {
    DBASE_NULL,
    DBASE_CRASH,
    DBASE_COCO,
    DBASE_CORTEX,
    DBASE_BRIO,
    DBASE_TINY,
    DBASE_KONG,
    DBASE_DINGO,
    DBASE_KANGA,
    DBASE_CRASH_PG,
    DBASE_COCO_PG,
    DBASE_CORTEX_PG,
    DBASE_BRIO_PG,
    DBASE_TINY_PG,
    DBASE_DINGO_PG,
    DBASE_KONG_PG,
    DBASE_KANGA_PG,
    DBASE_CRASH_PB,
    DBASE_COCO_PB,
    DBASE_CORTEX_PB,
    DBASE_TINY_PB,
    DBASE_DINGO_PB,
    DBASE_BRIO_PB,
    DBASE_GENERIX,
    DBASE_CRATES,
    DBASE_BALLOON,
    DBASE_GHOST,
    DBASE_PENGUIN,
    DBASE_PANEL,
    DBASE_PANEL_A,
    DBASE_PANEL_B,
    DBASE_PANEL_C,
    DBASE_PANEL_D,
    DBASE_PANEL_E,
    DBASE_SPACE,
    DBASE_SPACEY,
    DBASE_JUNGLE,
    DBASE_BUNGLE,
    DBASE_SNOW,
    DBASE_SNOWC,
    DBASE_DRAINS,
    DBASE_BRAINS,
    DBASE_TIE,
    DBASE_AZTEC,
    DBASE_PAPU,
    DBASE_MINION,
    DBASE_TANK0,
    DBASE_TANK1,
    DBASE_TANK2,
    DBASE_TANK3,
    DBASE_TANK4,
    DBASE_TANK0CHL,
    DBASE_TANK1CHL,
    DBASE_TANK2CHL,
    DBASE_TANK3CHL,
    DBASE_XTRA_TA,
    DBASE_TANK,
    DBASE_TANKBOSS,
    DBASE_MOEJOE,
    DBASE_MJTANK,
    DBASE_BOAT,
    DBASE_TOTEM,
    DBASE_CRASH_TA,
    DBASE_COCO_TA,
    DBASE_CORTX_TA,
    DBASE_TINY_TA,
    DBASE_BRIO_TA,
    DBASE_DINGO_TA,
    DBASE_KONG_TA,
    DBASE_KANGA_TA,
    DBASE_CHICKEN,
    DBASE_POGOPAINTER,
    DBASE_POGOAGOGO,
    DBASE_POGOLOCO,
    DBASE_POGOBOUNCER,
    DBASE_POGOPAINTERC,
    DBASE_POGOAGOGOC,
    DBASE_POGOLOCOC,
    DBASE_POGOBOUNCERC,
    DBASE_XTRA_PG,
    DBASE_ROO,
    DBASE_BONK,
    DBASE_BONKC,
    DBASE_XTRAS_BO,
    DBASE_CRASH_BO,
    DBASE_COCO_BO,
    DBASE_CORTEX_BO,
    DBASE_BRIO_BO,
    DBASE_DINGO_BO,
    DBASE_KANGA_BO,
    DBASE_TINY_BO,
    DBASE_KONG_BO,
    DBASE_OXIDE,
    DBASE_NOXIDE,
    DBASE_BOSSSHIP,
    DBASE_CHASE,
    DBASE_CHASECHL,
    DBASE_XTRA_CH,
    DBASE_CRASH_CH,
    DBASE_COCO_CH,
    DBASE_CORTX_CH,
    DBASE_TINY_CH,
    DBASE_BRIO_CH,
    DBASE_DINGO_CH,
    DBASE_KONG_CH,
    DBASE_KANGA_CH,
    DBASE_ARK,
    DBASE_ARKCHAL,
    DBASE_SEABALL,
    DBASE_SEABALL2,
    DBASE_NGINBALL,
    DBASE_NG2BALL,
    DBASE_SKYBALL,
    DBASE_STARBALL,
    DBASE_CRASH_AR,
    DBASE_COCO_AR,
    DBASE_CORTX_AR,
    DBASE_BRIO_AR,
    DBASE_DINGO_AR,
    DBASE_KANGA_AR,
    DBASE_TINY_AR,
    DBASE_KONG_AR,
    DBASE_XTRA_AR,
    DBASE_SAUCER,
    DBASE_N_GIN,
    DBASE_KNOCKOUT,
    DBASE_KNOCKC,
    DBASE_POLARDAY,
    DBASE_BEAR,
    DBASE_MASKS,
    DBASE_POLARFX,
    DBASE_POLARNGT,
    DBASE_POLARCUT,
    DBASE_WALRUS,
    DBASE_POLAREND,
    DBASE_CRASH_FT,
    DBASE_COCO_FT,
    DBASE_CORTEX_FT,
    DBASE_TINY_FT,
    DBASE_DINGO_FT,
    DBASE_BRIO_FT,
    DBASE_POLARBSS,
    DBASE_BIGBEAR,
    DBASE_GFXTEST,
    DBASE_KONG_PB,
    DBASE_MINATOR,
    DBASE_KANGA_PB,
    DBASE_POLARCHA,
    DBASE_KANGA_FT,
    DBASE_KONG_FT,
    DBASE_POLARENC,
    DBASE_POLARCUC,
    DBASE_POLARNGC,
    DBASE_CRASHPB,
    DBASE_COCOPB,
    DBASE_CORTEXPB,
    DBASE_TINYPB,
    DBASE_DINGOPB,
    DBASE_BRIOPB,
    DBASE_KANGAPB,
    DBASE_KONGPB,
    DBASE_DASH,
    DBASE_CRASH_RA,
    DBASE_CORTX_RA,
    DBASE_TINY_RA,
    DBASE_DINGO_RA,
    DBASE_BRIO_RA,
    DBASE_SLASH,
    DBASE_HASH,
    DBASE_CASH,
    DBASE_RACEFX,
    DBASE_COCO_RA,
    DBASE_KANGA_RA,
    DBASE_KONG_RA,
    DBASE_CRASH_WT,
    DBASE_CORTX_WT,
    DBASE_TINY_WT,
    DBASE_DINGO_WT,
    DBASE_KONG_WT,
    DBASE_BRIO_WT,
    DBASE_COCO_WT,
    DBASE_KANGA_WT,
    DBASE_SLASHC,
    DBASE_CASHC,
    DBASE_DASHC,
    DBASE_HASHC,
    DBASE_DINO,
    DBASE_CRASH_DI,
    DBASE_CORTX_DI,
    DBASE_TINY_DI,
    DBASE_DINGO_DI,
    DBASE_COCO_DI,
    DBASE_KANGA_DI,
    DBASE_BRIO_DI,
    DBASE_KONG_DI,
    DBASE_DINOC,
    DBASE_DINOFX,
    DBASE_TITLES_MAP,
    DBASE_LOADING,
    DBASE_XTRA_TI,
    DBASE_MINIMAPS,
    DBASE_MINIMAP2,
    DBASE_FONT,
    DBASE_EUROLOGO,
    DBASE_CRASHLOG,
    DBASE_PODIUM_MAP,
    DBASE_HUB0_MAP,
    DBASE_HUB1_MAP,
    DBASE_HUB2_MAP,
    DBASE_HUB3_MAP,
    DBASE_HUB4_MAP,
    DBASE_HUBDEMO_MAP,
    DBASE_HUB_2LEV_MAP,
    DBASE_XTRAS_HU,
    DBASE_CRASH_HU,
    DBASE_COCO_HU,
    DBASE_CORTX_HU,
    DBASE_BRIO_HU,
    DBASE_TINY_HU,
    DBASE_KONG_HU,
    DBASE_DINGO_HU,
    DBASE_KANGA_HU,
    DBASE_INTRO1_A,
    DBASE_INTRO1_B,
    DBASE_INTRO1_C,
    DBASE_INTRO1_D,
    DBASE_INTRO1_E,
    DBASE_INTRO1_F,
    DBASE_INTRO1_G,
    DBASE_INTRO2_A,
    DBASE_INTRO2_B,
    DBASE_INTRO2_C,
    DBASE_INTRO2C1,
    DBASE_INTRO2C2,
    DBASE_INTRO2C3,
    DBASE_INTRO2C4,
    DBASE_INTRO2C5,
    DBASE_INTRO2_D,
    DBASE_INTRO2_E,
    DBASE_INTRO2_F,
    DBASE_INTRO2_G,
    DBASE_INTRO2_H,
    DBASE_WIN_M_1,
    DBASE_WIN_M_2,
    DBASE_WIN_M_3,
    DBASE_WIN_M_4,
    DBASE_GOOD_A,
    DBASE_GOOD_B,
    DBASE_GOOD_C,
    DBASE_GOOD_D,
    DBASE_GOOD_E,
    DBASE_GOOD_F,
    DBASE_GOOD_G,
    DBASE_GOOD_H,
    DBASE_GOOD_I,
    DBASE_GOOD_J,
    DBASE_GOOD_K,
    DBASE_GOOD_L,
    DBASE_EVIL_A,
    DBASE_EVIL_B,
    DBASE_EVIL_C,
    DBASE_EVIL_D,
    DBASE_EVIL_E,
    DBASE_EVIL_F,
    DBASE_EVIL_G,
    DBASE_EVIL_H,
    DBASE_EVIL_H01,
    DBASE_EVIL_H02,
    DBASE_WIN_G_01,
    DBASE_WIN_E_01,
    DBASE_WIN_M_01,
    DBASE_CREDS_01,
    DBASE_INTRO_PH,
    DBASE_EBEAR_IN,
    DBASE_EPAPU_IN,
    DBASE_EOXID_IN,
    DBASE_ETANK_IN,
    DBASE_GBEAR_IN,
    DBASE_GPAPU_IN,
    DBASE_GOXID_IN,
    DBASE_GTANK_IN,
    DBASE_MOXID_IN,
    DBASE_MTANK_IN,
    MAX_dbase_num
};

enum {
    FREE_DBASE_NULL = 0x4000,
    FREE_DBASE_CRASH,
    FREE_DBASE_COCO,
    FREE_DBASE_CORTEX,
    FREE_DBASE_BRIO,
    FREE_DBASE_TINY,
    FREE_DBASE_KONG,
    FREE_DBASE_DINGO,
    FREE_DBASE_KANGA,
    FREE_DBASE_CRASH_PG,
    FREE_DBASE_COCO_PG,
    FREE_DBASE_CORTEX_PG,
    FREE_DBASE_BRIO_PG,
    FREE_DBASE_TINY_PG,
    FREE_DBASE_DINGO_PG,
    FREE_DBASE_KONG_PG,
    FREE_DBASE_KANGA_PG,
    FREE_DBASE_CRASH_PB,
    FREE_DBASE_COCO_PB,
    FREE_DBASE_CORTEX_PB,
    FREE_DBASE_TINY_PB,
    FREE_DBASE_DINGO_PB,
    FREE_DBASE_BRIO_PB,
    FREE_DBASE_GENERIX,
    FREE_DBASE_CRATES,
    FREE_DBASE_BALLOON,
    FREE_DBASE_GHOST,
    FREE_DBASE_PENGUIN,
    FREE_DBASE_PANEL,
    FREE_DBASE_PANEL_A,
    FREE_DBASE_PANEL_B,
    FREE_DBASE_PANEL_C,
    FREE_DBASE_PANEL_D,
    FREE_DBASE_PANEL_E,
    FREE_DBASE_SPACE,
    FREE_DBASE_SPACEY,
    FREE_DBASE_JUNGLE,
    FREE_DBASE_BUNGLE,
    FREE_DBASE_SNOW,
    FREE_DBASE_SNOWC,
    FREE_DBASE_DRAINS,
    FREE_DBASE_BRAINS,
    FREE_DBASE_TIE,
    FREE_DBASE_AZTEC,
    FREE_DBASE_PAPU,
    FREE_DBASE_MINION,
    FREE_DBASE_TANK0,
    FREE_DBASE_TANK1,
    FREE_DBASE_TANK2,
    FREE_DBASE_TANK3,
    FREE_DBASE_TANK4,
    FREE_DBASE_TANK0CHL,
    FREE_DBASE_TANK1CHL,
    FREE_DBASE_TANK2CHL,
    FREE_DBASE_TANK3CHL,
    FREE_DBASE_XTRA_TA,
    FREE_DBASE_TANK,
    FREE_DBASE_TANKBOSS,
    FREE_DBASE_MOEJOE,
    FREE_DBASE_MJTANK,
    FREE_DBASE_BOAT,
    FREE_DBASE_TOTEM,
    FREE_DBASE_CRASH_TA,
    FREE_DBASE_COCO_TA,
    FREE_DBASE_CORTX_TA,
    FREE_DBASE_TINY_TA,
    FREE_DBASE_BRIO_TA,
    FREE_DBASE_DINGO_TA,
    FREE_DBASE_KONG_TA,
    FREE_DBASE_KANGA_TA,
    FREE_DBASE_CHICKEN,
    FREE_DBASE_POGOPAINTER,
    FREE_DBASE_POGOAGOGO,
    FREE_DBASE_POGOLOCO,
    FREE_DBASE_POGOBOUNCER,
    FREE_DBASE_POGOPAINTERC,
    FREE_DBASE_POGOAGOGOC,
    FREE_DBASE_POGOLOCOC,
    FREE_DBASE_POGOBOUNCERC,
    FREE_DBASE_XTRA_PG,
    FREE_DBASE_ROO,
    FREE_DBASE_BONK,
    FREE_DBASE_BONKC,
    FREE_DBASE_XTRAS_BO,
    FREE_DBASE_CRASH_BO,
    FREE_DBASE_COCO_BO,
    FREE_DBASE_CORTEX_BO,
    FREE_DBASE_BRIO_BO,
    FREE_DBASE_DINGO_BO,
    FREE_DBASE_KANGA_BO,
    FREE_DBASE_TINY_BO,
    FREE_DBASE_KONG_BO,
    FREE_DBASE_OXIDE,
    FREE_DBASE_NOXIDE,
    FREE_DBASE_BOSSSHIP,
    FREE_DBASE_CHASE,
    FREE_DBASE_CHASECHL,
    FREE_DBASE_XTRA_CH,
    FREE_DBASE_CRASH_CH,
    FREE_DBASE_COCO_CH,
    FREE_DBASE_CORTX_CH,
    FREE_DBASE_TINY_CH,
    FREE_DBASE_BRIO_CH,
    FREE_DBASE_DINGO_CH,
    FREE_DBASE_KONG_CH,
    FREE_DBASE_KANGA_CH,
    FREE_DBASE_ARK,
    FREE_DBASE_ARKCHAL,
    FREE_DBASE_SEABALL,
    FREE_DBASE_SEABALL2,
    FREE_DBASE_NGINBALL,
    FREE_DBASE_NG2BALL,
    FREE_DBASE_SKYBALL,
    FREE_DBASE_STARBALL,
    FREE_DBASE_CRASH_AR,
    FREE_DBASE_COCO_AR,
    FREE_DBASE_CORTX_AR,
    FREE_DBASE_BRIO_AR,
    FREE_DBASE_DINGO_AR,
    FREE_DBASE_KANGA_AR,
    FREE_DBASE_TINY_AR,
    FREE_DBASE_KONG_AR,
    FREE_DBASE_XTRA_AR,
    FREE_DBASE_SAUCER,
    FREE_DBASE_N_GIN,
    FREE_DBASE_KNOCKOUT,
    FREE_DBASE_KNOCKC,
    FREE_DBASE_POLARDAY,
    FREE_DBASE_BEAR,
    FREE_DBASE_MASKS,
    FREE_DBASE_POLARFX,
    FREE_DBASE_POLARNGT,
    FREE_DBASE_POLARCUT,
    FREE_DBASE_WALRUS,
    FREE_DBASE_POLAREND,
    FREE_DBASE_CRASH_FT,
    FREE_DBASE_COCO_FT,
    FREE_DBASE_CORTEX_FT,
    FREE_DBASE_TINY_FT,
    FREE_DBASE_DINGO_FT,
    FREE_DBASE_BRIO_FT,
    FREE_DBASE_POLARBSS,
    FREE_DBASE_BIGBEAR,
    FREE_DBASE_GFXTEST,
    FREE_DBASE_KONG_PB,
    FREE_DBASE_MINATOR,
    FREE_DBASE_KANGA_PB,
    FREE_DBASE_POLARCHA,
    FREE_DBASE_KANGA_FT,
    FREE_DBASE_KONG_FT,
    FREE_DBASE_POLARENC,
    FREE_DBASE_POLARCUC,
    FREE_DBASE_POLARNGC,
    FREE_DBASE_CRASHPB,
    FREE_DBASE_COCOPB,
    FREE_DBASE_CORTEXPB,
    FREE_DBASE_TINYPB,
    FREE_DBASE_DINGOPB,
    FREE_DBASE_BRIOPB,
    FREE_DBASE_KANGAPB,
    FREE_DBASE_KONGPB,
    FREE_DBASE_DASH,
    FREE_DBASE_CRASH_RA,
    FREE_DBASE_CORTX_RA,
    FREE_DBASE_TINY_RA,
    FREE_DBASE_DINGO_RA,
    FREE_DBASE_BRIO_RA,
    FREE_DBASE_SLASH,
    FREE_DBASE_HASH,
    FREE_DBASE_CASH,
    FREE_DBASE_RACEFX,
    FREE_DBASE_COCO_RA,
    FREE_DBASE_KANGA_RA,
    FREE_DBASE_KONG_RA,
    FREE_DBASE_CRASH_WT,
    FREE_DBASE_CORTX_WT,
    FREE_DBASE_TINY_WT,
    FREE_DBASE_DINGO_WT,
    FREE_DBASE_KONG_WT,
    FREE_DBASE_BRIO_WT,
    FREE_DBASE_COCO_WT,
    FREE_DBASE_KANGA_WT,
    FREE_DBASE_SLASHC,
    FREE_DBASE_CASHC,
    FREE_DBASE_DASHC,
    FREE_DBASE_HASHC,
    FREE_DBASE_DINO,
    FREE_DBASE_CRASH_DI,
    FREE_DBASE_CORTX_DI,
    FREE_DBASE_TINY_DI,
    FREE_DBASE_DINGO_DI,
    FREE_DBASE_COCO_DI,
    FREE_DBASE_KANGA_DI,
    FREE_DBASE_BRIO_DI,
    FREE_DBASE_KONG_DI,
    FREE_DBASE_DINOC,
    FREE_DBASE_DINOFX,
    FREE_DBASE_TITLES_MAP,
    FREE_DBASE_LOADING,
    FREE_DBASE_XTRA_TI,
    FREE_DBASE_MINIMAPS,
    FREE_DBASE_MINIMAP2,
    FREE_DBASE_FONT,
    FREE_DBASE_EUROLOGO,
    FREE_DBASE_CRASHLOG,
    FREE_DBASE_PODIUM_MAP,
    FREE_DBASE_HUB0_MAP,
    FREE_DBASE_HUB1_MAP,
    FREE_DBASE_HUB2_MAP,
    FREE_DBASE_HUB3_MAP,
    FREE_DBASE_HUB4_MAP,
    FREE_DBASE_HUBDEMO_MAP,
    FREE_DBASE_HUB_2LEV_MAP,
    FREE_DBASE_XTRAS_HU,
    FREE_DBASE_CRASH_HU,
    FREE_DBASE_COCO_HU,
    FREE_DBASE_CORTX_HU,
    FREE_DBASE_BRIO_HU,
    FREE_DBASE_TINY_HU,
    FREE_DBASE_KONG_HU,
    FREE_DBASE_DINGO_HU,
    FREE_DBASE_KANGA_HU,
    FREE_DBASE_INTRO1_A,
    FREE_DBASE_INTRO1_B,
    FREE_DBASE_INTRO1_C,
    FREE_DBASE_INTRO1_D,
    FREE_DBASE_INTRO1_E,
    FREE_DBASE_INTRO1_F,
    FREE_DBASE_INTRO1_G,
    FREE_DBASE_INTRO2_A,
    FREE_DBASE_INTRO2_B,
    FREE_DBASE_INTRO2_C,
    FREE_DBASE_INTRO2C1,
    FREE_DBASE_INTRO2C2,
    FREE_DBASE_INTRO2C3,
    FREE_DBASE_INTRO2C4,
    FREE_DBASE_INTRO2C5,
    FREE_DBASE_INTRO2_D,
    FREE_DBASE_INTRO2_E,
    FREE_DBASE_INTRO2_F,
    FREE_DBASE_INTRO2_G,
    FREE_DBASE_INTRO2_H,
    FREE_DBASE_WIN_M_1,
    FREE_DBASE_WIN_M_2,
    FREE_DBASE_WIN_M_3,
    FREE_DBASE_WIN_M_4,
    FREE_DBASE_GOOD_A,
    FREE_DBASE_GOOD_B,
    FREE_DBASE_GOOD_C,
    FREE_DBASE_GOOD_D,
    FREE_DBASE_GOOD_E,
    FREE_DBASE_GOOD_F,
    FREE_DBASE_GOOD_G,
    FREE_DBASE_GOOD_H,
    FREE_DBASE_GOOD_I,
    FREE_DBASE_GOOD_J,
    FREE_DBASE_GOOD_K,
    FREE_DBASE_GOOD_L,
    FREE_DBASE_EVIL_A,
    FREE_DBASE_EVIL_B,
    FREE_DBASE_EVIL_C,
    FREE_DBASE_EVIL_D,
    FREE_DBASE_EVIL_E,
    FREE_DBASE_EVIL_F,
    FREE_DBASE_EVIL_G,
    FREE_DBASE_EVIL_H,
    FREE_DBASE_EVIL_H01,
    FREE_DBASE_EVIL_H02,
    FREE_DBASE_WIN_G_01,
    FREE_DBASE_WIN_E_01,
    FREE_DBASE_WIN_M_01,
    FREE_DBASE_CREDS_01,
    FREE_DBASE_INTRO_PH,
    FREE_DBASE_EBEAR_IN,
    FREE_DBASE_EPAPU_IN,
    FREE_DBASE_EOXID_IN,
    FREE_DBASE_ETANK_IN,
    FREE_DBASE_GBEAR_IN,
    FREE_DBASE_GPAPU_IN,
    FREE_DBASE_GOXID_IN,
    FREE_DBASE_GTANK_IN,
    FREE_DBASE_MOXID_IN,
    FREE_DBASE_MTANK_IN
};

struct particledata {
    /* 0x00 */ s32 pFlag;
    /* 0x04 */ s32 cFlag;
    /* 0x08 */ s32 effectflags;
    /* 0x0C */ s32 plife;
    /* 0x10 */ SLONG_XYZ vel;
};
BASH_SIZE_CHECK(particledata, 0x1C);

struct EXTparticlemaster {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 maxparticles;
    /* 0x10 */ s32 addamount;
    /* 0x14 */ s32 addfrequency;
    /* 0x18 */ s32 effectduration;
    /* 0x1C */ s32 particlelifetime;
    /* 0x20 */ s32 activecount;
    /* 0x24 */ s32 emittertype;
    /* 0x28 */ s32 emitterradius;
    /* 0x2C */ s32 startscale;
    /* 0x30 */ SLONG_XYZ gravity;
    /* 0x3C */ s32 speed;
    /* 0x40 */ s32 flags;
    /* 0x44 */ s32 frame;
    /* 0x48 */ particledata pdata[16];
};
BASH_SIZE_CHECK(EXTparticlemaster, 0x208);

struct box {
    /* 0x00 */ SLONG_XYZ min;
    /* 0x0C */ SLONG_XYZ max;
};
BASH_SIZE_CHECK(box, 0x18);

struct sphere {
    /* 0x00 */ s32 radius;
    /* 0x04 */ SLONG_XYZ *centre;
};
BASH_SIZE_CHECK(sphere, 0x8);

struct pcontrol {
    /* 0x00 */ s32 fadeinrate;
    /* 0x04 */ s32 fadeoutrate;
    /* 0x08 */ s32 scaleinrate;
    /* 0x0C */ s32 scaleoutrate;
    /* 0x10 */ s32 freqcounter;
    /* 0x14 */ s32 emitterang[4]; /* struct ._22 (0x10 bytes), not in the fragment */
    /* 0x24 */ subengine *lastsub;
};
BASH_SIZE_CHECK(pcontrol, 0x28);

struct threaddata {
    /* 0x00 */ s32 threadtype;
    /* 0x04 */ s32 threadnumber;
    /* 0x08 */ s32 keyindex;
    /* 0x0C */ s32 keycount;
    /* 0x10 */ void *fdata;
    /* 0x14 */ SLONG_XYZ start;
    /* 0x20 */ s32 framecount;
    /* 0x24 */ s32 scriptnumber;
    /* 0x28 */ s32 timer;
    /* 0x2C */ s32 particlesactive;
    /* 0x30 */ void *threadinfo;
    /* 0x34 */ void *threadcontrol;
};
BASH_SIZE_CHECK(threaddata, 0x38);

struct EXTscriptdata {
    /* 0x00 */ s32 EXTid;
    /* 0x04 */ EXTnode Extend;
    /* 0x0C */ s32 scriptnumber;
    /* 0x10 */ s32 subcount;
    /* 0x14 */ s32 timer;
    /* 0x18 */ s32 speed;
    /* 0x1C */ s32 totalframes;
    /* 0x20 */ s32 flags;
    /* 0x24 */ s32 times;
    /* 0x28 */ s32 maxloops;
    /* 0x2C */ threaddata threads[16];
    /* 0x3AC */ s32 threadcount;
    /* 0x3B0 */ s32 keycount;
    /* 0x3B4 */ s32 entityframe;
    /* 0x3B8 */ obj_dbase *dbase;
};
BASH_SIZE_CHECK(EXTscriptdata, 0x3BC);

#endif /* BASH_CORE_H */
