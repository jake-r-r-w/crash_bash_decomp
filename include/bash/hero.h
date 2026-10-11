#ifndef BASH_HERO_H
#define BASH_HERO_H

/* The heroes (the four players): AST ids, character names, hero commands,
 * requests, animation sequences and modes, and the boss and hook structs.
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/core.h"

typedef struct HEROjumpvel HEROjumpvel;
typedef struct WinInf WinInf;
typedef struct HookInf HookInf;
typedef struct BossInfo BossInfo;
typedef struct BossMasterInfo BossMasterInfo;
typedef struct HEROFunc HEROFunc;
typedef struct TweakInfo TweakInfo;
typedef struct grid_node grid_node;

enum HERO_AST_types {
    HERO_AST_zero,
    Null_AST,
    ASTzero,
    AST_Hero,
    AST_hero2
};

enum {
    nobody,
    Crash,
    Coco,
    Cortex,
    Brio,
    Tiny,
    Kong,
    Dingodile,
    Kanga,
    Off,
    HNames_MAX
};

struct HEROjumpvel {
    /* 0x00 */ u16 vel;
    /* 0x02 */ u16 max;
    /* 0x04 */ u16 acc;
    /* 0x06 */ u16 dec;
};
BASH_SIZE_CHECK(HEROjumpvel, 0x8);

struct WinInf {
    /* 0x00 */ s8 *name;
    /* 0x04 */ u16 seq;
};
BASH_SIZE_CHECK(WinInf, 0x8);

struct HookInf {
    /* 0x00 */ void (*InitHero)();
    /* 0x04 */ u16 *DataBase;
    /* 0x08 */ u16 *AuxDataBase;
    /* 0x0C */ u16 (*HeroSpeed)[3];
    /* 0x10 */ HEROjumpvel *HeroJumpVels;
    /* 0x14 */ WinInf *WinAnim;
    /* 0x18 */ void (*CreateShadow)();
    /* 0x1C */ void (*UpdateShadow)();
    /* 0x20 */ void (*PreMoveHero)();
    /* 0x24 */ void (*MoveHero)();
    /* 0x28 */ void (*PostMoveHero)();
    /* 0x2C */ void (*RampHero)();
    /* 0x30 */ void (*InitIdleMode)();
    /* 0x34 */ void (*SetIdleMode)();
    /* 0x38 */ void (*HeroBot)();
    /* 0x3C */ s32 (*TestActions)();
    /* 0x40 */ s32 (*TestRun)();
    /* 0x44 */ s32 (*TestJump)();
    /* 0x48 */ s32 (*TestTaunt)();
    /* 0x4C */ NAS *(*TestHero2ObjectCollision)();
    /* 0x50 */ NAS *(*TestHero2PickupObject)();
    /* 0x54 */ s32 (*TestHeroObstacles)();
    /* 0x58 */ s32 (*TestObstacles)();
    /* 0x5C */ void (*PowerUp)();
    /* 0x60 */ void (*Touched)();
    /* 0x64 */ void (*ChooseWinner)();
    /* 0x68 */ s32 (*CheckBounds)();
};
BASH_SIZE_CHECK(HookInf, 0x6C);

struct BossInfo {
    /* 0x00 */ NAS *boss;
    /* 0x04 */ EXTengine *boss_eng;
    /* 0x08 */ subengine *boss_sub;
    /* 0x0C */ EXTengine *boss_shadow;
    /* 0x10 */ EXTengine *boss_fx;
    /* 0x14 */ EXTmotion *boss_move;
    /* 0x18 */ EXTanimator *boss_anim;
    /* 0x1C */ EXTobject *boss_object;
    /* 0x20 */ u16 boss_mode;
    /* 0x22 */ u16 boss_name;
    /* 0x24 */ u16 boss_status;
    /* 0x26 */ s16 boss_health;
    /* 0x28 */ u16 boss_hit;
    /* 0x2A */ u16 boss_frozen;
    /* 0x2C */ u16 boss_weapon_count;
    /* 0x2E */ s16 boss_weapon_timer;
    /* 0x30 */ u16 boss_attack_timer;
    /* 0x32 */ u16 boss_attack_delay;
    /* 0x34 */ SLONG_XYZ boss_origin;
};
BASH_SIZE_CHECK(BossInfo, 0x40);

struct BossMasterInfo {
    /* 0x00 */ u16 boss_mode;
    /* 0x02 */ u16 boss_name;
    /* 0x04 */ u16 boss_status;
    /* 0x06 */ s16 boss_health;
    /* 0x08 */ u16 boss_frozen;
};
BASH_SIZE_CHECK(BossMasterInfo, 0xA);

enum HEROcmds {
    HEROcmd_loop = 0x8000,
    HEROcmd_new_base,
    HEROcmd_abort,
    HEROcmd_request,
    HEROcmd_mode,
    HEROcmd_anim,
    HEROcmd_dbase,
    HEROcmd_sequence,
    HEROcmd_SFX,
    HEROcmd_SFXpos,
    HEROcmd_SFXhandled,
    HEROcmd_SFXdelete,
    HEROcmd_SFXloop,
    HEROcmd_SFXloop_off,
    HEROcmd_XA,
    HEROcmd_flip_dir,
    HEROcmd_remap_datum,
    HEROcmd_xflip,
    HEROcmd_yflip,
    HEROcmd_zflip,
    HEROcmd_no_flip,
    HEROcmd_semaphore,
    HEROcmd_rumble,
    HEROcmd_kill_hero,
    HEROcmd_mspeed,
    HEROcmd_set_jump_ctrl,
    HEROcmd_clr_jump_ctrl
};

enum HEROreqs {
    HEROreq_none,
    HEROreq_error,
    HEROreq_loop,
    HEROreq_abort,
    HEROreq_func,
    HEROreq_idle,
    HEROreq_turn,
    HEROreq_run,
    HEROreq_change,
    HEROreq_change2,
    HEROreq_slide,
    HEROreq_jump,
    HEROreq_stop,
    HEROreq_start,
    HEROreq_remap,
    HEROreq_reverse,
    HEROreq_forward,
    HEROreq_duck,
    HEROreq_test,
    HEROreq_throw,
    HEROreq_throw1,
    HEROreq_throw2,
    HEROreq_throw3
};

struct HEROFunc {
    /* 0x00 */ void (*SetMode)();
    /* 0x04 */ void (*Action)();
};
BASH_SIZE_CHECK(HEROFunc, 0x8);

struct TweakInfo {
    /* 0x00 */ s16 Quality;
    /* 0x02 */ s16 Aggression;
    /* 0x04 */ s16 Target_Bias;
};
BASH_SIZE_CHECK(TweakInfo, 0x6);

struct grid_node {
    /* 0x00 */ u8 got;
    /* 0x01 */ u8 mask;
};
BASH_SIZE_CHECK(grid_node, 0x2);

enum HeroAnim_seqs {
    hero_seq_none,
    hero_seq_breathe,
    hero_seq_taunt_a,
    hero_seq_idle_a,
    hero_seq_idle_b,
    hero_seq_medium,
    hero_seq_jump,
    hero_seq_fall,
    hero_seq_land,
    hero_seq_hit,
    hero_seq_attack,
    hero_seq_dead,
    hero_seq_win,
    hero_seq_victory,
    hero_BA_seq_slide,
    hero_BA_seq_pickup,
    hero_BA_seq_hold,
    hero_BA_seq_hold_slow,
    hero_BA_seq_hold_throw,
    hero_BA_seq_die,
    hero_PA_seq_pickup,
    hero_PA_seq_hold,
    hero_PA_seq_hold_slow,
    hero_PA_seq_hold_throw,
    hero_PA_seq_die,
    hero_seq_arkbreathe,
    hero_seq_arkmove,
    hero_seq_arkidle,
    hero_seq_arktaunt,
    hero_seq_arkkick,
    hero_seq_arkwin,
    hero_seq_arkvictory,
    hero_seq_arklose,
    hero_seq_n2obreathe,
    hero_seq_n2omove,
    hero_seq_n2oidle,
    hero_seq_n2otaunt,
    hero_seq_n2okick,
    hero_seq_n2ohit,
    hero_seq_n2owin,
    hero_seq_n2ovictory,
    hero_seq_n2olose,
    hero_seq_n2ofly,
    hero_seq_long_hit,
    hero_seq_flip_jump,
    hero_SP_seq_die,
    hero_seq_pogo_idle,
    hero_seq_pogo_quick_idle,
    hero_seq_pogo_tazing,
    hero_seq_pogo_dazed,
    hero_seq_pogo_winner,
    hero_seq_bonk_idle,
    hero_seq_bonk_run,
    hero_seq_bonk_backswing,
    hero_seq_bonk_extended_backswing,
    hero_seq_bonk_swing,
    hero_seq_bonk_hit,
    hero_seq_bonk_winner,
    hero_seq_polaridle,
    hero_seq_polarrun,
    hero_seq_polardazed,
    hero_seq_polarfall,
    hero_seq_polarattack,
    hero_seq_polarhit,
    hero_seq_polardead,
    hero_seq_polartaunt,
    hero_seq_breathe_ft,
    hero_seq_skate_ft,
    hero_seq_lightbear,
    hero_seq_skate_attack,
    hero_seq_polarwin,
    hero_seq_win_ft,
    hero_seq_raceidle,
    hero_seq_racerun,
    hero_seq_racehit,
    hero_seq_racefall,
    hero_seq_racewin,
    hero_seq_dinoidle,
    hero_seq_dinorun,
    hero_seq_dinoshoot,
    hero_seq_dinoslide,
    hero_seq_dinodazed,
    hero_seq_raceswimidle,
    hero_seq_raceswimrun,
    hero_seq_raceswimfall,
    hero_seq_raceswimwin,
    hero_seq_dinowin,
    hero_seq_tankrecoil,
    hero_seq_tankdie,
    hero_seq_tankidle,
    hero_seq_tankidle1,
    hero_seq_tankminelay,
    hero_seq_tankvictory,
    hero_seq_tankfire1,
    hero_seq_tankfire2,
    hero_seq_tankfire3,
    hero_seq_kegnone,
    hero_seq_kegbreathe,
    hero_seq_kegidle_a,
    hero_seq_kegidle_b,
    hero_seq_kegmedium,
    hero_seq_kegjump,
    hero_seq_kegfall,
    hero_seq_kegland,
    hero_seq_keghit,
    hero_seq_kegdead,
    hero_seq_kegwin,
    hero_seq_kegvictory,
    HEROseq_max
};

enum HeroMode_nums {
    HEROmode_idle,
    HEROmode_quiet,
    HEROmode_taunt,
    HEROmode_run,
    HEROmode_jump,
    HEROmode_fall,
    HEROmode_drop,
    HEROmode_land,
    HEROmode_spin,
    HEROmode_whip,
    HEROmode_shoot,
    HEROmode_kick,
    HEROmode_BAloser,
    HEROmode_BAhit,
    HEROmode_BApickup,
    HEROmode_BAhold,
    HEROmode_BAcarry,
    HEROmode_BAthrow,
    HEROmode_bigweight,
    HEROmode_BAdead,
    HEROmode_BAwinner,
    HEROmode_PAhit,
    HEROmode_PApickup,
    HEROmode_PAhold,
    HEROmode_PAcarry,
    HEROmode_PAthrow,
    HEROmode_PAdead,
    HEROmode_PAwinner,
    HEROmode_ARIdle,
    HEROmode_ARMove,
    HEROmode_ARTaunt,
    HEROmode_ARKick,
    HEROmode_ARRedKick,
    HEROmode_ARGrab,
    HEROmode_ARWinner,
    HEROmode_ARLose,
    HEROmode_ARDie,
    HEROmode_ARDead,
    HEROmode_NOEnter,
    HEROmode_NOIdle,
    HEROmode_NOMove,
    HEROmode_NOTaunt,
    HEROmode_NOHit,
    HEROmode_NOKick,
    HEROmode_NOWinner,
    HEROmode_NODie,
    HEROmode_NODead,
    HEROmode_NOFly,
    HEROmode_N2ODead,
    HEROmode_N2OHit,
    HEROmode_N2OWinner,
    HEROmode_SPhit,
    HEROmode_SPflip,
    HEROmode_SPfall,
    HEROmode_SPWinner,
    HEROmode_SPLose,
    HEROmode_SPDead,
    HEROmode_pogo_idle,
    HEROmode_pogo,
    HEROmode_pogo_dazed,
    HEROmode_pogo_winner,
    HEROmode_pogo_loser,
    HEROmode_pogo_tied,
    HEROmode_pogo_tazing,
    HEROmode_bonk_idle,
    HEROmode_bonk_run,
    HEROmode_bonk_hit,
    HEROmode_bonk_backswing,
    HEROmode_bonk_ext_backswing,
    HEROmode_bonk_swing,
    HEROmode_bonk_swingrecoil,
    HEROmode_bonk_winner,
    HEROmode_bonk_loser,
    HEROmode_polaridle,
    HEROmode_polarrun,
    HEROmode_polardazed,
    HEROmode_polartransmute,
    HEROmode_polarweight,
    HEROmode_polaroffice,
    HEROmode_polarinbeam,
    HEROmode_polarpoll,
    HEROmode_polarattack,
    HEROmode_polarsnowball,
    HEROmode_polarthrowbomb,
    HEROmode_polarfootidle,
    HEROmode_polarfootrun,
    HEROmode_polarhit,
    HEROmode_polardead,
    HEROmode_polarwinner,
    HEROmode_polarsquashed,
    HEROmode_polarchallenge,
    HEROmode_raceidle,
    HEROmode_racerun,
    HEROmode_racedazed,
    HEROmode_racejump,
    HEROmode_racekick,
    HEROmode_raceoiled,
    HEROmode_raceofftrack,
    HEROmode_racewinner,
    HEROmode_dinoidle,
    HEROmode_dinorun,
    HEROmode_dinoshoot,
    HEROmode_dinoslide,
    HEROmode_dinodazed,
    HEROmode_dinowinner,
    HEROmode_TankRecoil,
    HEROmode_TankDie,
    HEROmode_TankDead,
    HEROmode_TankIdle,
    HEROmode_TankMinelay,
    HEROmode_TankMove,
    HEROmode_TankTurn,
    HEROmode_TankVictory,
    HEROmode_ChaseIdle,
    HEROmode_ChaseRun,
    HEROmode_ChaseHit,
    HEROmode_ChaseLose,
    HEROmode_ChaseWinner,
    HEROmode_ChaseDie,
    HEROmode_ChaseDead,
    HEROmode_ChaseJump,
    HEROmode_ChaseDrop,
    HEROmode_ChaseFall,
    HEROmode_ChaseLand,
    HEROmode_max
};

#endif /* BASH_HERO_H */
