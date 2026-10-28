#include "common.h"
#include "bash/main.h"
#include "bash/sound.h"

void sound_reset_counter(void) {
    SoundCritical = 0;
}

void sound_critical(void) {
    SoundCritical = 1;
}

void machine_init_sound(void) {
    CurrentMusic.data = 0;
    SsInit();
    SsSetTableSize(SsTable, 1, 1);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(7);
    SsUtSetReverbDepth(100, 0);
    func_800369FC();
    sound_reset_counter();
    SsTickSaved = (void (*)())SsSetTickCallback(sound_vsync_tick);
    SsSetTickMode(1);
    SsStart();
}

void machine_quit_sound(void) {
    if (SsTickSaved != 0) {
        music_stop();
        SsUtFlush();
        SsEnd();
        SsSetTickCallback(SsTickSaved);
        SsQuit();
        VSync(3);
        SsTickSaved = 0;
    }
}

void sound_vsync_tick(void) {
    s32 temp_v0;

    num_sfx_banks += 1;
    if (VabCueTimer > 0) {
        temp_v0 = VabCueTimer - 1;
        VabCueTimer = temp_v0;
        if (temp_v0 == 0) {
            VabCuePending += 1;
        }
    }
    if (SoundCritical == 0) {
        sound_continue();
    }
}

void sound_resume(void) {
    continue_music();
    sfx_resume_all();
}

void sound_pause(void) {
    music_pause();
    sfx_pause_all();
    xa_mute();
}

void sound_stop(void) {
    music_stop();
    sfx_delete_all();
    xa_mute();
}

void vab_map_sfx(s32 id, s32 arg1) {
    ProgAtr prog;
    VagAtr vag;
    s32 i;
    s32 j;
    s32 n;

    sound_critical();
    for (i = 0;; i++) {
        if (i >= 16) {
            goto done;
        }
        SsUtGetProgAtr(id, i, &prog);
        for (j = 0;; j++) {
            if (j >= prog.tones) {
                goto next;
            }
            if (SsUtGetVagAtr(id, i, j, &vag) == 0) {
                n = vag.min + (vag.max << 8);
                if (n != 0) {
                    if (arg1 != 0) {
                        SfxTable[n][0] = -1;
                        SfxTable[n][1] = -1;
                    } else {
                        SfxTable[n][0] = id;
                        SfxTable[n][1] = (i << 4) | (j & 0xF);
                    }
                }
            }
        }
    next:;
    }
done:
    sound_reset_counter();
}

void vab_loaded(void *data, sfx_machine_bank_info *slot) {
    u8 *head;
    u8 *body;
    s16 id;

    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    if ((VabLoading != NULL || SfxBankLoading != NULL) && SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
        SfxBankLoading->id = VabHeadId;
        mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
        vab_map_sfx(SfxBankLoading->id, 0);
        VabLoading = NULL;
        SfxBankLoading = NULL;
    }
    head = (u8 *)data + ((VabFile *)data)->head;
    slot->head = head;
    slot->handle = 0;
    body = (u8 *)data + ((VabFile *)data)->body;
    id = SsVabOpenHead(slot->head, -1);
    SsVabTransBody(body, id);
    VabHeadId = id;
    VabLoading = data;
    SfxBankLoading = slot;
}

void vab_transfer_poll(void) {
    MATRIX *m;

    if (VabLoading != NULL || SfxBankLoading != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            SfxBankLoading->id = VabHeadId;
            mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
            vab_map_sfx(SfxBankLoading->id, 0);
            VabLoading = NULL;
            SfxBankLoading = NULL;
        }
    }
    SpuGetAllKeysStatus(SpuKeyStatus);
    cam_build_view(MainCamera, ScreenRect);
    m = (MATRIX *)(MainCamera + 0x74);
    gte_SetRotMatrix(m);
    gte_SetGeomScreen(*(s32 *)(MainCamera + 0x18));
    gte_SetTransMatrix(m);
    gte_SetGeomOffset(ScreenRect->w / 2, 175);
}

s32 sfx_distance_volume(VECTOR *v, s32 *range) {
    s32 d[3];
    s32 r;

    if (v->vx < 0) {
        v->vx = -v->vx;
    }
    v->vx -= range[0];
    if (v->vx < 0) {
        v->vx = 0;
    }
    d[0] = (v->vx << 12) / (range[3] - range[0]);
    if (v->vy < 0) {
        v->vy = -v->vy;
    }
    v->vy -= range[1];
    if (v->vy < 0) {
        v->vy = 0;
    }
    d[1] = (v->vy << 12) / (range[4] - range[1]);
    if (v->vz < 0) {
        v->vz = -v->vz;
    }
    v->vz -= range[2];
    if (v->vz < 0) {
        v->vz = 0;
    }
    d[2] = (v->vz << 12) / (range[5] - range[2]);
    r = 0x1000 - (csqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) >> 6);
    if (r < 0) {
        r = 0;
    }
    if (r > 0x1000) {
        r = 0x1000;
    }
    return r;
}

S16Pair sfx_position_volume(VECTOR *pos) {
    S16Pair sxy;
    S16Pair ret;
    S16Pair tmp;
    s32 vol;

    ViewProject(pos, (s32 *)&sxy);
    vol = ViewVolumeXY(pos, (View *)MainCamera, (s32 *)&ret);
    if (GameOptions.stereo != 0) {
        vol *= 0x1450;
        vol /= 4096;
        if (vol > 0x1000) {
            vol = 0x1000;
        }
        tmp = PanVolume(sxy.a, vol);
        ret = tmp;
    } else {
        ret.a = vol;
        ret.b = vol;
    }
    return ret;
}

void sound_continue(void) {
    SsSeqCalledTbyT();
}

void sfx_change_pitch(PitchVoice *arg0, s32 arg1) {
    PitchInfo *info = arg0->info;

    SsUtChangePitch(arg0->voice, info->note, ((s32)(info->fine << 24)) >> 28, 0x3C, 0,
                    (s32)(arg1 << 9) >> 16, arg1 & 0x7F);
}

void sfx_set_voice_volume(s16 *arg0, s32 arg1, s32 arg2) {
    s32 vol = GameOptions.sfx * 0x7F;

    SsUtSetVVol(*arg0, (arg1 * vol) / 409600, (arg2 * vol) / 409600);
}

s32 sfx_key_on(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameOptionSet *p = &GameOptions;
    s8 *sfx = SfxTable[arg1];
    s16 v;

    v = SsUtKeyOn(sfx[0], (s32)(((u8 *)sfx)[1] << 24) >> 28, ((u8 *)sfx)[1] & 0xF, (s32)(arg2 << 9) >> 16,
                  arg2 & 0x7F, (arg3 * ((p->sfx << 7) - p->sfx)) / 409600, (arg4 * ((p->sfx << 7) - p->sfx)) / 409600);
    arg0[0] = v;
    arg0[1] = (s32)sfx;
    if (v != -1) {
        SfxVoiceMask |= SfxVoiceBits[v];
        SsSetVoiceMask(SfxVoiceMask);
    }
    return v;
}

s32 view_project_volume(VECTOR *pos, View *view) {
    VECTOR *out = &RenderState.gteOut;
    s32 sxy;
    s32 *psxy = &sxy;
    SVECTOR *in = &RenderState.gteIn;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(psxy);
    return sfx_distance_volume(out, SfxDistanceCurve);
}

void view_project(VECTOR *pos, View *view, VECTOR *out, s32 *sxy) {
    SVECTOR *in = &RenderState.gteIn;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(sxy);
}

S16Pair sfx_pan(s32 x, s32 v) {
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

void sfx_key_off(s32 *voice) {
    if (*voice != -1) {
        SsUtKeyOffV(*voice);
        SfxVoiceMask &= ~SfxVoiceBits[*voice];
        SsSetVoiceMask(SfxVoiceMask);
        *voice = -1;
    }
}


void sfx_all_off(void) {
    SpuIsTransferCompleted(1);
    SfxVoiceMask = 0;
    SsSetVoiceMask(0);
    SsUtAllKeyOff(0);
}

void sfx_table_init(void) {
    s8 *p;
    s32 i;

    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
    SfxBankCount = 0;
    VabLoading = NULL;
    SfxBankLoading = NULL;
    SfxVoiceMask = 0;
    sfx_all_off();
}

void machine_load_sfx(s32 arg0, sfx_machine_bank_info *slot) {
    slot->handle = asload_permanent_callback(arg0, vab_loaded, slot);
}

void vab_transfer_wait(void) {
    SpuIsTransferCompleted(1);
    if (VabLoading != NULL || SfxBankLoading != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            SfxBankLoading->id = VabHeadId;
            mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
            vab_map_sfx(SfxBankLoading->id, 0);
            VabLoading = NULL;
            SfxBankLoading = NULL;
        }
    }
}

void vab_transfer_check(void) {
    if ((VabLoading != NULL || SfxBankLoading != NULL) && SpuIsTransferCompleted(0) == 1) {
        SfxBankLoading->id = VabHeadId;
        mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
        vab_map_sfx(SfxBankLoading->id, 0);
        VabLoading = NULL;
        SfxBankLoading = NULL;
    }
}

s32 vab_loading(void) {
    if ((VabLoading != 0) || (SfxBankLoading != 0)) {
        return 1;
    }
    return 0;
}

/* Empties the SfxBanks slots and clears SfxTable. */
void sfx_table_clear(void) {
    s8 *p;
    s32 i;

    SfxBankCount = 0;
    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
}

void sfx_banks_close(void) {
    sfx_machine_bank_info *slot;
    s32 new_var;
    s8 *p;
    s32 i;

    for (;;) {
        s32 n = new_var = SfxBankCount;
        if (n == 0) {
            break;
        }
        n--;
        slot = &SfxBanks[n];
        SfxBankCount = n;
        SpuIsTransferCompleted(1);
        vab_map_sfx(slot->id, 1);
        SsVabClose(slot->id);
        mal_free(slot->head, &MainHeap);
        slot->head = NULL;
        slot->id = -1;
    }
    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
}

void sfx_bank_close(s32 *arg0) {
    SpuIsTransferCompleted(1);
    vab_map_sfx(arg0[1], 1);
    SsVabClose((s16)arg0[1]);
    mal_free((void *)arg0[0], &MainHeap);
    arg0[0] = 0;
    arg0[1] = -1;
}

void sfx_bank_load(s32 arg0) {
    if (SfxBankCount < 6) {
        machine_load_sfx(arg0, &SfxBanks[SfxBankCount]);
        SfxBankCount++;
    }
}

/* Clears SfxTable: no sound for any id. */
void sfx_table_reset(void) {
    s8 *p;
    s32 i;

    p = SfxTable[0];
    for (i = 0; i < 2001; i++) {
        p[0] = -1;
        p[1] = -1;
        p += 2;
    }
}

/* data, in address order (tools/scripts/data2c.py) */
s32 SoundCritical = 0;
s32 SfxVoiceBits[24] = {
    1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000,
    0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
};
s32 SfxDistanceCurve[12] = { 0x400, 0x1000, 0xF00, 0xA00, 0x1200, 0x2800, 0x800, 0x1000, 0xF00, 0xA00, 0x1200, 0x1C00 };
u8 SpuKeyStatus[24] = {0};
sfx_machine_bank_info SfxBanks[6] = {0};
s8 SfxTable[2002][2] = {0};
s32 num_sfx_banks = 0;
s32 VabCueTimer = 0;
s32 VabCuePending = 0;
s32 SfxVoiceMask = 0;
s32 SfxBankCount = 0;
VabFile *VabLoading = 0;
sfx_machine_bank_info *SfxBankLoading = 0;
s32 VabHeadId = 0;
char SsTable[176] = {0};
music_midi CurrentMusic = {0};
