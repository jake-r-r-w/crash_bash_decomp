/* The game's save slots, in memory and as files (platform/saves.c). The save screens read CardStatus, which is always ready here,
 * as there's no card to wait for, take out or format. */
#include "common.h"
#include "bash/main.h"
#include "bash/card.h"
#include "bash/engine.h"
#include "bash/gameflow.h"
#include "../../platform/saves.h"

CardSlot CardSlots[4];
s32 CardSaveSum = 0;
s32 CardStatus = CARD_READY;

static s32 slots_sum(void) {
    u32 *p = (u32 *)CardSlots;
    u32 *end = (u32 *)(CardSlots + 4);
    u32 sum = 0;

    while (p < end) {
        sum += *p++;
    }
    return (s32)sum;
}

void card_init(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (!saves_read(i, &CardSlots[i], sizeof(CardSlot))) {
            card_clear_slot(&CardSlots[i]);
        }
    }
    CardSaveSum = slots_sum();
    CardStatus = CARD_READY;
}

void card_save(void) {
    s32 i;

    CardStatus = CARD_READY;
    for (i = 0; i < 4; i++) {
        if (*(s32 *)CardSlots[i].summary != 0 && !saves_write(i, &CardSlots[i], sizeof(CardSlot))) {
            CardStatus = CARD_READY | CARD_FAILED;
        }
    }
    CardSaveSum = slots_sum();
}

void card_reset(void) {
    CardStatus = CARD_READY;
}

/* PC: whether a slot's bytes are a save the game could have made, and so one it can load: for a save from another machine (an online
 * game's host's: platform/net.c), which the game would otherwise take as it is. Loading a slot copies it over the game's own state
 * (*GAME = state, gameflow.c), and the card screen draws every slot's name and heroes, so any number in it that the game looks
 * something up by could have it read or write outside a table. Each such number is checked against the tables it's used for (an audit
 * of every field's uses, 2026-11-10; docs/PORT.md, Online play, Hardening). What the game only adds up, compares, tests the bits of or
 * sets again as it loads isn't checked. A save is only ever made in a warp room's save menu, which settles the mode, and the mode's
 * warp room settles which gateways the hero can stand at. */
int card_check(const void *data, int size, char *why, size_t room) {
    const CardSlot *slot = data;
    const GameState *g = &slot->state;
    const HubTableEntry *e;
    const char *wrong = NULL;
    s32 i;

    if (size == 0) {
        return 1;
    }
    if (size != (int)sizeof(CardSlot)) {
        wrong = "its size";
    } else if (*(const s32 *)slot->summary == 0) {
        wrong = "it's marked empty"; /* an empty slot isn't saved, but sent as none */
    } else if (g->unk20 != 0) {
        wrong = "its name doesn't end"; /* the byte after the name ends it when all 8 are used: it's drawn as text */
    } else if (g->language != 0) {
        wrong = "its language"; /* this is the USA version, whose tables of text have one language */
    } else if (g->mode < 0x21 || g->mode > 0x27) {
        wrong = "its game mode isn't a warp room";
    } else if (g->fps != 50 && g->fps != 60) {
        wrong = "its frame rate";
    } else if (g->players < 1 || g->players > 2) {
        wrong = "how many players"; /* an Adventure's one or two */
    } else if (g->cup > 6 || g->cupRound > 3) {
        wrong = "its cup"; /* CupTable[7], and four rounds a cup */
    } else if (g->arena < 0 || g->arena > 6 || g->arenaLevel < 0 || g->arenaLevel > 3) {
        wrong = "its battle arena"; /* BattleArenas[7], four levels each */
    } else if (g->aiLevel < 0 || g->aiLevel > 2) {
        wrong = "its difficulty"; /* the skill tables' three, and three more after */
    } else if (g->cupLeader < -1 || g->cupLeader > 3) {
        wrong = "its cup's leader";
    } else if (g->teamLeader[0] < 0 || g->teamLeader[0] > 3 || g->teamLeader[1] < 0 || g->teamLeader[1] > 3) {
        wrong = "its teams"; /* Heroes[4] */
    } else if (g->teams != (g->players == 2)) {
        wrong = "its teams"; /* two players play an Adventure as a team, and one doesn't (PlayersMenuUpdate) */
    } else if (g->winner[0] > 15) {
        wrong = "its winner"; /* HeroSide[16] */
    } else if (g->warpRoom > 9) {
        wrong = "its warp room"; /* WarpRooms[10] */
    } else if (g->returnToHub != 0 && g->gateway == -1) {
        wrong = "where its hero comes back to"; /* coming back from a game, the hero's put at its gateway (InitialiseHUB) */
    } else if (g->adventure.stage < 0 || g->adventure.stage > 2) {
        wrong = "how far its Adventure is"; /* the doors a warp room can have, HubDoorAngles[5] */
    } else if (g->adventure.options.music < 0 || g->adventure.options.music > 100 || g->adventure.options.sfx < 0 || g->adventure.options.sfx > 100) {
        wrong = "its volumes";
    }
    for (i = 0; wrong == NULL && i < 8; i++) {
        if (g->name[i] != 0 && (g->name[i] < 0x20 || g->name[i] > 0x7E)) {
            wrong = "its name"; /* the font's letters and marks: a 13 is a line break that reads on past the end */
        }
    }
    for (i = 0; wrong == NULL && i < 4; i++) {
        if (g->heroChoice[i] < 1 || g->heroChoice[i] > 8) {
            wrong = "its heroes"; /* IconImage[10], and the tables that count heroes from 1 */
        } else if (g->heroes[i] < 1 || g->heroes[i] > 9) {
            wrong = "its heroes in play"; /* 9 is none */
        } else if (g->teammateChoice[i] < -1 || g->teammateChoice[i] > 3 || g->teammates[i] < -1 || g->teammates[i] > 3) {
            wrong = "its teammates";
        } else if (g->teams != 0 && g->teammateChoice[i] < 0) {
            wrong = "its teammates"; /* in teams, each hero's teammate is looked up as a hero: the choice, which each level's end copies */
        } else if (g->adventure.rank[i] > 3) {
            wrong = "its players' standings";
        }
        /* not aiSkill: a warp room's sets it from the byte past modeSkill's end (SetTweakAi reads modeSkill[mode]), which in the fifth
           is the music's volume, and the game sets it again before it uses it */
    }
    /* each game's skill: up to 3, or 31 in a boss fight (the game's own limits, pause.c and panel.c) */
    for (i = 0; wrong == NULL && i < 0x21; i++) {
        if (g->adventure.modeSkill[i] > (i == 4 || i == 9 || i == 22 || i == 31 ? 31 : 3)) {
            wrong = "a game's skill";
        }
    }
    /* none, or one the warp room has (HubInitGateways gives only those a table to look in) */
    if (wrong == NULL && g->gateway != -1) {
        wrong = "where its hero stands";
        for (e = WarpRooms[WarpRoomIndex(g->mode)].table; g->gateway >= 0 && g->gateway < 12 && e != NULL && e->gateway != 0; e++) {
            if (e->gateway == g->gateway + 1) {
                wrong = NULL;
                break;
            }
        }
    }
    if (wrong != NULL) {
        snprintf(why, room, "%s", wrong);
        return 0;
    }
    return 1;
}

void card_clear_slot(CardSlot *slot) {
    s32 i;

    for (i = 0; i < 8; i++) {
        slot->summary[i] = 0;
    }
}
