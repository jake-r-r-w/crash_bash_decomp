/* Widescreen shows further than some levels were built to be seen, and they stop short, with nothing there. So a level can have more of its own
 * scenery placed round it, which only a wide view draws, behind everything else: copies of its map's pieces, moved and turned. No art here, just
 * where they go.
 *
 * A level's own piece that was never meant to be seen can go too, in a wide view. A level's floor can be carried on past where it stops too (widesand.c),
 * and a few of a piece's own faces can go with the backdrop, to be in real depth with what's added.
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
#include "bash/widesand.h"
#include "../../platform/gfx.h"

void draw_map_object(void *sub, void *arg1, s32 flags, void *arg3);
void draw_owner(const struct subengine *sub);
void draw_owner_part(const void *mesh);
extern s32 FarColor[3];

typedef struct {
    s16 file;      /* the map's model file, by its place in DatIndex */
    s16 piece;     /* the map's placement it copies */
    s32 cx, cz;    /* what it turns round */
    s32 dx, dy, dz;
    s32 turn;      /* 0x1000 is all the way round */
    s32 placed;    /* where the level's own things go, rather than behind everything */
    s32 face;      /* it faces what it turns round, so a row of them makes a curve, rather than keeping its own angle */
    s32 entity;    /* shows this of the level's file instead of what the piece does, or 0. A mesh animation's first frame plays it */
    s32 on_sand;   /* it stands on the level's extended floor (widesand.c), with dy from there */
} WideExtra;

#define FILE_N_BALLISM 651
#define FILE_BEACH_BALL 655
#define FILE_DANTES_DASH 637

static const WideExtra Extras[] = {
    /* kelp and coral on the extra sand, the level's own, in its own order: its fish is left out */
    { FILE_BEACH_BALL, 0, 0, 0, -5641, -20, 2999, 0, 1, 1, 0x4100, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5258, -20, 2124, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -4829, -20, 1115, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5315, -20, 93, 0, 1, 1, 0x4200, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5883, -20, -722, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5072, -20, -894, 0, 1, 1, 0x4300, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -6477, -20, -2358, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5970, -20, 3175, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5888, -20, 1144, 0, 1, 1, 0x4200, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 6551, -20, 0, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5209, -20, 517, 0, 1, 1, 0x4100, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5292, -20, -1070, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 4909, -20, -317, 0, 1, 1, 0x4300, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 6344, -20, -2309, 0, 1, 1, 0x4180, 1 },
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
};
#define NUM_EXTRAS ((s32)(sizeof(Extras) / sizeof(Extras[0])))

/* What a wide view is better without: */
static const struct {
    s16 file, piece;
} Hidden[] = {
    /* the front radars' bases, which go with them (draw_ballism_radars) */
    { FILE_N_BALLISM, 3 },
    { FILE_N_BALLISM, 4 },
    /* the rings round their spikes, only needles from here, as those radars are edge on to the camera */
    { FILE_N_BALLISM, 7 },
    { FILE_N_BALLISM, 8 },
    /* the floor under those bases, which sorts in front of them in places: it goes with the backdrop instead (BallismFloors) */
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

/* A mesh animation's first frame plays on with the game's clock, as the level's own objects do, and stops when it's paused. Each starts somewhere else. */
static s32 extra_frame(const map_enviroment *scene, s32 entity, s32 which) {
    ModelFile *m = (ModelFile *)scene->dbase->data;
    s32 anim = (entity & 0xF80) >> 7;

    if ((entity & 0x7000) == 0x4000 && (entity & 0x7F) == 0 && anim < m->nMeshAnims) {
        const FileEntry *list = (const FileEntry *)((const u8 *)&m->meshAnims + m->meshAnims);

        if (list[anim].frames > 0) {
            return entity + (GameFrameCount + which * 11) % list[anim].frames;
        }
    }
    return entity;
}

/* Between the floor's order and the arena's (the map's placements have 768 and 256), so the floor doesn't cover what stands on the sand beside it, and the arena still does. */
#define EXTRA_ON_SAND_OTZ 512

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
        copy->entity = (u16)extra_frame(scene, e->entity, (s32)(copy - Copies));
    }
    x = from->XYZ.x - e->cx;
    z = from->XYZ.z - e->cz;
    copy->XYZ.x = e->cx + (x * c + z * s) / 0x1000 + e->dx;
    copy->XYZ.y = from->XYZ.y + e->dy;
    copy->XYZ.z = e->cz + (z * c - x * s) / 0x1000 + e->dz;
    if (e->on_sand) {
        float y;

        if (!wide_sand_height((float)(copy->XYZ.x - from->XYZ.x), (float)(copy->XYZ.z - from->XYZ.z), &y)) {
            return;
        }
        copy->XYZ.y = from->XYZ.y + (s32)y + e->dy;
        copy->otz = EXTRA_ON_SAND_OTZ;
    }
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
    /* trying something else: what's hidden goes with what's in the table */
    if (NumTried > 0 || RingTried > 0) {
        return 0;
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
        /* the screen's fade, as the game's own models have it */
        f.colour[i] = RenderState.modelDp != 0 ? colour_fade(&colour[i], FarColor, RenderState.modelDp) : colour[i];
        f.uv[i] = (u16)(t->info.uv0 | (((i & 1) != 0) != ((lie & RING_FLIP_U) != 0) ? t->w - 1 : 0) | ((!(lie & RING_TOP_ROW) && (i & 2) ? t->h - 1 : 0) << 8));
    }
    draw_owner(owner);
    draw_owner_part((void *)(uintptr_t)(name + 1));
    draw_mesh_quad(RenderState.layer, &f, q);
}

/* The view's own axes, as the map's pieces have them, with nothing moved. */
static void ring_begin(map_enviroment *scene, subengine *owner, MATRIX *view) {
    /* where the level's own first piece is: a level's floor can be a few units off the middle, and what's made to meet it has to be too */
    *owner = scene->subs[0];
    owner->next = NULL;
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
   past them they carry on turning as it does. And how high the sand comes on it there: at its foot behind the level's floor, and at its ends, where the view sees
   the far edge of the sand along it, so it carries on the line the far edge of the floor makes on the screen. */
typedef struct {
    s16 tx, tz, mx, mz, bx, bz;
    s16 sand;
} WallColumn;

static const WallColumn BeachWall[] = {
    { -5773, -12693, -5756, -12655, -5740, -12619, 720 },
    { -8175, -10272, -8150, -10241, -8127, -10212, 720 },
    { -9736, -7240, -9708, -7219, -9680, -7198, 720 },
    { -10314, -3879, -10284, -3868, -10254, -3857, 720 },
    { -9855, -500, -9826, -499, -9798, -498, 720 },
    { -8486, 2404, -8461, 2396, -8437, 2390, 720 },
    { -6492, 4568, -6492, 4527, -6492, 4487, 710 },
    { -4234, 5859, -4234, 5818, -4234, 5778, 803 },
    { -2073, 6456, -2073, 6414, -2073, 6375, 843 },
    { 12, 6552, 12, 6511, 2, 6469, 838 },
    { 1994, 6539, 1994, 6497, 1994, 6458, 843 },
    { 4294, 5859, 4294, 5818, 4294, 5778, 803 },
    { 6635, 4265, 6635, 4223, 6635, 4184, 620 },
    { 8598, 1767, 8574, 1762, 8551, 1758, 680 },
    { 9805, -1422, 9777, -1418, 9751, -1414, 720 },
    { 9998, -4826, 9969, -4812, 9943, -4799, 720 },
    { 9157, -8131, 9131, -8107, 9107, -8086, 720 },
    { 7362, -11030, 7341, -10998, 7322, -10969, 720 },
    { 4778, -13255, 4765, -13217, 4752, -13181, 720 },
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
/* Its last column's foot is a little lighter, so what carries on from it starts the same. */
#define BEACH_LAST_FOOT_COLOUR 0x313118
#define BEACH_WALL_JOIN (-880) /* a little under its own top, which is -904 to -930 */
#define BEACH_WALL_TOP (-4000)

/* Its floor carried on, out to the wall's foot. */
static const WideSand BeachFloor = { 0, 0, 1, 0x1E1E10 };

/* The foot of each end of its own wall. Its floor stops short of them, with a gap you can't see in 4:3, so the sand has to come up in front of them. */
static const s16 BeachWallEnds[][2] = { { 252, 253 }, { 262, 263 } };
#define NUM_BEACH_WALL_ENDS ((s32)(sizeof(BeachWallEnds) / sizeof(BeachWallEnds[0])))

/* What N.Ballism's main mesh leaves out: a black side of one of its pillars, which only ever hid in the dark, and its front radars (draw_ballism_radars). */
static const s16 BallismLeftOut[][2] = { { 517, 517 }, { 801, 858 }, { 933, 982 } };
#define NUM_BALLISM_LEFT_OUT ((s32)(sizeof(BallismLeftOut) / sizeof(BallismLeftOut[0])))
#define BALLISM_RADAR_FACES (BallismLeftOut + 1)
#define NUM_BALLISM_RADAR_FACES (NUM_BALLISM_LEFT_OUT - 1)

/* Before one of a level's pieces draws: which of its own faces it leaves out, as they go with the backdrop or would show against it. */
void wide_scenery_piece_faces(const map_enviroment *scene, const subengine *sub) {
    if (gfx_view_extra() > 0 && NumTried == 0 && RingTried == 0 && sub == scene->subs) {
        s32 file = (s32)(scene->dbase->file - DatIndex);

        if (file == FILE_BEACH_BALL) {
            draw_face_list(BeachWallEnds, NUM_BEACH_WALL_ENDS, 0);
        } else if (file == FILE_N_BALLISM) {
            draw_face_list(BallismLeftOut, NUM_BALLISM_LEFT_OUT, 0);
        }
    }
}

static void draw_beach_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;
    s16 foot[BEACH_COLUMNS][3];
    s32 seg, band, name = 0;
    s16 c[4][3];
    u32 colours[4];

    if (ts->tex == NULL) {
        return;
    }
    for (seg = 0; seg < BEACH_COLUMNS; seg++) {
        foot[seg][0] = BeachWall[seg].bx;
        foot[seg][1] = BeachWall[seg].bz;
        foot[seg][2] = BeachWall[seg].sand;
    }
    ring_begin(scene, owner, view);
    if (wide_sand_ready(scene, FILE_BEACH_BALL, &BeachFloor, foot, BEACH_COLUMNS)) {
        wide_sand_draw(scene, &BeachFloor, owner);
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
            if (band == 1 && seg == BEACH_OWN_LAST) {
                colours[2] = BEACH_LAST_FOOT_COLOUR;
            }
            ring_quad(ts, BeachTiles[seg][band == 0 ? 1 : 0], c, colours, name++, owner, BeachTiles[seg][2] ? RING_FLIP_U : 0);
        }
    }
    draw_face_list(BeachWallEnds, NUM_BEACH_WALL_ENDS, 1);
    draw_owner(&scene->subs[0]);
    scene->subs[0].hook(&scene->subs[0], view, 0);
    draw_face_list(NULL, 0, 0);
    draw_behind(0);
    draw_owner(NULL);
}

/* N.Ballism's round wall stops at each side of the arena, where a 4:3 view stops too. It's carried on round with copies of its own last stretch, each the
   mirror image of the one before, as its tiles are from one stretch to the next, so their edges meet. These are the stretch's faces, */
static const s16 BallismStretch[][2] = { { 265, 272 }, { 409, 410 }, { 413, 414 } };
#define NUM_BALLISM_STRETCH ((s32)(sizeof(BallismStretch) / sizeof(BallismStretch[0])))
/* its two ends, */
static const s16 BallismStretchEnds[2][2] = { { 5509, 1765 }, { 5543, 2781 } };
/* and where the copies' ends go: on from where it stops on the right, and from behind where it stops on the left. */
static const s16 BallismRoundRight[][2] = { { 5509, 1765 }, { 5640, 745 }, { 5710, -270 }, { 5640, -1290 }, { 5400, -2280 }, { 5000, -3220 } };
static const s16 BallismRoundLeft[][2] = { { -5000, 2900 }, { -5400, 1950 }, { -5640, 960 }, { -5710, -60 }, { -5640, -1080 }, { -5400, -2070 }, { -5000, -3010 } };
#define NUM_BALLISM_RIGHT ((s32)(sizeof(BallismRoundRight) / sizeof(BallismRoundRight[0])))
#define NUM_BALLISM_LEFT ((s32)(sizeof(BallismRoundLeft) / sizeof(BallismRoundLeft[0])))
/* Under its floor, as the floor has holes where nothing was ever meant to show through them. It stops short of the floor's edge, which is see-through for its last few texels. */
#define BALLISM_PLATE_Y 8
static const s16 BallismPlate[4][2] = { { -2790, 2750 }, { 2750, 2750 }, { -2790, -2750 }, { 2750, -2750 } };

/* What it has in place of the floor under its front corners. */
static const s16 BallismFloors[] = { 9, 10 };
#define NUM_BALLISM_FLOORS ((s32)(sizeof(BallismFloors) / sizeof(BallismFloors[0])))

static subengine BallismCopies[NUM_BALLISM_RIGHT + NUM_BALLISM_LEFT];

static void draw_ballism_round(const map_enviroment *scene, const s16 (*round)[2], s32 count, subengine *copies, MATRIX *view) {
    const subengine *from = &scene->subs[0];
    const s16 *near_end = BallismStretchEnds[0], *far_end = BallismStretchEnds[1];
    float sx = (float)(near_end[0] - far_end[0]), sz = (float)(near_end[1] - far_end[1]);
    float length = sqrtf(sx * sx + sz * sz);
    /* along the stretch, and from it towards the arena */
    float dx = sx / length, dz = sz / length;
    float nx = dz, nz = -dx;
    s32 i;

    for (i = 0; i + 1 < count; i++) {
        const s16 *a = round[i + (i & 1)], *b = round[i + 1 - (i & 1)];
        subengine *copy = &copies[i];
        float wx = (float)(a[0] - b[0]), wz = (float)(a[1] - b[1]);
        float width = sqrtf(wx * wx + wz * wz);
        float tx = wz / width, tz = -wx / width;
        MATRIX spin, turned;

        if (tx * (float)(a[0] + b[0]) + tz * (float)(a[1] + b[1]) > 0) {
            tx = -tx;
            tz = -tz;
        }
        memset(&spin, 0, sizeof(spin));
        spin.m[0][0] = (s16)((wx / length * dx + tx * nx) * 4096.0f);
        spin.m[0][2] = (s16)((wx / length * dz + tx * nz) * 4096.0f);
        spin.m[1][1] = 0x1000;
        spin.m[2][0] = (s16)((wz / length * dx + tz * nx) * 4096.0f);
        spin.m[2][2] = (s16)((wz / length * dz + tz * nz) * 4096.0f);
        *copy = *from;
        copy->next = NULL;
        copy->XYZ.x = from->XYZ.x + a[0] - (spin.m[0][0] * near_end[0] + spin.m[0][2] * near_end[1]) / 0x1000;
        copy->XYZ.z = from->XYZ.z + a[1] - (spin.m[2][0] * near_end[0] + spin.m[2][2] * near_end[1]) / 0x1000;
        matrix_mul(&spin, (MATRIX *)&from->xform.mtx, &turned);
        copy->xform.mtx = turned;
        draw_mirrored((s32)spin.m[0][0] * spin.m[2][2] - (s32)spin.m[0][2] * spin.m[2][0] < 0);
        draw_owner(copy);
        copy->hook(copy, view, 0);
    }
    draw_mirrored(0);
}

static void draw_ballism_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    DrawFace f;
    ViewPoint q[4], t[3];
    s16 p[4];
    s32 i;

    ring_begin(scene, owner, view);
    memset(&f, 0, sizeof(f));
    if (RenderState.modelDp != 0) {
        const u32 black = 0;

        f.colour[0] = f.colour[1] = f.colour[2] = colour_fade(&black, FarColor, RenderState.modelDp);
    }
    draw_owner(owner);
    draw_owner_part(BallismPlate);
    for (i = 0; i < 4; i++) {
        p[0] = BallismPlate[i][0];
        p[1] = BALLISM_PLATE_Y;
        p[2] = BallismPlate[i][1];
        p[3] = 0;
        project_point(p, &q[i]);
    }
    for (i = 0; i < 2; i++) {
        t[0] = q[i];
        t[1] = q[i + 1];
        t[2] = q[i + 2];
        draw_mesh_tri_plain(RenderState.layer, &f, t, i, 0, 0);
    }
    for (i = 0; i < NUM_BALLISM_FLOORS; i++) {
        subengine *floor = &scene->subs[BallismFloors[i]];

        draw_owner(floor);
        floor->hook(floor, view, 0);
    }
    draw_face_list(BallismStretch, NUM_BALLISM_STRETCH, 1);
    draw_ballism_round(scene, BallismRoundRight, NUM_BALLISM_RIGHT, BallismCopies, view);
    draw_ballism_round(scene, BallismRoundLeft, NUM_BALLISM_LEFT, BallismCopies + NUM_BALLISM_RIGHT, view);
    draw_face_list(NULL, 0, 0);
    draw_behind(0);
    draw_owner(NULL);
}

/* Its front radars are in its main mesh, which is painted first, so their bases, painted after it, cover them. The 4:3 view hardly saw that, as it stops by them.
   So they and their bases go in real depth, as the game's solid things do, in the order of where they are. */
static const s16 BallismRadarPieces[] = { 3, 4 };
#define NUM_BALLISM_RADAR_PIECES ((s32)(sizeof(BallismRadarPieces) / sizeof(BallismRadarPieces[0])))

static subengine BallismRadars;

static void draw_ballism_radars(map_enviroment *scene, MATRIX *view) {
    s32 i;

    draw_depth(1);
    BallismRadars = scene->subs[0];
    BallismRadars.next = NULL;
    BallismRadars.otz = scene->subs[BallismRadarPieces[0]].otz;
    draw_face_list(BALLISM_RADAR_FACES, NUM_BALLISM_RADAR_FACES, 1);
    draw_owner(&BallismRadars);
    BallismRadars.hook(&BallismRadars, view, 0);
    draw_face_list(NULL, 0, 0);
    for (i = 0; i < NUM_BALLISM_RADAR_PIECES; i++) {
        subengine *piece = &scene->subs[BallismRadarPieces[i]];

        draw_owner(piece);
        piece->hook(piece, view, 0);
    }
    draw_owner(NULL);
    draw_depth(0);
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
        if (file == FILE_N_BALLISM) {
            draw_ballism_backdrop(scene, &RingOwner, view);
            draw_ballism_radars(scene, view);
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
