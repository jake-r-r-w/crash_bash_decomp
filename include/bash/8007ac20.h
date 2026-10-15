#ifndef BASH_8007AC20_H
#define BASH_8007AC20_H

/* Functions and data defined in src/overlays/engine/8007ac20.c. Anything the original headers
 * declare (bash/pogo.h, ...) is declared there instead, not here.
 * owner: src/overlays/engine/8007ac20.c */

#include "common.h"
#include "bash/engine.h"

extern s32 D_8005A554[];

void func_8007D5EC(void);

void func_8007E7A4(void);

s32 func_8007E7C8(void);

void func_8007E874(void);

void func_8007E9FC(u16 arg0, s16 arg1);

void func_8007EB04(void);

void func_8007E7F4(void);

s32 func_8007E894(void);

void func_8007D034(void);

void func_8007D0BC(void);

void func_8007EAC0(void);

void func_8007E8D0(EngineBox *p, s16 x, s16 y, s32 a, s32 b);

void func_8007EB1C(s32 arg0, s32 arg1, s32 arg2);

void func_8007CFAC(void);

void func_8007B7FC(u32 a, u32 b);

void func_8007D144(void);

extern EXTengine D_80099290[];
void func_8007EC48(void);

void func_8007ED0C(void);

void func_8007E93C(u16 arg0, s16 arg1, s16 arg2);

extern EXTengine D_80099290[4];
extern subengine D_8009971C[4][7];
extern u16 D_800991D8[4];
extern s16 D_800991E0[][2];
extern s16 D_800991F0[][2];
extern u16 D_80099218[];
typedef struct { s16 x, y; } EngXY;
extern EngXY D_80099240[4];

void func_8007BFA8(void);

extern subengine *D_80099170;
extern subengine D_80099500;
extern subengine D_8009AD94;
extern subengine D_8009AC04;
extern subengine D_8009ACAC;

void func_8007EB68(void);
typedef struct PanelPrim {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0x44];
    void *unk54;
    u8 pad58[0x10];
    s16 unk68, pad6A;
    union {
        void *ptr;
        struct {
            s16 unk6C, unk6E;
        } s;
    } u6C;
    s16 unk70, pad72, unk74, unk76, unk78, pad7A, unk7C, unk7E, unk80, pad82, unk84, unk86, unk88, pad8A;
    s32 unk8C, unk90, unk94, unk98;
    u8 pad9C[0xC];
} PanelPrim; /* 0xA8 */
extern PanelPrim D_8009986C[4][7];
extern S16Pair D_80099250[4];
extern s32 D_8009AE88[4];

void func_8007C170(void);

void func_8007CC80(void);

void func_8007AF3C(void);
void func_8007B330(void);
void func_8007CA70(void);
void func_8007CE50(void);
void func_8007D2F4(void);
void func_8007D4D0(void);
void func_8007D9DC(void);
extern s32 D_8005A618;
extern u16 D_8009D524[4];
s32 FntPrint();

void func_8007C514(void);

#endif /* BASH_8007AC20_H */
