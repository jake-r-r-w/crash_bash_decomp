#ifndef BASH_SHELL_H
#define BASH_SHELL_H

/* The game shell: pause and panel text, movies, awards, the cheat menu, camera
 * modes and the panel/system map and texture ids.
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/core.h"

typedef struct Pause_text_info Pause_text_info;
typedef struct process_seq_step process_seq_step;
typedef struct level_list_info level_list_info;
typedef struct arena_info arena_info;
typedef struct movie_script_info movie_script_info;
typedef struct movie_sound_info movie_sound_info;
typedef struct movie_info movie_info;
typedef struct award_vars_info award_vars_info;
typedef struct Ambient_data Ambient_data;
typedef struct PanelCup PanelCup;
typedef struct ClockInfo ClockInfo;
typedef struct ScoreInfo ScoreInfo;
typedef struct cheat_menu_struct cheat_menu_struct;
typedef struct widget_pos widget_pos;

enum PauseTextId {
    SYS_CONTINUE_TEXT,
    SYS_RULES_TEXT,
    SYS_OPTIONS_TEXT,
    SYS_CHANGE_TEXT,
    SYS_CHANGEQ_TEXT,
    SYS_QUIT_TEXT,
    SYS_EXIT_TEXT,
    SYS_EXITQ_TEXT,
    SYS_RETURN_TEXT,
    SYS_RETURNQ_TEXT,
    SYS_NEW_TEXT,
    SYS_REPLAY_TEXT,
    SYS_CHEAT_TEXT,
    SYS_LEVEL_TEXT,
    SYS_QUERY_TEXT,
    SYS_YESNO_TEXT,
    SYS_YES_TEXT,
    SYS_NO_TEXT,
    SYS_VIB_TEXT,
    SYS_ON_TEXT,
    SYS_OFF_TEXT,
    SYS_FLAGS_TEXT,
    SYS_TWEAK_TEXT,
    SYS_ITEMS_TEXT,
    SYS_STATS_TEXT,
    SYS_ADJUST_TEXT,
    SYS_PAD1_TEXT,
    SYS_PAD2_TEXT,
    SYS_PAD1A_TEXT,
    SYS_PAD1B_TEXT,
    SYS_PAD1C_TEXT,
    SYS_PAD1D_TEXT,
    SYS_PAD_TEXT,
    SYS_DISC_TEXT,
    SYS_NODISC_TEXT,
    SYS_DEMO_TEXT,
    SYS_X1_TEXT,
    SYS_X2_TEXT,
    SYS_X3_TEXT,
    SYS_MUSIC_TEXT,
    SYS_MUSIC_OPTIONS_TEXT,
    SYS_MODE_TEXT,
    SYS_VOLUME_TEXT,
    SYS_SFX_TEXT,
    SYS_CONTROLLER_TEXT,
    SYS_STEREO_TEXT,
    SYS_MONO_TEXT,
    SYS_100A_TEXT,
    SYS_100B_TEXT,
    SYS_EXIT2_TEXT,
    SYS_ACCEPT_TEXT,
    SYS_TOGGLE_TEXT,
    SYS_P1_TEXT,
    SYS_P2_TEXT,
    SYS_P3_TEXT,
    SYS_P4_TEXT
};

struct Pause_text_info {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x08 */ s32 txt;
    /* 0x0C */ s32 hi;
    /* 0x10 */ s32 lo;
    /* 0x14 */ s32 hi2;
    /* 0x18 */ s32 lo2;
};
BASH_SIZE_CHECK(Pause_text_info, 0x1C);

struct process_seq_step {
    /* 0x00 */ void *process;
};
BASH_SIZE_CHECK(process_seq_step, 0x4);

struct level_list_info {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 levnum;
    /* 0x08 */ obj_dbase *db;
    /* 0x0C */ s32 framenum;
};
BASH_SIZE_CHECK(level_list_info, 0x10);

struct arena_info {
    /* 0x00 */ level_list_info *level;
    /* 0x04 */ s32 num_levels;
};
BASH_SIZE_CHECK(arena_info, 0x8);

enum {
    PPM_INIT,
    PPM_WARP,
    PPM_WARPING,
    PPM_IDLING,
    PPM_MOREIDLING,
    PPM_DYING,
    PPM_WARPOUT,
    PPM_WARPINGOUT
};

enum {
    FLOATER_STARTDELAY,
    FLOATER_GOUP,
    FLOATER_BOB,
    FLOATER_TOPANEL
};

enum Cam_Modes {
    Cam_mode_nocam,
    Cam_mode_framed,
    Cam_mode_flipped_framed,
    Cam_mode_titles,
    Cam_mode_podium,
    Cam_mode_viewer,
    Cam_mode_hub,
    Cam_mode_static,
    Cam_mode_static2,
    Cam_Mode_MAX
};

struct movie_script_info {
    /* 0x00 */ s32 type;
    /* 0x04 */ obj_dbase *dbase;
    /* 0x08 */ s32 script_num;
    /* 0x0C */ s32 clear_flag;
};
BASH_SIZE_CHECK(movie_script_info, 0x10);

struct movie_sound_info {
    /* 0x00 */ void *sfx_file;
    /* 0x04 */ s32 fxnum;
    /* 0x08 */ s32 time;
};
BASH_SIZE_CHECK(movie_sound_info, 0xC);

struct movie_info {
    /* 0x00 */ movie_script_info *scripts;
    /* 0x04 */ movie_sound_info *sounds;
    /* 0x08 */ s32 speed_adjust;
};
BASH_SIZE_CHECK(movie_info, 0xC);

enum {
    TXTBOX_DEFAULT,
    TXTBOX_DEFAULT_TITLES,
    TXTBOX_SOLID,
    TXTBOX_CHARSEL_SELECTED,
    TXTBOX_ARENASEL_MAP,
    TXTBOX_HILIGHT_MEMCARD,
    TXTBOX_OUTLINE_MEMCARD,
    TXTBOX_NICE_BAND,
    TXTBOX_EVIL_BAND,
    TXTBOX_NICE_CHARSEL,
    TXTBOX_EVIL_CHARSEL,
    num_box_styles
};

struct award_vars_info {
    /* 0x00 */ s32 on;
    /* 0x04 */ s32 step;
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 item;
    /* 0x10 */ s32 background;
};
BASH_SIZE_CHECK(award_vars_info, 0x14);

struct Ambient_data {
    /* 0x00 */ u16 min_delay;
    /* 0x02 */ u16 max_delay;
    /* 0x04 */ u16 Background1;
    /* 0x06 */ u16 Background2;
    /* 0x08 */ u16 Random[8];
};
BASH_SIZE_CHECK(Ambient_data, 0x18);

enum panel_items {
    panel_icon0,
    panel_icon1,
    panel_icon2,
    panel_icon3,
    panel_countdown,
    pause_gem_left,
    pause_gem_right,
    TOT_PANEL_ITEMS
};

enum IconBits {
    IconBase,
    IconSplat1,
    IconWumpa,
    IconPowerBarPart0,
    IconPowerBarPart1,
    IconWeaponStatus1,
    IconWeaponStatus2,
    NUM_ICONPARTS
};

struct PanelCup {
    /* 0x00 */ subengine *sub;
    /* 0x04 */ u16 mode;
    /* 0x08 */ s32 timer;
    /* 0x0C */ u32 ticker;
    /* 0x10 */ s32 yoffset;
    /* 0x14 */ s32 surface;
    /* 0x18 */ u32 flags;
    /* 0x1C */ s32 yvelocity;
};
BASH_SIZE_CHECK(PanelCup, 0x20);

struct ClockInfo {
    /* 0x00 */ u16 frame;
    /* 0x02 */ s16 Xoffs;
    /* 0x04 */ u32 Ctrl;
};
BASH_SIZE_CHECK(ClockInfo, 0x8);

struct ScoreInfo {
    /* 0x00 */ u16 score;
    /* 0x02 */ s16 Xoffs;
    /* 0x04 */ s16 Yoffs;
    /* 0x06 */ u16 Ctrl;
};
BASH_SIZE_CHECK(ScoreInfo, 0x8);

struct cheat_menu_struct {
    /* 0x00 */ s8 *text;
    /* 0x04 */ s32 *var;
    /* 0x08 */ s32 type;
    /* 0x0C */ s32 min;
    /* 0x10 */ s32 max;
};
BASH_SIZE_CHECK(cheat_menu_struct, 0x14);

enum gd {
    GMode_battle,
    GMode_tournament,
    GMode_trophy,
    GMode_relic,
    GMode_plat_relic,
    GMode_gem,
    GMode_crystal,
    GMode_boss
};

struct widget_pos {
    /* 0x00 */ s16 left_x_r;
    /* 0x02 */ s16 left_y_r;
    /* 0x04 */ s16 right_x_r;
    /* 0x06 */ s16 right_y_r;
    /* 0x08 */ s16 left_x_p;
    /* 0x0A */ s16 left_y_p;
    /* 0x0C */ s16 right_x_p;
    /* 0x0E */ s16 right_y_p;
};
BASH_SIZE_CHECK(widget_pos, 0x10);

enum PAN_CHARMAPS {
    PAN_CHARMAP_NUM
};

enum {
    GEOTX_PAN_TROPHY_SHINE = 0x3000,
    GEOTX_PAN_TROPHY_GLOW,
    GEOTX_PAN_PI_COCO,
    GEOTX_PAN_PI_CRASH,
    GEOTX_PAN_PI_CORTEX,
    GEOTX_PAN_PI_DINGO,
    GEOTX_PAN_PI_BRIO,
    GEOTX_PAN_PI_TINY,
    GEOTX_PAN_PI_KOALA,
    GEOTX_PAN_TIM_FONT,
    GEOTX_PAN_TIM_FONT_1,
    GEOTX_PAN_TIM_FONT_2,
    GEOTX_PAN_TIM_FONT_3,
    GEOTX_PAN_TIM_FONT_4,
    GEOTX_PAN_TIM_FONT_5,
    GEOTX_PAN_TIM_FONT_6,
    GEOTX_PAN_TIM_FONT_7,
    GEOTX_PAN_TIM_FONT_8,
    GEOTX_PAN_TIM_FONT_9,
    GEOTX_PAN_TIM_FONT_10,
    GEOTX_PAN_TIM_FONT_11,
    GEOTX_PAN_X,
    GEOTX_PAN_PI_KANGA,
    GEOTX_PAN_TIMER01,
    GEOTX_PAN_TIMER01_1,
    GEOTX_PAN_TIMER01_2,
    GEOTX_PAN_TIMER01_3,
    GEOTX_PAN_TIMER01_4,
    GEOTX_PAN_TIMER01_5,
    GEOTX_PAN_TIMER01_6,
    GEOTX_PAN_TIMER01_7,
    GEOTX_PAN_TIMER01_8,
    GEOTX_PAN_TIMER01_9,
    GEOTX_PAN_BOOST,
    GEOTX_PAN_P4,
    GEOTX_PAN_P2,
    GEOTX_PAN_P3,
    GEOTX_PAN_P1,
    GEOTX_PAN_FRAME,
    GEOTX_PAN_PAUSED01,
    GEOTX_PAN_PAUSED02,
    GEOTX_PAN_PAUSED03,
    GEOTX_PAN_RULES01,
    GEOTX_PAN_RULES02,
    GEOTX_PAN_RULES03,
    GEOTX_PAN_FlatCols,
    GEOTX_PAN_GEO_TEXTURES_NUM
};

enum {
    GEOEN_PAN_CUP = 0x2001,
    GEOEN_PAN_COUNT_ONE,
    GEOEN_PAN_COUNT_TWO,
    GEOEN_PAN_COUNT_THREE,
    GEOEN_PAN_COUNT_GO,
    GEOEN_PAN_TIE,
    GEOEN_PAN_L2_COCO,
    GEOEN_PAN_L2_CRASH,
    GEOEN_PAN_L2_CORTEX,
    GEOEN_PAN_L2_DINGODILE,
    GEOEN_PAN_L2_NBRIO,
    GEOEN_PAN_L2_TINY,
    GEOEN_PAN_L2_KONG,
    GEOEN_PAN_CUP_NUMBERS,
    GEOEN_PAN_CUP_NUMBERS_1,
    GEOEN_PAN_CUP_NUMBERS_2,
    GEOEN_PAN_CUP_NUMBERS_3,
    GEOEN_PAN_CUP_NUMBERS_4,
    GEOEN_PAN_CUP_NUMBERS_5,
    GEOEN_PAN_CUP_NUMBERS_6,
    GEOEN_PAN_CUP_NUMBERS_7,
    GEOEN_PAN_CUP_NUMBERS_8,
    GEOEN_PAN_CUP_NUMBERS_9,
    GEOEN_PAN_CUP_NUMBERS_10,
    GEOEN_PAN_CUP_NUMBERS_11,
    GEOEN_PAN_CUP_X,
    GEOEN_PAN_FAIL,
    GEOEN_PAN_BAR,
    GEOEN_PAN_L2_KANGA,
    GEOEN_PAN_CRYSTAL,
    GEOEN_PAN_GEM,
    GEOEN_PAN_RELIC,
    GEOEN_PAN_RELIC_GOLD,
    GEOEN_PAN_TROPHY,
    GEOEN_PAN_AKUGLOW,
    GEOEN_PAN_GEO_ENTITIES_NUM
};

enum {
    EBS_PAN_RESIDENT,
    EBS_PAN_0_NUM
};

enum {
    SCRIPT_PAN_CRYSTAL,
    SCRIPT_PAN_GEM,
    SCRIPT_PAN_RELIC,
    SCRIPT_PAN_RELIC_GOLD,
    SCRIPT_PAN_TROPHY,
    SCRIPT_PAN_GLOW,
    SCRIPT_PAN_0_NUM
};

enum PAN_MAPS {
    GEOMP_PAN_MAPSNUM
};

enum PAN_MAPPATHS {
    GEOPA_PAN_PATHSNUM
};

enum SYS_CHARMAPS {
    SYS_CHARMAP_NUM
};

enum {
    GEOTX_SYS_SMLFONT = 0x3000,
    GEOTX_SYS_SMLFONT_1,
    GEOTX_SYS_SMLFONT_2,
    GEOTX_SYS_SMLFONT_3,
    GEOTX_SYS_SMLFONT_4,
    GEOTX_SYS_SMLFONT_5,
    GEOTX_SYS_SMLFONT_6,
    GEOTX_SYS_SMLFONT_7,
    GEOTX_SYS_SMLFONT_8,
    GEOTX_SYS_SMLFONT_9,
    GEOTX_SYS_SMLFONT_10,
    GEOTX_SYS_SMLFONT_11,
    GEOTX_SYS_SMLFONT_12,
    GEOTX_SYS_SMLFONT_13,
    GEOTX_SYS_SMLFONT_14,
    GEOTX_SYS_SMLFONT_15,
    GEOTX_SYS_SMLFONT_16,
    GEOTX_SYS_SMLFONT_17,
    GEOTX_SYS_SMLFONT_18,
    GEOTX_SYS_SMLFONT_19,
    GEOTX_SYS_SMLFONT_20,
    GEOTX_SYS_SMLFONT_21,
    GEOTX_SYS_SMLFONT_22,
    GEOTX_SYS_SMLFONT_23,
    GEOTX_SYS_SMLFONT_24,
    GEOTX_SYS_SMLFONT_25,
    GEOTX_SYS_SMLFONT_26,
    GEOTX_SYS_SMLFONT_27,
    GEOTX_SYS_SMLFONT_28,
    GEOTX_SYS_SMLFONT_29,
    GEOTX_SYS_SMLFONT_30,
    GEOTX_SYS_SMLFONT_31,
    GEOTX_SYS_SMLFONT_32,
    GEOTX_SYS_SMLFONT_33,
    GEOTX_SYS_SMLFONT_34,
    GEOTX_SYS_SMLFONT_35,
    GEOTX_SYS_SMLFONT_36,
    GEOTX_SYS_SMLFONT_37,
    GEOTX_SYS_SMLFONT_38,
    GEOTX_SYS_SMLFONT_39,
    GEOTX_SYS_SMLFONT_40,
    GEOTX_SYS_SMLFONT_41,
    GEOTX_SYS_SMLFONT_42,
    GEOTX_SYS_SMLFONT_43,
    GEOTX_SYS_SMLFONT_44,
    GEOTX_SYS_SMLFONT_45,
    GEOTX_SYS_SMLFONT_46,
    GEOTX_SYS_SMLFONT_47,
    GEOTX_SYS_SMLFONT_48,
    GEOTX_SYS_SMLFONT_49,
    GEOTX_SYS_SMLFONT_50,
    GEOTX_SYS_SMLFONT_51,
    GEOTX_SYS_SMLFONT_52,
    GEOTX_SYS_SMLFONT_53,
    GEOTX_SYS_SMLFONT_54,
    GEOTX_SYS_SMLFONT_55,
    GEOTX_SYS_SMLFONT_56,
    GEOTX_SYS_SMLFONT_57,
    GEOTX_SYS_SMLFONT_58,
    GEOTX_SYS_SMLFONT_59,
    GEOTX_SYS_SMLFONT_60,
    GEOTX_SYS_SMLFONT_61,
    GEOTX_SYS_SMLFONT_62,
    GEOTX_SYS_SMLFONT_63,
    GEOTX_SYS_FONT,
    GEOTX_SYS_FONT_1,
    GEOTX_SYS_FONT_2,
    GEOTX_SYS_FONT_3,
    GEOTX_SYS_FONT_4,
    GEOTX_SYS_FONT_5,
    GEOTX_SYS_FONT_6,
    GEOTX_SYS_FONT_7,
    GEOTX_SYS_FONT_8,
    GEOTX_SYS_FONT_9,
    GEOTX_SYS_FONT_10,
    GEOTX_SYS_FONT_11,
    GEOTX_SYS_FONT_12,
    GEOTX_SYS_FONT_13,
    GEOTX_SYS_FONT_14,
    GEOTX_SYS_FONT_15,
    GEOTX_SYS_FONT_16,
    GEOTX_SYS_FONT_17,
    GEOTX_SYS_FONT_18,
    GEOTX_SYS_FONT_19,
    GEOTX_SYS_FONT_20,
    GEOTX_SYS_FONT_21,
    GEOTX_SYS_FONT_22,
    GEOTX_SYS_FONT_23,
    GEOTX_SYS_FONT_24,
    GEOTX_SYS_FONT_25,
    GEOTX_SYS_FONT_26,
    GEOTX_SYS_FONT_27,
    GEOTX_SYS_FONT_28,
    GEOTX_SYS_FONT_29,
    GEOTX_SYS_FONT_30,
    GEOTX_SYS_FONT_31,
    GEOTX_SYS_FONT_32,
    GEOTX_SYS_FONT_33,
    GEOTX_SYS_FONT_34,
    GEOTX_SYS_FONT_35,
    GEOTX_SYS_FONT_36,
    GEOTX_SYS_FONT_37,
    GEOTX_SYS_FONT_38,
    GEOTX_SYS_FONT_39,
    GEOTX_SYS_FONT_40,
    GEOTX_SYS_FONT_41,
    GEOTX_SYS_FONT_42,
    GEOTX_SYS_FONT_43,
    GEOTX_SYS_FONT_44,
    GEOTX_SYS_FONT_45,
    GEOTX_SYS_FONT_46,
    GEOTX_SYS_FONT_47,
    GEOTX_SYS_FONT_48,
    GEOTX_SYS_FONT_49,
    GEOTX_SYS_FONT_50,
    GEOTX_SYS_FONT_51,
    GEOTX_SYS_FONT_52,
    GEOTX_SYS_FONT_53,
    GEOTX_SYS_FONT_54,
    GEOTX_SYS_FONT_55,
    GEOTX_SYS_FONT_56,
    GEOTX_SYS_FONT_57,
    GEOTX_SYS_FONT_58,
    GEOTX_SYS_FONT_59,
    GEOTX_SYS_FONT_60,
    GEOTX_SYS_FONT_61,
    GEOTX_SYS_FONT_62,
    GEOTX_SYS_FONT_63,
    GEOTX_SYS_FONT_64,
    GEOTX_SYS_FONT_65,
    GEOTX_SYS_PAUSED03,
    GEOTX_SYS_PAUSED02,
    GEOTX_SYS_PAUSED01,
    GEOTX_SYS_RULES03,
    GEOTX_SYS_RULES02,
    GEOTX_SYS_RULES01,
    GEOTX_SYS_ARROW_L,
    GEOTX_SYS_ARROW_R,
    GEOTX_SYS_B_CIRCLE,
    GEOTX_SYS_B_DPAD,
    GEOTX_SYS_B_SQUARE,
    GEOTX_SYS_B_TRIANG,
    GEOTX_SYS_B_X,
    GEOTX_SYS_B_L1,
    GEOTX_SYS_B_R1,
    GEOTX_SYS_ARROW_U,
    GEOTX_SYS_ARROW_D,
    GEOTX_SYS_LOADICON,
    GEOTX_SYS_LOADICON_1,
    GEOTX_SYS_GEO_TEXTURES_NUM
};

enum {
    EBS_SYS_RESIDENT,
    EBS_SYS_0_NUM
};

enum SYS_MAPS {
    GEOMP_SYS_MAPSNUM
};

enum SYS_MAPPATHS {
    GEOPA_SYS_PATHSNUM
};

#endif /* BASH_SHELL_H */
