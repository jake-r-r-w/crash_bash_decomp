/* The hero tool (docs/HERO_PACKS.md, The hero tool): anyone's glTF model made a hero pack the game plays.
 *
 *   herotool import MODEL.glb --id ID [--markers MARKERS.json] --out FOLDER [--name NAME] [--prefer HERO] [--template HERO] [--own GAME/MOVE=MOVE] [--height N]
 *                                                               [--face-head SHARE] [--author NAME]
 *
 * With no arguments, or a model alone, it opens its window (gui.cpp) instead.
 *
 * FOLDER becomes a mod (mod.json, heroes/ID/): put it in the game's mods folder. The model is never changed: one the game can't draw
 * (over its triangles or vertices) is refused, saying why. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "kit.h"

static void usage(void) {
    fprintf(stderr, "herotool import MODEL.glb --id ID [--markers MARKERS.json] --out FOLDER [--name NAME] [--prefer HERO] [--template HERO] [--own GAME/MOVE=MOVE] [--height N]\n"
                    "                [--face-head SHARE] [--author NAME] [--card CARD.png] [--voice JOB=FILE.wav ...]\n"
                    "                [--mod-name NAME]\n"
                    "  HERO: crash, coco, cortex, brio, tiny, kong, dingodile or rilla, whose place it takes if it's free\n"
                    "  MARKERS: where its joints are, for a model with no rig of its own (a rigged one's are its bones)\n"
                    "  JOB: the sound's job: name (the announcer's, short), laugh (taunt, win), yelp (hurt), jump, land, thud (a fall),\n"
                    "       throw, fire (an attack's own sound); a job it has no sound for is its base hero's\n"
                    "  N: its height standing, in the game's units (Crash 452, most heroes about 650)\n");
}

static int id_ok(const char *id) {
    const char *c;

    for (c = id; *c != 0; c++) {
        if (!((*c >= 'a' && *c <= 'z') || (*c >= '0' && *c <= '9') || *c == '-' || *c == '_')) {
            return 0;
        }
    }
    return c != id && c - id < 32;
}

int main(int argc, char **argv) {
    static KitModel m;
    const char *model = NULL, *markers = NULL, *out = NULL;
    char why[512], name[32];
    KitPack p;
    int i;

    memset(&p, 0, sizeof(p));
    p.prefer = "crash";
    p.height = 560;
    p.author = "";
    /* not import: the window, with a model to open, --mods FOLDER and --game PROGRAM (as the game's launcher starts it) */
    if (argc < 2 || strcmp(argv[1], "import") != 0) {
        const char *open = NULL, *mods = NULL, *game = NULL;

        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--mods") == 0 && i + 1 < argc) {
                mods = argv[++i];
            } else if (strcmp(argv[i], "--game") == 0 && i + 1 < argc) {
                game = argv[++i];
            } else {
                open = argv[i];
            }
        }
        return kit_gui(open, mods, game);
    }
    if (argc < 3 || strcmp(argv[1], "import") != 0) {
        usage();
        return 2;
    }
    model = argv[2];
    for (i = 3; i < argc; i += 2) {
        const char *a = argv[i], *v = i + 1 < argc ? argv[i + 1] : NULL;

        if (v == NULL) {
            usage();
            return 2;
        }
        if (strcmp(a, "--id") == 0) {
            p.id = v;
        } else if (strcmp(a, "--markers") == 0) {
            markers = v;
        } else if (strcmp(a, "--out") == 0) {
            out = v;
        } else if (strcmp(a, "--name") == 0) {
            p.name = v;
        } else if (strcmp(a, "--template") == 0) {
            p.template_hero = v;
        } else if (strcmp(a, "--own") == 0) {
            int n = 0;

            while (n < KIT_OWN_MOST && p.own[n] != NULL) {
                n++;
            }
            if (n < KIT_OWN_MOST) {
                p.own[n] = v;
            }
        } else if (strcmp(a, "--prefer") == 0) {
            p.prefer = v;
        } else if (strcmp(a, "--height") == 0) {
            p.height = atoi(v);
        } else if (strcmp(a, "--face-head") == 0) {
            p.face_head = (float)atof(v);
        } else if (strcmp(a, "--author") == 0) {
            p.author = v;
        } else if (strcmp(a, "--mod-name") == 0) {
            p.mod_name = v;
        } else if (strcmp(a, "--card") == 0) {
            p.card = v;
        } else if (strcmp(a, "--voice") == 0) {
            const char *eq = strchr(v, '=');
            int j;

            for (j = 0; eq != NULL && j < PACK_VOICE_JOBS
                        && !(strlen(PackVoiceJobs[j]) == (size_t)(eq - v) && strncmp(v, PackVoiceJobs[j], (size_t)(eq - v)) == 0);
                 j++) {
            }
            if (eq == NULL || j == PACK_VOICE_JOBS) {
                fprintf(stderr, "herotool: --voice JOB=FILE.wav, JOB one of name, laugh, yelp, jump, land, thud, throw, fire\n");
                return 2;
            }
            p.voice[j] = eq + 1;
        } else {
            usage();
            return 2;
        }
    }
    if (p.id == NULL || out == NULL || !id_ok(p.id)) {
        fprintf(stderr, "herotool: it needs --id (small letters, digits, - and _) and --out\n");
        usage();
        return 2;
    }
    if (p.name == NULL) {
        for (i = 0; p.id[i] != 0 && i < (int)sizeof(name) - 1; i++) {
            name[i] = (char)SDL_toupper((unsigned char)p.id[i]);
        }
        name[i] = 0;
        p.name = name;
    }
    p.mod_name = p.mod_name != NULL ? p.mod_name : p.name;
    /* its skeleton from its markers if it has them, else from its own rig */
    if (!kit_read(model, &m, why, sizeof(why)) || !kit_atlas(&m, why, sizeof(why))
        || (markers != NULL ? !kit_markers(&m, markers, why, sizeof(why)) : !kit_map_rig(&m, why, sizeof(why)))) {
        fprintf(stderr, "herotool: %s\n", why);
        kit_free(&m);
        return 1;
    }
    if (markers != NULL) {
        kit_weigh(&m);
    }
    SDL_CreateDirectory(out);
    if (!kit_write(&m, &p, out, why, sizeof(why))) {
        fprintf(stderr, "herotool: %s\n", why);
        kit_free(&m);
        return 1;
    }
    kit_free(&m);
    return 0;
}
