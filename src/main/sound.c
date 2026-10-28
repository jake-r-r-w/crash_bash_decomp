#include "common.h"
#include "bash/main.h"
#include "bash/sound.h"

void func_8002AAA0(void) {
    D_80064414 = 0;
}

void sound_critical(void) {
    D_80064414 = 1;
}

void func_8002AABC(void) {
    CurrentMusic.data = 0;
    SsInit();
    SsSetTableSize(D_800654CC, 1, 1);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(7);
    SsUtSetReverbDepth(100, 0);
    func_800369FC();
    func_8002AAA0();
    D_800643F8 = (void (*)())SsSetTickCallback(func_8002ABA8);
    SsSetTickMode(1);
    SsStart();
}

void machine_quit_sound(void) {
    if (D_800643F8 != 0) {
        func_8002C428();
        SsUtFlush();
        SsEnd();
        SsSetTickCallback(D_800643F8);
        SsQuit();
        VSync(3);
        D_800643F8 = 0;
    }
}

void func_8002ABA8(void) {
    s32 temp_v0;

    num_sfx_banks += 1;
    if (D_800654B0 > 0) {
        temp_v0 = D_800654B0 - 1;
        D_800654B0 = temp_v0;
        if (temp_v0 == 0) {
            D_800654B4 += 1;
        }
    }
    if (D_80064414 == 0) {
        sound_continue();
    }
}

void func_8002AC18(void) {
    continue_music();
    func_800227C0();
}

void func_8002AC40(void) {
    func_8002C11C();
    func_800227FC();
    xa_mute();
}

void func_8002AC70(void) {
    func_8002C428();
    sfx_delete_all();
    xa_mute();
}

void func_8002ACA0(s32 id, s32 arg1) {
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
    func_8002AAA0();
}

void func_8002ADD4(void *data, sfx_machine_bank_info *slot) {
    u8 *head;
    u8 *body;
    s16 id;

    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
    if ((VabLoading != NULL || SfxBankLoading != NULL) && SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
        SfxBankLoading->id = D_800654C8;
        mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
        func_8002ACA0(SfxBankLoading->id, 0);
        VabLoading = NULL;
        SfxBankLoading = NULL;
    }
    head = (u8 *)data + ((VabFile *)data)->head;
    slot->head = head;
    slot->handle = 0;
    body = (u8 *)data + ((VabFile *)data)->body;
    id = SsVabOpenHead(slot->head, -1);
    SsVabTransBody(body, id);
    D_800654C8 = id;
    VabLoading = data;
    SfxBankLoading = slot;
}

void func_8002AEE0(void) {
    MATRIX *m;

    if (VabLoading != NULL || SfxBankLoading != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            SfxBankLoading->id = D_800654C8;
            mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
            func_8002ACA0(SfxBankLoading->id, 0);
            VabLoading = NULL;
            SfxBankLoading = NULL;
        }
    }
    SpuGetAllKeysStatus(D_800644A8);
    cam_build_view(MainCamera, ScreenRect);
    m = (MATRIX *)(MainCamera + 0x74);
    gte_SetRotMatrix(m);
    gte_SetGeomScreen(*(s32 *)(MainCamera + 0x18));
    gte_SetTransMatrix(m);
    gte_SetGeomOffset(ScreenRect->w / 2, 175);
}

s32 func_8002B020(VECTOR *v, s32 *range) {
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

S16Pair func_8002B210(VECTOR *pos) {
    S16Pair sxy;
    S16Pair ret;
    S16Pair tmp;
    s32 vol;

    ViewProject(pos, (s32 *)&sxy);
    vol = ViewVolumeXY(pos, (View *)MainCamera, (s32 *)&ret);
    if (D_8005A60C != 0) {
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

void func_8002B4D8(PitchVoice *arg0, s32 arg1) {
    PitchInfo *info = arg0->info;

    SsUtChangePitch(arg0->voice, info->note, ((s32)(info->fine << 24)) >> 28, 0x3C, 0,
                    (s32)(arg1 << 9) >> 16, arg1 & 0x7F);
}

void func_8002B52C(s16 *arg0, s32 arg1, s32 arg2) {
    s32 vol = D_8005A608 * 0x7F;

    SsUtSetVVol(*arg0, (arg1 * vol) / 409600, (arg2 * vol) / 409600);
}

s32 func_8002B5B0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 *p = (s32 *)GameOptions;
    s8 *sfx = SfxTable[arg1];
    s16 v;

    v = SsUtKeyOn(sfx[0], (s32)(((u8 *)sfx)[1] << 24) >> 28, ((u8 *)sfx)[1] & 0xF, (s32)(arg2 << 9) >> 16,
                  arg2 & 0x7F, (arg3 * ((p[1] << 7) - p[1])) / 409600, (arg4 * ((p[1] << 7) - p[1])) / 409600);
    arg0[0] = v;
    arg0[1] = (s32)sfx;
    if (v != -1) {
        D_800654B8 |= D_80064418[v];
        SsSetVoiceMask(D_800654B8);
    }
    return v;
}

s32 func_8002B6D0(VECTOR *pos, View *view) {
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
    return func_8002B020(out, D_80064478);
}

void func_8002B760(VECTOR *pos, View *view, VECTOR *out, s32 *sxy) {
    SVECTOR *in = &RenderState.gteIn;

    in->vx = pos->vx - view->pos.vx;
    in->vy = pos->vy - view->pos.vy;
    in->vz = pos->vz - view->pos.vz;
    gte_ldv0(in);
    gte_rtps();
    gte_stlvl(out);
    gte_stsxy(sxy);
}

S16Pair func_8002B7C8(s32 x, s32 v) {
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

void func_8002B8F4(s32 *voice) {
    if (*voice != -1) {
        SsUtKeyOffV(*voice);
        D_800654B8 &= ~D_80064418[*voice];
        SsSetVoiceMask(D_800654B8);
        *voice = -1;
    }
}


void func_8002B96C(void) {
    SpuIsTransferCompleted(1);
    D_800654B8 = 0;
    SsSetVoiceMask(0);
    SsUtAllKeyOff(0);
}

void func_8002B9A4(void) {
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
    D_800654B8 = 0;
    func_8002B96C();
}

void machine_load_sfx(s32 arg0, sfx_machine_bank_info *slot) {
    slot->handle = asload_permanent_callback(arg0, func_8002ADD4, slot);
}

void func_8002BA44(void) {
    SpuIsTransferCompleted(1);
    if (VabLoading != NULL || SfxBankLoading != NULL) {
        if (SpuIsTransferCompleted(SPU_TRANSFER_PEEK) == 1) {
            SfxBankLoading->id = D_800654C8;
            mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
            func_8002ACA0(SfxBankLoading->id, 0);
            VabLoading = NULL;
            SfxBankLoading = NULL;
        }
    }
}

void func_8002BAE8(void) {
    if ((VabLoading != NULL || SfxBankLoading != NULL) && SpuIsTransferCompleted(0) == 1) {
        SfxBankLoading->id = D_800654C8;
        mal_trim_front(VabLoading, VabLoading->head, &MainHeap);
        func_8002ACA0(SfxBankLoading->id, 0);
        VabLoading = NULL;
        SfxBankLoading = NULL;
    }
}

s32 func_8002BB80(void) {
    if ((VabLoading != 0) || (SfxBankLoading != 0)) {
        return 1;
    }
    return 0;
}

/* Empties the SfxBanks slots and clears SfxTable. */
void func_8002BBB4(void) {
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

void func_8002BBEC(void) {
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
        func_8002ACA0(slot->id, 1);
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

void func_8002BCB0(s32 *arg0) {
    SpuIsTransferCompleted(1);
    func_8002ACA0(arg0[1], 1);
    SsVabClose((s16)arg0[1]);
    mal_free((void *)arg0[0], &MainHeap);
    arg0[0] = 0;
    arg0[1] = -1;
}

void func_8002BD0C(s32 arg0) {
    if (SfxBankCount < 6) {
        machine_load_sfx(arg0, &SfxBanks[SfxBankCount]);
        SfxBankCount++;
    }
}

/* Clears SfxTable: no sound for any id. */
void func_8002BD68(void) {
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
s32 D_80064414 = 0;
s32 D_80064418[24] = {
    1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000,
    0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
};
s32 D_80064478[12] = { 0x400, 0x1000, 0xF00, 0xA00, 0x1200, 0x2800, 0x800, 0x1000, 0xF00, 0xA00, 0x1200, 0x1C00 };
u8 D_800644A8[24] = {0};
sfx_machine_bank_info SfxBanks[6] = {0};
s8 SfxTable[2002][2] = {0};
s32 num_sfx_banks = 0;
s32 D_800654B0 = 0;
s32 D_800654B4 = 0;
s32 D_800654B8 = 0;
s32 SfxBankCount = 0;
VabFile *VabLoading = 0;
sfx_machine_bank_info *SfxBankLoading = 0;
s32 D_800654C8 = 0;
char D_800654CC[176] = {0};
music_midi CurrentMusic = {0};
