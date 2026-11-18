/* Button prompts (prompts.h; docs/PORT.md, Button prompts). The pictures are Kenney's Input Prompts (www.kenney.nl, CC0;
 * port/assets/prompts/License.txt), built into the program as data (assets/prompts/embed.cmake). Each is a white shape with its letter
 * cut out, so the letter's hole is filled dark and a dark rim put round it, as the game's own letters have, to read on any background.
 * Then it's made the size it's shown, and stands in the font's 32 x 16 texels of the original, so no text moves. */
#include "prompts.h"

#include <SDL3/SDL.h>
#include <string.h>

#include "controls.h"
#include "settings.h"

static const char *const Names[NUM_PROMPTS] = { "AUTO", "ORIGINAL", "XBOX", "PLAYSTATION", "NINTENDO", "STEAM DECK", "KEYBOARD" };
static const char *const Ids[NUM_PROMPTS] = { "auto", "original", "xbox", "playstation", "nintendo", "steamdeck", "keyboard" };

const char *prompts_name(int style) {
    return style >= 0 && style < NUM_PROMPTS ? Names[style] : Names[0];
}

const char *prompts_id(int style) {
    return style >= 0 && style < NUM_PROMPTS ? Ids[style] : Ids[0];
}

int prompts_from_id(const char *id) {
    int i;

    for (i = 0; i < NUM_PROMPTS; i++) {
        if (SDL_strcasecmp(id, Ids[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static int KeysOnly;

void prompts_keys_only(int on) {
    KeysOnly = on != 0;
}

/* What's drawn: the setting, or with AUTO what player 1 pressed last. Touch has the game's own, as its buttons are the PlayStation's
   shapes. The mouse is a keyboard's player's, so it's keys, but with the mouse's own buttons for what it does (prompt_name) */
int prompts_showing(void) {
    int style = settings()->button_prompts;

    if (style != PROMPTS_AUTO) {
        return style;
    }
    switch (host_prompt_device()) {
    case PROMPT_DEVICE_PAD:
        return host_pad_prompts();
    case PROMPT_DEVICE_TOUCH:
        return PROMPTS_ORIGINAL;
    default:
        return PROMPTS_KEYBOARD;
    }
}

/* --- which of Kenney's pictures --- */

/* A controller's button, SDL's (by its place: south, east, west, north) or a trigger, in a style */
static const char *pad_picture(int style, int button) {
    static const char *const Xbox[] = { "xbox_button_color_a", "xbox_button_color_b", "xbox_button_color_x", "xbox_button_color_y" };
    static const char *const Ps[] = { "playstation_button_color_cross", "playstation_button_color_circle", "playstation_button_color_square",
                                      "playstation_button_color_triangle" };
    /* a Nintendo's A is on the right, its B at the bottom */
    static const char *const Switch[] = { "switch_button_b", "switch_button_a", "switch_button_y", "switch_button_x" };
    static const char *const Deck[] = { "steamdeck_button_a", "steamdeck_button_b", "steamdeck_button_x", "steamdeck_button_y" };
    int ps = style == PROMPTS_PLAYSTATION, sw = style == PROMPTS_NINTENDO, deck = style == PROMPTS_STEAMDECK;

    switch (button) {
    case SDL_GAMEPAD_BUTTON_SOUTH:
    case SDL_GAMEPAD_BUTTON_EAST:
    case SDL_GAMEPAD_BUTTON_WEST:
    case SDL_GAMEPAD_BUTTON_NORTH:
        return (ps ? Ps : sw ? Switch : deck ? Deck : Xbox)[button - SDL_GAMEPAD_BUTTON_SOUTH];
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return ps ? "playstation_trigger_l1" : sw ? "switch_button_l" : deck ? "steamdeck_button_l1" : "xbox_lb";
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return ps ? "playstation_trigger_r1" : sw ? "switch_button_r" : deck ? "steamdeck_button_r1" : "xbox_rb";
    case BUTTON_LEFT_TRIGGER: return ps ? "playstation_trigger_l2" : sw ? "switch_button_zl" : deck ? "steamdeck_button_l2" : "xbox_lt";
    case BUTTON_RIGHT_TRIGGER: return ps ? "playstation_trigger_r2" : sw ? "switch_button_zr" : deck ? "steamdeck_button_r2" : "xbox_rt";
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return ps ? "playstation_button_l3" : sw ? "switch_stick_l_press" : deck ? "steamdeck_stick_l_press" : "xbox_ls";
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return ps ? "playstation_button_r3" : sw ? "switch_stick_r_press" : deck ? "steamdeck_stick_r_press" : "xbox_rs";
    case SDL_GAMEPAD_BUTTON_START: return ps ? "playstation5_button_options" : sw ? "switch_button_plus" : deck ? "steamdeck_button_options" : "xbox_button_menu";
    case SDL_GAMEPAD_BUTTON_BACK: return ps ? "playstation5_button_create" : sw ? "switch_button_minus" : deck ? "steamdeck_button_view" : "xbox_button_view";
    case SDL_GAMEPAD_BUTTON_GUIDE: return ps ? NULL : sw ? "switch_button_home" : deck ? "steamdeck_button_guide" : "xbox_guide";
    case SDL_GAMEPAD_BUTTON_MISC1: return ps || sw || deck ? NULL : "xbox_button_share";
    case SDL_GAMEPAD_BUTTON_TOUCHPAD: return ps ? "playstation5_touchpad" : NULL;
    case SDL_GAMEPAD_BUTTON_DPAD_UP: return ps ? "playstation_dpad_up" : sw ? "switch_dpad_up" : deck ? "steamdeck_dpad_up" : "xbox_dpad_up";
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return ps ? "playstation_dpad_down" : sw ? "switch_dpad_down" : deck ? "steamdeck_dpad_down" : "xbox_dpad_down";
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return ps ? "playstation_dpad_left" : sw ? "switch_dpad_left" : deck ? "steamdeck_dpad_left" : "xbox_dpad_left";
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return ps ? "playstation_dpad_right" : sw ? "switch_dpad_right" : deck ? "steamdeck_dpad_right" : "xbox_dpad_right";
    /* the back paddles: an Xbox Elite's, or the Deck's L4, R4, L5, R5 */
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return deck ? "steamdeck_button_r4" : ps || sw ? NULL : "xbox_elite_paddle_top_right";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return deck ? "steamdeck_button_l4" : ps || sw ? NULL : "xbox_elite_paddle_top_left";
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return deck ? "steamdeck_button_r5" : ps || sw ? NULL : "xbox_elite_paddle_bottom_right";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return deck ? "steamdeck_button_l5" : ps || sw ? NULL : "xbox_elite_paddle_bottom_left";
    default: return NULL;
    }
}

/* A key, an SDL scancode: Kenney's name for its cap, in buf */
static const char *key_picture(int key, char *buf, size_t size) {
    const char *name = NULL;

    /* a mouse button that plays a control (controls.h, KEY_MOUSE) */
    if (KEY_IS_MOUSE(key)) {
        static const char *const Mice[5] = { "mouse_left", "mouse_scroll", "mouse_right", "mouse_side_back", "mouse_side_forward" };

        return Mice[key - KEY_MOUSE - 1];
    }

    if (key >= SDL_SCANCODE_A && key <= SDL_SCANCODE_Z) {
        SDL_snprintf(buf, size, "keyboard_%c", 'a' + key - SDL_SCANCODE_A);
        return buf;
    }
    if (key >= SDL_SCANCODE_1 && key <= SDL_SCANCODE_9) {
        SDL_snprintf(buf, size, "keyboard_%c", '1' + key - SDL_SCANCODE_1);
        return buf;
    }
    if (key >= SDL_SCANCODE_KP_1 && key <= SDL_SCANCODE_KP_9) {
        SDL_snprintf(buf, size, "keyboard_%c", '1' + key - SDL_SCANCODE_KP_1);
        return buf;
    }
    if (key >= SDL_SCANCODE_F1 && key <= SDL_SCANCODE_F12) {
        SDL_snprintf(buf, size, "keyboard_f%d", 1 + key - SDL_SCANCODE_F1);
        return buf;
    }
    switch (key) {
    case SDL_SCANCODE_0: case SDL_SCANCODE_KP_0: name = "keyboard_0"; break;
    case SDL_SCANCODE_UP: name = "keyboard_arrow_up"; break;
    case SDL_SCANCODE_DOWN: name = "keyboard_arrow_down"; break;
    case SDL_SCANCODE_LEFT: name = "keyboard_arrow_left"; break;
    case SDL_SCANCODE_RIGHT: name = "keyboard_arrow_right"; break;
    case SDL_SCANCODE_RETURN: case SDL_SCANCODE_RETURN2: name = "keyboard_enter"; break;
    case SDL_SCANCODE_KP_ENTER: name = "keyboard_numpad_enter"; break;
    case SDL_SCANCODE_SPACE: name = "keyboard_space"; break;
    case SDL_SCANCODE_ESCAPE: name = "keyboard_escape"; break;
    case SDL_SCANCODE_TAB: name = "keyboard_tab"; break;
    case SDL_SCANCODE_BACKSPACE: name = "keyboard_backspace"; break;
    case SDL_SCANCODE_LSHIFT: case SDL_SCANCODE_RSHIFT: name = "keyboard_shift"; break;
    case SDL_SCANCODE_LCTRL: case SDL_SCANCODE_RCTRL: name = "keyboard_ctrl"; break;
    case SDL_SCANCODE_LALT: case SDL_SCANCODE_RALT: name = "keyboard_alt"; break;
    case SDL_SCANCODE_LGUI: case SDL_SCANCODE_RGUI: name = "keyboard_win"; break;
    case SDL_SCANCODE_CAPSLOCK: name = "keyboard_capslock"; break;
    case SDL_SCANCODE_DELETE: name = "keyboard_delete"; break;
    case SDL_SCANCODE_INSERT: name = "keyboard_insert"; break;
    case SDL_SCANCODE_HOME: name = "keyboard_home"; break;
    case SDL_SCANCODE_END: name = "keyboard_end"; break;
    case SDL_SCANCODE_PAGEUP: name = "keyboard_page_up"; break;
    case SDL_SCANCODE_PAGEDOWN: name = "keyboard_page_down"; break;
    case SDL_SCANCODE_MINUS: case SDL_SCANCODE_KP_MINUS: name = "keyboard_minus"; break;
    case SDL_SCANCODE_EQUALS: name = "keyboard_equals"; break;
    case SDL_SCANCODE_KP_PLUS: name = "keyboard_numpad_plus"; break;
    case SDL_SCANCODE_KP_MULTIPLY: name = "keyboard_asterisk"; break;
    case SDL_SCANCODE_LEFTBRACKET: name = "keyboard_bracket_open"; break;
    case SDL_SCANCODE_RIGHTBRACKET: name = "keyboard_bracket_close"; break;
    case SDL_SCANCODE_BACKSLASH: case SDL_SCANCODE_NONUSBACKSLASH: name = "keyboard_slash_back"; break;
    case SDL_SCANCODE_SLASH: case SDL_SCANCODE_KP_DIVIDE: name = "keyboard_slash_forward"; break;
    case SDL_SCANCODE_SEMICOLON: name = "keyboard_semicolon"; break;
    case SDL_SCANCODE_APOSTROPHE: name = "keyboard_apostrophe"; break;
    case SDL_SCANCODE_COMMA: name = "keyboard_comma"; break;
    case SDL_SCANCODE_PERIOD: case SDL_SCANCODE_KP_PERIOD: name = "keyboard_period"; break;
    case SDL_SCANCODE_GRAVE: name = "keyboard_tilde"; break;
    case SDL_SCANCODE_NUMLOCKCLEAR: name = "keyboard_numlock"; break;
    case SDL_SCANCODE_PRINTSCREEN: name = "keyboard_printscreen"; break;
    case SDL_SCANCODE_SCROLLLOCK: name = "keyboard_scroll_lock"; break;
    case SDL_SCANCODE_PAUSE: name = "keyboard_pause"; break;
    default: break;
    }
    return name;
}

/* A control's picture in a style, or NULL for the game's own: also a key with no cap of Kenney's, or a button with no picture */
static const char *prompt_name(int control, int style, char *buf, size_t size) {
    const Settings *s = settings();

    if (style == PROMPTS_ORIGINAL || (control != PROMPT_DPAD && (control < 0 || control >= NUM_CONTROLS))) {
        return NULL;
    }
    /* the d-pad as a whole: the arrow keys, or the controller's d-pad */
    if (control == PROMPT_DPAD) {
        return style == PROMPTS_KEYBOARD ? "keys:" : style == PROMPTS_PLAYSTATION ? "playstation_dpad" : style == PROMPTS_NINTENDO ? "switch_dpad"
             : style == PROMPTS_STEAMDECK ? "steamdeck_dpad" : "xbox_dpad";
    }
    if (style == PROMPTS_KEYBOARD) {
        /* clicking is Cross in the menus, and a right click Triangle (menumouse.c) */
        if (!KeysOnly && s->button_prompts == PROMPTS_AUTO && host_prompt_device() == PROMPT_DEVICE_MOUSE &&
            (control == CONTROL_CROSS || control == CONTROL_TRIANGLE)) {
            return control == CONTROL_CROSS ? "mouse_left" : "mouse_right";
        }
        return key_picture(s->keys[control], buf, size);
    }
    return pad_picture(style, s->buttons[control]);
}

/* --- Kenney's pictures, from the program's data, made ready once each --- */

typedef struct {
    const char *name;
    const unsigned char *png;
    size_t size;
} PromptPng;
extern const PromptPng PromptPngs[]; /* prompt_pngs.c, made by assets/prompts/embed.cmake */

#define INK 0x1E1E22u /* a letter's hole and the rim: the game's letters are outlined in black */
#define MAX_READY 24

#define NAME_SIZE 128
typedef struct {
    char name[NAME_SIZE]; /* Kenney's file's, or for the arrow keys built from four, "keys:" and theirs */
    int w, h;
    uint32_t *px; /* 0xAABBGGRR, straight alpha */
} Ready;
static Ready Readies[MAX_READY];
static int NextReady;

/* The hole of its letter filled (what's clear and can't be reached from the edge), then a rim round it all, rim texels wide */
static void ink_in(uint32_t *px, int w, int h, int rim) {
    unsigned char *outside = SDL_calloc((size_t)w * h, 1);
    int *stack = SDL_malloc(sizeof(int) * (size_t)w * h);
    uint32_t *copy = SDL_malloc(sizeof(uint32_t) * (size_t)w * h);
    int n = 0, i, x, y, dx, dy;

    if (outside == NULL || stack == NULL || copy == NULL) {
        SDL_free(outside);
        SDL_free(stack);
        SDL_free(copy);
        return;
    }
    for (i = 0; i < w * h; i++) {
        x = i % w;
        y = i / w;
        if ((x == 0 || y == 0 || x == w - 1 || y == h - 1) && (px[i] >> 24) < 128) {
            outside[i] = 1;
            stack[n++] = i;
        }
    }
    while (n > 0) {
        int at = stack[--n], ax = at % w, ay = at / w;
        int next[4] = { ax > 0 ? at - 1 : -1, ax < w - 1 ? at + 1 : -1, ay > 0 ? at - w : -1, ay < h - 1 ? at + w : -1 };

        for (i = 0; i < 4; i++) {
            if (next[i] >= 0 && !outside[next[i]] && (px[next[i]] >> 24) < 128) {
                outside[next[i]] = 1;
                stack[n++] = next[i];
            }
        }
    }
    for (i = 0; i < w * h; i++) {
        if (!outside[i] && (px[i] >> 24) < 255) {
            /* the letter: dark under what's there, so its edge stays smooth */
            uint32_t a = px[i] >> 24, c = px[i];
            uint32_t r = ((c & 0xFF) * a + (INK >> 16 & 0xFF) * (255 - a)) / 255;
            uint32_t g = ((c >> 8 & 0xFF) * a + (INK >> 8 & 0xFF) * (255 - a)) / 255;
            uint32_t b = ((c >> 16 & 0xFF) * a + (INK & 0xFF) * (255 - a)) / 255;

            px[i] = 0xFF000000u | b << 16 | g << 8 | r;
        }
    }
    memcpy(copy, px, sizeof(uint32_t) * (size_t)w * h);
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            uint32_t best = copy[y * w + x] >> 24;

            if (best == 255) {
                continue;
            }
            /* the rim: how near the shape is, as the alpha of a dark texel under it */
            for (dy = -rim; dy <= rim && best < 255; dy++) {
                for (dx = -rim; dx <= rim; dx++) {
                    int sx = x + dx, sy = y + dy;

                    if (sx >= 0 && sy >= 0 && sx < w && sy < h && dx * dx + dy * dy <= rim * rim && (copy[sy * w + sx] >> 24) > best) {
                        best = copy[sy * w + sx] >> 24;
                    }
                }
            }
            if (best > (copy[y * w + x] >> 24)) {
                uint32_t a = copy[y * w + x] >> 24, c = copy[y * w + x];
                uint32_t r = ((c & 0xFF) * a + (INK >> 16 & 0xFF) * (255 - a)) / 255;
                uint32_t g = ((c >> 8 & 0xFF) * a + (INK >> 8 & 0xFF) * (255 - a)) / 255;
                uint32_t bl = ((c >> 16 & 0xFF) * a + (INK & 0xFF) * (255 - a)) / 255;

                px[y * w + x] = best << 24 | bl << 16 | g << 8 | r;
            }
        }
    }
    SDL_free(outside);
    SDL_free(stack);
    SDL_free(copy);
}

/* One of Kenney's pictures as it is, or NULL: malloc'd, *w x *h */
static uint32_t *load(const char *name, int *w, int *h) {
    const PromptPng *p;
    SDL_Surface *loaded, *rgba;
    uint32_t *px;
    int y;

    for (p = PromptPngs; p->name != NULL && SDL_strcmp(p->name, name) != 0; p++) {
    }
    if (p->name == NULL) {
        return NULL;
    }
    loaded = SDL_LoadPNG_IO(SDL_IOFromConstMem(p->png, p->size), true);
    rgba = loaded != NULL ? SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_ABGR8888) : NULL;
    SDL_DestroySurface(loaded);
    if (rgba == NULL) {
        return NULL;
    }
    px = SDL_malloc(sizeof(uint32_t) * (size_t)rgba->w * rgba->h);
    if (px != NULL) {
        for (y = 0; y < rgba->h; y++) {
            memcpy(px + (size_t)y * rgba->w, (const uint8_t *)rgba->pixels + (size_t)y * rgba->pitch, sizeof(uint32_t) * (size_t)rgba->w);
        }
        *w = rgba->w;
        *h = rgba->h;
    }
    SDL_DestroySurface(rgba);
    return px;
}

/* The four keys that move, as the arrow keys sit, in an upside-down T. Kenney's own picture of the arrow keys is blank caps, which the
   size it's shown at leaves as white blocks, so it's made from the four keys bound to up, down, left and right, each with its own sign
   (the user's ask, 2026-11-18). name is "keys:UP,DOWN,LEFT,RIGHT", each key's picture's name. NULL if one has none */
static uint32_t *load_keys(const char *name, int *w, int *h) {
    /* where each goes, in keys across and down: up over down, left and right beside it */
    static const int At[4][2] = { { 1, 0 }, { 1, 1 }, { 0, 1 }, { 2, 1 } };
    char one[NAME_SIZE];
    uint32_t *keys[4] = { NULL, NULL, NULL, NULL }, *px = NULL;
    int kw[4] = { 0 }, kh[4] = { 0 }, i, n = 0, y;
    const char *s = name + 5;

    for (i = 0; i < 4; i++) {
        const char *end = SDL_strchr(s, ',');
        size_t len = end != NULL ? (size_t)(end - s) : SDL_strlen(s);

        SDL_strlcpy(one, s, len + 1 < NAME_SIZE ? len + 1 : NAME_SIZE);
        s = end != NULL ? end + 1 : s + len;
        keys[i] = load(one, &kw[i], &kh[i]);
        n += keys[i] != NULL && kw[i] == kw[0] && kh[i] == kh[0];
    }
    if (n == 4) {
        /* each key's square cut to the caps' own edges (the most of all four), with a little gap between, and a margin round the
           whole for the rim */
        int x0 = kw[0], y0 = kh[0], x1 = 0, y1 = 0, x, cw, ch, gap = kh[0] / 16, edge = kh[0] / 20;

        for (i = 0; i < 4; i++) {
            for (y = 0; y < kh[0]; y++) {
                for (x = 0; x < kw[0]; x++) {
                    if (keys[i][y * kw[0] + x] >> 24) {
                        x0 = x < x0 ? x : x0;
                        x1 = x > x1 ? x : x1;
                        y0 = y < y0 ? y : y0;
                        y1 = y > y1 ? y : y1;
                    }
                }
            }
        }
        cw = x1 - x0 + 1 + gap;
        ch = y1 - y0 + 1 + gap;
        if (cw > gap && ch > gap) {
            *w = cw * 3 - gap + edge * 2;
            *h = ch * 2 - gap + edge * 2;
            px = SDL_calloc((size_t)*w * *h, sizeof(uint32_t));
            for (i = 0; px != NULL && i < 4; i++) {
                for (y = y0; y <= y1; y++) {
                    memcpy(px + (size_t)(edge + At[i][1] * ch + y - y0) * *w + edge + At[i][0] * cw, keys[i] + (size_t)y * kw[0] + x0,
                           sizeof(uint32_t) * (size_t)(x1 - x0 + 1));
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        SDL_free(keys[i]);
    }
    return px;
}

static const Ready *ready(const char *name) {
    Ready *r;
    uint32_t *px;
    int i, w = 0, h = 0;

    for (i = 0; i < MAX_READY; i++) {
        if (Readies[i].name[0] != '\0' && SDL_strcmp(Readies[i].name, name) == 0) {
            return &Readies[i];
        }
    }
    px = SDL_strncmp(name, "keys:", 5) == 0 ? load_keys(name, &w, &h) : load(name, &w, &h);
    if (px == NULL) {
        return NULL;
    }
    r = &Readies[NextReady];
    NextReady = (NextReady + 1) % MAX_READY;
    SDL_free(r->px);
    r->px = px;
    SDL_strlcpy(r->name, name, NAME_SIZE);
    r->w = w;
    r->h = h;
    /* the rim by one key's height, so the four keys have the same as one */
    {
        int one = SDL_strncmp(name, "keys:", 5) == 0 ? r->h / 2 : r->h;

        ink_in(r->px, r->w, r->h, one / 40 > 1 ? one / 40 : 1);
    }
    return r;
}

/* The picture made smaller into out (out_w x out_h, cleared first): side high, as wide as its shape makes that, its middle at cx, cy,
   each pixel the average of 4 x 4 samples, and alpha clear or opaque, which is all the drawing knows */
static void shrink(const Ready *r, uint32_t *out, int out_w, int out_h, float cx, float cy, float side) {
    float across = side * r->w / r->h;
    int x, y, sx, sy;

    memset(out, 0, sizeof(uint32_t) * (size_t)out_w * out_h);
    for (y = 0; y < out_h; y++) {
        for (x = 0; x < out_w; x++) {
            float acc[4] = { 0, 0, 0, 0 };

            for (sy = 0; sy < 4; sy++) {
                for (sx = 0; sx < 4; sx++) {
                    float u = ((x + (sx + 0.5f) / 4) - (cx - across / 2)) / across * r->w;
                    float v = ((y + (sy + 0.5f) / 4) - (cy - side / 2)) / side * r->h;
                    int iu = (int)u, iv = (int)v;
                    uint32_t c, a;

                    if (u < 0 || v < 0 || iu >= r->w || iv >= r->h) {
                        continue;
                    }
                    c = r->px[iv * r->w + iu];
                    a = c >> 24;
                    acc[0] += (float)(c & 0xFF) * a;
                    acc[1] += (float)(c >> 8 & 0xFF) * a;
                    acc[2] += (float)(c >> 16 & 0xFF) * a;
                    acc[3] += (float)a;
                }
            }
            if (acc[3] >= 16 * 255 / 2) {
                out[y * out_w + x] = 0xFF000000u | (uint32_t)(acc[2] / acc[3]) << 16 | (uint32_t)(acc[1] / acc[3]) << 8 | (uint32_t)(acc[0] / acc[3]);
            }
        }
    }
}

/* Every one of Kenney's 128-pixel squares is this many of the game's rows high, so a key or a button is the same size wherever it is
   (the user's word, 2026-11-18): the most the arrow keys' T, three keys across, can be in the d-pad's 64 texels */
#define TILE_ROWS 17.0f
#define TILE_PIXELS 128.0f

/* Where in the font's texels the original has its picture, its middle in texels across and rows down, and how many rows high Kenney's
   picture is (pixels high, a tile or the arrow keys' two rows): a face button's disc, in 30 of its 32 across and 15 of its 16 down;
   L1's and R1's, the left 36 of their 64 and 13 of their 16; the d-pad's, the left 46 of its 64 and 30 of its 32. A controller's d-pad
   is a picture of the whole pad, so it's the original's size */
static void place(int control, int keys, int pixels, int art_w, int art_h, int w, int h, float *cx, float *cy, float *side) {
    float x = 15.0f, y = 7.5f, rows = TILE_ROWS * pixels / TILE_PIXELS;

    if (control == PROMPT_DPAD && keys) {
        x = 32.0f, y = 15.0f;
    } else if (control == PROMPT_DPAD) {
        x = 22.5f, y = 14.5f, rows = 36.0f;
    } else if (control == CONTROL_L1 || control == CONTROL_R1) {
        x = 18.0f, y = 7.0f;
    }
    *cx = x * art_w / w;
    *cy = y * art_h / h;
    *side = rows * art_h / h;
}

/* Kept pictures: one for each of the four, made again when what it shows or its size changes */
typedef struct {
    char name[NAME_SIZE];
    int w, h, art_w, art_h;
    uint32_t *art;
} Kept;
static Kept Pictures[7]; /* the four face buttons, L1, R1 and the d-pad */

const uint32_t *prompts_art(int control, int w, int h, int target_h, int *art_w, int *art_h, uint64_t *id) {
    char buf[32], keys[NAME_SIZE];
    const char *name;
    const Ready *r;
    Kept *k;
    int aw, ah, style = prompts_showing(), i;
    uint64_t hash = 0xCBF29CE484222325ULL;

    int slot = control == PROMPT_DPAD ? 6 : control == CONTROL_L1 ? 4 : control == CONTROL_R1 ? 5 : control - CONTROL_CROSS;

    if (slot < 0 || (slot > CONTROL_TRIANGLE - CONTROL_CROSS && control != PROMPT_DPAD && control != CONTROL_L1 && control != CONTROL_R1) ||
        w <= 0 || h <= 0 || target_h <= 0) {
        return NULL;
    }
    name = prompt_name(control, style, buf, sizeof(buf));
    /* the keyboard's d-pad: the four keys that move, whatever they are */
    if (control == PROMPT_DPAD && style == PROMPTS_KEYBOARD) {
        static const int Moves[4] = { CONTROL_UP, CONTROL_DOWN, CONTROL_LEFT, CONTROL_RIGHT };
        char one[32];

        SDL_strlcpy(keys, "keys:", sizeof(keys));
        for (i = 0; i < 4 && name != NULL; i++) {
            name = key_picture(settings()->keys[Moves[i]], one, sizeof(one));
            if (name != NULL) {
                SDL_strlcat(keys, name, sizeof(keys));
                SDL_strlcat(keys, i < 3 ? "," : "", sizeof(keys));
            }
        }
        name = name != NULL ? keys : NULL;
    }
    r = name != NULL ? ready(name) : NULL;
    if (r == NULL) {
        return NULL;
    }
    /* the game's 240 rows to the picture's height, and its texels 5/8 as wide as tall (a 512 x 240 screen shown at 4:3) */
    ah = (int)((float)h * target_h / 240 + 0.5f);
    aw = (int)((float)w * target_h / 384 + 0.5f);
    ah = ah < h ? h : ah > 512 ? 512 : ah;
    aw = aw < w ? w : aw > 640 ? 640 : aw;
    k = &Pictures[slot];
    if (k->art == NULL || SDL_strcmp(k->name, r->name) != 0 || k->w != w || k->h != h || k->art_w != aw || k->art_h != ah) {
        float cx, cy, side;

        SDL_free(k->art);
        k->art = SDL_malloc((size_t)aw * ah * sizeof(uint32_t));
        if (k->art == NULL) {
            return NULL;
        }
        place(control, SDL_strncmp(r->name, "keys:", 5) == 0, r->h, aw, ah, w, h, &cx, &cy, &side);
        shrink(r, k->art, aw, ah, cx, cy, side);
        SDL_strlcpy(k->name, r->name, NAME_SIZE);
        k->w = w;
        k->h = h;
        k->art_w = aw;
        k->art_h = ah;
    }
    *art_w = aw;
    *art_h = ah;
    /* the picture's name and size are its own */
    for (i = 0; r->name[i] != 0; i++) {
        hash = (hash ^ (unsigned char)r->name[i]) * 0x100000001B3ULL;
    }
    *id = 0x50524F4D50000000ULL ^ (hash << 20) ^ ((uint64_t)aw << 10) ^ (uint64_t)ah;
    return k->art;
}

/* --- a sheet of them all, for looking at --- */

int prompts_sheet(const char *path) {
    static const int Codes[] = { SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_NORTH,
                                 SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, BUTTON_RIGHT_TRIGGER, SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_DPAD_UP };
    static const int Keys[] = { SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_RETURN, SDL_SCANCODE_RSHIFT,
                                SDL_SCANCODE_UP, SDL_SCANCODE_SPACE };
    const int cols = 8, rows = 6, cw = 160, ch = 128, gap = 8;
    int W = cols * (cw + gap) + gap, H = rows * (ch + gap) + gap, row, col, x, y, ok;
    uint32_t *sheet = SDL_calloc((size_t)W * H, sizeof(uint32_t)), *art = SDL_malloc((size_t)cw * ch * sizeof(uint32_t));
    SDL_Surface *s;

    if (sheet == NULL || art == NULL) {
        SDL_free(sheet);
        SDL_free(art);
        return 0;
    }
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            /* light at the top and dark at the bottom, so a picture's edge shows on both */
            uint32_t v = (uint32_t)(0xD0 - y * 0xC0 / H);

            sheet[y * W + x] = 0xFF000000u | (v / 2) << 16 | (v * 3 / 4) << 8 | (v / 3);
        }
    }
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            static const int Styles[] = { PROMPTS_XBOX, PROMPTS_PLAYSTATION, PROMPTS_NINTENDO, PROMPTS_STEAMDECK };
            char buf[32];
            const char *name = row < 4 ? pad_picture(Styles[row], Codes[col]) : row == 4 ? key_picture(Keys[col], buf, sizeof(buf))
                             : col < 2 ? (col == 0 ? "mouse_left" : "mouse_right") : NULL;
            const Ready *r = name != NULL ? ready(name) : NULL;
            float cx, cy, side;

            if (r == NULL) {
                continue;
            }
            place(CONTROL_CROSS, 0, r->h, cw, ch, 32, 16, &cx, &cy, &side);
            shrink(r, art, cw, ch, cx, cy, side);
            for (y = 0; y < ch; y++) {
                for (x = 0; x < cw; x++) {
                    if (art[y * cw + x] >> 24) {
                        sheet[(gap + row * (ch + gap) + y) * W + gap + col * (cw + gap) + x] = art[y * cw + x];
                    }
                }
            }
        }
    }
    s = SDL_CreateSurfaceFrom(W, H, SDL_PIXELFORMAT_ABGR8888, sheet, W * 4);
    ok = s != NULL && SDL_SavePNG(s, path);
    SDL_DestroySurface(s);
    SDL_free(sheet);
    SDL_free(art);
    return ok;
}
