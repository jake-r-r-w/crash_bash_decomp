#!/usr/bin/env python3
"""Find pointers written as numbers: data arrays in C (bytes, halfwords or words with plain
numbers) with a word-aligned word equal to a symbol's address, or inside a sized data object.

    tools/dk python3 tools/scripts/bytes_ptrs.py [--src src] [--build build]

data2c writes a data block as words with relocations where the object had one, but a block it
read as bytes (or halfwords) keeps any pointer in it as plain numbers. Those bytes match while
nothing moves, and a shifted build (`make shift`) leaves them pointing at the old address. This
compiles nothing: each array's address and the symbols come from its unit's linked ELF (the
exe's, the engine's or the slot overlay's, which carries the exe's and engine's symbols too), so
`make` first. Words equal to a symbol of the same ELF are reported, with the symbol, and so are
words inside a data object of known size (`SYM+0xNN`: an element of a table, like
&DatIndex[2 * N]); a hit can be a coincidence (a sequence word like 0x80070000 that happens to
land on a label), so read it before rewriting the array as words with (s32)&SYM or as records
with pointer fields. Exit status 1 when anything is found.
"""
import argparse
import glob
import os
import re
import subprocess
import sys

ARRAY = re.compile(r"^(?:static )?(?:const )?(u8|s8|u16|s16|char|s32|u32) (\w+)((?:\[[^\]]*\])+) = \{(.*?)\};",
                   re.M | re.S)
SIZE = {"u8": 1, "s8": 1, "char": 1, "u16": 2, "s16": 2, "s32": 4, "u32": 4}


def elf_for(path, build):
    parts = os.path.normpath(path).split(os.sep)
    if "main" in parts:
        return os.path.join(build, "main.elf")
    if "overlays" in parts:
        return os.path.join(build, "%s.ovl.elf" % parts[parts.index("overlays") + 1])
    return None


def symbols(elf, funcs=frozenset()):
    """address -> name, name -> address, and the data objects [(start, size, name)] of an ELF's
    symbols (nm). The linked symbols carry no sizes, so a data object runs to the next symbol;
    function addresses (funcs) are not data."""
    out = subprocess.run(["mipsel-linux-gnu-nm", "-n", elf], capture_output=True, text=True).stdout
    by_addr, by_name = {}, {}
    for l in out.splitlines():
        f = l.split()
        if len(f) == 3 and not f[2].startswith(("$", ".")) and "compiled" not in f[2]:
            a = int(f[0], 16)
            by_addr.setdefault(a, f[2])
            by_name.setdefault(f[2], a)
    addrs = sorted(a for a in by_addr if a >= 0x80010000)
    objects = []
    for a, b in zip(addrs, addrs[1:]):
        if a not in funcs and b - a > 4:
            objects.append((a, b - a, by_addr[a]))
    return by_addr, by_name, objects


def function_addresses(path=".decomp/functions.json"):
    """Every function's address (functions.json), so a word inside one is not read as data."""
    try:
        import json
        return frozenset(int(k, 16) for k in json.load(open(path)))
    except (OSError, ValueError):
        return frozenset()


def inside(w, objects):
    """'SYM+0xNN' when w lies inside one of the sized objects (not at its start), else None."""
    lo, hi = 0, len(objects)
    while lo < hi:
        mid = (lo + hi) // 2
        if objects[mid][0] <= w:
            lo = mid + 1
        else:
            hi = mid
    if lo and objects[lo - 1][0] < w < objects[lo - 1][0] + objects[lo - 1][1]:
        a, n, name = objects[lo - 1]
        return "%s+0x%X" % (name, w - a)
    return None


def values(body):
    """The initializer's numbers, or None if it holds anything else (a symbol, a string)."""
    out = []
    for tok in re.sub(r"[{}]", ",", body).split(","):
        tok = tok.strip()
        if not tok:
            continue
        neg = tok.startswith("-")
        tok = tok.lstrip("-").strip()
        if tok.startswith("'") and tok.endswith("'") and len(tok) == 3:
            v = ord(tok[1])
        else:
            try:
                v = int(tok, 0)
            except ValueError:
                return None
        out.append(-v if neg else v)
    return out


def scan(vals, size, addr, by_addr, lo=0x80010000, objects=()):
    """(offset, word, symbol) for each aligned word of the array equal to a symbol's address or
    inside one of objects."""
    data = bytearray()
    for v in vals:
        data += (v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
    hits = []
    for off in range((-addr) % 4, len(data) - 3, 4):
        w = int.from_bytes(data[off:off + 4], "little")
        if w < lo:
            continue
        if w in by_addr:
            hits.append((off, w, by_addr[w]))
        else:
            s = inside(w, objects)
            if s:
                hits.append((off, w, s))
    return hits


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--src", default="src")
    ap.add_argument("--build", default="build")
    a = ap.parse_args()
    cache = {}
    funcs = function_addresses()
    total = 0
    scanned = {}
    for p in sorted(glob.glob(os.path.join(a.src, "**", "*.c"), recursive=True)):
        elf = elf_for(p, a.build)
        if elf is None or not os.path.exists(elf):
            continue
        if elf not in cache:
            cache[elf] = symbols(elf, funcs)
        by_addr, by_name, objects = cache[elf]
        for kind, name, dims, body in ARRAY.findall(open(p, encoding="utf-8").read()):
            addr = by_name.get(name)
            vals = values(body)
            if addr is None or vals is None:
                continue
            scanned[os.path.basename(elf)] = scanned.get(os.path.basename(elf), 0) + 1
            for off, w, sym in scan(vals, SIZE[kind], addr, by_addr, objects=objects):
                print("%s: %s+0x%X = 0x%08X (%s)" % (p, name, off, w, sym))
                total += 1
    print("scanned: " + ", ".join("%s %d" % kv for kv in sorted(scanned.items())))
    print("%d pointer%s written as numbers" % (total, "" if total == 1 else "s"))
    return 1 if total else 0


if __name__ == "__main__":
    sys.exit(main())
