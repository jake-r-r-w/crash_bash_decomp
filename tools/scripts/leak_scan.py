#!/usr/bin/env python3
"""Find leftover source text in disc files.

Reports every printable run of at least --min bytes that looks like C source
(keywords, preprocessor lines, build-machine paths). Tabs, CR and LF count as
printable, so a run is a whole block of text, not one line.

    tools/dk python3 -I tools/scripts/leak_scan.py FILE... [--min 256] [--dump DIR]

--dump writes each hit to DIR/<file>_<offset>.txt for reading.
"""
import argparse
import os
import re

KEYWORDS = re.compile(
    rb"typedef|struct\s|#define|#include|#if|extern\s|void\s|\bint\s|"
    rb"[A-Za-z]:\\\\?[A-Za-z0-9_]+\\\\|\.c\b|\.h\b|\.obj\b|\.lib\b"
)


def scan(path, minlen, dump):
    data = open(path, "rb").read()
    hits = 0
    for m in re.finditer(rb"[\t\n\r\x20-\x7e]{%d,}" % minlen, data):
        run = m.group()
        kw = len(KEYWORDS.findall(run))
        # Real source has many keyword hits per KB; random text has almost none.
        if kw < max(3, len(run) // 1024):
            continue
        hits += 1
        head = run[:120].decode("ascii").replace("\n", "\\n").replace("\r", "")
        print(f"{path}: 0x{m.start():08X} len {len(run):6d} kw {kw:4d}  {head}")
        if dump:
            name = os.path.basename(path).replace(".", "_")
            with open(os.path.join(dump, f"{name}_{m.start():08X}.txt"), "wb") as f:
                f.write(run)
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="+")
    ap.add_argument("--min", type=int, default=256)
    ap.add_argument("--dump")
    a = ap.parse_args()
    if a.dump:
        os.makedirs(a.dump, exist_ok=True)
    total = sum(scan(p, a.min, a.dump) for p in a.files)
    print(f"{total} hits")


if __name__ == "__main__":
    main()
