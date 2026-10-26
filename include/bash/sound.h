#ifndef BASH_SOUND_H
#define BASH_SOUND_H

/* Functions and data defined in src/main/sound.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/sound.c */

#include "common.h"
#include "bash/main.h"

void func_8002BAE8(void);
void machine_load_sfx(s32 arg0, sfx_machine_bank_info *slot);
void func_8002ADD4(void *data, sfx_machine_bank_info *slot);
void func_8002AAA0(void);
void sound_critical(void);
void machine_quit_sound(void);
void func_8002ABA8(void);
void func_8002AC18(void);
void func_8002AC40(void);
void func_8002AC70(void);
void sound_continue(void);
s32 func_8002B020(VECTOR *v, s32 *range);
s32 func_8002B6D0(VECTOR *pos, View *view);
void func_8002B760(VECTOR *pos, View *view, VECTOR *out, s32 *sxy);
void func_8002B96C(void);
s32 func_8002BB80(void);
/* by sound id; [1] == -1: no sound */
void func_8002B8F4(s32 *voice);
void func_8002ACA0(s32 id, s32 arg1);
void func_8002AEE0(void);
void func_8002AABC(void);
void func_8002BD0C(s32 arg0);
void func_8002BD68(void);
void func_8002BBB4(void);
void func_8002B9A4(void);
void func_8002B4D8(PitchVoice *arg0, s32 arg1);
void func_8002B52C(s16 *arg0, s32 arg1, s32 arg2);
void func_8002BA44(void);
void func_8002BCB0(s32 *arg0);
void func_8002BBEC(void);
s32 func_8002B5B0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
S16Pair func_8002B7C8(s32 x, s32 v);
S16Pair func_8002B210(VECTOR *pos);

static inline void ViewProject(VECTOR *pos, s32 *psxy) {
    VECTOR *out = &D_800569CC.out;
    SVECTOR *in = &D_800569CC.in;
    View *view = (View *)MainCamera;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
}
static inline S16Pair PanVolume(s32 x, s32 v) {
    S16Pair r;
    s32 lo = 0;
    s32 hi = 0x200;

    if (x < lo) {
        r.a = v;
    } else if (x > hi) {
        r.a = 0;
    } else {
        s32 d = 0x40000;
        r.a = ((d - x * x) * v) / d;
    }
    if (0x200 - lo < x) {
        r.b = v;
    } else if (x < 0x200 - hi) {
        r.b = 0;
    } else {
        r.b = ((hi * hi - (0x200 - x) * (0x200 - x)) * v) / (hi * hi - lo);
    }
    return r;
}
static inline s32 ViewVolumeXY(VECTOR *pos, View *view, s32 *psxy) {
    VECTOR *out = &D_800569CC.out;
    SVECTOR *in = &D_800569CC.in;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return func_8002B020(out, D_80064478);
}

#endif /* BASH_SOUND_H */
