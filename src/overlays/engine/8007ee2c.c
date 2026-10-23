#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8007ee2c.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_8007EE2C(s32 pad) {
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x80) && D_8009AEEC != 0) {
        D_8009AEEC = 0;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x20) && D_8009AEEC == 0) {
        D_8009AEEC = 1;
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((((PadButtons *)D_80051338)[pad].held & 9) != 9 &&
        (((PadButtons *)D_80051338)[pad].pressed & 8)) {
        s32 *p = (s32 *)D_8005A614;
        D_8009AEF0 = 0;
        D_8009AEEC = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)D_80051338)[pad].pressed & 0x4000;
}

s32 func_8007EFC0(s32 pad, s32 max) {
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x10) && D_8009AEEC > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009AEEC--;
    }
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x40) && D_8009AEEC < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009AEEC++;
    }
    if (D_8009AEEC < 0) {
        D_8009AEEC = 0;
    }
    if (D_8009AEEC > max) {
        D_8009AEEC = max;
    }
    if ((((PadButtons *)D_80051338)[pad].held & 9) != 9 &&
        (((PadButtons *)D_80051338)[pad].pressed & 8)) {
        s32 *p = (s32 *)D_8005A614;
        D_8009AEF0 = 0;
        D_8009AEEC = 0;
        p[4] &= 0x04000010;
        func_8002AC18();
        return 0;
    }
    return ((PadButtons *)D_80051338)[pad].pressed & 0x4000;
}

s32 func_8007F1A8(s32 pad, s32 max) {
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x10) && D_8009AEEC > 0) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009AEEC--;
    }
    if ((((PadButtons *)D_80051338)[pad].pressed & 0x40) && D_8009AEEC < max) {
        PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009AEEC++;
    }
    if (D_8009AEEC < 0) {
        D_8009AEEC = 0;
    }
    if (D_8009AEEC > max) {
        D_8009AEEC = max;
    }
    return ((PadButtons *)D_80051338)[pad].pressed & 0x4008;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDB0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDBC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDCC);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDE0);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADDF8);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE10);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE20);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADE38);

#ifdef NON_MATCHING
s32 func_8007F314(void) {
    s32 ret;
    s32 kind;
    s32 max;
    s32 i;
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
    if ((PSV->unk30 & 0x8008) && D_8004DDD0[PSV->mode * 3] != 0x102) {
        return 0;
    }
    if (D_8004DDD0[PSV->mode * 3] == 0x101 || PSV->mode == 0x2B) {
    /* goto into this copy: jump2 would fold it into the later one */
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
    kind = 7;
    if (!(D_8005A646 & 0x100)) {
        for (i = 0; i < 7; i++) {
            kind = i;
            if ((D_8005A646 & 0xFF) == D_8009AF38[i]) {
                goto found;
            }
        }
        kind = 0;
    }
found:
    if (!(D_8005A624 & 0x660)) {
        s32 bit = D_8005A624 & 1;
        s32 toggle;

        if (D_8005A624 & 0x01000000) {
            toggle = bit ^ 1;
        } else {
            toggle = bit != 0;
        }
        if (toggle) {
            if (D_8005A624 & 1) {
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
            if (func_8007EFC0(D_8009AEF0, max) != 0) {
            PSV->flags &= 0xFFC070D3;
            if (PSV->unk32 & 4) {
                switch (D_8009AEEC) {
                case 0:
                    PSV->flags &= 0x04000010;
                    D_8009AEF0 = 0;
                    D_8009AEEC = 0;
                    func_8002AC18();
                    break;
                case 1:
                    func_80083334(PSV->mode, kind, 0);
                    PSV->flags |= 0x80;
                    break;
                case 2:
                    PSV->flags |= 0x100;
                    D_8009AEEC = 0;
                    D_8009AEE8 = 0;
                    break;
                case 3:
                    if (PSV->mode == 0x20) {
                        PSV->flags |= 4;
                        D_8009AEEC = 1;
                    } else {
                        PSV->flags |= 0x8000;
                        D_8009AEEC = 1;
                    }
                    break;
                }
            } else {
                switch (D_8009AEEC) {
                case 0:
                    D_8009AEF0 = 0;
                    D_8009AEEC = 0;
                    PSV->flags &= 0x04000010;
                    func_8002AC18();
                    break;
                case 1:
                    func_80083334(PSV->mode, kind, 0);
                    PSV->flags |= 0x80;
                    break;
                case 2:
                    D_8009AEEC = 0;
                    D_8009AEE8 = 0;
                    PSV->flags |= 0x100;
                    break;
                case 3:
                    D_8009AEEC = 1;
                    PSV->flags |= 0x8000;
                    break;
                case 4:
                    D_8009AEEC = 1;
                    PSV->flags |= 4;
                    break;
                }
            }
            /* case 8 keeps the PlaySfx copy in the target */
            goto sfx;
            }
            ret = 0;
            break;
        case 8:
            if (func_8007EFC0(D_8009AEF0, max - 1) != 0) {
            switch (D_8009AEEC) {
            case 0:
                D_8009AEF0 = 0;
                D_8009AEEC = 0;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                break;
            case 1:
                D_8009AEE8 = 0;
                D_8009AEEC = 0;
                PSV->flags |= 0x100;
                break;
            case 2:
                D_8009AEEC = 1;
                PSV->flags |= 4;
                break;
            case 3:
                D_8009AEEC = 0;
                PSV->flags |= 0x10000;
                break;
            }
            sfx:
            PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            ret = 0;
            break;
        case 0x8000:
            if (func_8007EE2C(D_8009AEF0) != 0) {
                switch (D_8009AEEC) {
                case 0:
                    hi = 0;
                    if (PSV->unk32 & 0x10) {
                        for (i = 0; i < 4; i++) {
                            if (hi < D_8005A614[i + 0xCC]) {
                                hi = D_8005A614[i + 0xCC];
                            }
                        }
                        if (hi >= 2) {
                            g = &D_8005A614[PSV->mode];
                            if (g[0xE4] == 0) {
                                g[0xAB]++;
                                g = &D_8005A614[PSV->mode];
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
                    D_8009AEEC = 0;
                    PSV->flags &= ~0x8000;
                    break;
                }
            }
            if (HPADS[D_8009AEF0].pressed & 0x1000) {
                D_8009AEEC = 0;
                PSV->flags &= ~0x8000;
            }
            ret = 0;
            break;
        case 4:
        case 0xC:
            if (func_8007EE2C(D_8009AEF0) != 0) {
                switch (D_8009AEEC) {
                case 0:
                    func_8008F068();
                    break;
                case 1:
                    D_8009AEEC = 0;
                    PSV->flags &= ~4;
                    break;
                }
            }
            if (HPADS[D_8009AEF0].pressed & 0x1000) {
                D_8009AEEC = 0;
                PSV->flags &= ~4;
            }
            ret = 0;
            break;
        case 0x40:
        case 0x240:
            if (PSV->mode != 0x25) {
                p = &HPADS[D_8009AEF0];
                if ((p->held & 9) == 9) {
                    ret = 0;
                    break;
                }
                if (p->pressed & 0x4000) {
                    r = func_800832BC();
                    r ^= 1;
                    if (r) {
                        PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                        func_80095A04(0);
                        ret = 1;
                        fl = PSV->flags;
                        if (fl & 0x200) {
                            PSV->flags = fl & ~0x40;
                            D_8009AEEC = 0;
                        } else {
                            PSV->flags = fl & ~0x40 & 0x04000010;
                            D_8009AEF0 = 0;
                            D_8009AEEC = 0;
                            func_8002AC18();
                            ret = 1;
                            D_8009AEEC = 0;
                        }
                        break;
                    }
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                ret = 0;
                if (HPADS[D_8009AEF0].pressed & 0x1000) {
                    func_80083244();
                }
            }
            break;
        case 0x80:
            p = &HPADS[D_8009AEF0];
            if ((p->held & 9) == 9) {
                ret = 0;
                break;
            }
            if (p->pressed & 0x4000) {
                r = func_800832BC();
                r ^= 1;
                if (r) {
                    PlaySfx(0x191, NULL, NULL, 0x1000, 0x1E00, 0);
                    func_80095A04(0);
                    D_8009AF00 = 1;
                    D_8009AEEC = 0;
                    PSV->flags &= ~0x80;
                } else {
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
            }
            ret = 0;
            if (HPADS[D_8009AEF0].pressed & 0x1000) {
                func_80083244();
            }
            break;
        case 0x02000000:
            if (D_80051604 <= 0) {
                PSV->unk14 = 0;
            }
            ret = 1;
            func_80080FBC();
            break;
        case 0x100:
        case 0x108:
            s = D_8009AEE8;
            switch (s) {
            case 1:
                {
                    switch (D_8009AEEC) {
                    case 0:
                        on = OPTV->unk8 != 0;
                        D_8009BD1C[0] = on;
                        if (HPADS[D_8009AEF0].pressed & 0xA0) {
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
                        if (v > 0 && (HPADS[D_8009AEF0].pressed & 0x80)) {
                            v -= 5;
                        }
                        if (v < 100 && (HPADS[D_8009AEF0].pressed & 0x20)) {
                            v += 5;
                        }
                        OPTV->unk0 = v;
                        break;
                    case 2:
                        v = D_8005A608;
                        if (v > 0 && (HPADS[D_8009AEF0].pressed & 0x80)) {
                            v -= 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        if (v < 100 && (HPADS[D_8009AEF0].pressed & 0x20)) {
                            v += 5;
                            PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                        }
                        D_8005A608 = v;
                        break;
                    }
                }
                if (func_8007EFC0(D_8009AEF0, 3) != 0 || (HPADS[D_8009AEF0].pressed & 0x1000)) {
                    D_8009AEEC = 0;
                    D_8009AEE8 = 0;
                    func_8002C11C();
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                ret = 0;
                break;
            case 2:
                for (i = 0; i < PSV->numPlayers; i++) {
                    if ((HPADS[i].pressed & 0x8000) && (((PadStick *)D_80051338)[i].flags & 8)) {
                        OPTV->unkC[i] = OPTV->unkC[i] == 0;
                        func_80028018(i, 10);
                        PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                }
                if (func_8007EFC0(D_8009AEF0, 0) != 0 || (HPADS[D_8009AEF0].pressed & 0x1000)) {
                    D_8009AEEC = 1;
                    D_8009AEE8 = 0;
                    PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                ret = 0;
                break;
            default:
                if (func_8007EFC0(D_8009AEF0, 2) != 0) {
                    if (D_8009AEEC == 2) {
                        D_8009AEE8 = 0;
                        PSV->flags &= ~0x100;
                    } else {
                        D_8009AEE8 = D_8009AEEC + 1;
                    }
                    if (D_8009AEEC == 0) {
                        continue_music();
                    }
                    D_8009AEEC = 0;
                    PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (HPADS[D_8009AEF0].pressed & 0x1000) {
                    D_8009AEEC = 0;
                    PSV->flags &= ~0x100;
                }
                ret = 0;
                break;
            }
            break;
        case 0x20000:
        case 0x20008:
            if (HPADS[D_8009AEF0].pressed & 8) {
                D_8009AEEC = 0;
                PSV->flags = (PSV->flags & ~0x20000) | 0x10000;
            }
            ret = 0;
            break;
        case 0x40000:
        case 0x40008:
            if (HPADS[D_8009AEF0].pressed & 1) {
                PSV->unk30 = 0x8008;
                D_8009AEF0 = 2;
                D_8009E5DC = PSV->mode;
                PSV->flags &= 0xFFFBFFFE;
            }
            if (HPADS[D_8009AEF0].pressed & 0x1008) {
                D_8009AEEC = 2;
                PSV->flags = (PSV->flags & ~0x40000) | 0x10000;
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
            switch (D_8009AEEC) {
            case 1:
                func_80083334(PSV->mode, kind, 0);
                ret = 1;
                func_800831C4(D_8009BD18);
                PSV->flags |= 0x40;
                break;
            case 2:
                D_8009AEF0 = 0;
                D_8009AEEC = 0;
                PSV->flags = (PSV->flags & ~0x200) | 0x10;
                PSV->unk30 |= 0x8002;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                break;
            case 3:
                PSV->flags &= ~0x200;
                func_8008F068();
                D_8009AEF0 = 0;
                D_8009AEEC = 0;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                break;
            default:
                ret = 1;
                D_8009AEF0 = 0;
                D_8009AEEC = 0;
                PSV->flags &= 0x04000010;
                func_8002AC18();
                D_8009AE3C = 0;
                break;
            }
            break;
        case 0x20:
        case 0x400:
            if (HPADS[0].pressed & 0x10) {
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                D_8009AEEC--;
            }
            if (HPADS[0].pressed & 0x40) {
                PlaySfx(0x190, NULL, NULL, 0x1000, 0x1E00, 0);
                D_8009AEEC++;
            }
            if (D_8009AEEC < 0) {
                D_8009AEEC = 0;
            }
            if (D_8009AEEC >= 2) {
                D_8009AEEC = 1;
            }
            if (HPADS[0].pressed & 0x4008) {
                PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
                ret = 1;
                D_8009AE3C = 0;
                D_8009AEF0 = 0;
                PSV->flags &= ~0x21;
            } else {
                ret = 1;
                func_80080FBC();
            }
            break;
        default:
            ret = 0;
            D_8009AEF0 = 0;
            D_8009AEEC = 0;
            PSV->flags |= 1;
            break;
        }
        /* The hook in both branches: jump2 cross-jumps the copies, and the
           breaks keep the switch end as their target until then. */
        if (D_8005A644 & 0x1000) {
            func_80082D90(ret);
        }
    } else if (!(PSV->flags & 0x04000000) || (PSV->unk30 & 0x1000)) {
        n = 1;
        D_8009AEF0 = 0;
        D_8009AEF4 = -1;
        if (D_8004DDD0[PSV->mode * 3] != 0x102) {
            n = PSV->numPlayers;
        }
        for (i = 0; i < n; i++) {
            if ((HPADS[i].held & 9) != 9 && (HPADS[i].pressed & 8)) {
                if (!(Heroes[i].hero_status & 0xF800) || D_8004DDD0[PSV->mode * 3] == 0x102 || i == 0) {
                    D_8009AEF0 = i;
                    D_8009AEF4 = i;
                    D_8009AEEC = 0;
                    PSV->flags |= 1;
                    i = 99;
                    if (D_8004DDD0[PSV->mode * 3] == 0x102) {
                        PSV->flags |= 9;
                    }
                    D_8009BD00 = 1;
                    func_80081470(1);
                    ret = 0;
                }
            }
        }
        if (D_8005A644 & 0x1000) {
            func_80082D90(ret);
        }
    }
    if (ret == 0) {
        func_80080FBC();
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/overlays/engine/8007ee2c", func_8007F314);
#endif

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
        g = (HudGameView *)D_8005A614;
        switch (e->type) {
        case 1:
            if ((g->state & 0x80010000) != 0x80010000) {
                break;
            }
        case 0:
            if (D_8009AF00 == 0) {
                func_800243A0(e->x, (s16)(e->y + arg1),
                              ((u8 **)D_80059748[g->level])[e->idx], e->a, e->b,
                              D_8009AEEC == n);
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
            if (D_8004DDD0[g->mode * 3] == 0x102 && (g->flags & 0x40)) {
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
            if (D_8004DDD0[g->mode * 3] == 0x102) {
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
            D_8005B2B4 = e->x;
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
            if (D_8009AEEC != k) {
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
            if (D_8009AEEC != k) {
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
    eng = (HudEngView *)D_80099290;
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
    D_8005B2B4 = 0;
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
        sel = D_8009AEE8;
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
                if (D_80051338[i].unk0 & 8) {
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
    if (D_8005A646 & 0xC0) {
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
    if (D_8005A646 & 2) {
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
        D_8009AF48[1] = i % 10 + '0';
        i /= 10;
        D_8009AF48[0] = i % 10 + '0';
        i = c1;
        D_8009AF4C[1] = i % 10 + '0';
        i /= 10;
        D_8009AF4C[0] = i % 10 + '0';
        i = c3;
        D_8009AF50[1] = i % 10 + '0';
        i /= 10;
        D_8009AF50[0] = i % 10 + '0';
        i = c2;
        D_8009AF54[1] = i % 10 + '0';
        i /= 10;
        D_8009AF54[0] = i % 10 + '0';
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
                if (D_8005A624 & 8) {
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
        D_8009AF48[1] = i % 10 + '0';
        i /= 10;
        D_8009AF48[0] = i % 10 + '0';
        i = n[1];
        D_8009AF4C[1] = i % 10 + '0';
        i /= 10;
        D_8009AF4C[0] = i % 10 + '0';
        i = n[2];
        D_8009AF50[1] = i % 10 + '0';
        i /= 10;
        D_8009AF50[0] = i % 10 + '0';
        i = n[3];
        D_8009AF54[1] = i % 10 + '0';
        i /= 10;
        D_8009AF54[0] = i % 10 + '0';
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
        D_8009AF48[1] = i % 10 + '0';
        i /= 10;
        D_8009AF48[0] = i % 10 + '0';
        i = n[1];
        D_8009AF4C[1] = i % 10 + '0';
        i /= 10;
        D_8009AF4C[0] = i % 10 + '0';
        i = n[2];
        D_8009AF50[1] = i % 10 + '0';
        i /= 10;
        D_8009AF50[0] = i % 10 + '0';
        i = n[3];
        D_8009AF54[1] = i % 10 + '0';
        i /= 10;
        D_8009AF54[0] = i % 10 + '0';
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
    if (D_8004DDD0[D_8005A648 * 3] == 0x102) {
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
        if (D_8009BD38 != 0 && (((PadButtons *)D_80051338)[D_8009AEF0].pressed & 0x1000)) {
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
        pads = (PadButtons *)D_80051338;
        if (HPADS[D_8009AEF0].pressed & 0x1000) {
            if (D_8005A646 & 2) {
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
        if (D_8009BD38 != 0 && (((PadButtons *)D_80051338)[D_8009AEF0].pressed & 0x1000)) {
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
        if (D_8009BD38 != 0 && (((PadButtons *)D_80051338)[D_8009AEF0].pressed & 0x1000)) {
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
        pads = (PadButtons *)D_80051338;
        if (HPADS[D_8009AEF0].pressed & 0x1000) {
            if (D_8005A646 & 2) {
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
        if (D_8009BD38 != 0 && (((PadButtons *)D_80051338)[D_8009AEF0].pressed & 0x1000)) {
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

    if (D_8005A624 & 0x7000) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (D_8005A624 & 0x4000) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
    } else if (D_8005A624 & 1) {
        gs = &D_80056998;
        *(s32 *)&gs->unk28[0] = 0;
        *(s32 *)&gs->unk28[4] = 0;
        if (!(D_8005A624 & 0x40)) {
            e = D_8009BCE0;
            if (*(s16 *)(e + 0xC) != -1) {
                func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
                gs->otzAdd = 0x200;
                gs->modelDp = *(s32 *)D_8009F644;
                func_8001A6D4(e, 0x10000000);
            }
        }
        func_80018B08(&D_8009F6A4, &D_8009F7EC);
        func_8001C2C0((u8 *)GetEngineSlot(0));
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0((u8 *)GetEngineSlot(4));
        func_8001C298(&D_8009959C);
        func_8001C298(&D_8009959C + 1);
    }
}

void func_800826F8(s32 arg0) {
    EngineHintView *g;
    EngineHintView *g2;
    EngineRec24 **set;
    EngineRec24 *rec;

    D_8009E0D0 = 0x4B;
    D_8009E0D2 = 4;
    D_8009E0D4 = 0x63;
    D_8009BD08 = 0x19;
    D_8009BD40 = 0;
    D_8009BD44 = 1;
    g = (EngineHintView *)D_8005A614;
    g->flags &= 0xFDC07013;
    D_8009BD48 = 3;
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
            D_8009BD10 = set;
            rec = *set;
            D_8009BD14 = 0;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
        }
        g->shown[g->mode] |= 0x80;
        D_8009AEF0 = 0;
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
                D_8009BD10 = set;
                rec = *set;
                D_8009BD14 = 0;
                D_8009BD0C = rec;
                func_80095BEC(0, &rec->kind);
            }
            g->shown[g->mode] |= 8;
            D_8009AEF0 = 0;
            g->flags |= 0x41;
            D_8009BD08 >>= 1;
        } else {
            if (g->shown[g->mode] & 0x20) {
                return;
            }
            set = ((EngineRec24 ****)D_8005AAB0)[g->mode][6];
            if (set != NULL) {
                D_8009BD10 = set;
                rec = *set;
                D_8009BD14 = 0;
                D_8009BD0C = rec;
                func_80095BEC(0, &rec->kind);
            }
            g->shown[g->mode] |= 0x20;
            D_8009AEF0 = 0;
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
            D_8009BD10 = set;
            rec = *set;
            D_8009BD14 = 0;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
        }
        g->shown[g->mode] |= 0x40;
        D_8009AEF0 = 0;
        g->flags |= 0x41;
        D_8009BD08 >>= 1;
        return;
    default:
        g2 = (EngineHintView *)D_8005A614;
        if (g2->shown[g2->mode] & 0x10) {
            return;
        }
        set = ((EngineRec24 ****)D_8005AAB0)[g2->mode][0];
        if (set != NULL) {
            D_8009BD10 = set;
            rec = *set;
            D_8009BD14 = 0;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
        }
        g2->shown[g2->mode] |= 0x10;
        D_8009AEF0 = 0;
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
    g = D_8005A614;
    if (*(u16 *)(g + 0x32) & 0x100) {
        (g + *(s16 *)(g + 0x34))[0xAB] += 2;
        p = g + *(s16 *)(g + 0x34);
        if (p[0xAB] >= 0x20) {
            p[0xAB] = 0x1F;
        }
    }
    q = D_8005A614;
    if (*(u16 *)(q + 0x32) & 0x1E0) {
        i = 3;
        r = q + i;
        for (; i >= 0; i--) {
            r[0xCC] = 0;
            r--;
        }
        if ((D_8005A646 & 0x40) && (D_80099174 & 1) && D_80099178 == 0) {
            D_8009BD40 = 6;
            D_8009BD44 = 3;
            D_8009E0D2 = 0x28;
            D_8009E0D4 = -1;
            D_8009E0D0 = 0xF;
            D_8009E0D6 = 0x1E;
            return;
        }
        D_8009BD40 = 6;
        D_8009BD44 = 3;
        D_8009E0D2 = 0x1E;
        D_8009E0D4 = -1;
        D_8009E0D0 = 0xF;
        D_8009E0D6 = 0x1E;
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
        t = D_8009AD94.unk20.x = (D_8009BD08 << 12) / 25;
        D_8009AD94.unk20.y = t;
        D_8009AD94.unk20.z = t;
        D_8009AD94.unk10 = (D_8009BD08 << 10) / 25 - 0x400;
        if (D_8009BD40 & 1) {
            D_8009AD94.unk12 = D_8009AD94.unk12 + 0x80;
        } else {
            D_8009AD94.unk12 = D_8009AD94.unk12 - 0x80;
        }
        if (D_8009BD40 == 4) {
            D_8009AD94.unk04 = 0x140;
        }
        if (D_8009AD94.unk20.x < 0x101) {
            D_8009AD94.unk20.x = 0x1000;
            D_8009AD94.unk20.y = 0x1000;
            D_8009AD94.unk20.z = 0x1000;
            D_8009AD94.unk12 = 0;
            D_8009AD94.unk10 = 0;
            D_8009AD94.unk04 = 0x140;
            D_8009AD94.flags &= 0xFFFF7FFF;
            if (D_8009BD40 == 4) {
                u8 *g = D_8005A614;
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

    if (!(D_8009AD94.flags & 0x8000)) {
        switch (D_8009BD40) {
        case 1:
            D_8009AD94.unk74 = 0x2004;
            D_8009BD48 = 3;
            D_8009BD44 = 1;
            D_8009AD94.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&D_8009AD94.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A6, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 2:
            D_8009AD94.unk74 = 0x2003;
            D_8009BD48 = 3;
            D_8009BD44 = 1;
            D_8009AD94.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&D_8009AD94.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A5, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
            D_8009AD94.unk74 = 0x2002;
            D_8009BD48 = 3;
            D_8009BD44 = 1;
            D_8009AD94.flags |= 0x8000;
            pos = *(SLONG_XYZ *)&D_8009AD94.unk04;
            pos.y -= 0x300;
            pos.x -= 0x180;
            PlaySfx(0x1A4, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 4:
            D_8009BD48 = 3;
            D_8009AD94.unk74 = 0x2005;
            D_8009AD94.flags |= 0x8000;
            PlaySfx(0x1A7, NULL, NULL, 0x1000, 0x1E00, 0);
            ((EngineHintView *)D_8005A614)->flags &= 0xFBFFFFFF;
            break;
        case 5:
            D_8009AD94.unk74 = 0x2006;
            D_8009AD94.unk20.x = 0x100;
            D_8009AD94.unk20.y = 0x100;
            D_8009AD94.unk20.z = 0x100;
            D_8009AD94.unk52 = 1;
            D_8009BD40 = 0xB;
            D_8009BD44 = 2;
            D_8009AD94.flags |= 0x8000;
            break;
        case 6:
            D_8009AD94.unk74 = 0x201B;
            D_8009AD94.unk20.x = 0x100;
            D_8009AD94.unk20.y = 0x100;
            D_8009AD94.unk20.z = 0x100;
            D_8009AD94.unk10 = 0;
            D_8009AD94.unk12 = 0;
            D_8009AD94.unk14 = 0;
            D_8009AD94.unk52 = 1;
            D_8009BD40 = 0xB;
            D_8009BD44 = 2;
            D_8009AD94.flags |= 0x8000;
            break;
        }
    }
    if (D_8009BD44 == 0) {
        D_8009AD94.flags &= 0xFFFF7FFF;
    }
    if (arg0 != 0) {
        if (D_8009BD48 > 0) {
            D_8009BD48--;
        } else {
            func_80082C34();
            D_8009BD08--;
        }
    }
    if (D_8009BD40 == 0xB) {
        d = 0x1000 - D_8009AD94.unk20.x;
        D_8009AD94.unk20.x += d >> 3;
        D_8009AD94.unk20.y = D_8009AD94.unk20.x;
        D_8009AD94.unk20.z = D_8009AD94.unk20.x;
        D_8009AD94.unk12 = ((D_80068BD4[(((D_8004E0D8 << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        D_8009AD94.unk52 = 1;
        if (d < 0x100) {
            PlaySfx(0x1AF, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009BD40 = 0xC;
        }
    }
    if (D_8009BD40 == 0xC) {
        t = D_8009AD94.unk20.x + ((0x1000 - D_8009AD94.unk20.x) >> 3);
        D_8009AD94.unk20.x = t;
        D_8009AD94.unk20.y = t;
        D_8009AD94.unk20.z = t;
        D_8009AD94.unk12 = ((D_80068BD4[(((D_8004E0D8 << 5) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3 >> 9) << 12) / 360;
        D_8009AD94.unk52 = 1;
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
    EngineRec24 *p = D_8009BD0C;

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

    if (D_8009BD10 != NULL) {
        if (D_8009BD14 != 0) {
            n = D_8009BD14 - 1;
            rec = D_8009BD10[n];
            D_8009BD14 = n;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
            PlaySfx(0x195, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

s32 func_800832BC(void) {
    EngineRec24 *rec;

    if (D_8009BD10 != NULL) {
        rec = D_8009BD10[D_8009BD14 + 1];
        if (rec != NULL) {
            D_8009BD14 = D_8009BD14 + 1;
            D_8009BD0C = rec;
            func_80095BEC(0, &rec->kind);
            return 1;
        }
    }
    D_8009BD10 = NULL;
    D_8009BD0C = NULL;
    return 0;
}

void func_80083334(s32 arg0, s32 kind, s32 flag) {
    EngineRec24 ***t;
    EngineRec24 **set;
    EngineRec24 *rec;

    t = ((EngineRec24 ****)D_8005AAB0)[D_8005A648];
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
        D_8009BD10 = set;
        rec = *set;
        D_8009BD14 = 0;
        D_8009BD0C = rec;
        func_80095BEC(0, &rec->kind);
    }
}

s32 func_80083488(void) {
    PadButtons *base = (PadButtons *)D_80051338;
    PadButtons *pad = &base[D_8009AEF0];
    if ((pad->held & 9) != 9) {
        if (pad->pressed & 0x4008) {
            return 1;
        }
    }
    return 0;
}

void func_800834E4(void) {
    s32 *p = (s32 *)D_8005A614;
    D_8009AEF0 = 0;
    D_8009AEEC = 0;
    p[4] &= 0x04000010;
    func_8002AC18();
}

void func_8008352C(void) {
    u8 *p = D_8005A614;
    s32 a = *(s32 *)(p + 0x10);
    u16 b = *(u16 *)(p + 0x30);
    *(s32 *)(p + 0x10) = a | 0x10;
    *(u16 *)(p + 0x30) = b | 0x8002;
}

void func_80083550(void) {
    D_8009D6E8.unk0 = 0;
    D_8009D6E8.unk4 = D_8009D2E4;
    D_8009D6E8.unkC = D_8009D318;
    D_8009D6E8.unk10 = D_8009D354;
    D_8009D6E8.unk14 = D_8009D2F8;
    D_8009D6E8.unk20 = func_800859D4;
    D_8009D6E8.unk24 = func_80085D2C;
    D_8009D6E8.unk2C = func_80085C84;
    D_8009D6E8.unk30 = func_8008AA40;
    D_8009D6E8.unk34 = func_8008A9F0;
    D_8009D6E8.unk38 = func_8008680C;
    D_8009D6E8.unk3C = func_8008AB48;
    D_8009D6E8.unk40 = func_8008AB48;
    D_8009D6E8.unk4C = func_80086724;
    D_8009D6E8.unk50 = func_8008671C;
    D_8009D6E8.unk54 = func_80086714;
    D_8009D6E8.unk64 = func_80084788;
    D_8009D6E8.unk60 = func_80086704;
    D_8009D6E8.unk8 = 0;
    D_8009D6E8.unk18 = 0;
    D_8009D6E8.unk1C = 0;
    D_8009D6E8.unk28 = 0;
    D_8009D6E8.unk44 = 0;
    D_8009D6E8.unk48 = 0;
    D_8009D6E8.unk5C = 0;
    D_8009D6E8.unk58 = 0;
    D_8009D6E8.unk68 = func_8008670C;
    D_8009D8F4 = D_8009D894;
    D_8009D8F8 = D_8009D8A8;
    D_8009D8FC = D_8009D8BC;
    D_8009D904 = D_8009D8DC;
    D_8009D900 = D_8009D8D0;
    D_8009D908 = 0;
    D_8009E0D8 = 0xD;
    D_8009E0DA = 0xE;
    D_8009E0DC = 0xF;
    D_8009E0DE = 0x10;
    D_8009E0E0 = 0x11;
    D_8009E0E2 = 0x13;
    D_8009E0E4 = 0x14;
    D_8009E0E6 = 0xC;
}

void func_80083708(void) {
    s32 i;
    s32 j;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *e;
    EXTengine *fx;
    subengine *sub;
    s32 name;
    NAS *nas;
    NAS *hn;
    EXTheromove *mv;
    EXTbotai *ai;
    D_8009D87C_t *c;
    HeroSetupView *g;
    u8 *p;
    u8 *q;
    s32 m;
    s32 k;
    u16 ang;
    s32 mask;
    s32 fxmask;
    HeroInfo *hb;
    s8 tm;
    s32 idx;
    s32 fxv;

    i = 0;
    m = 0;
    k = 0;
    D_8009D864 = -1;
    D_8009D866 = 0;
    D_8009D872 = 0;
    D_8009D868 = 0;
    D_8009D874 = 0;
    D_8009D86C = 0;
    D_8009D878 = 0;
    D_8009D758 = 4;
    D_8009D75C = 4;
    D_8009D760 = 0;
    HSV->unk30 &= 0xF7FF;
    hb = Heroes;
    while (1) {
        if (i >= 4) {
            break;
        }
        nas = (NAS *)((u8 *)D_8005AA1C + m * 16);
        h = &hb[i];
        h->hero = nas;
        *nas = NASTemplate;
        hn = h->hero;
        if (NASActiveTail != NULL) {
            NASActiveTail->Tag.Child = hn;
            hn->Tag.Parent = NASActiveTail;
            NASActiveTail = hn;
        } else {
            NASActiveTail = hn;
            NASActiveHead = hn;
        }
        h->hero->NASid = 1;
        h->hero->Class = 0x1400;
        h->hero->Type = 3;
        h->hero_eng = NULL;
        h->hero_sub = NULL;
        h->hero_shadow = NULL;
        h->hero_powerup = NULL;
        h->hero_fx = NULL;
        h->hero_move = NULL;
        h->hero_anim = NULL;
        h->hero_ai = NULL;
        h->hero_object = NULL;
        h->hero_health = 0x14;
        g = HSV;
        g->unk10 = 0;
        g = (HeroSetupView *)((u8 *)g + i);
        h->hero_status = 0;
        h->hero_strength = 0x14;
        h->hero_immunity = 0;
        h->hero_immunity_level = 0;
        h->hero_hit = 0;
        h->hero_last_mode = 0;
        h->hero_mode = 0;
        tm = g->unk58[0];
        h->hero_idle_count = 600;
        h->hero_harm_player = -1;
        h->hero_pick_delay = 0;
        h->hero_aseq = 0;
        h->hero_harm_Nas = NULL;
        h->hero_harm_value = 0;
        h->hero_harm_fraction = 0;
        h->hero_teammate = tm;
        h->hero_harm_flags = 0;
        h->hero_harm_sfx = 0;
        h->hero_ctrl = 0;
        h->hero_score = 0;
        h->hero_score_displayed = 0;
        h->hero_pu = 0;
        h->hero_pu_timer = 0;
        h->hero_pu_lock = 0;
        h->hero_attack_timer = 0;
        h->hero_attack_delay = 0;
        h->hero_pu_action = 0;
        h->hero_pu_action_timer = 0;
        h->hero_weapon_count = 0;
        h->hero_weapon_timer = 0;
        h->hero_attack_counter = 0;
        D_8009D524[i] = 0;
        h->hero_name = g->unk50[0];
        c = (D_8009D87C_t *)((s16 *)D_8009D87C + k);
        c->unk0 = 0x10;
        c->unk2 = 0x10;
        c->unk4 = 0x10;
        if (h->hero_name == 9) {
            h->hero_status = 0x1000;
        }
        eng = AddExtEngine(h->hero, 1);
        if (eng == NULL) {
            goto next;
        }
        sub = eng->sub;
        h->hero_eng = eng;
        h->hero_sub = sub;
        eng->unk5D = 0;
        eng->unk10 = D_8005B2B8[i].pos;
        h->hero_eng->unk10.y = World_floor_level;
        eng->OldXYZ = eng->unk10;
        h->hero_anim = func_800265DC(h->hero);
        name = h->hero_name;
        if ((u32)(name - 1) < 8) {
            h->hero_sub->unk6C = D_80052A54 + ((u16 *)D_8009D6E8.unk4)[name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
        sub->unk74 = D_8009D3A4[name];
        FetchDefaultColBox(eng, 0x50);
        if (D_8009D6E8.unk0 != 0) {
            ((void (*)(s32))D_8009D6E8.unk0)(i);
        } else {
            SetHeroAction((HeroInfo *)i, 0);
        }
        e = AddExtEngine(h->hero, 2);
        if (e != NULL) {
            h->hero_shadow = e;
            if (D_8009D6E8.unk18 != NULL) {
                ((void (*)(s32))D_8009D6E8.unk18)(i);
            } else {
                e->sub->unk6C = D_80052F5C;
                e->sub->unk74 = 0x2001;
                e->unk10 = eng->unk10;
                e->flags |= 0x8041;
                e->sub->unk76 = 0x800;
                e->sub->flags |= 0x40000000;
            }
        }
        fx = AddExtEngine(h->hero, 3);
        if (fx != NULL) {
            h->hero_powerup = fx;
            fx->unk10 = eng->unk10;
            sub = fx->sub;
            fx->unk5D = 0;
            if (sub != NULL) {
                sub->unk6C = D_80052F5C;
                sub->unk74 = 0;
                mask = ~0x8000;
                sub->flags &= mask;
            }
        }
        fx = AddExtEngine(h->hero, 3);
        if (fx != NULL) {
            h->hero_fx = fx;
            fx->unk10 = eng->unk10;
            fx->unk5D = 0;
            func_8001D390(fx, 3);
            sub = fx->sub;
            fxv = 0x2002;
            while (sub != NULL) {
                idx = ((u16 *)D_8009D6E8.unk4)[name];
                fxmask = ~0x8000;
                sub->flags &= fxmask;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->unk6C = D_80052A54 + idx * 0x38;
                if (name == 3) {
                    sub->unk74 = fxv;
                } else if (name == 4) {
                    sub->unk74 = fxv;
                } else {
                    sub->unk74 = 0;
                }
                sub = sub->next;
            }
        }
        if (h->hero_move == NULL) {
            mv = AddExt(h->hero, 0x54, 0x400000F0);
            if (mv != NULL) {
                h->hero_move = mv;
                mv->forward_speed = 0;
                mv->reactive_speed = 0;
                mv->forward_angle = 0;
                mv->facing_angle = 0;
                mv->reactive_angle = 0;
                mv->run_walk = 1;
                mv->jump_ctrl = 3;
                mv->velocity_calc = 1;
                mv->velocity_enable = 1;
                mv->x_vel = 0;
                mv->x_max = 0;
                mv->x_acc = 0;
                mv->x_dec = 0;
                mv->x_dir = 0;
                mv->jump_vel = 0;
                mv->jump_max = 0;
                mv->jump_acc = 0;
                mv->jump_dec = 0;
                mv->jump_dir = 0;
                mv->z_vel = 0;
                mv->z_max = 0;
                mv->z_acc = 0;
                mv->z_dec = 0;
                mv->z_dir = 0;
                ang = D_8005B2B8[i].rot.x;
                eng->unk1E = ang;
                mv->facing_angle = ang;
                mv->forward_angle = ang;
                mv->HitBy = -1;
            }
        }
        if (h->hero_ai == NULL) {
            ai = AddExt(h->hero, 0x44, 0x400000F4);
            if (ai != NULL) {
                h->hero_ai = ai;
                ai->AImode = 0;
                ai->AIaborted_mode = 0;
                ai->AItimer1 = 0;
                ai->AItimer2 = 0;
                ai->AItimer3 = 0;
                ai->AIang = 0;
                ai->AIpad = 0;
                ai->pTargetNas = NULL;
                ai->TargetPlayer = -1;
                ai->TargetXYZ.z = 0;
                ai->TargetXYZ.y = 0;
                ai->TargetXYZ.x = 0;
                ai->AIinitial_delay = Random(0x3C) + 0x1E;
                ai->AIsleep = 8;
                j = 7;
                p = (u8 *)ai + j;
                do {
                    p[0x34] = 0;
                    j--;
                    p--;
                } while (j >= 0);
                j = 7;
                q = (u8 *)ai + j;
                do {
                    q[0x3C] = 0;
                    j--;
                    q--;
                } while (j >= 0);
            }
        }
        if (h->hero_status != 0x1000) {
            h->hero->Camera.width = 0x200;
            h->hero->Camera.height = 0x500;
            h->hero->Camera.look_ahead = 0x400;
            func_80026A04((u8 *)h->hero);
        }
    next:
        i++;
        m += 7;
        k += 3;
    }
    Setup_panel();
    func_80085650();
    func_80086FA0();
}

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF60);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF68);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF70);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF78);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF80);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF88);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF90);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADF98);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADFA4);

INCLUDE_RODATA("asm/nonmatchings/overlays/engine/8007ee2c", D_800ADFAC);

void func_80083E8C(void) {
    HeroInfo *h;
    HeroInfo *b;
    u16 *pc;
    HeroInfo *c;
    HeroInfo *d;
    HeroInfo *e;
    s32 r;
    s32 i;
    s32 id;
    s32 last;
    s32 ok;
    u16 status;
    u16 harm;
    u16 st;
    u16 lvl;
    subengine *sub;
    s32 bits;
    s32 pad;

    r = Random(0x10);
    last = -1;
    i = 0;
    D_8009D758 = 0;
    D_8009D75C = 0;
    for (;; i++) {
        HeroInfo *a;

        if (i >= 4) {
            break;
        }
        id = D_8009D764[r][i];
        a = &Heroes[id];
        status = a->hero_status;
        if (status & 0x1080) {
            a->hero_shadow->flags = 0;
            a->hero_eng->flags = 0;
            goto next;
        }
        if (status & 0x40) {
            goto next;
        }
        if (status & 0xFF00) {
            a->hero_immunity = 0;
            a->hero_eng->flags |= 0x8000;
            HPADS[id].unk50 = 0;
            HPADS[id].unk54 = 0;
            goto reset;
        }
        if (HLV->state < 0 && (HLV->state & 4) &&
            (pad = D_800515F8 - 1, HPADS[pad].held & 0x400)) {
            a->hero_status = status | 0x2000;
            SetHeroAction((HeroInfo *)id, (u16)D_8009E0E2);
            goto reset;
        }
        b = &Heroes[id];
        if (b->hero_status & 0xFF00) {
            goto reset;
        }
        ok = 1;
        if (b->hero_immunity | b->hero_hit) {
            lvl = b->hero_harm_flags & 0x300;
            if (b->hero_immunity_level < lvl) {
                b->hero_immunity_level = lvl;
            } else {
                ok = 0;
            }
        } else {
            b->hero_immunity_level = 0;
        }
        h = &Heroes[id];
        harm = h->hero_harm_flags;
        if (harm & 0x1000) {
            h->hero_status |= 0x2000;
            SetHeroAction((HeroInfo *)id, (u16)D_8009E0E2);
            h->hero_immunity = 0;
            h->hero_health = 0;
            goto reset;
        }
        if (ok && harm) {
            h->hero_immunity_level = harm & 0x300;
            if (h->hero_harm_sfx != 0) {
                PlaySfx(h->hero_harm_sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (h->hero_pu == 0x12) {
                h->hero_pu_timer = 0;
                SetHeroAction((HeroInfo *)id, (u16)D_8009E0D8);
            } else {
                D_8009D524[id] = h->hero_harm_value;
                h->hero_health -= h->hero_harm_value >> 8;
                h->hero_health -= h->hero_harm_fraction >> 8;
                h->hero_harm_fraction = (u8)h->hero_harm_fraction;
                h->hero_harm_fraction += (u8)h->hero_harm_value;
                SetHeroAction((HeroInfo *)id, (u16)D_8009E0D8);
                h->hero_immunity = 0;
                if (h->hero_harm_flags & 0x8000) {
                    ((void (*)(s32, s32, s32))func_80027F7C)(id, 4, 0xFF);
                } else {
                    func_80028018(id, 0xC);
                    ((void (*)(s32, s32, s32))func_80027F7C)(id, 0xC, 0xFF);
                }
            }
        }
        h = &Heroes[id];
        if (h->hero_health < 0) {
            h->hero_health = 0;
        }
        if (h->hero_hit != 0) {
            if (--h->hero_hit == 0) {
                if (h->hero_health <= 0) {
                    if (!(HLV->state & 2)) {
                        h->hero_status |= 0x2000;
                        SetHeroAction((HeroInfo *)id, (u16)D_8009E0E2);
                        h->hero_immunity = 0;
                    }
                } else {
                    h->hero_immunity = 0x28;
                }
            }
            if (!(HLV->unk32 & 0x1000)) {
                HPADS[id].unk50 = 0;
                HPADS[id].unk54 = 0;
            }
        }
    reset:
        c = &Heroes[id];
        st = c->hero_status;
        c->hero_harm_Nas = NULL;
        c->hero_harm_player = -1;
        c->hero_harm_value = 0;
        c->hero_harm_flags = 0;
        if (st & 0x8000) {
            c->hero_eng->flags = 0;
            func_80026980(c->hero);
            continue;
        }
        if (!(st & 0x2000)) {
            last = id;
            D_8009D758++;
        } else if (HLV->unk30 & 0x800) {
            func_80026980(c->hero);
        } else {
            D_8009D75C++;
        }
        d = &Heroes[id];
        if (d->hero_frozen != 0) {
            d->hero_frozen--;
        }
        if (d->hero_attack_timer != 0) {
            d->hero_attack_timer--;
        } else if (d->hero_attack_delay != 0) {
            d->hero_attack_delay--;
        }
        e = &Heroes[id];
        if (e->hero_immunity != 0 &&
            (e->hero_immunity--, e->hero_immunity % 3 == 0)) {
            e->hero_eng->flags &= ~0x8000;
        } else {
            e->hero_eng->flags |= 0x8000;
        }
        sub = Heroes[id].hero_fx->sub;
        if (sub != NULL) {
            sub->flags &= ~0x8000;
        }
        if (!(HLV->state & 0x100) && HLV->out[id] != 0) {
            ((void (*)(s32))D_8009D6E8.unk38)(id);
        }
        h = &Heroes[id];
        if (h->hero_pu != 0x15 || (D_8004E0D8 & 1)) {
            if (HLV->flags & 0x4000000) {
                HPADS[id].unk50 = 0;
                HPADS[id].unk54 = 0;
            }
            ((void (*)(s32))D_8009D6E8.unk20)(id);
            ((void (*)(s32))HeroActionTable[h->hero_mode].pad)(id);
            ((void (*)(s32))D_8009D6E8.unk24)(id);
            if (D_8009D6E8.unk28 != NULL) {
                ((void (*)(s32))D_8009D6E8.unk28)(id);
            }
        }
        ((void (*)(EXTengine *))D_8009D6E8.unk68)(Heroes[id].hero_eng);
        func_8008672C(id);
        if (D_8009D6E8.unk5C != NULL) {
            ((void (*)(s32))D_8009D6E8.unk5C)(id);
        }
    next:;
    }
    if (HLV->unk32 & 0x100) {
        if (!(D_8009D6E0 & 0x8000)) {
            if (!(D_8009D6E0 & 0x2000)) {
                D_8009D758++;
            } else if (!(HLV->unk30 & 0x800)) {
                D_8009D75C++;
            }
            pc = (u16 *)D_8009D6DC;
            if (pc[4] != 0) {
                pc[4]--;
            }
        }
    }
    if (!(HLV->state & 0x20)) {
        bits = 1;
        if (HLV->state & 0x800) {
            bits = 0x201;
        }
        if (!(HLV->unk32 & 0x200)) {
            ((void (*)(s32, s32))D_8009D6E8.unk64)(last, bits);
        }
        if (HLV->unk32 & 0x400) {
            HLV->unk30 |= bits;
        }
    }
    if (!(HLV->unk30 & 0x801) && D_8009D760 == 0) {
        switch (HLV->mode) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 30:
            if (D_8009D758 + D_8009D75C < 3) {
                D_8009D760 = 1;
                func_8002C1C0(0x6E, 0xF0);
            }
            break;
        }
    }
}

void func_80084788(s32 idx, u16 arg1) {
    HeroInfo *h;
    HeroInfo *m;
    s32 i;
    s32 n;
    s32 best;

    if (WINV->flags & 0x801) {
        return;
    }
    if (WINV->unk32 & 0xE0) {
        if (idx == -1) {
            WINV->winner = 0;
            WINV->flags |= arg1;
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                if (!(h->hero_status & 0xFA00)) {
                    h->hero_status |= 0x400;
                }
            }
        } else {
            n = 0;
            for (i = 0; i < 4; i++) {
                h = &Heroes[i];
                if (!(h->hero_status & 0xF800) && WINV->out[i] == 0) {
                    n++;
                }
            }
            if (n == 0) {
                WINV->winner = 0;
                WINV->flags |= arg1;
                for (i = 0; i < 4; i++) {
                    h = &Heroes[i];
                    if (!(h->hero_status & 0xFA00)) {
                        h->hero_status |= 0x400;
                    }
                }
                return;
            }
            if (WINV->out[idx] != 0) {
                return;
            }
        }
    }
    best = 0;
    if (idx == -1) {
        if (!(WINV->unk32 & 0xE0)) {
            switch (WINV->mode) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
            case 7:
            case 8:
            case 18:
            case 19:
            case 20:
            case 21:
            case 30:
            case 32:
                if (WINV->teams != 0 && WINV->mode != 0x20) {
                    HeroInfo *hb = Heroes;
                    s32 t0 = WINV->team0;
                    s32 t1;
                    s32 dead0, dead1;
                    u16 mate0, mate1;
                    s32 h0, h1;

                    m = &hb[t0];
                    dead0 = m->hero_status & 0xF800;
                    h0 = 0;
                    if (!dead0) {
                        h0 = m->hero_health;
                    }
                    mate0 = m->hero_teammate;
                    h = &Heroes[mate0];
                    if (!(h->hero_status & 0xF800)) {
                        h0 += h->hero_health;
                    }
                    t1 = WINV->team1;
                    h = &Heroes[t1];
                    dead1 = h->hero_status & 0xF800;
                    h1 = 0;
                    if (!dead1) {
                        h1 = h->hero_health;
                    }
                    mate1 = h->hero_teammate;
                    h = &Heroes[mate1];
                    if (!(h->hero_status & 0xF800)) {
                        h1 += h->hero_health;
                    }
                    if (h0 == h1) {
                        idx = -1;
                    } else if (h0 > h1) {
                        idx = mate0;
                        if (!dead0) {
                            idx = t0;
                        }
                    } else {
                        idx = mate1;
                        if (!dead1) {
                            idx = t1;
                        }
                    }
                } else {
                    for (i = 0; i < 4; i++) {
                        h = &Heroes[i];
                        if (!(h->hero_status & 0xF800)) {
                            if (best < h->hero_health) {
                                best = h->hero_health;
                                idx = i;
                            } else if (h->hero_health == best) {
                                idx = -1;
                            }
                        }
                    }
                }
                break;
            case 4:
            case 9:
                D_8009D6DC[2] |= 0x200;
                break;
            }
            if (idx != -1) {
                HeroInfo *b1 = Heroes;
                HeroInfo *w = &b1[idx];

                w->hero_status |= 0x200;
                if (WINV->teams != 0 && WINV->mode != 0x20) {
                    m = &b1[(u16)w->hero_teammate];
                    if (!(m->hero_status & 0xA000)) {
                        m->hero_status |= 0x200;
                    }
                }
                WINV->winner = Heroes[idx].hero_name;
                WINV->flags |= 0x800;
            } else {
                WINV->winner = 0;
                WINV->flags |= arg1;
            }
        }
    } else {
        WinnerView *g;
        HeroInfo *base = Heroes;

        h = &base[idx];
        g = WINV;
        n = (u16)h->hero_teammate;
        if (g->teams != 0 && g->mode != 0x20) {
            idx = D_8009D758;
            if (idx < 3) {
                m = &base[(u16)n];
                if (m->hero_status & 0xA000) {
                    if (idx < 2) {
                        h->hero_status |= 0x200;
                        g->winner = h->hero_name;
                        g->flags |= 0x800;
                    }
                } else {
                    h->hero_status |= 0x200;
                    m->hero_status |= 0x200;
                    g->winner = h->hero_name;
                    g->flags |= 0x800;
                }
            }
        } else if (D_8009D758 < 2) {
            Heroes[idx].hero_status |= 0x200;
            WINV->winner = Heroes[idx].hero_name;
            WINV->flags |= 0x800;
        }
    }
    if (D_8005A644 & 0x800) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00)) {
                h->hero_status |= 0x400;
            }
        }
    }
    if (D_8009D908 == 0) {
        D_8009D908 = 1;
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00)) {
                if (WINV->out[i] == 0) {
                    D_8009D908 = 0;
                }
                if (WINV->teams != 0 && WINV->out[h->hero_teammate] == 0) {
                    D_8009D908 = 0;
                }
            }
        }
    }
}

void func_80084E04(s32 player, NAS *nas, s32 attacker, u16 flags, u16 sfx) {
    HeroInfo *h;
    u8 *base;
    u8 *g;
    s32 dist;
    s32 v;

    if (Heroes[player].hero_status & 0xFF00) {
        return;
    }
    dist = D_8009D87C[player].unk0;
    base = D_8005A614;
    g = base + player;
    v = 0x20 - (dist + g[0xCC]);
    if (v < 0) {
        v = 0;
    }
    if (v >= 0x20) {
        v = 0x1F;
    }
    if (g[0x5C] == 0) {
        v = D_8009D4A4[v] - base[0x9B];
    } else {
        v = 0;
    }
    if (D_8009D908 != 0) {
        v = 0;
    }
    if (v < 0) {
        v = 0;
    }
    if (v >= 4) {
        v = 3;
    }
    if (((EngineGameView *)D_8005A614)->flags & 3) {
        v = 0;
    }
    if (*(s32 *)(D_8005A614 + 4) & 1) {
        return;
    }
    if (flags & 0x800) {
        v = flags & 0xFF;
    } else {
        v = D_8009D3B8[flags & 0xF][v];
    }
    if (!(((EngineGameView *)D_8005A614)->flags & 3)) {
        if (D_8005A614[player + 0x5C] != 0 && (u32)(dist -= 0x18) < 9) {
            v = D_8009D400[v >> 8][dist];
        }
    }
    if (flags & 0x1000) {
        v = 0x7FFFF;
        Heroes[player].hero_immunity = 0;
    }
    if (Heroes[player].hero_harm_value < v) {
        h = &Heroes[player];
        h->hero_harm_value = v;
        h->hero_harm_flags = flags;
        h->hero_harm_Nas = nas;
        h->hero_harm_player = attacker;
        h->hero_harm_sfx = sfx;
    }
}

s32 func_8008502C(player)
s32 player;
{
    HeroInfo *h;
    EXTanimator *anim = Heroes[player].hero_anim;
    subengine *sub;
    s32 ret;
    u16 cmd;

    ret = 0;
    if (--anim->Aspeed < 0) {
        h = &Heroes[player];
        do {
            if (anim->Seq_count != 0 && anim->Currentframe != 0) {
                cmd = anim->Currentframe + 1;
                anim->Seq_count--;
                continue;
            }
            cmd = ((hero_seq *)anim->Atable)[++anim->Aindex];
            if (!(cmd & 0x8000)) {
                break;
            }
            switch (cmd) {
            case 0x8002:
                ret = ((s16 *)anim->Atable)[++anim->Aindex];
                cmd = h->hero_sub->unk74;
                break;
            case 0x8003:
                ret = ((s16 *)anim->Atable)[++anim->Aindex];
                break;
            case 0x8004:
                SetHeroAction((HeroInfo *)player, ((s16 *)anim->Atable)[++anim->Aindex]);
                return ret;
            case 0x8005:
                return SetHeroSeq((HeroInfo *)player, ((s16 *)anim->Atable)[++anim->Aindex]);
            case 0x8001:
                h->hero_anim->Atable = (u8 *)h->hero_anim->Atable + (h->hero_anim->Aindex * 2 + 2);
                anim->Aindex = 0;
                break;
            case 0x8000:
                anim->Aindex = ((s16 *)anim->Atable)[++anim->Aindex];
                do {
                    do {
                        do {
                            if (ret == 0) {
                                ret = 2;
                            }
                        } while (0);
                    } while (0);
                } while (0);
                break;
            case 0x8006:
                h->hero_sub->unk6C = &D_80052A54[((s16 *)anim->Atable)[++anim->Aindex] * 0x38];
                break;
            case 0x8007:
                anim->Seq_count = ((s16 *)anim->Atable)[++anim->Aindex] - 1;
                anim->Currentframe = 0;
                break;
            case 0x8008:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 0x8009:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 0x800A:
                PlaySfx(((s16 *)anim->Atable)[++anim->Aindex], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
                break;
            case 0x800B:
                if (h->hero_eng->sfx != NULL) {
                    StopSfx(h->hero_eng->sfx);
                }
                break;
            case 0x8017:
                func_80084E04(player, NULL, -1, 0x1000, 0);
                break;
            case 0x8018:
                anim->Masterspeed = ((s16 *)anim->Atable)[++anim->Aindex];
                break;
            case 0x8019:
                h->hero_move->jump_ctrl |= ((u8 *)anim->Atable)[++anim->Aindex * 2];
                break;
            case 0x801A:
                h->hero_move->jump_ctrl &= ~((u8 *)anim->Atable)[++anim->Aindex * 2];
                break;
            }
        } while (cmd & 0x8000);
        sub = anim->sub;
        if (sub == NULL) {
            sub = Heroes[player].hero_sub;
        }
        sub->unk74 = cmd;
        anim->Currentframe = cmd;
        anim->Aspeed = anim->Masterspeed;
    }
    return ret;
}

s32 SetHeroSeq(HeroInfo *hero, s32 seq) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + (s32)hero;
    u16 name = h->hero_name;

    if ((u32)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return func_8008502C(hero, seq);
    }
    return 0;
}

s32 func_80085584(s32 hero, u16 name, s32 seq) {
    HeroInfo *h;

    if ((u16)(name - 1) < 8) {
        hero_seq **t = HeroSeqTables[name];

        HeroInfo *base = Heroes;

        h = base + hero;
        h->hero_aseq = seq;
        h->hero_anim->Atable = t[seq];
        h->hero_anim->Aspeed = -1;
        h->hero_anim->Aindex = 0;
        h->hero_anim->Masterspeed = *(u16 *)h->hero_anim->Atable;
        h->hero_anim->Seq_count = 0;
        return func_8008502C(hero);
    }
    return 0;
}

void func_80085650(void) {
    s32 i;
    s32 j;
    s32 off;
    s32 k;
    s32 tab;
    s32 m;
    s32 base;
    s32 *p;
    s32 *q;
    s32 t;
    s32 r;

    i = 0;
    tab = (s32)D_8009D764;
    off = 0;
    do {
        j = 0;
        m = off;
        do {
            *(s32 *)(m + tab) = j;
            j++;
            m += 4;
        } while (j < 4);
        j = 0;
        base = off;
        k = off;
        do {
            q = (s32 *)((Random(4) * 4 + base) + tab);
            p = (s32 *)(k + tab);
            t = *p;
            *p = *q;
            *q = t;
            k += 4;
        } while (++j < 4);
        i++;
        off += 16;
    } while (i < 16);
}

static inline void HeroAnimSlotSet(volatile s16 *id, u16 *seq, volatile s32 *a, volatile s32 *p, volatile s32 *p2, s32 hero, u16 n) {
    subengine *s;
    volatile D_8009D6FC_t *t;
    s32 q;
    s32 v;

    s = Heroes[hero].hero_anim->sub;
    if (s == NULL) {
        s = Heroes[hero].hero_sub;
    }
    *p = (s32)s->unk6C;
    do {
    } while (0);
    q = *p2;
    *id = hero;
    t = (volatile D_8009D6FC_t *)(n * sizeof(D_8009D6FC_t) + (u32)*(D_8009D6FC_t * volatile *)&D_8009D6FC);
    v = ((D_8009D6FC_t *)t)->a;
    *a = v;
    *seq = t->b;
    func_800165F8((u8 *)q, (u8 *)((D_8009D6FC_t *)t)->a);
}

void func_80085718(s32 arg0, u16 arg1) {
    if (D_8009D86C == 0) {
        HeroAnimSlotSet(&D_8009D864, &D_8009D866, &D_8009D868, &D_8009D86C, (volatile s32 *)((u8 *)&D_8009D878 - 12), arg0, arg1);
    } else if (D_8004DDD0[D_8005A648 * 3] != 0x104) {
        HeroAnimSlotSet(&D_8009D870, &D_8009D872, &D_8009D874, &D_8009D878, (volatile s32 *)((u8 *)&D_8009D868 + 16), arg0, arg1);
    }
}

s32 func_80085890(s32 arg0) {
    s32 i;

    if (D_8009D864 == arg0) {
        if (Heroes[arg0].hero_aseq == D_8009D866) {
            return D_8009D866;
        }
        if (D_8009D86C != 0) {
            if (dbase_mesh_anim_name_loaded((u8 *)D_8009D86C, (u8 *)D_8009D868) != 0) {
                SetHeroSeq((HeroInfo *)arg0, D_8009D866);
                i = 3;
                do {
                    i -= 1;
                } while (i >= 0);
            }
        }
    }
    if (D_8009D870 == arg0 && Heroes[arg0].hero_aseq != D_8009D872 && D_8009D878 != 0) {
        if (dbase_mesh_anim_name_loaded((u8 *)D_8009D878, (u8 *)D_8009D874) != 0) {
            SetHeroSeq((HeroInfo *)arg0, D_8009D872);
            i = 3;
            do {
                i -= 1;
            } while (i >= 0);
            return i;
        }
    }
}

void func_800859D4(s32 arg0) {
    HeroInfo *hb;
    HeroInfo *hb2;
    PadStick *pb;
    HeroInfo *h;
    EXTheromove *m;
    PadStick *pad;
    u16 *row;
    u16 speed;
    HeroInfo *h2;
    u16 rw;
    s32 vc;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    if (h->hero_frozen != 0) {
        ((PadStick *)D_80051338)[arg0].dirHeld2 = 0;
        ((PadStick *)D_80051338)[arg0].dirPressed = 0;
    }
    pb = (PadStick *)D_80051338;
    pad = pb + arg0;
    if ((pad->flags & 1) || h->hero_ai->AImode != 0) {
        if (pad->dirHeld2 & 0xF0) {
            m->forward_angle = *(u16 *)&pad->angle + 0x800;
            if (m->velocity_enable != 0) {
                rw = m->run_walk;
                if (h->hero_object != NULL) {
                    if (rw != 0) {
                        rw--;
                    }
                }
                row = D_8009D6F4;
                hb2 = Heroes;
                h2 = hb2 + arg0;
                speed = (row + h2->hero_name * 3)[rw];
                if (h2->hero_pu == 0x13) {
                    speed += (row + h2->hero_name * 3)[2];
                }
                vc = m->velocity_calc;
                if (vc != 1) {
                    if (vc != 0) {
                        if (vc < 4) {
                            m->x_max = (speed * D_80068BD4[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                            m->z_max = (speed * D_80068BD4[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
                        }
                    }
                }
                m->forward_speed = speed;
            }
        } else {
            if (D_8005A614[arg0 + 0x5C] != 0) {
                if (pad->dirHeld2 & 0x100) {
                    m->forward_angle -= 0x100;
                }
                if (pad->dirHeld2 & 0x200) {
                    m->forward_angle += 0x100;
                }
            }
            m->x_max = 0;
            m->z_max = 0;
            m->facing_angle = m->forward_angle = m->forward_angle & 0xFFF;
        }
    }
    m->forward_angle &= 0xFFF;
    if (abs(func_800154A0(m->forward_angle, m->facing_angle) & 0xFFFF) > 0x110) {
        m->facing_angle = (m->forward_angle + 0x100) & 0xE00;
    }
}

void func_80085C84(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
        }
        h->hero_ctrl &= 0xFFFE;
        h->hero_eng->unk5E = 1;
    }
}

void func_80085D2C(s32 arg0) {
    HeroInfo *h;
    EXTengine *e;
    EXTheromove *m;
    s32 step;
    HeroInfo *base;

    base = Heroes;
    h = base + arg0;
    e = h->hero_eng;
    m = h->hero_move;
    ((void (*)(s32))D_8009D6E8.unk2C)(arg0);
    func_800860D4(arg0);
    switch (m->velocity_calc) {
    case 2:
        m->x_vel += m->x_acc - m->x_dec;
        if (m->x_vel < 0) {
            m->x_vel = 0;
        }
        if (m->x_vel > m->x_max) {
            m->x_vel = m->x_max;
        }
        if (m->x_dir != 0) {
            e->unk10.x -= m->x_vel;
        } else {
            e->unk10.x += m->x_vel;
        }
        m->z_vel += m->z_acc - m->z_dec;
        if (m->z_vel < 0) {
            m->z_vel = 0;
        }
        if (m->z_vel > m->z_max) {
            m->z_vel = m->z_max;
        }
        if (m->z_dir != 0) {
            e->unk10.z -= m->z_vel;
        } else {
            e->unk10.z += m->z_vel;
        }
        break;
    case 3:
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
            }
            if (m->x_vel < m->x_max) {
                m->x_vel += step;
                if (m->x_vel > m->x_max) {
                    m->x_vel = m->x_max;
                }
            } else {
                m->x_vel -= step;
                if (m->x_vel < m->x_max) {
                    m->x_vel = m->x_max;
                }
            }
        }
        if (m->z_vel != m->z_max) {
            if (m->z_max != 0) {
                step = m->z_acc;
            } else {
                step = m->z_dec;
            }
            if (m->z_vel < m->z_max) {
                m->z_vel += step;
                if (m->z_vel > m->z_max) {
                    m->z_vel = m->z_max;
                }
            } else {
                m->z_vel -= step;
                if (m->z_vel < m->z_max) {
                    m->z_vel = m->z_max;
                }
            }
        }
        e->unk10.z += m->z_vel;
        e->unk10.x += m->x_vel;
        break;
    case 9:
        break;
    case 1:
    default:
        e->unk10.x -= (m->forward_speed * D_80068BD4[(m->forward_angle - 0x400) & 0xFFF].unk2) >> 12;
        e->unk10.z -= (m->forward_speed * D_80068BD4[m->forward_angle & 0xFFF].unk2) >> 12;
        if (m->reactive_speed != 0) {
            e->unk10.x -= (m->reactive_speed * D_80068BD4[(m->reactive_angle - 0x400) & 0xFFF].unk2) >> 12;
            e->unk10.z -= (m->reactive_speed * D_80068BD4[m->reactive_angle & 0xFFF].unk2) >> 12;
            m->reactive_speed -= 4;
            if (m->reactive_speed < 0) {
                m->reactive_speed = 0;
            }
        }
        break;
    }
}

void func_800860D4(s32 arg0) {
    HeroInfo *b0;
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *m;
    HeroInfo *h2;
    s32 dec;
    s32 v;
    s32 y;

    b0 = Heroes;
    h = b0 + arg0;
    if ((m = h->hero_move)->jump_max == 0) {
        return;
    }
    if (m->jump_dir != 0) {
        dec = m->jump_dec;
        if ((m->jump_ctrl & 1) && D_8005A614[arg0 + 0x5C] == 0 &&
            !(((PadButtons *)D_80051338)[arg0].unk50 & 0x4000) && m->jump_vel >= 0x41) {
            dec = 0x28;
        }
        base = Heroes;
        h2 = base + arg0;
        v = h2->hero_move->jump_vel - dec;
        h2->hero_move->jump_vel = v;
        if (v < 0) {
            h2->hero_move->jump_vel = 0;
            h2->hero_move->jump_dir = 0;
            switch (h2->hero_mode) {
            case 0x33:
            case 0x39:
            case 0x3B:
            case 0x3C:
            case 0x3F:
                return;
            case 0x34:
                SetHeroActionInline(arg0, 0x35);
                break;
            case 0x78:
                SetHeroActionInline(arg0, 0x7A);
                break;
            default:
                SetHeroActionInline(arg0, 5);
                break;
            }
        } else {
            h2->hero_eng->unk10.y -= h2->hero_move->jump_vel;
        }
    } else {
        m->jump_vel += m->jump_acc;
        if (h->hero_move->jump_max < m->jump_vel) {
            h->hero_move->jump_vel = h->hero_move->jump_max;
        }
        h->hero_eng->unk10.y += h->hero_move->jump_vel;
        if (h->hero_move->jump_ctrl & 0x10) {
            y = h->hero_eng->unk10.y;
            if (World_floor_level < y) {
                h->hero_sub->unk08 += y - World_floor_level;
                h->hero_eng->unk10.y = World_floor_level;
            }
        }
    }
}

s32 func_800863D0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    s32 dist;
    s32 dy;

    int new_var;
    s32 dz;
    s32 far;

    dist = abs(h->hero_eng->unk10.x - h->hero_eng->OldXYZ.x);
    dz = abs(h->hero_eng->unk10.z - h->hero_eng->OldXYZ.z);
    new_var = 0x40;
    dist = (dist + new_var) + dz;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy == 0x7FFFFFFF) {
        h->hero_eng->unk10 = h->hero_eng->OldXYZ;
        return 0;
    }
    if (dy < 0) {
        dist = ~dist + 1;
        far = dy < dist;
    } else {
        far = dist < dy;
    }
    if (far) {
        return 0;
    }
    h->hero_eng->unk10.y -= dy;
    return 1;
}

s32 func_800864CC(s32 id, u32 flag) {
    VECTOR pos;
    HeroInfo *base;
    HeroInfo *me;
    HeroInfo *h;
    EXTengine *eng;
    FlagRecord *box;
    FlagRecord *rec;
    s32 i;
    s32 r;
    s32 top;
    s32 bot;
    s32 y;
    s32 dx;
    s32 dz;
    s32 lo;
    s32 hi;
    s32 d;

    base = Heroes;
    h = &base[id];
    eng = h->hero_eng;
    box = func_80025A48(eng, flag);
    if (box == NULL) {
        return -1;
    }
    if (eng->unk5E != 0) {
        func_800142D4(&eng->unk1C);
    }
    func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
    i = 0;
    me = h;
    h = base;
    r = box->radius;
    top = eng->unk10.y + box->unk2;
    bot = eng->unk10.y + box->height;
    func_80015758((SVECTOR *)box, &pos);
    for (; i < 4; i++) {
        if (i == id) {
            continue;
        }
        if (h[i].hero_status & 0xF800) {
            continue;
        }
        eng = h[i].hero_eng;
        if (eng == NULL) {
            continue;
        }
        rec = func_80025A48(eng, 0x40);
        if (rec == NULL) {
            continue;
        }
        lo = eng->unk10.y + rec->unk2;
        hi = eng->unk10.y + rec->height;
        if (bot < lo && hi < top) {
            dx = eng->unk10.x - pos.vx;
            dz = eng->unk10.z - pos.vz;
            d = dx * dx + dz * dz;
            y = rec->radius + r;
            if (d < y * y) {
                if (me->hero_pu != 0) {
                    ((void (*)(s32, s32))D_8009D6E8.unk60)(id, i);
                }
                if (h[i].hero_pu != 0) {
                    ((void (*)(s32, s32))D_8009D6E8.unk60)(i, id);
                }
                return i;
            }
        }
    }
    return -1;
}

s32 func_800866DC(s32 *arg0) {
    s32 buf[4];

    *(Vec3W *)(buf + 1) = *(Vec3W *)(arg0 + 4);
    return 0;
}

void func_80086704(void) {
}

s32 func_8008670C(void) {
    return 0;
}

s32 func_80086714(void) {
    return 0;
}

s32 func_8008671C(void) {
    return 0;
}

s32 func_80086724(void) {
    return 0;
}

void func_8008672C(s32 idx) {
    HeroInfo *h;
    EXTengine *s;
    EXTengine *e;

    if (D_8009D704 != NULL) {
        D_8009D704();
    } else {
        HeroInfo *base = Heroes;
        h = base + idx;
        s = h->hero_shadow;
        e = h->hero_eng;
        s->unk10 = e->unk10;
        if (h->hero_status & 0xA000) {
            s->sub->flags &= 0xFFFF7FFF;
        }
        s->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(s);
    }
}

void func_800867EC(void) {
    func_80085D2C(1);
}

void func_8008680C(void) {
}

void func_80086814(void) {
    if (D_8009D86C != 0) {
        func_80016478(D_8009D86C, D_8009D868);
    }
    if (D_8009D878 != 0) {
        func_80016478(D_8009D878, D_8009D874);
    }
}

void func_80086868(void) {
    D_8009D6DC[0] = 0;
    D_8009D6DC[1] = 0;
    D_8009D6DC[2] = 0;
    D_8009D6DC[3] = 20;
    D_8009D6DC[4] = 0;
}

void func_8008688C(void) {
    D_800AD12C = D_8005A5E8[D_8005A61C].unkC;
}

void SetHeroAction(HeroInfo *hero, s32 action) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + (s32)hero;
    u16 old = h->hero_mode;

    h->hero_mode = action;
    h->hero_last_mode = old;
    HeroActionTable[action].fn();
}

void func_80086918(void) {
    D_8009D87C_t *t;
    u8 *g;
    s32 ret;
    s32 i;
    u16 flags;
    s32 c;
    s32 n;

    ret = -1;
    g = D_8005A614;
    t = D_8009D87C;
    for (i = 0; i < 4; i++) {
        flags = *(u16 *)(g + 0x32);
        if (flags & 0x100) {
            c = g[*(s16 *)(g + 0x34) + 0xAB];
            g[i + 0x9B] = c;
            t[i].unk0 = ((u16 *)D_8009D8FC)[c];
            t[i].unk4 = 0x10;
        } else if (flags & 1) {
            t[i].unk0 = ((u16 *)D_8009D900)[*(s16 *)(g + 0x38)];
            t[i].unk4 = 0x10;
        } else if (flags & 2) {
            t[i].unk0 = ((u16 *)D_8009D900)[*(s16 *)(g + 0x38) + 3];
            t[i].unk4 = 0x10;
        } else if (flags & 0x40) {
            t[i].unk0 = ((u16 *)D_8009D8FC)[4];
            if (g[i + 0x5C] != 0) {
                ret = t[i].unk4 = ((u16 *)D_8009D8F4)[4];
            } else {
                t[i].unk4 = ((u16 *)D_8009D8F8)[4];
            }
        } else if (flags & 0x20) {
            if (flags & 8) {
                t[i].unk0 = ((u16 *)D_8009D8FC)[9];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].unk4 = ((u16 *)D_8009D8F4)[9];
                } else {
                    t[i].unk4 = ((u16 *)D_8009D8F8)[9];
                }
            } else {
                t[i].unk0 = ((u16 *)D_8009D8FC)[5];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].unk4 = ((u16 *)D_8009D8F4)[5];
                } else {
                    t[i].unk4 = ((u16 *)D_8009D8F8)[5];
                }
            }
        } else if (flags & 0x80) {
            t[i].unk0 = ((u16 *)D_8009D8FC)[6];
            if (g[i + 0x5C] != 0) {
                ret = t[i].unk4 = ((u16 *)D_8009D8F4)[6];
            } else {
                t[i].unk4 = ((u16 *)D_8009D8F8)[6];
            }
        } else if (flags & 0x10) {
            if (*(s32 *)(g + 0x70) != 0) {
                c = g[*(s16 *)(g + 0x34) + 0xAB];
                g[i + 0x9B] = c;
                t[i].unk0 = ((u16 *)D_8009D8FC)[3];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].unk4 = ((u16 *)D_8009D8F4)[3];
                } else {
                    t[i].unk4 = ((u16 *)D_8009D904)[9];
                }
            } else {
                c = g[*(s16 *)(g + 0x34) + 0xAB];
                g[i + 0x9B] = c;
                t[i].unk0 = ((u16 *)D_8009D8FC)[c];
                if (g[i + 0x5C] != 0) {
                    ret = t[i].unk4 = ((u16 *)D_8009D8F4)[g[0x9B]];
                } else {
                    n = g[i + 0xCC];
                    if (n >= 3) {
                        n = 2;
                    }
                    t[i].unk4 = ((u16 (*)[3])D_8009D904)[c][n];
                }
            }
        }
    }
    func_80086C94(ret);
    n = 0;
    do {
        flags = n < 4;
        n++;
    } while (flags);
}

s32 func_80086C94(s32 arg0) {
    s32 v;
    u8 *g;
    s32 idx;

    if (arg0 < 0) {
        return -1;
    }
    g = D_8005A614;
    idx = *(s16 *)(g + 0x42);
    if (idx == 0 ||*(s32 *)(g + 0x6C) != 0 || !(*(u16 *)(g + 0x32) & 0x10)) {
        return -1;
    }
    {
        u8 t = g[*(s16 *)(g + 0x34) + 0xAB];

        if (t == 0) {
            arg0 = arg0 / 2;
        }
        if (t == 1) {
            v = arg0 * 3;
            arg0 = v / 4;
        }
        if (D_8005A614[*(s16 *)(D_8005A614 + 0x34) + 0xAB] == 3) {
            arg0 *= 2;
        }
        idx = *(s16 *)(D_8005A614 + 0x42);
        if (idx != -1) {
            D_8009D87C[idx].unk4 = arg0;
        }
        if (arg0 <= 0) {
            arg0 = 1;
        }
        return arg0;
    }
    return -1;
}

void func_80086D88(void) {
    s32 i;
    HeroInfo *hero;
    EXTengine *eng;
    s32 flag;
    s32 px;
    s32 x;
    s32 z;
    s32 xlo;
    s32 xhi;
    s32 zlo;
    s32 zhi;

    for (i = 0; i < 4; i++) {
        hero = &Heroes[i];
        if ((u32)(hero->hero_name - 1) < 8) {
            eng = hero->hero_eng;
            flag = (i << 5) | 0x80;
            px = eng->unk10.x >> 8;
            x = px + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x < 0) {
                continue;
            }
            if (x >= 20) {
                continue;
            }
            if (z < 0) {
                continue;
            }
            if (z < 20) {
                xhi = x;
                xlo = xhi;
                zhi = z;
                zlo = zhi;
                if (xhi > 0) {
                    xlo = px + 9;
                }
                if (xhi < 19) {
                    xhi = px + 11;
                }
                if (zhi > 0) {
                    zlo = zhi - 1;
                }
                if (zhi < 19) {
                    zhi++;
                }
                D_8009D90C[z * 20 + x].unk0 |= flag;
                D_8009D90C[zlo * 20 + x].unk0 |= flag;
                D_8009D90C[zhi * 20 + x].unk0 |= flag;
                D_8009D90C[z * 20 + xlo].unk0 |= flag;
                D_8009D90C[zlo * 20 + xlo].unk0 |= flag;
                D_8009D90C[zhi * 20 + xlo].unk0 |= flag;
                D_8009D90C[z * 20 + xhi].unk0 |= flag;
                D_8009D90C[zlo * 20 + xhi].unk0 |= flag;
                D_8009D90C[zhi * 20 + xhi].unk0 |= flag;
            }
        }
    }
}

void func_80086F4C(void) {
    s32 i;
    s32 j;

    i = 0;
loop:
    j = 0;
    if (i < 20) {
inner:
        if (j < 20) {
            j++;
            goto inner;
        }
        i++;
        goto loop;
    }
}

void func_80086F78(void) {
    s32 i = 399;
    D_8009D90C_t *p = D_8009D90C;

    for (p += 399; i >= 0; i--) {
        p->unk0 = 0;
        p--;
    }
}

void func_80086FA0(void) {
    s32 i;
    D_8009D90C_t *p;

    for (i = 0, p = D_8009D90C; i < 400; i++) {
        p->unk1 = 0;
        p->unk0 = 0;
        p++;
    }
}

s32 func_80086FCC(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    PadButtons *pads;
    PadButtons *pad;
    PadButtons *pads2;
    PadButtons *pad2;
    s32 action;

    if (D_8009D724(arg0) != 0) {
        return 1;
    }
    base = Heroes;
    h = base + arg0;
    if (!(h->hero_status & 0x10)) {
        pads = (PadButtons *)D_80051338;
        pad = pads + arg0;
        if (pad->unk54 & 0x8000) {
            if (h->hero_attack_timer != 0) {
                pad->unk4C &= ~0x8000;
            } else {
                switch (h->hero_name) {
                case 1:
                case 2:
                    SetHeroAction((HeroInfo *)arg0, 8);
                    return 1;
                case 7:
                case 8:
                    SetHeroAction((HeroInfo *)arg0, 9);
                    return 1;
                case 3:
                case 4:
                    SetHeroAction((HeroInfo *)arg0, 10);
                    return 1;
                default:
                    SetHeroAction((HeroInfo *)arg0, 11);
                    return 1;
                }
            }
        }
    }
    if (D_8005A648 != 0x1E) {
        pads2 = (PadButtons *)D_80051338;
        pad2 = pads2 + arg0;
        if (pad2->unk54 & 0x2000) {
            if (D_8009D738(arg0, 0x280) != 0 || Heroes[arg0].hero_pick_delay == 0) {
                SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DA);
                return 1;
            }
            pad2->unk4C &= ~0x2000;
        } else {
            Heroes[arg0].hero_pick_delay = 5;
        }
    }
    return 0;
}

s32 func_800871E4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    HeroInfo *base13;
    HeroInfo *h13;
    HeroInfo *base2;
    HeroInfo *h2;
    EXTengine *e;
    EXTengine *e2;
    EXTengine *p;
    s32 moved;

    if (h->hero_move->jump_max != 0) {
        if (func_80025BBC(h->hero_eng) != 0) {
            h->hero_move->jump_max = 0;
            h->hero_sub->unk08 = 0;
        } else if (!(h->hero_ctrl & 8)) {
            e = h->hero_eng;
            if (World_floor_level < e->unk10.y + h->hero_sub->unk08) {
                if (D_8005B5B8 == 0x7FFFFFFF) {
                    goto set6;
                }
                e->unk10.y = World_floor_level;
                h->hero_sub->unk08 = 0;
            }
        }
    } else if ((moved = func_800863D0(arg0) != 1)) {
        switch (h->hero_mode) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            SetHeroAction((HeroInfo *)arg0, 5);
            return 1;
        case 14:
        case 15:
        case 16:
        case 17:
        case 22:
        case 23:
        case 24:
        case 25:
            SetHeroAction((HeroInfo *)arg0, 5);
            return 1;
        case 13:
            base13 = Heroes;
            h13 = base13 + arg0;
            if (!(h13->hero_ctrl & 8) && World_floor_level < h13->hero_eng->unk10.y + h13->hero_sub->unk08) {
                if (D_8005B5B8 == 0x7FFFFFFF) {
                set6:
                    SetHeroAction((HeroInfo *)arg0, 6);
                    return 1;
                }
            }
        default:
            base2 = Heroes;
            h2 = base2 + arg0;
            e2 = h2->hero_eng;
            p = e2->platform;
            if (p != NULL) {
                p->platform = NULL;
            }
            e2->platform = NULL;
            h2->hero_move->jump_vel = 8;
            h2->hero_move->jump_max = 0x70;
            h2->hero_move->jump_acc = 10;
            h2->hero_move->jump_dec = 0;
            h2->hero_move->jump_dir = 0;
            return 0;
        }
    }
    return 0;
}

void func_80087428(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;

    h = base + arg0;

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 1;
    D_8009D718();
    SetHeroSeq((HeroInfo *)arg0, 1);
    h->hero->Camera.look_ahead = 0x400;
    if (h->hero_ctrl & 4) {
        e = h->hero_eng;
        e->unk2C.z = 0x1000;
        e->unk2C.y = 0x1000;
        e->unk2C.x = 0x1000;
        h->hero_ctrl &= 0xFFFB;
    }
}

void func_800874EC(s32 arg0) {
    s32 ret2;
    HeroInfo *h;
    HeroInfo *h2;
    HeroInfo *base;
    HeroInfo *base2;
    EXTheromove *m;
    EXTengine *e;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        func_80086FCC(arg0) == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckMoving(arg0) == 0 &&
            HeroCheckPad1000(arg0) == 0) {
            base = Heroes;
            h = base + arg0;
            m = h->hero_move;
            if (m->velocity_calc == 3) {
                e = h->hero_eng;
                if (e->unk10.y < World_floor_level) {
                    if (h->hero_aseq == 0xE) {
                        SetHeroSeq((HeroInfo *)arg0, 1);
                    }
                } else if (m->x_vel != 0 || m->z_vel != 0) {
                    if (h->hero_aseq != 1) {
                        goto idle_done;
                    }
                    PlaySfx(D_8009DC2C[Random(6)], &h->hero_eng->sfx, NULL,
                            0x1000, 0x1E00, 0);
                    SetHeroSeq((HeroInfo *)arg0, 0xE);
                } else if (h->hero_aseq == 0xE) {
                    if (e->sfx != NULL) {
                        StopSfx(e->sfx);
                    }
                    SetHeroSeq((HeroInfo *)arg0, 1);
                }
            }
            base2 = Heroes;
            h2 = base2 + arg0;
            if (h2->hero_aseq == 1) {
                if (h2->hero_idle_count != 0) {
                    h2->hero_idle_count--;
                } else {
                    D_8009D71C(arg0);
                }
            }
        idle_done:
            func_8008502C(arg0);
            if (D_8005A5DC < 4) {
                if (D_8005A5DC >= 2) {
                    D_8009D73C(arg0);
                } else if (D_8009D740 != NULL) {
                    D_8009D740(arg0);
                }
            } else if (D_8009D740 != NULL) {
                D_8009D740(arg0);
            }
            func_800871E4(arg0);
        }
    }
}

void func_8008795C(s32 arg0) {
    s32 ret2;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        func_80086FCC(arg0) == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckMoving(arg0) == 0) {
            func_8008502C(arg0);
            if (D_8005A5DC < 4) {
                if (D_8005A5DC >= 2) {
                    D_8009D73C(arg0);
                }
            }
            func_800871E4(arg0);
        }
    }
}

void func_80087B8C(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    s32 ret2;
    s32 id;

    if (HeroCheckStatus2000(arg0) == 0 && HeroCheckStatus600(arg0) == 0 &&
        func_80086FCC(arg0) == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0 && HeroCheckPad1000(arg0) == 0 && HeroCheckStopped(arg0) == 0) {
            base = Heroes;
            h = base + arg0;
            h->hero_move->velocity_enable = 1;
            D_8009D73C(arg0);
            switch (func_8008502C(arg0)) {
            case 8:
                if (h->hero_pu != 0x15) {
                    switch (h->hero_name) {
                    case 1: id = 0xA; break;
                    case 2: id = 0x3C; break;
                    case 3: id = 0x6E; break;
                    case 4: id = 0xA0; break;
                    case 7: id = 0x104; break;
                    case 8: id = 0xD2; break;
                    case 5: id = 0x136; break;
                    case 6: id = 0x168; break;
                    default: goto end;
                    }
                    PlaySfx(id, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                } else {
                    PlaySfx(0x41E, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                break;
            case 9:
                if (h->hero_pu == 0x15) {
                    break;
                }
                switch (h->hero_name) {
                case 1: id = 0xB; break;
                case 2: id = 0x3D; break;
                case 3: id = 0x6F; break;
                case 4: id = 0xA1; break;
                case 7: id = 0x105; break;
                case 8: id = 0xD3; break;
                case 5: id = 0x137; break;
                case 6: id = 0x169; break;
                default: goto end;
                }
                PlaySfx(id, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                break;
            }
        end:
            func_800871E4(arg0);
        }
    }
}

void func_80087FC0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    s32 idx;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    SetHeroSeq((HeroInfo *)arg0, 6);
    idx = 0;
    if (D_8005A614[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = ((HeroJumpEnt *)D_8009D6E8.unk10)[idx].vel;
    h->hero_move->jump_max = ((HeroJumpEnt *)D_8009D6E8.unk10)[idx].max;
    h->hero_move->jump_acc = ((HeroJumpEnt *)D_8009D6E8.unk10)[idx].acc;
    h->hero_move->jump_dec = ((HeroJumpEnt *)D_8009D6E8.unk10)[idx].dec;
    h->hero_move->jump_dir = -1;
    h->hero->Camera.look_ahead = 0x400;
}

void func_800880E0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *p;
    EXTobject *o;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 10;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    o = h->hero_object;
    if (o != NULL) {
        o->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 7);
}

void func_800881E0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    u8 *eng;

    if (func_80025BBC(h->hero_eng) != 0) {
        SetHeroAction((HeroInfo *)arg0, 7);
    } else if (!(h->hero_ctrl & 8)) {
        eng = (u8 *)h->hero_eng;
        if (World_floor_level < *(s32 *)(eng + 0x14) + *(s32 *)((u8 *)h->hero_sub + 8)) {
            if (D_8005B5B8 == 0x7FFFFFFF) {
                SetHeroAction((HeroInfo *)arg0, 6);
            } else {
                *(s32 *)(eng + 0x14) = World_floor_level;
                *(s32 *)((u8 *)h->hero_sub + 8) = 0;
            }
        }
    }
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_800882DC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *p;
    EXTengine *e;

    h = base + arg0;
    h->hero_eng->unk10.x &= ~0x1FF;
    h->hero_eng->unk10.x += 0x100;
    h->hero_eng->unk10.z &= ~0x1FF;
    h->hero_eng->unk10.z += 0x100;
    h->hero_move->velocity_enable = 0;
    h->hero_move->forward_speed = 0;
    h->hero_move->jump_ctrl |= 0x10;
    e = h->hero_eng;
    p = e->platform;
    if (p != NULL) {
        p->platform = NULL;
    }
    e->platform = NULL;
    h->hero_move->jump_vel = 8;
    h->hero_move->jump_max = 0x70;
    h->hero_move->jump_acc = 0xA;
    h->hero_move->jump_dec = 0;
    h->hero_move->jump_dir = 0;
    h->hero_status |= 0x800;
    if (h->hero_last_mode != 0xD) {
        SetHeroSeq((HeroInfo *)arg0, 7);
    }
    PlaySfx(0x410, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_80088450(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *e;
    EXTengine *o;
    EXTengine *t;
    u8 *x;

    h = base + arg0;
    h->hero_move->velocity_enable = 1;
    h->hero_move->jump_max = 0;
    h->hero_sub->unk08 = 0;
    SetHeroSeq((HeroInfo *)arg0, 8);
    if (D_8005AA9C != NULL) {
        e = h->hero_eng;
        o = D_8005AA9C->Engine;
        t = e->platform;
        if (t != NULL) {
            t->platform = NULL;
            e->platform = NULL;
        }
        t = o->platform;
        if (t != NULL) {
            t->platform = NULL;
            o->platform = NULL;
        }
        e->platform = o;
        o->platform = e;
        x = FindExtension(D_8005AA9C, 0x1000);
        if (x != NULL && (*(u16 *)(x + 0x1C) & 8)) {
            *(u16 *)(x + 0x1E) |= 8;
        }
    }
}

void func_80088544(s32 arg0) {
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0 && func_80086FCC(arg0) == 0) {
        s32 ret2;
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (D_8009D728(arg0) != 0) {
                ret = 1;
            } else {
                HeroInfo *base = Heroes;
                HeroInfo *hero = &base[arg0];
                if (hero->hero_move->forward_speed != 0) {
                    if (hero->hero_mode == 0xF || hero->hero_mode == 0x17) {
                        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DE);
                    } else {
                        SetHeroAction((HeroInfo *)arg0, 3);
                    }
                    ret = 1;
                } else {
                    ret = 0;
                }
            }
            if (ret == 0) {
                D_8009D73C(arg0);
                func_8008502C(arg0);
                func_800871E4(arg0);
            }
        }
    }
}

void func_80088708(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];
    NAS *nas;

    hero->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    nas = hero->hero;
    nas->Camera.look_ahead = nas->Camera.width;
}

void func_80088794(s32 arg0) {
    s32 ret2;
    s32 id;
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *o;
    EXTengine *me;
    EXTengine *oe;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 idx;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroCheckPad1000(arg0) == 0) {
                base = Heroes;
                h = base + arg0;
                h->hero_eng->unk1E -= 0x155;
                idx = (s16)((h->hero_eng->unk1E & 0xFFF) / 341);
                idx = (s16)(idx % 12);
                h->hero_eng->unk2C.x = D_8009DC38[idx];
                h->hero_eng->unk2C.y = D_8009DC50[idx];
                h->hero_eng->unk2C.z = D_8009DC38[idx];
                h->hero_ctrl |= 5;
                id = func_800864CC(arg0, 0x400);
                if (id != -1) {
                    o = base + id;
                    if ((o->hero_immunity | o->hero_hit) == 0) {
                        me = h->hero_eng;
                        oe = o->hero_eng;
                        angle = get_angle(me->unk10.x - oe->unk10.x, me->unk10.z - oe->unk10.z);
                        m = o->hero_move;
                        vc = m->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            m->x_vel -= (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                            o->hero_move->z_vel -= (D_80068BD4[angle & 0xFFF].unk2 * 3) >> 8;
                        }
                        Heroes[id].hero_move->reactive_angle = angle;
                        Heroes[id].hero_move->reactive_speed = 0x30;
                        func_80084E04(id, NULL, arg0, 0x8001, 0x446);
                    }
                }
                ((void (*)(s32, s32, s32, s32))D_8009D6E8.unk4C)(arg0, 2, 0x400, 0);
                ((void (*)(s32))D_8009D6E8.unk54)(arg0);
                func_8008502C(arg0);
                func_800871E4(arg0);
            }
        }
    }
}

void func_80088BAC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

void func_80088C28(s32 arg0) {
    s32 ret2;
    s32 id;
    HeroInfo *base;
    HeroInfo *o;
    EXTengine *me;
    EXTengine *oe;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (HeroCheckPad1000(arg0) == 0) {
                id = func_800864CC(arg0, 0x400);
                if (id != -1) {
                    base = Heroes;
                    o = base + id;
                    if ((o->hero_immunity | o->hero_hit) == 0) {
                        me = base[arg0].hero_eng;
                        oe = o->hero_eng;
                        angle = get_angle(me->unk10.x - oe->unk10.x, me->unk10.z - oe->unk10.z);
                        m = o->hero_move;
                        vc = m->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            m->x_vel -= (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                            o->hero_move->z_vel -= (D_80068BD4[angle & 0xFFF].unk2 * 3) >> 8;
                        }
                        Heroes[id].hero_move->reactive_angle = angle;
                        Heroes[id].hero_move->reactive_speed = 0x30;
                        func_80084E04(id, NULL, arg0, 0x8001, 0x446);
                    }
                }
                ((void (*)(s32, s32, s32, s32))D_8009D6E8.unk4C)(arg0, 2, 0x400, 0);
                ((void (*)(s32))D_8009D6E8.unk54)(arg0);
                func_8008502C(arg0);
                func_800871E4(arg0);
            }
        }
    }
}

void func_80088F78(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
    if (hero->hero_name == 3) {
        hero->hero_weapon_timer = -2;
    } else {
        hero->hero_weapon_timer = 0;
    }
}

void func_8008900C(s32 arg0) {
    s32 ret2;
    s32 ret3;
    s32 other;
    HeroInfo *base;
    HeroInfo *h2;
    HeroInfo *h;
    HeroInfo *self;
    HeroInfo *base2;
    s16 (*tbl)[3];
    EXTheromove *m;
    subengine *s;
    s16 *p;
    u16 t;
    s32 i;
    VECTOR pos;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (D_8009D730 != NULL) {
                ret3 = D_8009D730(arg0);
            } else if (Heroes[arg0].hero_status & 0xFA00) {
                ret3 = 0;
            } else if (((PadButtons *)D_80051338)[arg0].unk50 & 0x1000) {
                SetHeroAction((HeroInfo *)arg0, 2);
                ret3 = 1;
            } else {
                ret3 = 0;
            }
            if (ret3 == 0) {
                other = func_800864CC(arg0, 0x400);
                if (other != -1) {
                    base = Heroes;
                    h2 = &base[other];
                    if ((h2->hero_immunity | h2->hero_hit) == 0) {
                        h = &base[arg0];
                        if (func_80015480(h->hero_move->forward_angle,
                                          get_angle(h->hero_eng->unk10.x - h2->hero_eng->unk10.x,
                                                    h->hero_eng->unk10.z - h2->hero_eng->unk10.z)) < 0x200) {
                            angle = h->hero_move->forward_angle;
                            h2->hero_move->forward_angle = angle ^ 0x800;
                            m = h2->hero_move;
                            vc = m->velocity_calc;
                            if (vc != 1) {
                                if (vc != 0) {
                                    if (vc < 4) {
                                        m->x_vel -= (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                                        h2->hero_move->z_vel -= (D_80068BD4[angle & 0xFFF].unk2 * 3) >> 8;
                                    }
                                }
                            }
                            Heroes[other].hero_move->reactive_angle = angle;
                            Heroes[other].hero_move->reactive_speed = 0x30;
                            func_80084E04(other, NULL, arg0, 0x8001, 0x446);
                        }
                    }
                }
                ((void (*)(s32, s32, s32, s32))D_8009D6E8.unk4C)(arg0, 2, 0x400, 0x280);
                ((void (*)(s32))D_8009D6E8.unk54)(arg0);
                func_8008502C(arg0);
                base2 = Heroes;
                self = &base2[arg0];
                s = self->hero_fx->sub;
                if (s != NULL && func_80015780(self->hero_eng, self->hero_sub, 0x40, &pos) != NULL) {
                    self->hero_fx->unk10 = self->hero_eng->unk10;
                    t = self->hero_weapon_timer;
                    i = (s16)t;
                    if (i < 0) {
                        self->hero_weapon_timer = t + 1;
                    } else {
                        tbl = D_8009DC68;
                        p = tbl[i];
                        if (*p != 0) {
                            s->unk20.x = p[0];
                            s->unk20.y = p[1];
                            s->unk20.x = p[2];
                            s->unk04 = pos.vx - self->hero_eng->unk10.x;
                            s->unk08 = pos.vy - self->hero_eng->unk10.y;
                            s->unk0C = pos.vz - self->hero_eng->unk10.z;
                            s->flags |= 0x20008000;
                            s->unk52 = 1;
                            self->hero_weapon_timer++;
                        } else {
                            s->flags &= ~0x8000;
                        }
                    }
                }
                func_800871E4(arg0);
            }
        }
    }
}

void func_800894E0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    hero->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xA);
    hero->hero_attack_delay += 0xF;
    if (hero->hero_attack_delay > 0x28) {
        hero->hero_attack_delay = 0x28;
    }
    hero->hero_attack_timer = hero->hero_attack_delay;
}

void func_8008955C(s32 arg0) {
    s32 ret2;
    s32 ret3;
    s32 other;
    HeroInfo *base;
    HeroInfo *h2;
    HeroInfo *h;
    EXTheromove *m;
    s32 angle;
    s32 vc;
    s32 ret;
    u16 status;

    status = Heroes[arg0].hero_status;
    if (status & 0x200) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E4);
        ret = 1;
    } else if (status & 0x400) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E6);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        if (D_8009D72C != NULL) {
            ret2 = D_8009D72C(arg0);
        } else if (((PadButtons *)D_80051338)[arg0].unk54 & 0x4000) {
            SetHeroAction((HeroInfo *)arg0, 4);
            ret2 = 1;
        } else {
            ret2 = 0;
        }
        if (ret2 == 0) {
            if (D_8009D730 != NULL) {
                ret3 = D_8009D730(arg0);
            } else if (Heroes[arg0].hero_status & 0xFA00) {
                ret3 = 0;
            } else if (((PadButtons *)D_80051338)[arg0].unk50 & 0x1000) {
                SetHeroAction((HeroInfo *)arg0, 2);
                ret3 = 1;
            } else {
                ret3 = 0;
            }
            if (ret3 == 0) {
                other = func_800864CC(arg0, 0x400);
                if (other != -1) {
                    base = Heroes;
                    h2 = &base[other];
                    if ((h2->hero_immunity | h2->hero_hit) == 0) {
                        h = &base[arg0];
                        if (func_80015480(h->hero_move->forward_angle,
                                          get_angle(h->hero_eng->unk10.x - h2->hero_eng->unk10.x,
                                                    h->hero_eng->unk10.z - h2->hero_eng->unk10.z)) < 0x200) {
                            angle = h->hero_move->forward_angle;
                            h2->hero_move->forward_angle = angle ^ 0x800;
                            m = h2->hero_move;
                            vc = m->velocity_calc;
                            if (vc != 1) {
                                if (vc != 0) {
                                    if (vc < 4) {
                                        m->x_vel -= (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 3) >> 8;
                                        h2->hero_move->z_vel -= (D_80068BD4[angle & 0xFFF].unk2 * 3) >> 8;
                                    }
                                }
                            }
                            Heroes[other].hero_move->reactive_angle = angle;
                            Heroes[other].hero_move->reactive_speed = 0x30;
                            func_80084E04(other, NULL, arg0, 0x8001, 0x446);
                        }
                    }
                }
                ((void (*)(s32, s32, s32, s32))D_8009D6E8.unk4C)(arg0, 2, 0x400, 0x280);
                ((void (*)(s32))D_8009D6E8.unk54)(arg0);
                func_8008502C(arg0);
                func_800871E4(arg0);
            }
        }
    }
}

void func_800898EC(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    EXTengine *e;
    subengine *s;
    s32 i;

    h->hero_sub->unk6C = D_80053004;
    e = h->hero_eng;
    s = h->hero_sub;
    e->flags |= 0x8021;
    s->flags |= 0x8021;
    e->unk20 = 0;
    e->unk1E = 0;
    e->unk1C = 0;
    s->unk14 = 0;
    s->unk12 = 0;
    s->unk10 = 0;
    h->hero_anim->sub = NULL;
    e->unk2C.x = 0x1000;
    e->unk2C.y = 0x1000;
    e->unk2C.z = 0x1000;
    s->unk20.x = 0x1000;
    s->unk20.y = 0x1000;
    s->unk20.z = 0x1000;
    h->hero_ctrl |= 1;
    e->unk5E = 1;
    s->unk52 = 1;
    e = h->hero_fx;
    s = e->sub;
    e->unk20 = 0;
    e->unk1E = 0;
    e->unk1C = 0;
    s->unk14 = 0;
    s->unk12 = 0;
    s->unk10 = 0;
    e->flags |= 0x8021;
    s->flags |= 0x8021;
    h->hero_move->jump_max = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->x_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->jump_ctrl = 0;
    s->unk6C = D_80053004;
    s->unk74 = 0x2002;
    e->unk2C.x = 0x1000;
    e->unk2C.y = 0x1000;
    e->unk2C.z = 0x1000;
    s->unk20.x = 0x1000;
    s->unk20.y = 0x1000;
    s->unk20.z = 0x1000;
    e->unk5E = 1;
    s->unk52 = 1;
    e->unk10 = h->hero_eng->unk10;
    *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&h->hero_sub->unk04;
    for (s = s->next; s != NULL; s = s->next) {
        s->unk04 = 0;
        s->unk08 = 0;
        s->unk0C = 0;
        s->unk6C = D_80053004;
        s->unk74 = 0x2003;
        s->flags = (s->flags | 0x60000021) & ~0x8000;
    }
    for (i = 0; i < 3; i++) {
        D_8009E0E8[i][arg0].unk0 = Random(10) + i * 3;
        D_8009E0E8[i][arg0].unk2 = 0;
        D_8009E0E8[i][arg0].unk4 = 0;
        D_8009E0E8[i][arg0].unk6 = 0;
        D_8009E0E8[i][arg0].unk8 = 0;
        D_8009E0E8[i][arg0].unkA = 0;
        D_8009E0E8[i][arg0].unkC = 0;
    }
    if (Random(2) != 0) {
        PlaySfx(0x1C4, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    } else {
        PlaySfx(0x1C3, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
    func_80085584(arg0, 1, 0xB);
}

void func_80089BE8(arg0)
s32 arg0;
{
    s32 y;
    EXTengine *e;
    subengine *sub;
    subengine *s;
    s32 i;

    sub = Heroes[arg0].hero_sub;
    Heroes[arg0].hero_frozen = 2;
    y = sub->unk08;
    y -= 0x18;
    sub->unk08 = y;
    Heroes[arg0].hero_eng->unk5D = 4;
    Heroes[arg0].hero_sub->unk51 = 4;
    Heroes[arg0].hero_fx->unk5D = 4;
    Heroes[arg0].hero_fx->sub->unk51 = 4;
    Heroes[arg0].hero_eng->unk5C = 0;
    Heroes[arg0].hero_sub->unk50 = 0;
    Heroes[arg0].hero_fx->unk5C = 0;
    Heroes[arg0].hero_fx->sub->unk50 = 0;
    e = Heroes[arg0].hero_fx;
    s = e->sub;
    if (y < -0xA00) {
        Heroes[arg0].hero_status |= 0x8000;
        for (s = s->next; s != NULL; s = s->next) {
            s->flags &= ~0x8000;
        }
        return;
    }
    e->unk10 = Heroes[arg0].hero_eng->unk10;
    *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&Heroes[arg0].hero_sub->unk04;
    s->flags |= 0x8000;
    func_8008502C(arg0, y);
    for (s = s->next, i = 0; s != NULL; s = s->next, i++) {
        if (D_8009E0E8[i][arg0].unk2 != 0) {
            D_8009E0E8[i][arg0].unk6 -= 3;
            if (D_8009E0E8[i][arg0].unk6 < 0) {
                D_8009E0E8[i][arg0].unk6 = 0;
            }
            s->unk04 += D_8009E0E8[i][arg0].unk6 * D_80068BD4[(D_8009E0E8[i][arg0].unk4 - 0x400) & 0xFFF].unk2 >> 12;
            s->unk0C += D_8009E0E8[i][arg0].unk6 * D_80068BD4[D_8009E0E8[i][arg0].unk4 & 0xFFF].unk2 >> 12;
            y = D_8009E0E8[i][arg0].unk8;
            y += D_8009E0E8[i][arg0].unkA;
            if (y > 0x30) {
                y = 0x30;
            }
            D_8009E0E8[i][arg0].unk8 = y;
            s->unk08 += y;
            s->flags |= 0x8000;
            s->unk76 = D_8009E0E8[i][arg0].unkC += 0x60;
            if (World_floor_level < s->unk08 || D_8009E0E8[i][arg0].unkC >= 0x1000) {
                D_8009E0E8[i][arg0].unk2 = 0;
                D_8009E0E8[i][arg0].unk0 = Random(10) + 2;
                s->flags &= ~0x8000;
            }
        } else if (D_8009E0E8[i][arg0].unk0 != 0) {
            D_8009E0E8[i][arg0].unk0--;
        } else {
            D_8009E0E8[i][arg0].unk2 = 1;
            D_8009E0E8[i][arg0].unk4 = Random(0x800) + 0x400;
            D_8009E0E8[i][arg0].unk6 = 0x30;
            D_8009E0E8[i][arg0].unk8 = -0x40;
            D_8009E0E8[i][arg0].unkA = 4;
            D_8009E0E8[i][arg0].unkC = 0x100;
            *(SLONG_XYZ *)&s->unk04 = *(SLONG_XYZ *)&Heroes[arg0].hero_fx->sub->unk04;
        }
    }
}

void func_80089FA4(s32 arg0) {
    u16 mate;
    EngineGameView *g;

    mate = Heroes[arg0].hero_teammate;
    func_8002C1C0(100, 5);
    func_8002C29C();
    g = (EngineGameView *)D_8005A614;
    if (g->mode == 0x20) {
        if (D_8004E0A4[D_8005A614[0x60]] == 1) {
            D_8009E0D0 = 0xF;
            D_8009E0D2 = 0xF;
            D_8009E0D6 = 0x1E;
            D_8009E0D4 = arg0;
        } else {
            D_8009E0D0 = 0xF;
            D_8009E0D2 = 0x10;
            D_8009E0D6 = 0x1E;
            D_8009E0D4 = arg0;
        }
    } else if (g->flags & 0x1E0) {
        if ((s16)mate != -1) {
            if (Heroes[(s16)mate].hero_status & 0x200) {
                if (!(arg0 & 1)) {
                    D_8009E0D0 = 0x23;
                    D_8009E0D2 = 0x14;
                    D_8009E0D6 = 0x1E;
                    D_8009E0D4 = arg0;
                }
            } else {
                D_8009E0D0 = 0x23;
                D_8009E0D6 = 0x1E;
                D_8009E0D2 = 0x14;
                D_8009E0D4 = arg0;
            }
        } else {
            D_8009E0D0 = 0x23;
            D_8009E0D2 = 0x14;
            D_8009E0D6 = 0x1E;
            D_8009E0D4 = arg0;
        }
    } else if ((s16)mate != -1) {
        if (Heroes[(s16)mate].hero_status & 0x200) {
            if (!(arg0 & 1)) {
                D_8009E0D0 = 0x23;
                D_8009E0D2 = 9;
                D_8009E0D6 = 0x1E;
                D_8009E0D4 = arg0;
            }
        } else {
            D_8009E0D0 = 0x23;
            D_8009E0D2 = 9;
            D_8009E0D6 = 0x1E;
            D_8009E0D4 = arg0;
        }
    } else {
        D_8009E0D0 = 0x1E;
        D_8009E0D2 = 0;
        D_8009E0D6 = 0x1E;
        D_8009E0D4 = arg0;
    }
}

s32 func_8008A200(s32 arg0) {
    HeroInfo *h;
    HeroInfo *p;
    s32 ret;
    s32 i;
    u16 t;

    ret = 0;
    if (arg0 != -1) {
        p = Heroes;
        h = &p[arg0];
        if (!(h->hero_ctrl & 2)) {
            h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, ret) >> 2;
        }
        h->hero_ctrl |= 1;
        h->hero_eng->unk5E = 1;
    } else if ((u16)D_8009E0D2 < 9) {
        return 1;
    }
    if (D_8009E0D4 != arg0) {
        return ret;
    }
    t = D_8009E0D0;
    if (t != 0) {
        D_8009E0D0 = t - 1;
    } else {
    switch ((u16)D_8009E0D2) {
    case 0:
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(1,NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x18;
            break;
        case 2:
            PlaySfx(0x33, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x17;
            break;
        case 3:
            PlaySfx(0x65, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x1A;
            break;
        case 4:
            PlaySfx(0x97, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x17;
            break;
        case 7:
            PlaySfx(0xFB, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x1A;
            break;
        case 8:
            PlaySfx(0xC9, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x1A;
            break;
        case 5:
            PlaySfx(0x12D, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x17;
            break;
        case 6:
            PlaySfx(0x15F, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x17;
            break;
        default:
            D_8009E0D0 = 0x5A;
            D_8009E0D2 = 3;
            break;
        }
        break;
    case 1:
        PlaySfx(0x1B0, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x1E;
        break;
    case 2:
        ret = 1;
        switch (Heroes[arg0].hero_name) {
        case 1:
            PlaySfx(7, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 2:
            PlaySfx(0x39, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 3:
            PlaySfx(0x6B, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 4:
            PlaySfx(0x9D, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 7:
            PlaySfx(0x101, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 8:
            PlaySfx(0xCF, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 5:
            PlaySfx(0x133, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        case 6:
            PlaySfx(0x165, NULL, NULL, 0x1000, 0x1E00, 0);
            D_8009E0D0 = 0x12;
            break;
        default:
            D_8009E0D0 = 0x12;
            D_8009E0D2 = 3;
            break;
        }
        break;
    case 9:
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == D_8005A614[0x60]) {
                if (i & 2) {
                    PlaySfx(0x1AE, NULL, NULL, 0x1000, 0x1E00, 0);
                    D_8009E0D0 = 0xE;
                } else {
                    PlaySfx(0x1AE, NULL, NULL, 0x1000, 0x1E00, 0);
                    D_8009E0D0 = 0xB;
                }
                break;
            }
        }
        break;
    case 10:
        D_8009E0D0 = 0;
        D_8009E0D2 = 0;
        for (i = 0; i < 4; i++) {
            if (Heroes[i].hero_name == D_8005A614[0x60]) {
                if (i & 2) {
                    PlaySfx(0x1A5, NULL, NULL, 0x1000, 0x1E00, 0);
                    D_8009E0D0 = 0x16;
                    D_8009E0D2 = 0;
                } else {
                    PlaySfx(0x1A4, NULL, NULL, 0x1000, 0x1E00, 0);
                    D_8009E0D0 = 0x1B;
                    D_8009E0D2 = 0;
                }
                break;
            }
        }
        break;
    case 15:
        PlaySfx(0x1B1, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x10;
        D_8009E0D2 = 0;
        break;
    case 16:
        PlaySfx(0x1B2, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x14;
        D_8009E0D2 = 0;
        break;
    case 21:
        PlaySfx(0x1B9, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x1B;
        break;
    case 22:
        ret = 1;
        D_8009E0D2 = 1;
        D_8009E0D0 = 0x1B;
        break;
    case 20:
    case 30:
        PlaySfx(0x1B8, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0xC;
        break;
    case 31:
        PlaySfx(0x1BA, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x27;
        break;
    case 32:
        ret = 1;
        D_8009E0D2 = 3;
        D_8009E0D0 = 2;
        break;
    case 40:
        PlaySfx(0x1BB, NULL, NULL, 0x1000, 0x1E00, 0);
        D_8009E0D0 = 0x1E;
        break;
    case 41:
        ret = 1;
        D_8009E0D2 = 3;
        D_8009E0D0 = 0x1B;
        break;
    default:
        D_8009E0D0 = 0xB4;
        D_8009E0D2 = 3;
        break;
    }
    D_8009E0D2++;
    }
    return ret;
}

void func_8008A810(void) {
    u8 *p = D_8005A614;

    if (*(s32 *)(p + 4) & 0x800) {
        *(u16 *)(p + 0x30) |= 0x201;
        return;
    }
    *(u16 *)(p + 0x30) |= 1;
}

void func_8008A854(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    s32 sum;

    h->hero_frozen = 2;
    sum = *(s32 *)((u8 *)h->hero_eng + 0x14) + *(s32 *)((u8 *)h->hero_sub + 8);
    if (World_floor_level + 0x180 < sum) {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
    }
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_8008A8F0(s32 arg0) {
    D_8009D73C(arg0);
    func_8008502C(arg0);
}

void func_8008A924(s32 arg0) {
    u8 rw;

    Heroes[arg0].hero_move->velocity_enable = 1;
    do {
        rw = Heroes[arg0].hero_move->run_walk;
    } while (0);
    Heroes[arg0].hero_move->run_walk = rw;
    SetHeroSeq((HeroInfo *)arg0, 5);
    D_8009D73C(arg0);
}

void func_8008A99C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 2);
}

void func_8008A9F0(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x258) + 0x12C;
}

void func_8008AA40(s32 arg0) {
    Heroes[arg0].hero_idle_count = Random(0x96) + 0x5A;
}

void HeroQuiet(void) {
    func_8008502C();
}

void func_8008AAB0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->jump_max = 0;
    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0);
}

s32 func_8008AB04(s32 arg0) {
    PadState *base = D_80051338;
    PadState *pad = &base[arg0];

    *(s32 *)((u8 *)pad + 0x50) &= ~0x4000;
    *(s32 *)((u8 *)pad + 0x54) &= ~0x4000;
    return 0;
}

s32 func_8008AB48(void) {
    return 0;
}

s32 func_8008AB50(s32 arg0) {
    u16 temp_v1;
    u32 m;

    temp_v1 = Heroes[arg0].hero_status;

    if (temp_v1 & 0x200) {
        return 1;
    }
    m = temp_v1 & 0x400;
    return m != 0;
}

s32 func_8008AB94(s32 arg0) {
    s32 ret;

    if (!(Heroes[arg0].hero_status & 0x2000)) {
        ret = 0;
    } else {
        SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
        ret = 1;
    }
    return ret;
}

s32 func_8008ABF8(s32 arg0) {
    u16 temp_v1;
    u16 act;

    temp_v1 = Heroes[arg0].hero_status;
    if (temp_v1 & 0x200) {
        act = D_8009E0E4;
    } else if (temp_v1 & 0x400) {
        act = D_8009E0E6;
    } else {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_8008AC74(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    h = base + arg0;
    if (h->hero_move->forward_speed != 0) {
        return 0;
    }
    mode = h->hero_mode;
    if (mode == 0x10 || mode == 0x18) {
        a1 = (u16)D_8009E0DC;
    } else {
        a1 = 0;
    }
    SetHeroAction((HeroInfo *)arg0, a1);
    return 1;
}

s32 func_8008ACF0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 mode;
    s32 a1;

    if (D_8009D728(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode == 0xF || mode == 0x17) {
                SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0DE);
            } else {
                SetHeroAction((HeroInfo *)arg0, 3);
            }
            return 1;
        }
        return 0;
    }
    return 1;
}

void func_8008AD9C(void) {
}

s32 func_8008ADA4(u8 *arg0, u8 *arg1, u8 *arg2, s32 *arg3) {
    volatile s32 t0;
    volatile s32 t1;
    EmitPart *part;
    s32 *st;
    u8 *def;
    u8 *obj;
    u8 *p;
    s32 n;
    s32 i;
    s32 t;
    s32 r;

    obj = arg1;
    i = 0;
    part = *(EmitPart **)(arg2 + 0x30);
    st = *(s32 **)(arg2 + 0x34);
    t0 = VSync(1);
    def = NULL;
    p = func_80021DD4(((FxOwner *)arg0)->fx, ((FxEmitter *)arg2)->unk24, ((FxEmitter *)arg2)->unk4);
    if (p != NULL) {
        def = p + 0x14;
    }
    if (def != NULL) {
        n = 0;
        if (*(s32 *)(arg2 + 0x2C) <= *(s32 *)(def + 0x10)) {
            st[4] = 0;
            for (;;) {
                s32 life;

                if (n >= *(s32 *)(def + 0x14)) {
                    break;
                }
                if (!(*(s32 *)obj & 0x8000)) {
                func_8001F604(obj, part, def);
                life = *(s32 *)(def + 0x18);
                if (life == 0) {
                    life = *(s32 *)(arg2 + 0x20);
                }
                if (*(s32 *)(def + 0x58) != 0) {
                    st[0] = 0x1000 / *(s32 *)(def + 0x58);
                    *(s16 *)(obj + 0x76) = 0x1000;
                }
                if (*(s32 *)(def + 0x5C) != 0) {
                    st[1] = 0x1000 / (life - *(s32 *)(def + 0x5C));
                }
                if (*(s32 *)(def + 0x60) != 0) {
                    st[2] = 0x1000 / *(s32 *)(def + 0x60);
                    *(s32 *)(obj + 0x28) = 0;
                    *(s32 *)(obj + 0x24) = 0;
                    *(s32 *)(obj + 0x20) = 0;
                }
                if (*(s32 *)(def + 0x64) != 0) {
                    if (life != *(s32 *)(def + 0x64)) {
                        st[3] = 0x1000 / (life - *(s32 *)(def + 0x64));
                    } else {
                        st[3] = 0;
                    }
                }
                if (*(s32 *)(def + 0x68) != 0) {
                    *(s16 *)(obj + 0x14) = Random(0x1000);
                }
                n++;
                *(s32 *)(arg2 + 0x2C) += 1;
                }
                obj = *(u8 **)(obj + 0x5C);
                part++;
            }
        }
        part = *(EmitPart **)(arg2 + 0x30);
        obj = arg1;
        for (; i < *(s32 *)(def + 0x10); i++, obj = *(u8 **)(obj + 0x5C), part++) {
            if (obj == NULL || !(*(s32 *)obj & 0x8000)) {
                continue;
            }
            t = *(s32 *)(def + 0x58);
            if (t != 0 && part->age < t) {
                if ((*(s16 *)(obj + 0x76) -= st[0]) < 0) {
                    *(s16 *)(obj + 0x76) = 0;
                }
            }
            t = *(s32 *)(def + 0x5C);
            if (t != 0 && t < part->age) {
                if ((*(s16 *)(obj + 0x76) += st[1]) > 0x1000) {
                    *(s16 *)(obj + 0x76) = 0x1000;
                }
            }
            t = *(s32 *)(def + 0x60);
            if (t != 0 && t >= part->age) {
                if ((*(s32 *)(obj + 0x20) += st[2]) <= 0) {
                    *(s32 *)(obj + 0x20) = 0;
                }
                *(s32 *)(obj + 0x24) = *(s32 *)(obj + 0x28) = *(s32 *)(obj + 0x20);
            }
            t = *(s32 *)(def + 0x64);
            if (t != 0 && part->age >= t) {
                if ((*(s32 *)(obj + 0x20) -= st[3]) <= 0) {
                    *(s32 *)(obj + 0x20) = 0;
                }
                *(s32 *)(obj + 0x24) = *(s32 *)(obj + 0x28) = *(s32 *)(obj + 0x20);
            }
            func_8002147C((VelObj *)obj, (VelParams *)def, (VelState *)part);
            if (!(*(s32 *)obj & 0x8000) && *(s32 *)(def + 0x6C) != 0) {
                if ((*(s32 *)(arg2 + 0x2C) -= 1) < 0) {
                    *(s32 *)(arg2 + 0x2C) = 0;
                }
                part->age = 0;
            }
        }
    }
    if (*arg3 == 1) {
        obj = arg1;
        while (obj != NULL) {
            s32 t;
    s32 r;

            *(s32 *)obj &= ~0x8000;
            *(s32 *)(obj + 0x20) = 0x1000;
            *(s32 *)(obj + 0x28) = 0x1000;
            *(s32 *)(obj + 0x24) = 0x1000;
            *(s16 *)(obj + 0x76) = 0;
            t = *(s32 *)(def + 0x14);
            obj = *(u8 **)(obj + 0x5C);
            *(s32 *)(arg2 + 0x2C) = t;
        }
        *arg3 = 0;
    }
    if (*(s32 *)(arg0 + 0x14) >= *(s32 *)(def + 0x1C) << 8) {
        *arg3 = 2;
    }
    t1 = VSync(1);
    t1;
    t0;
    return st[9];
}

void func_8008B29C(s32 idx) {
    EXTengine *s;
    EXTengine *e;

    if (D_8005A614[idx + 0x50] != 9) {
        s = Heroes[idx].hero_shadow;
        if (((EngineGameView *)D_8005A614)->mode != 0x1C) {
            s->sub->unk6C = D_800546C4;
            s->sub->unk74 = 0x202F;
        } else {
            s->sub->unk6C = D_80055534;
            s->sub->unk74 = 0x2014;
        }
        e = Heroes[idx].hero_eng;
        s->unk10 = e->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x400;
        s->sub->flags |= 0x40000000;
        if (((EngineGameView *)D_8005A614)->mode == 0x17) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x19) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x1A) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x18) {
            s->unk2C.x = s->unk2C.y = s->unk2C.z = 0x2000;
        }
    }
}

void func_8008B418(s32 idx) {
    HeroInfo *h;
    EXTengine *s;
    EXTengine *e;

    if (D_8005A614[idx + 0x50] != 9) {
        HeroInfo *base = Heroes;
        h = base + idx;
        s = h->hero_shadow;
        e = h->hero_eng;
        s->unk10 = e->unk10;
        if (((EngineGameView *)D_8005A614)->mode == 0x17) {
            s->unk10.y = 0;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x19) {
            s->unk10.y = 0;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x1A) {
            s->unk10.y = 0;
        }
        if (((EngineGameView *)D_8005A614)->mode == 0x18) {
            s->unk10.y = 0;
        }
        s->unk1E = h->hero_eng->unk1E;
        s->unk5E = 1;
        if (h->hero_status & 0xA400) {
            s->sub->flags &= 0xFFFF7FFF;
        }
    }
}

void InitScriptFX(NAS *nas)
{
  u32 id;
  EXTscriptdata *sd;
  u8 *dbase;
  u8 *scripts;
  u8 *hdr;
  NAS *new_var;
  u8 *th;
  u8 *r;
  u8 *k;
  u8 *kf;
  s32 *ent;
  threaddata *t;
  subengine *sub;
  s32 i;
  s32 j;
  EXTengine *eng;
  EXT *fux;
  s32 type;
  s32 frames;
  s32 keys;

  new_var = nas;
  eng = new_var->Engine;
  if (eng == 0)
  {
    KillNAS(new_var);
    return;
  }
  eng->flags |= 0x8000;
  new_var->Engine->sub->flags = 0x42008000;
  new_var->Engine->unk5D = 0;
  new_var->Engine->unk5C = 1;
  eng = new_var->Engine;
  eng->unk2C.z = 0x1000;
  eng->unk2C.y = 0x1000;
  eng->unk2C.x = 0x1000;
  fux = new_var->FUX;
  if (fux == 0)
  {
    sd = AddExt(new_var, 0x3BC, 0x10000000);
    new_var->FUX = (EXT *) sd;
    if (sd == 0)
    {
      goto kill;
    }
    if (new_var->trigger == 0)
    {
      return;
    }
  }
  else
  {
    sd = (EXTscriptdata *) fux;
  }

  if (sd == 0)
  {
    return;
  }
  if (!(sd->flags & 4))
  {

    new_var->Engine->sub->unk6C = *((void **) D_80058AD0);

  }

  scripts = new_var->Engine->sub->unk6C;
  if (scripts == 0)
  {
    printf("script dbase NULL (probably not loaded)\n");
    goto kill;
  }

  dbase = scripts;

  sd->dbase = (obj_dbase *) dbase;
  scripts = *((u8 **) (dbase + 0xC));
  if (scripts == 0)
  {
    return;
  }
  sub = new_var->Engine->sub;
  sd->times = 0;
  id = 0;
  if (new_var->trigger != 0)
  {
    MapTriggerInfo *info = new_var->trigger->InfoStruct;
    sd->scriptnumber = info->val1;
    sd->speed = info->val2;
    if (sd->speed == 0)
    {
      sd->speed = 0x100;
    }
    id = info->val3;
    sd->maxloops = 0;
    sd->flags = 0;
  }

  sd->timer = 0;
  if ((*((s32 *) (scripts + 0x48))) == 0)
  {
    return;
  }
  if (sd->scriptnumber >= (*((s32 *) (scripts + 0x48))))
  {
    printf("requested script %d out of range\n", sd->scriptnumber);
    goto kill;
  }

  hdr = (scripts + ((*((s32 *) (scripts + 0x4C))) + 0x4C)) + (sd->scriptnumber * 4);
  hdr += *((s32 *) hdr);
  sd->threadcount = *((s32 *) hdr);
  sd->subcount = *((s32 *) (hdr + 4));
  ent = (s32 *) (hdr + 0x1C);
  if (func_80018F3C(new_var->Engine, sd->subcount) != sd->subcount)
  {
    for (i = 0; i < sd->threadcount; i++)
    {
      t = &sd->threads[i];
      t->threadinfo = 0;
      t->threadcontrol = 0;
    }

    goto kill;
  }

  i = 0;
  if (sd->threadcount != 0)
  {
    goto start;
  }
  goto kill;
  nokey:
  printf("must define more than one keyframe\n");

  kill:
  {

    KillNAS(new_var);
  }

  return;

  start:
  {

    new_var->Engine->kill = func_8008CFBC;
  }

  while (1)
  {
    if (i >= sd->threadcount)
    {
      break;
    }
    t = &sd->threads[i];
    th = ((u8 *) ent) + (*ent);
    type = *((s32 *) th);
    frames = *((s32 *) (th + 0xC));
    keys = *((s32 *) (th + 0x10));
    sd->totalframes = frames;
    t->threadtype = type;
    t->scriptnumber = sd->scriptnumber;
    t->keyindex = 0;
    t->keycount = keys;
    t->framecount = frames;
    t->start = new_var->Engine->unk10;
    t->threadnumber = i;
    t->timer = 0;
    t->threadinfo = 0;
    t->threadcontrol = 0;
    switch (t->threadtype)
    {
      case 0:
        if (keys == 0)
      {
        goto nokey;
      }
        if (sub == 0)
      {
        break;
      }
        sub->unk6C = dbase;
        sub->unk68 = id;
        sub->flags = 0x42000000;
        sub->flags = ((*((u32 *) (th + 0x18))) | 0x42000000) & (~0x8000);
        if (sd->flags & 8)
      {
        sub->unk51 = 3;
      }
      else
      {
        sub->unk51 = 2;
      }
        sub->unk50 = 1;
        r = func_80021DD4(dbase, t->scriptnumber, t->threadnumber);
        kf = 0;
        if (r != 0)
      {
        kf = r + 0x24;
      }
        if (kf == 0)
      {
        break;
      }
        sub->unk10 = *((u16 *) (kf + 0x18));
        sub->unk12 = *((u16 *) (kf + 0x1C));
        sub->unk14 = *((u16 *) (kf + 0x20));
        sub->unk04 = *((s32 *) (kf + 0xC));
        sub->unk08 = *((s32 *) (kf + 0x10));
        sub->unk0C = *((s32 *) (kf + 0x14));
        sub->unk20.x = *((s32 *) (kf + 0x40));
        sub->unk20.y = *((s32 *) (kf + 0x44));
        sub->unk20.z = *((s32 *) (kf + 0x48));
        sub->unk74 = *((u16 *) (kf + 0x8));
        sub->unk52 = 1;
        sub = sub->next;
        break;

      case 1:
        r = func_80021DD4(dbase, t->scriptnumber, t->threadnumber);
        k = 0;
        if (r != 0)
      {
        k = r + 0x14;
      }
        if (k == 0)
      {
        break;
      }
        if (mal_first_free(((u32) ((*((s32 *) (k + 0x10))) * 0x28)) >> 2, D_8004E0F0.free) == 0)
      {
        goto kill;
      }
        func_800115D8((*((s32 *) (k + 0x10))) * 0x28, &t->threadinfo, (s32) (&D_8004E0F0), 0);
        if (mal_first_free(0xA, D_8004E0F0.free) == 0)
      {
        goto kill;
      }
        func_800115D8(0x28, &t->threadcontrol, (s32) (&D_8004E0F0), 0);
        t->particlesactive = *((s32 *) (k + 0x14));
        ((pcontrol *) t->threadcontrol)->freqcounter = 0;
        j = 1;
        if ((*((s32 *) (k + 0x10))) > 0)
      {
        do
        {
          if (sub != 0)
          {
            sub->flags = 0x42000000;
            if ((*((u32 *) k)) & 0x20000000)
            {
              sub->flags = 0x62000000;
            }
            sub->unk6C = dbase;
            sub->unk68 = id;
            sub->flags &= ~0x8000;
            if (sd->flags & 8)
            {
              sub->unk51 = 3;
            }
            else
            {
              sub->unk51 = 2;
            }
            sub->unk50 = 1;
            sub->unk04 = 0;
            sub->unk08 = 0;
            sub->unk0C = 0;
            sub->unk74 = *((u16 *) (k + 0x28));
            sub->unk20.x = 0x1000;
            sub->unk20.z = 0x1000;
            sub->unk20.y = 0x1000;
            sub->unk76 = 0;
          }
          sub = sub->next;
        }
        while ((j++) < (*((s32 *) (k + 0x10))));
      }
        ((pcontrol *) t->threadcontrol)->lastsub = sub;
        break;

    }

    ent++;
    i++;
  }


}

void PlotScriptFX(NAS *nas) {
    volatile s32 t0 = 0;
    volatile s32 t1 = 0;
    EXTscriptdata *sd;
    subengine *sub;
    threaddata *t;
    threaddata *u;
    ScriptKey *kf;
    ScriptKey *cur;
    ScriptKey *next;
    ScriptKey *p;
    u8 *r;
    s32 i;
    s32 j;
    s32 k;
    s32 state;
    s32 f;
    s32 d;
    s32 start;
    s32 nf;
    s32 alpha;
    u32 fl;
    s32 first;

    t0 = VSync(1);
    if (((EngineGameView *)D_8005A614)->mode == 0x2C && D_8009E194 != 0) {
        return;
    }
    sd = (EXTscriptdata *)nas->FUX;
    i = 0;
    if (sd != NULL) {
        sub = nas->Engine->sub;
        sd->timer += sd->speed;
        while (1) {
            if (i >= sd->threadcount) {
                break;
            }
            t = &sd->threads[i];
            switch (t->threadtype) {
            case 1:
                state = 0;
                if (sd->timer >= sd->totalframes << 8) {
                    state = 1;
                }
                sub = (subengine *)func_8008ADA4((u8 *)sd, (u8 *)sub, (u8 *)t, &state);
                if (nas->trigger == NULL && state == 2) {
                    j = 0;
                    while (1) {
                        if (j >= sd->threadcount) {
                            break;
                        }
                        u = &sd->threads[j];
                        if (u->threadinfo != NULL) {
                            mal_free(u->threadinfo, &D_8004E0F0);
                        }
                        if (u->threadcontrol != NULL) {
                            mal_free(u->threadcontrol, &D_8004E0F0);
                        }
                        j++;
                    }
                    func_800270B0(nas);
                    return;
                }
                break;
            case 0:
                r = func_80021DD4((u8 *)sd->dbase, t->scriptnumber, t->threadnumber);
                kf = NULL;
                if (r != NULL) {
                    kf = (ScriptKey *)(r + 0x24);
                }
                if (kf != NULL) {
                    t->fdata = kf;
                    p = &kf[t->keyindex];
                    start = p->frame;
                    cur = p;
                    p++;
                    next = p;
                    nf = next->frame;
                    d = nf - start;
                    first = kf->frame;
                    p = &kf[t->keycount - 1];
                    if (p->frame < sd->timer >> 8 || sd->timer >> 8 < first) {
                        sub->flags &= ~0x8000;
                    } else {
                        sub->flags |= 0x8000;
                    }
                    f = 0;
                    do {} while (0);
                    f = 0;
                    do {} while (0);
                    f = 0;
                    if (sd->timer - start > 0) {
                        f = sd->timer - (start << 8);
                    }
                    if (sub != NULL && (sub->flags & 0x8000)) {
                        sub->unk52 = 1;
                        if (sub->unk51 == 3) {
                            func_80020344(&cur->quat, &next->quat, (f << 12) / d >> 8, (Quat *)&sub->unk10);
                        } else {
                            sub->unk12 = *(u16 *)&cur->ry - ((-next->ry + cur->ry) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                            sub->unk10 = *(u16 *)&cur->rx - ((-next->rx + cur->rx) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                            sub->unk14 = *(u16 *)&cur->rz + ((next->rz - cur->rz) << 20 >> 20) * f / (d << 8);
                            sub->unk52 = 1;
                        }
                        sub->unk04 = cur->x + (next->x - cur->x) * f / (d << 8);
                        sub->unk0C = cur->z + (next->z - cur->z) * f / (d << 8);
                        sub->unk08 = cur->y + (next->y - cur->y) * f / (d << 8);
                        sub->unk20.x = cur->sx + (next->sx - cur->sx) * f / (d << 8);
                        sub->unk52 = 1;
                        sub->unk20.y = cur->sy + (next->sy - cur->sy) * f / (d << 8);
                        sub->unk52 = 1;
                        sub->unk20.z = cur->sz + (next->sz - cur->sz) * f / (d << 8);
                        fl = sub->flags & ~0x40000000;
                        sub->flags = fl;
                        alpha = cur->fade + (next->fade - cur->fade) * f / (d << 8);
                        sub->unk76 = 0x1000 - alpha;
                        sub->flags = fl | 0x40000000;
                    }
                    p = NULL;
                    if (sd->timer >= nf << 8) {
                        t->keyindex++;
                    }
                    if (sd->timer >= t->framecount << 8) {
                        if (sd->flags & 1) {
                            sd->times++;
                            if (sd->maxloops < sd->times) {
                                k = 0;
                                while (1) {
                                    if (k >= sd->threadcount) {
                                        break;
                                    }
                                    u = &sd->threads[k];
                                    if (u->threadinfo != NULL) {
                                        mal_free(u->threadinfo, &D_8004E0F0);
                                    }
                                    if (u->threadcontrol != NULL) {
                                        mal_free(u->threadcontrol, &D_8004E0F0);
                                    }
                                    k++;
                                }
                                func_800270B0(nas);
                                return;
                            }
                        }
                        sub->unk20.x = 0x1000;
                        sub->unk20.z = 0x1000;
                        sub->unk20.y = 0x1000;
                        sub->unk76 = 0;
                        sub->unk52 = 1;
                        if (((EngineGameView *)D_8005A614)->mode == 0x2C) {
                            sub->flags &= ~0x8000;
                        }
                        t->keyindex = 0;
                    }
                }
                sub = sub->next;
                break;
            }
            i++;
        }
        if (sd->timer >= sd->totalframes << 8) {
            sd->timer = 0;
            if (((EngineGameView *)D_8005A614)->mode == 0x2C) {
                D_8009E194 = 1;
            }
        }
    }
    t1 = VSync(1);
    t1;
    t0;
}

void PlotSmokePoof(NAS *m) {
    s32 w = m->UserInfo.user1;
    EXTengine **u;
    EXTengine *e;
    s32 x;
    s32 z;

    if (w != 0) {
        u = (EXTengine **)w;
        if (D_8005A648 != 0x1C) {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) >> 4;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) >> 4;
            x = e->unk10.z;
        } else {
            m->Engine->unk10.x += (u[1]->unk10.x - m->Engine->unk10.x) / 6;
            z = (u[1]->unk10.z - (e = m->Engine)->unk10.z) / 6;
            x = e->unk10.z;
        }
        x += z;
        e->unk10.z = x;
        m->Engine->unk10.y = u[1]->unk10.y - ((u[1]->unk2C.x * 0x1A4) >> 12);
        m->Engine->unk2C.x = u[1]->unk2C.x;
        m->Engine->unk2C.y = u[1]->unk2C.y;
        m->Engine->unk2C.z = u[1]->unk2C.z;
    }
    func_800250B0(m);
}

s32 func_8008C4C8(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = *(u8 **)((u8 *)D_80058AD0 + 0x10);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = D_80068BD4;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

s32 func_8008C5C4(s32 *from, s32 *to, Arc *arc, s32 *pos) {
    s32 dx;
    s32 dz;
    s32 dist;
    s32 n;
    s32 dy;
    s32 hit;
    s32 ang;

    hit = 0;
    if (arc->started == 0) {
        dx = to[0] - from[0];
        dz = to[2] - from[2];
        n = 0;
        dist = csqrt(dx * dx + dz * dz) >> 6;
        dy = to[1] - from[1];
        if (arc->speed != 0) {
            n = dist / arc->speed;
        }
        if (n == 0) {
            return 1;
        }
        arc->vy = arc->vy0 = dy / n - (n >> 1) * arc->gravity;
        ang = get_angle(dx, dz) & 0xFFF;
        arc->angle = ang;
        arc->vx = arc->speed * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        arc->vz = arc->speed * D_80068BD4[arc->angle & 0xFFF].unk2 >> 12;
        arc->started = 1;
    }
    pos[1] += arc->vy;
    arc->vy += arc->gravity;
    pos[0] += arc->vx;
    pos[2] += arc->vz;
    if (pos[1] >= arc->floor && arc->vy > 0) {
        pos[1] -= abs(pos[1] - arc->floor);
        hit = 1;
    }
    return hit;
}

void func_8008C824(SparkObj *obj) {
    SparkPoly *p0;
    Spark *s;
    SparkPoly *p1;
    SparkTex *tex;
    s32 n;
    s16 a;
    s32 t;
    s32 r;
    SparkSet *ss;

    ss = &obj->set;
    n = ss->count;
    p0 = ss->polys0;
    p1 = ss->polys1;
    s = obj->set.sparks;
    tex = (SparkTex *)(D_80054944 + 0xAF0);
    if (D_8005A648 == 0x22) {
        tex = (SparkTex *)(*(u8 **)((u8 *)*(void **)D_80058AD0 + 0x18) + 0x2BF8);
    }
    if (D_8005A648 == 0x13) {
        tex = (SparkTex *)(*(u8 **)((u8 *)*(void **)D_80058AD0 + 0x18) + 0x6C8);
    }
    if (D_8005A648 == 0x1D) {
        tex = (SparkTex *)(*(u8 **)((u8 *)*(void **)D_80058AD0 + 0x18) + 0xA10);
    }
    for (; n != 0; n--) {
        do {
        do {
        a = Random(0x2000);
        a -= Random(0x2000);
        s->x = a;
        s->y = -Random(0x1B58);
        a = Random(0x2000);
        r = Random(0x2000);
        } while (0);
        } while (0);
        a -= r;
        s->z = a;
        s->y2 = s->y + 0x40;
        s->z2 = s->z;
        t = Random(0xFFF);
        s->ang = t;
        s->base_x = s->x;
        s->x = s->base_x + ((D_80068BD4[(t - 0x400) & 0xFFF].unk2 * 512 >> 12));
        s->x2 = s->x + 0x40;
        s++;
        *(u32 *)&p0->r0 = 0xA0A0A0;
        p0->tpage = (tex->tpage & 0xFF9F) | 0x20;
        p0->clut = tex->clut;
        p0->uv0 = tex->uv0;
        p0->uv1 = tex->uv1;
        p0->uv2 = tex->uv2;
        p0->uv3 = tex->uv3;
        ((u8 *)&p0->tag)[3] = 9;
        p0->code = 0x2E;
        *p1++ = *p0++;
    }
}

void func_8008CA90(u8 *arg0, MATRIX *m) {
    FallParticles *fp = (FallParticles *)(arg0 + 0x6C);
    FallParticle *p;
    POLY_FT4 *prim;
    s32 n;
    s32 col;
    s32 tag2;
    s16 *sxy;
    s32 tag;
    s32 t;
    s32 r;
    SVECTOR *v;

    p = fp->parts;
    prim = fp->prims[D_8005B690 & 1];
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    n = fp->count;
    col = calc_rgbcode(0x10, 0x10, 0x10);
    tag = col | 0x2E000000;
    while (n != 0) {
        p->top.vy += 0x18;
        p->top.vx = p->base + (D_80068BD4[(p->angle - 0x400) & 0xFFF].unk2 * 512 >> 12);
        p->angle = (p->angle + 0x10) & 0xFFF;
        if (p->top.vy > 0x800) {
            p->angle = Random(0xFFF);
            p->base = Random(0x3000) - Random(0x3000);
            p->top.vx = p->base + (D_80068BD4[(p->angle - 0x400) & 0xFFF].unk2 * 512 >> 12);
            t = Random(0x4000) - Random(0x1000);
            p->top.vz = t;
            p->top.vy = -0xDAC;
            p->bot.vx = p->top.vx + 0x40;
            p->bot.vz = p->top.vz;
        }
        p->bot.vy = p->top.vy + 0x40;
        p->bot.vx = p->top.vx + 0x40;
        v = &p->top;
        do { gte_ldv0(v); } while (0);
        gte_rtps();
        tag2 = tag;
        sxy = &prim->x0;
        do { *(s32 *)&prim->r0 = tag2; } while (0);
        gte_stsxy(sxy);
        v = &p->bot;
        gte_ldv0(v);
        gte_rtps();
        prim->y1 = prim->y0;
        prim->x2 = prim->x0;
        gte_stsxy(&prim->x3);
        prim->x1 = prim->x3;
        prim->y2 = prim->y3;
        addPrim(D_80056998.ot + 0x40, prim);
        prim++;
        p++;
        n--;
    }
}

void PlotWeatherNas(u8 *arg0) {
    if (*(s32 *)(arg0 + 0x60) == 0) {
        s32 v = *(s32 *)(arg0 + 0x64);
        switch (v) {
        case 0:
            func_80015D70(D_8009E200, *(u8 **)(*(u8 **)(arg0 + 0x54) + 0x6C));
            *(s32 *)(arg0 + 0x60) = 1;
            break;
        case 1:
            *(s32 *)(arg0 + 0x60) = v;
            break;
        }
    }
}

void InitWeatherNas(u8 *arg0) {
    *(s32 *)(arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x64) = 0;
    **(s32 **)(*(u8 **)(arg0 + 0x54) + 0x6C) = 0x20008000;
    if (D_8005A648 == 0x1F) {
        func_80015D70(D_800C45DC, *(u8 **)(*(u8 **)(arg0 + 0x54) + 0x6C));
    }
}

void func_8008CE10(void) {
}

void func_8008CE18(void) {
}

FlagPoint *func_8008CE20(subengine *sub, s32 idx, s32 mask) {
    PointList *list = func_8001562C(sub);
    u8 *p;
    s32 i;
    s32 n;

    if (list != NULL) {
        n = list->nRecs;
        p = (u8 *)list->pts;
        i = 0;
        if (--n != -1) {
            do {
                if (p != NULL && i == idx && (((u32 *)p)[3] & 0xC0000000) == mask) {
                    return (FlagPoint *)p;
                }
                i++;
                p += 0x10;
            } while (--n != -1);
        }
    }
    return NULL;
}

s32 func_8008CEB0(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0)) - (*(s32*)((u8*)(arg1) + 0));
    temp_v0_2 = (*(s32*)((u8*)(arg0) + 8)) - (*(s32*)((u8*)(arg1) + 8));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void PlotParticleEffect(void) {
}

void InitParticleEffect(void) {
}

void InitSmokePoof(NAS *nas) {
    func_80024E30(nas, &D_8009E1E8);
    nas->Engine->sub->unk6C = D_800546C4;
    nas->Engine->sub->flags |= 0x40000041;
    nas->UserInfo.user1 = 0;
    if (((EngineGameView *)D_8005A614)->mode == 0x1C) {
        nas->Engine->sub->flags = 0x40008021;
    }
    if (((EngineGameView *)D_8005A614)->mode == 0x17) {
        nas->Engine->unk2C.x = 0x800;
        nas->Engine->unk2C.y = 0x800;
        nas->Engine->unk2C.z = 0x800;
    }
}

void func_8008CFBC(EXTengine *eng) {
    NAS *nas;
    EXTscriptdata *sd;
    threaddata *t;
    s32 i;

    nas = ((NAS **)eng)[-1];
    if (nas != NULL) {
        sd = FindExtension(nas, 0x10000000);
        if (sd != NULL) {
            i = 0;
            while (1) {
                if (i >= sd->threadcount) {
                    break;
                }
                t = &sd->threads[i];
                if (t->threadinfo != NULL) {
                    mal_free(t->threadinfo, &D_8004E0F0);
                }
                if (t->threadcontrol != NULL) {
                    mal_free(t->threadcontrol, &D_8004E0F0);
                }
                i++;
            }
            func_8001D174(eng);
        }
    }
}

void func_8008D088(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4) {
    s32 life;
    s32 t;
    s32 r;
    s32 u;

    func_8001F604(arg0, arg2, arg1);
    life = *(s32 *)(arg1 + 0x18);
    if (life == 0) {
        life = *(s32 *)(arg4 + 0x20);
    }
    t = *(s32 *)(arg1 + 0x58);
    if (t != 0) {
        arg3[0] = 0x1000 / t;
        *(s16 *)(arg0 + 0x76) = 0x1000;
    }
    u = *(s32 *)(arg1 + 0x5C);
    if (u != 0) {
        arg3[1] = 0x1000 / (life - u);
    }
    t = *(s32 *)(arg1 + 0x60);
    if (t != 0) {
        arg3[2] = 0x1000 / t;
        *(s32 *)(arg0 + 0x28) = 0;
        *(s32 *)(arg0 + 0x24) = 0;
        *(s32 *)(arg0 + 0x20) = 0;
    }
    u = *(s32 *)(arg1 + 0x64);
    if (u != 0) {
        if (life != u) {
            arg3[3] = 0x1000 / (life - u);
        } else {
            arg3[3] = 0;
        }
    }
    if (*(s32 *)(arg1 + 0x68) != 0) {
        *(s16 *)(arg0 + 0x14) = Random(0x1000);
    }
    *(s32 *)(arg4 + 0x2C) += 1;
}

u8 *func_8008D24C(u8 *arg0, s32 *arg1) {
    u8 *r;

    r = func_80021DD4(arg0, arg1[0x24 / 4], arg1[1]);
    if (r != NULL) {
        return r + 0x24;
    }
    return NULL;
}

u8 *func_8008D284(u8 *arg0, u8 *arg1) {
    u8 *p = func_80021DD4(arg0, *(s32 *)(arg1 + 0x24), *(s32 *)(arg1 + 4));
    if (p != NULL) {
        return p + 0x14;
    }
    return NULL;
}

void PlotFloatyScore(NAS *nas) {
    EXTengine *eng = nas->Engine;
    s32 s;
    s32 one;
    s32 ang;
    s32 y;

    one = 1;
    if (eng != NULL) {
        switch (nas->UserInfo.status) {
        case 0:
            if (nas->UserInfo.user1 <= 0 || --nas->UserInfo.user1 <= 0) {
                nas->UserInfo.status = one;
                nas->UserInfo.user1 = 0;
            }
            break;
        case 1:
            eng->unk10.y -= 0x20;
            if (nas->UserInfo.user2 - (D_8006BBD6[0] * 3 >> 7) >= eng->unk10.y) {
                nas->UserInfo.status = 2;
                nas->UserInfo.flags = 0x1E;
            }
            break;
        case 2:
            ang = ++nas->UserInfo.user1;
            y = nas->UserInfo.user2;
            ang = D_80068BD4[((ang << 7) & 0xFFF) - 0x400 & 0xFFF].unk2;
            eng->unk5E = one;
            eng->unk10.y = y - (ang * 3 >> 7);
            s = eng->unk2C.x + 0x40;
            eng->unk2C.z = s;
            eng->unk2C.y = s;
            eng->unk2C.x = s;
            if (--nas->UserInfo.flags == 0) {
                nas->UserInfo.status = 3;
                nas->UserInfo.flags = 0x3C;
                nas->UserInfo.user1 = 6;
            }
            break;
        case 3:
            eng->unk10.y -= nas->UserInfo.user1;
            eng->unk5E = one;
            eng->unk2C.x = eng->unk2C.y = eng->unk2C.z -= (eng->unk2C.x - 0x800) >> 5;
            nas->UserInfo.user1 += 3;
            if (--nas->UserInfo.flags == 0) {
                KillNAS(nas);
            }
            break;
        }
    }
}

void func_8008D488(u8 *arg0, s32 arg1) {
    s32 a;
    s32 b;

    if (arg1 != 0) {
        arg0[4] = 0x25;
        arg0[3] = (arg1 % 10) + 0x30;
        a = ((arg1 / 10) % 10) + 0x30;
        arg0[2] = a;
        b = (((arg1 / 10) / 10) % 10) + 0x30;
        arg0[1] = b;
        b &= 0xFF;
        if (b == 0x30) {
            s32 c = a & 0xFF;
            do { } while (0);
            arg0[1] = 0x20;
            if (b == c) {
                arg0[2] = 0x20;
            }
        }
    } else {
        arg0[1] = 0x20;
        arg0[2] = 0x4F;
        arg0[3] = 0x46;
        arg0[4] = 0x46;
    }
}

void InitFloatyScore(u8 *arg0) {
    u8 *temp_v1;

    temp_v1 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_v1 != NULL) {
        (*(s16*)((u8*)(arg0) + 94)) = 0x1E;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = (s32) ((*(s32*)((u8*)(temp_v1) + 20)) - 0x1A0);
        (*(s8*)((u8*)(temp_v1) + 93)) = 2;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void func_8008D5D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *eng;
    NAS *nas;
    subengine *sub;
    s32 off;

    if (arg3 >= 0x64) {
        arg3 = 0x63;
    }
    nas = CreateNAS(arg0, arg1, arg2, 0x1409, 0x11);
    if (nas != NULL) {
        eng = nas->Engine;
        nas->UserInfo.user1 = arg4;
        if (eng != NULL) {
            off = 0;
            if (arg3 >= 10) {
                func_80018F3C(eng, 1);
            }
            sub = eng->sub;
            if (sub != NULL) {
                while (sub != NULL) {
                    sub->unk6C = D_80053074;
                    sub->unk04 = off;
                    sub->unk74 = arg3 % 10 + 0x200E;
                    sub = sub->next;
                    arg3 /= 10;
                    if (sub != NULL) {
                        off -= 0xB4;
                    }
                }
            }
        } else {
            KillNAS(nas);
        }
    }
}

void func_8008D6C8(NAS *nas) {
    EXTengine *eng;
    EXTengine *next;
    EXTmotion *m;
    s32 dy;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    m = FindExtension(nas, 0x800);
    if (m == NULL) {
        return;
    }
    m->Yvel += m->Yacc - m->Ydec;
    if (m->Yvel < 0) {
        m->Yvel = 0;
    }
    if (m->Yvel > m->Ymax) {
        m->Yvel = m->Ymax;
    }
    if (m->Ydir) {
        eng->unk10.y -= m->Yvel;
    } else {
        eng->unk10.y += m->Yvel;
    }
    dy = ((s32 (*)(EXTengine *))func_80025F44)(eng);
    next = eng->Engine;
    if (next != NULL) {
        next->unk10 = eng->unk10;
        next->unk10.y -= dy;
    }
    if (m->Yvel == 0) {
        s16 acc = m->Ydec;
        m->Ydec = 0;
        m->Ydir = 0;
        m->Yacc = acc;
    }
    if (m->Ydir == 0 && func_80025BBC(eng) != 0) {
        m->Ydec = 8;
        m->Ydir = 0xFF;
        m->Yacc = 0;
        m->Yvel = 0x40;
    }
}

void Init_HealthPU(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    EXTengine *e2;
    s32 x;
    s32 z;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->sub->unk6C = D_80052F5C;
        nas->UserInfo.user2 = D_8004E0D8;
        eng->sub->unk74 = Random(0xE) + 0x2018;
        obj = (EXTobject *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            nas->FUX = (EXT *)obj;
            obj->whatami = 0x11;
            obj->can_do |= 0x18;
            func_80025AE4((u8 *)eng, 0, 0, 0, 0x80, -0x100, 0x40);
            if (eng->unk10.y == World_floor_level) {
                obj->status = 1;
            } else {
                obj->status = 4;
            }
            mot = AddMotion(nas);
            if (mot != NULL) {
                mot->Ymax = 0xA0;
                mot->Ydec = 0xE;
                mot->Yacc = 0;
                mot->Ydir = 0xFF;
                mot->Yvel = 0;
            }
            e2 = AddExtEngine(nas, 1);
            if (e2 != NULL) {
                e2->sub->unk6C = D_80052F5C;
                e2->sub->unk74 = 0x2001;
                e2->unk10 = nas->Engine->unk10;
                e2->flags |= 0x8041;
                e2->sub->unk76 = 0x800;
                e2->sub->flags |= 0x40000000;
            }
            PlaySfx(0x3FA, NULL, NULL, 0x1000, 0x1E00, 0);
            x = (eng->unk10.x >> 8) + 10;
            z = 9 - (eng->unk10.z >> 8);
            if (x >= 20) {
                x = 19;
            }
            if (x < 0) {
                x = 0;
            }
            if (z >= 20) {
                z = 19;
            }
            if (z < 0) {
                z = 0;
            }
            D_8009D90C[z * 20 + x].unk0 = 0x11;
        }
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Plot_HealthPU(NAS *nas) {
    EXTengine *eng;
    EXTobject *obj;
    EXTmotion *mot;
    s32 p;
    s32 n;
    s32 st;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    nas->UserInfo.user2++;
    eng->sub->unk74 = (nas->UserInfo.user2 >> 1) % 14 + 0x2018;
    obj = (EXTobject *)nas->FUX;
    if (obj == NULL) {
        return;
    }
    st = obj->status;
    if (st < 6) {
        if (st < 4) {
            goto plot;
        }
        mot = FindExtension(nas, 0x800);
        if (mot != NULL && GravitateNas(nas) != 0) {
            if (obj->status == 4) {
                PlaySfx(D_8009E214[Random(8)], NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
            obj->status = 1;
            mot->Ydec = 8;
            mot->Ydir = 0xFF;
            mot->Yacc = 0;
            mot->Yvel = 0x40;
        }
        obj->will_do &= ~8;
        return;
    }
plot:
    func_8008D6C8(nas);
    if (obj->will_do & 8) {
        p = obj->player;
        if (p != -1 && p < 5 && !(Heroes[p].hero_status & 0xFA00)) {
            if (D_8005A614[p + 0x5C] != 0) {
                n = 3 - D_8005A614[0x9B];
            } else {
                n = 0;
            }
            if (D_8009D908 != 0) {
                n = 0;
            }
            Heroes[obj->player].hero_health += D_8009E224[n];
            if (Heroes[obj->player].hero_health > 20) {
                Heroes[obj->player].hero_health = 20;
            }
            PlaySfx(0x1C2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        KillNAS(nas);
    }
}

void PlotBigPoof(NAS *nas) {
    func_800250B0(nas);
}

void InitBigPoof(NAS *nas) {
    EXTengine *eng = nas->Engine;

    if (eng != NULL) {
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0;
        func_80024E30(nas, &D_8009E250);
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_8008DDC0(NAS *nas) {
    EXTmotion *mot;
    EXTengine *eng;
    EXTengine *src;

    mot = AddMotion(nas);
    if (mot != NULL) {
        mot->Ymax = 0xA0;
        mot->Ydec = 0xE;
        mot->Yacc = 0;
        mot->Ydir = 0xFF;
        mot->Yvel = 0;
    }
    eng = AddExtEngine(nas, 1);
    if (eng != NULL) {
        eng->sub->unk6C = D_80052F5C;
        eng->sub->unk74 = 0x2001;
        src = nas->Engine;
        eng->unk10 = src->unk10;
        eng->flags |= 0x8041;
        eng->sub->unk76 = 0x800;
        eng->sub->flags |= 0x40000000;
    }
}

s32 func_8008DE8C(void) {
    return 0;
}

s32 func_8008DE94(s32 arg0) {
    return (Heroes[arg0].hero_status & 0xFA00) == 0;
}

void func_8008DEC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    EXTengine *eng;
    HeroInfo *hero;
    NAS *nas;
    FuxFlags *fux;
    EXTengine *e;
    s32 x, y, z;
    s32 dx, dz;
    s32 dist;
    s32 i;
    s32 ok;
    s32 ang;
    s32 vc;

    if (D_8005A644 & 0x801) {
        return;
    }
    eng = ((NAS *)arg0)->Engine;
    i = 0;
    if (eng == NULL) {
        return;
    }
    hero = Heroes;
    x = eng->unk10.x;
    y = eng->unk10.y;
    z = eng->unk10.z;
    do {
        if (arg2 != i && !(hero[i].hero_status & 0xFA00)) {
            if (arg4 != 0) {
                if (hero[i].hero_mode == 4 ? hero[i].hero_eng->unk10.y < y - arg4 : arg1 < y) {
                    continue;
                }
            }
            e = hero[i].hero_eng;
            dx = e->unk10.x - x;
            dz = e->unk10.z - z;
            dist = csqrt(dx * dx + dz * dz) >> 6;
            if (dist < arg1) {
                if ((D_8005A618 & 0x80000001) != 0x80000001) {
                    ok = 1;
                    if (hero[i].hero_hit != 0) {
                        ok = hero[i].hero_immunity_level < 0x200;
                    }
                    if (hero[i].hero_immunity != 0) {
                        ok = 0;
                    }
                    if (ok) {
                        if (dist < 0x80) {
                            ang = hero[i].hero_move->facing_angle;
                        } else {
                            ang = get_angle(dx, dz);
                        }
                        dist = (arg1 - dist) >> 3;
                        vc = hero[i].hero_move->velocity_calc;
                        if (vc != 1) if (vc != 0) if (vc < 4) {
                            hero[i].hero_move->x_vel = (dist * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                            hero[i].hero_move->z_vel = (dist * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                            hero[i].hero_move->x_max = 0;
                            hero[i].hero_move->z_max = 0;
                        }
                        hero[i].hero_move->reactive_angle = ang ^ 0x800;
                        hero[i].hero_move->reactive_speed = dist;
                        hero[i].hero_frozen = 0x1E;
                        hero[i].hero_move->forward_angle = hero[i].hero_move->reactive_angle + 0x800;
                    }
                }
                ((void (*)(s32, NAS *, s32, s32, s32))func_80084E04)(i, NULL, -1, arg3, 0);
            }
        }
    } while (++i < 4);
    if (D_8009E494 != NULL) {
        D_8009E494(arg0, arg1, arg2, arg3);
    }
    if (abs(eng->unk10.y - World_floor_level) < 0x80) {
        arg1 = arg1 * arg1;
        for (nas = D_8005AA24; nas != NULL; nas = nas->Object.Child) {
            fux = (FuxFlags *)nas->FUX;
            if (fux != NULL && (fux->unk1C & 0x100)) {
                e = nas->Engine;
                if (e != NULL) {
                    dx = e->unk10.x - x;
                    dz = e->unk10.z - z;
                    if (dx * dx + dz * dz < arg1) {
                        fux->unk16 = -1;
                        fux->unk1E |= 0x100;
                    }
                }
            }
        }
    }
}
