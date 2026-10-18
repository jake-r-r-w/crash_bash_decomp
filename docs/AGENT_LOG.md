# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-18)

- Phase 2 of [ROADMAP.md](ROADMAP.md): matching `main.exe`. Phase 1 (types
  and names) is done.
- 1434/2551 game functions matched: **main.exe is complete (575/575)**;
  engine overlay 2 left, both parked (`func_8007F314`, `PlotScriptFX`),
  ovl61 95/97 (two 2026-10-18 swarm runs and three hand matches; 2 big ones left),
  ovl67 101/135, ovl69 68/77, ovl6a 93/94 (2026-10-18 swarm runs and hand fixes;
  `func_800B5710` parked at 120/121 blocks). Every slot overlay's id word is
  its own rodata unit (2026-10-19), so first-file switches link as `switch`, all 564 PsyQ functions (Sony
  objects). 3 parked (`decomp.py status`).
  `make check` OK. Live figures: `decomp.py progress` / `docs/index.html`.
- Dashboard (2026-10-18): `docs/index.html` is `tools/scripts/dashboard.html` plus
  the data; a live Swarm card from the git-ignored `docs/swarm.js` the swarm
  writes (tools/swarm/README.md, Live status). `progress` takes ~2.5 s warm.
- The engine overlay is six files and `ovl5d` three, `ovl5e`, `ovl66`,
  `ovl67`, `ovl6a` two each since 2026-10-15 (ARCHITECTURE.md, Files inside
  the engine / the slot overlays); main.c is still one file.
- Compiler: gcc 2.8.1-psx `-O2 -G0`, maspsx `--aspsx-version=2.86` (settled;
  2.7.2-cdk matched a strict subset).
- PsyQ 4.7: all library code is Sony's objects (281, plus `2MBYTE.OBJ`), and
  their `.data` comes from the objects except `bios_1.o`'s (one value differs;
  `configs/USA/psyq/data_keep.txt`). Roadmap: the PsyQ gaps box is ticked.
  The game is built with `-fno-builtin` (it calls `abs`).
- Every type and AST id in the Oct 9 prototype's debug fragment is in
  `include/bash/` and every overlay is identified (ARCHITECTURE.md, Prototype
  debug types). The disc images are in the git-ignored `protos/`.
- Game data (2026-10-16): every `CRASHBSH.DAT` entry but the 15 code overlays
  extracts to editable files in `assets/CRASHBSH/` (textures PNG, models OBJ
  + glTF with animations, sounds WAV/VAB, music SEQ/MIDI, text, screens) and
  packs back byte for byte; `make disc-check` from a fresh extraction is
  identical (docs/ASSETS.md). ROADMAP's asset extraction box is ticked.

## Next

**ovl6a (2026-10-18, run 20261018184703):** stopped by hand after 87 min,
$47.98: 64 matched, 2 failed at the gate (`func_800B4EAC`: compile error, a
field another match removed; `func_800B488C`: matches alone but the full
build is +4 bytes, two jump tables' alignment). Since then
`func_800B4EAC`, `func_800B551C` and `func_800B488C` are matched and
`func_800B5710` is parked (Open leads). Next: ovl61's `func_800B8A68`/`PlotBall`, ovl67's
parked `func_800B39C0`.

**ovl69 (2026-10-18, run 20261018173050):** stopped by hand after 75 min,
$33.21: 48 matched, 1 failed at the gate on a compile error (`func_800B7CE8`,
same cause as ovl67's two), 10 left in the queue. Folded.

**ovl67 (2026-10-18, run 20261018163029):** stopped by hand after 58 min,
$39.26: 82 matched ($0.48 each), 1 parked (`func_800B39C0`, registers 29/145,
an s0/s1 swap), 2 failed at the gate on compile errors from declarations
other matches in the run added (`func_800BA43C`, `func_800B3E74`): the
worker's C was right against an older tip, so retry them (`swarm.mjs forget
--only ...` resets their attempt if needed). 36 left in the queue; the
pattern rows are folded.

**ovl61 (2026-10-18):** two runs, 83 matched, and `CalcArkInfluence`,
`func_800B85DC` and `func_800BE4D4` by hand; 95/97 functions, 2 left, both big
and checkpointed:
`func_800B8A68` (3264 B; its best C has a `- k + k` hack to drop),
`PlotBall` (5576 B, paired 1366/1394). Next: a big-only run on
ovl61 (`--workers 1 --big-workers 3`), or a new overlay with the big-heavy
mix (`--workers 1 --big-workers 4 --permuters 2`). (The fold for run 20261018125240 is done.)

**Tooling: the 2026-10-16 tooling plan is done and removed** (it is in git
history, `git show 94f04c7:docs/TOOLING_PLAN.md`; what it left is under
Open leads, Tooling ideas). `swarm.mjs stats` shows $/match by size from
`ledger.jsonl`. The first supervised run on it (engine, 2026-10-16, 60 min, $41.11)
matched 28 (27 + one fixed by hand), parked 1. Against earlier runs, $/match
fell for 250-399 B normal ($0.79 -> $0.66, first-session matches 54% ->
70%) and 400-599 B big ($4.58 -> $3.52); small samples, and what is left
is the harder remainder. The closest-C carry-over did the work: three
functions parked close then matched on the retry from their C. **Next:
repeat the engine hour** (2 left, both parked: `func_8007F314` 5772 B,
`PlotScriptFX` 2080 B) with the big-heavy recipe below; `permute --near` first if there is CPU
time. After each swarm run, compare `stats --run ID` with the ledger's
earlier runs.

**main.exe retry (2026-10-16, run 20261016133425):** the 15 parked
main.exe functions, unparked with `decomp.py unblock` and run in big mode
only (`--big-bytes 1 --big-workers 5 --permuters 0 --retry-failed`), plus
`func_8002D4F4`: 12 matched in 75 min for $53.46 ($4.46 a match), most in
one session of 1-16 min. The day's tool upgrades (closest-C checkpoints,
`regions`, the fixed patch check and gate) cracked what hand sessions had
parked. **Retry parks with this recipe after tool upgrades.**

**Start of next session: matching.** Run the engine swarm hour (item 1b,
`--max-functions 500`), then the post-run checklist (`fold` was done
2026-10-16 after the main.exe retry; `permute --near` not yet: it covers
the three main.exe parks, all register-only; all three were matched by
hand since). main.exe is fully matched. The file splits are done as far as the evidence goes
(item 3).

0. Current focus (ROADMAP.md, Current focus): `make disc` works and the
   matching build is the retail image. The user tests builds by playing
   them; no recordings are kept up.
1. Big functions. Big mode works block by block with `decomp.py regions`
   and ranks checkpoints by region score (tools/swarm/README.md, Big mode).
   2026-10-14: 13 of 15 fresh 828-1212 B functions matched in the A/B
   runs, `func_80019A60` went 37/59 -> 55/59 blocks then matched by hand,
   and a 60-min `main/` run matched 10 of 16 functions leased.
   main.exe's big functions are all matched (the last four by hand on
   2026-10-16); the engine's are next (1b).
1b. Overlays with the swarm, engine first (shared by every minigame):
   `swarm.mjs run --branch main --seg overlays/engine --max-minutes 60
   --max-run-usd 80 --max-functions 500 --workers 3 --big-workers 2
   --big-bytes 400 --big-minutes 27` matched 45 in 60 min for $21.72 on
   2026-10-15 (mostly under 150 B; $21.72 / 45 = $0.48 a match, a floor: big
   sessions killed at their deadline cost $0 in the logs). A second hour matched 38 for $16.46, a third 59 in 43 min for
   $27.60 (stopped early). Evening: 20 in 24 min ($18.34) and 29 in 20 min
(both stopped early). 2026-10-16: 28 in 60 min ($41.11, mostly 250-600 B
now). 2026-10-16 night: 19 in 60 min ($56.63, $2.98 a match, 0 parked) with
`--workers 1 --big-workers 4`: 71 of the 73 left were big (>= 400 B or
escalated), so 3 normal workers would idle; use that split for the engine
from now on. 2026-10-17: 9 in 38 min ($28.30; stopped when the internet dropped), 600-1399 B now.
2026-10-17 morning: 23 in 60 min ($65.81, $2.86 a match; 900-1399 B big $2.50,
13/16 first-session). 2026-10-17 midday: 10 in 60 min ($63.10, 1456-2804 B), then a 15-min run
matched 2 more (`func_80083708` 1924 B, `func_8007D9DC` 3528 B; $6.37,
killed early). Repeat it; 10 engine functions are left. Near
misses with big-mode checkpoints: `func_8007BFA8` (instructions 5/114),
`func_8007C170` (17/114), `func_8007CC80` (structure 77/116),
`func_800926CC` (matched by hand 2026-10-18), `func_8008C824` (matched by hand 2026-10-18),
`func_80079260` (4/98), `func_80094A28` (5/68), `func_800939FC` (matched by the permuter 2026-10-18). `--max-functions 500` leaves time and cost as the limits (40 stopped
   leasing after about 30 min). Sizes left per overlay: run the size loop over
   `nonmatching NAME, 0xSIZE` headers (the `next --count` listing is slow).
2. After each swarm run, the "After a run" checklist in
   tools/swarm/README.md (`stats`, parks, `fold`, `permute --near`, this
   log). main.exe has no functions left; its swarm recipe (`--seg main/`,
   `--big-bytes 400 --workers 1 --big-workers 4`) carries over to overlays.
3. Split `main.c` into its translation units, between swarm runs (on hold
   since 2026-10-15: the evidence so far groups functions but places no
   boundary; see ARCHITECTURE.md, Files inside `main/main`). `decomp.py units
   --data` now shows the data blocks (regions of files); and, last, the
   inversions: eight probable one-file regions, boundaries in the gaps
   between (ARCHITECTURE.md). With the call graph, 17 possible cut zones
   remain, none proven (no positive signal). Positive sign found: jump-table parity
   (`decomp.py units SEG`, any segment) proves file starts; main/main has
   one (95..370), the engine five, five slot overlays some. The engine is split
   (2026-10-15). The slot overlays with parity
   flips are split too. Keep main.c whole: its one proven boundary
   (95..370) has three possible places (ARCHITECTURE.md), and no overlay
   shows another positive sign (no string stored twice, no 2.7.2 style).
   More cuts need new evidence (a leak, a symbol file of a game on this
   engine) (a split
   moves functions, so workers' patches and big-mode checkpoints for them
   stop applying). Find the boundaries first and write the evidence in
   ARCHITECTURE.md: rodata order, alignment padding, file-local statics,
   changes of compiler style (as at `0x8002D488`). `decomp.py units
   [--clusters]` collects that evidence: for `main/main` (2026-10-14) style
   and strings give nothing (all 2.8.1, no duplicate literals), two places
   where rodata goes back (after `func_80013488`, after `func_8002D178`),
   and 25 clusters of functions sharing file-local data (183 of 566
   functions: random, SFX voices, CD, pad, SEQ sound, ...), so boundaries
   fall between clusters. Code in the wrong unit
   can't match, and the rodata batch failures came from `main.c`'s single
   rodata block.
4. By hand: `decomp.py next --seg main/`, the MATCHING.md process (check
   the unit, evidence, STAGE by STAGE, Diff patterns, cc1 dump, then
   permuter); for a big function, `decomp.py regions` block by block. No
   `main.exe` function left has an identical copy (`decomp.py dups`). A
   function that parks still gets its prototype in its owned header (an
   ordinary commit after `make check`).
5. Name each function as it's matched when the evidence allows. Unpark
   when a new idea comes up (see Open leads).
6. Later Phase 2 items: data to C (after the split), naming and typing
   passes. (The `CRASHBSH.DAT` extractor is done: docs/ASSETS.md.)

## Open leads

- **`func_800B5710` (ovl6a, 2836 B), parked 2026-10-18 at regions 120/121
  blocks, 706/710 insns** (C under `#ifdef NON_MATCHING`). Only the 0x80
  button block differs (target 0x37c-0x394): the target puts
  `mv->forward_speed = speed` in the delay slot of the `% 0x1000` `bgez`,
  after `lui v0; lw t1,0x18(sp); addiu a0,v0,%lo(D_8009EE9C)`; ours issues
  `lw t1; nop; sw` first (710 against 709). Worked through against cc1's
  `-dS`/`-dR` traces: sched2 keeps sched1's order on ties (no hazard for
  stores), so the store must leave sched1 last in its block. sched1 only does
  that if the store comes after the modulo's `t = step` copy (expand_divmod
  emits it right before the `bgez`) and the address isn't a birthing insn.
  No ordering, spelling, twice-set `cam` pointer or `do { } while (0)` got
  there; something before reload must differ (an extra insn, or the copy
  emitted before the `if`). Everything else matched by rules now in Diff
  patterns: the case 5 fallthrough, `PlaySfx(0x38D)` written in each failing
  branch (lengthens `mv`'s life so it spills and the hoist gets `fp`),
  `Heroes[arg0].field` / `D_800BCCF4[x].field`, `G->unk63 = best`, the
  `mode` local and the case order 3/4, 2, 1, default.
- **Hack matches to revisit:** `decomp.py hacks` lists them (one on
  2026-10-18: `func_800193A8`, register pins that are probably the original
  source). Parks are `decomp.py status` / the dashboard. Before reaching for a
  barrier or pin, try `do { } while (0)` (Diff patterns): it matched
  `func_800926CC` and `func_8008A924` without one.
- **`PlotScriptFX` (engine script keyframes, 2080 B), parked 2026-10-18 at
  regions 95/112, 484/520 exact** (C under `#ifdef NON_MATCHING`). Left:
  - Block 21: the target computes `p = &kf[keyindex]` into v0, copies it to
    `cur` (s2), reads `start` through v0, does `addiu v0,v0,0x50` and
    copies that to `next` (s4). In ours, cse makes `cur` the canonical
    register (`make_regs_eqv`: it outlives `p`), rewrites `p + 80` to
    `cur + 80` and swaps the `next` copy (cse.c, the REG0/REG1 swap after
    insn 7455). Eight statement orders gave the same RTL. Post-reload CSE
    (`reload_cse_regs`) can turn an operand into an equal register, which
    may explain the v0 uses. Update (2026-10-18 pm): the v0 load is reorg's
    copy rewrite (Diff patterns row), so the RTL loads `start` via `cur`.
    What is left is the `p + 80` on the temp plus `end` via `next`: the temp
    must outlive `cur` but not `next` (`make_regs_eqv`), which the fade
    statement's use order rules out for a single temp. 14 statement orders,
    6 dispatch shapes and a late use of `p` were tried (block note).
  - `f` and `cur` sit in each other's registers (target `f` s1, `cur` s2).
    global.c gives `cur` 13533 against `f`'s 9719, too far apart for lives
    to explain. If block 21's fix moves uses from `cur` to the temp, it may
    fix this too.
- **`func_8007F314` (engine pause menu, 5772 B), parked 2026-10-18 at
  structure 1147/1443** (swarm checkpoint was 1263; C under
  `#ifdef NON_MATCHING`, the `block` note lists what's left). Most of the
  diff comes from jump2's cross-jumping of the ~20 `ret = 0; break;` tails:
  the target keeps the copy in case 0 (`.L8007F704`) and every other tail
  jumps there. What I found, from `jump.c` (`find_cross_jump`) and dumps
  (`-dg -dJ`):
  - jump2 walks the code forward. The jump it is looking at gives up its
    tail to a matching copy, and the search starts with the jump most
    recently added to the target label's chain.
  - A tail can give itself up when `s3 = 0` follows a label or a branch
    around it. A tail that follows a call never can, so it is kept and
    collects the later ones.
  - Jumps to a label made during jump2 itself are never cross-jumped with
    each other (`INSN_UID < max_uid`).
  - An empty `do {} while (0)` after the switch stops the `break`s being
    threaded before reload (its LOOP_BEG note), so the first copy is the
    one kept. That gives the target's merge pattern.
  - Still to find: the C that puts an *existing* label on case 0's
    `ret = 0` block. Then case 0's `PlaySfx(0x192)` tail could merge into
    case 8's, and that block's `j` would get its delay slot from the target
    (the target keeps `s3 = 0` outside it).
  - Also verified here: a constant set in the same basic block as its use
    stays in a register (reload_cse makes `and v0,a1` / `sllv ...,s5`), so
    move `max = 4` / `n = 1` past the label in front of them.
  - Tools: `scratch/f314/` (ignored) has `rtl.js` (compact RTL from a cc1
    dump), `copies.js` (lists the `s3 = 0; j` tails before and after
    jump2) and the listing scripts.
- **Reading the gcc source beats guessing for tie-breaks.** `func_8007B330`
  matched (2026-10-18) once the rules in `tools/reference/gcc-2.7.2.3` were
  read and worked through by hand against the dumps. The method and every
  rule found so far are in MATCHING.md, "Reading the compiler's rules"; the
  skill, CLAUDE.md and the reference README now point at it. Use it next on
  `func_8007F314`'s tail-merge question (jump.c). It also matched
  `func_8008C824` (registers 25/155, global.c priorities) the same day.
- **Tooling ideas not done** (from the 2026-10-16 plan):
  - Pilots, no code: model tiers (`--model haiku --escalate-model sonnet
    --big-workers 0` on ~40 functions under 128 B, check the first matches'
    types by hand; an A/B of `--big-model sonnet`), and `--chain 3`
    (alternate with single jobs, compare `normal-chained` in `stats`).
  - A by-hand typing pass with `tools/scripts/field_survey.py` on the
    overlays with no struct coverage (ovl64, ovl65, ovl67: enums only).
  - Usage-limit result: not recognised (runs are supervised); if a limit is
    ever hit, keep that `worker.out` and match its result line then.
  - Optional: `try --regions`; a `%lo` relocation-target check in `try`
    (must find 0 over all matched functions before it blocks anything);
    idle permuter slots rotating over registers-stage checkpoints (~60 s
    turns); a long-lived container per slot (`tools/dk` overhead measured
    1.2 s, under the 1.5 s that would justify it).

- **main.exe parks (2026-10-16 retry run: 12 of 15 matched).** The three
  left parked (`func_800177F4`, `func_8001795C`, `func_800248A0`) were
  matched by hand the same evening, each from a clean form plus a
  `-dl`/`-dg`/`-dR` dump, none from the permuter's C. Read the dumps
  before permuting a register-only park.
- **Checkpoint ranking (decided 2026-10-14): stays paired, then blocks,
  then exact.** A sum of the three fractions would have kept a closer
  `func_8002E3D0` version (11/12 blocks, registers right, paired 106/109
  vs 109/110; its C is in that session's result note), but the user
  ruled it out: it would make progress too slow. Port such a version by
  hand instead.

Other:
- The gcc 2.7.2 units (`main/8002d488`, `main/8002e0f0`) are fully
  matched. Their toolchain differences, all in the Makefile and
  `aspsx_fill.py`: ASPSX's slot filling and load expansion; `8002d488` is
  compiled with `-fno-rerun-cse-after-loop` (global arrays keep `la` +
  offsets); and `refill_from_jump` redoes one slot fill where Sony's reorg
  differs from FSF 2.7.2's (it fills a `j` from its target before the
  branches; FSF 2.7.2.3's source is in `tools/reference/gcc-2.7.2.3/`). 2.7.2-cdk and 2.8.1-psx share Sony's reorg order but
  not its codegen, so a gcc 2.7.2 game elsewhere may need the same rule.
- `main/8002e0f0` with `-fno-rerun-cse-after-loop`: `func_8002E25C` stops
  matching (structure 44/95), the other four still match. Whether both
  units had the flag (and E25C's C leans on cse2) is open.
- Swarm: `--seg` is a prefix and defaults to `main/main`, so the new units
  need `--seg main/8002` (or `--only`).
- Heap: `D_8004E0F0` is a `Heap` now. Its +0x10 is the deferred-free list
  (`func_80011578` pushes, `func_80011DB4` counts down); `MemBlock.unk2` is
  the countdown. The heap functions still in asm (`func_800107C8`,
  `func_800115D8`) need prototypes taking `Heap *`.
- `func_800C8BA0` (ovl5f) matches alone but adds 4 bytes of data/rodata to
  the overlay (offset 0x4C): its literal belongs in another file's rodata.
- Prototype debug types (ARCHITECTURE.md, Prototype debug types): every type
  in the Oct 9 fragment is in a header now, and every single-id AST
  Init/Plot is named. Use them while matching: type `NAS` fields, `FUX`
  extensions and globals with them instead of offsets, as the four Tank
  Inits (`InitMineBall`, ...) now are. 34 fields of 15 types
  whose bodies were on lost pages are word or byte arrays; retype them if code
  shows their fields.
- `func_8001A0D8` (blocks `func_8001C7FC`) stores SXY at +0x10/+0x18/+0x20
  too: use `gte_stsxy3_10`.
- Sound voices: the C walkers use `SfxVoicePool.active`; the symbol
  `SfxActiveVoices` stays only for the asm (`func_800228C4` and data). Drop it
  from `sym.main.txt` once nothing in asm uses it. `func_80022504` is "stop all" (`StopSfx` inlined in a loop),
  `func_800228C4` unlinks without stopping, `func_80022924` allocates,
  `func_80022984` inits the pool.
- Ambiguous naming leads are listed in ARCHITECTURE.md ("Functions reached
  from the AST pairs").

## Gotchas

**Tooling**
- **A new prototype can break callers in other files.** The gate adds the
  matched function's prototype to the owned header; an already matched
  caller elsewhere that calls it with other arguments then fails to compile
  (`func_80085890`: two `func_80085890()` calls in ovl63/ovl65). Fix the
  calls (`grep -rn NAME src`), not the prototype; the full build proves the
  callers' bytes. The gate's message shows cc1's error since 2026-10-16.
- **Don't use `swarm.mjs run --dry-run` to preview a queue.** It records its
  fake parks in the swarm dir's `history.json`, which counts them as attempts
  and moves functions to big mode. If you ran one, delete the entries whose
  only notes say `dry run`.
- **`dashboard.html` holds the data placeholder once.** `progress` puts the
  data in place of every `/*DATA*/null` in the template (Python's
  `str.replace`): one in a comment doubled the page. Name it some other way.
- **A swarm match the gate refused on a compile error is usually still good.**
  (Fixed 2026-10-18: the gate-fix worker used to get only the output's last six lines,
  warnings and "Error 33", and called the build flaky; it now gets the error lines
  and a hint about union-merged headers, so this should be rarer.)
  Its C matched against an older tip, and other matches in the run changed the
  same header (a second typedef of one struct, a prototype with other
  parameters). Apply `results/<key>/*gatefix*/patch.diff` (or the big1 one) with
  `git apply --3way`, keep both sides of a header conflict, merge duplicate
  structs into the existing one, and call a function whose callers pass other
  arguments than its definition takes through a cast (MATCHING.md). Run
  `git update-index --refresh` first if `apply` says "does not match index".
- **Smoke-test swarm changes end to end** before a real run: a scripted
  `--worker-cmd` run against a local bare remote (`--remote <bare clone>
  --branch smoke --only KEY`) catches what the unit tests can't (clone setup,
  the queue's Python, patches through `git`); see the 2026-10-16 entries.
- **Claude Code allow rules for paths are `Edit(...)`.** They cover Write
  too; a `Write(path)` rule is ignored, so in `dontAsk` mode the write is
  refused. Check a permission change with a one-turn haiku session in a
  worker clone before a run.
- **Stale asm and `decomp.py index`.** splat rewrites `asm/nonmatchings/` only
  for functions still under `INCLUDE_ASM`; a matched function's `.s` is the
  one from when it was asm. `index` reads every `.s` there, so after a split
  the old unit's copies overwrite the new segment, and stale files carry old
  sizes (`func_80027408.s` is 404 bytes, `functions.json` says 244). After a
  split, move the old unit's folder aside, `make split`, copy the matched
  functions' `.s` into their new folders and update `functions.json`'s seg,
  src and asm for the moved functions (as the engine split did) rather than
  running `index` over everything.
- **Rodata migrated into a function.** When a matched function's .s held
  `.rodata` (splat migrated it there), the full build loses that symbol. Add
  `D_xxx = 0x...; // force_not_migration:True` to `sym.main.txt`, put
  `INCLUDE_RODATA(...)` where the function was, `extern char D_xxx[];`, and
  `make split` (`func_800235BC`). Don't run `splat split --disassemble-all`:
  it writes `asm/matchings/`, which the build assembles and fails on.
- **`make clean` (and `make verify`) delete all of `build/`** (since
  2026-10-16). Keep drafts and notes in `scratch/` (ignored); the game data
  dump is `assets/` (your edits: `dat.py extract` won't overwrite it without
  `--force`); splat's binary segments are in `asm/bin/` (`asset_path`).
  `disc/files/` must be complete for `make disc` (rerun `decomp.py extract`).
- **The build tracks its pipeline:** C objects depend on `dmpsx.py`,
  `aspsx_fill.py`, `project.mk` and the Makefile, and overlay linker scripts
  on their own `sym.<ovl>.txt` and `sym.engine.txt`, so editing those
  rebuilds or re-splits. `decomp.py` takes `CPPFLAGS`/`ASFLAGS` from the
  Makefile (`make print-VAR`). `try` prints `STILL-ASM` for a function whose
  INCLUDE_ASM is still in place instead of a self-match.
- **Swarm clones and splat output.** Clones get `asm/` and `linkers/` copied
  from this checkout; since 2026-10-14 they are copied again when
  `configs/USA`'s sym/rel files or yamls change (a fingerprint in the clone's
  `.git/swarm-data`). Run `make split` here after such a change, before a
  swarm run: the clones copy this checkout's `asm/`. Before the fix, a symbol
  split made every gate fail ("can't open asm/nonmatchings/.../func_X.s").
- **Toolchain image tag.** `tools/dk` and `tools/dk.ps1` tag the image
  `psx-decomp:<git hash-object of the Dockerfile, 12 chars>`, so a Dockerfile
  change builds a new image by itself; nothing to bump.
- Splitting a function by adding a symbol (`name = 0x...; // type:func` in
  `sym.main.txt`, no `;` in the comment): run `decomp.py index`, which adds
  the new function but keeps the old one's `size` in
  `.decomp/functions.json`. Fix that by hand, or `make diff` compares
  against the old length.
- A unit whose last function ends in padding `nop`s (and rodata ending in
  zero padding) comes out short once that function is C: add a splat
  `[offset, pad]` subsegment after it (`configs/USA/main.yaml` has the two
  for `main/8002e0f0`).
- The swarm refuses any patch that adds inline asm, even a GTE macro, and a
  big-mode function then parks as "no progress" though its sessions said
  MATCH. Read the parks' `result.json` notes after a run: `func_8001AA48`
  was such a case, committed by hand with its macro (`gte_ldv0_t`) next to
  `gte_ldv0_w` in `include/bash/main.h`, the precedent for game-specific
  GTE macros.
- The swarm: start it with the Bash tool's `run_in_background`, not `&`, and
  wait for `run <id> finished` in `swarm.log` (or the pid gone, by
  `tasklist //FI "PID eq N"`; `process.kill(pid, 0)` doesn't work for it on
  Windows). Don't wait on a bare `fatal`: git errors quoted in the log
  contain it. `swarm.log` timestamps are UTC, an hour behind local time.
- Git Bash heredocs can fail outright ("unexpected EOF while looking for
  matching quote") on long scripts with backticks and quotes: write the
  script with the Write tool and run it with `tools/dk python3 build/x.py`.
- Before 2026-10-13, `try` masked every relocated word, so a `j` to the
  wrong label inside the function still "matched alone" and only the full
  build failed. Local jumps are compared now; a "matches alone, full build
  doesn't" refusal from before then is worth a fresh `try`.
- Everything runs through `tools/dk` (Docker); the host has no Python.
- In shell loops, give `tools/dk` `</dev/null` (it runs `docker -i`). But
  never with a heredoc: `tools/dk python3 - <<EOF ... </dev/null` silently
  runs nothing.
- Git Bash heredocs collapse `\\` to `\`: write Python with escapes using
  the Write tool.
- A C error does not stop the build: the Makefile pipes `cc1` into maspsx
  and `as`, so the pipeline's status is `as`'s and a partial object gets
  linked (a missed field rename showed up only as a 20-byte-short exe). After
  header changes, grep the build log for `stdin:N:` lines that are not
  warnings.
- `tryf.sh` replaces only the function, so a comment line above it in the
  draft file is added again on every run. Keep comments out of drafts.
- `make clean`/`make verify` wipe `build/` (including `build/objdiff/asm`; rerun
  `make objdiff-config`). Keep scratch scripts in `disc/scratch/`.
- `tools/dk` right after `git checkout` sometimes hits a Windows
  `PermissionError`; retry.
- `make permute` runs in its own container: stopping the background task
  doesn't stop it. `docker stop` it before running `done`.

**decomp.py**
- `done` refuses when another function in the file is also un-`INCLUDE_ASM`ed.
  For a batch: keep the full file in `disc/scratch/`, `git checkout` the
  source, apply one function (`disc/scratch/apply_one.py FULL SRC SEGDIR
  FUNC`), `done`, repeat. `done` also applies the same C to byte-identical
  copies in the file and commits them together: skip those (apply_one asserts).
- `done`'s scope check compares names: commit renames before matching.
- `make diff` shows at most 40 rows. For the whole diff, or to swap a draft
  in and diff in one step: `tools/dk python3 disc/scratch/fulldiff.py FUNC`,
  `sh disc/scratch/tryf.sh FUNC FILE.c [LINES]` (main.c only).
- Giving a core function its prototype can break old `auto` matches that
  call it with no arguments (m2c dropped a pass-through `a0`). Pass what
  the argument register holds at the call; the code doesn't change
  (`KillNAS((NAS *)arg0)` in six overlay Init functions).
- A new prototype can also change a matched caller's code through the
  type it assigns to (`D_800C5BB0` had to become `NAS *`). Run `make check`
  after adding one.
- Register pins pass the lint as hacks (`--hack`) since `ef8fdc9`; before,
  `asm("$20")` was misread as inline asm.
- Renaming a function to a leak-declared name and back loses its owned
  declaration; re-add it by hand.
- `rename` prints `ERROR: unknown function OVL:ADDR` for data symbols; the
  rename still works.
- **Near-misses at the registers stage:** `swarm.mjs permute --only ...`
  before another Opus session. Checkpoints now: `func_8001795C` registers
  12/107, `func_800248A0` 5/142 (the permuter didn't move it in 25 min),
  `func_80015F60` 2/89.
- `functions.json` names can lag `sym.*.txt` renames; don't trust them for
  file paths.

**splat and the build**
- splat only writes `.s` files for functions a C file `INCLUDE_ASM`s, and never
  rewrites `src/`. After adding symbols to an overlay with no C yet, delete
  its scaffold `.c` and re-split. `make check` won't notice a stale object
  (`.c.o` doesn't depend on the `.s`): use `make verify` after any re-split.
- Packed segments need `align: 4` on every segment in the file.
- `make verify`'s splat "Unable to determine a segment" errors for
  `funcEvSpError`/`funcEvSpErrorx` are harmless.

**Matching and types**
- 185-odd `D_` globals used by unmatched exe code are still undeclared: the
  match that first uses one declares and types it in the owned header.
- Matched-but-mistyped functions (`void f(void)` from the old `auto`) are the
  likeliest type stalls: retype them when a caller needs it.
- splat marks functions with cop2 moves as `/* Handwritten function */`; they
  are ordinary C with Sony's GTE macros. m2c can't draft them.
- `bool` is 32 bits. `NAS+0x58` is a per-object pointer (`EXTobject` for
  Arkenoid balls, other types elsewhere): leave it `void *`.
- The leak's prototype order follows code order inside a file, so the
  neighbours of a named function are good naming candidates.
- AST entry: `{id, Init, Plot}`, `0x80026D00` is the empty stub; pointers
  below `0x800B32B4` are engine functions.
- The GTE table's `ll`/`lc`/`sqr` entries come from encodings, not a retail
  match: if one mismatches by a bit, check `tools/scripts/dmpsx.py` first.
- Grepping asm for `4A`/`4B` opcode bytes also hits ASCII in data ("MARK").

**Progress check:** `tools/dk python3 tools/scripts/check_progress.py` recounts
every unit from the committed sources, prints functions % and bytes % side by
side (the dashboard's headline is bytes), and checks `docs/progress.json`.

**Scripts** (`disc/scratch/`, not committed): `callsites.py OVL` (what each
call passes, whether the result is used), `callgraph.py spawns`,
`nas_fields.py ARGREG FUNC...` (field offsets through a pointer argument).
`tools/scripts/field_survey.py` (committed) counts loads/stores per struct
offset. Most-called functions:
`grep -rhoE "jal +func_800[0-9A-F]{5}" asm/nonmatchings | sort | uniq -c | sort -rn`.

## Session log

Newest first, a few lines each. Details are in the commits.

- **2026-10-19 (hand):** the id word is its own rodata unit in the other 13
  slot overlays too (95e2fa09, byte-identical), and ovl61's two `&&label`
  tables are `switch` statements again through the gate: `func_800B68C8`
  (2d811ef3), `Plot_N_Gin` (1b763319). No other first file had any.
- **2026-10-19 (hand):** `func_800B488C` (ovl6a, 1032 B) matched (ee24f281)
  with the swarm's real-switch C. The +4-byte link failure was the overlay id
  word sharing `ovl6a.c`'s rodata: split into its own unit (815cad97),
  byte-identical. ARCHITECTURE.md's slot-overlay layout says why; one Diff
  patterns row extended, one added.
- **2026-10-18 (hand):** `func_800B5710` (ovl6a, 2836 B) taken from the
  swarm checkpoint (529/721 differing) to 120/121 blocks and parked on one
  delay-slot fill in the 0x80 block (Open leads). Four Diff patterns rows
  (constant held across two calls, a spill decided by live length, base-first
  `ARR[i].field`, a switch on a global read once) and three examples added.
- **2026-10-18 (hand):** the swarm's three other gate refusals fixed and
  committed: `func_800BA43C` (ovl67, 067bc83b: a third, unused bool parameter
  like ovl61's ArkHeroRad2ObjectCollision), `func_800B3E74` (ovl67, c079de93:
  conflicting prototypes merged; `func_800B4138` takes one argument but this
  caller passes two, so a cast call), `func_800B7CE8` (ovl69, 3ba0a413: its own
  types dropped for the header's `Ovl69Entry`). New gotcha on the recipe.
- **2026-10-18 (hand):** `func_800B4EAC` (ovl6a) matched (36eb852e) from the
  swarm's big-mode C, which matched but failed the gate on compile errors: its
  D_8005A614 view (a second `Ovl6aGameView`) merged into the file's own, stale
  prototypes dropped, and `func_800B7B38` (u16 return) called through an s32
  cast as the file's other caller does. `func_800B551C` turned out matched by
  the swarm (9c5b1dd8). The swarm's other gate failures (ovl67
  `func_800BA43C`, `func_800B3E74`; ovl69 `func_800B7CE8`) are likely the
  same: apply results/<key>/*gatefix*/patch.diff and fix the headers.
- **2026-10-18 (swarm):** ovl6a run 20261018184703, `--workers 3 --big-workers 2
  --permuters 2`, 87 min, $47.98: 64 matched ($0.75 each), 2 failed at the
  gate. Fold: 37 rows (a volatile and a frame-padding workaround dropped).
- **2026-10-18 (swarm):** ovl69 run 20261018173050, `--workers 3 --big-workers 1
  --permuters 2`, 75 min, $33.21: 48 matched ($0.69 each), 1 failed at the gate
  (compile error against the run's newer headers). Fold: 20 rows. Then ovl6a.
- **2026-10-18 (swarm):** ovl67 run 20261018163029, `--workers 3 --big-workers 2
  --permuters 2`, 58 min, $39.26: 82 matched, 1 parked, 2 failed at the gate
  (compile errors against the run's newer headers; see Next). Fold: 32 rows,
  3 more examples on existing rows; a volatile workaround and a shared
  prototype change dropped.
- **2026-10-18 (hand):** `func_800B9CB8` (ovl67, 1924 B) matched (e4529709) by
  porting ovl61's `func_800BE4D4`: no grab path, its own reach boxes
  (`D_800C4190`; the hero's y read once into a separate `hy` for top/bot) and
  one combined whatami-2 test ending in `KillNAS`. Three fixes after the first
  compile: `hero` assigned after the near point, `hy` apart from the loop's
  `y`, and ovl61's `-b + a` get_angle arguments kept (the plain `a - b` I
  wrote while porting loads the operands the other way).
- **2026-10-18 (hand):** `func_800BE4D4` (2816 B) matched (9fd67e11), no hack
  or permuter, from the swarm's 699/704 paired. The sin indexes' register copies
  (only here and in ovl67's `func_800B9CB8` in the whole game) are `u16` copies
  of the angles made before `abs()`, one pair per block: combine.c refuses to
  merge across the call and sched.c moves block-local copies after it. Then the
  second sin/cos copy's roles filled with the variables global.c had already
  put in the target's registers, and dz/x1 for the height test. Takes a second,
  unused parameter (its matched callers pass two). MATCHING.md: two rows.
  ovl67's `func_800B9CB8` (the same code) was ported from it the same day.
- **2026-10-18 (hand):** `func_800B85DC` matched (5b63fbe1), no register pin
  or permuter, from the swarm's 182/226 (s-reg rotation only). Worked through
  global.c's priority (floor_log2(refs) * refs / live) per pseudo from -dl:
  the target's order lim > h > arg0 > arg1 needs `arg2 *= arg2` (7 refs, not
  4) and loop 2's refs loop-weighted, i.e. its goto-loop body in
  `do { } while (0)` (a real while/for/do loop lets loop.c hoist the (s16)
  casts: 135 diffs). Plus a3 for loop 1's own hero pointer, block-scoped
  Heroes bases, and `goto ret0` for the xori test. MATCHING.md: two rows.
- **2026-10-18 (hand):** `CalcArkInfluence` matched (b46ab0f8), no hack or
  permuter, from the swarm's 146/150 (z2 in v1, three big sessions and a
  permuter run stuck there). From the -dl/-dg dumps and global.c: x2/z2
  overlapped (all shifts before the adds) and z2 inherited a v1 preference
  from its product, whose mult operand was the block-local `(s16)sz` in v1
  (set_preference, then expand_preferences). One temp `t` reused with
  `x1 += t` fixed the order; an `s32 scale` assigned `(s16)sx` then
  `(s16)sz` (dies twice: global, so no inherited preference) put t in v0.
  MATCHING.md: two rows.
- **2026-10-18 (swarm):** ovl61 run 20261018125240, `--workers 1
  --big-workers 4 --permuters 2` (31 of the 34 left were >= 600 B), 128 min,
  $88.97 (in-flight sessions run past the $80 cap): 28 matched ($3.18 each),
  0 parked; ended by the spend cap. Before it, `func_800B7518` by hand from
  the last run's dropped permuter match (worker patch + `permute-apply`).
  `bigPermuteMinutes` default 60 -> 30.
- **2026-10-18 (dashboard):** `decomp.py progress` 10.8 s -> 2.4 s (every
  `done` and each swarm batch paid it): the typed scan (a regex per matched
  function, ~8 s) is cached per source file in `build/cache/typed.json`, and
  the rodata cache key's mtimes are read once, not per segment. Same summary.
  The page moved to `tools/scripts/dashboard.html`; it gets the history packed
  (columns, per-unit changes only) and the code map: 474 KB -> 136 KB, ~160 ms
  to load. New: per-day bars, a table view, a treemap of every game function
  (click a unit to zoom), units with stacked bars and sparklines, largest left,
  recent and parked lists. The swarm writes `docs/swarm.js` (`--page`), and
  the page polls it as a script (works from `file://`). Smoke run (scripted
  worker, bare remote) showed the card follow a run live; 2 new swarm tests.
- **2026-10-18 (swarm):** ovl61 run 20261018104310, 80 min, $35.51: 55
  matched (about $0.65 each), 1 parked, stopped by hand with 35 left (see
  Next). Gotcha: `run --dry-run` writes its fake "parked: dry run" results
  to `history.json` and charges attempts; I deleted its 89 entries before
  the real run (backup `history.json.bak-dryrun` in the swarm dir).
- **2026-10-18 (hand):** `PlotScriptFX` parked at regions 95/112 blocks,
  484/520 exact, no hack or permuter (the swarm had 87/112 with a dead
  store and volatile casts). C under `#ifdef NON_MATCHING`; the
  `decomp.py block` note lists what was solved (the hoisted 1 via `case 1`
  first and two `unk52 = 1` stores that reload_cse deletes, `mode` read
  through `EngineGameView`) and what is left (block 21's keyframe copies,
  `f`/`cur` in s1/s2). New `ScriptKey` struct. MATCHING.md: three rows, and
  the loop-rotation rule corrected (`while (1) { if (..) break; }` stays
  top-tested).
- **2026-10-18 (hand):** `func_80078DD4` matched (308bf0e), no hack or
  permuter, from the swarm's 156/166 checkpoint (stuck on a b/t `a3`/`v1`
  swap). global.c's `set_preference` gives arg 4's variable `a3`, and t can
  always take `v1`, so no ordering of b and t works. Reusing `dx` for b does:
  dx is live in the other branch while that call sets `a3 = 0`, so
  `prune_preferences` drops the preference. Plus the target's b-first/t-last
  arm order. `EngineGameView` names `unk4`.
- **2026-10-18 (hand):** `func_80093854` matched (8ef8052), no hack or
  permuter, from the swarm's 66/106 checkpoint. Worked through by hand from
  global.c, loop.c, stmt.c, sched.c and local-alloc.c: its nine `s` registers
  need the outer loop's refs weighted, but a real loop hoists `%hi(D_8009FA14)`
  into a tenth. A `goto` loop with its body in `do { } while (0)` gives both
  (loop.c skips the region as phony). Then a shared scan counter (it ties `p`
  and wins on pseudo number), and block 0's `D_80052A54` index before its
  local. `EngineRec32` names `unk4`. MATCHING.md: four rules, four rows.
- **2026-10-18 (hand):** `func_80095258` matched (99f2fe3), no hack or
  permuter. The sibling `func_80094A28`'s C was right but for a t0/t1 swap
  of the parameter and the loop's hoisted `%hi(D_800A0C70)` copy. From
  global.c, flow.c, sched.c and local-alloc.c: the copy's life is doubled
  (`update_equiv_regs`, a constant), so it ranked 5/42 against the
  parameter's 5/32. A `do { } while (0)` around the counted loop (not the
  NULL test) keeps the hoisted insns at depth 2: 8 refs against 6, matched
  on the first compile. MATCHING.md: global.c rules and a Diff patterns row.
- **2026-10-18 (hand):** `func_8008C824` matched (d3e79ab) from the swarm's
  25/155, no hack, no permuter. global.c ranked the two loop givs 27/77
  against 25/76. A doubly nested `do { } while (0)` around the loop's first
  five statements gives the spark giv's store depth 4. Splitting
  `a -= Random()` makes the first insn after the notes a cost-1 `subu`.
  Diff patterns row; global.c and loop-note rules added to "Reading the
  compiler's rules".

- **2026-10-18 (hand):** `func_8007B330` matched (7233c73) from 7/307, no
  hack, no permuter. sched.c's birthing and tie rules gave per-block index
  variables plus a twice-assigned Heroes base. local-alloc's tie rule gave
  separate panel pointers, each panel's stores in `do { } while (0)`. Two
  Diff patterns rows.
- **2026-10-18 (hand):** `func_8007F314` (5772 B) taken from the swarm's
  1263/1443 to 1147/1443 and parked (Open leads). How jump2 merges the
  shared `ret = 0` tails is now worked out; still missing is the C that
  gives case 0's copy an existing label.
- **2026-10-18 (hand):** `func_800926CC` matched (01be8b2): `j = 1` inside
  `do { } while (0)` beats sched1's birthing priority and wins `j` a0. No
  parks left in the engine; 7 functions to go.
- **2026-10-18 (hand, third try):** `func_8008A924` matched without a hack
  (9787456). The target's unused `lbu` is half of `run_walk = run_walk`;
  post-reload CSE deletes the store. A `do { } while (0)` around the read
  gives the reloaded pointer loop-weighted refs, so it keeps `v0`. Two new
  Diff patterns rows.
- **2026-10-18 (hand, second try):** `func_8008A924` still parked. A `$3` pin
  matches (not committed: a workaround). A dead read gets the dying base's
  register, so the original's value was live at allocation; an if/else whose
  arms merge in jump2 drops the read too (delete_computation).
- **2026-10-18 (hand, no permuter):** `func_8008A924` (120 B) parked at
  registers 1/30. Its dead read of `run_walk` must be volatile and lands in
  `v0` (the dying base) where the target has `v1`; the block note lists what
  was tried. The permuter ran in parallel on the three register-only engine
  near misses: `func_800939FC` matched in 5 min (c7cf408); `func_8007B330`
  (registers 7/307) and `func_8008C824` (25/155) made no progress in 20 min
  before it was stopped.
- **2026-10-18:** `decomp.py hacks`: matched functions that need a register
  pin or asm barrier, with their match commit's `Hack:` reason (read from
  the source, so it can't go stale). CONVENTIONS points at it.
- **2026-10-17 (hand, no permuter):** `func_800926CC` from the swarm's 4/134 to
  1/134 and parked. The misplaced `li a0,1` was sched1's birthing priority on a
  single-set pseudo; `s8 j` avoids it, but post-reload CSE then turns the
  compare's `li a1,1` into `move a1,a0`. The target needs a HI/QI `j` set twice
  with the extra set costing nothing; the `decomp.py block` note lists what was
  proven and tried. New Diff patterns row for the birthing/SUBREG trick.
- **2026-10-17 midday (engine swarm):** runs 20261017113732 (60 min, 10
  matched, $63.10) and 20261017124451 (15 min, killed at the end of the
  session: 2 matched, $6.37; `func_8007F314` and `PlotScriptFX` sessions
  cut short). 0 parked. Folded 15 rows from matched functions.
- **2026-10-17 morning (engine swarm):** run 20261017101444, 60 min: 23
  matched (688-1580 B, $65.81), parked `func_8008C824`. Docker and the login
  were refreshed before launch; no failures. Folded 23 rows from matched
  functions (dropped 4 from unmatched ones).
- **2026-10-17 (engine swarm):** run 20261017064912, stopped at 38 min
  (internet down): 9 matched (688-948 B, $28.30), parked `func_80078DD4`
  and `func_800926CC`. The first launch failed in seconds: the OAuth token
  had expired overnight and 4 workers raced to refresh it (uncharged, but it
  writes error notes into history.json that feed later prompts; removed by
  hand). Gotcha: after a long idle, run `claude -p "reply ok"` once before
  `swarm.mjs run`. Folded 9 rows from matched functions.
- **2026-10-16 night (engine swarm):** run 20261016213103, 60 min, 1 normal
  + 4 big workers: 19 matched (incl. `InitHealth`, `PlotHealth`), 0 parked,
  $56.63. The two small engine functions left (`func_80095258`,
  `func_8008A924`: inline asm) are at max attempts, so the normal worker
  idled. Folded 18 pattern rows from matched functions; dropped rows from
  still-unmatched functions and `func_8002D4F4` duplicates.
- **2026-10-16 (docs, cleanup):** ROADMAP's "match every game function in
  the executable" ticked; README, ARCHITECTURE, MATCHING and CLAUDE.md
  updated for the 2.7.2 units' settings and `tools/reference/`. Fixed the
  2.7.2 unit's `EXTRA_CC1FLAGS` override dropping `-fno-builtin`.

- **2026-10-16 (reference):** FSF gcc 2.7.2.3's C compiler and MIPS
  target source added under `tools/reference/gcc-2.7.2.3/` (read-only,
  vendored, GPL; `README.decomp.md` maps passes to files).

- **2026-10-16 (by hand + tool, `func_8002D4F4`):** matched (710a20d);
  main.exe is complete. The last slot (state 6) was the compiler, not the
  C: read gcc-2.7.2.3 `reorg.c` (now in `tools/reference/gcc-2.7.2.3/`) and
  found FSF reorg fills a `j` after the branches, Sony's (like 2.7.2-cdk
  and 2.8.1-psx on a lab copy) before them. `aspsx_fill.py`'s new
  `refill_from_jump` redoes that order for a `beq` + `j` (07750fc, 5 tests);
  every matched function of both 2.7.2 units still matches, `make check` OK.

- **2026-10-16 (by hand, `func_8002D4F4`):** parked at 145/148 (was 144,
  length 592 -> 590). Found that the 2.7.2 unit compiles without cse2
  (`-fno-rerun-cse-after-loop`, Makefile; `CC1FLAGS` is now `=` so
  per-unit `EXTRA_CC1FLAGS` applies): array accesses through a constant
  base keep `la` + offsets, the `D_8006788D` symbol is just
  `D_8006788C[1]`, and `func_8002DE2C` matches without its always-zero
  offset. `make check` OK. Left: one reorg delay-slot choice in state 6,
  which turned out to be a compiler-build difference (Open leads). No
  permuter.

- **2026-10-16 (by hand, `func_800248A0`):** matched (03bef8c), no
  permuter. The caller passes x and y as shorts (`lh`, `sll/sra`), so the
  definition is K&R with `short x, y;`. gcc copies a0 at entry and emits the
  short conversion into s5; combine makes the test read the entry copy, which
  stays local in a0. The earlier attempts' `s16` prototype or local put the
  s5 move after the a2/a3 copies (Diff patterns row). main.exe: only
  `func_8002D4F4` is left.

- **2026-10-16 (by hand, `func_8001795C`):** matched (cfeda67), no
  permuter. Started from the sibling `func_80017EE8`'s arm form (clut
  stored in each arm, cross-jumped) on the checkpoint's setup: 6/107, a
  tie in local-alloc priority between the `face->tex` and `mc->uvs` offset
  temps (`-dl`). Setting `ts = &model->texs` early took `ts` out of
  `t = *tex`'s load-delay slot at the block end, so the tex temp dies
  sooner and wins a3 (Diff patterns row).

- **2026-10-16 (by hand, `func_800177F4`):** matched (c8e99ad), no
  permuter. The target's tail (both loads per arm, shared `andi/or` and
  `sw/sh` behind labels) is cross-jumped per-arm stores: writing the store
  in each arm makes tpage arm-local in a0, so idx's a0 preference is
  pruned and the face-first add allocates as retail. The earlier session
  had chased the preference with carrier variables (Diff patterns row).

- **2026-10-16 (tool, ASPSX conditional-branch slots):** `aspsx_fill.py`
  fills the slots of branches gcc 2.7.2 leaves to the assembler: from
  below, else from the target (or a `j`'s slot there), with an
  instruction whose register is dead on the other path (path-following
  liveness; a load's write doesn't count; a return reads everything); a
  target instruction every branch took and nothing falls into is dropped.
  Replaces `steal_jump_slot`. Every matched 2.7.2 function still matches;
  `func_8002D4F4` 126 -> 142/148 blocks, then a big-mode run (4 sessions,
  $9.63) took it to 144/148 and parked it (Open leads). Swarm fix: a big
  session that errored before any work (an OAuth refresh failure) was
  measured as "no progress" and parked the function; now an uncharged
  failure (fa40cae).

- **2026-10-16 (swarm, main.exe retry, run 20261016133425):** `decomp.py
  unblock` (new) unparked main.exe's 15 parks; big mode only, no
  permuter, 90 min / $100 cap. 12 matched in 75 min for $53.46:
  `func_8002E25C`, `func_800155FC`, `func_80010488`, `func_80026D08`,
  `func_80011E80` (refused once by the gate after a neighbour's commit,
  redone in 1.6 min), `func_80020C04`, `func_80010C70`, `func_80010E8C`,
  `func_8001A82C`, `func_80020680`, `func_80017164`, `func_80010000`.
  Re-parked: `func_800248A0`, `func_8001795C`, `func_800177F4` (1/89).
  `func_8002D4F4` to 126/148 blocks; its rest needs `aspsx_fill.py`.
  Folded 16 Diff patterns rows.

- **2026-10-16 (assets: the rest):** every DAT entry but the 15 code
  overlays is decoded. Models: all 400 (16-byte vertices were normals,
  +0x28; 27 have no meshes), texture coordinates and colours from main.c's
  func_80017EE8/func_80017D90 (UV table at 0x24 + [0x24], colours at 0x20 +
  [0x20], texture runs; image N of the set before the model), and
  animations (func_8001C0F0: keyframes as index lists into a vertex pool)
  exported as model.glb (morph targets, KHR_mesh_quantization; 259 MB).
  Music: SEQ <-> MIDI, all 57 byte for byte. Checked in a three.js viewer
  (Uka Uka talking). Re-extract from the disc + disc-check OK.

- **2026-10-16 (assets: sound packs):** `sfx.py`: all 160 packs to
  `<id>.sfx/` (bank.vh/.vb, music_N.seq, a WAV per sample) and back byte
  for byte; an edited WAV is ADPCM-encoded (55-70 dB SNR), keeping its
  loop/one-shot shape. Offset table: VB, VH, SEQs (the last runs to EOF;
  effects packs have one empty SEQ). 241 raw -> 81 (the odd models).

- **2026-10-16 (disposable build/):** `make clean` deletes all of `build/`;
  `build/scratch` -> `scratch/`; splat's `asset_path` -> `asm/bin` (the
  Makefile had been building all 992 DAT files from `assets/` into
  objects); `pack_disc` uses `disc/files` (no `build/disc-cache`);
  `decomp.py extract` also extracts `assets/` (never over edits). Checked by
  deleting build/, asm/, linkers/, assets/ and running the Quick Start.

- **2026-10-16 (assets: screens, text):** `tga.py` (5 title screens, plain
  TGA, to PNG) and `txt.py` (12 modes' text, strings in fixed slots
  because data points at them by address; four files keep a cut string's
  tail after its terminator) round-trip byte for byte; disc-check OK.
  `BASHY.` is 30 MB of zeros (filler), `SPYRO3/` the bundled demo: all of
  Crash Bash's data is in the DAT. Next: the 160 sound packs (VAB/SEQ).

- **2026-10-16 (assets: models):** `tools/scripts/mdl.py`: model files'
  meshes to OBJ (`<id>.mdl/`, geometry only) and back byte for byte, 319
  of 400 (CTR-tools' BashMesh reading; the other 81 differ); `dat.py` has
  a FORMATS table and saves index.json however decode ends. disc-check OK.
  CTR-tools' file-name list (GPL-2.0, the repo is private: the user said
  commit it) is `configs/USA/dat_names.txt`; names go into index.json.
  Next: texture coordinates (per-face arrays), sound banks.

- **2026-10-16 (assets, stage 2: textures):** `tools/scripts/tex.py`: the
  400 texture sets (type 8) to PNGs (`<id>.tex/`, true colours) and back
  byte for byte; `dat.py extract` decodes them, `make disc` packs them
  (cached in build/dat-cache/); disc-check OK; Pillow in the Docker image.
  Image->CLUT pairing and depth from CTR-tools' reading (the s16 at +12 is
  CLUT << 1 | 8-bit). Formats in the new docs/ASSETS.md; next: models (the
  odd entries), sound banks, text.

- **2026-10-16 (assets, stage 1):** `tools/scripts/dat.py` splits
  `CRASHBSH.DAT` into `assets/CRASHBSH/` (992 entries, not 993; plus
  `filler.bin`: entry padding and 12 unreferenced sectors are not zeros),
  and `make disc` builds the DAT from it (extracting first if missing);
  `disc-check` OK. Stage 2, decoding formats (textures, sounds, models), is
  not started.

- **2026-10-16 (supervised engine run):** 60 min, $41.11, 28 matched
  (`func_80085890` by hand: callers in ovl63/ovl65 passed no argument).
  Fixed: `checkPatch` read an added blank line before a context
  INCLUDE_ASM as an add (`s*` spanned lines) and refused
  `func_800939FC`'s best session; `verify_build` hid cc1's errors.
  Folded 67 pattern rows (dropped a K&R-to-dodge-callers hack and an
  unsolved row).

- **2026-10-16 (tooling plan done):** `--chain N` session chaining (off by
  default); a scripted run checked the stale-checkpoint hand-off, keeping a
  park's C and chaining. Every Part 1-3 code item is in.

- **2026-10-16 (tooling plan 3.4-3.9, paused):** `next` asm with offsets,
  jump tables, strings, callees; `try` LINK check and disassembly at the
  first difference; faster tools/dk/make/regions; permuter keeps GTE macros
  and applies in-run matches; Dockerfile pins maspsx/m2c/permuter (make
  verify OK); stall nudge; `splitcheck`; `port`.

- **2026-10-16 (tooling plan 3.2):** parks keep their closest C as the
  function's checkpoint and every attempt resumes from it. A `--worker-cmd`
  smoke run caught two 3.3 bugs that stopped runs at startup (no `build/` in
  a new clone; the queue copy couldn't import `regions`); fixed.

- **2026-10-16 (tooling plan 3.3):** `similar --survey`; `next` prints a
  matched look-alike's C (seq >= 0.90); queue `twin` field (exact / near /
  template) keeps adapt jobs out of big mode; swarm seeds cross-overlay
  identical groups and holds back a tried lead's twins.

- **2026-10-16 (tooling plan 3.1):** a worker session's git status changes
  every job and cost ~11k tokens of cache writes per session; workers now run
  without git instructions, memory, skills, TodoWrite. Prompts carry the loop
  inline. Real-session probe: the CLI ignores `Write(path)` allow rules, so
  1.8's result-file rule is now `Edit(.swarm-result.json)` (gotcha below).

- **2026-10-16 (tooling plan 3.5):** `try`/`make diff` show target and
  compiled aligned over the whole function (`regions.diff_rows`: register,
  operand, moved, missing, extra) with a `KINDS` count, identical runs cut;
  `DIFF`/`STAGE` lines unchanged. `regions` caps its table at 12 blocks
  and has `--changed`; the big-mode prompt mentions it.

- **2026-10-16 (tooling plan Part 2):** workers get `--session-id`; killed
  sessions are priced from their transcripts (prices fitted from 745 result
  lines, within 2%); `ledger.jsonl` (backfilled for 760 sessions, $453) and
  `stats` $/match by size and mode. Docs are read ~0.14 times per big
  session; cache writes are 34-49% of spend. Usage-limit detection deferred.

- **2026-10-16 (tooling plan Part 1):** all nine items, one commit each,
  with `tools/swarm/swarm.test.mjs` (node:test, 32 cases) and new
  `test_queue.py`/`test_lint.py`. Slot-overlay names go to tools as keys
  (`ovl5d:800B3A38`), checkpoints/results keyed by key; `reapply` keeps
  whole declarations; environment errors stop runs uncharged; unapplicable
  checkpoints get ported; escalation and notes carry across runs; asm-only
  matches go to report.md for a human `--hack`; gate lint covers every asm
  spelling and K&R definitions; workers are sandboxed from the host.
  propagate: `decl_name` misread function-pointer externs, which hid
  `D_8009D73C` from `ovl64:800BB52C`'s copy; fixed, and it matched
  (1021). Checked with a scripted `--worker-cmd` run on a local bare remote.

- **2026-10-16 (review, no code):** compared decomp.wtf's tools with ours;
  plan in TOOLING_PLAN.md (done, removed after 94f04c7). Found swarm bugs: shared overlay names are
  ambiguous to `find_func`, checkpoints keyed by name, `reapply` drops
  header lines (reproduced), Docker loss not detected, killed big sessions
  uncosted, workers can rewrite `tools/dk` on the host.
- **2026-10-15 (evening, swarm):** two engine runs, both stopped early by
  the user: 20 + 29 matched (1020/2551 now, engine 101 left), 1 parked
  (`func_80095258`, then retried). Stopped permuter runs kept their C in
  the swarm `results/` (`func_8007976C`, `func_80094A28`, `func_8007D208`).
- **2026-10-15 (tidy):** removed the replay harness (`replay.py`, recordings,
  `make replay`, the emulator core, ffmpeg and numpy in the image) and
  `make report` (decomp.dev's format); play instructions moved to
  `docs/PLAYING.md`. The user tests builds by playing them.
- **2026-10-15 (by hand, wrap-up):** no other positive split signal: no
  overlay or engine string stored twice, no 2.7.2 style in any overlay;
  main.c's proven boundary has three possible places. Docs brought up to
  date; next session is matching.
- **2026-10-15 (by hand, slot overlay splits):** `ovl5d` (two cuts), `ovl5e`,
  `ovl66`, `ovl67`, `ovl6a` split at their proven starts like the engine;
  headers `bash/<ovl>_<unit>.h`; `make check` byte-identical.
- **2026-10-15 (by hand, engine split):** the engine overlay is six files
  at its five proven starts (functions 33, 74, 208, 282, 330), each with
  its rodata block and owned header; `make check` byte-identical. Swarm
  checkpoints for engine functions were made against `engine.c` and may not
  apply any more.
- **2026-10-15 (by hand, file boundaries 4):** positive evidence at last:
  switch tables keep one address parity per file (`.align 3` relative to
  the object, objects 4-aligned), so a parity change proves a new file.
  `decomp.py units SEG` prints them for any segment: the engine has five,
  main/main one, `ovl5d` two, `ovl5e`/`ovl66`/`ovl67`/`ovl6a` one each.
- **2026-10-15 (by hand, file boundaries 3):** `units --data` adds the call
  graph (84 probably-static functions tie their callers' file): 17 possible
  cut zones, eight one or two functions wide, none proven. No byte-identical
  copies apart from neighbours, so no positive evidence; the split stays on
  hold.
- **2026-10-15 (by hand, file boundaries 2):** mapped every game `.data`
  label to its code users (overlays too) and table pointers. Private data out
  of code order proves shared files: `units --data` now lists eight
  probable one-file regions; the boundaries lie in the gaps between. The
  first data object is a shared tables file; the trig table is a data-only
  object linked among the libraries.
- **2026-10-15 (by hand, file boundaries):** `decomp.py units --data`
  groups the data each run of functions uses alone into address blocks;
  they climb along the code in one sequence (ARCHITECTURE.md, Files inside
  `main/main`), which maps regions but no exact edge. The prototype exes
  hold no source names, and Oct 9 has retail's link order.
- **2026-10-15 (swarm, engine overlay, run 20261015132721):** `--max-functions
  500`, stopped by hand after 43 min: 59 matched, $27.60, 1 failed
  (`func_8008A924`, patch refused for inline asm). Big-mode checkpoints for
  `func_8007BFA8` (28/114) and `func_8007C170` (17/114). Folded 37 Diff
  patterns rows (dropped a register-pin row).
- **2026-10-15 (swarm, engine overlay, run 20261015121127):** 38 matched,
  $16.46; leasing stopped at the 40-lease cap after about 30 min.
  `func_80093854` parked in big mode (instructions 53/106; checkpoint
  kept). Folded 25 Diff patterns rows.
- **2026-10-15 (swarm, engine overlay, run 20261015110034):** first swarm
  run on an overlay: 45 matched in 60 min, $21.72, no parks; gate
  refusals went back to workers as usual. `func_80093854` has a big-mode
  checkpoint (instructions 53/106). Folded 26 Diff patterns rows. The
  `main.c` split is on hold: the evidence (ARCHITECTURE.md, Files inside
  `main/main`) groups functions but places no boundary.
- **2026-10-15 (swarm, run 20261015090944):** `--only` the 8 untried big
  functions, `--big-workers 8 --big-minutes 27`, 60 min, $40.28: 7
  matched (`func_8001492C`, `func_8002926C`, `func_8001B6D4`,
  `func_80019094`, `func_8002C97C`, `func_8001AF2C`, `func_80023A90`), most
  in one or two sessions. `func_8002D4F4` reached structure 495/598. Folded
  12 of the workers' 15 Diff patterns rows (dropped an empty-`asm` hack,
  an unverified `volatile` row and a duplicate jump-table row).
- **2026-10-15 (by hand, the seven untried small main/ functions):** 5
  matched: `func_8001E21C` (key interpolation; `func_8001E41C`'s search as
  a static inline), `func_8001231C` (loader tick; three buffer variables by
  role), `func_800235BC` (arena setup; rodata kept via
  `force_not_migration`), `func_8001E610` (mode switcher; new `ModeMgr`,
  `GameModeDef`), `func_8001F604` (emitter spawn). 2 parked:
  `func_80010000`, `func_80017164` (Open leads). Only 8 big functions are
  left untried in main.exe.
- **2026-10-14 (by hand, main/, no permuter):** 6 matched: `func_800274FC`
  (returns a value; `OtBuf` type for `D_8005B790`), `func_8001C7FC` (void),
  `func_8001EAA4` (checkpoint plus `k = Find(); key = k;`), `func_80029560`,
  `func_8002854C`, `func_80028774` (sibling bodies as static inlines). 4 new
  parks within 2-18 insns (Open leads). `similar --all` on every main function:
  the overlays share no code with it; the matched siblings in main are the
  leads.
- **2026-10-14 (by hand, parks):** ported the stale checkpoints of
  `func_800248A0` (5/142) and `func_8001795C` (12/107) to today's types
  and parked both (Open leads); neither moved by hand.
- **2026-10-14 (by hand, `func_80028D40`, 488 B, no permuter):** matched
  from the checkpoint (17/122). The uv low byte is a `u16 : 8` bitfield
  (`TexUV.f`), the 8-bit tpage is `getTPage(1, 1, x >> 7 << 7, y)`, and the
  clut id goes through `cl = &set->cluts[i]`.
- **2026-10-14 (by hand, `func_80013D54`, 496 B, no permuter):** matched
  from the swarm checkpoint (6/124, a yz/zz register swap). The nine
  rounded products share one temp: `t = FIX12_MUL(..); var = t;`. Found
  from the greg dump: zz (two sets, short life) was allocated first and
  took the free `a0`. The checkpoint's `Fix12Mul`/`Fix12Round` inline
  helpers are gone from `main.h`.
- **2026-10-14 (by hand, `func_8002E3D0`, 436 B, gcc 2.7.2 unit, no
  permuter):** matched. `bits` is two `u16` halves: `lo = bits;` then
  `hi = bits >> 16;` (copy first, so `bits` dies at the `srl` in `a3`;
  `u16 hi` moves the `srl` early through sched2's hazard tie-break), the
  swarm's `bg = (u16)b; bg >>= 16;` split, and the plain `b = lo; b >= hi`
  loop. Got there with `vars` (register homes) and the cc1 `-dS -dR`
  dumps. `func_8002D4F4` not started.
- **2026-10-14 (by hand, `func_8002DE2C`, 468 B, gcc 2.7.2 unit):** matched
  from the checkpoint (2/116; 60 permuter minutes had found nothing).
  `vars`: a temporary, `splitvars`: no gain; ~16 rewrites, then the
  in-place mask `(t = buf[0], t &= 0x10, !t)`; the last slot was ASPSX
  retargeting a branch to a filled `j`, now in `aspsx_fill.py`
  (`steal_jump_slot`, all 16 binaries still match). Worth re-trying the
  other 2.7.2-unit functions (`func_8002E3D0`, `func_8002D4F4`) with it.
- **2026-10-14 (permute --near, 60 min, CPU only):** `func_8002DE2C`
  unchanged; `func_800248A0` and `func_8001795C` skipped: their
  checkpoints no longer build after the day's matches (the skip message
  now says so). A planned permuter A/B was cancelled.
- **2026-10-14 (swarm, main/, 60 min, $54.29):** 10 matched (`func_800243A0`,
  `func_8002BEA8`, `FreeExtension`, `func_80012678`, `func_800180BC`,
  `func_80028F28`, `func_8001DAF8`, `func_8001D6B4`, `func_8001FE80`,
  `func_80017EE8`), 0 parked. Workers used `vars` in 11 of 22 sessions,
  `splitvars` in 6, `similar` once; old checkpoints were measured; no
  patch refused. Fixed after: `regions` exit code, cc1 dumps in build/.
  Folded 41 pattern rows (dropped 2 asm-alias / macro-wrapper rows).
- **2026-10-14 (swarm check, 5 min, $2.9):** 3 big functions with the new
  tools. No errors or refused commands; workers used `regions` (no `make
  diff`); sessions too short for similar/vars/splitvars. Fixed after it:
  workers stopped with minutes left ("only 5 min"): the prompt now says a
  kill at the deadline is normal and the result file is kept current;
  `--retry-failed` ignored the stale rule (now a fresh allowance per run);
  old checkpoints without a region score were compared by STAGE and
  dropped `func_80010488`'s better session (now measured once; that
  session saved by hand: 22/37 blocks); session numbers in the log agree.
- **2026-10-14 (tooling):** five new `decomp.py` commands, each tested on
  `func_80019A60`: `similar` (matched functions by opcode 4-grams; ranks
  `func_8001EAA4`'s sibling first), `splitvars` (all groupings of a
  variable's assignments, scored by regions: found the 3-way split in 10 s),
  `permute --differing` (PERM_RANDOMIZE on the differing blocks' C: block 0
  matched in 2 of 4 runs of 2-3 min), `vars` (the variable behind each
  register swap), `units` (split evidence; see Next). Fixed the regions C
  line map (cpp -P squeezed blank lines). Swarm: `permute --near`.
- **2026-10-14 (by hand, `func_80019A60`, 1212 B):** matched from the
  swarm checkpoint (55/59 blocks). The s32 first parameter declared
  directly (no macro wrapper; both callers still match); the permuter,
  now able to run on it, found the 0x8A field read into a local first
  (56/59); the last 3 blocks needed three index variables instead of one
  (gcc 2.8.1 gives a pseudo one register for its life, and the target had
  a1/a1/v1). Two Diff patterns rows.
- **2026-10-14 (hard-function test, ~45 min, $10.82):** `func_80019A60`
  (1212 B, parked at 37/59 blocks by the old STAGE ranking) with the new
  defaults: 5 sessions, each a new best: 37 -> 37 -> 38 -> 46 -> 48 -> 55/59
  blocks, paired 280 -> 303/303, STAGE structure 291 -> registers 17/303.
  Sessions 1-2 raised STAGE's count (291 -> 301) and would have been
  dropped, and the function parked, under the old ranking. The jump to 46
  blocks came from a `static inline` draw helper shared by two paths.
  Stopped by hand during session 6. Loose ends fixed after: the permuter
  never ran (`func_def_regex` missed the parenthesized `void (name)(...)`
  definition; now matched, permute starts); sessions ended early because
  the fix they needed (the function's own u16 prototype) was outside the
  prompt's scope, so workers may now fix their function's declarations
  and are told to move on to the next block when blocked; a `#define`
  named after the function is refused; the STOP file is cleared.
- **2026-10-14 (swarm A/B round 2, 60 min):** 4 pairs of fresh 908-1212 B
  main functions, 27-min sessions, $29.56: 7/8 matched (regions arm 4/4,
  control 3/4). Both rounds: regions 6/7 in ~66 session-min/$17, control
  5/7 in ~85 min/$20; n too small to call. Control workers found `regions`
  in the docs and used it on `func_80019A60` (parked: STAGE 291/305, best
  37/59 blocks); its session 4 lost 10 blocks at the same STAGE count, so
  STAGE can't rank checkpoints there. Sessions still end early (1-19 of
  27 min). Next: regions as the big-mode default, checkpoints ranked by
  paired/blocks too, then an A/B on 1400+ B and parked functions.
- **2026-10-14 (swarm A/B, 30 min):** `--regions` arm vs control, 3
  size-matched pairs of fresh 828-868 B main functions, one 25-min big
  session each, $7.78. 4/6 matched (2 per arm: `func_80020344`,
  `func_80024AD8` regions; `func_8001A0D8`, `func_80029F9C` control).
  Left: `func_8001EAA4` (regions arm, paired 213/214, one missing move after
  the key-search loop; checkpoint kept) and `func_80010488` (control, paired
  187/208). Inconclusive: too easy, n too small, and every session quit
  after 2-9 min of its 25. Next round: harder functions (900+ B or parked),
  several sessions each, and tell big workers to use their time.
- **2026-10-14 (tooling: regions):** new `decomp.py regions NAME`
  (tools/scripts/regions.py, tests in tools/scripts/tests): aligned,
  block-by-block score with C lines. Re-scored 22 saved swarm patches:
  it localises (`func_80028F28`: 6/7 blocks ok, STAGE said structure
  59/118) and `paired` spotted a near-structure version STAGE called a
  regression (`func_800248A0` 110/144 -> paired 140/144); register
  cascades still mislead both. Not yet in the swarm; next: an A/B of big
  sessions with and without it.
- **2026-10-14 (by hand, roadmap order):** `func_80020C04` (main) parked
  at `structure` 18 vs 19 after compiler experiments (Open leads); the
  permuter was stopped early when the session ended.
- **2026-10-14 (by hand, 1 function):** `func_800C8870` (ovl5f) matched first
  try: an `s32` grid lookup `D_800CA34C[(y >> 9) * D_800CAFF4 + (x >> 9)]`,
  the same shape as its `u8` neighbours `func_800C891C`/`func_800C8950`.
- **2026-10-14 (permuter only, ~25 min):** two parallel `decomp.py permute`
  runs (7 jobs each) from the swarm checkpoints. `func_8001795C`: registers
  17 -> 14/107 (`ti = ti & 0x1FF` inside the index; `r0 = r0 | 0x02000000`
  instead of `|=`), saved as its checkpoint. `func_800248A0` (5/142): no
  improvement. Now one command: `swarm.mjs permute --only f1,f2
  --minutes 30` (tools/swarm/README.md); its first test took
  `func_8001795C` from 14 to 12/107 in 3 minutes on 15 cores.
- **2026-10-14 (swarm, 10 min):** no match. `func_8001795C` reached the
  registers stage (17/107, checkpoint); `func_800248A0` stayed at registers
  5/142 (2 stale sessions: big mode won't lease it again; it wants a long
  permuter run or `forget`). `func_8002E3D0`'s session was hard-stopped at
  19 min and lost. Big sessions run 13-20 min, so a run shorter than ~20
  min mostly ends in a graceful wait or a hard stop that loses work.
- **2026-10-14 (swarm, 15 min):** `--big-bytes 400 --workers 1
  --big-workers 3` (with 400, 52 of 54 candidates are big: give big mode
  the workers). `func_8002E000` matched in 3.5 min after 4 earlier fails
  (the 2.7.2 load fix). Checkpoints: `func_8002E3D0` 107/111 and
  `func_8001795C` 106/108 (structure). $9.68. A first start died on
  `tools/dk`'s image tag (`git -C` with a /c/ path under Windows git.exe;
  fixed in `6dda356`).
- **2026-10-14 (tooling):** Make deps on the pipeline scripts and overlay
  sym files; `try` prints `STILL-ASM`; flags from the Makefile; image
  tagged by the Dockerfile's hash (old images deleted); `make clean` keeps
  `build/scratch`, `emu`, `disc-cache`; disc cache. Swarm: stops at the
  first environment error without charging the function; `fold` (after-run
  step 3) and `forget` (reset functions a fix unblocked: done for
  `func_8002E000`, `func_8002E3D0`, `func_8002DE2C`). `make test-tools`:
  12 unit tests of `dmpsx.py` and `aspsx_fill.py`.
- **2026-10-14 (tool fix):** `aspsx_fill.py` now expands indexed symbol
  loads through the load's own register, as ASPSX does (maspsx used `$at`),
  in the gcc 2.7.2 units. First win: `func_8002E0F0` matched by hand. The
  other functions those units blocked on it (`func_8002E000`,
  `func_8002E3D0`, `func_8002DE2C`, `func_8002D4F4`) are ordinary matching
  now; their swarm notes predate the fix. `done` now refuses a function
  whose INCLUDE_ASM is still in place (it "matched" itself: a bogus
  metadata-only commit, and the swarm's duplicate resumes).
- **2026-10-14 (swarm):** Two runs failed every gate: the clones' `asm/`
  predated the `func_800274FC` symbol split (fixed in `ed7dc7a`, Gotchas).
  Rerun with `--resume` of both: 9 matched in ~30 min (stopped early), $16;
  `--resume` gated three functions twice (metadata-only commits), now one
  attempt per function. 400-599 B matches only 57% in normal mode (12/21):
  consider `--big-bytes 400`. Parks: `func_80010000` (registers 11/86),
  `func_8002E3D0` (needed loads expanded through the dest reg in 2.7.2
  units: fixed the same day), `func_80020C04` (inline asm refused).
- **2026-10-14 (play and test docs):** `tools/replay/README.md` (packer,
  playing in RetroArch, harness, recordings, limits), linked from README,
  CONTRIBUTING (when to run `make replay`) and CLAUDE.md. RetroArch + the
  PCSX-ReARMed core installed on the host at `C:\RetroArch-Win64`.
  `replay.py --video` (MP4 with sound); memory cards pinned in `OPTIONS`.
- **2026-10-14 (replay harness):** `make replay`: `tools/replay/replay.py`
  drives the PCSX-ReARMed libretro core through ctypes (pinned in the
  toolchain image, now `psx-decomp:2`), traces each frame's image hash and
  `values.txt` (seeds, `Heroes` mode/health/score), compares two discs.
  Retail vs build: 7512 identical frames; a patched `Random` is caught at
  frame 1294. Deterministic run to run.
- **2026-10-14 (disc packer):** `make disc` / `make disc-check`
  (`tools/scripts/pack_disc.py`): dumpsxiso the retail image into
  `build/disc/`, put the built exe and overlays back, mkpsxiso it. The
  matching build packs to the retail image byte for byte (~20 s). Roadmap:
  packer ticked, a playable, testable build is now the current focus.
- **2026-10-14 (by hand):** Split `func_80027408` at `0x800274FC` (a
  `// type:func` line in `sym.main.txt`; it never returns) and matched it.
  After a split, `decomp.py index` keeps the old size: fix `size` in
  `.decomp/functions.json`. Parked `func_800274FC` and `func_8002E25C`.
- **2026-10-14:** 1-hour swarm run: 20 committed, $41. Its parks showed
  `main/8002e0f0` is gcc 2.7.2 (Makefile changed; `func_8002E618` then
  matched by hand, with `pad` subsegments for the unit's end padding).
  `func_80027408` needs a symbol split (Open leads).
- **2026-10-13 (night):** 1-hour swarm run: 28 committed, plus 2 by hand
  (`func_80017D90`: the gate's m2c `?` lint flagged a ternary, now fixed;
  `func_8001AA48`: refused for its GTE macro, see Gotchas), 1 real park
  (`func_8002E0F0`, Open leads), $34. Biggest swarm match yet:
  `func_8002B210` (680 B). `tools/dk` names containers
  (`swarm-w1-diff-func_X-n`, `DK_NAME`); patterns folded.
- **2026-10-13 (evening):** Committed `func_80025534` (600 B) from the
  swarm's gate-fix patch. `ScriptCursor` (`func_80024FA4`) is a partial
  view of `EXTanimator` (both 0x2C), so the call casts; merge the two types.
  Roadmap: a final test (clean rebuild, shifted build plays, frame-for-frame
  replay vs retail, modern GCC), with its tools placed where first needed
  (packer + replay harness Phase 2, shift build Phase 4).

- **2026-10-13:** First real big-mode run (stopped early, ~18 min): 10
  matches incl. `func_8001D894` (612 B) by the big worker in one session,
  0 conflicts, $9. `func_80025534` matched alone but failed the gate on
  rodata (Open leads). Fixed: workers based on the integration clone's HEAD
  during a batch, so a failed batch dropped the base a permuter needed (now
  the last pushed tip); `stats` counts first-session big matches as big.

- **2026-10-13:** PsyQ gaps closed. The 27 gap functions were Sony's own tiny
  4.7 objects that signatures can't tell apart (identical bytes, different
  call or global): `psyq_gaps.py` compares every object with each gap and
  checks the relocations against the exe's symbol and data addresses, and
  found one object for each. The three multi-object gap files were split per
  object, then `link-psyq` (now incremental) linked them all. `--data` cut
  the data blob around 32 objects' `.data` (bios_1's differs, kept from the
  exe). `func_80032F3C` is `abs`; the rename only matches with
  `-fno-builtin`, so the game was built with it (the whole build matches).
  The pre-commit hook ignores whole PsyQ gap files deleted by link-psyq.

- **2026-10-13:** Big functions. `try` prints a `STAGE` line (structure /
  instructions / registers + diff count). Swarm big mode: functions >=
  `--big-bytes` (600) or parked in normal mode go to big workers (opus,
  150-min sessions, $20 each) that resume from a per-function checkpoint
  (`checkpoints/<func>`), kept only when the `try` stage/score improves; parks
  after 6 sessions or 2 without progress; the permuter (60 min) only at the
  registers stage. `swarm.mjs stats`: outcomes by size (normal mode 93-97%
  under 400 B, nothing bigger tried yet). Tested with fake workers (resume,
  stale park, escalation). Docs: MATCHING.md loop refreshed (unit check,
  stages) + Big functions section; swarm README, CONTRIBUTING, README,
  CLAUDE.md and the skill updated.

- **2026-10-13:** Swarm 1 h: 58 matches, 2 parked, 0 conflicts, $25.
  Two batch builds came out 8/32 bytes long: rodata, not code (a string
  literal duplicating an `INCLUDE_RODATA` one; gcc's `.align 3` jump table);
  the gate sent both back and they matched (Diff patterns rows). Folded 54
  rows (46 after merging). Split `0x8002D488..0x8002E0F0` into
  `main/8002d488` (text and rodata), built with gcc 2.7.2-psx plus
  `aspsx_fill.py` for ASPSX's delay-slot filling; per-unit Makefile
  settings, read by `decomp.py` (`unit_cfg`). `func_8002D488` matched.

- **2026-10-13:** Swarm, 30 min, 3 workers: 43 matches, 0 conflicts (the
  ordered prototypes worked), 1 failed, $11.16. The failure,
  `func_8001251C`, "matched alone" because `try` masked the in-function `j`
  targets. `compare` now checks them (91 functions with local jumps
  re-checked, only that one was wrong), and it matched by hand with a
  top-tested `goto` loop. 29 more Diff patterns rows folded in.

- **2026-10-13:** Improvements, no swarm run. Folded 36 swarm rows into
  MATCHING.md (the swarm's `patterns.md` was renamed to
  `patterns.folded-2026-10-13.md`). `declare()` now inserts prototypes in
  address order into the run of prototypes at the end of each owned header
  (`sort_protos.py` sorted the existing runs once), so parallel matches
  conflict less. `D_8004E0F0` is typed `Heap` (21 fewer warnings), and the
  Sfx walkers are typed. The build pads PsyQ gap objects to 16 bytes, and the
  three gap files that held several objects are split, so `func_80033530`
  and `func_8003E0EC` are matched.

- **2026-10-12:** Built the swarm orchestrator on branch `swarm-orchestrator`
  (`tools/swarm/`, README there): parallel Claude workers in clones, one
  serial integrator through `done --commit`, stop switch (`stop`, `stop
  --now`, STOP file), patch checks, its own `try`, circuit breakers. Added
  `decomp.py queue` and `DK_DOCKER_ARGS` in `tools/dk`. Tested end to end
  with fake workers (replayed two past matches, hard and graceful stop,
  timeout, escalation, a cheating patch, a false match claim). Then: CLI
  installed (npm), workers use `dontAsk` and `--max-budget-usd`, run cost
  cap; `done`'s full build now runs `make -j` (149 s to 30 s, same bytes),
  so a swarm commit takes ~1 min. First real runs (1 worker, Sonnet,
  `func_80012FFC`): run 1 parked at once because `sed -i` was refused
  (the prompt now says to edit with Edit/Write and not chain commands);
  run 2 worked properly but parked after 11 min, 10 of them waiting on the
  permuter (score 805 to 300). So the permuter moved to its own pool:
  workers hand off `needs_permute`, a follow-up worker ports the result.
  `func_80012FFC` has used its 2 attempts (`--retry-failed` to retry).
  First full run (2 workers, 1 permuter, 4 functions, $0.74 notional):
  3 matched on `swarm/integration` (`func_800135E4`, `func_80013650`, and
  `func_80011878` by Opus after Sonnet + permuter: it returns the split
  block). `func_800263D8` matched but `done` refused it (struct declared in
  main.c, prototype in main.h): workers are now told types go in the owned
  header, and a gate refusal goes back to a worker once. Two Diff patterns
  rows are waiting in `crash_bash_decomp-swarm/patterns.md`.
  Swarm on main (`--branch main`, 3 workers): 14 commits in ~25 min, and
  workers verified matches far faster than the one-at-a-time gate could
  commit them. Added the batched gate (`done --defer-build` + one
  `decomp.py verify` per batch), permuter pause during gate builds,
  back-pressure (`--max-queued`) and `--resume RUN`. First batch: 5 of 6
  committed in ~4 min (~50 s each, not 6x: per-function `done` overhead
  dominates; next, skip `cmd_progress` per function in batch mode). The
  batch re-check caught `func_80015A18`, which matched on an older main but
  not today's. 11 verified matches are waiting:
  `swarm.mjs run --branch main --resume 20261012120212`.
  Then profiled the gate: `done` spent ~36 s on the dashboard
  (`sym_names()` called per matched function; `rodata_syms` re-parsing
  2800 .s files): now ~5 s (hoisted, cached in build/cache). And every
  build re-ran all 16 splat splits (splat leaves an unchanged .ld alone, so
  it stayed older than sym.main.txt); under `make -j` those ran
  concurrently and sometimes produced a short exe. The splat pass is now
  serial and touches the .ld: full build 149 s -> 8 s, 11 verifies in a
  row OK. A flaky build could only refuse a good match, never pass a bad one.
  Then: 10-min run 15 commits, 20-min run 29 commits (~85/hour), mostly
  limited by conflicts on the end of `bash/main.h`. Fixed in the swarm:
  union merge for headers in its clones, neighbours (within 8 lines) never
  leased together, and a conflicting patch is re-applied mechanically
  instead of going back to Claude. Next swarm idea, not done: `done` could
  insert prototypes in address order instead of appending (fewer header
  clashes, also for hand sessions); long term, split `main.c` into units.

- **2026-10-12:** Short follow-on run: matched `func_80013688` (wrapper of
  `func_80013034`) and `func_80011BC0` (tail-recursive walk to the next free
  heap block; the NULL return goes last). `func_80012FFC` (sum of
  `D_80050628` fields) is next in the queue, untried: start from the m2c
  draft with a local for `unkC != 0`.
- **2026-10-12:** One-hour run: 21 matches in `main.exe` (`func_8001361C`, `func_80012E28`,
  `func_8001C34C`, `func_80028158`, `func_80026FCC`, `func_80026D9C`,
  `func_8002120C`, `func_8001D5D0`, `func_8002D094`, `func_80021E60`,
  `func_80022984`, `func_8002B8F4`, `func_80016E5C`, `func_8001DD20`,
  `func_8002C060`, `func_80021E98`, `func_80011828`, `func_800138A4`,
  `func_800133C8`, `func_8002D360`, `func_8001316C`); parked
  `func_80011E80`. `GfxState.unk8C` is a `MATRIX`. Six new Diff patterns
  rows (an inlined pop helper again, increment and constant order, operand
  order, early global loads).
- **2026-10-12:** Parked `func_80026D08` (free-list rebuild, not a spawner;
  declared the pool globals `D_8005AA10/12/1C`). Matched `func_8001636C`
  by copying its matched sibling `func_800163E0`, then `func_80016450` (same
  record; the pointer must be a local or gcc folds the offsets).
  Then `func_8001C270` (`eng->kill(eng)`, called from `KillNAS`) and its
  twin `func_8001C298` (`eng->render(eng)`).
- **2026-10-12:** Matched `SpawnNASFromRecord` (504 bytes) and unparked
  `SpawnNASById` (512 bytes): both pop the free list through a
  `static inline AllocNAS()` with an early `return NULL` (an earlier inline
  attempt without the early return had failed). `decomp.py try` now takes
  `DIFF_ROWS` for longer diffs. The ternary `?:` trips the `?` lint: use an `if`.
- **2026-10-12:** Matched the rest of Tank's `D_800CB264` table:
  `func_800C8598` (add), `func_800C85D0` (clear, descending loop) and
  `func_800C84D8` (returns the slot within `dist` of x/z; local declaration
  order fixed a t0/a3 swap).
- **2026-10-12:** Matched `func_800C8560` (Tank, 56 bytes): removes a `NAS *`
  from the 8-slot table `D_800CB264` (siblings `func_800C8598` add,
  `func_800C85D0` clear, `func_800C84D8` distance test: next candidates).
- **2026-10-12:** Matched `func_8008EF9C` (engine, 56 bytes): sets `D_8009E5DC`
  from `func_8008EF70()` (a word from `D_8009EDD4` by `D_8005A676`), then the
  same `D_8005A644 = 0x8008` as `func_8008F068`, then `func_8008ECE4()`.
- **2026-10-12:** Matched `func_80095D88` (engine, 52 bytes): counts 0x24-byte
  records while the first word is >= 2; m2c's stride was in bytes. Diff
  patterns row.
- **2026-10-12:** Matched `PlotRaceFire` (ovl63, 32 bytes): `FindExtension(nas,
  0x10000000)` with `nas` passed through; `ovl63.c` now includes `main.h`.
- **2026-10-12:** Matched `func_8008F068` (engine, 28 bytes): two global
  stores; `D_8005A644` is `u16` (its 0x8008 loads with `ori`). Diff patterns row.
- **2026-10-12:** Rewrote the four m2c Tank Inits (`InitTankBossCortexTurret`,
  `InitBoatField`, `InitShellExplosion`, `InitMineBall`) with `NAS`,
  `EXTengine` and `subengine` fields; typed 383 -> 387.
- **2026-10-12:** Added the rest of the Oct 9 types: new headers `core.h`,
  `hero.h`, `shell.h` and one per game (`hub.h`, `race.h`, `chase.h`, ...),
  plus Polar's missing structs in `polar.h`; each overlay includes its game's
  header. About 100 size checks; all binaries still match.
- **2026-10-12:** Named 201 AST Init/Plot functions from the Oct 9 enums
  (game functions named: 144 -> 345). Tank's 68 match the leaked Tank
  prototypes exactly; 14 matched Tank wrappers were retyped to `NAS *` first.
- **2026-10-11:** Integrated the Oct 9 prototype types: `nas.h` now uses the
  original names (`NAStype`, `EXTengtype`, `HeroInfo`, `EXTanimator`,
  `NASTrigger` for `NASSpawn`, `_sfx_handler` for `SfxVoice`) and defines
  `EXTheromove`, `EXTbotai`, `PreMap`, `LevSpec`; `tank.h` gains the Tank enums,
  `TA_player`, `TA_Bot`, `EXTobject_TA`. All binaries still match.
- **2026-10-11:** Searched the three Hidden Palace prototypes (`protos/`,
  git-ignored). No `.SYM`/`.MAP` files or text leaks, but the Aug 23 and
  Oct 9 DATs end in SN debug type records: engine structs with offsets and all
  AST enums, which name every overlay's game. New `leak_scan.py`,
  `sym_scan.py`.

- **2026-10-11:** Copied `func_80026A28` to `func_800C9B5C` (ovl62). The two
  PsyQ-gap copies don't fit in the build (function padding, see Open leads).
- **2026-10-11:** Matched `func_8001534C` (string hash) with a 10-minute
  permuter run after the loop rewrites stalled at 9 operands: the product goes
  through a reused `s32` temp. Diff patterns row added.
- **2026-10-11:** Third hour of `next`: 18 main.exe functions (16 matches,
  two identical copies), one via the permuter (`func_80015518`). New types:
  `SeqSlot` (`D_8006557C`, was an `s32`), `OffsetTable`, `PadState`,
  `MusicEntry`, `LoaderState`; `NAS.prev30`/`next30`. Two Diff patterns.
  Left `func_8001534C` unmatched (Open leads).
- **2026-10-11:** Second hour of `next`: 14 main.exe matches (`func_800280C0`
  to `func_8001682C`), one with the permuter (`func_8001828C`). New types:
  `NodePool`/`PoolNode` (`D_80056844`), `NAS.prev14`/`next14`.
  `func_80016EF4` takes a second argument (passed to `func_8002A5A4`).
  Four Diff patterns.
- **2026-10-11:** Matched `func_80016274` (112 bytes, first diff): first entry
  of a NULL-terminated array whose `func_8001534C` value equals `arg1`+0x10.
- **2026-10-11:** One hour of `next`: 17 main.exe matches (`func_8002C488` to
  `func_8001DCD4`), most on the first diff. `func_800101E0` launches the
  Spyro 3 demo (unused local for the frame). Subengine +0x54 is `hook`,
  `VabSlot.handle`, heap free-stats globals, ovl6a `D_800BCEE8` is a
  `subengine *`. Three Diff patterns.
- **2026-10-11:** Matched `func_80016AD4` (116 bytes, first diff): unloads
  what `func_80016C18` loads; `func_80012F04` takes one argument. Unparked
  `FindExtension`: a label inside the first `return` block, `goto` from the
  second (Diff patterns row updated).
- **2026-10-11:** Matched nine 32-byte wrappers like `func_800C6828` (eight
  in ovl5f, `func_800BCA18` in ovl65): pass `a0` through with a constant.
  `SetHeroSeq`/`SetHeroAction` prototypes (`Hero *`) in `bash/engine.h`.
- **2026-10-11:** Matched `func_800110A8` (508 bytes): the heap's free
  (merge with free neighbours, sorted insert). New `MemBlock`/`Heap` types.
  Finishes the core API list. Two new Diff patterns.
- **2026-10-11:** Matched `func_80025FAC` (156 bytes, first diff): selects
  sequence `n` of an `EXTanim` (`seq = seqs[n]`) and resets its state by kind.
- **2026-10-11:** Matched `func_8002C44C` (60 bytes) with `func_8001D604`'s
  C; `D_8006557C` is an `s32`, not an array.
- **2026-10-11:** Matched `func_800250B0` (248 bytes): runs a NAS's id-0x400
  `EXTanim` (new type) with `func_800251A8`/`func_80025534` by kind. Callees
  take `nas` through; ten overlay callers now pass `nas` (headers too).
- **2026-10-11:** Matched `func_80026A54` (132 bytes) with `func_80026980`'s C
  on the +0x28 list (`D_8005AA24`), first diff.
- **2026-10-11:** Matched `func_80026980` (132 bytes): unlinks a NAS from the
  list at +0x38/+0x3C headed by `D_8005AA34`. `NAS` gains `prev28`/`next28`
  and `prev38`/`next38`. Two new Diff patterns (head pointer local, `a || b`
  outer test).
- **2026-10-11:** Matched `func_80025A48` (156 bytes): finds the engine's
  `FlagRecord` with a flag (its own at +0x84, then the ones after a
  `PointList`'s points). New `FlagRecord` type, `PointList.nRecs`. Block
  layout came from joining the NULL checks with `&&` (Diff patterns).
- **2026-10-11:** Matched `get_angle` (752 bytes, `s32 get_angle(s32 x, s32 z)`,
  0x1000 per turn) on the second diff: the draft only had one quadrant's
  branch inverted; writing the `x >= z` case first there fixed it.
- **2026-10-11:** Started the core API (most-called unmatched functions,
  ~3,000 call sites; plan in Next): matched `Random` on the first diff,
  parked `FindExtension` (branch layout) but added its prototype; matched
  `StopSfx` on the first diff with new `SfxVoice`/`SfxPool` types, and
  `PlaySfx` (352 bytes; allocation written out in place, one final return),
  and `KillNAS` (388 bytes; `NASSpawn` typed, six callers fixed). Parked
  `SpawnNASById` at 4 operands with its prototype. Dashboard now shows
  function % beside byte %; new `check_progress.py`.
- **2026-10-11:** Matched `func_800193A8` (692 bytes, GTE triangle strips;
  register pins, `--hack`) through the new process; six Diff patterns rows.
  Fixed the gate lint so pins count as hacks.
- **2026-10-11:** MATCHING.md gained the evidence step, the Diff patterns
  table, per-kind checklists and the cc1 dump recipe; the skill's step 4 now
  requires them before the permuter. Parked `func_800155FC`. Matched
  `func_8002AABC` (sound init) on the first diff.
- **2026-10-11:** Matched 10 small exe functions (pool free-lists, bit
  set/clear loops, `AddExtension`, list length, ...). Root-caused
  `func_8001C7FC` with cc1 dumps (still parked). Dashboard: progress over time
  per unit. Unparked `func_80015780`; GTE batch (5 functions).
- **2026-10-10:** Phase 2 started: Sony's GTE macros in the sources
  (`dmpsx.py`), first GTE matches. Phase 1 finished: engine types (`NAS`,
  `EXTengine`, `Hero`, ...), core names and globals, owned headers per source
  file, PsyQ headers, leaked headers in `include/bash/`, AST Init/Plot names,
  objdiff config fixed.
- **2026-10-10:** Hand matching only (`done` gate, pre-commit hook; `auto`
  retired) after an auto pass to 391 functions. Overlays found and split
  (`engine` + 14 slot overlays at `0x800B32B4`). Project set up from the
  `psx-matching-decomp` skill.
