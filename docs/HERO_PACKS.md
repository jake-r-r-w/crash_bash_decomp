# Hero packs

A hero of your own in the PC port: a glTF model in a mod, playing in an original hero's place as Ripper Roo and Pinstripe do
([EXTRA_HEROES.md](EXTRA_HEROES.md)). Unlike them it needs nothing of another game: the pack holds only its creator's model,
clips, sounds and card, and the game makes it into Crash Bash's models as it loads. The plan, in phases, is in the session plan of
2026-11-21 (the hero tool, every kind of model, borrowed moves, online), and this page grows with it.

**Where it stands (phase 1):** a pack's model on foot and on the podium, from its own clips. Its other kinds play on foot. Its
sounds are its base's. Bobby, the example, plays in Crate Crush and Ring Ding.

**The character select's room.** Each of its slots of drawn pieces (`EngineSlotSubs`) held the PS1's 32, and the select's own 26
with two a card and their row's band left room for two cards: a third (the first pack's, beside Roo and Pinstripe) spilt into the
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
`port/mods/` with Roo and Pinstripe. The log says what was made: `hero packs: bobby made, 1318 triangles, ...`.

## Bobby, the example

`port/mods/bobby/` is made by `node port/mods/bobby.mjs`: the user's dog, a long-haired black-and-tan miniature dachshund, stood up
as a cartoon hero. His model is the user's own, made from photos of him (`port/mods/bobby-model.glb`, its texture saved again losslessly to fit the repo's 2 MB; 1367
triangles, one 2048 texture, no skeleton); the photos aren't in the repo. The script does to it what the hero tool will do to any
model (phase 3), by hand:

- its two smallest triangles go, for the game's 1365;
- its texture laid out again at 256 x 256, each of its 223 pieces with a border of two texels of its own colours: the game keeps a
  corner's place in its image as whole texels, so each moves up to half of one, and with the pieces up against each other as the
  model has them, that showed the neighbouring piece's colour (orange flecks over his black coat);
- a skeleton of 25 bones (`BONES`), placed from front and side views of him on a grid, and each corner's bones by the part of him
  it's in (`part`: ears, head and jaw, arms, legs, tail, body), the nearest two of them, smoothed three times over his surface, and
  every copy of a corner at one place (the texture's seams) given the same, or they come apart as he moves (cracks you see through);
- ten clips of his own on foot: a panting BREATHE with his tail wagging, a scampering RUN, a JUMP, his zoomies as he ATTACKs and
  WINs, a HIT, a TAUNT (his back to them, wiggling, then a bark), PICKUP, HOLD_SLOW and HOLD_THROW.

He rests begging, his forearms held up in front, and his arms are short against his big head: raised further than his chin, his paws
went inside his muzzle or ears (`arm`), so a crate he carries balances on his head.

## The hero tool

`herotool/` at the repo's top, its own CMake project (C, SDL3, cgltf), sharing the game's pack reader (`port/platform/heropack.c`),
so a pack it writes is one the game reads. It makes anyone's model a hero:

```bash
cd herotool && cmake --preset windows-clang -DCMAKE_PREFIX_PATH=<SDL3's install> && cmake --build --preset windows-clang
build/herotool/windows-clang/herotool import MODEL.glb --id ID --markers MARKERS.json --out FOLDER [--name NAME] [--prefer HERO]
                                       [--height N] [--face-head SHARE] [--author NAME]
```

`FOLDER` is a mod: put it in the game's mods folder. What it does, each step for any model (`src/`):

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
   `port/mods/bobby-markers.json`. The tool's window will let them be dragged onto the model.
4. **Weighs each corner** (`rig.c`): each place on its surface starts with the bone it's nearest (a trunk bone's distance counted
   for less, as a trunk is wide round its bones), and every place goes with the two it's nearest over the surface from those, so an
   ear goes with the ear even hanging beside a chest; smoothed a little, and every copy of a corner at one place (the texture's
   seams) the same, so it doesn't crack as it moves. Only its own side's limbs: a corner of its left half never goes with its right
   arm.
5. **Makes the standard moves** (`moves.c`): BREATHE, RUN, JUMP, ATTACK, WIN, HIT, TAUNT, PICKUP, HOLD_SLOW and HOLD_THROW, a pose
   each 30th of a second. The trunk's bones turn from rest; the limbs are aimed (where each points, from hanging), so a model made
   in a T-pose, hanging or begging moves the same.
6. **Writes the pack** (`write.c`): `heroes/ID/hero.glb` and `hero.json`, and `mod.json`; with `--card CARD.png` its card and
   `--voice JOB=FILE.wav` (as many as it has) its sounds, each checked to be a PNG or WAV the game reads.

Bobby's model has 1367 triangles, 2 over: the tool refuses it, and his pack is still made by `port/mods/bobby.mjs` until he's
exported again with fewer. A copy with two slivers taken out (in `scratch/`, never the repo) checks the tool end to end.

Checked: Bobby's model with its markers, and his first, shapes-built model with its bones renamed as Mixamo names them and no
markers, each four of him in Crate Crush.

Still to come: the window (an ImGui front end on the same steps: the model turning, markers dragged, the moves played, the
pack tried in the game), its own clips for moves by name, and guessing the markers for a model that has none.
