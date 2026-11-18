/* Button prompts (prompts.h; docs/PORT.md, Button prompts). Each picture is drawn here, by distance to its shapes: a button's disc or
 * a pill for the shoulders, a key's cap, a mouse, and their letters in a small stroke font of their own, at the size they're shown,
 * so they're sharp on any screen. The game's text is the font's, and a picture stands in the font's 32 x 16 texels of the original. */
#include "prompts.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "controls.h"
#include "settings.h"

static const char *const Names[NUM_PROMPTS] = { "AUTO", "ORIGINAL", "XBOX", "PLAYSTATION", "NINTENDO", "KEYBOARD" };
static const char *const Ids[NUM_PROMPTS] = { "auto", "original", "xbox", "playstation", "nintendo", "keyboard" };

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
   shapes. The mouse is a keyboard's player's, so it's keys, but with the mouse's own buttons for what it does (prompt_for) */
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

/* --- drawing: a picture's pixels as premultiplied colour and coverage, each shape laid over what's there --- */

typedef struct {
    float r, g, b;
} Colour;

typedef struct {
    int w, h;
    float *px; /* r, g, b, a premultiplied */
} Canvas;

static Colour rgb(uint32_t c) {
    Colour k = { (float)((c >> 16) & 0xFF) / 255.0f, (float)((c >> 8) & 0xFF) / 255.0f, (float)(c & 0xFF) / 255.0f };
    return k;
}

static void put(Canvas *cv, int x, int y, float cover, Colour c) {
    float *p = &cv->px[((size_t)y * cv->w + x) * 4];

    if (cover <= 0) {
        return;
    }
    if (cover > 1) {
        cover = 1;
    }
    p[0] = c.r * cover + p[0] * (1 - cover);
    p[1] = c.g * cover + p[1] * (1 - cover);
    p[2] = c.b * cover + p[2] * (1 - cover);
    p[3] = cover + p[3] * (1 - cover);
}

/* A rounded box, its middle cx, cy, half sizes hw, hh and corners' radius r, shaded from top to bottom; only between x0 and x1 */
static void fill_box_part(Canvas *cv, float cx, float cy, float hw, float hh, float r, uint32_t top, uint32_t bottom, float x0, float x1) {
    Colour a = rgb(top), b = rgb(bottom);
    int x, y;

    if (r > hw) {
        r = hw;
    }
    if (r > hh) {
        r = hh;
    }
    for (y = 0; y < cv->h; y++) {
        float py = y + 0.5f - cy, t = (py + hh) / (2 * hh);
        Colour c;

        t = t < 0 ? 0 : t > 1 ? 1 : t;
        c.r = a.r + (b.r - a.r) * t;
        c.g = a.g + (b.g - a.g) * t;
        c.b = a.b + (b.b - a.b) * t;
        for (x = 0; x < cv->w; x++) {
            float px = x + 0.5f - cx;
            float qx = fabsf(px) - (hw - r), qy = fabsf(py) - (hh - r);
            float out = sqrtf((qx > 0 ? qx * qx : 0) + (qy > 0 ? qy * qy : 0));
            float d = out + (qx > qy ? (qx < 0 ? qx : 0) : (qy < 0 ? qy : 0)) - r;
            float cover = 0.5f - d;

            if (x + 0.5f < x0 || x + 0.5f > x1) {
                continue;
            }
            put(cv, x, y, cover, c);
        }
    }
}

static void fill_box(Canvas *cv, float cx, float cy, float hw, float hh, float r, uint32_t top, uint32_t bottom) {
    fill_box_part(cv, cx, cy, hw, hh, r, top, bottom, -1e9f, 1e9f);
}

typedef struct {
    float x0, y0, x1, y1;
} Seg;

#define MAX_SEGS 160

static float seg_distance(const Seg *s, float px, float py) {
    float dx = s->x1 - s->x0, dy = s->y1 - s->y0, l = dx * dx + dy * dy;
    float t = l > 0 ? ((px - s->x0) * dx + (py - s->y0) * dy) / l : 0;
    float ex, ey;

    t = t < 0 ? 0 : t > 1 ? 1 : t;
    ex = px - (s->x0 + dx * t);
    ey = py - (s->y0 + dy * t);
    return sqrtf(ex * ex + ey * ey);
}

/* Strokes width wide, with round ends */
static void stroke(Canvas *cv, const Seg *segs, int n, float width, uint32_t colour) {
    Colour c = rgb(colour);
    int x, y, i;

    for (y = 0; y < cv->h; y++) {
        for (x = 0; x < cv->w; x++) {
            float best = 1e9f;

            for (i = 0; i < n; i++) {
                float d = seg_distance(&segs[i], x + 0.5f, y + 0.5f);

                if (d < best) {
                    best = d;
                }
            }
            put(cv, x, y, width / 2 - best + 0.5f, c);
        }
    }
}

static void stroke_ring(Canvas *cv, float cx, float cy, float r, float width, uint32_t colour) {
    Colour c = rgb(colour);
    int x, y;

    for (y = 0; y < cv->h; y++) {
        for (x = 0; x < cv->w; x++) {
            float dx = x + 0.5f - cx, dy = y + 0.5f - cy;

            put(cv, x, y, width / 2 - fabsf(sqrtf(dx * dx + dy * dy) - r) + 0.5f, c);
        }
    }
}

/* --- the stroke font: each character's lines on a grid 8 wide and 12 high (0-9, a, b, c), a point a pair, a comma between lines --- */

#define GLYPH_W 8
#define GLYPH_H 12
#define GLYPH_GAP 3
#define ARROW_UP '\1'
#define ARROW_DOWN '\2'
#define ARROW_LEFT '\3'
#define ARROW_RIGHT '\4'

static const char *glyph_lines(int c) {
    switch (c) {
    case 'A': return "0c 40 8c,18 78";
    case 'B': return "0c 00 50 72 74 56 06,56 78 7a 5c 0c";
    case 'C': return "82 60 20 02 0a 2c 6c 8a";
    case 'D': return "00 50 82 8a 5c 0c 00";
    case 'E': return "80 00 0c 8c,06 56";
    case 'F': return "80 00 0c,06 56";
    case 'G': return "82 60 20 02 0a 2c 6c 8a 87 57";
    case 'H': return "00 0c,80 8c,06 86";
    case 'I': return "20 60,40 4c,2c 6c";
    case 'J': return "20 70,60 6a 4c 2c 0a";
    case 'K': return "00 0c,80 08,35 8c";
    case 'L': return "00 0c 8c";
    case 'M': return "0c 00 46 80 8c";
    case 'N': return "0c 00 8c 80";
    case 'O': return "20 60 82 8a 6c 2c 0a 02 20";
    case 'P': return "0c 00 60 82 84 66 06";
    case 'Q': return "20 60 82 8a 6c 2c 0a 02 20,59 8c";
    case 'R': return "0c 00 60 82 84 66 06,46 8c";
    case 'S': return "82 60 20 02 04 26 66 88 8a 6c 2c 0a";
    case 'T': return "00 80,40 4c";
    case 'U': return "00 0a 2c 6c 8a 80";
    case 'V': return "00 4c 80";
    case 'W': return "00 2c 46 6c 80";
    case 'X': return "00 8c,80 0c";
    case 'Y': return "00 46 80,46 4c";
    case 'Z': return "00 80 0c 8c";
    case '0': return "20 60 82 8a 6c 2c 0a 02 20";
    case '1': return "22 40 4c,2c 6c";
    case '2': return "02 20 60 82 84 0c 8c";
    case '3': return "02 20 60 82 84 66 36,66 88 8a 6c 2c 0a";
    case '4': return "60 08 88,60 6c";
    case '5': return "80 00 05 65 87 8a 6c 2c 0a";
    case '6': return "72 50 20 02 0a 2c 6c 8a 88 66 26 08";
    case '7': return "00 80 3c";
    case '8': return "20 60 82 84 66 26 04 02 20,26 08 0a 2c 6c 8a 88 66";
    case '9': return "84 66 26 04 02 20 60 82 8a 6c 2c 0a";
    case '+': return "16 76,43 49";
    case '-': return "16 76";
    case '=': return "14 74,18 78";
    case '.': return "4b 4c";
    case ',': return "4a 4b 2c";
    case ';': return "44 45,4a 4b 2c";
    case '\'': return "40 43";
    case '`': return "20 43";
    case '/': return "70 1c";
    case '\\': return "10 7c";
    case '[': return "60 20 2c 6c";
    case ']': return "20 60 6c 2c";
    case '#': return "30 1c,70 5c,04 84,08 88";
    case ARROW_UP: return "4b 41,14 41 74";
    case ARROW_DOWN: return "41 4b,18 4b 78";
    case ARROW_LEFT: return "16 76,43 16 49";
    case ARROW_RIGHT: return "16 76,43 76 49";
    default: return NULL;
    }
}

static int hex_digit(char c) {
    return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'c' ? c - 'a' + 10 : -1;
}

static int text_units(const char *s) {
    int n = (int)strlen(s);

    return n > 0 ? n * GLYPH_W + (n - 1) * GLYPH_GAP : 0;
}

/* The text in the box (its middle cx, cy, half sizes hw, hh), as tall as the box and squeezed to its width if it's longer: one stroke's
   width is weight times the height's twelfth */
static void draw_text(Canvas *cv, const char *s, float cx, float cy, float hw, float hh, float weight, uint32_t colour) {
    Seg segs[MAX_SEGS];
    int n = 0, units = text_units(s), i;
    float sy = 2 * hh / GLYPH_H, sx = sy, width = weight * sy, x0, y0;

    if (units <= 0) {
        return;
    }
    if (units * sx > 2 * hw) {
        sx = 2 * hw / units;
    }
    /* the strokes' round ends reach past the grid by half a width */
    x0 = cx - units * sx / 2;
    y0 = cy - hh;
    for (i = 0; s[i] != '\0'; i++) {
        const char *p = glyph_lines((unsigned char)s[i]);
        float ox = x0 + i * (GLYPH_W + GLYPH_GAP) * sx, lx = 0, ly = 0;
        int have = 0;

        while (p != NULL && *p != '\0' && n < MAX_SEGS) {
            if (*p == ',') {
                have = 0;
                p++;
            } else if (*p == ' ') {
                p++;
            } else if (hex_digit(p[0]) >= 0 && hex_digit(p[1]) >= 0) {
                float x = ox + hex_digit(p[0]) * sx, y = y0 + hex_digit(p[1]) * sy;

                if (have) {
                    segs[n].x0 = lx;
                    segs[n].y0 = ly;
                    segs[n].x1 = x;
                    segs[n].y1 = y;
                    n++;
                }
                lx = x;
                ly = y;
                have = 1;
                p += 2;
            } else {
                p++;
            }
        }
    }
    stroke(cv, segs, n, width, colour);
}

/* --- the pictures --- */

/* What a picture's of */
enum { ART_PAD, ART_KEY, ART_MOUSE };
typedef struct {
    int kind;   /* ART_ */
    int style;  /* PROMPTS_XBOX and so on, for a controller's */
    int code;   /* SDL's button, BUTTON_LEFT_TRIGGER, a scancode, or the mouse's SDL_BUTTON_LEFT or _RIGHT */
} Prompt;

#define INK_DARK 0x101012
#define BODY_TOP 0x5E5E66
#define BODY_BOTTOM 0x26262B

/* A controller's round button: a dark disc with an outline, as the original's are */
static void draw_disc(Canvas *cv, float cx, float cy, float r) {
    fill_box(cv, cx, cy, r, r, r, INK_DARK, INK_DARK);
    r -= r * 0.11f > 1.2f ? r * 0.11f : 1.2f;
    fill_box(cv, cx, cy, r, r, r, BODY_TOP, BODY_BOTTOM);
}

static void draw_pill(Canvas *cv, float cx, float cy, float hw, float hh, float round, const char *label) {
    float edge = hh * 0.16f > 1.2f ? hh * 0.16f : 1.2f;

    fill_box(cv, cx, cy, hw, hh, hh * round, INK_DARK, INK_DARK);
    fill_box(cv, cx, cy, hw - edge, hh - edge, (hh - edge) * round, BODY_TOP, BODY_BOTTOM);
    draw_text(cv, label, cx, cy, hw - hh * 0.55f, hh * 0.5f, 2.2f, 0xF2F2F2);
}

/* A face button: by its place, as SDL names them, and what that place's button is on that kind of controller */
static int draw_face(Canvas *cv, const Prompt *p, float cx, float cy, float r) {
    static const char Xbox[4] = { 'A', 'B', 'X', 'Y' }, Nintendo[4] = { 'B', 'A', 'Y', 'X' };
    static const uint32_t XboxInk[4] = { 0x6FCF3A, 0xF0444A, 0x3E9BF5, 0xF5C73A };
    int place;
    char letter[2] = { 0, 0 };

    switch (p->code) {
    case SDL_GAMEPAD_BUTTON_SOUTH: place = 0; break;
    case SDL_GAMEPAD_BUTTON_EAST: place = 1; break;
    case SDL_GAMEPAD_BUTTON_WEST: place = 2; break;
    case SDL_GAMEPAD_BUTTON_NORTH: place = 3; break;
    default: return 0;
    }
    draw_disc(cv, cx, cy, r);
    if (p->style == PROMPTS_PLAYSTATION) {
        /* the shapes in the PlayStation's own colours */
        float s = r * 0.42f, w = r * 0.17f;
        Seg g[4];

        switch (place) {
        case 0: /* Cross */
            g[0].x0 = cx - s; g[0].y0 = cy - s; g[0].x1 = cx + s; g[0].y1 = cy + s;
            g[1].x0 = cx + s; g[1].y0 = cy - s; g[1].x1 = cx - s; g[1].y1 = cy + s;
            stroke(cv, g, 2, w, 0x8CB4F5);
            break;
        case 1: /* Circle */
            stroke_ring(cv, cx, cy, s * 1.02f, w, 0xF5636E);
            break;
        case 2: /* Square */
            s *= 0.86f;
            g[0].x0 = cx - s; g[0].y0 = cy - s; g[0].x1 = cx + s; g[0].y1 = cy - s;
            g[1].x0 = cx + s; g[1].y0 = cy - s; g[1].x1 = cx + s; g[1].y1 = cy + s;
            g[2].x0 = cx + s; g[2].y0 = cy + s; g[2].x1 = cx - s; g[2].y1 = cy + s;
            g[3].x0 = cx - s; g[3].y0 = cy + s; g[3].x1 = cx - s; g[3].y1 = cy - s;
            stroke(cv, g, 4, w, 0xEE93DC);
            break;
        default: /* Triangle, its middle a little low, as its weight is */
            g[0].x0 = cx; g[0].y0 = cy - s * 1.05f; g[0].x1 = cx + s * 1.1f; g[0].y1 = cy + s * 0.75f;
            g[1].x0 = g[0].x1; g[1].y0 = g[0].y1; g[1].x1 = cx - s * 1.1f; g[1].y1 = g[0].y1;
            g[2].x0 = g[1].x1; g[2].y0 = g[1].y1; g[2].x1 = g[0].x0; g[2].y1 = g[0].y0;
            stroke(cv, g, 3, w, 0x45D2A8);
            break;
        }
        return 1;
    }
    letter[0] = p->style == PROMPTS_NINTENDO ? Nintendo[place] : Xbox[place];
    draw_text(cv, letter, cx, cy, r * 0.6f, r * 0.46f, 2.4f, p->style == PROMPTS_NINTENDO ? 0xF2F2F2 : XboxInk[place]);
    return 1;
}

/* The other buttons' names, as each kind of controller has them */
static const char *pad_name(int style, int code) {
    int xbox = style == PROMPTS_XBOX, ps = style == PROMPTS_PLAYSTATION;

    switch (code) {
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return xbox ? "LB" : ps ? "L1" : "L";
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return xbox ? "RB" : ps ? "R1" : "R";
    case BUTTON_LEFT_TRIGGER: return xbox ? "LT" : ps ? "L2" : "ZL";
    case BUTTON_RIGHT_TRIGGER: return xbox ? "RT" : ps ? "R2" : "ZR";
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return ps ? "L3" : "LS";
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return ps ? "R3" : "RS";
    case SDL_GAMEPAD_BUTTON_START: return xbox ? "MENU" : ps ? "OPT" : "+";
    case SDL_GAMEPAD_BUTTON_BACK: return xbox ? "VIEW" : ps ? "SHARE" : "-";
    case SDL_GAMEPAD_BUTTON_GUIDE: return "HOME";
    case SDL_GAMEPAD_BUTTON_TOUCHPAD: return "PAD";
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return "P1";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return "P2";
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return "P3";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return "P4";
    default: return "?";
    }
}

/* The d-pad, with the arm that's meant lit */
static void draw_dpad(Canvas *cv, int code, float cx, float cy, float r) {
    float arm = r * 0.36f, e = r * 0.12f > 1.2f ? r * 0.12f : 1.2f;
    float ax = code == SDL_GAMEPAD_BUTTON_DPAD_LEFT ? -1.f : code == SDL_GAMEPAD_BUTTON_DPAD_RIGHT ? 1.f : 0.f;
    float ay = code == SDL_GAMEPAD_BUTTON_DPAD_UP ? -1.f : code == SDL_GAMEPAD_BUTTON_DPAD_DOWN ? 1.f : 0.f;

    fill_box(cv, cx, cy, r, arm, arm * 0.3f, INK_DARK, INK_DARK);
    fill_box(cv, cx, cy, arm, r, arm * 0.3f, INK_DARK, INK_DARK);
    fill_box(cv, cx, cy, r - e, arm - e, arm * 0.2f, BODY_TOP, BODY_BOTTOM);
    fill_box(cv, cx, cy, arm - e, r - e, arm * 0.2f, BODY_TOP, BODY_BOTTOM);
    /* the arm lit from its end to the middle's square */
    {
        float half = (r - e - arm * 0.6f) / 2, mid = arm * 0.6f + half;

        fill_box(cv, cx + ax * mid, cy + ay * mid, ax != 0 ? half : arm - e, ay != 0 ? half : arm - e, arm * 0.2f, 0xFFFFFF, 0xD0D0D6);
    }
}

/* A key's name on its cap, short: the arrows as arrows */
static void key_name(int key, char *out, size_t size) {
    const char *name;
    size_t i, n = 0;

    switch (key) {
    case SDL_SCANCODE_UP: SDL_snprintf(out, size, "%c", ARROW_UP); return;
    case SDL_SCANCODE_DOWN: SDL_snprintf(out, size, "%c", ARROW_DOWN); return;
    case SDL_SCANCODE_LEFT: SDL_snprintf(out, size, "%c", ARROW_LEFT); return;
    case SDL_SCANCODE_RIGHT: SDL_snprintf(out, size, "%c", ARROW_RIGHT); return;
    case SDL_SCANCODE_RETURN:
    case SDL_SCANCODE_RETURN2:
    case SDL_SCANCODE_KP_ENTER: SDL_snprintf(out, size, "ENTER"); return;
    case SDL_SCANCODE_ESCAPE: SDL_snprintf(out, size, "ESC"); return;
    case SDL_SCANCODE_BACKSPACE: SDL_snprintf(out, size, "BKSP"); return;
    case SDL_SCANCODE_DELETE: SDL_snprintf(out, size, "DEL"); return;
    case SDL_SCANCODE_INSERT: SDL_snprintf(out, size, "INS"); return;
    case SDL_SCANCODE_CAPSLOCK: SDL_snprintf(out, size, "CAPS"); return;
    case SDL_SCANCODE_LSHIFT:
    case SDL_SCANCODE_RSHIFT: SDL_snprintf(out, size, "SHIFT"); return;
    case SDL_SCANCODE_LCTRL:
    case SDL_SCANCODE_RCTRL: SDL_snprintf(out, size, "CTRL"); return;
    case SDL_SCANCODE_LALT:
    case SDL_SCANCODE_RALT: SDL_snprintf(out, size, "ALT"); return;
    case SDL_SCANCODE_PAGEUP: SDL_snprintf(out, size, "PGUP"); return;
    case SDL_SCANCODE_PAGEDOWN: SDL_snprintf(out, size, "PGDN"); return;
    case SDL_SCANCODE_NONUSBACKSLASH: SDL_snprintf(out, size, "\\"); return;
    case SDL_SCANCODE_NONUSHASH: SDL_snprintf(out, size, "#"); return;
    default: break;
    }
    name = SDL_GetScancodeName((SDL_Scancode)key);
    if (name == NULL) {
        name = "";
    }
    if (SDL_strncmp(name, "Keypad ", 7) == 0) {
        name += 7;
    }
    /* what the stroke font has of the name, in capitals, five at most */
    for (i = 0; name[i] != '\0' && n + 1 < size && n < 5; i++) {
        int c = SDL_toupper((unsigned char)name[i]);

        if (glyph_lines(c) != NULL) {
            out[n++] = (char)c;
        }
    }
    out[n] = '\0';
    if (n == 0) {
        SDL_snprintf(out, size, "?");
    }
}

static void draw_key(Canvas *cv, int key, float cx, float cy, float hh, float most_hw) {
    char name[16];
    float label_h = hh * 0.5f, hw;
    float edge = hh * 0.12f > 1.2f ? hh * 0.12f : 1.2f, lip = hh * 0.18f;

    key_name(key, name, sizeof(name));
    /* one letter's cap is square; a word's as wide as the word, up to the room there is */
    hw = text_units(name) * (2 * label_h / GLYPH_H) / 2 + hh * 0.55f;
    hw = hw < hh ? hh : hw > most_hw ? most_hw : hw;
    fill_box(cv, cx, cy, hw, hh, hh * 0.35f, INK_DARK, INK_DARK);
    fill_box(cv, cx, cy, hw - edge, hh - edge, hh * 0.28f, 0xB4B4BA, 0x8A8A92);
    fill_box(cv, cx, cy - lip / 2, hw - edge * 1.8f, hh - edge * 1.8f - lip / 2, hh * 0.24f, 0xFAFAFA, 0xD8D8DE);
    draw_text(cv, name, cx, cy - lip / 2, hw - edge * 1.8f - hh * 0.2f, label_h, 2.3f, 0x1C1C22);
}

/* A mouse, the button meant lit */
static void draw_mouse(Canvas *cv, int button, float cx, float cy, float hh) {
    float hw = hh * 0.68f, edge = hh * 0.1f > 1.2f ? hh * 0.1f : 1.2f, split = cy - hh * 0.12f;
    float inner_hw = hw - edge, inner_hh = hh - edge;
    Seg g[2];

    fill_box(cv, cx, cy, hw, hh, hw * 0.95f, INK_DARK, INK_DARK);
    fill_box(cv, cx, cy, inner_hw, inner_hh, inner_hw * 0.95f, 0xF4F4F4, 0xC4C4CC);
    /* the lit button: the top of the body, on its side of the middle, above the split */
    {
        Canvas top = *cv;
        float x0 = button == SDL_BUTTON_LEFT ? -1e9f : cx, x1 = button == SDL_BUTTON_LEFT ? cx : 1e9f;
        int y;

        /* only rows above the split: a canvas that's only those rows */
        y = (int)split;
        top.h = y > 0 ? (y < cv->h ? y : cv->h) : 0;
        if (top.h > 0) {
            fill_box_part(&top, cx, cy, inner_hw, inner_hh, inner_hw * 0.95f, 0xFFB43C, 0xF08A1E, x0, x1);
        }
    }
    g[0].x0 = cx; g[0].y0 = cy - inner_hh; g[0].x1 = cx; g[0].y1 = split;
    g[1].x0 = cx - inner_hw; g[1].y0 = split; g[1].x1 = cx + inner_hw; g[1].y1 = split;
    stroke(cv, g, 2, edge, INK_DARK);
}

/* --- what each of the game's face buttons is drawn as now --- */

/* A control's prompt in a style, or 0 for the game's own */
static int prompt_for(int control, int style, Prompt *p) {
    const Settings *s = settings();

    memset(p, 0, sizeof(*p));
    if (style == PROMPTS_ORIGINAL || control < 0 || control >= NUM_CONTROLS) {
        return 0;
    }
    if (style == PROMPTS_KEYBOARD) {
        /* clicking is Cross in the menus, and a right click Triangle (menumouse.c) */
        if (!KeysOnly && settings()->button_prompts == PROMPTS_AUTO && host_prompt_device() == PROMPT_DEVICE_MOUSE &&
            (control == CONTROL_CROSS || control == CONTROL_TRIANGLE)) {
            p->kind = ART_MOUSE;
            p->code = control == CONTROL_CROSS ? SDL_BUTTON_LEFT : SDL_BUTTON_RIGHT;
            return 1;
        }
        p->kind = ART_KEY;
        p->code = s->keys[control];
        return 1;
    }
    p->kind = ART_PAD;
    p->style = style;
    p->code = s->buttons[control];
    return 1;
}

/* The picture in a canvas the size of the font's texels as shown: cx, cy the original's middle, r the height of its disc's half */
static void draw_prompt(Canvas *cv, const Prompt *p, float cx, float cy, float r, float most_hw) {
    switch (p->kind) {
    case ART_KEY:
        draw_key(cv, p->code, cx, cy, r * 0.92f, most_hw);
        return;
    case ART_MOUSE:
        draw_mouse(cv, p->code, cx, cy, r);
        return;
    default:
        break;
    }
    if (draw_face(cv, p, cx, cy, r)) {
        return;
    }
    if (p->code == SDL_GAMEPAD_BUTTON_DPAD_UP || p->code == SDL_GAMEPAD_BUTTON_DPAD_DOWN || p->code == SDL_GAMEPAD_BUTTON_DPAD_LEFT ||
        p->code == SDL_GAMEPAD_BUTTON_DPAD_RIGHT) {
        draw_dpad(cv, p->code, cx, cy, r);
        return;
    }
    {
        const char *name = pad_name(p->style, p->code);
        float hh = r * 0.8f, hw = text_units(name) * (hh / GLYPH_H) / 2 + hh * 0.75f;

        hw = hw < hh ? hh : hw > most_hw ? most_hw : hw;
        /* Nintendo's + and - are round buttons; the rest are pills, the triggers' squarer */
        draw_pill(cv, cx, cy, hw, hh, p->code == BUTTON_LEFT_TRIGGER || p->code == BUTTON_RIGHT_TRIGGER ? 0.45f : 1.0f, name);
    }
}

/* A canvas's pixels as the atlas has them: colour unpremultiplied, and alpha clear or opaque, which is all the drawing knows */
static void finish(const Canvas *cv, uint32_t *out) {
    size_t i, n = (size_t)cv->w * cv->h;

    for (i = 0; i < n; i++) {
        const float *p = &cv->px[i * 4];

        if (p[3] < 0.5f) {
            out[i] = 0;
        } else {
            uint32_t r = (uint32_t)(p[0] / p[3] * 255 + 0.5f), g = (uint32_t)(p[1] / p[3] * 255 + 0.5f), b = (uint32_t)(p[2] / p[3] * 255 + 0.5f);

            r = r > 255 ? 255 : r;
            g = g > 255 ? 255 : g;
            b = b > 255 ? 255 : b;
            out[i] = 0xFF000000u | b << 16 | g << 8 | r;
        }
    }
}

/* Where in the font's texels the original has its disc: 30 of its 32 across and 15 of its 16 down, from the top left */
static void place(int art_w, int art_h, int w, int h, float *cx, float *cy, float *r, float *most_hw) {
    float tx = (float)art_w / w, ty = (float)art_h / h;

    *cx = 15.0f * tx;
    *cy = 7.5f * ty;
    *r = 7.3f * ty;
    *most_hw = 14.8f * tx;
}

/* Kept pictures: one for each of the four, made again when what it shows or its size changes */
typedef struct {
    Prompt p;
    int w, h, art_w, art_h;
    uint32_t *art;
} Kept;
static Kept Pictures[4];

const uint32_t *prompts_art(int control, int w, int h, int target_h, int *art_w, int *art_h, uint64_t *id) {
    Prompt p;
    Kept *k;
    int aw, ah;

    if (control < CONTROL_CROSS || control > CONTROL_TRIANGLE || w <= 0 || h <= 0 || target_h <= 0 ||
        !prompt_for(control, prompts_showing(), &p)) {
        return NULL;
    }
    /* the game's 240 rows to the picture's height, and its texels 5/8 as wide as tall (a 512 x 240 screen shown at 4:3) */
    ah = (int)((float)h * target_h / 240 + 0.5f);
    aw = (int)((float)w * target_h / 384 + 0.5f);
    ah = ah < h ? h : ah > 512 ? 512 : ah;
    aw = aw < w ? w : aw > 640 ? 640 : aw;
    k = &Pictures[control - CONTROL_CROSS];
    if (k->art == NULL || memcmp(&k->p, &p, sizeof(p)) != 0 || k->w != w || k->h != h || k->art_w != aw || k->art_h != ah) {
        Canvas cv;
        float cx, cy, r, most;

        cv.w = aw;
        cv.h = ah;
        cv.px = SDL_calloc((size_t)aw * ah * 4, sizeof(float));
        SDL_free(k->art);
        k->art = SDL_malloc((size_t)aw * ah * sizeof(uint32_t));
        if (cv.px == NULL || k->art == NULL) {
            SDL_free(cv.px);
            SDL_free(k->art);
            k->art = NULL;
            return NULL;
        }
        place(aw, ah, w, h, &cx, &cy, &r, &most);
        draw_prompt(&cv, &p, cx, cy, r, most);
        finish(&cv, k->art);
        SDL_free(cv.px);
        k->p = p;
        k->w = w;
        k->h = h;
        k->art_w = aw;
        k->art_h = ah;
    }
    *art_w = aw;
    *art_h = ah;
    *id = 0x50524F4D50000000ULL ^ ((uint64_t)control << 56) ^ ((uint64_t)p.kind << 48) ^ ((uint64_t)p.style << 44) ^
          ((uint64_t)(p.code & 0xFFF) << 32) ^ ((uint64_t)aw << 16) ^ (uint64_t)ah;
    return k->art;
}

/* --- a sheet of them all, for looking at --- */

int prompts_sheet(const char *path) {
    static const int Codes[] = { SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_NORTH,
                                 SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, BUTTON_RIGHT_TRIGGER, SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_DPAD_UP };
    static const int Keys[] = { SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_RETURN, SDL_SCANCODE_RSHIFT,
                                SDL_SCANCODE_UP, SDL_SCANCODE_SPACE };
    const int cols = 8, rows = 5, cw = 160, ch = 128, gap = 8;
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
            /* the launcher's blue-green, so a picture's edge shows as it would */
            sheet[y * W + x] = 0xFF000000u | (uint32_t)(0x50 + y * 0x40 / H) << 16 | (uint32_t)(0x48 + x * 0x30 / W) << 8 | 0x10;
        }
    }
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            Prompt p;
            Canvas cv;
            float cx, cy, r, most;

            memset(&p, 0, sizeof(p));
            if (row < 3) {
                p.kind = ART_PAD;
                p.style = row == 0 ? PROMPTS_XBOX : row == 1 ? PROMPTS_PLAYSTATION : PROMPTS_NINTENDO;
                p.code = Codes[col];
            } else if (row == 3) {
                p.kind = ART_KEY;
                p.code = Keys[col];
            } else if (col < 2) {
                p.kind = ART_MOUSE;
                p.code = col == 0 ? SDL_BUTTON_LEFT : SDL_BUTTON_RIGHT;
            } else {
                continue;
            }
            cv.w = cw;
            cv.h = ch;
            cv.px = SDL_calloc((size_t)cw * ch * 4, sizeof(float));
            if (cv.px == NULL) {
                continue;
            }
            place(cw, ch, 32, 16, &cx, &cy, &r, &most);
            draw_prompt(&cv, &p, cx, cy, r, most);
            finish(&cv, art);
            SDL_free(cv.px);
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
