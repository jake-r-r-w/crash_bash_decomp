#include "common.h"
#include "bash/main.h"
#include "bash/music.h"

void music_open_seq(music_midi *slot, s32 idx) {
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
    vol = (GameOptions.music * 0x7F) / 100;
    SsSeqSetVol((s16)slot->seq, vol, vol);
    D_800655B4 = (GameOptions.music * 0x7F) / 100;
    SsSetMute(0);
}

void music_update(void) {
    music_midi *slot = &CurrentMusic;
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
                    music_open_seq(slot, next);
                }
                D_800655A4 = D_8006559C;
            }
        } else {
            D_800655A0 = 10;
            music_fade_out(20);
        }
    } else {
        if (slot->seq >= 0) {
            vol = (GameOptions.music * 0x7F) / 100;
            if (D_800655B4 != vol || D_800655AC != D_800655B0) {
                if (D_800655AC != 0) {
                    SeqSlot_SetVol(slot, (GameOptions.music * 0x55) / 100);
                } else {
                    SeqSlot_SetVol(slot, vol);
                }
            }
        }
        D_800655B0 = D_800655AC;
        D_800655B4 = (GameOptions.music * 0x7F) / 100;
    }
}

void music_init(void) {
    CurrentMusic.data = 0;
    CurrentMusic.seq = -1;
    D_800655A4 = -1;
    D_8006559C = -1;
    D_800655B4 = 0;
}

void music_close(music_midi *slot) {
    if (slot->seq >= 0) {
        SsSeqClose(slot->seq);
        SsSepStop(slot->seq, 0);
    }
    slot->unk1C = -1;
}

void continue_music(void) {
    if (CurrentMusic.seq >= 0) {
        SsSeqReplay(CurrentMusic.seq);
    }
}

void music_pause(void) {
    if (CurrentMusic.seq >= 0) {
        SsSeqPause(CurrentMusic.seq);
    }
}

void music_replay(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqReplay(*(s32 *)(arg0 + 4));
    }
}

void music_pause_slot(u8 *arg0) {
    if (*(s32 *)(arg0 + 4) >= 0) {
        SsSeqPause(*(s32 *)(arg0 + 4));
    }
}

void music_set_tempo(s32 arg0, s32 arg1) {
    if (CurrentMusic.seq >= 0) {
        SsSeqSetAccelerando((s16)CurrentMusic.seq, (MusicTable[GameMode].tempo * arg0) / 100, arg1);
    }
}

void music_fade_out(s32 arg0) {
    if (CurrentMusic.seq >= 0) {
        SsSeqSetDecrescendo(CurrentMusic.seq, 0x7F, arg0);
    }
}

void music_request(s32 arg0) {
    D_8006559C = arg0;
}

void music_duck_off(void) {
    D_800655AC = 0;
}

void music_flag_restart(void) {
    D_800655AC = 1;
}

void music_restart(void) {
    D_800655A8 += 1;
}

void music_setup_slot(music_midi *slot) {
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
    slot->data = (s32)mal_trim_front((void *)slot->data, ((s32 *)slot->data)[1], &MainHeap);
}

void music_dload_midi(s32 arg0, music_midi *slot, s32 type) {
    asload_permanent_callback(arg0, music_set_data, slot);
    SpuReserveReverbWorkArea(1);
    SsUtSetReverbType(type);
    SsUtSetReverbDepth(0, 0);
    func_800369FC();
}

void music_set_data(s32 arg0, s32 *arg1) {
    *arg1 = arg0;
    music_setup_slot(arg1);
    D_800655A8 = 0;
    D_800655AC = 0;
    D_800655B0 = 0;
}

void music_stop(void) {
    music_close(&CurrentMusic);
}

void music_free(void) {
    if (CurrentMusic.data != 0) {
        mal_free(CurrentMusic.data, &MainHeap);
        CurrentMusic.data = 0;
    }
}

void music_load(s32 arg0, s32 type) {
    music_dload_midi(arg0, &CurrentMusic, type);
}

/* data, in address order (tools/scripts/data2c.py) */
s32 D_8006559C = 0;
s32 D_800655A0 = 0;
s32 D_800655A4 = 0;
s32 D_800655A8 = 0;
s32 D_800655AC = 0;
s32 D_800655B0 = 0;
s32 D_800655B4 = 0;
