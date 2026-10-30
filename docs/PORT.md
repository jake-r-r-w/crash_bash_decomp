# The PC port

The plan is ROADMAP-B, Part 6. This file is the boundary between the decomp and the port.

## Layout

```
src/                  the game's C, compiled by both builds
  main/  overlays/
  ps1/                PS1 only: the exe header, bios_1's data, the mod-chip check, the overlay id words
include/              the game's headers, used by both builds

port/                 PC only: everything new goes here
  CMakeLists.txt
  main.c              the PC entry point and its test options
  playtest.c          --playtest: plays minigames to the end with computer players
  overlays.c          puts an overlay's data back when the game loads it, relocates the mode text
  link_names.c        names the PS1 gets from its link, as real variables
  data_order.txt      the game's data objects in the PS1's order (tools/scripts/pc_data_order.py)
  sdk/                PC versions of PsyQ's functions, under the same names
    include/          libgpu.h, libgte.h, libsnd.h, libcd.h, libpad.h, ...
    gpu.c gpu_raster.c  libgpu and a software GPU; gte.c  the GTE and libgte
    cd.c  pad.c  mcrd.c  etc.c  libcd, libpad, libmcrd, libetc and libapi
    snd.c             libsnd's voice side and the SPU (not yet the sequencer)
  platform/           the host: sdl.c (window, sound, input), or no_host.c without SDL

Makefile  configs/  tools/  .decomp/    the PS1 decomp only
docs/                 both
```

Git-ignored, as for the decomp: `build/pc/` (its output) and `ext/sdl2/` (SDL2, if the system has none).

## The rules

1. **`src/` and `include/` always match retail.** Any change there has to pass `tools/dk make check`,
   including a fix the port asks for.
2. **The port can't affect the matching build.** The Makefile compiles only what's in `src/` and
   `asm/`, so nothing in `port/` reaches it.
3. **Dependencies go one way.** `port/` can include the game's headers. The game's code never includes
   anything from `port/`.
4. **The game's C keeps calling PsyQ by name.** The PC build finds `port/sdk/include` where the PS1 build
   finds `ext/psyq47/include`, so most game files need no edit.
5. **`#ifdef BASH_PC` is the last resort** in shared code. The cases so far:
   - the three `gte_*` macros in `include/bash/main.h` (inline asm);
   - `draw_verts`' register pins (`src/main/prims.c`);
   - four labels of Tank's into `Heroes` (`include/bash/tank_ovl.h`);
   - `N2OSpawnFirstMissile`, which spawns at whatever's on the stack;
   - `PASSED` and `LOW_RAM` (`include/common.h`, below);
   - `RAM_TOP` and the slot's names (`include/bash/main.h`, `engine.h`, below);
   - `shell_init`, which doesn't load the engine, and `LoadModeOverlay`, which says what it loaded.
6. **No Sony code in `port/`.** Write `sdk/` from what the game needs and from documented behaviour.

## What the PC build compiles

`src/main/` and `src/overlays/`, 38 files. It leaves out `src/ps1/` (18 files), which only make sense on
the PS1 or in its executable's format:

| Files | Why PS1 only | On PC |
|---|---|---|
| `header.c` | The PS-X EXE header | The host's own executable format |
| `bios_1.c` | The data of Sony's libcd object | `sdk/` replaces libcd |
| `modchip.c`, `modchip_msg.c` | Reads the BIOS's region byte, draws the warning from its font | An empty `modchip_check_region` |
| `<game>_id.c` (14) | The first word of each overlay's file | Not needed: the overlays link into the program |

## Targets

Windows, Linux, macOS and Android (decided 2026-10-30).

- **macOS and Android need 64-bit**, so 6.5 is required for them. The 32-bit build is the stage that
  gets the game running on Windows and Linux, and then the reference for 64-bit.
- **They're also ARM**, as are many Windows and Linux machines, so the shared C can't assume x86. In
  particular a plain `char` is unsigned on ARM Android and Linux, and signed on x86 and Apple's ARM.
- **Clang builds for all four** (with the NDK on Android), so it's the port's main compiler, with GCC
  as a second check.
- **`platform/` uses SDL2**, which covers all four, and a renderer that does too.
- **Android also needs** touch controls, its own packaging, and a way to get the game's data from the
  user's own disc onto the device.

## How the PS1's assumptions are kept

The game's C assumes a lot about its machine. On PC:

- **Memory below 16 MB.** A primitive's link keeps 24 bits of its address (`P_TAG`, `addPrim`), and the
  game's C reads and writes those bits itself. So the program loads at a fixed low address (0x400000,
  no ASLR) with the heap inside it, and `main.c` checks. `DrawOTag` takes a link's top bits from the
  list's own address, so a 64-bit build needs one 16 MB block aligned to 16 MB for lists and primitives.
- **The data's layout.** The game reads some globals through others: `GameState` over `GameInfo` and the
  names after it, arrays that run on into the next object, a table's `[-1]`. Clang doesn't keep a file's
  data in source order (it emits a global at its first use), so each object gets a section
  (`-fdata-sections`), and the linker lays them out from `data_order.txt`, generated from the PS1's link.
  Run `tools/dk python3 tools/scripts/pc_data_order.py` when data objects are added or renamed. Zeroed
  data stays with the rest (`-fno-zero-initialized-in-bss`) and unused data stays too (`--no-gc-sections`).
  Checked by comparing every neighbour's distance with the PS1's: only `s16`s that the PS1 pads to a word
  differ, and no read through a global lands anywhere different because of them.
- **No optimiser on the game's C (`-O0`).** An optimiser takes a global's declared size at its word.
  Clang deleted `GameInfo`'s first word's write, because a later write further on "covered" the 4-byte
  object. `sdk/` and `platform/` are optimised, and -O0 is plenty for a PS1 game.
- **Null pointers (`LOW_RAM`).** The PS1 has RAM at address 0, so reads and writes through a null pointer
  just work, and the game has them: `SetHeroIdle` before a hero has its movement record, the last
  subengine of a list. `LOW_RAM(p)` gives such a place a block of its own on PC (`p` on the PS1). As a
  safety net, on 32-bit Windows `main.c` catches any it missed, points the instruction at the same block,
  and prints `null page: ADDRESS`, once per place: mark those with `LOW_RAM`.
- **Writable constants.** The game writes the completion figures into a `const` string, so `.rdata` is
  writable, as all RAM is on the PS1.
- **The heap and the slot.** `HeapStart` is one block as big as the PS1's slot to `0x801FF000`, so the
  game has the same memory to work with. The engine isn't loaded. The overlay's file is still read into
  the slot, unused, so the heap starts where it does on the PS1.
- **Overlays.** They all link into the program (their 4,368 global names have one clash, `XZBoxToSphere`,
  renamed per file). Each one's data is saved at startup and restored when the game loads it.
- **Mode text.** Its files hold absolute addresses, which `overlays.c` relocates: every word in
  `ModeTextBuffer`'s range is one (checked on all 12 files).
- **Calls that rely on the caller's registers.** On the PS1, `f()` can hand on the caller's own `a0`
  without saying so, because nothing has touched it. On PC that argument would be junk, so those calls say
  it with `PASSED`: `f(PASSED(arg0))` is `f(arg0)` on PC and `f()` on the PS1. Each direct one was checked
  against the retail code, and each call through a function pointer against what's assigned to it.
- **Drawing.** `gpu_raster.c` is the PS1's GPU in software: 1 MB of VRAM and the GP0 commands, so texture
  tricks, VRAM copies and semi-transparency work as they do on the console. A faster or sharper renderer
  can come later.
- **Timing.** `VSync` is the one clock: each vblank runs the sound tick, the pads and a vblank's worth of
  audio. `VSync(1)` always says 0, so the heap compacts at its full rate. CD reads finish at once.
- **The GTE.** `gte.c` has the hardware's fixed-point rules, and libgte's functions keep their original
  order of multiplies and shifts. `rcossin_tbl` and `VectorNormal`'s table are computed at startup, and
  match a retail exe's exactly.

## What's known so far

- **It plays (6.2).** `--playtest 0-32` plays every minigame to the end with computer players, as
  `tools/playtest/playtest.mjs` does on the PS1 build. In a window it runs at the game's speed, with
  sound effects, keyboard and controllers, and saves to `memcard1.mcr`.
- **Not yet: music and reverb** (6.3), so sound effects only. Their volumes and pitches use the documented
  formulas, not libsnd's own tables yet. The SPU's interpolation is four-point, not the hardware's table.
- **Not yet: analog sticks and rumble.** Pads are digital, and a controller's left stick works as the d-pad.
- **Not checked against the PS1 yet (6.4).** The minigames finish and look right, but no one has compared
  them frame by frame. The drawing's rounding and each uninitialised stack read can differ.
- **The other compilers.** GCC 13 compiles everything but `sdl.c` with 0 failures. Linux isn't linked yet,
  because the Docker image has no 32-bit libc, and its link options are untested.
- **The loose C is switched off in CMake, not fixed.** About 1,650 pointer type mismatches and integer
  and pointer mixes are legal old C and work while pointers and ints are the same size. A 64-bit build
  needs `-Wint-conversion` back on (6.5).
- **Clang's other warnings have been read.** `-Wuninitialized` and friends find 21 reads, and only
  `N2OSpawnFirstMissile` really uses a junk value. `memset(RaceBarrels, 0x3600, 0)` is the original's
  bug and clears nothing, so the port keeps it.
- **32-bit first (decided 2026-10-30).** The shared C has about 430 casts of an address to an integer
  (`(s32)&...`), and the data has more. A 32-bit build keeps them working, along with the structs'
  layouts and size checks. 64-bit is a later step (ROADMAP-B 6.5), which also has to replace the fixed low
  address, the data's layout and the null-page net, so write `port/` itself to work on either: no pointer
  kept in an `int`.

## Building

It needs a 32-bit C compiler on the host, not the Docker image; CMake stops if the compiler is 64-bit.
On Windows, with [llvm-mingw](https://github.com/mstorsjo/llvm-mingw) and Ninja, and SDL2's
`SDL2-devel-*-mingw.tar.gz` from [its releases](https://github.com/libsdl-org/SDL/releases) unpacked in
`ext/sdl2/`:

```sh
cmake -S port -B build/pc -G Ninja -DCMAKE_C_COMPILER=i686-w64-mingw32-clang
cmake --build build/pc
```

The result is `build/pc/crashbash.exe`. Without SDL2 it still builds, with no window.

## Running it

From the repository's root, so that it finds the game's data: `CRASHBSH.DAT` in the current folder, then
`build/disc/files/CRASHBSH/` (`make disc` builds it from `assets/`, edits included), then
`disc/files/CRASHBSH/` (your own disc's). `--data FILE` names it. The memory card is `memcard1.mcr`, a raw
128 KB card that `tools/scripts/card.py` and emulators read; `--card FILE` names it.

| Keyboard | Pad |
|---|---|
| Arrows | D-pad |
| Z, X, A, S | Cross, Circle, Square, Triangle |
| Enter, right Shift | Start, Select |
| Q, W, 1, 2 | L1, R1, L2, R2 |

Up to four game controllers work, with a multitap once there are more than two. Alt+Enter is fullscreen.

For a script: `--no-host` (no window), `--no-pace`, `--frames N`, `--shot VBLANK:FILE.png`,
`--press VBLANK:FRAMES:BUTTONS`, `--wav FILE.wav`, and `--playtest MODES [--playtest-shots DIR]`
(`port/main.c`).
