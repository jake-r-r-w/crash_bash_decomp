#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl6a.h"
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
    D_800569AC = *(s32 *)D_8009F644;
    func_800798A4();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B478C);

INCLUDE_RODATA("asm/nonmatchings/overlays/ovl6a/ovl6a", D_800B32B4);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B488C);

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4C94);

s32 *func_800B4D8C(void) {
    return D_800C7294;
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4D98);

void func_800B4DE8(void) {
    if (D_8005A688 != 0) {
        func_800B488C();
    }
    func_800B85A8();
}

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4E1C);
