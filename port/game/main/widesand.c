/* A level's floor carried on past where it stops, for a wide view. It's made as the level loads, from the floor's own faces, so nothing of the
 * disc's is copied into this code.
 *
 * It's one surface: a grid of heights and colours, with one height at each spot, so it can't fold over itself. Under the floor the grid has the floor's own
 * values, and just past its edge the edge's own, which is why there's no line where they meet. Further out it's the smoothest thing between that edge and
 * the rim. Then bumps and patches of light grow in as it leaves the edge, at the size the floor's own have. It's drawn behind everything, so the floor itself
 * covers it wherever the two overlap. */
#include <math.h>
#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/engine.h"
#include "bash/widesand.h"
#include "../../platform/gfx.h"

void draw_owner(const struct subengine *sub);
void draw_owner_part(const void *mesh);
extern s32 FarColor[3];

#define SAND_CELL 250
/* The tile mirrors every this many cells, as the level's own floor's does, so its edges always meet: its texels are about the size of the floor's. */
#define SAND_CELLS_A_TILE 4
/* How far past the floor's edge the grid has the edge's own values. */
#define SAND_BAND (SAND_CELL * 3 / 2)
/* What the floor does, measured from its own faces: how far apart its corners are, how much they rise and fall and how much their light changes. */
#define SAND_FACET 1100.0f
#define SAND_BUMP 110.0f
#define SAND_PATCH 0.42f
/* How far from the edge the bumps take to grow in. */
#define SAND_BUMP_GROWS 900.0f
/* The bumps die away over this far before the rim, so its far edge is a clean line. */
#define SAND_RIM_CALM 900.0f
#define SAND_SMOOTHING 400
/* The best over-relaxation for a grid this size: it settles in a tenth of the steps. */
#define SAND_RELAX 1.9f
/* How far past the screen to keep drawing a cell, so one doesn't arrive late at a frame rate above the game's. */
#define SAND_SCREEN_MARGIN 64.0f

/* y, then red, green and blue. */
#define SAND_VALUES 4

typedef struct {
    float x, z;
    float v[SAND_VALUES];
} SandPoint;

typedef struct {
    SandPoint c[3];
} SandFace;

typedef struct {
    SandPoint a, b;
} SandEdge;

static s32 MadeFile = -1;
static s32 Made;
static s32 Width, Depth, MinX, MinZ;
static s16 *Heights;
static u32 *Colours;
static ViewPoint *Points;
static s32 *Cells, *Used;
static s32 NumCells, NumUsed;

/* The faces of a mesh with either tile. */
static s32 read_floor(const ModelFile *m, const MeshHeader *h, const WideSand *sand, SandFace *out) {
    const u32 *colours = (const u32 *)((const u8 *)&m->colors + m->colors);
    const u16 *colour = (const u16 *)((const u8 *)&h->colors + h->colors);
    const u16 *tex = (const u16 *)((const u8 *)&h->texs + h->texs);
    const u8 *run = (const u8 *)&h->runs + h->runs;
    const s16 *vert = (const s16 *)((const u8 *)&h->verts + h->verts + sizeof(BoundBox));
    s32 left = 0, count = 0, at = 0;
    u32 t = 0;

    for (; run[1] != 0xFF; run += 2) {
        s32 k;

        for (k = 0; k < run[1]; k++, colour++) {
            s32 image;

            if (--left < 0) {
                t = *tex++;
                left = (t >> 9) & 0x3F;
            }
            image = t & 0x1FF;
            if (!(run[0] & 1) && !(t & 0x8000) && (image == sand->image_a || image == sand->image_b)) {
                s32 j;

                for (j = 0; j < 3; j++) {
                    const s16 *p = vert + (at + k + j) * 4;
                    u32 c = colours[(*colour & 0x1FFF) + j];
                    SandPoint *o = &out[count].c[j];

                    o->x = p[0];
                    o->z = p[2];
                    o->v[0] = p[1];
                    o->v[1] = (float)(c & 255);
                    o->v[2] = (float)((c >> 8) & 255);
                    o->v[3] = (float)((c >> 16) & 255);
                }
                count++;
            }
        }
        at += run[1] + 2;
    }
    return count;
}

static s32 same_point(const SandPoint *a, const SandPoint *b) {
    return a->x == b->x && a->z == b->z && a->v[0] == b->v[0];
}

/* The edges only one face has: the floor's outline. */
static s32 outline_of(const SandFace *faces, s32 count, SandEdge *out) {
    s32 found = 0;
    s32 f, e;

    for (f = 0; f < count; f++) {
        for (e = 0; e < 3; e++) {
            const SandPoint *a = &faces[f].c[e], *b = &faces[f].c[(e + 1) % 3];
            s32 shared = 0;
            s32 g, h;

            for (g = 0; g < count && !shared; g++) {
                for (h = 0; h < 3 && g != f; h++) {
                    const SandPoint *c = &faces[g].c[h], *d = &faces[g].c[(h + 1) % 3];

                    shared |= (same_point(a, d) && same_point(b, c)) || (same_point(a, c) && same_point(b, d));
                }
            }
            if (!shared) {
                out[found].a = *a;
                out[found].b = *b;
                found++;
            }
        }
    }
    return found;
}

static float nearest_on_outline(float x, float z, const SandEdge *edges, s32 count, float *values) {
    float best = 1e30f;
    s32 e, i;

    for (e = 0; e < count; e++) {
        const SandEdge *o = &edges[e];
        float dx = o->b.x - o->a.x, dz = o->b.z - o->a.z;
        float len = dx * dx + dz * dz;
        float t = len > 0 ? ((x - o->a.x) * dx + (z - o->a.z) * dz) / len : 0;
        float qx, qz, d;

        t = t < 0 ? 0 : t > 1 ? 1 : t;
        qx = o->a.x + dx * t;
        qz = o->a.z + dz * t;
        d = (x - qx) * (x - qx) + (z - qz) * (z - qz);
        if (d < best) {
            best = d;
            for (i = 0; i < SAND_VALUES; i++) {
                values[i] = o->a.v[i] + (o->b.v[i] - o->a.v[i]) * t;
            }
        }
    }
    return sqrtf(best);
}

static s32 on_floor(float x, float z, const SandFace *faces, s32 count, float *values) {
    s32 f, i;

    for (f = 0; f < count; f++) {
        const SandPoint *a = &faces[f].c[0], *b = &faces[f].c[1], *c = &faces[f].c[2];
        float d = (b->z - c->z) * (a->x - c->x) + (c->x - b->x) * (a->z - c->z);
        float p, q, r;

        if (d == 0) {
            continue;
        }
        p = ((b->z - c->z) * (x - c->x) + (c->x - b->x) * (z - c->z)) / d;
        q = ((c->z - a->z) * (x - c->x) + (a->x - c->x) * (z - c->z)) / d;
        r = 1 - p - q;
        if (p >= 0 && q >= 0 && r >= 0) {
            for (i = 0; i < SAND_VALUES; i++) {
                values[i] = a->v[i] * p + b->v[i] * q + c->v[i] * r;
            }
            return 1;
        }
    }
    return 0;
}

static s32 inside_rim(float x, float z, const s16 (*rim)[3], s32 points) {
    s32 inside = 0;
    s32 i, j;

    for (i = 0, j = points - 1; i < points; j = i++) {
        float xi = rim[i][0], zi = rim[i][1], xj = rim[j][0], zj = rim[j][1];

        if ((zi > z) != (zj > z) && x < (xj - xi) * (z - zi) / (zj - zi) + xi) {
            inside = !inside;
        }
    }
    return inside;
}

/* How far from the rim, and its height at the nearest place. */
static float distance_to_rim(float x, float z, const s16 (*rim)[3], s32 points, float *y) {
    float best = 1e30f;
    s32 i, j;

    for (i = 0, j = points - 1; i < points; j = i++) {
        float dx = (float)rim[i][0] - rim[j][0], dz = (float)rim[i][1] - rim[j][1];
        float len = dx * dx + dz * dz;
        float t = len > 0 ? ((x - rim[j][0]) * dx + (z - rim[j][1]) * dz) / len : 0;
        float qx, qz, d;

        t = t < 0 ? 0 : t > 1 ? 1 : t;
        qx = rim[j][0] + dx * t;
        qz = rim[j][1] + dz * t;
        d = (x - qx) * (x - qx) + (z - qz) * (z - qz);
        if (d < best) {
            best = d;
            *y = rim[j][2] + (float)(rim[i][2] - rim[j][2]) * t;
        }
    }
    return sqrtf(best);
}

static float grow(float t) {
    t = t < 0 ? 0 : t > 1 ? 1 : t;
    return t * t * (3 - 2 * t);
}

static float noise_corner(s32 x, s32 z, u32 seed) {
    u32 h = ((u32)x * 73856093u) ^ ((u32)z * 19349663u) ^ (seed * 83492791u);

    h ^= h >> 13;
    h *= 1274126177u;
    h ^= h >> 16;
    return (float)(h & 0xFFFF) / 32767.5f - 1.0f;
}

/* Flat facets, as the floor's faces are, not lined up with the grid. */
static float facet_noise(float x, float z, u32 seed) {
    float tx = (x * 0.89f + z * 0.45f) / SAND_FACET, tz = (z * 0.89f - x * 0.45f) / SAND_FACET;
    s32 ix = (s32)floorf(tx), iz = (s32)floorf(tz);
    float fx = tx - (float)ix, fz = tz - (float)iz;
    float b = noise_corner(ix + 1, iz, seed), c = noise_corner(ix, iz + 1, seed);

    if (fx + fz < 1) {
        float a = noise_corner(ix, iz, seed);

        return a + (b - a) * fx + (c - a) * fz;
    } else {
        float d = noise_corner(ix + 1, iz + 1, seed);

        return d + (c - d) * (1 - fx) + (b - d) * (1 - fz);
    }
}

static void forget_sand(void) {
    free(Heights);
    free(Colours);
    free(Points);
    free(Cells);
    free(Used);
    Heights = NULL;
    Colours = NULL;
    Points = NULL;
    Cells = NULL;
    Used = NULL;
    Made = 0;
    NumCells = 0;
    NumUsed = 0;
}

/* The smoothest thing between what's fixed: each free point the average of the four round it, until they settle. */
static void smooth_grid(float *values, const u8 *fixed) {
    s32 step, i, j, n;

    for (step = 0; step < SAND_SMOOTHING; step++) {
        for (j = 1; j < Depth - 1; j++) {
            for (i = 1; i < Width - 1; i++) {
                s32 at = i + j * Width;

                if (!fixed[at]) {
                    for (n = 0; n < SAND_VALUES; n++) {
                        float *v = &values[at * SAND_VALUES + n];
                        float round = v[-SAND_VALUES] + v[SAND_VALUES] + v[-Width * SAND_VALUES] + v[Width * SAND_VALUES];

                        *v += SAND_RELAX * (round * 0.25f - *v);
                    }
                }
            }
        }
    }
}

static void make_sand(const map_enviroment *scene, const WideSand *sand, const s16 (*rim)[3], s32 rim_points) {
    const ModelFile *m = (const ModelFile *)scene->dbase->data;
    const subengine *sub;
    const MeshHeader *h;
    SandFace *faces;
    SandEdge *edges;
    float *values, *distances, *calm;
    u8 *fixed, *hidden, *used;
    s32 tile = SAND_CELL * SAND_CELLS_A_TILE;
    s32 margin = SAND_CELL * 4;
    s32 num_faces, num_edges, count;
    s32 min_x = 0x7FFF, max_x = -0x7FFF, min_z = 0x7FFF, max_z = -0x7FFF;
    s32 i, j, n;

    if (sand->piece >= scene->num_placements) {
        return;
    }
    sub = &scene->subs[sand->piece];
    h = map_entity_address(sub->entity, (ModelFile *)m);
    if (h == NULL || h->faces == 0) {
        return;
    }
    faces = malloc(sizeof(SandFace) * h->faces);
    edges = malloc(sizeof(SandEdge) * h->faces * 3);
    if (faces == NULL || edges == NULL) {
        free(faces);
        free(edges);
        return;
    }
    num_faces = read_floor(m, h, sand, faces);
    num_edges = outline_of(faces, num_faces, edges);
    for (i = 0; i < rim_points; i++) {
        min_x = rim[i][0] < min_x ? rim[i][0] : min_x;
        max_x = rim[i][0] > max_x ? rim[i][0] : max_x;
        min_z = rim[i][1] < min_z ? rim[i][1] : min_z;
        max_z = rim[i][1] > max_z ? rim[i][1] : max_z;
    }
    /* on the tile's mirror lines */
    MinX = (s32)floorf((float)(min_x - margin) / tile) * tile;
    MinZ = (s32)floorf((float)(min_z - margin) / tile) * tile;
    Width = (max_x + margin - MinX) / SAND_CELL + 2;
    Depth = (max_z + margin - MinZ) / SAND_CELL + 2;
    count = Width * Depth;
    values = malloc(sizeof(float) * SAND_VALUES * (size_t)count);
    distances = malloc(sizeof(float) * (size_t)count);
    calm = malloc(sizeof(float) * (size_t)count);
    fixed = calloc((size_t)count, 1);
    hidden = calloc((size_t)count, 1);
    used = calloc((size_t)count, 1);
    Heights = malloc(sizeof(s16) * (size_t)count);
    Colours = malloc(sizeof(u32) * (size_t)count);
    Points = malloc(sizeof(ViewPoint) * (size_t)count);
    Cells = malloc(sizeof(s32) * (size_t)count);
    Used = malloc(sizeof(s32) * (size_t)count);
    if (num_edges != 0 && values != NULL && distances != NULL && calm != NULL && fixed != NULL && hidden != NULL && used != NULL && Heights != NULL && Colours != NULL &&
        Points != NULL && Cells != NULL && Used != NULL) {
        for (j = 0; j < Depth; j++) {
            for (i = 0; i < Width; i++) {
                float x = (float)(MinX + i * SAND_CELL), z = (float)(MinZ + j * SAND_CELL);
                s32 at = i + j * Width;
                float *v = &values[at * SAND_VALUES];
                float own[SAND_VALUES];
                float d = nearest_on_outline(x, z, edges, num_edges, v);
                float rim_y;
                float to_rim = distance_to_rim(x, z, rim, rim_points, &rim_y);
                s32 inside = inside_rim(x, z, rim, rim_points);

                distances[at] = d;
                calm[at] = inside ? grow(to_rim / SAND_RIM_CALM) : 0;
                if (!inside || to_rim < SAND_CELL || (d <= SAND_BAND && to_rim < d)) {
                    /* the rim's own: what's behind the floor's far edge stays behind it */
                    v[0] = rim_y;
                    v[1] = (float)(sand->rim_colour & 255);
                    v[2] = (float)((sand->rim_colour >> 8) & 255);
                    v[3] = (float)((sand->rim_colour >> 16) & 255);
                    fixed[at] = 1;
                    hidden[at] = !inside && to_rim > margin / 2;
                } else if (on_floor(x, z, faces, num_faces, own)) {
                    for (n = 0; n < SAND_VALUES; n++) {
                        v[n] = own[n];
                    }
                    fixed[at] = 1;
                    /* covered by the floor itself */
                    hidden[at] = d > SAND_BAND + SAND_CELL;
                } else if (d <= SAND_BAND) {
                    /* the edge's own, by it */
                    fixed[at] = 1;
                }
            }
        }
        smooth_grid(values, fixed);
        for (j = 0; j < Depth; j++) {
            for (i = 0; i < Width; i++) {
                float x = (float)(MinX + i * SAND_CELL), z = (float)(MinZ + j * SAND_CELL);
                s32 at = i + j * Width;
                float *v = &values[at * SAND_VALUES];
                float out = grow((distances[at] - SAND_BAND) / SAND_BUMP_GROWS);
                float light = 1 + SAND_PATCH * (0.6f * facet_noise(x, z, 1) + 0.4f * facet_noise(x + 400, z - 300, 2)) * out;
                u32 colour = 0;

                Heights[at] = (s16)floorf(v[0] + SAND_BUMP * facet_noise(x, z, 1) * out * calm[at] + 0.5f);
                for (n = 0; n < 3; n++) {
                    float c = v[n + 1] * light;

                    colour |= (u32)(c < 0 ? 0 : c > 255 ? 255 : c + 0.5f) << (n * 8);
                }
                Colours[at] = colour;
            }
        }
        for (j = 0; j < Depth - 1; j++) {
            for (i = 0; i < Width - 1; i++) {
                s32 at = i + j * Width;

                if (!(hidden[at] && hidden[at + 1] && hidden[at + Width] && hidden[at + Width + 1])) {
                    Cells[NumCells++] = at;
                    used[at] = 1;
                    used[at + 1] = 1;
                    used[at + Width] = 1;
                    used[at + Width + 1] = 1;
                }
            }
        }
        for (i = 0; i < count; i++) {
            if (used[i]) {
                Used[NumUsed++] = i;
            }
        }
        Made = 1;
    }
    free(values);
    free(distances);
    free(calm);
    free(fixed);
    free(hidden);
    free(used);
    free(faces);
    free(edges);
}

s32 wide_sand_ready(const map_enviroment *scene, s32 file, const WideSand *sand, const s16 (*rim)[3], s32 rim_points) {
    if (MadeFile != file) {
        forget_sand();
        MadeFile = file;
        make_sand(scene, sand, rim, rim_points);
    }
    return Made;
}

s32 wide_sand_height(float x, float z, float *y) {
    float gx = (x - (float)MinX) / SAND_CELL, gz = (z - (float)MinZ) / SAND_CELL;
    s32 i = (s32)floorf(gx), j = (s32)floorf(gz);
    float fx = gx - (float)i, fz = gz - (float)j;
    const s16 *h;

    if (!Made || i < 0 || j < 0 || i >= Width - 1 || j >= Depth - 1) {
        return 0;
    }
    h = &Heights[i + j * Width];
    *y = (h[0] * (1 - fx) + h[1] * fx) * (1 - fz) + (h[Width] * (1 - fx) + h[Width + 1] * fx) * fz;
    return 1;
}

void wide_sand_draw(const map_enviroment *scene, const WideSand *sand, subengine *owner) {
    const obj_texture *t = &scene->dbase->textures.tex[sand->image_a];
    s32 fade = RenderState.modelDp;
    s32 left = -gfx_view_extra(), right = ScreenRect->w + gfx_view_extra();
    s32 mirror = SAND_CELLS_A_TILE * 2;
    s32 first_i = ((MinX / SAND_CELL) % mirror + mirror) % mirror, first_j = ((MinZ / SAND_CELL) % mirror + mirror) % mirror;
    DrawFace f;
    s32 n, c;

    memset(&f, 0, sizeof(f));
    f.image = t->info.image;
    f.palette = t->info.palette;
    for (n = 0; n < NumUsed; n++) {
        s32 at = Used[n];
        s16 p[4];

        p[0] = (s16)(MinX + (at % Width) * SAND_CELL);
        p[1] = Heights[at];
        p[2] = (s16)(MinZ + (at / Width) * SAND_CELL);
        p[3] = 0;
        project_point(p, &Points[at]);
    }
    for (n = 0; n < NumCells; n++) {
        s32 at = Cells[n];
        s32 i = at % Width, j = at / Width;
        ViewPoint q[4];
        s32 corner[4];
        s32 behind = 0, off_left = 0, off_right = 0, off_top = 0, off_bottom = 0;

        corner[0] = at;
        corner[1] = at + 1;
        corner[2] = at + Width;
        corner[3] = at + Width + 1;
        for (c = 0; c < 4; c++) {
            q[c] = Points[corner[c]];
            behind |= q[c].w <= 1;
            off_left += q[c].x < left - SAND_SCREEN_MARGIN;
            off_right += q[c].x > right + SAND_SCREEN_MARGIN;
            off_top += q[c].y < -SAND_SCREEN_MARGIN;
            off_bottom += q[c].y > ScreenRect->h + SAND_SCREEN_MARGIN;
        }
        if (behind || off_left == 4 || off_right == 4 || off_top == 4 || off_bottom == 4) {
            continue;
        }
        for (c = 0; c < 4; c++) {
            s32 mu = (first_i + i + (c & 1)) % mirror, mv = (first_j + j + (c >> 1)) % mirror;
            s32 tu = mu <= SAND_CELLS_A_TILE ? mu : mirror - mu, tv = mv <= SAND_CELLS_A_TILE ? mv : mirror - mv;

            f.colour[c] = fade != 0 ? colour_fade(&Colours[corner[c]], FarColor, fade) : Colours[corner[c]];
            f.uv[c] = (u16)(t->info.uv0 | (tu * (t->w - 1) / SAND_CELLS_A_TILE) | ((tv * (t->h - 1) / SAND_CELLS_A_TILE) << 8));
        }
        draw_owner(owner);
        draw_owner_part((void *)(uintptr_t)(at + 1));
        draw_mesh_quad(RenderState.layer, &f, q);
    }
}
