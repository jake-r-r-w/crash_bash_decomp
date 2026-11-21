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
hero tool (below) makes a pack of any standing two-legged model, rigged or not. Still to come: the other seven heroes' moves to borrow
(only Crash's yet).

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

## Online

A pack's hero plays online where every machine has the same pack (`platform/net.c`; PORT.md, Online play). Packs are files, so
they're compared, not sent: `mods_hero_hash` hashes a hero's id and every file of its folder, by name and bytes, added up so the
order a folder or zip lists them in doesn't matter. A joiner's ask (T_JOIN) carries the hashes of its heroes, if its CUSTOM HEROES is
on; the host's start (T_START) carries the first four of its own that every machine has, in its order, and CUSTOM HEROES as the
host has it. Every machine then makes only those (`mods_heroes_only`; `mods_apply_online`), in the same slots, so the same cards
are in the select and the computer picks the same. A pack a machine lacks just isn't played, and the host's log says why
(`bobby isn't played: machine 2 hasn't its pack, or has another`). A game's record (`--net-record`) names its packs by hash
(`hero ...`), and `--replay` needs them in a mod that's on.

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
