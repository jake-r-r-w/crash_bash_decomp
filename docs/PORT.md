# The PC port

A native 64-bit PC port of Crash Bash for Windows and Linux, built from this source on a modern stack: no
emulation and no hacks. Nothing is written yet: this file is the design, and the steps are in
[ROADMAP-B.md](ROADMAP-B.md), Part 6.

A first attempt ran the game as a 32-bit program on the PS1's memory layout, with a software PS1 GPU. It was
deleted on 2026-10-30, and this plan starts again without any of that. Its code is in git
at `7115b3b8`, as a reference only. What it taught is at the end of this file.

## The rules

1. **`src/` and `include/` always match retail.** Any change there has to pass `tools/dk make check`.
2. **Everything PC-only goes in `port/`.** The Makefile compiles only what's in `src/` and `asm/`, so
   nothing in `port/` reaches the matching build.
3. **Dependencies go one way.** `port/` can include the game's headers. The game's code never includes
   anything from `port/`.
4. **`src/ps1/` is PS1 only**, and the PC build leaves it out: the exe header, `bios_1`'s data, the mod-chip
   check, the overlay id words.
5. **No Sony code in `port/`.** Where game logic calls a PsyQ name for something ordinary (a pad, a file, a
   sound), the PC build gives it a native function under that name.
6. **No emulation and no hacks.** No PS1 GPU, VRAM or SPU model, no fixed low addresses, no linker lists that
   recreate the PS1's layout, no crash handlers that patch up bad accesses, no `-O0` to hide undefined
   behaviour. If the C relies on the PS1, the C gets fixed, in a form that still matches.
7. **`#ifdef BASH_PC` is the last resort** in shared code. A type or macro that means the same thing on
   both builds comes first.

## Tooling

| What | Choice |
|---|---|
| Language | C. The game's C has to stay C to match, and `port/` stays with it |
| Build | CMake with presets, and Ninja |
| Compilers | Clang on both systems, GCC as a second check on Linux |
| Window, sound, input | SDL3 |
| Drawing | SDL3's GPU API: Vulkan on Windows and Linux, Direct3D 12 on Windows |
| Checking | CTest, ASan and UBSan builds, optimised release builds, CI on both systems |

CI can only compile: the game's data and Sony's libraries can't be on a public server. Playing the
game in tests happens on a machine that has them.

## The design

**Addresses in integers.** The game keeps addresses in `s32`s (about 430 casts, and more in data tables).
They get a type of their own that is `s32` on the PS1 and pointer-sized on PC. The compiler's truncation
warnings are the to-do list, and they end up as errors.

**Real structs.** The game reads some globals through others: `GameState` over `GameInfo` and the names after
it, arrays that run on into the next object, byte offsets into structs. These only work on the PS1's
layout, and they stop an optimiser being safe, so each becomes a real struct, array or field. Structs that
hold pointers change size on PC, so their size checks apply to the PS1 only.

**Files aren't memory.** The game lays structs straight over the files it loads. Self-relative offsets
(`SELF_REL`, `FILE_AT`) work anywhere, but the 32-bit slots the game fills with pointers can't hold a 64-bit
one. Those go in a side table or a translation step at load. Mode text holds absolute addresses, which
are fixed up at load. The memory card keeps its format on disk, so saves stay compatible.

**What touches the hardware is small.** Of about 116,000 lines of game C, the drawing commands, VRAM calls and sound
calls come to a few hundred lines. 171 of the 269 GTE calls and most of the primitive code are in one file,
`src/main/prims.c`. The game's logic never sees the hardware, so the port replaces that layer, and the game above
it is unchanged.

**Overlays.** They all link into the program. A loaded overlay's data has to go back to its starting values,
so each overlay's data goes in a section of its own, saved at startup and restored on load. Their global
names have one clash, `XZBoxToSphere`.

**Null pointers.** The PS1 has RAM at address 0, so the game's null reads and writes just work there. Each one
gets a real check, found by running under the sanitizers.

**Calls that rely on the caller's registers.** On the PS1, `f()` can hand on the caller's own `a0` without
saying so. These calls name the argument on PC only, with no byte changed on the PS1.

**The game's maths.** The fixed-point maths in the game's logic stays, because it decides how the game plays:
camera, collision and sound positions come out the same. It becomes plain C functions, with no GTE registers.

**Drawing.** The PC build doesn't write PS1 drawing commands. The functions that do (`draw_object_model`, `draw_verts`
and the `prim_*` writers, the map, sprite and text drawing) have native versions in `port/` that send vertices to the
GPU with real 3D transforms, depth and perspective-correct textures. Textures go from the game's indexed files to
GPU textures at load, and palettes and animated or scrolling textures use GPU resources, not a VRAM allocator. Blend
modes, fades and draw order are kept, so it looks like the game, sharper. The few effects that draw into VRAM
become render targets.

**Sound.** The sound banks and music are game assets, so they're played directly: ADPCM samples decoded by a native
mixer, and the SEQ music by a native sequencer. To check it, Sony's linked code runs in a small MIPS interpreter against
the same inputs, and note timing, pitch and volume have to agree.

**Keeping it the same game.** The same pad inputs go to retail in an emulator and to the PC build, and the game's
state is compared frame by frame. The picture is meant to differ; how the game plays isn't.

## What the first attempt taught

Each of these cost time to find, and they all still apply:

- **Clang keeps data in first-use order, not source order**, and has no `-fno-toplevel-reorder`. Anything that
  relies on neighbouring globals breaks, which is why the real structs come first.
- **An optimiser believes a global's declared size.** `GameInfo` is declared as 4 bytes, and Clang deleted a write to
  it because a later write further on "covered" the object. Dead-store elimination is enough to do it.
- **Pass-through arguments are everywhere.** 44 direct calls, plus calls through function pointers:
  `HeroActionTable[].fn`, `HeroHooks.HeroSetIdle`, `UpdateShadow`, `HeroFacing`, `HeroKickCollision`, Tank's
  `SetMoeJoe*`.
- **Known null accesses.** `SetHeroIdle` and the games' own versions (`SetTankHeroIdle`, `SetHeroPogoIdle`,
  `SetDinoHeroIdle`, `SetChaseHeroIdle`, `SetNoxHeroFly`), which run before a hero has `hero_move`;
  `HubPreMoveHero`; `InitPolarBomb`; `InitBashHomer` and `InitLoadingScreen`, past the last subengine; the debug
  font's `ScreenRect` at boot.
- **Known overruns.** `PolarAIFlags` is 7 words, not 4. `VramAreas` and `HeroSide` are labels inside larger arrays.
  `LevelUnused` is defined in both the exe and the engine. `N2OCameraFollow` redeclares its parameter.
  `N2OSpawnFirstMissile` spawns at whatever is on the stack.
- **The game writes to a `const` string**: the completion figures, into `CardSaveTitleFormat`.
- **`SsInit` has to close every sound bank**: the game never closes its music banks.
- **The tables can be computed**, exactly as a retail exe has them: `rcossin_tbl` is `round(4096 sin)` and `round(4096
  cos)` over 4096 steps; `VectorNormal`'s is `floor(4096 sqrt(64 / (i + 64)))` for 192 entries; libsnd's pitch tables
  are `floor(0x8000 * 2^(i / 12))` and `floor(0x8000 * 2^(i / 1536))`.
- **GTE opcodes decide `sf`.** `VectorNormal`'s GPF is `0x190003D`, with `sf` clear, so it doesn't shift by 12.
  Getting this wrong made every model posed by a script key vanish.
- **libsnd.** A tick's timing, volumes and voice allocation were read from the linked code and can be read again:
  `_SsSeqPlay`, `_SsGetSeqData`, `_SsVmKeyOn`, `_SsVmKeyOnNow`, `_SsVmSetVol`, `_SsVmAlloc`, `SsPitchFromNote`.
  The music uses notes, program changes, pitch bends, controllers 6, 7, 10, 98 and 99, and the end marker.
  Reverb depth goes to 0 at every music load, so it's only heard until the first one.
- **Unsolved:** the sound wandered between slow and fast in a real window. The vblanks were steady and nothing was
  dropped, so the cause wasn't found. The new sound step has to check for this.
