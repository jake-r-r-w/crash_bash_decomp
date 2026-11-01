#ifndef BASH_SOUND_H
#define BASH_SOUND_H

/* Functions and data defined in src/main/sound.c. Anything the original headers declare
 * (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/main/sound.c */

#include "common.h"
#include "bash/main.h"

void vab_transfer_check(void);
void machine_load_sfx(file *arg0, sfx_machine_bank_info *slot);
void vab_loaded(void *data, sfx_machine_bank_info *slot);
void sound_reset_counter(void);
void sound_critical(void);
void machine_quit_sound(void);
void sound_vsync_tick(void);
void sound_resume(void);
void sound_pause(void);
void sound_stop(void);
s32 sfx_distance_volume(VECTOR *v, s32 *range);
s32 view_project_volume(VECTOR *pos, camera_info_tag *view);
void view_project(VECTOR *pos, camera_info_tag *view, VECTOR *out, s32 *sxy);
void sfx_all_off(void);
s32 vab_loading(void);
/* by sound id; [1] == -1: no sound */
void sfx_key_off(s32 *voice);
void vab_map_sfx(s32 id, s32 arg1);
void vab_transfer_poll(void);
void machine_init_sound(void);
void sfx_bank_load(file *arg0);
void sfx_table_reset(void);
void sfx_table_clear(void);
void sfx_table_init(void);
void sfx_change_pitch(PitchVoice *arg0, s32 arg1);
void sfx_set_voice_volume(s16 *arg0, s32 arg1, s32 arg2);
void vab_transfer_wait(void);
void sfx_bank_close(sfx_machine_bank_info *arg0);
void sfx_banks_close(void);
s32 sfx_key_on(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
S16Pair sfx_pan(s32 x, s32 v);
S16Pair sfx_position_volume(VECTOR *pos);

static inline void ViewProject(VECTOR *pos, s32 *psxy) {
    VECTOR *out = &RenderState.gteOut;
    SVECTOR *in = &RenderState.gteIn;
    camera_info_tag *view = &MainCamera;

    in->vx = pos->vx - view->at.x;
    in->vy = pos->vy - view->at.y;
    in->vz = pos->vz - view->at.z;
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
static inline s32 ViewVolumeXY(VECTOR *pos, camera_info_tag *view, s32 *psxy) {
    VECTOR *out = &RenderState.gteOut;
    SVECTOR *in = &RenderState.gteIn;

    in->vx = pos->vx - view->at.x;
    in->vy = pos->vy - view->at.y;
    in->vz = pos->vz - view->at.z;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return sfx_distance_volume(out, SfxDistanceCurve);
}

#endif /* BASH_SOUND_H */
