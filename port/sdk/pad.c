/* libpad on PC (ROADMAP-B 6.2): each vblank writes the host's input into the game's two receive
 * buffers, in the form the pads' hardware gives it: a status byte, a type byte, the buttons active
 * low. Digital pads for now, so the game asks for no rumble. */
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

void pad_vblank(void) {
    int i;

    if (!Running) {
        return;
    }
    for (i = 0; i < 2; i++) {
        int buttons;

        if (Buf[i] == NULL) {
            continue;
        }
        buttons = platform_pad(i);
        if (buttons < 0) {
            Buf[i][0] = 0xFF;
            Buf[i][1] = 0;
        } else {
            uint16_t b = (uint16_t)~buttons;

            Buf[i][0] = 0;
            Buf[i][1] = 0x41;
            memcpy(&Buf[i][2], &b, 2);
        }
    }
}
