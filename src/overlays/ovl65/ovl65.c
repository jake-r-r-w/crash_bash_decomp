#include "common.h"
#include "bash/ovl65.h"
#include "bash/engine.h"
#include "bash/dino.h"
#include "bash/hero.h"
#include "bash/shell.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B33D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B3538);

s32 func_800B3760(s32 arg0) {
    s32 best = -1;
    s32 bestd = 0x7FFFFFFF;
    s32 start = Random(3);
    s32 i = start;
    s32 off;
    s32 idx;
    s32 dx;
    s32 dz;
    s32 d;
    Ovl65Ext *ext;
    EXTengine *a;
    EXTengine *b;

    for (;;) {
        if (i >= start + 3) {
            goto done;
        }
        off = 0;
        if (i >= 3) {
            off = 3;
        }
        ext = FindExtension(D_800BDA08[i - off], 0x10000000);
        if (ext != NULL) {
            s32 v = ext->unkC;
            if (v == 0 || v == 3 || v == 4) {
                idx = i - off;
                b = Heroes[arg0].hero_eng;
                a = D_800BDA08[idx]->Engine;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                d = dx * dx + dz * dz;
                if (d < bestd) {
                    bestd = d;
                    best = idx;
                }
            }
        }
        i++;
    }
done:
    return best;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B38BC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B3A38);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B3C1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B3D90);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B3FDC);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl65/ovl65", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B4268);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B4474);

s32 func_800B475C(s32 a, s32 b) {
    s32 v1[3];
    s32 v2[3];
    s16 d[8];
    EXTengine *ea;
    EXTengine *eb;
    s32 yaw;

    ea = Heroes[a].hero_eng;
    yaw = ea->unk1E;
    eb = Heroes[b].hero_eng;
    v1[0] = (D_800BDB50 * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2) >> 12;
    v1[2] = (D_800BDB50 * D_80068BD4[yaw & 0xFFF].unk2) >> 12;
    v2[0] = 0;
    v2[2] = 0;
    d[0] = *(u16 *)((u8 *)eb + 0x10) - *(u16 *)((u8 *)ea + 0x10);
    d[2] = *(u16 *)((u8 *)eb + 0x18) - *(u16 *)((u8 *)ea + 0x18);
    d[1] = 0;
    d[3] = 0xC8;
    return func_800B4268(v2, v1, d, ea) == 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B485C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B49AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B4B14);

s32 func_800B4C70(s32 *arg0, s32 arg1) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B4D6C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B5424);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B55E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B5DCC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B62D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B69F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B6FE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B714C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7300);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7664);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7AA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B85DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B86C8);

void PlotDinoSmoke(NAS *m) {
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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B8A40);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B9020);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B9534);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B96A0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B9778);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B9930);

void func_800B9C10(s32 id) {
    Ovl65HeroExt *ext;

    ext = FindExtension(Heroes[id].hero, 0x10000000);
    if (ext != NULL) {
        if (++ext->timer == 6) {
            if (id == 0) {
                PlaySfx(0x6A4, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 1) {
                PlaySfx(0x6A6, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 2) {
                PlaySfx(0x6A8, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 3) {
                PlaySfx(0x6AA, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (ext->timer == 0x13) {
            if (id == 0) {
                PlaySfx(0x6A5, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 1) {
                PlaySfx(0x6A7, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 2) {
                PlaySfx(0x6A9, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (id == 3) {
                PlaySfx(0x6AB, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (ext->timer >= 0x1A) {
            ext->timer = 0;
        }
        func_800BA960(id);
        func_800BC820(id);
        func_800BA810(id);
        func_8008502C(id);
        if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
            SetHeroAction((HeroInfo *)id, 0x68);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B9E68);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA468);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA574);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA724);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA810);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA960);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BAD18);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BAE94);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", InitDinoGem);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", PlotDinoGem);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", PlotDinoScratch);

void func_800BC5B0(void) {
    D_800BDA08[0] = D_800BD9FC;
    D_800BDA08[1] = D_800BDA00;
    D_800BDA08[2] = D_800BDA04;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", InitDinoScratch);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BC684);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BC774);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BC820);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BC878);

void func_800BC8D0(s32 arg0) {
    func_800BA960(arg0);
    func_8008502C(arg0);
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x68) {
        SetHeroAction((HeroInfo *)arg0, 0x68);
    }
}

void func_800BC948(HeroInfo *hero) {
    SetHeroSeq(hero, 0x4F);
    if ((s32)hero == 0) {
        PlaySfx(0x6B3, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((s32)hero == 1) {
        PlaySfx(0x6B4, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((s32)hero == 2) {
        PlaySfx(0x6B5, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if ((s32)hero == 3) {
        PlaySfx(0x6B6, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void func_800BCA18(HeroInfo *hero) {
    SetHeroSeq(hero, 0x4E);
}

void func_800BCA38(s32 arg0) {
    func_800BA960(arg0);
    func_800BC878(arg0);
    func_800BA810(arg0);
    func_8008502C(arg0);
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x68) {
        SetHeroAction((HeroInfo *)arg0, 0x68);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BCAC0);

void func_800BCB4C(s32 arg0) {
    SetHeroAction((HeroInfo *)arg0, 0x63);
    Heroes[arg0].hero_eng->unk1E = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
    Heroes[arg0].hero->Camera.height = 0x600;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BCBB8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BCCE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", InitDinoSmoke);

s32 func_800BCDF0(u8 *arg0) {
    s32 var_v1;

    var_v1 = 0;
    if ((*(s32*)((u8*)(arg0) + 16)) >= 0x801) {
        (*(s32*)((u8*)(arg0) + 16)) = 0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 16)) < -0x800) {
        (*(s32*)((u8*)(arg0) + 16)) = -0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 24)) >= 0x801) {
        (*(s32*)((u8*)(arg0) + 24)) = 0x800;
        var_v1 = 1;
    }
    if ((*(s32*)((u8*)(arg0) + 24)) < -0x800) {
        (*(s32*)((u8*)(arg0) + 24)) = -0x800;
        var_v1 = 1;
    }
    return var_v1;
}

void func_800BCE6C(void) {
}

void func_800BCE74(void) {
}

void func_800BCE7C(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x40) {
            func_800B69F4(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B55E8(arg0, (u16)arg1);
        } else if (f & 0x80) {
            func_800B55E8(arg0, (u16)arg1);
        } else {
            func_800B62D8(arg0, (u16)arg1);
        }
    }
}

void func_800BCEFC(void) {
    D_8005AA70[0] = (s32)func_800B6FE4;
    D_8005AA70[1] = (s32)func_800B7664;
    D_8005AA70[2] = (s32)func_800B7664;
    D_8005AA70[3] = (s32)func_800B85DC;
    D_8005AA70[5] = (s32)func_800BCE6C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}

void func_800BCF44(s32 arg0) {
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800BCF88(s32 arg0) {
    func_80089FA4(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x4D);
    Heroes[arg0].hero_eng->unk20 = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
    if (Heroes[arg0].hero_eng->sfx != NULL) {
        StopSfx(Heroes[arg0].hero_eng->sfx);
    }
    func_80085718(arg0, 0);
}

s32 func_800BD01C(s32 id) {
    if (Heroes[id].hero_status & 0x200) {
        if (Heroes[id].hero_mode != 0x68) {
            SetHeroAction((HeroInfo *)id, 0x68);
        }
        return 1;
    }
    return 0;
}

void func_800BD08C(void) {
    func_800B5424();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BD0AC);

s32 func_800BD1B8(s32 *arg0, s32 arg1) {
    s32 v = func_800B3760(arg1);

    if (v != -1) {
        arg0[10] = v;
        return 1;
    }
    return 0;
}

void func_800BD1FC(void) {
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BD204);

s32 func_800BD258(s32 arg0) {
    Ovl65Ext *ext;

    ext = FindExtension(Heroes[arg0].hero, 0x10000000);
    if (ext != NULL && ext->unk14 != 0) {
        return 1;
    }
    return 0;
}

s32 func_800BD2B8(s32 arg0) {
    Ovl65Ext *ext;

    ext = FindExtension(D_800BDA08[arg0], 0x10000000);
    if (ext != NULL) {
        s32 v = ext->unkC;
        if (v == 0 || v == 3 || v == 4) {
            return 1;
        }
        return 0;
    }
    return 0;
}

s32 func_800BD320(s32 arg0, s32 arg1) {
    s32 total;
    s32 result;
    s32 i;

    result = 0;
    total = 0;
    if (Heroes[arg1].hero_status & 0x2000) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0x2000)) {
            total += D_8009D87C[i].unk4 << 7;
        }
    }
    if (total != 0) {
        result = (D_8009D87C[arg1].unk4 << 19) / total;
    }
    return result;
}

s32 func_800BD408(s32 arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0 & 0x80) {
        var_a1 = 0xC00;
    }
    if (arg0 & 0x20) {
        var_a1 = 0x400;
    }
    if (arg0 & 0x10) {
        var_a1 = 0;
    }
    if (arg0 & 0x40) {
        var_a1 = 0x800;
    }
    if ((arg0 & 0x90) == 0x90) {
        var_a1 = 0xE00;
    }
    if ((arg0 & 0xC0) == 0xC0) {
        var_a1 = 0xA00;
    }
    if ((arg0 & 0x30) == 0x30) {
        var_a1 = 0x200;
    }
    if ((arg0 & 0x60) == 0x60) {
        var_a1 = 0x600;
    }
    return var_a1;
}

s32 func_800BD488(u8 *arg0, u8 *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0));
    temp_a0 = (*(s32*)((u8*)(arg1) + 0));
    var_v1 = (temp_a0 < temp_v0) << 7;
    if (temp_v0 < temp_a0) {
        var_v1 |= 0x20;
    }
    temp_a2 = (*(s32*)((u8*)(arg0) + 8));
    temp_a0_2 = (*(s32*)((u8*)(arg1) + 8));
    if (temp_a0_2 < temp_a2) {
        var_v1 |= 0x40;
    }
    if (temp_a2 < temp_a0_2) {
        var_v1 |= 0x10;
    }
    return var_v1;
}
