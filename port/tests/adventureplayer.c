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
#include "bash/spin_ovl.h"

void platform_bot_buttons(int buttons);
void platform_finish(int code);
int platform_vblanks(void);
void BossBotUpdate(void);

#define BTN_UP 0x10
#define BTN_RIGHT 0x20
#define BTN_DOWN 0x40
#define BTN_LEFT 0x80
#define BTN_CROSS 0x4000
#define BTN_SQUARE 0x8000

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
#define CHOICES 5
#define GIVE_UP 3
#define RING_DING 27
#define POGO_PADLOCK 13
#define BALLOON_FIRST 8
#define BALLOON_LAST 11
#define BALLOON_BLACK 12
#define SPIN_CRATE 2
#define BLACK_PULL 0x300
#define CRATE_PULL 0x200
#define BALLOON_HIGH (-0x380)
#define FLIP_REACH 0x180
#define BAD_REACH 0x140
#define FLIP_MODE_FIRST 0x34
#define FLIP_MODE_LAST 0x35

static s32 Ticks;
static s32 InRoom;
static u16 Tries[0x21][CHOICES];
static s32 WantLevel, WantChoice;
static s32 Losses;

static s32 in_room(void) {
    return GameMode >= ROOM_FIRST && GameMode <= ROOM_LAST;
}

static s32 is_boss(s32 level) {
    return level == 4 || level == 9 || level == 0x16 || level == 0x1F;
}

/* Whether this door's menu offers this challenge now and it's still to win (HubOpenDoorMenu, HubPreMoveHero: choice 0 the trophy, 1 and 2
   won[2] and won[3], 3 and 4 won[1]'s two levels). A boss's door only has the fight. */
static s32 challenge_open(s32 level, s32 choice) {
    AdventureState *a = &GAME->adventure;
    s32 choices = a->stage >= 2 && a->won[1][level] == 0 ? 4 : a->stage + 3;

    if (ModeRequirementMet(level) != 1) {
        return 0;
    }
    if (is_boss(level)) {
        return choice == 0 && a->won[AWARD_BOSS][level] == 0;
    }
    if (choice == 0) {
        return a->won[AWARD_TROPHY][level] == 0;
    }
    if (a->won[AWARD_TROPHY][level] == 0 || choice >= choices) {
        return 0;
    }
    switch (choice) {
    case 1:
        return a->won[2][level] == 0;
    case 2:
        return a->won[3][level] == 0;
    case 3:
        return a->won[1][level] == 0;
    default:
        return a->won[1][level] == 1;
    }
}

/* Where a room's warp list has that room, if the door's list can get to it: it stops at the first that's still shut. */
static s32 warp_entry(s32 to) {
    hub_info *room = &WarpRooms[GameWarpRoom];
    s32 i;

    if (ModeRequirementMet(to) != 1) {
        return -1;
    }
    for (i = 0; i < room->num_warps; i++) {
        if (room->warp[i].levnum == to) {
            return i;
        }
        if (ModeRequirementMet(room->warp[i].levnum) != 1) {
            return -1;
        }
    }
    return -1;
}

/* The door to go to next: whatever's open and been tried least, so one it can't win doesn't hold up the rest, and here before another room.
   Fills which challenge, and the warp list's entry if it's another room's. -1 if there's nothing left. */
static s32 next_door(s32 *entry, s32 *pick) {
    s32 best = 0x7FFFFFFF, door = -1;
    s32 r, c, i;

    for (r = 0; r < ROOMS; r++) {
        HubTableEntry *t;
        s32 via = r == GameWarpRoom ? -1 : warp_entry(WarpRooms[r].levnum);

        if (r != GameWarpRoom && via < 0) {
            continue;
        }
        for (t = WarpRooms[r].table; t->gateway != 0; t++) {
            if (t->gate_type != GATE_LEVEL) {
                continue;
            }
            for (c = 0; c < CHOICES; c++) {
                if (challenge_open(t->level, c)) {
                    s32 score = Tries[t->level][c] * 2 + (via >= 0);

                    if (score < best) {
                        best = score;
                        *pick = c;
                        *entry = via;
                        WantLevel = t->level;
                        door = via >= 0 ? -2 : t->gateway - 1;
                    }
                }
            }
        }
    }
    if (door == -2) {
        for (i = 0; i < GATEWAYS; i++) {
            if (HubGateways[i].table != NULL && HubGateways[i].table->gate_type == GATE_WARP) {
                return i;
            }
        }
        return -1;
    }
    return door;
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
    door = next_door(&entry, &WantChoice);
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
    /* a door whose trophy's won has a menu of its challenges */
    if (entry < 0 && !is_boss(WantLevel) && GAME->adventure.won[AWARD_TROPHY][WantLevel] != 0) {
        if (HubDoorChoice < 0) {
            return 0;
        }
        if (HubDoorChoice != WantChoice) {
            return (Ticks & 7) != 0 ? 0 : HubDoorChoice < WantChoice ? BTN_RIGHT : BTN_LEFT;
        }
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
        /* a challenge's lost when nobody won it, a trophy when someone else has its three rounds */
        if (GAME->winner[0] == 0 || GAME->adventure.roundPoints[1] >= 3 || GAME->adventure.roundPoints[2] >= 3 || GAME->adventure.roundPoints[3] >= 3) {
            Losses++;
        }
        printf("round: vblank %d mode %d winner %d points %d %d %d %d scores %d %d %d %d\n", platform_vblanks(), GameMode, GAME->winner[0], GAME->adventure.roundPoints[0],
               GAME->adventure.roundPoints[1], GAME->adventure.roundPoints[2], GAME->adventure.roundPoints[3], Heroes[0].hero_score, Heroes[1].hero_score, Heroes[2].hero_score, Heroes[3].hero_score);
    }
    last = now;
}

/* Whether popping this balloon scores for this hero, or knocks him down */
static s32 ring_mine(const EXTobject *f) {
    return SpinBalloonTimer != 0 ? SpinBalloonOwner == 0 : f->contains == 0;
}

/* Ring Ding's player, in place of the computer's (which picks a balloon at random): the nearest balloon that scores, the black one from
   further off, and a flip up into it, unless that would burst someone else's on the way. */
static void ring_play(s32 id) {
    EXTengine *he = Heroes[0].hero_eng;
    EXTbotai *ai = Heroes[0].hero_ai;
    NAS *n, *best = NULL;
    s32 best_d = 0x7FFFFFFF, bad = 0, pad = 0;

    if (id != 0) {
        Spin_HeroBot(id);
        return;
    }
    ai->AImode = 2;
    for (n = HeadOfObjects; n != NULL; n = n->Object.Child) {
        EXTobject *f = (EXTobject *)n->FUX;
        EXTengine *e = n->Engine;
        s32 d;

        if (e == NULL || f == NULL || f->status != 1) {
            continue;
        }
        d = csqrt((e->XYZ.x - he->XYZ.x) * (e->XYZ.x - he->XYZ.x) + (e->XYZ.z - he->XYZ.z) * (e->XYZ.z - he->XYZ.z)) >> 6;
        if (f->whatami >= BALLOON_FIRST && f->whatami <= BALLOON_LAST) {
            if (!ring_mine(f)) {
                bad |= d < BAD_REACH;
                continue;
            }
        } else if (f->whatami == BALLOON_BLACK) {
            d -= BLACK_PULL;
        } else if (f->whatami == SPIN_CRATE) {
            /* solid: from on top of one a hit misses it */
            if (Heroes[0].hero_pu != 0 || he->platform == e) {
                continue;
            }
            d -= CRATE_PULL;
        } else {
            continue;
        }
        /* still dropping in, out of reach */
        if (e->XYZ.y < BALLOON_HIGH) {
            continue;
        }
        if (d < best_d) {
            best_d = d;
            best = n;
        }
    }
    if (best != NULL) {
        EXTobject *f = (EXTobject *)best->FUX;
        s32 x = best->Engine->XYZ.x - he->XYZ.x;
        s32 z = best->Engine->XYZ.z - he->XYZ.z;
        s32 ang = get_angle(x, z);

        ai->AIang = ang;
        pad = DirPadBits[(ang >> 9) & 7];
        if ((csqrt(x * x + z * z) >> 6) < FLIP_REACH && Heroes[0].hero_mode != FLIP_MODE_FIRST && Heroes[0].hero_mode != FLIP_MODE_LAST) {
            if (f->whatami == SPIN_CRATE) {
                pad |= (GameFrameCount & 1) ? BTN_SQUARE : 0;
            } else if (!bad) {
                pad |= BTN_CROSS;
            }
        }
    }
    set_joypad(&Pads[0], pad);
    Pads[0].angle.w = ai->AIang;
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
    if (InRoom && GameMode < 0x21 && !(GAME->exitFlags & 0x8008)) {
        Tries[GameMode][WantChoice]++;
        Losses = 0;
        printf("adventure: try %d of level %d challenge %d\n", Tries[GameMode][WantChoice], GameMode, WantChoice);
        InRoom = 0;
    }
    log_round();
    if (GAME->levelFlags & 0x100) {
        BossBotUpdate();
        return;
    }
    /* a game: the computer has the pad (heroes.c). It, the titles, a story or a menu all wait for a button at the end. Padlock deals
       out what's going by who's a player, and he can't win on the computer's hands against that: he counts as the computer's till the win
       comes, when it has to be a player's to count. Ring Ding has its own player, as the computer's picks its balloons at random. */
    GAME->heroCpu[0] = GameMode == POGO_PADLOCK && (GAME->exitFlags & 1) == 0;
    if (GameMode < ROOM_FIRST && GameMode != RING_DING && GameMode != POGO_PADLOCK) {
        AiPlayerSkill[0].level = MAX_SKILL;
    }
    if (GameMode == RING_DING) {
        HeroHooks.HeroBot = ring_play;
    }
    /* the fail screen: enough goes at this one, back to the warp room for something else */
    if ((GAME->exitFlags & 1) && Losses >= GIVE_UP && GameMode < ROOM_FIRST) {
        platform_bot_buttons((Ticks & 15) != 0 ? 0 : PauseChoice == 1 ? BTN_CROSS : BTN_DOWN);
        return;
    }
    /* in play the computer has the pad, and Cross is a game's jump or flip: the taps are for the screens either side */
    if (GameMode < ROOM_FIRST && GAME->exitFlags == 0 && !(GAME->pause & 0x04000000)) {
        platform_bot_buttons(0);
        return;
    }
    platform_bot_buttons((Ticks & 31) < 3 ? BTN_CROSS : 0);
}
