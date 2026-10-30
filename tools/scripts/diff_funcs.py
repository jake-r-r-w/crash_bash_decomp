#!/usr/bin/env python3
"""Every function whose bytes differ between a built binary and the retail one.

    tools/dk python3 tools/scripts/diff_funcs.py [UNIT ...]

UNIT is `main` (build/main.exe against disc/SCUS_945.70) or an overlay (build/OVL.bin against
disc/OVL.bin); with none, every unit. `decomp.py verify` stops at the first difference; this lists
them all (by functions.json's offsets), so after a type change you see every function it moved
and the first one to look at. A unit whose size changed lists everything after the change.
"""
import argparse
import json
import os
import sys


MAIN = "disc/SCUS_945.70"


def bin_of(e):
    return e.get("bin") or (MAIN if e.get("seg", "").startswith(("main/", "ps1/")) else None)


def differing(funcs, built, target, key):
    """[(rom offset, name)] of functions whose bytes differ."""
    out = []
    for e in funcs.values():
        if bin_of(e) != key or e.get("rom") is None or not e.get("size"):
            continue
        o, n = e["rom"], e["size"]
        if built[o:o + n] != target[o:o + n]:
            out.append((o, e["name"]))
    return sorted(out)


def units(funcs):
    return sorted(set(bin_of(e) for e in funcs.values() if bin_of(e)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("units", nargs="*")
    a = ap.parse_args()
    funcs = json.load(open(".decomp/functions.json"))
    keys = units(funcs)
    want = a.units or ["main" if k == MAIN else os.path.splitext(os.path.basename(k))[0] for k in keys]
    total = 0
    for u in want:
        key = MAIN if u == "main" else next((k for k in keys if os.path.splitext(os.path.basename(k))[0] == u), None)
        if key is None:
            print("?? no unit", u)
            continue
        built_p = "build/main.exe" if key == MAIN else "build/%s.bin" % os.path.splitext(os.path.basename(key))[0]
        if not os.path.exists(built_p):
            print("?? not built:", built_p)
            continue
        built, target = open(built_p, "rb").read(), open(key, "rb").read()
        d = differing(funcs, built, target, key)
        if len(built) != len(target):
            print("%s: size %d, retail %d" % (u, len(built), len(target)))
        for o, n in d:
            print("%s 0x%X %s" % (u, o, n))
        total += len(d)
    print("%d differ" % total)
    return 1 if total else 0


if __name__ == "__main__":
    sys.exit(main())
