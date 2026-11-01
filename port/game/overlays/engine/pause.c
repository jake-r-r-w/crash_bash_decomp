#include "../../../platform/settings.h"
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/pause.h"
#include "bash/hero.h"
#include "bash/shell.h"
#include "bash/videomenu.h"
#include "bash/cheatmenu.h"
#include <stdio.h>

/* The rows that aren't the game's own (DrawHudEntries' type 8), by a HudEntry's idx. */
static const char *const PauseOwnTexts[] = { "SAVE GAME", "VIDEO" };

/* A warp room's pause menu can save, as its computer can: the game's state is only ever saved between its games. */
static s32 pause_can_save(void) {
    return WarpRoomIndex(GAME->mode) >= 0;
}

static void pause_go_save(void) {
    PausePad = 0;
    PauseChoice = 0;
    GAME->pause &= 0x04000010;
    sound_resume();
    GAME->exitFlags |= 0x200;
    GotoProcess(&SaveScreenProcess, 0x200);
}

s32 PauseChooseYesNo(s32 pad) {
    if ((((PadState *)Pads)[pad].pressed & 0x80) && PauseChoice != 0) {
        PauseChoice = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadState *)Pads)[pad].pressed & 0x20) && PauseChoice == 0) {
        PauseChoice = 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadState *)Pads)[pad].held & 9) != 9 &&
        (((PadState *)Pads)[pad].pressed & 8)) {
        GameState *p = GAME;
        PausePad = 0;
        PauseChoice = 0;
        p->pause &= 0x04000010;
        sound_resume();
        return 0;
    }
    return ((PadState *)Pads)[pad].pressed & 0x4000;
}

s32 PauseChooseUpDown(s32 pad, s32 max) {
    if ((((PadState *)Pads)[pad].pressed & 0x10) && PauseChoice > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice--;
    }
    if ((((PadState *)Pads)[pad].pressed & 0x40) && PauseChoice < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice++;
    }
    if (PauseChoice < 0) {
        PauseChoice = 0;
    }
    if (PauseChoice > max) {
        PauseChoice = max;
    }
    if ((((PadState *)Pads)[pad].held & 9) != 9 &&
        (((PadState *)Pads)[pad].pressed & 8)) {
        GameState *p = GAME;
        PausePad = 0;
        PauseChoice = 0;
        p->pause &= 0x04000010;
        sound_resume();
        return 0;
    }
    return ((PadState *)Pads)[pad].pressed & 0x4000;
}

s32 ProcessPauseUD(s32 pad, s32 max) {
    if ((((PadState *)Pads)[pad].pressed & 0x10) && PauseChoice > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice--;
    }
    if ((((PadState *)Pads)[pad].pressed & 0x40) && PauseChoice < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice++;
    }
    if (PauseChoice < 0) {
        PauseChoice = 0;
    }
    if (PauseChoice > max) {
        PauseChoice = max;
    }
    return ((PadState *)Pads)[pad].pressed & 0x4008;
}

const char ModeNameBattleMode[] = "Battle Mode";

const char ModeNameTournamentMode[] = "Tournament Mode";

const char ModeNameAdventureTrophy[] = "Adventure - Trophy";

const char ModeNameAdventureGoldRelic[] = "Adventure - Gold Relic";

const char ModeNameAdventurePlatRelic[] = "Adventure - Plat Relic";

const char ModeNameAdventureGem[] = "Adventure - Gem";

const char ModeNameAdventureChallenge[] = "Adventure - Challenge";

const char ModeNameAdventureBoss[] = "Adventure - Boss";
const char ModeNameEmpty[] = "";

static inline PadState *ActivePad(void) {
    PadState *base = HPADS;

    return &base[PausePad];
}

s32 UpdatePause(void) {
    s32 ret;
    s32 kind;
    s32 max;
    s32 i;
    s32 key;
    u16 *tbl;
    s32 n;
    s32 v;
    s32 s;
    s32 r;
    s32 hi;
    s32 on;
    s32 fl;
    PadState *p;
    u8 *g;

    ret = 1;
    /* a boss test's player (--aim): the label a --mode test has, as this isn't a demo */
    if ((settings()->test_aim || settings()->test_adventure) && (GAME->frames & 0x10)) {
        s32 page = FontPage;

        PauseTexts[PAUSE_TEXT_DEMO] = "TESTING";
        FontPage = 0;
        DrawHudEntries(PauseDemoLabel, 0x70);
        FontPage = page;
    }
    if (GAME->pause & 0x10) {
        return 0;
    }
    if ((GAME->exitFlags & 0x8008) && ModeTypes[GAME->mode * 3] != 0x102) {
        return 0;
    }
    if (ModeTypes[GAME->mode * 3] == 0x101 || GAME->mode == 0x2B) {
        /* the unk74 test jumps into this copy: as a second return 1,
           jump2 would fold this one into it */
    ret1:
        return 1;
    }
    if (GAME->adventure.active != 0) {
        goto ret1;
    }
    if (GAME->demo != 0) {
        if (GAME->demoTimer != 0) {
            GAME->demoTimer--;
        } else {
            ExitToTitles();
            GAME->demo = 0;
            GAME->pause &= ~0x02000000;
        }
    }
    if (!(GameLevelFlags & 0x100)) {
        s32 j;
        j = 0;
        key = GameLevelFlags & 0xFF;
        tbl = LevelKindKeys;
        do {
            if (key == *tbl) {
                kind = j;
                goto found;
            }
            j++;
            tbl++;
        } while (j < 7);
        kind = 0;
    } else {
        kind = 7;
    }
found:
    if (!(GamePause & 0x660)) {
        s32 bit = GamePause & 1;
        s32 toggle;

        if (GamePause & 0x01000000) {
            toggle = bit ^ 1;
        } else {
            toggle = bit != 0;
        }
        if (toggle) {
            if (GamePause & 1) {
                pads_reset_rumble();
                sound_pause();
                PlaySfx(0x193, NULL, NULL, 0x1000, 0x1E00, 0);
            } else {
                sound_resume();
            }
        }
        GAME->pause &= ~0x01000000;
    }
    if (GAME->pause & 1) {
        GAME->pause |= 0x01000000;
    }
    if (GAME->pause & 0x08000000) {
        return 0;
    }
    max = 4;
    if (GAME->levelFlags & max) {
        max = 3;
    }
    if (GAME->cheats < 0 && (GAME->cheats & 0x10000)) {
        max++;
    }
    if (GAME->pause & 0x02007001) {
        switch ((u32)GAME->pause & 0x023F8FEC) {
        case 0:
            if (GAME->pause & 0x7000) {
                ret = 0;
                break;
            }
            if (PauseChooseUpDown(PausePad, max) != 0) {
                GAME->pause &= 0xFFC070D3;
                if (GAME->levelFlags & 4) {
                    switch (PauseChoice) {
                    case 0:
                        GAME->pause &= 0x04000010;
                        PausePad = 0;
                        PauseChoice = 0;
                        sound_resume();
                        break;
                    case 1:
                        SetPauseRecs(GAME->mode, kind, 0);
                        GAME->pause |= 0x80;
                        break;
                    case 2:
                        GAME->pause |= 0x100;
                        PauseChoice = 0;
                        PauseLastChoice = 0;
                        break;
                    case 3:
                        if (GAME->mode == 0x20) {
                            GAME->pause |= 4;
                            PauseChoice = 1;
                        } else {
                            GAME->pause |= 0x8000;
                            PauseChoice = 1;
                        }
                        break;
                    case 4:
                        GAME->pause |= 0x10000;
                        PauseChoice = 0;
                        break;
                    }
                } else {
                    switch (PauseChoice) {
                    case 0:
                        PausePad = 0;
                        PauseChoice = 0;
                        GAME->pause &= 0x04000010;
                        sound_resume();
                        break;
                    case 1:
                        SetPauseRecs(GAME->mode, kind, 0);
                        GAME->pause |= 0x80;
                        break;
                    case 2:
                        PauseChoice = 0;
                        PauseLastChoice = 0;
                        GAME->pause |= 0x100;
                        break;
                    case 3:
                        GAME->pause |= 0x8000;
                        PauseChoice = 1;
                        break;
                    case 4:
                        GAME->pause |= 4;
                        PauseChoice = 1;
                        break;
                    case 5:
                        GAME->pause |= 0x10000;
                        PauseChoice = 0;
                        break;
                    }
                }
                /* its own exit, so the PlaySfx tail follows the call and
                   cross-jumps into case 8's */
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                ret = 0;
                break;
            }
            ret = 0;
            break;
        case 8:
            if (PauseChooseUpDown(PausePad, max - 1 + (pause_can_save() ? 1 : 0)) != 0) {
                s32 choice = PauseChoice;

                if (pause_can_save()) {
                    if (choice == 2) {
                        pause_go_save();
                        choice = -1;
                    } else if (choice > 2) {
                        choice--;
                    }
                }
                switch (choice) {
                case 0:
                    PausePad = 0;
                    PauseChoice = 0;
                    GAME->pause &= 0x04000010;
                    sound_resume();
                    break;
                case 1:
                    PauseLastChoice = 0;
                    PauseChoice = 0;
                    GAME->pause |= 0x100;
                    break;
                case 2:
                    PauseChoice = 1;
                    GAME->pause |= 4;
                    break;
                case 3:
                    PauseChoice = 0;
                    GAME->pause |= 0x10000;
                    break;
                }
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            ret = 0;
            break;
        case 0x8000:
            if (PauseChooseYesNo(PausePad) != 0) {
                switch (PauseChoice) {
                case 0:
                    hi = 0;
                    if (GAME->levelFlags & 0x10) {
                        for (i = 0; i < 4; i++) {
                            if (hi < GAME->adventure.roundPoints[i]) {
                                hi = GAME->adventure.roundPoints[i];
                            }
                        }
                        if (hi >= 2) {
                            g = &GameInfo[GAME->mode];  /* the mode's bytes of the per-mode tables (+0xAB, +0xE4) */
                            if (g[0xE4] == 0) {
                                g[0xAB]++;
                                g = &GameInfo[GAME->mode];
                                if (g[0xAB] >= 4) {
                                    g[0xAB] = 3;
                                }
                            }
                        }
                    }
                    GAME->pause |= 0x10;
                    GAME->exitFlags |= 0x8002;
                    break;
                case 1:
                    PauseChoice = 0;
                    GAME->pause &= ~0x8000;
                    break;
                }
            }
            if (ActivePad()->pressed & 0x1000) {
                PauseChoice = 0;
                GAME->pause &= ~0x8000;
            }
            ret = 0;
            break;
        case 4:
        case 0xC:
            if (PauseChooseYesNo(PausePad) != 0) {
                switch (PauseChoice) {
                case 0:
                    ExitToTitles();
                    break;
                case 1:
                    PauseChoice = 0;
                    GAME->pause &= ~4;
                    break;
                }
            }
            if (ActivePad()->pressed & 0x1000) {
                PauseChoice = 0;
                GAME->pause &= ~4;
            }
            ret = 0;
            break;
        case 0x40:
        case 0x240:
            if (GAME->mode != 0x25) {
                p = ActivePad();
                /* no ret = 0 exit right after a branch: jump2 would give
                   case 0's 0x7000 exit to it (MATCHING.md, Cross-jumping) */
                if ((p->held & 9) != 9) {
                    if (p->pressed & 0x4000) {
                        r = PauseRecNext();
                        r ^= 1;
                        if (r) {
                            PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                            KillEngineSlot(0);
                            fl = GAME->pause;
                            if (fl & 0x200) {
                                ret = 1;
                                GAME->pause = fl & ~0x40;
                                PauseChoice = 0;
                            } else {
                                /* the mask set first and the cleared bit in its own
                                   statement, so the two ands stay apart */
                                s32 keep = 0x04000010;
                                s32 t = fl & ~0x40;

                                GAME->pause = t & keep;
                                PausePad = 0;
                                PauseChoice = 0;
                                sound_resume();
                                ret = 1;
                                PauseChoice = 0;
                            }
                            break;
                        }
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    if (ActivePad()->pressed & 0x1000) {
                        PauseRecPrev();
                    }
                    ret = 0;
                } else {
                    ret = 0;
                }
            }
            break;
        case 0x80:
            p = ActivePad();
            if ((p->held & 9) != 9) {
                if (p->pressed & 0x4000) {
                    r = PauseRecNext();
                    r ^= 1;
                    if (r) {
                        PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                        KillEngineSlot(0);
                        PauseTextHold = 1;
                        PauseChoice = 0;
                        GAME->pause &= ~0x80;
                    } else {
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                if (ActivePad()->pressed & 0x1000) {
                    PauseRecPrev();
                }
                ret = 0;
            } else {
                ret = 0;
            }
            break;
        case 0x02000000:
            if (joypad_idle_count <= 0 && !settings()->testing) {
                GAME->demoTimer = 0;
            }
            ret = 1;
            DrawPauseMenu();
            break;
        case 0x100:
        case 0x108:
            s = PauseLastChoice;
            switch (s) {
            case 1:
                {
                    switch (PauseChoice) {
                    case 0:
                        on = GameOptions.stereo != 0;
                        PauseToggles[0] = on;
                        if (ActivePad()->pressed & 0xA0) {
                            PauseToggles[0] = on ^ 1;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                            if (PauseToggles[0] != 0) {
                                GameOptions.stereo = s;
                                sound_set_stereo();
                            } else {
                                GameOptions.stereo = 0;
                                sound_set_mono();
                            }
                        }
                        break;
                    case 1:
                        v = GameOptions.music;
                        if (v > 0 && (ActivePad()->pressed & 0x80)) {
                            v -= 5;
                        }
                        if (v < 100 && (ActivePad()->pressed & 0x20)) {
                            v += 5;
                        }
                        GameOptions.music = v;
                        break;
                    case 2:
                        v = GameOptions.sfx;
                        if (v > 0 && (ActivePad()->pressed & 0x80)) {
                            v -= 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (v < 100 && (ActivePad()->pressed & 0x20)) {
                            v += 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        GameOptions.sfx = v;
                        break;
                    }
                }
                if (PauseChooseUpDown(PausePad, 3) != 0 || (ActivePad()->pressed & 0x1000)) {
                    PauseChoice = 0;
                    PauseLastChoice = 0;
                    music_pause();
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 2:
                for (i = 0; i < GAME->players; i++) {
                    if ((HPADS[i].pressed & 0x8000) && (((PadState *)Pads)[i].flags & 8)) {
                        GameOptions.vibrate[i] = GameOptions.vibrate[i] == 0;
                        pad_rumble_small(i, 10);
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                if (PauseChooseUpDown(PausePad, 0) != 0 || (ActivePad()->pressed & 0x1000)) {
                    PauseChoice = 1;
                    PauseLastChoice = 0;
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 3:
                if (ActivePad()->pressed & 0xA0) {
                    video_option_change(PauseChoice, (ActivePad()->pressed & 0x20) ? 1 : -1);
                    PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (PauseChooseUpDown(PausePad, video_option_count() - 1) != 0 || (ActivePad()->pressed & 0x1000)) {
                    PauseChoice = 2;
                    PauseLastChoice = 0;
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            default:
                if (PauseChooseUpDown(PausePad, 3) != 0) {
                    if (PauseChoice == 3) {
                        PauseLastChoice = 0;
                        GAME->pause &= ~0x100;
                    } else {
                        PauseLastChoice = PauseChoice + 1;
                    }
                    if (PauseChoice == 0) {
                        continue_music();
                    }
                    PauseChoice = 0;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (ActivePad()->pressed & 0x1000) {
                    PauseChoice = 0;
                    GAME->pause &= ~0x100;
                }
                break;
            }
            ret = 0;
            break;
        case 0x10000:
        case 0x10008:
            CheatMenuUpdate();
            ret = 0;
            break;
        case 0x200:
            if (ProcessPauseUD(0, 3) == 0) {
                ret = 1;
                DrawPauseMenu();
                break;
            }
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            switch (PauseChoice) {
            case 1:
                SetPauseRecs(GAME->mode, kind, 0);
                ret = 1;
                SetPauseY(PauseMenuY);
                GAME->pause |= 0x40;
                break;
            case 2:
                PausePad = 0;
                PauseChoice = 0;
                GAME->pause = (GAME->pause & ~0x200) | 0x10;
                GAME->exitFlags |= 0x8002;
                GAME->pause &= 0x04000010;
                sound_resume();
                break;
            case 3:
                GAME->pause &= ~0x200;
                ExitToTitles();
                PausePad = 0;
                PauseChoice = 0;
                GAME->pause &= 0x04000010;
                sound_resume();
                break;
            default:
                PausePad = 0;
                PauseChoice = 0;
                GAME->pause &= 0x04000010;
                sound_resume();
                ret = 1;
                CupTimer = 0;
                break;
            }
            break;
        case 0x20:
        case 0x400:
            if (HPADS[0].pressed & 0x10) {
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                PauseChoice--;
            }
            if (HPADS[0].pressed & 0x40) {
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                PauseChoice++;
            }
            if (PauseChoice < 0) {
                PauseChoice = 0;
            }
            if (PauseChoice >= 2) {
                PauseChoice = 1;
            }
            if (HPADS[0].pressed & 0x4008) {
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                ret = 1;
                CupTimer = 0;
                PausePad = 0;
                GAME->pause &= ~0x21;
            } else {
                ret = 1;
                DrawPauseMenu();
            }
            break;
        default:
            ret = 0;
            PausePad = 0;
            PauseChoice = 0;
            GAME->pause |= 1;
            break;
        }
        /* in both branches: when neither runs, the hook doesn't either */
        if (GameExitFlags & 0x1000) {
            UpdateWinCounter(ret);
        }
    } else if (!(GAME->pause & 0x04000000) || (GAME->exitFlags & 0x1000)) {
        n = 1;
        PausePad = 0;
        PauseFlashItem = -1;
        if (ModeTypes[GAME->mode * 3] != 0x102) {
            n = GAME->players;
        }
        for (i = 0; i < n; i++) {
            if ((HPADS[i].held & 9) != 9 && (HPADS[i].pressed & 8)) {
                if (!(Heroes[i].hero_status & 0xF800) || ModeTypes[GAME->mode * 3] == 0x102 || i == 0) {
                    PausePad = i;
                    PauseFlashItem = i;
                    PauseChoice = 0;
                    GAME->pause |= 1;
                    i = 99;
                    if (ModeTypes[GAME->mode * 3] == 0x102) {
                        GAME->pause |= 9;
                    }
                    StatsPageWanted = 1;
                    DrawAdventureStats(1);
                    ret = 0;
                }
            }
        }
        if (GameExitFlags & 0x1000) {
            UpdateWinCounter(ret);
        }
    }
    if (ret == 0) {
        DrawPauseMenu();
    }
    return ret;
}

void DrawHudEntries(HudEntry *e, s32 arg1) {
    s32 n;
    GameState *g;
    EXTengine *eng;
    s16 *row;
    s16 *row2;
    s32 i;
    s32 k;
    s32 sfx;

    for (n = 0; e->type != -1; e++) {
        g = (GameState *)GameInfo;
        switch (e->type) {
        case 1:
            if ((g->cheats & 0x80010000) != 0x80010000) {
                break;
            }
        case 0:
            if (PauseTextHold == 0) {
                DrawSysText(e->x, (s16)(e->y + arg1),
                              ((u8 **)LangMessages[g->language])[e->idx], e->a, e->b,
                              PauseChoice == n);
            }
            n++;
            break;
        case 8:
            if (PauseTextHold == 0) {
                DrawSysText(e->x, (s16)(e->y + arg1), (u8 *)PauseOwnTexts[e->idx], e->a, e->b, PauseChoice == n);
            }
            n++;
            break;
        case 2:
            HUDBOX->colour = e->a;
            HUDBOX->unk14 = e->c;
            HUDBOX->top = e->x + arg1;
            HUDBOX->x = 0x7C;
            HUDBOX->width = 0x204;
            if ((g->cheats & 0x80010000) != 0x80010000) {
                HUDBOX->bottom = arg1 + (e->x + e->y);
            } else {
                HUDBOX->bottom = arg1 + (e->x + e->idx);
            }
            break;
        case 3:
            if (PauseTextHold == 0) {
                DrawSysText(e->x, (s16)(e->y + arg1), (u8 *)(iptr)e->idx, e->a, e->b, 0);
            }
            break;
        case 4:
            if (ModeTypes[g->mode * 3] == 0x102 && (g->pause & 0x40)) {
                goto clear;
            }
            DrawSpriteTriple(e->x, (s16)(e->y + arg1), e->a, e->b, e->idx);
            if (!(g->pause & 0xC0)) {
                goto clear;
            }
            if (g->pause & 0x40) {
                row = LangHudRows[g->language];
                i = arg1 + e->y;
                PanelTab[5].XYZ.x = row[0] + 0x140;
                PanelTab[5].XYZ.y = i + row[1];
                PanelTab[6].XYZ.x = row[2] + 0x140;
                PanelTab[6].XYZ.y = i + row[3];
            } else {
                row2 = LangHudRows[g->language];
                i = arg1 + e->y;
                PanelTab[5].XYZ.x = row2[4] + 0x140;
                PanelTab[5].XYZ.y = i + row2[5];
                PanelTab[6].XYZ.x = row2[6] + 0x140;
                PanelTab[6].XYZ.y = i + row2[7];
            }
            sfx = 0;
            if (g->levelFlags & 0x10) {
                sfx = 0x2022;
            }
            if (g->levelFlags & 0x80) {
                sfx = 0x201E;
            }
            if (g->levelFlags & 0x40) {
                sfx = 0x201F;
            }
            if (g->levelFlags & 0x20) {
                sfx = 0x2021;
                if (g->levelFlags & 8) {
                    sfx = 0x2020;
                }
            }
            if (g->mode == 0x20) {
                sfx = 0;
            }
            if (g->levelFlags & 0x100) {
                sfx = 0;
            }
            if (ModeTypes[g->mode * 3] == 0x102) {
                sfx = 0;
            }
            PauseVolSubA.entity = sfx;
            PauseVolSubB.entity = sfx;
            break;
        clear:
            PauseVolSubA.entity = 0;
            PauseVolSubB.entity = 0;
            break;
        case 5:
            if (PauseTextHold != 0) {
                break;
            }
            DrawSysText(e->x, (s16)(e->y + arg1),
                          ((u8 **)LangMessages[g->language])[e->idx], e->a, e->b, 0);
            break;
        case 6:
            FontPage = e->x;
            break;
        case 7:
            HUDBOX->colour = e->a;
            HUDBOX->unk14 = e->c;
            HUDBOX->top = e->x + arg1;
            HUDBOX->x = 0x40;
            HUDBOX->width = 0x240;
            if ((g->cheats & 0x80010000) != 0x80010000) {
                HUDBOX->bottom = arg1 + (e->x + e->y);
            } else {
                HUDBOX->bottom = arg1 + (e->x + e->idx);
            }
            break;
        case 10: case 11: case 12: case 13: case 14:
        case 15: case 16: case 17: case 18: case 19:
            k = e->type - 10;
            if (PauseToggles[k] != 1) {
                break;
            }
            if (PauseChoice != k) {
                goto draw;
            }
            DrawSysText(e->x, (s16)(e->y + arg1),
                          ((u8 **)LangMessages[g->language])[e->idx], e->c, e->d, 0);
            break;
        case 20: case 21: case 22: case 23: case 24:
        case 25: case 26: case 27: case 28: case 29:
            k = e->type - 20;
            if (PauseToggles[k] != 0) {
                break;
            }
            if (PauseChoice != k) {
                goto draw;
            }
            DrawSysText(e->x, (s16)(e->y + arg1),
                          ((u8 **)LangMessages[g->language])[e->idx], e->c, e->d, 0);
            break;
        case 30: case 31: case 32: case 33: case 34:
        case 35: case 36: case 37: case 38: case 39:
            k = e->type - 30;
            if (PauseToggles[k] != 9) {
                break;
            }
        draw:
            DrawSysText(e->x, (s16)(e->y + arg1),
                          ((u8 **)LangMessages[g->language])[e->idx], e->a, e->b, 0);
            break;
        }
    }
    if (PauseTextHold != 0) {
        PauseTextHold--;
    }
    eng = PanelTab;
    eng[5].xform.dirty = 1;
    eng[6].xform.dirty = 1;
    eng[5].xform.rot.vy += 0x40;
    eng[6].xform.rot.vy += 0x40;
}

#define VIDEO_ROW_Y 0x2C
#define VIDEO_ROW_STEP 0x14
#define VIDEO_LABEL_X 0x58
#define VIDEO_VALUE_X 0x13C

/* A page like the sound one, with its rows from the shared options, in the small font to fit their values. */
static void DrawPauseVideo(void) {
    char value[24];
    s32 i;

    DrawHudEntries(PauseVideoMenu, PauseMenuY);
    if (PauseTextHold != 0) {
        return;
    }
    DrawSysText(-0x8000, (s16)(0xA + PauseMenuY), (u8 *)"VIDEO", 0xD0FF, 0xFF, 0);
    FontPage = 1;
    for (i = 0; i < video_option_count(); i++) {
        s32 y = VIDEO_ROW_Y + i * VIDEO_ROW_STEP + PauseMenuY;
        s32 lit = PauseChoice == i;

        video_option_value(i, value, sizeof(value));
        DrawSysText(VIDEO_LABEL_X, (s16)y, (u8 *)video_option_label(i), 0xD0FF, 0xFF, lit);
        DrawSysText(VIDEO_VALUE_X, (s16)y, (u8 *)value, 0xD0FF, 0xFF, lit);
    }
    DrawSysText(-0x8000, (s16)(0xE8 + PauseMenuY), (u8 *)video_option_help(PauseChoice), 0xD0FF, 0xFF, 0);
}

void DrawPauseMenu(void) {
    HudBox *e;
    s32 i;
    s32 sel;

    if (GAME->pause == 0) {
        return;
    }
    PauseVolSubA.entity = 0;
    PauseVolSubB.entity = 0;
    PanelCountCompletion();
    SetupCompletionDigits();
    PauseMenuY = 0x70;
    e = &PauseMenuPanel;
    e->top = -1;
    e->bottom = -1;
    FontPage = 0;
    if (GAME->pause & 0x08000002) {
        return;
    }
    if (GAME->pause & 0x7000) {
        if (GAME->pause & 0x2000) {
            DrawHudEntries(PauseBlankB, 0x70);
            return;
        }
        if (GAME->pause & 0x1000) {
            DrawHudEntries(PauseBlankA, 0x70);
            return;
        }
        if (GAME->pause & 0x4000) {
            DrawHudEntries(PausePadMessage, 0x70);
            return;
        }
    }
    switch (GAME->pause & 0x023F8FEC) {
    case 0:
        if (GAME->mode == 0x20) {
            DrawHudEntries(PauseMenuQuit, PauseMenuY);
            PauseStatsAllowed = 0;
            return;
        }
        if (GAME->levelFlags & 4) {
            DrawHudEntries(PauseMenuExitArena, PauseMenuY);
        } else {
            DrawHudEntries(PauseMenuChangeArena, PauseMenuY);
        }
        PauseStatsAllowed = 1;
        break;
    case 8:
        DrawHudEntries(pause_can_save() ? PauseMenuSave : PauseMenuShort, PauseMenuY);
        PauseStatsAllowed = 1;
        break;
    case 0x8000:
        DrawHudEntries(PauseConfirmExit, PauseMenuY);
        break;
    case 0x10000:
    case 0x10008:
        CheatMenuDraw(PauseMenuY);
        break;
    case 4:
    case 0xC:
        DrawHudEntries(PauseConfirmMenus, PauseMenuY);
        break;
    case 0x40:
    case 0x240:
        DrawHudEntries(PauseRules1, PauseMenuY);
        SetPauseY(PauseMenuY);
        break;
    case 0x80:
        DrawHudEntries(PauseRules0, PauseMenuY);
        SetPauseY(PauseMenuY);
        break;
    case 0x100:
    case 0x108:
        sel = PauseLastChoice;
        switch (sel) {
        case 1:
            PauseToggles[1] = sel;
            PauseToggles[2] = sel;
            PauseToggles[0] = GameOptions.stereo != 0;
            FormatPercent(LangMessages[GAME->language]->musicText, GameOptions.music);
            FormatPercent(LangMessages[GAME->language]->sfxText, GameOptions.sfx);
            DrawHudEntries(PauseSoundMenu, PauseMenuY);
            break;
        case 2:
            for (i = 0; i < GAME->players; i++) {
                if (Pads[i].flags & 8) {
                    PauseToggles[i] = GameOptions.vibrate[i] != 0;
                } else {
                    PauseToggles[i] = 9;
                }
            }
            for (; i < 4; i++) {
                PauseToggles[i] = 9;
            }
            DrawHudEntries(PauseControllerMenu, PauseMenuY);
            break;
        case 3:
            DrawPauseVideo();
            break;
        default:
            DrawHudEntries(PauseOptionsMenu, PauseMenuY);
            break;
        }
        break;
    case 0x200:
        DrawHudEntries(PauseEndMenu, PauseMenuY);
        break;
    case 0x20:
        DrawHudEntries(PauseEndWarpMenu, PauseMenuY);
        break;
    case 0x400:
        DrawHudEntries(PauseEndAdventureMenu, PauseMenuY);
        break;
    case 0x2000000:
        if (GAME->frames & 0x10) {
            DrawHudEntries(PauseDemoLabel, PauseMenuY);
        }
        break;
    }
}

void DrawAdventureStats(s32 arg0) {
    u16 mode;
    s32 n[4];
    s32 c0, c1, c2, c3, c4;
    s32 i;
    EXTengine *e;
    subengine *sub;

    mode = arg0;
    c3 = 0;
    c4 = 0;
    c2 = 0;
    c1 = 0;
    c0 = 0;
    for (i = 0; i < 0x21; i++) {
        if (GAME->adventure.won[0][i] != 0) {
            c0++;
        }
        if (GAME->adventure.won[2][i] != 0) {
            c1++;
        }
        if (GAME->adventure.won[1][i] != 0) {
            c2++;
        }
        if (GAME->adventure.won[1][i] == 2) {
            c4++;
        }
        if (GAME->adventure.won[3][i] != 0) {
            c3++;
        }
    }
    for (i = 0; i < 4; i++) {
        n[i] = GAME->adventure.roundPoints[i];
        if (mode == 5 && (GAME->levelFlags & 2)) {
            n[i] += GAME->adventure.points[i];
        }
    }
    if (GameLevelFlags & 0xC0) {
        StatsHasPage3 = 0;
    } else {
        StatsHasPage3 = 1;
    }
    if (GAME->pause & 8) {
        StatsHasPage3 = 0;
    }
    if (GAME->levelFlags & 0x100) {
        StatsHasPage3 = 0;
    }
    if (GAME->levelFlags & 3) {
        StatsHasPage1 = 0;
    } else {
        StatsHasPage1 = 1;
    }
    if (GameLevelFlags & 2) {
        StatsHasBonus = 1;
    } else {
        StatsHasBonus = 0;
    }
    if (mode == 1 && StatsHasPage1 == 0) {
        StatsPageWanted = 3;
        mode = 3;
    }
    if (mode == 3 && StatsHasPage3 == 0) {
        StatsPageWanted = 1;
        mode = 1;
    }
    if (StatsHasPage3 == 0 && StatsHasPage1 == 0) {
        mode = 0;
    }
    StatsPageShown = mode;
    e = GetEngineSlot(4);
    switch (mode) {
    case 1:
        i = c0;
        PauseDigits[1] = i % 10 + '0';
        i /= 10;
        PauseDigits[0] = i % 10 + '0';
        i = c1;
        PauseDigits2[1] = i % 10 + '0';
        i /= 10;
        PauseDigits2[0] = i % 10 + '0';
        i = c3;
        PauseDigits3[1] = i % 10 + '0';
        i /= 10;
        PauseDigits3[0] = i % 10 + '0';
        i = c2;
        PauseDigits4[1] = i % 10 + '0';
        i /= 10;
        PauseDigits4[0] = i % 10 + '0';
        i = c4;
        PauseDigits5[1] = i % 10 + '0';
        i /= 10;
        PauseDigits5[0] = i % 10 + '0';
        if (GAME->cheats & 0x2000) {
            if (GAME->pause & 8) {
                PlaceEngineSlot(4, AdvStatsPanel5);
            } else {
                PlaceEngineSlot(4, AdvStatsPanel4);
            }
        } else {
            switch (GAME->adventure.stage) {
            case 1:
                if (GAME->pause & 8) {
                    PlaceEngineSlot(4, AdvStatsPanel3);
                } else {
                    PlaceEngineSlot(4, AdvStatsPanel2);
                }
                break;
            case 2:
                if (GAME->pause & 8) {
                    PlaceEngineSlot(4, AdvStatsPanel5);
                } else {
                    PlaceEngineSlot(4, AdvStatsPanel4);
                }
                break;
            default:
                if (GamePause & 8) {
                    PlaceEngineSlot(4, AdvStatsPanel1);
                } else {
                    PlaceEngineSlot(4, AdvStatsPanel0);
                }
                break;
            }
        }
        e->XYZ.x = 0x60;
        e->XYZ.y = 0x1F4;
        sub = e->sub;
        if (GAME->levelFlags & 0x10) {
            sub[1].XYZ.x = sub[3].XYZ.x;
        }
        if (GAME->levelFlags & 0x40) {
            sub[1].XYZ.x = sub[5].XYZ.x;
        }
        if (GAME->levelFlags & 0x80) {
            sub[1].XYZ.x = sub[7].XYZ.x;
        }
        if (GAME->levelFlags & 0x20) {
            sub[1].XYZ.x = sub[9].XYZ.x;
        }
        if (GAME->levelFlags & 8) {
            sub[1].XYZ.x = sub[11].XYZ.x;
        }
        if (StatsHasPage3 != 0) {
            sub[2].flags |= 0x8000;
        } else {
            sub[2].flags &= ~0x8000;
        }
        e->flags |= 0x10008000;
        break;
    case 3:
        i = n[0];
        PauseDigits[1] = i % 10 + '0';
        i /= 10;
        PauseDigits[0] = i % 10 + '0';
        i = n[1];
        PauseDigits2[1] = i % 10 + '0';
        i /= 10;
        PauseDigits2[0] = i % 10 + '0';
        i = n[2];
        PauseDigits3[1] = i % 10 + '0';
        i /= 10;
        PauseDigits3[0] = i % 10 + '0';
        i = n[3];
        PauseDigits4[1] = i % 10 + '0';
        i /= 10;
        PauseDigits4[0] = i % 10 + '0';
        PlaceEngineSlot(4, AdvStatsPanel6);
        e->XYZ.x = 0x60;
        e->XYZ.y = 0x1E0;
        sub = e->sub;
        e->flags |= 0x10008000;
        if (StatsHasPage1 != 0) {
            sub[2].flags |= 0x8000;
        } else {
            sub[2].flags &= ~0x8000;
        }
        if (StatsHasBonus != 0) {
            sub[2].flags |= 0x8000;
        }
        break;
    case 5:
        i = n[0];
        PauseDigits[1] = i % 10 + '0';
        i /= 10;
        PauseDigits[0] = i % 10 + '0';
        i = n[1];
        PauseDigits2[1] = i % 10 + '0';
        i /= 10;
        PauseDigits2[0] = i % 10 + '0';
        i = n[2];
        PauseDigits3[1] = i % 10 + '0';
        i /= 10;
        PauseDigits3[0] = i % 10 + '0';
        i = n[3];
        PauseDigits4[1] = i % 10 + '0';
        i /= 10;
        PauseDigits4[0] = i % 10 + '0';
        PlaceEngineSlot(4, AdvStatsPanel7);
        e->XYZ.x = 0x60;
        e->XYZ.y = 0x1E0;
        sub = e->sub;
        e->flags |= 0x10008000;
        if (StatsHasPage1 != 0) {
            sub[2].flags |= 0x8000;
        } else {
            sub[2].flags &= ~0x8000;
        }
        if (StatsHasBonus != 0) {
            sub[2].flags |= 0x8000;
        }
        break;
    }
}

void UpdatePauseSlide(s32 arg0) {
    EXTengine *e;
    s32 lim;

    lim = 0x15E;
    if (ModeTypes[GameMode * 3] == 0x102) {
        lim = 0x140;
    }
    e = GetEngineSlot(4);
    if (PauseStatsAllowed == 0) {
        if (StatsPageShown != 0) {
            StatsPageShown = 9;
        }
    } else {
        switch (StatsPageShown) {
        case 3:
        case 4:
            if (StatsPageWanted == 1) {
                StatsPageShown = 2;
            }
            if (StatsPageWanted == 5) {
                StatsPageShown = 6;
            }
            break;
        case 1:
        case 2:
        case 5:
        case 6:
            if (StatsPageWanted == 3) {
                StatsPageShown = 4;
            }
            break;
        default:
            if (StatsPageWanted == 1) {
                StatsPageShown = 2;
            }
            if (StatsPageWanted == 3) {
                StatsPageShown = 4;
            }
            if (StatsPageWanted == 5) {
                StatsPageShown = 6;
            }
            break;
        }
    }
    PauseStatsAllowed = 0;
    switch (StatsPageShown) {
    case 1:
        if (e->XYZ.y > lim) {
            e->XYZ.y -= 0x20;
        }
        if (e->XYZ.y < lim) {
            e->XYZ.y = lim;
        }
        if (StatsHasPage3 != 0 && (((PadState *)Pads)[PausePad].pressed & 0x1000)) {
            StatsPageWanted = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 3:
        if (e->XYZ.y > lim) {
            e->XYZ.y -= 0x20;
        }
        if (e->XYZ.y < lim) {
            e->XYZ.y = lim;
        }
        if (StatsHasPage1 == 0 && StatsHasBonus == 0) {
            break;
        }
        if (HPADS[PausePad].pressed & 0x1000) {
            if (GameLevelFlags & 2) {
                StatsPageWanted = 5;
            } else {
                StatsPageWanted = 1;
            }
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 5:
        if (e->XYZ.y > lim) {
            e->XYZ.y -= 0x20;
        }
        if (e->XYZ.y < lim) {
            e->XYZ.y = lim;
        }
        if (StatsHasPage3 != 0 && (((PadState *)Pads)[PausePad].pressed & 0x1000)) {
            StatsPageWanted = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 2:
        if (e->XYZ.y <= 0x1E0) {
            e->XYZ.y += 0x20;
        } else {
            KillEngineSlot(4);
            DrawAdventureStats(1);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (StatsHasPage3 != 0 && (((PadState *)Pads)[PausePad].pressed & 0x1000)) {
            StatsPageWanted = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 4:
        if (e->XYZ.y <= 0x1E0) {
            e->XYZ.y += 0x20;
        } else {
            KillEngineSlot(4);
            DrawAdventureStats(3);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (StatsHasPage1 == 0 && StatsHasBonus == 0) {
            break;
        }
        if (HPADS[PausePad].pressed & 0x1000) {
            if (GameLevelFlags & 2) {
                StatsPageWanted = 5;
            } else {
                StatsPageWanted = 1;
            }
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 6:
        if (e->XYZ.y <= 0x1E0) {
            e->XYZ.y += 0x20;
        } else {
            KillEngineSlot(4);
            DrawAdventureStats(5);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (StatsHasPage3 != 0 && (((PadState *)Pads)[PausePad].pressed & 0x1000)) {
            StatsPageWanted = 5;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 9:
        if (arg0 != 0) {
            e->XYZ.y = 0x1E0;
        }
        if (e->XYZ.y < 0x1E0) {
            e->XYZ.y += 0x20;
            if (e->XYZ.y < 0x1E0) {
                break;
            }
        }
        KillEngineSlot(4);
        StatsPageShown = 0;
        break;
    }
}

void RenderPause(void) {
    GfxState *gs;
    HudBox *e;

    if (GamePause & 0x7000) {
        gs = &RenderState;
        *(s32 *)&gs->screen[0] = 0;
        *(s32 *)&gs->screen[4] = 0;
        if (GamePause & 0x4000) {
            e = &PauseMenuPanel;
            if (e->bottom != -1) {
                gfx_set_viewport(&MainViewport, &MainGfxCamera);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)&ScreenFade;   /* a scalar read: scheduled above the otzAdd store */
                draw_panel_rect(e, 0x10000000);
            }
        }
    } else if (GamePause & 1) {
        gs = &RenderState;
        *(s32 *)&gs->screen[0] = 0;
        *(s32 *)&gs->screen[4] = 0;
        if (!(GamePause & 0x40)) {
            e = &PauseMenuPanel;
            if (e->bottom != -1) {
                gfx_set_viewport(&MainViewport, &MainGfxCamera);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)&ScreenFade;
                draw_panel_rect(e, 0x10000000);
            }
        }
        gfx_set_viewport(&HudViewport, &HudCamera);
        eng_render_subs(GetEngineSlot(0));
        gfx_set_viewport(&MainViewport, &MainGfxCamera);
        eng_render_subs(GetEngineSlot(4));
        eng_render(&PanelTab[5]);
        eng_render(&PanelTab[6]);
    }
}

void StartWinSequence(s32 arg0) {
    GameState *g;
    GameState *g2;
    PlaceRec **set;
    PlaceRec *rec;

    WinSeqTimer = 0x4B;
    WinSeqStep = 4;
    WinSeqHero = 0x63;
    WinCounterSize = 0x19;
    WinCounterState = 0;
    WinCounterAnim = 1;
    g = (GameState *)GameInfo;
    g->pause &= 0xFDC07013;
    PauseDelay = 3;
    g->pause |= 0x04000000;
    if (g->demo != 0) {
        g->pause |= 0x02000000;
        g->demoTimer = settings()->testing ? 0x7FFFFFFF : 0x384;
        g->heroCpu[0] = 1;
        g->heroCpu[1] = 1;
        g->heroCpu[2] = 1;
        g->heroCpu[3] = 1;
        return;
    }
    switch (g->levelFlags & 0xE0) {
    case 0x80:
        if (g->adventure.modeFlags[g->mode] & 0x80) {
            return;
        }
        set = PauseMenuSets[g->mode][2];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            PlaceEngineSlot(0, rec);
        }
        g->adventure.modeFlags[g->mode] |= 0x80;
        PausePad = 0;
        g->pause |= 0x41;
        WinCounterSize >>= 1;
        return;
    case 0x20:
        if (g->levelFlags & 8) {
            if (g->adventure.modeFlags[g->mode] & 8) {
                return;
            }
            set = PauseMenuSets[g->mode][8];
            if (set != NULL) {
                PauseRecSet = set;
                rec = *set;
                PauseRecIndex = 0;
                PauseRec = rec;
                PlaceEngineSlot(0, rec);
            }
            g->adventure.modeFlags[g->mode] |= 8;
            PausePad = 0;
            g->pause |= 0x41;
            WinCounterSize >>= 1;
        } else {
            if (g->adventure.modeFlags[g->mode] & 0x20) {
                return;
            }
            set = PauseMenuSets[g->mode][6];
            if (set != NULL) {
                PauseRecSet = set;
                rec = *set;
                PauseRecIndex = 0;
                PauseRec = rec;
                PlaceEngineSlot(0, rec);
            }
            g->adventure.modeFlags[g->mode] |= 0x20;
            PausePad = 0;
            g->pause |= 0x41;
            WinCounterSize >>= 1;
        }
        return;
    case 0x40:
        if (g->adventure.modeFlags[g->mode] & 0x40) {
            return;
        }
        set = PauseMenuSets[g->mode][4];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            PlaceEngineSlot(0, rec);
        }
        g->adventure.modeFlags[g->mode] |= 0x40;
        PausePad = 0;
        g->pause |= 0x41;
        WinCounterSize >>= 1;
        return;
    default:
        g2 = (GameState *)GameInfo;
        if (g2->adventure.modeFlags[g2->mode] & 0x10) {
            return;
        }
        set = PauseMenuSets[g2->mode][0];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            PlaceEngineSlot(0, rec);
        }
        g2->adventure.modeFlags[g2->mode] |= 0x10;
        PausePad = 0;
        g2->pause |= 0x41;
        WinCounterSize >>= 1;
        return;
    }
}

void RecordWin(void) {
    s32 i;
    u8 *p;
    u8 *g;
    u8 *q;
    u8 *r;

    WinCounterSize = 0x1E;
    g = GameInfo;
    if (((GameState *)g)->levelFlags & 0x100) {
        (g + ((GameState *)g)->mode)[0xAB] += 2;  /* matching: the mode's modeSkill byte, through g + mode (as UpdatePause) */
        p = g + ((GameState *)g)->mode;  /* matching: as above */
        if (p[0xAB] >= 0x20) {
            p[0xAB] = 0x1F;
        }
    }
    q = GameInfo;
    if (((GameState *)q)->levelFlags & 0x1E0) {
        i = 3;
        r = q + i;
        for (; i >= 0; i--) {
            r[0xCC] = 0;  /* matching: roundPoints[i] through a stepped pointer */
            r--;
        }
        if ((GameLevelFlags & 0x40) && (PanelCtrl & 1) && Clock_Time == 0) {
            WinCounterState = 6;
            WinCounterAnim = 3;
            WinSeqStep = 0x28;
            WinSeqHero = -1;
            WinSeqTimer = 0xF;
            WinSeqDelay = 0x1E;
            return;
        }
        WinCounterState = 6;
        WinCounterAnim = 3;
        WinSeqStep = 0x1E;
        WinSeqHero = -1;
        WinSeqTimer = 0xF;
        WinSeqDelay = 0x1E;
        return;
    }
    WinCounterState = 5;
}

void AnimateWinCounter(void) {
    s32 t;

    switch (WinCounterAnim) {
    case 1:
        t = counter_sub.xform.scale.vx = (WinCounterSize << 12) / 25;
        counter_sub.xform.scale.vy = t;
        counter_sub.xform.scale.vz = t;
        counter_sub.xform.rot.vx = (WinCounterSize << 10) / 25 - 0x400;
        if (WinCounterState & 1) {
            counter_sub.xform.rot.vy = counter_sub.xform.rot.vy + 0x80;
        } else {
            counter_sub.xform.rot.vy = counter_sub.xform.rot.vy - 0x80;
        }
        if (WinCounterState == 4) {
            counter_sub.XYZ.x = 0x140;
        }
        if (counter_sub.xform.scale.vx < 0x101) {
            counter_sub.xform.scale.vx = 0x1000;
            counter_sub.xform.scale.vy = 0x1000;
            counter_sub.xform.scale.vz = 0x1000;
            counter_sub.xform.rot.vy = 0;
            counter_sub.xform.rot.vx = 0;
            counter_sub.XYZ.x = 0x140;
            counter_sub.flags &= 0xFFFF7FFF;
            if (WinCounterState == 4) {
                GameState *g = GAME;
                WinCounterAnim = 0;
                g->exitFlags &= 0xEFFF;
            }
        }
        counter_sub.xform.dirty = 1;
        return;
    case 3:
        WinCounterState = 6;
        WinCounterAnim = 0;
        return;
    }
}

void UpdateWinCounter(s32 arg0) {
    SLONG_XYZ pos;
    s32 d;
    s32 t;

    if (!(counter_sub.flags & 0x8000)) {
        switch (WinCounterState) {
        case 1:
            counter_sub.entity = 0x2004;
            PauseDelay = 3;
            WinCounterAnim = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.XYZ.x;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A6, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            counter_sub.entity = 0x2003;
            PauseDelay = 3;
            WinCounterAnim = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.XYZ.x;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A5, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
            counter_sub.entity = 0x2002;
            PauseDelay = 3;
            WinCounterAnim = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.XYZ.x;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A4, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PauseDelay = 3;
            counter_sub.entity = 0x2005;
            counter_sub.flags |= 0x8000;
            PlaySfx(0x1A7, NULL, NULL, 0x1000, 0x1E00, 0);
            ((GameState *)GameInfo)->pause &= 0xFBFFFFFF;
            break;
        case 5:
            counter_sub.entity = 0x2006;
            counter_sub.xform.scale.vx = 0x100;
            counter_sub.xform.scale.vy = 0x100;
            counter_sub.xform.scale.vz = 0x100;
            counter_sub.xform.dirty = 1;
            WinCounterState = 0xB;
            WinCounterAnim = 2;
            counter_sub.flags |= 0x8000;
            break;
        case 6:
            counter_sub.entity = 0x201B;
            counter_sub.xform.scale.vx = 0x100;
            counter_sub.xform.scale.vy = 0x100;
            counter_sub.xform.scale.vz = 0x100;
            counter_sub.xform.rot.vx = 0;
            counter_sub.xform.rot.vy = 0;
            counter_sub.xform.rot.vz = 0;
            counter_sub.xform.dirty = 1;
            WinCounterState = 0xB;
            WinCounterAnim = 2;
            counter_sub.flags |= 0x8000;
            break;
        }
    }
    if (WinCounterAnim == 0) {
        counter_sub.flags &= 0xFFFF7FFF;
    }
    if (arg0 != 0) {
        if (PauseDelay > 0) {
            PauseDelay--;
        } else {
            AnimateWinCounter();
            WinCounterSize--;
        }
    }
    if (WinCounterState == 0xB) {
        d = 0x1000 - counter_sub.xform.scale.vx;
        counter_sub.xform.scale.vx += d >> 3;
        counter_sub.xform.scale.vy = counter_sub.xform.scale.vx;
        counter_sub.xform.scale.vz = counter_sub.xform.scale.vx;
        counter_sub.xform.rot.vy = ((sin_cos_table[(((GameFrameCount << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        counter_sub.xform.dirty = 1;
        if (d < 0x100) {
            PlaySfx(0x1AF, NULL, NULL, 0x1000, 0x1E00, 0);
            WinCounterState = 0xC;
        }
    }
    if (WinCounterState == 0xC) {
        t = counter_sub.xform.scale.vx + ((0x1000 - counter_sub.xform.scale.vx) >> 3);
        counter_sub.xform.scale.vx = t;
        counter_sub.xform.scale.vy = t;
        counter_sub.xform.scale.vz = t;
        counter_sub.xform.rot.vy = ((sin_cos_table[(((GameFrameCount << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        counter_sub.xform.dirty = 1;
    }
    if (WinCounterSize <= 0) {
        WinCounterSize = 0x19;
        if (WinCounterAnim == 1) {
            WinCounterState++;
        }
    }
}

void SetPauseY(s32 arg0) {
    GetEngineSlot(0)->XYZ.y = arg0;
}

s16 PauseRecValue(void) {
    PlaceRec *p = PauseRec;

    if (p != NULL) {
        do {
            if (p->type != 0) {
                if (p->type == 1) {
                    return p->x;
                }
            } else {
                return 0;
            }
            p++;
        } while (p != NULL);
    }
    return 0;
}

void PauseRecPrev(void) {
    PlaceRec *rec;
    s32 n;

    if (PauseRecSet != NULL) {
        if (PauseRecIndex != 0) {
            n = PauseRecIndex - 1;
            rec = PauseRecSet[n];
            PauseRecIndex = n;
            PauseRec = rec;
            PlaceEngineSlot(0, rec);
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

s32 PauseRecNext(void) {
    PlaceRec *rec;

    if (PauseRecSet != NULL) {
        rec = PauseRecSet[PauseRecIndex + 1];
        if (rec != NULL) {
            PauseRecIndex = PauseRecIndex + 1;
            PauseRec = rec;
            PlaceEngineSlot(0, rec);
            return 1;
        }
    }
    PauseRecSet = NULL;
    PauseRec = NULL;
    return 0;
}

void SetPauseRecs(s32 arg0, s32 kind, s32 flag) {
    PlaceRec ***t;
    PlaceRec **set;
    PlaceRec *rec;

    t = PauseMenuSets[GameMode];
    if (flag != 0) {
        switch (kind) {
        case 3:
            set = t[6];
            break;
        case 4:
            set = t[8];
            break;
        case 5:
            set = t[4];
            break;
        case 6:
            set = t[2];
            break;
        default:
            set = t[0];
            break;
        }
    } else {
        switch (kind) {
        case 3:
            set = t[7];
            break;
        case 4:
            set = t[9];
            break;
        case 5:
            set = t[5];
            break;
        case 6:
            set = t[3];
            break;
        default:
            set = t[1];
            break;
        }
    }
    if (set != NULL) {
        PauseRecSet = set;
        rec = *set;
        PauseRecIndex = 0;
        PauseRec = rec;
        PlaceEngineSlot(0, rec);
    }
}

s32 PauseExitPressed(void) {
    PadState *base = (PadState *)Pads;
    PadState *pad = &base[PausePad];
    if ((pad->held & 9) != 9) {
        if (pad->pressed & 0x4008) {
            return 1;
        }
    }
    return 0;
}

void Unpause(void) {
    GameState *p = GAME;
    PausePad = 0;
    PauseChoice = 0;
    p->pause &= 0x04000010;
    sound_resume();
}

void RequestQuit(void) {
    GameState *p = GAME;
    s32 a = p->pause;
    u16 b = p->exitFlags;
    p->pause = a | 0x10;
    p->exitFlags = b | 0x8002;
}

void SetDefaultHeroHooks(void) {
    HeroHooks.InitHero = 0;
    HeroHooks.HeroModels = DefaultHeroModels;
    HeroHooks.RunSpeeds = DefaultRunSpeeds;
    HeroHooks.JumpEnts = DefaultJumpEnts;
    HeroHooks.VictorySeqs = DefaultVictorySeqs;
    HeroHooks.PreMoveHero = DefPreMoveHero;
    HeroHooks.MoveHero = DefMoveHero;
    HeroHooks.HeroFacing = DefHeroFacing;
    HeroHooks.HeroSetIdle = DefHeroSetIdle;
    HeroHooks.HeroTouched = DefHeroTouched;
    HeroHooks.HeroBot = DefHeroBot;
    HeroHooks.TestAttack = HeroNoKick;
    HeroHooks.TestRun = HeroNoKick;
    HeroHooks.Hero2ObjectCollision = DefHero2ObjectCollision;
    HeroHooks.HeroGrabObject = DefHeroGrabObject;
    HeroHooks.HeroKickCollision = DefHeroKickCollision;
    HeroHooks.ChooseWinner = DefChooseWinner;
    HeroHooks.HeroesTouched = DefaultTouched;
    HeroHooks.flag = 0;
    HeroHooks.CreateShadow = 0;
    HeroHooks.UpdateShadow = 0;
    HeroHooks.PostMoveHero = 0;
    HeroHooks.TestJump = 0;
    HeroHooks.TestKick = 0;
    HeroHooks.HeroExtraEffects = 0;
    HeroHooks.HeroCarryCollision = 0;
    HeroHooks.KeepInBounds = DefKeepInBounds;
    AiBotTweakTable = DefaultAiBotTweaks;
    AiHumanTweakTable = DefaultAiHumanTweaks;
    AiSkillTable = DefaultAiSkills;
    AiHumanRoundTweakTable = DefaultAiHumanRoundTweaks;
    AiTeamSkillTable = DefaultAiTeamSkills;
    HeroesAllOut = 0;
    HeroActHit = 0xD;
    HeroActPickup = 0xE;
    HeroActHold = 0xF;
    HeroActCarry = 0x10;
    HeroActThrow = 0x11;
    HeroActDead = 0x13;
    HeroActWinner = 0x14;
    HeroActLoser = 0xC;
}

/* data, in address order (tools/scripts/data2c.py) */
s16 LangHudRows[5][8] = {
    { -0x96, 0x23, 0x96, 0x23, -0xAA, 0x23, 0xAA, 0x23 },
    { -0xAA, 0x23, 0xAA, 0x23, -0xAA, 0x23, 0xAA, 0x23 },
    { -0xAA, 0x23, 0xAA, 0x23, -0xAA, 0x23, 0xAA, 0x23 },
    { -0xAA, 0x23, 0xAA, 0x23, -0xAA, 0x23, 0xAA, 0x23 },
    { -0xAA, 0x23, 0xAA, 0x23, -0xAA, 0x23, 0xAA, 0x23 },
};
s32 PauseLastChoice = 0;
s32 PauseChoice = 0;
s32 PausePad = 0;
s32 PauseFlashItem = -1;
s32 PauseUnused[2] = { 5, 0 };
s32 PauseTextHold = 0;  /* frames the pause texts stay hidden after a choice */
const char * ModeNames[9] = {
    NULL, ModeNameBattleMode, ModeNameTournamentMode, ModeNameAdventureTrophy,
    ModeNameAdventureGoldRelic, ModeNameAdventurePlatRelic,
    ModeNameAdventureGem, ModeNameAdventureChallenge, ModeNameAdventureBoss,
};
u16 LevelKindKeys[7] =  /* GameLevelFlags & 0xFF per level kind (Cups_Update, UpdatePause) */ { 1, 2, 0x14, 0x24, 0x2C, 0x44, 0x84 };
s16 PauseUnused2 = 0x14;
char PauseDigits[4] = { 0x31, 0x30 }; /* the PS1 had the zero after it as padding */
char PauseDigits2[4] = { 0x32, 0x30 }; /* the PS1 had the zero after it as padding */
char PauseDigits3[4] = { 0x33, 0x30 }; /* the PS1 had the zero after it as padding */
char PauseDigits4[4] = { 0x34, 0x30 }; /* the PS1 had the zero after it as padding */
char PauseDigits5[4] = { 0x35, 0x30 }; /* the PS1 had the zero after it as padding */
u8 PauseDigits6[4] = { 0x36, 0x30, 0, 0 };
PlaceRec AdvStatsPanel0[10] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 5, 0x4B, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 5, 2 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xCA, 0x3065, 0 },
    { 5, 0x4B, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 4, 2 },
    { 3, 0x50, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0xE1, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 1, 2 },
    { 3, 0xE6, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0x177, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 2 },
    { 3, 0x17C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel1[10] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 5, 0x4B, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 2 },
    { 3, 0x50, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0xE1, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 5, 2 },
    { 3, 0xE6, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0x177, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 7, 2 },
    { 3, 0x17C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel2[12] = {
    { 4, 0, 0x1C0, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 5, 0x38, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 5, 2 },
    { 0xA, 0x1B0, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xCA, 0x3065, 0 },
    { 5, 0x38, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 4, 2 },
    { 3, 0x3D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0xA8, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 1, 2 },
    { 3, 0xAD, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0x118, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 2 },
    { 3, 0x11D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 5, 0x188, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 3, 2 },
    { 3, 0x18D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel3[12] = {
    { 4, 0, 0x1C0, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 0xA, 0x140, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 0xA, 0x1B0, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 5, 0x38, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 2 },
    { 3, 0x3D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0xA8, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 5, 2 },
    { 3, 0xAD, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0x118, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 7, 2 },
    { 3, 0x11D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 5, 0x188, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 3, 2 },
    { 3, 0x18D, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel4[14] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 5, 0x2D, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 5, 2 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xCA, 0x3065, 0 },
    { 5, 0x2D, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 4, 2 },
    { 3, 0x32, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0x87, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 1, 2 },
    { 3, 0x8C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0xE1, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 2 },
    { 3, 0xE6, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 5, 0x13B, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 3, 2 },
    { 3, 0x140, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 5, 0x195, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 2, 2 },
    { 3, 0x19A, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits5, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel5[14] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 0xA, 0xFE, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 0 },
    { 5, 0x2D, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 0, 2 },
    { 3, 0x32, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 5, 0x87, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 5, 2 },
    { 3, 0x8C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 5, 0xE1, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 7, 2 },
    { 3, 0xE6, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 5, 0x13B, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 3, 2 },
    { 3, 0x140, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 5, 0x195, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0xD5, 2, 2 },
    { 3, 0x19A, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits5, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel6[12] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 0xA, 0xE1, 0x40, 0, 0x1800, 0x100, -0x80, 0, 0, NULL, 0, 0x1C, 0x2001, 2 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xCA, 0x3065, 0 },
    { 8, 0, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 3, 0x32, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 8, 0x5A, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 1, 0 },
    { 3, 0x8C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 8, 0x10E, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 2, 0 },
    { 3, 0x140, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 8, 0x168, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 3, 0 },
    { 3, 0x19A, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec AdvStatsPanel7[12] = {
    { 4, 0, 0x1C2, 0, 0x56, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 5, 0xE1, 0x2C, 0, 0, 0, 0, 0, 0, NULL, 0, 0x1C, 4, 2 },
    { 0xA, 0x1B2, 0x3C, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0xCA, 0x3065, 0 },
    { 8, 0, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 3, 0x32, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits, 4, 0, 0, 0 },
    { 8, 0x5A, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 1, 0 },
    { 3, 0x8C, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits2, 4, 0, 0, 0 },
    { 8, 0x10E, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 2, 0 },
    { 3, 0x140, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits3, 4, 0, 0, 0 },
    { 8, 0x168, 0x10, 0, 0x700, 0, 0, 0, 0, NULL, 0, 0, 3, 0 },
    { 3, 0x19A, 0x40, 0, 0, 0, 0, 0, 0, &PauseDigits4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
HudBox PauseMenuPanel = { 0x10008001, 0x7C, -1, 0, 0x204, -1, 0, 0, 0x8080 };
s32 PauseMenuPanelUnused[2] = { 0x40004, 0x10008021 };
u16 StatsPageWanted = 0;
s32 ElapsedStart = 0;
s32 WinCounterSize = 0;
PlaceRec *PauseRec = 0;
PlaceRec **PauseRecSet = 0;
s32 PauseRecIndex = 0;
s16 PauseMenuY = 0;
u16 PauseToggles[4] = {0};
s16 PauseTogglesUnused[4] = {0};
u16 StatsPageShown = 0;
s32 PauseStatsAllowed = 0;
s32 StatsHasPage1 = 0;
s32 StatsHasPage3 = 0;
s32 StatsHasBonus = 0;
s32 WinCounterState = 0;
s32 WinCounterAnim = 0;
s32 PauseDelay = 0;
