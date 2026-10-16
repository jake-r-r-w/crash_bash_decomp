# Playing a build

| Command | What it does |
|---|---|
| `tools/dk make disc` | Packs `build/main.exe` and every overlay into `build/CrashBash.bin`/`.cue`, a disc image any PS1 emulator boots. |
| `tools/dk make disc-check` | `make disc`, then checks the image is the retail one byte for byte (`configs/USA/disc.sha`). |

The matching build is the retail bytes, so `disc-check` passing already proves it plays like
retail.

## The disc packer

`tools/scripts/pack_disc.py` starts from your retail image as `decomp.py extract` unpacked it
(`disc/files/`, `disc/layout.xml`), replaces `SCUS_945.70` with `build/main.exe`, builds `CRASHBSH.DAT` from
its files in `assets/CRASHBSH/`, writes each `build/<ovl>.bin` over it at its retail offset, and
rebuilds the image with `mkpsxiso` so every file keeps its retail sector. A file may shrink (it is
zero-padded) but not grow: an overlay or a DAT entry is a fixed slot, and the packer refuses a
bigger one.

## The game's files (`assets/`)

`tools/dk python3 tools/scripts/dat.py extract` splits your retail `CRASHBSH.DAT` into its 992
files, `assets/CRASHBSH/000.bin` .. `3DF.bin` (ids as ARCHITECTURE.md names them), with
`index.json` (each one's name, sector, size and format; the code overlays are tagged) and `filler.bin` (the bytes
between files). `make disc` runs it the first time by itself. Edit a file and `make disc` puts it
on the disc; delete `assets/CRASHBSH/` to start again from the retail DAT (extracting takes about
6 minutes). The 400 texture sets come out as `<id>.tex/` folders of PNGs: edit an image's pixels in
`img_NNN.png` (keep it indexed, same size) and colours in `cluts.png`; the formats and the
rules are in [ASSETS.md](ASSETS.md). The 400 model files come out as `<id>.mdl/`: `model.obj`
(textured, with `model.mtl`) to move vertices in Blender or any OBJ editor, keeping each mesh's
vertex count and order, and `model.glb` to view a model with its animations (Blender: File >
Import > glTF). The 5 title screens are `<id>.tga/screen.png`, and the 12 modes' text is
`<id>.txt/text.txt` (a string may change but not outgrow its slot). The 160 sound packs
are `<id>.sfx/`: each sample a WAV (edit it, 16-bit mono), the bank as Sony's VAB, and the music as
SEQ and MIDI (edit the `.mid`). Only the 15 code overlays stay `<id>.bin`. It is your game data: `assets/` is git-ignored, never commit it. The 15 code overlays' entries are there too, but `make disc`
always writes the built `build/<ovl>.bin` over them.

## Playing it

On Windows, RetroArch with the PCSX-ReARMed core (no BIOS file needed):

```
winget install --id Libretro.RetroArch -e
# core: https://buildbot.libretro.com/nightly/windows/x86_64/latest/pcsx_rearmed_libretro.dll.zip
#       unzipped into C:\RetroArch-Win64\cores\
"C:/RetroArch-Win64/retroarch.exe" -L "C:/RetroArch-Win64/cores/pcsx_rearmed_libretro.dll" build/CrashBash.cue
```

Keys: arrows (d-pad), Z (cross), X (circle), A (square), S (triangle), Enter (start), right
Shift (select), Q/W (L1/R1), F1 (menu), Esc twice (quit). A pad works without setup. Any other
emulator works too; DuckStation needs a real BIOS dump.
