/* The templates' maps (bash/packtemplate.h; docs/HERO_PACKS.md, Templates): for each original hero but Crash (whose kinds are
 * packfollow.c's own tables) and each kind, markers on its model of that kind as it first moves, read off its model drawn front and
 * side on a grid of the game's units (tools/scripts/hero_view.py HERO KIND). A joint's corners are its marker's nearest (packfollow.c, map_fit);
 * the prop is the strips mostly in its boxes. Only numbers for where things are: no art. */
#include "common.h"
#include "bash/herodef.h"
#include "bash/modelbank.h"
#include "bash/packtemplate.h"

/* Each hero standing: its model on foot's height (crate2), 1 Crash to 8 Rilla Roo */
const float PackTemplateHeight[9] = { 452, 452, 627, 440, 606, 724, 666, 697, 733 };

#define P(x, y, z) { x, y, z }
#define PAIR(x, y, z) { P(-(x), y, z), P(x, y, z) }
#define NONE { P(0, 0, 0), P(0, 0, 0) }
/* a box either side (sym), and one as it is */
#define BOTH(x0, y0, z0, x1, y1, z1) { P(x0, y0, z0), P(x1, y1, z1), 1 }
#define BOX(x0, y0, z0, x1, y1, z1) { P(x0, y0, z0), P(x1, y1, z1), 0 }

static const TemplateMap Maps[] = {
    /* -- Tiny (5) -- */
    /* his pogo: he stands on its pegs, nothing to hold, his arms hanging */
    { 5, KIND_POGO, DB_CharsPogoTiny, 60,
      P(0, -280, 30), P(0, -440, 0), P(0, -560, -120),
      PAIR(180, -500, 40), PAIR(260, -330, 120), PAIR(280, -140, 180),
      PAIR(80, -250, 20), PAIR(90, -160, -40), PAIR(120, -80, -20), PAIR(150, -30, -200),
      NONE, PAIR(170, -12, -60), { BOTH(90, -40, -160, 270, 20, 60) }, 1, 0 },
    /* his car: sat, his arms out to its levers' tops */
    { 5, KIND_BALL, DB_CharsBallTiny, 55,
      P(0, -180, 280), P(0, -330, 260), P(0, -400, 60),
      PAIR(170, -320, 220), PAIR(260, -270, 150), PAIR(340, -235, 0),
      NONE, NONE, NONE, NONE,
      PAIR(355, -210, 0), NONE, { BOTH(300, -260, -70, 400, 10, 70) }, 1, 0 },
    /* his tank: him from the waist up in its hatch, his fists low in front */
    { 5, KIND_TANK, DB_CharsTankTiny, 55,
      P(0, -340, 20), P(0, -470, 20), P(0, -610, -120),
      PAIR(170, -540, 40), PAIR(190, -420, 150), PAIR(160, -280, -20),
      NONE, NONE, NONE, NONE,
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his race cart: sat in it, his hands on the cockpit's front */
    { 5, KIND_RACE, DB_CharsRaceTiny, 45,
      P(0, -150, 60), P(0, -220, 60), P(0, -280, -120),
      PAIR(130, -230, 50), PAIR(160, -180, 0), PAIR(140, -180, -80),
      NONE, NONE, NONE, NONE,
      PAIR(160, -110, -90), NONE, { BOX(-260, -125, -260, 260, 80, 300) }, 1, 0 },
    /* his fish: astride it, his feet at its sides, his hands down by them */
    { 5, KIND_SPLASH, DB_CharsSplashTiny, 55,
      P(0, -220, -100), P(0, -400, 0), P(0, -560, -20),
      PAIR(170, -450, 40), PAIR(190, -340, 60), PAIR(190, -270, -50),
      PAIR(80, -200, -80), PAIR(150, -180, -170), PAIR(200, -110, -230), NONE,
      NONE, NONE, { BOTH(0, -170, -340, 140, 140, 360), BOTH(140, -10, -340, 350, 140, 360) }, 1, 0 },
    /* his mallet: up in his left hand, his right arm hanging */
    { 5, KIND_MALLET, DB_CharsMalletTiny, 55,
      P(0, -280, 0), P(0, -450, 0), P(0, -580, -120),
      { P(-220, -480, 20), P(200, -490, 30) }, { P(-290, -370, -60), P(260, -340, 60) }, { P(-370, -430, -170), P(240, -210, 0) },
      PAIR(80, -250, 0), PAIR(90, -170, -30), PAIR(110, -60, -20), PAIR(150, -20, -180),
      { P(-360, -480, -170), P(0, 0, 0) }, NONE, { BOX(-460, -1040, -260, -220, -480, 80) }, 1, 0 },
    /* his dino: sat on its back, his feet at its sides, his hands in front */
    { 5, KIND_DINO, DB_CharsDinoTiny, 55,
      P(0, -430, 30), P(0, -620, 60), P(0, -780, -50),
      PAIR(200, -680, 60), PAIR(250, -520, 30), PAIR(220, -430, -80),
      PAIR(90, -400, 0), PAIR(220, -330, -60), PAIR(260, -230, -60), NONE,
      NONE, NONE, { BOTH(0, -400, -520, 120, 10, 420), BOTH(120, -150, -520, 200, 10, 420), BOX(-150, -640, -520, 150, -380, -180) },
      1, 0 },
    /* his keg: under his left arm, his right hanging; his model has one mesh */
    { 5, KIND_KEGS, DB_CharsKegsTiny, 55,
      P(0, -280, 0), P(0, -450, 0), P(0, -580, -120),
      { P(-200, -500, 30), P(200, -490, 30) }, { P(-360, -420, 0), P(270, -340, 60) }, { P(-350, -290, 20), P(250, -200, 0) },
      PAIR(80, -250, 0), PAIR(90, -170, -30), PAIR(110, -60, -20), PAIR(150, -20, -180),
      NONE, NONE, { BOX(-330, -440, -110, -150, -210, 110) }, 1, 1 },
};

const TemplateMap *pack_template_map(s32 hero, s32 kind) {
    u32 i;

    for (i = 0; i < sizeof(Maps) / sizeof(Maps[0]); i++) {
        if (Maps[i].hero == hero && Maps[i].kind == kind) {
            return &Maps[i];
        }
    }
    return NULL;
}
