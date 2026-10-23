#ifndef BASH_OVL67_H
#define BASH_OVL67_H

/* Functions and data defined in the ovl67 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl67/ovl67.c */

#include "common.h"
#include "bash/nas.h"

/* D_8009D87C (declared in bash/pause.h, which this unit does not include). */
typedef struct Ovl67Odds {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Ovl67Odds;

extern Ovl67Odds D_8009D87C[];

s32 func_800BB05C(u8 *arg0);
u8 *func_800BB624(void);
void func_800BBD18(void);
void func_800B34A4(void);
void func_800B361C(void);

extern u16 *D_8009D6EC;

extern u8 D_80053EAC[];
extern EXTengine *D_8009D530;
extern EXTengine *D_800C439C;
extern u16 D_800AD12C;
extern u16 D_800C39A0[];
extern s16 D_800C4150[][3];
extern u16 D_800C417A;
extern u16 D_800C417C[];
extern u16 D_800C39EC[][5];
extern s32 D_800C4050[][3];
extern EXTengine *D_8009D59C;
void func_800C3440(void);
extern u16 D_800C438A;
extern s16 D_800C4184[];
extern u16 D_800C39EC[][5];
extern s32 (*D_8009D750)(EXTengine *);

/* PadState (D_80051338) as func_800B447C reads it: the buttons at 0x50. */
typedef struct Ovl67Pad {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad4[0x50 - 0x04];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
} Ovl67Pad;

/* PadState as func_800B5064 reads it: flags at 0x4C and 0x54. */
typedef struct Ovl67PadFlags {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl67PadFlags;

/* The overlay's other files (ARCHITECTURE.md, Files inside the slot overlays). */
#include "bash/ovl67_800bbdb8.h"

void func_800BBA4C(s32 arg0);
extern HeroInfo Heroes[4];

void func_800B59A8(s32 arg0);
extern char D_80053EE4[];
s32 csqrt(s32 x);

NAS *func_8007A524(s32 arg0, s32 *arg1, s32 arg2);
void func_800898EC(s32 arg0);

void func_800B9800(SLONG_XYZ *pos, s32 arg1, SLONG_XYZ *target, s32 r);

s32 func_800BBBB4(s32 arg0, s16 arg1, s32 arg2, s16 arg3);

/* Each player's reach box for hitting the ball (func_800B9CB8): the y range
 * relative to the hero, the near and far radii, and the near and far points. */
typedef struct Ovl67HeroBox {
    s32 a, b, c, d;
    s32 x0, y0, z0;
    s32 x1, y1, z1;
} Ovl67HeroBox;

extern Ovl67HeroBox D_800C4190[];

/* Extension 0x800 of a ball (func_800B9CB8), as in ovl61's Ovl61Puck. */
typedef struct Ovl67Puck {
    u8 pad0[0xE];
    s16 speed;
    u8 pad10[4];
    s16 max;
    s16 dir;
    u8 pad18[0x32 - 0x18];
    u16 unk32;
} Ovl67Puck;

extern u16 D_800C39B0[];
extern u16 D_800C39DC[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 func_800B9CB8(s32 arg0, EXTengine *unused);

typedef struct Ovl67Vel {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
} Ovl67Vel;

typedef struct Ovl67Blend {
    u8 pad0[0xE];
    s16 speed_a;
    u8 pad10[6];
    u16 dir;
    u8 pad18[0xA];
    s16 speed_b;
    u8 pad24[6];
    u16 dir_b;
} Ovl67Blend;

typedef struct Ovl67Out {
    u8 pad0[0x16];
    s16 dir;
} Ovl67Out;

extern u16 D_800C39B0[];
extern s32 D_8005A618;
extern s32 D_8005A680[];

s32 func_800B9CB8(s32 arg0, EXTengine *unused);

void func_800B447C(s32 arg0);

s32 func_800BAEA8(Ovl67Vel *arg0, s32 arg1);

void PlotN2ODeadWall(NAS *arg0);

void func_800BB5F0(void);

void func_800BB72C(s32 arg0);

void func_800BB788(s32 arg0);
/* Round state cleared by func_800B361C. */
typedef struct Ovl67Round {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
} Ovl67Round;

typedef struct Ovl67Scene {
    u8 pad0[0x27];
    u8 unk27;
} Ovl67Scene;

typedef struct Ovl67FlagNode {
    u32 flags;
    u8 pad4[0x58];
    struct Ovl67FlagNode *next;
    u8 pad60[0xC];
    u8 *unk6C;
    u8 pad70[4];
    u16 unk74;
} Ovl67FlagNode;

typedef struct Ovl67SceneRef {
    u8 pad0[0x10];
    Ovl67Scene *unk10;
    u8 pad14[8];
    Ovl67FlagNode *unk1C;
} Ovl67SceneRef;

extern Ovl67Round D_800C4398;
extern s32 D_8005AA3C;
extern s32 D_800C4080;
extern NAS *D_800C4084;
typedef struct {
    s16 x;
    s16 z;
    s16 r;
} Ovl67Circle;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[0x6E];
    s16 radius;
    u8 pad8C[4];
    u32 flags;
} Ovl67Obj;

extern Ovl67Circle *D_800C418C;
extern u8 D_800C37DC[];

/* The object a D_800C42D0 slot moves (func_800B92D4). */
typedef struct Ovl67SlotObj {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ s16 ang;
    /* 0x14 */ u8 pad14[0x3D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u16 unk52;
} Ovl67SlotObj;

/* D_8005A614 as seen by func_800B92D4. */
typedef struct Ovl67Aim {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad32[0x3A];
    /* 0x6C */ s32 unk6C;
} Ovl67Aim;

typedef struct Ovl67Slot {
    Ovl67SlotObj *src;
    s32 unk4;
    NAS *nas;
    u16 f0C;
    u8 pad0E[4];
    u16 f12;
    u8 pad14[2];
    u16 f16;
    u16 f18;
    u16 kind;
    s32 f1C;
    s32 f20;
    s32 f24;
} Ovl67Slot;
extern Ovl67Slot D_800C42D0[];

typedef struct Ovl67Ent {
    s32 flags;
    u8 pad4[4];
    s32 y;
    u8 pad0C[0x50];
    struct Ovl67Ent *next;
    u8 pad60[0xC];
    u8 *key;
    u8 pad70[4];
    u16 id;
} Ovl67Ent;

typedef struct Ovl67Rec {
    Ovl67Ent *ent;
    s32 f4;
    s32 f8;
    u16 fC;
    u8 pad0E[4];
    u16 f12;
    u8 pad14[2];
    u16 f16;
    u16 f18;
    u16 f1A;
    s32 f1C;
    s32 f20;
    s32 f24;
} Ovl67Rec;
extern Ovl67Rec D_800C4230[];
extern char D_80054504[];
extern s16 D_800C4370;
extern s16 D_800C4372;
extern s16 D_800C4374;
extern s16 D_800C4376;
extern s16 D_800C4378;
extern s16 D_800C437A;
extern s16 D_800C437C;
extern s16 D_800C437E;
extern s16 D_800C4380;
extern s16 D_800C4382;
extern u16 D_800C4384;
extern s32 D_800C438C;
extern s32 D_800C4390;

/* func_800B7244's script: kind (99 = jump to step), slot kind, time/step. */
typedef struct Ovl67Step {
    u16 kind;
    u16 slot;
    u16 time;
} Ovl67Step;
extern Ovl67Step D_800C391C[];
typedef struct Ovl67Half {
    s16 v;
} Ovl67Half;
typedef struct Ovl67PanRef {
    struct Ovl67Pan *p;
} Ovl67PanRef;
extern u16 D_800C39A6;
extern s16 D_8009D6D6;
extern u16 D_8009D6E0;
typedef struct Ovl67Pan {
    u8 pad0[0xC];
    s32 unkC;
} Ovl67Pan;
extern Ovl67Pan *D_8009D688;
extern u16 D_800C4394;
extern s16 D_800C43DC;
void func_800B6F4C(void);
void func_800B90F4(void);
void func_800BB070(u16 arg0);
void InitN2OmissileExplode(NAS *nas);

void func_800C349C(void);

s32 func_800BB8EC(s32 arg0);

void func_800BBD68(void);
/* A ball in the D_8005AA38 list (func_800B817C), as in ovl61's Ovl61Node. */
typedef struct Ovl67Node {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[4];
    s32 z;
    u8 pad1C[4];
    struct Ovl67Node *next;
    u8 pad24[0x30];
    struct Ovl67Node *target;
    struct {
        u8 pad0[0x14];
        u16 unk14;
        u8 pad16[4];
        u16 unk1A;
    } *unk58;
} Ovl67Node;

typedef struct Ovl67Pos {
    s32 x;
    s32 y;
    s32 z;
} Ovl67Pos;

s32 func_800B3C04(Ovl67Obj *arg0);

void func_800B4FA4(s32 arg0);

void func_800B5064(s32 arg0);

void func_800B5674(s32 arg0);

void func_800B5774(s32 arg0);

void func_800B5B14(s32 arg0);

void func_800B5BFC(void);

typedef struct Ovl67Edge {
    s16 x0;
    s16 pad2;
    s16 z0;
    s16 pad6;
    s16 x1;
    s16 padA;
    s16 z1;
    s16 padE;
} Ovl67Edge;
extern Ovl67Edge D_800C4090[];
extern u16 D_800C4110[];
extern u16 D_800C4386;

s32 func_800B7858(s32 arg0, Ovl67Pos *from, Ovl67Pos *to, s32 angle);

s32 func_800B7C58(s32 arg0, Ovl67Pos *from, Ovl67Pos *to, s32 angle);

s32 func_800B817C(s32 arg0, s32 arg1, s32 arg2, u16 arg3, u16 arg4, s32 arg5);

void func_800B8968(s32 a);

void func_800BB184(void);

void PlotN2OmissileExplode(NAS *nas);

void func_800BB3A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void InitN2Ofirstmissile(NAS *nas);

void InitN2Omissile(NAS *nas);

void func_800BB6BC(s32 arg0);

void func_800BB7D4(s32 arg0);

void func_800BB86C(s32 arg0);

s32 func_800BB944(s32 arg0);

s32 func_800BB9B0(s32 arg0);

void func_800BBB10(s32 arg0);

s32 func_800BBC78(Ovl67Vel *arg0, Ovl67Out *arg1, s32 arg2);

extern s8 D_8005A689;

s32 func_800B4AD8();
extern u32 *D_800C43D8;

s32 func_800B3888(Ovl67Obj *arg0);

void func_800B5168(s32 arg0);

void func_800B52CC(s32 arg0);

void func_800B5440(s32 arg0);

void func_800B5550(s32 arg0);

void func_800B5838(s32 arg0);

void InitN2OBall(NAS *nas);

void func_800B6E6C(Ovl67Blend *p);

s32 func_800B8058(s32 arg0);

/* func_800B83EC */
extern s32 D_8005A624;
extern s32 D_8009D908;
extern u32 D_80051244[]; /* flags by angle octant */

/* D_8005A614 as seen by func_800B34A4. */
typedef struct Ovl67Game {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u8 pad34[0x1D];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 pad54[0x18];
    /* 0x6C */ s32 unk6C;
} Ovl67Game;
#define OVG ((Ovl67Game *)D_8005A614)

/* D_8005A614 as seen by PlotN2Omissile. */
typedef struct Ovl67MissileView {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x64];
    /* 0x6C */ s32 unk6C;
} Ovl67MissileView;
#define OVM ((Ovl67MissileView *)D_8005A614)
extern u16 D_800C39CC[];
extern u16 D_800C39D4[];

/* D_8005A614 as seen by func_800B6F4C. */
typedef struct Ovl67Teams {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x65];
    /* 0x9B */ u8 unk9B[0x10];
    /* 0xAB */ u8 unkAB[4];
} Ovl67Teams;
#define OVT ((Ovl67Teams *)D_8005A614)
extern u16 D_800C3EB0[];
extern u16 D_800C3EF0[];
extern s16 D_800C4182;
extern s32 D_800C43E0;
extern s16 D_8009D6E2;
void InitBossHealthIcons(void);
void func_80086868(void);

/* D_8009D6E8 (EngineHooks in bash/engine.h, which this unit does not include). */
typedef struct Ovl67Hooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ void *unk2C;
    /* 0x30 */ u8 pad30[0x38];
    /* 0x68 */ void *unk68;
} Ovl67Hooks;
extern Ovl67Hooks D_8009D6E8;
extern void *D_8009D8FC;
extern s16 D_8009E0D8;
extern s16 D_8009E0E2;
extern s32 D_800C4498;
extern s32 D_800C45F0;
extern u8 D_800C403C[];
extern u8 D_800C3774[];
extern u8 D_800C3E30[];
extern u8 D_800C3E70[];
s32 func_800B39C0(Ovl67Obj *arg0, s32 arg1);

void func_800B3CD0(s32 arg0);

void func_800B3E74(s32 i);
void func_800B46DC(s32 arg0);
void func_800B4CE0(s32 arg0);

void func_800B4E30(s32 arg0);

void InitN2ODeadWall(NAS *nas);

s32 func_800BA43C(u32 arg0, s32 arg1, bool arg2);

NAS *func_800BAAA8(void);

s32 func_800B9B10(s32 arg0);
void func_800BEB68(s32 arg0);
void func_800BF0AC(s32 arg0);

extern Ovl67Edge D_800C40D0[];

/* Motion extension (0x800) viewed by func_800BA6C4. */
typedef struct Ovl67Mot {
    u8 pad0[0xE];
    s16 a;
    s16 b;
    s16 c;
    s16 lim;
    s16 dir;
} Ovl67Mot;

/* A FUX viewed for its flags word (0x80 tested by func_800BA6C4). */
typedef struct Ovl67FuxF {
    u8 pad0[0x1C];
    u16 flags;
} Ovl67FuxF;

void func_800B7244(void);

void func_800B83EC(u16 *arg0);

void PlotN2Omissile(NAS *nas);

void PlotN2Ofirstmissile(NAS *nas);

void func_800B92D4(void);

s32 func_800BA6C4(EXTengine *eng, Ovl67FuxF *self);

/* D_8005A614 as seen by PlotN2OBall. */
typedef struct Ovl67BallView {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x1C];
    /* 0x30 */ u16 unk30;
} Ovl67BallView;
extern EXTengine *D_8009D674;
extern s16 D_8009D88E;
extern s32 D_8009E424[];
extern SLONG_XYZ D_800C3794[];
extern u16 D_800C39A8[];
extern u16 D_800C39B8[];

/* D_8005AA94 as PlotN2OBall reads it: the level's contour hook. */
typedef struct Ovl67Hook {
    s32 (*fn)(EXTengine *eng);
} Ovl67Hook;

/* PlotN2OBall's bounds tests: past an edge sets the scoring side. */
static inline s32 N2OBallOut(EXTengine *eng, EXTobject *obj, s32 wide) {
    s32 lim;
    s32 x;
    s32 z;

    lim = 0xB00;
    if (wide != 0) {
        lim = 0x1200;
    }
    x = eng->unk10.x;
    if (x > lim) {
        obj->player = -1;
        return 1;
    }
    if (x < -lim) {
        obj->player = -1;
        return 1;
    }
    z = eng->unk10.z;
    if (z > lim) {
        obj->player = 3;
        return 1;
    }
    if (z < -lim) {
        obj->player = 0;
        return 1;
    }
    if (wide != 0) {
        if (z < -0xA00) {
            if (x > 0x600) {
                eng->unk10.x = 0x600;
            }
            if (eng->unk10.x < -0x600) {
                eng->unk10.x = -0x600;
            }
        }
    }
    return 0;
}

void PlotN2OBall(NAS *nas);

#endif /* BASH_OVL67_H */
