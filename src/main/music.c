#include "common.h"
#include "bash/main.h"
#include "bash/music.h"

void func_8002BD98(SeqSlot *slot, s32 idx) {
    s16 vol;

    if (slot->unk1C != -1) {
        if (slot->seq != -1) {
            SsSeqClose((s16)slot->seq);
            SsSepStop((s16)slot->seq, 0);
        }
    }
    SsSeqOpen(*(s32 *)(((u8 *)slot + 8) - (-(idx * 4))), (s16)slot->seq);
    slot->unk1C = idx;
    SsSetMVol(0x7F, 0x7F);
    SsSeqPlay((s16)slot->seq, 1, 0);
    vol = (*(s32 *)D_8005A604 * 0x7F) / 100;
    SsSeqSetVol((s16)slot->seq, vol, vol);
    D_800655B4 = (*(s32 *)D_8005A604 * 0x7F) / 100;
    SsSetMute(0);
}

void func_8002BEA8(void) {
    SeqSlot *slot = &D_8006557C;
    s32 next = D_8006559C;
    s32 cur = D_800655A4;
    s32 vol;

    if (next != cur || D_800655A8 != 0) {
        s32 t = D_800655A0;

        if (t != 0 || cur == -1) {
            D_800655A0 = t - 1;
            if (D_800655A0 == 0 || cur == -1) {
                D_800655A8 = 0;
                D_800655A0 = 0;
                if (next != -1) {
                    func_8002BD98(slot, next);
                }
                D_800655A4 = D_8006559C;
            }
        } else {
            D_800655A0 = 10;
            music_fade_out(20);
        }
    } else {
        if (slot->seq >= 0) {
            vol = (*(s32 *)D_8005A604 * 0x7F) / 100;
            if (D_800655B4 != vol || D_800655AC != D_800655B0) {
                if (D_800655AC != 0) {
                    SeqSlot_SetVol(slot, (*(s32 *)D_8005A604 * 0x55) / 100);
                } else {
                    SeqSlot_SetVol(slot, vol);
                }
            }
        }
        D_800655B0 = D_800655AC;
        D_800655B4 = (*(s32 *)D_8005A604 * 0x7F) / 100;
    }
}

void func_8002C060(void) {
    D_8006557C.data = 0;
    D_8006557C.seq = -1;
    D_800655A4 = -1;
    D_8006559C = -1;
    D_800655B4 = 0;
}

void func_8002C090(SeqSlot *slot) {
    if (slot->seq >= 0) {
        SsSeqClose(slot->seq);
        SsSepStop(slot->seq, 0);
    }
    slot->unk1C = -1;
}

void continue_music(void) {
    if (D_8006557C.seq >= 0) {
        SsSeqReplay(D_8006557C.seq);
    }
}

void func_8002C11C(void) {
    if (D_8006557C.seq >= 0) {
        SsSeqPause(D_8006557C.seq);
    }
}

void func_8002C158(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqReplay(*(s32 *)(arg0 + 4));
    }
}

void func_8002C18C(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqPause(*(s32 *)(arg0 + 4));
    }
}

void func_8002C1C0(s32 arg0, s32 arg1) {
    if (D_8006557C.seq >= 0) {
        SsSeqSetAccelerando((s16)D_8006557C.seq, (D_80058B88[D_8005A648].tempo * arg0) / 100, arg1);
    }
}

void music_fade_out(s32 arg0) {
    if (D_8006557C.seq >= 0) {
        SsSeqSetDecrescendo(D_8006557C.seq, 0x7F, arg0);
    }
}

void func_8002C284(s32 arg0) {
    D_8006559C = arg0;
}

void music_duck_off(void) {
    D_800655AC = 0;
}

void func_8002C29C(void) {
    D_800655AC = 1;
}

void music_restart(void) {
    D_800655A8 += 1;
}

void func_8002C2C4(SeqSlot *slot) {
    u8 *body;
    int new_var;
    u8 *head;

    SeqHdr *h = (SeqHdr *)slot->data;

    head = (u8 *)h + h->head;
    body = (u8 *)h + h->body;
    slot->p8 = (s32)h + h->o8;
    slot->pC = slot->data + ((SeqHdr *)slot->data)->oC;
    slot->p10 = slot->data + ((SeqHdr *)slot->data)->o10;
    slot->p14 = slot->data + ((SeqHdr *)slot->data)->o14;
    new_var = slot->data + ((SeqHdr *)slot->data)->o18;
    slot->unk1C = -1;
    slot->p18 = new_var;
    slot->seq = SsVabOpenHead(head, -1);
    SsVabTransBody(body, (s16)slot->seq);
    SpuIsTransferCompleted(1);
    slot->data = (s32)func_800112A4((void *)slot->data, ((s32 *)slot->data)[1], &D_8004E0F0);
}

void music_dload_midi(s32 arg0, SeqSlot *slot, s32 type) {
    asload_permanent_callback(arg0, func_8002C3F0, slot);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(type);
    SsUtSetReverbDepth(0, 0);
    func_800369FC();
}

void func_8002C3F0(s32 arg0, s32 *arg1) {
    *arg1 = arg0;
    func_8002C2C4(arg1);
    D_800655A8 = 0;
    D_800655AC = 0;
    D_800655B0 = 0;
}

void func_8002C428(void) {
    func_8002C090(&D_8006557C);
}

void func_8002C44C(void) {
    if (D_8006557C.data != 0) {
        mal_free(D_8006557C.data, &D_8004E0F0);
        D_8006557C.data = 0;
    }
}

void func_8002C488(s32 arg0, s32 type) {
    music_dload_midi(arg0, &D_8006557C, type);
}
