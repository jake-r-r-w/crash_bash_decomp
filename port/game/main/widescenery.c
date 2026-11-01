/* Widescreen shows further than some levels were built to be seen, and they stop short, with nothing there. So a level can have more of its own
 * scenery placed round it, which only a wide view draws, behind everything else: copies of its map's pieces, moved and turned. No art here, just
 * where they go.
 *
 * A level's own piece that was never meant to be seen can go too, in a wide view.
 *
 * To place more: WIDE_LIST=1 prints a level's file number and pieces, WIDE_EXTRA="piece,cx,cz,dx,dy,dz,turn[,placed];..." tries some without a
 * rebuild, in place of the table, and WIDE_HIDE="piece,piece" hides some. */
#include <math.h>
#include <stdio.h>
#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/engine.h"
#include "../../platform/gfx.h"

typedef struct {
    s16 file;      /* the map's model file, by its place in DatIndex */
    s16 piece;     /* the map's placement it copies */
    s32 cx, cz;    /* what it turns round */
    s32 dx, dy, dz;
    s32 turn;      /* 0x1000 is all the way round */
    s32 placed;    /* where the level's own things go, rather than behind everything */
} WideExtra;

#define FILE_N_BALLISM 651
#define FILE_BEACH_BALL 655
#define FILE_VOLCANO_RACE 637

static const WideExtra Extras[] = {
    /* the walls round the arena, carried on round it */
    { FILE_N_BALLISM, 0, -3, 1, 0, 0, 0, 500 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 0, 0, -500 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 0, 0, 900 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 0, 0, -900 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 700, 0, 700 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 700, 0, -700 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 1400, 0, 500 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 1400, 0, -500 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 700, 0, 1100 },
    { FILE_N_BALLISM, 0, -3, 1, 0, 700, 0, -1100 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 0, 0, 500 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 0, 0, -500 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 0, 0, 900 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 0, 0, -900 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 700, 0, 700 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 700, 0, -700 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 1400, 0, 500 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 1400, 0, -500 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 700, 0, 1100 },
    { FILE_BEACH_BALL, 0, -3, 1, 0, 700, 0, -1100 },
    /* the forest behind the track */
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 0, 0, 600 },
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 0, 0, -600 },
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 0, 0, 1000 },
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 0, 0, -1000 },
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 700, 0, 800 },
    { FILE_VOLCANO_RACE, 10, 0, 0, 0, 700, 0, -800 },
};
#define NUM_EXTRAS ((s32)(sizeof(Extras) / sizeof(Extras[0])))

/* Off the sides of a 4:3 screen, and so unfinished: */
static const struct {
    s16 file, piece;
} Hidden[] = {
    /* the front corners' wedges and needles, floating on their bases with no dish */
    { FILE_N_BALLISM, 7 },
    { FILE_N_BALLISM, 8 },
    { FILE_N_BALLISM, 9 },
    { FILE_N_BALLISM, 10 },
};
#define NUM_HIDDEN ((s32)(sizeof(Hidden) / sizeof(Hidden[0])))

#define MAX_TRIED_HIDDEN 32
static s16 TriedHidden[MAX_TRIED_HIDDEN];
static s32 NumTriedHidden = -1;

#define MAX_TRIED 32
#define MAX_COPIES (NUM_EXTRAS + MAX_TRIED)

static WideExtra Tried[MAX_TRIED];
static s32 NumTried = -1;
static subengine Copies[MAX_COPIES];

static void read_tried(void) {
    const char *s = getenv("WIDE_EXTRA");

    NumTried = 0;
    while (s != NULL && *s != 0 && NumTried < MAX_TRIED) {
        WideExtra *e = &Tried[NumTried];
        int piece, cx, cz, dx, dy, dz, turn, used = 0, placed = 0, more = 0;

        if (sscanf(s, "%d,%d,%d,%d,%d,%d,%d%n", &piece, &cx, &cz, &dx, &dy, &dz, &turn, &used) != 7) {
            break;
        }
        e->file = -1;
        e->piece = (s16)piece;
        e->cx = cx;
        e->cz = cz;
        e->dx = dx;
        e->dy = dy;
        e->dz = dz;
        e->turn = turn;
        s += used;
        if (sscanf(s, ",%d%n", &placed, &more) == 1) {
            s += more;
        }
        e->placed = placed;
        NumTried++;
        if (*s == ';') {
            s++;
        }
    }
}

static void list_pieces(const map_enviroment *scene) {
    static const void *listed;
    s32 i;

    if (listed == scene->subs || getenv("WIDE_LIST") == NULL) {
        return;
    }
    listed = scene->subs;
    for (i = 0; i < scene->num_placements; i++) {
        const subengine *p = &scene->subs[i];

        printf("file %d piece %d entity %x at %d,%d,%d\n", (int)(scene->dbase->file - DatIndex), i, p->entity, p->XYZ.x, p->XYZ.y, p->XYZ.z);
    }
    fflush(stdout);
}

static void draw_extra(const map_enviroment *scene, const WideExtra *e, subengine *copy, MATRIX *view) {
    const subengine *from;
    MATRIX turned;
    MATRIX spin;
    float angle = (float)e->turn * 6.28318530718f / 4096.0f;
    s16 c = (s16)(cosf(angle) * 4096.0f), s = (s16)(sinf(angle) * 4096.0f);
    s32 x, z;

    if (e->piece < 0 || e->piece >= scene->num_placements) {
        return;
    }
    from = &scene->subs[e->piece];
    *copy = *from;
    copy->next = NULL;
    memset(&spin, 0, sizeof(spin));
    spin.m[0][0] = c;
    spin.m[0][2] = s;
    spin.m[1][1] = 0x1000;
    spin.m[2][0] = (s16)-s;
    spin.m[2][2] = c;
    matrix_mul(&spin, (MATRIX *)&from->xform.mtx, &turned);
    copy->xform.mtx = turned;
    x = from->XYZ.x - e->cx;
    z = from->XYZ.z - e->cz;
    copy->XYZ.x = e->cx + (x * c + z * s) / 0x1000 + e->dx;
    copy->XYZ.y = from->XYZ.y + e->dy;
    copy->XYZ.z = e->cz + (z * c - x * s) / 0x1000 + e->dz;
    draw_behind(!e->placed);
    draw_owner(copy);
    copy->hook(copy, view, 0);
    draw_owner(NULL);
    draw_behind(0);
}

s32 wide_scenery_hides(const map_enviroment *scene, const subengine *sub) {
    s32 piece = (s32)(sub - scene->subs);
    s32 file;
    s32 i;

    if (gfx_view_extra() <= 0) {
        return 0;
    }
    if (NumTriedHidden < 0) {
        const char *s = getenv("WIDE_HIDE");

        NumTriedHidden = 0;
        while (s != NULL && *s != 0 && NumTriedHidden < MAX_TRIED_HIDDEN) {
            char *end;
            long n = strtol(s, &end, 10);

            if (end == s) {
                break;
            }
            TriedHidden[NumTriedHidden++] = (s16)n;
            s = *end == ',' ? end + 1 : end;
        }
    }
    for (i = 0; i < NumTriedHidden; i++) {
        if (TriedHidden[i] == piece) {
            return 1;
        }
    }
    file = (s32)(scene->dbase->file - DatIndex);
    for (i = 0; i < NUM_HIDDEN; i++) {
        if (Hidden[i].file == file && Hidden[i].piece == piece) {
            return 1;
        }
    }
    return 0;
}

void wide_scenery_draw(map_enviroment *scene, MATRIX *view) {
    s32 file;
    s32 i;

    if (gfx_view_extra() <= 0 || scene->subs == NULL) {
        return;
    }
    if (NumTried < 0) {
        read_tried();
    }
    list_pieces(scene);
    file = (s32)(scene->dbase->file - DatIndex);
    for (i = 0; i < NUM_EXTRAS; i++) {
        if (Extras[i].file == file && NumTried == 0) {
            draw_extra(scene, &Extras[i], &Copies[i], view);
        }
    }
    for (i = 0; i < NumTried; i++) {
        draw_extra(scene, &Tried[i], &Copies[NUM_EXTRAS + i], view);
    }
}
