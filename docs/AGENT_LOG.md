# Agent log

Newest first. Each session appends: what changed, what was learned, what's next.

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
