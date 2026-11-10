#ifndef BASH_ONLINE_H
#define BASH_ONLINE_H

/* What the game does differently in an online game (main/online.c). */

#include "common.h"

/* The sound options this machine plays by and shows: the game's own (GameOptions), or in an online game this machine's own. */
s32 heard_music(void);
s32 heard_sfx(void);
s32 heard_stereo(void);

/* Whether the options are each machine's own: an online game. */
s32 own_options(void);
/* Whether a press at that pad is this machine's to act on, for an option that's its own: always, but in an online game only when
   the pad is one of its own players'. */
s32 own_press(s32 pad);

/* Online, a press on one of the sound options by the player at that pad: this machine's own changes, if the player is its own.
   dir is -1 or 1 for a volume, a step down or up; stereo just changes over. */
enum { OWN_STEREO, OWN_MUSIC, OWN_SFX };
void own_sound_change(s32 pad, s32 which, s32 dir);
/* As the game starts: this machine's own stereo or mono, if the game's an online one. */
void own_sound_start(void);

/* Online, the tag a connected player carries: from where the game draws that player's icon on its panel, at x and y. And once a
   frame, last: the tags where there's no panel, and a few words when there's news. */
void online_tag_draw(s32 player, s32 x, s32 y);
void online_strip_draw(void);

#endif
