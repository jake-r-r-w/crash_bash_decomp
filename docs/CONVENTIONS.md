# Conventions

What "finished" means for a file, and the rules that keep the source tree
consistent.

## Quality levels

1. **Matching** - the function is C, and the whole executable still matches
   the retail SHA-1 (`decomp.py done` checks both before recording it).
2. **Named** - the function, its parameters, locals and the globals it touches
   have real names (`decomp.py rename`), not `func_8001xxxx` / `D_8004xxxx`.
3. **Typed** - m2c pointer arithmetic (`*(s32*)((u8*)arg0 + 4)`) is replaced by
   struct fields declared in a header; no `M2C_*` macros or `?` types remain.
   Byte arithmetic that is the data's own layout goes through a named macro:
   `FILE_AT(base, off)` and `SELF_REL(p)` (`common.h`) for the offsets a file's
   header or a variable-size record stores, and the heap's `MAL_AFTER`,
   `MAL_END`, `MAL_DATA`, `MAL_LAST` and `MAL_OF_LAST` (`bash/main.h`). A byte
   form kept because the typed one changes the code carries a comment starting
   `/* matching:` that says what changes; `progress` counts it as typed.
   Field names: a member no code touches is `padXX`; one the code only stores keeps `unkXX`
   (nothing shows what it holds); one the code reads gets a name from those reads
   (`tools/scripts/unk_fields.py` sorts them, `field_uses.py STRUCT` lists a struct's uses).
4. **Placed** - the function lives in the translation unit and subsystem file it
   came from, next to the functions and file-local data it shares.

A file is finished when every function in it reaches level 4.

## Naming

- Functions and globals: `PascalCase` for exported game functions once their
  role is known, matching the style of the PsyQ names they call
  (`LoadImage`, `CdRead`); keep splat's `func_<ADDR>` / `D_<ADDR>` until then.
- Never invent a name you cannot justify from behaviour, strings or leaked
  symbols; a descriptive placeholder (`Player_UpdateUnk`) is better than a
  wrong confident one.
- Names are written through `decomp.py rename OLD NEW`, which updates
  `configs/USA/sym.main.txt` (or `sym.<ovl>.txt`), the sources and the index,
  and rolls back if the build stops matching. A struct field is renamed in its struct by
  hand, then `tools/scripts/field_rename.py OLD NEW` renames its uses: the lines where cc1
  reports the old member missing, so a name other structs share (`unk6C`) is safe.
- Typing by the compiler's layout (tools/scripts; no byte changes, check with `make check`):
  `struct_layout.py TYPE [--leaves]` prints a struct's members (or every reachable leaf by
  offset) from cc1's own -g output. `retype.py FILE FUNC VAR TYPE[:MEMBER]` turns a byte
  pointer's `*(T *)(VAR + N)` into `VAR->field` and fixes the prototype; every `LEFT` line it
  prints is arithmetic the new type now scales, to fix by hand (`--revert` undoes it).
  `fold_views.py CANON VIEW...` folds view structs into the one they view (by offset, in rounds,
  clashing names through `t_NAME` temporaries; `--dry` first; `--rename OLD=NEW ...` for the
  rename passes alone). `diff_funcs.py [UNIT]` lists every function whose bytes differ, where
  `decomp.py verify` stops at the first. Two things to watch after a retype: a cast like
  `*(s16 *)&x.f` is a different alias class from a field of that type (the schedule can change:
  give the struct the field instead), and a load's sign follows the member's type. A retype that
  still matches doesn't prove the type: read the callers first (the bytes can fit another struct).
- PsyQ names come from the signature scan and Sony's objects; do not rename them.

## Source layout

- A segment is split into its files (`src/main/<file>.c`,
  `src/overlays/<overlay>/<file>.c`) as boundaries are found: switch-table
  parity, `.data`/`.bss` and `.rodata` order, file-local data, strings, a
  change of compiler style, a leaked line table, and what the functions on
  either side of a cut zone do. The evidence goes in ARCHITECTURE.md.
  `tools/scripts/split_unit.py` does a split end to end: the yaml
  subsegments for `.text` and `.rodata`, the moved prototypes and header
  inlines, `make split`, `decomp.py index`, the build and the check. A file
  is named by what it holds when that is known (`sound.c`; a leaked source
  name wins: `panel.c` is `PANEL.CPP`) and by its start address until then
  (`src/main/prims.c`); rename it when it is.
- A unit built with other compiler settings than `configs/USA/project.mk`
  gets them in the Makefile as `build/<src>.o: VAR = value` lines
  (`CC_VER`, `ASPSX_FILL`); `decomp.py` reads the same lines.
- Overlays go in `src/overlays/<overlay>/`.
- `src/ps1/` holds what only the PS1 build compiles (the exe header, `bios_1`'s data, the mod-chip
  check, the overlay id words), and `port/` what only the PC build compiles (PORT.md).
- PsyQ library code is Sony's objects, linked in place (`decomp.py
  link-psyq`); it is never decompiled.

## Headers

- Game headers live in `include/bash/`, one owner per declaration (`tools/scripts/decl_dups.py`
  lists a repeat; `make test-tools` fails on one): a struct or
  prototype is declared in exactly one header, included where needed.
- Every source file has an **owned header** that declares the functions and data
  it defines: `bash/main.h` (`src/main/main.c`), `bash/engine.h` (the engine
  overlay's first file, which includes the others' `bash/panel.h` ...), `bash/bash_ovl.h`..`bash/hub_ovl.h` for the slot overlays (`_ovl`, since the leak's
  `bash/polar.h` and the rest are the original headers; each includes its later files'
  `bash/<ovl>_<unit>.h`), and one per split unit
  (`bash/modchip.h`, created by `decomp.py done` when it first needs one). Each is marked
  `owner: <source path>`, and that marker is how `decomp.py` finds it. When a
  file is split, its declarations move with its functions to the new file's
  header.
- The headers recovered from the original source (`bash/tank.h`, `pogo.h`,
  `arkenoid.h`, `polar.h`, and the ones built from the Oct 9 prototype's debug
  types: `core.h`, `hero.h`, `shell.h`, `hub.h`, ...; see ARCHITECTURE.md)
  win: a name they declare is not repeated in an owned header. `decomp.py
  rename` drops the owned copy when a function takes such a name.
- Every slot overlay file includes `bash/engine.h` first (it brings `main.h` and every
  engine header), so an overlay uses the engine's and the exe's declarations as they are and
  never redeclares one: no view of an engine global or function in an overlay header
  (`decl_dups.py --views` lists any; its test keeps them at zero). Where an overlay needs the
  original's different code from a declaration, the call or access says so with a
  `/* matching:` cast (an unprototyped call, an `int` function's call, a pointer read through
  a cast).
- `bash/nas.h` holds the engine object model (`NAS`, the `EXT*` extensions) and
  the other shared engine types.
- `decomp.py done` adds the matched function's prototype to its owned header.
  Data a new match uses goes in the owned header of the binary that holds it
  (`main.h` for the exe's data, `engine.h` for the engine's). Slot-overlay data
  goes in that overlay's own header, because all 14 overlays share one load
  address.
- `include/common.h` holds base types and includes `include/psyq.h`, which pulls
  in Sony's headers (`libgpu.h`, `libsnd.h`, `libcd.h`, ...), so every source
  sees the SDK's prototypes and types (`RECT`, `CdlLOC`, `CdlSetloc`, ...). Add a
  Sony header there when game code first calls into its library.
  `include_asm.h`, `macro.inc` and
  `labels.inc` are generated by splat; do not edit them.
- SDK types and prototypes come from Sony's headers (`ext/psyq47/include`), not
  from re-declarations.

## Matching rules

- Matching C must compile with the stock pipeline in `configs/USA/project.mk`.
  No compiler or maspsx patches, no post-build rewriting, no whole-function
  `asm()` bodies.
- Only C. No inline asm of any kind: no `asm()` statements, register pins
  (`register int x asm("$20")`) or empty `asm("")` barriers, in the body or in a
  macro or static inline it uses; `decomp.py done` refuses them. Two
  exceptions: the SDK's `gte_*` macros (GTE coprocessor instructions C can't
  express), and `draw_verts`'s (`draw_verts`, main.exe) three register
  variables, which are the original's (decided 2026-10-26). gcc reaches its
  `$18`-`$20` with `$16`/`$17` unused only through those pins (AGENT_LOG.md,
  Open leads), and they emit no instructions of their own. No other
  function gets one.
- Natural C first: what a 1999 programmer would have written, with the
  diff fixed by types, statement order and real code shapes found from the
  evidence and the compiler's rules (MATCHING.md). C that only steers the
  compiler (a `do { } while (0)` wrapper, an extra temporary or store, a
  reordered statement) is fine when no natural form matches; say which and
  the gcc rule it works around in the match note.
- A call that hands on the caller's own registers without naming them (`f()` with `f`'s `a0` still
  the caller's) is written `f(PASSED(arg0))`: nothing on the PS1, the argument on PC (PORT.md). A read
  or write through a null pointer, which the PS1 lets through, is written `LOW_RAM(p)->x`.
- One function per match commit, through `decomp.py done NAME --commit`
  ([MATCHING.md](MATCHING.md)).
- A function that will not match after a bounded effort is parked
  (`decomp.py block NAME --note ...`) with its best C kept under
  `#ifdef NON_MATCHING`, with the `INCLUDE_ASM` line under `#else`, so the
  build still matches.

## Data as C

A unit's data becomes C with `tools/scripts/data2c.py OVL [--rodata SYM]`, which prints
definitions for the overlay's asm data (and an `INCLUDE_RODATA` block of strings). The
recipe, as done for Tank (`tank`, 2026-10-26):

1. Name and type first: an object is written with the type its `extern` declaration gives
   it, so a placeholder type (`u8 D_X[]` over a table of pointers) should be fixed before;
   the tool warns where a byte or halfword array holds an address in the overlay. A
   declaration nobody can match (an anonymous `struct { ... }`) needs a named type.
2. Split the unit first (a file's local data moves with it).
3. Run the tool against the sources as they are (it reads `src/` to decide which splat
   labels are objects: one no C names and no data points at is folded into the object
   before it), and paste its output at the top of the C file, after the includes: rodata
   strings first (they must precede the functions' jump tables), then the data in address
   order (cc1 emits `.data` in definition order). Remove the `INCLUDE_RODATA`.
4. In the overlay's yaml, give the data subsegment to the C file (`[ROM, .data, <the c
   file>]`), add the tool's `PROVIDE` lines to `configs/USA/undefined_syms_manual.txt`
   (splat lists an overlay's outside symbols from its asm, and C data is no longer asm),
   `make split`, and move splat's now-stale `asm/data/...data.s` and the block's `D_*.s`
   out of `asm/` (they would keep the dashboard counting the data as asm).
5. `make check` is the test; a short build means an object came out smaller than its slot.
6. `tools/scripts/bytes_ptrs.py` (after `make`) finds a pointer left as numbers: a byte,
   halfword or word array of plain numbers with an aligned word equal to a symbol's address
   in its unit's ELF, or inside a data object (`SYM+0xNN`: the symbols carry no sizes, so an
   object runs to the next symbol). Those match but don't move in a shifted build; rewrite
   the array as words with `(s32)&SYM` (or its real type). It found four in the exe
   (ShellModules, ComboSeqs, VramNodePool, LangModeTypeTexts, 2026-10-28), and with interior
   pointers the engine's ModeDbaseLists (FileGroups, HeroModelBank and DatIndex entries);
   none left (Papu's PapuUnusedSeq word 0x80070000 is a coincidence).

How the tool writes things, and why:
- Bytes come from the retail binary, not splat's directives (splat reads small words as
  strings). A 4-byte value that points into the overlay is written as its symbol (name +
  offset), and one into the exe or the engine when a symbol file names it, splat wrote it
  as a symbol, or it is word-aligned off a 64 KB boundary (a round `0x80070000` among sound
  ids is a flag). Pointers stay pointers so a shifted build moves them.
- Strings in an initialiser are `const char` arrays: cc1 emits an initialiser's string
  literals in reverse order of first use, cc1plus (which built the game) in order, so only
  named arrays reproduce retail.
- Zero objects are `= 0` / `= {0}`: the overlay's link has no `*(COMMON)`, and gcc 2.8 keeps
  explicitly zero-initialised data in `.data` (the original's were `.bss`, which the overlay
  image holds as zeros right after its data; the order and bytes come out the same).
- When a declared type is shorter than its slot, the rest is an object of its own,
  `D_<address>` (cc1 pads only to the next object's alignment).

## Code style

C89 as GCC 2.8.1 accepts it: declarations at the top of blocks, `/* */`
comments, 4-space indentation, `s32`/`u8`/... types from `common.h`.
