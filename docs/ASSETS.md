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

What the entries are (first u32 is a type or a count, not a magic number):

| Entries | Count | What | Decoded |
|---|---|---|---|
| mostly even ids, `000`..`3DD` | 400 | texture sets, type 8 | `<id>.tex/` (tex.py) |
| odd ids after each texture set | 399 | probably the model the set before it textures (starts `29 00 16 0C`; its u32 at +0x50 equals the set's at +0x14) | no |
| `1E7`, `2CC`..`2D8`, `3DF` | 15 | code overlays (`make disc` writes the built ones) | C/asm |
| e.g. `099`, `126` | ~130 | sound banks: SPU ADPCM after a header (type 0x0C, 0x14) | no |
| `08D`..`098` | ~15 | text (menu and objective strings; the first u32 is a count) | no |
| `088`..`08C` | 5 | 256 KB blocks starting `00 00 02 00` | no |

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

## Next formats

- Models (the odd entries): CTR-tools has `BashMesh.cs`; whether it covers these is unchecked.
- Sound banks: CTR-tools `BashSfx.cs`; generic SPU ADPCM rippers may read the banks as they are.
- Text: a count and fixed-size strings; small.
