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
  sdk/                PC versions of PsyQ's functions, under the same names
    include/          libgpu.h, libgte.h, libsnd.h, libcd.h, libpad.h, ...
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
5. **`#ifdef BASH_PC` is the last resort** in shared code. Two cases are known: the three `gte_*` macros
   in `include/bash/main.h` (inline asm) and `draw_verts`' register pins (`src/main/prims.c`).
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

## What's known so far

- **The overlays can link into one program.** On the PS1 they share addresses, but their 4,368 global
  names have one clash: `XZBoxToSphere`, defined in Polar, Race and Dino (from the built ELFs,
  2026-10-30). So the PC build doesn't need to load overlays at runtime; `LoadModeOverlay` needs only
  the mode's text.
- **Addresses kept in integers.** The shared C has about 430 casts of an address to an integer
  (`(s32)&...`), and the data has more. Open decision for 6.1: a 32-bit build first keeps them all
  working, and 64-bit follows as a later step.
- **Hardware addresses.** 11 lines name a scratchpad or BIOS address (`0x1F80....`, `0xBFC.....`) in
  the shared C and headers.
- **GTE.** The shared C has about 270 `gte_*` calls, which `sdk/` gives as plain C.
- **Files with pointers.** The model and map files hold self-relative offsets (`SELF_REL`, `FILE_AT`;
  ASSETS.md), and the mode text files hold absolute addresses, which the loader will have to fix up.

## Building

```sh
cmake -S port -B build/pc
cmake --build build/pc
```

Today that builds only the entry point. `-DBASH_BUILD_GAME=ON` adds the game's C, which starts to
compile with 6.1.
