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
#include "sound.h"

extern "C" const unsigned char FredokaTtf[];
extern "C" const unsigned int FredokaTtfSize;

namespace {

// The window's scale (ui_scale): its screen's, from its display scale or its height against a 1080-line screen's (the user's word,
// 2026-11-22: far too small on a high resolution screen), and Ctrl with + and - on top. Sizes in the window are written as they are at
// 1, through U.
float Ui = 1;
inline float U(float px) {
    return px * Ui;
}
// --- the look: Graphite (the user's pick, 2026-11-22; docs/HERO_PACKS.md, The look): neutral charcoal, soft round shapes, coral for what
// does something when pressed, periwinkle for whatever is picked or on, Fredoka for the words, a soft note for each action (sound.c) ---

const ImU32 GROUND = IM_COL32(0x19, 0x19, 0x1B, 255), CARD = IM_COL32(0x24, 0x24, 0x27, 255), RAISED = IM_COL32(0x2E, 0x2E, 0x32, 255);
const ImU32 DEEP = IM_COL32(0x1F, 0x1F, 0x22, 255), PICTURE = IM_COL32(0x1E, 0x1E, 0x21, 255), SHADOW = IM_COL32(0x0F, 0x0F, 0x11, 255);
const ImU32 HOVER = IM_COL32(0x3A, 0x3A, 0x40, 255);
const ImU32 INK = IM_COL32(0xED, 0xEB, 0xEF, 255), MUTED = IM_COL32(0xBE, 0xBB, 0xC4, 255), DIM = IM_COL32(0x9A, 0x97, 0xA1, 255);
const ImU32 CORAL = IM_COL32(0xF5, 0x9E, 0x8B, 255), CORAL_INK = IM_COL32(0x2A, 0x15, 0x12, 255);
// deleting: red, its icon white (the user's word, 2026-11-23: "the trash icon should be red when hovered")
const ImU32 RED = IM_COL32(0xE5, 0x48, 0x4D, 255), RED_INK = IM_COL32(0xFF, 0xFF, 0xFF, 255);
const ImU32 PERI = IM_COL32(0xA9, 0xB8, 0xF5, 255), PERI_INK = IM_COL32(0x16, 0x1B, 0x33, 255), PERI_LIGHT = IM_COL32(0xC9, 0xD3, 0xFA, 255);
const ImU32 PERI_DEEP = IM_COL32(0x71, 0x83, 0xC9, 255);
const ImU32 MINT = IM_COL32(0x9E, 0xD8, 0xBE, 255), MINT_LIGHT = IM_COL32(0xB6, 0xE2, 0xC6, 255);
const ImU32 LILAC = IM_COL32(0xC3, 0xB1, 0xEE, 255), LILAC_INK = IM_COL32(0x1F, 0x17, 0x33, 255);
const ImU32 TINT_CORAL = IM_COL32(0x3D, 0x33, 0x33, 255), TINT_PERI = IM_COL32(0x34, 0x36, 0x40, 255);
const ImU32 TINT_MINT = IM_COL32(0x33, 0x3A, 0x39, 255), TINT_SAND = IM_COL32(0x3B, 0x39, 0x37, 255);
const ImU32 GREY_DOT = IM_COL32(0x8E, 0x8B, 0x96, 255);
// the axes' colours, softened: X coral, Y mint, Z periwinkle
const ImU32 AXIS_X = CORAL, AXIS_Y = MINT, AXIS_Z = PERI;

inline ImVec4 V4(ImU32 c) {
    return ImGui::ColorConvertU32ToFloat4(c);
}

// the words: Fredoka at a size for each use (ui_scale makes them at the window's scale)
ImFont *FontBody, *FontSmall, *FontTitle, *FontBig;

// Graphite's style at scale 1 (ui_scale scales it)
void graphite_style(ImGuiStyle &s) {
    ImGui::StyleColorsDark(&s);
    s.WindowPadding = ImVec2(18, 14);
    s.FramePadding = ImVec2(12, 7);
    s.ItemSpacing = ImVec2(10, 9);
    s.ItemInnerSpacing = ImVec2(8, 6);
    s.CellPadding = ImVec2(6, 4);
    s.ScrollbarSize = 10;
    s.GrabMinSize = 10;
    s.WindowBorderSize = s.ChildBorderSize = s.PopupBorderSize = s.FrameBorderSize = s.TabBorderSize = 0;
    s.WindowRounding = 0;
    s.ChildRounding = 24;
    s.FrameRounding = 14;
    s.PopupRounding = 18;
    s.ScrollbarRounding = s.GrabRounding = s.TabRounding = 12;
    s.SeparatorTextBorderSize = 2;
    s.WindowMenuButtonPosition = ImGuiDir_None;
    ImVec4 *c = s.Colors;
    c[ImGuiCol_Text] = V4(INK);
    c[ImGuiCol_TextDisabled] = V4(DIM);
    c[ImGuiCol_WindowBg] = V4(GROUND);
    c[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_PopupBg] = V4(RAISED);
    c[ImGuiCol_Border] = V4(RAISED);
    c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_FrameBg] = V4(DEEP);
    c[ImGuiCol_FrameBgHovered] = V4(IM_COL32(0x2A, 0x2A, 0x30, 255));
    c[ImGuiCol_FrameBgActive] = V4(TINT_PERI);
    c[ImGuiCol_TitleBg] = c[ImGuiCol_TitleBgActive] = c[ImGuiCol_TitleBgCollapsed] = c[ImGuiCol_MenuBarBg] = V4(CARD);
    c[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ScrollbarGrab] = V4(RAISED);
    c[ImGuiCol_ScrollbarGrabHovered] = V4(HOVER);
    c[ImGuiCol_ScrollbarGrabActive] = V4(PERI);
    c[ImGuiCol_CheckMark] = V4(PERI);
    c[ImGuiCol_SliderGrab] = V4(PERI);
    c[ImGuiCol_SliderGrabActive] = V4(PERI_LIGHT);
    c[ImGuiCol_Button] = V4(RAISED);
    c[ImGuiCol_ButtonHovered] = V4(HOVER);
    c[ImGuiCol_ButtonActive] = V4(TINT_PERI);
    c[ImGuiCol_Header] = V4(TINT_PERI);
    c[ImGuiCol_HeaderHovered] = V4(HOVER);
    c[ImGuiCol_HeaderActive] = V4(TINT_PERI);
    c[ImGuiCol_Separator] = c[ImGuiCol_SeparatorHovered] = V4(RAISED);
    c[ImGuiCol_SeparatorActive] = V4(PERI);
    c[ImGuiCol_ResizeGrip] = c[ImGuiCol_ResizeGripHovered] = c[ImGuiCol_ResizeGripActive] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_Tab] = c[ImGuiCol_TabDimmed] = V4(RAISED);
    c[ImGuiCol_TabHovered] = V4(HOVER);
    c[ImGuiCol_TabSelected] = c[ImGuiCol_TabDimmedSelected] = V4(TINT_PERI);
    c[ImGuiCol_TableHeaderBg] = V4(CARD);
    c[ImGuiCol_TableBorderStrong] = c[ImGuiCol_TableBorderLight] = V4(RAISED);
    c[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.02f);
    c[ImGuiCol_TextLink] = V4(PERI);
    c[ImGuiCol_TextSelectedBg] = ImVec4(0.66f, 0.72f, 0.96f, 0.35f);
    c[ImGuiCol_DragDropTarget] = c[ImGuiCol_NavCursor] = V4(PERI);
    c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.5f);
}

int ViewW = 300, ViewH = 420;
// A model's view's own size, if it has one (Hero Setup's: its joints' views big, its preview small), else the views' (ViewW x ViewH)
struct View;
int vw(const View &v);
int vh(const View &v);
int PrevW = 600, PrevH = 560; // the view of what the game made
int ThumbW = 220, ThumbH = 180; // each game's in the gallery of them all

struct View {
    SDL_Texture *tex = nullptr;
    unsigned *rgba = nullptr;
    float *depth = nullptr;
    // model to screen: x across (from its axes, turned by yaw), y up, both scaled and moved to fit
    float yaw = 0, scale = 1, ox = 0, oy = 0;
    int w = 0, h = 0; // its size, if not the preview's (PrevW x PrevH): draw_made's only
    unsigned bg = 0xFF211E1Eu; // what's behind the hero (draw_made's; Graphite's picture, #1E1E21), see-through for the strip's
    // looked at from above by pitch degrees, and zoomed about (zx, zy): dragged and scrolled in the views that allow it (look_around);
    // and moved across by (px, py) of its height (draw_made's: the card's camera, right-dragged)
    float pitch = 0, zoom = 1, zx = 0, zy = 0, px = 0, py = 0;
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
    now.add(v.px), now.add(v.py);
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
    // (the mod it's saved in is called as the hero is: the user's word, 2026-11-22, "The mod name should always be the same as the
    // custom hero name")
    char id[32] = "myhero", name[32] = "MY HERO", author[64] = "", card[1024] = "";
    char voice[PACK_VOICE_JOBS][1024] = {};
    int height = 560, voice_job = 1, mode = 0, template_hero = 0; // template_hero: who it's based on (its template and its place)
    int mode_page = -100; // the page Preview in last followed (mode: Modes')
    // on foot its own moves (its model's clips, and the tool's moves for the rest), not those of who it's based on: a pack that says no
    // template (Bobby) is so, and is saved so, as it was (the user's word, 2026-11-22)
    bool foot_own = false;
    float face_head = 0;
    char out[1024] = "", game[1024] = "";
    // a marker being dragged: its bone, whether it's the bone's end, and in which view
    int drag_bone = -1, drag_end = 0, drag_view = 0;
    // the joints' view being moved with the right button (-1 none)
    int pan_view = -1;
    // the joints picked to move together (each a bone and its start 0 or end 1: the user's word, 2026-11-23, "if i click and drag i should
    // be able to move many of the bones at the same time"), the box picking them being drawn (its view, -1 none, and where it began),
    // and whether they're being moved as one
    std::vector<std::pair<int, int>> joint_sel;
    int jbox_view = -1;
    float jbox_x = 0, jbox_y = 0;
    bool multi_drag = false;
    // the joints' undo and redo (Hero Setup's own, as a game's page's are its keys'): each the joints, arm lift and rig as they were
    // before a change, and what the change was
    struct JointStep {
        KitBone bones[KB_COUNT];
        float arm_raise;
        bool rigged;
        std::string says;
    };
    std::vector<JointStep> joint_undo, joint_redo;
    // the joints' grid on, and Guess joints being asked
    bool joint_grid = true, ask_guess = false;
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
    // each game it isn't drawn in to sit in (a tank's hatch, a race cart; hero.json's kind's "fit": false), Fit in seat off: its own
    // keys fit it, hiding bones (the user's word, 2026-11-22)
    bool no_fit[PACK_KINDS] = {};
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
    // left out), the move watched, and a preview asked for while one was being made
    std::vector<BakedMove> baked;
    std::vector<std::string> foot_back;
    char watching[32] = ""; // (and the move watched, kept as it's made again)
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
    // Stretch: Move slides a bone, the one it hangs from stretching to it; off, it turns the one it hangs from instead, every bone keeping
    // its length (the user's word, 2026-11-22: "a non stretch toggle so that bones can only move and not extend or shirnk")
    bool stretch = true;
    // how a new key fades (PACK_EASE_), and the key copied (Ctrl+C; empty none)
    int ease = PACK_EASE_SMOOTH;
    std::vector<PackAdjust> clip;
    int ask_kind = -1, ask_move = -1, ask_to = OWN_TEMPLATE;
    char ask_name[32] = "";
    bool ask_open = false;
    // Graphite (the user's pick, 2026-11-22): the bones shown on the picture (Bones, Tab; off, a clean view of the hero), the keys and
    // mouse card open, a short message over the picture (toast) and the time it was said, the status last made one, whether anything's
    // changed since it was saved, and each game's picture in the strip of them (a still of its first animation)
    bool bones = true, help = false, unsaved = false;
    // a new hero's first question being asked: who it's based on (base_chooser)
    bool choosing = false;
    // the game's mod holding the hero open, if it's in the game (its mods folder: Add to game puts it there); its own copy's the tool's
    // (loaded_mod, saved in the tool's folder, heroes_home)
    char game_mod[1100] = "";
    // the strip of games under the top bar shown (the user's word, 2026-11-22: "add a button to hide the top bar as a toggle"; kept for
    // next time, strip.txt beside the sounds')
    bool strip_shown = true;
    char toast[400] = "", toasted[2400] = "";
    double toast_at = -10;
    View strip[PACK_KINDS], face_view;
    // the card's camera (the picture on the hero's page as one: its view, and the move and frame it's posed in), whether the card is a
    // photo it took, and the card's picture (its PNG, read again when it's another or a new photo)
    bool camera = false, card_photo = false;
    View cam;
    int cam_anim = -1, cam_frame = 0;
    SDL_Texture *card_tex = nullptr;
    char card_tex_of[1024] = "";
    unsigned card_gen = 0, card_tex_gen = 0;
    int card_tw = 0, card_th = 0;
};
enum { KEY_FRAME, KEY_MOVE, KEY_GAME };
enum { GIZMO_TURN, GIZMO_MOVE, GIZMO_SIZE };
enum { KEY_BODY, KEY_START, KEY_END, KEY_EITHER };

// --- Graphite's widgets: line icons, round buttons that lift when the mouse is on them and squash when pressed, toggles that turn
// periwinkle when on, switches whose knob slides, cards, and a short message that fades in and out; each pressed makes its sound ---

// a and b mixed, t of the way to b
ImU32 mix(ImU32 a, ImU32 b, float t) {
    t = t < 0 ? 0 : t > 1 ? 1 : t;
    ImU32 out = 0;
    for (int sh = 0; sh < 32; sh += 8) {
        float x = (float)(a >> sh & 255), y = (float)(b >> sh & 255);
        out |= (ImU32)(x + (y - x) * t + 0.5f) << sh;
    }
    return out;
}

// a value kept for an id that eases towards to (a slide, a fade), rate an inverse time
float ease_to(ImGuiID id, float to, float rate = 14) {
    float *v = ImGui::GetStateStorage()->GetFloatRef(id, to);
    *v += (to - *v) * (1 - expf(-rate * ImGui::GetIO().DeltaTime));
    *v = fabsf(*v - to) < 0.002f ? to : *v;
    return *v;
}

enum {
    IC_BACK, IC_UNDO, IC_REDO, IC_SOUND, IC_MUTE, IC_GRID, IC_PLAY, IC_PAUSE, IC_PREV, IC_NEXT, IC_EYE, IC_EYE_OFF, IC_BONES,
    IC_ROTATE, IC_MOVE, IC_SCALE, IC_MIRROR, IC_ONION, IC_CLIP, IC_GAMEPAD, IC_HISTORY, IC_DOWN, IC_PERSON, IC_CUBE, IC_FOLDER,
    IC_REFRESH, IC_CHECK, IC_SPARK, IC_CURSOR, IC_RIGHT, IC_CAMERA, IC_UP, IC_TRASH, IC_NONE = -1
};

// An icon, as the mocks' line icons: drawn in a 24-square (as an SVG's), s across, about c
void icon(ImDrawList *dl, int which, ImVec2 c, float s, ImU32 col) {
    float k = s / 24, w = fmaxf(1.2f, s * 0.085f);
    auto P = [&](float x, float y) { return ImVec2(c.x + (x - 12) * k, c.y + (y - 12) * k); };
    auto L = [&](float x0, float y0, float x1, float y1) { dl->AddLine(P(x0, y0), P(x1, y1), col, w); };
    auto poly = [&](std::initializer_list<float> xy, bool closed = false) {
        ImVec2 pts[16];
        int n = 0;
        for (auto it = xy.begin(); it != xy.end() && n < 16; it += 2) {
            pts[n++] = P(*it, *(it + 1));
        }
        dl->AddPolyline(pts, n, col, closed ? ImDrawFlags_Closed : 0, w);
    };
    auto ring = [&](float x, float y, float r) { dl->AddCircle(P(x, y), r * k, col, 0, w); };
    auto arc = [&](float x, float y, float r, float a0, float a1) {
        dl->PathArcTo(P(x, y), r * k, a0 * 3.14159265f / 180, a1 * 3.14159265f / 180, 24);
        dl->PathStroke(col, 0, w);
    };
    switch (which) {
    case IC_BACK: poly({ 15, 18, 9, 12, 15, 6 }); break;
    case IC_RIGHT: poly({ 9, 6, 15, 12, 9, 18 }); break;
    case IC_DOWN: poly({ 6, 9, 12, 15, 18, 9 }); break;
    case IC_UP: poly({ 6, 15, 12, 9, 18, 15 }); break;
    case IC_TRASH:
        L(4, 7, 20, 7);
        poly({ 9, 7, 9, 4.5f, 15, 4.5f, 15, 7 });
        poly({ 6, 7, 7.2f, 20, 16.8f, 20, 18, 7 });
        L(10, 10.5f, 10, 16.5f);
        L(14, 10.5f, 14, 16.5f);
        break;
    case IC_UNDO: poly({ 9, 14, 4, 9, 9, 4 }); L(4, 9, 14, 9); arc(14, 15, 6, -90, 90); L(14, 21, 11, 21); break;
    case IC_REDO: poly({ 15, 14, 20, 9, 15, 4 }); L(20, 9, 10, 9); arc(10, 15, 6, 270, 90); L(10, 21, 13, 21); break;
    case IC_SOUND:
    case IC_MUTE:
        poly({ 4, 9, 8, 9, 13, 5, 13, 19, 8, 15, 4, 15 }, true);
        if (which == IC_SOUND) {
            arc(13.4f, 12, 4, -48, 48);
            arc(12.7f, 12, 8, -43, 43);
        } else {
            L(17, 9, 22, 15);
            L(22, 9, 17, 15);
        }
        break;
    case IC_GRID:
        for (int i = 0; i < 4; i++) {
            float x = i % 2 ? 13 : 4, y = i / 2 ? 13 : 4;
            dl->AddRect(P(x, y), P(x + 7, y + 7), col, 2 * k, 0, w);
        }
        break;
    case IC_PLAY: dl->AddTriangleFilled(P(7, 5), P(19, 12), P(7, 19), col); break;
    case IC_PAUSE: dl->AddRectFilled(P(6.5f, 5), P(10, 19), col, 1.5f * k); dl->AddRectFilled(P(14, 5), P(17.5f, 19), col, 1.5f * k); break;
    case IC_PREV: poly({ 18, 6, 10, 12, 18, 18 }); L(6, 6, 6, 18); break;
    case IC_NEXT: poly({ 6, 6, 14, 12, 6, 18 }); L(18, 6, 18, 18); break;
    case IC_EYE:
    case IC_EYE_OFF:
        dl->AddBezierQuadratic(P(2, 12), P(12, 2), P(22, 12), col, w);
        dl->AddBezierQuadratic(P(2, 12), P(12, 22), P(22, 12), col, w);
        ring(12, 12, 3);
        if (which == IC_EYE_OFF) {
            L(3, 3, 21, 21);
        }
        break;
    case IC_BONES:
        L(12, 6.5f, 12, 10);
        L(10.6f, 13.4f, 7.2f, 17.8f);
        L(13.4f, 13.4f, 16.8f, 17.8f);
        ring(12, 4.5f, 2);
        ring(12, 12, 2);
        ring(6, 19.5f, 2);
        ring(18, 19.5f, 2);
        break;
    case IC_ROTATE:
    case IC_REFRESH: arc(12, 12, 8, 0, 315); poly({ 20, 4, 20, 9, 15, 9 }); break;
    case IC_MOVE:
        L(12, 3, 12, 21);
        L(3, 12, 21, 12);
        poly({ 9, 6, 12, 3, 15, 6 });
        poly({ 9, 18, 12, 21, 15, 18 });
        poly({ 6, 9, 3, 12, 6, 15 });
        poly({ 18, 9, 21, 12, 18, 15 });
        break;
    case IC_SCALE: poly({ 4, 14, 4, 20, 10, 20 }); poly({ 20, 10, 20, 4, 14, 4 }); L(4, 20, 11, 13); L(20, 4, 13, 11); break;
    case IC_MIRROR: L(12, 3, 12, 21); poly({ 8, 7, 3, 12, 8, 17 }); poly({ 16, 7, 21, 12, 16, 17 }); break;
    case IC_ONION: poly({ 12, 4, 20, 8, 12, 12, 4, 8 }, true); poly({ 4, 12, 12, 16, 20, 12 }); poly({ 4, 16, 12, 20, 20, 16 }); break;
    case IC_CLIP: ring(9, 12, 5); ring(15, 12, 5); break;
    case IC_GAMEPAD:
        dl->AddRect(P(2, 6), P(22, 17), col, 5 * k, 0, w);
        L(6, 11.5f, 10, 11.5f);
        L(8, 9.5f, 8, 13.5f);
        dl->AddCircleFilled(P(16, 10.5f), 1.2f * k, col);
        dl->AddCircleFilled(P(18, 12.5f), 1.2f * k, col);
        break;
    case IC_HISTORY: arc(12, 12, 9, 180, -132); poly({ 3, 4, 3, 9, 8, 9 }); poly({ 12, 8, 12, 12, 15, 14 }); break;
    case IC_PERSON: ring(12, 8, 4); arc(12, 21, 8, 180, 360); break;
    case IC_CUBE:
        poly({ 12, 3, 20, 7.5f, 20, 16.5f, 12, 21, 4, 16.5f, 4, 7.5f }, true);
        L(12, 12, 20, 7.5f);
        L(12, 12, 12, 21);
        L(12, 12, 4, 7.5f);
        break;
    case IC_FOLDER: poly({ 3, 7, 3, 18, 21, 18, 21, 9, 11, 9, 9, 6, 3, 6 }, true); break;
    case IC_CAMERA:
        poly({ 3, 8, 3, 19, 21, 19, 21, 8, 16, 8, 14.5f, 5, 9.5f, 5, 8, 8 }, true);
        ring(12, 13.5f, 3.6f);
        break;
    case IC_CHECK: poly({ 5, 12, 10, 17, 19, 7 }); break;
    case IC_SPARK: poly({ 13, 3, 15, 9, 21, 11, 15, 13, 13, 19, 11, 13, 5, 11, 11, 9 }, true); break;
    case IC_CURSOR: poly({ 5, 3, 19, 11, 13, 12.5f, 10, 19 }, true); break;
    }
}

// the text of a label (to its ##, if any)
const char *label_end(const char *label) {
    const char *e = strstr(label, "##");
    return e != nullptr ? e : label + strlen(label);
}

// A round button: its colours, an icon before its words (or alone), w wide (0: as its words); with a shadow under it like a key's
bool button(const char *label, ImU32 bg = RAISED, ImU32 ink = INK, float w = 0, int ic = IC_NONE, int cue = CUE_TAP, float h = 0) {
    const char *end = label_end(label);
    ImVec2 ts = ImGui::CalcTextSize(label, end);
    float icon_w = ic != IC_NONE ? U(18) : 0, gap = ic != IC_NONE && end > label ? U(8) : 0;
    h = h > 0 ? h : ImGui::GetFrameHeight() + U(6);
    w = w > 0 ? w : ts.x + icon_w + gap + (end > label ? U(32) : 0) + (end == label ? h : 0);
    ImVec2 p = ImGui::GetCursorScreenPos();
    bool pressed = ImGui::InvisibleButton(label, ImVec2(w, h));
    bool hov = ImGui::IsItemHovered(), held = ImGui::IsItemActive();
    ImGuiID id = ImGui::GetItemID();
    float lift = ease_to(id, hov && !held ? 1 : 0, 18), squash = ease_to(id + 1, held ? 1 : 0, 30);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 a(p.x + squash * U(2), p.y - lift * U(2) + squash * U(2)), b(p.x + w - squash * U(2), p.y + h - lift * U(2) - squash * U(1));
    // (its colours through the style, so a disabled one fades)
    dl->AddRectFilled(ImVec2(a.x, a.y + U(3)), ImVec2(b.x, b.y + U(3)), ImGui::GetColorU32(mix(bg, SHADOW, 0.6f)), h / 2);
    dl->AddRectFilled(a, b, ImGui::GetColorU32(hov ? mix(bg, INK, 0.08f) : bg), h / 2);
    float x = (a.x + b.x) / 2 - (ts.x + icon_w + gap) / 2, my = (a.y + b.y) / 2;
    if (ic != IC_NONE) {
        icon(dl, ic, ImVec2(x + icon_w / 2, my), U(18), ImGui::GetColorU32(ink));
    }
    dl->AddText(ImVec2(x + icon_w + gap, my - ts.y / 2), ImGui::GetColorU32(ink), label, end);
    if (pressed) {
        sound_cue(cue);
    }
    return pressed;
}

// A round button that's only an icon (its words its tooltip)
bool icon_button(const char *id, int ic, const char *tip, ImU32 bg = CARD, ImU32 ink = INK, float size = 0, int cue = CUE_TAP) {
    size = size > 0 ? size : ImGui::GetFrameHeight() + U(6);
    bool pressed = button(id, bg, ink, size, ic, cue, size);
    if (tip != nullptr) {
        ImGui::SetItemTooltip("%s", tip);
    }
    return pressed;
}

// A toggle: periwinkle when on (fading to it), plain when off; pressed, it turns over (two notes up, or down)
bool toggle(const char *label, bool *on, int ic, const char *tip, float w = 0, ImU32 off_bg = RAISED) {
    ImGuiID id = ImGui::GetID(label);
    float t = ease_to(id + 7, *on ? 1 : 0, 16);
    bool pressed = button(label, mix(off_bg, PERI, t), mix(INK, PERI_INK, t), w, ic, *on ? CUE_OFF : CUE_ON);
    if (tip != nullptr) {
        ImGui::SetItemTooltip("%s", tip);
    }
    if (pressed) {
        *on = !*on;
    }
    return pressed;
}

// A switch: its knob slides across, its track periwinkle when on
bool switch_(const char *id, bool *on, const char *tip) {
    float w = U(50), h = U(30);
    ImVec2 p = ImGui::GetCursorScreenPos();
    bool pressed = ImGui::InvisibleButton(id, ImVec2(w, h));
    if (pressed) {
        *on = !*on;
        sound_cue(*on ? CUE_ON : CUE_OFF);
    }
    float t = ease_to(ImGui::GetItemID(), *on ? 1 : 0, 16);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), mix(CARD, PERI, t), h / 2);
    dl->AddCircleFilled(ImVec2(p.x + h / 2 + t * (w - h), p.y + h / 2), h / 2 - U(4), mix(INK, PERI_INK, t));
    if (tip != nullptr) {
        ImGui::SetItemTooltip("%s", tip);
    }
    return pressed;
}

// A card: a rounded block of its colour holding what's put in it till card_end (as high as that)
void card(const char *id, ImU32 bg, float w = 0) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, V4(bg));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(18), U(16)));
    ImGui::BeginChild(id, ImVec2(w, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void card_end() {
    ImGui::EndChild();
}

// A card's title (a dot before it, if dot isn't 0)
void heading(const char *text, ImU32 dot = 0) {
    ImGui::PushFont(FontTitle);
    if (dot != 0) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        float h = ImGui::GetTextLineHeight();
        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + U(5), p.y + h / 2), U(5), dot);
        ImGui::Dummy(ImVec2(U(10), h));
        ImGui::SameLine(0, U(8));
    }
    ImGui::TextUnformatted(text);
    ImGui::PopFont();
}

// A menu's item: periwinkle with dark words when it's the one picked
bool menu_item(const char *label, bool picked) {
    if (picked) {
        ImGui::PushStyleColor(ImGuiCol_Header, V4(PERI));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, V4(PERI));
        ImGui::PushStyleColor(ImGuiCol_Text, V4(PERI_INK));
    }
    bool hit = ImGui::Selectable(label, picked);
    if (picked) {
        ImGui::PopStyleColor(3);
    }
    if (hit) {
        sound_cue(CUE_PICK);
    }
    return hit;
}

// A drop-down's box: round, dark, its arrow in it
bool menu_begin(const char *id, const char *shown, float w = 0, ImU32 bg = DEEP) {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(bg));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, V4(mix(bg, INK, 0.08f)));
    ImGui::PushStyleColor(ImGuiCol_Button, V4(bg));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4(mix(bg, INK, 0.08f)));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(U(14), U(9)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(8), U(8)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(U(4), U(4)));
    if (w != 0) {
        ImGui::SetNextItemWidth(w);
    }
    bool open = ImGui::BeginCombo(id, shown);
    ImGui::PopStyleVar(2);
    if (open) {
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
        static ImGuiID last;
        if (last != ImGui::GetID(id)) {
            sound_cue(CUE_OPEN);
            last = ImGui::GetID(id);
        }
        return true;
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
    return false;
}

// A label's pill that isn't pressed: its colours, a dot before it
void tag(const char *text, ImU32 bg, ImU32 ink, ImU32 dot = 0, float h = 0) {
    ImGui::PushFont(FontSmall);
    ImVec2 ts = ImGui::CalcTextSize(text), p = ImGui::GetCursorScreenPos();
    h = h > 0 ? h : U(28);
    float w = ts.x + (dot != 0 ? U(34) : U(24));
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), bg, h / 2);
    if (dot != 0) {
        dl->AddCircleFilled(ImVec2(p.x + U(15), p.y + h / 2), U(4), dot);
    }
    dl->AddText(ImVec2(p.x + (dot != 0 ? U(24) : U(12)), p.y + (h - ts.y) / 2), ink, text);
    ImGui::Dummy(ImVec2(w, h));
    ImGui::PopFont();
}

// the width a button() of these words (and an icon) takes
float button_w(const char *label, bool has_icon) {
    return ImGui::CalcTextSize(label, label_end(label)).x + (has_icon ? U(26) : 0) + U(32);
}

// A short message over the picture (the status said once: saved, made, what went wrong), fading in and out
void toast(Tool &T, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(T.toast, sizeof(T.toast), fmt, ap);
    va_end(ap);
    T.toast_at = ImGui::GetTime();
}

void draw_toast(Tool &T, ImVec2 top_right, float wrap) {
    double age = ImGui::GetTime() - T.toast_at, last = 2.6 + strlen(T.toast) * 0.02;
    if (T.toast[0] == 0 || age > last + 0.3) {
        return;
    }
    float a = (float)(age < 0.25 ? age / 0.25 : age > last ? 1 - (age - last) / 0.3 : 1), slide = (1 - a) * U(16);
    ImDrawList *dl = ImGui::GetForegroundDrawList();
    ImVec2 ts = ImGui::CalcTextSize(T.toast, nullptr, false, wrap);
    float w = ts.x + U(64), h = ts.y + U(22);
    ImVec2 lo(top_right.x - w + slide, top_right.y), hi(top_right.x + slide, top_right.y + h);
    auto al = [&](ImU32 c) { return (c & 0x00FFFFFFu) | (ImU32)(a * ((c >> 24) & 255)) << 24; };
    dl->AddRectFilled(ImVec2(lo.x, lo.y + U(4)), ImVec2(hi.x, hi.y + U(4)), al(SHADOW), h / 2 < U(24) ? h / 2 : U(24));
    dl->AddRectFilled(lo, hi, al(INK), h / 2 < U(24) ? h / 2 : U(24));
    ImVec2 c(lo.x + U(26), (lo.y + hi.y) / 2);
    dl->AddCircleFilled(c, U(13), al(MINT));
    icon(dl, IC_CHECK, c, U(16), al(IM_COL32(0x17, 0x3A, 0x2C, 255)));
    dl->AddText(nullptr, 0, ImVec2(lo.x + U(48), lo.y + U(11)), al(PERI_INK), T.toast, nullptr, wrap);
}

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

const char *const HeroNames[] = { "Crash", "Coco", "Cortex", "Brio", "Tiny", "Koala Kong", "Dingodile", "Rilla Roo" };
// How tall each stands, in the game's units: its model on foot's (port/game/main/packtemplates.c, PackTemplateHeight, 1 Crash to 8
// Rilla Roo there)
const int HeroHeights[] = { 452, 627, 440, 606, 724, 666, 697, 733 };
const char *const Heroes[] = { "crash", "coco", "cortex", "brio", "tiny", "kong", "dingodile", "rilla" };
// The games Preview in can start (the game's mode, main.c's level names in order), each with the kind of model it shows: a game's page
// picks its own (the user's word, 2026-11-22: "preview in x should changed based on what is open"), the arrow any
const struct {
    const char *name;
    int mode, kind;
} Modes[] = { { "Crate Crush", 0, 0 },  { "Ring Ding", 27, 0 },   { "Polar Push", 18, 1 },  { "Pogo Painter", 10, 2 },
              { "Ballistix", 14, 3 },   { "Tank Wars", 6, 4 },    { "Crash Dash", 23, 5 },  { "Splash Dash", 25, 6 },
              { "Mallet Mash", 29, 7 }, { "Dragon Drop", 28, 8 }, { "Keg Kaboom", 30, 9 }, { "Tournament podium", 43, 10 } };

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

int vw(const View &v) {
    return v.w > 0 ? v.w : ViewW;
}

int vh(const View &v) {
    return v.h > 0 ? v.h : ViewH;
}

void fit(const KitModel *m, View &v) {
    float h = m->hi[1] - m->lo[1], w = fmaxf(m->hi[0] - m->lo[0], m->hi[2] - m->lo[2]);

    v.scale = fminf(vh(v) * 0.85f / h, vw(v) * 0.85f / (w > 0 ? w : 1));
    v.ox = vw(v) / 2.0f;
    v.oy = vh(v) * 0.93f + m->lo[1] * v.scale;
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
        ImGui::SetItemTooltip("Right-drag: orbit   Wheel: zoom   Home: reset");
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
    const int PW = vw(v), PH = vh(v);
    if (v.rgba == nullptr) {
        v.rgba = new unsigned[PW * PH];
        v.depth = new float[PW * PH];
        v.tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, PW, PH);
    }
    DrawnFrom now;
    now.add(m), now.add(m->ntri), now.add(m->nvert), now.add(v.scale), now.add(v.ox), now.add(v.oy), now.add(PW), now.add(PH);
    now.add(pos, sizeof(float) * 3 * (size_t)m->nvert);
    if (drawn_same(v, now)) {
        return;
    }
    float c = cosf(v.yaw * 3.14159265f / 180), s = sinf(v.yaw * 3.14159265f / 180);
    in_bands(PW, PH, [&](int ya, int yb) {
    std::fill(v.rgba + ya * PW, v.rgba + yb * PW, 0xFF262222u); // (Graphite: #222226)
    std::fill(v.depth + ya * PW, v.depth + yb * PW, 1e30f);
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
        fill_tri(sx, sy, sz, v.depth, PW, PH, ya, yb, [&](int at, float a, float b, float d, float z) {
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
    SDL_UpdateTexture(v.tex, nullptr, v.rgba, PW * 4);
}

// --- the model ---

void reweigh(Tool &T) {
    if (!T.rigged) {
        kit_weigh(T.m);
    }
}

void own_init(Tool &T);
int own_clip_of(const Tool &T, int g, int m);

bool open_model(Tool &T, const char *path) {
    char why[512];
    KitModel *m = new KitModel();

    if (!kit_read(path, m, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
        kit_free(m);
        delete m;
        return false;
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
    T.joint_undo.clear();
    T.joint_redo.clear();
    T.joint_sel.clear();
    T.baked.clear();
    T.foot_back.clear();
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
    // (its joints to place first if it has no rig of its own; Joints is there for any, the user's word, 2026-11-23: "That should always
    // be available")
    T.model_view = T.rigged ? 1 : 0;
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
    snprintf(T.status, sizeof(T.status), "%s: %d triangles, %d vertices, %s", base, m->ntri, m->nvert,
             T.rigged ? "rigged" : "no rig (place its joints)");
    return true;
}

// The game started, to make the preview or to play: on Windows in the background, so a game built with a console doesn't open one beside
// the tool (a window program, it has none for the game to share: the user's word, 2026-11-23, "when it opens a command window opens with
// it can that not happen?"); its end is still seen, not what it said at it. Elsewhere as it is (in the background there it couldn't be
// waited for)
SDL_Process *start_game(const char *const *args) {
#ifdef _WIN32
    SDL_PropertiesID p = SDL_CreateProperties();
    SDL_SetPointerProperty(p, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, (void *)args);
    SDL_SetBooleanProperty(p, SDL_PROP_PROCESS_CREATE_BACKGROUND_BOOLEAN, true);
    SDL_Process *proc = SDL_CreateProcessWithProperties(p);
    SDL_DestroyProperties(p);
    return proc;
#else
    return SDL_CreateProcess(args, false);
#endif
}

bool heroes_home(char *out, size_t size);
const char *short_path(const char *path);
bool remove_tree(const char *path);

bool export_pack(Tool &T, bool test, const char *out = nullptr) {
    char why[512], markers[1100];
    KitModel *t = new KitModel();
    KitPack p = {};
    bool ok = false;

    if (T.m == nullptr) {
        delete t;
        return false;
    }
    // (Save: in the tool's own folder, the mod it came from's name kept, else its id; a test or the preview's folder: by its id there)
    char home[1100];
    bool saving = out == nullptr;
    if (saving) {
        heroes_home(home, sizeof(home));
        out = home;
    }
    if (!kit_read(T.model, t, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
        delete t;
        return false;
    }
    if (T.rigged) {
        kit_map_rig(t, why, sizeof(why));
    } else {
        // (a model with a rig of its own whose joints were placed by hand: its rig mapped still, as its own clips go by it, then the
        // joints placed and the tool's weights)
        if (t->skinned) {
            char rig_why[256];
            kit_map_rig(t, rig_why, sizeof(rig_why));
        }
        memcpy(t->bones, T.m->bones, sizeof(t->bones));
        t->arm_raise = T.m->arm_raise;
        kit_weigh(t);
        // (its joints aren't written beside the model it's made from: all it is is in its pack, its hero.glb's skeleton; the user's word,
        // 2026-11-23, "when we save we seem to generate a file next to the original model. I think it should just be all self contained")
        (void)markers;
    }
    t->arm_raise = T.m->arm_raise;
    p.id = T.id;
    p.name = T.name;
    p.mod_name = T.name;
    p.author = T.author;
    p.prefer = Heroes[T.template_hero]; // who it's based on: whose place it takes, and its template (one choice, the user's word, 2026-11-22)
    p.template_hero = Heroes[T.template_hero];
    for (int k = 0; k < PACK_KINDS; k++) {
        p.kind_template[k] = T.kind_template[k] >= 0 && T.kind_template[k] < 8 ? Heroes[T.kind_template[k]] : nullptr;
        p.no_fit[k] = T.no_fit[k];
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
    if (saving) {
        snprintf(folder, sizeof(folder), "%s/%s", out, T.loaded_mod[0] != 0 ? short_path(T.loaded_mod) : T.id);
    } else {
        snprintf(folder, sizeof(folder), "%s/%s", out, T.id);
    }
    SDL_CreateDirectory(folder);
    if (!kit_atlas(t, why, sizeof(why)) || !kit_write(t, &p, folder, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
    } else {
        ok = true;
        snprintf(T.status, sizeof(T.status), "Saved to %s", folder);
        // (saved only by a save: the copy the games make the preview from, or a test's, isn't it; written there, it was taken for saved,
        // All saved and Add to game with it, though the tool's own copy was as it had been)
        if (saving) {
            T.unsaved = false;
            // (the tool's copy is the one it's opened from now)
            SDL_strlcpy(T.loaded_mod, folder, sizeof(T.loaded_mod));
        }
        if (test) {
            char mode[16];
            snprintf(mode, sizeof(mode), "%d", Modes[T.mode].mode);
            // (straight into the game, its launcher skipped: the user's word, 2026-11-22, "open the mini game directly rather than
            // needing the user to click play")
            // (the tool's own copy, just saved: it needn't be in the game to be seen in it)
            const char *args[] = { T.game, "--mods-dir", out, "--mod", short_path(folder), "--extra-hero", "0", T.id, "--mode", mode,
                                   "--skip-intro", "--skip-loading", "--no-launcher", nullptr };
            SDL_Process *proc = start_game(args);

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

// A folder and all in it copied to another (made); false if any of it wasn't
SDL_EnumerationResult SDLCALL copy_in(void *to, const char *dir, const char *name);
bool copy_tree(const char *from, const char *to) {
    SDL_PathInfo info;
    if (!SDL_GetPathInfo(from, &info)) {
        return false;
    }
    if (info.type != SDL_PATHTYPE_DIRECTORY) {
        return SDL_CopyFile(from, to);
    }
    SDL_CreateDirectory(to);
    struct Pair {
        const char *to;
        bool ok;
    } pr = { to, true };
    SDL_EnumerateDirectory(from, copy_in, &pr);
    return pr.ok;
}

SDL_EnumerationResult SDLCALL copy_in(void *ctx, const char *dir, const char *name) {
    struct Pair {
        const char *to;
        bool ok;
    } *pr = (Pair *)ctx;
    char a[1400], b[1400];
    snprintf(a, sizeof(a), "%s%s", dir, name);
    snprintf(b, sizeof(b), "%s/%s", pr->to, name);
    if (!copy_tree(a, b)) {
        pr->ok = false;
    }
    return SDL_ENUM_CONTINUE;
}

// Add to game (the user's word, 2026-11-23: Save keeps it "in the tool", and a button "installs"): once it's saved, its hero's folder put
// in the game's mods folder (MOD/heroes/ID in place of what was there, and the mod's mod.json if it's new there: another hero of the mod's
// there stays), and the game asked to switch the mod on (crashbash --add-hero MOD: a hero mod that's on puts its heroes in the select).
// Said in the status
// Whether the hero open is saved in the tool's own folder as it is now (Add to game puts that copy in the game)
bool saved_in_tool(const Tool &T) {
    char home[1100];
    heroes_home(home, sizeof(home));
    return !T.unsaved && T.loaded_mod[0] != 0 && home[0] != 0 && SDL_strncmp(T.loaded_mod, home, strlen(home)) == 0;
}

bool add_to_game(Tool &T) {
    // (its saved copy: the button's greyed out till it's saved, the user's word, 2026-11-23)
    if (!saved_in_tool(T)) {
        snprintf(T.status, sizeof(T.status), "Save it first: Add to game puts its saved copy in the game");
        return false;
    }
    char mod[128], from[1400], dest[1100], to[1400], path[1400], back[1400];
    SDL_strlcpy(mod, short_path(T.loaded_mod), sizeof(mod));
    if (T.game_mod[0] != 0) {
        SDL_strlcpy(dest, T.game_mod, sizeof(dest));
    } else {
        snprintf(dest, sizeof(dest), "%s/%s", T.out, mod);
    }
    SDL_CreateDirectory(T.out);
    SDL_CreateDirectory(dest);
    snprintf(path, sizeof(path), "%s/heroes", dest);
    SDL_CreateDirectory(path);
    snprintf(from, sizeof(from), "%s/heroes/%s", T.loaded_mod, T.id);
    snprintf(to, sizeof(to), "%s/heroes/%s", dest, T.id);
    // (the game's copy kept aside till the new one's in, then let go of)
    snprintf(back, sizeof(back), "%s.was", to);
    remove_tree(back);
    SDL_PathInfo pi;
    bool had = SDL_GetPathInfo(to, &pi);
    if (had && !SDL_RenamePath(to, back)) {
        snprintf(T.status, sizeof(T.status), "It isn't in the game: %s couldn't be changed (is the game open?)", to);
        return false;
    }
    if (!copy_tree(from, to)) {
        remove_tree(to);
        if (had) {
            SDL_RenamePath(back, to);
        }
        snprintf(T.status, sizeof(T.status), "It isn't in the game: it couldn't be copied to %s", to);
        return false;
    }
    remove_tree(back);
    snprintf(path, sizeof(path), "%s/mod.json", dest);
    if (!SDL_GetPathInfo(path, &pi)) {
        snprintf(from, sizeof(from), "%s/mod.json", T.loaded_mod);
        SDL_CopyFile(from, path);
    }
    SDL_strlcpy(T.game_mod, dest, sizeof(T.game_mod));
    // the game switches it on in its own settings (where they are is its to know), and says so
    const char *args[] = { T.game, "--add-hero", mod, nullptr };
    SDL_Process *proc = start_game(args);
    if (proc == nullptr) {
        snprintf(T.status, sizeof(T.status), "In the game's mods folder, but the game didn't start to switch it on (%s): is it at %s?", SDL_GetError(),
                 T.game);
        return false;
    }
    int code = 0;
    SDL_WaitProcess(proc, true, &code);
    SDL_DestroyProcess(proc);
    snprintf(T.status, sizeof(T.status), "In the game: %s", to);
    return true;
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

// Its joints as they are (before a change), a step of their undo history
void joint_snapshot(Tool &T, const char *says, ...) {
    if (T.m == nullptr) {
        return;
    }
    Tool::JointStep st;
    memcpy(st.bones, T.m->bones, sizeof(st.bones));
    st.arm_raise = T.m->arm_raise;
    st.rigged = T.rigged;
    char line[160];
    va_list ap;
    va_start(ap, says);
    vsnprintf(line, sizeof(line), says, ap);
    va_end(ap);
    st.says = line;
    T.joint_undo.push_back(st);
    if (T.joint_undo.size() > 300) {
        T.joint_undo.erase(T.joint_undo.begin());
    }
    T.joint_redo.clear();
    T.unsaved = true;
}

// Its joints put back as a step had them: its rig's own weights again if it was rigged then, else the tool's to the joints
void joint_restore(Tool &T, const Tool::JointStep &st) {
    memcpy(T.m->bones, st.bones, sizeof(st.bones));
    T.m->arm_raise = st.arm_raise;
    if (st.rigged && !T.rigged) {
        char why[256];
        T.rigged = kit_map_rig(T.m, why, sizeof(why));
        memcpy(T.m->bones, st.bones, sizeof(st.bones));
    } else if (!st.rigged) {
        T.rigged = false;
        kit_weigh(T.m);
    }
    T.dirty = T.unsaved = true;
}

// A joints' step undone (its state now kept to be done again), or done again
void joint_undo_step(Tool &T) {
    if (T.m == nullptr || T.joint_undo.empty()) {
        return;
    }
    Tool::JointStep now;
    memcpy(now.bones, T.m->bones, sizeof(now.bones));
    now.arm_raise = T.m->arm_raise, now.rigged = T.rigged, now.says = T.joint_undo.back().says;
    T.joint_redo.push_back(now);
    Tool::JointStep st = T.joint_undo.back();
    T.joint_undo.pop_back();
    joint_restore(T, st);
    snprintf(T.status, sizeof(T.status), "Undone: %s", st.says.c_str());
}

void joint_redo_step(Tool &T) {
    if (T.m == nullptr || T.joint_redo.empty()) {
        return;
    }
    Tool::JointStep now;
    memcpy(now.bones, T.m->bones, sizeof(now.bones));
    now.arm_raise = T.m->arm_raise, now.rigged = T.rigged, now.says = T.joint_redo.back().says;
    T.joint_undo.push_back(now);
    Tool::JointStep st = T.joint_redo.back();
    T.joint_redo.pop_back();
    joint_restore(T, st);
    snprintf(T.status, sizeof(T.status), "Done again: %s", st.says.c_str());
}

// A new hero from a model, the start screen's (the user's word, 2026-11-23, on starting one): nothing of the last one's kept (its card,
// sounds, changes and their history, its own moves, what the games made of it), its page Hero Setup, and who it's based on asked first
// (base_chooser: it starts their height). False if the model didn't open (why's the status)
bool new_hero(Tool &T, const char *path) {
    T.loaded_mod[0] = 0;
    T.game_mod[0] = 0;
    if (!open_model(T, path)) {
        return false;
    }
    made_forget(T);
    T.card[0] = 0;
    T.card_photo = false;
    T.camera = false;
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        T.voice[j][0] = 0;
    }
    T.adj.clear();
    T.undo.clear();
    T.redo.clear();
    T.face_head = 0;
    T.foot_own = false;
    for (int k = 0; k < PACK_KINDS; k++) {
        T.kind_template[k] = -1;
        T.no_fit[k] = false;
    }
    T.kind = 0, T.anim = 0, T.frame = 0, T.sel = -1, T.key_sel = -1;
    T.keys_sel.clear();
    T.watching[0] = 0;
    T.page = PAGE_MODEL;
    T.home = false;
    T.choosing = true;
    T.unsaved = true;
    return true;
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

// The tool's own folder for its heroes (the user's word, 2026-11-23: Save "in the tool", Add to game puts it in the game): heroes/ in its
// folder (pref_path), each a mod (MOD/mod.json, MOD/heroes/ID/...), made if it isn't there; false if there's no such place
bool heroes_home(char *out, size_t size) {
    char *pref = pref_path();
    if (pref == nullptr) {
        out[0] = 0;
        return false;
    }
    snprintf(out, size, "%sheroes", pref);
    SDL_free(pref);
    SDL_CreateDirectory(out);
    return true;
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
    T.maker = start_game(args);
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
    // (on Windows what it said at its end isn't known: started in the background, start_game)
    if (have != 0) {
        snprintf(T.status, sizeof(T.status), "Made in %d of %d games", have, PACK_KINDS);
    } else if (code != 0) {
        snprintf(T.status, sizeof(T.status), "The game didn't make it (it said %d): does it find its data when you start it yourself?", code);
    } else {
        snprintf(T.status, sizeof(T.status), "The game didn't make it: does it find its data when you start it yourself?");
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
            line(i * Step, -Far, i * Step, Far, 237, 235, 239, 0.2f, wide);
            line(-Far, i * Step, Far, i * Step, 237, 235, 239, 0.2f, wide);
        }
    }
    line(-Far, 0, Far, 0, 245, 158, 139, 0.85f, wide + 1); // (X coral, Y mint: Graphite's axes)
    line(0, -Far, 0, Far, 158, 216, 190, 0.85f, wide + 1);
}

// (ghost_before and ghost_after: onion skin's frames, as ghosts where they stick out past it, blue and orange)
void draw_made(const KitMade *m, int anim, int frame, View &v, SDL_Renderer *ren, const KitMade *rides = nullptr, int nrides = 0,
               const int16_t *live = nullptr, const int16_t *ghost_before = nullptr, const int16_t *ghost_after = nullptr) {
    const int PW = v.w > 0 ? v.w : PrevW, PH = v.h > 0 ? v.h : PrevH;

    if (v.rgba == nullptr) {
        v.rgba = new unsigned[PW * PH];
        v.depth = new float[PW * PH];
        v.tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, PW, PH);
        // blended, an opaque one too: its picture's rounded corners' smoothing (vertices fading out) is then laid over what's under it;
        // copied unblended, those pixels were the picture's own colour, part see-through, in the window itself (a pale line round the
        // corners in a screenshot)
        SDL_SetTextureBlendMode(v.tex, SDL_BLENDMODE_BLEND);
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
        std::fill(v.rgba, v.rgba + PW * PH, v.bg);
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
    // as tall as it looks from a little above (12 degrees, the view's own tilt to start: tilted further, it's the same size, so what's
    // drawn over it keeps to it), and its middle in the picture's
    float pc = cosf(v.pitch * 3.14159265f / 180), tall = fmaxf(h * 0.978f, w * 0.208f);
    v.scale = fminf(PH * 0.8f / (tall > 0 ? tall : 1), PW * 0.8f / (w > 0 ? w : 1));
    float cx = (lo[0] + hi[0]) / 2, cz = (lo[2] + hi[2]) / 2;
    v.cx = cx;
    v.cz = cz;
    v.ox = PW / 2.0f;
    v.oy = PH * 0.5f + (lo[1] + hi[1]) / 2 * pc * v.scale;
    v.zx = PW / 2.0f;
    v.zy = PH / 2.0f;
    // moved across (as it looks: the same however far it's zoomed)
    v.ox += v.px * PH / v.zoom;
    v.oy += v.py * PH / v.zoom;
    static std::vector<float> gd, gl;
    if (ghost_before != nullptr || ghost_after != nullptr) {
        gd.resize((size_t)PW * PH);
        gl.resize((size_t)PW * PH);
    }
    in_bands(PW, PH, [&](int ya, int yb) {
        std::fill(v.rgba + ya * PW, v.rgba + yb * PW, v.bg);
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
    pack_skin_posed(&sk, (const float(*)[3])p->P, (const float(*)[9])p->Q, p->hips, po->cut && !T.no_fit[kind], S.data());
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

// The keys picked made every move's in this game ("Apply to all animations": each a change at every frame of every move here, a blue bar), or
// game-wide ones made this move's alone ("Apply to this animation only": held all through it). The user's word, 2026-11-22: All moves as a box was a
// mode left on by mistake; it's what a key's made into now. A change to every move makes them all its own (own_before_change).
void own_before_change(Tool &T, const char *move);
void keys_all_moves(Tool &T, bool all) {
    const KitMade *mk = &T.made[T.kind];
    if (mk->nanims == 0) {
        return;
    }
    int a = T.anim < mk->nanims ? T.anim : 0, frames = mk->anims[a].frames;
    const char *move = mk->anims[a].name;
    std::vector<int> todo;
    for (int i : T.keys_sel) {
        bool game = strcmp(T.adj[i].move, "*") == 0;
        if (all ? !game : game) {
            todo.push_back(i);
        }
    }
    if (todo.empty()) {
        return;
    }
    if (todo.size() == 1) {
        adj_snapshot(T, all ? "Applied %s's key to all animations" : "Applied %s's key to this animation only", bone_title(T.adj[todo[0]].bone));
    } else {
        adj_snapshot(T, all ? "Applied %d keys to all animations" : "Applied %d keys to this animation only", (int)todo.size());
    }
    own_before_change(T, all ? "*" : move);
    for (int i : todo) {
        PackAdjust &k = T.adj[i];
        if (all) {
            SDL_strlcpy(k.move, "*", sizeof(k.move));
            k.at = -1, k.until = -1, k.spread = 0, k.blend = 0;
        } else {
            SDL_strlcpy(k.move, move, sizeof(k.move));
            k.at = 0, k.until = frames - 1, k.spread = 0;
        }
    }
}

// The keys picked taken away (the last first, so the others' places hold), a step of the history
void keys_delete(Tool &T) {
    std::vector<int> ks = T.keys_sel;
    if (ks.empty()) {
        return;
    }
    std::sort(ks.begin(), ks.end());
    if (ks.size() == 1) {
        adj_snapshot(T, "Deleted %s's key", bone_title(T.adj[ks[0]].bone));
    } else {
        adj_snapshot(T, "Deleted %d keys", (int)ks.size());
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
    T.unsaved = true;
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
    // (not said: the timeline's tag says whose it is now; the user's word, 2026-11-22: "remove the pop up about creating a new
    // animation")
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

// A key that hides its bone (and all on it) in every animation of its game: one in all of them, Scale 0. The user's word, 2026-11-22:
// a hero fitted into the tank (or a race cart) as its maker likes, by hiding bones, not by the game's squeeze (Fit in seat)
bool key_hides(const PackAdjust &k) {
    return k.at < 0 && strcmp(k.move, "*") == 0 && k.grow[0] <= -1 && k.grow[1] <= -1 && k.grow[2] <= -1;
}

// The key in all animations here of a bone (if hides, only one that hides it), -1 none
int game_key(const Tool &T, const char *bone, bool hides) {
    for (int i = 0; i < (int)T.adj.size(); i++) {
        const PackAdjust &k = T.adj[i];
        if (k.kind == T.kind && k.at < 0 && strcmp(k.move, "*") == 0 && SDL_strcasecmp(k.bone, bone) == 0 && (!hides || key_hides(k))) {
            return i;
        }
    }
    return -1;
}

// Each bone of a kind's pose hidden there, by its own key or one of a bone it's on
std::vector<char> hidden_bones(const Tool &T, int kind) {
    const KitPose *po = &T.poses[kind];
    std::vector<char> h((size_t)std::max(0, po->nbones), 0);
    for (const PackAdjust &k : T.adj) {
        int b;
        if (k.kind == kind && key_hides(k) && (b = pack_bone_index(po, k.bone)) >= 0) {
            h[b] = 1;
        }
    }
    for (int b = 0; b < po->nbones; b++) {
        for (int p = po->parent[b], n = 0; p >= 0 && n < po->nbones && !h[b]; p = po->parent[p], n++) {
            h[b] = h[p];
        }
    }
    return h;
}

// A bone hidden in every animation of this game, or shown again (and its other side, with X-Mirror), a step of the history: its key in
// all of them made Scale 0 (one made if it has none), or put back to Scale 1 (and gone if it changes nothing else). Its animations
// aren't made its own first (own_before_change): hiding changes no pose, so they stay its template's.
void bone_hide(Tool &T, const char *bone, bool hide) {
    char other[PACK_ADJUST_NAME] = "";
    if (T.xmirror && other_side(bone)[0] != 0 && pack_bone_index(&T.poses[T.kind], other_side(bone)) >= 0) {
        SDL_strlcpy(other, other_side(bone), sizeof(other));
    }
    adj_snapshot(T, hide ? "Hid %s" : "Showed %s", bone_title(bone));
    for (const char *n : { bone, (const char *)other }) {
        if (n[0] == 0) {
            continue;
        }
        int i = game_key(T, n, !hide);
        if (hide && i < 0) {
            if (T.adj.size() >= PACK_MAX_ADJUSTS) {
                snprintf(T.status, sizeof(T.status), "No room for more keys (%d)", PACK_MAX_ADJUSTS);
                break;
            }
            PackAdjust a = {};
            a.kind = T.kind;
            SDL_strlcpy(a.move, "*", sizeof(a.move));
            SDL_strlcpy(a.bone, n, sizeof(a.bone));
            a.at = -1, a.until = -1;
            T.adj.push_back(a);
            i = (int)T.adj.size() - 1;
        }
        if (i < 0) {
            continue;
        }
        PackAdjust &k = T.adj[i];
        for (int c = 0; c < 3; c++) {
            k.grow[c] = hide ? -1 : 0;
        }
        bool nothing = true;
        for (int c = 0; c < 3; c++) {
            nothing = nothing && k.turn[c] == 0 && k.reach[c] == 0;
        }
        if (!hide && nothing) {
            T.adj.erase(T.adj.begin() + i);
        }
    }
    keys_only(T, -1);
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
// several, each onto its own. Mirrored, each its other side's way (as X-Mirror makes it), and from several, each onto its other side
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
        snprintf(T.status, sizeof(T.status), "Pick a bone to paste onto");
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
bool view_axes(View &v, float &yaw, ImVec2 c, bool hovered) {
    static const char *const Letter[3] = { "X", "Y", "Z" };
    static const char *const Says[6] = { "Left (X)", "Back (Y)", "Top (Z)", "Right (-X)", "Front (-Y)", "Bottom (-Z)" };
    const ImU32 Col[3] = { AXIS_X, AXIS_Y, AXIS_Z };
    static const float Way[3][3] = { { 1, 0, 0 }, { 0, 0, 1 }, { 0, -1, 0 } }; // each in the game's axes
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 m = ImGui::GetIO().MousePos;
    float r = U(36), ball = U(11);
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
    // (on a round of its own, lighter with the mouse on it)
    dl->AddCircleFilled(c, r + ball + U(6), near ? HOVER : (RAISED & 0x00FFFFFFu) | 0xE6000000u);
    for (int e = 0; e < 6; e++) {
        const End &d = ends[e];
        int axis = d.side % 3;
        ImVec2 at(d.x, d.y);
        if (d.side < 3) {
            dl->AddLine(c, at, Col[axis], U(2));
            dl->AddCircleFilled(at, ball, Col[axis]);
            ImVec2 ts = ImGui::CalcTextSize(Letter[axis]);
            dl->AddText(ImVec2(at.x - ts.x / 2, at.y - ts.y / 2), PERI_INK, Letter[axis]);
        } else {
            dl->AddCircleFilled(at, ball * 0.8f, (Col[axis] & 0x00FFFFFFu) | 0x70000000u);
            dl->AddCircle(at, ball * 0.8f, Col[axis], 0, U(1.5f));
        }
        if (e == over) {
            dl->AddCircle(at, ball + U(2), INK, 0, U(2.5f));
        }
    }
    if (over >= 0) {
        ImGui::SetTooltip("%s", Says[ends[over].side]);
        if (ImGui::IsMouseClicked(0)) {
            view_from(v, yaw, ends[over].side);
            sound_cue(CUE_VIEW);
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
    // (what's hidden neither clips nor is clipped)
    std::vector<char> hid = hidden_bones(T, kind);
    for (int v = 0; v < po->nvert + std::max(0, nprop); v++) {
        int own = v < po->nvert ? Clips.dom[v] : po->hand;
        if (own < 0 || own >= nb || hid[own]) {
            continue;
        }
        float p[3] = { (float)vs[v * 3], (float)vs[v * 3 + 1], (float)vs[v * 3 + 2] };
        for (size_t c = 0; c < Clips.caps.size(); c++) {
            const Capsule &k = Clips.caps[c];
            if (k.r <= 0 || Clips.gap[(size_t)own * nb + k.bone] <= 2 || hid[k.bone]) {
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
    said = (said ^ (unsigned char)T.no_fit[kind]) * 1099511628211ull;
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
void adjust_view(Tool &T, ImVec2 im, float disp, bool hovered) {
    static Posed ps;
    static bool told_hips = false; // (Stretch off: a bone on the hips can't be moved, said once a drag)
    // the hero's own x (across), y (up and down) and z (front to back): Blender's X red, Z blue and Y green
    const ImU32 Axis[3] = { AXIS_X, AXIS_Z, AXIS_Y };
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
    /* a test's: HEROTOOL_LENGTHS=1 says where the bone picked is in the window, and each bone's distance from the one it hangs from, as
       it's shown (again as it moves) and after each drag (Stretch off keeps them) */
    static bool was_dragging = false;
    static float said_x = -1e9f, said_y = -1e9f;
    if (SDL_getenv("HEROTOOL_LENGTHS") != nullptr && T.sel >= 0) {
        if ((!T.dragging && !was_dragging && fabsf(sx[T.sel] - said_x) + fabsf(sy[T.sel] - said_y) > 2) || (was_dragging && !T.dragging)) {
            said_x = sx[T.sel], said_y = sy[T.sel];
            printf("herotool: %s at %.0f,%.0f, lengths", po->names[T.sel], sx[T.sel], sy[T.sel]);
            for (int b = 0; b < po->nbones; b++) {
                if (po->parent[b] >= 0) {
                    const float *x = ps.shown[b], *y = ps.shown[po->parent[b]];
                    printf(" %s:%.1f", po->names[b], sqrtf((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]) + (x[2] - y[2]) * (x[2] - y[2])));
                }
            }
            printf("\n");
        }
        was_dragging = T.dragging;
    }
    // the clipping finder: the frame's points inside another part, red
    if (T.clipping) {
        static std::vector<int> pts;
        clip_prepare(T, T.kind);
        clip_points(T, T.kind, scratch.data(), &ps.shown[0][0], pts);
        for (int pv : pts) {
            float q[3] = { scratch[pv * 3] - v.cx, -(float)scratch[pv * 3 + 1], -(float)scratch[pv * 3 + 2] - v.cz }, x, y, z;
            to_screen(v, q, x, y, z);
            dl->AddCircleFilled(ImVec2(im.x + x * disp, im.y + y * disp), U(2.5f), CORAL);
        }
    }
    // Bones off: a clean view of the hero (Tab)
    if (!T.bones) {
        return;
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
                if (axlen[i] < 0.25f) {
                    continue;
                }
                float d = seg_px(mouse, ImVec2(c.x + ax2[i][0] * U(14), c.y + ax2[i][1] * U(14)), ImVec2(c.x + ax2[i][0] * R * axlen[i], c.y + ax2[i][1] * R * axlen[i]));
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
    // each bone's dot in its part's colour (yellow the body, blue arms, green legs, orange the tail, ears and jaw), the one under the mouse
    // bigger; a hand's or foot's (which reach, and bend the arm or leg to follow) a square (as they were before Graphite: the user's word,
    // 2026-11-22, "that worked realy well before")
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
    // the gizmo, a little see-through (the user's word, 2026-11-22: "make it abit more sea-tru"), the handle under the mouse or being
    // dragged solid
    auto see = [](ImU32 col, bool solid) { return solid ? col : (col & 0x00FFFFFFu) | (ImU32)(((col >> 24) & 255) * 0.62f) << 24; };
    if (T.sel >= 0) {
        int lit = T.dragging ? T.handle : over;
        if (T.gizmo == GIZMO_TURN) {
            ImVec2 pts[48];
            dl->AddCircle(c, R * 1.25f, lit == H_TURN_VIEW ? INK : (INK & 0x00FFFFFFu) | 0x60000000u, 64, lit == H_TURN_VIEW ? U(3.5f) : U(2));
            for (int i = 0; i < 3; i++) {
                ring(i, pts, 48);
                dl->AddPolyline(pts, 48, see(Axis[i], lit == H_TURN + i), ImDrawFlags_Closed, lit == H_TURN + i ? U(5) : U(3));
            }
            dl->AddCircle(c, U(9), see(INK, lit == H_TURN_FREE), 0, U(2));
        } else {
            for (int i = 0; i < 3; i++) {
                if (axlen[i] < 0.12f) {
                    continue;
                }
                // (nearly end on: faded, and not taken)
                float fade = axlen[i] < 0.25f ? (axlen[i] - 0.12f) / 0.13f : 1, len = R * axlen[i];
                bool on = lit == (T.gizmo == GIZMO_MOVE ? H_MOVE : H_SIZE) + i;
                ImU32 col = (Axis[i] & 0x00FFFFFFu) | (ImU32)(255 * (0.25f + 0.75f * fade) * (on ? 1 : 0.62f)) << 24;
                ImVec2 e(c.x + ax2[i][0] * len, c.y + ax2[i][1] * len), s(c.x + ax2[i][0] * U(14), c.y + ax2[i][1] * U(14));
                float t = lit == (T.gizmo == GIZMO_MOVE ? H_MOVE : H_SIZE) + i ? U(5) : U(3);
                if (len > U(16)) {
                    dl->AddLine(s, e, col, t);
                }
                if (T.gizmo == GIZMO_MOVE) {
                    ImVec2 nrm(-ax2[i][1], ax2[i][0]);
                    dl->AddTriangleFilled(ImVec2(e.x + ax2[i][0] * U(12), e.y + ax2[i][1] * U(12)), ImVec2(e.x + nrm.x * U(6), e.y + nrm.y * U(6)),
                                          ImVec2(e.x - nrm.x * U(6), e.y - nrm.y * U(6)), col);
                } else {
                    dl->AddRectFilled(ImVec2(e.x - U(6), e.y - U(6)), ImVec2(e.x + U(6), e.y + U(6)), col, U(3));
                }
                ImGui::PushFont(FontSmall);
                ImVec2 ls = ImGui::CalcTextSize(AxisSays[i]), lp(e.x + ax2[i][0] * U(26) - ls.x / 2, e.y + ax2[i][1] * U(26) - ls.y / 2);
                dl->AddText(lp, col, AxisSays[i]);
                ImGui::PopFont();
            }
            bool mid = T.dragging && (T.handle == H_MOVE_FREE || T.handle == H_SIZE_ALL);
            dl->AddCircle(c, U(12), mid ? INK : (INK & 0x00FFFFFFu) | 0x70000000u, 0, mid ? U(3.5f) : U(2.5f));
        }
    }
    if (!T.dragging) {
        if (over >= H_TURN && over < H_TURN + 3) {
            ImGui::SetTooltip("%s: rotate %s", bone_title(po->names[T.sel]), AxisSays[over - H_TURN]);
        } else if (over == H_TURN_VIEW) {
            ImGui::SetTooltip("%s: rotate (view)", bone_title(po->names[T.sel]));
        } else if (over >= H_MOVE && over < H_MOVE + 3) {
            ImGui::SetTooltip("%s: move %s", bone_title(po->names[T.sel]), AxisSays[over - H_MOVE]);
        } else if (over >= H_SIZE && over < H_SIZE + 3) {
            ImGui::SetTooltip("%s: scale %s", bone_title(po->names[T.sel]), AxisSays[over - H_SIZE]);
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
            // (picked without a sound: the user's word, 2026-11-22, "When clicking bones lets not play a sound it gets anoying")
            T.sel = hover;
        } else if (take == H_NONE && T.sel >= 0) {
            // an empty part of the picture: let go of the bone picked (as Esc)
            T.sel = -1;
        }
        if (take != H_NONE) {
            const char *b = bone_title(po->names[T.sel]);
            if (take >= H_SIZE && take <= H_SIZE_ALL) {
                adj_snapshot(T, "Scaled %s", b);
            } else if (take >= H_MOVE && take <= H_MOVE_FREE) {
                int up = po->parent[T.sel];
                if (!T.stretch && !reach_bone(po->names[T.sel]) && up >= 0) {
                    char was[80];
                    SDL_strlcpy(was, b, sizeof(was));
                    adj_snapshot(T, "Moved %s, rotating %s", was, bone_title(po->names[up]));
                } else {
                    adj_snapshot(T, "Moved %s", b);
                }
            } else {
                adj_snapshot(T, "Rotated %s", b);
            }
            T.dragging = true;
            T.handle = take;
            T.adj_playing = false;
            told_hips = false;
        }
    }
    if (T.dragging && !ImGui::IsMouseDown(0)) {
        T.dragging = false;
        T.handle = H_NONE;
    }
    if (!T.dragging || T.sel < 0 || (io.MouseDelta.x == 0 && io.MouseDelta.y == 0)) {
        return;
    }
    int h = T.handle;
    float jx = sx[T.sel], jy = sy[T.sel];
    // Stretch off, moving a bone that isn't a hand, a foot or the hips (they never stretch: a hand or foot bends its arm or leg to reach,
    // the hips move all): the one it hangs from turned about its joint so this one's goes where it's dragged (along the axis, as far as
    // the mouse went that way; dragged by its middle, under the mouse), as near as it can at the same distance; the turn a key of that
    // one's at this frame, as Rotate makes. On the hips it can't move at all (turning the hips would turn the legs too): said so
    if (!T.stretch && h >= H_MOVE && h <= H_MOVE_FREE && !reach_bone(po->names[T.sel]) && po->parent[T.sel] >= 0) {
        int up = po->parent[T.sel];
        if (SDL_strcasecmp(po->names[up], "hips") == 0) {
            if (!told_hips) {
                told_hips = true;
                char who[80];
                SDL_strlcpy(who, bone_title(po->names[T.sel]), sizeof(who));
                toast(T, "%s hangs from the hips: rotate it, or turn Stretch on to move it", who);
            }
            return;
        }
        float d[3] = { 0, 0, 0 }, g[3] = { 0, 0, 0 };
        if (h < H_MOVE + 3) {
            int i = h - H_MOVE;
            float along = io.MouseDelta.x * ax2[i][0] + io.MouseDelta.y * ax2[i][1], per = fmaxf(axlen[i], 0.15f) * pp, ax[3];
            hero_axis(ps, i, ax);
            for (int c = 0; c < 3; c++) {
                g[c] = ax[c] * along / per;
            }
        } else {
            d[0] = (mouse.x - jx) / pp, d[1] = -(mouse.y - jy) / pp;
            view_to_game(v, d, g);
        }
        float u[3], w[3], n[3];
        for (int c = 0; c < 3; c++) {
            u[c] = ps.shown[T.sel][c] - ps.shown[up][c];
            w[c] = u[c] + g[c];
        }
        n[0] = u[1] * w[2] - u[2] * w[1], n[1] = u[2] * w[0] - u[0] * w[2], n[2] = u[0] * w[1] - u[1] * w[0];
        float nl = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]), dot = u[0] * w[0] + u[1] * w[1] + u[2] * w[2];
        if (nl < 1e-6f) {
            return;
        }
        float deg = atan2f(nl, dot) * 180 / 3.14159265f, axis[3] = { n[0] / nl, n[1] / nl, n[2] / nl }, hh[3];
        game_to_hero(ps, axis, hh);
        float hl = sqrtf(hh[0] * hh[0] + hh[1] * hh[1] + hh[2] * hh[2]);
        PackAdjust *kp = hl > 1e-6f ? adj_key(T, mk->anims[a].name, po->names[up], true) : nullptr;
        if (kp == nullptr) {
            return;
        }
        for (int c = 0; c < 3; c++) {
            kp->turn[c] = fmaxf(-360, fminf(360, kp->turn[c] + hh[c] / hl * deg));
        }
        keys_only(T, (int)(kp - T.adj.data()));
        mirror_edit(T, T.key_sel, T.adj[T.key_sel].at);
        return;
    }
    PackAdjust *k = adj_key(T, mk->anims[a].name, po->names[T.sel], true);
    if (k == nullptr) {
        return;
    }
    keys_only(T, (int)(k - T.adj.data()));
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
    // what a change goes to: a bar picked (every frame: the whole move, or every move here) of the joint picked, else the frame shown
    T.key_mode = KEY_FRAME;
    if (T.key_sel >= 0 && T.sel >= 0 && T.sel < po->nbones && T.adj[T.key_sel].at < 0
        && SDL_strcasecmp(T.adj[T.key_sel].bone, po->names[T.sel]) == 0) {
        T.key_mode = strcmp(T.adj[T.key_sel].move, "*") == 0 ? KEY_GAME : KEY_MOVE;
    }
    // its header: the animation shown (and whose it is), then the key before, play, the key after, the frame and the speed
    float x_start = ImGui::GetCursorPosX(), top = ImGui::GetCursorPosY(), H = U(42);
    auto mid = [&](float h) { ImGui::SetCursorPosY(top + (H - h) / 2); };
    mid(ImGui::GetFontSize() + U(18));
    if (menu_begin("##watching", move, U(190))) {
        for (int i = 0; i < mk->nanims; i++) {
            if (menu_item(mk->anims[i].name, i == a)) {
                T.anim = i;
                T.made_t = 0;
                T.frame = 0;
            }
        }
        ImGui::EndCombo();
    }
    {
        // whose it is: its own stays as it's made whoever it's based on; its template's becomes its own when it's first changed
        const BakedMove *copied = baked_of(T, T.kind, move);
        bool own = T.poses[T.kind].anims[a].own || copied != nullptr;
        char says[80];
        if (own) {
            snprintf(says, sizeof(says), copied != nullptr ? "Own animation (from %s)" : "Own animation", HeroNames[copied != nullptr ? copied->from : 0]);
        } else {
            snprintf(says, sizeof(says), "%s's animation", HeroNames[template_of(T, T.kind)]);
        }
        ImGui::SameLine(0, U(12));
        mid(U(28));
        tag(says, own ? TINT_MINT : RAISED, own ? MINT_LIGHT : MUTED, own ? MINT : DIM);
        ImGui::SetItemTooltip(own ? "This hero's own %s: it stays as you make it, whoever it's based on" : "Changing %s makes it this hero's own copy", move);
    }
    float bh = ImGui::GetFrameHeight() + U(6), right_w = bh * 2 + H + U(8) * 4 + U(76) + U(110);
    ImGui::SameLine(x_start + width - right_w);
    std::vector<int> keys;
    for (auto &k : T.adj) {
        if (k.kind == T.kind && k.at >= 0 && SDL_strcasecmp(k.move, move) == 0) {
            keys.push_back(k.at);
            keys.push_back(key_last(k));
        }
    }
    mid(bh);
    if (icon_button("##before", IC_PREV, "The key before", RAISED, INK, bh)) {
        int to = -1;
        for (int f : keys) {
            to = f < T.frame && f > to ? f : to;
        }
        T.frame = to >= 0 ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SameLine(0, U(8));
    mid(H);
    if (button("##play", CORAL, CORAL_INK, H, T.adj_playing ? IC_PAUSE : IC_PLAY, T.adj_playing ? CUE_STOP : CUE_PLAY, H)) {
        T.adj_playing = !T.adj_playing;
    }
    ImGui::SetItemTooltip("%s (Space)", T.adj_playing ? "Pause" : "Play");
    ImGui::SameLine(0, U(8));
    mid(bh);
    if (icon_button("##after", IC_NEXT, "The key after", RAISED, INK, bh)) {
        int to = frames;
        for (int f : keys) {
            to = f > T.frame && f < to ? f : to;
        }
        T.frame = to < frames ? to : T.frame;
        T.adj_playing = false;
    }
    ImGui::SameLine(0, U(8));
    {
        char fr[24];
        snprintf(fr, sizeof(fr), "%d / %d", T.frame, frames);
        mid(ImGui::GetTextLineHeight());
        ImVec2 tp = ImGui::GetCursorScreenPos(), ts = ImGui::CalcTextSize(fr);
        ImGui::GetWindowDrawList()->AddText(ImVec2(tp.x + (U(76) - ts.x) / 2, tp.y), INK, fr);
        ImGui::Dummy(ImVec2(U(76), ts.y));
        ImGui::SameLine(0, U(8));
    }
    mid(ImGui::GetFontSize() + U(14));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(RAISED));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, V4(HOVER));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
    ImGui::SetNextItemWidth(U(110));
    ImGui::SliderFloat("##speed", &T.made_speed, 0, 2, "%.2fx");
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
    ImGui::SetItemTooltip("Speed");
    ImGui::SetCursorPosY(top + H + U(10));

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
    const float label = U(130), ruler = U(24), row = U(36), strip = T.clipping ? U(14) : 0;
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
    // the ruler: a dot each frame, a number every so many
    int every = cw >= U(28) ? 1 : cw >= U(12) ? 5 : 10;
    dl->PushClipRect(ImVec2(x0, o.y), ImVec2(o.x + width, o.y + h), true);
    for (int f = std::max(0, (int)T.tl_first - 1); f < std::min(frames, (int)(T.tl_first + shown) + 2); f++) {
        float x = fx((float)f);
        bool big = f % every == 0;
        dl->AddCircleFilled(ImVec2(x, o.y + ruler - U(5)), big ? U(3.5f) : U(2), big ? HOVER : (HOVER & 0x00FFFFFFu) | 0xB0000000u);
        char n[8];
        snprintf(n, sizeof(n), "%d", f);
        ImGui::PushFont(FontSmall);
        if (big && x + U(2) + ImGui::CalcTextSize(n).x <= o.x + width) {
            dl->AddText(ImVec2(x - ImGui::CalcTextSize(n).x / 2, o.y - U(2)), DIM, n);
        }
        ImGui::PopFont();
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
                dl->AddRectFilled(ImVec2(fx((float)g) - cw / 2 + U(1), o.y + ruler + U(2)), ImVec2(fx((float)g) + cw / 2 - U(1), o.y + ruler + strip - U(3)),
                                  (CORAL & 0x00FFFFFFu) | (ImU32)al << 24, U(4));
            }
        }
    }
    if (T.tl_zoom > 1) {
        // the part of the move shown, under the ruler
        dl->AddRectFilled(ImVec2(x0 + T.tl_first / frames * span, o.y + ruler - U(3)), ImVec2(x0 + (T.tl_first + shown) / frames * span, o.y + ruler),
                          PERI, U(2));
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
        // its track (the bone picked's a periwinkle tint) and its name (the bone picked's a periwinkle pill)
        dl->AddRectFilled(ImVec2(x0, y + U(2)), ImVec2(o.x + width, y + row - U(2)), b == T.sel ? IM_COL32(0x27, 0x2A, 0x3A, 255) : DEEP, (row - U(4)) / 2);
        {
            const char *bt = bone_title(po->names[b]);
            ImVec2 ts = ImGui::CalcTextSize(bt);
            if (b == T.sel) {
                dl->AddRectFilled(ImVec2(o.x, y + (row - ts.y) / 2 - U(4)), ImVec2(o.x + ts.x + U(20), y + (row + ts.y) / 2 + U(4)),
                                  (PERI & 0x00FFFFFFu) | 0x38000000u, ts.y);
            }
            dl->AddText(ImVec2(o.x + U(10), y + (row - ts.y) / 2), b == T.sel ? PERI_LIGHT : MUTED, bt);
        }
        dl->PushClipRect(ImVec2(x0, y), ImVec2(o.x + width, y + row), true);
        for (int i = 0; i < (int)T.adj.size(); i++) {
            const PackAdjust &k = T.adj[i];
            if (!pack_adjust_for(&k, T.kind, move) || SDL_strcasecmp(k.bone, po->names[b]) != 0) {
                continue;
            }
            bool picked = key_picked(T, i);
            if (k.at < 0) {
                // the whole move (green) or every move here (blue): a bar; grey, one that hides its bone
                ImU32 col = key_hides(k)               ? (picked ? mix(IM_COL32(0x46, 0x45, 0x4B, 255), INK, 0.3f) : IM_COL32(0x46, 0x45, 0x4B, 255))
                            : strcmp(k.move, "*") == 0 ? (picked ? PERI : mix(TINT_PERI, PERI, 0.45f))
                                                       : (picked ? MINT : mix(TINT_MINT, MINT, 0.45f));
                ImVec2 lo(x0 + U(5), y + row * 0.32f), hi(o.x + width - U(5), y + row * 0.68f);
                dl->AddRectFilled(lo, hi, col, (hi.y - lo.y) / 2);
                hits.push_back({ i, KEY_BODY, lo, hi });
                continue;
            }
            int last = key_last(k);
            float xa = fx((float)k.at), xb = fx((float)last);
            ImU32 col = picked ? PERI : GREY_DOT;
            {
                // how much of it there is each frame as the game makes it (its fade, its blend into the next or from the one before),
                // up from the row's foot, where there's any
                std::vector<ImVec2> pts;
                float foot = y + row - U(2);
                auto flush = [&]() {
                    for (size_t p = 0; p + 1 < pts.size(); p++) {
                        dl->AddQuadFilled(ImVec2(pts[p].x, foot), pts[p], pts[p + 1], ImVec2(pts[p + 1].x, foot), (PERI & 0x00FFFFFFu) | 0x22000000u);
                    }
                    if (pts.size() > 1) {
                        dl->AddPolyline(pts.data(), (int)pts.size(), (PERI & 0x00FFFFFFu) | 0x70000000u, 0, U(1.5f));
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
                dl->AddLine(ImVec2(fx((float)last), my), ImVec2(fx((float)T.adj[to].at), my), PERI_LIGHT, U(2.5f));
            }
            auto diamond = [&](float x) {
                dl->AddCircleFilled(ImVec2(x, my), s + U(2), DEEP);
                dl->AddCircleFilled(ImVec2(x, my), s, col);
            };
            if (last > k.at) {
                // held: a bar from its first frame to its last, a diamond each end (the ends held over more frames or fewer)
                dl->AddRectFilled(ImVec2(xa - s, my - s), ImVec2(xb + s, my + s), col, s);
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
                dl->AddRectFilled(g0, g1, INK, U(3));
                dl->AddRectFilled(h0, h1, INK, U(3));
                hits.push_back({ i, KEY_START, g0, g1 });
                hits.push_back({ i, KEY_END, h0, h1 });
            }
        }
        dl->PopClipRect();
    }
    if (rows.empty()) {
        dl->AddText(ImVec2(o.x + U(10), o.y + ruler + strip + (row - ImGui::GetTextLineHeight()) / 2), MUTED, "Click a bone");
    }
    // the frame shown, across it all
    float px = fx((float)T.frame);
    dl->PushClipRect(ImVec2(x0, o.y), ImVec2(o.x + width, o.y + h), true);
    dl->AddLine(ImVec2(px, o.y + U(4)), ImVec2(px, o.y + h - U(2)), CORAL, U(3));
    dl->AddCircleFilled(ImVec2(px, o.y + ruler - U(5)), U(6), CORAL);
    if (T.key_drag && T.snapped >= 0) {
        // the frame a dragged key's snapped to
        dl->AddLine(ImVec2(fx((float)T.snapped), o.y), ImVec2(fx((float)T.snapped), o.y + h), (INK & 0x00FFFFFFu) | 0x96000000u, U(1.5f));
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
        dl->AddRectFilled(lo, hi, (PERI & 0x00FFFFFFu) | 0x28000000u, U(6));
        dl->AddRect(lo, hi, (PERI & 0x00FFFFFFu) | 0xAA000000u, U(6), 0, U(1.5f));
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
                    to > from ? adj_step_says(T, "Stretched %s's key to frames %d-%d", b, from, to) : adj_step_says(T, "Made %s's key one frame (%d)", b, from);
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
        } else if (key_hides(k)) {
            ImGui::SetTooltip("%s  hidden in all animations", bone_title(k.bone));
        } else {
            ImGui::SetTooltip("%s  %s\nRight-click: %s", bone_title(k.bone), strcmp(k.move, "*") == 0 ? "all animations" : "whole animation",
                              strcmp(k.move, "*") == 0 ? "Apply to this animation only" : "Apply to all animations");
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
            ImGui::SetItemTooltip("Flow into the bone's next key instead of fading out");
            bool here = false, game = false;
            for (int i : ks) {
                (strcmp(T.adj[i].move, "*") == 0 ? game : here) = true;
            }
            if (here && ImGui::MenuItem("Apply to all animations")) {
                keys_all_moves(T, true);
            }
            ImGui::SetItemTooltip("Select to apply this change to all animations in this game");
            if (game && ImGui::MenuItem("Apply to this animation only")) {
                keys_all_moves(T, false);
            }
            ImGui::SetItemTooltip("Select to apply this change to this animation only");
            if (paste != 0) {
                key_paste(T, pack_bone_index(po, k.bone), k.at >= 0 ? k.at : T.frame, paste == 2);
            } else if (!fk.empty()) {
                // held over more of the move: to its end, from its start, all through it; or each its own frame again
                bool short_end = false, late = false, held = false;
                for (PackAdjust *x : fk) {
                    short_end |= key_last(*x) < frames - 1, late |= x->at > 0, held |= key_last(*x) > x->at;
                }
                if (ImGui::MenuItem("Stretch to end", nullptr, false, short_end)) {
                    adj_snapshot(T, "Stretched %s to the end", who);
                    for (PackAdjust *x : fk) {
                        x->until = frames - 1;
                    }
                }
                if (ImGui::MenuItem("Stretch to start", nullptr, false, late)) {
                    adj_snapshot(T, "Stretched %s to the start", who);
                    for (PackAdjust *x : fk) {
                        x->until = key_last(*x);
                        x->at = 0;
                    }
                }
                if (ImGui::MenuItem("Stretch to whole animation", nullptr, false, late || short_end)) {
                    adj_snapshot(T, "Stretched %s over %s", who, move);
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

// A bone's other side's (upperarm_l's upperarm_r, ear_l1's ear_r1), else -1
// The joints' grid's step: a round size near a tenth of the model's height (1, 2 or 5 times a power of ten)
float grid_step(const KitModel *m) {
    float h = m->hi[1] - m->lo[1], step = powf(10, floorf(log10f(h / 10)));
    return step * (h / 10 / step >= 5 ? 5 : h / 10 / step >= 2 ? 2 : 1);
}

int mirror_bone(int b) {
    const char *name = KitBoneNames[b];
    size_t len = strlen(name);
    for (int o = 0; o < KB_COUNT; o++) {
        const char *on = KitBoneNames[o];
        if (o != b && strlen(on) == len
            && ((strncmp(on, name, len - 1) == 0 && ((name[len - 1] == 'l' && on[len - 1] == 'r') || (name[len - 1] == 'r' && on[len - 1] == 'l')))
                // (ears' and others' names end in a digit: ear_l1, ear_r1)
                || (len > 2 && strncmp(on, name, len - 2) == 0 && on[len - 1] == name[len - 1]
                    && ((name[len - 2] == 'l' && on[len - 2] == 'r') || (name[len - 2] == 'r' && on[len - 2] == 'l'))))) {
            return o;
        }
    }
    return -1;
}

bool joint_picked(const Tool &T, int b, int e) {
    for (auto &j : T.joint_sel) {
        if (j.first == b && j.second == e) {
            return true;
        }
    }
    return false;
}

// The joints picked put in a straight line (the user's word, 2026-11-23: "select nodes right click and be able to straighten"): the two
// farthest apart stay, and each of the others goes to where it's nearest on the line from one to the other (between them), with the one
// joint it is (the bone carrying on from it), and with Mirror its other side's mirrored across the hips unless that's picked itself; a
// step of the undo. False if fewer than three are picked
// A joint put at q: its place, and the one joint it is (its bone's end and the start of the one carrying on), and with Mirror its other
// side's mirrored across the hips unless that's picked itself
void joint_put(Tool &T, int b, int e, const float *q) {
    auto set = [&](int bb, int ee, const float *to) {
        memcpy(ee ? T.m->bones[bb].end : T.m->bones[bb].at, to, sizeof(float) * 3);
        for (int c = 0; c < KB_COUNT; c++) {
            if (T.m->bones[c].have && ee && kit_carries_on(bb, c)) {
                memcpy(T.m->bones[c].at, to, sizeof(float) * 3);
            }
            if (T.m->bones[c].have && !ee && kit_carries_on(c, bb)) {
                memcpy(T.m->bones[c].end, to, sizeof(float) * 3);
            }
        }
    };
    set(b, e, q);
    int o = mirror_bone(b);
    if (T.mirror && o >= 0 && T.m->bones[o].have && !joint_picked(T, o, e)) {
        float mid = T.m->bones[KB_HIPS].have ? T.m->bones[KB_HIPS].at[0] : (T.m->lo[0] + T.m->hi[0]) / 2, m[3] = { 2 * mid - q[0], q[1], q[2] };
        set(o, e, m);
    }
}

// Snap (Straighten first; renamed, the user's word, 2026-11-23: "rename straighten to snap"): the joints picked onto one line of the grid in the view (the user's word, 2026-11-23: "did not work for the spine. maybe the
// straighten can snap to the closes grid line"): up and down if they're spread more that way (a spine: across, the front view's x or the
// side view's z, made the nearest line's to their middle), else across (an arm held out: their height the nearest line's); the grid's
// lines (grid_step, through x 0 and z 0 and up from its feet) while it's shown, else their middle. A step of the undo; false if fewer
// than two are picked
bool joints_to_grid(Tool &T, int which) {
    if (T.m == nullptr || T.joint_sel.size() < 2) {
        return false;
    }
    int across = which == 0 ? 0 : 2;
    float lo[2] = { 1e9f, 1e9f }, hi[2] = { -1e9f, -1e9f }, sum[2] = { 0, 0 };
    for (auto &j : T.joint_sel) {
        const float *p = j.second ? T.m->bones[j.first].end : T.m->bones[j.first].at;
        float c[2] = { p[across], p[1] };
        for (int i = 0; i < 2; i++) {
            lo[i] = fminf(lo[i], c[i]), hi[i] = fmaxf(hi[i], c[i]), sum[i] += c[i];
        }
    }
    int n = (int)T.joint_sel.size(), axis = hi[0] - lo[0] <= hi[1] - lo[1] ? 0 : 1; // 0: a line up and down (their across set), 1: across
    float mean = sum[axis] / n, step = grid_step(T.m), base = axis == 0 ? 0 : T.m->lo[1];
    float to = T.joint_grid ? base + roundf((mean - base) / step) * step : mean;
    joint_snapshot(T, "Snapped %d joints", n);
    std::vector<std::pair<int, int>> sel = T.joint_sel;
    for (auto &j : sel) {
        const float *p = j.second ? T.m->bones[j.first].end : T.m->bones[j.first].at;
        float q[3] = { p[0], p[1], p[2] };
        q[axis == 0 ? across : 1] = to;
        joint_put(T, j.first, j.second, q);
    }
    reweigh(T);
    T.dirty = T.unsaved = true;
    snprintf(T.status, sizeof(T.status), "Snapped %d joints, %s", n, axis == 0 ? "up and down" : "across");
    if (SDL_getenv("HEROTOOL_MARKERS") != nullptr) {
        printf("herotool: %s\n", T.status);
        for (int b = 0; b < KB_COUNT; b++) {
            const KitBone &k = T.m->bones[b];
            if (k.have) {
                printf("herotool: now %s %.4f,%.4f,%.4f\n", KitBoneNames[b], k.at[0], k.at[1], k.at[2]);
            }
        }
    }
    return true;
}

// Line up between the ends: the old Straighten, for a line that isn't up and down or across (an arm held down and out)
bool joints_straighten(Tool &T) {
    if (T.m == nullptr || T.joint_sel.size() < 3) {
        return false;
    }
    auto pt = [&](int b, int e) { return e ? T.m->bones[b].end : T.m->bones[b].at; };
    int a = 0, z = 1;
    float far = -1;
    for (size_t i = 0; i < T.joint_sel.size(); i++) {
        for (size_t j = i + 1; j < T.joint_sel.size(); j++) {
            const float *p = pt(T.joint_sel[i].first, T.joint_sel[i].second), *q = pt(T.joint_sel[j].first, T.joint_sel[j].second);
            float d = (p[0] - q[0]) * (p[0] - q[0]) + (p[1] - q[1]) * (p[1] - q[1]) + (p[2] - q[2]) * (p[2] - q[2]);
            if (d > far) {
                far = d, a = (int)i, z = (int)j;
            }
        }
    }
    if (far <= 0) {
        return false;
    }
    joint_snapshot(T, "Lined up %d joints", (int)T.joint_sel.size());
    float A[3], B[3];
    memcpy(A, pt(T.joint_sel[a].first, T.joint_sel[a].second), sizeof(A));
    memcpy(B, pt(T.joint_sel[z].first, T.joint_sel[z].second), sizeof(B));
    float mid = T.m->bones[KB_HIPS].have ? T.m->bones[KB_HIPS].at[0] : (T.m->lo[0] + T.m->hi[0]) / 2;
    // a joint put at q: its place, and the one joint it is (its bone's end and the start of the one carrying on)
    auto put = [&](int b, int e, const float *q) {
        memcpy(pt(b, e), q, sizeof(float) * 3);
        for (int c = 0; c < KB_COUNT; c++) {
            if (T.m->bones[c].have && e && kit_carries_on(b, c)) {
                memcpy(T.m->bones[c].at, q, sizeof(float) * 3);
            }
            if (T.m->bones[c].have && !e && kit_carries_on(c, b)) {
                memcpy(T.m->bones[c].end, q, sizeof(float) * 3);
            }
        }
    };
    for (size_t k = 0; k < T.joint_sel.size(); k++) {
        if ((int)k == a || (int)k == z) {
            continue;
        }
        int b = T.joint_sel[k].first, e = T.joint_sel[k].second;
        const float *p = pt(b, e);
        float t = ((p[0] - A[0]) * (B[0] - A[0]) + (p[1] - A[1]) * (B[1] - A[1]) + (p[2] - A[2]) * (B[2] - A[2])) / far;
        t = t < 0 ? 0 : t > 1 ? 1 : t;
        float q[3] = { A[0] + (B[0] - A[0]) * t, A[1] + (B[1] - A[1]) * t, A[2] + (B[2] - A[2]) * t };
        put(b, e, q);
        int o = mirror_bone(b);
        if (T.mirror && o >= 0 && T.m->bones[o].have && !joint_picked(T, o, e)) {
            float m[3] = { 2 * mid - q[0], q[1], q[2] };
            put(o, e, m);
        }
    }
    reweigh(T);
    T.dirty = T.unsaved = true;
    snprintf(T.status, sizeof(T.status), "Lined up %d joints", (int)T.joint_sel.size());
    return true;
}

void view_window(Tool &T, View &v, const char *title, bool markers, int which) {
    // (a view's drawn only while its page shows it, so the first time it's shown its picture comes a frame later)
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2((float)vw(v), (float)vh(v)));
    if (v.tex != nullptr) {
        ImGui::GetWindowDrawList()->AddImageRounded((ImTextureID)(intptr_t)v.tex, p0, ImVec2(p0.x + vw(v), p0.y + vh(v)), ImVec2(0, 0), ImVec2(1, 1),
                                                    IM_COL32_WHITE, U(24));
    }
    ImVec2 at = ImGui::GetItemRectMin();
    if (!markers) {
        return;
    }
    // zoomed by the wheel about the mouse, moved by dragging with the right button, and back by double-clicking it (or Home): both views
    // alike up and down, so a joint's at the same height in each (the user's word, 2026-11-23: "i should also be able to zoom in when
    // moving the joints")
    if (ImGui::IsItemHovered() || (T.pan_view == which && ImGui::IsMouseDown(ImGuiMouseButton_Right))) {
        ImGuiIO &io = ImGui::GetIO();
        View &other = which == 0 ? T.side : T.front;
        float mx = io.MousePos.x - at.x, my = io.MousePos.y - at.y;
        float h = T.m->hi[1] - T.m->lo[1], w = fmaxf(T.m->hi[0] - T.m->lo[0], T.m->hi[2] - T.m->lo[2]);
        float base = fminf(vh(v) * 0.85f / h, vw(v) * 0.85f / (w > 0 ? w : 1));
        if (io.MouseWheel != 0 && ImGui::IsItemHovered()) {
            float s1 = fmaxf(base * 0.6f, fminf(base * 12, v.scale * powf(1.2f, io.MouseWheel))), f = s1 / v.scale;
            v.ox = mx - (mx - v.ox) * f, v.oy = my - (my - v.oy) * f, v.scale = s1;
            other.ox = vw(other) / 2.0f - (vw(other) / 2.0f - other.ox) * f, other.oy = my - (my - other.oy) * f, other.scale = s1;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()) {
            T.pan_view = which;
        }
        if (T.pan_view == which && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 2)) {
            v.ox += io.MouseDelta.x, v.oy += io.MouseDelta.y;
            other.oy += io.MouseDelta.y;
        }
        if (ImGui::IsItemHovered() && (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Home, false))) {
            fit(T.m, T.front);
            fit(T.m, T.side);
        }
        if (ImGui::IsItemHovered() && T.drag_bone < 0) {
            ImGui::SetItemTooltip("Drag a dot onto its joint   Drag round dots, or Ctrl-click them: pick several, then drag one   Right-click: "
                                  "snap them   Wheel: zoom   Right-drag: move   Right double-click: back");
        }
    }
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Right) && T.pan_view == which) {
        T.pan_view = -1;
    }
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 mouse = ImGui::GetIO().MousePos;
    /* a test's: HEROTOOL_MARKERS=BONE says where that bone's start is in the front view (again as it moves), and every joint's place
       after each drag */
    if (const char *mk = SDL_getenv("HEROTOOL_MARKERS")) {
        static float said_x[2] = { -1e9f, -1e9f }, said_y[2] = { -1e9f, -1e9f };
        for (int b = 0; which < 2 && b < KB_COUNT; b++) {
            if (T.m->bones[b].have && SDL_strcmp(KitBoneNames[b], mk) == 0) {
                float x, y, z;
                to_screen(v, T.m->bones[b].at, x, y, z);
                if (fabsf(at.x + x - said_x[which]) + fabsf(at.y + y - said_y[which]) > 2) {
                    said_x[which] = at.x + x, said_y[which] = at.y + y;
                    printf("herotool: %s at %.0f,%.0f%s\n", mk, said_x[which], said_y[which], which ? " (side)" : "");
                }
            }
        }
    }
    // (the dots kept in their view, zoomed in: drawn only there, and taken only there)
    ImVec2 lo(at.x, at.y), hi(at.x + vw(v), at.y + vh(v));
    bool in_view = mouse.x >= lo.x && mouse.x < hi.x && mouse.y >= lo.y && mouse.y < hi.y;
    dl->PushClipRect(lo, hi, true);
    float pick_d2 = 1e30f;
    int pick_bone = -1, pick_end = 0;
    // its grid, behind the dots (the user's word, 2026-11-23: "It needs a toggable grid"): a line each step of a round size near a tenth of
    // its height, across through its middle (x 0 front, z 0 side) and up from its feet, those two brighter
    if (T.joint_grid) {
        float step = grid_step(T.m);
        float x0 = (0 - v.ox) / v.scale, x1 = (vw(v) - v.ox) / v.scale;
        for (int k = (int)floorf(fminf(x0, x1) / step); k <= (int)ceilf(fmaxf(x0, x1) / step); k++) {
            float sx = at.x + v.ox + k * step * v.scale;
            dl->AddLine(ImVec2(sx, lo.y), ImVec2(sx, hi.y), k == 0 ? IM_COL32(237, 235, 239, 70) : IM_COL32(237, 235, 239, 22), k == 0 ? U(1.5f) : U(1));
        }
        float y0 = (v.oy - vh(v)) / v.scale, y1 = v.oy / v.scale;
        for (int k = (int)floorf((y0 - T.m->lo[1]) / step); k <= (int)ceilf((y1 - T.m->lo[1]) / step); k++) {
            float sy = at.y + v.oy - (T.m->lo[1] + k * step) * v.scale;
            dl->AddLine(ImVec2(lo.x, sy), ImVec2(hi.x, sy), k == 0 ? IM_COL32(237, 235, 239, 70) : IM_COL32(237, 235, 239, 22), k == 0 ? U(1.5f) : U(1));
        }
    }
    for (int b = 0; b < KB_COUNT; b++) {
        KitBone &k = T.m->bones[b];
        if (!k.have) {
            continue;
        }
        float sx[2], sy[2], sz;
        to_screen(v, k.at, sx[0], sy[0], sz);
        to_screen(v, k.end, sx[1], sy[1], sz);
        ImU32 col = b >= KB_JAW ? CORAL : (b >= KB_UPPERARM_L && b <= KB_HAND_R) ? PERI : b >= KB_THIGH_L ? MINT : IM_COL32(0xF2, 0xD5, 0x8A, 255);
        dl->AddLine(ImVec2(at.x + sx[0], at.y + sy[0]), ImVec2(at.x + sx[1], at.y + sy[1]), col, U(2));
        for (int e = 0; e < 2; e++) {
            ImVec2 c(at.x + sx[e], at.y + sy[e]);
            dl->AddCircleFilled(c, e ? U(4) : U(7), GROUND);
            dl->AddCircleFilled(c, e ? U(3) : U(5.5f), col);
            if (joint_picked(T, b, e)) {
                dl->AddCircle(c, e ? U(7) : U(10), INK, 0, U(2));
            }
            // the dot under the mouse: the nearest, a bone's own (its start) before another's end there (the spine's, not the hips'
            // end on it: taken as the hips' end, a drag took every bone starting near it, its thighs too; the user's word, 2026-11-23:
            // "when i move the spine node on the hero setup it turns into thigh _r")
            float d2 = (mouse.x - c.x) * (mouse.x - c.x) + (mouse.y - c.y) * (mouse.y - c.y);
            if (d2 < U(8) * U(8) && (d2 + 0.5f < pick_d2 || (fabsf(d2 - pick_d2) <= 0.5f && e == 0 && pick_end == 1))) {
                pick_d2 = d2, pick_bone = b, pick_end = e;
            }
        }
    }
    dl->PopClipRect();
    // a click: on a dot, it's taken (with Ctrl or Shift, added to those picked or let go of), or those picked, it among them, all moved
    // together; on nothing, a box begun to pick those in it (with Ctrl or Shift, as well as those picked)
    ImGuiIO &kio = ImGui::GetIO();
    // (only a click on the view itself: one on its menu over it had let go of those picked and begun a box, before the menu's Straighten
    // saw them, so it did nothing; the user's word, 2026-11-23: "its not working on the body. the nodes never move")
    bool on_view = in_view && ImGui::IsItemHovered();
    bool adding = kio.KeyCtrl || kio.KeyShift;
    if (on_view && T.drag_bone < 0 && T.jbox_view < 0 && ImGui::IsMouseClicked(0)) {
        auto to_hand = [&]() {
            // a rigged model's joints placed by hand: the tool weights it to them from now (its own clips kept, export_pack)
            if (T.rigged) {
                T.rigged = false;
                snprintf(T.status, sizeof(T.status), "Its joints are yours to place now: the tool weights it to them");
            }
        };
        if (pick_bone >= 0 && adding) {
            if (joint_picked(T, pick_bone, pick_end)) {
                T.joint_sel.erase(std::remove(T.joint_sel.begin(), T.joint_sel.end(), std::make_pair(pick_bone, pick_end)), T.joint_sel.end());
            } else {
                T.joint_sel.push_back({ pick_bone, pick_end });
            }
        } else if (pick_bone >= 0 && joint_picked(T, pick_bone, pick_end) && T.joint_sel.size() > 1) {
            joint_snapshot(T, "Moved %d joints", (int)T.joint_sel.size());
            T.drag_bone = pick_bone, T.drag_end = pick_end, T.drag_view = which, T.multi_drag = true;
            to_hand();
        } else if (pick_bone >= 0) {
            T.joint_sel.assign(1, { pick_bone, pick_end });
            joint_snapshot(T, "Moved %s%s", KitBoneNames[pick_bone], pick_end ? "'s end" : "");
            T.drag_bone = pick_bone, T.drag_end = pick_end, T.drag_view = which, T.multi_drag = false;
            to_hand();
        } else {
            if (!adding) {
                T.joint_sel.clear();
            }
            T.jbox_view = which, T.jbox_x = mouse.x, T.jbox_y = mouse.y;
        }
    }
    if (pick_bone >= 0 && on_view && T.drag_bone < 0 && T.jbox_view < 0) {
        ImGui::SetTooltip("%s%s", KitBoneNames[pick_bone], pick_end ? " (its end)" : "");
    }
    // a right click (not a drag: that moves the view) opens the joints' menu, a dot under it not picked taken alone first
    ImGui::PushID(which);
    if (on_view && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && kio.MouseDragMaxDistanceSqr[ImGuiMouseButton_Right] < U(4) * U(4)
        && T.drag_bone < 0) {
        if (pick_bone >= 0 && !joint_picked(T, pick_bone, pick_end)) {
            T.joint_sel.assign(1, { pick_bone, pick_end });
        }
        ImGui::OpenPopup("##jointmenu");
    }
    if (ImGui::BeginPopup("##jointmenu")) {
        char says[48];
        snprintf(says, sizeof(says), "Snap (%d joints)", (int)T.joint_sel.size());
        ImGui::BeginDisabled(T.joint_sel.size() < 2);
        if (menu_item(says, false)) {
            joints_to_grid(T, which);
        }
        ImGui::EndDisabled();
        ImGui::SetItemTooltip(T.joint_sel.size() < 2 ? "Pick two or more first: drag a box round them, or Ctrl-click them"
                                                      : T.joint_grid ? "Onto the nearest grid line: up and down, or across, whichever way they're spread"
                                                                     : "In a line up and down, or across, whichever way they're spread (with the grid on, onto its nearest line)");
        ImGui::BeginDisabled(T.joint_sel.size() < 3);
        if (menu_item("Line up between the ends", false)) {
            joints_straighten(T);
        }
        ImGui::EndDisabled();
        ImGui::SetItemTooltip("On the line between the two farthest apart (a line that isn't up and down or across)");
        ImGui::BeginDisabled(T.joint_sel.empty());
        if (menu_item("Let go of them", false)) {
            T.joint_sel.clear();
        }
        ImGui::EndDisabled();
        ImGui::EndPopup();
    }
    ImGui::PopID();
    // the box: drawn as it's dragged, and those in it picked when it's let go
    if (T.jbox_view == which) {
        ImVec2 a(fminf(T.jbox_x, mouse.x), fminf(T.jbox_y, mouse.y)), b(fmaxf(T.jbox_x, mouse.x), fmaxf(T.jbox_y, mouse.y));
        if (ImGui::IsMouseDown(0)) {
            dl->PushClipRect(lo, hi, true);
            dl->AddRectFilled(a, b, (PERI & 0x00FFFFFFu) | 0x30000000u, U(4));
            dl->AddRect(a, b, PERI, U(4), 0, U(1.5f));
            dl->PopClipRect();
        } else {
            if (b.x - a.x > U(3) || b.y - a.y > U(3)) {
                for (int bb = 0; bb < KB_COUNT; bb++) {
                    for (int e = 0; T.m->bones[bb].have && e < 2; e++) {
                        float x, y, z;
                        to_screen(v, e ? T.m->bones[bb].end : T.m->bones[bb].at, x, y, z);
                        ImVec2 c(at.x + x, at.y + y);
                        if (c.x >= a.x && c.x <= b.x && c.y >= a.y && c.y <= b.y && !joint_picked(T, bb, e)) {
                            T.joint_sel.push_back({ bb, e });
                        }
                    }
                }
            }
            T.jbox_view = -1;
        }
    }
    // dragging: across and up in the front view, through and up in the side view
    if (T.drag_bone >= 0 && T.drag_view == which && T.multi_drag && !ImGui::IsMouseDown(0)) {
        T.multi_drag = false;
    }
    if (T.drag_bone >= 0 && T.drag_view == which && T.multi_drag && ImGui::IsMouseDown(0)) {
        // those picked moved together as far as the mouse went (across and up in the front view, through and up in the side view), each
        // with the one joint it is (the bone carrying on from it), and with Mirror on each one's other side's too, mirrored, unless
        // it's picked itself; a place moved once however many of those it's in
        ImGuiIO &mio = ImGui::GetIO();
        float dx = mio.MouseDelta.x / v.scale, d3[3] = { which == 0 ? dx : 0, -mio.MouseDelta.y / v.scale, which == 0 ? 0 : -dx };
        std::vector<float *> moved, mirrored;
        auto add = [&](std::vector<float *> &list, int b, int e) {
            auto put = [&](float *q) {
                if (std::find(list.begin(), list.end(), q) == list.end()) {
                    list.push_back(q);
                }
            };
            put(e ? T.m->bones[b].end : T.m->bones[b].at);
            for (int c = 0; c < KB_COUNT; c++) {
                if (T.m->bones[c].have && e && kit_carries_on(b, c)) {
                    put(T.m->bones[c].at);
                }
                if (T.m->bones[c].have && !e && kit_carries_on(c, b)) {
                    put(T.m->bones[c].end);
                }
            }
        };
        for (auto &j : T.joint_sel) {
            add(moved, j.first, j.second);
        }
        for (auto &j : T.joint_sel) {
            int o = mirror_bone(j.first);
            if (T.mirror && o >= 0 && T.m->bones[o].have && !joint_picked(T, o, j.second)) {
                add(mirrored, o, j.second);
            }
        }
        for (float *q : moved) {
            q[0] += d3[0], q[1] += d3[1], q[2] += d3[2];
        }
        for (float *q : mirrored) {
            if (std::find(moved.begin(), moved.end(), q) == moved.end()) {
                q[0] -= d3[0], q[1] += d3[1], q[2] += d3[2];
            }
        }
    } else if (T.drag_bone >= 0 && T.drag_view == which) {
        if (ImGui::IsMouseDown(0)) {
            const float *was = T.drag_end ? T.m->bones[T.drag_bone].end : T.m->bones[T.drag_bone].at;
            float mx = (mouse.x - at.x - v.ox) / v.scale, my = (v.oy - (mouse.y - at.y)) / v.scale, p[3] = { was[0], my, was[2] };

            if (which == 0) {
                p[0] = mx;
            } else {
                p[2] = -mx;
            }
            // a joint's dot put somewhere, and what joins it with it: a bone's end is the next one's start
            // a joint's dot put somewhere, and the one joint it is with it: a bone's end and the start of the bone that carries on from it
            // (kit_carries_on: the hips' end and the spine's start, the chest's and the neck's), never a bone that branches off there (a
            // thigh off the hips, an arm off the chest; before, every bone starting within half its height went: the user's word,
            // 2026-11-23, "nek also turns into upperarm_r")
            auto place = [&](int bone, int end, const float *to) {
                float *d = end ? T.m->bones[bone].end : T.m->bones[bone].at;
                memcpy(d, to, sizeof(float) * 3);
                for (int c = 0; c < KB_COUNT; c++) {
                    if (T.m->bones[c].have && end && kit_carries_on(bone, c)) {
                        memcpy(T.m->bones[c].at, d, sizeof(float) * 3);
                    }
                    if (T.m->bones[c].have && !end && kit_carries_on(c, bone)) {
                        memcpy(T.m->bones[c].end, d, sizeof(float) * 3);
                    }
                }
            };
            place(T.drag_bone, T.drag_end, p);
            // and its other side's, mirrored across the hips (the body's middle)
            const char *name = KitBoneNames[T.drag_bone];
            size_t len = strlen(name);
            float mid = T.m->bones[KB_HIPS].have ? T.m->bones[KB_HIPS].at[0] : (T.m->lo[0] + T.m->hi[0]) / 2;
            for (int o = 0; T.mirror && o < KB_COUNT; o++) {
                const char *on = KitBoneNames[o];
                bool pair = o != T.drag_bone && strlen(on) == len
                            && ((strncmp(on, name, len - 1) == 0 && ((name[len - 1] == 'l' && on[len - 1] == 'r') || (name[len - 1] == 'r' && on[len - 1] == 'l')))
                                // (ears' and others' names end in a digit: ear_l1, ear_r1)
                                || (len > 2 && strncmp(on, name, len - 2) == 0 && on[len - 1] == name[len - 1]
                                    && ((name[len - 2] == 'l' && on[len - 2] == 'r') || (name[len - 2] == 'r' && on[len - 2] == 'l'))));
                if (pair && T.m->bones[o].have) {
                    float q[3] = { 2 * mid - p[0], p[1], p[2] };
                    place(o, T.drag_end, q);
                }
            }
        } else {
            T.drag_bone = -1;
            reweigh(T);
            T.dirty = T.unsaved = true;
            if (SDL_getenv("HEROTOOL_MARKERS") != nullptr) {
                for (int b = 0; b < KB_COUNT; b++) {
                    const KitBone &k = T.m->bones[b];
                    if (k.have) {
                        printf("herotool: %s %.4f,%.4f,%.4f to %.4f,%.4f,%.4f\n", KitBoneNames[b], k.at[0], k.at[1], k.at[2], k.end[0], k.end[1], k.end[2]);
                    }
                }
            }
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
    // the copy opened (the tool's if it has one, else the game's), and each place's mod holding it ("" not there)
    char mod[1100], id[64], name[64], tool_mod[1100], game_mod[1100];
    // its picture (found_picture: its card, or its face as the tool last made it; nullptr none) and the part of it shown, looked for once
    float face_head;
    SDL_Texture *pic;
    ImVec2 uv0, uv1;
    bool looked;
};
bool heroes_home(char *out, size_t size);
const int MaxFound = 64;
Found Founds[MaxFound];
int NumFound = 0;
int Deleting = -1; // the hero whose delete is being asked (DeleteAsk: its trash just clicked)
bool DeleteAsk = false;

struct FindCtx {
    const char *mod;
    bool tool; // in the tool's own folder, else the game's mods folder
};

SDL_EnumerationResult SDLCALL found_hero(void *ctx, const char *dir, const char *name) {
    const FindCtx *fc = (const FindCtx *)ctx;
    const char *mod = fc->mod;
    char path[1400];
    size_t n = 0;

    snprintf(path, sizeof(path), "%s%s/hero.json", dir, name);
    void *text = SDL_LoadFile(path, &n);
    if (text != nullptr && NumFound < MaxFound) {
        PackInfo info;
        char why[256];

        const char *id = info.id[0] ? info.id : name;
        int at = -1;
        for (int i = 0; i < NumFound; i++) {
            at = SDL_strcmp(Founds[i].id, id) == 0 ? i : at;
        }
        if (at >= 0 && pack_info_read((const char *)text, n, &info, why, sizeof(why))) {
            // (the game's copy of one the tool has: noted, the tool's still the one opened)
            SDL_strlcpy(fc->tool ? Founds[at].tool_mod : Founds[at].game_mod, mod, sizeof(Founds[at].game_mod));
        } else if (pack_info_read((const char *)text, n, &info, why, sizeof(why))) {
            Found &f = Founds[NumFound++];
            SDL_strlcpy(f.mod, mod, sizeof(f.mod));
            SDL_strlcpy(f.tool_mod, fc->tool ? mod : "", sizeof(f.tool_mod));
            SDL_strlcpy(f.game_mod, fc->tool ? "" : mod, sizeof(f.game_mod));
            SDL_strlcpy(f.id, info.id[0] ? info.id : name, sizeof(f.id));
            SDL_strlcpy(f.name, info.name[0] ? info.name : name, sizeof(f.name));
            f.face_head = info.face_head;
            f.pic = nullptr;
            f.looked = false;
        }
    }
    SDL_free(text);
    return SDL_ENUM_CONTINUE;
}

SDL_EnumerationResult SDLCALL found_mod(void *where, const char *dir, const char *name) {
    char mod[1100], heroes[1200];
    SDL_PathInfo info;

    snprintf(mod, sizeof(mod), "%s%s", dir, name);
    snprintf(heroes, sizeof(heroes), "%s/heroes", mod);
    if (SDL_GetPathInfo(heroes, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
        FindCtx fc = { mod, where != nullptr };
        SDL_EnumerateDirectory(heroes, found_hero, &fc);
    }
    return SDL_ENUM_CONTINUE;
}

void find_heroes(const Tool &T) {
    for (int i = 0; i < NumFound; i++) {
        if (Founds[i].pic != nullptr) {
            SDL_DestroyTexture(Founds[i].pic);
        }
        Founds[i].pic = nullptr;
    }
    NumFound = 0;
    // the tool's own first (the copy opened), then the game's
    char home[1100];
    static bool tool = true;
    if (heroes_home(home, sizeof(home))) {
        SDL_EnumerateDirectory(home, found_mod, &tool);
    }
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
        for (int k = 0; k < PACK_KINDS; k++) {
            int th = info.kinds[k].template_hero;
            T.kind_template[k] = th >= 1 && th <= 8 && th - 1 != T.template_hero ? th - 1 : -1;
            T.no_fit[k] = info.kinds[k].no_fit != 0;
        }
        T.foot_own = !info.template_said || info.kinds[0].template_hero < 0;
    }
    SDL_free(text);
    snprintf(path, sizeof(path), "%s/mod.json", f.mod);
    text = SDL_LoadFile(path, &n);
    json_string((const char *)text, "author", T.author, sizeof(T.author));
    SDL_free(text);
    // its card and sounds, as it has them; its own moves are already its clips (KIND/MOVE), so none are added
    SDL_PathInfo pi;
    snprintf(path, sizeof(path), "%s/heroes/%s/card.png", f.mod, f.id);
    SDL_strlcpy(T.card, SDL_GetPathInfo(path, &pi) ? path : "", sizeof(T.card));
    T.card_photo = false;
    T.camera = false;
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        snprintf(path, sizeof(path), "%s/heroes/%s/voice_%s.wav", f.mod, f.id, PackVoiceJobs[j]);
        SDL_strlcpy(T.voice[j], SDL_GetPathInfo(path, &pi) ? path : "", sizeof(T.voice[0]));
    }
    SDL_strlcpy(T.loaded_mod, f.mod, sizeof(T.loaded_mod));
    SDL_strlcpy(T.game_mod, f.game_mod, sizeof(T.game_mod));
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

// --- the window (Graphite, the user's pick, 2026-11-22; docs/HERO_PACKS.md, The look): a top bar (back, the hero, the status, undo and
// redo, sounds, preview it in a game, save), a strip of the games with the hero in each, and the page: a game's (its picture, its timeline,
// its cards), the hero's own (its model, card and sounds, details), or every game at a glance ---

// each game's name in the strip, and the chips' tints in turn
const char *const KindChip[PACK_KINDS] = { "On foot",    "Polar Push",  "Pogo Painter", "Ballistix",  "Tank",  "Crash Dash",
                                           "Splash Dash", "Mallet Mash", "Dragon Drop",  "Keg Kaboom", "Podium" };
const ImU32 ChipTint[4] = { TINT_CORAL, TINT_PERI, TINT_MINT, TINT_SAND };

// Sounds on or off, kept for next time (sounds.txt beside the zoom's)
void sound_keep() {
    char *pref = pref_path(), path[1100];
    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%ssounds.txt", pref);
    SDL_free(pref);
    SDL_SaveFile(path, sound_muted() ? "off\n" : "on\n", sound_muted() ? 4 : 3);
}

void sound_load() {
    char *pref = pref_path(), path[1100];
    size_t n = 0;
    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%ssounds.txt", pref);
    SDL_free(pref);
    char *text = (char *)SDL_LoadFile(path, &n);
    if (text != nullptr) {
        sound_mute(strncmp(text, "off", 3) == 0);
        SDL_free(text);
    }
}

// The strip of games shown or not, kept for next time (strip.txt beside the sounds'); a test's folder has its own
void strip_keep(const Tool &T) {
    char *pref = pref_path(), path[1100];
    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%sstrip.txt", pref);
    SDL_free(pref);
    SDL_SaveFile(path, T.strip_shown ? "on\n" : "off\n", T.strip_shown ? 3 : 4);
}

void strip_load(Tool &T) {
    char *pref = pref_path(), path[1100];
    size_t n = 0;
    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%sstrip.txt", pref);
    SDL_free(pref);
    char *text = (char *)SDL_LoadFile(path, &n);
    if (text != nullptr) {
        T.strip_shown = strncmp(text, "off", 3) != 0;
        SDL_free(text);
    }
}

// To a page: a game's (a kind), the hero's own (PAGE_MODEL) or every game's (PAGE_ALL)
void go_page(Tool &T, int page) {
    if (page == T.page) {
        return;
    }
    T.camera = false;
    sound_cue(CUE_NAV);
    T.page = page;
    if (page >= 0) {
        T.kind = page;
        T.anim = T.have_made[page] ? breathe_of(&T.made[page]) : 0;
        T.made_t = 0;
        T.frame = 0;
    }
}

void top_bar(Tool &T) {
    ImGuiStyle &st = ImGui::GetStyle();
    float bh = ImGui::GetFrameHeight() + U(6), top = ImGui::GetCursorPosY(), sp = U(10);
    auto mid = [&](float h) { ImGui::SetCursorPosY(top + (bh - h) / 2); };
    if (icon_button("##back", IC_BACK, "Back to your heroes", CARD)) {
        T.home = true;
        find_heroes(T);
    }
    ImGui::SameLine(0, U(12));
    ImGui::PushFont(FontBig);
    mid(ImGui::GetTextLineHeight());
    ImGui::TextUnformatted(T.name);
    ImGui::PopFont();
    ImGui::SameLine(0, U(10));
    mid(U(36));
    bool all = T.page == PAGE_ALL;
    if (button("##all", all ? PERI : CARD, all ? PERI_INK : INK, U(36), IC_GRID, CUE_NAV, U(36))) {
        go_page(T, all ? (T.have_made[T.kind] ? T.kind : PAGE_MODEL) : PAGE_ALL);
    }
    ImGui::SetItemTooltip("All games at a glance");
    // the strip of games shown or hidden (more room for the picture); hidden, a menu of the pages in its place, named for the one shown
    ImGui::SameLine(0, U(10));
    mid(U(36));
    if (button("##strip", T.strip_shown ? CARD : PERI, T.strip_shown ? INK : PERI_INK, U(36), T.strip_shown ? IC_UP : IC_DOWN,
               T.strip_shown ? CUE_HIDE : CUE_SHOW, U(36))) {
        T.strip_shown = !T.strip_shown;
        strip_keep(T);
    }
    ImGui::SetItemTooltip(T.strip_shown ? "Hide the games (more room)" : "Show the games");
    if (!T.strip_shown) {
        ImGui::SameLine(0, U(10));
        mid(ImGui::GetFrameHeight());
        const char *shown = T.page == PAGE_MODEL ? "Hero Setup" : T.page == PAGE_ALL ? "All games" : KindChip[T.page];
        if (menu_begin("##pages", shown, U(170))) {
            if (menu_item("Hero Setup", T.page == PAGE_MODEL)) {
                go_page(T, PAGE_MODEL);
            }
            if (menu_item("All games", T.page == PAGE_ALL)) {
                go_page(T, PAGE_ALL);
            }
            for (int k = 0; k < PACK_KINDS; k++) {
                if (menu_item(KindChip[k], T.page == k)) {
                    go_page(T, k);
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SetItemTooltip("The page shown: Hero Setup, All games or a game");
    }
    ImGui::SameLine(0, U(10));
    mid(U(28));
    bool busy = T.maker != nullptr, stale = T.dirty && T.made_asked, warn = busy || stale || T.unsaved;
    tag(busy ? "Making..." : stale ? "Preview out of date" : T.unsaved ? "Not saved yet" : "All saved", warn ? TINT_PERI : TINT_MINT,
        warn ? PERI_LIGHT : MINT_LIGHT, warn ? PERI : MINT);
    // on the right: undo, redo, sounds, preview it in a game (and which), save
    // (a game's page opened: its own game, kept if the one picked already shows its kind, as Ring Ding on foot; the arrow picks
    // another till the next page)
    if (T.page != T.mode_page) {
        T.mode_page = T.page;
        for (int i = 0; T.page >= 0 && i < (int)(sizeof(Modes) / sizeof(Modes[0])) && Modes[T.mode].kind != T.page; i++) {
            T.mode = Modes[i].kind == T.page ? i : T.mode;
        }
    }
    char play[64];
    snprintf(play, sizeof(play), "Preview in %s##play", Modes[T.mode].name);
    float rw = 3 * (bh + sp) + button_w(play, true) + U(36) + sp + button_w("Add to game", true) + sp + U(100);
    ImGui::SameLine(ImGui::GetWindowWidth() - st.WindowPadding.x - rw);
    mid(bh);
    // (on Hero Setup its joints', else its keys')
    bool joints = T.page == PAGE_MODEL;
    ImGui::BeginDisabled(joints ? T.joint_undo.empty() : T.undo.empty());
    if (icon_button("##undo", IC_UNDO, "Undo (Ctrl+Z)", CARD, INK, bh, CUE_OFF)) {
        joints ? joint_undo_step(T) : adj_undo(T);
    }
    ImGui::EndDisabled();
    ImGui::SameLine(0, sp);
    ImGui::BeginDisabled(joints ? T.joint_redo.empty() : T.redo.empty());
    if (icon_button("##redo", IC_REDO, "Redo (Ctrl+Y)", CARD, INK, bh, CUE_ON)) {
        joints ? joint_redo_step(T) : adj_redo(T);
    }
    ImGui::EndDisabled();
    ImGui::SameLine(0, sp);
    bool muted = sound_muted();
    if (icon_button("##sound", muted ? IC_MUTE : IC_SOUND, muted ? "Sounds off" : "Sounds on", CARD)) {
        sound_mute(!muted);
        sound_cue(CUE_ON);
        sound_keep();
    }
    ImGui::SameLine(0, sp);
    if (button(play, TINT_MINT, MINT_LIGHT, 0, IC_GAMEPAD, CUE_SAVE)) {
        export_pack(T, true);
    }
    ImGui::SetItemTooltip("Save, then see it in %s (it needn't be added to the game)", Modes[T.mode].name);
    ImGui::SameLine(0, U(2));
    if (button("##modes", TINT_MINT, MINT_LIGHT, U(36), IC_DOWN, CUE_OPEN)) {
        ImGui::OpenPopup("##playin");
    }
    ImGui::SetItemTooltip("Pick the game to preview it in");
    if (ImGui::BeginPopup("##playin")) {
        for (int i = 0; i < (int)(sizeof(Modes) / sizeof(Modes[0])); i++) {
            if (menu_item(Modes[i].name, T.mode == i)) {
                T.mode = i;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine(0, sp);
    // (greyed out till it's saved as it is: the user's word, 2026-11-23, "the add to game button should be greyed out until saved")
    bool can_add = saved_in_tool(T);
    ImGui::BeginDisabled(!can_add);
    if (button("Add to game", TINT_PERI, PERI_LIGHT, 0, IC_CHECK, CUE_SAVE)) {
        add_to_game(T);
    }
    ImGui::EndDisabled();
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_ForTooltip)) {
        if (can_add) {
            ImGui::SetTooltip("Puts its saved copy in the game's mods folder (%s) and switches it on there", T.out);
        } else {
            ImGui::SetTooltip("Save it first");
        }
    }
    ImGui::SameLine(0, sp);
    if (button("Save", CORAL, CORAL_INK, U(100), IC_NONE, CUE_SAVE)) {
        export_pack(T, false);
    }
    {
        char home[1100];
        heroes_home(home, sizeof(home));
        ImGui::SetItemTooltip("Saves it in the tool's own folder (%s); Add to game puts it in the game", T.loaded_mod[0] ? T.loaded_mod : home);
    }
    ImGui::SetCursorPosY(top + bh + U(10));
}

// A game's chip in the strip (or the hero's own, PAGE_MODEL): the hero in it, its name; the page shown periwinkle and lifted
void chip(Tool &T, int page, const char *name, SDL_Texture *tex, ImU32 tint, bool made) {
    ImGui::PushID(page + 10);
    float w = U(92), h = U(104);
    ImVec2 p = ImGui::GetCursorScreenPos();
    bool hit = ImGui::InvisibleButton("##chip", ImVec2(w, h));
    bool hov = ImGui::IsItemHovered(), on = T.page == page;
    ImGuiID id = ImGui::GetItemID();
    float t = ease_to(id, on ? 1 : 0, 16), lift = ease_to(id + 1, hov || on ? 1 : 0, 18), squash = ease_to(id + 2, ImGui::IsItemActive() ? 1 : 0, 30);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 a(p.x + squash * U(3), p.y + U(5) - lift * U(5)), b(p.x + w - squash * U(3), p.y + h - lift * U(5));
    if (t > 0) {
        dl->AddRectFilled(ImVec2(a.x - U(4), a.y - U(4)), ImVec2(b.x + U(4), b.y + U(4)), (PERI & 0x00FFFFFFu) | (ImU32)(t * 70) << 24, U(26));
    }
    dl->AddRectFilled(a, b, mix(tint, PERI, t), U(22));
    ImU32 ink = mix(made ? INK : DIM, PERI_INK, t);
    if (tex != nullptr) {
        dl->AddImage((ImTextureID)(intptr_t)tex, ImVec2((a.x + b.x) / 2 - U(36), a.y + U(4)), ImVec2((a.x + b.x) / 2 + U(36), a.y + U(76)));
    } else {
        icon(dl, page == PAGE_MODEL ? IC_PERSON : IC_CUBE, ImVec2((a.x + b.x) / 2, a.y + U(40)), U(30), ink);
    }
    ImGui::PushFont(FontSmall);
    ImVec2 ts = ImGui::CalcTextSize(name);
    dl->AddText(ImVec2((a.x + b.x) / 2 - ts.x / 2, b.y - U(10) - ts.y), ink, name);
    ImGui::PopFont();
    if (!made) {
        ImGui::SetItemTooltip("Not made yet");
    }
    if (hit) {
        go_page(T, page);
    }
    ImGui::PopID();
    ImGui::SameLine(0, U(10));
}

// The strip of the games: the hero's own first, then each game, the hero in it (a still of its first animation)
void games_strip(Tool &T) {
    ImGui::BeginChild("##strip", ImVec2(0, U(118)), 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseWheel != 0) {
        ImGui::SetScrollX(ImGui::GetScrollX() - ImGui::GetIO().MouseWheel * U(90));
    }
    chip(T, PAGE_MODEL, "Hero Setup", nullptr, TINT_PERI, true);
    for (int k = 0; k < PACK_KINDS; k++) {
        chip(T, k, KindChip[k], T.have_made[k] ? T.strip[k].tex : nullptr, ChipTint[k % 4], T.have_made[k]);
    }
    ImGui::EndChild();
}

// How many rows the timeline has: each bone with a change in this animation (or all of this game's), and the one picked
int timeline_rows(Tool &T, const char *move) {
    const KitPose *po = &T.poses[T.kind];
    int n = 0;
    for (int b = 0; b < po->nbones; b++) {
        bool has = b == T.sel;
        for (auto &k : T.adj) {
            has |= pack_adjust_for(&k, T.kind, move) && SDL_strcasecmp(k.bone, po->names[b]) == 0;
        }
        n += has;
    }
    return n;
}

// The keys and mouse, a card over the picture's corner (?)
void help_card(Tool &T, ImVec2 at) {
    static const char *const Rows[][2] = {
        { "Click", "pick a bone" }, { "Esc", "let go of it" }, { "R  G  S", "rotate, move, scale" }, { "Tab", "bones on or off" },
        { "Right-drag", "orbit" }, { "Wheel", "zoom" }, { "Home", "the view back" }, { "1  3  7", "front, side, above (keypad)" },
        { "Space", "play" }, { "Left  Right", "step a frame" }, { "Ctrl+C  Ctrl+V", "copy, paste a key" }, { "Delete", "remove a key" },
        { "Ctrl+Z  Ctrl+Y", "undo, redo" },
    };
    const int n = (int)(sizeof(Rows) / sizeof(Rows[0]));
    ImGui::PushFont(FontSmall);
    float line = ImGui::GetTextLineHeight() + U(9), w = U(330), h = n * line + U(26);
    ImVec2 lo(at.x, at.y - h), hi(at.x + w, at.y);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(ImVec2(lo.x, lo.y + U(4)), ImVec2(hi.x, hi.y + U(4)), SHADOW, U(20));
    dl->AddRectFilled(lo, hi, RAISED, U(20));
    for (int i = 0; i < n; i++) {
        float y = lo.y + U(13) + i * line;
        ImVec2 ks = ImGui::CalcTextSize(Rows[i][0]);
        dl->AddRectFilled(ImVec2(lo.x + U(14), y - U(2)), ImVec2(lo.x + U(30) + ks.x, y + ks.y + U(2)), HOVER, U(8));
        dl->AddText(ImVec2(lo.x + U(22), y), INK, Rows[i][0]);
        dl->AddText(ImVec2(lo.x + U(140), y), MUTED, Rows[i][1]);
    }
    ImGui::PopFont();
    (void)T;
}

// The picture's own buttons, over it: Bones and the tools (its top left), X-Mirror, Onion and Clipping (top right), the keys and mouse
// (bottom left; the axes are bottom right, view_axes)
float picture_overlays(Tool &T, ImVec2 p, ImVec2 size) {
    float below = p.y + U(16);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 keep = ImGui::GetCursorScreenPos();
    // Bones: shown to pose, or not, a clean view of the hero (Tab)
    ImGui::SetCursorScreenPos(ImVec2(p.x + U(16), p.y + U(16)));
    {
        float w = U(64), h = U(56);
        ImVec2 q = ImGui::GetCursorScreenPos();
        if (ImGui::InvisibleButton("##bones", ImVec2(w, h))) {
            T.bones = !T.bones;
            sound_cue(T.bones ? CUE_ON : CUE_OFF);
        }
        ImGui::SetItemTooltip("Bones (Tab): shown to pose; off, a clean view of the hero");
        float t = ease_to(ImGui::GetItemID(), T.bones ? 1 : 0, 16), lift = ease_to(ImGui::GetItemID() + 1, ImGui::IsItemHovered() ? 1 : 0, 18);
        ImVec2 a(q.x, q.y - lift * U(2)), b(q.x + w, q.y + h - lift * U(2));
        dl->AddRectFilled(ImVec2(a.x, a.y + U(3)), ImVec2(b.x, b.y + U(3)), SHADOW, U(20));
        dl->AddRectFilled(a, b, mix(RAISED, PERI, t), U(20));
        ImU32 ink = mix(INK, PERI_INK, t);
        icon(dl, IC_BONES, ImVec2((a.x + b.x) / 2, a.y + U(21)), U(22), ink);
        ImGui::PushFont(FontSmall);
        ImVec2 ts = ImGui::CalcTextSize("Bones");
        dl->AddText(ImVec2((a.x + b.x) / 2 - ts.x / 2, b.y - U(6) - ts.y), ink, "Bones");
        ImGui::PopFont();
    }
    // the tools, a column: the one in use periwinkle, sliding to it
    {
        static const char *const Tips[3] = { "Rotate (R): rings rotate, the middle aims", "Move (G): arrows move, the middle drags",
                                             "Scale (S): handles scale, the middle scales all" };
        static const int Icons[3] = { IC_ROTATE, IC_MOVE, IC_SCALE };
        float s = U(52), pad = U(6);
        ImVec2 q(p.x + U(16) + U(32) - s / 2 - pad, p.y + U(16) + U(56) + U(12));
        ImGui::BeginDisabled(!T.bones);
        dl->AddRectFilled(ImVec2(q.x, q.y + U(3)), ImVec2(q.x + s + 2 * pad, q.y + 3 * s + 2 * pad + U(3)), ImGui::GetColorU32(SHADOW), U(26));
        dl->AddRectFilled(q, ImVec2(q.x + s + 2 * pad, q.y + 3 * s + 2 * pad), ImGui::GetColorU32(RAISED), U(26));
        float y = ease_to(ImGui::GetID("##toolthumb"), (float)T.gizmo, 14);
        dl->AddRectFilled(ImVec2(q.x + pad, q.y + pad + y * s), ImVec2(q.x + pad + s, q.y + pad + (y + 1) * s), ImGui::GetColorU32(PERI), U(20));
        for (int i = 0; i < 3; i++) {
            ImGui::SetCursorScreenPos(ImVec2(q.x + pad, q.y + pad + i * s));
            ImGui::PushID(i);
            if (ImGui::InvisibleButton("##tool", ImVec2(s, s)) && T.gizmo != i) {
                T.gizmo = i;
                sound_cue(CUE_TAP);
            }
            ImGui::SetItemTooltip("%s", Tips[i]);
            ImGui::PopID();
            icon(dl, Icons[i], ImVec2(q.x + pad + s / 2, q.y + pad + i * s + s / 2), U(22), ImGui::GetColorU32(T.gizmo == i ? PERI_INK : INK));
        }
        ImGui::EndDisabled();
    }
    // Stretch, X-Mirror, Onion and Clipping, top right (Onion's frames apart, and the points clipping here, under them while they're on)
    {
        const char *names[4] = { "Stretch", "X-Mirror", "Onion", "Clipping" };
        const char *tips[4] = { "Moving a bone stretches the one it hangs from. Off: it rotates that one instead, so no bone gets longer or shorter",
                                "Changes to one side made to the other too, flipped", "Ghosts of the frames before (blue) and after (orange)",
                                "Points of it inside another part (red), and the frames where (the timeline's red)" };
        bool *flags[4] = { &T.stretch, &T.xmirror, &T.onion, &T.clipping };
        const int icons[4] = { IC_SCALE, IC_MIRROR, IC_ONION, IC_CLIP };
        float total = 0;
        for (int i = 0; i < 4; i++) {
            total += button_w(names[i], true) + (i > 0 ? U(8) : 0);
        }
        ImGui::SetCursorScreenPos(ImVec2(p.x + size.x - U(16) - total, p.y + U(16)));
        for (int i = 0; i < 4; i++) {
            if (i > 0) {
                ImGui::SameLine(0, U(8));
            }
            toggle(names[i], flags[i], icons[i], tips[i]);
        }
        float y = p.y + U(16) + ImGui::GetFrameHeight() + U(18);
        if (T.onion) {
            ImGui::SetCursorScreenPos(ImVec2(p.x + size.x - U(16) - U(130), y));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(RAISED));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
            ImGui::SetNextItemWidth(U(130));
            ImGui::SliderInt("##onion", &T.onion_step, 1, 15, "ghosts %d apart");
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
            ImGui::SetItemTooltip("Frames between the ghosts and the frame shown");
            y += ImGui::GetFrameHeight() + U(8);
        }
        if (T.clipping && Clips.kind == T.kind && T.frame >= 0 && T.frame < (int)Clips.count.size()) {
            char says[48];
            snprintf(says, sizeof(says), "%d clipping here", Clips.count[T.frame]);
            float w = ImGui::CalcTextSize(says).x + U(40);
            ImGui::SetCursorScreenPos(ImVec2(p.x + size.x - U(16) - w, y));
            bool any = Clips.count[T.frame] > 0;
            tag(says, any ? TINT_CORAL : TINT_MINT, any ? CORAL : MINT_LIGHT, any ? CORAL : MINT);
            y += U(28) + U(8);
        }
        below = y;
    }
    // the keys and mouse (?), bottom left
    ImGui::SetCursorScreenPos(ImVec2(p.x + U(16), p.y + size.y - U(16) - U(40)));
    ImGui::PushFont(FontTitle);
    toggle("?##help", &T.help, IC_NONE, "Keys and mouse", U(40));
    ImGui::PopFont();
    if (T.help) {
        help_card(T, ImVec2(p.x + U(16), p.y + size.y - U(16) - U(40) - U(10)));
    }
    ImGui::SetCursorScreenPos(keep);
    return below;
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
    make_in_game(T);
}

// The yes asked before a move of its own is given up (own_change), as a window over the rest
void ask_back(Tool &T) {
    if (!ImGui::BeginPopupModal("Replace its own animation?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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
    ImGui::TextDisabled("Own animations");
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
    const ImU32 Axis[3] = { AXIS_X, AXIS_Y, AXIS_Z };
    const ImU32 Words[3] = { IM_COL32(0xF7, 0xB4, 0xA6, 255), MINT_LIGHT, IM_COL32(0xB9, 0xC7, 0xF7, 255) };
    static const char *const Letter[3] = { "X", "Y", "Z" };
    bool changed = false;

    ImGui::PushID(label);
    ImGui::AlignTextToFramePadding();
    ImGui::TextColored(V4(MUTED), "%s", label);
    float w = (ImGui::GetContentRegionAvail().x - U(60)) / 3 - ImGui::GetStyle().ItemSpacing.x;
    for (int i = 0; i < 3; i++) {
        char f[24];
        snprintf(f, sizeof(f), "%s %s", Letter[i], fmt);
        ImGui::SameLine(i == 0 ? U(60) : 0);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(mix(TINT_PERI, Axis[i], 0.18f)));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, V4(mix(TINT_PERI, Axis[i], 0.28f)));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, V4(mix(TINT_PERI, Axis[i], 0.38f)));
        ImGui::PushStyleColor(ImGuiCol_Text, V4(Words[i]));
        ImGui::SetNextItemWidth(w);
        ImGui::PushID(i);
        changed |= ImGui::DragFloat("##v", &v[i], speed, lo, hi, f);
        ImGui::PopID();
        *taken |= ImGui::IsItemActivated();
        ImGui::PopStyleColor(4);
        ImGui::SetItemTooltip("%s", tips[i]);
    }
    ImGui::PopID();
    return changed;
}

// The animation's place, and the key picked's: the move shown, its frames, the key the panel's for (adjust's cards share these)
struct Posing {
    const KitMade *mk;
    const KitPose *po;
    int a, frames;
    const char *move;
};

bool posing(Tool &T, Posing &P) {
    if (T.kind < 0 || T.kind >= PACK_KINDS || !T.have_pose[T.kind] || !T.have_made[T.kind] || T.made[T.kind].nanims == 0) {
        return false;
    }
    P.mk = &T.made[T.kind];
    P.po = &T.poses[T.kind];
    P.a = T.anim < P.mk->nanims ? T.anim : 0;
    P.frames = P.mk->anims[P.a].frames;
    P.move = P.mk->anims[P.a].name;
    return true;
}

// The Fade card: how the key picked (or each of those picked) fades in and out, over how many frames each side, and whether it flows
// into the bone's next key (the user's word, 2026-11-22: "the transion effect should be chooseable")
void fade_card(Tool &T) {
    Posing P;
    if (T.kind == KIND_WARP_TOOL || !posing(T, P) || T.key_mode != KEY_FRAME) {
        return;
    }
    // the picked bone's key here: its fade shown, and changed with these (else they're what a new key gets); several picked, all of them
    PackAdjust *here_key = T.sel >= 0 && T.sel < P.po->nbones ? panel_key(T, P.move, P.po->names[T.sel]) : nullptr;
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
    card("##fade", TINT_SAND);
    heading("Fade");
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(CARD));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, V4(RAISED));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(U(14), U(9)));
    ImGui::SetNextItemWidth(U(108));
    bool faded = ImGui::SliderInt("##fade", &T.spread, 0, 30, "%d frames");
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
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
    ImGui::SameLine(0, U(6));
    if (menu_begin("##ease", EaseSays[T.ease >= 0 && T.ease < PACK_EASES ? T.ease : 0], U(118))) {
        for (int e = 0; e < PACK_EASES; e++) {
            if (menu_item(EaseSays[e], e == T.ease) && e != T.ease) {
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
    ImGui::SameLine(0, U(6));
    bool blend = !fades.empty() && fades[0]->blend;
    ImGui::BeginDisabled(fades.empty());
    if (toggle("Blend", &blend, IC_NONE, "Flow into the bone's next key instead of fading out", 0, CARD)) {
        adj_snapshot(T, blend ? "Blended %s into the next" : "Stopped %s blending", whose);
        for (PackAdjust *x : fades) {
            x->blend = blend;
        }
        mirror_all(T, fades);
    }
    ImGui::EndDisabled();
    card_end();
}

// The bone card: the bone picked, its Rotate, Move and Scale, Hide (or Show), and its keys or the animation's all taken away; with
// none picked, a word on picking one
void bone_card(Tool &T) {
    Posing P;
    card("##bone", TINT_PERI);
    if (T.kind == KIND_WARP_TOOL) {
        heading("The podium");
        ImGui::TextDisabled("Made from On foot's animations");
        card_end();
        return;
    }
    if (!posing(T, P)) {
        heading("Pose");
        ImGui::TextWrapped("%s", T.kind == 1 ? "Crash's bear seats its rider itself: pick another hero's bear (Prop from) to pose it"
                                 : T.maker != nullptr ? "Making it in the games..." : "Update the preview first");
        card_end();
        return;
    }
    if (T.sel >= 0 && T.sel < P.po->nbones) {
        const char *bone = P.po->names[T.sel];
        PackAdjust *k = panel_key(T, P.move, bone);
        float turn[3] = { 0, 0, 0 }, reach[3] = { 0, 0, 0 }, size[3] = { 1, 1, 1 };
        if (k != nullptr) {
            memcpy(turn, k->turn, sizeof(turn));
            memcpy(reach, k->reach, sizeof(reach));
            for (int i = 0; i < 3; i++) {
                size[i] = 1 + k->grow[i];
            }
        }
        // (hidden, by its own key or one on a bone it's on: its values change nothing that shows, so they're greyed)
        bool hidden = game_key(T, bone, true) >= 0, unseen = hidden || hidden_bones(T, T.kind)[T.sel];
        heading(bone_title(bone), PERI);
        char note[64];
        snprintf(note, sizeof(note), "%s", unseen ? "Hidden everywhere here" : T.key_mode == KEY_GAME ? "All animations"
                                                       : T.key_mode == KEY_MOVE                    ? "Whole animation"
                                                                                                   : "");
        if (note[0] == 0) {
            snprintf(note, sizeof(note), "Frame %d", T.frame);
        }
        ImGui::SameLine(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize(note).x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + U(3));
        ImGui::TextDisabled("%s", note);
        // its own axes as it stands: x to its left, y down, z to its back, shown in Blender's order X, Y, Z (its x, z, y) and ways (Z
        // up: its y the other way for a move); what each does its tooltip
        static const char *const TurnSays[3] = { "Tip forward", "Lean to its left", "Turn to its right" };
        static const char *const MoveSays[3] = { "To its left", "To its back", "Up" };
        static const char *const SizeSays[3] = { "Wider", "Deeper", "Taller" };
        static const char *const HandSays[3] = { "To its left (its arm or leg bends to follow)", "To its back (its arm or leg bends to follow)",
                                                 "Up (its arm or leg bends to follow)" };
        float t3[3] = { turn[0], turn[2], turn[1] }, m3[3] = { reach[0], reach[2], -reach[1] + 0.0f }, s3[3] = { size[0], size[2], size[1] };
        bool taken_t = false, taken_m = false, taken_s = false;
        ImGui::BeginDisabled(unseen);
        bool changed_t = xyz_row("Rotate", t3, 0.5f, -180, 180, "%.1f", TurnSays, &taken_t);
        bool hand = reach_bone(bone) && SDL_strcasecmp(bone, "hips") != 0;
        bool changed_m = xyz_row("Move", m3, 0.5f, -500, 500, "%.1f", hand ? HandSays : MoveSays, &taken_m);
        bool changed_s = xyz_row("Scale", s3, 0.01f, 0, 10, "%.2f", SizeSays, &taken_s);
        ImGui::EndDisabled();
        if (taken_t) {
            adj_snapshot(T, "Rotated %s", bone_title(bone));
        }
        if (taken_m) {
            adj_snapshot(T, "Moved %s", bone_title(bone));
        }
        if (taken_s) {
            adj_snapshot(T, "Scaled %s", bone_title(bone));
        }
        bool changed = changed_t || changed_m || changed_s;
        turn[0] = t3[0], turn[2] = t3[1], turn[1] = t3[2];
        reach[0] = m3[0], reach[2] = m3[1], reach[1] = -m3[2];
        size[0] = s3[0], size[2] = s3[1], size[1] = s3[2];
        // made to the key it shows; with none, a new one at the frame shown (stopped there, if it was playing)
        if (changed && (k = panel_key(T, P.move, bone)) == nullptr) {
            T.adj_playing = false;
            k = adj_key(T, P.move, bone, true);
        }
        if (changed && k != nullptr) {
            memcpy(k->turn, turn, sizeof(turn));
            memcpy(k->reach, reach, sizeof(reach));
            for (int i = 0; i < 3; i++) {
                k->grow[i] = fmaxf(0, fminf(10, size[i])) - 1;
            }
            mirror_edit(T, (int)(k - T.adj.data()), k->at);
        }
        if (T.key_mode == KEY_FRAME && T.adj_playing && panel_key(T, P.move, bone) == nullptr) {
            ImGui::TextDisabled("Click one of its keys to edit it as it plays");
        }
        ImGui::Dummy(ImVec2(0, U(2)));
        if (button(hidden ? "Show" : "Hide", LILAC, LILAC_INK, 0, hidden ? IC_EYE : IC_EYE_OFF, hidden ? CUE_SHOW : CUE_HIDE)) {
            bone_hide(T, bone, !hidden);
        }
        ImGui::SetItemTooltip(hidden ? "Show it again" : "Hide it, and all on it, in every animation of this game");
        ImGui::SameLine(0, U(8));
        if (button("Reset bone", CARD, INK, 0, IC_NONE, CUE_DROP)) {
            adj_snapshot(T, "Reset %s", bone_title(bone));
            std::vector<PackAdjust> keep;
            for (auto &x : T.adj) {
                if (!(x.kind == T.kind && pack_adjust_for(&x, T.kind, P.move) && SDL_strcasecmp(x.bone, bone) == 0)) {
                    keep.push_back(x);
                }
            }
            T.adj = keep;
            keys_only(T, -1);
        }
        ImGui::SetItemTooltip("Remove this bone's keys in %s", P.move);
    } else {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddCircleFilled(ImVec2(p.x + U(22), p.y + U(22)), U(22), CARD);
        icon(dl, IC_CURSOR, ImVec2(p.x + U(22), p.y + U(22)), U(20), PERI);
        ImGui::SetCursorScreenPos(ImVec2(p.x + U(56), p.y + U(1)));
        ImGui::PushFont(FontTitle);
        ImGui::TextUnformatted("Click a bone");
        ImGui::PopFont();
        ImGui::SetCursorScreenPos(ImVec2(p.x + U(56), ImGui::GetCursorScreenPos().y - U(4)));
        ImGui::TextDisabled("%s", T.bones ? "on the picture to pose it" : "Turn Bones on to pose");
        ImGui::Dummy(ImVec2(0, U(4)));
    }
    if (button("Reset animation", CARD, INK, 0, IC_NONE, CUE_DROP)) {
        adj_snapshot(T, "Reset %s", P.move);
        std::vector<PackAdjust> keep;
        for (auto &x : T.adj) {
            if (!(x.kind == T.kind && SDL_strcasecmp(x.move, P.move) == 0)) {
                keep.push_back(x);
            }
        }
        T.adj = keep;
        keys_only(T, -1);
    }
    ImGui::SetItemTooltip("Remove every key in %s", P.move);
    card_end();
}

// The History card: the last step, and opened, every step (the one it's at lit; clicked, gone back or on to it)
void history_card(Tool &T) {
    card("##history", CARD);
    bool *open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID("##histopen"), false);
    ImVec2 p = ImGui::GetCursorScreenPos();
    float w = ImGui::GetContentRegionAvail().x, h = U(30);
    if (ImGui::InvisibleButton("##histhead", ImVec2(w, h))) {
        *open = !*open;
        sound_cue(*open ? CUE_OPEN : CUE_TAP);
    }
    ImGui::SetItemTooltip(*open ? "Close the history" : "Every step: click one to go back to it");
    ImDrawList *dl = ImGui::GetWindowDrawList();
    icon(dl, IC_HISTORY, ImVec2(p.x + U(10), p.y + h / 2), U(20), INK);
    ImGui::PushFont(FontTitle);
    float th = ImGui::GetTextLineHeight();
    dl->AddText(ImVec2(p.x + U(30), p.y + (h - th) / 2), INK, "History");
    ImGui::PopFont();
    float turn = ease_to(ImGui::GetID("##histturn"), *open ? 1 : 0, 16);
    icon(dl, turn > 0.5f ? IC_DOWN : IC_RIGHT, ImVec2(p.x + w - U(10), p.y + h / 2), U(18), MUTED);
    if (*open) {
        int at = (int)T.undo.size(), all = at + (int)T.redo.size();
        static int shown_at = -1;
        float line = ImGui::GetTextLineHeightWithSpacing();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, V4(DEEP));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, U(16));
        ImGui::BeginChild("##steps", ImVec2(0, fminf((float)(all + 1), 7.0f) * line + U(16)), ImGuiChildFlags_AlwaysUseWindowPadding);
        for (int i = 0; i <= all; i++) {
            const char *says = i == 0 ? T.undo.size() >= 300 ? "(as far back as it goes)" : "(start)"
                             : i <= at ? T.undo[i - 1].says.c_str()
                                       : T.redo[T.redo.size() - (size_t)(i - at)].says.c_str();
            ImGui::PushID(i);
            if (i > at) {
                ImGui::PushStyleColor(ImGuiCol_Text, V4(DIM));
            }
            if (menu_item(says, i == at)) {
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
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
    card_end();
}

// A row of a card: its label (muted) on the left, then the cursor where its control goes, right-aligned to w wide
void row_label(const char *label, float control_w) {
    float right = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x;
    ImGui::AlignTextToFramePadding();
    ImGui::PushStyleColor(ImGuiCol_Text, V4(MUTED));
    ImGui::TextUnformatted(label);
    ImGui::PopStyleColor();
    ImGui::SameLine(right - control_w);
}

// A thin bar that goes back and forth while the games make it again
void making_bar() {
    ImGui::TextColored(V4(PERI_LIGHT), "Making it in the games...");
    ImVec2 p = ImGui::GetCursorScreenPos();
    float w = ImGui::GetContentRegionAvail().x, h = U(8), t = (sinf((float)ImGui::GetTime() * 3) + 1) / 2;
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), CARD, h / 2);
    dl->AddRectFilled(ImVec2(p.x + t * w * 0.7f, p.y), ImVec2(p.x + t * w * 0.7f + w * 0.3f, p.y + h), PERI, h / 2);
    ImGui::Dummy(ImVec2(w, h));
}

// The game's card: whose ride or prop (or on foot, whose animations), Fit in seat where it sits in something, its own animations here
// (each with its base's to go back to), and Update preview when a change wants the games to make it again
void game_card(Tool &T) {
    int k = T.kind, g = game_of(k);
    card("##game", TINT_MINT);
    ImGui::PushTextWrapPos(0);
    heading(KindTitles[k]);
    ImGui::PopTextWrapPos();
    if (k != KIND_WARP_TOOL) {
        char now[64];
        snprintf(now, sizeof(now), "%s's (base)", HeroNames[T.template_hero]);
        bool own = k == 0 && T.foot_own;
        row_label(k == 0 ? "Animations" : k == 1 ? "Bear from" : "Prop from", U(170));
        if (menu_begin("##from", own ? "Its own" : T.kind_template[k] < 0 ? now : HeroNames[T.kind_template[k]], U(170))) {
            // on foot, its own moves: its model's clips, and the tool's for the rest
            if (k == 0 && menu_item("Its own", own)) {
                T.foot_own = true;
                make_in_game(T);
            }
            if (menu_item(now, !own && T.kind_template[k] < 0)) {
                T.kind_template[k] = -1;
                T.foot_own = k == 0 ? false : T.foot_own;
                make_in_game(T);
            }
            for (int h = 0; h < 8; h++) {
                if (h != T.template_hero && menu_item(HeroNames[h], !own && T.kind_template[k] == h)) {
                    T.kind_template[k] = h;
                    T.foot_own = k == 0 ? false : T.foot_own;
                    make_in_game(T);
                }
            }
            ImGui::EndCombo();
        }
        if (k == 0) {
            ImGui::SetItemTooltip("Its own: its clips, and the tool's for any it hasn't got. A hero: animated as they are");
        }
    } else {
        ImGui::TextDisabled("Made from On foot's animations");
    }
    // where it sits in something (a tank's hatch, a race cart): drawn in to fit, as the game's heroes are, or as it's posed
    if (T.have_pose[k] && T.poses[k].cut) {
        bool fit = !T.no_fit[k];
        row_label("Fit in seat", U(50));
        if (switch_("##fit", &fit, "Squeezed in to sit, as the game's heroes are. Off: as you pose it (Hide bones to fit it)")) {
            T.no_fit[k] = !fit;
            T.unsaved = true;
        }
    }
    if (g >= 0) {
        // its own animations here, each with its base's to go back to; its base's aren't listed (the user's word, 2026-11-22: "less is
        // more": a row a move, most of them its base's, said nothing)
        int from = T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero;
        bool any = false;
        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
            int sel = T.ownmap[g][m];
            if (sel == OWN_TEMPLATE) {
                continue;
            }
            char now[80], use[64];
            if (sel == OWN_CLIP) {
                snprintf(now, sizeof(now), "its clip");
            } else if (sel == OWN_MADE) {
                const BakedMove *bm = baked_of(T, k, KitGames[g].moves[m]);
                snprintf(now, sizeof(now), "from %s", HeroNames[bm != nullptr ? bm->from : from]);
            } else {
                snprintf(now, sizeof(now), "my %s", KitMoves[sel].name);
            }
            if (!any) {
                ImGui::PushFont(FontSmall);
                ImGui::TextColored(V4(DIM), "OWN ANIMATIONS");
                ImGui::PopFont();
                ImGui::SetItemTooltip("Kept whoever it's based on; the rest are %s's", HeroNames[from]);
                any = true;
            }
            ImGui::PushID(m);
            ImVec2 p = ImGui::GetCursorScreenPos();
            float w = ImGui::GetContentRegionAvail().x, h = U(40);
            ImDrawList *dl = ImGui::GetWindowDrawList();
            dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), CARD, U(14));
            dl->AddCircleFilled(ImVec2(p.x + U(14), p.y + h / 2), U(4), MINT);
            float lh = ImGui::GetTextLineHeight();
            dl->AddText(ImVec2(p.x + U(26), p.y + (h - lh) / 2), INK, KitGames[g].moves[m]);
            float nw = ImGui::CalcTextSize(KitGames[g].moves[m]).x;
            ImGui::PushFont(FontSmall);
            dl->AddText(ImVec2(p.x + U(34) + nw, p.y + (h - ImGui::GetTextLineHeight()) / 2), DIM, now);
            ImGui::PopFont();
            snprintf(use, sizeof(use), "Use %s's", HeroNames[from]);
            ImGui::PushFont(FontSmall);
            float bw = button_w(use, false);
            ImGui::SetCursorScreenPos(ImVec2(p.x + w - bw - U(6), p.y + U(6)));
            if (button(use, RAISED, INK, bw, IC_NONE, CUE_DROP, h - U(12))) {
                own_change(T, k, g, m, OWN_TEMPLATE);
            }
            ImGui::PopFont();
            ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + h + U(6)));
            ImGui::Dummy(ImVec2(w, 0));
            ImGui::PopID();
        }
        if (!any) {
            ImGui::TextDisabled("All %s's animations", HeroNames[from]);
            ImGui::SetItemTooltip("Change one and it becomes its own");
        }
    } else if (k == 0 && !T.foot_own) {
        foot_own_moves(T);
    }
    if (T.maker != nullptr) {
        making_bar();
    } else if (T.dirty && T.made_asked) {
        if (button("Update preview", TINT_PERI, PERI_LIGHT, ImGui::GetContentRegionAvail().x, IC_REFRESH, CUE_TAP)) {
            make_in_game(T);
        }
        ImGui::SetItemTooltip("Make it again in the games, with your changes");
    }
    card_end();
}

// The model's card: its file (and another opened), its rig, and for one placed by hand: guess its joints, Mirror, what it has
// (a jaw, ears, a tail) and how high it raises its arms
void model_card(Tool &T, SDL_Window *win) {
    static const SDL_DialogFileFilter Glb[] = { { "3D model (glTF binary)", "glb" } };
    card("##model", TINT_PERI);
    heading("Model");
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        float w = ImGui::GetContentRegionAvail().x, h = U(46);
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), CARD, U(16));
        icon(dl, IC_CUBE, ImVec2(p.x + U(20), p.y + h / 2), U(20), PERI);
        float bw = button_w("Open...", false);
        ImGui::PushClipRect(p, ImVec2(p.x + w - bw - U(12), p.y + h), true);
        dl->AddText(ImVec2(p.x + U(38), p.y + (h - ImGui::GetTextLineHeight()) / 2), INK, short_path(T.model));
        ImGui::PopClipRect();
        ImGui::SetCursorScreenPos(ImVec2(p.x + w - bw - U(6), p.y + U(5)));
        if (button("Open...", RAISED, INK, bw, IC_NONE, CUE_OPEN, h - U(10))) {
            browse(win, PICK_MODEL, Glb);
        }
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + h + U(8)));
    }
    if (T.rigged) {
        tag("Rigged (its own bones)", TINT_MINT, MINT_LIGHT, MINT);
        ImGui::SetItemTooltip("Its own weights. Move a joint in Joints and the tool weights it instead (its animations kept)");
    } else {
        tag("No rig: place its joints", CARD, PERI_LIGHT, PERI);
        if (button("Guess joints", PERI, PERI_INK, 0, IC_SPARK, CUE_OPEN)) {
            T.ask_guess = true;
        }
        ImGui::SetItemTooltip("Put the dots where its shape says its joints are (asked first: yours go)");
        ImGui::AlignTextToFramePadding();
        ImGui::TextColored(V4(MUTED), "Has");
        const char *opt[] = { "Jaw", "Ears", "Tail" };
        const int first[] = { KB_JAW, KB_EAR_L1, KB_TAIL1 }, count[] = { 1, 4, 3 };
        for (int o = 0; o < 3; o++) {
            bool has = T.m->bones[first[o]].have;
            ImGui::SameLine(0, U(8));
            if (toggle(opt[o], &has, IC_NONE, nullptr, 0, CARD)) {
                joint_snapshot(T, "%s %s", opt[o], has ? "on" : "off");
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
                T.unsaved = true;
            }
        }
        row_label("Arm lift", U(190));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(CARD));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
        ImGui::SetNextItemWidth(U(190));
        float lift_was = T.m->arm_raise;
        if (ImGui::SliderFloat("##armlift", &T.m->arm_raise, 20, 160, "%.0f deg")) {
            T.dirty = true;
            T.unsaved = true;
        }
        if (ImGui::IsItemActivated()) {
            float now = T.m->arm_raise;
            T.m->arm_raise = lift_was;
            joint_snapshot(T, "Arm lift");
            T.m->arm_raise = now;
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::SetItemTooltip("How high it can raise its arms (less for short arms or a big head)");
    }
    card_end();
}

// A text field in a card: dark, round
bool field(const char *id, char *buf, size_t size, float w, ImGuiInputTextFlags flags = 0, ImU32 bg = CARD) {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(bg));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(14));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(U(14), U(9)));
    ImGui::SetNextItemWidth(w);
    bool changed = ImGui::InputText(id, buf, size, flags);
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    return changed;
}

// Based on another (the user's word, 2026-11-23: "if the base hero is changed like from brio to tiny (height or moves) it should rebuild
// them all but not replace any custom animations"): its height theirs if it was the last one's (a height of its own kept), and every game
// made again now with their moves, its own animations as they were (its moves of its own and its clips: what's its own in each game,
// ownmap and baked, isn't touched, nor a game's own Prop from)
void rebase(Tool &T, int h) {
    if (h == T.template_hero || h < 0 || h >= 8) {
        return;
    }
    if (T.height == HeroHeights[T.template_hero]) {
        T.height = HeroHeights[h];
    }
    T.template_hero = h;
    T.dirty = T.unsaved = true;
    int mine = (int)T.baked.size();
    for (int g = 0; g < KitGameCount && g < 8; g++) {
        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
            mine += T.ownmap[g][m] == OWN_CLIP || T.ownmap[g][m] >= 0;
        }
    }
    make_in_game(T);
    if (mine > 0) {
        toast(T, "Based on %s, %d tall: made again in every game, its %d own animations kept", HeroNames[h], T.height, mine);
    } else {
        toast(T, "Based on %s, %d tall: made again in every game", HeroNames[h], T.height);
    }
    SDL_strlcpy(T.toasted, T.status, sizeof(T.toasted));
}

// The hero's card: its name, id, height and who it's based on
void hero_card(Tool &T) {
    card("##hero", TINT_MINT);
    heading("Hero");
    float cw = ImGui::GetContentRegionAvail().x - U(90);
    row_label("Name", cw);
    if (field("##name", T.name, sizeof(T.name), cw, ImGuiInputTextFlags_CharsUppercase)) {
        T.unsaved = true;
    }
    ImGui::SetItemTooltip("As the game shows it");
    row_label("Id", cw);
    if (field("##id", T.id, sizeof(T.id), cw, ImGuiInputTextFlags_CharsNoBlank)) {
        T.unsaved = true;
    }
    ImGui::SetItemTooltip("Short name for files and online");
    row_label("Height", cw);
    {
        // as tall as one of the heroes, or its own (the number under it, typed or stepped)
        int as = -1;
        for (int h = 0; h < 8; h++) {
            as = T.height == HeroHeights[h] ? h : as;
        }
        char shown[48];
        if (as >= 0) {
            snprintf(shown, sizeof(shown), "As %s", HeroNames[as]);
        } else {
            snprintf(shown, sizeof(shown), "Its own");
        }
        if (menu_begin("##heightas", shown, cw)) {
            for (int h = 0; h < 8; h++) {
                char item[48];
                snprintf(item, sizeof(item), "%s  %d", HeroNames[h], HeroHeights[h]);
                if (menu_item(item, as == h) && T.height != HeroHeights[h]) {
                    T.height = HeroHeights[h];
                    T.dirty = T.unsaved = true;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SetItemTooltip("As tall as one of the heroes, or a height of its own (the number)");
        row_label("", cw);
        float b = ImGui::GetFrameHeight() + U(6);
        if (button("-##shorter", CARD, INK, b, IC_NONE, CUE_DOWN, b)) {
            T.height = T.height - 10 < 200 ? 200 : T.height - 10;
            T.dirty = T.unsaved = true;
        }
        ImGui::SameLine(0, U(6));
        float w = cw - 2 * b - U(12);
        int typed = T.height;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(CARD));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(14));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((w - ImGui::CalcTextSize("000").x) / 2, (b - ImGui::GetFontSize()) / 2));
        ImGui::SetNextItemWidth(w);
        if (ImGui::InputInt("##height", &typed, 0, 0) && typed >= 200 && typed <= 900 && typed != T.height) {
            T.height = typed;
            T.dirty = T.unsaved = true;
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
        ImGui::SetItemTooltip("Its height standing, in the game's units (200 to 900): type it, or step it");
        ImGui::SameLine(0, U(6));
        if (button("+##taller", CARD, INK, b, IC_NONE, CUE_UP, b)) {
            T.height = T.height + 10 > 900 ? 900 : T.height + 10;
            T.dirty = T.unsaved = true;
        }
    }
    row_label("Based on", cw);
    if (menu_begin("##base", HeroNames[T.template_hero], cw)) {
        for (int h = 0; h < 8; h++) {
            if (menu_item(HeroNames[h], T.template_hero == h) && h != T.template_hero) {
                rebase(T, h);
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SetItemTooltip("Plays as this hero: their animations, speed, ride or prop, and sounds it hasn't got");
    card_end();
}

// The card's picture: as wide against its height as the character select shows a card (its 56 of 64 columns less a 5% inset each
// side, by its 38 rows, a row 1.83 times as tall as a column is wide: extraheroes.c's SELECT_*), so a photo isn't stretched there
const int CARD_W = 290, CARD_H = 400;

// The card's PNG as a texture, read again when it's another or a new photo (nullptr: none, or it isn't one)
SDL_Texture *card_texture(Tool &T, SDL_Renderer *ren) {
    if (T.card[0] == 0) {
        return nullptr;
    }
    if (strcmp(T.card_tex_of, T.card) != 0 || T.card_tex_gen != T.card_gen) {
        if (T.card_tex != nullptr) {
            SDL_DestroyTexture(T.card_tex);
        }
        T.card_tex = nullptr;
        SDL_strlcpy(T.card_tex_of, T.card, sizeof(T.card_tex_of));
        T.card_tex_gen = T.card_gen;
        if (SDL_Surface *s = SDL_LoadPNG(T.card)) {
            T.card_tex = SDL_CreateTextureFromSurface(ren, s);
            T.card_tw = s->w, T.card_th = s->h;
            SDL_DestroySurface(s);
        }
    }
    return T.card_tex;
}

// The card's photo taken: what the camera sees drawn twice the card's size and halved (its edges smooth), on nothing (clear round it,
// as the game lays a card over its own background), saved as a PNG in the tool's folder (photos/ID.png) and made the card; Save puts
// it in the pack (card.png)
bool take_photo(Tool &T, SDL_Renderer *ren) {
    const KitMade *mk = &T.made[0];
    View shot;
    shot.w = CARD_W * 2, shot.h = CARD_H * 2, shot.bg = 0;
    shot.yaw = T.cam.yaw, shot.pitch = T.cam.pitch, shot.zoom = T.cam.zoom, shot.px = T.cam.px, shot.py = T.cam.py;
    draw_made(mk, T.cam_anim, T.cam_frame, shot, ren);
    std::vector<unsigned> out((size_t)CARD_W * CARD_H);
    for (int y = 0; y < CARD_H; y++) {
        for (int x = 0; x < CARD_W; x++) {
            unsigned r = 0, g = 0, b = 0, a = 0;
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 2; i++) {
                    unsigned c = shot.rgba[(size_t)(y * 2 + j) * shot.w + x * 2 + i], ca = c >> 24;
                    r += (c & 255) * ca, g += (c >> 8 & 255) * ca, b += (c >> 16 & 255) * ca, a += ca;
                }
            }
            out[(size_t)y * CARD_W + x] = a == 0 ? 0 : (r / a) | (g / a) << 8 | (b / a) << 16 | (a / 4) << 24;
        }
    }
    view_free(shot);
    char *pref = pref_path();
    char path[1300];
    if (pref == nullptr) {
        snprintf(T.status, sizeof(T.status), "The photo has nowhere to go: %s", SDL_GetError());
        return false;
    }
    snprintf(path, sizeof(path), "%sphotos", pref);
    SDL_CreateDirectory(path);
    snprintf(path, sizeof(path), "%sphotos/%s.png", pref, T.id);
    SDL_free(pref);
    SDL_Surface *s = SDL_CreateSurfaceFrom(CARD_W, CARD_H, SDL_PIXELFORMAT_ABGR8888, out.data(), CARD_W * 4);
    bool ok = s != nullptr && SDL_SavePNG(s, path);
    SDL_DestroySurface(s);
    if (!ok) {
        snprintf(T.status, sizeof(T.status), "The photo wasn't saved: %s", SDL_GetError());
        return false;
    }
    SDL_strlcpy(T.card, path, sizeof(T.card));
    T.card_photo = true;
    T.card_gen++;
    T.unsaved = true;
    snprintf(T.status, sizeof(T.status), "Photo taken: it's the card");
    return true;
}

// The card's camera, in the hero's page's picture (the user's word, 2026-11-22: "take a photo and use that. With full move him around
// rotate etc."): the hero as the game has it on foot, in a frame the card's shape, dragged to turn and tilt it, right-dragged to move
// it, the wheel to zoom, posed in any of its animations at any frame; Take photo makes that its card
// The camera as it starts (and double-clicked): close on its head and chest, a little from its right, as the select's portraits are
void cam_start(View &v) {
    v.yaw = 20, v.pitch = 8, v.zoom = 2.2f, v.px = 0, v.py = 0.34f;
}

void camera_view(Tool &T, SDL_Window *win, ImVec2 p, ImVec2 size) {
    ImDrawList *dl = ImGui::GetWindowDrawList();
    float bh = ImGui::GetFontSize() + U(22);
    ImGui::PushFont(FontTitle);
    dl->AddText(ImVec2(p.x + U(24), p.y + U(16) + (bh - ImGui::GetTextLineHeight()) / 2), INK, "Card photo");
    ImGui::PopFont();
    float right = p.x + size.x - U(16);
    if (!T.have_made[0] || T.made[0].nanims == 0) {
        // not made yet: made now (the camera shows what the game has)
        if (!T.made_asked && T.maker == nullptr) {
            make_in_game(T);
        }
        const char *says = T.maker != nullptr ? "Making it in the games..." : "It isn't made yet";
        ImVec2 ts = ImGui::CalcTextSize(says);
        dl->AddText(ImVec2(p.x + (size.x - ts.x) / 2, p.y + (size.y - ts.y) / 2), MUTED, says);
        ImGui::SetCursorScreenPos(ImVec2(right - button_w("Cancel", false), p.y + U(16)));
        if (button("Cancel##cam", RAISED, INK, 0, IC_NONE, CUE_DROP, bh)) {
            T.camera = false;
        }
        return;
    }
    const KitMade *mk = &T.made[0];
    if (T.cam_anim < 0 || T.cam_anim >= mk->nanims) {
        T.cam_anim = breathe_of(mk);
        T.cam_frame = 0;
        cam_start(T.cam);
    }
    int frames = mk->anims[T.cam_anim].frames;
    T.cam_frame = std::max(0, std::min(frames - 1, T.cam_frame));
    // on the right: the move and its frame it's posed in, Cancel, and Take photo
    float take_w = button_w("Take photo", true), cancel_w = button_w("Cancel", false), menu_w = U(180), frame_w = U(150);
    float x = right - take_w - U(10) - cancel_w - U(10) - (frames > 1 ? frame_w + U(10) : 0) - menu_w;
    ImGui::SetCursorScreenPos(ImVec2(x, p.y + U(16) + (bh - ImGui::GetFrameHeight()) / 2));
    if (menu_begin("##campose", mk->anims[T.cam_anim].name, menu_w, RAISED)) {
        for (int a = 0; a < mk->nanims; a++) {
            if (menu_item(mk->anims[a].name, a == T.cam_anim)) {
                T.cam_anim = a;
                T.cam_frame = 0;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SetItemTooltip("Its pose: one of its animations");
    if (frames > 1) {
        ImGui::SameLine(0, U(10));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(RAISED));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
        ImGui::SetNextItemWidth(frame_w);
        ImGui::SliderInt("##camframe", &T.cam_frame, 0, frames - 1, "frame %d");
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
        ImGui::SetItemTooltip("Its frame in that animation");
    }
    ImGui::SetCursorScreenPos(ImVec2(right - take_w - U(10) - cancel_w, p.y + U(16)));
    if (button("Cancel##cam", RAISED, INK, 0, IC_NONE, CUE_DROP, bh)) {
        T.camera = false;
    }
    ImGui::SameLine(0, U(10));
    if (button("Take photo##cam", CORAL, CORAL_INK, 0, IC_CAMERA, CUE_SAVE, bh) && take_photo(T, SDL_GetRenderer(win))) {
        T.camera = false;
    }
    ImGui::SetItemTooltip("Make what's in the frame its card");
    // the frame: as big as the picture has room for, the card's shape
    float top = p.y + U(16) + bh + U(20), fh = p.y + size.y - U(44) - top, fw = fh * CARD_W / CARD_H;
    if (fw > size.x - U(48)) {
        fw = size.x - U(48);
        fh = fw * CARD_H / CARD_W;
    }
    ImVec2 a(p.x + (size.x - fw) / 2, top), b(a.x + fw, a.y + fh);
    View &v = T.cam;
    if (v.w != (int)fw || v.h != (int)fh) {
        view_free(v);
        v.w = (int)fw, v.h = (int)fh;
    }
    v.bg = 0;
    draw_made(mk, T.cam_anim, T.cam_frame, v, SDL_GetRenderer(win));
    dl->AddRectFilled(a, b, TINT_PERI, U(22));
    ImGui::SetCursorScreenPos(a);
    ImGui::InvisibleButton("##camera", ImVec2(fw, fh), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle);
    ImGuiIO &io = ImGui::GetIO();
    if (ImGui::IsItemActive()) {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            v.yaw -= io.MouseDelta.x * 0.5f;
            v.yaw = v.yaw > 180 ? v.yaw - 360 : v.yaw < -180 ? v.yaw + 360 : v.yaw;
            v.pitch = fmaxf(-80, fminf(80, v.pitch + io.MouseDelta.y * 0.4f));
        }
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) || ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
            v.px += io.MouseDelta.x / fh;
            v.py += io.MouseDelta.y / fh;
        }
    }
    if (ImGui::IsItemHovered()) {
        if (io.MouseWheel != 0) {
            v.zoom = fmaxf(0.5f, fminf(8, v.zoom * powf(1.15f, io.MouseWheel)));
        }
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            cam_start(v);
        }
    }
    if (v.tex != nullptr) {
        ImVec2 in(0.5f / v.w, 0.5f / v.h);
        dl->AddImageRounded((ImTextureID)(intptr_t)v.tex, a, b, in, ImVec2(1 - in.x, 1 - in.y), IM_COL32_WHITE, U(22));
    }
    dl->AddRect(a, b, PERI, U(22), 0, U(3));
    ImGui::PushFont(FontSmall);
    const char *hint = "Drag: turn     Right-drag: move     Wheel: zoom     Double-click: back";
    ImVec2 hs = ImGui::CalcTextSize(hint);
    dl->AddText(ImVec2(p.x + (size.x - hs.x) / 2, b.y + U(12)), DIM, hint);
    ImGui::PopFont();
}

// The card's card: the picture for the character select: a photo of it (Take a photo: the picture above becomes a camera), a PNG, or
// else its face drawn from its model by the game
void face_card(Tool &T, SDL_Window *win, float w) {
    static const SDL_DialogFileFilter Png[] = { { "Picture (PNG)", "png" } };
    card("##face", CARD, w);
    ImVec2 p = ImGui::GetCursorScreenPos();
    float pw = U(108), ph = pw * CARD_H / CARD_W;
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + pw, p.y + ph), TINT_PERI, U(20));
    SDL_Texture *tex = card_texture(T, SDL_GetRenderer(win));
    if (tex != nullptr) {
        // all of it, filling the card's shape (as the game does)
        dl->AddImageRounded((ImTextureID)(intptr_t)tex, ImVec2(p.x + U(4), p.y + U(4)), ImVec2(p.x + pw - U(4), p.y + ph - U(4)), ImVec2(0, 0),
                            ImVec2(1, 1), IM_COL32_WHITE, U(17));
    } else if (T.card[0] == 0 && T.have_made[0] && T.face_view.tex != nullptr) {
        // its face from the model: the top of it from the front, on foot (as much as Face says: 0.36 if it's left to the game)
        float top = T.face_head > 0 ? T.face_head : 0.36f, half = top * 0.5f * pw / ph * 0.94f / 0.94f;
        dl->AddImageRounded((ImTextureID)(intptr_t)T.face_view.tex, ImVec2(p.x + U(4), p.y + U(4)), ImVec2(p.x + pw - U(4), p.y + ph - U(4)),
                            ImVec2(0.5f - half * 0.94f, 0.03f), ImVec2(0.5f + half * 0.94f, 0.03f + top * 0.94f), IM_COL32_WHITE, U(17));
    } else {
        icon(dl, T.card[0] ? IC_CHECK : IC_PERSON, ImVec2(p.x + pw / 2, p.y + ph / 2), U(34), PERI);
    }
    dl->AddRect(p, ImVec2(p.x + pw, p.y + ph), PERI, U(20), 0, U(3));
    ImGui::SetCursorScreenPos(ImVec2(p.x + pw + U(16), p.y));
    ImGui::BeginGroup();
    heading("Card");
    ImGui::TextColored(V4(MUTED), "%s", T.card[0] ? (T.card_photo ? "A photo of it" : short_path(T.card)) : "Its face, from the model");
    if (button("Take a photo##card", T.camera ? PERI : TINT_PERI, T.camera ? PERI_INK : PERI_LIGHT, 0, IC_CAMERA, CUE_OPEN)) {
        T.camera = !T.camera;
    }
    ImGui::SetItemTooltip("Pose it in the picture above and take its card");
    if (button("Browse##card", RAISED, INK, 0, IC_FOLDER, CUE_OPEN)) {
        browse(win, PICK_CARD, Png);
    }
    ImGui::SetItemTooltip("A PNG for the character select");
    if (T.card[0]) {
        ImGui::SameLine(0, U(8));
        if (button("Clear##card", RAISED, INK, 0, IC_NONE, CUE_DROP)) {
            T.card[0] = 0;
            T.card_photo = false;
            T.unsaved = true;
        }
        ImGui::SetItemTooltip("Back to its face, from the model");
    }
    ImGui::EndGroup();
    ImGui::SetCursorScreenPos(ImVec2(p.x, fmaxf(p.y + ph, ImGui::GetCursorScreenPos().y)));
    ImGui::Dummy(ImVec2(0, 0));
    card_end();
}

// The sounds' card: a WAV for each of its sounds (played to hear it), else its base's
void sounds_card(Tool &T, SDL_Window *win, float w) {
    static const SDL_DialogFileFilter Wav[] = { { "Sound (WAV)", "wav" } };
    card("##sounds", CARD, w);
    heading("Sounds");
    ImGui::SameLine(0, U(12));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + U(4));
    ImGui::PushFont(FontSmall);
    ImGui::TextColored(V4(DIM), "a WAV for each; without one, %s's", HeroNames[T.template_hero]);
    ImGui::PopFont();
    int cols = ImGui::GetContentRegionAvail().x > U(480) ? 2 : 1;
    float cw = (ImGui::GetContentRegionAvail().x - U(8) * (cols - 1)) / cols, h = U(40);
    ImVec2 base = ImGui::GetCursorScreenPos();
    char from[64];
    snprintf(from, sizeof(from), "%s's", HeroNames[T.template_hero]);
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        ImGui::PushID(j);
        ImVec2 p(base.x + (j % cols) * (cw + U(8)), base.y + (j / cols) * (h + U(6)));
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(p, ImVec2(p.x + cw, p.y + h), DEEP, U(16));
        ImGui::SetCursorScreenPos(ImVec2(p.x + U(5), p.y + U(5)));
        ImGui::BeginDisabled(T.voice[j][0] == 0);
        if (icon_button("##play", IC_PLAY, T.voice[j][0] ? "Play it" : "Its base's (in the game)", TINT_PERI, PERI, h - U(10), CUE_N)) {
            sound_wav(T.voice[j]);
        }
        ImGui::EndDisabled();
        float lh = ImGui::GetTextLineHeight(), nw = ImGui::CalcTextSize(PackVoiceJobs[j]).x;
        dl->AddText(ImVec2(p.x + h + U(4), p.y + (h - lh) / 2), INK, PackVoiceJobs[j]);
        ImGui::PushFont(FontSmall);
        ImGui::PushClipRect(p, ImVec2(p.x + cw - U(46), p.y + h), true);
        dl->AddText(ImVec2(p.x + h + U(12) + nw, p.y + (h - ImGui::GetTextLineHeight()) / 2), T.voice[j][0] ? MINT_LIGHT : DIM,
                    T.voice[j][0] ? short_path(T.voice[j]) : from);
        ImGui::PopClipRect();
        ImGui::PopFont();
        ImGui::SetCursorScreenPos(ImVec2(p.x + cw - U(38), p.y + U(6)));
        if (T.voice[j][0]) {
            if (icon_button("##clear", IC_EYE_OFF, "Back to its base's", RAISED, INK, h - U(12), CUE_DROP)) {
                T.voice[j][0] = 0;
                T.unsaved = true;
            }
        } else if (icon_button("##browse", IC_FOLDER, "Pick a WAV", RAISED, INK, h - U(12), CUE_OPEN)) {
            browse(win, PICK_VOICE + j, Wav);
        }
        ImGui::PopID();
    }
    ImGui::SetCursorScreenPos(ImVec2(base.x, base.y + ((PACK_VOICE_JOBS + cols - 1) / cols) * (h + U(6)) - U(6)));
    ImGui::Dummy(ImVec2(cols * cw + U(8) * (cols - 1), 0));
    card_end();
}

// The mod's card: who made it, and its folders (it's called as the hero is)
void mod_card(Tool &T) {
    card("##mod", CARD);
    heading("Mod");
    float cw = ImGui::GetContentRegionAvail().x - U(90);
    row_label("Author", cw);
    if (field("##author", T.author, sizeof(T.author), cw, 0, DEEP)) {
        T.unsaved = true;
    }
    ImGui::PushStyleColor(ImGuiCol_Text, V4(MUTED));
    if (ImGui::TreeNode("Folders")) {
        ImGui::PopStyleColor();
        char home[1100];
        heroes_home(home, sizeof(home));
        ImGui::TextColored(V4(MUTED), "Its own (Save)");
        ImGui::PushTextWrapPos(0);
        ImGui::TextUnformatted(home);
        ImGui::PopTextWrapPos();
        ImGui::TextColored(V4(MUTED), "The game's mods folder (Add to game)");
        path_field("##out", T.out, sizeof(T.out));
        ImGui::TextColored(V4(MUTED), "The game");
        path_field("##game", T.game, sizeof(T.game));
        ImGui::TreePop();
    } else {
        ImGui::PopStyleColor();
    }
    card_end();
}

// A space between cards
void card_gap() {
    ImGui::Dummy(ImVec2(0, U(4)));
}

// Under the picture without its timeline (a game it can't be posed in): the animation shown, and how fast
void player_bar(Tool &T, const KitMade *mk) {
    card("##player", CARD);
    if (menu_begin("##watching", T.anim < mk->nanims ? mk->anims[T.anim].name : "", U(200))) {
        for (int a = 0; a < mk->nanims; a++) {
            if (menu_item(mk->anims[a].name, a == T.anim)) {
                T.anim = a;
                T.made_t = 0;
                T.frame = 0;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine(0, U(10));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(DEEP));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
    ImGui::SetNextItemWidth(U(140));
    ImGui::SliderFloat("##speed", &T.made_speed, 0, 2, "%.2fx");
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::SetItemTooltip("Speed");
    card_end();
}

// A game's page: its picture (the bones and tools over it) and timeline, and its cards on the right
void game_page(Tool &T) {
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float right_w = U(356), gap = U(16), main_w = avail.x - right_w - gap;
    ImGui::BeginChild("##main", ImVec2(main_w, avail.y), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    if (!T.have_made[T.kind]) {
        card("##notmade", CARD);
        heading(T.maker != nullptr ? "Making it in the games..." : "Not made yet");
        if (T.maker == nullptr && button("Make it", CORAL, CORAL_INK, 0, IC_REFRESH, CUE_TAP)) {
            make_in_game(T);
        }
        card_end();
    } else {
        const KitMade *mk = &T.made[T.kind];
        int a = T.anim < mk->nanims ? T.anim : 0;
        bool posable = T.have_pose[T.kind] && mk->nanims > 0;
        T.adjusting = posable;
        // the timeline's height: its header, ruler and a row a bone (a third of the page at most; it scrolls past that)
        int rows = posable ? std::max(1, timeline_rows(T, mk->anims[a].name)) : 0;
        float tl_h = posable ? U(32) + U(50) + U(26) + (T.clipping ? U(14) : 0) + rows * U(36) : U(80);
        tl_h = fminf(tl_h, avail.y * 0.38f);
        ImVec2 p = ImGui::GetCursorScreenPos(), size(main_w, avail.y - tl_h - gap);
        View &v = T.game_view;
        if (v.w != (int)size.x || v.h != (int)size.y) {
            view_free(v);
            v.w = (int)size.x, v.h = (int)size.y;
        }
        ImDrawList *dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), PICTURE, U(30));
        ImGui::SetNextItemAllowOverlap();
        ImGui::InvisibleButton("##picture", size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        bool hovered = ImGui::IsItemHovered();
        if (v.tex != nullptr) {
            ImVec2 in(0.5f / v.w, 0.5f / v.h);
            dl->AddImageRounded((ImTextureID)(intptr_t)v.tex, p, ImVec2(p.x + size.x, p.y + size.y), in, ImVec2(1 - in.x, 1 - in.y), IM_COL32_WHITE, U(30));
        }
        // the bones and gizmo from the view the picture was drawn with this frame, then the axes, then the mouse turning or tilting it
        // (for the next: worked out after a tilt, they were a frame ahead of the picture, and slid off it as it tilted)
        ImVec2 axes(p.x + size.x - U(70), p.y + size.y - U(70)), m = ImGui::GetIO().MousePos;
        bool on_axes = hovered && (m.x - axes.x) * (m.x - axes.x) + (m.y - axes.y) * (m.y - axes.y) < U(53) * U(53);
        if (posable) {
            adjust_view(T, p, 1, hovered && !on_axes && T.bones);
        }
        view_axes(v, T.made_turn, axes, hovered);
        look_around(v, T.made_turn, false);
        float under = p.y + U(16);
        if (posable) {
            under = picture_overlays(T, p, size);
        }
        draw_toast(T, ImVec2(p.x + size.x - U(16), under), U(420));
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + size.y + gap));
        if (posable) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, V4(CARD));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(16), U(14)));
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, U(26));
            ImGui::BeginChild("##timeline", ImVec2(main_w, tl_h), ImGuiChildFlags_AlwaysUseWindowPadding);
            timeline(T, mk, a, ImGui::GetContentRegionAvail().x);
            ImGui::EndChild();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor();
        } else {
            player_bar(T, mk);
        }
    }
    ImGui::EndChild();
    ImGui::SameLine(0, gap);
    ImGui::BeginChild("##cards", ImVec2(right_w, avail.y));
    game_card(T);
    card_gap();
    fade_card(T);
    card_gap();
    bone_card(T);
    card_gap();
    history_card(T);
    ImGui::EndChild();
}

// The hero's own page: its model (Joints: its dots placed on its front and side; Preview: the tool's animations on it), its card and
// sounds under it, and its model, details and mod on the right
// The model's views at w by h (made again at that size, and the model fitted to them, when it changes)
void model_views_fit(Tool &T, int w, int h, int pw, int ph) {
    View *vs[3] = { &T.front, &T.side, &T.play };
    const int ws[3] = { w, w, pw }, hs[3] = { h, h, ph };
    for (int i = 0; i < 3; i++) {
        if (vs[i]->w == ws[i] && vs[i]->h == hs[i]) {
            continue;
        }
        view_free(*vs[i]);
        vs[i]->w = ws[i], vs[i]->h = hs[i];
        if (T.m != nullptr) {
            fit(T.m, *vs[i]);
        }
    }
}

// Guess joints, asked first: the dots it has go (Undo brings them back)
void guess_ask(Tool &T) {
    if (T.ask_guess) {
        ImGui::OpenPopup("##guessjoints");
        T.ask_guess = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(26), U(22)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, U(24));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, V4(CARD));
    bool asking = ImGui::BeginPopupModal("##guessjoints", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    if (!asking) {
        return;
    }
    ImGui::PushFont(FontTitle);
    ImGui::TextUnformatted("Guess its joints?");
    ImGui::PopFont();
    ImGui::Spacing();
    ImGui::TextColored(V4(MUTED), "The dots you've placed are replaced.");
    ImGui::Spacing();
    if (button("Keep mine##guess", RAISED, INK, U(140), IC_NONE, CUE_DROP) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine(0, U(10));
    if (button("Guess##guess", PERI, PERI_INK, U(140), IC_SPARK, CUE_GUESS)) {
        joint_snapshot(T, "Guessed its joints");
        kit_guess_markers(T.m);
        T.rigged = false;
        reweigh(T);
        T.dirty = T.unsaved = true;
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}

void model_page(Tool &T, SDL_Window *win) {
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float right_w = U(356), gap = U(16), main_w = avail.x - right_w - gap;
    ImGui::BeginChild("##main", ImVec2(main_w, avail.y), 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 p = ImGui::GetCursorScreenPos(), size(main_w, avail.y);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), PICTURE, U(30));
    // the front and the side (their joints to drag) big, and the preview (the tool's own moves on it) small at the side under Mirror and
    // Grid, its move and Play under it; all at once (the user's word, 2026-11-23: "the front and side should be much bigger, the preview
    // should not be in the main focus but of to the side and smaller"); the dots' legend in the picture's bottom left corner
    float top = U(14), foot = ImGui::GetFrameHeight() + U(24), vg = U(14), bar = ImGui::GetFrameHeight();
    float side_w = fminf(U(250), size.x * 0.22f), pw = side_w, ph = fminf(pw / 0.72f, size.y - top - bar - U(12) - bar - U(40));
    float left_w = size.x - side_w - U(16) * 3;
    {
        int h = (int)fminf(U(1200), size.y - top - foot), w = (int)fminf(h * 0.72f, (left_w - vg) / 2);
        model_views_fit(T, std::max(w, 60), std::max(h, 80), std::max((int)pw, 60), std::max((int)ph, 80));
    }
    float side_x = p.x + size.x - U(16) - side_w;
    if (T.camera) {
        camera_view(T, win, p, size);
    } else {
        float x0 = p.x + U(16) + (left_w - 2 * vw(T.front) - vg) / 2, y0 = p.y + top;
        const char *names[2] = { "Front", "Side" };
        View *views[2] = { &T.front, &T.side };
        for (int i = 0; i < 2; i++) {
            ImGui::SetCursorScreenPos(ImVec2(x0 + i * (vw(T.front) + vg), y0));
            view_window(T, *views[i], names[i], true, i);
            ImGui::SetCursorScreenPos(ImVec2(x0 + i * (vw(T.front) + vg) + U(12), y0 + U(12)));
            tag(names[i], RAISED, MUTED);
        }
        // Mirror and Grid, top right, as a game's page has its own there (the user's word, 2026-11-23: "the mirror button needs to be in
        // the joint window like the animation window does")
        float tw = button_w("Mirror", true) + U(8) + button_w("Grid", true);
        ImGui::SetCursorScreenPos(ImVec2(p.x + size.x - U(16) - tw, p.y + U(16)));
        toggle("Mirror##joints", &T.mirror, IC_MIRROR, "Moving one side's dot moves the other's, mirrored across its hips");
        ImGui::SameLine(0, U(8));
        toggle("Grid##joints", &T.joint_grid, IC_GRID, "A grid behind the joints: a line each step through its middle and up from its feet");
        // the preview, small, at the side
        float py = p.y + U(16) + bar + U(12);
        ImGui::SetCursorScreenPos(ImVec2(side_x, py));
        view_window(T, T.play, "move", false, 2);
        T.play.zx = vw(T.play) / 2.0f;
        T.play.zy = vh(T.play) / 2.0f;
        look_around(T.play, T.turn);
        ImGui::SetCursorScreenPos(ImVec2(side_x + U(10), py + U(10)));
        tag("Preview", RAISED, MUTED);
        float play_w = button_w("Play", true);
        ImGui::SetCursorScreenPos(ImVec2(side_x, py + vh(T.play) + U(10)));
        if (menu_begin("##preview", KitMoves[T.move].name, side_w - play_w - U(8), RAISED)) {
            for (int i = 0; i < KitMoveCount; i++) {
                if (menu_item(KitMoves[i].name, T.move == i)) {
                    T.move = i;
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SetItemTooltip("The tool's built-in animations on your model (on foot, its own uses them for any it hasn't got)");
        ImGui::SameLine(0, U(8));
        toggle("Play##model", &T.playing, IC_PLAY, nullptr);
        // the legend: small, in the corner
        static const char *const Parts[4] = { "body", "arms", "legs", "jaw, ears, tail" };
        const ImU32 Cols[4] = { IM_COL32(0xF2, 0xD5, 0x8A, 255), PERI, MINT, CORAL };
        ImGui::PushFont(FontSmall);
        float lh = ImGui::GetTextLineHeight(), lx = p.x + U(20), ly = p.y + size.y - U(12) - (ImGui::GetFrameHeight() + lh) / 2 - U(2);
        for (int i = 0; i < 4; i++) {
            dl->AddCircleFilled(ImVec2(lx + U(4), ly + lh / 2), U(3.5f), Cols[i]);
            dl->AddText(ImVec2(lx + U(12), ly), MUTED, Parts[i]);
            lx += U(12) + ImGui::CalcTextSize(Parts[i]).x + U(14);
        }
        ImGui::PopFont();
    }
    draw_toast(T, ImVec2(side_x - U(16), p.y + U(16)), U(420));
    guess_ask(T);
    ImGui::EndChild();
    ImGui::SameLine(0, gap);
    ImGui::BeginChild("##cards", ImVec2(right_w, avail.y));
    model_card(T, win);
    card_gap();
    hero_card(T);
    card_gap();
    face_card(T, win, 0);
    card_gap();
    sounds_card(T, win, 0);
    card_gap();
    mod_card(T);
    ImGui::EndChild();
}

// Every game at a glance: a tile each, the hero in it on the floor grid (turned as the pictures are), what's its own there, and
// whether it's squeezed in to sit; click one to edit it
void all_games_page(Tool &T) {
    heading("All games");
    ImGui::SameLine(0, U(14));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + U(3));
    ImGui::TextColored(V4(MUTED), "%s in every one. Click one to edit it.", T.name);
    ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x - U(200));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(CARD));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
    ImGui::SetNextItemWidth(U(200));
    ImGui::SliderFloat("##turnall", &T.made_turn, -180, 180, "turned %.0f deg");
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::BeginChild("##tiles", ImVec2(0, 0), 0, 0);
    float gap = U(16), tw = (float)ThumbW, th = (float)ThumbH, foot = U(76);
    int cols = std::max(1, (int)((ImGui::GetContentRegionAvail().x + gap) / (tw + gap)));
    ImVec2 origin = ImGui::GetCursorScreenPos();
    origin.y += U(8);
    for (int k = 0; k < PACK_KINDS; k++) {
        ImGui::PushID(k);
        ImVec2 p(origin.x + (k % cols) * (tw + gap), origin.y + (k / cols) * (th + foot + gap));
        ImGui::SetCursorScreenPos(p);
        bool hit = ImGui::InvisibleButton("##tile", ImVec2(tw, th + foot));
        bool hov = ImGui::IsItemHovered();
        float lift = ease_to(ImGui::GetItemID(), hov ? 1 : 0, 18);
        ImDrawList *dl = ImGui::GetWindowDrawList();
        ImVec2 a(p.x, p.y - lift * U(5)), b(p.x + tw, p.y + th + foot - lift * U(5));
        if (lift > 0) {
            dl->AddRectFilled(ImVec2(a.x - U(3), a.y - U(3)), ImVec2(b.x + U(3), b.y + U(3)), (PERI & 0x00FFFFFFu) | (ImU32)(lift * 120) << 24, U(28));
        }
        dl->AddRectFilled(ImVec2(a.x, a.y + U(3)), ImVec2(b.x, b.y + U(3)), SHADOW, U(26));
        dl->AddRectFilled(a, ImVec2(b.x, a.y + th), PICTURE, U(26), ImDrawFlags_RoundCornersTop);
        if (T.have_made[k] && T.thumbs[k].tex != nullptr) {
            ImVec2 in(0.5f / tw, 0.5f / th);
            dl->AddImageRounded((ImTextureID)(intptr_t)T.thumbs[k].tex, a, ImVec2(b.x, a.y + th), in, ImVec2(1 - in.x, 1 - in.y), IM_COL32_WHITE,
                                U(26), ImDrawFlags_RoundCornersTop);
        } else {
            icon(dl, IC_CUBE, ImVec2((a.x + b.x) / 2, a.y + th / 2), U(34), DIM);
        }
        dl->AddRectFilled(ImVec2(a.x, a.y + th), b, CARD, U(26), ImDrawFlags_RoundCornersBottom);
        ImGui::PushFont(FontTitle);
        dl->AddText(ImVec2(a.x + U(14), a.y + th + U(10)), INK, KindChip[k]);
        ImGui::PopFont();
        // what's its own there
        int g = game_of(k), mine = 0, moves = 0;
        for (int m = 0; g >= 0 && m < 8 && KitGames[g].moves[m] != nullptr; m++) {
            moves++;
            mine += T.ownmap[g][m] != OWN_TEMPLATE;
        }
        char says[64];
        int from = T.kind_template[k] >= 0 ? T.kind_template[k] : T.template_hero;
        if (k == KIND_WARP_TOOL) {
            snprintf(says, sizeof(says), "from On foot");
        } else if (k == 0) {
            snprintf(says, sizeof(says), T.foot_own ? "its own" : "%s's", HeroNames[from]);
        } else if (mine > 0) {
            snprintf(says, sizeof(says), "%d of %d its own", mine, moves);
        } else {
            snprintf(says, sizeof(says), "%s's", HeroNames[from]);
        }
        ImGui::SetCursorScreenPos(ImVec2(a.x + U(14), a.y + th + U(42)));
        bool own = mine > 0 || (k == 0 && T.foot_own);
        tag(says, own ? TINT_MINT : RAISED, own ? MINT_LIGHT : MUTED, 0, U(24));
        if (T.have_pose[k] && T.poses[k].cut && !T.no_fit[k]) {
            ImGui::SameLine(0, U(6));
            tag("Fit in seat", TINT_PERI, PERI_LIGHT, 0, U(24));
        }
        if (hit) {
            go_page(T, k);
        }
        ImGui::PopID();
    }
    ImGui::SetCursorScreenPos(ImVec2(origin.x, origin.y + ((PACK_KINDS + cols - 1) / cols) * (th + foot + gap)));
    ImGui::Dummy(ImVec2(0, 0));
    ImGui::EndChild();
}

// A folder and all in it gone (SDL_RemovePath takes a file or an empty folder); false if any of it stays
SDL_EnumerationResult SDLCALL remove_in(void *ok, const char *dir, const char *name);
bool remove_tree(const char *path) {
    SDL_PathInfo info;
    bool ok = true;

    if (!SDL_GetPathInfo(path, &info)) {
        return true;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        SDL_EnumerateDirectory(path, remove_in, &ok);
    }
    return SDL_RemovePath(path) && ok;
}

SDL_EnumerationResult SDLCALL remove_in(void *ok, const char *dir, const char *name) {
    char path[1400];
    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!remove_tree(path)) {
        *(bool *)ok = false;
    }
    return SDL_ENUM_CONTINUE;
}

// Whether a folder has nothing in it but what's named (or nothing at all)
struct OnlyCtx {
    const char *keep;
    bool only;
};
SDL_EnumerationResult SDLCALL only_in(void *c, const char *, const char *name) {
    OnlyCtx *o = (OnlyCtx *)c;
    if (o->keep == nullptr || SDL_strcasecmp(name, o->keep) != 0) {
        o->only = false;
    }
    return SDL_ENUM_CONTINUE;
}
bool folder_only(const char *path, const char *keep) {
    OnlyCtx o = { keep, true };
    SDL_EnumerateDirectory(path, only_in, &o);
    return o.only;
}

// What's in a folder, by name
SDL_EnumerationResult SDLCALL names_in(void *v, const char *, const char *name) {
    ((std::vector<std::string> *)v)->push_back(name);
    return SDL_ENUM_CONTINUE;
}

// Whether a hero's delete takes its whole mod: nothing in its heroes folder but it, nor in its mod but that and its mod.json (as
// delete_hero does it)
bool mod_goes(const char *mod_path, const char *id) {
    std::vector<std::string> heroes, mod;
    char path[1400];
    snprintf(path, sizeof(path), "%s/heroes", mod_path);
    SDL_EnumerateDirectory(path, names_in, &heroes);
    SDL_EnumerateDirectory(mod_path, names_in, &mod);
    for (auto &n : heroes) {
        if (SDL_strcmp(n.c_str(), id) != 0) {
            return false;
        }
    }
    for (auto &n : mod) {
        if (SDL_strcasecmp(n.c_str(), "heroes") != 0 && SDL_strcasecmp(n.c_str(), "mod.json") != 0) {
            return false;
        }
    }
    return true;
}

// A hero deleted, fully: its folder in its mod (heroes/ID), the mod too if nothing else is in it but its mod.json (another hero, or
// anything else of the mod's, stays), and what the tool kept of it (its preview's models, the folder its preview was made from, its card's
// photo). Said in the status; false if any of its folder stays
bool delete_hero(Tool &T, const Found &f) {
    char path[1400];
    bool ok = true;
    // (each copy: the tool's and the game's)
    for (const char *mod : { f.tool_mod, f.game_mod }) {
        if (mod[0] == 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/heroes/%s", mod, f.id);
        bool gone = remove_tree(path);
        ok = ok && gone;
        snprintf(path, sizeof(path), "%s/heroes", mod);
        if (gone && folder_only(path, nullptr)) {
            SDL_RemovePath(path);
            if (folder_only(mod, "mod.json")) {
                remove_tree(mod);
            }
        }
    }
    if (char *pref = pref_path()) {
        for (int k = 0; k < PACK_KINDS; k++) {
            for (const char *end : { ".hpv", ".pose", "_ride0.hpv", "_ride1.hpv" }) {
                snprintf(path, sizeof(path), "%spreview-made/%s_%s%s", pref, f.id, pack_kind_name(k), end);
                SDL_RemovePath(path);
            }
        }
        snprintf(path, sizeof(path), "%spreviews/%s", pref, f.id);
        remove_tree(path);
        snprintf(path, sizeof(path), "%sphotos/%s.png", pref, f.id);
        SDL_RemovePath(path);
        SDL_free(pref);
    }
    if (SDL_strcmp(T.loaded_mod, f.tool_mod) == 0 || SDL_strcmp(T.loaded_mod, f.game_mod) == 0) {
        T.loaded_mod[0] = 0;
        T.game_mod[0] = 0;
    }
    snprintf(T.status, sizeof(T.status), ok ? "Deleted %s" : "%s wasn't all deleted: is a file of it open?", f.name);
    return ok;
}

// A hero's picture on the start screen: its card (the top of it, square), else its face as the tool last made it (its model on foot
// from the front, the top of it as much as its face share says, as the Card shows it), else none (its first letter)
void found_picture(Found &f, SDL_Renderer *ren) {
    char path[1400];

    f.looked = true;
    snprintf(path, sizeof(path), "%s/heroes/%s/card.png", f.mod, f.id);
    if (SDL_Surface *s = SDL_LoadPNG(path)) {
        f.pic = SDL_CreateTextureFromSurface(ren, s);
        f.uv0 = ImVec2(0, 0), f.uv1 = ImVec2(1, s->h > 0 ? fminf(1, (float)s->w / s->h) : 1);
        SDL_DestroySurface(s);
        return;
    }
    char *pref = pref_path();
    if (pref == nullptr) {
        return;
    }
    snprintf(path, sizeof(path), "%spreview-made/%s_%s.hpv", pref, f.id, pack_kind_name(0));
    SDL_free(pref);
    KitMade mk = {};
    if (!kit_made_read(path, &mk)) {
        return;
    }
    if (mk.nanims > 0) {
        View v;
        v.w = v.h = (int)U(200), v.bg = 0;
        draw_made(&mk, breathe_of(&mk), 0, v, ren);
        f.pic = v.tex;
        v.tex = nullptr;
        view_free(v);
        float top = f.face_head > 0 ? f.face_head : 0.36f, half = top * 0.5f * 0.94f;
        f.uv0 = ImVec2(0.5f - half, 0.03f), f.uv1 = ImVec2(0.5f + half, 0.03f + top * 0.94f);
    }
    kit_made_free(&mk);
}

void start_screen(Tool &T, SDL_Window *win) {
    static const SDL_DialogFileFilter Glb[] = { { "3D model (glTF binary)", "glb" } };
    static bool looked = false;

    if (!looked) {
        find_heroes(T);
        looked = true;
    }
    if (T.status[0] != 0 && strcmp(T.status, T.toasted) != 0) {
        SDL_strlcpy(T.toasted, T.status, sizeof(T.toasted));
        toast(T, "%s", T.status);
    }
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Start", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    float w = fminf(U(1080), io.DisplaySize.x - U(40)), x = (io.DisplaySize.x - w) / 2, gap = U(20);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    // the tool's name, its mark before it; sounds on or off on the right
    ImGui::SetCursorPos(ImVec2(x, U(40)));
    {
        ImVec2 p = ImGui::GetCursorScreenPos();
        float s = U(64);
        dl->AddRectFilled(ImVec2(p.x, p.y + U(4)), ImVec2(p.x + s, p.y + s + U(4)), PERI_DEEP, U(22));
        dl->AddRectFilled(p, ImVec2(p.x + s, p.y + s), PERI, U(22));
        icon(dl, IC_BONES, ImVec2(p.x + s / 2, p.y + s / 2), U(34), PERI_INK);
        ImGui::SetCursorScreenPos(ImVec2(p.x + s + U(16), p.y + U(2)));
        ImGui::BeginGroup();
        ImGui::PushFont(FontBig);
        ImGui::TextUnformatted("Hero tool");
        ImGui::PopFont();
        ImGui::TextColored(V4(MUTED), "Custom heroes for Crash Bash PC");
        ImGui::EndGroup();
        ImGui::SetCursorScreenPos(ImVec2(x + w - ImGui::GetFrameHeight() - U(6), p.y + U(10)));
        bool muted = sound_muted();
        if (icon_button("##sound", muted ? IC_MUTE : IC_SOUND, muted ? "Sounds off" : "Sounds on", CARD)) {
            sound_mute(!muted);
            sound_cue(CUE_ON);
            sound_keep();
        }
        ImGui::SetCursorScreenPos(ImVec2(x, p.y + s + U(28)));
    }
    float cw = (w - gap) / 2, ch = U(460), top = ImGui::GetCursorScreenPos().y;
    // a new hero: a model dropped on the window or browsed for, and what it has to be
    ImGui::SetCursorScreenPos(ImVec2(x, top));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, V4(CARD));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(24), U(22)));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, U(30));
    ImGui::BeginChild("##create", ImVec2(cw, ch), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);
    heading("Create a new hero");
    {
        ImDrawList *cl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float dw = ImGui::GetContentRegionAvail().x, dh = U(204);
        if (ImGui::InvisibleButton("##drop", ImVec2(dw, dh))) {
            sound_cue(CUE_OPEN);
            SDL_ShowOpenFileDialog(picked, (void *)(intptr_t)PICK_MODEL, win, Glb, 1, nullptr, false);
        }
        ImGui::SetItemTooltip("Drop a .glb on the window, or click to browse for one");
        float t = ease_to(ImGui::GetItemID(), ImGui::IsItemHovered() ? 1 : 0, 14);
        cl->AddRectFilled(p, ImVec2(p.x + dw, p.y + dh), mix(DEEP, IM_COL32(0x20, 0x22, 0x30, 255), t), U(24));
        cl->AddRect(p, ImVec2(p.x + dw, p.y + dh), mix(IM_COL32(0x46, 0x45, 0x4B, 255), PERI, t), U(24), 0, U(2));
        ImVec2 c(p.x + dw / 2, p.y + U(52));
        cl->AddRectFilled(ImVec2(c.x - U(30), c.y - U(30)), ImVec2(c.x + U(30), c.y + U(30)), TINT_PERI, U(21));
        icon(cl, IC_CUBE, c, U(30), PERI);
        const char *l1 = "Drop a model here", *l2 = "a .glb model, rigged or not", *l3 = "Browse...";
        ImGui::PushFont(FontTitle);
        ImVec2 s1 = ImGui::CalcTextSize(l1);
        cl->AddText(ImVec2(c.x - s1.x / 2, p.y + U(94)), INK, l1);
        ImGui::PopFont();
        ImVec2 s2 = ImGui::CalcTextSize(l2), s3 = ImGui::CalcTextSize(l3);
        cl->AddText(ImVec2(c.x - s2.x / 2, p.y + U(124)), MUTED, l2);
        float bw = s3.x + U(36), by = p.y + U(152);
        cl->AddRect(ImVec2(c.x - bw / 2, by), ImVec2(c.x + bw / 2, by + U(38)), PERI, U(19), 0, U(2));
        cl->AddText(ImVec2(c.x - s3.x / 2, by + (U(38) - s3.y) / 2), PERI_LIGHT, l3);
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + dh + U(16)));
    }
    {
        // what the model has to be (the game's limits, kit.h's and heropack.c's; the user's word, 2026-11-22: "You also need to
        // specifiy any restrictions on the .glb")
        static const char *const Needs[] = {
            "A .glb file (glTF 2.0), its textures PNGs inside it, 8 at most",
            "1365 triangles and 4096 vertices at most",
            "Standing, facing forward (+Z, Blender's front), any size",
            "Rigged with Mixamo, VRM, Blender or Unity bone names, or not rigged: you place its joints",
            "Its textures share 256 x 256 texels in the game",
        };
        ImDrawList *cl = ImGui::GetWindowDrawList();
        ImGui::PushFont(FontSmall);
        ImGui::TextColored(V4(INK), "What the model needs");
        ImGui::Dummy(ImVec2(0, U(2)));
        float lh = ImGui::GetTextLineHeight(), tw = ImGui::GetContentRegionAvail().x - U(18);
        for (const char *n : Needs) {
            ImVec2 q = ImGui::GetCursorScreenPos();
            cl->AddCircleFilled(ImVec2(q.x + U(6), q.y + lh / 2), U(2.5f), PERI);
            ImGui::SetCursorScreenPos(ImVec2(q.x + U(18), q.y));
            ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + tw);
            ImGui::TextColored(V4(MUTED), "%s", n);
            ImGui::PopTextWrapPos();
        }
        ImGui::PopFont();
    }
    ImGui::EndChild();
    // the heroes made: open one to edit it
    ImGui::SetCursorScreenPos(ImVec2(x + cw + gap, top));
    ImGui::BeginChild("##edit", ImVec2(cw, ch), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);
    heading("Edit a hero");
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, U(4)));
    ImGui::BeginChild("##found", ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding);
    for (int i = 0; i < NumFound; i++) {
        const char *mod = Founds[i].mod;
        for (const char *c = Founds[i].mod; *c != 0; c++) {
            mod = *c == '/' || *c == '\\' ? c + 1 : mod;
        }
        ImGui::PushID(i);
        ImDrawList *fl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        float rw = ImGui::GetContentRegionAvail().x, rh = U(80);
        ImGui::SetNextItemAllowOverlap();
        bool hit = ImGui::InvisibleButton("##hero", ImVec2(rw, rh));
        float t = ease_to(ImGui::GetItemID(), ImGui::IsItemHovered() ? 1 : 0, 16);
        ImVec2 a(p.x + t * U(4), p.y), b(p.x + rw, p.y + rh);
        fl->AddRectFilled(a, b, mix(DEEP, RAISED, t), U(24));
        ImVec2 c(a.x + U(42), a.y + rh / 2);
        fl->AddRectFilled(ImVec2(c.x - U(30), c.y - U(30)), ImVec2(c.x + U(30), c.y + U(30)), ChipTint[i % 4], U(20));
        // its picture (the user's word, 2026-11-22: "Bobby has a hero picture. Might as well use that instead of the B"), else its
        // first letter
        if (!Founds[i].looked) {
            found_picture(Founds[i], SDL_GetRenderer(win));
        }
        if (Founds[i].pic != nullptr) {
            fl->AddImageRounded((ImTextureID)(intptr_t)Founds[i].pic, ImVec2(c.x - U(30), c.y - U(30)), ImVec2(c.x + U(30), c.y + U(30)),
                                Founds[i].uv0, Founds[i].uv1, IM_COL32_WHITE, U(20));
        } else {
            char first[8] = { Founds[i].name[0] != 0 ? Founds[i].name[0] : '?', 0 };
            ImGui::PushFont(FontBig);
            ImVec2 fs = ImGui::CalcTextSize(first);
            fl->AddText(ImVec2(c.x - fs.x / 2, c.y - fs.y / 2), INK, first);
            ImGui::PopFont();
        }
        ImGui::PushFont(FontTitle);
        fl->AddText(ImVec2(a.x + U(86), a.y + U(16)), INK, Founds[i].name);
        ImGui::PopFont();
        {
            // in the game, or the tool's only
            bool in_game = Founds[i].game_mod[0] != 0;
            ImGui::PushFont(FontSmall);
            const char *says = in_game ? "In the game" : "Not in the game";
            fl->AddCircleFilled(ImVec2(a.x + U(90), a.y + U(55)), U(3.5f), in_game ? MINT : DIM);
            fl->AddText(ImVec2(a.x + U(100), a.y + U(55) - ImGui::GetTextLineHeight() / 2), in_game ? MINT_LIGHT : DIM, says);
            ImGui::PopFont();
            (void)mod;
        }
        icon(fl, IC_RIGHT, ImVec2(b.x - U(24), a.y + rh / 2), U(20), MUTED);
        // its trash: deleted, asked first
        {
            ImVec2 keep = ImGui::GetCursorScreenPos();
            float s = U(40);
            ImVec2 tp(b.x - U(52) - s, a.y + (rh - s) / 2);
            ImGui::SetCursorScreenPos(tp);
            bool on = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(tp, ImVec2(tp.x + s, tp.y + s));
            float red = ease_to(ImGui::GetID("##trashred"), on ? 1 : 0, 18);
            if (icon_button("##trash", IC_TRASH, "Delete this hero", mix(RAISED, RED, red), mix(INK, RED_INK, red), s, CUE_OPEN)) {
                Deleting = i;
                DeleteAsk = true;
                hit = false;
            }
            ImGui::SetCursorScreenPos(keep);
        }
        if (hit) {
            sound_cue(CUE_OPEN);
            load_pack(T, Founds[i]);
        }
        ImGui::Dummy(ImVec2(0, U(2)));
        ImGui::PopID();
    }
    if (NumFound == 0) {
        ImGui::TextColored(V4(MUTED), "None yet: make one on the left");
    }
    // a hero's delete, asked first: it can't be undone (opened here, out of the rows' own ids)
    if (DeleteAsk) {
        ImGui::OpenPopup("##deletehero");
        DeleteAsk = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(26), U(22)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, U(24));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, V4(CARD));
    bool asking = ImGui::BeginPopupModal("##deletehero", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    if (asking) {
        if (Deleting >= 0 && Deleting < NumFound) {
            const Found &f = Founds[Deleting];
            ImGui::PushFont(FontTitle);
            ImGui::Text("Delete %s?", f.name);
            ImGui::PopFont();
            ImGui::Spacing();
            ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + U(560));
            // (the full path going: its mod's folder if this hero's all there is in it, else its own; no more words, the user's word,
            // 2026-11-23: "just put the full path its going to delete remove the words keep it simple")
            // (each copy going, the tool's and the game's)
            for (const char *mod : { f.tool_mod, f.game_mod }) {
                if (mod[0] == 0) {
                    continue;
                }
                char going[1400];
                if (mod_goes(mod, f.id)) {
                    snprintf(going, sizeof(going), "%s", mod);
                } else {
                    snprintf(going, sizeof(going), "%s/heroes/%s", mod, f.id);
                }
#ifdef _WIN32
                for (char *c = going; *c != 0; c++) {
                    *c = *c == '/' ? '\\' : *c; // (one way round on Windows)
                }
#endif
                ImGui::TextColored(V4(MUTED), "%s", going);
            }
            ImGui::PopTextWrapPos();
            ImGui::Spacing();
            if (button("Keep it##delete", RAISED, INK, U(130), IC_NONE, CUE_DROP) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                Deleting = -1;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine(0, U(10));
            if (button("Delete##delete", RED, RED_INK, U(130), IC_TRASH, CUE_HIDE)) {
                delete_hero(T, f);
                Deleting = -1;
                find_heroes(T);
                ImGui::CloseCurrentPopup();
            }
        } else {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
    // the mods folder, out of the way at the bottom
    {
        float bh = U(58), y = fmaxf(top + ch + U(24), io.DisplaySize.y - bh - U(28));
        ImVec2 p(x, y);
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + bh), CARD, U(22));
        icon(dl, IC_FOLDER, ImVec2(p.x + U(28), p.y + bh / 2), U(20), MUTED);
        dl->AddText(ImVec2(p.x + U(48), p.y + (bh - ImGui::GetTextLineHeight()) / 2), MUTED, "The game's mods folder");
        float lw = U(48) + ImGui::CalcTextSize("The game's mods folder").x + U(14);
        ImGui::SetCursorScreenPos(ImVec2(p.x + lw, p.y + (bh - ImGui::GetFontSize() - U(16)) / 2));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, V4(DEEP));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, U(16));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(U(14), U(8)));
        ImGui::SetNextItemWidth(w - lw - U(80));
        ImGui::InputText("##mods", T.out, sizeof(T.out));
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
        ImGui::SameLine(0, U(8));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - U(3));
        if (icon_button("##refresh", IC_REFRESH, "Look for heroes again", RAISED, INK, ImGui::GetFontSize() + U(22))) {
            find_heroes(T);
            toast(T, "%d hero%s found", NumFound, NumFound == 1 ? "" : "es");
        }
    }
    draw_toast(T, ImVec2(x + w, U(40) + U(64) + U(12)), U(420));
    ImGui::End();
}

// A new hero's first question (the user's word, 2026-11-23: "When you import a new hero you need to first choose who to use as a
// template / based on" and "the model should be resized to that hero"): who it's based on, the eight each with how tall they stand (a
// bar beside them, the tallest's full); it's made their height (a hero's or its own after, Hero Setup's Height), then Hero Setup
void base_chooser(Tool &T) {
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Based on", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    float w = fminf(U(900), io.DisplaySize.x - U(40)), x = (io.DisplaySize.x - w) / 2, gap = U(16);
    float tw = (w - 3 * gap) / 4, th = U(150), total = U(96) + 2 * th + gap + U(80);
    float y = fmaxf(U(30), (io.DisplaySize.y - total) / 2);
    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImGui::SetCursorScreenPos(ImVec2(x, y));
    ImGui::PushFont(FontBig);
    ImGui::Text("Who is %s based on?", T.name);
    ImGui::PopFont();
    ImGui::SetCursorScreenPos(ImVec2(x, y + U(42)));
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + w);
    ImGui::TextColored(V4(MUTED), "It plays as them: their animations, speed, ride or prop, and their sounds till it has its own. It starts as tall "
                                  "as they are.");
    ImGui::PopTextWrapPos();
    float tallest = 0;
    for (int h = 0; h < 8; h++) {
        tallest = fmaxf(tallest, (float)HeroHeights[h]);
    }
    for (int h = 0; h < 8; h++) {
        ImVec2 p(x + (h % 4) * (tw + gap), y + U(96) + (h / 4) * (th + gap));
        ImGui::SetCursorScreenPos(p);
        ImGui::PushID(h);
        bool hit = ImGui::InvisibleButton("##base", ImVec2(tw, th));
        float t = ease_to(ImGui::GetItemID(), ImGui::IsItemHovered() ? 1 : 0, 16);
        ImGui::PopID();
        ImVec2 a(p.x, p.y - t * U(4)), b(p.x + tw, p.y + th - t * U(4));
        dl->AddRectFilled(ImVec2(a.x, a.y + U(3)), ImVec2(b.x, b.y + U(3)), SHADOW, U(24));
        dl->AddRectFilled(a, b, mix(CARD, PERI, t), U(24));
        ImU32 ink = mix(INK, PERI_INK, t), soft = mix(MUTED, PERI_INK, t);
        ImGui::PushFont(FontTitle);
        dl->AddText(ImVec2(a.x + U(18), a.y + U(18)), ink, HeroNames[h]);
        ImGui::PopFont();
        char says[32];
        snprintf(says, sizeof(says), "%d tall", HeroHeights[h]);
        dl->AddText(ImVec2(a.x + U(18), a.y + U(48)), soft, says);
        // how tall, beside the others: a bar from the tile's foot
        float bar = (th - U(36)) * HeroHeights[h] / tallest;
        dl->AddRectFilled(ImVec2(b.x - U(30), b.y - U(18) - bar), ImVec2(b.x - U(18), b.y - U(18)), mix(PERI_LIGHT, PERI_INK, t), U(6));
        if (hit) {
            T.template_hero = h;
            T.height = HeroHeights[h];
            T.choosing = false;
            T.dirty = T.unsaved = true;
            sound_cue(CUE_NAV);
            snprintf(T.status, sizeof(T.status), "Based on %s, %d tall", HeroNames[h], HeroHeights[h]);
        }
    }
    ImGui::SetCursorScreenPos(ImVec2(x, y + U(96) + 2 * th + gap + U(24)));
    if (button("Back##chooser", RAISED, INK, 0, IC_BACK, CUE_DROP)) {
        T.choosing = false;
        T.home = true;
        find_heroes(T);
    }
    ImGui::SetItemTooltip("Back to your heroes (this one isn't kept)");
    ImGui::End();
}

void editor_window(Tool &T, SDL_Window *win) {
    if (T.choosing) {
        base_chooser(T);
        return;
    }
    // made by itself the first time a game's page is open for a model
    if (T.page != PAGE_MODEL && !T.made_asked && T.maker == nullptr) {
        make_in_game(T);
    }
    // the status said once, as a message over the picture; and the games' making done, its sound
    if (T.status[0] != 0 && strcmp(T.status, T.toasted) != 0) {
        SDL_strlcpy(T.toasted, T.status, sizeof(T.toasted));
        toast(T, "%s", T.status);
    }
    static bool making = false;
    if (making && T.maker == nullptr) {
        sound_cue(CUE_MADE);
    }
    making = T.maker != nullptr;
    // adjusting a move: Space plays or stops it, the arrows step a frame, Ctrl+Z undoes and Ctrl+Y (or Ctrl+Shift+Z) does it again;
    // Tab shows the bones or hides them, Esc lets go of the bone picked
    ImGuiIO &io = ImGui::GetIO();
    // on Hero Setup, Esc lets go of the joints picked
    if (T.page == PAGE_MODEL && !T.camera && !io.WantTextInput && ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
        T.joint_sel.clear();
    }
    // on Hero Setup, its joints' undo and redo: Ctrl+Z, and Ctrl+Y (or Ctrl+Shift+Z)
    if (T.page == PAGE_MODEL && !T.camera && !io.WantTextInput && io.KeyCtrl) {
        if (ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
            io.KeyShift ? joint_redo_step(T) : joint_undo_step(T);
            sound_cue(io.KeyShift ? CUE_ON : CUE_OFF);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Y, false)) {
            joint_redo_step(T);
            sound_cue(CUE_ON);
        }
    }
    if (T.adjusting && T.page >= 0 && T.have_pose[T.kind] && !io.WantTextInput) {
        if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
            T.adj_playing = !T.adj_playing;
            sound_cue(T.adj_playing ? CUE_PLAY : CUE_STOP);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            T.frame--;
            T.adj_playing = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            T.frame++;
            T.adj_playing = false;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
            T.bones = !T.bones;
            sound_cue(T.bones ? CUE_ON : CUE_OFF);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false) && (T.sel >= 0 || T.help)) {
            T.sel = -1;
            T.help = false;
            sound_cue(CUE_DROP);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
            io.KeyShift ? adj_redo(T) : adj_undo(T);
            sound_cue(io.KeyShift ? CUE_ON : CUE_OFF);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y, false)) {
            adj_redo(T);
            sound_cue(CUE_ON);
        }
        // Ctrl+C copies a key, Ctrl+V pastes it at the frame shown on the joint picked (Ctrl+Shift+V mirrored)
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C, false)) {
            key_copy(T);
            sound_cue(CUE_TAP);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_A, false) && T.made[T.kind].nanims > 0) {
            keys_all(T, T.made[T.kind].anims[T.anim < T.made[T.kind].nanims ? T.anim : 0].name);
            sound_cue(CUE_PICK);
        }
        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V, false)) {
            key_paste(T, T.sel, T.frame, io.KeyShift);
            sound_cue(CUE_PICK);
        }
        // R, G and S pick the gizmo, as Blender's
        if (!io.KeyCtrl && !T.dragging && T.bones) {
            int was = T.gizmo;
            T.gizmo = ImGui::IsKeyPressed(ImGuiKey_R, false) ? GIZMO_TURN : ImGui::IsKeyPressed(ImGuiKey_G, false) ? GIZMO_MOVE
                    : ImGui::IsKeyPressed(ImGuiKey_S, false) ? GIZMO_SIZE : T.gizmo;
            if (T.gizmo != was) {
                sound_cue(CUE_TAP);
            }
        }
    }
    // a game shown: the keypad looks from a side, as Blender's (1 its front, 3 its left side, 7 above; Ctrl from the other side)
    if (T.page >= 0 && !io.WantTextInput) {
        int from = ImGui::IsKeyPressed(ImGuiKey_Keypad1, false) ? 4 : ImGui::IsKeyPressed(ImGuiKey_Keypad3, false) ? 0
                 : ImGui::IsKeyPressed(ImGuiKey_Keypad7, false) ? 2 : -1;
        if (from >= 0) {
            view_from(T.game_view, T.made_turn, io.KeyCtrl ? (from + 3) % 6 : from);
            sound_cue(CUE_VIEW);
        }
    }
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(U(18), U(14)));
    ImGui::Begin("Hero tool", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleVar();
    top_bar(T);
    if (T.strip_shown) {
        games_strip(T);
    } else {
        ImGui::Dummy(ImVec2(0, U(4)));
    }
    if (T.page == PAGE_MODEL) {
        model_page(T, win);
    } else if (T.page == PAGE_ALL) {
        all_games_page(T);
    } else {
        game_page(T);
    }
    if (T.ask_open) {
        ImGui::OpenPopup("Replace its own animation?");
        T.ask_open = false;
    }
    ask_back(T);
    if (T.page == PAGE_ALL) {
        draw_toast(T, ImVec2(ImGui::GetIO().DisplaySize.x - U(18), U(14) + ImGui::GetFrameHeight() + U(16) + U(118) + U(50)), U(420));
    }
    ImGui::End();
}

} // namespace


// The window at scale s (Ui): its font made at that size, its style's sizes from the plain one (base), and its pictures that big
void ui_scale(Tool &T, const ImGuiStyle &base, float s) {
    ImGuiIO &io = ImGui::GetIO();
    ImFontConfig cfg;

    Ui = s < 0.75f ? 0.75f : s > 4 ? 4 : s;
    io.Fonts->Clear();
    // Fredoka at each size it's used at (its default weight is light: brightened a little to read as regular on the dark)
    static const ImWchar Ranges[] = { 0x0020, 0x00FF, 0x2013, 0x2026, 0x2212, 0x2212, 0 };
    cfg.FontDataOwnedByAtlas = false;
    cfg.RasterizerMultiply = 1.35f;
    cfg.OversampleH = 2;
    auto at = [&](float px) {
        return io.Fonts->AddFontFromMemoryTTF((void *)FredokaTtf, (int)FredokaTtfSize, floorf(px * Ui + 0.5f), &cfg, Ranges);
    };
    FontBody = at(16);
    FontSmall = at(13);
    FontTitle = at(19);
    FontBig = at(26);
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
        T.thumbs[k].grid = true;
        view_free(T.strip[k]);
        T.strip[k].w = T.strip[k].h = (int)U(144);
        T.strip[k].bg = 0;
    }
    view_free(T.face_view);
    T.face_view.w = T.face_view.h = (int)U(320);
    T.face_view.bg = 0;
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
    graphite_style(ImGui::GetStyle());
    // its sounds, but none while a test drives it (the test's own hooks are set)
    if (SDL_getenv("HEROTOOL_SHOT") == nullptr && SDL_getenv("HEROTOOL_SAVE_TO") == nullptr && SDL_getenv("HEROTOOL_LIVE") == nullptr) {
        sound_init();
        sound_load();
    }
    strip_load(T);
    /* a test's: HEROTOOL_NOSTRIP=1 hides the strip of games */
    if (SDL_getenv("HEROTOOL_NOSTRIP") != nullptr) {
        T.strip_shown = false;
    }
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
    /* a test's: HEROTOOL_DELETE=ID deletes that hero, as its trash does (said yes to); HEROTOOL_NEW=MODEL.glb starts a new hero from it, as
       the start screen does, and HEROTOOL_BASE=HERO answers who it's based on (crash, coco, ...) */
    if (SDL_getenv("HEROTOOL_DELETE") != nullptr) {
        find_heroes(T);
        for (int i = 0; i < NumFound; i++) {
            if (SDL_strcmp(Founds[i].id, SDL_getenv("HEROTOOL_DELETE")) == 0) {
                delete_hero(T, Founds[i]);
                printf("herotool: %s\n", T.status);
            }
        }
        find_heroes(T);
    }
    /* (HEROTOOL_ASK_DELETE=ID asks that hero's delete, as its trash does) */
    if (SDL_getenv("HEROTOOL_ASK_DELETE") != nullptr) {
        find_heroes(T);
        for (int i = 0; i < NumFound; i++) {
            if (SDL_strcmp(Founds[i].id, SDL_getenv("HEROTOOL_ASK_DELETE")) == 0) {
                Deleting = i, DeleteAsk = true;
            }
        }
    }
    if (SDL_getenv("HEROTOOL_NEW") != nullptr) {
        new_hero(T, SDL_getenv("HEROTOOL_NEW"));
        for (int h = 0; h < 8 && SDL_getenv("HEROTOOL_BASE") != nullptr; h++) {
            if (SDL_strcasecmp(SDL_getenv("HEROTOOL_BASE"), Heroes[h]) == 0) {
                T.template_hero = h, T.height = HeroHeights[h], T.choosing = false;
            }
        }
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
    /* a test's: HEROTOOL_ASK_GUESS=1 asks Guess joints' question, as its button does */
    if (SDL_getenv("HEROTOOL_ASK_GUESS") != nullptr) {
        T.ask_guess = true;
    }
    /* a test's: HEROTOOL_JOINTS=2 takes its joints as placed by hand (as a dot dragged), and with HEROTOOL_SAVE_TO=FOLDER (no
       HEROTOOL_ADJUST) saves it there */
    if (SDL_getenv("HEROTOOL_JOINTS") != nullptr) {
        T.model_view = 0;
        if (SDL_atoi(SDL_getenv("HEROTOOL_JOINTS")) == 2 && T.m != nullptr) {
            T.rigged = false;
            reweigh(T);
            if (SDL_getenv("HEROTOOL_SAVE_TO") != nullptr && SDL_getenv("HEROTOOL_ADJUST") == nullptr) {
                export_pack(T, false, SDL_getenv("HEROTOOL_SAVE_TO"));
                printf("herotool: %s\n", T.status);
            }
        }
    }
    /* a test's: HEROTOOL_CAMERA=YAW opens the card's camera (on the hero's page) turned so */
    if (SDL_getenv("HEROTOOL_CAMERA") != nullptr && T.m != nullptr) {
        T.camera = true;
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
                if (n > 4 && SDL_strcasecmp(f + n - 4, ".glb") == 0 && T.home) {
                    new_hero(T, f);
                } else if (n > 4 && SDL_strcasecmp(f + n - 4, ".glb") == 0) {
                    // (on a hero's pages: its model another)
                    T.loaded_mod[0] = 0;
                    open_model(T, f);
                } else if (n > 4 && SDL_strcasecmp(f + n - 4, ".png") == 0) {
                    SDL_strlcpy(T.card, f, sizeof(T.card));
                    T.card_photo = false;
                    T.unsaved = true;
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
            if (what == PICK_MODEL && T.home) {
                new_hero(T, PickedPath);
            } else if (what == PICK_MODEL) {
                // (its Model card's Open...: its model another)
                SDL_strlcpy(T.model, PickedPath, sizeof(T.model));
                T.loaded_mod[0] = 0;
                open_model(T, T.model);
            } else if (what == PICK_CARD) {
                SDL_strlcpy(T.card, PickedPath, sizeof(T.card));
                T.card_photo = false;
                T.unsaved = true;
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
        /* a test's: HEROTOOL_CAMERA=YAW[,PITCH,ZOOM,ACROSS,DOWN] once the camera's posed (HEROTOOL_CAMERA_POSE=MOVE@FRAME its pose), and
           HEROTOOL_PHOTO takes its photo then */
        static bool turned = false, photographed = false;
        if (!turned && T.camera && T.cam_anim >= 0 && SDL_getenv("HEROTOOL_CAMERA") != nullptr) {
            float c[5] = { 0, T.cam.pitch, T.cam.zoom, T.cam.px, T.cam.py };
            const char *at = SDL_getenv("HEROTOOL_CAMERA");
            for (int i = 0; i < 5 && at != nullptr; i++) {
                c[i] = (float)SDL_atof(at);
                at = strchr(at, ',');
                at = at != nullptr ? at + 1 : nullptr;
            }
            turned = true;
            T.cam.yaw = c[0], T.cam.pitch = c[1], T.cam.zoom = c[2], T.cam.px = c[3], T.cam.py = c[4];
            if (const char *pose = SDL_getenv("HEROTOOL_CAMERA_POSE")) {
                const char *f = strchr(pose, '@');
                for (int a = 0; a < T.made[0].nanims; a++) {
                    if (SDL_strncasecmp(T.made[0].anims[a].name, pose, f != nullptr ? (size_t)(f - pose) : strlen(pose)) == 0) {
                        T.cam_anim = a;
                    }
                }
                T.cam_frame = f != nullptr ? SDL_atoi(f + 1) : 0;
            }
        }
        if (turned && !photographed && SDL_getenv("HEROTOOL_PHOTO") != nullptr) {
            photographed = true;
            /* (and HEROTOOL_SAVE_TO=FOLDER, without HEROTOOL_ADJUST, saves it there with it) */
            if (take_photo(T, ren) && SDL_getenv("HEROTOOL_SAVE_TO") != nullptr && SDL_getenv("HEROTOOL_ADJUST") == nullptr) {
                export_pack(T, false, SDL_getenv("HEROTOOL_SAVE_TO"));
                printf("herotool: %s\n", T.status);
            }
        }
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
            /* and HEROTOOL_HIDE=1 hides it, as Hide does (with X-Mirror if HEROTOOL_XMIRROR; 2: and shows it again); HEROTOOL_NOFIT=1 turns
               Fit in seat off */
            if (SDL_getenv("HEROTOOL_HIDE") != nullptr && T.sel >= 0) {
                T.xmirror = SDL_getenv("HEROTOOL_XMIRROR") != nullptr;
                bone_hide(T, T.poses[T.kind].names[T.sel], true);
                if (SDL_atoi(SDL_getenv("HEROTOOL_HIDE")) == 2) {
                    bone_hide(T, T.poses[T.kind].names[T.sel], false);
                }
            }
            T.no_fit[T.kind] = SDL_getenv("HEROTOOL_NOFIT") != nullptr ? true : T.no_fit[T.kind];
            /* and HEROTOOL_ADJUST_TURN / _REACH / _SCALE=X,Y,Z changes it there, as the panel's values would; _GIZMO=N picks gizmo N;
               HEROTOOL_NOSTRETCH=1 turns Stretch off */
            T.stretch = SDL_getenv("HEROTOOL_NOSTRETCH") == nullptr;
            const char *turn = SDL_getenv("HEROTOOL_ADJUST_TURN"), *reach = SDL_getenv("HEROTOOL_ADJUST_REACH");
            const char *size = SDL_getenv("HEROTOOL_ADJUST_SCALE");
            T.gizmo = SDL_getenv("HEROTOOL_GIZMO") != nullptr ? SDL_atoi(SDL_getenv("HEROTOOL_GIZMO")) : T.gizmo;
            /* (HEROTOOL_PITCH=DEG / HEROTOOL_YAW=DEG: the picture tilted and turned so) */
            T.game_view.pitch = SDL_getenv("HEROTOOL_PITCH") != nullptr ? (float)SDL_atof(SDL_getenv("HEROTOOL_PITCH")) : T.game_view.pitch;
            T.made_turn = SDL_getenv("HEROTOOL_YAW") != nullptr ? (float)SDL_atof(SDL_getenv("HEROTOOL_YAW")) : T.made_turn;
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
                if (SDL_getenv("HEROTOOL_ALLMOVES") != nullptr) {
                    keys_all_moves(T, true);
                    if (SDL_atoi(SDL_getenv("HEROTOOL_ALLMOVES")) == 2) {
                        keys_all_moves(T, false);
                    }
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
        for (int k = 0; k < PACK_KINDS && !T.home; k++) {
            if (T.have_made[k] && T.made[k].nanims > 0 && T.strip[k].w > 0) {
                T.strip[k].yaw = 25;
                T.strip[k].pitch = 8;
                draw_made(&T.made[k], breathe_of(&T.made[k]), 0, T.strip[k], ren, T.rides[k], T.nrides[k]);
            }
        }
        if (!T.home && T.page == PAGE_MODEL && T.have_made[0] && T.made[0].nanims > 0 && T.face_view.w > 0) {
            T.face_view.yaw = 0;
            T.face_view.pitch = 0;
            draw_made(&T.made[0], breathe_of(&T.made[0]), 0, T.face_view, ren);
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
            // the front and side views at rest and the third playing the move, all at once as the model's page shows them, only while it's
            // shown (and not its card's camera)
            if (!T.camera) {
                draw(T.m, T.m->pos, T.front, ren);
                draw(T.m, T.m->pos, T.side, ren);
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
        /* a test's: HEROTOOL_HOVER=X,Y keeps the mouse there (the window's pixels), for what a hover shows */
        if (const char *hv = SDL_getenv("HEROTOOL_HOVER")) {
            float hx = 0, hy = 0;
            if (SDL_sscanf(hv, "%f,%f", &hx, &hy) == 2) {
                ImGui::GetIO().AddMousePosEvent(hx, hy);
            }
        }
        /* a test's drag: HEROTOOL_DRAG=X0,Y0,X1,Y1[,SHIFT[,BUTTON]] presses the left button (or the right) at (X0, Y0) in the window once a game's shown, moves
           to (X1, Y1) over ten frames and lets go (Shift held if SHIFT is 1), as a person's mouse would, and keeps it there after (the
           real mouse left out: the SDL backend gives where it is on the screen while the window has the focus) */
        /* (or on Hero Setup, its views settled) */
        static int setup_frames = 0;
        setup_frames = !T.home && !T.choosing && T.page == PAGE_MODEL ? setup_frames + 1 : 0;
        if (SDL_getenv("HEROTOOL_DRAG") != nullptr
            && (drag_step >= 0 || T.adjusting || (T.page >= 0 && T.have_made[T.kind] && T.maker == nullptr) || setup_frames > 5)) {
            /* (a sixth, BUTTON: 0 the left, 1 the right) */
            float d[6] = { 0, 0, 0, 0, 0, 0 };
            drag_step = drag_step < 100 ? drag_step + 1 : drag_step;
            /* (and HEROTOOL_DRAG2, the same, a second drag after the first: a box then a move) */
            const char *spec = SDL_getenv("HEROTOOL_DRAG");
            int st = drag_step;
            if (SDL_getenv("HEROTOOL_DRAG2") != nullptr && drag_step >= 18) {
                spec = SDL_getenv("HEROTOOL_DRAG2"), st = drag_step - 18;
            }
            /* (and HEROTOOL_DRAG3, a third after the second: a click on a menu the second opened) */
            if (SDL_getenv("HEROTOOL_DRAG3") != nullptr && drag_step >= 36) {
                spec = SDL_getenv("HEROTOOL_DRAG3"), st = drag_step - 36;
            }
            SDL_sscanf(spec, "%f,%f,%f,%f,%f,%f", &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
            ImGuiIO &dio = ImGui::GetIO();
            // Shift down the frame before the press and up the frame after the release, and the mouse before it each frame: ImGui
            // keeps a mouse move or press queued after a key's change for the frame after (its input trickling)
            float t = st < 3 ? 0 : st > 12 ? 1 : (st - 2) / 10.0f;
            dio.AddMousePosEvent(d[0] + (d[2] - d[0]) * t, d[1] + (d[3] - d[1]) * t);
            dio.AddMouseButtonEvent(d[5] != 0 ? 1 : 0, st >= 2 && st <= 12);
            dio.AddKeyEvent(ImGuiMod_Shift, d[4] != 0 && st >= 1 && st <= 13);
        }
        /* a test's: HEROTOOL_REBASE=HERO bases the hero open on another once it's made, as Based on does, and says what it's doing */
        static bool rebased = false;
        if (!rebased && SDL_getenv("HEROTOOL_REBASE") != nullptr && T.m != nullptr && !T.home && !T.choosing && T.have_made[0] && T.maker == nullptr) {
            rebased = true;
            for (int h = 0; h < 8; h++) {
                if (SDL_strcasecmp(SDL_getenv("HEROTOOL_REBASE"), Heroes[h]) == 0) {
                    int own = 0;
                    for (int g = 0; g < KitGameCount && g < 8; g++) {
                        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                            own += T.ownmap[g][m] != OWN_TEMPLATE;
                        }
                    }
                    printf("herotool: before: based on %s, %d tall, %d own moves, %d baked\n", Heroes[T.template_hero], T.height, own, (int)T.baked.size());
                    rebase(T, h);
                    own = 0;
                    for (int g = 0; g < KitGameCount && g < 8; g++) {
                        for (int m = 0; m < 8 && KitGames[g].moves[m] != nullptr; m++) {
                            own += T.ownmap[g][m] != OWN_TEMPLATE;
                        }
                    }
                    printf("herotool: after: based on %s, %d tall, %d own moves, %d baked, making %s\n", Heroes[T.template_hero], T.height, own,
                           (int)T.baked.size(), T.maker != nullptr ? "yes" : "no");
                }
            }
        }
        /* a test's: HEROTOOL_ADD=1 saves the hero open and adds it to the game, as Save then Add to game do, once it's open */
        static bool added = false;
        if (!added && SDL_getenv("HEROTOOL_ADD") != nullptr && T.m != nullptr && !T.home && !T.choosing) {
            added = true;
            export_pack(T, false);
            bool did = add_to_game(T);
            printf("herotool: add to game %s: %s\n", did ? "done" : "not done", T.status);
        }
        /* a test's: HEROTOOL_STRAIGHTEN=VIEW straightens Hero Setup's joints picked onto the grid in that view (0 front, 1 side), or "line"
           lines them up between the ends, once its test drag's let go (a box round them) */
        static bool straightened = false;
        if (!straightened && SDL_getenv("HEROTOOL_STRAIGHTEN") != nullptr && drag_step > 16 && T.m != nullptr) {
            straightened = true;
            const char *how = SDL_getenv("HEROTOOL_STRAIGHTEN");
            bool did = SDL_strcmp(how, "line") == 0 ? joints_straighten(T) : joints_to_grid(T, SDL_atoi(how));
            printf("herotool: straightened %d: %s\n", (int)T.joint_sel.size(), did ? "yes" : "no");
            for (int b = 0; b < KB_COUNT; b++) {
                const KitBone &k = T.m->bones[b];
                if (k.have) {
                    printf("herotool: straight %s %.4f,%.4f,%.4f to %.4f,%.4f,%.4f\n", KitBoneNames[b], k.at[0], k.at[1], k.at[2], k.end[0], k.end[1],
                           k.end[2]);
                }
            }
        }
        /* a test's: HEROTOOL_UNDO=1 undoes Hero Setup's last joints' step once its test drag's let go, and says where its joints are then */
        static bool undone = false;
        if (!undone && SDL_getenv("HEROTOOL_UNDO") != nullptr && drag_step > 16 && T.m != nullptr) {
            undone = true;
            joint_undo_step(T);
            for (int b = 0; b < KB_COUNT; b++) {
                const KitBone &k = T.m->bones[b];
                if (k.have) {
                    printf("herotool: undone %s %.4f,%.4f,%.4f to %.4f,%.4f,%.4f\n", KitBoneNames[b], k.at[0], k.at[1], k.at[2], k.end[0], k.end[1],
                           k.end[2]);
                }
            }
        }
        /* a test's wheel: HEROTOOL_WHEEL=X,Y,N turns the mouse wheel N notches at (X, Y) once a game's being adjusted, the mouse kept there */
        static int wheel_step = -1;
        if (SDL_getenv("HEROTOOL_WHEEL") != nullptr && SDL_getenv("HEROTOOL_DRAG") == nullptr && (wheel_step >= 0 || T.adjusting || setup_frames > 5)) {
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
        SDL_SetRenderDrawColor(ren, 0x19, 0x19, 0x1B, 255);
        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ren);
        lap(PR_DRAW);
        /* (HEROTOOL_SHOT_FRAME=N takes it N frames in, not 40: after a test's drags) */
        if (shot != nullptr && T.maker == nullptr && ++frames == (SDL_getenv("HEROTOOL_SHOT_FRAME") ? SDL_atoi(SDL_getenv("HEROTOOL_SHOT_FRAME")) : 40)) {
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
    sound_quit();
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
