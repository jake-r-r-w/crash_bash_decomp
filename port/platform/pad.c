/* libpad on PC (ROADMAP-B 6.2): each vblank writes the host's input into the game's receive buffers,
#include <stdint.h>
typedef unsigned int u32;
 * in the form the pads' hardware gives it: a status byte, a type byte, the buttons active low. With
 * more than two players, port 1 is a multitap, whose packet holds four pads' packets. Digital pads for
 * now, so the game asks for no rumble. */
#include <stdint.h>
#include <string.h>
#include <libpad.h>
#include "../platform/platform.h"

static unsigned char *Buf[2];
static int Running;

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

/* 2: stable, no actuators, so pad_rumble_send and the analog switch stay off */
int PadGetState(int port) {
    return 2;
}

int PadSetMainMode(int port, int offs, int lock) {
    return 0;
}

void PadSetAct(int port, unsigned char *table, int len) {
}

int PadSetActAlign(int port, unsigned char *table) {
    return 0;
}

static void put_pad(unsigned char *p, int buttons) {
    if (buttons < 0) {
        p[0] = 0xFF;
        p[1] = 0;
    } else {
        uint16_t b = (uint16_t)~buttons;

        p[0] = 0;
        p[1] = 0x41;
        memcpy(&p[2], &b, 2);
    }
}

void pad_vblank(void) {
    int i;

    if (!Running || Buf[0] == NULL || Buf[1] == NULL) {
        return;
    }
    if (platform_pad(2) >= 0 || platform_pad(3) >= 0) {
        Buf[0][0] = 0;
        Buf[0][1] = 0x80;
        for (i = 0; i < 4; i++) {
            put_pad(&Buf[0][2 + i * 8], platform_pad(i));
        }
        Buf[1][0] = 0xFF;
    } else {
        put_pad(Buf[0], platform_pad(0));
        put_pad(Buf[1], platform_pad(1));
    }
}
