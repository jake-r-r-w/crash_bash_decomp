#ifndef BASH_TITLES_H
#define BASH_TITLES_H

/* The title screens and front-end menus (overlay titles). Its AST entries
 * (TITLES_AST_types) are stubs in retail.
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct Char_Selector Char_Selector;
typedef struct Titles_Texture Titles_Texture;
typedef struct TitlesMenuOpt TitlesMenuOpt;

enum TITLES_AST_types {
    TITLES_AST_zero = 0x1000,
    AST_TitlesOdo,
    AST_MiniTitlesOdo
};

enum {
    TITLES_INIT_MAIN_MENU,
    TITLES_HANDLE_MENU,
    TITLES_INIT_PLAYTYPE_MENU,
    TITLES_INIT_CHARACTER_SELECTION,
    TITLES_PLAYER_PICKS_CHARACTER,
    TITLES_CPU_PICKS_CHARACTER,
    TITLES_CPU_FINISHED_PICKING,
    TITLES_SELECTION_CANCELLED,
    TITLES_WAITFORDOODAHTODROP,
    TITLES_WAITFORDOODAHTORISE,
    TITLES_PAUSEAFTERCPUPICK,
    TITLES_PICKINGALLDONE
};

enum {
    OPT_NONE,
    OPT_ONEPLAYER,
    OPT_TWOPLAYERS,
    OPT_FOURPLAYERS,
    OPT_TEAMPLAY,
    OPT_FREEFORALL,
    OPT_CANCEL
};

struct Char_Selector {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 frame;
    /* 0x08 */ s32 dbase;
    /* 0x0C */ ANIMtab *animation;
};
BASH_SIZE_CHECK(Char_Selector, 0x10);

struct Titles_Texture {
    /* 0x00 */ s32 frame;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ u32 col;
};
BASH_SIZE_CHECK(Titles_Texture, 0x10);

struct TitlesMenuOpt {
    /* 0x00 */ s32 xpos;
    /* 0x04 */ s32 ypos;
    /* 0x08 */ s8 text[50];
    /* 0x3C */ u32 off_hi;
    /* 0x40 */ u32 off_lo;
    /* 0x44 */ u32 on_hi;
    /* 0x48 */ u32 on_lo;
    /* 0x4C */ s32 return_code;
};
BASH_SIZE_CHECK(TitlesMenuOpt, 0x50);

#endif /* BASH_TITLES_H */
