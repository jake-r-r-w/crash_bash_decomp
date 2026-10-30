/* The drawing types, primitives and functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBGPU_H
#define PC_LIBGPU_H

#include <stdint.h>
#include <libgte.h>

typedef struct { short x, y, w, h; } RECT;

/* Every primitive starts with this: the next primitive's address, and its length in words. */
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
    uint8_t r0, g0, b0, code;
} P_TAG;

typedef struct { uint32_t tag; uint32_t code[15]; } DR_ENV;
typedef struct { uint32_t tag; uint32_t code[5]; } DR_MOVE;
typedef struct { uint32_t tag; uint32_t code[1]; } DR_TPAGE;
typedef struct { uint32_t tag; uint32_t code[2]; } DR_AREA;

typedef struct {
    RECT disp, screen;
    uint8_t isinter, isrgb24, pad0, pad1;
} DISPENV;

typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    uint16_t tpage;
    uint8_t dtd, dfe, isbg, r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct {
    uint32_t tag;
    uint8_t r0, g0, b0, code;
    short x0, y0;
} TILE_1;

typedef struct {
    uint32_t tag;
    uint8_t r0, g0, b0, code;
    short x0, y0;
    uint8_t r1, g1, b1, pad1;
    short x1, y1;
    uint8_t r2, g2, b2, pad2;
    short x2, y2;
} POLY_G3;

typedef struct {
    uint32_t tag;
    uint8_t r0, g0, b0, code;
    short x0, y0;
    uint8_t u0, v0; uint16_t clut;
    short x1, y1;
    uint8_t u1, v1; uint16_t tpage;
    short x2, y2;
    uint8_t u2, v2; uint16_t pad1;
    short x3, y3;
    uint8_t u3, v3; uint16_t pad2;
} POLY_FT4;

typedef struct {
    uint32_t tag;
    uint8_t r0, g0, b0, code;
    short x0, y0;
    uint8_t u0, v0; uint16_t clut;
    uint8_t r1, g1, b1, p1;
    short x1, y1;
    uint8_t u1, v1; uint16_t tpage;
    uint8_t r2, g2, b2, p2;
    short x2, y2;
    uint8_t u2, v2; uint16_t pad2;
} POLY_GT3;

typedef struct {
    uint32_t tag;
    uint8_t r0, g0, b0, code;
    short x0, y0;
    uint8_t u0, v0; uint16_t clut;
    uint8_t r1, g1, b1, p1;
    short x1, y1;
    uint8_t u1, v1; uint16_t tpage;
    uint8_t r2, g2, b2, p2;
    short x2, y2;
    uint8_t u2, v2; uint16_t pad2;
    uint8_t r3, g3, b3, p3;
    short x3, y3;
    uint8_t u3, v3; uint16_t pad3;
} POLY_GT4;

#define setaddr(p, a) (((P_TAG *)(p))->addr = (uint32_t)(a))
#define getaddr(p) ((uint32_t)((P_TAG *)(p))->addr)
#define setlen(p, n) (((P_TAG *)(p))->len = (uint8_t)(n))
#define setcode(p, c) (((P_TAG *)(p))->code = (uint8_t)(c))
#define getcode(p) ((uint8_t)((P_TAG *)(p))->code)
#define setSemiTrans(p, on) ((on) ? setcode(p, getcode(p) | 2) : setcode(p, getcode(p) & ~2))
#define setPolyFT4(p) setlen(p, 9), setcode(p, 0x2C)
#define setPolyGT4(p) setlen(p, 12), setcode(p, 0x3C)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define getTPage(tp, abr, x, y) \
    ((((tp) & 3) << 7) | (((abr) & 3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3FF) >> 6) | \
     (((y) & 0x200) << 2))

int ResetGraph(int mode);
int SetGraphDebug(int level);
void SetDispMask(int mask);
int DrawSync(int mode);
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
DISPENV *PutDispEnv(DISPENV *env);
DRAWENV *PutDrawEnv(DRAWENV *env);
uint32_t *ClearOTag(uint32_t *ot, int n);
uint32_t *ClearOTagR(uint32_t *ot, int n);
void DrawOTag(uint32_t *ot);
int ClearImage(RECT *r, uint8_t red, uint8_t green, uint8_t blue);
int ClearImage2(RECT *r, uint8_t red, uint8_t green, uint8_t blue);
int LoadImage(RECT *r, uint32_t *p);
int StoreImage(RECT *r, uint32_t *p);
int MoveImage2(RECT *r, int x, int y);
uint16_t GetClut(int x, int y);
uint16_t GetTPage(int tp, int abr, int x, int y);
uint16_t LoadClut2(uint32_t *clut, int x, int y);
void SetDrawArea(DR_AREA *p, RECT *r);
void SetDrawEnv(DR_ENV *dr, DRAWENV *env);
void SetDrawMove(DR_MOVE *p, RECT *r, int x, int y);
void SetTile1(TILE_1 *p);
void FntLoad(int tx, int ty);
int FntOpen(int x, int y, int w, int h, int isbg, int n);
int FntPrint();
uint32_t *FntFlush(int id);

#endif
