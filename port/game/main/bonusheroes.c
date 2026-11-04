/* Bonus heroes (bash/bonusheroes.h, PC only): extra heroes' models from the player's own copies of other Crash games.
 *
 * A disc is found in the discs folders (disc_image_find), the character's level read from it, and its model and animations made into a
 * Crash Bash model and texture set (docs/ASSETS.md), which go into the archive's entries 992 and 993 (main.c) as a mod's files do: the
 * game loads them as it does any model, into a bank of their own (DB_BonusRoo), and nothing of them is kept but in memory.
 *
 * Ripper Roo, from Crash Bandicoot (USA, SCUS-94900): level 0x17, his boss fight, has his model (RS1nG) and four animations, RS1nV (a big
 * jump), RS2nV (standing), RS3nV (a laugh) and RS4nV (knocked over), which become JUMP, BREATHE, TAUNT and DIE, and two more are made
 * for him from them: a walk, his legs stepping (WALK), and a spin attack, his legs out as he spins in a blue tornado (SPIN).
 *
 * Crash Bandicoot's formats, as CrashEdit (the cbhacks project) reads them: a level's NSF is 64 KB pages, each raw (magic 0x1234) or
 * compressed (0x1235: runs and repeats for its first `length` bytes, `skip` bytes to pass, the rest raw). A page of entries lists them,
 * and each entry its items; a texture page is 256 x 128 16-bit units of VRAM. An animation's items are its frames: a 56-byte header
 * (vertex count, model, x, y, z offsets, collision) and 6 bytes a vertex (x, y, z unsigned, a normal signed), each at (xyz + offset -
 * 128) * the model's scale. A model's first item has its triangle count, its scale and its colours (4-byte slots: r, g, b, flags) and
 * textures (3 slots: the colour with byte 3's top bit, the texture page, and where in it), its second 8 bytes a triangle: three
 * vertices' byte offsets and its slot. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/modelbank.h"
#include "bash/extraheroes.h"
#include "bash/bonusheroes.h"
#include "../../platform/platform.h"
#include "../../platform/audio.h"

void archive_replace(int entry, unsigned char *bytes, int size);
void SDL_free(void *mem);

#define PAGE_BYTES 65536
#define BONUS_TEX 992
#define BONUS_MDL 993
#define ROO_HEIGHT 620.0f   /* standing, in Crash Bash's units: Crate Crush's heroes stand 452 (Crash) to 748 (Kong), most about 650 */
#define MAX_TRIS 2048
#define MAX_VERTS 1024
#define MAX_IMAGES 32
#define FLIP_FACES 0        /* 1: the other way round from Crash Bandicoot's */

static u32 rd32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

static u16 rd16(const u8 *p) {
    return (u16)(p[0] | p[1] << 8);
}

static void wr32(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

static void wr16(u8 *p, u32 v) {
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
}

/* --- Crash Bandicoot's levels --- */

typedef struct {
    u8 *pages; /* count of PAGE_BYTES */
    s32 count;
} Level;

static s32 decompress(const u8 *in, size_t size, size_t *at, u8 *out) {
    size_t p = *at;
    s32 length, skip, pos = 0, rest;

    if (p + 12 > size) {
        return 0;
    }
    length = (s32)rd32(in + p + 4);
    skip = (s32)rd32(in + p + 8);
    if (length < 0 || length > PAGE_BYTES || skip < 0) {
        return 0;
    }
    p += 12;
    while (pos < length) {
        s32 prefix;

        if (p >= size) {
            return 0;
        }
        prefix = in[p++];
        if (prefix & 0x80) {
            s32 b, seek, span, i;

            if (p >= size) {
                return 0;
            }
            b = in[p++];
            seek = b >> 3 | (prefix & 0x7F) << 5;
            span = (b & 7) == 7 ? 64 : (b & 7) + 3;
            if (seek > pos || seek == 0 || pos + span > PAGE_BYTES) {
                return 0;
            }
            for (i = 0; i < span; i++) {
                out[pos + i] = out[pos - seek + i];
            }
            pos += span;
        } else {
            if (p + (size_t)prefix > size || pos + prefix > PAGE_BYTES) {
                return 0;
            }
            memcpy(out + pos, in + p, (size_t)prefix);
            p += (size_t)prefix;
            pos += prefix;
        }
    }
    p += (size_t)skip;
    rest = PAGE_BYTES - length < PAGE_BYTES - pos ? PAGE_BYTES - length : PAGE_BYTES - pos;
    if (p + (size_t)(PAGE_BYTES - length) > size) {
        return 0;
    }
    memcpy(out + pos, in + p, (size_t)rest);
    *at = p + (size_t)(PAGE_BYTES - length);
    return 1;
}

static s32 level_read(DiscImage *disc, const char *path, Level *out) {
    size_t size = 0, at = 0;
    u8 *nsf = disc_image_read(disc, path, &size);
    s32 cap = 0;

    memset(out, 0, sizeof(*out));
    if (nsf == NULL) {
        return 0;
    }
    while (at + 12 <= size) {
        u16 magic = rd16(nsf + at);
        u8 *page;

        if (out->count == cap) {
            u8 *more = realloc(out->pages, (size_t)(cap = cap ? cap * 2 : 64) * PAGE_BYTES);

            if (more == NULL) {
                break;
            }
            out->pages = more;
        }
        page = out->pages + (size_t)out->count * PAGE_BYTES;
        if (magic == 0x1234 && at + PAGE_BYTES <= size) {
            memcpy(page, nsf + at, PAGE_BYTES);
            at += PAGE_BYTES;
        } else if (magic != 0x1235 || !decompress(nsf, size, &at, page)) {
            break;
        }
        out->count++;
    }
    SDL_free(nsf);
    return out->count > 0;
}

static u32 eid_of(const char *name) {
    static const char Chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_!";
    u32 eid = 1;
    s32 i;

    for (i = 0; i < 5; i++) {
        const char *c = strchr(Chars, name[i]);

        eid |= (u32)(c != NULL ? c - Chars : 0) << (25 - 6 * i);
    }
    return eid;
}

/* An entry: its items, each with its size. */
typedef struct {
    s32 type, count;
    const u8 *item[128];
    s32 size[128];
} Entry;

static s32 entry_find(const Level *l, const char *name, Entry *out) {
    u32 eid = eid_of(name);
    s32 p, k, i;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * PAGE_BYTES;
        s32 n = (s32)rd32(page + 8);

        /* a page of entries, or of sounds (3) */
        if ((rd16(page + 2) != 0 && rd16(page + 2) != 3) || n < 0 || n > 4096) {
            continue;
        }
        for (k = 0; k < n; k++) {
            u32 s = rd32(page + 16 + k * 4), e = rd32(page + 20 + k * 4);
            const u8 *en = page + s;
            s32 items;

            if (s >= e || e > PAGE_BYTES || e - s < 16 || rd32(en) != 0x100FFFF || rd32(en + 4) != eid) {
                continue;
            }
            items = (s32)rd32(en + 12);
            if (items < 0 || items > 128 || 20 + items * 4 > (s32)(e - s)) {
                return 0;
            }
            out->type = (s32)rd32(en + 8);
            out->count = items;
            for (i = 0; i < items; i++) {
                u32 a = rd32(en + 16 + i * 4), b = rd32(en + 20 + i * 4);

                if (a > b || b > e - s) {
                    return 0;
                }
                out->item[i] = en + a;
                out->size[i] = (s32)(b - a);
            }
            return 1;
        }
    }
    return 0;
}

static const u8 *texture_page(const Level *l, u32 eid) {
    s32 p;

    for (p = 0; p < l->count; p++) {
        const u8 *page = l->pages + (size_t)p * PAGE_BYTES;

        if (rd16(page + 2) == 1 && rd32(page + 4) == eid) {
            return page;
        }
    }
    return NULL;
}

/* --- into Crash Bash's --- */

/* A grown buffer of bytes. */
typedef struct {
    u8 *b;
    size_t n, cap;
} Bytes;

static size_t put(Bytes *o, const void *src, size_t n) {
    size_t at = o->n;

    if (o->n + n > o->cap) {
        size_t cap = o->cap ? o->cap : 4096;
        u8 *more;

        while (cap < o->n + n) {
            cap *= 2;
        }
        more = realloc(o->b, cap);
        if (more == NULL) {
            return at;
        }
        o->b = more;
        o->cap = cap;
    }
    if (src != NULL) {
        memcpy(o->b + o->n, src, n);
    } else {
        memset(o->b + o->n, 0, n);
    }
    o->n += n;
    return at;
}

static void align(Bytes *o, size_t to) {
    if (o->n % to != 0) {
        put(o, NULL, to - o->n % to);
    }
}

/* A self offset: from the field at `field` to `to`. */
static void self(Bytes *o, size_t field, size_t to) {
    wr32(o->b + field, (u32)(s32)((long long)to - (long long)field));
}

typedef struct {
    float x, y, z;
} V3;

/* A texture of Roo's: a rect of a page, in 4 or 8 bits, with its palette, as a Crash Bash image. */
typedef struct {
    u32 page_eid, info;
    s32 clutx;
    s32 x, y, w, h, bpp;
} Image;

typedef struct {
    s32 a, b, c;      /* vertices */
    s32 slot;
    s32 image;        /* -1 untextured */
    s32 uv[3][2];     /* in the image's texels */
    u8 rgb[3];
    u8 glow;          /* the tornado's: its corners' own colours, unlit, drawn adding them */
    u8 corner_rgb[3][3];
} Tri;

typedef struct {
    const char *entry, *name;
    s32 made; /* the frames of one made for him from that entry's (rig_setup), or 0 for the entry's own */
} Anim;

static const Anim RooAnims[] = {
    { "RS1nV", "JUMP", 0 }, { "RS2nV", "BREATHE", 0 }, { "RS3nV", "TAUNT", 0 }, { "RS4nV", "DIE", 0 }, { "RS2nV", "WALK", 20 },
    { "RS1nV", "SPIN", 15 },
};
#define ROO_ANIMS 6
#define ROO_REST 1 /* the one he stands in: his height and his colours' light are from its first frame */
#define ROO_WALK 4 /* made for him, as his game has no walk (he only ever jumps in his boss fight) and no attack */
#define ROO_SPIN 5

/* A frame's vertices, in Crash Bash's axes (y down, z the other way) before they're scaled. */
static s32 frame_verts(const u8 *f, s32 size, const s32 scale[3], V3 *out, V3 *normals, s32 max) {
    s32 n = (s32)rd32(f), k;
    s32 off[3];

    if (n <= 0 || n > max || 56 + n * 6 > size) {
        return 0;
    }
    off[0] = (s32)rd32(f + 8);
    off[1] = (s32)rd32(f + 12);
    off[2] = (s32)rd32(f + 16);
    for (k = 0; k < n; k++) {
        const u8 *v = f + 56 + k * 6;

        out[k].x = (float)(v[0] + off[0] - 128) * scale[0];
        out[k].y = -(float)(v[1] + off[1] - 128) * scale[1];
        out[k].z = -(float)(v[2] + off[2] - 128) * scale[2];
        if (normals != NULL) {
            /* the normal's x is the other way from the position's (CrashEdit's lighting) */
            normals[k].x = -(float)(s8)v[3];
            normals[k].y = -(float)(s8)v[4];
            normals[k].z = -(float)(s8)v[5];
        }
    }
    return n;
}

/* His standing animation (ROO_REST) as his boss fight's, but with only ROO_SWAY of its rocking: each frame's vertices are drawn in
 * toward the middle of its frames, each vertex's mean over them. */
#define ROO_SWAY 0.6f
static V3 SwayMiddle[MAX_VERTS];
static s32 SwayVerts;

static void sway_middle(const Entry *a, const s32 scale[3]) {
    static V3 fv[MAX_VERTS];
    s32 f, k, n;

    SwayVerts = 0;
    memset(SwayMiddle, 0, sizeof(SwayMiddle));
    for (f = 0; f < a->count; f++) {
        n = frame_verts(a->item[f], a->size[f], scale, fv, NULL, MAX_VERTS);
        if (n == 0 || (f > 0 && n != SwayVerts)) {
            SwayVerts = 0;
            return;
        }
        SwayVerts = n;
        for (k = 0; k < n; k++) {
            SwayMiddle[k].x += fv[k].x / a->count;
            SwayMiddle[k].y += fv[k].y / a->count;
            SwayMiddle[k].z += fv[k].z / a->count;
        }
    }
}

/* --- his made animations ---
 * His game has no walk and no attack, so they're made for him from his own frames (ROO_WALK, ROO_SPIN), worked in his game's axes
 * and units, y up and him facing +z: frame_verts' over ROO_UNIT, y and z the other way round. Both start from his standing pose,
 * SwayMiddle. WALK steps: each foot goes back flat along the floor, then up, its knee out in front, and forward again, the body
 * bobbing, leaning over the leg it's on and turning against the legs. SPIN, his attack: he throws his legs out to the sides as his
 * jump does (its frame SPIN_OUT, the splits), spins round twice in a hop in a blue tornado, and lands as he stood. */
#define ROO_UNIT 4096.0f
#define ROO_PI 3.14159265f

/* His hips, and how much of a leg each vertex is: all of it below LEG_FULL, none above LEG_TOP (the bottom of his straitjacket goes
 * with both), and none within LEG_MID of his middle across, all from LEG_SIDE out. His tail, behind him and down to the floor
 * (TAIL_), is his body's. A leg leans from its hip (HIP_Y) to the top of its foot (FOOT_Y), and the foot under that stays flat. */
#define HIP_Y 58.0f
#define HIP_Z 4.0f
#define FOOT_Y 12.0f
#define LEG_TOP 70.0f
#define LEG_FULL 42.0f
#define LEG_MID 5.0f
#define LEG_SIDE 16.0f
#define TAIL_X 14.0f
#define TAIL_Y 30.0f
#define TAIL_Z -15.0f

/* A stride, as each leg goes through it: on the ground for its first STEP_STANCE, its foot going back steadily from STEP_REACH in
 * front of its hip to STEP_REACH behind, then up STEP_LIFT with its knee STEP_KNEE_OUT in front, and forward again. Two strides an
 * animation, a step 5 ticks of his base's 20-tick run: his legs are short, and so many steps keep his feet up with the floor as he
 * goes (Tiny's, a stride a run, go back as fast as he moves). The body bobs, leans over the leg it's on, and turns against the legs. */
#define STEP_STRIDES 2
#define STEP_STANCE 0.6f
#define STEP_REACH 44.0f
#define STEP_LIFT 22.0f
#define STEP_KNEE_OUT 26.0f
#define STEP_BOB 4.0f
#define STEP_ROLL 0.10f
#define STEP_TWIST 0.10f

/* The spin: his jump's frame with his legs out, the turns, the hop (extraheroes.c draws a third of a frame's lift, KEEP_LIFT), the
 * frames he takes to throw his legs out and bring them in, and the one he's landed by: the base's attack shows only its first 14. */
#define SPIN_OUT 14
#define SPIN_TURNS 2
#define SPIN_LIFT 150.0f
#define SPIN_OPEN 3
#define SPIN_LAND 13
#define SPIN_BODY 70.0f /* he spins round the middle of what's above this */

/* The tornado round his spin, as Crash's and Coco's spins have (their models' second mesh), but blue and glowing: streaks of faces,
 * drawn adding their colour (a face's colour index, bit 15 and blend mode 1), rising round him from his legs to his head, wide at
 * the bottom and narrower up, each bright at its front and fading behind, whirling round faster than he turns. They're vertices of
 * his mesh like the rest, all at one point (so nothing to draw) in any other frame, and open out and fold away with his legs. */
#define TORN_STREAKS 8
#define TORN_SEGS 14
#define TORN_VERTS (TORN_STREAKS * (TORN_SEGS + 1) * 2)
#define TORN_TRIS (TORN_STREAKS * TORN_SEGS * 4) /* two a quad, each side */
#define TORN_STEP 24.0f    /* how much higher each streak starts */
#define TORN_RISE 46.0f    /* and rises along itself */
#define TORN_HALF 7.0f     /* half its width */
#define TORN_BOTTOM 126.0f /* the radius at the floor, and at TORN_TOP */
#define TORN_RADIUS_TOP 66.0f
#define TORN_TOP 210.0f
#define TORN_SPAN 3.6f     /* the way round a streak goes (radians), the way it turns a frame, and how far round each starts from the last */
#define TORN_SPEED 1.75f
#define TORN_PHASE 2.2f
#define TORN_RAMP 1.6f     /* how a streak's brightness rises to its front */
static const u8 TornRgb[3] = { 60, 150, 255 };

static V3 Rest[MAX_VERTS], LegsOut[MAX_VERTS];
static float LegShare[MAX_VERTS];
static u8 LegOf[MAX_VERTS]; /* 0 his left (-x), 1 his right */
static s32 RigVerts;
static float SpinX, SpinZ;

static V3 to_rig(V3 p) {
    V3 r;

    r.x = p.x / ROO_UNIT;
    r.y = -p.y / ROO_UNIT;
    r.z = -p.z / ROO_UNIT;
    return r;
}

static V3 from_rig(V3 r) {
    V3 p;

    p.x = r.x * ROO_UNIT;
    p.y = -r.y * ROO_UNIT;
    p.z = -r.z * ROO_UNIT;
    return p;
}

/* 0 at a, 1 at b (either way round), smoothly between */
static float smooth(float a, float b, float x) {
    float t = (x - a) / (b - a);

    t = t < 0 ? 0 : t > 1 ? 1 : t;
    return t * t * (3 - 2 * t);
}

static V3 mix(V3 a, V3 b, float t) {
    V3 r;

    r.x = a.x + (b.x - a.x) * t;
    r.y = a.y + (b.y - a.y) * t;
    r.z = a.z + (b.z - a.z) * t;
    return r;
}

/* p turned a radians round an axis through c: up (y) or forward (z) */
static V3 turn_y(V3 p, V3 c, float a) {
    float s = sinf(a), k = cosf(a), x = p.x - c.x, z = p.z - c.z;

    p.x = c.x + x * k + z * s;
    p.z = c.z - x * s + z * k;
    return p;
}

static V3 turn_z(V3 p, V3 c, float a) {
    float s = sinf(a), k = cosf(a), x = p.x - c.x, y = p.y - c.y;

    p.x = c.x + x * k - y * s;
    p.y = c.y + x * s + y * k;
    return p;
}

/* His standing pose (sway_middle first), each vertex's share of a leg and its knee, and his jump's legs out, on the same floor. */
static void rig_setup(const Entry *jump, const s32 scale[3]) {
    static V3 fv[MAX_VERTS];
    float low = 1e9f, out_low = 1e9f, sx = 0, sz = 0;
    s32 k, n, f, body = 0;

    RigVerts = 0;
    if (SwayVerts == 0 || jump->count <= 0) {
        return;
    }
    for (k = 0; k < SwayVerts; k++) {
        V3 p = to_rig(SwayMiddle[k]);
        s32 tail = p.z < TAIL_Z && fabsf(p.x) < TAIL_X && p.y < TAIL_Y;

        Rest[k] = p;
        LegOf[k] = p.x < 0 ? 0 : 1;
        LegShare[k] = tail ? 0 : smooth(LEG_TOP, LEG_FULL, p.y) * smooth(LEG_MID, LEG_SIDE, fabsf(p.x));
        low = p.y < low ? p.y : low;
        if (p.y > SPIN_BODY) {
            sx += p.x;
            sz += p.z;
            body++;
        }
    }
    SpinX = body > 0 ? sx / body : 0;
    SpinZ = body > 0 ? sz / body : 0;
    f = jump->count > SPIN_OUT ? SPIN_OUT : jump->count - 1;
    n = frame_verts(jump->item[f], jump->size[f], scale, fv, NULL, MAX_VERTS);
    for (k = 0; k < SwayVerts; k++) {
        LegsOut[k] = k < n ? to_rig(fv[k]) : Rest[k];
        out_low = LegsOut[k].y < out_low ? LegsOut[k].y : out_low;
    }
    for (k = 0; k < SwayVerts; k++) {
        LegsOut[k].y += low - out_low;
    }
    RigVerts = SwayVerts;
}

/* Where a leg is at p (0 to 1) through its stride: its foot's way in front of its hip, and how far up it is (0 on the ground to 1). */
static void stride(float p, float *z, float *up) {
    if (p < STEP_STANCE) {
        *z = STEP_REACH * (1 - 2 * p / STEP_STANCE);
        *up = 0;
        return;
    }
    p = (p - STEP_STANCE) / (1 - STEP_STANCE);
    *z = STEP_REACH * (2 * smooth(0, 1, p) - 1);
    *up = sinf(ROO_PI * p);
}

static s32 walk_verts(s32 f, V3 *out) {
    float t = (float)STEP_STRIDES * f / RooAnims[ROO_WALK].made, z[2], up[2], bob, roll, twist;
    V3 pelvis = { 0, HIP_Y, HIP_Z };
    s32 k;

    /* his right leg half a stride behind his left */
    stride(t - floorf(t), &z[0], &up[0]);
    stride(t + 0.5f - floorf(t + 0.5f), &z[1], &up[1]);
    bob = STEP_BOB * (up[0] + up[1] - 0.5f);
    roll = STEP_ROLL * (up[0] - up[1]);
    twist = STEP_TWIST * (z[0] - z[1]) / (2 * STEP_REACH);
    for (k = 0; k < RigVerts; k++) {
        V3 p = Rest[k], q, body;
        s32 leg = LegOf[k];
        float w = LegShare[k], d;

        body = turn_y(turn_z(p, pelvis, roll), pelvis, -twist);
        body.y += bob;
        q = mix(p, body, 1 - w);
        if (w > 0) {
            /* how far down the leg, 0 at the hip to 1 at the foot and all of the foot */
            d = (HIP_Y - p.y) / (HIP_Y - FOOT_Y);
            d = d < 0 ? 0 : d > 1 ? 1 : d;
            q.y += STEP_LIFT * up[leg] * d * w;
            q.z += (z[leg] * d + STEP_KNEE_OUT * up[leg] * sinf(ROO_PI * d)) * w;
        }
        out[k] = from_rig(q);
    }
    return RigVerts;
}

/* The tornado's vertices after his n, from frame f of his spin, open of the way out and up off the ground: each streak a strip of
 * TORN_SEGS + 1 pairs, its back end first; or, shut, all at one point inside him. */
static s32 add_tornado(V3 *out, s32 n, s32 f, float open, float up) {
    s32 k, s, at = n;

    for (k = 0; k < TORN_STREAKS; k++) {
        for (s = 0; s <= TORN_SEGS; s++) {
            float u = (float)s / TORN_SEGS, a = k * TORN_PHASE + TORN_SPEED * f + TORN_SPAN * u, y = k * TORN_STEP + TORN_RISE * u;
            float r = (TORN_BOTTOM + (TORN_RADIUS_TOP - TORN_BOTTOM) * y / TORN_TOP) * open, h = TORN_HALF * open;
            V3 lo, hi;

            if (open <= 0) {
                lo.x = hi.x = SpinX;
                lo.y = hi.y = SPIN_BODY + 30;
                lo.z = hi.z = SpinZ;
            } else {
                lo.x = hi.x = SpinX + r * sinf(a);
                lo.z = hi.z = SpinZ + r * cosf(a);
                lo.y = y - h + up;
                hi.y = y + h + up;
            }
            out[at++] = from_rig(lo);
            out[at++] = from_rig(hi);
        }
    }
    return at;
}

static s32 spin_verts(s32 f, V3 *out) {
    float t = f >= SPIN_LAND ? 1 : (float)f / SPIN_LAND;
    float open = fminf(smooth(0, SPIN_OPEN, (float)f), smooth(SPIN_LAND, SPIN_LAND - SPIN_OPEN, (float)f));
    float turn = 2 * ROO_PI * SPIN_TURNS * smooth(0, 1, t), up = SPIN_LIFT * sinf(ROO_PI * t);
    V3 axis = { SpinX, 0, SpinZ };
    s32 k;

    for (k = 0; k < RigVerts; k++) {
        V3 q = turn_y(mix(Rest[k], LegsOut[k], open), axis, turn);

        q.y += up;
        out[k] = from_rig(q);
    }
    return add_tornado(out, RigVerts, f, open, up);
}

/* His animation j's frames, and its frame f as frame_verts gives it, then the tornado's (shut but in his spin): his standing one with
 * less sway, the made ones made. */
static s32 anim_frames(const Entry *a, s32 j) {
    return RooAnims[j].made > 0 && RigVerts > 0 ? RooAnims[j].made : a->count;
}

static s32 anim_verts(const Entry *a, s32 j, s32 f, const s32 scale[3], V3 *out, V3 *normals) {
    s32 n, k;

    if (RooAnims[j].made > 0 && RigVerts > 0) {
        return j == ROO_SPIN ? spin_verts(f, out) : add_tornado(out, walk_verts(f, out), 0, 0, 0);
    }
    f = f < a->count ? f : a->count - 1;
    n = frame_verts(a->item[f], a->size[f], scale, out, normals, MAX_VERTS - TORN_VERTS);
    if (j == ROO_REST && n > 0 && n == SwayVerts) {
        for (k = 0; k < n; k++) {
            out[k].x = SwayMiddle[k].x + (out[k].x - SwayMiddle[k].x) * ROO_SWAY;
            out[k].y = SwayMiddle[k].y + (out[k].y - SwayMiddle[k].y) * ROO_SWAY;
            out[k].z = SwayMiddle[k].z + (out[k].z - SwayMiddle[k].z) * ROO_SWAY;
        }
    }
    return n > 0 && RigVerts > 0 ? add_tornado(out, n, 0, 0, 0) : n;
}

static void put_box(Bytes *o, const V3 *v, s32 n, float k, float ground) {
    float lo[3] = { 1e9f, 1e9f, 1e9f }, hi[3] = { -1e9f, -1e9f, -1e9f }, r = 0;
    u8 box[0x14];
    s32 i, j;

    for (i = 0; i < n; i++) {
        float p[3] = { v[i].x * k, v[i].y * k - ground, v[i].z * k };

        for (j = 0; j < 3; j++) {
            lo[j] = p[j] < lo[j] ? p[j] : lo[j];
            hi[j] = p[j] > hi[j] ? p[j] : hi[j];
        }
    }
    for (j = 0; j < 3; j++) {
        float h = (hi[j] - lo[j]) / 2;

        r += h * h;
    }
    /* as the game's own: across from least to most, but down from the bottom to the top */
    wr16(box + 0, (u32)(s16)lo[0]);
    wr16(box + 2, (u32)(s16)hi[1]);
    wr16(box + 4, (u32)(s16)lo[2]);
    wr16(box + 6, (u32)(s16)hi[0]);
    wr16(box + 8, (u32)(s16)lo[1]);
    wr16(box + 10, (u32)(s16)hi[2]);
    wr16(box + 12, (u32)(s16)((lo[0] + hi[0]) / 2));
    wr16(box + 14, (u32)(s16)((lo[1] + hi[1]) / 2));
    wr16(box + 16, (u32)(s16)((lo[2] + hi[2]) / 2));
    wr16(box + 18, (u32)(s16)sqrtf(r));
    put(o, box, sizeof(box));
}

static u8 *RooTex, *RooMdl;
static s32 RooTexSize, RooMdlSize, RooTried, RooMade;

/* His sounds, which his boss fight's object (RRooC) plays: sound entries in sound pages, PS1 ADPCM at 11025 Hz, as CrashEdit plays that
 * game's. His laugh (CrznA), and his landing (ld00A). His game has no hurt voice for him, so his yelp is his laugh's first "ha", its
 * first 86 blocks (0.22 s, where it falls quiet), a little higher. */
#define ROO_LAUGH_PITCH 0x400
#define ROO_YELP_BYTES (86 * 16)
#define ROO_YELP_PITCH 0x480
static u8 *RooLaugh, *RooThud;
static s32 RooLaughSize, RooThudSize;

static u8 *sound_of(const Level *l, const char *name, s32 *size) {
    Entry e;
    u8 *copy;

    if (!entry_find(l, name, &e) || e.type != 12 || e.count < 1 || e.size[0] < 16 || (copy = malloc((size_t)e.size[0])) == NULL) {
        return NULL;
    }
    memcpy(copy, e.item[0], (size_t)e.size[0]);
    *size = e.size[0];
    return copy;
}

static void make_sounds(const Level *l) {
    if (RooLaugh == NULL) {
        RooLaugh = sound_of(l, "CrznA", &RooLaughSize);
    }
    if (RooThud == NULL) {
        RooThud = sound_of(l, "ld00A", &RooThudSize);
    }
}

/* His hero card: the face his boss fight shows, whose tongue is his health (ShadC, the fight's panel: a 64 x 64 sprite), as red | green
 * << 8 | blue << 16 | alpha << 24, with the tongue short: his head and the tongue as it leaves his mouth (rows 0 to 41), then the
 * curl at its end (rows 50 to 62), so it hangs over the health bar. */
#define CARD_HEAD 42
#define CARD_TIP 50
#define CARD_ROWS (CARD_HEAD + 63 - CARD_TIP)
static u32 RooCard[64 * CARD_ROWS];
static s32 RooCardMade;

static void make_card(const Level *l) {
    Entry obj;
    const u8 *it;
    s32 at, f, x, y;

    if (!entry_find(l, "ShadC", &obj) || obj.type != 11 || obj.count < 6) {
        return;
    }
    it = obj.item[5];
    for (at = 4; at + 12 <= obj.size[5]; at += 4) {
        const u8 *page = texture_page(l, rd32(it + at));
        u32 head = rd32(it + at - 4);

        if (page == NULL || (head & 0xFFFF) != 2) {
            continue;
        }
        for (f = 0; f < (s32)(head >> 16) && at + 12 + f * 8 <= obj.size[5]; f++) {
            u32 col = rd32(it + at + 4 + f * 8), ti = rd32(it + at + 8 + f * 8);
            s32 uvi = (s32)(ti >> 22 & 0x3FF), cm = (s32)(ti >> 20 & 3), seg = (s32)(ti >> 18 & 3), xoffu = (s32)(ti >> 13 & 0x1F);
            s32 cy = (s32)(ti >> 6 & 0x7F), y0 = (s32)(ti & 0x1F) * 4, x0 = 256 * seg + 8 * xoffu, cx = (s32)(col >> 24 & 0xF) * 16;

            /* the face: the one 64 x 64 4-bit sprite */
            if (cm != 0 || 4 << (uvi % 5) != 64 || 4 << (uvi / 5 % 5) != 64 || y0 + 64 > 128) {
                continue;
            }
            for (y = 0; y < CARD_ROWS; y++) {
                s32 row = y < CARD_HEAD ? y : y - CARD_HEAD + CARD_TIP;

                for (x = 0; x < 64; x++) {
                    s32 u = x0 + x, i = page[(y0 + row) * 512 + u / 2] >> (u & 1) * 4 & 15;
                    u16 c = rd16(page + cy * 512 + (cx + i) * 2);

                    RooCard[y * 64 + x] = c == 0 ? 0 : 0xFF000000u | (u32)((c & 0x1F) * 255 / 31) | (u32)((c >> 5 & 0x1F) * 255 / 31) << 8
                                                   | (u32)((c >> 10 & 0x1F) * 255 / 31) << 16;
                }
            }
            RooCardMade = 1;
            return;
        }
    }
}

static void make_roo(void) {
    static V3 verts[MAX_VERTS], normals[MAX_VERTS];
    static Tri tris[MAX_TRIS];
    static Image images[MAX_IMAGES];
    DiscImage *disc;
    Level level;
    Entry model, anims[ROO_ANIMS];
    Bytes tex = { 0 }, mdl = { 0 };
    const u8 *info, *polys;
    s32 scale[3], ntri, nslot, nvert = 0, nimage = 0, i, j, k, n;
    float lo = 1e9f, hi = -1e9f, kscale, ground;
    size_t mesh, strips, vblock, uvidx, texs, colidx, end, colours, uvs, zeros, count0, entries, kept;

    const V3 light = { -0.35f, -0.75f, -0.55f };

    RooTried = 1;
    disc = disc_image_find("SCUS-94900");
    if (disc == NULL) {
        return;
    }
    if (!level_read(disc, "S1/S0000017.NSF", &level)) {
        printf("bonus: Crash Bandicoot's Ripper Roo level can't be read\n");
        disc_image_close(disc);
        return;
    }
    disc_image_close(disc);
    if (!entry_find(&level, "RS1nG", &model) || model.type != 2 || model.count < 2) {
        printf("bonus: Crash Bandicoot's disc has no Ripper Roo\n");
        goto done;
    }
    for (i = 0; i < ROO_ANIMS; i++) {
        if (!entry_find(&level, RooAnims[i].entry, &anims[i]) || anims[i].type != 1 || anims[i].count <= 0) {
            printf("bonus: Ripper Roo's %s is missing\n", RooAnims[i].entry);
            goto done;
        }
    }
    info = model.item[0];
    polys = model.item[1];
    ntri = (s32)rd32(info);
    scale[0] = (s32)rd32(info + 4);
    scale[1] = (s32)rd32(info + 8);
    scale[2] = (s32)rd32(info + 12);
    nslot = (s32)rd32(info + 16);
    if (ntri <= 0 || ntri > MAX_TRIS || model.size[1] < ntri * 8 || model.size[0] < 20 + nslot * 4) {
        goto done;
    }
    /* how big he is, standing */
    sway_middle(&anims[ROO_REST], scale);
    rig_setup(&anims[ROO_SPIN], scale);
    nvert = anim_verts(&anims[ROO_REST], ROO_REST, 0, scale, verts, normals);
    if (nvert == 0) {
        goto done;
    }
    for (k = 0; k < nvert; k++) {
        lo = verts[k].y < lo ? verts[k].y : lo;
        hi = verts[k].y > hi ? verts[k].y : hi;
    }
    kscale = ROO_HEIGHT / (hi - lo);
    ground = hi * kscale;

    /* his triangles, their colours lit as he stands, and their textures */
    for (i = 0; i < ntri; i++) {
        const u8 *p = polys + i * 8, *s;
        Tri *t = &tris[i];
        u16 w = rd16(p + 6);

        t->a = rd16(p) / 6;
        t->b = rd16(p + 2) / 6;
        t->c = rd16(p + 4) / 6;
        t->slot = w & 0x7FFF;
        t->image = -1;
        if (t->a >= nvert || t->b >= nvert || t->c >= nvert || t->slot >= nslot) {
            t->a = t->b = t->c = 0;
            t->slot = 0;
        }
        s = info + 20 + t->slot * 4;
        t->rgb[0] = s[0];
        t->rgb[1] = s[1];
        t->rgb[2] = s[2];
        if ((s[3] & 0x80) && 20 + (t->slot + 3) * 4 <= model.size[0]) {
            u32 page = rd32(s + 4), tinfo = rd32(s + 8);
            s32 uvi = (s32)(tinfo >> 22 & 0x3FF), cm = (s32)(tinfo >> 20 & 3), seg = (s32)(tinfo >> 18 & 3);
            s32 xoffu = (s32)(tinfo >> 13 & 0x1F), yoffu = (s32)(tinfo & 0x1F), wind = uvi / 25 & 31;
            s32 tw = 4 << (uvi % 5), th = 4 << (uvi / 5 % 5), xoff, yoff;

            if (cm <= 1 && texture_page(&level, page) != NULL) {
                xoff = (64 << (2 - cm)) * seg + (2 << (2 - cm)) * xoffu;
                yoff = yoffu * 4;
                for (j = 0; j < nimage; j++) {
                    if (images[j].page_eid == page && images[j].x == xoff && images[j].y == yoff && images[j].w == tw && images[j].h == th
                        && images[j].bpp == (cm == 0 ? 4 : 8) && images[j].clutx == (s[3] & 0xF) && (images[j].info >> 6 & 0x7F) == (tinfo >> 6 & 0x7F)) {
                        break;
                    }
                }
                if (j == nimage && nimage < MAX_IMAGES) {
                    images[nimage].page_eid = page;
                    images[nimage].info = tinfo;
                    images[nimage].clutx = s[3] & 0xF;
                    images[nimage].x = xoff;
                    images[nimage].y = yoff;
                    images[nimage].w = tw;
                    images[nimage].h = th;
                    images[nimage].bpp = cm == 0 ? 4 : 8;
                    nimage++;
                }
                if (j < nimage) {
                    t->image = j;
                    /* corner a has U1 V1, b U2 V2, c U3 V3 */
                    t->uv[0][0] = tw * (0x30FF0C >> wind & 1);
                    t->uv[1][0] = tw * (0x8799E1 >> wind & 1);
                    t->uv[2][0] = tw * (0x4B66D2 >> wind & 1);
                    t->uv[0][1] = th * (0xF3CC30 >> wind & 1);
                    t->uv[1][1] = th * (0x9E7186 >> wind & 1);
                    t->uv[2][1] = th * (0x6DB249 >> wind & 1);

                }
            }
        }
    }

    /* the tornado's faces, after his own, on its vertices after his (add_tornado): each quad two faces a side, so it shows from in
     * and out, each corner bright as far as it is to its streak's front */
    if (RigVerts > 0 && nvert == RigVerts + TORN_VERTS && ntri + TORN_TRIS <= MAX_TRIS) {
        s32 streak, seg, q, c;

        for (streak = 0; streak < TORN_STREAKS; streak++) {
            for (seg = 0; seg < TORN_SEGS; seg++) {
                /* this pair's low and high, then the next pair's */
                s32 v0 = RigVerts + (streak * (TORN_SEGS + 1) + seg) * 2;
                const s32 faces[4][3] = {
                    { v0, v0 + 2, v0 + 3 }, { v0, v0 + 3, v0 + 1 }, { v0, v0 + 3, v0 + 2 }, { v0, v0 + 1, v0 + 3 },
                };

                for (q = 0; q < 4; q++) {
                    Tri *t = &tris[ntri++];

                    memset(t, 0, sizeof(*t));
                    t->a = faces[q][0];
                    t->b = faces[q][1];
                    t->c = faces[q][2];
                    t->image = -1;
                    t->glow = 1;
                    for (j = 0; j < 3; j++) {
                        float b = powf((float)((faces[q][j] - RigVerts) / 2 % (TORN_SEGS + 1)) / TORN_SEGS, TORN_RAMP);

                        for (c = 0; c < 3; c++) {
                            t->corner_rgb[j][c] = (u8)(TornRgb[c] * b);
                        }
                    }
                }
            }
        }
    }

    /* the texture set: a palette and an image a texture */
    {
        size_t first_image;

        put(&tex, NULL, 0x20);
        wr32(tex.b, 8);
        for (j = 0; j < nimage; j++) {
            const u8 *page = texture_page(&level, images[j].page_eid);
            s32 colours_n = images[j].bpp == 4 ? 16 : 256, cy = (s32)(images[j].info >> 6 & 0x7F), cx = images[j].clutx * 16;
            u8 head[4];

            wr32(head, (u32)colours_n);
            put(&tex, head, 4);
            for (k = 0; k < colours_n; k++) {
                s32 x = cx + k;

                put(&tex, x < 256 ? page + cy * 512 + x * 2 : (const u8 *)"\0\0", 2);
            }
        }
        first_image = tex.n;
        for (j = 0; j < nimage; j++) {
            const u8 *page = texture_page(&level, images[j].page_eid);
            s32 units = images[j].bpp == 4 ? images[j].w / 4 : images[j].w / 2, row = units * 2, y;
            u8 head[20];

            memset(head, 0, sizeof(head));
            wr16(head, (u32)units);
            wr16(head + 2, (u32)images[j].h);
            wr16(head + 6, (u32)((images[j].h & 0xFF) << 8 | (row & 0xFF)));
            wr16(head + 12, (u32)(j << 1 | (images[j].bpp == 8)));
            wr32(head + 16, 1);
            put(&tex, head, sizeof(head));
            for (y = 0; y < images[j].h; y++) {
                s32 at = (images[j].y + y) * 512 + (images[j].bpp == 4 ? images[j].x / 2 : images[j].x);

                put(&tex, at + row <= PAGE_BYTES ? page + at : NULL, (size_t)row);
            }
        }
        if (nimage == 0) {
            /* none: one clear image, as a set has some */
            u8 head[20], clut[36];

            memset(clut, 0, sizeof(clut));
            wr32(clut, 16);
            put(&tex, clut, sizeof(clut));
            first_image = tex.n;
            memset(head, 0, sizeof(head));
            wr16(head, 1);
            wr16(head + 2, 4);
            wr16(head + 6, 4 << 8 | 2);
            wr32(head + 16, 1);
            put(&tex, head, sizeof(head));
            put(&tex, NULL, 8);
            nimage = 1;
        }
        wr16(tex.b + 8, (u32)nimage);
        wr16(tex.b + 10, (u32)nimage);
        wr32(tex.b + 0x0C, (u32)(first_image - 0x0C));
        wr32(tex.b + 0x10, 0x10);
        align(&tex, 4);
        wr32(tex.b + 4, (u32)tex.n);
    }

    /* the model: a mesh of a strip a triangle, and his animations */
    put(&mdl, NULL, 0x58 + 0x34);
    mesh = 0x58;
    strips = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(1 << 8 | (tris[i].image < 0)));
        put(&mdl, s, 2);
    }
    put(&mdl, "\xFF\xFF", 2);
    align(&mdl, 4);
    vblock = mdl.n;
    {
        static V3 corners[MAX_TRIS * 3];

        for (i = 0; i < ntri; i++) {
            corners[i * 3] = verts[tris[i].a];
            corners[i * 3 + 1] = verts[tris[i].b];
            corners[i * 3 + 2] = verts[tris[i].c];
        }
        put_box(&mdl, corners, ntri * 3, kscale, ground);
        for (i = 0; i < ntri * 3; i++) {
            u8 v[8];

            memset(v, 0, sizeof(v));
            wr16(v, (u32)(s16)(corners[i].x * kscale));
            wr16(v + 2, (u32)(s16)(corners[i].y * kscale - ground));
            wr16(v + 4, (u32)(s16)(corners[i].z * kscale));
            put(&mdl, v, sizeof(v));
        }
    }
    uvidx = mdl.n;
    for (i = 0, n = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(tris[i].image >= 0 ? n : 0));
        n += tris[i].image >= 0 ? 3 : 0;
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    texs = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        wr16(s, (u32)(tris[i].image >= 0 ? tris[i].image : 0));
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    colidx = mdl.n;
    for (i = 0; i < ntri; i++) {
        u8 s[2];

        /* the tornado's added, semi-transparent (bit 15) in blend mode 1 */
        wr16(s, (u32)(i * 3 | (tris[i].glow ? 0x8000 | 1 << 13 : 0)));
        put(&mdl, s, 2);
    }
    align(&mdl, 4);
    end = mdl.n;
    /* each corner's colour, lit by the light as he stands: the game's own models have their light in their colours */
    colours = mdl.n;
    {
        float ll = sqrtf(light.x * light.x + light.y * light.y + light.z * light.z);

        for (i = 0; i < ntri; i++) {
            const s32 corner[3] = { tris[i].a, tris[i].b, tris[i].c };

            for (j = 0; j < 3; j++) {
                const V3 *nn = &normals[corner[j]];
                float len, lit = 0;
                u8 c[4];

                if (tris[i].glow) {
                    c[0] = tris[i].corner_rgb[j][0];
                    c[1] = tris[i].corner_rgb[j][1];
                    c[2] = tris[i].corner_rgb[j][2];
                    c[3] = 0;
                    put(&mdl, c, 4);
                    continue;
                }
                len = sqrtf(nn->x * nn->x + nn->y * nn->y + nn->z * nn->z);
                if (len > 0) {
                    lit = (nn->x * light.x + nn->y * light.y + nn->z * light.z) / (len * ll);
                }
                lit = 0.62f + 0.55f * (lit > 0 ? lit : 0);
                for (k = 0; k < 3; k++) {
                    float v = tris[i].rgb[k] * lit;

                    c[k] = (u8)(v > 255 ? 255 : v);
                }
                c[3] = 0;
                put(&mdl, c, 4);
            }
        }
    }
    uvs = mdl.n;
    for (i = 0; i < ntri; i++) {
        for (j = 0; tris[i].image >= 0 && j < 3; j++) {
            u8 s[2];
            s32 u = tris[i].uv[j][0], v = tris[i].uv[j][1];

            wr16(s, (u32)((u > 255 ? 255 : u) | (v > 255 ? 255 : v) << 8));
            put(&mdl, s, 2);
        }
    }
    put(&mdl, NULL, 4);
    align(&mdl, 4);
    zeros = put(&mdl, NULL, 0x18);
    count0 = put(&mdl, NULL, 4);
    entries = put(&mdl, NULL, (size_t)ROO_ANIMS * 0x18);
    kept = mdl.n;

    /* the header */
    wr32(mdl.b, 0x0C160029);
    self(&mdl, 0x08, zeros);
    self(&mdl, 0x10, zeros);
    self(&mdl, 0x18, count0);
    self(&mdl, 0x1C, count0);
    self(&mdl, 0x20, colours);
    self(&mdl, 0x24, uvs);
    self(&mdl, 0x28, zeros);
    self(&mdl, 0x2C, zeros);
    self(&mdl, 0x3C, zeros);
    wr32(mdl.b + 0x40, ROO_ANIMS);
    self(&mdl, 0x44, entries);
    self(&mdl, 0x4C, count0);
    wr32(mdl.b + 0x50, (u32)kept);
    wr32(mdl.b + 0x54, 1);
    wr16(mdl.b + mesh + 0x08, (u32)ntri);
    wr16(mdl.b + mesh + 0x0A, 4);
    self(&mdl, mesh + 0x10, vblock);
    self(&mdl, mesh + 0x14, strips);
    self(&mdl, mesh + 0x18, uvidx);
    self(&mdl, mesh + 0x1C, texs);
    self(&mdl, mesh + 0x20, colidx);
    self(&mdl, mesh + 0x24, end);
    wr32(tex.b + 0x14, (u32)kept);

    /* the animations, each in a block of its own from a 2 KB sector on, with room before it for the heap's header as it's split off */
    for (j = 0; j < ROO_ANIMS; j++) {
        static V3 fv[MAX_VERTS];
        s32 frames = anim_frames(&anims[j], j), f, nv = 0;
        size_t start, list0, pool, e = entries + (size_t)j * 0x18;

        put(&mdl, NULL, 0x20);
        align(&mdl, 2048);
        start = put(&mdl, NULL, 4 + (size_t)frames * 16);
        list0 = mdl.n;
        for (f = 0; f < frames; f++) {
            static V3 corners[MAX_TRIS * 3];
            size_t list;

            nv = anim_verts(&anims[j], j, f, scale, fv, NULL);
            for (i = 0; i < ntri; i++) {
                corners[i * 3] = fv[nv > tris[i].a ? tris[i].a : 0];
                corners[i * 3 + 1] = fv[nv > tris[i].b ? tris[i].b : 0];
                corners[i * 3 + 2] = fv[nv > tris[i].c ? tris[i].c : 0];
            }
            list = mdl.n;
            self(&mdl, start + 4 + (size_t)f * 16, list);
            put_box(&mdl, corners, ntri * 3, kscale, ground);
            for (i = 0; i < ntri; i++) {
                const s32 corner[3] = { tris[i].a, tris[i].b, tris[i].c };

                for (k = 0; k < 3; k++) {
                    u8 s[2];
                    s32 index = f * nvert + (corner[k] < nv ? corner[k] : 0);

                    /* the third corner says which way round the face is */
                    wr16(s, (u32)(index << 2 | (k == 2 && FLIP_FACES)));
                    put(&mdl, s, 2);
                }
            }
            align(&mdl, 4);
        }
        (void)list0;
        pool = mdl.n;
        self(&mdl, start, pool);
        for (f = 0; f < frames; f++) {
            nv = anim_verts(&anims[j], j, f, scale, fv, NULL);
            for (k = 0; k < nvert; k++) {
                u8 v[6];
                const V3 *p = &fv[k < nv ? k : 0];

                wr16(v, (u32)(s16)(p->x * kscale));
                wr16(v + 2, (u32)(s16)(p->y * kscale - ground));
                wr16(v + 4, (u32)(s16)(p->z * kscale));
                put(&mdl, v, sizeof(v));
            }
        }
        align(&mdl, 4);
        wr32(mdl.b + e, (u32)start);
        wr32(mdl.b + e + 4, (u32)mdl.n);
        wr32(mdl.b + e + 8, (u32)frames);
        self(&mdl, e + 0x0C, mesh);
        wr32(mdl.b + e + 0x10, (u32)string_hash((u8 *)RooAnims[j].name));
    }
    if (tex.b == NULL || mdl.b == NULL || (size_t)ntri * 3 > 4096) {
        goto done;
    }
    RooTex = tex.b;
    RooTexSize = (s32)tex.n;
    RooMdl = mdl.b;
    RooMdlSize = (s32)mdl.n;
    tex.b = mdl.b = NULL;
    RooMade = 1;
    make_card(&level);
    make_sounds(&level);
    printf("bonus: Ripper Roo from Crash Bandicoot, %d triangles, %d textures, %d bytes%s%s%s\n", ntri, nimage, RooMdlSize,
           RooCardMade ? ", his hero card" : "", RooLaugh != NULL ? ", his laugh" : "", RooThud != NULL ? ", his landing" : "");
done:
    free(tex.b);
    free(mdl.b);
    free(level.pages);
}

void bonus_heroes_files(void) {
    u8 *tex, *mdl;

    if (!extra_heroes_on()) {
        return;
    }
    if (!RooTried) {
        make_roo();
    }
    if (!RooMade || (tex = malloc((size_t)RooTexSize)) == NULL || (mdl = malloc((size_t)RooMdlSize)) == NULL) {
        return;
    }
    memcpy(tex, RooTex, (size_t)RooTexSize);
    memcpy(mdl, RooMdl, (size_t)RooMdlSize);
    archive_replace(BONUS_TEX, tex, RooTexSize);
    archive_replace(BONUS_MDL, mdl, RooMdlSize);
}

s32 bonus_heroes_have(s32 extra) {
    return extra == EXTRA_ROO && RooMade && extra_heroes_on();
}

u8 *bonus_heroes_model(s32 extra) {
    u8 *copy = extra == EXTRA_ROO && RooMade ? malloc((size_t)RooMdlSize) : NULL;

    if (copy != NULL) {
        memcpy(copy, RooMdl, (size_t)RooMdlSize);
    }
    return copy;
}

const u32 *bonus_heroes_card(s32 extra, s32 *w, s32 *h, float *stretch) {
    if (!bonus_heroes_have(extra) || !RooCardMade) {
        return NULL;
    }
    *w = 64;
    *h = CARD_ROWS;
    *stretch = 1.0f; /* as its pixels are: at the 1.6 its game showed it at, he looked squeezed in the panel */
    return RooCard;
}

/* One of his sounds, bytes of it, as loud as the game's own effects at the sound option it's at (sound.c, sfx_key_on); with a voice,
 * the one that had it last stops first. */
static void play_own(const u8 *adpcm, s32 bytes, u32 pitch, short *voice) {
    s32 vol = 0x1E00 * GameOptions.sfx * 0x7F / 409600;
    short v;

    if (adpcm == NULL || bytes < 16) {
        return;
    }
    if (voice != NULL && audio_sample_playing(*voice, adpcm)) {
        audio_voice_off(*voice);
    }
    vol = vol < 0 ? 0 : vol > 0x7F ? 0x7F : vol;
    v = audio_play_sample(adpcm, (uint32_t)bytes, pitch, 0x3FFF * vol / 0x7F, 0x3FFF * vol / 0x7F);
    if (voice != NULL) {
        *voice = v;
    }
}

void bonus_heroes_cheer(s32 extra) {
    static short voice = -1;

    /* one laugh at a time: a new one starts it again, as a hero's taunt sound does (its move's 0x800A, on the hero's own sound) */
    if (bonus_heroes_have(extra)) {
        play_own(RooLaugh, RooLaughSize, ROO_LAUGH_PITCH, &voice);
    }
}

void bonus_heroes_hurt(s32 extra, s32 fall) {
    if (!bonus_heroes_have(extra)) {
        return;
    }
    if (fall) {
        play_own(RooThud, RooThudSize, ROO_LAUGH_PITCH, NULL);
    } else {
        play_own(RooLaugh, RooLaughSize < ROO_YELP_BYTES ? RooLaughSize : ROO_YELP_BYTES, ROO_YELP_PITCH, NULL);
    }
}

s32 bonus_heroes_bank(s32 extra) {
    return extra == EXTRA_ROO ? DB_BonusRoo : DB_Empty;
}
