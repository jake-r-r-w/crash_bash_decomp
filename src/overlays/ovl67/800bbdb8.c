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

void func_800BD584(AngObj *o) {
    if (o->state == 0x5003) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x500B) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0xC00 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x5019) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ax -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x500C) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x501A) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0x400 - (s16)o->ax) << 20) >> 23;
    }
    if (o->state == 0x5004) {
        o->unk52 = 1;
        o->unk51 = 0;
        o->ay -= (s32)((0xC00 - (s16)o->ax) << 20) >> 23;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BD6E8);

void func_800BDB00(void)
{
  unsigned char one = 1;
  void (*new_var)(s32 arg0);
  D_800C417A = one;
  D_8005A6DE = 0;
  func_800BB070(one);
  Heroes[0].hero_eng->unk2C.x = 0x1000;
  Heroes[0].hero_eng->unk2C.y = 0x1000;
  Heroes[0].hero_eng->unk2C.z = 0x1000;
  Heroes[1].hero_eng->unk2C.x = 0x1000;
  (&(*(Heroes + 1)).hero_eng->unk2C)->y = 0x1000;
  Heroes[1].hero_eng->unk2C.z = 0x1000;
  Heroes[0].hero_eng->unk5E = 1;
  Heroes[1].hero_eng->unk5E = 1;
  func_80026F48();
  func_800B4138(0);
  func_800B4138(1);
  func_800B4138(3);
  D_8009D6E8.unk20 = func_800B447C;
  D_8009D6E8.unk68 = func_800B3888;
  new_var = (void (*)(s32))func_800B46DC;
  D_8009D6E8.unk24 = new_var;
  D_8009D6E8.unk18 = func_800BBB10;
  D_8009D6E8.unk1C = func_800BBA4C;
 ;
  D_8009F8B4 = func_80093038;
  func_800B6F4C();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BDC0C);

void func_800BE70C(void) {
    NAS *first;
    NAS *nas;

    if (((BossGameView *)D_8005A614)->unk75 != 0 && (((BossGameView *)D_8005A614)->unk30 & 1)) {
        ((BossGameView *)D_8005A614)->unk75 = 0;
    }
    if (((BossGameView *)D_8005A614)->unk4 & 8) {
        D_8009D6DC[2] |= 0x8000;
    }
    if (D_800C45F0 == 0) {
        D_800C4494 = NULL;
        D_800C4490 = NULL;
        D_800C76CC = Heroes[0].hero_eng->unk10;
        D_800C76D8 = Heroes[1].hero_eng->unk10;
        first = SpawnNASById(0, 0, 0, 0x1C03, 0x11);
        D_800C4490 = first;
        if (first != NULL) {
            first->UserInfo.user1 = 0;
            func_80026A04((u8 *)first);
        }
        if (((BossGameView *)D_8005A614)->unk6C != 0) {
            nas = SpawnNASById(0, 0, 0, 0x1C03, 0x11);
            D_800C4494 = nas;
            if (nas != NULL) {
                nas->UserInfo.user1 = 1;
                func_80026A04((u8 *)nas);
            }
        }
        func_80026980(Heroes[0].hero);
        func_80026980(Heroes[1].hero);
        D_800C45F0 = 1;
    }
    if (D_8005A624 == 0) {
        func_800BDC0C();
    }
}

void func_800BE8C0(s32 arg0) {
    HeroInfo *h;
    HeroInfo *me;
    EXTheromove *om;
    EXTheromove *mm;
    s32 i;
    s32 total;
    s32 dx;
    s32 dz;
    s32 dy;
    s32 dist;
    s32 ang;
    s32 push;
    s32 d;
    s32 q;
    s32 r;
    TrigEntry *trig;
    HeroInfo *base;

    base = Heroes;
    i = 0;
    total = 0x200;
    trig = D_80068BD4;
    while (1) {
        if (i >= 4) {
            break;
        }
        h = &base[i];
        if (i != arg0 && !(h->hero_status & 0xFA00)) {
            me = (HeroInfo *)(arg0 * sizeof(HeroInfo) + (u32)base);
            dx = h->hero_eng->unk10.x - me->hero_eng->unk10.x;
            dz = h->hero_eng->unk10.y - me->hero_eng->unk10.y;
            dy = h->hero_eng->unk10.z - me->hero_eng->unk10.z;
            dist = csqrt(dx * dx + dz * dz + dy * dy) >> 6;
            if (dist < 0x300) {
                om = h->hero_move;
                mm = me->hero_move;
                mm->mass = 0x100;
                om->mass = 0x100;
                if (mm->mass > 0) {
                    d = -((om->x_vel - mm->x_vel) << 7);
                    q = (mm->x_vel + om->x_vel) << 16;
                    q = ((d << 8) + q) / total;
                    r = q - d;
                    om->x_vel = q >> 8;
                    mm->x_vel = r >> 8;
                    d = -((om->jump_vel - mm->jump_vel) << 7);
                    q = (mm->jump_vel + om->jump_vel) << 16;
                    q = ((d << 8) + q) / total;
                    r = q - d;
                    om->jump_vel = q >> 8;
                    mm->jump_vel = r >> 8;
                }
                ang = (u16)get_angle(dx, dz) & 0xFFF;
                push = 0x304 - dist;
                me->hero_eng->unk10.x -= push * trig[(u16)(ang - 0x400) & 0xFFF].unk2 >> 12;
                me->hero_eng->unk10.y -= push * trig[ang].unk2 >> 12;
            }
        }
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BEB68);

void func_800BF0AC(s32 arg0) {
    EXTheromove *m;
    EXTengine *eng;
    HeroInfo *h;
    HeroInfo *hb;
    s32 v;
    s32 step;

    m = Heroes[arg0].hero_move;
    eng = Heroes[arg0].hero_eng;
    HERO_ACCEL(m, x_vel, x_max, x_acc, x_dec);
    HERO_ACCEL(m, z_vel, z_max, z_acc, z_dec);
    HERO_ACCEL(m, jump_vel, jump_max, jump_acc, jump_dec);
    hb = Heroes;
    h = hb + arg0;
    if (!(h->hero_status & 0x200)) {
        eng->unk10.x += m->x_vel;
        eng->unk10.z += m->z_vel;
        eng->unk10.y += m->jump_vel;
    }
    if (h->hero_eng->unk10.y < -0xB00) {
        h->hero_eng->unk10.y = -0xB00;
        m->jump_vel = 0;
    }
    if (h->hero_eng->unk10.y > 0) {
        h->hero_eng->unk10.y = 0;
        m->jump_vel = 0;
    }
    if (h->hero_eng->unk10.x < -0xA00) {
        h->hero_eng->unk10.x = -0xA00;
        m->x_vel = 0;
    }
    if (h->hero_eng->unk10.x > 0xA00) {
        h->hero_eng->unk10.x = 0xA00;
        m->x_vel = 0;
    }
    D_800C4184[arg0] = m->x_vel;
    if (D_800C4488 == 0 && (D_8005A680[0] & 1)) {
        func_800BE8C0(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800BF37C);

void func_800BF7C8(s32 arg0) {
    HeroInfo *h;
    NAS *nas;
    NAS *ret;
    LaserExt *ext;
    subengine *s;
    SLONG_XYZ *p;
    HeroInfo *base;

    func_80028018(arg0, 0xA);
    ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xA, 0xFF);
    base = Heroes;
    h = &base[arg0];
    h->hero_status |= 0xA000;
    if (arg0 == 0) {
        func_80026980(D_800C4490);
    }
    if (arg0 == 1) {
        func_80026980(D_800C4494);
    }
    ret = NULL;
    p = &h->hero_eng->unk10;
    nas = SpawnNASById(p->x, p->y, p->z, 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext->unkC = 5;
            ext->unk18 = 0x100;
            ext->unk20 = 1;
            ext->unk28 = 0;
            nas->InitDelete(nas);
        }
        ret = nas;
    }
    if (ret != NULL) {
        s = ret->Engine->sub;
        if (s != NULL) {
            do {
                s->flags = 0x02000000;
                s->unk68 = -0x200;
                s = s->next;
            } while (s != NULL);
        }
    }
}

void InitAsteroidRoll(NAS *m) {
    AsteroidExt *ext;
    EXTengine *e;

    ext = AddExtension(m, 0x80, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->unk6C = *(void **)D_80058AD0;
        m->Engine->unk5D = 1;
        ext->arc[0] = 0xC;
        ext->bounces = 0;
        ext->unk50 = 0xA0;
        ext->arc[1] = 0;
        ext->floor = 0;
        ext->to.x = Heroes->hero_eng->unk10.x;
        ext->to.z = (D_80068BD4[0].unk2 * 9) >> 4;
        m->Engine->unk1E = 0;
        ext->to.y = 0;
        ext->from.x = Heroes->hero_eng->unk10.x;
        ext->from.z = 0x1B00;
        ext->from.y = -0x100;
        m->Engine->unk10 = ext->from;
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

void PlotAsteroidRoll(NAS *nas) {
    AsteroidExt *ext;
    s32 n;

    ext = FindExtension(nas, 0x10000000);
    if (ext != NULL) {
        nas->Engine->unk1C += 0x200;
        if (func_8008C5C4(&ext->from.x, &ext->to.x, ext->arc, &nas->Engine->unk10.x) != 0) {
            ext->vy = -((ext->vy * 200) >> 8);
            nas->Engine->unk10.y = ext->floor;
            n = ext->bounces + 1;
            ext->vx *= 200;
            ext->bounces = n;
            ext->vz *= 200;
            ext->vx >>= 8;
            ext->vz >>= 8;
            if (n == 4 ||
                nas->Engine->unk10.x * nas->Engine->unk10.x +
                        nas->Engine->unk10.z * nas->Engine->unk10.z > 0x6400000) {
                KillNAS(nas);
                return;
            }
        }
        ext->shadow->unk10 = nas->Engine->unk10;
        ext->shadow->unk10.y = 0x100;
    }
    nas->Engine->unk5E = 1;
    ext->shadow->unk5E = 1;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroMine);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroMissile);

void func_800C0C28(BossPath *arg0, BossObj *obj) {
    BossObj *arg1 = obj;
    BossGameView *g = (BossGameView *)D_8005A614;
    s32 d;
    EXTengine *target;
    EXTengine *e;
    s32 tz;
    s32 dist;
    s32 ang[3];
    SLONG_XYZ pt;
    s32 *p;
    s32 *q;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 *out;

    if (g->unk10 != 0) {
        return;
    }
    do {
        if (g->unk6C == 0) {
            target = D_8009D530;
        } else if (!(Heroes[0].hero_status & 0x2000)) {
            target = Heroes[0].hero_eng;
        } else {
            target = Heroes[1].hero_eng;
        }
        e = arg1->eng;
        do {
            tz = target->unk10.z;
        } while (0);
        d = abs(tz - e->unk10.z);
    } while (0);
    if ((Heroes[0].hero_status & 0x2000) && (Heroes[1].hero_status & 0x2000)) {
        arg0->pos += D_800C4478;
    } else if (d < 0x2000) {
        arg0->pos += 0x400;
    }
    func_8001E21C(arg0->keys, arg0->pos, &pt);
    arg1->eng->unk10.x += (pt.x - arg1->eng->unk10.x) >> 4;
    arg1->eng->unk10.z += (pt.z - arg1->eng->unk10.z) >> 4;
    arg1->eng->unk10.y += (pt.y - arg1->eng->unk10.y) >> 4;
    p = (s32 *)func_8001E41C(arg0->keys, arg0->pos);
    out = ang;
    if (p[5] & 0x2000) {
        p -= 7;
    }
    dx = p[0] - p[7];
    q = p + 7;
    dz = p[2] - q[2];
    dy = p[1] - q[1];
    dist = csqrt(dx * dx + dz * dz) >> 6;
    out[1] = get_angle(dx, dz);
    ang[0] = -get_angle(dy, dist);
    arg1->eng->unk1E += ((ang[1] - arg1->eng->unk1E) << 20) >> 23;
    arg1->eng->unk1C += ((ang[0] - arg1->eng->unk1C) << 20) >> 23;
}

void InitNitroBoss(NAS *nas) {
    BossExt *ext = AddExtension(nas, 0x3C, 0x10000000);
    BossScene *scene;
    u8 *p;

    if (ext != NULL) {
        nas->Engine->sub->unk6C = *(void **)D_80058AD0;
        nas->Engine->unk2C.x = 0x1830;
        nas->Engine->unk2C.y = 0x1830;
        nas->Engine->unk2C.z = 0x1830;
        func_80086868();
        D_8009D6E2 = 0xFF;
        D_800C4530 = 0;
        ext->cursor = func_80024E30(nas, &D_800C446C);
        nas->Engine->unk5D = 2;
        nas->Engine->unk5C = 1;
        func_800250B0(nas);
        nas->UserInfo.user1 = 0;
        ext->unk14 = 0x3200;
        scene = D_80058AD0;
        p = scene->unk2C;
        ext->keys = (KeyBlock *)(p + (*(s32 *)(p + 4) + 4));
        func_8001E21C(ext->keys, ext->unk14, &nas->Engine->unk10);
        ext->unk18 = 0;
        ext->unk1C = 0;
        ext->unk20 = 0;
        ext->unk38 = 0;
        ext->unk30 = 0;
        ext->unk34 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotNitroBoss);

void InitSpiralSat(NAS *nas) {
    SpiralExt *ext = AddExtension(nas, 0x54, 0x10000000);
    EXTengine *e;
    NAS *s;
    LaserExt *le;
    NAS *ret;
    SLONG_XYZ *q;
    s32 i, mode;

    if (ext != NULL) {
        ext->c = nas->Engine->unk10;
        ext->unk3C = 0;
        ext->unk40 = Random(0x1000);
        i = 0;
        for (;; i++) {
            if (i >= 4) {
                goto done;
            }
            ext->sat[i] = NULL;
            e = AddEngine(nas, i + 2);
            if (e != NULL) {
                ext->ang[i] = i << 10;
                ext->eng[i] = e;
                ext->radius = 0x800;
                e->unk10.x = ext->c.x + ((ext->radius * D_80068BD4[(ext->ang[i] - 0x400) & 0xFFF].unk2) >> 12);
                e->unk10.y = ext->c.y + ((ext->radius * D_80068BD4[ext->ang[i] & 0xFFF].unk2) >> 12);
                e->unk10.z = ext->c.z;
                e->sub->unk6C = *(s32 *)D_80058AD0;
                e->sub->unk74 = 0x201A;
                e->sub->flags = 0x8000;
                q =&e->unk10;
                s = SpawnNASById(q->x, q->y, q->z, 0x1605, 0x11);
                mode = 2;
                if (s != NULL) {
                    le = FindExtension(s, 0x10000000);
                    if (le != NULL) {
                        le->unkC = mode;
                        le->unk18 = 0x100;
                        le->unk20 = 0;
                        le->unk28 = 0;
                        s->InitDelete(s);
                    }
                    ret = s;
                } else {
                    ret = NULL;
                }
                ext->sat[i] = ret;
            }
        }
    }
done:;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotSpiralSat);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotMineSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotLaserShoot);

void func_800C2218(SparkObj *arg0) {
    SparkSet *set = &arg0->set;
    s32 n = set->count;
    Spark *s = arg0->set.sparks;
    SparkLine *a;
    SparkLine *b;

    D_800C45B8 = 0x100;
    D_800C45B4 = 0x100;
    a = set->lines[0];
    b = set->lines[1];
    if (n != 0) do {
        s->x = Random(0x2000) - Random(0x2000);
        s->y = Random(0x1000) - Random(0x1000);
        s->z = Random(0x4000);
        s->x2 = s->x + 0x40;
        s->y2 = s->y + 0x40;
        a->x0 = 0;
        b->x0 = 0;
        a->y0 = 0;
        b->y0 = 0;
        a->r0 = 0x80;
        a->g0 = 0x80;
        a->b0 = 0x80;
        b->r0 = 0x80;
        b->g0 = 0x80;
        b->b0 = 0x80;
        a->len = 4;
        a->code = 0x50;
        b->len = 4;
        b->code = 0x50;
        a->code |= 2;
        b->code |= 2;
        a++;
        b++;
        s++;
        n--;
    } while (n != 0);
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C237C);

void func_800C27D4(void) {
    FollowCam *cam;
    EXTengine *e = Heroes->hero_eng;
    s32 y;

    cam = (FollowCam *)D_80051640;
    cam->x += (((e->unk10.x + Heroes[1].hero_eng->unk10.x) >> 1) - cam->x) >> 3;
    cam->z = e->unk10.z;
    y = e->unk10.y;
    cam->unk8 = 0x990;
    cam->unk0 = 0;
    cam->unk54 = 0x1A8;
    cam->y = y;
    cam->unk58 = e->unk20 >> 3;
}

void InitLaserShoot(LaserShoot *arg0) {
    arg0->obj->sub->unk6C = *(s32 *)D_80058AD0;
    arg0->obj->sub->unk74 = 0x201D;
    arg0->obj->unk1E = 0;
    arg0->obj->unk5D = 2;
    arg0->unk60 = 0;
    func_80026B70(&D_800C449C, (u8 *)arg0);
}

void InitMineSFX(MineSFX *arg0) {
    arg0->unk5C = Random(4);
    arg0->unk64 = Random(0x1000);
    arg0->unk60 = arg0->src->unk18;
}

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

void InitNitroMissile(MissileHolder *arg0) {
    MissileExt *ext = AddExtension(arg0, 0x1C, 0x10000000);
    s32 r;

    if (ext != NULL) {
        arg0->obj->model->unk6C = *(s32 *)D_80058AD0;
        arg0->obj->model->unk74 = 0x2019;
        arg0->obj->unk5D = 2;
        ext->unk10 = 0;
        ext->unkC = 0;
        ext->unk14 = 0;
        ext->unk18 = 0;
        r = Random(4);
        if (r == 0) {
            PlaySfx(0x2A3, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x2A4, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x2A5, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x2A6, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void InitNitroMine(MineHolder *arg0) {
    arg0->obj->model->unk6C = *(s32 *)D_80058AD0;
    arg0->obj->model->unk74 = 0x2012;
    arg0->obj->unk1C = Random(0x1000);
    arg0->obj->unk1E = Random(0x1000);
    arg0->obj->unk20 = Random(0x1000);
    arg0->obj->unk5E = 1;
}

void func_800C2BB0(void) {
    if (D_800C447C-- < 0) {
        SpawnNASById(0, 0, 0, 0x1C01, 0x11);
        D_800C447C = 0xF;
    }
}

void func_800C2C08(s32 arg0) {
    func_80085890(arg0);
    Heroes[arg0].hero_eng->unk1E = 0x800;
    Heroes[arg0].hero_move->forward_angle = 0x800;
    Heroes[arg0].hero_eng->unk5E = 1;
    if (func_8008A200(arg0) != 0) {
        func_8008A810();
    }
    func_8008502C(arg0);
}

void func_800C2C8C(s32 arg0) {
    func_80089FA4(arg0);
    func_80085718(arg0, 0);
    Heroes[arg0].hero_eng->unk20 = 0;
    D_800C4478 = 0;
    Heroes[arg0].hero_eng->unk1C = 0;
    Heroes[arg0].hero_eng->unk1E = 0x800;
    Heroes[arg0].hero_eng->unk5E = 1;
}

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

void func_800C2D80(s32 arg0) {
    HeroInfo *h;
    s32 act;

    HeroInfo *base = Heroes;
    h = &base[arg0];
    act = 0x2F;
    if (h->hero_health <= 0) {
        h->hero_health = 0;
        act = 0x30;
    }
    SetHeroAction((HeroInfo *)arg0, act);
}

void func_800C2DD8(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;

    SpawnNASById(e->unk10.x, e->unk10.y, e->unk10.z + 0x400, 0x1C09, 0x11);
    func_80028018(arg0, 0xA);
    ((void (*)(s32, s32, s32))func_80027F7C)(arg0, 0xA, 0xFF);
}

void func_800C2E58(void) {
}

void func_800C2E60(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTheromove *m = h->hero_move;

    SetHeroSeq((HeroInfo *)arg0, 0x22);
    func_800B4954(arg0);
    m->x_acc = 0x10;
    m->z_acc = 0x10;
    m->jump_acc = 0x10;
    m->mass = 0x100;
    m->z_dec = 1;
    m->x_dec = 1;
    m->jump_dec = 1;
    h->hero->UserInfo.user1 = 0;
}

void func_800C2EF8(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];

    h->hero_eng->unk1E += func_800154A0(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
    h->hero_eng->unk5E = 1;
}

s32 func_800C2F70(void) {
    return 0;
}

void func_800C2F78(MixObj *arg0, MixObj *arg1)
{
  MixObj *p = arg0;
  MixObj *q = arg1;
  s32 d;
  s32 s;
  int new_var;
  s32 t;
  s32 u;
  s32 n;
  q->w = 0x100;
  p->w = 0x100;
  if (q->w > 0)
  {
    d = -((p->a - q->a) << 7);
    s = (q->a + p->a) << 16;
    new_var = (d << 8) + s;
    n = 0x200;
    t = new_var / n;
    u = t - d;
    p->a = t >> 8;
    q->a = u >> 8;
    d = -((p->b - q->b) << 7);
    s = (q->b + p->b) << 16;
    t = ((d << 8) + s) / n;
    u = t - d;
    p->b = t >> 8;
    q->b = u >> 8;
  }
}

void PlotDummyCamera(NAS *nas) {
    nas->Camera.width = Heroes[nas->UserInfo.user1].hero->Camera.width;
    nas->Camera.height = Heroes[nas->UserInfo.user1].hero->Camera.height;
    nas->Camera.look_ahead = Heroes[nas->UserInfo.user1].hero->Camera.look_ahead;
    nas->Engine->unk10 = Heroes[nas->UserInfo.user1].hero_eng->unk10;
    if (nas->Engine->unk10.y < -0x900) {
        nas->Engine->unk10.y = -0x900;
    }
}

void InitDummyCamera(NAS *nas) {
    *(s32 *)((u8 *)nas->Engine + 0x14) = -0x480;
}

void func_800C3184(void) {
}

void func_800C318C(s32 arg0) {
    D_800C7614.sx = 0x1000;
    D_800C7614.sy = 0x1000;
    D_800C7614.sz = 0x1000;
    D_800C7614.unk8 = 0x8000;
    D_800C7614.unk18 = 0x140;
    D_800C7614.unk54 = 0;
    D_800C7614.unk56 = 0;
    D_800C7614.unk58 = 0;
    D_800C7614.x = 0;
    D_800C7614.y = 0;
    D_800C7614.z = 0;
    D_800C7614.unk95 = 0;
    D_800C7614.unk94 = 1;
    D_800C7614.unk0 = 0;
    D_800C7614.unk4 = 0;
    D_800C7614.unk8 = 0;
    D_800C7614.unk3C = 0;
    D_800C7614.unk38 = 0;
    D_800C7614.unk40 = 0;
    func_80093038();
    func_80018B08((GfxViewport *)D_8009F6F4, (GfxCamera *)&D_800C7614);
    D_800569AC = *(s32 *)D_8009F644;
    func_8009440C();
}

void func_800C3234(void) {
}

s32 func_800C323C(LaserPos p, s32 r) {
    LaserNode *n;
    LaserTgt *t;
    LaserExt *ext;
    NAS *nas;
    s32 dx, dy, dz;

    LaserPos *tp;
    s32 mode;

    n = (LaserNode *)D_800C449C;
loop:
    if (n != NULL) {
        t = n->tgt;
        if (t != NULL) {
            dx = t->pos.x - p.x;
            dy = t->pos.y - p.y;
            dz = t->pos.z - p.z;
            if (dx * dx + dy * dy + dz * dz < r) {
                tp = &t->pos;
                nas = SpawnNASById(t->pos.x, tp->y, tp->z, 0x1605, 0x11);
                mode = 3;
                do {
                    if (nas != NULL) {
                        ext = FindExtension(nas, 0x10000000);
                        if (ext != NULL) {
                            ext->unk18 = 0x100;
                            ext->unkC = mode;
                            ext->unk20 = 1;
                            ext->unk28 = 0;
                            nas->InitDelete(nas);
                        }
                    }
                } while (0);
                KillNAS((NAS *)n);
                return 1;
            }
        }
        n = n->next;
        goto loop;
    }
    return 0;
}

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

void func_800C3440(void) {
    D_800C4480 = 0;
    D_800C4488 = 0;
    D_800C44FC = 0;
    D_800C4500 = 0;
    D_800C4504 = 0;
    D_800C4508 = 0;
    Heroes->hero_eng->unk10.y = -0x480;
    D_800C448C = 1;
    Heroes[1].hero_eng->unk10.y = -0x480;
}

void func_800C349C(void) {
    NAS *nas;
    u8 *arg;

    func_8001E0A8((u8 *)D_80058AD0);
    func_800BC3A4();
    func_800BDC0C();
    nas = SpawnNASById(0, 0, 0, 0x1625, 0x11);
    if (nas != NULL) {
        nas->UserInfo.user2 = 1;
    }
    D_800C4478 = 0x100;
    D_800C4498 = 0;
    D_800C45F0 = 0;
    D_8009F8B4 = (void (*)(void))func_800C318C;
    arg = *(u8 **)D_80058AD0;
    D_800C449C = NULL;
    func_80095258(arg);
}

s32 func_800C3544(s32 *arg0, s32 *arg1) {
    Box2 *src;
    s32 d[3];
    BoxHalf lo;
    BoxHalf hi;

    src = (Box2 *)func_8008CE20((subengine *)arg0, 0, 0);
    if (src == NULL) {
        printf(D_800B336C);
        return 0;
    }
    lo = src->lo;
    hi = src->hi;
    d[0] = arg1[0] - arg0[1];
    d[1] = arg1[1] - arg0[2];
    d[2] = arg1[2] - arg0[3];
    if (lo.v[0] < d[0]) {
        if (d[0] < hi.v[0]) {
            if (hi.v[1] < d[1]) {
                if (d[1] < lo.v[1]) {
                    if (lo.v[2] < d[2] + 0x400) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

s32 func_800C365C(void) {
    u8 *g = D_8005A614;

    s32 r;

    if (g[0xCA] < 10) {
        r = 230;
    } else if (g[0xCA] < 30) {
        return 230 - (g[0xCA] - 10) * 179 / 20;
    } else {
        r = 51;
    }
    return r;
}

void func_800C36D4(void) {
}

void func_800C36DC(void) {
}

void PlotNitroSmokeBoost(NAS *nas) {
    func_800250B0(nas);
}

void InitNitroSmokeBoost(NAS *nas) {
    ScriptCursor *cur;

    cur = func_80024E30(nas, &D_800C44F0);
    if (D_8005A648 == 0x1F) {
        if (cur != NULL) {
            cur->unk1E = 4;
        }
    }
    nas->Engine->sub->unk6C = *(GfxModel **)D_80058AD0;
    nas->UserInfo.user1 = 0;
}
