#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Engine types used by the minigame headers (bash/tank.h, pogo.h, arkenoid.h,
 * polar.h). The leak names them but does not define them, and their layouts are
 * not recovered yet (roadmap phase 1, "engine core types").
 *
 * They are incomplete here, so they can only be used through pointers. A
 * minigame declaration that needs one of them complete is guarded by
 * BASH_HAVE_<TYPE>. When a layout is recovered, define the type here and
 * define its guard next to it. */

#include "common.h"

/* Provisional: the leak uses bool but never defines it, and its size is not
 * confirmed from code yet. */
typedef s32 bool;

typedef struct NAS NAS;                     /* engine object ("AST" types: Init/Plot) */
typedef struct NASTrigger NASTrigger;
typedef struct EXTnode EXTnode;             /* first member of every EXT* extension (by value) */
typedef struct EXTengine EXTengine;
typedef struct EXTobject EXTobject;
typedef struct EXTobject_TA EXTobject_TA;
typedef struct EXTmotion EXTmotion;
typedef struct EXTheromove EXTheromove;
typedef struct EXTbotai EXTbotai;
typedef struct subengine subengine;
typedef struct SLONG_XYZ SLONG_XYZ;         /* has x and z members (polar.h) */
typedef struct hero_seq hero_seq;           /* hero animation sequence */
typedef struct TA_player TA_player;         /* Tank player state */
typedef struct PreMap PreMap;
typedef struct LevSpec LevSpec;
typedef struct shell_setup shell_setup;
typedef struct file_group_load_info file_group_load_info;

#endif /* BASH_ENGINE_H */
