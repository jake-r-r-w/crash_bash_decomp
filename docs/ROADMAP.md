# Roadmap

The end state is a **source-only Crash Bash**. Every function in the executable
and the 15 overlays is C that rebuilds the retail bytes. Data is C. Nothing is
`INCLUDE_ASM`. Everything is named, typed and split into its original files.
PsyQ comes from Sony's objects. The quality levels are defined in
[CONVENTIONS.md](CONVENTIONS.md), and the per-function loop is in
[MATCHING.md](MATCHING.md).

Phases are ordered, but they overlap: matching continues in every phase. A
phase is done when its exit criteria hold. Live numbers are on the dashboard
(`docs/index.html`). The baseline below is a snapshot.

## Baseline (2026-10-10)

| Binary | Functions | Code bytes | Matched (bytes) | Known game |
|---|---|---|---|---|
| `main.exe` (game part) | 574 | 124,840 | 6.9% | boot, main loop, core |
| `engine` | 357 | 129,264 | 2.0% | shared engine module |
| `ovl62` | 226 | 130,512 | 0.4% | Polar (leaked headers) |
| `ovl5f` | 183 | 86,248 | 1.0% | probably Tank (leaked headers) |
| `ovl60` | 180 | 65,808 | 1.1% | Pogo (leaked headers) |
| `ovl61` | 97 | 55,000 | 0.5% | Arkenoid (leaked headers) |
| 10 other slot overlays | 933 | 427,364 | 0.4-4.4% | not identified yet |

- **Game code:** 2,550 functions, about 1 MB in total. 393 are matched (1.9%).
  Nothing is named yet, and there are no game headers yet (`include/bash/` doesn't exist).
- **Unmatched sizes:** 270 are ≤64 bytes, 896 are ≤256, 872 are ≤1 KB, and 216
  are larger than 1 KB (the largest is 9 KB, `ovl62`).
- **Duplicates:** 114 groups of identical functions (308 functions, 42 KB
  beyond the first of each). One match fills a whole group. There are also 88
  near-identical groups.
- **PsyQ:** 99.3% is linked from Sony objects. Left: 16 unidentified gap files
  (29 functions, 832 bytes) and `2MBYTE.OBJ` (196 bytes of asm).
- **Data:** still raw bytes in every binary.

## Phase 1 - Foundations: types and names

Hand matching stalls without types: most failed drafts failed on missing types
or didn't compile. Build the headers first, so every later match is a typed
one.

1. **Leaked headers into `include/bash/`.** Turn the 40 KB leak (Tank, Pogo,
   Arkenoid, Polar and `alldbase.h`) into proper headers: structs, enums, and
   prototypes in their original order. Only names and types go in the repo,
   never the leaked text itself.
2. **Name what the leak and the tables prove.**
   - Use the AST table (`{id, Init, Plot}` at `0x80098720`) to name each
     minigame's Init/Plot pairs.
   - Use prototype order and call patterns to name the functions they reach.
   - Make all renames through `decomp.py rename`.
3. **Engine core types.** The leak uses the engine's types (`NAS`,
   `EXTengine`, `NASTrigger`, ...) but doesn't define them. Recover their
   layouts from field accesses in matched code, and put them in one header.
4. **Retire `include/externs.h` and `include/prototypes.h`.** Move their
   declarations into owned headers as their types become known.
5. **Overlay layout cleanup.** In each slot overlay, split `.data` from the
   trailing rodata, so that functions using strings and jump tables can match.
   Fix `func_800C8BA0` (ovl5f), whose literal lands outside its function.
6. **Identify the 10 unknown overlays** (which minigame or mode each one is):
   use the AST ids, strings, and the asset files they load. Record it in
   `docs/ARCHITECTURE.md`.

**Exit:**
- `include/bash/` holds the minigame and engine core types.
- The leaked functions and every AST Init/Plot are named.
- `externs.h` and `prototypes.h` are empty or gone.
- Every overlay is identified.

## Phase 2 - `main.exe` to source-only

This is the smallest code base, and it has the most callers. Its names and
types spread into everything else.

1. Match every game function in `main` (dashboard order: smallest first,
   duplicates early).
2. Split `src/main/main.c` into translation units: `src/main/<subsystem>/<file>.c`,
   placed using alignment padding, `.data`/`.bss` order and strings.
3. Turn `main`'s `.rodata`, `.data` and `.bss` into C next to the code that
   owns them, so its `INCLUDE_RODATA` lines are gone.
4. PsyQ remainder:
   - Identify the 16 gap files against other PsyQ versions or libraries.
   - Decompile the rest.
   - Leave `2MBYTE.OBJ` as Sony's hand-written asm, the one documented exception.

**Exit:** `main.exe` has no `INCLUDE_ASM` and no raw data blobs, all its
functions are named and typed, and it still matches.

## Phase 3 - The engine overlay

The engine is shared by every minigame, so its types are the vocabulary of the
overlays. Same steps as phase 2: match everything, type it, split it into
files, and turn its data and jump-table rodata into C.

**Exit:** `engine.bin` is source-only.

## Phase 4 - The leaked minigames

Do Pogo (`ovl60`), Arkenoid (`ovl61`), Polar (`ovl62`) and Tank (`ovl5f`)
first. Their prototypes and structs are already known, so every function
starts out named and typed. Finish one overlay completely before starting the
next. This phase includes the overlay's big state machines (the 5-9 KB
functions). Budget them as multi-session work, using the permuter.

**Exit:** four slot overlays are source-only.

## Phase 5 - The remaining overlays

Do the other 10 slot overlays, one at a time. Start with the near-identical
groups, because a minigame's code is often a variant of another's. Names come
from behaviour, strings, and the patterns established in phase 4.

**Exit:** all 15 overlays are source-only.

## Phase 6 - Finish

1. **No assembly left:** zero `INCLUDE_ASM` and `INCLUDE_RODATA` across the
   repo, and every parked function is matched.
2. **Shiftable build:** a `NON_MATCHING=1` build in which code and data can
   move (no hard-coded addresses, linker scripts driven by symbols), checked by
   running it in an emulator.
3. **Quality bar:** every file at level 4 in [CONVENTIONS.md](CONVENTIONS.md),
   and no `func_`/`D_` placeholder names.
4. **Documentation:** `docs/ARCHITECTURE.md` describes the game's systems, not
   just its memory map.

## Running the project

- **Throughput:** one function per commit through `decomp.py done`. Use the
  `/match-function` skill for AI sessions. Keep the `AGENT_LOG.md` entry per
  session.
- **Typing a matched function** (no new match, just better types and names)
  is an ordinary commit after `make verify`. It counts toward the phase exits
  as much as new matches do.
- **Parked functions** (`decomp.py block`) are revisited at the end of each
  phase, when more types are known.
- **Dashboard tracking:** add *named* and *typed* counts per binary to
  `docs/index.html`, so phase exits can be read off the dashboard. Today it
  tracks matched bytes and named functions.
- Update this roadmap when a phase exits or the plan changes.
