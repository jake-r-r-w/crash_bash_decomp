# The PC port

A native 64-bit PC port of Crash Bash, built from this source on the best tech stack for a game that plays: no
emulation and no hacks. It builds but doesn't run the game yet: this file is the design, and the steps are in
[ROADMAP-B.md](ROADMAP-B.md), Part 6.

A first attempt ran the game as a 32-bit program on the PS1's memory layout, with a software PS1 GPU. It was
deleted on 2026-10-30, and this plan starts again without any of that. Its code is in git
at `7115b3b8`, as a reference only. What it taught is at the end of this file.

## The rules

1. **The port has its own copy of the game's C** (decided 2026-10-30): `port/game/`, taken once from `src/main/`,
   `src/overlays/` and `include/`. It doesn't have to match anything, so it can be refactored freely.
2. **The decomp is left alone.** `src/`, `include/` and the Makefile don't change for the port, and `make check`
   stays the proof that the source is the game. The Makefile never sees `port/`.
3. **Everything PC-only goes in `port/`**, and the port never builds from `src/` or `include/`. A fix found in one
   copy goes to the other by hand, if it applies.
4. **The port is tested by playing it.** Disc to disc is byte matching; this isn't. The user plays it and says what's wrong.
5. **No Sony code in `port/`.** PsyQ's names go as the code is cleaned up, in favour of plain functions.
6. **No emulation and no hacks.** No PS1 GPU, VRAM or SPU model, no fixed low addresses, no linker lists that
   recreate the PS1's layout, no crash handlers that patch up bad accesses, no `-O0` to hide undefined
   behaviour. If the C relies on the PS1, the C gets fixed.

## Tooling

| What | Choice |
|---|---|
| Language | C. The game's C has to stay C to match, and `port/` stays with it |
| Build | CMake with presets, and Ninja |
| Compilers | Clang on both systems, GCC as a second check on Linux |
| Window, sound, input | SDL3 |
| Drawing | SDL3's GPU API: Direct3D 12 on Windows, with Vulkan to come for Linux |
| Checking | CTest, ASan and UBSan builds, optimised release builds |

There are no GitHub workflows: all of this runs by hand, on a machine with the game's data. Playing the
game in tests needs that data, which can't go on a public server.

## The design

**The copy.** `port/game/` starts as the decomp's C and gets cleaned up: code written only to steer the old compiler goes
back to natural C, K&R calls get prototypes, and `src/ps1/` stays out. The names and types recovered in the decomp
stay.

**Addresses in integers.** The game keeps addresses in `s32`s (about 430 casts, and more in data tables). They become
real pointers. The compiler's truncation warnings are the to-do list, and they end up as errors.

**Real structs.** The game reads some globals through others: `GameState` over `GameInfo` and the names after
it, arrays that run on into the next object, about 400 casts to byte pointers. These only work on the PS1's
layout, and they stop an optimiser being safe, so each becomes a real struct, array or field.

**Files aren't memory.** The game lays structs straight over the files it loads, and even carves heap block headers out of a
loaded file's bytes, so those headers stay 0x18 bytes and a file's 4-byte pointer slots stay 4 bytes. Self-relative offsets
(`SELF_REL`, `FILE_AT`) work anywhere. A slot that needs a pointer is a `ref32`, an offset from the heap arena: the arena is
static, so every heap and static address the game holds is within 2 GB of it. Mode text's absolute addresses are translated
when it loads (`modetext.c`), into real records and pointer tables. The memory card keeps its format on disk: `GameState` has no
pointers, and a compile-time check holds it at the PS1's 0x18C bytes.

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
saying so. With real prototypes, these calls just pass the argument.

**The game's maths.** The fixed-point maths in the game's logic stays, because it decides how the game plays:
camera, collision and sound positions come out the same. It becomes plain C functions, with no GTE registers,
and uses the same integer rules as the PS1.

**Drawing.** The port doesn't write PS1 drawing commands. The functions that do (`draw_object_model`, `draw_verts`
and the `prim_*` writers, the map, sprite and text drawing) are rewritten to send vertices to the
GPU with real 3D transforms, depth and perspective-correct textures. Textures go from the game's indexed files to
GPU textures at load, and palettes and animated or scrolling textures use GPU resources, not a VRAM allocator. Blend
modes, fades and draw order are kept, so it looks like the game, sharper. The few effects that draw into VRAM
become render targets.

**Sound.** The sound banks and music are game assets, so they're played directly: ADPCM samples decoded by a native
mixer, and the SEQ music by a native sequencer. To check it, Sony's linked code runs in a small MIPS interpreter against
the same inputs, and note timing, pitch and volume have to agree.

**Keeping it the same game.** The user plays each stage next to retail and says what's wrong. All 33 modes also play to the
end by themselves on every build, optimised and under ASan and UBSan, which catches crashes and bad memory but not a
game that plays differently.

## Building

It needs CMake, Ninja, a 64-bit Clang and SDL3. On Windows that's [llvm-mingw](https://github.com/mstorsjo/llvm-mingw).
From `port/`, with `CMAKE_PREFIX_PATH` pointing at SDL3 (or `-DBASH_FETCH_SDL3=ON` to download and build it):

```sh
cmake --preset windows-clang
cmake --build --preset windows-clang
ctest --preset windows-clang
```

The presets are `windows-clang`, `linux-clang`, `linux-gcc` and `linux-asan`, and the output is under `build/port/`.
`port/` holds `game/` (the copy of the game's C), `psyq/` (PsyQ's names, which go as they're replaced) and `main.c`.

The game's C isn't in the default build until it compiles. To see what's left:

```sh
cmake --build --preset windows-clang --target game -- -k 0
```

## Running it

Run `build/port/windows-clang/crashbash.exe` from the repo's root, where it finds the game's data in `build/disc/`. A settings
menu opens in the game's window first (`platform/launcher.c`), and what's chosen is kept in the user's folder (`platform/settings.c`).
Keys: arrows, Z = Cross, X = Circle, A = Square, S = Triangle, Enter = Start, Q and W = L1 and R1, 1 and 2 = L2 and R2.

For testing, any run with `--frames N` ignores the saved settings and skips the menu:

| Option | What it does |
|---|---|
| `--frames N` | stop after N vblanks |
| `--skip-intro`, `--skip-loading` | the launcher's two skips, for this run |
| `--fps 30\|60\|120` | the frame rate, for this run |
| `--shot N FILE` | save vblank N as a PNG |
| `--press N BUTTONS` | hold pad 1's buttons (hex, `platform.h`) from vblank N for a moment |
| `--no-launcher` | start with the saved settings, without the menu |
| `--launcher-keys KEYS` | drive the menu from inside the program: u, d, l, r, e for enter |
| `--launcher-shot FILE` | save a picture of the menu and exit |
| `--no-window`, `--data`, `--card` | no window, and where the game's data and memory card are |

**Frame rates.** The game's logic runs at about 30 frames a second (2 vblanks a frame) and stays there, because its movement is per frame. At 60 and 120 the renderer keeps the game's last two frames and draws steps between them, so what's shown is one game frame behind. Triangles are matched by name: `draw_owner` says which object is drawing, and a face number or the order within that object does the rest (`gfx_mark`). What has no owner, like some 2D, just appears. A triangle that jumps more than a quarter of the screen isn't moved.

**Depth.** The game sorts by bucket, as a PS1 must. Solid parts of `draw_object`'s models also test real depth against each other, so a moving model's faces don't swap. The map, effects and anything semi-transparent or with a layer of its own stay in the game's order alone.

## What a pointer's size breaks

Giving a struct a real pointer makes it bigger, so anything that assumed the old size is now wrong, and the compiler can't
always see it. `node tools/port/views.mjs` finds every struct read as another by Clang's AST and checks it against the layouts
at 32 and 64 bits. Most of what it still reports is casts back to the real type, mistyped locals, or code that 6.4 and 6.6 replace. Two aren't proven: `script_control_info` and `HubSparkSet` lie over the end of a `subengine`, so check them when they first run.

- Byte strides: `* 0x38` for a model bank entry, `<< 4` for a `BattleLevel`, offsets like `[0x6C / 4]`. Use the array or field.
- Views: a pad written `u8 pad0[0x74]` is the PS1's. Write it `u8 pad0[offsetof(subengine, entity)]`, which holds at any size.
- Word views of a struct, such as `st[0x12]` and `hero[40]`. Use the field, or `EXT_W(ext, i)` for an extension of plain words.
- Fixed sizes: `AllocExtension` and `AddExt` now take `sizeof`, and its heap aligns to 8 and has a header of `EXT_HEADER` bytes. The
  sizes the game asks for its heaps are still the PS1's, so 6.4 has to scale them.
- Tables split across two labels: the PS1 read on from one to the next (`DefaultArenaDef`, `TitlesBackdropAtable`). Make them one array.
- Words that were next to each other: `ScreenLabelState` and the four after it are one array now. `p[1]` from a single global reads whatever Clang put next.
- Views over an `EXTobject` with `sizeof(EXT) + 8`: its two pointers count for more now. Name the field with `offsetof`.
- Hooks: a function stored in `subengine.hook` takes a view of the subengine, which the view check can't see through the pointer (`PosDst`, `TextNAS`, `ModelObj`).
- A view indexed as an array has to be the struct's size: `PanelPrim` has a size check now.
- A slot that holds a number in some uses and an address in others is an `iptr` (`NAS.UserInfo`). Otherwise it gets its
  real pointer type.

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
