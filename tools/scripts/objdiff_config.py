#!/usr/bin/env python3
"""Generate objdiff.json and the per-unit "target" sources objdiff compares against.

objdiff diffs two objects per unit: the base (build/src/X.c.o, built from our C) and the
target (the original code). For each src/X.c this writes build/objdiff/target/X.c, which
holds every function of that unit as INCLUDE_ASM in address order plus the unit's
INCLUDE_RODATA lines, so compiling it with the normal pipeline gives the original object
with the same symbol names. The Makefile builds it as build/objdiff/target/X.o.

The function asm comes from build/objdiff/asm/, a second splat run with
configs/USA/objdiff.yaml merged in (disassemble_all): asm/ only holds the functions still
INCLUDE_ASM'd in src, while the target needs all of them. Names and addresses are read
from those files, so renames in sym.*.txt are picked up without touching functions.json.

Units whose code is fully replaced by Sony's own objects (link-psyq) are skipped: there is
nothing left to decompile in them.

    tools/dk make objdiff-config      # splats into build/objdiff/asm and builds the targets

objdiff.json is generated, not committed.
"""
import glob
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
os.chdir(ROOT)

ASM = "build/objdiff/asm"   # asm_path in configs/USA/objdiff.yaml

funcs = json.load(open(".decomp/functions.json"))
proj = json.load(open(".decomp/project.json"))
lib_res = [re.compile(r) for r in proj.get("lib_segments", [])]

# src -> (asm dirs under asm/, splat segment); functions.json keys ("8001...", "engine:8007...")
# aren't needed: the asm files give each function's current name and address.
by_src = {}
for f in funcs.values():
    if f.get("src") and f.get("asm"):
        dirs, _ = by_src.setdefault(f["src"], (set(), f.get("seg", "")))
        dirs.add(os.path.relpath(os.path.dirname(f["asm"]), "asm"))

# "nonmatching NAME, 0xSIZE" heads a function file (rodata files have no size), then
# "glabel NAME" and the first instruction's "/* ROM VRAM WORD */" comment.
FUNC_RE = re.compile(r"^nonmatching (\S+), 0x[0-9A-Fa-f]+\s*\n\s*glabel \1\s*\n\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", re.M)


def unit_functions(dirs):
    """[(vram, name)] of every function splat wrote for these asm dirs, in address order."""
    out = []
    for d in dirs:
        for path in glob.glob(f"{ASM}/{d}/*.s"):
            m = FUNC_RE.search(open(path, encoding="utf-8").read())
            if m:
                out.append((int(m.group(2), 16), m.group(1), f"{ASM}/{d}"))
    return sorted(out)


RODATA_RE = re.compile(r'^\s*INCLUDE_RODATA\("asm/(.*)\);', re.M)
units = []
for src in sorted(by_src):
    if not os.path.exists(src):
        continue  # replaced by a linked Sony object
    dirs, seg = by_src[src]
    fns = unit_functions(dirs)
    if not fns:
        raise SystemExit(f"{src}: no functions under {ASM}/ (run `make objdiff-config`, which splats there first)")
    text = open(src, encoding="utf-8").read()
    unit = src[len("src/"):-len(".c")]
    lines = ['#include "common.h"', ""]
    lines += [f'INCLUDE_RODATA("{ASM}/{rest});' for rest in RODATA_RE.findall(text)]
    lines += [f'INCLUDE_ASM("{d}", {name});' for _, name, d in fns]
    out = f"build/objdiff/target/{unit}.c"
    os.makedirs(os.path.dirname(out), exist_ok=True)
    new = "\n".join(lines) + "\n"
    if not os.path.exists(out) or open(out).read() != new:
        open(out, "w").write(new)
    cat = "lib" if any(r.search(seg) for r in lib_res) else "game"
    units.append({
        "name": unit,
        "target_path": f"build/objdiff/target/{unit}.o",
        "base_path": f"build/{src}.o",
        "metadata": {"progress_categories": [cat], "source_path": src},
    })
cfg = {
    "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
    # objdiff (on the host) rebuilds through the toolchain container
    "custom_make": "powershell",
    "custom_args": ["-NoProfile", "-ExecutionPolicy", "Bypass", "-File", "tools/dk.ps1", "make"],
    "build_target": True,
    "build_base": True,
    "watch_patterns": ["*.c", "*.h"],
    "progress_categories": [
        {"id": "game", "name": "Game code"},
        {"id": "lib", "name": "PsyQ library (not linked from Sony objects)"},
    ],
    "units": units,
}
open("objdiff.json", "w").write(json.dumps(cfg, indent=2) + "\n")
print(f"objdiff.json: {len(units)} units")
