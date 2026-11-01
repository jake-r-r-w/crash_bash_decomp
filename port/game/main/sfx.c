#include "common.h"
#include "bash/main.h"
#include "bash/sfx.h"

void music_play_level(s32 arg0, s32 arg1) {
    GameState *p;
    u8 *q;
    u32 t;
    u32 v;
    s32 i;
    file *f;
    s32 m;
    u32 c;
    u16 *d;

    p = (GameState *)GameInfo;
    if (p->mode != 0x28) {
        sfx_table_clear();
        if (p->levelFlags & 0x80) {
            f = MusicTable[arg0].altFile;
        } else {
            f = MusicTable[arg0].file2;
        }
        sfx_bank_load(f);
    }
    q = &DatIndex[278];
    d = ModeTypes;
    m = ((GameState *)GameInfo)->mode;
    c = d[m * 3] - 0x101;
    f = (file *)(q - 0x10);
    if (c >= 2U) {
        f = (file *)(q - 8);
        if (!(((GameState *)GameInfo)->levelFlags & 0x1E0)) {
            f = (file *)q;
        }
    }
    sfx_bank_load(f);
    t = arg1 - 0x101;
    if (t >= 2U) {
        arg1 = t;
        for (i = 0; i < 4; i++) {
            v = GAME->heroes[i] - 1;
            if (v < 8U) {
                sfx_bank_load(LevelSfxBanks[arg1][v]);
            }
        }
    }
}

void sound_set_stereo(void) {
    audio_set_mono(0);
}

void sound_set_mono(void) {
    audio_set_mono(1);
}

void sound_init(void) {
    audio_reset();
    music_init();
    sfx_voices_init();
}

void sound_reinit(void) {
    sfx_banks_close();
    audio_reset();
    music_free();
}

s32 sfx_update_voice(_sfx_handler *voice) {
    S16Pair vol;
    S16Pair tmp;
    S16Pair tmp2;
    s32 f;
    SLONG_XYZ *pos = voice->pos;
    s32 *slot = &voice->slot;

    if (voice->voice != -1) {
        if (AudioVoiceStates[voice->slot] != 1) {
            return 0;
        }
        f = voice->flags & 0xE;
        voice->flags &= ~8;
        if (pos != NULL) {
            f |= 8;
            tmp = sfx_position_volume((VECTOR *)pos);
            vol = tmp;
        } else {
            u16 t = voice->volume;
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
            sfx_change_pitch((PitchVoice *)slot, voice->pitch);
            voice->flags &= ~1;
        }
    } else {
        if (pos != NULL) {
            tmp2 = sfx_position_volume((VECTOR *)pos);
            vol = tmp2;
        } else {
            u16 t = voice->volume;
            vol.b = t;
            vol.a = t;
        }
        voice->voice = sfx_key_on(&voice->slot, voice->id, voice->pitch, vol.a, vol.b);
    }
    return voice->voice != -1;
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
        sfx_key_off(&v->slot);
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
        sfx_key_off(&voice->slot);
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
            voice->voice = -1;
            voice->slot = -1;
        }
        if (voice != NULL) {
            voice->next = SfxVoicePool.active;
            voice->prev = NULL;
            SfxVoicePool.active = voice;
            voice->id = id;
            voice->volume = arg3;
            voice->pitch = arg4;
            voice->flags = flags;
            voice->voice = -1;
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
        voice->pitch = arg1;
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
    r->voice = -1;
    r->slot = -1;
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

/* data, in address order (tools/scripts/data2c.py) */
/* what the data points at that this file's headers don't declare */
extern const char PauseText01[];
extern const char PauseText02[];
extern const char PauseText03[];
extern char PauseText090Percent[];
extern char PauseText100Percent[];
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
    { &DatIndex[293], 2, { 0, 0 }, &DatIndex[167], &DatIndex[167], 0x9A, { 0, 0 } },
    { &DatIndex[297], 2, { 0, 0 }, &DatIndex[166], &DatIndex[166], 0x88, { 0, 0 } },
    { &DatIndex[289], 2, { 0, 0 }, &DatIndex[165], &DatIndex[165], 0xE1, { 0, 0 } },
    { &DatIndex[298], 2, { 0, 0 }, &DatIndex[164], &DatIndex[164], 0x90, { 0, 0 } },
    { &DatIndex[290], 2, { 0, 0 }, &DatIndex[163], &DatIndex[163], 0x88, { 0, 0 } },
    { &DatIndex[283], 2, { 0, 0 }, &DatIndex[159], &DatIndex[159], 0x90, { 0, 0 } },
    { &DatIndex[281], 2, { 0, 0 }, &DatIndex[162], &DatIndex[162], 0x74, { 0, 0 } },
    { &DatIndex[282], 2, { 0, 0 }, &DatIndex[160], &DatIndex[160], 0x79, { 0, 0 } },
    { &DatIndex[280], 2, { 0, 0 }, &DatIndex[161], &DatIndex[161], 0x6F, { 0, 0 } },
    { &DatIndex[280], 2, { 0, 0 }, &DatIndex[158], &DatIndex[158], 0x6F, { 0, 0 } },
    { &DatIndex[299], 2, { 0, 0 }, &DatIndex[171], &DatIndex[171], 0x6C, { 0, 0 } },
    { &DatIndex[286], 2, { 0, 0 }, &DatIndex[170], &DatIndex[170], 0xB2, { 0, 0 } },
    { &DatIndex[291], 2, { 0, 0 }, &DatIndex[169], &DatIndex[169], 0xA0, { 0, 0 } },
    { &DatIndex[454], 2, { 0, 0 }, &DatIndex[168], &DatIndex[168], 0x5F, { 0, 0 } },
    { &DatIndex[296], 2, { 0, 0 }, &DatIndex[157], &DatIndex[157], 0xA0, { 0, 0 } },
    { &DatIndex[287], 2, { 0, 0 }, &DatIndex[156], &DatIndex[156], 0x8C, { 0, 0 } },
    { &DatIndex[286], 2, { 0, 0 }, &DatIndex[155], &DatIndex[155], 0xB2, { 0, 0 } },
    { &DatIndex[302], 2, { 0, 0 }, &DatIndex[154], &DatIndex[154], 0x83, { 0, 0 } },
    { &DatIndex[279], 2, { 0, 0 }, &DatIndex[450], &DatIndex[449], 0xD7, { 0, 0 } },
    { &DatIndex[295], 2, { 0, 0 }, &DatIndex[451], &DatIndex[451], 0xA0, { 0, 0 } },
    { &DatIndex[298], 2, { 0, 0 }, &DatIndex[448], &DatIndex[448], 0x90, { 0, 0 } },
    { &DatIndex[453], 2, { 0, 0 }, &DatIndex[447], &DatIndex[447], 0x83, { 0, 0 } },
    { &DatIndex[452], 2, { 0, 0 }, &DatIndex[446], &DatIndex[446], 0x9D, { 0, 0 } },
    { &DatIndex[285], 2, { 0, 0 }, &DatIndex[175], &DatIndex[175], 0xC8, { 0, 0 } },
    { &DatIndex[284], 2, { 0, 0 }, &DatIndex[174], &DatIndex[174], 0xBE, { 0, 0 } },
    { &DatIndex[285], 2, { 0, 0 }, &DatIndex[173], &DatIndex[173], 0xC8, { 0, 0 } },
    { &DatIndex[283], 2, { 0, 0 }, &DatIndex[172], &DatIndex[172], 0x90, { 0, 0 } },
    { &DatIndex[279], 2, { 0, 0 }, &DatIndex[180], &DatIndex[180], 0xD7, { 0, 0 } },
    { &DatIndex[283], 2, { 0, 0 }, &DatIndex[179], &DatIndex[179], 0x90, { 0, 0 } },
    { &DatIndex[292], 3, { 0, 0 }, &DatIndex[178], &DatIndex[178], 0x9B, { 0, 0 } },
    { &DatIndex[292], 2, { 0, 0 }, &DatIndex[177], &DatIndex[177], 0x9B, { 0, 0 } },
    { &DatIndex[285], 2, { 0, 0 }, &DatIndex[176], &DatIndex[176], 0xC8, { 0, 0 } },
    { &DatIndex[290], 2, { 0, 0 }, &DatIndex[166], &DatIndex[166], 0x88, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[294], 2, { 0, 0 }, &DatIndex[272], &DatIndex[272], 0x78, { 0, 0 } },
    { &DatIndex[303], 2, { 0, 0 }, &DatIndex[273], &DatIndex[273], 0x88, { 0, 0 } },
    { &DatIndex[303], 2, { 0, 0 }, &DatIndex[273], &DatIndex[273], 0x88, { 0, 0 } },
    { &DatIndex[303], 2, { 0, 0 }, &DatIndex[273], &DatIndex[273], 0x88, { 0, 0 } },
    { &DatIndex[288], 2, { 0, 0 }, &DatIndex[181], &DatIndex[181], 0x9C, { 0, 0 } },
    { &DatIndex[301], 0, { 0, 0 }, &DatIndex[273], &DatIndex[273], 0x78, { 0, 0 } },
};
file *LevelSfxBanks[14][8] = {
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[188], &DatIndex[189], &DatIndex[187], &DatIndex[186],
        &DatIndex[183], &DatIndex[182], &DatIndex[185], &DatIndex[184],
    },
    {
        &DatIndex[196], &DatIndex[197], &DatIndex[195], &DatIndex[194],
        &DatIndex[191], &DatIndex[190], &DatIndex[193], &DatIndex[192],
    },
    {
        &DatIndex[196], &DatIndex[197], &DatIndex[195], &DatIndex[194],
        &DatIndex[191], &DatIndex[190], &DatIndex[193], &DatIndex[192],
    },
    {
        &DatIndex[204], &DatIndex[205], &DatIndex[203], &DatIndex[202],
        &DatIndex[199], &DatIndex[198], &DatIndex[201], &DatIndex[200],
    },
    {
        &DatIndex[196], &DatIndex[197], &DatIndex[195], &DatIndex[194],
        &DatIndex[191], &DatIndex[190], &DatIndex[193], &DatIndex[192],
    },
    {
        &DatIndex[204], &DatIndex[205], &DatIndex[203], &DatIndex[202],
        &DatIndex[199], &DatIndex[198], &DatIndex[201], &DatIndex[200],
    },
    {
        &DatIndex[188], &DatIndex[189], &DatIndex[187], &DatIndex[186],
        &DatIndex[183], &DatIndex[182], &DatIndex[185], &DatIndex[184],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
    {
        &DatIndex[212], &DatIndex[213], &DatIndex[211], &DatIndex[210],
        &DatIndex[207], &DatIndex[206], &DatIndex[209], &DatIndex[208],
    },
};
s32 FirstBootDone = 0;
SfxPool SfxVoicePool = {0};
/* the build's stamp (the library's own banner) */
char NasBuildStamp[80] = "NASbuild of 00:42:56 on Oct 10 2000\n Compiler version 2.8.1 SN32 BUILD 4.0.0010";
void *PauseTexts[56] = {
    &PauseTextContinue, &PauseTextShowRules, &PauseTextOptions, &PauseTextChangeArena, &PauseTextChangeArena2, &PauseTextQuitGame, &PauseTextExitArena, &PauseTextExitArena2, &PauseTextReturnToWarpRoom, &PauseTextReturnToWarpRoom2, &PauseTextNewGame, &PauseTextReplayThisArena, &PauseTextCheatMenu, &PauseTextLevelSelector, &PauseTextQuitGame2, &PauseTextYesNo, &PauseTextYes, &PauseTextNo, &PauseTextVibration, &PauseTextOn, &PauseTextOff, &PauseTextCheatFlags, &PauseTextTweakSettings, &PauseTextTweakAwards, &PauseTextViewStats, &PauseTextAdjustLevel, &PauseTextController1IsUnplugged, &PauseTextController2IsUnplugged, &PauseTextController1AIsUnplugged, &PauseTextController1BIsUnplugged, &PauseTextController1CIsUnplugged, &PauseTextController1DIsUnplugged, &PauseTextPleaseInsertAController, &PauseTextEmpty, &PauseTextEmpty, &PauseTextDemo, &PauseText01, &PauseText02, &PauseText03, &PauseTextSound, &PauseTextSoundOptions, &PauseTextAudioMode, &PauseTextMusicVolume, &PauseTextFxVolume, &PauseTextController, &PauseTextStereo, &PauseTextMono, &PauseText100Percent, &PauseText090Percent, &PauseTextExit, &PauseTextPressButtonToAccept, &PauseTextPressButtonToChange, &PauseTextP1, &PauseTextP2, &PauseTextP3, &PauseTextP4,
};
void *LangMessages[5] = { &PauseTexts, &PauseTexts, &PauseTexts, &PauseTexts, &PauseTexts };
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
/* The short one with a row to save the game, in a warp room. */
HudEntry PauseMenuSave[9] = {
    { 2, -0xA, 0xC6, 0xE0, 0, 0, 0x8080, 0 },
    { 4, -0x8000, 0, 0, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x3C, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x58, 2, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 8, -0x8000, 0x74, 0, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x90, 5, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 1, -0x8000, 0xAC, 0xC, 0xFF, 0xFF, 0xFFFF00, 0xFFFF00 },
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
HudEntry PauseOptionsMenu[7] = {
    { 2, -0xA, 0xCE, 0xCE, 0, 0, 0x8080, 0 },
    { 5, -0x8000, 0xA, 2, 0xD0FF, 0xFF, 0, 0 },
    { 0, -0x8000, 0x3E, 0x27, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0x60, 0x2C, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 8, -0x8000, 0x82, 1, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { 0, -0x8000, 0xA4, 0x31, 0xD0FF, 0xFF, 0xFFFFFF, 0xFFFF },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};
/* The rows are drawn by DrawPauseVideo, from the shared options. */
HudEntry PauseVideoMenu[2] = {
    { 7, -0xA, 0xFA, 0xFA, 0, 0, 0x8080, 0 },
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
PlaceRec HeroesOutPanel[3] = {
    { 4, -0x8000, 0x190, 0, 0x28, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 3, -0x8000, 0xC, 0, 0, 0, 0, 0, 0, &PauseTextPressButtonToForceVictory, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};