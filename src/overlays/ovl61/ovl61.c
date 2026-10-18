#include "common.h"
#include "bash/main.h"
#include "bash/ovl61.h"
#include "bash/arkenoid.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B3528);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B36CC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B3B20);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B40AC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B42F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B453C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4880);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4AE8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4D9C);

s32 func_800B4EFC(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;
    s32 x;
    s32 lim;
    s32 z;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = c->x;
        x = arg0->x;
        dx = x - dx;
        z = arg0->z;
        dz = z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    lim = 0x900;
    if (x > lim || x < -lim) {
        return 1;
    }
    if (z > lim) {
        return 1;
    }
    return z < -lim;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B4FC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5238);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B558C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B58E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5C30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B5EC8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B60FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B62F4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B649C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B66D0);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B68C8);

void func_800B69E8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 act;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        func_800B60FC(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed != 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction((HeroInfo *)arg0, 0x1D);
            }
        }
        func_8008502C(arg0);
    }
}

void func_800B6AE0(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    u16 mode;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x22);
        ret = 1;
    } else if (st & 0x2000) {
        SetHeroAction((HeroInfo *)arg0, 0x24);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x23);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret == 0) {
        func_800B60FC(arg0);
        base = Heroes;
        h = &base[arg0];
        if (h->hero_move->forward_speed == 0) {
            mode = h->hero_mode;
            if (mode != 0x1F && mode != 0x21) {
                SetHeroAction((HeroInfo *)arg0, 0x1C);
            }
        }
        if (func_800B649C(arg0) != 0) {
            func_8008502C(arg0);
        }
    }
}

void func_800B6BE8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    h->hero_status &= 0xDFFF;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_80026A04((u8 *)h->hero);
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    func_80085718(arg0, 0);
    func_80089FA4(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B6CF0);

void func_800B6FE0(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 5;
    h->hero_powerup->flags = 0;
    h->hero_ctrl |= 1;
    h->hero_eng->flags &= ~0x21;
    if (h->hero_powerup->sfx != NULL) {
        StopSfx(h->hero_powerup->sfx);
    }
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    h->hero_sub->unk08 = 0;
    h->hero_pu = 0;
    h->hero_pu_timer = 0;
    SetHeroSeq(arg0, 0x19);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B70CC);

void func_800B7450(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    h->hero_attack_delay += 0x1E;
    if (h->hero_attack_delay > 0x5A) {
        h->hero_attack_delay = 0x5A;
    }
    h->hero_attack_counter = 0;
    h->hero_attack_timer = h->hero_attack_delay;
    PlaySfx(D_800C18E0[arg0], NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7518);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7688);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B77F8);

void func_800B79F4(s32 arg0) {
    SLONG_XYZ sp18;
    EXTengine *temp_v0;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *temp_s0;
    HeroInfo *temp_s0_2;
    _sfx_handler *temp_a0;
    subengine *temp_v1;

    base = Heroes;
    temp_s0 = &base[arg0];
    temp_s0->hero_hit = 0;
    temp_s0->hero_immunity = 0;
    temp_s0->hero_move->velocity_enable = 0;
    temp_s0->hero_move->jump_max = 0;
    temp_s0->hero_health = 0;
    temp_s0->hero_frozen = 2;
    temp_s0->hero_powerup->flags = 0;
    temp_s0->hero_fx->flags = 0;
    temp_s0->hero_pu = 0;
    temp_s0->hero_pu_timer = 0;
    temp_s0->hero_anim->sub = NULL;
    temp_a0 = temp_s0->hero_powerup->sfx;
    if (temp_a0 != NULL) {
        StopSfx(temp_a0);
    }
    temp_s0->hero_status |= 0x2000;
    temp_v1 = temp_s0->hero_sub;
    temp_v1 = temp_v1->next;
    if (temp_v1 != NULL) {
        temp_v1->flags = 0;
        temp_v1 = temp_v1->next;
        if (temp_v1 != NULL) {
            temp_v1->flags = 0;
        }
    }
    base2 = Heroes;
    temp_s0_2 = &base2[arg0];
    temp_v0 = temp_s0_2->hero_eng;
    sp18 = temp_v0->unk10;
    func_8007A524(0, (s32 *)&sp18, (s32) &D_80054504);
    PlaySfx(0x586, NULL, &temp_s0_2->hero_eng->unk10, 0x1000, 0x1E00, 0);
    func_800898EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7B60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B7C60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B808C);

s32 func_800B84B8(s32 arg0) {
    s32 sum;
    s32 i;
    s32 r;
    s32 mate;

    mate = Heroes[arg0].hero_teammate;
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
        }
    }
    if (sum == 0) {
        return -1;
    }
    r = Random(sum);
    sum = 0;
    for (i = 0; i < 4; i++) {
        if (i != arg0 && i != mate && !(Heroes[i].hero_status & 0xFA00)) {
            sum += D_8009D87C[i].unk4;
            if (r < sum) {
                return i;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B85DC);

s32 func_800B8964(s32 arg0) {
    Ovl61Node *n;
    Ovl61Node *t;
    Ovl61Node *e;
    HeroInfo *h;
    s32 dx;
    s32 dz;
    s32 lim;
    HeroInfo *base;

    if (D_8005A648 == 0xF) {
        lim = 0x64000;
        n = D_8005AA38;
        if (n != NULL) {
            base = Heroes;
            h = &base[arg0];
            do {
                t = n->target;
                if (t != NULL) {
                    e = (Ovl61Node *)h->hero_eng;
                    dx = e->x - t->x;
                    dz = e->z - t->z;
                    if (dx * dx + dz * dz < lim) {
                        if (func_80015480(get_angle(dx, dz), h->hero_move->forward_angle) < 0x2AA) {
                            return 1;
                        }
                    }
                }
                n = n->next;
            } while (n != NULL);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl61/ovl61", D_800B3300);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B8A68);

void func_800B9728(void) {
    s32 best = 0;
    s32 idx = -1;
    s32 i;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if ((h->hero_status & 0xFA00) == 0 && best < h->hero_health) {
            best = h->hero_score;
            idx = i;
        }
    }
    if (idx != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if ((h->hero_status & 0xFA00) == 0 && i != idx) {
                h->hero_score = 0;
                h->hero_health = 0;
                h->hero_status |= 0x2000;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800B97D8);

void func_800B99E0(void) {
    NAS *nas;
    EXTengine *eng;
    EXTobject *fux;
    EXTmotion *mot;
    s32 dir;
    s32 n;

    nas = D_8005AA38;
    n = 0;
    if (nas != NULL) {
        do {
            nas = *(NAS **)((u8 *)nas + 0x20);
            n++;
        } while (nas != NULL);
    }
    if (n < 8) {
        nas = SpawnNASById(((Ovl61Spawn *)D_800C1B28)->x, ((Ovl61Spawn *)D_800C1B28)->y, ((Ovl61Spawn *)D_800C1B28)->z, 0x1501, 0x11);
        if (nas != NULL) {
            eng = nas->Engine;
            if (eng != NULL) {
                eng->flags |= 0x8000;
                if (eng->Engine != NULL) {
                    eng->Engine->flags |= 0x8000;
                }
                fux = (EXTobject *)nas->FUX;
                if (fux != NULL) {
                    fux->status = 1;
                    fux->can_do = 0x40;
                    mot = FindExtension(nas, 0x800);
                    if (mot != NULL) {
                        mot->Xacc = 4;
                        mot->Xdec = 1;
                        mot->Xvel = 0x80;
                        mot->Xmax = 0x80;
                        dir = ((Ovl61Spawn *)D_800C1B28)->dir;
                        mot->Xa = dir;
                        eng->unk10.x = D_80068BD4[(dir - 0x400) & 0xFFF].unk2 * 13 >> 7;
                        eng->unk10.z = D_80068BD4[dir & 0xFFF].unk2 * 13 >> 7;
                    }
                }
                PlaySfx(0x5C1, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", InitBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", PlotBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", CalcArkInfluence);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", FreeGrabbedObject);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", AlignGrabBall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_N_Gin);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC0D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BC564);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BCA30);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_ArkDeadWall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_NGArkDeadWall);

void func_800BD43C(u32 arg0) {
    Ovl61Wall *base;
    Ovl61Wall *w;
    s32 r = Random(0x1E);

    base = D_800C1E78;
    w = &base[arg0 & 0xFFFF];
    w->f1A = r + 0x3C;
    w->f18 = 1;
    w->f10 = 0xA0;
    w->f8 = 0x300;
    w->f4 = 0;
    w->fC = 0;
    w->f14 = -1;
    PlaySfx(0x5C9, &D_800C1BC4, NULL, 0x1000, 0x1E00, 0);
    PlaySfx(D_800C139C[Random(4)], NULL, NULL, 0x1000, 0x1E00, 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD510);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BD770);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_Ark_Pickup);

void func_800BDD30(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_pu_timer != 0) {
        h->hero_pu_timer--;
    }
    if (h->hero_pu_action_timer != 0) {
        h->hero_pu_action_timer--;
    }
    if (h->hero_pu_lock != 0) {
        h->hero_pu_lock--;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Plot_RepulseRing);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", Init_ArkLaserWall);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE1C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BE4D4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", ArkHeroRad2ObjectCollision);

void ArkHeroRad2ObjectRepel(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *t;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 q;
    s32 r;
    s32 lim;
    s32 ang;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 1);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = D_8005AA38;
        if (n != NULL) {
            do {
                t = *(u8 **)(n + 0x58);
                if (t != NULL && *(u16 *)(t + 0x1A) == 1) {
                    o = *(u8 **)(n + 0x54);
                    if (o != NULL) {
                        dx = *(s32 *)(o + 0x10) - sp10.x;
                        dz = *(s32 *)(o + 0x18) - sp10.z;
                        d = dx * dx + dz * dz;
                        if (d < lim) {
                            d = csqrt(d) >> 6;
                            ang = get_angle(-dx, -dz) & 0xFFF;
                            d = r - d;                            e = FindExtension((NAS *)n, 0x800);
                            if (e != NULL) {
                                *(s16 *)(e + 0x2A) = ang;
                                *(s16 *)(e + 0x22) = d >> 4;
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

void ArkHeroRad2ObjectAttract(u32 arg0, s32 arg1) {
    SLONG_XYZ sp10;
    EXTengine *eng;
    FlagRecord *rec;
    u8 *n;
    u8 *o;
    u8 *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 lim;
    s32 ang;
    s32 a;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        rec = func_80025A48(eng, 2);
        if (rec != NULL) {
            if (eng->unk5E != 0) {
                func_800142D4(&eng->unk1C);
            }
            func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
            func_80015758((SVECTOR *)rec, (VECTOR *)&sp10);
        } else {
            sp10 = eng->unk10;
        }
        r = arg1 + 0x59;
        lim = r * r;
        n = D_8005AA38;
        if (n != NULL) {
            do {
                o = *(u8 **)(n + 0x54);
                if (o != NULL) {
                    dx = *(s32 *)(o + 0x10) - sp10.x;
                    dz = *(s32 *)(o + 0x18) - sp10.z;
                    d = dx * dx + dz * dz;
                    if (d < lim) {
                        e = FindExtension((NAS *)n, 0x800);
                        if (e != NULL) {
                            ang = get_angle(dx, dz) & 0xFFF;
                            a = func_80015480(ang, *(s16 *)(e + 0x16));
                            if (a < 0x300) {
                                s32 s = csqrt(d);
                                d = s >> 6;
                                if (a < (s >> 8)) {
                                    d = r - d;
                                    *(s16 *)(e + 0x2A) = ang;
                                    *(s16 *)(e + 0x22) = d >> 4;
                                }
                            }
                        }
                    }
                }
                n = *(u8 **)(n + 0x20);
            } while (n != NULL);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BF640);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl61/ovl61", func_800BFA24);

void Plot_ArkLaserWall(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    if (D_8005A644 & 0x801) {
        D_800C1F14 &= ~nas->UserInfo.flags;
        KillNAS(nas);
        return;
    }
    eng = nas->Engine;
    if (eng != NULL) {
        if (D_800C1C70 == 0) {
            D_800C1F14 &= ~nas->UserInfo.flags;
            if (eng->sfx != NULL) {
                StopSfx(eng->sfx);
            }
            PlaySfx(0x5B8, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            KillNAS(nas);
            return;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Init_RepulseRing(NAS *nas)
{
  EXTengine *eng;
  char new_var;
  subengine *sub;
  eng = nas->Engine;
  if (eng != 0)
  {
    nas->UserInfo.user1 = D_8009D754;
    eng->unk5D = 0;
    eng->unk5C = 1;
    sub = eng->sub;
    eng->flags |= 0x21;
    sub->unk51 = (new_var = 0);
    sub->unk6C = &D_80054504;
    sub->unk74 = 0x2014;
    sub->unk50 = 1;
    sub->unk08 = 0x80;
    sub->flags = (sub->flags & 0xFFFF7FFF) | 0x21;
    PlaySfx(0x5D6, 0, 0, 0x1000, 0x1E00, new_var);
    PlaySfx(0x5D7, &eng->sfx, 0, 0x1000, 0x1E00, 0);
    nas->UserInfo.user2 = 0xF0;
    D_8009E25C.n += 1;
  }
  if (nas->Failure != 0)
  {
    KillNAS(nas);
  }
}

s32 func_800BFD14(Ovl61Vel *v) {
    return v->pad14 - ((D_8005A5D8 - (v->z * D_80068BD4[(D_800C1C68 - 0x400) & 0xFFF].unk2 >> 12)) +
                       (v->x * D_80068BD4[(D_800C1C6C - 0x400) & 0xFFF].unk2 >> 12));
}

s32 func_800BFD9C(u8 *arg0) {
    return (*(s32*)((u8*)(arg0) + 20)) - D_8005A5D8;
}

void Plot_ArkDeadWall(NAS *arg0) {
    EXTengine *eng;
    subengine *sub;

    if (D_800C1C88 == 0) {
        D_800C1F14 &= ~arg0->UserInfo.flags;
        KillNAS(arg0);
        return;
    }
    eng = arg0->Engine;
    if (eng != NULL) {
        if (arg0->UserInfo.user1 != 0) {
            arg0->UserInfo.user1 = arg0->UserInfo.user1 - 1;
        } else {
            eng->flags |= 0x8000;
        }
        sub = eng->sub;
        if (sub != NULL) {
            do {
                sub->unk74 = (D_8004E0D8 & 7) + 0x2001;
                sub = sub->next;
            } while (sub != NULL);
        }
    }
}

void Plot_ArkCameraFlash(NAS *arg0)
{
  long new_var;
  EXTengine *eng = arg0->Engine;
  if (eng != 0)
  {
    Ovl61Circle *base = D_800C1A44;
    subengine *sub = eng->sub;
    Ovl61Circle *c = &base[arg0->UserInfo.user1];
    if (c->x != 0)
    {
      sub->unk20.x = c->x;
      sub->unk20.y = c->z;
      new_var = c->r;
      sub->flags |= 0x8000;
      sub->unk52 = 1;
      sub->unk20.x = new_var;
      arg0->UserInfo.user1 += 1;
    }
    else
    {
      KillNAS(arg0);
    }
  }
}

void Init_ArkCameraFlash(NAS *arg0) {
    u8 *eng = (u8*)(*(s32*)((u8*)(arg0) + 84));
    u8 *sub;
    if (eng != NULL) {
        s32 val;
        sub = (u8*)(*(s32*)(eng + 108));
        val = *(s32*)D_80058AD0;
        (*(u16*)(sub + 116)) = 0x2017;
        (*(u8*)(sub + 80)) = 1;
        (*(u8*)(sub + 81)) = 0;
        (*(s32*)(sub + 108)) = val;
        (*(s32*)((u8*)(arg0) + 96)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS(arg0);
    }
}

void Plot_seaweed(NAS *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        func_800250B0(arg0);
    }
}

void Init_seaweed(NAS *arg0) {
    EXTengine *eng = arg0->Engine;
    if (eng != NULL) {
        ScriptCursor *cur;
        eng->sub->unk6C = *(s32*)D_80058AD0;
        cur = func_80024E30(arg0, &D_800C1554);
        if (cur != NULL) {
            u8 *trig = (u8*)arg0->trigger;
            if (trig != NULL) {
                u8 *info = (u8*)*(s32*)(trig + 16);
                s32 n = *(s32*)(info + 4);
                ((void (*)(EXTanimator*, s32))func_80025FAC)((EXTanimator*)cur, n);
                if (n < 2) {
                    func_800250B0(arg0);
                    *(u16*)((u8*)cur + 30) += Random(8);
                }
                eng->sub->flags |= 0x02000000;
                eng->sub->unk68 = *(u16*)(info + 8);
            }
        }
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
    }
}

void Init_N_Gin(NAS *nas) {
    EXTengine *eng;
    Ovl61Gin *obj;

    eng = nas->Engine;
    if (eng != NULL) {
        func_80024E30(nas, &D_800C14A8);
        D_800C1B28 = eng;
        eng->sub->unk6C = D_80054574;
        eng->sub->unk74 = 0x2001;
        D_800C1B2C = obj = (Ovl61Gin *)AddObject(nas);
        if (obj != NULL) {
            func_80026AD8((u8 *)nas);
            func_8002694C(nas, 0x200);
            nas->FUX = (EXT *)obj;
            obj->unk14 = 0xE;
            obj->unk1A = 0;
            obj->unk22 = 0x186;
            obj->unk1C = 0;
        }
        eng->unk10.y = -0x1000;
        nas->Class &= 0xFDFF;
        eng->unk5D = 1;
        eng->flags &= 0xFFFF7FFF;
        func_80025B10(eng, 0x50);
    }
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

u8 *func_800C01B8(void) {
    s32 var_v0;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_v1;

    var_v1 = D_8005AA38;
    var_v0 = 0;
    if (var_v1 != NULL) {
        do {
            var_v1 = (*(s32*)((u8*)(var_v1) + 32));
            var_v0 += 1;
        } while (var_v1 != NULL);
    }
    if (var_v0 >= 9) {
        return NULL;
    }
    temp_v0 = SpawnNASById(0x1000, 0x1000, 0x1000, 0x1501, 0x11);
    if ((temp_v0 != NULL) && ((*(s32*)((u8*)(temp_v0) + 84)) != 0)) {
        temp_v0_2 = (*(s32*)((u8*)(temp_v0) + 88));
        if (temp_v0_2 != NULL) {
            (*(s16*)((u8*)(temp_v0_2) + 26)) = 0;
        }
    }
    return temp_v0;
}

s32 func_800C0250(s32 arg0, s32 arg1) {
    s32 row;

    row = arg0 == D_800C2728;
    if (arg0 == D_800C2720) {
        row = 2;
    }
    if (arg0 == D_800C271C) {
        row = 3;
    }
    if (arg1 == *D_800C19C0[row][0]) {
        return 0;
    }
    if (arg1 == *D_800C19C0[row][1]) {
        return 1;
    }
    if (arg1 == *D_800C19C0[row][2]) {
        return 2;
    }
    return 3;
}

void func_800C02F8(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    h->hero_weapon_timer = 0x96;
    h->hero_attack_counter = 0;
    PlaySfx(D_800C18E8[arg0], &h->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
}

void func_800C039C(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;

    func_800B62F4();
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x1D);
    D_800C1F0C = Random(0x3C) + 0x3C;
    h->hero_attack_counter = 0;
    PlaySfx(D_800C18E0[arg0], NULL, NULL, 0x1000, 0x1E00, 0);
}

void func_800C0448(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_frozen = 5;
    if (h->hero_hit != 0) {
        h->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x25);
    }
    func_8008502C(arg0);
}

void func_800C04B8(s32 arg0) {
    Heroes[arg0].hero_frozen = 5;
    func_8008502C();
}

void func_800C04FC(s32 arg0) {
    func_800B62F4();
    SetHeroSeq(arg0, 0x1A);
}

void func_800C052C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    func_800B62F4(arg0);
    SetHeroSeq((HeroInfo *)arg0, 0x19);
    h->hero_idle_count = Random(0x96) + 0xB4;
}

s32 func_800C05B8(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x22;
    if (!(st & 0x200)) {
        act = 0x23;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800C0624(s32 arg0) {
    HeroInfo *base = Heroes;
    u16 st = base[arg0].hero_status;
    s32 act;

    if (st & 0x200) {
        act = 0x22;
    } else if (st & 0x2000) {
        act = 0x24;
    } else if (st & 0x400) {
        act = 0x23;
    } else {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

void func_800C069C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *shadow = h->hero_shadow;

    shadow->unk10 = h->hero_eng->unk10;
    shadow->unk10.y -= ((s32 (*)(EXTengine *))func_80025F44)(shadow);
    shadow->unk1E = h->hero_eng->unk1E;
    shadow->unk5E = 1;
    if (h->hero_status & 0xF800) {
        shadow->sub->flags &= ~0x8000;
    }
}

extern char D_8005453C[];

void func_800C0760(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    s->sub->unk6C = D_8005453C;
    s->sub->unk74 = 0x2002;
    s->unk10 = h->hero_eng->unk10;
    s->unk5D = 0;
    s->flags |= 0x8041;
    s->sub->unk76 = 0x200;
    s->sub->flags |= 0x42000000;
    s->sub->unk68 = -0x10;
}

s32 func_800C0804(s32 arg0, s16 arg1, s32 arg2, s16 arg3) {
    s32 a = arg1 * D_80068BD4[((arg0 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 b = arg1 * D_80068BD4[arg0 & 0xFFF].unk2;
    s32 c = arg3 * D_80068BD4[((arg2 & 0xFFFF) - 0x400) & 0xFFF].unk2;
    s32 d = arg3 * D_80068BD4[arg2 & 0xFFF].unk2;
    s32 x = a >> 12;
    s32 z = b >> 12;
    x += c >> 12;
    z += d >> 12;
    return get_angle(x, z) & 0xFFFF;
}

s32 func_800C08C8(Ovl61Obj *arg0) {
    Ovl61Circle *base = D_800C1A7C;
    Ovl61Circle *c;
    s32 i;
    s32 dx;
    s32 dz;
    s32 r;

    for (i = 0; i < 4; i++) {
        c = &base[i];
        if (arg0->flags & 0x40) {
            r = c->r + arg0->radius;
        } else {
            r = c->r + 0x80;
        }
        dx = arg0->x - c->x;
        dz = arg0->z - c->z;
        if (dx * dx + dz * dz < r * r) {
            return 1;
        }
    }
    return 0;
}

s32 func_800C096C(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0x1000;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = D_800C271C;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = D_800C2720;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = D_800C2724;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = D_800C2728;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x > 0x680) {
                arg0->x = 0x680;
            }
            if (arg0->x < -0x680) {
                arg0->x = -0x680;
            }
        } else {
            if (z > 0x680) {
                arg0->z = 0x680;
            }
            if (arg0->z < -0x680) {
                arg0->z = -0x680;
            }
        }
    }
    return 0;
}

s32 func_800C0A74(Ovl61Vel *arg0, Ovl61Dir *arg1, s32 arg2) {
    s32 lim;
    s32 x;
    s32 z;
    s32 d;

    lim = 0xA00;
    if (arg2 != 0) {
        lim = 0xB80;
    }
    x = arg0->x;
    if (x > lim) {
        arg1->dir = D_800C271C;
        return 1;
    }
    if (x < -lim) {
        arg1->dir = D_800C2720;
        return 1;
    }
    z = arg0->z;
    if (z > lim) {
        arg1->dir = D_800C2724;
        return 1;
    }
    if (z < -lim) {
        arg1->dir = D_800C2728;
        return 1;
    }
    if (arg2 != 0) {
        d = arg1->dir;
        if (d == D_800C2724 || d == D_800C2728) {
            if (x > 0x600) {
                arg0->x = 0x600;
            }
            if (arg0->x < -0x600) {
                arg0->x = -0x600;
            }
        } else {
            if (z > 0x600) {
                arg0->z = 0x600;
            }
            if (arg0->z < -0x600) {
                arg0->z = -0x600;
            }
        }
    }
    return 0;
}

void func_800C0B7C(void) {
}

void func_800C0B84(void) {
}

void func_800C0B8C(void) {
}

void func_800C0B94(void) {
    D_8005AA70[0] = (s32)func_800B36CC;
    D_8005AA70[1] = (s32)func_800B3B20;
    D_8005AA70[2] = (s32)func_800B3B20;
    D_8005AA70[3] = (s32)func_800B40AC;
    D_8005AA70[6] = (s32)func_800C0B7C;
    D_8005AA70[5] = (s32)func_800C0B84;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[9] = (s32)func_800BFD9C;
    D_8005AA70[10] = (s32)func_800B9728;
}
