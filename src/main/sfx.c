#include "common.h"
#include "bash/main.h"
#include "bash/sfx.h"

void func_80022140(s32 arg0, s32 arg1) {
    D5A614View *p;
    u8 *q;
    u32 t8;
    u32 t;
    u32 v;
    s32 i;
    s32 f;
    u32 c;
    u16 *d;

    p = (D5A614View *)GameInfo;
    if (p->mode != 0x28) {
        func_8002BBB4();
        if (p->flags & 0x80) {
            f = D_80058B88[arg0].unkC;
        } else {
            f = D_80058B88[arg0].unk8;
        }
        func_8002BD0C(f);
    }
    do {
        q = D_8004E9C0;
        t8 = (u32)(q - 8);
    } while (0);
    d = ModeTypes;
    f = ((D5A614View *)GameInfo)->mode;
    c = d[f * 3] - 0x101;
    f = (s32)(q - 0x10);
    if (c >= 2U) {
        f = (s32)t8;
        if (!(((D5A614View *)GameInfo)->flags & 0x1E0)) {
            f = (s32)q;
        }
    }
    func_8002BD0C(f);
    t = arg1 - 0x101;
    if (t >= 2U) {
        arg1 = t;
        for (i = 0; i < 4; i++) {
            v = GameInfo[i + 0x50] - 1;
            if (v < 8U) {
                func_8002BD0C(D_80058F0C[arg1][v]);
            }
        }
    }
}

void func_800222BC(void) {
    func_8003347C();
}

void func_800222DC(void) {
    func_8003346C();
}

void func_800222FC(void) {
    SsInit();
    func_8002C060();
    func_80022984();
}

void func_8002232C(void) {
    func_8002BBEC();
    SsInit();
    func_8002C44C();
}

s32 func_8002235C(_sfx_handler *voice) {
    S16Pair vol;
    S16Pair tmp;
    S16Pair tmp2;
    s32 f;
    SLONG_XYZ *pos = voice->pos;
    s32 *slot = &voice->unk2C;

    if (voice->unk28 != -1) {
        if (D_800644A8[voice->unk2C] != 1) {
            return 0;
        }
        f = voice->flags & 0xE;
        voice->flags &= ~8;
        if (pos != NULL) {
            f |= 8;
            tmp = func_8002B210((VECTOR *)pos);
            vol = tmp;
        } else {
            u16 t = voice->unk20;
            vol.b = t;
            vol.a = t;
        }
        if (f & 2) {
            if (f & 4) {
                voice->flags &= ~6;
            } else {
                vol.b = 0;
                vol.a = 0;
            }
        }
        voice->flags &= ~4;
        if (f != 0) {
            func_8002B52C((s16 *)slot, vol.a, vol.b);
        }
        if (voice->flags & 1) {
            func_8002B4D8((PitchVoice *)slot, voice->unk24);
            voice->flags &= ~1;
        }
    } else {
        if (pos != NULL) {
            tmp2 = func_8002B210((VECTOR *)pos);
            vol = tmp2;
        } else {
            u16 t = voice->unk20;
            vol.b = t;
            vol.a = t;
        }
        voice->unk28 = func_8002B5B0(&voice->unk2C, voice->id, voice->unk24, vol.a, vol.b);
    }
    return voice->unk28 != -1;
}

void sfx_delete_all(void) {
    _sfx_handler *next;
    _sfx_handler *v;

    v = SfxVoicePool.active;
    for (;;) {
        if (v == NULL) {
            return;
        }
        next = v->next;
        if (v->owner != NULL) {
            *v->owner = 0;
        }
        func_8002B8F4(&v->unk2C);
        if (v->next != NULL) {
            v->next->prev = v->prev;
        }
        if (v->prev != NULL) {
            v->prev->next = v->next;
        } else {
            SfxVoicePool.active = v->next;
        }
        v->next = SfxVoicePool.free;
        SfxVoicePool.free = v;
        v = next;
    }
}

void StopSfx(_sfx_handler *voice) {
    if (voice != NULL) {
        if (voice->owner != NULL) {
            *voice->owner = 0;
        }
        func_8002B8F4(&voice->unk2C);
        if (voice->next != NULL) {
            voice->next->prev = voice->prev;
        }
        if (voice->prev != NULL) {
            voice->prev->next = voice->next;
        } else {
            SfxVoicePool.active = voice->next;
        }
        voice->next = SfxVoicePool.free;
        SfxVoicePool.free = voice;
    }
}

_sfx_handler *PlaySfx(s32 id, _sfx_handler **handle, SLONG_XYZ *pos, s32 arg3, s32 arg4, s32 flags) {
    _sfx_handler *voice;
    _sfx_handler *v;

    voice = NULL;
    if (id & 0x8000) {
        func_80027A74();
    } else {
        if (SfxTable[id][1] == -1) {
            return NULL;
        }
        v = SfxVoicePool.free;
        if (v != NULL) {
            SfxVoicePool.free = v->next;
            v->next = SfxVoicePool.active;
            v->prev = NULL;
            if (SfxVoicePool.active != NULL) {
                SfxVoicePool.active->prev = v;
            }
            voice = v;
            voice->owner = NULL;
            voice->unk28 = -1;
            voice->unk2C = -1;
        }
        if (voice != NULL) {
            voice->next = SfxVoicePool.active;
            voice->prev = NULL;
            SfxVoicePool.active = voice;
            voice->id = id;
            voice->unk20 = arg3;
            voice->unk24 = arg4;
            voice->flags = flags;
            voice->unk28 = -1;
            if (handle != NULL) {
                if (*handle != NULL) {
                    StopSfx(*handle);
                }
                *handle = voice;
                voice->owner = handle;
                voice->pos = pos;
            } else if (pos != NULL) {
                voice->posCopy = *pos;
                voice->pos = &voice->posCopy;
            } else {
                voice->pos = NULL;
            }
        }
    }
    return voice;
}

void func_800227C0(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 4;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void func_800227FC(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 2;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void func_80022838(void) {
    _sfx_handler *next;
    _sfx_handler *voice;

    func_8002AEE0();
    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            next = voice->next;
            if (func_8002235C(voice) != 1) {
                StopSfx(voice);
            }
            voice = next;
        } while (voice != NULL);
    }
}

void func_800228A4(_sfx_handler *voice, s32 arg1) {
    if (voice != NULL) {
        voice->unk24 = arg1;
        voice->flags |= 1;
    }
}

void func_800228C4(_sfx_handler *voice) {
    if (voice->next != NULL) {
        voice->next->prev = voice->prev;
    }
    if (voice->prev != NULL) {
        voice->prev->next = voice->next;
    } else {
        SfxVoicePool.active = voice->next;
    }
    voice->next = SfxVoicePool.free;
    SfxVoicePool.free = voice;
}

_sfx_handler *func_80022924(void) {
    _sfx_handler *v;
    _sfx_handler *r;

    v = SfxVoicePool.free;
    if (v == NULL) {
        return NULL;
    }
    SfxVoicePool.free = v->next;
    v->next = SfxVoicePool.active;
    v->prev = NULL;
    if (SfxVoicePool.active != NULL) {
        SfxVoicePool.active->prev = v;
    }
    r = v;
    r->owner = NULL;
    r->unk28 = -1;
    r->unk2C = -1;
    return r;
}

void func_80022984(void) {
    _sfx_handler *v;
    s32 i;

    v = SfxVoicePool.voices;
    for (i = 22; i >= 0; i--) {
        v->next = v + 1;
        v++;
    }
    v->next = NULL;
    SfxVoicePool.active = NULL;
    SfxVoicePool.free = SfxVoicePool.voices;
    func_8002B9A4();
}


void func_800229D4(void) {
    func_8002BBB4();
    func_80022984();
}

void func_800229FC(s32 i) {
    func_8002C488(D_80058B88[i].file, D_80058B88[i].reverb);
}

static inline NAS *AllocNAS(void) {
    NAS *free;
    NAS *next;

    free = NASFreeList;
    if (free == NULL) {
        return NULL;
    }
    next = free->Tag.Child;
    NASFreeList = next;
    if (next != NULL) {
        next->Tag.Parent = NULL;
    }
    return free;
}
