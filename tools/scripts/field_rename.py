#!/usr/bin/env python3
"""Rename a struct field everywhere it is used, by the compiler's own account.

Rename the field in its struct by hand first, then run

    tools/dk python3 tools/scripts/field_rename.py OLD NEW

Every C file under src/ is preprocessed (with line markers) and compiled by cc1 for syntax
only; each `structure has no member named `OLD'` error names a line (in a .c file or a
header's inline function) that used the renamed field, and `.OLD` / `->OLD` on that line
becomes NEW. This repeats until no unit reports one. A field name like `unk6C` that many
structs share can't be renamed by text; this touches only the uses the compiler ties to the
renamed struct. A line where two different structs' OLD fields meet gets both renamed, and
then reports `no member named `NEW'`: those lines are printed for a hand fix. Code doesn't
change (the build checks it: `make check`).
"""
import argparse
import concurrent.futures
import glob
import os
import re
import subprocess
import sys

CPP = ["mipsel-linux-gnu-cpp", "-nostdinc", "-undef", "-Iinclude", "-Iext/psyq47/include",
       "-D__GNUC__=2", "-D_LANGUAGE_C", "-DPSX", "-DGVAL=0"]
CC1 = ["/opt/gcc/2.8.1-psx/cc1", "-quiet", "-O0", "-G0", "-fno-builtin", "-o", "/dev/null"]
ERR = re.compile(r"^(.+?):(\d+): (?:structure|union) has no member named `(\w+)'")


def errors(path):
    """(file, line, member) for every missing-member error compiling path."""
    cpp = subprocess.run(CPP + [path], capture_output=True, text=True)
    cc1 = subprocess.run(CC1, input=cpp.stdout, capture_output=True, text=True)
    out = []
    for l in cc1.stderr.splitlines():
        m = ERR.match(l)
        if m:
            out.append((os.path.normpath(m.group(1)), int(m.group(2)), m.group(3)))
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("old")
    ap.add_argument("new")
    ap.add_argument("--files", nargs="*", help="C files to compile (default: every src/**/*.c)")
    a = ap.parse_args()
    files = a.files or sorted(glob.glob("src/**/*.c", recursive=True))
    pat = re.compile(r"(\.|->)\s*%s\b" % re.escape(a.old))
    changed = 0
    for rnd in range(10):
        with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
            found = set()
            for r in ex.map(errors, files):
                found.update(r)
        olds = sorted({(f, n) for f, n, m in found if m == a.old})
        news = sorted({(f, n) for f, n, m in found if m == a.new})
        if not olds:
            for f, n in news:
                print("%s:%d: both fields meet here, fix by hand" % (f, n))
            print("renamed %d uses of %s to %s in %d round(s)" % (changed, a.old, a.new, rnd))
            return 1 if news else 0
        byfile = {}
        for f, n in olds:
            byfile.setdefault(f, set()).add(n)
        for f, lines in byfile.items():
            src = open(f, encoding="utf-8").read().split("\n")
            for n in lines:
                new = pat.sub(lambda m: m.group(1) + a.new, src[n - 1])
                if new != src[n - 1]:
                    src[n - 1] = new
                    changed += 1
                else:
                    print("%s:%d: no `.%s` on the line" % (f, n, a.old))
            open(f, "w", encoding="utf-8", newline="").write("\n".join(src))
    print("gave up after 10 rounds", file=sys.stderr)
    return 1


if __name__ == "__main__":
    sys.exit(main())
