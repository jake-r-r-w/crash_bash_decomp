/* The minigame overlays on PC. On the PS1 they share addresses, and loading one puts its data back to
 * how the file has it. Here they all link into the program (docs/PORT.md), so each one's data is saved
 * at startup and put back when the game loads it. Two markers bound it (CMakeLists.txt, data_order.txt). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The file a mode's overlay comes from, as ModeOverlayFiles has it, and the mode text's addresses. */
typedef struct {
    unsigned sector, size;
} file;

extern file DatIndex[];
extern unsigned char ModeTextBuffer[];

#define OVL(name, entry) extern char ovl_##name##_data_begin[], ovl_##name##_data_end[];
#include "overlay_list.h"
#undef OVL

typedef struct {
    int entry;
    char *data, *data_end;
    char *saved;
} Overlay;

static Overlay Overlays[] = {
#define OVL(name, entry) { entry, ovl_##name##_data_begin, ovl_##name##_data_end, NULL },
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

/* LoadModeOverlay has just loaded this file into the slot. */
void pc_overlay_loaded(const void *f) {
    int entry = (int)((const file *)f - DatIndex);
    size_t i;

    for (i = 0; i < sizeof(Overlays) / sizeof(Overlays[0]); i++) {
        Overlay *o = &Overlays[i];

        if (o->entry == entry) {
            memcpy(o->data, o->saved, (size_t)(o->data_end - o->data));
        }
    }
}

/* The mode text holds its own addresses as the PS1 has them, where ModeTextBuffer is 0x800AEC60:
 * every word in the buffer's range is one (checked on all 12 files). */
void pc_mode_text_loaded(const void *f) {
    enum { PS1_ADDR = 0x800AEC60u, BUFFER_SIZE = 0x4654 };
    uint32_t size = ((const file *)f)->size;
    uint32_t *w = (uint32_t *)ModeTextBuffer;
    uint32_t i;

    for (i = 0; i < size / 4; i++) {
        if (w[i] >= PS1_ADDR && w[i] < PS1_ADDR + BUFFER_SIZE) {
            w[i] = w[i] - PS1_ADDR + (uint32_t)(uintptr_t)ModeTextBuffer;
        }
    }
}
