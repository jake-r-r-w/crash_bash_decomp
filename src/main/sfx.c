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
            f = MusicTable[arg0].unkC;
        } else {
            f = MusicTable[arg0].unk8;
        }
        func_8002BD0C(f);
    }
    do {
        q = &DatIndex[2 * 278];
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
    SsSetStereo();
}

void func_800222DC(void) {
    SsSetMono();
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
    func_8002C488(MusicTable[i].file, MusicTable[i].reverb);
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

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern const char PauseText01[];
extern const char PauseText02[];
extern const char PauseText03[];
extern const char PauseText090Percent[];
extern const char PauseText100Percent[];
extern const char PauseTextAdjustLevel[];
extern const char PauseTextAudioMode[];
extern const char PauseTextChangeArena[];
extern const char PauseTextChangeArena2[];
extern const char PauseTextCheatFlags[];
extern const char PauseTextCheatMenu[];
extern const char PauseTextContinue[];
extern const char PauseTextController[];
extern const char PauseTextController1AIsUnplugged[];
extern const char PauseTextController1BIsUnplugged[];
extern const char PauseTextController1CIsUnplugged[];
extern const char PauseTextController1DIsUnplugged[];
extern const char PauseTextController1IsUnplugged[];
extern const char PauseTextController2IsUnplugged[];
extern const char PauseTextDemo[];
extern const char PauseTextEmpty[];
extern const char PauseTextExit[];
extern const char PauseTextExitArena[];
extern const char PauseTextExitArena2[];
extern const char PauseTextFxVolume[];
extern const char PauseTextLevelSelector[];
extern const char PauseTextMono[];
extern const char PauseTextMusicVolume[];
extern const char PauseTextNewGame[];
extern const char PauseTextNo[];
extern const char PauseTextOff[];
extern const char PauseTextOn[];
extern const char PauseTextOptions[];
extern const char PauseTextP1[];
extern const char PauseTextP2[];
extern const char PauseTextP3[];
extern const char PauseTextP4[];
extern const char PauseTextPleaseInsertAController[];
extern const char PauseTextPressButtonToAccept[];
extern const char PauseTextPressButtonToChange[];
extern const char PauseTextPressButtonToForceVictory[];
extern const char PauseTextQuitGame[];
extern const char PauseTextQuitGame2[];
extern const char PauseTextReplayThisArena[];
extern const char PauseTextReturnToWarpRoom[];
extern const char PauseTextReturnToWarpRoom2[];
extern const char PauseTextShowRules[];
extern const char PauseTextSound[];
extern const char PauseTextSoundOptions[];
extern const char PauseTextStereo[];
extern const char PauseTextTweakAwards[];
extern const char PauseTextTweakSettings[];
extern const char PauseTextVibration[];
extern const char PauseTextViewStats[];
extern const char PauseTextYes[];
extern const char PauseTextYesNo[];

MusicEntry MusicTable[45] = {
    { (s32)&DatIndex[2 * 293], 2, { 0, 0 }, (s32)&DatIndex[2 * 167], (s32)&DatIndex[2 * 167], 0x9A, { 0, 0 } },
    { (s32)&DatIndex[2 * 297], 2, { 0, 0 }, (s32)&DatIndex[2 * 166], (s32)&DatIndex[2 * 166], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 289], 2, { 0, 0 }, (s32)&DatIndex[2 * 165], (s32)&DatIndex[2 * 165], 0xE1, { 0, 0 } },
    { (s32)&DatIndex[2 * 298], 2, { 0, 0 }, (s32)&DatIndex[2 * 164], (s32)&DatIndex[2 * 164], 0x90, { 0, 0 } },
    { (s32)&DatIndex[2 * 290], 2, { 0, 0 }, (s32)&DatIndex[2 * 163], (s32)&DatIndex[2 * 163], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 283], 2, { 0, 0 }, (s32)&DatIndex[2 * 159], (s32)&DatIndex[2 * 159], 0x90, { 0, 0 } },
    { (s32)&DatIndex[2 * 281], 2, { 0, 0 }, (s32)&DatIndex[2 * 162], (s32)&DatIndex[2 * 162], 0x74, { 0, 0 } },
    { (s32)&DatIndex[2 * 282], 2, { 0, 0 }, (s32)&DatIndex[2 * 160], (s32)&DatIndex[2 * 160], 0x79, { 0, 0 } },
    { (s32)&DatIndex[2 * 280], 2, { 0, 0 }, (s32)&DatIndex[2 * 161], (s32)&DatIndex[2 * 161], 0x6F, { 0, 0 } },
    { (s32)&DatIndex[2 * 280], 2, { 0, 0 }, (s32)&DatIndex[2 * 158], (s32)&DatIndex[2 * 158], 0x6F, { 0, 0 } },
    { (s32)&DatIndex[2 * 299], 2, { 0, 0 }, (s32)&DatIndex[2 * 171], (s32)&DatIndex[2 * 171], 0x6C, { 0, 0 } },
    { (s32)&DatIndex[2 * 286], 2, { 0, 0 }, (s32)&DatIndex[2 * 170], (s32)&DatIndex[2 * 170], 0xB2, { 0, 0 } },
    { (s32)&DatIndex[2 * 291], 2, { 0, 0 }, (s32)&DatIndex[2 * 169], (s32)&DatIndex[2 * 169], 0xA0, { 0, 0 } },
    { (s32)&DatIndex[2 * 454], 2, { 0, 0 }, (s32)&DatIndex[2 * 168], (s32)&DatIndex[2 * 168], 0x5F, { 0, 0 } },
    { (s32)&DatIndex[2 * 296], 2, { 0, 0 }, (s32)&DatIndex[2 * 157], (s32)&DatIndex[2 * 157], 0xA0, { 0, 0 } },
    { (s32)&DatIndex[2 * 287], 2, { 0, 0 }, (s32)&DatIndex[2 * 156], (s32)&DatIndex[2 * 156], 0x8C, { 0, 0 } },
    { (s32)&DatIndex[2 * 286], 2, { 0, 0 }, (s32)&DatIndex[2 * 155], (s32)&DatIndex[2 * 155], 0xB2, { 0, 0 } },
    { (s32)&DatIndex[2 * 302], 2, { 0, 0 }, (s32)&DatIndex[2 * 154], (s32)&DatIndex[2 * 154], 0x83, { 0, 0 } },
    { (s32)&DatIndex[2 * 279], 2, { 0, 0 }, (s32)&DatIndex[2 * 450], (s32)&DatIndex[2 * 449], 0xD7, { 0, 0 } },
    { (s32)&DatIndex[2 * 295], 2, { 0, 0 }, (s32)&DatIndex[2 * 451], (s32)&DatIndex[2 * 451], 0xA0, { 0, 0 } },
    { (s32)&DatIndex[2 * 298], 2, { 0, 0 }, (s32)&DatIndex[2 * 448], (s32)&DatIndex[2 * 448], 0x90, { 0, 0 } },
    { (s32)&DatIndex[2 * 453], 2, { 0, 0 }, (s32)&DatIndex[2 * 447], (s32)&DatIndex[2 * 447], 0x83, { 0, 0 } },
    { (s32)&DatIndex[2 * 452], 2, { 0, 0 }, (s32)&DatIndex[2 * 446], (s32)&DatIndex[2 * 446], 0x9D, { 0, 0 } },
    { (s32)&DatIndex[2 * 285], 2, { 0, 0 }, (s32)&DatIndex[2 * 175], (s32)&DatIndex[2 * 175], 0xC8, { 0, 0 } },
    { (s32)&DatIndex[2 * 284], 2, { 0, 0 }, (s32)&DatIndex[2 * 174], (s32)&DatIndex[2 * 174], 0xBE, { 0, 0 } },
    { (s32)&DatIndex[2 * 285], 2, { 0, 0 }, (s32)&DatIndex[2 * 173], (s32)&DatIndex[2 * 173], 0xC8, { 0, 0 } },
    { (s32)&DatIndex[2 * 283], 2, { 0, 0 }, (s32)&DatIndex[2 * 172], (s32)&DatIndex[2 * 172], 0x90, { 0, 0 } },
    { (s32)&DatIndex[2 * 279], 2, { 0, 0 }, (s32)&DatIndex[2 * 180], (s32)&DatIndex[2 * 180], 0xD7, { 0, 0 } },
    { (s32)&DatIndex[2 * 283], 2, { 0, 0 }, (s32)&DatIndex[2 * 179], (s32)&DatIndex[2 * 179], 0x90, { 0, 0 } },
    { (s32)&DatIndex[2 * 292], 3, { 0, 0 }, (s32)&DatIndex[2 * 178], (s32)&DatIndex[2 * 178], 0x9B, { 0, 0 } },
    { (s32)&DatIndex[2 * 292], 2, { 0, 0 }, (s32)&DatIndex[2 * 177], (s32)&DatIndex[2 * 177], 0x9B, { 0, 0 } },
    { (s32)&DatIndex[2 * 285], 2, { 0, 0 }, (s32)&DatIndex[2 * 176], (s32)&DatIndex[2 * 176], 0xC8, { 0, 0 } },
    { (s32)&DatIndex[2 * 290], 2, { 0, 0 }, (s32)&DatIndex[2 * 166], (s32)&DatIndex[2 * 166], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 294], 2, { 0, 0 }, (s32)&DatIndex[2 * 272], (s32)&DatIndex[2 * 272], 0x78, { 0, 0 } },
    { (s32)&DatIndex[2 * 303], 2, { 0, 0 }, (s32)&DatIndex[2 * 273], (s32)&DatIndex[2 * 273], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 303], 2, { 0, 0 }, (s32)&DatIndex[2 * 273], (s32)&DatIndex[2 * 273], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 303], 2, { 0, 0 }, (s32)&DatIndex[2 * 273], (s32)&DatIndex[2 * 273], 0x88, { 0, 0 } },
    { (s32)&DatIndex[2 * 288], 2, { 0, 0 }, (s32)&DatIndex[2 * 181], (s32)&DatIndex[2 * 181], 0x9C, { 0, 0 } },
    { (s32)&DatIndex[2 * 301], 0, { 0, 0 }, (s32)&DatIndex[2 * 273], (s32)&DatIndex[2 * 273], 0x78, { 0, 0 } },
};
s32 D_80058F0C[14][8] = {
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 188], (s32)&DatIndex[2 * 189], (s32)&DatIndex[2 * 187], (s32)&DatIndex[2 * 186],
        (s32)&DatIndex[2 * 183], (s32)&DatIndex[2 * 182], (s32)&DatIndex[2 * 185], (s32)&DatIndex[2 * 184],
    },
    {
        (s32)&DatIndex[2 * 196], (s32)&DatIndex[2 * 197], (s32)&DatIndex[2 * 195], (s32)&DatIndex[2 * 194],
        (s32)&DatIndex[2 * 191], (s32)&DatIndex[2 * 190], (s32)&DatIndex[2 * 193], (s32)&DatIndex[2 * 192],
    },
    {
        (s32)&DatIndex[2 * 196], (s32)&DatIndex[2 * 197], (s32)&DatIndex[2 * 195], (s32)&DatIndex[2 * 194],
        (s32)&DatIndex[2 * 191], (s32)&DatIndex[2 * 190], (s32)&DatIndex[2 * 193], (s32)&DatIndex[2 * 192],
    },
    {
        (s32)&DatIndex[2 * 204], (s32)&DatIndex[2 * 205], (s32)&DatIndex[2 * 203], (s32)&DatIndex[2 * 202],
        (s32)&DatIndex[2 * 199], (s32)&DatIndex[2 * 198], (s32)&DatIndex[2 * 201], (s32)&DatIndex[2 * 200],
    },
    {
        (s32)&DatIndex[2 * 196], (s32)&DatIndex[2 * 197], (s32)&DatIndex[2 * 195], (s32)&DatIndex[2 * 194],
        (s32)&DatIndex[2 * 191], (s32)&DatIndex[2 * 190], (s32)&DatIndex[2 * 193], (s32)&DatIndex[2 * 192],
    },
    {
        (s32)&DatIndex[2 * 204], (s32)&DatIndex[2 * 205], (s32)&DatIndex[2 * 203], (s32)&DatIndex[2 * 202],
        (s32)&DatIndex[2 * 199], (s32)&DatIndex[2 * 198], (s32)&DatIndex[2 * 201], (s32)&DatIndex[2 * 200],
    },
    {
        (s32)&DatIndex[2 * 188], (s32)&DatIndex[2 * 189], (s32)&DatIndex[2 * 187], (s32)&DatIndex[2 * 186],
        (s32)&DatIndex[2 * 183], (s32)&DatIndex[2 * 182], (s32)&DatIndex[2 * 185], (s32)&DatIndex[2 * 184],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
    {
        (s32)&DatIndex[2 * 212], (s32)&DatIndex[2 * 213], (s32)&DatIndex[2 * 211], (s32)&DatIndex[2 * 210],
        (s32)&DatIndex[2 * 207], (s32)&DatIndex[2 * 206], (s32)&DatIndex[2 * 209], (s32)&DatIndex[2 * 208],
    },
};
s32 FirstBootDone = 0;
SfxPool SfxVoicePool = {0};
s32 D_80059618[20] = {
    0x6253414E, 0x646C6975, 0x20666F20, 0x343A3030, 0x36353A32, 0x206E6F20, 0x2074634F,
    0x32203031, 0xA303030, 0x6D6F4320, 0x656C6970, 0x65762072, 0x6F697372, 0x2E32206E,
    0x20312E38, 0x32334E53, 0x49554220, 0x3420444C, 0x302E302E, 0x303130,
};
s32 D_80059668[982] = {
    (s32)&PauseTextContinue, (s32)&PauseTextShowRules, (s32)&PauseTextOptions,
    (s32)&PauseTextChangeArena, (s32)&PauseTextChangeArena2, (s32)&PauseTextQuitGame,
    (s32)&PauseTextExitArena, (s32)&PauseTextExitArena2, (s32)&PauseTextReturnToWarpRoom,
    (s32)&PauseTextReturnToWarpRoom2, (s32)&PauseTextNewGame, (s32)&PauseTextReplayThisArena,
    (s32)&PauseTextCheatMenu, (s32)&PauseTextLevelSelector, (s32)&PauseTextQuitGame2,
    (s32)&PauseTextYesNo, (s32)&PauseTextYes, (s32)&PauseTextNo, (s32)&PauseTextVibration,
    (s32)&PauseTextOn, (s32)&PauseTextOff, (s32)&PauseTextCheatFlags,
    (s32)&PauseTextTweakSettings, (s32)&PauseTextTweakAwards, (s32)&PauseTextViewStats,
    (s32)&PauseTextAdjustLevel, (s32)&PauseTextController1IsUnplugged,
    (s32)&PauseTextController2IsUnplugged, (s32)&PauseTextController1AIsUnplugged,
    (s32)&PauseTextController1BIsUnplugged, (s32)&PauseTextController1CIsUnplugged,
    (s32)&PauseTextController1DIsUnplugged, (s32)&PauseTextPleaseInsertAController,
    (s32)&PauseTextEmpty, (s32)&PauseTextEmpty, (s32)&PauseTextDemo, (s32)&PauseText01,
    (s32)&PauseText02, (s32)&PauseText03, (s32)&PauseTextSound, (s32)&PauseTextSoundOptions,
    (s32)&PauseTextAudioMode, (s32)&PauseTextMusicVolume, (s32)&PauseTextFxVolume,
    (s32)&PauseTextController, (s32)&PauseTextStereo, (s32)&PauseTextMono,
    (s32)&PauseText100Percent, (s32)&PauseText090Percent, (s32)&PauseTextExit,
    (s32)&PauseTextPressButtonToAccept, (s32)&PauseTextPressButtonToChange, (s32)&PauseTextP1,
    (s32)&PauseTextP2, (s32)&PauseTextP3, (s32)&PauseTextP4, (s32)&D_80059668,
    (s32)&D_80059668, (s32)&D_80059668, (s32)&D_80059668, (s32)&D_80059668, (s32)0xFFF60002,
    0xEA00EA, 0, 0, 0x8080, 0, (s32)0x80000004, 0, 0xD0FF, 0xFF, 0, 0, (s32)0x80000000, 0x46,
    0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x1006E, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    (s32)0x80000000, 0x20096, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x600BE, 0xD0FF,
    0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000001, 0xC00FA, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00, 0xFFFF,
    0, 0, 0, 0, 0, (s32)0xFFF60002, 0xEA00EA, 0, 0, 0x8080, 0, (s32)0x80000004, 0, 0xD0FF,
    0xFF, 0, 0, (s32)0x80000000, 0x46, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000,
    0x1006E, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x20096, 0xD0FF, 0xFF, 0xFFFFFF,
    0xFFFF, (s32)0x80000000, 0x500BE, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000001, 0xC00FA,
    0xFF, 0xFF, 0xFFFF00, 0xFFFF00, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002, 0xEA00EA, 0, 0,
    0x8080, 0, (s32)0x80000004, 0, 0xD0FF, 0xFF, 0, 0, (s32)0x80000000, 0x46, 0xD0FF, 0xFF,
    0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x10064, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    (s32)0x80000000, 0x20082, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x300A0, 0xD0FF,
    0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x500BE, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    (s32)0x80000001, 0xC00FA, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00, 0xFFFF, 0, 0, 0, 0, 0,
    (s32)0xFFF60002, 0xC600C6, 0, 0, 0x8080, 0, (s32)0x80000004, 0, 0xD0FF, 0xFF, 0, 0,
    (s32)0x80000000, 0x46, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x2006E, 0xD0FF,
    0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x50096, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    (s32)0x80000001, 0xC00FA, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00, 0xFFFF, 0, 0, 0, 0, 0,
    (s32)0x80000004, 0x10000, 0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0x80000004, 0,
    0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002, 0x960096, 0, 0, 0x8080, 0,
    (s32)0x80000005, 0xE0014, 0xD0FF, 0xFF, 0, 0, (s32)0x80000000, 0x100050, 0xD0FF, 0xFF,
    0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x110050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0,
    0, 0, 0, (s32)0xFFF60002, 0x960096, 0, 0, 0x8080, 0, (s32)0x80000005, 0x40014, 0xD0FF,
    0xFF, 0, 0, (s32)0x80000000, 0x100050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000,
    0x110050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002, 0x960096,
    0, 0, 0x8080, 0, (s32)0x80000005, 0x90014, 0xD0FF, 0xFF, 0, 0, (s32)0x80000000, 0x100050,
    0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x110050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002, 0x960096, 0, 0, 0x8080, 0, (s32)0x80000005,
    0x70014, 0xD0FF, 0xFF, 0, 0, (s32)0x80000000, 0x100050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    (s32)0x80000000, 0x110050, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0,
    (s32)0xFFF60002, 0xCE00CE, 0, 0, 0x8080, 0, (s32)0x80000005, 0x2000A, 0xD0FF, 0xFF, 0, 0,
    (s32)0x80000000, 0x270046, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x2C006E,
    0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x310096, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60007, 0xFA00FA, 0, 0, 0x8080, 0, (s32)0x80000005,
    0x27000A, 0xD0FF, 0xFF, 0, 0, 0x640000, 0x29004B, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    0x186000A, 0x2D004B, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0x1860014, 0x2E004B, 0xD0FF, 0xFF,
    0xFFFFFF, 0xFFFF, 0x640000, 0x2A0073, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0x19A000B, 0x2F0073,
    0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0x640000, 0x2B009B, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF,
    0x19A000C, 0x30009B, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0x10006, 0, 0, 0, 0, 0,
    (s32)0x80000005, 0x3200CE, 0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002,
    0x1040104, 0, 0, 0x8080, 0, (s32)0x80000005, 0x2C000A, 0xD0FF, 0xFF, 0, 0, (s32)0x80000005,
    0x12003C, 0xD0FF, 0xFF, 0, 0, 0x10006, 0, 0, 0, 0, 0, 0x9B0005, 0x340062, 0xD0FF, 0xFF, 0,
    0, 0xFF0005, 0x350062, 0xD0FF, 0xFF, 0, 0, 0x1630005, 0x360062, 0xD0FF, 0xFF, 0, 0,
    0x1C70005, 0x370062, 0xD0FF, 0xFF, 0, 0, 6, 0, 0, 0, 0, 0, 0x93000A, 0x130072, 0xFFFFFF,
    0xFFFF, 0xFFFFFF, 0xFFFF, 0x8C0014, 0x140072, 0xD0FF, 0xFF, 0xD0FF, 0xFF, 0x8C001E,
    0x140072, 0x808080, 0x808080, 0, 0, 0xF7000B, 0x130072, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF,
    0xF00015, 0x140072, 0xD0FF, 0xFF, 0xD0FF, 0xFF, 0xF0001F, 0x140072, 0x808080, 0x808080, 0,
    0, 0x15B000C, 0x130072, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF, 0x1540016, 0x140072, 0xD0FF,
    0xFF, 0xD0FF, 0xFF, 0x1540020, 0x140072, 0x808080, 0x808080, 0, 0, 0x1BF000D, 0x130072,
    0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF, 0x1B80017, 0x140072, 0xD0FF, 0xFF, 0xD0FF, 0xFF,
    0x1B80021, 0x140072, 0x808080, 0x808080, 0, 0, 0x10006, 0, 0, 0, 0, 0, (s32)0x80000005,
    0x3300AA, 0xD0FF, 0xFF, 0, 0, (s32)0x80000005, 0x3200D2, 0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0,
    0, 0, 0, (s32)0xFFF60002, 0x1400140, 0, 0, 0x8080, 0, (s32)0x80000005, 0xC000A, 0xD0FF,
    0xFF, 0, 0, (s32)0x80000000, 0xD0032, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000,
    0x15005A, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x160082, 0xD0FF, 0xFF,
    0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x1700AA, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0,
    0, 0, 0, (s32)0x80000000, 0xB003C, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000,
    0x10064, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x3008C, 0xD0FF, 0xFF, 0xFFFFFF,
    0xFFFF, (s32)0x80000000, 0x500B4, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0,
    (s32)0x80000000, 0xB00C8, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x800FA, 0xD0FF,
    0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, 0xAA0002, 0x620062, 0, 0, 0x8080, 0,
    (s32)0x80000000, 0xB00B4, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF, (s32)0x80000000, 0x800E6, 0xD0FF,
    0xFF, 0xFFFFFF, 0xFFFF, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFEC0002, 0x14A014A, 0, 0, 0x80, 0,
    (s32)0x80000005, 0x16FFF6, 0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFEC0002,
    0x14A014A, 0, 0, 0x80, 0, (s32)0x80000005, 0xCFFF6, 0xD0FF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0,
    0, (s32)0xFFEC0002, 0x14A014A, 0, 0, 0x80, 0, (s32)0x80000005, 0x17FFF6, 0xD0FF, 0xFF, 0,
    0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFB00002, 0x1860186, 0, 0, 0x80, 0, 0xFFFF, 0, 0, 0, 0, 0,
    (s32)0xFFEC0002, 0x14A014A, 0, 0, 0x80, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002,
    0x1400140, 0, 0, 0x8080, 0, (s32)0x80000004, 0, 0xD0FF, 0xFF, 0, 0, 0x10006, 0, 0, 0, 0, 0,
    (s32)0x80000005, 0x1C0050, 0xD0FF, 0xFF, 0, 0, (s32)0x80000005, 0x1D0062, 0xD0FF, 0xFF, 0,
    0, (s32)0x80000005, 0x1E0074, 0xD0FF, 0xFF, 0, 0, (s32)0x80000005, 0x1F0086, 0xD0FF, 0xFF,
    0, 0, (s32)0x80000005, 0x200098, 0xD0FF, 0xFF, 0, 0, 6, 0, 0, 0, 0, 0, 0xFFFF, 0, 0, 0, 0,
    0, (s32)0xFFF60002, 0xC800C8, 0, 0, 0x8080, 0, (s32)0x80000005, 0x210014, 0xFF, 0xFF, 0, 0,
    0xFFFF, 0, 0, 0, 0, 0, (s32)0xFFF60002, 0xC800C8, 0, 0, 0x8080, 0, (s32)0x80000005,
    0x220014, 0xFF, 0xFF, 0, 0, 0xFFFF, 0, 0, 0, 0, 0, (s32)0x80000005, 0x230014, 0xD0FF, 0xFF,
    0, 0, 0xFFFF, 0, 0, 0, 0, 0, 4, 0x1908000, 0x280000, 0, 0, 0, 0, 0, 0, 3, 0xC8000, 0, 0, 0,
    (s32)&PauseTextPressButtonToForceVictory, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
