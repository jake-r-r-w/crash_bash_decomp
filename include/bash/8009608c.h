#ifndef BASH_8009608C_H
#define BASH_8009608C_H

/* Functions and data defined in src/overlays/engine/8009608c.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8009608c.c */

#include "common.h"
#include "bash/engine.h"

void func_80096F98(s32 arg0);

void func_80097158(u8 *p);

s32 func_800974EC(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2);

s32 func_800976A8(u8 *arg0, s32 arg1);

void func_80098278(void);

s32 func_800967E0(D_800AD2EC_t *a, s32 *b, D_800AD2EC_t *c, s32 *d, s32 *out);
void func_80096A08(void);
void func_80078DD4(void);

void func_8009608C(s32 arg0);

/* D_80051640 as the engine camera (func_8009608C). */
typedef struct EngCam {
    /* 0x00 */ CamXYZ eye;
    /* 0x0C */ CamXYZ at;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x38 - 0x1C];
    /* 0x38 */ s32 shakeTime;
    /* 0x3C */ s32 shakeAmp;
    /* 0x40 */ u8 pad40[0x54 - 0x40];
    /* 0x54 */ SVECTOR rot;
} EngCam;

s32 func_80096280(s32 *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);

/* func_80097608's body, inlined by func_800967E0. */
static inline s32 EngCam_HitY(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2) {
    VECTOR sp18;
    VECTOR sp28;
    s32 d = 0x2D0000 / ((EngCam *)D_80051640)->dist;
    extern s32 line_intersect_zy(VECTOR *, VECTOR *, VECTOR *, VECTOR *, VECTOR *);

    sp18.vy = arg0->vy + d;
    sp18.vz = arg0->vz - 0x3000;
    sp28.vy = arg1->vy - d;
    sp28.vz = arg1->vz - 0x3000;
    return line_intersect_zy(arg0, &sp18, arg1, &sp28, arg2) != 0;
}

/* func_800974EC's body, inlined by func_800967E0. */
static inline s32 EngCam_HitX(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2) {
    s32 sp18[4];
    s32 sp28[4];
    s32 d = 0x3C0000 / ((EngCam *)D_80051640)->dist;

    sp18[0] = arg0->a + d;
    sp18[2] = arg0->c - 0x3000;
    sp28[0] = arg1->a - d;
    sp28[2] = arg1->c - 0x3000;
    if (func_80096280(arg0, sp18, arg1, sp28, arg2) != 0) {
        return 1;
    }
    if (arg0->c < arg1->c) {
        *arg2 = *arg0;
    } else {
        *arg2 = *arg1;
    }
    arg2->c -= 0x600;
    return 1;
}

extern CamEase D_800AD0F4;
void func_80097AA4(subengine *s);

void TitlesCameraHook(u8 *cam, s32 arg1);
void func_800B8098(u8 *cam, s32 arg1);
void func_800B96F4(CamView *out, s32 arg1);
void func_800BB184(u8 *cam);

void func_80098344(void);

void func_80098570(void);

void camera_count_bottom(s32 arg0, s32 arg1);

void func_80097334(s32 arg0, s32 arg1);

void anim_item(void);

void func_80097244(u8 *p);

void func_800973AC(s32 arg0, s32 arg1);

void func_80097424(s32 arg0, s32 arg1);

void camera_add_frame_list(SLONG_XYZ *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);

void func_80098284(void);

void func_800982E0(void);

void func_80098464(void);

s32 func_80097608(VECTOR *arg0, VECTOR *arg1, s32 arg2);

void func_800984D4(void);

/* 0x24-byte engine records handed to func_80095BEC (func_800976F8). */
typedef struct EngRec24 {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 pad20[4];
} EngRec24;

/* D_8005A614 as seen by func_800976F8. */
typedef struct EngRecView {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x48 - 0x36];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ u8 pad4C[4];
    /* 0x50 */ u8 unk50[4];
    /* 0x54 */ u8 pad54[0x6C - 0x54];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[0xCC - 0x70];
    /* 0xCC */ u8 unkCC[4];
} EngRecView;

/* Read through a cast, not a local: sched then sees the base as the symbol and
 * moves these loads past the record stores, as the target does. */
#define GV ((EngRecView *)D_8005A614)

extern u16 D_8009922C[];
extern EngRec24 D_800AD32C[];
extern EngRec24 D_800AD5D8[];
extern EngRec24 D_800AD7AC[];
extern EngRec24 D_800AD8CC[];
extern EngRec24 D_800AD9C8[];
extern EngRec24 D_800ADB54[];

void func_800976F8(void);

#endif /* BASH_8009608C_H */
