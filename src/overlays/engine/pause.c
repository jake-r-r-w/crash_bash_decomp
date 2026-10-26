#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/pause.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_8007EE2C(s32 pad) {
    if ((((PadButtons *)Pads)[pad].pressed & 0x80) && PauseChoice != 0) {
        PauseChoice = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)Pads)[pad].pressed & 0x20) && PauseChoice == 0) {
        PauseChoice = 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)Pads)[pad].held & 9) != 9 &&
        (((PadButtons *)Pads)[pad].pressed & 8)) {
        s32 *p = (s32 *)GameInfo;
        PausePad = 0;
        PauseChoice = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)Pads)[pad].pressed & 0x4000;
}

s32 func_8007EFC0(s32 pad, s32 max) {
    if ((((PadButtons *)Pads)[pad].pressed & 0x10) && PauseChoice > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice--;
    }
    if ((((PadButtons *)Pads)[pad].pressed & 0x40) && PauseChoice < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice++;
    }
    if (PauseChoice < 0) {
        PauseChoice = 0;
    }
    if (PauseChoice > max) {
        PauseChoice = max;
    }
    if ((((PadButtons *)Pads)[pad].held & 9) != 9 &&
        (((PadButtons *)Pads)[pad].pressed & 8)) {
        s32 *p = (s32 *)GameInfo;
        PausePad = 0;
        PauseChoice = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)Pads)[pad].pressed & 0x4000;
}

s32 func_8007F1A8(s32 pad, s32 max) {
    if ((((PadButtons *)Pads)[pad].pressed & 0x10) && PauseChoice > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice--;
    }
    if ((((PadButtons *)Pads)[pad].pressed & 0x40) && PauseChoice < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        PauseChoice++;
    }
    if (PauseChoice < 0) {
        PauseChoice = 0;
    }
    if (PauseChoice > max) {
        PauseChoice = max;
    }
    return ((PadButtons *)Pads)[pad].pressed & 0x4008;
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

static inline PadButtons *ActivePad(void) {
    PadButtons *base = HPADS;

    return &base[PausePad];
}

s32 func_8007F314(void) {
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
    PadButtons *p;
    u8 *g;

    ret = 1;
    if (PSV->flags & 0x10) {
        return 0;
    }
    if ((PSV->unk30 & 0x8008) && ModeTypes[PSV->mode * 3] != 0x102) {
        return 0;
    }
    if (ModeTypes[PSV->mode * 3] == 0x101 || PSV->mode == 0x2B) {
        /* the unk74 test jumps into this copy: as a second return 1,
           jump2 would fold this one into it */
    ret1:
        return 1;
    }
    if (PSV->unk74 != 0) {
        goto ret1;
    }
    if (PSV->unk68 != 0) {
        if (PSV->unk14 != 0) {
            PSV->unk14--;
        } else {
            func_8008F068();
            PSV->unk68 = 0;
            PSV->flags &= ~0x02000000;
        }
    }
    if (!(GameLevelFlags & 0x100)) {
        s32 j;
        j = 0;
        key = GameLevelFlags & 0xFF;
        tbl = D_8009AF38;
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
                func_80028158();
                func_8002AC40();
                PlaySfx(0x193, NULL, NULL, 0x1000, 0x1E00, 0);
            } else {
                func_8002AC18();
            }
        }
        PSV->flags &= ~0x01000000;
    }
    if (PSV->flags & 1) {
        PSV->flags |= 0x01000000;
    }
    if (PSV->flags & 0x08000000) {
        return 0;
    }
    max = 4;
    if (PSV->unk32 & max) {
        max = 3;
    }
    if (PSV->state < 0 && (PSV->state & 0x10000)) {
        max++;
    }
    if (PSV->flags & 0x02007001) {
        switch ((u32)PSV->flags & 0x023F8FEC) {
        case 0:
            if (PSV->flags & 0x7000) {
                ret = 0;
                break;
            }
            if (func_8007EFC0(PausePad, max) != 0) {
                PSV->flags &= 0xFFC070D3;
                if (PSV->unk32 & 4) {
                    switch (PauseChoice) {
                    case 0:
                        PSV->flags &= 0x04000010;
                        PausePad = 0;
                        PauseChoice = 0;
                        func_8002AC18();
                        break;
                    case 1:
                        func_80083334(PSV->mode, kind, 0);
                        PSV->flags |= 0x80;
                        break;
                    case 2:
                        PSV->flags |= 0x100;
                        PauseChoice = 0;
                        PauseLastChoice = 0;
                        break;
                    case 3:
                        if (PSV->mode == 0x20) {
                            PSV->flags |= 4;
                            PauseChoice = 1;
                        } else {
                            PSV->flags |= 0x8000;
                            PauseChoice = 1;
                        }
                        break;
                    }
                } else {
                    switch (PauseChoice) {
                    case 0:
                        PausePad = 0;
                        PauseChoice = 0;
                        PSV->flags &= 0x04000010;
                        func_8002AC18();
                        break;
                    case 1:
                        func_80083334(PSV->mode, kind, 0);
                        PSV->flags |= 0x80;
                        break;
                    case 2:
                        PauseChoice = 0;
                        PauseLastChoice = 0;
                        PSV->flags |= 0x100;
                        break;
                    case 3:
                        PSV->flags |= 0x8000;
                        PauseChoice = 1;
                        break;
                    case 4:
                        PSV->flags |= 4;
                        PauseChoice = 1;
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
            if (func_8007EFC0(PausePad, max - 1) != 0) {
                switch (PauseChoice) {
                case 0:
                    PausePad = 0;
                    PauseChoice = 0;
                    PSV->flags &= 0x04000010;
                    func_8002AC18();
                    break;
                case 1:
                    PauseLastChoice = 0;
                    PauseChoice = 0;
                    PSV->flags |= 0x100;
                    break;
                case 2:
                    PauseChoice = 1;
                    PSV->flags |= 4;
                    break;
                case 3:
                    PauseChoice = 0;
                    PSV->flags |= 0x10000;
                    break;
                }
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            ret = 0;
            break;
        case 0x8000:
            if (func_8007EE2C(PausePad) != 0) {
                switch (PauseChoice) {
                case 0:
                    hi = 0;
                    if (PSV->unk32 & 0x10) {
                        for (i = 0; i < 4; i++) {
                            if (hi < GameInfo[i + 0xCC]) {
                                hi = GameInfo[i + 0xCC];
                            }
                        }
                        if (hi >= 2) {
                            g = &GameInfo[PSV->mode];
                            if (g[0xE4] == 0) {
                                g[0xAB]++;
                                g = &GameInfo[PSV->mode];
                                if (g[0xAB] >= 4) {
                                    g[0xAB] = 3;
                                }
                            }
                        }
                    }
                    PSV->flags |= 0x10;
                    PSV->unk30 |= 0x8002;
                    break;
                case 1:
                    PauseChoice = 0;
                    PSV->flags &= ~0x8000;
                    break;
                }
            }
            if (ActivePad()->pressed & 0x1000) {
                PauseChoice = 0;
                PSV->flags &= ~0x8000;
            }
            ret = 0;
            break;
        case 4:
        case 0xC:
            if (func_8007EE2C(PausePad) != 0) {
                switch (PauseChoice) {
                case 0:
                    func_8008F068();
                    break;
                case 1:
                    PauseChoice = 0;
                    PSV->flags &= ~4;
                    break;
                }
            }
            if (ActivePad()->pressed & 0x1000) {
                PauseChoice = 0;
                PSV->flags &= ~4;
            }
            ret = 0;
            break;
        case 0x40:
        case 0x240:
            if (PSV->mode != 0x25) {
                p = ActivePad();
                /* no ret = 0 exit right after a branch: jump2 would give
                   case 0's 0x7000 exit to it (MATCHING.md, Cross-jumping) */
                if ((p->held & 9) != 9) {
                    if (p->pressed & 0x4000) {
                        r = func_800832BC();
                        r ^= 1;
                        if (r) {
                            PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                            func_80095A04(0);
                            fl = PSV->flags;
                            if (fl & 0x200) {
                                ret = 1;
                                PSV->flags = fl & ~0x40;
                                PauseChoice = 0;
                            } else {
                                /* the mask set first and the cleared bit in its own
                                   statement, so the two ands stay apart */
                                s32 keep = 0x04000010;
                                s32 t = fl & ~0x40;

                                PSV->flags = t & keep;
                                PausePad = 0;
                                PauseChoice = 0;
                                func_8002AC18();
                                ret = 1;
                                PauseChoice = 0;
                            }
                            break;
                        }
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                    if (ActivePad()->pressed & 0x1000) {
                        func_80083244();
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
                    r = func_800832BC();
                    r ^= 1;
                    if (r) {
                        PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                        func_80095A04(0);
                        D_8009AF00 = 1;
                        PauseChoice = 0;
                        PSV->flags &= ~0x80;
                    } else {
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                if (ActivePad()->pressed & 0x1000) {
                    func_80083244();
                }
                ret = 0;
            } else {
                ret = 0;
            }
            break;
        case 0x02000000:
            if (joypad_idle_count <= 0) {
                PSV->unk14 = 0;
            }
            ret = 1;
            func_80080FBC();
            break;
        case 0x100:
        case 0x108:
            s = PauseLastChoice;
            switch (s) {
            case 1:
                {
                    switch (PauseChoice) {
                    case 0:
                        on = OPTV->unk8 != 0;
                        D_8009BD1C[0] = on;
                        if (ActivePad()->pressed & 0xA0) {
                            D_8009BD1C[0] = on ^ 1;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                            if ((u16)D_8009BD1C[0] != 0) {
                                OPTV->unk8 = s;
                                func_800222BC();
                            } else {
                                OPTV->unk8 = 0;
                                func_800222DC();
                            }
                        }
                        break;
                    case 1:
                        v = OPTV->unk0;
                        if (v > 0 && (ActivePad()->pressed & 0x80)) {
                            v -= 5;
                        }
                        if (v < 100 && (ActivePad()->pressed & 0x20)) {
                            v += 5;
                        }
                        OPTV->unk0 = v;
                        break;
                    case 2:
                        v = D_8005A608;
                        if (v > 0 && (ActivePad()->pressed & 0x80)) {
                            v -= 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (v < 100 && (ActivePad()->pressed & 0x20)) {
                            v += 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        D_8005A608 = v;
                        break;
                    }
                }
                if (func_8007EFC0(PausePad, 3) != 0 || (ActivePad()->pressed & 0x1000)) {
                    PauseChoice = 0;
                    PauseLastChoice = 0;
                    func_8002C11C();
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            case 2:
                for (i = 0; i < PSV->numPlayers; i++) {
                    if ((HPADS[i].pressed & 0x8000) && (((PadStick *)Pads)[i].flags & 8)) {
                        OPTV->unkC[i] = OPTV->unkC[i] == 0;
                        func_80028018(i, 10);
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                if (func_8007EFC0(PausePad, 0) != 0 || (ActivePad()->pressed & 0x1000)) {
                    PauseChoice = 1;
                    PauseLastChoice = 0;
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                break;
            default:
                if (func_8007EFC0(PausePad, 2) != 0) {
                    if (PauseChoice == 2) {
                        PauseLastChoice = 0;
                        PSV->flags &= ~0x100;
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
                    PSV->flags &= ~0x100;
                }
                break;
            }
            ret = 0;
            break;
        case 0x20000:
        case 0x20008:
            if (ActivePad()->pressed & 8) {
                PauseChoice = 0;
                PSV->flags = (PSV->flags & ~0x20000) | 0x10000;
            }
            ret = 0;
            break;
        case 0x40000:
        case 0x40008:
            if (ActivePad()->pressed & 1) {
                PSV->unk30 = 0x8008;
                PausePad = 2;
                NextGameMode = PSV->mode;
                PSV->flags &= 0xFFFBFFFE;
            }
            if (ActivePad()->pressed & 0x1008) {
                PSV->flags = (PSV->flags & ~0x40000) | 0x10000;
                PauseChoice = 2;
            }
            ret = 0;
            break;
        case 0x200:
            if (func_8007F1A8(0, 3) == 0) {
                ret = 1;
                func_80080FBC();
                break;
            }
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
            switch (PauseChoice) {
            case 1:
                func_80083334(PSV->mode, kind, 0);
                ret = 1;
                func_800831C4(D_8009BD18);
                PSV->flags |= 0x40;
                break;
            case 2:
                PausePad = 0;
                PauseChoice = 0;
                PSV->flags = (PSV->flags & ~0x200) | 0x10;
                PSV->unk30 |= 0x8002;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                break;
            case 3:
                PSV->flags &= ~0x200;
                func_8008F068();
                PausePad = 0;
                PauseChoice = 0;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                break;
            default:
                PausePad = 0;
                PauseChoice = 0;
                PSV->flags &= 0x04000010;
                func_8002AC18();
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
                PSV->flags &= ~0x21;
            } else {
                ret = 1;
                func_80080FBC();
            }
            break;
        default:
            ret = 0;
            PausePad = 0;
            PauseChoice = 0;
            PSV->flags |= 1;
            break;
        }
        /* in both branches: when neither runs, the hook doesn't either */
        if (GameExitFlags & 0x1000) {
            func_80082D90(ret);
        }
    } else if (!(PSV->flags & 0x04000000) || (PSV->unk30 & 0x1000)) {
        n = 1;
        PausePad = 0;
        D_8009AEF4 = -1;
        if (ModeTypes[PSV->mode * 3] != 0x102) {
            n = PSV->numPlayers;
        }
        for (i = 0; i < n; i++) {
            if ((HPADS[i].held & 9) != 9 && (HPADS[i].pressed & 8)) {
                if (!(Heroes[i].hero_status & 0xF800) || ModeTypes[PSV->mode * 3] == 0x102 || i == 0) {
                    PausePad = i;
                    D_8009AEF4 = i;
                    PauseChoice = 0;
                    PSV->flags |= 1;
                    i = 99;
                    if (ModeTypes[PSV->mode * 3] == 0x102) {
                        PSV->flags |= 9;
                    }
                    D_8009BD00 = 1;
                    func_80081470(1);
                    ret = 0;
                }
            }
        }
        if (GameExitFlags & 0x1000) {
            func_80082D90(ret);
        }
    }
    if (ret == 0) {
        func_80080FBC();
    }
    return ret;
}

void func_800809A0(HudEntry *e, s32 arg1) {
    s32 n;
    HudGameView *g;
    HudEngView *eng;
    s16 *row;
    s16 *row2;
    s32 i;
    s32 k;
    s32 sfx;

    for (n = 0; e->type != -1; e++) {
        g = (HudGameView *)GameInfo;
        switch (e->type) {
        case 1:
            if ((g->state & 0x80010000) != 0x80010000) {
                break;
            }
        case 0:
            if (D_8009AF00 == 0) {
                func_800243A0(e->x, (s16)(e->y + arg1),
                              ((u8 **)D_80059748[g->level])[e->idx], e->a, e->b,
                              PauseChoice == n);
            }
            n++;
            break;
        case 2:
            HUDBOX->unk10 = e->a;
            HUDBOX->unk14 = e->c;
            HUDBOX->unk6 = e->x + arg1;
            HUDBOX->unk4 = 0x7C;
            HUDBOX->unkA = 0x204;
            if ((g->state & 0x80010000) != 0x80010000) {
                HUDBOX->unkC = arg1 + (e->x + e->y);
            } else {
                HUDBOX->unkC = arg1 + (e->x + e->idx);
            }
            break;
        case 3:
            if (D_8009AF00 == 0) {
                func_800243A0(e->x, (s16)(e->y + arg1), (u8 *)e->idx, e->a, e->b, 0);
            }
            break;
        case 4:
            if (ModeTypes[g->mode * 3] == 0x102 && (g->flags & 0x40)) {
                goto clear;
            }
            func_800248A0(e->x, (s16)(e->y + arg1), e->a, e->b, e->idx);
            if (!(g->flags & 0xC0)) {
                goto clear;
            }
            if (g->flags & 0x40) {
                row = D_8009AE98[g->level];
                i = arg1 + e->y;
                HUDENG->x0 = row[0] + 0x140;
                HUDENG->y0 = i + row[1];
                HUDENG->x1 = row[2] + 0x140;
                HUDENG->y1 = i + row[3];
            } else {
                row2 = D_8009AE98[g->level];
                i = arg1 + e->y;
                HUDENG->x0 = row2[4] + 0x140;
                HUDENG->y0 = i + row2[5];
                HUDENG->x1 = row2[6] + 0x140;
                HUDENG->y1 = i + row2[7];
            }
            sfx = 0;
            if (g->unk32 & 0x10) {
                sfx = 0x2022;
            }
            if (g->unk32 & 0x80) {
                sfx = 0x201E;
            }
            if (g->unk32 & 0x40) {
                sfx = 0x201F;
            }
            if (g->unk32 & 0x20) {
                sfx = 0x2021;
                if (g->unk32 & 8) {
                    sfx = 0x2020;
                }
            }
            if (g->mode == 0x20) {
                sfx = 0;
            }
            if (g->unk32 & 0x100) {
                sfx = 0;
            }
            if (ModeTypes[g->mode * 3] == 0x102) {
                sfx = 0;
            }
            D_8009AC78 = sfx;
            D_8009AD20 = sfx;
            break;
        clear:
            D_8009AC78 = 0;
            D_8009AD20 = 0;
            break;
        case 5:
            if (D_8009AF00 != 0) {
                break;
            }
            func_800243A0(e->x, (s16)(e->y + arg1),
                          ((u8 **)D_80059748[g->level])[e->idx], e->a, e->b, 0);
            break;
        case 6:
            FontPage = e->x;
            break;
        case 7:
            HUDBOX->unk10 = e->a;
            HUDBOX->unk14 = e->c;
            HUDBOX->unk6 = e->x + arg1;
            HUDBOX->unk4 = 0x40;
            HUDBOX->unkA = 0x240;
            if ((g->state & 0x80010000) != 0x80010000) {
                HUDBOX->unkC = arg1 + (e->x + e->y);
            } else {
                HUDBOX->unkC = arg1 + (e->x + e->idx);
            }
            break;
        case 10: case 11: case 12: case 13: case 14:
        case 15: case 16: case 17: case 18: case 19:
            k = e->type - 10;
            if (((u16 *)D_8009BD1C)[k] != 1) {
                break;
            }
            if (PauseChoice != k) {
                goto draw;
            }
            func_800243A0(e->x, (s16)(e->y + arg1),
                          ((u8 **)D_80059748[g->level])[e->idx], e->c, e->d, 0);
            break;
        case 20: case 21: case 22: case 23: case 24:
        case 25: case 26: case 27: case 28: case 29:
            k = e->type - 20;
            if (((u16 *)D_8009BD1C)[k] != 0) {
                break;
            }
            if (PauseChoice != k) {
                goto draw;
            }
            func_800243A0(e->x, (s16)(e->y + arg1),
                          ((u8 **)D_80059748[g->level])[e->idx], e->c, e->d, 0);
            break;
        case 30: case 31: case 32: case 33: case 34:
        case 35: case 36: case 37: case 38: case 39:
            k = e->type - 30;
            if (((u16 *)D_8009BD1C)[k] != 9) {
                break;
            }
        draw:
            func_800243A0(e->x, (s16)(e->y + arg1),
                          ((u8 **)D_80059748[g->level])[e->idx], e->a, e->b, 0);
            break;
        }
    }
    if (D_8009AF00 != 0) {
        D_8009AF00--;
    }
    eng = (HudEngView *)PanelTab;
    eng->on0 = 1;
    eng->on1 = 1;
    eng->rot0 += 0x40;
    eng->rot1 += 0x40;
}

void func_80080FBC(void) {
    u8 *e;
    s32 i;
    s32 sel;

    if (MSGV->flags == 0) {
        return;
    }
    D_8009AC78 = 0;
    D_8009AD20 = 0;
    func_8007E7A4();
    func_8007BE84();
    D_8009BD18 = 0x70;
    e = D_8009BCE0;
    *(s16 *)(e + 6) = -1;
    *(s16 *)(e + 0xC) = -1;
    FontPage = 0;
    if (MSGV->flags & 0x08000002) {
        return;
    }
    if (MSGV->flags & 0x7000) {
        if (MSGV->flags & 0x2000) {
            func_800809A0(D_8005A4DC, 0x70);
            return;
        }
        if (MSGV->flags & 0x1000) {
            func_800809A0(D_8005A494, 0x70);
            return;
        }
        if (MSGV->flags & 0x4000) {
            func_800809A0(D_8005A3A4, 0x70);
            return;
        }
    }
    switch (MSGV->flags & 0x023F8FEC) {
    case 0:
        if (MSGV->mode == 0x20) {
            func_800809A0(D_8005981C, D_8009BD18);
            D_8009BD30 = 0;
            return;
        }
        if (MSGV->unk32 & 4) {
            func_800809A0(D_8005975C, D_8009BD18);
        } else {
            func_800809A0(D_800598DC, D_8009BD18);
        }
        D_8009BD30 = 1;
        break;
    case 8:
        func_800809A0(D_800599B4, D_8009BD18);
        D_8009BD30 = 1;
        break;
    case 0x8000:
        func_800809A0(D_80059C24, D_8009BD18);
        break;
    case 4:
    case 0xC:
        func_800809A0(D_80059ABC, D_8009BD18);
        break;
    case 0x40:
    case 0x240:
        func_800809A0(D_80059A5C, D_8009BD18);
        func_800831C4(D_8009BD18);
        break;
    case 0x80:
        func_800809A0(D_80059A8C, D_8009BD18);
        func_800831C4(D_8009BD18);
        break;
    case 0x100:
    case 0x108:
        sel = PauseLastChoice;
        switch (sel) {
        case 1:
            D_8009BD1C[1] = sel;
            D_8009BD1C[2] = sel;
            D_8009BD1C[0] = OPTV->unk8 != 0;
            func_8008D488(D_80059748[MSGV->level]->unkBC, OPTV->unk0);
            func_8008D488(D_80059748[MSGV->level]->unkC0, OPTV->unk4);
            func_800809A0(D_80059D2C, D_8009BD18);
            break;
        case 2:
            for (i = 0; i < MSGV->numPlayers; i++) {
                if (Pads[i].unk0 & 8) {
                    D_8009BD1C[i] = OPTV->unkC[i] != 0;
                } else {
                    D_8009BD1C[i] = 9;
                }
            }
            for (; i < 4; i++) {
                D_8009BD1C[i] = 9;
            }
            func_800809A0(D_80059E4C, D_8009BD18);
            break;
        default:
            func_800809A0(D_80059C9C, D_8009BD18);
            break;
        }
        break;
    case 0x200:
        func_800809A0(D_8005A14C, D_8009BD18);
        break;
    case 0x20:
        func_800809A0(D_8005A1C4, D_8009BD18);
        break;
    case 0x400:
        func_800809A0(D_8005A20C, D_8009BD18);
        break;
    case 0x2000000:
        if (MSGV->unkC & 0x10) {
            func_800809A0(D_8005A524, D_8009BD18);
        }
        break;
    }
}

void func_80081470(s32 arg0) {
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
        if (SCV->got0[i] != 0) {
            c0++;
        }
        if (SCV->got1[i] != 0) {
            c1++;
        }
        if (SCV->got2[i] != 0) {
            c2++;
        }
        if (SCV->got2[i] == 2) {
            c4++;
        }
        if (SCV->got3[i] != 0) {
            c3++;
        }
    }
    for (i = 0; i < 4; i++) {
        n[i] = SCV->score[i];
        if (mode == 5 && (SCV->unk32 & 2)) {
            n[i] += SCV->bonus[i];
        }
    }
    if (GameLevelFlags & 0xC0) {
        D_8009BD38 = 0;
    } else {
        D_8009BD38 = 1;
    }
    if (SCV->flags & 8) {
        D_8009BD38 = 0;
    }
    if (SCV->unk32 & 0x100) {
        D_8009BD38 = 0;
    }
    if (SCV->unk32 & 3) {
        D_8009BD34 = 0;
    } else {
        D_8009BD34 = 1;
    }
    if (GameLevelFlags & 2) {
        D_8009BD3C = 1;
    } else {
        D_8009BD3C = 0;
    }
    if (mode == 1 && D_8009BD34 == 0) {
        D_8009BD00 = 3;
        mode = 3;
    }
    if (mode == 3 && D_8009BD38 == 0) {
        D_8009BD00 = 1;
        mode = 1;
    }
    if (D_8009BD38 == 0 && D_8009BD34 == 0) {
        mode = 0;
    }
    D_8009BD2C = mode;
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
        D_8009AF58[1] = i % 10 + '0';
        i /= 10;
        D_8009AF58[0] = i % 10 + '0';
        if (SCV->unk4 & 0x2000) {
            if (SCV->flags & 8) {
                func_80095BEC(4, D_8009B788);
            } else {
                func_80095BEC(4, D_8009B590);
            }
        } else {
            switch (SCV->unkE0) {
            case 1:
                if (SCV->flags & 8) {
                    func_80095BEC(4, D_8009B3E0);
                } else {
                    func_80095BEC(4, D_8009B230);
                }
                break;
            case 2:
                if (SCV->flags & 8) {
                    func_80095BEC(4, D_8009B788);
                } else {
                    func_80095BEC(4, D_8009B590);
                }
                break;
            default:
                if (GamePause & 8) {
                    func_80095BEC(4, D_8009B0C8);
                } else {
                    func_80095BEC(4, D_8009AF60);
                }
                break;
            }
        }
        e->unk10.x = 0x60;
        e->unk10.y = 0x1F4;
        sub = e->sub;
        if (SCV->unk32 & 0x10) {
            sub[1].unk04 = sub[3].unk04;
        }
        if (SCV->unk32 & 0x40) {
            sub[1].unk04 = sub[5].unk04;
        }
        if (SCV->unk32 & 0x80) {
            sub[1].unk04 = sub[7].unk04;
        }
        if (SCV->unk32 & 0x20) {
            sub[1].unk04 = sub[9].unk04;
        }
        if (SCV->unk32 & 8) {
            sub[1].unk04 = sub[11].unk04;
        }
        if (D_8009BD38 != 0) {
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
        func_80095BEC(4, D_8009B980);
        e->unk10.x = 0x60;
        e->unk10.y = 0x1E0;
        sub = e->sub;
        e->flags |= 0x10008000;
        if (D_8009BD34 != 0) {
            sub[2].flags |= 0x8000;
        } else {
            sub[2].flags &= ~0x8000;
        }
        if (D_8009BD3C != 0) {
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
        func_80095BEC(4, D_8009BB30);
        e->unk10.x = 0x60;
        e->unk10.y = 0x1E0;
        sub = e->sub;
        e->flags |= 0x10008000;
        if (D_8009BD34 != 0) {
            sub[2].flags |= 0x8000;
        } else {
            sub[2].flags &= ~0x8000;
        }
        if (D_8009BD3C != 0) {
            sub[2].flags |= 0x8000;
        }
        break;
    }
}

void func_80081F64(s32 arg0) {
    EXTengine *e;
    s32 lim;
    PadButtons *pads;

    lim = 0x15E;
    if (ModeTypes[GameMode * 3] == 0x102) {
        lim = 0x140;
    }
    e = GetEngineSlot(4);
    if (D_8009BD30 == 0) {
        if (D_8009BD2C != 0) {
            D_8009BD2C = 9;
        }
    } else {
        switch (D_8009BD2C) {
        case 3:
        case 4:
            if (D_8009BD00 == 1) {
                D_8009BD2C = 2;
            }
            if (D_8009BD00 == 5) {
                D_8009BD2C = 6;
            }
            break;
        case 1:
        case 2:
        case 5:
        case 6:
            if (D_8009BD00 == 3) {
                D_8009BD2C = 4;
            }
            break;
        default:
            if (D_8009BD00 == 1) {
                D_8009BD2C = 2;
            }
            if (D_8009BD00 == 3) {
                D_8009BD2C = 4;
            }
            if (D_8009BD00 == 5) {
                D_8009BD2C = 6;
            }
            break;
        }
    }
    D_8009BD30 = 0;
    switch (D_8009BD2C) {
    case 1:
        if (e->unk10.y > lim) {
            e->unk10.y -= 0x20;
        }
        if (e->unk10.y < lim) {
            e->unk10.y = lim;
        }
        if (D_8009BD38 != 0 && (((PadButtons *)Pads)[PausePad].pressed & 0x1000)) {
            D_8009BD00 = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 3:
        if (e->unk10.y > lim) {
            e->unk10.y -= 0x20;
        }
        if (e->unk10.y < lim) {
            e->unk10.y = lim;
        }
        if (D_8009BD34 == 0 && D_8009BD3C == 0) {
            break;
        }
        pads = (PadButtons *)Pads;
        if (HPADS[PausePad].pressed & 0x1000) {
            if (GameLevelFlags & 2) {
                D_8009BD00 = 5;
            } else {
                D_8009BD00 = 1;
            }
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 5:
        if (e->unk10.y > lim) {
            e->unk10.y -= 0x20;
        }
        if (e->unk10.y < lim) {
            e->unk10.y = lim;
        }
        if (D_8009BD38 != 0 && (((PadButtons *)Pads)[PausePad].pressed & 0x1000)) {
            D_8009BD00 = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 2:
        if (e->unk10.y <= 0x1E0) {
            e->unk10.y += 0x20;
        } else {
            func_80095A04(4);
            func_80081470(1);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (D_8009BD38 != 0 && (((PadButtons *)Pads)[PausePad].pressed & 0x1000)) {
            D_8009BD00 = 3;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 4:
        if (e->unk10.y <= 0x1E0) {
            e->unk10.y += 0x20;
        } else {
            func_80095A04(4);
            func_80081470(3);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (D_8009BD34 == 0 && D_8009BD3C == 0) {
            break;
        }
        pads = (PadButtons *)Pads;
        if (HPADS[PausePad].pressed & 0x1000) {
            if (GameLevelFlags & 2) {
                D_8009BD00 = 5;
            } else {
                D_8009BD00 = 1;
            }
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 6:
        if (e->unk10.y <= 0x1E0) {
            e->unk10.y += 0x20;
        } else {
            func_80095A04(4);
            func_80081470(5);
            PlaySfx(0x194, 0, 0, 0x1000, 0x1E00, 0);
        }
        if (D_8009BD38 != 0 && (((PadButtons *)Pads)[PausePad].pressed & 0x1000)) {
            D_8009BD00 = 5;
            PlaySfx(0x195, 0, 0, 0x1000, 0x1E00, 0);
        }
        break;
    case 9:
        if (arg0 != 0) {
            e->unk10.y = 0x1E0;
        }
        if (e->unk10.y < 0x1E0) {
            e->unk10.y += 0x20;
            if (e->unk10.y < 0x1E0) {
                break;
            }
        }
        func_80095A04(4);
        D_8009BD2C = 0;
        break;
    }
}

void func_80082590(void) {
    GfxState *gs;
    u8 *e;

    if (GamePause & 0x7000) {
        gs = &RenderState;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (GamePause & 0x4000) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08(&MainViewport, &MainGfxCamera);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)&ScreenFade;   /* a scalar read: scheduled above the otzAdd store */
                func_8001A6D4(e, 0x10000000);
            }
        }
    } else if (GamePause & 1) {
        gs = &RenderState;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (!(GamePause & 0x40)) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08(&MainViewport, &MainGfxCamera);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)&ScreenFade;
                func_8001A6D4(e, 0x10000000);
            }
        }
        func_80018B08(&HudViewport, &HudCamera);
        func_8001C2C0((u8 *)GetEngineSlot(0));
        func_80018B08(&MainViewport, &MainGfxCamera);
        func_8001C2C0((u8 *)GetEngineSlot(4));
        func_8001C298(&PanelTab[5]);
        func_8001C298(&PanelTab[6]);
    }
}

void func_800826F8(s32 arg0) {
    EngineHintView *g;
    EngineHintView *g2;
    EngineRec24 **set;
    EngineRec24 *rec;

    WinSeqTimer = 0x4B;
    WinSeqStep = 4;
    WinSeqHero = 0x63;
    D_8009BD08 = 0x19;
    D_8009BD40 = 0;
    D_8009BD44 = 1;
    g = (EngineHintView *)GameInfo;
    g->flags &= 0xFDC07013;
    PauseDelay = 3;
    g->flags |= 0x04000000;
    if (g->unk68 != 0) {
        g->flags |= 0x02000000;
        g->unk14 = 0x384;
        g->unk5C[0] = 1;
        g->unk5C[1] = 1;
        g->unk5C[2] = 1;
        g->unk5C[3] = 1;
        return;
    }
    switch (g->unk32 & 0xE0) {
    case 0x80:
        if (g->shown[g->mode] & 0x80) {
            return;
        }
        set = ((EngineRec24 ****)D_8005AAB0)[g->mode][2];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            func_80095BEC(0, &rec->kind);
        }
        g->shown[g->mode] |= 0x80;
        PausePad = 0;
        g->flags |= 0x41;
        D_8009BD08 >>= 1;
        return;
    case 0x20:
        if (g->unk32 & 8) {
            if (g->shown[g->mode] & 8) {
                return;
            }
            set = ((EngineRec24 ****)D_8005AAB0)[g->mode][8];
            if (set != NULL) {
                PauseRecSet = set;
                rec = *set;
                PauseRecIndex = 0;
                PauseRec = rec;
                func_80095BEC(0, &rec->kind);
            }
            g->shown[g->mode] |= 8;
            PausePad = 0;
            g->flags |= 0x41;
            D_8009BD08 >>= 1;
        } else {
            if (g->shown[g->mode] & 0x20) {
                return;
            }
            set = ((EngineRec24 ****)D_8005AAB0)[g->mode][6];
            if (set != NULL) {
                PauseRecSet = set;
                rec = *set;
                PauseRecIndex = 0;
                PauseRec = rec;
                func_80095BEC(0, &rec->kind);
            }
            g->shown[g->mode] |= 0x20;
            PausePad = 0;
            g->flags |= 0x41;
            D_8009BD08 >>= 1;
        }
        return;
    case 0x40:
        if (g->shown[g->mode] & 0x40) {
            return;
        }
        set = ((EngineRec24 ****)D_8005AAB0)[g->mode][4];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            func_80095BEC(0, &rec->kind);
        }
        g->shown[g->mode] |= 0x40;
        PausePad = 0;
        g->flags |= 0x41;
        D_8009BD08 >>= 1;
        return;
    default:
        g2 = (EngineHintView *)GameInfo;
        if (g2->shown[g2->mode] & 0x10) {
            return;
        }
        set = ((EngineRec24 ****)D_8005AAB0)[g2->mode][0];
        if (set != NULL) {
            PauseRecSet = set;
            rec = *set;
            PauseRecIndex = 0;
            PauseRec = rec;
            func_80095BEC(0, &rec->kind);
        }
        g2->shown[g2->mode] |= 0x10;
        PausePad = 0;
        g2->flags |= 0x41;
        D_8009BD08 >>= 1;
        return;
    }
}

void func_80082AC4(void) {
    s32 i;
    u8 *p;
    u8 *g;
    u8 *q;
    u8 *r;

    D_8009BD08 = 0x1E;
    g = GameInfo;
    if (*(u16 *)(g + 0x32) & 0x100) {
        (g + *(s16 *)(g + 0x34))[0xAB] += 2;
        p = g + *(s16 *)(g + 0x34);
        if (p[0xAB] >= 0x20) {
            p[0xAB] = 0x1F;
        }
    }
    q = GameInfo;
    if (*(u16 *)(q + 0x32) & 0x1E0) {
        i = 3;
        r = q + i;
        for (; i >= 0; i--) {
            r[0xCC] = 0;
            r--;
        }
        if ((GameLevelFlags & 0x40) && (PanelCtrl & 1) && Clock_Time == 0) {
            D_8009BD40 = 6;
            D_8009BD44 = 3;
            WinSeqStep = 0x28;
            WinSeqHero = -1;
            WinSeqTimer = 0xF;
            WinSeqDelay = 0x1E;
            return;
        }
        D_8009BD40 = 6;
        D_8009BD44 = 3;
        WinSeqStep = 0x1E;
        WinSeqHero = -1;
        WinSeqTimer = 0xF;
        WinSeqDelay = 0x1E;
        return;
    }
    D_8009BD40 = 5;
}

void func_80082C34(void) {
    s32 t;
    s32 r;
    s32 u;

    switch (D_8009BD44) {
    case 1:
        t = counter_sub.unk20.x = (D_8009BD08 << 12) / 25;
        counter_sub.unk20.y = t;
        counter_sub.unk20.z = t;
        counter_sub.unk10 = (D_8009BD08 << 10) / 25 - 0x400;
        if (D_8009BD40 & 1) {
            counter_sub.unk12 = counter_sub.unk12 + 0x80;
        } else {
            counter_sub.unk12 = counter_sub.unk12 - 0x80;
        }
        if (D_8009BD40 == 4) {
            counter_sub.unk04 = 0x140;
        }
        if (counter_sub.unk20.x < 0x101) {
            counter_sub.unk20.x = 0x1000;
            counter_sub.unk20.y = 0x1000;
            counter_sub.unk20.z = 0x1000;
            counter_sub.unk12 = 0;
            counter_sub.unk10 = 0;
            counter_sub.unk04 = 0x140;
            counter_sub.flags &= 0xFFFF7FFF;
            if (D_8009BD40 == 4) {
                u8 *g = GameInfo;
                D_8009BD44 = 0;
                *(u16 *)(g + 0x30) &= 0xEFFF;
            }
        }
        D_8009ADE6 = 1;
        return;
    case 3:
        D_8009BD40 = 6;
        D_8009BD44 = 0;
        return;
    }
}

void func_80082D90(s32 arg0) {
    SLONG_XYZ pos;
    s32 d;
    s32 t;
    s32 r;

    if (!(counter_sub.flags & 0x8000)) {
        switch (D_8009BD40) {
        case 1:
            counter_sub.unk74 = 0x2004;
            PauseDelay = 3;
            D_8009BD44 = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A6, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            counter_sub.unk74 = 0x2003;
            PauseDelay = 3;
            D_8009BD44 = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A5, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
            counter_sub.unk74 = 0x2002;
            PauseDelay = 3;
            D_8009BD44 = 1;
            counter_sub.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&counter_sub.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A4, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PauseDelay = 3;
            counter_sub.unk74 = 0x2005;
            counter_sub.flags |= 0x8000;
            PlaySfx(0x1A7, NULL, NULL, 0x1000, 0x1E00, 0);
            ((EngineHintView *)GameInfo)->flags &= 0xFBFFFFFF;
            break;
        case 5:
            counter_sub.unk74 = 0x2006;
            counter_sub.unk20.x = 0x100;
            counter_sub.unk20.y = 0x100;
            counter_sub.unk20.z = 0x100;
            counter_sub.unk52 = 1;
            D_8009BD40 = 0xB;
            D_8009BD44 = 2;
            counter_sub.flags |= 0x8000;
            break;
        case 6:
            counter_sub.unk74 = 0x201B;
            counter_sub.unk20.x = 0x100;
            counter_sub.unk20.y = 0x100;
            counter_sub.unk20.z = 0x100;
            counter_sub.unk10 = 0;
            counter_sub.unk12 = 0;
            counter_sub.unk14 = 0;
            counter_sub.unk52 = 1;
            D_8009BD40 = 0xB;
            D_8009BD44 = 2;
            counter_sub.flags |= 0x8000;
            break;
        }
    }
    if (D_8009BD44 == 0) {
        counter_sub.flags &= 0xFFFF7FFF;
    }
    if (arg0 != 0) {
        if (PauseDelay > 0) {
            PauseDelay--;
        } else {
            func_80082C34();
            D_8009BD08--;
        }
    }
    if (D_8009BD40 == 0xB) {
        d = 0x1000 - counter_sub.unk20.x;
        counter_sub.unk20.x += d >> 3;
        counter_sub.unk20.y = counter_sub.unk20.x;
        counter_sub.unk20.z = counter_sub.unk20.x;
        counter_sub.unk12 = ((TrigTable[(((GameFrameCount << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        counter_sub.unk52 = 1;
        if (d < 0x100) {
            PlaySfx(0x1AF, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009BD40 = 0xC;
        }
    }
    if (D_8009BD40 == 0xC) {
        t = counter_sub.unk20.x + ((0x1000 - counter_sub.unk20.x) >> 3);
        counter_sub.unk20.x = t;
        counter_sub.unk20.y = t;
        counter_sub.unk20.z = t;
        counter_sub.unk12 = ((TrigTable[(((GameFrameCount << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        counter_sub.unk52 = 1;
    }
    if (D_8009BD08 <= 0) {
        D_8009BD08 = 0x19;
        if (D_8009BD44 == 1) {
            D_8009BD40++;
        }
    }
}

void func_800831C4(s32 arg0) {
    GetEngineSlot(0)->unk10.y = arg0;
}

s16 func_800831F0(void) {
    EngineRec24 *p = PauseRec;

    if (p != NULL) {
        do {
            if (p->kind != 0) {
                if (p->kind == 1) {
                    return p->val;
                }
            } else {
                return 0;
            }
            p++;
        } while (p != NULL);
    }
    return 0;
}

void func_80083244(void) {
    EngineRec24 *rec;
    s32 n;

    if (PauseRecSet != NULL) {
        if (PauseRecIndex != 0) {
            n = PauseRecIndex - 1;
            rec = PauseRecSet[n];
            PauseRecIndex = n;
            PauseRec = rec;
            func_80095BEC(0, &rec->kind);
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

s32 func_800832BC(void) {
    EngineRec24 *rec;

    if (PauseRecSet != NULL) {
        rec = PauseRecSet[PauseRecIndex + 1];
        if (rec != NULL) {
            PauseRecIndex = PauseRecIndex + 1;
            PauseRec = rec;
            func_80095BEC(0, &rec->kind);
            return 1;
        }
    }
    PauseRecSet = NULL;
    PauseRec = NULL;
    return 0;
}

void func_80083334(s32 arg0, s32 kind, s32 flag) {
    EngineRec24 ***t;
    EngineRec24 **set;
    EngineRec24 *rec;

    t = ((EngineRec24 ****)D_8005AAB0)[GameMode];
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
        func_80095BEC(0, &rec->kind);
    }
}

s32 func_80083488(void) {
    PadButtons *base = (PadButtons *)Pads;
    PadButtons *pad = &base[PausePad];
    if ((pad->held & 9) != 9) {
        if (pad->pressed & 0x4008) {
            return 1;
        }
    }
    return 0;
}

void func_800834E4(void) {
    s32 *p = (s32 *)GameInfo;
    PausePad = 0;
    PauseChoice = 0;
    p[4] &= 0x04000010;
    func_8002AC18();
}

void func_8008352C(void) {
    u8 *p = GameInfo;
    s32 a = *(s32 *)(p + 0x10);
    u16 b = *(u16 *)(p + 0x30);
    *(s32 *)(p + 0x10) = a | 0x10;
    *(u16 *)(p + 0x30) = b | 0x8002;
}

void func_80083550(void) {
    HeroHooks.unk0 = 0;
    HeroHooks.unk4 = D_8009D2E4;
    HeroHooks.unkC = D_8009D318;
    HeroHooks.unk10 = D_8009D354;
    HeroHooks.unk14 = D_8009D2F8;
    HeroHooks.unk20 = func_800859D4;
    HeroHooks.unk24 = func_80085D2C;
    HeroHooks.unk2C = func_80085C84;
    HeroHooks.unk30 = func_8008AA40;
    HeroHooks.unk34 = func_8008A9F0;
    HeroHooks.unk38 = func_8008680C;
    HeroHooks.unk3C = func_8008AB48;
    HeroHooks.unk40 = func_8008AB48;
    HeroHooks.unk4C = DefHero2ObjectCollision;
    HeroHooks.unk50 = func_8008671C;
    HeroHooks.unk54 = func_80086714;
    HeroHooks.unk64 = func_80084788;
    HeroHooks.unk60 = DefaultTouched;
    HeroHooks.unk8 = 0;
    HeroHooks.unk18 = 0;
    HeroHooks.unk1C = 0;
    HeroHooks.unk28 = 0;
    HeroHooks.unk44 = 0;
    HeroHooks.unk48 = 0;
    HeroHooks.unk5C = 0;
    HeroHooks.unk58 = 0;
    HeroHooks.unk68 = func_8008670C;
    AiBotTweakTable = D_8009D894;
    AiHumanTweakTable = D_8009D8A8;
    AiSkillTable = D_8009D8BC;
    AiHumanRoundTweakTable = D_8009D8DC;
    AiTeamSkillTable = D_8009D8D0;
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
