# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` left, and
everything named, typed and split into its original files. The game's assets
(textures, sounds, models, levels) extract to editable formats and rebuild byte
for byte into a playable disc. Assets stay on the user's machine and never go
in git; only the tools and format documentation do. Phases are ordered but
overlap; matching continues throughout. Progress lives on the dashboard and
badges. Tick a box in the commit that finishes that item.

Preparation goes in the phase that first needs it. When later work turns up
something new to do, add it to the phase it blocks, not to an earlier one.

## Phase 1 - Foundations: types and names

Build the names and types that matching `main.exe` depends on.

**Done when** matching `main.exe` functions no longer stalls on missing names
or types.

- [x] Turn the leaked minigame headers (Tank, Pogo, Arkenoid, Polar) into
  `include/bash/` headers: structs, enums and prototypes. Commit only the names
  and types, never the leaked text.
- [x] Name the AST Init/Plot pairs that the leaked enums prove (Pogo,
  Arkenoid, Polar).
- [x] Name the functions the named AST pairs reach, using the prototypes in
  `include/bash/`.
- [x] Recover the core engine layouts: `NAS`, `EXTnode`, `EXTengine` and
  `SLONG_XYZ` (sizes and proven fields).
- [x] Retire `include/externs.h` and `include/prototypes.h` into owned headers.
- [ ] Name the core engine functions every match calls (spawn by AST id,
  extension and engine allocation, extension lookup, random, sound, angle, and
  the most-called exe and engine functions), each with code evidence.
- [ ] Name the core globals: the AST table, the NAS lists and the player/hero
  array.
- [ ] Include Sony's PsyQ headers in the game sources, so library calls get
  real prototypes.
- [ ] Recover the layouts of the engine types `main.exe`'s code uses (from
  `EXTobject`, `EXTmotion`, `NASTrigger`, `subengine`, `hero_seq`, ...) and the
  player/hero struct indexed from `D_8009D52C`, and settle the size of `bool`.
  Naming `unkXX` fields is ongoing work in every phase, not part of this item.

## Phase 2 - `main.exe` to source-only

**Done when** the exe's game code has no `INCLUDE_ASM` or `INCLUDE_RODATA`
left, it's split into its translation units, its data is C, every function and
global is named and typed, and `build/main.exe` still matches. The library
code is Sony's objects plus `2MBYTE.OBJ`. `make disc` rebuilds a retail-identical
disc image, and the asset formats the exe loads round-trip.

- [ ] Match every game function in the executable.
- [ ] Split `main.c` into its original translation units.
- [ ] Turn its rodata, data and bss into C.
- [ ] Close the PsyQ gaps. Sony's hand-written `2MBYTE.OBJ` is the one documented
  exception.
- [ ] `make disc`: pack the built exe and overlays, plus the user's extracted
  files, into `CRASHBSH.DAT` and a bin/cue byte-identical to retail.
- [ ] Document the asset formats the exe loads (the `CRASHBSH.DAT` index, file
  groups, textures, sound banks). Write extractors to `assets/` and rebuilders
  that reproduce the original bytes.

## Phase 3 - The engine overlay

Its types are the vocabulary the minigames are written in.

**Done when** `src/overlays/engine/` has no `INCLUDE_ASM` or `INCLUDE_RODATA`,
it's split into its translation units, its data is C, everything is named and
typed, `build/engine.bin` still matches, and the formats the engine loads
round-trip.

- [ ] Match every function in `engine`.
- [ ] Split it into its original translation units.
- [ ] Turn its data and jump-table rodata into C.
- [ ] Document the asset formats the engine loads (models, animation, level
  data, ...), with extractors and byte-identical rebuilders.

## Phase 4 - The leaked minigames

Their functions start out named and typed. Finish one overlay completely
before starting the next.

**Done when** Tank, Pogo, Arkenoid and Polar (`ovl5f`, `ovl60`, `ovl61`,
`ovl62`) are source-only and still match. Every declaration in their
`include/bash/` headers is either defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build. Each of their asset files
round-trips.

- [ ] Finish the slot overlays' section splits (data vs rodata), so functions
  using strings and jump tables can match.
- [ ] Recover the minigame-only types (`EXTbotai`, `EXTheromove`, `TA_player`,
  ...) as each minigame needs them.
- [ ] Round-trip each minigame's own asset files (its `file_group_*` groups) as
  its overlay is finished.
- [ ] Pogo, including the globals its header declares (`PogoPlay`, ...).
- [ ] Arkenoid
- [ ] Polar, including the globals its header declares (`BombMaster`,
  `Walrus`, `HeadPolarObject`, ...).
- [ ] Name Tank's AST pairs. Its enum is cut off in the leak, so match each
  table entry to its `Init`/`Plot` prototype from evidence in the code (calls,
  spawned objects, assets).
- [ ] Tank

## Phase 5 - The remaining overlays

One at a time, starting with code shared or near-identical between minigames.

**Done when** every slot overlay is identified in `docs/ARCHITECTURE.md`, is
source-only, and still matches, and its asset files round-trip.

- [ ] Identify which minigame or mode each remaining overlay is (the asset
  files it loads are the best clue), then list them here, one box each.
- [ ] Every remaining slot overlay is source-only.

## Phase 6 - Finish

**Done when** the matching build is byte-identical to retail with no
`INCLUDE_ASM`, `INCLUDE_RODATA`, parked functions or `func_`/`D_` placeholder
names anywhere. Every file on the disc round-trips through documented
extractors and rebuilders. `make disc` from the extracted assets gives a
retail-identical image, and a `NON_MATCHING` build with shifted code boots and
plays in an emulator.

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere, and no parked functions.
- [ ] Every remaining disc file (music, movies, anything not covered above)
  round-trips, and the formats are documented.
- [ ] A shiftable `NON_MATCHING` build that runs in an emulator.
- [ ] Every file at quality level 4 ([CONVENTIONS.md](CONVENTIONS.md)), with no
  placeholder names.
- [ ] `docs/ARCHITECTURE.md` describes the game's systems.
