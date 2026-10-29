# Playing a build

| Command | What it does |
|---|---|
| `tools/dk make disc` | Packs `build/main.exe` and every overlay into `build/CrashBash.bin`/`.cue`, a disc image any PS1 emulator boots. |
| `tools/dk make disc-check` | `make disc`, then checks the image is the retail one byte for byte (`configs/USA/disc.sha`). |

The matching build is the retail bytes, so `disc-check` passing already proves it plays like
retail.

## The disc packer

`tools/scripts/pack_disc.py` starts from your retail image as `decomp.py extract` unpacked it
(`disc/files/`, `disc/layout.xml`), replaces `SCUS_945.70` with `build/main.exe`, builds `CRASHBSH.DAT` from
its files in `assets/CRASHBSH/`, writes each `build/<ovl>.bin` over it at its retail offset, and
rebuilds the image with `mkpsxiso` so every file keeps its retail sector. A file may shrink (it is
zero-padded) but not grow: an overlay or a DAT entry is a fixed slot, and the packer refuses a
bigger one.

## The game's files (`assets/`)

`tools/dk python3 tools/scripts/dat.py extract` splits your retail `CRASHBSH.DAT` into its 992
files, `assets/CRASHBSH/000.bin` .. `3DF.bin` (ids as ARCHITECTURE.md names them), with
`index.json` (each one's name, sector, size and format; the code overlays are tagged) and `filler.bin` (the bytes
between files). `make disc` runs it the first time by itself. Edit a file and `make disc` puts it
on the disc; delete `assets/CRASHBSH/` to start again from the retail DAT (extracting takes about
6 minutes). The 400 texture sets come out as `<id>.tex/` folders of PNGs: edit an image's pixels in
`img_NNN.png` (keep it indexed, same size) and colours in `cluts.png`; the formats and the
rules are in [ASSETS.md](ASSETS.md). The 400 model files come out as `<id>.mdl/`: `model.obj`
(textured, with `model.mtl`) to move vertices in Blender or any OBJ editor, keeping each mesh's
vertex count and order, and `model.glb` to view a model with its animations (Blender: File >
Import > glTF). The 5 title screens are `<id>.tga/screen.png`, and the 12 modes' text is
`<id>.txt/text.txt` (a string may change but not outgrow its slot). The 160 sound packs
are `<id>.sfx/`: each sample a WAV (edit it, 16-bit mono), the bank as Sony's VAB, and the music as
SEQ and MIDI (edit the `.mid`). Only the 15 code overlays stay `<id>.bin`. It is your game data: `assets/` is git-ignored, never commit it. The 15 code overlays' entries are there too, but `make disc`
always writes the built `build/<ovl>.bin` over them.

## Playing it

On Windows, RetroArch with the PCSX-ReARMed core (no BIOS file needed):

```
winget install --id Libretro.RetroArch -e
# core: https://buildbot.libretro.com/nightly/windows/x86_64/latest/pcsx_rearmed_libretro.dll.zip
#       unzipped into C:\RetroArch-Win64\cores\
"C:/RetroArch-Win64/retroarch.exe" -L "C:/RetroArch-Win64/cores/pcsx_rearmed_libretro.dll" build/CrashBash.cue
```

Keys: arrows (d-pad), Z (cross), X (circle), A (square), S (triangle), Enter (start), right
Shift (select), Q/W (L1/R1), F1 (menu), Esc twice (quit). A pad works without setup. Any other
emulator works too; DuckStation needs a real BIOS dump.

## The fully shifted build (`make shift-all`)

`tools/dk make shift-all PAD=0x40` builds a disc in which every binary has moved:
`build/shiftall/CrashBash.bin`/`.cue` (tools/scripts/shift_all.py). From the matching build's
objects (nothing is recompiled) it pads the exe, the engine and every slot overlay by PAD before
their first C rodata, text and data, so the engine loads 3 × PAD higher and the slot 6 × PAD
higher, and links them against each other's new addresses, repeating until nothing changes
(two links). Where the matching build fixes an address, the tool moves it:

- splat's name lists (`linkers/*_undefined_*`, `undefined_syms_manual.txt`, the PsyQ names)
  link names at their retail addresses: each line becomes a fallback a definition overrides; a
  name another binary defines takes its new address there; a name nothing defines (a .bss
  buffer, a label inside a data object) moves by position: its binary's base shift plus a pad
  for each pad point at or below it, from the matching build's link maps.
- `HeapStart` (`0x80078C90`: where the exe loads the engine and its heap starts) and
  `SlotHeapStart` follow the exe's and the slot's ends; the slot's id word its base.
- Sections the scripts pin at retail addresses (the exe's PsyQ .bss and `bios_1` .data, the
  engine's .bss image) move by position.
- The exe header (`src/main/header.c`) links its initial PC to `__SN_ENTRY_POINT`, so it follows
  the move (`shift_all.py` checks it and pads after the header, not before). Sony's
  startup code (`2MBYTE.OBJ`, kept as raw words in `asm/main/psyq/2mbyte.s`, so without
  relocations: it clears the .bss, sets `$gp` and calls `InitHeap` and `machine_init` by
  number) is assembled from a copy that names what it uses, byte for byte the same code.
- The mode text files (`overlays/text/*.bin`) point into the engine's `ModeTextBuffer`: their
  1,645 pointers move with it (`pack_disc.py --dat`).

Then it checks that no name a binary uses still links by a retail address in a range that
moved, packs the disc with the moved exe (`pack_disc.py --exe --elf`: the DAT index is found by
name) and the bigger engine and overlays (`--grow`). `--pad-main`, `--pad-engine` and
`--pad-slot` move one binary at a time (0 leaves it), to bisect a crash; at PAD 0 every binary
is the matching build's (the engine's slot image and .bss image aside, which are zeros). Playtested 2026-10-29:
the front end, the pause screens' text, a load and a save (a new slot, its sum right), the hub
and the minigames. The save holds an address (`GameState.boot`; a slot saved on the shifted
build holds a different one): harmless here, since the game doesn't seem to read it on load. The zeroed engine .bss (2026-10-29,
ROADMAP-B 2.5) boots and plays (playtested by hand 2026-10-29), and its mode text buffer fills with the mode's text at the address
`ModeTextLoadAddr` gives (read through RetroArch's command interface, `scratch/texttest2.ps1`).

## The shifted build (`make shift`)

`tools/dk make shift OVL=tank PAD=0x40` builds a disc in which one slot overlay's every
address has moved: `build/shift/CrashBash.bin`/`.cue` (tools/scripts/shift.py). It reuses the
matching build's objects and:

- links the overlay with PAD bytes before its C rodata, its text and its data (its id word
  stays first), so its code and data move by PAD, 2*PAD and 3*PAD;
- links the engine with the moved overlay's addresses for every slot name its C uses (the AST
  table, the world records, the per-mode table, the hero tables; `slot_syms.py`), and with its
  slot image as the id word and zeros instead of the retail leftovers. Until the engine's data
  was C (2026-10-27) a stopgap rewrote its asm data's words by address; a value in the range
  could be another game's, so other games could break on a disc shifted that way.
- packs a disc with `pack_disc.py --grow`: the overlay, now bigger than its retail size, stays
  in its place in `CRASHBSH.DAT`, which is laid out again around it when it outgrows the
  sectors before the next entry, and the exe's DAT index and file group sizes follow. (It
  can't move to the DAT's end: the 13 slot overlays are one file group, read in one go, each
  found by its sector offset inside it. The first shift disc did that, and the game kept
  loading the retail Tank from its old place, so it crashed.)

It plays like any build (`retroarch ... build/shift/CrashBash.cue`). The shift test is to play the
overlay's game on it through to a win, the same way as on the retail disc.

### The shift test route (Tank, 2026-10-26)

Tank needs to be unlocked: use a completed save. The one used is GameFAQs' North American
DexDrive save "200% of the game completed" (`crash-bash.655.gme`): strip its 3904-byte header
(the rest is a raw 128 KB card holding `BASCUS-94570`) and give RetroArch that as
`CrashBash.srm` in a save folder of its own (`--appendconfig` with `savefile_directory` and
`sort_savefiles_enable = "false"`, so your own saves are untouched). Then: Battle Mode, Tank
Wars, Vs Battle, the game options screen, X to start (Desert Fox). Play it through to a win.

### The shift test route (Hub, 2026-10-26)

`make shift OVL=hub PAD=0x40`, with the same save: load it into the warp rooms, walk through
the gateways, open the door menus (choosing a level, the locked doors' messages), warp
between rooms, and save with the name entry. Loading the save gave a black screen until the
engine's `camera.c` called the hub's camera (`HubCameraFollow`, and `PodiumCamera`,
`HubStartCredits`) by name: their old `func_` names were pinned to retail addresses by
splat's undefined-functions list. Passed 2026-10-26 (the credits and the tournament podium,
mode 0x2B, weren't played).

### The shift test route (N2O, 2026-10-26)

`make shift OVL=n2o PAD=0x40`, with the same save: the Nitros Oxide boss in Adventure, and
dying and picking Retry. Retry crashed until the engine's `InitWeatherNas` named N2O's star
field (`NoxSparkFxDef`) instead of its retail address; shift.py now warns about such
references. Passed 2026-10-26 (the first phase and Retry; the playtest didn't reach the
second phase, `N2OBallPhase`'s other half). With the 200% save, the pause menu's "return to
warp room" plays the ending and the credits: retail does the same (the engine's mode 0x1F exit,
`gameflow.c`, takes the ending when the save's game-state byte 0x187 is set), so it isn't a
shift bug.

### The shift test route (Chase, 2026-10-26)

`make shift OVL=chase PAD=0x40`, with the same save: a Chase match to the end, with the
barrels and their generators, the burning coal and its spreading fire, pickups, jumps, falls
and landings, hits, and the winner's and loser's animations. Passed 2026-10-26.

### The shift test route (Bonk, 2026-10-26)

`make shift OVL=bonk PAD=0x40`, with the same save: a Bonk match to the end, with the mallet's
swings and backswings, hits on heroes, crates and stools, the shockwave, and the winner's and
loser's animations. Passed 2026-10-26.

### The shift test route (Dino, 2026-10-26)

`make shift OVL=dino PAD=0x40`, with the same save: a Dino match to the end, with the gems
picked up, carried, shot and scored, the dazes and the winners' animations. Passed 2026-10-26.

### The shift test route (Spin, 2026-10-26)

`make shift OVL=spin PAD=0x40`, with the same save: a Spin match (the ring arena) to the end,
with the turning arena, the hazards and balloons, scoring, flips and falls. Passed 2026-10-26.

### The shift test route (Race, 2026-10-26)

`make shift OVL=race PAD=0x40`, with the same save: a race on each of the four tracks (Dash,
Toxic, Water, Lava) to the finish, with the pickups, the spinners and pistons, the rolling
barrels, laps and Water's swimming. Passed 2026-10-26.

### The shift test route (Papu, 2026-10-26)

`make shift OVL=papu PAD=0x40`, with the same save: the Papu Papu boss fight in Adventure to a
win: crates and TNT at the boss, his ground slam, the floor waves, the columns and the minions.
It crashed after the first TNT hit and slam until the overlay links took splat's undefined
symbols as `PROVIDE`s (ten of Papu's own halfword variables had stayed at their retail
addresses); passed 2026-10-26 after that.

### The shift test route (Bash, 2026-10-26)

`make shift OVL=bash PAD=0x40`, with the same save: Battle Mode's crate games (Jungle,
Space, Snow and Drains), throwing and kicking crates, the pickups and powerups, Space's
falling tiles, Snow's penguin and sliding, Drains' edge crates. Passed 2026-10-26.

### The shift test route (Polar, 2026-10-26)

`make shift OVL=polar PAD=0x40`, with the same save: Battle Mode, Polar, and its arenas,
watching each hero's sequences, the bombs, the transmuter beam, the walrus and the ice.
There is no taunt button in Polar: only computer players taunt (Crash, Coco, Cortex, Brio
and Dingodile, idle in two of the AI's states, from about 3.5 s into a round;
`PolarHeroAnimState`). Passed 2026-10-26.

### The shift test route (Arkenoid, 2026-10-26)

`make shift OVL=arkenoid PAD=0x40`, with the same save: Battle Mode, Ballistix, and its four
levels (Ballistix, Beach Ball, N. Ballism with N. Gin, Sky Balls), watching each hero's
sequences and the balls on the walls and posts. Passed 2026-10-26.

### The shift test route (Pogo, 2026-10-26)

`make shift OVL=pogo PAD=0x40`, with the same save: Battle Mode, Pogo, and its levels
(Painter, a-Gogo with its moving kerbs, Loco with the Ripper Roo, Bouncer with the
padlocks), watching each hero's idle, tazer, dazed and win sequences. Passed 2026-10-26.
On a disc shifted this way, the crate games' world records point at a Pogo address by
coincidence and are rewritten too, so those games break; only the shifted game counts.

### The shift test route (Titles, the front end, 2026-10-26)

`make shift OVL=titles PAD=0x40`, with the same save. The front end is every menu, so walk
them all: the main menu, the number of players, new/load game, character select (each
character's model and animation shows when picked), the battle level select (arenas and
levels, the locked ones), team battle, the battle options, a battle and back to the level
select, the options screen, and a tournament. Passed 2026-10-26, after one fix (the
character cards' animation pointers had been written as bytes and didn't move).

The game also has a cheat word at `0x8005A618` (`0x2000` unlocks every list entry, `0x10000`
adds the pause menu's hidden fifth item; nothing in retail sets them). RetroArch's network
commands (`network_cmd_enable`, `READ_CORE_MEMORY`/`WRITE_CORE_MEMORY`; PCSX-ReARMed maps RAM
at 0) read and write it, and dump RAM; the BIOS's current thread control block (`0x8000E29C`)
holds the registers saved at the last interrupt or exception (EPC at +0x88, cause +0x98),
which is how a crash can be traced when the emulator itself goes down.
