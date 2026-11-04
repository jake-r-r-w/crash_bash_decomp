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
  game with no kind of its own for it), riding a polar bear (Polar Push), on a pogo stick
  (Pogo), sat in a car (Ballistix, the game drawing the car round it), stood in a tank's
  hatch (Tank, the game drawing the tank round it), sat in a race cart (Crash Dash's tracks but
  the water's, the cart in its own model), riding a fish (Splash Dash, Crash Dash's water track,
  the fish in its own model), with a mallet (the mallet game, the mallet in its own model), riding
  a dino (the gem game, the dino in its own model), and with a keg (the kegs game, the keg in its own
  model); a kind it has no model of, it plays as on foot. Each goes in the
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
  Crash (`extra_hero_like`, in Polar Push's `ResetMass` and the engine's running speed); in a
  tank he has Dingodile's flamethrower and speed whoever's place he has (in Tank's
  `ResetToBasicWeapon`, its speed and its shots' sounds).

The points the game finds on a frame (the boxes the heroes collide with, the hands that hold a
crate) stay the base's, scaled to the hero's height, so pushing, colliding and carrying all work as
the base's do.

## The files

| File | What |
|---|---|
| `include/bash/herodef.h` | The definition a hero's file fills in (`HeroDef`): the template's shape |
| `include/bash/heromake.h` | What a hero's file uses to make itself: reading its disc, writing Crash Bash's models, taking part of one of the game's own, riding a bear |
| `main/heroes/roo.c` | Ripper Roo: everything that's his |
| `main/bonusheroes.c` | The making that's shared, the list of heroes (`HeroDefs`), their archive entries and banks (`Slots`), playing their sounds |
| `main/extraheroes.c` | The playing that's shared: bases, tables, frames, points, sounds, faces |

## Ripper Roo, a part at a time

### 1. Who he is: the `HeroDef`

```c
const HeroDef HeroRoo = {
    "RIPPER ROO", "roo", "SCUS-94900", Kanga,
    { { RooStands, "BREATHE", ROO_KEEP_LIFT, 0, ROO_SUBFRAMES }, { RooRides, "BREATHE", 0x1000, Crash, 1 },
      { RooPogo, "BREATHE", 0x1000, 0, ROO_SUBFRAMES }, { RooCar, "BREATHE", 0x1000, 0, 1 },
      { RooTank, "BREATHE", 0x1000, Dingodile, 1 }, { RooRace, "BREATHE", 0x1000, 0, 1 }, { RooSplash, "SWIM", 0x1000, 0, 1 },
      { RooMallet, "BREATHE", ROO_KEEP_LIFT, 0, 1 }, { RooDinoStands, "BREATHE", 0x1000, 0, 1 },
      { RooKegs, "BREATHE", ROO_KEEP_LIFT, 0, 1 } },
    RooVoices, "BREATHE", 0, roo_make,
};
```

- The name, in the game's capitals, and the id a test run's `--extra-hero 0 roo` uses.
- His game's disc, by its serial (`disc_image_find`).
- The base he borrows if nobody's playing it (`Kanga`, Rilla Roo); else the first nobody is.
- A `HeroKind` for each kind of model: its stand-ins, the stand-in for any animation not listed,
  how much of a frame's lift off the ground is drawn (Roo's own jump is a crate or more high, so
  on foot 0x500 of 0x1000), whose weight and speeds he has (0 for the base's), and how many of his
  model's frames make one of his game's (on foot 4: below, step 4; in a car, a tank, a race cart, on a fish, with a mallet,
  on a dino and with a keg 1).
- His sounds for his base's, the animation his face is drawn from if he had no card, and his
  make.

### 2. His stand-ins

`RooStands`, on foot: `{ "RUN", "WALK" }` is his WALK where the base's moves run, `{ "JUMP",
"JUMP:13-27" }` frames 13 to 27 of his JUMP, `{ "WIN", "TAUNT", LOOP }` his laugh round and round
at its own speed as he wins. An animation the hero's model has by the base's own name is its own
stand-in, so `RooRides` (riding) is empty: his bear model has all of Polar Push's by name. On his
cane (`RooPogo`) he has Pogo's by name too, and `{ "TAZING", "TAZING", LOOP }` plays his tazing
at its own speed, as his base's is a short loop; in a race cart (`RooRace`) his BREATHE goes at
its own pace the same way, as the base's is as short as 10 frames.

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
| 0 | The announcer naming the winner (`UpdateHeroVictory`, step 0) | 0.50 s | "Rilla Roo" (*) |
| 1 | Taunt (the taunt move's 0x800A) | 0.46 s | laugh |
| 2-5 | Hit, one at random (`HeroHitSfx`) | 0.18 s | yelp |
| 6 | Cheer as it wins (`UpdateHeroVictory`, step 2) | 0.43 s | laugh |
| 9, 10 | Crate Crush: walking with a crate (`hold_slow`), and the warp rooms' | 0.06 s | none |
| 11 | Crate Crush: jump | 0.06 s | jump |
| 12 | Crate Crush: land; a hit's fall (`BashHeroHit`) | 0.05 s | thud |
| 13 | Crate Crush: attack; and the warp rooms' | 0.35 s | jump |
| 14 | Crate Crush: pick up | 0.21 s | none |
| 15 | Crate Crush: throw | 0.23 s | Crash's throw |
| 16 | Crate Crush (not played by the moves) | 0.07 s | none |
| 19, 20, 25 | Polar Push: skating on foot (`skate_ft`), one sample | 0.25 s | none |
| 21 | Polar Push: a push starts | 0.32 s | jump |
| 22 | Polar Push (not played by the moves) | 0.11 s | none |
| 23 | Polar Push: the push | 0.12 s | none |
| 24 | Polar Push: win | 0.36 s | laugh |

(*) The game has no name for an extra hero, and its base's is another hero's (with Coco's place,
Roo "won" as Coco). So a hero has a `VOICE_NAME` of its own, short enough to be done before the
announcer's "wins!" (0x1B0), which comes 0.77 to 0.87 s after the name, by its base. Roo's is the
announcer's "Rilla Roo", the nearest name the game has, whoever's place he has: sound 0xC9, the
first of her block, from `sfx/vox_common/rilla.sfx` in the player's archive (`hm_game_sound`; if it's
not there, his laugh's first 0.65 s, cut at a quiet gap). A win is then his name, "wins!", and his
laugh as he cheers, as the others' go. All the games' wins go through `UpdateHeroVictory`.

His own (`VOICE_`), as his make finds them: the laugh, a yelp (the laugh's first "ha", higher),
his jump's call, a light landing and a heavy one; and one of the game's own, Crash's throw, as his game has no throw of
his (`hm_game_sound`: a tone's sample out of a sound pack in the archive, at the pitch the game plays it). He yelps only as he's
hurt. One limit: a game's own sounds with ids in a
base's block are taken too. Only the medieval chicken arena has any (1 to 0x11, in Crash's block),
so there a hero whose base is Crash would quiet them. And the other way round, a game's voice by
hero outside the blocks isn't caught: Splash Dash's are (its yelps as a hero's hit, 0x685 to 0x68C,
and the calls four of them give as they set off, 0x67D, 0x67F, 0x681 and 0x684), so its code plays
an extra hero's own there (race.c: the yelps of its base's block, as on the other tracks, and its
`VOICE_JUMP` where its base calls out).

### 4. Making him: `roo_make`

His make fills a `HeroMade`: a model and texture set of each kind (docs/ASSETS.md's formats), his
sounds, and his card. From his disc (Crash Bandicoot's formats, `c1_` in heromake.h):

1. **His level.** `c1_level_read(disc, "S1/S0000017.NSF")`, his boss fight; `c1_entry_find` its
   model (`RS1nG`) and animations (`RS1nV` a jump, `RS2nV` standing, `RS3nV` a laugh, `RS4nV`
   knocked over).
2. **His faces and textures.** `c1_model_faces` makes the model's triangles into made faces, each
   corner's colour lit as he stands, and their textures into made images.
3. **His made animations.** His game has no walk and no attack, so they're made from his own
   frames: a rig (his hips, each vertex's share of a leg, his tail), a WALK of two small hops, and a
   SPIN in a blue tornado (faces of their own, drawn adding their colour). This is the part a new
   hero writes most of, and only if it needs moves its game hasn't got.
4. **On foot.** A `MadeModel` (faces, images, animations, a frame function giving each frame's
   vertices in Crash Bash's units), written by `made_write`. Each animation has four frames to each
   of his game's (`ROO_SUBFRAMES`): his game's are its keyframes (`MadeAnim.step`) and the frames
   between are blended. His base's moves are longer than his own (Rilla Roo's idle is 20 frames, his
   12), and with only his own he'd hold some of his frames for two of theirs, stepping, and slowly
   at 120 frames a second. His stand-ins at their own speed play four a tick, and a range in a
   stand-in (`JUMP:13-27`) is in his game's frames. An animation with more vertices than
   an index can reach (16,384 over its frames) is written as keyframes with the frames between
   blended, as the game's own models are. What's loose on him, his tongue and his ears, swings as
   his head is thrown about (`foot_swing`): his head's path through each of his game's frames, as
   it's drawn (with the lift `keep_lift` takes off), goes through `hm_flop`, a damped spring
   (`Flop`: a swing's frames, its damping, how much of the throw it takes, the most it swings),
   settled into the loop in the ones that loop (his breathing, laugh and walk). So they flap with
   every hop, carrying a crate too (`HOLD_SLOW` is his walk).
5. **Riding a polar bear.** `ride_make` takes Crash's polar model from the player's archive (the
   bear is its strips with only the bear's textures, inside its outline, neither his jeans' blue
   nor his skin's orange) and sits the hero on its back: `seated()` is his pose, as on a horse:
   thighs out over the bear's sides, knees bent so the shins go back under, his long feet poking out
   at the sides and tipped a little down, his tail tucked away into its root inside the bear (laid
   behind him, it only cut through the bear's rump), his hips down in its fur where Crash sits
   (`ride_seat`). Each
   frame he moves with the bear's back (a fit of its corners there: Crash's own hips wobble with
   his legs), and leans as Crash does on his, hips to head, as the world sees it: smoothed over a
   few frames, then turned into the bear's frame, so he leans smoothly against every pitch of its
   gallop; and a share of it kept back toward upright (`steady`), as a tall hero's head swings
   further for the same lean. Roo takes it at his waist and neck. Over that, his own motion
   (`ride_own`): a little of his standing sway as he breathes, and his laugh as he taunts and wins,
   each blended between its frames and the laugh faded in and out. What's loose on him, his tongue
   and his ears, swings on the same spring at his head as the bear throws it about (`flop`: each
   vertex's share, 1 at the tips; `swing`, his `Flop`), settled into the loop in one that loops. The animations are as long as
   his base's (`match_bank`, Rilla Roo's), Crash's resampled to them, so the base's moves play them
   a frame a tick; and there's a second mesh, charred, that the game draws once a bomb hits him.
6. **On his cane (Pogo).** Pogo's heroes bounce on pogo sticks, each a model of its own with its
   stick (`models/chars/pogo/`, five moves each: BREATHE, JUMP, DAZED, WIN, TAZING). Where Ripper
   Roo is the arena's boss (`DB_Roo`, `models/arena/pogo_epl/roo2.mdl`) he bounces on a stick of
   his own, a cane that telescopes: squeezed short between his feet as he lands, it shoots out and
   throws him high, and stays below as he flies. `hm_game_part` takes part of one of the game's own
   models from the player's archive (the strips a `keep` picks, as made faces and images, and
   `hm_model_at` for where its corners are in any frame): `keep_cane` picks the boss's cane, his
   strips within 16 of his middle across and below 400. `pogo_make` puts Roo on it: his feet on the
   cane's top as it squeezes and shoots out (`perched`: his long splayed feet turned in about the
   ankles and his legs drawn together, so they go side by side over the crook), and once the boss goes
   higher than it (the middle of the boss's body against his bounce's bottom), flying above it, his
   legs thrown out as in his jump's splits and in again to land on it; his tongue and ears swinging
   (`hm_flop`, a tick at a time). `PogoMoves` says what each of Pogo's moves is: the boss's bounce; the boss's
   hop, eased from and back to his bounce's bottom as the base's hop lands on it; dazed, a stagger
   through his knocked-over animation's frames before he falls, on the squeezed cane; a laugh
   through a bounce as he wins; and the boss's attack, the cane flipped up and fired, as he tazes.
   Each is as long as his base's (`hm_bank_frames`, Rilla Roo's pogo model's) at four frames to
   each of hers; the tazing at the boss's own speed.
7. **In a car (Ballistix).** The game draws a car round each hero (`DB_BallsVehicle`, its meshes
   4 and 1) and the hero's own model in it, the four of them round the arena, so each is seen from a
   side of its own: from behind, the front and either side. The car's handles are in the heroes'
   models, not the car's, so Roo's has them too, Rilla Roo's taken with `hm_game_part` and rocking
   as hers do in the same move (`hm_model_at` of her frame), though he can't hold them. For big
   heroes the game draws the near car by its own order, 16 nearer (`AR_InitHero`), and the port
   draws heroes by real depth, so the handles and car swapped as the car bobbed; an extra hero's
   car is drawn by real depth too. `car_make` sits Roo in it as big as the others (1.25 of his size), back in its
   seat (`car_seated`: thighs forward, shins down, his long feet turned in, his tail tucked, all of it
   inside the car, his straitjacket and head above it). He has no hands for its handles, so he
   drives it with his head (at the user's word): BANK, which the game sets from his sideways speed
   each tick and holds at frame 9 or 23 of 28 as he goes one way or the other, smacks his head down
   on the car's side the way he goes (`car_bank`), and he comes back upright as he stops. He sways
   as he breathes, laughs as he fidgets (IDLE_A), taunts and wins (bouncing in his seat), and slumps
   back as he loses (DIE); each move as long as Rilla Roo's ball model's, a frame to each of hers
   (his moves there are worked out for each of her frames, so they don't step).
8. **In a tank (Tank).** The game draws each hero's tank round it (the hull and its weapon's
   turret, or the swamp's hover tank) and the hero's own model stood in the hatch
   (`models/chars/tank/`, as big as on foot, nothing below the hatch). `tank_make` stands Roo in
   it, his straitjacket's bottom at the turret's top; what of him is below, his legs, feet and tail,
   isn't drawn (all of it at one point inside, so its faces have no size; at the user's word, as his
   long feet poked out through the hull). Tank's moves: he sways, looks about as he waits (IDLE1),
   ducks into the hatch for a mine and pops up again (MINE: no hands to drop it), is thrown back as
   he's hit, slumps down over the turret as he loses, folded over its front with his head hanging
   over the edge, and laughs, bouncing, as he wins.
9. **In a race cart (Crash Dash).** On its tracks but the water's (Splash Dash, where the heroes
   ride fish, `models/chars/splash/`), each hero's model is it in its cart (`models/chars/race/`, all
   the carts the same size; five moves: BREATHE as it drives, RUN as a boost stretches the cart out
   and back, HIT dazed, FALL as it drops off the track, WIN). `dash_make` takes Rilla Roo's cart
   (`keep_cart`: her strips all below 220 as her model rests, as she's above it) and moves it as hers
   moves in the same move and frame. The heroes' carts are each a colour of their own (Crash's blue,
   Coco's pink, Cortex's yellow, Brio's green, Tiny's dark purple, the rest orange) and Rilla Roo may
   be racing too, so it's painted a colour of his own, red (`hm_paint`: the colours of its faces and
   images of about one hue, its oranges and yellows, turned to a paint's, each keeping its shade; his fur's blue was tried first, but
   that's Crash's, and he blended into it), but for its lights (her texture set's image 10,
   `GamePart.image_of`), yellow as every cart's are. He sits in it as big as the biggest (Brio), back against its back, legs
   as in a Ballistix car, and is thrown about as she is in it (the middle of her body from where it is
   as she drives: back as it boosts, up as it's knocked, down as it falls). He sways as he drives,
   laughs as he boosts, thrown back into his seat, his head goes round and round as he's dazed, he
   leans back as he falls, and laughs, bouncing, as he wins.
10. **Riding a fish (Splash Dash).** On the water track each hero's model is it riding a fish
   (`models/chars/splash/`; the mounts go in pairs: Crash and Coco a pink dolphin, Cortex and Brio a
   grey shark, Tiny and Kong an orca, Dingodile and Rilla Roo a blue fish; five moves: BREATHE sat up,
   SWIM as it swims and waits, TRANS as it sets off, FALL as it dives, WIN). It's a mount
   (`mount_make` with `RooFish`, a `Mount`: what's ridden, as the dino is too, step 12): Rilla Roo's fish
   (`keep_mount`: her model's first 16 strips, as it's before her in it), moving as hers
   does, painted his red as his cart is (`hm_paint` from its blue, as he's blue too). He sits astride
   it as on the bear (`astride`, his legs less spread on the narrower fish) and moves as its back does
   by his seat (`hm_fit`: the turn and move that best take its corners there from as it rests to a
   frame, Horn's, shared with the bear). He has no hands, so he holds on with his tongue (at the
   user's word): `mount_rein` stretches it from his mouth to the top of the fish's head, its own shape
   kept about the way there, as a rein. He sways as he sits up, crouches forward as it swims, leans
   into it as it sets off, is thrown back as it dives, and laughs, bouncing, as he wins: each of a
   mount's moves is one of those motions of his (`MountMove`).
11. **With a mallet (the mallet game).** Each hero's model is it with its big spiked mallet
   (`models/chars/mallet/`; six moves, BREATHE, MEDIUM as it runs, HIT knocked down, ATTACK the
   swing, WIN, and one by a name only as its hash, #4a7, that Rilla Roo's sequences don't play).
   `mallet_make` takes Rilla Roo's mallet (`keep_mallet`: her model's first 16 strips, its head, and
   its last six, its handle). He holds it in his mouth (at the user's word) as a dog carries a stick:
   its handle across his mouth, bitten in its side, its end poking out on his tongue's side and the
   rest out of the other, a little forward and up from level (out ahead from his mouth, or angled out
   of its corner, it looked less real). It goes through the middle of his mouth's inside, between his
   jaws (`InMouth`: the reds of his model behind his lips; his tongue's root, where it was first, is
   lower, on his lower jaw, so it looked under his mouth), and its shaft is a third as thick across
   (about the line along it, a fit of its corners), as hers is a club for her big hands and his mouth
   is about 34 tall. It moves with his head (`hm_fit` of his head's
   vertices from standing to the frame). He's as on foot (`mallet_own`: his own animations, between
   their frames): standing, hopping as he runs, falling over as he's hit, laughing as he wins. He
   swings it with his whole body (`mallet_bend`), timed as hers by the share of the way through
   ATTACK, whichever hero's place he has (every base's sequences name frames of its own, and his
   frames go the same share of the way): rearing back for the backswing, back further as it's
   charged, twisting round to carry it behind him, then bowing forward and twisting the other way,
   faster and faster, to sweep it round and down onto the floor in front as hers lands
   (`mallet_twist`), and up again. Where the game finds a swing's hit, the base's point on its frame, it stays.
12. **Riding a dino (the gem game).** Each hero's model is it riding a small dino, each a colour of
   its own (`models/chars/dino/`: Crash's blue, Coco's pink, Cortex's yellow, Brio's teal, Tiny's dark
   purple, Kong's and Dingodile's reds, Rilla Roo's orange; five moves: BREATHE, RUN, SLIDE as it
   skids, RECOIL as it shoots, WIN). The same mount ride as the fish (`RooDino`): Rilla Roo's dino, her
   model's first 5 strips (a crest down its middle) and all from the 45th (its body, head, legs and
   tail), painted red from its oranges and yellows; him astride its back, his tongue a rein to its
   head; crouched forward as it runs, thrown back as it skids, jolted back as it shoots (`GO_KICKED`),
   laughing as he wins. Its corners are few by his seat, so a mount says how far round it looks for
   the top of its back (`seat_near`: at the fish's 40 only the dino's feet were near, and he sat in it).
   Dingodile's dino is red too, with a green crest: they're told apart by their riders.
13. **With a keg (the kegs game).** Each hero's model is it with the keg it carries all game, if it
   has one (`models/chars/kegs/`: Rilla Roo's and Crash's in their second mesh, which all their moves
   move, the first without it never drawn; Cortex's in his only one; Tiny's has none; five moves,
   BREATHE, MEDIUM, JUMP up, falling and landing, HIT and WIN). `keg_make` takes Rilla Roo's keg (her
   second mesh's first strip: `hm_game_part_of` takes part of any mesh of a model, `hm_game_part` the
   first's) and balances it on his head, lying as she carries it, a little down into his crown (at the
   user's word, that or under his chin, whichever looked best: as big as it is, under his chin it would
   cover his neck, chest and tongue); it goes with his head as the mallet does. He's as on foot:
   standing, hopping as he runs, jumping as he does on foot (from frame 13 of his jump, its crouch left
   out), falling over as he's hit, laughing as he wins.
14. **His card and sounds.** `c1_sprite64` the 64 x 64 face his boss fight shows (his tongue is his
   health), `c1_sound` his sounds.

### 5. Putting him in

- An `EXTRA_` number for him (`extraheroes.h`).
- His `HeroDef` in `HeroDefs` and his archive entries and banks in `Slots` (`bonusheroes.c`).
- An archive entry for each model and its texture set (main.c's `DatIndex`, from 992, empty until
  his make fills them) and a file group of each pair (`FileGroups`, from 130).
- A bank of each (`modelbank.h`'s `DB_BonusRoo`, `DB_BonusRooPolar`, `DB_BonusRooPogo`, `DB_BonusRooBall`, `DB_BonusRooTank`, `DB_BonusRooRace`, `DB_BonusRooSplash`, `DB_BonusRooMallet`, `DB_BonusRooDino` and `DB_BonusRooKegs`, dbase.c's
  `HeroModelBank`) and their loading (levelload.c's `ModeDbaseLists`).
- A card in the character select (titles.c): the ninth, first of the third row, is Roo's; another
  hero needs one of its own.

### 6. Trying him

```bash
build/port/windows-clang/crashbash.exe --mode 18 --skip-intro --skip-loading --turbo --window 640 480 --extra-hero 0 roo --shot 700 build/roo.png --frames 720
```

Mode 0 is Crate Crush, 5 to 8 Tank's four arenas (5 the swamp's hover tanks), 10 to 13 Pogo's,
14 to 17 Ballistix's, 18 to 21 Polar Push's, 23 to 26 Crash Dash's (25 the water's, Splash Dash), 28 the gem game, 29 the mallet game, 30 the kegs game.
`--extra-hero N roo` for each of players 0 to 3 puts him on every side at once. In a test run the computer plays
player 1, the same way each run. The log says what was made: `bonus: Ripper Roo from Crash
Bandicoot, 1145 triangles, 1 textures, ... riding a bear, on his cane, in a car, in a tank, in a race
cart, riding a fish, with a mallet, riding a dino, with a keg, his hero card, his laugh, his landing`. A test run needs a window (a small one will do) once a
mode draws: with `--no-window` it crashes there (AGENT_LOG.md), though his models are made, and
written for `BONUS_DUMP`, before that.

A boss fight plays through the boss test, from a real Adventure: `node port/tests/playtest.mjs
bosses oxide --hero roo` (mode 31 alone only starts its space phase, and nobody fights). Oxide's
fight draws the heroes' Ballistix models in its ships, so Roo is in his car there: he won it as
Roo. Two looks were left as they are, at the user's word: its ball phase loops the whole steering
move as a hero moves (Ballistix holds it at the lean for the way he goes), so his head smacks down
on one side then the other; and in its space phase he sits higher in the ship than the others, as
his car model is bigger and sat back for Ballistix.
