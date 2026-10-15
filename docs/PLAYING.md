# Playing a build

| Command | What it does |
|---|---|
| `tools/dk make disc` | Packs `build/main.exe` and every overlay into `build/CrashBash.bin`/`.cue`, a disc image any PS1 emulator boots. |
| `tools/dk make disc-check` | `make disc`, then checks the image is the retail one byte for byte (`configs/USA/disc.sha`). |

The matching build is the retail bytes, so `disc-check` passing already proves it plays like
retail.

## The disc packer

`tools/scripts/pack_disc.py` unpacks your retail image with `dumpsxiso` once into
`build/disc-cache/`, replaces `SCUS_945.70` with `build/main.exe`, writes each `build/<ovl>.bin`
back into `CRASHBSH.DAT` at its retail offset, and rebuilds the image with `mkpsxiso` so every
file keeps its retail sector. A file may shrink (it is zero-padded) but not grow: an overlay is a
fixed slot in `CRASHBSH.DAT`, and the packer refuses a bigger one.

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
