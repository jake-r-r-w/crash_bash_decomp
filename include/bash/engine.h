#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Functions and data defined in the engine overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/engine.c */

#include "common.h"
#include "bash/main.h"
#include "bash/nas.h"

extern s32 D_80051658;
extern s32 D_800AD2F8[];
extern u16 D_800A00D0[];
extern s32 D_800AD320;
extern s32 D_800AD310[];
extern s32 D_800AD328;
typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} D_800AD1EC_entry;
extern s32 D_800AD1E8;
extern D_800AD1EC_entry D_800AD1EC[];
extern void (*D_8005AA80)(void);

void func_8007A688(s32 arg0);
void func_8007A6B0(s32 arg0);
void func_8007A6D0(s32 arg0);
void func_8007A6F8(void);
void func_8007A7C0(void);
void func_8007A9D0(u8 *arg0);
void func_8007AAF0(void);
void func_8007AB20(void);
void func_8007AB6C(void);
void func_8007AB8C(void);
void func_8007ABC4(void);
void func_8007E7A4(void);
s32 func_8007E7C8(void);
void func_8007E874(void);
void func_8007EB04(void);
void func_80086704(void);
s32 func_8008670C(void);
s32 func_80086714(void);
s32 func_8008671C(void);
s32 func_80086724(void);
void func_800867EC(void);
void func_8008680C(void);
void func_80086814(void);
void func_8008AA90(void);
s32 func_8008AB48(void);
void func_8008AD9C(void);
void func_8008CE10(void);
void func_8008CE18(void);
s32 func_8008CEB0(u8 *arg0, u8 *arg1);
void PlotParticleEffect(void);
void InitParticleEffect(void);
void InitFloatyScore(u8 *arg0);
void PlotBigPoof(NAS *nas);
s32 func_8008DE8C(void);
void func_8008EB3C(void);
void func_8008EB8C(void);
void func_8008FC58(void);
void func_80090048(void);
s32 func_800923F0(s32 *arg0);
void func_800923FC(void);
void func_80092404(void);
void func_80092B7C(void);
void func_80092BA0(void);
void func_80092C54(void);
void func_80092E40(void);
void func_80094058(void);
void func_8009407C(void);
void func_80094120(void);
void func_8009414C(void);
void func_800941BC(void);
void func_800941EC(void);
void func_80094D88(s32 arg0, s32 arg1);
s32 func_80094F30(void);
void func_80094F60(void);
void func_80094FA8(void);
void func_80094FE0(void);
void func_80095A04(s32 arg0);
void func_800952F8(subengine *sub, s32 *recs);
void func_80095BEC(s32 arg0, s32 *recs);
void func_80091E58(void);
extern s32 D_8009EF64;
void func_80090310(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80090564(u8 *arg0, s32 arg1);
extern s32 D_8009F008;
extern s32 D_8009F3B8;
extern subengine D_800A13F4[][32]; /* 32 subengines per EnginePool slot */
EXTengine *GetEngineSlot(s32 arg0);
s32 func_800976A8(u8 *arg0, s32 arg1);
void func_80098278(void);

typedef struct { u16 a, b; } U16Pair;
typedef struct { u16 a, b, c, d; } U16Quad;
extern U16Quad D_8009AD74[4];
extern U16Pair D_80099270[4];

extern s32 D_800ADBC0;
extern s32 D_80099174;
extern s32 D_80099178;
extern s16 D_8009917C;
extern s32 D_800996D4[3];
extern s16 D_8009AE76;
extern s16 D_80099194;
extern s32 D_80099700[];
extern s32 D_8009AE78;
extern u8 D_8001C7FC[];
typedef struct EngineBox {
    s32 unk0;
    u8 pad4[0x50];
    void *unk54;
    u8 pad58[0x10];
    s16 unk68, pad6A, unk6C, unk6E, unk70, pad72, unk74, unk76, unk78, pad7A, unk7C, unk7E, unk80, pad82, unk84, unk86, unk88, pad8A;
    s32 unk8C, unk90, unk94, unk98;
} EngineBox;
extern s16 D_8009AE70;
extern s32 D_8009AE3C;
extern u16 D_8009AE74;
extern s32 D_8009F708;
extern u8 D_8009F658[];
extern u8 D_8009FA14[];
extern u8 D_800A015C[];
extern u8 D_8001C448[];
extern EXTengine EnginePool[];
extern HeroInfo Heroes[4];
typedef struct HeroActionEntry {
    void (*fn)(void);
    s32 pad;
} HeroActionEntry;
extern HeroActionEntry HeroActionTable[];
extern hero_seq **HeroSeqTables[]; /* by HeroInfo.hero_name (1-8): each character's sequences, by id */
extern s32 D_8009BD04;
extern s16 D_8009D6DC[];
extern s32 D_800A02C0;
extern s32 D_800A02C4;
extern u8 D_800A0224[];
extern void *D_800A0D2C;
extern u8 D_8009F8A4;
extern void (*D_8009F8B4)(void);
void func_80082590(void);
extern u8 D_8009F8BC[];
extern u8 D_800A016C[];
extern s32 D_800A0158;
extern s8 D_80064C93;
extern Blk32 D_8009F9D4;
extern CamVec3 D_8009F9F4;
void func_80095258(u8 *arg0);
extern ASTEntry EngineASTTable[];
extern u8 D_8009F644[];
extern u8 D_800A00DC[];
extern s32 D_8005A624;
extern u8 D_8009BCE0[];
extern EXTengine D_8009959C;
extern u8 D_80052F5C[];
extern ScriptInit D_8009E250;
extern GfxViewport D_8009F6A4;
extern GfxViewport D_800A0BCC;
extern GfxCamera D_8009F7EC;
extern s32 D_800A0C08;
extern GfxViewport D_8009F3BC;
extern GfxViewport D_8009F3E4;
extern GfxCamera D_8009F4C0;
extern GfxCamera D_8009F578;
extern s32 D_8009F630;
extern s32 D_8009F634;
extern s32 D_8009F638;
extern s32 D_8009F640;
void func_80090A10(void);
void func_80090E20(void);

/* func_800936C4's level setup config (D_8005A5E4, or D_8009F96C if unset). */
typedef struct EngineCfg {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 pools[3];       /* func_80017164 */
    /* 0x18 */ VramArea *areas;    /* func_80028774 */
    /* 0x1C */ FreeNode *pool;
    /* 0x20 */ s32 npool;
    /* 0x24 */ s32 nbuf;
} EngineCfg;

s32 func_8009317C(void);
extern EngineCfg D_8009F96C;
extern u8 D_800D7144[];
extern ArenaDef D_80098F48;
extern u8 D_800B9524[];
extern u8 D_8005B72C[];
extern u8 D_8005B744[];
extern s32 D_800A0D3C;
extern u8 D_8009F67C[];
extern u8 D_8009F734[];
extern u8 D_8009F46C[];
extern s32 *D_8009F65C;
extern void *D_80098718;
extern s16 D_8009871C;
extern u8 D_800A0BF4[];
extern s32 D_800A0C0C;
extern s32 D_800A0D30;
extern s32 D_800A0C70;
/* func_8009440C's entries (D_800A0C10, 0xC bytes) and the table they index (0x14 bytes). */
typedef struct EngineMarker {
    /* 0x0 */ u16 unk0;
    /* 0x4 */ s32 idx;
    /* 0x8 */ void *unk8;
} EngineMarker;
typedef struct EngineMarkerPos {
    /* 0x00 */ s32 x, y, z;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 unkE[6];
} EngineMarkerPos;
extern EngineMarker D_800A0C10[];
extern EngineMarkerPos D_800A02D8[];
extern subengine D_800A0C74;
extern s32 D_800A0D1C;
extern s32 D_800A0D20[]; /* arrays: their stores must alias struct loads (gcc 2.7 sched) */
extern s32 D_800A0D24[];
extern s32 D_800A0D28;
extern s32 D_800A0D38;
extern u8 D_8009E498[];
/* D_8005A614 as seen by func_80092CAC. */
typedef struct EngineGameView {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0xE0 - 0x36];
    /* 0xE0 */ s32 unkE0;
} EngineGameView;
extern u8 D_8009F6F4[];
extern s32 D_8009F8B8;
typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; } EngineEntry16Head;
typedef struct { s32 unk0; EngineEntry16Head *unk4; s32 unk8; } EngineTable12;
extern EngineTable12 D_8009E7A0[];
/* Also in main.h; repeated for overlays that include engine.h first. */
extern u8 D_8005A614[];
extern u16 D_8004DDD0[];
void func_8001E8F0(void *arg0);
void func_80079260(void);
s32 func_8007A5E8(u8 *arg0);
void func_800826F8(s32 arg0);
void func_8009608C(s32 arg0);
void func_80098570(void);
void func_800B478C(void);

/* func_80092CAC's tail: resets state, then starts the mode by its D_8004DDD0 world. */
static inline void EngineBeginMode(s32 arg) {
    EngineGameView *g = (EngineGameView *)D_8005A614;
    s32 mode;
    u16 world;

    D_8009F6F4[0x24] = 0;
    D_8009F6F4[0x25] = 0;
    D_8009F6F4[0x26] = 0;
    D_8009F6F4[0x27] = 0;
    g->unk30 &= 0xFDFF;
    func_8009608C(arg);
    func_8001E8F0(&D_8009F8A4);
    func_80098570();
    mode = g->mode;
    world = D_8004DDD0[mode * 3];
    if (world != 0x101) {
        if (world == 0x102) {
            if (mode == 0x25) {
                func_800B478C();
            }
        } else {
            g->unk30 |= 0x1000;
            func_800826F8(mode);
        }
    }
}
extern s32 D_800AEC5C;
extern s32 D_8009D868;
extern s32 D_8009D86C;
extern s32 D_8009D874;
extern s32 D_8009D878;
extern u8 D_8009F9AC[];
extern s32 D_8009E5A4;
extern s32 D_8009E5A8;
extern s32 D_8009E5B0;
extern s32 D_8009E5B4;
extern s32 D_800AEC58;
extern u8 D_8009E7F4[];
extern u8 D_8009E8B4[];
extern u8 D_8009EB3C[];
extern s32 D_8009EB98;
typedef struct { u16 a, b, c, flags; } EngEntry;
typedef struct { s32 unk0; EngEntry *entries; s32 unk8, unkC, unk10; } EngTable;
typedef struct { u16 unk0; s32 unk4; s32 unk8; } EngState;
extern EngState D_8009EE9C;
extern s32 D_8009E5DC;
extern u8 D_8005A676;
extern s32 D_8009EDD4[][5];
extern u16 D_800986FC[];
typedef struct { u8 pad0[2]; u16 unk2; u16 unk4; } EngSub90248;
typedef struct { u8 pad0[0x14]; EngSub90248 *unk14; u8 pad18[0x10]; } EngRec90248;
extern s8 D_8005A677;
extern EngRec90248 D_800BCCF4[];

void func_8007A650(s32 arg0);

typedef struct {
    u8 unk0;
    u8 unk1;
} D_8009D90C_t;
extern D_8009D90C_t D_8009D90C[400];

int func_8007AC00(void);

void SetHeroSeq(HeroInfo *hero, s32 seq);

void SetHeroAction(HeroInfo *hero, s32 action);

void func_80086FA0(void);

void func_8008ECE4(void);

s32 func_8008EF70(void);

void func_8008EF9C(void);

void func_8008F068(void);

void func_80092F9C(void);

void func_800940B0(s32 arg0, s32 arg1);

void func_8009421C(void);

s32 func_80095D88(s32 *rec);

void func_800972BC(s32 arg0, s32 arg1);
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} D_800AD2EC_t;

extern s32 D_80051658;
extern D_800AD2EC_t D_800AD2EC;
extern s32 D_800AD31C;
extern D_800AD2EC_t D_800AD304;
extern s32 D_800AD324;

void func_8007E7F4(void);

u8 *func_8008D24C(u8 *arg0, s32 *arg1);

void func_8008EC94(void);

s32 func_80092560(u8 *arg0);

void func_80097334(s32 arg0, s32 arg1);

/* PadState (D_80051338) seen by func_8007EE2C: button flags at 0x44/0x48. */
typedef struct PadButtons {
    /* 0x00 */ u8 unk0[0x44];
    /* 0x44 */ s32 held;
    /* 0x48 */ s32 pressed;
    /* 0x4C */ u8 unk4C[0x54 - 0x4C];
    /* 0x54 */ s32 unk54;
} PadButtons;

extern s32 D_8009AEEC;
extern s32 D_8009AEF0;
void func_8007A614(s32 arg0);

s32 func_8007EE2C(s32 pad);

extern void (*D_8009D73C)();
extern s32 (*D_8009D728)(s32);
extern s32 (*D_8009D72C)(s32);
s32 func_80086FCC(s32 arg0);

void func_80086F78(void);

void func_8008A8F0(s32 arg0);

u8 *func_8008D284(u8 *arg0, u8 *arg1);

void func_8009029C(void);
/* func_800945C0's argument. */
typedef struct {
    void *unk0; /* -> D_800A0D2C */
    s32 unk4;   /* -> D_800A0D1C */
    s32 unk8;   /* frame rate scale */
} EngineStartArgs;
s32 func_8007E894(void);

void func_800831C4(s32 arg0);

void func_8008352C(void);

void func_80086868(void);

void func_80086F4C(void);

void func_8008EAC8(void);

s32 func_8009020C(s32 arg0);

void func_80092CAC(void);

void func_800936C4(void);

void func_800945C0(EngineStartArgs *arg0);

typedef struct Vec3W {
    s32 a, b, c;
} Vec3W;

s32 func_800866DC(s32 *arg0);

s32 func_8008DE94(s32 arg0);

s32 func_80092598(u8 *arg0);

void func_8009342C(void);

void func_80095020(void);
/* 32-byte records of D_8005A5E8, indexed by D_8005A61C (func_8008688C). */
typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u8 unkE[0x12];
} EngineRec32;
extern EngineRec32 *D_8005A5E8;
extern s32 D_8005A61C;
extern u16 D_800AD12C;

void func_800834E4(void);
typedef struct EngineRec24 {
    s32 kind;
    s16 val;
    char pad[0x1E];
} EngineRec24;
extern EngineRec24 *D_8009BD0C;
extern EngineRec24 **D_8009BD10;
extern s32 D_8009BD14;

s16 func_800831F0(void);

void func_8008688C(void);

s32 func_8008AB04(s32 arg0);

s32 func_800925C4(u8 *arg0);

void func_80092C78(void);

void func_80093110(void);

void func_800943B8(void);

void func_8009440C(void);

s32 func_80095F48(s32 *p, s32 a1, s32 a2, s32 a3, s32 a4);

/* Engine callback table filled by func_80083550 (0x54 is D_8009D73C). */
typedef struct EngineHooks {
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
} EngineHooks;
extern EngineHooks D_8009D6E8;
extern u8 D_8009D2E4[];
extern u8 D_8009D2F8[];
extern u8 D_8009D318[];
extern u8 D_8009D354[];
extern u8 D_8009D894[];
extern u8 D_8009D8A8[];
extern u8 D_8009D8BC[];
extern u8 D_8009D8D0[];
extern u8 D_8009D8DC[];
extern void *D_8009D8F4;
extern void *D_8009D8F8;
extern void *D_8009D8FC;
extern void *D_8009D900;
extern void *D_8009D904;
extern s32 D_8009D908;
extern s16 D_8009E0D8;
extern s16 D_8009E0DA;
extern s16 D_8009E0DC;
extern s16 D_8009E0DE;
extern s16 D_8009E0E0;
extern s16 D_8009E0E2;
extern s16 D_8009E0E4;
extern s16 D_8009E0E6;
void func_8007AB28(void);

void func_80084788();
void func_800859D4();
void func_80085C84();
void func_80085D2C();
void func_80088BAC(s32 arg0);

void func_800894E0(s32 arg0);

void func_8008A9F0();
void func_8008AA40();
void func_80083550(void);

s32 func_80090008(void);

s32 *func_800901B4(s32 arg0);

void func_800917B4(void);

s32 func_8009251C(u8 *arg0);

void func_80092B40(void);

void func_80094DD0(void);

void func_80098428(void);
/* Object wobbled by func_80093270 (D_800A0290); unk5C is a linked one. */
typedef struct WobbleObj {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x51 - 0x16];
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad54[0x5C - 0x54];
    /* 0x5C */ struct WobbleObj *unk5C;
} WobbleObj;
extern WobbleObj *D_800A0290;

void func_8007D034(void);

void func_8007D0BC(void);

void func_8007EAC0(void);

void func_80082590(void);

void func_8008A810(void);

void func_80093270(void);

extern U16Pair D_80099280[4];
extern s16 D_8005A650;
extern s32 D_800A00F0[];

void func_8007A8C4(void);

void func_8007E8D0(EngineBox *p, s16 x, s16 y, s32 a, s32 b);

void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2);

s32 func_80083488(void);

s32 func_8008AB50(s32 arg0);

void func_8008FD40(void *arg0);

s32 func_80090248(void);

void func_80092AA8(GameModeDef *arg0);

void func_80092AF4(s32 arg0, s32 arg1);

void func_80092E94(void);

void func_8009400C(void);

void func_80094364(void);
extern u8 D_800C45DC[];

void func_80086D88(void);

void func_8008A99C(s32 arg0);

void func_8008AAB0(s32 arg0);

s32 func_8008AB94(s32 arg0);

s32 func_8008C4C8(s32 *arg0, s32 arg1);

void InitWeatherNas(u8 *arg0);

extern u8 D_8009E200[];

void func_80083244(void);

void func_80088544(s32 arg0);

s32 func_8008ABF8(s32 arg0);

void PlotWeatherNas(u8 *arg0);

void func_8008D088(u8 *arg0, u8 *arg1, EmitPart *arg2, s32 *arg3, u8 *arg4);

void func_8008D488(u8 *arg0, s32 arg1);

void InitBigPoof(NAS *nas);

void func_8008FD94(void);

void func_8008FE00(void);

s32 func_8008FF94(void);

void func_80092198(void);

void func_80092BDC(void);

void func_80092FB8(void);

void func_80094EB8(void);

s32 func_800950FC(void);

void func_80095160(EngineStartArgs *arg0);

void func_80097244(u8 *p);

void func_800973AC(s32 arg0, s32 arg1);

void func_80097424(s32 arg0, s32 arg1);

void func_8009749C(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5);

void func_80098284(void);

void func_800982E0(void);

void func_80098464(void);

#endif /* BASH_ENGINE_H */
