#ifndef BASH_SCRIPTS_H
#define BASH_SCRIPTS_H

/* Functions and data defined in src/main/scripts.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/scripts.c */

#include "common.h"
#include "bash/prims.h"

void quat_slerp_ex_frac(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void kill_emitter_thread(script_thread_handler *th);
void render_entity_thread(script_thread_handler *th, geom_info *gi);
void render_model_thread(script_thread_handler *th, s32 *arg1);
void interp3(s32 *arg0, s32 *arg1, s32 *arg2, s32 arg3, s32 arg4);
void script_list_push(script_thread_handler *th);
void script_unlink(script_handler *s);
script_handler *script_alloc(void);
void scripts_release(void);
void scripts_kill_all(void);
void scripts_update(void);
void emit_particle(u8 *obj, EmitPart *part, u8 *def);
void render_emitter_thread(script_thread_handler *th, s32 *arg1);
u8 *offset_table_entry(OffsetTable *tbl, s32 i);
script_thread_handler *script_list_pop(void);
void kill_sub_thread(script_thread_handler *th);
u8 *get_script_header(obj_dbase *db, s32 arg1);
script_thread_handler *script_thread_alloc(script_control_info *ci);
void init_camera_thread(s32 def, script_control_info *ci, s32 num);
void render_sub_thread(script_thread_handler *th, s32 *arg1);
void interp3_s16(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void lerp_angles3(s32 *arg0, s32 *arg1, s16 *arg2, s32 arg3, s32 arg4);
void init_fade_thread(u8 *def, script_control_info *ci, s32 num);
void quat_slerp_ex_frac(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void quat_slerp_frac(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
s16 cos_half(s32 arg0);
s32 interp_window(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 *find_range_30(u8 *arg0, s32 arg1);
s32 *find_range_1c(u8 *arg0, s32 arg1);
s32 *find_range_24(u8 *arg0, s32 arg1);
u8 *get_script_track(u8 *arg0, s32 arg1, s32 arg2);
void render_script(script_control_info *ci, GteSetup *arg1);
void scripts_render(void);
void init_scripts(s32 arg0, s32 arg1);
void kill_nas_script(u8 *arg0);
void plot_script_nas(NAS *nas, u8 *arg1);
void kill_script(script_handler *s);
void kill_script_threads(script_control_info *ci);
void init_sub_thread(u8 *def, script_control_info *ci, s32 num);
void vel_update(VelObj *obj, VelParams *p, VelState *s);
void init_emitter_thread(u8 *def, script_control_info *ci, s32 num);
void init_entity_thread(u8 *def, script_control_info *ci, s32 num);
void init_model_thread(u8 *def, script_control_info *ci, s32 num);
s32 *find_range(s32 *arg0, s32 arg1, s32 arg2);
u8 *script_track_at(u8 *arg0, s32 arg1);
void update_fade_thread(script_thread_handler *th, script_control_info *ci);
void sub_play_script(obj_dbase *db, s32 arg1, subengine *sub, s32 arg3);
void script_attach(u8 *arg0, s32 arg1, u8 *arg2, s32 arg3, u8 * *arg4);
s32 update_script(script_control_info *ci);
void update_camera_thread(script_thread_handler *th, script_control_info *ci);
void update_sub_thread(script_thread_handler *th, script_control_info *ci);
void quat_slerp(Quat *a, Quat *b, s32 t, Quat *out);
void update_entity_thread(u8 *arg0, u8 *arg1);
void update_emitter_thread(u8 *arg0, u8 *arg1);
u8 *play_script(u8 *arg0, s32 arg1, Blk32 *arg2, CamVec3 *arg3, s32 arg4, u8 * *arg5);
void update_model_thread(u8 *arg0, u8 *arg1);
void quat_slerp_ex(Quat *a, Quat *b, s32 t, Quat *out, s32 *pre);
s32 cos_interp(s32 arg0);

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
