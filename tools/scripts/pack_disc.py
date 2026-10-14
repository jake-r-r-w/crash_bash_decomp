#!/usr/bin/env python3
"""Disc packer: put the built exe and overlays back into a bootable disc image.

Runs inside the toolchain container:   tools/dk make disc   (or make disc-check)

The user's retail image is unpacked with dumpsxiso into build/disc/ (a fresh copy every run, so
nothing from an earlier pack leaks in), build/main.exe replaces the boot exe, every build/<ovl>.bin
is written back into CRASHBSH.DAT at the offset decomp.py add-overlay recorded, and mkpsxiso
rebuilds the image from dumpsxiso's layout, which keeps every file at its retail sector. The result
is build/CrashBash.bin/.cue. Packing the matching build gives the retail image byte for byte
(configs/USA/disc.sha, checked by make disc-check).

A file may shrink (it is zero-padded to its retail size, so nothing after it moves) but not grow:
an overlay is a slot in CRASHBSH.DAT, and a bigger one needs the DAT and its index rebuilt.
"""
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys

P_PROJ = ".decomp/project.json"
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


def main():
    proj = json.load(open(P_PROJ))
    src = retail_bin(proj)
    if not os.path.exists(src):
        die(f"{src} not found (the retail disc image goes in disc/)")

    shutil.rmtree(OUT, ignore_errors=True)
    run(["dumpsxiso", "-x", FILES, "-s", LAYOUT, src])

    exe = f"{FILES}/{proj['exe']}"
    put(exe, read(f"build/{proj['name']}.exe"), size=os.path.getsize(exe))
    for o in proj.get("overlays", []):
        # recorded as disc/files/..., the layout dumpsxiso writes; the same path under build/disc/
        dat = os.path.join(FILES, os.path.relpath(o["from"], "disc/files"))
        put(dat, read(f"build/{o['name']}.bin"), o["offset"], o["size"])

    run(["mkpsxiso", "-y", "-q", "-o", IMAGE, "-c", CUE, LAYOUT])
    sha = hashlib.sha1(read(IMAGE)).hexdigest()
    same = sha == hashlib.sha1(read(src)).hexdigest()
    print(f"{IMAGE} + {os.path.basename(CUE)}: sha1 {sha[:12]}, "
          + ("identical to the retail image" if same else "differs from the retail image"))


if __name__ == "__main__":
    main()
