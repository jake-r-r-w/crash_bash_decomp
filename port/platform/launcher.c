#include <SDL3/SDL.h>
#include "launcher.h"
#include "settings.h"

#define WIDTH 640
#define HEIGHT 480
#define ROW_X 60
#define ROW_W (WIDTH - ROW_X * 2)
#define ROW_H 32
#define ROWS_Y 96
#define VALUE_X 410

typedef struct {
    int w, h;
} Size;

static const Size WindowSizes[] = { { 640, 480 }, { 960, 720 }, { 1280, 960 }, { 1600, 1200 }, { 1920, 1440 } };
static const Size RenderSizes[] = { { 640, 480 }, { 1280, 960 }, { 1920, 1440 }, { 2560, 1920 }, { 3840, 2880 } };

enum {
    ROW_SKIP_INTRO,
    ROW_SKIP_LOADING,
    ROW_FULLSCREEN,
    ROW_WINDOW,
    ROW_RENDER,
    ROW_FPS,
    ROW_VSYNC,
    ROW_PLAY,
    ROW_QUIT,
    NUM_ROWS
};

static const char *const RowNames[NUM_ROWS] = {
    "Skip intro screens", "Skip loading screens", "Display", "Window size", "Render resolution", "Frame rate", "VSync", "PLAY", "QUIT",
};

static SDL_Renderer *Renderer;
static SDL_Gamepad *Pad;
static int Selected = ROW_PLAY;

/* Moves through a list of sizes from the one in use, wrapping at the ends. */
static void step_size(const Size *sizes, int count, int *w, int *h, int dir) {
    int i, at = 0;

    for (i = 0; i < count; i++) {
        if (sizes[i].w == *w && sizes[i].h == *h) {
            at = i;
        }
    }
    at = (at + dir + count) % count;
    *w = sizes[at].w;
    *h = sizes[at].h;
}

/* Changes the setting on a row. */
static void change_row(int row, int dir) {
    Settings *s = settings();

    switch (row) {
    case ROW_SKIP_INTRO:
        s->skip_intro = !s->skip_intro;
        break;
    case ROW_SKIP_LOADING:
        s->skip_loading = !s->skip_loading;
        break;
    case ROW_FULLSCREEN:
        s->fullscreen = !s->fullscreen;
        break;
    case ROW_WINDOW:
        step_size(WindowSizes, SDL_arraysize(WindowSizes), &s->window_w, &s->window_h, dir);
        break;
    case ROW_RENDER:
        step_size(RenderSizes, SDL_arraysize(RenderSizes), &s->render_w, &s->render_h, dir);
        break;
    case ROW_FPS:
        s->fps = dir > 0 ? (s->fps == 30 ? 60 : s->fps == 60 ? 120 : 30) : (s->fps == 30 ? 120 : s->fps == 60 ? 30 : 60);
        break;
    case ROW_VSYNC:
        s->vsync = !s->vsync;
        break;
    }
}

static void row_value(int row, char *text, size_t size) {
    const Settings *s = settings();

    text[0] = '\0';
    switch (row) {
    case ROW_SKIP_INTRO:
        SDL_strlcpy(text, s->skip_intro ? "On" : "Off", size);
        break;
    case ROW_SKIP_LOADING:
        SDL_strlcpy(text, s->skip_loading ? "On" : "Off", size);
        break;
    case ROW_FULLSCREEN:
        SDL_strlcpy(text, s->fullscreen ? "Fullscreen" : "Windowed", size);
        break;
    case ROW_WINDOW:
        SDL_snprintf(text, size, "%d x %d", s->window_w, s->window_h);
        break;
    case ROW_RENDER:
        SDL_snprintf(text, size, "%d x %d", s->render_w, s->render_h);
        break;
    case ROW_FPS:
        if (s->fps == 30) {
            SDL_strlcpy(text, "30 (PS1)", size);
        } else {
            SDL_snprintf(text, size, "%d", s->fps);
        }
        break;
    case ROW_VSYNC:
        SDL_strlcpy(text, s->vsync ? "On" : "Off", size);
        break;
    }
}

static void draw_text(float x, float y, float scale, const char *text) {
    SDL_SetRenderScale(Renderer, scale, scale);
    SDL_RenderDebugText(Renderer, x / scale, y / scale, text);
    SDL_SetRenderScale(Renderer, 1, 1);
}

static SDL_FRect row_rect(int row) {
    SDL_FRect r;

    r.x = ROW_X;
    r.y = (float)(ROWS_Y + row * ROW_H + (row >= ROW_PLAY ? 12 : 0));
    r.w = ROW_W;
    r.h = ROW_H - 4;
    return r;
}

static void draw(void) {
    int i;

    SDL_SetRenderDrawColor(Renderer, 18, 14, 30, 255);
    SDL_RenderClear(Renderer);
    SDL_SetRenderDrawColor(Renderer, 255, 150, 30, 255);
    draw_text(ROW_X, 36, 4, "CRASH BASH");
    SDL_SetRenderDrawColor(Renderer, 170, 165, 190, 255);
    draw_text(ROW_X, 76, 1, "Settings");
    for (i = 0; i < NUM_ROWS; i++) {
        SDL_FRect r = row_rect(i);
        char value[32];

        if (i == Selected) {
            SDL_SetRenderDrawColor(Renderer, 70, 45, 110, 255);
            SDL_RenderFillRect(Renderer, &r);
        }
        SDL_SetRenderDrawColor(Renderer, 240, 238, 245, 255);
        draw_text(r.x + 10, r.y + 7, 2, RowNames[i]);
        row_value(i, value, sizeof(value));
        if (value[0] != '\0') {
            SDL_SetRenderDrawColor(Renderer, 255, 200, 90, 255);
            draw_text(VALUE_X, r.y + 7, 2, value);
        }
    }
    SDL_SetRenderDrawColor(Renderer, 170, 165, 190, 255);
    draw_text(ROW_X, HEIGHT - 44, 1, "Arrows or d-pad to choose, Enter or X to confirm, or the mouse.");
    draw_text(ROW_X, HEIGHT - 30, 1, "Esc quits.");
    SDL_RenderPresent(Renderer);
}

static int row_at(float x, float y) {
    SDL_FPoint p;
    int i;

    p.x = x;
    p.y = y;
    for (i = 0; i < NUM_ROWS; i++) {
        SDL_FRect r = row_rect(i);

        if (SDL_PointInRectFloat(&p, &r)) {
            return i;
        }
    }
    return -1;
}

/* Confirming: play, quit, or change the row. Returns 1 to play, 0 to quit, -1 to stay. */
static int confirm(void) {
    if (Selected == ROW_PLAY) {
        return 1;
    }
    if (Selected == ROW_QUIT) {
        return 0;
    }
    change_row(Selected, 1);
    return -1;
}

/* A direction or confirm from the keyboard or a pad. Returns as confirm() does. */
static int navigate(int up, int down, int left, int right, int ok) {
    if (up) {
        Selected = (Selected + NUM_ROWS - 1) % NUM_ROWS;
    }
    if (down) {
        Selected = (Selected + 1) % NUM_ROWS;
    }
    if (left) {
        change_row(Selected, -1);
    }
    if (right) {
        change_row(Selected, 1);
    }
    return ok ? confirm() : -1;
}

/* Returns 1 to play, 0 to quit, -1 to stay. */
static int handle_event(const SDL_Event *e) {
    SDL_Event ev = *e;
    int row;

    /* the mouse in the menu's own coordinates, whatever the window's size */
    SDL_ConvertEventToRenderCoordinates(Renderer, &ev);
    e = &ev;
    switch (e->type) {
    case SDL_EVENT_QUIT:
        return 0;
    case SDL_EVENT_KEY_DOWN:
        if (e->key.key == SDLK_ESCAPE) {
            return 0;
        }
        return navigate(e->key.key == SDLK_UP, e->key.key == SDLK_DOWN, e->key.key == SDLK_LEFT, e->key.key == SDLK_RIGHT,
                        e->key.key == SDLK_RETURN || e->key.key == SDLK_SPACE);
    case SDL_EVENT_GAMEPAD_ADDED:
        if (Pad == NULL) {
            Pad = SDL_OpenGamepad(e->gdevice.which);
        }
        break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
        return navigate(e->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP, e->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN,
                        e->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT, e->gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
                        e->gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH);
    case SDL_EVENT_MOUSE_MOTION:
        row = row_at(e->motion.x, e->motion.y);
        if (row >= 0) {
            Selected = row;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        row = row_at(e->button.x, e->button.y);
        if (row >= 0) {
            Selected = row;
            return confirm();
        }
        break;
    default:
        break;
    }
    return -1;
}

static int open_renderer(void *window) {
    Renderer = SDL_CreateRenderer(window, NULL);
    if (Renderer == NULL) {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return 0;
    }
    SDL_SetRenderLogicalPresentation(Renderer, WIDTH, HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(Renderer, 1);
    return 1;
}

static void close_renderer(void) {
    if (Pad != NULL) {
        SDL_CloseGamepad(Pad);
        Pad = NULL;
    }
    SDL_DestroyRenderer(Renderer);
    Renderer = NULL;
}

int launcher_run(void *window) {
    int result = -1;

    if (!open_renderer(window)) {
        return 1; /* no menu: go on with the settings as they are */
    }
    while (result < 0) {
        SDL_Event e;

        draw();
        if (SDL_WaitEvent(&e)) {
            result = handle_event(&e);
            while (result < 0 && SDL_PollEvent(&e)) {
                result = handle_event(&e);
            }
        }
    }
    close_renderer();
    if (result == 1) {
        settings_save();
    }
    return result;
}

int launcher_run_keys(void *window, const char *keys) {
    int result = -1;

    if (!open_renderer(window)) {
        return 0;
    }
    for (; *keys != '\0' && result < 0; keys++) {
        draw();
        result = navigate(*keys == 'u', *keys == 'd', *keys == 'l', *keys == 'r', *keys == 'e');
    }
    close_renderer();
    return result;
}

int launcher_screenshot(void *window, const char *path) {
    SDL_Surface *s;
    int ok = 0;

    if (!open_renderer(window)) {
        return 0;
    }
    draw();
    draw();
    s = SDL_RenderReadPixels(Renderer, NULL);
    if (s != NULL) {
        ok = SDL_SavePNG(s, path);
        SDL_DestroySurface(s);
    }
    close_renderer();
    return ok;
}
