#ifndef BASH_HUB_H
#define BASH_HUB_H

/* The adventure hub (overlay hub).
 * Names and layouts come from the Oct 9 2000 prototype's debug types; see
 * docs/ARCHITECTURE.md (Prototype debug types). Fields whose type the fragment
 * does not define are word (or byte) arrays of the recorded size. */

#include "bash/hero.h"

typedef struct HubHero HubHero;
typedef struct HubTableEntry HubTableEntry;
typedef struct hub_warp_info hub_warp_info;
typedef struct hub_info hub_info;
typedef struct HubGateway HubGateway;
typedef struct PedestalInfo PedestalInfo;

enum HUB_AST_types {
    HUB_AST_zero = 0x1F00,
    AST_PodiumOdo,
    AST_PodiumTrophy,
    AST_GatewayTrophy
};

enum {
    CRED_TITLE,
    CRED_GLOWYTITLE,
    CRED_CTEXT,
    CRED_GAP,
    CRED_STOP
};

enum {
    GATETYPE_NONE,
    GATETYPE_DUMMY,
    GATETYPE_LEVEL,
    GATETYPE_OPTIONS,
    GATETYPE_LOADSAVE,
    GATETYPE_LIFT
};

struct HubHero {
    /* 0x00 */ u16 controller;
    /* 0x04 */ u32 angle;
    /* 0x08 */ u32 radius;
    /* 0x0C */ u32 hub_radius_inner;
    /* 0x10 */ u32 hub_radius_outer;
};
BASH_SIZE_CHECK(HubHero, 0x14);

struct HubTableEntry {
    /* 0x00 */ u16 gateway;
    /* 0x02 */ u16 gate_type;
    /* 0x04 */ u16 level;
    /* 0x06 */ u16 status;
};
BASH_SIZE_CHECK(HubTableEntry, 0x8);

struct hub_warp_info {
    /* 0x00 */ s32 frame;
    /* 0x04 */ s32 levnum;
};
BASH_SIZE_CHECK(hub_warp_info, 0x8);

struct hub_info {
    /* 0x00 */ s32 levnum;
    /* 0x04 */ HubTableEntry *table;
    /* 0x08 */ s32 num_warps;
    /* 0x0C */ hub_warp_info *warp;
    /* 0x10 */ s32 blank_frame;
};

enum {
    hub0,
    hub1,
    hub2,
    hub3,
    hub4,
    hub5,
    hub6,
    hub7,
    hub_demo,
    hub_2lev,
    num_hubs
};

struct HubGateway {
    /* 0x00 */ subengine *spoke;
    /* 0x04 */ subengine *pad;
    /* 0x08 */ u32 radius_inner;
    /* 0x0C */ u32 radius_outer;
    /* 0x10 */ u32 angle;
    /* 0x14 */ HubTableEntry *table;
    /* 0x18 */ subengine *colour_map;
    /* 0x1C */ u32 stood_on;
    /* 0x20 */ u32 on_pad;
    /* 0x24 */ u32 group;
};

enum {
    RELICS_OFF,
    RELICS_GOLD,
    RELICS_PLATINUM
};

struct PedestalInfo {
    /* 0x00 */ SLONG_XY pos;
    /* 0x08 */ u8 pedestal_type;
    /* 0x09 */ u8 anim_type;
};
BASH_SIZE_CHECK(PedestalInfo, 0xC);

enum {
    LETTER_ENTRY,
    LETTER_DONE,
    LETTER_CANCEL,
    LETTER_DEL,
    LETTER_SPACE
};

#endif /* BASH_HUB_H */
