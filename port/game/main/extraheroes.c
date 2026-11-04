/* Extra heroes (bash/extraheroes.h, PC only): characters from the player's own copies of other Crash games, as heroes of their own:
 * Ripper Roo, from Crash Bandicoot, when that disc is beside the program or in a discs folder. Each is a file of its own in heroes/
 * (bash/herodef.h: how it plays, and how bonusheroes.c makes its models), and without its disc there's none.
 *
 * The game keeps a great deal by hero number, 1 to 8: speeds, the computer's play, sounds, shadows, the moves, which model each game
 * loads. So an extra hero borrows, inside, the number of an original hero nobody's playing that match (at most four of the eight are),
 * its base, and plays as that hero does: the same moves, timing and reach. What makes it itself is its models, one of each kind the
 * games have (on foot, riding a polar bear), each put in the base's place in the game's tables of models by hero (HeroHooks.HeroModels
 * and the second some games load, HeroHooks.flag) and drawn wherever the game shows the base's of that kind; its face on the panel; its
 * sounds in place of its base's; and how its frames are drawn. The moves name frames of the base's own model, 0x4000 | n << 7 | f,
 * frame f of its animation n, so as an extra hero is drawn each one is its stand-in for that animation, by name (Roo's walk where a hero
 * would run, his spin where one attacks), the same share of the way through or at its own speed, with less of its lift off the ground
 * if its kind says so. The points the game finds on a frame (the boxes it collides with, the hands that hold a crate) are the base's,
 * from its own model, and so is its body's box (body_box); what it holds sits on its head, which is where the head point is too. The
 * moves themselves never change, so the game's checks of how far one has got still hold. In a game where its kind says so it's as
 * heavy and as quick as another hero (Roo on his bear as Crash). */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/dbase.h"
#include "bash/modelbank.h"
#include "bash/panel.h"
#include "bash/nascalls.h"
#include "bash/pause.h"
#include "bash/heroes.h"
#include "bash/herodef.h"
#include "bash/extraheroes.h"
#include "bash/bonusheroes.h"
#include "../../platform/settings.h"
#include "../../platform/gfx.h"

unsigned char *archive_own_file(int entry, int *size);

#define MAX_ANIMS 32   /* a model's animations, as n in a frame's model id can count */
#define MODELS 10      /* a game's table of models by hero number: nobody, the eight, and Off */
#define MAX_POINTS 512 /* bytes of a frame's points and boxes */
#define HELD_FLAGS 0x31 /* the points a held crate goes between (0x10, 0x20) or at (1): xtras.c, CrateFux */
#define HEAD_FLAG 0x80  /* the top of a hero's head */
#define MORPH_VERTS 4096 /* prims.c's MorphVerts, a frame's vertices as they're drawn: all of a mesh's (Roo's are 3435, 3 a triangle) */
#define HIT_FIRST 2     /* a hero's hit voices, in its block of sounds (HeroHitSfx: the first of them) */

/* A player's extra hero's model of one kind, while a level's on */
typedef struct {
    s32 kind;
    const obj_dbase *bank;  /* the extra hero's model */
    s32 base_bank;          /* and its base's, as the game's table has it */
    u8 *base;               /* the base's own model, from the archive */
    u8 *own;                /* and the extra hero's */
    s32 count;              /* the base's animations */
    s32 to_anim[MAX_ANIMS]; /* the extra hero's stand-in for each, and the frames of it that stand in */
    s32 to_first[MAX_ANIMS], to_count[MAX_ANIMS];
    s32 from_frames[MAX_ANIMS];
    s32 pace[MAX_ANIMS];    /* STRETCH, LOOP or ONCE */
    s32 clock_anim;         /* the base's animation it was last drawn in, its frame, and the tick it started (GameFrameCount) */
    s32 clock_frame, clock_start;
    s32 scale;              /* the extra hero's height over the base's, 0x1000 = the same */
} Variant;

/* A player's extra hero while a level's on: its models of the kinds this game loads (its table's, and its second's) */
typedef struct {
    s32 on;
    const subengine *sub; /* the hero's own */
    s32 nvar;
    Variant var[2];
    union {
        PointList list;
        u8 bytes[MAX_POINTS];
    } points;
} Playing;

static s32 PlayerExtra[4];
static s32 PlayerBase[4]; /* 0 until extra_heroes_assign has given it one */
static u16 Models[MODELS], FlagModels[MODELS];
static u16 *ModelsFrom, *FlagFrom;
static Playing Play[4];
static s32 AnyPlaying;

s32 extra_heroes_on(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (settings()->test_extra[i] != 0) {
            return 1;
        }
    }
    return settings()->extra_heroes;
}

s32 extra_hero_of(s32 player) {
    return player >= 0 && player < 4 ? PlayerExtra[player] : EXTRA_NONE;
}

void extra_hero_pick(s32 player, s32 extra) {
    if (player >= 0 && player < 4) {
        PlayerExtra[player] = extra > EXTRA_NONE && extra < NUM_EXTRA ? extra : EXTRA_NONE;
        PlayerBase[player] = 0;
    }
}

const char *extra_hero_name(s32 extra) {
    return extra > EXTRA_NONE && extra < NUM_EXTRA ? HeroDefs[extra]->name : "";
}

s32 extra_hero_named(const char *id) {
    s32 x;

    for (x = EXTRA_NONE + 1; x < NUM_EXTRA; x++) {
        if (strcmp(HeroDefs[x]->id, id) == 0) {
            return x;
        }
    }
    return EXTRA_NONE;
}

/* Whether it's there to play: its models came from the player's disc of its game (bonusheroes.c). */
s32 extra_hero_available(s32 extra) {
    return extra > EXTRA_NONE && extra < NUM_EXTRA && bonus_heroes_have(extra);
}

/* Its model on foot's bank, and its file (to free): what its face and portrait are drawn from */
static s32 foot_bank(s32 extra) {
    return bonus_heroes_bank(extra, KIND_FOOT);
}

static u8 *foot_file(s32 extra) {
    return bonus_heroes_have(extra) ? bonus_heroes_model(extra, KIND_FOOT) : NULL;
}

s32 extra_hero_model(s32 extra) {
    return extra > EXTRA_NONE && extra < NUM_EXTRA ? foot_bank(extra) : DB_Empty;
}

/* The kind of model a hero's bank in one of the game's tables is */
static s32 kind_of(s32 bank) {
    if ((bank >= DB_CharsPolar2Crash && bank <= DB_CharsPolar2Brio) || bank == DB_CharsPolar2Kong || bank == DB_CharsPolar2Rilla
        || (bank >= DB_CharsPolarCrash && bank <= DB_CharsPolarKong)) {
        return KIND_POLAR;
    }
    return KIND_FOOT;
}

static s32 taken(s32 name, s32 except) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (i != except && GAME->heroes[i] == name) {
            return 1;
        }
    }
    return 0;
}

void extra_heroes_assign(void) {
    s32 i, name;

    for (i = 0; i < 4; i++) {
        if (settings()->test_extra[i] != 0 && PlayerExtra[i] != settings()->test_extra[i]) {
            extra_hero_pick(i, settings()->test_extra[i]);
        }
        if (!extra_heroes_on() || (PlayerExtra[i] != EXTRA_NONE && !extra_hero_available(PlayerExtra[i]))) {
            if (PlayerExtra[i] != EXTRA_NONE && settings()->test_extra[i] != 0) {
                printf("extra heroes: %s needs your copy of his game beside the program or in a discs folder\n", HeroDefs[PlayerExtra[i]]->name);
            }
            PlayerExtra[i] = EXTRA_NONE;
        }
    }
    for (i = 0; i < 4; i++) {
        if (PlayerExtra[i] == EXTRA_NONE) {
            continue;
        }
        /* its own base, if nobody else has it; then the first nobody has */
        name = HeroDefs[PlayerExtra[i]]->prefer;
        if (taken(name, i)) {
            for (name = Crash; name <= Kanga && taken(name, i); name++) {
            }
        }
        GAME->heroes[i] = (u8)name;
        GAME->heroChoice[i] = (u8)name; /* the panel's tallies show the face of who was chosen */
        PlayerBase[i] = name;
    }
}

/* A table of models by hero with each extra hero's of the kind there in its base's place */
static void swap_models(u16 *to, const u16 *from) {
    s32 i;

    memcpy(to, from, sizeof(u16) * MODELS);
    for (i = 0; i < 4; i++) {
        if (PlayerExtra[i] != EXTRA_NONE && (u32)(PlayerBase[i] - 1) < 8) {
            to[PlayerBase[i]] = (u16)bonus_heroes_bank(PlayerExtra[i], kind_of(from[PlayerBase[i]]));
        }
    }
}

void extra_heroes_models(void) {
    s32 i;

    for (i = 0; i < 4 && PlayerExtra[i] == EXTRA_NONE; i++) {
    }
    if (i == 4) {
        return;
    }
    if (HeroHooks.HeroModels != NULL && HeroHooks.HeroModels != Models) {
        ModelsFrom = HeroHooks.HeroModels;
        swap_models(Models, ModelsFrom);
        HeroHooks.HeroModels = Models;
    }
    if (HeroHooks.flag == NULL) {
        FlagFrom = NULL;
    } else if (HeroHooks.flag != (void *)FlagModels) {
        FlagFrom = HeroHooks.flag;
        swap_models(FlagModels, FlagFrom);
        HeroHooks.flag = FlagModels;
    }
}

s32 extra_heroes_model_is(const obj_dbase *db) {
    s32 x, k;

    for (x = EXTRA_NONE + 1; x < NUM_EXTRA; x++) {
        for (k = 0; k < NUM_KINDS; k++) {
            if (db == &HeroModelBank[bonus_heroes_bank(x, k)]) {
                return 1;
            }
        }
    }
    return 0;
}

/* --- models --- */

static const FileEntry *anims_of(const ModelFile *m) {
    return (const FileEntry *)((const u8 *)&m->meshAnims + m->meshAnims);
}

static s32 height_of(const ModelFile *m) {
    const MeshHeader *h = &m->meshes[0];
    const BoundBox *box;

    if (m->nMeshes < 1 || h->verts == 0) {
        return 0;
    }
    box = (const BoundBox *)((const u8 *)&h->verts + h->verts);
    return abs(box->max[1] - box->min[1]);
}

static s32 hash_of(const char *s) {
    return string_hash((u8 *)s);
}

static s32 has(const ModelFile *m, s32 key) {
    const FileEntry *e = anims_of(m);
    s32 i;

    for (i = 0; i < m->nMeshAnims; i++) {
        if (e[i].hash == key) {
            return 1;
        }
    }
    return 0;
}

/* The extra hero's stand-in for one of its base's animations, by name, in its model of a kind m: the first of its stand-in's names that
 * m has; or the base's own name, if m has it; or else its kind's `otherwise`. If first isn't NULL, the range of its frames that name
 * gives (or -1, all of them) and its pace. */
static s32 stand_in(const HeroDef *x, s32 kind, s32 key, const ModelFile *m, s32 *first, s32 *last, s32 *pace) {
    const HeroKind *k = &x->kinds[kind];
    const Stand *s;
    const char *names = NULL;
    char one[24];

    if (first != NULL) {
        *first = *last = -1;
        *pace = STRETCH;
    }
    for (s = k->stands; s != NULL && s->from != NULL; s++) {
        if (hash_of(s->from) == key) {
            names = s->to;
            if (pace != NULL) {
                *pace = s->pace;
            }
            break;
        }
    }
    while (names != NULL && *names != 0) {
        size_t n = strcspn(names, " ");

        if (n > 0 && n < sizeof(one)) {
            char *range;

            memcpy(one, names, n);
            one[n] = 0;
            if ((range = strchr(one, ':')) != NULL) {
                *range++ = 0;
            }
            if (has(m, hash_of(one))) {
                if (range != NULL && first != NULL) {
                    s32 sub = k->subframes > 1 ? k->subframes : 1;

                    *first = atoi(range) * sub;
                    *last = (strchr(range, '-') != NULL ? atoi(strchr(range, '-') + 1) : atoi(range)) * sub;
                }
                return hash_of(one);
            }
        }
        names += n;
        names += strspn(names, " ");
    }
    if (has(m, key)) {
        return key;
    }
    return hash_of(k->otherwise);
}

static s32 find(const ModelFile *m, s32 key) {
    const FileEntry *e = anims_of(m);
    s32 i;

    for (i = 0; i < m->nMeshAnims; i++) {
        if (e[i].hash == key) {
            return i;
        }
    }
    return 0;
}

/* A mesh's vertices, as its strips use them (MeshHeader.runs: a u16 faces << 8 | flags each, n faces on n + 2 vertices, to 0xFFFF). */
static s32 vertex_count(const MeshHeader *h) {
    const u16 *s = (const u16 *)((const u8 *)&h->runs + h->runs);
    s32 n = 0;

    for (; *s != 0xFFFF; s++) {
        n += (*s >> 8) + 2;
    }
    return n;
}

/* A frame of a mesh animation (prims.c, draw_object_model): each of the mesh's vertices is one in the animation's pool, s16 x, y, z, by
 * an index list after a bounding box; one between two keyframes is blend of the way from its list to the next one. */
typedef struct {
    const MeshHeader *mesh;
    const s16 *pool;
    const u16 *a, *b;
    s32 blend;
    s32 count;
} Frame;

/* Frame f of animation n, in a whole model file. */
static s32 frame_at(const u8 *file, s32 n, s32 f, Frame *out) {
    const ModelFile *m = (const ModelFile *)file;
    const FileEntry *e;
    const s32 *anim, *fr;

    if (file == NULL || n < 0 || n >= m->nMeshAnims) {
        return 0;
    }
    e = &anims_of(m)[n];
    if (f < 0 || f >= e->frames) {
        return 0;
    }
    anim = (const s32 *)(file + e->start);
    fr = (const s32 *)((const u8 *)anim + 4 + f * 16);
    out->mesh = (const MeshHeader *)((const u8 *)&e->mesh + e->mesh);
    out->pool = (const s16 *)(anim[0] != 0 ? (const u8 *)anim + anim[0] : (const u8 *)&m->verts + m->verts);
    out->a = (const u16 *)((const u8 *)fr + fr[0] + 0x14);
    out->blend = fr[2];
    out->b = fr[2] != 0 ? (const u16 *)((const u8 *)&fr[1] + fr[1] + 0x14) : out->a;
    out->count = vertex_count(out->mesh);
    return 1;
}

static void vertex_of(const Frame *fr, s32 k, s32 v[3]) {
    const s16 *a = fr->pool + (fr->a[k] >> 2) * 3, *b = fr->pool + (fr->b[k] >> 2) * 3;
    s32 j;

    for (j = 0; j < 3; j++) {
        v[j] = a[j] + ((b[j] - a[j]) * fr->blend >> 12);
    }
}

/* A frame's shape: how far it's off the ground, and its top (up is less y) and the middle of that. */
typedef struct {
    s32 lift;
    s32 top;
    s32 head_x, head_z;
} Shape;

static s32 shape_of(const u8 *file, s32 n, s32 f, Shape *s) {
    Frame fr;
    s32 k, v[3], bottom = -0x8000, top = 0x7FFF, line, sx = 0, sz = 0, count = 0;

    if (!frame_at(file, n, f, &fr) || fr.count <= 0) {
        return 0;
    }
    for (k = 0; k < fr.count; k++) {
        vertex_of(&fr, k, v);
        bottom = v[1] > bottom ? v[1] : bottom;
        top = v[1] < top ? v[1] : top;
    }
    /* its top sixth: the head */
    line = top + (bottom - top) / 6;
    for (k = 0; k < fr.count; k++) {
        vertex_of(&fr, k, v);
        if (v[1] <= line) {
            sx += v[0];
            sz += v[2];
            count++;
        }
    }
    s->lift = bottom < 0 ? -bottom : 0;
    s->top = top;
    s->head_x = count > 0 ? sx / count : 0;
    s->head_z = count > 0 ? sz / count : 0;
    return 1;
}

/* How far down a frame's drawn, for less of its lift: keep of it is kept. */
static s32 lowered(const Shape *s, s32 keep) {
    return s->lift * (0x1000 - keep) >> 12;
}

/* --- the frames --- */

void extra_heroes_stop(void) {
    s32 i, k;

    for (i = 0; i < 4; i++) {
        for (k = 0; k < Play[i].nvar; k++) {
            free(Play[i].var[k].base);
            free(Play[i].var[k].own);
        }
        memset(&Play[i], 0, sizeof(Play[i]));
    }
    AnyPlaying = 0;
}

static u8 *own_model(s32 bank) {
    s32 size = 0;

    return archive_own_file((s32)(HeroModelBank[bank].file - DatIndex), &size);
}

static void body_box(s32 player);

/* Its model of the kind a table's base bank is, as it's loaded now: 0 if it isn't */
static s32 variant_start(s32 player, Variant *v, s32 base_bank) {
    const HeroDef *x = HeroDefs[PlayerExtra[player]];
    const ModelFile *from, *to;
    s32 k, from_height;

    memset(v, 0, sizeof(*v));
    v->kind = kind_of(base_bank);
    v->bank = &HeroModelBank[bonus_heroes_bank(PlayerExtra[player], v->kind)];
    v->base_bank = base_bank;
    if ((to = v->bank->data) == NULL) {
        return 0;
    }
    /* the base's own model for this game, as it isn't loaded, and the extra hero's whole, with the frames a load leaves out */
    v->base = own_model(base_bank);
    v->own = bonus_heroes_model(PlayerExtra[player], v->kind);
    if (v->base == NULL || v->own == NULL) {
        free(v->base);
        free(v->own);
        v->base = v->own = NULL;
        return 0;
    }
    from = (const ModelFile *)v->base;
    v->count = from->nMeshAnims < MAX_ANIMS ? from->nMeshAnims : MAX_ANIMS;
    for (k = 0; k < v->count; k++) {
        s32 first, last, m, frames, pace;

        v->from_frames[k] = anims_of(from)[k].frames;
        m = find(to, stand_in(x, v->kind, anims_of(from)[k].hash, to, &first, &last, &pace));
        frames = anims_of(to)[m].frames > 0 ? anims_of(to)[m].frames : 1;
        first = first < 0 ? 0 : first >= frames ? frames - 1 : first;
        last = last < first ? frames - 1 : last >= frames ? frames - 1 : last;
        v->to_anim[k] = m;
        v->to_first[k] = first;
        v->to_count[k] = last - first + 1;
        v->pace[k] = pace;
    }
    v->clock_anim = -1;
    from_height = height_of(from);
    v->scale = from_height > 0 && height_of(to) > 0 ? height_of(to) * 0x1000 / from_height : 0x1000;
    return 1;
}

void extra_heroes_start(void) {
    s32 i;

    extra_heroes_stop();
    for (i = 0; i < 4; i++) {
        Playing *p = &Play[i];
        s32 base = PlayerBase[i];

        if (PlayerExtra[i] == EXTRA_NONE || ModelsFrom == NULL || Heroes[i].hero_sub == NULL || (u32)(base - 1) >= 8) {
            continue;
        }
        if (variant_start(i, &p->var[p->nvar], ModelsFrom[base])) {
            p->nvar++;
        }
        if (FlagFrom != NULL && FlagFrom[base] != ModelsFrom[base] && variant_start(i, &p->var[p->nvar], FlagFrom[base])) {
            p->nvar++;
        }
        if (p->nvar == 0) {
            continue;
        }
        p->sub = Heroes[i].hero_sub;
        p->on = 1;
        AnyPlaying = 1;
        body_box(i);
    }
}

/* Its body's box, which crates, TNT and Nitro and the other heroes stop against (FindCollisionBox's 0x40): the game takes it from a
 * hero's model as the hero's made (CreateHeroes, FetchDefaultColBox, from the mesh HeroEntities shows), before an extra hero's on,
 * when its own model, which has none, was all there was. So again now, from its base's mesh. */
static void body_box(s32 player) {
    EXTengine *eng = Heroes[player].hero_eng;
    subengine *sub = eng != NULL ? eng->sub : NULL;
    s32 was;

    if (sub == NULL || sub != Play[player].sub || (u32)(PlayerBase[player] - 1) >= 8) {
        return;
    }
    was = sub->entity;
    sub->entity = HeroEntities[PlayerBase[player]];
    FetchDefaultColBox(eng, 0x50);
    sub->entity = was;
}

/* A hero's sounds are a block from the one the announcer names it with (UpdateHeroVictory's first): its hit voices the third to the
 * sixth (HeroHitSfx, one at random as it's hit), and the rest the games' own (docs/EXTRA_HEROES.md). Nobody plays an extra hero's
 * base, so any of them is the extra hero's: its own for that one, or none, or (VOICE_BASE) the base's. */
s32 extra_hero_sound(s32 sfx) {
    s32 i, block;
    const Voice *v;

    for (i = 0; AnyPlaying && i < 4; i++) {
        if (!Play[i].on || (u32)(PlayerBase[i] - 1) >= 8) {
            continue;
        }
        block = HeroHitSfx[PlayerBase[i]][0] - HIT_FIRST;
        if (sfx < block || sfx >= block + HERO_SOUNDS) {
            continue;
        }
        for (v = HeroDefs[PlayerExtra[i]]->voices; v != NULL && v->offset >= 0; v++) {
            if (v->offset == sfx - block) {
                if (v->voice == VOICE_BASE) {
                    return 0;
                }
                bonus_heroes_voice(PlayerExtra[i], v->voice);
                return 1;
            }
        }
        return 1;
    }
    return 0;
}

s32 extra_hero_like(s32 player, s32 name) {
    s32 kind, like;

    if (player < 0 || player >= 4 || PlayerExtra[player] == EXTRA_NONE || ModelsFrom == NULL || (u32)(PlayerBase[player] - 1) >= 8
        || name != PlayerBase[player]) {
        return name;
    }
    kind = kind_of(FlagFrom != NULL ? FlagFrom[PlayerBase[player]] : ModelsFrom[PlayerBase[player]]);
    like = HeroDefs[PlayerExtra[player]]->kinds[kind].like;
    return like > 0 ? like : name;
}

s32 extra_hero_bank(s32 player, s32 bank) {
    s32 base;

    if (player < 0 || player >= 4 || PlayerExtra[player] == EXTRA_NONE || (u32)((base = PlayerBase[player]) - 1) >= 8) {
        return bank;
    }
    if ((ModelsFrom != NULL && bank == ModelsFrom[base]) || (FlagFrom != NULL && bank == FlagFrom[base])) {
        return bonus_heroes_bank(PlayerExtra[player], kind_of(bank));
    }
    return bank;
}

/* The extra hero a subengine draws, if it's one's own and shows one of its models or its base's in its place, and which. */
static Playing *playing(const subengine *sub, Variant **var) {
    s32 i, k;

    if (!AnyPlaying || sub == NULL) {
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        Playing *p = &Play[i];

        if (!p->on || (sub != p->sub && (Heroes[i].hero_anim == NULL || sub != Heroes[i].hero_anim->sub))) {
            continue;
        }
        for (k = 0; k < p->nvar; k++) {
            if (sub->u6C.dbase == p->var[k].bank || sub->u6C.dbase == &HeroModelBank[p->var[k].base_bank]) {
                *var = &p->var[k];
                return p;
            }
        }
    }
    return NULL;
}

const obj_dbase *extra_hero_dbase(const subengine *sub, const obj_dbase *db) {
    Variant *v;

    return playing(sub, &v) != NULL ? v->bank : db;
}

s32 extra_hero_frame(const subengine *sub, s32 entity) {
    Variant *v;
    Playing *p = playing(sub, &v);
    s32 tag = entity & 0x7000, n, f, m, frames, t;

    if (p == NULL) {
        return entity;
    }
    if (tag == 0x4000 || tag == 0x1000) {
        n = (entity >> 7) & 0x1F;
        if (n >= v->count) {
            return entity;
        }
        m = v->to_anim[n];
        frames = v->to_count[n] > 0 ? v->to_count[n] : 1;
        f = entity & 0x7F;
        /* the base's animation started, or one played once started again */
        if (n != v->clock_anim || (f < v->clock_frame && v->pace[n] != LOOP)) {
            v->clock_anim = n;
            v->clock_start = GameFrameCount;
        }
        v->clock_frame = f;
        if (v->pace[n] != STRETCH) {
            /* its model's frames to each of its game's, a tick */
            s32 per = HeroDefs[PlayerExtra[p - Play]]->kinds[v->kind].subframes;

            t = GameFrameCount - v->clock_start;
            t = (t < 0 ? 0 : t) * (per > 1 ? per : 1);
            f = v->pace[n] == LOOP ? t % frames : t;
        } else if (v->from_frames[n] > 0) {
            f = f * frames / v->from_frames[n];
        }
        if (f >= frames) {
            f = frames - 1;
        }
        return (entity & 0xF000) | m << 7 | (v->to_first[n] + f);
    }
    if (tag == 0x2000) {
        return (entity & 0xF000) | 1;
    }
    return entity;
}

void extra_hero_shape(const subengine *sub, s32 entity, const MeshHeader *mesh, SVECTOR *verts) {
    Variant *v;
    const Playing *p = playing(sub, &v);
    s32 keep, k, n, dy;
    Shape s;

    if (p == NULL || (keep = HeroDefs[PlayerExtra[p - Play]]->kinds[v->kind].keep_lift) >= 0x1000
        || ((entity & 0x7000) != 0x4000 && (entity & 0x7000) != 0x1000)
        || !shape_of(v->own, (entity >> 7) & 0x1F, entity & 0x7F, &s) || (dy = lowered(&s, keep)) == 0) {
        return;
    }
    n = vertex_count(mesh);
    for (k = 0; k < n && k < MORPH_VERTS; k++) {
        verts[k].vy = (s16)(verts[k].vy + dy);
    }
}

s32 extra_hero_points(const subengine *sub, PointList **list) {
    Variant *v;
    Playing *p = playing(sub, &v);
    const ModelFile *m;
    const PointList *from = NULL;
    PointList *out;
    FlagPoint head;
    Shape s;
    s32 x, n, f, k, held = 0, have_head, drawn, keep;

    if (p == NULL) {
        return 0;
    }
    *list = NULL;
    m = (const ModelFile *)v->base;
    x = sub->entity;
    switch (x & 0x7000) {
    case 0x4000:
        n = (x >> 7) & 0x1F;
        f = x & 0x7F;
        if (n < m->nMeshAnims && f < anims_of(m)[n].frames) {
            const MeshAnimFrame *fr = (const MeshAnimFrame *)(v->base + anims_of(m)[n].start + 4) + f;

            if (fr->points != 0) {
                from = (const PointList *)((const u8 *)&fr->points + fr->points);
            }
        }
        break;
    case 0x2000:
        k = (x & 0xFFF) - 1;
        if (k >= 0 && k < m->nMeshes && m->meshes[k].points != 0) {
            from = (const PointList *)((const u8 *)&m->meshes[k].points + m->meshes[k].points);
        }
        break;
    default:
        return 0;
    }
    if (from == NULL) {
        return 1;
    }
    if (from->count < 0 || from->nRecs < 0
        || 4 + (from->count + 1) * (s32)sizeof(FlagPoint) + from->nRecs * (s32)sizeof(FlagRecord) > MAX_POINTS) {
        return 1;
    }
    /* the top of its head, in the frame it's drawn in */
    keep = HeroDefs[PlayerExtra[p - Play]]->kinds[v->kind].keep_lift;
    drawn = extra_hero_frame(sub, x);
    have_head = (drawn & 0x7000) == 0x4000 && shape_of(v->own, (drawn >> 7) & 0x1F, drawn & 0x7F, &s);
    memset(&head, 0, sizeof(head));
    if (have_head) {
        head.vx = (s16)s.head_x;
        head.vy = (s16)(s.top + (keep < 0x1000 ? lowered(&s, keep) : 0));
        head.vz = (s16)s.head_z;
    }
    out = &p->points.list;
    out->count = 0;
    for (k = 0; k < from->count; k++) {
        FlagPoint pt = from->pts[k];

        if (have_head && (pt.flags & HELD_FLAGS)) {
            /* it holds things on its head */
            held = 1;
            continue;
        }
        if (have_head && (pt.flags & HEAD_FLAG)) {
            pt.vx = head.vx;
            pt.vy = head.vy;
            pt.vz = head.vz;
        } else {
            pt.vx = (s16)(pt.vx * v->scale >> 12);
            pt.vy = (s16)(pt.vy * v->scale >> 12);
            pt.vz = (s16)(pt.vz * v->scale >> 12);
        }
        out->pts[out->count++] = pt;
    }
    if (held) {
        head.flags = 1;
        out->pts[out->count++] = head;
    }
    /* the boxes it collides with stay the base's */
    out->nRecs = from->nRecs;
    memcpy(&out->pts[out->count], &from->pts[from->count], (size_t)from->nRecs * sizeof(FlagRecord));
    *list = out;
    return 1;
}

s32 extra_hero_anim_key(const obj_dbase *db, s32 key) {
    const ModelFile *m;
    s32 i, k;

    if (!AnyPlaying) {
        return key;
    }
    for (i = 0; i < 4; i++) {
        for (k = 0; Play[i].on && k < Play[i].nvar; k++) {
            if (Play[i].var[k].bank == db && (m = db->data) != NULL) {
                return has(m, key) ? key : stand_in(HeroDefs[PlayerExtra[i]], Play[i].var[k].kind, key, m, NULL, NULL, NULL);
            }
        }
    }
    return key;
}

/* --- its face, for the panel ---
 * The game has a picture of each hero's face for the panel, and none of an extra hero's, so one's made from its model on the player's
 * machine: its face animation's first frame, its head filling the picture, drawn with its own colours and textures at FACE_SCALE times
 * the panel's, with a dark edge as the game's faces have. */

#define FACE_SCALE 4        /* the picture's pixels to the panel's texels */
#define FACE_SAMPLES 2      /* drawn this much bigger again, and each pixel the mean of those */
#define FACE_ASPECT 1.25f   /* the panel shows its faces this much taller than their texels are wide */
#define FACE_HEAD 0.36f     /* the share of the model, from the top, that's its head */
#define FACE_ROOM 0.78f     /* and of the picture, from the top, it fills: the panel draws the health bar over the rest */

static u32 *Face[NUM_EXTRA];
static s32 FaceW[NUM_EXTRA], FaceH[NUM_EXTRA], FaceBank[NUM_EXTRA];

/* A texel of image `image` with clut `clut` (or its own, if -1), as red | green << 8 | blue << 16, and alpha 0 if it's clear. */
static u32 texel_of(const obj_textureset *ts, s32 image, s32 clut, s32 u, s32 v) {
    const obj_texture *t;
    const u8 *px;
    const u16 *pal;
    s32 i, c;
    u16 col;

    if (ts->tex == NULL || ts->base == NULL || image < 0 || image >= ts->nTex) {
        return 0xFF808080;
    }
    t = &ts->tex[image];
    if (t->w <= 0 || t->h <= 0) {
        return 0xFF808080;
    }
    u = u < 0 ? 0 : u >= t->w ? t->w - 1 : u;
    v = v < 0 ? 0 : v >= t->h ? t->h - 1 : v;
    px = (const u8 *)ts->base + t->pix;
    if (t->flag & 1) {
        i = px[v * ((t->w + 1) & ~1) + u];
    } else {
        i = (px[v * ((t->w + 3) / 4 * 2) + u / 2] >> ((u & 1) * 4)) & 15;
    }
    c = clut >= 0 ? clut : t->clut;
    if (c == 0x7FFF || c < 0 || c >= ts->nEnt || ts->ent == NULL) {
        return 0xFF808080;
    }
    pal = (const u16 *)((const u8 *)ts->base + ts->ent[c].off);
    col = pal[i];
    if (col == 0) {
        return 0;
    }
    return 0xFF000000u | (u32)((col & 0x1F) * 255 / 31) | (u32)(((col >> 5) & 0x1F) * 255 / 31) << 8
         | (u32)(((col >> 10) & 0x1F) * 255 / 31) << 16;
}

/* Its model's face frame, drawn: w x h, with depth. */
static void draw_face(const HeroDef *x, const u8 *file, const obj_textureset *ts, u32 *rgba, s32 w, s32 h, float aspect, float room) {
    const ModelFile *m = (const ModelFile *)file;
    Frame fr;
    float *depth, *sx, *sy, *sz;
    float turn = x->face_turn * 3.14159265f / 180.0f, c = cosf(turn), sn = sinf(turn);
    float minx = 1e9f, maxx = -1e9f, miny = 1e9f, maxy = -1e9f, top = 1e9f, bottom = -1e9f, scale, ox, oy;
    const u16 *strips, *uv_idx, *texs, *col_idx, *uvs;
    const u8 *colours;
    s32 k, v[3], face, corner, left = 0, tex = 0, i;

    if (!frame_at(file, find(m, hash_of(x->face)), 0, &fr) || fr.count <= 0) {
        return;
    }
    sx = malloc(sizeof(float) * (size_t)fr.count * 3);
    depth = malloc(sizeof(float) * (size_t)w * h);
    if (sx == NULL || depth == NULL) {
        free(sx);
        free(depth);
        return;
    }
    sy = sx + fr.count;
    sz = sy + fr.count;
    /* turned round, then its head (its top share) fitted to the picture: the panel shows texels taller than wide, so across is more */
    for (k = 0; k < fr.count; k++) {
        vertex_of(&fr, k, v);
        sx[k] = v[0] * c + v[2] * sn;
        sz[k] = v[2] * c - v[0] * sn;
        sy[k] = (float)v[1];
        top = sy[k] < top ? sy[k] : top;
        bottom = sy[k] > bottom ? sy[k] : bottom;
    }
    for (k = 0; k < fr.count; k++) {
        if (sy[k] <= top + (bottom - top) * FACE_HEAD) {
            minx = sx[k] < minx ? sx[k] : minx;
            maxx = sx[k] > maxx ? sx[k] : maxx;
            miny = sy[k] < miny ? sy[k] : miny;
            maxy = sy[k] > maxy ? sy[k] : maxy;
        }
    }
    scale = fminf(w * 0.92f / ((maxx - minx) * aspect + 1), h * (room - 0.04f) / (maxy - miny + 1));
    ox = w * 0.5f - (minx + maxx) * 0.5f * scale * aspect;
    oy = h * 0.04f - miny * scale;
    for (k = 0; k < fr.count; k++) {
        sx[k] = ox + sx[k] * scale * aspect;
        sy[k] = oy + sy[k] * scale;
    }
    for (i = 0; i < w * h; i++) {
        depth[i] = 1e30f;
        rgba[i] = 0;
    }
    /* the faces: strips of triangles, each with its colours (3 from the model's table), and its texture and texture corners */
    strips = (const u16 *)((const u8 *)&fr.mesh->runs + fr.mesh->runs);
    uv_idx = (const u16 *)((const u8 *)&fr.mesh->uvIdx + fr.mesh->uvIdx);
    texs = (const u16 *)((const u8 *)&fr.mesh->texs + fr.mesh->texs);
    col_idx = (const u16 *)((const u8 *)&fr.mesh->colors + fr.mesh->colors);
    uvs = (const u16 *)((const u8 *)&m->uvs + m->uvs);
    colours = (const u8 *)&m->colors + m->colors;
    for (face = 0, corner = 0; *strips != 0xFFFF; corner += (*strips >> 8) + 2, strips++) {
        s32 n = *strips >> 8, flat = *strips & 1, j;

        for (j = 0; j < n; j++, face++) {
            s32 a = corner + j, b = a + 1, d = a + 2, image = -1, clut = -1, cx0, cy0, cx1, cy1, px, py;
            const u8 *col = colours + 4 * (col_idx[face] & 0x1FFF);
            const u16 *uv = uvs + uv_idx[face];
            float area;

            if (--left < 0) {
                tex = texs[0];
                texs++;
                left = (tex >> 9) & 0x3F;
            }
            if (!flat) {
                if (tex & 0x8000) {
                    image = ts->nTex - 1;
                    clut = tex & 0x1FF;
                } else {
                    image = tex & 0x1FF;
                }
            }
            if (d >= fr.count) {
                continue;
            }
            area = (sx[b] - sx[a]) * (sy[d] - sy[a]) - (sy[b] - sy[a]) * (sx[d] - sx[a]);
            if (fabsf(area) < 1e-6f) {
                continue;
            }
            cx0 = (s32)floorf(fminf(sx[a], fminf(sx[b], sx[d])));
            cx1 = (s32)ceilf(fmaxf(sx[a], fmaxf(sx[b], sx[d])));
            cy0 = (s32)floorf(fminf(sy[a], fminf(sy[b], sy[d])));
            cy1 = (s32)ceilf(fmaxf(sy[a], fmaxf(sy[b], sy[d])));
            cx0 = cx0 < 0 ? 0 : cx0;
            cy0 = cy0 < 0 ? 0 : cy0;
            cx1 = cx1 > w - 1 ? w - 1 : cx1;
            cy1 = cy1 > h - 1 ? h - 1 : cy1;
            for (py = cy0; py <= cy1; py++) {
                for (px = cx0; px <= cx1; px++) {
                    float qx = px + 0.5f, qy = py + 0.5f;
                    float wa = ((sx[b] - qx) * (sy[d] - qy) - (sy[b] - qy) * (sx[d] - qx)) / area;
                    float wb = ((sx[d] - qx) * (sy[a] - qy) - (sy[d] - qy) * (sx[a] - qx)) / area;
                    float wd = 1.0f - wa - wb, z;
                    u32 out, t = 0xFF808080;
                    s32 ch;

                    if (wa < 0 || wb < 0 || wd < 0) {
                        continue;
                    }
                    z = wa * sz[a] + wb * sz[b] + wd * sz[d];
                    if (z >= depth[py * w + px]) {
                        continue;
                    }
                    if (image >= 0) {
                        float u = wa * (uv[0] & 0xFF) + wb * (uv[1] & 0xFF) + wd * (uv[2] & 0xFF);
                        float vv = wa * (uv[0] >> 8) + wb * (uv[1] >> 8) + wd * (uv[2] >> 8);

                        t = texel_of(ts, image, clut, (s32)u, (s32)vv);
                        if ((t >> 24) == 0) {
                            continue;
                        }
                    }
                    /* the face's colours, Gouraud; on a texture they tint it, 0x80 as it is */
                    out = 0xFF000000u;
                    for (ch = 0; ch < 3; ch++) {
                        float shade = wa * col[ch] + wb * col[4 + ch] + wd * col[8 + ch];
                        float value = image >= 0 ? ((t >> (ch * 8)) & 0xFF) * shade / 128.0f : shade;

                        out |= (u32)(value > 255 ? 255 : value < 0 ? 0 : value) << (ch * 8);
                    }
                    depth[py * w + px] = z;
                    rgba[py * w + px] = out;
                }
            }
        }
    }
    free(sx);
    free(depth);
}

/* A hero card on the panel (64 x 32 texels, as the game's faces are, which fill it across and down to row 24): right across, its head
 * CARD_FACE_ROWS texels tall as theirs are, from CARD_TOP down, and what hangs from it (Roo's tongue) on down to CARD_BAR, the health
 * bar's top, so it goes on behind the bar. */
#define CARD_FACE_ROWS 24.0f
#define CARD_TOP 0.6f
#define CARD_BAR 26

/* Its own game's face for it (bonusheroes.c), drawn into the top left fw x fh of a picture `stride` wide: across it but `inset` of its
 * width each side, a card row `rows` texels tall from oy, and none of it from row `stop` on; each pixel the mean of the card's under
 * it. */
static void card_face(const u32 *card, s32 cw, s32 ch, u32 *out, s32 stride, s32 fw, s32 fh, float inset, float rows, float oy, s32 stop) {
    float cols = fw * (1 - 2 * inset) / cw, ox = fw * inset;
    s32 px, py, i, j;

    for (py = 0; py < fh; py++) {
        for (px = 0; px < fw; px++) {
            u32 r = 0, g = 0, b = 0, a = 0;

            for (j = 0; j < 4; j++) {
                for (i = 0; i < 4; i++) {
                    s32 x = (s32)floorf((px + (i + 0.5f) / 4 - ox) / cols), y = (s32)floorf((py + (j + 0.5f) / 4 - oy) / rows);
                    u32 c = x >= 0 && y >= 0 && x < cw && y < ch && py < stop ? card[y * cw + x] : 0, ca = c >> 24;

                    r += (c & 0xFF) * ca;
                    g += (c >> 8 & 0xFF) * ca;
                    b += (c >> 16 & 0xFF) * ca;
                    a += ca;
                }
            }
            out[py * stride + px] = a == 0 ? 0 : (r / a) | (g / a) << 8 | (b / a) << 16 | (a / 16) << 24;
        }
    }
}

/* Its face for the panel, w x h texels: its own game's, at the panel's texels as the others are, or else drawn from its model; made
 * once, when its model and textures are loaded. */
static const u32 *face_of(s32 extra, s32 w, s32 h) {
    const HeroDef *x = HeroDefs[extra];
    const obj_dbase *bank = &HeroModelBank[foot_bank(extra)];
    s32 bw = w * FACE_SCALE * FACE_SAMPLES, bh = h * FACE_SCALE * FACE_SAMPLES, fw = w * FACE_SCALE, fh = h * FACE_SCALE;
    s32 px, py, i, j, k, cw, ch, head;
    u32 *big, *out;
    const u32 *card = bonus_heroes_card(extra, &cw, &ch, &head);
    u8 *file;

    if (card != NULL) {
        fw = w;
        fh = h;
    }
    if (Face[extra] != NULL && FaceW[extra] == fw && FaceH[extra] == fh && FaceBank[extra] == foot_bank(extra)) {
        return Face[extra];
    }
    if (card != NULL) {
        if ((out = malloc(sizeof(u32) * (size_t)fw * fh)) == NULL) {
            return NULL;
        }
        card_face(card, cw, ch, out, fw, fw, fh, 0, CARD_FACE_ROWS * h / 32 / (head > 0 ? head : ch), CARD_TOP * h / 32, CARD_BAR * h / 32);
        goto made;
    }
    if (bank->textures.base == NULL || bank->textures.tex == NULL || (file = foot_file(extra)) == NULL) {
        return NULL;
    }
    big = malloc(sizeof(u32) * (size_t)bw * bh);
    out = malloc(sizeof(u32) * (size_t)fw * fh);
    if (big == NULL || out == NULL) {
        free(big);
        free(out);
        free(file);
        return NULL;
    }
    draw_face(x, file, &bank->textures, big, bw, bh, FACE_ASPECT, FACE_ROOM);
    free(file);
    /* each pixel the mean of its samples, its colour weighed by how solid each is */
    for (py = 0; py < fh; py++) {
        for (px = 0; px < fw; px++) {
            u32 r = 0, g = 0, b = 0, a = 0;

            for (j = 0; j < FACE_SAMPLES; j++) {
                for (i = 0; i < FACE_SAMPLES; i++) {
                    u32 c = big[(py * FACE_SAMPLES + j) * bw + px * FACE_SAMPLES + i], ca = c >> 24;

                    r += (c & 0xFF) * ca;
                    g += ((c >> 8) & 0xFF) * ca;
                    b += ((c >> 16) & 0xFF) * ca;
                    a += ca;
                }
            }
            out[py * fw + px] = a == 0 ? 0 : (r / a) | (g / a) << 8 | (b / a) << 16 | (a / (FACE_SAMPLES * FACE_SAMPLES)) << 24;
        }
    }
    /* a dark edge round it, half a texel wide */
    for (k = 0; k < fw * fh; k++) {
        big[k] = out[k];
    }
    for (py = 0; py < fh; py++) {
        for (px = 0; px < fw; px++) {
            s32 near = 0;

            if ((big[py * fw + px] >> 24) == 0xFF) {
                continue;
            }
            for (j = -FACE_SCALE / 2; j <= FACE_SCALE / 2 && !near; j++) {
                for (i = -FACE_SCALE / 2; i <= FACE_SCALE / 2 && !near; i++) {
                    s32 qx = px + i, qy = py + j;

                    near = qx >= 0 && qy >= 0 && qx < fw && qy < fh && (big[qy * fw + qx] >> 24) != 0;
                }
            }
            if (near) {
                u32 c = big[py * fw + px], ca = c >> 24, edge = 0xFF000000u;
                s32 ch;

                /* what's partly there stays over the edge's dark */
                for (ch = 0; ch < 3; ch++) {
                    edge |= (((c >> (ch * 8)) & 0xFF) * ca / 255 + 12 * (255 - ca) / 255) << (ch * 8);
                }
                out[py * fw + px] = edge;
            }
        }
    }
    free(big);
made:
    free(Face[extra]);
    Face[extra] = out;
    FaceW[extra] = fw;
    FaceH[extra] = fh;
    FaceBank[extra] = foot_bank(extra);
    return out;
}

const GfxTex *extra_hero_face(s32 index, s32 w, s32 h) {
    s32 i;

    if (!extra_heroes_on()) {
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        if (PlayerExtra[i] != EXTRA_NONE && PlayerBase[i] != 0 && (IconImage[PlayerBase[i]] & 0xFFF) == index) {
            const u32 *face = face_of(PlayerExtra[i], w, h);

            if (face == NULL) {
                return NULL;
            }
            return gfx_texture_art(0x4558545241464143ULL + ((uint64_t)FaceBank[PlayerExtra[i]] << 20) + (uint64_t)PlayerExtra[i] * 0x10000
                                       + (uint64_t)index,
                                   face, FaceW[PlayerExtra[i]], FaceH[PlayerExtra[i]], w, h);
        }
    }
    return NULL;
}

/* --- the character select (titles.c) ---
 * An extra hero's card is the ninth, the first of a third row under the red one: a frame and a portrait of the game's own kinds, the portrait
 * one of the game's in its place on the screen, and as it's drawn the extra hero's own instead: its face on a gradient, as the others'
 * are. Picked, it stands on its stand in its own model, which loads while the select's up. */

#define SELECT_ROWS 38      /* of a portrait's 64 rows, those the game's own use, and of its 64 columns: the rest is clear, and */
#define SELECT_COLS 56      /* the frame's border shows there */
#define SELECT_ASPECT 1.83f /* and how much taller than wide the select shows its texels, a row to each of 240 lines */
#define SELECT_ROOM 1.06f   /* the share of those a face drawn from a model fills: a close-up, as the others are, its ears to the edges */
#define SELECT_INSET 0.05f  /* and a hero card's room each side, so its ears clear the frame */

static const subengine *SelectPortrait;
static s32 SelectBank = -1; /* the model loaded for its stand, while the select's up */
static u32 *Portrait;
static s32 PortraitW, PortraitH, PortraitBank;
static s16 SelectSteps[136];

void extra_heroes_select_enter(const subengine *portrait) {
    SelectPortrait = portrait;
    if (portrait != NULL && extra_heroes_on()) {
        obj_dbase *db = &HeroModelBank[foot_bank(EXTRA_ROO)];

        if (db->data == NULL && db->load == 0) {
            dbase_asload_with_anims(db, NULL);
            SelectBank = foot_bank(EXTRA_ROO);
        }
    }
}

void extra_heroes_select_leave(void) {
    SelectPortrait = NULL;
    if (SelectBank >= 0) {
        dbase_release(&HeroModelBank[SelectBank]);
        SelectBank = -1;
    }
}

void *extra_hero_select_steps(s32 extra) {
    u8 *file = extra > EXTRA_NONE && extra < NUM_EXTRA ? foot_file(extra) : NULL;
    const ModelFile *m = (const ModelFile *)file;
    s32 n, frames, step, f, k = 0;

    if (file == NULL) {
        return NULL;
    }
    /* its face animation, the one it stands in, round and round, a frame of its game's every other tick: its model's frames a tick
     * (DoAnimCommand: a speed, then the frames, each shown that many ticks more), half its subframes apart */
    n = find(m, hash_of(HeroDefs[extra]->face));
    frames = n < m->nMeshAnims && anims_of(m)[n].frames > 0 ? anims_of(m)[n].frames : 1;
    free(file);
    step = HeroDefs[extra]->kinds[KIND_FOOT].subframes / 2;
    step = step < 1 ? 1 : step;
    SelectSteps[k++] = (s16)(step < 2 && HeroDefs[extra]->kinds[KIND_FOOT].subframes < 2 ? 1 : 0);
    for (f = 0; f < frames && k < (s32)(sizeof(SelectSteps) / sizeof(SelectSteps[0])) - 2; f += step) {
        SelectSteps[k++] = (s16)(0x4000 | n << 7 | f);
    }
    SelectSteps[k++] = (s16)(u16)0x8000;
    SelectSteps[k] = 0;
    return SelectSteps;
}

/* How big it stands on its stand (CharCard.target): a hero 640 tall at 0x9999, as Kong is about. */
s32 extra_hero_select_scale(s32 extra) {
    u8 *file = extra > EXTRA_NONE && extra < NUM_EXTRA ? foot_file(extra) : NULL;
    s32 target = 0x9999;
    Shape s;

    if (file != NULL && shape_of(file, find((const ModelFile *)file, hash_of(HeroDefs[extra]->face)), 0, &s) && s.top < -100) {
        target = 0x9999 * 640 / -s.top;
        target = target < 0x2000 ? 0x2000 : target > 0xCCCC ? 0xCCCC : target;
    }
    free(file);
    return target;
}

/* A portrait's picture, fw x fh: its top left pw x ph (the game's own portraits' SELECT_COLS x SELECT_ROWS) a gradient from yellow
 * to orange, that his blue shows against, with face (as wide as the picture, or none) over it; the rest clear. */
static void select_picture(u32 *out, s32 fw, s32 fh, s32 pw, s32 ph, const u32 *face) {
    s32 x, y, k;

    for (y = 0; y < fh; y++) {
        u32 bg[3];

        bg[0] = 255;
        bg[1] = (u32)(224 - 104 * y / (ph > 0 ? ph : 1));
        bg[2] = (u32)(72 - 56 * y / (ph > 0 ? ph : 1));
        for (x = 0; x < fw; x++) {
            u32 c = y < ph && x < pw && face != NULL ? face[y * fw + x] : 0, a = c >> 24, mixed = 0xFF000000u;

            for (k = 0; k < 3; k++) {
                mixed |= ((c >> (k * 8) & 0xFF) * a + bg[k] * (255 - a)) / 255 << (k * 8);
            }
            out[y * fw + x] = y < ph && x < pw ? mixed : 0;
        }
    }
}

/* Its portrait, w x h texels: its face on the gradient, its own game's at the select's texels as the others' are, or else drawn from
 * its model at FACE_SCALE. NULL until its model's loaded, if there's no card. */
static const u32 *portrait_of(s32 extra, s32 w, s32 h) {
    s32 cw, ch, head, x, y, i, j;
    const u32 *card = bonus_heroes_card(extra, &cw, &ch, &head);
    s32 scale = card != NULL ? 1 : FACE_SCALE;
    s32 fw = w * scale, fh = h * scale, ph = h > SELECT_ROWS ? fh * SELECT_ROWS / h : fh, pw = w > SELECT_COLS ? fw * SELECT_COLS / w : fw;
    s32 bw = pw * FACE_SAMPLES, bh = ph * FACE_SAMPLES;
    const obj_dbase *bank = &HeroModelBank[foot_bank(extra)];
    u32 *face, *out;

    if (Portrait != NULL && PortraitW == fw && PortraitH == fh && PortraitBank == foot_bank(extra)) {
        return Portrait;
    }
    if (card == NULL && (bank->textures.base == NULL || bank->textures.tex == NULL)) {
        return NULL;
    }
    face = malloc(sizeof(u32) * (size_t)fw * fh);
    out = malloc(sizeof(u32) * (size_t)fw * fh);
    if (face == NULL || out == NULL) {
        free(face);
        free(out);
        return NULL;
    }
    if (card != NULL) {
        /* all of it, his tongue too, across and down the part of the picture the game's own use */
        card_face(card, cw, ch, face, fw, pw, ph, SELECT_INSET, (float)ph / ch, 0, ph);
    } else {
        u32 *big = malloc(sizeof(u32) * (size_t)bw * bh);
        u8 *file = foot_file(extra);

        memset(face, 0, sizeof(u32) * (size_t)fw * ph);
        if (big != NULL && file != NULL) {
            draw_face(HeroDefs[extra], file, &bank->textures, big, bw, bh, SELECT_ASPECT, SELECT_ROOM);
            for (y = 0; y < ph; y++) {
                for (x = 0; x < pw; x++) {
                    u32 r = 0, g = 0, b = 0, a = 0;

                    for (j = 0; j < FACE_SAMPLES; j++) {
                        for (i = 0; i < FACE_SAMPLES; i++) {
                            u32 c = big[(y * FACE_SAMPLES + j) * bw + x * FACE_SAMPLES + i], ca = c >> 24;

                            r += (c & 0xFF) * ca;
                            g += (c >> 8 & 0xFF) * ca;
                            b += (c >> 16 & 0xFF) * ca;
                            a += ca;
                        }
                    }
                    face[y * fw + x] = a == 0 ? 0 : (r / a) | (g / a) << 8 | (b / a) << 16 | (a / (FACE_SAMPLES * FACE_SAMPLES)) << 24;
                }
            }
        }
        free(big);
        free(file);
    }
    select_picture(out, fw, fh, pw, ph, face);
    free(face);
    free(Portrait);
    Portrait = out;
    PortraitW = fw;
    PortraitH = fh;
    PortraitBank = foot_bank(extra);
    return out;
}

const GfxTex *extra_hero_menu_portrait(s32 index, s32 w, s32 h) {
    /* titles.c's TeamModeHeroFrames: each hero's portrait */
    static const u16 Portraits[9] = { 0, 0x6B, 0x67, 0x69, 0x7B, 0x73, 0x71, 0x6D, 0x6F };
    const u32 *art;
    s32 i, hero;

    if (SelectPortrait != NULL || !extra_heroes_on()) {
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        hero = GAME->heroChoice[i];
        if (PlayerExtra[i] != EXTRA_NONE && hero >= 1 && hero <= 8 && Portraits[hero] == index) {
            art = portrait_of(PlayerExtra[i], w, h);
            return art != NULL ? gfx_texture_art(0x504F525452414954ULL + ((uint64_t)PortraitBank << 20), art, PortraitW, PortraitH, w, h) : NULL;
        }
    }
    return NULL;
}

const GfxTex *extra_hero_portrait(const subengine *sub, s32 w, s32 h) {
    static u32 *blank;
    static s32 blank_size;
    const u32 *art;
    s32 fw = w * FACE_SCALE, fh = h * FACE_SCALE;

    if (sub == NULL || sub != SelectPortrait || !extra_heroes_on() || w <= 0 || h <= 0) {
        return NULL;
    }
    if ((art = portrait_of(EXTRA_ROO, w, h)) != NULL) {
        return gfx_texture_art(0x504F525452414954ULL + ((uint64_t)PortraitBank << 20), art, PortraitW, PortraitH, w, h);
    }
    /* its model's still loading: the gradient alone, not another's face */
    if (blank_size < fw * fh) {
        free(blank);
        blank = malloc(sizeof(u32) * (size_t)fw * fh);
        blank_size = blank != NULL ? fw * fh : 0;
    }
    if (blank == NULL) {
        return NULL;
    }
    select_picture(blank, fw, fh, w > SELECT_COLS ? fw * SELECT_COLS / w : fw, h > SELECT_ROWS ? fh * SELECT_ROWS / h : fh, NULL);
    return gfx_texture_art(0x424C414E4B504F52ULL, blank, fw, fh, w, h);
}
