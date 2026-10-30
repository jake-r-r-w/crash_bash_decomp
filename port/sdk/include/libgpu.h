/* The drawing types, primitives and functions the game uses: PsyQ's names and layouts (docs/PORT.md). */
#ifndef PC_LIBGPU_H
#define PC_LIBGPU_H

#include <pc_types.h>
#include <libgte.h>

typedef struct { short x, y, w, h; } RECT;

/* Every primitive starts with this: the next primitive's address, and its length in words. */
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
    u_char r0, g0, b0, code;
} P_TAG;

typedef struct { u_long tag; u_long code[15]; } DR_ENV;
typedef struct { u_long tag; u_long code[5]; } DR_MOVE;
typedef struct { u_long tag; u_long code[1]; } DR_TPAGE;
typedef struct { u_long tag; u_long code[2]; } DR_AREA;

typedef struct {
    RECT disp, screen;
    u_char isinter, isrgb24, pad0, pad1;
} DISPENV;

typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    u_short tpage;
    u_char dtd, dfe, isbg, r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
} TILE_1;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, pad1;
    short x1, y1;
    u_char r2, g2, b2, pad2;
    short x2, y2;
} POLY_G3;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0; u_short clut;
    short x1, y1;
    u_char u1, v1; u_short tpage;
    short x2, y2;
    u_char u2, v2; u_short pad1;
    short x3, y3;
    u_char u3, v3; u_short pad2;
} POLY_FT4;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0; u_short clut;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char u1, v1; u_short tpage;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_char u2, v2; u_short pad2;
} POLY_GT3;

typedef struct {
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char u0, v0; u_short clut;
    u_char r1, g1, b1, p1;
    short x1, y1;
    u_char u1, v1; u_short tpage;
    u_char r2, g2, b2, p2;
    short x2, y2;
    u_char u2, v2; u_short pad2;
    u_char r3, g3, b3, p3;
    short x3, y3;
    u_char u3, v3; u_short pad3;
} POLY_GT4;

#define setaddr(p, a) (((P_TAG *)(p))->addr = (u_long)(a))
#define getaddr(p) ((u_long)((P_TAG *)(p))->addr)
#define setlen(p, n) (((P_TAG *)(p))->len = (u_char)(n))
#define setcode(p, c) (((P_TAG *)(p))->code = (u_char)(c))
#define getcode(p) ((u_char)((P_TAG *)(p))->code)
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
u_long *ClearOTag(u_long *ot, int n);
u_long *ClearOTagR(u_long *ot, int n);
void DrawOTag(u_long *ot);
int ClearImage(RECT *r, u_char red, u_char green, u_char blue);
int ClearImage2(RECT *r, u_char red, u_char green, u_char blue);
int LoadImage(RECT *r, u_long *p);
int StoreImage(RECT *r, u_long *p);
int MoveImage2(RECT *r, int x, int y);
u_short GetClut(int x, int y);
u_short GetTPage(int tp, int abr, int x, int y);
u_short LoadClut2(u_long *clut, int x, int y);
void SetDrawArea(DR_AREA *p, RECT *r);
void SetDrawEnv(DR_ENV *dr, DRAWENV *env);
void SetDrawMove(DR_MOVE *p, RECT *r, int x, int y);
void SetTile1(TILE_1 *p);
void FntLoad(int tx, int ty);
int FntOpen(int x, int y, int w, int h, int isbg, int n);
int FntPrint();
u_long *FntFlush(int id);

#endif
