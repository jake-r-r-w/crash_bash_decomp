/* The minigame overlays. On the PS1 they share addresses, and loading one puts its data back as the file has it. Here they
 * all link into the program, so each one's data has a section of its own (ovlsection.h), saved at startup and put back when
 * the game loads that overlay. */
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned sector, size;
} DatFile;

extern DatFile DatIndex[];

#define OVL_MARKS(name)                                                         \
    __attribute__((section(".ov" #name "$a"))) char ovl_##name##_begin[8] = { 1 }; \
    __attribute__((section(".ov" #name "$c"))) char ovl_##name##_end[8] = { 1 };
#define OVL(name, entry) OVL_MARKS(name)
#include "overlay_list.h"
#undef OVL

typedef struct {
    int entry;
    char *data, *data_end;
    char *saved;
} Overlay;

static Overlay Overlays[] = {
#define OVL(name, entry) { entry, ovl_##name##_begin, ovl_##name##_end, NULL },
#include "overlay_list.h"
#undef OVL
};

void overlays_init(void) {
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        Overlay *o = &Overlays[i];
        size_t n = (size_t)(o->data_end - o->data);

        o->saved = malloc(n ? n : 1);
        memcpy(o->saved, o->data, n);
    }
}

/* LoadModeOverlay's "load this file into the slot": F names which overlay. */
void overlay_load(const void *f) {
    int entry = (int)((const DatFile *)f - DatIndex);
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        Overlay *o = &Overlays[i];

        if (o->entry == entry) {
            memcpy(o->data, o->saved, (size_t)(o->data_end - o->data));
        }
    }
}
