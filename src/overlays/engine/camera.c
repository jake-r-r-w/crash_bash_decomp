#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/camera.h"
#include "bash/hero.h"
#include "bash/shell.h"

void UpdateCamera(s32 arg0) {
    CamView view[2];
    s32 *obj;
    camera_info_tag *cam;

    switch (CameraMode) {
    case 0:
    case 5:
        break;
    case 1:
        if (arg0 != 0) {
            CamFromPath1(&MainCamera);
        }
        CameraFrameHeroes(arg0);
        break;
    case 2:
        if (arg0 != 0) {
            CamFromPath1(&MainCamera);
        }
        CameraFrameHeroes(arg0);
        MainCamera.xform.angle.vx = -0x130;
        MainCamera.xform.angle.vy = 0;
        MainCamera.xform.angle.vz = 0x800;
        break;
    case 3:
        TitlesCameraHook(&MainCamera, arg0);
        break;
    case 6:
        HubCameraFollow(&MainCamera, arg0);
        break;
    case 4:
        PodiumCamera(view, arg0);
        cam_ease_to((CamView *)&MainCamera, view, &CameraEase);
        break;
    case 7:
        obj = path_get(1);
        if (obj != NULL) {
            cam = &MainCamera;
            cam->eye.x = 0;
            cam->eye.y = 0;
            cam->eye.z = get_dist_angles(obj + 0x13, obj + 0xC, &cam->xform.angle.vx);
            cam->at = *(CamXYZ *)(obj + 0xC);  /* a path record (map file): its position */
            cam->dist = 0x200;
        }
        GameViewport.ofy = 0x15E;
        break;
    case 8:
        N2OCameraFollow(&MainCamera);
        GameViewport.ofy = 0x15E;
        break;
    }
    if (GamePause == 0 && MainCamera.shakeTime > 0) {
        if (GameFrameCount & 1) {
            MainCamera.at.y = MainCamera.at.y + MainCamera.shakeAmp;
        } else {
            MainCamera.at.y = MainCamera.at.y - MainCamera.shakeAmp;
        }
        MainCamera.shakeTime--;
    }
}

s32 line_intersect_xz(s32 *p0, s32 *p1, s32 *q0, s32 *q1, s32 *out) {
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

s32 cam_frame_hit(CamFramePoint *a, CamFramePoint *b, CamFramePoint *c, CamFramePoint *d, s32 *out) {
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

void CalcCameraFrame(void) {
    MATRIX m;
    VECTOR in;
    VECTOR out;
    CamFrameEntry *e;
    s32 n;
    camera_info_tag *cam;
    s32 z0;

    cam = &MainCamera;
    RotMatrix(&cam->xform.angle, &m);
    n = camera_frame_list_size;
    CamBoundBottom = 0x7FFFFFFF;
    CamBoundTop = 0x80000000;
    CamBoundLeft = 0x7FFFFFFF;
    CamBoundRight = 0x80000000;
    e = CameraFrameList;
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
        v = x + (z0 + 0x3000) * 320 / MainCamera.dist;
        if (v < CamBoundLeft) {
            CamBoundLeft = v;
            camera_frame_left.x = x;
            camera_frame_left.z = z0;
        }
        z = out.vz - e->unk8;
        xr =out.vx + e->unk6;
        v = xr - (z + 0x3000) * 320 / MainCamera.dist;
        if (v > CamBoundRight) {
            CamBoundRight = v;
            camera_frame_right.x = xr;
            camera_frame_right.z = z;
        }
        z = out.vz - e->unk8;
        xr = out.vy - e->unkC;
        v = xr + (z + 0x3000) * 240 / MainCamera.dist;
        if (v < CamBoundBottom) {
            CamBoundBottom = v;
            camera_frame_bottom.y = xr;
            camera_frame_bottom.z = z;
        }
        z = out.vz - e->unk8;
        w = out.vy + e->unkC;
        v = w - (z + 0x3000) * 240 / MainCamera.dist;
        if (v > CamBoundTop) {
            CamBoundTop = v;
            camera_frame_top.y = w;
            camera_frame_top.z = z;
        }

        z = out.vz + e->unkA;
        xr = out.vx - e->unk4;
        v = xr + (z + 0x3000) * 320 / MainCamera.dist;
        if (v < CamBoundLeft) {
            CamBoundLeft = v;
            camera_frame_left.x = xr;
            camera_frame_left.z = z;
        }
        z = out.vz + e->unkA;
        w = out.vx + e->unk6;
        v = w - (z + 0x3000) * 320 / MainCamera.dist;
        if (v > CamBoundRight) {
            CamBoundRight = v;
            camera_frame_right.x = w;
            camera_frame_right.z = z;
        }
        z = out.vz + e->unkA;
        w = out.vy - e->unkC;
        v = w + (z + 0x3000) * 240 / MainCamera.dist;
        if (v < CamBoundBottom) {
            CamBoundBottom = v;
            camera_frame_bottom.y = w;
            camera_frame_bottom.z = z;
        }
        w = out.vy + e->unkC;
        z = out.vz + e->unkA;
        v = w - (z + 0x3000) * 240 / MainCamera.dist;
        if (v > CamBoundTop) {
            CamBoundTop = v;
            camera_frame_top.y = w;
            camera_frame_top.z = z;
        }
        e++;
    }
}

void CameraFrameHeroes(s32 arg0) {
    s32 pos[4];
    s32 d[3];
    camera_info_tag *view;
    GameState *g;
    s32 z;
    s32 w;
    s32 v;

    BuildCameraFrameList();
    CalcCameraFrame();
    if (cam_frame_hit(&camera_frame_bottom, &camera_frame_top, &camera_frame_left, &camera_frame_right, pos) != 0) {
        d[0] = MainCamera.at.x - pos[0];
    }
    d[1] = MainCamera.at.y - pos[1];
    g = (GameState *)GameInfo;
    d[2] = MainCamera.at.z - pos[2];
    if (!(g->exitFlags & 0x801) && g->mode != 0x1F) {
        if (d[0] < -0x514) {
            d[0] = -0x514;
        }
        if (d[0] > 0x514) {
            d[0] = 0x514;
        }
    }
    if (arg0 != 0) {
        MainCamera.eye.x = d[0];
        MainCamera.eye.y = d[1];
        MainCamera.eye.z = d[2];
        return;
    }
    view = &MainCamera;
    w = d[2];
    z = view->eye.z;
    if (w < z) {
        if (GameMode != 0x1F) {
            view->eye.z = z + ((w - z) >> 4);
            view->eye.x += (d[0] - view->eye.x) >> 4;
            view->eye.y += (d[1] - view->eye.y) >> 4;
        } else {
            view->eye.z = z + ((w - z) >> 2);
            view->eye.x += (d[0] - view->eye.x) >> 2;
            view->eye.y += (d[1] - view->eye.y) >> 2;
        }
    } else {
        view->eye.z = z + ((w - z) >> 2);
        view->eye.x += (d[0] - view->eye.x) >> 2;
        view->eye.y += (d[1] - view->eye.y) >> 2;
    }
}

void CamFromPath1(camera_info_tag *cam) {
    s32 *t = path_get(1);
    GameState *g;

    if (t != NULL) {
        cam->xform.angle.vx = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;  /* the path record's two points (+0x34, +0x50) */
        cam->dist = 0x200;
        g = (GameState *)GameInfo;
        cam->xform.angle.vy = 0;
        cam->xform.angle.vz = 0;
        if (g->mode == 0x19) {
            cam->dist = 0x258;
        }
        if (g->mode == 0x18) {
            cam->dist = 0x22B;
        }
        if (g->mode == 0x17) {
            cam->dist = 0x22B;
        }
        if (g->mode == 0x1A) {
            cam->dist = 0x22B;
        }
        if (g->mode == 0x1C) {
            cam->dist = 0x258;
        }
    }
    GameViewport.ofy = 0x15E;
}

void CamFromPath9(camera_info_tag *cam) {
    s32 *t = path_get(9);

    if (t != NULL) {
        cam->xform.angle.vx = (get_angle(t[0x58 / 4] - t[0x3C / 4], t[0x50 / 4] - t[0x34 / 4]) - 0x400) & 0xFFF;  /* as CamFromPath1 */
        cam->xform.angle.vy = 0;
        cam->xform.angle.vz = 0;
        cam->dist = 0x200;
    }
    GameViewport.ofy = 0x15E;
}

void camera_count_bottom(s32 arg0, s32 arg1) {
    s32 temp = arg0 - ((arg1 + 0x3000) * 0xF0) / MainCamera.dist;

    if (CamBoundTop < temp) {
        CamBoundTop = temp;
        camera_frame_top.y = arg0;
        camera_frame_top.z = arg1;
    }
}

void CamBoundBottomPoint(s32 arg0, s32 arg1) {
    s32 v = arg0 + ((arg1 + 0x3000) * 0xF0) / MainCamera.dist;

    if (v < CamBoundBottom) {
        CamBoundBottom = v;
        camera_frame_bottom.y = arg0;
        camera_frame_bottom.z = arg1;
    }
}

void CamBoundRightPoint(s32 arg0, s32 arg1) {
    s32 temp = arg0 - ((arg1 + 0x3000) * 0x140) / MainCamera.dist;

    if (CamBoundRight < temp) {
        CamBoundRight = temp;
        camera_frame_right.x = arg0;
        camera_frame_right.z = arg1;
    }
}

void CamBoundLeftPoint(s32 arg0, s32 arg1) {
    s32 v = arg0 + ((arg1 + 0x3000) * 0x140) / MainCamera.dist;

    if (v < CamBoundLeft) {
        CamBoundLeft = v;
        camera_frame_left.x = arg0;
        camera_frame_left.z = arg1;
    }
}

void camera_add_frame_list(SLONG_XYZ *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    CamFrameEntry *e;

    if (camera_frame_list_size < 0x10) {
        e = &CameraFrameList[camera_frame_list_size];
        e->unk0 = arg0;
        e->unk4 = arg1;
        e->unk6 = arg2;
        e->unk8 = arg3;
        e->unkA = arg4;
        e->unkC = arg5;
        camera_frame_list_size += 1;
    }
}

s32 CamEdgeIntersect(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2) {
    s32 sp18[3];
    s32 sp28[3];
    s32 d;

    d = 0x3C0000 / MainCamera.dist;
    sp18[0] = arg0->a + d;
    sp18[2] = arg0->c - 0x3000;
    sp28[0] = arg1->a - d;
    sp28[2] = arg1->c - 0x3000;
    if (line_intersect_xz(arg0, sp18, arg1, sp28, arg2) != 0) {
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

s32 CamEdgeIntersectZY(VECTOR *arg0, VECTOR *arg1, s32 arg2) {
    VECTOR sp18;
    VECTOR sp28;
    s32 d = 0x2D0000 / MainCamera.dist;
    extern s32 line_intersect_zy(VECTOR *, VECTOR *, VECTOR *, VECTOR *, VECTOR *);

    sp18.vy = arg0->vy + d;
    sp18.vz = arg0->vz - 0x3000;
    sp28.vy = arg1->vy - d;
    sp28.vz = arg1->vz - 0x3000;
    return line_intersect_zy(arg0, &sp18, arg1, &sp28, (VECTOR *)arg2) != 0;
}

s32 span_z(SLONG_XYZ *arg0, s32 arg1) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    SLONG_XYZ *var_a0;

    var_a0 = arg0;
    var_t0 = 0x80000000;
    var_a3 = 0x7FFFFFFF;
    var_a2 = 0;
    if (arg1 > 0) {
        do {
            temp_v1 = var_a0->z;
            if (var_t0 < temp_v1) {
                var_t0 = temp_v1;
            }
            if (temp_v1 < var_a3) {
                var_a3 = temp_v1;
            }
            var_a2 += 1;
            var_a0++;
        } while (var_a2 < arg1);
    }
    return var_t0 - var_a3;
}

extern s32 ScreenCount;
void InitScreenLabels(void) {
    EXTengine *eng;
    s32 v;

    eng = GetEngineSlot(3);
    ScreenLabels4P[6].unk1E = HeroNameText[GAME->heroes[0]];
    ScreenLabels4P[7].unk1E = HeroNameText[GAME->heroes[1]];
    ScreenLabels4P[8].unk1E = HeroNameText[GAME->heroes[2]];
    ScreenLabels4P[9].unk1E = HeroNameText[GAME->heroes[3]];
    ScreenLabels4P[10].unk1E = GAME->adventure.unkCC[0] + 0x200E;
    ScreenLabels4P[11].unk1E = GAME->adventure.unkCC[1] + 0x200E;
    ScreenLabels4P[12].unk1E = GAME->adventure.unkCC[2] + 0x200E;
    ScreenLabels4P[13].unk1E = GAME->adventure.unkCC[3] + 0x200E;
    ScreenLabelsTeams[4].unk1E = HeroNameText[GAME->heroes[0]];
    ScreenLabelsTeams[5].unk1E = HeroNameText[GAME->heroes[1]];
    ScreenLabelsTeams[6].unk1E = HeroNameText[GAME->heroes[2]];
    ScreenLabelsTeams[7].unk1E = HeroNameText[GAME->heroes[3]];
    ScreenLabelsTeams[8].unk1E = GAME->adventure.unkCC[GAME->teamLeader[0]] + 0x200E;
    ScreenLabelsTeams[9].unk1E = GAME->adventure.unkCC[GAME->teamLeader[1]] + 0x200E;
    ScreenCount = 0;
    ScreenLabelsPair[3].unk1E = HeroNameText[GAME->heroes[0]];
    ScreenLabelsPair[4].unk1E = HeroNameText[GAME->heroes[1]];
    ScreenLabelsPair[5].unk1E = GAME->adventure.unkCC[0] + 0x200E;
    ScreenLabels2P[4].unk1E = HeroNameText[GAME->heroes[0]];
    ScreenLabels2P[5].unk1E = HeroNameText[GAME->heroes[1]];
    ScreenLabels2P[6].unk1E = GAME->adventure.unkCC[0] + 0x200E;
    ScreenLabels2P[7].unk1E = GAME->adventure.unkCC[1] + 0x200E;
    ScreenLabelsOne[3].unk1E = HeroNameText[GAME->heroes[0]];
    ScreenLabelsOne[4].unk1E = GAME->adventure.unkCC[0] + 0x200E;
    switch (GAME->levelFlags & 0xF0) {
    case 0x20:
        v = 0x2021;
        if (GAME->levelFlags & 8) {
            v = 0x2020;
        }
        ScreenLabels4P[0].unk1E = v;
        ScreenLabelsTeams[0].unk1E = v;
        ScreenLabelsPair[0].unk1E = v;
        ScreenLabelsOne[0].unk1E = v;
        if (GAME->teams != 0) {
            PlaceEngineSlot(3, (PlaceRec *)ScreenLabelsPair);
            ScreenCount = 1;
        } else {
            PlaceEngineSlot(3, (PlaceRec *)ScreenLabelsOne);
            ScreenCount = 1;
        }
        break;
    case 0x40:
        ScreenLabelsNotice[0].unk1E = 0x201F;
        PlaceEngineSlot(3, (PlaceRec *)ScreenLabelsNotice);
        goto end;
    case 0x80:
        ScreenLabelsNotice[0].unk1E = 0x201E;
        PlaceEngineSlot(3, (PlaceRec *)ScreenLabelsNotice);
        goto end;
    case 0x10:
    default:
        ScreenLabels4P[0].unk1E = 0x2022;
        ScreenLabelsTeams[0].unk1E = 0x2022;
        ScreenLabelsPair[0].unk1E = 0x2022;
        ScreenLabelsOne[0].unk1E = 0x2022;
        if (GAME->mode == 0x20) {
            PlaceEngineSlot(3, (PlaceRec *)ScreenLabels2P);
            ScreenCount = 2;
        } else if (GAME->teams != 0) {
            PlaceEngineSlot(3, (PlaceRec *)ScreenLabelsTeams);
            ScreenCount = 2;
        } else {
            PlaceEngineSlot(3, (PlaceRec *)ScreenLabels4P);
            ScreenCount = 4;
        }
        break;
    }
    eng->sub->flags &= 0xFFFF7FFF;
end:
    GetEngineSlot(3)->unk10.x = 0x140;
    GetEngineSlot(3)->unk10.y = 0x118;
    GetEngineSlot(3)->flags &= 0xEFFFFFFF;
}

void BobSub(subengine *s) {
    TrigEntry *t;
    s32 f;
    TrigEntry *e;

    s->unk51 = 0;
    t = rcossin_tbl;
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

void UpdateScreenLabels(void) {
    s32 *p = &ScreenLabelState;
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
        eng->unk20 = lerp_angle(-0x800, 0, 0x14, p[2]);
        eng->unk1C = lerp_angle(-0x400, 0, 0x14, p[2]);
        v = interp(0x800, 0x1800, 0x14, p[2]);
        eng->unk2C.z = v;
        eng->unk2C.y = v;
        eng->unk2C.x = v;
        eng->unk5E = 1;
        s = GetEngineSlot(3)->sub + 2;
        for (i = 0; i < ScreenCount; i++) {
            s->unk20.z = 0x199;
            s->unk51 = 0;
            s->unk50 = 1;
            s->flags |= 0x8000;
            s->unk12 = i * (0x1000 / (ScreenCount + 1)) + FrameCount * 40;
            s->unk52 = 1;
            s = s->next;
        }
        {
            subengine *e = GetEngineSlot(3)->sub;
            u32 t = GameLevelFlags & 0xF0;
            if (t == 0x40 || t == 0x80) {
                e->flags |= 0x8000;
                BobSub(e);
            } else {
                e->flags &= ~0x8000;
            }
        }
        q = &ScreenLabelState;
        if (++q[2] > 0x14) {
            q[2] = 0;
            q[1] = 1;
        }
        break;
    case 1:
        s = GetEngineSlot(3)->sub + 2;
        for (i = 0; i < ScreenCount; i++) {
            s->unk20.z = 0x199;
            s->unk51 = 0;
            s->unk50 = 1;
            s->flags |= 0x8000;
            s->unk12 = i * (0x1000 / (ScreenCount + 1)) + FrameCount * 40;
            s->unk52 = 1;
            s = s->next;
        }
        {
            subengine *e = GetEngineSlot(3)->sub;
            u32 t = GameLevelFlags & 0xF0;
            if (t == 0x40 || t == 0x80) {
                e->flags |= 0x8000;
                BobSub(e);
            } else {
                e->flags &= ~0x8000;
            }
        }
        if (!(GameLevelFlags & 0xC0)) {
            q = &ScreenLabelState;
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
            for (i = 0; i < ScreenCount; i++) {
                s->unk20.z = 0x199;
                s->unk51 = 0;
                s->unk50 = 1;
                s->flags |= 0x8000;
                s->unk12 = i * (0x1000 / (ScreenCount + 1)) + FrameCount * 40;
                s->unk52 = 1;
                s = s->next;
            }
            {
                subengine *e = GetEngineSlot(3)->sub;
                u32 t = GameLevelFlags & 0xF0;
                if (t == 0x40 || t == 0x80) {
                    e->flags |= 0x8000;
                    BobSub(e);
                } else {
                    e->flags &= ~0x8000;
                }
            }
        } else {
            {
                subengine *e = GetEngineSlot(3)->sub;
                e->flags |= 0x8000;
                BobSub(e);
            }
            {
                s32 j;
                subengine *e = GetEngineSlot(3)->sub + 2;
                for (j = 0; j < ScreenCount; j++) {
                    e->flags &= ~0x8000;
                    e = e->next;
                }
            }
        }
        {
            s32 *r = &ScreenLabelState;
            eng->unk1E = lerp_angle(0, 0x800, 0x14, ++r[2]);
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
            BobSub(e);
        }
        {
            s32 j;
            subengine *e = GetEngineSlot(3)->sub + 2;
            for (j = 0; j < ScreenCount; j++) {
                e->flags &= ~0x8000;
                e = e->next;
            }
        }
        break;
    }
}

void ScreenLabelsOff(void) {
    ScreenLabelState = 0;
}

void ScreenLabelsStart(void) {
    s32 *p;
    if (ScreenLabelState == 0) {
        ScreenLabelState = 1;
        p = &ScreenLabelState;
        p[1] = 0;
        p[2] = 0;
        InitScreenLabels();
        PlaySfx(0x19D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

extern s32 ScreenCount;
void HideScreenLabels(void) {
    s32 i;
    subengine *s = GetEngineSlot(3)->sub + 2;
    for (i = 0; i < ScreenCount; i++) {
        s->flags &= ~0x8000;
        s = s->next;
    }
}

void ShowScreenLabels(void) {
    s32 i;
    subengine *s = GetEngineSlot(3)->sub + 2;
    for (i = 0; i < ScreenCount; i++) {
        s->unk20.z = 0x199;
        s->unk51 = 0;
        s->unk50 = 1;
        s->flags |= 0x8000;
        s->unk12 = i * (0x1000 / (ScreenCount + 1)) + FrameCount * 40;
        s->unk52 = 1;
        s = s->next;
    }
}

void anim_item(void) {
    subengine *s = GetEngineSlot(3)->sub;
    s->flags |= 0x8000;
    BobSub(s);
}

void ShowLevelLabel(void) {
    subengine *s = GetEngineSlot(3)->sub;
    u32 t = GameLevelFlags & 0xF0;
    if (t == 0x40 || t == 0x80) {
        s->flags |= 0x8000;
        BobSub(s);
    } else {
        s->flags &= ~0x8000;
    }
}

void RenderScreenLabels(void) {
    s32 *s;
    s32 v;
    int new_var;

    gfx_set_viewport(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    if (ScreenLabelShown != -1) {
        s = (s32 *)GetEngineSlot(3)->sub;
        s[new_var = 0xA8 / 4] &= 0xFFFF7FFF;  /* the next subengine's flags: as s[1].flags the branches change */
        if ((GamePause & 0x240) != 0x240) {
            s[0xA8 / 4] |= 0x8000;
        }
        eng_render_subs(GetEngineSlot(3));
    }
}

void ResetScreenLabels(void) {
    s32 *p;
    ScreenLabelState = 0;
    p = &ScreenLabelState;
    p[1] = -1;
    p[3] = 0;
    p[4] = 1;
}

/* data, in address order (tools/scripts/data2c.py) */
CamEase CameraEase = { { 2, 2, 2 }, { 2, 2, 1 }, { 4, 4, 4 } };
s32 CameraModeFlags[5] = { 1, 1, 1, 0, 0 };
u16 CameraMode = 0;
s16 CameraUnused[92] = {0};
s32 camera_frame_list_size = 0;
CamFrameEntry CameraFrameList[16] = {0};
CamFramePoint camera_frame_bottom = {0};
CamFramePoint camera_frame_top = {0};
CamFramePoint camera_frame_left = {0};
CamFramePoint camera_frame_right = {0};
s32 CamBoundBottom = 0;
s32 CamBoundTop = 0;
s32 CamBoundLeft = 0;
s32 CamBoundRight = 0;
PlaceRec ScreenLabels4P[19] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2022, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 7, 0x1C0, -0x180, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, 0x1C0, -0x60, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, 0x1C0, 0xC0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, 0x1C0, 0x1E0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, -0x1C0, -0x1E0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, -0xC0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0x60, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0x180, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x40, -0x180, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x200F, 0 },
    { 7, -0x40, -0x60, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2010, 0 },
    { 7, -0x40, 0xC0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2011, 0 },
    { 7, -0x40, 0x1E0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2012, 0 },
    { 7, 0xC0, -0x180, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 7, 0xC0, -0x60, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 7, 0xC0, 0xC0, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 7, 0xC0, 0x1E0, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec ScreenLabelsTeams[13] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 7, 0x1C0, -0xF0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, 0x1C0, 0x150, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, -0x1C0, -0x1E0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, -0xC0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0x60, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0x180, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x40, -0xF0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x200F, 0 },
    { 7, -0x40, 0x150, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2010, 0 },
    { 7, 0xC0, -0xF0, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 7, 0xC0, 0x150, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec ScreenLabelsPair[8] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 7, 0x1C0, 0x10, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, -0x1C0, -0xE0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0x40, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x40, 0x10, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x200F, 0 },
    { 7, 0xC0, 0x10, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec ScreenLabelsOne[7] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 7, 0x1C0, 0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, -0x1C0, -0x60, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x40, 0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x200F, 0 },
    { 7, 0xC0, 0, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec ScreenLabels2P[11] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 7, 0x1C0, -0xF0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, 0x1C0, 0x150, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2001, 0 },
    { 7, -0x1C0, -0x150, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x1C0, 0xF0, 0, 0x11EB, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2009, 0 },
    { 7, -0x40, -0xF0, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x200F, 0 },
    { 7, -0x40, 0x150, 0, 0x1000, 0, 0, 0, 0, NULL, 0, 0x1C, 0x2010, 0 },
    { 7, 0xC0, -0xF0, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 7, 0xC0, 0x150, 0, 0xC00, 0, 0, 0, 0, NULL, 0, 0x1C, 0x201A, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
PlaceRec ScreenLabelsNotice[3] = {
    { 7, 0, 0, 0, 0x1C00, 0, 0, 0, 0, NULL, 0, 0x1C, 0, 0 },
    { 0xC, -0x330, 0x330, -0x2C0, 0x260, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0, 0, 0, 0 },
};
s32 ScreenLabelState = 0;
s32 ScreenLabelShown = 0;
s32 ScreenLabelFrame[1] = {0};  /* ScreenLabelState's [1]: -1 when off */
s32 CupsDirty = 0;
s32 ScreenLabelStep[1] = {0};  /* ScreenLabelState's [3] */
s32 ScreenCount = 0;
