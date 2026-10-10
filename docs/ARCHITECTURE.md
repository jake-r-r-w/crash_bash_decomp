# Architecture

How `SCUS_945.70` is laid out and how this repository rebuilds it. The
evidence behind each boundary is kept as comments next to it in
`configs/USA/main.yaml`.

## Disc

| File | Size | Contents |
|---|---|---|
| `SYSTEM.CNF` | 69 B | boots `SCUS_945.70` |
| `SCUS_945.70` | 422 KB | resident executable (the current build target) |
| `CRASHBSH/CRASHBSH.DAT` | 70 MB | data archive; expected to hold the code overlays (engine + minigames), not yet indexed |
| `BASHY.` | 30 MB | unidentified (likely streamed audio/video) |
| `SPYRO3/SPYRO3.EXE`, `WAD.WAD`, `SPEECH.STR` | | Spyro: Year of the Dragon demo, out of scope |

## Executable memory map

PS-X EXE header at file offset `0x0`; the file body (offset `0x800`) loads at
`0x80010000`, `0x69000` bytes. Entry point `0x8002E7B0` (`__SN_ENTRY_POINT`).

| VRAM | File | Section | Owner |
|---|---|---|---|
| `0x80010000..0x8002E7A8` | `0x800` | `.text` | game code, `src/main/main.c` |
| `0x8002E7A8..0x8004C56C` | `0x1EFA8` | `.text` | PsyQ 4.7: 254 Sony objects linked in place, plus `src/main/psyq/` |
| `0x8004C56C..0x8004CEFC` | `0x3CD6C` | `.rodata` | game (strings, jump tables) |
| `0x8004CEFC..0x8004DCBC` | `0x3D6FC` | `.rodata` | PsyQ, per object (`.rdata` of the linked objects) |
| `0x8004DCBC..0x80079000` | `0x3E4BC` | `.data` | game and library data, still one blob |
| `0x80079000..` | | `.bss` | not yet measured |

How the boundaries were found:

- **Library start** `0x8002E7A8` (`__main`, 2MBYTE.OBJ): lowest address matched
  by the PsyQ signatures; no function above it calls below it (0 calls), the
  game calls into it 285 times.
- **`.rodata` start** `0x8004C56C`: the end of `.text`. **Game/library seam**
  `0x8004CEFC`: the unreferenced `$Id: sys.c` string of LIBGPU; the highest
  game reference is `0x8004CEEC`, the lowest library one `0x8004CF34`.
- **`.data` start** `0x8004DCBC`: the first game table (pointers to the game's
  own strings) after the last LIBMCRD string; the lowest stored-to address is
  `0x8004E0B4`.

## PsyQ

Version 4.7 (signature scan: 262 objects, best of all versions; `$Id` strings
dated 1998). Sony's converted libraries (`ext/psyq47/lib/*.a`) contain the
objects the game was linked with: 254 of 255 identified objects are
byte-identical, and are linked in place of their code instead of being
decompiled. `configs/USA/psyq/`:

- `objects.txt` - every object found in the exe (address, size, LIB/OBJ).
- `layout.txt` - where each object's `.data`/`.bss`/`.rdata` sat, inferred from
  its relocations; the externals each object needs, at the exe's addresses.
- `sections.ld` - the objects' data sections pinned `NOLOAD` at their original
  addresses (the data blob still supplies the bytes).
- `symbols.ld` - symbols psylink placed differently from the objects' own
  layout; read last so they win.

The few library functions not covered by an object are in
`src/main/psyq/unknown/` (named by address) and `asm/main/psyq/2mbyte.s`
(Sony's hand-written `2MBYTE.OBJ`, not in the converted libraries).

## Build pipeline

```
src/**/*.c --cpp--> .i --cc1 2.8.1 -O2 -G0--> .s --maspsx (ASPSX 2.86)--> .s --as--> build/src/**/*.c.o
asm/**/*.s (hand-written, non-function asm) --as--> build/asm/**/*.s.o
ext/psyq47/lib/*.a --ar x + psyq_objs.py fix--> build/psyq/<lib>/<obj>.o
linkers/main.ld (splat) + configs/USA/psyq/sections.ld --> build/main.full.ld
ld --> build/main.elf --objcopy--> build/main.exe   == configs/USA/checksum.sha
```

Functions not yet decompiled stay in the C files as `INCLUDE_ASM(...)`, which
pulls `asm/nonmatchings/<seg>/<func>.s` into the compiled object, so the build
matches at every step. splat (`make split`) regenerates `asm/` and `linkers/`
from the retail executable; it never overwrites existing `src/` files.

Overlays (once registered with `decomp.py add-overlay`) get their own
`configs/USA/overlays/<ovl>.yaml`, are cut from their archive to
`disc/<ovl>.bin`, link from `linkers/<ovl>.ld` into `build/<ovl>.bin` against
the executable's symbols, and are checked by the same `checksum.sha`.
