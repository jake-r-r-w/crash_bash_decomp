#include "common.h"
#include "bash/main.h"
#include "bash/ovl60.h"
#include "bash/pogo.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3534);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3808);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3A38);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3C44);

void func_800B3E34(void) {
    Ovl60Slot *slot;
    s32 i;
    s32 ok;
    u8 *g;

    func_800C2EE0();
    func_800B4230();
    func_800B87DC();
    if (D_8005A644 & 0x801) {
        return;
    }
    i = 0;
    slot = D_800C535C;
loop:
    if (slot->unk4 == 5) {
        goto tick;
    }
    if (slot->unk4 == 6) {
    tick:
        if (--slot->unk6 < 2) {
            func_800C1CF0(i);
        }
    }
    i++;
    slot++;
    if (i < 4) {
        goto loop;
    }
    func_800B9C10();
    func_800B9D64();
    g = D_8005A614;
    switch ((Ovl60Mode)*(s16 *)(g + 0x34)) {
    case OVL60_MODE_12:
        break;
    case OVL60_MODE_10:
        if (*(u16 *)(g + 0x32) & 0x80) {
            switch ((u32)D_80099178 >> 8) {
            case 0x115:
                ok = D_800C52F4 < 3;
                break;
            case 0x50:
                ok = D_800C52F4 < 4;
                break;
            case 0x30:
                ok = D_800C52F4 < 5;
                break;
            case 0x20:
                ok = D_800C52F4 < 6;
                break;
            case 0x10:
                ok = D_800C52F4 < 7;
                break;
            default:
                return;
            }
            if (ok) {
                SpawnNASById(0, 0, 0, 0x1407, 0x11);
            }
        }
        break;
    case OVL60_MODE_11:
        func_800BCB4C(g);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B3FF0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4230);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4374);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B44F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4A70);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B4E50);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B53CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B574C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5B1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5D90);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl60/ovl60", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B5EE4);

void func_800B6370(s32 arg0) {
    s32 seq;

    func_800C2810(arg0);
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    Heroes[arg0].hero_move->jump_ctrl |= 0x22;
    Heroes[arg0].hero_move->jump_ctrl &= 0xEF;
    func_800C159C(arg0);
    if (D_800C535C[arg0].unk4 == 5) {
        SetHeroSeq((HeroInfo *)arg0, 0x2F);
    } else {
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6438);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B662C);

void func_800B6790(s32 arg0) {
    EXTengine *eng;
    u8 *g;

    Heroes[arg0].hero_frozen = 2;
    func_800C1CF0();
    if (func_8008A200(arg0) != 0) {
        g = D_8005A614;
        if (*(s32 *)(g + 4) & 0x800) {
            *(u16 *)(g + 0x30) |= 0x201;
        } else {
            *(u16 *)(g + 0x30) |= 1;
        }
    }
    switch (func_8008502C(arg0)) {
    case 0x12:
        func_800B8AF8(arg0);
        func_800B8BE4(arg0, 1);
        break;
    case 0x11:
        func_800B72B0(arg0);
        break;
    case 0x14:
        func_800B741C(arg0);
        break;
    case 0x13:
        func_800B7574(arg0);
        break;
    }
    switch (Heroes[arg0].hero_name) {
    case 4:
        D_800C52E0 = Random(0x10);
        break;
    case 3:
        D_800C52E4 = Random(0x10);
        break;
    }
    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        s32 r = func_80015480(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6960);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6AE0);

s32 func_800B6C14(s32 arg0) {
    HeroInfo *h;
    s32 x;
    s32 z;
    Ovl60Cell *c;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    if (!(h->hero_status & 0x800) && !(D_800C535C[arg0].flags & 1)) {
        return 0;
    }
    x = (h->hero_eng->unk10.x + 0x800) >> 9;
    z = (h->hero_eng->unk10.z + 0x800) >> 9;
    if (h->hero_mode != 0x3B && h->hero_last_mode != 0x3B) {
        switch (h->hero_name) {
        case 1:
            PlaySfx(3, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 3:
            PlaySfx(0x67, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 2:
            PlaySfx(0x35, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 5:
            PlaySfx(0x12F, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 6:
            PlaySfx(0x161, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 7:
            PlaySfx(0xFD, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 4:
            PlaySfx(0x99, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        case 8:
            PlaySfx(0xCB, NULL, &Heroes[arg0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            break;
        }
    }
    SetHeroAction((HeroInfo *)arg0, 0x3B);
    if ((u32)x < 8 && z >= 0 && z < 8) {
        Ovl60Cell *cb = D_800C53DC[0];
        c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
        c->unkE = arg0 + 1;
        c->unk10 = arg0 + 1;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6DF4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B6F34);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B72B0);

void func_800B741C(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void func_800B7574(s32 arg0) {
    s32 *pos;
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    subengine *s;
    u8 *p;

    pos = (s32 *)&Heroes[arg0].hero_eng->unk10;
    p = D_80053B9C;
    nas = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = 4;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)nas->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            nas->InitDelete(nas);
        }
        r = nas;
    } else {
        r = NULL;
    }
    if (r != NULL) {
        eng = r->Engine;
        if (eng != NULL) {
            u32 a = (u32)(Random(360) << 12) / 360;
            eng->unk5E = 1;
            eng->unk1E = a;
            for (s = eng->sub; s != NULL; s = s->next) {
                s->flags |= 0x20000000;
            }
        }
    }
}

void func_800B76CC(s32 arg0) {
    VECTOR pos;
    HeroInfo *h;
    EXTengine *pu;
    subengine *sub;
    s32 state;
    s32 dy;
    HeroInfo *base = Heroes;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    pu = h->hero_powerup;
    if (pu != NULL) {
        sub = pu->sub;
        if (sub != NULL) {
            state = (u16)D_800C535C[arg0].unk4;
            if (state >= 7) {
                goto clear;
            }
            if (state != 0) {
                if (func_80015780(h->hero_eng, h->hero_sub, 0x80, &pos) != NULL) {
                    pu->unk10 = *(SLONG_XYZ *)&pos;
                    dy = D_800C4484[h->hero_name];
                } else {
                    dy = 0x280;
                    pu->unk10 = h->hero_eng->unk10;
                }
                pu->unk5E = 1;
                pu->unk10.y -= dy;
                pu->unk1E += 0x71;
                if ((u16)D_800C535C[arg0].unk4 != 6 || D_800C535C[arg0].unk6 >= 0x5B ||
                    !((D_8004E0D8 >> 2) & 1)) {
                    sub->flags |= 0x8000;
                    pu->flags |= 0x8000;
                    return;
                }
            }
        clear:
            sub->flags &= ~0x8000;
            pu->flags &= ~0x8000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B788C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B7990);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B7B1C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B7C3C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8180);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B83AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8660);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B87DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8AF8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8BE4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8DFC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8EC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B8F94);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B9080);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B91E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B935C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B9C10);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800B9D64);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA018);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA110);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA25C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA3E0);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BA51C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoTazer);

void Plot_PogoPickup(NAS *nas) {
    EXTengine *eng;
    s32 dir;
    s32 scale;
    s32 d;
    s32 state;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    state = nas->UserInfo.flags;
    if (state >= 9) {
        return;
    }
    if (state < 7) {
        return;
    }
    dir = nas->UserInfo.user1 & 3;
    switch (nas->UserInfo.status) {
    case 0:
    case 1:
        PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        eng->unk2C.x = 0x100;
        eng->unk2C.y = 0x100;
        eng->unk2C.z = 0x100;
        eng->flags |= 0x21;
        eng->sub->unk76 = 0x100;
        nas->UserInfo.status = 2;
        eng->unk1E = dir << 10;
        eng->unk5E = 1;
        break;
    case 2:
        scale = eng->unk2C.x;
        scale += 0x100;
        if (scale >= 0x1000) {
            nas->UserInfo.status = 3;
            scale = 0x1000;
            nas->UserInfo.user2 = D_800C38F8[D_800C5BB8];
            eng->flags &= ~0x21;
        }
        eng->unk2C.x = scale;
        eng->unk2C.y = scale;
        eng->unk2C.z = scale;
        eng->sub->unk76 = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        d = func_80015480(dir << 10, eng->unk1E) >> 3;
        eng->unk5E = 1;
        eng->unk1E += d;
        if (d < 2) {
            nas->UserInfo.user1 = (nas->UserInfo.user1 + 1) & 3;
        }
        break;
    case 6:
        KillNAS(nas);
        D_800C52EC--;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Init_PogoPowerup);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoPowerup);

void Plot_PogoBanker(NAS *nas) {
    s32 x;
    s32 z;
    EXTengine *eng;
    Ovl60CellPos *pos;
    Ovl60Cell *c;
    s32 scale;

    eng = nas->Engine;
    if (eng == NULL) {
        return;
    }
    switch (nas->UserInfo.status) {
    case 0:
        if (--nas->UserInfo.user1 <= 0) {
            nas->UserInfo.status = 1;
        }
        break;
    case 1:
        if (func_800C165C(&x, &z) != 0) {
            Ovl60Cell *cb = D_800C53DC[0];
            c = (Ovl60Cell *)(z * sizeof(Ovl60Cell) + x * sizeof(D_800C53DC[0]) + (u32)cb);
            pos = c->unk4;
            c->hero = (HeroInfo *)nas;
            if (pos != NULL) {
                eng->unk10.x = pos->x;
                eng->unk10.y = D_8005A5D8;
                eng->unk10.z = pos->z;
                eng->unk2C.x = 0;
                eng->unk2C.y = 0;
                eng->unk2C.z = 0;
                eng->flags |= 0x8021;
                eng->unk5E = 1;
                nas->UserInfo.status = 2;
                nas->UserInfo.user2 = D_800C38F8[D_800C5BB8];
                PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        scale = eng->unk2C.x + 0x100;
        if (scale >= 0x1000) {
            scale = 0x1000;
            eng->flags &= ~0x21;
            nas->UserInfo.status = 3;
        }
        eng->unk2C.z = scale;
        eng->unk2C.y = scale;
        eng->unk2C.x = scale;
        eng->unk5E = 1;
        break;
    case 3:
        if (nas->UserInfo.user2 != 0) {
            nas->UserInfo.user2--;
        }
        break;
    case 4:
        eng->flags &= ~0x8000;
        nas->UserInfo.status = 0;
        nas->UserInfo.user1 = Random(0x28) + 0x28;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoMushroom);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BB724);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BB840);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BBB08);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BBC94);

s32 func_800BBF00(s32 *arg0, s32 *arg1) {
    s32 x;
    s32 y;
    s32 i;
    s32 found;
    s32 ok;

    i = 0;
loop:
    x = Random(8);
    y = Random(8);
    found = func_800BC04C(x, y) ^ 1;
    i++;
    if (found && i < 10) {
        goto loop;
    }
    if (i >= 10) {
        found = 0;
    } else {
        *arg0 = x;
        *arg1 = y;
        found = 1;
    }
    if (!found) {
        return 0;
    }
    ok = 1;
    if (*arg0 > 0) {
        ok = func_800C1608(*arg0 - 1, *arg1) == 0;
    }
    if (*arg0 < 7 && func_800C1608(*arg0 + 1, *arg1)) {
        ok = 0;
    }
    if (*arg1 > 0 && func_800C1608(*arg0, *arg1 - 1)) {
        ok = 0;
    }
    if (*arg1 < 7 && func_800C1608(*arg0, *arg1 + 1)) {
        ok = 0;
    }
    return ok;
}

s32 func_800BC04C(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    s32 s;

    if (p->unkE != 0 || p->unk10 != 0 || p->hero != NULL) {
        return 0;
    }
    s = (u16)p->unk12;
    if (s == 5) {
        return 0;
    }
    if (s == 6) {
        return 0;
    }
    return func_800C0AFC(arg0, arg1) == 0;
}

void func_800BC0EC(s32 arg0) {
    s32 x = Heroes[arg0].hero_eng->unk10.x;
    s32 tx = (((x + 0x800) >> 9) << 9) - 0x700;
    s32 tz = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;

    Heroes[arg0].hero_eng->unk10.x = x - (x - tx) / 4;
    Heroes[arg0].hero_eng->unk10.z -= (Heroes[arg0].hero_eng->unk10.z - tz) / 4;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BC184);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BC314);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BC490);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoProjectile);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BCB4C);

void func_800BD008(s32 arg0) {
    D_800C5BD0 = arg0;
    switch (arg0) {
    case 1:
        D_800C5300 = 0;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 8;
        break;
    case 2:
        D_800C5300 = 0;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 0x10;
        break;
    case 3:
        D_800C5300 = 1;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 8;
        break;
    case 4:
        D_800C5300 = 1;
        D_800C5BCC = 8;
        D_800C5BC4 = 8;
        D_800C5BC8 = 0x10;
        break;
    case 6:
        D_800C5BCC = 0x18;
        D_800C5BC4 = 0x18;
        D_800C5300 = D_8004E0D8 & 1;
        D_800C5BC8 = (Random(2) + 2) * 8;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BD10C);

s32 func_800BD378(s32 hero) {
    u8 *g;
    HeroInfo *p;
    EXTengine *eng;
    s32 mate;
    u32 i;
    s32 x, z, ox, oz;
    s32 dir;
    s32 hit;

    if (D_8005A680[0] != 0) {
        mate = Heroes[hero].hero_teammate;
    } else {
        mate = 1000;
    }
    i = 0;
    eng = Heroes[hero].hero_eng;
    g = D_8005A614;
    x = (eng->unk10.x + 0x800) >> 9;
    z = (eng->unk10.z + 0x800) >> 9;
    dir = Heroes[hero].hero_move->facing_angle >> 10;
    p = Heroes;
loop:
    {
        hit = 0;
        if (i == hero || i == mate) {
            goto next;
        }
        ox = (p->hero_eng->unk10.x + 0x800) >> 9;
        oz = (p->hero_eng->unk10.z + 0x800) >> 9;
        switch (dir) {
        case 0:
            if (oz < z && x == ox) {
                hit = 1;
            }
            break;
        case 1:
            if (ox < x && z == oz) {
                hit = 1;
            }
            break;
        case 2:
            if (z < oz && x == ox) {
                hit = 1;
            }
            break;
        case 3:
            if (x < ox && z == oz) {
                hit = 1;
            }
            break;
        }
        if (hit) {
            if ((*(u16 *)(g + 0x32) & 0x20) && g[i + 0x5C] != 0) {
                return 0;
            }
            return 1;
        }
    }
next:
    i++;
    p++;
    if (i < 4) {
        goto loop;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BD520);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BD630);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BD790);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BDCB8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BDE24);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BDF60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BE158);

s32 func_800BE6C4(s32 arg0, Ovl60Goal *arg1) {
    Ovl60Move *m2;
    EXTengine *eng;
    Ovl60Move *m;
    Ovl60Move *base;
    Ovl60Move *base2;
    s32 x;
    s32 z;
    s32 odd;
    s32 dir;

    if ((u16)D_8005A648 - 0xB >= 2U) {
        return 0;
    }
    if (func_800C0DE4(arg0) < 9) {
        return 0;
    }
    {
        eng = Heroes[arg0].hero_eng;
        if (eng != NULL) {
            base = D_800C5ADC;
            m = (Ovl60Move *)(arg0 * sizeof(Ovl60Move) + (u32)base);
            x = (eng->unk10.x + 0x800) >> 9;
            z = (eng->unk10.z + 0x800) >> 9;
            odd = D_8004E0D8 & 1;
            m->unkC = 0;
            m->unk10 = 8;
            m->unk0 = odd;
            if (x & 4) {
                if (z & 4) {
                    dir = odd * 2;
                } else if (odd != 0) {
                    dir = 4;
                } else {
                    dir = 6;
                }
            } else if (z & 4) {
                dir = (odd == 0) * 2;
            } else if (odd != 0) {
                dir = 6;
            } else {
                dir = 4;
            }
            m->unk8 = dir;
            base2 = D_800C5ADC;
            m2 = (Ovl60Move *)(arg0 * sizeof(Ovl60Move) + (u32)base2);
            if (m2->unk0 != 0) {
                arg1->unk14 = x + D_800C457C[m2->unk8].x;
                arg1->unk1C = z + D_800C457C[m2->unk8].z;
            } else {
                arg1->unk14 = x + D_800C45BC[m2->unk8].x;
                arg1->unk1C = z + D_800C45BC[m2->unk8].z;
            }
            arg1->unk20 = 5;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BE864);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BEA34);

s32 func_800BEB6C(s32 arg0, s32 arg1, s32 arg2) {
    s32 n;

    n = 0;
    if (arg0 > 0) {
        s32 v = D_800C53DC[arg0 - 1][arg1].unkA;
        n = v == arg2;
    }
    if (arg0 < 7 && D_800C53DC[arg0 + 1][arg1].unkA == arg2) {
        n++;
    }
    if (arg1 > 0 && D_800C53DC[arg0][arg1 - 1].unkA == arg2) {
        n++;
    }
    if (arg1 < 7 && D_800C53DC[arg0][arg1 + 1].unkA == arg2) {
        n++;
    }
    if (arg0 == 0 && D_800C5B2C[arg1] == arg2) {
        n++;
    }
    if (arg0 == 7 && D_800C5B4C[arg1] == arg2) {
        n++;
    }
    if (arg1 == 0 && D_800C5B8C[arg0] == arg2) {
        n++;
    }
    if (arg1 == 7 && D_800C5B6C[arg0] == arg2) {
        n++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Init_PogoRipperRoo);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoRipperRoo);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_UpdatePosition);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_ChangeDirOnEdge);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_FindNewSquare);

void Roo_GoBoing(EXTengine *eng) {
    switch (Random(6)) {
    case 0:
        PlaySfx(0x53C, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 1:
        PlaySfx(0x53D, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 2:
        PlaySfx(0x53E, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 3:
        PlaySfx(0x53F, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 4:
        PlaySfx(0x540, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    case 5:
        PlaySfx(0x541, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800BFFF4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_ShootAtHumans);

void func_800C02AC(EXTengine *eng, s32 angle) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    s32 x = eng->unk10.x;
    s32 z = eng->unk10.z;

    nas = SpawnNASById(x, D_8005A5D8 - 0x280, z, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user2 = 1;
                nas->UserInfo.flags = 0x502;
                nas->UserInfo.user1 = 0;
                nas->UserInfo.status = angle;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                func_80018F3C(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->unk6C = D_80053B9C;
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                sub->unk74 = 0x200D;
                e->unk1E = (angle + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
    PlaySfx(0x522, &eng->sfx, &eng->unk10, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Init_PogoPadlock);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Plot_PogoPadlock);

s32 func_800C0AFC(s32 arg0, s32 arg1) {
    EXTengine *e;

    if (D_800C5BB0 != NULL) {
        e = D_800C5BB0->Engine;
        if (e != NULL) {
            if ((u16)D_800C5BB0->UserInfo.status >= 3 && (e->unk10.x + 0x800) >> 9 == arg0
                && (e->unk10.z + 0x800) >> 9 == arg1) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C0B70(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *p;

    if ((u32)arg1 >= 8 || (u32)arg2 >= 8) {
        return 0;
    }
    if (D_800C53DC[arg1][arg2].hero != NULL) {
        return 0;
    }
    p = &D_800C53DC[arg1][arg2];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    return p->unk10 == 0;
}

void func_800C0BF0(s32 arg0, s32 arg1) {
    D_800C5BB0 = SpawnNASById((arg0 << 9) - 0x700, D_8005A5D8, (arg1 << 9) - 0x700, 0x1408, 0x11);
}

void func_800C0C34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_8008D5D4((arg1 << 9) - 0x700, D_8005A5D8 - 0x100, (arg2 << 9) - 0x800, arg3, 0);
}

bool SquareIsFreeForRoo(s32 arg0, s32 arg1) {
    Ovl60Cell *p;
    u16 t;

    if ((u32)arg0 >= 8 || (u32)arg1 >= 8) {
        return 1;
    }
    p = &D_800C53DC[arg0][arg1];
    if (p == NULL) {
        return 0;
    }
    if (p->unkE != 0) {
        return 0;
    }
    t = p->unk10;
    if (t != 0 && t != 5) {
        return 0;
    }
    return 1;
}

bool Roo_Bounce(NAS *nas) {
    if (++D_800C5304 < 14) {
        return 0;
    }
    D_800C5304 = 0;
    if (nas->Engine != NULL) {
        Roo_GoBoing(nas->Engine);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", Roo_UpdateVelocities);

s32 func_800C0DE4(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

s32 func_800C0E40(s32 arg0, u32 arg1, u32 arg2) {
    if (arg2 < 8) {
        if (arg1 == -1 && D_800C5B2C[arg2] == arg0) {
            return 1;
        }
        if (arg1 == 8 && D_800C5B4C[arg2] == arg0) {
            return 1;
        }
    }
    if (arg1 < 8) {
        if (arg2 == -1 && D_800C5B8C[arg1] == arg0) {
            return 1;
        }
        if (arg2 == 8 && D_800C5B6C[arg1] == arg0) {
            return 1;
        }
    }
    return 0;
}

s32 func_800C0F08(s32 arg0, EXTbotai *ai)
{
    s32 x;
    s32 z;

    EXTengine *e = Heroes[arg0].hero_eng;
    s32 a = Heroes[arg0].hero_move->facing_angle >> 10;

    if (func_800BE158(arg0, (e->unk10.x + 0x800) >> 9, (e->unk10.z + 0x800) >> 9,
                      a, &x, &z) != 0) {
        ai->AImode = 7;
        ai->TargetXYZ.x = x;
        ai->TargetXYZ.z = z;
        return 1;
    }
    return 0;
}

void func_800C0FAC(s32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AItimer1 = D_800C3A38[D_800C5BBC][0];
    }
}

void func_800C1004(s32 i)
{
    EXTbotai *ai;

    ai = Heroes[i].hero_ai;
    if (ai != NULL) {
        ai->AImode = 1;
        ai->AItimer1 = 0;
        ai->AItimer2 = Random(3) + 3;
    }
}

void func_800C1068(void) {
    s32 i;
    u8 (*p)[8] = D_800C3C14[D_800C5BC0];

    for (i = 0; i < 8; i++) {
        D_800C5B2C[i] = p[0][i];
        D_800C5B6C[i] = p[1][i];
        D_800C5B4C[7 - i] = p[2][i];
        D_800C5B8C[7 - i] = p[3][i];
    }
}

void func_800C10F8(void) {
    D_800C5BC0 = 0;
    D_800C5BC4 = Random(0xC8) + 0xC8;
    D_800C5BD0 = 0;
    D_800C5BCC = 8;
    D_800C5BD4 = 0;
    D_800C5300 = 0;
    if (D_8005A646 & 0x80) {
        D_800C5BC4 = 0x78;
    }
    func_800C1068();
}

void Init_PogoProjectile(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void func_800C11C4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    NAS *nas;
    EXTengine *e;
    subengine *sub;
    subengine *s;

    nas = SpawnNASById(arg1, D_8005A5D8 - arg3, arg2, 0x1404, 0x11);
    if (nas != NULL) {
        e = nas->Engine;
        if (e != NULL) {
            sub = e->sub;
            if (sub != NULL) {
                nas->UserInfo.user1 = 0;
                nas->UserInfo.user2 = arg0;
                nas->UserInfo.status = arg5;
                nas->UserInfo.flags = ((arg4 & 7) << 8) | 2;
                sub->unk6C = D_80053B9C;
                sub->unk74 = 0x200D;
                func_80018F3C(e, 8);
                for (s = sub->next; s != NULL; s = s->next) {
                    s->unk6C = D_80053B9C;
                    s->flags = (s->flags & ~0x8000) | 0x20000041;
                    s->unk20.z = 0x1000;
                    s->unk20.y = 0x1000;
                    s->unk20.x = 0x1000;
                    s->unk50 = 1;
                    s->unk51 = 0;
                    s->unk04 = 0x90;
                }
                switch (arg0) {
                case 1:
                    sub->unk74 = 0x200D;
                    break;
                case 2:
                    sub->unk74 = 0x201B;
                    break;
                case 3:
                    sub->unk74 = 0x2040;
                    break;
                }
                e->unk1E = (arg5 + 0xC00) % 0x1000;
                e->unk5E = 1;
                nas->UserInfo.flags |= 1;
            }
        }
    }
}

void func_800C1384(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    EXTengine *eng;

    eng = Heroes[arg0].hero_eng;
    func_800C11C4(arg1, (((eng->unk10.x + 0x800) >> 9) << 9) - 0x700,
                  (((eng->unk10.z + 0x800) >> 9) << 9) - 0x700, arg3, arg0 & 7, arg2);
}

void func_800C1400(s32 arg0) {
    D_800C535C[arg0].flags |= 1;
}

void func_800C142C(s32 arg0) {
    Heroes[arg0].hero_status |= 0x800;
}

s32 func_800C1460(s32 arg0) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unkA == arg0) {
                count++;
            }
        }
    }
    return count;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C14BC);

void func_800C159C(s32 arg0) {
    Heroes[arg0].hero_eng->unk10.x = (((Heroes[arg0].hero_eng->unk10.x + 0x800) >> 9) << 9) - 0x700;
    Heroes[arg0].hero_eng->unk10.z = (((Heroes[arg0].hero_eng->unk10.z + 0x800) >> 9) << 9) - 0x700;
}

s32 func_800C1608(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    HeroInfo *hero = ((Ovl60Cell *)(arg0 * sizeof(D_800C53DC[0]) + arg1 * sizeof(Ovl60Cell) + (u32)base))->hero;

    if (hero != NULL && hero->hero_pu_action == 0xB) {
        return 1;
    }
    return 0;
}

s32 func_800C165C(s32 *arg0, s32 *arg1) {
    s32 x;
    s32 y;
    s32 i;
    s32 ok;

    i = 0;
loop:
    x = Random(8);
    y = Random(8);
    ok = func_800BC04C(x, y) != 1;
    i++;
    if (ok && i < 10) {
        goto loop;
    }
    if (i >= 10) {
        return 0;
    }
    *arg0 = x;
    *arg1 = y;
    return 1;
}

s32 func_800C16FC(x0, y0, x1, y1)
s32 x0;
s32 y0;
s32 x1;
s32 y1;
{
    s32 dx;
    s32 dy;
    s32 a;
    s32 b;

    a = (y0 << 9) - 0x800;
    b = (y1 << 9) - 0x800;
    dx = abs((x0 - x1) << 9);
    dy = abs(a - b);
    return dx * dx + dy * dy;
}

s32 func_800C1764(void) {
    return csqrt(func_800C16FC()) >> 6;
}

s32 func_800C178C(void) {
    D5A614View *g = (D5A614View *)D_8005A614;
    s32 v;

    switch (g->mode) {
    case 10:
        v = 0x303C;
        if (g->flags & 0x80) {
            v = 0x3033;
        }
        return v;
    case 11:
        v = 0x3052;
        if (g->flags & 0x80) {
            v = 0x301B;
        }
        return v;
    case 12:
        v = 0x3035;
        if (g->flags & 0x80) {
            v = 0x303F;
        }
        return v;
    case 13:
        return 0x3000;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C1840);

void func_800C19A8(s32 *arg0, s32 arg1) {
    s32 i;
    s32 j;
    s32 *p;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            p = (s32 *)((i * 8 + j) * sizeof(s32) + (u32)arg0);
            *p = 0;
            if (D_800C53DC[j][i].unkA == arg1) {
                *p = 1;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C1A18);

void Init_PogoMushroom(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053ABC;
        e->sub->unk74 = 0x2007;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 11;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        D_800C52F4++;
    }
}

void Init_PogoBanker(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x200B;
        e->flags &= ~0x8000;
    }
    nas->UserInfo.flags = 10;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = Random(60) + 60;
    nas->UserInfo.user2 = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoPickup(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x200B;
        e->sub->unk68 = -0x10;
        e->sub->flags |= 0x02000000;
    }
    nas->UserInfo.flags = 0;
    nas->UserInfo.status = 0;
    nas->UserInfo.user1 = 0xF0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void Init_PogoTazer(NAS *nas) {
    EXTengine *e = nas->Engine;

    if (e != NULL) {
        e->sub->unk6C = D_80053B9C;
        e->sub->unk74 = 0x2005;
        e->unk5D = 2;
    }
    nas->UserInfo.flags = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800C1CF0(arg0)
s32 arg0;
{
    Ovl60Slot *base;
    Ovl60Slot *slot;

    if (D_800C535C[arg0].unk4 == 5 && Heroes[arg0].hero_aseq == 0x2F) {
        SetHeroAction((HeroInfo *)arg0, 0x39);
    }
    base = D_800C535C;
    slot = &base[arg0];
    if (slot->unk4 == 6) {
        func_800B91E8(arg0);
    }
    slot->unk4 = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C1DA4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C1EA8);

void func_800C2018(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 > 0) {
        func_800B8660(arg0, arg1, -1, 0, arg2);
    }
    if (arg0 < 7) {
        func_800B8660(arg0, arg1, 1, 0, arg2);
    }
    if (arg1 > 0) {
        func_800B8660(arg0, arg1, 0, -1, arg2);
    }
    if (arg1 < 7) {
        func_800B8660(arg0, arg1, 0, 1, arg2);
    }
}

void func_800C20C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    NAS *nas = (NAS *)D_800C53DC[arg0][arg1].hero;
    s32 dir;
    s32 i;
    s32 dx;
    s32 dz;

    if (nas != NULL) {
        if (nas->Engine != NULL) {
            PlaySfx(0x515, NULL, &nas->Engine->unk10, 0x1000, 0x1E00, 0);
        }
        dir = nas->UserInfo.user1 & 3;
        for (i = 0; i < arg3; i++) {
            switch (dir) {
            case 0:
                dx = 0;
                dz = 1;
                break;
            case 1:
                dx = 1;
                dz = 0;
                break;
            case 2:
                dx = 0;
                dz = -1;
                break;
            case 3:
                dx = -1;
                dz = 0;
                break;
            default:
                dx = 0;
                dz = 0;
                break;
            }
            func_800B8660(arg0, arg1, dx, dz, arg2);
            dir ^= 2;
        }
    }
}

void func_800C2204(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if ((u16)D_800C53DC[i][j].unk12 == 6) {
                func_800B83AC(i, j);
            }
        }
    }
}

void func_800C2294(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 4;
    p->unk14 = arg3;
    p->unk16 = arg2;
    p->unkA = arg2;
}

void func_800C22E8(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 2;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void func_800C2338(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
    p->unk12 = 3;
    p->unk14 = arg3;
    p->unk16 = arg2;
}

void func_800C2388(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);
    s16 old = p->unkA;

    p->unkA = arg2;
    p->unk14 = arg3;
    p->unk12 = 0;
    p->unk16 = old;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            func_800B7C3C(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            func_800B7C3C(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            func_800B7C3C(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            func_800B7C3C(arg0, arg1 + 1);
        }
    }
}

void func_800C244C(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 7;
}

void func_800C2488(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 6;
    p->unk14 = D_800C3998[D_800C5BB8][0];
}

void func_800C24E4(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unkA = 5;
    p->unk12 = 5;
    p->unk14 = 0x78;
}

void func_800C2524(s32 arg0, s32 arg1, s32 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 10;
    p->unkA = arg2;
    p->unk14 = 0xF0;
}

void func_800C2564(s32 arg0, s32 arg1, s16 arg2) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = arg2;
    if (D_8005A648 == 0xB) {
        if (arg0 > 0) {
            func_800B7C3C(arg0 - 1, arg1);
        }
        if (arg0 < 7) {
            func_800B7C3C(arg0 + 1, arg1);
        }
        if (arg1 > 0) {
            func_800B7C3C(arg0, arg1 - 1);
        }
        if (arg1 < 7) {
            func_800B7C3C(arg0, arg1 + 1);
        }
    }
}

void func_800C2620(s32 arg0, s32 arg1) {
    Ovl60Cell *base = D_800C53DC[0];
    Ovl60Cell *p = (Ovl60Cell *)(arg1 * sizeof(Ovl60Cell) + arg0 * sizeof(D_800C53DC[0]) + (u32)base);

    p->unk12 = 1;
    p->unkA = -1;
    p->unk14 = 0;
}

s32 func_800C2660(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C2674);

void func_800C2770(s32 arg0, u32 arg1, u32 arg2) {
    s32 i;
    s32 j;

    if (arg1 < 8 && arg2 < 8) {
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (D_800C53DC[i][j].unk10 == arg0 + 1) {
                    D_800C53DC[i][j].unk10 = 0;
                }
            }
        }
        D_800C53DC[arg1][arg2].unk10 = arg0 + 1;
    }
}

void func_800C2810(s32 arg0) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (D_800C53DC[i][j].unk10 == arg0 + 1) {
                D_800C53DC[i][j].unk10 = 0;
            }
        }
    }
}

void func_800C286C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h;
    EXTengine *eng;

    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_frozen = 2;
    func_800C1CF0();
    eng = h->hero_eng;
    if (eng != NULL) {
        s32 r = func_80015480(0, eng->unk1E);
        eng->unk5E = 1;
        eng->unk1E += r >> 3;
    }
    func_8008502C(arg0);
}

void func_800C28FC(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800C2810(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
}

s32 func_800C295C(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x100) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3E);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

s32 func_800C29F4(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x400) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3D);
        SetHeroSeq((HeroInfo *)arg0, 0x2E);
        return 1;
    }
    return 0;
}

void func_800C2A8C(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800C2810(arg0);
    func_80089FA4(arg0);
    func_800C1CF0(arg0);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    h->hero_move->velocity_enable = 0;
    h->hero_frozen = 2;
    D_800AD12C = 1;
}

s32 func_800C2B00(s32 arg0) {
    if (Heroes[arg0].hero_status & 0x200) {
        if (D_800C535C[arg0].nas != NULL) {
            func_800C2B98(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, 0x3C);
        SetHeroSeq((HeroInfo *)arg0, 0x32);
        return 1;
    }
    return 0;
}

void func_800C2B98(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        KillNAS(slot->nas);
        slot->nas = NULL;
    }
}

void func_800C2BE8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    EXTheromove *move;

    func_800C2810(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x30);
    base = Heroes;
    h = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
    move = h->hero_move;
    if (move != NULL) {
        move->velocity_enable = 0;
        move->forward_angle = move->facing_angle;
    }
    h->hero_frozen = 0xA;
    D_800C535C[arg0].unk6 = 0x14;
}

s32 func_800C2C70(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *slot = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    if (slot->nas != NULL) {
        func_800C2B98(arg0);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C2CBC);

void func_800C2DCC(s32 arg0) {
    Ovl60Slot *slot;

    Heroes[arg0].hero_move->velocity_enable = 0;
    Heroes[arg0].hero_move->jump_ctrl &= 0xFE;
    slot = &D_800C535C[arg0];
    *(s16 *)((u8 *)slot + 0x16) = 0;
    slot->unk18 = (u16)slot->unk4 == 5;
    func_800B8BE4(arg0, 0);
}

void func_800C2E50(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B4374(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B4E50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B4A70(arg0, (u16)arg1);
        } else {
            func_800B53CC(arg0, (u16)arg1);
        }
    }
}

void func_800C2EE0(void) {
    s32 i;
    u8 *p;

    for (i = 0; i < 4; i++) {
        p = D_8005A614 + i;
        if (p[0x5C] != 0) {
            D_800C5BB8 = D_8009D87C[i][0];
        }
        if (p[0x5C] == 0) {
            D_800C5BBC = D_8009D87C[i][2];
        }
    }
}

void func_800C2F50(void) {
    if (!(D_8005A646 & 0xE0)) {
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C2F84(void) {
    if (D_8005A646 & 0x20) {
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C2FB8(void) {
    u8 *g = D_8005A614;
    s32 i;

    if (*(u16 *)(g + 0x32) & 0x80) {
        if (*(s16 *)(g + 0x34) == 10) {
            D_800C52F4 = 0;
            for (i = 0; i < 2; i++) {
                SpawnNASById(0, 0, 0, 0x1407, 0x11);
            }
        }
        func_8007B7FC(0x130, 0x81D0);
    }
}

void func_800C3048(void) {
    s32 i;
    s32 n;

    n = 0;
    switch (D_8005A648) {
    case 10:
        n = 2;
        break;
    case 11:
        func_800C10F8();
        break;
    case 12:
        n = 3;
        SpawnNASById(0, 0, 0, 0x1406, 0x11);
        break;
    case 13:
        n = 2;
        func_800C0BF0(4, 4);
        break;
    }
    if (n != 0) {
        for (i = 0; i < n; i++) {
            SpawnNASById(0, 0, 0, 0x1403, 0x11);
        }
    }
}

void func_800C3148(void) {
    ((Ovl60Cam *)D_80051640)->eyeX = 0;
    ((Ovl60Cam *)D_80051640)->eyeZ = 0xE98;
    ((Ovl60Cam *)D_80051640)->rotX = 0x1BE;
    ((Ovl60Cam *)D_80051640)->dist = 0x200;
    ((Ovl60Cam *)D_80051640)->eyeY = 0;
    ((Ovl60Cam *)D_80051640)->rotY = 0;
    ((Ovl60Cam *)D_80051640)->rotZ = 0;
    ((Ovl60Cam *)D_80051640)->atX = 0;
    ((Ovl60Cam *)D_80051640)->atY = 0;
    ((Ovl60Cam *)D_80051640)->atZ = -0x650;
}

void func_800C318C(s32 arg0) {
    func_8007B7FC(arg0 & 0xFFFF, 0x81D0);
}

void func_800C31B0(void) {
    func_8007EB1C(0x140, 0x28, 0);
    D_800C5BB0 = 0;
    D_800C5BAC = 0;
    D_800C52FC = 0;
    D_800C52E0 = 0;
    D_800C52E4 = 0;
    D_8005A5D8 = -0x10;
    D_800C52EC = 0;
    D_800C52F0 = 0;
    D_800C52F8 = 0;
    D_800C5BB8 = 0x10;
    D_800C5BBC = 0x10;
    func_800B3FF0();
    func_800C3148();
    func_800B3C44();
    func_800C3048();
    func_800B3808();
    func_800C2FB8();
    func_800C2F84();
    func_800B3A38();
    func_800C2F50();
}

void func_800C327C(s32 arg0) {
    Ovl60Slot *base = D_800C535C;
    Ovl60Slot *p = (Ovl60Slot *)(arg0 * sizeof(Ovl60Slot) + (u32)base);

    p->flags = 0;
    p->unk4 = 0;
    p->nas = NULL;
    D_800C53CC[arg0] = 5;
    SetHeroAction((HeroInfo *)arg0, 0x39);
    func_800C1004(arg0);
}

void func_800C32E8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    if (s != NULL) {
        s->sub->unk6C = D_80053B9C;
        s->sub->unk74 = 0x2029;
        s->unk10 = h->hero_eng->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x800;
        s->sub->flags |= 0x40000000;
    }
}

void func_800C3388(void) {
}

void func_800C3390(void) {
}

void func_800C3398(void) {
    D_8009D6E8.unk0 = (s32)func_800C327C;
    D_8009D6E8.unk44 = &D_8008AB04;
    D_8009D6E8.unk4 = &D_800C52CC;
    D_8009D6E8.unkC = &D_800C44A8;
    D_8009D6E8.unk20 = func_800B574C;
    D_8009D6E8.unk28 = func_800B5B1C;
    D_8009D6E8.unk64 = func_800C2E50;
    D_8009D6E8.unk5C = func_800B76CC;
    D_8009D6E8.unk18 = func_800C32E8;
    D_8009D6E8.unk38 = func_800BD10C;
    func_800B3534();
}

void func_800C3434(void) {
    D_8005AA70[0] = (s32)func_800C3398;
    D_8005AA70[1] = (s32)func_800C31B0;
    D_8005AA70[2] = (s32)func_800C31B0;
    D_8005AA70[3] = (s32)func_800B3E34;
    D_8005AA70[5] = (s32)func_800C3388;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800C2660;
    func_8001E934(&D_8009F8A4, (s32)D_800C52B8);
}

void func_800C34B0(void) {
    func_800183DC();
}

void func_800C34D0(void) {
    func_80018430(0x80);
}

void func_800C34F0(s32 arg0) {
    func_80018430(arg0 * 2);
}

void func_800C3510(u8 *arg0) {
    (*(s32*)((u8*)(arg0) + 84)) = func_800C353C;
    (*(s32*)((u8*)(arg0) + 88)) = D_8001C378;
    (*(s32*)((u8*)(arg0) + 0)) = (s32) ((*(s32*)((u8*)(arg0) + 0)) | 0x01000000);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl60/ovl60", func_800C353C);
