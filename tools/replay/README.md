# Playing and testing a build

Three tools turn the build into something you can play and test:

| Command | What it does |
|---|---|
| `tools/dk make disc` | Packs `build/main.exe` and every overlay into `build/CrashBash.bin`/`.cue`, a disc image any PS1 emulator boots. |
| `tools/dk make disc-check` | `make disc`, then checks the image is the retail one byte for byte (`configs/USA/disc.sha`). |
| `tools/dk make replay [REC=...]` | `make disc`, then plays an input recording on the retail disc and the built one in a headless emulator and reports the first frame that differs. |

The matching build is the retail bytes, so `disc-check` passing already proves it plays like
retail. The harness is for builds that aren't byte-identical: `NON_MATCHING` experiments, the
shifted builds (ROADMAP.md, Phase 4) and the modern-compiler build.

## The disc packer

`tools/scripts/pack_disc.py` unpacks the user's retail image with `dumpsxiso` once into
`build/disc-cache/` (kept by `make clean`, redone when the image changes), copies it to
`build/disc/` every run (unchanged files as hard links), replaces `SCUS_945.70` with `build/main.exe`, writes each `build/<ovl>.bin`
back into `CRASHBSH.DAT` at the offset `.decomp/project.json` records for it, and rebuilds the
image with `mkpsxiso` from `dumpsxiso`'s layout, so every file keeps its retail sector. A file
may shrink (it is zero-padded to its retail size) but not grow: an overlay is a slot in
`CRASHBSH.DAT`, and a bigger one needs the DAT and its index (993 entries at `0x8004E110`)
rebuilt. The packer refuses it with the sizes.

## Playing it

On Windows, RetroArch with the PCSX-ReARMed core (the harness's emulator, no BIOS file needed):

```
winget install --id Libretro.RetroArch -e
# core: https://buildbot.libretro.com/nightly/windows/x86_64/latest/pcsx_rearmed_libretro.dll.zip
#       unzipped into C:\RetroArch-Win64\cores\
"C:/RetroArch-Win64/retroarch.exe" -L "C:/RetroArch-Win64/cores/pcsx_rearmed_libretro.dll" build/CrashBash.cue
```

Keys: arrows (d-pad), Z (cross), X (circle), A (square), S (triangle), Enter (start), right
Shift (select), Q/W (L1/R1), F1 (menu), Esc twice (quit). A pad works without setup. Any other
emulator works too; DuckStation needs a real BIOS dump.

## The replay harness

`tools/replay/replay.py` loads the PCSX-ReARMed libretro core (built into the toolchain image at
`/opt/libretro/`) through ctypes: no window, no audio device, no wall clock. The core options
are pinned in `OPTIONS` (interpreter, no threads, no CD read-ahead, NTSC, HLE BIOS), so a run
depends only on the disc and the recording: the same disc twice gives the same trace.

Every frame gives one trace line: the frame number, a hash of the image, and the game values
listed in `values.txt`, read from RAM. Values are `<symbol[+offset]> <u8|s8|u16|s16|u32|s32>`,
looked up in each disc's own symbols (`--syms`, comma-separated: sym files for the matching
build, ELFs for a shifted one), so a build that moved them is read where it keeps them. Only
values that mean the same in every build go there: seeds, modes, health, scores; no pointers.

```
tools/dk make replay                                   # retail vs build, recordings/battle.txt
tools/dk make replay REC=tools/replay/recordings/x.txt
tools/dk python3 tools/replay/replay.py run DISC.cue REC --shot 2300 --shot 2600   # PNGs in build/replay/
tools/dk python3 tools/replay/replay.py run DISC.cue REC --video build/demo.mp4 --video-from 300
tools/dk python3 tools/replay/replay.py compare A.cue B.cue REC --syms-b build/main.elf,build/engine.ovl.elf
```

`compare` prints `REPLAY OK n frames identical`, or `REPLAY DIFF at frame N` with the values
that differ, screenshots of both discs at that frame (`build/replay/{a,b}_shot_N.png`) and both
traces (`build/replay/{a,b}.trace`). A game value usually differs before the image does.

### Recordings

`recordings/*.txt` are written by hand: lines of `<frames> [buttons]`, holding those buttons
(none if empty) for that many frames, port 1. `#` starts a comment. Buttons: `cross circle
square triangle start select up down left right l1 r1 l2 r2 l3 r3`. Press a button for about 4
frames and leave a gap after it; menus ignore a press while they animate in. To write one, add
a few lines, run it with `--shot` at the frames you care about and look at the PNGs. Under the
HLE BIOS the title screen is up by frame 1800 and Select Game Type by 2300.

| Recording | Covers | Frames |
|---|---|---|
| `battle.txt` | Boot, Battle Mode, 1 player, default characters, the crate battle (`ovl5d`), two minutes of play | 7512 |

Keep them short (a few minutes, one per minigame). The roadmap's plan is a recording per
minigame, and a state-sequence comparison for builds whose code changes size (Phase 4).

### Limits

- Comparison is frame-exact. That is right for the matching build, but a build whose code is a
  different size can finish a heavy frame one frame earlier or later, and every frame after it
  then differs although the game is right. The state-sequence mode (ROADMAP.md, Phase 4) is
  for those.
- The HLE BIOS skips the Sony logo and differs from a real BIOS in timing. Both discs run
  under the same one, so comparisons are fair; pass `--bios-dir` with a BIOS dump to use it.
- Port 1 only. Memory cards are the core's default, empty on every run.
