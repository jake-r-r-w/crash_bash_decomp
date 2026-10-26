#!/usr/bin/env python3
"""Rename one source file (a unit of a segment) everywhere: its C file, owned header, yaml paths,
.decomp entries, asm and the Makefile's per-unit settings.

    tools/dk python3 tools/scripts/rename_file.py SEG NEW [--dry-run]
    e.g. rename_file.py overlays/bash/800bb64c xtras

SEG is the unit's path under src/ without .c (as the yaml and .decomp name it). Names never
reach the binary, so this changes no byte. What it moves and rewrites:
- src/SEG.c -> the same directory's NEW.c; its owned header (the include/bash/*.h marked
  `owner: src/SEG.c`) has the old file name in its own replaced by NEW (bash/bash_800bb64c.h ->
  bash/bash_xtras.h, bash/80083708.h -> bash/<NEW>.h), with its include guard;
- the text of every tracked file: the unit's path (SEG, src/SEG.c, build/src/SEG.c.o), the
  header's include path and guard, and the bare `OLD.c` where it names this file in its own
  directory's sources and headers;
- the ignored files the tools read: asm/nonmatchings/SEG moves to the new name (splat writes a
  matched function's asm only once); build/src/SEG.c.* are deleted (the build writes them again).
A file or header of the new name that exists already stops it. Run `make check` after it.
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys

SELF = {"tools/scripts/rename_file.py", "tools/scripts/tests/test_rename_file.py"}


def tracked():
    out = subprocess.run(["git", "ls-files", "-z"], capture_output=True, check=True).stdout
    # the tools' tests keep their fixtures as they are
    return [p for p in out.decode().split("\0") if p and p not in SELF and os.path.exists(p)
            and not p.startswith("tools/scripts/tests/")]


def owned_header(seg):
    for h in sorted(glob.glob("include/bash/*.h")):
        if f"owner: src/{seg}.c" in open(h, encoding="utf-8").read():
            return h
    return None


def new_header(h, old, new):
    """include/bash/bash_800bb64c.h -> include/bash/bash_xtras.h (the old stem as a word)."""
    d, b = os.path.split(h)
    nb = re.sub(r"(?<![0-9A-Za-z])" + re.escape(old) + r"(?![0-9A-Za-z])", new, b)
    return os.path.join(d, nb).replace("\\", "/")


def guard(h):
    return "BASH_" + os.path.splitext(os.path.basename(h))[0].upper() + "_H"


def rewrite(text, seg, new_seg, hdr, new_hdr, local):
    """The new text of one file; local: whether it is in the unit's own directory or a header,
    where a bare `OLD.c` names this file."""
    old, new = os.path.basename(seg), os.path.basename(new_seg)
    text = re.sub(r"(?<![0-9A-Za-z_])" + re.escape(seg) + r"(?![0-9A-Za-z_])", new_seg, text)
    if hdr:
        text = text.replace("bash/" + os.path.basename(hdr), "bash/" + os.path.basename(new_hdr))
        text = re.sub(r"\b" + guard(hdr) + r"\b", guard(new_hdr), text)
    if local:
        text = re.sub(r"(?<![0-9A-Za-z_/.])" + re.escape(old) + r"\.c\b", new + ".c", text)
    return text


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("seg")
    ap.add_argument("new")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    seg = a.seg.strip("/")
    c = f"src/{seg}.c"
    if not os.path.exists(c):
        sys.exit(f"rename_file: no {c}")
    new_seg = f"{os.path.dirname(seg)}/{a.new}"
    nc = f"src/{new_seg}.c"
    hdr = owned_header(seg)
    nhdr = new_header(hdr, os.path.basename(seg), a.new) if hdr else None
    for p in (nc, nhdr):
        if p and os.path.exists(p):
            sys.exit(f"rename_file: {p} exists")
    if hdr and nhdr == hdr:
        sys.exit(f"rename_file: {hdr} doesn't carry the file's name; rename it by hand")
    srcdir = os.path.dirname(c)
    changed = {}
    for p in tracked():
        try:
            t = open(p, encoding="utf-8").read()
        except UnicodeDecodeError:
            continue
        local = os.path.dirname(p) == srcdir or p == hdr
        n = rewrite(t, seg, new_seg, hdr, nhdr, local)
        if n != t:
            changed[p] = n
    print(f"{c} -> {nc}" + (f"; {hdr} -> {nhdr}" if hdr else "") + f"; {len(changed)} files rewritten")
    if a.dry_run:
        for p in sorted(changed):
            print("  " + p)
        return
    for p, t in changed.items():
        open(p, "w", encoding="utf-8", newline="").write(t)
    os.rename(c, nc)
    if hdr:
        os.rename(hdr, nhdr)
    old_asm, new_asm = f"asm/nonmatchings/{seg}", f"asm/nonmatchings/{new_seg}"
    if os.path.isdir(old_asm):
        if os.path.isdir(new_asm):
            sys.exit(f"rename_file: {new_asm} exists")
        os.rename(old_asm, new_asm)
    for p in glob.glob(f"build/src/{seg}.c.*"):
        os.remove(p)
    print("done: now make check")


if __name__ == "__main__":
    main()
