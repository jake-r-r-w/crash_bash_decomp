/* The game's sound, played directly from its assets: sound banks (VABs), a sequencer for its music (SEQs)
 * and a mixer. The sound device asks for samples on its own thread, and the music is timed by them, so
 * the music's speed doesn't depend on the frame rate. The game's calls take the lock and change what the
 * mix will do next.
 *
 * A note's volume is libsnd's own sum (seq_volume); pitches are the documented formula. Reverb is the game's own presets in a delay network,
 * with the depth each song asks for. */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libsnd.h>
#include <libspu.h>
#include "platform.h"

#define NUM_BANKS 16
#define NUM_SEQS NUM_BANKS
/* The game counts its effects' voices itself. The music has its own, so neither takes the other's. */
#define FX_VOICES 24
#define NUM_VOICES (FX_VOICES * 2)
#define OUT_RATE 44100
#define SAMPLES_PER_STEP (OUT_RATE / 60)
#define MAX_SAMPLE 256
#define VAB_PROGRAMS_AT 0x20
#define VAB_TONES_AT (VAB_PROGRAMS_AT + 128 * (int)sizeof(ProgAtr))
#define SEQ_HEADER 15

typedef struct {
    int used;
    ProgAtr prog[128];
    VagAtr *tone; /* 16 a program */
    int nprog, nsample;
    int master; /* the bank's own volume */
    uint32_t sample_end[MAX_SAMPLE + 1];
    uint8_t *samples;
    uint32_t samples_size;
} Bank;

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct {
    int state;
    const uint8_t *data;
    uint32_t pos, end, loop;
    uint32_t pitch;
    uint32_t counter;
    int16_t decoded[28];
    int16_t history[4];
    int s1, s2;
    int index;
    int stop_after_block;
    int env, env_wait;
    int vol_l, vol_r;
    int adsr1, adsr2;
    /* what it's playing, for the sequencer's changes */
    int center, shift, bend_down, bend_up;
    int level, pan1, pan2; /* for a note: the bank's, program's and tone's volumes together, and the tone's and program's pans */
    int seq, channel, note, velocity;
    int reverb; /* the tone asks for it */
} Voice;

typedef struct {
    int program;
    int volume, pan;
    int bend;
    int nrpn;
} Channel;

typedef struct {
    int open, playing;
    int bank;
    const uint8_t *data, *pos, *loop_at;
    int loop_left;
    int forever, plays_left;
    int resolution;
    double us_per_quarter, base_us_per_quarter;
    double ticks; /* to go before the next event */
    int status;
    int vol_l, vol_r;
    Channel channel[16];
    /* a fade and a change of speed, in steps of 1/60 s */
    int fade_steps;
    double fade_per_step, fade;
    int speed_steps;
    double speed_per_step, bpm;
} Seq;

static Bank Banks[NUM_BANKS];
static Voice Voices[NUM_VOICES];
static Seq Seqs[NUM_SEQS];
static uint32_t VoiceMask; /* the voices the game's effects still hold: theirs until it lets them go, even once silent */
static int Mute;
static int Mono;
static void (*TickCb)();
static int TickOn;
static int ToNextStep;

static void step_sequencers(void);

/* Reverb. What a preset holds, in the order the game's table has them: delays and places are in 8-byte
 * units of a 16-bit ring that moves on at half the sample rate, volumes are signed fractions. */
enum {
    R_DAPF1, R_DAPF2, R_VIIR, R_VCOMB1, R_VCOMB2, R_VCOMB3, R_VCOMB4, R_VWALL, R_VAPF1, R_VAPF2,
    R_MLSAME, R_MRSAME, R_MLCOMB1, R_MRCOMB1, R_MLCOMB2, R_MRCOMB2, R_DLSAME, R_DRSAME, R_MLDIFF, R_MRDIFF,
    R_MLCOMB3, R_MRCOMB3, R_MLCOMB4, R_MRCOMB4, R_DLDIFF, R_DRDIFF, R_MLAPF1, R_MRAPF1, R_MLAPF2, R_MRAPF2,
    R_VLIN, R_VRIN, NUM_REVERB_VALUES
};

/* By the game's reverb types, from 1: room, three studios, hall, space echo, chaos echo, delay, half echo. */
static const uint16_t ReverbPresets[9][NUM_REVERB_VALUES] = {
    { 0x007D, 0x005B, 0x6D80, 0x54B8, 0xBED0, 0x0000, 0x0000, 0xBA80, 0x5800, 0x5300, 0x04D6, 0x0333, 0x03F0, 0x0227, 0x0374, 0x01EF,
      0x0334, 0x01B5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01B4, 0x0136, 0x00B8, 0x005C, 0x8000, 0x8000 },
    { 0x0033, 0x0025, 0x70F0, 0x4FA8, 0xBCE0, 0x4410, 0xC0F0, 0x9C00, 0x5280, 0x4EC0, 0x03E4, 0x031B, 0x03A4, 0x02AF, 0x0372, 0x0266,
      0x031C, 0x025D, 0x025C, 0x018E, 0x022F, 0x0135, 0x01D2, 0x00B7, 0x018F, 0x00B5, 0x00B4, 0x0080, 0x004C, 0x0026, 0x8000, 0x8000 },
    { 0x00B1, 0x007F, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xB4C0, 0x5280, 0x4EC0, 0x0904, 0x076B, 0x0824, 0x065F, 0x07A2, 0x0616,
      0x076C, 0x05ED, 0x05EC, 0x042E, 0x050F, 0x0305, 0x0462, 0x02B7, 0x042F, 0x0265, 0x0264, 0x01B2, 0x0100, 0x0080, 0x8000, 0x8000 },
    { 0x00E3, 0x00A9, 0x6F60, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0xA680, 0x5680, 0x52C0, 0x0DFB, 0x0B58, 0x0D09, 0x0A3C, 0x0BD9, 0x0973,
      0x0B59, 0x08DA, 0x08D9, 0x05E9, 0x07EC, 0x04B0, 0x06EF, 0x03D2, 0x05EA, 0x031D, 0x031C, 0x0238, 0x0154, 0x00AA, 0x8000, 0x8000 },
    { 0x01A5, 0x0139, 0x6000, 0x5000, 0x4C00, 0xB800, 0xBC00, 0xC000, 0x6000, 0x5C00, 0x15BA, 0x11BB, 0x14C2, 0x10BD, 0x11BC, 0x0DC1,
      0x11C0, 0x0DC3, 0x0DC0, 0x09C1, 0x0BC4, 0x07C1, 0x0A00, 0x06CD, 0x09C2, 0x05C1, 0x05C0, 0x041A, 0x0274, 0x013A, 0x8000, 0x8000 },
    { 0x033D, 0x0231, 0x7E00, 0x5000, 0xB400, 0xB000, 0x4C00, 0xB000, 0x6000, 0x5400, 0x1ED6, 0x1A31, 0x1D14, 0x183B, 0x1BC2, 0x16B2,
      0x1A32, 0x15EF, 0x15EE, 0x1055, 0x1334, 0x0F2D, 0x11F6, 0x0C5D, 0x1056, 0x0AE1, 0x0AE0, 0x07A2, 0x0464, 0x0232, 0x8000, 0x8000 },
    { 0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x8100, 0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
      0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000 },
    { 0x0001, 0x0001, 0x7FFF, 0x7FFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1FFF, 0x0FFF, 0x1005, 0x0005, 0x0000, 0x0000,
      0x1005, 0x0005, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1004, 0x1002, 0x0004, 0x0002, 0x8000, 0x8000 },
    { 0x0017, 0x0013, 0x70F0, 0x4FA8, 0xBCE0, 0x4510, 0xBEF0, 0x8500, 0x5F80, 0x54C0, 0x0371, 0x02AF, 0x02E5, 0x01DF, 0x02B0, 0x01D7,
      0x0358, 0x026A, 0x01D6, 0x011E, 0x012D, 0x00B1, 0x011F, 0x0059, 0x01A0, 0x00E3, 0x0058, 0x0040, 0x0028, 0x0014, 0x8000, 0x8000 },
};

#define REVERB_RING 0x10000
static float ReverbRing[REVERB_RING];
static int ReverbPos;
static const uint16_t *Reverb; /* NULL: off */
static float ReverbDepthL, ReverbDepthR;
/* It runs at half the rate: what's gone in since its last step, and what it last gave out and is giving out now. */
static float ReverbInL, ReverbInR, ReverbOutL[2], ReverbOutR[2];
static int ReverbHalf;

static float reverb_value(int i) {
    return (int16_t)Reverb[i] / 32768.0f;
}

static float *reverb_at(int place) {
    return &ReverbRing[(ReverbPos + place) & (REVERB_RING - 1)];
}

/* A preset's place, in samples of the ring. */
static int reverb_place(int i) {
    return Reverb[i] * 4;
}

static void reverb_store(int place, float v) {
    *reverb_at(place) = v < -32768.0f ? -32768.0f : v > 32767.0f ? 32767.0f : v;
}

/* One side's reflections: from itself through same, from the other side through diff. */
static void reverb_reflect(float in, int same, int d_same, int diff, int d_other) {
    float wall = reverb_value(R_VWALL), iir = reverb_value(R_VIIR);
    float last = *reverb_at(same - 1);

    reverb_store(same, (in + *reverb_at(d_same) * wall - last) * iir + last);
    last = *reverb_at(diff - 1);
    reverb_store(diff, (in + *reverb_at(d_other) * wall - last) * iir + last);
}

static float reverb_side(int comb1, int comb2, int comb3, int comb4, int apf1, int apf2) {
    int d1 = reverb_place(R_DAPF1), d2 = reverb_place(R_DAPF2);
    float a1 = reverb_value(R_VAPF1), a2 = reverb_value(R_VAPF2);
    float out = reverb_value(R_VCOMB1) * *reverb_at(comb1) + reverb_value(R_VCOMB2) * *reverb_at(comb2)
              + reverb_value(R_VCOMB3) * *reverb_at(comb3) + reverb_value(R_VCOMB4) * *reverb_at(comb4);
    float t;

    t = out - a1 * *reverb_at(apf1 - d1);
    out = t * a1 + *reverb_at(apf1 - d1);
    reverb_store(apf1, t);
    t = out - a2 * *reverb_at(apf2 - d2);
    out = t * a2 + *reverb_at(apf2 - d2);
    reverb_store(apf2, t);
    return out;
}

static void reverb_step(float in_l, float in_r, float *out_l, float *out_r) {
    float l = in_l * reverb_value(R_VLIN), r = in_r * reverb_value(R_VRIN);

    reverb_reflect(l, reverb_place(R_MLSAME), reverb_place(R_DLSAME), reverb_place(R_MLDIFF), reverb_place(R_DRDIFF));
    reverb_reflect(r, reverb_place(R_MRSAME), reverb_place(R_DRSAME), reverb_place(R_MRDIFF), reverb_place(R_DLDIFF));
    *out_l = reverb_side(reverb_place(R_MLCOMB1), reverb_place(R_MLCOMB2), reverb_place(R_MLCOMB3), reverb_place(R_MLCOMB4), reverb_place(R_MLAPF1), reverb_place(R_MLAPF2));
    *out_r = reverb_side(reverb_place(R_MRCOMB1), reverb_place(R_MRCOMB2), reverb_place(R_MRCOMB3), reverb_place(R_MRCOMB4), reverb_place(R_MRAPF1), reverb_place(R_MRAPF2));
    ReverbPos = (ReverbPos + 1) & (REVERB_RING - 1);
}

/* A sample of the voices that ask for reverb goes in, and a sample comes out. */
static void reverb_mix(int in_l, int in_r, int *l, int *r) {
    ReverbInL += in_l;
    ReverbInR += in_r;
    if (Reverb == NULL) {
        return;
    }
    if ((ReverbHalf ^= 1) != 0) {
        ReverbOutL[0] = ReverbOutL[1];
        ReverbOutR[0] = ReverbOutR[1];
        reverb_step(ReverbInL * 0.5f, ReverbInR * 0.5f, &ReverbOutL[1], &ReverbOutR[1]);
        ReverbInL = ReverbInR = 0;
        *l += (int)(ReverbOutL[0] * ReverbDepthL);
        *r += (int)(ReverbOutR[0] * ReverbDepthR);
    } else {
        *l += (int)((ReverbOutL[0] + ReverbOutL[1]) * 0.5f * ReverbDepthL);
        *r += (int)((ReverbOutR[0] + ReverbOutR[1]) * 0.5f * ReverbDepthR);
    }
}

/* The samples */

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
        int nibble = (b[2 + i / 2] >> ((i & 1) * 4)) & 15;
        int s = (int)((uint32_t)nibble << 28) >> (16 + shift);

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

/* The envelope, as the hardware steps it, once a sample. */
static void env_step(Voice *v) {
    int exponential, decrease, shift, step_value, target, step, increment;

    switch (v->state) {
    case ENV_ATTACK:
        exponential = (v->adsr1 >> 15) & 1;
        decrease = 0;
        shift = (v->adsr1 >> 10) & 31;
        step_value = (v->adsr1 >> 8) & 3;
        target = 0x7FFF;
        break;
    case ENV_DECAY:
        exponential = 1;
        decrease = 1;
        shift = (v->adsr1 >> 4) & 15;
        step_value = 0;
        target = ((v->adsr1 & 15) + 1) * 0x800;
        if (target > 0x7FFF) {
            target = 0x7FFF;
        }
        break;
    case ENV_SUSTAIN:
        exponential = (v->adsr2 >> 15) & 1;
        decrease = (v->adsr2 >> 14) & 1;
        shift = (v->adsr2 >> 8) & 31;
        step_value = (v->adsr2 >> 6) & 3;
        target = -1;
        break;
    case ENV_RELEASE:
        exponential = (v->adsr2 >> 5) & 1;
        decrease = 1;
        shift = v->adsr2 & 31;
        step_value = 0;
        target = 0;
        break;
    default:
        return;
    }
    step = 7 - step_value;
    if (decrease) {
        step = ~step;
    }
    step *= 1 << (shift < 11 ? 11 - shift : 0);
    increment = 0x8000 >> (shift > 11 ? shift - 11 : 0);
    if (exponential && !decrease && v->env > 0x6000) {
        if (shift < 10) {
            step /= 4;
        } else if (shift >= 11) {
            increment /= 4;
        } else {
            step /= 2;
            increment /= 2;
        }
    } else if (exponential && decrease) {
        step = step * v->env / 0x8000;
    }
    if ((step_value | (shift << 2)) != 0x7F && increment < 1) {
        increment = 1;
    }
    v->env_wait += increment;
    if (!(v->env_wait & 0x8000)) {
        return;
    }
    v->env_wait = 0;
    v->env += step;
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

/* The hardware's interpolation, whose gentle roll-off of the highs is a good part of how the game sounds. */
static const int16_t Gauss[512] = {
    -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001,
    -0x0001, -0x0001, -0x0001, -0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0005,
    0x0005, 0x0006, 0x0007, 0x0007, 0x0008, 0x0009, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E,
    0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0015, 0x0016, 0x0018, 0x0019, 0x001B, 0x001C, 0x001E,
    0x0020, 0x0021, 0x0023, 0x0025, 0x0027, 0x0029, 0x002C, 0x002E, 0x0030, 0x0033, 0x0035, 0x0038,
    0x003A, 0x003D, 0x0040, 0x0043, 0x0046, 0x0049, 0x004D, 0x0050, 0x0054, 0x0057, 0x005B, 0x005F,
    0x0063, 0x0067, 0x006B, 0x006F, 0x0074, 0x0078, 0x007D, 0x0082, 0x0087, 0x008C, 0x0091, 0x0096,
    0x009C, 0x00A1, 0x00A7, 0x00AD, 0x00B3, 0x00BA, 0x00C0, 0x00C7, 0x00CD, 0x00D4, 0x00DB, 0x00E3,
    0x00EA, 0x00F2, 0x00FA, 0x0101, 0x010A, 0x0112, 0x011B, 0x0123, 0x012C, 0x0135, 0x013F, 0x0148,
    0x0152, 0x015C, 0x0166, 0x0171, 0x017B, 0x0186, 0x0191, 0x019C, 0x01A8, 0x01B4, 0x01C0, 0x01CC,
    0x01D9, 0x01E5, 0x01F2, 0x0200, 0x020D, 0x021B, 0x0229, 0x0237, 0x0246, 0x0255, 0x0264, 0x0273,
    0x0283, 0x0293, 0x02A3, 0x02B4, 0x02C4, 0x02D6, 0x02E7, 0x02F9, 0x030B, 0x031D, 0x0330, 0x0343,
    0x0356, 0x036A, 0x037E, 0x0392, 0x03A7, 0x03BC, 0x03D1, 0x03E7, 0x03FC, 0x0413, 0x042A, 0x0441,
    0x0458, 0x0470, 0x0488, 0x04A0, 0x04B9, 0x04D2, 0x04EC, 0x0506, 0x0520, 0x053B, 0x0556, 0x0572,
    0x058E, 0x05AA, 0x05C7, 0x05E4, 0x0601, 0x061F, 0x063E, 0x065C, 0x067C, 0x069B, 0x06BB, 0x06DC,
    0x06FD, 0x071E, 0x0740, 0x0762, 0x0784, 0x07A7, 0x07CB, 0x07EF, 0x0813, 0x0838, 0x085D, 0x0883,
    0x08A9, 0x08D0, 0x08F7, 0x091E, 0x0946, 0x096F, 0x0998, 0x09C1, 0x09EB, 0x0A16, 0x0A40, 0x0A6C,
    0x0A98, 0x0AC4, 0x0AF1, 0x0B1E, 0x0B4C, 0x0B7A, 0x0BA9, 0x0BD8, 0x0C07, 0x0C38, 0x0C68, 0x0C99,
    0x0CCB, 0x0CFD, 0x0D30, 0x0D63, 0x0D97, 0x0DCB, 0x0E00, 0x0E35, 0x0E6B, 0x0EA1, 0x0ED7, 0x0F0F,
    0x0F46, 0x0F7F, 0x0FB7, 0x0FF1, 0x102A, 0x1065, 0x109F, 0x10DB, 0x1116, 0x1153, 0x118F, 0x11CD,
    0x120B, 0x1249, 0x1288, 0x12C7, 0x1307, 0x1347, 0x1388, 0x13C9, 0x140B, 0x144D, 0x1490, 0x14D4,
    0x1517, 0x155C, 0x15A0, 0x15E6, 0x162C, 0x1672, 0x16B9, 0x1700, 0x1747, 0x1790, 0x17D8, 0x1821,
    0x186B, 0x18B5, 0x1900, 0x194B, 0x1996, 0x19E2, 0x1A2E, 0x1A7B, 0x1AC8, 0x1B16, 0x1B64, 0x1BB3,
    0x1C02, 0x1C51, 0x1CA1, 0x1CF1, 0x1D42, 0x1D93, 0x1DE5, 0x1E37, 0x1E89, 0x1EDC, 0x1F2F, 0x1F82,
    0x1FD6, 0x202A, 0x207F, 0x20D4, 0x2129, 0x217F, 0x21D5, 0x222C, 0x2282, 0x22DA, 0x2331, 0x2389,
    0x23E1, 0x2439, 0x2492, 0x24EB, 0x2545, 0x259E, 0x25F8, 0x2653, 0x26AD, 0x2708, 0x2763, 0x27BE,
    0x281A, 0x2876, 0x28D2, 0x292E, 0x298B, 0x29E7, 0x2A44, 0x2AA1, 0x2AFF, 0x2B5C, 0x2BBA, 0x2C18,
    0x2C76, 0x2CD4, 0x2D33, 0x2D91, 0x2DF0, 0x2E4F, 0x2EAE, 0x2F0D, 0x2F6C, 0x2FCC, 0x302B, 0x308B,
    0x30EA, 0x314A, 0x31AA, 0x3209, 0x3269, 0x32C9, 0x3329, 0x3389, 0x33E9, 0x3449, 0x34A9, 0x3509,
    0x3569, 0x35C9, 0x3629, 0x3689, 0x36E8, 0x3748, 0x37A8, 0x3807, 0x3867, 0x38C6, 0x3926, 0x3985,
    0x39E4, 0x3A43, 0x3AA2, 0x3B00, 0x3B5F, 0x3BBD, 0x3C1B, 0x3C79, 0x3CD7, 0x3D35, 0x3D92, 0x3DEF,
    0x3E4C, 0x3EA9, 0x3F05, 0x3F62, 0x3FBD, 0x4019, 0x4074, 0x40D0, 0x412A, 0x4185, 0x41DF, 0x4239,
    0x4292, 0x42EB, 0x4344, 0x439C, 0x43F4, 0x444C, 0x44A3, 0x44FA, 0x4550, 0x45A6, 0x45FC, 0x4651,
    0x46A6, 0x46FA, 0x474E, 0x47A1, 0x47F4, 0x4846, 0x4898, 0x48E9, 0x493A, 0x498A, 0x49D9, 0x4A29,
    0x4A77, 0x4AC5, 0x4B13, 0x4B5F, 0x4BAC, 0x4BF7, 0x4C42, 0x4C8D, 0x4CD7, 0x4D20, 0x4D68, 0x4DB0,
    0x4DF7, 0x4E3E, 0x4E84, 0x4EC9, 0x4F0E, 0x4F52, 0x4F95, 0x4FD7, 0x5019, 0x505A, 0x509A, 0x50DA,
    0x5118, 0x5156, 0x5194, 0x51D0, 0x520C, 0x5247, 0x5281, 0x52BA, 0x52F3, 0x532A, 0x5361, 0x5397,
    0x53CC, 0x5401, 0x5434, 0x5467, 0x5499, 0x54CA, 0x54FA, 0x5529, 0x5558, 0x5585, 0x55B2, 0x55DE,
    0x5609, 0x5632, 0x565B, 0x5684, 0x56AB, 0x56D1, 0x56F6, 0x571B, 0x573E, 0x5761, 0x5782, 0x57A3,
    0x57C3, 0x57E2, 0x57FF, 0x581C, 0x5838, 0x5853, 0x586D, 0x5886, 0x589E, 0x58B5, 0x58CB, 0x58E0,
    0x58F4, 0x5907, 0x5919, 0x592A, 0x593A, 0x5949, 0x5958, 0x5965, 0x5971, 0x597C, 0x5986, 0x598F,
    0x5997, 0x599E, 0x59A4, 0x59A9, 0x59AD, 0x59B0, 0x59B2, 0x59B3,
};

static int next_sample(Voice *v) {
    int i = (v->counter >> 4) & 0xFF;
    int s = ((Gauss[0xFF - i] * v->history[0]) >> 15) + ((Gauss[0x1FF - i] * v->history[1]) >> 15)
          + ((Gauss[0x100 + i] * v->history[2]) >> 15) + ((Gauss[i] * v->history[3]) >> 15);

    return s < -32768 ? -32768 : s > 32767 ? 32767 : s;
}

/* On to the sample after this one. */
static void advance(Voice *v) {
    v->counter += v->pitch;
    while (v->counter >= 0x1000) {
        v->counter -= 0x1000;
        if (v->index >= 28) {
            if (v->stop_after_block) {
                v->state = ENV_OFF;
                v->env = 0;
                return;
            }
            decode_block(v);
            v->index = 0;
            if (v->state == ENV_OFF) {
                return;
            }
        }
        v->history[0] = v->history[1];
        v->history[1] = v->history[2];
        v->history[2] = v->history[3];
        v->history[3] = v->decoded[v->index++];
    }
}

/* Room at the top of the mix, and loud moments rounded off rather than clipped, which sounds harsh. */
#define MIX_LEVEL 0.7f
#define LIMIT_KNEE 22000.0f

static int limit(int sample) {
    float x = sample * MIX_LEVEL;
    float a = x < 0 ? -x : x;

    if (a > LIMIT_KNEE) {
        float room = 32767.0f - LIMIT_KNEE;

        a = LIMIT_KNEE + room * (1.0f - room / (room + a - LIMIT_KNEE));
    }
    return (int)(x < 0 ? -a : a);
}

/* What the sound device, or a test's recording, plays next. Whoever calls it holds the lock. */
void audio_mix(int16_t *out, int frames) {
    int i, j;

    for (i = 0; i < frames; i++) {
        int l = 0, r = 0, wet_l = 0, wet_r = 0;

        if (--ToNextStep <= 0) {
            ToNextStep = SAMPLES_PER_STEP;
            step_sequencers();
        }
        for (j = 0; j < NUM_VOICES; j++) {
            Voice *v = &Voices[j];
            int s;

            if (v->state == ENV_OFF) {
                continue;
            }
            s = (next_sample(v) * v->env) >> 15;
            {
                int vl = (s * v->vol_l) >> 14, vr = (s * v->vol_r) >> 14;

                l += vl;
                r += vr;
                if (v->reverb) {
                    wet_l += vl;
                    wet_r += vr;
                }
            }
            env_step(v);
            if (v->state != ENV_OFF) {
                advance(v);
            }
        }
        reverb_mix(wet_l, wet_r, &l, &r);
        l = limit(l);
        r = limit(r);
        if (Mute) {
            l = r = 0;
        }
        if (Mono) {
            l = r = (l + r) / 2;
        }
        out[i * 2] = (int16_t)(l < -32768 ? -32768 : l > 32767 ? 32767 : l);
        out[i * 2 + 1] = (int16_t)(r < -32768 ? -32768 : r > 32767 ? 32767 : r);
    }
}

/* A test run's recording: a vblank's worth at a time, the same every run. */
static FILE *Recording;
static uint32_t RecordedFrames;
static uint32_t RecordingFrac;

static void write_wav_header(FILE *f, uint32_t frames) {
    uint32_t bytes = frames * 4, u;
    uint16_t h;

    fwrite("RIFF", 1, 4, f);
    u = 36 + bytes;
    fwrite(&u, 4, 1, f);
    fwrite("WAVEfmt ", 1, 8, f);
    u = 16;
    fwrite(&u, 4, 1, f);
    h = 1;
    fwrite(&h, 2, 1, f);
    h = 2;
    fwrite(&h, 2, 1, f);
    u = OUT_RATE;
    fwrite(&u, 4, 1, f);
    u = OUT_RATE * 4;
    fwrite(&u, 4, 1, f);
    h = 4;
    fwrite(&h, 2, 1, f);
    h = 16;
    fwrite(&h, 2, 1, f);
    fwrite("data", 1, 4, f);
    fwrite(&bytes, 4, 1, f);
}

void audio_record(const char *path) {
    Recording = fopen(path, "wb");
    if (Recording != NULL) {
        write_wav_header(Recording, 0);
    }
}

void audio_finish(void) {
    if (Recording != NULL) {
        fseek(Recording, 0, SEEK_SET);
        write_wav_header(Recording, RecordedFrames);
        fclose(Recording);
        Recording = NULL;
    }
}

/* Once a vblank (etc.c): the game's sound tick. */
void snd_vblank(void) {
    if (TickCb != NULL && TickOn) {
        TickCb();
    }
    if (Recording != NULL) {
        static int16_t buf[4096];
        uint32_t n;

        RecordingFrac += (uint32_t)(OUT_RATE * 65536.0 / 59.94);
        n = RecordingFrac >> 16;
        RecordingFrac &= 0xFFFF;
        audio_mix(buf, (int)n);
        fwrite(buf, 4, n, Recording);
        RecordedFrames += n;
    }
}

/* Sound banks: a copy of what's needed, as the game frees its own after the transfer. */

short SsVabOpenHead(unsigned char *addr, short vab) {
    Bank *b;
    uint16_t nprog, nsample, size;
    uint32_t off = 0;
    int i;

    host_audio_lock();
    if (vab < 0) {
        for (i = 0; i < NUM_BANKS; i++) {
            if (!Banks[i].used) {
                vab = (short)i;
                break;
            }
        }
    }
    if (vab < 0 || vab >= NUM_BANKS) {
        host_audio_unlock();
        return -1;
    }
    b = &Banks[vab];
    memcpy(&nprog, addr + 0x12, 2);
    memcpy(&nsample, addr + 0x16, 2);
    b->used = 1;
    b->nprog = nprog;
    b->master = addr[0x18];
    b->nsample = nsample > MAX_SAMPLE ? MAX_SAMPLE : nsample;
    memcpy(b->prog, addr + VAB_PROGRAMS_AT, sizeof(b->prog));
    b->tone = realloc(b->tone, sizeof(VagAtr) * 16 * (nprog ? nprog : 1));
    memcpy(b->tone, addr + VAB_TONES_AT, sizeof(VagAtr) * 16 * nprog);
    /* a size a sample, from slot 1: sample n is from the end of n - 1 to the end of n */
    b->sample_end[0] = 0;
    for (i = 1; i <= b->nsample; i++) {
        memcpy(&size, addr + VAB_TONES_AT + sizeof(VagAtr) * 16 * nprog + i * 2, 2);
        off += (uint32_t)size * 8;
        b->sample_end[i] = off;
    }
    b->samples_size = off;
    host_audio_unlock();
    return vab;
}

short SsVabTransBody(unsigned char *addr, short vab) {
    Bank *b;

    if (vab < 0 || vab >= NUM_BANKS) {
        return -1;
    }
    host_audio_lock();
    b = &Banks[vab];
    b->samples = realloc(b->samples, b->samples_size ? b->samples_size : 1);
    memcpy(b->samples, addr, b->samples_size);
    host_audio_unlock();
    return vab;
}

void SsVabClose(short vab) {
    int i;

    if (vab < 0 || vab >= NUM_BANKS) {
        return;
    }
    host_audio_lock();
    for (i = 0; i < NUM_VOICES; i++) {
        if (Voices[i].state != ENV_OFF && Voices[i].data == Banks[vab].samples) {
            Voices[i].state = ENV_OFF;
        }
    }
    Seqs[vab].open = 0;
    Seqs[vab].playing = 0;
    free(Banks[vab].tone);
    free(Banks[vab].samples);
    memset(&Banks[vab], 0, sizeof(Banks[vab]));
    host_audio_unlock();
}

short SsUtGetProgAtr(short vab, short prog, ProgAtr *atr) {
    if (vab < 0 || vab >= NUM_BANKS || !Banks[vab].used || prog < 0 || prog >= 128) {
        return -1;
    }
    *atr = Banks[vab].prog[prog];
    return 0;
}

short SsUtGetVagAtr(short vab, short prog, short tone, VagAtr *atr) {
    if (vab < 0 || vab >= NUM_BANKS || !Banks[vab].used || prog < 0 || prog >= Banks[vab].nprog || tone < 0 || tone >= 16) {
        return -1;
    }
    *atr = Banks[vab].tone[prog * 16 + tone];
    return 0;
}

/* Voices */

static uint32_t note_pitch(double note, int center, int shift) {
    double p = 4096.0 * pow(2.0, (note - (center + shift / 128.0)) / 12.0);

    return p > 0x3FFF ? 0x3FFF : (uint32_t)p;
}

/* A pan takes from the side it's away from. */
static void apply_pan(int pan, int *left, int *right) {
    if (pan < 64) {
        *right = *right * pan / 63;
    } else {
        *left = *left * (127 - pan) / 63;
    }
}

/* Sets a voice going on a tone of a bank's program. */
static Voice *start_voice(Voice *v, const Bank *b, int prog, int tone) {
    const VagAtr *t = &b->tone[prog * 16 + tone];

    if (t->vag <= 0 || t->vag > b->nsample || b->samples == NULL) {
        return NULL;
    }
    memset(v, 0, sizeof(*v));
    v->data = b->samples;
    v->pos = b->sample_end[t->vag - 1];
    v->loop = v->pos;
    v->end = b->sample_end[t->vag];
    v->index = 28;
    v->center = t->center;
    v->shift = t->shift;
    v->bend_down = t->pbmin;
    v->bend_up = t->pbmax;
    v->adsr1 = t->adsr1;
    v->adsr2 = t->adsr2;
    v->env_wait = 0;
    v->level = (int)((int64_t)b->master * 0x3FFF / 127 * b->prog[prog].mvol / 127 * t->vol / 127);
    v->pan1 = t->pan;
    v->pan2 = b->prog[prog].mpan;
    v->seq = -1;
    v->reverb = (t->mode & 4) != 0;
    v->state = ENV_ATTACK;
    return v;
}

short SsUtKeyOn(short vab, short prog, short tone, short note, short fine, short left, short right) {
    Bank *b;
    Voice *v = NULL;
    int i;

    if (vab < 0 || vab >= NUM_BANKS || !Banks[vab].used || prog < 0 || prog >= Banks[vab].nprog || tone < 0 || tone >= 16) {
        return -1;
    }
    host_audio_lock();
    b = &Banks[vab];
    for (i = 0; i < FX_VOICES && v == NULL; i++) {
        if (Voices[i].state == ENV_OFF && !(VoiceMask & (1u << i))) {
            v = &Voices[i];
        }
    }
    for (i = 0; i < FX_VOICES && v == NULL; i++) {
        if (Voices[i].state == ENV_RELEASE && !(VoiceMask & (1u << i))) {
            v = &Voices[i];
        }
    }
    if (v == NULL || start_voice(v, b, prog, tone) == NULL) {
        host_audio_unlock();
        return -1;
    }
    v->pitch = note_pitch(note + fine / 128.0, v->center, v->shift);
    /* libsnd's sum: the louder side is the note's volume and the other a pan, and it isn't squared */
    {
        int vol = left > right ? left : right;
        int pan = left == right ? 64 : right < left ? (right * 64) / left : 127 - (left * 64) / right;
        int l = (int)((int64_t)v->level * vol / 127), r = l;

        apply_pan(v->pan1, &l, &r);
        apply_pan(v->pan2, &l, &r);
        apply_pan(pan, &l, &r);
        v->vol_l = l;
        v->vol_r = r;
    }
    host_audio_unlock();
    return (short)(v - Voices);
}

short SsUtKeyOffV(short voice) {
    if (voice < 0 || voice >= FX_VOICES) {
        return -1;
    }
    host_audio_lock();
    if (Voices[voice].state != ENV_OFF) {
        Voices[voice].state = ENV_RELEASE;
        Voices[voice].env_wait = 0;
    }
    host_audio_unlock();
    return 0;
}

void SsUtAllKeyOff(short mode) {
    int i;

    host_audio_lock();
    for (i = 0; i < NUM_VOICES; i++) {
        Voices[i].state = ENV_OFF;
    }
    host_audio_unlock();
}

short SsUtChangePitch(short voice, short vab, short prog, short old_note, short old_fine, short new_note, short new_fine) {
    if (voice < 0 || voice >= FX_VOICES) {
        return -1;
    }
    host_audio_lock();
    Voices[voice].pitch = note_pitch(new_note + new_fine / 128.0, Voices[voice].center, Voices[voice].shift);
    host_audio_unlock();
    return 0;
}

short SsUtSetVVol(short voice, short left, short right) {
    if (voice < 0 || voice >= FX_VOICES) {
        return -1;
    }
    host_audio_lock();
    Voices[voice].vol_l = left * 129;
    Voices[voice].vol_r = right * 129;
    host_audio_unlock();
    return 0;
}

void SpuGetAllKeysStatus(char *status) {
    int i;

    host_audio_lock();
    for (i = 0; i < FX_VOICES; i++) {
        int s = Voices[i].state;

        status[i] = s == ENV_OFF ? 0 : s == ENV_RELEASE ? 2 : 1;
    }
    host_audio_unlock();
}

/* The sequencer */

static int read_var(const uint8_t **p) {
    int v = 0;
    int c;

    do {
        c = *(*p)++;
        v = (v << 7) | (c & 0x7F);
    } while (c & 0x80);
    return v;
}

/* libsnd's sum for a sequence's note: everything multiplied, each pan in turn, and the result squared, so
 * that a quiet part is much quieter than its figures look. A key-on from the game's effects isn't squared. */
static void update_voice_volume(Voice *v, const Seq *s, const Channel *c) {
    int level = (int)((int64_t)v->level * (v->velocity * c->volume / 127) / 127);
    int left = (int)(level * s->vol_l * s->fade / 127);
    int right = (int)(level * s->vol_r * s->fade / 127);

    apply_pan(v->pan1, &left, &right);
    apply_pan(v->pan2, &left, &right);
    apply_pan(c->pan, &left, &right);
    v->vol_l = (int)((int64_t)left * left / 0x3FFF);
    v->vol_r = (int)((int64_t)right * right / 0x3FFF);
}

static void update_voice_pitch(Voice *v, const Channel *c) {
    double bend = (c->bend - 0x2000) / 8192.0;
    double semis = bend * (bend < 0 ? v->bend_down : v->bend_up);

    v->pitch = note_pitch(v->note + semis, v->center, v->shift);
}

static Voice *music_voice(void) {
    Voice *best = NULL;
    int i;

    for (i = FX_VOICES; i < NUM_VOICES; i++) {
        if (Voices[i].state == ENV_OFF) {
            return &Voices[i];
        }
    }
    /* the quietest of those on their way out, or of any */
    for (i = FX_VOICES; i < NUM_VOICES; i++) {
        if (Voices[i].state == ENV_RELEASE && (best == NULL || Voices[i].env < best->env)) {
            best = &Voices[i];
        }
    }
    for (i = FX_VOICES; i < NUM_VOICES && best == NULL; i++) {
        best = &Voices[i];
    }
    for (i = FX_VOICES; i < NUM_VOICES && best->state != ENV_RELEASE; i++) {
        if (Voices[i].env < best->env) {
            best = &Voices[i];
        }
    }
    return best;
}

static void note_on(Seq *s, int n, int channel, int note, int velocity) {
    const Bank *b = &Banks[s->bank];
    const Channel *c = &s->channel[channel];
    int prog = c->program;
    int i;

    if (!b->used || prog >= b->nprog) {
        return;
    }
    for (i = 0; i < b->prog[prog].tones && i < 16; i++) {
        const VagAtr *t = &b->tone[prog * 16 + i];

        if (note >= t->min && note <= t->max) {
            Voice *v = start_voice(music_voice(), b, prog, i);

            if (v != NULL) {
                v->seq = n;
                v->channel = channel;
                v->note = note;
                v->velocity = velocity;
                update_voice_pitch(v, c);
                update_voice_volume(v, s, c);
            }
        }
    }
}

static void note_off(int n, int channel, int note) {
    int i;

    for (i = FX_VOICES; i < NUM_VOICES; i++) {
        Voice *v = &Voices[i];

        if (v->state != ENV_OFF && v->state != ENV_RELEASE && v->seq == n && v->channel == channel && v->note == note) {
            v->state = ENV_RELEASE;
            v->env_wait = 0;
        }
    }
}

static void release_seq(int n) {
    int i;

    for (i = FX_VOICES; i < NUM_VOICES; i++) {
        Voice *v = &Voices[i];

        if (v->state != ENV_OFF && v->seq == n) {
            v->state = ENV_RELEASE;
            v->env_wait = 0;
        }
    }
}

/* A channel's volume, pan or bend, for the voices it has sounding. */
static void update_channel(Seq *s, int n, int channel) {
    int i;

    for (i = FX_VOICES; i < NUM_VOICES; i++) {
        Voice *v = &Voices[i];

        if (v->state != ENV_OFF && v->seq == n && v->channel == channel) {
            update_voice_volume(v, s, &s->channel[channel]);
            update_voice_pitch(v, &s->channel[channel]);
        }
    }
}

static void rewind_seq(Seq *s) {
    int i;

    s->pos = s->data + SEQ_HEADER;
    s->status = 0;
    s->loop_at = NULL;
    s->loop_left = 0;
    for (i = 0; i < 16; i++) {
        s->channel[i].program = 0;
        s->channel[i].volume = 127;
        s->channel[i].pan = 64;
        s->channel[i].bend = 0x2000;
        s->channel[i].nrpn = -1;
    }
    s->ticks = read_var(&s->pos);
}

/* A controller. 99 picks what 6 means: 16 is the reverb's depth, and 20 starts a loop, its value the times
 * round (127: for ever). 99 being 30 ends the loop, with no value. */
static void controller(Seq *s, int n, int channel, int number, int value) {
    Channel *c = &s->channel[channel];

    switch (number) {
    case 7:
        c->volume = value;
        update_channel(s, n, channel);
        break;
    case 10:
        c->pan = value;
        update_channel(s, n, channel);
        break;
    case 99:
        c->nrpn = value;
        if (value == 30 && s->loop_at != NULL && s->loop_left > 0) {
            if (s->loop_left != 127) {
                s->loop_left--;
            }
            s->pos = s->loop_at;
        }
        break;
    case 6:
        if (c->nrpn == 16) {
            ReverbDepthL = ReverbDepthR = value / 127.0f;
        } else if (c->nrpn == 20) {
            s->loop_at = s->pos;
            s->loop_left = value;
        }
        c->nrpn = -1;
        break;
    }
}

/* The next event. Returns 0 at the sequence's end. */
static int run_event(Seq *s, int n) {
    int status = *s->pos;
    int channel, a, b;

    if (status < 0x80) {
        status = s->status;
    } else {
        s->pos++;
        s->status = status;
    }
    channel = status & 15;
    switch (status & 0xF0) {
    case 0x80:
        a = *s->pos++;
        s->pos++;
        note_off(n, channel, a);
        break;
    case 0x90:
        a = *s->pos++;
        b = *s->pos++;
        if (b != 0) {
            note_on(s, n, channel, a, b);
        } else {
            note_off(n, channel, a);
        }
        break;
    case 0xB0:
        a = *s->pos++;
        b = *s->pos++;
        controller(s, n, channel, a, b);
        break;
    case 0xC0:
        s->channel[channel].program = *s->pos++;
        break;
    case 0xE0:
        a = *s->pos++;
        b = *s->pos++;
        s->channel[channel].bend = a | (b << 7);
        update_channel(s, n, channel);
        break;
    case 0xF0:
        a = *s->pos++;
        if (a == 0x2F) {
            return 0;
        }
        b = read_var(&s->pos);
        if (a == 0x51 && b == 3) {
            s->us_per_quarter = (s->pos[0] << 16) | (s->pos[1] << 8) | s->pos[2];
            s->bpm = 60000000.0 / s->us_per_quarter;
        }
        s->pos += b;
        break;
    default:
        s->pos += 2;
        break;
    }
    return 1;
}

static void step_seq(Seq *s, int n) {
    if (s->speed_steps > 0) {
        s->speed_steps--;
        s->bpm += s->speed_per_step;
        s->us_per_quarter = 60000000.0 / s->bpm;
    }
    if (s->fade_steps > 0) {
        int i;

        s->fade_steps--;
        s->fade -= s->fade_per_step;
        if (s->fade < 0) {
            s->fade = 0;
        }
        for (i = 0; i < 16; i++) {
            update_channel(s, n, i);
        }
    }
    s->ticks -= s->resolution * (1000000.0 / 60.0) / s->us_per_quarter;
    while (s->ticks <= 0) {
        if (!run_event(s, n)) {
            release_seq(n);
            if (s->forever || --s->plays_left > 0) {
                rewind_seq(s);
                continue;
            }
            s->playing = 0;
            return;
        }
        s->ticks += read_var(&s->pos);
    }
}

static void step_sequencers(void) {
    int i;

    for (i = 0; i < NUM_SEQS; i++) {
        if (Seqs[i].open && Seqs[i].playing) {
            step_seq(&Seqs[i], i);
        }
    }
}

/* The game opens a sequence on the bank it plays from, and uses the bank's number for it. */
short SsSeqOpen(uint32_t *addr, short vab) {
    Seq *s;
    const uint8_t *p = (const uint8_t *)addr;

    if (vab < 0 || vab >= NUM_SEQS || addr == NULL || memcmp(p, "pQES", 4) != 0) {
        return -1;
    }
    host_audio_lock();
    s = &Seqs[vab];
    memset(s, 0, sizeof(*s));
    s->open = 1;
    s->bank = vab;
    s->data = p;
    s->resolution = (p[8] << 8) | p[9];
    s->base_us_per_quarter = s->us_per_quarter = (p[10] << 16) | (p[11] << 8) | p[12];
    s->bpm = 60000000.0 / s->us_per_quarter;
    s->vol_l = s->vol_r = 127;
    s->fade = 1.0;
    rewind_seq(s);
    host_audio_unlock();
    return vab;
}

void SsSeqClose(short seq) {
    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    release_seq(seq);
    Seqs[seq].open = 0;
    Seqs[seq].playing = 0;
    host_audio_unlock();
}

void SsSepStop(short seq, short sep) {
}

void SsSeqPlay(short seq, char mode, short count) {
    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    if (Seqs[seq].open) {
        Seqs[seq].playing = 1;
        Seqs[seq].forever = count == 0;
        Seqs[seq].plays_left = count;
    }
    host_audio_unlock();
}

void SsSeqPause(short seq) {
    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    Seqs[seq].playing = 0;
    release_seq(seq);
    host_audio_unlock();
}

void SsSeqReplay(short seq) {
    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    if (Seqs[seq].open) {
        Seqs[seq].playing = 1;
    }
    host_audio_unlock();
}

void SsSeqSetVol(short seq, short left, short right) {
    int i;

    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    Seqs[seq].vol_l = left;
    Seqs[seq].vol_r = right;
    for (i = 0; i < 16; i++) {
        update_channel(&Seqs[seq], seq, i);
    }
    host_audio_unlock();
}

/* To this speed over this many steps. */
void SsSeqSetAccelerando(short seq, int32_t tempo, int32_t ticks) {
    Seq *s;

    if (seq < 0 || seq >= NUM_SEQS || tempo <= 0) {
        return;
    }
    host_audio_lock();
    s = &Seqs[seq];
    s->speed_steps = ticks > 0 ? ticks : 1;
    s->speed_per_step = (tempo - s->bpm) / s->speed_steps;
    host_audio_unlock();
}

/* Down by this much of 127 over this many steps. */
void SsSeqSetDecrescendo(short seq, short vol, int32_t ticks) {
    Seq *s;

    if (seq < 0 || seq >= NUM_SEQS) {
        return;
    }
    host_audio_lock();
    s = &Seqs[seq];
    s->fade_steps = ticks > 0 ? ticks : 1;
    s->fade_per_step = (vol / 127.0) / s->fade_steps;
    host_audio_unlock();
}

void SsSeqCalledTbyT(void) {
    /* the mix steps the sequences */
}

/* The rest of what the game asks of the library. */

int32_t SpuIsTransferCompleted(int32_t flag) {
    return 1;
}

int32_t SpuReserveReverbWorkArea(int32_t on_off) {
    return 1;
}

void SpuSetCommonAttr(SpuCommonAttr *attr) {
}

/* Starts over: every voice off and every bank closed. The game counts on it: it never closes its music
 * banks. */
void SsInit(void) {
    int i;

    for (i = 0; i < NUM_BANKS; i++) {
        if (Banks[i].used) {
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

void SsSetTickMode(int32_t mode) {
}

void *SsSetTickCallback(void (*cb)()) {
    void (*old)() = TickCb;

    TickCb = cb;
    return (void *)old;
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

void SsSetVoiceMask(uint32_t mask) {
    host_audio_lock();
    VoiceMask = mask;
    host_audio_unlock();
}

void SsUtFlush(void) {
}

void SsUtReverbOn(void) {
}

short SsUtSetReverbType(short type) {
    host_audio_lock();
    Reverb = type >= 1 && type <= 9 ? ReverbPresets[type - 1] : NULL;
    memset(ReverbRing, 0, sizeof(ReverbRing));
    ReverbOutL[0] = ReverbOutL[1] = ReverbOutR[0] = ReverbOutR[1] = 0;
    host_audio_unlock();
    return type;
}

void SsUtSetReverbDepth(short left, short right) {
    host_audio_lock();
    ReverbDepthL = left / 127.0f;
    ReverbDepthR = right / 127.0f;
    host_audio_unlock();
}
