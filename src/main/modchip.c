#include "common.h"
#include "bash/modchip.h"
#include "bash/modchip_msg.h"

extern u8 D_BFC7FF52;
extern s32 ModchipState;
extern s32 MsgFontRom;

void modchip_check_region(void) {
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

s32 modchip_step(s32 arg0) {
    s32 ret;

    ret = 0;
loop:
    {
        switch (ModchipState) {
        case 1:
            modchip_cd_reset(1);
            ModchipState = 2;
            ret = 1;
            break;
        case 2:
            ret = modchip_cd_wait(1);
            switch (ret) {
            case 0:
            default:
                ret = 2;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = 2;
                break;
            case 1:
                ret = 2;
                ModchipCdStatus = CdResult[2];
                modchip_cd_reset(6);
                ModchipState = 3;
                break;
            case -1:
                modchip_cd_reset(1);
                ret = 2;
                break;
            }
            break;
        case 3:
            ret = modchip_cd_wait(6);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = (ModchipCdStatus >= 2) * 2;
                modchip_cd_reset(2);
                ModchipState = 4;
            case 0:
            default:
                ret =3;
                break;
            }
            break;
        case 4:
            ret = modchip_cd_wait(2);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
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
                modchip_cd_reset(0xC);
                ModchipState = 5;
            }
            case 0:
            default:
                ret =7;
                break;
            }
            break;
        case 5:
            ret = modchip_cd_wait(0xC);
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
                    modchip_cd_reset(3);
                    ModchipState = 7;
                    ret = 5;
                }
                break;
            }
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                modchip_cd_reset(0xD);
                ModchipState = 6;
            case 0:
            default:
                ret =5;
                break;
            }
            break;
        case 6:
            ret = modchip_cd_wait(0xD);
            switch (ret) {
            case -1:
                modchip_show_warning();
                break;
            case -2:
                modchip_cd_reset(0);
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
                modchip_cd_reset(3);
                ModchipState = 7;
            case 0:
            default:
                ret =6;
                break;
            }
            break;
        case 7:
            ret = modchip_cd_wait(3);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = ret;
                modchip_cd_reset(5);
                ModchipState = 8;
            case 0:
            default:
                ret =7;
                break;
            }
            break;
        case 8:
            ret = modchip_cd_wait(5);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
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
                modchip_cd_reset(4);
                ModchipState = 0xA;
            }
            ret = 9;
            break;
        case 10:
            ret = modchip_cd_wait(4);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                modchip_cd_reset(7);
                ModchipState = 0xB;
            case 0:
            default:
                ret =0xA;
                break;
            }
            break;
        case 11:
            ret = modchip_cd_wait(7);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                modchip_cd_reset(8);
                ModchipState = 0xC;
            case 0:
            default:
                ret =0xB;
                break;
            }
            break;
        case 12:
            ret = modchip_cd_wait(8);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                CdParams[0] = 4;
                modchip_cd_reset(9);
                ModchipState = 0xD;
            case 0:
            default:
                ret =0xC;
                break;
            }
            break;
        case 13:
            ret = modchip_cd_wait(9);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
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
                modchip_cd_reset(0xA);
                ModchipState = 0xF;
            }
            ret = 0xE;
            break;
        case 15:
            ret = modchip_cd_wait(0xA);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                if (CdResult[1] != 0) {
                    modchip_cd_reset(0);
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
            ret = modchip_cd_wait(0);
            switch (ret) {
            case -1:
                ModchipState = 1;
                break;
            case -2:
                modchip_cd_reset(0);
                ModchipState = 0x11;
                ret = -1;
                break;
            case 1:
                modchip_show_warning();
            case 0:
            default:
                ret =0x10;
                break;
            }
            break;
        case 17:
            ret = modchip_cd_wait(0);
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
                modchip_cd_reset(0);
                ret = 0x10;
                break;
            }
            break;
        case 18:
            ret = modchip_cd_wait(0xB);
            switch (ret) {
            case -2:
                modchip_cd_reset(0);
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
                modchip_cd_reset(0xB);
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

s32 modchip_cd_wait(s32 arg0) {
    s32 limit;
    s32 n;
    s32 t;
    s32 i;
    s32 off;

    limit = ModchipCdCmds[arg0 * 4 + 3];
    *CdRegIndex = 1;
    t = *CdIntFlagReg & 7;
    if (t != (*CdIntFlagReg & 7)) {
        return 0;
    }
    n = t;
    if ((u8)n != 0) {
        i = 0;
        ModchipWaitCount += (u8)n;
        *CdRegIndex = 1;
        *CdIntFlagReg = 7;
        do {
            *(s32 *)0 = i;
            i++;
        } while (i < 4);
        if (ModchipWaitCount < limit) {
            return 0;
        }
        ModchipWaitCount = 0;
        if (n == 5) {
            volatile u8 *src = CdCmdReg;

            CdResult[0] = *src;
            CdResult[1] = *src;
            *CdRegIndex = 1;
            *CdParamFifo = 0x1F;
            if (!(CdResult[0] & 0x10)) {
                return -1;
            }
            return -2;
        }
        off = arg0 * 4;
        t = 0;
        if (ModchipCdCmds[off + 2] != 0) {
            do {
                CdResult[t] = *CdCmdReg;
                t++;
            } while (t < ModchipCdCmds[off + 2]);
        }
        *CdRegIndex = 1;
        *CdParamFifo = 0x1F;
        if (arg0 == 10 || (t = CdResult[0], t &= 0x10, !t)) {
            return 1;
        }
        return -2;
    }
    return 0;
}

void modchip_cd_reset(s32 arg0) {
    s32 i;
    s32 off;

    i = 0;
    *CdRegIndex = 1;
    *CdIntFlagReg = 7;
    do {
        *(s32 *)0 = i;
        i++;
    } while (i < 4);
    *CdRegIndex = 1;
    *CdParamFifo = 0x18;
    *CdRegIndex = 0;
    off = arg0 * 4;
    i = 0;
    if (ModchipCdCmds[off + 1] != 0) {
        do {
            *CdParamFifo = CdParams[i];
            i++;
        } while (i < ModchipCdCmds[off + 1]);
    }
    *CdRegIndex = 0;
    *CdCmdReg = ModchipCdCmds[arg0 * 4];
}

/* data, in address order (tools/scripts/data2c.py) */
u8 ModchipCdCmds[56] = {
    1, 0, 1, 3, 0x13, 0, 3, 3, 0x14, 1, 3, 3, 2, 3, 1, 3, 0x16, 0, 1, 5, 0xE, 1, 1, 3, 0xA, 0,
    1, 5, 0xB, 0, 1, 3, 3, 0, 1, 3, 0x19, 1, 1, 3, 0x19, 1, 2, 3, 9, 0, 1, 5, 0x1E, 0, 1, 5,
    0x1A, 0, 1, 5,
};
volatile u8 *CdRegIndex = (void *)0x1F801800;
volatile u8 *CdCmdReg = (void *)0x1F801801;
volatile u8 *CdParamFifo = (void *)0x1F801802;
volatile u8 *CdIntFlagReg = (void *)0x1F801803;
u8 CdParams[8] = {0};
u8 CdResult[8] = {0};
s32 ModchipState = 1;
s32 ModchipWaitCount = 0;

/* the .bss */
s32 ModchipVSync __attribute__((section(".bss")));
s32 ModchipCdStatus __attribute__((section(".bss")));
volatile u8 CdTimeBcd __attribute__((section(".bss")));
