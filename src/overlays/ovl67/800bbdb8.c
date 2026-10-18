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

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", InitSpiralSat);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotSpiralSat);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotMineSFX);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", PlotLaserShoot);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl67/800bbdb8", func_800C2218);

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
