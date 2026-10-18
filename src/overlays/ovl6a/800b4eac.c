#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl6a.h"
#include "bash/ovl6a_800b4eac.h"
#include "bash/hub.h"
#include "bash/n2o.h"
#include "bash/titles.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B4EAC);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B330C);

void func_800B5158(void) {
    s32 i;
    EngRec90248 *p;
    u8 *o;
    u8 *obj;
    EngSub90248 *e;
    Vec3W ref;
    Vec3W d;

    for (i = 0; i < 12; i++) {
        D_800BCCF4[i].unk0 = NULL;
        D_800BCCF4[i].unk4 = NULL;
        D_800BCCF4[i].unk18 = NULL;
        D_800BCCF4[i].unk8 = 0;
        D_800BCCF4[i].unkC = 0;
        D_800BCCF4[i].unk10 = 0;
        D_800BCCF4[i].unk14 = NULL;
        D_800BCCF4[i].unk1C = 0;
        D_800BCCF4[i].unk20 = 0;
        D_800BCCF4[i].unk24 = 0;
    }
    o = *(u8 **)((u8 *)D_80058AD0 + 0x1C);
    for (;;) {
        s32 k;
        void *g;
        EngRec90248 *q;

        if (o == NULL) {
            break;
        }
        i = o[0x65] - 1;
        if ((u32)i < 12) {
            switch (o[0x64]) {
            case 1:
                D_800BCCF4[i].unk0 = (s32 *)o;
                break;
            case 2:
                D_800BCCF4[i].unk4 = (s32 *)o;
                break;
            case 3:
                break;
            case 5:
                break;
            case 4:
                q = &D_800BCCF4[i];
                q->unk18 = o;
                k = *(s32 *)o;
                g = D_80058AD0;
                *(void **)(o + 0x54) = func_800BBE24;
                *(s32 *)o = k & ~0x8000;
                q->unk24 = func_80015984(*(u16 *)(o + 0x74), *(EntryBlock **)(*(u8 **)g + 0xC));
                break;
            }
        }
        o = *(u8 **)(o + 0x5C);
    }
    for (e = (EngSub90248 *)D_8009EDD4[D_8005A676][1]; e->unk0 != 0; e++) {
        i = e->unk0 - 1;
        p = &D_800BCCF4[i];
        p->unk14 = e;
        if (e->unk6 & 1) {
            if (p->unk0 != NULL) {
                *p->unk0 |= 0x8000;
            }
            if (p->unk4 != NULL) {
                *p->unk4 |= 0x8000;
            }
        } else {
            if (p->unk0 != NULL) {
                *p->unk0 &= ~0x8000;
            }
            if (p->unk4 != NULL) {
                *p->unk4 &= ~0x8000;
            }
        }
    }
    ref = *(Vec3W *)((u8 *)func_8001E48C(2) + 0x30);
    i = 0;
    for (;;) {
        s32 k;

        if (i >= 12) {
            break;
        }
        p = &D_800BCCF4[i];
        if (p->unk14 != NULL) {
            obj = (u8 *)func_8001E48C(i + 0x65);
            if (obj != NULL) {
                d = *(Vec3W *)(obj + 0x30);
                d.a -= ref.a;
                d.c -= ref.c;
                p->unk8 = csqrt(d.a * d.a + d.c * d.c) >> 6;
                p->unk10 = get_angle(d.a, d.c);
                d = *(Vec3W *)(obj + 0x4C);
                d.a -= ref.a;
                d.c -= ref.c;
                p->unkC = csqrt(d.a * d.a + d.c * d.c) >> 6;
            }
        }
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B551C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B5710);

void func_800B6224(s32 arg0) {
    EXTengine *e;
    subengine *sub;
    subengine *p;
    s32 w;
    s32 n;

    D_800BCA20[5].unk1E = 4;
    D_800BCA20[6].unk1E = 4;
    if (((Ovl6aGameView *)D_8005A614)->unk105[arg0] != 0) {
        D_800BCA20[5].unk1E = 3;
        if (((Ovl6aGameView *)D_8005A614)->unk105[arg0] >= 2) {
            D_800BCA20[6].unk1E = 2;
        }
    }
    n = ((Ovl6aGameView *)D_8005A614)->unkE0;
    if (n >= 2 && ((Ovl6aGameView *)D_8005A614)->unk105[arg0] == 0) {
        D_800BCEFC = 4;
    } else {
        D_800BCEFC = n + 3;
    }
    if (((Ovl6aGameView *)D_8005A614)->unk4 & 0x2000) {
        D_800BCEFC = ((Ovl6aGameView *)D_8005A614)->unkE0 + 3;
    }
    D_800BCA20[4].unk1E = ((Ovl6aGameView *)D_8005A614)->unk147[arg0] ? 7 : 8;
    D_800BCA20[3].unk1E = ((Ovl6aGameView *)D_8005A614)->unk126[arg0] ? 5 : 6;
    e = GetEngineSlot(1);
    func_80095BEC(1, (s32 *)D_800BCA20);
    e->unk10.y = 200;
    e->unk10.x = 0;
    D_800BCED4 = 0;
    e->flags = (e->flags & ~0x8000) | 0x10000000;
    if (((Ovl6aGameView *)D_8005A614)->unkE4[arg0] != 0) {
        D_800BCED4 = 1;
    }
    if (((Ovl6aGameView *)D_8005A614)->unk126[arg0] != 0) {
        D_800BCED4 = 2;
    }
    if (((Ovl6aGameView *)D_8005A614)->unk147[arg0] != 0 && ((Ovl6aGameView *)D_8005A614)->unkE0 > 0) {
        D_800BCED4 = 3;
    }
    if (((Ovl6aGameView *)D_8005A614)->unk105[arg0] != 0 && ((Ovl6aGameView *)D_8005A614)->unkE0 >= 2) {
        D_800BCED4 = 4;
        n = 1;
    } else {
        n = 1;
    }
    p = e->sub;
    w = (((D_800BCEFC << n) + D_800BCEFC) * 15) << n;
    p[1].unk76 = w;
    e->unk10.x = 320 - (w >> n);
    sub = GetEngineSlot(n)->sub;
    p = &sub[5];
    switch (D_800BCEFC - 3) {
    case 0:
        sub[5].flags &= ~0x8000;
        p[1].flags &= ~0x8000;
        break;
    case 1:
        p[1].flags &= ~0x8000;
        break;
    case 2:
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B64C8);

s32 func_800B694C(void) {
    s32 r = 0;
    s32 v;
    s32 *t;
    s32 k = 5;

    if (D_8005A677 >= 0) {
        v = D_800BCCF4[D_8005A677].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == k && D_800BCCF4[D_8005A677].unk20 != 0) {
        r = 1;
    }
    if (r != 0) {
        s32 (*base)[5];
        r = D_800BCEE0;
        if (r < 0) {
            return -1;
        }
        base = D_8009EDD4;
        t = base[D_8005A676];
        if (r < t[2]) {
            return ((s32 *)(t[3] + r * 8))[1];
        }
        return -1;
    }
    return -1;
}

void func_800B6A3C(void) {
    s32 (*base)[5] = D_8009EDD4;
    Ovl6aSelView *g = (Ovl6aSelView *)D_8005A614;
    Ovl6aSelEntry *ents = (Ovl6aSelEntry *)base[g->unk62][3];
    s32 r;
    s32 v;
    s32 k;
    s32 n;
    u8 *p;

    switch (D_800BCED8) {
    case 0:
        r = 0;
        k = 5;
        if (g->unk63 >= 0) {
            v = D_800BCCF4[g->unk63].unk14->unk2;
        } else {
            v = 0;
        }
        if (v == k && D_800BCCF4[D_8005A677].unk20 != 0) {
            r = 1;
        }
        if (r != 0 && ((Ovl6aSelView *)D_8005A614)->unk30 != 0x8008) {
            if ((D_80051380 & 0x20)
                && D_800BCEE0 < D_8009EDD4[((Ovl6aSelView *)D_8005A614)->unk62][2] - 1
                && ((s32(*)(s32))func_8007A6D0)(ents[D_800BCEE0 + 1].unk4)) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BCEE0++;
            }
            if ((D_80051380 & 0x80) && D_800BCEE0 > 0) {
                PlaySfx(0x39F, NULL, NULL, 0x1000, 0x1E00, 0);
                D_800BCEE0--;
            }
            if (D_800BCEE0 < 0) {
                D_800BCEE0 = func_800900CC();
            }
            if (D_800BCEE0 >= D_8009EDD4[D_8005A676][2]) {
                n = D_8009EDD4[D_8005A676][2];
                D_800BCEE0 = n - 1;
            }
            if (D_800BCEE8 != NULL) {
                if (D_800BCEE0 >= 0 && ((s32(*)(s32))func_8007A6D0)(ents[D_800BCEE0].unk4)) {
                    D_800BCEE8->unk74 = ents[D_800BCEE0].unk0;
                    return;
                }
                D_800BCEE8->unk74 = D_8009EDD4[D_8005A676][4];
            }
        } else if (D_800BCEE8 != NULL) {
            D_800BCEE8->unk74 = D_8009EDD4[D_8005A676][4];
        }
        break;
    case 1:
        if (func_80012FFC() == 0) {
            func_8001E588(D_8009F644, 0x200, 0);
            D_800BCED8 = 2;
        }
        break;
    case 2:
        p = D_8009F644;
        if (((s32 *)p)[2] >= 3) {
            func_8001DDD8(D_80058AD0);
            func_8001E054(D_80058AD0, D_800BCEE4);
            g->unk62 = D_800BCEDC;
            g->unk34 = base[g->unk62][0];
            func_800B4EAC();
            D_800BCED8 = 0;
            func_8001E588(p, -0x200, 0);
        }
        break;
    }
}

void func_800B6E20(void) {
    s32 *tbl;
    s32 n;
    s32 i;
    s32 *p;
    s32 *e;
    NAS *nas;
    Ovl6aSpawnExt *ext;

    tbl = NULL;
    if (*(s32 *)D_80058AD0 != 0) {
        tbl = *(s32 **)((u8 *)D_80058AD0 + 0x28);
    }
    i = 0;
    n = tbl[0];
    p = tbl + 1;
    while (1) {
        if (!(i < n)) {
            break;
        }
        e = (s32 *)((u8 *)p + *p);
        if ((u32)(e[3] - 0xC8) < 5) {
            nas = SpawnNASById(0, 0, 0, 0x1626, 0x11);
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    s32 *q = (s32 *)(*(u8 **)((u8 *)D_80058AD0 + 0x2C) + i * 4);
                    ext->unkC = (s32 *)((u8 *)q + *q);
                    if (e[3] == 0xC8) {
                        ext->unk34 = 0;
                    }
                    if (e[3] == 0xC9) {
                        ext->unk34 = 1;
                    }
                    if (e[3] == 0xCA) {
                        ext->unk34 = 2;
                    }
                    if (e[3] == 0xCB) {
                        ext->unk34 = 3;
                        nas->Engine->sub->flags &= 0xFFFF7FFF;
                    }
                    if (e[3] == 0xCC) {
                        ext->unk34 = 4;
                        nas->Engine->sub->flags &= 0xFFFF7FFF;
                    }
                }
            }
        }
        i++;
        p++;
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3618);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B6FD4);

void func_800B77B0(NAS *nas) {
    ScriptExt40 *ext = AddExtension(nas, 0x40, 0x10000000);

    if (ext != NULL) {
        ext->cursor = func_80024E30(nas, &D_800BCCE8);
        nas->Engine->sub->unk6C = *(void **)D_80058AD0;
        nas->Engine->sub->unk74 = 0x2018;
        nas->Engine->sub->flags &= 0xFEFFFFFF;
        nas->Engine->unk5D = 2;
        ext->unk18 = 0;
        ext->unk24 = 0xC;
        ext->unk14 = 0;
        ext->unk38 = 0;
        ext->unk3C = 0;
    }
}

void func_800B7874(void) {
}

void func_800B787C(FxObj *arg0) {
    arg0->mid->inner->unk6C = **(s32 **)&D_80058AD0;
    arg0->mid->inner->unk74 = 0x200E;
    arg0->mid->inner->unk51 = 0;
    arg0->mid->inner->unk50 = 1;
    arg0->mid->inner->unk20 = 0x1000;
    arg0->mid->inner->unk24 = 0x1000;
    arg0->mid->inner->unk28 = 0x1000;
    arg0->mid->inner->unk10 = 0;
    arg0->mid->inner->unk12 = 0;
    arg0->mid->inner->unk14 = 0;
    arg0->mid->inner->unk52 = 1;
    arg0->mid->inner->flags |= 0x60000000;
    arg0->unk60 = 0;
}

void func_800B7988(s32 *arg0, s32 flag, s32 *out) {
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

void func_800B7A78(void) {
    D_800BCEE8 = func_8001DCD4(1, 0x14);
    D_800BCED8 = 0;
    D_800BCEE0 = -1;
}

s32 func_800B7AB4(s32 arg0) {
    s32 r = 0;
    s32 v;

    if (D_8005A677 >= 0) {
        v = D_800BCCF4[D_8005A677].unk14->unk2;
    } else {
        v = 0;
    }
    if (v == arg0 && D_800BCCF4[D_8005A677].unk20 != 0) {
        r = 1;
    }
    return r;
}

u16 func_800B7B38(void) {
    if (D_8005A677 >= 0) {
        return D_800BCCF4[D_8005A677].unk14->unk2;
    }
    return 0;
}

void func_800B7B7C(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void func_800B7BA4(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = *(s32 *)D_8009F644;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void func_800B7C78(void) {
    func_800B64C8();
}

void func_800B7C98(void) {
    D_800BCED4 = -1;
}

void func_800B7CA8(void) {
    EXTengine *e;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = *(s32 *)D_8009F644;
    if (D_800BCED4 >= 0) {
        if (!(D_8005A624 & 1)) {
            e = GetEngineSlot(1);
            e->flags |= 0x8000;
            e->unk10.y = 0x1D6 - (D_800BF260 >> 5);
            func_8001C2C0((u8 *)GetEngineSlot(1));
        }
    }
    if (D_800BF260 != 0 && !(D_8005A624 & 1)) {
        func_8001C2C0((u8 *)GetEngineSlot(2));
    }
}

void func_800B7D7C(void) {
    D_800BCED4 = -1;
}

void func_800B7D8C(void) {
    func_80095A04(1);
    func_80095A04(2);
}

void func_800B7DB4(u8 *arg0, s32 arg1) {
    u8 *temp_a2;

    temp_a2 = (*(s32*)((u8*)(arg0) + 84));
    if (temp_a2 != NULL) {
        if (arg1 != 0) {
            (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) | 0x8000);
            (*(s16*)((u8*)(arg0) + 92)) = 1;
            return;
        }
        (*(s32*)((u8*)(temp_a2) + 12)) = (s32) ((*(s32*)((u8*)(temp_a2) + 12)) & 0xFFFF7FFF);
        (*(s16*)((u8*)(arg0) + 92)) = 0;
    }
}

void PlotGatewayTrophy(void) {
}

void InitGatewayTrophy(void) {
}

void func_800B7E14(s32 *arg0, s32 arg1) {
    NAS *nas;
    NAS *r;
    s32 *ext;
    EXTengine *eng;
    u8 *p;

    p = D_800558EC;
    nas = SpawnNASById(arg0[0], arg0[1], arg0[2], 0x1605, 0x11);
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL) {
            ext[3] = arg1;
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
            eng->flags |= 0x20000000;
        }
    }
    PlaySfx(0x398, NULL, NULL, 0x1000, 0x1E00, 0);
}

s32 func_800B7F24(DistPos *a, DistPos *b) {
    s32 dx = b->x - a->x;
    s32 dz = b->z - a->z;
    return csqrt(dx * dx + dz * dz) >> 6;
}

s32 func_800B7F74(u8 *arg0, u8 *arg1) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = (*(s32*)((u8*)(arg1) + 16)) - (*(s32*)((u8*)(arg0) + 16));
    temp_v0_2 = (*(s32*)((u8*)(arg1) + 24)) - (*(s32*)((u8*)(arg0) + 24));
    return (temp_v0 * temp_v0) + (temp_v0_2 * temp_v0_2);
}

void func_800B7FAC(s32 arg0) {
    u8 *p;
    D_8009E5DC = arg0;
    p = D_8005A614;
    *(u16 *)(p + 0x30) = 0x8008;
    *(u16 *)(p + 0x24) = 0xA;
}

void func_800B7FD0(u8 *cam, s32 arg1) {
    u8 *o = (u8 *)func_8001E48C(arg1);
    s32 *p;
    s32 t;

    if (o != NULL) {
        *(s16 *)(cam + 0x54) = (get_angle(*(s32 *)(o + 0x58) - *(s32 *)(o + 0x3C),
                                          *(s32 *)(o + 0x50) - *(s32 *)(o + 0x34)) - 0x400) & 0xFFF;
        *(s16 *)(cam + 0x56) = 0;
        *(s16 *)(cam + 0x58) = 0;
        *(s32 *)(cam + 0x18) = (((s32)(*(u16 *)(o + 0x40) << 16)) >> 18) + 0x200;
        do {
            *(s32 *)(cam + 0xC) = *(s32 *)(o + 0x30);
        } while (0);
        p = (s32 *)(o + 0x30);
        t = p[1];
        *(s32 *)(cam + 0x10) = t;
        t = p[2];
        *(s32 *)(cam + 0x14) = t;
        *(s32 *)(cam + 4) = -p[1];
        *(s32 *)cam = 0;
        *(s32 *)(cam + 8) = *(s32 *)(o + 0x58);
    }
}

void func_800B8098(u8 *arg0, s32 arg1) {
    CamView *cam = (CamView *)arg0;
    struct {
        CamView v;
        u8 pad[0xB8 - sizeof(CamView)];
    } buf;
    s32 n;
#define v buf.v

    n = 1;
    cam->dist = 0x200;
    if (D_800BCEF4 != 0) {
        n = 3;
    }
    func_800B7FD0((u8 *)&v, n);
    v.at = *(CamXYZ *)&D_8009D530->unk10;
    v.rot.vy = -D_8009EEA0;
    if (arg1 != 0) {
        func_80013F44(cam, &v, NULL);
    } else {
        func_80013F44(cam, &v, &D_800BCBAC);
    }
}
#undef v

void func_800B8148(s32 arg0) {
    Heroes[arg0].hero_idle_count = 0xF;
}

s32 func_800B8174(s32 arg0) {
    if (D_8005A688 != 0) {
        return 1;
    }
    if (((PadButtons *)D_80051338)[arg0].unk54 & 0x8000) {
        if ((u32)(Heroes[arg0].hero_name - 1) >= 2U) {
            if (Heroes[arg0].hero_frozen == 0) {
                Heroes[arg0].hero_frozen = 0xF;
            }
        } else {
            D_800BCEF0 = 1;
        }
    }
    return 0;
}

s32 func_800B821C(void) {
    return 0;
}

s32 func_800B8224(void) {
    return 0;
}

void func_800B822C(s32 arg0) {
    if (Heroes[arg0].hero->UserInfo.user1 != 0) {
        D_800BCEF4 += 1;
        return;
    }
    D_800BCEF4 = 0;
}

void func_800B8284(void) {
    volatile s32 v[2];

    s32 a, b;

    a = D_80068BD4[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[0] = D_80068BD4[(D_8009EE9C.unk4 - 0x400) & 0xFFF].unk2;
    b = D_80068BD4[D_8009EE9C.unk4 & 0xFFF].unk2 * D_8009EE9C.unk8;
    v[1] = D_80068BD4[D_8009EE9C.unk4 & 0xFFF].unk2;
    v[0] = a;
    a >>= 12;
    v[0] = a;
    v[1] = b;
    b >>= 12;
    v[1] = b;
    Heroes->hero_eng->unk10.x = a;
    Heroes->hero_eng->unk10.z = v[1];
}

void func_800B8324(s32 arg0) {
    HeroInfo *hero;
    EXTengine *shadow;

    HeroInfo *base = Heroes;

    hero = &base[arg0];
    shadow = hero->hero_shadow;

    if (shadow != NULL) {
        EXTengine *eng;

        shadow->flags |= 0x8000;
        eng = hero->hero_eng;
        shadow->unk10 = eng->unk10;
    }
}

void func_800B838C(s32 arg0) {
    EXTengine *s = Heroes[arg0].hero_shadow;

    if (s != NULL) {
        s->flags &= 0xFFFF7FFF;
    }
}

void func_800B83D4(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = &base[arg0];
    EXTengine *s = h->hero_shadow;

    if (s != NULL) {
        s->sub->unk6C = D_800558EC;
        s->sub->unk74 = 0x2020;
        s->unk10 = h->hero_eng->unk10;
        s->flags |= 0x8041;
        s->sub->unk76 = 0x800;
        s->sub->flags |= 0x40000000;
    }
}

void func_800B8474(s32 arg0) {
    u8 *o;

    func_800B83D4(arg0);
    o = (u8 *)func_8001E48C(2);
    if (o != NULL) {
        D_8009EE9C.unkC = *(s32 *)(o + 0x58);
        D_8009EE9C.unk10 = *(s32 *)(o + 0x74);
    }
    func_800B5710(arg0);
}

s32 func_800B84CC(void) {
    EngRec90248 *p;
    s32 i;
    s32 best;
    s32 bestDist;
    s32 d;

    best = -1;
    bestDist = 0x1000;
    for (i = 0; i < 12; i++) {
        p = &D_800BCCF4[i];
        if (p->unk14 != NULL && (p->unk14->unk6 & 2)) {
            d = func_80015480(p->unk10, D_8009EE9C.unk4);
            if (d < bestDist) {
                bestDist = d;
                if ((u32)d < 0x81U) {
                    best = i;
                }
            }
        }
    }
    return best;
}

void func_800B8588(void) {
    func_8009440C();
}

void func_800B85A8(void) {
    if (D_8005A688 == 0) {
        func_800B551C();
        func_800B6A3C();
        func_800139A8();
    }
}

void func_800B85E4(void) {
    u8 *o;
    u8 *cam;
    s32 a;

    o = *(u8 **)((u8 *)D_80058AD0 + 0x1C);
    cam = D_80051640;
    for (;;) {
        if (o == 0) {
            return;
        }
        if (o[0x64] == 0) {
            a = (-(get_angle(*(s32 *)(o + 4), *(s32 *)(o + 0xC)) & 0xFFF) - *(s16 *)(cam + 0x56)) << 20 >> 20;
            *(s32 *)o &= 0xFFFF7FFF;
            if (a >= 0x301) {
                *(s32 *)o |= 0x8000;
            }
            if (a < -0x300) {
                *(s32 *)o |= 0x8000;
            }
        }
        o = *(u8 **)(o + 0x5C);
    }
}

void func_800B86AC(void) {
    ((Ovl6aGameView *)D_8005A614)->unk51 = 9;
    ((Ovl6aGameView *)D_8005A614)->unk52 = 9;
    ((Ovl6aGameView *)D_8005A614)->unk53 = 9;
    D_8009D6E8.unk0 = (s32)func_800B8474;
    D_8009D6E8.unk20 = (void *)func_800B5710;
    D_8009D6E8.unk34 = (void *)func_800B822C;
    ((Ovl6aGameView *)D_8005A614)->unk50 = ((Ovl6aGameView *)D_8005A614)->unk4C;
    ((Ovl6aGameView *)D_8005A614)->unk32 |= 0x200;
}

s32 func_800B8708(s32 arg0) {
    KeyedPair *p = D_800BCB54;
    s32 i = 10;
    u8 *g = D_8005A614;

    for (; i != -1; i--) {
        if (p->key == arg0) {
            return p->val[*(s32 *)(g + 0x2C)];
        }
        p++;
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B37EC);

void func_800B875C(void) {
    s32 i;

    D_800BD594 = NULL;
    for (i = 0; i < 4; i++) {
        D_800BD584[i] = 0;
        switch (D_8005A614[i + 0x4C]) {
        case 1: func_80016D44(D_80055924); break;
        case 2: func_80016D44(D_8005595C); break;
        case 3: func_80016D44(D_80055994); break;
        case 4: func_80016D44(D_800559CC); break;
        case 5: func_80016D44(D_80055A04); break;
        case 6: func_80016D44(D_80055A3C); break;
        case 7: func_80016D44(D_80055A74); break;
        case 8: func_80016D44(D_80055AAC); break;
        }
    }
    ((Ovl6aGameView *)D_8005A614)->unk50 = 9;
    ((Ovl6aGameView *)D_8005A614)->unk51 = 9;
    ((Ovl6aGameView *)D_8005A614)->unk52 = 9;
    ((Ovl6aGameView *)D_8005A614)->unk53 = 9;
    D_800BD5B0 = 0;
    ((Ovl6aGameView *)D_8005A614)->unk32 |= 0x200;
    func_800B8AA4();
    func_800B8BC8();
    *(s32 *)&D_8005A5E8[((Ovl6aGameView *)D_8005A614)->unk8] = 0xCD;
    ((Ovl6aGameView *)D_8005A614)->unk3C = 0xCD;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800B88B8);

void func_800B8AA4(void) {
    s32 mask = 0;
    s32 i;
    s32 j;
    s32 best;
    u8 *p;
    u8 *q;

    for (i = 0; i < 4; i++) {
        best = -1;
        for (j = 0; j < 4; j++) {
            if (!((mask >> j) & 1)) {
                if (D_8005A614[0x9F + j] > D_8005A614[0x9F + best] || best == -1) {
                    best = j;
                }
            }
        }
        D_8005A614[0xA7 + i] = best;
        mask |= 1 << best;
    }
    for (i = 0; i < 4; i++) {
        s32 k;
        k = 4;
        q = D_8005A614 + i;
        if (i == 0) {
            D_800BD598[0] = k;
        } else {
            if (D_8005A614[q[0xA7] + 0x9F] == D_8005A614[(p = D_8005A614 + (j = i - 1))[0xA7] + 0x9F]) {
                D_800BD598[i] = D_800BD598[i - 1];
            } else {
                D_800BD598[i] = D_800BD598[j] - 1;
            }
        }
    }
}

void func_800B8BC8(void) {
    u8 *g = D_8005A614;
    s32 a = g[g[0xA7] + 0x9F];
    s32 b = g[g[0xA8] + 0x9F];
    s32 c = g[g[0xA9] + 0x9F];
    s32 d = g[g[0xAA] + 0x9F];

    D_800BD5A8 = 0;
    if (a == d) {
        D_800BD5A8 = 7;
        return;
    }
    if (b == d) {
        D_800BD5A8 = 5;
        return;
    }
    if (a == c) {
        D_800BD5A8 = 6;
        return;
    }
    if (c == d && a != b) {
        D_800BD5A8 = 1;
        return;
    }
    if (b == c) {
        D_800BD5A8 = 2;
        return;
    }
    if (a == b) {
        if (c != d) {
            D_800BD5A8 = 3;
            return;
        }
        D_800BD5A8 = 4;
    }
}

void func_800B8CA0(void) {
    s32 r;
    s32 x;

    if (D_8005A624 == 0 && D_800BD390-- < 0) {
        D_800BD394 = Random(4);
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x355, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x356, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x357, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x358, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x359, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x35A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        r = Random(6);
        if (r == 0) {
            PlaySfx(0x35B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x35C, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 2) {
            PlaySfx(0x35D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 3) {
            PlaySfx(0x35E, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 4) {
            PlaySfx(0x35F, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 5) {
            PlaySfx(0x360, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        x = Random(10000) - 5000;
        SpawnNASById(x, Random(10000) - 5000, 0xE10, 0x1628, 0x11);
        D_800BD390 = Random(10) + 5;
    }
}

void func_800B8F50(void) {
    s32 r;
    s32 x;

    switch (D_8005A614[D_8005A614[0xA7] + 0x4C]) {
    case 3:
    case 4:
    case 6:
    case 8:
        if (D_8005A624 == 0 && D_800BD390-- < 0) {
            D_800BD394 = Random(2) + 2;
            goto spawn;
        }
        break;
    default:
        if (D_8005A624 == 0 && D_800BD390-- < 0) {
            D_800BD394 = Random(2);
        spawn:
            r = Random(6);
            if (r == 0) {
                PlaySfx(0x355, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x356, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x357, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x358, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 4) {
                PlaySfx(0x359, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 5) {
                PlaySfx(0x35A, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            r = Random(6);
            if (r == 0) {
                PlaySfx(0x35B, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x35C, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x35D, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x35E, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 4) {
                PlaySfx(0x35F, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 5) {
                PlaySfx(0x360, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            x = -Random(0x2000) + Random(0x2000);
            SpawnNASById(x, Random(0x400) - 0xC00, 0x1000, 0x1628, 0x11);
            D_800BD390 = Random(0x14) + 0xA;
        }
        break;
    }
}

void PlotPodiumOdo(NAS *arg0) {
    EXTengine *eng;
    EXTanimator *anim;
    s16 idx;

    eng = arg0->Engine;
    if (eng != NULL) {
        anim = FindExtension(arg0, 0x400);
        if (anim != NULL && ((idx = anim->ALindex) == 0 || idx == 4)) {
            if (arg0->UserInfo.status == 0) {
                if (arg0->UserInfo.flags != 0) {
                    func_80025FAC(anim, 5);
                    arg0->UserInfo.status = Random(0x28);
                } else {
                    func_80025FAC(anim, 1);
                    switch (arg0->UserInfo.user2) {
                    case 3:
                        arg0->UserInfo.status = Random(0x1E) + 0x14;
                        break;
                    case 4:
                        arg0->UserInfo.status = Random(0x3C) + 0x3C;
                        break;
                    default:
                        arg0->UserInfo.status = Random(0x28) + 0xA;
                        break;
                    }
                }
            } else {
                arg0->UserInfo.status = arg0->UserInfo.status - 1;
            }
        }
        if (eng->flags & 0x8000) {
            func_800250B0(arg0);
        }
    }
}

void func_800B93B4(s32 arg0, s32 arg1) {
    PodiumScriptObj *obj;
    PodiumMid *mid;

    obj = (PodiumScriptObj *)D_800BD584[arg0];
    if (obj != NULL) {
        mid = obj->mid;
        if (mid != NULL) {
            switch (arg1) {
            case 1:
                mid->inner->script = D_80055924;
                func_80024E30((NAS *)obj, &D_800BD008);
                break;
            case 2:
                mid->inner->script = D_8005595C;
                func_80024E30((NAS *)obj, &D_800BCFB8);
                break;
            case 3:
                mid->inner->script = D_80055994;
                func_80024E30((NAS *)obj, &D_800BD070);
                break;
            case 4:
                mid->inner->script = D_800559CC;
                func_80024E30((NAS *)obj, &D_800BCF68);
                break;
            case 5:
                mid->inner->script = D_80055A04;
                func_80024E30((NAS *)obj, &D_800BD18C);
                break;
            case 6:
                mid->inner->script = D_80055A3C;
                func_80024E30((NAS *)obj, &D_800BD1E4);
                break;
            case 7:
                mid->inner->script = D_80055A74;
                func_80024E30((NAS *)obj, &D_800BD0D8);
                break;
            case 8:
                mid->inner->script = D_80055AAC;
                func_80024E30((NAS *)obj, &D_800BD134);
                break;
            }
            obj->unk64 = arg1;
            func_800250B0((NAS *)obj);
            mid->flags |= 0x8000;
        }
    }
}

void PlotPodiumTrophy(PodiumTrophy *arg0) {
    TrophyInner *in;
    PodiumFx *fx;
    TrigEntry *tb;
    s32 t;
    u16 st;
    s32 v;

    in = arg0->inner;
    if (in != NULL) {
        st = arg0->state;
        if (st == 1) {
            in->unk2C += (0x1000 - in->unk2C) >> 5;
            if (in->unk2C >= 0xF00) {
                in->unk2C = 0x1000;
                arg0->state = 2;
            }
            v = in->unk2C;
            in->unk5E = st;
            in->unk34 = v;
            in->unk30 = v;
        }
        fx = in->fx;
        if (fx != NULL) {
            tb = D_80068BD4;
            t = D_8005B690;
#define TSIN(k) tb[((((t * (k)) & 0xFFF) - 0x400) & 0xFFF)].unk2
            fx->unk10 = *(s32 *)&tb[(((t * 0x46) & 0xFFF) - 0x400) & 0xFFF] >> 20;
            fx->unk12 = (TSIN(0x8C) + 0x800) >> 5;
            fx->unk14 = (TSIN(0x46) + TSIN(0x64)) >> 5;
            fx->unk4 = tb[((t * 0x28) - 0x400) & 0xFFF].unk2 * 0x118 / 4096;
            fx->unk8 = (TSIN(0x2D) + TSIN(0x50)) >> 6;
#undef TSIN
            fx->unk52 = 1;
        }
    }
}

void func_800B96F4(CamView *out, s32 arg1) {
    SLONG_XYZ key;
    CamXYZ d;
    u8 *o;
    PodiumChild *c;
    s32 id;
    s32 end;
    s32 t;

    if (arg1 != 0) {
        D_800BD5B0 = 0;
        switch (D_8005A614[D_8005A614[0xA7] + 0x4C]) {
        case 1:
        case 2:
            PlaySfx(0x352, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 5:
        case 7:
            PlaySfx(0x353, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        case 3:
        case 4:
        case 6:
        case 8:
            PlaySfx(0x354, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        }
    }
    id = 2;
    if (D_800BD5B0 < 2) {
        id = 1;
    }
    o = (u8 *)func_8001E48C(id);
    if (arg1 != 0) {
        t = *(s32 *)(o + 0x58);
        D_800C7F04 = *(CamXYZ *)(o + 0x30);
        D_800C7EF4 = t;
    }
    func_8001E21C((KeyBlock *)o, D_800C7EF4, &key);
    D_800C7F04.x += (*(s32 *)(o + 0x30) - D_800C7F04.x) >> 4;
    D_800C7F04.y += (*(s32 *)(o + 0x34) - D_800C7F04.y) >> 4;
    D_800C7F04.z += (*(s32 *)(o + 0x38) - D_800C7F04.z) >> 4;
    out->at = D_800C7F04;
    d.x = D_800C7F04.x - key.x;
    d.y = D_800C7F04.y - key.y;
    d.z = D_800C7F04.z - key.z;
    out->eye.z = csqrt(d.x * d.x + d.y * d.y + d.z * d.z) >> 6;
    out->eye.x = 0;
    out->eye.y = 0;
    out->rot.vy = get_angle(-d.x, d.z);
    out->rot.vx = get_angle(d.y, csqrt(d.x * d.x + d.z * d.z) >> 6);
    out->rot.vz = 0;
    switch (D_800BD5B0) {
    case 0:
        end = *(s32 *)(o + 4);
        D_800C7EF4 += 0x20;
        if (D_800C7EF4 >= end) {
            D_800C7EF4 = end;
            D_800BD5AC = 0x3C;
            D_800BD5B0 = 1;
        }
        break;
    case 1:
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 2;
            D_800C7EF4 = *(s32 *)((u8 *)func_8001E48C(2) + 0x58);
            if (D_800BD594 != NULL) {
                c = D_800BD594->child;
                if (c != NULL && D_800BD594->f5C == 0) {
                    D_800BD594->f5C = 1;
                    c->f5C = 1;
                    c->f34 = 0;
                    c->f30 = 0;
                    c->f2C = 0;
                    c->f5E = 1;
                    c->flags |= 0x8000;
                }
            }
        }
        break;
    case 2:
        end = *(s32 *)(o + 4);
        D_800C7EF4 += 0x20;
        if (D_800C7EF4 >= end) {
            D_800BD5AC = 0xF0;
            D_800C7EF4 = end;
            D_800BD5B0 = 3;
            D_800BD5B4 = 1;
        }
        break;
    case 3:
        func_800BA848();
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 4;
        }
        break;
    case 4:
        D_800BD5B0 = 5;
        D_800BD5AC = 0x3C;
        break;
    case 5:
        func_800BA804();
        if (--D_800BD5AC <= 0) {
            D_800BD5B0 = 0x64;
        }
        break;
    case 0x64:
        break;
    }
    out->dist = 0x200;
    D_8009F708 = 0x15E;
}

void func_800B9B0C(void) {
    EXTengine *e;
    s32 n;

    e = GetEngineSlot(6);
    if (e != NULL) {
        n = D_8005A614[0x9F];
        D_800BD3AC[1] = n % 10 + 0x30;
        D_800BD3AC[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA0];
        D_800BD3B0[1] = n % 10 + 0x30;
        D_800BD3B0[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA1];
        D_800BD3B4[1] = n % 10 + 0x30;
        D_800BD3B4[0] = n / 10 % 10 + 0x30;
        n = D_8005A614[0xA2];
        D_800BD3B8[1] = n % 10 + 0x30;
        D_800BD3B8[0] = n / 10 % 10 + 0x30;
        func_80095BEC(6, &D_800BD3C4);
        e->unk10.x = 0x28;
        e->unk10.y = 0x21C;
        e->flags |= 0x10008000;
    }
}

void func_800B9D1C(NAS *arg0) {
    u8 *nv;
    subengine *s;

    arg0->Engine->sub->flags = 0x8000;
    arg0->UserInfo.user1 = 0;
    arg0->Engine->unk5D = 0;
    arg0->Engine->unk5C = 1;
    if (func_80018F3C(arg0->Engine, 12) != 12 || func_80015D70(D_800BD570, (u8 *)arg0->Engine->sub) == 0) {
        KillNAS(arg0);
        return;
    }
    s = arg0->Engine->sub;
    s = s->next;
    if (s != NULL) {
        u32 fl;
        do {
            s->unk51 = 0;
            nv = D_800558EC;
            s->unk50 = 1;
            s->unk6C = nv;
            if (D_800BD394 == 0) {
                s->unk74 = 0x2021;
            }
            if (D_800BD394 == 1) {
                s->unk74 = 0x2025;
            }
            if (D_800BD394 == 2) {
                s->unk74 = 0x2022;
            }
            if (D_800BD394 == 3) {
                s->unk74 = 0x2024;
            }
            if (D_800BD394 == 4) {
                s->unk74 = 0x2027;
            }
            if (D_800BD394 == 5) {
                s->unk74 = 0x2026;
            }
            s->unk20.x = 0x1000;
            fl = 0x20000000;
            s->unk20.y = 0x1000;
            s->unk20.z = 0x1000;
            s->flags = fl;
            s->unk76 = 0;
            s = s->next;
        } while (s != NULL);
    }
}

void func_800B9E90(Ovl6aSparks *arg0) {
    s32 n;
    Ovl6aFt4 *src;
    Ovl6aFt4 *dst;
    Ovl6aSparkRow *row;
    Ovl6aTex *tex;
    TrigEntry *a;
    TrigEntry *b;
    s32 ang;
    s32 k;
    Ovl6aSparkSet *set;
    u8 *g;
    s32 sel;

    set = &arg0->set;
    tex = NULL;
    n = set->count;
    src = set->src;
    dst = set->dst;
    row = arg0->set.rows;
    g = D_800558EC;
    sel = D_800BD394;
    if (sel == 0) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x188);
    }
    if (sel == 1) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x268);
    }
    if (sel == 2) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x1C0);
    }
    if (sel == 3) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x230);
    }
    if (sel == 4) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x1F8);
    }
    if (sel == 5) {
        tex = (Ovl6aTex *)(*(u8 **)(g + 0x18) + 0x2A0);
    }
    k = 4;
    for (;;) {
        s32 i;

        i = 0;
        if (n == 0) {
            break;
        }
        row->unk134 = 0;
        ang = n << 9;
        a = &D_80068BD4[ang & 0xFFF];
        for (;;) {
            s32 r;

            if (i >= 8) {
                break;
            }
            row->rot[i].vx = D_80068BD4[(ang - 0x400) & 0xFFF].unk2 * 64 >> 12;
            row->rot[i].vy = (a->unk2 * 64 >> 12) - 0x400;
            row->rot[i].vz = 0x2000;
            row->unk120 = (a->unk2 * 256 >> 12) + Random(0x40) - Random(0x40);
            row->unk12C = (D_80068BD4[(ang - 0x400) & 0xFFF].unk2 >> k) + Random(0x40) - Random(0x40);
            row->unkE0[i].vz = 0;
            row->unkA0[i].vz = 0;
            row->unk40[i] = 0;
            row->unk128 = 6;
            row->unk130 = Random(k) - Random(k);
            row->unk124 = 0;
            row->unk138 = 0;
            row->unk13C = 0;
            *(u32 *)&src->r0 = 0xA0A0A0;
            src->tpage = (tex->tpage & ~0x60) | 0x20;
            src->clut = tex->clut;
            src->uv0 = tex->uv0;
            src->uv1 = tex->uv1;
            src->uv2 = tex->uv2;
            src->uv3 = tex->uv3;
            ((u8 *)src)[3] = 9;
            src->code = 0x2E;
            *dst = *src;
            src++;
            dst++;
            i++;
        }
        row++;
        n--;
    }
}

void func_800BA1B4(Ovl6aTrailDst *d, Ovl6aTrail *t, s32 idx) {
    Ovl6aPt *p;
    s32 px, py;

    if (idx == 0) {
        if (t->unk138 != 0) {
            d->flags |= 0x40000000;
            d->unk76 += 0x200;
        }
        t->prev[0].x = t->pos[0].x;
        t->prev[0].y = t->pos[0].y;
        d->flags |= 0x8000;
        t->unk120 = ((t->unk120 + t->unk128) * 0xE6) >> 8;
        t->unk12C = (t->unk12C * 0xE6) >> 8;
        t->pos[0].y += *(u16 *)&t->unk120;
        t->pos[0].x += *(u16 *)&t->unk12C;
        d->x = (s16)t->pos[0].x;
        d->y = (s16)t->pos[0].y;
        d->z = t->pos[0].z;
        px = (s16)t->prev[0].x;
        py = (s16)t->prev[0].y;
        t->ang[0].z = get_angle(py - (s16)t->pos[0].y,
                                px - (s16)t->pos[0].x) & 0xFFF;
        return;
    }
    p = &t->pos[idx];
    t->prev[idx].x = p->x;
    t->prev[idx].y = p->y;
    p->x = t->prev[idx - 1].x;
    p->y = t->prev[idx - 1].y;
    t->ang[idx].z = get_angle((s16)t->prev[idx].x - (s16)p->x,
                              (s16)t->prev[idx].y - (s16)p->y) & 0xFFF;
    if (idx != 1) {
        t->flag[idx] = 1;
    }
}

void func_800BA374(Ovl6aSparks *arg0, MATRIX *m) {
    Ovl6aSparkRow *row;
    Ovl6aFt4 *prim;
    Ovl6aSparkSet *set;
    SVECTOR *p;
    s32 i;
    s32 c;
    s32 r;

    set = &arg0->set;
    row = set->rows;
    set = (Ovl6aSparkSet *)((u8 *)set + (D_8005B690 & 1) * 4);
    prim = set->src;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    arg0 = arg0->next;
    while (1) {
        if (arg0 == NULL) {
            break;
        }
        if (D_8005A624 == 0) {
            row->unk134++;
        }
        i = 0;
        if (row->unk134 == 0x14) {
            row->unk138 = 1;
        }
        while (1) {
            SVECTOR v;

            if (i >= 8) {
                break;
            }
            func_800BA1B4((Ovl6aTrailDst *)arg0, (Ovl6aTrail *)row, i);
            if (row->unk138 != 0) {
                row->unk13C = row->unk13C * 220 >> 8;
            } else {
                row->unk13C = 255 - i * 255 / 8;
            }
            c = func_8002A2EC(row->unk13C, row->unk13C, row->unk13C) | 0x2E000000;
            if (i == 0) {
                v.vx = row->rot[0].vx - 0x80;
                v.vy = row->rot[0].vy - 0x80;
                v.vz = row->rot[0].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                *(s32 *)&prim->r0 = c;
                gte_stsxy(&prim->x0);
                prim->y1 = prim->y0;
                prim->x2 = prim->x0;
            } else {
                *(s32 *)&prim->r0 = c;
                prim->x0 = prim[-1].x1;
                prim->y0 = prim[-1].y1;
                prim->x2 = prim[-1].x3;
                prim->y2 = prim[-1].y3;
            }
            if (i == 0) {
                v.vx = row->rot[0].vx + 0x80;
                v.vy = row->rot[0].vy + 0x80;
                v.vz = row->rot[0].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x3);
                prim->x1 = prim->x3;
                prim->y2 = prim->y3;
            } else {
                r = 0x80 - i * 16;
                v.vx = row->rot[i].vx + (r * D_80068BD4[row->unkE0[i].vz & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * D_80068BD4[(row->unkE0[i].vz + 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x3);
                v.vx = row->rot[i].vx + (r * D_80068BD4[(((row->unkE0[i].vz - 0x400) & 0xFFF) - 0x400) & 0xFFF].unk2 >> 12);
                v.vy = row->rot[i].vy + (r * D_80068BD4[(row->unkE0[i].vz - 0x400) & 0xFFF].unk2 >> 12);
                v.vz = row->rot[i].vz;
                p = &v;
                gte_ldv0(p);
                gte_rtps();
                gte_stsxy(&prim->x1);
            }
            if (row->unk40[i] != 0) {
                prim->tag = (prim->tag & 0xFF000000) | (D_80056998.ot[700] & 0xFFFFFF);
                p = (SVECTOR *)0xFF000000;
                D_80056998.ot[700] = (D_80056998.ot[700] & (u32)p) | ((u32)prim & 0xFFFFFF);
            }
            prim++;
            i++;
        }
        arg0 = arg0->next;
        row++;
    }
}

void func_800BA7BC(NAS *arg0) {
    if (D_8005A624 == 0) {
        if (arg0->UserInfo.user1++ >= 0x1D) {
            KillNAS(arg0);
        }
    }
}

void func_800BA804(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 < 0x21C) {
            eng->unk10.y = temp_a0 + 5;
        }
    }
}

void func_800BA848(void) {
    s32 temp_a0;
    EXTengine *eng;

    eng = GetEngineSlot(6);
    if (eng != NULL) {
        temp_a0 = eng->unk10.y;
        if (temp_a0 >= 0x155) {
            eng->unk10.y = temp_a0 - 5;
        }
    }
}

void func_800BA88C(void) {
    PodiumChild *c;

    if (D_800BD594 != NULL) {
        c = D_800BD594->child;
        if (c != NULL && D_800BD594->f5C == 0) {
            D_800BD594->f5C = 1;
            c->f5C = 1;
            c->f34 = 0;
            c->f30 = 0;
            c->f2C = 0;
            c->f5E = 1;
            c->flags |= 0x8000;
        }
    }
}

void InitPodiumTrophy(NAS *arg0) {
    EXTengine *e;
    subengine *s;

    e = arg0->Engine;
    arg0->UserInfo.status = 0;
    if (e != NULL && (e->flags &= ~0x8000, s = e->sub, s != NULL)) {
        s->unk6C = &D_8005572C;
        s->unk74 = 0x200A;
        s->unk51 = 0;
    } else {
        arg0->Failure = 1;
    }
    if (arg0->Failure != 0) {
        KillNAS(arg0);
        return;
    }
    D_800BD594 = (PodiumObj *)arg0;
}

void InitPodiumOdo(u8 *arg0) {
    if ((*(s32*)((u8*)(arg0) + 84)) != 0) {
        (*(s32*)((u8*)(arg0) + 96)) = 0;
        (*(s32*)((u8*)(arg0) + 100)) = 0;
        (*(s16*)((u8*)(arg0) + 92)) = 0;
        (*(s16*)((u8*)(arg0) + 94)) = 0;
    }
    if ((*(u16*)((u8*)(arg0) + 106)) != 0) {
        KillNAS((NAS *)arg0);
    }
}

void func_800BA9C8(s32 arg0, Ovl6aSpawnEnt *arg1) {
    NAS *nas;
    EXTengine *e;
    subengine *s;
    s32 k;
    s32 t;
    u8 *p;

    nas = SpawnNASById(arg1->x, D_8005A5D8, arg1->z, 0x1F01, 0x11);
    if (nas != NULL) {
        D_800BD584[arg0] = nas;
        nas->UserInfo.user1 = arg0 + 1;
        nas->UserInfo.flags = arg1->flags;
        k = arg1->kind;
        if (k <= 0) {
            k = 1;
        }
        if (k >= 5) {
            k = 4;
        }
        if (k >= 2) {
            e = nas->Engine;
            if (e != NULL) {
                func_80018F3C(e, 1);
                s = e->sub;
                if (s != NULL && (s = s->next, s != NULL)) {
                    s->unk6C = &D_8005572C;
                    p = (u8 *)D_800BD1F0;
                    t = (k - 1) * 4;
                    s->unk74 = *(u16 *)(p + t);
                    s->flags |= 0x8000;
                    s = e->sub;
                    s->unk08 = -*(s32 *)((u8 *)D_800BD200 + t);
                }
            }
        }
    }
}

void func_800BAAFC(void) {
    func_8009440C();
    func_80018B08(D_8009F67C, D_8009F734);
    func_8001C2C0(GetEngineSlot(6));
}

void func_800BAB40(void) {
    s32 i;
    s32 any = 0;

    for (i = 0; i < 4; i++) {
        if (((PadButtons *)D_80051338)[i].unk54 & 0x4000) {
            any = 1;
        }
    }
    if (any != 0 && D_800BD5B4 != 0 && D_800BD5B0 < 4U) {
        D_800BD5B0 = 4;
    }
    if (D_800BD5B0 == 0x64) {
        ((Ovl6aGameView *)D_8005A614)->unk24 = 0;
        ((Ovl6aGameView *)D_8005A614)->unk30 = 0x8002;
    }
    func_800B8F50();
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3894);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BABEC);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB038);

void func_800BB280(void) {
    s32 i;
    u8 *g;
    s32 *p;
    EXTengine *e;

    func_80018B08((GfxViewport *)D_800BDB14, (GfxCamera *)D_800BF1A8);
    D_800569AC = *(s32 *)D_8009F644;
    e = GetEngineSlot(0);
    i = 0;
    g = D_8005A614;
    p = (s32 *)((u8 *)e->sub + 0x540);
    do {
        D_800BD5B8[i * 2] = g[i + 0x18];
        if (D_800BD5B8[i * 2] == 0x20) {
            *p |= 0x8000;
        } else {
            *p &= ~0x8000;
        }
        p += 0x2A;
        i++;
    } while (i < 8);
    func_8001C2C0((u8 *)GetEngineSlot(0));
    func_8001C2C0((u8 *)GetEngineSlot(1));
    func_8001C2C0((u8 *)&D_800BF100);
}

void func_800BB364(void) {
    func_80095A04(0);
    func_80095A04(1);
}

void func_800BB38C(void) {
    func_800BABEC();
    if (D_80051380 & 9) {
        D_8009F65C = D_8009F46C;
    }
}

void func_800BB3D0(s8 arg0) {
    Ovl6aEntry12 *e;

    e = &D_800BD7E0[D_800BF1A0];
    if (D_800BF1A4 < 8) {
        D_8005A614[D_800BF1A4 + 0x18] = arg0;
        PlaySfx(0x192, NULL, NULL, 0x1000, 0x1E00, 0);
        if (D_800BF1A4 < 7) {
            D_800BF1A4 += 1;
        }
        if (D_800BF1A4 == 8) {
            D_800BF19C = e->unk8;
        }
    } else {
        PlaySfx(0x38D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void func_800BB4AC(SubInitList *list) {
    s32 i;
    subengine *s;
    SubInit *p;

    func_8001D334(D_800BF100.sub);
    do {
        i = 0;
        func_8001D3B0(&D_800BF100, 0x21, D_800BDB58);
    } while (0);
    s = D_800BDB58;
    D_800BF100.unk10.x = 0xC4;
    D_800BF100.unk10.y = 0xBE;
    D_800BF100.flags |= 0x10000000;
    p = list->items;
    if (list->count > 0) {
        do {
            func_80095F48((s32 *)s, p->a, p->b, p->c, 0);
            p++;
            s++;
            i++;
        } while (i < list->count);
    }
}

void func_800BB584(void) {
    D_800BF100 = EngineTemplate;
    D_800BF19C = 0;
    D_800BF1A0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/800b4eac", func_800BB5F0);

void func_800BBC64(s16 *a, void *b, s32 c, s32 d) {
    RECT *r;
    RECT *rc;
    u32 *p;
    u32 *q;
    s32 off2;
    s32 off;

    r = D_8005B698;
    if (D_8005B68C == D_8005B790) {
        r += 1;
    }
    SetDrawArea((DRAWENV *)((u8 *)b + (D_8005B690 & 1) * 12), r);
    rc = (RECT *)a;
    rc->x += r->x;
    rc->y += r->y;
    SetDrawArea((DRAWENV *)((u8 *)b + ((D_8005B690 & 1) * 12 + 0x18)), rc);
    off = (D_8005B690 & 1) * 12;
    off2 = off + 0x18;
    p = (u32 *)(off + (s32)b);
    ((Ovl6aPTag *)&p[6])->addr = ((Ovl6aPTag *)&D_80056998.ot[0x136])->addr;
    ((Ovl6aPTag *)&D_80056998.ot[0x136])->addr = (u32)((u8 *)b + off2);
    ((Ovl6aPTag *)&p[0])->addr = ((Ovl6aPTag *)&D_80056998.ot[2])->addr;
    ((Ovl6aPTag *)&D_80056998.ot[2])->addr = (u32)p;
    q = ((u32 *)D_8005B68C) - (-c);
    ((Ovl6aPTag *)&D_80056998.ot[0])->addr = ((Ovl6aPTag *)q)->addr;
    ((Ovl6aPTag *)q)->addr = (u32)(D_80056998.ot + 0x137);
}

void func_800BBE1C(void) {
}

void func_800BBE24(Ovl6aRenderObj *obj, void *arg1) {
    s16 sp10[4];
    s16 sp18[4];
    s32 c;
    s32 h;
    s32 y;
    s32 d;
    u32 *saved;

    c = obj->sub->unkC;
    if ((obj->flags & 0x8000) && c != 0) {
        saved = D_80056998.ot;
        func_8001D894(obj, arg1, obj->flags, c);
        D_80056998.ot = (u32 *)(D_800BF264 + ((D_8005B690 & 1) << 14));
        ClearOTagR(D_80056998.ot, 0x138);
        obj->flags |= 0x2000000;
        obj->unk68 = -0x170;
        func_80019A60(obj->unk74, (u8 *)c, obj->flags, obj);
        func_800BB5F0(obj, sp10);
        d = sp10[3];
        y = sp10[1];
        h = (D_800BF260 * (d - y)) / 4096;
        sp18[2] = 0x200;
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[3] = h + y;
        func_800BBC64(sp18, D_800C7264, 0x400, y);
        D_80056998.ot = saved;
    }
}

void func_800BBF44(SVECTOR *v, s16 *b) {
    s16 xy[2];

    gte_ldv0(v);
    gte_rtps();
    gte_stsxy(xy);
    if (xy[0] < b[0]) {
        b[0] = xy[0];
    }
    if (xy[0] > b[2]) {
        b[2] = xy[0];
    }
    if (xy[1] < b[1]) {
        b[1] = xy[1];
    }
    if (b[3] < xy[1]) {
        b[3] = xy[1];
    }
}

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/800b4eac", D_800B3928);
