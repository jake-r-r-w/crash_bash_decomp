#ifndef BASH_BONK_H
#define BASH_BONK_H

/* Bonk (overlay bonk).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct BonkCollectable BonkCollectable;
typedef struct BonkHero BonkHero;

enum BONK_AST_types {
    BONK_AST_zero = 0x1A00,
    AST_BonkStool,
    AST_BonkFlag,
    AST_BonkShockwave,
    AST_BonkCrate
};

enum {
    BONKOBJ_SETUP,
    BONKOBJ_DEADFOREVER,
    BONKOBJ_WAITINGTOAPPEAR,
    BONKOBJ_BURROWDOWN,
    BONKOBJ_PUFFOUT,
    BONKOBJ_EXPLODE,
    BONKOBJ_IMDEADNOWWHAT,
    BONKOBJ_FALLDOWN,
    BONKOBJ_FLYAWAY,
    BONKOBJ_CANBESEENBYAI,
    BONKOBJ_BURROWUP,
    BONKOBJ_ACTIVE,
    BONKOBJ_APPEARED,
    BONKOBJ_SMALLVIBES,
    BONKOBJ_BIGVIBES
};

enum {
    BONKPOWERUP_SPEEDYBOOTS,
    BONKPOWERUP_BIGBANG,
    BONKPOWERUP_MAXIMUM,
    BONKPOWERUP_NONE
};

struct BonkCollectable {
    /* 0x00 */ u32 type;
    /* 0x04 */ NAS *nas;
    /* 0x08 */ s32 visible_timer;
};

struct BonkHero {
    /* 0x00 */ BonkCollectable *target;
    /* 0x04 */ s32 frozen;
    /* 0x08 */ s32 powerup;
    /* 0x0C */ s32 powerup_timer;
};

#endif /* BASH_BONK_H */
