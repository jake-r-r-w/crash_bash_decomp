#!/usr/bin/env python3
"""Struct layouts as cc1 sees them: every member's offset, width, signedness and type, read
from the compiler's own -g (SDB) output, so they never disagree with the build.

    tools/dk python3 tools/scripts/struct_layout.py TYPE... [--header bash/x.h ...] [--leaves]

TYPE is a typedef name or `struct TAG`. Without --leaves it prints each member (offset, size,
name); with --leaves the flattened view retype.py and fold_views.py work from: one line per
offset reached through nested structs and arrays (`a.b[2].c`). The headers default to
bash/main.h and bash/engine.h; a type defined elsewhere needs its header with --header.
"""
import argparse
import json
import os
import re
import subprocess
import sys

CPP = ["mipsel-linux-gnu-cpp", "-nostdinc", "-undef", "-Iinclude", "-Iext/psyq47/include", "-D__GNUC__=2",
       "-D_LANGUAGE_C", "-DPSX"]
CC1 = ["/opt/gcc/2.8.1-psx/cc1", "-quiet", "-O0", "-G0", "-g", "-o", "-"]
DEFAULT_HEADERS = ["bash/main.h", "bash/engine.h"]
# SDB basic type codes (the low 4 bits of .type): width, signed
BASE = {2: (1, True), 0xc: (1, False), 3: (2, True), 0xd: (2, False), 4: (4, True), 0xe: (4, False),
        5: (4, True), 0xf: (4, False)}
CTYPE = {"u8": (1, False), "s8": (1, True), "char": (1, True), "u16": (2, False), "s16": (2, True),
         "s32": (4, True), "u32": (4, False)}
DEF = re.compile(r"\.def\s+([^;]+);(.*?)\.endef")


def parse_sdb(asm):
    """{struct tag: [(name, off, size, type, dims, tag)]} and {typedef: tag} from cc1 -g output."""
    structs, typedefs, cur = {}, {}, None
    for line in asm.splitlines():
        m = DEF.search(line)
        if not m:
            continue
        name, rest = m.group(1).strip(), m.group(2)
        kv = dict((a.strip().split(None, 1) + [""])[:2] for a in rest.split(";") if a.strip())
        scl = kv.get(".scl", "")
        if scl in ("10", "12") and kv.get(".type") in ("0x8", "0x9"):
            cur = name
            structs[cur] = []
        elif name == ".eos":
            cur = None
        elif cur is not None and scl in ("8", "11"):
            structs[cur].append((name, int(kv.get(".val", "0")), int(kv.get(".size", "0") or 0),
                                 kv.get(".type", ""), kv.get(".dim", ""), kv.get(".tag", "")))
        elif scl == "13":
            typedefs[name] = kv.get(".tag", name)
    return structs, typedefs


def compile_layouts(types, headers=None):
    """(structs, typedefs) for a file including headers and naming each of types."""
    headers = headers or DEFAULT_HEADERS
    src = '#include "common.h"\n' + "".join('#include "%s"\n' % h for h in headers)
    src += "".join("%s *struct_layout_dummy_%d;\n" % (t, n) for n, t in enumerate(types))
    cpp = subprocess.run(CPP + ["-"], input=src, capture_output=True, text=True)
    asm = subprocess.run(CC1, input=cpp.stdout, capture_output=True, text=True).stdout
    return parse_sdb(asm)


def tag_of(ty, typedefs):
    return ty[7:] if ty.startswith("struct ") else typedefs.get(ty, ty)


def width(t, size, dims):
    """element width and signedness of a member (pointers 4, structs their size)."""
    code = int(t, 16) if t else 0
    if (code >> 4) & 3 in (1, 2):
        return 4, False, "ptr"
    if code & 0xF in (8, 9):
        n = count(dims)
        return (size // n if n else size), False, "struct"
    w, s = BASE.get(code & 0xF, (0, False))
    if dims:
        w = size // count(dims)
    return w, s, ""


def count(dims):
    n = 1
    for d in dims.split(",") if dims else []:
        n *= int(d)
    return n


def unravel(k, ds):
    out = []
    for d in reversed(ds):
        out.append(k % d)
        k //= d
    return list(reversed(out))


def leaves(ty, structs, typedefs):
    """{offset: [(path, width, signed, kind)]}: TYPE's members flattened through nested structs and
    arrays; kind is "ptr", "" for a number, or "struct" for an aggregate with no layout here."""
    res = {}

    def flat(fields, base, prefix):
        for name, off, size, t, dims, tag in fields or []:
            w, sg, kind = width(t, size, dims)
            ds = [int(x) for x in dims.split(",")] if dims else []
            n = count(dims)
            if kind == "struct" and tag in structs and w == 4 and max(
                    (f[1] + f[2] for f in structs[tag]), default=0) > 4:
                kind = "ptr"    # a pointer typedef that shares a struct's name (EXTengtype)
            if kind == "struct" and tag in structs:
                for k in range(n if ds else 1):
                    idx = "".join("[%d]" % v for v in unravel(k, ds)) if ds else ""
                    flat(structs[tag], base + off + k * w, "%s%s%s." % (prefix, name, idx))
                res.setdefault(base + off, []).append((prefix + name, size, False, "struct"))
                continue
            if ds and kind != "ptr":
                for k in range(n):
                    idx = "".join("[%d]" % v for v in unravel(k, ds))
                    res.setdefault(base + off + k * w, []).append((prefix + name + idx, w, sg, kind))
            else:
                res.setdefault(base + off, []).append((prefix + name, w, sg, kind))
    flat(structs.get(tag_of(ty, typedefs)), 0, "")
    return res


def header_of(ty, root="include"):
    """the bash/*.h header that defines typedef TYPE, or None."""
    for f in sorted(os.listdir(os.path.join(root, "bash"))):
        if not f.endswith(".h"):
            continue
        t = open(os.path.join(root, "bash", f), encoding="utf-8").read()
        e = re.escape(ty)
        if (re.search(r"^\} %s;" % e, t, re.M) or re.search(r"typedef struct \{[^}]*\} %s;" % e, t)
                or re.search(r"^typedef struct (\w+) %s;" % e, t, re.M)
                and re.search(r"^struct %s \{" % re.search(r"^typedef struct (\w+) %s;" % e, t, re.M).group(1), t, re.M)):
            return "bash/" + f
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("types", nargs="+")
    ap.add_argument("--header", action="append", default=[])
    ap.add_argument("--leaves", action="store_true")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    hdrs = DEFAULT_HEADERS + a.header + [h for h in (header_of(t) for t in a.types) if h]
    structs, typedefs = compile_layouts(a.types, hdrs)
    out = {}
    for t in a.types:
        if a.leaves:
            out[t] = {"0x%X" % k: v for k, v in sorted(leaves(t, structs, typedefs).items())}
        else:
            out[t] = structs.get(tag_of(t, typedefs))
    if a.json:
        print(json.dumps(out))
        return 0
    for t, v in out.items():
        print("== %s" % t)
        if v is None:
            print("  (no layout: is its header included?)")
        elif a.leaves:
            for off, cs in v.items():
                print("  +%-6s %s" % (off, "  ".join("%s (%d%s)" % (p, w, "s" if s else "u") for p, w, s, k in cs)))
        else:
            for name, off, size, ty, dims, tag in v:
                print("  +0x%03X %-20s %s%s" % (off, name, tag or ty, "[%s]" % dims if dims else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
