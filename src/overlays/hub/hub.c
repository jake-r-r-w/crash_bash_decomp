#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/hub_ovl.h"
#include "bash/hub.h"

void func_800B4694(void) {
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
        func_800BAAFC();
    } else {
        func_800B8588();
    }
    if (D_8005A688 != 0) {
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0(GetEngineSlot(0));
    }
    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = D_8009F644.amount;
    func_800798A4();
}

void func_800B478C(void) {
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
    func_800B4C94();
}

INCLUDE_RODATA("asm/nonmatchings/overlays/hub/hub", D_800B32B8);

s32 func_800B488C(void) {
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
    func_800B8CA0();
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
            func_800B4C94();
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

void func_800B4C94(void)
{
  int new_var;
  u8 *g;
  s32 i;
  func_8002C284(1);
  g = D_8005A614;
  ((EngMenuGame *) g)->unk74 = 0;
  func_800B8324(0);
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

s32 *func_800B4D8C(void) {
    return D_800C7294;
}

int func_800B4D98(unsigned char *s) {
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

void func_800B4DE8(void) {
    if (D_8005A688 != 0) {
        func_800B488C();
    }
    func_800B85A8();
}

void func_800B4E1C(void) {
    if (D_8005A648 == 0x2B) {
        D_8005AA70[0] = (s32)func_800B875C;
        D_8005AA70[1] = (s32)func_800B88B8;
        D_8005AA70[2] = (s32)func_800B88B8;
        D_8005AA70[3] = (s32)func_800BAB40;
    } else {
        D_8005AA70[0] = (s32)func_800B86AC;
        D_8005AA70[1] = (s32)func_800B4EAC;
        D_8005AA70[2] = (s32)func_800B4EAC;
        D_8005AA70[3] = (s32)func_800B4DE8;
    }
    D_8005AA70[7] = 0;
    D_8005AA70[8] = 0;
    D_8009F8B4 = func_800B4694;
}
