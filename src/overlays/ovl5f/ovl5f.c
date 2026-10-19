#include "common.h"
#include "bash/main.h"
#include "bash/ovl5f.h"
#include "bash/engine.h"
#include "bash/tank.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B3B24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B3C7C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B3F44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B4240);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B4490);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl5f/ovl5f", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B4688);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B4A28);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B4FAC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B5228);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B5588);

void func_800B56A0(void) {
    void **p;
    void *t;

    D_8009D8F4 = D_800C9EE8;
    D_8009D8F8 = D_800C9EFC;
    D_8009D904 = D_800C9ED0;
    p = &D_8009D8FC;
    if (D_8005A680[0] != 0) {
        t = D_800C9EB0;
    } else {
        t = D_800C9E9C;
    }
    *p = t;
    D_8009D900 = D_800C9EC4;
}

void func_800B5714(void) {
    void **p;
    void *t;

    D_8009D8F4 = D_800C9F5C;
    D_8009D8F8 = D_800C9F70;
    D_8009D904 = D_800C9F44;
    p = &D_8009D8FC;
    if (D_8005A680[0] != 0) {
        t = D_800C9F24;
    } else {
        t = D_800C9F10;
    }
    *p = t;
    D_8009D900 = D_800C9F38;
}

void func_800B5788(void) {
    void **p;
    void *t;

    D_8009D8F4 = D_800C9FD0;
    D_8009D8F8 = D_800C9FE4;
    D_8009D904 = D_800C9FB8;
    p = &D_8009D8FC;
    if (D_8005A680[0] != 0) {
        t = D_800C9F98;
    } else {
        t = D_800C9F84;
    }
    *p = t;
    D_8009D900 = D_800C9FAC;
}

void func_800B57FC(void) {
    void **p;
    void *t;
    Ovl5fGameView *g;

    D_8009D8F4 = D_800CA120;
    D_8009D8F8 = D_800CA134;
    D_8009D904 = D_800CA148;
    p = &D_8009D8FC;
    if (D_8005A680[0] != 0) {
        t = D_800CA08C;
    } else {
        t = D_800C9FF8;
    }
    *p = t;
    D_8009D900 = D_800C9FAC;
    Heroes[2].hero_name = 9;
    Heroes[3].hero_name = 9;
    g = (Ovl5fGameView *)D_8005A614;
    g->flags |= 0x100;
    if (g->w6C == 0) {
        Heroes[1].hero_name = 9;
        g->b51 = 9;
    }
    g->b52 = 9;
    g->b53 = 9;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B58B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B5FC4);

void func_800B62A4(void) {
    s32 i;
    s32 best;
    s32 bestHealth;
    s32 h;

    bestHealth = 0;
    best = -1;
    i = bestHealth;
    for (; i < 4; i++) {
        if (!(Heroes[i].hero_status & 0xFA00)) {
            h = Heroes[i].hero_health;
            if (bestHealth < h) {
                bestHealth = h;
                best = i;
            }
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            if (!(Heroes[i].hero_status & 0xFA00) && i != best) {
                D_800CAD50[i].RequestQuickFinish = 1;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6354);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6658);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6C24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6ED8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B6FF4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B71F8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B77E8);

void func_800B8EC4(s32 player) {
    s32 done;
    u8 unused[8];

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        EXTengine *eng;

        Heroes[player].hero_move->jump_max = 0;
        Heroes[player].hero_move->velocity_enable = 1;
        Heroes[player].hero_idle_count = Random(0x96) + 0x5A;
        SetHeroSeq((HeroInfo *)player, 0x59);
        eng = Heroes[player].hero_eng;
        if (D_800CAD50[player].HullSFX != 0) {
            D_800CAD50[player].HullSFX = 0;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
        }
        if (D_800CAD50[player].TurretSFX != 0) {
            D_800CAD50[player].TurretSFX = 0;
            StopSfx(D_800CAD50[player].TurretSFXHandler);
        }
        if (D_800CAD50[player].FireBreatherSFX != 0) {
            D_800CAD50[player].FireBreatherSFX = 0;
            StopSfx(D_800CAD50[player].FireBreatherSFXHandler);
        }
        func_800B6ED8(player);
    }
}

void func_800B906C(s32 player) {
    s32 done;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[player].hero_status & 0x200) && Heroes[player].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)player, 0x70);
        D_800CB0F0 = player + 1;
        done = 1;
    } else {
        done = 0;
    }
    if (done == 0) {
        SetHeroSeq((HeroInfo *)player, 0x59);
        PlaySfx(D_800C94E8[player], NULL, &Heroes[player].hero_eng->unk10, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B9170);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B9430);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B993C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800B9E20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BA95C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BAAB0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BABCC);

void func_800BACD4(s32 arg0) {
    EXTengine *eng;
    subengine *sub;

    eng = Heroes[arg0].hero_eng;
    sub = Heroes[arg0].hero_sub->next->next;
    eng->unk1E += 0x15;
    eng->unk5E = 1;
    if (HLV->mode != 5) {
        sub->unk52 = 1;
        sub->unk12 -= 0x15;
    }
    func_8008502C(arg0);
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        if (HLV->state & 0x800) {
            HLV->unk30 |= 0x201;
        } else {
            HLV->unk30 |= 1;
        }
    }
}

void func_800BADB8(s32 arg0) {
    s32 ok;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0 && func_8008502C(arg0) != 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = D_8005365C;
        nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0xE;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    nas->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        SetHeroAction((HeroInfo *)arg0, 0x6B);
    }
}

void func_800BAF64(s32 arg0) {
    subengine *s;

    Heroes[arg0].hero_hit = 0;
    Heroes[arg0].hero_immunity = 0;
    Heroes[arg0].hero_health = 0;
    Heroes[arg0].hero_status |= 0x6000;
    func_800898EC(arg0);
    s = Heroes[arg0].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BAFFC);

void PlotLaserBitTA(NAS *nas) {
    EXTengine *eng;
    LaserBitFux *fux;
    subengine *sub;
    s32 hit;
    s32 sfx;
    s32 i;
    s16 rot;

    eng = nas->Engine;
    fux = (LaserBitFux *)nas->FUX;
    sub = eng->sub;
    sub = sub->next;
    sub->unk51 = 0;
    sub->unk14 += 0x32;
    rot = -eng->unk1E;
    sub->unk10 = 0;
    sub->unk52 = 1;
    sub->unk12 = rot;
    hit = func_800BAFFC(nas, 0x14, 0, 0xA);
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4D8;
            break;
        case 1:
            sfx = 0x4D9;
            break;
        case 2:
            sfx = 0x4DA;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (hit != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E7, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotFlamerBitTA);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotFlubberTA);

void PlotSwirlyTA(NAS *nas) {
    EXTengine *eng;
    LaserBitFux *fux;
    subengine *sub;
    s32 sfx;
    s32 i;

    eng = nas->Engine;
    eng->unk5E = 1;
    eng->unk1C += 0x17C;
    eng->unk1E += 0x118;
    eng->unk20 += 0xB4;
    sub = eng->sub->next;
    sub->unk52 = 1;
    sub->unk10 += 0x46;
    sub->unk12 += 0x8C;
    sub->unk14 += 0xD2;
    fux = (LaserBitFux *)nas->FUX;
    if (func_800BAFFC(nas, 0x14, 0x1306, 0xD) != 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        PlaySfx(0x4E6, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        for (i = 0; i < 3; i++) {
            if (D_800CAD50[fux->player].MainWeaponNas[i] == nas) {
                D_800CAD50[fux->player].MainWeaponNas[i] = NULL;
                break;
            }
        }
        KillNAS(nas);
        return;
    }
    if (D_800CB0FC != 0) {
        switch ((u32)Random(3)) {
        case 0:
            sfx = 0x4DB;
            break;
        case 1:
            sfx = 0x4DC;
            break;
        case 2:
            sfx = 0x4DD;
            break;
        default:
            goto skip;
        }
        PlaySfx(sfx, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
    skip:;
    }
    if (fux->unk30 == 1) {
        fux->unk30 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotShell);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotLobber_TA);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotRocketTA);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotMine);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotWaterMine);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTorpedo);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800BD654);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotShrapnelBit);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPickupGenerator_TA);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", InitHelicopterTA);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotHelicopterTA);

void InitPickupTA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    PickupTAExt *ext;
    subengine *sub;
    u8 *p;
    s32 y;

    eng = nas->Engine;
    switch ((u32)Random(4)) {
    default:
    case 0:
        PlaySfx(0x4B0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x4B1, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x4B2, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x4B3, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        ext->unk14 = 0;
        nas->FUX = ext;
        p = D_8005365C;
        eng = nas->Engine;
        sub = eng->sub;
        sub->unk6C = p;
        sub->unk74 = 0x202B;
        eng->unk5D = 1;
        e = AddEngine(nas, 1);
        if (e != NULL) {
            e->unk10.x = eng->unk10.x;
            e->unk10.y = 0;
            e->unk10.z = eng->unk10.z;
            e->sub->unk51 = 2;
            e->sub->unk6C = p;
            e->sub->unk74 = 0x202A;
            e->sub->flags |= 0x40000041;
            e->sub->unk76 = 0x800;
            e->sub->unk50 = 1;
            y = eng->unk10.y + 0x1000;
            e->sub->unk20.x = y;
            e->sub->unk20.z = y;
            e->sub->unk52 = 1;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPickupTA);

static inline void SpawnPuff1605(SLONG_XYZ *pos, s32 kind) {
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;

    p = D_8005365C;
    nas = SpawnNASById(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = kind;
            ext->unk18 = 0x100;
            if (p != NULL) {
                ext->unk20 = 5;
                nas->Engine->sub->unk6C = p;
            } else {
                ext->unk20 = 1;
            }
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
    }
}

s32 func_800BF548(EXTengine *eng) {
    s32 i;
    s32 j;
    NAS *pickup;
    EXTengine *pe;
    NAS **pp;
    NAS **tbl;
    s32 x;
    s32 y;
    s32 z;

    x = eng->unk10.x;
    y = eng->unk10.y;
    z = eng->unk10.z;
    for (i = 0;; i++) {
        if (i >= 3) {
            return 0;
        }
        pickup = D_800CAD44[i];
        if (pickup == NULL) {
            continue;
        }
        pe = pickup->Engine;
        if ((u32)(pe->unk10.x - x + 0xFF) < 0x1FF
            && (u32)(pe->unk10.z - z + 0xFF) < 0x1FF
            && (u32)(pe->unk10.y - y + 0xFF) < 0x1FF) {
            SpawnPuff1605(&pe->unk10, 0x10);
            PlaySfx(0x4B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            for (j = 0; j < 3; j++) {
                if (D_800CAD44[j] == pickup) {
                    D_800CAD44[j] = NULL;
                    break;
                }
            }
            KillNAS(pickup);
            return 1;
        }
    }
}

void PlotRock_TA(NAS *nas) {
    EXTengine *e;

    if (nas->UserInfo.status == 1) {
        e = nas->Engine;
        nas->UserInfo.status = 0;
        {
            s32 z = e->unk10.z >> 9;
            s32 x = e->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            D_800CACB4[(z * D_800CAFF4) + x] = v;
            D_800CACFC[(z * D_800CAFF4) + x] = v;
        }
        {
            s32 z = nas->Engine->unk10.z >> 9;
            s32 x = nas->Engine->unk10.x >> 9;
            u8 v = ((x << 5) | 0x10) + ((z * 2) + 1);
            s32 i = (z * D_800CAFF4) + x;
            D_800CACB4[i] = v;
            D_800CACFC[i] = v;
            D_800CA34C[i] = (s32)nas;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPeriscope);

void PlotPeriscopeGun(NAS *nas) {
    EXTengine *e;
    EXTengine *se;
    NAS *shell;
    PeriscopeShellExt *ext;
    s32 dx;
    s32 dz;

    if (D_800CB0F0 != 0) {
        KillNAS(nas);
        return;
    }
    e = nas->Engine;
    e->sub->unk74++;
    switch (nas->Engine->sub->unk74) {
    case 0x4081:
        shell = SpawnNASById(e->unk10.x, e->unk10.y - 200, e->unk10.z, 0x130B, 0x11);
        if (shell == NULL) {
            return;
        }
        PlaySfx(0x4F3, &shell->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        ext = AddExtension(shell, 0x38, 0x10000000);
        if (ext == NULL) {
            KillNAS(shell);
            return;
        }
        shell->FUX = (EXT *)ext;
        se = shell->Engine;
        dx = -(D_80068BD4[(e->unk1E - 0x400) & 0xFFF].unk2 * 620);
        se->unk10.x += dx >> 12;
        dz = -(D_80068BD4[(u16)e->unk1E & 0xFFF].unk2 * 620);
        se->unk10.z += dz >> 12;
        ext->vy = -150;
        ext->unk2C = -1;
        ext->vx = dx >> 16;
        ext->vz = dz >> 16;
        ext->unk30 = 1;
        break;
    case 0x4087:
        KillNAS(nas);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotBoatField);

void PlotPillarControl(NAS *nas) {
    s32 *p;
    s32 delay;
    s32 x;
    s32 z;
    s32 v;
    NAS *obj;

    if (D_800CB0F0 == 0 && --nas->UserInfo.flags == 0) {
        PlaySfx(0x501, NULL, NULL, 0x1000, 0x1E00, 0);
        p = D_800CA8C4[D_800CB204++];
        delay = *p++;
        while ((v = *p++) >= 0) {
            x = (v << 9) + 0x100;
            z = (*p++ << 9) + 0x100;
            obj = SpawnNASById(x, 0, z, 0x1311, 0x11);
            if (obj != NULL) {
                obj->Engine->sub->unk74 = *p++;
                obj->UserInfo.user2 = delay;
                obj = SpawnNASById(x, 0, z, 0x1312, 0x11);
                if (obj != NULL) {
                    obj->Engine->sub->unk74 = 0x4000;
                    obj->UserInfo.user1 = 6;
                    obj->UserInfo.status = 0;
                }
            }
        }
        if (D_800CB204 < 0x20) {
            nas->UserInfo.flags = delay + 0x46;
        } else {
            KillNAS(nas);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotPillar);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTotemPole);

void func_800C0C68(NAS *nas, s32 id) {
    TotemExt *ext;
    EXTengine *eng;
    subengine *sub;
    s32 i;
    NAS **p;

    nas->UserInfo.user1 = 1;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        goto kill;
    }
    nas->FUX = (EXT *)ext;
    ext->unk30 = 1;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk1E = 0;
    sub->unk6C = D_800537AC;
    if (id == 0x131B) {
        sub->unk74 = 0x4180;
        ext->unk1C = 0;
    } else {
        sub->unk74 = 0x4180;
        ext->unk1C = 0xD;
    }
    ext->unk20 = D_800C9B94[ext->unk1C][0];
    ext->unk24 = *(s32 *)((u8 *)D_800C9B94 + ((ext->unk1C * 8) | 4));
    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == NULL) {
            D_800CB264[i] = nas;
            break;
        }
    }
    if (nas->Failure != 0) {
kill:
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C0DA0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C0F24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C18F0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C1F44);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", InitTankBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTankBoss);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C2940);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C30C0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTankBossCrashTurret);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C3778);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTankBossCortexTurret);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C3D48);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", PlotTankBossRocketTurret);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C4320);

void func_800C44E8(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        D_800CB24C = D_800CAA0C;
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        s32 val;
        D_800CB250 = D_800CAB58;
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C4548);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C49F8);

void func_800C4B60(s32 arg0) {
    if (arg0 == 3) {
        s32 val;
        D_800CB24C = D_800CAA0C;
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        s32 val;
        D_800CB250 = D_800CAB58;
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C4BC0);

void func_800C4D04(s32 hero, s32 last_mode) {
    subengine *s;

    Heroes[hero].hero_hit = 0;
    Heroes[hero].hero_immunity = 0;
    Heroes[hero].hero_health = 0;
    Heroes[hero].hero_status |= 0x6000;
    func_800898EC(hero);
    s = Heroes[hero].hero_eng->sub;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    s = s->next;
    s->flags &= 0xFFFF7FFF;
    if (hero == 3) {
        D_800CB254 = 0x26;
    } else {
        D_800CB258 = 0x26;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C4DCC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C53B0);

void func_800C5620(s32 arg0) {
    s32 val;
    EXTengine *eng;
    s32 *pos;
    u8 *p;
    NAS *nas;
    Ovl5fExt1605 *ext;
    s32 mode;

    if (arg0 == 3) {
        D_8009D678->unk74 = val = *D_800CB24C++;
    } else {
        D_8009D60C->unk74 = val = *D_800CB250++;
    }
    if (val == 0) {
        eng = Heroes[arg0].hero_eng;
        pos = (s32 *)&eng->unk10;
        p = D_8005365C;
        nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0xE;
                ext->unk18 = 0x100;
                if (p != NULL) {
                    ext->unk20 = 5;
                    nas->Engine->sub->unk6C = p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                nas->InitDelete(nas);
            }
        }
        PlaySfx(0x4F0, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        mode = Heroes[arg0].hero_mode;
        Heroes[arg0].hero_mode = 0x6B;
        Heroes[arg0].hero_last_mode = mode;
        func_800C4D04(arg0, mode);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C57B0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C5BCC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C60AC);

void func_800C60AC(NAS *nas, s32 id);

void PlotJoe(NAS *nas) {
    func_800C60AC(nas, 0x131F);
}

void func_800C4320(NAS *nas, s32 id);

void InitJoe(NAS *nas) {
    func_800C4320(nas, 0x131F);
}

void PlotMoe(NAS *nas) {
    func_800C60AC(nas, 0x131E);
}

void InitMoe(NAS *nas) {
    func_800C4320(nas, 0x131E);
}

void func_800C68A8(void) {
    func_80089BE8();
}

void func_800C68C8(s32 arg0) {
    u16 prev = Heroes[arg0].hero_mode;
    Heroes[arg0].hero_mode = 0x6E;
    Heroes[arg0].hero_last_mode = prev;
    func_800C4B60(arg0);
}

void func_800C6914(s32 hero, s32 mode) {
    u16 last = Heroes[hero].hero_mode;

    Heroes[hero].hero_mode = mode;
    Heroes[hero].hero_last_mode = last;
    switch (mode) {
    case 0x6C:
        func_800C49F8();
        break;
    case 0x6E:
        ((void (*)())func_800C4B60)();
        break;
    case 0x69:
        func_800C4BC0();
        break;
    case 0x6B:
        func_800C4D04(hero, mode);
        break;
    case 0x6A:
    case 0x6D:
        break;
    }
}

void func_800C69BC(void) {
}

void func_800C69C4(s32 arg0, s32 *arg1) {
    s32 val;

    if (arg0 == 3) {
        D_800CB24C = arg1;
    } else {
        D_800CB250 = arg1;
    }
    if (arg0 == 3) {
        val = *D_800CB24C++;
        D_8009D678->unk74 = val;
    } else {
        val = *D_800CB250++;
        D_8009D60C->unk74 = val;
    }
}

s32 func_800C6A30(s32 arg0) {
    s32 val;

    if (arg0 == 3) {
        D_8009D678->unk74 = val = *D_800CB24C++;
    } else {
        D_8009D60C->unk74 = val = *D_800CB250++;
    }
    return val;
}

void InitTankBossRocketTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    eng->unk10.z += 0x258;
    eng->unk10.y += 0xC8;
    sub->unk6C = D_800536CC;
    if (nas->UserInfo.flags == 0) {
        sub->unk74 = 0x2024;
    } else {
        sub->unk74 = 0x2025;
    }
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
    eng->flags = 0;
}

void InitTankBossCortexTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    sub->unk6C = D_800536CC;
    sub->unk74 = 0x2004;
    sub->unk68 = 0x100;
    sub->flags |= 0x02000000;
    nas->UserInfo.status = 0x11;
}

void InitTankBossCrashTurret(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 v;

    eng = nas->Engine;
    sub = eng->sub;
    eng->unk10.z += 0x100;
    v = 0x100;
    nas->UserInfo.flags = v;
    nas->UserInfo.user2 = 0x168;
    nas->UserInfo.status = 4;
    sub->unk6C = D_800536CC;
    sub->unk74 = 0x2003;
    sub->unk68 = v;
    sub->unk51 = 0;
    sub->flags |= 0x02000000;
}

s32 func_800C6B9C(s32 id, s32 x, s32 z, s32 angle) {
    s32 d;

    if (D_800CB0F0 == 0) {
        if (D_800CA9E4[id].z >= 0) {
            d = (angle - get_angle(x - D_800CA9E4[id].x, z - D_800CA9E4[id].z)) & 0xFFF;
            if (d < 0x15 || d > 0xFEB) {
                D_800CB260 = id;
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C6C30(void) {
    if (D_8005A680[0] != 0) {
        if (Heroes[0].hero_health == 0) {
            return 1;
        }
        if (Heroes[1].hero_health == 0) {
            return 0;
        }
        return Random(2);
    }
    return 0;
}

s32 func_800C6C90(OvlClampPos *p, s32 x, s32 y, s32 z) {
    s32 d;

    d = p->x - x;
    if (d > 0x250) {
        return 0;
    }
    if (d < -0x250) {
        return 0;
    }
    d = p->pad14 - y;
    if (d > 0x200) {
        return 0;
    }
    if (d < -0x200) {
        return 0;
    }
    d = p->z - z;
    if (d > 0x500) {
        return 0;
    }
    return d >= -0x500;
}

void PlotTotemBot(NAS *nas) {
    func_800C0F24(nas, 0x131C);
}

void InitTotemBot(NAS *nas) {
    func_800C0C68(nas, 0x131C);
}

void PlotTotemTop(NAS *nas) {
    func_800C0F24(nas, 0x131B);
}

void InitTotemTop(NAS *nas) {
    func_800C0C68(nas, 0x131B);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", InitTotemPole);

void PlotPillarCover(NAS *nas) {
    switch (nas->UserInfo.user1) {
    case 6:
        nas->UserInfo.status++;
        if (nas->UserInfo.status >= 4) {
            KillNAS(nas);
            return;
        }
        break;
    case 7:
        if (nas->UserInfo.status == 0) {
            KillNAS(nas);
            return;
        }
        nas->UserInfo.status--;
        break;
    }
    nas->Engine->sub->unk74 = D_800CA944[nas->UserInfo.status].v;
}

void InitPillarCover(NAS *nas) {
    EXTengine *e = nas->Engine;
    e->unk10.y = 0;
    e->sub->unk6C = *(void **)D_80058AD0;
}

void InitPillar(NAS *nas) {
    EXTengine *e = nas->Engine;
    subengine *s;
    nas->UserInfo.user1 = 1;
    nas->UserInfo.status = 6;
    e->unk10.y = 0xB8;
    s = e->sub;
    s->unk6C = *(void **)D_80058AD0;
    s->unk68 = 0x20;
    s->flags |= 0x02000000;
}

void InitPillarControl(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x5A;
}

void InitBoatField(NAS *nas) {
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBoatWake(NAS *nas) {
    EXTengine *src;
    EXTengine *dst;
    u16 t;

    src = Heroes[nas->UserInfo.user1].hero_eng;
    dst = nas->Engine;
    t = src->unk1E;
    dst->unk5E = 1;
    dst->unk1E = t;
    dst->unk10.x = src->unk10.x;
    dst->unk10.z = src->unk10.z;
    if (nas->UserInfo.status == 0) {
        nas->UserInfo.status = 0;
    }
    if (Heroes[nas->UserInfo.user1].hero_health <= 0) {
        KillNAS(nas);
    }
}

void InitBoatWake(NAS *nas) {
    subengine *sub;

    nas->UserInfo.status = 0;
    sub = nas->Engine->sub;
    sub->unk6C = D_80053774;
    sub->unk74 = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitPeriscopeGun(NAS *nas) {
    nas->Engine->sub->unk6C = *(void **)D_80058AD0;
    nas->Engine->sub->unk74 = 0x4080;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", InitPeriscope);

void InitRock_TA(NAS *nas) {
    MapTriggerInfo *info;
    EXT *ext;
    subengine *sub;
    u32 val;

    info = nas->trigger->InfoStruct;
    val = info->val1;
    nas->UserInfo.status = 1;
    nas->UserInfo.user2 = val;
    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = ext;
    sub = nas->Engine->sub;
    sub->unk6C = *(void **)D_80058AD0;
    sub->unk74 = D_800CA6D4[info->val1].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotBigApple(NAS *nas) {
}

void InitBigApple(NAS *nas) {
}

void func_800C7298(NAS *nas) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800CAD44[i] == nas) {
            D_800CAD44[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPickupGenerator_TA(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0xF0;
}

void PlotShellExplosion(NAS *nas) {
    subengine *sub = nas->Engine->sub;

    sub->unk74++;
    if ((u16)sub->unk74 >= 0x2022) {
        KillNAS(nas);
    }
}

void InitShellExplosion(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2019;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitShrapnelBit(NAS *nas) {
    subengine *sub = nas->Engine->sub;
    sub->unk6C = D_8005365C;
    sub->unk74 = D_800CA4AC[Random(3)].v;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotMineBall(NAS *nas) {
    if (++nas->UserInfo.status >= 8) {
        KillNAS(nas);
        return;
    }
    nas->Engine->sub->unk74 = D_800CA48C[nas->UserInfo.status];
}

void InitMineBall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    eng = nas->Engine;
    eng->flags |= 0x21;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2019;
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitTorpedo(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    nas->UserInfo.status = 0x5A;
    nas->UserInfo.flags = 0x11;
    nas->UserInfo.user2 = 0;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->unk6C = *(void **)D_80058AD0;
    sub->unk74 = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800CB0C0[i] == NULL) {
                D_800CB0C0[i] = nas;
                break;
            }
        }
    }
}

void InitWaterMine(NAS *nas) {
    EXTengine *e;
    subengine *s;
    nas->UserInfo.flags = 1;
    nas->UserInfo.user1 = -1;
    nas->UserInfo.status = D_8004E0D8;
    e = nas->Engine;
    nas->UserInfo.user2 = 0;
    e->unk10.y = -0x14;
    s = e->sub;
    s->unk6C = *(void **)D_80058AD0;
    s->unk74 = 0x2001;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitMine(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 i;

    eng = nas->Engine;
    eng->unk10.y = -0x14;
    sub = eng->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2005;
    nas->UserInfo.status = 0x78;
    nas->UserInfo.user1 = -1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800CB0C0[i] == NULL) {
                D_800CB0C0[i] = nas;
                break;
            }
        }
    }
}

void InitRocketTA(NAS *nas) {
    FlamerBitExt *ext;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    sub = nas->Engine->sub;
    sub->unk6C = D_80053694;
    sub->unk74 = 0x2007;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLobber_TA(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->unk6C = D_8005365C;
    eng->sub->unk74 = 0x200C;
    eng->unk5D = 1;
    e2 = AddEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->unk6C = D_8005365C - 0x700;
        e2->sub->unk74 = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitShell(NAS *nas) {
    EXTengine *eng;
    EXTengine *e2;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    eng->sub->unk6C = D_8005365C;
    eng->sub->unk74 = 0x200C;
    eng->unk5D = 1;
    e2 = AddEngine(nas, 1);
    if (e2 != NULL) {
        e2->unk10.x = eng->unk10.x;
        e2->unk10.y = 0;
        e2->unk10.z = eng->unk10.z;
        e2->sub->unk6C = D_8005365C - 0x700;
        e2->sub->unk74 = 0x2001;
        e2->sub->flags |= 0x40000041;
        e2->sub->unk76 = 0x800;
    }
}

void InitSwirlyTA(NAS *nas) {
    Ovl5fExt1605 *ext;
    EXTengine *eng;
    subengine *sub;
    void *p;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext != NULL) {
        nas->FUX = (EXT *)ext;
        p = D_8005365C;
        ext->unk28 = 0x87;
        eng = nas->Engine;
        sub = eng->sub;
        eng->unk5D = 0;
        sub->unk6C = p;
        sub->unk74 = 0x2038;
        sub->flags |= 0x20000000;
        eng->flags |= 0x21;
        if (func_80018F3C(eng, 2) == 2) {
            sub = sub->next;
            sub->unk6C = D_8005365C;
            sub->unk74 = 0x2030;
            sub = sub->next;
            sub->unk6C = D_8005365C;
            sub->unk74 = 0x2030;
        }
        if (nas->Failure == 0) {
            return;
        }
    }
    KillNAS(nas);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", InitFlubberTA);

void InitFlamerBitTA(NAS *nas) {
    FlamerBitExt *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    ext->timer = 0x3F;
    eng = nas->Engine;
    eng->unk5D = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->unk6C = D_8005365C;
    sub->unk74 = 0x202F;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitLaserBitTA(NAS *nas) {
    FlamerBitExt *ext;
    EXTengine *eng;
    subengine *sub;

    ext = AddExtension(nas, 0x38, 0x10000000);
    if (ext == NULL) {
        KillNAS(nas);
        return;
    }
    nas->FUX = &ext->ext;
    ext->timer = 0x5A;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk5D = 0;
    sub->unk6C = D_8005365C;
    sub->unk74 = 0x2021;
    eng->flags |= 0x21;
    if (func_80018F3C(eng, 1) != 0) {
        sub = sub->next;
        sub->unk6C = D_8005365C;
        sub->unk74 = 0x2023;
        eng->flags |= 0x21;
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800C7C08(u8 *arg0) {
    u32 temp_v0;

    temp_v0 = (*(s32*)((u8*)(arg0) + 16)) * 3;
    (*(s32*)((u8*)(arg0) + 16)) = (s32) ((s32) (temp_v0 + (temp_v0 >> 0x1F)) >> 1);
    (*(s32*)((u8*)(arg0) + 24)) = (s32) ((s32) ((*(s32*)((u8*)(arg0) + 24)) * 3) / 2);
    (*(s32*)((u8*)(arg0) + 40)) = (s32) (((*(s32*)((u8*)(arg0) + 40)) * 2) / 3);
}

void func_800C7C68(void) {
}

void func_800C7C70(s32 arg0) {
    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
    }
}

void func_800C7D04(void) {
    func_80089BE8();
}

void func_800C7D24(HeroInfo *hero) {
    SetHeroSeq(hero, 0x58);
}

s32 func_800C7D44(s32 flags, Ovl5fHolder *a, Ovl5fPos *b) {
    s32 dx = a->pos->x - b->x;
    s32 dz = a->pos->z - b->z;

    if (dx < -0x15D) {
        return flags;
    }
    if (dx >= 0x15E || dz < -0x15D || dz >= 0x15E) {
        return flags;
    }
    flags &= (dx < 0) ? -0x81 : -0x21;
    flags &= (dz > 0) ? -0x11 : -0x41;
    return flags;
}

void func_800C7DB8(s32 arg0) {
    s32 ok;

    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        ok = 1;
    } else {
        ok = 0;
    }
    if (ok == 0) {
        SetHeroSeq((HeroInfo *)arg0, 0x59);
    }
}

void func_800C7E68(HeroInfo *hero, Ovl5fActObj *obj, s32 arg2) {
    SetHeroAction(hero, 0x6F);
    obj->unk54 = -0x80;
    obj->unk50 = 8;
    obj->unk8 = arg2;
}

void func_800C7EB0(HeroInfo *hero, Ovl5fActState *state, s32 arg2) {
    SetHeroAction(hero, 0x6F);
    state->unk54 = 0x80;
    state->unk50 = 8;
    state->unk8 = arg2;
}

void func_800C7EF8(HeroInfo *hero) {
    SetHeroAction(hero, 0x6E);
}

void func_800C7F18(NAS *nas, s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800CAD50[player].MainWeaponNas[i] == nas) {
            D_800CAD50[player].MainWeaponNas[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void func_800C7F80(s32 player, s32 unused, s32 ReqdSFX) {
    if (D_800CAD50[player].FireBreatherSFX != ReqdSFX) {
        D_800CAD50[player].FireBreatherSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(D_800CAD50[player].FireBreatherSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &D_800CAD50[player].FireBreatherSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void func_800C8008(s32 player, s32 unused, s32 ReqdSFX) {
    if (D_800CAD50[player].TurretSFX != ReqdSFX) {
        D_800CAD50[player].TurretSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            StopSfx(D_800CAD50[player].TurretSFXHandler);
        } else {
            PlaySfx(ReqdSFX, &D_800CAD50[player].TurretSFXHandler, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void func_800C8090(s32 player, EXTengine *pEngine, s32 ReqdSFX) {
    if (D_800CAD50[player].HullSFX != ReqdSFX) {
        D_800CAD50[player].HullSFX = ReqdSFX;
        if (ReqdSFX == 0) {
            if (pEngine->sfx != NULL) {
                StopSfx(pEngine->sfx);
            }
        } else {
            PlaySfx(ReqdSFX, &pEngine->sfx, &pEngine->unk10, 0x1000, 0x1E00, 0);
        }
    }
}

s32 func_800C8120(s32 arg0) {
    if (D_8005A644 & 0x801) {
        D_800CB0F0 = -1;
    }
    if ((Heroes[arg0].hero_status & 0x200) && Heroes[arg0].hero_mode != 0x70) {
        SetHeroAction((HeroInfo *)arg0, 0x70);
        D_800CB0F0 = arg0 + 1;
        return 1;
    }
    return 0;
}

void func_800C81C0(void) {
}

void func_800C81C8(s32 x, s32 z, s32 range, s32 arg3) {
    HeroInfo *h;
    s32 d;
    s32 i;

    s32 neg;

    i = 0;
    neg = -range;
    for (h = Heroes; i < 4; i++, h++) {
        d = x - h->hero_eng->unk10.x;
        if (d >= neg && d <= range) {
            d = z - h->hero_eng->unk10.z;
            if (d >= neg && d <= range) {
                func_800B6354(i, arg3);
            }
        }
    }
}

s32 func_800C8294(OvlClampPos *p) {
    s32 r;
    s32 lim;

    r = 0;
    if (p->x > D_800CB100) {
        lim = D_800CB100;
        p->x = lim;
        r = 1;
    }
    if (p->x < 0x100) {
        p->x = 0x100;
        r = 1;
    }
    if (p->z > D_800CB104) {
        lim = D_800CB104;
        p->z = lim;
        r = 1;
    }
    if (p->z < 0x100) {
        p->z = 0x100;
        r = 1;
    }
    return r;
}

void func_800C831C(void) {
}

void func_800C8324(void) {
}

void func_800C832C(void) {
}

void func_800C8334(s32 arg0) {
    Heroes[arg0].hero_shadow->flags = 0;
}

void func_800C8360(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800CAD50[i].TurretSFXHandler != NULL) {
            StopSfx(D_800CAD50[i].TurretSFXHandler);
        }
        if (D_800CAD50[i].FireBreatherSFXHandler != NULL) {
            StopSfx(D_800CAD50[i].FireBreatherSFXHandler);
        }
    }
}

void func_800C83D0(void) {
}

void func_800C83D8(void) {
    D_8005AA70[0] = (s32)func_800B5588;
    D_8005AA70[1] = (s32)func_800B58B0;
    D_8005AA70[2] = (s32)func_800B58B0;
    D_8005AA70[3] = (s32)func_800B5FC4;
    D_8005AA70[4] = (s32)func_800C8360;
    D_8005AA70[6] = (s32)func_800C8324;
    D_8005AA70[5] = (s32)func_800C832C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B62A4;
    D_800CB25C = 1;
}

void func_800C8450(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = D_800CB0C0[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    nas->UserInfo.status = 1;
                }
            }
        }
    }
}

NAS *func_800C84D8(s32 x, s32 z, s32 dist) {
    EXTengine *eng;
    s32 d;
    s32 i;
    NAS *nas;

    for (i = 0; i < 8; i++) {
        nas = D_800CB264[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d >= -dist && d <= dist) {
                d = z - eng->unk10.z;
                if (d >= -dist && d <= dist) {
                    return nas;
                }
            }
        }
    }
    return NULL;
}


void func_800C8560(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == nas) {
            D_800CB264[i] = NULL;
            return;
        }
    }
}

void func_800C8598(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800CB264[i] == NULL) {
            D_800CB264[i] = nas;
            return;
        }
    }
}

void func_800C85D0(void) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        D_800CB264[i] = NULL;
    }
}

void func_800C85F8(Ovl5fHolder *h, s16 v) {
    Ovl5fLink *l;

    ((Ovl5fObj *)h->pos)->unk1E = v;
    ((Ovl5fObj *)h->pos)->unk5E = 1;
    l = ((Ovl5fObj *)h->pos)->list;
    if (l != NULL) {
        do {
            l->flags |= 0x20000000;
            l = l->next;
        } while (l != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C8650);

s32 func_800C872C(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 d;
    u16 name;
    EXTengine *e;

    for (i = 0; i < 4; i++) {
        name = Heroes[i].hero_name;
        if (name == 9 &&
            (((Ovl5fModeView *)D_8005A614)->mode != name || D_800CB210 != 0 ||
             (u32)(i - 2) >= 2)) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        e = Heroes[i].hero_eng;
        d = arg0 - e->unk10.x;
        if (d < -arg2) {
            continue;
        }
        if (arg2 < d) {
            continue;
        }
        d = arg1 - e->unk10.z;
        if (-arg2 < d && d < arg2) {
            return i;
        }
    }
    return -1;
}

void func_800C8800(s32 arg0, s32 arg1) {
    s32 idx;
    NAS *nas;

    idx = ((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9);
    nas = (NAS *)D_800CA34C[idx];
    D_800CA34C[idx] = 0;
    D_800CACB4[idx] = 0;
    D_800CACFC[idx] = 0;
    KillNAS(nas);
}

s32 func_800C8870(s32 arg0, s32 arg1) {
    return D_800CA34C[((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9)];
}

void func_800C88A8(NAS *nas) {
    s32 temp_a0;
    s32 temp_a1;
    s32 idx;
    u8 temp_a2;

    temp_a1 = nas->Engine->unk10.z >> 9;
    temp_a0 = nas->Engine->unk10.x >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    idx = (temp_a1 * D_800CAFF4) + temp_a0;
    D_800CACB4[idx] = temp_a2;
    D_800CACFC[idx] = temp_a2;
    D_800CA34C[idx] = (s32)nas;
}

u8 func_800C891C(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9) + D_800CACFC);
}

u8 func_800C8950(s32 arg0, s32 arg1) {
    return *(((arg1 >> 9) * D_800CAFF4) + (arg0 >> 9) + D_800CACB4);
}

void func_800C8984(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    D_800CACB4[(temp_a1 * D_800CAFF4) + temp_a0] = 0;
    D_800CACFC[(temp_a1 * D_800CAFF4) + temp_a0] = 0;
}

void func_800C89D8(s32 arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a1;
    u8 temp_a2;

    temp_a1 = arg1 >> 9;
    temp_a0 = arg0 >> 9;
    temp_a2 = ((temp_a0 << 5) | 0x10) + ((temp_a1 * 2) + 1);
    D_800CACB4[(temp_a1 * D_800CAFF4) + temp_a0] = temp_a2;
    D_800CACFC[(temp_a1 * D_800CAFF4) + temp_a0] = temp_a2;
}

void func_800C8A40(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x2C) + D_800CAFFC;
    (*(s32*)((u8*)(temp_v0) + 0)) = 0;
    (*(s32*)((u8*)(temp_v0) + 4)) = 0;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 12)) = 0;
    (*(s32*)((u8*)(temp_v0) + 16)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s16*)((u8*)(temp_v0) + 40)) = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl5f/ovl5f", func_800C8A80);

void func_800C8AF8(s32 player, EXTengine *eng, s32 *arg2) {
    s32 hero;
    EXTengine *target;

    if (D_8005A648 != 5) {
        hero = arg2[2];
        if (hero >= 0) {
            target = Heroes[hero].hero_eng;
            D_800CAD50[player].TurretReqdDir =
                (get_angle(target->unk10.x - eng->unk10.x, target->unk10.z - eng->unk10.z) + 0x800) & 0xFFF;
        }
    }
}

void func_800C8BA0(void) {
    switch ((s16)(D_8005A648 - 5)) {
    case 1:
    case 3:
        func_800B3C7C();
        break;
    case 0:
    case 2:
    case 4:
    default:
        func_800B3B24();
        break;
    }
}
