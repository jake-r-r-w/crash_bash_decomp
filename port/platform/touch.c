/* On-screen buttons for a touch screen (touch.h): a d-pad on the left, the four face buttons on the right, the shoulder buttons in the
 * top corners and Select and Start at the bottom, sized by the screen's shorter side and kept clear of a notch (the window's safe area).
 * Each finger holds what it's on, and slides: round the d-pad, or from one face button to the next. */
#include <math.h>
#include <SDL3/SDL.h>
#include "platform.h"
#include "touch.h"

#define MAX_FINGERS 10

typedef struct {
    SDL_FingerID id;
    int down;
    float x, y; /* 0 to 1 across the window */
} Finger;

static Finger Fingers[MAX_FINGERS];
/* -1: as the system starts them (shown on Android, where a touch screen is how most play) */
static int Shown = -1;

/* Where everything is, in the window's pixels */
typedef struct {
    float u;                  /* the shorter side */
    float pad_x, pad_y;       /* the d-pad's centre */
    float face_x, face_y;     /* the face buttons' centre */
    float button_r, spacing;  /* a face button's size, and how far each is from that centre */
    float shoulder[4][4];     /* L1, L2, R1, R2: x0, y0, x1, y1 */
    float small[2][4];        /* Select, Start */
} Layout;

static const int ShoulderBits[4] = { PAD_L1, PAD_L2, PAD_R1, PAD_R2 };
static const int SmallBits[2] = { PAD_SELECT, PAD_START };
/* Triangle, Circle, Cross, Square: up, right, down, left of the centre */
static const int FaceBits[4] = { PAD_TRIANGLE, PAD_CIRCLE, PAD_CROSS, PAD_SQUARE };
static const float FaceDir[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

static int shown(void) {
    if (Shown < 0) {
#ifdef SDL_PLATFORM_ANDROID
        Shown = 1;
#else
        Shown = 0;
#endif
    }
    return Shown;
}

static void layout(int w, int h, Layout *l) {
    SDL_Window *win = host_window();
    float left = 0, top = 0, right = (float)w, bottom = (float)h;
    SDL_Rect safe;
    int ww = 0, wh = 0;
    float u = (float)(w < h ? w : h), m;
    int i;

    /* the safe area is in the window's own units, which may not be its pixels */
    if (win != NULL && SDL_GetWindowSafeArea(win, &safe) && SDL_GetWindowSize(win, &ww, &wh) && ww > 0 && wh > 0) {
        float sx = (float)w / ww, sy = (float)h / wh;

        left = safe.x * sx;
        top = safe.y * sy;
        right = (safe.x + safe.w) * sx;
        bottom = (safe.y + safe.h) * sy;
    }
    m = 0.03f * u;
    l->u = u;
    l->pad_x = left + m + 0.19f * u;
    l->pad_y = bottom - m - 0.21f * u;
    l->face_x = right - m - 0.19f * u;
    l->face_y = l->pad_y;
    l->button_r = 0.062f * u;
    l->spacing = 0.11f * u;
    for (i = 0; i < 4; i++) {
        float w_ = 0.22f * u, h_ = 0.085f * u, y0 = top + m + (i & 1) * (h_ + 0.03f * u);
        float x0 = i < 2 ? left + m : right - m - w_;

        l->shoulder[i][0] = x0;
        l->shoulder[i][1] = y0;
        l->shoulder[i][2] = x0 + w_;
        l->shoulder[i][3] = y0 + h_;
    }
    /* Select and Start: on a screen held sideways, at the sides under L2 and R2, out of the picture; held upright, at the bottom
       between the d-pad and the face buttons, below it */
    for (i = 0; i < 2; i++) {
        float w_ = 0.13f * u, h_ = 0.06f * u, cx, y0;

        if (w > h) {
            cx = (l->shoulder[i * 2 + 1][0] + l->shoulder[i * 2 + 1][2]) / 2;
            y0 = l->shoulder[i * 2 + 1][3] + 0.05f * u;
        } else {
            cx = w / 2.0f + (i == 0 ? -1 : 1) * 0.1f * u;
            y0 = bottom - m - h_;
        }
        l->small[i][0] = cx - w_ / 2;
        l->small[i][1] = y0;
        l->small[i][2] = cx + w_ / 2;
        l->small[i][3] = y0 + h_;
    }
}

static int in_rect(const float r[4], float x, float y, float slack) {
    return x >= r[0] - slack && x <= r[2] + slack && y >= r[1] - slack && y <= r[3] + slack;
}

/* What a finger at x, y (pixels) presses */
static int pressed_at(const Layout *l, float x, float y) {
    float dx = x - l->pad_x, dy = y - l->pad_y, d = sqrtf(dx * dx + dy * dy);
    int i, best = -1;
    float best_d = 0;

    if (d < 0.30f * l->u) {
        int b = 0;

        if (d < 0.035f * l->u) {
            return 0;
        }
        /* eight ways: a diagonal is a wedge of its own between two arms */
        if (dx / d > 0.38f) {
            b |= PAD_RIGHT;
        } else if (dx / d < -0.38f) {
            b |= PAD_LEFT;
        }
        if (dy / d > 0.38f) {
            b |= PAD_DOWN;
        } else if (dy / d < -0.38f) {
            b |= PAD_UP;
        }
        return b;
    }
    for (i = 0; i < 4; i++) {
        float bx = l->face_x + FaceDir[i][0] * l->spacing - x, by = l->face_y + FaceDir[i][1] * l->spacing - y;
        float bd = sqrtf(bx * bx + by * by);

        if (bd < l->button_r * 1.7f && (best < 0 || bd < best_d)) {
            best = i;
            best_d = bd;
        }
    }
    if (best >= 0) {
        return FaceBits[best];
    }
    for (i = 0; i < 4; i++) {
        if (in_rect(l->shoulder[i], x, y, 0.02f * l->u)) {
            return ShoulderBits[i];
        }
    }
    for (i = 0; i < 2; i++) {
        if (in_rect(l->small[i], x, y, 0.02f * l->u)) {
            return SmallBits[i];
        }
    }
    return 0;
}

static int held(const Layout *l, int w, int h) {
    int b = 0, i;

    for (i = 0; i < MAX_FINGERS; i++) {
        if (Fingers[i].down) {
            b |= pressed_at(l, Fingers[i].x * w, Fingers[i].y * h);
        }
    }
    return b;
}

void touch_event(const SDL_Event *e) {
    int i, free_slot = -1;

    if (e->type != SDL_EVENT_FINGER_DOWN && e->type != SDL_EVENT_FINGER_MOTION && e->type != SDL_EVENT_FINGER_UP &&
        e->type != SDL_EVENT_FINGER_CANCELED) {
        return;
    }
    for (i = 0; i < MAX_FINGERS; i++) {
        if (Fingers[i].down && Fingers[i].id == e->tfinger.fingerID) {
            break;
        }
        if (!Fingers[i].down && free_slot < 0) {
            free_slot = i;
        }
    }
    if (e->type == SDL_EVENT_FINGER_UP || e->type == SDL_EVENT_FINGER_CANCELED) {
        if (i < MAX_FINGERS) {
            Fingers[i].down = 0;
        }
        return;
    }
    if (i == MAX_FINGERS) {
        if (e->type != SDL_EVENT_FINGER_DOWN || free_slot < 0) {
            return;
        }
        i = free_slot;
        Fingers[i].id = e->tfinger.fingerID;
        Fingers[i].down = 1;
    }
    Fingers[i].x = e->tfinger.x;
    Fingers[i].y = e->tfinger.y;
    Shown = 1;
}

void touch_other_input(void) {
    shown();
    Shown = 0;
}

int touch_buttons(void) {
    SDL_Window *win = host_window();
    int w = 0, h = 0, i, any = 0;
    Layout l;

    for (i = 0; i < MAX_FINGERS; i++) {
        any |= Fingers[i].down;
    }
    if (!any || win == NULL || !SDL_GetWindowSizeInPixels(win, &w, &h) || w <= 0 || h <= 0) {
        return 0;
    }
    layout(w, h, &l);
    return held(&l, w, h);
}

/* --- the shapes --- */

typedef struct {
    TouchVert *v;
    int n, max;
} Out;

static void tri(Out *o, float x0, float y0, float x1, float y1, float x2, float y2, uint32_t rgb) {
    if (o->n + 3 > o->max) {
        return;
    }
    o->v[o->n].x = x0;
    o->v[o->n].y = y0;
    o->v[o->n++].rgb = rgb;
    o->v[o->n].x = x1;
    o->v[o->n].y = y1;
    o->v[o->n++].rgb = rgb;
    o->v[o->n].x = x2;
    o->v[o->n].y = y2;
    o->v[o->n++].rgb = rgb;
}

static void rect(Out *o, float x0, float y0, float x1, float y1, uint32_t rgb) {
    tri(o, x0, y0, x1, y0, x1, y1, rgb);
    tri(o, x0, y0, x1, y1, x0, y1, rgb);
}

static void disc(Out *o, float cx, float cy, float r, uint32_t rgb) {
    int i, n = 28;

    for (i = 0; i < n; i++) {
        float a0 = 6.2831853f * i / n, a1 = 6.2831853f * (i + 1) / n;

        tri(o, cx, cy, cx + r * cosf(a0), cy + r * sinf(a0), cx + r * cosf(a1), cy + r * sinf(a1), rgb);
    }
}

/* A stroke t thick from one point to another, run on by t at each end so strokes meet without gaps */
static void line(Out *o, float x0, float y0, float x1, float y1, float t, uint32_t rgb) {
    float dx = x1 - x0, dy = y1 - y0, len = sqrtf(dx * dx + dy * dy), nx, ny;

    if (len <= 0) {
        return;
    }
    dx /= len;
    dy /= len;
    x0 -= dx * t / 2;
    y0 -= dy * t / 2;
    x1 += dx * t / 2;
    y1 += dy * t / 2;
    nx = -dy * t / 2;
    ny = dx * t / 2;
    tri(o, x0 + nx, y0 + ny, x1 + nx, y1 + ny, x1 - nx, y1 - ny, rgb);
    tri(o, x0 + nx, y0 + ny, x1 - nx, y1 - ny, x0 - nx, y0 - ny, rgb);
}

static void ring(Out *o, float cx, float cy, float r, float t, uint32_t rgb) {
    int i, n = 28;

    for (i = 0; i < n; i++) {
        float a0 = 6.2831853f * i / n, a1 = 6.2831853f * (i + 1) / n;

        line(o, cx + r * cosf(a0), cy + r * sinf(a0), cx + r * cosf(a1), cy + r * sinf(a1), t, rgb);
    }
}

/* Letters as strokes in a box from 0, 0 (top left) to 1, 1: pairs of points, each pair a stroke */
static const float GlyphL[] = { 0, 0, 0, 1, 0, 1, 0.75f, 1 };
static const float GlyphR[] = { 0, 1, 0, 0, 0, 0, 0.6f, 0, 0.6f, 0, 0.8f, 0.15f, 0.8f, 0.15f, 0.8f, 0.35f, 0.8f, 0.35f, 0.6f, 0.5f,
                                0.6f, 0.5f, 0, 0.5f, 0.35f, 0.5f, 0.8f, 1 };
static const float Glyph1[] = { 0.15f, 0.25f, 0.5f, 0, 0.5f, 0, 0.5f, 1 };
static const float Glyph2[] = { 0, 0.2f, 0.15f, 0, 0.15f, 0, 0.6f, 0, 0.6f, 0, 0.8f, 0.2f, 0.8f, 0.2f, 0.8f, 0.4f, 0.8f, 0.4f, 0, 1, 0, 1,
                                0.85f, 1 };

static void glyph(Out *o, const float *pts, int count, float x, float y, float w, float h, float t, uint32_t rgb) {
    int i;

    for (i = 0; i + 3 < count; i += 4) {
        line(o, x + pts[i] * w, y + pts[i + 1] * h, x + pts[i + 2] * w, y + pts[i + 3] * h, t, rgb);
    }
}

#define BODY 0x202020u
#define BODY_HELD 0xD0D0D0u
#define MARK 0xFFFFFFu

int touch_shapes(int w, int h, TouchVert *out, int max, int *bodies) {
    Out o;
    Layout l;
    int b, i;
    float u, a, r, t;

    *bodies = 0;
    if (!shown() || w <= 0 || h <= 0) {
        return 0;
    }
    layout(w, h, &l);
    b = held(&l, w, h);
    u = l.u;
    o.v = out;
    o.n = 0;
    o.max = max;

    /* the buttons themselves */
    a = 0.055f * u; /* the d-pad's arms' half-width */
    r = 0.17f * u;  /* and reach */
    rect(&o, l.pad_x - a, l.pad_y - a, l.pad_x + a, l.pad_y + a, BODY);
    rect(&o, l.pad_x - a, l.pad_y - r, l.pad_x + a, l.pad_y - a, b & PAD_UP ? BODY_HELD : BODY);
    rect(&o, l.pad_x - a, l.pad_y + a, l.pad_x + a, l.pad_y + r, b & PAD_DOWN ? BODY_HELD : BODY);
    rect(&o, l.pad_x - r, l.pad_y - a, l.pad_x - a, l.pad_y + a, b & PAD_LEFT ? BODY_HELD : BODY);
    rect(&o, l.pad_x + a, l.pad_y - a, l.pad_x + r, l.pad_y + a, b & PAD_RIGHT ? BODY_HELD : BODY);
    for (i = 0; i < 4; i++) {
        disc(&o, l.face_x + FaceDir[i][0] * l.spacing, l.face_y + FaceDir[i][1] * l.spacing, l.button_r, b & FaceBits[i] ? BODY_HELD : BODY);
    }
    for (i = 0; i < 4; i++) {
        rect(&o, l.shoulder[i][0], l.shoulder[i][1], l.shoulder[i][2], l.shoulder[i][3], b & ShoulderBits[i] ? BODY_HELD : BODY);
    }
    for (i = 0; i < 2; i++) {
        rect(&o, l.small[i][0], l.small[i][1], l.small[i][2], l.small[i][3], b & SmallBits[i] ? BODY_HELD : BODY);
    }
    *bodies = o.n;

    /* their marks: arrows, the PlayStation's four shapes in their colours, the shoulders' names, Select's bars and Start's arrow */
    t = 0.012f * u;
    {
        float e = r - 0.03f * u, s = 0.035f * u;

        tri(&o, l.pad_x, l.pad_y - e, l.pad_x - s, l.pad_y - e + s, l.pad_x + s, l.pad_y - e + s, MARK);
        tri(&o, l.pad_x, l.pad_y + e, l.pad_x + s, l.pad_y + e - s, l.pad_x - s, l.pad_y + e - s, MARK);
        tri(&o, l.pad_x - e, l.pad_y, l.pad_x - e + s, l.pad_y + s, l.pad_x - e + s, l.pad_y - s, MARK);
        tri(&o, l.pad_x + e, l.pad_y, l.pad_x + e - s, l.pad_y - s, l.pad_x + e - s, l.pad_y + s, MARK);
    }
    {
        float s = 0.5f * l.button_r;
        float tx = l.face_x, ty = l.face_y - l.spacing;     /* Triangle, green */
        float cx = l.face_x + l.spacing, cy = l.face_y;     /* Circle, red */
        float xx = l.face_x, xy = l.face_y + l.spacing;     /* Cross, blue */
        float sx = l.face_x - l.spacing, sy = l.face_y;     /* Square, pink */

        line(&o, tx, ty - s, tx + s * 0.95f, ty + s * 0.6f, t, 0x9FE03Fu);
        line(&o, tx + s * 0.95f, ty + s * 0.6f, tx - s * 0.95f, ty + s * 0.6f, t, 0x9FE03Fu);
        line(&o, tx - s * 0.95f, ty + s * 0.6f, tx, ty - s, t, 0x9FE03Fu);
        ring(&o, cx, cy, s * 0.85f, t, 0x5F5FFFu);
        line(&o, xx - s * 0.8f, xy - s * 0.8f, xx + s * 0.8f, xy + s * 0.8f, t, 0xFF9F7Fu);
        line(&o, xx + s * 0.8f, xy - s * 0.8f, xx - s * 0.8f, xy + s * 0.8f, t, 0xFF9F7Fu);
        line(&o, sx - s * 0.75f, sy - s * 0.75f, sx + s * 0.75f, sy - s * 0.75f, t, 0xDF7FFFu);
        line(&o, sx + s * 0.75f, sy - s * 0.75f, sx + s * 0.75f, sy + s * 0.75f, t, 0xDF7FFFu);
        line(&o, sx + s * 0.75f, sy + s * 0.75f, sx - s * 0.75f, sy + s * 0.75f, t, 0xDF7FFFu);
        line(&o, sx - s * 0.75f, sy + s * 0.75f, sx - s * 0.75f, sy - s * 0.75f, t, 0xDF7FFFu);
    }
    for (i = 0; i < 4; i++) {
        const float *r4 = l.shoulder[i];
        float gh = (r4[3] - r4[1]) * 0.5f, gw = gh * 0.7f, cx = (r4[0] + r4[2]) / 2, y = r4[1] + gh * 0.5f;

        glyph(&o, i < 2 ? GlyphL : GlyphR, i < 2 ? (int)SDL_arraysize(GlyphL) : (int)SDL_arraysize(GlyphR), cx - gw * 1.25f, y, gw, gh,
              t * 0.8f, MARK);
        glyph(&o, i & 1 ? Glyph2 : Glyph1, i & 1 ? (int)SDL_arraysize(Glyph2) : (int)SDL_arraysize(Glyph1), cx + gw * 0.25f, y, gw, gh,
              t * 0.8f, MARK);
    }
    {
        const float *s4 = l.small[0], *t4 = l.small[1];
        float sh = s4[3] - s4[1], scx = (s4[0] + s4[2]) / 2, scy = (s4[1] + s4[3]) / 2;
        float tcx = (t4[0] + t4[2]) / 2, tcy = (t4[1] + t4[3]) / 2;

        line(&o, scx - sh * 0.3f, scy - sh * 0.12f, scx + sh * 0.3f, scy - sh * 0.12f, t * 0.8f, MARK);
        line(&o, scx - sh * 0.3f, scy + sh * 0.12f, scx + sh * 0.3f, scy + sh * 0.12f, t * 0.8f, MARK);
        tri(&o, tcx - sh * 0.2f, tcy - sh * 0.28f, tcx + sh * 0.28f, tcy, tcx - sh * 0.2f, tcy + sh * 0.28f, MARK);
    }
    return o.n;
}
