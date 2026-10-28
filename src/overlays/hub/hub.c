#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/hub_ovl.h"
#include "bash/hub.h"

/* the exe's data the tables point at, not declared in the headers yet */
extern s32 VramNodePool[];
extern u8 VramAreas[];

void HubRender(void) {
    SetGameViewport();
    if (GameMode == 0x25) {
        RenderAllNas();
        if (CreditsActive == 0) {
            draw_map_scene((GfxScene *)MapScene);
        }
    } else {
        RenderAllNas();
        draw_map_scene((GfxScene *)MapScene);
    }
    if (GameMode == 0x2B) {
        PodiumRenderScene();
    } else {
        HubRenderScene();
    }
    if (GameAdventure.unk74 != 0) {
        gfx_set_viewport(&MainViewport, &MainGfxCamera);
        eng_render_subs(GetEngineSlot(0));
    }
    gfx_set_viewport(&MainViewport, &MainGfxCamera);
    RenderState.modelDp = ScreenFade.amount;
    DrawLevelHud();
}

void HubStartCredits(void) {
    s32 one;
    CreditsActive = 0;
    ((EngMenuGame *)GameInfo)->flags &= 0xFDC07013;
    *(s32 *)&HubSavedCamH = (s16)((HubCam *)MainCamera)->h;
    *(s32 *)&HubSavedFov = CameraMode;
    HubSavedCamX = ((HubCam *)MainCamera)->x;
    HubSavedCamY = ((HubCam *)MainCamera)->y;
    HubSavedCamZ = ((HubCam *)MainCamera)->z;
    if (*(u16 *)&((EngMenuGame *)GameInfo)->unk74 == 0) {
        music_request(2);
        ((EngMenuGame *)GameInfo)->unk74 = 1;
        one = 1;
        CreditsStep = one;
        CreditsTimer = 0;
        HubCreditsY = 0;
        CreditsActive = one;
        CameraMode = 0;
        HubCreditsStart = GameOptions.sfx;
        InitEngineSlot(0, 0x1E);
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
    switch (CreditsStep) {
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
                PlaceFlashSub((s32 *)sub, 0x8000, ypos, (s32)line->text, 7);
                y += n * 40;
                break;
            case 0:
                PlaceFlashSub((s32 *)sub, 0x8000, ypos, (s32)line->text, 0);
                y += n * 40;
                break;
            case 2:
                PlaceFlashSub((s32 *)sub, 0x8000, ypos, (s32)line->text, 4);
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
        CreditsStep = 3;
    clear:
        while (sub != NULL) {
            sub->flags &= ~0x8000;
            sub = sub->next;
        }
        return 0;
    case 3:
        HubCreditsStart = GameOptions.sfx;
        KillEngineSlot(0);
        CreditsStep = 2;
        CreditsTimer = 45;
        music_fade_out(45);
        set_fade(&ScreenFade, 0x200, 0);
        return 0;
    case 2:
        if (CreditsTimer == 0) {
            GameOptions.sfx = HubCreditsStart;
            HubEndCredits();
            return 1;
        }
        CreditsTimer--;
        {
            GameOptionSet *opt = &GameOptions;
            if (opt->sfx > 0) {
                opt->sfx -= opt->sfx >> 2;
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
  music_request(1);
  g = GameInfo;
  ((EngMenuGame *) g)->unk74 = 0;
  HubShowShadow(0);
  CreditsActive = 0;
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
    if (GameAdventure.unk74 != 0) {
        HubCreditsScroll();
    }
    HubUpdateRoom();
}

void HUBSetUp(void) {
    if (GameMode == 0x2B) {
        LevelSpecs.LevelPreInit = InitialisePodiumFirst;
        LevelSpecs.LevelInit = InitialisePodium;
        LevelSpecs.LevelRestartInit = InitialisePodium;
        LevelSpecs.LevelRoutine = UpdatePodium;
    } else {
        LevelSpecs.LevelPreInit = InitialiseHUBFirst;
        LevelSpecs.LevelInit = InitialiseHUB;
        LevelSpecs.LevelRestartInit = InitialiseHUB;
        LevelSpecs.LevelRoutine = UpdateHUB;
    }
    LevelSpecs.SpecTriggerFunc = 0;
    LevelSpecs.OnOffWindowTest = 0;
    RenderHook = HubRender;
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
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 373], (s32)&DatIndex[2 * 372], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 375],
    (s32)&DatIndex[2 * 374], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 371], (s32)&DatIndex[2 * 370], (s32)&HeroModelBank[DB_AdventureItems],
    (s32)&DatIndex[2 * 377], (s32)&DatIndex[2 * 376], 0, 0,
};
/* the warp rooms' files (the engine's per-mode table points at modes 0x21..0x27's) */
file_group_load_info file_group_HUB = { &FileGroups[12], HubFiles };
/* the shell settings (engine world record 0x0102) */
shell_setup hub_shell_info = {
    0x3000, 0x10, 0x2000, 0xC8, 0x2BC, 0x200, VramAreas, VramNodePool, 0x20, 0x600,
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
