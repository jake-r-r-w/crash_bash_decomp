/* Online play (net.c; docs/PORT.md, Online play): one machine hosts, the others join it, and every machine runs the whole game from
 * everyone's presses. */
#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <stdint.h>

/* The port a game's hosted on from the launcher, and looked for on. */
#define NET_PORT 47000

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
    int local;          /* a test: the launcher's games are this PC's alone: hosted at 127.0.0.1, and looked for there (--online-local) */
    int port;           /* the launcher's games' port, if not NET_PORT (--online-port) */
} NetOptions;

/* What the command line asked of online play, for a game from there or from the launcher: before either. */
void net_configure(const NetOptions *options);
/* From the command line (--host, --join, --replay), before the game starts: hosts and waits for the others, or joins and waits for
   the host's start, or opens the recording. 0: it couldn't, and has said why; -1: the window was closed while it waited. */
int net_start(void);

/* The lobby, for the launcher's ONLINE screens: hosting or joining a frame at a time. net_lobby_poll, once a frame, says where it's
   got to. */
enum {
    LOBBY_OFF,
    LOBBY_FINDING,  /* looking for games on this network (net_lobby_found) */
    LOBBY_OPEN,     /* hosting: others can join (net_lobby_machines), till net_lobby_start */
    LOBBY_STARTING, /* hosting: the others are being sent the start */
    LOBBY_ASKING,   /* joining: no answer yet */
    LOBBY_IN,       /* joined, and waiting for the host to start */
    LOBBY_GO,       /* everyone's ready: the game's to start now, from its beginning (platform_online_start) */
    LOBBY_FAILED    /* it couldn't: net_lobby_problem says why */
};
int net_lobby_host(const char *where); /* NULL: the usual port. 0: it couldn't */
void net_lobby_start(void);
void net_lobby_find(void);
int net_lobby_found(int i, char *text, size_t room); /* the i'th game found: its address, and how many machines are in it (0: no i'th) */
int net_lobby_join(const char *where); /* an address, with a port or the usual one. 0: that's no address */
int net_lobby_poll(void);
int net_lobby_machines(void);                          /* hosting: how many machines are here, this one too */
int net_lobby_machine(int m, char *text, size_t room); /* its address and ping (nothing for this one), and how many players it brings */
const char *net_lobby_address(void);                   /* this machine's address on its network, for the others to join */
const char *net_lobby_problem(void);
void net_lobby_leave(void); /* backing out: the others are told */
void net_lobby_reset(void); /* a game that's over, put away: another can be hosted or joined */
void net_record(void);      /* keeps the game's presses, if that was asked (--net-record): as it starts */

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
/* As the game or the program ends: the others are told, and a test run says how it went. */
void net_end(void);

#endif
