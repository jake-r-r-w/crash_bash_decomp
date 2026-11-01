/* The Adventure test's player for the Pogo games, in place of the computer's hands, which go to a random square: paint the nearest
 * square that isn't his by the shortest way round what's in the way, take a pickup that's close, and go to the banker when he's got enough
 * to bank. Pogo-A-Gogo scores for closing a loop, so there he walks a square's edge. */
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/hero.h"
#include "bash/pogo_ovl.h"

#define BTN_UP 0x10
#define BTN_RIGHT 0x20
#define BTN_DOWN 0x40
#define BTN_LEFT 0x80
#define BTN_SQUARE 0x8000

#define BOARD 8
#define MODE_AGOGO 11
#define MODE_PADLOCK 13
#define PICKUP_BANKER 10
#define PICKUP_MUSHROOM 11
#define TILE_TNT 5
#define TILE_NITRO 6
#define TILE_ARROW 7
#define TILE_LOCKED 10
#define BANK_AT 14
#define NEAR_PICKUP 3
#define FAR 1000
#define LOOP_SIZE 4
#define PADLOCK_BANK_AT 8
#define PADLOCK_PULL (BOARD * 3)

bool PogoAI_CanPlayerShootAnyone(u32 hero);
bool PogoAI_CanPlayerShootAnyoneFourWays(u32 hero);
bool Pickup_IsClaimable(s32 arg0, s32 arg1, s32 arg2);
bool PogoPadlock_IsHere(s32 arg0, s32 arg1);
void PogoAI_Poll(u32 arg0);

static const s8 StepX[4] = { 0, 1, 0, -1 };
static const s8 StepZ[4] = { 1, 0, -1, 0 };
static const u16 StepButton[4] = { BTN_UP, BTN_RIGHT, BTN_DOWN, BTN_LEFT };

static s32 LoopStep;

/* Whether he can hop onto this square on his way somewhere */
static s32 square_open(s32 x, s32 z) {
    PogoSquare *c = &PogoGrid[x][z];

    if (c->occupied != 0 || (c->reserved != 0 && c->reserved != 1)) {
        return 0;
    }
    if (c->pickup != NULL && c->pickup->UserInfo.flags == PICKUP_MUSHROOM) {
        return 0;
    }
    if (c->mode == TILE_TNT || c->mode == TILE_NITRO) {
        return 0;
    }
    /* Padlock: standing on his own square loses them all, while he isn't locked */
    return !(GameMode == MODE_PADLOCK && PogoPlay[0].status != 6 && c->player == 0 && c->mode != TILE_LOCKED && !(c->flags & 0x10));
}

/* The shortest way from every square to (x, z). */
static void distances_from(s32 x, s32 z, s16 dist[BOARD][BOARD], u8 first[BOARD][BOARD]) {
    u8 qx[BOARD * BOARD], qz[BOARD * BOARD];
    s32 head = 0, tail = 0;
    s32 i, j, d;

    for (i = 0; i < BOARD; i++) {
        for (j = 0; j < BOARD; j++) {
            dist[i][j] = FAR;
        }
    }
    dist[x][z] = 0;
    qx[tail] = x;
    qz[tail++] = z;
    while (head < tail) {
        s32 cx = qx[head], cz = qz[head++];

        for (d = 0; d < 4; d++) {
            s32 nx = cx + StepX[d], nz = cz + StepZ[d];

            if ((u32)nx >= BOARD || (u32)nz >= BOARD || dist[nx][nz] != FAR || !square_open(nx, nz)) {
                continue;
            }
            dist[nx][nz] = dist[cx][cz] + 1;
            first[nx][nz] = cx == x && cz == z ? d : first[cx][cz];
            qx[tail] = nx;
            qz[tail++] = nz;
        }
    }
}

static s32 tiles_held(void) {
    s32 i, j, n = 0;

    for (i = 0; i < BOARD; i++) {
        for (j = 0; j < BOARD; j++) {
            n += PogoGrid[i][j].player == 0;
        }
    }
    return n;
}

/* What it's worth going to this square, for how far it is: less is better. */
static s32 square_cost(s32 x, s32 z, s32 far, s32 held) {
    PogoSquare *c = &PogoGrid[x][z];
    s32 cost = FAR;

    if (GameMode == MODE_PADLOCK && PogoPadlock_IsHere(x, z)) {
        return held >= PADLOCK_BANK_AT ? far - PADLOCK_PULL : far;
    }
    if (c->pickup != NULL) {
        s32 kind = c->pickup->UserInfo.flags;

        if (!Pickup_IsClaimable(0, x, z)) {
            return FAR * 2;
        }
        if (kind == PICKUP_BANKER) {
            return held >= (GameMode == MODE_PADLOCK ? PADLOCK_BANK_AT : BANK_AT) ? far - BOARD * 2 : FAR * 2;
        }
        return far <= NEAR_PICKUP ? far - NEAR_PICKUP : far;
    }
    if (c->mode == TILE_ARROW) {
        cost = far - NEAR_PICKUP;
    } else if (c->player != 0 && c->mode != TILE_LOCKED) {
        cost = far;
    }
    return cost;
}

/* Pogo-A-Gogo: round the edge of a square, which pays for all of it as it closes. */
static s32 next_loop_square(s32 x, s32 z, s32 *tx, s32 *tz) {
    static s32 cx, cz;
    s32 side = LoopStep / LOOP_SIZE;
    s32 along = LoopStep % LOOP_SIZE;

    if (LoopStep == 0) {
        cx = x > BOARD - 1 - LOOP_SIZE ? BOARD - 1 - LOOP_SIZE : x;
        cz = z > BOARD - 1 - LOOP_SIZE ? BOARD - 1 - LOOP_SIZE : z;
    }
    switch (side) {
    case 0:
        *tx = cx + along + 1;
        *tz = cz;
        break;
    case 1:
        *tx = cx + LOOP_SIZE;
        *tz = cz + along + 1;
        break;
    case 2:
        *tx = cx + LOOP_SIZE - along - 1;
        *tz = cz + LOOP_SIZE;
        break;
    default:
        *tx = cx;
        *tz = cz + LOOP_SIZE - along - 1;
        break;
    }
    return x == *tx && z == *tz;
}

void PogoPlayerBot(s32 id) {
    EXTengine *he;
    EXTbotai *ai;
    s16 dist[BOARD][BOARD];
    u8 first[BOARD][BOARD];
    s32 x, z, i, j;
    s32 best = FAR, tx = -1, tz = -1, held, pad = 0;

    if (id != 0) {
        PogoAI_Poll(id);
        return;
    }
    if (GAME->pause & 0x04000000) {
        return;
    }
    he = Heroes[0].hero_eng;
    ai = Heroes[0].hero_ai;
    ai->AImode = 2;
    x = (he->XYZ.x + 0x800) >> 9;
    z = (he->XYZ.z + 0x800) >> 9;
    if ((u32)x >= BOARD || (u32)z >= BOARD) {
        set_joypad(&Pads[0], 0);
        return;
    }
    distances_from(x, z, dist, first);
    held = tiles_held();
    if (GameMode == MODE_AGOGO) {
        if (next_loop_square(x, z, &tx, &tz)) {
            LoopStep = (LoopStep + 1) % (LOOP_SIZE * 4);
            next_loop_square(x, z, &tx, &tz);
        }
        if (dist[tx][tz] == FAR) {
            LoopStep = 0;
            tx = -1;
        }
    }
    if (tx < 0) {
        for (i = 0; i < BOARD; i++) {
            for (j = 0; j < BOARD; j++) {
                if ((i != x || j != z) && dist[i][j] != FAR) {
                    s32 cost = square_cost(i, j, dist[i][j], held);

                    if (cost < best) {
                        best = cost;
                        tx = i;
                        tz = j;
                    }
                }
            }
        }
    }
    if (tx >= 0 && (tx != x || tz != z) && dist[tx][tz] != FAR) {
        pad = StepButton[first[tx][tz]];
    }
    switch (PogoPlay[0].status) {
    case 1:
    case 3:
    case 4:
        pad |= PogoAI_CanPlayerShootAnyone(0) ? BTN_SQUARE : 0;
        break;
    case 2:
        pad |= PogoAI_CanPlayerShootAnyoneFourWays(0) ? BTN_SQUARE : 0;
        break;
    }
    set_joypad(&Pads[0], pad);
}
