#!/usr/bin/env python3
"""Turn a disc scan's function matches into names for the decomp (docs/PROTOTYPES.md).

Reads protos/<disc>/scan.md (written by proto_scan.py) and lists, for one module, the
retail functions the disc's symbols match, each with the C name its symbol gives (a C++
name without its mangled parameters: mal_free__FPvP9_mal_area -> mal_free):

    tools/dk python3 -I tools/scripts/proto_apply.py protos/<disc>/scan.md
        [--module main|engine] [--min 0.9] [--replace] [--apply]

Matches at --min and up become renames; those from 0.75 up to --min are listed to check
by hand. Left out, each with its reason: PsyQ functions (Sony's names stay), a C++ name
with a class or an operator (no plain C name), a name two retail functions would get
(overloads), a name the build already uses, and a retail function two symbols claim. A
retail function the repo has already named keeps its name unless --replace (the repo's
names are its own guesses; a symbol's is the original). The renames are written to
protos/<disc>/renames_<module>.txt; --apply passes that file to `decomp.py rename
--file`, which rolls everything back if the build stops matching. Without --apply
nothing in the repo changes.
"""
import argparse
import glob
import os
import re
import subprocess
import sys

LIKELY = 0.75
ROW = re.compile(r"^\| `([^`]+)` \| ([^|]*) \| `0x([0-9A-F]{8})` \| ([^|]*) \| ([^|]*) \| ([0-9.]*) \|$")
PLACEHOLDER = re.compile(r"func_[0-9A-F]{8}$")


def rows(scan):
    """(symbol, source, demo address, module, retail name or None, similarity) per
    Functions table row with a match; a bracketed (weak) retail name counts as a match
    with its similarity."""
    out = []
    for line in open(scan):
        m = ROW.match(line.rstrip("\n"))
        if not m or not m.group(5).strip():
            continue
        name = m.group(5).strip().strip("()").strip("`")
        out.append((m.group(1), m.group(2).strip(), int(m.group(3), 16), m.group(4).strip(),
                    name, float(m.group(6) or 0)))
    return out


def c_name(sym):
    """(C name, None) or (None, why not) for a symbol's name."""
    if "__" not in sym:
        return sym, None
    m = re.match(r"([A-Za-z_][A-Za-z0-9_]*?)__F", sym)
    if m and not m.group(1).startswith("_"):
        return m.group(1), None
    return None, "a C++ class member or operator"


def taken_names():
    """Every name the build already uses: the symbol files and the link maps."""
    names = set()
    for p in glob.glob("configs/USA/sym.*.txt"):
        for line in open(p):
            m = re.match(r"\s*([A-Za-z_]\w*)\s*=", line)
            if m:
                names.add(m.group(1))
    for p in glob.glob("build/*.map"):
        for line in open(p):
            m = re.match(r"^\s+0x[0-9a-f]+\s+([A-Za-z_]\w*)$", line)
            if m:
                names.add(m.group(1))
    return names


def aliases(renames):
    """[(old, file:line)]: sources still naming a renamed function's address as data
    (D_<address>). The overlays' generated symbol lists name an address after the symbol
    files only, so once the function has a name that D_ no longer links; the source must
    use the function's name first."""
    want = {old[5:]: old for old, _, _, _ in renames}
    out = []
    for p in glob.glob("src/**/*.c", recursive=True) + glob.glob("include/**/*.h", recursive=True):
        for i, line in enumerate(open(p, errors="replace"), 1):
            for m in re.finditer(r"\bD_([0-9A-F]{8})\b", line):
                if m.group(1) in want:
                    out.append((want[m.group(1)], f"{p.replace(os.sep, '/')}:{i}"))
    return out


def plan(found, module, lo, replace, taken):
    """(renames [(old, new, symbol, similarity)], check [rows], left [(row, why)])."""
    mine = [r for r in found if r[3] == module]
    want, check, left = {}, [], []
    for r in mine:
        sym, _, _, _, retail, sim = r
        if sim < LIKELY:
            continue
        if not PLACEHOLDER.match(retail) and not re.fullmatch(r"[A-Za-z_]\w*", retail):
            left.append((r, "a PsyQ function (Sony's names stay)"))
            continue
        new, why = c_name(sym)
        if new is None:
            left.append((r, why))
            continue
        if new == retail:
            continue
        if not PLACEHOLDER.match(retail) and not replace:
            left.append((r, f"already named `{retail}` (pass --replace to take `{new}`)"))
            continue
        if sim < lo:
            check.append(r)
            continue
        want.setdefault(retail, []).append((new, r))
    renames, by_new = [], {}
    for retail, cands in want.items():
        names = {n for n, _ in cands}
        if len(names) > 1:
            for n, r in cands:
                left.append((r, f"`{retail}` is claimed by {len(names)} symbols"))
            continue
        by_new.setdefault(next(iter(names)), []).append((retail, cands[0][1]))
    for new, targets in by_new.items():
        if len(targets) > 1:
            for retail, r in targets:
                left.append((r, f"`{new}` would name {len(targets)} functions (overloads)"))
            continue
        retail, r = targets[0]
        if new in taken - {retail}:
            left.append((r, f"`{new}` is already a symbol in the build"))
            continue
        renames.append((retail, new, r[0], r[5]))
    return sorted(renames), check, left


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("scan")
    ap.add_argument("--module", default="main", choices=("main", "engine"))
    ap.add_argument("--min", type=float, default=0.9)
    ap.add_argument("--replace", action="store_true", help="also replace names the repo gave itself")
    ap.add_argument("--apply", action="store_true", help="run decomp.py rename on the renames")
    a = ap.parse_args()
    renames, check, left = plan(rows(a.scan), a.module, a.min, a.replace, taken_names())
    out = os.path.join(os.path.dirname(a.scan), f"renames_{a.module}.txt")
    with open(out, "w") as f:
        f.write(f"# from {a.scan} (proto_apply.py): {a.module}, similarity {a.min} and up\n")
        for old, new, sym, sim in renames:
            f.write(f"{old} {new}  # {sym} {sim:.2f}\n")
    print(f"{len(renames)} renames -> {out}")
    for old, new, sym, sim in renames:
        print(f"  {old:16s} -> {new:40s} {sim:.2f}  {sym}")
    if check:
        print(f"\n{len(check)} to check by hand ({LIKELY} to {a.min}):")
        for sym, _, addr, _, retail, sim in sorted(check, key=lambda r: -r[5]):
            print(f"  {retail:16s} <- {c_name(sym)[0]:40s} {sim:.2f}  {sym}")
    if left:
        print(f"\n{len(left)} left out:")
        for (sym, _, _, _, retail, sim), why in left:
            print(f"  {sym} -> {retail} {sim:.2f}: {why}")
    al = aliases(renames)
    if al:
        print(f"\n{len(al)} uses of a renamed function's address as data: write func_<address> there "
              "(and drop its D_ extern) before --apply:")
        for old, where in al:
            print(f"  {old}: {where}")
    sys.stdout.flush()
    if a.apply and renames:
        if al:
            sys.exit("not applied: fix the uses above first")
        sys.exit(subprocess.run(["python3", "tools/scripts/decomp.py", "rename", "--file", out]).returncode)


if __name__ == "__main__":
    main()
