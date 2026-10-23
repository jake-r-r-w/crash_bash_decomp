#ifndef BASH_ENGINE_H
#define BASH_ENGINE_H

/* Functions and data defined in the engine overlay. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * The engine is several files; this one owns engine.c's and includes the others' headers.
 * owner: src/overlays/engine/engine.c */

#include "common.h"
#include "bash/main.h"
#include "bash/nas.h"
#include "bash/shell.h"   /* ClockInfo, ScoreInfo */

extern s32 D_80051658;
extern s32 D_800AD2F8[];
extern u16 D_800A00D0[];
extern s32 D_800AD320;
extern s32 camera_frame_right[];
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
extern s32 camera_frame_list_size;
extern D_800AD1EC_entry D_800AD1EC[];
extern void (*D_8005AA80)(void);
extern void (*D_8009D704)(void);
extern void (*D_8005AA74)(void);
extern s32 D_8009E29C;
extern s32 D_8009E298;

void func_8007A688(s32 arg0);
void func_8007A6B0(s32 arg0);
void func_8007A6D0(s32 arg0);
void func_8007A6F8(void);
void func_8007A7C0(void);
void InitAmbience(u8 *arg0);
void func_8007AAF0(void);
void func_8007AB20(void);
void func_8007AB6C(void);
void func_8007AB8C(void);
void func_8007ABC4(void);
extern s32 D_8009EF64;
extern s32 D_8009F008;
extern s32 D_8009F3B8;
extern s32 D_8009F4BC;
extern s32 D_8009F494;
extern s32 D_8009F40C;
extern s32 D_8009EEB0;
extern u8 D_800555DC[];
extern u8 D_800558EC[];
extern subengine D_800A13F4[][32]; /* 32 subengines per EnginePool slot */

typedef struct { u16 a, b; } U16Pair;
extern ScoreInfo Score[4];
extern SWORD_XY ScoreIcon3Pos[4];
extern SWORD_XY ScoreIcon2Pos[4];

extern s32 D_800ADBC0;
extern s32 PanelCtrl;
typedef struct EngObj {
    s32 flags;
    u8 pad[0x48];
    void (*update)(struct EngObj *);
} EngObj;
extern EngObj **D_8005AA40;
extern void (*D_8005AA7C)(void);
extern subengine D_8009A04C[];
extern struct { s32 *c; u8 pad[4]; s32 *a; s32 *b; } Icon;
extern s16 D_80099208[];
extern s32 D_8005A680[];
extern subengine D_8009A4E4;
extern u32 Clock_Time;
extern u16 Clock_flags;
extern U16Pair Clock_offsets[];
extern ClockInfo Clock[4];
extern s32 Clock_XYZ[3];
extern u16 Clock_last_ping;
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
extern u16 CupMode;
extern s32 CupTimer;
extern u16 Trophy_Cups;
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
extern u8 D_8009F8BC[];
extern u8 D_800A016C[];
extern s32 D_800A0158;
extern s8 D_80064C93;
extern Blk32 D_8009F9D4;
extern CamVec3 D_8009F9F4;
extern ASTEntry EngineASTTable[];
extern u8 D_8009F644[];
extern s32 D_800ADBC4;
extern u8 D_800A00DC[];
extern s32 D_8005A624;
extern u8 D_8009BCE0[];
extern EXTengine D_8009959C;
extern u8 D_80052F5C[];
extern ScriptInit D_8009E250;
extern ScriptInit D_8009E1E8;
extern u8 D_800546C4[];
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

/* func_800936C4's level setup config (D_8005A5E4, or D_8009F96C if unset). */
typedef struct EngineCfg {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 pools[3];       /* func_80017164 */
    /* 0x18 */ VramArea *areas;    /* func_80028774 */
    /* 0x1C */ FreeNode *pool;
    /* 0x20 */ s32 npool;
    /* 0x24 */ s32 nbuf;
} EngineCfg;

extern EngineCfg D_8009F96C;
extern EngineCfg D_800A0314;
extern s32 D_8004E0D4;
extern s32 D_8009F654;
extern s16 core_process;
extern u8 *D_80098594;
extern s32 D_8009F968;
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
extern u16 D_8009871C;
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
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8[0x28];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 flags;
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 unk36[0xE0 - 0x36];
    /* 0xE0 */ s32 unkE0;
} EngineGameView;
extern u8 D_8009F6F4[];
typedef struct { s32 key; s32 v[3]; } EngineKeyed16;
extern EngineKeyed16 D_800A0B60[];
extern s32 D_8009F8B8;
typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; } EngineEntry16Head;
typedef struct { s32 unk0; EngineEntry16Head *unk4; s32 unk8; } EngineTable12;
extern EngineTable12 D_8009E7A0[];
/* Also in main.h; repeated for overlays that include engine.h first. */
extern u8 D_8005A614[];
extern s32 func_800B7EE4(s32, u8 *);
extern u8 D_800657D0[];
extern u16 D_8004DDD0[];
void process_init(void *arg0);
void func_80079260(void);
s32 func_8007A5E8(u8 *arg0);
void func_800B478C(void);

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
typedef struct { u16 unk0; s32 unk4; s32 unk8; s32 unkC; s32 unk10; } EngState;
extern EngState D_8009EE9C;
extern s32 D_8009E5DC;
extern u8 D_8005A676;
extern s32 D_8009EDD4[][5];
extern u16 D_800986FC[];
extern u16 D_800986BC[];
typedef struct { u16 unk0; u16 unk2; u16 unk4; u16 unk6; } EngSub90248;
typedef struct { s32 *unk0; s32 *unk4; s32 unk8; s32 unkC; s32 unk10; EngSub90248 *unk14; u8 *unk18; s32 unk1C; s32 unk20; s32 unk24; } EngRec90248;
extern s8 D_8005A677;
extern EngRec90248 D_800BCCF4[];

void func_8007A650(s32 arg0);

typedef struct {
    u8 unk0;
    u8 unk1;
} D_8009D90C_t;
extern D_8009D90C_t D_8009D90C[400];

int func_8007AC00(void);

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

/* PadState (D_80051338) seen by func_8007EE2C: button flags at 0x44/0x48. */
typedef struct PadButtons {
    /* 0x00 */ u8 unk0[0x44];
    /* 0x44 */ s32 held;
    /* 0x48 */ s32 pressed;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
} PadButtons;

extern s32 D_8009AEEC;
extern s32 D_8009AEF0;
void func_8007A614(s32 arg0);

extern void (*D_8009D718)();
extern void (*D_8009D73C)();
extern s32 (*D_8009D728)(s32);
extern s32 (*D_8009D72C)(s32);

/* An entry of the list D_800A0D1C walks (func_80094860), 12 bytes. */
typedef struct {
    s32 vab;   /* machine_load_sfx's first argument; 0 ends the list */
    s32 sfx;   /* PlaySfx id */
    s32 unk8;  /* -> D_800654B0 */
} EngineVabCue;

/* func_800945C0's argument. */
typedef struct {
    void *unk0; /* -> D_800A0D2C */
    s32 unk4;   /* -> D_800A0D1C */
    s32 unk8;   /* frame rate scale */
} EngineStartArgs;

typedef struct Vec3W {
    s32 a, b, c;
} Vec3W;

/* 32-byte records of D_8005A5E8, indexed by D_8005A61C (func_8008688C). */
typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s32 unk4; /* passed to func_800129D0 by func_80093854 */
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u8 unkE[0x12];
} EngineRec32;
extern EngineRec32 *D_8005A5E8;
typedef struct { s32 unk0; EngineRec32 *unk4; } EngineBootSrc;
typedef struct {
    /* 0x00 */ void (*unk0)();
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 unkC[0x20];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 unk30[0xC];
    /* 0x3C */ u16 unk3C;
} EngineBootView;
extern u8 *D_8005AA20;
extern u8 *D_8005AA44;
extern s32 D_8005AAB0;
extern s32 D_800986A4[];
extern s32 D_8005A61C;
extern u16 D_800AD12C;

typedef struct EngineRec24 {
    s32 kind;
    s16 val;
    char pad[0x1E];
} EngineRec24;
extern EngineRec24 *D_8009BD0C;
extern EngineRec24 **D_8009BD10;
extern s32 D_8009BD14;

/* Hero jump parameters, 8 bytes per hero (EngineHooks.unk10). */
typedef struct HeroJumpEnt {
    u16 vel, max, acc, dec;
} HeroJumpEnt;

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

extern SWORD_XY ScoreIcon1Pos[4];
extern s16 D_8005A650;
extern s32 map[];

void func_8007A8C4(void);

extern u8 D_800C45DC[];

extern u8 D_8009E200[];

extern s32 D_8009E424[];
extern u8 D_80052F94[];
NAS *func_8007A524(s32 arg0, s32 *arg1, s32 arg2);

void func_8007A718(void);

extern s16 D_8006BBD6[];

NAS *func_8007A464(s32 arg0, s32 *arg1, s32 arg2);

void DoAmbience(void);

/* The engine's other files (ARCHITECTURE.md, Files inside the engine). */
#include "bash/panel.h"
#include "bash/pause.h"
#include "bash/80083708.h"
#include "bash/8008e27c.h"
#include "bash/8009317c.h"
#include "bash/8009608c.h"

/* Needs prototypes from the unit headers above. */
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
    process_init(&D_8009F8A4);
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

void func_8007A808(void);

void func_8007AA18(void);

/* D_8004DEE0 rows (10 bytes) and the object func_8007A164 totals over. */
typedef struct { u16 min0, min2, min4, min6, off; } EngineReq;
extern EngineReq D_8004DEE0[];
typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0xE4 - 8];
    u8 a[0x21];
    u8 pad105[0x105 - 0xE4 - 0x21];
    u8 b[0x21];
    u8 pad126[0x126 - 0x105 - 0x21];
    u8 c[0x21];
    u8 pad147[0x147 - 0x126 - 0x21];
    u8 d[0x21];
    u8 pad168[0x168 - 0x147 - 0x21];
    u8 e[1];
} EngineReqObj;

void InitialiseNasSystem(void);

void func_8007906C(void);

void func_800793E8(void);

s32 GravitateNas(NAS *nas);

/* D_8005A614 as seen by func_80079554. */
typedef struct EngMenuGame {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x26 - 0x14];
    /* 0x26 */ u8 count;
    /* 0x27 */ u8 pad27[0x34 - 0x27];
    /* 0x34 */ s16 mode;
    /* 0x36 */ u8 pad36[0x68 - 0x36];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 pad6C[0x74 - 0x6C];
    /* 0x74 */ u8 unk74;
} EngMenuGame;
/* 0x18-byte entries at D_8005A3EC (func_80079554). */
typedef struct EngMenuItem {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0x18 - 0x08];
} EngMenuItem;
extern EngMenuItem D_8005A3EC[];
extern s16 D_8005A3A4[];

void func_80079554(void);

/* D_8005A614 as seen by func_800798A4. */
typedef struct EngHudGame {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x32 - 0x14];
    /* 0x32 */ u16 unk32;
    /* 0x34 */ s16 mode;
} EngHudGame;
#define HUDGAME ((EngHudGame *)D_8005A614)
#define HUDPLAYERS ((EXTengine **)&Icon)
extern s32 D_8009AEF4;
void func_800798A4(void);

extern u8 D_8005B3A8[];
extern u8 D_8005B3D8[];

#endif /* BASH_ENGINE_H */
