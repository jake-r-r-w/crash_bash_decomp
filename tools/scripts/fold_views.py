#!/usr/bin/env python3
"""Fold view structs into the one struct they are views of, by offset.

    tools/dk python3 tools/scripts/fold_views.py [--dry] CANON VIEW...
    tools/dk python3 tools/scripts/fold_views.py --rename OLD=NEW ...

Each VIEW's members are mapped onto CANON's by offset and width (layouts from cc1, see
struct_layout.py; each view compiled with the header that defines it). Then, in rounds of views
whose renames don't conflict, the view's name becomes CANON everywhere, its typedef goes, and
every `no member named OLD` error is fixed on its line by the round's mapping (one parallel
syntax compile of every C file per pass, until none are left). Views whose names clash first go
through temporary names, so nothing compiles silently against the wrong field:

  - a view member named like a CANON member at a different offset (`buttons` at 0x50 when
    CANON's `buttons` is at 4) is renamed `t_NAME` in the view first;
  - a view member with no CANON member of its width at its offset (a halfword read of a word)
    becomes `t_NAME` and is left for a hand fix: the `left:` lines name each use.

--dry prints the mapping, the traps and the members that don't map, and changes nothing.
--rename runs the error-driven rename passes alone (OLD=NEW pairs), e.g. after a manual swap.
Bytes don't change; check with `make check`. Sign differences are mapped (a load's sign can
change the code): fix those in CANON's member types.
"""
import argparse
import collections
import concurrent.futures
import glob
import os
import re
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import field_rename as fr  # noqa: E402
import struct_layout as sl  # noqa: E402


def rd(p):
    return open(p, encoding="utf-8").read()


def wr(p, t):
    for _ in range(20):  # Windows can hold a file open for a moment
        try:
            open(p, "w", encoding="utf-8", newline="").write(t)
            return
        except PermissionError:
            time.sleep(0.5)
    raise SystemExit("can't write " + p)


def members(ty):
    hdr = sl.header_of(ty)
    structs, typedefs = sl.compile_layouts([ty], sl.DEFAULT_HEADERS + ([hdr] if hdr else []))
    return structs.get(sl.tag_of(ty, typedefs)), structs, typedefs


def map_view(view_members, canon_leaves, canon_names):
    """(renames {old: new}, traps [names], unmapped [(name, off, width)]) for one view."""
    renames, traps, unmapped = {}, [], []
    for name, off, size, t, dims, tag in view_members:
        if name.startswith("pad"):
            continue
        w, sg, kind = sl.width(t, size, dims)
        cs = [c for c in canon_leaves.get(off, []) if c[1] == w and c[3] != "struct"] or \
             [c for c in canon_leaves.get(off, []) if c[1] == w]
        if not cs and any(c[0] == name for c in canon_leaves.get(off, [])):
            continue  # the same member (a union or struct of that name)
        if not cs:
            unmapped.append((name, off, w))
            if name in canon_names:
                traps.append(name)
                renames[name] = "t_" + name
            continue
        same = [c for c in canon_leaves.get(off, []) if c[0] == name]
        path = same[0][0] if same else cs[0][0]
        if dims and path.endswith("[0]"):
            path = path[:-3]
        if path != name:
            renames[name] = path
            if name in canon_names:
                traps.append(name)
    return renames, traps, unmapped


def rounds_of(maps):
    """views grouped so no OLD name maps to two NEWs within a round."""
    rounds = []
    for v, m in maps.items():
        for r in rounds:
            if all(r["map"].get(o, n) == n for o, n in m.items()):
                r["views"].append(v)
                r["map"].update(m)
                break
        else:
            rounds.append({"views": [v], "map": dict(m)})
    return rounds


def drop_typedef(v, headers):
    for p in headers:
        L = rd(p).split("\n")
        o = list(L)
        while "} %s;" % v in L:
            e = L.index("} %s;" % v)
            s = e
            while not L[s].startswith("typedef struct"):
                s -= 1
            if s > 0 and L[s - 1].startswith("/*") and L[s - 1].endswith("*/"):
                s -= 1
            del L[s:e + 1]
        L = [x for x in L if not re.match(r"typedef struct \{.*\} %s;$" % v, x)]
        if L != o:
            wr(p, "\n".join(L))


def errors(path):
    cpp = subprocess.run(fr.CPP + [path], capture_output=True, text=True)
    cc1 = subprocess.run(fr.CC1, input=cpp.stdout, capture_output=True, text=True)
    return [(m.group(1), int(m.group(2)), m.group(3)) for m in (fr.ERR.match(l) for l in cc1.stderr.splitlines()) if m]


def rename_pass(mp, srcs, passes=8):
    """fix every missing-member error whose name is in mp; returns the other errors."""
    miss = set()
    for it in range(passes):
        with concurrent.futures.ThreadPoolExecutor(8) as ex:
            errs = sorted(set(e for es in ex.map(errors, srcs) for e in es))
        todo = collections.defaultdict(set)
        miss = set()
        for f, ln, mem in errs:
            (todo[f].add((ln, mem)) if mem in mp else miss.add((f, ln, mem)))
        if not todo:
            break
        for f, items in todo.items():
            L = rd(f).split("\n")
            for ln, mem in items:
                L[ln - 1] = re.sub(r"(->|\.)%s\b" % mem, lambda q: q.group(1) + mp[mem], L[ln - 1])
            wr(f, "\n".join(L))
        print("  pass %d: %d lines" % (it, sum(len(x) for x in todo.values())), flush=True)
    return sorted(miss)


def rename_in_view(v, names, headers):
    """rename members of view v's typedef (to t_NAME), then their uses."""
    for h in headers:
        t = rd(h)
        if "} %s;" % v not in t:
            continue
        e = t.index("} %s;" % v)
        s = t.rindex("typedef struct", 0, e)
        body = t[s:e]
        for n in names:
            body = re.sub(r"\b%s\b(?=\s*(\[[^\]]*\])?\s*;)" % n, "t_" + n, body)
        wr(h, t[:s] + body + t[e:])
    return {n: "t_" + n for n in names}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--rename", action="store_true", help="only the rename passes, for OLD=NEW pairs")
    ap.add_argument("names", nargs="+")
    a = ap.parse_args()
    srcs = glob.glob("src/**/*.c", recursive=True)
    headers = glob.glob("include/bash/*.h")
    if a.rename:
        for e in rename_pass(dict(x.split("=", 1) for x in a.names), srcs):
            print("  left: %s:%d %s" % e)
        return 0
    canon, views = a.names[0], a.names[1:]
    cm, structs, typedefs = members(canon)
    if cm is None:
        raise SystemExit("no layout for " + canon)
    cl = sl.leaves(canon, structs, typedefs)
    names = set(m[0] for m in cm)
    maps, traps = {}, {}
    for v in views:
        vm = members(v)[0]
        if vm is None:
            print("?? no layout for", v)
            continue
        m, tr, un = map_view(vm, cl, names)
        maps[v] = m
        if tr:
            traps[v] = tr
        print("%s: %s" % (v, m))
        for n, off, w in un:
            print("   %s.%s +0x%X (%d): no %s member of that width%s" % (v, n, off, w, canon, " (TRAP: same name)" if n in names else ""))
    if traps:
        print("traps (renamed t_NAME first):", traps)
    if a.dry:
        return 0
    for v, tr in traps.items():
        tmp = rename_in_view(v, tr, headers)
        rename_pass(tmp, srcs)
        for n in tr:
            maps[v]["t_" + n] = maps[v].pop(n)
            if maps[v]["t_" + n] == "t_" + n:
                del maps[v]["t_" + n]  # left for a hand fix
    for r in rounds_of(maps):
        print("round:", " ".join(r["views"]), flush=True)
        for v in r["views"]:
            drop_typedef(v, headers)
        for p in srcs + headers:
            t = rd(p)
            n = t
            for v in r["views"]:
                n = re.sub(r"\b%s\b" % v, canon, n)
            if n != t:
                wr(p, n)
        for e in rename_pass(r["map"], srcs)[:40]:
            print("  left: %s:%d %s" % e)
    return 0


if __name__ == "__main__":
    sys.exit(main())
