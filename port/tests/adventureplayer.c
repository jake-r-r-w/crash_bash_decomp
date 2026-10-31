/* A test run's player for a whole Adventure (--adventure). In a warp room it walks to the next door the game's own rules open and
 * goes through it, as a player does. In a game the computer plays for it, and hands back as the win comes, as a win only counts
 * for a player; a boss is bossbot.c's. The log says how far it's got each time it's back. */
#include <stdio.h>
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/hub.h"
#include "bash/hub_ovl.h"
#include "bash/aiskill.h"

void platform_bot_buttons(int buttons);
void platform_finish(int code);
int platform_vblanks(void);
void BossBotUpdate(void);

#define BTN_UP 0x10
#define BTN_RIGHT 0x20
#define BTN_DOWN 0x40
#define BTN_LEFT 0x80
#define BTN_CROSS 0x4000

#define GATEWAYS 12
#define ROOMS 5
#define ROOM_FIRST 0x21
#define ROOM_LAST 0x25
#define GATE_LEVEL 2
#define GATE_WARP 5
#define AWARD_TROPHY 0
#define AWARD_BOSS 4
#define NEAR_ANGLE 0x40
#define MAX_SKILL 0x20

static s32 Ticks;
static s32 InRoom;

static s32 in_room(void) {
    return GameMode >= ROOM_FIRST && GameMode <= ROOM_LAST;
}

static s32 level_to_do(s32 level) {
    return ModeRequirementMet(level) == 1 && GAME->adventure.won[AWARD_TROPHY][level] == 0 && GAME->adventure.won[AWARD_BOSS][level] == 0;
}

/* The door in this room to go to next, and the entry on the warp door's list if it's another room's level. -1 if none's left. */
static s32 next_door(s32 *entry) {
    s32 i, r;

    for (i = 0; i < GATEWAYS; i++) {
        HubTableEntry *t = HubGateways[i].table;

        if (t != NULL && t->gate_type == GATE_LEVEL && level_to_do(t->level)) {
            return i;
        }
    }
    for (r = 0; r < ROOMS; r++) {
        HubTableEntry *t;

        for (t = WarpRooms[r].table; t->gateway != 0; t++) {
            if (t->gate_type == GATE_LEVEL && level_to_do(t->level)) {
                hub_info *room = &WarpRooms[GameWarpRoom];

                for (i = 0; i < room->num_warps; i++) {
                    if (room->warp[i].levnum == WarpRooms[r].levnum) {
                        *entry = i;
                        for (i = 0; i < GATEWAYS; i++) {
                            if (HubGateways[i].table != NULL && HubGateways[i].table->gate_type == GATE_WARP) {
                                return i;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

static s32 play_room(void) {
    s32 entry = -1;
    s32 door;
    s32 at = GameGateway;
    HubGateway *g;
    s32 i;

    /* still loading: the room's doors aren't set up yet */
    for (i = 0; i < GATEWAYS && HubGateways[i].table == NULL; i++) {
    }
    if (i == GATEWAYS || (GAME->exitFlags & 0x8008) || Heroes[0].hero_frozen != 0) {
        return 0;
    }
    door = next_door(&entry);
    if (door < 0) {
        printf("adventure: no door left to go through\n");
        platform_finish(0);
    }
    g = &HubGateways[door];
    if (at != -1 && at != door) {
        return BTN_DOWN;
    }
    if (at == -1) {
        s32 turn = ((g->angle - HubHeroRing.angle + 0x800) & 0xFFF) - 0x800;

        if (abs(turn) > NEAR_ANGLE) {
            return turn > 0 ? BTN_RIGHT : BTN_LEFT;
        }
        return BTN_UP;
    }
    if (!g->on_pad) {
        return BTN_UP;
    }
    if (entry >= 0 && HubSelectedEntry != entry) {
        if ((Ticks & 7) != 0) {
            return 0;
        }
        return HubSelectedEntry < entry ? BTN_RIGHT : BTN_LEFT;
    }
    return (Ticks & 15) == 0 ? BTN_CROSS : 0;
}

static void log_progress(void) {
    s32 count[5] = { 0 };
    s32 a, i;

    for (a = 0; a < 5; a++) {
        for (i = 0; i < 0x21; i++) {
            count[a] += GAME->adventure.won[a][i] != 0;
        }
    }
    printf("adventure: vblank %d room %d trophies %d relics %d crystals %d gems %d bosses %d stage %d\n", platform_vblanks(), GameMode, count[0],
           count[1], count[2], count[3], count[4], GAME->adventure.stage);
}

/* Each round's result, as a round's the thing that takes the time. */
static void log_round(void) {
    static s32 last;
    s32 now = GAME->exitFlags & 1;

    if (now && !last) {
        printf("round: vblank %d mode %d winner %d points %d %d %d %d\n", platform_vblanks(), GameMode, GAME->winner[0], GAME->adventure.roundPoints[0],
               GAME->adventure.roundPoints[1], GAME->adventure.roundPoints[2], GAME->adventure.roundPoints[3]);
    }
    last = now;
}

void AdventurePlayerUpdate(void) {
    Ticks++;
    if (in_room()) {
        if (!InRoom) {
            InRoom = 1;
            log_progress();
        }
        GAME->heroCpu[0] = 0;
        platform_bot_buttons(play_room());
        return;
    }
    InRoom = 0;
    log_round();
    if (GAME->levelFlags & 0x100) {
        BossBotUpdate();
        return;
    }
    /* a game: the computer has the pad (heroes.c). It, the titles, a story or a menu all wait for a button at the end */
    GAME->heroCpu[0] = 0;
    if (GameMode < ROOM_FIRST) {
        AiPlayerSkill[0].level = MAX_SKILL;
    }
    platform_bot_buttons((Ticks & 31) < 3 ? BTN_CROSS : 0);
}
