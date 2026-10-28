#include "common.h"
#include "bash/main.h"
#include "bash/sfx.h"

void music_play_level(s32 arg0, s32 arg1) {
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
        sfx_table_clear();
        if (p->flags & 0x80) {
            f = MusicTable[arg0].unkC;
        } else {
            f = MusicTable[arg0].unk8;
        }
        sfx_bank_load(f);
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
    sfx_bank_load(f);
    t = arg1 - 0x101;
    if (t >= 2U) {
        arg1 = t;
        for (i = 0; i < 4; i++) {
            v = GameInfo[i + 0x50] - 1;
            if (v < 8U) {
                sfx_bank_load(LevelSfxBanks[arg1][v]);
            }
        }
    }
}

void sound_set_stereo(void) {
    SsSetStereo();
}

void sound_set_mono(void) {
    SsSetMono();
}

void sound_init(void) {
    SsInit();
    music_init();
    sfx_voices_init();
}

void sound_reinit(void) {
    sfx_banks_close();
    SsInit();
    music_free();
}

s32 sfx_update_voice(_sfx_handler *voice) {
    S16Pair vol;
    S16Pair tmp;
    S16Pair tmp2;
    s32 f;
    SLONG_XYZ *pos = voice->pos;
    s32 *slot = &voice->unk2C;

    if (voice->unk28 != -1) {
        if (SpuKeyStatus[voice->unk2C] != 1) {
            return 0;
        }
        f = voice->flags & 0xE;
        voice->flags &= ~8;
        if (pos != NULL) {
            f |= 8;
            tmp = sfx_position_volume((VECTOR *)pos);
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
            sfx_set_voice_volume((s16 *)slot, vol.a, vol.b);
        }
        if (voice->flags & 1) {
            sfx_change_pitch((PitchVoice *)slot, voice->unk24);
            voice->flags &= ~1;
        }
    } else {
        if (pos != NULL) {
            tmp2 = sfx_position_volume((VECTOR *)pos);
            vol = tmp2;
        } else {
            u16 t = voice->unk20;
            vol.b = t;
            vol.a = t;
        }
        voice->unk28 = sfx_key_on(&voice->unk2C, voice->id, voice->unk24, vol.a, vol.b);
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
        sfx_key_off(&v->unk2C);
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
        sfx_key_off(&voice->unk2C);
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
        loader_pending_stub();
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

void sfx_resume_all(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 4;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void sfx_pause_all(void) {
    _sfx_handler *voice;

    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            voice->flags |= 2;
            voice = voice->next;
        } while (voice != NULL);
    }
}

void sfx_update_all(void) {
    _sfx_handler *next;
    _sfx_handler *voice;

    vab_transfer_poll();
    voice = SfxVoicePool.active;
    if (voice != NULL) {
        do {
            next = voice->next;
            if (sfx_update_voice(voice) != 1) {
                StopSfx(voice);
            }
            voice = next;
        } while (voice != NULL);
    }
}

void sfx_set_pitch(_sfx_handler *voice, s32 arg1) {
    if (voice != NULL) {
        voice->unk24 = arg1;
        voice->flags |= 1;
    }
}

void sfx_voice_free(_sfx_handler *voice) {
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

_sfx_handler *sfx_voice_alloc(void) {
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

void sfx_voices_init(void) {
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
    sfx_table_init();
}


void sfx_reset(void) {
    sfx_table_clear();
    sfx_voices_init();
}

void music_load_track(s32 i) {
    music_load(MusicTable[i].file, MusicTable[i].reverb);
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
s32 LevelSfxBanks[14][8] = {
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
s32 PauseTexts[56] = {
    (s32)&PauseTextContinue, (s32)&PauseTextShowRules, (s32)&PauseTextOptions, (s32)&PauseTextChangeArena, (s32)&PauseTextChangeArena2, (s32)&PauseTextQuitGame, (s32)&PauseTextExitArena, (s32)&PauseTextExitArena2, (s32)&PauseTextReturnToWarpRoom, (s32)&PauseTextReturnToWarpRoom2, (s32)&PauseTextNewGame, (s32)&PauseTextReplayThisArena, (s32)&PauseTextCheatMenu, (s32)&PauseTextLevelSelector, (s32)&PauseTextQuitGame2, (s32)&PauseTextYesNo, (s32)&PauseTextYes, (s32)&PauseTextNo, (s32)&PauseTextVibration, (s32)&PauseTextOn, (s32)&PauseTextOff, (s32)&PauseTextCheatFlags, (s32)&PauseTextTweakSettings, (s32)&PauseTextTweakAwards, (s32)&PauseTextViewStats, (s32)&PauseTextAdjustLevel, (s32)&PauseTextController1IsUnplugged, (s32)&PauseTextController2IsUnplugged, (s32)&PauseTextController1AIsUnplugged, (s32)&PauseTextController1BIsUnplugged, (s32)&PauseTextController1CIsUnplugged, (s32)&PauseTextController1DIsUnplugged, (s32)&PauseTextPleaseInsertAController, (s32)&PauseTextEmpty, (s32)&PauseTextEmpty, (s32)&PauseTextDemo, (s32)&PauseText01, (s32)&PauseText02, (s32)&PauseText03, (s32)&PauseTextSound, (s32)&PauseTextSoundOptions, (s32)&PauseTextAudioMode, (s32)&PauseTextMusicVolume, (s32)&PauseTextFxVolume, (s32)&PauseTextController, (s32)&PauseTextStereo, (s32)&PauseTextMono, (s32)&PauseText100Percent, (s32)&PauseText090Percent, (s32)&PauseTextExit, (s32)&PauseTextPressButtonToAccept, (s32)&PauseTextPressButtonToChange, (s32)&PauseTextP1, (s32)&PauseTextP2, (s32)&PauseTextP3, (s32)&PauseTextP4,
};
s32 LangMessages[5] = { (s32)&PauseTexts, (s32)&PauseTexts, (s32)&PauseTexts, (s32)&PauseTexts, (s32)&PauseTexts };
HudEntry PauseMenuExitArena[8] = {
    { 2, -0xA, 0xEA, 0xEA, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x46, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x6E, 1, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x96, 2, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xBE, 6, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 1, -0x8000, 0xFA, 0xC, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseMenuQuit[8] = {
    { 2, -0xA, 0xEA, 0xEA, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x46, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x6E, 1, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x96, 2, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xBE, 5, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 1, -0x8000, 0xFA, 0xC, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseMenuChangeArena[9] = {
    { 2, -0xA, 0xEA, 0xEA, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x46, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x64, 1, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x82, 2, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xA0, 3, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xBE, 5, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 1, -0x8000, 0xFA, 0xC, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseMenuShort[7] = {
    { 2, -0xA, 0xC6, 0xC6, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x46, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x6E, 2, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x96, 5, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 1, -0x8000, 0xFA, 0xC, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseRules1[2] = {
    { 4, -0x8000, 0, 1, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseRules0[2] = {
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseConfirmMenus[15] = {
    { 2, -0xA, 0x96, 0x96, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 0xE, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x50, 0x10, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x50, 0x11, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0xA, 0x96, 0x96, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 4, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x50, 0x10, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x50, 0x11, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0xA, 0x96, 0x96, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 9, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x50, 0x10, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x50, 0x11, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseConfirmExit[5] = {
    { 2, -0xA, 0x96, 0x96, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 7, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x50, 0x10, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x50, 0x11, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseOptionsMenu[6] = {
    { 2, -0xA, 0xCE, 0xCE, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0xA, 2, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x46, 0x27, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x6E, 0x2C, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x96, 0x31, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseSoundMenu[12] = {
    { 7, -0xA, 0xFA, 0xFA, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0xA, 0x27, 0xD0FF, 0xFF, 0, 0 },
    { 0, 0x64, 0x4B, 0x29, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0xA, 0x186, 0x4B, 0x2D, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0x14, 0x186, 0x4B, 0x2E, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, 0x64, 0x73, 0x2A, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0xB, 0x19A, 0x73, 0x2F, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, 0x64, 0x9B, 0x2B, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0xC, 0x19A, 0x9B, 0x30, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 6, 1, 0, 0, 0, 0, 0, 0 },
    { 5, -0x8000, 0xCE, 0x32, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseControllerMenu[32] = {
    { 2, -0xA, 0x104, 0x104, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0xA, 0x2C, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0x3C, 0x12, 0xD0FF, 0xFF, 0, 0 },
    { 6, 1, 0, 0, 0, 0, 0, 0 },
    { 5, 0x9B, 0x62, 0x34, 0xD0FF, 0xFF, 0, 0 },
    { 5, 0xFF, 0x62, 0x35, 0xD0FF, 0xFF, 0, 0 },
    { 5, 0x163, 0x62, 0x36, 0xD0FF, 0xFF, 0, 0 },
    { 5, 0x1C7, 0x62, 0x37, 0xD0FF, 0xFF, 0, 0 },
    { 6, 0, 0, 0, 0, 0, 0, 0 },
    { 0xA, 0x93, 0x72, 0x13, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF },
    { 0x14, 0x8C, 0x72, 0x14, 0xD0FF, 0xFF, 0xD0FF, 0xFF },
    { 0x1E, 0x8C, 0x72, 0x14, 0x808080, 0x808080, 0, 0 },
    { 0xB, 0xF7, 0x72, 0x13, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF },
    { 0x15, 0xF0, 0x72, 0x14, 0xD0FF, 0xFF, 0xD0FF, 0xFF },
    { 0x1F, 0xF0, 0x72, 0x14, 0x808080, 0x808080, 0, 0 },
    { 0xC, 0x15B, 0x72, 0x13, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF },
    { 0x16, 0x154, 0x72, 0x14, 0xD0FF, 0xFF, 0xD0FF, 0xFF },
    { 0x20, 0x154, 0x72, 0x14, 0x808080, 0x808080, 0, 0 },
    { 0xD, 0x1BF, 0x72, 0x13, 0xFFFFFF, 0xFFFF, 0xFFFFFF, 0xFFFF },
    { 0x17, 0x1B8, 0x72, 0x14, 0xD0FF, 0xFF, 0xD0FF, 0xFF },
    { 0x21, 0x1B8, 0x72, 0x14, 0x808080, 0x808080, 0, 0 },
    { 6, 1, 0, 0, 0, 0, 0, 0 },
    { 5, -0x8000, 0xAA, 0x33, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0xD2, 0x32, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0xA, 0x140, 0x140, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0xA, 0xC, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x32, 0xD, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x5A, 0x15, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x82, 0x16, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xAA, 0x17, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseEndMenu[5] = {
    { 0, -0x8000, 0x3C, 0xB, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x64, 1, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x8C, 3, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xB4, 5, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseEndWarpMenu[3] = {
    { 0, -0x8000, 0xC8, 0xB, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xFA, 8, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseEndAdventureMenu[17] = {
    { 2, 0xAA, 0x62, 0x62, 0, 0, 0x8080, 0 },
    { 0, -0x8000, 0xB4, 0xB, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xE6, 8, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0x14, 0x14A, 0x14A, 0, 0, 0x80, 0 },
    { 5, -0x8000, -0xA, 0x16, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0x14, 0x14A, 0x14A, 0, 0, 0x80, 0 },
    { 5, -0x8000, -0xA, 0xC, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0x14, 0x14A, 0x14A, 0, 0, 0x80, 0 },
    { 5, -0x8000, -0xA, 0x17, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0x50, 0x186, 0x186, 0, 0, 0x80, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
    { 2, -0x14, 0x14A, 0x14A, 0, 0, 0x80, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PausePadMessage[3] = {
    { 2, -0xA, 0x140, 0x140, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 6, 1, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseUnpluggedMenu[7] = {
    { 5, -0x8000, 0x50, 0x1C, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0x62, 0x1D, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0x74, 0x1E, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0x86, 0x1F, 0xD0FF, 0xFF, 0, 0 },
    { 5, -0x8000, 0x98, 0x20, 0xD0FF, 0xFF, 0, 0 },
    { 6, 0, 0, 0, 0, 0, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseBlankA[3] = {
    { 2, -0xA, 0xC8, 0xC8, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 0x21, 0xFF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseBlankB[3] = {
    { 2, -0xA, 0xC8, 0xC8, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0x14, 0x22, 0xFF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
HudEntry PauseDemoLabel[2] = {
    { 5, -0x8000, 0x14, 0x23, 0xD0FF, 0xFF, 0, 0 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
s32 HeroesOutPanel[27] = {
    4, 0x1908000, 0x280000, 0, 0, 0, 0, 0, 0, 3, 0xC8000, 0, 0, 0, (s32)&PauseTextPressButtonToForceVictory, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};