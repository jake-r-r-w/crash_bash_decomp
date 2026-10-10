# Agent log

Newest first. Each session appends: what changed, what was learned, what's next.

## 2026-10-10 - auto-matching pass, compiler settled

**State at end of session:** `tools/dk make verify` -> 16 targets OK. Game code
1.89% (19,248 / 1,019,036 bytes, 391 / 2,550 functions); was 97 functions.

**Done**
- Compiler tie settled: **2.8.1-psx -O2 -G0** (kept in `configs/USA/project.mk`).
  Same pool, `auto --max-size 128` (200 functions, the default `--limit`),
  no commit, reverted between runs: 2.7.2-cdk matched 71, 2.8.1-psx matched 128.
  All 71 from 2.7.2-cdk are among the 2.8.1 ones (57 extra, none the other way).
- Auto run over every function (`--max-size 1000000 --limit 100000`), committed in
  `346b9dc` (its title says 77, but it also holds the 128 from the 2.8.1 tie run)
  and `693366a` (89). Remaining failures in `.decomp/state.json`: 814 types,
  716 compile, 628 diff, 16 m2c, 7 in-file, 1 full-build.
- `decomp.py` `land()`: when the full build fails, it now bisects the batch and
  leaves out only the functions that break it (tagged `full-build`), instead of
  reverting everything.

**Learned / gotchas**
- `auto` defaults to `--limit 200`: pass `--limit 100000` for a full sweep.
- A function can match on its own and still break the binary: `func_800C8BA0`
  (ovl5f) adds 4 bytes of data/rodata (ovl5f offset 0x4C). It needs its literal
  or constant placed as in the original (likely rodata belongs to another file).

- Then switched to **hand matching only** (`010aed4`): `decomp.py done NAME`
  is a validation gate (scope: one function; lint: no m2c leftovers, hacks need
  `--hack`; function; full build), the pre-commit hook refuses matches made
  outside it, and `auto` is retired (`--legacy`). See `docs/MATCHING.md` and
  the `/match-function` skill. First match through it: `func_800C6828` (ovl5f,
  `d3386d9`; m2c dropped the pass-through `a0`).
- README: states the goal (a source-only Crash Bash: all C, no INCLUDE_ASM,
  byte-identical; PsyQ from Sony objects) and has a Mermaid diagram of the
  matching loop and gate ("How functions are matched").

**Next**
1. `/match-function`, one function per commit. `next` still shows the old
   auto drafts for the 628 near-misses, which are good starting points.
2. Many failures need types: turn the leaked Pogo/Arkenoid/Polar headers
   (`disc/leaked_source.txt`) into `include/bash/` headers.
3. Fix `func_800C8BA0` by hand (see above).

## 2026-10-10 - overlays and leaked-source scan

**State at end of session:** `tools/dk make verify` -> 16 targets OK
(`build/main.exe`, `build/engine.bin`, `build/ovl5d.bin`..`build/ovl6a.bin`).
Game code is now 1,019,036 bytes / 2,550 functions (was ~125 KB / 574): the exe
was about an eighth of the game.

**Done**
- Found the `CRASHBSH.DAT` index in the exe: `{u32 sector, u32 size}` x 993 at
  `0x8004E110`. Code scan of every entry and of the whole DAT/`BASHY.`: 15 code
  files, nothing else.
- Registered them (`add-overlay`, yamls in `configs/USA/overlays/`, sources in
  `src/overlays/<ovl>/`): `engine` (entry 0x3DF, loads at `0x80078C90`) and 14
  slot overlays at `0x800B32B4`, named by their id word (`ovl5d`..`ovl6a`).
  Facts and evidence are in `docs/ARCHITECTURE.md` (Overlays) and the yaml
  comments.
- Engine split into header (textbin), `.text`, `.data`, `.rodata` (jump tables
  only), `sdata` (second writable block), plus a separate `engine_slot` segment
  for the slot image it reserves (stub id 0x5C + the leaked text).
- AST table (`0x80098720`) maps id groups to overlays (Pogo `ovl60`, Arkenoid
  `ovl61`, Polar `ovl62`, probably Tank `ovl5f`); its 10 pointers that fell
  inside ovl62 functions are now function symbols in `sym.ovl62.txt`.
- Leak scan: one block only (40 KB headers, DAT `0x45C9800`); no other paths or
  `__FILE__` strings anywhere. Local copy `disc/leaked_source.txt`.

**Learned / gotchas**
- splat only writes `.s` files for functions an existing C file `INCLUDE_ASM`s,
  and never rewrites `src/`. After adding function symbols to an overlay that
  has no C yet, delete its scaffold `.c` and re-split. And `make check` won't
  notice: `.c.o` doesn't depend on the included `.s` files, so a stale object
  still matches. Use `make verify` (clean) after any re-split.
- Packed segments: `align: 4` is needed on *every* segment in a file, since
  splat aligns each segment's end (`__romPos = ALIGN(..., 16)` by default).
- `make clean` wipes `build/`: keep scratch scripts in `disc/scratch/`.

**Next**
1. Slot overlays: split `.data` vs trailing rodata the same way (store
   evidence), and look for more pointer-only leaf functions (data tables of
   function pointers, as the AST table did for ovl62).
2. Turn the leaked headers into `include/` headers for Pogo/Arkenoid/Polar
   (structs, prototypes in id order), name the AST Init/Plot pairs.
3. Start matching: `decomp.py auto --max-size 128 --commit` (now across 2,550
   functions), then break the compiler tie (2.7.2-cdk vs 2.8.1) with
   `auto --max-size 48` under each.

## 2026-10-10 - project setup and restructure

**State at end of session:** `tools/dk make verify` -> `build/main.exe: OK`.
All game functions are still `INCLUDE_ASM`, except 17 empty functions that splat
writes as C itself. PsyQ is 99.3% covered by Sony's linked objects. No overlays
yet.

**Done**
- Scaffolded the project from the `psx-matching-decomp` skill. Disc copied from
  the user's `disk/` (kept, gitignored) to `disc/CrashBash.{bin,cue}`.
- Split `SCUS_945.70` with splat: 1140 functions, 574 of them game code.
- Compiler: probe tied four ways on 10 leaf functions (2.7.2-cdk / 2.8.1-psx,
  -O1/-O2). Chose **2.8.1-psx -O2 -G0** because an earlier Crash Bash attempt
  recorded 2.8.1 matching 14/16 vs 2.7.2-cdk 5/16. Not yet confirmed in
  this repo: re-test with `auto --max-size 48` under each compiler once a few
  dozen functions match. maspsx `--aspsx-version=2.86` (PsyQ 4.7).
- PsyQ 4.7 identified (`$Id: sys.c` 1998/01/12), library start `0x8002E7A8`,
  432 functions named, one file per object, then 254 Sony objects linked in
  place (`configs/USA/psyq/`).
- `.rodata` split from `.data` (game rodata, library rodata, data); the
  boundaries and evidence are in `docs/ARCHITECTURE.md` and the yaml.
- objdiff support (`make objdiff-config`, `make report`); objdiff v3.8.2 CLI
  and GUI downloaded to `ext/objdiff/`.
- Restructured to mirror khasinski/parasite-eve-decomp (README sections,
  `configs/USA/`, `linkers/`, `src/main/...`, `tools/scripts/`, docs, badges),
  with the local dashboard in place of decomp.dev.

**Learned / gotchas**
- `decomp.py index` kept stale `src`/`seg` paths after files moved; fixed so a
  missing `src` file is re-resolved. If files are moved again, check
  `make report` and `decomp.py progress` agree with the figures before the move.
- Git Bash heredocs passed through the Bash tool collapse `\\` to `\`: write
  Python helpers with the Write tool, not heredocs, when they contain escapes.
- Host has no Python; run every script through `tools/dk`.
- `tools/dk` in shell loops needs `</dev/null` (it runs `docker -i`).
- Paths differ from the skill's docs: the skill says `tools/decomp.py`,
  `config/`, `config/symbol_addrs.txt`; here they are `tools/scripts/decomp.py`,
  `configs/USA/`, `configs/USA/sym.main.txt`, exe target `build/main.exe`,
  linker scripts in `linkers/`.
- README badges point at `raw.githubusercontent.com/jake-r-r-w/crash_bash_decomp`;
  no git remote exists yet. Fix the URL if the repo is published elsewhere.

**Next**
1. Overlays (skill setup step 3d): find code in `CRASHBSH/CRASHBSH.DAT`
   (engine module + minigames), its index table in the exe, load addresses;
   `decomp.py add-overlay`, one yaml per overlay in `configs/USA/overlays/`,
   then split rodata/data per overlay. Most of the game's code is there.
2. Look for leaked source text in the disc files (C keywords, `typedef`,
   `# 12 "c:\...`); an earlier attempt found ~40 KB of minigame headers inside
   the engine overlay.
3. Start matching: `decomp.py auto --max-size 128 --commit`, then one function
   per session with `next` / `make diff` / `done --commit`.
