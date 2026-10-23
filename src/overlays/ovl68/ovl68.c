#include "common.h"
#include "bash/ovl68.h"
#include "bash/chase.h"
#include "bash/pogo.h"
#include "bash/bash.h"
#include "bash/xtras.h"
#include "bash/bonk.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_800B3330(s32 *arg0) {
}

void func_800B3338(s32 arg0, EXTengine *arg1) {
    HeroInfo *h;
    s32 hx, hz;
    s32 a;
    s32 dx, dz;
    u8 *p;

    hx = Heroes[arg0].hero_eng->unk10.x;
    hz = Heroes[arg0].hero_eng->unk10.z;
    h = &Heroes[arg0];
    a = get_angle(arg1->unk10.x - hx, arg1->unk10.z - hz);
    switch (h->hero_name) {
    case 3:
    case 4:
        a -= 0x5A;
        break;
    default:
        a += 0x5A;
        break;
    }
    a &= 0xFFF;
    dx = (D_80068BD4[(a - 0x400) & 0xFFF].unk2 * 256) >> 12;
    dz = (D_80068BD4[a].unk2 * 256) >> 12;
    get_angle(arg1->unk10.x + dx - hx, arg1->unk10.z + dz - hz);
    p = (arg0 * 0x30) + D_800BC210;
    *(s32 *)(p + 0xC) = arg1->unk10.x + dx;
    *(s32 *)(p + 0x10) = arg1->unk10.z + dz;
    *(s32 *)(p + 0x14) = get_angle(*(s32 *)(p + 0xC) - hx, *(s32 *)(p + 0x10) - hz);
}

void func_800B3488(s32 arg0, EXTengine *arg1, Ovl68AiState *arg2) {
    NAS *nas;
    NAS *best;
    EXTengine *eng;
    s32 x, z;
    s32 i;
    s32 d, dx, dz;
    s32 bestd;
    u32 r;
    Ovl68CoalFux *fux;

    if (Random(0x40) <= D_8009D87C[arg0].unk0 + 0x20) {
        x = arg1->unk10.x;
        z = arg1->unk10.z;
        if ((arg2->pickup = Ovl68NearPickup(x, z)) != NULL) {
            arg2->state = 5;
            arg2->timer = 0x96;
            ((void (*)(s32, EXTengine *, s32))func_800B3338)(arg0, arg2->pickup->Engine, 0);
            return;
        }
        bestd = 0x8000;
        if (arg2->unk20 <= 0) {
            arg2->state = 7;
            arg2->timer = 0x3C;
            arg2->angle = get_angle(arg1->unk10.x, arg1->unk10.z);
            return;
        }
        best = NULL;
        for (i = 0; i < 8; i++) {
            nas = D_800BC424[i];
            if (nas != NULL) {
                eng = nas->Engine;
                dx = eng->unk10.x - x;
                dz = eng->unk10.z - z;
                dx *= dx;
                dz *= dz;
                d = csqrt(dx + dz) >> 6;
                fux = (Ovl68CoalFux *)nas->FUX;
                if (fux->owner >= 0 && fux->owner != arg0) {
                    d *= 2;
                }
                if (eng->sub->unk74 == 0x2017) {
                    d = d * 2 / 3;
                } else if (eng->sub->unk74 == 0x2016) {
                    d /= 2;
                }
                if (d < bestd) {
                    bestd = d;
                    best = nas;
                }
            }
        }
        if (best != NULL) {
            arg2->state = 6;
            arg2->target = best;
            arg2->timer = 0x3C;
            ((void (*)(s32, EXTengine *, s32))func_800B3338)(arg0, best->Engine, 0x100);
            fux = (Ovl68CoalFux *)best->FUX;
            fux->owner = arg0;
            return;
        }
    }
    arg2->timer = 0x1E;
    r = Random(8);
    switch (r) {
    case 0:
    case 1:
    case 2:
    default:
        arg2->timer = Random(0x1E) + 0xF;
        arg2->unk8 = 0;
        arg2->state = 1;
        return;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    arg2->state = 4;
    arg2->x = Random(0x1100) - 0x880;
    arg2->z = Random(0x1100) - 0x880;
    arg2->angle = get_angle(arg2->x - arg1->unk10.x, arg2->z - arg1->unk10.z);
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl68/ovl68", D_800B32B8);

void func_800B37C8(s32 arg0) {
    u32 off;
    Ovl68AiState *ai;
    EXTengine *eng;
    EXTengine *e;
    NAS *nas;
    NAS *best;
    NAS **pp;
    s32 i;
    s32 n;
    s32 d, dx, dz;
    s32 bestd;
    s32 x, z;
    Ovl68CoalFux *fux;

    if (D_8005A624 & 0x04000000) {
        return;
    }
    eng = Heroes[arg0].hero->Engine;
    ai = (Ovl68AiState *)(D_800BC210 + arg0 * 0x30);
    switch ((ai->timer >> 4) & 3) {
    case 1:
    default:
        off = 0;
        break;
    case 0:
        off = -200;
        break;
    case 2:
        off = 200;
        break;
    }
    ai->unk8 = 0;
    if (ai->unk20 > 0) {
        ai->unk20--;
    }
    if (--ai->timer <= 0) {
        func_800B3488(arg0, eng, ai);
    } else {
        Ovl68NearPickup(eng->unk10.x, eng->unk10.z);
    }
    switch (ai->state) {
    case 4:
        ai->unk8 |= D_80051244[((ai->angle + off) >> 9) & 7];
        if (ai->timer < 5 || ((d = ai->x - eng->unk10.x, (u32)(d + 99) < 199) &&
                              (dz = ai->z - eng->unk10.z, (u32)(dz + 99) < 199))) {
            ai->state = 1;
            ai->timer = 1;
            ai->unk8 |= 0x2000;
        }
        break;
    case 5:
        if (ai->pickup->Type != Ovl68ASTIndex(0x1D04) || ai->pickup->NASid == 0 || ai->unk24 != 0) {
            goto rethink;
        }
        ((void (*)(s32, EXTengine *, s32))func_800B3338)(arg0, ai->pickup->Engine, 0);
        ai->unk8 |= D_80051244[((ai->angle + off) >> 9) & 7];
        break;
    case 6:
        if (ai->unk20 <= 0) {
            func_800B3488(arg0, eng, ai);
            break;
        }
        if (ai->target->Type != Ovl68ASTIndex(0x1D01) || ai->target->NASid == 0) {
            goto rethink;
        }
        if (Random(0x20) < D_8009D87C[arg0].unk0) {
            nas = ai->target;
            fux = (Ovl68CoalFux *)nas->FUX;
            x = eng->unk10.x;
            z = eng->unk10.z;
            if (fux->owner == arg0) {
                fux->owner = -21;
            }
            bestd = 0x8000;
            best = NULL;
            for (i = 0; i < 8; i++) {
                nas = D_800BC424[i];
                if (nas != NULL) {
                    e = nas->Engine;
                    dx = e->unk10.x - x;
                    dz = e->unk10.z - z;
                    dx *= dx;
                    dz *= dz;
                    d = csqrt(dx + dz) >> 6;
                    fux = (Ovl68CoalFux *)nas->FUX;
                    if (fux->owner >= 0 && fux->owner != arg0) {
                        d *= 2;
                    }
                    if (e->sub->unk74 == 0x2017) {
                        d = d * 2 / 3;
                    } else if (e->sub->unk74 == 0x2016) {
                        d /= 2;
                    }
                    if (d < bestd) {
                        bestd = d;
                        best = nas;
                    }
                }
            }
            if (best != NULL) {
                ai->state = 6;
                ai->target = best;
                ai->timer = 0x3C;
                ((void (*)(s32, EXTengine *, s32))func_800B3338)(arg0, best->Engine, 0x100);
                fux = (Ovl68CoalFux *)best->FUX;
                fux->owner = arg0;
            }
        }
        ((void (*)(s32, EXTengine *, s32))func_800B3338)(arg0, ai->target->Engine, 0);
        ai->unk8 |= D_80051244[((ai->angle + off) >> 9) & 7];
        break;
    case 7:
        if (ai->unk20 > 0) {
            goto rethink;
        }
        dx = eng->unk10.x;
        dz = eng->unk10.z;
        dx *= dx;
        dz *= dz;
        if ((csqrt(dx + dz) >> 6) >= D_800BC448 - 200) {
            ai->state = 8;
            ai->timer = 0x3C;
            ai->angle -= 0x400;
        }
        ai->unk8 |= D_80051244[(ai->angle >> 9) & 7];
        break;
    case 8:
        if (ai->unk20 > 0) {
        rethink:
            func_800B3488(arg0, eng, ai);
            break;
        }
        ai->unk8 |= D_80051244[(ai->angle >> 9) & 7];
        break;
    }
    if (ai->timer & 7) {
        n = 0;
        for (i = 0; i < 8; i++) {
            nas = D_800BC424[i];
            if (nas != NULL) {
                e = nas->Engine;
                dx = e->unk10.x - eng->unk10.x;
                dz = e->unk10.z - eng->unk10.z;
                dx *= dx;
                dz *= dz;
                d = csqrt(dx + dz) >> 6;
                if (d < 0x380) {
                    n++;
                }
            }
        }
        if (n >= 3) {
            ai->unk8 |= 0x2000;
        }
    }
    switch (ai->state) {
    case 1:
    case 4:
    case 7:
        break;
    case 5:
    default:
        ai->unk8 |= 0x8000;
        break;
    }
    set_joypad((u8 *)&D_80051338[arg0], ai->unk8);
}

void func_800B3EFC(void) {
    D_8009D6E8.unk4 = D_800BB928;
    D_8009D6E8.unk38 = (void *)func_800B37C8;
    D_8009D6E8.unk40 = (void *)func_800BAFB4;
    D_8009D6E8.unk0 = (void *)func_800B46BC;
    D_8009D6E8.unk68 = (void *)func_800B459C;
    D_8009D6E8.unk14 = D_800BB93C;
    D_8009D6E8.unk20 = (void *)func_800B48E4;
    D_8009D6E8.unk24 = (void *)func_800B4B34;
    D_8009D6E8.unk54 = (void *)func_800B52C8;
    D_8009D6E8.unk44 = (void *)func_800BAF00;
    D_8009D6E8.unk48 = (void *)func_800BAE84;
    D_8009D6E8.unk40 = (void *)func_800BAFB4;
    D_8009D6E8.unk5C = (void *)func_800BA70C;
    D_8009E0E2 = 0x77;
    D_8009E0E4 = 0x75;
    D_8009E0E6 = 0x74;
    D_8009E0D8 = 0x73;
    D_8009D6E8.unk64 = (void *)func_800BA5A0;
    D_8009D8F4 = D_800BC0E4;
    D_8009D8F8 = D_800BC0F8;
    D_8009D904 = D_800BC0CC;
    D_8009D8FC = D_8005A680 != 0 ? D_800BC0AC : D_800BC098;
    D_8009D900 = D_800BC0C0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl68/ovl68", func_800B4048);

void func_800B4444(void) {
    s32 i;
    s16 c;
    s32 n;
    u8 *g;
    u8 *h;
    NAS **p;

    c = 0;
    i = 0;
    p = D_800BC424;
loop:
    if (i < 8) {
        if (*p != NULL) {
            c++;
        }
        p++;
        i++;
        goto loop;
    }
    g = D_80052A54;
    if (*(s32 *)(g + 0x1544) != 0) {
        func_80029100(g + 0x1548);
    }
    n = 3;
    do {
        n--;
    } while (n >= 0);
    h = D_80053F8C;
    func_80029100(h + 0x10);
    func_800B3330(D_800BC240);
}

void func_800B44C8(void) {
    HeroInfo *h;
    s32 i;
    s32 best;
    s32 max;

    max = 0;
    best = -1;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (!(h->hero_status & 0xFA00) && max < h->hero_health) {
            max = h->hero_health;
            best = i;
        }
    }
    if (best != -1) {
        for (i = 0; i < 4; i++) {
            h = &Heroes[i];
            if (!(h->hero_status & 0xFA00) && i != best) {
                func_80084E04(i, 0, -1, 0x1000, 0);
            }
        }
    }
}

s32 func_800B459C(s32 *p) {
    s32 d;
    s32 a;
    if (p[4] < -D_800BC44C || D_800BC44C < p[4] || p[6] < -D_800BC44C || D_800BC44C < p[6]) {
        d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - D_800BC448;
        if (d > 0) {
            a = get_angle(p[4], p[6]);
            p[4] -= (d * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
            p[6] -= (d * D_80068BD4[a & 0xFFF].unk2) >> 12;
            return 0;
        }
    }
    return 1;
}

void func_800B46BC(s32 arg0) {
    HeroInfo *b;
    HeroInfo *h;
    EXTengine *eng;
    subengine *sub;
    s32 *st;
    u8 *g;
    u8 *p;
    NAS *nas;

    D_80051338[arg0].unk50 = 0;
    D_80051338[arg0].unk54 = 0;
    b = Heroes;
    h = b + arg0;
    if (h->hero_name == 9) {
        return;
    }
    g = &D_8005A614[arg0];
    if (g[0x50] == 9) {
        return;
    }
    SetHeroAction((HeroInfo *)arg0, 0x71);
    eng = h->hero_eng;
    eng->unk5D = 1;
    eng->unk1E = 0x400;
    st = D_800BC2D0[arg0];
    func_80018F3C(eng, 1);
    sub = eng->sub;
    sub->unk51 = 1;
    sub->unk74 = D_800BC188[h->hero_name];
    FetchDefaultColBox(eng, 0x50);
    sub->unk74 = D_800BC19C[h->hero_name];
    h->hero_score = 0;
    st[0] = 1;
    st[2] = 0xB4;
    st[0x12] = 0;
    D_800BC46C[arg0] = 0;
    D_800BC47C[arg0] = 0;
    nas = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1D09, 0x11);
    nas->UserInfo.user2 = arg0;
    D_800BC45C[arg0] = (s32)nas->Engine;
    D_800BC48C[arg0] = 0;
    if (g[0x5C] != 0) {
        p = (arg0 * 0x30) + D_800BC210;
        *(s32 *)(p + 0) = 1;
        *(s32 *)(p + 4) = 1;
        *(s32 *)(p + 8) = 0;
        *(s32 *)(p + 24) = 0;
        *(s32 *)(p + 28) = 0;
        *(s32 *)(p + 32) = 0;
        *(s32 *)(p + 36) = 0;
        *(s32 *)(p + 40) = 0x8000;
        *(s32 *)(p + 44) = 0x8000;
    }
}

void func_800B48E4(s32 arg0) {
    HeroInfo *hb;
    HeroInfo *hb2;
    Ovl68Pad *pb;
    HeroInfo *h;
    HeroInfo *h2;
    EXTheromove *m;
    Ovl68Pad *pad;
    u16 *row;
    u16 speed;
    u16 rw;
    s32 vc;

    hb = Heroes;
    h = hb + arg0;
    m = h->hero_move;
    m->forward_speed = 0;
    if (h->hero_frozen != 0) {
        D_80051338[arg0].unk50 = 0;
        D_80051338[arg0].unk54 = 0;
    }
    pb = D_80051338;
    pad = pb + arg0;
    if ((pad->unk0 & 1) || &D_8005A670 != NULL) {
        if (pad->unk50 & 0xF0) {
            m->forward_angle = pad->unk38 + 0x800;
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

void func_800B4B34(s32 arg0) {
    HeroInfo *h;
    EXTengine *e;
    EXTengine *e0;
    EXTengine *ne;
    s32 dx;
    s32 dz;
    EXTheromove *m;
    s32 step;
    HeroInfo *base;
    s32 *st;
    NAS *t;
    SLONG_XYZ *v;
    u8 *anim;
    NAS *nas;
    s32 *ext;

    base = Heroes;
    h = base + arg0;
    e0 = h->hero_eng;
    h->hero_immunity = 0;
    m = h->hero_move;
    e = e0;
    if (D_80051338[arg0].unk50 & 0x8000) {
        m->run_walk = 0;
    } else {
        m->run_walk = 1;
    }
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
    if ((D_80051338[arg0].unk50 & 0x8000) && e0->unk10.y >= -0x31) {
        D_800BC49C[arg0] = 0xF;
        e0 = Heroes[arg0].hero_eng;
        st = D_800BC2D0[arg0];
        t = NULL;
        if ((NAS *)st[0x12] == NULL
            || (ne = ((NAS *)st[0x12])->Engine, dx = e0->unk10.x - ne->unk10.x, dx = dx * dx,
                dz = e0->unk10.z - ne->unk10.z, dz = dz * dz, dx + dz >= 0x400)) {
            t = func_800B7948(arg0);
        }
        if (t != NULL) {
            st[0x12] = (s32)t;
        }
    } else {
        if (--D_800BC49C[arg0] <= 0) {
            D_800BC48C[arg0] = 0;
        }
    }
    if ((D_80051338[arg0].unk50 & 0x2A00) && e0->unk10.y >= -0x31 && D_800BC2D0[arg0][0x14] != 0) {
        v = &e0->unk10;
        PlaySfx(0x43A, NULL, v, 0x1000, 0x1E00, 0);
        func_800BA694(arg0);
        func_800B7DF0(e0->unk10.x, e0->unk10.z, arg0, 0, 0x380, 0);
        func_800BB010(e0->unk10.x, e0->unk10.z, arg0, 0x380);
        anim = D_80053F8C;
        nas = CreateNAS(e0->unk10.x, v->y, v->z, 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext[3] = 3;
                ext[6] = 0x100;
                if (anim != NULL) {
                    ext[8] = 5;
                    nas->Engine->sub->unk6C = anim;
                } else {
                    ext[8] = 1;
                }
                ext[10] = 0;
                nas->InitDelete(nas);
            }
        }
    }
}

s32 func_800B51C8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    if (h->hero_move->jump_max != 0) {
        if (func_80025BBC(h->hero_eng) != 0) {
            h->hero_move->jump_max = 0;
            h->hero_sub->unk08 = 0;
        } else {
            if (h->hero_ctrl & 8) {
                return 0;
            }
            if (World_floor_level < h->hero_eng->unk10.y + h->hero_sub->unk08) {
                if (D_8005B5B8 == 0x7FFFFFFF) {
                    SetHeroAction((HeroInfo *)arg0, 0x79);
                    return 1;
                }
                h->hero_eng->unk10.y = World_floor_level;
                h->hero_sub->unk08 = 0;
            }
        }
    } else {
        func_800863D0(arg0);
    }
    return 0;
}

s32 func_800B52C8(s32 arg0) {
    Ovl68Trig *tbl;
    VECTOR pos;
    NAS *own;
    EXTengine *obj;
    s32 hit;
    s32 top;
    s32 push;
    s32 rad;
    s32 lo;
    s32 hi;
    s32 rad2;
    s32 hi2;
    s32 lo2;
    s32 rad3;
    s32 lo3;
    s32 hi3;
    u32 i;
    HeroInfo *self;
    HeroInfo *other;
    EXTengine *eng;
    EXTengine *e;
    FlagRecord *rec;
    FlagRecord *box;
    NAS *n;
    Ovl68HitFlags *t;
    s32 bot;
    s32 dx;
    s32 dz;
    s32 d;
    s32 r;
    s32 rr;
    s32 ang;
    s32 a;
    s32 tmp;

    hit = 0;
    lo2 = 0;
    hi2 = 0;
    rad2 = 0;
    hi3 = 0;
    lo3 = 0;
    rad3 = 0;
    eng = Heroes[arg0].hero_eng;
    obj = NULL;
    if (eng != NULL) {
        box = func_80025A48(eng, 0x40);
        if (box != NULL) {
            rad = box->radius;
            hi = eng->unk10.y + box->unk2;
            lo = eng->unk10.y + box->height;
            box = func_80025A48(eng, 1);
            if (box != NULL) {
                if (eng->unk5E != 0) {
                    func_800142D4(&eng->unk1C);
                }
                func_8001CD04(eng->sub, eng->unk3C, &eng->unk10);
                lo2 = eng->unk10.y + box->unk2;
                hi2 = eng->unk10.y + box->height;
                if (lo2 < hi2) {
                    tmp = hi2;
                    hi2 = lo2;
                    lo2 = tmp;
                }
                rad2 = box->radius;
                func_80015758((SVECTOR *)box, &pos);
            }
            if (Heroes[arg0].hero_object != NULL) {
                n = ((NAS **)Heroes[arg0].hero_object)[-1];
                obj = n->Engine;
                if (obj != NULL) {
                    rad3 = 0x80;
                    hi3 = obj->unk10.y;
                    lo3 = hi3 - 0x100;
                }
            }
            other = Heroes;
            i = 0;
            self = &Heroes[arg0];
            do {
                tbl = D_80068BD4;
                if (arg0 != i && !(other[i].hero_status & 0xF800)) {
                    if (self->hero_move->forward_speed != 0) {
                        push = 1;
                    } else {
                        push = other[i].hero_move->forward_speed == 0;
                    }
                    if (D_8005A5DC < 4) {
                        if (D_8005A5DC >= 2) {
                            push = 1;
                        }
                    }
                    e = other[i].hero_eng;
                    if (e != NULL) {
                        rec = func_80025A48(e, 0x40);
                        if (rec != NULL) {
                            bot = e->unk10.y + rec->unk2;
                            top = e->unk10.y + rec->height;
                            if (push != 0) {
                                r = rec->radius;
                                r += rad;
                                rr = r * r;
                                if (lo < bot && top < hi) {
                                    dx = e->unk10.x - eng->unk10.x;
                                    dz = e->unk10.z - eng->unk10.z;
                                    d = dx * dx + dz * dz;
                                    if (d < rr) {
                                        d = csqrt(d) >> 6;
                                        if (d < r) {
                                            ang = get_angle(dx, dz) & 0xFFF;
                                            d = r - d + 1;
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                            hit = 1;
                                            if (self->hero_pu != 0) {
                                                (*(void (**)(s32, s32))D_8009D6E8.pad60)(arg0, i);
                                            }
                                            if (other[i].hero_pu != 0) {
                                                (*(void (**)(s32, s32))D_8009D6E8.pad60)(i, arg0);
                                            }
                                        }
                                    }
                                }
                            }
                            if (rad2 != 0 && hi2 < bot && top < lo2) {
                                dx = e->unk10.x - pos.vx;
                                dz = e->unk10.z - pos.vz;
                                r = rec->radius;
                                r += rad2;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                            if (rad3 != 0 && lo3 < bot && top < hi3) {
                                dx = e->unk10.x - obj->unk10.x;
                                dz = e->unk10.z - obj->unk10.z;
                                r = rec->radius;
                                r += rad3;
                                d = dx * dx + dz * dz;
                                rr = r * r;
                                if (d < rr) {
                                    d = csqrt(d) >> 6;
                                    if (d < r) {
                                        hit = 1;
                                        ang = get_angle(dx, dz) & 0xFFF;
                                        d = r - d + 1;
                                        if (push != 0) {
                                            eng->unk10.x -= (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            eng->unk10.z -= (d * tbl[ang].unk2) >> 12;
                                        } else {
                                            e->unk10.x += (d * tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                            e->unk10.z += (d * tbl[ang].unk2) >> 12;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                i++;
            } while (i < 4);
            own = NULL;
            n = D_8005AA2C;
            if (Heroes[arg0].hero_object != NULL) {
                own = ((NAS **)Heroes[arg0].hero_object)[-1];
            }
            rad = 0x78;
            push = Heroes[arg0].hero_move->forward_speed != 0;
            if (D_8005A5DC < 4) {
                if (D_8005A5DC >= 2) {
                    push = 1;
                }
            }
            for (; n != NULL; n = n->Solid.Child) {
                if (!(n->Class & 0x200)) continue;
                if (n == own) continue;
                e = n->Engine;
                if (e == NULL) continue;
                if (e == eng) continue;
                rec = func_80025A48(e, 0x40);
                if (rec == NULL) continue;
                bot = e->unk10.y + rec->unk2;
                top = e->unk10.y + rec->height;
                if (lo < bot && top < hi) {
                    dx = e->unk10.x - eng->unk10.x;
                    dz = e->unk10.z - eng->unk10.z;
                    r = rec->radius;
                    r += rad;
                    d = dx * dx + dz * dz;
                    rr = r * r;
                    if (d < rr) {
                        d = csqrt(d) >> 6;
                        if (d < r) {
                            hit = 1;
                            a = get_angle(dx, dz);
                            ang = a & 0xFFF;
                            if (!(a & 0xFF)) {
                                ang -= 8 - Random(0x10);
                            }
                            d = r - d + 1;
                            if (push != 0) {
                                eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
                                eng->unk10.z -= (d * D_80068BD4[ang & 0xFFF].unk2) >> 12;
                            }
                            t = FindExtension(n, 0x1000);
                            if (t != NULL && (t->unk1C & 8)) {
                                t->unk16 = arg0;
                                t->unk1E |= 8;
                            }
                        }
                    }
                }
            }
        }
    }
    return hit;
}

static inline s32 Chk5CD0_A(s32 arg0) {
    s32 ret;
    u16 st;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk5CD0_B(s32 arg0) {
    s32 ret;

    if (D_80051338[arg0].unk54 & 0x4000) {
        SetHeroAction((HeroInfo *)arg0, 0x78);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk5CD0_C(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_move->forward_speed != 0) {
        SetHeroAction((HeroInfo *)arg0, 0x72);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void func_800B5CD0(s32 arg0) {
    if (!Chk5CD0_A(arg0)) {
        if (!Chk5CD0_B(arg0)) {
            if (!Chk5CD0_C(arg0)) {
                if (Heroes[arg0].hero_move->velocity_calc == 3) {
                    SetHeroSeq((HeroInfo *)arg0, 0x61);
                }
                func_8008502C(arg0);
                if (D_8005A5DC < 4) {
                    if (D_8005A5DC >= 2) {
                        func_800B52C8(arg0);
                    }
                }
                func_800B51C8(arg0);
                Heroes[arg0].hero_hit = 0;
            }
        }
    }
}

static inline s32 Chk5E84_C(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction((HeroInfo *)arg0, 0x71);
        return 1;
    }
    return 0;
}

void func_800B5E84(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *e;

    if (!Chk5CD0_A(arg0)) {
        if (!Chk5CD0_B(arg0)) {
            if (!Chk5E84_C(arg0)) {
                base = Heroes;
                h = base + arg0;
                h->hero_move->velocity_enable = 1;
                func_800B52C8(arg0);
                switch (func_8008502C(arg0)) {
                case 8:
                    e = h->hero_eng;
                    if (h->hero_pu == 0x15) {
                        PlaySfx(0x41E, NULL, NULL, 0x1000, 0x1E00, 0);
                        break;
                    }
                    switch (h->hero_name) {
                    case 1: PlaySfx(0xA, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 2: PlaySfx(0x3C, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 3: PlaySfx(0x6E, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 4: PlaySfx(0xA0, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 7: PlaySfx(0x104, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 8: PlaySfx(0xD2, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 5: PlaySfx(0x136, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    case 6: PlaySfx(0x168, NULL, &e->unk10, 0x1000, 0x1E00, 0); break;
                    }
                    break;
                case 9:
                    if (h->hero_pu == 0x15) {
                        break;
                    }
                    switch (h->hero_name) {
                    case 1: PlaySfx(0xB, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 2: PlaySfx(0x3D, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 3: PlaySfx(0x6F, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 4: PlaySfx(0xA1, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 7: PlaySfx(0x105, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 8: PlaySfx(0xD3, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 5: PlaySfx(0x137, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    case 6: PlaySfx(0x169, NULL, NULL, 0x1000, 0x1E00, 0); break;
                    }
                    break;
                }
                func_800B51C8(arg0);
            }
        }
    }
}

void func_800B6168(s32 arg0) {
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
    SetHeroSeq((HeroInfo *)arg0, 0x65);
    idx = 0;
    if (D_8005A614[arg0 + 0x5C] == 0) {
        idx = h->hero_name;
    }
    h->hero_move->jump_vel = D_8009D6E8.unk10[idx].vel;
    h->hero_move->jump_max = D_8009D6E8.unk10[idx].max;
    h->hero_move->jump_acc = D_8009D6E8.unk10[idx].acc;
    h->hero_move->jump_dec = D_8009D6E8.unk10[idx].dec;
    h->hero_move->jump_dir = -1;
}

void func_800B627C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;
    EXTengine *p;

    h->hero_move->velocity_enable = 1;
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
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x66);
}

void func_800B6350(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *e;

    if (func_80025BBC(h->hero_eng) != 0) {
        SetHeroAction((HeroInfo *)arg0, 0x7B);
    } else if (!(h->hero_ctrl & 8)) {
        e = h->hero_eng;
        if (World_floor_level < e->unk10.y + h->hero_sub->unk08) {
            if (D_8005B5B8 == 0x7FFFFFFF) {
                SetHeroAction((HeroInfo *)arg0, 0x79);
            } else {
                e->unk10.y = World_floor_level;
                h->hero_sub->unk08 = 0;
            }
        }
    }
    func_800B52C8(arg0);
    func_8008502C(arg0);
}

void func_800B643C(s32 arg0) {
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
    if (h->hero_last_mode != 0x73) {
        SetHeroSeq((HeroInfo *)arg0, 0x66);
    }
    PlaySfx(0x410, NULL, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_800B65B0(s32 arg0) {
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
    SetHeroSeq((HeroInfo *)arg0, 0x67);
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

static inline s32 Chk66A4_A(s32 arg0) {
    s32 ret;
    u16 st;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_B(s32 arg0) {
    s32 ret;

    if (D_80051338[arg0].unk54 & 0x4000) {
        SetHeroAction((HeroInfo *)arg0, 0x78);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

static inline s32 Chk66A4_C(s32 arg0) {
    s32 ret;

    if (Heroes[arg0].hero_move->forward_speed != 0) {
        SetHeroAction((HeroInfo *)arg0, 0x72);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void func_800B66A4(s32 arg0) {
    if (!Chk66A4_A(arg0)) {
        if (!Chk66A4_B(arg0)) {
            if (!Chk66A4_C(arg0)) {
                func_800B52C8(arg0);
                func_8008502C(arg0);
                func_800B51C8(arg0);
            }
        }
    }
}

void func_800B67D8(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base;

    func_80027F7C(arg0, 0xA, 0xF0);
    func_800BA694(arg0);
    base = Heroes;
    h = &base[arg0];
    h->hero_move->velocity_enable = 0;
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    SetHeroSeq((HeroInfo *)arg0, 0x68);
    PlaySfx(D_800BB95C[h->hero_name - 1][Random(4)], 0, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
}

void func_800B68B8(s32 arg0) {
    u8 *g;

    if (func_8008502C() == 8 && ((g = D_8005A614), (*(u16 *)(g + 0x32) & 0x80)) && g[arg0 + 0x5C] == 0 &&
        !(*(s32 *)(g + 4) & 2)) {
        HeroInfo *base = Heroes;
        HeroInfo *h = &base[arg0];

        h->hero_immunity = 0;
        h->hero_hit += 0x28;
        SetHeroSeq((HeroInfo *)arg0, 0x61);
        if (D_800BC2D0[arg0][0] == 0) {
            SetHeroAction((HeroInfo *)arg0, 0x76);
        } else {
            SetHeroAction((HeroInfo *)arg0, 0x71);
        }
    }
    func_800B51C8(arg0);
}

void func_800B69AC(s32 arg0) {
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
    SetHeroSeq(arg0, 0x61);
    h->hero_hit = 0x3C;
    h->hero_status |= 0x2000;
}

void func_800B6AAC(s32 arg0) {
    HeroInfo *h;
    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_hit = 0;
    h->hero_immunity = 0;
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
    D_800BC2D0[arg0][0] = 0;
    func_800898EC(arg0);
}

void func_800B6B84(s32 arg0) {
    HeroInfo *base;
    HeroInfo *h;
    u16 st;
    s32 ret;

    st = Heroes[arg0].hero_status;
    if (st & 0x200) {
        SetHeroAction((HeroInfo *)arg0, 0x75);
        ret = 1;
    } else if (st & 0x400) {
        SetHeroAction((HeroInfo *)arg0, 0x74);
        ret = 1;
    } else {
        ret = 0;
    }
    if (ret != 0) {
        base = Heroes;
        h = &base[arg0];
        if ((u32)(h->hero_name - 1) < 8) {
            h->hero_sub->unk6C = (s32)(D_80052A54 + D_8009D6EC[*(volatile u16 *)&h->hero_name] * 0x38);
        } else {
            h->hero_sub->unk6C = 0;
        }
    } else {
        func_80089BE8(arg0);
    }
}

void func_800B6C84(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
    h->hero_frozen = 1;
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
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    {
        HeroInfo *b2 = Heroes;
        HeroInfo *g = b2 + arg0;
        g->hero_sub->unk08 = 0;
        g->hero_pu = 0;
        g->hero_pu_timer = 0;
    }
    func_80085718(arg0, 0);
    SetHeroSeq(arg0, 0x6B);
    func_80089FA4(arg0);
    D_800AD12C = 1;
    D_800BC444 = 1;
}

void func_800B6E84(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800B6F6C(s32 arg0) {
    HeroInfo *h;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    h->hero_move->velocity_enable = 0;
    h->hero_move->jump_max = 0;
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
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    {
        HeroInfo *b2 = Heroes;
        HeroInfo *g = b2 + arg0;
        SLONG_XYZ *v;
        void *anim;
        NAS *nas;
        s32 *ext;

        g->hero_sub->unk08 = 0;
        g->hero_pu = 0;
        g->hero_pu_timer = 0;
        SetHeroSeq(arg0, 0x61);
        v = &g->hero_eng->unk10;
        anim = D_80053F8C;
        nas = CreateNAS(v->x, v->y, v->z, 0x1605, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext[3] = 0;
                ext[6] = 0x100;
                if (anim != NULL) {
                    ext[8] = 5;
                    nas->Engine->sub->unk6C = anim;
                } else {
                    ext[8] = 1;
                }
                ext[10] = 0;
                nas->InitDelete(nas);
            }
        }
    }
    {
        HeroInfo *b3 = Heroes;
        HeroInfo *k = b3 + arg0;
        EXTengine *eng = k->hero_eng;

        eng->unk5C = 1;
        eng->unk2C.x = 0x1000;
        eng->unk2C.y = 0x1000;
        eng->unk2C.z = 0x1000;
        eng->Engine->sub->flags = 0;
        func_80026980(k->hero);
    }
}

void func_800B722C(s32 arg0) {
    HeroInfo *h;
    EXTengine *eng;
    s32 dy;

    HeroInfo *base = Heroes;

    h = base + arg0;
    if (h->hero_health < 2) {
        h->hero_health = 2;
    }
    h->hero_eng->unk10.y = World_floor_level - 0x200;
    dy = ((s32 (*)(EXTengine *))func_80025F44)(h->hero_eng);
    if (dy != 0x7FFFFFFF) {
        h->hero_eng->unk10.y -= dy;
    } else {
        h->hero_eng->unk10.y = World_floor_level;
    }
    func_8008502C(arg0);
    eng = Heroes[arg0].hero_eng;
    eng->unk5C = 1;
    if (eng->unk2C.x > 0x12C) {
        eng->unk2C.x -= 0x12C;
        eng->unk2C.y -= 0x12C;
        eng->unk2C.z -= 0x12C;
    } else {
        eng->unk2C.x = 3;
        eng->unk2C.y = 3;
        eng->unk2C.z = 3;
    }
}

static inline s32 BarrelNearHero(s32 x, s32 z, s32 r) {
    s32 i;
    s32 d;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_name == 9) {
            continue;
        }
        if (Heroes[i].hero_status & 0xC000) {
            continue;
        }
        d = x - Heroes[i].hero_eng->unk10.x;
        if (d >= -r) {
            if (d <= r) {
                d = z - Heroes[i].hero_eng->unk10.z;
                if (d > -r) {
                    if (d < r) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

void PlotBarrelGenerator(NAS *nas) {
    s32 i;
    s32 n;
    s32 x;
    s32 z;

    if (D_800BC444 != 0) {
        KillNAS(nas);
        return;
    }
    if (nas->UserInfo.flags != 0) {
        nas->UserInfo.flags--;
        return;
    }
    n = 0;
    for (i = 0; i < 8; i++) {
        if (D_800BC424[i] != NULL) {
            n++;
        }
    }
    nas->UserInfo.flags = n * 5 + 1;
    if (n >= 8) {
        return;
    }
again:
    do {
        x = Random(8) - 4;
        x <<= 9;
        z = Random(8) - 4;
        z <<= 9;
        if ((csqrt(x * x + z * z) >> 6) > 0x800) {
            goto again;
        }
    } while (BarrelNearHero(x, z, 0x200) >= 0);
    for (i = 0; i < 8; i++) {
        if (D_800BC424[i] != NULL && D_800BC424[i]->Engine->unk10.x == x &&
            D_800BC424[i]->Engine->unk10.z == z) {
            goto again;
        }
    }
    CreateNAS(x, 0, z, 0x1D01, 0x11);
}

void InitBarrelCH(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;
    s32 i;
    s32 n;

    ext = AddExt(nas, 0x44, 0x10000001);
    nas->FUX = &ext->ext;
    ((s32 *)ext)[0x30 / 4] = 1;
    ((s32 *)ext)[0x40 / 4] = -0x17;
    ext->unk1C = 0x3E8;
    eng = nas->Engine;
    sub = eng->sub;
    eng->unk2C.x = 0x3E8;
    eng->unk2C.y = 0x3E8;
    eng->unk2C.z = 0x3E8;
    eng->unk5C = 1;
    eng->unk10.y = 0;
    sub->unk6C = D_80053F8C;
    n = ++D_800BC450;
    if ((n & 0x1F) == 0) {
        sub->unk74 = 0x2016;
    } else if ((n & 7) == 0) {
        sub->unk74 = 0x2017;
    } else {
        sub->unk74 = 0x2001;
    }
    nas->UserInfo.status = 0;
    if (nas->Failure != 0) {
        KillNAS(nas);
    } else {
        for (i = 0; i < 8; i++) {
            if (D_800BC424[i] == NULL) {
                D_800BC424[i] = nas;
                break;
            }
        }
    }
}

void PlotBarrelCH(NAS *nas) {
    EXTengine *eng;
    BarrelCHExt *fux;
    EXTengine *he;
    s32 i;
    s32 j;
    s32 bx, bz;
    s32 dx, dz;
    s32 d;
    s32 a;
    SLONG_XYZ *v;
    void *anim;
    NAS *n;
    s32 *ext;

    i = 0;
    eng = nas->Engine;
    fux = (BarrelCHExt *)nas->FUX;
    bx = eng->unk10.x;
    bz = eng->unk10.z;
    do {
        he = Heroes[i].hero_eng;
        dx = he->unk10.x - bx;
        dz = he->unk10.z - bz;
        if ((u32)(dx + 0xFF) < 0x1FF && dz >= -0xFF && dz < 0x100) {
            d = (csqrt(dx * dx + dz * dz) >> 6) - 0x70;
            if (d < 0x70) {
                a = get_angle(dx, dz);
                he->unk10.x += (d * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
                he->unk10.z += (d * D_80068BD4[a & 0xFFF].unk2) >> 12;
            }
        }
        i++;
    } while (i < 4);
    if (fux->growing == 1) {
        fux->scale += 0x3E8;
        if (fux->scale >= 0x1770) {
            fux->scale = 0x1770;
            fux->growing = 0;
        }
        eng->unk2C.x = fux->scale;
        eng->unk2C.y = fux->scale;
        eng->unk2C.z = fux->scale;
        eng->unk5E = 1;
        return;
    }
    if (nas->UserInfo.status != 0) {
        v = &eng->unk10;
        PlaySfx(D_800BB9EC[Random(4)], 0, v, 0x1000, 0x1E00, 0);
        anim = D_80053F8C;
        n = CreateNAS(eng->unk10.x, v->y, v->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext[3] = 0;
                ext[6] = 0x100;
                if (anim != NULL) {
                    ext[8] = 5;
                    n->Engine->sub->unk6C = anim;
                } else {
                    ext[8] = 1;
                }
                ext[10] = 0;
                n->InitDelete(n);
            }
        }
        for (j = 0; j < 8; j++) {
            if (D_800BC424[j] == nas) {
                D_800BC424[j] = NULL;
                break;
            }
        }
        KillNAS(nas);
    }
}

NAS *func_800B7948(s32 arg0) {
    u8 *g = D_8005A614 + arg0;
    EXTengine *he = Heroes[arg0].hero_eng;
    s32 lv = g[0x50];
    Ovl68SpawnOfs *o = &D_800BBA44[--lv];
    s32 r = o->r;
    s32 a = (he->unk1E + o->ang) & 0xFFF;
    s32 dx = (r * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
    s32 dz = (r * D_80068BD4[a].unk2) >> 12;
    s32 x = he->unk10.x + dx;
    s32 y = -o->y;
    s32 z;
    s32 old;
    NAS *nas;
    EXTengine *e;

    if (x < -0x9FF) {
        x = -0x9FF;
    }
    if (x >= 0xA00) {
        x = 0x9FF;
    }
    z = he->unk10.z + dz;
    if (z < -0x9FF) {
        z = -0x9FF;
    }
    if (z >= 0xA00) {
        z = 0x9FF;
    }
    nas = CreateNAS(x, he->unk10.y + y, z, 0x1D02, 0x11);
    if (nas != NULL) {
        {
            CoalPosInner *p = ((CoalGridObj *)nas)->pos;
            s32 gx = p->x;
            s32 gz = p->z;
            s32 idx;

            gx += 0xA00;
            gz += 0xA00;
            gx >>= 7;
            gz >>= 7;
            idx = gz * 40 + gx;
            old = D_800BC4E8[idx];
            D_800BC4E8[idx] = (s32)nas;
            if (old != 0) {
                KillNAS((NAS *)old);
            }
        }
        if (g[0x5C] != 0) {
            nas->UserInfo.status = (nas->UserInfo.status * D_8009D87C[arg0].unk0) / 16;
        }
        e = nas->Engine;
        e->unk1E = he->unk1E;
        e->unk1C = 0x400;
        e->unk5E = 1;
        ((s32 *)nas->FUX)[0x2C / 4] = arg0;
    }
    return nas;
}

void func_800B7B80(CoalFlameEng *eng, CoalFlameFux *fux, s32 id) {
    s32 *grid;
    s32 cx;
    s32 cz;
    s32 i;
    CircleFireOfs *ofs;

    if (D_800BC444 != 0) {
        return;
    }
    i = 0;
    grid = D_800BC4E8;
    ofs = D_800BC1B0;
    cx = eng->x;
    cz = eng->z;
    for (;;) {
        CoalFlameCell *cell;
        CoalFlamePos *pos;
        CoalFlameFux *cf;
        NAS *fire;
        CoalFlameFux *ff;
        s32 x;
        s32 z;
        s32 s;

        if (i >= 8) {
            break;
        }
        x = cx + ofs->x;
        if (x >= -0x9FF) if (x < 0xA00) {
            z = cz + ofs->z;
            if (z >= -0x9FF) if (z < 0xA00) {
                cell = (CoalFlameCell *)CircleFireGridCell(grid, x, z);
                if (cell != NULL) {
                    pos = cell->pos;
                    cf = cell->fux;
                    if (fux->unk1C < 0) {
                        fux->unk1C = 0;
                        fux->unk20 = pos->x;
                        fux->unk24 = pos->z;
                        fux->unk2C = cf->unk2C;
                        s = ((cell->unk5C + 0xF) << 12) / 60;
                        if (s > 0x1000) {
                            s = 0x1000;
                        } else if (s < 0x258) {
                            s = 0x258;
                        }
                        eng->sx = s;
                        eng->sy = s;
                        eng->sz = s;
                        eng->unk5E = 1;
                        grid[CIRCLEFIRE_CELL(x, z)] = 0;
                        KillNAS((NAS *)cell);
                    } else {
                        fire = CreateNAS(cx, 0, cz, 0x1D05, 0x11);
                        if (fire != NULL) {
                            ff = (CoalFlameFux *)fire->FUX;
                            ff->unk2C = cf->unk2C;
                            ff->unk1C = -7;
                            ff->unk20 = pos->x;
                            ff->unk24 = pos->z;
                            if (cf->unk2C == id) {
                                ff->unk28 = fux->unk28;
                            }
                        }
                    }
                }
            }
        }
        ofs++;
        i++;
    }
}

s32 func_800B7DF0(s32 x, s32 z, s32 id, s32 cur, s32 range, s32 delay) {
    CoalChainNAS *nas;
    CoalEng *eng;
    HeroInfo *h;
    s32 total;
    s32 i;
    s32 d;
    s32 pts;
    u16 kind;

    total = 0;
    for (i = 0; i < 8; i++) {
        nas = (CoalChainNAS *)D_800BC424[i];
        if (nas == NULL || nas->unk5C == 1) {
            continue;
        }
        eng = nas->eng;
        d = eng->x - x;
        if (-range < d && d < range) {
            d = eng->z - z;
            if (-range < d && d < range && nas->fux->unk30 != 1) {
                nas->unk5C = 1;
                kind = eng->sub->unk74;
                total += 2;
                if (kind == 0x2016) {
                    pts = 20;
                    PlaySfx(0x755, NULL, (SLONG_XYZ *)&eng->x, 0x1000, 0x1E00, 0);
                } else if (kind == 0x2017) {
                    pts = 10;
                    PlaySfx(0x754, NULL, (SLONG_XYZ *)&eng->x, 0x1000, 0x1E00, 0);
                } else {
                    pts = cur + total;
                    if (pts > 10) {
                        pts = 10;
                    }
                    PlaySfx(D_800BBA0C[id], NULL, (SLONG_XYZ *)&eng->x, 0x1000, 0x1E00, 0);
                }
                if (((CoalChainGame *)D_8005A614)->flags & 0x40) {
                    if (((CoalChainGame *)D_8005A614)->unk6C != 0) {
                        h = &Ovl68Heroes()[id];
                        if (h->hero_score >= pts) {
                            h->hero_score -= pts;
                        } else {
                            Heroes[h->hero_teammate].hero_score -= pts - (u16)h->hero_score;
                            h->hero_score = 0;
                        }
                    } else {
                        Ovl68Heroes()[id].hero_score -= pts;
                    }
                } else {
                    Ovl68Heroes()[id].hero_score += pts;
                }
                func_8008D5D4(eng->x, -300, eng->z, pts, delay);
                delay += 10;
                total += func_800B7DF0(eng->x, eng->z, id, pts, 0x120, delay);
            }
        }
    }
    return total;
}

void PlotCoalFlame(NAS *nas) {
    CoalFlameFux *fux;
    CoalFlameEng *eng;
    s32 t;

    fux = (CoalFlameFux *)nas->FUX;
    eng = (CoalFlameEng *)nas->Engine;
    ((CoalPlayerRec *)D_800BC210)[fux->unk2C].unk20 = 2;
    if (fux->unk1C == 0) {
        if (eng->x < fux->unk20) {
            eng->x += 0x14;
        } else if (eng->x > fux->unk20) {
            eng->x -= 0x14;
        }
        if (eng->z < fux->unk24) {
            eng->z += 0x14;
        } else if (eng->z > fux->unk24) {
            eng->z -= 0x14;
        }
        if ((u32)(fux->unk20 - eng->x + 0x27) < 0x4F
            && (u32)((t = fux->unk24) - eng->z + 0x27) < 0x4F) {
            eng->x = fux->unk20;
            eng->z = fux->unk24;
            fux->unk1C = -7;
        }
    } else if ((t = --nas->UserInfo.user2) <= 0) {
        if (eng->sfx != NULL) {
            StopSfx(eng->sfx);
        }
        KillNAS(nas);
        return;
    }
    if (fux->unk1C < 0) {
        func_800B7B80(eng, fux, fux->unk2C);
    }
    D_800BC48C[fux->unk2C] += func_800B7DF0(eng->x, eng->z, fux->unk2C,
                                            D_800BC48C[fux->unk2C], 0x12C, 0);
    if (D_800BC48C[fux->unk2C] > 10) {
        D_800BC48C[fux->unk2C] = 10;
    }
    ((CoalFlameEng *)D_800BC45C[fux->unk2C])->x = eng->x;
    ((CoalFlameEng *)D_800BC45C[fux->unk2C])->z = eng->z;
    D_800BC46C[fux->unk2C] = 10;
}

void PlotCircleFire(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    s32 cx;
    s32 cz;
    s32 i;
    s32 *grid;

    eng = nas->Engine;
    sub = eng->sub;
    if (++sub->unk74 >= 0x4014) {
        sub->unk74 = 0x4000;
    }
    nas->UserInfo.status = (nas->UserInfo.status + 0xC) & 0xFFF;
    eng->unk10.x = (D_80068BD4[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 4;
    eng->unk10.z = (D_80068BD4[nas->UserInfo.status & 0xFFF].unk2 * 9) >> 4;
    cx = eng->unk10.x;
    eng->unk1E = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->unk5E = 1;
    cz = eng->unk10.z;
    i = 0;
    grid = D_800BC4E8;
    for (;;) {
        NAS *cell;
        NAS *fire;
        CircleFireFux *fux;
        s32 x;
        s32 z;

        if (i >= 8) {
            break;
        }
        x = cx + D_800BC1B0[i].x;
        if (x >= -0x9FF) if (x < 0xA00) {
            z = cz + D_800BC1B0[i].z;
            if (z >= -0x9FF) if (z < 0xA00) {
                cell = CircleFireGridCell(grid, x, z);
                if (cell != NULL) {
                    fux = (CircleFireFux *)cell->FUX;
                    fire = CreateNAS(x, 0, z, 0x1D05, 0x11);
                    if (fire != NULL) {
                        ((CircleFireFux *)fire->FUX)->unk2C = fux->unk2C;
                    }
                    fire = CreateNAS(x, 0, z, 0x1D05, 0x11);
                    if (fire != NULL) {
                        ((CircleFireFux *)fire->FUX)->unk2C = fux->unk2C;
                    }
                    grid[CIRCLEFIRE_CELL(x, z)] = 0;
                    KillNAS(cell);
                }
            }
        }
        i++;
    }
}

void PlotEgg(NAS *nas) {
    EXTengine *eng;
    EggExt *fux;
    subengine *sub;
    SLONG_XYZ *pos;
    SLONG_XYZ *at;
    NAS *n;
    EggPuffExt *ext;
    s32 p;
    s32 i;
    s32 skip;
    s32 lim;
    s32 x;
    s32 z;
    HeroInfo *h;

    eng = nas->Engine;
    sub = eng->sub;
    fux = (EggExt *)nas->FUX;
    if (Clock_Time == 0 || (D_8005A644 & 0x801)) {
        PlaySfx(0x75A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        pos = &eng->unk10;
        p = *D_80058AD0;
        n = CreateNAS(eng->unk10.x, pos->y, pos->z, 0x1605, 0x11);
        if (n != NULL) {
            ext = FindExtension(n, 0x10000000);
            if (ext != NULL) {
                ext->unkC = 0;
                ext->unk18 = 0x100;
                if (p != 0) {
                    ext->unk20 = 5;
                    n->Engine->sub->unk6C = (void *)p;
                } else {
                    ext->unk20 = 1;
                }
                ext->unk28 = 0;
                n->InitDelete(n);
            }
        }
        KillNAS(nas);
        return;
    }
    switch (fux->unk30) {
    case 0x26:
    default:
        if (--nas->UserInfo.user2 > 0) {
            return;
        }
        if (nas->UserInfo.user2 == 0) {
            PlaySfx(0x756, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        eng->unk10.x += fux->unk10;
        fux->unk14 += 0xC;
        eng->unk10.y += fux->unk14;
        if (++sub->unk74 == 0x4091) {
            fux->unk30 = 0x29;
            sub->unk74 = 0x4000;
            eng->unk10.x = 0;
            eng->unk10.y = -0x12C;
        }
        break;
    case 0x29:
        nas->UserInfo.status = (nas->UserInfo.status - 0xA) & 0xFFF;
        eng->unk10.x = (D_80068BD4[(nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 0x53) >> 7;
        eng->unk10.z = (D_80068BD4[nas->UserInfo.status & 0xFFF].unk2 * 0x53) >> 7;
        eng->unk1E = (nas->UserInfo.status + 0x400) & 0xFFF;
        eng->unk5E = 1;
        if (eng->unk10.z > 0) {
            sub->unk68 = 0x100;
        } else {
            sub->unk68 = -0x100;
        }
        if (++sub->unk74 == 0x4006) {
            PlaySfx(0x757, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (sub->unk74 == 0x400F) {
            PlaySfx(0x758, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        }
        if (sub->unk74 == 0x4012) {
            sub->unk74 = 0x4000;
            i = BarrelNearHero(eng->unk10.x, eng->unk10.z, 0x600);
            if (i >= 0 && D_8005A614[i + 0x5C] == 0) {
                PlaySfx(0x759, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
                fux->unk1C = get_angle(Heroes[i].hero_eng->unk10.x - eng->unk10.x,
                                       Heroes[i].hero_eng->unk10.z - eng->unk10.z);
                fux->unk30 = 0x27;
                fux->unk14 = -0x64;
                sub->unk74 = 0x4080;
                eng->unk5E = 1;
            }
        }
        break;
    case 0x27:
        if (++sub->unk74 == 0x4091) {
            sub->unk74 = 0x4090;
        }
        fux->unk14 += 0xC;
        eng->unk10.y += fux->unk14;
        eng->unk10.x += (D_80068BD4[(fux->unk1C - 0x400) & 0xFFF].unk2 * 5) >> 8;
        eng->unk10.z += (D_80068BD4[fux->unk1C & 0xFFF].unk2 * 5) >> 8;
        if (eng->unk10.y > 0) {
            PlaySfx(0x75A, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
            i = BarrelNearHero(eng->unk10.x, eng->unk10.z, 0x280);
            if (i >= 0) {
                if (D_800BC444 == 0) {
                    HeroInfo *hb = Heroes;

                    h = &hb[i];
                    if (h->hero_immunity == 0 && !(h->hero_status & 0xFA00)) {
                        SetHeroAction((HeroInfo *)i, (u16)D_8009E0D8);
                        func_80027F7C(i, 0xA, 0xF0);
                    }
                }
                if (D_8005A614[i + 0x5C] == 0) {
                    D_800BC2D0[i][0] = 0;
                }
            }
            skip = 0x63;
            lim = 0x280;
            i = 0;
            x = eng->unk10.x;
            z = eng->unk10.z;
            for (;;) {
                s32 d;

                if (i >= 4) {
                    break;
                }
                if (skip != i) {
                    d = x - Heroes[i].hero_eng->unk10.x;
                    if (d >= -0x280 && d <= lim) {
                        d = z - Heroes[i].hero_eng->unk10.z;
                        if (d >= -0x280 && d <= lim && D_800BC444 == 0 && Heroes[i].hero_immunity == 0 &&
                            !(Heroes[i].hero_status & 0xFA00)) {
                            SetHeroAction((HeroInfo *)i, (u16)D_8009E0D8);
                            func_80027F7C(i, 0xA, 0xF0);
                        }
                    }
                }
                i++;
            }
            at = &eng->unk10;
            p = *D_80058AD0;
            n = CreateNAS(eng->unk10.x, at->y, at->z, 0x1605, 0x11);
            if (n != NULL) {
                ext = FindExtension(n, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = 0;
                    ext->unk18 = 0x100;
                    if (p != 0) {
                        ext->unk20 = 5;
                        n->Engine->sub->unk6C = (void *)p;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    n->InitDelete(n);
                }
            }
            KillNAS(nas);
            CreateNAS(0, 0, 0, 0x1D08, 0x11);
        }
        break;
    }
}

void PlotCoalFlameSFX(CoalSfxObj *obj) {
    CoalSfxInner *in = obj->unk54;

    if (D_800BC46C[obj->idx] > 0) {
        if (D_800BC47C[obj->idx] == 0) {
            PlaySfx(D_800BB9FC[Random(4)], &in->voice, &in->pos, 0x1000, 0x1E00, 0);
        } else {
            D_800BC46C[obj->idx]--;
            if (D_800BC46C[obj->idx] <= 0) {
                if (in->voice != 0) {
                    StopSfx(in->voice);
                }
                D_800BC48C[obj->idx] = 0;
            }
        }
    }
    D_800BC47C[obj->idx] = D_800BC46C[obj->idx];
}

void PlotPUGenerator_CH(NAS *nas) {
    s32 i;
    s32 j;
    s32 x;
    s32 z;
    NAS *pu;
    subengine *sub;

    if (D_800BC444 != 0) {
        KillNAS(nas);
        return;
    }
    if (nas->UserInfo.flags != 0) {
        nas->UserInfo.flags--;
        return;
    }
    nas->UserInfo.flags = 0x258;
    for (i = 0;; i++) {
        if (i >= 1) {
            return;
        }
        if (D_800BC420[i] != NULL) {
            continue;
        }
    again:
        do {
            x = (Random(0xE) - 7) << 8;
            z = (Random(0xE) - 7) << 8;
        } while (BarrelNearHero(x, z, 0x200) >= 0);
        for (j = 0; j < 8; j++) {
            if (D_800BC424[j] != NULL && D_800BC424[j]->Engine->unk10.x == x &&
                D_800BC424[j]->Engine->unk10.z == z) {
                goto again;
            }
        }
        pu = CreateNAS(x, 0, z, 0x1D04, 0x11);
        if (pu != NULL) {
            D_800BC420[i] = pu;
            sub = pu->Engine->sub;
            sub->unk6C = D_80052F5C;
            sub->unk74 = 0x200C;
            pu->UserInfo.status = Random(2) + 1;
            return;
        }
    }
}

void InitPickupCH(NAS *nas) {
    EXTengine *eng;
    EXTengine *e;
    PickupCHExt *ext;

    if (nas->Failure != 0) {
        KillNAS(nas);
        return;
    }
    nas->UserInfo.user2 = 0;
    nas->UserInfo.user1 = 5;
    ext = AddExt(nas, 0x44, 0x10000006);
    ext->unk14 = 0;
    eng = nas->Engine;
    nas->FUX = (EXT *)ext;
    eng->unk5D = 1;
    e = AddExtEngine(nas, 1);
    if (e != NULL) {
        e->unk10.x = eng->unk10.x;
        e->unk10.y = 0;
        e->unk10.z = eng->unk10.z;
        e->sub->unk51 = 2;
        e->sub->unk6C = D_80052F5C;
        e->sub->unk74 = 0x2001;
        e->sub->flags |= 0x40000041;
        e->sub->unk76 = 0x800;
        e->sub->unk50 = 1;
        e->sub->unk52 = 1;
    }
    PlaySfx(0x3FA, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
}

void PlotPickupCH(NAS *nas) {
    PickupCHExt *ext = (PickupCHExt *)nas->FUX;
    EXTengine *eng = nas->Engine;
    HeroInfo *h;
    EXTengine *he;
    EXTengine *p;
    subengine *sub;
    s32 i;
    s32 hero;
    s32 dx;
    s32 dz;
    s32 x;
    s32 z;
    s32 lim;
    s32 status;

    ext->unk14 += 8;
    eng->unk10.y += ext->unk14;
    if (eng->unk10.y >= 0) {
        eng->unk10.y = -1;
        ext->unk14 = -0x28;
        eng->unk10.y -= 0x28;
    }
    lim = 0x100;
    x = eng->unk10.x;
    z = eng->unk10.z;
    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) {
            continue;
        }
        if (h->hero_status & 0xC000) {
            continue;
        }
        he = h->hero_eng;
        dx = x - he->unk10.x;
        if (dx < -0x100) {
            continue;
        }
        if (dx > lim) {
            continue;
        }
        dz = z - he->unk10.z;
        if (dz > -lim) {
            if (dz < 0x100) {
                hero = i;
                goto found;
            }
        }
    }
    hero = -1;
found:
    if (hero < 0) {
        return;
    }
    if (D_800BC444 != 0) {
        for (i = 0; i < 1; i++) {
            if (D_800BC420[i] == nas) {
                D_800BC420[i] = NULL;
                break;
            }
        }
    } else {
        PlaySfx(0x439, NULL, &eng->unk10, 0x1000, 0x1E00, 0);
        status = nas->UserInfo.status;
        sub = NULL;
        func_800BA694(hero);
        p = Heroes[hero].hero_powerup;
        if (p != NULL) {
            sub = p->sub;
        }
        if (status < 3) {
            if (status != 0) {
                if (sub != NULL) {
                    sub->unk6C = D_80052F5C;
                    sub->unk74 = 0x200C;
                    sub->flags |= 0x8000;
                    p->flags |= 0x8000;
                }
                D_800BC2D0[hero][0x14] = status;
            }
        }
        for (i = 0; i < 1; i++) {
            if (D_800BC420[i] == nas) {
                D_800BC420[i] = NULL;
                break;
            }
        }
    }
    KillNAS(nas);
}

void func_800B94FC(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    s32 free;
    s32 m;
    u8 *g;

    free = -1;
    for (i = 0; i < 4; i++) {
        if (D_800BC2D0[i][0] == 0) {
            free = i;
        }
    }
    if (free != -1) {
        D_8005A614[0x60] = 0;
        *(u16 *)(D_8005A614 + 0x30) |= arg1;
        for (i = 0; i < 4; i++) {
            if (D_8005A614[i + 0x5C] != 0) {
                func_80026980(Heroes[i].hero);
                Heroes[i].hero_status |= 0x400;
            } else if (i != free) {
                Heroes[i].hero_status |= 0x100;
            }
        }
        D_800AD12C = 1;
    } else if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (m = 0; m < 4; m++) {
                if (!((taken >> m) & 1)) {
                    if (Heroes[m].hero_score > Heroes[best].hero_score || best == -1) {
                        best = m;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void func_800B9A50(s32 arg0, u16 arg1) {
    s32 taken;
    s32 j;
    s32 i;
    s32 best;
    s32 k;
    u8 *g;

    if (arg0 == -1) {
        taken = 0;
        for (j = 0; j < 4; j++) {
            best = -1;
            for (i = 0; i < 4; i++) {
                if (!((taken >> i) & 1)) {
                    if (Heroes[i].hero_score > Heroes[best].hero_score || best == -1) {
                        best = i;
                    }
                }
            }
            D_8005A614[j + 0xA7] = best;
            taken |= 1 << best;
        }
        g = D_8005A614;
        if (*(s32 *)(g + 0x6C) == 0) {
            HeroInfo *hb;
            HeroInfo *a;
            s32 n;

            if (g[g[0xA7] + 0x5C] != 0
                || (hb = Heroes, a = &hb[g[0xA7]], a->hero_score == hb[g[0xA8]].hero_score)) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                for (n = 0; n < 4; n++) {
                    if (D_8005A614[0xA7] == n) {
                        hb[n].hero_status |= 0x200;
                    } else {
                        hb[n].hero_status |= 0x400;
                    }
                }
            }
        } else {
            HeroInfo *a;
            HeroInfo *b;
            HeroInfo *ta;
            HeroInfo *tb;
            HeroInfo *hb;
            s32 sa;
            s32 sb;

            InitBigScore();
            hb = Heroes;
            a = &hb[*(s16 *)(g + 0x48)];
            b = &hb[*(s16 *)(g + 0x4A)];
            ta = &hb[a->hero_teammate];
            tb = &hb[b->hero_teammate];
            sa = a->hero_score + ta->hero_score;
            sb = b->hero_score + tb->hero_score;
            if (sa == sb) {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            } else if (sb < sa) {
                *(u16 *)(g + 0x30) |= 0x800;
                if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                    g[0x60] = a->hero_name;
                    g[0x61] = ta->hero_name;
                    a->hero_status |= 0x200;
                    ta->hero_status |= 0x200;
                    b->hero_status |= 0x400;
                    tb->hero_status |= 0x400;
                } else {
                    g[0x60] = b->hero_name;
                    g[0x61] = tb->hero_name;
                    b->hero_status |= 0x200;
                    tb->hero_status |= 0x200;
                    a->hero_status |= 0x400;
                    ta->hero_status |= 0x400;
                }
            } else {
                D_8005A614[0x60] = 0;
                *(u16 *)(D_8005A614 + 0x30) |= arg1;
                for (k = 0; k < 4; k++) {
                    if (D_8005A614[k + 0x5C] != 0) {
                        func_80026980(Heroes[k].hero);
                        Heroes[k].hero_status |= 0x400;
                    } else {
                        Heroes[k].hero_status |= 0x100;
                    }
                }
            }
        }
    }
}

void func_800B9ED8(s32 arg0, u16 arg1) {
    s32 found;
    s32 fail;
    u8 *g;
    s32 i;

    found = -1;
    fail = 0;
    g = D_8005A614;
    if (*(s32 *)(g + 0x6C) != 0) {
        HeroInfo *hb = Heroes;
        HeroInfo *a;
        HeroInfo *ta;
        HeroInfo *b;
        s32 sa;
        s32 sb;

        a = &hb[*(s16 *)(g + 0x48)];
        ta = &hb[a->hero_teammate];
        b = &hb[*(s16 *)(g + 0x4A)];
        sa = a->hero_score + ta->hero_score;
        sb = b->hero_score + hb[b->hero_teammate].hero_score;
        if (sa == 0) {
            if (g[*(s16 *)(g + 0x48) + 0x5C] == 0) {
                D_8005A614[0x60] = a->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        } else if (sb == 0) {
            if (g[*(s16 *)(g + 0x4A) + 0x5C] == 0) {
                D_8005A614[0x60] = b->hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        hb[i].hero_status |= 0x400;
                    } else {
                        hb[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (!Heroes[i].hero_score) {
                found = i;
            }
        }
        if (found != -1) {
            if (D_8005A614[found + 0x5C] == 0) {
                D_8005A614[0x60] = Heroes[found].hero_name;
                *(u16 *)(D_8005A614 + 0x30) |= 0x800;
                Clock_flags |= 0x200;
                for (i = 0; i < 4; i++) {
                    if (D_8005A614[i + 0x5C] != 0) {
                        Heroes[i].hero_status |= 0x400;
                    } else {
                        Heroes[i].hero_status |= 0x200;
                    }
                }
            } else {
                fail = 1;
            }
        }
    }
    if (fail || arg0 == -1) {
        D_8005A614[0x60] = 0;
        *(u16 *)(D_8005A614 + 0x30) |= arg1;
        Clock_flags |= 0x200;
        for (i = 0; i < 4; i++) {
            if (D_8005A614[i + 0x5C] != 0) {
                Heroes[i].hero_status |= 0x400;
            } else {
                Heroes[i].hero_status |= 0x100;
            }
        }
    }
}

void func_800BA220(s32 arg0, u16 arg1) {
    Ovl68RankView *g;
    Ovl68RankView *p;
    HeroInfo *hb;
    HeroInfo *h;
    HeroInfo *top;
    HeroInfo *ta;
    HeroInfo *b;
    HeroInfo *am;
    HeroInfo *bm;
    s32 used;
    s32 i;
    s32 k;
    s32 n;
    s32 j;
    s32 best;
    s32 sa;
    s32 sb;

    if (arg0 != -1) {
        return;
    }
    used = 0;
    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((used >> j) & 1) && (Heroes[j].hero_score > Heroes[best].hero_score || best == -1)) {
                best = j;
            }
        }
        RANK68V->order[i] = best;
        used |= 1 << best;
    }
    g = RANK68V;
    if (g->teams == 0) {
        h = Heroes;
        top = &h[g->order[0]];
        if (top->hero_score == h[g->order[1]].hero_score) {
            g->winner[0] = 0;
            g->unk30 |= arg1;
            for (k = 0; k < 4; k++) {
                Heroes[k].hero_status |= 0x100;
            }
            return;
        }
        p = g;
        p->winner[0] = top->hero_name;
        p->unk30 |= 0x800;
        for (n = 0; n < 4; n++) {
            if (p->order[0] == n) {
                h[n].hero_status |= 0x200;
            } else {
                h[n].hero_status |= 0x400;
            }
        }
        return;
    }
    InitBigScore();
    hb = Heroes;
    ta = &hb[g->team[0]];
    b = &hb[g->team[1]];
    am = &hb[ta->hero_teammate];
    bm = &hb[b->hero_teammate];
    sa = ta->hero_score + am->hero_score;
    sb = b->hero_score + bm->hero_score;
    if (sa == sb) {
        g->winner[0] = 0;
        g->unk30 |= arg1;
        for (k = 0; k < 4; k++) {
            Heroes[k].hero_status |= 0x100;
        }
        return;
    }
    g->unk30 |= 0x800;
    if (sb < sa) {
        g->winner[0] = ta->hero_name;
        g->winner[1] = am->hero_name;
        ta->hero_status |= 0x200;
        am->hero_status |= 0x200;
        b->hero_status |= 0x400;
        bm->hero_status |= 0x400;
    } else {
        RANK68V->winner[0] = b->hero_name;
        RANK68V->winner[1] = bm->hero_name;
        b->hero_status |= 0x200;
        bm->hero_status |= 0x200;
        ta->hero_status |= 0x400;
        am->hero_status |= 0x400;
    }
}

void func_800BA5A0(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (!(*(u16 *)(g + 0x30) & 0x801)) {
        u16 f = *(u16 *)(g + 0x32);

        if (f & 0x80) {
            func_800B94FC(arg0, (u16)arg1);
        } else if (f & 0x20) {
            func_800B9A50(arg0, (u16)arg1);
        } else if (f & 0x40) {
            func_800B9ED8(arg0, (u16)arg1);
        } else {
            func_800BA220(arg0, (u16)arg1);
        }
    }
}

void func_800BA630(NAS *nas) {
    s32 i;

    for (i = 0; i < 1; i++) {
        if (D_800BC420[i] == nas) {
            D_800BC420[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitPUGenerator_CH(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x258;
}

void func_800BA694(s32 arg0) {
    EXTengine *p = Heroes[arg0].hero_powerup;
    subengine *s = NULL;

    if (p != NULL) {
        s = p->sub;
    }
    if (s != NULL) {
        p->flags &= 0xFFFF7FFF;
    }
    D_800BC2D0[arg0][0x14] = 0;
}

void func_800BA70C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *p = h->hero_powerup;
    EXTengine *e;

    if (p != NULL) {
        e = h->hero_eng;
        p->unk10 = e->unk10;
        p->unk5E = 1;
        p->unk10.y -= 0x300;
        p->unk1E = p->unk1E + 0x38;
    }
}

void func_800BA784(s32 arg0, s32 arg1) {
    EXTengine *p;
    subengine *sub;

    sub = NULL;
    func_800BA694(arg0);
    p = Heroes[arg0].hero_powerup;
    if (p != NULL) {
        sub = p->sub;
    }
    if (arg1 < 3) {
        if (arg1 > 0) {
            if (sub != NULL) {
                sub->unk6C = D_80052F5C;
                sub->unk74 = 0x200C;
                sub->flags |= 0x8000;
                p->flags |= 0x8000;
            }
            D_800BC2D0[arg0][0x14] = arg1;
        }
    }
}

void InitCoalFlameSFX(CoalFlameObj *arg0) {
    arg0->unk54->unkC = 0;
}

void InitEgg(NAS *nas) {
    EggExt *ext;
    EXTengine *eng;
    subengine *sub;
    s32 scene;

    ext = AddExt(nas, 0x44, 0x10000005);
    nas->FUX = &ext->ext;
    ext->unk30 = 8;
    ext->unk10 = -0x55;
    ext->unk14 = -0xA0;
    eng = nas->Engine;
    nas->UserInfo.user2 = 0x78;
    sub = eng->sub;
    nas->UserInfo.status = eng->unk10.x;
    eng->unk10.x = 0x600;
    eng->unk10.y = 0x12C;
    eng->unk10.z = 0xA60;
    eng->unk1E = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->unk5E = 1;
    scene = *D_80058AD0;
    sub->unk74 = 0x4080;
    sub->unk68 = 0x3E7;
    sub->unk6C = (void *)scene;
    sub->flags |= 0x2000000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCircleFire(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CircleFireExt *ext;

    ext = AddExt(nas, 0x44, 0x10000004);
    nas->FUX = &ext->ext;
    ext->unk30 = 1;
    eng = nas->Engine;
    nas->UserInfo.status = eng->unk10.x;
    eng->unk10.x = (D_80068BD4[((u16)nas->UserInfo.status - 0x400) & 0xFFF].unk2 * 9) >> 4;
    eng->unk10.z = (D_80068BD4[nas->UserInfo.status & 0xFFF].unk2 * 9) >> 4;
    sub = eng->sub;
    eng->unk1E = (nas->UserInfo.status - 0x400) & 0xFFF;
    eng->unk5E = 1;
    eng->unk10.y = 0;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x4000;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void InitCoalFlame(NAS *nas) {
    EXTengine *eng;
    subengine *sub;
    CoalFlameExt *ext;

    eng = nas->Engine;
    nas->UserInfo.user2 = 0xF;
    ext = AddExt(nas, 0x44, 0x10000003);
    nas->FUX = &ext->ext;
    ext->unk1C = -0xD;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200E;
    eng->unk5C = 1;
    eng->unk2C.x = 0x1000;
    eng->unk2C.y = 0x1000;
    eng->unk2C.z = 0x1000;
    eng->unk5E = 1;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void PlotCoal(CoalNAS *nas) {
    CoalEng *eng = nas->eng;

    if (nas->unk5E == 5) {
        eng->unk5E = 1;
        eng->unk1C += 0x44;
        nas->unk60 += 0xC;
        eng->y += nas->unk60;
        if (eng->y > 0) {
            eng->y = 0;
            eng->sub->unk74 = 0x2002;
            eng->unk1C = 0;
            eng->unk5E = 1;
            nas->unk5E = 0;
        }
    }
    nas->unk5C--;
    if (nas->unk5C == 0) {
        s32 xi = (eng->x + 0xA00) >> 7;
        s32 zi = (eng->z + 0xA00) >> 7;
        s32 idx = zi * 40 + xi;
        D_800BC4E8[idx] = 0;
        KillNAS((NAS *)nas);
    }
}

void InitCoal(NAS *nas) {
    EXTengine *eng;
    subengine *sub;

    nas->FUX = AddExt(nas, 0x44, 0x10000002);
    eng = nas->Engine;
    eng->unk5D = 1;
    nas->UserInfo.flags = 5;
    nas->UserInfo.user1 = 0;
    sub = eng->sub;
    sub->unk6C = D_80053F8C;
    sub->unk74 = 0x200F;
    nas->UserInfo.status = 0x3C;
    if (nas->Failure != 0) {
        KillNAS(nas);
    }
}

void func_800BAC30(NAS *nas) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (D_800BC424[i] == nas) {
            D_800BC424[i] = NULL;
            break;
        }
    }
    KillNAS(nas);
}

void InitBarrelGenerator(NAS *nas) {
    nas->Engine->flags = 0;
    nas->UserInfo.flags = 0x3C;
}

void func_800BAC98(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 5;
    SetHeroAction((HeroInfo *)arg0, 0x77);
    if (hero->hero_hit != 0) {
        hero->hero_hit--;
    } else {
        SetHeroAction((HeroInfo *)arg0, 0x77);
    }
    func_8008502C(arg0);
}

void func_800BAD1C(s32 arg0) {
    HeroInfo *hero;
    HeroInfo *base;
    s32 y;

    base = Heroes;
    hero = base + arg0;
    hero->hero_frozen = 2;
    y = hero->hero_eng->unk10.y + hero->hero_sub->unk08;
    if (World_floor_level + 0x180 < y) {
        SetHeroAction((HeroInfo *)arg0, 0x77);
    }
    func_800B52C8(arg0);
    func_8008502C(arg0);
}

void func_800BADA8(s32 arg0) {
    func_800B52C8(arg0);
    func_8008502C(arg0);
}

void func_800BADD4(s32 arg0) {
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x64);
}

void func_800BAE20(void) {
}

void func_800BAE28(void) {
}

void func_800BAE30(s32 arg0) {
    Heroes[arg0].hero_move->jump_max = 0;
    Heroes[arg0].hero_move->velocity_enable = 1;
    SetHeroSeq((HeroInfo *)arg0, 0x61);
}

s32 func_800BAE84(void) {
    return 0;
}

s32 func_800BAE8C(void) {
    return 1;
}

s32 func_800BAE94(s32 arg0) {
    s32 act;
    u16 st = Heroes[arg0].hero_status;

    act = 0x75;
    if (!(st & 0x200)) {
        act = 0x74;
        if (!(st & 0x400)) {
            return 0;
        }
    }
    SetHeroAction((HeroInfo *)arg0, act);
    return 1;
}

s32 func_800BAF00(s32 arg0) {
    if (!(D_80051338[arg0].unk54 & 0x4000)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x78);
    return 1;
}

s32 func_800BAF58(s32 arg0) {
    if (Heroes[arg0].hero_move->forward_speed == 0) {
        SetHeroAction((HeroInfo *)arg0, 0x71);
        return 1;
    }
    return 0;
}

s32 func_800BAFB4(s32 arg0)
{
  if (Heroes[arg0].hero_move->forward_speed != 0)
  {
    SetHeroAction((HeroInfo *)arg0, 0x72);
    return 1;
  }
  return 0;
}

void func_800BB010(s32 x, s32 z, s32 skip, s32 range) {
    HeroInfo *h;
    s32 d;
    s32 i;
    s32 neg;

    i = 0;
    neg = -range;
    h = Heroes;
loop:
    if (i < 4) {
        if (skip != i) {
            d = x - h->hero_eng->unk10.x;
            do {
                if (d < neg) {
                    goto next;
                }
            } while (0);
            if (d <= range) {
                d = z - h->hero_eng->unk10.z;
                if (d >= neg && d <= range && D_800BC444 == 0 &&
                    h->hero_immunity == 0 && !(h->hero_status & 0xFA00)) {
                    SetHeroAction((HeroInfo *)i, (u16)D_8009E0D8);
                    func_80027F7C(i, 0xA, 0xF0);
                }
            }
        }
    next:
        h++;
        i++;
        goto loop;
    }
}

void func_800BB134(s32 arg0)
{
  HeroInfo *h;
  HeroInfo *new_var;
  if (D_800BC444 == 0)
  {
    new_var = Heroes;
    h = new_var;
    h += arg0;
    if ((h->hero_immunity == 0) && (!(h->hero_status & 0xFA00)))
    {
      SetHeroAction((HeroInfo *) arg0, (u16) D_8009E0D8);
      func_80027F7C(arg0, 0xA, 0xF0);
    }
  }
}

s32 func_800BB1C0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

s32 func_800BB260(s32 arg0) {
    u32 m = D_80051338[arg0].unk54 & 0x2000;
    return m != 0;
}

void func_800BB294(s32 *p, s32 arg1) {
    s32 d = (csqrt(p[4] * p[4] + p[6] * p[6]) >> 6) - arg1;
    s32 a;
    if (d > 0) {
        a = get_angle(p[4], p[6]);
        p[4] -= (d * D_80068BD4[(a - 0x400) & 0xFFF].unk2) >> 12;
        p[6] -= (d * D_80068BD4[a & 0xFFF].unk2) >> 12;
    }
}

void func_800BB36C(void) {
}

void func_800BB374(void) {
}

void func_800BB37C(void) {
}

void func_800BB384(void) {
    D_8005AA70[0] = (s32)func_800B3EFC;
    D_8005AA70[1] = (s32)func_800B4048;
    D_8005AA70[2] = (s32)func_800B4048;
    D_8005AA70[3] = (s32)func_800B4444;
    D_8005AA70[4] = (s32)func_800BB37C;
    D_8005AA70[6] = (s32)func_800BB374;
    D_8005AA70[5] = (s32)func_800BB36C;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8005AA70[10] = (s32)func_800B44C8;
}

void func_800BB3F0(s32 x, s32 y) {
    s32 xi = (x + 0xA00) >> 7;
    s32 yi = (y + 0xA00) >> 7;
    s32 idx = yi * 40 + xi;
    D_800BC4E8[idx] = 0;
}

s32 func_800BB428(s32 x, s32 z) {
    s32 xi;
    s32 zi;
    s32 idx;

    if ((u32)(x + 0x9FF) >= 0x13FF || z >= 0xA00 || z < -0x9FF) {
        return 0;
    }
    xi = (x + 0xA00) >> 7;
    zi = (z + 0xA00) >> 7;
    idx = zi * 40 + xi;
    return D_800BC4E8[idx];
}

void func_800BB488(CoalGridObj *obj) {
    CoalPosInner *p = obj->pos;
    s32 x = p->x;
    s32 z = p->z;
    s32 idx;
    CoalGridObj *old;

    x += 0xA00;
    z += 0xA00;
    x >>= 7;
    z >>= 7;
    idx = z * 40 + x;
    old = D_800BC4E8[idx];

    D_800BC4E8[idx] = obj;
    if (old != NULL) {
        KillNAS((NAS *)old);
    }
}

s32 func_800BB4F8(s32 x, s32 z, s32 range, s32 skip) {
    s32 i;
    s32 neg;
    s32 dx;
    s32 dz;
    HeroInfo *h;
    EXTengine *eng;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (i == skip || h->hero_name == 9 || (h->hero_status & 0xC000)) {
            continue;
        }
        eng = h->hero_eng;
        dx = x - eng->unk10.x;
        if (dx < -range || range < dx) {
            continue;
        }
        dz = z - eng->unk10.z;
        if (-range < dz && dz < range) {
            return i;
        }
    }
    return -1;
}

s32 func_800BB5A0(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    HeroInfo *h;

    for (i = 0; i < 4; i++) {
        h = &Heroes[i];
        if (h->hero_name == 9) continue;
        if (h->hero_status & 0xC000) continue;
        d = x - h->hero_eng->unk10.x;
        if (d < -range || range < d) continue;
        d = z - h->hero_eng->unk10.z;
        if (-range < d && d < range) return i;
    }
    return -1;
}

void func_800BB640(s32 arg0) {
    u8 *temp_v0;

    temp_v0 = (arg0 * 0x30) + D_800BC210;
    (*(s32*)((u8*)(temp_v0) + 0)) = 1;
    (*(s32*)((u8*)(temp_v0) + 4)) = 1;
    (*(s32*)((u8*)(temp_v0) + 8)) = 0;
    (*(s32*)((u8*)(temp_v0) + 24)) = 0;
    (*(s32*)((u8*)(temp_v0) + 28)) = 0;
    (*(s32*)((u8*)(temp_v0) + 32)) = 0;
    (*(s32*)((u8*)(temp_v0) + 36)) = 0;
    (*(s32*)((u8*)(temp_v0) + 40)) = 0x8000;
    (*(s32*)((u8*)(temp_v0) + 44)) = 0x8000;
}

NAS *func_800BB688(s32 x, s32 z, s32 range) {
    s32 i;
    s32 d;
    NAS *nas;
    EXTengine *eng;

    for (i = 0; i < 1; i++) {
        nas = D_800BC420[i];
        if (nas != NULL) {
            eng = nas->Engine;
            d = x - eng->unk10.x;
            if (d < 0) {
                d = -d;
            }
            if (d + (z - eng->unk10.z) < range) {
                return nas;
            }
        }
    }
    return NULL;
}

void func_800BB6FC(s32 arg0, s32 arg1, s32 arg2, EXTengine *arg3, Ovl68Out *arg4) {
    s32 ang;
    s32 n;

    ang = get_angle(arg3->unk10.x - arg1, arg3->unk10.z - arg2);
    arg4->unk0 = 2;
    n = D_800BC2D0[arg0][17];
    arg4->unk4 = (n / 2 + 0x400) / n;
    arg4->unk8 = *(u32 *)((u8 *)D_80051244 + ((ang >> 7) & 0x1C));
    if (D_8009D908 == 0) {
        Random(4);
    }
}

s32 func_800BB7E0(Ovl68Obj *a, Ovl68Pos *b) {
    Ovl68Pos *p = a->pos;
    s32 dx = b->x - p->x;
    s32 dz;
    dx = dx * dx;
    dz = b->z - p->z;
    dz = dz * dz;
    return csqrt(dx + dz) >> 6;
}
