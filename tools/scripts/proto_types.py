#!/usr/bin/env python3
"""Give the functions named from the demo discs the parameter types their C++ names encode.

The demo discs' symbols are g++ 2.x names, which spell each parameter's type
(docs/PROTOTYPES.md): CreateNAS__FiiiUsUs is CreateNAS(int, int, int, unsigned short,
unsigned short), InsertNasTag__FP7NAStypeT0 takes two NAStype pointers. This reads the
renames proto_apply.py wrote (protos/*/renames_*.txt, `OLD NEW  # MANGLED sim`), maps each
parameter to the repo's type, and rewrites the definition and its declarations:

    tools/dk python3 -I tools/scripts/proto_types.py [--apply]

Only types the repo already has are written (s32, u16, ..., NAS *, EXTengine *,
VECTOR *, ...); an unknown class keeps the parameter as it is. Left out, with the reason:
a function an original header declares (its types are the leak's), a count that differs
from the definition's, and names g++ mangles in ways not handled here (classes, arrays,
member pointers). A type change can change how a caller or the function compiles, so
--apply builds the whole batch and checks it; if the build no longer matches, it halves
the batch until each change that breaks it is found and dropped. Without --apply it
only lists.
"""
import argparse
import glob
import os
import re
import subprocess
import sys

BUILTIN = {"v": "void", "c": "char", "s": "s16", "i": "s32", "l": "s32", "x": "s64", "f": "f32",
           "Uc": "u8", "Us": "u16", "Ui": "u32", "Ul": "u32", "Ux": "u64", "Sc": "s8"}
CLASSES = {"NAStype": "NAS", "EXTengtype": "EXTengine", "subengine": "subengine", "SLONG_XYZ": "SLONG_XYZ",
           "VECTOR": "VECTOR", "SVECTOR": "SVECTOR", "MATRIX": "MATRIX", "map_enviroment": "map_enviroment",
           "script_thread_handler": "script_thread_handler", "script_control_info": "script_control_info",
           "script_thread": "script_thread", "geom_info": "geom_info", "Ambient_data": "Ambient_data",
           "NASTrigger": "NASTrigger", "EXTanimator": "EXTanimator", "draw_window": "draw_window"}
# headers recovered from the original source: their prototypes win (CONVENTIONS.md, Headers)
ORIGINAL = ("tank.h", "pogo.h", "psx_pogo.h", "arkenoid.h", "polar.h")


class Unsupported(Exception):
    pass


def demangle(sym):
    """[C type or None (a class the repo lacks)] for the parameters of a g++ 2.x free
    function's name, or [] for none (`__Fv`)."""
    m = re.match(r"[A-Za-z_][A-Za-z0-9_]*?__F(.*)$", sym)
    if not m:
        raise Unsupported("not a free function")
    s, p, out = m.group(1), 0, []

    def one(p):
        stars, mods = 0, ""
        while p < len(s) and s[p] in "PRCVG":
            if s[p] in "PR":
                stars += 1
            p += 1
        if p < len(s) and s[p] in "US" and s[p:p + 2] in BUILTIN:
            mods, p = s[p], p + 1
        if p >= len(s):
            raise Unsupported("truncated")
        ch = s[p]
        if ch.isdigit():
            n = re.match(r"\d+", s[p:]).group()
            p += len(n)
            name = s[p:p + int(n)]
            p += int(n)
            base = CLASSES.get(name)
        elif mods + ch in BUILTIN:
            base, p = BUILTIN[mods + ch], p + 1
        elif ch == "F":
            raise Unsupported("a function pointer parameter")
        else:
            raise Unsupported(f"mangling '{s[p:]}'")
        if base is None:
            return None, p
        if base == "void" and not stars:
            return "void", p
        return base + (" " + "*" * stars if stars else ""), p

    while p < len(s):
        if s[p] == "T":
            m = re.match(r"T(\d)", s[p:])
            out.append(out[int(m.group(1))])
            p += 2
        elif s[p] == "N":
            m = re.match(r"N(\d)(\d)", s[p:])
            out += [out[int(m.group(2))]] * int(m.group(1))
            p += 3
        elif s[p] == "e":
            raise Unsupported("varargs")
        else:
            t, p = one(p)
            out.append(t)
    return [] if out == ["void"] else out


def renamed():
    """[(C name, mangled symbol)] from the rename lists."""
    out = {}
    for p in sorted(glob.glob("protos/*/renames_*.txt")):
        for line in open(p):
            m = re.match(r"^\S+\s+(\w+)\s+#\s+(\S+__F\S*)", line)
            if m:
                out.setdefault(m.group(1), m.group(2))
    return sorted(out.items())


def split_params(text):
    """The parameters of a C parameter list, or None if one has parentheses."""
    text = text.strip()
    if text in ("", "void"):
        return []
    if "(" in text:
        return None
    return [x.strip() for x in text.split(",")]


def param_name(p):
    m = re.search(r"([A-Za-z_]\w*)\s*$", p)
    return m.group(1) if m and not re.fullmatch(r"(void|s8|u8|s16|u16|s32|u32|char|int|short|long|unsigned)",
                                                 m.group(1)) else None


def norm(t):
    return re.sub(r"\s+", " ", re.sub(r"\s*\*\s*", " *", t)).strip()


SCALARS = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "char", "int", "short", "long"}
GENERIC = {"u8 *", "s8 *", "void *", "s16 *", "u16 *", "s32 *", "u32 *", "char *"}


def type_of(p):
    """A parameter's type: the text before its name."""
    n = param_name(p)
    return norm(p[:p.rfind(n)] if n else p)


def better(old, new):
    """Whether the C++ name's type new should replace the C's type old: a pointer to a
    known type over a generic pointer or an integer, `char *` over a byte pointer, and
    one integer type for another (whose code the build then checks). Never `void *`,
    and never over a specific type the repo already gives (EngineBox *, POLY_GT4 *)."""
    if new is None or new == old or norm(new) == "void *":
        return False
    if new.endswith("*"):
        return old in GENERIC or old in SCALARS
    return new in SCALARS and old in SCALARS


def joined(t, n):
    return f"{t}{'' if t.endswith('*') else ' '}{n}"


def plan(names):
    """(changes [(name, [(path, old, new)])], left [(name, why)])."""
    srcs = {p: open(p).read() for p in glob.glob("src/**/*.c", recursive=True)}
    hdrs = {p: open(p).read() for p in glob.glob("include/**/*.h", recursive=True)}
    changes, left = [], []
    for name, sym in names:
        try:
            types = demangle(sym)
        except Unsupported as e:
            left.append((name, str(e)))
            continue
        if any(re.search(rf"\b{name}\s*\(", t) for p, t in hdrs.items() if os.path.basename(p) in ORIGINAL):
            left.append((name, "declared in an original header (its types are the leak's)"))
            continue
        rx = re.compile(rf"^([^\n;{{}}()]*\b){name}\(([^)]*)\)(\s*\{{)", re.M)
        defs = [(p, m) for p, t in srcs.items() for m in rx.finditer(t)]
        if len(defs) != 1:
            left.append((name, f"{len(defs)} definitions"))
            continue
        path, m = defs[0]
        params = split_params(m.group(2))
        empty = srcs[path][m.end():].lstrip().startswith("}")
        if params == [] and types and empty and all(types):
            # an empty function (a stub) takes its parameters without its code changing
            new = [joined(t, f"arg{i}") for i, t in enumerate(types)]
        elif params is None or len(params) != len(types):
            left.append((name, f"{'a function pointer parameter' if params is None else len(params)} "
                         f"parameters in C, {len(types)} in {sym}"))
            continue
        else:
            new = [joined(t, param_name(p)) if param_name(p) and better(type_of(p), t) else p
                   for p, t in zip(params, types)]
        if [norm(x) for x in new] == [norm(x) for x in params]:
            continue
        edits = [(path, m.group(0), f"{m.group(1)}{name}({', '.join(new)}){m.group(3)}")]
        decl = re.compile(rf"\b{name}\s*\(([^)]*)\)\s*;")
        for p, t in hdrs.items():
            for d in decl.finditer(t):
                edits.append((p, d.group(0), f"{name}({', '.join(new)});"))
        for p, t in srcs.items():
            for d in decl.finditer(t):
                edits.append((p, d.group(0), f"{name}({', '.join(new)});"))
        changes.append((name, edits, params, new))
    return changes, left


C_BASE = {"int": "s32", "unsigned int": "u32", "short": "s16", "unsigned short": "u16", "char": "char",
          "unsigned char": "u8", "long": "s32", "unsigned long": "u32", "struct EXTengtype": "EXTengine",
          "struct subengine": "subengine", "SLONG_XYZ": "SLONG_XYZ", "SVECTOR": "SVECTOR", "VECTOR": "VECTOR",
          "SWORD_XY": "SWORD_XY", "SWORD_XYZ": "SWORD_XYZ", "ClockInfo": "ClockInfo", "ScoreInfo": "ScoreInfo"}


def plan_globals(scans):
    """Like plan, for the globals the debug records type (scan.md's Globals table): the
    element type of each renamed global's extern, where better(); its array sizes stay
    the repo's (the demo's can be older: IconParts[4][5] is [4][7] in retail)."""
    typed = {}
    for scan in scans:
        for line in open(scan):
            m = re.match(r"^\| `(\w+)` \| `0x[0-9A-F]+` \| 0x[0-9A-F]+ \| `(.+)` \|$", line.rstrip())
            if m:
                typed.setdefault(m.group(1), m.group(2))
    hdrs = {p: open(p).read() for p in glob.glob("include/**/*.h", recursive=True)}
    changes, left = [], []
    for name, decl in sorted(typed.items()):
        d = re.fullmatch(rf"((?:struct |unsigned )?\w+)\s*(\**){name}((?:\[\d+\])*)", decl)
        base = C_BASE.get(d.group(1)) if d else None
        if base is None:
            left.append((name, f"type `{decl}` not in the repo"))
            continue
        new_t = base + (" " + d.group(2) if d.group(2) else "")
        rx = re.compile(rf"^extern\s+([\w ]+?)\s*(\**)\s*\b{name}\b((?:\s*\[[^\]]*\])*)\s*;", re.M)
        found = [(p, m) for p, t in hdrs.items() for m in rx.finditer(t)]
        olds = {(norm(m.group(1) + (" " + m.group(2) if m.group(2) else "")), m.group(3).replace(" ", ""))
                for _, m in found}
        if len(olds) != 1:
            left.append((name, f"{len(found)} extern declarations" + (", not alike" if found else "")))
            continue
        old_t, dims = next(iter(olds))
        if not better(old_t, norm(new_t)):
            continue
        new = f"extern {joined(norm(new_t), name)}{dims};"
        changes.append((name, [(p, m.group(0), new) for p, m in found], [found[0][1].group(0)], [new]))
    return changes, left


def write(changes, originals):
    """Rewrite the files from their originals with these changes."""
    text = dict(originals)
    for _, edits, _, _ in changes:
        for p, old, new in edits:
            text[p] = text[p].replace(old, new, 1)
    for p, t in text.items():
        with open(p, "w") as f:
            f.write(t)


def builds():
    r = subprocess.run(["make", "build"], capture_output=True, text=True)
    if r.returncode:
        return False
    r = subprocess.run(["make", "check"], capture_output=True, text=True)
    return r.returncode == 0 and "FAIL" not in r.stdout


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--globals", action="store_true", help="the debug records' global types instead")
    a = ap.parse_args()
    changes, left = (plan_globals(sorted(glob.glob("protos/*/scan.md"))) if a.globals else plan(renamed()))
    for name, _, old, new in changes:
        print(f"  {name}({', '.join(old)})\n    -> ({', '.join(new)})")
    print(f"{len(changes)} prototypes to change")
    if left:
        print(f"\n{len(left)} left out:")
        for name, why in left:
            print(f"  {name}: {why}")
    if not a.apply or not changes:
        return
    files = {p for _, edits, _, _ in changes for p, _, _ in edits}
    originals = {p: open(p).read() for p in files}
    good, bad = [], []

    def solve(batch):
        write(good + batch, originals)
        print(f"building with {len(good)} kept + {len(batch)} to try...", flush=True)
        if builds():
            good.extend(batch)
        elif len(batch) == 1:
            bad.append(batch[0][0])
        else:
            solve(batch[:len(batch) // 2])
            solve(batch[len(batch) // 2:])

    solve(changes)
    write(good, originals)
    if not builds():
        write([], originals)
        sys.exit("the kept changes no longer build to a match: everything undone")
    print(f"kept {len(good)}: {' '.join(c[0] for c in good)}")
    if bad:
        print(f"dropped {len(bad)} (the build no longer matched): {' '.join(bad)}")


if __name__ == "__main__":
    main()
