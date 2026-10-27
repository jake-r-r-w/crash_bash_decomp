# Taking an overlay to source-only

The recipe Phase 4 follows for each slot overlay, written from the Tank pilot
(`tank`, Phase 3, finished 2026-10-26). An overlay is done when it has no
`INCLUDE_ASM`, no `INCLUDE_RODATA` and no data asm, every function and global
in it is named and typed, every declaration of its leaked header (if it has
one) is defined or noted as not in this build, `build/<ovl>.bin` still
matches, its asset files extract, and its shift test passes. Every step below
is an ordinary commit once `tools/dk make check` prints `OK`; tick the
overlay's roadmap box and run `tools/dk make progress` in the commit that
finishes it.

The steps go in this order because each one needs the one before: names say
what the tables are, the file split says which file a table belongs to, and
the data has to be C (pointers as symbols) before anything can move.

## 1. Names and types: the functions

- **Start from what is proven.** The leaked header (`bash/tank.h`, `pogo.h`,
  `arkenoid.h`, `polar.h`) names the AST `Init`/`Plot` pairs through its enum
  and the engine's AST table (`src/overlays/engine/asttable.c`), and gives
  prototypes. The level callbacks are what the overlay's setup function
  (`TA_SetUp` for Tank) writes into the exe's `LevSpec` at `0x8005AA70`. The
  hero states are the overlay's entries in the engine's `HEROFunc` table
  (from `0x8009DCA0`, a Set/Do pair per `HeroMode_nums` action; Tank's start
  at `0x8009DFE8`; ARCHITECTURE.md, Tank hero states).
- **Name the rest from their code**, a descriptive name where the original is
  unknown (CONVENTIONS.md, Naming). `decomp.py similar NAME` finds the same
  code matched elsewhere, `protos NAME` the function on the prototype and demo
  discs, which may still use something retail dropped.
- **Types.** Replace each stand-in struct (`PolarCam`, ...) with the real type: most are
  views of `NAS`, `EXTengine`, `subengine`, the hero structs or the overlay's
  own player struct. Every object extension Tank adds is one type
  (`EXTobject_TA`, all `AddExt(nas, 0x38, ...)`): look for that.
- **Settle the leaked header.** Every declaration is defined by a built unit
  or noted in ARCHITECTURE.md as not in this build: list the declared names and
  check each against `mipsel-linux-gnu-nm --defined-only build/*.elf`. Tank's
  two misses were a stale alias of ours and `TANKSpecifics` (retail fills the
  exe's `LevSpec` field by field instead).

## 2. The file split

`decomp.py units <ovl> --data` gives the evidence: switch-table parity proves
a file start, and initialised data ties the functions that use it.
cc1plus, which built the game, writes a file's out-of-line inline functions at
its end in reverse definition order, so a run of functions that the leaked
header lists in the opposite order is one file's inline tail (MATCHING.md,
Inline functions behind odd shapes; ARCHITECTURE.md, Files inside the slot
overlays). `tools/scripts/leak_order.py <ovl> <header>` scores that shape:
a second file is only evidence when it explains clearly more than Tank's
noise (+7 of 104) and its cut isn't inside a range the data ties together.
Tank turned out to be one file. Split before the data goes
to C: a file's local data moves with it (`split_unit.py`).

## 3. Data to C

Follow CONVENTIONS.md, Data as C: `tools/scripts/data2c.py <ovl>` (and
`--rodata SYM` for an `INCLUDE_RODATA` block of strings), the yaml handing the
subsegment to the C file, the `PROVIDE` lines in
`configs/USA/undefined_syms_manual.txt`, `make split`, and the stale asm moved
out of `asm/`. Then:

- **The id word.** A slot overlay's first word is its id (`0x5D..0x6A`). It
  can't share the first C file's object (that file's jump tables are
  8-aligned and the slot starts at 4 mod 8), so it is a rodata-only unit of
  its own: `src/overlays/<ovl>/<ovl>_id.c` with `const s32 <Name>OverlayId`,
  and the yaml line `[0x0, .rodata, overlays/<ovl>/<ovl>_id]` (splat takes a
  `.rodata` subsegment with no `c` one).
- **The asset check.** List the `CRASHBSH.DAT` entries the overlay's
  `file_group_*` objects and the exe's file groups load (DAT index at
  `0x8004E110`, file groups at `0x80050010`; names in
  `configs/USA/dat_names.txt`) and check each decodes (`decomp.py extract`,
  docs/ASSETS.md). A format still kept as bytes is decoded here.

## 4. Names and types: the data

data2c writes `D_` names and the placeholder types. Name each object from
the code that reads it, and from what points at it:

- **The engine's asm data points into every slot overlay**, and those words
  say what the objects are. `make shift` (step 5) prints each one it rewrites,
  which lists them all. For Tank they were:
  - the per-world record (`0x80098FC0` for Tank): `{world id, ..., &<GAME>_PREMAP,
    &<game>_shell_info, <setup function>}`;
  - the per-mode table at `0x800A0030`: one `file_group_<LEVEL>` per mode,
    each `{exe file group, file list}`;
  - `HeroSeqTables` (`0x8009D29C`): per character (`hero_name` 1..8 = Crash,
    Coco, Cortex, Brio, Tiny, Kong, Dingodile, Kangaroo), the sequence
    pointers by id. They are the leak's `<PRE>_seq_<anim>` arrays, defined
    in the leak's prefix order.
- **The engine's hooks.** A table the overlay stores into an engine pointer
  takes its role from the engine code that reads it. For example,
  `SetTweakAi` reads the five AI tables `TA_PreInitLevel` sets per arena.
- **Per-mode tables.** The game mode (`D_8005A648`, the game-state struct's
  `+0x34`) is the arena or level; it follows `levnumbers` from the game's
  first level. Name per-arena tables by arena (`TankDesertMap`).
- **Split objects at pointer targets.** One data2c object can be several
  objects: anything the engine points into the middle of is its own object.
  `D_800C8E7C` was `TankLUP` + `TANK_PREMAP`, and `D_800C94E8` was the turn
  sounds + 72 sequences.
- **Retype keeping the bytes.** A one-shot script that rewrites the
  initialisers is the reliable way (Tank's: `s32` words to `s16`/`u16`
  pairs, `u8` bytes to `u16`, words to `WWInf`/`PreMap` records). cc1
  word-aligns every array, struct and union in data (`DATA_ALIGNMENT` in
  mips.h); only a scalar keeps its own alignment. So an object at 2 mod 4 is
  a scalar (`u16`, not `u8[2]`), and a retype from `u8` is safe only when
  the object's size is a multiple of 4.
- **Words nothing reads.** Check them on the prototype discs too: map the
  word through a function that touches it (its absolute references, in
  order) and look for other references in that disc's copy of the overlay.
  If none reads them, name them `<Game>Unused*` with a comment on the
  evidence.
- **Complete the leaked types the data needs.** `shell_setup` and
  `file_group_load_info` were incomplete in `nas.h`. Give them the size the
  data proves, and only the fields it shows.
- Put the names in `configs/USA/sym.<ovl>.txt` (generate them from
  `mipsel-linux-gnu-nm -n build/<ovl>.ovl.elf`), and game facts in
  ARCHITECTURE.md. The dashboard's map should then show the overlay all
  done in its four tabs: Game code, Functions named, Data in C, Data named.

## 5. The shift test

`tools/dk make shift OVL=<ovl> PAD=0x40` relinks the overlay padded 0x40
before each of its rodata, text and data, relinks the engine against it with
the zeroed slot image and the moved overlay's addresses (the engine's data is
C, so every word of it that points into the overlay is a symbol and moves), and packs `build/shift/CrashBash.cue` (docs/PLAYING.md, The shifted
build). Read its output:

- the last line counts the engine's slot names that moved with the overlay.
- the packer lays the DAT out again in place if the overlay outgrew its
  sectors. The slot overlays are one file group, so entries can't move to
  the end.

Then play the overlay's game to a win on the shifted disc and write the
route in PLAYING.md, as for Tank. A save card with the games unlocked is
better than the cheat word (PLAYING.md). If it crashes:

1. build with `PAD=0`. If that plays, the crash is a pointer that didn't
   move;
2. scan RAM for words that still hold the old addresses;
3. log the registers at the crash: RetroArch's network commands read RAM,
   and the BIOS's saved registers (the TCB at `0x8000E29C`: EPC at `+0x88`,
   cause at `+0x98`) say where it died. PLAYING.md has the setup.

`make shift ... ONLY=.data` (or `.rodata`, `.text`) pads one section only, which halves the
search. Papu's crash (2026-10-26) was its own halfword variables at 2 mod 4: splat lists them in
`linkers/<ovl>_undefined_syms_auto.txt`, and a script assignment there overrode the C
definition, so they stayed at their retail address. The link now takes that list as
`PROVIDE`s (`build/<ovl>_undefined_syms.ld`), and `shift.py` refuses an overlay symbol that
still links as absolute.

Tank's three crashes were all unmoved pointers: a pointer into the middle of
an object, a fixed address in engine C (the HUD's `TA_players`), and the
per-character sequence tables. N2O's Retry crash (2026-10-26) was the same kind as the HUD's:
the engine's `InitWeatherNas` used N2O's star field by its `D_` address (a `PROVIDE`, mode
0x1F only). `make shift` now prints a warning for each such engine name; give the engine's C
the overlay's name and its link takes the address from the overlay's ELF. The same goes
for functions: the hub's black screen on loading a save (2026-10-26) was `camera.c` calling
`func_800B8098` (`HubCameraFollow`), which `linkers/engine_undefined_funcs_auto.txt` pins
to its retail address; the warning covers that list too. Before the shift test, `nm -u` the
engine's objects: no `func_800B`/`func_800C` or `D_800B`/`D_800C` name should be left.

## Gotchas from the pilot

- `open(p, "w").write(f(open(p).read()))` empties the file when `f` fails:
  read, transform, then write.
- In a Git Bash heredoc piped to Python, `\n` inside the script loses its
  backslash. Write scripts with the editor tools, or quote the heredoc
  (`<<'EOF'`).
- cc1 emits an initialiser's string literals in reverse first-use order, so
  strings in data are named `const char` arrays.
- The slot overlays link with `SUBALIGN(2)`, so where one file's `.data`
  ends and the next one's starts, retail's alignment padding is gone from
  the link: write it out as the first file's last object (`s16 D_<addr> = 0;`,
  Papu's `D_800BE896`). Inside a file cc1 pads by itself.
- Explicit zero data stays in `.data` with gcc 2.8, and the overlay link has
  no `*(COMMON)`, so zero objects are `= 0` / `= {0}`.
- Read the game mode as the game-state struct's field when the schedule
  matters (MATCHING.md, Diff patterns).
- The engine's data is C (2026-10-27), so a shifted disc moves exactly the
  engine words that name the overlay's symbols. Until then `shift.py` rewrote
  the asm data's words by address, which could catch another overlay's, so
  only the shifted overlay's game was tested.
