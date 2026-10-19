#include "common.h"
#include "bash/ovl5d.h"
#include "bash/ovl5d_800bff1c.h"
#include "bash/bash.h"
#include "bash/xtras.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800BFF1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C013C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C02D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C05A8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C07F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C09F4);

void func_800C0BAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *pu;
    s32 y;

    if (func_8008ABF8(arg0) == 0) {
        base = Heroes;
        h = base + arg0;
        pu = h->hero_powerup;
        if ((u16)h->hero_frozen >= 3) {
            y = pu->unk10.y;
            if (y < D_8005A5D8) {
                pu->unk10.y = y + 0x100;
                if (y + 0x100 >= D_8005A5D8) {
                    pu->unk10.y = D_8005A5D8;
                    if (h->hero_powerup->sfx != NULL) {
                        StopSfx(h->hero_powerup->sfx);
                    }
                    if (D_800C4780 != NULL) {
                        StopSfx(D_800C4780);
                    }
                    PlaySfx(0x455, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                Heroes[arg0].hero_frozen = 0x1E;
            }
            if (abs(D_8005A5D8 - pu->unk10.y) < 0x100) {
                Heroes[arg0].hero_eng->unk2C.y = 0x100;
                Heroes[arg0].hero_eng->unk10.y = D_8005A5D8;
            }
        } else {
            SetHeroAction((HeroInfo *)arg0, (u16)D_8009E0E2);
            if (h->hero_powerup->sfx != NULL) {
                StopSfx(h->hero_powerup->sfx);
            }
            if (D_800C4780 != NULL) {
                StopSfx(D_800C4780);
            }
            pu->flags &= 0xFFFF7FFF;
            h->hero_eng->unk2C.y = 0x1000;
        }
        pu->unk10.x = Heroes[arg0].hero_eng->unk10.x;
        pu->unk10.z = Heroes[arg0].hero_eng->unk10.z;
        Heroes[arg0].hero_eng->unk5E = 1;
        func_8008502C(arg0);
    }
}

void func_800C0DE4(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base2;

    if (Heroes[arg0].hero_pu == 0x14) {
        if (D_800C4780 != NULL) {
            StopSfx(D_800C4780);
        }
    }
    base2 = Heroes;
    h = base2 + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 2;
    h->hero_health = 0;
    h->hero_status |= 0x2000;
    h->hero_powerup->flags = 0;
    h->hero_fx->flags = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    func_800898EC(arg0);
}

void func_800C0F14(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    if (Heroes[arg0].hero_pu == 0x14 && D_800C4780 != NULL) {
        StopSfx(D_800C4780);
    }
    base = Heroes;
    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_sub->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    func_80085718(arg0, 0);
    SetHeroSeq((HeroInfo *)arg0, 0xC);
    func_80089FA4(arg0);
}

void func_800C1190(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800C1278(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h2;
    s32 dy;

    if (Heroes[arg0].hero_pu == 0x14 && D_800C4780 != NULL) {
        StopSfx(D_800C4780);
    }
    base = Heroes;
    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_move->z_vel = 0;
    h->hero_move->x_vel = 0;
    h->hero_move->z_max = 0;
    h->hero_move->x_max = 0;
    h->hero_frozen = 8;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_object != NULL) {
        h->hero_object->player = h->hero_harm_player;
        h->hero_object->status = 4;
        h->hero_object = NULL;
    }
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    base2 = Heroes;
    h2 = base2 + arg0;
    h2->hero_sub->unk08 = 0;
    h2->hero_pu = 0;
    h2->hero_pu_timer = 0;
    SetHeroSeq((HeroInfo *)arg0, 1);
    func_80026980(h2->hero);
}

void func_800C14D4(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = D_8005A5D8 - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = D_8005A5D8;
    }
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C159C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1848);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C1A30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5d/800bff1c", func_800C284C);

void *func_800C2B60(s32 idx, s32 mask) {
    NAS *n;
    EXTengine *he;
    EXTengine *e;
    EXT *fux;
    s32 hx, hz, dx, dz, lim, m;

    n = D_8005AA24;
    m = mask;
    lim = 0x10000;
    he = Heroes[idx].hero_eng;
    hx = he->unk10.x - (D_80068BD4[(he->unk1E - 0x400) & 0xFFF].unk2 * 1024 >> 14);
    hz = he->unk10.z - (D_80068BD4[he->unk1E & 0xFFF].unk2 * 1024 >> 14);
    for (; n != NULL; n = n->Object.Child) {
        fux = n->FUX;
        if (fux == NULL) continue;
        if (!(*(u16 *)((u8 *)fux + 0x1C) & m)) continue;
        e = n->Engine;
        if (e == NULL) continue;
        dx = e->unk10.x - hx;
        dz = e->unk10.z - hz;
        if (dx * dx + dz * dz < lim) return fux;
    }
    return 0;
}

void func_800C2C70(s32 *a, s32 *b, s16 ma, s32 *c, s32 *d, s16 mb) {
    s32 total;
    s32 r;
    s32 q;
    s32 dd;
    s32 x;
    s32 y;

    if (ma > 0 && mb > 0) {
        x = *a;
        y = *c;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / (total = mb + ma);
        r = q - dd;
        *a = q >> 8;
        *c = r >> 8;
        x = *b;
        y = *d;
        dd = -((x - y) << 8);
        q = (dd * mb + ((ma * x + mb * y) << 8)) / total;
        r = q - dd;
        *b = q >> 8;
        *d = r >> 8;
    }
}

void func_800C2DAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    if (func_8008ABF8(arg0) != 0) {
        base = Heroes;
        h = base + arg0;
        if ((u16)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = D_80052A54 + D_8009D6EC[h->hero_name] * 0x38;
        } else {
            h->hero_sub->unk6C = NULL;
        }
    } else {
        func_80089BE8(arg0);
    }
}

void func_800C2E58(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0x12);
    if (h->hero_object != NULL) {
        h->hero_object->can_do |= 0x20;
    }
    h->hero_weapon_timer = 0;
}

void func_800C2EC8(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x11);
}

void func_800C2F14(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x10);
}

void func_800C2F60(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;

    h->hero_move->velocity_enable = 0;
    SetHeroSeq((HeroInfo *)arg0, 0xF);
    h->hero_weapon_timer = 0;
}

s32 func_800C2FB0(s32 arg0) {
    u32 m;

    if (D_8005A614[arg0 + 0x5C] != 0) {
        return 0;
    }
    m = D_80051338[arg0].unk54 & 0x2000;
    return m != 0;
}
