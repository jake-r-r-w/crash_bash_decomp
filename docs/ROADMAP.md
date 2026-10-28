# Roadmap

The goal is a **source-only Crash Bash**: the executable and every overlay
rebuilt byte for byte from C, data included, with no `INCLUDE_ASM` or
`INCLUDE_RODATA` left, and everything named, typed and split into its
original files. The game's assets (textures, sounds, models, levels) are not
part of the source: a tool extracts them from the user's own disc into
editable formats, and their formats are documented. Assets stay on the
user's machine and never go in git; only the tools and format documentation
do. [The final test](#the-final-test) says when it's done: a rebuilt disc,
and a shifted build that plays the same as retail. Progress lives on the
dashboard and badges. Tick a box in the commit that finishes that item, and
run `tools/dk make progress` in it so the dashboard and roadmap badge
update.

Preparation goes in the phase that first needs it. When later work turns up
something new to do, add it to the phase it blocks, not to an earlier one.

## Current focus: every slot overlay the same way

Every game function matches (2026-10-26), so what is left is what makes the
source source-only: data as C, names and types, and the proof that the code
works as code, a shifted build that still plays. Phase 3 did all of it for
one overlay, Tank, end to end, and built the tooling on the way
(finished 2026-10-26). Phase 4 now repeats it overlay by overlay with the
recipe in [OVERLAY_RECIPE.md](OVERLAY_RECIPE.md), the other leaked
minigames first, and Phase 5 does the same for the engine and `main.exe`.

Any build of ours can be played: `tools/dk make disc` packs `build/main.exe`
and the overlays into `build/CrashBash.bin`/`.cue`, which boots in an
emulator, and `make disc-check` confirms the matching build packs to the
retail image byte for byte (docs/PLAYING.md).

Matches are C only (no inline asm; the SDK's `gte_*` macros excepted), and
natural C comes first: C that only steers the compiler is used when nothing
natural matches (docs/CONVENTIONS.md, Matching rules). A rework of a matched
function (a rename, a retype, a natural form) is an ordinary commit once
`make check` is OK.

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

## Phase 2 - Matching

Every game function becomes C that compiles to the retail bytes, with the
tools that needs: the disc packer, the asset extractor and the library.

**Done when** every game function in `main.exe`, the engine and the slot
overlays matches in C only (no inline asm but the SDK's `gte_*` macros and
`draw_verts`'s original register pins), the
library is Sony's objects, `CRASHBSH.DAT` extracts, and the disc packer
rebuilds the retail image.

- [x] A disc packer: put the built exe and overlays, with the user's other
  files, back into a disc image with the retail layout (every file at its
  original sector). Packing the matching build must give the retail image
  byte for byte. It's the only way to run any build of ours in an emulator.
  `tools/dk make disc` (`tools/scripts/pack_disc.py`) and `make disc-check`
  (`configs/USA/disc.sha`).
- [x] Match GTE code: include Sony's GTE macros (`inline_c.h`, `gtemac.h`) in
  the game sources and match one GTE function through `done`. 27 of the exe's
  game functions use the GTE, and m2c can't draft them.
- [x] Split `main.c` into translation units at the boundaries the binary shows
  (rodata and data order, alignment padding, file-local data), with the
  evidence for each in `docs/ARCHITECTURE.md`. Done 2026-10-23: `main/main`
  is nine files, cut where the evidence settles it (`decomp.py units`, the
  data blocks, what the functions on either side do; `split_unit.py` does a
  split end to end). No leak names the exe's files and the code evidence is
  used up (ARCHITECTURE.md, Files inside `main/main`).
- [x] Close the PsyQ gaps: every library function is linked from Sony's 4.7
  objects (281, plus the hand-written `2MBYTE.OBJ`), with their data. The one
  exception is `bios_1.o`'s `.data`, kept from the exe because the game's
  libcd differs from 4.7 in one initial value
  (`configs/USA/psyq/data_keep.txt`, ARCHITECTURE.md, PsyQ).
- [x] Asset extraction tool: `tools/scripts/dat.py` splits `CRASHBSH.DAT`
  into `assets/CRASHBSH/` and `make disc` builds the DAT from there
  (2026-10-16). Every entry but the code overlays is a decoded folder:
  texture sets to PNG (`tex.py`), models to OBJ and glTF with their
  animations (`mdl.py`, `gltf.py`), title screens (`tga.py`), text
  (`txt.py`), sounds to WAV, VAB, SEQ and MIDI (`sfx.py`), each documented in
  docs/ASSETS.md, which lists what is still bytes.
- [x] Recover the minigame-only types (`EXTbotai`, `EXTheromove`, `TA_player`,
  `EXTobject_TA`, `NASTrigger`, ...) from the Oct 9 prototype's debug types
  (ARCHITECTURE.md, Prototype debug types), and identify which minigame or
  mode each slot overlay is (its AST enums; `titles`'s strings make it the
  front end).
- [x] Name Tank's AST pairs: the Oct 9 prototype's `TANK_AST_types` gives
  every id, and all 68 names match the leak's `Init`/`Plot` prototypes.
- [x] Match every game function in the executable (2026-10-23).
- [x] Match every function in `engine` (2026-10-23; the pause menu,
  `func_8007F314`, last, from its earlier compiles on the prototype discs:
  PROTOTYPES.md, Earlier compiles of a function).
- [x] Match every function in the slot overlays (2026-10-26; the last three
  with cc1 analysis variants, `tools/cc1/`).
- [x] Only C everywhere, with one recorded exception: `draw_verts`
  (`func_800193A8`, `src/main/prims.c`) keeps its three register pins
  (`register s32 xy asm("$20")`, ...). They are the original's register
  variables: no pin-free C can give the target's `$18`-`$20` with `$16`/`$17`
  unused (2026-10-26; CONVENTIONS.md, Matching rules).

## Phase 3 - The Tank pilot

Take one slot overlay, Tank, all the way to source-only and
shift-tested, and build the tools for it so the other overlays only repeat
the recipe. A slot overlay is the first unit that can move: the only
pointers into one from outside are the engine's AST table entries
(`Init`/`Plot`), so once those are symbols the engine relinks against the
moved overlay without moving itself. `main.exe` and the engine can't move
until every overlay linking to them is source (Phase 6).

**Done when** `tank` has no `INCLUDE_RODATA` and no data asm, every
function and global in it is named and typed, every declaration in
`include/bash/tank.h` is defined by matched code or noted in
`docs/ARCHITECTURE.md` as not in this build, `build/tank.bin` still
matches, its asset files extract, and it passes its shift test: padded so
everything in it moves, it boots and plays through to a win the same way
retail does. The recipe is written up for Phase 4.

- [x] Name and type every Tank function and global (a descriptive name
  where the original is unknown, per [CONVENTIONS.md](CONVENTIONS.md)),
  from the leaked `tank.h`, the Oct 9 debug types and code evidence. Settle
  each `tank.h` declaration: defined by matched code, or noted as not in
  this build. First, because the data's types come from the code that reads
  it: naming a function shows what each table it reads is, and names those
  globals too. Done 2026-10-26: every function and global (the seven bss
  words nothing reads on any disc are `TankUnused*`). The leak's
  data is defined: `TANK_PREMAP` (with its `WWInf`/`TrigLookUp` pieces),
  `tank_shell_info`, the five `file_group_TANK*` and the 72
  `*_seq_tank*` hero sequences; `shell_setup` and `file_group_load_info`
  are complete types now. `TANKSpecifics` is not in Tank's data.
- [x] Split Tank into its files where the evidence settles it, after the
  naming and before its data goes to C. Done 2026-10-26: Tank is one file.
  `decomp.py units --data` now reads the slot overlays (its initialised data
  ties functions 1..123), and `cc1plus`'s emission order settles the rest:
  the out-of-line inline functions close a file in reverse definition
  order, and against the leak's order Tank has one such tail, 87..180
  (ARCHITECTURE.md, Files inside the slot overlays).
- [x] Data to C tooling, once the names and types say what the tables are:
  turn a splat rodata or data block into C
  initialisers (strings, tables, structs from the headers, pointers as
  symbols, never as numbers) that rebuild byte for byte, in the unit's
  section order (`tank`'s is rodata, text, data). Generic, so every
  overlay can use it. Done 2026-10-26: `tools/scripts/data2c.py`
  (CONVENTIONS.md, Data as C).
- [x] Tank's rodata as C: the id word (`tank_id`) and the `INCLUDE_RODATA`
  of `D_800B32B8` (the animation name strings). Done 2026-10-26: the
  strings are `const char` arrays in `tank.c`, and the id word is
  `src/overlays/tank/tank_id.c` (`TankOverlayId`, a rodata-only unit:
  splat takes a `.rodata` subsegment with no `c` one). No asm is left in
  Tank.
- [x] Tank's data and bss as C (`asm/data/overlays/tank/`): every pointer
  a symbol, every table typed with the struct the naming found. Done
  2026-10-26: all 9.9 KB are C in `tank.c`, the structured tables retyped
  with the naming (`PreMap`, `WWInf`, `hero_seq`, `file_group_load_info`,
  `shell_setup`, `u16` AI tables); lists of ids and frames stay `s32[]`.
- [x] Tank's asset files extract: list the `CRASHBSH.DAT` entries its
  `file_group_TANK*` groups load and check each decodes (docs/ASSETS.md); a
  format still kept as bytes gets decoded here. Done 2026-10-26: every DAT
  entry but the 15 code overlays decodes (400 texture sets, 400 models,
  160 sound banks, 12 texts, 5 screens), so all Tank loads does; its 66
  named entries sit in 16 of the exe's file groups (the mode texts, the
  arena sounds and music, the arena and object models, the eight
  characters' tanks).
- [x] The engine's AST table (`EngineASTTable`, `0x80098720`) as C with
  symbols: its slot entries were numbers (`.word 0x800B...`), so a moved
  overlay would still be called at its old addresses. Done 2026-10-26:
  `src/overlays/engine/asttable.c`, all 174 entries by name with the AST
  enums' ids; the engine's link takes the slot functions' addresses from
  the overlays' own links (`tools/scripts/slot_syms.py`, a Makefile step).
- [x] A shiftable `NON_MATCHING` build: an option (say `make shift OVL=tank
  PAD=N`) that pads a source-only slot overlay's code and data so every
  address in it moves, relinks the engine's AST table against it and packs a
  disc. The packer rebuilds `CRASHBSH.DAT` and its index when an entry grows
  (today a file may shrink but not grow), and the padded overlay must stay
  inside the slot (`0x800B32B4` up to `0x800D7144`, the slot image's end;
  Tank ends at `0x800CB294`). Done 2026-10-26: `make shift OVL=tank
  PAD=0x40` (`tools/scripts/shift.py`, docs/PLAYING.md). It relinks from the
  matching objects with padding in the linker script, so the C needs no
  `NON_MATCHING` code. Until the engine's data is C (Phase 5), it rewrites
  the engine data words that point at Tank (19 of them) by name.
- [x] The engine's slot image in the `NON_MATCHING` build: the id word (`0x5C`)
  and zeros in place of the 147 KB the matching build reads from the disc
  (ARCHITECTURE.md, The engine's slot image). The shift test runs with it.
- [x] Tank's shift test: the padded build boots and Tank plays through to a
  win, the same way as on the retail disc. Write the route down in
  docs/PLAYING.md, so the test can be repeated and later automated.
  Passed 2026-10-26: `make shift OVL=tank PAD=0x40`, Desert Fox played
  through to a win, everything as on retail (route: PLAYING.md, The shift
  test route). It relies on `shift.py`'s stopgap for the engine's asm data
  (the per-character hero sequence tables, the hero mode table, the
  per-world record), until Phase 5 makes that data C.
- [x] Settle `tank.h`: every declaration is defined by a built unit but
  `TANKSpecifics`, which is not in this build (`TA_SetUp` fills the exe's
  `LevSpec` at `0x8005AA70` field by field instead; ARCHITECTURE.md, Tank's
  arenas and data). Done 2026-10-26; a stale alias of ours (`D_800CAD50`, at
  `TA_players`) is gone.
- [x] Words nothing reads, checked on the other discs: Tank's seven
  never-read bss words were mapped into each prototype and demo disc's Tank
  through `TA_InitLevel`, and no build reads them, so they are
  `TankUnused*` (2026-10-26).
- [x] The dashboard measures what the pilot works on: its map has four
  tabs (Game code, Functions named, Data in C, Data named), a C file's data
  is cut at its objects, a switch's jump table shows as its function's
  (`PlotTotemBit switch table`), and the engine's slot image is counted
  apart from the data. Done 2026-10-26; Tank shows all done in every tab.
- [x] What the shift test needed: a completed save card in place of the
  cheat word, and a way to trace a crash when the emulator goes down
  (RetroArch's network commands, the BIOS's saved registers; PLAYING.md).
  Done 2026-10-26.
- [x] Write the recipe up (data to C, naming, the AST table, the shift
  test) with what the pilot learned, so each Phase 4 overlay follows it.
  Done 2026-10-26: [OVERLAY_RECIPE.md](OVERLAY_RECIPE.md).

## Phase 4 - Every slot overlay the same way

One at a time, with Phase 3's recipe ([OVERLAY_RECIPE.md](OVERLAY_RECIPE.md)): the other leaked minigames first,
since their headers name most of them, then the rest, starting with code
shared or near-identical between minigames.

**Done when** every slot overlay is source-only, named and typed, still
matches, its asset files extract and it passes its shift test, on Phase 3's
terms. A box below is ticked on the same terms. Where a leaked header covers
an overlay (Pogo, Arkenoid, Polar), every declaration in it is either
defined by matched code or noted in `docs/ARCHITECTURE.md` as not in this
build.

- [x] Finish the slot overlays' file splits where the binary proves a start
  (ARCHITECTURE.md, Files inside the slot overlays), each before that
  overlay's data goes to C, since a file's local data moves with it. Done
  2026-10-26: every start the binary proves was split already (`bash`,
  `papu`, `bonk`, `n2o`, `hub`, by jump-table parity); the leaked
  headers' order (`tools/scripts/leak_order.py`) and `units --data` prove no
  other. Pogo and Arkenoid read as one file each like Tank. Polar's
  functions 155..216 can't be placed until they are named (Polar's box).
- [x] Pogo. Done 2026-10-26: every function named (11 from the
  leak's order), rodata, data and id word as C and named, one file, its
  shift test passed (`make shift OVL=pogo PAD=0x40`, PLAYING.md). `pogo.h`
  is settled but `POGOSpecifics` (not in this build) and `number_of_banks`
  (no object fits it). Left for Phase 5: its game-state views
  (`Pogo*View`) and the tile grid's and AI's stand-ins (`PogoCell`,
  `PogoMove`, the leak's `PogoSquare`/`PogoAIPlayer`).
- [x] Arkenoid. Done 2026-10-26: every function named (6 from
  the leak's order), rodata, data and id word as C and named, one file,
  its shift test passed (`make shift OVL=arkenoid PAD=0x40`, PLAYING.md).
  `arkenoid.h`'s eight undefined names are not in this build
  (ARCHITECTURE.md, Arkenoid's data). Left for Phase 5: its game-state
  views and stand-in structs (`Arkenoid*`).
- [x] Polar. Done 2026-10-26: every function named (functions
  155..216 included: the hero actions 73..90 by the game's
  `HeroMode_nums`), rodata, data and id word as C and named, its shift
  test passed (`make shift OVL=polar PAD=0x40`, PLAYING.md). The leak
  can't place 155..216 (none of them is in `polar.h`), so it stays one
  unit; 26 of `polar.h`'s names are not identified (ARCHITECTURE.md,
  Polar's data). Left for Phase 5: its game-state views and stand-ins.
- [x] Bash (`bash`, with the crates and pickups of `XTRAS_AST_types`). Done
  2026-10-26: every function named from its code (no leaked header), rodata,
  data, id word and last byte as C, each later file's data in its own file
  (`xtras.c`, `bahero.c`), all 172 data objects named, the DAT entries it
  loads all decode, its shift test passed (`make shift OVL=bash PAD=0x40`,
  PLAYING.md). Mode 0x20's level is not identified. Left for Phase 5: its
  stand-in structs (`BashCell`, `T_BC038*`, the crate stats as the leak's
  `CrateVelocity`).
- [x] Papu. Done 2026-10-26: every function named (most from their Bash
  twins), rodata, data, id word and tail as C (`pahero.c`'s data in its own
  file), all 166 data objects named, the DAT entries it loads all decode, its
  shift test passed (`make shift OVL=papu PAD=0x40`, PLAYING.md) after the
  link fix for an overlay's own halfword labels (OVERLAY_RECIPE.md, The shift
  test). Left for Phase 5: its stand-ins (`PapuTile`, `PapuKey`, `Slot40`, the
  leak's `PapuFloor_info`/`PapuMinion_info`/`PapuSquare`).
- [x] Race. Done 2026-10-26: every function named from its code (about
  twenty uncalled in retail), rodata, data and id word as C (one file), all
  212 data objects named, the DAT entries it loads all decode, its shift test
  passed (`make shift OVL=race PAD=0x40`, PLAYING.md). Left for Phase 5: its
  stand-ins (`RaceFux*`, `RaceBarrelView` as the leak's `RaceBarrel`, the
  `EXTrace*` extensions).
- [x] Spin. Done 2026-10-26: every function named (many from their Bash and
  Papu twins), rodata, data and id word as C (one file), all 95 data objects
  named, the DAT entries it loads all decode, its shift test passed (`make
  shift OVL=spin PAD=0x40`, PLAYING.md). Left for Phase 5: its stand-ins
  (`SpinFux`, `SpinCell`, `SpinSlot`, `SpinGame`).
- [x] Dino. Done 2026-10-26: every function named (many from their Polar and
  Race twins), rodata, data and id word as C (one file), all 113 data objects
  named, the DAT entries it loads all decode, its shift test passed (`make
  shift OVL=dino PAD=0x40`, PLAYING.md) with shift.py's sequence-column rule.
  Left for Phase 5: its stand-ins (`DinoExt`, `DinoChase`, `DinoObj`, `DinoGame`).
- [x] Bonk. Done 2026-10-26: every function named, rodata, data and id word as
  C (`bohero.c`'s data in its own file), all 118 data objects named, the DAT
  entries it loads all decode, its shift test passed (`make shift OVL=bonk
  PAD=0x40`, PLAYING.md). Left for Phase 5: its stand-ins (`BonkEntry`,
  `BonkEnt`, `BonkSlot`, `BonkPad`).
- [x] N2O and NOX (`n2o`). Done 2026-10-26: every function named (many from
  Arkenoid's twins), rodata, data and id word as C (n2o.c's, then nox.c's
  from its script records), all 205 data objects named, the DAT entries it
  loads all decode, its shift test passed (`make shift OVL=n2o PAD=0x40`,
  PLAYING.md) once the engine named N2O's star field. Left for Phase 5:
  its stand-ins (`N2OSlot`, `N2ORec`, `N2ORound`, `BossGameView`, the
  asteroid and laser views).
- [x] Chase. Done 2026-10-26: every function named (many from their twins),
  rodata, data and id word as C (one file), all 143 data objects named, the
  DAT entries it loads all decode, its shift test passed (`make shift
  OVL=chase PAD=0x40`, PLAYING.md) with shift.py's mode-table rule. Left for
  Phase 5: its stand-ins (`CoalChainGame`, `ChaseAiState`, `ChasePad`, the
  coal and flame views).
- [x] Hub (`hub`: the warp rooms, the credits and the tournament podium).
  Done 2026-10-26: every function named, its six string blocks, data and id
  word as C (hub.c's data, then hubcore.c's from `0x800BC2D0`), all 207
  data objects named, the DAT entries it loads all decode, its shift test
  passed (`make shift OVL=hub PAD=0x40`, PLAYING.md) once the engine named
  the hub's functions and data it uses. Left for Phase 5: its stand-ins
  (`EngRec90248` for the gateways, the text-panel records, `HubGameView`).
- [x] Titles, the front end (`titles`). Done 2026-10-26: named, typed,
  rodata, data and id word as C, one file, and its shift test passed
  (`make shift OVL=titles PAD=0x40`, the route in PLAYING.md). Left for
  Phase 5: its views of the game-state struct (`Titles*View`).

## Phase 5 - The engine and `main.exe` to source-only

The same recipe for the two units that stay in place: the engine overlay
(its types are the vocabulary the minigames are written in) and the
executable.

**Done when** `src/overlays/engine/` and `src/main/` have no
`INCLUDE_RODATA` or data asm, both are split into their translation units,
every function and global in them is named and typed, `build/engine.bin`
and `build/main.exe` still match, and the formats the engine and the exe
load extract and are documented.

- [ ] Split the engine into its original translation units. Status: split
  at every start the binary proves (switch-table parity; ARCHITECTURE.md,
  Files inside the engine), and Jampack's records name two of the files
  (`panel` is `PANEL.CPP`, `pause` `PAUSE.CPP`); more files may hide inside
  those units.
- [x] The engine's rodata as C: all 59 INCLUDE_RODATA blocks are `const char` arrays where
  they were (2026-10-26), and its 176 strings are named from their text.
- [x] The engine's data as C (2026-10-27). `slot_refs.py` named the overlay symbol
  behind all 1086 slot words and `data2c.py engine` wrote the 515 objects, cut by
  file in link order (engine.c, asttable.c with the world records, panel.c from
  `countdown_eng`, pause.c, heroes.c, gameflow.c, levelload.c, camera.c). The
  0x465C bytes after the rodata are the engine's .bss, a memory image kept from
  the disc like the slot's (ARCHITECTURE.md, The engine's .bss). The engine's
  slot words take their addresses from the overlays' links, so `shift.py`'s
  engine-data rewrite is gone. Left from it: the shift retests below (playtests).
- [ ] Name and type every engine function and global. Status (2026-10-28): the
  most-used globals are named (`GameInfo`, `HeroHooks`, `LevelSpecs`, `Pads`,
  `MainCamera`, the viewports and cameras typed, ...); 231 more engine functions are
  named from their code (the level loop, panel, pause, heroes, game flow, loading,
  camera), the default hero actions from the leaked `HeroMode_nums` (`SetHeroIdle`
  .. `HeroKick`) and the process functions from their `process_info` slots. Every
  engine function is named (2026-10-28). Left: 773 `D_` names (2254 uses) across the
  engine and the exe. The most-used are labels inside objects: `GameOptions + 4`
  (`D_8005A608`), the arena object counts (`D_8009E46C`, `D_8009E25C`); they need their
  structs typed first. The model bank is typed (2026-10-28): `obj_dbase HeroModelBank[283]`
  with `enum DbaseIds` (ARCHITECTURE.md, The model bank), its 136 labels gone.
- [x] Type the game state (2026-10-28). `GameMode`, `GameTeams` and the other labels are
  globals of their own (the user's choice: Pogo's `Tiles_PlayerIsStealing` reloads
  `GameMode` by label in a loop, which a struct field can't give; MATCHING.md, Diff
  patterns), and the rest of `GameInfo`'s 0x18C bytes are objects around them, every
  label the code names by address defined in C: `GamePlayers` (+0x26), `GameArena`
  (+0x36), `GameTeamLeader` (+0x48), `GameHeroOut` (+0x5C), `GameWinner` (+0x60), and
  from +0x70 the adventure's progress, `GameAdventure` (`AdventureState`, laid out as
  gameflow.h's `GameStateView`). `GameTeams` stays an array: as a scalar, Nox's
  `PlotMineSFX` loads it ahead of a store through a pointer (scalars and struct fields
  are different alias classes). Left: the views (`HubGameView` ..) and the byte-offset
  users stay; turning a constant-offset user into the global at that offset changed the
  code in every unit tried (2026-10-28), so the game read those through `GameInfo`.
- [x] Name `EngineHooks`' fields (2026-10-27), from what the games put in each slot
  (`InitHero`, `HeroModels`, `RunSpeeds`, `PreMoveHero` .. `KeepInBounds`; `unk8` is
  always 0), in `EngineHooks` and in the overlays' views of `HeroHooks` (Bonk, Chase,
  N2O, Pogo, Polar, Race, Spin). Left: merging the views into the one type.
- [ ] Extract and document the asset formats the engine loads (models,
  animation, level data, ...) that Phase 2's extractor still keeps as bytes.
- [x] `main.exe`'s rodata as C: its six INCLUDE_RODATA blocks (`data2c.py main
  --strings-only`, 2026-10-26); no INCLUDE_RODATA is left in `src/`.
- [ ] `main.exe`'s data and bss as C, each file's local data with its unit.
  Status (2026-10-27): the game's .data is C, data2c's 918 objects cut by file in
  link order (main.c, dbase.c from `HeroModelBank`, prims.c, scripts.c, sfx.c from
  `MusicTable`, nascalls.c, sound.c, music.c, card.c from `CardChannel`, modchip.c,
  modchip_msg.c); the data-only Sony objects are linked (ARCHITECTURE.md, PsyQ).
  Left: the .bss written into the file (ARCHITECTURE.md, The exe's .bss), as C
  with a link that writes it out; and 471 pointer words in byte arrays (data2c's
  warnings), all inside the exe: the model bank's 0x38-byte `obj_dbase` records
  (`HeroModelBank` .. `AdventureItemsModel`, one array the code indexes, still
  separate `u8[]` labels), the archive index's 8-byte entries (`DatIndex`, still
  one label per entry) and `D_80063BB8`'s table. They only matter once the exe
  moves (Phase 6, Shift everything at once).
- [ ] Name and type every function and global in the exe. Status (2026-10-28): the
  names, parameter types and struct names the demo discs leaked are applied
  ([PROTOTYPES.md](PROTOTYPES.md)); 447 more game functions are named from their code
  (the shell, allocator, loader, dbase, renderer, scripts, NAS calls, textures, sound,
  music, card, modchip), in the leak's styles (`mal_*`, `dbase_*`, `*_thread`,
  CamelCase for the NAS calls); the last ones from their callers, every game function
  named (2537 of 2537, 2026-10-28; empty ones nothing calls are `*_unused`). Left: the
  globals, and the 131 Sony statics the converted objects carry no names for. For
  browsing cross-references across the whole binary, try Ghidra with a PSX
  loader by hand.

- [ ] Shift-test again (playtests): Bash, Arkenoid and Pogo were shift-tested
  before the overlay links took splat's labels as PROVIDEs (they had 7, 6 and 1
  pinned labels); Polar before its skating sequences (`*_seq_breathe_ft` ..) were
  split out, so 32 engine words into it didn't move then; and every overlay once
  the engine's data is C and `shift.py`'s rewrite is gone.

## Phase 6 - Finish

**Done when** every test in [The final test](#the-final-test) passes.

- [ ] No `INCLUDE_ASM` or `INCLUDE_RODATA` anywhere: none since 2026-10-26 (every
  function C, every string block C); the box closes with the data.
- [ ] The memory images: the slot's and the engine's .bss stay files read from
  the user's disc (build-machine leftovers, not source); decide whether a
  source build may write zeros there instead (the game shouldn't read them
  before writing; the shift build already zeroes the slot's). The exe's .bss is
  written into its file too (zeros, after one two-word function list;
  ARCHITECTURE.md, The exe's .bss): as C, uninitialised definitions in each file
  and a link that writes `.bss` out, or the same memory-image treatment.
- [ ] Every remaining disc file (music, movies, anything not covered above)
  extracts, and the formats are documented.
- [ ] Shift everything at once: pad `main.exe` and the engine too, and relink
  every overlay against their new addresses instead of the fixed ones in
  `configs/USA/sym.main.txt` and `sym.engine.txt`. This needs every overlay's
  data to be C, since a pointer left in a data blob keeps its old address.
- [ ] A build with a current GCC for MIPS (`NON_MATCHING`).
- [ ] Every file at quality level 4 ([CONVENTIONS.md](CONVENTIONS.md)), with no
  placeholder names.
- [ ] `docs/ARCHITECTURE.md` describes the game's systems.

## The final test

The decomp is done when all of these pass. Each proves something a byte match
alone does not. The tools come earlier: the disc packer in Phase 2, the shifted build in Phase 3 (the Tank
pilot), and every slot overlay passes a shift test of its own when it's finished. So these tests are the last check on the whole
game, not the first time anything has been tested.

1. **Clean rebuild.** A fresh clone, the user's disc and `tools/dk make check`
   (nothing else) rebuild `main.exe` and every overlay, and each matches
   `configs/USA/checksum.sha`. The disc packer then rebuilds the disc image,
   and it matches the retail image byte for byte. *Proves: nothing outside the
   repo is needed except the user's disc.*
2. **Nothing hidden.** No `INCLUDE_ASM`, `INCLUDE_RODATA`, `.incbin` of code or
   data, parked function or `func_`/`D_` name anywhere; the only binary inputs
   are the assets the extractor reads from the disc, and the engine's slot
   image, which the matching build reads from the disc too (it is the build
   machine's memory, not source: ARCHITECTURE.md). *Proves: everything is
   source.*
3. **Shifted build plays.** The fully shifted `NON_MATCHING` build boots in an
   emulator and plays: the front end, a save and a load, the hub, and every
   minigame through to a win. *Proves: no address is hard-coded; the code works
   as code, not as bytes that happen to land in the right place.*
4. **Same game.** Played side by side with the retail disc over the same
   route (front end, a save and a load, the hub, each minigame), the shifted
   build behaves the same. *Proves: the rebuilt game behaves like the
   original, not just roughly like it.* Keep the retail disc layout: load
   times depend on where files sit on the CD.
5. **A modern compiler.** The source builds with a current GCC for MIPS
   (warnings allowed, `NON_MATCHING`) and passes test 4. *Proves: the C is
   correct C, not C that only works because of the old compiler's quirks;
   this is also the first step of the PC port.*

## Afterwards - decided once the source is complete

These are not phases yet. Each gets its own plan when the final test passes.

- **Native PC port.** Build the C for PC, with assets loaded from the extracted
  files instead of the CD. That means replacing the PS1 hardware layers (CD,
  GPU and GTE, SPU sound, pads) with PC equivalents.
- **The library as C.** Sony's PsyQ code is linked from Sony's own objects.
  Rewriting it as C that compiles to the same objects would make the whole
  game source; the PC port replaces most of it anyway, so decide after that.
