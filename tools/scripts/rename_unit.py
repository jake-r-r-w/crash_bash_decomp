#!/usr/bin/env python3
"""Rename a slot overlay unit everywhere: its files, paths, header, keys and type prefix.

    tools/dk python3 tools/scripts/rename_unit.py OLD NEW PREFIX [--as OLDNAME=NEWNAME ...] [--dry-run]
    e.g. rename_unit.py ovl62 polar Polar --as Ovl62Bomb=PolarBombExt

Names never reach the binary, so this changes no byte. What it moves and rewrites:
- tracked paths naming OLD (configs/USA/overlays/OLD.yaml, sym.OLD.txt, src/overlays/OLD/,
  OLD_id.c, include/bash/OLD_<addr>.h); the overlay's owned header include/bash/OLD.h becomes
  include/bash/NEW_ovl.h, since the leak's include/bash/NEW.h is the original header;
- the text of every tracked file: `bash/OLD.h` includes and the BASH_OLD_H guard, the lowercase
  name as a word (paths, `OLD:ADDRESS` keys in .decomp, `OLD_` file names, prose), and the
  C names that carry it as a prefix (`OvlXXName` -> PREFIX + Name, dropping a doubled
  PREFIX; `OVLXX_NAME` -> PREFIX upper + _NAME). A new C name that already exists stops it
  (--as names it instead);
- the ignored files the build reads or writes under the old name: disc/OLD.bin and the split's
  asm/ move (splat can't write a matched function's asm again); linkers/ and build/ products
  of OLD are deleted (`make split` and the build write them again under NEW).
Run `make split` and `make check` after it.
"""
import argparse
import os
import re
import shutil
import subprocess
import sys


SELF = {"tools/scripts/rename_unit.py", "tools/scripts/tests/test_rename_unit.py"}


def tracked():
    """Tracked files (stage a rename before the next one) but this tool's own, whose examples
    keep the old names."""
    out = subprocess.run(["git", "ls-files", "-z"], capture_output=True, check=True).stdout
    return [p for p in out.decode().split("\0") if p and p not in SELF and os.path.exists(p)]


def word(old):
    """The lowercase unit name as a word: not inside another name's letters or digits."""
    return re.compile(r"(?<![A-Za-z0-9])" + re.escape(old) + r"(?![A-Za-z0-9])")


def new_path(p, old, new):
    if p == f"include/bash/{old}.h":
        return f"include/bash/{new}_ovl.h"
    return word(old).sub(new, p)


def c_names(text, old, prefix):
    """{old C name: new} for the names that carry the unit's name as a prefix."""
    out = {}
    cap = old[0].upper() + old[1:]
    for m in re.finditer(r"\b" + re.escape(cap) + r"(_?)([A-Z]\w*)\b", text):
        rest = m.group(2)
        if rest.startswith(prefix) and rest[len(prefix):len(prefix) + 1].isupper():
            rest = rest[len(prefix):]
        out[m.group(0)] = prefix + m.group(1) + rest
    for m in re.finditer(r"\b" + re.escape(old.upper()) + r"_(\w+)\b", text):
        if not m.group(0).startswith("BASH_"):
            out[m.group(0)] = prefix.upper() + "_" + m.group(1)
    return out


def rewrite(text, old, new, names):
    text = text.replace(f"bash/{old}.h", f"bash/{new}_ovl.h")
    text = text.replace(f"BASH_{old.upper()}_H", f"BASH_{new.upper()}_OVL_H")
    text = text.replace(f"BASH_{old.upper()}_", f"BASH_{new.upper()}_")
    for o in sorted(names, key=len, reverse=True):
        text = re.sub(r"\b" + re.escape(o) + r"\b", names[o], text)
    return word(old).sub(new, text)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("old")
    ap.add_argument("new")
    ap.add_argument("prefix", help="the C name prefix, e.g. Polar")
    ap.add_argument("--as", dest="as_", action="append", default=[], metavar="OLDNAME=NEWNAME",
                    help="the new name of a C name whose default one is taken")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    old, new = a.old, a.new
    if not os.path.exists(f"configs/USA/overlays/{old}.yaml"):
        sys.exit(f"rename_unit: no configs/USA/overlays/{old}.yaml")
    if os.path.exists(f"configs/USA/overlays/{new}.yaml"):
        sys.exit(f"rename_unit: configs/USA/overlays/{new}.yaml exists")
    files = tracked()
    texts = {}
    for p in files:
        try:
            texts[p] = open(p, encoding="utf-8").read()
        except (UnicodeDecodeError, IsADirectoryError, FileNotFoundError):
            pass
    names = {}
    for t in texts.values():
        names.update(c_names(t, old, a.prefix))
    for s in a.as_:
        o, n = s.split("=")
        if o not in names:
            sys.exit(f"rename_unit: --as {o}: no such C name")
        names[o] = n
    taken = set()
    for t in texts.values():
        taken.update(re.findall(r"\b\w+\b", t))
    dups = {n for n in names.values() if list(names.values()).count(n) > 1}
    clash = sorted(f"{o} -> {n}" for o, n in names.items() if n in taken or n in dups)
    if clash:
        sys.exit("rename_unit: new names already in use:\n  " + "\n  ".join(clash))
    moves = {p: new_path(p, old, new) for p in files if new_path(p, old, new) != p}
    changed = [p for p, t in texts.items() if rewrite(t, old, new, names) != t]
    print(f"{len(moves)} files moved, {len(changed)} rewritten, {len(names)} C names:")
    for o, n in sorted(names.items()):
        print(f"  {o} -> {n}")
    if a.dry_run:
        for p, q in sorted(moves.items()):
            print(f"  mv {p} -> {q}")
        return
    for p in changed:
        open(p, "w", encoding="utf-8", newline="").write(rewrite(texts[p], old, new, names))
    for p, q in moves.items():
        os.makedirs(os.path.dirname(q), exist_ok=True)
        os.rename(p, q)
    for d in {os.path.dirname(p) for p in moves}:
        while d and os.path.isdir(d) and not os.listdir(d):
            os.rmdir(d)
            d = os.path.dirname(d)
    if os.path.exists(f"disc/{old}.bin"):
        os.rename(f"disc/{old}.bin", f"disc/{new}.bin")
    # splat writes a function's asm only while its C still includes it, so the asm of matched
    # functions (which similar, regions and the rest read) can't be split again: move it
    for kind in ("nonmatchings", "data", "bin"):
        src = f"asm/{kind}/overlays/{old}"
        if not os.path.isdir(src):
            continue
        for dirpath, _, fnames in os.walk(src, topdown=False):
            for f in fnames:
                p = os.path.join(dirpath, f).replace("\\", "/")
                q = word(old).sub(new, p)
                os.makedirs(os.path.dirname(q), exist_ok=True)
                os.replace(p, q)
            os.rmdir(dirpath)
    stale = [f"build/src/overlays/{old}", f"build/asm/overlays/{old}", f"build/asm/data/overlays/{old}",
             f"build/asm/bin/overlays/{old}"]
    for root in ("linkers", "build", "build/shift"):
        if os.path.isdir(root):
            stale += [os.path.join(root, f) for f in os.listdir(root) if re.match(re.escape(old) + r"[._]", f)]
    for s in stale:
        if os.path.isdir(s):
            shutil.rmtree(s)
        elif os.path.exists(s):
            os.remove(s)
    print("done: now make split, then make check")


if __name__ == "__main__":
    main()
