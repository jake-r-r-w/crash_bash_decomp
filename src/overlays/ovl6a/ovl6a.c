#include "common.h"
#include "bash/engine.h"
#include "bash/main.h"
#include "bash/ovl6a.h"
#include "bash/hub.h"

INCLUDE_ASM("asm/nonmatchings/overlays/ovl6a/ovl6a", func_800B4694);

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
