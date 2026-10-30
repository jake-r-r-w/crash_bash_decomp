/* The host on SDL2: a window, sound, the keyboard and game controllers. */
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "platform.h"

#define MAX_PADS 2
#define VBLANK_HZ 59.94
#define OUT_RATE 44100
/* how much sound may wait in the device's queue before more is dropped: 100 ms */
#define MAX_QUEUED (OUT_RATE / 10 * 4)

static SDL_Window *Window;
static SDL_Renderer *Renderer;
static SDL_Texture *Texture;
static int TexW, TexH;
static SDL_AudioDeviceID Audio;
static SDL_GameController *Controllers[MAX_PADS];
static Uint64 NextVblank;
static int Fullscreen;

int host_init(void) {
    SDL_AudioSpec want, got;

    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
        SDL_Log("SDL_Init: %s", SDL_GetError());
        return 0;
    }
    Window = SDL_CreateWindow("Crash Bash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 720, SDL_WINDOW_RESIZABLE);
    if (Window == NULL) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return 0;
    }
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    if (Renderer == NULL) {
        Renderer = SDL_CreateRenderer(Window, -1, 0);
    }
    if (Renderer == NULL) {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return 0;
    }
    /* the PS1 draws to a 4:3 TV whatever the mode's width in pixels */
    SDL_RenderSetLogicalSize(Renderer, 960, 720);
    SDL_memset(&want, 0, sizeof(want));
    want.freq = OUT_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 1024;
    Audio = SDL_OpenAudioDevice(NULL, 0, &want, &got, 0);
    if (Audio != 0) {
        SDL_PauseAudioDevice(Audio, 0);
    }
    NextVblank = SDL_GetPerformanceCounter();
    return 1;
}

void host_present(const uint32_t *pixels, int w, int h) {
    if (Texture == NULL || w != TexW || h != TexH) {
        if (Texture != NULL) {
            SDL_DestroyTexture(Texture);
        }
        Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
        TexW = w;
        TexH = h;
    }
    SDL_UpdateTexture(Texture, NULL, pixels, w * 4);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, Texture, NULL, NULL);
    SDL_RenderPresent(Renderer);
}

void host_wait_vblank(void) {
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 step = (Uint64)(freq / VBLANK_HZ);
    Uint64 now = SDL_GetPerformanceCounter();

    NextVblank += step;
    /* far behind (a window drag, a breakpoint): start again from now */
    if (now > NextVblank + freq / 4) {
        NextVblank = now;
    }
    while ((now = SDL_GetPerformanceCounter()) < NextVblank) {
        Uint64 left = (NextVblank - now) * 1000 / freq;

        if (left > 1) {
            SDL_Delay((Uint32)(left - 1));
        }
    }
}

static void open_controllers(void) {
    int i, n = 0;

    for (i = 0; i < MAX_PADS; i++) {
        if (Controllers[i] != NULL) {
            SDL_GameControllerClose(Controllers[i]);
            Controllers[i] = NULL;
        }
    }
    for (i = 0; i < SDL_NumJoysticks() && n < MAX_PADS; i++) {
        if (SDL_IsGameController(i)) {
            Controllers[n] = SDL_GameControllerOpen(i);
            if (Controllers[n] != NULL) {
                n++;
            }
        }
    }
}

int host_poll(void) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            return 0;
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
            open_controllers();
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_RETURN && (e.key.keysym.mod & KMOD_ALT)) {
                Fullscreen = !Fullscreen;
                SDL_SetWindowFullscreen(Window, Fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            }
            break;
        default:
            break;
        }
    }
    return 1;
}

static int controller_buttons(SDL_GameController *c) {
    static const struct {
        SDL_GameControllerButton button;
        int bit;
    } map[] = {
        { SDL_CONTROLLER_BUTTON_A, PAD_CROSS },
        { SDL_CONTROLLER_BUTTON_B, PAD_CIRCLE },
        { SDL_CONTROLLER_BUTTON_X, PAD_SQUARE },
        { SDL_CONTROLLER_BUTTON_Y, PAD_TRIANGLE },
        { SDL_CONTROLLER_BUTTON_START, PAD_START },
        { SDL_CONTROLLER_BUTTON_BACK, PAD_SELECT },
        { SDL_CONTROLLER_BUTTON_LEFTSHOULDER, PAD_L1 },
        { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, PAD_R1 },
        { SDL_CONTROLLER_BUTTON_DPAD_UP, PAD_UP },
        { SDL_CONTROLLER_BUTTON_DPAD_DOWN, PAD_DOWN },
        { SDL_CONTROLLER_BUTTON_DPAD_LEFT, PAD_LEFT },
        { SDL_CONTROLLER_BUTTON_DPAD_RIGHT, PAD_RIGHT },
    };
    int b = 0;
    size_t i;

    for (i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (SDL_GameControllerGetButton(c, map[i].button)) {
            b |= map[i].bit;
        }
    }
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 16000) {
        b |= PAD_L2;
    }
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 16000) {
        b |= PAD_R2;
    }
    /* the left stick works as the d-pad */
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX) < -16000) {
        b |= PAD_LEFT;
    }
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTX) > 16000) {
        b |= PAD_RIGHT;
    }
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY) < -16000) {
        b |= PAD_UP;
    }
    if (SDL_GameControllerGetAxis(c, SDL_CONTROLLER_AXIS_LEFTY) > 16000) {
        b |= PAD_DOWN;
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
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
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

/* Port 1 has the keyboard and the first controller, port 2 the second controller. */
int host_pad(int port) {
    int b = -1;

    if (port == 0) {
        b = keyboard_buttons();
    }
    if (port < MAX_PADS && Controllers[port] != NULL) {
        b = (b < 0 ? 0 : b) | controller_buttons(Controllers[port]);
    }
    return b;
}

void host_audio(const int16_t *stereo, int frames) {
    if (Audio != 0 && SDL_GetQueuedAudioSize(Audio) < MAX_QUEUED) {
        SDL_QueueAudio(Audio, stereo, (Uint32)frames * 4);
    }
}

void host_quit(void) {
    SDL_Quit();
}
