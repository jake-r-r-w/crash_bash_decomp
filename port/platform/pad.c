/* What's left of a pad that the host doesn't do by itself: its rumble runs out, so a game that's asked for it for a long time has to keep asking. */
#include "platform.h"

#define NUM_PLAYERS 4
#define RUMBLE_REFRESH 30 /* vblanks */

static unsigned char Motors[NUM_PLAYERS][2];
static int RumbleWait;

void pad_set_motors(int player, int small_on, int big) {
    Motors[player][0] = (unsigned char)small_on;
    Motors[player][1] = (unsigned char)big;
    platform_rumble(player, small_on, big);
}

void pad_vblank(void) {
    int i;

    if (++RumbleWait < RUMBLE_REFRESH) {
        return;
    }
    RumbleWait = 0;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (Motors[i][0] != 0 || Motors[i][1] != 0) {
            platform_rumble(i, Motors[i][0], Motors[i][1]);
        }
    }
}
