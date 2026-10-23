#ifndef BASH_SCRIPTS_H
#define BASH_SCRIPTS_H

/* Functions and data defined in src/main/scripts.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/scripts.c */

#include "common.h"
#include "bash/80017164.h"

void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void func_8002141C(u8 *arg0);
void render_entity_thread(script_thread_handler *th, geom_info *gi);
void func_80021990(u8 *arg0, s32 *arg1);
void func_80021C20(u8 *arg0, u8 *arg1, u8 *arg2, s32 arg3, s32 arg4);
void func_80021ED8(u8 *arg0);
void func_80021F14(u8 *arg0);
u8 *func_80021F6C(void);
void func_80021FB0(void);
void func_80022030(void);
void scripts_update(void);
void func_8001F604(u8 *obj, EmitPart *part, u8 *def);
void func_80021330(u8 *arg0, s32 *arg1);
u8 *func_80021E44(OffsetTable *tbl, s32 i);
void *func_80021EEC(void);
void func_8002120C(u8 *arg0);
u8 *get_script_header(obj_dbase *db, s32 arg1);
u8 *func_80021E98(u8 *arg0);
void func_80021940(s32 arg0, u8 *arg1, s32 arg2);
void func_80021238(u8 *arg0, s32 *arg1);
void func_80021B88(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void func_80021AF0(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void func_80021708(u8 *arg0, u8 *arg1, s32 arg2);
void func_80020B44(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void func_80020BA8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
s16 func_80020C4C(s32 arg0);
s32 func_800213A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 *func_80021878(u8 *arg0, s32 arg1);
s32 *func_800218DC(u8 *arg0, s32 arg1);
s32 *func_800219B8(u8 *arg0, s32 arg1);
u8 *func_80021DD4(u8 *arg0, s32 arg1, s32 arg2);
void func_800201BC(u8 *arg0, GteSetup *arg1);
void scripts_render(void);
void func_8001E984(s32 arg0, s32 arg1);
void func_80020DF8(u8 *arg0);
void func_80020E9C(NAS *nas, u8 *arg1);
void func_80020F5C(u8 *arg0);
void func_80021054(u8 *arg0);
void func_8002128C(u8 *arg0, u8 *arg1, s32 arg2);
void func_8002147C(VelObj *obj, VelParams *p, VelState *s);
void func_80021604(u8 *arg0, u8 *arg1, s32 arg2);
void func_80021798(u8 *arg0, u8 *arg1, s32 arg2);
void func_80021A1C(u8 *arg0, u8 *arg1, s32 arg2);
s32 *func_80021CB8(s32 *arg0, s32 arg1, s32 arg2);
u8 *func_80021D1C(u8 *arg0, s32 arg1);
void func_8001F4F8(u8 *arg0, u8 *arg1);
void sub_play_script(obj_dbase *db, s32 arg1, subengine *sub, s32 arg3);
void func_800210F8(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 * *arg4);
s32 func_80020064(u8 *arg0);
void update_camera_thread(script_thread_handler *th, script_control_info *ci);
void func_8001FCDC(u8 *arg0, u8 *arg1);
void func_80020344(Quat *a, Quat *b, s32 t, Quat *out);
void func_8001F0D4(u8 *arg0, u8 *arg1);
void func_8001F828(u8 *arg0, u8 *arg1);
u8 *func_8001FE80(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 * *arg5);
void func_8001EAA4(u8 *arg0, u8 *arg1);
void func_80020680(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre);
s32 func_80020C04(s32 arg0);

/* out = a + (b - a) * t / len, per component. */
static inline void CamVec3_Lerp(CamVec3 *out, CamVec3 *a, CamVec3 *b, s32 len, s32 t) {
    out->v[0] = interp(a->v[0], b->v[0], len, t);
    out->v[1] = interp(a->v[1], b->v[1], len, t);
    out->v[2] = interp(a->v[2], b->v[2], len, t);
}

/* The key of the camera track that covers frame v (or the terminating one). */
static inline CamKey *CamTrack_FindKey(u8 *track, s32 v) {
    CamKey *key = (CamKey *)(track + 0x1C);

    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

static inline AnimKey *AnimTrack_FindKey(AnimKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* The key of the object track that covers frame v (or the terminating one). */
static inline ObjKey *ObjTrack_FindKey(ObjKey *key, s32 v) {
    while (key->len != 0) {
        if (v >= key->start && v < key->start + key->len) {
            return key;
        }
        key++;
    }
    return key;
}

/* Frame t (16.16) of the range, clamped or looped, relative to its first frame. */
static inline s32 AnimRange_Frame(AnimRange *range, s32 t) {
    s32 frame;

    t -= range->off << 16;
    if (t < 0) {
        frame = range->lo << 16;
    } else if (range->clamp != 0) {
        if (t < (range->hi - range->lo) << 16) {
            frame = t + (range->lo << 16);
        } else {
            frame = (range->hi - 1) << 16;
        }
    } else {
        frame = t % ((range->hi - range->lo + 1) << 16);
        frame += range->lo << 16;
        if ((range->hi - 1) << 16 < frame) {
            frame = (range->hi - 1) << 16;
        }
    }
    frame -= range->lo << 16;
    return frame;
}

static inline s32 QuatDot(Quat *a, Quat *b) {
    return (a->v[0] * b->v[0] + a->v[1] * b->v[1] + a->v[2] * b->v[2] + a->v[3] * b->v[3] + 0x800) >> 12;
}

/* fade from `from` to `to` over [0,in), hold `to`, back to `from` over (out,end] */
static inline s32 EmitPart_Fade(s32 in, s32 out, s32 n, s32 end, s32 from, s32 to) {
    s32 ret;

    if (n < 0) {
        ret = from;
    } else if (n < in) {
        ret = interp(from, to, in, n);
    } else if (out < n) {
        if (end < n) {
            ret = from;
        } else {
            ret = interp(to, from, end - out, n - out);
        }
    } else {
        ret = to;
    }
    return ret;
}

#endif /* BASH_SCRIPTS_H */
