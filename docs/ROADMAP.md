# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` left, and
everything named, typed and split into its original files. Phases are ordered
but overlap; matching continues throughout. Progress lives on the dashboard
and badges. Tick a box in the commit that finishes that item.

## Phase 1 - Foundations: types and names

Build the headers and names that every later match depends on.

- [ ] Turn the leaked minigame headers (Tank, Pogo, Arkenoid, Polar) into
  `include/bash/` headers: structs, enums and prototypes. Commit only the names
  and types, never the leaked text.
- [ ] Name what the leak and the engine's AST table prove: each minigame's
  Init/Plot pairs and the functions they reach.
- [ ] Recover the engine's core types, which the leak uses but doesn't define.
- [ ] Retire `include/externs.h` and `include/prototypes.h` into owned headers.
- [ ] Finish the overlay section splits (data vs rodata) so that functions using
  strings and jump tables can match.
- [ ] Identify which minigame or mode each overlay is.

## Phase 2 - `main.exe` to source-only

- [ ] Match every game function in the executable.
- [ ] Split `main.c` into its original translation units.
- [ ] Turn its rodata, data and bss into C.
- [ ] Close the PsyQ gaps. Sony's hand-written `2MBYTE.OBJ` is the one documented
  exception.

## Phase 3 - The engine overlay

Its types are the vocabulary the minigames are written in.

- [ ] Match every function in `engine`.
- [ ] Split it into its original translation units.
- [ ] Turn its data and jump-table rodata into C.

## Phase 4 - The leaked minigames

Their functions start out named and typed. Finish one overlay completely
before starting the next.

- [ ] Pogo
- [ ] Arkenoid
- [ ] Polar
- [ ] Tank

## Phase 5 - The remaining overlays

One at a time, starting with code shared or near-identical between minigames.

- [ ] Every remaining slot overlay is source-only (list them here once phase 1
  has identified them).

## Phase 6 - Finish

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere, and no parked functions.
- [ ] A shiftable `NON_MATCHING` build that runs in an emulator.
- [ ] Every file at quality level 4 ([CONVENTIONS.md](CONVENTIONS.md)), with no
  placeholder names.
- [ ] `docs/ARCHITECTURE.md` describes the game's systems.
