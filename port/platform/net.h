/* Online play (net.c; docs/PORT.md, Online play): one machine hosts, the others join it, and every machine runs the whole game from
 * everyone's presses. */
#ifndef NET_H
#define NET_H

#include <stdint.h>

typedef struct NetOptions {
    const char *host;   /* --host [ADDRESS:]PORT: host a game there, or NULL */
    const char *join;   /* --join ADDRESS:PORT: join the game hosted there, or NULL */
    const char *replay; /* --replay FILE: play a recorded game's presses again, with nobody else, or NULL */
    const char *record; /* --net-record FILE: keep the game's presses, step by step, or NULL */
    int joiners;        /* how many machines the host waits for before it starts */
    int delay;          /* how many steps late a press is used, or 0 for what the ping asks */
    int lag, loss;      /* pretend, for tests: milliseconds each packet takes, and how many in a hundred are lost */
    int patience;       /* how long the host waits for a machine that's gone quiet before playing on without it, ms (0: 5 seconds) */
    int differ_at;      /* a test: this machine's fingerprints are wrong from this step on, or -1 */
    int vanish_at;      /* a test: at this step the program ends without a word to anyone, as a machine switched off, or -1 */
    int stall_at, stall_ms; /* a test: at this step the machine does nothing for that long, as a window being dragged, or -1 */
} NetOptions;

/* Before the game starts: hosts and waits for the others, or joins and waits for the host's start, or opens the recording. 0: it
   couldn't, and has said why; -1: the window was closed while it waited. */
int net_start(const NetOptions *options);
/* Whether the pads are an online game's (or a recording's), not this machine's own. */
int net_on(void);
/* Before the game reads the pads, once a step: this machine's own go out, and it waits for everyone's for this step. */
void net_step(void);
/* The pads of the step being played, as platform_pad and its like give them. */
int net_pad(int pad);
int net_pad_sticks(int pad, unsigned char sticks[4]);
int net_pad_rumbles(int pad);
/* Which of this machine's own pads a player's is, or -1 if it's another machine's. */
int net_own_pad(int pad);
/* After a step, with the game's fingerprint (game/main/fingerprint.c): the machines compare them once a second. */
void net_step_done(uint32_t fingerprint);
/* Once a vblank: what's arrived is taken in, so nobody waits on this machine longer than that. */
void net_vblank(void);
/* Whether this machine is behind the host, and should play on without waiting for its clock. */
int net_behind(void);
/* As the program ends: the others are told, and a test run says how it went. */
void net_end(void);

#endif
