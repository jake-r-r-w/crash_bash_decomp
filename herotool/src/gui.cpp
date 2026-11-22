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
#include <vector>
#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "kit.h"
#include "heroadjust.h"

namespace {

// The window's scale (ui_scale): its screen's, from its display scale or its height against a 1080-line screen's (the user's word,
// 2026-11-22: far too small on a high resolution screen), and Ctrl with + and - on top. Sizes in the window are written as they are at
// 1, through U.
float Ui = 1;
inline float U(float px) {
    return px * Ui;
}
int ViewW = 300, ViewH = 420;
int PrevW = 600, PrevH = 560; // the view of what the game made
int ThumbW = 220, ThumbH = 180; // each game's in the gallery of them all

struct View {
    SDL_Texture *tex = nullptr;
    unsigned *rgba = nullptr;
    float *depth = nullptr;
    // model to screen: x across (from its axes, turned by yaw), y up, both scaled and moved to fit
    float yaw = 0, scale = 1, ox = 0, oy = 0;
    int w = 0, h = 0; // its size, if not the preview's (PrevW x PrevH): draw_made's only
    // looked at from above by pitch degrees, and zoomed about (zx, zy): dragged and scrolled in the views that allow it (look_around)
    float pitch = 0, zoom = 1, zx = 0, zy = 0;
    // the game's x and z at the middle of what draw_made drew (its model's middle, which it's turned about)
    float cx = 0, cz = 0;
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
    int height = 560, voice_job = 1, mode = 0, template_hero = 0; // template_hero: who it's based on (its template and its place)
    // on foot its own moves (its model's clips, and the tool's moves for the rest), not those of who it's based on: a pack that says no
    // template (Bobby) is so, and is saved so, as it was (the user's word, 2026-11-22)
    bool foot_own = false;
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
    // the animation tools (Adjust its moves; docs/HERO_PACKS.md): each kind's poses before its adjustments (the game's .pose), its
    // adjustments (hero.json's "adjust"), what undo and redo go back and on to, the bone picked (the pose's, -1 none), what a change
    // is made to (KEY_FRAME this frame, fading over spread frames each side; KEY_MOVE the whole move; KEY_GAME every move in this game),
    // the frame shown, and a drag that's changing it
    KitPose poses[PACK_KINDS] = {};
    bool have_pose[PACK_KINDS] = {};
    std::vector<PackAdjust> adj;
    std::vector<std::vector<PackAdjust>> undo, redo;
    bool adjusting = false, adj_playing = false, dragging = false, bare = false;
    int sel = -1, key_mode = 0, spread = 4, frame = 0;
    float adj_t = 0;
    // the frame shown made again with its adjustments, and the picked bone's joint and each bone's on the screen
    std::vector<int16_t> live;
};
enum { KEY_FRAME, KEY_MOVE, KEY_GAME };

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
    float x = p[0] * c - p[2] * s, z = p[0] * s + p[2] * c, y = p[1];

    if (v.pitch != 0) {
        float pc = cosf(v.pitch * 3.14159265f / 180), ps = sinf(v.pitch * 3.14159265f / 180), y2 = y * pc - z * ps;
        z = y * ps + z * pc;
        y = y2;
    }
    sx = v.ox + x * v.scale;
    sy = v.oy - y * v.scale;
    if (v.zoom != 1) {
        sx = v.zx + (sx - v.zx) * v.zoom;
        sy = v.zy + (sy - v.zy) * v.zoom;
    }
    sz = -z;
}

// A view's picture dragged to turn and tilt it, and scrolled to zoom it (the item just drawn); yaw the turn it's drawn with. button: the
// mouse button that turns it (the right one while adjusting a move, whose left one picks and moves joints)
void look_around(View &v, float &yaw, int button = 0) {
    if (!ImGui::IsItemHovered() && !(ImGui::IsItemActive())) {
        return;
    }
    ImGuiIO &io = ImGui::GetIO();
    if (ImGui::IsMouseDragging(button)) {
        yaw += io.MouseDelta.x * 0.5f;
        yaw = yaw > 180 ? yaw - 360 : yaw < -180 ? yaw + 360 : yaw;
        v.pitch = fmaxf(-60, fminf(80, v.pitch + io.MouseDelta.y * 0.4f));
    }
    if (io.MouseWheel != 0) {
        v.zoom = fmaxf(0.4f, fminf(6, v.zoom * powf(1.15f, io.MouseWheel)));
    }
    if (ImGui::IsMouseDoubleClicked(button)) {
        v.pitch = 12, v.zoom = 1, yaw = 30;
    }
    if (button == 0) {
        ImGui::SetItemTooltip("Drag to turn it, scroll to zoom, double-click to set it back");
    }
}

// A made model's breathing move, its rest (the polar bear's first is its fall), or its first
int breathe_of(const KitMade *m) {
    for (int a = 0; a < m->nanims; a++) {
        if (SDL_strcasecmp(m->anims[a].name, "BREATHE") == 0) {
            return a;
        }
    }
    return 0;
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
    p.prefer = Heroes[T.template_hero]; // who it's based on: whose place it takes, and its template (one choice, the user's word, 2026-11-22)
    p.template_hero = Heroes[T.template_hero];
    for (int k = 0; k < PACK_KINDS; k++) {
        p.kind_template[k] = T.kind_template[k] >= 0 && T.kind_template[k] < 8 ? Heroes[T.kind_template[k]] : nullptr;
    }
    // its own moves on foot: based on Crash, it says no template (as a pack that never did, so one is saved as it was); else on foot "own"
    if (T.foot_own) {
        p.template_hero = T.template_hero == 0 ? nullptr : p.template_hero;
        p.kind_template[0] = T.template_hero == 0 ? nullptr : "own";
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
    // its adjustments, but not in what the preview's made from: the preview makes them itself, on the game's poses before them
    p.adjust = T.adj.empty() || T.bare ? nullptr : T.adj.data();
    p.nadjust = T.bare ? 0 : (int)T.adj.size();
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
        if (T.have_pose[k]) {
            kit_pose_free(&T.poses[k]);
        }
        T.have_pose[k] = false;
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
    /* a mods folder of its own for each hero, so the game makes only this one (one left by another hero isn't made again with it) */
    snprintf(mods, sizeof(mods), "%spreviews/%s", pref, T.id);
    SDL_CreateDirectory(mods);
    snprintf(T.made_dir, sizeof(T.made_dir), "%spreview-made", pref);
    SDL_CreateDirectory(T.made_dir);
    SDL_free((void *)pref);
    T.bare = true;
    bool exported = export_pack(T, false, mods);
    T.bare = false;
    if (!exported) {
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
        // what it's made of, for the animation tools: kept only where it fits the model (the same moves and frames)
        snprintf(path, sizeof(path), "%s/%s_%s.pose", T.made_dir, T.id, pack_kind_name(k));
        T.have_pose[k] = T.have_made[k] && kit_pose_read(path, &T.poses[k]);
        if (T.have_pose[k]) {
            bool fits = T.poses[k].nanims == T.made[k].nanims && T.poses[k].nvert + T.poses[k].nprop <= T.made[k].nvert;
            for (int a = 0; fits && a < T.made[k].nanims; a++) {
                fits = T.poses[k].anims[a].frames == T.made[k].anims[a].frames && strcmp(T.poses[k].anims[a].name, T.made[k].anims[a].name) == 0;
            }
            if (!fits) {
                kit_pose_free(&T.poses[k]);
                T.have_pose[k] = false;
            }
        }
        for (int r = 0; r < 2; r++) {
            snprintf(path, sizeof(path), "%s/%s_%s_ride%d.hpv", T.made_dir, T.id, pack_kind_name(k), r);
            if (kit_made_read(path, &T.rides[k][T.nrides[k]])) {
                T.nrides[k]++;
            }
        }
    }
    T.anim = T.have_made[T.kind] ? breathe_of(&T.made[T.kind]) : 0;
    T.made_t = 0;
    snprintf(T.status, sizeof(T.status), have != 0 ? "The game made it in %d of its %d games: pick one on the left to choose its prop and moves."
                                                   : "The game didn't make it (it said %d): does it find its data when you start it yourself?",
             have != 0 ? have : code, PACK_KINDS);
}

// A made model's frame drawn: its faces textured as the PS1 does (the texel times its corner's colour, 128 as it is), clear texels
// left out, semi-transparent ones half over what's there. The game's axes are y down, so it's turned up the tool's way.
void raster_made(const KitMade *m, const int16_t *vs, float cx, float cz, View &v, int PW, int PH);

// With what the game draws beside it (rides, still: their one frame), fitted to them all; its frame's vertices from live if given (made
// again with its adjustments, adjusted_frame)
void draw_made(const KitMade *m, int anim, int frame, View &v, SDL_Renderer *ren, const KitMade *rides = nullptr, int nrides = 0,
               const int16_t *live = nullptr) {
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
    const int16_t *vs = live != nullptr ? live : a->verts + (size_t)(frame % a->frames) * m->nvert * 3;
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
    v.cx = cx;
    v.cz = cz;
    v.ox = PW / 2.0f;
    v.oy = PH * 0.9f + lo[1] * v.scale;
    v.zx = PW / 2.0f;
    v.zy = PH / 2.0f;
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

// --- the animation tools (Adjust its moves): a kind's move made again with its adjustments as the game makes it (heroadjust.c) ---

// A frame's pose with its adjustments, and what it needs to turn a drag into the hero's own axes
struct Posed {
    float P[PACK_MAX_BONES][3], Q[PACK_MAX_BONES][9], trunk[9], A[9], b[3], hips[3];
    float shown[PACK_MAX_BONES][3]; // each bone's joint where the model shows it (the game's axes)
};

// A frame of a kind's move as the game makes it with the hero's adjustments now: its pose before them (the game's .pose) adjusted and
// its mesh skinned to it, what it carries with its hand, the rest as the game made it; false if there's no pose to make it from
bool adjusted_frame(Tool &T, int kind, int anim, int frame, std::vector<int16_t> &out, Posed *posed = nullptr) {
    static Posed mine;
    static std::vector<float> S;
    Posed *p = posed != nullptr ? posed : &mine;

    if (kind < 0 || kind >= PACK_KINDS || !T.have_made[kind] || !T.have_pose[kind] || anim < 0 || anim >= T.made[kind].nanims) {
        return false;
    }
    const KitMade *m = &T.made[kind];
    const KitPose *po = &T.poses[kind];
    const KitMadeAnim *ma = &m->anims[anim];
    frame = ma->frames > 0 ? (frame % ma->frames + ma->frames) % ma->frames : 0;
    const float *rec = po->anims[anim].pose + (size_t)frame * (KIT_POSE_HEAD + (size_t)po->nbones * 12);
    memcpy(p->hips, rec, sizeof(float) * 3);
    memcpy(p->trunk, rec + 3, sizeof(float) * 9);
    memcpy(p->A, rec + 12, sizeof(float) * 9);
    memcpy(p->b, rec + 21, sizeof(float) * 3);
    for (int b = 0; b < po->nbones; b++) {
        memcpy(p->P[b], rec + KIT_POSE_HEAD + b * 12, sizeof(float) * 3);
        memcpy(p->Q[b], rec + KIT_POSE_HEAD + b * 12 + 3, sizeof(float) * 9);
    }
    float hand_q[9] = {}, hand_p[3] = {};
    if (po->hand >= 0 && po->hand < po->nbones) {
        memcpy(hand_q, p->Q[po->hand], sizeof(hand_q));
        memcpy(hand_p, p->P[po->hand], sizeof(hand_p));
    }
    PackBones bones;
    bones.nbones = po->nbones;
    for (int b = 0; b < po->nbones; b++) {
        bones.name[b] = po->names[b];
        bones.parent[b] = po->parent[b];
    }
    pack_adjust_pose(T.adj.data(), (int)T.adj.size(), kind, ma->name, frame, &bones, p->trunk, p->P, p->Q);
    PackSkin sk = { po->nvert, po->rest, po->bone, po->joint, po->weight, po->part };
    S.resize((size_t)po->nvert * 3);
    pack_skin_posed(&sk, (const float(*)[3])p->P, (const float(*)[9])p->Q, p->hips, po->cut, S.data());
    out.assign(ma->verts + (size_t)frame * m->nvert * 3, ma->verts + (size_t)(frame + 1) * m->nvert * 3);
    auto put = [&](int v, const float s[3]) {
        for (int i = 0; i < 3; i++) {
            float o = p->A[i * 3] * s[0] + p->A[i * 3 + 1] * s[1] + p->A[i * 3 + 2] * s[2] + p->b[i];
            out[(size_t)v * 3 + i] = (int16_t)fmaxf(-32768, fminf(32767, floorf(o + 0.5f)));
        }
    };
    for (int v = 0; v < po->nvert; v++) {
        put(v, &S[(size_t)v * 3]);
    }
    // what it carries, turned and moved as its hand is by its adjustments (A is a turn, so A' undoes it)
    if (po->hand >= 0 && po->hand < po->nbones && po->nprop > 0) {
        float D[9];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                D[i * 3 + j] = p->Q[po->hand][i * 3] * hand_q[j * 3] + p->Q[po->hand][i * 3 + 1] * hand_q[j * 3 + 1]
                             + p->Q[po->hand][i * 3 + 2] * hand_q[j * 3 + 2];
            }
        }
        for (int v = po->nvert; v < po->nvert + po->nprop && v < m->nvert; v++) {
            float g[3], L[3], d[3], s[3];
            for (int i = 0; i < 3; i++) {
                g[i] = out[(size_t)v * 3 + i] - p->b[i];
            }
            for (int i = 0; i < 3; i++) {
                L[i] = p->A[i] * g[0] + p->A[3 + i] * g[1] + p->A[6 + i] * g[2];
                d[i] = L[i] - hand_p[i];
            }
            for (int i = 0; i < 3; i++) {
                s[i] = p->P[po->hand][i] + D[i * 3] * d[0] + D[i * 3 + 1] * d[1] + D[i * 3 + 2] * d[2];
            }
            put(v, s);
        }
    }
    for (int b = 0; b < po->nbones; b++) {
        for (int i = 0; i < 3; i++) {
            p->shown[b][i] = p->A[i * 3] * p->P[b][0] + p->A[i * 3 + 1] * p->P[b][1] + p->A[i * 3 + 2] * p->P[b][2] + p->b[i];
        }
    }
    return true;
}

// Whether a bone is moved (a hand, a foot, the hips) rather than turned
bool reach_bone(const char *name) {
    return SDL_strcasecmp(name, "hips") == 0 || SDL_strncasecmp(name, "hand", 4) == 0 || SDL_strncasecmp(name, "foot", 4) == 0;
}

// Where a bone's name says its side: the l or r of a "_l" or "_r" at its end, or before the digits there (ear_l1), else -1
int side_letter(const char *name) {
    int n = (int)strlen(name), i = n;

    while (i > 0 && name[i - 1] >= '0' && name[i - 1] <= '9') {
        i--;
    }
    return i >= 2 && name[i - 2] == '_' && (name[i - 1] == 'l' || name[i - 1] == 'L' || name[i - 1] == 'r' || name[i - 1] == 'R') ? i - 1 : -1;
}

// A bone's name as a person would say it: upperarm_l "Left upper arm", ear_l1 "Left ear 1"
const char *bone_title(const char *name) {
    static char out[80];
    static const struct {
        const char *bone, *says;
    } Says[] = { { "upperarm", "upper arm" }, { "forearm", "forearm" }, { "hand", "hand" }, { "thigh", "thigh" }, { "shin", "shin" },
                 { "foot", "foot" }, { "toe", "toes" }, { "ear", "ear" }, { "eye", "eye" } };
    char base[64];
    int at = side_letter(name);
    const char *side = at < 0 ? "" : (name[at] | 32) == 'l' ? "Left " : "Right ", *digits = at < 0 ? "" : name + at + 1;
    SDL_strlcpy(base, name, sizeof(base));
    if (at >= 1 && at - 1 < (int)sizeof(base)) {
        base[at - 1] = 0;
    }
    const char *says = nullptr;
    for (auto &s : Says) {
        says = SDL_strcasecmp(base, s.bone) == 0 || (SDL_strcasecmp(s.bone, "toe") == 0 && SDL_strncasecmp(base, "toe", 3) == 0) ? s.says : says;
    }
    if (says == nullptr) {
        for (char *c = base; *c != 0; c++) {
            *c = *c == '_' ? ' ' : *c;
        }
        says = base;
    }
    snprintf(out, sizeof(out), "%s%s%s%s", side, says, digits[0] != 0 ? " " : "", digits);
    if (side[0] == 0 && out[0] >= 'a' && out[0] <= 'z') {
        out[0] = (char)(out[0] - 'a' + 'A');
    }
    return out;
}

// Undo and redo: what its adjustments were before each change
void adj_snapshot(Tool &T) {
    T.undo.push_back(T.adj);
    if (T.undo.size() > 300) {
        T.undo.erase(T.undo.begin());
    }
    T.redo.clear();
}

void adj_undo(Tool &T) {
    if (!T.undo.empty()) {
        T.redo.push_back(T.adj);
        T.adj = T.undo.back();
        T.undo.pop_back();
    }
}

void adj_redo(Tool &T) {
    if (!T.redo.empty()) {
        T.undo.push_back(T.adj);
        T.adj = T.redo.back();
        T.redo.pop_back();
    }
}

// The adjustment a change of a bone in this move is made to, as T.key_mode says (this frame, the whole move, every move in this game):
// the one there, else a new one if make (none if there are as many as a pack has room for)
PackAdjust *adj_key(Tool &T, const char *move, const char *bone, bool make) {
    int at = T.key_mode == KEY_FRAME ? T.frame : -1;
    const char *mv = T.key_mode == KEY_GAME ? "*" : move;

    for (auto &a : T.adj) {
        if (a.kind == T.kind && a.at == at && SDL_strcasecmp(a.move, mv) == 0 && SDL_strcasecmp(a.bone, bone) == 0) {
            if (make && T.key_mode == KEY_FRAME) {
                a.spread = T.spread;
            }
            return &a;
        }
    }
    if (!make) {
        return nullptr;
    }
    if (T.adj.size() >= PACK_MAX_ADJUSTS) {
        snprintf(T.status, sizeof(T.status), "That's as many changes as a hero has room for (%d): take some away first.", PACK_MAX_ADJUSTS);
        return nullptr;
    }
    PackAdjust a = {};
    a.kind = T.kind;
    SDL_strlcpy(a.move, mv, sizeof(a.move));
    SDL_strlcpy(a.bone, bone, sizeof(a.bone));
    a.at = at;
    a.spread = T.key_mode == KEY_FRAME ? T.spread : 0;
    T.adj.push_back(a);
    return &T.adj.back();
}

// A bone's other side (upperarm_l's upperarm_r, ear_l1's ear_r1), or "" if it has none
const char *other_side(const char *name) {
    static char out[PACK_ADJUST_NAME];
    int at = side_letter(name);

    if (at < 0 || strlen(name) >= sizeof(out)) {
        return "";
    }
    SDL_strlcpy(out, name, sizeof(out));
    out[at] = name[at] == 'l' ? 'r' : name[at] == 'L' ? 'R' : name[at] == 'r' ? 'l' : 'L';
    return out;
}

// A bone's changes in this move (and those to every move here) given to its other side, mirrored: across the other way, and its turns
// about its up and its front-to-back the other way. Its other side's own there are replaced.
void adj_mirror(Tool &T, const char *move, const char *bone) {
    const char *other = other_side(bone);
    std::vector<PackAdjust> keep, made;

    if (other[0] == 0) {
        return;
    }
    for (auto &a : T.adj) {
        bool here = a.kind == T.kind && (SDL_strcasecmp(a.move, move) == 0 || strcmp(a.move, "*") == 0);
        if (here && SDL_strcasecmp(a.bone, other) == 0) {
            continue;
        }
        keep.push_back(a);
        if (here && SDL_strcasecmp(a.bone, bone) == 0) {
            PackAdjust m = a;
            SDL_strlcpy(m.bone, other, sizeof(m.bone));
            m.turn[1] = -m.turn[1], m.turn[2] = -m.turn[2];
            m.reach[0] = -m.reach[0];
            made.push_back(m);
        }
    }
    keep.insert(keep.end(), made.begin(), made.end());
    T.adj = keep;
}

// A direction in the view (x right, y up, z towards whoever's looking) in the game's axes (its turn and tilt undone: to_screen's)
void view_to_game(const View &v, const float d[3], float out[3]) {
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
    float pc = cosf(v.pitch * 3.14159265f / 180), ps = sinf(v.pitch * 3.14159265f / 180);
    float y = d[1] * pc + d[2] * ps, z = -d[1] * ps + d[2] * pc, x = d[0];

    out[0] = x * c + z * s;
    out[1] = -y;
    out[2] = -(-x * s + z * c);
}

// A direction in the game's axes, as a frame shows it, in the hero's own (its move and its trunk's turn undone: A' then trunk')
void game_to_hero(const Posed &p, const float g[3], float out[3]) {
    float L[3];

    for (int i = 0; i < 3; i++) {
        L[i] = p.A[i] * g[0] + p.A[3 + i] * g[1] + p.A[6 + i] * g[2];
    }
    for (int i = 0; i < 3; i++) {
        out[i] = p.trunk[i] * L[0] + p.trunk[3 + i] * L[1] + p.trunk[6 + i] * L[2];
    }
}

// The joints over the picture of a game's model while adjusting it (the picture at im, shown at disp of its size): each bone's joint a
// dot, picked by clicking, and the one picked changed by dragging: a hand, foot or the hips moved with the mouse (Shift: turned), any
// other bone turned round its joint as far as the mouse goes round it
void adjust_view(Tool &T, ImVec2 im, float disp, bool hovered) {
    static Posed ps;
    const KitMade *mk = &T.made[T.kind];
    const KitPose *po = &T.poses[T.kind];
    int a = T.anim < mk->nanims ? T.anim : 0;
    std::vector<int16_t> scratch;

    if (!adjusted_frame(T, T.kind, a, T.frame, scratch, &ps)) {
        return;
    }
    View &v = T.game_view;
    ImGuiIO &io = ImGui::GetIO();
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 mouse = io.MousePos;
    float sx[PACK_MAX_BONES], sy[PACK_MAX_BONES];
    for (int b = 0; b < po->nbones; b++) {
        float q[3] = { ps.shown[b][0] - v.cx, -ps.shown[b][1], -ps.shown[b][2] - v.cz }, x, y, z;
        to_screen(v, q, x, y, z);
        sx[b] = im.x + x * disp;
        sy[b] = im.y + y * disp;
    }
    if (T.sel >= po->nbones) {
        T.sel = -1;
    }
    int hover = -1;
    float best = U(8) * U(8);
    for (int b = 0; b < po->nbones; b++) {
        float d2 = (mouse.x - sx[b]) * (mouse.x - sx[b]) + (mouse.y - sy[b]) * (mouse.y - sy[b]);
        if (hovered && d2 < best) {
            best = d2, hover = b;
        }
        if (po->parent[b] >= 0) {
            dl->AddLine(ImVec2(sx[po->parent[b]], sy[po->parent[b]]), ImVec2(sx[b], sy[b]), IM_COL32(255, 255, 255, 90), U(1.5f));
        }
    }
    for (int b = 0; b < po->nbones; b++) {
        const char *n = po->names[b];
        ImU32 col = po->part[b] == 3 ? IM_COL32(80, 200, 255, 255) : po->part[b] == 1 ? IM_COL32(120, 255, 120, 255)
                  : po->part[b] == 2 || SDL_strncasecmp(n, "ear", 3) == 0 || SDL_strncasecmp(n, "jaw", 3) == 0 ? IM_COL32(255, 160, 40, 255)
                                                                                                                 : IM_COL32(255, 230, 80, 255);
        bool moved = reach_bone(n);
        if (b == T.sel) {
            dl->AddCircle(ImVec2(sx[b], sy[b]), U(9), IM_COL32(255, 255, 255, 255), 0, U(2.5f));
        }
        if (moved) {
            dl->AddRectFilled(ImVec2(sx[b] - U(4), sy[b] - U(4)), ImVec2(sx[b] + U(4), sy[b] + U(4)), col);
        } else {
            dl->AddCircleFilled(ImVec2(sx[b], sy[b]), b == hover ? U(6) : U(4.5f), col);
        }
    }
    if (hover >= 0 && !T.dragging) {
        ImGui::SetTooltip("%s: %s", bone_title(po->names[hover]),
                          reach_bone(po->names[hover]) ? "drag to move it (Shift: to turn it)" : "drag away from it to point what's on it at the mouse (Shift: turn it round)");
    }
    // pick, and start a change
    if (hovered && ImGui::IsMouseClicked(0)) {
        if (hover >= 0) {
            T.sel = hover;
        }
        if (T.sel >= 0) {
            adj_snapshot(T);
            T.dragging = true;
            T.adj_playing = false;
        }
    }
    if (T.dragging && !ImGui::IsMouseDown(0)) {
        T.dragging = false;
    }
    if (!T.dragging || T.sel < 0 || (io.MouseDelta.x == 0 && io.MouseDelta.y == 0) || !ImGui::IsMouseDragging(0, 2)) {
        return;
    }
    PackAdjust *k = adj_key(T, mk->anims[a].name, po->names[T.sel], true);
    if (k == nullptr) {
        return;
    }
    float pp = v.scale * v.zoom * disp;
    if (reach_bone(po->names[T.sel]) && !io.KeyShift) {
        // moved with the mouse across the view: at its own frame (or every frame) it's all there, so the joint goes with the mouse
        float d[3] = { io.MouseDelta.x / pp, -io.MouseDelta.y / pp, 0 }, g[3], h[3];
        view_to_game(v, d, g);
        game_to_hero(ps, g, h);
        for (int i = 0; i < 3; i++) {
            k->reach[i] = fmaxf(-1000, fminf(1000, k->reach[i] + h[i]));
        }
    } else {
        // turned about the line through its joint towards whoever's looking, to point at the mouse: its end (the middle of the
        // joints on it, or on past its joint from the one it's on) turned to where the mouse is from its joint. Near its joint it's
        // left be (which way it points is no way to tell), and with Shift, or where its end is too near its joint on the screen
        // (pointing at whoever's looking), it's turned as far as the mouse goes round its joint
        float jx = sx[T.sel], jy = sy[T.sel], ex = 0, ey = 0;
        int n = 0;
        for (int c = 0; c < po->nbones; c++) {
            if (po->parent[c] == T.sel) {
                ex += sx[c], ey += sy[c], n++;
            }
        }
        if (n > 0) {
            ex /= n, ey /= n;
        } else if (po->parent[T.sel] >= 0) {
            ex = jx + (jx - sx[po->parent[T.sel]]) * 0.5f, ey = jy + (jy - sy[po->parent[T.sel]]) * 0.5f;
        }
        float mx = mouse.x - jx, my = mouse.y - jy, px = mx - io.MouseDelta.x, py = my - io.MouseDelta.y;
        if (mx * mx + my * my < U(12) * U(12)) {
            return;
        }
        bool aim = !io.KeyShift && n + (po->parent[T.sel] >= 0) > 0 && (ex - jx) * (ex - jx) + (ey - jy) * (ey - jy) >= U(6) * U(6);
        if (!aim && px * px + py * py < U(12) * U(12)) {
            return;
        }
        float da = atan2f(-my, mx) - (aim ? atan2f(-(ey - jy), ex - jx) : atan2f(-py, px));
        da = da > 3.14159265f ? da - 6.2831853f : da < -3.14159265f ? da + 6.2831853f : da;
        float z[3] = { 0, 0, 1 }, g[3], h[3];
        view_to_game(v, z, g);
        game_to_hero(ps, g, h);
        for (int i = 0; i < 3; i++) {
            k->turn[i] = fmaxf(-360, fminf(360, k->turn[i] + h[i] * da * 180 / 3.14159265f));
        }
    }
}

// Under the picture while adjusting: play or stop, the frame (its changes marked), and the changes before and after
void timeline(Tool &T, const KitMade *mk, int a, float width) {
    int frames = mk->anims[a].frames;
    const char *move = mk->anims[a].name;

    T.frame = frames > 0 ? (T.frame % frames + frames) % frames : 0;
    if (ImGui::Button(T.adj_playing ? "Stop" : "Play", ImVec2(U(50), 0))) {
        T.adj_playing = !T.adj_playing;
    }
    ImGui::SetItemTooltip("Space");
    auto key_frames = [&](std::vector<int> &at) {
        for (auto &k : T.adj) {
            if (k.kind == T.kind && k.at >= 0 && SDL_strcasecmp(k.move, move) == 0) {
                at.push_back(k.at);
            }
        }
    };
    std::vector<int> keys;
    key_frames(keys);
    ImGui::SameLine();
    if (ImGui::ArrowButton("##before", ImGuiDir_Left)) {
        int to = -1;
        for (int f : keys) {
            to = f < T.frame && f > to ? f : to;
        }
        T.frame = to >= 0 ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SetItemTooltip("The change before this frame");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(width - U(200) > U(100) ? width - U(200) : U(100));
    if (ImGui::SliderInt("##frame", &T.frame, 0, frames - 1, "frame %d")) {
        T.adj_playing = false;
    }
    ImVec2 lo = ImGui::GetItemRectMin(), hi = ImGui::GetItemRectMax();
    float grab = ImGui::GetStyle().GrabMinSize, x0 = lo.x + grab / 2 + 2, x1 = hi.x - grab / 2 - 2;
    for (int f : keys) {
        float x = frames > 1 ? x0 + (x1 - x0) * f / (frames - 1) : x0;
        ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(x - U(4), lo.y - 1), ImVec2(x + U(4), lo.y - 1), ImVec2(x, lo.y + U(5)),
                                                      IM_COL32(255, 200, 60, 255));
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##after", ImGuiDir_Right)) {
        int to = frames;
        for (int f : keys) {
            to = f > T.frame && f < to ? f : to;
        }
        T.frame = to < frames ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SetItemTooltip("The change after this frame");
    ImGui::SameLine();
    ImGui::Text("%d frames", frames);
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
        dl->AddLine(ImVec2(at.x + sx[0], at.y + sy[0]), ImVec2(at.x + sx[1], at.y + sy[1]), col, U(2));
        for (int e = 0; e < 2; e++) {
            ImVec2 c(at.x + sx[e], at.y + sy[e]);
            dl->AddCircleFilled(c, e ? U(3) : U(5), col);
            if (T.drag_bone < 0 && ImGui::IsMouseClicked(0) && (mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y) < U(7) * U(7)) {
                T.drag_bone = b, T.drag_end = e, T.drag_view = which;
            }
        }
        if (ImGui::IsItemHovered()) {
            for (int e = 0; e < 2; e++) {
                ImVec2 c(at.x + sx[e], at.y + sy[e]);
                if ((mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y) < U(7) * U(7)) {
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
        /* who it's based on: its template, else (a pack that says none) the hero whose place it takes; saved, it's both */
        T.template_hero = info.template_said && info.template_hero >= 1 && info.template_hero <= 8 ? info.template_hero - 1
                          : info.prefer >= 1 && info.prefer <= 8                             ? info.prefer - 1
                                                                                              : 0;
        T.face_head = info.face_head;
        T.adj.assign(info.adjust, info.adjust + info.nadjust);
        T.undo.clear();
        T.redo.clear();
        for (int k = 0; k < PACK_KINDS; k++) {
            int th = info.kinds[k].template_hero;
            T.kind_template[k] = th >= 1 && th <= 8 && th - 1 != T.template_hero ? th - 1 : -1;
        }
        T.foot_own = !info.template_said || info.kinds[0].template_hero < 0;
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
    float w = U(620), x = (ImGui::GetIO().DisplaySize.x - w) / 2;
    ImGui::SetCursorPos(ImVec2(x, U(60)));
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
    if (ImGui::Button("Browse for a model...", ImVec2(U(200), 0))) {
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
    ImGui::BeginChild("found", ImVec2(0, U(220)), ImGuiChildFlags_Borders);
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
        if (ImGui::Selectable(label, false, 0, ImVec2(0, U(26)))) {
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
    ImGui::TextDisabled("Ctrl and + or - make everything bigger or smaller, kept for next time (Ctrl and 0: as your screen has it).");
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
            T.anim = T.have_made[k] ? breathe_of(&T.made[k]) : 0;
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
            ImGui::SameLine(ViewW + U(16));
            ImGui::Text("Side");
            view_window(T, T.front, "front", true, 0);
            ImGui::SameLine();
            view_window(T, T.side, "side", true, 1);
            ImGui::TextWrapped("Drag each dot onto its joint in both views: yellow its body, blue its arms, green its legs, orange its jaw, "
                               "ears and tail. A small dot is a bone's end.");
        } else {
            view_window(T, T.play, "move", false, 2);
            T.play.zx = ViewW / 2.0f;
            T.play.zy = ViewH / 2.0f;
            look_around(T.play, T.turn);
            ImGui::SetNextItemWidth(U(180));
            ImGui::Combo("Move", &T.move, [](void *, int i) { return KitMoves[i].name; }, nullptr, KitMoveCount);
            ImGui::SameLine();
            ImGui::Checkbox("Play", &T.playing);
            ImGui::SetNextItemWidth(U(180));
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
                    T.anim = breathe_of(&T.made[k]);
                    T.made_t = 0;
                }
                ImGui::SetItemTooltip("Open %s: its prop and moves", KindShort[k]);
            } else {
                ImGui::Dummy(ImVec2(ThumbW + U(8), ThumbH + U(8)));
            }
            ImGui::TextUnformatted(KindShort[k]);
            ImGui::EndGroup();
            ImGui::PopID();
            if ((k + 1) % cols != 0 && k + 1 < PACK_KINDS) {
                ImGui::SameLine();
            }
        }
        ImGui::SetNextItemWidth(U(200));
        ImGui::SliderFloat("Turn##all", &T.made_turn, -180, 180, "%.0f");
        return;
    }
    if (!T.have_made[T.kind]) {
        ImGui::TextWrapped(T.maker != nullptr ? "Making it..." : "The game didn't make this one: see the message at the top.");
        return;
    }
    float w = avail < PrevW ? avail : PrevW;
    bool adjusting = T.adjusting && T.have_pose[T.kind];
    ImGui::Image((ImTextureID)(intptr_t)T.game_view.tex, ImVec2(w, w * PrevH / PrevW));
    ImVec2 im = ImGui::GetItemRectMin();
    bool hovered = ImGui::IsItemHovered();
    look_around(T.game_view, T.made_turn, adjusting ? 1 : 0);
    const KitMade *mk = &T.made[T.kind];
    if (adjusting) {
        adjust_view(T, im, w / PrevW, hovered);
        if (hovered && !T.dragging) {
            ImGui::SetItemTooltip("Click a joint to pick it, then drag. The right mouse button turns the view; scroll to zoom.");
        }
    }
    ImGui::SetNextItemWidth(U(200));
    if (ImGui::BeginCombo("Watching", T.anim < mk->nanims ? mk->anims[T.anim].name : "")) {
        for (int a = 0; a < mk->nanims; a++) {
            if (ImGui::Selectable(mk->anims[a].name, a == T.anim)) {
                T.anim = a;
                T.made_t = 0;
                T.frame = 0;
            }
        }
        ImGui::EndCombo();
    }
    if (adjusting && mk->nanims > 0) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(U(120));
        ImGui::SliderFloat("Speed", &T.made_speed, 0, 2, "%.2f");
        timeline(T, mk, T.anim < mk->nanims ? T.anim : 0, w);
        return;
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(U(160));
    ImGui::SliderFloat("Turn##game", &T.made_turn, -180, 180, "%.0f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(U(120));
    ImGui::SliderFloat("Speed", &T.made_speed, 0, 2, "%.2f");
}

// A game's page, on the right: Adjust its moves (the animation tools), for fixing what the game makes of it there by hand
void adjust_panel(Tool &T) {
    if (!ImGui::CollapsingHeader("Adjust its moves", ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }
    if (T.kind == KIND_WARP_TOOL) {
        ImGui::TextWrapped("The podium is made from its moves on foot: adjust them on On foot (they show here once it's saved and "
                           "made again).");
        return;
    }
    if (!T.have_pose[T.kind]) {
        ImGui::TextWrapped(T.kind == 1 ? "Riding Crash's polar bear, it's sat on the bear by the game itself, so it can't be adjusted "
                                         "here. It can on another hero's bear (Prop from, above)."
                         : T.maker != nullptr ? "Making it..."
                                              : "Not ready to adjust: Update the preview.");
        return;
    }
    const KitMade *mk = &T.made[T.kind];
    const KitPose *po = &T.poses[T.kind];
    int a = T.anim < mk->nanims ? T.anim : 0;
    const char *move = mk->anims[a].name;
    int here = 0;
    for (auto &k : T.adj) {
        here += k.kind == T.kind;
    }
    if (ImGui::Checkbox("Show its joints, to adjust it", &T.adjusting) && T.adjusting) {
        T.frame = mk->anims[a].frames > 0 ? (int)(T.made_t * 30) % mk->anims[a].frames : 0;
        T.adj_playing = false;
    }
    hint("In the picture, click a joint to pick it, then drag. A hand, a foot or the hips (the squares) moves with the mouse, its arm or "
         "leg bending to follow; from any other joint, drag the way you want what's on it to point (Shift: drag round it to turn it). "
         "Turn the view with the right mouse button to move it forward or back. The values below set it exactly. Keys: Space plays, "
         "the arrows step a frame, Ctrl+Z undoes, Ctrl+Y does it again.");
    if (!T.adjusting) {
        if (here > 0) {
            ImGui::TextDisabled("%d change%s in this game.", here, here == 1 ? "" : "s");
        }
        return;
    }
    ImGui::Text("A change is made to");
    ImGui::RadioButton("this frame", &T.key_mode, KEY_FRAME);
    ImGui::SameLine();
    ImGui::RadioButton("the whole move", &T.key_mode, KEY_MOVE);
    ImGui::SameLine();
    ImGui::RadioButton("every move here", &T.key_mode, KEY_GAME);
    if (T.key_mode == KEY_FRAME) {
        ImGui::SetNextItemWidth(U(160));
        ImGui::SliderInt("Fades over", &T.spread, 0, 30, "%d frames");
        hint("Either side of the frame: its change is all there at that frame and fades to none this many frames away.");
    }
    ImGui::Spacing();
    if (T.sel >= 0 && T.sel < po->nbones) {
        const char *bone = po->names[T.sel];
        PackAdjust *k = adj_key(T, move, bone, false);
        float turn[3] = { 0, 0, 0 }, reach[3] = { 0, 0, 0 };
        if (k != nullptr) {
            memcpy(turn, k->turn, sizeof(turn));
            memcpy(reach, k->reach, sizeof(reach));
        }
        ImGui::SeparatorText(bone_title(bone));
        // its own axes as it stands: x to its left, y down, z to its back (turns about them; shown as a person would say them)
        static const char *const Turns[3] = { "Tip forward", "Turn to its right", "Lean to its left" };
        bool changed = false;
        for (int i = 0; i < 3; i++) {
            ImGui::SetNextItemWidth(U(200));
            changed |= ImGui::DragFloat(Turns[i], &turn[i], 0.5f, -180, 180, "%.1f deg");
            if (ImGui::IsItemActivated()) {
                adj_snapshot(T);
            }
        }
        if (reach_bone(bone)) {
            static const char *const Moves[3] = { "To its left", "Up", "Forward" };
            static const float Way[3] = { 1, -1, -1 };
            for (int i = 0; i < 3; i++) {
                float shown = reach[i] * Way[i];
                ImGui::SetNextItemWidth(U(200));
                if (ImGui::DragFloat(Moves[i], &shown, 0.5f, -500, 500, "%.1f")) {
                    reach[i] = shown * Way[i];
                    changed = true;
                }
                if (ImGui::IsItemActivated()) {
                    adj_snapshot(T);
                }
            }
        }
        if (changed && (k = adj_key(T, move, bone, true)) != nullptr) {
            memcpy(k->turn, turn, sizeof(turn));
            memcpy(k->reach, reach, sizeof(reach));
        }
        if (T.key_mode == KEY_FRAME) {
            ImGui::TextDisabled("At frame %d of %s.", T.frame, move);
        } else {
            ImGui::TextDisabled(T.key_mode == KEY_MOVE ? "In all of %s." : "In every move here%s.", T.key_mode == KEY_MOVE ? move : "");
        }
        if (ImGui::Button(other_side(bone)[0] != 0 ? "Mirror to its other side" : "Mirror (it has no other side)")
            && other_side(bone)[0] != 0) {
            adj_snapshot(T);
            adj_mirror(T, move, bone);
        }
        ImGui::SetItemTooltip("Its changes in this move given to its other side, the other way round");
        ImGui::SameLine();
        if (ImGui::Button("Reset this joint")) {
            adj_snapshot(T);
            std::vector<PackAdjust> keep;
            for (auto &x : T.adj) {
                if (!(x.kind == T.kind && pack_adjust_for(&x, T.kind, move) && SDL_strcasecmp(x.bone, bone) == 0)) {
                    keep.push_back(x);
                }
            }
            T.adj = keep;
        }
        ImGui::SetItemTooltip("Its changes in this move taken away (and those to every move here)");
    } else {
        ImGui::TextDisabled("Pick a joint in the picture.");
    }
    ImGui::Spacing();
    ImGui::BeginDisabled(T.undo.empty());
    if (ImGui::Button("Undo")) {
        adj_undo(T);
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(T.redo.empty());
    if (ImGui::Button("Redo")) {
        adj_redo(T);
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Reset this move")) {
        adj_snapshot(T);
        std::vector<PackAdjust> keep;
        for (auto &x : T.adj) {
            if (!(x.kind == T.kind && SDL_strcasecmp(x.move, move) == 0)) {
                keep.push_back(x);
            }
        }
        T.adj = keep;
    }
    ImGui::SetItemTooltip("All its changes in %s taken away", move);
    // its changes in this move: each picked to go to it, or taken away
    ImGui::SeparatorText("Its changes in this move");
    int shown = 0, gone = -1;
    for (int i = 0; i < (int)T.adj.size(); i++) {
        PackAdjust &x = T.adj[i];
        if (!pack_adjust_for(&x, T.kind, move)) {
            continue;
        }
        char line[160];
        int b = -1;
        for (int j = 0; j < po->nbones; j++) {
            b = SDL_strcasecmp(po->names[j], x.bone) == 0 ? j : b;
        }
        snprintf(line, sizeof(line), "%s: %s%s##%d", bone_title(x.bone),
                 x.at >= 0 ? "" : strcmp(x.move, "*") == 0 ? "every move here" : "the whole move", "", i);
        if (x.at >= 0) {
            snprintf(line, sizeof(line), "%s: frame %d, fading over %d##%d", bone_title(x.bone), x.at, x.spread, i);
        }
        ImGui::PushID(i);
        if (ImGui::SmallButton("x")) {
            gone = i;
        }
        ImGui::SetItemTooltip("Take it away");
        ImGui::PopID();
        ImGui::SameLine();
        if (ImGui::Selectable(line, b == T.sel && (x.at < 0 || x.at == T.frame))) {
            T.sel = b;
            if (x.at >= 0) {
                T.frame = x.at;
                T.key_mode = KEY_FRAME;
                T.spread = x.spread;
            } else {
                T.key_mode = strcmp(x.move, "*") == 0 ? KEY_GAME : KEY_MOVE;
            }
            T.adj_playing = false;
        }
        shown++;
    }
    if (gone >= 0) {
        adj_snapshot(T);
        T.adj.erase(T.adj.begin() + gone);
    }
    if (shown == 0) {
        ImGui::TextDisabled("None yet.");
    }
    ImGui::TextDisabled("They're saved with the hero (Save to mods folder).");
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
            ImGui::SetNextItemWidth(U(160));
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
            snprintf(now, sizeof(now), "%s's (who it's based on)", HeroNames[T.template_hero]);
            ImGui::Text(k == 0 ? "Stands like" : k == 1 ? "Rides the bear of" : "Prop from");
            ImGui::SameLine(U(130));
            ImGui::SetNextItemWidth(-1);
            bool own = k == 0 && T.foot_own;
            if (ImGui::BeginCombo("##from", own ? "itself: its own moves" : T.kind_template[k] < 0 ? now : HeroNames[T.kind_template[k]])) {
                // on foot, its own moves: its model's clips, and the tool's for the rest
                if (k == 0 && ImGui::Selectable("itself: its own moves", own)) {
                    T.foot_own = true;
                    make_in_game(T);
                }
                if (ImGui::Selectable(now, !own && T.kind_template[k] < 0)) {
                    T.kind_template[k] = -1;
                    T.foot_own = k == 0 ? false : T.foot_own;
                    make_in_game(T);
                }
                for (int h = 0; h < 8; h++) {
                    if (h != T.template_hero && ImGui::Selectable(HeroNames[h], !own && T.kind_template[k] == h)) {
                        T.kind_template[k] = h;
                        T.foot_own = k == 0 ? false : T.foot_own;
                        make_in_game(T);
                    }
                }
                ImGui::EndCombo();
            }
            if (k == 0) {
                hint("Itself: on foot it plays its own moves (its model's clips, and the tool's moves on it for the rest: see Model and "
                     "joints). A hero: it stands and moves as that hero does.");
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
                ImGui::SameLine(U(130));
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
            if (k == 0 && T.foot_own) {
                ImGui::TextWrapped("Its moves here are its own: its model's clips, and the tool's moves on it for the rest.");
            } else {
                ImGui::TextWrapped("Its moves here are %s's.", HeroNames[T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero]);
            }
        }
    }
    if (T.page >= 0) {
        adjust_panel(T);
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
        if (ImGui::Combo("Based on", &T.template_hero, HeroNames, 8)) {
            T.dirty = true;
        }
        hint("The hero it plays as: their moves, speed and reach, and their ride or prop in every game (each game can pick another "
             "prop). It takes their place in the game, or the first free hero's if someone's playing them, and uses their sound "
             "for any it hasn't one of.");
        if (ImGui::SliderFloat("Face", &T.face_head, 0, 0.6f, T.face_head > 0 ? "top %.2f" : "the game's choice")) {
            T.dirty = true;
        }
        hint("How much of the model, from the top, is its head: its face on the panel is drawn from that, if it has no card.");
    }
    if (ImGui::CollapsingHeader("Card and sounds")) {
        ImGui::Text("Card");
        hint("A picture for the character select (PNG). Without one, its face is drawn from the model.");
        ImGui::SameLine(U(110));
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
        hint("A WAV for each job. A job without one uses the sound of the hero it's based on.");
        for (int j = 0; j < PACK_VOICE_JOBS; j++) {
            ImGui::PushID(j);
            ImGui::TextUnformatted(PackVoiceJobs[j]);
            ImGui::SameLine(U(110));
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
        if (ImGui::Button("Save to mods folder", ImVec2(-1, U(30)))) {
            export_pack(T, false);
        }
        ImGui::SetItemTooltip("%s", T.loaded_mod[0] ? T.loaded_mod : T.out);
        ImGui::SetNextItemWidth(U(160));
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
    // adjusting a move: Space plays or stops it, the arrows step a frame, Ctrl+Z undoes and Ctrl+Y (or Ctrl+Shift+Z) does it again
    ImGuiIO &io = ImGui::GetIO();
    if (T.adjusting && T.page >= 0 && T.have_pose[T.kind] && !io.WantTextInput) {
        if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
            T.adj_playing = !T.adj_playing;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            T.frame--;
            T.adj_playing = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            T.frame++;
            T.adj_playing = false;
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
            io.KeyShift ? adj_redo(T) : adj_undo(T);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y, false)) {
            adj_redo(T);
        }
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
        ImGui::TableSetupColumn("pages", ImGuiTableColumnFlags_WidthFixed, U(190));
        ImGui::TableSetupColumn("view", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("choices", ImGuiTableColumnFlags_WidthFixed, U(410));
        ImGui::TableNextColumn();
        editor_left(T);
        ImGui::TableNextColumn();
        ImGui::BeginChild("middle", ImVec2(0, ImGui::GetContentRegionAvail().y), 0,
                          T.page != PAGE_ALL ? ImGuiWindowFlags_NoScrollWithMouse : 0);
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

// A view's picture let go, to be made again at its new size
void view_free(View &v) {
    delete[] v.rgba;
    delete[] v.depth;
    if (v.tex != nullptr) {
        SDL_DestroyTexture(v.tex);
    }
    v.rgba = nullptr;
    v.depth = nullptr;
    v.tex = nullptr;
}

// The window at scale s (Ui): its font made at that size, its style's sizes from the plain one (base), and its pictures that big
void ui_scale(Tool &T, const ImGuiStyle &base, float s) {
    ImGuiIO &io = ImGui::GetIO();
    ImFontConfig cfg;

    Ui = s < 0.75f ? 0.75f : s > 4 ? 4 : s;
    io.Fonts->Clear();
    cfg.SizePixels = floorf(13 * Ui + 0.5f);
    io.Fonts->AddFontDefault(&cfg);
    ImGui_ImplSDLRenderer3_DestroyFontsTexture();
    ImGui::GetStyle() = base;
    ImGui::GetStyle().ScaleAllSizes(Ui);
    ViewW = (int)U(300), ViewH = (int)U(420);
    PrevW = (int)U(600), PrevH = (int)U(560);
    ThumbW = (int)U(220), ThumbH = (int)U(180);
    view_free(T.front);
    view_free(T.side);
    view_free(T.play);
    view_free(T.game_view);
    for (int k = 0; k < PACK_KINDS; k++) {
        view_free(T.thumbs[k]);
        T.thumbs[k].w = ThumbW;
        T.thumbs[k].h = ThumbH;
    }
}

// The scale for a screen: its display scale (Windows' 150%, 200%), or its height against an 800-line screen's if that's more (1.25 on a
// 1080-line screen; a high resolution screen left at 100% more), in quarters
float screen_scale(SDL_DisplayID d) {
    float s = SDL_GetDisplayContentScale(d);
    SDL_Rect r;

    if (SDL_GetDisplayUsableBounds(d, &r)) {
        s = fmaxf(s, floorf(r.h / 800.0f * 4) / 4);
    }
    return s < 1 ? 1 : s > 4 ? 4 : s;
}

// The user's share of a screen's scale (Ctrl with + and -), kept in the tool's own folder (SDL's pref path, zoom.txt) for next time
float Zoom = 1;

void zoom_load() {
    char *pref = SDL_GetPrefPath("CrashBashPC", "herotool"), path[1100];
    size_t n = 0;

    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%szoom.txt", pref);
    SDL_free(pref);
    char *text = (char *)SDL_LoadFile(path, &n);
    if (text != nullptr) {
        float z = (float)SDL_atof(text);
        Zoom = z >= 0.5f && z <= 3 ? z : 1;
        SDL_free(text);
    }
}

void zoom_keep() {
    char *pref = SDL_GetPrefPath("CrashBashPC", "herotool"), path[1100], text[32];

    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%szoom.txt", pref);
    SDL_free(pref);
    snprintf(text, sizeof(text), "%.2f\n", Zoom);
    SDL_SaveFile(path, text, strlen(text));
}

// The window as big as the scale wants (U(1360) by U(900)), within its screen; centred on it if centre
void fit_window(SDL_Window *win, SDL_DisplayID d, bool centre) {
    SDL_Rect r;
    int w = (int)U(1360), h = (int)U(900);

    if (SDL_GetDisplayUsableBounds(d, &r)) {
        w = w > r.w * 95 / 100 ? r.w * 95 / 100 : w;
        h = h > r.h * 92 / 100 ? r.h * 92 / 100 : h;
    }
    SDL_SetWindowSize(win, w, h);
    if (centre) {
        SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED_DISPLAY(d), SDL_WINDOWPOS_CENTERED_DISPLAY(d));
    }
}

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
    // on the screen the mouse is on (where the game's launcher was clicked), as big as that screen's scale wants, the window too
    const ImGuiStyle base_style = ImGui::GetStyle();
    SDL_DisplayID screen;
    {
        float mx = 0, my = 0;
        SDL_GetGlobalMouseState(&mx, &my);
        SDL_Point at = { (int)mx, (int)my };
        screen = SDL_GetDisplayForPoint(&at);
        screen = screen != 0 ? screen : SDL_GetPrimaryDisplay();
    }
    zoom_load();
    float want_scale = SDL_getenv("HEROTOOL_SCALE") != nullptr ? (float)SDL_atof(SDL_getenv("HEROTOOL_SCALE")) : screen_scale(screen) * Zoom;
    ui_scale(T, base_style, want_scale);
    fit_window(win, screen, true);
    {
        int n = 0;
        SDL_DisplayID *all = SDL_GetDisplays(&n);
        for (int i = 0; all != nullptr && i < n; i++) {
            SDL_Rect r = {};
            SDL_GetDisplayUsableBounds(all[i], &r);
            printf("herotool: screen %u, %dx%d, its scale %.2f%s\n", (unsigned)all[i], r.w, r.h, SDL_GetDisplayContentScale(all[i]),
                   all[i] == screen ? ": the window's, at its scale " : "");
        }
        SDL_free(all);
        printf("herotool: the window at %.2f\n", Ui);
    }
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
    T.game_view.pitch = 12;
    T.play.pitch = 8;
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
            if (e.type == SDL_EVENT_KEY_DOWN && (e.key.mod & SDL_KMOD_CTRL) != 0) {
                SDL_Keycode k = e.key.key;
                bool bigger = k == SDLK_EQUALS || k == SDLK_PLUS || k == SDLK_KP_PLUS, smaller = k == SDLK_MINUS || k == SDLK_KP_MINUS;
                if (bigger || smaller || k == SDLK_0 || k == SDLK_KP_0) {
                    // as a share of its screen's, kept for next time
                    Zoom = bigger ? Zoom + 0.1f : smaller ? Zoom - 0.1f : 1;
                    Zoom = Zoom < 0.5f ? 0.5f : Zoom > 3 ? 3 : Zoom;
                    want_scale = screen_scale(SDL_GetDisplayForWindow(win)) * Zoom;
                    zoom_keep();
                }
            } else if (e.type == SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED || e.type == SDL_EVENT_WINDOW_DISPLAY_CHANGED) {
                // moved to another screen: its scale, and the window grown or shrunk with it (as far as it was the size it started)
                float was = Ui, now = screen_scale(SDL_GetDisplayForWindow(win)) * Zoom;
                if (fabsf(now - was) > 0.01f) {
                    int w = 0, h = 0;
                    SDL_GetWindowSize(win, &w, &h);
                    SDL_Rect r;
                    if (SDL_GetDisplayUsableBounds(SDL_GetDisplayForWindow(win), &r)) {
                        w = (int)fminf(w * now / was, r.w * 0.95f);
                        h = (int)fminf(h * now / was, r.h * 0.92f);
                        SDL_SetWindowSize(win, w, h);
                    }
                    want_scale = now;
                }
            }
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
        /* a test's: HEROTOOL_ADJUST=BONE@FRAME adjusts the game shown once it's made, that bone picked at that frame */
        static bool adjust_set = false;
        if (!adjust_set && SDL_getenv("HEROTOOL_ADJUST") != nullptr && T.page >= 0 && T.have_pose[T.kind]) {
            char bone[64];
            const char *at = strchr(SDL_getenv("HEROTOOL_ADJUST"), '@');
            SDL_strlcpy(bone, SDL_getenv("HEROTOOL_ADJUST"), sizeof(bone));
            if (strchr(bone, '@') != nullptr) {
                *strchr(bone, '@') = 0;
            }
            T.adjusting = true;
            T.frame = at != nullptr ? SDL_atoi(at + 1) : 0;
            for (int b = 0; b < T.poses[T.kind].nbones; b++) {
                T.sel = SDL_strcasecmp(T.poses[T.kind].names[b], bone) == 0 ? b : T.sel;
            }
            adjust_set = true;
            /* and HEROTOOL_ADJUST_TURN / _REACH=X,Y,Z changes it there, as the panel's values would */
            const char *turn = SDL_getenv("HEROTOOL_ADJUST_TURN"), *reach = SDL_getenv("HEROTOOL_ADJUST_REACH");
            PackAdjust *k = T.sel >= 0 && (turn != nullptr || reach != nullptr)
                                ? adj_key(T, T.made[T.kind].anims[T.anim].name, T.poses[T.kind].names[T.sel], true)
                                : nullptr;
            if (k != nullptr) {
                if (turn != nullptr) {
                    SDL_sscanf(turn, "%f,%f,%f", &k->turn[0], &k->turn[1], &k->turn[2]);
                }
                if (reach != nullptr) {
                    SDL_sscanf(reach, "%f,%f,%f", &k->reach[0], &k->reach[1], &k->reach[2]);
                }
            }
        }
        if (T.page == PAGE_ALL) {
            for (int k = 0; k < PACK_KINDS; k++) {
                if (T.have_made[k] && T.made[k].nanims > 0) {
                    T.thumbs[k].yaw = T.made_turn;
                    T.thumbs[k].pitch = T.game_view.pitch;
                    int b = breathe_of(&T.made[k]), f = (int)(T.made_t * 30) % T.made[k].anims[b].frames;
                    // with its adjustments now, where there's a pose to make them on
                    bool live = adjusted_frame(T, k, b, f, T.live);
                    draw_made(&T.made[k], b, f, T.thumbs[k], ren, T.rides[k], T.nrides[k], live ? T.live.data() : nullptr);
                }
            }
        }
        if (T.page >= 0 && T.have_made[T.kind]) {
            const KitMade *mk = &T.made[T.kind];
            int a = T.anim < mk->nanims ? T.anim : 0;

            T.game_view.yaw = T.made_turn;
            // 30 frames a second, as the game plays a move a frame a tick; a kind made with more frames a tick has as many more. While
            // it's adjusted, the frame picked (played on if it's playing)
            bool adjusting = T.adjusting && T.have_pose[T.kind];
            if (adjusting && T.adj_playing && mk->nanims > 0) {
                T.adj_t += dt * T.made_speed;
                for (; T.adj_t >= 1 / 30.0f; T.adj_t -= 1 / 30.0f) {
                    T.frame = (T.frame + 1) % mk->anims[a].frames;
                }
            }
            int f = mk->nanims > 0 ? adjusting ? T.frame % mk->anims[a].frames : (int)(T.made_t * 30) % mk->anims[a].frames : 0;
            bool live = adjusted_frame(T, T.kind, a, f, T.live);
            draw_made(mk, a, f, T.game_view, ren, T.rides[T.kind], T.nrides[T.kind], live ? T.live.data() : nullptr);
        }
        if (T.m != nullptr) {
            // the front and side views at rest, the third playing the move
            draw(T.m, T.m->pos, T.front, ren);
            draw(T.m, T.m->pos, T.side, ren);
            pose(T);
            T.play.yaw = T.turn;
            draw(T.m, T.posed, T.play, ren);
        }
        if (fabsf(want_scale - Ui) > 0.01f) {
            ui_scale(T, base_style, want_scale);
            want_scale = Ui;
        }
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        /* a test's drag: HEROTOOL_DRAG=X0,Y0,X1,Y1[,SHIFT] presses the left button at (X0, Y0) in the window once it's adjusting, moves
           to (X1, Y1) over ten frames and lets go (Shift held if SHIFT is 1), as a person's mouse would */
        static int drag_step = -1;
        if (SDL_getenv("HEROTOOL_DRAG") != nullptr && T.adjusting && drag_step < 14) {
            float d[5] = { 0, 0, 0, 0, 0 };
            SDL_sscanf(SDL_getenv("HEROTOOL_DRAG"), "%f,%f,%f,%f,%f", &d[0], &d[1], &d[2], &d[3], &d[4]);
            ImGuiIO &dio = ImGui::GetIO();
            drag_step++;
            float t = drag_step < 2 ? 0 : drag_step > 11 ? 1 : (drag_step - 1) / 10.0f;
            dio.AddKeyEvent(ImGuiMod_Shift, d[4] != 0 && drag_step >= 1 && drag_step <= 12);
            dio.AddMousePosEvent(d[0] + (d[2] - d[0]) * t, d[1] + (d[3] - d[1]) * t);
            dio.AddMouseButtonEvent(0, drag_step >= 1 && drag_step <= 12);
        }
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
            /* and HEROTOOL_LIVE=FILE the frame shown as it made it, its vertices (s16 x, y, z each), to set beside the game's */
            if (SDL_getenv("HEROTOOL_LIVE") != nullptr && T.page >= 0 && adjusted_frame(T, T.kind, T.anim, T.frame, T.live)) {
                SDL_SaveFile(SDL_getenv("HEROTOOL_LIVE"), T.live.data(), T.live.size() * sizeof(int16_t));
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
