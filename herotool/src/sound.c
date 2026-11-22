/* The hero tool's sounds (the user's word, 2026-11-22: "Soft audio cues and smooth transitions make routine actions ... feel
 * relaxing"): felt-piano notes made as they play, no files. Each note a triangle and a sine an octave under it, a soft start and a long
 * fall (gone in half a second), low-passed; a cue is a few of them (Hearth's, docs/HERO_PACKS.md, The look). Muted, or with no sound
 * device, nothing plays. */
#include "sound.h"

#include <SDL3/SDL.h>
#include <math.h>

#define RATE 48000
#define VOICES 24

typedef struct {
    float f, to, glide; /* its pitch, the one it slides to over glide seconds (0: none) */
    int wait, left, age; /* samples before it starts, still to play, played */
    float ph, ph2;
} Voice;

static Voice Voices[VOICES];
static SDL_Mutex *Lock;
static SDL_AudioStream *Stream, *Wav;
static int Muted;
static float Low;

static void SDLCALL fill(void *ud, SDL_AudioStream *s, int need, int total) {
    float buf[512];
    int n = need / (int)sizeof(float);

    (void)ud, (void)total;
    while (n > 0) {
        int m = n < 512 ? n : 512;

        SDL_LockMutex(Lock);
        for (int i = 0; i < m; i++) {
            float sum = 0;

            for (int k = 0; k < VOICES; k++) {
                Voice *v = &Voices[k];
                float t, f, env;

                if (v->left <= 0) {
                    continue;
                }
                if (v->wait > 0) {
                    v->wait--;
                    continue;
                }
                t = v->age / (float)RATE;
                f = v->glide > 0 ? v->f * powf(v->to / v->f, fminf(t / v->glide, 1)) : v->f;
                v->ph += f / RATE;
                v->ph -= floorf(v->ph);
                v->ph2 += f * 0.5f / RATE;
                v->ph2 -= floorf(v->ph2);
                env = t < 0.012f ? t / 0.012f : expf(-(t - 0.012f) * 18.4f);
                sum += (4 * fabsf(v->ph - 0.5f) - 1 + 0.35f * sinf(v->ph2 * 6.2831853f)) * env;
                v->age++;
                v->left--;
            }
            /* one pole at about 1100 Hz */
            Low += (sum - Low) * 0.134f;
            buf[i] = Low * 0.09f;
        }
        SDL_UnlockMutex(Lock);
        SDL_PutAudioStreamData(s, buf, m * (int)sizeof(float));
        n -= m;
    }
}

void sound_init(void) {
    SDL_AudioSpec spec = { SDL_AUDIO_F32, 1, RATE };

    if (Stream != NULL || !SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        return;
    }
    Lock = SDL_CreateMutex();
    Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, fill, NULL);
    if (Stream != NULL) {
        SDL_ResumeAudioStreamDevice(Stream);
    }
}

void sound_quit(void) {
    if (Wav != NULL) {
        SDL_DestroyAudioStream(Wav);
        Wav = NULL;
    }
    if (Stream != NULL) {
        SDL_DestroyAudioStream(Stream);
        Stream = NULL;
    }
    if (Lock != NULL) {
        SDL_DestroyMutex(Lock);
        Lock = NULL;
    }
}

void sound_wav(const char *path) {
    SDL_AudioSpec spec;
    Uint8 *buf = NULL;
    Uint32 len = 0;

    if (Muted || Stream == NULL || path == NULL || !SDL_LoadWAV(path, &spec, &buf, &len)) {
        return;
    }
    if (Wav != NULL) {
        SDL_DestroyAudioStream(Wav);
    }
    Wav = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (Wav != NULL) {
        SDL_PutAudioStreamData(Wav, buf, (int)len);
        SDL_FlushAudioStream(Wav);
        SDL_ResumeAudioStreamDevice(Wav);
    }
    SDL_free(buf);
}

void sound_mute(int muted) {
    Muted = muted;
}

int sound_muted(void) {
    return Muted;
}

/* a note: its pitch, when (seconds from now) and what it slides to (0: none) */
static void note(float f, float when, float to) {
    int best = 0;

    for (int k = 0; k < VOICES; k++) {
        if (Voices[k].left <= 0) {
            best = k;
            break;
        }
        if (Voices[k].age > Voices[best].age) {
            best = k;
        }
    }
    Voices[best] = (Voice){ f, to > 0 ? to : f, to > 0 ? 0.2f : 0, (int)(when * RATE), (int)(0.55f * RATE), 0, 0, 0 };
}

void sound_cue(int cue) {
    /* each cue's notes: pitch, when, slid to */
    static const float Cues[CUE_N][5][3] = {
        [CUE_TAP] = { { 523, 0, 0 } },
        [CUE_NAV] = { { 440, 0, 0 } },
        [CUE_ON] = { { 440, 0, 0 }, { 659, 0.08f, 0 } },
        [CUE_OFF] = { { 659, 0, 0 }, { 440, 0.08f, 0 } },
        [CUE_PICK] = { { 587, 0, 0 } },
        [CUE_DROP] = { { 587, 0, 440 } },
        [CUE_OPEN] = { { 494, 0, 0 }, { 587, 0.05f, 0 } },
        [CUE_HIDE] = { { 523, 0, 262 } },
        [CUE_SHOW] = { { 262, 0, 523 } },
        [CUE_PLAY] = { { 392, 0, 0 }, { 587, 0.07f, 0 } },
        [CUE_STOP] = { { 587, 0, 0 }, { 392, 0.07f, 0 } },
        [CUE_SAVE] = { { 392, 0, 0 }, { 494, 0.1f, 0 }, { 587, 0.2f, 0 }, { 784, 0.3f, 0 } },
        [CUE_MADE] = { { 440, 0, 0 }, { 554, 0.09f, 0 }, { 659, 0.18f, 0 } },
        [CUE_VIEW] = { { 659, 0, 523 } },
        [CUE_UP] = { { 523, 0, 587 } },
        [CUE_DOWN] = { { 587, 0, 523 } },
        [CUE_GUESS] = { { 392, 0, 0 }, { 440, 0.06f, 0 }, { 494, 0.12f, 0 }, { 587, 0.18f, 0 }, { 659, 0.24f, 0 } },
    };

    if (Muted || Stream == NULL || cue < 0 || cue >= CUE_N) {
        return;
    }
    SDL_LockMutex(Lock);
    for (int i = 0; i < 5 && Cues[cue][i][0] > 0; i++) {
        note(Cues[cue][i][0], Cues[cue][i][1], Cues[cue][i][2]);
    }
    SDL_UnlockMutex(Lock);
}
