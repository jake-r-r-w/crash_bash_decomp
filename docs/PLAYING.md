# Playing a build

| Command | What it does |
|---|---|
| `tools/dk make disc` | Packs `build/main.exe` and every overlay into `build/CrashBash.bin`/`.cue`, a disc image any PS1 emulator boots. |
| `tools/dk make disc-check` | `make disc`, then checks the image is the retail one byte for byte (`configs/USA/disc.sha`). |

The matching build is the retail bytes, so `disc-check` passing already proves it plays like
retail.

## The disc packer

`tools/scripts/pack_disc.py` unpacks your retail image with `dumpsxiso` once into
`build/disc-cache/`, replaces `SCUS_945.70` with `build/main.exe`, builds `CRASHBSH.DAT` from
its files in `assets/CRASHBSH/`, writes each `build/<ovl>.bin` over it at its retail offset, and
rebuilds the image with `mkpsxiso` so every file keeps its retail sector. A file may shrink (it is
zero-padded) but not grow: an overlay or a DAT entry is a fixed slot, and the packer refuses a
bigger one.

## The game's files (`assets/`)

`tools/dk python3 tools/scripts/dat.py extract` splits your retail `CRASHBSH.DAT` into its 992
files, `assets/CRASHBSH/000.bin` .. `3DF.bin` (ids as ARCHITECTURE.md names them), with
`index.json` (each one's sector and size; the code overlays are tagged) and `filler.bin` (the bytes
between files). `make disc` runs it the first time by itself. Edit a file and `make disc` puts it
on the disc; delete `assets/CRASHBSH/` to start again from the retail DAT (extracting takes about
4 minutes). The 400 texture sets come out as `<id>.tex/` folders of PNGs: edit an image's pixels in
`img_NNN.png` (keep it indexed, same size) and colours in `cluts.png`; the formats and the
rules are in [ASSETS.md](ASSETS.md). Everything else is still raw (`<id>.bin`: models, sounds,
text). It is your game data: `assets/` is git-ignored, never commit it. The 15 code overlays' entries are there too, but `make disc`
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
