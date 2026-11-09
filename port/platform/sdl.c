/* The host on SDL3: a window, sound, the keyboard and game controllers. */
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "platform.h"
#include "gfx.h"
#include "settings.h"
#include "touch.h"

#define MAX_PADS 4
#define VBLANK_HZ 59.94
#define OUT_RATE 44100

static SDL_Window *Window;
static SDL_AudioStream *Audio;
static SDL_Gamepad *Pads[MAX_PADS];
static Uint64 NextVblank;
static bool Fullscreen;

static void open_pads(void);

/* A phone's game has the whole screen, its bars hidden, whatever the settings (made for a window on a PC) say */
static int want_fullscreen(const Settings *s) {
#ifdef SDL_PLATFORM_ANDROID
    (void)s;
    return 1;
#else
    return s->fullscreen;
#endif
}

void *host_window(void) {
    const Settings *s = settings();

    if (Window != NULL) {
        return Window;
    }
    SDL_SetMainReady();
    /* a phone turns either way: the picture fits it, and the touch buttons go where there's room (touch.c) */
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait PortraitUpsideDown");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        platform_fatal("Crash Bash couldn't start its window and sound.");
        return NULL;
    }
    Window = SDL_CreateWindow("Crash Bash", s->window_w, s->window_h, SDL_WINDOW_RESIZABLE | (want_fullscreen(s) ? SDL_WINDOW_FULLSCREEN : 0));
    if (Window == NULL) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
    }
    return Window;
}

/* A test window's slot in a grid on the screen */
/* The monitor for the window: a test run's out of the way on a second one, if there is one. */
static SDL_DisplayID window_display(void) {
    if (settings()->no_save) {
        SDL_DisplayID primary = SDL_GetPrimaryDisplay();
        int count = 0, i;
        SDL_DisplayID *ids = SDL_GetDisplays(&count);
        SDL_DisplayID other = 0;

        for (i = 0; ids != NULL && i < count; i++) {
            if (ids[i] != primary) {
                other = ids[i];
                break;
            }
        }
        SDL_free(ids);
        if (other != 0) {
            return other;
        }
    }
    return SDL_GetDisplayForWindow(Window);
}

static void place_tile(int tile, int tiles) {
    SDL_Rect area;
    int cols = 1, rows, w, h;

    if (!SDL_GetDisplayUsableBounds(window_display(), &area)) {
        return;
    }
    while (cols * cols < tiles) {
        cols++;
    }
    rows = (tiles + cols - 1) / cols;
    w = area.w / cols;
    h = area.h / rows;
    /* room for the title bar above each */
    SDL_SetWindowSize(Window, w, h - 32);
    SDL_SetWindowPosition(Window, area.x + (tile % cols) * w, area.y + (tile / cols) * h + 32);
}

/* The device asks for sound when it's ready for it, on its own thread, with the stream locked. */
static void SDLCALL fill_audio(void *userdata, SDL_AudioStream *stream, int additional, int total) {
    static int16_t buf[1024 * 2];

    while (additional > 0) {
        int frames = additional / 4;

        if (frames > 1024) {
            frames = 1024;
        }
        if (frames == 0) {
            frames = 1;
        }
        audio_mix(buf, frames);
        SDL_PutAudioStreamData(stream, buf, frames * 4);
        additional -= frames * 4;
    }
}

void host_audio_lock(void) {
    if (Audio != NULL) {
        SDL_LockAudioStream(Audio);
    }
}

void host_audio_unlock(void) {
    if (Audio != NULL) {
        SDL_UnlockAudioStream(Audio);
    }
}

static int ApplyPending;

void settings_apply(void) {
    ApplyPending = 1;
}

/* In the middle of its screen, with its title bar on the screen to drag it by, however big it is. */
static void centre_window(void) {
    SDL_Rect usable;
    int w, h, top = 0;

    if (!SDL_GetDisplayUsableBounds(window_display(), &usable)) {
        return;
    }
    SDL_GetWindowSize(Window, &w, &h);
    SDL_GetWindowBordersSize(Window, &top, NULL, NULL, NULL);
    SDL_SetWindowPosition(Window, usable.x + (usable.w > w ? (usable.w - w) / 2 : 0),
                          usable.y + (usable.h > h + top ? (usable.h - h - top) / 2 : 0) + top);
}

/* The screen's room for a window, below its title bar. */
static int window_room(int *w, int *h) {
    SDL_Rect usable;
    int top = 0;

    if (Window == NULL || !SDL_GetDisplayUsableBounds(window_display(), &usable)) {
        return 0;
    }
    SDL_GetWindowBordersSize(Window, &top, NULL, NULL, NULL);
    *w = usable.w;
    *h = usable.h - top;
    return 1;
}

int settings_window_fits(int w, int h) {
    int room_w, room_h;

    return !window_room(&room_w, &room_h) || (w <= room_w && h <= room_h);
}

/* A size the screen can't hold, as one kept from a window that was maximised, is a maximised window, at the most of that shape that fits for when it's not. */
static void keep_on_screen(Settings *s) {
    int room_w, room_h;
    float scale;

    if (!window_room(&room_w, &room_h) || settings_window_fits(s->window_w, s->window_h) || room_w <= 0 || room_h <= 0) {
        return;
    }
    scale = SDL_min((float)room_w / s->window_w, (float)room_h / s->window_h);
    s->window_w = (int)(s->window_w * scale);
    s->window_h = (int)(s->window_h * scale);
    s->maximized = 1;
}

/* The window as the settings have it. Coming out of fullscreen or to another size it's centred; otherwise it stays where it was moved to. */
static GfxConfig gfx_config_from(const Settings *s) {
    GfxConfig c;

    c.width = s->render_w;
    c.height = s->render_h;
    c.vsync = s->vsync;
    c.fps = s->fps;
    c.antialias = s->antialias;
    c.smooth = s->smooth;
    c.wide = s->widescreen;
    c.upscale = s->upscale;
    c.icons = s->icons;
    return c;
}

static void place_window(void) {
    Settings *s = settings();
    int was_fullscreen = Fullscreen, w, h, maximised;

    Fullscreen = want_fullscreen(s);
    SDL_SetWindowFullscreen(Window, Fullscreen);
    SDL_SyncWindow(Window);
    if (s->tiles == 0) {
        keep_on_screen(s);
    }
    /* a maximised window takes no size: it has to come out of that first */
    maximised = (SDL_GetWindowFlags(Window) & SDL_WINDOW_MAXIMIZED) != 0;
    if (!Fullscreen && maximised && !s->maximized) {
        SDL_RestoreWindow(Window);
        SDL_SyncWindow(Window);
        maximised = 0;
    }
    SDL_GetWindowSize(Window, &w, &h);
    if (!Fullscreen && !maximised && (was_fullscreen || w != s->window_w || h != s->window_h)) {
        SDL_SetWindowSize(Window, s->window_w, s->window_h);
        SDL_SyncWindow(Window);
        if (s->tiles == 0) {
            centre_window();
        }
    }
    if (!Fullscreen && !maximised && s->maximized && s->tiles == 0) {
        SDL_MaximizeWindow(Window);
    }
    if (s->tiles > 0) {
        place_tile(s->tile, s->tiles);
    }
    SDL_SyncWindow(Window);
}

int host_init(void) {
    const Settings *s = settings();
    SDL_AudioSpec spec;

    if (host_window() == NULL) {
        return 0;
    }
    place_window();
    if (!Fullscreen && s->tiles == 0 && !s->maximized) {
        centre_window();
    }
    GfxConfig config = gfx_config_from(s);

    if (!gfx_init(Window, &config)) {
        return 0;
    }
    open_pads();
    /* a test run is silent, unless it's asked for (--sound): it can record its sound instead (--wav) */
    if (!s->no_save || s->test_sound) {
        spec.format = SDL_AUDIO_S16;
        spec.channels = 2;
        spec.freq = OUT_RATE;
        SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "512");
        Audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, fill_audio, NULL);
        if (Audio != NULL) {
            SDL_ResumeAudioStreamDevice(Audio);
        } else {
            SDL_Log("No sound: %s", SDL_GetError());
        }
    }
    NextVblank = SDL_GetPerformanceCounter();
    return 1;
}


static void wait_until(Uint64 when) {
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 now;

    while ((now = SDL_GetPerformanceCounter()) < when) {
        Uint64 left = (when - now) * 1000 / freq;

        if (left > 1) {
            SDL_Delay((Uint32)(left - 1));
        }
    }
}

void host_wait_vblank(void) {
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 step = (Uint64)(freq / VBLANK_HZ);
    Uint64 now = SDL_GetPerformanceCounter();
    int pictures = settings()->fps > 60 ? settings()->fps / 60 : 1;
    int i;

    /* far behind (a window drag, a breakpoint): start again from now */
    if (now > NextVblank + step + freq / 4) {
        NextVblank = now;
    }
    for (i = 1; i <= pictures; i++) {
        wait_until(NextVblank + step * i / pictures);
        gfx_present();
    }
    NextVblank += step;
}

static void open_pads(void) {
    SDL_JoystickID *ids;
    int i, count = 0;

    for (i = 0; i < MAX_PADS; i++) {
        if (Pads[i] != NULL) {
            SDL_CloseGamepad(Pads[i]);
            Pads[i] = NULL;
        }
    }
    ids = SDL_GetGamepads(&count);
    if (ids != NULL) {
        int n = 0;

        for (i = 0; i < count && n < MAX_PADS; i++) {
            Pads[n] = SDL_OpenGamepad(ids[i]);
            if (Pads[n] != NULL) {
                n++;
            }
        }
        SDL_free(ids);
    }
}

int host_poll(void) {
    SDL_Event e;

    if (ApplyPending) {
        const Settings *s = settings();

        ApplyPending = 0;
        GfxConfig config = gfx_config_from(s);

        place_window();
        gfx_configure(&config);
    }

    int resized = 0;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT:
            settings_save();
            return 0;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            resized = 1;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            /* maximised it's the screen's size, not the window's */
            if (!Fullscreen && settings()->tiles == 0 && !(SDL_GetWindowFlags(Window) & (SDL_WINDOW_MAXIMIZED | SDL_WINDOW_MINIMIZED))) {
                settings()->window_w = e.window.data1;
                settings()->window_h = e.window.data2;
            }
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESTORED:
            if (!Fullscreen && settings()->tiles == 0) {
                settings()->maximized = (SDL_GetWindowFlags(Window) & SDL_WINDOW_MAXIMIZED) != 0;
            }
            break;
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
            open_pads();
            break;
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_MOTION:
        case SDL_EVENT_FINGER_UP:
        case SDL_EVENT_FINGER_CANCELED:
            touch_event(&e);
            break;
        case SDL_EVENT_KEY_DOWN:
            if (e.key.key == SDLK_RETURN && (e.key.mod & SDL_KMOD_ALT) && !e.key.repeat) {
                settings()->fullscreen = !Fullscreen;
                settings_apply();
            }
            break;
        default:
            break;
        }
    }
    if (resized) {
        gfx_window_resized();
    }
    return 1;
}

static int pad_buttons(SDL_Gamepad *c) {
    static const struct {
        SDL_GamepadButton button;
        int bit;
    } map[] = {
        { SDL_GAMEPAD_BUTTON_SOUTH, PAD_CROSS },
        { SDL_GAMEPAD_BUTTON_EAST, PAD_CIRCLE },
        { SDL_GAMEPAD_BUTTON_WEST, PAD_SQUARE },
        { SDL_GAMEPAD_BUTTON_NORTH, PAD_TRIANGLE },
        { SDL_GAMEPAD_BUTTON_START, PAD_START },
        { SDL_GAMEPAD_BUTTON_BACK, PAD_SELECT },
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, PAD_L1 },
        { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, PAD_R1 },
        { SDL_GAMEPAD_BUTTON_DPAD_UP, PAD_UP },
        { SDL_GAMEPAD_BUTTON_DPAD_DOWN, PAD_DOWN },
        { SDL_GAMEPAD_BUTTON_DPAD_LEFT, PAD_LEFT },
        { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, PAD_RIGHT },
    };
    int b = 0;
    size_t i;

    for (i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (SDL_GetGamepadButton(c, map[i].button)) {
            b |= map[i].bit;
        }
    }
    if (SDL_GetGamepadAxis(c, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000) {
        b |= PAD_L2;
    }
    if (SDL_GetGamepadAxis(c, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000) {
        b |= PAD_R2;
    }
    return b;
}

static int keyboard_buttons(void) {
    static const struct {
        SDL_Scancode key;
        int bit;
    } map[] = {
        { SDL_SCANCODE_Z, PAD_CROSS },
        { SDL_SCANCODE_X, PAD_CIRCLE },
        { SDL_SCANCODE_A, PAD_SQUARE },
        { SDL_SCANCODE_S, PAD_TRIANGLE },
        { SDL_SCANCODE_RETURN, PAD_START },
        { SDL_SCANCODE_RSHIFT, PAD_SELECT },
        { SDL_SCANCODE_Q, PAD_L1 },
        { SDL_SCANCODE_W, PAD_R1 },
        { SDL_SCANCODE_1, PAD_L2 },
        { SDL_SCANCODE_2, PAD_R2 },
        { SDL_SCANCODE_UP, PAD_UP },
        { SDL_SCANCODE_DOWN, PAD_DOWN },
        { SDL_SCANCODE_LEFT, PAD_LEFT },
        { SDL_SCANCODE_RIGHT, PAD_RIGHT },
    };
    const bool *keys = SDL_GetKeyboardState(NULL);
    int b = 0;
    size_t i;

    if (keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT]) {
        return 0;
    }
    for (i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (keys[map[i].key]) {
            b |= map[i].bit;
        }
    }
    return b;
}

/* The first pad is the keyboard, the first controller and the touch buttons (touch.c); then a controller each. Using the keyboard or a
   controller hides the touch buttons. Android's back button is Triangle, the game's own way back. */
int host_pad(int port) {
    int b = -1;

    if (port == 0) {
        int other = keyboard_buttons();

        if (port < MAX_PADS && Pads[port] != NULL) {
            other |= pad_buttons(Pads[port]);
        }
        if (other != 0) {
            touch_other_input();
        }
        b = other | touch_buttons();
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_AC_BACK]) {
            b |= PAD_TRIANGLE;
        }
        return b;
    }
    if (port < MAX_PADS && Pads[port] != NULL) {
        b = pad_buttons(Pads[port]);
    }
    return b;
}

static unsigned char stick_of(SDL_Gamepad *c, SDL_GamepadAxis axis) {
    return (unsigned char)((SDL_GetGamepadAxis(c, axis) + 32768) >> 8);
}

int host_pad_sticks(int port, unsigned char sticks[4]) {
    SDL_Gamepad *c = port < MAX_PADS ? Pads[port] : NULL;

    if (c == NULL) {
        return 0;
    }
    sticks[0] = stick_of(c, SDL_GAMEPAD_AXIS_RIGHTX);
    sticks[1] = stick_of(c, SDL_GAMEPAD_AXIS_RIGHTY);
    sticks[2] = stick_of(c, SDL_GAMEPAD_AXIS_LEFTX);
    sticks[3] = stick_of(c, SDL_GAMEPAD_AXIS_LEFTY);
    return 1;
}

int host_pad_rumbles(int port) {
    SDL_Gamepad *c = port < MAX_PADS ? Pads[port] : NULL;

    return c != NULL && SDL_GetBooleanProperty(SDL_GetGamepadProperties(c), SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false);
}

/* The big motor is the low rumble, in strengths, and the small one the high, on or off. */
void host_rumble(int port, int small_on, int big) {
    SDL_Gamepad *c = port < MAX_PADS ? Pads[port] : NULL;

    if (c != NULL) {
        SDL_RumbleGamepad(c, (Uint16)(big * 0x101), small_on ? 0xFFFF : 0, 1000);
    }
}

void host_quit(void) {
    SDL_Quit();
}

/* The window's icon, from w x h colours (red in the low byte, alpha in the top), scaled pixel for pixel so it stays sharp. SDL gives the
   system the one picture, and Windows stretches that to its big and small icons itself, which blurred 16 pixels to the 72 it wants on a
   screen at 225%: so on Windows it's the big icon's own size at this screen's scaling, and elsewhere a big one, which shrinks well.
   Android's windows have none. */
void host_set_icon(const uint32_t *rgba, int w, int h) {
#ifndef SDL_PLATFORM_ANDROID
    SDL_Surface *icon, *big;
    int y, size;

    if (Window == NULL || (icon = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ABGR8888)) == NULL) {
        return;
    }
    for (y = 0; y < h; y++) {
        SDL_memcpy((Uint8 *)icon->pixels + (size_t)y * icon->pitch, rgba + (size_t)y * w, (size_t)w * 4);
    }
#ifdef SDL_PLATFORM_WINDOWS
    size = (int)(32 * SDL_GetWindowDisplayScale(Window) + 0.5f);
#else
    size = 128;
#endif
    big = SDL_ScaleSurface(icon, SDL_max(size, w), SDL_max(size, w) * h / w, SDL_SCALEMODE_NEAREST);
    SDL_SetWindowIcon(Window, big != NULL ? big : icon);
    SDL_DestroySurface(big);
    SDL_DestroySurface(icon);
#else
    (void)rgba;
    (void)w;
    (void)h;
#endif
}
