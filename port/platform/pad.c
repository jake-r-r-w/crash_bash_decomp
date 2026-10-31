/* libpad on PC (ROADMAP-B 6.2): each vblank writes the host's input into the game's receive buffers,
#include <stdint.h>
typedef unsigned int u32;
 * in the form the pads' hardware gives it: a status byte, a type byte, the buttons active low. With
 * more than two players, port 1 is a multitap, whose packet holds four pads' packets. A controller is an
 * analog pad, with motors if it has them, and the keyboard alone a digital one. */
#include <stdint.h>
#include <string.h>
#include <libpad.h>
#include "../platform/platform.h"

#define NUM_PLAYERS 4
#define RUMBLE_REFRESH 30 /* vblanks: the host's rumble runs out by itself, so a game that stops can't leave it on */

static unsigned char *Buf[2];
static int Running;
static const unsigned char *Act[NUM_PLAYERS];
static unsigned char Motors[NUM_PLAYERS][2];
static int RumbleWait;

/* A pad's number in libpad: its slot on a multitap in port 1, or 0x10 for port 2. */
static int player_of(int port) {
    return port == 0x10 ? 1 : port & (NUM_PLAYERS - 1);
}

void PadInitMtap(unsigned char *buf1, unsigned char *buf2) {
    Buf[0] = buf1;
    Buf[1] = buf2;
    if (buf1 != NULL) {
        buf1[0] = 0xFF;
    }
    if (buf2 != NULL) {
        buf2[0] = 0xFF;
    }
}

void PadStartCom(void) {
    Running = 1;
}

void PadStopCom(void) {
    Running = 0;
}

/* 6: stable, with actuators, so the game sends it rumble. 2: stable, without. */
int PadGetState(int port) {
    return platform_pad_rumbles(player_of(port)) ? 6 : 2;
}

/* The game locks a pad with motors into analog mode, which is the only mode a controller has here. */
int PadSetMainMode(int port, int offs, int lock) {
    return 1;
}

void PadSetAct(int port, unsigned char *table, int len) {
    Act[player_of(port)] = table;
}

/* The game's table is always the small motor, on or off, then the big one's strength. */
int PadSetActAlign(int port, unsigned char *table) {
    int player = player_of(port);

    if (Act[player] != NULL) {
        Motors[player][0] = Act[player][0];
        Motors[player][1] = Act[player][1];
        platform_rumble(player, Motors[player][0], Motors[player][1]);
    }
    return 1;
}

static void refresh_rumble(void) {
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

static void put_pad(unsigned char *p, int player) {
    int buttons = platform_pad(player);

    if (buttons < 0) {
        p[0] = 0xFF;
        p[1] = 0;
    } else {
        uint16_t b = (uint16_t)~buttons;

        p[0] = 0;
        p[1] = platform_pad_sticks(player, &p[4]) ? 0x73 : 0x41;
        memcpy(&p[2], &b, 2);
    }
}

void pad_vblank(void) {
    int i;

    if (!Running || Buf[0] == NULL || Buf[1] == NULL) {
        return;
    }
    refresh_rumble();
    if (platform_pad(2) >= 0 || platform_pad(3) >= 0) {
        Buf[0][0] = 0;
        Buf[0][1] = 0x80;
        for (i = 0; i < 4; i++) {
            put_pad(&Buf[0][2 + i * 8], i);
        }
        Buf[1][0] = 0xFF;
    } else {
        put_pad(Buf[0], 0);
        put_pad(Buf[1], 1);
    }
}
