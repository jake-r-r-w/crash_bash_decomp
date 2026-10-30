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
  main.c              the PC entry point
  link_names.c        names the PS1 gets from its link, as real variables
  sdk/                PC versions of PsyQ's functions, under the same names
    include/          libgpu.h, libgte.h, libsnd.h, libcd.h, libpad.h, ...
    stubs.c           every PsyQ function, doing nothing yet (6.2 replaces them)
  platform/           the host: window, renderer, audio device, input, files

Makefile  configs/  tools/  .decomp/    the PS1 decomp only
docs/                 both
```

Generated and git-ignored, as for the decomp: `assets/` (the port reads the game's data from it) and
`build/pc/` (its output).

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
   - `PASSED` (`include/common.h`, below).
6. **No Sony code in `port/`.** Write `sdk/` from what the game needs and from documented behaviour.

## What the PC build compiles

`src/main/` and `src/overlays/`, 38 files. It leaves out `src/ps1/` (18 files), which only make sense on
the PS1 or in its executable's format:

| Files | Why PS1 only | On PC |
|---|---|---|
| `header.c` | The PS-X EXE header | The host's own executable format |
| `bios_1.c` | The data of Sony's libcd object | `sdk/` replaces libcd |
| `modchip.c`, `modchip_msg.c` | Reads the BIOS's region byte, draws the warning from its font | Stub `modchip_check_region` |
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

## What's known so far

- **The game's C builds and links for PC (6.1).** All 38 files compile with Clang 22 and GCC 13, and
  link into a 32-bit Windows exe against the stubs. It doesn't start the game yet: that's 6.2. Linux
  is compiled but not linked yet, because the Docker image has no 32-bit libc.
- **Calls that rely on the caller's registers.** On the PS1, `f()` can hand on the caller's own `a0`
  without saying so, because nothing has touched it. On PC that argument would be junk, so those 44
  calls say it with `PASSED`: `f(PASSED(arg0))` is `f(arg0)` on PC and `f()` on the PS1. Each one was
  checked against the retail code. Left alone, because they're harmless: extra arguments, and the
  fourth parameter of `RaceRayHit` and `DinoRayHit`, which is never read. Calls through function
  pointers aren't checked yet.
- **The loose C is switched off in CMake, not fixed.** About 1,650 pointer type mismatches and integer
  and pointer mixes are legal old C and work while pointers and ints are the same size. A 64-bit build
  needs `-Wint-conversion` back on (6.5).
- **Clang's other warnings have been read.** `-Wuninitialized` and friends find 21 reads, and only
  `N2OSpawnFirstMissile` really uses a junk value. `memset(RaceBarrels, 0x3600, 0)` is the original's
  bug and clears nothing, so the port keeps it.
- **For 6.2:**
  - `P_TAG.addr` keeps 24 bits of an address, so ordering tables and primitives need a new form.
  - `rcossin_tbl` is all zeros.
  - `HeapStart`, `SlotOverlayId` and `SlotHeapStart` (`link_names.c`) are placeholders.
  - `__main` links to the host's C runtime.
- **The overlays can link into one program.** On the PS1 they share addresses, but their 4,368 global
  names have one clash: `XZBoxToSphere`, defined in Polar, Race and Dino, which CMake renames per file.
  So the PC build doesn't need to load overlays at runtime; `LoadModeOverlay` needs only the mode's
  text.
- **32-bit first (decided 2026-10-30).** The shared C has about 430 casts of an address to an integer
  (`(s32)&...`), and the data has more. A 32-bit build keeps them working, along with the structs'
  layouts and size checks, so the game can run before any of that changes. 64-bit is a later step
  (ROADMAP-B 6.5), so write `port/` itself to work on either: no pointer kept in an `int`.
- **Hardware addresses.** 11 lines name a scratchpad or BIOS address (`0x1F80....`, `0xBFC.....`) in
  the shared C and headers.
- **GTE.** The shared C has about 270 `gte_*` calls, which `sdk/` gives as plain C.
- **Files with pointers.** The model and map files hold self-relative offsets (`SELF_REL`, `FILE_AT`;
  ASSETS.md), and the mode text files hold absolute addresses, which the loader will have to fix up.

## Building

It needs a 32-bit C compiler on the host, not the Docker image; CMake stops if the compiler is 64-bit.
On Windows, with [llvm-mingw](https://github.com/mstorsjo/llvm-mingw) and Ninja:

```sh
cmake -S port -B build/pc -G Ninja -DCMAKE_C_COMPILER=i686-w64-mingw32-clang
cmake --build build/pc
```

The result is `build/pc/crashbash.exe`.
