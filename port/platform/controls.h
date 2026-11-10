/* The game's buttons, and what plays them: a key each on the keyboard, and a button each on a controller, the same for every
 * controller (controls.c). The settings keep them, sdl.c reads them, and the launcher's CONTROLS page changes them. */
#ifndef CONTROLS_H
#define CONTROLS_H

enum {
    CONTROL_CROSS,
    CONTROL_CIRCLE,
    CONTROL_SQUARE,
    CONTROL_TRIANGLE,
    CONTROL_START,
    CONTROL_SELECT,
    CONTROL_L1,
    CONTROL_R1,
    CONTROL_L2,
    CONTROL_R2,
    CONTROL_UP,
    CONTROL_DOWN,
    CONTROL_LEFT,
    CONTROL_RIGHT,
    NUM_CONTROLS
};

/* A controller's buttons are SDL's (SDL_GamepadButton), and its triggers, pulled past halfway, these */
#define BUTTON_LEFT_TRIGGER 100
#define BUTTON_RIGHT_TRIGGER 101
/* Keys are SDL's scancodes. Escape is no control's: it keeps a key as it was, on the CONTROLS page */
#define KEY_ESCAPE 41 /* SDL_SCANCODE_ESCAPE */

const char *control_name(int control); /* as the menus say it: CROSS, L1, UP */
int control_bit(int control);          /* its PAD_ bit (platform.h) */

void controls_default_keys(int keys[NUM_CONTROLS]);
void controls_default_buttons(int buttons[NUM_CONTROLS]);
/* control gets code, and the control that had code gets control's old one, so every control keeps one of its own */
void controls_bind(int binds[NUM_CONTROLS], int control, int code);

/* The settings file's names (SDL's own, so they read back whatever the keyboard), and the menu's, in the game's letters */
const char *controls_key_id(int key);
int controls_key_from_id(const char *id);
const char *controls_button_id(int button);
int controls_button_from_id(const char *id);
const char *controls_key_label(int key);
const char *controls_button_label(int button); /* by the first controller plugged in: A, CROSS, L1, LB... */
const char *control_file_id(int control);     /* cross, l1, up: key_cross and pad_cross in the settings file */

/* The key or controller button pressed since this was last asked, or -1 (sdl.c), for a page waiting for one */
int host_key_pressed_any(void);
int host_button_pressed_any(void);

#endif
