#include "common.h"
#include "bash/8002d488.h"

extern u8 D_BFC7FF52;
extern s32 D_80067894;
extern s32 D_8006EA24;

void func_8002D488(void) {
    switch (D_BFC7FF52) {
    case 0x41:
        D_80067894 = 1;
        D_8006EA24 = 1;
        return;
    case 0x45:
        D_80067894 = 0;
        D_8006EA24 = 2;
        return;
    case 0x4A:
    default:
        D_80067894 = 1;
        D_8006EA24 = 0;
        return;
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/main/8002d488", func_8002D4F4);
#else
s32 func_8002D4F4(s32 arg0) {
    s32 ret;

    ret = 0;
loop:
    {
        switch (D_80067894) {
        case 1:
            func_8002E000(1);
            D_80067894 = 2;
            ret = 1;
            break;
        case 2:
            ret = func_8002DE2C(1);
            switch (ret) {
            case 0:
            default:
                ret = 2;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = 2;
                break;
            case 1:
                ret = 2;
                D_8006EA1C = D_8006788C[2];
                func_8002E000(6);
                D_80067894 = 3;
                break;
            case -1:
                func_8002E000(1);
                ret = 2;
                break;
            }
            break;
        case 3:
            ret = func_8002DE2C(6);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                D_80067884[0] = (D_8006EA1C >= 2) * 2;
                func_8002E000(2);
                D_80067894 = 4;
            case 0:
            default:
                ret =3;
                break;
            }
            break;
        case 4:
            ret = func_8002DE2C(2);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1: {
                s32 m, s, t, mm, ss;
                u8 a, b;
                a = D_8006788C[1];
                b = D_8006788C[2];
                m = (a >> 4) * 10 + (a & 0xF);
                s = (b >> 4) * 10 + (b & 0xF);
                t = (m * 60 + s) >> 1;
                D_8006EA20 = t / 60;
                mm = D_8006EA20;
                ((volatile BcdTime *)&D_8006EA20)->sec = t % 60;
                ss = ((volatile BcdTime *)&D_8006EA20)->sec;
                D_8006EA20 = cd_to_bcd(mm, mm);
                mm = ((BcdTime *)&D_8006EA20)->min;
                ((BcdTime *)&D_8006EA20)->sec = cd_to_bcd(ss, mm);
                func_8002E000(0xC);
                D_80067894 = 5;
            }
            case 0:
            default:
                ret =7;
                break;
            }
            break;
        case 5:
            ret = func_8002DE2C(0xC);
            switch (ret) {
            case -1: {
                if (!(D_8006788C[0] & 1)) {
                    D_80067894 = 1;
                } else if (D_8006788C[1] & 0x40) {
                    u8 m = D_8006EA20;
                    u8 s = ((volatile BcdTime *)&D_8006EA20)->sec;
                    D_80067884[0] = m;
                    D_80067884[1] = s;
                    D_80067884[2] = 0;
                    func_8002E000(3);
                    D_80067894 = 7;
                    ret = 5;
                }
                break;
            }
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(0xD);
                D_80067894 = 6;
            case 0:
            default:
                ret =5;
                break;
            }
            break;
        case 6:
            ret = func_8002DE2C(0xD);
            switch (ret) {
            case -1:
                func_8002E0F0();
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1: {
                u8 m = D_8006EA20;
                u8 s = ((volatile BcdTime *)&D_8006EA20)->sec;
                D_80067884[0] = m;
                D_80067884[1] = s;
                D_80067884[2] = 0;
            }
                func_8002E000(3);
                D_80067894 = 7;
            case 0:
            default:
                ret =6;
                break;
            }
            break;
        case 7:
            ret = func_8002DE2C(3);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                D_80067884[0] = ret;
                func_8002E000(5);
                D_80067894 = 8;
            case 0:
            default:
                ret =7;
                break;
            }
            break;
        case 8:
            ret = func_8002DE2C(5);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                D_8006EA18 = VSync(-1);
                D_80067894 = 9;
            case 0:
            default:
                ret =8;
                break;
            }
            break;
        case 9:
            if (VSync(-1) > D_8006EA18 + 3) {
                func_8002E000(4);
                D_80067894 = 0xA;
            }
            ret = 9;
            break;
        case 10:
            ret = func_8002DE2C(4);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(7);
                D_80067894 = 0xB;
            case 0:
            default:
                ret =0xA;
                break;
            }
            break;
        case 11:
            ret = func_8002DE2C(7);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(8);
                D_80067894 = 0xC;
            case 0:
            default:
                ret =0xB;
                break;
            }
            break;
        case 12:
            ret = func_8002DE2C(8);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                D_80067884[0] = 4;
                func_8002E000(9);
                D_80067894 = 0xD;
            case 0:
            default:
                ret =0xC;
                break;
            }
            break;
        case 13:
            ret = func_8002DE2C(9);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                D_8006EA18 = VSync(-1);
                D_80067894 = 0xE;
            case 0:
            default:
                ret =0xD;
                break;
            }
            break;
        case 14:
            if (VSync(-1) > D_8006EA18 + 200) {
                D_80067884[0] = 5;
                func_8002E000(0xA);
                D_80067894 = 0xF;
            }
            ret = 0xE;
            break;
        case 15:
            ret = func_8002DE2C(0xA);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                if (D_8006788C[1] != 0) {
                    func_8002E000(0);
                    D_80067894 = 0x10;
                } else {
                    D_8006EA18 = VSync(-1);
                    D_80067894 = 0x13;
                }
            case 0:
            default:
                ret =0xF;
                break;
            }
            break;
        case 16:
            ret = func_8002DE2C(0);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E0F0();
            case 0:
            default:
                ret =0x10;
                break;
            }
            break;
        case 17:
            ret = func_8002DE2C(0);
            switch (ret) {
            case -1:
                D_80067894 = 1;
                break;
            case 0:
            default:
                ret = 0x10;
                break;
            case 1:
                D_80067894 = ret;
                ret = 0x10;
                break;
            case -2:
                func_8002E000(0);
                ret = 0x10;
                break;
            }
            break;
        case 18:
            ret = func_8002DE2C(0xB);
            switch (ret) {
            case -2:
                func_8002E000(0);
                D_80067894 = 0x11;
                ret = -1;
                break;
            case -1:
                D_80067894 = 1;
                break;
            case 1:
                D_80067894 = 0;
            case 0:
            default:
                ret =0x12;
                break;
            }
            break;
        case 19:
            if (VSync(-1) > D_8006EA18 + 10) {
                func_8002E000(0xB);
                D_80067894 = 0x12;
            }
            ret = 0x13;
            break;
        case 0:
            ret = 0;
            break;
        }
    }
    if (arg0 == 0 && ret != 0) {
        goto loop;
    }
    return ret;
}
#endif

s32 func_8002DE2C(s32 arg0) {
    s32 limit;
    s32 n;
    s32 t;
    s32 i;
    s32 off;

    limit = D_8006783C[arg0 * 4 + 3];
    *D_80067874 = 1;
    t = *D_80067880 & 7;
    if (t != (*D_80067880 & 7)) {
        return 0;
    }
    n = t;
    if ((u8)n != 0) {
        i = 0;
        D_80067898 += (u8)n;
        *D_80067874 = 1;
        *D_80067880 = 7;
        do {
            *(s32 *)0 = i;
            i++;
        } while (i < 4);
        if (D_80067898 < limit) {
            return 0;
        }
        D_80067898 = 0;
        if (n == 5) {
            volatile u8 *src = D_80067878;

            D_8006788C[0] = *src;
            D_8006788C[1] = *src;
            *D_80067874 = 1;
            *D_8006787C = 0x1F;
            if (!(D_8006788C[0] & 0x10)) {
                return -1;
            }
            return -2;
        }
        off = arg0 * 4;
        t = 0;
        if (D_8006783C[off + 2] != 0) {
            do {
                D_8006788C[t] = *D_80067878;
                t++;
            } while (t < D_8006783C[off + 2]);
        }
        *D_80067874 = 1;
        *D_8006787C = 0x1F;
        if (arg0 == 10 || (t = D_8006788C[0], t &= 0x10, !t)) {
            return 1;
        }
        return -2;
    }
    return 0;
}

void func_8002E000(s32 arg0) {
    s32 i;
    s32 off;

    i = 0;
    *D_80067874 = 1;
    *D_80067880 = 7;
    do {
        *(s32 *)0 = i;
        i++;
    } while (i < 4);
    *D_80067874 = 1;
    *D_8006787C = 0x18;
    *D_80067874 = 0;
    off = arg0 * 4;
    i = 0;
    if (D_8006783C[off + 1] != 0) {
        do {
            *D_8006787C = D_80067884[i];
            i++;
        } while (i < D_8006783C[off + 1]);
    }
    *D_80067874 = 0;
    *D_80067878 = D_8006783C[arg0 * 4];
}
