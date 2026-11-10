#ifndef BASH_MENUMOUSE_H
#define BASH_MENUMOUSE_H

/* The mouse in the game's own menus (main/menumouse.c): it presses the pad, so it's the same online as any press. */

#include "common.h"
#include "../../../platform/controllers.h"

/* A menu, told to the mouse each frame it's on screen and taking presses: whose pad chooses in it (a player from 0, or -1 for
   anyone's), which row is chosen now (by the number its cursor has for it), and how it's chosen. Then each of its rows, by its
   number: a box, left to right and top to bottom, in the game's text's units (a 640 x 480 picture's, from its top left, as
   DrawSysText has them); the box a text makes, drawn at x and y as DrawSysText draws it in the font it's in now; or the box a text
   object (a subengine) made as it was last drawn, which is nothing if it wasn't. */
enum {
    MENU_ACROSS = 1,  /* the rows are side by side, chosen by left and right */
    MENU_START = 2,   /* a row is taken by START, not Cross */
    MENU_NO_BACK = 4, /* Triangle does nothing here: a right click doesn't press it */
    MENU_BUSY = 32,   /* the row chosen won't take its press just now (the cursor's still on its way to it): a click on it waits */
    MENU_VALUES = 8,  /* a row's: it has a value left and right change, so a click on its left half or its right does that */
    MENU_CHOOSE = 16  /* a row's: a click only chooses it, as what Cross does on it isn't what it says (its value's beside it) */
};
void menu_mouse_list(s32 pad, s32 chosen, s32 flags);
void menu_mouse_row(s32 row, s32 left, s32 top, s32 right, s32 bottom, s32 flags);
void menu_mouse_text(s32 row, s16 x, s32 y, const char *text, s32 flags);
void menu_mouse_sub(s32 row, const void *sub, s32 flags);
/* The row just told is a button, not one of the cursor's: a click on its left half presses less, on its right half more (the same
   for one press). */
void menu_mouse_press(s32 less, s32 more);
/* A text object as it's drawn (prims.c's draw_text_nas), for menu_mouse_sub the next frame. */
void menu_mouse_drawn(const void *what, s16 x, s32 y, const char *text);

/* Once a step, before the pads are read: what the mouse has done since, turned into presses for this machine's first pad
   (platform_mouse_buttons). And the launcher's menus (titles/settingsmenu.c), which aren't the game's and take the mouse as it is. */
void menu_mouse_step(void);
void menu_mouse_take(HostMouse *out);

#endif
