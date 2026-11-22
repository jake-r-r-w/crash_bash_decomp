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
#include <algorithm>
#include <cstdarg>
#include <thread>
#include <string>
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
    // a floor's grid under it, as Blender's (draw_made's: floor_grid)
    bool grid = false;
    // what it was last drawn from (drawn_same), so a picture that wouldn't change isn't drawn again; and the move it was last fitted to
    // (draw_made), its lowest and highest
    std::vector<unsigned char> drawn;
    const void *fit_of = nullptr;
    int fit_anim = -1;
    unsigned fit_gen = 0;
    float fit_lo[3] = {}, fit_hi[3] = {};
};

// Each time the game's models are read in again (or let go), one more: what a picture was drawn from is known by it as well as by where
// it is (a model read in again may be where the last one was)
unsigned MadeGen = 1;

// What a picture is drawn from, as bytes, to be set beside what it was last drawn from (drawn_same)
struct DrawnFrom {
    std::vector<unsigned char> k;
    void add(const void *p, size_t n) {
        k.insert(k.end(), (const unsigned char *)p, (const unsigned char *)p + n);
    }
    template <class V> void add(const V &x) {
        add(&x, sizeof(x));
    }
};

// A picture PW x PH made in bands of its rows, each band's fn(its first row, the row after it) on a thread of its own (as many as the
// machine has, up to 8, a band at least 32768 pixels: a smaller one would take longer to start than it saves), so it's made in a share
// of the time. Each band's pixels are its own, and made in the same order, so the picture is the same as made in one.
template <class F> void in_bands(int PW, int PH, F fn) {
    static const int most = (int)std::min(8u, std::max(1u, std::thread::hardware_concurrency()));
    int n = std::min(most, std::max(1, PW * PH / 32768));
    std::thread th[8];

    if (n <= 1) {
        fn(0, PH);
        return;
    }
    for (int i = 0; i < n; i++) {
        int ya = PH * i / n, yb = PH * (i + 1) / n;
        th[i] = std::thread([&fn, ya, yb] { fn(ya, yb); });
    }
    for (int i = 0; i < n; i++) {
        th[i].join();
    }
}

// Whether a view would be drawn as it was last, from now (its turn, tilt, zoom, size and what's in it): if not, now is kept as what it
// was drawn from. Its picture's made again only when this says it's changed (the user's word, 2026-11-22: "it lags abit").
bool drawn_same(View &v, DrawnFrom &now) {
    now.add(v.yaw), now.add(v.pitch), now.add(v.zoom), now.add(v.zx), now.add(v.zy), now.add(v.w), now.add(v.h), now.add(v.grid);
    now.add(v.tex);
    if (v.tex != nullptr && now.k == v.drawn) {
        return true;
    }
    v.drawn.swap(now.k);
    return false;
}

// A triangle's pixels on a picture PW wide and PH high whose middles are in it, in its rows ya to yb (yb not), and nearer than what's
// there (depth), each as f(its index, its share of each corner a, b and d, its depth z). Row by row across only what it covers, its
// shares added as it goes (no division a pixel): what both pictures are made with.
template <class F>
void fill_tri(const float sx[3], const float sy[3], const float sz[3], const float *depth, int PW, int PH, int ya, int yb, F f) {
    float area = (sx[1] - sx[0]) * (sy[2] - sy[0]) - (sx[2] - sx[0]) * (sy[1] - sy[0]);

    if (fabsf(area) < 1e-6f) {
        return;
    }
    float inv = 1 / area;
    // a corner's share at a pixel's middle (px, py) is A px + B py + C: corner 0's, corner 1's, and the rest corner 2's
    float A0 = (sy[1] - sy[2]) * inv, B0 = (sx[2] - sx[1]) * inv, C0 = (sx[1] * sy[2] - sx[2] * sy[1]) * inv;
    float A1 = (sy[2] - sy[0]) * inv, B1 = (sx[0] - sx[2]) * inv, C1 = (sx[2] * sy[0] - sx[0] * sy[2]) * inv;
    float A2 = -A0 - A1;
    int x0 = (int)fmaxf(0, floorf(fminf(sx[0], fminf(sx[1], sx[2])))), x1 = (int)fminf(PW - 1, ceilf(fmaxf(sx[0], fmaxf(sx[1], sx[2]))));
    int y0 = (int)fmaxf((float)ya, floorf(fminf(sy[0], fminf(sy[1], sy[2]))));
    int y1 = (int)fminf((float)(yb < PH ? yb : PH) - 1, ceilf(fmaxf(sy[0], fmaxf(sy[1], sy[2]))));
    for (int y = y0; y <= y1; y++) {
        float py = y + 0.5f, k0 = B0 * py + C0, k1 = B1 * py + C1, k2 = 1 - k0 - k1, lo = (float)x0, hi = (float)x1 + 1;
        // the row's span where every share's at least 0 (a pixel more each end, each pixel tested as well)
        auto keep = [&](float A, float K) {
            if (A > 0) {
                lo = fmaxf(lo, -K / A);
            } else if (A < 0) {
                hi = fminf(hi, -K / A);
            } else if (K < 0) {
                hi = lo - 2;
            }
        };
        keep(A0, k0);
        keep(A1, k1);
        keep(A2, k2);
        if (hi < lo - 1) {
            continue;
        }
        int xs = (int)fmaxf((float)x0, floorf(lo - 0.5f)), xe = (int)fminf((float)x1, ceilf(hi - 0.5f));
        float px = xs + 0.5f, a = A0 * px + k0, b = A1 * px + k1;
        for (int x = xs; x <= xe; x++, a += A0, b += A1) {
            float d = 1 - a - b;
            if (a < 0 || b < 0 || d < 0) {
                continue;
            }
            float z = a * sz[0] + b * sz[1] + d * sz[2];
            int at = y * PW + x;
            if (z >= depth[at]) {
                continue;
            }
            f(at, a, b, d, z);
        }
    }
}

enum { OWN_TEMPLATE = -1, OWN_CLIP = -2, OWN_MADE = -3 };

// A step of the undo history (the user's word, 2026-11-22: "add undo history"): its adjustments before it, and what it was, as the
// history's list says it ("Moved Head's key to frame 32")
struct Step {
    std::vector<PackAdjust> adj;
    std::string says;
};

// A move of its own copied from its template's (the animation tools, bake_move): as the game made it then, kept whatever it's based on
struct BakedMove {
    int kind, from;           // the game (PACK_KINDS'), and the hero it was copied from (HeroNames')
    char move[32], name[48];  // its move, and its clip's name (KIND/MOVE, foot/MOVE on foot)
    int frames;
    float length;
    std::vector<float> rot, hips; // each frame's turns of the standard bones (KB_COUNT x 4) and the hips' move (3), as kit_pose gives
};
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
    std::vector<Step> undo, redo;
    bool adjusting = false, adj_playing = false, dragging = false, bare = false;
    int sel = -1, key_mode = 0, spread = 4, frame = 0;
    float adj_t = 0;
    // the frame shown made again with its adjustments, and the picked bone's joint and each bone's on the screen
    std::vector<int16_t> live;
    // its moves of its own copied from its template's (OWN_MADE), its moves on foot of its own given back to its template (their clips
    // left out), what was last made its own (said on the page), and a preview asked for while one was being made
    std::vector<BakedMove> baked;
    std::vector<std::string> foot_back;
    char own_note[200] = "", watching[32] = ""; // (and the move watched, kept as it's made again)
    bool remake = false;
    // a change to a move of its own waiting on the user's yes (confirm_back): the game, its move and what it's to be
    // the gizmo (GIZMO_TURN, _MOVE, _SIZE), the handle of it being dragged, and the key picked in the timeline, dragged or scrubbed
    int gizmo = 0, handle = 0, key_sel = -1;
    bool key_drag = false, key_moved = false, scrub = false;
    // a key dragged in the timeline: the part of it (KEY_BODY moved, KEY_START or KEY_END held over more frames or fewer, KEY_EITHER
    // its end the way it's dragged), the frame it was taken at, where it was from and to; and the fade being changed (its step taken)
    int key_part = 0, key_grab = 0, key_from = 0, key_to = 0;
    bool fade_step = false;
    // the keys picked (T.adj's places; key_sel the one picked last, the panel's), a drag of several (each one's first and last frame
    // as it was taken; a click on one of them that picks it alone if it's let go unmoved), and a box being dragged to pick them
    std::vector<int> keys_sel;
    std::vector<std::pair<int, int>> drag_orig;
    bool key_pending = false, box = false;
    float box_x = 0, box_y = 0;
    // the timeline zoomed in (1 all of the move) from its first frame shown; the frame shown as a key's drag began, and the frame
    // it's snapped to (-1 none)
    float tl_zoom = 1, tl_first = 0;
    int drag_frame0 = 0, snapped = -1;
    // X-Mirror: each change to a joint's key made to its other side's too, flipped
    bool xmirror = false;
    // onion skin: the frames onion_step before and after the one shown, as ghosts (their vertices)
    bool onion = false;
    int onion_step = 4;
    std::vector<int16_t> ghost[2];
    // the clipping finder on: its points on the picture, its frames in the timeline
    bool clipping = false;
    // how a new key fades (PACK_EASE_), and the key copied (Ctrl+C; empty none)
    int ease = PACK_EASE_SMOOTH;
    std::vector<PackAdjust> clip;
    int ask_kind = -1, ask_move = -1, ask_to = OWN_TEMPLATE;
    char ask_name[32] = "";
    bool ask_open = false;
};
enum { KEY_FRAME, KEY_MOVE, KEY_GAME };
enum { GIZMO_TURN, GIZMO_MOVE, GIZMO_SIZE };
enum { KEY_BODY, KEY_START, KEY_END, KEY_EITHER };

// Each way a key can fade in and out (PACK_EASE_), as the tool says it, and what it does
const char *const EaseSays[PACK_EASES] = { "Smooth", "Steady", "Slow start", "Slow finish", "Sudden", "Overshoot", "Bounce", "Wobble" };
const char *const EaseTips[PACK_EASES] = {
    "Eases in and out",
    "Even rate",
    "Slow, then quick",
    "Quick, then settles",
    "No fade",
    "Goes past and back",
    "Bounces in",
    "Springs past and back",
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

// A view's picture (the item just drawn) dragged with the right mouse button to turn and tilt it, scrolled to zoom it, and set back by
// double-clicking it with the right one (or Home); yaw the turn it's drawn with. The same in every view, watched or edited (the user's
// word, 2026-11-22: the buttons swapped between them), the left one free to pick and drag joints. tip: says so as it's hovered.
void look_around(View &v, float &yaw, bool tip = true) {
    if (!ImGui::IsItemHovered() && !(ImGui::IsItemActive())) {
        return;
    }
    ImGuiIO &io = ImGui::GetIO();
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        // turned as the hand goes, as if it were held: its front goes the way the mouse does (the user's word, 2026-11-22: it went
        // back to front)
        yaw -= io.MouseDelta.x * 0.5f;
        yaw = yaw > 180 ? yaw - 360 : yaw < -180 ? yaw + 360 : yaw;
        v.pitch = fmaxf(-90, fminf(90, v.pitch + io.MouseDelta.y * 0.4f));
    }
    if (io.MouseWheel != 0) {
        v.zoom = fmaxf(0.4f, fminf(6, v.zoom * powf(1.15f, io.MouseWheel)));
    }
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Home, false)) {
        v.pitch = 12, v.zoom = 1, yaw = 30;
    }
    if (tip) {
        ImGui::SetItemTooltip("Right-drag: turn   Wheel: zoom   Home: reset");
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
    DrawnFrom now;
    now.add(m), now.add(m->ntri), now.add(m->nvert), now.add(v.scale), now.add(v.ox), now.add(v.oy), now.add(ViewW), now.add(ViewH);
    now.add(pos, sizeof(float) * 3 * (size_t)m->nvert);
    if (drawn_same(v, now)) {
        return;
    }
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
    in_bands(ViewW, ViewH, [&](int ya, int yb) {
    std::fill(v.rgba + ya * ViewW, v.rgba + yb * ViewW, 0xFF3A3434u);
    std::fill(v.depth + ya * ViewW, v.depth + yb * ViewW, 1e30f);
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
        float ln = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        float facing = ln > 0 ? ((n[0] * s + n[2] * c) * 0.7f + n[1] * 0.5f) / ln : 0, lit = 0.55f + 0.45f * fmaxf(0, facing);
        fill_tri(sx, sy, sz, v.depth, ViewW, ViewH, ya, yb, [&](int at, float a, float b, float d, float z) {
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
            v.depth[at] = z;
            v.rgba[at] = 0xFF000000u | bl << 16 | g << 8 | r;
        });
    }
    });
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
    T.baked.clear();
    T.foot_back.clear();
    T.own_note[0] = 0;
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
    snprintf(T.status, sizeof(T.status), "%s: %d triangles, %d vertices, %s", base, m->ntri, m->nvert,
             T.rigged ? "rigged" : "no rig (place its joints)");
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
    // its moves of its own copied from its template's, as clips (in the preview too: they're its moves, not changes made on them)
    static std::vector<KitBaked> bk;
    bk.clear();
    for (auto &b : T.baked) {
        KitBaked k = {};
        SDL_strlcpy(k.name, b.name, sizeof(k.name));
        k.frames = b.frames;
        k.length = b.length;
        k.rot = (const float(*)[KB_COUNT][4])b.rot.data();
        k.hips = (const float(*)[3])b.hips.data();
        bk.push_back(k);
    }
    p.baked = bk.empty() ? nullptr : bk.data();
    p.nbaked = (int)bk.size();
    // its moves on foot of its own given back to its template: their clips left out
    for (auto &n : T.foot_back) {
        if (nd < KIT_DROP_MOST) {
            snprintf(drops[nd], sizeof(drops[0]), "%s", n.c_str());
            p.drop[nd] = drops[nd];
            nd++;
        }
    }
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
        snprintf(T.status, sizeof(T.status), "Saved to %s", folder);
        if (test) {
            char mode[16];
            snprintf(mode, sizeof(mode), "%d", Modes[T.mode].mode);
            const char *args[] = { T.game, "--mods-dir", T.out, "--mod", T.id, "--extra-hero", "0", T.id, "--mode", mode, "--skip-intro",
                                   "--skip-loading", nullptr };
            SDL_Process *proc = SDL_CreateProcess(args, false);

            if (proc == nullptr) {
                snprintf(T.status, sizeof(T.status), "Saved, but the game didn't start (%s): is it at %s?", SDL_GetError(), T.game);
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
    MadeGen++;
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

// Where the tool keeps its own things (its previews, its zoom), to SDL_free: its folder for the user's settings, or a test's own
// (HEROTOOL_PREF=FOLDER/, so a test doesn't touch what a tool that's open has there)
char *pref_path() {
    const char *test = SDL_getenv("HEROTOOL_PREF");

    if (test != nullptr) {
        SDL_CreateDirectory(test);
        return SDL_strdup(test);
    }
    return SDL_GetPrefPath("CrashBashPC", "herotool");
}

void make_in_game(Tool &T) {
    char mods[1100];
    const char *pref = pref_path();

    if (T.m == nullptr || T.maker != nullptr || pref == nullptr) {
        T.remake = T.m != nullptr && T.maker != nullptr;
        SDL_free((void *)pref);
        return;
    }
    T.made_asked = true;
    T.remake = false;
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
    snprintf(T.status, sizeof(T.status), T.maker != nullptr ? "Making..." : "The game didn't start (%s): is it at %s?",
             SDL_GetError(), T.game);
}

void made_check(Tool &T) {
    int code = 0;

    if (T.maker == nullptr || !SDL_WaitProcess(T.maker, false, &code)) {
        return;
    }
    SDL_DestroyProcess(T.maker);
    T.maker = nullptr;
    T.watching[0] = 0;
    if (T.have_made[T.kind] && T.anim >= 0 && T.anim < T.made[T.kind].nanims) {
        SDL_strlcpy(T.watching, T.made[T.kind].anims[T.anim].name, sizeof(T.watching));
    }
    made_forget(T);
    if (T.remake) {
        // asked for again while it was being made (a move made its own): made again with that, and this one's used meanwhile
        make_in_game(T);
    }
    int have = 0;
    for (int k = 0; k < PACK_KINDS; k++) {
        char path[1300];

        snprintf(path, sizeof(path), "%s/%s_%s.hpv", T.made_dir, T.id, pack_kind_name(k));
        T.have_made[k] = kit_made_read(path, &T.made[k]);
        MadeGen++;
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
    // the move watched kept (made again as it's changed), else its breathing
    int was = T.anim;
    T.anim = T.have_made[T.kind] ? breathe_of(&T.made[T.kind]) : 0;
    for (int a = 0; T.have_made[T.kind] && a < T.made[T.kind].nanims; a++) {
        T.anim = SDL_strcasecmp(T.made[T.kind].anims[a].name, T.watching) == 0 ? a : T.anim;
    }
    if (T.anim != was) {
        T.made_t = 0;
    }
    snprintf(T.status, sizeof(T.status), have != 0 ? "Made in %d of %d games"
                                                   : "The game didn't make it (it said %d): does it find its data when you start it yourself?",
             have != 0 ? have : code, PACK_KINDS);
    // a move just made its own said so
    if (T.own_note[0] != 0) {
        snprintf(T.status, sizeof(T.status), "%s", T.own_note);
        T.own_note[0] = 0;
    }
}

// A made model's frame drawn: its faces textured as the PS1 does (the texel times its corner's colour, 128 as it is), clear texels
// left out, semi-transparent ones half over what's there. The game's axes are y down, so it's turned up the tool's way.
void raster_made(const KitMade *m, const int16_t *vs, float cx, float cz, View &v, int PW, int PH, int ya, int yb);

// A ghost of a made model's faces in frame vs (onion skin), in its rows ya to yb: its nearest surface's depth (gd) and light (gl)
void raster_ghost(const KitMade *m, const int16_t *vs, float cx, float cz, const View &v, int PW, int PH, int ya, int yb, float *gd, float *gl) {
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);

    for (int t = 0; t < m->nfaces; t++) {
        const KitMadeFace *f = &m->faces[t];
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
        float ln = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]), lit = 0.55f + 0.45f * (ln > 0 ? fabsf(n[0] * s + n[2] * c) / ln : 1);
        fill_tri(sx, sy, sz, gd, PW, PH, ya, yb, [&](int at, float, float, float, float z) {
            gd[at] = z;
            gl[at] = lit;
        });
    }
}

// With what the game draws beside it (rides, still: their one frame), fitted to them all; its frame's vertices from live if given (made
// again with its adjustments, adjusted_frame)
// The floor under a game's model, as Blender's (the user's word, 2026-11-22: "a direction grid like blender"): the ground at ground (up
// from the game's y 0, as the picture's y is) in squares of 50 of the game's units, fading out from its middle (the game's x 0, z 0),
// its line across red (Blender's X, the hero's left and right) and its line front to back green (Y), drawn where the model doesn't hide it
void floor_grid(View &v, int PW, int PH, float cx, float cz, float ground) {
    const int N = 10;
    const float Step = 50, Far = N * Step;

    // a line on the ground, from the game's (x0, z0) to (x1, z1), wide pixels wide (as the picture's made bigger on a bigger screen)
    auto line = [&](float x0, float z0, float x1, float z1, int r, int g, int b, float strength, int wide) {
        float p0[3] = { x0 - cx, ground, -z0 - cz }, p1[3] = { x1 - cx, ground, -z1 - cz }, ax, ay, az, bx, by, bz;
        to_screen(v, p0, ax, ay, az);
        to_screen(v, p1, bx, by, bz);
        bool across = fabsf(bx - ax) >= fabsf(by - ay); // a step a column, its width down it, or a step a row, its width along it
        int n = (int)fminf(8192, fmaxf(fabsf(bx - ax), fabsf(by - ay))) + 1;
        for (int k = 0; k <= n; k++) {
            float t = (float)k / n, gx = x0 + (x1 - x0) * t, gz = z0 + (z1 - z0) * t, z = az + (bz - az) * t;
            float al = strength * (1 - (gx * gx + gz * gz) / (Far * Far));
            int x0p = (int)(ax + (bx - ax) * t) - (across ? 0 : wide / 2), y0p = (int)(ay + (by - ay) * t) - (across ? wide / 2 : 0);
            for (int w = 0; w < wide && al > 0; w++) {
                int x = x0p + (across ? 0 : w), y = y0p + (across ? w : 0);
                if (x < 0 || y < 0 || x >= PW || y >= PH || z >= v.depth[y * PW + x]) {
                    continue;
                }
                unsigned was = v.rgba[y * PW + x];
                unsigned R = (unsigned)((was & 255) * (1 - al) + r * al), G = (unsigned)((was >> 8 & 255) * (1 - al) + g * al);
                unsigned B = (unsigned)((was >> 16 & 255) * (1 - al) + b * al);
                v.rgba[y * PW + x] = 0xFF000000u | B << 16 | G << 8 | R;
            }
        }
    };
    int wide = PW / 500 > 1 ? PW / 500 : 1;
    for (int i = -N; i <= N; i++) {
        if (i != 0) {
            line(i * Step, -Far, i * Step, Far, 160, 160, 172, 0.55f, wide);
            line(-Far, i * Step, Far, i * Step, 160, 160, 172, 0.55f, wide);
        }
    }
    line(-Far, 0, Far, 0, 240, 80, 80, 1, wide + 1);
    line(0, -Far, 0, Far, 130, 220, 80, 1, wide + 1);
}

// (ghost_before and ghost_after: onion skin's frames, as ghosts where they stick out past it, blue and orange)
void draw_made(const KitMade *m, int anim, int frame, View &v, SDL_Renderer *ren, const KitMade *rides = nullptr, int nrides = 0,
               const int16_t *live = nullptr, const int16_t *ghost_before = nullptr, const int16_t *ghost_after = nullptr) {
    const int PW = v.w > 0 ? v.w : PrevW, PH = v.h > 0 ? v.h : PrevH;

    if (v.rgba == nullptr) {
        v.rgba = new unsigned[PW * PH];
        v.depth = new float[PW * PH];
        v.tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, PW, PH);
    }
    const KitMadeAnim *a = m->nanims > 0 ? &m->anims[anim < m->nanims ? anim : 0] : nullptr;
    const int16_t *vs = a == nullptr ? nullptr : live != nullptr ? live : a->verts + (size_t)(frame % a->frames) * m->nvert * 3;
    DrawnFrom now;
    now.add(m), now.add(MadeGen), now.add(anim), now.add(rides), now.add(nrides), now.add(PW), now.add(PH);
    if (vs != nullptr) {
        now.add(vs, sizeof(int16_t) * 3 * (size_t)m->nvert);
    }
    for (const int16_t *g : { ghost_before, ghost_after }) {
        now.add(g != nullptr);
        if (g != nullptr) {
            now.add(g, sizeof(int16_t) * 3 * (size_t)m->nvert);
        }
    }
    if (drawn_same(v, now)) {
        return;
    }
    if (a == nullptr) {
        std::fill(v.rgba, v.rgba + PW * PH, 0xFF3A3434u);
        SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
        return;
    }
    // fitted to every frame of the move shown, so it doesn't jump about, nor leave the picture, as it moves (found once for the move)
    if (v.fit_of != m || v.fit_anim != anim || v.fit_gen != MadeGen) {
        float flo[3] = { 1e9f, 1e9f, 1e9f }, fhi[3] = { -1e9f, -1e9f, -1e9f };
        for (int r = -1; r < nrides; r++) {
            const KitMade *o = r < 0 ? m : &rides[r];
            const KitMadeAnim *fa = r < 0 ? a : &o->anims[0];
            for (int i = 0; o->nanims > 0 && i < o->nvert * (r < 0 ? fa->frames : 1); i++) {
                float q[3] = { (float)fa->verts[i * 3], -(float)fa->verts[i * 3 + 1], -(float)fa->verts[i * 3 + 2] };
                for (int c = 0; c < 3; c++) {
                    flo[c] = fminf(flo[c], q[c]);
                    fhi[c] = fmaxf(fhi[c], q[c]);
                }
            }
        }
        memcpy(v.fit_lo, flo, sizeof(flo));
        memcpy(v.fit_hi, fhi, sizeof(fhi));
        v.fit_of = m, v.fit_anim = anim, v.fit_gen = MadeGen;
    }
    const float *lo = v.fit_lo, *hi = v.fit_hi;
    float h = hi[1] - lo[1], w = sqrtf((hi[0] - lo[0]) * (hi[0] - lo[0]) + (hi[2] - lo[2]) * (hi[2] - lo[2]));
    // as tall as it looks from the view's tilt (from above, as deep as it is), and its middle in the picture's
    float pc = cosf(v.pitch * 3.14159265f / 180), tall = fmaxf(h * fabsf(pc), w * fabsf(sinf(v.pitch * 3.14159265f / 180)));
    v.scale = fminf(PH * 0.8f / (tall > 0 ? tall : 1), PW * 0.8f / (w > 0 ? w : 1));
    float cx = (lo[0] + hi[0]) / 2, cz = (lo[2] + hi[2]) / 2;
    v.cx = cx;
    v.cz = cz;
    v.ox = PW / 2.0f;
    v.oy = PH * 0.5f + (lo[1] + hi[1]) / 2 * pc * v.scale;
    v.zx = PW / 2.0f;
    v.zy = PH / 2.0f;
    static std::vector<float> gd, gl;
    if (ghost_before != nullptr || ghost_after != nullptr) {
        gd.resize((size_t)PW * PH);
        gl.resize((size_t)PW * PH);
    }
    in_bands(PW, PH, [&](int ya, int yb) {
        std::fill(v.rgba + ya * PW, v.rgba + yb * PW, 0xFF3A3434u);
        std::fill(v.depth + ya * PW, v.depth + yb * PW, 1e30f);
        raster_made(m, vs, cx, cz, v, PW, PH, ya, yb);
        for (int r = 0; r < nrides; r++) {
            if (rides[r].nanims > 0) {
                raster_made(&rides[r], rides[r].anims[0].verts, cx, cz, v, PW, PH, ya, yb);
            }
        }
        // onion skin: each ghost blended in where it sticks out past the frame shown, where nothing's drawn (over it, it hid the pose)
        static const float Tint[2][3] = { { 90, 150, 255 }, { 255, 160, 60 } };
        for (int g = 0; g < 2; g++) {
            const int16_t *gv = g == 0 ? ghost_before : ghost_after;
            if (gv == nullptr) {
                continue;
            }
            std::fill(gd.begin() + (size_t)ya * PW, gd.begin() + (size_t)yb * PW, 1e30f);
            raster_ghost(m, gv, cx, cz, v, PW, PH, ya, yb, gd.data(), gl.data());
            for (int p = ya * PW; p < yb * PW; p++) {
                if (gd[p] < 1e29f && v.depth[p] >= 1e29f) {
                    unsigned was = v.rgba[p];
                    float al = 0.5f, l = gl[p];
                    unsigned R = (unsigned)fminf(255, (was & 255) * (1 - al) + Tint[g][0] * l * al);
                    unsigned G = (unsigned)fminf(255, (was >> 8 & 255) * (1 - al) + Tint[g][1] * l * al);
                    unsigned B = (unsigned)fminf(255, (was >> 16 & 255) * (1 - al) + Tint[g][2] * l * al);
                    v.rgba[p] = 0xFF000000u | B << 16 | G << 8 | R;
                }
            }
        }
    });
    if (v.grid) {
        // on the game's ground (its y 0), or where the move's lowest is if that's far from it
        floor_grid(v, PW, PH, cx, cz, fabsf(lo[1]) > h * 0.25f ? lo[1] : 0);
    }
    SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
}

// A made model's faces in frame vs, into the view's picture and depth, in its rows ya to yb (yb not)
void raster_made(const KitMade *m, const int16_t *vs, float cx, float cz, View &v, int PW, int PH, int ya, int yb) {
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);

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
        float ln = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        // its own light's baked in its colours: only a touch of the view's, so its shape reads
        float facing = ln > 0 ? fabsf(n[0] * s + n[2] * c) / ln : 1, lit = 0.8f + 0.25f * facing;
        fill_tri(sx, sy, sz, v.depth, PW, PH, ya, yb, [&](int at, float a0, float b0, float d0, float z) {
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
                    return;
                }
                for (int ch = 0; ch < 3; ch++) {
                    col[ch] = col[ch] * tex[ch] / 128;
                }
            }
            unsigned r = (unsigned)fminf(255, col[0] * lit), g = (unsigned)fminf(255, col[1] * lit), bl = (unsigned)fminf(255, col[2] * lit);
            unsigned was = v.rgba[at];
            if (f->semi) {
                r = (r + (was & 255)) / 2, g = (g + (was >> 8 & 255)) / 2, bl = (bl + (was >> 16 & 255)) / 2;
            } else {
                v.depth[at] = z;
            }
            v.rgba[at] = 0xFF000000u | bl << 16 | g << 8 | r;
        });
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

// A bone of a pose by name, or -1
int pack_bone_index(const KitPose *po, const char *name) {
    for (int b = 0; b < po->nbones; b++) {
        if (SDL_strcasecmp(po->names[b], name) == 0) {
            return b;
        }
    }
    return -1;
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

// The keys picked in the timeline (the user's word, 2026-11-22: "add multi-select for keys in the timeline"): whether key i is one,
// key i alone picked (-1 none), key i picked as well or not any more (Ctrl+click)
bool key_picked(const Tool &T, int i) {
    return std::find(T.keys_sel.begin(), T.keys_sel.end(), i) != T.keys_sel.end();
}

void keys_only(Tool &T, int i) {
    T.key_sel = i;
    T.keys_sel.clear();
    if (i >= 0) {
        T.keys_sel.push_back(i);
    }
}

void keys_toggle(Tool &T, int i) {
    auto at = std::find(T.keys_sel.begin(), T.keys_sel.end(), i);
    if (at != T.keys_sel.end()) {
        T.keys_sel.erase(at);
        T.key_sel = T.keys_sel.empty() ? -1 : T.keys_sel.back();
    } else {
        T.keys_sel.push_back(i);
        T.key_sel = i;
    }
}

// The key after key i on its bone in its move (the first whose frame is after i's last), which i blends into if it blends; or -1
int key_next(const Tool &T, int i) {
    const PackAdjust &a = T.adj[i];
    int next = -1, last = a.until > a.at ? a.until : a.at;

    for (int j = 0; a.at >= 0 && j < (int)T.adj.size(); j++) {
        const PackAdjust &b = T.adj[j];
        if (j != i && b.kind == a.kind && b.at > last && SDL_strcasecmp(b.move, a.move) == 0 && SDL_strcasecmp(b.bone, a.bone) == 0
            && (next < 0 || b.at < T.adj[next].at)) {
            next = j;
        }
    }
    return next;
}

// Every key of this kind's move picked (Ctrl+A)
void keys_all(Tool &T, const char *move) {
    keys_only(T, -1);
    for (int i = 0; i < (int)T.adj.size(); i++) {
        if (pack_adjust_for(&T.adj[i], T.kind, move)) {
            T.keys_sel.push_back(i);
            T.key_sel = i;
        }
    }
}

void adj_snapshot(Tool &T, const char *says, ...);

// The keys picked taken away (the last first, so the others' places hold), a step of the history
void keys_delete(Tool &T) {
    std::vector<int> ks = T.keys_sel;
    if (ks.empty()) {
        return;
    }
    std::sort(ks.begin(), ks.end());
    if (ks.size() == 1) {
        adj_snapshot(T, "Took away %s's key", bone_title(T.adj[ks[0]].bone));
    } else {
        adj_snapshot(T, "Took away %d keys", (int)ks.size());
    }
    for (size_t i = ks.size(); i-- > 0;) {
        T.adj.erase(T.adj.begin() + ks[i]);
    }
    keys_only(T, -1);
}

// Undo and redo: what its adjustments were before each change, and what the change was (printf's way), the history's step for it
void adj_snapshot(Tool &T, const char *says, ...) {
    char line[160];
    va_list ap;

    va_start(ap, says);
    vsnprintf(line, sizeof(line), says, ap);
    va_end(ap);
    T.undo.push_back({ T.adj, line });
    if (T.undo.size() > 300) {
        T.undo.erase(T.undo.begin());
    }
    T.redo.clear();
}

// What the last step was, said again as it goes on (a drag's: where it's got to)
void adj_step_says(Tool &T, const char *says, ...) {
    char line[160];
    va_list ap;

    if (T.undo.empty()) {
        return;
    }
    va_start(ap, says);
    vsnprintf(line, sizeof(line), says, ap);
    va_end(ap);
    T.undo.back().says = line;
}

void adj_undo(Tool &T) {
    if (!T.undo.empty()) {
        T.redo.push_back({ T.adj, T.undo.back().says });
        T.adj = T.undo.back().adj;
        T.undo.pop_back();
        keys_only(T, -1);
    }
}

void adj_redo(Tool &T) {
    if (!T.redo.empty()) {
        T.undo.push_back({ T.adj, T.redo.back().says });
        T.adj = T.redo.back().adj;
        T.redo.pop_back();
        keys_only(T, -1);
    }
}

// The undo history gone to its step at: 0 before them all, as many as are done, more to do them again
void adj_go(Tool &T, int at) {
    while ((int)T.undo.size() > at && !T.undo.empty()) {
        adj_undo(T);
    }
    while ((int)T.undo.size() < at && !T.redo.empty()) {
        adj_redo(T);
    }
}

// A key's last frame: held from at to until, or at alone
int key_last(const PackAdjust &k) {
    return k.until > k.at ? k.until : k.at;
}

int game_of(int kind);
void make_in_game(Tool &T);

// Who a game's moves follow: its own choice there (Prop from, Stands like), else who it's based on
int template_of(const Tool &T, int kind) {
    return T.kind_template[kind] >= 0 ? T.kind_template[kind] : T.template_hero;
}

// A game's move's own copy (made from its template's), or null
BakedMove *baked_of(Tool &T, int kind, const char *move) {
    for (auto &b : T.baked) {
        if (b.kind == kind && SDL_strcasecmp(b.move, move) == 0) {
            return &b;
        }
    }
    return nullptr;
}

// The protection (the user's word, 2026-11-22: "if you make a new animation for something ... it will override the template one ...
// if you change the template that won't override your custom animation"): a move of its template's, the first time it's changed, is
// copied as the game makes it now (its pose before any change, the game's .pose) into a move of its own, a clip (KIND/MOVE) the game
// plays in place of following, which its changes are then made on. Changing who it's based on, or its prop, doesn't change it after.
// Each standard bone's turn from its rest is the game's in glTF's axes (C Q C, C = y and z the other way), made a turn from the bone
// it hangs from; on foot (where a clip's hips are its own, not put on its template's) the hips' move too, in the model's units.
bool bake_move(Tool &T, int kind, int anim) {
    if (kind < 0 || kind >= PACK_KINDS || !T.have_pose[kind] || T.m == nullptr || anim < 0 || anim >= T.poses[kind].nanims) {
        return false;
    }
    const KitPose *po = &T.poses[kind];
    const KitPoseAnim *pa = &po->anims[anim];
    if (pa->own || baked_of(T, kind, pa->name) != nullptr) {
        return false;
    }
    int of[KB_COUNT];
    for (int b = 0; b < KB_COUNT; b++) {
        of[b] = -1;
        for (int j = 0; j < po->nbones; j++) {
            of[b] = SDL_strcasecmp(po->names[j], KitBoneNames[b]) == 0 ? j : of[b];
        }
    }
    if (of[KB_HIPS] < 0) {
        return false;
    }
    // the game's units to the model's: its rest hips to head in each
    float k = 0;
    if (of[KB_HEAD] >= 0 && T.m->bones[KB_HEAD].have) {
        float dg = 0, dm = 0;
        for (int i = 0; i < 3; i++) {
            float a = po->bone[of[KB_HEAD] * 3 + i] - po->bone[of[KB_HIPS] * 3 + i], c = T.m->bones[KB_HEAD].at[i] - T.m->bones[KB_HIPS].at[i];
            dg += a * a, dm += c * c;
        }
        k = dm > 0 ? sqrtf(dg / dm) : 0;
    }
    BakedMove bm;
    bm.kind = kind;
    bm.from = template_of(T, kind);
    SDL_strlcpy(bm.move, pa->name, sizeof(bm.move));
    snprintf(bm.name, sizeof(bm.name), "%s/%s", pack_kind_name(kind), pa->name);
    bm.frames = pa->frames;
    // as many frames as the game made of it: 30 a second, and on foot as many more as its subframes (2, as the tool writes them)
    bm.length = (pa->frames - 1) / (kind == 0 ? 60.0f : 30.0f);
    bm.rot.assign((size_t)pa->frames * KB_COUNT * 4, 0);
    bm.hips.assign((size_t)pa->frames * 3, 0);
    static const float C[3] = { 1, -1, -1 };
    for (int f = 0; f < pa->frames; f++) {
        const float *rec = pa->pose + (size_t)f * (KIT_POSE_HEAD + (size_t)po->nbones * 12), *A = rec + 12;
        float W[KB_COUNT][9];
        for (int b = 0; b < KB_COUNT; b++) {
            float Q[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
            if (of[b] >= 0) {
                memcpy(Q, rec + KIT_POSE_HEAD + of[b] * 12 + 3, sizeof(Q));
            }
            // on foot it's as it's shown (its clip isn't moved with anything), so its move's turn is in it
            if (kind == 0) {
                float AQ[9];
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        AQ[i * 3 + j] = A[i * 3] * Q[j] + A[i * 3 + 1] * Q[3 + j] + A[i * 3 + 2] * Q[6 + j];
                    }
                }
                memcpy(Q, AQ, sizeof(Q));
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    W[b][i * 3 + j] = C[i] * C[j] * Q[i * 3 + j];
                }
            }
        }
        for (int b = 0; b < KB_COUNT; b++) {
            // its turn from the bone it hangs from: parent' W
            float Lm[9], *q = &bm.rot[((size_t)f * KB_COUNT + b) * 4];
            int par = KitBoneParent[b];
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    Lm[i * 3 + j] = par < 0 ? W[b][i * 3 + j]
                                            : W[par][i] * W[b][j] + W[par][3 + i] * W[b][3 + j] + W[par][6 + i] * W[b][6 + j];
                }
            }
            // a turn as a quaternion (x y z w)
            float tr = Lm[0] + Lm[4] + Lm[8];
            if (tr > 0) {
                float r = sqrtf(1 + tr) * 2;
                q[3] = r / 4, q[0] = (Lm[7] - Lm[5]) / r, q[1] = (Lm[2] - Lm[6]) / r, q[2] = (Lm[3] - Lm[1]) / r;
            } else if (Lm[0] > Lm[4] && Lm[0] > Lm[8]) {
                float r = sqrtf(1 + Lm[0] - Lm[4] - Lm[8]) * 2;
                q[3] = (Lm[7] - Lm[5]) / r, q[0] = r / 4, q[1] = (Lm[1] + Lm[3]) / r, q[2] = (Lm[2] + Lm[6]) / r;
            } else if (Lm[4] > Lm[8]) {
                float r = sqrtf(1 + Lm[4] - Lm[0] - Lm[8]) * 2;
                q[3] = (Lm[2] - Lm[6]) / r, q[0] = (Lm[1] + Lm[3]) / r, q[1] = r / 4, q[2] = (Lm[5] + Lm[7]) / r;
            } else {
                float r = sqrtf(1 + Lm[8] - Lm[0] - Lm[4]) * 2;
                q[3] = (Lm[3] - Lm[1]) / r, q[0] = (Lm[2] + Lm[6]) / r, q[1] = (Lm[5] + Lm[7]) / r, q[2] = r / 4;
            }
        }
        // on foot the hips' move from their rest, in the model's units and axes
        if (kind == 0 && k > 0) {
            const float *P = rec + KIT_POSE_HEAD + of[KB_HIPS] * 12, *b = rec + 21;
            for (int i = 0; i < 3; i++) {
                float shown = A[i * 3] * P[0] + A[i * 3 + 1] * P[1] + A[i * 3 + 2] * P[2] + b[i];
                bm.hips[(size_t)f * 3 + i] = C[i] * (shown - po->bone[of[KB_HIPS] * 3 + i]) / k;
            }
        }
    }
    T.baked.push_back(std::move(bm));
    int g = game_of(kind);
    if (g >= 0) {
        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
            T.ownmap[g][m] = SDL_strcasecmp(KitGames[g].moves[m], pa->name) == 0 ? OWN_MADE : T.ownmap[g][m];
        }
    }
    for (size_t i = 0; i < T.foot_back.size(); i++) {
        if (SDL_strcasecmp(T.foot_back[i].c_str(), T.baked.back().name) == 0) {
            T.foot_back.erase(T.foot_back.begin() + (long)i);
            break;
        }
    }
    snprintf(T.own_note, sizeof(T.own_note), "%s is now its own move (from %s)",
             pa->name, HeroNames[T.baked.back().from]);
    make_in_game(T);
    return true;
}

// Before a change to a game's move (or every move there): each of its template's made its own first
void own_before_change(Tool &T, const char *move) {
    if (T.kind < 0 || T.kind >= PACK_KINDS || !T.have_pose[T.kind]) {
        return;
    }
    for (int a = 0; a < T.poses[T.kind].nanims; a++) {
        if (strcmp(move, "*") == 0 || SDL_strcasecmp(T.poses[T.kind].anims[a].name, move) == 0) {
            bake_move(T, T.kind, a);
        }
    }
}

// The adjustment a change of a bone in this move is made to, as T.key_mode says (this frame, the whole move, every move in this game):
// the one there, else a new one if make (none if there are as many as a pack has room for)
PackAdjust *adj_key(Tool &T, const char *move, const char *bone, bool make) {
    int at = T.key_mode == KEY_FRAME ? T.frame : -1;
    const char *mv = T.key_mode == KEY_GAME ? "*" : move;
    PackAdjust *held = nullptr;

    // the key at this frame, else one held over it (changed there, it's changed all the way it's held)
    for (auto &a : T.adj) {
        if (a.kind != T.kind || SDL_strcasecmp(a.move, mv) != 0 || SDL_strcasecmp(a.bone, bone) != 0) {
            continue;
        }
        if (a.at == at) {
            held = &a;
            break;
        }
        if (held == nullptr && at >= 0 && a.at >= 0 && a.at <= at && at <= key_last(a)) {
            held = &a;
        }
    }
    if (held != nullptr) {
        if (make && T.key_mode == KEY_FRAME) {
            held->spread = T.spread;
        }
        return held;
    }
    if (!make) {
        return nullptr;
    }
    own_before_change(T, mv);
    if (T.adj.size() >= PACK_MAX_ADJUSTS) {
        snprintf(T.status, sizeof(T.status), "No room for more keys (%d)", PACK_MAX_ADJUSTS);
        return nullptr;
    }
    PackAdjust a = {};
    a.kind = T.kind;
    SDL_strlcpy(a.move, mv, sizeof(a.move));
    SDL_strlcpy(a.bone, bone, sizeof(a.bone));
    a.at = at;
    a.spread = T.key_mode == KEY_FRAME ? T.spread : 0;
    a.ease = T.ease;
    T.adj.push_back(a);
    return &T.adj.back();
}

// The key the panel shows and changes for a joint in this move. Stopped, the one at the frame shown (made the one picked, lit in the
// timeline); playing, the one picked, whatever frame's shown, so its fade can be changed as it plays and a change goes to it, not to
// whichever key the frames pass (the user's word, 2026-11-22: "if you change the fade while the animation is playing another one it
// will disregard your change"); none if the one picked isn't that joint's.
PackAdjust *panel_key(Tool &T, const char *move, const char *bone) {
    if (T.adj_playing) {
        if (T.key_sel >= 0 && T.key_sel < (int)T.adj.size()) {
            PackAdjust &k = T.adj[T.key_sel];
            if (pack_adjust_for(&k, T.kind, move) && SDL_strcasecmp(k.bone, bone) == 0) {
                return &k;
            }
        }
        return nullptr;
    }
    // (the one picked, if it's that joint's there: another key at the same frame doesn't take its place)
    if (T.key_sel >= 0 && T.key_sel < (int)T.adj.size() && T.key_mode == KEY_FRAME) {
        PackAdjust &k = T.adj[T.key_sel];
        if (k.at >= 0 && k.at <= T.frame && T.frame <= key_last(k) && pack_adjust_for(&k, T.kind, move) && SDL_strcasecmp(k.bone, bone) == 0) {
            return &k;
        }
    }
    PackAdjust *k = adj_key(T, move, bone, false);
    if (k != nullptr && T.keys_sel.size() <= 1) {
        keys_only(T, (int)(k - T.adj.data()));
    }
    return k;
}

const char *other_side(const char *name);

// X-Mirror (the user's word, 2026-11-22: "mirror while editing", as Blender's): key i's other side's key made its mirror image (its
// frames, fade and blend the same; across, and its turns about up and front to back, the other way), the one at was_at (its frame
// before a move, else its frame now) or a new one. Only while X-Mirror's on, and for a joint with another side. (A new key's added at
// the end: pointers into T.adj don't hold over it.)
void mirror_edit(Tool &T, int i, int was_at) {
    if (!T.xmirror || i < 0 || i >= (int)T.adj.size()) {
        return;
    }
    char other[PACK_ADJUST_NAME];
    SDL_strlcpy(other, other_side(T.adj[i].bone), sizeof(other));
    if (other[0] == 0 || pack_bone_index(&T.poses[T.kind], other) < 0) {
        return;
    }
    int m = -1;
    for (int j = 0; j < (int)T.adj.size(); j++) {
        const PackAdjust &x = T.adj[j];
        if (j != i && x.kind == T.adj[i].kind && x.at == was_at && SDL_strcasecmp(x.move, T.adj[i].move) == 0 && SDL_strcasecmp(x.bone, other) == 0) {
            m = j;
        }
    }
    PackAdjust x = T.adj[i];
    SDL_strlcpy(x.bone, other, sizeof(x.bone));
    x.turn[1] = -x.turn[1], x.turn[2] = -x.turn[2];
    x.reach[0] = -x.reach[0];
    if (m >= 0) {
        T.adj[m] = x;
    } else if (T.adj.size() < PACK_MAX_ADJUSTS) {
        T.adj.push_back(x);
    }
}

// mirror_edit for each of keys (their places taken first, as a key it adds moves T.adj); keys then no use
void mirror_all(Tool &T, std::vector<PackAdjust *> &keys) {
    std::vector<int> at;
    for (PackAdjust *x : keys) {
        at.push_back((int)(x - T.adj.data()));
    }
    for (int i : at) {
        mirror_edit(T, i, T.adj[i].at);
    }
    keys.clear();
}

// The keys picked (else the picked joint's at the frame shown) copied, to paste (the user's word, 2026-11-22: "add keyframe copy and
// paste in the timeline")
void key_copy(Tool &T) {
    const KitMade *mk = &T.made[T.kind];
    const KitPose *po = &T.poses[T.kind];
    std::vector<int> ks = T.keys_sel;

    if (ks.empty() && T.sel >= 0 && T.sel < po->nbones && mk->nanims > 0) {
        const PackAdjust *k = adj_key(T, mk->anims[T.anim < mk->nanims ? T.anim : 0].name, po->names[T.sel], false);
        if (k != nullptr) {
            ks.push_back((int)(k - T.adj.data()));
        }
    }
    if (ks.empty()) {
        snprintf(T.status, sizeof(T.status), "Pick a key to copy");
        return;
    }
    T.clip.clear();
    for (int i : ks) {
        T.clip.push_back(T.adj[i]);
    }
    if (ks.size() == 1) {
        snprintf(T.status, sizeof(T.status), "Copied %s's key", bone_title(T.clip[0].bone));
    } else {
        snprintf(T.status, sizeof(T.status), "Copied %d keys", (int)ks.size());
    }
}

// The keys copied pasted at frame (the first of them there, the rest as far after it as they were), each with its change, its fade
// and the frames it's held over (a key to the whole move, or every move here, as that). Copied from one joint, onto joint b; from
// several, each onto its own. Mirrored, each its other side's way (as Mirror makes it), and from several, each onto its other side
// (its left ear's onto its right). One there already (at that frame, or to the whole move) is replaced; the pasted ones are picked.
void key_paste(Tool &T, int b, int frame, bool mirrored) {
    const KitMade *mk = &T.made[T.kind];
    const KitPose *po = &T.poses[T.kind];

    if (T.clip.empty() || mk->nanims == 0) {
        snprintf(T.status, sizeof(T.status), "Nothing copied");
        return;
    }
    bool one = true;
    for (auto &c : T.clip) {
        one &= SDL_strcasecmp(c.bone, T.clip[0].bone) == 0;
    }
    if (one && (b < 0 || b >= po->nbones)) {
        snprintf(T.status, sizeof(T.status), "Pick a joint to paste onto");
        return;
    }
    int a = T.anim < mk->nanims ? T.anim : 0, frames = mk->anims[a].frames, base = -1;
    for (auto &c : T.clip) {
        base = c.at >= 0 && (base < 0 || c.at < base) ? c.at : base;
    }
    // each as it's to be
    std::vector<PackAdjust> put;
    for (auto c : T.clip) {
        char to[PACK_ADJUST_NAME];
        SDL_strlcpy(to, one ? po->names[b] : c.bone, sizeof(to));
        if (!one && mirrored && other_side(c.bone)[0] != 0) {
            SDL_strlcpy(to, other_side(c.bone), sizeof(to));
        }
        if (pack_bone_index(po, to) < 0) {
            continue;
        }
        c.kind = T.kind;
        if (strcmp(c.move, "*") != 0) {
            SDL_strlcpy(c.move, mk->anims[a].name, sizeof(c.move));
        }
        SDL_strlcpy(c.bone, to, sizeof(c.bone));
        if (c.at >= 0) {
            int len = key_last(c) - c.at;
            c.at = std::max(0, std::min(frames - 1, frame + c.at - base));
            c.until = std::min(frames - 1, c.at + len);
        }
        if (mirrored) {
            c.turn[1] = -c.turn[1], c.turn[2] = -c.turn[2];
            c.reach[0] = -c.reach[0];
        }
        put.push_back(c);
    }
    // where each goes: over one there, or after the rest
    std::vector<int> there;
    int adding = 0;
    for (auto &c : put) {
        int at = -1;
        for (int i = 0; i < (int)T.adj.size(); i++) {
            const PackAdjust &x = T.adj[i];
            at = x.kind == c.kind && x.at == c.at && SDL_strcasecmp(x.move, c.move) == 0 && SDL_strcasecmp(x.bone, c.bone) == 0 ? i : at;
        }
        there.push_back(at);
        adding += at < 0;
    }
    if (put.empty() || T.adj.size() + (size_t)adding > PACK_MAX_ADJUSTS) {
        snprintf(T.status, sizeof(T.status), put.empty() ? "Nothing to paste onto here" : "No room for more keys (%d)", PACK_MAX_ADJUSTS);
        return;
    }
    char at[40] = "", from[64];
    SDL_strlcpy(from, bone_title(T.clip[0].bone), sizeof(from));
    if (put[0].at >= 0) {
        snprintf(at, sizeof(at), " at frame %d", frame);
    }
    if (put.size() > 1) {
        adj_snapshot(T, "Pasted %d keys%s%s", (int)put.size(), at, mirrored ? ", mirrored" : "");
    } else if (SDL_strcasecmp(T.clip[0].bone, put[0].bone) == 0) {
        adj_snapshot(T, "Pasted %s's key%s%s", from, at, mirrored ? ", mirrored" : "");
    } else {
        adj_snapshot(T, "Pasted %s's key onto %s%s%s", from, bone_title(put[0].bone), at, mirrored ? ", mirrored" : "");
    }
    own_before_change(T, put[0].move);
    keys_only(T, -1);
    for (size_t i = 0; i < put.size(); i++) {
        int to = there[i];
        if (to >= 0) {
            T.adj[to] = put[i];
        } else {
            T.adj.push_back(put[i]);
            to = (int)T.adj.size() - 1;
        }
        T.keys_sel.push_back(to);
        T.key_sel = to;
    }
    T.sel = pack_bone_index(po, put.back().bone);
    T.frame = put[0].at >= 0 ? std::max(0, std::min(frames - 1, frame)) : T.frame;
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

// The joints over the picture of a game's model while adjusting it (the picture at im, shown at disp of its size), each a dot, picked by
// clicking, and on the one picked a gizmo, as Blender has (the user's word, 2026-11-22: "a proper gizmo tool like blender has so we can
// rotate, move and scale"): in the hero's own axes (red across, green up and down, blue front to back), rings to turn it about each and
// a white ring about the line towards whoever's looking (GIZMO_TURN, R), arrows to move it along each (GIZMO_MOVE, G), and handles to
// make it bigger along each (GIZMO_SIZE, S). Its middle, dragged: turned to point at the mouse, moved across the view, or bigger all over.
enum { H_NONE, H_TURN, H_TURN_VIEW = H_TURN + 3, H_TURN_FREE, H_MOVE, H_MOVE_FREE = H_MOVE + 3, H_SIZE, H_SIZE_ALL = H_SIZE + 3 };

// A direction in the game's axes as the view shows it: x right, y up, z towards whoever's looking (to_screen's turn and tilt, no more)
void view_turn(const View &v, const float g[3], float out[3]) {
    float p0 = g[0], p1 = -g[1], p2 = -g[2];
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
    float pc = cosf(v.pitch * 3.14159265f / 180), ps = sinf(v.pitch * 3.14159265f / 180);
    float x = p0 * c - p2 * s, z = p0 * s + p2 * c, y = p1;

    out[0] = x;
    out[1] = y * pc - z * ps;
    out[2] = y * ps + z * pc;
}

// The hero's own axis i (as it stands: x to its left, y down, z to its back) in the game's axes, as the frame shows it (A trunk e_i)
void hero_axis(const Posed &p, int i, float out[3]) {
    float t[3] = { p.trunk[i], p.trunk[3 + i], p.trunk[6 + i] };

    for (int r = 0; r < 3; r++) {
        out[r] = p.A[r * 3] * t[0] + p.A[r * 3 + 1] * t[1] + p.A[r * 3 + 2] * t[2];
    }
}

// The distance from a point to a segment, on the screen
float seg_px(ImVec2 m, ImVec2 a, ImVec2 b) {
    float dx = b.x - a.x, dy = b.y - a.y, l = dx * dx + dy * dy, t = l > 0 ? ((m.x - a.x) * dx + (m.y - a.y) * dy) / l : 0;
    t = t < 0 ? 0 : t > 1 ? 1 : t;
    return sqrtf((m.x - a.x - dx * t) * (m.x - a.x - dx * t) + (m.y - a.y - dy * t) * (m.y - a.y - dy * t));
}

// The view turned to look from one side (as Blender's keypad or its axes in a corner): 0 its left (X), 1 its back (Y), 2 above (Z),
// 3 to 5 the other sides (its right, its front, below), its front at the bottom from above or below
void view_from(View &v, float &yaw, int side) {
    static const float Yaw[6] = { 90, 180, 0, -90, 0, 0 }, Pitch[6] = { 0, 0, 90, 0, 0, -90 };

    yaw = Yaw[side];
    v.pitch = Pitch[side];
}

// The view's axes in the picture's top right corner, as Blender's (the user's word, 2026-11-22: "a direction grid like blender"): X red
// to the hero's left, Y green to its back (it faces -Y, as a character does in Blender's front view), Z blue up, each lettered at its
// end and plain at its other. Clicking one looks from there (view_from). True if the mouse is over it, so a click there is its.
bool view_axes(View &v, float &yaw, ImVec2 im, float w, bool hovered) {
    static const char *const Letter[3] = { "X", "Y", "Z" };
    static const char *const Says[6] = { "Left (X)", "Back (Y)", "Top (Z)", "Right (-X)", "Front (-Y)", "Bottom (-Z)" };
    static const ImU32 Col[3] = { IM_COL32(235, 75, 75, 255), IM_COL32(120, 210, 70, 255), IM_COL32(80, 140, 245, 255) };
    static const float Way[3][3] = { { 1, 0, 0 }, { 0, 0, 1 }, { 0, -1, 0 } }; // each in the game's axes
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 c(im.x + w - U(56), im.y + U(56)), m = ImGui::GetIO().MousePos;
    float r = U(38), ball = U(10);
    struct End {
        float x, y, z;
        int side;
    } ends[6];
    for (int i = 0; i < 3; i++) {
        float s[3];
        view_turn(v, Way[i], s);
        ends[i] = { c.x + s[0] * r, c.y - s[1] * r, s[2], i };
        ends[i + 3] = { c.x - s[0] * r, c.y + s[1] * r, -s[2], i + 3 };
    }
    // drawn from the far ones to the near, and the one under the mouse the nearest under it
    std::sort(ends, ends + 6, [](const End &a, const End &b) { return a.z < b.z; });
    bool near = hovered && (m.x - c.x) * (m.x - c.x) + (m.y - c.y) * (m.y - c.y) < (r + ball) * (r + ball);
    int over = -1;
    for (int e = 0; e < 6 && near; e++) {
        if ((m.x - ends[e].x) * (m.x - ends[e].x) + (m.y - ends[e].y) * (m.y - ends[e].y) < ball * ball) {
            over = e;
        }
    }
    if (near) {
        dl->AddCircleFilled(c, r + ball + U(4), IM_COL32(255, 255, 255, 28));
    }
    for (int e = 0; e < 6; e++) {
        const End &d = ends[e];
        int axis = d.side % 3;
        ImVec2 at(d.x, d.y);
        if (d.side < 3) {
            dl->AddLine(c, at, Col[axis], U(2));
            dl->AddCircleFilled(at, ball, Col[axis]);
            ImVec2 ts = ImGui::CalcTextSize(Letter[axis]);
            dl->AddText(ImVec2(at.x - ts.x / 2, at.y - ts.y / 2), IM_COL32(20, 20, 24, 255), Letter[axis]);
        } else {
            dl->AddCircleFilled(at, ball * 0.8f, (Col[axis] & 0x00FFFFFFu) | 0x70000000u);
            dl->AddCircle(at, ball * 0.8f, Col[axis], 0, U(1.5f));
        }
        if (e == over) {
            dl->AddCircle(at, ball + U(2), IM_COL32(255, 255, 255, 255), 0, U(2));
        }
    }
    if (over >= 0) {
        ImGui::SetTooltip("%s", Says[ends[over].side]);
        if (ImGui::IsMouseClicked(0)) {
            view_from(v, yaw, ends[over].side);
        }
    } else if (near) {
        ImGui::SetTooltip("Click an axis to view from it");
    }
    return near;
}

// --- the clipping finder (the user's word, 2026-11-22: clipping, what the animation tools are for) ---

// A part's core: a capsule from a bone's joint to a bone on it (to -1: on past its joint from its parent's), r its radius
struct Capsule {
    int bone, to;
    float r;
};

// The distance from p to the segment a b
float seg_dist(const float p[3], const float a[3], const float b[3]) {
    float ab[3] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] }, ap[3] = { p[0] - a[0], p[1] - a[1], p[2] - a[2] };
    float l = ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2], t = l > 0 ? (ap[0] * ab[0] + ap[1] * ab[1] + ap[2] * ab[2]) / l : 0;
    t = t < 0 ? 0 : t > 1 ? 1 : t;
    float d[3] = { ap[0] - ab[0] * t, ap[1] - ab[1] * t, ap[2] - ab[2] * t };
    return sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
}

// A capsule's ends, from joints J (3 a bone)
void capsule_ends(const KitPose *po, const Capsule &c, const float *J, float A[3], float B[3]) {
    for (int i = 0; i < 3; i++) {
        A[i] = J[c.bone * 3 + i];
        B[i] = c.to >= 0 ? J[c.to * 3 + i] : A[i] + (A[i] - J[po->parent[c.bone] * 3 + i]) * 0.6f;
    }
}

// What a kind's clipping is found with: its parts' cores (from its rest), each vertex's bone (the most of it), how far apart (bones
// between) each two bones are; and for the move it was last found in (its changes then, MadeGen's count), each frame's clipping
// points' count
struct ClipFinder {
    int kind = -1;
    unsigned gen = 0;
    std::vector<Capsule> caps;
    std::vector<int> dom;
    std::vector<unsigned char> gap;
    int anim = -1;
    uint64_t said = 0;
    std::vector<int> count;
};
ClipFinder Clips;

// The cores made for a kind (again if its model's been made again)
void clip_prepare(Tool &T, int kind) {
    const KitPose *po = &T.poses[kind];
    if (Clips.kind == kind && Clips.gen == MadeGen) {
        return;
    }
    Clips.kind = kind, Clips.gen = MadeGen, Clips.anim = -1;
    int nb = po->nbones;
    // bones apart: up from each to where they meet
    std::vector<int> depth(nb, 0);
    for (int b = 0; b < nb; b++) {
        for (int p = po->parent[b]; p >= 0 && depth[b] < 255; p = po->parent[p]) {
            depth[b]++;
        }
    }
    Clips.gap.assign((size_t)nb * nb, 0);
    for (int a = 0; a < nb; a++) {
        for (int b = 0; b < nb; b++) {
            int x = a, y = b, n = 0;
            while (depth[x] > depth[y] && x >= 0) {
                x = po->parent[x], n++;
            }
            while (depth[y] > depth[x] && y >= 0) {
                y = po->parent[y], n++;
            }
            while (x != y && x >= 0 && y >= 0) {
                x = po->parent[x], y = po->parent[y], n += 2;
            }
            Clips.gap[(size_t)a * nb + b] = (unsigned char)std::min(n, 255);
        }
    }
    // each vertex's bone
    Clips.dom.assign(po->nvert, 0);
    for (int v = 0; v < po->nvert; v++) {
        float best = -1;
        for (int j = 0; j < 4; j++) {
            if (po->weight[v * 4 + j] > best) {
                best = po->weight[v * 4 + j], Clips.dom[v] = po->joint[v * 4 + j];
            }
        }
    }
    // the cores: a capsule from each bone along its part, to the bone on it that most hangs from it (the chest's to its neck, not its
    // shoulders: one to a shoulder reached up under the ears), an end bone's on past it; its radius three quarters of the way out to
    // its part's nearest third (well inside it, so parts that only touch don't count)
    std::vector<int> under(nb, 0);
    for (int b = 0; b < nb; b++) {
        for (int p = po->parent[b]; p >= 0; p = po->parent[p]) {
            under[p]++;
        }
    }
    Clips.caps.clear();
    for (int b = 0; b < nb; b++) {
        int to = -1;
        for (int c = 0; c < nb; c++) {
            if (po->parent[c] == b && (to < 0 || under[c] > under[to])) {
                to = c;
            }
        }
        if (to >= 0 || po->parent[b] >= 0) {
            Clips.caps.push_back({ b, to, 0 });
        }
    }
    std::vector<std::vector<float>> near(Clips.caps.size());
    for (int v = 0; v < po->nvert; v++) {
        int best = -1;
        float bd = 1e30f;
        for (size_t c = 0; c < Clips.caps.size(); c++) {
            if (Clips.caps[c].bone != Clips.dom[v]) {
                continue;
            }
            float A[3], B[3];
            capsule_ends(po, Clips.caps[c], po->bone, A, B);
            float d = seg_dist(&po->rest[v * 3], A, B);
            if (d < bd) {
                bd = d, best = (int)c;
            }
        }
        if (best >= 0) {
            near[best].push_back(bd);
        }
    }
    for (size_t c = 0; c < Clips.caps.size(); c++) {
        if (near[c].size() >= 4) {
            std::sort(near[c].begin(), near[c].end());
            Clips.caps[c].r = near[c][near[c].size() * 3 / 10] * 0.75f;
        }
    }
}

// The points clipping in a frame of a kind (its vertices vs, its joints J, both in the game's axes): each vertex inside a core of a
// part more than two bones from its own, and each of what it holds inside one more than two from its hand
void clip_points(Tool &T, int kind, const int16_t *vs, const float *J, std::vector<int> &out) {
    const KitPose *po = &T.poses[kind];
    const KitMade *m = &T.made[kind];
    int nb = po->nbones, nprop = std::min(po->nprop, m->nvert - po->nvert);
    std::vector<float> A(Clips.caps.size() * 3), B(Clips.caps.size() * 3);
    for (size_t c = 0; c < Clips.caps.size(); c++) {
        capsule_ends(po, Clips.caps[c], J, &A[c * 3], &B[c * 3]);
    }
    out.clear();
    for (int v = 0; v < po->nvert + std::max(0, nprop); v++) {
        int own = v < po->nvert ? Clips.dom[v] : po->hand;
        if (own < 0 || own >= nb) {
            continue;
        }
        float p[3] = { (float)vs[v * 3], (float)vs[v * 3 + 1], (float)vs[v * 3 + 2] };
        for (size_t c = 0; c < Clips.caps.size(); c++) {
            const Capsule &k = Clips.caps[c];
            if (k.r <= 0 || Clips.gap[(size_t)own * nb + k.bone] <= 2) {
                continue;
            }
            if (seg_dist(p, &A[c * 3], &B[c * 3]) < k.r) {
                out.push_back(v);
                break;
            }
        }
    }
}

// Each frame's clipping points counted for a kind's move, again only when its changes (or its model) have changed, and not while
// something's being dragged (then the frame shown's alone, clip_points)
void clip_scan(Tool &T, int kind, int anim) {
    if (kind < 0 || !T.have_pose[kind] || !T.have_made[kind] || anim < 0 || anim >= T.made[kind].nanims || T.dragging || T.key_drag) {
        return;
    }
    clip_prepare(T, kind);
    uint64_t said = 1469598103934665603ull;
    for (const PackAdjust &a : T.adj) {
        const unsigned char *b = (const unsigned char *)&a;
        for (size_t i = 0; i < sizeof(a); i++) {
            said = (said ^ b[i]) * 1099511628211ull;
        }
    }
    if (Clips.anim == anim && Clips.said == said && (int)Clips.count.size() == T.made[kind].anims[anim].frames) {
        return;
    }
    Clips.anim = anim, Clips.said = said;
    int frames = T.made[kind].anims[anim].frames;
    Clips.count.assign(frames, 0);
    std::vector<int16_t> vs;
    std::vector<int> pts;
    Posed p;
    for (int f = 0; f < frames; f++) {
        if (adjusted_frame(T, kind, anim, f, vs, &p)) {
            clip_points(T, kind, vs.data(), &p.shown[0][0], pts);
            Clips.count[f] = (int)pts.size();
        }
    }
}

// Over the picture while adjusting: which gizmo it has, Turn (R), Move (G) or Size (S), the one on lit
void gizmo_buttons(Tool &T) {
    static const char *const Says[3] = { "Turn (R)", "Move (G)", "Size (S)" };
    static const char *const Tips[3] = { "Rings turn, the middle aims", "Arrows move, the middle drags", "Handles scale, the middle scales all" };
    for (int i = 0; i < 3; i++) {
        bool on = T.gizmo == i;
        if (on) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        }
        if (ImGui::Button(Says[i], ImVec2(U(90), 0))) {
            T.gizmo = i;
        }
        if (on) {
            ImGui::PopStyleColor();
        }
        ImGui::SetItemTooltip("%s", Tips[i]);
        ImGui::SameLine();
    }
    ImGui::SameLine(0, U(16));
    bool on = T.xmirror;
    if (on) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    }
    if (ImGui::Button("X-Mirror", ImVec2(U(90), 0))) {
        T.xmirror = !T.xmirror;
    }
    if (on) {
        ImGui::PopStyleColor();
    }
    ImGui::SetItemTooltip("Changes copy to the other side, flipped");
    ImGui::SameLine();
    on = T.onion;
    if (on) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    }
    if (ImGui::Button("Onion", ImVec2(U(70), 0))) {
        T.onion = !T.onion;
    }
    if (on) {
        ImGui::PopStyleColor();
    }
    ImGui::SetItemTooltip("Ghosts of the frames before (blue) and after (orange)");
    if (T.onion) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(U(90));
        ImGui::SliderInt("##onion", &T.onion_step, 1, 15, "%d apart");
        ImGui::SetItemTooltip("Frames between the ghosts and the frame shown");
    }
    ImGui::SameLine();
    on = T.clipping;
    if (on) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    }
    if (ImGui::Button("Clipping", ImVec2(U(90), 0))) {
        T.clipping = !T.clipping;
    }
    if (on) {
        ImGui::PopStyleColor();
    }
    ImGui::SetItemTooltip("Points of it inside another part (red), and the frames where (the timeline's red)");
    if (T.clipping && Clips.kind == T.kind && T.frame >= 0 && T.frame < (int)Clips.count.size()) {
        ImGui::SameLine();
        ImGui::TextColored(Clips.count[T.frame] > 0 ? ImVec4(1, 0.4f, 0.4f, 1) : ImVec4(0.5f, 0.8f, 0.5f, 1), "%d", Clips.count[T.frame]);
        ImGui::SetItemTooltip("Points clipping at this frame");
    }
    ImGui::SameLine();
    ImGui::NewLine();
}

void adjust_view(Tool &T, ImVec2 im, float disp, bool hovered) {
    static Posed ps;
    // the hero's own x (across), y (up and down) and z (front to back): Blender's X red, Z blue and Y green
    static const ImU32 Axis[3] = { IM_COL32(235, 75, 75, 255), IM_COL32(80, 140, 245, 255), IM_COL32(120, 210, 70, 255) };
    static const char *const AxisSays[3] = { "X", "Z", "Y" };
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
    // the clipping finder: the frame's points inside another part, red
    if (T.clipping) {
        static std::vector<int> pts;
        clip_prepare(T, T.kind);
        clip_points(T, T.kind, scratch.data(), &ps.shown[0][0], pts);
        for (int pv : pts) {
            float q[3] = { scratch[pv * 3] - v.cx, -(float)scratch[pv * 3 + 1], -(float)scratch[pv * 3 + 2] - v.cz }, x, y, z;
            to_screen(v, q, x, y, z);
            dl->AddCircleFilled(ImVec2(im.x + x * disp, im.y + y * disp), U(2.5f), IM_COL32(255, 50, 50, 230));
        }
    }
    float pp = v.scale * v.zoom * disp, R = U(70);
    // the gizmo's handles on the screen: each axis's direction there (per unit, and as seen), and whether it's towards the viewer. Its
    // axes point as Blender's would on a character facing -Y, and as the panel's values say them: X to its left, Z up, Y to its back
    // (Way: the hero's own y is down)
    static const float Way[3] = { 1, -1, 1 };
    float ax2[3][2], axlen[3], axz[3];
    for (int i = 0; i < 3; i++) {
        float g[3], s[3];
        hero_axis(ps, i, g);
        g[0] *= Way[i], g[1] *= Way[i], g[2] *= Way[i];
        view_turn(v, g, s);
        axlen[i] = sqrtf(s[0] * s[0] + s[1] * s[1]);
        ax2[i][0] = axlen[i] > 1e-4f ? s[0] / axlen[i] : 0;
        ax2[i][1] = axlen[i] > 1e-4f ? -s[1] / axlen[i] : 0;
        axz[i] = s[2];
    }
    ImVec2 c = T.sel >= 0 ? ImVec2(sx[T.sel], sy[T.sel]) : ImVec2(0, 0);
    // a ring about axis i: R about the joint, the circle in the plane of the other two, as seen
    auto ring = [&](int i, ImVec2 *pts, int n) {
        float u[3], w[3], gu[3], gw[3];
        hero_axis(ps, (i + 1) % 3, gu);
        hero_axis(ps, (i + 2) % 3, gw);
        view_turn(v, gu, u);
        view_turn(v, gw, w);
        for (int k = 0; k < n; k++) {
            float t = 6.2831853f * k / n, x = cosf(t) * u[0] + sinf(t) * w[0], y = cosf(t) * u[1] + sinf(t) * w[1];
            pts[k] = ImVec2(c.x + x * R, c.y - y * R);
        }
    };
    // what the mouse is over: a handle of the gizmo, else a joint
    int over = H_NONE, hover = -1;
    if (hovered && T.sel >= 0 && !T.dragging) {
        float near = U(6);
        if (T.gizmo == GIZMO_TURN) {
            ImVec2 pts[48];
            for (int i = 0; i < 3; i++) {
                ring(i, pts, 48);
                for (int k = 0; k < 48; k++) {
                    float d = seg_px(mouse, pts[k], pts[(k + 1) % 48]);
                    if (d < near) {
                        near = d, over = H_TURN + i;
                    }
                }
            }
            float r = sqrtf((mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y));
            if (over == H_NONE && fabsf(r - R * 1.25f) < U(6)) {
                over = H_TURN_VIEW;
            }
        } else {
            for (int i = 0; i < 3; i++) {
                if (axlen[i] < 0.15f) {
                    continue;
                }
                float d = seg_px(mouse, ImVec2(c.x + ax2[i][0] * U(14), c.y + ax2[i][1] * U(14)), ImVec2(c.x + ax2[i][0] * R, c.y + ax2[i][1] * R));
                if (d < near) {
                    near = d, over = (T.gizmo == GIZMO_MOVE ? H_MOVE : H_SIZE) + i;
                }
            }
        }
    }
    float best = U(8) * U(8);
    for (int b = 0; b < po->nbones; b++) {
        float d2 = (mouse.x - sx[b]) * (mouse.x - sx[b]) + (mouse.y - sy[b]) * (mouse.y - sy[b]);
        if (hovered && over == H_NONE && d2 < best) {
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
        if (reach_bone(n)) {
            dl->AddRectFilled(ImVec2(sx[b] - U(4), sy[b] - U(4)), ImVec2(sx[b] + U(4), sy[b] + U(4)), col);
        } else {
            dl->AddCircleFilled(ImVec2(sx[b], sy[b]), b == hover ? U(6) : U(4.5f), col);
        }
    }
    // X-Mirror: the other side's joint ringed
    if (T.sel >= 0 && T.xmirror && other_side(po->names[T.sel])[0] != 0) {
        int ob = pack_bone_index(po, other_side(po->names[T.sel]));
        if (ob >= 0) {
            dl->AddCircle(ImVec2(sx[ob], sy[ob]), U(11), IM_COL32(200, 150, 255, 230), 0, U(2));
        }
    }
    // the gizmo
    if (T.sel >= 0) {
        int lit = T.dragging ? T.handle : over;
        if (T.gizmo == GIZMO_TURN) {
            ImVec2 pts[48];
            dl->AddCircle(c, R * 1.25f, lit == H_TURN_VIEW ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 255, 255, 140), 64,
                          lit == H_TURN_VIEW ? U(3) : U(1.5f));
            for (int i = 0; i < 3; i++) {
                ring(i, pts, 48);
                dl->AddPolyline(pts, 48, Axis[i], ImDrawFlags_Closed, lit == H_TURN + i ? U(3.5f) : U(2));
            }
            dl->AddCircle(c, U(9), IM_COL32(255, 255, 255, 255), 0, U(2));
        } else {
            for (int i = 0; i < 3; i++) {
                if (axlen[i] < 0.15f) {
                    continue;
                }
                ImVec2 e(c.x + ax2[i][0] * R, c.y + ax2[i][1] * R), s(c.x + ax2[i][0] * U(14), c.y + ax2[i][1] * U(14));
                float t = lit == (T.gizmo == GIZMO_MOVE ? H_MOVE : H_SIZE) + i ? U(3.5f) : U(2);
                dl->AddLine(s, e, Axis[i], t);
                if (T.gizmo == GIZMO_MOVE) {
                    ImVec2 nrm(-ax2[i][1], ax2[i][0]);
                    dl->AddTriangleFilled(ImVec2(e.x + ax2[i][0] * U(12), e.y + ax2[i][1] * U(12)), ImVec2(e.x + nrm.x * U(6), e.y + nrm.y * U(6)),
                                          ImVec2(e.x - nrm.x * U(6), e.y - nrm.y * U(6)), Axis[i]);
                } else {
                    dl->AddRectFilled(ImVec2(e.x - U(5), e.y - U(5)), ImVec2(e.x + U(5), e.y + U(5)), Axis[i]);
                }
            }
            bool mid = T.dragging && (T.handle == H_MOVE_FREE || T.handle == H_SIZE_ALL);
            dl->AddCircle(c, U(12), mid ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 255, 255, 170), 0, mid ? U(3) : U(2));
        }
    }
    if (!T.dragging) {
        if (over >= H_TURN && over < H_TURN + 3) {
            ImGui::SetTooltip("%s: turn %s", bone_title(po->names[T.sel]), AxisSays[over - H_TURN]);
        } else if (over == H_TURN_VIEW) {
            ImGui::SetTooltip("%s: turn (view)", bone_title(po->names[T.sel]));
        } else if (over >= H_MOVE && over < H_MOVE + 3) {
            ImGui::SetTooltip("%s: move %s", bone_title(po->names[T.sel]), AxisSays[over - H_MOVE]);
        } else if (over >= H_SIZE && over < H_SIZE + 3) {
            ImGui::SetTooltip("%s: size %s", bone_title(po->names[T.sel]), AxisSays[over - H_SIZE]);
        } else if (hover >= 0) {
            ImGui::SetTooltip("%s", bone_title(po->names[hover]));
        }
    }
    // a click: a handle of the gizmo taken, or a joint picked (its middle taken, if it's the one picked already)
    if (hovered && ImGui::IsMouseClicked(0)) {
        int take = over;
        if (take == H_NONE && hover >= 0 && hover == T.sel) {
            take = T.gizmo == GIZMO_TURN ? H_TURN_FREE : T.gizmo == GIZMO_MOVE ? H_MOVE_FREE : H_SIZE_ALL;
        } else if (take == H_NONE && hover >= 0) {
            T.sel = hover;
        }
        if (take != H_NONE) {
            const char *b = bone_title(po->names[T.sel]);
            if (take >= H_SIZE && take <= H_SIZE_ALL) {
                adj_snapshot(T, "Made %s bigger or smaller", b);
            } else if (take >= H_MOVE && take <= H_MOVE_FREE) {
                adj_snapshot(T, "Moved %s", b);
            } else {
                adj_snapshot(T, "Turned %s", b);
            }
            T.dragging = true;
            T.handle = take;
            T.adj_playing = false;
        }
    }
    if (T.dragging && !ImGui::IsMouseDown(0)) {
        T.dragging = false;
        T.handle = H_NONE;
    }
    if (!T.dragging || T.sel < 0 || (io.MouseDelta.x == 0 && io.MouseDelta.y == 0)) {
        return;
    }
    PackAdjust *k = adj_key(T, mk->anims[a].name, po->names[T.sel], true);
    if (k == nullptr) {
        return;
    }
    keys_only(T, (int)(k - T.adj.data()));
    int h = T.handle;
    float jx = sx[T.sel], jy = sy[T.sel];
    // the angle the mouse went round the joint (radians, as seen: anticlockwise more), from where it was
    auto swept = [&]() {
        float mx = mouse.x - jx, my = mouse.y - jy, px = mx - io.MouseDelta.x, py = my - io.MouseDelta.y;
        if (mx * mx + my * my < U(10) * U(10) || px * px + py * py < U(10) * U(10)) {
            return 0.0f;
        }
        float da = atan2f(-my, mx) - atan2f(-py, px);
        return da > 3.14159265f ? da - 6.2831853f : da < -3.14159265f ? da + 6.2831853f : da;
    };
    if (h >= H_TURN && h < H_TURN + 3) {
        // about its own axis: anticlockwise as seen is its turn's way if the axis is towards whoever's looking; its ring seen almost
        // edge on, as far as the mouse goes along the ring
        int i = h - H_TURN;
        float da = swept() * 180 / 3.14159265f * (axz[i] >= 0 ? 1 : -1);
        if (fabsf(axz[i]) < 0.3f) {
            da = (io.MouseDelta.x * -ax2[i][1] + io.MouseDelta.y * ax2[i][0]) / R * 180 / 3.14159265f;
        }
        k->turn[i] = fmaxf(-360, fminf(360, k->turn[i] + da * Way[i]));
    } else if (h == H_TURN_VIEW || h == H_TURN_FREE) {
        // about the line towards whoever's looking: round as far as the mouse goes (the white ring), or to point at it (its middle:
        // its end, the middle of the joints on it, or on past its joint from the one it's on, turned to the mouse)
        float da = swept();
        if (h == H_TURN_FREE) {
            float ex = 0, ey = 0, mx = mouse.x - jx, my = mouse.y - jy;
            int n = 0;
            for (int b = 0; b < po->nbones; b++) {
                if (po->parent[b] == T.sel) {
                    ex += sx[b], ey += sy[b], n++;
                }
            }
            if (n > 0) {
                ex /= n, ey /= n;
            } else if (po->parent[T.sel] >= 0) {
                ex = jx + (jx - sx[po->parent[T.sel]]) * 0.5f, ey = jy + (jy - sy[po->parent[T.sel]]) * 0.5f;
            }
            if (mx * mx + my * my < U(12) * U(12)) {
                return;
            }
            if ((n > 0 || po->parent[T.sel] >= 0) && (ex - jx) * (ex - jx) + (ey - jy) * (ey - jy) >= U(6) * U(6)) {
                da = atan2f(-my, mx) - atan2f(-(ey - jy), ex - jx);
                da = da > 3.14159265f ? da - 6.2831853f : da < -3.14159265f ? da + 6.2831853f : da;
            }
        }
        float z[3] = { 0, 0, 1 }, g[3], hh[3];
        view_to_game(v, z, g);
        game_to_hero(ps, g, hh);
        for (int i = 0; i < 3; i++) {
            k->turn[i] = fmaxf(-360, fminf(360, k->turn[i] + hh[i] * da * 180 / 3.14159265f));
        }
    } else if (h >= H_MOVE && h < H_MOVE + 3) {
        // along its own axis: as far as the mouse went that way on the screen, in the game's units
        int i = h - H_MOVE;
        float along = io.MouseDelta.x * ax2[i][0] + io.MouseDelta.y * ax2[i][1], per = fmaxf(axlen[i], 0.15f) * pp;
        k->reach[i] = fmaxf(-1000, fminf(1000, k->reach[i] + along / per * Way[i]));
    } else if (h == H_MOVE_FREE) {
        // across the view with the mouse
        float d[3] = { io.MouseDelta.x / pp, -io.MouseDelta.y / pp, 0 }, g[3], hh[3];
        view_to_game(v, d, g);
        game_to_hero(ps, g, hh);
        for (int i = 0; i < 3; i++) {
            k->reach[i] = fmaxf(-1000, fminf(1000, k->reach[i] + hh[i]));
        }
    } else if (h >= H_SIZE && h <= H_SIZE_ALL) {
        // bigger as the mouse goes out along its axis (all over: right or up), by a share of how far
        float along = h == H_SIZE_ALL ? io.MouseDelta.x - io.MouseDelta.y : io.MouseDelta.x * ax2[h - H_SIZE][0] + io.MouseDelta.y * ax2[h - H_SIZE][1];
        float f = 1 + along / U(150);
        for (int i = 0; i < 3; i++) {
            if (h == H_SIZE_ALL || i == h - H_SIZE) {
                k->grow[i] = fmaxf(0.1f, fminf(10, (1 + k->grow[i]) * f)) - 1;
            }
        }
    }
    mirror_edit(T, T.key_sel, T.adj[T.key_sel].at);
}

// Under the picture while adjusting, the keyframes (the user's word, 2026-11-22: "a key frame time line below the animation"): play
// or stop, the keys before and after, and a sheet of the move's frames, a row for each joint changed in it (and the one picked). A
// change at a frame is a diamond; one held over frames (the user's word: "extend it along the entire animation and that will keep it
// big") a bar between two; its fade a band either side; one to the whole move or every move here a long bar. Click or drag the ruler
// to go to a frame. Click a key to pick it and go to it; drag it (a held one by its bar) to move it; drag a held one's end, a picked
// one's grips either side, or Shift and drag one, to hold it over more frames or fewer; right-click it for more (held to the start,
// the end, all through, or its own frame again; taken away), or press Delete. Each is a step of the undo history.
void timeline(Tool &T, const KitMade *mk, int a, float width) {
    const KitPose *po = &T.poses[T.kind];
    int frames = mk->anims[a].frames;
    const char *move = mk->anims[a].name;
    ImGuiIO &io = ImGui::GetIO();

    T.frame = frames > 0 ? (T.frame % frames + frames) % frames : 0;
    // what's picked: this move's keys only
    for (size_t i = 0; i < T.keys_sel.size();) {
        int k = T.keys_sel[i];
        if (k < 0 || k >= (int)T.adj.size() || !pack_adjust_for(&T.adj[k], T.kind, move)) {
            T.keys_sel.erase(T.keys_sel.begin() + (long)i);
        } else {
            i++;
        }
    }
    if (!key_picked(T, T.key_sel)) {
        T.key_sel = T.keys_sel.empty() ? -1 : T.keys_sel.back();
    }
    if (ImGui::Button(T.adj_playing ? "Stop" : "Play", ImVec2(U(50), 0))) {
        T.adj_playing = !T.adj_playing;
    }
    ImGui::SetItemTooltip("Space");
    std::vector<int> keys;
    for (auto &k : T.adj) {
        if (k.kind == T.kind && k.at >= 0 && SDL_strcasecmp(k.move, move) == 0) {
            keys.push_back(k.at);
            keys.push_back(key_last(k));
        }
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##before", ImGuiDir_Left)) {
        int to = -1;
        for (int f : keys) {
            to = f < T.frame && f > to ? f : to;
        }
        T.frame = to >= 0 ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SetItemTooltip("Previous key");
    ImGui::SameLine();
    if (ImGui::ArrowButton("##after", ImGuiDir_Right)) {
        int to = frames;
        for (int f : keys) {
            to = f > T.frame && f < to ? f : to;
        }
        T.frame = to < frames ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SetItemTooltip("Next key");
    ImGui::SameLine();
    ImGui::Text("%d / %d", T.frame, frames);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(U(110));
    ImGui::SliderFloat("##speed", &T.made_speed, 0, 2, "%.2fx");
    ImGui::SetItemTooltip("Speed");

    // its rows: each joint with a change in this move (or every move here), and the one picked
    std::vector<int> rows;
    for (int b = 0; b < po->nbones; b++) {
        bool has = b == T.sel;
        for (auto &k : T.adj) {
            has |= pack_adjust_for(&k, T.kind, move) && SDL_strcasecmp(k.bone, po->names[b]) == 0;
        }
        if (has) {
            rows.push_back(b);
        }
    }
    const float label = U(130), ruler = U(22), row = U(20), strip = T.clipping ? U(8) : 0;
    float h = ruler + strip + row * (rows.empty() ? 1 : (float)rows.size());
    if (T.clipping) {
        clip_scan(T, T.kind, a);
    }
    ImVec2 o = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton("##sheet", ImVec2(width, h), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    bool over = ImGui::IsItemHovered();
    ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY); // (its wheel zooms it, not the window's scroll)
    ImDrawList *dl = ImGui::GetWindowDrawList();
    // zoomed in on part of the move (the user's word, 2026-11-22: zoom and snapping): the wheel zooms about the mouse, Shift and the
    // wheel (or a sideways wheel) scroll, Home shows all of it again; playing, it follows the frame shown
    float x0 = o.x + label, span = width - label, most = std::max(1.0f, frames / 4.0f);
    T.tl_zoom = std::max(1.0f, std::min(most, T.tl_zoom));
    float shown = frames / T.tl_zoom;
    if (over && io.MouseWheel != 0 && !io.KeyShift && io.MousePos.x >= x0) {
        float at = T.tl_first + (io.MousePos.x - x0) / span * shown;
        T.tl_zoom = std::max(1.0f, std::min(most, T.tl_zoom * powf(1.25f, io.MouseWheel)));
        shown = frames / T.tl_zoom;
        T.tl_first = at - (io.MousePos.x - x0) / span * shown;
    }
    float side = io.MouseWheelH + (io.KeyShift ? io.MouseWheel : 0);
    if (over && side != 0) {
        T.tl_first -= side * shown * 0.1f;
    }
    if (over && ImGui::IsKeyPressed(ImGuiKey_Home, false)) {
        T.tl_zoom = 1, shown = (float)frames;
    }
    if (T.adj_playing && (T.frame < T.tl_first || T.frame >= T.tl_first + shown)) {
        T.tl_first = T.frame - shown * 0.1f;
    }
    T.tl_first = std::max(0.0f, std::min(frames - shown, T.tl_first));
    float cw = frames > 0 ? span / shown : 1;
    auto fx = [&](float f) { return x0 + (f - T.tl_first + 0.5f) * cw; };
    auto xf = [&](float x) { int f = (int)floorf((x - x0) / cw + T.tl_first); return f < 0 ? 0 : f >= frames ? frames - 1 : f; };
    dl->AddRectFilled(o, ImVec2(o.x + width, o.y + h), IM_COL32(30, 30, 36, 255));
    dl->AddRectFilled(ImVec2(x0, o.y), ImVec2(o.x + width, o.y + ruler), IM_COL32(45, 45, 54, 255));
    // the ruler: a tick each frame, a number every so many
    int every = cw >= U(28) ? 1 : cw >= U(12) ? 5 : 10;
    dl->PushClipRect(ImVec2(x0, o.y), ImVec2(o.x + width, o.y + h), true);
    for (int f = std::max(0, (int)T.tl_first - 1); f < std::min(frames, (int)(T.tl_first + shown) + 2); f++) {
        float x = fx((float)f);
        bool big = f % every == 0;
        dl->AddLine(ImVec2(x, o.y + ruler - (big ? U(8) : U(4))), ImVec2(x, o.y + ruler), IM_COL32(150, 150, 160, 255));
        char n[8];
        snprintf(n, sizeof(n), "%d", f);
        if (big && x + U(2) + ImGui::CalcTextSize(n).x <= o.x + width) {
            dl->AddText(ImVec2(x + U(2), o.y + U(1)), IM_COL32(170, 170, 180, 255), n);
        }
    }
    if (T.clipping && Clips.kind == T.kind && Clips.anim == a) {
        // the clipping finder: each frame's clipping, redder for more
        int most = 1;
        for (int c : Clips.count) {
            most = std::max(most, c);
        }
        for (int g = 0; g < frames && g < (int)Clips.count.size(); g++) {
            if (Clips.count[g] > 0) {
                int al = 90 + 165 * Clips.count[g] / most;
                dl->AddRectFilled(ImVec2(fx((float)g) - cw / 2, o.y + ruler + U(1)), ImVec2(fx((float)g) + cw / 2, o.y + ruler + strip - U(1)),
                                  IM_COL32(255, 60, 60, al));
            }
        }
    }
    if (T.tl_zoom > 1) {
        // the part of the move shown, under the ruler
        dl->AddRectFilled(ImVec2(x0 + T.tl_first / frames * span, o.y + ruler - U(3)), ImVec2(x0 + (T.tl_first + shown) / frames * span, o.y + ruler),
                          IM_COL32(110, 160, 255, 200));
    }
    dl->PopClipRect();
    // the rows: a joint's name, its keys, and each key's parts the mouse can take (later ones over earlier)
    struct Hit {
        int key, part;
        ImVec2 lo, hi;
    };
    std::vector<Hit> hits;
    const float s = U(6), grip = U(6);
    for (size_t r = 0; r < rows.size(); r++) {
        int b = rows[r];
        float y = o.y + ruler + strip + row * r, my = y + row / 2;
        if (r % 2 == 1) {
            dl->AddRectFilled(ImVec2(o.x, y), ImVec2(o.x + width, y + row), IM_COL32(36, 36, 44, 255));
        }
        if (b == T.sel) {
            dl->AddRectFilled(ImVec2(o.x, y), ImVec2(o.x + label - U(4), y + row), IM_COL32(60, 80, 120, 255));
        }
        dl->AddText(ImVec2(o.x + U(4), y + U(3)), IM_COL32(220, 220, 225, 255), bone_title(po->names[b]));
        dl->PushClipRect(ImVec2(x0, y), ImVec2(o.x + width, y + row), true);
        for (int i = 0; i < (int)T.adj.size(); i++) {
            const PackAdjust &k = T.adj[i];
            if (!pack_adjust_for(&k, T.kind, move) || SDL_strcasecmp(k.bone, po->names[b]) != 0) {
                continue;
            }
            bool picked = key_picked(T, i);
            if (k.at < 0) {
                // the whole move (green) or every move here (blue): a bar
                ImU32 col = strcmp(k.move, "*") == 0 ? IM_COL32(90, 140, 230, picked ? 255 : 150) : IM_COL32(110, 200, 110, picked ? 255 : 150);
                ImVec2 lo(x0, y + row * 0.35f), hi(o.x + width - U(2), y + row * 0.65f);
                dl->AddRectFilled(lo, hi, col, U(3));
                hits.push_back({ i, KEY_BODY, lo, hi });
                continue;
            }
            int last = key_last(k);
            float xa = fx((float)k.at), xb = fx((float)last);
            ImU32 col = picked ? IM_COL32(255, 255, 255, 255) : IM_COL32(255, 200, 60, 255);
            {
                // how much of it there is each frame as the game makes it (its fade, its blend into the next or from the one before),
                // up from the row's foot, where there's any
                std::vector<ImVec2> pts;
                float foot = y + row - U(2);
                auto flush = [&]() {
                    for (size_t p = 0; p + 1 < pts.size(); p++) {
                        dl->AddQuadFilled(ImVec2(pts[p].x, foot), pts[p], pts[p + 1], ImVec2(pts[p + 1].x, foot), IM_COL32(255, 200, 60, 35));
                    }
                    if (pts.size() > 1) {
                        dl->AddPolyline(pts.data(), (int)pts.size(), IM_COL32(255, 200, 60, 140), 0, U(1.5f));
                    }
                    pts.clear();
                };
                for (int g = 0; g < frames; g++) {
                    float sh = pack_adjust_share_among(T.adj.data(), (int)T.adj.size(), i, g);
                    if (sh != 0 || (g + 1 < frames && pack_adjust_share_among(T.adj.data(), (int)T.adj.size(), i, g + 1) != 0) || !pts.empty()) {
                        pts.push_back(ImVec2(fx((float)g), fmaxf(y + U(1), fminf(y + row, foot - sh * (row - U(5))))));
                    }
                    if (sh == 0 && pts.size() > 1) {
                        flush();
                    }
                }
                flush();
            }
            int to = k.blend ? key_next(T, i) : -1;
            if (to >= 0) {
                // its blend into the next: a line to it
                dl->AddLine(ImVec2(fx((float)last), my), ImVec2(fx((float)T.adj[to].at), my), IM_COL32(120, 190, 255, 200), U(2));
            }
            auto diamond = [&](float x) {
                dl->AddQuadFilled(ImVec2(x, my - s), ImVec2(x + s, my), ImVec2(x, my + s), ImVec2(x - s, my), col);
            };
            if (last > k.at) {
                // held: a bar from its first frame to its last, a diamond each end (the ends held over more frames or fewer)
                dl->AddRectFilled(ImVec2(xa, my - row * 0.2f), ImVec2(xb, my + row * 0.2f), picked ? IM_COL32(255, 255, 255, 170) : IM_COL32(255, 200, 60, 150));
                if (xb - xa > 2 * s) {
                    hits.push_back({ i, KEY_BODY, ImVec2(xa + s, my - s), ImVec2(xb - s, my + s) });
                }
                diamond(xa);
                diamond(xb);
                hits.push_back({ i, KEY_START, ImVec2(xa - s, my - s), ImVec2(xa + s, my + s) });
                hits.push_back({ i, KEY_END, ImVec2(xb - s, my - s), ImVec2(xb + s, my + s) });
            } else {
                diamond(xa);
                hits.push_back({ i, KEY_BODY, ImVec2(xa - s, my - s), ImVec2(xa + s, my + s) });
            }
            if (picked && T.keys_sel.size() == 1) {
                // grips either side, to hold it over more frames
                ImVec2 g0(xa - s - U(3) - grip, my - row * 0.32f), g1(xa - s - U(3), my + row * 0.32f);
                ImVec2 h0(xb + s + U(3), my - row * 0.32f), h1(xb + s + U(3) + grip, my + row * 0.32f);
                dl->AddRectFilled(g0, g1, IM_COL32(255, 255, 255, 210), U(2));
                dl->AddRectFilled(h0, h1, IM_COL32(255, 255, 255, 210), U(2));
                hits.push_back({ i, KEY_START, g0, g1 });
                hits.push_back({ i, KEY_END, h0, h1 });
            }
        }
        dl->PopClipRect();
    }
    if (rows.empty()) {
        dl->AddText(ImVec2(o.x + U(4), o.y + ruler + strip + U(3)), IM_COL32(150, 150, 160, 255), "Click a joint");
    }
    // the frame shown, across it all
    float px = fx((float)T.frame);
    dl->PushClipRect(ImVec2(x0, o.y), ImVec2(o.x + width, o.y + h), true);
    dl->AddRectFilled(ImVec2(px - cw / 2, o.y), ImVec2(px + cw / 2, o.y + ruler), IM_COL32(70, 110, 190, 255));
    dl->AddLine(ImVec2(px, o.y), ImVec2(px, o.y + h), IM_COL32(110, 160, 255, 255), U(2));
    if (T.key_drag && T.snapped >= 0) {
        // the frame a dragged key's snapped to
        dl->AddLine(ImVec2(fx((float)T.snapped), o.y), ImVec2(fx((float)T.snapped), o.y + h), IM_COL32(255, 255, 255, 150), U(1));
    }
    dl->PopClipRect();
    // what's under the mouse, and what a click or a drag does
    int under = -1, part = KEY_BODY;
    for (auto &ht : hits) {
        if (io.MousePos.x >= x0 && io.MousePos.x >= ht.lo.x - U(2) && io.MousePos.x <= ht.hi.x + U(2) && io.MousePos.y >= ht.lo.y - U(2)
            && io.MousePos.y <= ht.hi.y + U(2)) {
            under = ht.key, part = ht.part;
        }
    }
    int at_row = (int)floorf((io.MousePos.y - o.y - ruler - strip) / row);
    if (over && ImGui::IsMouseClicked(0)) {
        bool playing = T.adj_playing;
        T.adj_playing = false;
        if (under >= 0 && io.KeyCtrl) {
            // Ctrl: picked as well, or not any more
            keys_toggle(T, under);
            T.adj_playing = playing;
        } else if (under >= 0) {
            // a key picked: its joint, its frame, and the change it makes; taken to drag (one of several picked: all of them, or it
            // alone if it's let go unmoved). While it plays, picked and left playing (to change it, its fade, as it plays), not moved
            const PackAdjust &k = T.adj[under];
            bool group = key_picked(T, under) && T.keys_sel.size() > 1 && part == KEY_BODY && !io.KeyShift;
            if (group) {
                T.key_sel = under;
            } else {
                keys_only(T, under);
            }
            T.sel = pack_bone_index(po, k.bone);
            T.key_mode = k.at >= 0 ? KEY_FRAME : strcmp(k.move, "*") == 0 ? KEY_GAME : KEY_MOVE;
            if (k.at >= 0 && playing) {
                T.spread = k.spread;
                T.ease = k.ease;
                T.adj_playing = true;
            } else if (k.at >= 0) {
                T.spread = k.spread;
                T.ease = k.ease;
                T.key_drag = true;
                T.key_moved = false;
                T.key_pending = group;
                T.key_part = part == KEY_BODY && io.KeyShift ? KEY_EITHER : part;
                T.key_grab = xf(io.MousePos.x);
                T.drag_frame0 = T.frame;
                T.key_from = k.at;
                T.key_to = key_last(k);
                T.drag_orig.clear();
                for (int i : T.keys_sel) {
                    T.drag_orig.push_back({ T.adj[i].at, key_last(T.adj[i]) });
                }
                T.frame = part == KEY_END ? T.key_to : k.at;
            }
        } else if (io.MousePos.x >= x0 && io.MousePos.y < o.y + ruler + strip) {
            T.scrub = true;
        } else if (io.MousePos.x >= x0 && at_row >= 0 && at_row < (int)rows.size()) {
            // an empty place in the rows: a box to pick the keys in (Ctrl: as well as those picked), or nothing picked
            T.box = true;
            T.box_x = io.MousePos.x, T.box_y = io.MousePos.y;
            if (!io.KeyCtrl) {
                keys_only(T, -1);
            }
        } else if (at_row >= 0 && at_row < (int)rows.size()) {
            // a row's name: its joint picked
            T.sel = rows[at_row];
        }
    }
    if (T.box) {
        ImVec2 lo(fminf(T.box_x, io.MousePos.x), fminf(T.box_y, io.MousePos.y)), hi(fmaxf(T.box_x, io.MousePos.x), fmaxf(T.box_y, io.MousePos.y));
        dl->AddRectFilled(lo, hi, IM_COL32(110, 160, 255, 40));
        dl->AddRect(lo, hi, IM_COL32(110, 160, 255, 170));
        if (!ImGui::IsMouseDown(0)) {
            // the keys the box was over picked
            for (auto &ht : hits) {
                if (ht.hi.x >= lo.x && ht.lo.x <= hi.x && ht.hi.y >= lo.y && ht.lo.y <= hi.y && ht.hi.x >= x0 && ht.lo.x <= o.x + width
                    && !key_picked(T, ht.key)) {
                    T.keys_sel.push_back(ht.key);
                    T.key_sel = ht.key;
                }
            }
        }
    }
    if (!ImGui::IsMouseDown(0)) {
        if (T.key_drag && T.key_pending && !T.key_moved) {
            keys_only(T, T.key_sel);
        }
        T.scrub = T.key_drag = T.box = T.key_pending = false;
    }
    if (T.scrub) {
        T.frame = xf(io.MousePos.x);
    }
    // snapping: a dragged key's frame (or end) to another key's first or last, or the frame shown as the drag began, the nearest within
    // a few pixels (Ctrl: none); how far frame g goes to snap, and how far off it was
    T.snapped = -1;
    std::vector<int> snaps;
    if (T.key_drag && !io.KeyCtrl) {
        for (int i = 0; i < (int)T.adj.size(); i++) {
            const PackAdjust &x = T.adj[i];
            if (x.at >= 0 && pack_adjust_for(&x, T.kind, move) && !key_picked(T, i) && i != T.key_sel) {
                snaps.push_back(x.at);
                snaps.push_back(key_last(x));
            }
        }
        snaps.push_back(T.drag_frame0);
    }
    auto snap_off = [&](int g, float *off_px) {
        int by = 0, to = -1;
        float best = U(8) + 1;
        for (int c : snaps) {
            float d = fabsf(fx((float)c) - fx((float)g));
            if (d <= U(8) && d < best) {
                best = d, by = c - g, to = c;
            }
        }
        if (off_px != nullptr) {
            *off_px = best;
        }
        if (to >= 0) {
            T.snapped = to;
        }
        return by;
    };
    // (of two frames, its first and last, the one nearer one to snap to)
    auto snap_pair = [&](int a, int b) {
        float pa, pb;
        int da = snap_off(a, &pa), sa = T.snapped;
        T.snapped = -1;
        int db = snap_off(b, &pb), sb = T.snapped;
        T.snapped = pa <= pb ? sa : sb;
        return pa <= pb ? da : db;
    };
    if (T.key_drag && T.key_sel >= 0 && T.key_sel < (int)T.adj.size() && T.adj[T.key_sel].at >= 0) {
        // moved, or held over more frames or fewer, to the frame under the mouse (a step of the history once it's changed); several
        // picked, all of them moved as far, none past the move's ends
        PackAdjust &k = T.adj[T.key_sel];
        int f = xf(io.MousePos.x), from = T.key_from, to = T.key_to;
        if (T.key_part == KEY_EITHER && f != T.key_grab) {
            T.key_part = f > T.key_grab ? KEY_END : KEY_START;
        }
        if (T.key_part == KEY_BODY && T.keys_sel.size() > 1 && T.drag_orig.size() == T.keys_sel.size()) {
            int lo = frames, hi = 0, n = 0;
            for (size_t j = 0; j < T.keys_sel.size(); j++) {
                if (T.drag_orig[j].first >= 0) {
                    lo = std::min(lo, T.drag_orig[j].first), hi = std::max(hi, T.drag_orig[j].second), n++;
                }
            }
            int d = f - T.key_grab;
            for (size_t j = 0; j < T.keys_sel.size(); j++) {
                if (T.keys_sel[j] == T.key_sel && T.drag_orig[j].first >= 0) {
                    d += snap_pair(T.drag_orig[j].first + d, T.drag_orig[j].second + d);
                }
            }
            d = std::max(-lo, std::min(frames - 1 - hi, d));
            bool changed = false;
            for (size_t j = 0; j < T.keys_sel.size(); j++) {
                const PackAdjust &x = T.adj[T.keys_sel[j]];
                changed |= x.at >= 0 && (x.at != T.drag_orig[j].first + d || key_last(x) != T.drag_orig[j].second + d);
            }
            if (changed) {
                if (!T.key_moved) {
                    adj_snapshot(T, "Moved %d keys", n);
                    T.key_moved = true;
                }
                for (size_t j = 0; j < T.keys_sel.size(); j++) {
                    PackAdjust &x = T.adj[T.keys_sel[j]];
                    if (x.at >= 0) {
                        x.at = T.drag_orig[j].first + d;
                        x.until = T.drag_orig[j].second + d;
                    }
                }
                adj_step_says(T, "Moved %d keys %+d frames", n, d);
            }
            T.frame = f;
        } else {
            if (T.key_part == KEY_BODY) {
                int d = f - T.key_grab;
                d += snap_pair(from + d, to + d);
                d = std::max(-from, std::min(frames - 1 - to, d));
                from += d, to += d;
            } else if (T.key_part == KEY_START) {
                from = std::min(f + snap_off(f, nullptr), to);
            } else if (T.key_part == KEY_END) {
                to = std::max(f + snap_off(f, nullptr), from);
            }
            if (from != k.at || to != key_last(k)) {
                const char *b = bone_title(k.bone);
                if (!T.key_moved) {
                    adj_snapshot(T, "%s's key", b);
                    T.key_moved = true;
                }
                int was = k.at;
                k.at = from;
                k.until = to;
                mirror_edit(T, T.key_sel, was);
                if (T.key_part == KEY_BODY) {
                    to > from ? adj_step_says(T, "Moved %s's key to frames %d to %d", b, from, to) : adj_step_says(T, "Moved %s's key to frame %d", b, from);
                } else {
                    to > from ? adj_step_says(T, "Held %s's key from frame %d to %d", b, from, to) : adj_step_says(T, "Made %s's key frame %d alone", b, from);
                }
            }
            T.frame = T.key_part == KEY_START ? from : T.key_part == KEY_END ? to : f;
        }
    }
    if ((over && under >= 0 && part != KEY_BODY) || (T.key_drag && (T.key_part == KEY_START || T.key_part == KEY_END))) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }
    if (over && T.clipping && io.MousePos.x >= x0 && io.MousePos.y >= o.y + ruler && io.MousePos.y < o.y + ruler + strip
        && Clips.kind == T.kind && Clips.anim == a) {
        int g = xf(io.MousePos.x);
        if (g < (int)Clips.count.size()) {
            ImGui::SetTooltip("Frame %d: %d points clipping", g, Clips.count[g]);
        }
    }
    if (over && under >= 0 && !T.key_drag) {
        const PackAdjust &k = T.adj[under];
        if (k.at >= 0 && part != KEY_BODY) {
            ImGui::SetTooltip("Stretch");
        } else if (k.at >= 0 && key_last(k) > k.at) {
            ImGui::SetTooltip("%s  %d-%d  %s %d%s", bone_title(k.bone), k.at, key_last(k), EaseSays[k.ease], k.spread, k.blend ? "  blend" : "");
        } else if (k.at >= 0) {
            ImGui::SetTooltip("%s  %d  %s %d%s\nShift-drag: stretch   Ctrl-click: pick more", bone_title(k.bone), k.at, EaseSays[k.ease], k.spread,
                              k.blend ? "  blend" : "");
        } else {
            ImGui::SetTooltip("%s  %s", bone_title(k.bone), strcmp(k.move, "*") == 0 ? "all moves" : "whole move");
        }
    }
    if (over && under >= 0 && ImGui::IsMouseClicked(1)) {
        // its menu: for all the keys picked, if it's one of them
        if (key_picked(T, under)) {
            T.key_sel = under;
        } else {
            keys_only(T, under);
        }
        ImGui::OpenPopup("key");
    } else if (over && ImGui::IsMouseClicked(1) && io.MousePos.x >= x0) {
        // an empty place in a joint's row: that joint and frame, to paste there
        if (at_row >= 0 && at_row < (int)rows.size()) {
            T.sel = rows[at_row];
            T.frame = xf(io.MousePos.x);
            T.adj_playing = false;
            ImGui::OpenPopup("here");
        }
    }
    if (ImGui::BeginPopup("here")) {
        ImGui::TextDisabled("%s  %d", T.sel >= 0 ? bone_title(po->names[T.sel]) : "", T.frame);
        if (ImGui::MenuItem("Paste here", "Ctrl+V", false, !T.clip.empty())) {
            key_paste(T, T.sel, T.frame, false);
        }
        if (ImGui::MenuItem("Paste mirrored", "Ctrl+Shift+V", false, !T.clip.empty())) {
            key_paste(T, T.sel, T.frame, true);
        }
        if (T.clip.empty()) {
            ImGui::TextDisabled("(nothing copied)");
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Select all", "Ctrl+A")) {
            keys_all(T, move);
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("key")) {
        if (T.key_sel >= 0 && T.key_sel < (int)T.adj.size()) {
            // the keys it's for: those picked (it among them), the frame ones of them for what only they have
            std::vector<int> ks = T.keys_sel;
            std::vector<PackAdjust *> fk;
            for (int i : ks) {
                if (T.adj[i].at >= 0) {
                    fk.push_back(&T.adj[i]);
                }
            }
            PackAdjust &k = T.adj[T.key_sel];
            char who[64];
            if (ks.size() > 1) {
                snprintf(who, sizeof(who), "%d keys", (int)ks.size());
            } else {
                snprintf(who, sizeof(who), "%s's key", bone_title(k.bone));
            }
            int paste = 0;
            ImGui::TextDisabled("%s", ks.size() > 1 ? who : bone_title(k.bone));
            // copied, or the one copied pasted over it (after the rest of the menu: a paste may move the keys)
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                key_copy(T);
            }
            if (ImGui::MenuItem("Paste", nullptr, false, !T.clip.empty())) {
                paste = 1;
            }
            if (ImGui::MenuItem("Paste mirrored", nullptr, false, !T.clip.empty())) {
                paste = 2;
            }
            ImGui::Separator();
            if (!fk.empty() && ImGui::BeginMenu("Fade")) {
                // how they fade in and out: each way, the one it has ticked
                for (int e = 0; e < PACK_EASES; e++) {
                    if (ImGui::MenuItem(EaseSays[e], nullptr, k.ease == e)) {
                        adj_snapshot(T, "Made %s fade: %s", who, EaseSays[e]);
                        for (PackAdjust *x : fk) {
                            x->ease = e;
                        }
                        T.ease = e;
                    }
                    ImGui::SetItemTooltip("%s", EaseTips[e]);
                }
                ImGui::EndMenu();
            }
            if (!fk.empty() && ImGui::MenuItem("Blend into next", nullptr, k.blend)) {
                bool on = !k.blend;
                adj_snapshot(T, on ? "Blended %s into the next" : "Stopped %s blending", who);
                for (PackAdjust *x : fk) {
                    x->blend = on;
                }
            }
            ImGui::SetItemTooltip("Flow into the joint's next key instead of fading out");
            if (paste != 0) {
                key_paste(T, pack_bone_index(po, k.bone), k.at >= 0 ? k.at : T.frame, paste == 2);
            } else if (!fk.empty()) {
                // held over more of the move: to its end, from its start, all through it; or each its own frame again
                bool short_end = false, late = false, held = false;
                for (PackAdjust *x : fk) {
                    short_end |= key_last(*x) < frames - 1, late |= x->at > 0, held |= key_last(*x) > x->at;
                }
                if (ImGui::MenuItem("Stretch to end", nullptr, false, short_end)) {
                    adj_snapshot(T, "Held %s to the end", who);
                    for (PackAdjust *x : fk) {
                        x->until = frames - 1;
                    }
                }
                if (ImGui::MenuItem("Stretch to start", nullptr, false, late)) {
                    adj_snapshot(T, "Held %s from the start", who);
                    for (PackAdjust *x : fk) {
                        x->until = key_last(*x);
                        x->at = 0;
                    }
                }
                if (ImGui::MenuItem("Stretch to whole move", nullptr, false, late || short_end)) {
                    adj_snapshot(T, "Held %s all through %s", who, move);
                    for (PackAdjust *x : fk) {
                        x->at = 0;
                        x->until = frames - 1;
                    }
                }
                if (ImGui::MenuItem("Single frame", nullptr, false, held)) {
                    adj_snapshot(T, "Made %s single frames", who);
                    for (PackAdjust *x : fk) {
                        x->until = x->at;
                    }
                }
                ImGui::Separator();
            }
            if (paste == 0 && ImGui::MenuItem("Delete", "Delete")) {
                keys_delete(T);
            }
        }
        ImGui::EndPopup();
    }
    if (!T.keys_sel.empty() && !io.WantTextInput && ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
        keys_delete(T);
    }
}


// --- the views, with their markers ---

void view_window(Tool &T, View &v, const char *title, bool markers, int which) {
    // (a view's drawn only while its page shows it, so the first time it's shown its picture comes a frame later)
    if (v.tex == nullptr) {
        ImGui::Dummy(ImVec2(ViewW, ViewH));
    } else {
        ImGui::Image((ImTextureID)(intptr_t)v.tex, ImVec2(ViewW, ViewH));
    }
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

const char *short_path(const char *path);

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
        T.baked.clear();
        T.foot_back.clear();
        T.own_note[0] = 0;
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
    snprintf(T.status, sizeof(T.status), "Editing %s (%s)", T.name, short_path(f.mod));
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
    ImGui::TextDisabled("Custom heroes for Crash Bash PC");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SetWindowFontScale(1.25f);
    ImGui::Text("Create a new hero");
    ImGui::SetWindowFontScale(1.0f);
    ImGui::TextDisabled("A .glb model, or drop one on the window");
    if (ImGui::Button("Browse...", ImVec2(U(200), 0))) {
        SDL_ShowOpenFileDialog(picked, (void *)(intptr_t)PICK_MODEL, win, Glb, 1, nullptr, false);
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(w - U(200) - U(110) - 2 * ImGui::GetStyle().ItemSpacing.x);
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
    ImGui::BeginChild("found", ImVec2(0, U(220)), ImGuiChildFlags_Borders);
    if (NumFound == 0) {
        ImGui::TextDisabled("None yet (unzip a zipped mod to edit it)");
    }
    for (int i = 0; i < NumFound; i++) {
        char label[256];
        const char *mod = Founds[i].mod;
        for (const char *c = Founds[i].mod; *c != 0; c++) {
            mod = *c == '/' || *c == '\\' ? c + 1 : mod;
        }
        snprintf(label, sizeof(label), "%s   (%s)##%d", Founds[i].name, mod, i);
        if (ImGui::Selectable(label, false, 0, ImVec2(0, U(26)))) {
            load_pack(T, Founds[i]);
        }
    }
    ImGui::EndChild();
    ImGui::Text("Mods folder");
    ImGui::SetNextItemWidth(w - U(110) - ImGui::GetStyle().ItemSpacing.x);
    ImGui::InputText("##mods", T.out, sizeof(T.out));
    ImGui::SameLine();
    if (ImGui::Button("Refresh", ImVec2(-1, 0))) {
        find_heroes(T);
    }
    ImGui::Spacing();
    ImGui::TextDisabled("Ctrl + / - zoom    Ctrl 0 reset");
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
        ImGui::TextDisabled("Making...");
    } else if (T.dirty && T.made_asked) {
        if (ImGui::Button("Update preview", ImVec2(-1, 0))) {
            make_in_game(T);
        }
        ImGui::SetItemTooltip("Make it again in the games, with your changes");
    }
}

void editor_middle(Tool &T) {
    float avail = ImGui::GetContentRegionAvail().x;

    if (T.page == PAGE_MODEL) {
        if (!T.rigged) {
            if (ImGui::RadioButton("Joints", T.model_view == 0)) {
                T.model_view = 0;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Moves", T.model_view == 1)) {
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
            ImGui::TextDisabled("Drag the dots onto its joints: yellow body, blue arms, green legs, orange jaw, ears and tail");
        } else {
            view_window(T, T.play, "move", false, 2);
            T.play.zx = ViewW / 2.0f;
            T.play.zy = ViewH / 2.0f;
            look_around(T.play, T.turn);
            ImGui::SetNextItemWidth(U(180));
            ImGui::Combo("Move", &T.move, [](void *, int i) { return KitMoves[i].name; }, nullptr, KitMoveCount);
            ImGui::SetItemTooltip("The tool's own moves, which a game's moves can use");
            ImGui::SameLine();
            ImGui::Checkbox("Play", &T.playing);
            ImGui::SetNextItemWidth(U(180));
            ImGui::SliderFloat("Turn", &T.turn, -180, 180, "%.0f");
        }
        return;
    }
    bool any = false;
    for (int k = 0; k < PACK_KINDS; k++) {
        any |= T.have_made[k];
    }
    if (!any) {
        ImGui::TextDisabled(T.maker != nullptr ? "Making..." : "Not made yet");
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
                ImGui::SetItemTooltip("Edit %s", KindShort[k]);
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
        ImGui::TextDisabled(T.maker != nullptr ? "Making..." : "Not made (see the top bar)");
        return;
    }
    float w = avail < PrevW ? avail : PrevW;
    bool adjusting = T.adjusting && T.have_pose[T.kind];
    if (adjusting) {
        gizmo_buttons(T);
    }
    ImGui::Image((ImTextureID)(intptr_t)T.game_view.tex, ImVec2(w, w * PrevH / PrevW));
    ImVec2 im = ImGui::GetItemRectMin();
    bool hovered = ImGui::IsItemHovered();
    look_around(T.game_view, T.made_turn, !adjusting);
    bool on_axes = view_axes(T.game_view, T.made_turn, im, w, hovered);
    const KitMade *mk = &T.made[T.kind];
    if (adjusting) {
        adjust_view(T, im, w / PrevW, hovered && !on_axes);

    }
    ImGui::SetNextItemWidth(U(200));
    if (ImGui::BeginCombo("##watching", T.anim < mk->nanims ? mk->anims[T.anim].name : "")) {
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

// A game's move's choice changed (Its moves here): one of its own given up only on the user's yes (ask_back), as it's work of theirs
void own_change(Tool &T, int kind, int g, int m, int to) {
    int sel = T.ownmap[g][m];

    if (sel == OWN_MADE || sel == OWN_CLIP) {
        T.ask_kind = kind, T.ask_move = m, T.ask_to = to;
        SDL_strlcpy(T.ask_name, KitGames[g].moves[m], sizeof(T.ask_name));
        T.ask_open = true;
        return;
    }
    T.ownmap[g][m] = to;
    make_in_game(T);
}

// A move of its own given up: its copy, and its changes there, gone; its choice made what the user picked
void own_give_up(Tool &T, int kind, const char *move, int g, int m, int to) {
    for (size_t i = 0; i < T.baked.size(); i++) {
        if (T.baked[i].kind == kind && SDL_strcasecmp(T.baked[i].move, move) == 0) {
            T.baked.erase(T.baked.begin() + (long)i);
            break;
        }
    }
    if (kind == 0) {
        T.foot_back.push_back(std::string("foot/") + move);
    }
    std::vector<PackAdjust> keep;
    for (auto &a : T.adj) {
        if (!(a.kind == kind && SDL_strcasecmp(a.move, move) == 0)) {
            keep.push_back(a);
        }
    }
    T.adj = keep;
    if (g >= 0 && m >= 0) {
        T.ownmap[g][m] = to;
    }
    T.own_note[0] = 0;
    make_in_game(T);
}

// The yes asked before a move of its own is given up (own_change), as a window over the rest
void ask_back(Tool &T) {
    if (!ImGui::BeginPopupModal("Replace its own move?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        return;
    }
    int g = game_of(T.ask_kind);
    char to[80];
    if (T.ask_to == OWN_TEMPLATE) {
        snprintf(to, sizeof(to), "%s's", HeroNames[template_of(T, T.ask_kind)]);
    } else if (T.ask_to == OWN_CLIP) {
        snprintf(to, sizeof(to), "its own clip");
    } else {
        snprintf(to, sizeof(to), "my %s", T.ask_to >= 0 && T.ask_to < KitMoveCount ? KitMoves[T.ask_to].name : "");
    }
    ImGui::Text("Replace its own %s with %s? Your changes to it go too.", T.ask_name, to);
    ImGui::Spacing();
    if (ImGui::Button("Keep mine", ImVec2(U(160), 0)) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    char use[100];
    snprintf(use, sizeof(use), "Use %s", to);
    if (ImGui::Button(use, ImVec2(U(220), 0))) {
        own_give_up(T, T.ask_kind, T.ask_name, g, T.ask_move, T.ask_to);
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}

// On foot, where it follows a hero: its moves of its own there (made from that hero's, or its clips), each to give back
void foot_own_moves(Tool &T) {
    std::vector<std::string> mine;
    for (auto &b : T.baked) {
        if (b.kind == 0) {
            mine.push_back(b.move);
        }
    }
    for (int c = 0; T.m != nullptr && T.m->src != nullptr && c < T.m->src->nclips; c++) {
        const char *n = T.m->src->clips[c].name;
        bool back = false;
        for (auto &fb : T.foot_back) {
            back |= SDL_strcasecmp(fb.c_str(), n) == 0;
        }
        if (SDL_strncasecmp(n, "foot/", 5) == 0 && !back && baked_of(T, 0, n + 5) == nullptr) {
            mine.push_back(n + 5);
        }
    }
    if (mine.empty()) {
        return;
    }
    ImGui::TextDisabled("Own moves");
    ImGui::SetItemTooltip("Kept whoever it's based on");
    for (size_t i = 0; i < mine.size(); i++) {
        ImGui::PushID((int)i);
        ImGui::BulletText("%s", mine[i].c_str());
        ImGui::SameLine();
        char t[80];
        snprintf(t, sizeof(t), "Use %s's", HeroNames[template_of(T, 0)]);
        if (ImGui::SmallButton(t)) {
            T.ask_kind = 0, T.ask_move = -1, T.ask_to = OWN_TEMPLATE;
            SDL_strlcpy(T.ask_name, mine[i].c_str(), sizeof(T.ask_name));
            T.ask_open = true;
        }
        ImGui::PopID();
    }
}

// A game's page, on the right: Adjust its moves (the animation tools), for fixing what the game makes of it there by hand
// A row of three values, X, Y and Z in Blender's order and its colours (red, green, blue), after its label: true if one changed, and
// *taken set when one's taken (for its undo step). The panel's turns, moves and sizes (the user's word, 2026-11-22: "its very wordy for
// a visual tool": what each does is its tooltip)
bool xyz_row(const char *label, float v[3], float speed, float lo, float hi, const char *fmt, const char *const tips[3], bool *taken) {
    static const ImVec4 Tint[3] = { ImVec4(0.42f, 0.15f, 0.15f, 1), ImVec4(0.18f, 0.34f, 0.13f, 1), ImVec4(0.15f, 0.22f, 0.45f, 1) };
    static const char *const Letter[3] = { "X", "Y", "Z" };
    bool changed = false;

    ImGui::PushID(label);
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);
    float w = (ImGui::GetContentRegionAvail().x - U(60)) / 3 - ImGui::GetStyle().ItemSpacing.x;
    for (int i = 0; i < 3; i++) {
        char f[24];
        snprintf(f, sizeof(f), "%s %s", Letter[i], fmt);
        ImGui::SameLine(i == 0 ? U(60) : 0);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Tint[i]);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(Tint[i].x * 1.4f, Tint[i].y * 1.4f, Tint[i].z * 1.4f, 1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(Tint[i].x * 1.7f, Tint[i].y * 1.7f, Tint[i].z * 1.7f, 1));
        ImGui::SetNextItemWidth(w);
        ImGui::PushID(i);
        changed |= ImGui::DragFloat("##v", &v[i], speed, lo, hi, f);
        ImGui::PopID();
        *taken |= ImGui::IsItemActivated();
        ImGui::PopStyleColor(3);
        ImGui::SetItemTooltip("%s", tips[i]);
    }
    ImGui::PopID();
    return changed;
}

void adjust_panel(Tool &T) {
    if (!ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
        return;
    }
    if (T.kind == KIND_WARP_TOOL) {
        ImGui::TextDisabled("Made from On foot's moves");
        return;
    }
    if (!T.have_pose[T.kind]) {
        ImGui::TextDisabled(T.kind == 1 ? "Crash's bear seats its rider itself" : T.maker != nullptr ? "Making..." : "Update the preview first");
        if (T.kind == 1) {
            ImGui::SetItemTooltip("Pick another hero's bear (Prop from) to pose it");
        }
        return;
    }
    const KitMade *mk = &T.made[T.kind];
    const KitPose *po = &T.poses[T.kind];
    int a = T.anim < mk->nanims ? T.anim : 0, frames = mk->anims[a].frames;
    const char *move = mk->anims[a].name;
    int here = 0;
    for (auto &k : T.adj) {
        here += k.kind == T.kind;
    }
    if (ImGui::Checkbox("Edit joints", &T.adjusting) && T.adjusting) {
        T.frame = frames > 0 ? (int)(T.made_t * 30) % frames : 0;
        T.adj_playing = false;
    }
    hint("Click a joint, then drag its gizmo\n"
         "R turn   G move   S size\n"
         "Right-drag orbit   Wheel zoom   Home reset   Keypad 1 3 7 views\n"
         "Space play   Left/Right step a frame\n"
         "Ctrl+C / Ctrl+V copy and paste a key   Delete remove it\n"
         "Ctrl+Z undo   Ctrl+Y redo");
    // whose the move is: its own stays as it's made whoever it's based on; its template's becomes its own when it's first changed
    const BakedMove *copied = baked_of(T, T.kind, move);
    bool own = T.poses[T.kind].anims[a].own || copied != nullptr;
    ImGui::SameLine();
    if (own) {
        ImGui::TextColored(ImVec4(0.55f, 0.9f, 0.55f, 1), copied != nullptr ? "Own move (from %s)" : "Own move", HeroNames[copied != nullptr ? copied->from : 0]);
        ImGui::SetItemTooltip("This hero's own %s: it stays as you make it, whoever it's based on", move);
    } else {
        ImGui::TextDisabled("%s's move", HeroNames[template_of(T, T.kind)]);
        ImGui::SetItemTooltip("Changing it makes it this hero's own copy");
    }
    if (!T.adjusting) {
        if (here > 0) {
            ImGui::TextDisabled("%d key%s", here, here == 1 ? "" : "s");
        }
        return;
    }
    // what a change is made to: this frame (fading either side), the whole move, every move in this game
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Key");
    ImGui::SameLine(U(60));
    ImGui::RadioButton("Frame", &T.key_mode, KEY_FRAME);
    ImGui::SetItemTooltip("This frame, fading either side");
    ImGui::SameLine();
    ImGui::RadioButton("Move", &T.key_mode, KEY_MOVE);
    ImGui::SetItemTooltip("All of %s", move);
    ImGui::SameLine();
    ImGui::RadioButton("All moves", &T.key_mode, KEY_GAME);
    ImGui::SetItemTooltip("Every move in this game");
    if (T.key_mode == KEY_FRAME) {
        // the picked joint's key here: its fade shown, and changed with these (else they're what a new key gets)
        PackAdjust *here_key = T.sel >= 0 && T.sel < po->nbones ? panel_key(T, move, po->names[T.sel]) : nullptr;
        // what the fade's made to: every frame key picked if several are, else that key
        std::vector<PackAdjust *> fades;
        if (T.keys_sel.size() > 1) {
            for (int i : T.keys_sel) {
                if (T.adj[i].at >= 0) {
                    fades.push_back(&T.adj[i]);
                }
            }
        } else if (here_key != nullptr && here_key->at >= 0) {
            fades.push_back(here_key);
        }
        char whose[64] = "";
        if (fades.size() > 1) {
            snprintf(whose, sizeof(whose), "%d keys", (int)fades.size());
        } else if (!fades.empty()) {
            snprintf(whose, sizeof(whose), "%s's key", bone_title(fades[0]->bone));
        }
        if (!fades.empty()) {
            T.spread = fades[0]->spread;
            T.ease = fades[0]->ease;
        }
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted("Fade");
        ImGui::SameLine(U(60));
        ImGui::SetNextItemWidth(U(120));
        bool faded = ImGui::SliderInt("##fade", &T.spread, 0, 30, "%d frames");
        ImGui::SetItemTooltip("Frames it fades over, either side");
        if (ImGui::IsItemActivated()) {
            T.fade_step = false;
        }
        if (faded && !fades.empty()) {
            if (!T.fade_step) {
                adj_snapshot(T, "%s's fade", whose);
                T.fade_step = true;
            }
            for (PackAdjust *x : fades) {
                x->spread = T.spread;
            }
            adj_step_says(T, "Faded %s over %d frames", whose, T.spread);
            mirror_all(T, fades);
        }
        // how it fades in and out (the user's word, 2026-11-22: "the transion effect should be chooseable")
        ImGui::SameLine();
        ImGui::SetNextItemWidth(U(130));
        if (ImGui::BeginCombo("##ease", EaseSays[T.ease >= 0 && T.ease < PACK_EASES ? T.ease : 0])) {
            for (int e = 0; e < PACK_EASES; e++) {
                if (ImGui::Selectable(EaseSays[e], e == T.ease) && e != T.ease) {
                    T.ease = e;
                    if (!fades.empty()) {
                        adj_snapshot(T, "Made %s fade: %s", whose, EaseSays[e]);
                        for (PackAdjust *x : fades) {
                            x->ease = e;
                        }
                        mirror_all(T, fades);
                    }
                }
                ImGui::SetItemTooltip("%s", EaseTips[e]);
            }
            ImGui::EndCombo();
        }
        ImGui::SetItemTooltip("How it fades in and out");
        ImGui::SameLine();
        bool blend = !fades.empty() && fades[0]->blend;
        ImGui::BeginDisabled(fades.empty());
        if (ImGui::Checkbox("Blend", &blend)) {
            adj_snapshot(T, blend ? "Blended %s into the next" : "Stopped %s blending", whose);
            for (PackAdjust *x : fades) {
                x->blend = blend;
            }
            mirror_all(T, fades);
        }
        ImGui::EndDisabled();
        ImGui::SetItemTooltip("Flow into the joint's next key instead of fading out");
    }
    if (T.sel >= 0 && T.sel < po->nbones) {
        const char *bone = po->names[T.sel];
        PackAdjust *k = panel_key(T, move, bone);
        float turn[3] = { 0, 0, 0 }, reach[3] = { 0, 0, 0 }, size[3] = { 1, 1, 1 };
        if (k != nullptr) {
            memcpy(turn, k->turn, sizeof(turn));
            memcpy(reach, k->reach, sizeof(reach));
            for (int i = 0; i < 3; i++) {
                size[i] = 1 + k->grow[i];
            }
        }
        ImGui::SeparatorText(bone_title(bone));
        // its own axes as it stands: x to its left, y down, z to its back, shown in Blender's order X, Y, Z (its x, z, y) and ways (Z
        // up: its y the other way for a move); what each does its tooltip
        static const char *const TurnSays[3] = { "Tip forward", "Lean to its left", "Turn to its right" };
        static const char *const MoveSays[3] = { "To its left", "To its back", "Up" };
        static const char *const SizeSays[3] = { "Wider", "Deeper", "Taller" };
        static const char *const HandSays[3] = { "To its left (its arm or leg bends to follow)", "To its back (its arm or leg bends to follow)",
                                                 "Up (its arm or leg bends to follow)" };
        float t3[3] = { turn[0], turn[2], turn[1] }, m3[3] = { reach[0], reach[2], -reach[1] + 0.0f }, s3[3] = { size[0], size[2], size[1] };
        bool taken_t = false, taken_m = false, taken_s = false;
        bool changed_t = xyz_row("Turn", t3, 0.5f, -180, 180, "%.1f", TurnSays, &taken_t);
        bool hand = reach_bone(bone) && SDL_strcasecmp(bone, "hips") != 0;
        bool changed_m = xyz_row("Move", m3, 0.5f, -500, 500, "%.1f", hand ? HandSays : MoveSays, &taken_m);
        bool changed_s = xyz_row("Size", s3, 0.01f, 0.1f, 10, "%.2f", SizeSays, &taken_s);
        if (taken_t) {
            adj_snapshot(T, "Turned %s (its values)", bone_title(bone));
        }
        if (taken_m) {
            adj_snapshot(T, "Moved %s (its values)", bone_title(bone));
        }
        if (taken_s) {
            adj_snapshot(T, "Made %s bigger or smaller (its values)", bone_title(bone));
        }
        bool changed = changed_t || changed_m || changed_s;
        turn[0] = t3[0], turn[2] = t3[1], turn[1] = t3[2];
        reach[0] = m3[0], reach[2] = m3[1], reach[1] = -m3[2];
        size[0] = s3[0], size[2] = s3[1], size[1] = s3[2];
        // made to the key it shows; with none, a new one at the frame shown (stopped there, if it was playing)
        if (changed && (k = panel_key(T, move, bone)) == nullptr) {
            T.adj_playing = false;
            k = adj_key(T, move, bone, true);
        }
        if (changed && k != nullptr) {
            memcpy(k->turn, turn, sizeof(turn));
            memcpy(k->reach, reach, sizeof(reach));
            for (int i = 0; i < 3; i++) {
                k->grow[i] = fmaxf(0.1f, fminf(10, size[i])) - 1;
            }
            mirror_edit(T, (int)(k - T.adj.data()), k->at);
        }
        if (T.key_mode == KEY_FRAME && (k = panel_key(T, move, bone)) != nullptr && k->at >= 0) {
            // the frames it's held over: from, to (the same: that frame alone), or all of them
            int from = k->at, to = key_last(*k);
            float w = (ImGui::GetContentRegionAvail().x - U(60) - U(50)) / 2 - ImGui::GetStyle().ItemSpacing.x;
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Frames");
            ImGui::SameLine(U(60));
            ImGui::SetNextItemWidth(w);
            bool held = ImGui::DragInt("##held_from", &from, 0.2f, 0, frames - 1, "%d");
            ImGui::SetItemTooltip("First frame it's all there");
            if (ImGui::IsItemActivated()) {
                adj_snapshot(T, "Held %s's key", bone_title(bone));
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(w);
            held |= ImGui::DragInt("##held_to", &to, 0.2f, 0, frames - 1, "%d");
            ImGui::SetItemTooltip("Last frame it's all there");
            if (ImGui::IsItemActivated()) {
                adj_snapshot(T, "Held %s's key", bone_title(bone));
            }
            if (held) {
                int was = k->at, me = (int)(k - T.adj.data());
                k->at = std::max(0, std::min(from, to));
                k->until = std::min(frames - 1, std::max(from, to));
                T.frame = std::max(k->at, std::min(key_last(*k), T.frame));
                adj_step_says(T, k->until > k->at ? "Held %s's key from frame %d to %d" : "Made %s's key frame %d alone", bone_title(bone), k->at,
                              k->until);
                mirror_edit(T, me, was);
                k = &T.adj[me];
            }
            ImGui::SameLine();
            if (ImGui::Button("All", ImVec2(U(46), 0))) {
                adj_snapshot(T, "Held %s's key all through %s", bone_title(bone), move);
                k->at = 0;
                k->until = frames - 1;
            }
            ImGui::SetItemTooltip("Hold it through the whole move");
        } else if (T.key_mode == KEY_FRAME && T.adj_playing) {
            ImGui::TextDisabled("Click one of its keys to edit it as it plays");
        }
        ImGui::BeginDisabled(other_side(bone)[0] == 0);
        if (ImGui::Button("Mirror")) {
            adj_snapshot(T, "Mirrored %s to its other side", bone_title(bone));
            own_before_change(T, move);
            adj_mirror(T, move, bone);
        }
        ImGui::EndDisabled();
        ImGui::SetItemTooltip("Copy this joint's changes to its other side, flipped");
        ImGui::SameLine();
        if (ImGui::Button("Reset joint")) {
            adj_snapshot(T, "Reset %s", bone_title(bone));
            std::vector<PackAdjust> keep;
            for (auto &x : T.adj) {
                if (!(x.kind == T.kind && pack_adjust_for(&x, T.kind, move) && SDL_strcasecmp(x.bone, bone) == 0)) {
                    keep.push_back(x);
                }
            }
            T.adj = keep;
            keys_only(T, -1);
        }
        ImGui::SetItemTooltip("Remove this joint's keys in %s", move);
    } else {
        ImGui::TextDisabled("Click a joint in the picture");
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
    if (ImGui::Button("Reset move")) {
        adj_snapshot(T, "Reset %s", move);
        std::vector<PackAdjust> keep;
        for (auto &x : T.adj) {
            if (!(x.kind == T.kind && SDL_strcasecmp(x.move, move) == 0)) {
                keep.push_back(x);
            }
        }
        T.adj = keep;
        keys_only(T, -1);
    }
    ImGui::SetItemTooltip("Remove every key in %s", move);
    // the undo history: each step, the one it's at lit; clicked, gone back (or on) to it
    if (ImGui::TreeNodeEx("History", ImGuiTreeNodeFlags_DefaultOpen)) {
        int at = (int)T.undo.size(), all = at + (int)T.redo.size();
        static int shown_at = -1;
        float line = ImGui::GetTextLineHeightWithSpacing(), pad = ImGui::GetStyle().WindowPadding.y * 2;
        ImGui::BeginChild("##history", ImVec2(0, fminf((float)(all + 1), 6.0f) * line + pad), ImGuiChildFlags_Borders);
        for (int i = 0; i <= all; i++) {
            const char *says = i == 0 ? T.undo.size() >= 300 ? "(as far back as it goes)" : "(start)"
                             : i <= at ? T.undo[i - 1].says.c_str()
                                       : T.redo[T.redo.size() - (size_t)(i - at)].says.c_str();
            ImGui::PushID(i);
            if (i > at) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
            }
            if (ImGui::Selectable(says, i == at)) {
                adj_go(T, i);
            }
            if (i > at) {
                ImGui::PopStyleColor();
            }
            if (i == at && shown_at != at) {
                ImGui::SetScrollHereY(1);
            }
            ImGui::PopID();
        }
        shown_at = at;
        ImGui::EndChild();
        ImGui::TreePop();
    }
    ImGui::SetItemTooltip("Click a step to go back to it");
    // its keys in this move, folded away (the timeline shows them): each picked to go to it, or removed
    int shown = 0, gone = -1;
    for (auto &x : T.adj) {
        shown += pack_adjust_for(&x, T.kind, move);
    }
    char head[48];
    snprintf(head, sizeof(head), "Keys (%d)###keys", shown);
    if (shown > 0 && ImGui::TreeNode(head)) {
        for (int i = 0; i < (int)T.adj.size(); i++) {
            PackAdjust &x = T.adj[i];
            if (!pack_adjust_for(&x, T.kind, move)) {
                continue;
            }
            char line[160];
            int b = pack_bone_index(po, x.bone);
            if (x.at < 0) {
                snprintf(line, sizeof(line), "%s  %s##%d", bone_title(x.bone), strcmp(x.move, "*") == 0 ? "all moves" : "whole move", i);
            } else if (key_last(x) > x.at) {
                snprintf(line, sizeof(line), "%s  %d-%d##%d", bone_title(x.bone), x.at, key_last(x), i);
            } else {
                snprintf(line, sizeof(line), "%s  %d##%d", bone_title(x.bone), x.at, i);
            }
            ImGui::PushID(i);
            if (ImGui::SmallButton("x")) {
                gone = i;
            }
            ImGui::SetItemTooltip("Remove it");
            ImGui::PopID();
            ImGui::SameLine();
            if (ImGui::Selectable(line, b == T.sel && (x.at < 0 || (x.at <= T.frame && T.frame <= key_last(x))))) {
                T.sel = b;
                keys_only(T, i);
                if (x.at >= 0) {
                    T.frame = x.at;
                    T.key_mode = KEY_FRAME;
                    T.spread = x.spread;
                    T.ease = x.ease;
                } else {
                    T.key_mode = strcmp(x.move, "*") == 0 ? KEY_GAME : KEY_MOVE;
                }
                T.adj_playing = false;
            }
        }
        ImGui::TreePop();
    }
    if (gone >= 0) {
        adj_snapshot(T, "Took away %s's change", bone_title(T.adj[gone].bone));
        T.adj.erase(T.adj.begin() + gone);
        keys_only(T, -1);
    }
}

void editor_right(Tool &T, SDL_Window *win) {
    static const SDL_DialogFileFilter Glb[] = { { "3D model (glTF binary)", "glb" } };
    static const SDL_DialogFileFilter Png[] = { { "Picture (PNG)", "png" } };
    static const SDL_DialogFileFilter Wav[] = { { "Sound (WAV)", "wav" } };

    // the fields leave room for their labels
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Mod's name").x - 2 * ImGui::GetStyle().ItemInnerSpacing.x);

    // the page's own choices
    if (T.page == PAGE_MODEL) {
        ImGui::SeparatorText("Model");
        ImGui::TextWrapped("%s", short_path(T.model));
        if (ImGui::Button("Open...")) {
            browse(win, PICK_MODEL, Glb);
        }
        ImGui::Spacing();
        if (T.rigged) {
            ImGui::TextDisabled("Rigged (its own bones)");
        } else {
            ImGui::TextDisabled("No rig: place its joints");
            ImGui::Checkbox("Mirror##dots", &T.mirror);
            ImGui::SetItemTooltip("Moving one side's dot moves the other's");
            ImGui::SameLine();
            if (ImGui::Button("Guess joints")) {
                kit_guess_markers(T.m);
                reweigh(T);
            }
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Has");
            ImGui::SameLine();
            const char *opt[] = { "Jaw", "Ears", "Tail" };
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
            if (ImGui::SliderFloat("Arm lift", &T.m->arm_raise, 20, 160, "%.0f deg")) {
                T.dirty = true;
            }
            ImGui::SetItemTooltip("How high it can raise its arms (less for short arms or a big head)");
        }
    } else if (T.page == PAGE_ALL) {
        ImGui::SeparatorText("All games");
        ImGui::TextDisabled("Click one to edit it");
    } else {
        int k = T.kind, g = game_of(k);
        ImGui::SeparatorText(KindTitles[k]);
        if (k != KIND_WARP_TOOL) {
            char now[64];
            snprintf(now, sizeof(now), "%s's (base)", HeroNames[T.template_hero]);
            ImGui::Text(k == 0 ? "Stands like" : k == 1 ? "Rides the bear of" : "Prop from");
            ImGui::SameLine(U(130));
            ImGui::SetNextItemWidth(-1);
            bool own = k == 0 && T.foot_own;
            if (ImGui::BeginCombo("##from", own ? "Own moves" : T.kind_template[k] < 0 ? now : HeroNames[T.kind_template[k]])) {
                // on foot, its own moves: its model's clips, and the tool's for the rest
                if (k == 0 && ImGui::Selectable("Own moves", own)) {
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
                ImGui::SetItemTooltip("Own moves: its clips, and the tool's moves for the rest. A hero: moves as they do");
            }
        } else {
            ImGui::TextDisabled("Made from On foot's moves");
        }
        ImGui::Spacing();
        if (g >= 0) {
            ImGui::TextDisabled("Moves");
            ImGui::SetItemTooltip("What it plays for each of this game's moves: its base's, or its own");
            int from = T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero;
            for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                char now[80];
                int sel = T.ownmap[g][m];
                bool clip = own_clip_of(T, g, m) >= 0;
                if (sel == OWN_TEMPLATE) {
                    snprintf(now, sizeof(now), "%s's", HeroNames[from]);
                } else if (sel == OWN_CLIP) {
                    snprintf(now, sizeof(now), "own clip");
                } else if (sel == OWN_MADE) {
                    const BakedMove *bm = baked_of(T, k, KitGames[g].moves[m]);
                    snprintf(now, sizeof(now), "own (from %s)", HeroNames[bm != nullptr ? bm->from : from]);
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
                    if (ImGui::Selectable(t, sel == OWN_TEMPLATE) && sel != OWN_TEMPLATE) {
                        own_change(T, k, g, m, OWN_TEMPLATE);
                    }
                    if (sel == OWN_MADE) {
                        ImGui::Selectable(now, true);
                    }
                    if (clip && ImGui::Selectable("own clip", sel == OWN_CLIP) && sel != OWN_CLIP) {
                        own_change(T, k, g, m, OWN_CLIP);
                    }
                    // (the tool's own moves aren't offered: they never line up with a game's, the user's word, 2026-11-22; one a pack
                    // has from before still shows while it's picked)
                    if (sel >= 0) {
                        ImGui::Selectable(now, true);
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopID();
            }
        } else {
            if (k == 0 && !T.foot_own) {
                foot_own_moves(T);
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
        ImGui::SetItemTooltip("As the game shows it");
        ImGui::InputText("Id", T.id, sizeof(T.id), ImGuiInputTextFlags_CharsNoBlank);
        ImGui::SetItemTooltip("Short name for files and online");
        if (ImGui::InputInt("Height", &T.height, 10, 50)) {
            T.height = T.height < 200 ? 200 : T.height > 900 ? 900 : T.height;
            T.dirty = true;
        }
        ImGui::SetItemTooltip("Crash is 452, most heroes about 650");
        if (ImGui::Combo("Based on", &T.template_hero, HeroNames, 8)) {
            T.dirty = true;
            int mine = (int)T.baked.size();
            for (int g = 0; g < KitGameCount && g < 8; g++) {
                for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                    mine += T.ownmap[g][m] == OWN_CLIP || T.ownmap[g][m] >= 0;
                }
            }
            if (mine > 0) {
                snprintf(T.status, sizeof(T.status), "Based on %s: its %d own moves stay. Update the preview to see it.", HeroNames[T.template_hero],
                         mine);
            }
        }
        ImGui::SetItemTooltip("Plays as this hero: their moves, speed, ride or prop, and sounds it hasn't got");
        if (ImGui::SliderFloat("Face", &T.face_head, 0, 0.6f, T.face_head > 0 ? "top %.2f" : "auto")) {
            T.dirty = true;
        }
        ImGui::SetItemTooltip("How much of it, from the top, is its head (its face, without a card)");
    }
    if (ImGui::CollapsingHeader("Card and sounds")) {
        ImGui::Text("Card");
        ImGui::SetItemTooltip("Character select picture (PNG)");
        ImGui::SameLine(U(110));
        ImGui::TextUnformatted(T.card[0] ? short_path(T.card) : "from the model");
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
        ImGui::SetItemTooltip("A WAV for each; without one, its base's");
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
        if (ImGui::Button("Save##mod", ImVec2(-1, U(30)))) {
            export_pack(T, false);
        }
        ImGui::SetItemTooltip("%s", T.loaded_mod[0] ? T.loaded_mod : T.out);
        ImGui::SetNextItemWidth(U(160));
        ImGui::Combo("##mode", &T.mode, [](void *, int i) { return Modes[i].name; }, nullptr, (int)(sizeof(Modes) / sizeof(Modes[0])));
        ImGui::SameLine();
        if (ImGui::Button("Save and play", ImVec2(-1, 0))) {
            export_pack(T, true);
        }
        ImGui::SetItemTooltip("Save, then play it in this game");
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
        // Ctrl+C copies a key, Ctrl+V pastes it at the frame shown on the joint picked (Ctrl+Shift+V mirrored)
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C, false)) {
            key_copy(T);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_A, false) && T.made[T.kind].nanims > 0) {
            keys_all(T, T.made[T.kind].anims[T.anim < T.made[T.kind].nanims ? T.anim : 0].name);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V, false)) {
            key_paste(T, T.sel, T.frame, io.KeyShift);
        }
        // R, G and S pick the gizmo, as Blender's
        if (!io.KeyCtrl && !T.dragging) {
            T.gizmo = ImGui::IsKeyPressed(ImGuiKey_R, false) ? GIZMO_TURN : ImGui::IsKeyPressed(ImGuiKey_G, false) ? GIZMO_MOVE
                    : ImGui::IsKeyPressed(ImGuiKey_S, false) ? GIZMO_SIZE : T.gizmo;
        }
    }
    // a game shown: the keypad looks from a side, as Blender's (1 its front, 3 its left side, 7 above; Ctrl from the other side)
    if (T.page >= 0 && !io.WantTextInput) {
        int from = ImGui::IsKeyPressed(ImGuiKey_Keypad1, false) ? 4 : ImGui::IsKeyPressed(ImGuiKey_Keypad3, false) ? 0
                 : ImGui::IsKeyPressed(ImGuiKey_Keypad7, false) ? 2 : -1;
        if (from >= 0) {
            view_from(T.game_view, T.made_turn, io.KeyCtrl ? (from + 3) % 6 : from);
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
    if (T.ask_open) {
        ImGui::OpenPopup("Replace its own move?");
        T.ask_open = false;
    }
    ask_back(T);
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
    v.drawn.clear();
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
    // the model's views fitted again to their new size (they kept the old one's: the model small in a corner)
    if (T.m != nullptr) {
        fit(T.m, T.front);
        fit(T.m, T.side);
        fit(T.m, T.play);
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
    char *pref = pref_path(), path[1100];
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
    char *pref = pref_path(), path[1100], text[32];

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
    T.game_view.grid = true;
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
    int drag_step = -1; /* (a test drag's step: HEROTOOL_DRAG) */
    /* a test's: HEROTOOL_PROFILE=1 prints how long each part of a frame took, on average, every 60 frames */
    bool profile = SDL_getenv("HEROTOOL_PROFILE") != nullptr;
    enum { PR_THUMBS, PR_ADJUST, PR_GAME, PR_MODEL, PR_UI, PR_DRAW, PR_PRESENT, PR_N };
    static const char *const PrSays[PR_N] = { "thumbs", "adjusted", "game view", "model views", "ui", "draw ui", "present" };
    double pr[PR_N] = {};
    int pr_frames = 0;
    Uint64 pr_at = 0;
    auto lap = [&](int part) {
        Uint64 n = SDL_GetPerformanceCounter();
        if (profile && part >= 0) {
            pr[part] += (double)(n - pr_at) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        }
        pr_at = n;
    };
    while (!done) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            // (a test's drag has the mouse: the real one's left out)
            bool mouse = e.type == SDL_EVENT_MOUSE_MOTION || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP
                      || e.type == SDL_EVENT_MOUSE_WHEEL;
            if (!(mouse && (SDL_getenv("HEROTOOL_DRAG") != nullptr || SDL_getenv("HEROTOOL_WHEEL") != nullptr))) {
                ImGui_ImplSDL3_ProcessEvent(&e);
            }
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
        /* a test's: HEROTOOL_ASK=MOVE picks its template's for that move of the game shown, as its list would (asking first if it's
           its own) */
        static bool asked = false;
        if (!asked && SDL_getenv("HEROTOOL_ASK") != nullptr && T.page >= 0 && T.have_pose[T.kind] && T.maker == nullptr) {
            int g = game_of(T.kind);
            for (int m = 0; g >= 0 && m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                if (SDL_strcasecmp(KitGames[g].moves[m], SDL_getenv("HEROTOOL_ASK")) == 0) {
                    own_change(T, T.kind, g, m, OWN_TEMPLATE);
                }
            }
            asked = true;
        }
        /* a test's: HEROTOOL_SAVE_TO=FOLDER saves it there once HEROTOOL_ADJUST's change is made and the game's made it again */
        static bool adjust_set = false, adjust_saved = false;
        /* (after a test drag, once it's let go) */
        if (adjust_set && !adjust_saved && SDL_getenv("HEROTOOL_SAVE_TO") != nullptr && T.maker == nullptr && !T.remake
            && (SDL_getenv("HEROTOOL_DRAG") == nullptr || drag_step > 14)) {
            export_pack(T, false, SDL_getenv("HEROTOOL_SAVE_TO"));
            printf("herotool: %s\n", T.status);
            adjust_saved = true;
        }
        /* a test's: HEROTOOL_ADJUST=BONE@FRAME adjusts the game shown once it's made, that bone picked at that frame */
        if (!adjust_set && SDL_getenv("HEROTOOL_ADJUST") != nullptr && T.page >= 0 && T.have_pose[T.kind]) {
            char bone[64];
            const char *at = strchr(SDL_getenv("HEROTOOL_ADJUST"), '@');
            SDL_strlcpy(bone, SDL_getenv("HEROTOOL_ADJUST"), sizeof(bone));
            if (strchr(bone, '@') != nullptr) {
                *strchr(bone, '@') = 0;
            }
            T.adjusting = true;
            /* (HEROTOOL_MOVE=NAME: that move watched) */
            for (int m = 0; SDL_getenv("HEROTOOL_MOVE") != nullptr && m < T.made[T.kind].nanims; m++) {
                T.anim = SDL_strcasecmp(T.made[T.kind].anims[m].name, SDL_getenv("HEROTOOL_MOVE")) == 0 ? m : T.anim;
            }
            T.frame = at != nullptr ? SDL_atoi(at + 1) : 0;
            for (int b = 0; b < T.poses[T.kind].nbones; b++) {
                T.sel = SDL_strcasecmp(T.poses[T.kind].names[b], bone) == 0 ? b : T.sel;
            }
            adjust_set = true;
            /* and HEROTOOL_ADJUST_TURN / _REACH / _SCALE=X,Y,Z changes it there, as the panel's values would; _GIZMO=N picks gizmo N */
            const char *turn = SDL_getenv("HEROTOOL_ADJUST_TURN"), *reach = SDL_getenv("HEROTOOL_ADJUST_REACH");
            const char *size = SDL_getenv("HEROTOOL_ADJUST_SCALE");
            T.gizmo = SDL_getenv("HEROTOOL_GIZMO") != nullptr ? SDL_atoi(SDL_getenv("HEROTOOL_GIZMO")) : T.gizmo;
            PackAdjust *k = T.sel >= 0 && (turn != nullptr || reach != nullptr || size != nullptr)
                                ? adj_key(T, T.made[T.kind].anims[T.anim].name, T.poses[T.kind].names[T.sel], true)
                                : nullptr;
            if (k != nullptr) {
                if (turn != nullptr) {
                    SDL_sscanf(turn, "%f,%f,%f", &k->turn[0], &k->turn[1], &k->turn[2]);
                }
                if (reach != nullptr) {
                    SDL_sscanf(reach, "%f,%f,%f", &k->reach[0], &k->reach[1], &k->reach[2]);
                }
                if (size != nullptr) {
                    float g[3] = { 1, 1, 1 };
                    SDL_sscanf(size, "%f,%f,%f", &g[0], &g[1], &g[2]);
                    for (int i = 0; i < 3; i++) {
                        k->grow[i] = g[i] - 1;
                    }
                }
                /* (BONE@FROM-TO: held from one to the other) */
                if (at != nullptr && strchr(at, '-') != nullptr) {
                    k->until = SDL_atoi(strchr(at, '-') + 1);
                }
                /* (HEROTOOL_ADJUST_FADE=EASE,N: its fade, hero.json's name for it, over N frames) */
                if (SDL_getenv("HEROTOOL_ADJUST_FADE") != nullptr) {
                    char e[32] = "";
                    int n = k->spread, blend = 0;
                    SDL_sscanf(SDL_getenv("HEROTOOL_ADJUST_FADE"), "%31[^,],%d,%d", e, &n, &blend);
                    k->ease = pack_ease_named(e) >= 0 ? pack_ease_named(e) : k->ease;
                    k->spread = n;
                    k->blend = blend != 0; /* (EASE,N,1: blended into the next) */
                }
                keys_only(T, (int)(k - T.adj.data()));
                /* (HEROTOOL_XMIRROR=1: X-Mirror on, this change mirrored as an edit's is) */
                T.xmirror = SDL_getenv("HEROTOOL_XMIRROR") != nullptr;
                mirror_edit(T, T.key_sel, T.adj[T.key_sel].at);
                /* and HEROTOOL_PASTE=FRAME[,M[,BONE]] copies it and pastes it there (M: mirrored; onto BONE, else its own), as Ctrl+C and
                   Ctrl+V (Ctrl+Shift+V) do; then HEROTOOL_SELECT_ALL=1 picks every key of the move (Ctrl+A), and HEROTOOL_PASTE2=FRAME[,M]
                   copies those picked and pastes them there */
                if (SDL_getenv("HEROTOOL_PASTE") != nullptr) {
                    int to = 0, mirrored = 0;
                    char onto[64] = "";
                    SDL_sscanf(SDL_getenv("HEROTOOL_PASTE"), "%d,%d,%63s", &to, &mirrored, onto);
                    key_copy(T);
                    key_paste(T, onto[0] != 0 ? pack_bone_index(&T.poses[T.kind], onto) : T.sel, to, mirrored != 0);
                }
                if (SDL_getenv("HEROTOOL_SELECT_ALL") != nullptr) {
                    keys_all(T, T.made[T.kind].anims[T.anim].name);
                }
                if (SDL_getenv("HEROTOOL_PASTE2") != nullptr) {
                    int to = 0, mirrored = 0;
                    SDL_sscanf(SDL_getenv("HEROTOOL_PASTE2"), "%d,%d", &to, &mirrored);
                    key_copy(T);
                    key_paste(T, T.sel, to, mirrored != 0);
                }
            }
            /* and HEROTOOL_SHOW_FRAME=N shows that frame after (what HEROTOOL_LIVE writes) */
            if (SDL_getenv("HEROTOOL_SHOW_FRAME") != nullptr) {
                T.frame = SDL_atoi(SDL_getenv("HEROTOOL_SHOW_FRAME"));
            }
            /* and HEROTOOL_ONION=STEP turns onion skin on, its ghosts that far apart */
            if (SDL_getenv("HEROTOOL_ONION") != nullptr) {
                T.onion = true;
                T.onion_step = SDL_atoi(SDL_getenv("HEROTOOL_ONION"));
            }
            /* and HEROTOOL_CLIPPING=1 turns the clipping finder on, printing each move's frames with any and its most */
            if (SDL_getenv("HEROTOOL_CLIPPING") != nullptr) {
                T.clipping = true;
                for (int m = 0; m < T.made[T.kind].nanims; m++) {
                    clip_scan(T, T.kind, m);
                    int any = 0, most = 0, at = 0;
                    for (int g = 0; g < (int)Clips.count.size(); g++) {
                        any += Clips.count[g] > 0;
                        if (Clips.count[g] > most) {
                            most = Clips.count[g], at = g;
                        }
                    }
                    printf("herotool: clipping in %s: %d of %d frames, most %d at frame %d\n", T.made[T.kind].anims[m].name, any,
                           (int)Clips.count.size(), most, at);
                }
                fflush(stdout);
                Clips.anim = -1;
            }
            /* and HEROTOOL_PLAY=1 plays it (as Space does) */
            T.adj_playing = SDL_getenv("HEROTOOL_PLAY") != nullptr ? true : T.adj_playing;
        }
        lap(-1);
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
        lap(PR_THUMBS);
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
            lap(PR_GAME);
            bool live = adjusted_frame(T, T.kind, a, f, T.live);
            // onion skin, while adjusting: the frames onion_step before and after (round the move), as the game makes them
            const int16_t *ghosts[2] = { nullptr, nullptr };
            if (adjusting && T.onion && mk->nanims > 0 && mk->anims[a].frames > 1) {
                int n = mk->anims[a].frames, st = std::max(1, std::min(n - 1, T.onion_step));
                for (int g = 0; g < 2; g++) {
                    int at = ((f + (g == 0 ? -st : st)) % n + n) % n;
                    ghosts[g] = adjusted_frame(T, T.kind, a, at, T.ghost[g]) ? T.ghost[g].data()
                                                                             : mk->anims[a].verts + (size_t)at * mk->nvert * 3;
                }
            }
            lap(PR_ADJUST);
            draw_made(mk, a, f, T.game_view, ren, T.rides[T.kind], T.nrides[T.kind], live ? T.live.data() : nullptr, ghosts[0], ghosts[1]);
        }
        lap(PR_GAME);
        if (T.m != nullptr && !T.home && T.page == PAGE_MODEL) {
            // the front and side views at rest, or the third playing the move: as the model's page shows them, only while it's shown
            if (!T.rigged && T.model_view == 0) {
                draw(T.m, T.m->pos, T.front, ren);
                draw(T.m, T.m->pos, T.side, ren);
            } else {
                pose(T);
                T.play.yaw = T.turn;
                draw(T.m, T.posed, T.play, ren);
            }
        }
        lap(PR_MODEL);
        if (fabsf(want_scale - Ui) > 0.01f) {
            ui_scale(T, base_style, want_scale);
            want_scale = Ui;
        }
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        /* a test's drag: HEROTOOL_DRAG=X0,Y0,X1,Y1[,SHIFT[,BUTTON]] presses the left button (or the right) at (X0, Y0) in the window once a game's shown, moves
           to (X1, Y1) over ten frames and lets go (Shift held if SHIFT is 1), as a person's mouse would, and keeps it there after (the
           real mouse left out: the SDL backend gives where it is on the screen while the window has the focus) */
        if (SDL_getenv("HEROTOOL_DRAG") != nullptr && (drag_step >= 0 || T.adjusting || (T.page >= 0 && T.have_made[T.kind] && T.maker == nullptr))) {
            /* (a sixth, BUTTON: 0 the left, 1 the right) */
            float d[6] = { 0, 0, 0, 0, 0, 0 };
            SDL_sscanf(SDL_getenv("HEROTOOL_DRAG"), "%f,%f,%f,%f,%f,%f", &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
            ImGuiIO &dio = ImGui::GetIO();
            // Shift down the frame before the press and up the frame after the release, and the mouse before it each frame: ImGui
            // keeps a mouse move or press queued after a key's change for the frame after (its input trickling)
            drag_step = drag_step < 100 ? drag_step + 1 : drag_step;
            float t = drag_step < 3 ? 0 : drag_step > 12 ? 1 : (drag_step - 2) / 10.0f;
            dio.AddMousePosEvent(d[0] + (d[2] - d[0]) * t, d[1] + (d[3] - d[1]) * t);
            dio.AddMouseButtonEvent(d[5] != 0 ? 1 : 0, drag_step >= 2 && drag_step <= 12);
            dio.AddKeyEvent(ImGuiMod_Shift, d[4] != 0 && drag_step >= 1 && drag_step <= 13);
        }
        /* a test's wheel: HEROTOOL_WHEEL=X,Y,N turns the mouse wheel N notches at (X, Y) once a game's being adjusted, the mouse kept there */
        static int wheel_step = -1;
        if (SDL_getenv("HEROTOOL_WHEEL") != nullptr && SDL_getenv("HEROTOOL_DRAG") == nullptr && (wheel_step >= 0 || T.adjusting)) {
            float w[3] = { 0, 0, 0 };
            SDL_sscanf(SDL_getenv("HEROTOOL_WHEEL"), "%f,%f,%f", &w[0], &w[1], &w[2]);
            wheel_step = wheel_step < 100 ? wheel_step + 1 : wheel_step;
            ImGui::GetIO().AddMousePosEvent(w[0], w[1]);
            if (wheel_step == 3) {
                ImGui::GetIO().AddMouseWheelEvent(0, w[2]);
            }
        }
        ImGui::NewFrame();
        if (T.home) {
            start_screen(T, win);
        } else {
            editor_window(T, win);
        }
        ImGui::Render();
        lap(PR_UI);
        SDL_SetRenderDrawColor(ren, 30, 30, 34, 255);
        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ren);
        lap(PR_DRAW);
        if (shot != nullptr && T.maker == nullptr && ++frames == 40) {
            SDL_Surface *grab = SDL_RenderReadPixels(ren, nullptr);

            if (grab != nullptr) {
                SDL_SavePNG(grab, shot);
                SDL_DestroySurface(grab);
            }
            /* and HEROTOOL_LIVE=FILE the frame shown as it made it, its vertices (s16 x, y, z each), to set beside the game's */
            if (SDL_getenv("HEROTOOL_LIVE") != nullptr && T.page >= 0 && adjusted_frame(T, T.kind, T.anim, T.frame, T.live)) {
                SDL_SaveFile(SDL_getenv("HEROTOOL_LIVE"), T.live.data(), T.live.size() * sizeof(int16_t));
                char said[300], at[80];
                SDL_snprintf(said, sizeof(said), "%s.txt", SDL_getenv("HEROTOOL_LIVE"));
                SDL_snprintf(at, sizeof(at), "%s %d\n", T.made[T.kind].anims[T.anim].name, T.frame);
                SDL_SaveFile(said, at, strlen(at));
            }
            done = true;
        }
        SDL_RenderPresent(ren);
        lap(PR_PRESENT);
        if (profile && ++pr_frames == 60) {
            printf("herotool: a frame's");
            for (int p = 0; p < PR_N; p++) {
                printf(" %s %.2f ms%s", PrSays[p], pr[p] / pr_frames, p + 1 < PR_N ? "," : "\n");
                pr[p] = 0;
            }
            fflush(stdout);
            pr_frames = 0;
        }
    }
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
