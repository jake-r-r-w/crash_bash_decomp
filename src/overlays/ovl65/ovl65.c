#include "common.h"
#include "bash/ovl65.h"
#include "bash/engine.h"
#include "bash/dino.h"
#include "bash/hero.h"
#include "bash/shell.h"

s32 func_800B33D8(s32 arg0) {
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

void func_800B3538(id)
s32 id;
{
    HeroInfo *h;
    HeroInfo *h2;
    Ovl65Chase *c;
    EXTbotai *ai;
    u32 pad;
    u32 dir;
    u16 ang;
    u16 t;
    s32 a;
    s32 b;
    SLONG_XYZ *p;
    SLONG_XYZ *q;
    HeroInfo *base;
    s32 off;

    base = Heroes;
    off = id * sizeof(HeroInfo);
    h = (HeroInfo *)(off + (u8 *)base);
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        p = &c->pos;
        q = &c->tgt;
        dir = (p->x > q->x) << 7;
        if (p->x < q->x) {
            dir |= 0x20;
        }
        a = p->z;
        b = q->z;
        if (b < a) {
            dir |= 0x40;
        }
        if (a < b) {
            dir |= 0x10;
        }
        h->hero_ai->AIpad |= dir;
        if (h->hero_frozen == 0) {
            ((Ovl65PadAng *)D_80051338)[id].buttons = h->hero_ai->AIpad;
        }
        ai = h->hero_ai;
        t = ai->AItimer1--;
        if (t == 0) {
            pad = h->hero_ai->AIpad;
            ang = 0;
            if (pad & 0x80) {
                ang = 0xC00;
            }
            if (pad & 0x20) {
                ang = 0x400;
            }
            if (pad & 0x10) {
                ang = 0;
            }
            if (pad & 0x40) {
                ang = 0x800;
            }
            if ((pad & 0x90) == 0x90) {
                ang = 0xE00;
            }
            if ((pad & 0xC0) == 0xC0) {
                ang = 0xA00;
            }
            if ((pad & 0x30) == 0x30) {
                ang = 0x200;
            }
            if ((pad & 0x60) == 0x60) {
                ang = 0x600;
            }
            h2 = (HeroInfo *)(off + (s32)base);
            h2->hero_ai->AIang = ang;
            h2->hero_ai->AItimer1 = 0;
        }
        if (Heroes[id].hero_frozen == 0) {
            ((Ovl65PadAng *)D_80051338)[id].angle = Heroes[id].hero_ai->AIang;
        }
    }
}

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

s32 func_800B38BC(s32 arg0) {
    s32 best = -1;
    s32 bestd = 0x7FFFFFFF;
    s32 start = Random(3);
    s32 i = start;
    s32 off;
    s32 idx;
    s32 dx;
    s32 dz;
    s32 d;
    s32 t;
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
        idx = i - off;
        ext = FindExtension(D_800BDA08[idx], 0x10000000);
        if (ext != NULL && ext->unkC == 1) {
            t = func_800B33D8(arg0);
            if (t == ext->unk10 && func_800B485C(arg0, t) == 0) {
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

void func_800B3A38(s32 id) {
    HeroInfo *h;
    HeroInfo *hb;
    Ovl65Chase *c;
    Ovl65Ext *e;
    EXTengine *b;
    EXTengine *a;
    s32 near;
    s32 far;
    s32 ok;
    s32 t;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        t = func_800B3760(id);
        if (t != -1) {
            c->unk28 = t;
        }
        e = FindExtension(D_800BDA08[c->unk28], 0x10000000);
        if (e != NULL) {
            t = e->unkC;
            if (t == 0 || t == 3 || t == 4) {
                near = 1;
            } else {
                near = 0;
            }
        } else {
            near = 0;
        }
        if ((near ^ 1) != 0) {
            t = func_800B3760(id);
            if (t != -1) {
                ok = 1;
                c->unk28 = t;
            } else {
                ok = 0;
            }
            if ((ok ^ 1) != 0) {
                c->unk28 = -1;
                c->fn = func_800BD1FC;
                return;
            }
        }
        if (func_800B49AC(id) != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
        }
        b = Heroes[id].hero_eng;
        c->pos = b->unk10;
        a = D_800BDA08[c->unk28]->Engine;
        c->tgt = a->unk10;
        func_800B3538(id);
    }
}

void func_800B3C1C(s32 id) {
    HeroInfo *h;
    HeroInfo *hb;
    Ovl65Chase *c;
    Ovl65Ext *e;
    EXTengine *b;
    EXTengine *a;
    s32 near;
    s32 far;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        e = FindExtension(D_800BDA08[c->unk28], 0x10000000);
        near = 0;
        if (e != NULL) {
            near = e->unkC == 1;
        }
        far = near ^ 1;
        if (far != 0) {
            c->unk28 = -1;
            c->fn = func_800BD1FC;
            return;
        }
        b = h->hero_eng;
        c->pos = b->unk10;
        a = D_800BDA08[c->unk28]->Engine;
        c->tgt = a->unk10;
        if (func_800B49AC(id) != 0) {
            h->hero_ai->AIpad |= 0x8000;
        }
        ((PadButtons *)D_80051338)[id].unk50 = h->hero_ai->AIpad;
        func_800B3538(id);
    }
}

s32 func_800B4268();

void func_800B3D90(s32 id) {
    Ovl65Chase *c;
    Ovl65Ext *e;
    s32 near;
    s32 hit;
    s32 yaw;
    s32 sc;
    Ovl65PosH *p;
    s32 v1[3];
    s32 v2[3];
    s16 d[8];

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        hit = 0;
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        if ((near ^ 1) != 0) {
            c->unk28 = -1;
            c->fn = func_800BD1FC;
            return;
        }
        if (func_800B49AC(id) != 0) {
            c->timer = Random(0xF) + 0x3C;
            c->state = 4;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = ((Ovl65Pos *)D_800BDB54)->pos;
        func_800B3538(id);
        yaw = Heroes[id].hero_eng->unk1E;
        sc = D_800BDB50 * 4;
        v1[0] = (sc * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2) >> 12;
        v1[2] = (sc * D_80068BD4[yaw & 0xFFF].unk2) >> 12;
        v2[0] = 0;
        v2[2] = 0;
        p = (Ovl65PosH *)D_800BDB54;
        d[0] = p->x - ((Ovl65EngH *)Heroes[id].hero_eng)->x;
        d[2] = p->z - ((Ovl65EngH *)Heroes[id].hero_eng)->z;
        d[1] = 0;
        d[3] = 0x300;
        if (func_800B4268(v2, v1, d) == 1) {
            hit = 1;
        }
        if (hit != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
            ((Ovl65Pad *)D_80051338)[id].unk50 = Heroes[id].hero_ai->AIpad;
            c->unk28 = -1;
            c->unk30 = 8;
            c->fn = func_800BD1FC;
            c->state = 0;
        }
    }
}

void func_800B3FDC(s32 id) {
    Ovl65Chase *c;
    Ovl65Ext *e;
    s32 near;
    s32 hit;
    s32 yaw;
    s32 sc;
    s32 r;
    Ovl65PosH *p;
    s32 v1[3];
    s32 v2[3];
    s16 d[8];

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        hit = 0;
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        if ((near ^ 1) != 0) {
            c->unk28 = -1;
            c->fn = func_800BD1FC;
            return;
        }
        r = func_800BCCE8(id);
        if (r != 1) {
            c->timer = 0x3C;
            c->state = 4;
            c->fn = func_800B4D6C;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = ((Ovl65Pos *)D_800BDB54)->pos;
        if (c->unk30-- > 0) {
            func_800B3538(id);
        }
        if (c->unk30 < -2) {
            c->unk30 = 8;
        }
        yaw = Heroes[id].hero_eng->unk1E;
        sc = D_800BDB50 * 4;
        v1[0] = (sc * D_80068BD4[(yaw - 0x400) & 0xFFF].unk2) >> 12;
        v1[2] = (sc * D_80068BD4[yaw & 0xFFF].unk2) >> 12;
        v2[0] = 0;
        v2[2] = 0;
        p = (Ovl65PosH *)D_800BDB54;
        d[0] = p->x - ((Ovl65EngH *)Heroes[id].hero_eng)->x;
        d[2] = p->z - ((Ovl65EngH *)Heroes[id].hero_eng)->z;
        d[1] = 0;
        d[3] = 0x300;
        if (func_800B4268(v2, v1, d) == r) {
            hit = 1;
        }
        if (hit != 0) {
            Heroes[id].hero_ai->AIpad |= 0x8000;
            ((Ovl65Pad *)D_80051338)[id].unk50 = Heroes[id].hero_ai->AIpad;
            c->unk28 = -1;
            c->unk30 = 8;
            c->fn = func_800BD1FC;
            c->state = 0;
        }
    }
}

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

s32 func_800B485C(s32 arg0, s32 arg1) {
    u8 *g = D_8005A614;

    if (*(s32 *)(g + 0x6C) != 0) {
        if (Heroes[arg1].hero_status & 0x2000) {
            return 0;
        }
        if (arg0 == *(s16 *)(g + 0x48) && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == D_8005A65C[0] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
        if (arg0 == D_8005A65C[1] && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == D_8005A65C[1] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
    }
    return 0;
}

s32 func_800B49AC(s32 id)
{
  Vec3W pos;
  s32 best;
  s32 bestd;
  s32 i;
  s32 d;
  u16 st;
  s32 dist;
  s32 sel;
  s32 *pd = &dist;
  best = -1;
  bestd = 0x07FFFFFF;
  i = 0;
  for (;;)
  {
    if (i >= 4)
    {
      goto done;
    }
    if (i != id)
    {
      st = Heroes[i].hero_status;
      sel = st & 0x2000;
      if (!sel)
      {
        if (!(st & 0x8000))
        {
          pos = *((Vec3W *) (&Heroes[i].hero_eng->unk10));
          d = func_8008CEB0((u8 *) (&pos), (u8 *) (&Heroes[id].hero_eng->unk10));
          if (d < bestd)
          {
            bestd = d;
            best = i;
          }
        }
      }
    }
    i++;
  }

  done:
  *pd = bestd;

  sel = best;
  if (sel != (-1))
  {
    if (func_800B33D8(id) == sel)
    {
      if (func_800B485C(id, sel) == 0)
      {
        if (dist <= 0xFFFFF)
        {
          if (func_800B475C(id, sel) != 0)
          {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

void func_800B4B14(s32 id) {
    Ovl65Chase *c;
    Ovl65Ext *e;
    s32 near;
    s32 dist;
    s32 far;
    SLONG_XYZ sp;
    s32 dx;
    s32 dz;

    c = FindExtension(Heroes[id].hero, 0x20000000);
    if (c != NULL) {
        sp.x = 0;
        sp.y = 0;
        sp.z = -0xB00;
        if (func_800B49AC(id) != 0) {
            c->timer = 0x5A;
            c->state = 4;
            return;
        }
        e = FindExtension(Heroes[id].hero, 0x10000000);
        near = 0;
        if (e != NULL) {
            near = (u32)e->unk14 > 0;
        }
        far = near ^ 1;
        if (far != 0) {
            c->unk28 = -1;
            c->fn = func_800BD1FC;
            return;
        }
        c->pos = Heroes[id].hero_eng->unk10;
        c->tgt = sp;
        dx = c->pos.x - c->tgt.x;
        dz = c->pos.z - c->tgt.z;
        dist = dx * dx + dz * dz;
        func_800B3538(id);
        if (dist <= 0x3FFFF) {
            c->state = 3;
        }
    }
}

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

void func_800B5424(id)
s32 id;
{
    Ovl65Chase *c;
    Ovl65Ext *e;
    s32 near;
    s32 r;
    s32 r2;
    HeroInfo *h;
    HeroInfo *hb;

    hb = Heroes;
    h = &hb[id];
    c = FindExtension(h->hero, 0x20000000);
    if (c != NULL) {
        h->hero_ai->AIpad = 0;
        h->hero_ai->AImode = 1;
        if (c->fn != func_800B3D90) {
            e = FindExtension(h->hero, 0x10000000);
            near = 0;
            if (e != NULL) {
                near = (u32)e->unk14 > 0;
            }
            if (near != 0) {
                if (c->state == 0) {
                    if (Random(0x1000) < 0x800) {
                        r = 2;
                    } else {
                        c->timer = 0x3C;
                        r = 4;
                    }
                    c->state = r;
                }
                if (c->state == 2) {
                    c->fn = func_800B4B14;
                }
                if (c->state == 1) {
                    c->fn = func_800B3D90;
                }
                if (c->state == 4) {
                    c->fn = func_800B4D6C;
                }
                if (c->state == 3) {
                    c->fn = func_800B3FDC;
                }
            }
        }
        if (c->fn == func_800BD1FC) {
            c->state = 0;
            r = func_800B3760(id);
            if (r != -1) {
                c->unk28 = r;
                c->fn = func_800B3A38;
            }
            r2 = func_800B38BC(id);
            if (r2 != -1) {
                c->unk28 = r2;
                c->fn = func_800B3C1C;
            }
        }
        c->fn(id);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B55E8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B5DCC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B62D8);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B69F4);

void func_800B6FE4(void) {
    void *v;
    void *f;
    void **p;
    u8 *g;
    D_8009D6E8.unk38 = func_800BD08C;
    D_8009D6E8.unk4 = D_800BDA7C;
    D_8009D6E8.unk0 = func_800BCB4C;
    D_8009D6E8.unk20 = func_800B9930;
    D_8009D6E8.unk24 = func_800B9778;
    D_8009D6E8.unk2C = func_800B96A0;
    f = func_800BCE7C;
    D_8009D6E8.unk64 = f;
    g = D_8005A614;
    if (*(u16 *)(g + 0x32) & 0x40) {
        D_8009D6E8.unk64 = func_800B69F4;
    }
    D_8009D6E8.unk14 = D_800BDA5C;
    D_8009D6E8.unk18 = D_8008B29C;
    D_8009D6E8.unk1C = D_8008B418;
    p = &D_8009D8FC;
    if (*(s32 *)(g + 0x6C) != 0) {
        v = D_800BD95C;
    } else {
        v = D_800BD948;
    }
    *p = v;
    D_8009D8F4 = D_800BD994;
    D_8009D8F8 = D_800BD9A8;
    D_8009D904 = D_800BD97C;
    D_8009D900 = D_800BD970;
    D_800BDB88 = 0;
    D_800BDB8C = 0;
    D_800BDC6C[0] = 0;
    D_800BDC6C[1] = 0;
    D_800BDC6C[2] = 0;
    D_800BDC78[0] = 0;
    D_800BDC78[1] = 0;
    D_800BDC78[2] = 0;
    D_800BDB7C = 0;
    D_800BDB80 = 0;
    D_800BDB84 = 0;
    D_800BDB54 = 0;
}

void func_800B714C(void) {
    Ovl65Obj *o;

    for (o = *(Ovl65Obj **)((u8 *)D_80058AD0 + 0x1C); o != NULL; o = o->next) {
        if (((Ovl65Game *)D_8005A614)->flags & 0x80) {
            if (o->index == 0x5002) {
                D_800BDB54 = o;
                printf("found target\n");
            }
            if (o->index == 0x5009) {
                D_800BDB7C = o;
                printf("found target\n");
            }
            if (o->index == 0x500B) {
                D_800BDB80 = o;
                printf("found target\n");
            }
            if (o->index == 0x500A) {
                D_800BDB84 = o;
                printf("found target\n");
            }
            if (o->index == 0x500C) {
                D_800BDC78[0] = o;
                printf("1\n");
            }
            if (o->index == 0x500D) {
                D_800BDC78[1] = o;
                printf("2\n");
            }
            if (o->index == 0x500E) {
                D_800BDC78[2] = o;
                printf("3\n");
            }
        } else if (o->index == 0x5002) {
            D_800BDB54 = o;
            printf("found target\n");
        }
    }
    D_800BDC6C[0] = D_800BDB7C;
    D_800BDC6C[1] = D_800BDB80;
    D_800BDC6C[2] = D_800BDB84;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7300);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7664);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800B7AA0);

void func_800B85DC(void) {
    s32 i;
    Ovl65Ext *ext;
    HeroInfo *hero;

    func_800B7AA0();
    i = 0;
    func_800B7300();
    hero = Heroes;
loop:
    if (i < 4) {
        ext = FindExtension(hero->hero, 0x10000000);
        if (ext != NULL && ext->unk10 > 0) {
            ext->unk10--;
        }
        hero++;
        i++;
        goto loop;
    }
    if (D_8005A624 == 0) {
        D_800BDBA8++;
    }
    if (D_800BDBA8 == 0x1C2) {
        FindExtension(D_800BDA00, 0x10000000);
    }
    if (D_800BDBA8 == 0x546) {
        ext = FindExtension(D_800BDA04, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 7;
        }
    }
}

s32 func_800B86C8(s32 arg0) {
    s32 dist;
    s32 ang;
    s32 d;
    s32 r;

    dist = csqrt(Heroes[arg0].hero_eng->unk10.x * Heroes[arg0].hero_eng->unk10.x +
                 Heroes[arg0].hero_eng->unk10.z * Heroes[arg0].hero_eng->unk10.z) >> 6;
    if (dist < 0xB01) {
        return 0;
    }
    ang = get_angle(Heroes[arg0].hero_eng->unk10.x, Heroes[arg0].hero_eng->unk10.z) & 0xFFF;
    d = dist - 0xAFE;
    Heroes[arg0].hero_eng->unk10.x -= (d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2) >> 12;
    Heroes[arg0].hero_eng->unk10.z -= (d * D_80068BD4[ang].unk2) >> 12;
    if (Heroes[arg0].hero_move->x_vel * Heroes[arg0].hero_move->x_vel +
            Heroes[arg0].hero_move->z_vel * Heroes[arg0].hero_move->z_vel > 0x1000 &&
        Heroes[arg0].hero_mode == 0x66) {
        r = Random(4);
        if (r == 0) {
            PlaySfx(0x6D0, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x6D1, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x6D2, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        do {
            if (r == 3) {
                PlaySfx(0x6D3, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        } while (0);
    }
    return ang;
}

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

void func_800B9534(s32 arg0, s32 arg1) {
    EXTheromove *a;
    EXTheromove *b;
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;
    s32 av;
    s32 bv;

    a = Heroes[arg0].hero_move;
    b = Heroes[arg1].hero_move;
    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        av = a->x_vel;
        bv = b->x_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        av = a->z_vel;
        bv = b->z_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

void func_800B96A0(s32 arg0) {
    if (Heroes[arg0].hero_frozen == 0) {
        if (!(Heroes[arg0].hero_ctrl & 1)) {
            Heroes[arg0].hero_eng->unk1E += func_800154A0(Heroes[arg0].hero_eng->unk1E, Heroes[arg0].hero_move->forward_angle) >> 2;
        }
        Heroes[arg0].hero_ctrl &= 0xFFFE;
    }
    Heroes[arg0].hero_eng->unk5E = 1;
}

void func_800B9778(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    ((void (*)(s32))D_8009D6E8.unk2C)(arg0);
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
    e->unk10.x += m->x_vel;
    e->unk10.z += m->z_vel;
    if (func_800B86C8(arg0) != 0 && Heroes[arg0].hero_mode == 0x66) {
        m->z_vel = 0;
        m->x_vel = 0;
        m->z_max = 0;
        m->x_max = 0;
    }
}

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

void func_800BA468(s32 id) {
    Ovl65Ext *ext;

    ext = FindExtension(Heroes[id].hero, 0x10000000);
    if (ext != NULL) {
        Heroes[id].hero_eng->unk1E += 0x80;
        func_800BA960(id);
        Heroes[id].hero_frozen = 2;
        ((PadButtons *)D_80051338)[id].unk50 = 0;
        ((PadButtons *)D_80051338)[id].unk54 = 0;
        if (ext->unkC-- < 0) {
            SetHeroAction((HeroInfo *)id, 0x63);
        }
        func_8008502C(id);
        if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
            SetHeroAction((HeroInfo *)id, 0x68);
        }
    }
}

void func_800BA574(s32 id) {
    EXTheromove *m;
    s32 v;

    m = Heroes[id].hero_move;
    if (id == 0) {
        PlaySfx(0x6C4, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 1) {
        PlaySfx(0x6C5, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 2) {
        PlaySfx(0x6C6, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    if (id == 3) {
        PlaySfx(0x6C7, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    m->x_acc = 0xA;
    m->z_acc = 0xA;
    m->z_dec = 6;
    m->x_dec = 6;
    m->forward_angle = Heroes[id].hero_eng->unk1E;
    v = (D_80068BD4[(m->forward_angle + 0x400) & 0xFFF].unk2 * 0x59) >> 0xB;
    m->x_vel = v;
    m->x_max = v;
    v = (D_80068BD4[(m->forward_angle + 0x800) & 0xFFF].unk2 * 0x59) >> 0xB;
    m->z_vel = v;
    m->z_max = v;
    SetHeroSeq((HeroInfo *)id, 0x50);
}

void func_800BA724(s32 id) {
    NAS *nas;

    if (Heroes[id].hero_anim->Aindex == 2) {
        nas = SpawnNASById(Heroes[id].hero_eng->unk10.x, Heroes[id].hero_eng->unk10.y, Heroes[id].hero_eng->unk10.z, 0x1903, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = (s32)&Heroes[id];
        }
    }
    func_800BA960(id);
    func_800BC774(id);
    func_8008502C(id);
    if ((Heroes[id].hero_status & 0x200) && Heroes[id].hero_mode != 0x68) {
        SetHeroAction((HeroInfo *)id, 0x68);
    }
}

s32 func_800BA810(s32 arg0) {
    Ovl65ExtB *a;
    Ovl65ExtB *b;

    a = FindExtension(Heroes[arg0].hero, 0x10000000);
    if (a != NULL) {
        if (a->unk14 != 0) {
            b = FindExtension(a->unk18, 0x10000000);
            if (b != NULL && b->unk10 == arg0) {
                if (((Ovl65Pad *)D_80051338)[arg0].unk50 & 0x8000) {
                    SetHeroAction((HeroInfo *)arg0, 0x65);
                    b->unkC = 2;
                    a->unk14 = 0;
                    return 1;
                }
            }
            return 0;
        }
        if (a->unk10 <= 0) {
            if (((Ovl65Pad *)D_80051338)[arg0].unk50 & 0x8000) {
                a->unk10 = 0x1E;
                SetHeroAction((HeroInfo *)arg0, 0x66);
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BA960);

s32 func_800BAD18(NAS *m) {
    Ovl65ExtB *me;
    Ovl65ExtB *he;
    s32 i;
    s32 dx;
    s32 dz;

    me = FindExtension(m, 0x10000000);
    if (me == NULL) {
        return 0;
    }
    i = 0;
    for (;;) {
        if (i >= 4) {
            goto done;
        }
        he =FindExtension(Heroes[i].hero, 0x10000000);
        if (he != NULL && he->unk14 == 0 && Heroes[i].hero_mode != 0x65 && me->unkC != 1
            && abs(m->Engine->unk10.y - Heroes[i].hero_eng->unk10.y) < 0x201
            && (me->unkC == 0 || me->unkC == 3 || me->unkC == 4)) {
            dx = Heroes[i].hero_eng->unk10.x - m->Engine->unk10.x;
            dz = Heroes[i].hero_eng->unk10.z - m->Engine->unk10.z;
            if (dx * dx + dz * dz <= 0x3FFFF) {
                me->unk10 = i;
                he = FindExtension(Heroes[i].hero, 0x10000000);
                if (he != NULL) {
                    he->unk14 = 1;
                    he->unk18 = m;
                }
                return 1;
            }
        }
        i++;
    }
done:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", func_800BAE94);

void InitDinoGem(NAS *m) {
    Ovl65DinoGem *ext;
    EXTengine *e;

    ext = AddExtension(m, 0xE8, 0x10000000);
    if (ext != NULL) {
        do {
            m->Engine->sub->unk74 = 0x2001;
        } while (0);
        if (D_8005A646 & 0x80) {
            m->Engine->sub->unk74 = 0x2001;
        }
        m->Engine->sub->unk6C = D_80055534;
        ext->unkC = 6;
        ext->unk10 = 5;
        ext->unk80 = 0;
        ext->unk88 = 0;
        ext->unk8C = 0;
        ext->unk90 = 1;
        ext->unkE2 = 0x100;
        ext->unkB4 = 0;
        ext->unkD4 = 0;
        func_80026A04((u8 *)m);
        m->Engine->sub->flags &= 0xFFFF7FFF;
        e = AddEngine(m, 2);
        if (e != NULL) {
            ext->unk84 = e;
            e->sub->unk6C = D_80055534;
            e->sub->unk74 = 0x2014;
            e->unk5C = 1;
            e->unk2C.x = 0x23E8;
            e->unk2C.y = 0x23E8;
            e->unk2C.z = 0x23E8;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->sub->unk76 = 0x800;
            e->sub->flags |= 0x40008041;
            ext->unk84->sub->flags &= 0xFFFF7FFF;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl65/ovl65", PlotDinoGem);

void PlotDinoScratch(NAS *m) {
    subengine *s;
    EXTengine *e;
    s32 dx;
    s32 v;
    u16 ang;

    s = m->Engine->sub;
    while (s != NULL) {
        if (!(s->flags & 0x8000)) {
            break;
        }
        s = s->next;
    }
    e = Heroes[m->UserInfo.user1].hero_shadow;
    if (e != NULL) {
        if ((e->unk10.x - e->OldXYZ.x) * (e->unk10.x - e->OldXYZ.x)
            + (e->unk10.z - e->OldXYZ.z) * (e->unk10.z - e->OldXYZ.z) > 0 && s != NULL
            && Heroes[m->UserInfo.user1].hero_mode == 0x66
            && Heroes[m->UserInfo.user1].hero_anim->Aindex >= 0xE) {
            if (Heroes[m->UserInfo.user1].hero_anim->Aindex == 0xE) {
                if (m->UserInfo.user1 == 0) {
                    PlaySfx(0x6AC, &Heroes[0].hero_eng->sfx, &Heroes[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 1) {
                    PlaySfx(0x6AD, &Heroes[1].hero_eng->sfx, &Heroes[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 2) {
                    PlaySfx(0x6AE, &Heroes[2].hero_eng->sfx, &Heroes[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (m->UserInfo.user1 == 3) {
                    PlaySfx(0x6AF, &Heroes[3].hero_eng->sfx, &Heroes[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
            }
            m->UserInfo.user2++;
            if (!(s->flags & 0x8000)) {
                *(SLONG_XYZ *)&s->unk04 = Heroes[m->UserInfo.user1].hero_eng->unk10;
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x > 0x1000) {
                    s->unk50 = 1;
                    dx = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = dx;
                    s->unk20.y = dx;
                    s->unk20.x = dx;
                }
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x < 0x1000) {
                    s->unk50 = 1;
                    v = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = v;
                    s->unk20.y = v;
                    s->unk20.x = v;
                }
                s->flags = 0x40008041;
                ang = Heroes[m->UserInfo.user1].hero_eng->unk1E;
                s->unk52 = 1;
                s->unk76 = 0x400;
                s->unk12 = ang + 0x800;
            }
        }
    }
    for (s = m->Engine->sub; s != NULL; s = s->next) {
        if (s->flags & 0x8000) {
            s->unk76 += 0x40;
            if ((s16)s->unk76 > 0xC00) {
                s->flags = 0;
                s->unk20.z = 0x1000;
                s->unk20.y = 0x1000;
                s->unk20.x = 0x1000;
            }
        }
    }
}

void func_800BC5B0(void) {
    D_800BDA08[0] = D_800BD9FC;
    D_800BDA08[1] = D_800BDA00;
    D_800BDA08[2] = D_800BDA04;
}

void InitDinoScratch(NAS *m) {
    EXTengine *eng;
    subengine *sub;

    if (func_80018F3C(m->Engine, 0x10) != 0) {
        eng = m->Engine;
        sub = eng->sub;
        eng->flags |= 0x200000;
        m->UserInfo.user1 = 0;
        m->UserInfo.user2 = 0;
        if (sub != NULL) do {
            sub->flags = 0x40000000;
            sub->unk6C = *(void **)D_80058AD0;
            sub->unk74 = 0x2003;
            sub->unk51 = 2;
            sub->unk14 = 0;
            sub->unk12 = 0;
            sub->unk10 = 0;
            sub->unk76 = 0x800;
            sub = sub->next;
        } while (sub != NULL);
    }
}

s32 func_800BC684(s32 *p, s32 arg1) {
    s32 d;
    s32 ang;

    d = csqrt(p[0] * p[0] + p[2] * p[2]) >> 6;
    if (0xB00 - arg1 < d) {
        d -= 0xB00;
        d += arg1;
        ang = get_angle(p[0], p[2]) & 0xFFF;
        d += 4;
        p[0] -= d * D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> 12;
        p[2] -= d * D_80068BD4[ang].unk2 >> 12;
        return ang;
    }
    return -1;
}

s32 func_800BC774(s32 arg0) {
    if ((csqrt(Heroes[arg0].hero_move->x_vel * Heroes[arg0].hero_move->x_vel +
               Heroes[arg0].hero_move->z_vel * Heroes[arg0].hero_move->z_vel) >> 6) < 0x20) {
        if (Heroes[arg0].hero_mode != 0x63) {
            SetHeroAction((HeroInfo *)arg0, 0x63);
        }
        return 1;
    }
    return 0;
}

s32 func_800BC820(s32 arg0) {
    if (((PadButtons *)D_80051338)[arg0].unk50 & 0xF0) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x63);
    return 1;
}

s32 func_800BC878(s32 arg0) {
    if (!(((PadButtons *)D_80051338)[arg0].unk50 & 0xF0)) {
        return 0;
    }
    SetHeroAction((HeroInfo *)arg0, 0x64);
    return 1;
}

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

void func_800BCAC0(s32 arg0) {
    EXTheromove *mv;

    mv = Heroes[arg0].hero_move;
    SetHeroSeq((HeroInfo *)arg0, 0x4D);
    if (Heroes[arg0].hero_eng->sfx != NULL) {
        StopSfx(Heroes[arg0].hero_eng->sfx);
    }
    mv->x_acc = 9;
    mv->z_acc = 9;
    mv->z_dec = 9;
    mv->x_dec = 9;
    mv->mass = 0x100;
}

void func_800BCB4C(s32 arg0) {
    SetHeroAction((HeroInfo *)arg0, 0x63);
    Heroes[arg0].hero_eng->unk1E = 0;
    Heroes[arg0].hero_eng->unk5E = 1;
    Heroes[arg0].hero->Camera.height = 0x600;
}

void func_800BCBB8(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        s32 av = a->x_vel;
        s32 bv = b->x_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        av = a->z_vel;
        bv = b->z_vel;
        d = -((av - bv) << 8);
        q = (d * mb + ((ma * av + mb * bv) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

s32 func_800BCCE8(s32 arg0) {
    s32 i;
    s32 best;

    best = -1;
    i = 3;
    do {
        if (func_8008C4C8((s32 *)&Heroes[arg0].hero_eng->unk10, i) != 0) {
            best = i;
        }
        i--;
    } while (i > 0);
    return best;
}

void InitDinoSmoke(NAS *arg0) {
    s16 *p;

    func_80024E30(arg0, &D_800BDC08);
    arg0->Engine->sub->unk6C = D_80055534;
    arg0->Engine->sub->flags |= 0x40000041;
    p = &D_8005A648;
    do {
        arg0->UserInfo.user1 = 0;
    } while (0);
    if (*p == 0x1C) {
        arg0->Engine->sub->flags = 0x40008021;
    }
}

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

s32 func_800BD0AC(s32 self, s32 *dist) {
    SLONG_XYZ pos;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        if (i != self &&!((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = func_8008CEB0((u8 *)&pos, (u8 *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                do {
                    best = i;
                } while (0);
            }
        }
        do {
            h++;
        } while (0);
        i++;
        goto loop;
    }
    *dist = min;
    return best;
}

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

s32 func_800BD204(s32 arg0) {
    Ovl65Ext *ext;
    s32 ret;

    ext = FindExtension(D_800BDA08[arg0], 0x10000000);
    if (ext != NULL) {
        ret = 1;
        if (ext->unkC == ret) {
            return ret;
        }
    }
    return 0;
}

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
