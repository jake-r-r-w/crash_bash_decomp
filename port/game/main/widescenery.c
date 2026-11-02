/* Widescreen shows further than some levels were built to be seen, and they stop short, with nothing there. So a level can have more of its own
 * scenery placed round it, which only a wide view draws, behind everything else: copies of its map's pieces, moved and turned. No art here, just
 * where they go.
 *
 * A level's own piece that was never meant to be seen can go too, in a wide view.
 *
 * To place more: WIDE_LIST=1 prints a level's file number and pieces, WIDE_EXTRA="piece,cx,cz,dx,dy,dz,turn[,placed[,face]];..." tries some without a
 * rebuild, in place of the table, WIDE_HIDE="piece,piece" hides some, and
 * WIDE_RING="floor,wall,walls,y,tile,inner,outer,top,sides,floor_colour,wall_colour" tries a ring. */
#include <math.h>
#include <stdio.h>
#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/engine.h"
#include "../../platform/gfx.h"

void draw_map_object(void *sub, void *arg1, s32 flags, void *arg3);
void draw_owner(const struct subengine *sub);
void draw_owner_part(const void *mesh);

typedef struct {
    s16 file;      /* the map's model file, by its place in DatIndex */
    s16 piece;     /* the map's placement it copies */
    s32 cx, cz;    /* what it turns round */
    s32 dx, dy, dz;
    s32 turn;      /* 0x1000 is all the way round */
    s32 placed;    /* where the level's own things go, rather than behind everything */
    s32 face;      /* it faces what it turns round, so a row of them makes a curve, rather than keeping its own angle */
    s32 entity;    /* shows this of the level's file instead of what the piece does, or 0 */
} WideExtra;

#define FILE_N_BALLISM 651
#define FILE_BEACH_BALL 655
#define FILE_DANTES_DASH 637

static const WideExtra Extras[] = {
    /* kelp and coral on the extra sand */
    { FILE_BEACH_BALL, 0, 0, 0, -5641, 683, 2999, 0, 0, 1, 0x2014 },
    { FILE_BEACH_BALL, 0, 0, 0, -5258, 610, 2124, 0, 0, 1, 0x2015 },
    { FILE_BEACH_BALL, 0, 0, 0, -4829, 653, 1115, 0, 0, 1, 0x2016 },
    { FILE_BEACH_BALL, 0, 0, 0, -4534, 642, 557, 0, 0, 1, 0x2014 },
    { FILE_BEACH_BALL, 0, 0, 0, -5315, 552, 93, 0, 0, 1, 0x2013 },
    { FILE_BEACH_BALL, 0, 0, 0, -5883, 602, -722, 0, 0, 1, 0x2015 },
    { FILE_BEACH_BALL, 0, 0, 0, -5072, 659, -894, 0, 0, 1, 0x2017 },
    { FILE_BEACH_BALL, 0, 0, 0, -6477, 598, -2358, 0, 0, 1, 0x2016 },
    { FILE_BEACH_BALL, 0, 0, 0, -4272, 564, -2368, 0, 0, 1, 0x2018 },
    { FILE_BEACH_BALL, 0, 0, 0, -3578, 573, -2796, 0, 0, 1, 0x2014 },
    { FILE_BEACH_BALL, 0, 0, 0, 5970, 662, 3175, 0, 0, 1, 0x2015 },
    { FILE_BEACH_BALL, 0, 0, 0, 4532, 570, 1386, 0, 0, 1, 0x2016 },
    { FILE_BEACH_BALL, 0, 0, 0, 4068, 606, 1322, 0, 0, 1, 0x2014 },
    { FILE_BEACH_BALL, 0, 0, 0, 5888, 648, 1144, 0, 0, 1, 0x2013 },
    { FILE_BEACH_BALL, 0, 0, 0, 6551, 636, 0, 0, 0, 1, 0x2015 },
    { FILE_BEACH_BALL, 0, 0, 0, 4209, 612, -517, 0, 0, 1, 0x2017 },
    { FILE_BEACH_BALL, 0, 0, 0, 4292, 664, -1070, 0, 0, 1, 0x2016 },
    { FILE_BEACH_BALL, 0, 0, 0, 6344, 615, -2309, 0, 0, 1, 0x2018 },
    { FILE_BEACH_BALL, 0, 0, 0, 4193, 655, -2230, 0, 0, 1, 0x2014 },
    { FILE_BEACH_BALL, 0, 0, 0, 3541, 580, -2573, 0, 0, 1, 0x2015 },
    /* the swamp grass, on round the edge of the lava, close enough to make a row, and facing in so it curves */
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -100, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -100, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -200, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -200, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -300, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -300, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -400, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -400, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -500, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -500, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -600, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -600, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -700, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -700, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 80, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 80, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 160, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 160, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 240, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 240, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 320, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 320, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 400, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 400, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 480, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 480, 0, 1 },
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

/* A ring round the arena, made of the level's own tiles at their proper size: a floor, and a wall that curves round it. */
typedef struct {
    s16 file;
    s16 floor_image;
    s16 wall_image, wall_images; /* a panorama: these tiles in turn, all the way round */
    s32 floor_y, floor_tile;     /* where the floor is, and how big a tile of it */
    s32 inner, outer;            /* the floor's radius */
    s32 wall_top;
    s32 sides;
    u32 floor_colour, wall_colour;
} WideRing;

static const WideRing Rings[] = {
    /* only a wall, as the lava's fine: its own forest panorama, at the colours of the trees in front */
    { FILE_DANTES_DASH, 0, 14, 4, 3000, 600, 7000, 7000, -4000, 20, 0, 0x0A3A60 },
};
#define NUM_RINGS ((s32)(sizeof(Rings) / sizeof(Rings[0])))

static WideRing TriedRing;
static subengine RingOwner;
static s32 RingTried = -1;

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
        e->face = 0;
        e->entity = 0;
        if (sscanf(s, ",%d%n", &placed, &more) == 1) {
            s += more;
            e->face = placed;
        }
        if (sscanf(s, ",%x%n", &placed, &more) == 1) {
            s += more;
            e->entity = placed;
        }
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
    if (e->entity != 0) {
        copy->entity = (u16)e->entity;
    }
    x = from->XYZ.x - e->cx;
    z = from->XYZ.z - e->cz;
    copy->XYZ.x = e->cx + (x * c + z * s) / 0x1000 + e->dx;
    copy->XYZ.y = from->XYZ.y + e->dy;
    copy->XYZ.z = e->cz + (z * c - x * s) / 0x1000 + e->dz;
    if (e->face) {
        angle = atan2f((float)(copy->XYZ.x - e->cx), (float)(copy->XYZ.z - e->cz));
        c = (s16)(cosf(angle) * 4096.0f);
        s = (s16)(sinf(angle) * 4096.0f);
    }
    memset(&spin, 0, sizeof(spin));
    spin.m[0][0] = c;
    spin.m[0][2] = s;
    spin.m[1][1] = 0x1000;
    spin.m[2][0] = (s16)-s;
    spin.m[2][2] = c;
    matrix_mul(&spin, (MATRIX *)&from->xform.mtx, &turned);
    copy->xform.mtx = turned;
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

static void ring_quad(const obj_textureset *ts, s32 image, const s16 corners[4][3], const u32 colour[4], s32 name, subengine *owner) {
    const obj_texture *t = &ts->tex[image];
    DrawFace f;
    ViewPoint q[4];
    s16 p[4];
    s32 i;

    memset(&f, 0, sizeof(f));
    f.image = t->info.image;
    f.palette = t->info.palette;
    for (i = 0; i < 4; i++) {
        p[0] = corners[i][0];
        p[1] = corners[i][1];
        p[2] = corners[i][2];
        p[3] = 0;
        project_point(p, &q[i]);
        f.colour[i] = colour[i];
        f.uv[i] = (u16)(t->info.uv0 | ((i & 1) ? t->w - 1 : 0) | (((i & 2) ? t->h - 1 : 0) << 8));
    }
    draw_owner(owner);
    draw_owner_part((void *)(uintptr_t)(name + 1));
    draw_mesh_quad(RenderState.layer, &f, q);
}

/* The view's own axes, as the map's pieces have them, with nothing moved. */
static void ring_begin(map_enviroment *scene, subengine *owner, MATRIX *view) {
    *owner = scene->subs[0];
    owner->next = NULL;
    owner->XYZ.x = 0;
    owner->XYZ.y = 0;
    owner->XYZ.z = 0;
    memset(&owner->xform.mtx, 0, sizeof(owner->xform.mtx));
    owner->xform.mtx.m[0][0] = 0x1000;
    owner->xform.mtx.m[1][1] = 0x1000;
    owner->xform.mtx.m[2][2] = 0x1000;
    draw_map_object(owner, view, owner->flags, scene->dbase->data);
    draw_behind(1);
}

static void draw_ring(map_enviroment *scene, const WideRing *r, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;
    s32 side, ring, name = 0;
    s32 rows = (r->outer - r->inner) / r->floor_tile;
    s16 c[4][3];
    const u32 wall_colours[4] = { r->wall_colour, r->wall_colour, r->wall_colour, r->wall_colour };
    const u32 floor_colours[4] = { r->floor_colour, r->floor_colour, r->floor_colour, r->floor_colour };

    if (ts->tex == NULL) {
        return;
    }
    ring_begin(scene, owner, view);
    for (side = 0; side < r->sides; side++) {
        float a0 = side * 6.28318530718f / r->sides, a1 = (side + 1) * 6.28318530718f / r->sides;
        float x0 = cosf(a0), z0 = sinf(a0), x1 = cosf(a1), z1 = sinf(a1);

        c[0][0] = (s16)(x0 * r->outer);
        c[0][1] = (s16)r->wall_top;
        c[0][2] = (s16)(z0 * r->outer);
        c[1][0] = (s16)(x1 * r->outer);
        c[1][1] = (s16)r->wall_top;
        c[1][2] = (s16)(z1 * r->outer);
        c[2][0] = c[0][0];
        c[2][1] = (s16)r->floor_y;
        c[2][2] = c[0][2];
        c[3][0] = c[1][0];
        c[3][1] = (s16)r->floor_y;
        c[3][2] = c[1][2];
        ring_quad(ts, r->wall_image + side % r->wall_images, c, wall_colours, name++, owner);
        for (ring = 0; ring < rows; ring++) {
            float in = (float)(r->inner + ring * r->floor_tile), out = in + r->floor_tile;

            c[0][0] = (s16)(x0 * out);
            c[0][1] = (s16)r->floor_y;
            c[0][2] = (s16)(z0 * out);
            c[1][0] = (s16)(x1 * out);
            c[1][1] = (s16)r->floor_y;
            c[1][2] = (s16)(z1 * out);
            c[2][0] = (s16)(x0 * in);
            c[2][1] = (s16)r->floor_y;
            c[2][2] = (s16)(z0 * in);
            c[3][0] = (s16)(x1 * in);
            c[3][1] = (s16)r->floor_y;
            c[3][2] = (s16)(z1 * in);
            ring_quad(ts, r->floor_image, c, floor_colours, name++, owner);
        }
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Beach Ball's backdrop runs on past its ends, along the curve it's on. These are its 3 rows at each column: its own for FIRST to LAST, and
   past them they carry on turning as it does. */
typedef struct {
    s16 tx, tz, mx, mz, bx, bz;
} WallColumn;

static const WallColumn BeachWall[] = {
    { -5773, -12693, -5756, -12655, -5740, -12619 },
    { -8175, -10272, -8150, -10241, -8127, -10212 },
    { -9736, -7240, -9708, -7219, -9680, -7198 },
    { -10314, -3879, -10284, -3868, -10254, -3857 },
    { -9855, -500, -9826, -499, -9798, -498 },
    { -8486, 2404, -8461, 2396, -8437, 2390 },
    { -6492, 4568, -6492, 4527, -6492, 4487 },
    { -4234, 5859, -4234, 5818, -4234, 5778 },
    { -2073, 6456, -2073, 6414, -2073, 6375 },
    { 12, 6552, 12, 6511, 2, 6469 },
    { 1994, 6539, 1994, 6497, 1994, 6458 },
    { 4294, 5859, 4294, 5818, 4294, 5778 },
    { 6635, 4265, 6635, 4223, 6635, 4184 },
    { 8598, 1767, 8574, 1762, 8551, 1758 },
    { 9805, -1422, 9777, -1418, 9751, -1414 },
    { 9998, -4826, 9969, -4812, 9943, -4799 },
    { 9157, -8131, 9131, -8107, 9107, -8086 },
    { 7362, -11030, 7341, -10998, 7322, -10969 },
    { 4778, -13255, 4765, -13217, 4752, -13181 },
};
#define BEACH_COLUMNS ((s32)(sizeof(BeachWall) / sizeof(BeachWall[0])))
#define BEACH_OWN_FIRST 6
#define BEACH_OWN_LAST 12
/* The tiles of each segment: below, then above. They go round in a cycle, as the panorama does. */
static const s16 BeachTiles[BEACH_COLUMNS - 1][2] = { { 5, 10 }, { 2, 7 }, { 3, 8 }, { 4, 9 }, { 6, 11 }, { 5, 10 }, { 2, 7 }, { 3, 8 }, { 4, 9 }, { 6, 11 }, { 5, 10 }, { 5, 10 }, { 2, 7 }, { 3, 8 }, { 4, 9 }, { 6, 11 }, { 5, 10 }, { 2, 7 } };
/* Its rows' heights and colours, as the level's. */
static const s16 BeachRowY[3] = { -930, -16, 740 };
static const u32 BeachRowColour[3] = { 0x9C5A39, 0x634A18, 0x212108 };

/* The sand: bumps and varied colours at each corner, in the ranges the level's own has, from a little inside the wall out to it. */
#define SAND_IMAGE 0
#define SAND_ACROSS 2 /* cells a wall segment */
#define SAND_ROWS 5
#define SAND_INSIDE 0.35f
#define SAND_Y_IN 430
#define SAND_Y_OUT 770
#define SAND_BUMP 150

static const u32 SandColours[] = { 0x393921, 0x313118, 0x212110, 0x525231, 0x181810, 0x292918, 0x313110 };

static u32 sand_hash(s32 col, s32 row) {
    u32 h = (u32)col * 73856093u ^ (u32)row * 19349663u;

    h ^= h >> 13;
    h *= 1274126177u;
    return h ^ (h >> 16);
}

static void sand_corner(s32 col, s32 row, s16 *c, u32 *colour) {
    s32 seg = col / SAND_ACROSS;
    float along = (float)(col % SAND_ACROSS) / SAND_ACROSS;
    const WallColumn *a = &BeachWall[seg < BEACH_COLUMNS - 1 ? seg : BEACH_COLUMNS - 1];
    const WallColumn *b = &BeachWall[seg < BEACH_COLUMNS - 1 ? seg + 1 : BEACH_COLUMNS - 1];
    float out = SAND_INSIDE + (1.0f - SAND_INSIDE) * row / SAND_ROWS;
    u32 h = sand_hash(col, row);
    /* the last row meets the wall's foot, so no bump there */
    s32 bump = row == SAND_ROWS ? 0 : (s32)(h % (SAND_BUMP * 2 + 1)) - SAND_BUMP;

    c[0] = (s16)((a->bx + (b->bx - a->bx) * along) * out);
    c[1] = (s16)(SAND_Y_IN + (SAND_Y_OUT - SAND_Y_IN) * row / SAND_ROWS + bump);
    c[2] = (s16)((a->bz + (b->bz - a->bz) * along) * out);
    *colour = SandColours[(h >> 8) % (sizeof(SandColours) / sizeof(SandColours[0]))];
}

static void draw_beach_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;
    s32 col, row, seg, band, name = 0;
    s16 c[4][3];
    u32 colours[4];

    if (ts->tex == NULL) {
        return;
    }
    ring_begin(scene, owner, view);
    for (col = 0; col < (BEACH_COLUMNS - 1) * SAND_ACROSS; col++) {
        for (row = 0; row < SAND_ROWS; row++) {
            sand_corner(col, row + 1, c[0], &colours[0]);
            sand_corner(col + 1, row + 1, c[1], &colours[1]);
            sand_corner(col, row, c[2], &colours[2]);
            sand_corner(col + 1, row, c[3], &colours[3]);
            ring_quad(ts, SAND_IMAGE, c, colours, name++, owner);
        }
    }
    for (seg = 0; seg < BEACH_COLUMNS - 1; seg++) {
        const WallColumn *a = &BeachWall[seg], *b = &BeachWall[seg + 1];

        if (seg >= BEACH_OWN_FIRST && seg < BEACH_OWN_LAST) {
            continue;
        }
        for (band = 0; band < 2; band++) {
            /* band 0 is the upper tile */
            c[0][0] = band == 0 ? a->tx : a->mx;
            c[0][1] = BeachRowY[band];
            c[0][2] = band == 0 ? a->tz : a->mz;
            c[1][0] = band == 0 ? b->tx : b->mx;
            c[1][1] = BeachRowY[band];
            c[1][2] = band == 0 ? b->tz : b->mz;
            c[2][0] = band == 0 ? a->mx : a->bx;
            c[2][1] = BeachRowY[band + 1];
            c[2][2] = band == 0 ? a->mz : a->bz;
            c[3][0] = band == 0 ? b->mx : b->bx;
            c[3][1] = BeachRowY[band + 1];
            c[3][2] = band == 0 ? b->mz : b->bz;
            colours[0] = colours[1] = BeachRowColour[band];
            colours[2] = colours[3] = BeachRowColour[band + 1];
            ring_quad(ts, BeachTiles[seg][band == 0 ? 1 : 0], c, colours, name++, owner);
        }
    }
    draw_behind(0);
    draw_owner(NULL);
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
    if (RingTried < 0) {
        const char *r = getenv("WIDE_RING");
        int floor_image, wall_image, wall_images, floor_y, floor_tile, inner, outer, wall_top, sides;
        unsigned floor_colour, wall_colour;

        RingTried = r != NULL && sscanf(r, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%x,%x", &floor_image, &wall_image, &wall_images, &floor_y, &floor_tile, &inner, &outer, &wall_top, &sides, &floor_colour, &wall_colour) == 11;
        if (RingTried) {
            TriedRing.floor_image = (s16)floor_image;
            TriedRing.wall_image = (s16)wall_image;
            TriedRing.wall_images = (s16)wall_images;
            TriedRing.floor_y = floor_y;
            TriedRing.floor_tile = floor_tile;
            TriedRing.inner = inner;
            TriedRing.outer = outer;
            TriedRing.wall_top = wall_top;
            TriedRing.sides = sides;
            TriedRing.floor_colour = floor_colour;
            TriedRing.wall_colour = wall_colour;
        }
    }
    if (RingTried) {
        draw_ring(scene, &TriedRing, &RingOwner, view);
    } else if (NumTried == 0) {
        s32 file = (s32)(scene->dbase->file - DatIndex);

        if (file == FILE_BEACH_BALL) {
            draw_beach_backdrop(scene, &RingOwner, view);
        }

        for (i = 0; i < NUM_RINGS; i++) {
            if (Rings[i].file == file) {
                draw_ring(scene, &Rings[i], &RingOwner, view);
            }
        }
    }
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
