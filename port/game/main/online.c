/* What the game does differently in an online game (docs/PORT.md, Online play). Every machine runs the same game from the same
 * presses, so whatever a press changes, changes on all of them. Two things here mustn't work that way, and one thing is added.
 *
 * The sound options, in the pause menu and the main menu's Options: one player turning the music down would turn it down for
 * everyone (the user's word, 2026-11-10). Online they're each machine's own (settings.h: online_music and its like, kept in the
 * settings file): shown on that machine, changed only by its own players' presses, and what its sound plays by. The game's own
 * (GameOptions, which a save keeps and the fingerprint covers through the game's state) are left alone for as long as the game's
 * online, so every machine's stay the same. What volume a voice is told to sound at is nothing the game reads back (it reads which
 * voices have ended, which a volume doesn't change), so machines can differ in it. The moves on those pages make their sounds on
 * every machine, whoever's they are, so the same voices are in use everywhere.
 *
 * The video options in the same menus are a machine's own settings already; online, only the machine whose player pressed changes
 * its own (own_press).
 *
 * And each player who's connected carries a small tag, so that it's plain other people are there without anything being in the way
 * (the user's words on a line of text that stayed: "its abit in your face"; and on dots alone, that they didn't say how many players
 * are connected; of three designs drawn for them they chose this one, 2026-11-10). The tag sits over the player's icon on the game's
 * own panel: a dot, green while their connection keeps up, orange when the game has had to wait for it, red when it's stopped
 * answering or gone, and which player they are, or YOU. Where the game has no panel, in its menus and warp rooms, the same tags are
 * in a row at the foot of the screen. Words come only for a few seconds: as the game starts (that it's online, how many players,
 * and which this machine's is), and when a player stops answering, comes back or leaves. All of it is drawn with the game's own
 * text and reads nothing but the network's state: it changes nothing the game goes by.
 */
#include <stdio.h>
#include "../../platform/net.h"
#include "../../platform/settings.h"
#include "common.h"
#include "bash/draw.h"
#include "bash/main.h"
#include "bash/modelbank.h"
#include "bash/nascalls.h"
#include "bash/online.h"
#include "bash/sfx.h"

s32 own_options(void) {
    return net_on();
}

s32 own_press(s32 pad) {
    return !net_on() || net_own_pad(pad) >= 0;
}

s32 heard_music(void) {
    return net_on() ? settings()->online_music : GameOptions.music;
}

s32 heard_sfx(void) {
    return net_on() ? settings()->online_sfx : GameOptions.sfx;
}

s32 heard_stereo(void) {
    return net_on() ? settings()->online_stereo : GameOptions.stereo;
}

void own_sound_change(s32 pad, s32 which, s32 dir) {
    Settings *s = settings();
    int *volume = which == OWN_MUSIC ? &s->online_music : &s->online_sfx;

    if (!net_on() || net_own_pad(pad) < 0) {
        return;
    }
    if (which == OWN_STEREO) {
        s->online_stereo = !s->online_stereo;
        if (s->online_stereo) {
            sound_set_stereo();
        } else {
            sound_set_mono();
        }
    } else if (dir != 0) {
        /* the game's own steps: fives, from none to all */
        *volume += dir > 0 ? 5 : -5;
        *volume = *volume < 0 ? 0 : *volume > 100 ? 100 : *volume;
    } else {
        return;
    }
    settings_save();
    if (s->no_save) {
        printf("own sound: music %d, fx %d, %s\n", s->online_music, s->online_sfx, s->online_stereo ? "stereo" : "mono");
    }
}

void own_sound_start(void) {
    if (net_on() && !settings()->online_stereo) {
        sound_set_mono();
    }
}

/* --- who's playing: a tag on each player, and a few words when there's news --- */

#define NEWS_Y 0x1AC    /* the words, near the foot of the screen, in the units text is placed in (640 across, 480 down) */
#define NEWS_FRAMES 180 /* how long words stay: six seconds */
#define TAG_OVER 20     /* a tag's top is this far over the top of its player's icon, */
#define TAG_ICON_MID 36 /* and its middle this far in from the icon's left: the heroes' pictures are about 72 across */
#define ROW_Y 0x1CA     /* the tags' top where there are no icons to carry them: in a row at the foot of the screen */
#define ROW_GAP 14

/* colours are blue, green, red; text's are doubled, so 0x80 is as the font is */
enum { DOT_GOOD = 0x40E040, DOT_SLOW = 0x20A0FF, DOT_BAD = 0x3030F0, TAG_BACK = 0x383838 };
#define TEXT_OTHER 0x80FFFFFFu /* the top bit: half see-through */
#define TEXT_OWN 0x8040FFFFu

static s32 TagsOnIcons; /* how many the game's own panel carried this frame */

/* "PLAYER 2", or "PLAYERS 2 AND 3", or "PLAYERS 2 TO 4": a machine's, numbered as the game numbers them. */
static void players_text(char *text, size_t size, s32 first, s32 pads) {
    if (pads <= 1) {
        snprintf(text, size, "PLAYER %d", (int)first + 1);
    } else {
        snprintf(text, size, pads == 2 ? "PLAYERS %d AND %d" : "PLAYERS %d TO %d", (int)first + 1, (int)(first + pads));
    }
}

/* A filled box, in the screen's own units (ScreenRect's across, 240 down). */
static void tag_box(s32 x, s32 y, s32 w, s32 h, u32 colour, s32 blend) {
    DVECTOR xy[4];
    u32 c[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        xy[i].vx = (s16)(x + ((i & 1) ? w : 0));
        xy[i].vy = (s16)(y + ((i & 2) ? h : 0));
        c[i] = colour;
    }
    draw_plain_quad(RenderState.layer, xy, c, blend);
}

/* The machine a player is on and how it's doing (NET_), or -1 if the player isn't one of the online game's. */
static s32 player_state(s32 player, s32 *machine) {
    s32 machines = net_machines(), first = 0, pads = 0, m;

    for (m = 0; m < machines; m++) {
        s32 state = net_machine(m, &first, &pads);

        if (player >= first && player < first + pads) {
            *machine = m;
            return state;
        }
    }
    return -1;
}

/* A player's tag: a dot that's green while their connection keeps up, orange when the game has had to wait for it, and red when
   it's stopped answering or gone; and which player they are, or YOU. Faint, so it's there to be looked for and not in the way (the
   user's word). Its middle is at x and its top at y, in text's units; drawn back to front, as what's drawn later in a layer is under
   what's drawn before. */
static void tag_draw(s32 player, s32 x, s32 y) {
    char text[8];
    s32 machine = 0, first = 0, pads = 0, state = player_state(player, &machine), page = FontPage;
    s32 own = machine == net_me(), wide, left, nx, ny, w;
    u32 dot = state == NET_PLAYING ? DOT_GOOD : state == NET_SLOW ? DOT_SLOW : DOT_BAD;

    if (state < 0 || ScreenRect == NULL || sysfont_dbase == NULL || sysfont_dbase->textures.tex == NULL) {
        return;
    }
    net_machine(machine, &first, &pads);
    if (own && pads == 1) {
        snprintf(text, sizeof(text), "YOU");
    } else {
        snprintf(text, sizeof(text), "P%d", (int)player + 1);
    }
    FontPage = 1; /* the small one */
    wide = SysGetLen(text) * 640 / ScreenRect->w;
    /* the dot, a gap, and the words, with a little round them */
    left = x - (10 + 5 + wide) / 2;
    DrawSysText((s16)(left + 15), y, (u8 *)text, own ? TEXT_OWN : TEXT_OTHER, own ? TEXT_OWN : TEXT_OTHER, 0);
    FontPage = page;
    /* the dot: three rows of the screen's own points, the middle one widest, which is round enough at this size. One that's
       stopped answering blinks */
    nx = left * ScreenRect->w / 640;
    ny = y / 2 + 2;
    if (state != NET_QUIET || (FrameCount & 0x10)) {
        tag_box(nx + 1, ny, 3, 1, dot, GFX_OPAQUE);
        tag_box(nx, ny + 1, 5, 2, dot, GFX_OPAQUE);
        tag_box(nx + 1, ny + 3, 3, 1, dot, GFX_OPAQUE);
    }
    /* and a faint dark patch behind it all, its corners cut */
    w = (10 + 5 + wide + 12) * ScreenRect->w / 640;
    nx -= 5;
    tag_box(nx + 1, y / 2, w - 2, 8, TAG_BACK, GFX_SUB);
    tag_box(nx, y / 2 + 1, 1, 6, TAG_BACK, GFX_SUB);
    tag_box(nx + w - 1, y / 2 + 1, 1, 6, TAG_BACK, GFX_SUB);
}

/* From where the game draws a player's icon on its panel (engine.c, DrawLevelHud): the tag sits over the icon, clear of what each
   game puts under it. */
void online_tag_draw(s32 player, s32 x, s32 y) {
    if (net_machines() == 0) {
        return;
    }
    TagsOnIcons++;
    tag_draw(player, x + TAG_ICON_MID, y - TAG_OVER);
}

/* Once a frame, last: the words, for a while, when there's news; and the tags in a row at the foot of the screen where the game
   has no panel to carry them, which is its menus and the warp rooms. */
void online_strip_draw(void) {
    static s32 was[4], changed_at[4], frames;
    char text[96], who[32];
    s32 machines = net_machines(), me = net_me(), page = FontPage;
    s32 players = 0, news = -1, first = 0, pads = 0, on_icons = TagsOnIcons, m, p;

    TagsOnIcons = 0;
    if (machines == 0 || ScreenRect == NULL || sysfont_dbase == NULL || sysfont_dbase->textures.tex == NULL) {
        return;
    }
    frames++;
    for (m = 0; m < machines; m++) {
        s32 state = net_machine(m, &first, &pads);

        /* slow comes and goes, and its dot says it: the words are for a player who's stopped answering, is back, or has left */
        state = state == NET_SLOW ? NET_PLAYING : state;
        players += pads;
        if (state != was[m]) {
            was[m] = state;
            changed_at[m] = frames;
            /* the newest change is the one that's said */
            news = m;
        } else if (changed_at[m] != 0 && frames - changed_at[m] < NEWS_FRAMES && (news < 0 || changed_at[m] > changed_at[news])) {
            news = m;
        }
    }
    if (on_icons == 0 && (ModeTypes[GameMode * 3] == 0x101 || ModeTypes[GameMode * 3] == 0x102)) {
        s32 x = 320 - (players - 1) * (48 + ROW_GAP) / 2;

        for (p = 0; p < players; p++, x += 48 + ROW_GAP) {
            tag_draw(p, x, ROW_Y);
        }
    }
    text[0] = '\0';
    if (news >= 0) {
        s32 state = net_machine(news, &first, &pads);

        players_text(who, sizeof(who), first, pads);
        if (state == NET_QUIET && news == me) {
            snprintf(text, sizeof(text), "CATCHING UP WITH THE OTHERS");
        } else if (state == NET_QUIET) {
            snprintf(text, sizeof(text), pads > 1 ? "%s AREN'T ANSWERING" : "%s ISN'T ANSWERING", who);
        } else if (state == NET_LEFT) {
            snprintf(text, sizeof(text), pads > 1 ? "%s HAVE LEFT" : "%s HAS LEFT", who);
        } else if (news != me) {
            snprintf(text, sizeof(text), pads > 1 ? "%s ARE BACK" : "%s IS BACK", who);
        }
    } else if (frames < NEWS_FRAMES) {
        net_machine(me, &first, &pads);
        players_text(who, sizeof(who), first, pads);
        snprintf(text, sizeof(text), "ONLINE - %d PLAYERS - YOU ARE %s", (int)players, who);
    }
    if (text[0] != '\0') {
        FontPage = 1; /* the small one */
        DrawSysText(-0x8000, NEWS_Y, (u8 *)text, 0xD0FF, 0xFF, 0); /* the menus' own text colours */
        FontPage = page;
    }
}
