# Agent log

Read this first. It holds the current state, what to do next, open leads and
the gotchas that still bite. Rules that apply to every match belong in
[MATCHING.md](MATCHING.md) (the Diff patterns table), facts about the game in
[ARCHITECTURE.md](ARCHITECTURE.md), and repo rules in
[CONVENTIONS.md](CONVENTIONS.md): move them there rather than here.

At the end of a session, update **Current state**, **Next** and **Open
leads**, add any new gotcha, and add one short entry at the top of the
**Session log**.

## Current state (2026-10-29)

- **Every game function matches**: all 2537 in `main.exe`, the engine and the 14 slot overlays
  (100% of game code bytes, 2026-10-26). All 564 PsyQ functions are Sony's own objects (281,
  plus `2MBYTE.OBJ`), and so is their `.data`, except `bios_1.o`'s, which is C.
  `make check` and `make disc-check` are OK. Live figures: `decomp.py progress` /
  `docs/index.html`.
- **The source is complete.** Every function, all rodata and all data are C. Every function is
  named, and all but one are typed (`progress`: 2536/2537). Nothing is parked, and no `func_`
  or `D_` name is left in `src/`.
  - What still comes from the disc: Sony's `2MBYTE.OBJ` and the engine's two memory images
    (zeros in source builds). The exe header is C (`src/ps1/header.c`). `make hidden` checks
    that nothing else is (ROADMAP-B 4.2, passed 2026-10-29).
- **The roadmap.** [ROADMAP-A.md](ROADMAP-A.md) holds the done work: the old Phases 1-5 and
  ROADMAP-B Parts 1-3 (typing, names and loose ends, documentation; 2026-10-29).
  [ROADMAP-B.md](ROADMAP-B.md) has what is left: Part 4 (the final test), Part 5 (release),
  Part 6 (the PC port).
- **Matching rules.** Matches are C only, natural C first (CONVENTIONS.md, Matching rules).
  - Steering C counts as matched, and the gate refuses inline asm.
  - One recorded exception (2026-10-26): `draw_verts` (`src/main/prims.c`) keeps its three
    original register pins (CONVENTIONS.md).
  - Three matches stay steering: `InitialiseCHASE`, `RaceHeroesBump` and `PlotTransmuterBeam`
    (ROADMAP-A, Names and loose ends).
- **Files.**
  - `main/main` is nine files: `main`, `dbase`, `prims`, `scripts`, `sfx`, `nascalls`,
    `sound`, `music`, `card`.
  - `src/ps1/` holds what only the PS1 build compiles (PORT.md): `header`, `bios_1`, `modchip` and
    `modchip_msg` (gcc 2.7.2), and the 14 `<game>_id` words.
  - The engine is nine files: `engine`, `panel`, `pause`, `heroes`, `gameflow`,
    `levelload`, `camera`, `asttable`, `entry`.
  - `bash` is three files, and `papu`, `bonk`, `n2o`, `hub` two each. Every slot overlay's id
    word is its own unit.
  - Evidence: ARCHITECTURE.md, Files inside `main/main`, the engine, the slot overlays.
- **The compiler.** gcc 2.8.1-psx `-O2 -G0 -fno-builtin` and maspsx `--aspsx-version=2.86`.
  The two gcc 2.7.2 + ASPSX units share one setting (`-fno-rerun-cse-after-loop`, ROADMAP-A
  2.4). `tools/cc1/` rebuilds the cc1 from source byte for byte, for analysis variants only.
- **Docs.** ARCHITECTURE.md opens with How the game works: boot, processes, objects, heroes,
  drawing, maps, scripts, loading, sound, card. The prototype and demo discs' findings are in
  PROTOTYPES.md, and the asset formats in ASSETS.md.
- **Game data.** Every `CRASHBSH.DAT` entry but the 15 code overlays extracts to editable files
  in `assets/CRASHBSH/` and packs back byte for byte, and `card.py` reads and writes the
  memory-card save (docs/ASSETS.md). Edits to placements, menus, cutscene keys and saves are
  playtested.
- **Shifted builds.** `make shift-all` (every binary moved, the memory images zeroed) boots and
  plays, playtested by hand on 2026-10-29.
- **The PC port is starting again (2026-10-30).** A 32-bit port ran but was deleted, with its changes to the
  shared C. The new one is native and 64-bit, for Windows and Linux, with its own copy of the game's C: PORT.md has the
  design and what the first attempt taught.

## Next

ROADMAP-B.md, Part 7, more for the PC port, in its order (2026-11-01, the user's list). Parts 1 to 6 are done:

1. Done 2026-10-29: **the final test** (Part 4; 4.4 closed by decision).
2. Done 2026-10-29: **the release** (Part 5; the repo is public, listed through `.recomp.json`).
3. Done 2026-11-01, marked so by the user: **the PC port** (Part 6), native and 64-bit, for Windows and Linux, with its own copy of the game's C in
   `port/game/`. 6.1 to 6.6 are done: it boots, plays every mode and has sound. Windows only for now, at the user's request: Linux
   stays in the plan, but isn't built. 6.7, finishing it, has saving, any resolution, analog sticks and rumble written, and the user has tried a real controller and it was fine (2026-11-01). What's left is a first
   release. Whether it plays the same as retail is checked by the user playing it.
   The Adventure test passes at 200% (2026-11-01, with a handicap; Open leads). The user calls play testing done. The first release, v0.1.0, is published (2026-11-01, on GitHub, with the user's OK), which finishes 6.7. The latest is v0.1.1 (2026-11-01). 6.8, fully native, is done (2026-11-01): plain maths, direct drawing and the port's own APIs. Sticks and rumble since its pad rewrite still need a try on a real controller. Before a change that shouldn't alter the game, `playtest.mjs same [--all]` makes a baseline with `--save`, in `build/`, and afterwards shows in a minute that frames and sound haven't changed. `node port/package.mjs` builds a release's zip, and publishing another needs the user's OK.

A retail-DAT reader for scratch scripts: `scratch/retail.py` (`each(fmt)`).

## Open leads

- **Port, texture packs' PNGs:** packs can come from anyone, and SDL's docs say `SDL_LoadPNG` is for trusted files. `looks_fine` in `texpack.c` checks the header's dimensions and the file's size first, and the README warns, but a hardened decoder would be the real answer. Tried with 14 kinds of PNG by hand (palette, grey, 16-bit, interlaced, extra chunks) and GDI+'s, not yet with a pack made in a paint program. A chosen pack is saved to the settings file (seen after the user's run); loading it back at the next launch hasn't been checked by a test, as test runs don't load settings.
- **Port, `--level-flags 80` on a boss:** `--mode 31 --level-flags 80` crashes with or without a pack. The flag forces a crystal map a boss level doesn't have; a real game can't ask for that, so it's left alone.
- **Port, one `same --all` difference:** `pause` differed at vblank 1300 once (2026-11-01, antialias off, right after the shader's `centroid` went in), and three full runs straight after it matched. Not explained. If it comes back, it's a timing leak in that run, as nothing else about it changed.
- **Port, widescreen's level edges:** some levels were never modelled past what the 4:3 camera showed: black corners in Pogo Painter, Pogo-a-Gogo and Pogo Padlock, a white patch at the top of Sky Balls, magenta at the edge of mode 19, a white block beside Tilt Panic's crystal arena, cut edges in Beach Ball. Harmless, but they show. Filling them would need per-level backdrops. Seen only in still frames from one moment of each mode's start, not played through.
- **Port, the first-run dialog (7.1):** `disc_first_run` has been seen to start, but its buttons and file dialog need a person to click through, and that hasn't happened. `playtest.mjs image` covers everything after them.
- **Port, the Adventure test (`playtest.mjs adventure`):** passes at 200% with no cheat flags, about 28 million vblanks: every award, all four bosses, the ending, a save and a new run that loads it. But the test's player isn't that good, so of the 140 challenges 70 were won at full strength, 44 against weakened computer heroes (after 6 tries, a little weaker each try) and 26 against ones standing still (after 24 tries). So a win there doesn't show that challenge can be beaten at full strength. Standing still: all of Dragon Drop, 3 of Mallet Mash, 6 of Bash, 6 of Pogo, 3 of Dash, 2 of Fox, 1 of Keg Kaboom. The bosses have no handicap. Real players are in `port/tests`: `ring_play`, `pogoplayer.c`, `bashplayer.c`, `play_nox`. Better ones for Dragon Drop, Mallet Mash and Dash would cut that most. `playtest.mjs challenges LEVEL.CHOICE` tries one in about a minute.
- **Port, the boss player needs NO HARM HERO:** with no cheats only the Bearminator wins in the time `playtest.mjs bosses` allows; Papu, Fox and Oxide (15 falls, in his asteroid phase) don't. It's the player's dodging, so
  `bosses` keeps `--cheats 1`. The user would rather have all cheats off.

- **Port, not yet checked by hand or unwritten:** Adventure after a game's first win (back to the hub from a game is untested), level unlocks and
  Mono. 2D prims have no owners, so they step at 30 fps at higher rates. Scrolling past 10 rows in the launcher
  and fullscreen haven't been seen to work.
- **Port, reads off the end of a table, harmless so far (ASan):** Tank's map at negative cells at the arena's edges, `PolarShadowFrames`
  past a table's end (retail reads the next table; here there can be padding), `AiSkillTable` from a hub mode's byte. A table read across labels
  should be one array, as Fox's and Bonk's now are. These are why `playtest.mjs modes --asan` fails on modes 6, 8, 9, 18, 19, 20 and 37 for now.
- **Port, a face partly behind the camera:** one corner gets a depth of 1 and a position that means nothing, as it isn't clipped at the near plane. Faces wholly
  behind are dropped (`mesh_submit_*`), and these haven't been seen to flash, but real near clipping would need the camera-space positions (`ViewPoint.view`).
- **Port, seen once:** a run with no presses showed the launcher well after PLAY; three more didn't, and the likely cause is real keys reaching a
  test window.
- **Port's saves, tidying:** the save screens still test `CardStatus` for a card that's missing, unformatted or being checked, which can't happen now; those branches
  and their texts can go.
- **Port's sound:** `FX_LEVEL` (25% under the original's sums), `MIX_LEVEL` and the limiter are by ear, and the user never compared it with
  the original in RetroArch. Vibrato (a tone's `vibW`, `vibT`) and portamento aren't played.

- **The GCC 12 PS1 build (dropped; for reference only).** Every game file compiles with mipsel GCC 12
  (`-march=r3000 -msoft-float -mno-abicalls -fno-pic -G0 -O2 -fno-builtin -ffreestanding
  -fno-strict-aliasing -fno-common -fno-toplevel-reorder -fno-zero-initialized-in-bss -std=gnu89
  -DNON_MATCHING`). The linker is `scratch/modern/modern.py`, a `shift_all.Build` subclass. It
  needs `provide()` to drop a line when an override is `None`; that change isn't committed. The
  linker flows each binary from the one before it and keeps the PsyQ .bss block at retail's
  offsets, because its gaps hold libsnd's `_svm_*` variables. The full build boots to a black
  screen. Even with every unit on the matching objects it doesn't boot: Sony's `ssinit.o`
  `.bss` lays out its variables differently from retail. For example, `VBLANK_MINUS` is at
  `0x80077330`, not `0x80077334`, and `_snd_ev_flag` is at `0x80077340`, not `0x80076b30`. So in
  retail those variables were commons that psylink placed, and the object's own order is wrong.
  Next, if it's picked up again: place libsnd's .bss names by retail's list (as the matching link
  does), then bisect main, engine and slot with `scratch/boottest.ps1`. The PC port (ROADMAP-B
  6.1) meets the same C with a modern compiler.
- **`draw_verts` needs its register pins** (proved 2026-10-26; kept as a recorded exception to
  only-C, CONVENTIONS.md).
  - The target is a leaf that saves only s2-s4 (frame 24: two locals at 0/4, then s2, s3, s4).
    It keeps `xy`, `z` and `flags` in `$20`/`$19`/`$18`, and never uses `$15` or `$25`.
  - mips.h has no `REG_ALLOC_ORDER`, so global.c's `find_reg` takes the lowest free register.
    A pseudo reaches `$20` while `$16`/`$17` are free only through a hard-register preference,
    which needs `$20` in the RTL. In C, only `register ... asm("$20")` puts it there.
  - Anything that took `$16`/`$17` away (a pseudo, a clobber) would make them live and saved,
    which the target doesn't do. Without the pins it builds with t-registers and no frame
    (structure 171/173).
  - The original was almost certainly written with these register variables (Sony-style GTE
    code).
- **The steering matches** (`InitialiseCHASE`, `RaceHeroesBump`, `PlotTransmuterBeam`) are
  kept as steering (ROADMAP-A, Names and loose ends). Their rows in MATCHING.md hold the
  analysis.
- **`shell_setup`'s fields** (each overlay's world record's second pointer) aren't decoded. It
  holds the VRAM areas and pool sizes, and may be the same layout as `EngineCfg`.
- **Whether `modchip` and `modchip_msg` are one file** is open. They share a compiler and flags.
- **Prototype debug types.** The fragment's bodiless structs that the code shows are typed
  (2026-10-29). Two stay word arrays: `script_key_anim.quat`/`quat_interp` (a quaternion and its
  slerp table, whose shape is an array) and Polar's `ang[2]` (struct ._23, which nothing reads).

## Gotchas

**Tooling**
- **Data as C: a label inside another object is a field.** The asm gave every
  label its own symbol; in C a label data2c merges into the object before it
  (or that the object's type covers) is undefined at link time. Rewrite its uses
  as the field (`FadeAmount` was `RenderState.modelDp`, `LevelContourHook`
  `LevelSpecs.GetContour`). A field store can change the code where the label
  didn't: `func_8002B760` had to store through a pointer like its twin. The
  splice script reads the build's `.i` files to see what a file already
  declares: build from clean sources first, or a failed try's extern lines
  count as declarations.
- **cc1 variants live in `build/cc1/`, so `make clean` deletes them**
  (rebuild with `tools/cc1/build.sh`; its downloads stay in `ext/cc1/`). In
  Git Bash, `[ -x FILE ]` is false for a Linux ELF such as cc1 (MSYS counts
  only `.exe` files and `#!` scripts): test it with `-f`.
- **Variant scripts for `try --file`: edit the one function, not the file.**
  A plain replace over the whole `.c` also hits other functions' `HeroInfo
  *h;` or `goto next;`, and the variant then fails to build. Cut the
  function's text out first (from `void NAME(` to the next `\n}\n`), edit
  that, and put it back. And `tools/dk
  python3 - <<EOF ... </dev/null` never runs the script: the later
  `</dev/null` replaces the heredoc. Write the script to a file.
- **`BUILD FAILED` with `tee: build/...c.i: Permission denied`** after
  back-to-back `try` runs is a Windows file lock on the build output, not
  the C: run it again.
- **`try`'s `structure n/total` counts differing instructions** (lower is
  better), and a shift in alignment swings it by hundreds; for a big
  function compare `regions` blocks instead. Look the symptom up in the
  Diff patterns table before digging into a pass: `func_8007F314`'s
  base-before-index order was already a row (`T *base = Array; p =
  &base[i];`).
- **`try` adds one to the function's `tries` in `.decomp/state.json`
  each run**, so a script that runs it over many variants should restore
  that file afterwards (`git checkout .decomp/state.json`).
- **Typing a byte-array global as a struct can move its loads.** Its reads
  become in-struct MEMs, and gcc 2.8's alias rule lets a scalar read at a
  fixed address pass a store through a struct pointer but not an in-struct
  one: `D_8009F644.amount` stayed below `gs->otzAdd = 0x200` in pause.c's
  `func_80082590`, where retail hoists it, so that read is
  `*(s32 *)&D_8009F644`. Build and check after retyping a global, and look
  for that pattern where one function stops matching.
- **An overlay that names a symbol its code reaches only at an offset**
  (`D_8009F658.next_process` for splat's `D_8009F65C`) doesn't link: splat's
  `<ovl>_undefined_syms_auto.txt` lists only the labels the code touches.
  Add a `PROVIDE` to `configs/USA/undefined_syms_manual.txt`.
- **A split whose rodata starts with an asm item needs 4-aligned objects.**
  An `INCLUDE_RODATA` item's `.align 2` doesn't survive the linker
  script's `SUBALIGN(2)`, so after a file whose rodata ends at 2 mod 4 the
  next one lands 2 bytes early (the whole exe shifts). psylink 4-aligned
  objects: the main segment now links with `SUBALIGN(4)` (`subalign: 4` in
  main.yaml); the overlays still use 2 (bash 1), so check a split there.
- **Splitting C: K&R definitions and temporary macros.** `split_unit.py`
  takes the moved functions from `decomp.py`'s definition finder (a K&R
  definition, whose parameter lines have `;`, was missed once and its stale
  `.s` sent it back to the old unit in the index, which then dropped it),
  and copies a file-scope `#define` only if no `#undef` follows it (a
  temporary macro copied into the new file broke every local of that name).
- **Splitting a unit: its header's inline helpers go with their users.** A
  `static inline` helper in the old unit's header that calls a function now
  declared in the new unit's header sees no prototype (the headers are
  included in unit order) and gets "conflicting types"; reordering the
  includes just moves the problem when the new header needs the old one's
  types. Move each helper to the header of the file that uses it, and point
  other headers' direct `#include "bash/<old>.h"` at both new headers.
  `split_unit.py` moves the ones only the new unit uses (with the comment
  above, signatures over several lines too); a prototype's comment moves
  only if it starts its own line (a trailing one belongs to the line
  above).
- **Renaming a unit of matched C leaves the index on the old name.**
  `decomp.py index` moves a function to a segment only through its `.s`, and
  splat writes none for matched C, so after renaming a unit's `.c`, header and
  yaml paths, set the `.c` aside for one `make split` (splat then writes every
  function's `.s` into the new folder and a stub `.c`), put the real `.c`
  back, and `index`. Delete the old asm folder only after that: its stale
  `.s` files, under a segment no yaml has any more, become bogus bare-address
  entries if `index` sees them.
- **A new name an original header declares must take its types.** When a
  function gets a name the recovered headers (`polar.h`, `tank.h`, ...)
  already declare, `decomp.py rename` keeps only that declaration, and a
  definition with other types fails as "conflicting types" (rolled back).
  Give the definition the header's types first (`TestBeamChance(u32
  player)`). `proto_apply.py` lists such names before `--apply`.
- **Renaming a function breaks a `D_` alias of its address.** An overlay's
  generated symbol lists (`linkers/<ovl>_undefined_*_auto.txt`) name an
  outside address after the symbol files only, so source that took a
  function's address as data (`= D_8001C378;`, `extern u8 D_8001C378[]`)
  stops linking once the function is named, and `decomp.py rename` rolls
  back. Write the function's name there in the same change (it can't link
  as `func_<addr>` before the rename either). `proto_apply.py` lists such
  uses and refuses `--apply` until they're fixed.
- **A park can gain a matched twin later, and nothing revisits it.** When a
  function matches, the gate's propagate step tries its C on the identical
  copies; one that fails is marked `copy_failed` (and `copy_failed_stage`)
  in `.decomp/state.json`, and differing prototypes (`VECTOR *` against
  `s32 *`) are enough to fail it. `func_800BFEE0` and `func_800B7190` stayed
  unmatched for one to two days after their dino twins matched. `decomp.py similar
  NAME` shows a twin as similarity 1.00: port its C by hand with the local
  types.
- **A matched function's `.s` exists only from before it was matched.**
  splat writes `asm/nonmatchings/**/NAME.s` only for functions the C file
  lacks. Until 2026-10-22 `rename` deleted the old file, losing the target
  asm (`regions`, `vars`, `similar`) and the rodata ownership behind the
  dashboard's data % (1.38% -> 1.27% over that day's renames). `rename` now
  moves the file. To re-create missing ones, split a copy of the overlay's
  yaml with `asm_path: build/asmall`, `disassemble_all: True` and the
  linker/undefined-syms paths moved under `build/asmall`, then copy the
  missing NAME.s files from `build/asmall/matchings/` (byte-identical to the
  originals); 300+ were restored this way.
- **The pre-commit hook refuses files over 2 MB** (a catch-all for game
  data) except docs/progress.json, whose history grows by a line per commit
  (exempt since 2026-10-21; it once blocked 5 swarm matches). A match the
  gate refuses at commit stays in the swarm's results/:
  `run --resume RUNID --only ...` gates it again.
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
- **`open(p, "w").write(f(open(p).read()))` empties the file:** the
  `open(p, "w")` truncates before the argument is read. Read into a
  variable first, then write (it wiped `tank.c` once; `git checkout`
  restored it).
- **Write files that hold backslash escapes with the Write tool**, not a
  heredoc through the Bash tool: twice on 2026-10-25 a doubled backslash
  came out single, which broke a generated patch's `\n` strings and joined a
  macro's continuation lines.
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
  (`func_80085890`: two `func_80085890()` calls in race/dino). Fix the
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
  for `ps1/modchip_msg`).
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
  contain it. `swarm.log` timestamps are UTC.
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
- `tools/dk` right after `git checkout` sometimes hits a Windows
  `PermissionError`; retry. Writing a source file (`main.c`) from a script
  in a loop can hit it too, now and then: retry the write, and check the
  file is back to its base after a variant loop (`git diff`).
- **main.exe has no target `.s` files** (it is fully matched, so splat
  writes none): `make diff`, `regions` and `vars` can't align a main.exe
  function. To rework one, `objdump -d -r` the current object as the target
  and diff a variant's disassembly against it.
- `make permute` runs in its own container: stopping the background task
  doesn't stop it. `docker stop` it before running `done`.
- Stopping `swarm.mjs permute` early (a STOP file or Ctrl+C) saves nothing,
  and its STOP file then blocks the next run until deleted. Copy the best
  `output-SCORE-N/` from `<swarm>/permuters/hN/build/permuter/NAME/` first,
  apply its `diff.txt` in that clone, measure it, and write `best.diff` and
  `best.json` (`git diff` in the clone, against its HEAD).

**decomp.py**
- `done` refuses when another function in the file is also un-`INCLUDE_ASM`ed.
  For a batch, keep the full file aside, `git checkout` the source, apply
  and `done` one function at a time. `done` also applies the same C to
  byte-identical copies in the file and commits them together: skip those.
- `done`'s scope check compares names: commit renames before matching.
- `make diff` shows at most 40 rows. For the whole function, `decomp.py try
  NAME --file F.c` (a draft swapped in) or `decomp.py regions NAME`.
- Giving a core function its prototype can break old `auto` matches that
  call it with no arguments (m2c dropped a pass-through `a0`). Pass what
  the argument register holds at the call; the code doesn't change
  (`KillNAS((NAS *)arg0)` in six overlay Init functions).
- A new prototype can also change a matched caller's code through the
  type it assigns to (`D_800C5BB0` had to become `NAS *`). Run `make check`
  after adding one.
- Every inline asm spelling, register pins included, is a lint error since
  2026-10-22 (only C), in macros and inlines too; `gte_*` macros are exempt.
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
- **A `HeroInfo *` parameter used as `base + (s32)p` is a hero index.**
  `SetHeroAction` and `SetHeroSeq` take `s32 player` since 2026-10-25 (335
  and 135 casts dropped, `make check` OK each time), and so do the handlers that only
  pass it on (`func_800C7E68`, `func_800C7EB0`, `func_800BC948`,
  `func_800BCA18`). Type new ones `s32` from the start. `PlotSpiralSat`'s one
  cast call of `SetHeroAction` stays: its file has no prototype in scope, and
  the cast makes that call void so jump2 can't merge it with the others.
- The leak's prototype order follows code order inside a file, so the
  neighbours of a named function are good naming candidates.
- AST entry: `{id, Init, Plot}`, `0x80026D00` is the empty stub; pointers
  below `0x800B32B4` are engine functions.
- The GTE table's `ll`/`lc`/`sqr` entries come from encodings, not a retail
  match: if one mismatches by a bit, check `tools/scripts/dmpsx.py` first.
- Grepping asm for `4A`/`4B` opcode bytes also hits ASCII in data ("MARK").

## Session log

Newest first, a few lines each. Details are in the commits.

- 2026-11-01: The texture pack option moved to a new Mods section in the launcher (the main and pause menus' Video pages are back to 9 rows). Scrolling textures can now be replaced: named and saved at rest. A pack of untouched originals draws identically in all 41 modes. A dump of all modes gave 3024 textures, which became an inverted test pack in the ignored build folder.
- 2026-11-01: 7.10 Texture packs: Texture packs (7.10): `packs/NAME/*.png`, named by texture hash, chosen in the launcher's Mods section; `--dump-textures` saves the originals from the player's own disc. Off is unchanged (`same --all`). Broken files fall back to the game's own.
- 2026-11-01: The user tried UPSCALE: small text too smooth, large text with artifacts. Text (`DB_Font`) and the panel's digits now stay as they were under UPSCALE and SMOOTH, by `GfxTex.sharp`.
- 2026-11-01: 7.5 Upscaled textures: The user chose this over packs: no AI, a toggle, and the only setup is the disc next to the exe. Every mode ran at 4X with widescreen on, and a 300-frame run took the same time at any setting. 4X doubles the atlas each way, about 512 MB more while it's on (half on the card, half in memory), and scrolling textures stay unscaled, as each position is its own copy. Still frames only: not seen in motion.
- 2026-11-01: `--level-flags 80` with `--mode` starts a level's crystal challenge as a one-player game, on the level's other map (`LevelInfo.levelRecAlt`; the gem and relic flags don't change the map). In a test run's computer-played demo, a crystal challenge is lost from its first frame, FAIL across the screen: it's for a person. Press through its rules screen, with `--bot` to have it play.
- 2026-11-01: 7.2's seam specks: with multisampling, a triangle just touching a pixel was read at the pixel's middle, outside itself, picking up the next atlas texture or a clear texel. `centroid` in the fragment shader's inputs fixes it. Measured as pixels outside the colour range of their neighbourhood in the off frame (blending can't do that): Crashball at 2X went from 365 to 13, at 4X from 136 to 13. Up to about 15 a frame remain, mostly blended edges. Still frames of three levels only: not seen in motion.
- 2026-11-01: 7.4 Widescreen: `gfx_view_extra`, `clipX0`, `HudSpread`. Every mode's been seen at 16:9 in still frames, and one at about 21:9, with no crash, but it hasn't been played.
- 2026-11-01: 7.3 Texture filtering: `gfx.c`. Roadmap had said it needed gaps on the atlas: it already had them. What it needed was clear texels' colour.
- 2026-11-01: 7.2 Anti-aliasing: `gfx.c`. Changing it live rebuilds the pipelines; stepped through every value in the launcher.
- 2026-11-01: 7.1 Easier setup: `platform/discimage.c`, with `--image` and `playtest.mjs image`. The prompt and file dialog need a person to click through: they've been seen to start, not driven.
- 2026-11-01: ROADMAP-B has a Part 7, the user's list for the port: easier setup, anti-aliasing, texture filtering, widescreen, texture packs, sharper icons and UI text, modern lighting, mods and online play. Checked widescreen first: a 16:9 window shows the 4:3 picture with bars at the sides (`resolve_size`, `fit_picture`), and `--render 1280 720` shows the same view stretched.
- 2026-11-01: Port's C cleaned up by `-Wall -Wextra` (on for good now, at zero warnings; `-Wno-missing-braces` for the flat data tables) and clang's analyzer: unused variables, calls through wrongly cast functions (`ApplyContour` was void and read for a result), garbage reads, too few arguments, a divide by zero, and about 40 checks for null followed by using it anyway. `same --all` stayed the same throughout, so most of these are on paths no test reaches and haven't been seen to run. What the analyzer still says is false alarms: `ref_get`'s -1, and one each in `bossbot.c`, `maths.c`, `draw.c`, `gfx.c` and `draw_gouraud_quad`. To run it: `clang --analyze` per file from compile_commands.json (`-Xclang -analyzer-checker=core`). The matching's leftovers remain: `new_var` names, `temp_` names and `do { } while (0)` wrappers.
- 2026-11-01: [v0.1.1](https://github.com/jake-r-r-w/crash_bash_decomp/releases/tag/v0.1.1) is published, with the user's OK: the floor, spark and sound fixes and all of 6.8. Its zip was tried unzipped, with and without `CRASHBSH.DAT`, and with `same --all`, `menu` and `bosses`. Sticks and rumble weren't tried, which its notes say. `same` had passed runs that never started, on the last run's files: it's fixed. `gh release create --target` needs the full hash.
- 2026-11-01: The user marked Part 6 done, which leaves nothing on ROADMAP-B. Linux stays in PORT.md's plan, unbuilt.
- 2026-11-01: The VRAM bookkeeping is gone from the port (`vram_*`, clut lists, `GfxTexInfo`'s rect, the shell settings' VRAM fields). `texture_scroll_step`, which looked like frame-swapping textures not showing, was an uncalled second copy of what `texture_update_anims` does properly, so it went too. `same --all` unchanged.
- 2026-11-01: 6.8's packets are gone: the game calls `bash/draw.h` directly (`DrawFace`, `draw_face_quad`, `draw_plain_quad`, `draw_line`, `draw_mesh_*`, `draw_viewport`), a layer is a plain number, and there are no order tables, per-frame prim buffers, draw environments or `port/psyq/`. The matrix functions lost their Sony names too. `same --all` (44 runs, new, a minute) shows two differences, both fixes: the credits' spark trails now draw (`HubDrawSparks` linked itself into a table nothing read), and Mallet Mash's hero sounds and Tank Wars' Moe and Joe recoils pan from the hero (a raw `hero_eng + 0x10` was reading a pointer as a position). Debug and release builds agree. The suites all pass on it: `modes` 39 of 39, `bosses` 4 of 4, `menu`, and `adventure` to 200% with its save and reload. 6.8 is closed.
- 2026-11-01: 6.8's GTE is gone: `platform/maths.c` has the view, `project_point` and its `ViewPoint`, `points_order`, `points_facing`, `view_transform`, `colour_fade` and `vector_towards`, by the same integer rules. `same` 14 of 14, `modes` 39 of 39, `bosses` all. Bear's test limit was too short (it wins at vblank 8378, the old code too), so it's 9000 now. `MulMatrix0` and `ApplyMatrixLV` still leave the view's rotation as their matrix, which the old ones did and nothing has shown the game doesn't need.
- 2026-11-01: 6.8 begun. `playtest.mjs same` compares frames and sound of 14 modes with a saved baseline, about a minute, for changes that shouldn't alter anything; runs are deterministic enough for it, sound included. Gone: the no-op stubs, the export table, libcd, libapi, libetc and libpad (`archive.c`, `clock.c`, and `pads_read` asks the host directly). Pads are read a vblank fresher, which `same` doesn't see, and sticks and rumble took no test here beyond the build: a person should try a controller. The sound is `platform/audio.h` now, not libsnd and libspu, with no change to how it mixes.
- 2026-11-01: Floor tiles (`draw_model_quad`: Space Bash, Papu, Pogo) now draw at the GTE's precise positions with perspective (`mesh_submit_gt4`), not as packets of whole PS1 pixels with no depth, which swam against the models on them. The user found it as "super floaty" in Space Bash, and watched the fix and passed it. It's not in v0.1.0.
- 2026-11-01: The first release published, v0.1.0 (PORT.md, A release): the release build has no console or symbols, finds `CRASHBSH.DAT` next to itself and says so if it can't. On that build `modes`, `menu` and all four bosses with no cheats pass, and Battle and Tournament start with one player; several controllers are untested. The 200% Adventure was on the ordinary build, and needn't be re-run for each build, the user says.
- 2026-11-01: Play testing is done, the user says, and the controller was fine when the user tried it.
- 2026-11-01: The Adventure test goes to 200% (Open leads, for the handicap it takes). Wins now need a player for each game, and looking for why one lost found real port bugs, all pointer-size views: Pogo's pickups were read through HeroInfo, so a player's claim on a pickup or banker read the wrong bytes (`-Wincompatible-pointer-types`, which the build turns off, finds this kind: scan with it after a change like this); `CHASE_KeepInBounds` took an engine as `s32 *`, so Keg Kaboom's arena edge wrote over a hero's link, and only for some load addresses, so a crash that comes and goes between copies of an exe can be this; Dash's dive splash, Dante's Dac; `KillNAS` now lets go of what a hero's holding. A relic is three round wins in a row: a loss resets it. On Windows `_IOLBF` is really full buffering, so a crash's handler flushes stdout, or the log's end is lost.
- 2026-11-01: The Adventure test passes, no cheats, all four bosses by real play. Oxide's asteroid run: each tick `play_nox` tries nine ways of steering by the flight's own rules against what's ahead and takes what costs least. Ring Ding: `ring_play` as a real player; the computer's hands pick balloons at random. Crashes it found, all null or stale accesses a PS1 shrugs off: Path objects overrunning on level close, Polar's force-victory lights and bomb, Race's missile owner, Papu's spawn point past its array, a hero left holding a destroyed crate (the original clears `player` before the release that needs it; found with a hardware watchpoint), and a retry from Oxide's ball game before `hero_move` exists. Also images leaking until the 16384 limit over a long game. `--draw-every N` runs about 260x normal speed.
- 2026-11-01: The Adventure test started (Open leads). Found by the user watching: NO HARM HERO stops every player's score in the ball games, not just the hero's, so a match never ends; and Cortex's and Brio's charge left its
  flames behind, as `PlotBrioPack` read its owner from an unset variable. The compiler's `-Wuninitialized -Wsometimes-uninitialized` now finds none in `port/game` (a one-shot scan; the build's own flags don't ask for them).
  A trophy only counts if `heroCpu` of the winner is 0.
- 2026-10-31: The port's play tests moved out of `scratch/` to `port/tests/`: `playtest.mjs bosses|modes|menu|shots`, with the boss player as `port/tests/bossbot.c` (PORT.md, Running it).
- 2026-10-31: Oxide beaten by real play: `play_oxide` reads each ball's path, gets beside where it'll cross and kicks it to the side he isn't. The old button pattern never kicked, so the ball phase was won by
  bounces. His first phase has no bar and no damage in the matched code either (`InitBossHealthIcons` is only called from `N2OInitBoss`, and `PlotLaserShoot` never lowers his health): it ends when his path does.
  `--sound` plays a test run's sound, for showing one.
- 2026-10-31: The Bearminator beaten by a test run, from a real Adventure: all three phases and back to the warp room. A real bug on the way, found by the user: the hero couldn't hit his robots, as
  PolarHeroCollisions read their flag through the wrong view. How the fight works (`--aim`, `play_bear`): each robot fires one missile a while after landing, and a stunned hero goes over with no recovery;
  a charge drives a robot about 1450 along the lunge, not along the line of contact, and the edge is about 2600 out; a sniper shot flies through the crosshair and only the bear's own box does damage, not
  the sub's; each hit takes a chunk of the ice and lands more robots. Holding the middle of what's left and charging what's in reach beat every cleverer plan.
- 2026-10-31: Boss fights played through by the rules, from a real Adventure through the cheat menu's level selector: Papu, Big Bad Fox and Oxide beaten by real hits and back to the warp room,
  with `--aim`, the pad only, and the user's advice on each fight's order. NO DEATH keeps Moe and Joe alive too: use NO HARM HERO (`--cheats 1`). `csqrt` is fixed point: `>> 6`. `--stall` stops a run going nowhere.
- 2026-10-31: Boss fights, from the user playing: the HUD was hidden as `SetPanelHeroIcon*` moved icons by word index, so "Z" landed on the engine's flags; Papu's minions crashed in
  `PapuExploRange` by a PS1-shaped slot view, and were placed by a PS1 stride. Raw word indexes into engines and PS1-shaped views are still the class to suspect when a rare path crashes.
- 2026-10-31: The original's cheat menu, recovered at the user's asking (`overlays/engine/cheatmenu.c`, a launcher toggle, off by default): retail has its way in, flags and name; the
  rest is from the demo disc's engine (`PAUSE.CPP`, the `{text, var, type, min, max}` table at `0x8008CED4`). TWEAK SETTINGS isn't back: its variables are gone. Not every flag has been played with.
- 2026-10-31: A window coming out of fullscreen, or opening as big as the screen, had its title bar off the top, so it couldn't be dragged: `centre_window` keeps it on.
- 2026-10-31: Video options in the game, at the user's asking: `main/videomenu.c` holds them for the launcher and the VIDEO pages of Options in the main menu and the
  pause menu; they apply at once and are kept. Changing to 60 or 120 fps in a `--turbo` run stops its shots, as stepping needs real time.
- 2026-10-31: Any resolution: `MATCH WINDOW` (render 0 x 0, the new default) draws a 4:3 picture pixel for pixel in whatever the window is and follows a resize, with
  widescreen window sizes, and the window's size and fullscreen remembered. The game's picture stays 4:3; true widescreen isn't attempted. `--window` and `--render` test it.
- 2026-10-31: Saving, on the user's call not to emulate the card: each slot is a checksummed file written safely (`platform/saves.c`, in the settings' folder), and
  `mcrd.c` and the card's icon and title code are gone. A warp room's pause menu saves too. The save screen's counters on an empty slot, which the user read as won
  items, were the panel's placeholders left showing by a PS1-sized stride (`EngineSetSlot3Digits`). ASan found the name's letters were a table split across labels.
- 2026-10-31: 6.7 begun, at the user's asking, with the controller: a pad is an analog one with motors (`pad.c`), so the game's own stick and rumble code runs. No
  controller here, so `--stick` stands in; the user has yet to feel it. It found a heap bug: `mal_set_owner` wrote a raw pointer over a 4-byte file slot, which broke
  on-demand animation blocks (a hub door). Saving and any resolution are next.
- 2026-10-31: Notes that rang on through a pause: the envelope's exponential step was a division, which rounds to 0 at low levels, so a released voice stuck at
  `env 4095` and droned for good, in play too (likely the user's metallic undertone; all 24 music voices were usually taken). It's the hardware's `>> 15` now. `FX_LEVEL`,
  `MIX_LEVEL` and the limiter were set by ear with those drones in the mix, so they may want another listen.
- 2026-10-31: The hub's flicker while walking: a face wholly behind the camera could sweep the screen as one huge triangle (a full flat frame, found by scoring 60
  real-time shots for cells unlike both neighbours), and a model's animated textures stopped gliding between frames, as "another texture" meant "another thing".
- 2026-10-31: The hub, from the user walking it: a gateway's door crashed (a PS1 offset into `obj_dbase`, and a private ordering table with 32-bit links), now
  drawn natively by `draw_clipped_begin`; the ring's limits read a path through `subengine`'s fields, so the hero walked off the platform; `CAMTBL6A` and the
  save name screen stepped by PS1 sizes. The route to a real game (hub, door, rules, Crashball) runs clean under ASan.
- 2026-10-31: From the same testing: a tone's fine-tune was subtracted, not added (Sony's `SsPitchFromNote` read in the linked ELF), so everything with one
  played flat, the scene's clips by 2 semitones. `scratch`'s check: correlate a recording with the bank's sample. Closing a bank now lets its effects play out.
  Models send their faced-away faces when stepping, and each picture between frames decides which show, which ends the gaps in moving mouths.
- 2026-10-31: The user's bug testing: a new adventure's story stopped after its first part, because a script's `handle` was typed `s32 *` over an
  owner's pointer, so its end cleared half the pointer and the scene waited for ever. All 19 parts now play into the hub.
- 2026-10-31: 6.6 done, the user listening: `platform/audio.c` mixes on the device's thread. Their ears beat an old note: the real game's RAM
  shows reverb on (each song sets the depth). Read from Sony's code in the linked ELF (`objdump` through `tools/dk`): a note's volume is squared, effects'
  aren't, `SsSetVoiceMask` reserves voices. Gaussian table and envelope from psx-spx. Test runs are silent and, without `--wav`, their voices never
  finish, so the effects' pool empties: record when testing sound. 6.5 and 6.6 moved to ROADMAP-A.
- 2026-10-31: 6.5, the user testing by hand. Every game mode runs (`--mode N`, TESTING on screen, no time limit;
  `--tile I N` to watch several). Nearly all the crashes were one class: extension and object views with fixed PS1 pads, now by field
  (PORT.md, What a pointer's size breaks). The launcher is now a game menu, `overlays/titles/settingsmenu.c`: the intro's 3D logo,
  categories and a scrolling option list, settings applied live (`gfx_configure`). The facing test uses unrounded positions, which
  cleared holes in small models and most of the user's model complaints. Frame steps no longer blend between different meshes or texts,
  or leave a huge turning model's far triangles behind. Scrolling textures are rotated copies on the atlas. The volume texts are written to,
  so aren't const.
- 2026-10-31: 6.5, frame rates of 30, 60 and 120 by drawing between the game's frames, depth for moving models, and opaque
  full-screen pictures (PORT.md, Running it).
- 2026-10-31: 6.5, from the user's first play: skip loading in the launcher, no black flash between logos (the picture stays
  until the game clears it), team and level menus and lock icons draw, the crate limit works so TNT explodes. First-effect lag was
  whole-row uploads through a 64 MB buffer; now each new texture uploads alone, and a battle shows 3 slow frames in 40 s, all loads.
- 2026-10-31: 6.5, the game plays on the native renderer. It boots through the logos to the menu, and Battle mode's Crate Crush
  plays from the menu with all four heroes; the other games are untested. Added the settings menu, in the game's window, with skip
  intro (PORT.md, Running it). Not done: sound is silent until 6.6, texture scrolling and the hub's render-to-texture are unwritten
  (`texture_update_scroll`, `key_of`), and the attract demo isn't a test path until the games play. Test with `--press` and
  `--launcher-keys`, never with key presses sent to the desktop: they go to whichever window has focus.

- **2026-10-31 (6.5 started):** the first link lists 180 missing names, nearly all PsyQ's. A loose edit in 6.4 had deleted 640 lines of
  `scripts.c`; they're back, and no other file lost code. `VramNodePool` and `VramAreas`, deleted in 6.2 as unused, are vram_init's setup
  data, used through the world configs: they're in git before `25763e02` if VRAM stays.
- **2026-10-31 (ROADMAP-B 6.4):** the static heap arena and `ref32`, mode text translated at load (test: 161 lists, 2,058 records and all
  33 pause tables on the real files), address warnings as errors. Exempt until 6.6 and 6.7: `prims.c`, `nascalls.c` (its VRAM half),
  `music.c`, `sfx.c`, `sound.c`, 106 lines. Left for 6.5: the overlay and engine loads (`load_file_direct` to `SlotBase` and
  `HeapStart`), PsyQ's card calls (`DIRENTRY`), and a heap block split at a file's offset is only 4-byte aligned. Some `keep` and
  `heroLoadArg` values are cast through `iptr` without knowing what the data holds, so check them on the first run.
- **2026-10-31 (ROADMAP-B 6.3):** `GameInfo` is one struct, views use `offsetof`, and `tools/port/views.mjs` checks them. Of
  138 views it reports 33: casts back to the real type or mistyped locals, code 6.4 and 6.6 replace, and two not proven right (`script_control_info` and `HubSparkSet`, laid over the end of a `subengine`). 226 lines of address casts are left, and about
  30 hook fields with empty parens that gnu17 allows. Steering C isn't cleaned up in bulk; 6.5 does it where it gets in the way.
- **2026-10-31 (ROADMAP-B 6.2):** the game's C compiles as 64-bit in the default build, gnu17, with real pointers in its
  tables, prototypes on its functions, and pass-through calls passing their arguments (from the first port's `PASSED` list
  at `7115b3b8`). 236 lines of address casts are left: about 95 in prims and sound (replaced in 6.6 and 6.7), the rest
  word views and file pointers for 6.3 and 6.4. 80 hook fields still have empty parens (6.3).
- **2026-10-31 (retail recordings dropped):** the user will test the port by playing it, so there's no recorder and no
  GitHub workflow, and the steps are renumbered. Seen while trying it: Cross on the network RetroPad never showed in
  `Pads` over 31 taps on PCSX ReARMed, and a level's opening wait ended by itself, so playtest.mjs's taps may do nothing.
- **2026-10-31 (ROADMAP-B 6.1):** `port/` has the copy of the game's C, presets, SDL3 and CTest. The game's C
  compiles as 64-bit apart from 164 data tables, and isn't in the default build until 6.2 fixes them.
- **2026-10-30 (the port restarted):** the 32-bit port and its shared C changes are deleted, and ROADMAP-B
  Part 6 and PORT.md are a new native 64-bit plan for Windows and Linux, with no emulation and its own copy of
  the game's C. The old code is at `7115b3b8`.
- **2026-10-30 (ROADMAP-B 6.3):** the host is on SDL3. The window, pacing and playtest are checked, but the
  sound and controllers need a person to try them.
- **2026-10-30 (the roadmap):** the port moves to modern tooling: SDL3, then after the same-game test a
  64-bit build with no PS1 layout, tests and CI, and a real GPU renderer (Vulkan, Direct3D 12, Metal). The
  steps after 6.2 are renumbered (ROADMAP-B).
- **2026-10-30 (ROADMAP-B 6.2):**
  - The game runs on PC: `port/sdk` has a software GPU, the GTE, CD, pads, card and sound effects, and
    `port/platform/sdl.c` the window. `--playtest 0-32` plays all 33 modes to the end (31 to 33 wins,
    the rest draws by the clock), and it looks right by eye. Music and reverb moved to 6.4.
  - What the game's C assumes, and how the port keeps it, is in PORT.md. The two to know: its data needs
    the PS1's layout (`port/data_order.txt`), and it can't be optimised (`-O0`).
  - Not played by hand yet, and not compared with retail frame by frame (6.5).
- **2026-10-30 (ROADMAP-B 6.1):**
  - The game's C compiles with Clang 22 and GCC 13, and links into a 32-bit Windows exe against stub
    PsyQ functions. It doesn't start the game yet (6.2).
  - `port/sdk/include` holds the PC headers, compared with Sony's by `scratch/layout.py`.
  - Shared C fixes are in ROADMAP-A 6.1. The one to know is `PASSED`, for calls that relied on the
    caller's registers (PORT.md).
  - Gotcha: splat rewrites `include/include_asm.h` on every build, so the PC build sets
    `INCLUDE_ASM_H` rather than editing it.
  - Gotcha: a new global in a slot overlay can change the engine's bytes too (`slot_syms.py`).

- **2026-10-30 (the decomp/port boundary):**
  - Layout in PORT.md: `src/` and `include/` shared, `src/ps1/` PS1 only, `port/` PC only.
  - Seg names changed: `main/modchip` is `ps1/modchip`, and `decomp.py protos` and `diff_funcs.py`
    count `ps1/` as main.exe.
  - Decided: the port is 32-bit first, and 64-bit is ROADMAP-B 6.6. The image's GCC may need
    32-bit support added for 6.1 (not checked yet).

- **2026-10-29 (ROADMAP-B Part 5 done):**
  - The release checklist is done: the licence, the README (cut to the basics), the credits, and
    the docs and history checks.
  - `make split` is quiet, the repo is public, and `.recomp.json` is in place.
  - The progress badges are gone. Only the PC port (Part 6) is left.
- **2026-10-29 (ROADMAP-B Part 4 done):** 4.4 is closed by the user's decision (they watched the shifted
  build play and judged it the same game). The final test is complete; Part 5 (release) is next.
- **2026-10-29 (ROADMAP-B 4.3 passes):**
  - `tools/playtest/playtest.mjs` plays all 33 minigame modes to a win on the shifted build,
    in about 5 minutes, by RetroArch RAM reads and writes with no screenshots.
  - Gotchas:
    - The fast-forward toggle is dropped when it's sent too early: measure `FrameCount`.
    - A late 0x8008 lands in the next level and pauses it: reset `exitFlags` after the load.
    - The rules pages need Cross from the network RetroPad. They accept it even though every
      hero is a CPU.
    - Time boss fights in game frames: at about 9x, wall-clock waits are far too long.
    - `GameState.heroOut` meant "is a computer player": renamed `heroCpu` (with `GameHeroCpu`).
- **2026-10-29 (ROADMAP-B 4.2 passes):**
  - `make hidden` is the scan, and it passes.
  - The exe header is C (`src/ps1/header.c`), and `mal_update`'s stale `#else` draft is gone.
  - Gotcha: the header segment needs a vram that nothing points into (0x8000F800). At 0, splat
    reads the startup code's zero constants as `D_00000000`.
- **2026-10-29 (ROADMAP-B 4.1 passes):**
  - The fresh GitHub clone went through the Quick Start (extract, split, check, disc-check).
    All 16 binaries match, the disc is identical to retail, and the tree stays clean.
  - The test clone was deleted afterwards.
- **2026-10-29 (ROADMAP-B Part 3 done, docs refreshed):**
  - ARCHITECTURE.md opens with How the game works.
  - The reference docs' `func_` addresses are names now. For shared slot addresses,
    `git blame` the line, then take the match commit nearest before it.
  - Stale notes are fixed, and the current state is rewritten.
  - Found on the way: `LoadModeText` is `LoadModeOverlay` (it loads the slot overlay),
    and `D_BFC7FF52` is `BiosRegion`.
- **2026-10-29 (ROADMAP-B Part 2 done):**
  - The demos' `file` class types the archive index and every loader.
    `DatIndex` is a `file[992]`, and the lists are `GroupFile` triples.
  - Both 2.7.2 units share one set of flags.
  - The engine's .bss is zeros in source builds (it boots, and the mode text loads).
  - The three steering matches stay as they are, and the stale naming notes are settled.
  - Gotcha: drop a cast like `(s32)&DatIndex[` only after retyping the field it fills.
    Compare warnings over a full rebuild (touch every .c), both with and without the
    change. An incremental build only shows the files it recompiled.
- **2026-10-29 (ROADMAP-B 1.8 done, Part 1 complete):** the overlays' byte-pointer reads are
  typed: 84 functions down to 5 left on purpose (ROADMAP-A lists them). Trying each typed form
  and keeping it only on a match worked well; fold renames still need a hand pass for
  same-named fields of other structs on the same line and for uses inside macros.
- **2026-10-29 (ROADMAP-B 1.7 done):** `mal_alloc_owned_low` (the old `func_800115D8`) takes
  `(s32 bytes, void **owner, _mal_area *heap, s32 flags)`. Its ten callers pass `&MainHeap`, not
  `(s32)&MainHeap`.

- **2026-10-29 (ROADMAP-B 1.6 done):** the fragment's bodiless structs the code reads have
  bodies: `script_key_frame`, `script_key_subscript`, the entity and camera thread parts.
  `AnimKey` is folded into `script_key_anim`. `CamVec3`, `ObjQuat` and `ObjKey` moved to core.h
  for them. The roadmap's "34 fields" was stale: earlier sessions had retyped all but 7.

- **2026-10-29 (ROADMAP-B 1.5 done):** Spin, Dino and N2O's last byte-pointer reads are typed
  (Dino's winner choice on `GameState`, its contours on `map_map.contour_grids`, Nox's boss keys
  through `SELF_REL`). `raw_offsets.py --src DIR` now scans overlays too. Over every overlay it
  found 84 functions, 375 lines the typed check misses: ROADMAP-B 1.8. Gotcha: `retype.py`
  leaves byte-indexed reads (`g[0xA7]`, `g[i + 0x5C]`) as LEFT lines, so map them by hand from
  `struct_layout --leaves`.

- **2026-10-29 (ROADMAP-B 1.4 done):** every field the code reads is named (ROADMAP-A has the
  numbers). The flow:
  - `unk_fields.py` (new, tested) sorts every struct's unk fields into none / write / read;
  - the unused ones become `padXX`;
  - scans with `fold_views --dry` find the stand-ins that fold cleanly into a core type;
  - the rest are named from their reads (a pattern proposer did the flags, timers and states).

  The 307 store-only fields stay `unkXX` by convention (CONVENTIONS.md). Gotchas:
  - A fold changes a field's signedness when the view's type differed (`DinoObj.scale` s16,
    `N2OBlend.dir` u16): a `(s16)`/`(u16)` cast marked `matching:` restores the load.
  - A fold also renames another struct's same-named field on the same line (stats' `speed`
    became `Xvel`, a pillar's `x` became `XYZ.x`). Grep each fold's diff and the build errors
    for that.
  - Uses inside header macros and macro arguments need a hand rename.

- **2026-10-29 (ROADMAP-B 1.4, Polar and Race):** four Polar stand-ins are leak types:
  `UkaPolarExt` is `EXTtransmuter`, `PolarFux` is `EXTpolarAI`, `PolarBombExt` is
  `EXTpolarbomb`, `WeightDeathExt` is `EXTbeardeath`. The Sniper, Bearminator, boss, link and
  missile extensions and Race's kart, missile and AI extensions are named from their uses.
  `unkXX` uses are at 4,096, declarations at 1,227.
  - Tools: `struct_layout` leaves treat a pointer typedef that shares a struct's name
    (`EXTengtype`) as a pointer.
  - Gotcha: fold_views maps an array member (`s32 from[3]`) onto a leaf (`startpoint.x`), so a
    use that passed the array (its address) now passes the value. Check the diffs of calls taking
    the old array: `&ext->startpoint.x`.
  - Left: `EXTwalrus` (its `grav` is an `EXTGravMotion` where our code passes an `Arc`, its
    `anim` an `EXTanimator` where it reads a script cursor), and `EXTGravMotion`/`Grav`/
    `EXTpolardata`, which don't line up with our stand-ins.

- **2026-10-29 (ROADMAP-B 1.4, more names):** `field_uses.py` (new, tested) lists every use of a
  struct's fields. It renames them all in the header for one syntax compile and reads cc1's
  missing-member errors, so shared names like `unk10` don't mix in other structs' uses. Named:
  - `FlagRecord`'s offsets; `_mal_area`'s first and end blocks; `HitCallbacks`;
  - `ArenaDef`; `EngineRec32`'s hero load and camera mode;
  - the adventure's progress (`won[5][0x21]`: only [0], trophies, is settled);
  - `PanelPrim` (with `EngineBox` folded in) on `subengine`'s head.

  `unkXX` uses are at 4,619. Gotcha: rename every struct of a pair before running field_rename
  and a line is renamed for the wrong one (`LevelRecs[].firstNas`). Rename one struct, run,
  then the next. Fields only cleared in an init are left unnamed: nothing shows what they hold.

- **2026-10-29 (ROADMAP-B 1.4 started, field names):** the prototype's `xyzrf` (struct ._319,
  0x54 bytes) is flags, a position and a `RotScale` (rotation, pad, scale, matrix, has-scale,
  order, dirty: what `rotscale_update` takes). It begins `subengine`, sits at +0xC of
  `EXTengine`, and is `GfxXform`. All three now read `XYZ` and `xform.*`, and `RotScale` moved to
  nas.h with a `u16` dirty (it's only ever stored). `field_rename.py` did most of the uses
  (`unkXX` uses 12,440 -> 5,066). Gotchas:
  - A line where two structs' same-named fields meet gets both renamed (`fux->XYZ`,
    `GfxScreen->XYZ.x`): grep the diff for the new name on other structs and put those back.
  - Uses inside a header macro or a macro argument aren't seen ("no `.unk10` on the line"):
    rename those by hand.

- **2026-10-29 (ROADMAP-B 1.3, exe raw offsets):** `raw_offsets.py` (new) finds the reads the
  progress check's regex misses (`*(T *)(p + N)` on a `u8 *`): 32 exe functions became 12, each
  left for a reason (ROADMAP-A). Gotcha: `retype.py` only finds an ANSI definition. A K&R one
  (`SetScreen(arg0) RECT *arg0; {`) reports "no function", so retype it by hand.

- **2026-10-29 (ROADMAP-B 1.2, stand-ins):** the stand-ins with a recovered type are folded
  (ROADMAP-A lists them).
  - Tools: `struct_layout.header_of` finds a `typedef struct X X;` + `struct X {}` type (the
    recovered headers' form), and `fold_views.drop_typedef` drops a view whose closing line
    carries a comment (`test_fold_views.py`).
  - Gotcha: fold_views' leaves walk into a struct's pointer members (`EXTengtype light_eng[4]`
    mapped as `light_eng[0].EXTid`), so check the `--dry` mapping past a pointer and map by
    hand with `--rename`.
  - Gotcha: two adjacent u16 fields tested together (`flags & 0x10 && occupied == 0`) are
    combined into one word test as fields but not through a cast read (`Update_Tiles`, kept
    `/* matching:`).

- **2026-10-29 (ROADMAP-B 1.2, views):** the overlays' views of engine globals are folded
  (ROADMAP-A has the list). Gotchas:
  - A failed compile left an object behind: the pipeline's `as -o` creates it before
    `pipefail` fails the recipe. `.DELETE_ON_ERROR` in the Makefile fixes that. Before, one
    build printed the errors and the next looked clean.
  - Folding a declaration can change code three ways:
    - an implicit (unprototyped) call passes a constant as an int where the prototype narrows
      it (`pad_rumble_big`'s s8);
    - a call through an `int ()` declaration is `call_value`, which jump2 won't cross-jump
      with the `void` calls;
    - a field read is MEM_IN_STRUCT where a cast pointer read isn't, so it schedules
      differently.

  Each is kept as a `/* matching:` cast. ROADMAP-B's 1.2 is now the stand-in structs.

- **2026-10-29 (ROADMAP-B 1.1, typing):** every game function is typed (113 -> 0). `retype.py`
  did the m2c offsets: the KeepInBounds hooks, the AST Init/Plot functions, the object lists,
  and `FindExtension` results (0x1000 is `EXTobject`, 0x800 `EXTmotion`). The layout arithmetic
  goes through macros: `FILE_AT`/`SELF_REL` in `common.h`, the heap's `MAL_*` in `main.h`. Seven
  functions' byte forms the matching needs are marked `/* matching:`, which `progress` counts as typed.
  Gotchas:
  - `retype.py` scaled `nas->Engine + 0x94`; it now reports such lines.
  - A macro a unit's headers don't define compiles as an implicit function call with no error
    (`chase.c` doesn't include `main.h`), which is why `FILE_AT` lives in `common.h`.
  - N2OInitBoss: an array read (`N2OHeroStarts[3][2]`) can't be scheduled above a struct store,
    but a plain `*(s32 *)` read can (MATCHING.md).
- **2026-10-29 (roadmap split):** ROADMAP.md is now two files. ROADMAP-A.md holds the finished
  work (Phases 1 to 5, and Phase 6's three done items). ROADMAP-B.md holds everything open, in
  numbered parts with a table each: decisions, typing, loose ends, docs, the final test, release
  (RELEASE_TODO.md merged in) and afterwards. `decomp.py`'s roadmap badge reads both files and
  counts table rows ending in `[ ]`. Decided: source builds write zeros in the memory images; no
  PS1 build with a modern compiler (the PC port's 6.1 does that test, so the final test is four
  tests); PsyQ stays linked (no PsyQ-as-C task; Sony's
  unnamed statics dropped with it). The `SfxActiveVoices` lead was stale: that name is already
  gone from `sym.main.txt`.

- **2026-10-29 (GCC 12 build, shelved):** every game file builds with GCC 12, and
  `scratch/modern/modern.py` links them. It boots to a black screen: libsnd's `ssinit.o` .bss
  order differs from retail's (Open leads). Shelved by the user's decision: a PC port replaces
  PsyQ, so the PS1 layout work wouldn't carry over. Fixed on the way and committed (8cd0ae1d):
  `LoadScreenImage` takes the file it loads, and `VictorySeq` is defined before engine.h's array
  of it. Gotcha: in a linker script, a `PROVIDE` of a name beats the script's own assignment to
  it.

- **2026-10-29 (Phase 6: everything shifted at once):** `shift_all.py` (`make shift-all`) pads the
  exe, the engine and all 14 overlays and relinks them against each other until the addresses
  settle (two links). Fixed addresses found and moved: splat's name lists (fallbacks; others'
  definitions; position-mapped .bss and interior labels), `HeapStart` (`0x80078C90` in the retail
  link), `SlotHeapStart`, the slot id word, the pinned PsyQ/engine .bss sections, the header's
  entry point, the mode text pointers. The first disc crashed before anything drew: Sony's
  startup code (`2MBYTE.OBJ`) is raw words with absolute addresses; bisecting (one binary
  padded at a time) put it in the exe, and the tool now assembles a copy naming what it uses
  (identical at pad 0). The second disc plays (front end, pause text, load, save, hub,
  minigames). Gotcha: GNU as writes `break 1` differently from the original `0x0000004D`.

- **2026-10-29 (housekeeping):** dead code out of tools/scripts (`decomp.py auto` and the six
  helpers only it used, `sort_protos.py`, which also respaced headers, three unused functions,
  seven constants, two imports: 300 lines); 113 repeated header declarations removed (60 inside
  one header, 41 owned copies of a leaked header's, 12 conflicting pairs merged to the
  definition's), and `decl_dups.py` with a test that keeps them at zero. The 136 names several
  units' headers declare are the overlays' views of engine globals (`decl_dups.py --views`): typing
  work. Docs checked for stale paths (none real), objdiff.json regenerated, stale build/shift and
  build/retype.json removed. make check 16/16, disc-check, test-tools and the swarm tests pass.

- **2026-10-28 (bookkeeping: Phase 5 closed):** the user dropped the shift retests (Phase 6's
  shift of everything covers them), which closed Phase 5. Phase 6's no-`INCLUDE_ASM` box (data
  100% C) and its remaining-disc-files box (every DAT entry and the card save decode; no movies on
  the disc) are ticked; ROADMAP's Current focus and this log's Next point at Phase 6. scratch/ was
  cleaned (RetroArch's `card/` and `ra/` folders and five helpers kept).

- **2026-10-28 (Phase 5: asset formats closed):** mapdata.py decodes the scripts' six thread types
  (3,333 threads, 12,605 keys; threads can share bytes, so pack writes only edited fields). txt.py
  decodes the mode text's menus: `PlaceRec` lists (161, 2,058 records; an x of 0x8000 is a centring
  flag, which hid them from a first survey). card.py reads and writes the memory-card save (a card
  image's BASCUS-94570 block: header, icon, 4 `CardSlot`s named from `GameState`, the sum the game
  checks). All round-trip; make disc-check passes. Playtested all three: a save edit (194%), a
  menu panel move (Ballistix's pause title) and cutscene keys (the Eurocom logo, `07D`: shift by
  thousands, its pieces are 1,500 apart; the `uka/` files aren't the scenes).
- **2026-10-28 (Phase 5: asset formats, collision):** the maps' contours decoded in mapdata.py
  (107 in 30 arena maps, 1,200 edges; each edge's length matches its points, all round-trip). The
  hubs have none: with no `LevelSpecs.GetContour` hook the ground is `World_floor_level` plus solid
  objects (`AddPlatformContour`), which is why Crash stayed put when Warp Room 1's placements moved.
- **2026-10-28 (Phase 5: asset formats, level data):** `mapdata.py` (used by mdl.py) reads and
  writes the maps: paths (a 0x30-byte header, not the runtime KeyBlock view: keys from +0x30),
  triggers, placements (a rotation MATRIX and a model id each); all 205 files with level data
  round-trip, and `make disc-check` passes with map.json read in. assets/ got map.json from each
  folder's own model.bin. Playtested: Warp Room 1's placements raised 400 lifted the floor and
  scenery, and Crash stood sunk below it: the geometry is placed, the ground collision is not (it is
  the maps' contour grids / cell lists, still bytes: the next decode).
- **2026-10-28 (Phase 5: asset formats, model files):** `ModelFile` names every header section
  the code reads; RelTable, UvTable, EntryBlock, FaceRuns, TexFaceRuns, ModelSet, ModelEntry and
  MorphModel folded into it and `MeshHeader`; dbase.c, main.c, scripts.c (21 functions) and prims.c
  read models by field. Mesh i is `&m->meshes[i - 1]` (gcc folds it to retail's `i * 0x34 +
  0x24`). `mapscene_setup_tables` fills `map_enviroment` from the prototype's `map_list`/`map_map`:
  the header's +0x18 is the maps, +0x1C the map directory. ASSETS.md corrected: the animation
  entry's +0x10 is its name's hash, not a speed. `entry_block_value` needs the index first in its
  sum (`(idx + base->e)->group`).
- **2026-10-28 (Phase 5: asset formats, texture sets):** the exe's name-and-type item ticked.
  tex.py decodes the animated textures (tracks of frame PNGs, scrolls; all 86 sets byte for byte,
  1,137 frames; checked by eye on 068's electric arc). `TexFile`/`TexImage` give the code the
  header: the image's +0x0C is `u16 eight : 1, clut : 15` (u16 bitfields give lhu for bit 0 and
  lw/srl for bits 1-15, as retail; u32 ones give lw for both).
- **2026-10-28 (Phase 5: the texture views):** obj_texture nests its +0x18 block (`info`,
  GfxTexInfo); four clut entry views are `TexClut`; AnimTex, TexAnimEntry, GfxTexEntry,
  GfxModel and GfxTexSet folded. field_rename.py takes dotted new names (`info.vramX`); where a
  primitive's own `tpage`/`clut` met the renamed field on one line both got renamed (fixed by
  hand). texture_update_scroll reads the VRAM node's x, y signed: `(s16)` casts.
- **2026-10-28 (Phase 5: the exe's raw offsets):** typed with retype.py and by hand, each
  checked by its callers: the prim set pool and `primset_alloc` (`PrimSet`), the music slot's
  `songs[5]`, the large clut list (`FreeNode`), `TexFrameAnim`/`texture_scroll` walks,
  `textureset_init` (Func2A5A4Obj folded), `gfx_setup_xform` and its callers' parent `MATRIX`,
  `dbase_texture_ptr`/`_entry` (`TexEnt12.pos`). 106 to 97 functions; the rest are file
  formats, heap arithmetic and the export table. Folding GfxTexInfo flat into obj_texture
  failed by one `addiu`: the original points at a nested struct at +0x18, so obj_texture
  now nests it as `info` (field_rename.py with dotted new names; the lines where a prim's own
  `tpage`/`clut` met it fixed by hand).
- **2026-10-28 (Phase 5: the tools, then the overlays' GameInfo):** the scratch scripts are tools
  now: `struct_layout.py` (layouts from cc1 -g), `retype.py` (a byte pointer's offsets as fields;
  LEFT lines for arithmetic the type now scales or byte reads it left), `fold_views.py` (views
  onto one struct, traps through `t_` names, `--rename` passes), `diff_funcs.py` (every differing
  function); tests in test_struct_tools.py. With them: the overlays' 43 GameInfo views folded
  (GameState moved to nas.h for Chase; +0x75 and battleWins unsigned; N2OPuck is a ball
  extension, kept), 106 overlay functions' raw GameInfo reads as fields, and seven exe dbase
  functions typed. Gotcha: a retype that still matches proves nothing about the type (it can
  change only the signature, or the offsets can fit another struct): check the callers before
  keeping a guess. The fold can't rename through macros: their uses show as `left:` on the
  macro's call line; fix the macro body.

- **2026-10-28 (Phase 5: the exe, partway):** placeholders gone (GetVideoMode, CdStatus,
  SsUtReverbOn, CdLastPos; the inlines named after what they inline). One `obj_texture`
  (TexDst2A84C, AnimEntry folded); `PadState` everywhere (PadStick, PadInput and 14 overlay pad
  views; the stick angle a union because two games read its low half as a field; the rumble the
  existing `PadRumble`); `DrawBuffer` for DrawBuffers and cdb; the script threads and headers
  typed; 20-odd functions' byte parameters typed. Tools in scratch/: `retype.py FILE FUNC VAR
  TYPE[:member]` (a variable's offsets as fields, from cc1's layout; it warns about arithmetic
  left that the new type would scale), `fold.py CANON VIEW...` (any views onto one struct, by
  offset, in rounds; clashing names go through temporaries first), `rename_pass.py`. Gotchas:
  after retyping, `VAR + N` scales by the struct (check the LEFT warnings); an `*(T *)&x.f`
  cast is a different alias class from a field of type T (titles' and Bonk's pad halfwords
  needed real fields); a file write can fail mid-pass on Windows (rename_pass retries).

- **2026-10-28 (Phase 5: the engine typed, item closed):** `GameState` (main.h) is GameInfo's
  layout, each field the global at its address; the engine's 23 views folded (`GAME`). The
  prototype's own types took over where they exist: `hub_info`/`HubTableEntry`/
  `hub_warp_info`/`HubGateway` (WarpRooms; `ModeRequirementMet` takes a u16, which is why its
  callers read a halfword), `threaddata`/`partdata`/`EXTscriptdata` (the script emitter),
  `script_thread`, `sub_string_info` (TextTemplates). Data: `ModeDbaseLists` (96 records:
  `AdvStatsFiles` was its tail; its pointers were bytes, `bytes_ptrs.py` now finds pointers
  inside objects), the panels as `PlaceRec` lists (XPanel + XItems were one list; text panel
  items and pause records are PlaceRec), the cutscene mark and cue lists (two arrays held the
  next list), `LoadingIconEngine` one `EXTengine` (LoadingIconObj was its sub), the card
  slots `CardSlot`, the pointer tables. Gotchas: a fold can turn a byte index into
  `field[i]` that adds base and index the other way round (a register swap: keep
  `((u8 *)g)[i + 0xNN]`); a `struct X` first named in a prototype has prototype scope;
  `typedef struct { } X` has no tag for `struct X`.

- **2026-10-28 (Phase 5: the engine's raw offsets):** 11 -> 4. Typed: `NasPoolLast` a `NAS *`,
  `CpuHeroChoices` read by row (`(&CpuHeroChoices[w * 4])[n]`: its rows overlap, n runs to 5),
  `CompletionDigitSrc[4][4]`, LevelRequirementMet's sums by field, EngineGameView's `pause`,
  InitScriptFX's thread as `script_thread`, CupCurrentModeType, SceneModelLoaded, and the
  model bank indexed in InitialiseHeroes and UpdateLoadingScene. Kept, with comments: a byte
  pointer stepped down through an array (indexing the field drops it), counters apart from
  the loop index, an int sum's operand order. ROADMAP's "the engine's read GameInfo" was wrong:
  only UpdateOutMessage did, and a view field matched.

- **2026-10-28 (Phase 5: the camera's type):** `GfxCamera` is `camera_info_tag` (the Oct 9
  prototype's name; 184 bytes), in core.h with its fields named (`eye`, `at`, `dist`,
  `shakeTime`, `xform.angle`, `view`, `rot`, ...). `MainCamera` is one (`u8[368]` was two
  cameras' room: `MainCameraUnused` after it), and the overlays' eleven views of it
  (EngCam, PogoCam, PolarCam, BonkCam, ChaseCam, FollowCam, TankCam, HubCam, OvlTextView,
  View, CamState) are gone. cam_build_view, cam_set_projection, CamFromPath1/9 and the
  camera hooks take a `camera_info_tag *`. Gotcha: a view's `rot` (the angle at 0x54) still
  compiles against the camera's `rot` matrix at 0x98; `make check` caught it.

- **2026-10-28 (Phase 5: the last data names):** main.exe's 39 and the engine's 33 unnamed
  blocks named: `GameInfo`'s gaps (`GameHeroes[4]`, `GameTeammates[4]`, `GameCupRound`, ...;
  `CupMenuAnim` was `GameCup`), `NasBuildStamp` as its string, the card image's tail,
  `ArenaDefs[22]` as `ArenaDef` rows, the panels' item lists, the screen tables, unread
  objects as `*Unused`. The last four labels inside objects became field and element
  expressions. The dashboard shows no unnamed data in any unit.

- **2026-10-28 (Phase 5: raw-offset functions):** `tools/scripts/bytes_ptrs.py` (pointers
  left as bytes in C data; none now). Typed: MapScene as `map_enviroment *` (the
  prototype's type; MapEnvHead was its head) and the map scene functions, the subengine's
  +0x70 union (draw_object_model's blend and frame), SceneMark as `MarkCtx *`, the object
  and camera list heads as `NAS *`, the rumble setters, the ambience (`Ambient_data`), the
  cup trophy functions, the mal header helpers (`(mal *)p - 1`), the small node pool
  (`BlockRef`), the dbase setters, PadState's 0x2C..0x40 words. Engine 24 -> 11, exe 94 ->
  62. Gotcha: draw_object_model's flags word must stay a plain deref (`*(u32 *)d`): as an
  in-struct field sched1 moves its load past the PrimDepthLimit store. Then the overlays'
  eight views of MapScene folded into map_enviroment (GfxScene's `viewport` was the
  map_map's clear colour and flag), chase.c's extern too; draw_map_scene walks the subs.

- **2026-10-28 (Phase 5: globals and typing, second pass):** `tools/scripts/field_rename.py`
  renames a struct field's uses by cc1's missing-member errors (names like `unk6C` that
  many structs share). subengine +0x6C is `union u6C` (dbase / text / ptr), +0x74
  `entity`; the fx buffers (`FxDef` for three views, `SubFx`); the script threads, the
  pool inlines (four copies, one pair), `ScriptCommand`; `plot_script_nas` took a `NAS *`
  but is a subengine's hook. Blobs split: the pause screens (`PauseTexts`, `LangMessages`,
  19 `HudEntry` lists), `FileGroups[130]`, `HeroActionTable`'s tail (`HeroHitSfx`), the
  credits' state. Names by family: 113 hero sequence records from `HeroSeqTables` and the
  enum, 34 cutscene records from their model bank ids, 28 `Lang*` tables. 18 sequence
  words data2c read as addresses are numbers. 565 -> 77 `D_` names. Gotchas: data2c can
  leave a pointer as bytes in a u8 array (ShellModules, ComboSeqs, VramNodePool, LangModeTypeTexts):
  `scratch/bytesptrs.py` scans every u8/s8 array for an aligned word equal to a symbol's
  address; separate NTSC/PAL screen mode globals, like the hero anim slots, can't be one
  array (a shared lui).

- **2026-10-28 (Phase 5: globals and typing):** labels inside objects typed: the boss's
  record (`BossState`), `GameOptions` (music, sfx, stereo, vibrate), the crate and
  pickup counts out of `HitSfx`'s tail, `AiPlayerSkill` one type (`bash/aiskill.h`;
  four overlays had copies). 103 globals named (664 -> 565 left). Typed off raw offsets: `GfxCamera` from
  `cam_init`, the loader's queue (`LoaderState` head/tail, `PosList` was the same
  type), `mal_alloc` and the mal tick count, the NAS list functions, the scripts
  (`script_control_info`'s threads/flags/handle, `script_handler` lists), the prim
  lists (`PrimList`), the collision box (`SetEngineMotion` is `SetEngineColBox`), the
  dbase anim chunks (`DbaseAnim`). Engine 27 -> 24, exe 128 -> 100. Gotchas: a struct
  for globals that were separate labels can shrink code (one `lui` for all the
  fields: the hero anim slots stay eight globals); replacing a `(u8 *)p + 0x10` cast
  with a struct pointer cast scales the offset (use `&p->field`).

- **2026-10-28 (Phase 5: the game state, main.exe's functions):** `GameInfo`'s bytes are
  globals around its labels (the user chose labels as globals of their own; ROADMAP);
  converting its constant-offset users to those globals changed code everywhere, so
  they stay. Then 447 main.exe functions named from their code (2204 of 2537 now), the
  nine Sony ones by their ELF names, the script table's entries by function, and nine
  engine words data2c had read as offsets into functions (`&func_8008FF94 + 0x6C`) as
  the halfword pairs they are. Gotchas: a function the engine names by a `D_` label
  (`D_8001C7FC`) must be named as the function before a rename, or the link loses it;
  the rename tool writes engine-range data names to sym.main.txt (move them). Then 231
  engine functions named (2435 of 2537): the default hero actions from the leaked
  HeroMode_nums, the process functions by their process_info slots, the rest from
  their code; the overlays' D_ references to engine functions (HeroNoJump ..) name them.
  Then the last 102 from their callers and tables: every game function is named. Then 39
  globals named (WinCounterState .., HeadOfCameras, VramAreas ..), and 31 hero sequence
  words data2c had read as addresses (`&D_800A0000` is HEROcmd_SFXhandled, 0) written as
  numbers. Gotcha: `decomp.py rename` failed on main.exe data names for no clear reason;
  sed over src/include plus a sym.main.txt line, then `make check`, did it. Then the model
  bank as `obj_dbase HeroModelBank[DB_COUNT]` with `enum DbaseIds` (bash/modelbank.h,
  included by nas.h; the dbase types moved there): 136 slot labels are `&HeroModelBank[DB_x]`.
  Gotchas: a label inside a record can be a pointer *field* read (`D_80054944` was
  `textures.tex`) or a sequence word that only looks like an address (Papu's `0x8005408C`);
  a macro that did byte arithmetic (`TANK_MODEL`) has to stay a `u8 *`. Then the exe's last
  data asm: bios_1.o's .data as C, the $gp word and the .bss in their files, the file padded
  to 0x69800; data is 100% C. Gotcha: cc1 2.8.1 emits a file's variables in the order of
  their *first declaration*, so a header that declares two globals in the other order swaps
  them (main.h had `D_8006E9F8` before `D_8006E9F4`). Then the engine split closed (no more
  evidence will come) and the engine's 0x14-byte header as C (`entry.c`'s `EngineEntry`,
  a `ModuleEntry` in `.text` by section attribute, ahead of `engine.c`).

Older sessions (2026-10-10 to 2026-10-27: the matching, the overlays and engine to source, the data
to C) are in the git history (`git log`) and, by result, in ROADMAP-A.md.
