/* The game's fingerprint: its own values folded into a few numbers after every step, which come out the same on every machine given the
 * same presses. Online play has each machine run the whole game from everyone's presses, and this is how they check they still agree
 * (docs/PORT.md, Online play).
 *
 * It's made of the game's values, not its memory: addresses differ from one machine and one run to the next, so nothing here is a
 * pointer, and what's in the heap goes in by its place in the heap.
 *
 * A test run prints it (--fingerprints N: every N steps), as a line of parts, so that the first to differ says where to look:
 *     fp STEP all ... rand ... game ... heroes ... objects ... heap ... pads ... sound ...
 * Each part is that step's alone, and `all` is every step's so far: two runs with the same `all` have agreed at every step. */
#include <stddef.h>
#include "../../platform/settings.h"
#include "common.h"
#include "bash/main.h"
#include "bash/engine.h"
#include "bash/fingerprint.h"

int printf(const char *, ...);

#define FOLD_START 0x811C9DC5u

static s32 Steps;
static u32 All = FOLD_START;

static u32 fold(u32 h, const void *bytes, size_t size) {
    const u8 *p = bytes;

    while (size-- != 0) {
        h = (h ^ *p++) * 0x01000193u;
    }
    return h;
}

static u32 fold_word(u32 h, u32 word) {
    u8 b[4] = { (u8)word, (u8)(word >> 8), (u8)(word >> 16), (u8)(word >> 24) };

    return fold(h, b, 4);
}

/* A pointer the game holds is only followed if it's into the heap, so a stale one reads old bytes and nothing worse. */
static s32 in_heap(const void *p, size_t size) {
    return (const u8 *)p >= HeapArena && (const u8 *)p + size <= ARENA_END;
}

static u32 place(const void *p) {
    return (u32)((const u8 *)p - HeapArena);
}

/* Each hero's numbers: what it's doing, its health, score, power-up and timers. Where it is comes with the objects. */
static u32 heroes_part(void) {
    u32 h = FOLD_START;
    s32 i;

    for (i = 0; i < 4; i++) {
        h = fold(h, &Heroes[i].hero_mode, offsetof(HeroInfo, hero_attack_counter) + sizeof(Heroes[i].hero_attack_counter) - offsetof(HeroInfo, hero_mode));
    }
    return h;
}

/* Every object in play: which it is, what kind, its state and where it is. */
static u32 objects_part(void) {
    u32 h = FOLD_START;
    NAS *nas;
    s32 n;

    for (nas = NASActiveHead, n = 0; nas != NULL && in_heap(nas, sizeof(*nas)) && n < 4096; nas = nas->Tag.Child, n++) {
        EXTengine *eng = nas->Engine;

        h = fold_word(h, place(nas));
        h = fold_word(h, (u32)nas->NASid);
        h = fold_word(h, nas->Type | (u32)nas->Class << 16);
        h = fold_word(h, nas->UserInfo.status | (u32)nas->UserInfo.flags << 16);
        if (eng != NULL && in_heap(eng, sizeof(*eng))) {
            h = fold(h, &eng->XYZ, sizeof(eng->XYZ));
        }
    }
    return fold_word(h, (u32)n);
}

/* The heap's shape: where its free room is. Anything made or freed at a different moment shows here. */
static u32 heap_part(void) {
    u32 h = FOLD_START;
    mal *b;
    s32 n;

    for (b = MainHeap.free, n = 0; b != NULL && in_heap(b, sizeof(*b)) && n < 65536; b = MAL_NEXT_FREE(b), n++) {
        h = fold_word(h, place(b));
        h = fold_word(h, (u32)b->size);
    }
    h = fold_word(h, (u32)n);
    for (b = MainHeap.deferred, n = 0; b != NULL && in_heap(b, sizeof(*b)) && n < 65536; b = MAL_NEXT_FREE(b), n++) {
        h = fold_word(h, place(b));
    }
    return fold_word(h, (u32)n);
}

/* Before the first step of a game that every machine starts together. */
void fingerprint_start(void) {
    Steps = 0;
    All = FOLD_START;
    if (settings()->fingerprints > 0) {
        printf("fp start\n");
    }
}

s32 fingerprint_steps(void) {
    return Steps;
}

/* After a step: every step's fingerprint so far. */
u32 fingerprint_step(void) {
    u32 part[7];
    s32 every = settings()->fingerprints;

    part[0] = fold_word(fold_word(FOLD_START, (u32)RandomSeedA), (u32)RandomSeedB);
    part[1] = fold_word(fold_word(fold(FOLD_START, GAME, sizeof(*GAME)), (u32)FrameCount), (u32)GameFrameCount);
    part[2] = heroes_part();
    part[3] = objects_part();
    part[4] = heap_part();
    part[5] = fold(FOLD_START, Pads, sizeof(Pads));
    part[6] = fold(FOLD_START, AudioVoiceStates, 24);
    Steps++;
    All = fold_word(All, (u32)Steps);
    All = fold(All, part, sizeof(part));
    if (every > 0 && Steps % every == 0) {
        printf("fp %d all %08X rand %08X game %08X heroes %08X objects %08X heap %08X pads %08X sound %08X\n", Steps, All, part[0], part[1], part[2], part[3],
               part[4], part[5], part[6]);
    }
    return All;
}
