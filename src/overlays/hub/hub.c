#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/hub_ovl.h"
#include "bash/hub.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern u8 D_8004ECA0[];
extern u8 D_8004ECA8[];
extern u8 D_8004ECB0[];
extern u8 D_8004ECB8[];
extern u8 D_8004ECC0[];
extern u8 D_8004ECC8[];
extern u8 D_8004ECD0[];
extern u8 D_8004ECD8[];
extern u8 D_800500A0[];
extern u8 D_80063BB8[];
extern u8 D_80063EC8[];

void HubRender(void) {
    func_80093110();
    if (D_8005A648 == 0x25) {
        func_8007A718();
        if (D_80098714 == 0) {
            func_8001DAF8((GfxScene *)D_80058AD0);
        }
    } else {
        func_8007A718();
        func_8001DAF8((GfxScene *)D_80058AD0);
    }
    if (D_8005A648 == 0x2B) {
        PodiumRenderScene();
    } else {
        HubRenderScene();
    }
    if (D_8005A688 != 0) {
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0(GetEngineSlot(0));
    }
    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = D_8009F644.amount;
    func_800798A4();
}

void HubStartCredits(void) {
    s32 one;
    D_80098714 = 0;
    ((EngMenuGame *)D_8005A614)->flags &= 0xFDC07013;
    *(s32 *)&D_800BC2BC = (s16)((HubCam *)D_80051640)->h;
    *(s32 *)&D_800BC2C0 = D_800AD12C;
    D_800BC2C4 = ((HubCam *)D_80051640)->x;
    D_800BC2C8 = ((HubCam *)D_80051640)->y;
    D_800BC2CC = ((HubCam *)D_80051640)->z;
    if (*(u16 *)&((EngMenuGame *)D_8005A614)->unk74 == 0) {
        func_8002C284(2);
        ((EngMenuGame *)D_8005A614)->unk74 = 1;
        one = 1;
        D_8009870E = one;
        D_80098710 = 0;
        D_800BC2B4 = 0;
        D_80098714 = one;
        D_800AD12C = 0;
        D_800BC2B8 = D_8005A608;
        func_80095A60(0, 0x1E);
        return;
    }
    HubEndCredits();
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char D_800B32B8[] = "BREATHE";
const char D_800B32C0[] = "MEDIUM";
const char D_800B32C8[] = "ATTACK";
const char D_800B32D0[] = "JUMP";
const char D_800B32D8[] = "KICK";

s32 HubCreditsScroll(void) {
    OvlTextView *view;
    OvlTextLine *line;
    OvlTextLine *lines;
    subengine *sub;
    s32 n;
    s32 i;
    s32 y;
    s32 done;
    s32 ypos;

    view = (OvlTextView *)D_80051640;
    view->unk54 = 0xF0;
    view->unk56 = 0;
    view->unkC = 0;
    view->unk10 = 0;
    view->unk14 = 0x3AE;
    HubCreditsEffects();
    switch (D_8009870E) {
    case 1:
        i = 0;
        y = 0;
        lines = (OvlTextLine *)D_800C7294;
        sub = GetEngineSlot(0)->sub;
        done = 0;
        line = lines;
        while (1) {
            if (y >= D_800BC2B4) {
                break;
            }
            if (done) {
                goto end;
            }
            line = &lines[i];
            OVL_TEXT_LINE_COUNT(n, line->text);
            switch (line->kind) {
            case 0:
            case 1:
                y += n * 40;
                break;
            case 2:
                y += n * 20;
                break;
            case 3:
                y += line->height;
                break;
            case 4:
                done = 1;
                break;
            }
            i++;
        }
        if (done) {
            goto end;
        }
        while (1) {
            if (done) {
                break;
            }
            if (sub == NULL) {
                done = 1;
                continue;
            }
            line = &lines[i];
            OVL_TEXT_LINE_COUNT(n, line->text);
            sub->flags |= 0x8000;
            ypos = y - D_800BC2B4 - 50;
            switch (line->kind) {
            case 1:
                func_80095F48((s32 *)sub, 0x8000, ypos, (s32)line->text, 7);
                y += n * 40;
                break;
            case 0:
                func_80095F48((s32 *)sub, 0x8000, ypos, (s32)line->text, 0);
                y += n * 40;
                break;
            case 2:
                func_80095F48((s32 *)sub, 0x8000, ypos, (s32)line->text, 4);
                y += n * 20;
                break;
            case 3:
                y += line->height;
                sub->flags &= ~0x8000;
                break;
            case 4:
                done = 1;
                sub->flags &= ~0x8000;
                break;
            }
            sub = sub->next;
            i++;
            if (y - D_800BC2B4 > 500) {
                done = 1;
            }
        }
        D_800BC2B4 += 2;
        goto clear;
    end:
        D_8009870E = 3;
    clear:
        while (sub != NULL) {
            sub->flags &= ~0x8000;
            sub = sub->next;
        }
        return 0;
    case 3:
        D_800BC2B8 = D_8005A608;
        func_80095A04(0);
        D_8009870E = 2;
        D_80098710 = 45;
        music_fade_out(45);
        set_fade(&D_8009F644, 0x200, 0);
        return 0;
    case 2:
        if (D_80098710 == 0) {
            D_8005A608 = D_800BC2B8;
            HubEndCredits();
            return 1;
        }
        D_80098710--;
        {
            s32 *opt = (s32 *)D_8005A604;
            if (opt[1] > 0) {
                opt[1] -= opt[1] >> 2;
            }
        }
        return 0;
    }
    return 0;
}

void HubEndCredits(void)
{
  int new_var;
  u8 *g;
  s32 i;
  func_8002C284(1);
  g = D_8005A614;
  ((EngMenuGame *) g)->unk74 = 0;
  HubShowShadow(0);
  D_80098714 = 0;
  set_fade(&D_8009F644, -0x200, 0);
  new_var = 0x1628;
  *((u8 *) (g + 0x75)) = 1;
  for (i = 0; i < ASTCount; i++)
  {
    if (ASTTable[i].id == new_var)
    {
      goto found;
    }
  }

  i = 0;
  found:
  DeleteNas_AllAST(i);

  ((HubCam *) D_80051640)->h = D_800BC2BC;
  D_800AD12C = D_800BC2C0;
  ((HubCam *) D_80051640)->x = D_800BC2C4;
  ((HubCam *) D_80051640)->y = D_800BC2C8;
  ((HubCam *) D_80051640)->z = D_800BC2CC;
}

s32 *HubCreditsText(void) {
    return (s32 *)D_800C7294;
}

int HubCountLines(unsigned char *s) {
    int n = 1;

    if (s == 0) {
        return 0;
    }
    if (*s != 0) {
        unsigned char c;
        int k;

        k = 0xD;
        c = *(volatile unsigned char *)s;
        do {
            if (c == k) {
                n++;
            }
            s++;
            c = *s;
        } while (c != 0);
    }
    return n;
}

void UpdateHUB(void) {
    if (D_8005A688 != 0) {
        HubCreditsScroll();
    }
    HubUpdateRoom();
}

void HUBSetUp(void) {
    if (D_8005A648 == 0x2B) {
        D_8005AA70[0] = (s32)InitialisePodiumFirst;
        D_8005AA70[1] = (s32)InitialisePodium;
        D_8005AA70[2] = (s32)InitialisePodium;
        D_8005AA70[3] = (s32)UpdatePodium;
    } else {
        D_8005AA70[0] = (s32)InitialiseHUBFirst;
        D_8005AA70[1] = (s32)InitialiseHUB;
        D_8005AA70[2] = (s32)InitialiseHUB;
        D_8005AA70[3] = (s32)UpdateHUB;
    }
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8009F8B4 = HubRender;
}

/* data, in address order */
s32 D_800BBFE8[6] = { (s32)&D_800B32B8, (s32)&D_800B32C0, (s32)&D_800B32C8, (s32)&D_800B32D0, (s32)&D_800B32D8, 0 };
s32 D_800BC000[2] = { 0x1C00D5, 0xFFFF };
s32 D_800BC008[1] = {0};
s32 D_800BC00C[2] = { 0x1C0017, 0xFFFF };
s32 D_800BC014[27] = {
    (s32)&D_80055684, (s32)&D_8004ECB8, (s32)&D_8004ECB0, (s32)&D_80053074, (s32)&D_8004ECC8,
    (s32)&D_8004ECC0, (s32)&D_800555A4, (s32)&D_8004ECA8, (s32)&D_8004ECA0, (s32)&D_800558EC,
    (s32)&D_8004ECD8, (s32)&D_8004ECD0, 0, 0, 0, (s32)&D_800500A0, (s32)&D_800BC014, 0x3000,
    0x10, 0x2000, 0xC8, 0x2BC, 0x200, (s32)&D_80063EC8, (s32)&D_80063BB8, 0x20, 0x600,
};
s32 D_800BC080[1] = { 0xFFFF };
s32 D_800BC084[128] = {
    0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCF, (s32)&D_800BBFE8,
    (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xD0, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0,
    0xD1, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xD2, (s32)&D_800BBFE8,
    (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0,
    0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCE, (s32)&D_800BBFE8,
    (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0,
    0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCE, (s32)&D_800BBFE8,
    (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0,
    0xCE, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xD3, (s32)&D_800BBFE8,
    (s32)&D_800BC000, 6, 0, 0, 0, 0, 0xD4, (s32)&D_800BBFE8, (s32)&D_800BC000, 6, 0, 0, 0, 0,
    0, (s32)&D_800BC008, (s32)&D_800BC00C, 4, 0, 0, 0, 0,
};
s32 D_800BC284[3] = { 0x20D0003, 0x1F021F01, 0x1F03 };
s32 D_800BC290[9] = { (s32)&D_800BC284, 5, (s32)&D_800BC080, (s32)&D_800BC084, (s32)&D_800BC290, 0, 0, 0, 0 };
s32 D_800BC2B4 = 0;
s32 D_800BC2B8 = 0;
u16 D_800BC2BC = 0;
s16 D_800BC2BE = 0;
u16 D_800BC2C0 = 0;
s32 D_800BC2C4 = 0;
s32 D_800BC2C8 = 0;
s32 D_800BC2CC = 0;
