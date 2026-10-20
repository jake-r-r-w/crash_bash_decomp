# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-20)

- [ROADMAP.md](ROADMAP.md): Phase 1 (types and names) is done, and every
  `main.exe` function matches; Phase 2's naming, data and `main.c` split
  are left. The work now is matching the overlays (Phases 3-5) with the
  swarm.
- 2158/2537 game functions matched (55.2% of game code bytes, 2026-10-20):
  **main.exe and ovl61 are complete (575/575, 97/97)**; engine 355/357
  (both left parked: `func_8007F314`, `PlotScriptFX`); **ovl6a
  complete (94/94, 2026-10-20)**, ovl69 69/77, ovl67 102/135, ovl60
  131/180, ovl5f 127/179, ovl62 151/216, ovl63 97/139, ovl5d 70/104,
  ovl65 64/80, ovl68 60/93 (swarm runs 2026-10-18/19), ovl66 49/65 left
  16 (swarm 2026-10-20); ovl5e 7 left (54 matched 2026-10-20, `func_800BB3F0` by
  hand); ovl64 23 left (22 matched 2026-10-20, plus BB3F0's copy). All 564 PsyQ functions (Sony objects). 3 parked (`decomp.py
  status`). Every slot overlay's id word is
  its own rodata unit (2026-10-19), so first-file switches link as `switch`.
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

**Matching the slot overlays with the swarm.** Left per overlay (2026-10-19,
`INCLUDE_ASM` count per file): ovl5e 7 (2026-10-20), ovl62 65, ovl66 16 (2026-10-20), ovl68 33,
ovl5f 52, ovl64 23 (2026-10-20), ovl60 49, ovl63 42, ovl5d 33, ovl67 33, ovl65 16,
ovl69 8, engine 2 (ovl61 and ovl6a are done). main.exe is complete. **Next session:
ovl5e's 7 and ovl64's 23 are mostly >= 600 B: big-heavy recipe (`--workers 1 --big-workers 4 --big-bytes 400`), then ovl66's leftovers**, on the low-cost recipe; after each run, the
"After a run" checklist in tools/swarm/README.md (`stats --run ID`, parks,
gate failures, `fold`, this log).

Recipes (`--seg overlays/NAME --branch main --max-minutes 60`):
- **Low cost** (2026-10-19, ovl5f: 78 matched for $15.96, ~$0.20 a match;
  ovl5d: 64 for $31.79, ~$0.50, its 250-599 B functions cost more; ovl65:
  50 in 38 min for $16.70, ~$0.33; ovl68: 46 in 26 min for $11.71, ~$0.25):
  `--model sonnet --escalate-model opus --big-workers 1 --big-budget-usd 8
  --max-run-usd 30`. Use it for fresh overlays.
- **Opus** (the `config.json` defaults, 5 normal + 1 big; ovl62/ovl60/ovl63
  matched 77-117 an hour for $34-47): add `--big-workers 2` while big
  functions are queued. The normal workers finish the small functions first,
  then idle.
- **Big-heavy** for an overlay's leftovers, mostly >= 600 B: `--workers 1
  --big-workers 4 --big-bytes 400`. Retry parks with it after tool upgrades
  (`decomp.py unblock`, `--retry-failed`): on 2026-10-16 it matched 12 of
  main.exe's 15 parks.

Gate failures are usually header clashes or the toolchain, not the worker's
C: the swarm now reconciles headers itself (`decomp.py reconcile`), and the
rest are fixed by hand from the worker's patch (Gotchas). A park whose
`jtbl_` names functions is a bad split (Gotchas).

Hand work, between runs:
1. The parks: `func_8007F314` and `PlotScriptFX` (engine), `func_800BC274` (ovl5e), and the hacks in `func_800B345C` and `func_800B7134` (Open leads for each).
2. Name each function as it's matched when the evidence allows.
3. Split `main.c` into its translation units: on hold since 2026-10-15. Its
   one proven boundary (jump-table parity, 95..370) has three possible
   places and no overlay shows another positive sign; more cuts need new
   evidence (a leak, a symbol file of a game on this engine). The evidence
   so far is in ARCHITECTURE.md, Files inside `main/main`; `decomp.py units
   [--data] [--clusters]` collects it. A split moves functions, so workers'
   patches and checkpoints for them stop applying.
4. Later Phase 2 items: data to C (after the split), naming and typing
   passes.

Swarm history in short (details: `swarm.mjs stats`, the ledger, the commits):
engine 2026-10-15..17 (big-heavy from 2026-10-16), main.exe parks
2026-10-16, ovl61/ovl67/ovl69/ovl6a 2026-10-18, ovl62/ovl60/ovl63/ovl5f
2026-10-19 (opus normal workers since then, $0.19-0.45 a match under
400 B against sonnet's ~$0.48 before; low cost ~$0.20 with the reconcile
and current tools).

## Open leads

- **Tune the permuter from its record.** Runs are logged since 2026-10-20
  (`decomp.py permstats`, `swarm.mjs stats`). Two ideas to try once there
  are a few dozen runs, compared on that record:
  - Registers-stage pass weights in `settings.toml` (`weight_overrides`):
    favour temp_for_expr, split_assignment, randomize_internal_type,
    reorder_decls and duplicate_assignment, the edits behind the
    permuter finds in Diff patterns.
  - A deterministic search over the Diff patterns' common fixes (copy a
    parameter to a local, s16 -> s32, split a statement, a constant in a
    local, a goto loop), generalising `splitvars`.
- **`func_800B345C` (ovl63, 30735f0c) still matches only with a hack** (no-op
  `h++; h--;` and `p++; p++; p--;` in its goto loop; `done` now refuses such runs
  without --hack). Reworked 2026-10-19, not finished. Without the no-ops it is
  instructions 19/92: `h`/`i` swap s1/s2 and `i = 0` sits at 0x48, not 0x10.
  - Registers: global.c priority (-dl) is i 4 refs/30 = 0.267 against h 6/46 =
    0.261, so i takes s1. `do { } while (0)` around only the first
    `h->hero_ai->AIpad &= ~0x20;` (one weighted ref, h 0.304) fixes all of
    them: 11/92 left. Wrapping any other statement or the whole branch breaks
    the structure; a real `while (1)` loop too.
  - Left: `i = 0` fills the load-delay stall after `lw a0,4(v1)`, where the
    target has `addiu a1,%lo(D_80068BD4)`. In sched1 (scheduling backwards)
    the table address insn 29 loses every launch-priority tie to the index
    insns on INSN_LUID, so it goes to the top and `i = 0` (priority 1,
    initially ready) takes the stall. No statement order, `pos.x` spelling or
    index temp moved it. Next: find C whose RTL emits the table address after
    the index arithmetic (or makes `i = 0` depend on something late).
- **ovl5e big-mode checkpoints (2026-10-20), hints the workers left** (their
  Diff-patterns rows, not folded: the functions aren't matched yet). Closest
  first; the first two were worked by hand on 2026-10-20 (BC274 parked,
  B7134 matched with a hack):
  - `func_800BC274` instructions 2/197, parked by hand 2026-10-20 (block
    note): only `addiu a1` / `lw a0` before `jal func_8001CD04` are swapped.
    sched1 ties them and luid puts a0 first; the target's order needs a
    second load/store in that block (potential hazard; every matched
    a1-first caller has one), so the original's was deleted after sched1.
    A second pass (2026-10-20) ruled out cross-jumped tails, local-alloc
    and reload equivalences, reload_cse and an unfolded a1 precompute (the
    block note has why), and a third ruled out single -f flags and inserted
    reads; the function is absent from the Aug/Sep prototypes and identical
    in Oct 9. The same code is unmatched in `func_800C284C`
    (ovl5d) and `func_800BAF1C` (ovl64): solve the three together.
    Untried mechanism from `func_800BB3F0` (same file, same
    `func_8001CD04` block shape, a1 first there): duplicated code wrapped
    in `do { } while (0)` lives until jump2's cross-jump (after sched1 and
    allocation), while plain duplicates merge before flow. A wrapped copy
    of a join load in the then-arm could be sched1's second memory insn
    (the loop notes are also a sched1 barrier: check -dS).
  - `func_800B9E74` registers 12/238: a `subu` takes the second operand's
    register; split `d = a->x; d -= b->x;`. Its permuter run was cut short.
  - `PlotPapuBoss` structure 242/455: two adjacent u16 stores, chain them
    (`a = (&a)[1] = X;`). `PlotPapuMinion` structure 854/877: index
    `Table[slot].f` at each use, not through a pointer. `func_800BB86C`
    structure 439/644: `x < 4 && x >= 2` as nested ifs (fold-const makes a
    range check of the `&&`).
  - `func_800B7134` matched with a dead-store hack 2026-10-20 (ba1244e6):
    locals `tb/trig/seq` set at the outer loop's top and NULLed at its
    bottom. Without them (globals used directly) it is 2 insns off, the
    inner loop exact: the `D_800BE838` lui/addiu is hoisted out of the outer
    loop. `move_movables` doubles `insn_count` per inner-moved reg, so the
    3rd+ candidate always moves; a user variable's set escapes only with
    `maybe_never` (outer scan past a label/jump, e.g. a loop entered at its
    exit test) or a second set. Tried and failed: for/while/do/goto outer loops, block-scope locals, cell pointers.
    Scripts: `scratch/b7134/` (`brute2.py`, `full.py`).
  - `func_800BB3F0` matched by hand 2026-10-20 (08aa8ff7, no hack; copies
    `func_800C159C` and `func_800BA098` too). Diff patterns has the three
    fixes: read `Heroes[player]` directly in the loop, so loop.c moves its
    la first and lowers its threshold, which keeps the multiply in the
    loop; K&R `u16 cone`; and the claim written once per arm, each in
    `do { } while (0)`. For `func_800B7134`'s outer-loop hoist (a hack
    match), wrapped duplicates also add to loop.c's `insn_count` without
    changing the final code: untried there.
  - `func_800B38FC` instructions 32/131: stack `u16` args get `andi` +
    `sll` after a branch; declare the params `u16` and scale them in place
    (`n4 *= 4;`).
- **Overlay run 2026-10-20 (run 20261020211358), to fix by hand:**
  - `func_800B4268` (ovl65): the gate refused the worker's match because the
    full build failed. It gave the function an extra parameter, and its
    callers still pass fewer arguments. Fix the callers' prototype use from
    the worker's patch (`<swarm dir>/results/ovl65_800B4268/`). Its hint:
    end with `r2 = r1 < 0x100; if (r2) goto one; t0 = 0; return t0;` so the
    early `return 0` isn't cross-jumped onto the final one.
  - `func_800B4444` (ovl68) matched with two workarounds that
    `decomp.py hacks` doesn't catch. It keeps a dead pointer increment
    alive with an unused `j`/`k` cycle (`if (*p != 0) { j = k + 1; k = j; }`,
    which cse keeps and flow deletes), and it calls `func_800B3330` through a
    function-pointer cast instead of fixing that stub's `void (void)`
    prototype. Fix the prototype, find the real loop shape, and extend the
    dead-store lint to variable cycles.
  - Hints on still-unmatched functions: `func_800B47C8` (ovl60) lays out
    0xD/0xE/else as E, D, else (`goto case_d` into an `else if (0)`
    block got it there); `PlotCircleFire` structure 133/155: index the
    table by the loop counter (`T[i].x`), not a walking pointer;
    `func_800B5D90`: `lh` + `sra 3` as `.unk2 * 1024 >> 13`.
- **Hack matches to revisit:** `decomp.py hacks` lists them (three on
  2026-10-20: `func_800193A8`, register pins that are probably the original
  source, `func_800B345C` and `func_800B7134` above). Since 2026-10-20 the
  `done` lint also flags dead stores, and `hacks` lints every matched
  function (its old file prefilter hid no-op pairs). Parks are `decomp.py status` / the dashboard. Before reaching for a
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
- Heap: `D_8004E0F0` is a `Heap` now. Its +0x10 is the deferred-free list
  (`func_80011578` pushes, `func_80011DB4` counts down); `MemBlock.unk2` is
  the countdown. `func_800115D8`'s prototype still takes `void **`, not
  `Heap *`; retype it when its callers allow.
- Prototype debug types (ARCHITECTURE.md, Prototype debug types): every type
  in the Oct 9 fragment is in a header now, and every single-id AST
  Init/Plot is named. Use them while matching: type `NAS` fields, `FUX`
  extensions and globals with them instead of offsets, as the four Tank
  Inits (`InitMineBall`, ...) now are. 34 fields of 15 types
  whose bodies were on lost pages are word or byte arrays; retype them if code
  shows their fields.
- Sound voices: the C walkers use `SfxVoicePool.active`; the symbol
  `SfxActiveVoices` stays only for the data asm (`asm/data/3E4BC.data.s`).
  Drop it from `sym.main.txt` once data goes to C. `func_80022504` is "stop all" (`StopSfx` inlined in a loop),
  `func_800228C4` unlinks without stopping, `func_80022924` allocates,
  `func_80022984` inits the pool.
- Ambiguous naming leads are listed in ARCHITECTURE.md ("Functions reached
  from the AST pairs").

## Gotchas

**Tooling**
- **`done --commit` in a git worktree (`.claude/worktrees/...`) fails at
  `git add`** ("not a git repository: /work/C:/..."): the worktree's `.git`
  file names a Windows path the container can't see. A worktree also lacks
  `asm/`, `linkers/`, `ext/` and `disc/`: copy them from the main checkout
  first. Re-run the gate with the main `.git` mounted (one line, from the
  worktree): `DK_DOCKER_ARGS="-v <main>\.git:/maingit -e
  GIT_DIR=/maingit/worktrees/<name> -e GIT_WORK_TREE=/work -e
  GIT_CONFIG_COUNT=2 -e GIT_CONFIG_KEY_0=core.hooksPath -e
  GIT_CONFIG_VALUE_0=/work/tools/hooks -e GIT_CONFIG_KEY_1=safe.directory -e
  GIT_CONFIG_VALUE_1=*" tools/dk python3 tools/scripts/decomp.py done ...`.
  If the failed run already matched and propagated copies, `git checkout`
  everything but your own edits first: the scope check refuses the copies.
  Propagation also skips a copy whose header still declares a stale
  prototype (`void f();` against `NAS *`): fix it and `done` that copy alone.
- **`permute --lines A-B` widens to whole statements.** A range starting
  on `if (...) {` takes the whole if/else chain (`func_800B5710` 317-324
  became 277-458). Start inside the block. A `decomp.py permute` stopped
  early records no PERMUTE-STATS line, and a `stop` of `swarm.mjs
  permute` saves nothing: copy `output-*/diff.txt` first.
- **`tools/dk python3 - <<'EOF' </dev/null` runs nothing:** the `</dev/null`
  replaces the heredoc as stdin, and Python exits quietly on an empty
  script. Write the script to a file in `scratch/` and run that.
- **After a resplit or rename, run `decomp.py index`.** Since 2026-10-19 it
  checks every entry against the symbol files and `src/`: a renamed
  function gets its symbol's name (before, 51 entries kept their old
  `func_` names and no `src`), and an entry no symbol, `INCLUDE_ASM` or C
  definition names any more (a function a resplit merged away) is dropped
  from `functions.json` and `state.json`. `decomp.py rename` also failed its build check
  on 2026-10-19 with the error hidden ("BUILD FAILED") when the same rename
  by hand built and matched; rename by hand (symbol file, sed, `make split`,
  `make check`) if it does.
- **A switch whose cases each return is cut into functions.** In a leaf
  function without a frame, every case's `return`/`break` becomes its own
  `jr $ra`, and splat starts a new function at each jump-table target after
  one (`func_800B5588`, 2026-10-19). Sign: a `jtbl_` whose words name
  functions instead of `.L` labels. Fix: give the function its full extent
  in the overlay's symbol file (`func_X = 0x...; // type:func size:0x...`,
  up to the next real function), resplit, then `index` (above).
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
  The swarm runs `decomp.py reconcile` on a declaration clash first
  (2026-10-19), then a gate-fix worker with cc1's error lines; what still
  fails is left for a hand fix. Its C matched against an older tip, and other matches in the run changed the
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
- `tools/dk` right after `git checkout` sometimes hits a Windows
  `PermissionError`; retry.
- `make permute` runs in its own container: stopping the background task
  doesn't stop it. `docker stop` it before running `done`.
- Stopping `swarm.mjs permute` early (a STOP file or Ctrl+C) saves nothing,
  and its STOP file then blocks the next run until deleted. Copy the best
  `output-SCORE-N/` from `<swarm>/permuters/hN/build/permuter/NAME/` first,
  apply its `diff.txt` in that clone, measure it, and write `best.diff` and
  `best.json` (`git diff` in the clone, against its HEAD).

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

- **2026-10-20 (swarm all overlays):** run 20261020211358, `--seg overlays/
  --model sonnet --escalate-model opus --workers 5 --big-workers 2
  --big-budget-usd 8 --big-minutes 12 --max-run-usd 40`, stopped
  gracefully by request after 18 min: 10 matched for $10.26 (ovl68 5,
  ovl62 2, ovl66 2 incl. the morning's park `func_800B34F8`, ovl64 1), 2
  failed (an OAuth refresh clash at start; `func_800B4268` refused at the
  gate, Open leads). 85 functions under 600 B had never been tried (runs
  ended on budget before them); most are still queued. `fold`: 12 rows, 4
  new, 1 example added, 2 hack rows and 4 on unmatched functions to Open
  leads, 1 duplicate dropped.

- **2026-10-20 (cleanup):** merged the sched tooling (260a4660); removed
  the two side-session worktrees and their merged branches (the BB3F0
  session's scratch kept in `scratch/bb3f0/`). BC274's third pass (flags,
  810 inserted reads, prototypes, do-while(0) copies) is in its block note.

- **2026-10-20 (hand: `func_800BB3F0`, ovl5e, 684 B):** matched with no hack
  (08aa8ff7; its copies too: `func_800C159C` in it, `func_800BA098`
  8e6f2255). From the swarm's 26/171: `-dL` showed the `player*108` chain
  moving at 85 insns (29*2*2 >= insn_count). Reading `Heroes[player]`
  directly in the loop moves its la first (threshold -3 per move), which
  keeps the chain in the loop. The last `s0/s1`, `s6/s7` swaps were global.c
  priorities (`mask` 5 refs/222 can't beat `self` 4/86). The claim in a
  do-while (0) in each arm fixed them. Worktree git gotcha added.
- **2026-10-20 (tools: `sched` on parked C, tie rules):** `sched`,
  `regions` and `vars` now compile a parked function's C under
  `#ifdef NON_MATCHING` (`-DNON_MATCHING`, into `build/regions/` and
  `build/sched/`, never the build's object). `sched`'s report counts each
  block's memory insns and names the rule behind every pick: priority,
  birthing, class, potential hazard or luid, following sched.c's lazy
  re-sort (`order from T-n`). The potential-hazard model (store > load >
  ALU at 2+ memory insns) predicted all 16.9k tie groups of five units'
  dumps. `sched func_800BC274` shows block 5's 100-over-98 luid tie with
  one memory insn. A worktree needs `asm/`, `linkers/`, `ext/` and the
  small `disc/` files copied in (git-ignored) before cpp or make work.

- **2026-10-20 (hand: `func_800B7134` as a hack match):** on the user's go,
  committed the dead-store version (ba1244e6, `Hack:` line in the commit):
  198/198, the `j = 0;` before the three sets is needed (sets first, then
  `for (j = 0; ...)`: 7/198). `done`'s lint now flags a local whose last
  store in a loop body is dead (63b06bf4); no other matched function hits
  it. `decomp.py hacks` now finds `func_800B345C` too.

- **2026-10-20 (hand: ovl5e parks `func_800BC274`, `func_800B7134`):** both
  parked, C under `NON_MATCHING`. BC274 (2/197): the a0/a1 order before
  `jal func_8001CD04` is a sched1 tie that `schedule_select`'s potential
  hazard or luid decides; the target's order needs a 2nd memory insn in that
  block, which its final code doesn't have. 47k permuter iterations and every
  shape tried fail. B7134: the inner loop now matches exactly
  (`a >>= 7` in place, the angle index in its own `k`), found by compiling a
  statement-order x form grid with cc1 directly; the outer loop still hoists
  one table base (2 insns) unless the worker's dead NULL stores stay, which
  match 198/198. Added FSF gcc 2.8.1 source to `tools/reference/gcc-2.8.1/`
  (sha256 in its README.decomp.md); the passes used here have the same rules
  as 2.7.2.3. Four Diff patterns rows.

- **2026-10-20 (swarm ovl5e, run 3, 30 min big-heavy):** run
  20261020123536, `--workers 1 --big-workers 4 --big-bytes 400
  --max-minutes 20 --big-minutes 12 --worker-minutes 12 --permute-minutes 8
  --big-permute-minutes 8 --max-run-usd 30`: 3 big matched (`func_800B6D14`
  740 B, `func_800B36D4` 552 B, `func_800B6860` 1204 B) in 27 min for
  $19.01, ~$6 a match (run 2's big mode: $20.82 for one); 4 parked after
  two big sessions without progress (`func_800BC274` 2/197, `func_800B7134`
  5/198, `func_800BB3F0`, `func_800B38FC`). 9 left. `fold`: 8 rows, 1 new,
  2 into the pointer-local row, 5 on unmatched functions to Open leads.

- **2026-10-20 (swarm ovl5e, run 2):** run 20261020112410, `--model sonnet
  --escalate-model opus --workers 5 --big-workers 2 --big-budget-usd 8
  --max-run-usd 40`: 34 matched in 63 min for $41.68 (stopped at the
  budget), no parks; 12 left (3 with checkpoints, 9 big ones not reached).
  $/match: 150-399 B ~$0.28, 400-599 B $0.90, big mode $20.82 for one.
  Permuter pool (first live record): 10 runs, 9 improved, 4 matched
  (`func_800B7BE4`, `func_800B4EE8`, `func_800B64AC`, `func_800B5E44`; 3
  went to the gate without a port session), helped by scoring by address.
  `fold`: 29 rows -> 15 new, 4 into existing rows, 3 rows on unmatched
  functions kept as leads (Open leads), duplicates merged.

- **2026-10-20 (swarm ovl64):** run 20261020083954, `--model sonnet
  --escalate-model opus --workers 4 --big-workers 2 --big-budget-usd 8
  --max-run-usd 40`: 22 matched in 22 min for $8.26 (~$0.38 a match), no
  parks; stopped with `--now` (network going down), which killed 2 big
  sessions and 2 permuter runs in flight. 24 left, mostly big. `fold`: 16
  rows, the six `Heroes[arg0]` lookup rows merged into one.

- **2026-10-20 (two parks, permuter then hand):** `func_800B5710` (ovl6a,
  2836 B, c1cb924c; ovl6a complete) matched with the permuter's change
  alone: `ang = 0x400; ang = (... % 0x1000) + ang;`. The permuter scored
  that C 20, not 0: the target names fields by their own address
  (`D_8005A677`), ours `D_8005A614+0x63`, and its scorer compared names, so
  it never reported the match. Fixed (a5ac14a0: relocations compared by
  address). `func_800B7458` (ovl69, 1360 B, 2cb2b9c4): the permuter's `off`
  temp fixed the row block; the test then needed `val`, another variable
  the target keeps in v1 (global.c's a1 preference, Diff patterns).

- **2026-10-20 (swarm ovl5e, then a permuter pass):** low-cost recipe, run
  20261020073721: 16 matched in 10 min for $1.43, stopped by request (47
  left). `fold`: 6 rows down to 2 new, 3 merged into existing rows.
  Then 4 permuter runs (stopped at ~15 of 30 min), all gains in the first
  2 min but one: `func_800B5710` 300 -> 20 and `func_800B7458` 20 -> 10
  with real C (their leads); `func_800B34F8` (ovl66) -> 15 only with a
  no-op `D_800B9EF8++; D_800B9EF8--;` (that global's reference count is
  the lever); `func_800C159C` (ovl5d, registers 45/171) 295 -> 235 at
  13 min with `(*(new_var = &h))->hero_object`, a hack that says `h`'s
  allocation is the lever.

- **2026-10-20 (permuter speed and record):** `decomp.py permute` now
  prunes `base.c` to the declarations the function uses and compiles
  candidates through `tools/scripts/permuter_fast.py`, which runs the
  Python filters in-process, reads includes from /tmp and skips a no-op
  cpp. Throughput went up 1.7x to 7x. Identical code on 58 sampled
  functions across every unit (bash, fast, pruned). `base.c` also gained
  the file's own declarations; before, a function using the file's own
  externs failed to compile there (3 of 58). Every run is recorded
  (`permstats`, the swarm's `permuter.jsonl` and `stats`). MATCHING.md, Big
  functions.

- **2026-10-20 (swarm ovl66):** low-cost recipe, run 20261020061838,
  49 matched, 1 parked (`func_800B34F8`, registers 8/92, checkpoint kept)
  in 39 min for $7.10 (~$0.15 a match); stopped early by request (`stop`,
  then `--now`, so the big session on `func_800B7B48` and two permuters were
  killed). Its first run (20261020061709) died at start on an OAuth refresh
  clash even with the 10 s start gap; `--start-gap-seconds 30` worked.
  `fold`: 14 rows down to 8 (merged duplicates, dropped two hacky ones).

- **2026-10-19 (PlotBall by hand):** matched (f0a4117d, 5576 B), from the
  swarm checkpoint at structure 777/1394; ovl61 is complete. No hacks, no
  permuter: each step came from the cc1 dumps (CSE's canonical register,
  local-alloc ties, global priorities) and small test files compiled with
  cc1. The keys were `s16` locals (gcc 2.8 extends a short when it's
  read, not set), splitting `done` into an `s32` flag and an `s16`, a
  `static inline` for the player-to-side chain, and reading `D_8005A648`
  and the `GetContour` hook as aggregate members so stores stay ordered
  (that removed the checkpoint's pointer temps and gotos). Eight Diff
  patterns rows; `D_8005AA70` is the level's `LevSpec` (ARCHITECTURE.md).
- **2026-10-19 (permuter + hand, near misses):** `swarm.mjs permute --only
  ovl67:800B39C0,ovl69:800B7458`, stopped by hand after 32 of 60 min.
  `func_800B39C0` went from registers 29/145 to 3/145 (a `do { } while
  (0)` around one assignment, found in the first minute), then matched
  by hand (ca3a674c): `D_800C418C[i]` indexed in place in the tail, not
  through the loop's pointer (Diff patterns). Gated in the swarm's `h1`
  clone because the main checkout had another session's edits.
  `func_800B7458` was skipped: its checkpoint no longer applies (Open
  leads).
- **2026-10-19 (ovl68, low cost):** run 20261019171842, stopped by hand
  after 26 min: 46 matched for $11.71, no gate failures; game code passes
  50% of bytes (2024/2537 functions). Four left unmatched: two cut off by
  the stop before their C built (`func_800B5CD0`, `func_800B65B0`, retry
  next run), `func_800B6C84` (instructions 12/128, checkpoint kept) and
  `func_800B4444` (cc1 deletes its empty first loop). Fold: 19 rows, 3
  repeats dropped.
- **2026-10-19 (hand, ovl5d):** `func_800BB64C` (364 B) matched by hand
  from the swarm park (registers 2/91), no permuter: `z` masked in place
  (one death, so local) and `x` its own local loaded before the two
  multiplies (its longer life drops its local-alloc priority). Two Diff
  patterns rows.
- **2026-10-19 (hand, ovl69):** `func_800B7458` from its checkpoint
  (registers 4/340), parked at the same score: the last swap is a global.c
  register preference (Open leads). The checkpoint's header retype of
  `Ovl69Entry.unk0` broke four integer users; it stays `s32` with a cast.
- **2026-10-19 (hand, ovl65):** `func_800B4D6C` (1720 B) matched by hand
  from the swarm's checkpoint (structure 314/430), no permuter: loop 3
  as a real loop, `Heroes[i]` indexing, separate loop variables, and
  `best` reused as the done flag (four Diff patterns rows). `decomp.py
  sched` misread hex ready-list priorities (>= 0x10); fixed.
- **2026-10-19 (ovl65, low cost):** run 20261019154152, stopped by hand
  after 38 min: 50 matched for $16.70, 0 parked. The first start failed on
  "Docker is not reachable" (Docker Desktop had quit; start it and rerun).
  Both gate failures matched by hand from the workers' C: `func_800BC820`
  (prototype said void, it returns s32), `func_800B85DC` (the tree's
  `Ovl65Ext`; `D_800BDA00`/`04` retyped `NAS *`). `func_800B4D6C` (1720 B)
  keeps its big-mode checkpoint (structure 314/430). Fold: 29 rows, 5
  repeats dropped.
- **2026-10-19 (ovl5d, low cost):** run 20261019133956, 64 matched in 67
  min for $31.79 (the $30 cap stops leasing, sessions finish). Gate
  failure `func_800B6DA4` matched by hand (the tree's `EXTobject.status`,
  an s32 return: a caller's s16 prototype was wrong). Parked
  `func_800BB64C` (registers 2/91, checkpoint kept). Fold: 40 rows, 4
  dropped (repeats, a dead-value hack).
- **2026-10-19 (tidy):** `decomp.py index` repairs the index itself: 51
  renamed functions had kept their old names and no `src`; merged-away
  entries are dropped. This log's Next is current counts and recipes
  instead of run reports; dead leads and gotchas (main.exe's, now matched)
  removed; session entries before 2026-10-16 moved to git history. Swarm
  README example is the low-cost recipe; `--big-minutes` default is 27.
- **2026-10-19 (reconcile):** `decomp.py reconcile` now keeps the tree's
  declaration over a patch's only when every C file including the patched
  header also sees the other header (`headers.visible`); before, any header
  counted, which broke `func_800C8A80` once.
- **2026-10-19 (ovl5f split):** `func_800B5588`'s four switch cases had
  been split off (and swarm-matched) as functions; a `size:` in
  sym.ovl5f.txt rejoins them and the whole function matches by hand (a
  pointer per case, like ovl61's sibling).
- **2026-10-19 (ovl5f, low cost):** 78 matched for $15.96 with Sonnet
  workers; survived an internet drop via --resume; 33 pattern rows folded
  (7 repeats dropped); one park points at a bad split in ovl5f.
- **2026-10-19 (tools):** three matching aids. (1) `decomp.py reconcile`
  (headers.py): on a declaration-clash gate refusal the swarm keeps the
  tree's declarations and gates again before paying a worker. (2)
  `aspsx_compare.py` runs Sony's real ASPSX (2.56-2.86, `ext/aspsx/`) on
  every file's matched C: identical to our pipeline on all ~1,700 gcc 2.8.1
  functions; the slot fills `aspsx_fill.py` adds (frame-only epilogue, the
  2.7.2 units) are Sony's compilers', not ASPSX's (ARCHITECTURE.md,
  Toolchain). (3) `decomp.py sched NAME` prints global.c's allocation
  order and priorities and the rule that placed each insn in sched1/sched2.
- **2026-10-19 (lint):** `done` refuses a run of `x++; x--;` statements
  (no-op pair) without --hack; only `func_800B345C` has one. Its rework got
  the registers with one natural `do { } while (0)` but not a sched1 tie
  (Open leads); the committed hack stays.
- **2026-10-19 (hand + toolchain):** `func_800C0F5C` matched with plain C
  after the build learned ASPSX's jump-slot fill for gcc 2.8.1 units
  (f8824b13: `aspsx_fill.py --jumps-only` by default; of the ten slots gcc
  2.8.1 leaves to the assembler, nine follow a store and keep their nop,
  the frame-only epilogue gets filled; `make check` OK). Gate failures
  matched from the workers' C: `InitRaceSmokeBoost`, `func_800C5620`,
  `func_800C6B9C`.
- **2026-10-19 (ovl63 swarm):** 77 matched in 51 min ($34.41) with 5+2
  workers (now the default); 1 park, 1 gate failure, 1 hack to redo
  (`func_800B345C`); 28 pattern rows folded (3 repeats and the hack
  dropped).
- **2026-10-19 (ovl5f swarm):** 9 matched in a 9-min run, stopped for the
  ovl63 7-worker trial; 6 pattern rows folded.
- **2026-10-19 (hand):** `func_800C8BA0` (ovl5f, 567ce2fb): its "+4 bytes
  of rodata" lead is gone (the id-word split fixed it); it needed all five
  cases listed so gcc builds the 0..4 table.
- **2026-10-19 (gate):** `done` now refuses other functions' C edited on
  lines that don't call the match (`stray_edits`; caught the ovl60 case,
  0 hits over the last 150 match commits), and `extract_function(top=True)`
  skips an indented call line it used to take for the definition.
- **2026-10-19 (hand):** ovl60's two gate failures matched from the
  workers' C: `func_800BC184` (6ff3b7da; the header already had the slot
  field, plus a pad view) and `Roo_ChangeDirOnEdge` (b0e170e0; without its
  patch's stray edit to `func_800BE864`).
- **2026-10-19 (ovl60 swarm):** 113 matched in 60 min ($46.61, 0 parked,
  2 gate failures to retry); 40 Diff pattern rows folded.
- **2026-10-19 (ovl62 swarm):** 117 matched in 60 min ($46.59, 0 parked) on
  opus workers after the resplit; fold added 39 Diff pattern rows (10
  repeats of existing rows dropped).
- **2026-10-19 (ovl62 splits):** the first ovl62 swarm run stopped after a
  minute (5 workers hit an OAuth refresh race at once), but its one park
  found ten AST names that `5bf518f2` put in `ovl62`, each cutting a real
  function mid-code. They belong in `ovl63` (Rippled, InnerRipple) and
  `ovl6a` (PathEffect, CrystalFlash, SparkGFX); moved, ovl62 resplit (now
  186 functions), `make check` OK (ARCHITECTURE.md, Which game is which). A
  sweep for any function whose predecessor doesn't end in a jump finds no
  others.
- **2026-10-19 (log):** the three gate failures listed under Next
  (`func_800BA43C`, `func_800B3E74`, `func_800B7CE8`) were already matched
  on 2026-10-18; Next now says so.
- **2026-10-19 (hand):** `func_800B8A68` (ovl61, 3264 B) matched (fc3176ef)
  from the swarm checkpoint (144/150 blocks), without its `min + k - k`: the
  dead skill value needs a use that combine cancels after flow, now
  `(e->unk10.x + k) - (pos.x + k)` (user's choice; still a no-op term). The
  register spots fell to field-form bases, repeated case-3 bodies instead of
  gotos, `abs()` in both arms, and single-set locals. Five Diff patterns
  rows added, one extended.
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
- **2026-10-10 .. 2026-10-15:** project setup, the overlay splits, the swarm
  and the first matching runs. Those entries are in git history:
  `git show 9c6e0800:docs/AGENT_LOG.md`.
