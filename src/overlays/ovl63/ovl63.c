#include "common.h"
#include "bash/main.h"
#include "bash/ovl63.h"
#include "bash/race.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_800B345C(s32 player) {
    s32 i;
    HeroInfo *h;
    s32 *p;
    SLONG_XYZ pos;

    i = 0;
    pos.x = Heroes[player].hero_eng->unk10.x - (0x40 * D_80068BD4[(Heroes[player].hero_eng->unk1E - 0x400) & 0xFFF].unk2 >> 12);
    h = &Heroes[player];
    pos.y = 0;
    p = D_800CA544;
    pos.z = h->hero_eng->unk10.z - (0x40 * D_80068BD4[h->hero_eng->unk1E & 0xFFF].unk2 >> 12);
loop:
    if (i < 6) {
        if (func_800B82FC(*p, &pos, 1, &pos) != -1) {
            h->hero_ai->AIpad &= ~0x20;
            h->hero_ai->AIpad |= 0x2000;
            h->hero_ai->AIpad |= 0x80;
            ((Ovl63Pad *)D_80051338)[player].unk50 = h->hero_ai->AIpad;
            return 1;
        }
        h++;
        h--;
        p++;
        p++;
        p--;
        i++;
        goto loop;
    }
    return 0;
}

s32 func_800B35CC(s32 arg0) {
    s32 start;
    s32 r;
    s32 i;
    s32 j;
    s32 k;
    s32 total;
    s32 result;
    s32 v;
    s32 neg;

    start = Random(4);
    r = Random(0x1000);
    neg = -1;
    i = start;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        if ((i & 3) == arg0) {
            goto next;
        }
        j = i & 3;
        result = 0;
        total = 0;
        if (Heroes[j].hero_status & 0x2000) {
            v = -1;
        } else {
            for (k = 0; k < 4; k++) {
                if (k != arg0 && !(Heroes[k].hero_status & 0x2000)) {
                    total += D_8009D87C[k].unk4 << 7;
                }
            }
            if (total != 0) {
                result = (D_8009D87C[j].unk4 << 19) / total;
            }
            v = result;
        }
        if (r < v) {
            return neg = i & 3;
        }
    next:
        i++;
    }
    return neg;
}

void func_800B372C(s32 player) {
    SLONG_XYZ pos;
    s32 *p;
    s32 i;
    s32 r;
    HeroInfo *h;

    pos.x = Heroes[player].hero_eng->unk10.x - (0x40 * D_80068BD4[(Heroes[player].hero_eng->unk1E - 0x400) & 0xFFF].unk2 >> 12);
    pos.y = 0;
    pos.z = Heroes[player].hero_eng->unk10.z - (0x40 * D_80068BD4[Heroes[player].hero_eng->unk1E & 0xFFF].unk2 >> 12);
    i = 0;
    if (Heroes[player].hero->FUX != NULL) {
        do {
            h = &Heroes[player];
        } while (0);
        p = D_800CA544;
    loop:
        if (i < 6) {
            r = func_800B82FC(*p, &pos, 1, &pos);
            if (r != -1) {
                p++;
                if (!(r & 4)) {
                    h->hero_ai->AIpad |= 0x4000;
                    ((Ovl63Pad *)D_80051338)[player].unk50 = h->hero_ai->AIpad;
                    return;
                }
            } else {
                p++;
            }
            i++;
            goto loop;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3890);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3BC0);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B32B8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B3E60);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B406C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B41DC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B442C);

void func_800B4708(arg0)
s32 arg0;
{
    HeroInfo *hero;
    EXTovl63ai *ext;
    NAS *nas;
    NAS *found;
    s32 angle;
    HeroInfo *base;

    base = Heroes;
    hero = base + arg0;
    ext = FindExtension(hero->hero, 0x20000000);
    if (ext != NULL) {
        hero->hero_ai->AIpad = 0;
        hero->hero_ai->AImode = 1;
        ext->unk18 = func_800B5360;
        nas = func_800C5C64(arg0);
        found = NULL;
        if (nas != NULL) {
            if (func_8008CEB0((u8 *)&nas->Engine->unk10, (u8 *)&hero->hero_eng->unk10) <= 0x270FFF) {
                angle = get_angle(nas->Engine->unk10.x, nas->Engine->unk10.z) & 0xFFF;
                if ((get_angle(hero->hero_eng->unk10.x, hero->hero_eng->unk10.z) & 0xFFF) < angle) {
                    found = nas;
                    goto done;
                }
            }
            found = NULL;
        }
    done:
        if (found != NULL) {
            ext->unk18 = func_800B5660;
            ext->unk14 = D_800C6980[Heroes[arg0].hero_name];
        }
        if (G63->unk34 != 0x17) {
            func_800B442C(arg0);
        }
        ext->unk18(arg0);
        if (G63->unk34 == 0x19) {
            func_800B372C(arg0);
        } else {
            func_800B41DC(arg0);
        }
        if (D_8005A648 == 0x18) {
            func_800B3890(arg0);
            func_800B3BC0(arg0);
            func_800B406C(arg0);
        }
    }
}

void func_800B48EC(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
            if (D_8005A614[player + 0x5C] != 0) {
                h->hero_eng->unk20 += func_800154A0(h->hero_eng->unk20, D_800CA534[player]) >> 3;
            } else {
                h->hero_eng->unk20 += func_800154A0(h->hero_eng->unk20, D_800CA534[player]) >> 2;
            }
        }
        Heroes[player].hero_ctrl &= ~1;
        Heroes[player].hero_eng->unk5E = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4A4C);

s32 func_800B4D64(s32 *arg0, s32 arg1, s32 *arg2)
{
  u8 *base;
  s32 off;
  u8 *p;
  u8 *a2;
  u8 *a3;
  s32 found;
  u8 *new_var;
  s32 n;
  s32 i;
  s32 ang;
  s32 a;
  s32 b;
  s32 r;
  s32 c;
  found = -1;
  if ((*((s32 *) D_80058AD0)) != 0)
  {
    u8 *q = *((u8 **) (((u8 *) D_80058AD0) + 0x10));
    c = arg1 >= (*((s32 *) (((*((s32 *) (q + 0x14))) + 0x14) + q)));
  }
  else
  {
    if (1)
    {
    }
    c = arg1 >= 0;
  }
  if (c)
  {
    return 0;
  }
  base = *((u8 **) (((u8 *) D_80058AD0) + 0x10));
  off = *((s32 *) (base + 0x14));
  p = ((u8 *) (&((s32 *) ((base + 0x14) + off))[arg1 + 1])) + ((s32 *) ((base + 0x14) + off))[arg1 + 1];
  if ((new_var = p) != 0)
  {
    i = 0;
    a2 = p + 0x5C;
    a3 = p;
    ang = get_angle(arg0[0], arg0[2]) & 0xFFF;
    n = *((s32 *) (a3 + 0x38));
    loop:
    if (i < n)
    {
 do { r = *((s32 *) (a3 + 0x64)); a = get_angle(*((s32 *) (a2 - 0x14)), *((s32 *) (a2 - 0xC))) & 0xFFF; b = get_angle(*((s32 *) (a2 - 8)), *((s32 *) a2)) & 0xFFF; if ((a < ang) && (ang < b)) { *arg2 = -r; found = i; } a3 += 0x38; i++; a2 += 0x38; } while (0);
      goto loop;
    }

  }
  return found;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B4EDC);

void func_800B5360(s32 player) {
    Ovl63VelFux *fux;
    SLONG_XYZ pos;
    s32 d;

    if (FindExtension(Heroes[player].hero, 0x20000000) != NULL) {
        fux = (Ovl63VelFux *)Heroes[player].hero->FUX;
        if (fux != NULL) {
            d = (((csqrt((fux->vx * fux->vx >> 8) + (fux->vz * fux->vz >> 8)) >> 14) - 0x50) << 10) / 80;
            if (d < 0) {
                d = 0;
            }
            d += 0xBE8;
            pos.x = Heroes[player].hero_eng->unk10.x - (d * D_80068BD4[(Heroes[player].hero_eng->unk1E - 0x400) & 0xFFF].unk2 >> 12);
            pos.y = 0;
            pos.z = Heroes[player].hero_eng->unk10.z - (d * D_80068BD4[Heroes[player].hero_eng->unk1E & 0xFFF].unk2 >> 12);
            Heroes[player].hero_ai->AIpad |= 0x8000;
            Heroes[player].hero_ai->AIpad |= func_800B4EDC(player);
            Heroes[player].hero_ai->AIpad |= 0x800;
            ((Ovl63Pad *)D_80051338)[player].unk50 = Heroes[player].hero_ai->AIpad;
        }
    }
}

void func_800B5528(s32 player, SLONG_XYZ *pos) {
    HeroInfo *base;
    HeroInfo *hero;
    s32 d;

    base = Heroes;
    hero = base + player;
    d = (((get_angle(hero->hero_eng->unk10.x - pos->x, hero->hero_eng->unk10.z - pos->z) & 0xFFF) -
          hero->hero_eng->unk1E) << 20) >> 20;
    if (d > 500) {
        hero->hero_ai->AIpad |= 0x20;
        hero->hero_ai->AIpad &= ~0x80;
    }
    if (d < -500) {
        hero->hero_ai->AIpad |= 0x80;
        hero->hero_ai->AIpad &= ~0x20;
    }
    hero->hero_ai->AIpad |= 0x800;
    ((Ovl63Pad *)D_80051338)[player].unk50 = hero->hero_ai->AIpad;
}

static inline NAS *func_800B5660_near(s32 arg0) {
    NAS *nas;
    HeroInfo *hero;
    s32 angle;

    nas = func_800C5C64(arg0);
    if (nas != NULL) {
        hero = &Heroes[arg0];
        if (func_8008CEB0((u8 *)&nas->Engine->unk10, (u8 *)&hero->hero_eng->unk10) <= 0x270FFF) {
            angle = get_angle(nas->Engine->unk10.x, nas->Engine->unk10.z) & 0xFFF;
            if ((get_angle(hero->hero_eng->unk10.x, hero->hero_eng->unk10.z) & 0xFFF) < angle) {
                return nas;
            }
        }
    }
    return NULL;
}

void func_800B5660(s32 arg0) {
    s32 *ext;
    NAS *nas;

    ext = FindExtension(Heroes[arg0].hero, 0x20000000);
    if (ext != NULL) {
        nas = func_800B5660_near(arg0);
        if (nas != NULL) {
            func_800B5528(arg0, &nas->Engine->unk10);
            return;
        }
        ext[6] = (s32)func_800B5360;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5778);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B5A70);

void func_800B5D14(void) {
    MATRIX m;
    SVECTOR v;
    Ovl63SpinObj **pp;
    Ovl63SpinObj *o;
    s32 i;
    s32 spd;

    spd = 4;
    if ((u32)(D_800C69AC - 1) < 2) {
        spd = -4;
    }
    if (D_8005A646 & 0x80) {
        spd = -0x20;
    }
    if (D_800C69A4 != NULL) {
        D_800C69A4->rot.vy -= spd;
        v.vx = D_800C69A4->rot.vx;
        v.vy = D_800C69A4->rot.vy;
        v.vz = D_800C69A4->rot.vz;
        RotMatrix(&v, &m);
        D_800C69A4->mat = m;
    }
    i = 0;
    pp = (Ovl63SpinObj **)D_800CA544;
loop:
    if (i < 6) {
        o = *pp;
        if (o != NULL) {
            o->rot.vy -= spd;
            v.vx = o->rot.vx;
            v.vy = o->rot.vy;
            v.vz = o->rot.vz;
            RotMatrix(&v, &m);
            o->mat = m;
        }
        pp++;
        i++;
        goto loop;
    }
    if (D_800CA608 < 0) {
        if (D_800C695C != 0) {
            D_800C69AC = 5;
        }
        if (func_800B5A70(D_800C6B70, D_800C69AC) != 0) {
            D_800C69AC = Random(5);
        }
    } else {
        D_800CA608--;
    }
}

void func_800B5F04(void) {
    Ovl63Barrel *b;
    Ovl63SceneObj *obj;
    s32 n;

    n = 0;
    obj = ((Ovl63Scene *)D_80058AD0)->objs;
    memset(D_800C6F34, 0x3600, 0);
    while (obj != NULL) {
        if (obj->index == 0x5006) {
            b = &D_800C6F34[n];
            b->obj = obj;
            obj->unk51 = 2;
            b->unk4E = 0x80;
            b->unk20 = 0;
            b->unk40 = 0;
            b->unk48 = 0x500;
            b->unk28 = 0x500;
            b->unk4A = 0x500;
            b->unk2A = 0x500;
            b->unk24 = 0;
            b->unk44 = 0;
            b->pos = obj->pos;
            b->unk58 = 1;
            n++;
        }
        obj = obj->next;
    }
    D_800C66F0 = n;
    printf("%d  barrels found\n", n);
}

void func_800B5FFC(Ovl63EaseCam *c, s32 *pos) {
    s32 step;

    if (c->x.cur != c->x.target) {
        if (c->x.target != 0) {
            step = c->x.step;
        } else {
            step = c->x.stepZero;
        }
        if (c->x.cur < c->x.target) {
            c->x.cur += step;
            if (c->x.cur > c->x.target) {
                c->x.cur = c->x.target;
            }
        } else {
            c->x.cur -= step;
            if (c->x.cur < c->x.target) {
                c->x.cur = c->x.target;
            }
        }
    }
    if (c->z.cur != c->z.target) {
        if (c->z.target != 0) {
            step = c->z.step;
        } else {
            step = c->z.stepZero;
        }
        if (c->z.cur < c->z.target) {
            c->z.cur += step;
            if (c->z.cur > c->z.target) {
                c->z.cur = c->z.target;
            }
        } else {
            c->z.cur -= step;
            if (c->z.cur < c->z.target) {
                c->z.cur = c->z.target;
            }
        }
    }
    pos[1] += c->x.cur >> 8;
    pos[3] += c->z.cur >> 8;
}

s32 func_800B6108(Ovl63Barrel *b) {
    EXTengine *e;
    Ovl63SceneObj *obj;
    s32 hit;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 angle;
    s32 r;

    hit = 0;
    obj = b->obj;
    for (i = 0;; i++) {
        if (i >= 4) {
            return hit;
        }
        e = Heroes[i].hero_eng;
        dx = e->unk10.x - obj->pos.x;
        dz = e->unk10.z - obj->pos.z;
        d = dx * dx + dz * dz;
        if (d < 0x24000) {
            d = csqrt(d) >> 6;
            angle = get_angle(dx, dz) & 0xFFF;
            d = 0x180 - d;
            e->unk10.x += d * D_80068BD4[(angle - 0x400) & 0xFFF].unk2 >> 12;
            e->unk10.z += d * D_80068BD4[angle].unk2 >> 12;
            func_800BE7B4(i, b);
            if (Heroes[i].hero_mode != 0x61) {
                r = Random(2);
                if (r == 0) {
                    PlaySfx(0x663, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (r == 1) {
                    PlaySfx(0x664, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction((HeroInfo *)i, 0x5D);
                }
            }
            hit = 1;
        }
    }
}

s32 func_800B62F4(Ovl63Barrel *self) {
    Ovl63Barrel *b;
    Ovl63SceneObj *obj;
    Ovl63SceneObj *o;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 angle;

    b = D_800C6F34;
    i = 0;
    obj = self->obj;
    for (;; i++, b++) {
        if (i >= D_800C66F0) {
            return 1;
        }
        o = b->obj;
        if (b != self && b->unk58 == 1) {
            dx = obj->pos.x - o->pos.x;
            dz = obj->pos.z - o->pos.z;
            d = dx * dx + dz * dz;
            if (d < 0x24000) {
                d = csqrt(d) >> 6;
                angle = get_angle(dx, dz) & 0xFFF;
                d = 0x180 - d;
                obj->pos.x += d * D_80068BD4[(angle - 0x400) & 0xFFF].unk2 >> 12;
                obj->pos.z += d * D_80068BD4[angle].unk2 >> 12;
            }
        }
    }
}

void func_800B6474(void) {
    Ovl63Barrel *b;
    SLONG_XYZ pos;
    s32 i;

    b = D_800C6F34;
    for (i = 0;; b++, i++) {
        Ovl63SceneObj *obj;

        if (i >= D_800C66F0) {
            break;
        }
        obj = b->obj;
        switch (b->unk58) {
        case 1:
            b->unk24 = 0;
            b->unk44 = 0;
            if (func_800B6108(b) != 0) {
                pos = obj->pos;
                pos.y = -0xD2;
                b->unk58 = 0;
                b->unk68 = Random(0x3C) + 0xB4;
                obj->flags &= ~0x8000;
                Ovl63SpawnPuff(&pos, 1, &D_80054F84);
            }
            break;
        case 2:
            pos = obj->pos;
            pos.y = 0;
            if (b->unk68 == 4) {
                Ovl63SpawnPuff(&pos, 9, NULL);
            }
            if (b->unk68-- < 0) {
                obj->flags |= 0x8000;
                b->unk58 = 1;
            }
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B66C4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B6DBC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B7190);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B76A4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B7D94);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B3310);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B82FC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B8890);

void func_800B8D68(void) {
    D_800C692C++;
    if (G63->unk30 & 0x800) {
        D_800C695C = 1;
    }
    if (G63->unk30 & 1) {
        D_800C695C = 1;
    }
    if (G63->unk10 == 0) {
        D_800C6BB0--;
    }
    if (D_800C695C == 0) {
        D_800AD12C = 0;
    } else {
        D_800AD12C = 1;
    }
    func_800C04EC();
    if (G63->unk34 != 0x19) {
        func_800C0CB4();
    }
    if (G63->unk34 == 0x18) {
        func_800B6474();
    }
    if (G63->unk34 == 0x19) {
        func_800B5D14();
    }
    if (G63->unk34 == 0x1A && !(G63->unk32 & 0x80)) {
        D_800C69B4->unk74 = (D_800C69B8 >> 8) + 0x4000;
        if (D_800C69BC == 0) {
            D_800C69B8 += 0x80;
            if (D_800C69B8 >= 0x6000) {
                D_800C69B8 = 0;
            }
        }
        if (D_800C69BC == 1) {
            D_800C69B8 -= 0x19C;
            if (D_800C69B8 <= 0) {
                D_800C69B8 = 0;
                D_800C69BC = 0;
            }
        }
    }
}

void func_800B8F04(s32 arg0) {
    EXTheromove *m;

    m = Heroes[arg0].hero_move;
    if (m != NULL) {
        if (D_8005A648 == 0x19) {
            D_800C66CC = 0x258;
            D_800C66D0 = 0x640;
            D_800C66D4 = 0xC80;
            D_800C66D8 = 0x7D;
            D_800C66DC = 0x190;
            D_800C66E0 = 0x64;
            D_800C66E4 = 0x3E;
            D_800C66E8 = 0x7D;
        }
        m->mass = 0x100;
        m->x_acc = D_800C66CC;
        m->z_acc = D_800C66CC;
        m->z_dec = D_800C66D8;
        m->x_dec = D_800C66D8;
        m->velocity_calc = 3;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B8FD8);

s32 func_800B96B0(s32 arg0, s32 arg1) {
    EXTengine *eng;
    s32 dist;

    eng = Heroes[arg0].hero_eng;
    if (eng != NULL) {
        dist = csqrt(eng->unk10.x * eng->unk10.x + eng->unk10.z * eng->unk10.z) >> 6;
        if (arg1 == 0) {
            if (dist > 0xB00) {
                s32 ang = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
                s32 c = D_80068BD4[(ang - 0x400) & 0xFFF].unk2;
                s32 d = dist - 0xAFC;
                eng->unk10.x -= d * c >> 12;
                eng->unk10.z -= d * D_80068BD4[ang].unk2 >> 12;
                return 1;
            }
        } else {
            if (dist < 0x464) {
                s32 ang = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
                s32 c = D_80068BD4[(ang - 0x400) & 0xFFF].unk2;
                s32 d = dist - 0x460;
                eng->unk10.x -= d * c >> 12;
                eng->unk10.z -= d * D_80068BD4[ang].unk2 >> 12;
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800B980C);

void func_800B9E50(Quat *q) {
    s32 n;
    s32 sw;
    s32 sz;
    s32 sx;
    s32 sy;

    sw = q->v[3] * q->v[3];
    sz = q->v[2] * q->v[2];
    sx = q->v[0] * q->v[0];
    sy = q->v[1] * q->v[1];
    n = ((sx >= 0 ? sx + 0x800 : sx - 0x800) >> 12)
      + ((q->v[1] * q->v[1] >= 0 ? sy + 0x800 : sy - 0x800) >> 12)
      + ((q->v[2] * q->v[2] >= 0 ? sz + 0x800 : sz - 0x800) >> 12)
      + ((q->v[3] * q->v[3] >= 0 ? sw + 0x800 : sw - 0x800) >> 12);
    q->v[0] = (q->v[0] << 12) / n;
    q->v[1] = (q->v[1] << 12) / n;
    q->v[2] = (q->v[2] << 12) / n;
    q->v[3] = (q->v[3] << 12) / n;
}

void func_800BA004(VECTOR *ang, Quat *q) {
    s32 x, y, z;
    s32 sx, cx, sy, cy, sz, cz;
    s32 p, a, b, c, d;
    s32 m0, m1, m2, m3;
    s32 t;

    x = ang->vx >> 1;
    y = ang->vy >> 1;
    z = ang->vz >> 1;
    sx = D_80068BD4[x & 0xFFF].unk2;
    cx = D_80068BD4[(x - 0x400) & 0xFFF].unk2;
    sy = D_80068BD4[y & 0xFFF].unk2;
    cy = D_80068BD4[(y - 0x400) & 0xFFF].unk2;
    sz = D_80068BD4[z & 0xFFF].unk2;
    cz = D_80068BD4[(z - 0x400) & 0xFFF].unk2;
    a = Ovl63FixMul(sx, sz);
    b = Ovl63FixMul(sx, cz);
    c = Ovl63FixMul(cx, sz);
    d = Ovl63FixMul(cx, cz);
    p = sy * c;
    m0 = cy * b;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t -= (cy * b >= 0 ? m0 + 0x800 : m0 - 0x800) >> 12;
    q->v[0] = t;
    p = sy * d;
    m1 = cy * a;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t += (cy * a >= 0 ? m1 + 0x800 : m1 - 0x800) >> 12;
    q->v[1] = t;
    p = sy * b;
    m2 = cy * c;
    t = (p >= 0 ? p + 0x800 : p - 0x800) >> 12;
    t -= (cy * c >= 0 ? m2 + 0x800 : m2 - 0x800) >> 12;
    q->v[2] = t;
    p = sy * a;
    m3 = cy * d;
    p = ((p >= 0 ? p + 0x800 : p - 0x800) >> 12) + ((cy * d >= 0 ? m3 + 0x800 : m3 - 0x800) >> 12);
    q->v[3] = p;
    func_800B9E50(q);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BA278);

void func_800BAD54(s32 player) {
    HeroInfo *h;
    Ovl63ResetFux *f;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    h->hero_eng->unk1C = 0;
    h->hero_eng->unk5E = 1;
    func_800C5400(player);
    f = (Ovl63ResetFux *)h->hero->FUX;
    if (f != NULL) {
        f->unk54 = get_angle(h->hero_eng->unk10.x, h->hero_eng->unk10.z) & 0xFFF;
        f->unk6C = 0x300;
        f->unk60 = 0;
        f->unk80 = 2;
        f->unk7C = 100;
        f->unk88 = 0;
        f->unk24 = 0;
        f->unk90 = 0;
        f->unk144 = 0;
        f->unk78 = D_800C6930;
        if (h->hero_eng->sfx != NULL) {
            StopSfx(h->hero_eng->sfx);
        }
        if (f->unk128 != NULL) {
            StopSfx(f->unk128);
        }
        f->unk18 = 0;
        if (player == 0) {
            PlaySfx(0x650, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 1) {
            PlaySfx(0x651, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 2) {
            PlaySfx(0x652, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (player == 3) {
            PlaySfx(0x653, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        h->hero_eng->unk5E = 1;
        SetHeroSeq((HeroInfo *)player, 0x4B);
        if (G63->unk34 == 0x19) {
            SetHeroSeq((HeroInfo *)player, 0x54);
        }
        h->hero_shadow->sub->flags &= ~0x8000;
        h->hero_eng->sub->unk68 = 0x200;
        h->hero_eng->sub->flags |= 0x02000000;
        if (G63->unk34 == 0x18 && (G63->unk32 & 0x80)) {
            f->unk168 = 150;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BAFC8);

void func_800BB794(s32 player) {
    HeroInfo *h;
    Ovl63Fux40 *f;
    EXTheromove *m;
    HeroInfo *base;
    u16 spd;

    base = Heroes;
    h = &base[player];
    f = (Ovl63Fux40 *)h->hero->FUX;
    if (f != NULL) {
        m = h->hero_move;
        if (m != NULL) {
            spd = D_8009D6F4[h->hero_name * 3];
            m->x_vel = (spd * D_80068BD4[(h->hero_eng->unk1E + 0x400) & 0xFFF].unk2) >> 12;
            m->z_vel = (spd * D_80068BD4[(h->hero_eng->unk1E + 0x800) & 0xFFF].unk2) >> 12;
        }
        f->unk40 = 0;
    }
}

void func_800BB878(s32 player) {
    HeroInfo *h;
    Ovl63Fux40 *f;
    EXTheromove *m;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    h->hero_eng->unk10.y = 0;
    f = (Ovl63Fux40 *)h->hero->FUX;
    if (f == NULL) {
        return;
    }
    m = h->hero_move;
    if (m == NULL) {
        return;
    }
    h->hero_eng->unk10.y = 0;
    if (++f->unk40 > 20) {
        SetHeroAction((HeroInfo *)player, 0x5B);
        if (D_8005A648 == 0x19) {
            SetHeroSeq((HeroInfo *)player, 0x52);
        }
        return;
    }
    if (((Ovl63Pad *)D_80051338)[player].unk50 & 0x8000) {
        m->x_acc = 0xC;
        m->z_acc = 0xC;
        m->x_max = (D_800C66E4 * D_80068BD4[(h->hero_eng->unk1E + 0x400) & 0xFFF].unk2) >> 12;
        m->z_max = (D_800C66E4 * D_80068BD4[(h->hero_eng->unk1E + 0x800) & 0xFFF].unk2) >> 12;
    }
    func_8008502C(player);
    func_800BC984(player);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BBA04);

void func_800BC0A4(s32 arg0) {
    HeroInfo *hero;
    EXTraceshield *fux;
    Ovl63Pad *pad;
    Ovl63Pad *pb;
    HeroInfo *base;

    func_800C5400(arg0);
    base = Heroes;
    hero = &base[arg0];
    fux = (EXTraceshield *)hero->hero->FUX;
    if (fux == NULL) {
        return;
    }
    pb = (Ovl63Pad *)D_80051338;
    pad = &pb[arg0];
    pad->unk50 = 0;
    pad->unk54 = 0;
    if (D_8005A648 != 0x19) {
        func_8008502C(arg0);
    }
    hero->hero_frozen = 2;
    hero->hero_eng->unk5E = 1;
    if (Ovl63JumpAbove(arg0)) {
        Heroes[arg0].hero_eng->unk10.y -= abs(D_800C6930 - Heroes[arg0].hero_eng->unk10.y);
        Heroes[arg0].hero_eng->unk1C -= Heroes[arg0].hero_eng->unk1C >> 1;
        Heroes[arg0].hero_eng->unk1E += 0x100;
    } else {
        Heroes[arg0].hero_eng->unk1C += 0x80;
    }
    if (fux->unkC-- < 0) {
        SetHeroAction((HeroInfo *)arg0, 0x5B);
    }
}

void func_800BC2D0(s32 player) {
    HeroInfo *h;
    Ovl63Fux138 *f;
    EXTheromove *m;
    SLONG_XYZ pos;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    f = (Ovl63Fux138 *)h->hero->FUX;
    if (f != NULL) {
        f->unk138 = 0;
        m = h->hero_move;
        if (m != NULL) {
            pos.x = h->hero_eng->unk10.x + (D_80068BD4[(h->hero_eng->unk1E - 0x400) & 0xFFF].unk2 * 25 >> 9);
            pos.z = h->hero_eng->unk10.z + (D_80068BD4[h->hero_eng->unk1E & 0xFFF].unk2 * 25 >> 9);
            pos.y = h->hero_eng->unk10.y;
            m->jump_acc = 5;
            m->jump_dec = 10;
            m->jump_vel = -0x44;
            if (D_8005A648 == 0x19) {
                if (player == 0) {
                    PlaySfx(0x68D, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 1) {
                    PlaySfx(0x68E, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 2) {
                    PlaySfx(0x68F, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (player == 3) {
                    PlaySfx(0x690, NULL, NULL, 0x1000, 0x1E00, 0);
                }
            }
        }
    }
}

void func_800BC4B0(s32 player) {
    Ovl63JumpFux *fux;
    EXTheromove *move;

    func_8008502C();
    move = Heroes[player].hero_move;
    if (move == NULL) {
        return;
    }
    fux = (Ovl63JumpFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (fux->phase == 0 && Ovl63JumpAbove(player)) {
        move->jump_vel = -0x20;
        fux->phase = 1;
    }
    if (fux->phase == 1 && Ovl63JumpAbove(player)) {
        Heroes[player].hero_eng->unk10.y -= abs(D_800C6930 - Heroes[player].hero_eng->unk10.y);
        SetHeroAction((HeroInfo *)player, 0x5B);
    }
}

void func_800BC6C8(s32 arg0) {
    Ovl63FuxTimer *fux;
    HeroInfo *hero;
    EXTheromove *move;
    HeroInfo *base;

    base = Heroes;
    hero = &base[arg0];
    fux = (Ovl63FuxTimer *)hero->hero->FUX;
    if (fux == NULL) {
        return;
    }
    func_8008502C();
    func_800BC984(arg0);
    move = hero->hero_move;
    if (move == NULL) {
        return;
    }
    hero->hero_frozen = 1000;
    hero->hero_eng->unk5E = 1;
    if (fux->unk24-- < 0) {
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = 0;
        move->z_vel = 0;
        if (fux->unk24 < -0xF) {
            SetHeroAction((HeroInfo *)arg0, 0x5B);
            hero->hero_frozen = 0;
        }
    } else {
        do {
            hero->hero_eng->unk1E += 0x100;
        } while (0);
    }
}

void func_800BC7C4(s32 arg0) {
    HeroInfo *hero;
    EXTraceshield *ext;
    s32 lim;
    EXTengine *eng;
    EXTengine *eng2;
    EXTheromove *move;
    HeroInfo *other;
    u32 i;
    s32 found;
    s32 t;
    s32 angle;
    HeroInfo *base;
    SLONG_XYZ *pos;

    base = Heroes;
    hero = &base[arg0];
    ext = (EXTraceshield *)hero->hero->FUX;
    if (ext == NULL) {
        return;
    }
    eng = hero->hero_eng;
    t = ext->unk14[0]--;
    if (t < 0) {
        SetHeroAction((HeroInfo *)arg0, 0x5B);
        return;
    }
    pos = &eng->unk10;
    i = 0;
    lim = 0x27100;
    other = base;
loop:
    found = -1;
    if (i < 4) {
        if (i == arg0 || func_8008CEB0((u8 *)pos, (u8 *)&other->hero_eng->unk10) >= lim) {
            other++;
            i++;
            goto loop;
        }
        found = i;
    }
    if (found != -1) {
        move = Heroes[found].hero_move;
        if (move != NULL) {
            eng2 = Heroes[found].hero_eng;
            angle = get_angle(eng->unk10.x - eng2->unk10.x, eng->unk10.z - eng2->unk10.z) & 0xFFF;
            if ((u32)(angle - 0x201) < 0xBFF) {
                move->x_vel = -((D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 3) >> 7);
                move->z_vel = -((D_80068BD4[angle].unk2 * 3) >> 7);
            }
        }
    }
    func_8008502C(arg0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BC984);

s32 func_800BCC20(s32 arg0) {
    Ovl63Pad *base;
    Ovl63Pad *pad;
    Ovl63Pad *b1;
    Ovl63Pad *p1;
    s32 st;

    if (Heroes[arg0].hero->FUX != NULL) {
        if (D_8005A614[arg0 + 0x5C] != 0) {
            b1 = (Ovl63Pad *)D_80051338;
            p1 = &b1[arg0];
            st = p1->unk50;
            if (!(st & 0x4000)) {
                p1->unk50 = st & ~0x4000;
                return 0;
            }
            goto act;
        }
        base = (Ovl63Pad *)D_80051338;
        pad = &base[arg0];
        if (pad->unk54 & 0x4000) {
act:
            SetHeroAction((HeroInfo *)arg0, 0x5E);
            return 1;
        }
        pad->unk50 &= ~0x4000;
        return 0;
    }
    return 0;
}

s32 func_800BCD1C(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + arg0;

    if (func_8008C4C8((s32 *)&hero->hero_eng->unk10, 2) == 0 &&
        func_8008C4C8((s32 *)&hero->hero_eng->unk10, 3) == 0) {
        return 0;
    }
    if (hero->hero_mode != 0x60) {
        SetHeroAction((HeroInfo *)arg0, 0x60);
    }
    return 1;
}

void func_800BCDAC(s32 player) {
    HeroInfo *hero;
    HeroInfo *base;

    D_800C695C = 1;
    func_80089FA4(player);
    SetHeroSeq((HeroInfo *)player, 0x48);
    base = Heroes;
    hero = base + player;
    hero->hero_eng->unk20 = 0;
    hero->hero_eng->unk1C = 0;
    hero->hero_eng->unk10.y = D_800C6930;
    hero->hero_eng->unk5E = 1;
    if (D_8005A648 != 0x19) {
        func_80085718(player, 0);
        return;
    }
    func_80085718(player, 1);
    func_80017640();
    func_80011D28((u8 *)&D_8004E0F0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BCE88);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BD99C);

void func_800BE7B4(s32 player, Ovl63Barrel *b) {
    Ovl63Fux *f;
    s32 ma;
    s32 mb;
    s32 xa;
    s32 xb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = Heroes[player].hero_move->mass;
    mb = b->unk4E;
    if (ma > 0 && mb > 0) {
        f = (Ovl63Fux *)Heroes[player].hero->FUX;
        if (f != NULL) {
            xa = f->unk13C >> 8;
            xb = b->unk20 >> 8;
            d = -((xa - xb) << 8);
            xb = (ma * xa + mb * xb) << 8;
            q = (d * mb + xb) / (total = mb + ma);
            xa = f->unk140 >> 8;
            xb = b->unk40 >> 8;
            d = -((xa - xb) << 8);
            r = (d * mb + ((ma * xa + mb * xb) << 8)) / total;
            f->unk13C = q;
            b->unk20 = q;
            f->unk140 = r;
            b->unk40 = r - d;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BE914);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800BEBC0);

s32 func_800BF0F8(u8 *arg0, u32 arg1) {
    u32 i;
    s32 d;
    s32 j;
    u16 f;

    for (i = 0; ; i++) {
        if (i >= 4) {
            return -1;
        }
        if (i != arg1 && Heroes[i].hero_mode != 0x61) {
            d = func_8008CEB0(arg0, (u8 *)Heroes[i].hero_eng + 0x10);
            if (abs(*(s32 *)(arg0 + 4) - *(s32 *)((u8 *)Heroes[i].hero_eng + 0x14)) > 0x180) {
                break;
            }
            if (d < 0x24000) {
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction((HeroInfo *)i, 0x5D);
                }
                if (G63B->unk34 == 0x17 && (G63B->unk32 & 0x80) && G63B->unk5C[i] == 0 && !(G63B->unk30 & 0x801)) {
                    f = 1;
                    if (G63B->unk4 & 0x800) {
                        f = 0x201;
                    }
                    G63B->unk30 |= f;
                    G63B->unk60 = 0;
                    for (j = 0; j < 4; j++) {
                        Heroes[j].hero_status |= 0x100;
                        if (G63B->unk5C[j] != 0) {
                            func_80026980(Heroes[j].hero);
                        }
                    }
                    D_800AD12C = 1;
                }
                return i;
            }
        }
    }
    return -1;
}

void InitRaceMissile(NAS *m) {
    RaceMissileExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x78, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk74 = 0x2011;
        m->Engine->sub->unk6C = D_80052F5C + 0x2028;
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk51 = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk64 = 0x8C;
        ext->unk34 = 0x3000;
        ext->unk40 = 0x700;
        ext->unk54 = 4;
        ext->unk10 = 0;
        ext->unk20 = 0;
        ext->unk60 = 0;
        ext->unk4C = 0;
        ext->unk50 = 0x100;
        ext->unk5C = NULL;
        ext->unk68 = 0;
        ext->unk58 = ext->unk34;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->unk5C = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
        }
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->unk10, 0x1000, 0x1E00, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceMissile);

void InitRacePickUp(NAS *m) {
    RacePickUpExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x50, 0x10000000);
    if (ext != NULL) {
        func_80026B70((u8 **)&D_800C6920, (u8 *)m);
        m->Engine->sub->unk74 = 0x2006;
        m->Engine->sub->unk6C = &D_80054F84;
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        ext->unk10 = -0x2000;
        ext->unk1C = 0x200;
        ext->unk40 = 0;
        ext->unk48 = 0;
        ext->unk4C = 0;
        ext->unk30 = 1;
        ext->unk28 = 0;
        ext->unk2C = 0x100;
        ext->unk3C = NULL;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->unk3C = e;
            e->sub->unk6C = (u8 *)&D_80054F84 - 0x2028;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl63/ovl63", D_800B3400);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRacePickUp);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C0218);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C04EC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", func_800C0CB4);

void func_800C0F5C(void) {
    u8 unused[24];
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotTNTCrate);

void InitDinoEgg(NAS *m) {
    EXTdinoegg *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x34, 0x10000000);
    if (ext != NULL) {
        ext->anim = (EXTanimator *)func_80024E30(m, &D_800C6D14);
        m->Engine->unk5D = 0;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk74 = 0x2005;
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        ext->NasStatus = 0;
        ext->angcounter = 0;
        ext->yang = 0;
        ext->shadow = NULL;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
        }
        D_800C6924++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotDinoEgg);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceShield);

void PlotRaceTag(NAS *m) {
    s32 i;
    HeroInfo *h1;
    HeroInfo *h;
    Ovl63TagFux *fux1;
    Ovl63TagFux *fux;
    SLONG_XYZ pos;
    u8 *tex;
    SLONG_XYZ *pp;
    NAS *tag;
    Ovl63TagExt *ext;

    D_800C6D20--;
    i = 0;
    h1 = Heroes;
loop1:
    if (i < 4) {
        fux1 = (Ovl63TagFux *)h1->hero->FUX;
        if (fux1 != NULL && fux1->unk2C > 0) {
            fux1->unk2C--;
        }
        h1++;
        i++;
        goto loop1;
    }
    if (m->Engine->sfx != NULL) {
        func_800228A4(m->Engine->sfx, (0x12C - D_800C6D20) * 0xF00 / 300 + 0x1E00);
    }
    i = 0;
    if (D_800C6D20 == 0) {
        pp = &pos;
        h = Heroes;
loop2:
        if (i < 4) {
            fux = (Ovl63TagFux *)h->hero->FUX;
            if (fux == NULL) {
                goto next;
            }
            if (fux->unk18 == 0x10) {
                goto clear;
            }
            pos = h->hero_eng->unk10;
            tex = D_800546C4;
            tag = SpawnNASById(pos.x, pp->y, pp->z, 0x1605, 0x11);
            if (tag != NULL) {
                ext = FindExtension(tag, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = 0;
                    ext->unk18 = 0x100;
                    if (tex != NULL) {
                        ext->unk20 = 5;
                        tag->Engine->sub->unk6C = tex;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    tag->InitDelete(tag);
                }
            }
            if (h->hero_mode != 0x5D) {
                SetHeroAction((HeroInfo *)i, 0x5D);
            }
            fux->unkC = 0x78;
            PlaySfx(0x466, NULL, NULL, 0x1000, 0x1E00, 0);
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            goto next;
        clear:
            fux->unk18 = 0;
        next:
            h++;
            i++;
            goto loop2;
        }
        D_800C6928 = 0x12C;
        KillNAS(m);
    }
}

void InitRaceVolcano(NAS *nas) {
    s32 i;
    RaceVolcanoExt *v;
    RaceVolcanoExt *ext;
    EXTengine *eng;

    v = AddExtension(nas, 0x15C, 0x10000000);
    i = 0;
    if (v != NULL) {
        ext = v;
        ext->unk10 = 0x1C2;
        ext->unkC = 0;
    loop:
        if (i < 3) {
            eng = AddEngine(nas, i + 2);
            if (eng != NULL) {
                ext->eng[0] = eng;
                eng->sub->flags &= ~0x8000;
                eng->unk5C = 1;
                eng->unk5D = 2;
                eng->sub->unk74 = 0x2019;
                eng->sub->unk6C = *(void **)D_80058AD0;
                ext->eng[0]->unk1E = Random(0x1000);
                ext->eng[0]->unk20 = 0;
                ext->unk144[0] = 0;
                ext->unk150[0] = 0;
            }
            ext = (RaceVolcanoExt *)((s32 *)ext + 1);
            i++;
            goto loop;
        }
        i = 2;
    loop2:
        i--;
        if (i >= 0) goto loop2;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotRaceVolcano);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotGlob);

s32 func_800C310C(SLONG_XYZ *pos) {
    s32 hit;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 dz;
    u16 bits;

    hit = 0;
    for (i = 0;; i++) {
        if (i >= 4) {
            return hit;
        }
        dx = Heroes[i].hero_eng->unk10.x - pos->x;
        dz = Heroes[i].hero_eng->unk10.z - pos->z;
        dy = Heroes[i].hero_eng->unk10.y - pos->y;
        if (dx * dx + dz * dz + dy * dy < 0x90000) {
            if (Heroes[i].hero_mode != 0x61) {
                if (Heroes[i].hero_mode != 0x5D) {
                    SetHeroAction((HeroInfo *)i, 0x5D);
                }
                if (MODE63->flags & 0x80) {
                    if (MODE63->unk5C[i] != 0) {
                        hit = 1;
                        continue;
                    }
                    bits = 1;
                    if (MODE63->unk4 & 0x800) {
                        bits = 0x201;
                    }
                    MODE63->unk60 = 0;
                    MODE63->unk30 |= bits;
                    for (j = 0; j < 4; j++) {
                        Heroes[j].hero_status |= 0x100;
                        if (MODE63->unk5C[j] != 0) {
                            func_80026980(Heroes[j].hero);
                        }
                    }
                    D_800AD12C = 1;
                }
            }
            hit = 1;
        }
    }
}

void InitBarrelRoll(NAS *m) {
    BarrelRollExt *ext;
    EXTengine *e;
    s32 r;

    ext = AddExtension(m, 0x80, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        m->Engine->sub->unk74 = 0x2037;
        m->Engine->unk5D = 1;
        ext->unkC = 0xC;
        ext->unk78 = 0;
        ext->unk50 = 0xA0;
        ext->unk10 = 0;
        ext->unk24 = 0;
        r = Random(0x400);
        ext->vel.x = (D_80068BD4[(r + 0xA00) & 0xFFF].unk2 * 9) >> 4;
        r += 0xE00;
        ext->vel.z = (D_80068BD4[r & 0xFFF].unk2 * 9) >> 4;
        m->Engine->unk1E = -r;
        ext->pos.z = 0x1B00;
        ext->vel.y = 0;
        ext->pos.x = 0;
        ext->pos.y = -0x100;
        m->Engine->unk10 = ext->pos;
        m->Engine->unk5E = 1;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            e->unk2C.z = 0x4000;
            e->unk2C.y = 0x4000;
            e->unk2C.x = 0x4000;
            ext->shadow->unk5E = 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotBarrelRoll);

void InitTrackGlob(NAS *m) {
    EXTtrackglob *ext;
    EXTengine *e;
    s32 angle;

    ext = AddExtension(m, 0x8C, 0x10000000);
    if (ext != NULL) {
        ext->cursor = func_80024E30(m, &D_800C6EA4);
        func_80026B70(&D_800C66EC, (u8 *)m);
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        m->Engine->sub->unk74 = 0x4100;
        m->Engine->unk5D = 1;
        ext->unk10 = 0x10;
        ext->unk54 = 0x40;
        ext->unkC = 0;
        ext->unk14 = 0;
        ext->unk28 = 0;
        angle = 0xA00 - (D_800C6EB0 << 9);
        if (++D_800C6EB0 == 3) {
            D_800C6EB0 = 0;
        }
        ext->unk64 = (D_80068BD4[(angle - 0x400) & 0xFFF].unk2 * 7) >> 4;
        ext->unk6C = (D_80068BD4[angle & 0xFFF].unk2 * 7) >> 4;
        ext->unk68 = 0;
        ext->unk84 = 0x5A;
        m->Engine->unk1E = 0;
        ext->pos = m->Engine->unk10;
        m->Engine->unk5E = 1;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->eng = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            ext->eng->unk5E = 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotTrackGlob);

void InitDac(NAS *m) {
    DacExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x28, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 2;
        m->Engine->unk2C.x = 0x2000;
        m->Engine->unk2C.y = 0x2000;
        m->Engine->unk2C.z = 0x2000;
        m->Engine->unk10.x = (D_80068BD4[0x800].unk2 * 7) >> 4;
        m->Engine->unk10.z = (D_80068BD4[0xC00].unk2 * 7) >> 4;
        m->Engine->sub->unk68 = -0x300;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk10 = (s32)func_80024E30(m, &D_800C6ECC);
        func_800250B0(m);
        m->Engine->unk1E = 0x800;
        m->Engine->unk5E = 1;
        ext->unkC = 0;
        ext->unk18 = 0xF0;
        m->Engine->unk10.y = -0x600;
        ext->unk1C = -0x600;
        ext->unk24 = 0;
        ext->shadow = NULL;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->shadow = e;
            e->sub->unk6C = D_80052F5C;
            e->sub->unk74 = 0x2001;
            e->unk5C = 1;
            e->unk2C.x = 0x2000;
            e->unk2C.y = 0x2000;
            e->unk2C.z = 0x2000;
            e->sub->unk68 = -0x300;
            e->sub->flags |= 0x02000000;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            ext->shadow->unk5E = 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl63/ovl63", PlotDac);

void func_800C44B0(void) {
    D_800C6BB4[0] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[1] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[2] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[3] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[4] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[5] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[6] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
    D_800C6BB4[7] = (Ovl63Short3){ D_800C66E4, 0x28, 0x38 };
}

void PlotRaceSmokeBoostWater(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceSmokeBoostWater(NAS *nas) {
    func_80024E30(nas, &D_800C6F28);
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->UserInfo.user1 = 0;
}

s32 func_800C467C(Ovl63BounceObj *obj, Ovl63Bouncer *b) {
    s32 pos;

    b->vel += b->accel;
    pos = obj->pos += b->vel >> 8;
    if (b->limit < pos) {
        obj->pos -= abs(pos - b->limit);
        if (b->flags & 1) {
            b->vel = -b->vel;
        }
        if (b->flags & 2) {
            b->vel = -((b->vel * b->damp) >> 8);
        }
        if (b->flags & 4) {
            b->vel = -b->rebound;
        }
        return 1;
    }
    return 0;
}

void InitGlob(NAS *m) {
    EXTglob *ext;

    ext = AddExtension(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        ext->unk10 = func_80024E30(m, &D_800C6E80);
        m->Engine->sub->unk6C = *(s32 *)D_80058AD0;
        func_80025FAC((EXTanimator *)ext->unk10, 2);
        func_800250B0(m);
        ext->unkC = 3;
        ext->unk14 = Random(0x5A) + 0xF0;
        ext->unk18 = 0;
    }
}

void PlotInnerRipple(NAS *m) {
    EXTinnerripple *ext;
    subengine *sub;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        ext->scale += ext->speed;
        if (ext->max < ext->scale) {
            ext->scale = ext->max;
        }
        m->Engine->unk2C.x = ext->scale + 0x400;
        m->Engine->unk2C.y = ext->scale + 0x400;
        m->Engine->unk2C.z = ext->scale + 0x400;
        m->Engine->unk5E = 1;
        sub = m->Engine->sub;
        if ((s16)sub->unk76 >= 0xFFF) {
            func_800270B0(m);
            return;
        }
        sub->unk76 += ext->unk18;
    }
}

void InitInnerRipple(NAS *m) {
    EXTinnerripple *ext;

    ext = AddExtension(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        ext->speed = 0x240;
        ext->max = 0x4000;
        ext->scale = 0;
        *(s32 *)&ext->unk18 = 0xC8;
        m->Engine->sub->unk6C = &D_80054F84;
        m->Engine->sub->unk74 = 0x2018;
        m->Engine->sub->flags |= 0x40000000;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        m->Engine->sub->unk76 = 0;
        m->Engine->unk2C.x = 0x400;
        m->Engine->unk2C.y = 0x400;
        m->Engine->unk2C.z = 0x400;
        m->Engine->unk5E = 1;
    }
}

void PlotRippled(NAS *m) {
    EXTripple *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        ext->scale += ext->step;
        if (ext->max < ext->scale) {
            ext->scale = ext->max;
        }
        m->Engine->unk2C.x = ext->scale + 0x400;
        m->Engine->unk2C.y = ext->scale + 0x400;
        m->Engine->unk2C.z = ext->scale + 0x400;
        m->Engine->sub->unk76 += ext->fade;
        m->Engine->unk5E = 1;
        if ((s16)m->Engine->sub->unk76 >= 0xFFF) {
            func_800270B0(m);
        }
    }
}

void InitRippled(NAS *m) {
    EXTrippled *ext;

    ext = AddExtension(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        ext->speed = 0x480;
        ext->max = 0x4000;
        ext->scale = 0;
        ext->unk18 = 0x190;
        m->Engine->sub->unk6C = &D_80054F84;
        m->Engine->unk5C = 1;
        m->Engine->unk2C.x = 0x400;
        m->Engine->unk2C.y = 0x400;
        m->Engine->unk2C.z = 0x400;
        m->Engine->sub->unk74 = 0x2018;
        m->Engine->sub->flags = 0x40008000;
        m->Engine->unk5D = 0;
        m->Engine->unk5C = 1;
        m->Engine->unk5E = 1;
        m->Engine->sub->unk76 = 0;
    }
}

void PlotRaceGreenSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceGreenSmokeBoost(NAS *nas) {
    func_80024E30(nas, &D_800C6E2C);
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->UserInfo.user1 = 0;
}

void PlotRaceGreenSmokePoof(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceGreenSmokePoof(NAS *nas) {
    func_80024E30(nas, &D_800C6DD0);
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->Engine->sub->flags |= 0x21;
    nas->UserInfo.user1 = 0;
}

void PlotRaceSmokePoof(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceSmokePoof(NAS *nas) {
    func_80024E30(nas, &D_800C6D74);
    nas->Engine->sub->unk6C = D_800546C4;
    nas->Engine->sub->flags |= 0x21;
    nas->UserInfo.user1 = 0;
}

s32 func_800C4D1C(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0)) - (*(s32*)((u8*)(arg1) + 0));
    temp_v0_2 = (*(s32*)((u8*)(arg0) + 8)) - (*(s32*)((u8*)(arg1) + 8));
    temp_v0_3 = (*(s32*)((u8*)(arg0) + 4)) - (*(s32*)((u8*)(arg1) + 4));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2) + (temp_v0_3 * temp_v0_3);
}

void InitRaceTag(u8 *arg0) {
    D_800C6D20 = 0x12C;
    PlaySfx(0x452, (*(s32*)((u8*)(arg0) + 84)) + 0x94, 0, 0x1000, 0x1E00, 0);
}

void InitRaceShield(NAS *nas) {
    EXTraceshield *ext;

    ext = AddExtension(nas, 0x20, 0x10000000);
    if (ext != NULL) {
        ext->unk10 = 0xE;
        ext->unkC = 0;
        ext->unk1C = 0x100;
    }
}

void PlotRaceFire(NAS *nas) {
    FindExtension(nas, 0x10000000);
}

void InitRaceFire(NAS *nas) {
    EXTracefire *ext;

    ext = AddExtension(nas, 0x20, 0x10000000);
    if (ext != NULL) {
        ext->unk10 = 0;
        ext->unk18 = 0;
        ext->unk1C = 0;
        ext->unk14 = 0;
    }
}

static inline s32 DotXZ(s32 *u, s32 *v) {
    return (u[0] * v[0] >> 12) + (u[2] * v[2] >> 12);
}

s32 func_800C4E54(s32 a, s32 b) {
    s32 n[4];
    s32 d[4];
    s32 r[4];
    s32 dot;

    n[0] = D_80068BD4[b & 0xFFF].unk2;
    n[2] = D_80068BD4[(b - 0x400) & 0xFFF].unk2;
    d[0] = -D_80068BD4[(a - 0x400) & 0xFFF].unk2;
    d[2] = -D_80068BD4[a & 0xFFF].unk2;
    dot = DotXZ(d, n);
    r[0] = d[0] - (dot * n[0] >> 11);
    r[2] = d[2] - (dot * n[2] >> 11);
    return get_angle(r[0], r[2]) + 0x800;
}

s32 func_800C4F30(u8 *arg0) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (func_8008CEB0(arg0, (u8 *)hero->hero_eng + 0x10) < 0x6184) {
            if (hero->hero_mode != 0x5D) {
                SetHeroAction((HeroInfo *)i, 0x5D);
            }
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void InitTNTCrate(NAS *m) {
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->unk74 = 0x2009;
    m->Engine->sub->unk6C = (s32)D_80052F94;
    m->UserInfo.status = 0;
    m->UserInfo.user1 = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.flags = 0;
}

s32 func_800C5014(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if (var_a0 < 0) {
        var_a0 += 0x1FF;
    }
    return var_a0 >> 9;
}

void PlotRaceSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

void InitRaceSmokeBoost(NAS *nas) {
    ScriptCursor *cur;

    cur = func_80024E30(nas, &D_800C6C8C);
    if (D_8005A648 == 0x1F) {
        if (cur != NULL) {
            cur->unk1E = 4;
        }
    }
    nas->Engine->sub->unk6C = &D_80054F84;
    nas->UserInfo.user1 = 0;
}

s32 func_800C50B0(u8 *arg0, s32 arg1) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (func_8008CEB0(arg0, (u8 *)hero->hero_eng + 0x10) < arg1 * arg1) {
            if (abs(((s32 *)arg0)[1] - hero->hero_eng->unk10.y) < 0x200) {
                return i;
            }
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void func_800C5154(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    SLONG_XYZ pos;
    NAS *nas;
    s32 *ext;

    pos = hero->hero_eng->unk10;
    nas = SpawnNASById(pos.x, pos.y - 0x80, pos.z, 0x1701, 0x11);
    if (nas != NULL) {
        nas->Engine->unk1E = hero->hero_eng->unk1E;
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = player;
        }
    }
}

void func_800C5204(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        d = -((a->x_vel - b->x_vel) << 8);
        q = (d * mb + ((ma * a->x_vel + mb * b->x_vel) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        d = -((a->z_vel - b->z_vel) << 8);
        q = (d * mb + ((ma * a->z_vel + mb * b->z_vel) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

s32 func_800C5334(s32 arg0) {
    EXTengine *eng;
    s32 angle;

    eng = Heroes[arg0].hero_eng;
    angle = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
    angle = 0x152C - angle;
    angle &= 0xFFF;
    printf(D_800B3400, arg0, angle);
    return angle;
}

void func_800C53BC(s32 arg0) {
    func_80085890(arg0);
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

s32 func_800C5400(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x62) {
            SetHeroAction((HeroInfo *)arg0, 0x62);
        }
        return 1;
    }
    return 0;
}

s32 func_800C5470(s32 arg0) {
    HeroInfo *hero;
    Ovl63FuxCount *fux;
    EXTraceshield *ext;
    NAS *nas;
    HeroInfo *base;

    base = Heroes;
    hero = &base[arg0];
    fux = (Ovl63FuxCount *)hero->hero->FUX;
    if (fux != NULL && fux->unk3C > 0 &&
        (((Ovl63Pad *)D_80051338)[arg0].unk54 & 0x8000)) {
        nas = SpawnNASById(hero->hero_eng->unk10.x, hero->hero_eng->unk10.y,
                           hero->hero_eng->unk10.z, 0x1705, 0x11);
        if (nas != NULL) {
            ext = FindExtension(nas, 0x10000000);
            if (ext != NULL) {
                ext->unkC = arg0;
            }
        }
        return 1;
    }
    return 0;
}

s32 func_800C554C(s32 arg0) {
    if (((Ovl63Pad *)D_80051338)[arg0].unk50 & 0xF0) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x5B);
    return 1;
}

s32 func_800C55A4(s32 arg0) {
    if (!(((Ovl63Pad *)D_80051338)[arg0].unk54 & 0x8000)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x5C);
    return 1;
}

void func_800C55FC(s32 player) {
    EXTraceshield *ext;

    ext = (EXTraceshield *)Heroes[player].hero->FUX;
    if (ext != NULL) {
        ext->unk14[0] = 8;
    }
}

s32 func_800C5640(u8 *arg0, u32 arg1, s32 arg2) {
    HeroInfo *hero;
    u32 i;

    i = 0;
    hero = Heroes;
loop:
    if (i < 4) {
        if (i != arg1) {
            if (func_8008CEB0(arg0, (u8 *)hero->hero_eng + 0x10) < arg2 * arg2) {
                return i;
            }
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void func_800C56D4(s32 player) {
    HeroInfo *hero;
    Ovl63SpinFux *fux;
    EXTheromove *move;
    s32 a;
    s32 b;
    HeroInfo *base = Heroes;

    hero = &base[player];
    fux = (Ovl63SpinFux *)hero->hero->FUX;
    if (fux != NULL) {
        move = hero->hero_move;
        if (move != NULL) {
            a = Random(0x300);
            b = Random(0x300);
            b = hero->hero_eng->unk1E + a - b;
            fux->unk24 = 0x2D;
            fux->unk20 = b;
            fux->unk28 = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        }
    }
}

void func_800C57B4(s32 player) {
    EXTheromove *move;
    Ovl63HeroFux *fux;

    SetHeroSeq((HeroInfo *)player, 0x48);
    if (D_8005A648 == 0x19) {
        SetHeroSeq((HeroInfo *)player, 0x52);
    }
    move = Heroes[player].hero_move;
    if (move != NULL) {
        move->z_max = 0;
        move->x_max = 0;
    }
    fux = (Ovl63HeroFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk8C = 0;
        fux->unk90 = 0;
    }
}

void func_800C5854(s32 *arg0, s32 *arg1)
{
    s32 a;
    s32 c;
    s32 t;

    a = arg0[3] >> 1;
    arg1[3] = D_80068BD4[a & 0xFFF].unk2;
    c = D_80068BD4[(a - 0x400) & 0xFFF].unk2;
    t = c * arg0[0];
    arg1[0] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
    t = c * arg0[1];
    arg1[1] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
    t = c * arg0[2];
    arg1[2] = (t >= 0 ? t + 0x800 : t - 0x800) >> 12;
}

void func_800C5918(s32 arg0) {
    SetHeroAction((HeroInfo *)arg0, 0x5B);
    Heroes[arg0].hero_eng->unk1E = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
}

s32 func_800C5978(s32 player) {
    EXTheromove *move;
    EXTengine *eng;
    s32 y;

    move = Heroes[player].hero_move;
    eng = Heroes[player].hero_eng;
    if (move != NULL && eng != NULL) {
        if (move->jump_vel < 0) {
            move->jump_vel += move->jump_acc;
        } else {
            move->jump_vel += move->jump_dec;
        }
        y = eng->unk10.y += move->jump_vel;
        if (move->jump_vel < 0 && y < D_800C6930) {
            eng->unk10.y += abs(D_800C6930 - y);
            return 1;
        }
    }
    return 0;
}

s32 func_800C5A50(s32 player) {
    EXTheromove *move;
    EXTengine *eng;
    s32 y;

    move = Heroes[player].hero_move;
    eng = Heroes[player].hero_eng;
    if (move != NULL && eng != NULL) {
        if (move->jump_vel < 0) {
            move->jump_vel += move->jump_acc;
        } else {
            move->jump_vel += move->jump_dec;
        }
        y = eng->unk10.y += move->jump_vel;
        if (move->jump_vel > 0 && y > D_800C6930) {
            return 1;
        }
    }
    return 0;
}

s32 func_800C5AF8(void) {
    return D_800C6930;
}

void func_800C5B08(s32 *pos, s32 *polar) {
    polar[0] = csqrt(pos[0] * pos[0] + pos[2] * pos[2]) >> 6;
    polar[1] = get_angle(pos[0], pos[2]) & 0xFFF;
}

s32 func_800C5B7C(void) {
    return 0;
}

void func_800C5B84(void) {
}

void func_800C5B8C(void) {
}

void func_800C5B94(void) {
    if (D_800C6BAC-- < 0) {
        SpawnNASById(0, 0, 0, 0x170C, 0x11);
        D_800C6BAC = 0xB4;
    }
}

void func_800C5BEC(void) {
    D_800C69B4->unk74 = (D_800C69B8 >> 8) + 0x4000;
    if (D_800C69BC == 0) {
        D_800C69B8 += 0x80;
        if (D_800C69B8 >= 0x6000) {
            D_800C69B8 = 0;
        }
    }
    if (D_800C69BC == 1) {
        D_800C69B8 -= 0x19C;
        if (D_800C69B8 <= 0) {
            D_800C69B8 = 0;
            D_800C69BC = 0;
        }
    }
}

NAS *func_800C5C64(s32 arg0) {
    HeroInfo *base;
    HeroInfo *hero;
    NAS *nas;
    NAS *best;
    s32 dist;
    s32 min;

    best = NULL;
    min = 0x7FFFFFFF;
    nas = D_800C6920;
    base = Heroes;
    hero = base + arg0;
loop:
    if (nas != NULL) {
        dist = func_8008CEB0((u8 *)&nas->Engine->unk10, (u8 *)&hero->hero_eng->unk10);
        do {
            if (dist < min) {
                min = dist;
                best = nas;
            }
        } while (0);
        nas = nas->Group.Child;
        goto loop;
    }
    return best;
}

s32 func_800C5D0C(s32 arg0) {
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

void func_800C5D8C(void) {
    func_800B4708();
}

NAS *func_800C5DAC(s32 arg0) {
    NAS *nas;
    HeroInfo *base;
    HeroInfo *hero;
    s32 angle;

    nas = func_800C5C64(arg0);
    if (nas != NULL) {
        base = Heroes;
        hero = base + arg0;
        if (func_8008CEB0((u8 *)&nas->Engine->unk10, (u8 *)&hero->hero_eng->unk10) <= 0x270FFF) {
            angle = get_angle(nas->Engine->unk10.x, nas->Engine->unk10.z) & 0xFFF;
            if ((get_angle(hero->hero_eng->unk10.x, hero->hero_eng->unk10.z) & 0xFFF) < angle) {
                return nas;
            }
        }
    }
    return NULL;
}

s32 func_800C5E6C(s32 arg0, s32 arg1) {
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

void func_800C5F54(void) {
    D_8005AA70[0] = (s32)func_800B4A4C;
    D_8005AA70[1] = (s32)func_800B8890;
    D_8005AA70[2] = (s32)func_800B8890;
    D_8005AA70[3] = (s32)func_800B8D68;
    D_8005AA70[5] = (s32)func_800C5B84;
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
}
