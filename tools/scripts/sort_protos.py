"""Sort the run of one-line prototypes that ends each owned header by address, the order
decomp.py's declare() inserts new ones in. Usage: sort_protos.py [HEADER...] (default: all)."""
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
import decomp  # noqa: E402


def sort_tail(t):
    lines = t.splitlines(keepends=True)
    end = max(i for i, l in enumerate(lines) if l.startswith("#endif"))
    start = end
    while start > 0 and (not lines[start - 1].strip()
                         or (decomp.decl_name(lines[start - 1]) and "(" in lines[start - 1])):
        start -= 1
    while start < end and not lines[start].strip():
        start += 1
    protos = [l.rstrip("\n") for l in lines[start:end] if l.strip()]
    if len(protos) < 2:
        return t
    known = [p for p in protos if decomp.func_addr(decomp.decl_name(p)) is not None]
    other = [p for p in protos if p not in known]
    known.sort(key=lambda p: decomp.func_addr(decomp.decl_name(p)))
    return "".join(lines[:start]) + "".join(p + "\n\n" for p in known + other) + "".join(lines[end:])


for h in sys.argv[1:] or [h for h in decomp.all_headers() if decomp.header_owner(h)]:
    t = decomp.read(h)
    s = sort_tail(t)
    if s != t:
        decomp.write(h, s)
        print("sorted", h)
