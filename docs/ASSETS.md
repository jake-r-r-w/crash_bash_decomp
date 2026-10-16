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
| the entry after each texture set | 400 | model files (`.mdl`), textured by the set before them | 319 `<id>.mdl/` (mdl.py, geometry only) |
| `1E7`, `2CC`..`2D8`, `3DF` | 15 | code overlays (`make disc` writes the built ones) | C/asm |
| `.sfx`, e.g. `099`, `126` | 160 | sound packs: SPU ADPCM after a header; the music ones VAB/SEQ (the name list says) | no |
| `08D`..`098` | 12 | mode text (`overlays/text/*.bin`): strings, then data pointing at them | `<id>.txt/` (txt.py) |
| `088`..`08C` | 5 | title screens (`screens/*.tga`): plain TGA files | `<id>.tga/` (tga.py) |

The rest of the disc holds nothing more of Crash Bash's: `BASHY.` (30 MB) is all zeros, a
filler, and `SPYRO3/` is the bundled Spyro 3 demo.

## Texture sets (type 8)

`tools/scripts/tex.py`; layout as CTR-tools reads it, confirmed on all 400 sets (each packs back
byte for byte) and by eye (the PNGs show the game's textures in their colours).

| Offset | Size | Field |
|---|---|---|
| 0x00 | u32 | type, 8 |
| 0x04 | u32 | size of the file |
| 0x08 | u16, u16 | number of images, number of CLUTs |
| 0x0C | u32 | bytes from 0x0C to the first image |
| 0x10 | 16 | kept as they are (CTR-tools: skip to palettes, unknown, offset of the animated textures, 0) |
| 0x20 | | CLUTs: u32 n (16 or 256), then n 16-bit colours |
| | | images: u16 w (VRAM 16-bit units), u16 h, u32 (unknown; CTR-tools: margins/size bytes), u32 0, s16 CLUT << 1 \| 8-bit, s16 (0, 3 for some banners), u32 (1, 2, 8, 16), then w * h * 2 bytes of pixels |
| | | then the animated textures (u32 8, size, count, records) or zeros: `tail.bin`, not decoded |

Pixels are 4-bit (low nibble = left pixel, width w * 4) or 8-bit (width w * 2). A negative CLUT
index (the last image of some sets) means none of its own; such an image is shown in grey.

`<id>.tex/` holds `img_NNN.png` (indexed, its CLUT as the palette: edit the pixels; pack reads
the indices only), `cluts.png` (one row per CLUT: edit colours here; r, g, b are 5-bit values
times 8, alpha 255 = STP off, 128 = STP on, 0 = the colour 0x0000, transparent), `tex.json` (the
fields above) and `tail.bin`. An image keeps its size and depth; at most 16 (or 256) colours.

## Model files (`.mdl`)

`tools/scripts/mdl.py`; the mesh reading is CTR-tools' (`CTRFramework/Code/bash/BashMesh.cs`),
confirmed on 319 of the 400 files: in each, every mesh's triangle strips use exactly its vertex
count, each file packs back byte for byte, and the meshes look right (Uka Uka's mask and sky dome
in `001`, Oxide's hover vehicle and its lower-detail versions in `13F`). The other 81 (fonts,
the main-menu arena models, some arena objects) differ and stay `.bin`.

| Offset | Size | Field |
|---|---|---|
| 0x00 | 0x54 | file header, not decoded (u32s that look like self-relative offsets) |
| 0x54 | u32 | number of meshes |
| 0x58 | 0x34 each | mesh headers: u32 0, u32 0, s16 faces, s16 (4, 6, ...), u32, then six s32 offsets relative to their own position: vertices, strips, after the vertices, two per-face arrays' bounds, the end; then three u32 |
| (vertices) | 0x14 + 8 each | a 0x14-byte block (looks like a bounding box), then s16 x, y, z and a u16 (small values, 0-3: not decoded) |
| (strips) | u16 each | faces << 8 \| flags, until 0xFFFF; a strip of n faces takes the next n + 2 vertices; flag 0x08 flips the first face's winding |
| (after the vertices) | 2 per face, twice | per-face values (small rising numbers; values near 0x240): probably texture coordinates and texture choice, not decoded |

The meshes are often under a fifth of a file; the rest (probably animation) isn't decoded.

`<id>.mdl/` holds `model.bin` (the whole file) and `model.obj` (one object per mesh: the file's
integers with y and z negated, CTR-tools' orientation; faces from the strips, no texture
coordinates yet). `make disc` writes the OBJ's vertex positions into `model.bin`: vertices can be
moved, but each mesh keeps its vertex count and order (Blender: import with Split > Keep Vert
Order). Next for models: texture coordinates (the per-face arrays and the vertices' u16), so the
OBJ can carry the textures; the engine's model loader, once matched, will say.

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

`<id>.txt/` holds `data.bin` (the file as it is) and `text.txt` (`@offset: text` a line; `\n`,
`\r`, `\xNN`, `\\` for the bytes above). An unchanged line keeps its slot byte for byte; a changed
one is written in, zero-padded, and one too long for its slot is refused.

## Next formats

- Sound packs (160): CTR-tools `BashSfx.cs` converts them to VAB/SEQ (Sony's sound bank and
  sequence formats); from there, WAV for the samples and MIDI for the music.
