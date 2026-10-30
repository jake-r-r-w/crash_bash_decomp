/* libsnd's voice side and the SPU on PC (ROADMAP-B 6.2): sound banks, sound effects and a 24-voice
 * ADPCM mixer. The volume and pitch curves are the documented formulas, close to but not exactly
 * libsnd's. Not yet: the sequencer (6.4, the SsSeq functions below do nothing) and reverb. */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libsnd.h>
#include <libspu.h>
#include "../platform/platform.h"

#define NVAB 16
#define NVOICE 24
#define MAX_VAG 256
#define OUT_RATE 44100
/* samples per vblank at 59.94 Hz, as a 16.16 fraction */
#define SAMPLES_PER_VBLANK ((uint32_t)(OUT_RATE * 65536.0 / 59.94))

typedef struct {
    int used;
    ProgAtr prog[128];
    VagAtr *vag; /* 16 per program */
    int nprog;
    uint32_t vag_offs[MAX_VAG + 1];
    int nvag;
    uint8_t *body;
    uint32_t body_size;
} Vab;

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct {
    int state;
    const uint8_t *data;
    uint32_t pos, end, loop;
    uint32_t pitch;
    uint32_t counter;
    int16_t decoded[28];
    int16_t hist[4];
    int s1, s2;
    int index;
    int stop_after_block;
    int env;
    int env_wait;
    int vol_l, vol_r;
    int adsr1, adsr2;
    int center, shift;
    int base_vol;
    int pan;
} Voice;

static Vab Vabs[NVAB];
static Voice Voices[NVOICE];
static int Mute;
static int Mono;
static void (*TickCb)();
static int TickOn;
static uint32_t OutFrac;
static int16_t OutBuf[4096];

/* ADPCM */

static void decode_block(Voice *v) {
    static const int f0[5] = { 0, 60, 115, 98, 122 };
    static const int f1[5] = { 0, 0, -52, -55, -60 };
    const uint8_t *b;
    int shift, filter, flags, i;

    if (v->pos + 16 > v->end) {
        v->state = ENV_OFF;
        return;
    }
    b = v->data + v->pos;
    shift = b[0] & 15;
    filter = (b[0] >> 4) & 7;
    flags = b[1];
    if (shift > 12) {
        shift = 9;
    }
    if (filter > 4) {
        filter = 4;
    }
    if (flags & 4) {
        v->loop = v->pos;
    }
    for (i = 0; i < 28; i++) {
        int nib = (b[2 + i / 2] >> ((i & 1) * 4)) & 15;
        int s = (int)((uint32_t)nib << 28) >> (16 + shift);

        s += (v->s1 * f0[filter] + v->s2 * f1[filter] + 32) >> 6;
        s = s < -32768 ? -32768 : s > 32767 ? 32767 : s;
        v->s2 = v->s1;
        v->s1 = s;
        v->decoded[i] = (int16_t)s;
    }
    v->pos += 16;
    v->stop_after_block = 0;
    if (flags & 1) {
        if (flags & 2) {
            v->pos = v->loop;
        } else {
            v->stop_after_block = 1;
        }
    }
}

/* The envelope (SPU documented rates): one step per output sample at most. */
static void env_step(Voice *v) {
    int mode_exp, decrease, shift, step, target;
    int cycles, delta;

    switch (v->state) {
    case ENV_ATTACK:
        mode_exp = (v->adsr1 >> 15) & 1;
        decrease = 0;
        shift = (v->adsr1 >> 10) & 31;
        step = 7 - ((v->adsr1 >> 8) & 3);
        target = 0x7FFF;
        break;
    case ENV_DECAY:
        mode_exp = 1;
        decrease = 1;
        shift = (v->adsr1 >> 4) & 15;
        step = -8;
        target = (((v->adsr1 & 15) + 1) * 0x800);
        if (target > 0x7FFF) {
            target = 0x7FFF;
        }
        break;
    case ENV_SUSTAIN:
        mode_exp = (v->adsr2 >> 15) & 1;
        decrease = (v->adsr2 >> 14) & 1;
        shift = (v->adsr2 >> 8) & 31;
        step = decrease ? -8 + ((v->adsr2 >> 6) & 3) : 7 - ((v->adsr2 >> 6) & 3);
        target = -1;
        break;
    case ENV_RELEASE:
        mode_exp = (v->adsr2 >> 5) & 1;
        decrease = 1;
        shift = v->adsr2 & 31;
        step = -8;
        target = 0;
        break;
    default:
        return;
    }
    if (--v->env_wait > 0) {
        return;
    }
    cycles = 1 << (shift > 11 ? shift - 11 : 0);
    delta = step << (shift < 11 ? 11 - shift : 0);
    if (mode_exp && !decrease && v->env > 0x6000) {
        cycles *= 4;
    }
    if (mode_exp && decrease) {
        delta = (delta * v->env) / 0x8000;
    }
    v->env_wait = cycles;
    v->env += delta;
    if (v->env > 0x7FFF) {
        v->env = 0x7FFF;
    }
    if (v->env < 0) {
        v->env = 0;
    }
    if (v->state == ENV_ATTACK && v->env >= 0x7FFF) {
        v->state = ENV_DECAY;
    } else if (v->state == ENV_DECAY && v->env <= target) {
        v->state = ENV_SUSTAIN;
    } else if (v->state == ENV_RELEASE && v->env <= 0) {
        v->state = ENV_OFF;
    }
}

static void mix(int16_t *out, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        int l = 0, r = 0;

        for (j = 0; j < NVOICE; j++) {
            Voice *v = &Voices[j];
            int a, b, c, d, t, s;

            if (v->state == ENV_OFF) {
                continue;
            }
            /* four-point interpolation over the last samples */
            t = (v->counter >> 3) & 0x1FF;
            a = v->hist[0];
            b = v->hist[1];
            c = v->hist[2];
            d = v->hist[3];
            {
                int tt = (t * t) >> 9;
                int ttt = (tt * t) >> 9;

                s = (b * 512 + ((c - a) * t) / 2 + ((2 * a - 5 * b + 4 * c - d) * tt) / 2 +
                     ((3 * (b - c) + d - a) * ttt) / 2) >> 9;
            }
            s = s < -32768 ? -32768 : s > 32767 ? 32767 : s;
            s = (s * v->env) >> 15;
            l += (s * v->vol_l) >> 14;
            r += (s * v->vol_r) >> 14;
            env_step(v);
            if (v->state == ENV_OFF) {
                continue;
            }
            v->counter += v->pitch;
            while (v->counter >= 0x1000) {
                v->counter -= 0x1000;
                if (v->index >= 28) {
                    if (v->stop_after_block) {
                        v->state = ENV_OFF;
                        v->env = 0;
                        break;
                    }
                    decode_block(v);
                    v->index = 0;
                    if (v->state == ENV_OFF) {
                        break;
                    }
                }
                v->hist[0] = v->hist[1];
                v->hist[1] = v->hist[2];
                v->hist[2] = v->hist[3];
                v->hist[3] = v->decoded[v->index++];
            }
        }
        /* the game's master volume, xa_setvol's 0x37FF */
        l = (l * 0x37FF) >> 14;
        r = (r * 0x37FF) >> 14;
        if (Mute) {
            l = r = 0;
        }
        if (Mono) {
            l = r = (l + r) / 2;
        }
        l = l < -32768 ? -32768 : l > 32767 ? 32767 : l;
        r = r < -32768 ? -32768 : r > 32767 ? 32767 : r;
        out[i * 2] = (int16_t)l;
        out[i * 2 + 1] = (int16_t)r;
    }
}

/* Once per vblank (etc.c): the sound tick, and a vblank's worth of audio. */
void snd_vblank(void) {
    uint32_t n;

    if (TickCb != NULL && TickOn) {
        TickCb();
    }
    OutFrac += SAMPLES_PER_VBLANK;
    n = OutFrac >> 16;
    OutFrac &= 0xFFFF;
    if (n > 2048) {
        n = 2048;
    }
    mix(OutBuf, (int)n);
    platform_audio(OutBuf, (int)n);
}

/* Sound banks: a copy of the head and of the samples, as the game frees its own after the transfer. */

static const uint8_t *vab_rd(const uint8_t *p, void *out, size_t n) {
    memcpy(out, p, n);
    return p + n;
}

short SsVabOpenHead(unsigned char *addr, short vab) {
    Vab *b;
    const uint8_t *p = addr;
    uint16_t ps, ts, vs;
    uint16_t sizes[256];
    uint32_t off = 0;
    int i;

    if (vab < 0) {
        for (i = 0; i < NVAB; i++) {
            if (!Vabs[i].used) {
                vab = (short)i;
                break;
            }
        }
        if (vab < 0) {
            return -1;
        }
    }
    b = &Vabs[vab];
    memcpy(&ps, p + 0x12, 2);
    memcpy(&ts, p + 0x14, 2);
    memcpy(&vs, p + 0x16, 2);
    b->used = 1;
    b->nprog = ps;
    b->nvag = vs;
    p = addr + 0x20;
    p = vab_rd(p, b->prog, sizeof(b->prog));
    b->vag = realloc(b->vag, sizeof(VagAtr) * 16 * (ps ? ps : 1));
    p = vab_rd(p, b->vag, sizeof(VagAtr) * 16 * ps);
    vab_rd(p, sizes, sizeof(sizes));
    /* the table holds a size per sample, from entry 1; what's past them is junk */
    for (i = 0; i <= MAX_VAG; i++) {
        if (i < 256 && i <= vs) {
            off += (uint32_t)sizes[i] * 8;
        }
        b->vag_offs[i] = off;
    }
    b->body_size = off;
    return vab;
}

short SsVabTransBody(unsigned char *addr, short vab) {
    Vab *b;

    if (vab < 0 || vab >= NVAB) {
        return -1;
    }
    b = &Vabs[vab];
    b->body = realloc(b->body, b->body_size ? b->body_size : 1);
    memcpy(b->body, addr, b->body_size);
    return vab;
}

void SsVabClose(short vab) {
    int i;

    if (vab < 0 || vab >= NVAB) {
        return;
    }
    for (i = 0; i < NVOICE; i++) {
        if (Voices[i].state != ENV_OFF && Voices[i].data == Vabs[vab].body) {
            Voices[i].state = ENV_OFF;
        }
    }
    free(Vabs[vab].vag);
    free(Vabs[vab].body);
    memset(&Vabs[vab], 0, sizeof(Vabs[vab]));
}

short SsUtGetProgAtr(short vab, short prog, ProgAtr *atr) {
    if (vab < 0 || vab >= NVAB || !Vabs[vab].used || prog < 0 || prog >= 128) {
        return -1;
    }
    *atr = Vabs[vab].prog[prog];
    return 0;
}

short SsUtGetVagAtr(short vab, short prog, short tone, VagAtr *atr) {
    if (vab < 0 || vab >= NVAB || !Vabs[vab].used || prog < 0 || prog >= Vabs[vab].nprog || tone < 0 || tone >= 16) {
        return -1;
    }
    *atr = Vabs[vab].vag[prog * 16 + tone];
    return 0;
}

static uint32_t note_pitch(int note, int fine, int center, int shift) {
    double semis = (note + fine / 128.0) - (center + shift / 128.0);
    double p = 4096.0 * pow(2.0, semis / 12.0);

    return p > 0x3FFF ? 0x3FFF : (uint32_t)p;
}

/* left and right are 0..127, and a tone's own volume and pan scale them; the SPU's voice volume is
 * 0..0x3FFF (the mixer: 0x4000 is 100%). */
static void set_volume(Voice *v, int left, int right) {
    int pl = v->pan > 64 ? 127 - v->pan : 63;
    int pr = v->pan < 64 ? v->pan : 63;

    v->vol_l = (left * v->base_vol * pl / 63) * 0x3FFF / (127 * 127);
    v->vol_r = (right * v->base_vol * pr / 63) * 0x3FFF / (127 * 127);
}

short SsUtKeyOn(short vab, short prog, short tone, short note, short fine, short left, short right) {
    Vab *b;
    const VagAtr *t;
    Voice *v;
    int i;
    uint32_t start, end;

    if (vab < 0 || vab >= NVAB || !Vabs[vab].used || Vabs[vab].body == NULL || prog < 0 || prog >= Vabs[vab].nprog ||
        tone < 0 || tone >= 16) {
        return -1;
    }
    b = &Vabs[vab];
    t = &b->vag[prog * 16 + tone];
    if (t->vag <= 0 || t->vag > MAX_VAG) {
        return -1;
    }
    for (i = 0; i < NVOICE; i++) {
        if (Voices[i].state == ENV_OFF) {
            break;
        }
    }
    if (i == NVOICE) {
        for (i = 0; i < NVOICE; i++) {
            if (Voices[i].state == ENV_RELEASE) {
                break;
            }
        }
        if (i == NVOICE) {
            return -1;
        }
    }
    v = &Voices[i];
    start = t->vag >= 2 ? b->vag_offs[t->vag - 2] : 0;
    end = b->vag_offs[t->vag - 1];
    memset(v, 0, sizeof(*v));
    v->data = b->body;
    v->pos = start;
    v->loop = start;
    v->end = end;
    v->index = 28;
    v->center = t->center;
    v->shift = t->shift;
    v->pitch = note_pitch(note, fine, t->center, t->shift);
    v->adsr1 = t->adsr1;
    v->adsr2 = t->adsr2;
    v->env_wait = 1;
    v->base_vol = (t->vol * b->prog[prog].mvol) / 127;
    v->pan = t->pan;
    set_volume(v, left, right);
    v->state = ENV_ATTACK;
    return (short)i;
}

short SsUtKeyOffV(short voice) {
    if (voice < 0 || voice >= NVOICE) {
        return -1;
    }
    if (Voices[voice].state != ENV_OFF) {
        Voices[voice].state = ENV_RELEASE;
        Voices[voice].env_wait = 1;
    }
    return 0;
}

void SsUtAllKeyOff(short mode) {
    int i;

    for (i = 0; i < NVOICE; i++) {
        Voices[i].state = ENV_OFF;
    }
}

short SsUtChangePitch(short voice, short vab, short prog, short old_note, short old_fine, short new_note, short new_fine) {
    if (voice < 0 || voice >= NVOICE) {
        return -1;
    }
    Voices[voice].pitch = note_pitch(new_note, new_fine, Voices[voice].center, Voices[voice].shift);
    return 0;
}

short SsUtSetVVol(short voice, short left, short right) {
    if (voice < 0 || voice >= NVOICE) {
        return -1;
    }
    set_volume(&Voices[voice], left, right);
    return 0;
}

void SpuGetAllKeysStatus(char *status) {
    int i;

    for (i = 0; i < NVOICE; i++) {
        int s = Voices[i].state;

        status[i] = s == ENV_OFF ? 0 : s == ENV_RELEASE ? 2 : 1;
    }
}

long SpuIsTransferCompleted(long flag) {
    return 1;
}

long SpuReserveReverbWorkArea(long on_off) {
    return 1;
}

void SpuSetCommonAttr(SpuCommonAttr *attr) {
}

/* Starts over: every voice off and every bank closed. The game counts on it: it never closes its music
 * banks. */
void SsInit(void) {
    int i;

    for (i = 0; i < NVAB; i++) {
        if (Vabs[i].used) {
            SsVabClose((short)i);
        }
    }
    SsUtAllKeyOff(0);
}
void SsStart(void) {
    TickOn = 1;
}
void SsEnd(void) {
    TickOn = 0;
}
void SsQuit(void) {
    TickOn = 0;
}
void SsSetTableSize(char *table, short s_max, short t_max) {
}
void SsSetTickMode(long mode) {
}
int SsSetTickCallback(void (*cb)()) {
    void (*old)() = TickCb;

    TickCb = cb;
    return (int)(intptr_t)old;
}
void SsSetMVol(short left, short right) {
}
void SsSetMono(void) {
    Mono = 1;
}
void SsSetStereo(void) {
    Mono = 0;
}
void SsSetMute(char mode) {
    Mute = mode;
}
void SsSetVoiceMask(unsigned long mask) {
}
void SsUtFlush(void) {
}
void SsUtReverbOn(void) {
}
short SsUtSetReverbType(short type) {
    return type;
}
void SsUtSetReverbDepth(short left, short right) {
}

/* The sequencer: music (ROADMAP-B 6.4). */
void SsSeqCalledTbyT(void) {
}
short SsSeqOpen(unsigned long *addr, short vab) {
    return 0;
}
void SsSeqClose(short seq) {
}
void SsSeqPlay(short seq, char mode, short count) {
}
void SsSeqPause(short seq) {
}
void SsSeqReplay(short seq) {
}
void SsSeqSetVol(short seq, short left, short right) {
}
void SsSeqSetAccelerando(short seq, long tempo, long ticks) {
}
void SsSeqSetDecrescendo(short seq, short vol, long ticks) {
}
void SsSepStop(short seq, short sep) {
}
