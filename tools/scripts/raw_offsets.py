#!/usr/bin/env python3
"""Raw offsets the progress check's typed test misses, in the exe's game functions:

    tools/dk python3 tools/scripts/raw_offsets.py [--src DIR ...]

`*(T *)(p + N)`, `p[N / 4]` and `(T *)(p + 0xN)` on a byte pointer, where `decomp.py progress`
only sees a cast at the addition (`(u8 *)p + N`). Lines marked `/* matching:` are left out. Prints
each function with its lines, most first. --src scans other source folders (default src/main/,
e.g. --src src/overlays/dino).
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import decomp as d  # noqa: E402

PAT = re.compile(r"\*\s*\(\s*[\w ]+\*+\s*\)\s*\(\s*\w+\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\)"
                 r"|\b\w+\[(?:0x[0-9A-Fa-f]+|\d+) / 4\]"
                 r"|\(\s*[\w ]+\*+\s*\)\s*\(\s*\w+\s*\+\s*0x[0-9A-Fa-f]+\s*\)")


def raw_lines(body):
    """The lines of BODY reading through a byte offset, but those kept for the matching."""
    return [l.strip() for l in body.split("\n") if PAT.search(l) and "/* matching:" not in l]


def main():
    if any(a in ("-h", "--help") for a in sys.argv[1:]):
        print(__doc__)
        return 0
    args = sys.argv[1:]
    dirs = [args[i + 1].rstrip("/") + "/" for i, x in enumerate(args) if x == "--src"] or ["src/main/"]
    funcs, syms, proj = d.need_funcs(), d.sym_names(), d.need_proj()
    out = []
    for k, e in funcs.items():
        if not any(e.get("src", "").startswith(x) for x in dirs) or d.is_lib(e, proj):
            continue
        n = d.cur_name(k, e, syms)
        b = d.extract_function(d.read(e["src"]), n)
        ls = raw_lines(b) if b else []
        if ls:
            out.append((len(ls), n, e["src"], ls))
    out.sort(reverse=True)
    print(len(out), "functions,", sum(x[0] for x in out), "lines")
    for c, n, s, ls in out:
        print("== %s %s (%d)" % (n, s, c))
        for l in ls[:4]:
            print("     ", l[:140])
    return 0


if __name__ == "__main__":
    sys.exit(main())
