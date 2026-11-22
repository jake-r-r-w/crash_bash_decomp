# Hero packs

A hero of your own in the PC port: a glTF model in a mod, playing in an original hero's place as Ripper Roo and Pinstripe did
([EXTRA_HEROES.md](EXTRA_HEROES.md), retired: they were removed for packs on 2026-11-21, with the disc reading they were made
from). Unlike them it needs nothing of another game: the pack holds only its creator's model,
clips, sounds and card, and the game makes it into Crash Bash's models as it loads. The plan, in phases, is in the session plan of
2026-11-21 (the hero tool, every kind of model, borrowed moves, online), and this page grows with it.

**Where it stands:** a pack's hero has a model of every kind the game's heroes have: on foot and on the podium from its own moves,
riding the polar bear (`pack_ride`, through `ride_make`), and in the eight kinds where the heroes ride or hold something else
(`packfollow.c`): Crash's model of that kind followed frame by frame, as Pinstripe's were made, his tables of where Crash and his
prop are copied, but posing the pack's own skeleton: its hips on Crash's, its trunk turned as his, each arm reaching (two-bone) for
his hand, on the grip where there's one, its legs astride as his, on the pegs or the floor where he stands, else sat, and in
a tank's hatch or a race cart its tail folded away and its legs drawn in to the hatch's width (its arms left where they are), unless
its pack says not (Fit in seat, below: then it's as it's posed, and its maker fits it, hiding bones); the prop
(painted gunmetal where Pinstripe's was) as it moves, or in its hand where he carries it. Where its body isn't Crash's shape: on the
pegs its feet go to his soles (his shins stretch as he bounces) and its hips come down to where its legs reach them; what it carries
turns out of its head (a ball fitted to all of it above its shoulders), and the keg (kept Crash's size) is pushed out of its side, its
hand reaching after it.
Bobby, the example, plays in every game (each checked with four of him), and online where everyone has him (Online, below). The
hero tool (below) makes a pack of any standing two-legged model, rigged or not. A pack can take
another hero as its template, its moves and props in every game (any of the eight; Templates, below).

**The character select's room.** Each of its slots of drawn pieces (`EngineSlotSubs`) held the PS1's 32, and the select's own 26
with two a card and their row's band left room for two cards: a third (the first pack's, beside Roo and Pinstripe then) spilt into the
next slot, the menus' backdrop, which went black after the select. They hold `ENGINE_SLOT_SUBS` (40) now, room for all four.

## A pack

A folder `heroes/<id>/` in an ordinary mod (a zip or folder in `mods/`, [MODDING.md](MODDING.md)); `platform/mods.c` finds it, and a mod
with only heroes in it counts as usable. The first four heroes of the mods that are on, in the mods' order, take the four pack slots
(`EXTRA_PACK_LIST` in `herolist.h`), whose cards come first in the character select's third row. A hero id in two mods is the higher
one's.

| File | What |
|---|---|
| `hero.json` | How it plays (below) |
| `hero.glb` | Its mesh, skeleton, clips and textures |
| `card.png` | Its card in the character select, any size; without one, a face drawn from its model (`face`) |
| `voice_JOB.wav` | Its sound for a job, any WAV: `name` (the announcer's, short enough for "wins!" 0.77 s after), `laugh` (taunting, winning), `yelp` (hurt), `jump`, `land`, `thud` (a fall), `throw` and `fire` (an attack's own sound). The game makes each PS1 ADPCM at 22050 Hz as it loads the pack (`pack_adpcm_encode`). |

### hero.json

```json
{
  "format": 1, "id": "bobby", "name": "BOBBY", "prefer": "crash", "height": 520,
  "face": "BREATHE", "face_turn": 20,
  "kinds": { "foot": { "subframes": 2, "otherwise": "BREATHE",
                       "stands": [["MEDIUM", "RUN"], ["WIN", "WIN", "loop"], ["HIT", "HIT", "once"]] } },
  "voices": { "1": "base", "2": "none" }
}
```

- `prefer`: the original hero it borrows if nobody's playing it (`crash`, `coco`, `cortex`, `brio`, `tiny`, `kong`, `dingodile`,
  `rilla`). The tool writes the same hero here and as `"template"` (Templates, below), one choice it calls **Based on** (the user's
  word, 2026-11-22: two settings that seemed to say the same thing); the game still reads the two apart, so an older pack whose two
  differ plays as before.
- `height`: standing, in the game's units (Crash 452, most heroes about 650). It's the one number of its model the game's play sees
  (the base's collision points are scaled to it), so it's a whole number in the JSON, never worked out from the mesh.
- `kinds`: by name, `foot`, `polar`, `pogo`, `ball`, `tank`, `race`, `splash`, `mallet`, `dino`, `kegs`, `warp`. Each has its
  stand-ins for its base's animations (`[from, to, pace]`, pace `stretch`, `loop` or `once`: `HeroKind` in `herodef.h`, and
  EXTRA_HEROES.md, How one plays), `otherwise`, `keep_lift` (0 to 4096), `like` (a hero's name) and `subframes` (1 to 4).
  `tank` and `race` can say `"fit": false`: not drawn in to sit in the hatch or the cart (the tool's Fit in seat off).
- `face_head`: the share of its model, from the top, that's its head, for a face drawn from it on the panel and in the select
  (0.36 if it's left out); Bobby's is 0.26, as his long ears hang past his chin. A face drawn so stops where the panel's health bar
  starts.
- `voices`: each of its base's 26 sound offsets (EXTRA_HEROES.md, His sounds): a job (`"1": "laugh"`), `base` or `none`. An offset
  it doesn't name plays the job it does for the game's own heroes if the pack has that job's sound (0 name, 1 and 6 and 24 laugh, 2 to
  5 yelp, 11, 13 and 21 jump, 12 thud, 15 throw), else its base's.
- `adjust`: its moves adjusted by hand (Adjusting its moves, below), each `{"kind", "move", "bone", "at", "until", "spread", "ease", "blend", "turn": [x, y,
  z], "reach": [x, y, z], "scale": [x, y, z]}`. A scale of 0 (0 to 10) makes the bone and all on it nothing: hidden.

### hero.glb

glTF 2.0, binary. The platform reads it with cgltf (`platform/heropack.c`) into plain C; `game/main/packheroes.c` makes the models.

- Axes: glTF's, y up, facing +z, in any unit: it's scaled so its rest pose is `height` tall, its lowest point on the floor.
- Every node is a bone. Skinned meshes go with their skin's joints; a mesh on a node with no skin goes with that node.
- Clips: named `MOVE` or `foot/MOVE` for on foot (`BREATHE`, `RUN`, ...), in capitals as the game names them; another kind's are
  `KIND/MOVE` (`ball/BANK`), for the phases to come. A clip's second is 30 of the game's ticks, times `subframes` frames: a move has
  128 frames at most, so a clip is at most 4.2 s at 1 subframe, 2.1 s at 2, 1 s at 4. No `BREATHE` clip: its rest pose stands in.
- Limits, the game's: 1365 triangles (a frame's 4096 corners) and 4096 vertices, 8 images.
- Materials: the base colour (factor, `COLOR_0` and texture, PNG only, made a power of two up to 256 texels and 255 colours), alpha
  mode BLEND for a half-see-through face, `KHR_materials_unlit` for an unlit one. Its light is baked into its corners' colours as
  the game's heroes' is (a soft wrapped light, `PackLight`).
- Winding: glTF's front faces (counter-clockwise) are drawn the other way round, as the game's front is the other winding once a
  model's in its axes. Drawn as glTF has them, a pack's hero showed the inside of its far side and none of its near: its face and
  chest missing, see-through, everywhere (worst on the character select's stands, seen close).
- Depth: a pack's model is always drawn with a real depth test (`prims.c`), where the game draws one with a layer of its own (the
  character select's stands) back to front face by face, which its own models were made for and others aren't.

## Trying one

```bash
build/port/windows-clang/crashbash.exe --mode 0 --skip-intro --skip-loading --window 960 720 --mods-dir port/mods --mod bobby --extra-hero 0 bobby
```

`--extra-hero` names a pack's hero by its id once the mods are read. `node port/tests/playtest.mjs heroes` loads the packs in
`port/mods/`, and `hero ID` plays one in every game. The log says what was made: `hero packs: bobby made, 1318 triangles, ...`.

## Templates

A pack's **template** is the original hero whose model of each kind it follows: that hero's moves there, and what it rides or holds
(its pogo, car, cart, fish, mallet, dino, keg), the pack's own body in that hero's place, at that hero's size. `"template": "tiny"` in
hero.json for every kind (Crash if it's not said), or `"template"` in a kind's own entry for that kind alone; on foot `"own"` there
keeps its own moves though it says a template (a pack that says none has its own on foot anyway). **Its own moves** take the
template's place move by move: a clip in its glTF named for a game's move (`ball/BANK`, heropack.c's kind names) is played there in
place of following (`own_frame`: its skeleton posed by that clip across the length of the move it replaces, its hips on the template's,
the prop moving as the template's move has it, or in its own hand where it's carried). A rigged model's clips keep their names, so a
clip named so in Blender does it; the tool writes one of its standard moves under that name (`--own ball/BANK=WIN`, or the window's
**Its own moves in the games** list, by game, move and which of its own). The tool's **Template** list writes it, and its preview shows what it gives. The
heroes have the same moves by name in each kind (`BREATHE`, `JUMP` ... on a pogo for all eight), so a template changes how a move
looks, never which moves there are.

**How it follows.** Crash's kinds are `packfollow.c`'s own tables (Crash's joints and props by boxes of his models). Another hero's
are **maps** (`port/game/main/packtemplates.c`, `bash/packtemplate.h`): for its model of a kind, markers in its first move's first
frame, in the game's units (y down), each pair its left (x below 0) first: hips, chest, head, shoulders, elbows, hands, the legs' hips,
knees, ankles and feet, and on the prop its grips and pegs where Crash's kind has them. `map_kind` makes Crash's kind that hero's: the
same moves, flags and numbers (cut, stand, carry, legs, reach), its model (`bank`, and `mesh1` where its model has fewer meshes, as
Tiny's keg has one), not painted, and the pack at `scale` x that hero's height (`PackTemplateHeight`) / Crash's. A joint's corners
are its marker's 6 nearest (`map_fit`, at least `r` away and at most 5 `r`; the chest's on both sides of its middle too), so a marker
inside a big body still finds its surface; the prop is the strips with most of their corners in the map's boxes (`TmplBox`, `sym`
for either side). Where a hero has no map of a kind yet the pack follows Crash's there, and the log says so.

**Making a hero's maps**, a kind at a time:

1. Look at its model: `tools/dk python3 tools/scripts/hero_view.py tiny pogo` draws it front (x, y) and side (z, y) on a grid of
   the game's units, each strip a colour (`scratch/hero-view-tiny-pogo.png`), in its BREATHE's first frame, the pose the follower
   takes the markers from (`--move NAME`, `--frame F` another; the file's order isn't BREATHE first in every model). The heroes face -z. Read off where its joints are and what its prop is.
2. Write its map in `packtemplates.c` (its kind's Crash table in `packfollow.c` says what's needed: a grip, pegs, legs astride).
3. Make a pack with it: copy `port/mods/bobby` somewhere, add `"template": "tiny"` to its hero.json, and
   `crashbash --mods-dir THERE --mod bobby --dump-hero bobby DUMP`. The log says `follows hero 5 pogo stick`, or which parts found
   too few corners (`hips 0, chest 1, ...`: move those markers onto it).
4. Look at it: `tools/dk python3 tools/scripts/hpv_sheet.py DUMP bobby pogo ball ...` (`--turn 90` from the side) draws each kind a
   row, a few frames of two moves, each face the colour of its texture's middle texel (a face whose middle is clear isn't drawn:
   the tool's preview, which draws every texel, settles it). A part of the hero left on the prop is a box too big; a pack stretched thin is a part found on
   one side only. `tools/dk python3 tools/scripts/hero_strips.py ENTRY MESH X` lists the strips whose middle is left of x X, each
   with its box, for a prop a box misses (most of a strip's corners have to be in it: Dingodile's keg reaches in to his body).
5. Look at the prop as the map takes it: `tools/dk python3 tools/scripts/prop_view.py tiny pogo` draws the model in its own colours
   (texel times corner colours), the strips the map keeps bright and the rest dim, its boxes outlined and every strip numbered
   (`--only` the kept alone; `--box x0,y0,z0,x1,y1,z1[,sym][,faces|not|skip]` and `--skip N,...` to try others before writing them). It
   prints the strips kept and the faces, which are the faces the game makes of it (all 56 maps checked against `--dump-hero`).

**A prop's boxes** (`TmplBox`, up to 8; packtemplate.h): `BOX`/`BOTH` take the strips with most of their corners in them. Where a
hero's strip runs on into its prop, or lies over it, there's more:

- `FBOX`/`FBOTH` take the faces all in them, of strips with a corner in them: Coco's pogo grips run on into her arms, and Tiny's and
  Kong's pogo plates have their toes' strips touching them.
- `NOTBOX`/`NOTBOTH` leave out the faces all in them, whatever took them: Coco's hands in her race cart, her shoes on her dolphin,
  Cortex's trousers on his shark.
- `SKIPBOX`/`SKIPBOTH` leave out the strips mostly in them: Tiny's legs and tail on his dino, where its own back's faces are within
  his legs' box.
- `.skip`, a list of strip numbers ended by -1 after the map's last field, leaves those strips out: where the hero's and the prop's
  strips are too mixed for boxes (Kong's legs and feet about his dino's neck and arms; most of Dingodile's and Rilla Roo's).

**Where it stands (2026-11-22):** every hero is a template: Crash by his own tables, and Coco, Cortex, Brio, Tiny, Koala Kong,
Dingodile and Rilla Roo each by eight maps (pogo, car, tank, race cart, fish, mallet, dino, keg), each looked at prop by prop in
`prop_view.py`, the props their own (Tiny's purple cart and orca, Coco's pink dolphin and raptor, Cortex's shark ...), with none of
their heroes left on them: Coco's pogo has its handlebar, Tiny's, Kong's and Rilla Roo's mallets their handles, Dingodile's pogo its
whole rocket, and the dinos their arms. What differs between them, all in the maps:

- **A jetpack**: Cortex's and Brio's pogo models are them and their jetpacks; the game draws the hover board under them from its
  jets. Their maps' prop is the jetpack, its faces behind their backs (`FBOX`, z from 25). The game draws the jets (pogo.c, `PostMovePOGO`) for a pack whose pogo template is theirs, in any hero's place
  (`extra_hero_looks`), at the point on its frame where theirs are (0x200): a template's model carries its frames' points, carried
  as its prop is (`MadeModel.points`), and the game finds a pack's own where its base's frame has none (`extra_hero_points`).
- **No hands**: Coco's and Brio's race carts have only their heads, necks (and Brio's small hands): a hand marker takes the prop's
  corners too, so the pack's hands go where theirs would be.
- **Another mesh**: the keg is in the second or third mesh of some heroes' models (`mesh1`: Coco's 3, Brio's 3, Kong's 2, Rilla's 2;
  Tiny's, Cortex's and Dingodile's models have one).
- **The other hand**: Cortex and Brio carry the keg on the right, and Brio his mallet (`carry` 2).
- **A thin prop**: one too small to fit a frame to (fewer than 8 corners) takes the model's own frame.

**On foot, the podium and the polar bear** (2026-11-21, later):

- **On foot**, a pack that says its template (`"template"` in hero.json, Crash too; the tool's window always writes it) stands and
  moves as that hero does in the crate games: its BREATHE, HIT, SKATE, SLIDE and WIN (`PackFootMoves`) followed from that hero's
  crate2 model (`FootKind`, at the pack's own size, its feet on the floor), each at the pack's subframes. A clip `foot/MOVE` of its own
  takes one's place; its plain clips (the tool's standard moves) are its other moves on foot (RUN, JUMP, ATTACK ...), and a stand-in
  that would send a followed move to another is left out. A pack that doesn't say one (Bobby's, from before) keeps its own moves.
  Crash has a foot map like the others (`FOOT(1, ...)`): his tables never had on foot.
- **The podium** is made from its model on foot (its WIN, its rest, HIT as it loses), so it's its template's win.
- **The polar bear**: another hero's template follows that hero's polar2 model (`PolarKind`, its LIGHT on a second, charred mesh as
  ride_make's has it); Crash's is still `ride_make` on his bear. The bear is the same bear in every hero's model, and its rider's
  strips run into it, so it's taken face by face (`bear_cut`): all three corners in its box (`POLAR`: ±90 across, below its back;
  every bear's body is 83 either side) and the colour of its fur, the texel at the face's middle times its corners' colours. Its fur
  is warm and greyish: not bluer than its red or green, not greener than its red, its red at most 0.3 over its green, its channels
  close. Black is the bear's (its nose, eyes and soles) only below its back and drawn as they are, untextured or in an image its pale
  fur uses too; a rider's shoes are black as well. Then its holes, where its rider covered it, are filled as Crash's are
  (`hm_fill_make`, ride_make's fill for his): the same rounded body, kept under the bear's own faces too (`under`: dropped only
  where one is just inside it, else a slit opens across its back by its rump), fitted to the bear's box as his is to his, and lit as much
  darker as its own colours are (Brio's bear is dark). Seen alone from four sides (`tools/scripts/hpv_bear.py` on a `--dump-hero`
  folder), all seven bears are whole, with none of their riders left on them (2026-11-21).

## Online

A pack's hero plays online where every machine has the same pack (`platform/net.c`; PORT.md, Online play). Packs are files, so
they're compared, not sent: `mods_hero_hash` hashes a hero's id and every file of its folder, by name and bytes, added up so the
order a folder or zip lists them in doesn't matter. A joiner's ask (T_JOIN) carries the hashes of the heroes of the mods it has on;
the host's start (T_START) carries the first four of its own that every machine has, in its order (there's no switch: a hero mod
that's on puts its heroes in the select, `extra_heroes_on`, and online only those every machine has count, so all agree). Every machine then makes only those (`mods_heroes_only`; `mods_apply_online`), in the same slots, so the same cards
are in the select and the computer picks the same. A pack a machine lacks just isn't played, and the host's log says why
(`bobby isn't played: machine 2 hasn't its pack, or has another`). A game's record (`--net-record`) names its packs by hash
(`hero ...`), and `--replay` needs them in a mod that's on.

The hero tool's export of the same model differs between Windows and Linux in a few float bits (each system's maths library), so
packs are shared as their files: the same files match whatever made them.

What they change of the game's state is the same on every machine: its whole numbers from hero.json, and models, sounds and their
sizes made by the same program from the same files and the same game data. `playtest.mjs online --heroes` plays a Battle with
Bobby as player 1 on two machines, from the command line and from the launcher, one where the joiner hasn't him, and the record
played again; all four match step for step.

**A restart keeps them.** An online game starts the game again from its beginning (`platform_restart`), which puts the game's data
back as it was at the start (`overlays_restart`). The packs' slots, `HeroDefs`, what's made of them and their pictures' caches
(`packheroes.c`, `bonusheroes.c`, `extraheroes.c`) are between `OVL_KEEP` and `OVL_HERE` (`ovlsection.h`): ordinary data, not
put back, as they point to memory that's freed and made again when the packs change. A slot whose hero changes forgets its pictures
(`extra_hero_forget_pictures`).

## Bobby, the example

`port/mods/bobby/` is made by the hero tool, as anyone's hero is:

```bash
build/herotool/windows-clang/herotool import port/mods/bobby-model.glb --id bobby --name BOBBY --mod-name Bobby     --author "Crash Bash PC" --markers port/mods/bobby-model.glb.markers.json --out port/mods/bobby --height 540 --face-head 0.26
```

He's the user's dog, a long-haired black-and-tan miniature dachshund, stood up as a cartoon hero. His model is the user's own, made
from photos of him (`port/mods/bobby-model.glb`, beside the markers, as a folder there would be a mod of its own): 1365 triangles,
the two smallest of its first export's 1367 taken out at the user's word, one texture, no rig, so the markers
(`bobby-model.glb.markers.json`) place his skeleton, ears, jaw and tail too, and `arm_raise` keeps his short arms from going into his big
head. The photos aren't in the repo. Its command says no template (no `--based-on`): his pack never has, so on foot he plays his own
moves; made again so, the pack is the one in the repo byte for byte.

His ears (2026-11-22, the user's word: "his ears are not wired up correctly i cant move them"): their markers ran down the back of
his head, behind the flaps and short of their ends, so most of each long flap went with his head and its lower half with his chest.
Now each ear's two bones run down the middle of its flap, from its root on his head to its end beside his cheek, and the tool counts an
ear as wide as a head (`Wide`, rig.c: a flap is wide, its corners far from its bone), so each flap goes with its own ear, the top half
with the first bone and the lower half with the second.
Then still "his ears are attached to his shoulders": a corner goes with the two bones nearest it over the surface, smoothed, and for
most of an ear's that second was his chest (an arm's for some), so as his arms or chest moved the flaps' ends went with them. Now what
hangs off a hero (each ear, the jaw, the tail; `Hanging`, rig.c) goes only with its own bones and the one it hangs from (the head,
the hips for a tail), and nothing else with them but that one, where they join.
Then "look at the ear": turned in the tool, the left ear pulled a fin of faces down to his shoulder. The fault was in his model: the
bottom edge of each ear flap was welded to his chest and upper arm, 13 faces a side, so no weights could free it. At the user's word
("if it's a problem from the model itself, fix the model in blender then re-import") `port/mods/bobby_ears.py` cut each ear's corners
shared with his body free in Blender (headless, the faces as ear or body from `bobby_ears.json`), moving them 2 mm into the ear: the
same 1365 triangles, 20 more corners, nothing else moved. And the tool keeps each bone's nearest places in one patch (in each piece
of a model only its biggest patch; the rest go with whatever's nearest over the surface), so a shoulder's place nearer an ear's bone
isn't the ear's, nor an ear's end nearer the shoulder the shoulder's. Turned hard now, his ear moves and nothing of his body does.

## The hero tool

`herotool/` at the repo's top, its own CMake project (C, SDL3, cgltf), sharing the game's pack reader (`port/platform/heropack.c`),
so a pack it writes is one the game reads. It makes anyone's model a hero:

```bash
cd herotool && cmake --preset windows-clang -DCMAKE_PREFIX_PATH=<SDL3's install> && cmake --build --preset windows-clang
build/herotool/windows-clang/herotool import MODEL.glb --id ID --markers MARKERS.json --out FOLDER [--name NAME] [--based-on HERO]
                                       [--height N] [--face-head SHARE] [--author NAME]
```

**The window**: `herotool` with no arguments, or a model alone (`herotool MODEL.glb`), opens it (`src/gui.cpp`, Dear ImGui on
SDL3's renderer, the model drawn by a little software renderer of its own). It's as big as its screen wants (`ui_scale`, the user's
word, 2026-11-22: far too small on a high resolution screen), on the screen the mouse is on: its display scale, or its height against
an 800-line screen's if that's more (1.25 on a 1080-line screen), in quarters; its font made at that size, its style's sizes, the
window and its pictures with it, and again as it's moved to another screen. Ctrl and + or - make it a tenth bigger or smaller, a
share of the screen's kept for next time (zoom.txt in its own folder), Ctrl and 0 sets it back (`HEROTOOL_SCALE=S` for a test). With no model it starts on its **start screen**
(2026-11-22, the user's word): **Create a new hero** (Browse, the system's file chooser; a path; or a `.glb` dropped on it), or
**Edit a hero**, a list of every hero in the mods folder's unzipped mods (`heroes/<id>/hero.json`). Editing one opens the pack's
own `hero.glb` (rigged on the standard skeleton, its moves its clips, so anyone's shared pack can be changed without its first
model) and takes its name, height, who it's based on (its template, else whose place it takes) and face from hero.json, the mod's name and author from mod.json, and its
card and sounds from its files; **Save** writes it back into that mod. `< Start screen` goes back to it (`HEROTOOL_LOAD=ID`
loads a hero, for a test). Drop a `.glb` on it (or a `.png` for its card, a `.wav`
for the sound job picked, a markers `.json`). Its front and side views show the model at rest with its markers, to drag onto its
joints (its left's mirror its right's, a bone's end is the next one's start, and its weights are worked out again on letting go);
the third plays the move picked, turning. A model with no markers beside it (`MODEL.glb.markers.json`, saved there on each
export) starts from a guess (`kit_guess_markers`: a standing humanoid's proportions in its box, arms out if it's as wide as a
T-pose; Jaw, Ears and Tail add theirs). A rigged model's bones are its own, no markers. Then its name, height, who it's based on, card,
sounds and the mod's name, and **Save** writes the mod to the mods folder (beside the tool by default, the game's if it's
there), or **Save and play** starts the game too, with it as player 1 in the game picked. `HEROTOOL_SHOT=FILE.png`
saves a picture of it and closes it, and `HEROTOOL_EXPORT=FOLDER` exports as it opens, for tests.

**Its pictures** are drawn by the tool itself, on the processor (`draw`, `draw_made`), so drawing them is kept down (the user's
word, 2026-11-22: "it lags abit"; a frame was 54 ms, 31 of them the model's three views drawn on every page and 22 the game's
picture): a picture is drawn only while its page shows it, and again only when what it's drawn from has changed (`drawn_same`: the
view's turn, tilt, zoom and size, and the frame's vertices; `MadeGen` counts the game's models read in), so a still picture costs
nothing and a playing move a picture a frame of the game's (30 a second). A triangle is filled row by row across only what it
covers, its corners' shares added as it goes (`fill_tri`), and a picture in bands of rows on as many threads as the machine has, to 8
(`in_bands`; each band's pixels its own, in the same order, so the picture's the same). A move's fit to the picture is found once.
Now (on a 16-thread machine, its pictures at 2.2 times): a game's page still 0.3 ms a frame, playing 3.8, every game at a glance 7,
the model's page 4.6. `HEROTOOL_PROFILE=1` prints a frame's parts every 60 frames, and `HEROTOOL_PREF=FOLDER/` keeps a test's
previews and zoom in a folder of its own (not the open tool's).

**The editor** (2026-11-22, the user's word: one window, simple; a UX pass): three columns. On the left the hero's pages: **Model
and joints** (its rig, or the dots to place on its joints, and the tool's own moves on it), **All games at a glance** (every kind of
model at once, each playing), and each game. In the middle the page's picture; on the right its choices, then **Hero details**
(name, id, height, **Based on**: its template and whose place it takes, one choice), **Card and sounds** (Browse, the system's file
chooser; **Face**, how much of it from the top is its head for its face, shown only with no card) and **Save** (**Save**, **Save and play** in a game picked). A game's page has **Prop from** (that game's
ride or prop and moves from another hero than its template) and **Own animations**: only the game's moves (KitGames) that are its own
there, its own copy of its template's (once it's changed: **from Crash**) or its clip (`GAME/MOVE`, a pack loaded to change; one given
up is left out, `KitPack.drop`), each with **Use Crash's**; with none, a line says it has its template's (the user's word,
2026-11-22: "less is more"; a row a move, most of them its template's, said nothing). The tool's own moves (**my MOVE**) aren't
offered: they never line up with a game's; a pack that has one from before still lists it, to give it up, and `HEROTOOL_OWN` still
sets one for a test. A change of prop or move makes the
preview again by itself; other changes show **Update preview**. Its tests' settings: `HEROTOOL_PAGE=N` (-2 the model's, -1 all,
a kind), `HEROTOOL_OWN=GAME/MOVE=MOVE`.

**The preview**: the first time a game's page is open for a model it's made by itself. Making it
exports the hero to the tool's own folder (SDL's pref
path, `CrashBashPC/herotool/previews/ID`, a mods folder of its own for each hero, so only that one is made) and starts the game with `--dump-hero ID FOLDER`: it reads the player's own settings, so it
finds their game data, makes the hero as it does when it loads a pack, writes what it made and ends, in about a second, with no
window. Each kind's model goes to `FOLDER/ID_KIND.hpv` as `made_write` writes it to the archive (`bonusheroes.c`, `made_dump`:
its images in RGBA, its faces, every frame of every move), so what the tool shows is what the game draws: the hero with each game's
ride or prop, any move it has there, turned and slowed at will (`src/preview.c` reads them, `draw_made` draws them textured as the
PS1 does). What the game draws beside the hero's model, the Ballistix car round it and the tank under it, is written too
(`ID_KIND_rideN.hpv`, `bonus_heroes_dump_rides`: a mesh of the game's model placed as that game places it) and drawn with it, still:
the tank as every tank game but Swamp Fox has it, its hull and the turret of the hero it plays like (Crash's and Coco's swirly gun,
Dingodile's and Rilla Roo's flamer, the others' laser). A pack plays like its template in each game (its weight and speeds, the
tank's turret and weapon) unless its kind says `"like"`; one that says no template, as the hero whose place it takes
(packheroes.c). The previews are dragged with the right mouse button to turn and tilt (as if held: its front goes the way the mouse
does), scrolled to zoom, set back by a right double-click or Home, and show BREATHE first: the same in every view, watched or edited,
the left button left to pick and drag joints (the user's word, 2026-11-22: the buttons swapped between watching and animating).
Under each game in the gallery a choice of **whose ride or prop** it has there: its template's, or another hero's (the pogo stick
or Cortex's jetpack, Tiny's keg ...), written as that kind's `"template"` in hero.json (`--template-in GAME=HERO` from the command
line); picking one makes it again. On foot it's **Animations**, which also has **Its own** (its model's clips, and the
tool's moves on it for the rest): a pack loaded that says no template (Bobby) is so, and is saved as it was, with no template, if
it's based on Crash, else with `"own"` on foot (the user's word, 2026-11-22: the tool saved a template for Bobby, so on foot he
followed Crash's moves). Cortex's and Brio's hover board is the game's jets, drawn in the game, not shown here.
`HEROTOOL_MAKE=KIND` (heropack.c's names: foot, polar, pogo, ...) makes it as the window opens and shows that kind, for tests.

**The command line**: `FOLDER` is a mod: put it in the game's mods folder. What it does, each step for any model (`src/`):

1. **Reads it** (`model.c`), and **refuses** one over the game's limits, 1365 triangles and 4096 vertices, saying how many it has:
   the tool never changes anyone's model (the user's rule, 2026-11-21), so its maker takes out what they choose.
2. **Lays out its texture** (`atlas.c`): every piece of every image (triangles joined by their corners) in one 256 x 256 image, as
   big as they all fit, with a border of its own colours two texels wide, as the game moves each corner to a whole texel; triangles
   with no texture share a white piece, their corners' colours on it. The look's the same; only where the corners are in the
   texture changes.
3. **Fits the standard skeleton**: from **its own rig**, if it has one (`kit_map_rig`), its bones found by their names (Mixamo's
   `mixamorig:LeftArm`, VRM's `leftUpperArm`, Blender's `upper_arm.L`, Unity's and the like; any case, `_`, `.` and spaces
   ignored), its own weights kept, a bone it has that the game hasn't (fingers, twists) giving its weight to the one it hangs from;
   else **from markers** (`rig.c`, `--markers`): a JSON file of where each bone starts, by name (hips, spine, chest, neck,
   head, both arms' upperarm, forearm and hand, both legs' thigh, shin and foot; a jaw, ears and a tail if it has them), with
   `NAME_tip` for a chain's end and `arm_raise`, how many degrees its arms can lift before its paws are in its head. Bobby's is
   `port/mods/bobby-model.glb.markers.json`. The tool's window will let them be dragged onto the model.
4. **Weighs each corner** (`rig.c`): each place on its surface starts with the bone it's nearest (a trunk bone's distance counted
   for less, as a trunk is wide round its bones), and every place goes with the two it's nearest over the surface from those, so an
   ear goes with the ear even hanging beside a chest; smoothed a little, and every copy of a corner at one place (the texture's
   seams) the same, so it doesn't crack as it moves. Only its own side's limbs: a corner of its left half never goes with its right
   arm.
5. **Its own clips** (`kit_clip_pose`), for a rigged model: a clip named as a move (or `foot/` and the name, any case) is that move,
   each standard bone turning in the world as its bone in the model's rig turns from its rest, and the hips moving as its hips do;
   its other clips go in too, by their own names, so a clip named as one of the game's own animations (`MEDIUM`, `KICK`, a kind's
   `ball/BANK`) is the hero's for it. Checked: the first, shapes-built Bobby's ten clips come out as they went in.
6. **Makes the standard moves** (`moves.c`), the ones it has no clip of: BREATHE, RUN, JUMP, ATTACK, WIN, HIT, TAUNT, PICKUP, HOLD_SLOW and HOLD_THROW, a pose
   each 30th of a second. The trunk's bones turn from rest; the limbs are aimed (where each points, from hanging), so a model made
   in a T-pose, hanging or begging moves the same.
7. **Writes the pack** (`write.c`): `heroes/ID/hero.glb` and `hero.json`, and `mod.json`; with `--card CARD.png` its card and
   `--voice JOB=FILE.wav` (as many as it has) its sounds, each checked to be a PNG or WAV the game reads.

Checked: Bobby's model with its markers, and his first, shapes-built model with its bones renamed as Mixamo names them and no
markers, each four of him in Crate Crush.

A model with no markers and no rig starts from the guess (`kit_guess_markers`, above), to drag where they belong.

## Adjusting its moves

The user's word (2026-11-22): "a full animation tool suite so that we can adjust the animation for the hero to fix issues like
clipping". What the game makes of a pack in a game (following its template there, or its own clip) can be changed by hand, a joint
at a time, and the change is kept in its hero.json (`adjust`) and made by the game as it makes the model.

**A change** (`PackAdjust`, heropack.h) is to one bone in one game's move (`"move"`, the model's move as the game made it, the
tool's list; `"*"` every move of that game): at one frame (`"at"`), or held from it to `"until"` (the user's word, 2026-11-22: "if i
make his head realy big i can extend it along the entire animation and that will keep it big"), all of it there and fading to none
over `"spread"` frames each side, or without `"at"` at every frame. How it fades is its `"ease"` (the user's word, 2026-11-22: "the
transion effect should be chooseable"; `PACK_EASE_`, `pack_ease_name`): `smooth` (a cosine, as keys always did, and what one with
none has), `linear`, `slow-start`, `slow-finish`, `sudden` (all of it on its frames, none either side, its spread not counted),
`overshoot` (a little past all of it and back, an ease-out "back"), `bounce` and `wobble` (an ease-out elastic), each by how near
its frames a frame is (`pack_adjust_share`). A key with `"blend": true` flows into its bone's next key in the move instead of
fading out (the user's word, 2026-11-22: blending between keys): over the frames between them less of it as more of that, by its ease
(sudden holds it to that key's frame, overshoot and the rest go past and back), none of it at that key's frame, the two adding up to
all of the change between them; that key then fades in from it, not over its spread (`pack_adjust_share_among`, what the game's
`pack_adjust_pose` and the tool's timeline use). Checked: a head turned 40 degrees at frame 10 blending into -40 at frame 50 is as it
was at frame 30, and the tool's frames and the game's the same exactly. Several at a frame add up. Its `turn` turns the bone and all on it about its joint
(degrees about the hero's own axes as it stands at rest, in the game's: x to its left, y down, z to its back; an axis times its
angle); its `reach` (the game's units, the same axes) moves a hand or foot (a bone named hand... or foot...), its arm or leg bent to
reach it (two-bone, its elbow or knee bent the way it was), the hips (or a root), all of it, or any other bone, it and all on it (an
ear, the tail, the head: the user's word, 2026-11-22, "why cant i move the ears?"; as Blender's G moves a bone that isn't connected,
what joins it to the bone it hangs from stretching); its `scale` (1 as it is, 0.1 to 10; kept as `grow`, the scale less 1) makes the
bone and all on it bigger or smaller along the same axes, about its joint, before its turn (the turn made on the hero's axes:
trunk, turn, size, trunk'). Sizes at a frame multiply, each faded as a turn is. They're made in that order: the hips' reach, each
bone's move, turn and size (those nearest the hips first, a bone moved before it's turned), the hands' and feet's reach. A hand that carries something (the mallet, the keg)
takes it with it, made bigger with it.

**One code for both** (`port/platform/heroadjust.c`, built into the game and the tool): `pack_adjust_pose` makes a pose's changes
(each bone's joint and turn from rest, in the game's axes), and `pack_skin_posed` skins the mesh to it, with the tank's and race
cart's cut. Every way the game poses a pack goes through `pose_finish` (packfollow.c): following its template (`frame_at`), its own
clips in a game (`own_frame`) and on foot (`pack_follow_clip_frame`, packheroes.c's `foot_frame`; on foot it's bound by
`pack_follow_foot_bind` where it follows no template there). The podium is made from its moves on foot, so it has their changes.
Crash's polar bear is the one model it can't change: its rider is sat on the bear by `ride_make`, not posed (another hero's bear,
its prop, can be).

**What the tool makes it from**: with `--dump-hero` the game writes each kind's poses beside its model, `ID_KIND.pose` (HPP1,
`pose_dump_begin`): the skeleton, the mesh as it rests at its size there and each frame's pose before its changes, with the frame's
trunk turn and how it moves with what it rides. The tool's preview is exported without the changes (`Tool.bare`) and makes them
itself on those poses, as the game would, so a change shows as it's made, with no waiting for the game. Checked: the tool's frame
and the game's (from hero.json with the same change) the same to a unit, on the pogo, the tank, the mallet (its carrying hand) and
on foot (an own clip); with a scale (Bobby's head x1.6, x1.3, x1.6 and turned 20 degrees at BREATHE's frame 10, 107 units moved),
an ear moved (his left ear's root 60 out and 40 up) and a hand's reach, the same exactly.

**Its own moves, kept whoever it's based on** (the user's word, 2026-11-22: "if you make a new animation for something ... it will
over ride the template one ... if you change the template that wont overide your custom animation"). A change is made on a move, so a
change made on its template's would land on another hero's move if the template changed. So the first change to a move of its
template's (`bake_move`, gui.cpp) copies that move as the game makes it now (the .pose says which moves are its template's) into a
move of its own: each frame's turns of the standard bones (the game's turns in glTF's axes, each made a turn from the bone it hangs
from), and on foot the hips' move too, written as a clip `KIND/MOVE` (`foot/MOVE`; `KitBaked`, write.c), which the game plays in
place of following (`own_frame`; on foot before its template's). The change is then made on that copy. A template changed after,
for the kind or the whole hero, doesn't change it: its hips go on the new template's, as a clip's do, and it plays over that hero's
length of the move, but its body moves as it was made. For the copy to be the move exactly, a clip's hips come down onto the pegs as a
followed move's do, and what it carries is turned out of its head (`carry_clear`) as when it follows. Checked: a copy is the move it
was made from to the unit (the pogo's BREATHE, from Crash's; on foot, from Cortex's), and on Cortex's pogo the same BREATHE keeps its
body's shape (1% to 2%, any turn or size aside, against 50% for a move still Cortex's).

In the tool a game's **Own animations** lists its own there: **from Crash** (such a copy) or **its clip** (a clip it has, such a copy once
it's saved). Giving one of its own back asks first (**Replace its own animation?**: **Keep mine** or **Use
Crash's**), and takes its changes with it. On foot, where it follows a hero, **Own animations** lists them, each with **Use Crash's**
(asked the same). The page's **Animation** says whose the move is with a tag beside **Edit pose** (**Own animation (from Crash)**, green,
or **Crash's animation**; its tooltip says the rest), and changing **Based on** says how many of its own stay. `HEROTOOL_SAVE_TO=FOLDER` saves after `HEROTOOL_ADJUST`'s change (and after `HEROTOOL_DRAG`'s drag, once it's let go)
(the game having made it again), and `HEROTOOL_ASK=MOVE` picks a move's template's, for tests.

**In the tool**, each game's page has **Animation**: **Edit pose** puts a dot on every joint over the picture (squares for the
hips, hands and feet) and stops the move; its (?) is a card of the controls and keys. Click a joint to pick it: on it is a **gizmo**, as Blender's (the
user's word, 2026-11-22: "a proper gizmo tool like blender has so we can rotate, move and scale"), in the hero's own axes as Blender's
are for a character facing -Y: X red to its left, Y green to its back, Z blue up (`Way` in `adjust_view`, gui.cpp: the game's y is
down), the panel's values lettered the same. Over the picture, **Rotate (R)**, **Move (G)** and **Scale (S)** pick which, as do those
keys:
- **Rotate**: a ring about each axis, dragged round to turn it about that axis (a ring seen almost edge on, as far as the mouse goes
  along it, its near side following the mouse); a white ring about the line towards whoever's looking; its middle, dragged, points
  what's on it at the mouse (its end, the joints on it, turned about that line).
- **Move**: an arrow along each axis, dragged along it; its middle, dragged, moves it across the view with the mouse. Any joint moves,
  with all on it; a hand or foot bends its arm or leg to follow, and the hips move all of it.
- **Scale**: a handle along each axis, dragged out to make it bigger that way (in to make it smaller); its middle, dragged right or up,
  bigger all over.

The picture has a **floor grid**, as Blender's (the user's word, 2026-11-22: "a direction grid like blender"; `floor_grid`): on the
game's ground (y 0, or the move's lowest if that's far from it), squares of 50 of the game's units fading out from the middle, its X
line red and its Y line green, drawn where the model doesn't hide it. In its top right corner are **its axes**, as Blender's
(`view_axes`): each axis lettered at its end, plain at the other; clicking one looks from there (the keypad's 1 from its front, 3
from its left side, 7 from above, with Ctrl from the other side; `view_from`), its front at the bottom from above. The picture is
fitted to how tall the model looks from the view's tilt, its middle in the picture's. The right mouse button turns the view (to
straight above or below), so an arrow or ring can be had from any side (the pictures' Turn sliders went: right-drag does it). A change
is made to the key picked if it's the joint's and covers the frame shown, else to a key at the frame shown, fading either side; a bar
picked (every frame: a blue one every move in the game, a green one, from before, its whole move) takes the change, and the joint's
heading says so ("Head, all animations"). Right-click a key for **Apply to all animations** (the keys picked made every move's in the game, a blue bar)
or, on a blue bar, **Apply to this animation only** (held all through the move shown); the user's word, 2026-11-22: All moves as a box was a mode
easily left on, so it's what a key's made into now (`keys_all_moves`; a change to every move makes them all its own). Under the joint's name, its values set
it exactly: **Rotate**, **Move** and **Scale** rows of X, Y and Z, each field tinted its axis's colour as Blender's (`xyz_row`; X tips it
forward, Y leans it to its left, Z turns it to its right; to its left, to its back, up; wider, deeper, taller, 1 as it is: each
field's tooltip says it); **Reset bone**, **Hide** and **Reset animation**, then the history. (Cut, as doing what's done elsewhere, the user's word,
2026-11-22, "less is more": the panel's Frames row, its Mirror button, its Undo and Redo buttons and its list of keys; the timeline,
X-Mirror, Paste mirrored, Ctrl+Z and Ctrl+Y and the history do them.)

**Fitting it into the tank or the race cart** (the user's word, 2026-11-22: the game's own squeeze, a pinch where it sits, "needs to be
done in tool", so people can fit their own heroes however they want). Those two games' pages have **Fit in seat** (`no_fit`, a
kind's `"fit": false`), on by default: the game's tail folded away and legs drawn in to the hatch's width, as before. Off, it's shown and
played as it's posed, and fitted by hand: **Hide** (beside Reset bone; `bone_hide`) makes the bone picked, and all on it, nothing in
every animation of that game, a grey bar in the timeline ("hidden in all animations"); with X-Mirror both sides. It's an ordinary key
underneath, in all animations with Scale 0 (Scale goes down to 0 for it), so undo takes it back; with a hidden bone picked it's
**Show**, which puts it back to Scale 1 (a key that did no more goes). Hiding doesn't make the animations its own as other changes in
all of them do (`own_before_change`): it changes no pose, so they stay its template's. A hidden bone's values are greyed, and the
clipping finder leaves what's hidden out. Which games can fit comes from the game's .pose (its cut flag, the capability), and the
tool's preview follows the switch live.

**The tool's words** (the user's word, 2026-11-22: "clean up terminology across the entire tool. moves does not make much sense"):
an **animation** is what the game plays (BREATHE, RUN ...; the game's code, KitGames and hero.json's "move" still say move), a **bone**
is what's posed (its dot at its joint; **joints** only where the model's page places its dots to rig it), a **key** a change at frames,
**Rotate**, **Move** and **Scale** what the gizmo and the values do (as Blender's: moving is only a bone's sliding), and **Fade** and
**Blend** how a key comes and goes. The history's steps say the same: Rotated, Moved, Scaled, Stretched, Deleted, Pasted, Faded.

The tool says as little as it can (the user's word, 2026-11-22: "its very wordy for a visual tool"): a word or two for a control,
what it does in its tooltip, one (?) (the animation's controls card), short messages on the top bar, and no paragraphs; the right's
fields leave room for their labels at the window's scale.

Under the picture is its **timeline** (the user's word: "a key frame time line below the animation"): play or stop, the key before
and after, the frame, the speed, and a sheet of the move's frames: a ruler (click or drag it to go to a frame) with the frame shown
across it all, and a row for each joint with a change in the move (and the joint picked; clicking a row's name picks it). A change
at a frame is a diamond; one **held** over frames a bar between two diamonds; its fade a band either side; one to the whole move a
green bar, to every move here a blue one. Click a key to pick it and go to it (its joint, its frame and fade). **Several** can be
picked (the user's word, 2026-11-22: "add multi-select for keys in the timeline"; `keys_sel`, `key_sel` the one picked last, the
panel's): Ctrl+click picks one more (or not any more), dragging a box over an empty place in the rows picks those it's over (Ctrl: as
well), Ctrl+A (or right-click an empty place, **Select all**) every key of the move, and clicking an empty place none. Dragging one of
several moves them all as far (none past the move's ends; clicked and let go unmoved, it alone is picked), Delete takes them all away,
the panel's **Fade** fades them all, and right-clicking one of them is for them all (**Fade**, the **Stretch** ones, **Delete**).
The timeline **zooms** (the user's word, 2026-11-22: zoom and snapping): the wheel over it zooms about the mouse (to a quarter of the
move's frames shown), Shift and the wheel (or a sideways wheel) scroll it, Home shows all of it again, a bar under the ruler says
which part is shown, and playing it follows the frame shown. A dragged key (or its end) **snaps** to another key's first or last
frame, or the frame shown as the drag began, the nearest within a few pixels (a line shows where); Ctrl and drag doesn't snap.

**X-Mirror** (beside Turn, Move and Size; the user's word, 2026-11-22: mirror while editing, as Blender's): while it's on, each
change to a joint's key (by its gizmo, its values, its fade or blend, its frames, or dragging it in the timeline) is made to its
other side's key at that frame too, its mirror image (across, and its turns about up and front to back, the other way; its frames,
fade and blend the same), the key made if there's none (`mirror_edit`); the other side's joint is ringed in purple. Checked: a left
ear dragged by its gizmo, its right ear the mirror of it, and the game's frame the same exactly.

**Onion** (beside X-Mirror; the user's word, 2026-11-22: onion skin): ghosts of the frames some apart (its slider, 1 to 15) before
(blue) and after (orange) the one shown, round the move, as the game makes them with the changes, drawn only where they stick out past
the frame shown (over it, they hid the pose): each ghost's own nearest surface found apart (`raster_ghost`) and blended where the
picture has nothing.

**Clipping** (beside Onion; the user's word, 2026-11-22: a clipping finder, what the animation tools are for): the points of it inside
another part, red on the picture, with their count at the frame shown, and a strip under the ruler, redder where more clip (click it
to go there; its tooltip says how many). Each part's core is a capsule from the rest (`clip_prepare`): a bone to the bone on it that
most hangs from it (the chest's to its neck: one to a shoulder reached up under the ears and found them on every frame), an end bone's
on past it, its radius three quarters of the way to its part's nearest third of points, well inside it, so parts that only touch don't
count. A point clips if it's inside a core of a part more than two bones from its own (near ones overlap at their joints), and a held
prop's points inside one more than two from its hand (`clip_points`). Each frame of the move is counted again when its changes (or
its model) change, not while something's dragged (`clip_scan`). On Bobby: BREATHE one point on 2 frames of 121, PICKUP none; TAUNT's
head-dips put his jaw into his chest, and the mallet's ATTACK its handle through the back of his head, both found. Drag a diamond (a held
key by its bar) to move it to other frames. To hold one over more frames or fewer: drag a held key's end, or a picked key's grips
(the white bars either side), or Shift and drag a key the way it's to go. Right-click a key for **Copy**, **Paste**, **Paste
mirrored**, **Fade**, **Stretch to end**, **Stretch to start**, **Stretch to whole animation**, **Single frame** and **Delete** (or press
Delete). A change made at any
frame it's held over is made to it (`adj_key`), so its head made bigger anywhere there is bigger all the way. The fade picked
(**Fade**: its frames, its curve: Smooth, Steady, Slow start, Slow finish, Sudden, Overshoot, Bounce, Wobble, and **Blend**: into
the joint's next key, a blue line to it in the timeline; right-click a key for **Blend into next** too) is the picked
joint's key's there (or a new key's), and right-clicking a key has **Fade** too. Each key's row draws its fade as it is, how much of
it there is each frame (a bounce's bumps, an overshoot's rise past it).

**As it plays** the panel keeps to the key picked (`panel_key`): stopped, it's the picked joint's key at the frame shown (and that's
the one picked); playing, it's the one picked whatever frame's shown, so its fade (or its values) can be changed as it plays and seen
at once (the user's word, 2026-11-22: changing the fade as it played went to another key or none, as the frame shown moved on).
Clicking a key as it plays picks it and leaves it playing (moving a key wants it stopped). With none of that joint's picked, the
panel says to click one; a value changed then stops it and makes a key at the frame shown. `HEROTOOL_PLAY=1` plays it, for a test.

**Copy and paste** (the user's word, 2026-11-22: "add keyframe copy and paste in the timeline"): Ctrl+C (or right-click, **Copy**)
copies the keys picked (else the picked joint's at the frame shown); Ctrl+V pastes them at the frame shown (the first there, the rest
as far after it as they were), each with its change, its fade and the frames it's held over: copied from one joint, onto the joint
picked; from several, each onto its own (mirrored, each onto its other side: a left ear's keys made the right ear's in one go). Ctrl+Shift+V pastes them mirrored, each its other side's way (as X-Mirror makes it: across, and its
turns about up and front to back, the other way). Right-click an empty place in a joint's row for **Paste here** (or **Paste
mirrored**) at that frame, or a key for **Paste** over it. A key there already (at that frame, or to the whole move) is replaced; a
key to the whole move or every move here pastes as that (`key_copy`, `key_paste`).

**The undo history** (the user's word, 2026-11-22: "add undo history for keyframe moves in the timeline"): every change is a step
with a name, as Blender's: "Turned Head", "Made Head bigger or smaller", "Moved Head's key to frames 32 to 82", "Held Head's key from
frame 10 to 66", "Took away Head's key", "Mirrored ...", "Reset ..." (a drag's step said again as it goes, `adj_step_says`). The
panel's **History** lists them, oldest first, the one it's at lit and those undone greyed; clicking one goes back (or on) to just
after it (`adj_go`). Up to 300 steps. Keys: Space plays, the arrows step a frame, R, G and S pick the gizmo, Delete takes the key
picked away, Ctrl+C and Ctrl+V copy and paste a key (Ctrl+Shift+V mirrored), Ctrl+Z, Ctrl+Y (or Ctrl+Shift+Z). Up to 256 changes a
hero.

Its tests' settings: `HEROTOOL_ADJUST=BONE@FRAME` (or `@FROM-TO`, held) adjusts the page shown once it's made, that bone picked there
(and `HEROTOOL_SHOW_FRAME=N` shows that frame after; `HEROTOOL_MOVE=NAME` watches that move; `HEROTOOL_ONION=STEP` onion skin; `HEROTOOL_CLIPPING=1` the clipping finder, each move's clipping printed; `HEROTOOL_XMIRROR=1` turns X-Mirror on, the test's change mirrored; `HEROTOOL_ALLMOVES=1` puts the test's key in all moves (2: and back); `HEROTOOL_ADJUST_FADE=EASE,N` its fade, hero.json's name, over N frames;
`HEROTOOL_PASTE=FRAME[,1[,BONE]]` copies it and pastes it at FRAME, mirrored with 1, onto BONE; then `HEROTOOL_SELECT_ALL=1`
picks every key of the move and `HEROTOOL_PASTE2=FRAME[,1]` copies and pastes those);
`HEROTOOL_ADJUST_TURN` / `_REACH` / `_SCALE=X,Y,Z` changes it; `HEROTOOL_GIZMO=N` picks the gizmo (0 turn, 1 move, 2 size);
`HEROTOOL_DRAG=X0,Y0,X1,Y1[,1[,1]]` drags the mouse in the window (the right button with the second 1; Shift with the first, down the frame before the press and up the frame after, as
ImGui holds a press queued after a key's change for the next frame; the window's pixels, as `HEROTOOL_SHOT`'s picture's), and keeps
it there after, the real mouse left out (ImGui's SDL backend gives the real one's place while the window has the focus);
`HEROTOOL_WHEEL=X,Y,N` turns the wheel N notches at (X, Y) once it's adjusting, the mouse kept there; `HEROTOOL_LIVE=FILE` writes the frame shown as the tool made it (s16 x, y, z a vertex), and FILE.txt its move and frame, to set
beside the game's. Run one test tool at a time, or each with its own `HEROTOOL_PREF`: each has the game make its models into its
folder.
