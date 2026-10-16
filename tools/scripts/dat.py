#!/usr/bin/env python3
"""CRASHBSH.DAT: split the user's retail archive into its files, and build it back from them.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/dat.py extract     retail DAT -> assets/CRASHBSH/
    tools/dk python3 tools/scripts/dat.py build OUT   assets/CRASHBSH/ -> OUT (make disc does this)

The DAT has no header: the exe indexes it with a table of {u32 sector, u32 size} pairs at
0x8004E110, 992 entries (0x000..0x3DF; what follows is another table). Each entry is written to
assets/CRASHBSH/<id>.bin (id in hex, as ARCHITECTURE.md names them: the engine is 3DF), and
index.json records each one's sector and retail size. The bytes no entry covers (the rest of each
entry's last sector, and 12 sectors no entry points at) are not zeros, so they go, in DAT order, to
filler.bin; a rebuild puts them back and gives the retail DAT byte for byte.

An entry file may be edited. It may shrink (zero-padded to its retail size, so nothing moves) but
not grow: a bigger one needs the DAT laid out again and the exe's index rewritten. assets/ is
the user's game data: git-ignored, never committed.
"""
import argparse
import hashlib
import json
import os
import struct
import sys

P_PROJ = ".decomp/project.json"
ASSETS = "assets/CRASHBSH"
INDEX = "index.json"
FILLER = "filler.bin"
TABLE = 0x8004E110
COUNT = 992
SECTOR = 2048


def die(msg):
    print(f"dat: {msg}", file=sys.stderr)
    sys.exit(1)


def read(p):
    with open(p, "rb") as f:
        return f.read()


def entry_name(i):
    return f"{i:03X}.bin"


def read_index(exe, t_addr, count=COUNT, table=TABLE):
    """[(sector, size)] from a PS-X EXE (0x800-byte header, then the text loaded at t_addr)."""
    off = 0x800 + table - t_addr
    return [struct.unpack_from("<II", exe, off + 8 * i) for i in range(count)]


def gaps(entries, total):
    """The (start, end) byte ranges of a total-byte DAT that no entry covers, in order."""
    out, pos = [], 0
    for sector, size in sorted(entries):
        a = sector * SECTOR
        if a > pos:
            out.append((pos, a))
        pos = max(pos, a + size)
    if pos < total:
        out.append((pos, total))
    return out


def split(dat, entries):
    """(entry bytes in index order, filler bytes) of a DAT."""
    files = [dat[s * SECTOR:s * SECTOR + n] for s, n in entries]
    filler = b"".join(dat[a:b] for a, b in gaps(entries, len(dat)))
    return files, filler


def join(files, filler, entries, total):
    """The DAT from its entry files and filler; each file zero-padded to its slot, none bigger."""
    out = bytearray(total)
    for i, ((sector, size), data) in enumerate(zip(entries, files)):
        if len(data) > size:
            die(f"{entry_name(i)} is {len(data)} bytes, its slot only {size} "
                "(a bigger file needs the DAT laid out again)")
        out[sector * SECTOR:sector * SECTOR + len(data)] = data
    pos = 0
    for a, b in gaps(entries, total):
        out[a:b] = filler[pos:pos + b - a]
        pos += b - a
    if pos != len(filler):
        die(f"{FILLER} is {len(filler)} bytes, the gaps hold {pos}: re-extract")
    return bytes(out)


def project():
    proj = json.load(open(P_PROJ))
    dats = {o["from"] for o in proj.get("overlays", [])}
    if len(dats) != 1:
        die(f"expected one DAT among the overlays' 'from' in {P_PROJ}, found {sorted(dats)}")
    return proj, dats.pop()


def extract(dest=ASSETS, quiet=False):
    proj, src = project()
    exe_path = os.path.join(os.path.dirname(os.path.dirname(src)), proj["exe"])
    entries = read_index(read(exe_path), int(proj["header"]["t_addr"], 16))
    dat = read(src)
    files, filler = split(dat, entries)
    if join(files, filler, entries, len(dat)) != dat:
        die("the split does not rebuild the DAT (overlapping entries?)")
    overlays = {(o["offset"] // SECTOR): o["name"] for o in proj.get("overlays", [])}
    os.makedirs(dest, exist_ok=True)
    index = []
    for i, ((sector, size), data) in enumerate(zip(entries, files)):
        with open(os.path.join(dest, entry_name(i)), "wb") as f:
            f.write(data)
        e = {"id": f"{i:03X}", "sector": sector, "size": size}
        if sector in overlays:
            e["overlay"] = overlays[sector]   # make disc writes build/<overlay>.bin over it
        index.append(e)
    with open(os.path.join(dest, FILLER), "wb") as f:
        f.write(filler)
    with open(os.path.join(dest, INDEX), "w") as f:
        json.dump({"source": src, "size": len(dat), "sha1": hashlib.sha1(dat).hexdigest(),
                   "entries": index}, f, indent=1)
        f.write("\n")
    if not quiet:
        print(f"{src}: {len(entries)} entries and {len(filler)} filler bytes -> {dest}/")


def build(out, src=ASSETS):
    """Write the DAT rebuilt from src to out; returns its index (for the overlays' slots)."""
    meta = json.load(open(os.path.join(src, INDEX)))
    entries = [(e["sector"], e["size"]) for e in meta["entries"]]
    files = [read(os.path.join(src, entry_name(i))) for i in range(len(entries))]
    with open(out, "wb") as f:
        f.write(join(files, read(os.path.join(src, FILLER)), entries, meta["size"]))
    return meta


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("extract", help=f"the retail DAT -> {ASSETS}/")
    b = sub.add_parser("build", help=f"{ASSETS}/ -> a DAT")
    b.add_argument("out")
    a = ap.parse_args()
    if a.cmd == "extract":
        extract()
    else:
        meta = build(a.out)
        sha = hashlib.sha1(read(a.out)).hexdigest()
        print(f"{a.out}: {'identical to' if sha == meta['sha1'] else 'differs from'} the retail DAT")


if __name__ == "__main__":
    main()
