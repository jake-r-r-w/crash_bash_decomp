/* The mouse in the game's own menus (the user's word, 2026-11-10: "the menu should be clickable in game too"; the launcher's own
 * menus take it in titles/settingsmenu.c).
 *
 * Every machine in an online game runs the same game from the same presses, so the mouse can't choose a row itself: it presses this
 * machine's first pad, as the keyboard does, and its presses go to the others like any. Pointing at a row presses up or down (or
 * left and right, for rows side by side) until the menu's own cursor is on it; a click does that and then presses Cross, or START
 * where that's what takes a row, or on a row with a value, left or right by the half that's clicked; a right click presses
 * Triangle, and the wheel up and down. Some things on a screen are buttons with no cursor: a click on one is its press (an arrow's
 * left or right, a line saying what Cross does). A mouse lying still does nothing, so it never fights the keys or a controller: a
 * row's pointed at only when the mouse moves.
 *
 * Each menu tells it its rows as it draws them, or as it updates (menu_mouse_list and the rows after it): where they are, which is
 * chosen, and whose pad chooses. A row can be a box, a text as DrawSysText draws it, or a text object (a subengine) as it was drawn
 * the frame before (menu_mouse_drawn is told each one as it's drawn). One press goes at a time, held for a step and let go for one,
 * and the next waits till the cursor's moved, or long enough that it won't: a row it can't get to (one the cursor skips, or past an
 * end) is given up on, and so is anything once the menu's gone or another's up. Only a menu this machine's first player chooses in
 * takes it: in an online game, another machine's player's menu is theirs. */
#include <string.h>
#include "../../platform/net.h"
#include "../../platform/platform.h"
#include "common.h"
#include "bash/main.h"
#include "bash/menumouse.h"
#include "bash/nascalls.h"

#define LISTS 8
#define ROWS 24
#define DRAWN 128
/* How many steps a press has to move the cursor before it's taken that it won't: online, it's played a few steps after it's made */
#define WAIT_STEPS 8
#define WAIT_STEPS_ONLINE 40

typedef struct MenuRow {
    s16 row, flags;
    s16 left, top, right, bottom;
    s32 less, more; /* a button's presses, by the half clicked: 0 for a row of the cursor's */
} MenuRow;

typedef struct MenuList {
    s32 pad, chosen, flags, rows;
    MenuRow row[ROWS];
} MenuList;

typedef struct Drawn {
    const void *what;
    s16 left, top, right, bottom;
} Drawn;

static MenuList Lists[LISTS], Shown[LISTS]; /* the menus told since the last step, and the step before's */
static s32 NumLists, NumShown;
static MenuRow *LastRow;                     /* the row last told, for menu_mouse_press: NULL if it wasn't there */
static Drawn Drawing[DRAWN], Drew[DRAWN];    /* the text objects drawn since the last step, and the step before's */
static s32 NumDrawing, NumDrew;
static HostMouse Mouse; /* what the mouse has done that's not been taken */
static s32 Taken;       /* whether it's been taken this step: what wasn't, by the next, was for nothing and goes */

/* What the mouse is after: a row (-1 none) of the menu it's in, and then what to press on it; a back press, a button's, and the
   wheel's turns */
static MenuList Menu;
static s32 Target = -1, Then, Back, Button, Wheel;
static s32 Waiting, WaitFrom, WaitDir; /* a press that's gone: how many steps more for it to move the cursor, from where, which way */
static s32 Let;                         /* a step with nothing pressed, after a press, so the next is a press again */

void menu_mouse_list(s32 pad, s32 chosen, s32 flags) {
    MenuList *l;

    LastRow = NULL;
    if (NumLists >= LISTS) {
        return;
    }
    l = &Lists[NumLists++];
    l->pad = pad;
    l->chosen = chosen;
    l->flags = flags;
    l->rows = 0;
}

void menu_mouse_row(s32 row, s32 left, s32 top, s32 right, s32 bottom, s32 flags) {
    MenuList *l;
    MenuRow *r;

    LastRow = NULL;
    if (NumLists == 0 || Lists[NumLists - 1].rows >= ROWS) {
        return;
    }
    l = &Lists[NumLists - 1];
    r = &l->row[l->rows++];
    r->row = (s16)row;
    r->flags = (s16)flags;
    r->left = (s16)left;
    r->top = (s16)top;
    r->right = (s16)right;
    r->bottom = (s16)bottom;
    r->less = r->more = 0;
    LastRow = r;
}

void menu_mouse_press(s32 less, s32 more) {
    if (LastRow != NULL) {
        LastRow->row = -1;
        LastRow->less = less;
        LastRow->more = more;
    }
}

/* The box a text makes, drawn at x and y as DrawSysText draws it in the font it's in now: its first line, and only where there's
   more than spaces. 0 if there's nothing to see. */
static s32 text_box(s16 x, s32 y, const char *text, s32 box[4]) {
    const u8 *s = (const u8 *)text;
    s32 x0, col = 0, first = -1, last = 0, height = SysFontWidth[FontPage << 8] * 2;

    if (text == NULL || ScreenRect == NULL) {
        return 0;
    }
    x0 = TextAlignX(x, (u8 *)text);
    for (; *s != 0 && *s != '\n' && *s != '\r'; s++) {
        s32 i = *s + (FontPage << 8);
        s32 w = SysFontWidth[i];

        if (SysFontFlags[i] & 2) {
            col -= w;
        }
        if (*s != ' ') {
            if (first < 0) {
                first = col;
            }
            last = col + w;
        }
        col += w;
    }
    if (first < 0) {
        return 0;
    }
    /* y is the line's top; a line's letters are most of its height */
    box[0] = x0 + first * 640 / ScreenRect->w - 6;
    box[1] = y - 3;
    box[2] = x0 + last * 640 / ScreenRect->w + 6;
    box[3] = y + height * 7 / 10;
    return 1;
}

void menu_mouse_text(s32 row, s16 x, s32 y, const char *text, s32 flags) {
    s32 box[4];

    LastRow = NULL;
    if (text_box(x, y, text, box)) {
        menu_mouse_row(row, box[0], box[1], box[2], box[3], flags);
    }
}

void menu_mouse_drawn(const void *what, s16 x, s32 y, const char *text) {
    s32 box[4];

    if (NumDrawing < DRAWN && text_box(x, y, text, box)) {
        Drawing[NumDrawing].what = what;
        Drawing[NumDrawing].left = (s16)box[0];
        Drawing[NumDrawing].top = (s16)box[1];
        Drawing[NumDrawing].right = (s16)box[2];
        Drawing[NumDrawing++].bottom = (s16)box[3];
    }
}

void menu_mouse_sub(s32 row, const void *sub, s32 flags) {
    s32 i;

    LastRow = NULL;
    for (i = 0; i < NumDrew; i++) {
        if (Drew[i].what == sub) {
            menu_mouse_row(row, Drew[i].left, Drew[i].top, Drew[i].right, Drew[i].bottom, flags);
            return;
        }
    }
}

void menu_mouse_take(HostMouse *out) {
    *out = Mouse;
    Mouse.moved = Mouse.left = Mouse.right = Mouse.wheel = 0;
    Taken = 1;
}

/* Whether the player at that pad is this machine's first (or anyone's, -1): the one the mouse is. */
static s32 ours(s32 pad) {
    return pad < 0 || (net_on() ? net_own_pad(pad) == 0 : pad == 0);
}

/* The same menu as before, whatever's chosen in it and wherever its rows have moved to. */
static s32 same(const MenuList *a, const MenuList *b) {
    s32 i;

    if (a->pad != b->pad || (a->flags & ~MENU_BUSY) != (b->flags & ~MENU_BUSY) || a->rows != b->rows) {
        return 0;
    }
    for (i = 0; i < a->rows; i++) {
        if (a->row[i].row != b->row[i].row || a->row[i].less != b->row[i].less) {
            return 0;
        }
    }
    return 1;
}

/* The row of a menu the mouse is on (NULL none): in the game's text's units, a 640 x 480 picture's from its top left. */
static const MenuRow *row_at(const MenuList *l) {
    float x = Mouse.x + 320.0f, y = Mouse.y + 240.0f;
    s32 i;

    if (!Mouse.on) {
        return NULL;
    }
    for (i = 0; i < l->rows; i++) {
        const MenuRow *r = &l->row[i];

        if (x >= r->left && x <= r->right && y >= r->top && y <= r->bottom) {
            return r;
        }
    }
    return NULL;
}

/* Which half of a row the mouse is on: the left (1) or the right (0). */
static s32 left_half(const MenuRow *r) {
    return Mouse.x + 320.0f < (r->left + r->right) / 2.0f;
}

static void give_up(void) {
    Target = -1;
    Then = 0;
}

static void press(s32 buttons) {
    Let = 1;
    platform_mouse_buttons(buttons);
}

void menu_mouse_step(void) {
    HostMouse m;
    const MenuList *l = NULL;
    const MenuRow *on = NULL;
    s32 i;

    /* what the mouse has done since */
    host_mouse(&m);
    if (!Taken) {
        Mouse.moved = Mouse.left = Mouse.right = Mouse.wheel = 0;
    }
    Taken = 0;
    Mouse.on = m.on;
    Mouse.x = m.x;
    Mouse.y = m.y;
    Mouse.moved |= m.moved;
    Mouse.left += m.left;
    Mouse.right += m.right;
    Mouse.wheel += m.wheel;

    /* the menus told since the last step: the one the mouse is on, or the first of ours; and the text drawn since */
    memcpy(Shown, Lists, sizeof(Lists));
    NumShown = NumLists;
    NumLists = 0;
    LastRow = NULL;
    memcpy(Drew, Drawing, sizeof(Drawing));
    NumDrew = NumDrawing;
    NumDrawing = 0;
    platform_mouse_buttons(0);
    for (i = 0; i < NumShown && l == NULL; i++) {
        if (ours(Shown[i].pad) && (on = row_at(&Shown[i])) != NULL) {
            l = &Shown[i];
        }
    }
    for (i = 0; i < NumShown && l == NULL; i++) {
        if (ours(Shown[i].pad)) {
            l = &Shown[i];
        }
    }
    if (l == NULL) {
        /* no menu of ours: what the mouse did is left for the launcher, if it's up */
        give_up();
        Back = Button = Wheel = Waiting = Let = 0;
        return;
    }
    Taken = 1;
    if (!same(l, &Menu)) {
        give_up();
        Back = Button = Wheel = Waiting = 0;
    }
    Menu = *l;

    /* what's asked for */
    if (Mouse.right && !(l->flags & MENU_NO_BACK)) {
        Back = 1;
        give_up();
    }
    if (Mouse.wheel != 0) {
        Wheel += Mouse.wheel;
        give_up();
    }
    if (on != NULL && on->row < 0) {
        if (Mouse.left) {
            Button = left_half(on) ? on->less : on->more;
            give_up();
        }
    } else if (on != NULL && (Mouse.moved || Mouse.left)) {
        Target = on->row;
        Then = 0;
        if (Mouse.left) {
            Then = on->flags & MENU_CHOOSE ? 0
                 : on->flags & MENU_VALUES ? (left_half(on) ? PAD_LEFT : PAD_RIGHT)
                 : l->flags & MENU_START   ? PAD_START
                                           : PAD_CROSS;
        }
    }
    Mouse.moved = Mouse.left = Mouse.right = Mouse.wheel = 0;

    /* one press at a time */
    if (Let) {
        Let = 0;
        return;
    }
    if (Waiting > 0) {
        if (l->chosen == WaitFrom && --Waiting > 0) {
            return;
        }
        if (Target >= 0 && (l->chosen == WaitFrom || (WaitDir > 0 ? l->chosen < WaitFrom || l->chosen > Target : l->chosen > WaitFrom || l->chosen < Target))) {
            give_up(); /* it didn't move, went round past an end, or skipped the row: the row can't be got to */
        }
        Waiting = 0;
    }
    if (Back) {
        Back = 0;
        press(PAD_TRIANGLE);
        return;
    }
    if (Button) {
        press(Button);
        Button = 0;
        return;
    }
    if (Wheel != 0) {
        WaitDir = Wheel > 0 ? -1 : 1;
        Wheel -= Wheel > 0 ? 1 : -1;
    } else if (Target >= 0 && l->chosen != Target) {
        WaitDir = Target > l->chosen ? 1 : -1;
    } else {
        if (Target >= 0 && Then != 0) {
            if (l->flags & MENU_BUSY) {
                return; /* the cursor's still on its way: the row's press waits till it's there */
            }
            press(Then);
        }
        give_up();
        return;
    }
    press(l->flags & MENU_ACROSS ? (WaitDir > 0 ? PAD_RIGHT : PAD_LEFT) : (WaitDir > 0 ? PAD_DOWN : PAD_UP));
    Waiting = net_on() ? WAIT_STEPS_ONLINE : WAIT_STEPS;
    WaitFrom = l->chosen;
}
