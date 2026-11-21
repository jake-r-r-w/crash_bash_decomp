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
- `voices`: each of its base's 26 sound offsets (EXTRA_HEROES.md, His sounds) `base` (the default) or `none`. `own` is for its own
  `voice_NN.wav`, still to come.

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
