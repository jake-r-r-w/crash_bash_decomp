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
his hand, on the grip where there's one, its legs astride as his, on the pegs or the floor where he stands, else sat, and hidden in
a tank's hatch or a race cart; the prop (painted gunmetal where Pinstripe's was) as it moves, or in its hand where he carries it.
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
  `rilla`).
- `height`: standing, in the game's units (Crash 452, most heroes about 650). It's the one number of its model the game's play sees
  (the base's collision points are scaled to it), so it's a whole number in the JSON, never worked out from the mesh.
- `kinds`: by name, `foot`, `polar`, `pogo`, `ball`, `tank`, `race`, `splash`, `mallet`, `dino`, `kegs`, `warp`. Each has its
  stand-ins for its base's animations (`[from, to, pace]`, pace `stretch`, `loop` or `once`: `HeroKind` in `herodef.h`, and
  EXTRA_HEROES.md, How one plays), `otherwise`, `keep_lift` (0 to 4096), `like` (a hero's name) and `subframes` (1 to 4).
- `face_head`: the share of its model, from the top, that's its head, for a face drawn from it on the panel and in the select
  (0.36 if it's left out); Bobby's is 0.26, as his long ears hang past his chin. A face drawn so stops where the panel's health bar
  starts.
- `voices`: each of its base's 26 sound offsets (EXTRA_HEROES.md, His sounds): a job (`"1": "laugh"`), `base` or `none`. An offset
  it doesn't name plays the job it does for the game's own heroes if the pack has that job's sound (0 name, 1 and 6 and 24 laugh, 2 to
  5 yelp, 11, 13 and 21 jump, 12 thud, 15 throw), else its base's.

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
hero.json for every kind (Crash if it's not said), or `"template"` in a kind's own entry for that kind alone. **Its own moves** take the
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
order a folder or zip lists them in doesn't matter. A joiner's ask (T_JOIN) carries the hashes of its heroes, if its CUSTOM HEROES is
on; the host's start (T_START) carries the first four of its own that every machine has, in its order, and CUSTOM HEROES as the
host has it. Every machine then makes only those (`mods_heroes_only`; `mods_apply_online`), in the same slots, so the same cards
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
build/herotool/windows-clang/herotool import port/mods/bobby-model.glb --id bobby --name BOBBY --mod-name Bobby     --author "Crash Bash PC" --markers port/mods/bobby-model.glb.markers.json --out port/mods/bobby --height 540 --face-head 0.26 --prefer crash
```

He's the user's dog, a long-haired black-and-tan miniature dachshund, stood up as a cartoon hero. His model is the user's own, made
from photos of him (`port/mods/bobby-model.glb`, beside the markers, as a folder there would be a mod of its own): 1365 triangles,
the two smallest of its first export's 1367 taken out at the user's word, one texture, no rig, so the markers
(`bobby-model.glb.markers.json`) place his skeleton, ears, jaw and tail too, and `arm_raise` keeps his short arms from going into his big
head. The photos aren't in the repo.

## The hero tool

`herotool/` at the repo's top, its own CMake project (C, SDL3, cgltf), sharing the game's pack reader (`port/platform/heropack.c`),
so a pack it writes is one the game reads. It makes anyone's model a hero:

```bash
cd herotool && cmake --preset windows-clang -DCMAKE_PREFIX_PATH=<SDL3's install> && cmake --build --preset windows-clang
build/herotool/windows-clang/herotool import MODEL.glb --id ID --markers MARKERS.json --out FOLDER [--name NAME] [--prefer HERO]
                                       [--height N] [--face-head SHARE] [--author NAME]
```

**The window**: `herotool` with no arguments, or a model alone (`herotool MODEL.glb`), opens it (`src/gui.cpp`, Dear ImGui on
SDL3's renderer, the model drawn by a little software renderer of its own). Drop a `.glb` on it (or a `.png` for its card, a `.wav`
for the sound job picked, a markers `.json`). Its front and side views show the model at rest with its markers, to drag onto its
joints (its left's mirror its right's, a bone's end is the next one's start, and its weights are worked out again on letting go);
the third plays the move picked, turning. A model with no markers beside it (`MODEL.glb.markers.json`, saved there on each
export) starts from a guess (`kit_guess_markers`: a standing humanoid's proportions in its box, arms out if it's as wide as a
T-pose; Jaw, Ears and Tail add theirs). A rigged model's bones are its own, no markers. Then its name, height, whose place, card,
sounds and the mod's name, and **Export** writes the mod to the mods folder (beside the tool by default, the game's if it's
there), or **Export and test in game** starts the game too, with it as player 1 in the game picked. `HEROTOOL_SHOT=FILE.png`
saves a picture of it and closes it, and `HEROTOOL_EXPORT=FOLDER` exports as it opens, for tests.

**The preview** (the window's **In the games** tab): **Make it in every game** exports the hero to the tool's own folder (SDL's pref
path, `CrashBashPC/herotool/preview`) and starts the game with `--dump-hero ID FOLDER`: it reads the player's own settings, so it
finds their game data, makes the hero as it does when it loads a pack, writes what it made and ends, in about a second, with no
window. Each kind's model goes to `FOLDER/ID_KIND.hpv` as `made_write` writes it to the archive (`bonusheroes.c`, `made_dump`:
its images in RGBA, its faces, every frame of every move), so what the tool shows is what the game draws: the hero with each game's
ride or prop, any move it has there, turned and slowed at will (`src/preview.c` reads them, `draw_made` draws them textured as the
PS1 does). Where the game draws part of a ride itself (the tank's hull) that part isn't in the hero's model, so it isn't shown.
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
