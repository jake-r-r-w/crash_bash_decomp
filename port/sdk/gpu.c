/* libgpu on PC (ROADMAP-B 6.2), on top of the software GPU in gpu_raster.c. Drawing happens at once,
 * so DrawSync never waits. */
#include <stdint.h>
#include <string.h>
#include <libgpu.h>
#include "gpu_raster.h"
#include "../platform/platform.h"

static DISPENV Disp;
static int DispOn;
static uint32_t Frame[VRAM_W * VRAM_H];

int ResetGraph(int mode) {
    if (mode == 0 || mode == 1) {
        gpu_set_draw_env(0, 0, 1023, 511, 0, 0, 0, 0);
    }
    return 0;
}

int SetGraphDebug(int level) {
    return 0;
}

void SetDispMask(int mask) {
    DispOn = mask;
}

int DrawSync(int mode) {
    return 0;
}

DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h) {
    env->disp.x = x;
    env->disp.y = y;
    env->disp.w = w;
    env->disp.h = h;
    env->screen.x = 0;
    env->screen.y = 0;
    env->screen.w = 0;
    env->screen.h = 0;
    env->isinter = 0;
    env->isrgb24 = 0;
    env->pad0 = 0;
    env->pad1 = 0;
    return env;
}

DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h) {
    env->clip.x = x;
    env->clip.y = y;
    env->clip.w = w;
    env->clip.h = h;
    env->ofs[0] = x;
    env->ofs[1] = y;
    env->tw.x = 0;
    env->tw.y = 0;
    env->tw.w = 0;
    env->tw.h = 0;
    env->tpage = 0;
    env->dtd = 1;
    env->dfe = 0;
    env->isbg = 0;
    env->r0 = 0;
    env->g0 = 0;
    env->b0 = 0;
    return env;
}

DISPENV *PutDispEnv(DISPENV *env) {
    Disp = *env;
    return env;
}

/* The packet SetDrawEnv writes: the clip, offset, texture page, texture window, mask and, if isbg is
 * set, a rectangle over the clip in the background colour. */
void SetDrawEnv(DR_ENV *dr, DRAWENV *env) {
    u_long *c = dr->code;
    int n = 0;
    int twx = (env->tw.x >> 3) & 31, twy = (env->tw.y >> 3) & 31;
    int twm = (-env->tw.w >> 3) & 31, twn = (-env->tw.h >> 3) & 31;

    c[n++] = 0xE3000000u | (env->clip.x & 0x3FF) | ((env->clip.y & 0x1FF) << 10);
    c[n++] = 0xE4000000u | ((env->clip.x + env->clip.w - 1) & 0x3FF) | (((env->clip.y + env->clip.h - 1) & 0x1FF) << 10);
    c[n++] = 0xE5000000u | (env->ofs[0] & 0x7FF) | ((env->ofs[1] & 0x7FF) << 11);
    c[n++] = 0xE1000000u | (env->tpage & 0x9FF) | ((env->dtd & 1) << 9) | ((env->dfe & 1) << 10);
    c[n++] = 0xE2000000u | twm | (twn << 5) | (twx << 10) | (twy << 15);
    c[n++] = 0xE6000000u;
    if (env->isbg) {
        c[n++] = 0x60000000u | env->r0 | (env->g0 << 8) | ((u_long)env->b0 << 16);
        c[n++] = ((env->clip.x - env->ofs[0]) & 0xFFFF) | (((env->clip.y - env->ofs[1]) & 0xFFFF) << 16);
        c[n++] = (env->clip.w & 0xFFFF) | ((u_long)(env->clip.h & 0xFFFF) << 16);
    }
    dr->tag = (u_long)n << 24;
}

DRAWENV *PutDrawEnv(DRAWENV *env) {
    SetDrawEnv(&env->dr_env, env);
    gpu_run_packet((const uint32_t *)env->dr_env.code, env->dr_env.tag >> 24);
    return env;
}

u_long *ClearOTag(u_long *ot, int n) {
    int i;

    for (i = 0; i < n - 1; i++) {
        ot[i] = ((uintptr_t)&ot[i + 1]) & 0xFFFFFF;
    }
    ot[n - 1] = 0xFFFFFF;
    return ot;
}

u_long *ClearOTagR(u_long *ot, int n) {
    int i;

    for (i = n - 1; i > 0; i--) {
        ot[i] = ((uintptr_t)&ot[i - 1]) & 0xFFFFFF;
    }
    ot[0] = 0xFFFFFF;
    return ot;
}

/* Walks the list. A link keeps 24 bits of its address (docs/PORT.md); the rest are the list's own. */
void DrawOTag(u_long *ot) {
    uintptr_t high = (uintptr_t)ot & ~(uintptr_t)0xFFFFFF;
    const uint32_t *p = (const uint32_t *)ot;

    for (;;) {
        uint32_t tag = p[0];
        uint32_t next = tag & 0xFFFFFF;
        int len = tag >> 24;

        if (len != 0) {
            gpu_run_packet(p + 1, len);
        }
        if (next == 0xFFFFFF) {
            break;
        }
        p = (const uint32_t *)(high | next);
    }
}

int ClearImage(RECT *r, u_char red, u_char green, u_char blue) {
    gpu_fill(r->x, r->y, r->w, r->h, red | (green << 8) | ((uint32_t)blue << 16));
    return 0;
}

int ClearImage2(RECT *r, u_char red, u_char green, u_char blue) {
    return ClearImage(r, red, green, blue);
}

int LoadImage(RECT *r, u_long *p) {
    gpu_load_image(r->x, r->y, r->w, r->h, (const uint16_t *)p);
    return 0;
}

int StoreImage(RECT *r, u_long *p) {
    gpu_store_image(r->x, r->y, r->w, r->h, (uint16_t *)p);
    return 0;
}

int MoveImage2(RECT *r, int x, int y) {
    gpu_move_image(r->x, r->y, x, y, r->w, r->h);
    return 0;
}

u_short GetClut(int x, int y) {
    return (u_short)((y << 6) | ((x >> 4) & 0x3F));
}

u_short GetTPage(int tp, int abr, int x, int y) {
    return (u_short)getTPage(tp, abr, x, y);
}

u_short LoadClut2(u_long *clut, int x, int y) {
    gpu_load_image(x, y, 16, 1, (const uint16_t *)clut);
    return GetClut(x, y);
}

void SetDrawArea(DR_AREA *p, RECT *r) {
    p->tag = (p->tag & 0x00FFFFFF) | (2u << 24);
    p->code[0] = 0xE3000000u | (r->x & 0x3FF) | ((r->y & 0x1FF) << 10);
    p->code[1] = 0xE4000000u | ((r->x + r->w - 1) & 0x3FF) | (((r->y + r->h - 1) & 0x1FF) << 10);
}

void SetDrawMove(DR_MOVE *p, RECT *r, int x, int y) {
    p->tag = (p->tag & 0x00FFFFFF) | ((u_long)((r->w != 0 && r->h != 0) ? 5 : 0) << 24);
    p->code[0] = 0x01000000u;
    p->code[1] = 0x80000000u;
    p->code[2] = (r->x & 0xFFFF) | ((u_long)(r->y & 0xFFFF) << 16);
    p->code[3] = (x & 0xFFFF) | ((u_long)(y & 0xFFFF) << 16);
    p->code[4] = (r->w & 0xFFFF) | ((u_long)(r->h & 0xFFFF) << 16);
}

void SetTile1(TILE_1 *p) {
    setlen(p, 2);
    setcode(p, 0x68);
}

/* The debug font: the retail game draws nothing with it. */
void FntLoad(int tx, int ty) {
}
int FntOpen(int x, int y, int w, int h, int isbg, int n) {
    return 0;
}
int FntPrint() {
    return 0;
}
u_long *FntFlush(int id) {
    return NULL;
}

/* What the TV would show: the display rectangle of VRAM as 32-bit pixels. */
void gpu_present(void) {
    int w = Disp.disp.w, h = Disp.disp.h;
    int x, y;

    if (w <= 0 || h <= 0) {
        return;
    }
    if (w > VRAM_W) {
        w = VRAM_W;
    }
    if (h > VRAM_H) {
        h = VRAM_H;
    }
    if (!DispOn) {
        memset(Frame, 0, (size_t)w * h * 4);
    } else if (Disp.isrgb24) {
        for (y = 0; y < h; y++) {
            const uint8_t *s = (const uint8_t *)&Vram[(Disp.disp.y + y) & 511][Disp.disp.x & 1023];
            uint32_t *d = &Frame[y * w];

            for (x = 0; x < w; x++) {
                d[x] = 0xFF000000u | ((uint32_t)s[0] << 16) | ((uint32_t)s[1] << 8) | s[2];
                s += 3;
            }
        }
    } else {
        for (y = 0; y < h; y++) {
            const uint16_t *s = Vram[(Disp.disp.y + y) & 511];
            uint32_t *d = &Frame[y * w];

            for (x = 0; x < w; x++) {
                uint32_t c = s[(Disp.disp.x + x) & 1023];
                uint32_t r = c & 31, g = (c >> 5) & 31, b = (c >> 10) & 31;

                d[x] = 0xFF000000u | (((r << 3) | (r >> 2)) << 16) | (((g << 3) | (g >> 2)) << 8) | ((b << 3) | (b >> 2));
            }
        }
    }
    platform_present(Frame, w, h);
}
