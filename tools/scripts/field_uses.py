#!/usr/bin/env python3
"""Every use of a struct's fields, by the compiler's own account:

    tools/dk python3 tools/scripts/field_uses.py STRUCT [--fields F ...] [--all] [--files F.c ...]

The struct's header gets its `unkXX` fields (or --fields, or with --all every field) renamed for
one syntax-only compile of every C file (field_rename.py's), and each `no member named` error is
a line that uses one of them; the header is restored afterwards. Field names many structs share
(`unk10`) can't be told apart by text; this sees only this struct's. Prints each field with its
offset and type line and its distinct uses, most frequent first, for naming the field from what
the code does with it.
"""
import argparse
import collections
import concurrent.futures
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import field_rename as fr  # noqa: E402

PROBE = "__use"


def find_struct(name, root="include/bash"):
    """(header path, start, end) of STRUCT's body braces in the header that defines it."""
    for h in sorted(glob.glob(os.path.join(root, "*.h"))):
        t = open(h, encoding="utf-8").read()
        for m in re.finditer(r"(?:typedef struct\s*\w*\s*|struct %s\s*)\{" % re.escape(name), t):
            depth, i = 0, m.end() - 1
            while i < len(t):
                depth += {"{": 1, "}": -1}.get(t[i], 0)
                if depth == 0:
                    break
                i += 1
            close = t[i:i + 200]
            if (m.group(0).startswith("struct %s" % name) and m.group(0) != "typedef struct {"
                    and not m.group(0).startswith("typedef")) or re.match(r"\}\s*%s\s*;" % re.escape(name), close):
                return h, m.end(), i
    return None


def members(body):
    """[(name, declaration line)] of a struct body's direct members (nested unions included)."""
    out = []
    for line in body.split("\n"):
        code = re.sub(r"/\*.*?\*/", "", line).strip()
        m = re.search(r"\b(\w+)\s*(?:\[[^\]]*\])*\s*;", code)
        if m and code and not code.startswith("}") and m.group(1) not in ("struct", "union"):
            out.append((m.group(1), line.strip()))
    return out


def probe(text, start, end, fields):
    """TEXT with FIELDS renamed to NAME__use inside the struct body only."""
    body = text[start:end]
    for f in fields:
        body = re.sub(r"\b%s\b(?=\s*(?:\[[^\]]*\]\s*)*;)" % re.escape(f), f + PROBE, body)
    return text[:start] + body + text[end:]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("struct")
    ap.add_argument("--fields", nargs="*")
    ap.add_argument("--all", action="store_true", help="every member, not only the unkXX ones")
    ap.add_argument("--files", nargs="*", help="C files to compile (default: every src/**/*.c)")
    ap.add_argument("--top", type=int, default=6, help="uses shown per field")
    a = ap.parse_args()
    where = find_struct(a.struct)
    if not where:
        sys.exit("no struct %s in include/bash" % a.struct)
    h, s, e = where
    text = open(h, encoding="utf-8").read()
    decl = members(text[s:e])
    fields = a.fields or [n for n, _ in decl if a.all or re.match(r"unk[0-9A-Fa-f]+$", n)]
    files = a.files or sorted(glob.glob("src/**/*.c", recursive=True))
    open(h, "w", encoding="utf-8", newline="").write(probe(text, s, e, fields))
    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
            found = set(x for r in ex.map(fr.errors, files) for x in r)
    finally:
        open(h, "w", encoding="utf-8", newline="").write(text)
    src = {}
    uses = collections.defaultdict(collections.Counter)
    for f, n, m in found:
        if m in fields:
            lines = src.setdefault(f, open(f, encoding="utf-8").read().split("\n"))
            uses[m][lines[n - 1].strip()] += 1
    print("%s (%s)" % (a.struct, h))
    for name, line in decl:
        if name not in fields:
            continue
        c = uses.get(name, collections.Counter())
        print("--- %s: %s  [%d uses]" % (name, line, sum(c.values())))
        for l, k in c.most_common(a.top):
            print("   %3d  %s" % (k, l[:140]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
