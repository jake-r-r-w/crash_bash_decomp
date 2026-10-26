#!/usr/bin/env python3
"""Disc packer: put the built exe and overlays back into a bootable disc image.

Runs inside the toolchain container:   tools/dk make disc   (or make disc-check)

It starts from the retail image as `decomp.py extract` unpacked it (dumpsxiso: disc/files/ and
disc/layout.xml), copied to build/disc/ every run, so nothing from an earlier pack leaks in
(files the pack doesn't change are hard links). build/main.exe replaces
the boot exe; CRASHBSH.DAT is built from its files in assets/CRASHBSH/ (tools/scripts/dat.py,
which extracts them from the retail DAT first if they aren't there), and every build/<ovl>.bin
is written over it at the offset decomp.py add-overlay recorded; mkpsxiso rebuilds the image from dumpsxiso's layout, which keeps every
file at its retail sector. The result is build/CrashBash.bin/.cue. Packing the matching build gives the retail image byte for byte
(configs/USA/disc.sha, checked by make disc-check).

A file may shrink (it is zero-padded to its retail size, so nothing after it moves). With --grow,
an overlay bigger than its slot stays in place and CRASHBSH.DAT is laid out again around it
(dat.relayout: entries keep their order, the ones after it move down by whole sectors only when
it outgrows the sectors before the next entry), and the exe's DAT index (the {sector, size}
table at 0x8004E110) and its file groups' byte sizes (0x80050010) are rewritten. An entry can't
move elsewhere: the exe loads a file group, a run of consecutive entries (the 13 slot overlays
are one), in one read and finds each inside it by its sector offset. The game finds the DAT
through the CD's file system, so the DAT may grow. Without --grow a bigger overlay is an
error.

    pack_disc.py [--out DIR] [--use OVL=PATH ...] [--grow]

--out packs into DIR (CrashBash.bin/.cue and DIR/disc/) instead of build/; --use takes an
overlay's binary from PATH instead of build/<ovl>.bin (the shifted build, tools/scripts/shift.py).
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET

import dat

P_PROJ = ".decomp/project.json"
SRC = "disc"                           # decomp.py extract: files/ and layout.xml
OUT = "build/disc"
FILES = f"{OUT}/files"
LAYOUT = f"{OUT}/layout.xml"
IMAGE = "build/CrashBash.bin"
CUE = "build/CrashBash.cue"


def die(msg):
    print(f"pack_disc: {msg}", file=sys.stderr)
    sys.exit(1)


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        die(f"{cmd[0]} failed:\n" + "\n".join((r.stdout + r.stderr).splitlines()[-15:]))


def retail_bin(proj):
    cue = proj.get("cue") or die("no cue in .decomp/project.json (run decomp.py extract)")
    with open(cue) as f:
        m = re.search(r'FILE\s+"([^"]+)"', f.read(), re.I) or die(f"no FILE line in {cue}")
    return os.path.join(os.path.dirname(cue), m.group(1))


def put(dst, data, off=0, size=None):
    """Write data at off in dst, zero-padded to size (the retail size of that slot)."""
    size = len(data) if size is None else size
    if len(data) > size:
        die(f"{dst}+0x{off:X}: built file is {len(data)} bytes, its retail slot only {size}")
    with open(dst, "r+b") as f:
        f.seek(off)
        f.write(data + bytes(size - len(data)))


def read(p):
    with open(p, "rb") as f:
        return f.read()


def unpacked(src):
    """The retail image's sha1, after checking disc/ holds every file its layout names (decomp.py
    extract writes both; run it again if one is missing)."""
    layout = f"{SRC}/layout.xml"
    if not os.path.exists(layout):
        die(f"no {layout}: run decomp.py extract on your cue first (README, Quick Start)")
    tree = ET.parse(layout).getroot().find(".//directory_tree")

    def files(node, rel):
        for c in node:
            if c.tag == "file":
                yield os.path.join(rel, c.get("name"))
            elif c.tag == "dir":
                yield from files(c, os.path.join(rel, c.get("name")))
    missing = [p for p in files(tree, f"{SRC}/files") if not os.path.exists(p)]
    if missing:
        die(f"{', '.join(missing[:3])} missing from {SRC}/files: run decomp.py extract again")
    return hashlib.sha1(read(src)).hexdigest()


def copy_tree(changed):
    """SRC -> OUT: the files in changed are copied (they get written), the rest hard-linked."""
    shutil.rmtree(OUT, ignore_errors=True)
    for dp, _, fns in os.walk(f"{SRC}/files"):
        rel = os.path.relpath(dp, SRC)
        os.makedirs(os.path.join(OUT, rel), exist_ok=True)
        for fn in fns:
            a, b = os.path.join(dp, fn), os.path.join(OUT, rel, fn)
            if os.path.normpath(b) in changed:
                shutil.copyfile(a, b)
                continue
            try:
                os.link(a, b)
            except OSError:
                shutil.copyfile(a, b)
    shutil.copyfile(f"{SRC}/layout.xml", LAYOUT)


GROUPS = 0x80050010    # the exe's file groups, right after the index: {u32 first, u32 count, u32 bytes}


def span(entries, first, count):
    """Bytes from a group's first entry's sector to the end of its last entry's last sector."""
    last = entries[first + count - 1]
    return (last["sector"] - entries[first]["sector"]) * dat.SECTOR + -(-last["size"] // dat.SECTOR) * dat.SECTOR


def write_index(exe_path, proj, old, meta):
    """The exe's DAT index ({u32 sector, u32 size} per entry at dat.TABLE) from a relaid DAT's
    index, and its file groups' byte sizes, each grown by its span's growth (the loader reads a
    group in one go; 6 of the 130 stored sizes aren't the span formula's, so only the change is
    applied)."""
    t_addr = int(proj["header"]["t_addr"], 16)
    with open(exe_path, "r+b") as f:
        f.seek(0x800 + dat.TABLE - t_addr)
        f.write(b"".join(e["sector"].to_bytes(4, "little") + e["size"].to_bytes(4, "little")
                         for e in meta["entries"]))
        n = len(meta["entries"])
        for g in range(0x400):
            at = 0x800 + GROUPS - t_addr + 12 * g
            f.seek(at)
            first, count, size = (int.from_bytes(f.read(4), "little") for _ in range(3))
            if count == 0 or first + count > n:
                break
            grown = span(meta["entries"], first, count) - span(old["entries"], first, count)
            if grown:
                f.seek(at + 8)
                f.write((size + grown).to_bytes(4, "little"))
                print(f"pack_disc: file group {g} (entries {first:03X}..{first + count - 1:03X}): "
                      f"{size:#x} -> {size + grown:#x} bytes")


def main():
    global OUT, FILES, LAYOUT, IMAGE, CUE
    ap = argparse.ArgumentParser(description="Pack the built exe and overlays into a disc image.")
    ap.add_argument("--out", help="pack into this directory instead of build/")
    ap.add_argument("--use", action="append", default=[], metavar="OVL=PATH",
                    help="take an overlay's binary from PATH")
    ap.add_argument("--grow", action="store_true", help="lay the DAT out again around an overlay bigger than its slot")
    a = ap.parse_args()
    if a.out:
        OUT, IMAGE, CUE = f"{a.out}/disc", f"{a.out}/CrashBash.bin", f"{a.out}/CrashBash.cue"
        FILES, LAYOUT = f"{OUT}/files", f"{OUT}/layout.xml"
    use = dict(u.split("=", 1) for u in a.use)
    proj = json.load(open(P_PROJ))
    src = retail_bin(proj)
    if not os.path.exists(src):
        die(f"{src} not found (the retail disc image goes in disc/)")

    retail_sha = unpacked(src)
    # recorded as disc/files/..., the layout dumpsxiso writes; the same path under build/disc/
    dats = {o["name"]: os.path.join(FILES, os.path.relpath(o["from"], "disc/files"))
            for o in proj.get("overlays", [])}
    exe = f"{FILES}/{proj['exe']}"
    copy_tree({os.path.normpath(exe)})

    put(exe, read(f"build/{proj['name']}.exe"), size=os.path.getsize(exe))
    if not os.path.exists(os.path.join(dat.ASSETS, dat.INDEX)):
        print(f"pack_disc: no {dat.ASSETS}/ yet: extracting it from the retail DAT")
        dat.extract()
    bins = {o["name"]: read(use.get(o["name"], f"build/{o['name']}.bin")) for o in proj.get("overlays", [])}
    big = {n: d for n, d in bins.items() if len(d) > next(o["size"] for o in proj["overlays"] if o["name"] == n)}
    if big and not a.grow:
        n = next(iter(big))
        die(f"{n}: built file is {len(big[n])} bytes, its retail slot smaller (pack with --grow)")
    meta = old = json.load(open(os.path.join(dat.ASSETS, dat.INDEX)))
    # each overlay's entry by its retail sector (index.json's "overlay" names are the ones at
    # extract time, and an overlay unit may have been renamed since)
    at = {e["sector"]: i for i, e in enumerate(meta["entries"])}
    entry = {o["name"]: at[o["offset"] // dat.SECTOR] for o in proj.get("overlays", [])}
    for p in set(dats.values()):
        os.remove(p)                 # a hard link into the cache: replace it, don't write through it
        meta = dat.build(p, grow={entry[n]: d for n, d in big.items()})
    if big:
        write_index(exe, proj, old, meta)
        for n, d in big.items():
            e = meta["entries"][entry[n]]
            print(f"pack_disc: {n} is {len(d)} bytes, over its slot: the DAT is laid out again "
                  f"({n} at sector {e['sector']}, {meta['size'] // dat.SECTOR} sectors; the exe's index rewritten)")
    for o in proj.get("overlays", []):
        e = meta["entries"][entry[o["name"]]]
        put(dats[o["name"]], bins[o["name"]], e["sector"] * dat.SECTOR, e["size"])

    run(["mkpsxiso", "-y", "-q", "-o", IMAGE, "-c", CUE, LAYOUT])
    sha = hashlib.sha1(read(IMAGE)).hexdigest()
    same = sha == retail_sha
    print(f"{IMAGE} + {os.path.basename(CUE)}: sha1 {sha[:12]}, "
          + ("identical to the retail image" if same else "differs from the retail image"))


if __name__ == "__main__":
    main()
