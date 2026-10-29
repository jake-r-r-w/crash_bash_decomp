#!/usr/bin/env python3
"""Give a function's byte-pointer variable its real type, by the compiler's layout.

    tools/dk python3 tools/scripts/retype.py FILE FUNC VAR TYPE[:MEMBER] [VAR TYPE ...]
    tools/dk python3 tools/scripts/retype.py --revert FILE FUNC

VAR (a parameter or local of FUNC declared `u8 *`, `s32 *`, `void *`, `s16 *` or `u16 *`)
becomes a TYPE *. Its raw accesses `*(T *)(VAR + N)`, `*(T *)VAR` and `VAR[N / 4]` become
`VAR->field` where TYPE has a field of T's width at offset N (struct_layout.py's flattened
view, so nested members like `type.particle.zrot` are found); a narrower read of a wider field
becomes `*(T *)&VAR->field`. `:MEMBER` prefers that union member's names (`script_thread:
type.fade`). An assignment of byte arithmetic to VAR gets a `(TYPE *)` cast, and the
function's prototype in include/bash/*.h follows its new signature.

It prints each access it couldn't map, and every line still doing `VAR + N`: after the change
that arithmetic is scaled by sizeof(TYPE), so each LEFT line needs a hand fix (usually a field's
address, `&VAR->f`). Check the result with `make diff FUNC=...`; the old body is kept in
build/retype.json for --revert.
"""
import argparse
import glob
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import struct_layout as sl  # noqa: E402

UNDO = "build/retype.json"
DECL_TYPES = "u8|s8|s32|u32|void|s16|u16"


def pick(cands, width, prefer=None):
    """the path of a number (or else pointer) field of that width at an offset."""
    cs = list(cands or [])
    if prefer:
        cs.sort(key=lambda c: not c[0].startswith(prefer))
    for kind_ok in (("",), ("", "ptr")):
        for p, w, s, kind in cs:
            if w == width and kind in kind_ok:
                return p
    return None


def rewrite(body, var, ty, lv, prefer=None, log=print, ptrs=None):
    """body with VAR's raw accesses rewritten as fields of TYPE (lv: struct_layout.leaves). The
    pointer fields a raw word read became are added to PTRS: arithmetic on them is scaled now."""
    def field(cast, ptr, off, whole):
        if ptrs is not None and not ptr:
            p = pick(lv.get(off), sl.CTYPE[cast][0], prefer) if cast in sl.CTYPE else None
            if p and any(c[0] == p and c[3] == "ptr" for c in lv.get(off) or []):
                ptrs.add(p)
        if ptr:
            path = pick(lv.get(off), 4, prefer)
        elif cast in sl.CTYPE:
            path = pick(lv.get(off), sl.CTYPE[cast][0], prefer)
        else:
            cs = [c for c in (lv.get(off) or []) if c[3] != "struct"] or (lv.get(off) or [])
            path = cs[0][0] if cs else None
        if path:
            return "%s->%s" % (var, path)
        cs = [c for c in (lv.get(off) or []) if c[3] != "struct"]
        if cs and cast in sl.CTYPE:
            return "*(%s *)&%s->%s" % (cast, var, cs[0][0])
        log("  unmapped %s+0x%X as %s%s" % (var, off, cast, " *" if ptr else ""))
        return whole

    body = re.sub(r"\*\(\s*(\w+)\s*(\*+)\)\s*\(\s*(?:\(u8 ?\*\)\s*)?\(?%s\)?\s*\+\s*(0x[0-9A-Fa-f]+|\d+)\s*\)" % var,
                  lambda q: field(q.group(1), len(q.group(2)) > 1, int(q.group(3), 0), q.group(0)), body)
    body = re.sub(r"\*\(\s*(\w+)\s*(\*+)\)\s*%s\b(?!\s*[+\[])" % var,
                  lambda q: field(q.group(1), len(q.group(2)) > 1, 0, q.group(0)), body)
    body = re.sub(r"\b%s\[(0x[0-9A-Fa-f]+) / 4\]" % var,
                  lambda q: "%s->%s" % (var, pick(lv.get(int(q.group(1), 0)), 4, prefer)) if pick(lv.get(int(q.group(1), 0)), 4, prefer) else q.group(0), body)
    body = re.sub(r"\b((?:%s) \*)%s\b(?=[;,)= ])" % (DECL_TYPES, var), "%s *%s" % (ty, var), body, count=1)
    body = re.sub(r"(\b%s = )(?!NULL|\(%s \*\))([^;]*[+][^;]*);" % (var, re.escape(ty)),
                  lambda q: "%s(%s *)(%s);" % (q.group(1), ty, q.group(2)), body)
    return body


def leftovers(body, var, ptrs=()):
    """lines still adding to VAR (scaled by the new type now) or reading it as bytes, and lines
    adding to a pointer field that was a raw word (PTRS)."""
    pf = "|".join(re.escape(p) for p in ptrs)
    return [l.strip() for l in body.split("\n")
            if re.search(r"\b%s\)?\s*\+|\+ %s\b|\b%s\[|\(u8 ?\*\)\s*\(?%s\b" % (var, var, var, var), l)
            or pf and re.search(r"\b%s->(?:%s)\)?\s*[+-]" % (var, pf), l)]


def func_span(t, fn):
    m = re.search(r"^[A-Za-z_][^;{}\n]*?\b%s\([^;{}]*\)\s*\{" % fn, t, re.M)
    if not m:
        raise SystemExit("no function %s" % fn)
    return m.start(), t.index("\n}\n", m.end()) + 2


def signature(body):
    return re.match(r"([^{]*?)\s*\{", body).group(1).strip()


def set_prototype(fn, sig, headers):
    for h in headers:
        ht = open(h, encoding="utf-8").read()
        nt = re.sub(r"^[A-Za-z_][^;{}\n]*?\b%s\([^;{}]*\);" % fn, lambda q: sig + ";", ht, flags=re.M)
        if nt != ht:
            open(h, "w", encoding="utf-8", newline="").write(nt)
            print("  prototype in", h)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--revert", action="store_true")
    ap.add_argument("--header", action="append", default=[], help="a header TYPE needs (besides main.h, engine.h)")
    ap.add_argument("file")
    ap.add_argument("func")
    ap.add_argument("pairs", nargs="*", help="VAR TYPE[:MEMBER] ...")
    a = ap.parse_args()
    headers = glob.glob("include/bash/*.h")
    undo = json.load(open(UNDO)) if os.path.exists(UNDO) else {}
    key = a.file + ":" + a.func
    t = open(a.file, encoding="utf-8").read()
    if a.revert:
        old, new = undo.pop(key)
        if new not in t:
            raise SystemExit("%s changed since it was retyped; revert by hand" % a.func)
        open(a.file, "w", encoding="utf-8", newline="").write(t.replace(new, old, 1))
        set_prototype(a.func, signature(old), headers)
        json.dump(undo, open(UNDO, "w"))
        print("reverted", a.func)
        return 0
    if len(a.pairs) % 2:
        ap.error("VAR TYPE pairs")
    s, e = func_span(t, a.func)
    body = old = t[s:e]
    pairs = list(zip(a.pairs[::2], a.pairs[1::2]))
    ptrs = {}
    for var, ty in pairs:
        prefer = None
        if ":" in ty:
            ty, prefer = ty.split(":", 1)
        hdrs = sl.DEFAULT_HEADERS + a.header + [h for h in [sl.header_of(ty)] if h]
        structs, typedefs = sl.compile_layouts([ty], hdrs)
        lv = sl.leaves(ty, structs, typedefs)
        if not lv:
            raise SystemExit("no layout for %s (pass its header with --header)" % ty)
        ptrs[var] = set()
        body = rewrite(body, var, ty, lv, prefer, ptrs=ptrs[var])
    if body == old:
        print("no change")
        return 1
    for var, ty in pairs:
        for l in leftovers(body, var, ptrs.get(var, ())):
            print("  LEFT (now scaled by the type):", l[:110])
    undo[key] = [old, body]
    os.makedirs("build", exist_ok=True)
    json.dump(undo, open(UNDO, "w"))
    open(a.file, "w", encoding="utf-8", newline="").write(t[:s] + body + t[e:])
    if signature(body) != signature(old):
        set_prototype(a.func, signature(body), headers)
    print("retyped", a.func)
    return 0


if __name__ == "__main__":
    sys.exit(main())
