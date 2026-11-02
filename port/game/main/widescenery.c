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

static void ring_quad(const obj_textureset *ts, s32 image, const s16 corners[4][3], const u32 colour[4], s32 name, subengine *owner, s32 top_row) {
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
        f.uv[i] = (u16)(t->info.uv0 | ((i & 1) ? t->w - 1 : 0) | (((i & 2) && !top_row ? t->h - 1 : 0) << 8));
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
        ring_quad(ts, r->wall_image + side % r->wall_images, c, wall_colours, name++, owner, 0);
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
            ring_quad(ts, r->floor_image, c, floor_colours, name++, owner, 0);
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
#define BEACH_WALL_JOIN (-880) /* a little under its own top, which is -904 to -930 */
#define BEACH_WALL_TOP (-4000)

/* The sand, from a little inside the wall out to it: each corner's height and colour blended from the level's nearest floor vertices, so it
   meets the real floor, and keeping some unevenness further off. Columns of SAND_ROWS + 1 corners, inside first. */
#define SAND_IMAGE 0
#define SAND_ROWS 5
#define SAND_COLUMNS 37

typedef struct {
    s16 x, y, z;
    u32 colour;
} SandCorner;

static const SandCorner BeachSand[SAND_COLUMNS * (SAND_ROWS + 1)] = {
    { -2009, 317, -4417, 0x232316 }, { -2755, 497, -6057, 0x1C1C12 }, { -3501, 362, -7698, 0x2A2A1A }, { -4248, 479, -9338, 0x202014 },
    { -4994, 400, -10979, 0x30301E }, { -5740, 760, -12619, 0x18180F }, { -2427, 459, -3995, 0x262618 }, { -3328, 451, -5479, 0x16160E },
    { -4229, 342, -6963, 0x1E1E13 }, { -5131, 498, -8447, 0x393924 }, { -6032, 386, -9931, 0x2A2A1B }, { -6933, 760, -11415, 0x30301E },
    { -2844, 433, -3574, 0x2D2D1D }, { -3901, 397, -4902, 0x373723 }, { -4957, 518, -6229, 0x333320 }, { -6014, 409, -7557, 0x2D2D1D },
    { -7070, 377, -8884, 0x14140D }, { -8127, 760, -10212, 0x232316 }, { -3116, 423, -3047, 0x18180F }, { -4274, 500, -4178, 0x2C2C1C },
    { -5431, 484, -5310, 0x1F1F14 }, { -6589, 498, -6442, 0x343421 }, { -7746, 498, -7573, 0x31311F }, { -8903, 760, -8705, 0x1B1B11 },
    { -3388, 345, -2519, 0x242417 }, { -4646, 409, -3455, 0x393924 }, { -5905, 476, -4391, 0x1E1E13 }, { -7163, 393, -5327, 0x212115 },
    { -8422, 513, -6262, 0x2E2E1D }, { -9680, 760, -7198, 0x15150D }, { -3488, 349, -1935, 0x28281A }, { -4784, 485, -2653, 0x333321 },
    { -6080, 519, -3372, 0x282819 }, { -7376, 377, -4090, 0x272719 }, { -8671, 399, -4809, 0x1F1F14 }, { -9967, 760, -5527, 0x3D3D27 },
    { -3589, 424, -1350, 0x202015 }, { -4922, 407, -1851, 0x1A1A11 }, { -6255, 538, -2353, 0x373723 }, { -7588, 491, -2854, 0x2A2A1B },
    { -8921, 553, -3356, 0x3F3F28 }, { -10254, 760, -3857, 0x15150D }, { -3509, 367, -762, 0x27271A }, { -4812, 380, -1045, 0x353524 },
    { -6116, 483, -1328, 0x212115 }, { -7419, 462, -1611, 0x41412A }, { -8723, 399, -1894, 0x404029 }, { -10026, 760, -2177, 0x373723 },
    { -3429, 343, -174, 0x2E2E1F }, { -4703, 399, -239, 0x393928 }, { -5977, 487, -304, 0x3C3C26 }, { -7251, 375, -369, 0x222215 },
    { -8524, 507, -433, 0x272719 }, { -9798, 760, -498, 0x2F2F1E }, { -3191, 374, 331, 0x16160F }, { -4376, 428, 454, 0x44442B },
    { -5562, 395, 577, 0x45452A }, { -6747, 387, 700, 0x2F2F1E }, { -7932, 456, 823, 0x1C1C11 }, { -9117, 760, 946, 0x29291A },
    { -2953, 326, 837, 0x020202 }, { -4050, 560, 1147, 0x33331F }, { -5147, 372, 1458, 0x4C4C2E }, { -6243, 514, 1769, 0x3B3B25 },
    { -7340, 461, 2079, 0x1B1B11 }, { -8437, 760, 2390, 0x2A2A1A }, { -2613, 325, 1203, 0x040402 }, { -3583, 539, 1650, 0x535339 },
    { -4553, 401, 2097, 0x40402A }, { -5524, 449, 2544, 0x3A3A25 }, { -6494, 487, 2991, 0x363620 }, { -7464, 760, 3439, 0x2F2F1D },
    { -2272, 323, 1570, 0x0F0F0A }, { -3116, 349, 2154, 0x0E0E09 }, { -3960, 343, 2737, 0x323222 }, { -4804, 548, 3320, 0x404027 },
    { -5648, 518, 3904, 0x383820 }, { -6492, 760, 4487, 0x1E1E13 }, { -1877, 240, 1796, 0x11110B }, { -2574, 279, 2464, 0x040402 },
    { -3271, 477, 3131, 0x2A2A1D }, { -3969, 629, 3798, 0x616148 }, { -4666, 586, 4465, 0x4F4F31 }, { -5363, 760, 5133, 0x222210 },
    { -1482, 241, 2022, 0x11110B }, { -2032, 105, 2773, 0x020201 }, { -2583, 421, 3525, 0x252519 }, { -3133, 612, 4276, 0x393929 },
    { -3684, 633, 5027, 0x4C4C35 }, { -4234, 760, 5778, 0x34341C }, { -1104, 170, 2127, 0x0F0F0A }, { -1514, 103, 2917, 0x000000 },
    { -1924, 396, 3707, 0x2E2E1C }, { -2334, 522, 4497, 0x333320 }, { -2744, 667, 5287, 0x202013 }, { -3153, 760, 6077, 0x303018 },
    { -726, 207, 2231, 0x0D0D08 }, { -995, 135, 3060, 0x060604 }, { -1265, 402, 3889, 0x3C3C23 }, { -1534, 490, 4718, 0x393921 },
    { -1804, 597, 5546, 0x2A2A19 }, { -2073, 760, 6375, 0x101008 }, { -362, 177, 2248, 0x0E0E09 }, { -497, 113, 3083, 0x030302 },
    { -632, 352, 3917, 0x4A4A2A }, { -766, 487, 4752, 0x1D1D12 }, { -901, 496, 5587, 0x333320 }, { -1035, 760, 6422, 0x313121 },
    { 1, 146, 2264, 0x0A0A07 }, { 1, 104, 3105, 0x030302 }, { 1, 285, 3946, 0x4A4A34 }, { 1, 484, 4787, 0x686848 },
    { 2, 485, 5628, 0x393921 }, { 2, 760, 6469, 0x292919 }, { 349, 151, 2262, 0x0A0A07 }, { 479, 111, 3102, 0x040403 },
    { 609, 305, 3943, 0x3F3F2B }, { 739, 484, 4783, 0x373722 }, { 868, 596, 5623, 0x33331E }, { 998, 760, 6464, 0x2B2B19 },
    { 698, 178, 2260, 0x0C0C07 }, { 957, 109, 3100, 0x030302 }, { 1216, 390, 3939, 0x30301D }, { 1476, 564, 4779, 0x2E2E1A },
    { 1735, 734, 5618, 0x33331D }, { 1994, 760, 6458, 0x212110 }, { 1100, 218, 2141, 0x11100A }, { 1509, 180, 2937, 0x000000 },
    { 1918, 187, 3732, 0x696941 }, { 2327, 264, 4527, 0x4E4E2F }, { 2735, 590, 5323, 0x141B12 }, { 3144, 760, 6118, 0x292921 },
    { 1503, 199, 2022, 0x0E0E09 }, { 2061, 130, 2773, 0x0A0A06 }, { 2619, 148, 3525, 0x6A6242 }, { 3178, 187, 4276, 0x505030 },
    { 3736, 785, 5027, 0x1C1C12 }, { 4294, 760, 5778, 0x191911 }, { 1913, 226, 1743, 0x11110B }, { 2623, 273, 2391, 0x020202 },
    { 3333, 347, 3038, 0x2E2D1C }, { 4044, 451, 3686, 0x34341E }, { 4754, 579, 4333, 0x404024 }, { 5465, 760, 4981, 0x1C1C12 },
    { 2322, 312, 1464, 0x0D0D09 }, { 3185, 358, 2008, 0x1A1A11 }, { 4047, 455, 2552, 0x45452E }, { 4910, 373, 3096, 0x212115 },
    { 5772, 569, 3640, 0x1C1C12 }, { 6635, 760, 4184, 0x212110 }, { 2658, 316, 1040, 0x010101 }, { 3645, 590, 1426, 0x232317 },
    { 4632, 346, 1812, 0x505030 }, { 5619, 418, 2199, 0x262614 }, { 6606, 529, 2585, 0x1F1F13 }, { 7593, 760, 2971, 0x2A2A19 },
    { 2993, 370, 615, 0x0B0B07 }, { 4104, 497, 844, 0x3C3C23 }, { 5216, 383, 1072, 0x4A4A2B }, { 6328, 388, 1301, 0x2E2E1B },
    { 7439, 467, 1529, 0x1D1D11 }, { 8551, 760, 1758, 0x33331F }, { 3203, 362, 60, 0x17170F }, { 4392, 395, 83, 0x393929 },
    { 5582, 472, 105, 0x2A2A1A }, { 6772, 347, 127, 0x272718 }, { 7961, 436, 150, 0x1C1C11 }, { 9151, 760, 172, 0x2D2D1B },
    { 3413, 379, -495, 0x242418 }, { 4680, 368, -679, 0x323222 }, { 5948, 475, -863, 0x29291A }, { 7216, 446, -1046, 0x373722 },
    { 8483, 348, -1230, 0x272718 }, { 9751, 760, -1414, 0x1F1F13 }, { 3446, 414, -1087, 0x27271A }, { 4727, 419, -1491, 0x27271A },
    { 6007, 499, -1895, 0x252517 }, { 7287, 490, -2299, 0x2F2F1D }, { 8567, 403, -2703, 0x2D2D1C }, { 9847, 760, -3106, 0x343420 },
    { 3480, 360, -1680, 0x14140D }, { 4773, 359, -2304, 0x2C2C1C }, { 6065, 507, -2927, 0x373723 }, { 7358, 498, -3551, 0x292919 },
    { 8650, 389, -4175, 0x1B1B11 }, { 9943, 760, -4799, 0x14130C }, { 3334, 466, -2255, 0x1E1E13 }, { 4572, 463, -3092, 0x2B2B1C },
    { 5810, 416, -3930, 0x232316 }, { 7049, 515, -4767, 0x2D2D1D }, { 8287, 515, -5605, 0x31311F }, { 9525, 760, -6442, 0x252517 },
    { 3187, 495, -2830, 0x232316 }, { 4371, 317, -3881, 0x343421 }, { 5555, 423, -4932, 0x14140C }, { 6739, 484, -5984, 0x15150D },
    { 7923, 483, -7035, 0x272718 }, { 9107, 760, -8086, 0x1E1E13 }, { 2875, 453, -3335, 0x30301F }, { 3943, 385, -4573, 0x1C1C12 },
    { 5011, 336, -5812, 0x212115 }, { 6079, 393, -7050, 0x32321F }, { 7147, 381, -8289, 0x282819 }, { 8215, 760, -9527, 0x262618 },
    { 2563, 468, -3839, 0x14140D }, { 3515, 321, -5265, 0x282819 }, { 4466, 382, -6691, 0x2B2B1B }, { 5418, 396, -8117, 0x2A2A1B },
    { 6370, 448, -9543, 0x1C1C11 }, { 7322, 760, -10969, 0x1D1D12 }, { 2113, 361, -4226, 0x272719 }, { 2898, 377, -5796, 0x19180F },
    { 3683, 411, -7366, 0x1A1A10 }, { 4467, 402, -8935, 0x14140D }, { 5252, 485, -10505, 0x343421 }, { 6037, 760, -12075, 0x18180F },
    { 1663, 482, -4613, 0x31311F }, { 2281, 317, -6327, 0x262618 }, { 2899, 435, -8040, 0x252518 }, { 3516, 431, -9754, 0x15150D },
    { 4134, 363, -11467, 0x2B2B1B }, { 4752, 760, -13181, 0x2F2F1D },
};

static void sand_corner(s32 col, s32 row, s16 *c, u32 *colour) {
    const SandCorner *p = &BeachSand[col * (SAND_ROWS + 1) + row];

    c[0] = p->x;
    c[1] = p->y;
    c[2] = p->z;
    *colour = p->colour;
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
    for (col = 0; col < SAND_COLUMNS - 1; col++) {
        for (row = 0; row < SAND_ROWS; row++) {
            sand_corner(col, row + 1, c[0], &colours[0]);
            sand_corner(col + 1, row + 1, c[1], &colours[1]);
            sand_corner(col, row, c[2], &colours[2]);
            sand_corner(col + 1, row, c[3], &colours[3]);
            ring_quad(ts, SAND_IMAGE, c, colours, name++, owner, 0);
        }
    }
    for (seg = 0; seg < BEACH_COLUMNS - 1; seg++) {
        const WallColumn *a = &BeachWall[seg], *b = &BeachWall[seg + 1];

        /* above it all, its art's top row carried on up, as the view sees over its top at the sides */
        c[0][0] = a->tx;
        c[0][1] = BEACH_WALL_TOP;
        c[0][2] = a->tz;
        c[1][0] = b->tx;
        c[1][1] = BEACH_WALL_TOP;
        c[1][2] = b->tz;
        c[2][0] = a->tx;
        c[2][1] = BEACH_WALL_JOIN;
        c[2][2] = a->tz;
        c[3][0] = b->tx;
        c[3][1] = BEACH_WALL_JOIN;
        c[3][2] = b->tz;
        colours[0] = colours[1] = colours[2] = colours[3] = BeachRowColour[0];
        ring_quad(ts, BeachTiles[seg][1], c, colours, name++, owner, 1);
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
            ring_quad(ts, BeachTiles[seg][band == 0 ? 1 : 0], c, colours, name++, owner, 0);
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
