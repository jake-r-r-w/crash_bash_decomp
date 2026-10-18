#include "common.h"
#include "bash/main.h"
#include "bash/ovl67.h"
#include "bash/ovl67_800bbdb8.h"
#include "bash/n2o.h"

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl67/800bbdb8", D_800B336C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BBDB8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BC3A4);

void func_800BC6B8(void) {
    s32 temp_v0;

    temp_v0 = Random(6);
    if (temp_v0 == 0) {
        PlaySfx(0x2BC, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 1) {
        PlaySfx(0x2BD, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 2) {
        PlaySfx(0x2BE, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 3) {
        PlaySfx(0x2BF, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 4) {
        PlaySfx(0x2C0, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 5) {
        PlaySfx(0x2C1, 0, 0, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BC7E0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BC9B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BD584);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BD6E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BDB00);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BDC0C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BE70C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BE8C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BEB68);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BF0AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BF37C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BF7C8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitAsteroidRoll);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotAsteroidRoll);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroMine);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C0C28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitNitroBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitSpiralSat);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotSpiralSat);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotMineSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotLaserShoot);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2218);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C237C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C27D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitLaserShoot);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitMineSFX);

void func_800C2904(s32 *arg0, s32 flag, s32 *out) {
    s32 *p = arg0;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 *q;

    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz) + (flag << 11);
    if (flag != 0) {
        out[0] = get_angle(dy, dist);
        return;
    }
    out[0] = -get_angle(dy, dist);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitNitroMissile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitNitroMine);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2BB0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2C08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2C8C);

s32 func_800C2D10(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x32) {
            SetHeroAction((HeroInfo *)arg0, 0x32);
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2D80);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2DD8);

void func_800C2E58(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2E60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2EF8);

s32 func_800C2F70(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2F78);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotDummyCamera);

void InitDummyCamera(NAS *nas) {
    *(s32 *)((u8 *)nas->Engine + 0x14) = -0x480;
}

void func_800C3184(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C318C);

void func_800C3234(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C323C);

void func_800C3370(void) {
    s32 temp_v0;

    temp_v0 = Random(4);
    if (temp_v0 == 0) {
        PlaySfx(0x28B, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 1) {
        PlaySfx(0x28C, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 2) {
        PlaySfx(0x28D, 0, 0, 0x1000, 0x1E00, 0);
    }
    if (temp_v0 == 3) {
        PlaySfx(0x28E, 0, 0, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C3440);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C349C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C3544);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C365C);

void func_800C36D4(void) {
}

void func_800C36DC(void) {
}

void PlotNitroSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitNitroSmokeBoost);
