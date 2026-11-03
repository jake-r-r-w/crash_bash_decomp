/* Widescreen shows further than some levels were built to be seen, and they stop short, with nothing there. So a level can have more of its own
 * scenery placed round it, which only a wide view draws, behind everything else: copies of its map's pieces, moved and turned. No art here, just
 * where they go.
 *
 * A level's own piece that was never meant to be seen can go too, in a wide view. A level's floor can be carried on past where it stops too (widesand.c),
 * and a few of a piece's own faces can go with the backdrop, to be in real depth with what's added.
 *
 * To place more: WIDE_LIST=1 prints a level's file number and pieces, WIDE_EXTRA="piece,cx,cz,dx,dy,dz,turn[,placed[,face]];..." tries some without a
 * rebuild, in place of the table, WIDE_HIDE="piece,piece" hides some, and
 * WIDE_RING="floor,wall,walls,y,tile,inner,outer,top,sides,floor_colour,wall_colour" tries a ring. */
#include <math.h>
#include <stdio.h>
#include "common.h"
#include "bash/main.h"
#include "bash/prims.h"
#include "bash/engine.h"
#include "bash/widesand.h"
#include "../../platform/gfx.h"

void draw_map_object(void *sub, void *arg1, s32 flags, void *arg3);
void draw_owner(const struct subengine *sub);
void draw_owner_part(const void *mesh);
extern s32 FarColor[3];

typedef struct {
    s16 file;      /* the map's model file, by its place in DatIndex */
    s16 piece;     /* the map's placement it copies */
    s32 cx, cz;    /* what it turns round */
    s32 dx, dy, dz;
    s32 turn;      /* 0x1000 is all the way round */
    s32 placed;    /* where the level's own things go, rather than behind everything */
    s32 face;      /* it faces what it turns round, so a row of them makes a curve, rather than keeping its own angle */
    s32 entity;    /* shows this of the level's file instead of what the piece does, or 0. A mesh animation's first frame plays it */
    s32 on_sand;   /* it stands on the level's extended floor (widesand.c), with dy from there */
    s32 mirror;    /* it's the mirror image across the line x = cx, after turning, so an end of it meets the same end of the level's own */
    const s16 (*faces)[2]; /* only these of its faces, as runs of their places in its mesh, first and last, or all of them */
    s32 num_faces;
} WideExtra;

#define FILE_N_BALLISM 651
/* its crystal map: other surroundings, and the same front radars in its own colours */
#define FILE_N_BALLISM_CRYSTAL 649
#define FILE_BEACH_BALL 655
#define FILE_DANTES_DASH 637
#define FILE_CRASHBALL 659
#define FILE_JUNGLE_FOX 691
#define FILE_RING_DING 627
#define FILE_POGO_PAINTER 675
#define FILE_EL_POGO_LOCO 667
#define FILE_SKY_BALLS 647
#define DRAW_UNCULLED 0x1000000
#define FILE_PAPU_PUMMEL 697
#define FILE_DESERT_FOX 693
/* its crystal map, the same island and far dunes with a pool in the middle: the same fix */
#define FILE_DESERT_FOX_CRYSTAL 683
/* Desert Fox's panorama of far dunes and sky, on part of a circle round this, its two ends each other's mirror image across the line through it, so turned
   by the span between them a copy's first end is at its last. Its tiles make a loop, so they go on in their order. A little less, so the level's own covers
   the join */
#define DESERT_FOX_PANORAMA 6
#define DESERT_FOX_PANORAMA_X 1873
#define DESERT_FOX_PANORAMA_Z 358
#define DESERT_FOX_PANORAMA_SPAN 1203
#define FILE_BEARMINATOR 472
#define FILE_TOXIC_DASH 639
#define FILE_POLAR_PANIC 478
/* Polar Panic's misty band along the horizon, which stops at the sea's two ends: its mirror image across each, a little inside it, so the level's own
   covers the join */
#define POLAR_HORIZON 52
#define POLAR_HORIZON_LEFT_END (-10132 + 30)
#define POLAR_HORIZON_RIGHT_END (10221 - 30)
/* and its stars, each a piece of its own, spread only as wide as a 4:3 view sees: copies of them all moved this far each way, a little more than they
   spread, so they don't meet. Mirror images would pair off across the line between, there being so few */
#define POLAR_STAR 43 /* and the seven after it */
#define POLAR_STARS_ACROSS 14000
#define FILE_TILT_PANIC 486
#define FILE_OXIDE 613
/* The second part of Oxide's fight is in an arena in his ship, its towers round the back of its floor, out of the dark. A wide view sees past them at the
   sides, into the dark, so copies of them, without the floor, stand on out at each side */
#define OXIDE_ARENA 10
#define OXIDE_TOWERS_ACROSS 13000
static const s16 OxideTowers[][2] = { { 0, 41 }, { 73, 78 }, { 84, 217 }, { 382, 877 } };
#define NUM_OXIDE_TOWERS ((s32)(sizeof(OxideTowers) / sizeof(OxideTowers[0])))
/* The crystal challenges' maps, a level's other map (its crystal flag, levelRecAlt), are files of their own */
#define FILE_JUNGLE_BASH_CRYSTAL 713
/* Jungle Bash's crystal map, at night: its backdrop of dark jungle is two walls on part of a circle round this, either side of the temple, and it stops at
   their far ends, where a wide view looks out past them. Its mirror images across each end, a little inside it, carry it on round */
#define JUNGLE_BASH_CRYSTAL_BACKDROP 2
#define JUNGLE_BASH_CRYSTAL_X 61
#define JUNGLE_BASH_CRYSTAL_Z (-4010)
#define JUNGLE_BASH_CRYSTAL_LEFT_MIRROR 965
#define JUNGLE_BASH_CRYSTAL_RIGHT_MIRROR (-971)
#define FILE_POGO_PAINTER_CRYSTAL 673
/* Pogo Painter's crystal map, at night, is a ruined garden with its far backdrop the night sky, a wall on part of a wide circle round this, and the ruins'
   black outline in front of it, which both stop at the sky's ends. Their mirror images across each end, a little inside it, carry them on round */
#define POGO_PAINTER_CRYSTAL_SKY 6
#define POGO_PAINTER_CRYSTAL_RUINS 7
#define POGO_PAINTER_CRYSTAL_X 396
#define POGO_PAINTER_CRYSTAL_Z (-31820)
#define POGO_PAINTER_CRYSTAL_LEFT_MIRROR 328
#define POGO_PAINTER_CRYSTAL_RIGHT_MIRROR (-326)
#define FILE_POGO_PADLOCK_CRYSTAL 661
/* Pogo Padlock's crystal map is a back alley at night, its brick wall along the back with the pavement at its foot, and nothing past them at the sides but
   the night. The wall goes on at each side as copies of a stretch of it, a panel of its own, edge to edge, and the pavement out to their foot (widesand.c)
   in the road's dark. On the right the copies start where the level's own wall ends, and as low as it is */
#define POGO_PADLOCK_CRYSTAL_ALLEY 64
#define POGO_PADLOCK_CRYSTAL_PAVEMENT_IMAGE 7
#define POGO_PADLOCK_CRYSTAL_DARK 0x292121
#define POGO_PADLOCK_CRYSTAL_WALL 83
#define POGO_PADLOCK_CRYSTAL_WALL_SPAN 1532
#define POGO_PADLOCK_CRYSTAL_RIGHT_START 10719
#define POGO_PADLOCK_CRYSTAL_RIGHT_DROP 235
#define FILE_BEACH_BALL_CRYSTAL 653
/* Beach Ball's crystal map is a raft on the sea before a floating city, with a wall of sky along the far side of the sea, a part of the sea's own piece,
   and the sea stepped at its sides. The sea is carried on out at the sides (widesand.c), and the sky by its mirror images across each end, square to it
   there and a little inside it */
#define BEACH_BALL_CRYSTAL_SEA 0
#define BEACH_BALL_CRYSTAL_SEA_IMAGE 0
#define BEACH_BALL_CRYSTAL_FAR_SEA_IMAGE 1
#define BEACH_BALL_CRYSTAL_FAR_SEA 0x9C7B63
static const s16 BeachBallCrystalSky[][2] = { { 83, 102 } };
#define BEACH_BALL_CRYSTAL_LEFT_X (-10231)
#define BEACH_BALL_CRYSTAL_LEFT_Z 11253
#define BEACH_BALL_CRYSTAL_LEFT_MIRROR 118
#define BEACH_BALL_CRYSTAL_RIGHT_X 10222
#define BEACH_BALL_CRYSTAL_RIGHT_Z 11063
#define BEACH_BALL_CRYSTAL_RIGHT_MIRROR (-121)
#define FILE_SKY_BALLS_CRYSTAL 645
/* Sky Balls' crystal map is the arena over a city at night, its rooftops a wide stretch below, wider in front than at the back, so past its slanting edges
   there was only the night. Its mirror images across those edges, a little inside them, carry it on */
#define SKY_BALLS_CRYSTAL_CITY 1
#define SKY_BALLS_CRYSTAL_LEFT_X (-15559)
#define SKY_BALLS_CRYSTAL_LEFT_Z 21690
#define SKY_BALLS_CRYSTAL_LEFT_MIRROR (-1042)
#define SKY_BALLS_CRYSTAL_RIGHT_X 15002
#define SKY_BALLS_CRYSTAL_RIGHT_Z 21752
#define SKY_BALLS_CRYSTAL_RIGHT_MIRROR 1048
/* Tilt Panic's sky, a wall at the back whose two ends are alike, from the haze at its foot up to blue: its mirror image across each, a little inside it */
#define TILT_SKY 1
#define TILT_SKY_LEFT_END (-10708 + 30)
#define TILT_SKY_RIGHT_END (10516 - 30)
#define FILE_TILT_PANIC_CRYSTAL 476
/* Tilt Panic's crystal map is in a storm: a dome of storm clouds turning round it, its foot above the sea, and the sea stopping short of it at the sides,
   where the clear colour, white, showed under the dome's foot. So the sea is carried on out to past the dome (widesand.c), and seen under its foot */
#define TILT_CRYSTAL_SEA 2
#define TILT_CRYSTAL_SEA_IMAGE 22
#define TILT_CRYSTAL_SEA_COLOUR 0x6B4231
/* and the dark band along its horizon, between the sea and the dome's foot, which stops at the sea's ends: its mirror images across each end, square to it
   there and a little inside it */
#define TILT_CRYSTAL_HORIZON 1
#define TILT_CRYSTAL_HORIZON_LEFT_X (-11188)
#define TILT_CRYSTAL_HORIZON_LEFT_Z 11222
#define TILT_CRYSTAL_HORIZON_LEFT_MIRROR 214
#define TILT_CRYSTAL_HORIZON_RIGHT_X 11263
#define TILT_CRYSTAL_HORIZON_RIGHT_Z 11235
#define TILT_CRYSTAL_HORIZON_RIGHT_MIRROR (-212)
#define FILE_POLAR_PANIC_CRYSTAL 456
/* Polar Panic's crystal map, at sunset, has Polar Panic's sea, and a wall of sunset sky behind it that stops at the sea's ends, straight across: the sea is
   carried on as the normal map's is, and the sky by its mirror images across each end, a little inside it */
#define POLAR_CRYSTAL_SKY 43
#define POLAR_CRYSTAL_SKY_LEFT_END (-10782 + 30)
#define POLAR_CRYSTAL_SKY_RIGHT_END (10442 - 30)
#define POLAR_CRYSTAL_SUNSET 0x42ADC6
/* The Bearminator's sea, the mountains round the back of it and the sky behind them, a wall whose two ends are each other's mirror image across the line
   x = 40, so turned round the middle of the arena, on that line, by the span between them as seen from there, a copy's first end is at its last. The sea
   and the mountains turn with it, so they stay in front of it, at its foot. A little less than the span, so the level's own covers the join */
#define BEAR_SEA 0
#define BEAR_SKY 1
#define BEAR_MOUNTAINS 3
#define BEAR_X 40
#define BEAR_SPAN 950
/* Papu Pummel's sunset, a wall that's a part of a circle round this, seen from above, and its far hills, which reach round it as far */
#define PAPU_SKY 51
#define PAPU_HILLS 53
#define PAPU_SKY_X 365
#define PAPU_SKY_Z (-10738)
/* turns that make the mirror images across each end of it, a little inside them, and the turn two of them make */
#define PAPU_SKY_LEFT_MIRROR 532
#define PAPU_SKY_RIGHT_MIRROR (-513)
#define PAPU_SKY_TWICE 1045
/* Sky Balls' far fields, the fields under the arena and the wall of sky at the end of them: one model each, and the same place */
#define SKY_BALLS_NEAR 12
#define SKY_BALLS_SKY 13
#define SKY_BALLS_FAR 14
/* the lines its mirror images are across: the sky's two ends, a little inside them, so the level's own covers the join, where edge to edge a pixel showed
   through now and then */
#define SKY_BALLS_LEFT_END (-15580 + 30)
#define SKY_BALLS_RIGHT_END (15023 - 30)
/* two mirror images are the level's own moved this far */
#define SKY_BALLS_TWICE (2 * (SKY_BALLS_RIGHT_END - SKY_BALLS_LEFT_END))

static const WideExtra Extras[] = {
    /* kelp and coral on the extra sand, the level's own, in its own order: its fish is left out */
    { FILE_BEACH_BALL, 0, 0, 0, -5641, -20, 2999, 0, 1, 1, 0x4100, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5258, -20, 2124, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -4829, -20, 1115, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5315, -20, 93, 0, 1, 1, 0x4200, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5883, -20, -722, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -5072, -20, -894, 0, 1, 1, 0x4300, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, -6477, -20, -2358, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5970, -20, 3175, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5888, -20, 1144, 0, 1, 1, 0x4200, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 6551, -20, 0, 0, 1, 1, 0x4180, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5209, -20, 517, 0, 1, 1, 0x4100, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 5292, -20, -1070, 0, 1, 1, 0x4280, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 4909, -20, -317, 0, 1, 1, 0x4300, 1 },
    { FILE_BEACH_BALL, 0, 0, 0, 6344, -20, -2309, 0, 1, 1, 0x4180, 1 },
    /* the swamp grass, on round the edge of the lava, close enough to make a row, and facing in so it curves */
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -100, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -100, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -200, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -200, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -300, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -300, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -400, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -400, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -500, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -500, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -600, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -600, 0, 1 },
    { FILE_DANTES_DASH, 15, 0, 0, 0, 0, 0, -700, 0, 1 },
    { FILE_DANTES_DASH, 16, 0, 0, 0, 0, 0, -700, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 80, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 80, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 160, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 160, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 240, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 240, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 320, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 320, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 400, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 400, 0, 1 },
    { FILE_DANTES_DASH, 6, 0, 0, 0, 0, 0, 480, 0, 1 },
    { FILE_DANTES_DASH, 7, 0, 0, 0, 0, 0, 480, 0, 1 },
    /* the canopy carried on into the top corners: the level's own leaf clusters, out past the floor's edge, where nothing's under them */
    { FILE_JUNGLE_FOX, 5, 0, 0, -1400, 0, -500, 0 },
    { FILE_JUNGLE_FOX, 8, 0, 0, -1900, 0, -1500, 0 },
    { FILE_JUNGLE_FOX, 4, 0, 0, -1800, 0, -300, 0 },
    { FILE_JUNGLE_FOX, 9, 0, 0, 1300, 0, -1000, 0 },
    { FILE_JUNGLE_FOX, 6, 0, 0, 3300, 0, -2000, 0 },
    { FILE_JUNGLE_FOX, 9, 0, 0, 2200, 0, -2300, 0 },
    /* the grey gear under the platform on the left, on the right too, where it's black: it's in the level's order, as the platform has to come in front */
    { FILE_RING_DING, 4, 0, 0, 4712, 0, 0, 0, 1 },
    /* the far wall of mesas and the sky behind it, side by side with copies of themselves: each one's two ends are alike, so they meet. A little overlap so no
       crack shows, where the level's own covers it */
    { FILE_EL_POGO_LOCO, 70, 0, 0, -21169, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 70, 0, 0, 21169, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 70, 0, 0, -42338, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 70, 0, 0, 42338, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 72, 0, 0, -26103, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 72, 0, 0, 26103, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 72, 0, 0, -52206, 0, 0, 0 },
    { FILE_EL_POGO_LOCO, 72, 0, 0, 52206, 0, 0, 0 },
    /* the sky and the fields carried on at the sides: their mirror images across each end of the sky, so the sky's shading meets its own, and beyond them
       the level's own again. The fields' far edge is the sky's foot, so they all move as one, and the fields' scroll runs towards the camera, the same in a
       mirror image */
    { FILE_SKY_BALLS, SKY_BALLS_SKY, SKY_BALLS_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_FAR, SKY_BALLS_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_NEAR, SKY_BALLS_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_SKY, SKY_BALLS_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_FAR, SKY_BALLS_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_NEAR, SKY_BALLS_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS, SKY_BALLS_SKY, 0, 0, -SKY_BALLS_TWICE, 0, 0, 0 },
    { FILE_SKY_BALLS, SKY_BALLS_FAR, 0, 0, -SKY_BALLS_TWICE, 0, 0, 0 },
    { FILE_SKY_BALLS, SKY_BALLS_NEAR, 0, 0, -SKY_BALLS_TWICE, 0, 0, 0 },
    { FILE_SKY_BALLS, SKY_BALLS_SKY, 0, 0, SKY_BALLS_TWICE, 0, 0, 0 },
    { FILE_SKY_BALLS, SKY_BALLS_FAR, 0, 0, SKY_BALLS_TWICE, 0, 0, 0 },
    { FILE_SKY_BALLS, SKY_BALLS_NEAR, 0, 0, SKY_BALLS_TWICE, 0, 0, 0 },
    /* the sunset carried on round, on its own circle: its mirror images across each end, as its ends are shaded differently, and beyond them the level's own
       turned on round. The hills go the same way, so they stay in front of it */
    { FILE_PAPU_PUMMEL, PAPU_SKY, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_PAPU_PUMMEL, PAPU_SKY, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_PAPU_PUMMEL, PAPU_SKY, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, -PAPU_SKY_TWICE },
    { FILE_PAPU_PUMMEL, PAPU_SKY, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_TWICE },
    { FILE_PAPU_PUMMEL, PAPU_HILLS, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_PAPU_PUMMEL, PAPU_HILLS, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_PAPU_PUMMEL, PAPU_HILLS, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, -PAPU_SKY_TWICE },
    { FILE_PAPU_PUMMEL, PAPU_HILLS, PAPU_SKY_X, PAPU_SKY_Z, 0, 0, 0, PAPU_SKY_TWICE },
    { FILE_DESERT_FOX, DESERT_FOX_PANORAMA, DESERT_FOX_PANORAMA_X, DESERT_FOX_PANORAMA_Z, 0, 0, 0, DESERT_FOX_PANORAMA_SPAN },
    { FILE_DESERT_FOX, DESERT_FOX_PANORAMA, DESERT_FOX_PANORAMA_X, DESERT_FOX_PANORAMA_Z, 0, 0, 0, -DESERT_FOX_PANORAMA_SPAN },
    { FILE_DESERT_FOX_CRYSTAL, DESERT_FOX_PANORAMA, DESERT_FOX_PANORAMA_X, DESERT_FOX_PANORAMA_Z, 0, 0, 0, DESERT_FOX_PANORAMA_SPAN },
    { FILE_DESERT_FOX_CRYSTAL, DESERT_FOX_PANORAMA, DESERT_FOX_PANORAMA_X, DESERT_FOX_PANORAMA_Z, 0, 0, 0, -DESERT_FOX_PANORAMA_SPAN },
    { FILE_BEARMINATOR, BEAR_SEA, BEAR_X, 0, 0, 0, 0, BEAR_SPAN },
    { FILE_BEARMINATOR, BEAR_SEA, BEAR_X, 0, 0, 0, 0, -BEAR_SPAN },
    { FILE_BEARMINATOR, BEAR_MOUNTAINS, BEAR_X, 0, 0, 0, 0, BEAR_SPAN },
    { FILE_BEARMINATOR, BEAR_MOUNTAINS, BEAR_X, 0, 0, 0, 0, -BEAR_SPAN },
    { FILE_BEARMINATOR, BEAR_SKY, BEAR_X, 0, 0, 0, 0, BEAR_SPAN },
    { FILE_BEARMINATOR, BEAR_SKY, BEAR_X, 0, 0, 0, 0, -BEAR_SPAN },
    { FILE_POLAR_PANIC, POLAR_HORIZON, POLAR_HORIZON_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_POLAR_PANIC, POLAR_HORIZON, POLAR_HORIZON_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_POLAR_PANIC, POLAR_STAR, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 1, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 2, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 3, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 4, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 5, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 6, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 7, 0, 0, -POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 1, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 2, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 3, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 4, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 5, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 6, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_POLAR_PANIC, POLAR_STAR + 7, 0, 0, POLAR_STARS_ACROSS, 0, 0, 0 },
    { FILE_TILT_PANIC, TILT_SKY, TILT_SKY_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_TILT_PANIC, TILT_SKY, TILT_SKY_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_OXIDE, OXIDE_ARENA, 0, 0, -OXIDE_TOWERS_ACROSS, 0, 0, 0, 0, 0, 0, 0, 0, OxideTowers, NUM_OXIDE_TOWERS },
    { FILE_OXIDE, OXIDE_ARENA, 0, 0, OXIDE_TOWERS_ACROSS, 0, 0, 0, 0, 0, 0, 0, 0, OxideTowers, NUM_OXIDE_TOWERS },
    { FILE_JUNGLE_BASH_CRYSTAL, JUNGLE_BASH_CRYSTAL_BACKDROP, JUNGLE_BASH_CRYSTAL_X, JUNGLE_BASH_CRYSTAL_Z, 0, 0, 0, JUNGLE_BASH_CRYSTAL_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_JUNGLE_BASH_CRYSTAL, JUNGLE_BASH_CRYSTAL_BACKDROP, JUNGLE_BASH_CRYSTAL_X, JUNGLE_BASH_CRYSTAL_Z, 0, 0, 0, JUNGLE_BASH_CRYSTAL_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POGO_PAINTER_CRYSTAL, POGO_PAINTER_CRYSTAL_SKY, POGO_PAINTER_CRYSTAL_X, POGO_PAINTER_CRYSTAL_Z, 0, 0, 0, POGO_PAINTER_CRYSTAL_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POGO_PAINTER_CRYSTAL, POGO_PAINTER_CRYSTAL_SKY, POGO_PAINTER_CRYSTAL_X, POGO_PAINTER_CRYSTAL_Z, 0, 0, 0, POGO_PAINTER_CRYSTAL_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POGO_PAINTER_CRYSTAL, POGO_PAINTER_CRYSTAL_RUINS, POGO_PAINTER_CRYSTAL_X, POGO_PAINTER_CRYSTAL_Z, 0, 0, 0, POGO_PAINTER_CRYSTAL_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POGO_PAINTER_CRYSTAL, POGO_PAINTER_CRYSTAL_RUINS, POGO_PAINTER_CRYSTAL_X, POGO_PAINTER_CRYSTAL_Z, 0, 0, 0, POGO_PAINTER_CRYSTAL_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -1 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -2 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -3 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -4 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -5 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, -6 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, 0, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 0 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 1 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 2 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 3 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 4 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_POGO_PADLOCK_CRYSTAL, POGO_PADLOCK_CRYSTAL_WALL, 0, 0, POGO_PADLOCK_CRYSTAL_RIGHT_START + 5 * POGO_PADLOCK_CRYSTAL_WALL_SPAN, POGO_PADLOCK_CRYSTAL_RIGHT_DROP, 0, 0 },
    { FILE_BEACH_BALL_CRYSTAL, BEACH_BALL_CRYSTAL_SEA, BEACH_BALL_CRYSTAL_LEFT_X, BEACH_BALL_CRYSTAL_LEFT_Z, 0, 0, 0, BEACH_BALL_CRYSTAL_LEFT_MIRROR, 0, 0, 0, 0, 1, BeachBallCrystalSky, 1 },
    { FILE_BEACH_BALL_CRYSTAL, BEACH_BALL_CRYSTAL_SEA, BEACH_BALL_CRYSTAL_RIGHT_X, BEACH_BALL_CRYSTAL_RIGHT_Z, 0, 0, 0, BEACH_BALL_CRYSTAL_RIGHT_MIRROR, 0, 0, 0, 0, 1, BeachBallCrystalSky, 1 },
    { FILE_SKY_BALLS_CRYSTAL, SKY_BALLS_CRYSTAL_CITY, SKY_BALLS_CRYSTAL_LEFT_X, SKY_BALLS_CRYSTAL_LEFT_Z, 0, 0, 0, SKY_BALLS_CRYSTAL_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_SKY_BALLS_CRYSTAL, SKY_BALLS_CRYSTAL_CITY, SKY_BALLS_CRYSTAL_RIGHT_X, SKY_BALLS_CRYSTAL_RIGHT_Z, 0, 0, 0, SKY_BALLS_CRYSTAL_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_POLAR_PANIC_CRYSTAL, POLAR_CRYSTAL_SKY, POLAR_CRYSTAL_SKY_LEFT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_POLAR_PANIC_CRYSTAL, POLAR_CRYSTAL_SKY, POLAR_CRYSTAL_SKY_RIGHT_END, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { FILE_TILT_PANIC_CRYSTAL, TILT_CRYSTAL_HORIZON, TILT_CRYSTAL_HORIZON_LEFT_X, TILT_CRYSTAL_HORIZON_LEFT_Z, 0, 0, 0, TILT_CRYSTAL_HORIZON_LEFT_MIRROR, 0, 0, 0, 0, 1 },
    { FILE_TILT_PANIC_CRYSTAL, TILT_CRYSTAL_HORIZON, TILT_CRYSTAL_HORIZON_RIGHT_X, TILT_CRYSTAL_HORIZON_RIGHT_Z, 0, 0, 0, TILT_CRYSTAL_HORIZON_RIGHT_MIRROR, 0, 0, 0, 0, 1 },
};
#define NUM_EXTRAS ((s32)(sizeof(Extras) / sizeof(Extras[0])))

/* What a wide view is better without: */
static const struct {
    s16 file, piece;
} Hidden[] = {
    /* the front radars' bases and rings, which go with them (draw_ballism_radars) */
    { FILE_N_BALLISM, 3 },
    { FILE_N_BALLISM, 4 },
    { FILE_N_BALLISM, 7 },
    { FILE_N_BALLISM, 8 },
    /* the floor under those bases, which sorts in front of them in places: it goes with the backdrop instead (BallismFloors) */
    { FILE_N_BALLISM, 9 },
    { FILE_N_BALLISM, 10 },
    /* the same in its crystal map */
    { FILE_N_BALLISM_CRYSTAL, 3 },
    { FILE_N_BALLISM_CRYSTAL, 4 },
    { FILE_N_BALLISM_CRYSTAL, 7 },
    { FILE_N_BALLISM_CRYSTAL, 8 },
    { FILE_N_BALLISM_CRYSTAL, 9 },
    { FILE_N_BALLISM_CRYSTAL, 10 },
    /* a shadow cast over the turning cog on the right, by taking light away from what's drawn before it. The cog's faces come before or after it by
       where each one is as it turns, so they flickered in and out of it, out where a 4:3 view doesn't reach */
    { FILE_RING_DING, 13 },
};
#define NUM_HIDDEN ((s32)(sizeof(Hidden) / sizeof(Hidden[0])))

#define MAX_TRIED_HIDDEN 32
static s16 TriedHidden[MAX_TRIED_HIDDEN];
static s32 NumTriedHidden = -1;

/* A ring round the arena, made of the level's own tiles at their proper size: a floor, and a wall that curves round it. */
typedef struct {
    s16 file;
    s16 floor_image;
    s16 wall_image, wall_images; /* a panorama: these tiles in turn, all the way round */
    s32 floor_y, floor_tile;     /* where the floor is, and how big a tile of it */
    s32 inner, outer;            /* the floor's radius */
    s32 wall_top;
    s32 sides;
    u32 floor_colour, wall_colour;
} WideRing;

static const WideRing Rings[] = {
    /* only a wall, as the lava's fine: its own forest panorama, at the colours of the trees in front */
    { FILE_DANTES_DASH, 0, 14, 4, 3000, 600, 7000, 7000, -4000, 20, 0, 0x0A3A60 },
};
#define NUM_RINGS ((s32)(sizeof(Rings) / sizeof(Rings[0])))

static WideRing TriedRing;
static subengine RingOwner;
static s32 RingTried = -1;

#define MAX_TRIED 32
#define MAX_COPIES (NUM_EXTRAS + MAX_TRIED)

static WideExtra Tried[MAX_TRIED];
static s32 NumTried = -1;
static subengine Copies[MAX_COPIES];

static void read_tried(void) {
    const char *s = getenv("WIDE_EXTRA");

    NumTried = 0;
    while (s != NULL && *s != 0 && NumTried < MAX_TRIED) {
        WideExtra *e = &Tried[NumTried];
        int piece, cx, cz, dx, dy, dz, turn, used = 0, placed = 0, more = 0;

        if (sscanf(s, "%d,%d,%d,%d,%d,%d,%d%n", &piece, &cx, &cz, &dx, &dy, &dz, &turn, &used) != 7) {
            break;
        }
        e->file = -1;
        e->piece = (s16)piece;
        e->cx = cx;
        e->cz = cz;
        e->dx = dx;
        e->dy = dy;
        e->dz = dz;
        e->turn = turn;
        s += used;
        if (sscanf(s, ",%d%n", &placed, &more) == 1) {
            s += more;
        }
        e->placed = placed;
        e->face = 0;
        e->entity = 0;
        if (sscanf(s, ",%d%n", &placed, &more) == 1) {
            s += more;
            e->face = placed;
        }
        if (sscanf(s, ",%x%n", &placed, &more) == 1) {
            s += more;
            e->entity = placed;
        }
        NumTried++;
        if (*s == ';') {
            s++;
        }
    }
}

static void list_pieces(const map_enviroment *scene) {
    static const void *listed;
    s32 i;

    if (listed == scene->subs || getenv("WIDE_LIST") == NULL) {
        return;
    }
    listed = scene->subs;
    for (i = 0; i < scene->num_placements; i++) {
        const subengine *p = &scene->subs[i];

        printf("file %d piece %d entity %x at %d,%d,%d otz %d\n", (int)(scene->dbase->file - DatIndex), i, p->entity, p->XYZ.x, p->XYZ.y, p->XYZ.z, (int)p->otz);
    }
    fflush(stdout);
}

/* A mesh animation's first frame plays on with the game's clock, as the level's own objects do, and stops when it's paused. Each starts somewhere else. */
static s32 extra_frame(const map_enviroment *scene, s32 entity, s32 which) {
    ModelFile *m = (ModelFile *)scene->dbase->data;
    s32 anim = (entity & 0xF80) >> 7;

    if ((entity & 0x7000) == 0x4000 && (entity & 0x7F) == 0 && anim < m->nMeshAnims) {
        const FileEntry *list = (const FileEntry *)((const u8 *)&m->meshAnims + m->meshAnims);

        if (list[anim].frames > 0) {
            return entity + (GameFrameCount + which * 11) % list[anim].frames;
        }
    }
    return entity;
}

/* Between the floor's order and the arena's (the map's placements have 768 and 256), so the floor doesn't cover what stands on the sand beside it, and the arena still does. */
#define EXTRA_ON_SAND_OTZ 512

static void draw_extra(const map_enviroment *scene, const WideExtra *e, subengine *copy, MATRIX *view) {
    const subengine *from;
    MATRIX turned;
    MATRIX spin;
    float angle = (float)e->turn * 6.28318530718f / 4096.0f;
    s16 c = (s16)(cosf(angle) * 4096.0f), s = (s16)(sinf(angle) * 4096.0f);
    s32 x, z;

    if (e->piece < 0 || e->piece >= scene->num_placements) {
        return;
    }
    from = &scene->subs[e->piece];
    *copy = *from;
    copy->next = NULL;
    /* the game's own flag for a model drawn whatever its bounding box says (draw_object_model): the box's test goes by a sphere round its middle, and a big
       piece moved out to the side has its middle off the screen while its near end is on it */
    copy->flags |= DRAW_UNCULLED;
    if (e->entity != 0) {
        copy->entity = (u16)extra_frame(scene, e->entity, (s32)(copy - Copies));
    }
    x = from->XYZ.x - e->cx;
    z = from->XYZ.z - e->cz;
    copy->XYZ.x = e->cx + (e->mirror ? -1 : 1) * (x * c + z * s) / 0x1000 + e->dx;
    copy->XYZ.y = from->XYZ.y + e->dy;
    copy->XYZ.z = e->cz + (z * c - x * s) / 0x1000 + e->dz;
    if (e->on_sand) {
        float y;

        if (!wide_sand_height((float)(copy->XYZ.x - from->XYZ.x), (float)(copy->XYZ.z - from->XYZ.z), &y)) {
            return;
        }
        copy->XYZ.y = from->XYZ.y + (s32)y + e->dy;
        copy->otz = EXTRA_ON_SAND_OTZ;
    }
    if (e->face) {
        angle = atan2f((float)(copy->XYZ.x - e->cx), (float)(copy->XYZ.z - e->cz));
        c = (s16)(cosf(angle) * 4096.0f);
        s = (s16)(sinf(angle) * 4096.0f);
    }
    memset(&spin, 0, sizeof(spin));
    spin.m[0][0] = c;
    spin.m[0][2] = s;
    spin.m[1][1] = 0x1000;
    spin.m[2][0] = (s16)-s;
    spin.m[2][2] = c;
    if (e->mirror) {
        spin.m[0][0] = (s16)-c;
        spin.m[0][2] = (s16)-s;
    }
    matrix_mul(&spin, (MATRIX *)&from->xform.mtx, &turned);
    copy->xform.mtx = turned;
    draw_behind(!e->placed);
    draw_mirrored(e->mirror);
    draw_owner(copy);
    if (e->faces != NULL) {
        draw_face_list(e->faces, e->num_faces, 1);
    }
    copy->hook(copy, view, 0);
    draw_face_list(NULL, 0, 0);
    draw_owner(NULL);
    draw_mirrored(0);
    draw_behind(0);
}

/* The first part of Oxide's fight is a flight through asteroids, over a purple haze, a backdrop of the level's that's always the same in front of the
   camera (DrawSceneMarks). The haze stops a little past where a 4:3 view does, with a straight edge, so it's carried on out at the sides to past where any
   view reaches, from the colours its edge has, from its foot up to where it goes from purple to black, fading on as the haze does towards its sides. Its
   edge is where it is in the view. The haze is all at one depth and faces the camera, so a point of the view at that depth lands on the screen a straight
   step from two of the haze's own points, whatever the scale its model is drawn at */
#define OXIDE_HAZE 0x201F
#define OXIDE_HAZE_Z 21639
#define OXIDE_HAZE_ROWS 3
#define OXIDE_HAZE_COLUMNS 4
static const s32 OxideHazeEdge[2][OXIDE_HAZE_ROWS][2] = {
    { { -18298, 6610 }, { -18506, 1527 }, { -20842, -1661 } },
    { { 18934, 6609 }, { 18939, 1353 }, { 20803, -1913 } },
};
static const u32 OxideHazeRowColour[OXIDE_HAZE_ROWS] = { 0x390021, 0x390021, 0 };
/* each column's way out from the edge, and how much of the edge's colour it has, of 256 */
static const s32 OxideHazeColumnOut[OXIDE_HAZE_COLUMNS] = { 0, 12000, 28000, 52000 };
static const s32 OxideHazeColumnKeep[OXIDE_HAZE_COLUMNS] = { 256, 170, 90, 40 };

static u32 haze_colour(u32 c, s32 keep) {
    u32 out = 0;
    s32 n;

    for (n = 0; n < 3; n++) {
        out |= (((c >> (n * 8)) & 0xFF) * (u32)keep / 256) << (n * 8);
    }
    /* the screen's fade, as the game's own models have it */
    return RenderState.modelDp != 0 ? colour_fade(&out, FarColor, RenderState.modelDp) : out;
}

void wide_scenery_mark(const subengine *mark) {
    ViewPoint a, b, q[OXIDE_HAZE_COLUMNS][OXIDE_HAZE_ROWS];
    u32 colour[OXIDE_HAZE_COLUMNS][OXIDE_HAZE_ROWS];
    DrawFace f;
    float kx, ky, cx, cy;
    s32 side, col, row, layer, face = 0;
    s16 pa[4] = { 0, 0, OXIDE_HAZE_Z, 0 }, pb[4] = { 8192, 8192, OXIDE_HAZE_Z, 0 };

    if (gfx_view_extra() <= 0 || mark->entity != OXIDE_HAZE || mark->u6C.dbase == NULL || (s32)(mark->u6C.dbase->file - DatIndex) != FILE_OXIDE) {
        return;
    }
    project_point(pa, &a);
    project_point(pb, &b);
    /* not as it was measured: leave it as it is */
    if (a.view[2] != OXIDE_HAZE_Z || b.view[2] != OXIDE_HAZE_Z || b.view[0] == a.view[0] || b.view[1] == a.view[1]) {
        return;
    }
    kx = (b.x - a.x) / (float)(b.view[0] - a.view[0]);
    ky = (b.y - a.y) / (float)(b.view[1] - a.view[1]);
    cx = a.x - kx * (float)a.view[0];
    cy = a.y - ky * (float)a.view[1];
    /* its place in the order is the haze's own, as all of the haze is at the one depth */
    layer = RenderState.layer + (s32)((points_order(&a, &a, &a) + RenderState.otzAdd) >> 1);
    if (layer - RenderState.layer >= RenderState.otLen) {
        return;
    }
    memset(&f, 0, sizeof(f));
    draw_owner(mark);
    for (side = 0; side < 2; side++) {
        s32 out = side ? 1 : -1;

        draw_owner_part(OxideHazeEdge[side]);
        for (col = 0; col < OXIDE_HAZE_COLUMNS; col++) {
            for (row = 0; row < OXIDE_HAZE_ROWS; row++) {
                s32 x = OxideHazeEdge[side][row][0] + out * OxideHazeColumnOut[col], y = OxideHazeEdge[side][row][1];

                q[col][row] = a;
                q[col][row].view[0] = x;
                q[col][row].view[1] = y;
                q[col][row].x = cx + kx * (float)x;
                q[col][row].y = cy + ky * (float)y;
                colour[col][row] = haze_colour(OxideHazeRowColour[row], OxideHazeColumnKeep[col]);
            }
        }
        for (col = 0; col + 1 < OXIDE_HAZE_COLUMNS; col++) {
            for (row = 0; row + 1 < OXIDE_HAZE_ROWS; row++) {
                ViewPoint t[3];

                t[0] = q[col][row];
                t[1] = q[col + 1][row];
                t[2] = q[col][row + 1];
                f.colour[0] = colour[col][row];
                f.colour[1] = colour[col + 1][row];
                f.colour[2] = colour[col][row + 1];
                draw_mesh_tri_plain(layer, &f, t, face++, 0, 0);
                t[0] = q[col + 1][row + 1];
                f.colour[0] = colour[col + 1][row + 1];
                draw_mesh_tri_plain(layer, &f, t, face++, 0, 0);
            }
        }
    }
    draw_owner(NULL);
}

/* As the Bearminator's binoculars: its place in the order is where its own middle is. From a little way in under its edges, so no crack shows. */
#define COVER_OVERLAP 8

void wide_scenery_cover_sides(void) {
    static const u32 black[4] = { 0, 0, 0, 0 };
    s32 extra = gfx_view_extra();
    s16 middle[4] = { 0, 0, 0, 0 };
    ViewPoint c;
    DVECTOR xy[4];
    u32 otz;
    s32 side, i;

    if (extra <= 0 || ScreenRect == NULL) {
        return;
    }
    project_point(middle, &c);
    otz = (points_order(&c, &c, &c) + RenderState.otzAdd) >> 1;
    if (otz >= (u32)RenderState.otLen) {
        return;
    }
    for (side = 0; side < 2; side++) {
        s32 x0 = side ? ScreenRect->w - COVER_OVERLAP : -extra, x1 = side ? ScreenRect->w + extra : COVER_OVERLAP;

        for (i = 0; i < 4; i++) {
            xy[i].vx = (s16)((i & 1) ? x1 : x0);
            xy[i].vy = (s16)((i & 2) ? ScreenRect->h : 0);
        }
        draw_plain_quad(RenderState.layer + (s32)otz, xy, black, GFX_OPAQUE);
    }
}

s32 wide_scenery_hides(const map_enviroment *scene, const subengine *sub) {
    s32 piece = (s32)(sub - scene->subs);
    s32 file;
    s32 i;

    if (gfx_view_extra() <= 0) {
        return 0;
    }
    if (NumTriedHidden < 0) {
        const char *s = getenv("WIDE_HIDE");

        NumTriedHidden = 0;
        while (s != NULL && *s != 0 && NumTriedHidden < MAX_TRIED_HIDDEN) {
            char *end;
            long n = strtol(s, &end, 10);

            if (end == s) {
                break;
            }
            TriedHidden[NumTriedHidden++] = (s16)n;
            s = *end == ',' ? end + 1 : end;
        }
    }
    for (i = 0; i < NumTriedHidden; i++) {
        if (TriedHidden[i] == piece) {
            return 1;
        }
    }
    /* trying something else: what's hidden goes with what's in the table */
    if (NumTried > 0 || RingTried > 0) {
        return 0;
    }
    file = (s32)(scene->dbase->file - DatIndex);
    for (i = 0; i < NUM_HIDDEN; i++) {
        if (Hidden[i].file == file && Hidden[i].piece == piece) {
            return 1;
        }
    }
    return 0;
}

/* How a quad's tile lies: mirrored, so its edge meets the same edge of the next, or all from its top row. And whether it's in the level's own order, by its
   depth as the game's own quads are, rather than at the front of it, or behind everything with draw_behind. */
#define RING_FLIP_U 1
#define RING_TOP_ROW 4
#define RING_IN_ORDER 8

static void ring_quad(const obj_textureset *ts, s32 image, const s16 corners[4][3], const u32 colour[4], s32 name, subengine *owner, s32 lie) {
    const obj_texture *t = &ts->tex[image];
    DrawFace f;
    ViewPoint q[4];
    s16 p[4];
    s32 i, otz = 0;

    memset(&f, 0, sizeof(f));
    f.image = t->info.image;
    f.palette = t->info.palette;
    for (i = 0; i < 4; i++) {
        p[0] = corners[i][0];
        p[1] = corners[i][1];
        p[2] = corners[i][2];
        p[3] = 0;
        project_point(p, &q[i]);
        /* the screen's fade, as the game's own models have it */
        f.colour[i] = RenderState.modelDp != 0 ? colour_fade(&colour[i], FarColor, RenderState.modelDp) : colour[i];
        f.uv[i] = (u16)(t->info.uv0 | (((i & 1) != 0) != ((lie & RING_FLIP_U) != 0) ? t->w - 1 : 0) | ((!(lie & RING_TOP_ROW) && (i & 2) ? t->h - 1 : 0) << 8));
    }
    if (lie & RING_IN_ORDER) {
        otz = (s32)((points_order(&q[1], &q[2], &q[3]) + RenderState.otzAdd) >> 1);
        if (otz >= RenderState.otLen) {
            return;
        }
    }
    draw_owner(owner);
    draw_owner_part((void *)(uintptr_t)(name + 1));
    draw_mesh_quad(RenderState.layer + otz, &f, q);
}

/* The view's own axes, as the map's pieces have them, with nothing moved, from where one of them is. */
static void ring_view_of(map_enviroment *scene, s32 piece, subengine *owner, MATRIX *view) {
    *owner = scene->subs[piece];
    owner->next = NULL;
    owner->otz = 0;
    memset(&owner->xform.mtx, 0, sizeof(owner->xform.mtx));
    owner->xform.mtx.m[0][0] = 0x1000;
    owner->xform.mtx.m[1][1] = 0x1000;
    owner->xform.mtx.m[2][2] = 0x1000;
    draw_map_object(owner, view, owner->flags, scene->dbase->data);
}

static void ring_view(map_enviroment *scene, subengine *owner, MATRIX *view) {
    /* where the level's own first piece is: a level's floor can be a few units off the middle, and what's made to meet it has to be too */
    ring_view_of(scene, 0, owner, view);
}

static void ring_begin(map_enviroment *scene, subengine *owner, MATRIX *view) {
    ring_view(scene, owner, view);
    draw_behind(1);
}

static void draw_ring(map_enviroment *scene, const WideRing *r, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;
    s32 side, ring, name = 0;
    s32 rows = (r->outer - r->inner) / r->floor_tile;
    s16 c[4][3];
    const u32 wall_colours[4] = { r->wall_colour, r->wall_colour, r->wall_colour, r->wall_colour };
    const u32 floor_colours[4] = { r->floor_colour, r->floor_colour, r->floor_colour, r->floor_colour };

    if (ts->tex == NULL) {
        return;
    }
    ring_begin(scene, owner, view);
    for (side = 0; side < r->sides; side++) {
        float a0 = side * 6.28318530718f / r->sides, a1 = (side + 1) * 6.28318530718f / r->sides;
        float x0 = cosf(a0), z0 = sinf(a0), x1 = cosf(a1), z1 = sinf(a1);

        c[0][0] = (s16)(x0 * r->outer);
        c[0][1] = (s16)r->wall_top;
        c[0][2] = (s16)(z0 * r->outer);
        c[1][0] = (s16)(x1 * r->outer);
        c[1][1] = (s16)r->wall_top;
        c[1][2] = (s16)(z1 * r->outer);
        c[2][0] = c[0][0];
        c[2][1] = (s16)r->floor_y;
        c[2][2] = c[0][2];
        c[3][0] = c[1][0];
        c[3][1] = (s16)r->floor_y;
        c[3][2] = c[1][2];
        ring_quad(ts, r->wall_image + side % r->wall_images, c, wall_colours, name++, owner, 0);
        for (ring = 0; ring < rows; ring++) {
            float in = (float)(r->inner + ring * r->floor_tile), out = in + r->floor_tile;

            c[0][0] = (s16)(x0 * out);
            c[0][1] = (s16)r->floor_y;
            c[0][2] = (s16)(z0 * out);
            c[1][0] = (s16)(x1 * out);
            c[1][1] = (s16)r->floor_y;
            c[1][2] = (s16)(z1 * out);
            c[2][0] = (s16)(x0 * in);
            c[2][1] = (s16)r->floor_y;
            c[2][2] = (s16)(z0 * in);
            c[3][0] = (s16)(x1 * in);
            c[3][1] = (s16)r->floor_y;
            c[3][2] = (s16)(z1 * in);
            ring_quad(ts, r->floor_image, c, floor_colours, name++, owner, 0);
        }
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Beach Ball's backdrop runs on past its ends, along the curve it's on. These are its 3 rows at each column: its own for FIRST to LAST, and
   past them they carry on turning as it does. And how high the sand comes on it there: at its foot behind the level's floor, and at its ends, where the view sees
   the far edge of the sand along it, so it carries on the line the far edge of the floor makes on the screen. */
typedef struct {
    s16 tx, tz, mx, mz, bx, bz;
    s16 sand;
} WallColumn;

static const WallColumn BeachWall[] = {
    { -5773, -12693, -5756, -12655, -5740, -12619, 720 },
    { -8175, -10272, -8150, -10241, -8127, -10212, 720 },
    { -9736, -7240, -9708, -7219, -9680, -7198, 720 },
    { -10314, -3879, -10284, -3868, -10254, -3857, 720 },
    { -9855, -500, -9826, -499, -9798, -498, 720 },
    { -8486, 2404, -8461, 2396, -8437, 2390, 720 },
    { -6492, 4568, -6492, 4527, -6492, 4487, 710 },
    { -4234, 5859, -4234, 5818, -4234, 5778, 803 },
    { -2073, 6456, -2073, 6414, -2073, 6375, 843 },
    { 12, 6552, 12, 6511, 2, 6469, 838 },
    { 1994, 6539, 1994, 6497, 1994, 6458, 843 },
    { 4294, 5859, 4294, 5818, 4294, 5778, 803 },
    { 6635, 4265, 6635, 4223, 6635, 4184, 620 },
    { 8598, 1767, 8574, 1762, 8551, 1758, 680 },
    { 9805, -1422, 9777, -1418, 9751, -1414, 720 },
    { 9998, -4826, 9969, -4812, 9943, -4799, 720 },
    { 9157, -8131, 9131, -8107, 9107, -8086, 720 },
    { 7362, -11030, 7341, -10998, 7322, -10969, 720 },
    { 4778, -13255, 4765, -13217, 4752, -13181, 720 },
};
#define BEACH_COLUMNS ((s32)(sizeof(BeachWall) / sizeof(BeachWall[0])))
#define BEACH_OWN_FIRST 6
#define BEACH_OWN_LAST 12
/* The tiles of each segment: below, then above, and whether it's mirrored. Past the backdrop's ends the panorama goes back the way it came, mirrored, so each
   join is a tile's edge against itself or its true neighbour. */
static const s16 BeachTiles[BEACH_COLUMNS - 1][3] = { { 5, 10, 0 }, { 5, 10, 1 }, { 6, 11, 1 }, { 4, 9, 1 }, { 3, 8, 1 }, { 2, 7, 1 }, { 2, 7, 0 }, { 3, 8, 0 }, { 4, 9, 0 }, { 6, 11, 0 }, { 5, 10, 0 }, { 5, 10, 0 }, { 5, 10, 1 }, { 6, 11, 1 }, { 4, 9, 1 }, { 3, 8, 1 }, { 2, 7, 1 }, { 2, 7, 0 } };
/* Its rows' heights and colours, as the level's. */
static const s16 BeachRowY[3] = { -930, -16, 740 };
static const u32 BeachRowColour[3] = { 0x9C5A39, 0x634A18, 0x212108 };
/* Its last column's foot is a little lighter, so what carries on from it starts the same. */
#define BEACH_LAST_FOOT_COLOUR 0x313118
#define BEACH_WALL_JOIN (-880) /* a little under its own top, which is -904 to -930 */
#define BEACH_WALL_TOP (-4000)

/* Its floor carried on, out to the wall's foot. */
static const WideSand BeachFloor = { 0, 0, 1, 0x1E1E10 };

/* The foot of each end of its own wall. Its floor stops short of them, with a gap you can't see in 4:3, so the sand has to come up in front of them. */
static const s16 BeachWallEnds[][2] = { { 252, 253 }, { 262, 263 } };
#define NUM_BEACH_WALL_ENDS ((s32)(sizeof(BeachWallEnds) / sizeof(BeachWallEnds[0])))

/* What N.Ballism's main mesh leaves out: a black side of one of its pillars, which only ever hid in the dark, and its front radars (draw_ballism_radars). */
static const s16 BallismLeftOut[][2] = { { 517, 517 }, { 801, 858 }, { 933, 982 } };
#define NUM_BALLISM_LEFT_OUT ((s32)(sizeof(BallismLeftOut) / sizeof(BallismLeftOut[0])))
#define BALLISM_RADAR_FACES (BallismLeftOut + 1)
#define NUM_BALLISM_RADAR_FACES (NUM_BALLISM_LEFT_OUT - 1)
/* its crystal map's front radars, in its own main mesh */
static const s16 BallismCrystalLeftOut[][2] = { { 466, 523 }, { 1048, 1097 } };
#define NUM_BALLISM_CRYSTAL_LEFT_OUT ((s32)(sizeof(BallismCrystalLeftOut) / sizeof(BallismCrystalLeftOut[0])))

/* Crashball's side pipes fade to black at their front ends, past where a 4:3 view stops. A corner of the right one's was left lit, so it ends in a hard wedge. So a wide
   view has the mirror image of the left one's end there instead. */
#define CRASHBALL_LEFT_PIPE 6
#define CRASHBALL_RIGHT_PIPE 8
static const s16 CrashballLeftPipeEnd[][2] = { { 8, 11 }, { 46, 49 }, { 68, 71 }, { 90, 93 }, { 116, 119 } };
#define NUM_CRASHBALL_LEFT_PIPE_END ((s32)(sizeof(CrashballLeftPipeEnd) / sizeof(CrashballLeftPipeEnd[0])))
static const s16 CrashballRightPipeEnd[][2] = { { 24, 35 }, { 62, 69 } };
#define NUM_CRASHBALL_RIGHT_PIPE_END ((s32)(sizeof(CrashballRightPipeEnd) / sizeof(CrashballRightPipeEnd[0])))

/* What a level's pieces leave out of their own faces, as they go with the backdrop or would show against it. */
static const struct {
    s16 file, piece;
    const s16 (*faces)[2];
    s32 count;
} LeftOutFaces[] = {
    { FILE_BEACH_BALL, 0, BeachWallEnds, NUM_BEACH_WALL_ENDS },
    { FILE_N_BALLISM, 0, BallismLeftOut, NUM_BALLISM_LEFT_OUT },
    { FILE_N_BALLISM_CRYSTAL, 0, BallismCrystalLeftOut, NUM_BALLISM_CRYSTAL_LEFT_OUT },
    { FILE_CRASHBALL, CRASHBALL_RIGHT_PIPE, CrashballRightPipeEnd, NUM_CRASHBALL_RIGHT_PIPE_END },
};
#define NUM_LEFT_OUT_FACES ((s32)(sizeof(LeftOutFaces) / sizeof(LeftOutFaces[0])))

/* Before one of a level's pieces draws. */
void wide_scenery_piece_faces(const map_enviroment *scene, const subengine *sub) {
    if (gfx_view_extra() > 0 && NumTried == 0 && RingTried == 0) {
        s32 file = (s32)(scene->dbase->file - DatIndex), piece = (s32)(sub - scene->subs);
        s32 i;

        for (i = 0; i < NUM_LEFT_OUT_FACES; i++) {
            if (LeftOutFaces[i].file == file && LeftOutFaces[i].piece == piece) {
                draw_face_list(LeftOutFaces[i].faces, LeftOutFaces[i].count, 0);
            }
        }
    }
}

/* Ring Ding's brick wall curves round behind the arena and stops at its sides, where a 4:3 view stops too. It's carried on round them: on along the
   turn its ends make, then straight towards the camera. A tile to each row and column, as the wall has, in its own tile and the colours of its ends. It starts a
   little way in behind the wall's own end, which covers that: the two are made apart and their corners don't quite meet, which left a crack of black between them. */
#define RING_DING_WALL_IMAGE 7
static const s16 RingDingWallRight[][2] = { { 6475, 3583 }, { 6654, 2589 }, { 6654, 1589 }, { 6654, 589 }, { 6654, -411 }, { 6654, -1411 }, { 6654, -2411 } };
static const s16 RingDingWallLeft[][2] = { { -6429, 3585 }, { -6608, 2592 }, { -6608, 1592 }, { -6608, 592 }, { -6608, -408 }, { -6608, -1408 }, { -6608, -2408 } };
#define RING_DING_WALL_COLUMNS ((s32)(sizeof(RingDingWallRight) / sizeof(RingDingWallRight[0])))
static const s16 RingDingWallRowY[] = { -3171, -2406, -1642, -877, -113, 651, 1415, 2180, 2944 };
static const u32 RingDingWallRowColour[] = { 0x080808, 0x080808, 0x181010, 0x292121, 0x292121, 0x292121, 0x292121, 0x292121, 0 };
#define RING_DING_WALL_ROWS ((s32)(sizeof(RingDingWallRowY) / sizeof(RingDingWallRowY[0])))

/* side is 1 for the right, where the tile runs on outwards from the wall's end, and -1 for the left, where it runs in to it. */
static s32 draw_ring_ding_side(const obj_textureset *ts, const s16 (*columns)[2], s32 side, s32 name, subengine *owner) {
    s16 c[4][3];
    u32 colours[4];
    s32 col, row;

    for (col = 0; col + 1 < RING_DING_WALL_COLUMNS; col++) {
        /* on the screen, left then right */
        const s16 *a = side > 0 ? columns[col] : columns[col + 1], *b = side > 0 ? columns[col + 1] : columns[col];

        for (row = 0; row + 1 < RING_DING_WALL_ROWS; row++) {
            c[0][0] = a[0];
            c[0][1] = RingDingWallRowY[row];
            c[0][2] = a[1];
            c[1][0] = b[0];
            c[1][1] = RingDingWallRowY[row];
            c[1][2] = b[1];
            c[2][0] = a[0];
            c[2][1] = RingDingWallRowY[row + 1];
            c[2][2] = a[1];
            c[3][0] = b[0];
            c[3][1] = RingDingWallRowY[row + 1];
            c[3][2] = b[1];
            colours[0] = colours[1] = RingDingWallRowColour[row];
            colours[2] = colours[3] = RingDingWallRowColour[row + 1];
            ring_quad(ts, RING_DING_WALL_IMAGE, c, colours, name++, owner, 0);
        }
    }
    return name;
}

static void draw_ring_ding_wall(map_enviroment *scene, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;

    if (ts->tex == NULL) {
        return;
    }
    ring_begin(scene, owner, view);
    draw_ring_ding_side(ts, RingDingWallLeft, -1, draw_ring_ding_side(ts, RingDingWallRight, 1, 0, owner), owner);
    draw_behind(0);
    draw_owner(NULL);
}

/* Pogo Painter's garden has a wall only at the back, and stone blocks for its sides, with nothing past them, where a 4:3 view doesn't reach. So the wall is
   carried on down each side from its ends, straight towards the camera, in its own tiles and the colours of its rows. It has a row more on top: the view looks
   out over the blocks, almost level at the top of the screen, and the wall's own top is only just out of it. And the garden's grass is carried on out to the
   walls' foot (widesand.c). All in the coordinates of the garden, the piece that has the wall and the grass. */
#define POGO_GARDEN 64
#define POGO_GRASS_IMAGE 11
#define POGO_WALL_ROWS 5
/* A level's wall carried on straight from an end of it, in rows of its own tiles in turn, with the colours its rows' edges have. */
#define SIDE_WALL_MAX_ROWS 5
#define SIDE_WALL_MAX_COLUMNS 12
typedef struct {
    s16 start_x, start_z;   /* a little way in behind the end of the level's own wall, which covers the join */
    s16 x, front_z;         /* then along this, to past where any view reaches */
    s16 columns;
    s16 side;               /* 1 on the right, where the tiles run on from the back on the screen, and -1 on the left, where they run in to it */
    s16 rows;               /* how many edges its rows have, from the top */
    s16 row_y[SIDE_WALL_MAX_ROWS];
    const s16 (*images)[3]; /* each row's tiles in turn, from the join */
    const u32 *colours;     /* each edge's */
} SideWall;
/* Each row's tiles in turn, as the back wall has them, and its colours, from the top: the new row darkens on up, as the top row does. */
static const s16 PogoWallImages[POGO_WALL_ROWS - 1][3] = { { 21, 16, 17 }, { 17, 21, 16 }, { 15, 12, 20 }, { 14, 13, 12 } };
static const u32 PogoWallRowColour[POGO_WALL_ROWS] = { 0x1C140C, 0x392918, 0x735A39, 0x735A39, 0x213121 };
static const SideWall PogoWalls[] = {
    { -4257, 2257, -4271, -3400, 9, -1, POGO_WALL_ROWS, { -2477, -1867, -1261, -713, -200 }, PogoWallImages, PogoWallRowColour },
    { 3773, 2669, 3783, -3400, 10, 1, POGO_WALL_ROWS, { -2446, -1836, -1230, -715, -200 }, PogoWallImages, PogoWallRowColour },
};
#define NUM_POGO_WALLS ((s32)(sizeof(PogoWalls) / sizeof(PogoWalls[0])))

/* The grass out to a loop round the garden: behind the back wall, then down the walls' foot, as high as the grass is where it meets the level's own wall at
   the back corners, and across the front, where the level stops. It runs on a little past that, so the strip of black along the bottom, in front of the
   level's edge, is grass too in a wide view. */
static const WideSand PogoGrass = { POGO_GARDEN, POGO_GRASS_IMAGE, POGO_GRASS_IMAGE, 0x213121 };
static const s16 PogoRim[][3] = {
    { -4271, 2252, -752 }, { -3641, 2700, -752 }, { 3783, 2700, -715 }, { 3783, 1916, -312 }, { 3783, -2864, -250 }, { -4271, -2864, -250 }, { -4271, 1518, -752 },
};
#define NUM_POGO_RIM ((s32)(sizeof(PogoRim) / sizeof(PogoRim[0])))

static s32 draw_side_wall(map_enviroment *scene, const SideWall *w, s32 name, subengine *owner) {
    const obj_textureset *ts = &scene->dbase->textures;
    s16 columns[SIDE_WALL_MAX_COLUMNS + 1][2];
    s16 c[4][3];
    u32 colours[4];
    s32 col, row;

    for (col = 0; col <= w->columns; col++) {
        columns[col][0] = col == 0 ? w->start_x : w->x;
        columns[col][1] = (s16)(w->start_z + (w->front_z - w->start_z) * col / w->columns);
    }
    for (col = 0; col < w->columns; col++) {
        /* on the screen, left then right */
        const s16 *a = w->side > 0 ? columns[col] : columns[col + 1], *b = w->side > 0 ? columns[col + 1] : columns[col];

        for (row = 0; row + 1 < w->rows; row++) {
            c[0][0] = a[0];
            c[0][1] = w->row_y[row];
            c[0][2] = a[1];
            c[1][0] = b[0];
            c[1][1] = w->row_y[row];
            c[1][2] = b[1];
            c[2][0] = a[0];
            c[2][1] = w->row_y[row + 1];
            c[2][2] = a[1];
            c[3][0] = b[0];
            c[3][1] = w->row_y[row + 1];
            c[3][2] = b[1];
            colours[0] = colours[1] = w->colours[row];
            colours[2] = colours[3] = w->colours[row + 1];
            ring_quad(ts, w->images[row][col % 3], c, colours, name++, owner, 0);
        }
    }
    return name;
}

static void draw_pogo_painter_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    s32 i, name = 0;

    if (scene->dbase->textures.tex == NULL || POGO_GARDEN >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, POGO_GARDEN, owner, view);
    draw_behind(1);
    if (wide_sand_ready(scene, FILE_POGO_PAINTER, &PogoGrass, PogoRim, NUM_POGO_RIM)) {
        wide_sand_draw(scene, &PogoGrass, owner);
    }
    for (i = 0; i < NUM_POGO_WALLS; i++) {
        name = draw_side_wall(scene, &PogoWalls[i], name, owner);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Toxic Dash's room is round, with its wall round the back half and a floor of toxic waste round the track, and open at the front for the camera, which is
   outside it. A wide view looks out past the wall's two ends, at the sides. So the wall is carried on from each end, straight towards the camera, in its own
   tiles in their turn and the colours of its rows, and the waste on out to the walls' foot (widesand.c). In the coordinates of the room, its piece. */
#define TOXIC_ROOM 16
#define TOXIC_WASTE_IMAGE 16
#define TOXIC_WALL_ROWS 4
/* the tiles before the left end's on the screen, and after the right end's: the wall has them in that turn, left to right */
static const s16 ToxicWallLeftImages[TOXIC_WALL_ROWS - 1][3] = { { 13, 12, 11 }, { 13, 12, 11 }, { 10, 9, 8 } };
static const s16 ToxicWallRightImages[TOXIC_WALL_ROWS - 1][3] = { { 12, 13, 11 }, { 12, 13, 11 }, { 9, 10, 8 } };
/* dark at the top, and lit by the waste at the foot */
static const u32 ToxicWallRowColour[TOXIC_WALL_ROWS] = { 0x000010, 0x080831, 0x08525A, 0x21FFEF };
static const SideWall ToxicWalls[] = {
    { -4871, 54, -4880, -6000, 4, -1, TOXIC_WALL_ROWS, { -3560, -2151, -585, 441 }, ToxicWallLeftImages, ToxicWallRowColour },
    { 4807, 54, 4816, -6000, 4, 1, TOXIC_WALL_ROWS, { -3560, -2151, -585, 441 }, ToxicWallRightImages, ToxicWallRowColour },
};
#define NUM_TOXIC_WALLS ((s32)(sizeof(ToxicWalls) / sizeof(ToxicWalls[0])))
/* The waste out to a loop behind the room's wall, then down the new walls' foot and across behind the camera, as high as it is at the wall's foot, and as
   bright as it is there, where it lights the wall. Its tile is the size the floor's own is. */
static const WideSand ToxicWaste = { TOXIC_ROOM, TOXIC_WASTE_IMAGE, TOXIC_WASTE_IMAGE, 0x6BFF00, 0, 0, 6 };
static const s16 ToxicRim[][3] = {
    { -4950, 0, 441 }, { -4950, -6100, 441 }, { 4890, -6100, 441 }, { 4890, 0, 441 }, { 3500, 3500, 441 }, { 0, 4970, 441 }, { -3500, 3500, 441 },
};
#define NUM_TOXIC_RIM ((s32)(sizeof(ToxicRim) / sizeof(ToxicRim[0])))

static void draw_toxic_dash_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    s32 i, name = 0;

    if (scene->dbase->textures.tex == NULL || TOXIC_ROOM >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, TOXIC_ROOM, owner, view);
    draw_behind(1);
    if (wide_sand_ready(scene, FILE_TOXIC_DASH, &ToxicWaste, ToxicRim, NUM_TOXIC_RIM)) {
        wide_sand_draw(scene, &ToxicWaste, owner);
    }
    for (i = 0; i < NUM_TOXIC_WALLS; i++) {
        name = draw_side_wall(scene, &ToxicWalls[i], name, owner);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Polar Panic's sea is only as wide as a 4:3 view sees, with steps at its sides. So it's carried on out at the sides, to past where the horizon's band and
   its mirror images reach (Extras), and behind the band (widesand.c), from its own tiles, shading on out to the band's foot, as the sea does at the back. Its
   far texels fade to its plain one, as side on they'd be only stripes. The sky above is black, as it is. In the sea's coordinates: it's 2750 further on. */
#define POLAR_SEA 1
#define POLAR_SEA_IMAGE 22
#define POLAR_FAR_SEA_IMAGE 23
#define POLAR_HORIZON_FOOT 0x4A3931
static const WideSand PolarSea = { POLAR_SEA, POLAR_SEA_IMAGE, POLAR_FAR_SEA_IMAGE, POLAR_HORIZON_FOOT, 0, 4000, 6 };
static const s16 PolarRim[][3] = { { -26103, -5850, 751 }, { 25897, -5850, 751 }, { 25897, 6950, 751 }, { -26103, 6950, 751 } };
#define NUM_POLAR_RIM ((s32)(sizeof(PolarRim) / sizeof(PolarRim[0])))

/* Tilt Panic's sea is Polar Panic's again by day, as wide as a 4:3 view and with steps at its sides, and its sky a wall at the back whose ends are alike
   (Extras). So it's carried on in the same way, out to past where the sky's mirror images reach, shading on out to the haze at the sky's foot, as it does at
   the back. In the sea's coordinates: it's 2735 further on. */
#define TILT_SEA 2
#define TILT_SEA_IMAGE 22
#define TILT_FAR_SEA_IMAGE 23
#define TILT_HAZE 0xADA5B5
static const WideSand TiltSea = { TILT_SEA, TILT_SEA_IMAGE, TILT_FAR_SEA_IMAGE, TILT_HAZE, 0, 4000, 6 };
static const s16 TiltRim[][3] = { { -32062, -7835, 728 }, { 31938, -7835, 728 }, { 31938, 6965, 728 }, { -32062, 6965, 728 } };
#define NUM_TILT_RIM ((s32)(sizeof(TiltRim) / sizeof(TiltRim[0])))
/* Tilt Panic's crystal map's sea, out past the storm's dome all round, rising a little to its far edge so that's above the dome's foot from the arena: the
   level's white clear colour showed between them. And Polar Panic's crystal map's, as the normal map's, out to its sunset */
static const WideSand TiltCrystalSea = { TILT_CRYSTAL_SEA, TILT_CRYSTAL_SEA_IMAGE, TILT_CRYSTAL_SEA_IMAGE, TILT_CRYSTAL_SEA_COLOUR, 0, 4000, 6 };
static const s16 TiltCrystalRim[][3] = { { -32000, -9000, 0 }, { 32000, -9000, 0 }, { 32000, 20000, 0 }, { -32000, 20000, 0 } };
#define NUM_TILT_CRYSTAL_RIM ((s32)(sizeof(TiltCrystalRim) / sizeof(TiltCrystalRim[0])))
static const WideSand PolarCrystalSea = { POLAR_SEA, POLAR_SEA_IMAGE, POLAR_FAR_SEA_IMAGE, POLAR_CRYSTAL_SUNSET, 0, 4000, 6 };

/* Pogo Padlock's crystal map's pavement, out to the foot of the wall's copies at the back and to past where any view reaches at the sides and in front,
   where it's as low as the road: its tiles fade to their plain one far off, in the road's dark. In the alley's coordinates */
static const WideSand PogoPadlockCrystalPavement = { POGO_PADLOCK_CRYSTAL_ALLEY, POGO_PADLOCK_CRYSTAL_PAVEMENT_IMAGE, POGO_PADLOCK_CRYSTAL_PAVEMENT_IMAGE, POGO_PADLOCK_CRYSTAL_DARK, POGO_PADLOCK_CRYSTAL_DARK, 3000, 2 };
static const s16 PogoPadlockCrystalRim[][3] = { { -14000, 4254, -157 }, { 14000, 4254, -157 }, { 14000, -5000, 4 }, { -14000, -5000, 4 } };
#define NUM_POGO_PADLOCK_CRYSTAL_RIM ((s32)(sizeof(PogoPadlockCrystalRim) / sizeof(PogoPadlockCrystalRim[0])))

/* Beach Ball's crystal map's sea, out to past where any view reaches, at its own level, its far texels faded to its plain one. In the sea's coordinates */
static const WideSand BeachBallCrystalSea = { BEACH_BALL_CRYSTAL_SEA, BEACH_BALL_CRYSTAL_SEA_IMAGE, BEACH_BALL_CRYSTAL_FAR_SEA_IMAGE, BEACH_BALL_CRYSTAL_FAR_SEA, 0, 4000, 6 };
static const s16 BeachBallCrystalRim[][3] = { { -30000, -6000, 505 }, { 30000, -6000, 505 }, { 30000, 13000, 505 }, { -30000, 13000, 505 } };
#define NUM_BEACH_BALL_CRYSTAL_RIM ((s32)(sizeof(BeachBallCrystalRim) / sizeof(BeachBallCrystalRim[0])))

/* A level whose only floor carried on is its sea, or its pavement, in the view of that floor's piece, behind everything. */
static void draw_sea_backdrop(map_enviroment *scene, s32 file, const WideSand *sea, const s16 (*rim)[3], s32 rim_points, subengine *owner, MATRIX *view) {
    if (scene->dbase->textures.tex == NULL || sea->piece >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, sea->piece, owner, view);
    draw_behind(1);
    if (wide_sand_ready(scene, file, sea, rim, rim_points)) {
        wide_sand_draw(scene, sea, owner);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* El Pogo Loco's sand stops at a forecourt on the left and a road on the right, with nothing past them. So its sand is carried on out (widesand.c), under them
   and on past them, dipping away to the foot of its far wall, whose copies carry the wall on at the sides (Extras). Its view is of the piece with the sand. */
#define POGO_LOCO_SAND 64
#define POGO_LOCO_SAND_IMAGE 7
/* its own colour away from its edges, as they're shaded dark beside the forecourt and the road: a warm one, as the level gives this tile, which at its plain
   texel far off is the colour of the far wall's own foot */
static const WideSand PogoLocoSand = { POGO_LOCO_SAND, POGO_LOCO_SAND_IMAGE, POGO_LOCO_SAND_IMAGE, 0x687B99, 0x687B99, 2000 };
static const s16 PogoLocoRim[][3] = { { -30000, 13844, 2170 }, { 30000, 13844, 2170 }, { 16000, -3300, 4 }, { -16000, -3300, 4 } };
#define NUM_POGO_LOCO_RIM ((s32)(sizeof(PogoLocoRim) / sizeof(PogoLocoRim[0])))

static void draw_el_pogo_loco_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    if (scene->dbase->textures.tex == NULL || POGO_LOCO_SAND >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, POGO_LOCO_SAND, owner, view);
    draw_behind(1);
    if (wide_sand_ready(scene, FILE_EL_POGO_LOCO, &PogoLocoSand, PogoLocoRim, NUM_POGO_LOCO_RIM)) {
        wide_sand_draw(scene, &PogoLocoSand, owner);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Desert Fox's arena is on an island of sand, with a panorama of far dunes and sky on part of a circle round it, far off, and nothing between them. So its
   sand is carried on out to the panorama's foot all round (widesand.c), from all its ground, of many tiles, in its plain sand at the size the ground has it.
   The panorama goes on round its circle at the sides as copies of itself (Extras). All in the coordinates of the ground, its first piece. */
#define DESERT_FOX_GROUND 0
#define DESERT_FOX_SAND_IMAGE 2
#define DESERT_FOX_FOOT (-60)
#define DESERT_FOX_COLOUR 0x7B7B7B
static const WideSand DesertFoxSand = { DESERT_FOX_GROUND, DESERT_FOX_SAND_IMAGE, WIDE_SAND_ANY_TILE, DESERT_FOX_COLOUR, DESERT_FOX_COLOUR, 0, 2 };
/* a little behind the panorama's foot all round, its own and its copies, so the sand goes under it, and on round behind the camera */
static const s16 DesertFoxRim[][3] = {
    { -10251, -5546, DESERT_FOX_FOOT }, { -10870, -2939, DESERT_FOX_FOOT }, { -10820, -259, DESERT_FOX_FOOT }, { -10106, 2325, DESERT_FOX_FOOT },
    { -8772, 4650, DESERT_FOX_FOOT }, { -6994, 6692, DESERT_FOX_FOOT }, { -4651, 8050, DESERT_FOX_FOOT }, { -2291, 8765, DESERT_FOX_FOOT },
    { -241, 8786, DESERT_FOX_FOOT }, { 1817, 8765, DESERT_FOX_FOOT }, { 4177, 8050, DESERT_FOX_FOOT }, { 6520, 6692, DESERT_FOX_FOOT },
    { 8298, 4650, DESERT_FOX_FOOT }, { 9632, 2325, DESERT_FOX_FOOT }, { 10346, -259, DESERT_FOX_FOOT }, { 10396, -2939, DESERT_FOX_FOOT },
    { 9777, -5546, DESERT_FOX_FOOT }, { 8528, -7919, DESERT_FOX_FOOT }, { 6729, -9907, DESERT_FOX_FOOT }, { 4493, -11384, DESERT_FOX_FOOT },
    { 1959, -12259, DESERT_FOX_FOOT }, { -713, -12476, DESERT_FOX_FOOT }, { -3355, -12022, DESERT_FOX_FOOT }, { -5801, -10926, DESERT_FOX_FOOT },
    { -7897, -9255, DESERT_FOX_FOOT }, { -9512, -7116, DESERT_FOX_FOOT },
};
#define NUM_DESERT_FOX_RIM ((s32)(sizeof(DesertFoxRim) / sizeof(DesertFoxRim[0])))

/* the file is the map's, normal or crystal, which the sand is made from */
static void draw_desert_fox_backdrop(map_enviroment *scene, s32 file, subengine *owner, MATRIX *view) {
    if (scene->dbase->textures.tex == NULL || DESERT_FOX_GROUND >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, DESERT_FOX_GROUND, owner, view);
    draw_behind(1);
    if (wide_sand_ready(scene, file, &DesertFoxSand, DesertFoxRim, NUM_DESERT_FOX_RIM)) {
        wide_sand_draw(scene, &DesertFoxSand, owner);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* The Bearminator's floe on the right has a face missing from the ice cliff along its edge, out where a 4:3 view doesn't reach, between the two either side
   of it. It's there again, as they have their tile and their colours where they meet it, in the level's own order, so what passes in front covers it. In the
   floes' coordinates: its top and its foot at one end, then at the other, with the tile's top along the cliff's top */
#define BEAR_FLOES 4
#define BEAR_CLIFF_IMAGE 23
static const s16 BearCliff[4][3] = { { 5579, 251, 1202 }, { 5579, 772, 1202 }, { 5133, 251, 393 }, { 5133, 772, 393 } };
static const u32 BearCliffColour[4] = { 0xBDB5B5, 0x425210, 0xC6C6C6, 0x5A6B18 };

static void draw_bear_cliff(map_enviroment *scene, subengine *owner, MATRIX *view) {
    if (scene->dbase->textures.tex == NULL || BEAR_FLOES >= scene->num_placements) {
        return;
    }
    ring_view_of(scene, BEAR_FLOES, owner, view);
    /* and the floes' own place in the order, which a view leaves out */
    RenderState.otzAdd += scene->subs[BEAR_FLOES].otz;
    ring_quad(&scene->dbase->textures, BEAR_CLIFF_IMAGE, BearCliff, BearCliffColour, 0, owner, RING_IN_ORDER);
    draw_owner(NULL);
}

static void draw_beach_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    obj_textureset *ts = &scene->dbase->textures;
    s16 foot[BEACH_COLUMNS][3];
    s32 seg, band, name = 0;
    s16 c[4][3];
    u32 colours[4];

    if (ts->tex == NULL) {
        return;
    }
    for (seg = 0; seg < BEACH_COLUMNS; seg++) {
        foot[seg][0] = BeachWall[seg].bx;
        foot[seg][1] = BeachWall[seg].bz;
        foot[seg][2] = BeachWall[seg].sand;
    }
    ring_begin(scene, owner, view);
    if (wide_sand_ready(scene, FILE_BEACH_BALL, &BeachFloor, foot, BEACH_COLUMNS)) {
        wide_sand_draw(scene, &BeachFloor, owner);
    }
    for (seg = 0; seg < BEACH_COLUMNS - 1; seg++) {
        const WallColumn *a = &BeachWall[seg], *b = &BeachWall[seg + 1];

        /* above it all, its art's top row carried on up, as the view sees over its top at the sides */
        c[0][0] = a->tx;
        c[0][1] = BEACH_WALL_TOP;
        c[0][2] = a->tz;
        c[1][0] = b->tx;
        c[1][1] = BEACH_WALL_TOP;
        c[1][2] = b->tz;
        c[2][0] = a->tx;
        c[2][1] = BEACH_WALL_JOIN;
        c[2][2] = a->tz;
        c[3][0] = b->tx;
        c[3][1] = BEACH_WALL_JOIN;
        c[3][2] = b->tz;
        colours[0] = colours[1] = colours[2] = colours[3] = BeachRowColour[0];
        ring_quad(ts, BeachTiles[seg][1], c, colours, name++, owner, RING_TOP_ROW | (BeachTiles[seg][2] ? RING_FLIP_U : 0));
        if (seg >= BEACH_OWN_FIRST && seg < BEACH_OWN_LAST) {
            continue;
        }
        for (band = 0; band < 2; band++) {
            /* band 0 is the upper tile */
            c[0][0] = band == 0 ? a->tx : a->mx;
            c[0][1] = BeachRowY[band];
            c[0][2] = band == 0 ? a->tz : a->mz;
            c[1][0] = band == 0 ? b->tx : b->mx;
            c[1][1] = BeachRowY[band];
            c[1][2] = band == 0 ? b->tz : b->mz;
            c[2][0] = band == 0 ? a->mx : a->bx;
            c[2][1] = BeachRowY[band + 1];
            c[2][2] = band == 0 ? a->mz : a->bz;
            c[3][0] = band == 0 ? b->mx : b->bx;
            c[3][1] = BeachRowY[band + 1];
            c[3][2] = band == 0 ? b->mz : b->bz;
            colours[0] = colours[1] = BeachRowColour[band];
            colours[2] = colours[3] = BeachRowColour[band + 1];
            if (band == 1 && seg == BEACH_OWN_LAST) {
                colours[2] = BEACH_LAST_FOOT_COLOUR;
            }
            ring_quad(ts, BeachTiles[seg][band == 0 ? 1 : 0], c, colours, name++, owner, BeachTiles[seg][2] ? RING_FLIP_U : 0);
        }
    }
    draw_face_list(BeachWallEnds, NUM_BEACH_WALL_ENDS, 1);
    draw_owner(&scene->subs[0]);
    scene->subs[0].hook(&scene->subs[0], view, 0);
    draw_face_list(NULL, 0, 0);
    draw_behind(0);
    draw_owner(NULL);
}

/* N.Ballism's round wall stops at each side of the arena, where a 4:3 view stops too. It's carried on round with copies of its own last stretch, each the
   mirror image of the one before, as its tiles are from one stretch to the next, so their edges meet. These are the stretch's faces, */
static const s16 BallismStretch[][2] = { { 265, 272 }, { 409, 410 }, { 413, 414 } };
#define NUM_BALLISM_STRETCH ((s32)(sizeof(BallismStretch) / sizeof(BallismStretch[0])))
/* its two ends, */
static const s16 BallismStretchEnds[2][2] = { { 5509, 1765 }, { 5543, 2781 } };
/* and where the copies' ends go: on from where it stops on the right, and from behind where it stops on the left. */
static const s16 BallismRoundRight[][2] = { { 5509, 1765 }, { 5640, 745 }, { 5710, -270 }, { 5640, -1290 }, { 5400, -2280 }, { 5000, -3220 } };
static const s16 BallismRoundLeft[][2] = { { -5000, 2900 }, { -5400, 1950 }, { -5640, 960 }, { -5710, -60 }, { -5640, -1080 }, { -5400, -2070 }, { -5000, -3010 } };
#define NUM_BALLISM_RIGHT ((s32)(sizeof(BallismRoundRight) / sizeof(BallismRoundRight[0])))
#define NUM_BALLISM_LEFT ((s32)(sizeof(BallismRoundLeft) / sizeof(BallismRoundLeft[0])))
/* Under its floor, as the floor has holes where nothing was ever meant to show through them. It stops short of the floor's edge, which is see-through for its last few texels. */
#define BALLISM_PLATE_Y 8
static const s16 BallismPlate[4][2] = { { -2790, 2750 }, { 2750, 2750 }, { -2790, -2750 }, { 2750, -2750 } };

/* What it has in place of the floor under its front corners. */
static const s16 BallismFloors[] = { 9, 10 };
#define NUM_BALLISM_FLOORS ((s32)(sizeof(BallismFloors) / sizeof(BallismFloors[0])))

static subengine BallismCopies[NUM_BALLISM_RIGHT + NUM_BALLISM_LEFT];

static void draw_ballism_round(const map_enviroment *scene, const s16 (*round)[2], s32 count, subengine *copies, MATRIX *view) {
    const subengine *from = &scene->subs[0];
    const s16 *near_end = BallismStretchEnds[0], *far_end = BallismStretchEnds[1];
    float sx = (float)(near_end[0] - far_end[0]), sz = (float)(near_end[1] - far_end[1]);
    float length = sqrtf(sx * sx + sz * sz);
    /* along the stretch, and from it towards the arena */
    float dx = sx / length, dz = sz / length;
    float nx = dz, nz = -dx;
    s32 i;

    for (i = 0; i + 1 < count; i++) {
        const s16 *a = round[i + (i & 1)], *b = round[i + 1 - (i & 1)];
        subengine *copy = &copies[i];
        float wx = (float)(a[0] - b[0]), wz = (float)(a[1] - b[1]);
        float width = sqrtf(wx * wx + wz * wz);
        float tx = wz / width, tz = -wx / width;
        MATRIX spin, turned;

        if (tx * (float)(a[0] + b[0]) + tz * (float)(a[1] + b[1]) > 0) {
            tx = -tx;
            tz = -tz;
        }
        memset(&spin, 0, sizeof(spin));
        spin.m[0][0] = (s16)((wx / length * dx + tx * nx) * 4096.0f);
        spin.m[0][2] = (s16)((wx / length * dz + tx * nz) * 4096.0f);
        spin.m[1][1] = 0x1000;
        spin.m[2][0] = (s16)((wz / length * dx + tz * nx) * 4096.0f);
        spin.m[2][2] = (s16)((wz / length * dz + tz * nz) * 4096.0f);
        *copy = *from;
        copy->next = NULL;
        copy->XYZ.x = from->XYZ.x + a[0] - (spin.m[0][0] * near_end[0] + spin.m[0][2] * near_end[1]) / 0x1000;
        copy->XYZ.z = from->XYZ.z + a[1] - (spin.m[2][0] * near_end[0] + spin.m[2][2] * near_end[1]) / 0x1000;
        matrix_mul(&spin, (MATRIX *)&from->xform.mtx, &turned);
        copy->xform.mtx = turned;
        draw_mirrored((s32)spin.m[0][0] * spin.m[2][2] - (s32)spin.m[0][2] * spin.m[2][0] < 0);
        draw_owner(copy);
        copy->hook(copy, view, 0);
    }
    draw_mirrored(0);
}

static void draw_ballism_backdrop(map_enviroment *scene, subengine *owner, MATRIX *view) {
    DrawFace f;
    ViewPoint q[4], t[3];
    s16 p[4];
    s32 i;

    ring_begin(scene, owner, view);
    memset(&f, 0, sizeof(f));
    if (RenderState.modelDp != 0) {
        const u32 black = 0;

        f.colour[0] = f.colour[1] = f.colour[2] = colour_fade(&black, FarColor, RenderState.modelDp);
    }
    draw_owner(owner);
    draw_owner_part(BallismPlate);
    for (i = 0; i < 4; i++) {
        p[0] = BallismPlate[i][0];
        p[1] = BALLISM_PLATE_Y;
        p[2] = BallismPlate[i][1];
        p[3] = 0;
        project_point(p, &q[i]);
    }
    for (i = 0; i < 2; i++) {
        t[0] = q[i];
        t[1] = q[i + 1];
        t[2] = q[i + 2];
        draw_mesh_tri_plain(RenderState.layer, &f, t, i, 0, 0);
    }
    for (i = 0; i < NUM_BALLISM_FLOORS; i++) {
        subengine *floor = &scene->subs[BallismFloors[i]];

        draw_owner(floor);
        floor->hook(floor, view, 0);
    }
    draw_face_list(BallismStretch, NUM_BALLISM_STRETCH, 1);
    draw_ballism_round(scene, BallismRoundRight, NUM_BALLISM_RIGHT, BallismCopies, view);
    draw_ballism_round(scene, BallismRoundLeft, NUM_BALLISM_LEFT, BallismCopies + NUM_BALLISM_RIGHT, view);
    draw_face_list(NULL, 0, 0);
    draw_behind(0);
    draw_owner(NULL);
}

/* Its crystal map's floor under the front radars' bases goes with the backdrop too, for the same reason: its surroundings are another map's */
static void draw_ballism_crystal_floors(map_enviroment *scene, subengine *owner, MATRIX *view) {
    s32 i;

    ring_begin(scene, owner, view);
    for (i = 0; i < NUM_BALLISM_FLOORS; i++) {
        subengine *floor = &scene->subs[BallismFloors[i]];

        draw_owner(floor);
        floor->hook(floor, view, 0);
    }
    draw_behind(0);
    draw_owner(NULL);
}

/* Its front radars are in its main mesh, which is painted first, so their bases, painted after it, cover them. The 4:3 view hardly saw that, as it stops by them.
   So they, their bases and their rings go in real depth, as the game's solid things do, in the order of where they are. */
static const s16 BallismRadarBases[] = { 3, 4 };
#define NUM_BALLISM_RADAR_BASES ((s32)(sizeof(BallismRadarBases) / sizeof(BallismRadarBases[0])))
/* The right one's rings, then the left's. */
static const s16 BallismRadarRings[] = { 8, 7 };
#define NUM_BALLISM_RADAR_RINGS ((s32)(sizeof(BallismRadarRings) / sizeof(BallismRadarRings[0])))
/* The game swaps a base and its rings for lit ones while a radar's firing. The lit rings have a shaft of light down the spike too, in these faces. */
static const s16 BallismLitShaft[][2] = { { 0, 15 } };
/* It's only lit on one side, which faces the camera on the radars at the back. These are turned copies of those, so it would face away. So it's turned a third of the
   way round the spike, the line between these on the right one, and all its sides are alike. */
static const s16 BallismShaftLine[2][3] = { { 2068, -520, -2018 }, { 1836, -419, -1781 } };
#define BALLISM_SHAFT_ORDER (-96)
static subengine BallismShafts[NUM_BALLISM_RADAR_RINGS];

static subengine BallismRadars;

/* They were never meant to be seen from the side, so they stop at the back of their rims with nothing there. This is where one's rim stops, and the other's is its
   mirror image. A wide view gives them a back, as an old television has, with a flat shade to each side, by how much it faces up. */
static const s16 BallismRadarRim[][3] = {
    { 2130, -174, -2377 }, { 1928, -397, -2574 }, { 1928, -712, -2574 }, { 2130, -935, -2377 },
    { 2416, -935, -2098 }, { 2618, -712, -1901 }, { 2618, -397, -1901 }, { 2416, -174, -2098 },
};
#define NUM_BALLISM_RIM ((s32)(sizeof(BallismRadarRim) / sizeof(BallismRadarRim[0])))
/* Its rows from the rim back: a collar, a taper and a block at the end. How far it's gone in each of x and z, how much of the rim's size is left, in 64ths, and how much of its shade, in eighths. */
static const struct {
    s16 along;
    u8 size, light;
} BallismBackRows[] = {
    { 0, 64, 8 },
    { 26, 64, 8 },
    { 26, 58, 5 },
    { 105, 38, 6 },
    { 105, 34, 4 },
    { 135, 32, 4 },
};
#define NUM_BALLISM_BACK_ROWS ((s32)(sizeof(BallismBackRows) / sizeof(BallismBackRows[0])))
/* What it faces up and down most: the radar's own purple. */
static const u8 BallismBackShade[2][3] = { { 0x18, 0x10, 0x31 }, { 0x5A, 0x42, 0xAD } };
/* the crystal map's gold */
static const u8 BallismCrystalBackShade[2][3] = { { 0x31, 0x26, 0x12 }, { 0xAD, 0x8C, 0x46 } };

/* A map's front radars: their faces in its main mesh, and the shades of their backs and of their rings, as they are and while firing */
typedef struct {
    const s16 (*faces)[2];
    s32 num_faces;
    const u8 (*back_shade)[3];
    const u8 (*ring_shade)[2][3];
} BallismRadarLook;
static const BallismRadarLook *RadarLook;

/* That much of a shade, of 256, and of 8 of that. */
static u32 radar_shade(const u8 (*shade)[3], s32 up, s32 light) {
    u32 c = 0;
    s32 n;

    for (n = 0; n < 3; n++) {
        s32 v = shade[0][n] + (shade[1][n] - shade[0][n]) * up / 256;

        c |= (u32)(v * light / 8) << (n * 8);
    }
    /* the screen's fade, as the game's own models have it */
    return RenderState.modelDp != 0 ? colour_fade(&c, FarColor, RenderState.modelDp) : c;
}

static void radar_back_tri(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c, u32 ca, u32 cb, u32 cc, s32 face) {
    DrawFace f;
    ViewPoint q[3];

    memset(&f, 0, sizeof(f));
    q[0] = *a;
    q[1] = *b;
    q[2] = *c;
    f.colour[0] = ca;
    f.colour[1] = cb;
    f.colour[2] = cc;
    draw_mesh_tri_plain(RenderState.layer + ((points_order(a, b, c) + RenderState.otzAdd) >> 1), &f, q, face, 0, 0);
}

/* Two corners near and two far. The next face's place. */
static s32 radar_quad(const ViewPoint *a, const ViewPoint *b, const ViewPoint *c, const ViewPoint *d, u32 near_shade, u32 far_shade, s32 face) {
    radar_back_tri(a, b, c, near_shade, near_shade, far_shade, face);
    radar_back_tri(b, c, d, near_shade, far_shade, far_shade, face + 1);
    return face + 2;
}

/* side is 1 for the right one and -1 for the left. */
static s32 draw_ballism_radar_back(s32 side, s32 face) {
    ViewPoint near_row[NUM_BALLISM_RIM], far_row[NUM_BALLISM_RIM];
    s32 cx = 0, cz = 0, top = 0x7FFF, bottom = -0x7FFF, cy;
    s16 p[4];
    s32 i, row;

    for (i = 0; i < NUM_BALLISM_RIM; i++) {
        cx += BallismRadarRim[i][0];
        cz += BallismRadarRim[i][2];
        top = BallismRadarRim[i][1] < top ? BallismRadarRim[i][1] : top;
        bottom = BallismRadarRim[i][1] > bottom ? BallismRadarRim[i][1] : bottom;
    }
    cx /= NUM_BALLISM_RIM;
    cz /= NUM_BALLISM_RIM;
    cy = (top + bottom) / 2;
    p[3] = 0;
    for (row = 0; row < NUM_BALLISM_BACK_ROWS; row++) {
        s32 along = BallismBackRows[row].along, size = BallismBackRows[row].size;

        for (i = 0; i < NUM_BALLISM_RIM; i++) {
            s32 x = BallismRadarRim[i][0], y = BallismRadarRim[i][1], z = BallismRadarRim[i][2];

            near_row[i] = far_row[i];
            p[0] = (s16)(side * (cx + (x - cx) * size / 64 + along));
            p[1] = (s16)(cy + (y - cy) * size / 64);
            p[2] = (s16)(cz + (z - cz) * size / 64 - along);
            project_point(p, &far_row[i]);
        }
        if (row == 0) {
            continue;
        }
        for (i = 0; i < NUM_BALLISM_RIM; i++) {
            s32 j = (i + 1) % NUM_BALLISM_RIM;
            /* y is down: a side along the top is the most up it gets */
            s32 up = (bottom - (BallismRadarRim[i][1] + BallismRadarRim[j][1]) / 2) * 256 / (bottom - top);

            face = radar_quad(&near_row[i], &near_row[j], &far_row[i], &far_row[j], radar_shade(RadarLook->back_shade, up, BallismBackRows[row - 1].light), radar_shade(RadarLook->back_shade, up, BallismBackRows[row].light), face);
        }
    }
    {
        u32 cap = radar_shade(RadarLook->back_shade, 0, BallismBackRows[NUM_BALLISM_BACK_ROWS - 1].light);

        for (i = 1; i + 1 < NUM_BALLISM_RIM; i++) {
            radar_back_tri(&far_row[0], &far_row[i], &far_row[i + 1], cap, cap, cap, face++);
        }
    }
    return face;
}

/* side is 1 for the right one and -1 for the left. */
static void draw_ballism_shaft(const subengine *rings, subengine *copy, s32 side, MATRIX *view) {
    float px = (float)(side * BallismShaftLine[0][0]), py = (float)BallismShaftLine[0][1], pz = (float)BallismShaftLine[0][2];
    float ax = (float)(side * BallismShaftLine[1][0]) - px, ay = (float)BallismShaftLine[1][1] - py, az = (float)BallismShaftLine[1][2] - pz;
    float length = sqrtf(ax * ax + ay * ay + az * az);
    float c = -0.5f, n = -0.8660254f * (float)side, t = 1.5f;
    float m[3][3];
    MATRIX spin, turned;
    s32 i, j;

    ax /= length;
    ay /= length;
    az /= length;
    m[0][0] = c + t * ax * ax;
    m[0][1] = t * ax * ay - n * az;
    m[0][2] = t * ax * az + n * ay;
    m[1][0] = t * ay * ax + n * az;
    m[1][1] = c + t * ay * ay;
    m[1][2] = t * ay * az - n * ax;
    m[2][0] = t * az * ax - n * ay;
    m[2][1] = t * az * ay + n * ax;
    m[2][2] = c + t * az * az;
    memset(&spin, 0, sizeof(spin));
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            spin.m[i][j] = (s16)(m[i][j] * 4096.0f);
        }
    }
    *copy = *rings;
    copy->next = NULL;
    /* it's light, so it goes by the painted order, and it has to come after the spike it's round, whose long faces sort by their middles */
    copy->otz = BALLISM_SHAFT_ORDER;
    copy->XYZ.x = rings->XYZ.x + (s32)(px - (m[0][0] * px + m[0][1] * py + m[0][2] * pz));
    copy->XYZ.y = rings->XYZ.y + (s32)(py - (m[1][0] * px + m[1][1] * py + m[1][2] * pz));
    copy->XYZ.z = rings->XYZ.z + (s32)(pz - (m[2][0] * px + m[2][1] * py + m[2][2] * pz));
    matrix_mul(&spin, (MATRIX *)&rings->xform.mtx, &turned);
    copy->xform.mtx = turned;
    draw_face_list(BallismLitShaft, 1, 1);
    draw_owner(copy);
    copy->hook(copy, view, 0);
    draw_face_list(NULL, 0, 0);
}

/* Their rings are flat, and this view's almost exactly edge on to them, so all they'd be is needles. So they're made solid, from the hexagons the level has them in, in
   whichever model the game has swapped in. A run of this many faces in it is a ring, going round its corners, outside and inside in turn. */
#define BALLISM_RING_FACES 12
#define BALLISM_RING_CORNERS 6
/* How thick, in 32nds of how big it is. */
#define BALLISM_RING_THICK 4
/* The gold they are on the screen, on what faces up and down most: as they are, and while firing. */
static const u8 BallismRingShade[2][2][3] = { { { 0x40, 0x3A, 0x00 }, { 0xB0, 0xA4, 0x10 } }, { { 0x70, 0x68, 0x00 }, { 0xF0, 0xE4, 0x20 } } };
/* the crystal map's green */
static const u8 BallismCrystalRingShade[2][2][3] = { { { 0x12, 0x40, 0x16 }, { 0x33, 0xB0, 0x3E } }, { { 0x20, 0x70, 0x26 }, { 0x45, 0xF0, 0x55 } } };
static const BallismRadarLook BallismRadarLooks[] = {
    { BALLISM_RADAR_FACES, NUM_BALLISM_RADAR_FACES, BallismBackShade, BallismRingShade },
    { BallismCrystalLeftOut, NUM_BALLISM_CRYSTAL_LEFT_OUT, BallismCrystalBackShade, BallismCrystalRingShade },
};

/* A ring made solid, from the strip of corners its faces go round it in. The next face's place. */
static s32 draw_ballism_ring(const s16 *strip, const u8 (*shade)[3], s32 face) {
    ViewPoint point[2][2][BALLISM_RING_CORNERS], middle[BALLISM_RING_CORNERS];
    float cx = 0, cy = 0, cz = 0, size = 0, nx, ny, nz, length, half;
    float d[2];
    const s16 *a, *b, *c;
    s32 top = 0x7FFF, bottom = -0x7FFF;
    s32 outside, corner, end, part;
    s16 p[4];

    for (corner = 0; corner < BALLISM_RING_CORNERS * 2; corner++) {
        cx += strip[corner * 4];
        cy += strip[corner * 4 + 1];
        cz += strip[corner * 4 + 2];
    }
    cx /= BALLISM_RING_CORNERS * 2;
    cy /= BALLISM_RING_CORNERS * 2;
    cz /= BALLISM_RING_CORNERS * 2;
    for (part = 0; part < 2; part++) {
        float x = strip[part * 4] - cx, y = strip[part * 4 + 1] - cy, z = strip[part * 4 + 2] - cz;

        d[part] = x * x + y * y + z * z;
    }
    outside = d[0] > d[1] ? 0 : 1;
    a = strip + outside * 4;
    b = a + 8;
    c = b + 8;
    nx = (float)(b[1] - a[1]) * (float)(c[2] - a[2]) - (float)(b[2] - a[2]) * (float)(c[1] - a[1]);
    ny = (float)(b[2] - a[2]) * (float)(c[0] - a[0]) - (float)(b[0] - a[0]) * (float)(c[2] - a[2]);
    nz = (float)(b[0] - a[0]) * (float)(c[1] - a[1]) - (float)(b[1] - a[1]) * (float)(c[0] - a[0]);
    length = sqrtf(nx * nx + ny * ny + nz * nz);
    nx /= length;
    ny /= length;
    nz /= length;
    for (corner = 0; corner < BALLISM_RING_CORNERS; corner++) {
        const s16 *v = a + corner * 8;
        float x = v[0] - cx, y = v[1] - cy, z = v[2] - cz;

        size += sqrtf(x * x + y * y + z * z);
        top = v[1] < top ? v[1] : top;
        bottom = v[1] > bottom ? v[1] : bottom;
    }
    half = size / BALLISM_RING_CORNERS * BALLISM_RING_THICK / 64.0f;
    p[3] = 0;
    for (end = 0; end < 2; end++) {
        float along = end == 0 ? -half : half;

        for (part = 0; part < 2; part++) {
            for (corner = 0; corner < BALLISM_RING_CORNERS; corner++) {
                const s16 *v = strip + (corner * 2 + (part == 0 ? outside : 1 - outside)) * 4;

                p[0] = (s16)((float)v[0] + nx * along);
                p[1] = (s16)((float)v[1] + ny * along);
                p[2] = (s16)((float)v[2] + nz * along);
                project_point(p, &point[end][part][corner]);
            }
        }
    }
    for (corner = 0; corner < BALLISM_RING_CORNERS; corner++) {
        p[0] = a[corner * 8];
        p[1] = a[corner * 8 + 1];
        p[2] = a[corner * 8 + 2];
        project_point(p, &middle[corner]);
    }
    for (corner = 0; corner < BALLISM_RING_CORNERS; corner++) {
        s32 next = (corner + 1) % BALLISM_RING_CORNERS;
        /* y is down: a side along the top is the most up it gets */
        s32 up = (bottom - (a[corner * 8 + 1] + a[next * 8 + 1]) / 2) * 256 / (bottom - top);
        /* shining down the middle of the outside, as metal does */
        u32 wall = radar_shade(shade, up, 8), edge = radar_shade(shade, up, 4), inner = radar_shade(shade, 256 - up, 4);

        face = radar_quad(&point[0][0][corner], &point[0][0][next], &middle[corner], &middle[next], edge, wall, face);
        face = radar_quad(&middle[corner], &middle[next], &point[1][0][corner], &point[1][0][next], wall, edge, face);
        face = radar_quad(&point[0][1][corner], &point[0][1][next], &point[1][1][corner], &point[1][1][next], inner, inner, face);
        for (end = 0; end < 2; end++) {
            face = radar_quad(&point[end][0][corner], &point[end][0][next], &point[end][1][corner], &point[end][1][next], radar_shade(shade, up, 6), radar_shade(shade, up, 2), face);
        }
    }
    return face;
}

static void draw_ballism_rings(const map_enviroment *scene, const subengine *rings) {
    const MeshHeader *h = (const MeshHeader *)entity_address(rings->entity, (ModelFile *)scene->dbase->data);
    const u8 (*shade)[3] = RadarLook->ring_shade[(rings->entity & 0x7000) == 0x2000];
    const u8 *run;
    const s16 *vert;
    s32 at = 0, face = 0;

    if (h == NULL) {
        return;
    }
    run = (const u8 *)&h->runs + h->runs;
    vert = (const s16 *)((const u8 *)&h->verts + h->verts + sizeof(BoundBox));
    draw_owner_part(h);
    for (; run[1] != 0xFF; run += 2) {
        if (run[1] == BALLISM_RING_FACES) {
            face = draw_ballism_ring(vert + at * 4, shade, face);
        }
        at += run[1] + 2;
    }
}

static void draw_ballism_radars(map_enviroment *scene, MATRIX *view, const BallismRadarLook *look) {
    s32 i;

    RadarLook = look;
    draw_depth(1);
    BallismRadars = scene->subs[0];
    BallismRadars.next = NULL;
    BallismRadars.otz = scene->subs[BallismRadarBases[0]].otz;
    draw_face_list(look->faces, look->num_faces, 1);
    draw_owner(&BallismRadars);
    BallismRadars.hook(&BallismRadars, view, 0);
    draw_face_list(NULL, 0, 0);
    for (i = 0; i < NUM_BALLISM_RADAR_BASES; i++) {
        subengine *piece = &scene->subs[BallismRadarBases[i]];

        draw_owner(piece);
        piece->hook(piece, view, 0);
    }
    for (i = 0; i < NUM_BALLISM_RADAR_RINGS; i++) {
        subengine *rings = &scene->subs[BallismRadarRings[i]];

        if ((rings->entity & 0x7000) == 0x2000) {
            draw_ballism_shaft(rings, &BallismShafts[i], i == 0 ? 1 : -1, view);
        }
    }
    ring_view(scene, &BallismRadars, view);
    draw_owner(&BallismRadars);
    draw_owner_part(BallismRadarRim);
    draw_ballism_radar_back(-1, draw_ballism_radar_back(1, 0));
    for (i = 0; i < NUM_BALLISM_RADAR_RINGS; i++) {
        draw_ballism_rings(scene, &scene->subs[BallismRadarRings[i]]);
    }
    draw_owner(NULL);
    draw_depth(0);
}

static subengine CrashballPipeEnd;

static void draw_crashball_pipe_end(const map_enviroment *scene, MATRIX *view) {
    const subengine *left = &scene->subs[CRASHBALL_LEFT_PIPE];
    MATRIX mirror, turned;

    if (CRASHBALL_RIGHT_PIPE >= scene->num_placements) {
        return;
    }
    CrashballPipeEnd = *left;
    CrashballPipeEnd.next = NULL;
    CrashballPipeEnd.XYZ = scene->subs[CRASHBALL_RIGHT_PIPE].XYZ;
    memset(&mirror, 0, sizeof(mirror));
    mirror.m[0][0] = -0x1000;
    mirror.m[1][1] = 0x1000;
    mirror.m[2][2] = 0x1000;
    matrix_mul(&mirror, (MATRIX *)&left->xform.mtx, &turned);
    CrashballPipeEnd.xform.mtx = turned;
    draw_mirrored(1);
    draw_face_list(CrashballLeftPipeEnd, NUM_CRASHBALL_LEFT_PIPE_END, 1);
    draw_owner(&CrashballPipeEnd);
    CrashballPipeEnd.hook(&CrashballPipeEnd, view, 0);
    draw_owner(NULL);
    draw_face_list(NULL, 0, 0);
    draw_mirrored(0);
}

void wide_scenery_draw(map_enviroment *scene, MATRIX *view) {
    s32 file;
    s32 i;

    if (gfx_view_extra() <= 0 || scene->subs == NULL) {
        return;
    }
    if (NumTried < 0) {
        read_tried();
    }
    list_pieces(scene);
    if (RingTried < 0) {
        const char *r = getenv("WIDE_RING");
        int floor_image, wall_image, wall_images, floor_y, floor_tile, inner, outer, wall_top, sides;
        unsigned floor_colour, wall_colour;

        RingTried = r != NULL && sscanf(r, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%x,%x", &floor_image, &wall_image, &wall_images, &floor_y, &floor_tile, &inner, &outer, &wall_top, &sides, &floor_colour, &wall_colour) == 11;
        if (RingTried) {
            TriedRing.floor_image = (s16)floor_image;
            TriedRing.wall_image = (s16)wall_image;
            TriedRing.wall_images = (s16)wall_images;
            TriedRing.floor_y = floor_y;
            TriedRing.floor_tile = floor_tile;
            TriedRing.inner = inner;
            TriedRing.outer = outer;
            TriedRing.wall_top = wall_top;
            TriedRing.sides = sides;
            TriedRing.floor_colour = floor_colour;
            TriedRing.wall_colour = wall_colour;
        }
    }
    if (RingTried) {
        draw_ring(scene, &TriedRing, &RingOwner, view);
    } else if (NumTried == 0) {
        s32 file = (s32)(scene->dbase->file - DatIndex);

        if (file == FILE_BEACH_BALL) {
            draw_beach_backdrop(scene, &RingOwner, view);
        }
        if (file == FILE_CRASHBALL) {
            draw_crashball_pipe_end(scene, view);
        }
        if (file == FILE_RING_DING) {
            draw_ring_ding_wall(scene, &RingOwner, view);
        }
        if (file == FILE_POGO_PAINTER) {
            draw_pogo_painter_backdrop(scene, &RingOwner, view);
        }
        if (file == FILE_EL_POGO_LOCO) {
            draw_el_pogo_loco_backdrop(scene, &RingOwner, view);
        }
        if (file == FILE_DESERT_FOX || file == FILE_DESERT_FOX_CRYSTAL) {
            draw_desert_fox_backdrop(scene, file, &RingOwner, view);
        }
        if (file == FILE_BEARMINATOR) {
            draw_bear_cliff(scene, &RingOwner, view);
        }
        if (file == FILE_TOXIC_DASH) {
            draw_toxic_dash_backdrop(scene, &RingOwner, view);
        }
        if (file == FILE_POLAR_PANIC) {
            draw_sea_backdrop(scene, FILE_POLAR_PANIC, &PolarSea, PolarRim, NUM_POLAR_RIM, &RingOwner, view);
        }
        if (file == FILE_TILT_PANIC) {
            draw_sea_backdrop(scene, FILE_TILT_PANIC, &TiltSea, TiltRim, NUM_TILT_RIM, &RingOwner, view);
        }
        if (file == FILE_TILT_PANIC_CRYSTAL) {
            draw_sea_backdrop(scene, FILE_TILT_PANIC_CRYSTAL, &TiltCrystalSea, TiltCrystalRim, NUM_TILT_CRYSTAL_RIM, &RingOwner, view);
        }
        if (file == FILE_POLAR_PANIC_CRYSTAL) {
            draw_sea_backdrop(scene, FILE_POLAR_PANIC_CRYSTAL, &PolarCrystalSea, PolarRim, NUM_POLAR_RIM, &RingOwner, view);
        }
        if (file == FILE_BEACH_BALL_CRYSTAL) {
            draw_sea_backdrop(scene, FILE_BEACH_BALL_CRYSTAL, &BeachBallCrystalSea, BeachBallCrystalRim, NUM_BEACH_BALL_CRYSTAL_RIM, &RingOwner, view);
        }
        if (file == FILE_POGO_PADLOCK_CRYSTAL) {
            draw_sea_backdrop(scene, FILE_POGO_PADLOCK_CRYSTAL, &PogoPadlockCrystalPavement, PogoPadlockCrystalRim, NUM_POGO_PADLOCK_CRYSTAL_RIM, &RingOwner, view);
        }
        if (file == FILE_N_BALLISM) {
            draw_ballism_backdrop(scene, &RingOwner, view);
            draw_ballism_radars(scene, view, &BallismRadarLooks[0]);
        }
        if (file == FILE_N_BALLISM_CRYSTAL) {
            draw_ballism_crystal_floors(scene, &RingOwner, view);
            draw_ballism_radars(scene, view, &BallismRadarLooks[1]);
        }

        for (i = 0; i < NUM_RINGS; i++) {
            if (Rings[i].file == file) {
                draw_ring(scene, &Rings[i], &RingOwner, view);
            }
        }
    }
    file = (s32)(scene->dbase->file - DatIndex);
    for (i = 0; i < NUM_EXTRAS; i++) {
        if (Extras[i].file == file && NumTried == 0) {
            draw_extra(scene, &Extras[i], &Copies[i], view);
        }
    }
    for (i = 0; i < NumTried; i++) {
        draw_extra(scene, &Tried[i], &Copies[NUM_EXTRAS + i], view);
    }
}
