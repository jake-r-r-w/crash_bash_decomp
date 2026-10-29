#!/usr/bin/env python3
"""Filter splat's output: drop the "lost symbol" notices this project expects, keep the rest.

    python3 -m splat split YAML 2>&1 | python3 tools/scripts/splat_quiet.py YAML

An overlay's yaml reads main.exe's and the engine's symbol files, so that its references to them
get their names. Those addresses lie outside the overlay's own segments, so splat lists every
one under "Unable to determine a segment" (thousands per overlay). It still names and links them
(they go to its unknown segment and into the undefined-symbol lists the links and shift_all.py
read), so the notice is expected for them. This filter drops a listed symbol when it is defined
in one of YAML's symbol files other than its own (configs/USA/sym.<that binary>.txt), and keeps
the notice for anything else, which would be a real mistake. It also drops splat's overlap
warning for the known alias EngineEntry / HeapStart (the engine's entry table sits where main.exe's
heap starts). Everything else passes through unchanged.
"""
import os
import re
import sys

ALIASES = {frozenset(("EngineEntry", "HeapStart"))}
HEADER = "Error: Unable to determine a segment for the following user-declared symbols."
LOST = re.compile(r"^\s+([A-Za-z_][\w.]*) \(Vram: 0x[0-9A-Fa-f]+\)\. Suspected segments: .*$")
OVERLAP = re.compile(r"^WARNING: The user declared symbol '([^']+)'")
OVERLAP2 = re.compile(r"^\s+overlaps with the previously defined '([^']+)'")


def foreign_names(yaml_path):
    """Names defined in the symbol files YAML reads, other than its own binary's."""
    own = "sym.%s.txt" % os.path.splitext(os.path.basename(yaml_path))[0]
    names = set()
    in_list = False
    for line in open(yaml_path, encoding="utf-8"):
        if line.strip() == "symbol_addrs_path:":
            in_list = True
            continue
        if in_list:
            m = re.match(r"^\s+-\s+(\S+)", line)
            if not m:
                break
            p = m.group(1)
            if os.path.basename(p) != own and os.path.exists(p):
                names.update(re.findall(r"^\s*([A-Za-z_][\w.]*)\s*=", open(p, encoding="utf-8").read(), re.M))
    return names


def quiet(lines, foreign):
    """LINES less the expected notices."""
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.strip() == HEADER:
            # the header, its guidance lines (indented, not a symbol), then the symbols
            j = i + 1
            guide = []
            while j < len(lines) and lines[j].startswith("  ") and not LOST.match(lines[j]):
                guide.append(lines[j])
                j += 1
            kept = []
            while j < len(lines) and LOST.match(lines[j]):
                if LOST.match(lines[j]).group(1) not in foreign:
                    kept.append(lines[j])
                j += 1
            if kept:
                out += [line] + guide + kept
            i = j
            continue
        m = OVERLAP.match(line)
        if m and i + 1 < len(lines):
            m2 = OVERLAP2.match(lines[i + 1])
            if m2 and frozenset((m.group(1), m2.group(1))) in ALIASES:
                i += 2
                continue
        out.append(line)
        i += 1
    return out


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    foreign = foreign_names(sys.argv[1])
    lines = sys.stdin.read().split("\n")
    sys.stdout.write("\n".join(quiet(lines, foreign)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
