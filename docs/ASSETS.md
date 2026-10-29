# Assets

The game's files are not part of the source: `tools/scripts/dat.py extract` takes them from your
own disc into `assets/` (git-ignored, never committed), as editable files where the format is
known, and `make disc` builds the disc from there. This page documents the formats. How to use
`assets/` is in [PLAYING.md](PLAYING.md).

[CTR-tools](https://github.com/DCxDemo/CTR-tools) (the Crash Team Racing toolkit, same studio)
reads some Crash Bash formats (`formats/bash_tex.ksy`, `CTRFramework/Code/bash/`); it extracts
but doesn't rebuild. Check its readings against ours and credit it where a format comes from it.

## `CRASHBSH.DAT`

No header: the exe's table at `0x8004E110` gives 992 `{u32 sector, u32 size}` entries
(ARCHITECTURE.md, Overlays). `dat.py` writes entry `N` to `assets/CRASHBSH/<NNN>.bin` (hex id)
or, when it decodes it, to a folder; `index.json` has each entry's sector, size and format, and
`filler.bin` the non-zero bytes between entries. An entry may shrink but not grow.

Every entry has a name: `configs/USA/dat_names.txt` (CTR-tools'
`CTRFramework/Data/bash_filelist.txt`, GPL-2.0, credited in its header; decimal ids) lists all
992 for this release, 400 `.tex`, 400 `.mdl`, 160 `.sfx` (sound packs; its comments say the music
ones hold VAB/SEQ), 27 `.bin` (the code overlays and text) and 5 `.tga` (title screens).
`dat.py` copies each name into `index.json` (e.g. `000` is `models/cutscene/uka/data.tex`, `001`
its `data.mdl`, `3DF` `overlays/gameeng.bin`).

What the entries are (first u32 is a type or a count, not a magic number):

| Entries | Count | What | Decoded |
|---|---|---|---|
| mostly even ids, `000`..`3DD` | 400 | texture sets, type 8 | `<id>.tex/` (tex.py) |
| the entry after each texture set | 400 | model files (`.mdl`), textured by the set before them | `<id>.mdl/` (mdl.py: OBJ + glTF) |
| `1E7`, `2CC`..`2D8`, `3DF` | 15 | code overlays (`make disc` writes the built ones) | C/asm |
| `.sfx`, e.g. `099`, `126` | 160 | sound packs: samples (VAB) and, in 28, music (SEQ) | `<id>.sfx/` (sfx.py) |
| `08D`..`098` | 12 | mode text (`overlays/text/*.bin`): strings, then data pointing at them | `<id>.txt/` (txt.py) |
| `088`..`08C` | 5 | title screens (`screens/*.tga`): plain TGA files | `<id>.tga/` (tga.py) |

The rest of the disc holds nothing more of Crash Bash's: `BASHY.` (30 MB) is all zeros, a
filler, and `SPYRO3/` is the bundled Spyro 3 demo.

## Texture sets (type 8)

`tools/scripts/tex.py`; layout as CTR-tools reads it, confirmed on all 400 sets (each packs back
byte for byte) and by eye (the PNGs show the game's textures in their colours), and as the game
reads it: `TexFile` and `TexImage` in `include/bash/main.h` (`textureset_init`,
`texture_init_mem_textureset`).

| Offset | Size | Field |
|---|---|---|
| 0x00 | u32 | type, 8 |
| 0x04 | u32 | size of the file |
| 0x08 | u16, u16 | number of images, number of CLUTs |
| 0x0C | u32 | bytes from 0x0C to the first image |
| 0x10 | s32 | 0x10: from here to the CLUTs |
| 0x14 | u32 | unknown (the game copies it to `obj_textureset.f14`) |
| 0x18 | u32 | offset of the animated textures (0: none; 86 sets) |
| 0x1C | s32 | 0 in every file (self: a count and words the game copies into `obj_textureset.anim`) |
| 0x20 | | CLUTs: u32 n (16 or 256), then n 16-bit colours |
| | | images: u16 w (VRAM 16-bit units), u16 h, u16, u16 (unknown; CTR-tools: margins/size bytes), u32 0 (copied to `obj_texture.fC`), u16 CLUT << 1 \| 8-bit (CLUT 0x7FFF: none), u16 flags (0; 3 for some banners: bit 1 means this and the next images are one per language, `frames` of them, and the game picks its language's), s32 frames (1, 2, 8, 16), then w * h * 2 bytes of pixels |
| | | then the animated textures (below) or zeros: `tail.bin` |

Pixels are 4-bit (low nibble = left pixel, width w * 4) or 8-bit (width w * 2). A negative CLUT
index (the last image of some sets) means none of its own; such an image is shown in grey.

The animated textures, as `texture_update_anims` reads them (records `TexFrameAnim` and
`texture_scroll` in `include/bash/main.h`). Offsets are s32s; "self" counts from its own position:

| Part | What |
|---|---|
| head | s32 tracks (from the block), s32 scrolls (self); 0: none |
| tracks | s32 count, then 20-byte records: s32 frames (from the record to the frame table), s32 texture (image index), s32 count, s32 speed, s32 pos. Each update adds speed to pos (8.8 frames; negative runs backwards, wrapping at count) and loads the frame `pos >> 8` into the texture's VRAM rect |
| frame tables | count self s32s, each to a frame's pixels: the texture's w * h * 2 bytes, in its depth and CLUT |
| scrolls | s32 count, then 16-byte records: s32 texture, s32 speed, s32 pos, s32 dst (0 in the file: the VRAM node the game gives it). Each update adds speed to pos (8.8 rows) and copies the texture into dst rotated down by `pos >> 8` rows: a scrolling texture |

65 sets have frame tracks (1,137 frames in all, most at speed 0x80: a frame every other update), 61
have scrolls; 44 blocks end in 4 bytes of zero padding. Checked by eye: the Cortex lab intro's
(`068`) electric arc and pulsing light.

`<id>.tex/` holds `img_NNN.png` (indexed, its CLUT as the palette: edit the pixels; pack reads
the indices only), `cluts.png` (one row per CLUT: edit colours here; r, g, b are 5-bit values
times 8, alpha 255 = STP off, 128 = STP on, 0 = the colour 0x0000, transparent), `tex.json` (the
fields above, and `anims`: each track's texture, speed, pos and frame files, each scroll's
texture, speed, pos and dst) and `tail.bin`. An animated set also has `anim_TT_FFF.png`, frame FFF
of track TT, in its texture's palette. Pack writes the frames and the anims fields into `tail.bin`,
which keeps its layout: tracks, scrolls and frames can't be added, and a track's texture can only
change to one of the same size. An image keeps its size and depth; at most 16 (or 256) colours.

## Model files (`.mdl`)

`tools/scripts/mdl.py` and `gltf.py`. The mesh reading starts from CTR-tools' (`BashMesh.cs`);
the rest comes from the game's own code, matched in `src/main/main.c`: `prim_write_uvs` (texture
coordinates), `faces_write_colors` (colours), `draw_verts` (drawing), `morph_blend_verts` and the draw
switch around it (animation). All 400 files pack back byte for byte; 373 have meshes, the 22
fonts and 5 arenas (e.g. `tank_jungle/arena.mdl`) have none in this table. Checked by eye: Uka
Uka's mask textured and talking (`001`, `043`), Oxide's hovercraft (`13F`), the menu's arena
pieces (`07F`).

Offsets marked "self" are s32s counted from their own position. The game reads the file through
`ModelFile`, `MeshHeader`, `MeshVerts`, `ModelEntity`, `FileEntry` and `MeshAnimFrame` in
`include/bash/main.h`, and an arena's maps through the Oct 9 prototype's own types (`map_list`,
`map_map`, `trigger_list`, `Placement` in `include/bash/core.h`; `mapscene_setup_tables`). A model
id picks a part: `0x2000 | i` mesh i (from 1), `0x5000 | i` entity i (from 1), `0x4000 | n << 7 |
f` frame f of mesh animation n.

| Offset | Size | Field |
|---|---|---|
| 0x00 | u32 | tag (`0x0C160029`; `0x09160026` in one file) |
| 0x04 .. 0x0C | | not read by the exe (0x08 self, 0x0C a count) |
| 0x10 | self | the map's animations (`map_enviroment.map_animations` is 8 bytes past it) |
| 0x14 | u32 | 0 or 1 (1 in 73 files, arenas and others) |
| 0x18 | self | the maps: a `map_list` (u32 count, then a self offset each to a `map_map`) |
| 0x1C | self | the map directory: s32 count, then 12-byte entities (s32 self offset to a mesh header, s32, s32 group) |
| 0x20 | self | the colour table: u32 RGB + code, 3 per face (one a corner: Gouraud) |
| 0x24 | self | the UV table: u16 `u \| v << 8` |
| 0x28 | self | the default vertex pool for animations (s16 x, y, z) |
| 0x2C | self | not read by the exe |
| 0x3C | self | chunks loaded on demand: s32 count, then 16 bytes each (u32 start, u32 end, then the loaded data's pointer and load handle, 0 in the file) |
| 0x40, 0x44 | u32, self | the mesh animations: count, then 0x18-byte entries (below) |
| 0x48, 0x4C | u32, self | the scripts: count, then a self offset each to a `script_header` (the prototype's: thread count, sub count, end, start, frames, then a self offset per thread) |
| 0x50 | u32 | the size of the part kept in memory: the whole file in 168, in 228 the on-demand data (mesh animations, chunks) starts at the next 2 KB sector after it |
| 0x54 | u32 | number of meshes (0 in the fonts) |
| 0x58 | 0x34 each | mesh headers |

A map (`map_map`): s32 cell size x, y, z, then self offsets at +0x0C to the paths, +0x10 the
triggers (`trigger_list`), +0x14 the contour grids, +0x18 the cell list, s32 at +0x1C the number
of placements and a self offset at +0x20 to them (always the map's +0x34), an RGB clear colour and
a clear flag at +0x24. `tools/scripts/mapdata.py` reads and writes them (73 files have a map, one
each: the arenas, hubs and menus; 217 paths, 473 triggers, 2,689 placements):

| Part | What |
|---|---|
| paths | s32 count, then a self offset each to a path (`path_get` finds one by id): s32 keys, s32 frames (the last key's), s32, s32 id, 8 s32s (0 in most), then 0x1C-byte keys: s32 x, y, z, s32 frame (0 first), s32, u32 flags (0x2000 on the last key, and only it), s32. The game moves cameras and hub objects along them (`key_interp_pos`) |
| triggers | u32 count, then 16 bytes each: s32 x, y, z, then a word whose bits 0-1 are the cast and the rest a self offset to its info: u16 type (into the level's trigger types), u16 flags (0x4000: a point, `MapPoints[slot]`, not a NAS), s32 slot, s32 rotation x, y, z (`InitTriggers`) |
| contours | s32 count, then a self offset each to a contour: s32 min x, y, z, max x, y, z (its bounds), 7 s32s, s32 at +0x34, s32 edges, then 0x38-byte edges: 3 s32s (the first ±0x3FFFFFFF), s32 p0 x, y, z, p1 x, y, z (y 0 in all 1,200), s32 length (p0 to p1 in x, z), s32 angle, 3 s32s. The arenas' walls and zones: `PointInZone` tests which side of each edge a point is, Dino collides with them (`DinoTrackCollide`), Race and Polar look edges up by angle. 107 contours in 30 maps; the hubs and menus have none |
| cell list | +0x18: empty (a 0 count) in every map |
| placements | 0xA0 bytes each, one subengine each (`mapenv_place_subs`): u32 flags, s32 x, y, z, +0x28 a `MATRIX` (rotation: every row has length 4096; its t is the position), +0x74 4 bytes, +0x88 u16 the model id it draws (`0x5000 \| i` an entity, or `0x4000 \| ...` a mesh animation frame), +0x9C u16 (the subengine's +0x68), +0x9E u16; the rest not read |

A mesh header: u32 0 (the game's pointer to its prims while drawn), u32 0, s16 faces, s16 (4..7), u32 (flags), then self offsets at +0x10 to
the vertices, +0x14 the strips, +0x18 the UV indices, +0x1C the texture runs, +0x20 the colour
indices, +0x24 the end, +0x28 the normals (0: none), +0x2C its attachment points (a
`PointList`: s16 count, s16, then s16 x, y, z and u16 flags each; 0: none), and a u32.

| Block | What |
|---|---|
| vertices | a 0x14-byte bounding box (s16 min[3], max[3], centre[3] and radius), then s16 x, y, z and a u16 each |
| normals | lit meshes only: s16 x, y, z (4.12, length 4096) and a u16 each |
| strips | u16 `faces << 8 \| flags` until 0xFFFF (in memory: a flags byte and a count byte, ended by count 0xFF); a strip of n faces takes the next n + 2 vertices; flag bit 0: untextured (Gouraud only); 0x08 flips the first face's winding |
| UV indices | u16 a face: its corners are UV table entries i, i + 1, i + 2, in strip order (faces along a strip share two) |
| texture runs | u16 entries, each for 1 + bits 9-14 faces: bits 0-8 the texture (image N of the texture set before the model in the DAT); bit 15: the set's last image with CLUT bits 0-8 instead (half of all faces) |
| colour indices | u16 a face: bits 0-12 the first of its 3 colours, bit 15 semi-transparent, bits 13-14 the blend mode |

Animation entry (0x18 bytes): u32 start and end (offsets of its data in the file, loaded on
demand), u32 frames, s32 offset from +0xC to the mesh header it moves, u32 the `string_hash` of
its name (the game finds animations by name: `dbase_mesh_anim_name_load`), u32 0 (the loader's
node). Its data: s32 offset to its own vertex pool (0: the file's), then 16 bytes a frame: s32
offset from the frame to an index list, s32 offset from +4 to a second one, s32 blend
(0..0x1000), s32 self offset to the frame's attachment points (0: none). An index list is a 0x14-byte bounding box, then a u16 per mesh vertex
(`pool index << 2 \| flags`). A frame with blend 0 is a keyframe (its list's positions); the
others blend the first list's towards the second's (1/3, 2/3: frames between two keyframes).
1,037 animations in 225 files, 49,151 frames of which 13,648 keyframes.

`<id>.mdl/` holds:

- `model.bin`: the file as it is.
- `model.obj` and `model.mtl`: one object per mesh, the vertices as the file's integers with y
  and z negated (CTR-tools' orientation), normals, the faces with their texture coordinates and
  a material each (the texture set's image, a recoloured copy for a bit-15 face, or the
  untextured face's average colour).
- `map.json` (files with level data, 205): the maps (cell size, clear colour, paths with their keys,
  triggers, contours with their edges, placements), the map directory's entities (each one's mesh header offset and group)
  and the scripts (below).

  A script (`script_header`: threads, subs, end, start and frame counts, then a self offset per
  thread) runs threads of six types, each read by its own update function in `src/main/scripts.c`
  (`ScriptCommands`). Every thread starts with s32 type, start frame, end frame, total frames, key
  count; the keyed tracks then hold keys of s32 start, s32 len, ... ended by a key with len 0 (each
  key starts where the one before ended):

  | Type | Thread | After the header |
  |---|---|---|
  | 0 | model | 4 s32s (+0x18 flags ORed into the subengine's), then 0x50-byte keys from +0x24: position, rotation (a quaternion), scale, fade |
  | 1 | emitter | the prototype's `partdata`: flags, position, particle counts, lifetimes, speeds, rotation and gravity ranges, fades, scale |
  | 2 | camera | s32, s32 distance, then 0x28-byte keys from +0x1C: look-at point, eye |
  | 3 | entity | a frame range (aframe, start, end, delay, prolong, sort priority), then 0x4C-byte keys from +0x30: position, rotation, scale, alpha |
  | 4 | fade | start, end |
  | 5 | sub-script | the script it plays, sort priority, 2 spares, 18 s32s (position, angles, scale) |

  3,333 threads (2,877 model, 177 entity, 138 emitter, 83 camera, 44 fade, 14 sub-script), 12,605
  keys. Threads can share bytes (a model track that starts inside another's keys), so pack writes
  only the fields you change. Playtested 2026-10-28: shifting the Eurocom boot logo's model keys
  (`07D`, `DB_CutsceneIntroEurocom`, boot step 2 in `IntroSteps`) 3,000 units right moved the logo
  off-centre (300 wasn't noticeable: its pieces are about 1,500 apart). The `uka/` files are Uka
  Uka's model with a short script of its own; the scenes are the `level_*` files.
- `model.glb`: the same as glTF (textures embedded; positions as the file's 16-bit integers,
  KHR_mesh_quantization, scaled 1/256), with each animation as morph targets (a keyframe each)
  and a weights track: Blender imports them as shape keys and actions. 30 frames a second (the
  real speed isn't decoded).

`make disc` writes `model.obj`'s vertex positions and normals into `model.bin`: move vertices,
keeping each mesh's count and order (Blender: Split > Keep Vert Order). Texture coordinates,
materials, colours and the animations are for viewing: pack ignores them, and they stay as they
are in `model.bin`. Edit the textures in the texture set's PNGs. `make disc` also writes
`map.json`'s map fields into `model.bin`: move or turn placements, triggers, path keys and contour
edges (an edge's length and angle are stored, not worked out: keep them matching its points), change
their ids and flags (playtested 2026-10-28: raising Warp Room 1's 81 placements by 400 raised its
floor and scenery, so the room's geometry is placed objects, while Crash stayed at his old height:
the hubs have no ground in the file: with no level `GetContour` hook the game stands everything on
the flat `World_floor_level` and on solid objects, `AddPlatformContour`); the counts stay (nothing can be
added or removed), and two triggers that share
one info record must keep the same values. The entities and scripts are for viewing.

## Title screens (`.tga`)

`tools/scripts/tga.py`. Plain TGA 2.0: an 18-byte header (type 2, uncompressed; 16 bits a pixel,
x1r5g5b5; 1 attribute bit, unused in all five; rows bottom-up), the pixels, the 26-byte footer.
Four are 512x256, `089` 512x240. `<id>.tga/` holds `screen.png` (RGB, the top 5 bits of each
channel count) and `tga.json` (header and footer as they are). `08A` is the Universal Interactive
Studios screen.

## Mode text (`overlays/text/*.bin`)

`tools/scripts/txt.py`. A u32 (the file's number, 7 for `ball` to 18 for `tank`), then the mode's
strings (menu names, objectives, controls), each zero-terminated and starting on a 4-byte
boundary, then data with absolute pointers to them (e.g. `0x800AEC64`): a string can change but
not move or grow past its slot (its start to the boundary after its terminator). In the strings,
`0A` and `0D` break lines (`0D` is followed by two digits, a layout code) and `FC`..`FF` look like
button icons. Four files (`ring`, `dino`, `kegs`, `mallet`) hold, after "DECREASE SCORE TO ZERO",
the tail of a string cut short in development ("9IN THE ALLOTTED TIME").

The data after the strings is the mode's menus (the file loads at `ModeTextBuffer`,
`0x800AEC60`): panel records (`PlaceRec`, 0x24 bytes: s32 type, s16 x, y, z, scale and four more,
a pointer at +0x14 to the record's text or image, s32, two s16, s32) in lists that end at a type
below 2, which `ApplyPlaceRecs` draws, reached through pointer tables (the engine's
`PauseMenuTexts` gives each game mode's pause table: a pointer to a zero-separated array of list
pointers per pause screen). Types 3, 11 and 14 place text or a sprite (14 picks its text by
language), 4, 12, 16 and 18 a template, 13 shows or hides the records after it by the game's state
(teams, paused, the level's flags in `unk20`). An x with 0x8000 set is centred. 161 lists, 2,058
records in the 12 files, about 93% of their data; the rest is the pointer tables.

`<id>.txt/` holds `data.bin` (the file as it is), `text.txt` (`@offset: text` a line; `\n`,
`\r`, `\xNN`, `\\` for the bytes above) and `menus.json` (each list's records, a text pointer as
`@offset` with its text beside it, another pointer as `&offset`). An unchanged line keeps its slot
byte for byte; a changed one is written in, zero-padded, and one too long for its slot is refused.
Pack writes the menu fields you change (move a panel, point it at another string); lists and
records can't be added. Playtested 2026-10-28: Ballistix's pause "OBJECTIVE" title moved to y 140.

## Sound packs (`.sfx`)

`tools/scripts/sfx.py`; the split into Sony's files is CTR-tools' reading (`BashSfx.cs`).
All 160 pack back byte for byte (checked against the retail DAT).

| Part | What |
|---|---|
| offset table | u32s: VB, VH, each SEQ (the first is the table's size; the last SEQ runs to the end of the file). The 132 effects-only packs have one empty SEQ, its offset the file's end; 27 music packs have two, one has three |
| VB | the samples, SPU ADPCM, back to back: 16-byte blocks of 28 samples (byte 0 shift \| filter << 4, byte 1 flags). Every sample starts with a silent block; a one-shot ends with flag 1 on its last block, then a terminator block (flag 7); a loop has flags 6, 2 ... 2, 3 |
| VH | a VAB header (`pBAV`, version 7): 0x20 bytes, 128 program slots of 16, 16 tones of 32 per program, then u16 sample sizes / 8 (slot 0 unused): one per sample in the effects packs, all 256 in the music packs |
| SEQ | Sony sequences (`pQES`), the music |

`<id>.sfx/` holds `bank.vh` and `bank.vb` (a standard VAB pair: PS1 sound tools, e.g. VGMTrans,
open it with the SEQs), `music_N.seq` and `music_N.mid` (the SEQ as a MIDI file: an edited one
becomes the SEQ, tracks merged; a SEQ's time-signature denominator that isn't a power of two, 3
in one song, rides in a sequencer-specific event; all 57 songs go SEQ -> MIDI -> SEQ byte for
byte), `sample_NNN.wav` (sample N of the VAB, decoded to 16-bit
mono) and `sfx.json`. The WAVs are written at a nominal 22050 Hz: the game sets each note's pitch
from the VAB's tones, so the true rate isn't one number; pack ignores it. Edit a WAV (16-bit mono)
and pack encodes it to ADPCM (filter and shift chosen per block for the least error; about 55-70
dB SNR on the game's own samples), keeps its one-shot or loop shape, and rewrites the VH's size
table and the offsets; an unchanged WAV keeps its original bytes. A pack may shrink, not grow.

## The memory card (`BASCUS-94570`)

Not on the disc: the game's save, one 8 KB block on a memory card. `tools/scripts/card.py` reads a
128 KB card image (RetroArch's `.srm`, `.mcr`, `.mcd`) or the save alone, the layout as `card.c`
reads it:

| Offset | What |
|---|---|
| 0x000 | the PsyQ save header: `SC`, the icon's frame count (2) and type, blocks (1), the title (64 bytes of Shift-JIS: `ＣＲＡＳＨ　ＢＡＳＨ` and each slot's completion as `％NNN`), the icon's CLUT at 0x60, two 16x16 4-bit frames from 0x80 (the CB logo and Crash's face) |
| 0x200 | 4 slots (`CardSlot`, 0x194 each): 8 summary bytes (the heroes picked and out), then the slot's `GameState` (`include/bash/nas.h`, the game state at `GameInfo`) |
| 0x850 | `CardSaveFlags`: a word set to 1, then 8 bytes cleared with the slots |
| 0x85C | `CardSaveSum`: the sum of the s32s from 0x200 to 0x850; a save whose sum doesn't match isn't loaded |
| 0x860 | unused to the block's end |

`card.py unpack CARD` writes `<name>.card/`: `save.bin`, `card.json` (the title, also as ASCII,
the flags, whether the sum is right, and each slot's summary and every `GameState` field, named from
the C struct through `struct_layout.py`) and `icon_0.png`, `icon_1.png`. `card.py pack DIR CARD
OUT` writes the fields you change, works the sum out again and puts the save into a copy of CARD.
Playtested 2026-10-28: clearing level 1's awards (`adventure.unkE4[0..3][0]`) in a 200% slot made
the load screen show 194%. The completion counts `adventure.unkE4[5][33]`, five awards per level
(`count_completion`: 1 each, the second counting 2 more at 2, the fifth 8).

## What isn't decoded

Every file comes out editable or viewable. What is kept only as bytes (exact, in the folders'
`.bin` files):

- Models: the animations' timing (not in the entry: +0x10 is the name's hash), the vertices' and
  indices' flag bits, the header words not read by the exe, and every file's sections other than
  the meshes, animations and maps: the placements' words the exe doesn't read, the contours'
  unnamed words, the scripts' threads (`script_thread` records), the
  on-demand chunks.
- Texture sets: the header word at 0x14, and the images' header words.
- Mode text: the menu records' fields the code doesn't name (`unkC` ..), and the pointer tables.
- Editing beyond the slots: a file can't grow (the DAT would have to be laid out again), and
  animations, texture coordinates and the mode data can't be edited yet.
