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

/* PC: the slots as the game's save on a PS1 memory card, for an emulator (platform/cardexport.c; docs/PORT.md, Saves from a PS1 memory
   card): one 0x2000-byte block as src/main/card.c's card_make_header and card_save write it. The PsyQ header ("SC", two icon frames,
   one block), the title (CRASH BASH and each slot's completion, in Shift-JIS), the icon's colours at 0x60 and frames at 0x80, the four
   slots from 0x200, the flags at 0x850 (1) and the slots' sum at 0x85C, which the game checks as it loads. How many saves it has.
   Or the European game's (europe): each slot 8 bytes longer, two more option words at the state's 0xE0 (left 0, as that game keeps
   them) and its frame rate 50, so the flags at 0x870, which it wants at 2, and the sum at 0x87C (platform/cardimport.c says how that
   was found). */
int save_icon_frames(unsigned char frames[0x100], unsigned short colours[16]);

static u16 sjis(u8 c) {
    static const char marks[] = "!\"#$%()*+,-./:;?";
    static const u16 codes[] = { 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8169, 0x816A, 0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E,
                                 0x8146, 0x8147, 0x8148 };
    const char *m;

    if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
        return (u16)(0x8200 | (c + 0x1F));
    }
    if (c >= 'a' && c <= 'z') {
        return (u16)(0x8200 | (c + 0x20));
    }
    m = c != 0 ? strchr(marks, c) : NULL;
    return m != NULL ? codes[m - marks] : 0x8140;
}

_Static_assert(sizeof(CardSlot) == 0x194, "a slot as the USA game's card has it");

int card_export_block(unsigned char *out, int europe) {
    char title[] = "CRASH BASH %000 %000 %000 %000";
    u32 sum = 0, *w;
    s32 i, n = 0, at, flags = europe ? 0x870 : 0x850;

    memset(out, 0, 0x2000);
    out[0] = 'S';
    out[1] = 'C';
    out[2] = europe ? 0x11 : 0x12; /* two icon frames shown in turn; the European game's save says one, its frames the same */
    out[3] = 1;    /* one block */
    for (i = 0; i < 4; i++) {
        n += *(s32 *)CardSlots[i].summary != 0;
        itoa_div_end(title + 0xC + i * 5, *(s32 *)CardSlots[i].summary != 0 ? count_completion((u8 *)&CardSlots[i].state) : 0, 100);
    }
    for (i = 0, at = 4; i < 32; i++, at += 2) {
        u16 c = sjis(i < (s32)strlen(title) ? (u8)title[i] : ' ');

        out[at] = (u8)(c >> 8);
        out[at + 1] = (u8)c;
    }
    if (!save_icon_frames(out + 0x80, (unsigned short *)(out + 0x60))) {
        memset(out + 0x60, 0, 0x120);
    }
    if (!europe) {
        memcpy(out + 0x200, CardSlots, sizeof(CardSlot) * 4);
    } else {
        for (i = 0; i < 4; i++) {
            const u8 *from = (const u8 *)&CardSlots[i];
            u8 *to = out + 0x200 + i * 0x19C;

            memcpy(to, from, 8 + 0xE0);
            memcpy(to + 8 + 0xE8, from + 8 + 0xE0, sizeof(CardSlot) - 8 - 0xE0);
            if (*(s32 *)CardSlots[i].summary != 0) {
                to[8 + 0x21] = 50; /* GameState.fps */
            }
        }
    }
    out[flags] = europe ? 2 : 1;
    for (w = (u32 *)(out + 0x200); w < (u32 *)(out + flags); w++) {
        sum += *w;
    }
    memcpy(out + flags + 0xC, &sum, 4);
    return n;
}
