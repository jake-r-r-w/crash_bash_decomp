#include "common.h"
#include "bash/modchip.h"

extern u8 D_BFC7FF52;
extern s32 ModchipState;
extern s32 MsgFontRom;

void func_8002D488(void) {
    switch (D_BFC7FF52) {
    case 0x41:
        ModchipState = 1;
        MsgFontRom = 1;
        return;
    case 0x45:
        ModchipState = 0;
        MsgFontRom = 2;
        return;
    case 0x4A:
    default:
        ModchipState = 1;
        MsgFontRom = 0;
        return;
    }
}

s32 func_8002D4F4(s32 arg0) {
    s32 ret;

    ret = 0;
loop:
    {
        switch (ModchipState) {
        case 1:
            func_8002E000(1);
            ModchipState = 2;
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
                ModchipState = 0x11;
                ret = 2;
                break;
            case 1:
                ret = 2;
                D_8006EA1C = CdResult[2];
                func_8002E000(6);
                ModchipState = 3;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = (D_8006EA1C >= 2) * 2;
                func_8002E000(2);
                ModchipState = 4;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1: {
                s32 m, s, t, mm, ss;
                u8 a, b;
                a = CdResult[1];
                b = CdResult[2];
                m = (a >> 4) * 10 + (a & 0xF);
                s = (b >> 4) * 10 + (b & 0xF);
                t = (m * 60 + s) >> 1;
                CdTimeBcd = t / 60;
                mm = CdTimeBcd;
                ((volatile BcdTime *)&CdTimeBcd)->sec = t % 60;
                ss = ((volatile BcdTime *)&CdTimeBcd)->sec;
                CdTimeBcd = cd_to_bcd(mm, mm);
                mm = ((BcdTime *)&CdTimeBcd)->min;
                ((BcdTime *)&CdTimeBcd)->sec = cd_to_bcd(ss, mm);
                func_8002E000(0xC);
                ModchipState = 5;
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
                if (!(CdResult[0] & 1)) {
                    ModchipState = 1;
                } else if (CdResult[1] & 0x40) {
                    u8 m = CdTimeBcd;
                    u8 s = ((volatile BcdTime *)&CdTimeBcd)->sec;
                    CdParams[0] = m;
                    CdParams[1] = s;
                    CdParams[2] = 0;
                    func_8002E000(3);
                    ModchipState = 7;
                    ret = 5;
                }
                break;
            }
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(0xD);
                ModchipState = 6;
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
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1: {
                u8 m = CdTimeBcd;
                u8 s = ((volatile BcdTime *)&CdTimeBcd)->sec;
                CdParams[0] = m;
                CdParams[1] = s;
                CdParams[2] = 0;
            }
                func_8002E000(3);
                ModchipState = 7;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = ret;
                func_8002E000(5);
                ModchipState = 8;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                ModchipVSync = VSync(-1);
                ModchipState = 9;
            case 0:
            default:
                ret =8;
                break;
            }
            break;
        case 9:
            if (VSync(-1) > ModchipVSync + 3) {
                func_8002E000(4);
                ModchipState = 0xA;
            }
            ret = 9;
            break;
        case 10:
            ret = func_8002DE2C(4);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(7);
                ModchipState = 0xB;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                func_8002E000(8);
                ModchipState = 0xC;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = 4;
                func_8002E000(9);
                ModchipState = 0xD;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                ModchipVSync = VSync(-1);
                ModchipState = 0xE;
            case 0:
            default:
                ret =0xD;
                break;
            }
            break;
        case 14:
            if (VSync(-1) > ModchipVSync + 200) {
                CdParams[0] = 5;
                func_8002E000(0xA);
                ModchipState = 0xF;
            }
            ret = 0xE;
            break;
        case 15:
            ret = func_8002DE2C(0xA);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                if (CdResult[1] != 0) {
                    func_8002E000(0);
                    ModchipState = 0x10;
                } else {
                    ModchipVSync = VSync(-1);
                    ModchipState = 0x13;
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
                ModchipState = 1;
                break;
            case -2:
                func_8002E000(0);
                ModchipState = 0x11;
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
                ModchipState = 1;
                break;
            case 0:
            default:
                ret = 0x10;
                break;
            case 1:
                ModchipState = ret;
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
                ModchipState = 0x11;
                ret = -1;
                break;
            case -1:
                ModchipState = 1;
                break;
            case 1:
                ModchipState = 0;
            case 0:
            default:
                ret =0x12;
                break;
            }
            break;
        case 19:
            if (VSync(-1) > ModchipVSync + 10) {
                func_8002E000(0xB);
                ModchipState = 0x12;
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

s32 func_8002DE2C(s32 arg0) {
    s32 limit;
    s32 n;
    s32 t;
    s32 i;
    s32 off;

    limit = D_8006783C[arg0 * 4 + 3];
    *CdRegIndex = 1;
    t = *D_80067880 & 7;
    if (t != (*D_80067880 & 7)) {
        return 0;
    }
    n = t;
    if ((u8)n != 0) {
        i = 0;
        D_80067898 += (u8)n;
        *CdRegIndex = 1;
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

            CdResult[0] = *src;
            CdResult[1] = *src;
            *CdRegIndex = 1;
            *D_8006787C = 0x1F;
            if (!(CdResult[0] & 0x10)) {
                return -1;
            }
            return -2;
        }
        off = arg0 * 4;
        t = 0;
        if (D_8006783C[off + 2] != 0) {
            do {
                CdResult[t] = *D_80067878;
                t++;
            } while (t < D_8006783C[off + 2]);
        }
        *CdRegIndex = 1;
        *D_8006787C = 0x1F;
        if (arg0 == 10 || (t = CdResult[0], t &= 0x10, !t)) {
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
    *CdRegIndex = 1;
    *D_80067880 = 7;
    do {
        *(s32 *)0 = i;
        i++;
    } while (i < 4);
    *CdRegIndex = 1;
    *D_8006787C = 0x18;
    *CdRegIndex = 0;
    off = arg0 * 4;
    i = 0;
    if (D_8006783C[off + 1] != 0) {
        do {
            *D_8006787C = CdParams[i];
            i++;
        } while (i < D_8006783C[off + 1]);
    }
    *CdRegIndex = 0;
    *D_80067878 = D_8006783C[arg0 * 4];
}
