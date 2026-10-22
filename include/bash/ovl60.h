#ifndef BASH_OVL60_H
#define BASH_OVL60_H

/* Functions and data defined in the ovl60 overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/ovl60/ovl60.c */

#include "common.h"
#include "bash/nas.h"

void func_800C318C(s32 arg0);
void func_800C3390(void);
void UpdatePOGO(void);
extern u8 D_8008AB04[];
extern u8 D_800C52CC[];
extern u8 D_800C44A8[];
void func_800B3534();

/* The engine callback table (EngineHooks in bash/engine.h). */
typedef struct Ovl60Hooks {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ void *unk34;
    /* 0x38 */ void *unk38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ void *unk40;
    /* 0x44 */ void *unk44;
    /* 0x48 */ void *unk48;
    /* 0x4C */ void *unk4C;
    /* 0x50 */ void *unk50;
    /* 0x54 */ void *unk54;
    /* 0x58 */ void *unk58;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
} Ovl60Hooks;
extern Ovl60Hooks D_8009D6E8;
extern u8 D_800C52B8[];
extern u8 D_8009F8A4;
void func_800C34B0(void);
void func_800C34D0(void);
void func_800C34F0(s32 arg0);
void func_800C3510(u8 *arg0);

void func_800C353C();

void func_80089FA4(s32 arg0);
extern u16 D_800AD12C;

/* D_80051640 as the camera PogoInit_Camera sets up. */
typedef struct Ovl60Cam {
    /* 0x00 */ s32 eyeX, eyeY, eyeZ;
    /* 0x0C */ s32 atX, atY, atZ;
    /* 0x18 */ s32 dist;
    /* 0x1C */ u8 pad1C[0x54 - 0x1C];
    /* 0x54 */ s16 rotX;
    /* 0x56 */ s16 rotY;
    /* 0x58 */ s16 rotZ;
} Ovl60Cam;
/* What a board cell's unk4 points to (Plot_PogoBanker reads its x and z). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 z;
} Ovl60CellPos;
typedef struct {
    HeroInfo *hero;
    Ovl60CellPos *unk4;
    u8 unk8[2];
    s16 unkA;
    u8 unkC[2];
    u16 unkE;
    u16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8 unk18[4];
} Ovl60Cell; /* size 0x1C */

extern Ovl60Cell D_800C53DC[8][8];
/* The board cell at byte offset off from base (D_800C53DC). */
#define CELL_AT(off) ((Ovl60Cell *)((off) + (u32)base))
/* The position a board cell's unk4 points to, as PlaySfx takes it. */
#define CELL_POS(cx, cz) ((SLONG_XYZ *)&((Ovl60Cell *)((cx) * sizeof(D_800C53DC[0]) + (cz) * sizeof(Ovl60Cell) + (u32)D_800C53DC))->unk4->x)

/* A scene object in the list at D_80058AD0 + 0x1C (InitPogo_Tiles). */
typedef struct Ovl60Obj {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x5C - 0x4];
    /* 0x5C */ struct Ovl60Obj *next;
    /* 0x60 */ u8 pad60[0x6C - 0x60];
    /* 0x6C */ u8 *model;
    /* 0x70 */ u8 pad70[0x74 - 0x70];
    /* 0x74 */ u16 index;
} Ovl60Obj;
extern Ovl60Obj *D_800C5BB4;
extern HeroInfo Heroes[4];

/* 0x1C-byte entries of D_800C535C */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u16 unk4; /* state */
    /* 0x06 */ s16 unk6; /* timer */
    /* 0x08 */ s32 unk8[2];
    /* 0x10 */ NAS *nas;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 unk18;
} Ovl60Slot;

extern Ovl60Slot D_800C535C[];
/* PadState (D_80051338) as Fire_Rocket reads it: d-pad bits 0xF0 at 0x50. */
typedef struct Ovl60Pad {
    /* 0x00 */ u8 unk0[0x50];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} Ovl60Pad;
void func_800B935C(s32 x, s32 z, s32 hero);
void Tile_Claim(s32 arg0, s32 arg1, s32 arg2);
extern s32 D_800C53CC[];
extern s16 D_800C44D8[];
extern s32 D_800C4484[];
void func_800C02AC(EXTengine *eng, s32 angle);
extern u8 D_80053B9C[];
extern u8 D_80053ABC[];
/* Plot_PogoMushroom's 16-step y scale bounce. */
extern s32 D_800C45FC[16];
/* Plot_PogoMushroom's puff of smoke at pos (type 0x1605, flags 0x11). */
static inline void Ovl60_SpawnPuff(s32 *pos, s32 mode, u8 *p) {
    NAS *obj;
    s32 *ext;

    obj = SpawnNASById(pos[0], pos[1], pos[2], 0x1605, 0x11);
    if (obj != NULL) {
        ext = FindExtension(obj, 0x10000000);
        if (ext != NULL) {
            ext[3] = mode;
            ext[6] = 0x100;
            if (p != NULL) {
                ext[8] = 5;
                ((s32 *)obj->Engine->sub)[0x6C / 4] = (s32)p;
            } else {
                ext[8] = 1;
            }
            ext[10] = 0;
            obj->InitDelete(obj);
        }
    }
}
extern u8 D_80053BD4[];
extern ScriptInit D_800C4680;
extern s32 D_800C5308;
extern s32 D_8005A680[];
extern u8 D_8005A614[];
extern NAS *D_800C5BB0;
extern s32 D_800C52E0;
extern u16 D_800C530C[];
extern s32 D_800C52E4;
extern s32 D_800C52EC;
extern s16 D_800C52E8;
extern s32 D_800C38D8[];
extern s32 D_800C38E8[];
extern s32 D_800C38F8[];
extern s32 D_800C52F0;
/* the s16 after D_800C52E8: PogoPowerups_Poll's timer */
#define D_800C52EA ((&D_800C52E8)[1])
extern s32 D_800C38B8[];
extern s32 D_800C38C8[];
extern s32 D_800C52F8;
extern s32 D_800C52FC;
extern s32 D_800C5304;
extern s32 D_800C5BAC;
extern s32 D_800C5BB8;
extern u16 D_800C3998[][2];
extern s32 D_800C5BBC;
extern u16 D_800C3A38[][2];
extern s32 D_800C5074[];
extern s32 D_800C5050[];
extern s32 D_800C502C[];
/* HeroPogo's per-hero frame tables */
extern s32 D_800C4758[];
extern s32 D_800C4878[];
extern s32 D_800C49DC[];
extern s32 D_800C4AB0[];
extern s32 D_800C4BA0[];
extern s32 D_800C4CF4[];
extern s32 D_800C4E40[];
extern s32 D_800C4F3C[];
extern s32 D_800C5300;
extern s32 D_800C5BC0;
extern s32 D_800C5BC4;
extern s32 D_800C5BC8;
extern s32 D_800C5BCC;
extern s32 D_800C5BD0;
extern s32 D_800C5BD4;
extern s32 D_800C5B2C[8];
extern s32 D_800C5B4C[8];
extern s32 D_800C5B6C[8];
extern s32 D_800C5B8C[8];
extern u8 D_800C3C14[][4][8];
extern s32 D_800C3B14[];
extern s32 D_800C3B94[];
extern s16 D_800C4014[][2];
extern s32 D_800C4084[][4];
extern s32 D_800C4284[][4];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ovl60Move; /* size 0x14 */

typedef struct {
    s32 x;
    s32 z;
} Ovl60Offset;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
} Ovl60Goal;

extern Ovl60Move D_800C5ADC[];
extern Ovl60Offset D_800C457C[];
extern Ovl60Offset D_800C45BC[];
s32 PogoAI_CountMyTiles(u32 arg0);
s32 func_800BE6C4(s32 arg0, Ovl60Goal *arg1);
void func_800C1068();

void func_800C10F8(void);




void HeroPogoTazingCancel(u32 arg0);
extern s16 D_8009D87C[][3];

void func_800B76CC(s32 arg0);

void func_800BC0EC(s32 arg0);

void func_800BD008(s32 arg0);





s32 PogoAI_CountMyTiles(u32 arg0);


void func_800C0FAC(s32 i);








void func_800C2294(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C22E8(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

void func_800C2338(s32 arg0, s32 arg1, s16 arg2, s16 arg3);














void PogoPowerups_Poll(void);
void func_800BCB4C();
extern s32 D_800C52F4;
extern s32 D_80099178;
extern s32 D_800C5BD8;
extern s32 D_800C5BDC;
extern s32 D_800C5BE0;
extern s32 D_800C5BE4;
extern s32 D_800C5BE8;
extern s32 D_800C5BEC;

/* D_8005A614+0x34 as UpdatePOGO switches on it: an enum, so gcc's
 * switch tree skips its text cost table. */
typedef enum Ovl60Mode {
    OVL60_MODE_NONE = -1,
    OVL60_MODE_10 = 10,
    OVL60_MODE_11 = 11,
    OVL60_MODE_12 = 12
} Ovl60Mode;
s32 func_8008502C();
void InitPogo_Tiles(void);








void PogoWinner_Relic(s32 arg0, u16 arg1);
void PogoWinner_Normal(s32 arg0, u16 arg1);






s32 func_800C0F08(s32 arg0, EXTbotai *ai);


s32 func_800C178C(void);














void func_800C3048(void);

void PogoInit_Camera(void);

s32 func_800BC04C(s32 arg0, s32 arg1);



s32 func_800BE864(s32 hero, Ovl60Goal *goal);

/* D_8005A614 as seen by PogoInit_RelicChallenge. */
typedef struct Ovl60ScoreView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0x5C - 0x36];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 unk60[0x6C - 0x60];
    /* 0x6C */ s32 teams;
} Ovl60ScoreView;
#define SCORE60V ((Ovl60ScoreView *)D_8005A614)

/* D_8005A614 as seen by PogoWinner_Normal (the final ranking). */
typedef struct Ovl60RankView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team[2];
    /* 0x4C */ u8 unk4C[0x60 - 0x4C];
    /* 0x60 */ u8 winner[2];
    /* 0x62 */ u8 unk62[0x6C - 0x62];
    /* 0x6C */ s32 teams;
    /* 0x70 */ u8 unk70[0xA7 - 0x70];
    /* 0xA7 */ u8 order[4];
} Ovl60RankView;
#define RANK60V ((Ovl60RankView *)D_8005A614)

/* func_800B3FF0: per-mode entries picked by teams, with two values at 0xE/0x10. */
typedef struct Ovl60ModeEntry {
    /* 0x00 */ u8 unk0[0xE];
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 unk12[2];
} Ovl60ModeEntry;
extern Ovl60ModeEntry D_800C36B4;
extern Ovl60ModeEntry D_800C36C8;
extern Ovl60ModeEntry D_800C3760;
extern Ovl60ModeEntry D_800C3774;
extern Ovl60ModeEntry D_800C37F8;
extern Ovl60ModeEntry D_800C380C;
extern Ovl60ModeEntry D_800C3890;
extern Ovl60ModeEntry D_800C38A4;
extern u16 D_800C3678[];
extern u16 D_800C3724[];
extern u16 D_800C37BC[];
extern u16 D_800C3854[];
/* D_8005A614 as seen by func_800B3FF0. */
typedef struct Ovl60ModeView {
    /* 0x00 */ u8 unk0[0x32];
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[2];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ u8 unk3A[0x5C - 0x3A];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 unk60[0x6C - 0x60];
    /* 0x6C */ s32 teams;
} Ovl60ModeView;
extern u16 D_8009917C;
/* D_8005A614 as seen by PogoWinner_Relic (team end of round). */
typedef struct Ovl60TeamView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32[0x48 - 0x32];
    /* 0x48 */ s16 team0;
    /* 0x4A */ s16 team1;
    /* 0x4C */ u8 unk4C[0x5C - 0x4C];
    /* 0x5C */ u8 out[4];
    /* 0x60 */ u8 winner;
    /* 0x61 */ u8 unk61[0x6C - 0x61];
    /* 0x6C */ s32 teams;
} Ovl60TeamView;
/* PogoWinner_Relic: hero idx's team is out of points (i starts at i0). */
#define OVL60_TEAM_LOST(g, idx, i, i0)                                   \
    {                                                                    \
        Ovl60TeamView *g_;                                               \
                                                                         \
        i = (i0);                                                        \
        g_ = (g);                                                        \
        g_->winner = Heroes[idx].hero_name;                              \
        g_->unk30 |= 0x800;                                              \
        D_8009917C |= 0x200;                                             \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                func_80026980(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x200;                          \
            }                                                            \
        }                                                                \
    }
/* PogoWinner_Relic: OVL60_TEAM_LOST, g copied for the loop after the stores. */
#define OVL60_TEAM_LOST_LATE(g, idx, i, i0)                              \
    {                                                                    \
        Ovl60TeamView *g_;                                               \
                                                                         \
        i = (i0);                                                        \
        (g)->winner = Heroes[idx].hero_name;                             \
        (g)->unk30 |= 0x800;                                             \
        D_8009917C |= 0x200;                                             \
        g_ = (g);                                                        \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                func_80026980(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x200;                          \
            }                                                            \
        }                                                                \
    }
/* PogoWinner_Relic: the round ends with no winner. */
#define OVL60_ROUND_OVER(g, flag, i)                                     \
    do {                                                                 \
        Ovl60TeamView *g_;                                               \
                                                                         \
        i = 0;                                                           \
        g_ = (g);                                                        \
        g_->winner = 0;                                                  \
        g_->unk30 |= (flag);                                             \
        D_8009917C |= 0x200;                                             \
        for (; i < 4; i++) {                                             \
            if (((u8 *)g_)[i + 0x5C] != 0) {                             \
                Heroes[i].hero_status |= 0x400;                          \
                func_80026980(Heroes[i].hero);                           \
            } else {                                                     \
                Heroes[i].hero_status |= 0x100;                          \
            }                                                            \
        }                                                                \
    } while (0)
/* PogoWinner_Relic: a hero's score plus its teammate's. */
static inline s32 Ovl60TeamScore(s32 idx) {
    HeroInfo *hb = Heroes;
    HeroInfo *h = &hb[idx];

    return h->hero_score + hb[h->hero_teammate].hero_score;
}
/* func_800B3534: the engine's per-mode table pointers. */
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern Ovl60ModeEntry D_800C36DC;
extern Ovl60ModeEntry D_800C3644;
extern Ovl60ModeEntry D_800C3658;
extern Ovl60ModeEntry D_800C366C;
extern Ovl60ModeEntry D_800C3684;
extern Ovl60ModeEntry D_800C369C;
extern Ovl60ModeEntry D_800C36F0;
extern Ovl60ModeEntry D_800C3704;
extern Ovl60ModeEntry D_800C3718;
extern Ovl60ModeEntry D_800C3730;
extern Ovl60ModeEntry D_800C3748;
extern Ovl60ModeEntry D_800C3788;
extern Ovl60ModeEntry D_800C379C;
extern Ovl60ModeEntry D_800C37B0;
extern Ovl60ModeEntry D_800C37C8;
extern Ovl60ModeEntry D_800C37E0;
extern Ovl60ModeEntry D_800C3820;
extern Ovl60ModeEntry D_800C3834;
extern Ovl60ModeEntry D_800C3848;
extern Ovl60ModeEntry D_800C3860;
extern Ovl60ModeEntry D_800C3878;
void func_8007B7FC(u32 a, u32 b);
void func_8007D144(void);
extern s32 D_80099174;


void PogoInit_RelicChallenge(void);

void func_800B3FF0(void);





void Tile_Claim(s32 arg0, s32 arg1, s32 arg2);






void func_800BB724(s32 arg0, s32 arg1);

s32 func_800BBC94(s32 x, s32 z);

void Fire_Rocket(s32 arg0, s32 arg1);



void func_800BD790(s32 hero);


bool PogoAI_IsSquareOKForPlayer(u32 arg0, s32 x, s32 z, s32 dx, s32 dz, u32 flags);

s32 func_800BDE24(s32 arg0);

void func_800BFFF4(EXTengine *eng);

bool PogoAI_IsSquareOKForPlayer(u32 arg0, s32 x, s32 z, s32 dx, s32 dz, u32 flags);

/* HeroPogoDazed: a status bit sends the hero to an action and seq. */
static inline s32 Ovl60_StatusAction(s32 arg0, s32 bit, s32 action, s32 seq)
{
    if (Heroes[arg0].hero_status & bit) {
        if (D_800C535C[arg0].nas != NULL) {
            HeroPogoTazingCancel(arg0);
        }
        SetHeroAction((HeroInfo *)arg0, action);
        SetHeroSeq((HeroInfo *)arg0, seq);
        return 1;
    }
    return 0;
}

void HeroPogo(u32 arg0);

void HeroPogoDazed(u32 arg0);


#endif /* BASH_OVL60_H */
