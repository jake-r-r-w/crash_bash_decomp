// The hero tool's window (docs/HERO_PACKS.md, The hero tool): the same steps as `herotool import`, with the model to look at.
//
// Drop a model (.glb) on it, or open one. A rigged model's own bones are found by their names; an unrigged one gets markers guessed
// from its shape, to be dragged onto its joints in the front and side views (its left side's mirror its right's unless that's turned
// off). The moves play on it as the game will play them. Set its name, height and the rest, add a card and sounds (drop a .png or a
// .wav on it), then Export writes the mod, and Test in game starts the game with the hero in it.
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

struct View {
    SDL_Texture *tex = nullptr;
    unsigned *rgba = nullptr;
    float *depth = nullptr;
    // model to screen: x across (from its axes, turned by yaw), y up, both scaled and moved to fit
    float yaw = 0, scale = 1, ox = 0, oy = 0;
};

struct Tool {
    KitModel *m = nullptr;
    char model[1024] = "";
    char status[2400] = "Drop a .glb model on this window, or type its path and Open.";
    bool rigged = false, mirror = true, playing = true;
    int move = 0;
    float t = 0, turn = 30;
    float (*posed)[3] = nullptr;
    View front, side, play;
    // the pack
    char id[32] = "myhero", name[32] = "MY HERO", mod_name[64] = "My hero", author[64] = "", card[1024] = "";
    char voice[PACK_VOICE_JOBS][1024] = {};
    int height = 560, prefer = 0, voice_job = 1, mode = 0;
    float face_head = 0;
    char out[1024] = "", game[1024] = "";
    // a marker being dragged: its bone, whether it's the bone's end, and in which view
    int drag_bone = -1, drag_end = 0, drag_view = 0;
};

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

void export_pack(Tool &T, bool test) {
    char why[512], markers[1100];
    KitModel *t = new KitModel();
    KitPack p = {};

    if (T.m == nullptr) {
        return;
    }
    if (!kit_read(T.model, t, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
        delete t;
        return;
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
    p.height = T.height;
    p.face_head = T.face_head;
    p.card = T.card[0] ? T.card : nullptr;
    for (int j = 0; j < PACK_VOICE_JOBS; j++) {
        p.voice[j] = T.voice[j][0] ? T.voice[j] : nullptr;
    }
    char folder[1100];
    snprintf(folder, sizeof(folder), "%s/%s", T.out, T.id);
    SDL_CreateDirectory(folder);
    if (!kit_atlas(t, why, sizeof(why)) || !kit_write(t, &p, folder, why, sizeof(why))) {
        snprintf(T.status, sizeof(T.status), "%s", why);
    } else {
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
    if (game != nullptr) {
        SDL_strlcpy(T.game, game, sizeof(T.game));
    }
    if (start != nullptr) {
        open_model(T, start);
    }
    /* a test's: HEROTOOL_EXPORT=FOLDER exports to it once the model's open */
    if (SDL_getenv("HEROTOOL_EXPORT") != nullptr && T.m != nullptr) {
        SDL_strlcpy(T.out, SDL_getenv("HEROTOOL_EXPORT"), sizeof(T.out));
        export_pack(T, false);
        printf("herotool: %s\n", T.status);
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
                    open_model(T, f);
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
        Uint64 now = SDL_GetTicks();
        if (T.m != nullptr && T.playing) {
            T.t += (now - last) / 1000.0f;
            if (T.t > KitMoves[T.move].length) {
                T.t = 0;
            }
        }
        last = now;
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
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Hero tool", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::TextWrapped("%s", T.status);
        ImGui::Separator();
        if (ImGui::BeginTable("main", 2, ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("views", ImGuiTableColumnFlags_WidthFixed, ViewW * 3 + 40);
            ImGui::TableNextColumn();
            if (T.m != nullptr) {
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
        ImGui::Render();
        SDL_SetRenderDrawColor(ren, 30, 30, 34, 255);
        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), ren);
        if (shot != nullptr && ++frames == 40) {
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
