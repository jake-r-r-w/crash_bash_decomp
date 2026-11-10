/* The host on SDL3: a window, sound, the keyboard and game controllers. */
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include "controllers.h"
#include "platform.h"
#include "gfx.h"
#include "settings.h"
#include "touch.h"

#define MAX_PADS 4
#define VBLANK_HZ 59.94
#define OUT_RATE 44100

static SDL_Window *Window;
static SDL_AudioStream *Audio;
static Uint64 NextVblank;
static bool Fullscreen;

/* The controllers, a player's each: slot n is player n + 1's. One that's unplugged leaves its slot empty, so nobody else's player
   changes, and it comes back to it when it's plugged in again; a new one takes the first empty slot, as one plugged into a free port
   on the PlayStation does. The launcher's players screen puts them in another order (controllers.h). */
typedef struct {
    SDL_Gamepad *pad;
    SDL_JoystickID id;
    char key[160]; /* who it is, to know it again: its GUID, name and serial */
    int held;      /* its buttons when last polled */
    int pressed;   /* what it's pressed since that was last asked for */
    int triggers;  /* its triggers pulled past halfway when last polled: bit 0 the left, 1 the right */
} Controller;
static Controller Slots[MAX_PADS];

/* What's been pressed, for a page that's waiting for a key or button to choose (controls.h), and test runs' pretend keys */
static int KeyPressed = -1, ButtonPressed = -1;
static int UsingPad = -1; /* the last press was on a controller (1) or the keyboard (0), or there's been none (-1) */
static bool PretendKeys[SDL_SCANCODE_COUNT];

static void open_pads(void);
static int pad_buttons(SDL_Gamepad *c);
static bool pad_holds(SDL_Gamepad *c, int button);

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

/* --- pretend controllers, for test runs (--pretend-pads N): SDL's virtual gamepads, so they're plugged in, pressed and pulled out as
   real ones are, and a run with them uses no others. N are plugged in at the start; any of the four can come and go later --- */

#define MAX_PRETEND MAX_PADS
static int NumPretend;
static SDL_JoystickID PretendIds[MAX_PRETEND];
static SDL_Joystick *PretendSticks[MAX_PRETEND];

void host_pretend_pads(int count) {
    NumPretend = count < 0 ? 0 : count > MAX_PRETEND ? MAX_PRETEND : count;
    /* SDL drops a controller's presses while the window hasn't the focus, which a test run's never has */
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
}

static void pretend_plug(int n) {
    SDL_VirtualJoystickDesc d;
    char name[32];

    if (n < 0 || n >= MAX_PRETEND || PretendIds[n] != 0) {
        return;
    }
    SDL_snprintf(name, sizeof(name), "Pretend pad %d", n + 1);
    SDL_INIT_INTERFACE(&d);
    d.type = SDL_JOYSTICK_TYPE_GAMEPAD;
    d.vendor_id = 0x1209;
    d.product_id = (Uint16)(0xC0 + n);
    d.naxes = SDL_GAMEPAD_AXIS_COUNT;
    d.nbuttons = SDL_GAMEPAD_BUTTON_COUNT;
    d.button_mask = (1u << SDL_GAMEPAD_BUTTON_COUNT) - 1;
    d.axis_mask = (1u << SDL_GAMEPAD_AXIS_COUNT) - 1;
    d.name = name;
    PretendIds[n] = SDL_AttachVirtualJoystick(&d);
    PretendSticks[n] = PretendIds[n] != 0 ? SDL_OpenJoystick(PretendIds[n]) : NULL;
    if (PretendIds[n] == 0) {
        SDL_Log("pretend pad %d: %s", n + 1, SDL_GetError());
    }
    host_pretend_press(n, 0);
}

/* in or out (--pad-plug, --pad-unplug) */
void host_pretend_plug(int n, int in) {
    if (n < 0 || n >= MAX_PRETEND) {
        return;
    }
    if (in) {
        pretend_plug(n);
    } else if (PretendIds[n] != 0) {
        SDL_CloseJoystick(PretendSticks[n]);
        SDL_DetachVirtualJoystick(PretendIds[n]);
        PretendSticks[n] = NULL;
        PretendIds[n] = 0;
    }
}

/* what its buttons are now (--pad-press): the game's bits, as pad_buttons reads them back */
void host_pretend_press(int n, int buttons) {
    static const struct {
        SDL_GamepadButton button;
        int bit;
    } map[] = {
        { SDL_GAMEPAD_BUTTON_SOUTH, PAD_CROSS },        { SDL_GAMEPAD_BUTTON_EAST, PAD_CIRCLE },
        { SDL_GAMEPAD_BUTTON_WEST, PAD_SQUARE },        { SDL_GAMEPAD_BUTTON_NORTH, PAD_TRIANGLE },
        { SDL_GAMEPAD_BUTTON_START, PAD_START },        { SDL_GAMEPAD_BUTTON_BACK, PAD_SELECT },
        { SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, PAD_L1 },   { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, PAD_R1 },
        { SDL_GAMEPAD_BUTTON_DPAD_UP, PAD_UP },         { SDL_GAMEPAD_BUTTON_DPAD_DOWN, PAD_DOWN },
        { SDL_GAMEPAD_BUTTON_DPAD_LEFT, PAD_LEFT },     { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, PAD_RIGHT },
    };
    size_t i;

    if (n < 0 || n >= MAX_PRETEND || PretendSticks[n] == NULL) {
        return;
    }
    for (i = 0; i < SDL_arraysize(map); i++) {
        SDL_SetJoystickVirtualButton(PretendSticks[n], map[i].button, (buttons & map[i].bit) != 0);
    }
    /* a trigger's at rest at the axis's bottom: its middle is half pulled */
    SDL_SetJoystickVirtualAxis(PretendSticks[n], SDL_GAMEPAD_AXIS_LEFT_TRIGGER, (buttons & PAD_L2) ? SDL_JOYSTICK_AXIS_MAX : SDL_JOYSTICK_AXIS_MIN);
    SDL_SetJoystickVirtualAxis(PretendSticks[n], SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, (buttons & PAD_R2) ? SDL_JOYSTICK_AXIS_MAX : SDL_JOYSTICK_AXIS_MIN);
}

/* --- plugging in and pulling out --- */

static void controller_log(const char *what, const Controller *c, int slot) {
    if (settings()->no_save) {
        printf("pads: %s %s player %d at vblank %d\n", SDL_GetGamepadNameForID(c->id) != NULL ? SDL_GetGamepadNameForID(c->id) : "?",
               what, slot + 1, platform_vblanks());
    }
}

static void plug(SDL_JoystickID id) {
    char key[sizeof(Slots[0].key)];
    SDL_GUID guid = SDL_GetGamepadGUIDForID(id);
    SDL_Gamepad *pad;
    int i, slot = -1;

    if (NumPretend != 0 && !SDL_IsJoystickVirtual(id)) {
        return;
    }
    for (i = 0; i < MAX_PADS; i++) {
        if (Slots[i].pad != NULL && Slots[i].id == id) {
            return;
        }
    }
    pad = SDL_OpenGamepad(id);
    if (pad == NULL) {
        return;
    }
    SDL_GUIDToString(guid, key, sizeof(key));
    SDL_strlcat(key, " ", sizeof(key));
    SDL_strlcat(key, SDL_GetGamepadName(pad) != NULL ? SDL_GetGamepadName(pad) : "", sizeof(key));
    SDL_strlcat(key, " ", sizeof(key));
    SDL_strlcat(key, SDL_GetGamepadSerial(pad) != NULL ? SDL_GetGamepadSerial(pad) : "", sizeof(key));
    /* its own slot if it had one, or the first that's free */
    for (i = 0; i < MAX_PADS && slot < 0; i++) {
        if (Slots[i].pad == NULL && SDL_strcmp(Slots[i].key, key) == 0) {
            slot = i;
        }
    }
    for (i = 0; i < MAX_PADS && slot < 0; i++) {
        if (Slots[i].pad == NULL) {
            slot = i;
        }
    }
    if (slot < 0) {
        SDL_CloseGamepad(pad);
        return;
    }
    Slots[slot].pad = pad;
    Slots[slot].id = id;
    Slots[slot].held = pad_buttons(pad);
    Slots[slot].pressed = 0;
    Slots[slot].triggers = pad_holds(pad, BUTTON_LEFT_TRIGGER) | pad_holds(pad, BUTTON_RIGHT_TRIGGER) << 1;
    SDL_strlcpy(Slots[slot].key, key, sizeof(Slots[slot].key));
    controller_log("is", &Slots[slot], slot);
}

static void unplug(SDL_JoystickID id) {
    int i;

    for (i = 0; i < MAX_PADS; i++) {
        if (Slots[i].pad != NULL && Slots[i].id == id) {
            controller_log("is out: no controller for", &Slots[i], i);
            SDL_CloseGamepad(Slots[i].pad);
            Slots[i].pad = NULL;
            Slots[i].held = Slots[i].pressed = 0;
        }
    }
}

static void open_pads(void) {
    SDL_JoystickID *ids;
    int i, count = 0;

    for (i = 0; i < NumPretend; i++) {
        pretend_plug(i);
    }
    ids = SDL_GetGamepads(&count);
    if (ids != NULL) {
        for (i = 0; i < count; i++) {
            plug(ids[i]);
        }
        SDL_free(ids);
    }
}

/* --- for the launcher's players screen (controllers.h) --- */

int host_controllers(void) {
    int i, n = 0;

    for (i = 0; i < MAX_PADS; i++) {
        n += Slots[i].pad != NULL;
    }
    return n;
}

const char *host_controller_name(int player) {
    if (player < 0 || player >= MAX_PADS || Slots[player].pad == NULL) {
        return NULL;
    }
    return SDL_GetGamepadName(Slots[player].pad) != NULL ? SDL_GetGamepadName(Slots[player].pad) : "CONTROLLER";
}

int host_controller_pressed(int player) {
    int b;

    if (player < 0 || player >= MAX_PADS || Slots[player].pad == NULL) {
        return 0;
    }
    b = Slots[player].pressed;
    Slots[player].pressed = 0;
    return b;
}

void host_controllers_swap(int a, int b) {
    Controller t;

    if (a < 0 || b < 0 || a >= MAX_PADS || b >= MAX_PADS || a == b) {
        return;
    }
    t = Slots[a];
    Slots[a] = Slots[b];
    Slots[b] = t;
    if (Slots[a].pad != NULL) {
        controller_log("is now", &Slots[a], a);
    }
    if (Slots[b].pad != NULL) {
        controller_log("is now", &Slots[b], b);
    }
}

/* Before a game: what's plugged in, in its order, to players 1 on, and the empty slots after, forgetting who they were */
void host_controllers_close_gaps(void) {
    int i, j;

    for (i = 0; i < MAX_PADS; i++) {
        if (Slots[i].pad == NULL) {
            for (j = i + 1; j < MAX_PADS && Slots[j].pad == NULL; j++) {
            }
            if (j < MAX_PADS) {
                host_controllers_swap(i, j);
            }
        }
    }
    for (i = 0; i < MAX_PADS; i++) {
        if (Slots[i].pad == NULL) {
            Slots[i].key[0] = '\0';
        }
    }
}

/* a short buzz, so its player knows which it is */
void host_controller_buzz(int player) {
    if (player >= 0 && player < MAX_PADS && Slots[player].pad != NULL) {
        SDL_RumbleGamepad(Slots[player].pad, 0x5000, 0x5000, 150);
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
            plug(e.gdevice.which);
            break;
        case SDL_EVENT_GAMEPAD_REMOVED:
            unplug(e.gdevice.which);
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
            } else if (!e.key.repeat) {
                KeyPressed = e.key.scancode;
                UsingPad = 0;
            }
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            ButtonPressed = e.gbutton.button;
            UsingPad = 1;
            break;
        default:
            break;
        }
    }
    if (resized) {
        gfx_window_resized();
    }
    for (int i = 0; i < MAX_PADS; i++) {
        if (Slots[i].pad != NULL) {
            int now = pad_buttons(Slots[i].pad);
            int triggers = pad_holds(Slots[i].pad, BUTTON_LEFT_TRIGGER) | pad_holds(Slots[i].pad, BUTTON_RIGHT_TRIGGER) << 1;

            Slots[i].pressed |= now & ~Slots[i].held;
            Slots[i].held = now;
            /* a trigger's an axis, so it's pressed when it's pulled past halfway */
            if (triggers & ~Slots[i].triggers & 1) {
                ButtonPressed = BUTTON_LEFT_TRIGGER;
                UsingPad = 1;
            }
            if (triggers & ~Slots[i].triggers & 2) {
                ButtonPressed = BUTTON_RIGHT_TRIGGER;
                UsingPad = 1;
            }
            Slots[i].triggers = triggers;
        }
    }
    return 1;
}

/* --- the game's buttons, by the player's choice of key and controller button for each (controls.h) --- */

static bool pad_holds(SDL_Gamepad *c, int button) {
    if (button == BUTTON_LEFT_TRIGGER) {
        return SDL_GetGamepadAxis(c, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000;
    }
    if (button == BUTTON_RIGHT_TRIGGER) {
        return SDL_GetGamepadAxis(c, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000;
    }
    return button >= 0 && button < SDL_GAMEPAD_BUTTON_COUNT && SDL_GetGamepadButton(c, (SDL_GamepadButton)button);
}

static int pad_buttons(SDL_Gamepad *c) {
    const int *buttons = settings()->buttons;
    int b = 0, i;

    for (i = 0; i < NUM_CONTROLS; i++) {
        if (pad_holds(c, buttons[i])) {
            b |= control_bit(i);
        }
    }
    return b;
}

static int keyboard_buttons(void) {
    const bool *keys = SDL_GetKeyboardState(NULL);
    const int *bound = settings()->keys;
    int b = 0, i;

    if (keys[SDL_SCANCODE_LALT] || keys[SDL_SCANCODE_RALT]) {
        return 0;
    }
    for (i = 0; i < NUM_CONTROLS; i++) {
        int key = bound[i];

        if (key > SDL_SCANCODE_UNKNOWN && key < SDL_SCANCODE_COUNT && (keys[key] || PretendKeys[key])) {
            b |= control_bit(i);
        }
    }
    return b;
}

int host_key_pressed_any(void) {
    int key = KeyPressed;

    KeyPressed = -1;
    return key;
}

/* With no press yet, a controller if one's plugged in */
int host_using_gamepad(void) {
    return UsingPad >= 0 ? UsingPad : host_controllers() > 0;
}

int host_button_pressed_any(void) {
    int button = ButtonPressed;

    ButtonPressed = -1;
    return button;
}

void host_pretend_key(int key, int down) {
    if (key > SDL_SCANCODE_UNKNOWN && key < SDL_SCANCODE_COUNT) {
        if (down && !PretendKeys[key]) {
            KeyPressed = key;
            UsingPad = 0;
        }
        PretendKeys[key] = down != 0;
    }
}

/* A button's name for the menu, as the first controller plugged in calls it, in 8 letters at most (a column of the CONTROLS page): its
   face buttons by their own letters or shapes, and the rest as Xbox controllers have them, or as a PlayStation's do. With no controller,
   the face buttons by the shapes of a PlayStation's in their places. */
const char *controls_button_label(int button) {
    SDL_Gamepad *first = NULL;
    SDL_GamepadType type = SDL_GAMEPAD_TYPE_STANDARD;
    int ps, xbox, i;

    for (i = 0; i < MAX_PADS && first == NULL; i++) {
        first = Slots[i].pad;
    }
    if (first != NULL) {
        type = SDL_GetGamepadType(first);
    }
    ps = type == SDL_GAMEPAD_TYPE_PS3 || type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5;
    xbox = type == SDL_GAMEPAD_TYPE_XBOX360 || type == SDL_GAMEPAD_TYPE_XBOXONE;
    switch (button) {
    case SDL_GAMEPAD_BUTTON_SOUTH:
    case SDL_GAMEPAD_BUTTON_EAST:
    case SDL_GAMEPAD_BUTTON_WEST:
    case SDL_GAMEPAD_BUTTON_NORTH:
        if (first != NULL) {
            switch (SDL_GetGamepadButtonLabel(first, (SDL_GamepadButton)button)) {
            case SDL_GAMEPAD_BUTTON_LABEL_A: return "A";
            case SDL_GAMEPAD_BUTTON_LABEL_B: return "B";
            case SDL_GAMEPAD_BUTTON_LABEL_X: return "X";
            case SDL_GAMEPAD_BUTTON_LABEL_Y: return "Y";
            case SDL_GAMEPAD_BUTTON_LABEL_CROSS: return "CROSS";
            case SDL_GAMEPAD_BUTTON_LABEL_CIRCLE: return "CIRCLE";
            case SDL_GAMEPAD_BUTTON_LABEL_SQUARE: return "SQUARE";
            case SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE: return "TRIANGLE";
            default: break;
            }
        }
        return button == SDL_GAMEPAD_BUTTON_SOUTH ? "CROSS" : button == SDL_GAMEPAD_BUTTON_EAST ? "CIRCLE"
             : button == SDL_GAMEPAD_BUTTON_WEST  ? "SQUARE" : "TRIANGLE";
    case SDL_GAMEPAD_BUTTON_BACK:
        return type == SDL_GAMEPAD_TYPE_PS5 ? "CREATE" : type == SDL_GAMEPAD_TYPE_PS4 ? "SHARE" : type == SDL_GAMEPAD_TYPE_PS3 ? "SELECT"
             : type == SDL_GAMEPAD_TYPE_XBOXONE ? "VIEW" : "BACK";
    case SDL_GAMEPAD_BUTTON_START:
        return type == SDL_GAMEPAD_TYPE_PS4 || type == SDL_GAMEPAD_TYPE_PS5 ? "OPTIONS" : type == SDL_GAMEPAD_TYPE_XBOXONE ? "MENU" : "START";
    case SDL_GAMEPAD_BUTTON_GUIDE: return ps ? "PS" : xbox ? "XBOX" : "HOME";
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return xbox ? "LS" : "L3";
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return xbox ? "RS" : "R3";
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return xbox ? "LB" : "L1";
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return xbox ? "RB" : "R1";
    case BUTTON_LEFT_TRIGGER: return xbox ? "LT" : "L2";
    case BUTTON_RIGHT_TRIGGER: return xbox ? "RT" : "R2";
    case SDL_GAMEPAD_BUTTON_DPAD_UP: return "D-UP";
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return "D-DOWN";
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return "D-LEFT";
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return "D-RIGHT";
    case SDL_GAMEPAD_BUTTON_TOUCHPAD: return "TOUCHPAD";
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return "PADDLE 1";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return "PADDLE 2";
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return "PADDLE 3";
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return "PADDLE 4";
    default: return "OTHER";
    }
}

/* The first pad is the keyboard, the first controller and the touch buttons (touch.c); then a controller each. Using the keyboard or a
   controller hides the touch buttons. Android's back button is Triangle, the game's own way back. */
int host_pad(int port) {
    int b = -1;

    if (port == 0) {
        int other = keyboard_buttons();

        if (port < MAX_PADS && Slots[port].pad != NULL) {
            other |= pad_buttons(Slots[port].pad);
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
    if (port < MAX_PADS && Slots[port].pad != NULL) {
        b = pad_buttons(Slots[port].pad);
    }
    return b;
}

static unsigned char stick_of(SDL_Gamepad *c, SDL_GamepadAxis axis) {
    return (unsigned char)((SDL_GetGamepadAxis(c, axis) + 32768) >> 8);
}

int host_pad_sticks(int port, unsigned char sticks[4]) {
    SDL_Gamepad *c = port < MAX_PADS ? Slots[port].pad : NULL;

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
    SDL_Gamepad *c = port < MAX_PADS ? Slots[port].pad : NULL;

    return c != NULL && SDL_GetBooleanProperty(SDL_GetGamepadProperties(c), SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN, false);
}

/* The big motor is the low rumble, in strengths, and the small one the high, on or off. */
void host_rumble(int port, int small_on, int big) {
    SDL_Gamepad *c = port < MAX_PADS ? Slots[port].pad : NULL;

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
