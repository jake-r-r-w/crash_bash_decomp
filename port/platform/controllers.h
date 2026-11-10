/* The controllers (sdl.c): up to four, each a player's, player 1's first. One pulled out leaves its player with none, and nobody else's
 * player changes; it comes back to its player when it's plugged in again. The launcher's players screen puts them in order. */
#ifndef CONTROLLERS_H
#define CONTROLLERS_H

int host_controllers(void);                  /* how many are plugged in */
const char *host_controller_name(int player); /* player's (0 to 3), or NULL if it has none */
int host_controller_pressed(int player);     /* the buttons its controller's pressed since this was last asked (PAD_ bits) */
int host_controller_stick(int player, int *x, int *y); /* where its left stick is, as SDL has it (-32768 to 32767): 0 if it has none */
void host_controllers_swap(int a, int b);    /* players a and b swap controllers */
/* The mouse, for the launcher's menus: where it is on the picture, in a 640 x 480 picture's units from its middle (gfx_window_point),
   whether it's there at all, and since this was last asked whether it's moved, its left and right clicks, and the wheel's turns (up
   is more). A touch screen's taps aren't it: touch has buttons of its own (touch.c). A test's pretend mouse (--mouse) does the same. */
typedef struct HostMouse {
    float x, y;
    int on, moved, left, right, wheel;
} HostMouse;
void host_mouse(HostMouse *out);
void host_pretend_mouse(float x, float y, int left, int right, int wheel);
void host_controllers_close_gaps(void);      /* before a game: those plugged in to players 1 on, in their order */
void host_controller_buzz(int player);       /* a short rumble, so its player knows which it is */
/* Several copies of the game on one PC (--controllers): this one uses all the controllers (-1), none (0), or only the Nth found, from 1,
   and that one whether or not its window has the focus. Before the window opens. */
void host_controllers_use(int which);

/* Pretend controllers, for test runs: SDL's virtual gamepads, plugged in, pressed and pulled out as real ones are (--pretend-pads).
   A press is of the buttons where the game's are by default, whatever they're set to now. */
void host_pretend_pads(int count);
void host_pretend_stick(int n, int x, int y); /* a pretend controller's left stick, in percent of the way out each way */
void host_pretend_plug(int n, int in);
void host_pretend_press(int n, int buttons);
/* and a pretend key held down or let go (--key-press), an SDL scancode, through the keyboard's settings as a real one is */
void host_pretend_key(int key, int down);

#endif
