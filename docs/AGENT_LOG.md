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
   The Adventure test passes at 200% (2026-11-01, with a handicap; Open leads). The user calls play testing done. The first release, v0.1.0, is published (2026-11-01, on GitHub, with the user's OK), which finishes 6.7. The latest is v0.2.0 (2026-11-03). 6.8, fully native, is done (2026-11-01): plain maths, direct drawing and the port's own APIs. The user has tried sticks and rumble on a real controller since its pad rewrite, and they're fine (2026-11-03). Before a change that shouldn't alter the game, `playtest.mjs same [--all]` makes a baseline with `--save`, in `build/`, and afterwards shows in a minute that frames and sound haven't changed. `node port/package.mjs` builds a release's zip, and publishing another needs the user's OK.

4. Part 7 (2026-11-03): 7.6, sharper icons and text, is done (ICONS: CRISP; the game's own lettering stays). 7.11, widescreen level by level, is done (2026-11-03): every level shown to the user and fixed or left at their word, and the eleven crystal maps they picked. 7.8, mods, is done (2026-11-03; PORT.md, Mods, and MODDING.md), approved by the user after playing with it. **Now: 7.12, extra heroes** (the user's, 2026-11-03): with the launcher's EXTRA HEROES on and the player's Crash Bandicoot (USA) disc in a `discs` folder (or the repo's `disc/`), Ripper Roo has a ninth card in the character select and plays Crate Crush as that game's Roo, converted on their machine (`main/bonusheroes.c`); only that Roo, at the user's word (no Crash Bash Roo without the disc). Next the other games and the bosses (ROADMAP-B 7.12). Then 7.9 online play, the last row. 7.7, optional modern lighting, was taken off the roadmap by the user (2026-11-03). At the next release, README.md's Texture packs row becomes Mods.

A retail-DAT reader for scratch scripts: `scratch/retail.py` (`each(fmt)`).

## Open leads

- **Port, a black last pixel row:** at a window that isn't the picture's own shape (1920x1000 with widescreen on or off, 3840x2001 maximised), the bottom row of the picture is black right across, on every level; at 1333x1000 it isn't (2026-11-03). Most likely the 240 lines' scale to the window's height rounding one row short.
- **Port, a test window maximised now and then:** a test run comes out 3840x2001 (maximised) instead of its `--window` size: once in four runs at once after the window-size change, and once alone, right after a build, at 1920x1000 (2026-11-03); the repeat of each didn't. Test runs take the default settings, and only `keep_on_screen` maximises, for a size that doesn't fit, which 1920x1000 does: so either it saw a smaller screen for a moment, or something outside the game maximised the window. A third time it happened in the middle of an `--adventure` run, between two shots in Crashball. A fourth on 2026-11-03, in a 1333x1000 Oxide fight reached through the menus: its shots from vblank 11500 on came out 2668x2001, sheared, and a rerun matched the other build on every shot.
- **Port, the present queue:** at 60 and above with vsync on, `Present` takes the whole vblank, as its queue stays full, and the clock was 60 to 70 ms behind after loading, closing by about 1 ms a second. That could mean a few frames of input lag against 30. Not looked into, and nobody's complained.
- **Port, raw byte offsets:** the snowball bug was a PS1 offset written through a `u8 *`, which lands somewhere else on 64 bits. A grep of `port/game` found no more writes of that exact shape, but `raw_offsets.py --src port/game` hasn't been run for them, as Docker wasn't running on 2026-11-02, and other shapes could hide some.
- **Port, mods' PNGs:** mods can come from anyone, and SDL's docs say its PNG loader is for trusted files. `looks_fine` in `texpack.c` checks the header's dimensions, and `mods.c` the file's size, first, and the README warns, but a hardened decoder would be the real answer. Tried with 14 kinds of PNG by hand (palette, grey, 16-bit, interlaced, extra chunks) and GDI+'s, not yet with a pack made in a paint program.
- **Port, mods, not yet tried (7.8):** what a test run can't do, as test runs neither load nor save settings and look in no mods folder but `--mods-dir`. The user's runs (2026-11-03) showed new mods coming on by themselves, the order and on or off saved (`mod=` lines) and loaded back at the next launch, and removed mods dropping out of the settings; an old `texture_pack=` line becoming that pack on, and OPEN MODS FOLDER opening Explorer, haven't been seen. Zips were made by Node, PowerShell's `Compress-Archive` and Windows' `tar` (libarchive), not by Explorer's menu, 7-Zip or a Mac. A replacement much bigger than its file may not fit where the game loads it: only the mode text files' limit is known and checked (`mode_text_room`). The demo mod is `port/mods/gift-boxes/` (TNT as presents, new art and patches made by `port/mods/gift_boxes.mjs` from the user's DAT), at the user's word in place of Blue Crates, which recoloured 39 of the game's own files and so couldn't be in git; it's installed as `Gift Boxes.zip` in `build/port/windows-clang/mods/`. Blue Crates and the demo pack went to the Recycle Bin. Their tools stay in `scratch/`: `crate_sets.mjs` (sets with the crates, by pixels), `mdl_runs.mjs` (a model's faces by texture and colour, placed objects too), `tnt_hashes.mjs` (the names a texture's PNG needs, worked out from the archive), `grey_top.mjs` (every model face on one image, with its tint).
- **Port, two crashes from before mods:** a test run with `--no-window` crashes once a mode draws (`--mode 1 --no-window`: reading 0x28), and `--mode 43` crashes with a window too. The build from before the mods work does both the same, so neither is new. `playtest.mjs image` passes, as its runs stop at the first frame.
- **Port, `--level-flags 80` on a boss:** `--mode 31 --level-flags 80` crashes with or without a pack. The flag forces a crystal map a boss level doesn't have; a real game can't ask for that, so it's left alone.
- **Port, one `same --all` difference:** `pause` differed at vblank 1300 once (2026-11-01, antialias off, right after the shader's `centroid` went in), and three full runs straight after it matched. Not explained. If it comes back, it's a timing leak in that run, as nothing else about it changed.
- **Port, the packs play test:** 6 of 7 once on 2026-11-01, straight after `same --all`, then 7 of 7 on the same build. Which check failed wasn't seen. 7 of 7 again on 2026-11-03, with packs now mods.
- **Port, widescreen's level edges (see `main/widescenery.c` for the levels with extra scenery; crystal maps have none yet):** some levels were never modelled past what the 4:3 camera showed: black corners in Pogo Painter, Pogo-a-Gogo and Pogo Padlock, a white patch at the top of Sky Balls, magenta at the edge of mode 19, a white block beside Tilt Panic's crystal arena, cut edges in Beach Ball. Harmless, but they show. The list to work through is ROADMAP-B's 7.11. Seen only in still frames from one moment of each mode's start, not played through.
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

**Port test runs**
- **`GameFrameCount` in the launcher steps by 2, and is odd:** something else counts it too, so `GameFrameCount % 30 == 0` never comes. The Mods page counts its own updates.
- **A crystal challenge's test run waits on its rules:** with `--level-flags 80` the rules box covers the screen until a press: `--press 300 4000 --press 360 4000` starts it.
- **`WIDE_HIDE` takes 32 pieces at most** (`MAX_TRIED_HIDDEN`); past that the rest are left shown, so hide a big level's pieces in groups. To find what draws a pixel, `scratch/dbg_tri2.mjs` (`TRI_PROBE="x,y,r"`) names the owner of the triangles under it: Pogo Painter's crystal sky and Oxide's haze weren't the pieces they looked to be.
- **A `--mode` run is the game's demo:** it has a pause bit set (0x2000000), so code that stops while `GAME->pause` isn't 0 never runs there. Polar Push's tilt (`PolarUpdateScene`) stays level whoever stands where, and start doesn't open the pause menu. Check those in a game reached through the menus, by `--press` as `route_to_boss` in `port/tests/playtest.mjs` does.

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
- The port's `--no-window` is for one-frame boot checks only (playtest.mjs): a level run with it crashes in `texture_scaled`
  (no window, no texture cache), with or without any change. Shoot levels with `--window W H`.
- In a test run of Crate Crush (`--mode 1 --skip-intro --skip-loading`) the round's GO is at about vblank 276 to 300; a press
  before it does nothing. A hero taunts on triangle (`--press N 1000`, the engine's `dir & 0x1000`).

## Session log

Newest first, a few lines each. Details are in the commits.

- 2026-11-04: Ripper Roo after the user's play: he walks on the hop of his JUMP alone (frames 13 to 24; a stand-in can name a
  range, `JUMP:13-24`), not its crouch into a tall stance (0 to 12, feet below the floor) and settle; he jumps from 13 to 27. He
  rocks less standing: BREATHE is drawn in toward its frames' middle, keeping 0.6 of its sway (`ROO_SWAY`, bonusheroes.c), his
  height and floor from that. His taunt laughs his own laugh: the base's taunt sound (its moves' third's 0x800A operand, 0xCA for
  Rilla Roo, N2O's taunt the same) is caught in ProcessHeroAnim's sound commands (`extra_hero_voice`) and his laugh plays
  instead, a new one restarting the last (`audio_sample_playing`). His other sounds (pickup, hurt, win) are still his base's.

- 2026-11-04: Ripper Roo in the character select, at the user's word: a ninth card at the red row's left under Aku Aku (the game's 26 placements, then his portrait, then his frame: in the other order the frame hid it), his portrait his hero card on a yellow-to-orange gradient in the top 38 of a portrait's 64 rows (the rest are clear in the game's own; the select draws a row a line of 240, so a texel shows 1.83 times as tall as it's wide), his own model on his stand, a quarter turn to face the camera, and his pick taking his base's card too (Rilla Roo's if it's free), so the menus after, which show heroes by number (Battle Type), show his portrait in its place. The launcher's GAME page has EXTRA HEROES. At the user's word only the Crash Bandicoot Roo: without the disc there's no ninth card. His panel face is his card at its own pixel shape (at 1.6 he looked squeezed) with a short tongue, his head and mouth then the curl at its end, hanging over the health bar. menu and same pass with it off. After the user's play: he stands 620 (Crate Crush's heroes stand 452 to 748, most about 650; he was 820), faces out of the screen on his stand like the others (0xE00: his idle pose turns his body, so it's his face that's square), and as he's picked laughs his own laugh, CrznA from his boss fight's sound pages (type 3; RRooC plays it), PS1 ADPCM at 11025 Hz as CrashEdit plays that game's, through `audio_play_sample` (a voice of its own, at the sound option's loudness).

- 2026-11-03: 7.12 extra heroes started, at the user's word: a hero of its own borrows a free original hero's number (`main/extraheroes.c`), so every table by hero still works, with its model in that hero's place in the game's table of models and loaded with all its animations. The moves stay the base's (game code checks how far a move has got), and each frame is drawn as the extra's stand-in for that animation, by name, the same share through it, with two thirds of its lift off the ground taken away (the user: Roo bounced too high). Points on a frame are the base's, from its own model, but what it holds sits on its head (the user: crates on Roo's head) and so does the head point. Its face on the panel is drawn from its model on the player's machine (`gfx_texture_art`, 4x). At the user's idea, Roo comes from their own Crash Bandicoot disc when it's there: `discimage.c` reads any disc's files now, and `bonusheroes.c` reads level 0x17's NSF (pages and their compression, entries; formats as CrashEdit reads them), his model RS1nG and four animations, and makes a Crash Bash model (a strip a triangle, its colours lit as he stands, his texture) and texture set, in the archive's new entries 992 and 993 and their bank DB_BonusRoo, so El Pogo Loco's Roo is never touched. `open_disc` read a sector into 32 bytes (fixed). `same`: 14 of 14 with extra heroes off. His face on the panel is then his hero card from that game, as the user pointed out: the face his boss fight shows (its tongue is his health), a 64 x 64 sprite of the fight's panel object `ShadC` (in other levels `ShadC` is only a shadow, so a sweep that skipped repeated names missed it), shown 1.6 times as tall as its texels, as that game's 512-pixel-wide screen did. A Crash Bandicoot object's sprites are in its item 5: a header (frames << 16 | 2), the page, then a colour word and a texture word a frame.

- 2026-11-03: Patch mods, at the user's word: a mod's `files/NNN.bps` is a BPS patch to the game's own file (`platform/bps.c`, every count and offset checked, and the CRCs of patch, source and target), made by `port/mods/bps.mjs` (which applies each patch again before keeping it); `playtest.mjs mods` 29 of 29 with four patch checks. Gift Boxes uses them so TNT's lid can be gold: in each of the six crate arenas the set gains an image (`28e68781b1159299` in every one) and the model points TNT's lid at it with the sides' colour, not red; the colour entry is shared with the Japanese TNT's mesh (sides a bomb), so both count as TNT. Nitro keeps its own lid. Checked in the snow and Space arenas. After the user's test, the 3, 2 and 1 a TNT turns into get the gift lid too (their lids were the wooden crate's image tinted purple; only those meshes are repointed, so wooden crates stay).

- 2026-11-03: The demo mod is Gift Boxes (`port/mods/gift-boxes/`), at the user's word: TNT and its 3-2-1 countdown as presents, with a bow on TNT's lid and big polka dots, in new art drawn by `port/mods/gift_boxes.mjs` at 4x. A texture's PNG name is gfx.c's FNV-1a of its pixels then its palette, so it can be worked out from the archive; TNT's is the same in every arena (`1a581a1597bc181d`). The lid (`07e0cd6021f94c95`, the grey crate) is tinted red for TNT and green for Nitro, so its PNG has the bow in red and a crate lid in green; 12 faces of the jungle crate crystal map's scenery tint it warm grey and show both. Checked in the snow and Space arenas.

- 2026-11-03: 7.8 Mod support: the user approved mods (LGTM) after playing with Blue Crates: their order and on or off came back at the next launch, and the removed demo mods dropped out of the settings
- 2026-11-03: 7.8 mods, written, waiting on the user's review: zips (or folders) in `mods/` of textures and `files/NNN.bin` archive files, with a `mod.json`, read in place (`platform/zipread.c`, zlib) by `platform/mods.c`; the launcher's Mods page lists, turns on and off and orders them, and picks up a zip dropped in while it's open. Texture packs are mods now (`texpack.c` reads through `mods.c`, `packs/` folders still listed). `archive.c` lays the archive out again round bigger files. New flags `--mods-dir`, `--mod`, `--dump-files`, `--log-files`; `playtest.mjs mods` 23 of 23, `packs` 7 of 7, and no mods is byte-identical to the build before. Guide: docs/MODDING.md.

- 2026-11-03: [v0.2.0](https://github.com/jake-r-r-w/crash_bash_decomp/releases/tag/v0.2.0) is published, with the user's OK: widescreen with every level's own scenery carried on, the disc image as data, anti-aliasing, smooth and upscaled textures, crisp icons and texture packs, and the snowball, 120 fps and window fixes (notes in `port/release/NOTES-0.2.0.md`). On the release build `modes` 39 of 39, `bosses` 4 of 4 and `menu` pass; the zip unzipped by itself says what it needs with no data and plays with `CRASHBSH.DAT` beside it. Not re-run: the 200% Adventure (the user's word) and `same`. The user has since tried sticks and rumble on a real controller and they're fine, so the notes' caveat about them is gone; several controllers are still untested.
- 2026-11-03: 7.11 Widescreen fixes, level by level: 7.11, widescreen level by level, is done: every row ticked
- 2026-11-03: 7.11.24 Crystal maps: the user approved the eleven crystal maps in widescreen
- 2026-11-03: Eleven crystal maps in widescreen, at the user's pick while they were away; not ticked. Crystal maps are their own files (`levelRecAlt`); gem and relic use the normal map. Jungle Bash 713: backdrop piece 2 mirrored at its circle's ends round (61, -4010). Desert Fox 683: the normal fix, with the sand made from its own map. Pogo Painter 673: the blue sky is piece 6 and the ruins' outline piece 7, both mirrored at the sky's ends on a circle round (396, -31820). Pogo Padlock 661: wall panel piece 83 copied 1532 apart (on the right from 10719, 235 lower), the pavement (tile 7) out to its foot. Beach Ball 653: sea piece 0 carried on, its sky faces 83-102 mirrored at their ends. N.Ballism 649: its front radars are faces 466-523 and 1048-1097 of its piece 0, the normal map's in other colours: `BallismRadarLook`. Sky Balls 645: city piece 1 mirrored across its slanting edges. Tilt Panic 476: sea out past the storm dome, its horizon band piece 1 mirrored. Polar Panic 456: sunset piece 43 mirrored, the sea as the normal map's. Dot Dash 631: the Bearminator's sky (12) and mesas (18) turned round (40, -859). Ring Ding 625: the normal map's wall and gear fixes, the wall's rows one shade (100000). The eleven's 4:3, the other 17 crystal maps' wide shots and 21 normal maps' are byte-identical; 3500-frame runs and 120 fps look right. Still dark at 32:9: Jungle Bash's bottom corners past the mound, and the level's own dark in Pogo Painter (under the garden), N.Ballism and Ring Ding.
- 2026-11-03: 7.11.25, Later in each level, taken off the roadmap by the user.
- 2026-11-03: 7.11.23 Oxide Ride: the user approved Oxide in widescreen
- 2026-11-03: Oxide (mode 31, file 613) in widescreen, both parts. The asteroid part's purple haze isn't a map piece: it's scene mark 0x201F of the level's own file, drawn every frame by `DrawSceneMarks` (from `NoxInitCamera`, the part's render hook) at view depth 21639, its 40 faces a fan whose outline ends in straight edges at view x about -18300..-20800 and 18900..20800. `wide_scenery_mark` (called after each mark) adds bands from those edges outward in the edge's colours (390021 to black), fading out, mapped to the screen by projecting two of the haze's own points; a first try through `view_transform` gave its middle as 0,0,0 and drew nothing. The ship part's camera is fixed at (24,-1694,-4908) from piece 10, the arena (floor, towers and beams in one mesh, 878 faces; the floor is faces 42-72, 79-83, 218-381): its towers are copied 13000 each way with the new `WideExtra.faces`. In a `--mode 31` run the fight never leaves the asteroids; part two needs the menus (`scratch/route_play.mjs`, the boss play test's route). The whole fight in 4:3 (23 shots) and seventeen modes' 21:9 shots, warp room 3 included, are byte-identical.
- 2026-11-03: The user saw Tilt Panic's platform not tip toward the walrus. Checked: the tilt code, the walrus's and the tilted map's drawing are the decomp's; he counts as heavy as all four heroes, so with them on the far side the platform swings to him over a second or two. In a game reached through the menus it does, at 30 and 120 fps, and the user found it fine when they played it. In a `--mode` run it never tilts (Gotchas).
- 2026-11-03: 7.11.22 Tilt Panic: the user approved Tilt Panic in widescreen
- 2026-11-03: Tilt Panic (mode 18, file 486) in widescreen. Four pieces only: 0 the arena, 1 a gradient sky wall (untextured, haze ADA5B5 at the foot to blue and pale at the top, from x -10708 to 10516, z 8057 to 9422, its two ends alike), 2 the sea (at 62,-198,2735; tiles 22 and 23, Polar Panic's stepped shape again, out to z -4996 in front), 3 the arena's front; the icebergs are objects, not pieces. The camera only moves, from (-1042..1637, -1051..-690, -4907..-2837), and goes right when the round ends. The sky goes on as its mirror image across each end (`TILT_SKY_LEFT_END`/`RIGHT_END`), and the sea out to x -32000..32000 (`TiltSea`, rim the haze), past where those reach; `draw_sea_backdrop` draws both seas, and Polar Panic's shots are byte-identical. 4:3 and sixteen other levels' 21:9 shots are byte-identical. No white block in this map.
- 2026-11-03: 7.11.21 Polar Panic: the user approved Polar Panic in widescreen
- 2026-11-03: Polar Panic's stars, after the user saw the sky plain black at the sides: pieces 43-50, one star each (tiles 25 and 26), at z 6760-6856 from x -6466 to 6517. Copies of all eight moved 14000 each way (`POLAR_STARS_ACROSS`), in the backdrop; mirror images across a line past the outermost paired off visibly, with so few. They're left of the sky wall's screen extent, so it doesn't paint over them. At 1920x1000 one copy shows at the left edge.
- 2026-11-03: Polar Panic (mode 19, file 478; Tilt, Melt and Manic Panic are 486, 474 and 466) in widescreen. Piece 1 is the sea (at 103,-198,2750; tiles 22 and 23, flat at y 547-560), only as wide as a 4:3 view, with steps at its sides; 52 the misty band along the horizon (untextured, black at y -165 to 4A3931 at the sea), on a bent line from x -10139 to 10223, z 8290 to 9473; 53 the sky, a black wall at z 10571; 42 the aurora and 43-50 its stars. The camera starts at (-371, -1006, -3036), as Bearminator's turned, and goes lower and further back later on. The sea is carried on out to x -26000..26000 and z 9700 (`PolarSea`, rim colour the band's foot, plain far out), and the band on as its mirror image across each end (`POLAR_HORIZON_LEFT_END`/`RIGHT_END`). At 1920x1000 the shot is byte-identical to before; the change is at 21:9 and wider. 4:3 and sixteen other levels' 21:9 shots are byte-identical.
- 2026-11-03: 7.11.20 Warp Room 3: the user left Warp Room 3 as it was: walked round the hub at 32:9 (`--mode 35`, left held through `--press`), the room's own sky, tree and platforms reach the edges at every angle
- 2026-11-03: 7.11.19 Toxic Dash: the user approved Toxic Dash in widescreen
- 2026-11-03: Toxic Dash (mode 24, file 639) in widescreen. Piece 16 is the room: a wall on a circle of radius about 4880 round the back half only (angles -90 to 90 from the middle; panels about 17.7 degrees wide, rows at y -3560, -2151, -585 and 441, tiles 11-13 above and 8-10 at the foot in that turn left to right, colours 000010, 080831, 08525A and 21FFEF), and the waste floor (tile 16, a flipbook of a frame each frame) round the track. The camera stays at (181, -2770, -5864), outside the room, until a close one at the end. The walls go on from each end to z -6000 (`ToxicWalls`; `PogoWall` is `SideWall` now, with its rows, tiles and colours, and Pogo Painter's shots are byte-identical), and the waste out to their foot (`ToxicWaste`, rim colour 6BFF00, a tile 6 cells across). The black left inside the right porthole is the level's own. 4:3 and twelve other levels' wide shots are byte-identical.
- 2026-11-03: 7.11.18 Bearminator: the user approved the Bearminator in widescreen
- 2026-11-03: The Bearminator (file 472) in widescreen. Piece 0 is the sea, 1 a wall of sky (4 columns, rows of vertex colours, untextured) at the back, 2 the arena, 3 the mountains round the back (a mesh and flat silhouette cards), 4 the ice floes at the sides, out to x -7893 and 8136; the pieces' otz biases are 1280, 1792, 768, 1024 and 768 (`WIDE_LIST` prints them now). The camera only moves, never turns (eye x -1296..1301, z -5565..-2379), and at 32:9 its edges are 63 degrees out, past the floes. The sky's ends are mirror images across x 40, at 41.86 degrees from (40, 0), so the sea, mountains and sky turned 950 (83.5 degrees) each way about there fill the sides. Face 115/138's neighbour in the right floe's ice cliff, (5133,393)-(5579,1202), was never in the model: `ring_quad` can now sort a quad as the game's own (`RING_IN_ORDER`). The sniper's binoculars (map entity 0x2018, otz -0x600) cover only 4:3: their engine's render now draws black bars beside them. 4:3 through a whole won fight (24 shots) and eleven other levels' wide shots are byte-identical. The last pixel row of the picture is black at 1920x1000 and 3840x2001 (every level, widescreen on or off), but not at an exact 4:3 window: before this too, and not looked into (Open leads).
- 2026-11-03: 7.11.17 Desert Fox: the user approved it from the shots.
- 2026-11-03: Desert Fox (file 693) in widescreen. Piece 0 (at 2110,15,2151) is the whole island: arena walls, dunes and buildings, of many tiles; 2 the arena floor; 5 a far dune; 6 a panorama of far dunes and sky (tiles 62-65 twice, a loop) on a circle round (1873, 358) of radius about 10350, from -52.95 to 52.95 degrees, its ends mirror images across x 1873. The camera's edges at 32:9 meet that circle at about 65 degrees, and all the ground they see is inside it. widesand.c can now read every face of a piece as the floor (`WIDE_SAND_ANY_TILE` as image_b) and take a tile size of its own (`WideSand.tile_cells`; Desert Fox's sand tile is about 550 units, 2 cells); the rim is 400 units behind the panorama's foot all round. The panorama goes on as two copies turned by 1203 (105.7 degrees), a little less than the span, so the level's own covers the join: drawn as ring quads they met it with a dotted crack and a slightly lighter shade. Ten other levels' wide shots and Desert Fox's 4:3 are byte-identical. Its crystal version is file 683, another map, not changed.
- 2026-11-03: 7.11.16 Papu Pummel: the user played it and approved it.
- 2026-11-03: Papu Pummel (file 697) in widescreen. Piece 51 is the sunset, a wall on a circle round (365, -10738) of radius 20838 (fitted to its points, within 97), from -23.5 to 22.6 degrees, its two ends shaded differently; 53 the far hills, an uneven band that spans the same angles from there; 49 the island's cliffs, 50 the lava, 54 the pit's walls, 52 the ruins. A mirror image across a vertical plane through C at angle phi from +z is `turn` -2 phi with `mirror`, as T = Mx R(a) reflects across the line at -a/2; so the copies are mirror images across each end (30 units inside), then the level's own turned by twice the span, for both sky and hills. Left: the island's own pixel slivers at its outer edges (one's inside 4:3 too), and at 32:9 its cliffs stop at the bottom corners, which would need a second pit. `scratch/mk_worldprobe2.mjs`'s anchor follows `draw_mirrored` now.
- 2026-11-03: 7.11.15 Space Bash: the user looked at it and chose to leave it as it was.
- 2026-11-03: 7.11.14 Sky Balls: the user played it and approved it.
- 2026-11-03: Sky Balls (file 647) in widescreen. Pieces 12, 13 and 14 (all at -364,4249,-738, unturned) are the near fields, the wall of sky (z ~21700, untextured, three bands of colour, its ends at x -15580 and 15023) and the far fields, whose far edge is the sky's foot, the same points. Copies moved by the sky's width met it with a faint seam, its two ends being shaded a little differently, so the first copies are mirror images across each end, 30 units inside (edge to edge, single pixels showed through), and the next the level's own moved by twice the width (`WideExtra.mirror`). Fields and sky have to move as one, or a dotted crack shows along the horizon. The fields' textures don't scroll (the level's scrolls are on images 1, 64, 71 and 83, and a track on 63, the jets), so mirror images look right; what moves over them is the clouds. Copies of the near fields were culled by `bbox_visible`, whose sphere round the middle is off the screen for them: every copy now has the game's own 0x1000000, which skips it, and eight other levels' wide shots are byte-identical. Its crystal version is file 645, a night map, not changed (14% black at the top at the user's shape).
- 2026-11-03: 7.11.13 Pogo Padlock: the user looked at it and chose to leave it as it was.
- 2026-11-03: 7.7, optional modern lighting, taken off the roadmap at the user's word: the game keeps its own lighting, the colours baked into its models.
- 2026-11-03: The Video pages after the tenth row, at the user's word: the main menu Options page's first row sat on its panel's top edge (it always started above it, at -0x44 against -0x40), and the pause menu's help line hung below its box (at 0xE8 against a box to 0xF0, which the cheat menu's pages share, and had too). The Options panel is now worked out from the row count (`VIDEO_ROW_TOP`, `VIDEO_HELP_Y`), and `PauseVideoMenu` is 0x114 tall. The launcher's fitted already.
- 2026-11-03: 7.6 Sharper icons and UI text: the user played it and approved it.
- 2026-11-03: 7.6, ICONS: CRISP, a tenth Video option (`settings.icons`, `icons=` in settings.ini, `--sharp-icons`), off by default; with it off 4:3 and wide shots are byte-identical and `same` passes 14 of 14. `upscale_texels_sharp`: Catmull-Rom 4x, unsharp mask (3.0, sigma 1.2), alpha where coverage is half, square corners kept (`square_corner`, or a portrait's corner rounds off). Which textures: found with a temporary probe in `draw_face_quad` printing each sprite's bank and index; heroes' are the interface bank's `IconImage` frames (index 0x16 is among the digits' range, so the icon check comes before `is_text`), the hero select's are `DB_MainmenuModels` 0x67-0x74 and 0x7B-0x7C, and a boss's is set by `SetPanelHeroIconB` (Papu bank 44 frame 10, Bearminator 130/0x31, Oxide 92/0x88; Big Bad Fox has none). The Video pages' rows went from 0x14 to 0x12 apart to fit ten. Pause in a test run: `--press` start works in a tournament game reached through the menus, not with `--mode`, whose demo flag keeps pause off.
- 2026-11-03: 7.6, sharper icons and text, looked at with mock-ups only (nothing in the game changed). The HUD icons are 64x32 textures in the interface bank, at 5 screen pixels a texel on the user's 3840x2001 window: sampled sharp already, so their fuzziness is the art's own soft edges. A Catmull-Rom 4x with an unsharp mask (3.0, sigma 1.2 at 4x) and the silhouette thresholded looked crisp (scratch/icon_try*.mjs); the user hasn't decided. The game's letters are grey shapes with a black outline (sysfont, `SysFontLookUp`, tinted top to bottom by `DrawSysText`'s colours). With the user's OK a fan font (Crash-a-Like, "100% Free", kept in the ignored `build/port/downloads/`, never in the repo) was mocked up over the main menu at 4K (scratch/font_mock.ps1): the user preferred the game's own letters, so text stays.
- 2026-11-03: 7.11.12 El Pogo Loco: the user approved it from the shots.
- 2026-11-03: El Pogo Loco (file 667) in widescreen. Same camera as Pogo Painter (eye 0,-2360,-4511). Pieces: 0-63 the board, 64 the sand, petrol station and dunes, 70 the far wall of mesas (z ~13850, plain colours, both ends a low ridge from y 904 to 2170, below ground), 72 the sky (z 16603, a row of quads of one gradient), 78 the road's dashes; the asphalt forecourt and road are plain faces of 64. Copies of 70 and 72 shifted by their own widths, less 100 to overlap, carry them on (Extras). The sand (image 7) carries on with widesand.c to a rim dipping to the wall's foot. Two gotchas: image 7 is a grey texture made warm by the level's vertex colours, so the new sand needed a warm shade of its own (`WideSand.colour` 0x687B99, worked out from on-screen samples); and a textured floor seen almost side on, at the sides of 21:9 and wider, makes stripes with sharp sampling, bumps or not, so far out the tile's detail fades to its plainest texel (`WideSand.plain_far`). Other levels' wide views and El Pogo Loco's 4:3 are byte-identical to before. Pause wasn't checked: `--press` start with `--bot` didn't open it. Its crystal version is file 665, not changed.
- 2026-11-03: 7.11.26 Menus and the player panel: the user played it and approved it.
- 2026-11-03: The menus in widescreen. Everything was shot by `--press` routes (the menu test's, plus Adventure, Options with its Video page, Tournament, load game, the launcher, a level's pause, the warp room's door panel, a battle's end), each against 4:3. Only two things were wrong: the Universal logo (a 4:3 picture, black at the sides) and panels that ran off a side of the 4:3 screen showing their ends. A screen-space panel's place on the screen is `RenderState.screen->x` plus its own x, in 640 units (`draw_gouraud_quad`), so `wide_panel_sides` in `draw_panel_rect` and `draw_panel_frame` carries a side at or past 0 or 640 out by the wide extra. The hero select's panels are from the left (its engine at -0x140), the rest from the middle, so it has to go by that sum, not the records. The HUD's health bars are `draw_nas_model_flags` quads, not panels, so it doesn't touch them. Within one key the last added draws first, so `draw_back`'s edge columns go at `KEY_BACK - 1` to be over the viewport's black, as nothing else uses keys from 0x1000 to 0x1FFF. 41 4:3 shots of the menus and a level are byte-identical, and 54 in-play wide ones were before the next change. Then the user asked for the heroes' pictures to stay in the middle in a level, so `HudSpread` is gone: the player panel and the big scores keep their 4:3 places. Test runs: a window maximised once in the middle of a run this time, during Crashball; `--fps 60` or above draws nothing in `--turbo`.
- 2026-11-03: 7.11.11 Pogo Painter: the user played it and approved it.
- 2026-11-03: Pogo Painter (file 675) in widescreen. Pieces 0-63 are the board's tiles, 64 the garden (back wall, ruins, tree, grass), 65 the front ledge, 66-72 props. Its sides are stone blocks with nothing past them, and the camera's top edge looks almost level (eye at 0,-2360,-4511, the back wall's top -1869 only just off the screen). `draw_pogo_painter_backdrop` carries the back wall down both sides at x -4271 and 3783 towards the camera, a row taller (-2477), and the grass out to their foot with widesand.c (piece 64, image 11, a rim round the garden). The view is of piece 64, at the origin unturned (`ring_view_of`), as the first piece is a floor tile at -1792,0,1792. The other levels' wide views are pixel-identical to before. Test runs at `--fps 60` or above draw nothing in `--turbo` (stepping draws only on the present path), so shots there need real time. Its crystal version is file 673, another map, not changed.
- 2026-11-03: 7.11.10 Drain Bash: the user looked at it in widescreen and chose to leave it as it was.
- 2026-11-03: 7.11.9 Metal Fox: the user looked at it in widescreen and chose to leave it as it was.
- 2026-11-03: 7.11.8 Swamp Fox: the user looked at it in widescreen and chose to leave it as it was.
- 2026-11-03: 7.11.7 Ring Ding: the user played it and approved it.
- 2026-11-03: Ring Ding again, after the user played it: the brick wall needed carrying on round, and the big right cog was artifacting. The artifact: piece 13 is a shadow that takes light away (subtractive, its faces' colour index has 0x8000 and blend 2, read with a dump of `*idx >> 13`), over the right cog, which turns (`SpinTurnArena`). Painted by order, each cog face came before or after the shadow by its own depth as it turned, so faces popped between dark and lit. It's an original artefact, but mostly out where a 4:3 view stops. Drawing the level's pieces in real depth looked the same and wouldn't help, as blended faces don't test depth here; a fixed order for the cog would put it over the pillar behind it. So piece 13 is hidden in a wide view, which makes the cog's right side a little brighter. The left shadow (12) made no visible difference, so it stays. The wall (piece 10, image 7) is a tile to each row and column, fading to the colours at its ends; `draw_ring_ding_wall` carries it on round both sides to past where a 32:9 view reaches. Its own end and the new start are made apart, so their corners missed by a unit and left a crack of black: the new one starts 10 units behind the old end. The user's window is maximised at 3840x2001, about 1.92 wide, so checks now include 1920x1000.
- 2026-11-03: Ring Ding (mode 27, file 627), at the user's word after seeing it. The level's faces in world space (`scratch/mk_worldprobe.mjs`, a probe that dumps each piece moved and turned) showed its back wall (piece 10) is lit in the middle and fades to near black at its ends and its bottom, so the dark at the sides is its own lighting, and carrying the wall on would only add more of it. The black at the bottom right was under the platform, where the left has a grey gear (piece 4) and the right only a faint shadow plane (14). So a copy of that gear goes on the right, in the level's own order (`placed`), so the platform's in front: 16:9 margins 23% black to 12%, 21:9 51% to 44%. It turns and fades as the original does. One test shot came out at the maximised size, 3840x2001, with four copies running; seven more alone and together didn't, so it's an open lead.
- 2026-11-03: 7.11.6 Big Bad Fox: the user looked at it in widescreen and chose to leave it as it was.
- 2026-11-03: 7.11.5 Jungle Fox: the user approved it from the shots.
- 2026-11-03: Jungle Fox (mode 7, file 691; its crystal map is 681), at the user's word. Like Crashball, most of its dark is its own: the floor's outline is coloured dark (4a4a4a, 393939, black), so it fades into the night even where its edge shows at 21:9, and 11% of a 4:3 picture is already black. What stops is the canopy of leaf clusters at the top, so six copies of its own clusters carry it on into the top corners (16:9 margins 33.6% black to 30.0%, 21:9 60.7% to 53.8%). Copies turned to vary them showed thin edge-on leaves, so they're as the level has them. Two more lower down were tried and dropped: over the floor, which is painted after what's behind everything, they were cut straight across, and the small clusters are blue there. The camera fit (`scratch/bbcam.mjs`) now leaves out probe points behind the camera, and takes the shot's size and margin (`BB_SHOT_W`, `BB_SHOT_H`, `BB_EXTRA`); `lv_outline.mjs`, `lv_facts.mjs`, `margin_black.mjs` and `jf_find.mjs` help place things.
- 2026-11-03: The user found the window size broken. A maximised window's size (3840x2001 on their 4K screen at 225%) was kept as the window's size, so the next start opened at it, not maximised, a little under the taskbar. And SDL ignores a size asked for while a window's maximised, so WINDOW SIZE changed in the menu and nothing happened; from a size not on its list it went back to the start of the list. Shown with a temporary probe that maximised the window and stepped the option as the menu does (`scratch/mk_winprobe2.mjs`). Now `maximized` is a setting of its own, a size change takes the window out of maximised first, a size the screen can't hold opens maximised, and the option goes through the sizes that fit, then MAXIMISED. `scratch/winrect.ps1` reads a window's real rectangle as a DPI-aware caller. Only on this one screen.
- 2026-11-02: The user saw slow motion after going back to their normal settings. The settings had FPS 120, on a 60 Hz screen: `scratch/speed.mjs` timed 28.5 vblanks a second, against 60. SDL's non-waiting swapchain call only checks the card's fence, not DXGI's present queue, so with vsync on `Present` itself waits for the screen, and the game's clock with it. A limit by the clock didn't work, as the waiting itself seemed to make room, so `picture_fits` leaves out a share of the pictures by the rates' ratio. It's about 60 a second again, and 120 now draws the same as 60 on that screen. Only tested on this 60 Hz screen, with vsync on.
- 2026-11-02: 7.11.4 Crashball: the user played it and approved it.
- 2026-11-02: Crashball, at the user's word: its black is the level's own, unlike the last two. It's a lit arena in the dark: 18% of a 4:3 picture is already black, the floor's outline, the pipes' front ends and the cannons' tails are all coloured black, and none of its textures is a backdrop. So nothing's added, and that's my recommendation, for the user to judge by playing. What was wrong: one vertex of the right pipe's end has a colour where the left's is black, in a part a 4:3 view never shows, which made a hard wedge. The two are exact mirror images there, so the right one leaves those faces out and draws the left one's in a mirror (`draw_crashball_pipe_end`), and leaving faces out is now a table by level and piece (`LeftOutFaces`). `scratch/lvmap.mjs`, `lvwire.mjs`, `lvpick.mjs` and `cb_facts.mjs` read any level from a dump by `mk_nbprobe.mjs`, and comparing left and right vertices finds slips like this quickly. Its crystal map is file 657, so it isn't changed. After playing it the user asked for the blue sparks above the top goal to go. They're the game's own: `Init_ArkCameraFlash`, spawned one frame in 15 out in the dark behind the arch, a hexagon shrinking over 5 frames, in the decomp too. A wide view keeps them hidden by their draw flag, and doesn't drop them, so the random numbers and the objects stay as they were: a bot run's frames match but for the flashes. 4:3 keeps them.
- 2026-11-02: 7.11.3 N.Ballism: the user played it and approved it, with the radars' backs and thicker rings.
- 2026-11-02: N.Ballism's front radars, at the user's word: "some sort of back on it like old CRT tvs did", and the rings back on. The back is rows out from the rim's own octagon (collar, taper, rear block), plain faces in real depth, with a shade per side, in the radar's own purple: a first try in the rim's colours just looked like a wider rim. The rings: the game swaps a base and its rings for lit models while a radar fires (`UpdateArkFloor`, `FloorFramesC`, mesh 0x1F to 0x26), which explains the white streak. The lit rings are one-sided, and this camera is just behind them, so they vanished; and the lit shaft down the spike has only 4 of its hexagon's 6 sides, brightest on the side facing the camera on the back radars. The front ones are those turned 90 degrees, so it faced away, and only the far wall's top showed: the streak. It's turned 120 degrees round the spike now (`draw_ballism_shaft`), with its order brought forward, as it's blended and so goes by the painted order. The user then asked for the rings a bit thicker. Seen edge on, stacking flat copies would need dozens to meet, so `draw_ballism_ring` makes each one solid from the mesh's own strip (a run of 12 faces is a ring): the outside wall with a shine down its middle, the inside wall and two ends, in gold measured off the back radars' rings. It tried 3, 4, 5, 7 and 10 32nds of a ring's size: 7 and up read as planks, and flat colour looked dull. `scratch/mk_meshdump.mjs` dumps a level's numbered meshes, and `WIDE_CRT` and `WIDE_RINGT` were throwaway env vars for trying numbers without a rebuild.
- 2026-11-02: N.Ballism's front radars, at the user's word after playing: their bases were painted over them. The radars are faces of the main mesh (otz 512), and the bases are pieces of their own (otz 0), so a base always paints later. Keys go 512 + average w / 8 + otz / 2, read from `scratch/mk_who4.mjs`, which lists every triangle covering a point of the screen in one frame. Now those faces (`BallismLeftOut`) and the bases draw together in real depth with the heroes (`draw_ballism_radars`). Their gold rings stay hidden, as the user approved that look: drawn, the right ones are thin needles and the left ones hardly show, with a white streak beside the spike that wasn't looked into. A 4:3 view keeps the level's own order, where this hardly shows. `draw_face_list` takes runs of faces now. `WIDE_EXTRA="-1,0,0,0,0,0,0"` turns off the hidden pieces too, so it really is the level as built.
- 2026-11-02: N.Ballism in widescreen, at the user's word. With the level's faces laid over a shot (`scratch/lvwire.mjs`, `lvmap.mjs`), the jumble at the sides was my copies of the whole map, pillars and screens and all, turned round the middle. They're gone, for its own round wall carried on round: mirror-image copies of its last stretch, by faces (`BallismStretch`, `draw_ballism_round`, `draw_mirrored`). The floor has see-through texels, so a black plate goes under it, stopping short of the floor's edge. The front radars are edge on to this camera, so the "pole" and the "wedge" at the front corners are their real shape, not a glitch. Their gold rings stay hidden, as they're only needles. The flat floor under their bases (pieces 9, 10) now goes with the backdrop rather than hide, which filled the white gaps. A black side of the left pillar (face 517) goes too, as it only ever hid in the dark. The white tube on the right was an old picture: a level that fills a 4:3 screen doesn't clear, so the margins kept whatever was there and blends built up. `draw_viewport` now starts them black, at the very back of the frame, as a level's panel is a view of its own drawn later. That also removed Sky Balls' white patch and Polar Panic's magenta, and shows a little more black in Bearminator and Ring Ding, where old pictures hid holes. `scratch/margin_cmp.mjs` compares every mode's widescreen picture between two builds, 4 at a time: only those and N.Ballism changed. `same` 14 of 14, 6000 vblanks of bots with no crash, 120 fps looks right, and the fade reaches the margins. The pause menu isn't tested, as it doesn't take scripted presses. Waiting on the user to play it.
- 2026-11-02: 7.11.2 Beach Ball: the user played it and approved, and approved Dante's Dash again after the backdrop's finer depth.
- 2026-11-02: Beach Ball looked worse to the user: kelp under sand, and a bigger tear. A proper look, with the camera fitted from a probe so the level's faces and the sand's grid can be laid over a shot (`scratch/bbcam.mjs`, `bbwire.mjs`, `bbgrid.mjs`). The swept sand sheets folded over themselves. The tear was the level's own wall painting over the sand, where the real floor stops short of it at the back corners. The rim's bumps made a jagged skyline, and the saw teeth along it were depth: with a near of 1, floats near 1.0 are about 244 units apart at a depth of 8000. So now: `main/widesand.c` makes one height field from the floor's faces, in 17 ms as the level loads; the foot of each end of the wall (faces 252, 253, 262, 263) goes with the backdrop by `draw_face_list`; the sand has a height for each wall column, chosen so its far edge carries on the floor's line on the screen; the backdrop's depth has `DEPTH_NEAR_BEHIND`. Kelp and coral stand in the level's order, and one kelp type I'd picked was a fish, now gone. The finer depth also cleans jagged edges in N.Ballism's and Dante's Dash's extras. The sand's vertices under the floor must take the floor's height, not the nearest edge's: the floor has steps, and a vertex at the wrong height pokes out above its far edge. In the 4:3 part only two small notches by the pillars change, where the level's wall used to paint over its own floor. `same` runs 4:3, so it can't see any of this; a shot of the same frame from the old build compares it. Its Nov 1 baseline failed even on the previous commit's build, for a reason not looked into, as the sound still matched: it was saved again from that build, and this one passes 14 of 14. 6000 vblanks of bots with no crash. Waiting on the user to play it.
- 2026-11-02: README rewritten short, with the PC port first (the release zip, then building it) and the disc rebuild second.
- 2026-11-02: Beach Ball's seams: the level's own floor mirrors its tile from cell to cell, so edges meet, and the extra sand and wall now do too (`RING_FLIP_U`, `BeachTiles`). Its sand is a full ring past the wall's foot, which filled the void wedges. The user still saw a big line near the kelp, where the real floor stops: blending from its vertices can't meet an edge exactly. So `BeachSheetCorners` starts from the floor's outline itself (`scratch/floor_edge.mjs` traces it), in its heights, colours and tile mapping, and sweeps that round. What's made to meet a level's mesh has to use its piece's position too: Beach Ball's floor is at (-3, 0, 1), so at (0, 0, 0) the right side had a crack the left didn't. Extra scenery drawn by hand has to take `modelDp` as well, or it stays lit through the screen's fade. The added kelp plays the level's own mesh animations, as still meshes looked dead beside the real ones (`Init_seaweed` has the frames). The black still under the front horns is the level's own faces, coloured black (15 faces in pieces 2 and 3), not a gap. Latest version not played yet.
- 2026-11-02: Beach Ball in widescreen, at the user's word: its backdrop carries on along its own curve, read from the level's faces (a flattened arc, not a circle), with sand blended from the real floor's vertices and the level's kelp and coral, and the wall taller after the user saw black at the top. Invented sand colours didn't match, so copy a level's own numbers, not by eye. The latest version hasn't been played yet. To read a level's faces, a temporary dump like `scratch/mk_dumpfaces.mjs` and `read_faces.mjs` is quick. One frame comparison once showed the whole picture changing, and it didn't come back in two repeats.
- 2026-11-02: Dante's Dash in widescreen: the copied trees looked doubled, so they're gone, for a curved wall of the level's forest panorama and its swamp grass carried on round the lava, facing the centre so the row curves, as the user asked. The lava's as it was. The user played it and approved. A 12-cell `--tile` grid broke `same`, as the grid sets the window's size and so the pictures: it stays 14 cells, with 12 running at a time.
- 2026-11-02: 7.12 Polar Push's snowball: The snowball's mesh was set through a raw offset that's `entity` on a PS1 and the `prev` pointer here, so the hero vanished and only the reflection showed; only the normal level, as a crystal one set the field too. Checked by making the mask's beam always pick the snowball, on eight heroes. The user played it and approved.
- 2026-11-01: Stopped here. Next, one at a time with the user, who plays each: the rest of 7.11, starting with 7.11.4 (Crashball). The play tests run 12 copies at most in their grid; a script with full-size windows, 4.
- 2026-11-01: Beach Ball in widescreen: the user found the copied walls stretched its sea floor and wanted the background to curve. It now has a ring (`Rings` in `widescenery.c`): sand tiles at the level's size, and a curved wall of its city panorama. Its crystal map hasn't one.
- 2026-11-01: N.Ballism's front corners in widescreen: the level's main mesh has upright dishes only at the back; the front has a base with a stray wedge and needles. Those are now hidden in a wide view. Not a culling problem: drawing every mesh both sides changed nothing. Not compared with the original in RetroArch, by the user's choice, so whether the original shows more there is unproven.
- 2026-11-01: Widescreen's black parts: mirroring, drawing edges outward and stretching were tried and dropped (copies, with hard cuts at the 4:3 line). The user suggested adding scenery, which works: `main/widescenery.c`, a table of the level's own pieces moved and turned, drawn behind everything, only in a wide view. Melt Panic had some and lost it again: it flickered, and the user prefers that level as it was. A test script that started 37 copies of the game at once crashed the user's PC: the play tests run 12 at most, in their grid (`AT_ONCE` in `playtest.mjs`), and other scripts with full-size windows 4.
- 2026-11-01: The user saw lines through Polar Push's aurora, with SMOOTH on. The art's tiles don't quite meet, here a scrolling 64 x 64 wrapped round the dome, and only square texels hid it. Joins are now found as faces are drawn, and both sides eased together. Mirroring the level into widescreen's black parts was tried and dropped: the user found it looked bad, and it copied the race track.
- 2026-11-01: A Halloween texture pack with hero costumes was tried and abandoned at the user's word. Only local leftovers remain: `scratch/halloween.mjs`, `costumes.mjs` and `build/port/windows-clang/packs/halloween`. Pack colours per model face and a `--heroes` test option were written for it and backed out unused (`scratch/model_faces.patch`). Learned: heroes get most of their colour from textures (about 20% of their faces are untextured), so a pack reaches most of a hero.
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
