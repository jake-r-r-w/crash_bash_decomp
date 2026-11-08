# Extra heroes

The PC port's extra heroes (ROADMAP-B.md, 7.12): characters from the player's own copies of other
Crash games, playing as heroes of their own when EXTRA HEROES is on. Ripper Roo, from Crash
Bandicoot (USA), is the first, and his file, `port/game/main/heroes/roo.c`, is the template:
a new hero copies it and changes what's his. This page goes through it a part at a time.

Nothing of another game is kept or shipped. Each hero is made from its disc on the player's
machine every time the port starts (the disc beside `crashbash.exe` or in a `discs` folder,
`disc_image_find`), and lives only in memory.

## Adding a hero: the checklist

A new hero is one line in a list and a file of its own; everything else comes from the line. Go
through it in order, and show the user each game's model before starting the next: they test by
playing.

1. **Its line.** `X(NAME, HeroName)` in `EXTRA_HERO_LIST` (`include/bash/herolist.h`). From it
   come its `EXTRA_NAME` number, its place in `HeroDefs`, its card in the character select, and,
   for each kind of model, its archive entries, file group, bank and loading (below, 5). The third
   row has room for four cards, in the list's order.
2. **Its file**, `main/heroes/name.c`: a copy of `roo.c` with what's his taken out. It holds its
   `HeroDef` (below, 1), with its game's disc by serial and the base it would rather have, and its
   make. Another game's formats need readers of their own, beside Crash Bandicoot's (`c1_` in
   heromake.h).
3. **On foot** (below, 4, steps 1 to 4): its model, faces and textures. Make any moves its game
   hasn't got, and give it stand-ins for its base's (below, 2). Try it in Crate Crush and Ring Ding
   (below, 6). Most of the work is here, and in the games where it rides or holds something.
   - Make its moves 4 frames a tick (`subframes` 4, as Roo's and Pinstripe's): another hero's
     longer moves would otherwise hold its frames for two ticks, and it steps at 120 frames a second.
   - It won't always have its own base: a test run's player 1 has Crash's place, and in a game it
     has another's when someone has its own. Each hero's moves are its own scripts, so give it
     **sequence stand-ins** (below, 2) for the ones that play parts of an animation or none: the
     jump, fall and landing, the attack (Crash's and Coco's spin is a still mesh), the hit, the
     taunt, picking up and throwing a crate. Try it as player 1 and as player 3 (another place).
   - Check its game's moves are the size of the pose it's rigged from: by a rigid piece (a shoe, a
     hand), not the whole body. Pinstripe's still is 1/1.2 the size of his moves.
   - Look at it the way the game draws it: only faces' fronts, and with a depth test. A game drawn
     back to front (the PS1's) can have layers drawn in order whatever their depths, which a depth
     test cuts into slivers (Pinstripe's eyeballs, pupils and lids), and holes nobody saw (his palms).
   - Fit it to Crash Bash's look. The game's heroes are bright and soft, their shading mostly in their
     textures: a corner's colour no more than about a quarter either way from the rest of its colour's.
     A light that leaves the side away from it flat looks dark and faceted by them (`C1Light`, `wrap`).
     Their idles move their corners 2 to 4% of their height on average, and their runs 11 to 43%;
     moves made much stiller than that look stiff beside them. Their eyes are their textures: eyes made
     of a few polygons (Pinstripe's pupils were slits) look broken by them, and an eye painted in code
     (`eye_image` in pinstripe.c) does better.
4. **Its sounds** (below, 3): a voice for each offset of its base's block, or none where one would
   be wrong. Its name as it wins has to be short enough for the announcer's "wins!" after it. An
   attack with a sound of its own (Pinstripe's tommy gun) is `VOICE_FIRE`, at offset 13.
5. **Its card and portraits**: its game's own picture if it has one (`HeroMade.card`, any size
   by `c1_sprite`), or one drawn from its model (`HeroDef.face`). It shows in the select, on the
   panel and in the menus after. The select stretches a card to fill its portrait (Roo's), unless
   `card_aspect` says how wide its pixels are as its game shows them (Pinstripe's 64 x 32, 0.625 on
   Crash Bandicoot's 512-wide screen).
6. **Each other kind** (below, 4, steps 5 to 13), one at a time: riding a polar bear, on a pogo
   stick, in a Ballistix car, in a tank, in a race cart, riding a fish, with a mallet, riding a
   dino, and with a keg. Each step says what it takes from the game's own models and how it's put
   on the hero. `hm_game_part` (part of any of the game's models) and `mount_make` (a ride, for the
   fish and the dino) are already written. A kind it has no model of, it plays as on foot.
7. **The check**: `node port/tests/playtest.mjs hero ID` plays it in every game, four of it at once,
   each on a base of its own (`--one` makes only player 1 it). It passes when:
   - its model of every kind is made;
   - each game draws every one of them in that game's kind;
   - none is on foot for want of a model;
   - every kind it has was played.

   Then look over its sheet (`build/port/playtest/hero/ID/sheet.png`), and the user plays each game.
8. **The bosses**: `node port/tests/playtest.mjs bosses --hero ID` beats each boss as it. Oxide's
   fight draws its Ballistix car model in the ships.
9. **Its baseline**, once the user approves it: `node port/tests/playtest.mjs same --hero ID --save`.
   After that, `same --hero ID` shows in a minute whether it still draws and sounds the same after
   a change to its making or to the extra heroes' code.
10. **The words**: the EXTRA HEROES setting's hint (`overlays/titles/settingsmenu.c`) and the
    release's README.txt say who's there. Game facts found go in ARCHITECTURE.md. Then the roadmap
    (ROADMAP-B.md, 7.12.7) and AGENT_LOG.md.

**Done:** Ripper Roo (`roo`, from Crash Bandicoot (USA), SCUS-94900). The user played and approved
every game (2026-11-04) and called him done (2026-11-05). Two things are left out at their word: his
name is never written in the game, and the podium and win screens weren't looked at with him.

**Under way:** Pinstripe (`pinstripe`, the same disc, `main/heroes/pinstripe.c`): on foot (step 3,
2026-11-05); every other game plays him on foot for now. His model has his tommy gun in every frame,
and at the user's word he has it only as he attacks: its faces (and its front grip, in his left
hand's piece) are on copies of their corners of their own, folded into his hands the rest of the
time. His arms are rigged (two bones each, from a shoulder, elbow and wrist placed by hand), as his
game's animations never take them off the gun. The user played him in Crash's place and found his
hands glitchy (the front grip), his size changing as he taunted (his game's moves are 1.2 times his
still), a jump after each throw (Crash lands after a throw), no gun in his attack (Crash's spin)
and a stiff frame rate (held frames): all fixed, his jump, attack, hit, taunt, pickup and throw now
going by the sequence. Then (2026-11-06) his palms were still see-through: his hands close round the
grips, so without them each palm is a hole, and the game draws only faces' fronts. Each is closed by a
cap fanned across the hole's edge loop, which folds away as the gun comes out. As he wins he shoots his
gun about laughing, as in his boss fight (its PiG2V, at the user's word), for both wins (`@WIN`,
`@VICTORY`). Then, at the user's word, his face's slivers near his eyes went (his game drew his eyeballs,
pupils and lids in order; his still's pupils and lids are lifted a little forward, and his game's moves
have his still's eyes), and he was made to fit the game's look: a brighter, wrapped light (he'd been
darker and duller than the others, 42% of his faces one flat colour), an idle that bobs at the knees and
a jog with his arms pumping, moving as much as the game's heroes do. Then, at the user's word: his head
1.2 times as big, grown about his neck in every frame; his pupils, which looked cross-eyed (in his still
one looks aside), on corners of their own in the middle of his eyes; his muzzle flash added to what's
behind it, unlit, as his game's colour slot says (it had been drawn solid, black round it); and as he's
hurt, his arms flung out and back by his rig, then hanging (his game had them on the gun). His eyes were
still wrong by the game's heroes, whose eyes are their textures (the user's diagnosis): his eyeballs are
painted now with an eye made in code, of nobody's art (yellow, a round pupil, its glint), laid flat
over each eye as he faces at one size for both, and his pupils' polygons are left out. The user called
the rest his design and moved on to his other games (2026-11-06). Riding a polar bear (step 6, the
first): sat astride Crash's (`ride_bear`, `seated`, `pose_astride`), at 0.8 of his size about his
hips (at his own his long legs reached the floor beside it), his hands on its neck. How it's done is at
the top of his file. The user asked him to ride more like Crash and Coco, without the jetpack push, and
for the gaps in the bear closed: his push is one now (`@POLAR_ATTACK`, his stand-ins below), and the
bear's back and side are filled for every rider (step 5). Then the user saw him sit stiffly as he was
hit off the edge, where the others hang on and flail ("look carefully how some other hero falls like
crash"): at the ice's edge he goes as Crash does (`ride_own`, from Crash's body measured frame by frame,
step 5), thrown back off the bear's rump with his arms flying, back up if he gets back on, and left in
the air as the bear drops away; and his fall's teeter and drop now meet the game's (Splits, step 2).
The user approved his bear (2026-11-06). On a pogo stick (step 6, the second): Pogo's heroes ride things
of their own (Crash and Coco pogo sticks, Cortex and Brio their jetpacks, Tiny and Kong springs), and
as he rides as Crash does he bounces on Crash's stick (`pogo_make`): its strips of Crash's pogo model,
picked by where they are as Crash first bounces (the model's own pose has his arms out, and a first
pick by it took his arms and legs), its tube 1.3 times as long above its pegs so the handle's at his
waist, its blues gunmetal against his red suit. He stands on it at 0.8 of his size, his feet on its
pegs and his hands on its grips (two-bone limbs put to them), and moves as Crash does in the stick's
own frame: Crash springs up off the pegs at the top of each bounce, his limbs stretching to keep hold,
so Pinstripe's hips and feet move as Crash's (for his height, 1.3 as far) and his limbs stretch the
same way; his body turns as Crash's, and a hand Crash lets go with (punching the air as he wins) goes
as Crash's arm goes. Pogo's five moves are Crash's; his win and tazing at their own speed, as
Cortex's are 87 and 4 ticks. His lean (`pose_body`) had gone back for a lean above 0, though his
moves were written as if forward (his crate pickup leaned back as he knelt, his jog a little back): at
the user's word (2026-11-08) it leans forward, so his pickup reaches down for the crate and he leans
into his jog, jump and throw; his bear is as approved (`RIDE_LEAN` is a lean back now) and his pogo
model byte for byte the same. In a Ballistix car (step 7, the third): his pogo stick's making became
one for any kind where Crash holds or stands on something (`follow_make`, a `FollowKind` each: the
prop's strips, its frame if it has one, its grips and pegs, Crash's hips, chest, shoulders, elbows,
hands and feet, his size, lean and squat), and his stick came out of it byte for byte the same. In
the car he has Crash's handles (`CarKind`: the levers, Crash's textured strips all 210 or more out to
the sides), holds their grip bars' inner ends as they rock, sits where Crash sits (his hips on
Crash's, his legs folded in the car), and lets go with a hand as Crash does to fidget, wave, cheer
and wipe his face. The levers' posts are red in the model, but inside the car; only their grey grips
show, as Crash's do. Ballistix's six moves are Crash's (BANK has HOLD's name hash: "HOLD" and "BANK"
hash alike), each over all of his base's. The user saw his hands off the grips: aiming his hand's
middle at a grip left it hanging under the bar, as he stands with his hands round the gun's pistol
grips. So a hand that holds a grip (`pose_arm_grip`) puts the middle of its hole, where the gun's
grip went through it (`hand_hole`: the palm's cap loop, its axis by Newell's method), on the bar
(`grip_in` out along it from the car's grip's inner end, as Crash's fist covers that end), turns
the hand at the wrist so the hole runs along the bar (both bars are along x), and puts the wrist
where that hand needs it, worked out a few times over. His pogo handle's grip is the same now. The
user approved his car (2026-11-08). In a tank (step 8, the fourth): Crash's tank model is him from the
waist up and nothing else, the game drawing the tank round him, so `TankKind` has no prop (`follow_make`
then follows all of Crash's model and draws none of it), no grips (his arms go as Crash's do, all the
time: Crash's hands found by where they are, `FollowKind.hand`), and everything of Pinstripe's below
his hips put at one point just under them (`cut`), as the others have nothing below the hatch. Crash
rests his hands on his belly by the hatch, and a hips set with them in it rose out of the hatch as he
raised his arms to cheer (Pinstripe floated above the turret in a win's close-up): the tank's hips and
chest sets keep to his back half, and his body's line across comes from his chest's two halves
(`across_chest`), as his shoulders' corners go with his arms. The user approved his tank (2026-11-08)
and had his last five games' models made in one go, to look over at the end. In a race cart (step 9,
the fifth): Crash's race model is him from the waist up in the cockpit and his cart, so `RaceKind`
takes the cart (its strips with a corner where Crash isn't: below his seat or out past his elbows) and
paints it gunmetal from its blues (`hm_paint`, a blue-grey half as bright, as his pogo stick is), its
jets' lights yellow as every cart's are (a kind's `paint` now paints the prop's faces and images at
once). Its frame is the cockpit's rim, which goes forward and back as a boost stretches the cart and
down as it drops off the track. He holds the front of the cockpit by the dashboard as Crash does,
sits in Crash's seat as big as on foot (as Cortex is in his), and all of him below his hips is hidden
in the cart, as in the tank. Crash's body stretches as he drives, cheers and drops (his chest from his
hips 1.25 times as far as he bobs, 1.7 as he wins, nearly 3 as the cart falls away under him), and so
does Pinstripe's (`FollowKind.stretch`: his chest and all above it lifted, his body between his waist
and neck drawn out); his sway as he drives goes at its own pace (Cortex's is half as long). Riding a
fish (step 10, the sixth): Crash rides a pink dolphin, so Pinstripe rides it too, painted the same
gunmetal (from its pinks; its white belly and eyes kept), at 0.8 of his size as on the bear (`FishKind`:
the dolphin is Crash's model's strips from the 40th on, as it's after him in the model, and its frame
is its middle, which bobs, leaps out of the water and flips over as he wins, and spins round as it
dives). Crash goes all over it, so Pinstripe follows all of him (`FollowKind.legs`, Crash's hips,
knees and ankles): his hands and feet go where Crash's are from his shoulders and hips (for his size),
his elbows and knees bent Crash's way and his shoes turned with his shins, and his limbs stretch as
Crash's do: Crash sits up on it, lies along it holding its head as it swims, kicks his legs up behind
as it sets off, goes up and over with it as it flips, and does a handstand on it as it dives, holding
on with his arms stretched long, and so does Pinstripe. Following only which way Crash's arms pointed,
his own arms' length left him floating above the diving dolphin (`FollowKind.reach` now: his hands
where Crash's are). With a mallet (step 11, the seventh): Crash carries his wooden mallet over his
shoulder in his left hand, and swings it, slams it down and cheers with it in that hand alone, so
Pinstripe does too (`MalletKind`: the mallet is Crash's model's strips from the 40th on, its handle its
corners below its head). He's on foot as big as in Crate Crush (`FollowKind.stand`: on the floor, his
hips moved as Crash's are and his feet as far off their places as Crash's go, for his height), his
hands where Crash's are, and the mallet goes with his left (`FollowKind.carry`): its handle through
his hand's hole (the line along it a fit of its corners, his hand turned so the hole runs along it,
as on the car's grips), as big against him as Crash's is against Crash, so its head lands on the floor
as he slams it down, as Crash's does (at Crash's size, held higher in his bigger hand, it would stop
short of it). Cortex's sixth
move, #4a7, Crash's model hasn't: his BREATHE stands in. His cheer goes at its own pace, as
Cortex's is twice as long.

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
  or at its own speed (LOOP, ONCE). The moves are each hero's own scripts (`HeroSeqTables`, by
  `HeroInfo.hero_aseq`), and a **sequence stand-in** is for all of one, whatever frames of the
  base's it shows: over as many ticks as the script shows frames for, or at its own speed.
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
| `include/bash/herolist.h` | The list of heroes, a line each, and where each one's models go |
| `include/bash/herodef.h` | The definition a hero's file fills in (`HeroDef`): the template's shape |
| `include/bash/heromake.h` | What a hero's file uses to make itself: reading its disc, writing Crash Bash's models, taking part of one of the game's own, riding a bear |
| `main/heroes/roo.c` | Ripper Roo: everything that's his |
| `main/bonusheroes.c` | The making that's shared, the heroes by number (`HeroDefs`), putting their models in the archive, playing their sounds |
| `main/extraheroes.c` | The playing that's shared: bases, tables, frames, points, sounds, faces, the select's cards |
| `overlays/titles/titles.c` | The character select: the extra heroes' cards, a third row |

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

A stand-in from `"@"` and a sequence's name (`SeqNames` in extraheroes.c: the shared ones, then
Crate Crush's `BA_` and the jungle game's `PA_`, and on in the order of the heroes' tables, heroes.c's
`CTX_seqs`, through Polar Push's `POLAR_`) is for the whole of that sequence. Each hero's
scripts differ: Crash's jump, fall and landing are frames 0-7, 8-11 and 12-38 of his 41-frame JUMP,
Cortex's 0-7, 7-12 and 13-18 of 20; Crash and Coco land after a throw; their attack is a still mesh
(their spin) for 16 ticks, the others' an animation of 12 to 19; Coco, Tiny and Kong taunt forwards
and back. So Pinstripe has `{ "@JUMP", "JUMP:0-7", ONCE }`, `@FALL` and `@LAND` (his JUMP split
as Cortex's is, each part's last frame held as long as the game takes), and `{ "@ATTACK", "ATTACK" }`,
`@HIT`, `@TAUNT_A`, `@BA_PICKUP` and `@BA_HOLD_THROW`, each all of his over the base's script. On
his bear he has `{ "@POLAR_ATTACK", "PUSH" }`: Cortex's push (`CTX_seq_polarattack`, 37 ticks) plays
its frames 8 to 13 three times, his jetpack, which on Crash's bear was a push lunging three times
(the user: not like Crash's); his is one push as long as the script, as Crash's and Coco's are one
of 20. Once at its own speed it ended with him still, sliding as the game pushed. The game's push
makes things of the base's own too (`PolarHeroAttackThrow`): Cortex's and Brio's pack, flames out of
their backs and a smoke trail, and Dingodile's and Rilla Roo's tails, each a part of their models. An
extra hero in their place makes none (the user saw the flames on Pinstripe). It's
used only while what's drawn is the frame the script shows. Roo has none, and plays as before.
A "@" stand-in goes by the time since its sequence started, so it can't follow a script that jumps:
Polar Push's fall (`seq_polarfall`) is its teeter for its first 32 frames in every hero's script and
then the drop (8 frames in Cortex's and Brio's, 10 in the others'), and a hero hit too hard starts at
the drop (`SetPolarHeroOffice`, Aindex 0x21). So its FALL goes by its frames, in two parts at 32
(`Splits` in extraheroes.c): the base's teeter frames to the rider's, and the rest of what the script
plays to the rest of the rider's (`script_last`). The rider's FALL is Crash's 42 frames for it
(`ride_make` doesn't match it to the base's length); all of it over all of Cortex's 50 had had his drop
start 6 frames into the base's.
The scripts are in heroes.c (`CRA_seq_jump` ...) and bash.c (`CRA_BA_seq_hold_throw` ...), as u16s
(the s32 arrays low half first): the first is the ticks past one each frame shows, `0x8007 n` the
frame after it and n - 1 more, `0x8018` a new speed, `0x8004` the next action (ProcessHeroAnim).

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
   nor his skin's orange; and in those strips a face of his jeans' blue or their shadow, by its
   colour or, as his jeans share the fur's image, by its texel at its middle, is painted the bear's
   fur: they showed on its back beside Pinstripe's thinner legs, and left out they were gaps). The
   bear is still open where Crash covered it, its back between its shoulders and rump and its left
   side from its neck to its rump (one loop of 23 open edges), so
   `bear_fill` closes it for any rider: a rounded body just inside the bear's (`FILL_*`), kept only
   where none of the bear's faces is within reach along its facing (179 of 352 faces), each corner
   moved as its nearest four of the bear's are (above its legs), and lit as its fur is, its charred
   colours 100 darker as the bear's are. Models draw with real depth (prims.c, `draw_depth`), so
   what's inside the bear's own fur stays hidden. It then sits the hero on its back: `seated()` is his pose, as on a horse:
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
   a frame a tick (but FALL, Crash's own, step 2); and there's a second mesh, charred, that the game
   draws once a bomb hits him. A rider can follow Crash's body itself where sitting still and leaning
   wouldn't do (`Rider.follow`, its own motion then not bent as he leans): `ride_crash_body` gives, in
   the bear's back's frame, how far his hips have moved, his body's turn (his spine from hips to chest
   and his shoulders' line, `crash_frame`: a fit of his chest's corners followed his cartoon stretch as
   the bear drops and turned him back 60 degrees while he was upright) and each arm's upper arm and
   forearm, from his corners near his shoulders, elbows and hands as he first breathes (`CrashJoints`).
   Pinstripe's FALL is that (`ride_own` in pinstripe.c): each arm turned from how he sits as Crash's
   turns from how Crash sat, then more and more Crash's very direction as it goes further, his body
   turned at the waist as Crash's is, his legs astride, all of him moved with Crash's hips.
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

His line in `include/bash/herolist.h`, `X(ROO, HeroRoo)`, is all of it:

- His number, `EXTRA_ROO` (extraheroes.h), and his place in `HeroDefs` (bonusheroes.c).
- For each kind, at `BONUS_SLOT` (his place in the list times the kinds, plus the kind):
  - an archive entry for its textures and one for its model, two from 992 for each, empty until
    his make fills them (main.c's `DatIndex`);
  - a file group of the pair, from 130 (`FileGroups`);
  - a bank, `DB_Bonus` on (modelbank.h; dbase.c's `HeroModelBank`);
  - and its loading, from 96 (levelload.c's `ModeDbaseLists`).
- His card in the character select (titles.c). The extra heroes' cards are a third row under the red
  one, four at most, in the list's order. Each card's portrait and frame come after the game's own
  pieces. The computer picks each free one as often as any one hero still free.

### 6. Trying him

In every game at once, with a contact sheet of them (the checklist, step 7):

```bash
node port/tests/playtest.mjs hero roo
```

One game:

```bash
build/port/windows-clang/crashbash.exe --mode 18 --skip-intro --skip-loading --turbo --window 640 480 --extra-hero 0 roo --shot 700 build/roo.png --frames 720
```

The modes:

- 0 to 3: Crate Crush.
- 5 to 8: Tank's four arenas (5 is the swamp's, with hover tanks).
- 10 to 13: Pogo.
- 14 to 17: Ballistix.
- 18 to 21: Polar Push. In 21, Manic Panic, a hero can be on foot as well as on its bear.
- 23 to 26: Crash Dash. 25 is the water track, Splash Dash.
- 27: Ring Ding, on foot.
- 28: the gem game.
- 29: the mallet game.
- 30: the kegs game.

`--extra-hero N roo` for each of players 0 to 3 puts him on every side at once. In a test run the
computer plays player 1, the same way each run.

The log says what was made, in his make's words (`bonus: Ripper Roo from Crash Bandicoot, 1145
triangles, ...`) and in the checks' (`bonus heroes: roo is made: a model on foot, riding a polar
bear, ...`). As a game starts, it says what each player who's an extra hero is drawn as: `extra
heroes: player 1 is roo, in hero 8's place: riding a polar bear`.

A test run needs a window once a mode draws (a small one will do). With `--no-window` it crashes
there (AGENT_LOG.md), though his models are made before that.

A boss fight plays through the boss test, from a real Adventure: `node port/tests/playtest.mjs
bosses oxide --hero roo` (mode 31 alone only starts its space phase, and nobody fights). Oxide's
fight draws the heroes' Ballistix models in its ships, so Roo is in his car there: he won it as
Roo. Two looks were left as they are, at the user's word: its ball phase loops the whole steering
move as a hero moves (Ballistix holds it at the lean for the way he goes), so his head smacks down
on one side then the other; and in its space phase he sits higher in the ship than the others, as
his car model is bigger and sat back for Ballistix.
