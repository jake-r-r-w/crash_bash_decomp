# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` left, and
everything named, typed and split into its original files. The game's assets
(textures, sounds, models, levels) are not part of the source: a tool extracts
them from the user's own disc into editable formats, and their formats are
documented. Assets stay on the user's machine and never go in git; only the
tools and format documentation do. Phases are ordered but
overlap; matching continues throughout. Progress lives on the dashboard and
badges. Tick a box in the commit that finishes that item, and run
`tools/dk make progress` in it so the dashboard and roadmap badge update.

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
- [x] Name the core engine functions every match calls (spawn by AST id,
  extension and engine allocation, extension lookup, random, sound, angle, and
  the most-called exe and engine functions), each with code evidence.
- [x] Name the core globals: the AST table, the NAS lists and the player/hero
  array.
- [x] Include Sony's PsyQ headers in the game sources, so library calls get
  real prototypes.
- [x] Recover the layouts of the engine types `main.exe`'s code uses (from
  `EXTobject`, `EXTmotion`, `NASTrigger`, `subengine`, `hero_seq`, ...) and the
  player/hero struct indexed from `D_8009D52C`, and settle the size of `bool`.
  Naming `unkXX` fields is ongoing work in every phase, not part of this item.
  `NASTrigger` and the other minigame-only types (`EXTbotai`, `EXTheromove`,
  `TA_player`) are only used in the overlays, so they're recovered with them.

## Phase 2 - `main.exe` to source-only

**Done when** the exe's game code has no `INCLUDE_ASM` or `INCLUDE_RODATA`
left, it's split into translation units at the boundaries the binary shows,
its data is C, every function and global is named and typed, and
`build/main.exe` still matches. The library is Sony's objects, code and data,
plus `2MBYTE.OBJ`. `CRASHBSH.DAT` extracts, and so do the formats whose
loaders are in the exe, each documented.

- [x] Match GTE code: include Sony's GTE macros (`inline_c.h`, `gtemac.h`) in
  the game sources and match one GTE function through `done`. 27 of the exe's
  game functions use the GTE, and m2c can't draft them.
- [ ] Match every game function in the executable.
- [ ] Name and type every function and global in the exe (a descriptive name
  where the original is unknown, per [CONVENTIONS.md](CONVENTIONS.md)).
- [ ] Split `main.c` into translation units at the boundaries the binary shows
  (rodata and data order, alignment padding, file-local data), with the
  evidence for each in `docs/ARCHITECTURE.md`. The exe has no file names, so
  the units are named by what they contain. Do this before the data, so each
  file's local data moves with it.
- [ ] Turn its rodata, data and bss into C.
- [ ] Close the PsyQ gaps: find the object for each function in
  `src/main/psyq/unknown/` (mostly one-call wrappers such as `CdSync`) and
  link it, or match it as C; name `func_80032F3C` (`abs` and `labs` are
  identical objects); and take the objects' data from the objects instead of
  the retail data blob. Sony's hand-written `2MBYTE.OBJ` is the one documented
  exception.
- [ ] Asset extraction tool: read the `CRASHBSH.DAT` index (993 entries at
  `0x8004E110`) from the user's disc and extract its files to `assets/`, plus
  the formats whose loaders are in the exe, to editable files. Document each
  format. Formats parsed by the engine or a minigame belong to that phase.

## Phase 3 - The engine overlay

Its types are the vocabulary the minigames are written in.

**Done when** `src/overlays/engine/` has no `INCLUDE_ASM` or `INCLUDE_RODATA`,
it's split into its translation units, its data is C, everything is named and
typed, `build/engine.bin` still matches, and the formats the engine loads
extract and are documented.

- [ ] Match every function in `engine`.
- [ ] Split it into its original translation units.
- [ ] Turn its data and jump-table rodata into C.
- [ ] Extract and document the asset formats the engine loads (models,
  animation, level data, ...).

## Phase 4 - The leaked minigames

Their functions start out named and typed. Finish one overlay completely
before starting the next. A minigame's box is ticked when its overlay is
source-only, the globals its header declares are named, and its asset files
(its `file_group_*` groups) extract.

**Done when** Tank, Pogo, Arkenoid and Polar (`ovl5f`, `ovl60`, `ovl61`,
`ovl62`) are source-only and still match. Every declaration in their
`include/bash/` headers is either defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build. Each of their asset files
extracts.

- [ ] Finish the slot overlays' section splits (data vs rodata), so functions
  using strings and jump tables can match.
- [x] Recover the minigame-only types (`EXTbotai`, `EXTheromove`, `TA_player`,
  `EXTobject_TA`, `NASTrigger`, ...): done from the Oct 9 prototype's debug
  types (ARCHITECTURE.md, Prototype debug types).
- [ ] Pogo (`ovl60`)
- [ ] Arkenoid (`ovl61`)
- [ ] Polar (`ovl62`)
- [ ] Name Tank's AST pairs. Its enum is cut off in the leak, so match each
  table entry to its `Init`/`Plot` prototype from evidence in the code (calls,
  spawned objects, assets).
- [ ] Tank (`ovl5f`)

## Phase 5 - The remaining overlays

One at a time, starting with code shared or near-identical between minigames.

**Done when** every slot overlay is identified in `docs/ARCHITECTURE.md`, is
source-only, and still matches, and its asset files extract.

- [ ] Identify which minigame or mode each remaining overlay is (the asset
  files it loads are the best clue), then list them here, one box each.
- [ ] Every remaining slot overlay is source-only.

## Phase 6 - Finish

**Done when** the matching build is byte-identical to retail with no
`INCLUDE_ASM`, `INCLUDE_RODATA`, parked functions or `func_`/`D_` placeholder
names anywhere. Every file on the disc extracts with a documented format, and
a `NON_MATCHING` build with shifted code boots and plays in an emulator.

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere, and no parked functions.
- [ ] Every remaining disc file (music, movies, anything not covered above)
  extracts, and the formats are documented.
- [ ] A shiftable `NON_MATCHING` build that runs in an emulator. Running it needs
  the built exe and overlays packed into a disc image with the user's files.
- [ ] Every file at quality level 4 ([CONVENTIONS.md](CONVENTIONS.md)), with no
  placeholder names.
- [ ] `docs/ARCHITECTURE.md` describes the game's systems.

## Afterwards - decided once the source is complete

These are not phases yet. Each gets its own plan when Phase 6 is done.

- **Native PC port.** Build the C for PC, with assets loaded from the extracted
  files instead of the CD. That means replacing the PS1 hardware layers (CD,
  GPU and GTE, SPU sound, pads) with PC equivalents.
