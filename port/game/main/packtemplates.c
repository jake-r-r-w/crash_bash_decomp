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

    /* -- Coco (2) -- */
    /* her pogo: hands on its handlebar, feet on its pegs */
    { 2, KIND_POGO, DB_CharsPogoCoco, 25,
      P(0, -210, 30), P(0, -300, -20), P(0, -420, -150),
      PAIR(45, -330, -40), PAIR(75, -300, -90), PAIR(85, -345, -150),
      PAIR(30, -200, 30), PAIR(40, -160, 80), PAIR(45, -100, 0), PAIR(50, -75, -40),
      PAIR(95, -345, -150), PAIR(55, -65, -30),
      { BOX(-10, -370, -220, 10, 10, 60), BOX(-120, -360, -180, 120, -330, -120), BOTH(20, -80, -80, 100, -50, 20) }, 1, 0 },
    /* her car: sat, her arms out to its levers' inner ends */
    { 2, KIND_BALL, DB_CharsBallCoco, 30,
      P(0, -170, 100), P(0, -250, 80), P(0, -330, 40),
      PAIR(60, -240, 80), PAIR(150, -230, 60), PAIR(240, -225, 30),
      NONE, NONE, NONE, NONE,
      PAIR(300, -230, 30), NONE, { BOTH(260, -260, -100, 420, 20, 120) }, 1, 0 },
    /* her tank: her head and shoulders in its hatch, her hands low */
    { 2, KIND_TANK, DB_CharsTankCoco, 25,
      P(0, -320, 10), P(0, -380, 0), P(0, -480, -40),
      PAIR(50, -390, -10), PAIR(60, -360, 20), PAIR(45, -340, -40),
      NONE, NONE, NONE, NONE,
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* her race cart: sat in it, her hands on the cockpit's front */
    { 2, KIND_RACE, DB_CharsRaceCoco, 25,
      P(0, -120, 40), P(0, -170, 30), P(0, -260, -20),
      PAIR(50, -175, 20), PAIR(70, -140, -20), PAIR(60, -125, -60),
      NONE, NONE, NONE, NONE,
      PAIR(80, -110, -80), NONE, { BOX(-260, -112, -260, 260, 80, 300) }, 1, 0 },
    /* her fish: astride it, reaching forward */
    { 2, KIND_SPLASH, DB_CharsSplashCoco, 25,
      P(0, -170, -20), P(0, -240, 0), P(0, -380, -30),
      PAIR(40, -260, 0), PAIR(60, -230, -60), PAIR(60, -210, -120),
      PAIR(30, -160, -20), PAIR(70, -100, -70), PAIR(90, -30, -80), NONE,
      NONE, NONE, { BOX(-30, -150, -260, 30, 100, 300), BOX(-230, -20, -260, 230, 100, 300), BOTH(110, -150, -260, 230, 100, 300) }, 1, 0 },
    /* her mallet: its head at her left side in her left hand, her right on her hip */
    { 2, KIND_MALLET, DB_CharsMalletCoco, 25,
      P(0, -230, 0), P(0, -330, 0), P(0, -470, -20),
      PAIR(35, -370, 10), { P(-60, -330, -40), P(80, -300, 40) }, { P(-90, -300, -80), P(40, -280, 20) },
      PAIR(25, -220, 0), PAIR(30, -120, 0), PAIR(40, -20, -10), PAIR(40, -10, -50),
      { P(-120, -320, -90), P(0, 0, 0) }, NONE, { BOX(-340, -620, -200, -100, -270, 300) }, 1, 0 },
    /* her dino: sat on its back, her feet at its sides, the reins in front */
    { 2, KIND_DINO, DB_CharsDinoCoco, 25,
      P(0, -410, 30), P(0, -450, 20), P(0, -560, -20),
      PAIR(40, -470, 0), PAIR(60, -430, -40), PAIR(30, -405, -80),
      PAIR(30, -400, 0), PAIR(110, -330, -40), PAIR(120, -290, -60), NONE,
      NONE, NONE, { BOX(-95, -370, -520, 95, 10, 420), BOX(-130, -620, -520, 130, -380, -170), BOTH(95, -150, -300, 160, 10, 300) },
      1, 0 },
    /* her keg: hugged at her left, her right hand on her hip; it's in her model's third mesh */
    { 2, KIND_KEGS, DB_CharsKegsCoco, 25,
      P(0, -230, -10), P(10, -330, 0), P(0, -470, -20),
      { P(-30, -370, -20), P(40, -370, 10) }, { P(-40, -310, -60), P(60, -310, 20) }, { P(-20, -280, -110), P(30, -280, 10) },
      PAIR(25, -220, 0), PAIR(30, -120, -10), PAIR(40, -20, -10), PAIR(40, -10, -60),
      NONE, NONE, { BOX(-200, -415, -260, -10, -145, 10) }, 1, 3 },

    /* -- Cortex (3) -- */
    /* his pogo: crouched on his hover board, which the game draws: his model is him alone */
    { 3, KIND_POGO, DB_CharsPogoCortex, 25,
      P(0, -200, 0), P(0, -270, -30), P(0, -430, -90),
      PAIR(60, -280, -10), PAIR(110, -240, 20), PAIR(150, -200, 60),
      PAIR(40, -180, 0), PAIR(60, -120, -40), PAIR(80, -80, -60), PAIR(90, -60, -80),
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his car: sat, his arms out to its levers' inner ends */
    { 3, KIND_BALL, DB_CharsBallCortex, 30,
      P(0, -140, 100), P(0, -250, 90), P(0, -430, 0),
      PAIR(70, -265, 90), PAIR(150, -245, 90), PAIR(240, -225, 90),
      NONE, NONE, NONE, NONE,
      PAIR(290, -215, 90), NONE, { BOTH(260, -240, -100, 420, 20, 250) }, 1, 0 },
    /* his tank: in its hatch, his arms down by his sides */
    { 3, KIND_TANK, DB_CharsTankCortex, 25,
      P(0, -310, 30), P(0, -410, 20), P(0, -600, -60),
      PAIR(60, -440, 30), PAIR(80, -380, 60), PAIR(85, -310, 0),
      NONE, NONE, NONE, NONE,
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his race cart: sat in it, reaching forward to its rim */
    { 3, KIND_RACE, DB_CharsRaceCortex, 25,
      P(0, -140, 50), P(0, -180, 40), P(0, -320, -30),
      PAIR(50, -200, 30), PAIR(60, -170, 0), PAIR(50, -155, -60),
      NONE, NONE, NONE, NONE,
      PAIR(80, -120, -80), NONE, { BOX(-260, -125, -260, 260, 80, 300) }, 1, 0 },
    /* his fish: sat on it, his hands down at its sides */
    { 3, KIND_SPLASH, DB_CharsSplashCortex, 25,
      P(0, -150, 40), P(0, -210, 20), P(0, -380, -60),
      PAIR(50, -230, 20), PAIR(90, -190, 20), PAIR(100, -130, 20),
      PAIR(30, -140, 40), PAIR(60, -120, 0), PAIR(70, -100, -30), NONE,
      NONE, NONE, { BOX(-300, -140, -320, 300, 130, 300) }, 1, 0 },
    /* his mallet: across his front, his left hand on its handle */
    { 3, KIND_MALLET, DB_CharsMalletCortex, 25,
      P(0, -110, 0), P(0, -200, -10), P(0, -380, -60),
      PAIR(50, -230, 0), { P(-60, -190, -60), P(60, -190, -50) }, { P(-30, -160, -100), P(40, -150, -90) },
      PAIR(15, -100, 0), PAIR(15, -50, 10), PAIR(20, -10, 0), PAIR(30, -5, -30),
      { P(-40, -150, -110), P(0, 0, 0) }, NONE, { BOX(-340, -240, -280, 130, -90, -60) }, 1, 0 },
    /* his dino: sat on its back, behind its head, his hands up at his sides */
    { 3, KIND_DINO, DB_CharsDinoCortex, 25,
      P(0, -420, 40), P(0, -520, 40), P(0, -710, 40),
      PAIR(60, -560, 40), PAIR(100, -470, 0), PAIR(130, -410, -30),
      PAIR(40, -410, 40), PAIR(100, -330, 0), PAIR(120, -280, -20), NONE,
      NONE, NONE, { BOX(-95, -370, -520, 95, 10, 420), BOX(-130, -620, -520, 130, -380, -170), BOTH(95, -150, -300, 160, 10, 300) },
      1, 0 },
    /* his keg: on his right; his model has one mesh */
    { 3, KIND_KEGS, DB_CharsKegsCortex, 25,
      P(0, -170, -20), P(0, -240, -40), P(0, -380, -60),
      PAIR(60, -280, -20), { P(-80, -200, -20), P(90, -230, -20) }, { P(-60, -140, -30), P(110, -200, -40) },
      PAIR(20, -160, 0), PAIR(25, -90, 0), PAIR(30, -15, 0), PAIR(40, -5, -30),
      NONE, NONE, { BOX(70, -360, -240, 280, -30, 80) }, 1, 1, 2 },

    /* -- Brio (4) -- */
    /* his pogo: him alone, on what the game draws */
    { 4, KIND_POGO, DB_CharsPogoBrio, 25,
      P(0, -270, 0), P(0, -370, -20), P(0, -600, -190),
      PAIR(60, -400, -10), PAIR(100, -330, 30), PAIR(120, -270, 40),
      PAIR(40, -260, 0), PAIR(80, -180, 30), PAIR(100, -100, 10), PAIR(105, -70, 0),
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his car: sat, his arms out to its levers' inner ends */
    { 4, KIND_BALL, DB_CharsBallBrio, 30,
      P(0, -150, 180), P(0, -260, 170), P(0, -480, 110),
      PAIR(50, -260, 170), PAIR(140, -220, 150), PAIR(230, -215, 130),
      NONE, NONE, NONE, NONE,
      PAIR(290, -210, 120), NONE, { BOTH(260, -240, -100, 420, 20, 260) }, 1, 0 },
    /* his tank: in its hatch, reaching forward and down */
    { 4, KIND_TANK, DB_CharsTankBrio, 25,
      P(0, -330, 0), P(0, -420, -10), P(0, -660, -90),
      PAIR(50, -450, -20), PAIR(90, -380, -40), PAIR(100, -310, -90),
      NONE, NONE, NONE, NONE,
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his race cart: his head, neck and small hands above it */
    { 4, KIND_RACE, DB_CharsRaceBrio, 25,
      P(0, -130, 20), P(0, -170, 20), P(0, -330, -30),
      PAIR(40, -180, 10), PAIR(60, -150, -30), PAIR(60, -140, -70),
      NONE, NONE, NONE, NONE,
      PAIR(80, -115, -80), NONE, { BOX(-260, -120, -260, 260, 80, 300) }, 1, 0 },
    /* his fish: sat on it, holding on behind him */
    { 4, KIND_SPLASH, DB_CharsSplashBrio, 25,
      P(0, -160, 20), P(0, -230, 0), P(0, -450, -60),
      PAIR(40, -260, 0), PAIR(70, -200, 60), PAIR(70, -110, 60),
      PAIR(30, -150, 20), PAIR(60, -120, -30), PAIR(80, -90, -60), NONE,
      NONE, NONE, { BOX(-300, -95, -320, 300, 130, 300), BOX(-40, -140, -320, 40, 130, 300) }, 1, 0 },
    /* his mallet: in his right hand, its head behind him */
    { 4, KIND_MALLET, DB_CharsMalletBrio, 25,
      P(0, -160, 0), P(0, -280, 0), P(0, -520, -40),
      PAIR(50, -330, 0), { P(-80, -270, -20), P(90, -300, 60) }, { P(-100, -200, -20), P(110, -180, 150) },
      PAIR(25, -150, 0), PAIR(25, -80, 0), PAIR(25, -15, 0), PAIR(30, -5, -60),
      { P(0, 0, 0), P(115, -200, 150) }, NONE, { BOX(0, -500, 60, 230, -140, 360) }, 1, 0, 2 },
    /* his dino: sat on its back */
    { 4, KIND_DINO, DB_CharsDinoBrio, 25,
      P(0, -420, 0), P(0, -500, 0), P(0, -770, -60),
      PAIR(60, -540, 0), PAIR(110, -470, -30), PAIR(110, -420, -80),
      PAIR(40, -410, 0), PAIR(100, -330, -40), PAIR(130, -270, -60), NONE,
      NONE, NONE, { BOX(-95, -370, -520, 95, 10, 420), BOX(-130, -600, -520, 130, -330, -200), BOTH(95, -150, -300, 170, 10, 300) },
      1, 0 },
    /* his keg: in front of him, more to his right; it's in his model's third mesh */
    { 4, KIND_KEGS, DB_CharsKegsBrio, 25,
      P(0, -130, 20), P(0, -260, 40), P(0, -500, -30),
      PAIR(40, -300, 30), { P(-60, -220, 10), P(80, -240, -20) }, { P(-60, -150, -20), P(220, -180, -60) },
      PAIR(25, -120, 20), PAIR(25, -60, 20), PAIR(25, -15, 20), PAIR(30, -5, -50),
      NONE, NONE, { BOX(-25, -375, -215, 215, -110, 5) }, 1, 3, 2 },

    /* -- Koala Kong (6) -- */
    /* his pogo: on its pegs, nothing to hold, his arms hanging to his fists */
    { 6, KIND_POGO, DB_CharsPogoKong, 50,
      P(0, -240, 60), P(0, -380, 80), P(0, -500, -60),
      PAIR(170, -450, 100), PAIR(260, -300, 120), PAIR(320, -110, 150),
      PAIR(60, -230, 40), PAIR(70, -150, 0), PAIR(80, -60, -20), PAIR(100, -20, -100),
      NONE, PAIR(130, -5, -80), { BOTH(20, -25, -160, 250, 15, 60) }, 1, 0 },
    /* his car: sat, his arms out to its levers' inner ends */
    { 6, KIND_BALL, DB_CharsBallKong, 45,
      P(0, -150, 250), P(0, -280, 250), P(0, -430, 150),
      PAIR(140, -330, 230), PAIR(200, -260, 180), PAIR(250, -215, 120),
      NONE, NONE, NONE, NONE,
      PAIR(300, -210, 60), NONE, { BOTH(270, -240, -100, 420, 20, 250) }, 1, 0 },
    /* his tank: from the waist up, his fists low */
    { 6, KIND_TANK, DB_CharsTankKong, 50,
      P(0, -330, 60), P(0, -480, 60), P(0, -660, -40),
      PAIR(200, -560, 60), PAIR(250, -400, 150), PAIR(210, -260, -30),
      NONE, NONE, NONE, NONE,
      NONE, NONE, { { P(0, 0, 0), P(0, 0, 0), 0 } }, 1, 0 },
    /* his race cart: his fists on its rim */
    { 6, KIND_RACE, DB_CharsRaceKong, 45,
      P(0, -140, 60), P(0, -220, 60), P(0, -310, -60),
      PAIR(140, -250, 60), PAIR(160, -190, 40), PAIR(140, -130, -40),
      NONE, NONE, NONE, NONE,
      PAIR(150, -110, -60), NONE, { BOX(-260, -112, -260, 260, 80, 300) }, 1, 0 },
    /* his fish: astride it, hunched over, his fists at its sides */
    { 6, KIND_SPLASH, DB_CharsSplashKong, 50,
      P(0, -200, 60), P(0, -380, 80), P(0, -560, 60),
      PAIR(200, -450, 80), PAIR(260, -300, 20), PAIR(260, -110, -60),
      PAIR(80, -180, 40), PAIR(160, -130, -40), PAIR(200, -80, -100), NONE,
      NONE, NONE, { BOX(-100, -170, -350, 100, 130, 370), BOX(-330, 0, -350, 330, 130, 370) }, 1, 0 },
    /* his mallet: up in his left hand, his right fist hanging */
    { 6, KIND_MALLET, DB_CharsMalletKong, 50,
      P(0, -300, 0), P(0, -480, 0), P(0, -650, -80),
      PAIR(200, -560, 0), { P(-260, -480, -60), P(260, -400, 30) }, { P(-220, -520, -200), P(240, -200, -40) },
      PAIR(60, -300, 0), PAIR(70, -180, -20), PAIR(80, -60, -20), PAIR(100, -20, -100),
      { P(-220, -540, -220), P(0, 0, 0) }, NONE, { BOX(-360, -960, -380, -110, -600, 280) }, 1, 0 },
    /* his dino: sat on its back, behind its head */
    { 6, KIND_DINO, DB_CharsDinoKong, 50,
      P(0, -500, 60), P(0, -650, 60), P(0, -820, -40),
      PAIR(220, -700, 60), PAIR(270, -560, 0), PAIR(220, -480, -80),
      PAIR(80, -480, 40), PAIR(200, -380, -20), PAIR(240, -240, -40), NONE,
      NONE, NONE, { BOX(-120, -460, -520, 120, 10, 420), BOX(-170, -150, -520, 170, 10, 420), BOX(-150, -680, -520, 150, -420, -170) },
      1, 0 },
    /* his keg: under his left arm, his right fist hanging; it's in his model's second mesh */
    { 6, KIND_KEGS, DB_CharsKegsKong, 50,
      P(0, -330, -40), P(0, -480, -60), P(0, -650, -140),
      PAIR(200, -600, -60), { P(-380, -480, -40), P(290, -440, -40) }, { P(-420, -320, -20), P(240, -200, -120) },
      PAIR(70, -300, -40), PAIR(80, -180, -30), PAIR(90, -60, -20), PAIR(100, -20, -100),
      NONE, NONE, { BOX(-375, -465, -300, -165, -245, 300) }, 1, 2 },
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
