#!/usr/bin/env python3
"""Every struct's remaining unkXX fields, sorted by what the code does with them:

    tools/dk python3 tools/scripts/unk_fields.py [HEADER.h ...] [--json OUT]

For each struct in include/bash/ (or the headers named) with unkXX fields, field_uses' probe
finds every line using one (one syntax compile per struct, of its overlay's files or, for an
engine or exe header, all of them). A field is `none` (no use: make it padXX), `write` (only ever
stored: nothing shows what it holds) or `read` (some line reads it: name it from those lines).
Prints a count per struct and the totals; --json writes the reads too.
"""
import argparse
import collections
import concurrent.futures
import glob
import json
import os
import re
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import field_rename as fr  # noqa: E402
import field_uses as fu  # noqa: E402

OVL = "arkenoid|bash|bonk|chase|dino|hub|n2o|papu|pogo|polar|race|spin|tank|titles"


def kind(field, lines):
    """none, write or read for FIELD's use LINES (a read is any use but a plain store target)."""
    if not lines:
        return "none", []
    use = re.compile(r"(?:->|\.)%s\b" % re.escape(field))
    store = re.compile(r"(?:->|\.)%s(?:\[[^\]]*\])*\s*=(?!=)" % re.escape(field))
    step = re.compile(r"(?:->|\.)%s\b[^;=]*(?:\+\+|--)|(?:\+\+|--)[^;]*(?:->|\.)%s\b" % (re.escape(field), re.escape(field)))
    reads = [l for l in lines if len(use.findall(l)) > len(store.findall(l)) or step.search(l)]
    return ("read" if reads else "write"), reads


def files_for(h, every):
    m = re.match(r"(%s)[_.]" % OVL, os.path.basename(h))
    return sorted(glob.glob("src/overlays/%s/*.c" % m.group(1))) if m else every


def write_retry(path, text):
    for _ in range(50):     # Windows can hold a header open for a moment
        try:
            open(path, "w", encoding="utf-8", newline="").write(text)
            return
        except PermissionError:
            time.sleep(0.2)
    raise SystemExit("could not write " + path)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("headers", nargs="*")
    ap.add_argument("--json")
    a = ap.parse_args()
    every = sorted(glob.glob("src/**/*.c", recursive=True))
    out, total = {}, collections.Counter()
    for h in sorted(glob.glob("include/bash/*.h")):
        if a.headers and os.path.basename(h) not in a.headers:
            continue
        t0 = open(h, encoding="utf-8").read()
        for s in sorted(set(re.findall(r"^\}\s*(\w+)\s*;", t0, re.M)) | set(re.findall(r"^struct (\w+) \{", t0, re.M))):
            w = fu.find_struct(s)
            if not w or w[0] != h:
                continue
            text = open(h, encoding="utf-8").read()
            _, b, e = w
            decl = dict(fu.members(text[b:e]))
            fields = [n for n in decl if re.match(r"unk[0-9A-Fa-f]+$", n)]
            if not fields:
                continue
            write_retry(h, fu.probe(text, b, e, fields))
            try:
                with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
                    found = set(x for r in ex.map(fr.errors, files_for(h, every)) for x in r)
            finally:
                write_retry(h, text)
            src, uses = {}, collections.defaultdict(list)
            for f, n, m in found:
                if m in fields:
                    uses[m].append(src.setdefault(f, open(f, encoding="utf-8").read().split("\n"))[n - 1].strip())
            rec = {}
            for f in fields:
                k, reads = kind(f, uses.get(f, []))
                rec[f] = {"kind": k, "reads": reads[:6], "decl": decl[f]}
                total[k] += 1
            out["%s:%s" % (os.path.basename(h), s)] = rec
            print("%-28s %s" % (s, dict(collections.Counter(r["kind"] for r in rec.values()))), flush=True)
    print("total", dict(total))
    if a.json:
        json.dump(out, open(a.json, "w"), indent=1)
    return 0


if __name__ == "__main__":
    sys.exit(main())
