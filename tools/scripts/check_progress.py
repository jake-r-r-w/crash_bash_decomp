#!/usr/bin/env python3
"""Check the dashboard's progress figures against the source tree.

Recounts every game unit straight from src/: a function is done when its
C definition is there and its INCLUDE_ASM line is gone. Prints functions and
bytes matched side by side (they differ: small functions get matched first),
then compares the recount with docs/progress.json and checks its percentages.

    tools/dk python3 tools/scripts/check_progress.py [--worktree]

Reads the committed sources (git HEAD), which is what the dashboard was
generated from; --worktree reads the working tree instead (a draft in place
then counts as done). Exits 1 on any mismatch.
"""
import json
import os
import re
import subprocess
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import decomp  # noqa: E402  (reuses the symbol and unit helpers)

INCLUDE_ASM = re.compile(r'INCLUDE_ASM\(\s*"asm/nonmatchings/([^"]+)"\s*,\s*(\w+)\s*\)')


def pct(a, b):
    return 100.0 * a / b if b else 0.0


def source(path, worktree):
    if worktree:
        return decomp.read(path) if os.path.exists(path) else None
    r = subprocess.run(["git", "show", f"HEAD:{path}"], capture_output=True, text=True)
    return r.stdout if r.returncode == 0 else None


def main():
    worktree = "--worktree" in sys.argv[1:]
    proj, funcs, syms = decomp.need_proj(), decomp.need_funcs(), decomp.sym_names()
    texts, inc = {}, defaultdict(set)
    for k, e in funcs.items():
        src = e.get("src")
        if src and src not in texts:
            text = source(src, worktree)
            if text is None:
                continue
            texts[src] = text
            for seg, name in INCLUDE_ASM.findall(text):
                inc[src].add(name)

    units = defaultdict(lambda: {"done_f": 0, "total_f": 0, "done_b": 0, "total_b": 0})
    problems = []
    for k, e in funcs.items():
        if decomp.is_lib(e, proj) or e.get("kind") in ("asm", "sony") or not e.get("src"):
            continue
        src = e["src"]
        if src not in texts:
            problems.append(f"{k}: source {src} is missing")
            continue
        name = decomp.cur_name(k, e, syms)
        names = {name, e["name"], e["orig"]}
        u = units[decomp.unit_of(e, proj)]
        u["total_f"] += 1
        u["total_b"] += e["size"]
        if names & inc[src]:
            continue
        if not any(decomp.func_def_regex(nm).search(texts[src]) for nm in names):
            problems.append(f"{name} ({src}): no INCLUDE_ASM and no C definition")
            continue
        u["done_f"] += 1
        u["done_b"] += e["size"]

    print("sources:", "working tree" if worktree else "git HEAD")
    print(f"{'unit':<10} {'functions':>13} {'fn %':>7}   {'bytes':>17} {'byte %':>7}")
    tot = defaultdict(int)
    for un in sorted(units):
        u = units[un]
        for f in u:
            tot[f] += u[f]
        print(f"{un:<10} {u['done_f']:>5} / {u['total_f']:<5} {pct(u['done_f'], u['total_f']):>6.2f}%   "
              f"{u['done_b']:>7} / {u['total_b']:<7} {pct(u['done_b'], u['total_b']):>6.2f}%")
    print(f"{'game':<10} {tot['done_f']:>5} / {tot['total_f']:<5} {pct(tot['done_f'], tot['total_f']):>6.2f}%   "
          f"{tot['done_b']:>7} / {tot['total_b']:<7} {pct(tot['done_b'], tot['total_b']):>6.2f}%")

    # the dashboard's own numbers: same counts, and percentages that follow from them
    data = json.load(open("docs/progress.json"))
    s = data.get("summary", data)
    shown = {u["unit"]: u for u in s.get("units", []) if not u.get("lib")}
    for un, u in units.items():
        d = shown.get(un)
        if d is None:
            problems.append(f"{un}: not on the dashboard")
            continue
        for f in ("done_f", "total_f", "done_b", "total_b"):
            if d[f] != u[f]:
                problems.append(f"{un}: dashboard {f} = {d[f]}, source says {u[f]}")
    for d in list(shown.values()) + [s["game"]]:
        want = pct(d["done_b"], d["total_b"])
        if abs(d["pct"] - want) > 1e-6:
            problems.append(f"{d.get('unit', 'game')}: dashboard pct {d['pct']:.4f} != bytes {want:.4f}")
    for f in ("done_f", "total_f", "done_b", "total_b"):
        if s["game"][f] != tot[f]:
            problems.append(f"game: dashboard {f} = {s['game'][f]}, source says {tot[f]}")

    if problems:
        print("\nMISMATCHES (run `decomp.py progress` after fixing; stale until then):")
        for p in problems:
            print("  " + p)
        return 1
    print("\nOK: the dashboard matches the source tree (byte % is the headline; fn % shown alongside).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
