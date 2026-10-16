#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/8009608c.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_8009608C(s32 arg0) {
    CamView view[2];
    s32 *obj;
    EngCam *cam;

    switch (D_800AD12C) {
    case 0:
    case 5:
        break;
    case 1:
        if (arg0 != 0) {
            func_80097158(D_80051640);
        }
        func_80096F98(arg0);
        break;
    case 2:
        if (arg0 != 0) {
            func_80097158(D_80051640);
        }
        func_80096F98(arg0);
        ((EngCam *)D_80051640)->rot.vx = -0x130;
        ((EngCam *)D_80051640)->rot.vy = 0;
        ((EngCam *)D_80051640)->rot.vz = 0x800;
        break;
    case 3:
        func_800B5A40(D_80051640, arg0);
        break;
    case 6:
        func_800B8098(D_80051640, arg0);
        break;
    case 4:
        func_800B96F4(view, arg0);
        func_80013F44((CamView *)D_80051640, view, &D_800AD0F4);
        break;
    case 7:
        obj = func_8001E48C(1);
        if (obj != NULL) {
            cam = (EngCam *)D_80051640;
            cam->eye.x = 0;
            cam->eye.y = 0;
            cam->eye.z = func_800153B4(obj + 0x13, obj + 0xC, &cam->rot.vx);
            cam->at = *(CamXYZ *)(obj + 0xC);
            cam->dist = 0x200;
        }
        D_8009F708 = 0x15E;
        break;
    case 8:
        func_800BB184(D_80051640);
        D_8009F708 = 0x15E;
        break;
    }
    if (D_8005A624 == 0 && ((EngCam *)D_80051640)->shakeTime > 0) {
        if (D_8004E0D8 & 1) {
            ((EngCam *)D_80051640)->at.y = ((EngCam *)D_80051640)->at.y + ((EngCam *)D_80051640)->shakeAmp;
        } else {
            ((EngCam *)D_80051640)->at.y = ((EngCam *)D_80051640)->at.y - ((EngCam *)D_80051640)->shakeAmp;
        }
        ((EngCam *)D_80051640)->shakeTime--;
    }
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_80096280);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_80096530);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_800967E0);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_80096A08);

void func_80096F98(s32 arg0) {
    s32 pos[4];
    s32 d[3];
    View *view;
    EngineGameView *g;
    s32 z;
    s32 w;
    s32 v;

    func_80078DD4();
    func_80096A08();
    if (func_800967E0(&D_800AD2EC, D_800AD2F8, &D_800AD304, D_800AD310, pos) != 0) {
        d[0] = ((View *)D_80051640)->pos.vx - pos[0];
    }
    d[1] = ((View *)D_80051640)->pos.vy - pos[1];
    g = (EngineGameView *)D_8005A614;
    d[2] = ((View *)D_80051640)->pos.vz - pos[2];
    if (!(g->unk30 & 0x801) && g->mode != 0x1F) {
        if (d[0] < -0x514) {
            d[0] = -0x514;
        }
        if (d[0] > 0x514) {
            d[0] = 0x514;
        }
    }
    if (arg0 != 0) {
        ((s32 *)D_80051640)[0] = d[0];
        ((s32 *)D_80051640)[1] = d[1];
        ((s32 *)D_80051640)[2] = d[2];
        return;
    }
    view = (View *)D_80051640;
    w = d[2];
    z = view->unk0[2];
    if (w < z) {
        if (D_8005A648 != 0x1F) {
            view->unk0[2] = z + ((w - z) >> 4);
            view->unk0[0] += (d[0] - view->unk0[0]) >> 4;
            view->unk0[1] += (d[1] - view->unk0[1]) >> 4;
        } else {
            view->unk0[2] = z + ((w - z) >> 2);
            view->unk0[0] += (d[0] - view->unk0[0]) >> 2;
            view->unk0[1] += (d[1] - view->unk0[1]) >> 2;
        }
    } else {
        view->unk0[2] = z + ((w - z) >> 2);
        view->unk0[0] += (d[0] - view->unk0[0]) >> 2;
        view->unk0[1] += (d[1] - view->unk0[1]) >> 2;
    }
}

void func_80097158(u8 *p) {
    s32 *t = func_8001E48C(1);
    EngineGameView *g;

    if (t != NULL) {
        *(s16 *)(p + 0x54) = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;
        *(s32 *)(p + 0x18) = 0x200;
        g = (EngineGameView *)D_8005A614;
        *(s16 *)(p + 0x56) = 0;
        *(s16 *)(p + 0x58) = 0;
        if (g->mode == 0x19) {
            *(s32 *)(p + 0x18) = 0x258;
        }
        if (g->mode == 0x18) {
            *(s32 *)(p + 0x18) = 0x22B;
        }
        if (g->mode == 0x17) {
            *(s32 *)(p + 0x18) = 0x22B;
        }
        if (g->mode == 0x1A) {
            *(s32 *)(p + 0x18) = 0x22B;
        }
        if (g->mode == 0x1C) {
            *(s32 *)(p + 0x18) = 0x258;
        }
    }
    D_8009F708 = 0x15E;
}

void func_80097244(u8 *p) {
    s32 *t = func_8001E48C(9);

    if (t != NULL) {
        *(s16 *)(p + 0x54) = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;
        *(s16 *)(p + 0x56) = 0;
        *(s16 *)(p + 0x58) = 0;
        *(s32 *)(p + 0x18) = 0x200;
    }
    D_8009F708 = 0x15E;
}

void func_800972BC(s32 arg0, s32 arg1) {
    s32 temp = arg0 - ((arg1 + 0x3000) * 0xF0) / D_80051658;

    if (D_800AD320 < temp) {
        D_800AD320 = temp;
        D_800AD2F8[1] = arg0;
        D_800AD2F8[2] = arg1;
    }
}

void func_80097334(s32 arg0, s32 arg1) {
    s32 v = arg0 + ((arg1 + 0x3000) * 0xF0) / D_80051658;

    if (v < D_800AD31C) {
        D_800AD31C = v;
        D_800AD2EC.unk4 = arg0;
        D_800AD2EC.unk8 = arg1;
    }
}

void func_800973AC(s32 arg0, s32 arg1) {
    s32 temp = arg0 - ((arg1 + 0x3000) * 0x140) / D_80051658;

    if (D_800AD328 < temp) {
        D_800AD328 = temp;
        D_800AD310[0] = arg0;
        D_800AD310[2] = arg1;
    }
}

void func_80097424(s32 arg0, s32 arg1) {
    s32 v = arg0 + ((arg1 + 0x3000) * 0x140) / D_80051658;

    if (v < D_800AD324) {
        D_800AD324 = v;
        D_800AD304.unk0 = arg0;
        D_800AD304.unk8 = arg1;
    }
}

void func_8009749C(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    D_800AD1EC_entry *e;

    if (D_800AD1E8 < 0x10) {
        e = &D_800AD1EC[D_800AD1E8];
        e->unk0 = arg0;
        e->unk4 = arg1;
        e->unk6 = arg2;
        e->unk8 = arg3;
        e->unkA = arg4;
        e->unkC = arg5;
        D_800AD1E8 += 1;
    }
}

s32 func_800974EC(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2) {
    s32 sp18[3];
    s32 sp28[3];
    s32 d;

    d = 0x3C0000 / D_80051658;
    sp18[0] = arg0->a + d;
    sp18[2] = arg0->c - 0x3000;
    sp28[0] = arg1->a - d;
    sp28[2] = arg1->c - 0x3000;
    if (func_80096280(arg0, sp18, arg1, sp28, arg2) != 0) {
        return 1;
    }
    if (arg0->c < arg1->c) {
        *arg2 = *arg0;
    } else {
        *arg2 = *arg1;
    }
    arg2->c -= 0x600;
    return 1;
}

s32 func_80097608(s32 *arg0, s32 *arg1, s32 arg2) {
    s32 sp18[3];
    s32 sp28[3];
    s32 d = 0x2D0000 / D_80051658;
    extern s32 func_80096530(s32 *, s32 *, s32 *, s32 *, s32);

    sp18[1] = arg0[1] + d;
    sp18[2] = arg0[2] - 0x3000;
    sp28[1] = arg1[1] - d;
    sp28[2] = arg1[2] - 0x3000;
    return func_80096530(arg0, sp18, arg1, sp28, arg2) != 0;
}

s32 func_800976A8(u8 *arg0, s32 arg1) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    u8 *var_a0;

    var_a0 = arg0;
    var_t0 = 0x80000000;
    var_a3 = 0x7FFFFFFF;
    var_a2 = 0;
    if (arg1 > 0) {
        do {
            temp_v1 = (*(s32*)((u8*)(var_a0) + 8));
            if (var_t0 < temp_v1) {
                var_t0 = temp_v1;
            }
            if (temp_v1 < var_a3) {
                var_a3 = temp_v1;
            }
            var_a2 += 1;
            var_a0 += 0xC;
        } while (var_a2 < arg1);
    }
    return var_t0 - var_a3;
}

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_800976F8);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_80097AA4);

INCLUDE_ASM("asm/nonmatchings/overlays/engine/8009608c", func_80097C28);

void func_80098278(void) {
    D_800ADBC0 = 0;
}

void func_800976F8(s32 *);
void func_80098284(void) {
    s32 *p;
    if (D_800ADBC0 == 0) {
        do {
            D_800ADBC0 = 1;
            p = &D_800ADBC0;
            p[1] = 0;
            p[2] = 0;
            func_800976F8(p);
        } while (0);
        PlaySfx(0x19D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

extern s32 D_800ADBD4;
void func_800982E0(void) {
    s32 i;
    subengine *s = GetEngineSlot(3)->sub + 2;
    for (i = 0; i < D_800ADBD4; i++) {
        s->flags &= ~0x8000;
        s = s->next;
    }
}

void func_80098344(void) {
    s32 i;
    subengine *s = GetEngineSlot(3)->sub + 2;
    for (i = 0; i < D_800ADBD4; i++) {
        s->unk20.z = 0x199;
        s->unk51 = 0;
        s->unk50 = 1;
        s->flags |= 0x8000;
        s->unk12 = i * (0x1000 / (D_800ADBD4 + 1)) + D_8005B690 * 40;
        s->unk52 = 1;
        s = s->next;
    }
}

void func_80098428(void) {
    subengine *s = GetEngineSlot(3)->sub;
    s->flags |= 0x8000;
    func_80097AA4(s);
}

void func_80098464(void) {
    subengine *s = GetEngineSlot(3)->sub;
    u32 t = D_8005A646 & 0xF0;
    if (t == 0x40 || t == 0x80) {
        s->flags |= 0x8000;
        func_80097AA4(s);
    } else {
        s->flags &= ~0x8000;
    }
}

void func_800984D4(void) {
    s32 *s;
    s32 v;
    int new_var;

    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = *(s32 *)D_8009F644;
    if (D_800ADBC4 != -1) {
        s = (s32 *)GetEngineSlot(3)->sub;
        s[new_var = 0xA8 / 4] &= 0xFFFF7FFF;
        if ((D_8005A624 & 0x240) != 0x240) {
            s[0xA8 / 4] |= 0x8000;
        }
        func_8001C2C0((u8 *)GetEngineSlot(3));
    }
}

void func_80098570(void) {
    s32 *p;
    D_800ADBC0 = 0;
    p = &D_800ADBC0;
    p[1] = -1;
    p[3] = 0;
    p[4] = 1;
}
