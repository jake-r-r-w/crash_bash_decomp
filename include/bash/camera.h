#ifndef BASH_CAMERA_H
#define BASH_CAMERA_H

/* Functions and data defined in src/overlays/engine/camera.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/camera.c */

#include "common.h"
#include "bash/engine.h"

void CameraFrameHeroes(s32 arg0);

void CamFromPath1(camera_info_tag *cam);

s32 CamEdgeIntersect(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2);

s32 span_z(SLONG_XYZ *arg0, s32 arg1);

void ScreenLabelsOff(void);

s32 cam_frame_hit(CamFramePoint *a, CamFramePoint *b, CamFramePoint *c, CamFramePoint *d, s32 *out);
void CalcCameraFrame(void);
void BuildCameraFrameList(void);

void UpdateCamera(s32 arg0);


s32 line_intersect_xz(s32 *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);

/* CamEdgeIntersectZY's body, inlined by cam_frame_hit. */
static inline s32 EngCam_HitY(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2) {
    VECTOR sp18;
    VECTOR sp28;
    s32 d = 0x2D0000 / MainCamera.dist;
    extern s32 line_intersect_zy(VECTOR *, VECTOR *, VECTOR *, VECTOR *, VECTOR *);

    sp18.vy = arg0->vy + d;
    sp18.vz = arg0->vz - 0x3000;
    sp28.vy = arg1->vy - d;
    sp28.vz = arg1->vz - 0x3000;
    return line_intersect_zy(arg0, &sp18, arg1, &sp28, arg2) != 0;
}

/* CamEdgeIntersect's body, inlined by cam_frame_hit. */
static inline s32 EngCam_HitX(Vec3W *arg0, Vec3W *arg1, Vec3W *arg2) {
    s32 sp18[4];
    s32 sp28[4];
    s32 d = 0x3C0000 / MainCamera.dist;

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

extern CamEase CameraEase;
void BobSub(subengine *s);

void TitlesCameraHook(camera_info_tag *cam, s32 arg1);
void HubCameraFollow(camera_info_tag *cam, s32 arg1);
void PodiumCamera(CamView *out, s32 arg1);
void N2OCameraFollow(camera_info_tag *cam);

void ShowScreenLabels(void);

void ResetScreenLabels(void);

void camera_count_bottom(s32 arg0, s32 arg1);

void CamBoundBottomPoint(s32 arg0, s32 arg1);

void anim_item(void);

void CamFromPath9(camera_info_tag *cam);

void CamBoundRightPoint(s32 arg0, s32 arg1);

void CamBoundLeftPoint(s32 arg0, s32 arg1);

void camera_add_frame_list(SLONG_XYZ *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);

void ScreenLabelsStart(void);

void HideScreenLabels(void);

void ShowLevelLabel(void);

s32 CamEdgeIntersectZY(VECTOR *arg0, VECTOR *arg1, s32 arg2);

void RenderScreenLabels(void);



/* Read through a cast, not a local: sched then sees the base as the symbol and
 * moves these loads past the record stores, as the target does. */

extern u16 HeroNameText[];
extern TextPanelItem ScreenLabels4P[];
extern TextPanelItem ScreenLabelsTeams[];
extern TextPanelItem ScreenLabelsPair[];
extern TextPanelItem ScreenLabelsOne[];
extern TextPanelItem ScreenLabels2P[];
extern TextPanelItem ScreenLabelsNotice[];

void InitScreenLabels(void);

#endif /* BASH_CAMERA_H */
