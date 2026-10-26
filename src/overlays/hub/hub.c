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
    if (GameMode == 0x25) {
        func_8007A718();
        if (D_80098714 == 0) {
            func_8001DAF8((GfxScene *)MapScene);
        }
    } else {
        func_8007A718();
        func_8001DAF8((GfxScene *)MapScene);
    }
    if (GameMode == 0x2B) {
        PodiumRenderScene();
    } else {
        HubRenderScene();
    }
    if (D_8005A688 != 0) {
        func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
        func_8001C2C0(GetEngineSlot(0));
    }
    func_80018B08((GfxViewport *)D_8009F67C, (GfxCamera *)D_8009F734);
    D_800569AC = ScreenFade.amount;
    func_800798A4();
}

void HubStartCredits(void) {
    s32 one;
    D_80098714 = 0;
    ((EngMenuGame *)GameInfo)->flags &= 0xFDC07013;
    *(s32 *)&HubSavedCamH = (s16)((HubCam *)MainCamera)->h;
    *(s32 *)&HubSavedFov = CameraMode;
    HubSavedCamX = ((HubCam *)MainCamera)->x;
    HubSavedCamY = ((HubCam *)MainCamera)->y;
    HubSavedCamZ = ((HubCam *)MainCamera)->z;
    if (*(u16 *)&((EngMenuGame *)GameInfo)->unk74 == 0) {
        func_8002C284(2);
        ((EngMenuGame *)GameInfo)->unk74 = 1;
        one = 1;
        D_8009870E = one;
        D_80098710 = 0;
        HubCreditsY = 0;
        D_80098714 = one;
        CameraMode = 0;
        HubCreditsStart = D_8005A608;
        func_80095A60(0, 0x1E);
        return;
    }
    HubEndCredits();
}

/* the strings (const arrays keep their retail order among the jump tables; data2c.py) */
const char HubAnimBreathe[] = "BREATHE";
const char HubAnimMedium[] = "MEDIUM";
const char HubAnimAttack[] = "ATTACK";
const char HubAnimJump[] = "JUMP";
const char HubAnimKick[] = "KICK";

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

    view = (OvlTextView *)MainCamera;
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
        lines = (OvlTextLine *)HubCreditsLines;
        sub = GetEngineSlot(0)->sub;
        done = 0;
        line = lines;
        while (1) {
            if (y >= HubCreditsY) {
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
            ypos = y - HubCreditsY - 50;
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
            if (y - HubCreditsY > 500) {
                done = 1;
            }
        }
        HubCreditsY += 2;
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
        HubCreditsStart = D_8005A608;
        func_80095A04(0);
        D_8009870E = 2;
        D_80098710 = 45;
        music_fade_out(45);
        set_fade(&ScreenFade, 0x200, 0);
        return 0;
    case 2:
        if (D_80098710 == 0) {
            D_8005A608 = HubCreditsStart;
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
  g = GameInfo;
  ((EngMenuGame *) g)->unk74 = 0;
  HubShowShadow(0);
  D_80098714 = 0;
  set_fade(&ScreenFade, -0x200, 0);
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

  ((HubCam *) MainCamera)->h = HubSavedCamH;
  CameraMode = HubSavedFov;
  ((HubCam *) MainCamera)->x = HubSavedCamX;
  ((HubCam *) MainCamera)->y = HubSavedCamY;
  ((HubCam *) MainCamera)->z = HubSavedCamZ;
}

s32 *HubCreditsText(void) {
    return (s32 *)HubCreditsLines;
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
    if (GameMode == 0x2B) {
        LevelSpecs[0] = (s32)InitialisePodiumFirst;
        LevelSpecs[1] = (s32)InitialisePodium;
        LevelSpecs[2] = (s32)InitialisePodium;
        LevelSpecs[3] = (s32)UpdatePodium;
    } else {
        LevelSpecs[0] = (s32)InitialiseHUBFirst;
        LevelSpecs[1] = (s32)InitialiseHUB;
        LevelSpecs[2] = (s32)InitialiseHUB;
        LevelSpecs[3] = (s32)UpdateHUB;
    }
    LevelSpecs[7] = 0;
    LevelSpecs[8] = 0;
    D_8009F8B4 = HubRender;
}

/* data, in address order */
char *HubAnims[6] = {
    (char *)HubAnimBreathe, (char *)HubAnimMedium, (char *)HubAnimAttack, (char *)HubAnimJump,
    (char *)HubAnimKick, NULL,
};
s16 HubLevelDB[4] = { 0xD5, 0x1C, -1, 0 };
char *HubNoAnims[1] = { NULL };
s16 HubLevelDB2[4] = { 0x17, 0x1C, -1, 0 };
s32 HubFiles[15] = {
    (s32)&D_80055684, (s32)&D_8004ECB8, (s32)&D_8004ECB0, (s32)&D_80053074, (s32)&D_8004ECC8,
    (s32)&D_8004ECC0, (s32)&D_800555A4, (s32)&D_8004ECA8, (s32)&D_8004ECA0, (s32)&D_800558EC,
    (s32)&D_8004ECD8, (s32)&D_8004ECD0, 0, 0,
};
/* the warp rooms' files (the engine's per-mode table points at modes 0x21..0x27's) */
file_group_load_info file_group_HUB = { D_800500A0, HubFiles };
/* the shell settings (engine world record 0x0102) */
shell_setup hub_shell_info = {
    0x3000, 0x10, 0x2000, 0xC8, 0x2BC, 0x200, D_80063EC8, D_80063BB8, 0x20, 0x600,
};
s16 HubPreLoadDB[2] = { -1, 0 };
WWInf HubWorldInfo[16] = {
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCF, HubAnims, HubLevelDB, 6 },
    { 0xD0, HubAnims, HubLevelDB, 6 },
    { 0xD1, HubAnims, HubLevelDB, 6 },
    { 0xD2, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xCE, HubAnims, HubLevelDB, 6 },
    { 0xD3, HubAnims, HubLevelDB, 6 },
    { 0xD4, HubAnims, HubLevelDB, 6 },
    { 0x0, HubNoAnims, HubLevelDB2, 4 },
};
u16 HubLUPtab[6] = { 3, 0x20D, 0x1F01, 0x1F02, 0x1F03, 0 };
TrigLookUp HubLUP = { HubLUPtab, 5 };
PreMap HUB_PREMAP = { HubPreLoadDB, HubWorldInfo, &HubLUP };
s32 HubCreditsY = 0;
s32 HubCreditsStart = 0;
u16 HubSavedCamH = 0;
s16 HubSavedCamPad = 0;
u16 HubSavedFov = 0;
s32 HubSavedCamX = 0;
s32 HubSavedCamY = 0;
s32 HubSavedCamZ = 0;
