# Extra heroes

The PC port's extra heroes (ROADMAP-B.md, 7.12): characters from the player's own copies of other
Crash games, playing as heroes of their own when EXTRA HEROES is on. Ripper Roo, from Crash
Bandicoot (USA), is the first, and his file, `port/game/main/heroes/roo.c`, is the template:
a new hero copies it and changes what's his. This page goes through it a part at a time.

Nothing of another game is kept or shipped. Each hero is made from its disc on the player's
machine every time the port starts (the disc beside `crashbash.exe` or in a `discs` folder,
`disc_image_find`), and lives only in memory.

## How one plays

An extra hero borrows, inside, the number of an original hero nobody's playing that match: its
**base**. The game keeps everything by hero number (speeds, the computer's play, sounds, the
moves, which model each game loads), so the base's all still work, and the extra hero plays the
base's moves with the same timing and reach. What makes it itself:

- **Its models**, one of each **kind** the games' own heroes have: on foot (Crate Crush, and any
  game with no kind of its own for it) and riding a polar bear (Polar Push). Each goes in the
  base's place in the game's tables of models by hero (`HeroHooks.HeroModels`, and the second
  table Polar Push's last arena loads, `HeroHooks.flag`), and wherever the game draws the base's
  model of that kind it draws the hero's (`extra_hero_dbase`, from `prims.c`): Polar Push gives
  each hero its model by name in 108 places, and none of them needed changing.
- **Its frames.** A move names a frame of the base's model, `0x4000 | n << 7 | f`. As the hero is
  drawn, animation n becomes the hero's **stand-in** for it, by name (`extra_hero_frame`):
  Roo walks where a hero runs. A stand-in goes the same share of the way through as the base's,
  or at its own speed (LOOP, ONCE).
- **Its sounds**, in place of all of its base's (below).
- **Its face** on the panel and in the character select: its game's own picture if it has one
  (Roo's hero card), or drawn from its model.
- **Its weight and speed**, where its kind says: on his bear, Roo is as heavy and as quick as
  Crash (`extra_hero_like`, in Polar Push's `ResetMass` and the engine's running speed).

The points the game finds on a frame (the boxes the heroes collide with, the hands that hold a
crate) stay the base's, scaled to the hero's height, so pushing, colliding and carrying all work as
the base's do.

## The files

| File | What |
|---|---|
| `include/bash/herodef.h` | The definition a hero's file fills in (`HeroDef`): the template's shape |
| `include/bash/heromake.h` | What a hero's file uses to make itself: reading its disc, writing Crash Bash's models, riding a bear |
| `main/heroes/roo.c` | Ripper Roo: everything that's his |
| `main/bonusheroes.c` | The making that's shared, the list of heroes (`HeroDefs`), their archive entries and banks (`Slots`), playing their sounds |
| `main/extraheroes.c` | The playing that's shared: bases, tables, frames, points, sounds, faces |

## Ripper Roo, a part at a time

### 1. Who he is: the `HeroDef`

```c
const HeroDef HeroRoo = {
    "RIPPER ROO", "roo", "SCUS-94900", Kanga,
    { { RooStands, "BREATHE", 0x500, 0 }, { RooRides, "BREATHE", 0x1000, Crash } },
    RooVoices, "BREATHE", 0, roo_make,
};
```

- The name, in the game's capitals, and the id a test run's `--extra-hero 0 roo` uses.
- His game's disc, by its serial (`disc_image_find`).
- The base he borrows if nobody's playing it (`Kanga`, Rilla Roo); else the first nobody is.
- A `HeroKind` for each kind of model: its stand-ins, the stand-in for any animation not listed,
  how much of a frame's lift off the ground is drawn (Roo's own jump is a crate or more high, so
  on foot 0x500 of 0x1000), and whose weight and speeds he has (0 for the base's).
- His sounds for his base's, the animation his face is drawn from if he had no card, and his
  make.

### 2. His stand-ins

`RooStands`, on foot: `{ "RUN", "WALK" }` is his WALK where the base's moves run, `{ "JUMP",
"JUMP:13-27" }` frames 13 to 27 of his JUMP, `{ "WIN", "TAUNT", LOOP }` his laugh round and round
at its own speed as he wins. An animation the hero's model has by the base's own name is its own
stand-in, so `RooRides` (riding) is empty: his bear model has all of Polar Push's by name.

### 3. His sounds

Each original hero's sounds are a block of ids from the one the announcer names it with (Crash 1,
Coco 0x33, Cortex 0x65, Brio 0x97, Rilla Roo 0xC9, Dingodile 0xFB, Tiny 0x12D, Koala Kong 0x15F,
50 apart), and the games play the ones for its moves from the first `HERO_SOUNDS` (26) of it.
Every one of the base's is the extra hero's instead (`extra_hero_sound`, from `PlaySfx`): his own
by offset (`RooVoices`), `VOICE_BASE` to keep the base's, and anything not listed is quiet. What
each offset is, from the heroes' voice packs (`vox_*/*.sfx`, their VAB tones) and the moves that
play them:

| Offset | Where | Rilla Roo's | Roo's |
|---|---|---|---|
| 0 | The announcer naming the winner (`UpdateHeroVictory`, step 0) | 0.50 s | hers |
| 1 | Taunt (the taunt move's 0x800A) | 0.46 s | laugh |
| 2-5 | Hit, one at random (`HeroHitSfx`) | 0.18 s | yelp |
| 6 | Cheer as it wins (`UpdateHeroVictory`, step 2) | 0.43 s | laugh |
| 9, 10 | Crate Crush: walking with a crate (`hold_slow`), and the warp rooms' | 0.06 s | none |
| 11 | Crate Crush: jump | 0.06 s | jump |
| 12 | Crate Crush: land; a hit's fall (`BashHeroHit`) | 0.05 s | thud |
| 13 | Crate Crush: attack; and the warp rooms' | 0.35 s | jump |
| 14 | Crate Crush: pick up | 0.21 s | yelp |
| 15 | Crate Crush: throw | 0.23 s | yelp |
| 16 | Crate Crush (not played by the moves) | 0.07 s | yelp |
| 19, 20, 25 | Polar Push: skating on foot (`skate_ft`), one sample | 0.25 s | yelp |
| 21 | Polar Push: a push starts | 0.32 s | jump |
| 22 | Polar Push (not played by the moves) | 0.11 s | yelp |
| 23 | Polar Push: the push | 0.12 s | yelp |
| 24 | Polar Push: win | 0.36 s | laugh |

His own (`VOICE_`), as his make finds them: the laugh, a yelp (the laugh's first "ha", higher),
his jump's call, a light landing and a heavy one. One limit: a game's own sounds with ids in a
base's block are taken too. Only the medieval chicken arena has any (1 to 0x11, in Crash's block),
so there a hero whose base is Crash would quiet them.

### 4. Making him: `roo_make`

His make fills a `HeroMade`: a model and texture set of each kind (docs/ASSETS.md's formats), his
sounds, and his card. From his disc (Crash Bandicoot's formats, `c1_` in heromake.h):

1. **His level.** `c1_level_read(disc, "S1/S0000017.NSF")`, his boss fight; `c1_entry_find` its
   model (`RS1nG`) and animations (`RS1nV` a jump, `RS2nV` standing, `RS3nV` a laugh, `RS4nV`
   knocked over).
2. **His faces and textures.** `c1_model_faces` makes the model's triangles into made faces, each
   corner's colour lit as he stands, and their textures into made images.
3. **His made animations.** His game has no walk and no attack, so they're made from his own
   frames: a rig (his hips, each vertex's share of a leg, his tail), a WALK of two strides, and a
   SPIN in a blue tornado (faces of their own, drawn adding their colour). This is the part a new
   hero writes most of, and only if it needs moves its game hasn't got.
4. **On foot.** A `MadeModel` (faces, images, animations, a frame function giving each frame's
   vertices in Crash Bash's units), written by `made_write`. An animation with more vertices than
   an index can reach (16,384 over its frames) is written as keyframes with the frames between
   blended, as the game's own models are.
5. **Riding a polar bear.** `ride_make` takes Crash's polar model from the player's archive (the
   bear is its strips with only the bear's textures, inside its outline, neither his jeans' blue
   nor his skin's orange) and sits the hero on its back: `seated()` is his pose, thighs out over
   the bear's sides, shins hanging, feet turned forward, tail along its back, his hips just above
   its back (`ride_seat`). Each frame he moves as Crash does on his: the motion of Crash's hips, and
   his lean from hips to head, which Roo takes at his waist and neck. Over that, his own motion
   (`ride_own`): his standing sway as he breathes, his laugh as he taunts and wins. The model has
   Polar Push's seven animations as long as Crash's, and a second mesh, charred, that the game
   draws once a bomb hits him.
6. **His card and sounds.** `c1_sprite64` the 64 x 64 face his boss fight shows (his tongue is his
   health), `c1_sound` his sounds.

### 5. Putting him in

- An `EXTRA_` number for him (`extraheroes.h`).
- His `HeroDef` in `HeroDefs` and his archive entries and banks in `Slots` (`bonusheroes.c`).
- An archive entry for each model and its texture set (main.c's `DatIndex`, from 992, empty until
  his make fills them) and a file group of each pair (`FileGroups`, from 130).
- A bank of each (`modelbank.h`'s `DB_BonusRoo` and `DB_BonusRooPolar`, dbase.c's
  `HeroModelBank`) and their loading (levelload.c's `ModeDbaseLists`).
- A card in the character select (titles.c): the ninth, first of the third row, is Roo's; another
  hero needs one of its own.

### 6. Trying him

```bash
build/port/windows-clang/crashbash.exe --mode 18 --skip-intro --skip-loading --turbo --window 640 480 --extra-hero 0 roo --shot 700 build/roo.png --frames 720
```

Mode 0 is Crate Crush, 18 to 21 Polar Push's four arenas. In a test run the computer plays
player 1, the same way each run. The log says what was made: `bonus: Ripper Roo from Crash
Bandicoot, 1145 triangles, 1 textures, ... riding a bear, his hero card, his laugh, his landing`.
