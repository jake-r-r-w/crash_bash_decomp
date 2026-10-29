#!/usr/bin/env python3
"""The final test's "nothing hidden" scan (ROADMAP-B 4.2): everything the build links is source.

    tools/dk python3 tools/scripts/hidden.py          (after make: it reads build/*.map)

It fails (exit 1) on any of these:
- in src/ and include/: an INCLUDE_ASM or INCLUDE_RODATA, an .incbin, inline asm, a NON_MATCHING
  block (a parked draft), or an address name (func_XXXXXXXX, D_XXXXXXXX);
- a function parked in .decomp's state;
- a linked object that isn't built from src/ or one of Sony's PsyQ objects (build/psyq/).

Each allowed exception is listed in EXCEPTIONS with the decision behind it, and the report
prints them.
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# linked objects from outside src/, each with its reason
EXCEPTIONS = {
    "build/asm/main/psyq/2mbyte.s.o":
        "Sony's startup object (2MBYTE.OBJ, PsyQ): Sony's code, which the repo doesn't hold, cut from "
        "the user's disc as the other PsyQ objects come from the user's SDK",
    "build/asm/bin/overlays/engine/engine_bss.bin.o":
        "the engine's .bss image, the build machine's leftovers: the matching build reads them from the "
        "disc, source builds write zeros (decided 2026-10-29, ROADMAP-A)",
    "build/asm/bin/overlays/engine/engine_slot.bin.o":
        "the engine's slot image, the build machine's leftovers: as the .bss image",
}
# source lines allowed to hold asm, each with its reason
ASM_ALLOWED = {
    ("src/main/prims.c", "register s32 "):
        "draw_verts' three original register pins, the recorded exception (CONVENTIONS.md)",
    ("include/bash/main.h", "#define gte_"):
        "three GTE macros in the form of the SDK's inline_c.h ones (gte_ldv0_w, gte_ldv0_t, "
        "gte_stsxy3_10): the gte_* exception the match gate makes too (SDK_HELPER)",
}
SKIP_FILES = {"include/include_asm.h"}  # the INCLUDE_ASM macro itself, kept for reworking a function

SOURCE_CHECKS = [
    ("asm include", re.compile(r"\bINCLUDE_(?:ASM|RODATA)\s*\(")),
    (".incbin", re.compile(r"\.incbin\b")),
    ("inline asm", re.compile(r"\b(?:__asm__|asm)\s*(?:volatile\s*)?\(")),
    ("parked draft", re.compile(r"^\s*#\s*if(?:n?def)?\b.*\bNON_MATCHING\b", re.M)),
    ("address name", re.compile(r"\b(?:func|D)_[0-9A-Fa-f]{8}\b")),
]


def scan_source(path, text):
    """[(line, kind, text)] of the checks TEXT fails, less the allowed lines."""
    out = []
    for n, line in enumerate(text.split("\n"), 1):
        for kind, rx in SOURCE_CHECKS:
            if rx.search(line):
                if kind == "inline asm" and any(path == f and pat in line for f, pat in ASM_ALLOWED):
                    continue
                out.append((n, kind, line.strip()))
    return out


def scan_map(text):
    """The objects a GNU ld map LOADs that aren't built from src/ or PsyQ: [path]."""
    return [m for m in re.findall(r"^LOAD (\S+)", text, re.M)
            if not m.startswith(("build/src/", "build/psyq/")) and m.endswith(".o")]


def parked():
    import decomp
    funcs = decomp.need_funcs()
    stat, _ = decomp.statuses(funcs)
    return sorted(funcs[k]["name"] for k, v in stat.items() if v == "blocked")


def main():
    bad = 0
    files = sorted(glob.glob("src/**/*.c", recursive=True) + glob.glob("include/**/*.h", recursive=True))
    for f in files:
        if f.replace(os.sep, "/") in SKIP_FILES:
            continue
        for n, kind, line in scan_source(f.replace(os.sep, "/"), open(f, encoding="utf-8", errors="replace").read()):
            print(f"FAIL {f}:{n}: {kind}: {line[:100]}")
            bad += 1
    p = parked()
    for name in p:
        print(f"FAIL parked: {name}")
    bad += len(p)
    maps = sorted(glob.glob("build/*.map"))
    if not maps:
        sys.exit("hidden: no build/*.map: run make first")
    seen = set()
    for m in maps:
        for o in scan_map(open(m, encoding="utf-8", errors="replace").read()):
            if o in EXCEPTIONS:
                seen.add(o)
            else:
                print(f"FAIL {m}: links {o}, not built from src/")
                bad += 1
    print(f"{len(files)} source files, {len(maps)} links scanned; parked: {len(p)}")
    for o in sorted(seen):
        print(f"allowed: {o}: {EXCEPTIONS[o]}")
    for (f, _), why in ASM_ALLOWED.items():
        print(f"allowed: {f}: {why}")
    print("OK: nothing hidden" if not bad else f"{bad} problem(s)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
