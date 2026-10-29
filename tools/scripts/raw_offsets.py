#!/usr/bin/env python3
"""Raw offsets the progress check's typed test misses, in the exe's game functions:

    tools/dk python3 tools/scripts/raw_offsets.py

`*(T *)(p + N)`, `p[N / 4]` and `(T *)(p + 0xN)` on a byte pointer, where `decomp.py progress`
only sees a cast at the addition (`(u8 *)p + N`). Lines marked `/* matching:` are left out. Prints
each function with its lines, most first.
"""
import re, sys
sys.path.insert(0, "tools/scripts")
import decomp as d
PAT = re.compile(r"\*\s*\(\s*[\w ]+\*+\s*\)\s*\(\s*\w+\s*\+\s*(?:0x[0-9A-Fa-f]+|\d+)\s*\)|\b\w+\[(?:0x[0-9A-Fa-f]+|\d+) / 4\]|\(\s*[\w ]+\*+\s*\)\s*\(\s*\w+\s*\+\s*0x[0-9A-Fa-f]+\s*\)")
funcs = d.need_funcs(); syms = d.sym_names(); proj = d.need_proj()
out = []
for k, e in funcs.items():
    if not e.get("src", "").startswith("src/main/") or d.is_lib(e, proj): continue
    n = d.cur_name(k, e, syms); b = d.extract_function(d.read(e["src"]), n)
    if not b: continue
    ls = [l.strip() for l in b.split("\n") if PAT.search(l) and "/* matching:" not in l]
    if ls: out.append((len(ls), n, e["src"], ls))
out.sort(reverse=True)
print(len(out), "functions,", sum(x[0] for x in out), "lines")
for c, n, s, ls in out:
    print("== %s %s (%d)" % (n, s, c))
    for l in ls[:4]: print("     ", l[:140])
