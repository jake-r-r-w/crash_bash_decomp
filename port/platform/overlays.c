/* The minigame overlays. On the PS1 they share addresses, and loading one puts its data back as the file has it. Here they
 * all link into the program, so each one's data has a section of its own (ovlsection.h), saved at startup and put back when
 * the game loads that overlay. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned sector, size;
} DatFile;

extern DatFile DatIndex[];

/* Each section's ends, as each system's linker gives them (ovlsection.h) */
#if defined(_WIN32)
#define OVL_MARKS(name)                                                         \
    __attribute__((section(".ov" #name "$a"))) char ovl_##name##_begin[8] = { 1 }; \
    __attribute__((section(".ov" #name "$c"))) char ovl_##name##_end[8] = { 1 };
#define OVL_BEGIN(name) ovl_##name##_begin
#define OVL_END(name) ovl_##name##_end
#elif defined(__APPLE__)
#define OVL_MARKS(name)                                                \
    extern char ovl_##name##_begin[] __asm("section$start$__DATA$__ov" #name); \
    extern char ovl_##name##_end[] __asm("section$end$__DATA$__ov" #name);
#define OVL_BEGIN(name) ovl_##name##_begin
#define OVL_END(name) ovl_##name##_end
#elif defined(__clang__)
#define OVL_MARKS(name) extern char __start_ovl_##name[], __stop_ovl_##name[];
#define OVL_BEGIN(name) __start_ovl_##name
#define OVL_END(name) __stop_ovl_##name
#else
/* GCC has no pragma for the section, so its builds have none: they link, as a check of the C, but don't play (overlays_init) */
#define OVL_MARKS(name) extern char __start_ovl_##name[] __attribute__((weak)), __stop_ovl_##name[] __attribute__((weak));
#define OVL_BEGIN(name) __start_ovl_##name
#define OVL_END(name) __stop_ovl_##name
#endif
#define OVL(name, entry) OVL_MARKS(name)
#include "overlay_list.h"
#undef OVL

typedef struct {
    int entry;
    char *data, *data_end;
    char *saved;
} Overlay;

static Overlay Overlays[] = {
#define OVL(name, entry) { entry, OVL_BEGIN(name), OVL_END(name), NULL },
#include "overlay_list.h"
#undef OVL
};

/* A whole section, across every object in it: a sanitizer's check of one object would stop memcpy. */
__attribute__((no_sanitize("address")))
static void copy_section(void *to, const void *from, size_t n) {
    volatile char *d = to;
    const volatile char *s = from;

    while (n-- != 0) {
        *d++ = *s++;
    }
}

void overlays_init(void) {
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        Overlay *o = &Overlays[i];
        size_t n = (size_t)(o->data_end - o->data);

        /* every overlay has data, more than the marks' 8 bytes: less is a section the linker didn't lay out as ovlsection.h says,
           and the game would carry an overlay's data over from one load to the next */
        if (o->data == NULL || o->data_end <= o->data + 8) {
            fprintf(stderr, "overlay %d: its data has no section of its own (ovlsection.h), so it can't be put back: a GCC build doesn't play\n",
                    o->entry);
            abort();
        }
        o->saved = malloc(n);
        copy_section(o->saved, o->data, n);
    }
}

/* The engine's own overlay, overlays/gameeng.bin, which the program has built in too */
#define ENGINE_ENTRY 991

/* Whether an archive entry is code: the program has all of it built in, so a mod's file in its place would do nothing (mods.c). */
int overlay_is_code(int entry) {
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        if (Overlays[i].entry == entry) {
            return 1;
        }
    }
    return entry == ENGINE_ENTRY;
}

/* LoadModeOverlay's "load this file into the slot": F names which overlay. */
void overlay_load(const void *f) {
    int entry = (int)((const DatFile *)f - DatIndex);
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        Overlay *o = &Overlays[i];

        if (o->entry == entry) {
            copy_section(o->data, o->saved, (size_t)(o->data_end - o->data));
        }
    }
}
