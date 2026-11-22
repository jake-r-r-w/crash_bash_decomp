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
const int ThumbW = 220, ThumbH = 205; // each game's in the gallery of them all

struct View {
    SDL_Texture *tex = nullptr;
    unsigned *rgba = nullptr;
    float *depth = nullptr;
    // model to screen: x across (from its axes, turned by yaw), y up, both scaled and moved to fit
    float yaw = 0, scale = 1, ox = 0, oy = 0;
    int w = 0, h = 0; // its size, if not the preview's (PrevW x PrevH): draw_made's only
};

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
    SDL_Process *maker = nullptr;
    char made_dir[1100] = "";
    int kind = 0, anim = 0;
    float made_t = 0, made_turn = 30, made_speed = 1;
    View game_view;
    bool show_games = false; // the tab brought forward once (the preview's test)
    // its own moves in the games' places (KIND/MOVE=OURS), and the one being added
    char own[KIT_OWN_MOST][48] = {};
    int nown = 0, own_game = 0, own_move = 1, own_ours = 0;
    // the gallery of every game's at once (or one game's, big), and whether the game's been asked to make it for this model yet
    View thumbs[PACK_KINDS];
    bool gallery = true, made_asked = false;
    // the start screen, till a model's opened or a hero loaded; a loaded hero's mod folder, which Export writes back to
    bool home = true;
    char loaded_mod[1100] = "";
};

// The kinds of model the game makes (heropack.c's names), as the games call them
const char *const KindTitles[PACK_KINDS] = { "On foot (Crate Crush, Ring Ding and the others)", "Polar Push: the polar bear", "Pogo",
                                             "Ballistix: the car", "Tank", "Crash Dash: the race cart", "Splash Dash: the fish",
                                             "Mallet Mash: the mallet", "Dragon Drop: the dino", "Keg Kaboom: the keg", "The podium" };

const char *const KindShort[PACK_KINDS] = { "On foot", "Polar Push: bear", "Pogo Painter: pogo", "Ballistix: car", "Tank",
                                            "Crash Dash: cart", "Splash Dash: fish", "Mallet Mash: mallet", "Dragon Drop: dino",
                                            "Keg Kaboom: keg", "The podium" };

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
                      : "No rig: drag the markers onto its joints in the front and side views.");
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
    for (int o = 0; o < T.nown; o++) {
        p.own[o] = T.own[o];
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
        snprintf(T.status, sizeof(T.status), "Exported to %s: put that folder in the game's mods folder (it's there if this is it).", folder);
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
    }
}

void make_in_game(Tool &T) {
    char mods[1100];
    const char *pref = SDL_GetPrefPath("CrashBashPC", "herotool");

    if (T.m == nullptr || T.maker != nullptr || pref == nullptr) {
        return;
    }
    T.made_asked = true;
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
    }
    T.anim = 0;
    T.made_t = 0;
    snprintf(T.status, sizeof(T.status), have != 0 ? "The game made it in %d of its %d kinds of game: they're all under In the games."
                                                   : "The game didn't make it (it said %d): does it find its data when you start it yourself?",
             have != 0 ? have : code, PACK_KINDS);
}

// A made model's frame drawn: its faces textured as the PS1 does (the texel times its corner's colour, 128 as it is), clear texels
// left out, semi-transparent ones half over what's there. The game's axes are y down, so it's turned up the tool's way.
void draw_made(const KitMade *m, int anim, int frame, View &v, SDL_Renderer *ren) {
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
    // fitted to its first move's first frame, so it doesn't jump about as it moves
    float lo[3] = { 1e9f, 1e9f, 1e9f }, hi[3] = { -1e9f, -1e9f, -1e9f };
    for (int i = 0; i < m->nvert; i++) {
        float q[3] = { (float)m->anims[0].verts[i * 3], -(float)m->anims[0].verts[i * 3 + 1], -(float)m->anims[0].verts[i * 3 + 2] };
        for (int c = 0; c < 3; c++) {
            lo[c] = fminf(lo[c], q[c]);
            hi[c] = fmaxf(hi[c], q[c]);
        }
    }
    float h = hi[1] - lo[1], w = sqrtf((hi[0] - lo[0]) * (hi[0] - lo[0]) + (hi[2] - lo[2]) * (hi[2] - lo[2]));
    v.scale = fminf(PH * 0.8f / (h > 0 ? h : 1), PW * 0.8f / (w > 0 ? w : 1));
    float cx = (lo[0] + hi[0]) / 2, cz = (lo[2] + hi[2]) / 2;
    v.ox = PW / 2.0f;
    v.oy = PH * 0.9f + lo[1] * v.scale;
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
    SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
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
    T.nown = 0;
    SDL_strlcpy(T.loaded_mod, f.mod, sizeof(T.loaded_mod));
    T.home = false;
    snprintf(T.status, sizeof(T.status), "%s loaded from %s: change it, then Export to save it back there.", T.name, f.mod);
}

// The system's file chooser's pick, from whatever thread it answers on, for the main loop to open
SDL_AtomicInt Picked;
char PickedPath[1024];

void SDLCALL picked(void *, const char *const *files, int) {
    if (files != nullptr && files[0] != nullptr) {
        SDL_strlcpy(PickedPath, files[0], sizeof(PickedPath));
        SDL_SetAtomicInt(&Picked, 1);
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
        SDL_ShowOpenFileDialog(picked, nullptr, win, Glb, 1, nullptr, false);
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
        T.show_games = true;
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
        if (SDL_GetAtomicInt(&Picked)) {
            SDL_SetAtomicInt(&Picked, 0);
            SDL_strlcpy(T.model, PickedPath, sizeof(T.model));
            T.loaded_mod[0] = 0;
            open_model(T, T.model);
            T.home = T.m == nullptr;
        }
        Uint64 now = SDL_GetTicks();
        if (T.m != nullptr && T.playing) {
            T.t += (now - last) / 1000.0f;
            if (T.t > KitMoves[T.move].length) {
                T.t = 0;
            }
        }
        last = now;
        made_check(T);
        if (T.gallery) {
            for (int k = 0; k < PACK_KINDS; k++) {
                if (T.have_made[k] && T.made[k].nanims > 0) {
                    T.thumbs[k].yaw = T.made_turn;
                    draw_made(&T.made[k], 0, (int)(T.made_t * 30) % T.made[k].anims[0].frames, T.thumbs[k], ren);
                }
            }
        }
        if (!T.gallery && T.have_made[T.kind]) {
            const KitMade *mk = &T.made[T.kind];
            int a = T.anim < mk->nanims ? T.anim : 0;

            T.made_t += (now - last) / 1000.0f * T.made_speed;
            T.game_view.yaw = T.made_turn;
            // 30 frames a second, as the game plays a move a frame a tick; a kind made with more frames a tick has as many more
            draw_made(mk, a, mk->nanims > 0 ? (int)(T.made_t * 30) % mk->anims[a].frames : 0, T.game_view, ren);
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
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Hero tool", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        if (ImGui::Button("< Start screen")) {
            T.home = true;
            find_heroes(T);
        }
        ImGui::SameLine();
        ImGui::TextWrapped("%s", T.status);
        ImGui::Separator();
        if (ImGui::BeginTable("main", 2, ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("views", ImGuiTableColumnFlags_WidthFixed, ViewW * 3 + 40);
            ImGui::TableNextColumn();
            if (T.m != nullptr && ImGui::BeginTabBar("tabs")) {
                if (ImGui::BeginTabItem("The model and its moves")) {
                ImGui::Text("Front");
                ImGui::SameLine(ViewW + 16);
                ImGui::Text("Side");
                ImGui::SameLine(ViewW * 2 + 24);
                ImGui::Text("%s", KitMoves[T.move].name);
                view_window(T, T.front, "front", true, 0);
                ImGui::SameLine();
                view_window(T, T.side, "side", true, 1);
                ImGui::SameLine();
                view_window(T, T.play, "move", false, 2);
                if (!T.rigged) {
                    ImGui::TextWrapped("Drag each marker onto its joint (yellow: its middle, blue: arms, green: legs, orange: jaw, ears and tail; "
                                       "the small dot is a bone's end). Its weights are worked out again as you let go.");
                    ImGui::Checkbox("Mirror left and right", &T.mirror);
                    ImGui::SameLine();
                    if (ImGui::Button("Guess them again")) {
                        kit_guess_markers(T.m);
                        reweigh(T);
                    }
                    ImGui::SameLine();
                    const char *opt[] = { "Jaw", "Ears", "Tail" };
                    const int first[] = { KB_JAW, KB_EAR_L1, KB_TAIL1 }, count[] = { 1, 4, 3 };
                    for (int o = 0; o < 3; o++) {
                        bool has = T.m->bones[first[o]].have;
                        ImGui::SameLine();
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
                        }
                    }
                    ImGui::SliderFloat("How high its arms can lift (degrees)", &T.m->arm_raise, 20, 160, "%.0f");
                }
                ImGui::SetNextItemWidth(200);
                ImGui::Combo("Move", &T.move, [](void *, int i) { return KitMoves[i].name; }, nullptr, KitMoveCount);
                ImGui::SameLine();
                ImGui::Checkbox("Play", &T.playing);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                ImGui::SliderFloat("Turn", &T.turn, -180, 180, "%.0f");
                ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("In the games", nullptr, T.show_games ? ImGuiTabItemFlags_SetSelected : 0)) {
                T.show_games = false;
                // made by itself the first time the tab's open for a model; again by the button after a change
                if (!T.made_asked && T.maker == nullptr) {
                    T.made_asked = true;
                    make_in_game(T);
                }
                ImGui::BeginDisabled(T.maker != nullptr);
                if (ImGui::Button(T.maker != nullptr ? "Making it..." : "Make it again")) {
                    make_in_game(T);
                }
                ImGui::EndDisabled();
                ImGui::SameLine();
                ImGui::TextDisabled("the game makes it as it will when it loads it, with each game's ride or prop (after a change)");
                // its own moves in place of its template's, in a game: then made again to see them
                ImGui::Text("Its own moves in the games, in place of %s's:", Heroes[T.template_hero]);
                for (int o = 0; o < T.nown; o++) {
                    ImGui::PushID(o);
                    ImGui::BulletText("%s", T.own[o]);
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Remove")) {
                        memmove(T.own[o], T.own[o + 1], sizeof(T.own[0]) * (size_t)(T.nown - o - 1));
                        T.nown--;
                    }
                    ImGui::PopID();
                }
                if (T.nown < KIT_OWN_MOST) {
                    const KitGameMoves &g = KitGames[T.own_game];
                    int gm = 0;
                    while (gm < 8 && g.moves[gm] != nullptr) {
                        gm++;
                    }
                    T.own_move = T.own_move < gm ? T.own_move : 0;
                    ImGui::SetNextItemWidth(110);
                    ImGui::Combo("##game", &T.own_game, [](void *, int i) { return KitGames[i].kind; }, nullptr, KitGameCount);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(110);
                    ImGui::Combo("##its", &T.own_move, [](void *d, int i) { return ((const KitGameMoves *)d)->moves[i]; }, (void *)&g, gm);
                    ImGui::SameLine();
                    ImGui::TextUnformatted("as");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(130);
                    ImGui::Combo("##ours", &T.own_ours, [](void *, int i) { return KitMoves[i].name; }, nullptr, KitMoveCount);
                    ImGui::SameLine();
                    if (ImGui::Button("Add")) {
                        snprintf(T.own[T.nown++], sizeof(T.own[0]), "%s/%s=%s", g.kind, g.moves[T.own_move], KitMoves[T.own_ours].name);
                    }
                }
                if (T.have_made[T.kind] || T.maker == nullptr) {
                    bool any = false;
                    for (int k = 0; k < PACK_KINDS; k++) {
                        any |= T.have_made[k];
                    }
                    if (any) {
                        ImGui::Separator();
                        if (ImGui::RadioButton("All the games", T.gallery)) {
                            T.gallery = true;
                        }
                        ImGui::SameLine();
                        if (ImGui::RadioButton("One game, its moves", !T.gallery)) {
                            T.gallery = false;
                        }
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(160);
                        ImGui::SliderFloat("Turn##all", &T.made_turn, -180, 180, "%.0f");
                    }
                    if (any && T.gallery) {
                        // each game's at once, its first move playing: one picked opens it big, with its moves
                        for (int k = 0; k < PACK_KINDS; k++) {
                            ImGui::PushID(k);
                            ImGui::BeginGroup();
                            if (T.have_made[k] && T.thumbs[k].tex != nullptr) {
                                if (ImGui::ImageButton("thumb", (ImTextureID)(intptr_t)T.thumbs[k].tex, ImVec2(ThumbW, ThumbH))) {
                                    T.kind = k;
                                    T.anim = 0;
                                    T.made_t = 0;
                                    T.gallery = false;
                                }
                            } else {
                                ImGui::Dummy(ImVec2(ThumbW + 8, ThumbH + 8));
                            }
                            ImGui::TextUnformatted(T.have_made[k] ? KindShort[k] : "(not made)");
                            ImGui::EndGroup();
                            ImGui::PopID();
                            if (k % 4 != 3 && k + 1 < PACK_KINDS) {
                                ImGui::SameLine();
                            }
                        }
                    } else if (any) {
                        ImGui::BeginGroup();
                        if (T.have_made[T.kind]) {
                            ImGui::Image((ImTextureID)(intptr_t)T.game_view.tex, ImVec2(PrevW, PrevH));
                        } else {
                            ImGui::Dummy(ImVec2(PrevW, PrevH));
                        }
                        ImGui::EndGroup();
                        ImGui::SameLine();
                        ImGui::BeginGroup();
                        for (int k = 0; k < PACK_KINDS; k++) {
                            ImGui::BeginDisabled(!T.have_made[k]);
                            if (ImGui::RadioButton(KindTitles[k], T.kind == k)) {
                                T.kind = k;
                                T.anim = 0;
                                T.made_t = 0;
                            }
                            ImGui::EndDisabled();
                        }
                        if (T.have_made[T.kind]) {
                            const KitMade *mk = &T.made[T.kind];
                            ImGui::SetNextItemWidth(200);
                            if (ImGui::BeginCombo("Its move", T.anim < mk->nanims ? mk->anims[T.anim].name : "")) {
                                for (int a = 0; a < mk->nanims; a++) {
                                    if (ImGui::Selectable(mk->anims[a].name, a == T.anim)) {
                                        T.anim = a;
                                        T.made_t = 0;
                                    }
                                }
                                ImGui::EndCombo();
                            }
                            ImGui::SetNextItemWidth(200);
                            ImGui::SliderFloat("Turn##game", &T.made_turn, -180, 180, "%.0f");
                            ImGui::SetNextItemWidth(200);
                            ImGui::SliderFloat("Speed", &T.made_speed, 0, 2, "%.2f");
                        }
                        ImGui::EndGroup();
                    }
                }
                ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::TableNextColumn();
            ImGui::Text("Model (.glb)");
            path_field("##model", T.model, sizeof(T.model));
            if (ImGui::Button("Open")) {
                char path[1024];
                SDL_strlcpy(path, T.model, sizeof(path));
                open_model(T, path);
            }
            ImGui::Separator();
            ImGui::Text("The hero");
            ImGui::InputText("Id", T.id, sizeof(T.id));
            ImGui::InputText("Name (capitals)", T.name, sizeof(T.name));
            ImGui::InputInt("Height", &T.height);
            T.height = T.height < 200 ? 200 : T.height > 900 ? 900 : T.height;
            ImGui::TextDisabled("Crash stands 452, most heroes about 650");
            ImGui::Combo("Takes the place of", &T.prefer, Heroes, 8);
            ImGui::Combo("Template", &T.template_hero, Heroes, 8);
            ImGui::TextDisabled("whose moves it makes, and what it rides and holds, in each game");
            ImGui::SliderFloat("Head for its face", &T.face_head, 0, 0.6f, T.face_head > 0 ? "%.2f of it" : "the game's own");
            ImGui::Separator();
            ImGui::Text("Card (.png)");
            path_field("##card", T.card, sizeof(T.card));
            ImGui::Text("Sounds (.wav)");
            for (int j = 0; j < PACK_VOICE_JOBS; j++) {
                ImGui::RadioButton(PackVoiceJobs[j], &T.voice_job, j);
                ImGui::SameLine(90);
                ImGui::PushID(j);
                path_field("##voice", T.voice[j], sizeof(T.voice[0]));
                ImGui::PopID();
            }
            ImGui::TextDisabled("Drop a .wav on the window for the job picked");
            ImGui::Separator();
            ImGui::InputText("Mod's name", T.mod_name, sizeof(T.mod_name));
            ImGui::InputText("Author", T.author, sizeof(T.author));
            ImGui::Text("Mods folder");
            path_field("##out", T.out, sizeof(T.out));
            if (ImGui::Button("Export") && T.m != nullptr) {
                export_pack(T, false);
            }
            ImGui::Separator();
            ImGui::Text("The game");
            path_field("##game", T.game, sizeof(T.game));
            ImGui::Combo("In", &T.mode, [](void *, int i) { return Modes[i].name; }, nullptr, (int)(sizeof(Modes) / sizeof(Modes[0])));
            if (ImGui::Button("Export and test in game") && T.m != nullptr) {
                export_pack(T, true);
            }
            ImGui::EndTable();
        }
        ImGui::End();
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
