# Notes for agents

Read [docs/AGENT_LOG.md](docs/AGENT_LOG.md) first (latest state and next
steps), then [CONTRIBUTING.md](CONTRIBUTING.md) and
[docs/CONVENTIONS.md](docs/CONVENTIONS.md).

- Every game function matches (2026-10-26). The work now is naming, typing,
  data to C and file splits (docs/ROADMAP.md); each such change is an
  ordinary commit once `tools/dk make check` prints `OK`. The matching tools
  below stay for reworking a function.
- Everything runs through `tools/dk` (Docker); the host has no Python.
- This repo follows the `psx-matching-decomp` skill with different paths:
  `tools/scripts/decomp.py` (not `tools/decomp.py`), `configs/USA/` (not
  `config/`), symbols in `configs/USA/sym.main.txt`, exe target `build/main.exe`.
- Never commit game data or `ext/`; never commit a match except via
  `decomp.py done NAME --commit`.
- Only C: no inline asm of any kind (the gate refuses it, in macros and inlines
  too; the SDK's `gte_*` macros are the exception, and `draw_verts`'s original
  register pins the one recorded exception, CONVENTIONS.md). Natural C first; C that only
  steers the compiler (a `do { } while (0)`, an extra temporary) is fine when
  nothing natural matches.
- Matching is by hand, one function per commit, through the validation gate
  `decomp.py done NAME --commit` ([docs/MATCHING.md](docs/MATCHING.md); skill
  `/match-function`). `decomp.py auto` is retired.
- Parallel matching: `node tools/swarm/swarm.mjs` (host Node; see
  [tools/swarm/README.md](tools/swarm/README.md)). Stop it with
  `node tools/swarm/swarm.mjs stop` (`--now` to kill). It commits only through
  the same gate. Functions >= `--big-bytes` (600) go to resumable big-mode
  workers; `swarm.mjs stats` shows match rates by size to tune that. `--seg`
  is a prefix (`main/` covers every main.exe unit).
- Playing/testing a build: `tools/dk make disc` (bootable `build/CrashBash.cue`),
  `make disc-check` (byte-identical to retail), `make shift OVL=tank PAD=0x40`
  (a disc with that overlay's addresses all moved, `build/shift/CrashBash.cue`). To show the user, launch it on the host: `C:/RetroArch-Win64/retroarch.exe -L
  C:/RetroArch-Win64/cores/pcsx_rearmed_libretro.dll build/CrashBash.cue`.
  See [docs/PLAYING.md](docs/PLAYING.md).
- Game data: `assets/` (ignored) holds the archive's files as editable formats
  (`dat.py extract`, run by `decomp.py extract`; formats in
  [docs/ASSETS.md](docs/ASSETS.md)); `make disc` builds `CRASHBSH.DAT` from it.
  `build/` is disposable (`make clean` deletes it all); notes and drafts go in
  `scratch/`.
- `make diff` / `decomp.py try` end with `STAGE NAME structure|instructions|registers n/total`:
  fix the stages in that order. Check the asm's style fits the file's compiler
  first (gcc 2.7.2 units: `main/modchip`, `main/modchip_msg`; per-unit settings
  in the Makefile). For a tie in instruction order, register choice or a
  merged tail, read the deciding pass in `tools/reference/gcc-2.8.1/` (FSF base of our
  2.8.1-psx; `gcc-2.7.2.3/` for the 2.7.2 units) before the permuter. Work the rule through by hand against the
  `-dS`/`-dR`/`-dl`/`-dJ` dumps (MATCHING.md, Reading the compiler's rules).
  The real cc1 is in the container: dump any pass (`-dr`/`-ds`/`-dc` too) and
  try C shapes in small test files before rebuilding the function
  (MATCHING.md, Using the compiler directly). `tools/cc1/` rebuilds it from
  source byte for byte, for analysis variants only (`build.sh NAME --patch F
  --cflags ...`, `with.sh NAME CMD`, `check.sh NAME`, and `cc1_compare.py
  FUNC [FILE...]` under `with.sh` for which builds a variant changes;
  tools/cc1/README.md);
  matches always use `/opt/gcc`. `try NAME --file F.c` diffs a
  test copy of the whole function with its unit's settings in about 2 s, and
  `--file A.c B.c ...` compiles several in parallel and ranks them by the
  regions score; `orders NAME --lines A-B` searches random dependency-keeping
  orders of those statements the same way (for sched1 luid ties;
  `--free-memory` lets stores to different places swap).
  For a big function, `decomp.py regions NAME` aligns the code and scores it
  block by block with C lines (MATCHING.md, Big functions); the swarm's big
  mode works and ranks checkpoints by it. Also: `similar NAME` (matched
  functions with the same code), `idiom 'PATTERN'` (matched functions with an
  instruction pattern, e.g. `'move A, B; sll B, A, N'`, and the C lines behind
  each hit), `vars NAME` (the variable behind a register
  swap), `sched NAME` (why cc1 allocated and scheduled it so: priorities, each
  block's memory-insn count, and the rule that took each insn off the ready
  list: priority, birthing, class, potential hazard or luid), `jtrace NAME`
  (jump2's cross-jumping step by step: which copy of a repeated tail is
  kept, and why the others gave theirs up), `trace NAME --insn UID|--var V`
  (one insn, or every insn of a variable's pseudo, through every pass dump:
  which pass moved, rewrote or deleted it, and where a deleted insn went),
  `protos NAME` (the function's
  earlier compiles on the prototype and demo discs; `--diff DISC` aligns
  one copy with retail and prints the hunks that differ, `--file F.c` with
  a test copy's build instead, `--words` prints the copy), `splitvars NAME VAR` (search splits of a variable), `permute NAME
  --differing` (permute only the differing blocks' C), `permstats` (every
  permuter run: speed, improvements, matches), `units [SEG]` (evidence
  for file boundaries: switch-table parity proves a file start in any
  segment; `--data` adds data blocks, inversions and call ties for any
  exe or slot overlay unit, `--why` the symbol pairs behind each inversion;
  `tools/scripts/leak_order.py OVL HEADER` the climb/fall fit against a
  leaked header's order).
  `tools/scripts/rename_unit.py OLD NEW PREFIX` renames an overlay unit everywhere and
  `rename_file.py SEG NEW` one of its files (no byte
  changes); `field_rename.py OLD NEW` a struct field's uses after its struct (by cc1's
  missing-member errors, so shared names like `unk6C` are safe); `bytes_ptrs.py` byte and
  halfword data arrays holding a symbol's address (pointers a shift would leave behind). `tools/scripts/model_slots.py` lists each exe model slot with the archive files
  every game loads into it. `tools/scripts/data2c.py OVL [--rodata SYM ...] [--strings-only]` writes an overlay's asm data as C
  (OVL `engine` or `main` for main.exe too; `--strings-only` for just the string blocks)
  definitions (docs/CONVENTIONS.md, Data as C). The engine's link gets the slot
  overlays' symbols its AST table names from their ELFs (`slot_syms.py`, run by
  the Makefile), so the overlays link before the engine.
  `sched`, `jtrace`, `trace`, `regions` and `vars` read a parked function's C under
  `#ifdef NON_MATCHING` (built apart, with `-DNON_MATCHING`).
- Taking an overlay to source-only (Phase 4): follow
  [docs/OVERLAY_RECIPE.md](docs/OVERLAY_RECIPE.md), written from the Tank pilot.
- At the end of every session, update `docs/AGENT_LOG.md`: Current state,
  Next and Open leads, any new gotcha, and one short entry at the top of its
  Session log. Matching idioms go in the Diff patterns table in
  `docs/MATCHING.md`, game facts in `docs/ARCHITECTURE.md`.
