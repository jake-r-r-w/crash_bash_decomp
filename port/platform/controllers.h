/* The controllers (sdl.c): up to four, each a player's, player 1's first. One pulled out leaves its player with none, and nobody else's
 * player changes; it comes back to its player when it's plugged in again. The launcher's players screen puts them in order. */
#ifndef CONTROLLERS_H
#define CONTROLLERS_H

int host_controllers(void);                  /* how many are plugged in */
const char *host_controller_name(int player); /* player's (0 to 3), or NULL if it has none */
int host_controller_pressed(int player);     /* the buttons its controller's pressed since this was last asked (PAD_ bits) */
void host_controllers_swap(int a, int b);    /* players a and b swap controllers */
void host_controllers_close_gaps(void);      /* before a game: those plugged in to players 1 on, in their order */
void host_controller_buzz(int player);       /* a short rumble, so its player knows which it is */

/* Pretend controllers, for test runs: SDL's virtual gamepads, plugged in, pressed and pulled out as real ones are (--pretend-pads) */
void host_pretend_pads(int count);
void host_pretend_plug(int n, int in);
void host_pretend_press(int n, int buttons);

#endif
