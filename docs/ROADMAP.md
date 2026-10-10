# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` left, and
everything named, typed and split into its original files. Phases are ordered
but overlap; matching continues throughout. Progress lives on the dashboard
and badges. Tick a box in the commit that finishes that item.

## Phase 1 - Foundations: types and names

Build the headers and names that every later match depends on.

- [x] Turn the leaked minigame headers (Tank, Pogo, Arkenoid, Polar) into
  `include/bash/` headers: structs, enums and prototypes. Commit only the names
  and types, never the leaked text.
- [x] Name the AST Init/Plot pairs that the leaked enums prove (Pogo,
  Arkenoid, Polar).
- [x] Name the functions the named AST pairs reach, using the prototypes in
  `include/bash/`.
- [x] Recover the core engine layouts: `NAS`, `EXTnode`, `EXTengine` and
  `SLONG_XYZ` (sizes and proven fields).
- [ ] Recover the layouts of the remaining engine types the code uses
  (`EXTobject`, `EXTmotion`, `EXTbotai`, `EXTheromove`, `NASTrigger`,
  `subengine`, `hero_seq`, `TA_player`) and of the player/hero struct indexed
  from `D_8009D52C`, and settle the size of `bool`. Naming `unkXX` fields is
  ongoing work in every phase, not part of this item.
- [ ] Name the core engine functions every match calls (spawn by AST id,
  extension and engine allocation, extension lookup, random, sound, angle, and
  the most-called exe and engine functions), each with code evidence.
- [ ] Name the core globals: the AST table, the NAS lists, the player/hero
  array, and the globals the leaked headers declare (`BombMaster`, `Walrus`,
  `HeadPolarObject`, `PogoPlay`, ...).
- [ ] Include Sony's PsyQ headers in the game sources, so library calls get
  real prototypes.
- [x] Retire `include/externs.h` and `include/prototypes.h` into owned headers.
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
- [ ] Name Tank's AST pairs. Its enum is cut off in the leak, so match each
  table entry to its `Init`/`Plot` prototype from evidence in the code (calls,
  spawned objects, assets).
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
