#!/usr/bin/env python3
"""Writes port/data_order.txt: every global data object of the game, in the PS1's address order.

Runs inside the toolchain container, after a build of the PS1 binaries:
    tools/dk python3 tools/scripts/pc_data_order.py

The game reads some of its globals through others: GameInfo's struct covers GameMode and its neighbours,
and many arrays run on into the next object. That needs the PS1's layout. A compiler doesn't keep a
file's data in order, so the PC build puts each object in a section of its own (-fdata-sections) and has
the linker lay them out from this list (docs/PORT.md). An overlay's data sits between its two markers,
which is what the PC build saves and restores when the game loads that overlay.
"""
import glob
import re
import subprocess
import sys

SLOT = 0x800B32B4
OVERLAYS = ["arkenoid", "bash", "bonk", "chase", "dino", "hub", "n2o", "papu", "pogo", "polar", "race", "spin",
            "tank", "titles"]
# Names the PC build gives memory of its own (port/link_names.c).
SKIP = {"HeapStart", "ModeTextBuffer"}
LINE = re.compile(r"^\s*\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+(?:OBJECT|NOTYPE)\s+\S+\s+\S+\s+(\S+)\s+(\S+)$")


def symbols(path):
    """[(address, name, section)] of the data symbols of an ELF (ASPSX leaves them untyped)."""
    out = subprocess.run(["mipsel-linux-gnu-readelf", "-sW", path], check=True, capture_output=True, text=True).stdout
    return [(int(m.group(1), 16), m.group(4), m.group(3)) for m in map(LINE.match, out.splitlines()) if m]


def game_names():
    """The names the game's own C defines: Sony's are in the PS1 link, and not in the PC build."""
    names = set()
    for obj in glob.glob("build/src/**/*.c.o", recursive=True):
        names.update(name for _, name, section in symbols(obj) if section != "UND")
    return names


def objects(elf, lo, hi, game):
    """The game's data symbols defined in [lo, hi) of an ELF, by address."""
    found = {name: addr for addr, name, section in symbols(elf)
             if section != "UND" and lo <= addr < hi and name in game and name not in SKIP}
    return [n for _, n in sorted((a, n) for n, a in found.items())]


def main():
    game = game_names()
    lines = objects("build/main.elf", 0x80010000, 0x80078C90, game)
    lines += objects("build/engine.ovl.elf", 0x80078C90, SLOT, game)
    for ovl in OVERLAYS:
        lines.append(f"ovl_{ovl}_data_begin")
        lines += objects(f"build/{ovl}.ovl.elf", SLOT, 0x801FF000, game)
        lines.append(f"ovl_{ovl}_data_end")
    with open("port/data_order.txt", "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")
    print(f"port/data_order.txt: {len(lines)} objects")
    return 0


if __name__ == "__main__":
    sys.exit(main())
