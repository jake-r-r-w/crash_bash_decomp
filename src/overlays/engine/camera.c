#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/camera.h"
#include "bash/hero.h"
#include "bash/shell.h"

void func_8009608C(s32 arg0) {
    CamView view[2];
    s32 *obj;
    EngCam *cam;

    switch (CameraMode) {
    case 0:
    case 5:
        break;
    case 1:
        if (arg0 != 0) {
            func_80097158(MainCamera);
        }
        func_80096F98(arg0);
        break;
    case 2:
        if (arg0 != 0) {
            func_80097158(MainCamera);
        }
        func_80096F98(arg0);
        ((EngCam *)MainCamera)->rot.vx = -0x130;
        ((EngCam *)MainCamera)->rot.vy = 0;
        ((EngCam *)MainCamera)->rot.vz = 0x800;
        break;
    case 3:
        TitlesCameraHook(MainCamera, arg0);
        break;
    case 6:
        HubCameraFollow(MainCamera, arg0);
        break;
    case 4:
        PodiumCamera(view, arg0);
        func_80013F44((CamView *)MainCamera, view, &D_800AD0F4);
        break;
    case 7:
        obj = path_get(1);
        if (obj != NULL) {
            cam = (EngCam *)MainCamera;
            cam->eye.x = 0;
            cam->eye.y = 0;
            cam->eye.z = func_800153B4(obj + 0x13, obj + 0xC, &cam->rot.vx);
            cam->at = *(CamXYZ *)(obj + 0xC);
            cam->dist = 0x200;
        }
        D_8009F708 = 0x15E;
        break;
    case 8:
        N2OCameraFollow(MainCamera);
        D_8009F708 = 0x15E;
        break;
    }
    if (GamePause == 0 && ((EngCam *)MainCamera)->shakeTime > 0) {
        if (GameFrameCount & 1) {
            ((EngCam *)MainCamera)->at.y = ((EngCam *)MainCamera)->at.y + ((EngCam *)MainCamera)->shakeAmp;
        } else {
            ((EngCam *)MainCamera)->at.y = ((EngCam *)MainCamera)->at.y - ((EngCam *)MainCamera)->shakeAmp;
        }
        ((EngCam *)MainCamera)->shakeTime--;
    }
}

s32 func_80096280(s32 *p0, s32 *p1, s32 *q0, s32 *q1, s32 *out) {
    s32 denom, num, u, t;
    s32 dqz;
    s32 dpx;
    s32 dpz, dqx, ez, ex;
    s32 a, b, c;

    a = p0[2];
    if (a < (b = q0[2]) && (c = p1[2]) < b && a < q1[2] && c < q1[2]) {
        return 0;
    }
    if ((b = q0[2]) < a && b < (c = p1[2]) && q1[2] < a && q1[2] < c) {
        return 0;
    }
    do {
        do {
            a = p0[0];
        } while (0);
    } while (0);
    if (a < (b = q0[0]) && (c = p1[0]) < b && a < q1[0] && c < q1[0]) {
        return 0;
    }
    if ((b = q0[0]) < a && b < (c = p1[0]) && q1[0] < a && q1[0] < c) {
        return 0;
    }
    dqz = (q0[2] - q1[2]) >> 4;
    dpx = (p0[0] - q0[0]) >> 4;
    dpz = (p0[2] - q0[2]) >> 4;
    dqx = (q0[0] - q1[0]) >> 4;
    ez = (p1[2] - p0[2]) >> 4;
    ex = (p1[0] - p0[0]) >> 4;
    num = dqz * dpx - dqx * dpz;
    denom = ez * dqx - ex * dqz;
    if (denom > 0) {
        if (denom < num) {
            return 0;
        }
        if (num < 0) {
            return 0;
        }
    } else {
        if (num < denom) {
            return 0;
        }
        if (num > 0) {
            return 0;
        }
    }
    u = ex * dpz - ez * dpx;
    if (denom > 0) {
        if (denom < u) {
            return 0;
        }
        if (u < 0) {
            return 0;
        }
    } else {
        if (u < denom) {
            return 0;
        }
        if (u > 0) {
            return 0;
        }
    }
    if (denom == 0) {
        return 0;
    }
    t = (num << 8) / denom;
    out[0] = p0[0] + (((p1[0] - p0[0]) * t) >> 8);
    out[2] = p0[2] + (((p1[2] - p0[2]) * t) >> 8);
    out[1] = 0;
    return 1;
}

s32 line_intersect_zy(VECTOR *p0, VECTOR *p1, VECTOR *q0, VECTOR *q1, VECTOR *out) {
    s32 denom, num, u, t;
    s32 dqz;
    s32 dpy;
    s32 dpz, dqy, ez, ey;
    s32 a, b, c;

    a = p0->vz;
    if (a < (b = q0->vz) && (c = p1->vz) < b && a < q1->vz && c < q1->vz) {
        return 0;
    }
    if ((b = q0->vz) < a && b < (c = p1->vz) && q1->vz < a && q1->vz < c) {
        return 0;
    }
    do {
        do {
            a = p0->vy;
        } while (0);
    } while (0);
    if (a < (b = q0->vy) && (c = p1->vy) < b && a < q1->vy && c < q1->vy) {
        return 0;
    }
    if ((b = q0->vy) < a && b < (c = p1->vy) && q1->vy < a && q1->vy < c) {
        return 0;
    }
    dqz = (q0->vz - q1->vz) >> 4;
    dpy = (p0->vy - q0->vy) >> 4;
    dpz = (p0->vz - q0->vz) >> 4;
    dqy = (q0->vy - q1->vy) >> 4;
    ez = (p1->vz - p0->vz) >> 4;
    ey = (p1->vy - p0->vy) >> 4;
    num = dqz * dpy - dqy * dpz;
    denom = ez * dqy - ey * dqz;
    if (denom > 0) {
        if (denom < num) {
            return 0;
        }
        if (num < 0) {
            return 0;
        }
    } else {
        if (num < denom) {
            return 0;
        }
        if (num > 0) {
            return 0;
        }
    }
    u = ey * dpz - ez * dpy;
    if (denom > 0) {
        if (denom < u) {
            return 0;
        }
        if (u < 0) {
            return 0;
        }
    } else {
        if (u < denom) {
            return 0;
        }
        if (u > 0) {
            return 0;
        }
    }
    if (denom == 0) {
        return 0;
    }
    t = (num << 8) / denom;
    out->vx = 0;
    out->vy = p0->vy + (((p1->vy - p0->vy) * t) >> 8);
    out->vz = p0->vz + (((p1->vz - p0->vz) * t) >> 8);
    return 1;
}

s32 func_800967E0(D_800AD2EC_t *a, s32 *b, D_800AD2EC_t *c, s32 *d, s32 *out) {
    Vec3W hit;
    VECTOR sp28;
    s32 flags;
    s32 k;

    flags = 0;
    if (EngCam_HitY((VECTOR *)a, (VECTOR *)b, &sp28)) {
        out[1] = sp28.vy;
        flags = 1;
        out[2] = sp28.vz;
    }
    if (EngCam_HitX((Vec3W *)c, (Vec3W *)d, &hit)) {
        out[0] = hit.a;
        if (flags != 0) {
            k = sp28.vz;
            if (hit.c < k) {
                k = hit.c;
            }
            out[2] = k;
        } else {
            out[2] = sp28.vz;
        }
        flags |= 2;
    }
    return flags;
}

void func_80096A08(void) {
    MATRIX m;
    VECTOR in;
    VECTOR out;
    D_800AD1EC_entry *e;
    s32 n;
    EngCam *cam;
    s32 z0;

    cam = (EngCam *)MainCamera;
    RotMatrix(&cam->rot, &m);
    n = camera_frame_list_size;
    D_800AD31C = 0x7FFFFFFF;
    D_800AD320 = 0x80000000;
    D_800AD324 = 0x7FFFFFFF;
    D_800AD328 = 0x80000000;
    e = D_800AD1EC;
    for (;;) {
        s32 z;
        s32 x;
        s32 xr;
        s32 w;
        s32 v;

        if (--n == -1) {
            break;
        }
        in.vx = ((s32 *)e->unk0)[0] - cam->at.x;
        in.vy = ((s32 *)e->unk0)[1] - cam->at.y;
        in.vz = ((s32 *)e->unk0)[2] - cam->at.z;
        ApplyMatrixLV(&m, &in, &out);
        out.vx += cam->at.x;
        out.vy += cam->at.y;
        out.vz += cam->at.z;

        z0 = out.vz - e->unk8;
        x = out.vx - e->unk4;
        v = x + (z0 + 0x3000) * 320 / ((EngCam *)MainCamera)->dist;
        if (v < D_800AD324) {
            D_800AD324 = v;
            D_800AD304.unk0 = x;
            D_800AD304.unk8 = z0;
        }
        z = out.vz - e->unk8;
        xr =out.vx + e->unk6;
        v = xr - (z + 0x3000) * 320 / ((EngCam *)MainCamera)->dist;
        if (v > D_800AD328) {
            D_800AD328 = v;
            camera_frame_right[0] = xr;
            camera_frame_right[2] = z;
        }
        z = out.vz - e->unk8;
        xr = out.vy - e->unkC;
        v = xr + (z + 0x3000) * 240 / ((EngCam *)MainCamera)->dist;
        if (v < D_800AD31C) {
            D_800AD31C = v;
            D_800AD2EC.unk4 = xr;
            D_800AD2EC.unk8 = z;
        }
        z = out.vz - e->unk8;
        w = out.vy + e->unkC;
        v = w - (z + 0x3000) * 240 / ((EngCam *)MainCamera)->dist;
        if (v > D_800AD320) {
            D_800AD320 = v;
            ((D_800AD2EC_t *)D_800AD2F8)->unk4 = w;
            ((D_800AD2EC_t *)D_800AD2F8)->unk8 = z;
        }

        z = out.vz + e->unkA;
        xr = out.vx - e->unk4;
        v = xr + (z + 0x3000) * 320 / ((EngCam *)MainCamera)->dist;
        if (v < D_800AD324) {
            D_800AD324 = v;
            D_800AD304.unk0 = xr;
            D_800AD304.unk8 = z;
        }
        z = out.vz + e->unkA;
        w = out.vx + e->unk6;
        v = w - (z + 0x3000) * 320 / ((EngCam *)MainCamera)->dist;
        if (v > D_800AD328) {
            D_800AD328 = v;
            camera_frame_right[0] = w;
            camera_frame_right[2] = z;
        }
        z = out.vz + e->unkA;
        w = out.vy - e->unkC;
        v = w + (z + 0x3000) * 240 / ((EngCam *)MainCamera)->dist;
        if (v < D_800AD31C) {
            D_800AD31C = v;
            D_800AD2EC.unk4 = w;
            D_800AD2EC.unk8 = z;
        }
        w = out.vy + e->unkC;
        z = out.vz + e->unkA;
        v = w - (z + 0x3000) * 240 / ((EngCam *)MainCamera)->dist;
        if (v > D_800AD320) {
            D_800AD320 = v;
            ((D_800AD2EC_t *)D_800AD2F8)->unk4 = w;
            ((D_800AD2EC_t *)D_800AD2F8)->unk8 = z;
        }
        e++;
    }
}

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
    if (func_800967E0(&D_800AD2EC, D_800AD2F8, &D_800AD304, camera_frame_right, pos) != 0) {
        d[0] = ((View *)MainCamera)->pos.vx - pos[0];
    }
    d[1] = ((View *)MainCamera)->pos.vy - pos[1];
    g = (EngineGameView *)GameInfo;
    d[2] = ((View *)MainCamera)->pos.vz - pos[2];
    if (!(g->unk30 & 0x801) && g->mode != 0x1F) {
        if (d[0] < -0x514) {
            d[0] = -0x514;
        }
        if (d[0] > 0x514) {
            d[0] = 0x514;
        }
    }
    if (arg0 != 0) {
        ((s32 *)MainCamera)[0] = d[0];
        ((s32 *)MainCamera)[1] = d[1];
        ((s32 *)MainCamera)[2] = d[2];
        return;
    }
    view = (View *)MainCamera;
    w = d[2];
    z = view->unk0[2];
    if (w < z) {
        if (GameMode != 0x1F) {
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
    s32 *t = path_get(1);
    EngineGameView *g;

    if (t != NULL) {
        *(s16 *)(p + 0x54) = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;
        *(s32 *)(p + 0x18) = 0x200;
        g = (EngineGameView *)GameInfo;
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
    s32 *t = path_get(9);

    if (t != NULL) {
        *(s16 *)(p + 0x54) = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;
        *(s16 *)(p + 0x56) = 0;
        *(s16 *)(p + 0x58) = 0;
        *(s32 *)(p + 0x18) = 0x200;
    }
    D_8009F708 = 0x15E;
}

void camera_count_bottom(s32 arg0, s32 arg1) {
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
        camera_frame_right[0] = arg0;
        camera_frame_right[2] = arg1;
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

void camera_add_frame_list(SLONG_XYZ *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    D_800AD1EC_entry *e;

    if (camera_frame_list_size < 0x10) {
        e = &D_800AD1EC[camera_frame_list_size];
        e->unk0 = arg0;
        e->unk4 = arg1;
        e->unk6 = arg2;
        e->unk8 = arg3;
        e->unkA = arg4;
        e->unkC = arg5;
        camera_frame_list_size += 1;
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

s32 func_80097608(VECTOR *arg0, VECTOR *arg1, s32 arg2) {
    VECTOR sp18;
    VECTOR sp28;
    s32 d = 0x2D0000 / D_80051658;
    extern s32 line_intersect_zy(VECTOR *, VECTOR *, VECTOR *, VECTOR *, VECTOR *);

    sp18.vy = arg0->vy + d;
    sp18.vz = arg0->vz - 0x3000;
    sp28.vy = arg1->vy - d;
    sp28.vz = arg1->vz - 0x3000;
    return line_intersect_zy(arg0, &sp18, arg1, &sp28, (VECTOR *)arg2) != 0;
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

extern s32 D_800ADBD4;
void func_800976F8(void) {
    EXTengine *eng;
    s32 v;

    eng = GetEngineSlot(3);
    D_800AD32C[6].unk1E = D_8009922C[GV->unk50[0]];
    D_800AD32C[7].unk1E = D_8009922C[GV->unk50[1]];
    D_800AD32C[8].unk1E = D_8009922C[GV->unk50[2]];
    D_800AD32C[9].unk1E = D_8009922C[GV->unk50[3]];
    D_800AD32C[10].unk1E = GV->unkCC[0] + 0x200E;
    D_800AD32C[11].unk1E = GV->unkCC[1] + 0x200E;
    D_800AD32C[12].unk1E = GV->unkCC[2] + 0x200E;
    D_800AD32C[13].unk1E = GV->unkCC[3] + 0x200E;
    D_800AD5D8[4].unk1E = D_8009922C[GV->unk50[0]];
    D_800AD5D8[5].unk1E = D_8009922C[GV->unk50[1]];
    D_800AD5D8[6].unk1E = D_8009922C[GV->unk50[2]];
    D_800AD5D8[7].unk1E = D_8009922C[GV->unk50[3]];
    D_800AD5D8[8].unk1E = GV->unkCC[GV->unk48] + 0x200E;
    D_800AD5D8[9].unk1E = GV->unkCC[GV->unk4A] + 0x200E;
    D_800ADBD4 = 0;
    D_800AD7AC[3].unk1E = D_8009922C[GV->unk50[0]];
    D_800AD7AC[4].unk1E = D_8009922C[GV->unk50[1]];
    D_800AD7AC[5].unk1E = GV->unkCC[0] + 0x200E;
    D_800AD9C8[4].unk1E = D_8009922C[GV->unk50[0]];
    D_800AD9C8[5].unk1E = D_8009922C[GV->unk50[1]];
    D_800AD9C8[6].unk1E = GV->unkCC[0] + 0x200E;
    D_800AD9C8[7].unk1E = GV->unkCC[1] + 0x200E;
    D_800AD8CC[3].unk1E = D_8009922C[GV->unk50[0]];
    D_800AD8CC[4].unk1E = GV->unkCC[0] + 0x200E;
    switch (GV->flags & 0xF0) {
    case 0x20:
        v = 0x2021;
        if (GV->flags & 8) {
            v = 0x2020;
        }
        D_800AD32C[0].unk1E = v;
        D_800AD5D8[0].unk1E = v;
        D_800AD7AC[0].unk1E = v;
        D_800AD8CC[0].unk1E = v;
        if (GV->unk6C != 0) {
            func_80095BEC(3, (s32 *)D_800AD7AC);
            D_800ADBD4 = 1;
        } else {
            func_80095BEC(3, (s32 *)D_800AD8CC);
            D_800ADBD4 = 1;
        }
        break;
    case 0x40:
        D_800ADB54[0].unk1E = 0x201F;
        func_80095BEC(3, (s32 *)D_800ADB54);
        goto end;
    case 0x80:
        D_800ADB54[0].unk1E = 0x201E;
        func_80095BEC(3, (s32 *)D_800ADB54);
        goto end;
    case 0x10:
    default:
        D_800AD32C[0].unk1E = 0x2022;
        D_800AD5D8[0].unk1E = 0x2022;
        D_800AD7AC[0].unk1E = 0x2022;
        D_800AD8CC[0].unk1E = 0x2022;
        if (GV->mode == 0x20) {
            func_80095BEC(3, (s32 *)D_800AD9C8);
            D_800ADBD4 = 2;
        } else if (GV->unk6C != 0) {
            func_80095BEC(3, (s32 *)D_800AD5D8);
            D_800ADBD4 = 2;
        } else {
            func_80095BEC(3, (s32 *)D_800AD32C);
            D_800ADBD4 = 4;
        }
        break;
    }
    eng->sub->flags &= 0xFFFF7FFF;
end:
    GetEngineSlot(3)->unk10.x = 0x140;
    GetEngineSlot(3)->unk10.y = 0x118;
    GetEngineSlot(3)->flags &= 0xEFFFFFFF;
}

void func_80097AA4(subengine *s) {
    TrigEntry *t;
    s32 f;
    TrigEntry *e;

    s->unk51 = 0;
    t = TrigTable;
    f = FrameCount;
    e = &t[(((f * 70) & 0xFFF) - 0x400) & 0xFFF];
    s->unk10 = *(s32 *)e >> 20;
    s->unk12 = (t[(((f * 140) & 0xFFF) - 0x400) & 0xFFF].unk2 + 0x800) >> 5;
    s->unk14 = (e->unk2 + t[(((f * 100) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 5;
    s->unk04 = *(s32 *)&t[(((f * 35) & 0xFFF) - 0x400) & 0xFFF] >> 23;
    s->unk04 = (t[((f * 75) - 0x400) & 0xFFF].unk2 * 480) / 4096;
    s->unk08 = (t[(((f * 45) & 0xFFF) - 0x400) & 0xFFF].unk2 + t[(((f * 145) & 0xFFF) - 0x400) & 0xFFF].unk2) >> 6;
    s->unk20.z = 0x199;
    s->unk50 = 1;
    s->unk52 = 1;
}

void func_80097C28(void) {
    s32 *p = &D_800ADBC0;
    EXTengine *eng;
    subengine *s;
    s32 *q;
    s32 i;
    s32 v;

    if (p[1] == -1) {
        return;
    }
    eng = GetEngineSlot(3);
    if (eng->sub == NULL) {
        return;
    }
    eng->flags = 0x10008000;
    eng->unk10.x = 0x140;
    eng->unk5D = 0;
    eng->unk10.y = 0x118;
    eng->unk10.z = 0;
    switch (p[1]) {
    case 0:
        eng->unk20 = func_800152A8(-0x800, 0, 0x14, p[2]);
        eng->unk1C = func_800152A8(-0x400, 0, 0x14, p[2]);
        v = interp(0x800, 0x1800, 0x14, p[2]);
        eng->unk2C.z = v;
        eng->unk2C.y = v;
        eng->unk2C.x = v;
        eng->unk5E = 1;
        s = GetEngineSlot(3)->sub + 2;
        for (i = 0; i < D_800ADBD4; i++) {
            s->unk20.z = 0x199;
            s->unk51 = 0;
            s->unk50 = 1;
            s->flags |= 0x8000;
            s->unk12 = i * (0x1000 / (D_800ADBD4 + 1)) + FrameCount * 40;
            s->unk52 = 1;
            s = s->next;
        }
        {
            subengine *e = GetEngineSlot(3)->sub;
            u32 t = GameLevelFlags & 0xF0;
            if (t == 0x40 || t == 0x80) {
                e->flags |= 0x8000;
                func_80097AA4(e);
            } else {
                e->flags &= ~0x8000;
            }
        }
        q = &D_800ADBC0;
        if (++q[2] > 0x14) {
            q[2] = 0;
            q[1] = 1;
        }
        break;
    case 1:
        s = GetEngineSlot(3)->sub + 2;
        for (i = 0; i < D_800ADBD4; i++) {
            s->unk20.z = 0x199;
            s->unk51 = 0;
            s->unk50 = 1;
            s->flags |= 0x8000;
            s->unk12 = i * (0x1000 / (D_800ADBD4 + 1)) + FrameCount * 40;
            s->unk52 = 1;
            s = s->next;
        }
        {
            subengine *e = GetEngineSlot(3)->sub;
            u32 t = GameLevelFlags & 0xF0;
            if (t == 0x40 || t == 0x80) {
                e->flags |= 0x8000;
                func_80097AA4(e);
            } else {
                e->flags &= ~0x8000;
            }
        }
        if (!(GameLevelFlags & 0xC0)) {
            q = &D_800ADBC0;
            if (q[3] != 0) {
                q[1] = 2;
                q[2] = 0;
                PlaySfx(0x19F, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 2:
        if (p[2] < 10) {
            s = GetEngineSlot(3)->sub + 2;
            for (i = 0; i < D_800ADBD4; i++) {
                s->unk20.z = 0x199;
                s->unk51 = 0;
                s->unk50 = 1;
                s->flags |= 0x8000;
                s->unk12 = i * (0x1000 / (D_800ADBD4 + 1)) + FrameCount * 40;
                s->unk52 = 1;
                s = s->next;
            }
            {
                subengine *e = GetEngineSlot(3)->sub;
                u32 t = GameLevelFlags & 0xF0;
                if (t == 0x40 || t == 0x80) {
                    e->flags |= 0x8000;
                    func_80097AA4(e);
                } else {
                    e->flags &= ~0x8000;
                }
            }
        } else {
            {
                subengine *e = GetEngineSlot(3)->sub;
                e->flags |= 0x8000;
                func_80097AA4(e);
            }
            {
                s32 j;
                subengine *e = GetEngineSlot(3)->sub + 2;
                for (j = 0; j < D_800ADBD4; j++) {
                    e->flags &= ~0x8000;
                    e = e->next;
                }
            }
        }
        {
            s32 *r = &D_800ADBC0;
            eng->unk1E = func_800152A8(0, 0x800, 0x14, ++r[2]);
            eng->unk5E = 1;
            if (r[2] >= 0x14) {
                r[1] = 3;
            }
        }
        break;
    case 3:
        {
            subengine *e = GetEngineSlot(3)->sub;
            e->flags |= 0x8000;
            func_80097AA4(e);
        }
        {
            s32 j;
            subengine *e = GetEngineSlot(3)->sub + 2;
            for (j = 0; j < D_800ADBD4; j++) {
                e->flags &= ~0x8000;
                e = e->next;
            }
        }
        break;
    }
}

void func_80098278(void) {
    D_800ADBC0 = 0;
}

void func_80098284(void) {
    s32 *p;
    if (D_800ADBC0 == 0) {
        D_800ADBC0 = 1;
        p = &D_800ADBC0;
        p[1] = 0;
        p[2] = 0;
        func_800976F8();
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
        s->unk12 = i * (0x1000 / (D_800ADBD4 + 1)) + FrameCount * 40;
        s->unk52 = 1;
        s = s->next;
    }
}

void anim_item(void) {
    subengine *s = GetEngineSlot(3)->sub;
    s->flags |= 0x8000;
    func_80097AA4(s);
}

void func_80098464(void) {
    subengine *s = GetEngineSlot(3)->sub;
    u32 t = GameLevelFlags & 0xF0;
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
    FadeAmount = ScreenFade.amount;
    if (D_800ADBC4 != -1) {
        s = (s32 *)GetEngineSlot(3)->sub;
        s[new_var = 0xA8 / 4] &= 0xFFFF7FFF;
        if ((GamePause & 0x240) != 0x240) {
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
