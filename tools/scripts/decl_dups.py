#!/usr/bin/env python3
"""Declarations a header repeats, which CONVENTIONS.md (Headers) rules out:

    tools/dk python3 tools/scripts/decl_dups.py [--views]

A prototype or extern declared twice in one header, or repeated in an overlay's owned header
(`<ovl>_ovl.h`) when the header recovered from the original source (`<ovl>.h`) already declares
it (the leaked header wins). Exits 1 if there are any. --views also lists the names several units'
headers declare, each with its own type: an overlay's view of an engine global, to fold into the
one type (typing work, not an error). Two kinds are not views and are left out: a name a leaked
header (`pogo.h`, ...) declares, which is the original's own text, and a function several units
define, each its own (they are in different binaries).
"""
import collections
import glob
import os
import re
import sys

LEAKED = {"arkenoid_ovl.h": "arkenoid.h", "pogo_ovl.h": "pogo.h", "polar_ovl.h": "polar.h", "tank_ovl.h": "tank.h"}
DECL = re.compile(r"^(?:extern\s+[^;(]*?\b(\w+)\s*(?:\[[^\]]*\])*|(?!typedef|extern|static|#|/\*| )"
                  r"[A-Za-z_][\w\s\*]*?\b(\w+)\s*\([^;{}]*\))\s*;", re.M)


def declarations(text):
    """[(name, line number)] of the prototypes and externs a header declares at file scope."""
    return [(m.group(1) or m.group(2), text.count("\n", 0, m.start()) + 1) for m in DECL.finditer(text)]


def repeats(headers):
    """[(header, line, name, why)] for {header name: text}."""
    decls = {h: declarations(t) for h, t in headers.items()}
    out = []
    for h, ds in decls.items():
        seen = {}
        for name, line in ds:
            if name in seen:
                out.append((h, line, name, "already declared at line %d" % seen[name]))
            else:
                seen[name] = line
        leak = LEAKED.get(os.path.basename(h))
        if leak:
            lh = next((k for k in decls if os.path.basename(k) == leak), None)
            names = {n for n, _ in decls.get(lh, [])}
            out += [(h, line, name, "declared in the leaked %s" % leak) for name, line in ds if name in names]
    return sorted(out)


def views(headers, skip=()):
    """{name: [header]} for the names more than one header declares (the units' own views),
    but the names in SKIP."""
    by = collections.defaultdict(set)
    for h, t in headers.items():
        for name, _ in declarations(t):
            by[name].add(os.path.basename(h))
    return {n: sorted(hs) for n, hs in by.items() if len(hs) > 1 and n not in skip}


DEF = re.compile(r"^(?!static\b|extern\b|typedef\b)[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{}]*\)\s*\{", re.M)


def not_views(headers, src="src"):
    """The names views() leaves out: those a leaked header declares, and functions more than
    one C file defines."""
    leaked = {n for h, t in headers.items() if os.path.basename(h) in LEAKED.values()
              for n, _ in declarations(t)}
    files = collections.Counter()
    for c in glob.glob(os.path.join(src, "**", "*.c"), recursive=True):
        for n in set(DEF.findall(open(c, encoding="utf-8", errors="replace").read())):
            files[n] += 1
    return leaked | {n for n, k in files.items() if k > 1}


def load(root="include/bash"):
    return {h: open(h, encoding="utf-8").read() for h in sorted(glob.glob(os.path.join(root, "*.h")))}


def main():
    if any(a in ("-h", "--help") for a in sys.argv[1:]):
        print(__doc__)
        return 0
    hs = load()
    bad = repeats(hs)
    for h, line, name, why in bad:
        print("%s:%d: %s: %s" % (h, line, name, why))
    if "--views" in sys.argv:
        for n, where in sorted(views(hs, not_views(hs)).items()):
            print("view: %s in %s" % (n, ", ".join(where)))
    print("%d repeated declarations" % len(bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
