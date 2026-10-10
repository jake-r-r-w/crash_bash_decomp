#!/usr/bin/env python3
"""Generate objdiff.json and the per-unit "target" sources objdiff compares against.

objdiff diffs two objects per unit: the base (build/src/X.c.o, built from our C) and the
target (the original code). For each src/X.c this writes build/objdiff/target/X.c, which
holds every function of that unit as INCLUDE_ASM in address order plus the unit's
INCLUDE_RODATA lines, so compiling it with the normal pipeline gives the original object
with the same symbol names. The Makefile builds it as build/objdiff/target/X.o.

Units whose code is fully replaced by Sony's own objects (link-psyq) are skipped: there is
nothing left to decompile in them.

    tools/dk python3 tools/objdiff_gen.py      # or: tools/dk make objdiff
"""
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)

funcs = json.load(open(".decomp/functions.json"))
proj = json.load(open(".decomp/project.json"))
lib_res = [re.compile(r) for r in proj.get("lib_segments", [])]

by_src = {}
for addr, f in funcs.items():
    if f.get("src"):
        by_src.setdefault(f["src"], []).append((int(addr, 16), f))

RODATA_RE = re.compile(r"^\s*INCLUDE_RODATA\(.*\);", re.M)
units = []
for src in sorted(by_src):
    if not os.path.exists(src):
        continue  # replaced by a linked Sony object
    text = open(src, encoding="utf-8").read()
    unit = src[len("src/"):-len(".c")]
    lines = ['#include "common.h"', ""]
    lines += RODATA_RE.findall(text)
    for _, f in sorted(by_src[src], key=lambda t: t[0]):
        lines.append(f'INCLUDE_ASM("{os.path.dirname(f["asm"])}", {f["name"]});')
    out = f"build/objdiff/target/{unit}.c"
    os.makedirs(os.path.dirname(out), exist_ok=True)
    new = "\n".join(lines) + "\n"
    if not os.path.exists(out) or open(out).read() != new:
        open(out, "w").write(new)
    seg = by_src[src][0][1].get("seg", "")
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
        {"id": "lib", "name": "PsyQ library (unidentified)"},
    ],
    "units": units,
}
open("objdiff.json", "w").write(json.dumps(cfg, indent=2) + "\n")
print(f"objdiff.json: {len(units)} units")
