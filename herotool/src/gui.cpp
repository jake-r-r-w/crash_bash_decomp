// The hero tool's window (docs/HERO_PACKS.md, The hero tool): the same steps as `herotool import`, with the model to look at.
//
// Drop a model (.glb) on it, or open one. A rigged model's own bones are found by their names; an unrigged one gets markers guessed
// from its shape, to be dragged onto its joints in the front and side views (its left side's mirror its right's unless that's turned
// off). The moves play on it as the game will play them. Set its name, height and the rest, add a card and sounds (drop a .png or a
// .wav on it), then Export writes the mod, and Test in game starts the game with the hero in it. "Make it in every game" has the game
// make it, as it does when it loads, and shows each game's model: what it rides or holds, and every move it has there.
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "kit.h"

namespace {

const int ViewW = 300, ViewH = 420;
const int PrevW = 600, PrevH = 560; // the view of what the game made
const int ThumbW = 220, ThumbH = 180; // each game's in the gallery of them all

struct View {
    SDL_Texture *tex = nullptr;
    unsigned *rgba = nullptr;
    float *depth = nullptr;
    // model to screen: x across (from its axes, turned by yaw), y up, both scaled and moved to fit
    float yaw = 0, scale = 1, ox = 0, oy = 0;
    int w = 0, h = 0; // its size, if not the preview's (PrevW x PrevH): draw_made's only
};

enum { OWN_TEMPLATE = -1, OWN_CLIP = -2 };
const int PAGE_MODEL = -2, PAGE_ALL = -1;
enum { PICK_MODEL, PICK_CARD, PICK_VOICE };

struct Tool {
    KitModel *m = nullptr;
    char model[1024] = "";
    char status[2400] = "";
    bool rigged = false, mirror = true, playing = true;
    int move = 0;
    float t = 0, turn = 30;
    float (*posed)[3] = nullptr;
    View front, side, play;
    // the pack
    char id[32] = "myhero", name[32] = "MY HERO", mod_name[64] = "My hero", author[64] = "", card[1024] = "";
    char voice[PACK_VOICE_JOBS][1024] = {};
    int height = 560, prefer = 0, voice_job = 1, mode = 0, template_hero = 0;
    float face_head = 0;
    char out[1024] = "", game[1024] = "";
    // a marker being dragged: its bone, whether it's the bone's end, and in which view
    int drag_bone = -1, drag_end = 0, drag_view = 0;
    // what the game made of it (the preview): each kind's model, the game making it, and what's shown
    KitMade made[PACK_KINDS] = {};
    bool have_made[PACK_KINDS] = {};
    // what the game draws with it beside its own model in a game (the Ballistix car, the tank): drawn with it, as still
    KitMade rides[PACK_KINDS][2] = {};
    int nrides[PACK_KINDS] = {};
    SDL_Process *maker = nullptr;
    char made_dir[1100] = "";
    int kind = 0, anim = 0;
    float made_t = 0, made_turn = 30, made_speed = 1;
    View game_view;
    // its own moves in the games' places (KIND/MOVE=OURS), and the one being added
    // each game's moves (KitGames): OWN_TEMPLATE its template's, OWN_CLIP its model's own clip, else one of the tool's (KitMoves)
    int ownmap[8][8] = {};
    // the page shown (PAGE_MODEL, PAGE_ALL or a kind), the model page's view, and whether a change wants the preview made again
    int page = -1, model_view = 0;
    bool dirty = false;
    // each game's tile on the page of them all, and whether the game's been asked to make it for this model yet
    View thumbs[PACK_KINDS];
    // each game's ride or prop from another hero than its template, -1 its template's (hero.json's kinds' "template")
    int kind_template[PACK_KINDS] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    bool made_asked = false;
    // the start screen, till a model's opened or a hero loaded; a loaded hero's mod folder, which Export writes back to
    bool home = true;
    char loaded_mod[1100] = "";
};

// The kinds of model the game makes (heropack.c's names), as the games call them
const char *const KindTitles[PACK_KINDS] = { "On foot (Crate Crush, Ring Ding and the others)", "Polar Push: the polar bear", "Pogo",
                                             "Ballistix: the car", "Tank", "Crash Dash: the race cart", "Splash Dash: the fish",
                                             "Mallet Mash: the mallet", "Dragon Drop: the dino", "Keg Kaboom: the keg", "The podium" };

const int KIND_WARP_TOOL = PACK_KINDS - 1; // the podium: its model on foot's, nothing of its own to pick
const char *const KindShort[PACK_KINDS] = { "On foot", "Polar Push: bear", "Pogo Painter: pogo", "Ballistix: car", "Tank",
                                            "Crash Dash: cart", "Splash Dash: fish", "Mallet Mash: mallet", "Dragon Drop: dino",
                                            "Keg Kaboom: keg", "The podium" };

const char *const HeroNames[] = { "Crash", "Coco", "Cortex", "Brio", "Tiny", "Koala Kong", "Dingodile", "Rilla Roo" };
const char *const Heroes[] = { "crash", "coco", "cortex", "brio", "tiny", "kong", "dingodile", "rilla" };
const struct {
    const char *name;
    int mode;
} Modes[] = { { "Crate Crush", 0 }, { "Ring Ding", 27 }, { "Polar Push", 18 }, { "Pogo", 10 }, { "Tournament podium", 43 } };

// --- posing: the standard skeleton's bones turned and moved as a move says, the mesh with them ---

void q_rot(const float q[4], const float v[3], float out[3]) {
    float x = q[0], y = q[1], z = q[2], w = q[3];
    float tx = 2 * (y * v[2] - z * v[1]), ty = 2 * (z * v[0] - x * v[2]), tz = 2 * (x * v[1] - y * v[0]);

    out[0] = v[0] + w * tx + (y * tz - z * ty);
    out[1] = v[1] + w * ty + (z * tx - x * tz);
    out[2] = v[2] + w * tz + (x * ty - y * tx);
}

void q_mul(const float a[4], const float b[4], float o[4]) {
    float r[4] = { a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1], a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0],
                   a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3], a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2] };
    memcpy(o, r, sizeof(r));
}

void pose(Tool &T) {
    KitModel *m = T.m;
    float rot[KB_COUNT][4], hips[3], wq[KB_COUNT][4], wp[KB_COUNT][3];

    kit_pose(m, T.move, T.t, rot, hips);
    for (int b = 0; b < KB_COUNT; b++) {
        int p = KitBoneParent[b];

        if (!m->bones[b].have) {
            continue;
        }
        if (p < 0) {
            memcpy(wq[b], rot[b], sizeof(wq[b]));
            for (int i = 0; i < 3; i++) {
                wp[b][i] = m->bones[b].at[i] + hips[i];
            }
        } else {
            float d[3], r[3];

            for (int i = 0; i < 3; i++) {
                d[i] = m->bones[b].at[i] - m->bones[p].at[i];
            }
            q_rot(wq[p], d, r);
            for (int i = 0; i < 3; i++) {
                wp[b][i] = wp[p][i] + r[i];
            }
            q_mul(wq[p], rot[b], wq[b]);
        }
    }
    for (int v = 0; v < m->nvert; v++) {
        float s[3] = { 0, 0, 0 };

        for (int k = 0; k < 4; k++) {
            int b = m->joint[v][k];
            float w = m->weight[v][k], d[3], r[3];

            if (w <= 0 || !m->bones[b].have) {
                continue;
            }
            for (int i = 0; i < 3; i++) {
                d[i] = m->pos[v][i] - m->bones[b].at[i];
            }
            q_rot(wq[b], d, r);
            for (int i = 0; i < 3; i++) {
                s[i] += (wp[b][i] + r[i]) * w;
            }
        }
        memcpy(T.posed[v], s, sizeof(s));
    }
}

// --- drawing it: a little software renderer, its texture sampled, lit from the front ---

void fit(const KitModel *m, View &v) {
    float h = m->hi[1] - m->lo[1], w = fmaxf(m->hi[0] - m->lo[0], m->hi[2] - m->lo[2]);

    v.scale = fminf(ViewH * 0.85f / h, ViewW * 0.85f / (w > 0 ? w : 1));
    v.ox = ViewW / 2.0f;
    v.oy = ViewH * 0.93f + m->lo[1] * v.scale;
}

void to_screen(const View &v, const float p[3], float &sx, float &sy, float &sz) {
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
    float x = p[0] * c - p[2] * s, z = p[0] * s + p[2] * c;

    sx = v.ox + x * v.scale;
    sy = v.oy - p[1] * v.scale;
    sz = -z;
}

void draw(const KitModel *m, const float (*pos)[3], View &v, SDL_Renderer *ren) {
    if (v.rgba == nullptr) {
        v.rgba = new unsigned[ViewW * ViewH];
        v.depth = new float[ViewW * ViewH];
        v.tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, ViewW, ViewH);
    }
    for (int i = 0; i < ViewW * ViewH; i++) {
        v.rgba[i] = 0xFF3A3434;
        v.depth[i] = 1e30f;
    }
    for (int t = 0; t < m->ntri; t++) {
        float sx[3], sy[3], sz[3], n[3], e1[3], e2[3];
        const int *tri = m->tri[t];
        const PackImage *im = m->tri_image[t] >= 0 && m->tri_image[t] < m->nimages ? &m->images[m->tri_image[t]] : nullptr;

        for (int j = 0; j < 3; j++) {
            to_screen(v, pos[tri[j]], sx[j], sy[j], sz[j]);
        }
        for (int i = 0; i < 3; i++) {
            e1[i] = pos[tri[1]][i] - pos[tri[0]][i];
            e2[i] = pos[tri[2]][i] - pos[tri[0]][i];
        }
        n[0] = e1[1] * e2[2] - e1[2] * e2[1];
        n[1] = e1[2] * e2[0] - e1[0] * e2[2];
        n[2] = e1[0] * e2[1] - e1[1] * e2[0];
        float ln = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]), c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
        float facing = ln > 0 ? ((n[0] * s + n[2] * c) * 0.7f + n[1] * 0.5f) / ln : 0, lit = 0.55f + 0.45f * fmaxf(0, facing);
        float area = (sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]);

        if (fabsf(area) < 1e-6f) {
            continue;
        }
        int x0 = (int)fmaxf(0, floorf(fminf(sx[0], fminf(sx[1], sx[2])))), x1 = (int)fminf(ViewW - 1, ceilf(fmaxf(sx[0], fmaxf(sx[1], sx[2]))));
        int y0 = (int)fmaxf(0, floorf(fminf(sy[0], fminf(sy[1], sy[2])))), y1 = (int)fminf(ViewH - 1, ceilf(fmaxf(sy[0], fmaxf(sy[1], sy[2]))));
        for (int y = y0; y <= y1; y++) {
            for (int x = x0; x <= x1; x++) {
                float px = x + 0.5f, py = y + 0.5f;
                float a = ((sx[1] - px) * (sy[2] - py) - (sx[2] - px) * (sy[1] - py)) / area;
                float b = ((sx[2] - px) * (sy[0] - py) - (sx[0] - px) * (sy[2] - py)) / area, d = 1 - a - b, z;

                if (a < 0 || b < 0 || d < 0) {
                    continue;
                }
                z = a * sz[0] + b * sz[1] + d * sz[2];
                if (z >= v.depth[y * ViewW + x]) {
                    continue;
                }
                float col[3];
                for (int ch = 0; ch < 3; ch++) {
                    col[ch] = (a * m->rgba[tri[0]][ch] + b * m->rgba[tri[1]][ch] + d * m->rgba[tri[2]][ch]) * 255;
                }
                if (im != nullptr) {
                    float u = a * m->uv[tri[0]][0] + b * m->uv[tri[1]][0] + d * m->uv[tri[2]][0];
                    float w = a * m->uv[tri[0]][1] + b * m->uv[tri[1]][1] + d * m->uv[tri[2]][1];
                    int tx = (int)(u * im->w), ty = (int)(w * im->h);

                    tx = tx < 0 ? 0 : tx >= im->w ? im->w - 1 : tx;
                    ty = ty < 0 ? 0 : ty >= im->h ? im->h - 1 : ty;
                    for (int ch = 0; ch < 3; ch++) {
                        col[ch] = col[ch] * im->rgba[((size_t)ty * im->w + tx) * 4 + ch] / 255;
                    }
                }
                unsigned r = (unsigned)fminf(255, col[0] * lit), g = (unsigned)fminf(255, col[1] * lit), bl = (unsigned)fminf(255, col[2] * lit);
                v.depth[y * ViewW + x] = z;
                v.rgba[y * ViewW + x] = 0xFF000000u | bl << 16 | g << 8 | r;
            }
        }
    }
    SDL_UpdateTexture(v.tex, nullptr, v.rgba, ViewW * 4);
}

// --- the model ---

void reweigh(Tool &T) {
    if (!T.rigged) {
        kit_weigh(T.m);
    }
}

void own_init(Tool &T);
int own_clip_of(const Tool &T, int g, int m);

void open_model(Tool &T, const char *path) {
    char why[512];
    KitModel *m = new KitModel();

    if (!kit_read(path, m, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
        kit_free(m);
        delete m;
        return;
    }
    if (T.m != nullptr) {
        kit_free(T.m);
        delete T.m;
        delete[] T.posed;
    }
    T.m = m;
    T.posed = new float[m->nvert][3];
    T.made_asked = false;
    T.dirty = false;
    T.page = PAGE_ALL;
    T.model_view = 0;
    own_init(T);
    SDL_strlcpy(T.model, path, sizeof(T.model));
    T.rigged = m->skinned && kit_map_rig(m, why, sizeof(why));
    if (!T.rigged) {
        // markers saved beside it before, else a guess
        char markers[1100];

        snprintf(markers, sizeof(markers), "%s.markers.json", path);
        if (!kit_markers(m, markers, why, sizeof(why))) {
            kit_guess_markers(m);
        }
        kit_weigh(m);
    }
    fit(m, T.front);
    fit(m, T.side);
    fit(m, T.play);
    T.side.yaw = 90;
    // its id from its file's name
    const char *base = SDL_strrchr(path, '/') ? SDL_strrchr(path, '/') + 1 : SDL_strrchr(path, '\\') ? SDL_strrchr(path, '\\') + 1 : path;
    int n = 0;
    for (const char *c = base; *c != 0 && *c != '.' && n < 30; c++) {
        char ch = (char)SDL_tolower((unsigned char)*c);
        T.id[n++] = (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ? ch : '_';
    }
    T.id[n] = 0;
    for (int i = 0; i <= n; i++) {
        T.name[i] = (char)SDL_toupper((unsigned char)T.id[i]);
    }
    snprintf(T.mod_name, sizeof(T.mod_name), "%s", T.id);
    snprintf(T.status, sizeof(T.status), "%s: %d triangles, %d vertices. %s", base, m->ntri, m->nvert,
             T.rigged ? "Its own rig is used: its bones were found by their names."
                      : "It has no rig: place the dots on its joints under Model and joints.");
}

bool export_pack(Tool &T, bool test, const char *out = nullptr) {
    char why[512], markers[1100];
    KitModel *t = new KitModel();
    KitPack p = {};
    bool ok = false;

    if (T.m == nullptr) {
        delete t;
        return false;
    }
    if (out == nullptr) {
        out = T.out;
    }
    if (!kit_read(T.model, t, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
        delete t;
        return false;
    }
    if (T.rigged) {
        kit_map_rig(t, why, sizeof(why));
    } else {
        memcpy(t->bones, T.m->bones, sizeof(t->bones));
        t->arm_raise = T.m->arm_raise;
        kit_weigh(t);
        snprintf(markers, sizeof(markers), "%s.markers.json", T.model);
        kit_save_markers(T.m, markers);
    }
    t->arm_raise = T.m->arm_raise;
    p.id = T.id;
    p.name = T.name;
    p.mod_name = T.mod_name;
    p.author = T.author;
    p.prefer = Heroes[T.prefer];
    p.template_hero = Heroes[T.template_hero];
    for (int k = 0; k < PACK_KINDS; k++) {
        p.kind_template[k] = T.kind_template[k] >= 0 && T.kind_template[k] < 8 ? Heroes[T.kind_template[k]] : nullptr;
    }
    // each game's moves: one of the tool's written as GAME/MOVE; a model's own GAME/MOVE clip kept only where it's chosen
    static char owns[KIT_OWN_MOST][64], drops[KIT_DROP_MOST][64];
    int no = 0, nd = 0;
    for (int g = 0; g < KitGameCount && g < 8; g++) {
        for (int mv = 0; mv < 8 && KitGames[g].moves[mv] != nullptr; mv++) {
            int sel = T.ownmap[g][mv];
            if (sel >= 0 && no < KIT_OWN_MOST) {
                snprintf(owns[no], sizeof(owns[0]), "%s/%s=%s", KitGames[g].kind, KitGames[g].moves[mv], KitMoves[sel].name);
                p.own[no] = owns[no];
                no++;
            }
            // (whether or not the model as it's read now has one: a pack saved since it was opened has its own)
            if (sel != OWN_CLIP && nd < KIT_DROP_MOST) {
                snprintf(drops[nd], sizeof(drops[0]), "%s/%s", KitGames[g].kind, KitGames[g].moves[mv]);
                p.drop[nd] = drops[nd];
                nd++;
            }
        }
    }
    p.height = T.height;
    p.face_head = T.face_head;
    p.card = T.card[0] ? T.card : nullptr;
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        p.voice[j] = T.voice[j][0] ? T.voice[j] : nullptr;
    }
    char folder[1100];
    // a hero loaded from a mod goes back into it; a new one is a mod of its own, named by its id
    if (T.loaded_mod[0] != 0 && out == T.out) {
        SDL_strlcpy(folder, T.loaded_mod, sizeof(folder));
    } else {
        snprintf(folder, sizeof(folder), "%s/%s", out, T.id);
    }
    SDL_CreateDirectory(folder);
    if (!kit_atlas(t, why, sizeof(why)) || !kit_write(t, &p, folder, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
    } else {
        ok = true;
        snprintf(T.status, sizeof(T.status), "Saved to %s. It's in the game when that's the game's mods folder.", folder);
        if (test) {
            char mode[16];
            snprintf(mode, sizeof(mode), "%d", Modes[T.mode].mode);
            const char *args[] = { T.game, "--mods-dir", T.out, "--mod", T.id, "--extra-hero", "0", T.id, "--mode", mode, "--skip-intro",
                                   "--skip-loading", nullptr };
            SDL_Process *proc = SDL_CreateProcess(args, false);

            if (proc == nullptr) {
                snprintf(T.status, sizeof(T.status), "Exported, but the game didn't start (%s): is it at %s?", SDL_GetError(), T.game);
            } else {
                SDL_DestroyProcess(proc);
            }
        }
    }
    kit_free(t);
    delete t;
    return ok;
}

// --- what the game makes of it: exported to the tool's own folder, the game started to make it and write it out (--dump-hero), and
// each kind's model read back ---

void made_forget(Tool &T) {
    for (int k = 0; k < PACK_KINDS; k++) {
        if (T.have_made[k]) {
            kit_made_free(&T.made[k]);
        }
        T.have_made[k] = false;
        for (int r = 0; r < T.nrides[k]; r++) {
            kit_made_free(&T.rides[k][r]);
        }
        T.nrides[k] = 0;
    }
}

void make_in_game(Tool &T) {
    char mods[1100];
    const char *pref = SDL_GetPrefPath("CrashBashPC", "herotool");

    if (T.m == nullptr || T.maker != nullptr || pref == nullptr) {
        return;
    }
    T.made_asked = true;
    T.dirty = false;
    snprintf(mods, sizeof(mods), "%spreview", pref);
    SDL_CreateDirectory(mods);
    snprintf(T.made_dir, sizeof(T.made_dir), "%spreview-made", pref);
    SDL_CreateDirectory(T.made_dir);
    SDL_free((void *)pref);
    if (!export_pack(T, false, mods)) {
        return;
    }
    const char *args[] = { T.game, "--mods-dir", mods, "--mod", T.id, "--dump-hero", T.id, T.made_dir, nullptr };
    T.maker = SDL_CreateProcess(args, false);
    snprintf(T.status, sizeof(T.status), T.maker != nullptr ? "The game's making it in every game..." : "The game didn't start (%s): is it at %s?",
             SDL_GetError(), T.game);
}

void made_check(Tool &T) {
    int code = 0;

    if (T.maker == nullptr || !SDL_WaitProcess(T.maker, false, &code)) {
        return;
    }
    SDL_DestroyProcess(T.maker);
    T.maker = nullptr;
    made_forget(T);
    int have = 0;
    for (int k = 0; k < PACK_KINDS; k++) {
        char path[1300];

        snprintf(path, sizeof(path), "%s/%s_%s.hpv", T.made_dir, T.id, pack_kind_name(k));
        T.have_made[k] = kit_made_read(path, &T.made[k]);
        have += T.have_made[k];
        for (int r = 0; r < 2; r++) {
            snprintf(path, sizeof(path), "%s/%s_%s_ride%d.hpv", T.made_dir, T.id, pack_kind_name(k), r);
            if (kit_made_read(path, &T.rides[k][T.nrides[k]])) {
                T.nrides[k]++;
            }
        }
    }
    T.anim = 0;
    T.made_t = 0;
    snprintf(T.status, sizeof(T.status), have != 0 ? "The game made it in %d of its %d games: pick one on the left to choose its prop and moves."
                                                   : "The game didn't make it (it said %d): does it find its data when you start it yourself?",
             have != 0 ? have : code, PACK_KINDS);
}

// A made model's frame drawn: its faces textured as the PS1 does (the texel times its corner's colour, 128 as it is), clear texels
// left out, semi-transparent ones half over what's there. The game's axes are y down, so it's turned up the tool's way.
void raster_made(const KitMade *m, const int16_t *vs, float cx, float cz, View &v, int PW, int PH);

// With what the game draws beside it (rides, still: their one frame), fitted to them all
void draw_made(const KitMade *m, int anim, int frame, View &v, SDL_Renderer *ren, const KitMade *rides = nullptr, int nrides = 0) {
    const int PW = v.w > 0 ? v.w : PrevW, PH = v.h > 0 ? v.h : PrevH;

    if (v.rgba == nullptr) {
        v.rgba = new unsigned[PW * PH];
        v.depth = new float[PW * PH];
        v.tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, PW, PH);
    }
    for (int i = 0; i < PW * PH; i++) {
        v.rgba[i] = 0xFF3A3434;
        v.depth[i] = 1e30f;
    }
    if (m->nanims == 0) {
        SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
        return;
    }
    const KitMadeAnim *a = &m->anims[anim < m->nanims ? anim : 0];
    const int16_t *vs = a->verts + (size_t)(frame % a->frames) * m->nvert * 3;
    // fitted to every frame of the move shown, so it doesn't jump about, nor leave the picture, as it moves
    float lo[3] = { 1e9f, 1e9f, 1e9f }, hi[3] = { -1e9f, -1e9f, -1e9f };
    for (int r = -1; r < nrides; r++) {
        const KitMade *o = r < 0 ? m : &rides[r];
        const KitMadeAnim *fa = r < 0 ? a : &o->anims[0];
        for (int i = 0; o->nanims > 0 && i < o->nvert * (r < 0 ? fa->frames : 1); i++) {
            float q[3] = { (float)fa->verts[i * 3], -(float)fa->verts[i * 3 + 1], -(float)fa->verts[i * 3 + 2] };
            for (int c = 0; c < 3; c++) {
                lo[c] = fminf(lo[c], q[c]);
                hi[c] = fmaxf(hi[c], q[c]);
            }
        }
    }
    float h = hi[1] - lo[1], w = sqrtf((hi[0] - lo[0]) * (hi[0] - lo[0]) + (hi[2] - lo[2]) * (hi[2] - lo[2]));
    v.scale = fminf(PH * 0.8f / (h > 0 ? h : 1), PW * 0.8f / (w > 0 ? w : 1));
    float cx = (lo[0] + hi[0]) / 2, cz = (lo[2] + hi[2]) / 2;
    v.ox = PW / 2.0f;
    v.oy = PH * 0.9f + lo[1] * v.scale;
    raster_made(m, vs, cx, cz, v, PW, PH);
    for (int r = 0; r < nrides; r++) {
        if (rides[r].nanims > 0) {
            raster_made(&rides[r], rides[r].anims[0].verts, cx, cz, v, PW, PH);
        }
    }
    SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
}

// A made model's faces in frame vs, into the view's picture and depth
void raster_made(const KitMade *m, const int16_t *vs, float cx, float cz, View &v, int PW, int PH) {
    for (int t = 0; t < m->nfaces; t++) {
        const KitMadeFace *f = &m->faces[t];
        const KitMadeImage *im = f->image >= 0 && f->image < m->nimages ? &m->images[f->image] : nullptr;
        float p[3][3], sx[3], sy[3], sz[3];

        for (int j = 0; j < 3; j++) {
            const int16_t *q = vs + (size_t)f->v[j] * 3;
            p[j][0] = q[0] - cx, p[j][1] = -q[1], p[j][2] = -q[2] - cz;
            to_screen(v, p[j], sx[j], sy[j], sz[j]);
        }
        float e1[3], e2[3], n[3];
        for (int i = 0; i < 3; i++) {
            e1[i] = p[1][i] - p[0][i];
            e2[i] = p[2][i] - p[0][i];
        }
        n[0] = e1[1] * e2[2] - e1[2] * e2[1];
        n[1] = e1[2] * e2[0] - e1[0] * e2[2];
        n[2] = e1[0] * e2[1] - e1[1] * e2[0];
        float area = (sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]);
        if (fabsf(area) < 1e-6f) {
            continue;
        }
        float ln = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]), c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
        // its own light's baked in its colours: only a touch of the view's, so its shape reads
        float facing = ln > 0 ? fabsf(n[0] * s + n[2] * c) / ln : 1, lit = 0.8f + 0.25f * facing;
        int x0 = (int)fmaxf(0, floorf(fminf(sx[0], fminf(sx[1], sx[2])))), x1 = (int)fminf(PW - 1, ceilf(fmaxf(sx[0], fmaxf(sx[1], sx[2]))));
        int y0 = (int)fmaxf(0, floorf(fminf(sy[0], fminf(sy[1], sy[2])))), y1 = (int)fminf(PH - 1, ceilf(fmaxf(sy[0], fmaxf(sy[1], sy[2]))));
        for (int y = y0; y <= y1; y++) {
            for (int x = x0; x <= x1; x++) {
                float px = x + 0.5f, py = y + 0.5f;
                float a0 = ((sx[1] - px) * (sy[2] - py) - (sx[2] - px) * (sy[1] - py)) / area;
                float b0 = ((sx[2] - px) * (sy[0] - py) - (sx[0] - px) * (sy[2] - py)) / area, d0 = 1 - a0 - b0;

                if (a0 < 0 || b0 < 0 || d0 < 0) {
                    continue;
                }
                float z = a0 * sz[0] + b0 * sz[1] + d0 * sz[2];
                if (z >= v.depth[y * PW + x]) {
                    continue;
                }
                float col[3];
                for (int ch = 0; ch < 3; ch++) {
                    col[ch] = a0 * f->rgb[0][ch] + b0 * f->rgb[1][ch] + d0 * f->rgb[2][ch];
                }
                if (im != nullptr) {
                    int tx = (int)(a0 * f->uv[0][0] + b0 * f->uv[1][0] + d0 * f->uv[2][0]);
                    int ty = (int)(a0 * f->uv[0][1] + b0 * f->uv[1][1] + d0 * f->uv[2][1]);
                    tx = tx < 0 ? 0 : tx >= im->w ? im->w - 1 : tx;
                    ty = ty < 0 ? 0 : ty >= im->h ? im->h - 1 : ty;
                    const uint8_t *tex = im->rgba + ((size_t)ty * im->w + tx) * 4;
                    if (tex[3] == 0) {
                        continue;
                    }
                    for (int ch = 0; ch < 3; ch++) {
                        col[ch] = col[ch] * tex[ch] / 128;
                    }
                }
                unsigned r = (unsigned)fminf(255, col[0] * lit), g = (unsigned)fminf(255, col[1] * lit), bl = (unsigned)fminf(255, col[2] * lit);
                unsigned was = v.rgba[y * PW + x];
                if (f->semi) {
                    r = (r + (was & 255)) / 2, g = (g + (was >> 8 & 255)) / 2, bl = (bl + (was >> 16 & 255)) / 2;
                } else {
                    v.depth[y * PW + x] = z;
                }
                v.rgba[y * PW + x] = 0xFF000000u | bl << 16 | g << 8 | r;
            }
        }
    }
}

// --- the views, with their markers ---

void view_window(Tool &T, View &v, const char *title, bool markers, int which) {
    ImGui::Image((ImTextureID)(intptr_t)v.tex, ImVec2(ViewW, ViewH));
    ImVec2 at = ImGui::GetItemRectMin();
    if (!markers || T.rigged) {
        return;
    }
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 mouse = ImGui::GetIO().MousePos;
    for (int b = 0; b < KB_COUNT; b++) {
        KitBone &k = T.m->bones[b];
        if (!k.have) {
            continue;
        }
        float sx[2], sy[2], sz;
        to_screen(v, k.at, sx[0], sy[0], sz);
        to_screen(v, k.end, sx[1], sy[1], sz);
        ImU32 col = b >= KB_JAW ? IM_COL32(255, 160, 40, 255) : (b >= KB_UPPERARM_L && b <= KB_HAND_R) ? IM_COL32(80, 200, 255, 255)
                  : b >= KB_THIGH_L ? IM_COL32(120, 255, 120, 255) : IM_COL32(255, 230, 80, 255);
        dl->AddLine(ImVec2(at.x + sx[0], at.y + sy[0]), ImVec2(at.x + sx[1], at.y + sy[1]), col, 2);
        for (int e = 0; e < 2; e++) {
            ImVec2 c(at.x + sx[e], at.y + sy[e]);
            dl->AddCircleFilled(c, e ? 3.0f : 5.0f, col);
            if (T.drag_bone < 0 && ImGui::IsMouseClicked(0) && (mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y) < 49) {
                T.drag_bone = b, T.drag_end = e, T.drag_view = which;
            }
        }
        if (ImGui::IsItemHovered()) {
            for (int e = 0; e < 2; e++) {
                ImVec2 c(at.x + sx[e], at.y + sy[e]);
                if ((mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y) < 49) {
                    ImGui::SetTooltip("%s%s", KitBoneNames[b], e ? " (its end)" : "");
                }
            }
        }
    }
    // dragging: across and up in the front view, through and up in the side view
    if (T.drag_bone >= 0 && T.drag_view == which) {
        if (ImGui::IsMouseDown(0)) {
            float *p = T.drag_end ? T.m->bones[T.drag_bone].end : T.m->bones[T.drag_bone].at;
            float mx = (mouse.x - at.x - v.ox) / v.scale, my = (v.oy - (mouse.y - at.y)) / v.scale;

            p[1] = my;
            if (which == 0) {
                p[0] = mx;
            } else {
                p[2] = -mx;
            }
            // what joins it moves with it: a bone's end is the next one's start
            for (int c = 0; c < KB_COUNT; c++) {
                if (T.m->bones[c].have && KitBoneParent[c] == T.drag_bone && T.drag_end) {
                    float *q = T.m->bones[c].at;
                    if (fabsf(q[0] - p[0]) + fabsf(q[1] - p[1]) + fabsf(q[2] - p[2]) < 0.5f * (T.m->hi[1] - T.m->lo[1])) {
                        memcpy(q, p, sizeof(float) * 3);
                    }
                }
                if (T.m->bones[c].have && c == KitBoneParent[T.drag_bone] && !T.drag_end && T.drag_bone != KB_UPPERARM_L
                    && T.drag_bone != KB_UPPERARM_R && T.drag_bone != KB_THIGH_L && T.drag_bone != KB_THIGH_R && T.drag_bone < KB_JAW) {
                    memcpy(T.m->bones[c].end, p, sizeof(float) * 3);
                }
            }
            // and its other side's, mirrored
            const char *name = KitBoneNames[T.drag_bone];
            size_t len = strlen(name);
            if (T.mirror) {
                for (int o = 0; o < KB_COUNT; o++) {
                    const char *on = KitBoneNames[o];
                    if (o != T.drag_bone && strlen(on) == len && strncmp(on, name, len - 1) == 0
                        && ((name[len - 1] == 'l' && on[len - 1] == 'r') || (name[len - 1] == 'r' && on[len - 1] == 'l'))) {
                        float *q = T.drag_end ? T.m->bones[o].end : T.m->bones[o].at;
                        q[0] = -p[0], q[1] = p[1], q[2] = p[2];
                    }
                    // ears' and others' names end in a digit: ear_l1, ear_r1
                    if (o != T.drag_bone && strlen(on) == len && len > 2 && name[len - 2] != on[len - 2]
                        && strncmp(on, name, len - 2) == 0 && on[len - 1] == name[len - 1]
                        && ((name[len - 2] == 'l' && on[len - 2] == 'r') || (name[len - 2] == 'r' && on[len - 2] == 'l'))) {
                        float *q = T.drag_end ? T.m->bones[o].end : T.m->bones[o].at;
                        q[0] = -p[0], q[1] = p[1], q[2] = p[2];
                    }
                }
            }
        } else {
            T.drag_bone = -1;
            reweigh(T);
        }
    }
    (void)title;
}

void path_field(const char *label, char *buf, size_t size) {
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText(label, buf, size);
}

// --- the start screen: make a new hero from a model, or change one in the mods folder (the user's word, 2026-11-22) ---

// The heroes in the mods folder's mod folders (heroes/<id>/hero.json): a zipped mod's aren't listed, as it's changed by unzipping it
struct Found {
    char mod[1100], id[64], name[64];
};
const int MaxFound = 64;
Found Founds[MaxFound];
int NumFound = 0;

struct FindCtx {
    const char *mod;
};

SDL_EnumerationResult SDLCALL found_hero(void *ctx, const char *dir, const char *name) {
    const char *mod = (const char *)ctx;
    char path[1400];
    size_t n = 0;

    snprintf(path, sizeof(path), "%s%s/hero.json", dir, name);
    void *text = SDL_LoadFile(path, &n);
    if (text != nullptr && NumFound < MaxFound) {
        PackInfo info;
        char why[256];

        if (pack_info_read((const char *)text, n, &info, why, sizeof(why))) {
            Found &f = Founds[NumFound++];
            SDL_strlcpy(f.mod, mod, sizeof(f.mod));
            SDL_strlcpy(f.id, info.id[0] ? info.id : name, sizeof(f.id));
            SDL_strlcpy(f.name, info.name[0] ? info.name : name, sizeof(f.name));
        }
    }
    SDL_free(text);
    return SDL_ENUM_CONTINUE;
}

SDL_EnumerationResult SDLCALL found_mod(void *, const char *dir, const char *name) {
    char mod[1100], heroes[1200];
    SDL_PathInfo info;

    snprintf(mod, sizeof(mod), "%s%s", dir, name);
    snprintf(heroes, sizeof(heroes), "%s/heroes", mod);
    if (SDL_GetPathInfo(heroes, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
        SDL_EnumerateDirectory(heroes, found_hero, mod);
    }
    return SDL_ENUM_CONTINUE;
}

void find_heroes(const Tool &T) {
    NumFound = 0;
    SDL_EnumerateDirectory(T.out, found_mod, nullptr);
}

// A string of a JSON file's top object, by its key (mod.json's name and author), or empty
void json_string(const char *text, const char *key, char *out, size_t size) {
    char want[80];
    const char *at;

    out[0] = 0;
    snprintf(want, sizeof(want), "\"%s\"", key);
    if (text == nullptr || (at = SDL_strstr(text, want)) == nullptr || (at = SDL_strchr(at + strlen(want), '"')) == nullptr) {
        return;
    }
    size_t n = 0;
    for (at++; *at != 0 && *at != '"' && n + 1 < size; at++) {
        out[n++] = *at == '\\' && at[1] != 0 ? *++at : *at;
    }
    out[n] = 0;
}

// A hero pack loaded to change: its model (the pack's own, rigged on the standard skeleton, its moves its clips), and what hero.json,
// mod.json, card.png and the voices say. Export writes it back into the same mod.
void load_pack(Tool &T, const Found &f) {
    char path[1400], why[256];
    size_t n = 0;

    snprintf(path, sizeof(path), "%s/heroes/%s/hero.glb", f.mod, f.id);
    open_model(T, path);
    if (T.m == nullptr || SDL_strcmp(T.model, path) != 0) {
        return;
    }
    snprintf(path, sizeof(path), "%s/heroes/%s/hero.json", f.mod, f.id);
    void *text = SDL_LoadFile(path, &n);
    PackInfo info;
    if (text != nullptr && pack_info_read((const char *)text, n, &info, why, sizeof(why))) {
        SDL_strlcpy(T.id, info.id, sizeof(T.id));
        SDL_strlcpy(T.name, info.name, sizeof(T.name));
        T.height = info.height > 0 ? info.height : T.height;
        T.prefer = info.prefer >= 1 && info.prefer <= 8 ? info.prefer - 1 : 0;
        T.template_hero = info.template_hero >= 1 && info.template_hero <= 8 ? info.template_hero - 1 : 0;
        T.face_head = info.face_head;
        for (int k = 0; k < PACK_KINDS; k++) {
            int th = info.kinds[k].template_hero;
            T.kind_template[k] = th >= 1 && th <= 8 && th - 1 != T.template_hero ? th - 1 : -1;
        }
    }
    SDL_free(text);
    snprintf(path, sizeof(path), "%s/mod.json", f.mod);
    text = SDL_LoadFile(path, &n);
    json_string((const char *)text, "name", T.mod_name, sizeof(T.mod_name));
    json_string((const char *)text, "author", T.author, sizeof(T.author));
    SDL_free(text);
    // its card and sounds, as it has them; its own moves are already its clips (KIND/MOVE), so none are added
    SDL_PathInfo pi;
    snprintf(path, sizeof(path), "%s/heroes/%s/card.png", f.mod, f.id);
    SDL_strlcpy(T.card, SDL_GetPathInfo(path, &pi) ? path : "", sizeof(T.card));
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        snprintf(path, sizeof(path), "%s/heroes/%s/voice_%s.wav", f.mod, f.id, PackVoiceJobs[j]);
        SDL_strlcpy(T.voice[j], SDL_GetPathInfo(path, &pi) ? path : "", sizeof(T.voice[0]));
    }
    SDL_strlcpy(T.loaded_mod, f.mod, sizeof(T.loaded_mod));
    T.home = false;
    snprintf(T.status, sizeof(T.status), "%s, from %s: change what you like, then Save to mods folder puts it back there.", T.name, f.mod);
}

// The system's file chooser's pick, from whatever thread it answers on, for the main loop to open
SDL_AtomicInt Picked;
char PickedPath[1024];

// (what it's for: PICK_MODEL, PICK_CARD or PICK_VOICE and a job, plus 1, so 0 is nothing picked)
void SDLCALL picked(void *what, const char *const *files, int) {
    if (files != nullptr && files[0] != nullptr) {
        SDL_strlcpy(PickedPath, files[0], sizeof(PickedPath));
        SDL_SetAtomicInt(&Picked, (int)(intptr_t)what + 1);
    }
}

void start_screen(Tool &T, SDL_Window *win) {
    static const SDL_DialogFileFilter Glb[] = { { "3D model (glTF binary)", "glb" } };
    static bool looked = false;

    if (!looked) {
        find_heroes(T);
        looked = true;
    }
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Start", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float w = 620, x = (ImGui::GetIO().DisplaySize.x - w) / 2;
    ImGui::SetCursorPos(ImVec2(x, 60));
    ImGui::BeginChild("panel", ImVec2(w, 0), ImGuiChildFlags_AutoResizeY);
    ImGui::SetWindowFontScale(1.6f);
    ImGui::Text("Hero tool");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::TextDisabled("Make a hero of your own for Crash Bash PC, as a mod, from a 3D model.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetWindowFontScale(1.25f);
    ImGui::Text("Create a new hero");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::TextDisabled("Pick its 3D model, a .glb file (or drop one on this window).");
    if (ImGui::Button("Browse for a model...", ImVec2(200, 0))) {
        SDL_ShowOpenFileDialog(picked, (void *)(intptr_t)PICK_MODEL, win, Glb, 1, nullptr, false);
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(w - 300);
    ImGui::InputTextWithHint("##newmodel", "or type its path", T.model, sizeof(T.model));
    ImGui::SameLine();
    if (ImGui::Button("Create", ImVec2(-1, 0)) && T.model[0] != 0) {
        T.loaded_mod[0] = 0;
        open_model(T, T.model);
        T.home = T.m == nullptr;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetWindowFontScale(1.25f);
    ImGui::Text("Edit a hero");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::TextDisabled("The heroes in the mods folder: pick one to change it.");
    ImGui::BeginChild("found", ImVec2(0, 220), ImGuiChildFlags_Borders);
    if (NumFound == 0) {
        ImGui::TextDisabled("No heroes here yet (a zipped mod's are changed by unzipping it).");
    }
    for (int i = 0; i < NumFound; i++) {
        char label[256];
        const char *mod = Founds[i].mod;
        for (const char *c = Founds[i].mod; *c != 0; c++) {
            mod = *c == '/' || *c == '\\' ? c + 1 : mod;
        }
        snprintf(label, sizeof(label), "%s   (%s, in the mod %s)##%d", Founds[i].name, Founds[i].id, mod, i);
        if (ImGui::Selectable(label, false, 0, ImVec2(0, 26))) {
            load_pack(T, Founds[i]);
        }
    }
    ImGui::EndChild();
    ImGui::Text("Mods folder");
    ImGui::SetNextItemWidth(w - 110);
    ImGui::InputText("##mods", T.out, sizeof(T.out));
    ImGui::SameLine();
    if (ImGui::Button("Look again", ImVec2(-1, 0))) {
        find_heroes(T);
    }
    ImGui::Spacing();
    if (T.status[0] != 0 && T.m == nullptr) {
        ImGui::TextWrapped("%s", T.status);
    }
    ImGui::EndChild();
    ImGui::End();
}

// --- the editor (the user's word, 2026-11-22: one window, simple): the hero's pages down the left (its model and joints, every game
// at a glance, each game), the page's picture in the middle, and its choices on the right, with the hero's details and saving under
// them. A game's choices are whose ride or prop it has there and, move by move, what it plays: its template's, its own clip, or one
// of the tool's moves on it. The pictures are the game's own making of it, made again after a change. ---


// The game whose moves can be its own (KitGames), for a kind of model; -1 none (on foot, the bear, the podium follow its template)
int game_of(int kind) {
    for (int g = 0; g < KitGameCount; g++) {
        if (SDL_strcmp(KitGames[g].kind, pack_kind_name(kind)) == 0) {
            return g;
        }
    }
    return -1;
}

// The model's own clip for a game's move (GAME/MOVE), or -1
int own_clip_of(const Tool &T, int g, int m) {
    char name[64];

    snprintf(name, sizeof(name), "%s/%s", KitGames[g].kind, KitGames[g].moves[m]);
    for (int c = 0; T.m != nullptr && T.m->src != nullptr && c < T.m->src->nclips; c++) {
        if (SDL_strcasecmp(T.m->src->clips[c].name, name) == 0) {
            return c;
        }
    }
    return -1;
}

// Each game's moves as a model opens: its own clip where it has one (a pack loaded to change), else its template's
void own_init(Tool &T) {
    for (int g = 0; g < KitGameCount && g < 8; g++) {
        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
            T.ownmap[g][m] = own_clip_of(T, g, m) >= 0 ? OWN_CLIP : OWN_TEMPLATE;
        }
    }
}

void browse(SDL_Window *win, int target, const SDL_DialogFileFilter *filter) {
    SDL_ShowOpenFileDialog(picked, (void *)(intptr_t)target, win, filter, 1, nullptr, false);
}

const char *short_path(const char *path) {
    const char *s = path;
    for (const char *c = path; *c != 0; c++) {
        s = *c == '/' || *c == '\\' ? c + 1 : s;
    }
    return s;
}

void hint(const char *text) {
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    ImGui::SetItemTooltip("%s", text);
}

void editor_left(Tool &T) {
    ImGui::TextDisabled("YOUR HERO");
    if (ImGui::Selectable("Model and joints", T.page == PAGE_MODEL)) {
        T.page = PAGE_MODEL;
    }
    ImGui::Spacing();
    ImGui::TextDisabled("IN THE GAMES");
    if (ImGui::Selectable("All games at a glance", T.page == PAGE_ALL)) {
        T.page = PAGE_ALL;
    }
    for (int k = 0; k < PACK_KINDS; k++) {
        char label[96];
        snprintf(label, sizeof(label), "%s%s##page%d", KindShort[k], !T.have_made[k] && T.made_asked && T.maker == nullptr ? "  (not made)" : "", k);
        if (ImGui::Selectable(label, T.page == k)) {
            T.page = k;
            T.kind = k;
            T.anim = 0;
            T.made_t = 0;
        }
    }
    ImGui::Spacing();
    ImGui::Separator();
    if (T.maker != nullptr) {
        ImGui::TextWrapped("Making it in the games...");
    } else if (T.dirty && T.made_asked) {
        if (ImGui::Button("Update the preview", ImVec2(-1, 0))) {
            make_in_game(T);
        }
        ImGui::TextDisabled("after your changes");
    }
}

void editor_middle(Tool &T) {
    float avail = ImGui::GetContentRegionAvail().x;

    if (T.page == PAGE_MODEL) {
        if (!T.rigged) {
            if (ImGui::RadioButton("Place its joints", T.model_view == 0)) {
                T.model_view = 0;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Watch its moves", T.model_view == 1)) {
                T.model_view = 1;
            }
        }
        if (!T.rigged && T.model_view == 0) {
            ImGui::Text("Front");
            ImGui::SameLine(ViewW + 16);
            ImGui::Text("Side");
            view_window(T, T.front, "front", true, 0);
            ImGui::SameLine();
            view_window(T, T.side, "side", true, 1);
            ImGui::TextWrapped("Drag each dot onto its joint in both views: yellow its body, blue its arms, green its legs, orange its jaw, "
                               "ears and tail. A small dot is a bone's end.");
        } else {
            view_window(T, T.play, "move", false, 2);
            ImGui::SetNextItemWidth(180);
            ImGui::Combo("Move", &T.move, [](void *, int i) { return KitMoves[i].name; }, nullptr, KitMoveCount);
            ImGui::SameLine();
            ImGui::Checkbox("Play", &T.playing);
            ImGui::SetNextItemWidth(180);
            ImGui::SliderFloat("Turn", &T.turn, -180, 180, "%.0f");
            ImGui::TextDisabled("The tool's own moves on your model, which a game's moves can use (on the right of a game's page).");
        }
        return;
    }
    bool any = false;
    for (int k = 0; k < PACK_KINDS; k++) {
        any |= T.have_made[k];
    }
    if (!any) {
        ImGui::TextWrapped(T.maker != nullptr ? "The game's making your hero in every game: a moment..."
                                              : "Not made yet. The game makes it as it will when it loads it.");
        if (T.maker == nullptr && ImGui::Button("Make it")) {
            make_in_game(T);
        }
        return;
    }
    if (T.page == PAGE_ALL) {
        int cols = (int)(avail / (ThumbW + 16)) < 1 ? 1 : (int)(avail / (ThumbW + 16));
        for (int k = 0; k < PACK_KINDS; k++) {
            ImGui::PushID(k);
            ImGui::BeginGroup();
            if (T.have_made[k] && T.thumbs[k].tex != nullptr) {
                if (ImGui::ImageButton("thumb", (ImTextureID)(intptr_t)T.thumbs[k].tex, ImVec2(ThumbW, ThumbH))) {
                    T.page = k;
                    T.kind = k;
                    T.anim = 0;
                    T.made_t = 0;
                }
                ImGui::SetItemTooltip("Open %s: its prop and moves", KindShort[k]);
            } else {
                ImGui::Dummy(ImVec2(ThumbW + 8, ThumbH + 8));
            }
            ImGui::TextUnformatted(KindShort[k]);
            ImGui::EndGroup();
            ImGui::PopID();
            if ((k + 1) % cols != 0 && k + 1 < PACK_KINDS) {
                ImGui::SameLine();
            }
        }
        ImGui::SetNextItemWidth(200);
        ImGui::SliderFloat("Turn##all", &T.made_turn, -180, 180, "%.0f");
        return;
    }
    if (!T.have_made[T.kind]) {
        ImGui::TextWrapped(T.maker != nullptr ? "Making it..." : "The game didn't make this one: see the message at the top.");
        return;
    }
    float w = avail < PrevW ? avail : PrevW;
    ImGui::Image((ImTextureID)(intptr_t)T.game_view.tex, ImVec2(w, w * PrevH / PrevW));
    const KitMade *mk = &T.made[T.kind];
    ImGui::SetNextItemWidth(200);
    if (ImGui::BeginCombo("Watching", T.anim < mk->nanims ? mk->anims[T.anim].name : "")) {
        for (int a = 0; a < mk->nanims; a++) {
            if (ImGui::Selectable(mk->anims[a].name, a == T.anim)) {
                T.anim = a;
                T.made_t = 0;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(160);
    ImGui::SliderFloat("Turn##game", &T.made_turn, -180, 180, "%.0f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    ImGui::SliderFloat("Speed", &T.made_speed, 0, 2, "%.2f");
}

void editor_right(Tool &T, SDL_Window *win) {
    static const SDL_DialogFileFilter Glb[] = { { "3D model (glTF binary)", "glb" } };
    static const SDL_DialogFileFilter Png[] = { { "Picture (PNG)", "png" } };
    static const SDL_DialogFileFilter Wav[] = { { "Sound (WAV)", "wav" } };

    // the fields leave room for their labels
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 140);

    // the page's own choices
    if (T.page == PAGE_MODEL) {
        ImGui::SeparatorText("Your model");
        ImGui::TextWrapped("%s", short_path(T.model));
        if (ImGui::Button("Open another model...")) {
            browse(win, PICK_MODEL, Glb);
        }
        ImGui::Spacing();
        if (T.rigged) {
            ImGui::TextWrapped("It has a rig of its own: its bones were found by their names and are used as they are.");
        } else {
            ImGui::TextWrapped("It has no rig, so it gets one from the dots you place on its joints.");
            ImGui::Checkbox("Left and right the same", &T.mirror);
            hint("Moving one side's dot moves the other side's to match.");
            if (ImGui::Button("Guess the joints again")) {
                kit_guess_markers(T.m);
                reweigh(T);
            }
            ImGui::Text("It also has:");
            const char *opt[] = { "A jaw", "Ears", "A tail" };
            const int first[] = { KB_JAW, KB_EAR_L1, KB_TAIL1 }, count[] = { 1, 4, 3 };
            for (int o = 0; o < 3; o++) {
                bool has = T.m->bones[first[o]].have;
                if (o > 0) {
                    ImGui::SameLine();
                }
                if (ImGui::Checkbox(opt[o], &has)) {
                    float h = T.m->hi[1] - T.m->lo[1];
                    for (int c = 0; c < count[o]; c++) {
                        KitBone &k = T.m->bones[first[o] + c];
                        k.have = has;
                        if (has) {
                            const KitBone &head = T.m->bones[KB_HEAD], &hips = T.m->bones[KB_HIPS];
                            float side = (c == 2 || c == 3) && o == 1 ? -1.0f : 1.0f;
                            if (o == 0) {
                                k.at[0] = head.at[0], k.at[1] = head.at[1] + 0.02f * h, k.at[2] = head.at[2] + 0.04f * h;
                                k.end[0] = k.at[0], k.end[1] = k.at[1] - 0.02f * h, k.end[2] = k.at[2] + 0.08f * h;
                            } else if (o == 1) {
                                float lower = (c == 1 || c == 3) ? 1.0f : 0.0f;
                                k.at[0] = side * (0.07f + 0.02f * lower) * h, k.at[1] = head.end[1] - (0.04f + 0.08f * lower) * h;
                                k.at[2] = head.at[2];
                                k.end[0] = side * (0.09f + 0.02f * lower) * h, k.end[1] = k.at[1] - 0.08f * h, k.end[2] = head.at[2];
                            } else {
                                k.at[0] = 0, k.at[1] = hips.at[1] + 0.02f * h * c, k.at[2] = hips.at[2] - (0.08f + 0.07f * c) * h;
                                k.end[0] = 0, k.end[1] = k.at[1] + 0.02f * h, k.end[2] = k.at[2] - 0.07f * h;
                            }
                        }
                    }
                    reweigh(T);
                    T.dirty = true;
                }
            }
            ImGui::SetNextItemWidth(160);
            if (ImGui::SliderFloat("Arm lift", &T.m->arm_raise, 20, 160, "%.0f degrees")) {
                T.dirty = true;
            }
            hint("How high it can raise its arms: less for short arms or a big head.");
        }
    } else if (T.page == PAGE_ALL) {
        ImGui::SeparatorText("All games");
        ImGui::TextWrapped("Your hero as the game makes it in each game, with that game's ride or prop. Pick one to choose its prop and "
                           "moves.");
    } else {
        int k = T.kind, g = game_of(k);
        ImGui::SeparatorText(KindTitles[k]);
        if (k != KIND_WARP_TOOL) {
            char now[64];
            snprintf(now, sizeof(now), "%s's (its template)", HeroNames[T.template_hero]);
            ImGui::Text(k == 0 ? "Stands like" : k == 1 ? "Rides the bear of" : "Prop from");
            ImGui::SameLine(130);
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##from", T.kind_template[k] < 0 ? now : HeroNames[T.kind_template[k]])) {
                if (ImGui::Selectable(now, T.kind_template[k] < 0)) {
                    T.kind_template[k] = -1;
                    make_in_game(T);
                }
                for (int h = 0; h < 8; h++) {
                    if (h != T.template_hero && ImGui::Selectable(HeroNames[h], T.kind_template[k] == h)) {
                        T.kind_template[k] = h;
                        make_in_game(T);
                    }
                }
                ImGui::EndCombo();
            }
        } else {
            ImGui::TextWrapped("The podium is made from its moves on foot.");
        }
        ImGui::Spacing();
        if (g >= 0) {
            ImGui::Text("Its moves here");
            hint("What it plays for each of this game's moves: the hero it follows, its model's own clip if it came with one, or "
                 "one of the tool's moves on your model (see Model and joints).");
            int from = T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero;
            for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                char now[80];
                int sel = T.ownmap[g][m];
                bool clip = own_clip_of(T, g, m) >= 0;
                if (sel == OWN_TEMPLATE) {
                    snprintf(now, sizeof(now), "%s's", HeroNames[from]);
                } else if (sel == OWN_CLIP) {
                    snprintf(now, sizeof(now), "its own clip");
                } else {
                    snprintf(now, sizeof(now), "my %s", KitMoves[sel].name);
                }
                ImGui::PushID(m);
                ImGui::TextUnformatted(KitGames[g].moves[m]);
                ImGui::SameLine(130);
                ImGui::SetNextItemWidth(-1);
                if (ImGui::BeginCombo("##move", now)) {
                    char t[64];
                    snprintf(t, sizeof(t), "%s's", HeroNames[from]);
                    if (ImGui::Selectable(t, sel == OWN_TEMPLATE)) {
                        T.ownmap[g][m] = OWN_TEMPLATE;
                        make_in_game(T);
                    }
                    if (clip && ImGui::Selectable("its own clip", sel == OWN_CLIP)) {
                        T.ownmap[g][m] = OWN_CLIP;
                        make_in_game(T);
                    }
                    ImGui::Separator();
                    for (int s = 0; s < KitMoveCount; s++) {
                        snprintf(t, sizeof(t), "my %s", KitMoves[s].name);
                        if (ImGui::Selectable(t, sel == s)) {
                            T.ownmap[g][m] = s;
                            make_in_game(T);
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopID();
            }
        } else {
            ImGui::TextWrapped("Its moves here are %s's.", HeroNames[T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero]);
        }
    }
    ImGui::Spacing();

    // the hero, whatever page
    if (ImGui::CollapsingHeader("Hero details", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Name", T.name, sizeof(T.name), ImGuiInputTextFlags_CharsUppercase);
        hint("As the game shows it, in capitals.");
        ImGui::InputText("Id", T.id, sizeof(T.id), ImGuiInputTextFlags_CharsNoBlank);
        hint("Its short name in files and online: small letters and digits.");
        if (ImGui::InputInt("Height", &T.height, 10, 50)) {
            T.height = T.height < 200 ? 200 : T.height > 900 ? 900 : T.height;
            T.dirty = true;
        }
        hint("Standing, in the game's units: Crash is 452, most heroes about 650.");
        if (ImGui::Combo("Plays like", &T.template_hero, HeroNames, 8)) {
            T.dirty = true;
        }
        hint("Its template: the hero whose moves it makes, and whose ride or prop it has, in every game (each game can pick another).");
        ImGui::Combo("Takes place of", &T.prefer, HeroNames, 8);
        hint("Whose place in the game it takes when nobody's playing them.");
        if (ImGui::SliderFloat("Face", &T.face_head, 0, 0.6f, T.face_head > 0 ? "top %.2f" : "the game's choice")) {
            T.dirty = true;
        }
        hint("How much of the model, from the top, is its head: its face on the panel is drawn from that, if it has no card.");
    }
    if (ImGui::CollapsingHeader("Card and sounds")) {
        ImGui::Text("Card");
        hint("A picture for the character select (PNG). Without one, its face is drawn from the model.");
        ImGui::SameLine(110);
        ImGui::TextUnformatted(T.card[0] ? short_path(T.card) : "drawn from the model");
        ImGui::SameLine();
        if (ImGui::SmallButton("Browse##card")) {
            browse(win, PICK_CARD, Png);
        }
        if (T.card[0]) {
            ImGui::SameLine();
            if (ImGui::SmallButton("Clear##card")) {
                T.card[0] = 0;
            }
        }
        ImGui::Spacing();
        ImGui::Text("Sounds");
        hint("A WAV for each job. A job without one uses the sound of the hero whose place it takes.");
        for (int j = 0; j < PACK_VOICE_JOBS; j++) {
            ImGui::PushID(j);
            ImGui::TextUnformatted(PackVoiceJobs[j]);
            ImGui::SameLine(110);
            ImGui::TextUnformatted(T.voice[j][0] ? short_path(T.voice[j]) : "-");
            ImGui::SameLine();
            if (ImGui::SmallButton("Browse")) {
                browse(win, PICK_VOICE + j, Wav);
            }
            if (T.voice[j][0]) {
                ImGui::SameLine();
                if (ImGui::SmallButton("Clear")) {
                    T.voice[j][0] = 0;
                }
            }
            ImGui::PopID();
        }
    }
    if (ImGui::CollapsingHeader("Save", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Mod's name", T.mod_name, sizeof(T.mod_name));
        ImGui::InputText("Author", T.author, sizeof(T.author));
        if (ImGui::Button("Save to mods folder", ImVec2(-1, 30))) {
            export_pack(T, false);
        }
        ImGui::SetItemTooltip("%s", T.loaded_mod[0] ? T.loaded_mod : T.out);
        ImGui::SetNextItemWidth(160);
        ImGui::Combo("##mode", &T.mode, [](void *, int i) { return Modes[i].name; }, nullptr, (int)(sizeof(Modes) / sizeof(Modes[0])));
        ImGui::SameLine();
        if (ImGui::Button("Save and play", ImVec2(-1, 0))) {
            export_pack(T, true);
        }
        ImGui::SetItemTooltip("Saves it, then starts the game with you as it in the game picked.");
        if (ImGui::TreeNode("Folders")) {
            ImGui::Text("Mods folder");
            path_field("##out", T.out, sizeof(T.out));
            ImGui::Text("The game");
            path_field("##game", T.game, sizeof(T.game));
            ImGui::TreePop();
        }
    }
    ImGui::PopItemWidth();
}

void editor_window(Tool &T, SDL_Window *win) {
    // made by itself the first time a game's page is open for a model
    if (T.page != PAGE_MODEL && !T.made_asked && T.maker == nullptr) {
        make_in_game(T);
    }
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Hero tool", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    if (ImGui::Button("< Start")) {
        T.home = true;
        find_heroes(T);
    }
    ImGui::SameLine();
    ImGui::SetWindowFontScale(1.3f);
    ImGui::TextUnformatted(T.name);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::SameLine();
    bool bad = SDL_strstr(T.status, "n't") != nullptr || SDL_strstr(T.status, "can't") != nullptr || SDL_strstr(T.status, "not ") != nullptr;
    ImGui::PushStyleColor(ImGuiCol_Text, bad ? ImVec4(1.0f, 0.55f, 0.5f, 1) : ImVec4(0.65f, 0.65f, 0.7f, 1));
    ImGui::TextWrapped("%s", T.status);
    ImGui::PopStyleColor();
    ImGui::Separator();
    if (ImGui::BeginTable("editor", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV)) {
        ImGui::TableSetupColumn("pages", ImGuiTableColumnFlags_WidthFixed, 190);
        ImGui::TableSetupColumn("view", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("choices", ImGuiTableColumnFlags_WidthFixed, 410);
        ImGui::TableNextColumn();
        editor_left(T);
        ImGui::TableNextColumn();
        ImGui::BeginChild("middle", ImVec2(0, ImGui::GetContentRegionAvail().y));
        editor_middle(T);
        ImGui::EndChild();
        ImGui::TableNextColumn();
        ImGui::BeginChild("right", ImVec2(0, ImGui::GetContentRegionAvail().y));
        editor_right(T, win);
        ImGui::EndChild();
        ImGui::EndTable();
    }
    ImGui::End();
}

} // namespace

extern "C" int kit_gui(const char *start, const char *mods, const char *game) {
    static Tool T;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "herotool: no window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Hero tool - Crash Bash PC", 1360, 900, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, nullptr);
    SDL_SetRenderVSync(ren, 1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(win, ren);
    ImGui_ImplSDLRenderer3_Init(ren);
    // where it writes and what it starts: beside it, if it's beside the game
    const char *base = SDL_GetBasePath();
    snprintf(T.out, sizeof(T.out), "%smods", base != nullptr ? base : "");
    snprintf(T.game, sizeof(T.game), "%scrashbash%s", base != nullptr ? base : "",
#ifdef _WIN32
             ".exe"
#else
             ""
#endif
    );
    if (mods != nullptr) {
        SDL_strlcpy(T.out, mods, sizeof(T.out));
    }
    for (int k = 0; k < PACK_KINDS; k++) {
        T.thumbs[k].w = ThumbW;
        T.thumbs[k].h = ThumbH;
    }
    if (game != nullptr) {
        SDL_strlcpy(T.game, game, sizeof(T.game));
    }
    if (start != nullptr) {
        open_model(T, start);
        T.home = T.m == nullptr;
    }
    /* a test's: HEROTOOL_LOAD=ID loads that hero from the mods folder, as the start screen's list does */
    if (SDL_getenv("HEROTOOL_LOAD") != nullptr) {
        find_heroes(T);
        for (int i = 0; i < NumFound; i++) {
            if (SDL_strcmp(Founds[i].id, SDL_getenv("HEROTOOL_LOAD")) == 0) {
                load_pack(T, Founds[i]);
            }
        }
    }
    /* a test's: HEROTOOL_PAGE=N shows that page (-2 the model's, -1 all the games, a kind) */
    if (SDL_getenv("HEROTOOL_PAGE") != nullptr && T.m != nullptr) {
        T.page = SDL_atoi(SDL_getenv("HEROTOOL_PAGE"));
        T.kind = T.page >= 0 ? T.page : T.kind;
    }
    /* a test's: HEROTOOL_OWN=GAME/MOVE=MOVE picks one of the tool's moves for a game's move, as its page's list does */
    if (SDL_getenv("HEROTOOL_OWN") != nullptr && T.m != nullptr) {
        char o[96], *eq;
        SDL_strlcpy(o, SDL_getenv("HEROTOOL_OWN"), sizeof(o));
        if ((eq = SDL_strchr(o, '=')) != nullptr) {
            *eq = 0;
            for (int g = 0; g < KitGameCount && g < 8; g++) {
                for (int mv = 0; mv < 8 && KitGames[g].moves[mv] != nullptr; mv++) {
                    char n[64];
                    snprintf(n, sizeof(n), "%s/%s", KitGames[g].kind, KitGames[g].moves[mv]);
                    for (int s = 0; s < KitMoveCount && SDL_strcasecmp(n, o) == 0; s++) {
                        T.ownmap[g][mv] = SDL_strcasecmp(KitMoves[s].name, eq + 1) == 0 ? s : T.ownmap[g][mv];
                    }
                }
            }
        }
    }
    /* a test's: HEROTOOL_EXPORT=FOLDER exports to it once the model's open */
    if (SDL_getenv("HEROTOOL_EXPORT") != nullptr && T.m != nullptr) {
        SDL_strlcpy(T.out, SDL_getenv("HEROTOOL_EXPORT"), sizeof(T.out));
        export_pack(T, false);
        printf("herotool: %s\n", T.status);
    }
    /* a test's: HEROTOOL_MAKE=KIND has the game make it in every game once it's open, and shows that kind (heropack.c's name) */
    if (SDL_getenv("HEROTOOL_TEMPLATE") != nullptr) {
        for (int h = 0; h < 8; h++) {
            T.template_hero = SDL_strcasecmp(SDL_getenv("HEROTOOL_TEMPLATE"), Heroes[h]) == 0 ? h : T.template_hero;
        }
    }
    if (SDL_getenv("HEROTOOL_MAKE") != nullptr && T.m != nullptr) {
        T.kind = pack_kind_named(SDL_getenv("HEROTOOL_MAKE")) >= 0 ? pack_kind_named(SDL_getenv("HEROTOOL_MAKE")) : 0;
        make_in_game(T);
        T.page = PAGE_ALL;
        if (SDL_getenv("HEROTOOL_PAGE") != nullptr) {
            T.page = SDL_atoi(SDL_getenv("HEROTOOL_PAGE"));
            T.kind = T.page >= 0 ? T.page : T.kind;
        }
    }
    Uint64 last = SDL_GetTicks();
    bool done = false;
    /* a test's picture of it: HEROTOOL_SHOT=FILE.png saves the window after a moment, and closes it */
    const char *shot = SDL_getenv("HEROTOOL_SHOT");
    int frames = 0;
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) {
                done = true;
            } else if (e.type == SDL_EVENT_DROP_FILE && e.drop.data != nullptr) {
                const char *f = e.drop.data;
                size_t n = strlen(f);
                if (n > 4 && SDL_strcasecmp(f + n - 4, ".glb") == 0) {
                    T.loaded_mod[0] = 0;
                    open_model(T, f);
                    T.home = T.home && T.m == nullptr;
                } else if (n > 4 && SDL_strcasecmp(f + n - 4, ".png") == 0) {
                    SDL_strlcpy(T.card, f, sizeof(T.card));
                } else if (n > 4 && SDL_strcasecmp(f + n - 4, ".wav") == 0) {
                    SDL_strlcpy(T.voice[T.voice_job], f, sizeof(T.voice[0]));
                } else if (n > 5 && SDL_strcasecmp(f + n - 5, ".json") == 0 && T.m != nullptr) {
                    char why[512];
                    if (kit_markers(T.m, f, why, sizeof(why))) {
                        T.rigged = false;
                        reweigh(T);
                    } else {
                        snprintf(T.status, sizeof(T.status), "%s", why);
                    }
                }
            }
        }
        // a model picked in the system's chooser, from the start screen
        if (int what = SDL_GetAtomicInt(&Picked)) {
            SDL_SetAtomicInt(&Picked, 0);
            what--;
            if (what == PICK_MODEL) {
                SDL_strlcpy(T.model, PickedPath, sizeof(T.model));
                T.loaded_mod[0] = 0;
                open_model(T, T.model);
                T.home = T.m == nullptr;
            } else if (what == PICK_CARD) {
                SDL_strlcpy(T.card, PickedPath, sizeof(T.card));
            } else if (what >= PICK_VOICE && what < PICK_VOICE + PACK_VOICE_JOBS) {
                SDL_strlcpy(T.voice[what - PICK_VOICE], PickedPath, sizeof(T.voice[0]));
            }
        }
        Uint64 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        // the game's models play in the gallery and the big view alike (each move looped at its own length)
        T.made_t += dt * T.made_speed;
        if (T.m != nullptr && T.playing) {
            T.t += dt;
            if (T.t > KitMoves[T.move].length) {
                T.t = 0;
            }
        }
        last = now;
        made_check(T);
        if (T.page == PAGE_ALL) {
            for (int k = 0; k < PACK_KINDS; k++) {
                if (T.have_made[k] && T.made[k].nanims > 0) {
                    T.thumbs[k].yaw = T.made_turn;
                    draw_made(&T.made[k], 0, (int)(T.made_t * 30) % T.made[k].anims[0].frames, T.thumbs[k], ren, T.rides[k], T.nrides[k]);
                }
            }
        }
        if (T.page >= 0 && T.have_made[T.kind]) {
            const KitMade *mk = &T.made[T.kind];
            int a = T.anim < mk->nanims ? T.anim : 0;

            T.game_view.yaw = T.made_turn;
            // 30 frames a second, as the game plays a move a frame a tick; a kind made with more frames a tick has as many more
            draw_made(mk, a, mk->nanims > 0 ? (int)(T.made_t * 30) % mk->anims[a].frames : 0, T.game_view, ren, T.rides[T.kind],
                      T.nrides[T.kind]);
        }
        if (T.m != nullptr) {
            // the front and side views at rest, the third playing the move
            draw(T.m, T.m->pos, T.front, ren);
            draw(T.m, T.m->pos, T.side, ren);
            pose(T);
            T.play.yaw = T.turn;
            draw(T.m, T.posed, T.play, ren);
        }
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        if (T.home) {
            start_screen(T, win);
        } else {
            editor_window(T, win);
        }
        ImGui::Render();
        SDL_SetRenderDrawColor(ren, 30, 30, 34, 255);
        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ren);
        if (shot != nullptr && T.maker == nullptr && ++frames == 40) {
            SDL_Surface *grab = SDL_RenderReadPixels(ren, nullptr);

            if (grab != nullptr) {
                SDL_SavePNG(grab, shot);
                SDL_DestroySurface(grab);
            }
            done = true;
        }
        SDL_RenderPresent(ren);
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
