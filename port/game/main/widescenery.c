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

/* How a quad's tile lies: mirrored, so its edge meets the same edge of the next, or all from its top row. */
#define RING_FLIP_U 1
#define RING_FLIP_V 2
#define RING_TOP_ROW 4

static void ring_quad(const obj_textureset *ts, s32 image, const s16 corners[4][3], const u32 colour[4], s32 name, subengine *owner, s32 lie) {
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
        f.uv[i] = (u16)(t->info.uv0 | (((i & 1) != 0) != ((lie & RING_FLIP_U) != 0) ? t->w - 1 : 0) |
                        ((!(lie & RING_TOP_ROW) && ((i & 2) != 0) != ((lie & RING_FLIP_V) != 0) ? t->h - 1 : 0) << 8));
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
/* The tiles of each segment: below, then above, and whether it's mirrored. Past the backdrop's ends the panorama goes back the way it came, mirrored, so each
   join is a tile's edge against itself or its true neighbour. */
static const s16 BeachTiles[BEACH_COLUMNS - 1][3] = { { 5, 10, 0 }, { 5, 10, 1 }, { 6, 11, 1 }, { 4, 9, 1 }, { 3, 8, 1 }, { 2, 7, 1 }, { 2, 7, 0 }, { 3, 8, 0 }, { 4, 9, 0 }, { 6, 11, 0 }, { 5, 10, 0 }, { 5, 10, 0 }, { 5, 10, 1 }, { 6, 11, 1 }, { 4, 9, 1 }, { 3, 8, 1 }, { 2, 7, 1 }, { 2, 7, 0 } };
/* Its rows' heights and colours, as the level's. */
static const s16 BeachRowY[3] = { -930, -16, 740 };
static const u32 BeachRowColour[3] = { 0x9C5A39, 0x634A18, 0x212108 };
#define BEACH_WALL_JOIN (-880) /* a little under its own top, which is -904 to -930 */
#define BEACH_WALL_TOP (-4000)

/* The sand, all the way round, under the arena and out to the wall: each corner's height and colour blended from the level's nearest floor vertices, so it
   meets the real floor, and keeping some unevenness further off. Columns of SAND_ROWS + 1 corners, from the middle out. An even number of cells, as its tile
   mirrors from each to the next, as the level's own floor does. */
#define SAND_IMAGE 0
#define SAND_ROWS 6
#define SAND_COLUMNS 31

typedef struct {
    s16 x, y, z;
    u32 colour;
} SandCorner;

static const SandCorner BeachSand[SAND_COLUMNS * (SAND_ROWS + 1)] = {
    { 0, 284, -1500, 0x222216 }, { 0, 475, -3760, 0x262618 }, { 0, 347, -6020, 0x2F2F1E }, { 0, 467, -8280, 0x272718 },
    { 0, 391, -10540, 0x31311F }, { 0, 349, -12801, 0x29291A }, { 0, 760, -15061, 0x2C2C1C }, { -312, 428, -1467, 0x252517 },
    { -780, 432, -3671, 0x252518 }, { -1249, 329, -5875, 0x282819 }, { -1717, 488, -8078, 0x2F2F1D }, { -2185, 378, -10282, 0x2E2E1D },
    { -2654, 494, -12485, 0x2F2F1E }, { -3122, 760, -14689, 0x29291A }, { -610, 403, -1370, 0x232316 }, { -1523, 379, -3422, 0x2A2A1B },
    { -2437, 505, -5473, 0x2B2B1C }, { -3350, 400, -7525, 0x2D2C1C }, { -4264, 370, -9576, 0x2C2C1C }, { -5177, 501, -11628, 0x28281A },
    { -6090, 760, -13679, 0x2D2D1D }, { -882, 395, -1214, 0x1E1E13 }, { -2131, 481, -2933, 0x28281A }, { -3380, 472, -4652, 0x29291A },
    { -4629, 490, -6372, 0x32321F }, { -5879, 492, -8091, 0x2F2F1D }, { -7128, 363, -9811, 0x2A2A1B }, { -8377, 760, -11530, 0x2A2A1B },
    { -1115, 319, -1004, 0x212115 }, { -2590, 389, -2332, 0x272719 }, { -4066, 463, -3661, 0x2B2B1B }, { -5541, 385, -4990, 0x2C2C1C },
    { -7017, 507, -6318, 0x31311F }, { -8493, 436, -7647, 0x29291A }, { -9968, 760, -8975, 0x29291A }, { -1299, 330, -750, 0x1E1E13 },
    { -2891, 456, -1669, 0x272719 }, { -4484, 503, -2589, 0x2B2B1C }, { -6076, 368, -3508, 0x2D2D1D }, { -7669, 395, -4428, 0x2F2F1E },
    { -9261, 359, -5347, 0x353521 }, { -10854, 760, -6266, 0x343421 }, { -1427, 412, -464, 0x1B1B11 }, { -3035, 380, -986, 0x1C1C12 },
    { -4644, 498, -1509, 0x353523 }, { -6253, 480, -2032, 0x30301F }, { -7862, 549, -2554, 0x353522 }, { -9470, 505, -3077, 0x2C2C1C },
    { -11079, 760, -3600, 0x2C2C1C }, { -1492, 355, -157, 0x1B1B11 }, { -3035, 372, -319, 0x0C0C08 }, { -4579, 408, -481, 0x373726 },
    { -6122, 447, -643, 0x363623 }, { -7665, 396, -806, 0x373723 }, { -9209, 470, -968, 0x393924 }, { -10752, 760, -1130, 0x353521 },
    { -1492, 334, 157, 0x191910 }, { -2911, 379, 306, 0x040403 }, { -4330, 429, 455, 0x414129 }, { -5748, 383, 604, 0x3B3B25 },
    { -7167, 504, 753, 0x343421 }, { -8586, 394, 902, 0x383823 }, { -10005, 760, 1052, 0x2E2E1D }, { -1427, 316, 464, 0x1A1A10 },
    { -2673, 322, 868, 0x020201 }, { -3919, 591, 1273, 0x2B2B1C }, { -5165, 368, 1678, 0x49492D }, { -6411, 457, 2083, 0x373722 },
    { -7657, 584, 2488, 0x2F2F1D }, { -8903, 760, 2893, 0x31311E }, { -1299, 337, 750, 0x1B1B11 }, { -2346, 358, 1355, 0x0E0E09 },
    { -3394, 488, 1960, 0x474731 }, { -4441, 395, 2564, 0x373725 }, { -5489, 452, 3169, 0x393921 }, { -6536, 555, 3774, 0x32321E },
    { -7584, 760, 4379, 0x343420 }, { -1115, 323, 1004, 0x1D1D12 }, { -1947, 336, 1753, 0x14140D }, { -2780, 320, 2503, 0x010101 },
    { -3612, 595, 3252, 0x383827 }, { -4444, 569, 4002, 0x636345 }, { -5277, 621, 4751, 0x292916 }, { -6109, 760, 5501, 0x2A2A19 },
    { -882, 264, 1214, 0x181810 }, { -1492, 216, 2054, 0x10100A }, { -2102, 103, 2894, 0x010101 }, { -2713, 523, 3734, 0x343424 },
    { -3323, 597, 4574, 0x4E4E38 }, { -3933, 668, 5414, 0x383822 }, { -4544, 760, 6254, 0x36361E }, { -610, 208, 1370, 0x18180F },
    { -1010, 161, 2269, 0x0D0D08 }, { -1410, 144, 3168, 0x080805 }, { -1811, 448, 4067, 0x383821 }, { -2211, 570, 4965, 0x2E2E1B },
    { -2611, 681, 5864, 0x222213 }, { -3011, 760, 6763, 0x292917 }, { -312, 293, 1467, 0x1A1A11 }, { -508, 142, 2389, 0x090906 },
    { -704, 158, 3311, 0x0C0C08 }, { -900, 346, 4233, 0x595930 }, { -1096, 482, 5155, 0x2D2D1B }, { -1292, 502, 6077, 0x2E2E1E },
    { -1488, 760, 6999, 0x252517 }, { 0, 184, 1500, 0x1A1A10 }, { 0, 154, 2414, 0x090905 }, { 0, 156, 3329, 0x0E0E09 },
    { 0, 255, 4243, 0x5F5F46 }, { 0, 481, 5158, 0x58583A }, { 0, 542, 6072, 0x32321E }, { 0, 760, 6987, 0x2E2E1C },
    { 312, 336, 1467, 0x191910 }, { 510, 142, 2397, 0x080805 }, { 707, 158, 3327, 0x0D0D08 }, { 905, 496, 4258, 0x2A2A18 },
    { 1103, 695, 5188, 0x373720 }, { 1300, 683, 6118, 0x2B2B19 }, { 1498, 760, 7048, 0x2A2A18 }, { 610, 274, 1370, 0x1B1A11 },
    { 1015, 183, 2279, 0x0C0C08 }, { 1419, 190, 3187, 0x16160D }, { 1824, 266, 4096, 0x4D4D2F }, { 2228, 533, 5004, 0x232819 },
    { 2632, 657, 5913, 0x25261A }, { 3037, 760, 6821, 0x29291C }, { 882, 223, 1214, 0x19180F }, { 1495, 180, 2058, 0x11110B },
    { 2108, 145, 2902, 0x13130C }, { 2721, 186, 3746, 0x5D5839 }, { 3335, 700, 4590, 0x232316 }, { 3948, 650, 5434, 0x1F1F14 },
    { 4561, 760, 6278, 0x202115 }, { 1115, 279, 1004, 0x18180F }, { 1938, 237, 1745, 0x12120B }, { 2762, 313, 2487, 0x010101 },
    { 3586, 378, 3228, 0x292918 }, { 4409, 587, 3970, 0x3D3D21 }, { 5233, 571, 4712, 0x242417 }, { 6056, 760, 5453, 0x212114 },
    { 1299, 387, 750, 0x1B1B11 }, { 2320, 314, 1340, 0x0D0D08 }, { 3341, 434, 1929, 0x313021 }, { 4363, 397, 2519, 0x313120 },
    { 5384, 501, 3108, 0x252517 }, { 6405, 560, 3698, 0x212111 }, { 7426, 760, 4288, 0x212112 }, { 1427, 400, 464, 0x1A1A10 },
    { 2636, 323, 857, 0x020202 }, { 3846, 628, 1250, 0x1F1F14 }, { 5056, 350, 1643, 0x4B4B2D }, { 6266, 470, 2036, 0x2D2D1A },
    { 7475, 435, 2429, 0x272717 }, { 8685, 760, 2822, 0x292918 }, { 1492, 323, 157, 0x17170F }, { 2863, 377, 301, 0x030302 },
    { 4234, 418, 445, 0x3E3E27 }, { 5605, 468, 589, 0x3B3B24 }, { 6976, 534, 733, 0x33321F }, { 8347, 459, 877, 0x31311E },
    { 9718, 760, 1021, 0x2C2C1B }, { 1492, 314, -157, 0x16160E }, { 2984, 378, -314, 0x090906 }, { 4477, 385, -471, 0x373727 },
    { 5969, 440, -627, 0x353522 }, { 7461, 442, -784, 0x33321F }, { 8954, 464, -941, 0x2E2E1C }, { 10446, 760, -1098, 0x2C2C1B },
    { 1427, 351, -464, 0x181810 }, { 2989, 355, -971, 0x191911 }, { 4551, 384, -1479, 0x313120 }, { 6113, 416, -1986, 0x2E2E1D },
    { 7675, 401, -2494, 0x29291A }, { 9237, 370, -3001, 0x31311E }, { 10799, 760, -3509, 0x292919 }, { 1299, 394, -750, 0x1F1F14 },
    { 2851, 464, -1646, 0x202015 }, { 4403, 468, -2542, 0x262619 }, { 5955, 509, -3438, 0x282819 }, { 7507, 479, -4334, 0x262618 },
    { 9059, 506, -5230, 0x2C2B1B }, { 10611, 760, -6126, 0x2D2D1C }, { 1115, 402, -1004, 0x222115 }, { 2562, 331, -2307, 0x252417 },
    { 4009, 322, -3609, 0x2E2E1D }, { 5456, 369, -4912, 0x2C2C1B }, { 6903, 436, -6215, 0x262618 }, { 8350, 421, -7518, 0x2F2F1E },
    { 9797, 760, -8821, 0x2F2F1E }, { 882, 306, -1214, 0x1E1E13 }, { 2114, 423, -2909, 0x202014 }, { 3345, 506, -4604, 0x272718 },
    { 4577, 340, -6300, 0x2A2A1A }, { 5809, 424, -7995, 0x2A2A1A }, { 7041, 336, -9691, 0x2B2B1B }, { 8273, 760, -11386, 0x2E2E1D },
    { 610, 477, -1370, 0x1F1F14 }, { 1516, 301, -3405, 0x222216 }, { 2422, 474, -5440, 0x252518 }, { 3328, 442, -7475, 0x2F2F1E },
    { 4234, 343, -9510, 0x2E2E1D }, { 5140, 460, -11545, 0x282819 }, { 6046, 760, -13580, 0x2E2E1D }, { 312, 440, -1467, 0x252517 },
    { 783, 423, -3685, 0x242417 }, { 1255, 500, -5903, 0x262518 }, { 1726, 491, -8121, 0x2F2F1E }, { 2197, 404, -10338, 0x2D2D1C },
    { 2669, 463, -12556, 0x2F2F1E }, { 3140, 760, -14774, 0x2D2D1D }, { 0, 284, -1500, 0x222216 }, { 0, 475, -3760, 0x262618 },
    { 0, 347, -6020, 0x2F2F1E }, { 0, 467, -8280, 0x272718 }, { 0, 391, -10540, 0x31311F }, { 0, 349, -12801, 0x29291A },
    { 0, 760, -15061, 0x2C2C1C },
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
            ring_quad(ts, SAND_IMAGE, c, colours, name++, owner, ((col & 1) ? RING_FLIP_U : 0) | ((row & 1) ? 0 : RING_FLIP_V));
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
        ring_quad(ts, BeachTiles[seg][1], c, colours, name++, owner, RING_TOP_ROW | (BeachTiles[seg][2] ? RING_FLIP_U : 0));
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
            ring_quad(ts, BeachTiles[seg][band == 0 ? 1 : 0], c, colours, name++, owner, BeachTiles[seg][2] ? RING_FLIP_U : 0);
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
