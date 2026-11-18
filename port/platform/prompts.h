/* Button prompts (docs/PORT.md, Button prompts): the game's pictures of the PlayStation's face buttons in its font (\xFC Cross, \xFD
 * Circle, \xFE Triangle, \xFF Square) drawn instead as what plays that button on what player 1 is using, as modern games' prompts
 * are: an Xbox, PlayStation or Nintendo controller's button, a key, or the mouse's. The pictures are drawn here in code, at the size
 * they're shown, so nothing ships. Only the drawing changes: the game, its text's layout and an online game's state never see it. */
#ifndef PROMPTS_H
#define PROMPTS_H

#include <stdint.h>

/* The setting (settings.h, button_prompts): AUTO follows player 1's last press; the others are always that */
enum {
    PROMPTS_AUTO,
    PROMPTS_ORIGINAL, /* the game's own pictures */
    PROMPTS_XBOX,
    PROMPTS_PLAYSTATION,
    PROMPTS_NINTENDO,
    PROMPTS_KEYBOARD,
    NUM_PROMPTS
};

const char *prompts_name(int style); /* as the menu shows it: AUTO, XBOX... */
const char *prompts_id(int style);   /* as the settings file has it: auto, xbox... */
int prompts_from_id(const char *id); /* or -1 */

/* What player 1 last pressed with (sdl.c), for AUTO, and what their controller is */
enum { PROMPT_DEVICE_KEYS, PROMPT_DEVICE_MOUSE, PROMPT_DEVICE_PAD, PROMPT_DEVICE_TOUCH };
int host_prompt_device(void);
int host_pad_prompts(void); /* player 1's controller's kind, PROMPTS_XBOX, _PLAYSTATION or _NINTENDO, or PROMPTS_ORIGINAL with none */

/* What's drawn now, AUTO worked out: PROMPTS_ORIGINAL to PROMPTS_KEYBOARD */
int prompts_showing(void);
/* The launcher's CONTROLS page is where keys are chosen, with the mouse too: there a key's shown, not the mouse's buttons */
void prompts_keys_only(int on);

/* A face button's picture (CONTROL_CROSS to CONTROL_TRIANGLE, controls.h) in place of the font's, for a texture of w x h texels on a
 * picture target_h pixels high: RGBA (0xAABBGGRR, alpha 0 or 255), *art_w x *art_h, kept here, and an id for it that changes when
 * it does. NULL: the game's own. */
const uint32_t *prompts_art(int control, int w, int h, int target_h, int *art_w, int *art_h, uint64_t *id);

/* A test run (--prompt-sheet FILE): every style's pictures of the four, and of a few other buttons and keys, side by side as a PNG */
int prompts_sheet(const char *path);

#endif
