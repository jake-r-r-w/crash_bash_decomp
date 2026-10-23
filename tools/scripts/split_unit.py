#!/usr/bin/env python3
"""Split a C unit in two at a function: the function and everything after it go to a new unit.

    tools/dk python3 tools/scripts/split_unit.py SRC FUNC NEW --text-off OFF [--rodata-off OFF]
        --yaml configs/USA/<file>.yaml [--umbrella include/bash/<header>.h]

SRC is the C file (src/<seg>.c), FUNC the first function of the new unit, NEW its name (the
new file is src/<dir of seg>/NEW.c with the owned header bash/NEW.h); OFF are the yaml's offsets
(as in its other subsegment lines) of FUNC's code and of the new unit's first rodata item. The
rodata split must be exact: the byte-identical build proves it (CONVENTIONS.md: each split is
a yaml subsegment change for .text and .rodata, then make split, index and verify).

What moves with the code: the prototypes (and the comment right above each) of the moved
functions, from the old unit's owned header to the new one, and the old header's `static
inline` helpers that only the new unit uses. What is copied: the old file's file-scope helpers
(`static inline` functions, macros, externs) defined before the cut and used after it. The
umbrella header (the one the others include: bash/engine.h for the engine units, else the old
owned header) includes the new header after the old one. Then the asm is regenerated (splat
writes no .s for matched C, so the C files are set aside for one `make split`), the old asm
folder loses the moved functions' .s, and the index, build and check run. Nothing is
committed; on a failure the tree is left as it is for a look (git restores it).
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import decomp  # noqa: E402


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    return r.returncode, r.stdout + r.stderr


def blocks(text):
    """File-scope definitions: [(start, end, kind, name)] for static inline functions (to the
    } at column 0), #define macros (with their continuation lines) and one-line externs."""
    out = []
    for m in re.finditer(r"^static inline [^\n]*?\b(\w+)\s*\([^\n]*\{\n.*?^\}\n", text, re.M | re.S):
        out.append((m.start(), m.end(), "inline", m.group(1)))
    for m in re.finditer(r"^#define\s+(\w+)(?:[^\n]*\\\n)*[^\n]*\n", text, re.M):
        # a macro #undef'd again (a temporary one around a few functions) is gone by the cut
        if not re.search(rf"^#undef\s+{m.group(1)}\b", text[m.end():], re.M):
            out.append((m.start(), m.end(), "define", m.group(1)))
    for m in re.finditer(r"^extern [^\n;(]*?\b(\w+)\s*(?:\([^\n]*\))?(?:\[[^\n]*\])?\s*;[^\n]*\n", text, re.M):
        out.append((m.start(), m.end(), "extern", m.group(1)))
    return sorted(out)


def used(name, text):
    return re.search(rf"\b{re.escape(name)}\b", text) is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("src")
    ap.add_argument("func")
    ap.add_argument("new")
    ap.add_argument("--text-off", required=True)
    ap.add_argument("--rodata-off", help="the new unit's first rodata item; leave out when it has none")
    ap.add_argument("--yaml", required=True)
    ap.add_argument("--umbrella")
    a = ap.parse_args()

    seg = os.path.splitext(os.path.relpath(a.src, "src"))[0].replace(os.sep, "/")
    newseg = f"{os.path.dirname(seg)}/{a.new}"
    old_name = os.path.basename(seg)
    new_c = f"src/{newseg}.c"
    old_h = next(p for p in glob.glob("include/bash/*.h") if f"owner: {a.src}" in open(p).read())
    new_h = f"include/bash/{a.new}.h"
    umbrella = a.umbrella or old_h

    src = open(a.src).read()
    m = re.search(rf"^[A-Za-z_][^\n;{{}}()]*\b{a.func}\(", src, re.M)
    if not m:
        sys.exit(f"no definition of {a.func} in {a.src}")
    cut = m.start()
    # the comment block right above the function goes with it
    above = re.search(r"(?:/\*(?:[^*]|\*(?!/))*\*/\n)+\Z", src[:cut])
    if above:
        cut = above.start()
    first, rest = src[:cut], src[cut:]
    includes = re.match(r"(?:#include[^\n]*\n|\n)*", src).group(0)
    old_inc = f'#include "bash/{os.path.basename(old_h)}"'
    new_inc = f'#include "bash/{a.new}.h"'
    head = includes.rstrip("\n") + "\n" + (new_inc + "\n" if old_inc in includes else "")
    if old_inc in includes and old_h != umbrella:
        head = head.replace(old_inc + "\n", "")
    if new_inc not in head:
        head += new_inc + "\n"
    defs = [(s, e, name) for s, e, kind, name in blocks(first) if s >= len(includes)]
    need, seen = rest, set()
    while True:   # helpers the moved code uses, and the helpers those use
        more = {(s, e) for s, e, name in defs if (s, e) not in seen and used(name, need)}
        if not more:
            break
        seen |= more
        need += "".join(first[s:e] for s, e in more)
    copied = [first[s:e] for s, e, name in defs if (s, e) in seen]
    rest = rest.replace(f"asm/nonmatchings/{seg}\"", f"asm/nonmatchings/{newseg}\"")
    moved = decomp.defined_names(rest)   # K&R definitions too
    open(a.src, "w").write(first.rstrip("\n") + "\n")
    open(new_c, "w").write(head + "\n" + "".join(c if c.endswith("\n") else c + "\n" for c in copied)
                           + ("\n" if copied else "") + rest)

    # the moved functions' prototypes, and the header inlines only the new unit uses
    hdr = open(old_h).read()
    others = "".join(open(p).read() for p in glob.glob("src/**/*.c", recursive=True) if p != new_c)
    take, keep = [], hdr
    for s, e, kind, name in reversed(blocks(hdr)):
        if kind == "inline" and used(name, rest) and not used(name, others):
            take.insert(0, hdr[s:e])
            keep = keep[:s] + keep[e:]
    protos = []
    for pm in reversed(list(re.finditer(r"(?:(?:/\*(?:[^*]|\*(?!/))*\*/\n))?^[A-Za-z_][^\n;()]*?\b(\w+)\s*\([^;{]*?\)\s*;[^\n]*\n",
                                        keep, re.M))):
        if pm.group(1) in moved:
            protos.insert(0, pm.group(0))
            keep = keep[:pm.start()] + keep[pm.end():]
    guard = f"BASH_{a.new.upper()}_H"
    old_inc_line = f'#include "bash/{os.path.basename(umbrella)}"'
    text = (f"#ifndef {guard}\n#define {guard}\n\n"
            f"/* Functions and data defined in {new_c}. Anything the original headers declare\n"
            f" * (bash/pogo.h, ...) is declared there instead, not here.\n * owner: {new_c} */\n\n"
            f'#include "common.h"\n{old_inc_line}\n\n' + "".join(protos) + ("\n" if take else "")
            + "\n".join(t.rstrip("\n") for t in take) + f"\n\n#endif /* {guard} */\n")
    open(new_h, "w").write(text)
    if umbrella == old_h:
        keep = re.sub(r"\n(#endif[^\n]*\n?)\Z", f"\n{new_inc}\n\n\\1", keep.rstrip("\n") + "\n")
        open(old_h, "w").write(keep)
    else:
        open(old_h, "w").write(keep)
        u = open(umbrella).read()
        u = u.replace(old_inc + "\n", old_inc + "\n" + new_inc + "\n", 1)
        open(umbrella, "w").write(u)

    y = open(a.yaml).read()
    y, n1 = re.subn(rf"(\n(\s*)- \[0x[0-9A-Fa-f]+, c, {re.escape(seg)}\][^\n]*)",
                    rf"\1\n\2- [{a.text_off}, c, {newseg}]", y, count=1)
    n2 = 0
    if a.rodata_off:   # a unit whose code uses no rodata gets no .rodata subsegment
        y, n2 = re.subn(rf"(\n(\s*)- \[0x[0-9A-Fa-f]+, \.rodata, {re.escape(seg)}\][^\n]*)",
                        rf"\1\n\2- [{a.rodata_off}, .rodata, {newseg}]", y, count=1)
    if n1 != 1 or (a.rodata_off and n2 != 1):
        sys.exit(f"{a.yaml}: no c and .rodata lines for {seg}")
    open(a.yaml, "w").write(y)
    print(f"{a.src}: {len(moved)} functions to {new_c}; {len(protos)} prototypes and {len(take)} "
          f"inlines to {new_h}; {len(copied)} file-scope helpers copied")

    # the asm: splat writes .s only for functions no C file defines, so set both aside once
    keep_dir = "build/split_unit_keep"
    os.makedirs(keep_dir, exist_ok=True)
    for p in (a.src, new_c):
        shutil.move(p, os.path.join(keep_dir, os.path.basename(p)))
    rc, out = run(["make", "split"])
    for p in (a.src, new_c):
        shutil.copy(os.path.join(keep_dir, os.path.basename(p)), p)
    if rc:
        sys.exit("make split failed:\n" + out[-2000:])
    for f in moved:
        stale = f"asm/nonmatchings/{seg}/{f}.s"
        if os.path.exists(stale) and os.path.exists(f"asm/nonmatchings/{newseg}/{f}.s"):
            os.remove(stale)
    for cmd in (["python3", "tools/scripts/decomp.py", "index"], ["make", "build"], ["make", "check"]):
        rc, out = run(cmd)
        if rc or (cmd[-1] == "check" and "FAIL" in out):
            sys.exit(f"{' '.join(cmd)} failed:\n" + out[-3000:])
    print("split, indexed, built: make check is byte-identical")


if __name__ == "__main__":
    main()
