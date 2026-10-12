#!/usr/bin/env python3
"""Helper CLI for a PSX matching decompilation.

Runs inside the toolchain container:   tools/dk python3 tools/scripts/decomp.py <command> [args]

Output is deliberately short. The caller is usually a model that pays for every line it
reads, so details go to files and stdout carries only verdicts, small diff windows and
one-line summaries. Run with -h for the command list.
"""
import argparse
import datetime
import glob
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import time
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor

DEC = ".decomp"
P_PROJ = os.path.join(DEC, "project.json")
P_FUNCS = os.path.join(DEC, "functions.json")
P_STATE = os.path.join(DEC, "state.json")
P_HIST = os.path.join(DEC, "history.json")
CFG = "configs/USA"          # splat configs, symbols, checksums, compiler settings
P_MK = f"{CFG}/project.mk"
P_SUMS = f"{CFG}/checksum.sha"
PSYQ_CFG = f"{CFG}/psyq"      # PsyQ provenance: objects found, layout, linker glue

GCC_ROOT = os.environ.get("GCC_ROOT", "/opt/gcc")
MASPSX = os.environ.get("MASPSX_PY", "/opt/maspsx/maspsx.py")
M2C = os.environ.get("M2C_PY", "/opt/m2c/m2c.py")
PERMUTER = os.environ.get("PERMUTER_PY", "/opt/permuter/permuter.py")
X = os.environ.get("CROSS", "mipsel-linux-gnu-")
CPPFLAGS = ["-P", "-nostdinc", "-undef", "-Iinclude", "-Iext/psyq47/include", "-D__GNUC__=2", "-D_LANGUAGE_C", "-DPSX"]
ASFLAGS = ["-Iinclude", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0"]
DEFAULT_MASPSX = "--macro-inc --expand-div"

# splat comment on each instruction: /* ROM VRAM WORD */ instruction
ASM_LINE = re.compile(r"/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*(.*)$")
INC_ASM = re.compile(r'INCLUDE_ASM\(\s*"([^"]+)"\s*,\s*(\w+)\s*\)')
SYM_LINE = re.compile(r"^\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")
OBJ_LINE = re.compile(r"^\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)$")
REL_LINE = re.compile(r"^\s*([0-9a-f]+):\s+(R_MIPS_\w+)\s+(\S+)")
# bits that are compared when an instruction carries a relocation (the rest is filled by the linker)
RELOC_KEEP = {"R_MIPS_26": 0xFC000000, "R_MIPS_HI16": 0xFFFF0000,
              "R_MIPS_LO16": 0xFFFF0000, "R_MIPS_GPREL16": 0xFFFF0000}
M2C_PRELUDE = """typedef signed char s8; typedef unsigned char u8; typedef short s16;
typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(expr, type, offset) (*(type)((char*)(expr) + (offset)))
#define M2C_ERROR(x) 0
#define NULL ((void*)0)
"""


# ---------------------------------------------------------------- small helpers

def die(msg, code=2):
    print("ERROR: " + msg)
    sys.exit(code)


def now():
    return datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def jload(path, default):
    try:
        with open(path, encoding="utf-8") as f:
            return json.load(f)
    except FileNotFoundError:
        return default


def jsave(path, obj):
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    tmp = path + ".tmp"
    with open(tmp, "w", encoding="utf-8", newline="\n") as f:
        json.dump(obj, f, indent=1, sort_keys=True)
        f.write("\n")
    os.replace(tmp, path)


def read(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read()


def write(path, text):
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    for attempt in range(6):
        try:
            with open(path, "w", encoding="utf-8", newline="\n") as f:
                f.write(text)
            return
        except OSError:
            # Docker Desktop's Windows bind mount can return EINVAL on rapid rewrites; retry
            if attempt == 5:
                raise
            time.sleep(0.2 * (attempt + 1))


def run(cmd, inp=None, timeout=None):
    shell = isinstance(cmd, str)
    try:
        return subprocess.run(cmd, input=inp, capture_output=True, text=True, shell=shell,
                              executable="/bin/bash" if shell else None, timeout=timeout)
    except FileNotFoundError as ex:
        die(f"{ex.filename} not found - run this inside the toolchain container: tools/dk python3 tools/scripts/decomp.py ...")


def tail(text, n=20):
    lines = [l for l in text.strip().splitlines() if l.strip()]
    return "\n".join(lines[-n:])


def mk_config():
    cfg = {}
    if not os.path.exists(P_MK):
        die(f"{P_MK} missing - finish setup (decomp.py probe --write or decomp.py config)")
    for line in read(P_MK).splitlines():
        m = re.match(r"^\s*([A-Z_0-9]+)\s*:?=\s*(.*?)\s*$", line)
        if m:
            cfg[m.group(1)] = m.group(2)
    return cfg


def write_mk(name, exe, cc, opt, g, maspsx, extra=""):
    if g and f"-G{g}" not in maspsx.split():
        maspsx = (maspsx + f" -G{g}").strip()
    write(P_MK, f"""# Compiler settings for this project (written by tools/scripts/decomp.py)
NAME := {name}
EXE := {exe}
CC_VER := {cc}
OPT_FLAGS := {opt}
GVAL := {g}
MASPSX_ARGS := {maspsx}
EXTRA_CC1FLAGS := {extra}
""")


def need_proj():
    proj = jload(P_PROJ, None)
    if not proj:
        die("no .decomp/project.json - run setup first (decomp.py extract <cue>)")
    return proj


def need_funcs():
    funcs = jload(P_FUNCS, None)
    if not funcs:
        die("no function index - run `make split` then `decomp.py index`")
    return funcs


def is_psx_exe(path):
    try:
        with open(path, "rb") as f:
            return f.read(8) == b"PS-X EXE"
    except OSError:
        return False


def exe_header(path):
    with open(path, "rb") as f:
        h = f.read(0x800)
    if h[:8] != b"PS-X EXE":
        return None
    pc0, gp0, t_addr, t_size = struct.unpack_from("<4I", h, 0x10)
    region = h[0x4C:0x80].split(b"\0")[0].decode("ascii", "replace").strip()
    return {"pc0": f"{pc0:08X}", "gp0": f"{gp0:08X}", "t_addr": f"{t_addr:08X}",
            "t_size": t_size, "region": region}


# ---------------------------------------------------------------- asm / source scanning

def parse_asm(path):
    """Instructions of one splat function file: (rom, vram, words, texts, has_jal, uses_gp)."""
    rom = vram = None
    words, texts = [], []
    jal = gp = False
    in_text = True
    for line in read(path).splitlines():
        s = line.strip()
        if s.startswith(".section"):
            in_text = ".text" in s
            continue
        if s == ".text":
            in_text = True
            continue
        if s in (".rodata", ".data", ".bss") or s.startswith(".late_rodata"):
            in_text = False
            continue
        if not in_text:
            continue
        m = ASM_LINE.search(line)
        if not m:
            continue
        if rom is None:
            rom, vram = int(m.group(1), 16), int(m.group(2), 16)
        words.append(int(m.group(3), 16))
        ins = re.sub(r"\s+", " ", m.group(4)).strip()
        texts.append(ins)
        if ins.startswith("jal "):
            jal = True
        if "%gp_rel" in ins or "($gp)" in ins:
            gp = True
    return rom, vram, words, texts, jal, gp


REF_SYM = re.compile(r"%(?:hi|lo|gp_rel)\(([A-Za-z_][\w.]*)(?:\s*\+\s*(0x[0-9A-Fa-f]+|\d+))?\)")
JUMP_SYM = re.compile(r"^(?:jal|j)\s+([A-Za-z_]\w*)$")


def insn_syms(text):
    """Symbols an instruction refers to through relocations: [(name, offset)]."""
    m = JUMP_SYM.match(text)
    if m and not m.group(1).startswith(".L"):
        return [(m.group(1), 0)]
    return [(s, int(o, 0) if o else 0) for s, o in REF_SYM.findall(text)]


def fingerprints(words, texts):
    """(exact, near) hashes of a function's code. exact masks only the fields the linker fills
    (jal targets, %hi/%lo/%gp_rel immediates), so two functions with the same exact hash
    compile from the same C with only symbol names changed. near also masks every other
    16-bit immediate (constants, struct offsets, stack sizes): the same code shape."""
    n = trim_nops(words)
    ex, nr = [], []
    for w, t in zip(words[:n], texts[:n]):
        op = w >> 26
        if op in (2, 3):                             # j/jal: absolute target, filled by the linker
            w &= 0xFC000000
        elif "%hi(" in t or "%lo(" in t or "%gp_rel(" in t:
            w &= 0xFFFF0000
        ex.append(w)
        # I-type with an immediate that isn't a branch offset
        nr.append(w & 0xFFFF0000 if op >= 8 and op not in (0x10, 0x12) else w)
    h = lambda ws: hashlib.sha1(struct.pack(f"<{len(ws)}I", *ws)).hexdigest()[:16]
    return h(ex), h(nr)


def scan_src():
    """(seg, name) -> (src file, asm folder) for every INCLUDE_ASM still in src/. Keyed by segment
    too: overlays that share a load address get identical splat names (func_800B5000)."""
    inc = {}
    for p in glob.glob("src/**/*.c", recursive=True):
        for m in INC_ASM.finditer(read(p)):
            seg = os.path.relpath(m.group(1), "asm/nonmatchings").replace("\\", "/")
            inc[(seg, m.group(2))] = (p.replace("\\", "/"), m.group(1))
    return inc


def func_key(vram, seg, ovl):
    """functions.json key: the address for the main exe; '<overlay>:<address>' for overlay code."""
    return f"{os.path.splitext(os.path.basename(ovl[seg]))[0]}:{vram:08X}" if seg in ovl else f"{vram:08X}"


def yaml_of(name):
    """splat config of a build target: the exe is configs/USA/main.yaml, overlays live in overlays/."""
    return f"{CFG}/{name}.yaml" if name == "main" else f"{CFG}/overlays/{name}.yaml"


def set_checksum(built, sha):
    """Record `sha  built` in configs/USA/checksum.sha (one line per target, read by make check)."""
    lines = [l for l in (read(P_SUMS).splitlines() if os.path.exists(P_SUMS) else []) if not l.endswith("  " + built)]
    write(P_SUMS, "\n".join(lines + [f"{sha}  {built}"]) + "\n")


def sym_file(ovl):
    """Where names live: configs/USA/sym.main.txt for the exe, sym.<ovl>.txt per overlay
    (slot overlays share load addresses, so each needs its own)."""
    return f"{CFG}/sym.{ovl}.txt" if ovl else f"{CFG}/sym.main.txt"


def sym_names():
    """functions.json-style key (ADDRESS or OVERLAY:ADDRESS) -> name."""
    names = {}
    for p in glob.glob(f"{CFG}/sym.*.txt"):
        b = os.path.basename(p)[len("sym."):-len(".txt")]
        ovl = "" if b == "main" else b
        for line in read(p).splitlines():
            m = SYM_LINE.match(line)
            if m:
                a = int(m.group(2), 16)
                names[f"{ovl}:{a:08X}" if ovl else f"{a:08X}"] = m.group(1)
    return names


def cur_name(key, e, syms):
    return syms.get(key, e["name"])


def find_func(funcs, name):
    """NAME, SEG/NAME, or a functions.json key (ADDRESS or OVERLAY:ADDRESS)."""
    syms = sym_names()
    n = name.lower()
    seg = None
    if "/" in n and ":" not in n:
        seg, n = n.rsplit("/", 1)
    hits = [(k, e) for k, e in funcs.items()
            if (seg is None or e["seg"].lower() == seg)
            and n in (e["name"].lower(), e["orig"].lower(), cur_name(k, e, syms).lower(), k.lower())]
    if len(hits) > 1:
        die(f"{name} is ambiguous - give SEG/NAME: " + ", ".join(f"{e['seg']}/{e['name']}" for _, e in hits))
    if not hits:
        die(f"unknown function {name} (not in .decomp/functions.json - re-run index?)")
    return hits[0]


def statuses(funcs):
    inc, syms, st = scan_src(), sym_names(), jload(P_STATE, {})
    out = {}
    for k, e in funcs.items():
        if e.get("kind") in ("asm", "sony"):
            out[k] = e["kind"]  # asm: written in assembly, the .s is its source; sony: Sony's object linked
            continue
        names = {e["name"], e["orig"], cur_name(k, e, syms)}
        if any((e["seg"], nm) in inc for nm in names):
            s = st.get(k, {}).get("status", "todo")
            out[k] = s if s in ("blocked", "in_progress") else "todo"
        elif e.get("src"):
            out[k] = "matched"
        else:
            out[k] = "unsplit"  # never seen as INCLUDE_ASM in a C file (asm-only segment)
    return out, st


def is_lib(e, proj):
    return any(re.search(p, e["seg"]) for p in proj.get("lib_segments", []))


def func_def_regex(name):
    return re.compile(r"^[^\n;#{}]*\b" + re.escape(name) + r"\s*\([^;{]*\)\s*\{", re.M)


def locate_def(name):
    rx = func_def_regex(name)
    for p in glob.glob("src/**/*.c", recursive=True):
        if rx.search(read(p)):
            return p.replace("\\", "/")
    return None


def extract_function(text, name):
    m = func_def_regex(name).search(text)
    if not m:
        return None
    depth, i = 0, m.end() - 1
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return text[m.start():i + 1] + "\n"
        i += 1
    return None


# ---------------------------------------------------------------- compile and compare

def obj_func(obj, name):
    r = run([X + "objdump", "-drz", f"--disassemble={name}", obj])
    words, texts, relocs, base = [], [], {}, None
    for line in r.stdout.splitlines():
        m = REL_LINE.match(line)
        if m and base is not None:
            relocs[(int(m.group(1), 16) - base) // 4] = (m.group(2), m.group(3))
            continue
        m = OBJ_LINE.match(line)
        if m:
            off = int(m.group(1), 16)
            if base is None:
                base = off
            words.append(int(m.group(2), 16))
            texts.append(re.sub(r"\s+", " ", m.group(3)).strip())
    for i, (typ, sym) in relocs.items():
        if i < len(texts):
            texts[i] += f"  [{typ[7:]} {sym}]"
    return words, texts, relocs


def overlay_segs():
    """seg name -> original binary, for every overlay yaml (configs/USA/overlays/<ovl>.yaml)."""
    import yaml
    out = {}
    proj = jload(P_PROJ, {})
    for o in proj.get("overlays", []):
        y = yaml.safe_load(read(yaml_of(o['name'])))
        for seg in y.get("segments", []):
            for sub in (seg.get("subsegments", []) if isinstance(seg, dict) else []):
                if isinstance(sub, list) and len(sub) >= 3 and sub[1] in ("c", "asm"):
                    out[str(sub[2])] = f"disc/{o['name']}.bin"
    return out


def target_words(e, proj):
    with open(e.get("bin") or os.path.join("disc", proj["exe"]), "rb") as f:
        f.seek(e["rom"])
        data = f.read(e["size"])
    return list(struct.unpack(f"<{len(data) // 4}I", data))


def trim_nops(ws):
    n = len(ws)
    while n and ws[n - 1] == 0:
        n -= 1
    return n


def compare(tw, cw, relocs):
    nt, nc = trim_nops(tw), trim_nops(cw)
    diffs, ops = [], 0
    for i in range(max(nt, nc)):
        if i >= nt or i >= nc:
            diffs.append(i)
            ops += 1
            continue
        keep = RELOC_KEEP.get(relocs.get(i, ("",))[0], 0xFFFFFFFF)
        t, c = tw[i] & keep, cw[i] & keep
        if t != c:
            diffs.append(i)
            special = (t >> 26) == 0 and (c >> 26) == 0
            if (t >> 26) != (c >> 26) or (special and (t & 0x3F) != (c & 0x3F)):
                ops += 1
    return diffs, ops, nt, nc


def show_diff(diffs, ttext, ctext, nt, nc, limit=40):
    dset = set(diffs)
    n = max(nt, nc)
    rows = sorted({j for i in diffs for j in range(i - 2, i + 3) if 0 <= j < n})
    print(f"{'':6}{'target':<38}| compiled")
    prev = None
    for j in rows[:limit]:
        if prev is not None and j != prev + 1:
            print("   ...")
        t = ttext[j] if j < nt and j < len(ttext) else "-"
        c = ctext[j] if j < nc and j < len(ctext) else "-"
        print(f"{'!' if j in dset else ' '}{j * 4:4x} {t[:37]:<37} | {c[:60]}")
        prev = j
    if len(rows) > limit:
        print(f"   ... {len(rows) - limit} more rows")


def compile_error(src, stderr):
    """Short build error, with the offending preprocessed line where cc1 gives one."""
    out = tail(stderr, 12)
    ipath = f"build/{src}.i"
    m = re.search(r":(\d+):", stderr)
    if m and os.path.exists(ipath):
        lines = read(ipath).splitlines()
        ln = int(m.group(1))
        if 0 < ln <= len(lines):
            out += f"\n  line {ln} of preprocessed {src}: {lines[ln - 1].strip()[:150]}"
    return out


def check_function(key, e, proj, name, quiet=False):
    """Build e's source file and compare. Returns (ok, ndiffs)."""
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    obj = f"build/{src}.o"
    if os.path.exists(obj):
        os.remove(obj)
    r = run(["make", "-s", obj])
    if r.returncode:
        print("BUILD FAILED")
        print(compile_error(src, r.stderr + r.stdout))
        return False, None
    cw, ctext, relocs = obj_func(obj, name)
    if not cw:
        print(f"{name} not found in {obj} - still INCLUDE_ASM, misspelled, or static and inlined?")
        return False, None
    tw = target_words(e, proj)
    diffs, ops, nt, nc = compare(tw, cw, relocs)
    if not diffs:
        if not quiet:
            print(f"MATCH {name} ({nt} instructions)")
        return True, 0
    ttext = parse_asm(e["asm"])[3] if os.path.exists(e.get("asm", "")) else [f"{w:08x}" for w in tw]
    print(f"DIFF {name}: {len(diffs)}/{max(nt, nc)} instructions differ "
          f"({ops} opcode, {len(diffs) - ops} operand-only); length target {nt} vs compiled {nc}")
    show_diff(diffs, ttext, ctext, nt, nc, int(os.environ.get("DIFF_ROWS", "40")))
    return False, len(diffs)


def pipeline(c_path, o_path, cc, opt, g, maspsx):
    gflag = f"-G{g}"
    return (f"{X}cpp {' '.join(CPPFLAGS)} {c_path} | {GCC_ROOT}/{cc}/cc1 -quiet {opt} {gflag} "
            f"| python3 tools/scripts/dmpsx.py | python3 {MASPSX} {maspsx}{' ' + gflag if g else ''} | {X}as {' '.join(ASFLAGS)} -o {o_path}")


def m2c_draft(e, src=None):
    ctx = []
    if src:
        build_ctx(src)
        ctx = ["--context", "build/ctx.c"]
    r = run(["python3", M2C, "-t", "mipsel-gcc-c", *ctx, e["asm"]])
    return r.returncode == 0, (r.stdout if r.returncode == 0 else tail(r.stderr, 8))


def build_ctx(src):
    incs = [l for l in read(src).splitlines() if l.lstrip().startswith("#include")]
    write("build/ctx_in.c", "\n".join(incs) + "\n")
    r = run([X + "cpp", *CPPFLAGS, "-DM2CTX", "build/ctx_in.c"])
    write("build/ctx.c", r.stdout if r.returncode == 0 else "")
    return r.stdout if r.returncode == 0 else ""


def permuter_base(src, decls, body, tag):
    """base.c for decomp-permuter: the file's headers, decls and the function, preprocessed together
    (a preprocessed header alone loses its macros, so a body using NULL etc. wouldn't compile)."""
    incs = [l for l in read(src).splitlines() if l.lstrip().startswith("#include")]
    p = f"build/permuter/{tag}_in.c"
    write(p, "\n".join(incs + list(decls)) + "\n\n" + body)
    r = run([X + "cpp", *CPPFLAGS, "-DM2CTX", "-DPERMUTER", p])
    return r.stdout if r.returncode == 0 else None


# ---------------------------------------------------------------- commands: setup

def cmd_extract(a):
    cue = a.cue
    if not os.path.exists(cue):
        die(f"{cue} not found (put the bin/cue in disc/)")
    text = read(cue)
    files = re.findall(r'FILE\s+"([^"]+)"', text, re.I) or re.findall(r"FILE\s+(\S+)", text, re.I)
    if not files:
        die("no FILE line in the cue sheet")
    data_bin = os.path.join(os.path.dirname(cue), files[0])
    out = "disc/files"
    shutil.rmtree(out, ignore_errors=True)
    r = run(["dumpsxiso", "-x", out, "-s", "disc/layout.xml", data_bin])
    if r.returncode:
        die("dumpsxiso failed:\n" + tail(r.stdout + r.stderr))
    allfiles = [os.path.join(dp, fn) for dp, _, fns in os.walk(out) for fn in fns]
    lower = {os.path.relpath(p, out).replace("\\", "/").lower(): p for p in allfiles}
    boot = None
    if "system.cnf" in lower:
        m = re.search(r"BOOT\s*=\s*cdrom0?:\\*([^;\s]+)", read(lower["system.cnf"]), re.I)
        if m:
            boot = lower.get(m.group(1).replace("\\", "/").lstrip("/").lower())
    boot = boot or lower.get("psx.exe")
    if not boot:
        die("boot executable not found (no BOOT line in SYSTEM.CNF and no PSX.EXE)")
    exe = os.path.basename(boot).upper()
    name = "main"
    shutil.copyfile(boot, os.path.join("disc", exe))
    with open(boot, "rb") as f:
        sha = hashlib.sha1(f.read()).hexdigest()
    set_checksum(f"build/{name}.exe", sha)
    others = sorted(os.path.relpath(p, out) for p in allfiles if p != boot and is_psx_exe(p))
    proj = jload(P_PROJ, {})
    proj.update({"name": name, "exe": exe, "cue": cue, "sha1": sha, "header": exe_header(boot),
                 "other_exes": others, "lib_segments": proj.get("lib_segments", [])})
    jsave(P_PROJ, proj)
    h = proj["header"] or {}
    print(f"boot exe {exe} -> disc/{exe}  sha1 {sha[:12]}  text {h.get('t_addr')} "
          f"size 0x{h.get('t_size', 0):X}  entry {h.get('pc0')}  gp {h.get('gp0')}  region '{h.get('region')}'")
    print(f"{len(allfiles)} files extracted to {out}/")
    if others:
        print(f"other executables ({len(others)}), candidates for later targets: " + ", ".join(others[:12])
              + (" ..." if len(others) > 12 else ""))
    for o in proj.get("overlays", []):
        extract_overlay(o)
        print(f"overlay {o['name']} -> disc/{o['name']}.bin")


def extract_overlay(o):
    """Cut an overlay out of its container file (e.g. an archive on the disc) into disc/<name>.bin."""
    with open(o["from"], "rb") as f:
        f.seek(o["offset"])
        data = f.read(o["size"])
    if len(data) != o["size"]:
        die(f"{o['from']} is too short for overlay {o['name']}")
    with open(f"disc/{o['name']}.bin", "wb") as f:
        f.write(data)
    sha = hashlib.sha1(data).hexdigest()
    if o.get("sha1") and o["sha1"] != sha:
        die(f"overlay {o['name']}: sha1 {sha} differs from the recorded {o['sha1']} (different disc version?)")
    return sha


def cmd_add_overlay(a):
    """Register a code overlay stored inside another file (archive) as its own build target."""
    proj = need_proj()
    off = a.offset if a.offset is not None else (a.sector * a.sector_size if a.sector is not None else None)
    if off is None:
        die("give --offset BYTES or --sector N")
    o = {"name": a.name, "from": a.source.replace("\\", "/"), "offset": off, "size": a.size}
    o["sha1"] = extract_overlay(o)
    set_checksum(f"build/{a.name}.bin", o["sha1"])
    proj["overlays"] = [x for x in proj.get("overlays", []) if x["name"] != a.name] + [o]
    jsave(P_PROJ, proj)
    print(f"overlay {a.name}: {a.size} bytes from {o['from']} @0x{off:X} -> disc/{a.name}.bin, "
          f"sha1 {o['sha1'][:12]}; now write configs/USA/overlays/{a.name}.yaml (see references/setup.md, overlays)")


def cmd_index(a):
    proj = need_proj()
    funcs = jload(P_FUNCS, {})
    inc = scan_src()
    ovl = overlay_segs()
    src_text = {}
    added = gp_funcs = 0
    files = sorted(glob.glob("asm/nonmatchings/**/*.s", recursive=True))
    if not files:
        die("no asm/nonmatchings/**/*.s - run `make split` with code segments of type c")
    # entries keyed by bare address before their segment became an overlay: move them (and their
    # state) to the OVERLAY:ADDRESS key
    st = jload(P_STATE, {})
    for k in [k for k, e in funcs.items() if ":" not in k and e.get("seg") in ovl]:
        nk = func_key(int(k, 16), funcs[k]["seg"], ovl)
        funcs[nk] = funcs.pop(k)
        if k in st:
            st[nk] = st.pop(k)
    jsave(P_STATE, st)
    for p in files:
        rom, vram, words, texts, jal, gp = parse_asm(p)
        if rom is None:
            continue
        name = os.path.splitext(os.path.basename(p))[0]
        seg = os.path.dirname(os.path.relpath(p, "asm/nonmatchings")).replace("\\", "/")
        key = func_key(vram, seg, ovl)
        e = funcs.get(key)
        if e is None:
            added += 1
            e = funcs[key] = {"orig": name}
        # splat's comments show each word's bytes in file order: swap to the instruction value
        words = [int.from_bytes(w.to_bytes(4, "big"), "little") for w in words]
        fx, fn = fingerprints(words, texts)
        # direct calls, as target addresses (resolved to keys by call_graph)
        calls = sorted({f"{(vram & 0xF0000000) | ((w & 0x3FFFFFF) << 2):08X}"
                        for w in words if w >> 26 == 3})
        e.update({"name": name, "seg": seg, "rom": rom, "size": len(words) * 4,
                  "asm": p.replace("\\", "/"), "leaf": not jal, "fp": fx, "fpn": fn, "calls": calls})
        e.pop("kind", None)     # back in a C file
        if seg in ovl:
            e["bin"] = ovl[seg]          # rom is an offset into this overlay file, not the exe
        else:
            e.pop("bin", None)
        if (seg, name) in inc:
            e["src"] = inc[(seg, name)][0]
        elif not e.get("src") or not os.path.exists(e["src"]):
            # no INCLUDE_ASM (or its file moved): splat wrote it as C itself (empty `void f(void) {}`), which matches
            p = f"src/{seg}.c"
            if os.path.exists(p):
                if p not in src_text:
                    src_text[p] = read(p)
                if func_def_regex(name).search(src_text[p]):
                    e["src"] = p
        gp_funcs += gp
    jsave(P_FUNCS, funcs)
    proj["gp_funcs"] = gp_funcs
    jsave(P_PROJ, proj)
    total = sum(e["size"] for e in funcs.values())
    nosrc = sum(1 for e in funcs.values() if not e.get("src"))
    print(f"indexed {len(funcs)} functions ({added} new), {total} code bytes")
    print(f"{gp_funcs} functions use $gp-relative access -> {'-G8 (or -G4) likely' if gp_funcs > 5 else '-G0 likely'}")
    if nosrc:
        print(f"WARNING: {nosrc} functions have no INCLUDE_ASM and no C definition in src/ (asm-type segment?) "
              "and won't count as matchable")


def cmd_config(a):
    proj = need_proj()
    cur = mk_config() if os.path.exists(P_MK) else {}
    cc = a.cc or cur.get("CC_VER")
    if not cc:
        die("--cc is required (one of: " + ", ".join(sorted(os.listdir(GCC_ROOT))) + ")")
    if not os.path.exists(f"{GCC_ROOT}/{cc}/cc1"):
        die(f"compiler {cc} not installed; have: " + ", ".join(sorted(os.listdir(GCC_ROOT))))
    g = a.g if a.g is not None else int(cur.get("GVAL", "0"))
    maspsx = a.maspsx if a.maspsx is not None else re.sub(r"\s*-G\d+", "", cur.get("MASPSX_ARGS", DEFAULT_MASPSX))
    write_mk(proj["name"], proj["exe"], cc, a.opt or cur.get("OPT_FLAGS", "-O2"), g, maspsx,
             a.extra if a.extra is not None else cur.get("EXTRA_CC1FLAGS", ""))
    print(f"{P_MK}: {cc} {a.opt or cur.get('OPT_FLAGS', '-O2')} -G{g}")


def cmd_probe(a):
    proj, funcs = need_proj(), need_funcs()
    gccs = sorted(d for d in os.listdir(GCC_ROOT) if os.path.exists(f"{GCC_ROOT}/{d}/cc1"))
    gvals = [8, 4, 0] if proj.get("gp_funcs", 0) > 5 else [0]
    configs = [(cc, opt, g) for cc in gccs for opt in ("-O2", "-O1") for g in gvals]
    # Libraries were usually built with a different compiler, so probe game code unless --seg says otherwise.
    pool = sorted((e for e in funcs.values() if e.get("leaf") and 12 <= e["size"] <= 160
                   and os.path.exists(e.get("asm", ""))
                   and (re.search(a.seg, e["seg"]) if a.seg else not is_lib(e, proj))),
                  key=lambda e: e["size"])
    score = {c: 0 for c in configs}
    os.makedirs("build/probe", exist_ok=True)
    tested = []
    for e in pool:
        if len(tested) >= a.n:
            break
        ok, draft = m2c_draft(e)
        if not ok or "M2C_ERROR" in draft:
            continue
        write("build/probe/f.c", M2C_PRELUDE + re.sub(r"^\s*(static\s+)?\?.*$", "", draft, flags=re.M))
        tw = target_words(e, proj)
        compiled = False
        for c in configs:
            if os.path.exists("build/probe/f.o"):
                os.remove("build/probe/f.o")
            r = run(pipeline("build/probe/f.c", "build/probe/f.o", c[0], c[1], c[2], DEFAULT_MASPSX))
            if r.returncode:
                continue
            compiled = True
            cw, _, rel = obj_func("build/probe/f.o", e["name"])
            if cw and not compare(tw, cw, rel)[0]:
                score[c] += 1
        if compiled:
            tested.append(e["name"])
    if not tested:
        die("no m2c drafts compiled - set the compiler by hand with `decomp.py config --cc ...`")
    ranked = sorted(configs, key=lambda c: (-score[c], c[2] != gvals[0], c[1] != "-O2"))
    print(f"probed {len(tested)} small leaf functions; exact matches per config:")
    for c in ranked[:8]:
        print(f"  {score[c]:2d}/{len(tested)}  {c[0]:<12} {c[1]} -G{c[2]}")
    best = ranked[0]
    if score[best] == 0:
        print("no config matched anything: m2c drafts may need types. Pick by release year (see setup.md).")
        return
    ties = [c for c in ranked if score[c] == score[best]]
    if len(ties) > 1:
        print(f"{len(ties)} configs tie; the first listed is used. Larger functions will tell them apart.")
    if a.write:
        write_mk(proj["name"], proj["exe"], best[0], best[1], best[2], DEFAULT_MASPSX)
        print(f"wrote {P_MK}: {best[0]} {best[1]} -G{best[2]}")


# ---------------------------------------------------------------- commands: matching

def cmd_status(a):
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    s = summary(funcs, stat, proj)
    inprog = [funcs[k]["name"] for k, v in stat.items() if v == "in_progress"]
    print(f"{proj['exe']}: game code {s['game']['pct']:.2f}% ({s['game']['done_b']}/{s['game']['total_b']} bytes, "
          f"{s['game']['done_f']}/{s['game']['total_f']} functions); libs {s['lib']['pct']:.2f}%; "
          f"parked {s['blocked']}; in progress: {', '.join(inprog) or 'none'}")


def asm_listing(path, limit):
    """Compact listing of a function's .s: instructions and branch labels, plus its own rodata labels."""
    out, rodata, in_text = [], [], True
    for line in read(path).splitlines():
        s = line.strip()
        if s.startswith(".section"):
            in_text = ".text" in s
            continue
        if not in_text:
            m = re.match(r"dlabel\s+(\w+)", s)
            if m:
                rodata.append(m.group(1))
            continue
        m = ASM_LINE.search(line)
        if m:
            out.append("    " + re.sub(r"\s+", " ", m.group(4)).strip())
        elif re.match(r"^\.L\w+:", s):
            out.append("  " + s)
    text = out[:limit] + ([f"    ... {len(out) - limit} more lines ({path})"] if len(out) > limit else [])
    return text, rodata


def pick_targets(funcs, stat, st, proj, seg, count):
    pool = [(k, e) for k, e in funcs.items() if stat[k] in ("todo", "in_progress")
            and (seg and e["seg"].startswith(seg) or not seg and not is_lib(e, proj))]
    # Most code per unit of work first. Work: auto's instruction-diff count where known (a
    # compiling draft that's 2 off beats a fresh 8-instruction function), otherwise the
    # instruction count. Payoff: the function plus its unmatched identical copies, which
    # `done` fills in from the same C.
    twins = defaultdict(int)
    for k, e in funcs.items():
        if e.get("fp") and e["size"] > 8 and stat[k] != "matched":
            twins[e["fp"]] += 1

    def cost(ke):
        rec = st.get(ke[0], {})
        work = rec.get("auto_diff", ke[1]["size"] // 4)
        return work / max(1, twins.get(ke[1].get("fp"), 1))
    pool.sort(key=lambda ke: (stat[ke[0]] != "in_progress", st.get(ke[0], {}).get("tries", 0) > 0,
                              cost(ke), ke[1]["size"], ke[0]))
    return pool[:count]


def relatives(key, funcs, stat, syms):
    """Lines naming a function's identical copies and its near-identical matched relatives."""
    e, out = funcs[key], []
    for field, label in (("fp", "identical copies (one match fills all)"),
                         ("fpn", "near-identical, constants differ")):
        h = e.get(field)
        if not h or e["size"] <= 8:
            continue
        rel = [k for k, x in funcs.items() if k != key and x.get(field) == h
               and (field == "fp" or x.get("fp") != e.get("fp"))]
        if rel:
            out.append(f"  {label}: " + ", ".join(
                f"{funcs[k]['seg']}/{cur_name(k, funcs[k], syms)}{' (matched, adapt its C)' if stat[k] == 'matched' else ''}"
                for k in rel[:8]) + (f" +{len(rel) - 8}" if len(rel) > 8 else ""))
    return out


AUTO_DRAFTS = "build/auto_drafts"


def auto_draft(key):
    """The compiling (non-matching) draft auto saved for this function, or None."""
    p = f"{AUTO_DRAFTS}/{key}.json"
    return jload(p, None) if os.path.exists(p) else None


def cmd_next(a):
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms = sym_names()
    if a.func:
        key, e = find_func(funcs, a.func)
        if stat[key] == "matched":
            die(f"{a.func} is already matched")
        targets = [(key, e)]
    else:
        targets = pick_targets(funcs, stat, st, proj, a.seg, max(1, a.count))
        if not targets:
            die("nothing left to match in this scope" + (" (parked functions: use --func NAME)" if any(
                v == "blocked" for v in stat.values()) else ""))
    cg_callers = call_graph(funcs)[1]
    for i, (key, e) in enumerate(targets):
        name = cur_name(key, e, syms)
        rec = st.get(key, {})
        # only a single pick is being worked on; --count N is a survey and marks nothing
        # (in_progress functions are served first, so stray marks would bias every later pick)
        if len(targets) == 1 and rec.get("status") != "in_progress":
            rec = st.setdefault(key, {})
            rec.update(status="in_progress", started=now())
        if i:
            print()
        print(f"TARGET {name}  @{key}  seg {e['seg']}  {e['size']} bytes ({e['size'] // 4} insns)"
              f"{'  leaf' if e.get('leaf') else ''}{'  [resumed]' if rec.get('tries') else ''}"
              f"  src {e.get('src') or '?'}")
        if rec.get("note"):
            print(f"  note: {rec['note']}")
        for line in relatives(key, funcs, stat, syms):
            print(line)
        callers = sorted(cg_callers.get(key, ()))
        if callers:
            print(f"  called by {len(callers)}: " + ", ".join(cur_name(k, funcs[k], syms) for k in callers[:6])
                  + (" ..." if len(callers) > 6 else ""))
        if not os.path.exists(e.get("asm", "")):
            print("  asm file missing - run `make split`")
            continue
        lst, rodata = asm_listing(e["asm"], a.asm_lines)
        print("--- asm" + (f" (own rodata: {', '.join(rodata)})" if rodata else "") + " ---")
        print("\n".join(lst))
        ad = auto_draft(key)
        if ad:
            # already typed and compiling: start from it instead of the raw m2c output
            print(f"--- auto draft (compiles; {rec.get('auto_diff', '?')} instructions differ) ---")
            draft = "\n".join(ad["decls"] + [""] + [ad["ftext"]]) if ad["decls"] else ad["ftext"]
        else:
            ok, draft = m2c_draft(e, e.get("src"))
            print("--- m2c draft ---" if ok else "--- m2c failed ---")
        lines = draft.rstrip().splitlines()
        print("\n".join(lines[:a.max_lines]))
        if len(lines) > a.max_lines:
            print(f"... {len(lines) - a.max_lines} more lines (decomp.py m2c {name} for the rest)")
    jsave(P_STATE, st)


def cmd_queue(a):
    """The next targets as JSON, in `next` order, marking nothing (for tools/swarm)."""
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms = sym_names()
    out = [{"key": k, "name": cur_name(k, e, syms), "seg": e["seg"], "size": e["size"],
            "src": e.get("src"), "fp": e.get("fp") if e["size"] > 8 else None,
            "tries": st.get(k, {}).get("tries", 0), "note": st.get(k, {}).get("note", "")}
           for k, e in pick_targets(funcs, stat, st, proj, a.seg, a.count)]
    print(json.dumps(out))


def cmd_m2c(a):
    funcs = need_funcs()
    key, e = find_func(funcs, a.func)
    ok, draft = m2c_draft(e, e.get("src"))
    print(draft.rstrip())


def cmd_try(a):
    proj, funcs = need_proj(), need_funcs()
    syms = sym_names()
    st = jload(P_STATE, {})
    all_ok = True
    stat = statuses(funcs)[0]
    for fname in a.funcs:
        key, e = find_func(funcs, fname)
        name = cur_name(key, e, syms)
        ok, nd = check_function(key, e, proj, name)
        if stat[key] == "matched":          # re-checking a match is not an attempt
            all_ok &= ok
            continue
        rec = st.setdefault(key, {"status": "in_progress", "started": now()})
        rec["tries"] = rec.get("tries", 0) + 1
        if nd is not None:
            rec["best"] = min(rec.get("best", nd), nd)
        if not ok and rec["tries"] >= 8 and rec["tries"] % 4 == 0:
            print(f"({rec['tries']} tries so far - consider `permute {name}` or `block {name} --note ...`)")
        all_ok &= ok
    jsave(P_STATE, st)
    sys.exit(0 if all_ok else 1)


def verify_build(proj, funcs=None):
    shutil.rmtree("build/src", ignore_errors=True)
    r = run(["make", "-s", f"-j{os.cpu_count() or 1}", "all"])  # ~5x faster, same bytes
    if r.returncode:
        out = r.stderr + r.stdout
        errs = [l for l in out.splitlines() if re.search(r"error|undefined reference|No rule|Error \d", l, re.I)]
        return False, "BUILD FAILED\n" + ("\n".join(errs[:20]) if errs else tail(out, 20))
    pairs = [(f"build/{proj['name']}.exe", os.path.join("disc", proj["exe"]), None)]
    pairs += [(f"build/{o['name']}.bin", f"disc/{o['name']}.bin", f"disc/{o['name']}.bin")
              for o in proj.get("overlays", [])]
    oks = []
    for built_p, target_p, binkey in pairs:
        with open(built_p, "rb") as f:
            built = f.read()
        with open(target_p, "rb") as f:
            target = f.read()
        if built == target:
            oks.append(f"{built_p} ({len(built)} bytes)")
            continue
        off = next((i for i in range(min(len(built), len(target))) if built[i] != target[i]),
                   min(len(built), len(target)))
        where = "outside indexed functions (data, rodata, header or padding)"
        for e in (funcs or {}).values():
            if e.get("bin") == binkey and e["rom"] <= off < e["rom"] + e["size"]:
                where = f"inside {e['name']} (+0x{off - e['rom']:X})"
                break
        return False, (f"MISMATCH in {built_p}: first difference at file offset 0x{off:X}, {where}; "
                       f"size built {len(built)} vs target {len(target)}")
    return True, "OK: " + ", ".join(oks) + " match"


def cmd_verify(a):
    proj = need_proj()
    ok, msg = verify_build(proj, jload(P_FUNCS, {}))
    print(msg)
    sys.exit(0 if ok else 1)


# m2c leftovers and hacks a hand match must not contain (see docs/CONVENTIONS.md, Matching rules)
LINT_ERRORS = [
    (re.compile(r"\bM2C_\w+"), "m2c macro"),
    (re.compile(r"(^|[\s(,])\?(\s|\*)"), "m2c unknown type `?`"),
    (re.compile(r"\b(asm|__asm__)\s*(volatile\s*)?\(\s*\"[^\"]"), "inline asm with instructions"),
]
# allowed only with --hack "reason", which goes into the commit message
LINT_HACKS = [
    (re.compile(r"\bregister\b[^;]*\b(asm|__asm__)\s*\("), "register pin"),
    (re.compile(r"\b(asm|__asm__)\s*(volatile\s*)?\(\s*\"\"\s*\)"), "empty asm barrier"),
]
INCLUDE_ASM_LINE = re.compile(r'^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)')


def lint_function(src, name):
    """(errors, hacks) found in NAME's definition in SRC."""
    body = extract_function(read(src), name) or ""
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    # a register pin's asm("$20") names a register, not instructions: it's a hack, not an error
    unpinned = re.sub(r"(\bregister\b[^;]*?)\b(asm|__asm__)\s*\(\s*\"[^\"]*\"\s*\)", r"\1", body)
    errs = sorted({why for rx, why in LINT_ERRORS if rx.search(unpinned)})
    hacks = sorted({why for rx, why in LINT_HACKS if rx.search(body)})
    return errs, hacks


def matched_in_worktree():
    """Functions whose INCLUDE_ASM line the working tree removes from src/ compared with HEAD."""
    r = run(["git", "diff", "HEAD", "-U0", "--", "src"])
    gone = {m.group(1) for l in r.stdout.splitlines() if l.startswith("-") and not l.startswith("---")
            for m in [INCLUDE_ASM_LINE.match(l[1:])] if m}
    back = {m.group(1) for l in r.stdout.splitlines() if l.startswith("+") and not l.startswith("+++")
            for m in [INCLUDE_ASM_LINE.match(l[1:])] if m}
    return gone - back


def cmd_done(a):
    """Validation gate for one hand-matched function: scope, lint, function match, full build.
    Only after all four pass is it recorded (and committed with --commit)."""
    proj, funcs = need_proj(), need_funcs()
    syms = sym_names()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, syms)
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    # 1. scope: one function per commit
    others = sorted(matched_in_worktree() - {name, e["name"], e["orig"]})
    if others:
        die(f"the working tree also replaces INCLUDE_ASM for {', '.join(others)}: one function per "
            "commit. Restore those INCLUDE_ASM lines (keep the C elsewhere) and match them separately.")
    # 2. lint: no m2c leftovers, hacks only when declared
    errs, hacks = lint_function(src, name)
    if errs:
        die(f"{name}: {', '.join(errs)} left in the C - clean it up before recording the match")
    if hacks and not a.hack:
        die(f"{name} uses {' and '.join(hacks)}: pass --hack \"why it is needed\" (it goes in the commit "
            "message) or remove it")
    # its prototype goes in its file's owned header (unless an original header declares it)
    proto = prototype_of(read(src), name)
    if proto:
        declare(src, [proto])
    # 3. the function itself, 4. the whole build
    if not check_function(key, e, proj, name, quiet=True)[0]:
        print(f"{name} does not match: nothing recorded or committed")
        sys.exit(1)
    if a.defer_build:
        # batch mode (tools/swarm): the caller runs `decomp.py verify` once after the batch's last
        # function and publishes none of the batch's commits unless it passes
        print(f"{name}: full build deferred to the end of the batch")
    else:
        ok, msg = verify_build(proj, funcs)
        if not ok:
            print(f"{name} matches alone, but the full build does not:\n{msg}")
            sys.exit(1)
    items = [(key, e, name)]
    record_matches(items, funcs)
    # the same code elsewhere (other overlays, copies in the exe) takes the same C
    copies = propagate(funcs, proj, only={key})
    cmd_progress(a)
    if a.commit:
        notes = ([a.note] if a.note else []) + ([f"Hack: {', '.join(hacks)} - {a.hack}"] if hacks else [])
        if copies:
            notes.append("Identical copies matched with the same C:\n"
                         + "\n".join(f"- {n} ({ce['seg']})" for _, ce, n, *_ in copies))
        gate = ("scope, lint, function; full build once for the batch" if a.defer_build
                else "scope, lint, function, full build")
        commit_match(items, notes + [f"Validated-by: decomp.py done ({gate})"] + (a.trailer or []))


def record_matches(items, funcs, via=None):
    st = jload(P_STATE, {})
    for key, e, name in items:
        rec = st.setdefault(key, {})
        rec.update(status="matched", matched_at=now())
        if via:
            rec["via"] = via
        for k in ("note", "auto_failed"):
            rec.pop(k, None)
        if not e.get("src"):
            e["src"] = locate_def(name)
        print(f"MATCHED {name} ({e['size']} bytes), full build OK")
    jsave(P_STATE, st)
    jsave(P_FUNCS, funcs)


COMMIT_PATHS = ("src", "include", "configs", ".decomp", "docs")


def commit_match(items, trailers, title=None):
    """Commit verified matches. Only reached after every function and the full build matched,
    so a non-matching function can never be committed by this path."""
    paths = [p for p in COMMIT_PATHS if os.path.exists(p)]
    r = run(["git", "add", "--", *paths])
    if r.returncode:
        die("git add failed:\n" + tail(r.stderr))
    if run(["git", "diff", "--cached", "--quiet", "--", *paths]).returncode == 0:
        print("nothing to commit (already committed?)")
        return
    if len(items) == 1 and not title:
        _, e, name = items[0]
        subject, body = f"Match {name} ({e['seg']}, {e['size']} bytes)", ""
    else:
        subject = title or f"Match {len(items)} functions ({sum(e['size'] for _, e, _ in items)} bytes)"
        body = "\n".join(f"- {n} ({e['seg']}, {e['size']} bytes)" for _, e, n in items)
    msg = subject + ("\n\n" + body if body else "") + ("\n\n" + "\n".join(trailers) if trailers else "")
    # tools/hooks/pre-commit refuses matches (removed INCLUDE_ASM lines) committed without this
    r = subprocess.run(["git", "commit", "-q", "-m", msg, "--", *paths], capture_output=True, text=True,
                       env={**os.environ, "DECOMP_VALIDATED": "1"})
    if r.returncode:
        print("git commit failed (matches are recorded; commit by hand):\n" + tail(r.stderr + r.stdout))
        sys.exit(1)
    print("committed " + run(["git", "rev-parse", "--short", "HEAD"]).stdout.strip() + f": {subject}")


# ---------------------------------------------------------------- auto: match trivial functions without the model

MEM_OP = re.compile(r"^(lw|lh|lhu|lb|lbu|sw|sh|sb)\s+\$\w+,\s*(?:%lo\((\w+)\)|(-?0x[0-9A-Fa-f]+|-?\d+))\(\$\w+\)")
EXTERN_LINE = re.compile(r"^extern\s+(.+?)\s*(\**)\s*(\w+)\s*(\[\d*\])?\s*;\s*$")
FUNC_DECL_LINE = re.compile(r"^[A-Za-z_?][\w \t*?]*\b\w+\s*\([^;{}]*\)\s*;\s*(/\*.*\*/)?\s*$")


def mem_widths(texts):
    """offset -> load/store ops, symbol -> ops, from a function's instructions."""
    offs, syms = defaultdict(set), defaultdict(set)
    for t in texts:
        m = MEM_OP.match(t)
        if m:
            (syms[m.group(2)] if m.group(2) else offs[int(m.group(3), 0)]).add(m.group(1))
    return offs, syms


def width_type(ops):
    sizes = {"w" if o in ("lw", "sw") else "h" if o in ("lh", "lhu", "sh") else "b" for o in ops}
    if len(sizes) != 1:
        return None
    s = sizes.pop()
    return {"w": "s32", "h": "u16" if "lhu" in ops else "s16", "b": "u8" if "lbu" in ops else "s8"}[s]


def sanitize_draft(draft, texts):
    """Turn an m2c draft into compilable C, typing `->unkN` / `D_x.unkN` / `extern ?` from the
    asm's load/store widths. Returns (extern lines, function text) or None if it can't."""
    if "M2C_ERROR" in draft:
        return None
    offs, syms = mem_widths(texts)

    def field(base, off_s):
        off = int(off_s, 16)
        t = width_type(offs.get(off, set()))
        if not t:
            raise ValueError
        return f"(*({t}*)((u8*)({base}) + {off}))"

    externs, body = [], []
    for line in draft.splitlines():
        # m2c's own declarations of called functions (`s32 func_x(?); /* extern */`): drop them. The
        # call then uses C's implicit declaration (same code), and prototypes.h supplies the real
        # signature once that function is matched - a stale `s32 f();` here would clash with it.
        if FUNC_DECL_LINE.match(line):
            continue
        m = EXTERN_LINE.match(line)
        (externs if m else body).append(m if m else line)
    text = "\n".join(body).strip() + "\n"
    out_ext = []
    try:
        for m in externs:
            typ, stars, name, arr = m.group(1), m.group(2), m.group(3), m.group(4)
            if "?" not in typ:
                out_ext.append(m.group(0).strip())
                continue
            if name.startswith("func_"):     # a function whose address is taken (a callback)
                out_ext.append(f"void {name}();")
                text = re.sub(rf"&{name}\b", name, text)
                continue
            if re.search(rf"\b{name}\.unk", text) or re.search(rf"&{name}\b", text) or name not in syms:
                out_ext.append(f"extern u8 {name}[];")
                text = re.sub(rf"\b{name}\.unk(-?[0-9A-Fa-f]+)\b", lambda mm: field(name, mm.group(1)), text)
                text = re.sub(rf"&{name}\b", name, text)
            else:
                t = width_type(syms[name])
                if not t:
                    return None
                out_ext.append(f"extern {t} {stars}{name};")
        text = re.sub(r"M2C_FIELD\(([^,()]+), ([^,()]+), (-?0x[0-9A-Fa-f]+|-?\d+)\)",
                      r"(*(\2)((u8*)(\1) + \3))", text)
        text = re.sub(r"\b(\w+)->unk(-?[0-9A-Fa-f]+)\b", lambda mm: field(mm.group(1), mm.group(2)), text)
    except ValueError:
        return None
    text = re.sub(r"(?m)^(\s*)\?(\s)", r"\1s32\2", text)          # `? temp_v0;`, `? func(...)`
    text = text.replace("(?)", "()")                                # unknown parameter list
    text = re.sub(r"([(,]\s*)\?(?=\s|[),])", r"\1s32", text)      # `(? arg0, ? arg1)`, `(s32, ?)`
    text = text.replace("void *", "u8 *")                           # so pointer arithmetic is bytewise
    if "M2C" in text or ".unk" in text or re.search(r"(^|[\s(,])\?(\s|[),])", text):
        return None
    return out_ext, text


def src_includes(src):
    return [l for l in read(src).splitlines() if l.lstrip().startswith("#include")]


HEADERS = "include"
OWNER = re.compile(r"\bowner:\s*(\S+)")


def all_headers():
    return sorted(p.replace("\\", "/") for p in glob.glob(f"{HEADERS}/**/*.h", recursive=True))


def header_owner(h):
    m = OWNER.search(read(h))
    return m.group(1) if m else None


def owned_header(src):
    """The header that declares what `src` defines (marked `owner: <src>`); created if missing.
    One owner per declaration: see docs/CONVENTIONS.md (Headers)."""
    for h in all_headers():
        if header_owner(h) == src:
            return h
    stem = os.path.splitext(os.path.basename(src))[0]
    h, g = f"{HEADERS}/bash/{stem}.h", f"BASH_{stem.upper()}_H"
    if os.path.exists(h):
        die(f"{h} exists but is not marked `owner: {src}`")
    write(h, f"#ifndef {g}\n#define {g}\n\n/* Functions and data defined in {src}. Anything the original "
          f"headers\n * declare (bash/pogo.h, ...) is declared there instead, not here.\n * owner: {src} */\n\n"
          f'#include "common.h"\n#include "bash/nas.h"\n\n#endif /* {g} */\n')
    return h


DECL_NAME = re.compile(r"\b(\w+)\s*(?:\([^;{}]*\)|\[[^\]]*\])?\s*;\s*(?:/\*.*\*/)?\s*$")


def decl_name(line):
    """Name declared by a one-line prototype or extern, or None."""
    l = line.strip()
    if not l or l.startswith(("#", "/*", "*", "typedef", "}")) or not l.endswith((";", "*/")):
        return None
    m = DECL_NAME.search(l)
    return m.group(1) if m else None


def visible_names(src):
    """Names declared by the headers `src` includes (one level, plus their own includes)."""
    seen, names, todo = set(), set(), [re.search(r'"([^"]+)"', l).group(1)
                                       for l in src_includes(src) if '"' in l]
    while todo:
        h = f"{HEADERS}/{todo.pop()}"
        if h in seen or not os.path.exists(h):
            continue
        seen.add(h)
        for l in read(h).splitlines():
            if l.lstrip().startswith("#include") and '"' in l:
                todo.append(re.search(r'"([^"]+)"', l).group(1))
            n = decl_name(l)
            if n:
                names.add(n)
    return names


def declare(src, decls):
    """Add declarations for things `src` uses or defines to its owned header, skipping names
    any header it includes already declares, and make sure it includes that header."""
    h = owned_header(src)
    ensure_include(src, h)
    have = visible_names(src)
    new = []
    for d in decls:
        n = decl_name(d)
        if n and n not in have:
            new.append(d.strip())
            have.add(n)
    if new:
        t = read(h)
        i = t.rstrip().rfind("#endif")
        write(h, t[:i] + "\n".join(new) + "\n\n" + t[i:])


def prototype_of(ftext, name):
    """One-line prototype for function `name` defined in `ftext`, or None."""
    for m in FUNC_DEF.finditer(ftext):
        if m.group(2) == name:
            ret, params = " ".join(m.group(1).split()), " ".join(m.group(3).split())
            return re.sub(r"\s*\*\s*", " *", f"{ret} {name}({params or 'void'});").replace("( *", "(*")
    return None


def dedupe_owned(src):
    """Drop owned-header declarations of names another header `src` includes now declares
    (e.g. after a rename to a name the original headers use)."""
    h = owned_header(src)
    others = set()
    for l in src_includes(src):
        m = re.search(r'"([^"]+)"', l)
        p = f"{HEADERS}/{m.group(1)}" if m else None
        if p and p != h and os.path.exists(p):
            others |= {decl_name(x) for x in read(p).splitlines()} - {None}
    lines = read(h).splitlines(keepends=True)
    kept = [l for l in lines if decl_name(l) not in others]
    if len(kept) != len(lines):
        write(h, "".join(kept))


def compile_alone(src, decls, ftext, name, cfg, tag="t"):
    """Compile one function with its file's headers; return (words, relocs) or None.
    `tag` names the scratch files, so parallel workers don't collide."""
    os.makedirs("build/auto", exist_ok=True)
    incs = src_includes(src)
    c, o = f"build/auto/{tag}.c", f"build/auto/{tag}.o"
    write(c, "\n".join(incs + decls) + "\n\n" + ftext)
    g = int(cfg.get("GVAL", "0"))
    cmd = pipeline(c, o, cfg["CC_VER"], cfg["OPT_FLAGS"], g,
                   re.sub(r"\s*-G\d+", "", cfg.get("MASPSX_ARGS", DEFAULT_MASPSX)))
    # pipefail: without it a cc1 error is hidden and `as` still writes an object from partial output
    if run("set -o pipefail; " + cmd).returncode:
        return None
    w, _, rel = obj_func(o, name)
    return (w, rel) if w else None


def auto_try(job):
    """One auto candidate (runs in a worker thread): (key, reason or None, decls, ftext, ndiff)."""
    k, e, name, src, ctx, cfg, proj = job
    r = run(["python3", M2C, "-t", "mipsel-gcc-c", *(["--context", ctx] if ctx else []), e["asm"]])
    if r.returncode:
        return k, "m2c", None, None, None
    res = sanitize_draft(r.stdout, parse_asm(e["asm"])[3])
    if res is None:
        return k, "types", None, None, None
    decls, ftext = res
    got = compile_alone(src, decls, ftext, name, cfg, tag=f"w_{k}")
    if got is None:
        return k, "compile", None, None, None
    diffs = compare(target_words(e, proj), got[0], got[1])[0]
    if diffs:
        return k, "diff", decls, ftext, len(diffs)   # keep the draft: next shows it, --permute starts from it
    return k, None, decls, ftext, 0


FUNC_DEF = re.compile(r"^(?!static\b|typedef\b|extern\b)([A-Za-z_][\w \t]*?[\s*]+)(\w+)\s*\(([^;{}()]*)\)\s*\{", re.M)
NOT_FUNCS = {"if", "while", "for", "switch", "return", "else", "sizeof", "do"}


def ensure_include(src, header):
    text = read(src)
    rel = os.path.relpath(header, HEADERS).replace("\\", "/") if header.startswith(HEADERS + "/") else header
    inc = f'#include "{rel}"'
    if inc in text:
        return
    lines = text.splitlines(keepends=True)
    last = max((i for i, l in enumerate(lines) if l.lstrip().startswith("#include")), default=-1)
    lines.insert(last + 1, inc + "\n")
    write(src, "".join(lines))


def auto_permute(a, funcs, stat, st, proj, syms, cfg, jobs, skip):
    """Run decomp-permuter on auto's near-misses (saved drafts few instructions off).
    Returns candidates whose permuted version matches in isolation."""
    near = []
    for k, e in funcs.items():
        rec = st.get(k, {})
        if stat.get(k) != "todo" or k in skip or rec.get("auto_failed") != "diff":
            continue
        if rec.get("auto_diff", 1 << 30) > a.max_diff or e["size"] > a.max_size:
            continue
        if not (re.search(a.seg, e["seg"]) if a.seg else not is_lib(e, proj)):
            continue
        if "permuted" in rec and not a.retry:
            continue
        d = auto_draft(k)
        if d:
            near.append((rec["auto_diff"], e["size"], k, e, d))
    near.sort(key=lambda t: t[:3])
    near = near[:a.permute_limit]
    if not near:
        print("  permute: no near-misses to try")
        return []
    print(f"  permute: {len(near)} near-misses (<= {a.max_diff} instructions off), up to {a.permute_seconds}s each")
    found = []
    for i, (nd, _, k, e, d) in enumerate(near, 1):
        name = cur_name(k, e, syms)
        src = e.get("src") or locate_def(name)
        print(f"    [{i}/{len(near)}] {name} ({nd} off)...")
        base = permuter_base(src, d["decls"], d["ftext"], f"auto_{name}")
        if base is None:
            st.setdefault(k, {})["permuted"] = -1
            continue
        best, path, base_score, log = permute_run(f"build/permuter/auto_{name}", name, e, base, cfg, jobs,
                                                  a.permute_seconds)
        if base_score is None and best is None:
            # the permuter didn't start (toolchain or setup problem): say so once, don't mark anything
            print(f"  permute: the permuter failed to run on {name}, stopping:\n{tail(log, 4)}")
            break
        st.setdefault(k, {})["permuted"] = best if best is not None else base_score
        if best != 0:
            continue
        body = extract_function(read(f"{path}/source.c"), name)
        got = compile_alone(src, d["decls"], body, name, cfg, tag=f"p_{k}") if body else None
        if got and not compare(target_words(e, proj), got[0], got[1])[0]:
            print(f"    permuter matched {name} (was {nd} off)")
            found.append((k, e, name, src, d["decls"], body))
    print(f"  permute: {len(found)} matched")
    return found


def cmd_auto(a):
    if not a.legacy:
        die("auto is retired: functions are matched by hand, one per commit (docs/MATCHING.md). "
            "--legacy runs it anyway")
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms, cfg = sym_names(), mk_config()
    pool = [(k, e) for k, e in funcs.items() if stat[k] == "todo" and e["size"] <= a.max_size
            and (re.search(a.seg, e["seg"]) if a.seg else not is_lib(e, proj))
            and (a.retry or not st.get(k, {}).get("auto_failed")) and os.path.exists(e.get("asm", ""))]
    pool.sort(key=lambda ke: (ke[1]["size"], ke[0]))
    pool = pool[:a.limit]
    if not pool and not a.permute:
        die("no candidates (all small functions matched or already tried; --retry or a larger --max-size)")
    jobs = a.jobs or max(1, (os.cpu_count() or 2) - 1)
    srcs = sorted(glob.glob("src/**/*.c", recursive=True))
    protos = not a.no_protos
    print(f"auto: trying {len(pool)} functions <= {a.max_size} bytes on {jobs} workers")
    # m2c context: one preprocessed header set per source file, shared by that file's candidates
    os.makedirs("build/auto", exist_ok=True)
    ctxs, work = {}, []
    for k, e in pool:
        name = cur_name(k, e, syms)
        src = e.get("src") or locate_def(name)
        if src not in ctxs:
            ctxs[src] = f"build/auto/ctx_{len(ctxs)}.c"
            write(ctxs[src], build_ctx(src))
        work.append((k, e, name, src, ctxs[src], cfg, proj))
    info = {w[0]: (w[1], w[2], w[3]) for w in work}
    cands, reasons = [], defaultdict(int)
    os.makedirs(AUTO_DRAFTS, exist_ok=True)
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for k, why, decls, ftext, nd in ex.map(auto_try, work):
            rec = st.setdefault(k, {})
            if why:
                reasons[why] += 1
                rec["auto_failed"] = why
            else:
                e, name, src = info[k]
                cands.append((k, e, name, src, decls, ftext))
            if why == "diff":
                rec["auto_diff"] = nd
                jsave(f"{AUTO_DRAFTS}/{k}.json", {"decls": decls, "ftext": ftext})
            else:
                rec.pop("auto_diff", None)
    shutil.rmtree("build/auto", ignore_errors=True)
    jsave(P_STATE, st)
    print(f"  {len(cands)} match in isolation; skipped: " + (", ".join(f"{v} {r}" for r, v in sorted(reasons.items())) or "none"))
    if a.permute:
        cands += auto_permute(a, funcs, stat, st, proj, syms, cfg, jobs, {c[0] for c in cands})
        jsave(P_STATE, st)
    good = land(cands, funcs, proj, st, protos) if cands else []
    if good is None:
        sys.exit(1)
    if good:
        record_matches([(k, e, name) for k, e, name, *_ in good], funcs, via="auto")
    # copies of everything matched so far (including what just landed) in other overlays/files
    copies = [] if a.no_propagate else propagate(funcs, proj)
    good += copies
    if not good:
        return
    cmd_progress(a)
    if a.commit:
        commit_match([(k, e, name) for k, e, name, *_ in good], a.trailer or [],
                     title=f"Auto-match {len(good)} functions ({sum(c[1]['size'] for c in good)} bytes)")


def land(cands, funcs, proj, st, protos=True, tag="auto_failed"):
    """Insert candidates (key, entry, name, src, decls, ftext) into the real sources and keep the
    ones that still match there, then check the full build. Returns the kept candidates, or None
    when the full build failed (everything reverted)."""
    srcs = sorted(glob.glob("src/**/*.c", recursive=True))
    backup = {p: read(p) for p in srcs + all_headers()}

    def apply(sel):
        for p, t in backup.items():
            write(p, t)
        for h in all_headers():
            if h not in backup:
                os.remove(h)
        for k, e, name, src, decls, ftext in sel:
            # the data it uses and its own prototype go in its file's owned header
            proto = prototype_of(ftext, name) if protos else None
            declare(src, list(decls) + ([proto] if proto else []))
            write(src, read(src).replace(f'INCLUDE_ASM("{os.path.dirname(e["asm"])}", {name});', ftext.rstrip("\n"), 1))

    def real_ok(sel):
        good = []
        for src in {c[3] for c in sel}:
            obj = f"build/{src}.o"
            if os.path.exists(obj):
                os.remove(obj)
            if run(["make", "-s", obj]).returncode:
                return None
            for c in sel:
                if c[3] == src:
                    w, _, rel = obj_func(obj, c[2])
                    if w and not compare(target_words(c[1], proj), w, rel)[0]:
                        good.append(c)
        return good

    def settle(sel, good):
        """Add the candidates in `sel` that still match inside the real file to `good`. A build
        failure splits the batch in half; a build with some mismatches drops just those."""
        if not sel:
            return good
        apply(good + sel)
        r = real_ok(good + sel)
        if r is not None and len(r) == len(good) + len(sel):
            return good + sel
        if len(sel) == 1:
            st.setdefault(sel[0][0], {})[tag] = "in-file"
            return good
        if r is not None:                             # built: keep the ones that matched
            keep = [c for c in sel if c in r]
            for c in sel:
                if c not in r:
                    st.setdefault(c[0], {})[tag] = "in-file"
            return settle(keep, good) if len(keep) < len(sel) else good
        half = len(sel) // 2                          # didn't build: bisect
        return settle(sel[half:], settle(sel[:half], good))

    def settle_full(sel, good):
        """Add the candidates in `sel` that keep the full build matching (a function can match on
        its own yet add data or rodata that shifts the rest of the file). Bisects on failure."""
        if not sel:
            return good
        apply(good + sel)
        ok, msg = verify_build(proj, funcs)
        if ok:
            return good + sel
        if len(sel) == 1:
            st.setdefault(sel[0][0], {})[tag] = "full-build"
            print(f"  {sel[0][2]}: breaks the full build, left out ({msg.splitlines()[-1].strip()})")
            return good
        half = len(sel) // 2
        return settle_full(sel[half:], settle_full(sel[:half], good))

    good = settle(cands, [])
    apply(good)
    if not good:
        apply([])
        jsave(P_STATE, st)
        print("  none survived in the real source files")
        return []
    good = settle_full(good, [])
    apply(good)
    if not good:
        apply([])
        jsave(P_STATE, st)
        print("  full build failed with every new match, all reverted")
        return None
    ok, msg = verify_build(proj, funcs)
    if not ok:
        apply([])
        jsave(P_STATE, st)
        print(f"  full build failed with the new matches, all reverted:\n{msg}")
        return None
    jsave(P_STATE, st)
    return good


# ---------------------------------------------------------------- file layout: cutting a segment into its files

DLABEL = re.compile(r"^\s*dlabel\s+(\w+)")
ROM_COMMENT = re.compile(r"/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]{8}\s")


RODATA_CACHE = "build/cache/rodata_syms.json"


def rodata_syms(seg):
    """[(rom, symbol, owners)] for the rodata splat put under asm/nonmatchings/<seg>/: owners is
    the function it was migrated into, else every function that refers to it.
    Cached in build/: parsing ~2800 .s files over Docker's Windows bind mount takes ~11 s, and the
    dashboard (so every `done`) needs it. The .s files only change when splat runs, which depends
    on the configs and symbol files, so those mtimes and the file list are the cache key."""
    files = sorted(glob.glob(f"asm/nonmatchings/{seg}/*.s"))
    inputs = sorted(glob.glob(f"{CFG}/**/*.yaml", recursive=True) + glob.glob(f"{CFG}/sym.*.txt"))
    key = hashlib.sha1(json.dumps([files, [(p, os.path.getmtime(p)) for p in inputs]]).encode()).hexdigest()
    cache = jload(RODATA_CACHE, {})
    if cache.get(seg, {}).get("key") == key:
        return [(rom, sym, owners) for rom, sym, owners in cache[seg]["syms"]]
    out = _rodata_syms(seg, files)
    cache[seg] = {"key": key, "syms": out}
    jsave(RODATA_CACHE, cache)
    return out


def _rodata_syms(seg, files):
    syms, ftext = [], {}
    for p in files:
        name = os.path.splitext(os.path.basename(p))[0]
        text = read(p)
        is_func = re.search(r"^glabel\s", text, re.M) is not None
        if is_func:
            ftext[name] = text
        in_ro, cur = False, None
        for line in text.splitlines():
            s = line.strip()
            if s.startswith(".section"):
                in_ro = "rodata" in s
                continue
            m = DLABEL.match(s)
            if m and in_ro:
                cur = m.group(1)
                continue
            mm = ROM_COMMENT.search(line) if cur else None
            if mm:
                syms.append([int(mm.group(1), 16), cur, [name] if is_func else None])
                cur = None
    for s in syms:
        if s[2] is None:
            rx = re.compile(rf"\b{re.escape(s[1])}\b")
            s[2] = sorted(f for f, t in ftext.items() if rx.search(t))
    return sorted(tuple(s) for s in syms)


def plan_rodata(seg, file_of, order):
    """Rodata subsegment starts [(rom, file)] for cutting seg's .rodata the way its text is cut.
    file_of: function name -> new file; order: files in link order. Rodata keeps link order, so
    a symbol whose owner comes earlier than the current file stays where it is (noted)."""
    idx = {f: i for i, f in enumerate(order)}
    out, cur, conflicts = [], None, []
    for rom, sym, owners in rodata_syms(seg):
        files = {file_of[o] for o in owners if o in file_of}
        want = files.pop() if len(files) == 1 else None
        if want is None or (cur is not None and idx[want] < idx[cur]):
            if want is not None:
                conflicts.append(sym)
            if cur is None:                     # nothing claimed yet: the first file with rodata
                cur = order[0]
                out.append((rom, cur))
            continue
        if want != cur:
            cur = want
            out.append((rom, cur))
    return out, conflicts


def yaml_replace_line(text, entry, new_lines):
    """Replace the subsegment line `- [entry...]` (same indentation, trailing comment dropped)."""
    rx = re.compile(r"^(\s*)- \[" + re.escape(entry) + r"\][^\n]*$", re.M)
    m = rx.search(text)
    if not m:
        die(f"subsegment [{entry}] not found in the yaml")
    ind = m.group(1)
    return text[:m.start()] + "\n".join(f"{ind}- [{l}]" for l in new_lines) + text[m.end():]


def resplit(yaml_path, edits, old_segs, kinds, asm_names=()):
    """Re-cut segments: rewrite yaml subsegment lines (edits: [(old entry, [new entries])]),
    regenerate the C files with splat, carry every matched function's C across, rebuild and
    compare. kinds: new file name -> "c" or "asm"; asm_names: functions that now live in .s files.
    Rolls everything back if the build differs."""
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms = sym_names()
    old_srcs = {p: read(p) for s in old_segs for p in [f"src/{s}.c"] if os.path.exists(p)}
    snap = {p: read(p) for p in [yaml_path, P_FUNCS, P_STATE]}
    incs = []
    for t in old_srcs.values():
        incs += [l for l in t.splitlines() if l.startswith("#include") and l not in incs]
    # matched C, carried by name into whichever new file now has its INCLUDE_ASM
    carry = {}
    for k, e in funcs.items():
        if e["seg"] in old_segs and stat[k] == "matched":
            name = cur_name(k, e, syms)
            src = e.get("src")
            body = def_with_decls(old_srcs.get(src, ""), name) if src in old_srcs else None
            if body:
                carry[name] = body
    text = read(yaml_path)
    for entry, lines in edits:
        text = yaml_replace_line(text, entry, lines)
    write(yaml_path, text)
    for p in old_srcs:
        os.remove(p)
    for s in old_segs:
        shutil.rmtree(f"asm/nonmatchings/{s}", ignore_errors=True)

    def rollback(why):
        for p in glob.glob("src/**/*.c", recursive=True):
            p = p.replace("\\", "/")
            if any(p == f"src/{n}.c" for n in kinds) and p not in old_srcs:
                os.remove(p)
        for n in kinds:
            shutil.rmtree(f"asm/nonmatchings/{n}", ignore_errors=True)
        for p, t in list(old_srcs.items()) + list(snap.items()):
            write(p, t)
        run(["python3", "-m", "splat", "split", yaml_path])
        die(f"re-split rolled back: {why}")

    r = run(["python3", "-m", "splat", "split", yaml_path])
    if r.returncode:
        rollback("splat failed:\n" + tail(r.stdout + r.stderr, 15))
    placed = set()
    for n, kind in kinds.items():
        p = f"src/{n}.c"
        if kind != "c" or not os.path.exists(p):
            continue
        t = read(p)
        body = [l for l in t.splitlines() if not l.startswith("#include")]
        t = "\n".join(incs + body).rstrip("\n") + "\n"
        for m in list(INC_ASM.finditer(t)):
            if m.group(2) in carry:
                t = t.replace(m.group(0), carry[m.group(2)].rstrip("\n"), 1)
                placed.add(m.group(2))
        write(p, t)
        # splat writes empty functions as C itself: already there
        placed |= {n for n in carry if n not in placed and func_def_regex(n).search(t)}
    lost = sorted(set(carry) - placed - set(asm_names))
    if lost:
        rollback("matched functions with no INCLUDE_ASM to replace in the new files: " + ", ".join(lost[:10]))
    run(["python3", "tools/scripts/decomp.py", "index"])
    funcs = need_funcs()
    for k, e in funcs.items():
        if e["seg"] in kinds and e.get("src") and not os.path.exists(e["src"]):
            e["src"] = f"src/{e['seg']}.c"
    jsave(P_FUNCS, funcs)
    ok, msg = verify_build(proj, funcs)
    if not ok:
        rollback(msg)
    return funcs


def lib_objects():
    """[(address, size, 'LIB/OBJ')] from configs/USA/psyq/objects.txt."""
    if not os.path.exists(f"{PSYQ_CFG}/objects.txt"):
        die("configs/USA/psyq/objects.txt missing - run tools/scripts/psyq_match.py ... --write")
    out = []
    for l in read(f"{PSYQ_CFG}/objects.txt").splitlines():
        if l.startswith("0x"):
            a, n, o = l.split()
            out.append((int(a, 16), int(n, 16), o))
    return out


def handwritten(e):
    """Asm a C compiler wouldn't produce: GTE/COP0 instructions spimdisasm tags as handwritten,
    BIOS call stubs (jr $t2), code that never returns through $ra."""
    if not os.path.exists(e.get("asm", "")):
        return False                    # splat wrote it as C (an empty function)
    t = read(e["asm"])
    return ("handwritten instruction" in t or re.search(r"\bjr\s+\$t\d", t) is not None
            or re.search(r"\bjr\s+\$ra", t) is None)


def obj_file_name(obj, taken):
    lib, o = obj.split("/")
    lib = lib.lower().replace(".lib", "").replace(".obj", "")
    o = o.lower().replace(".obj", "")
    n = f"lib/{lib}" if lib == o else f"lib/{lib}/{o}"
    base, i = n, 2
    while n in taken:
        n, i = f"{base}_{i}", i + 1
    taken.add(n)
    return n


def cmd_split_lib(a):
    """Cut the PsyQ segment into one file per library object (configs/USA/psyq/objects.txt), objects
    written in assembly as .s files, the rest as C files; .rodata cut to match."""
    proj, funcs = need_proj(), need_funcs()
    seg = a.seg
    fs = sorted((e["rom"], int(k.rpartition(":")[2], 16), k, e) for k, e in funcs.items() if e["seg"] == seg)
    if not fs:
        die(f"no functions in segment {seg} (already split?)")
    stat, _ = statuses(funcs)
    missing = [e["name"] for _, _, k, e in fs if not os.path.exists(e.get("asm", "")) and stat[k] != "matched"]
    if missing:
        die(f"{len(missing)} functions have no asm file (e.g. {missing[0]}): run `make split` and `index` first")
    objs = lib_objects()
    files, taken = [], set()                      # [(rom, name, [func names])]
    syms = sym_names()
    for rom, vram, k, e in fs:
        hit = next((o for o in objs if o[0] <= vram < o[0] + max(o[1], 4)), None)
        label = hit[2] if hit else None
        if files and files[-1][3] == label and (hit is None or files[-1][4] == hit[0]):
            files[-1][2].append((k, e))
            continue
        name = obj_file_name(label, taken) if label else obj_file_name(f"unknown/{vram:08x}", taken)
        files.append([rom, name, [(k, e)], label, hit[0] if hit else None])
    kinds = {f[1]: "asm" if any(handwritten(e) for _, e in f[2]) else "c" for f in files}
    file_of = {cur_name(k, e, syms): f[1] for f in files for k, e in f[2]}
    order = [f[1] for f in files]
    yaml_path = yaml_of(a.yaml or proj['name'])
    ytext = read(yaml_path)
    m = re.search(r"- \[(0x[0-9A-Fa-f]+), c, " + re.escape(seg) + r"\]", ytext)
    mr = re.search(r"- \[(0x[0-9A-Fa-f]+), \.rodata, " + re.escape(seg) + r"\]", ytext)
    if not m:
        die(f"no `[..., c, {seg}]` subsegment in {yaml_path}")
    text_lines = [f"0x{f[0]:X}, {kinds[f[1]]}, {f[1]}" for f in files]
    edits = [(f"{m.group(1)}, c, {seg}", text_lines)]
    nasm = sum(1 for v in kinds.values() if v == "asm")
    print(f"{seg}: {len(files)} files ({len(files) - nasm} C, {nasm} hand-written asm), "
          f"{sum(1 for f in files if f[3] is None)} unidentified")
    if mr:
        ro, conflicts = plan_rodata(seg, file_of, order)
        ro_lines = [f"0x{r:X}, {'.rodata' if kinds[n] == 'c' else 'rodata'}, {n if kinds[n] == 'c' else n + '_rodata'}"
                    for r, n in ro]
        if not ro or ro[0][0] != int(mr.group(1), 16):
            die(f"rodata plan doesn't start at the segment's .rodata (0x{mr.group(1)})")
        edits.append((f"{mr.group(1)}, .rodata, {seg}", ro_lines))
        print(f"  .rodata: {len(ro)} pieces" + (f"; {len(conflicts)} symbols used out of order, left in place: "
                                                 + ", ".join(conflicts[:6]) if conflicts else ""))
    if a.dry_run:
        print("\n".join("  " + l for l in text_lines[:a.dry_run]))
        return
    asm_names = {n for n, f in file_of.items() if kinds[f] == "asm"}
    funcs = resplit(yaml_path, edits, [seg], kinds, asm_names)
    # hand-written objects: their functions are now plain .s, kept as the original source
    for f in files:
        if kinds[f[1]] == "asm":
            for k, _ in f[2]:
                funcs[k].update(kind="asm", seg=f[1], asm=f"asm/{f[1]}.s")
                funcs[k].pop("src", None)
    jsave(P_FUNCS, funcs)
    print(f"split {seg} into {len(files)} files; full build matches")
    cmd_progress(a)


# ---------------------------------------------------------------- Sony's PsyQ objects, linked in place

PSYQ_LIBS = "ext/psyq47/lib"
PSYQ_SECTIONS = f"{PSYQ_CFG}/sections.ld"


def psyq_layout():
    """[(LIB/OBJ, file, text address, ok, text size, {section: (address or None, size)})]."""
    if not os.path.exists(f"{PSYQ_CFG}/layout.txt"):
        die("configs/USA/psyq/layout.txt missing - run tools/scripts/psyq_objs.py ext/psyq47/lib disc/<EXE> --write")
    out = []
    for l in read(f"{PSYQ_CFG}/layout.txt").splitlines():
        if l.startswith(("#", "=", "!")) or not l.strip():
            continue
        p = l.split()
        secs = {}
        for f in p[5:]:
            s, v = f.split("=")
            a, n = v.split("/")
            secs[s] = (None if a == "?" else int(a, 16), int(n, 16))
        out.append((p[0], p[1], int(p[2], 16), p[3] == "ok", int(p[4], 16), secs))
    return out


def archive_member(lib, stem):
    """The archive member name for an object (members keep the converter's case)."""
    r = run([X + "ar", "t", f"{PSYQ_LIBS}/{lib}.a"])
    for m in r.stdout.split():
        if m.lower() == f"{stem.lower()}.o":
            return m[:-2]
    return None


def cmd_link_psyq(a):
    """Link Sony's PsyQ objects in place of the library code: .text and .rdata as splat `lib`
    subsegments, .data/.bss pinned NOLOAD at their original addresses (configs/USA/psyq/sections.ld)
    so references resolve while the data blob still supplies the bytes."""
    proj, funcs = need_proj(), need_funcs()
    if not os.path.isdir(PSYQ_LIBS):
        die(f"{PSYQ_LIBS} missing: extract psyq-4.7-converted-full.7z into ext/psyq47 (see README)")
    yaml_path = yaml_of(proj['name'])
    text = read(yaml_path)
    lay = [r for r in psyq_layout() if r[3]]
    vram_off = int(proj["header"]["t_addr"], 16) - 0x800       # exe: rom = vram - this
    edits, linked, members = [], {}, {}
    for o, f, addr, ok, n, secs in lay:
        lib, stem = f.split("/")[0], os.path.splitext(os.path.basename(f))[0]
        mem = archive_member(lib, stem)
        if not mem:
            print(f"  {o}: no member {stem}.o in {lib}.a, left as is")
            continue
        rom = addr - vram_off
        m = re.search(rf"- \[0x{rom:X}, (c|asm), (lib/[\w/]+)\]", text)
        if m:
            text = text.replace(m.group(0), f"- [0x{rom:X}, lib, {lib}, {mem}, .text]", 1)
            linked[m.group(2)] = (lib, mem, o)
        else:
            # no symbol at the object's start, so splat ran its first function into the previous
            # object's file: cut a lib entry into that file's range (and resume the file after it)
            subs = [(int(x, 16), t, nm, mm.group(0)) for mm in re.finditer(
                r"- \[0x([0-9A-F]+), (c|asm|lib), ([^\]]+)\]", text) for x, t, nm in [mm.groups()]]
            prev = [x for x in subs if x[0] < rom][-1]
            nxt = next(x for x in subs if x[0] > rom)
            end = (rom + n + 15) & ~15 if nxt[0] > rom + n else rom + n
            new = [prev[3], f"- [0x{rom:X}, lib, {lib}, {mem}, .text]"]
            if nxt[0] > end and prev[1] != "lib":
                new.append(f"- [0x{end:X}, {prev[1]}, {prev[2]}_tail]")
            text = text.replace(prev[3], ("\n" + " " * 6).join(new), 1)
            # the rest of the object may already be a file of its own: the lib entry covers it
            for x, t, nm, line in subs:
                if rom < x < rom + n and t != "lib":
                    text = re.sub(r"\n[ \t]*" + re.escape(line) + r"[^\n]*", "", text, 1)
                    linked[nm] = (lib, mem, o)
            print(f"  {o}: cut out of {prev[2]}")
        members[o] = (lib, mem, secs)
    # .rdata: one entry per object in address order, replacing the rodata cut by owner
    rd = sorted((s[".rdata"][0], members[o][0], members[o][1]) for o, (_, _, s) in members.items()
                if ".rdata" in s and s[".rdata"][0] is not None)
    ro_lines = [l for l in text.splitlines() if re.search(r"- \[0x[0-9A-F]+, \.?rodata, lib/", l)]
    if ro_lines and rd:
        ind = ro_lines[0][:len(ro_lines[0]) - len(ro_lines[0].lstrip())]
        first = int(re.search(r"0x([0-9A-F]+)", ro_lines[0]).group(1), 16)
        if rd[0][0] - vram_off != first:
            die(f".rdata of the first object (0x{rd[0][0]:08X}) isn't where the library rodata starts")
        new = "\n".join(f"{ind}- [0x{v - vram_off:X}, lib, {lib}, {mem}, .rdata]" for v, lib, mem in rd)
        text = text.replace("\n".join(ro_lines), new, 1) if "\n".join(ro_lines) in text else die(
            "library rodata subsegments aren't contiguous in the yaml")
    if "lib_path:" not in text:
        text = text.replace("  asset_path: assets\n", f"  asset_path: assets\n  lib_path: {PSYQ_LIBS}\n", 1)
    # NOLOAD placement of the other sections
    ld = ["/* Sony PsyQ objects' data sections at their original addresses (decomp.py link-psyq).",
          " * NOLOAD: the bytes come from the exe's data; this only makes references resolve.",
          " * The Makefile splices these lines into splat's linker script, ahead of its /DISCARD/. */"]
    spare = 0x81000000                     # unreferenced sections: anywhere out of the way
    for o, (lib, mem, secs) in sorted(members.items(), key=lambda kv: kv[1][2].get(".data", (0,))[0] or 0):
        for s in (".data", ".bss", ".sbss", ".sdata"):
            if s not in secs or not secs[s][1]:
                continue
            at = secs[s][0]
            if at is None:
                at, spare = spare, spare + ((secs[s][1] + 15) & ~15)
            nm = f"psyq_{lib}_{mem}{s.replace('.', '_')}"
            ld.append(f"    {nm} 0x{at:08X} (NOLOAD) : SUBALIGN(1) {{ {PSYQ_LIBS}/{lib}.a:{mem}.o({s}) }}")
    # what the objects refer to outside themselves, at the addresses the exe uses
    for l in read(f"{PSYQ_CFG}/layout.txt").splitlines():
        if l.startswith("= "):
            _, sym, v = l.split()
            ld.append(f"    {sym} = {v};")
    ld += ["    /DISCARD/ : { " + " ".join(f"{PSYQ_LIBS}/{lib}.a:{mem}.o(.note)" for lib, mem, _ in members.values()) + " }",
           ""]
    # symbols the objects define somewhere other than where the exe uses them (bss allocated
    # by psylink symbol by symbol, labels the converter swapped): assigned last, so they win
    pins = [l.split()[1:] for l in read(f"{PSYQ_CFG}/layout.txt").splitlines() if l.startswith("! ")]
    write(f"{PSYQ_CFG}/symbols.ld", "/* Symbols pinned to the addresses the exe uses (decomp.py link-psyq). */\n"
          + "".join(f"{sym} = {v};\n" for sym, v in pins))
    snap = {p: read(p) for p in [yaml_path, P_FUNCS]}
    old_src = {p: read(p) for n in linked for p in [f"src/{n}.c"] if os.path.exists(p)}
    write(yaml_path, text)
    write(PSYQ_SECTIONS, "\n".join(ld))
    for p in old_src:
        os.remove(p)
    for n in linked:
        shutil.rmtree(f"asm/nonmatchings/{n}", ignore_errors=True)
        for p in (f"asm/{n}.s", f"asm/data/{n}_rodata.rodata.s"):
            if os.path.exists(p):
                os.remove(p)
    for p in glob.glob("asm/data/lib/**/*.s", recursive=True):
        os.remove(p)                       # rodata now comes from the objects
    r = run(["python3", "-m", "splat", "split", yaml_path])
    ok, msg = (False, "splat failed:\n" + tail(r.stdout + r.stderr)) if r.returncode else verify_build(proj, funcs)
    if not ok and a.keep:
        die("build differs (left in place, --keep):\n" + msg)
    if not ok:
        for p, t in list(snap.items()) + list(old_src.items()):
            write(p, t)
        os.remove(PSYQ_SECTIONS)
        run(["python3", "-m", "splat", "split", yaml_path])
        die("link-psyq undone, the build differs:\n" + msg)
    # their functions now come from Sony's objects
    for k, e in funcs.items():
        if e["seg"] in linked:
            e["kind"] = "sony"
            e["obj"] = linked[e["seg"]][2]
            e.pop("src", None)
    jsave(P_FUNCS, funcs)
    print(f"linked {len(linked)} Sony objects in place of their decompiled/asm files; full build matches")
    cmd_progress(a)


# ---------------------------------------------------------------- names

def yamls_seeing(ovl, proj):
    """Configs whose splat run uses the names of `ovl` ('' = the exe): every config sees the
    exe's names, the slot overlays see the engine's, and a slot overlay's names only itself."""
    allo = [o["name"] for o in proj.get("overlays", [])]
    if not ovl:
        return [proj["name"]] + allo
    if ovl == "engine":
        return allo
    return [ovl]


def ensure_sym_path(ovl, proj):
    """Create configs/USA/sym.<ovl>.txt and list it in every config that should see it."""
    p = sym_file(ovl)
    if not os.path.exists(p):
        write(p, f"// Names in {ovl} (its load addresses), written by `decomp.py rename`.\n")
    for y in yamls_seeing(ovl, proj):
        yp = yaml_of(y)
        lines = read(yp).splitlines(keepends=True)
        if any(p in l for l in lines):
            continue
        i = next(i for i, l in enumerate(lines) if l.strip() == "symbol_addrs_path:")
        j = i + 1
        while j < len(lines) and lines[j].lstrip().startswith("- "):
            j += 1
        ind = lines[j - 1][:len(lines[j - 1]) - len(lines[j - 1].lstrip())]
        lines.insert(j, f"{ind}- {p}\n")
        write(yp, "".join(lines))


def set_symbol(ovl, addr, name, typ=None):
    p = sym_file(ovl)
    t = read(p) if os.path.exists(p) else ""
    rx = re.compile(rf"^\s*(\w+)\s*=\s*0x0*{addr:X}\s*;.*\n?", re.M | re.I)
    t = rx.sub("", t)
    t = t.rstrip("\n") + "\n" + f"{name} = 0x{addr:08X};" + (f" // type:{typ}" if typ else "") + "\n"
    write(p, t)


def ref_files(ovl):
    """(sources to rename in, headers to rename in). A slot overlay's names are only its own:
    other overlays may use the same placeholder for a different thing at the same address, so
    only its sources and the headers they own are rewritten."""
    srcs = [p.replace("\\", "/") for p in glob.glob("src/**/*.c", recursive=True)]
    if ovl in ("", "engine"):
        return srcs, all_headers()
    mine = [p for p in srcs if p == f"src/{ovl}.c" or p.startswith((f"src/{ovl}/", f"src/overlays/{ovl}/"))]
    return mine, [h for h in all_headers() if header_owner(h) in mine]


def apply_renames(pairs, proj, write_syms=True):
    """pairs: [(ovl, addr, old, new, typ)]. Writes the names, rewrites references in src/ and
    include/, re-splits, re-indexes and checks the build; undoes everything if it differs."""
    snap = {p: read(p) for p in glob.glob(f"{CFG}/**/*", recursive=True) + all_headers()
            + glob.glob("src/**/*.c", recursive=True) + [P_FUNCS] if os.path.isfile(p)}
    created = set()
    yamls = set()
    by_scope = defaultdict(dict)
    for ovl, addr, old, new, typ in pairs:
        if write_syms:
            if ovl:
                if not os.path.exists(sym_file(ovl)):
                    created.add(sym_file(ovl))
                ensure_sym_path(ovl, proj)
            set_symbol(ovl, addr, new, typ)
        by_scope[ovl][old] = new
        yamls |= set(yamls_seeing(ovl, proj))
        for f in glob.glob(f"asm/nonmatchings/**/{old}.s", recursive=True):
            os.remove(f)                          # splat writes the new name; don't index both
    for ovl, mp in by_scope.items():
        srcs, hdrs = ref_files(ovl)
        for p in srcs + hdrs:
            t = read(p)
            n = rename_syms(t, mp)
            if n != t:
                write(p, n)
        # a new name the original headers already declare keeps only that declaration
        for p in srcs:
            if any(header_owner(h) == p for h in all_headers()):
                dedupe_owned(p)

    def resplit_all():
        for y in sorted(yamls):
            r = run(["python3", "-m", "splat", "split", yaml_of(y)])
            if r.returncode:
                return f"splat failed on {y}:\n" + tail(r.stdout + r.stderr, 10)
        r = run(["python3", "tools/scripts/decomp.py", "index"])
        return None if r.returncode == 0 else "index failed:\n" + tail(r.stdout + r.stderr)

    err = resplit_all()
    ok, msg = (False, err) if err else verify_build(proj, need_funcs())
    if not ok:
        for p, t in snap.items():
            write(p, t)
        for p in created:
            os.remove(p)
        for _, _, old, new, _ in pairs:
            for f in glob.glob(f"asm/nonmatchings/**/{new}.s", recursive=True):
                os.remove(f)
        resplit_all()
        die("rename undone, the build no longer matched:\n" + msg)
    return len(pairs)


def resolve_symbol(spec, funcs, syms, ovl_hint=None):
    """(ovl, addr, current name, type) for a function (NAME, SEG/NAME, key) or a data symbol
    (D_XXXXXXXX, name in a symbol file; overlay data needs --ovl)."""
    try:
        key, e = find_func(funcs, spec)
        ovl, _, a = key.rpartition(":")
        return ovl, int(a, 16), cur_name(key, e, syms), "func"
    except SystemExit:
        pass
    m = re.fullmatch(r"(?:(\w+):)?(?:D_|0x)?([0-9A-Fa-f]{8})", spec)
    if m:
        return (m.group(1) or ovl_hint or ""), int(m.group(2), 16), f"D_{m.group(2).upper()}", None
    hits = [(k, n) for k, n in syms.items() if n == spec]
    if len(hits) == 1:
        o, _, a = hits[0][0].rpartition(":")
        return o, int(a, 16), spec, None
    die(f"can't resolve {spec} (give OVERLAY:ADDRESS for overlay data)")


def cmd_rename(a):
    proj, funcs = need_proj(), need_funcs()
    syms = sym_names()
    pairs = []
    if a.sync:
        # names already in the symbol files that the sources don't use yet (e.g. after psyq_match)
        inc = scan_src()
        for k, e in funcs.items():
            want = cur_name(k, e, syms)
            for nm in {e["name"], e["orig"]} - {want}:
                if (e["seg"], nm) in inc:
                    pairs.append((k.rpartition(":")[0], int(k.rpartition(":")[2], 16), nm, want, "func"))
        if not pairs:
            print("names in sync")
            return
        apply_renames(pairs, proj, write_syms=False)
        print(f"synced {len(pairs)} names; full build matches")
        return
    items = list(zip(a.names[::2], a.names[1::2]))
    if a.file:
        for l in read(a.file).splitlines():
            l = l.split("#", 1)[0].strip()
            if l:
                items.append(tuple(l.split()[:2]))
    if not items or len(a.names) % 2:
        die("give OLD NEW pairs (or --file with one `OLD NEW` per line)")
    for old, new in items:
        if not re.fullmatch(r"[A-Za-z_]\w*", new):
            die(f"{new} is not a C identifier")
        ovl, addr, cur, typ = resolve_symbol(old, funcs, syms, a.ovl)
        pairs.append((ovl, addr, cur, new, typ))
    n = apply_renames(pairs, proj)
    print(f"renamed {n}; full build matches")
    cmd_progress(a)


# ---------------------------------------------------------------- duplicates: one match, many copies

def groups_by(funcs, field="fp"):
    """fingerprint -> keys, for fingerprints shared by more than one function."""
    g = defaultdict(list)
    for k, e in funcs.items():
        if e.get(field) and e["size"] > 8:          # `jr ra; nop` stubs are all alike: not a copy
            g[e[field]].append(k)
    return {h: sorted(ks) for h, ks in g.items() if len(ks) > 1}


def sym_map(src_e, dst_e, src_name, dst_name):
    """Symbol renames that turn src's C into dst's: aligned relocations must agree on offsets."""
    t1, t2 = parse_asm(src_e["asm"])[3], parse_asm(dst_e["asm"])[3]
    if len(t1) != len(t2):
        return None
    mp = {src_name: dst_name}
    for a, b in zip(t1, t2):
        sa, sb = insn_syms(a), insn_syms(b)
        if len(sa) != len(sb):
            return None
        for (na, oa), (nb, ob) in zip(sa, sb):
            if oa != ob or mp.setdefault(na, nb) != nb:
                return None
    return {a: b for a, b in mp.items() if a != b}


def def_with_decls(text, name):
    """A function's definition plus the declaration lines right above it (m2c's `/* extern */`
    prototypes), as they'd need to travel with it."""
    m = func_def_regex(name).search(text)
    body = extract_function(text, name)
    if not m or not body:
        return None
    lines, pre = text[:m.start()].splitlines(), []
    while lines:
        l = lines.pop()
        if not l.strip():
            continue
        if FUNC_DECL_LINE.match(l) or l.startswith("extern "):
            pre.insert(0, l)
            continue
        break
    return "\n".join(pre + ([""] if pre else []) + [body.rstrip("\n")]) + "\n"


def rename_syms(text, mp):
    if not mp:
        return text
    rx = re.compile(r"\b(" + "|".join(sorted(map(re.escape, mp), key=len, reverse=True)) + r")\b")
    return rx.sub(lambda m: mp[m.group(1)], text)


def header_decls(mp):
    """Declarations (externs and prototypes) for renamed symbols, copied from how the source
    symbols are declared. declare() later skips any the copy's file can already see."""
    lines = [l for p in all_headers() for l in read(p).splitlines()]
    out = []
    for a, b in mp.items():
        for l in lines:
            if decl_name(l) == a:
                out.append(rename_syms(l.strip(), {a: b}))
                break
    return out


def propagate_candidates(funcs, stat, syms, only=None, st=None, proj=None):
    """(key, entry, name, src, decls, ftext) for every todo function whose exact twin is matched.
    Copies that already failed from the same original (recorded in st) are skipped."""
    out, texts = [], {}
    st = st or {}
    for keys in groups_by(funcs).values():
        done = [k for k in keys if stat.get(k) == "matched" and (only is None or k in only)]
        todo = [k for k in keys if stat.get(k) in ("todo", "in_progress", "blocked")]
        if not done or not todo:
            continue
        sk = done[0]
        se, sname = funcs[sk], cur_name(sk, funcs[sk], syms)
        ssrc = se.get("src") or locate_def(sname)
        if not ssrc:
            continue
        if ssrc not in texts:
            texts[ssrc] = read(ssrc)
        base = def_with_decls(texts[ssrc], sname)
        if not base:
            continue
        for k in todo:
            if st.get(k, {}).get("copy_failed") == sk:
                continue
            # game code and library code that compile alike are a coincidence, not a copy
            if proj and is_lib(funcs[k], proj) != is_lib(se, proj):
                continue
            e = funcs[k]
            name = cur_name(k, e, syms)
            src = e.get("src")
            mp = sym_map(se, e, sname, name) if src and os.path.exists(e.get("asm", "")) else None
            if mp is None:
                continue
            # the copy must see the same headers as the original (except the headers another
            # file owns: what it needs from those comes in through header_decls)
            for inc in src_includes(ssrc):
                hm = re.search(r'"([^"]+)"', inc)
                if hm and not (os.path.exists(f"{HEADERS}/{hm.group(1)}")
                               and header_owner(f"{HEADERS}/{hm.group(1)}") not in (None, src)):
                    ensure_include(src, f"{HEADERS}/{hm.group(1)}")
            out.append((k, e, name, src, header_decls(mp), rename_syms(base, mp), sk))
    return out


def propagate(funcs, proj, only=None):
    """Land the copies of matched functions (repeated while new matches open up more).
    Returns what landed; each is recorded as matched."""
    landed = []
    syms = sym_names()
    while True:
        stat, st = statuses(funcs)
        cands = propagate_candidates(funcs, stat, syms, only, st, proj)
        if not cands:
            break
        print(f"propagate: {len(cands)} copies of matched functions")
        origin = {c[0]: c[6] for c in cands}
        good = land([c[:6] for c in cands], funcs, proj, st, tag="copy_failed")
        st = jload(P_STATE, {})
        for k in origin:                              # remember which original didn't fit here
            if k not in {g[0] for g in good or []}:
                st.setdefault(k, {})["copy_failed"] = origin[k]
        jsave(P_STATE, st)
        if not good:
            break
        record_matches([(k, e, n) for k, e, n, *_ in good], funcs, via="copy")
        landed += good
        only = {c[0] for c in good}                   # their own twins are already covered
    return landed


def cmd_propagate(a):
    proj, funcs = need_proj(), need_funcs()
    got = propagate(funcs, proj)
    if not got:
        print("propagate: nothing to copy")
        return
    cmd_progress(a)
    if a.commit:
        commit_match([(k, e, n) for k, e, n, *_ in got], a.trailer or [],
                     title=f"Copy {len(got)} matched functions to their duplicates "
                           f"({sum(c[1]['size'] for c in got)} bytes)")


def call_graph(funcs):
    """key -> callee keys, and key -> caller keys. A call from an overlay resolves to that
    overlay first, then the engine, then the exe (slot overlays share one load address)."""
    by_ovl = defaultdict(dict)
    for k in funcs:
        o, _, addr = k.rpartition(":")
        by_ovl[o][addr] = k
    callees, callers = {}, defaultdict(set)
    for k, e in funcs.items():
        o = k.rpartition(":")[0]
        out = set()
        for c in e.get("calls", []):
            t = by_ovl[o].get(c) or by_ovl["engine"].get(c) or by_ovl[""].get(c)
            if t:
                out.add(t)
                callers[t].add(k)
        callees[k] = out
    return callees, callers


def cmd_dups(a):
    funcs = need_funcs()
    if not any("fp" in e for e in funcs.values()):
        die("no fingerprints - re-run `decomp.py index`")
    stat, _ = statuses(funcs)
    syms = sym_names()
    for field, label in (("fp", "identical (same C, symbols renamed)"), ("fpn", "near-identical (constants differ)")):
        g = groups_by(funcs, field)
        if field == "fpn":                           # near groups that aren't already exact groups
            ex = {tuple(v) for v in groups_by(funcs, "fp").values()}
            g = {h: v for h, v in g.items() if tuple(v) not in ex}
        n = sum(len(v) for v in g.values())
        extra = sum(funcs[v[0]]["size"] * (len(v) - 1) for v in g.values())
        ready = sum(1 for v in g.values() if any(stat[k] == "matched" for k in v) and any(stat[k] != "matched" for k in v))
        print(f"{label}: {len(g)} groups, {n} functions, {extra} bytes beyond the first of each"
              + (f"; {ready} groups have a matched member to copy from" if field == "fp" else ""))
        if a.list:
            for v in sorted(g.values(), key=lambda v: -funcs[v[0]]["size"] * len(v))[:a.list]:
                print(f"  {funcs[v[0]]['size']:5d} B x{len(v)}: " + ", ".join(
                    f"{funcs[k]['seg']}/{cur_name(k, funcs[k], syms)}{'*' if stat[k] == 'matched' else ''}" for k in v))


def cmd_block(a):
    funcs = need_funcs()
    key, e = find_func(funcs, a.func)
    st = jload(P_STATE, {})
    rec = st.setdefault(key, {})
    rec.update(status="blocked", note=a.note, blocked_at=now())
    jsave(P_STATE, st)
    print(f"parked {e['name']}: {a.note}")
    cmd_progress(a)  # the dashboard counts parked functions


def permute_run(d, name, e, base_c, cfg, jobs, seconds):
    """Set up and run decomp-permuter in `d` on base_c for `seconds`.
    Returns (best score or None, best output dir or None, base score or None, log)."""
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    write(f"{d}/base.c", base_c)
    write(f"{d}/target.s", f'.include "macro.inc"\n.set noat\n.set noreorder\n.text\n.include "{e["asm"]}"\n')
    r = run([X + "as", *ASFLAGS, "-o", f"{d}/target.o", f"{d}/target.s"])
    if r.returncode:
        return None, None, None, "could not assemble target:\n" + tail(r.stderr)
    g = int(cfg.get("GVAL", "0"))
    cmd = pipeline('"$1"', '"$3"', cfg["CC_VER"], cfg["OPT_FLAGS"], g,
                   re.sub(r"\s*-G\d+", "", cfg.get("MASPSX_ARGS", DEFAULT_MASPSX)))
    cmd = cmd.replace("-Iinclude", "-I/work/include").replace("-Iext/", "-I/work/ext/")
    write(f"{d}/compile.sh", f"#!/bin/bash\nset -eo pipefail\ncd /work\n{cmd}\n")
    os.chmod(f"{d}/compile.sh", 0o755)
    write(f"{d}/settings.toml", f'func_name = "{name}"\ncompiler_type = "gcc"\n')
    # The permuter looks for mips-linux-gnu-objdump; the image has the mipsel- build of the same tool.
    shim = os.path.abspath("build/permuter/bin")
    os.makedirs(shim, exist_ok=True)
    if not os.path.exists(f"{shim}/mips-linux-gnu-objdump"):
        real = shutil.which(X + "objdump")
        if real:
            os.symlink(real, f"{shim}/mips-linux-gnu-objdump")
    env = dict(os.environ, PATH=shim + os.pathsep + os.environ.get("PATH", ""))
    # own process group, so a timeout also stops the permuter's worker processes
    p = subprocess.Popen(["python3", PERMUTER, d, f"-j{jobs}", "--stop-on-zero"], stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT, text=True, start_new_session=True, env=env)
    try:
        log = p.communicate(timeout=seconds)[0]
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, 9)
        log = p.communicate()[0] or ""
    write(f"{d}/log.txt", log)
    m = re.search(r"base score\D*(\d+)", log, re.I)
    outs = []
    for q in glob.glob(f"{d}/output-*"):
        mm = re.match(r"output-(\d+)-", os.path.basename(q))
        if mm:
            outs.append((int(mm.group(1)), q))
    best, path = min(outs) if outs else (None, None)
    return best, path, (int(m.group(1)) if m else None), log


def cmd_permute(a):
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    cfg = mk_config()
    src = e.get("src") or locate_def(name)
    body = extract_function(read(src), name) if src else None
    if not body:
        die(f"no C definition of {name} found - write a first attempt in its source file")
    if not os.path.exists(e.get("asm", "")):
        die("function asm missing - run `make split`")
    d = f"build/permuter/{name}"
    jobs = a.jobs or max(1, (os.cpu_count() or 2) - 1)
    base_c = permuter_base(src, [], body, name)
    if base_c is None:
        die(f"could not preprocess {name} for the permuter - does `try {name}` build?")
    best, path, base, log = permute_run(d, name, e, base_c, cfg, jobs, a.minutes * 60)
    if path is None:
        print(f"permuter found no improvement{f' (base score {base})' if base is not None else ''}; log {d}/log.txt")
        if base is None:
            print(tail(log, 6))
        return
    print(f"permuter best score {best}{f' (base {base})' if base is not None else ''}: {path}/source.c"
          + ("  <- MATCH, apply this function's changes and run try" if best == 0 else ""))
    if os.path.exists(f"{path}/diff.txt"):
        print(f"changes vs base: {path}/diff.txt")


# ---------------------------------------------------------------- progress + dashboard

PLACEHOLDER_NAME = re.compile(r"^(func|sub|fn)_[0-9A-Fa-f]{8}$")


# raw offset arithmetic m2c leaves behind: a matched function with this is not typed yet
UNTYPED = re.compile(r"\(\s*(?:u8|s8|char)\s*\*\s*\)\s*\(?\s*\w+\s*\)?\s*\+|M2C_FIELD")


def unit_meta(unit, proj):
    """(title, roadmap phase) for a dashboard row, from project.json: the exe is phase 2, and each
    overlay entry may carry "title" and "phase" (the roadmap phase that finishes it)."""
    if unit == "main exe":
        return proj.get("title", "main exe"), proj.get("phase", 2)
    o = next((o for o in proj.get("overlays", []) if o["name"] == unit), {})
    return o.get("title", ""), o.get("phase")


def unit_of(e, proj):
    """Dashboard row a function counts under: its PsyQ library, or the binary it is in."""
    if is_lib(e, proj):
        parts = e["seg"].split("/")
        return parts[1] if len(parts) > 2 else parts[-1]
    return os.path.splitext(os.path.basename(e.get("bin") or proj["exe"]))[0] if e.get("bin") else "main exe"


def summary(funcs, stat, proj):
    """Bytes matched per group and per unit. Functions written in assembly originally (kind asm)
    are recovered as .s and counted apart from the C totals; names and copies are tracked too."""
    syms = sym_names()
    blank = lambda: {"done_b": 0, "total_b": 0, "done_f": 0, "total_f": 0, "asm_b": 0, "asm_f": 0,
                     "named_f": 0, "all_f": 0, "blocked": 0, "sony_b": 0, "c_f": 0, "typed_f": 0}
    texts = {}

    def typed(k, e):
        """None if the function has no C of its own, else whether its C is free of raw offsets."""
        src = e.get("src")
        if not src or not os.path.exists(src):
            return None
        if src not in texts:
            texts[src] = read(src)
        body = extract_function(texts[src], cur_name(k, e, syms))
        return None if body is None else not UNTYPED.search(body)
    grp = {g: blank() for g in ("game", "lib")}
    units = {}
    twins = defaultdict(list)
    for k, e in funcs.items():
        if e.get("fp") and e["size"] > 8 and stat[k] not in ("matched", "asm", "sony", "unsplit"):
            twins[e["fp"]].append(e["size"])
    for k, e in funcs.items():
        if stat[k] == "unsplit":
            continue
        g = "lib" if is_lib(e, proj) else "game"
        un = unit_of(e, proj)
        u = units.get(un)
        if u is None:
            title, phase = unit_meta(un, proj) if g == "game" else ("", None)
            u = units[un] = dict(blank(), unit=un, lib=g == "lib", files=set(), title=title, phase=phase,
                                 file=os.path.basename(e.get("bin") or proj["exe"]))
        ty = typed(k, e) if stat[k] == "matched" and g == "game" else None
        u["files"].add(e["seg"])
        for t in (grp[g], u):
            t["all_f"] += 1
            t["named_f"] += not PLACEHOLDER_NAME.match(cur_name(k, e, syms))
            t["blocked"] += stat[k] == "blocked"
            if stat[k] == "asm":
                t["asm_b"] += e["size"]
                t["asm_f"] += 1
                continue
            t["total_b"] += e["size"]
            t["total_f"] += 1
            if stat[k] == "sony":
                t["sony_b"] += e["size"]
            if stat[k] in ("matched", "sony"):
                t["done_b"] += e["size"]
                t["done_f"] += 1
            if ty is not None:
                t["c_f"] += 1
                t["typed_f"] += ty
    for t in list(grp.values()) + list(units.values()):
        t["pct"] = 100.0 * t["done_b"] / t["total_b"] if t["total_b"] else 0.0
    for u in units.values():
        u["files"] = len(u["files"])
    copies = sum(sum(v[1:]) for v in twins.values())
    return {"game": grp["game"], "lib": grp["lib"], "units": sorted(units.values(), key=lambda s: (s["lib"], s["unit"])),
            "blocked": sum(1 for v in stat.values() if v == "blocked"),
            "unsplit": sum(1 for v in stat.values() if v == "unsplit"),
            "copies_b": copies, "data": data_summary(funcs, stat, proj)}


CODE_TYPES = {"c", "asm", "hasm"}
DATA_TYPES = {"data", ".data", "rodata", ".rodata", "sdata", ".sdata", "bss", ".bss", "sbss", ".sbss",
              "bin", "textbin"}


def subsegments(yaml_path):
    """[(start, end, type, name)] for every subsegment of a splat yaml, ends filled in."""
    import yaml
    y = yaml.safe_load(read(yaml_path))
    segs = y.get("segments", [])
    starts = [s["start"] if isinstance(s, dict) else s[0] for s in segs]
    out = []
    for i, s in enumerate(segs):
        if not isinstance(s, dict) or "subsegments" not in s:
            continue
        end = next((x for x in starts[i + 1:] if x > s["start"]), None)
        subs = [x for x in s["subsegments"] if isinstance(x, list) and len(x) >= 2]
        for j, x in enumerate(subs):
            nxt = subs[j + 1][0] if j + 1 < len(subs) else end
            out.append((x[0], nxt, x[1], str(x[2]) if len(x) > 2 else str(x[1])))
    return out


def data_summary(funcs, stat, proj):
    """Data bytes (everything outside code subsegments) and how much of it is produced by C:
    rodata migrated into matched functions, plus data subsegments owned by a C file (.data etc.)
    with nothing left in asm."""
    syms = sym_names()  # once: inside the comprehension it re-read every symbol file per function
    matched = {cur_name(k, e, syms) for k, e in funcs.items() if stat.get(k) == "matched"}
    total = done = 0
    yamls = [yaml_of(proj['name'])] + [yaml_of(o['name']) for o in proj.get("overlays", [])]
    for y in yamls:
        if not os.path.exists(y):
            continue
        for start, end, typ, name in subsegments(y):
            if typ not in DATA_TYPES or end is None:
                continue
            total += end - start
            if typ == ".rodata":
                syms = [s for s in rodata_syms(name) if start <= s[0] < end]
                for i, (rom, sym, owners) in enumerate(syms):
                    nxt = syms[i + 1][0] if i + 1 < len(syms) else end
                    if len(owners) == 1 and owners[0] in matched and not os.path.exists(
                            f"asm/nonmatchings/{name}/{sym}.s"):
                        done += nxt - rom
            elif typ.startswith(".") and not glob.glob(f"asm/nonmatchings/{name}/D_*.s"):
                done += end - start
    return {"done_b": done, "total_b": total, "pct": 100.0 * done / total if total else 0.0}


P_ROADMAP = "docs/ROADMAP.md"


def roadmap():
    """Phases of docs/ROADMAP.md: [{title, done_when, items: [{done, text}]}]."""
    if not os.path.exists(P_ROADMAP):
        return []
    phases, cur, item = [], None, None
    for line in read(P_ROADMAP).splitlines():
        if line.startswith("## "):
            cur = {"title": line[3:].strip(), "done_when": "", "items": []}
            phases.append(cur)
            item = None
            continue
        if cur is None:
            continue
        m = re.match(r"^- \[([ xX])\] (.*)$", line)
        if m:
            item = {"done": m.group(1) != " ", "text": m.group(2).strip()}
            cur["items"].append(item)
        elif item and line.startswith("  ") and line.strip():
            item["text"] += " " + line.strip()
        elif line.startswith("**Done when**"):
            cur["done_when"] = line[len("**Done when**"):].strip()
            item = "dw"
        elif item == "dw" and line.strip():
            cur["done_when"] += " " + line.strip()
        else:
            item = None if not line.strip() else item
    return phases


def current_phase(phases):
    """Index of the first phase with an unticked item, or None when all are done."""
    return next((i for i, ph in enumerate(phases) if any(not it["done"] for it in ph["items"])), None)


def write_badges(g, l):
    """docs/badges/*.json in shields.io endpoint format, so the README shows live figures without
    hard-coded numbers. Game code is the headline; Sony's linked objects are reported apart."""
    def badge(fname, label, msg, pct):
        color = "brightgreen" if pct >= 90 else "green" if pct >= 50 else "yellow" if pct >= 10 else "orange"
        jsave(f"docs/badges/{fname}.json", {"schemaVersion": 1, "label": label, "message": msg, "color": color})
    os.makedirs("docs/badges", exist_ok=True)
    badge("code", "game code", f"{g['pct']:.2f}%", g["pct"])
    fp = 100.0 * g["done_f"] / g["total_f"] if g["total_f"] else 0.0
    badge("functions", "functions", f"{g['done_f']}/{g['total_f']}", fp)
    badge("psyq", "PsyQ", f"{l['pct']:.2f}%", l["pct"])
    phases = roadmap()
    i = current_phase(phases)
    if phases:
        if i is None:
            badge("roadmap", "roadmap", "complete", 100.0)
        else:
            its = phases[i]["items"]
            done = sum(it["done"] for it in its)
            badge("roadmap", "roadmap", f"{phases[i]['title'].split(' - ')[0]} · {done}/{len(its)}",
                  100.0 * done / len(its))


def backfill_units(hist, funcs, stat, st, proj):
    """Per-unit figures for history points recorded before they were kept: a game function counts
    from its matched_at (untimed matches from the start, overlays once they were indexed), and
    Sony's objects from the first point with the library linked."""
    old = [h for h in hist if "units" not in h]
    if not old:
        return
    done = [(st.get(k, {}).get("matched_at"), unit_of(e, proj), e["size"], is_lib(e, proj))
            for k, e in funcs.items() if stat[k] in ("matched", "sony")]
    exe_f = sum(1 for e in funcs.values() if not is_lib(e, proj) and unit_of(e, proj) == "main exe")
    for h in old:
        u = defaultdict(lambda: [0, 0])
        for at, un, size, lib in done:
            if lib:
                ok = h.get("lib_b", 0) > 0
            elif at:
                ok = at <= h["ts"]
            else:
                ok = un == "main exe" or h.get("game_all_f", 0) > exe_f
            if ok:
                u[un][0] += size
                u[un][1] += 1
        h["units"] = dict(u)


def cmd_progress(a):
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms = sym_names()
    s = summary(funcs, stat, proj)
    commit = run(["git", "rev-parse", "--short", "HEAD"]).stdout.strip() or None
    point = {"ts": now(), "commit": commit,
             "game_b": s["game"]["done_b"], "game_tb": s["game"]["total_b"], "game_f": s["game"]["done_f"],
             "lib_b": s["lib"]["done_b"], "lib_tb": s["lib"]["total_b"], "lib_f": s["lib"]["done_f"],
             "game_named_f": s["game"]["named_f"], "game_all_f": s["game"]["all_f"],
             "game_c_f": s["game"]["c_f"], "game_typed_f": s["game"]["typed_f"],
             "data_b": s["data"]["done_b"], "data_tb": s["data"]["total_b"],
             "units": {u["unit"]: [u["done_b"], u["done_f"]] for u in s["units"]}}
    hist = jload(P_HIST, [])
    backfill_units(hist, funcs, stat, st, proj)
    same = lambda p, q: all(p.get(k) == q.get(k) for k in point if k not in ("ts", "commit"))
    if hist and same(hist[-1], point):
        hist[-1] = point
    else:
        hist.append(point)
    jsave(P_HIST, hist)
    recent = sorted(((r.get("matched_at", ""), k) for k, r in st.items()
                     if stat.get(k) == "matched" and r.get("matched_at")), reverse=True)[:15]
    data = {
        "game": proj.get("exe"), "generated": now(), "commit": commit, "summary": s, "history": hist,
        "recent": [{"name": cur_name(k, funcs[k], syms), "seg": funcs[k]["seg"], "size": funcs[k]["size"],
                    "at": t, "tries": st[k].get("tries", 0),
                    "auto": st[k].get("via") == "auto" or not st[k].get("tries")} for t, k in recent],
        "blocked": [{"name": cur_name(k, funcs[k], syms), "seg": funcs[k]["seg"], "size": funcs[k]["size"],
                     "note": st.get(k, {}).get("note", "")} for k, v in sorted(stat.items()) if v == "blocked"],
    }
    os.makedirs("docs", exist_ok=True)
    jsave("docs/progress.json", data)
    write("docs/index.html", DASHBOARD.replace("/*DATA*/null", json.dumps(data, separators=(",", ":"))
                                               .replace("</", "<\\/")))
    g, l = s["game"], s["lib"]
    write_badges(g, l)
    fmt = lambda t: f"{t['pct']:.2f}% ({t['done_b']}/{t['total_b']} bytes, {t['done_f']}/{t['total_f']} functions)"
    print(f"progress: game code {fmt(g)}; libs {fmt(l)}, +{l['asm_b']} bytes hand-written asm; "
          f"game functions named {g['named_f']}/{g['all_f']}; typed {g['typed_f']}/{g['c_f']} matched; "
          f"data {s['data']['pct']:.2f}%; "
          f"parked {s['blocked']} -> docs/index.html")


DASHBOARD = r"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Decomp Progress</title>
<style>
:root{--page:#f9f9f7;--surface:#fcfcfb;--ink:#0b0b0b;--ink2:#52514e;--muted:#898781;--grid:#e1e0d9;--axis:#c3c2b7;
--s1:#2a78d6;--s2:#eb6834;--s3:#1f9e74;--s4:#8a63d2;--u3:#1baf7a;--track:#ecebe7;--warn:#fab219}
@media (prefers-color-scheme:dark){:root:not([data-theme="light"]){--page:#0d0d0d;--surface:#1a1a19;--ink:#fff;--ink2:#c3c2b7;
--muted:#898781;--grid:#2c2c2a;--axis:#383835;--s1:#3987e5;--s2:#d95926;--s3:#2bb487;--s4:#9d7be0;--u3:#199e70;--track:#2c2c2a}}
:root[data-theme="dark"]{--page:#0d0d0d;--surface:#1a1a19;--ink:#fff;--ink2:#c3c2b7;--muted:#898781;--grid:#2c2c2a;
--axis:#383835;--s1:#3987e5;--s2:#d95926;--s3:#2bb487;--s4:#9d7be0;--u3:#199e70;--track:#2c2c2a}
*{box-sizing:border-box}
body{margin:0;background:var(--page);color:var(--ink);font:15px/1.45 system-ui,-apple-system,"Segoe UI",sans-serif}
main{max-width:1040px;margin:0 auto;padding:28px 16px 48px}
h1{font-size:22px;margin:0}
.sub{color:var(--ink2);font-size:13px;margin:4px 0 20px}
h2{font-size:15px;margin:0 0 12px}
.tiles{display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:12px;margin-bottom:16px}
.card{background:var(--surface);border:1px solid var(--grid);border-radius:10px;padding:16px}
.label{color:var(--ink2);font-size:13px}
.big{font-size:32px;font-weight:650;letter-spacing:-.01em;font-variant-numeric:tabular-nums}
.small{color:var(--muted);font-size:12px;font-variant-numeric:tabular-nums}
.bar{height:8px;background:var(--track);border-radius:4px;overflow:hidden;margin-top:10px}
.bar>span{display:block;height:100%;border-radius:4px}
.card+.card,.section{margin-top:16px}
.tiles .card{margin-top:0}
svg text{fill:var(--muted);font-size:11px}
.legend{display:flex;gap:16px;font-size:13px;color:var(--ink2);margin-bottom:6px}
.legend i{display:inline-block;width:12px;height:2px;vertical-align:middle;margin-right:6px}
table{width:100%;border-collapse:collapse;font-size:13px}
th{text-align:left;color:var(--ink2);font-weight:600;padding:6px 8px;border-bottom:1px solid var(--axis);cursor:pointer;white-space:nowrap}
td{padding:6px 8px;border-bottom:1px solid var(--grid);font-variant-numeric:tabular-nums;vertical-align:top}
td.num,th.num{text-align:right}
.mini{width:120px;height:6px;background:var(--track);border-radius:3px;overflow:hidden;display:inline-block;vertical-align:middle}
.mini>span{display:block;height:100%;background:var(--s1);border-radius:3px}
.tag{font-size:11px;color:var(--ink2);border:1px solid var(--axis);border-radius:4px;padding:0 4px;margin-left:6px}
.two{display:grid;grid-template-columns:1fr 1fr;gap:16px}
tr.grp td{color:var(--ink2);font-weight:600;font-size:12px;padding-top:14px;border-bottom:1px solid var(--axis)}
@media (max-width:760px){.two{grid-template-columns:1fr}.mini{width:70px}.opt{display:none}}
.tip{position:fixed;pointer-events:none;background:var(--surface);border:1px solid var(--axis);border-radius:6px;padding:6px 8px;
font-size:12px;color:var(--ink);box-shadow:0 2px 8px rgba(0,0,0,.15);display:none;white-space:nowrap}
.empty{color:var(--muted);font-size:13px}
.head{display:flex;justify-content:space-between;align-items:flex-start;gap:12px;flex-wrap:wrap}
details.card>summary{cursor:pointer;list-style-position:inside}
details.card>summary h2{display:inline;margin:0}
.ctl{display:flex;gap:8px;flex-wrap:wrap;justify-content:flex-end}
.scale{display:inline-flex;border:1px solid var(--axis);border-radius:6px;overflow:hidden}
.scale button{font:inherit;font-size:12px;background:transparent;color:var(--ink2);border:0;padding:4px 10px;cursor:pointer}
.scale button+button{border-left:1px solid var(--axis)}
.scale button[aria-pressed="true"]{background:var(--track);color:var(--ink);font-weight:600}
.wrap{overflow-x:auto}
.chips{display:flex;flex-wrap:wrap;gap:6px;margin-top:10px}
.chips button{font:inherit;font-size:12px;border:1px solid var(--grid);background:transparent;color:var(--ink2);border-radius:999px;padding:2px 10px;cursor:pointer}
.chips button[aria-pressed="true"]{color:var(--ink);font-weight:600;border-color:var(--axis)}
.chips i{display:inline-block;width:12px;height:2px;margin-right:6px;vertical-align:middle;background:var(--axis);border-radius:1px}
#multi svg .ctx{cursor:pointer}
.sub2{color:var(--ink2);font-size:12px;font-weight:600;margin:16px 0 8px}
details summary{cursor:pointer;color:var(--ink2);font-size:12px;font-weight:600;margin:16px 0 8px}
code{font-size:12px}
</style>
</head>
<body>
<main>
<div class="head"><div><h1 id="title">Decomp progress</h1><div class="sub" id="sub"></div></div>
<div class="scale" id="theme" role="group" aria-label="Theme"></div></div>
<div class="tiles" id="tiles"></div>
<div class="card section"><div class="head"><h2>Progress over time</h2><div class="ctl"><div class="scale" id="measure" role="group" aria-label="Measure"></div><div class="scale" id="scale" role="group" aria-label="Time scale"></div></div></div>
<div class="legend" id="legend"></div><div id="chart"></div></div>
<div class="card section"><h2>Progress per unit</h2>
<div class="small" style="margin-bottom:6px"><span id="multi-what">C bytes matched</span> in each unit, on the time scale and measure above. main.exe and the engine are highlighted; click a unit to highlight it too. PsyQ (Sony's objects, ~99%) is left out.</div>
<div id="multi"></div><div class="chips" id="uchips" role="group" aria-label="Highlight a unit"></div></div>
<div class="card section"><h2>By binary</h2><div class="wrap"><table id="segs"></table></div></div>

</main>
<div class="tip" id="tip"></div>
<script>
const D=/*DATA*/null;
// theme: Auto follows the OS; Light/Dark set data-theme on <html> (the CSS handles both)
(function(){
 const el=document.getElementById("theme"),root=document.documentElement;
 let t=null;try{t=localStorage.getItem("decomp-theme")}catch(e){}
 function set(v){t=v;if(v=="auto")root.removeAttribute("data-theme");else root.setAttribute("data-theme",v);
  try{localStorage.setItem("decomp-theme",v)}catch(e){}
  el.innerHTML=[["auto","Auto"],["light","Light"],["dark","Dark"]].map(([k,l])=>`<button data-t="${k}" aria-pressed="${k==t}">${l}</button>`).join("");
  el.querySelectorAll("button").forEach(b=>b.onclick=()=>set(b.dataset.t))}
 set(["light","dark"].includes(t)?t:"auto");
})();
const $=id=>document.getElementById(id);
const esc=s=>String(s??"").replace(/[&<>"]/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;"}[c]));
const pct=(a,b)=>b?100*a/b:0, f2=x=>x.toFixed(2)+"%", n=x=>x.toLocaleString();
const S=D.summary, hasLib=S.lib.total_f>0;
$("title").textContent=(D.game||"Game")+" decomp progress";
$("sub").textContent="Updated "+D.generated.replace("T"," ").replace("Z"," UTC")+(D.commit?" · commit "+D.commit:"");
const tile=(label,big,small,p,color)=>`<div class="card"><div class="label">${label}</div><div class="big">${big}</div>
<div class="small">${small}</div>${p==null?"":`<div class="bar"><span style="width:${Math.min(100,p)}%;background:${color}"></span></div>`}</div>`;
// one tile per code group, same layout for both: bytes % (headline + bar), bytes and functions below
const grpTile=(label,G,color,extra)=>tile(label,f2(G.pct),
 `${n(G.done_b)} / ${n(G.total_b)} bytes<br>${n(G.done_f)} / ${n(G.total_f)} functions · ${f2(pct(G.done_f,G.total_f))}`+(extra||""),G.pct,color);
const named=S.game.named_f, all=S.game.all_f, DA=S.data||{done_b:0,total_b:0,pct:0};
$("tiles").innerHTML=grpTile("Game code matched",S.game,"var(--s1)")+
 tile("Game functions named",f2(pct(named,all)),`${n(named)} / ${n(all)} with a real name`,pct(named,all),"var(--s3)")+
 tile("Data in C",f2(DA.pct),`${n(DA.done_b)} / ${n(DA.total_b)} bytes of data and rodata`,DA.pct,"var(--s4)")+
 tile("Parked",`${S.blocked}`,"functions waiting for another attempt",null);

// history chart: % of bytes matched per group, bucketed by a selectable time unit
(function(){
 const el=$("chart"), ctl=$("scale"), mctl=$("measure");
 const MEASURES={bytes:"Bytes",functions:"Functions"};
 let measure=null;try{measure=localStorage.getItem("decomp-measure")}catch(e){}
 if(!MEASURES[measure])measure="bytes";
 const fnMode=()=>measure=="functions";
 const H=(D.history||[]).filter(h=>h.game_tb>0);
 const series=[{k:"game",name:"Game code",col:"var(--s1)",G:S.game}];
 // the library is linked whole from Sony's objects: charting it would only flatten the game lines
 series.push({k:"named",name:"Game functions named",col:"var(--s3)",num:"game_named_f",den:"game_all_f"});
 // data has no functions: its line stays in bytes in both measures, and says so
 series.push({k:"data",name:"Data in C",col:"var(--s4)",num:"data_b",den:"data_tb",bytesOnly:1});
 const legend=()=>{$("legend").innerHTML=series.map(s=>`<span><i style="background:${s.col}"></i>${s.name}`+
  (s.G?` (${fnMode()?"functions":"bytes"})`:s.bytesOnly?` (bytes${fnMode()?"; data has no functions":""})`:"")+`</span>`).join("")};
 const UNITS={minute:{label:"Minutes",win:120},hour:{label:"Hours",win:48},day:{label:"Days",win:60},
  week:{label:"Weeks",win:52},month:{label:"Months",win:0}};
 const floor=(t,u)=>{const d=new Date(t);d.setSeconds(0,0);if(u!="minute"){d.setMinutes(0);if(u!="hour"){d.setHours(0);
  if(u=="week")d.setDate(d.getDate()-((d.getDay()+6)%7));if(u=="month")d.setDate(1)}}return d.getTime()};
 const step=(t,u,n=1)=>{const d=new Date(t);if(u=="minute")d.setMinutes(d.getMinutes()+n);else if(u=="hour")d.setHours(d.getHours()+n);
  else if(u=="day")d.setDate(d.getDate()+n);else if(u=="week")d.setDate(d.getDate()+7*n);else d.setMonth(d.getMonth()+n);return d.getTime()};
 const lab=(t,u)=>{const d=new Date(t);return u=="minute"?d.toLocaleTimeString(undefined,{hour:"2-digit",minute:"2-digit"})
  :u=="hour"?d.toLocaleString(undefined,{month:"short",day:"numeric",hour:"2-digit",minute:"2-digit"})
  :u=="month"?d.toLocaleDateString(undefined,{month:"short",year:"numeric"})
  :(u=="week"?"w/c ":"")+d.toLocaleDateString(undefined,{month:"short",day:"numeric"})};
 const auto=()=>{const sp=H.length?Date.now()-Date.parse(H[0].ts):0,HR=36e5,DAY=864e5;
  return sp<3*HR?"minute":sp<2*DAY?"hour":sp<60*DAY?"day":sp<365*DAY?"week":"month"};
 let unit=null;try{unit=localStorage.getItem("decomp-scale")}catch(e){}
 if(!UNITS[unit])unit=auto();
 function buckets(u){
  const now=Date.now(),last=floor(now,u),out=[];
  let start=H.length?floor(Date.parse(H[0].ts),u):last;
  if(UNITS[u].win)start=Math.max(start,step(last,u,1-UNITS[u].win));
  let j=0,prev=null;
  for(let b=start;b<=last;b=step(b,u)){
   const end=step(b,u);
   while(j<H.length&&Date.parse(H[j].ts)<end){prev=H[j];j++}
   out.push({t:b,h:prev});
  }
  // the state at the start of the window, to compute the first bucket's gain
  let before=null;for(const h of H)if(Date.parse(h.ts)<start)before=h;
  return {out,before};
 }
 function draw(){
  ctl.innerHTML=Object.entries(UNITS).map(([k,v])=>`<button data-u="${k}" aria-pressed="${k==unit}">${v.label}</button>`).join("");
  ctl.querySelectorAll("button").forEach(b=>b.onclick=()=>{unit=b.dataset.u;try{localStorage.setItem("decomp-scale",unit)}catch(e){}draw()});
  mctl.innerHTML=Object.entries(MEASURES).map(([k,v])=>`<button data-m="${k}" aria-pressed="${k==measure}">${v}</button>`).join("");
  mctl.querySelectorAll("button").forEach(b=>b.onclick=()=>{measure=b.dataset.m;try{localStorage.setItem("decomp-measure",measure)}catch(e){}draw()});
  legend();$("multi-what").textContent=fnMode()?"Functions matched":"C bytes matched";
  if(!H.length){el.innerHTML='<div class="empty">The chart fills in as functions are matched.</div>';$("multi").innerHTML="";return}
  const {out:Bk,before}=buckets(unit), N=Bk.length;
  multi(Bk,before);
  // every point against today's totals: the code known to exist grew (overlays were added
  // later), and a percentage of a smaller total would make the line fall when nothing was lost
  const v=(s,h)=>!h?0:s.num?pct(h[s.num]||0,s.den=="game_all_f"?S.game.all_f:(S.data||{}).total_b)
   :fnMode()?pct(h[s.k+"_f"],s.G.total_f):pct(h[s.k+"_b"],s.G.total_b);
  const W=Math.max(320,el.clientWidth||900),Ht=240,L=44,R=16,T=10,B=28,iw=W-L-R,ih=Ht-T-B;
  const hi=Math.max(...Bk.map(b=>Math.max(...series.map(s=>v(s,b.h)))));
  const ymax=Math.min(100,Math.max(1,hi*1.15)), bw=iw/Math.max(1,N);
  const x=i=>L+bw*(i+.5), y=val=>T+ih*(1-val/ymax);
  let g="";
  for(let i=0;i<=4;i++){const val=ymax*i/4,yy=y(val).toFixed(1);
   g+=`<line x1="${L}" x2="${W-R}" y1="${yy}" y2="${yy}" stroke="${i?"var(--grid)":"var(--axis)"}" stroke-width="1"/>`+
      `<text x="${L-6}" y="${+yy+4}" text-anchor="end">${val<1?val.toFixed(2):val<10?val.toFixed(1):val.toFixed(0)}%</text>`}
  const nl=Math.max(1,Math.min(N,Math.floor(iw/110)));
  for(let k=0;k<nl;k++){const i=nl==1?N-1:Math.round(k*(N-1)/(nl-1));
   g+=`<text x="${x(i).toFixed(1)}" y="${Ht-8}" text-anchor="${k==0&&nl>1?"start":k==nl-1?"end":"middle"}">${esc(lab(Bk[i].t,unit))}</text>`}
  for(const s of series){
   let d="";Bk.forEach((b,i)=>{const px=x(i).toFixed(1),py=y(v(s,b.h)).toFixed(1);d+=(i?"L":"M")+px+","+py});
   g+=`<path d="${d}" fill="none" stroke="${s.col}" stroke-width="2" stroke-linejoin="round"/>`;
   if(N<=90)Bk.forEach((b,i)=>{g+=`<circle cx="${x(i).toFixed(1)}" cy="${y(v(s,b.h)).toFixed(1)}" r="${i==N-1?4:2.5}" fill="${s.col}" stroke="var(--surface)" stroke-width="${i==N-1?2:1}"/>`});
  }
  g+=`<line id="xh" y1="${T}" y2="${T+ih}" stroke="var(--axis)" stroke-width="1" visibility="hidden"/>`;
  g+=`<rect x="${L}" y="${T}" width="${iw}" height="${ih}" fill="transparent" id="hit"/>`;
  el.innerHTML=`<svg width="100%" viewBox="0 0 ${W} ${Ht}" role="img" aria-label="Matched ${fnMode()?"functions":"code bytes"} percentage by ${unit}">${g}</svg>`;
  const tip=$("tip"),xh=el.querySelector("#xh"),svg=el.querySelector("svg");
  el.querySelector("#hit").addEventListener("mousemove",ev=>{
   const r=svg.getBoundingClientRect(),mx=(ev.clientX-r.left)*W/r.width;
   const i=Math.max(0,Math.min(N-1,Math.floor((mx-L)/bw))),b=Bk[i],p=i?Bk[i-1].h:before;
   xh.setAttribute("x1",x(i));xh.setAttribute("x2",x(i));xh.setAttribute("visibility","visible");
   tip.innerHTML=`<b>${esc(lab(b.t,unit))}</b>`+series.map(s=>{
    if(s.num)return `<br>${s.name}: ${f2(v(s,b.h))}`;
    const gb=(b.h?b.h[s.k+"_b"]:0)-(p?p[s.k+"_b"]:0),gf=(b.h?b.h[s.k+"_f"]:0)-(p?p[s.k+"_f"]:0);
    return `<br>${s.name}: ${f2(v(s,b.h))} · ${n(b.h?b.h[s.k+"_f"]:0)} functions`+(gf||gb?` <span class="small">(+${n(gf)} fn, +${n(gb)} B)</span>`:"")}).join("");
   tip.style.display="block";tip.style.left=Math.min(ev.clientX+12,innerWidth-300)+"px";tip.style.top=(ev.clientY+12)+"px";});
  el.querySelector("#hit").addEventListener("mouseleave",()=>{tip.style.display="none";xh.setAttribute("visibility","hidden")});
 }
 // all game units on one chart: every unit a thin grey line, main.exe and the engine in
 // colour, plus one unit the viewer picks (palette slots 1-3; more hues would not stay apart)
 let pick=null;try{pick=localStorage.getItem("decomp-unit")}catch(e){}
 function multi(Bk,before){
  $("tip").style.display="none";
  const box=$("multi"), N=Bk.length, units=(S.units||[]).filter(u=>!u.lib&&u.total_b>0);
  const FIX={"main exe":"var(--s1)","engine":"var(--s2)"};
  if(!units.some(u=>u.unit==pick&&!FIX[u.unit]))pick=null;
  const col=u=>FIX[u.unit]||(u.unit==pick?"var(--u3)":null);
  const at=(h,u)=>h&&h.units&&h.units[u.unit]||[0,0];
  const val=(h,u)=>fnMode()?pct(at(h,u)[1],u.total_f):pct(at(h,u)[0],u.total_b);
  const name=u=>u.unit+(u.title&&u.title!=u.unit?` (${u.title})`:"");
  const W=Math.max(320,box.clientWidth||900),Ht=280,L=44,R=Math.min(150,W*.28),T=10,B=28,iw=W-L-R,ih=Ht-T-B,bw=iw/Math.max(1,N);
  const hi=Math.max(...units.flatMap(u=>Bk.map(b=>val(b.h,u))));
  const ymax=Math.min(100,Math.max(1,hi*1.1)), x=i=>L+bw*(i+.5), y=v=>T+ih*(1-v/ymax);
  let g="";
  for(let i=0;i<=4;i++){const v=ymax*i/4,yy=y(v).toFixed(1);
   g+=`<line x1="${L}" x2="${L+iw}" y1="${yy}" y2="${yy}" stroke="${i?"var(--grid)":"var(--axis)"}" stroke-width="1"/>`+
      `<text x="${L-6}" y="${+yy+4}" text-anchor="end">${v<1?v.toFixed(2):v<10?v.toFixed(1):v.toFixed(0)}%</text>`}
  const nl=Math.max(1,Math.min(N,Math.floor(iw/110)));
  for(let k=0;k<nl;k++){const i=nl==1?N-1:Math.round(k*(N-1)/(nl-1));
   g+=`<text x="${x(i).toFixed(1)}" y="${Ht-8}" text-anchor="${k==0&&nl>1?"start":k==nl-1?"end":"middle"}">${esc(lab(Bk[i].t,unit))}</text>`}
  const path=u=>Bk.map((b,i)=>(i?"L":"M")+x(i).toFixed(1)+","+y(val(b.h,u)).toFixed(1)).join("");
  // context lines first, highlighted ones on top; a wide transparent stroke is the click target
  const ctx=units.filter(u=>!col(u)), hl=units.filter(u=>col(u));
  for(const u of ctx)g+=`<g class="ctx" data-u="${esc(u.unit)}"><path d="${path(u)}" fill="none" stroke="var(--axis)" stroke-width="1.25" stroke-linejoin="round"/>`+
   `<path d="${path(u)}" fill="none" stroke="transparent" stroke-width="10"><title>${esc(name(u))}</title></path></g>`;
  for(const u of hl){g+=`<path d="${path(u)}" fill="none" stroke="${col(u)}" stroke-width="2" stroke-linejoin="round"/>`+
   `<circle cx="${x(N-1).toFixed(1)}" cy="${y(val(Bk[N-1].h,u)).toFixed(1)}" r="4" fill="${col(u)}" stroke="var(--surface)" stroke-width="2"/>`}
  // direct labels for the highlighted lines, nudged apart so they never overlap
  const labs=hl.map(u=>({u,v:val(Bk[N-1].h,u),yy:y(val(Bk[N-1].h,u))})).sort((a,b)=>a.yy-b.yy);
  for(let i=1;i<labs.length;i++)labs[i].yy=Math.max(labs[i].yy,labs[i-1].yy+14);
  for(const l of labs)g+=`<text x="${(x(N-1)+8).toFixed(1)}" y="${(l.yy+4).toFixed(1)}" style="fill:var(--ink2)">${esc(l.u.unit)} ${f2(l.v)}</text>`;
  g+=`<line class="xh" y1="${T}" y2="${T+ih}" stroke="var(--axis)" stroke-width="1" visibility="hidden"/>`;
  g+=`<rect class="hit" x="${L}" y="${T}" width="${iw}" height="${ih}" fill="transparent" pointer-events="none"/>`;
  box.innerHTML=`<svg width="100%" viewBox="0 0 ${W} ${Ht}" role="img" aria-label="${fnMode()?"Functions":"C bytes"} matched per unit by ${unit}">${g}</svg>`;
  $("uchips").innerHTML=units.map(u=>`<button data-u="${esc(u.unit)}" aria-pressed="${!!col(u)}"${FIX[u.unit]?" disabled":""}>`+
   `<i style="background:${col(u)||"var(--axis)"}"></i>${esc(name(u))}</button>`).join("");
  const choose=un=>{pick=pick==un?null:un;try{localStorage.setItem("decomp-unit",pick||"")}catch(e){}multi(Bk,before)};
  $("uchips").querySelectorAll("button:not([disabled])").forEach(b=>b.onclick=()=>choose(b.dataset.u));
  box.querySelectorAll(".ctx").forEach(e=>{e.onclick=()=>choose(e.dataset.u);
   e.onmouseenter=()=>e.firstChild.setAttribute("stroke","var(--ink2)");e.onmouseleave=()=>e.firstChild.setAttribute("stroke","var(--axis)")});
  // hover anywhere: every unit's value at that time, highest first
  const svg=box.querySelector("svg"), xh=box.querySelector(".xh"), tip=$("tip");
  svg.addEventListener("mousemove",ev=>{
   const r=svg.getBoundingClientRect(),mx=(ev.clientX-r.left)*W/r.width;
   if(mx<L||mx>L+iw){tip.style.display="none";xh.setAttribute("visibility","hidden");return}
   const i=Math.max(0,Math.min(N-1,Math.floor((mx-L)/bw))),b=Bk[i];
   xh.setAttribute("x1",x(i));xh.setAttribute("x2",x(i));xh.setAttribute("visibility","visible");
   tip.innerHTML=`<b>${esc(lab(b.t,unit))}</b> · ${fnMode()?"functions":"bytes"}`+units.map(u=>({u,v:val(b.h,u)})).sort((a,c)=>c.v-a.v)
    .map(o=>`<br><i style="display:inline-block;width:10px;height:2px;vertical-align:middle;margin-right:6px;background:${col(o.u)||"var(--axis)"}"></i>`+
     `${col(o.u)?"<b>":""}${esc(o.u.unit)} ${f2(o.v)}${col(o.u)?"</b>":""}`).join("");
   tip.style.display="block";tip.style.left=Math.min(ev.clientX+12,innerWidth-220)+"px";tip.style.top=Math.max(8,Math.min(ev.clientY+12,innerHeight-tip.offsetHeight-8))+"px"});
  svg.addEventListener("mouseleave",()=>{tip.style.display="none";xh.setAttribute("visibility","hidden")});
 }
 draw();
 let rt;addEventListener("resize",()=>{clearTimeout(rt);rt=setTimeout(draw,150)});
})();

// segment table, sortable
(function(){
 // [key, label, numeric, hidden on narrow screens]
 const cols=[["unit","Unit",0],["pct","C bytes matched",1],["done_f","Functions matched",1],["named_f","Named",1,1],["blocked","Parked",1,1]];
 let key="unit",dir=1;
 function draw(){
  const cmp=(a,b)=>((a[key]>b[key]?1:a[key]<b[key]?-1:0)||(a.unit>b.unit?1:-1))*dir;
  const cls=c=>[c[2]?"num":"",c[3]?"opt":""].join(" ");
  const row=s=>{const c=s.total_b>0;   // no C (all hand-written): a dash, not 0.00%
   return `<tr><td>${esc(s.unit)}${s.title&&s.title!=s.unit?`<div class="small">${esc(s.title)}</div>`:""}</td>
   <td class="num">${c?`${f2(s.pct)} <span class="mini"><span style="width:${s.pct}%;${s.lib?"background:var(--s2)":""}"></span></span>`:"—"}</td>
   <td class="num">${c?`${n(s.done_f)} / ${n(s.total_f)} <span class="small">${f2(pct(s.done_f,s.total_f))}</span>`:"—"}</td>
   <td class="num opt">${n(s.named_f)} / ${n(s.all_f)}</td><td class="num opt">${s.blocked||""}</td></tr>`};
  const all=S.units||[], game=all.filter(s=>!s.lib), lib=all.filter(s=>s.lib).sort(cmp);
  const head=t=>`<tr class="grp"><td colspan="5">${t}</td></tr>`;
  const gameRows=(lib.length?head("Game code"):"")+game.sort(cmp).map(row).join("");
  $("segs").innerHTML="<tr>"+cols.map(c=>`<th class="${cls(c)}" data-k="${c[0]}">${c[1]}${key==c[0]?(dir>0?" ▲":" ▼"):""}</th>`).join("")+"</tr>"+
   (all.length?gameRows+
    (lib.length?head("PsyQ library: Sony's objects, linked in place")+lib.map(row).join(""):"")
   :'<tr><td class="empty" colspan="5">Nothing indexed yet.</td></tr>');
  $("segs").querySelectorAll("th").forEach(th=>th.onclick=()=>{const k=th.dataset.k;dir=key==k?-dir:(k=="unit"?1:-1);key=k;draw()});
 }
 draw();
})();


</script>
</body>
</html>
"""


# ---------------------------------------------------------------- CLI

def main():
    # line-buffered even when redirected to a file, so long runs (auto --permute) show live progress
    sys.stdout.reconfigure(line_buffering=True)
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("extract", help="dump disc, find boot exe, record sha1")
    p.add_argument("cue")
    p.set_defaults(fn=cmd_extract)
    sub.add_parser("index", help="catalogue functions from asm/nonmatchings").set_defaults(fn=cmd_index)
    p = sub.add_parser("add-overlay", help="register a code overlay stored inside another file as a build target")
    p.add_argument("name", help="short name: disc/NAME.bin, configs/USA/overlays/NAME.yaml, build/NAME.bin")
    p.add_argument("source", help="file that contains it, e.g. disc/files/GAME/DATA.DAT")
    p.add_argument("--offset", type=lambda s: int(s, 0), help="byte offset in source")
    p.add_argument("--sector", type=lambda s: int(s, 0), help="sector in source (x --sector-size)")
    p.add_argument("--sector-size", type=int, default=2048)
    p.add_argument("--size", type=lambda s: int(s, 0), required=True, help="bytes")
    p.set_defaults(fn=cmd_add_overlay)
    p = sub.add_parser("probe", help="find the compiler/flags that match small functions")
    p.add_argument("--n", type=int, default=10, help="number of functions to test")
    p.add_argument("--write", action="store_true", help="save the best to configs/USA/project.mk")
    p.add_argument("--seg", help="regex of segments to sample (default: all non-lib segments)")
    p.set_defaults(fn=cmd_probe)
    p = sub.add_parser("config", help="set compiler settings by hand")
    p.add_argument("--cc")
    p.add_argument("--opt")
    p.add_argument("--g", type=int)
    p.add_argument("--maspsx", help='maspsx args without -G, e.g. "--macro-inc --expand-div --aspsx-version=2.79"')
    p.add_argument("--extra", help="extra cc1 flags, e.g. -funsigned-char")
    p.set_defaults(fn=cmd_config)
    sub.add_parser("status", help="one-line overview").set_defaults(fn=cmd_status)
    p = sub.add_parser("next", help="pick the next function and print its m2c draft")
    p.add_argument("--func")
    p.add_argument("--seg")
    p.add_argument("--count", type=int, default=1, help="show the N smallest targets at once")
    p.add_argument("--max-lines", type=int, default=120)
    p.add_argument("--asm-lines", type=int, default=80)
    p.set_defaults(fn=cmd_next)
    p = sub.add_parser("queue", help="the next targets as JSON, marking nothing (tools/swarm)")
    p.add_argument("--seg")
    p.add_argument("--count", type=int, default=50)
    p.set_defaults(fn=cmd_queue)
    p = sub.add_parser("m2c", help="print m2c draft")
    p.add_argument("func")
    p.set_defaults(fn=cmd_m2c)
    p = sub.add_parser("try", help="build and diff one or more functions")
    p.add_argument("funcs", nargs="+")
    p.set_defaults(fn=cmd_try)
    p = sub.add_parser("auto", help="match small functions from m2c drafts without the model")
    p.add_argument("--max-size", type=int, default=32, help="largest function to try, in bytes")
    p.add_argument("--limit", type=int, default=200)
    p.add_argument("--seg", help="regex of segments (default: all non-lib)")
    p.add_argument("--retry", action="store_true", help="also retry functions auto already failed on")
    p.add_argument("--jobs", type=int, help="parallel workers (default: CPUs - 1)")
    p.add_argument("--permute", action="store_true", help="then run decomp-permuter on near-misses")
    p.add_argument("--max-diff", type=int, default=4, help="near-miss: at most this many instructions differ")
    p.add_argument("--permute-seconds", type=int, default=60, help="permuter time per function")
    p.add_argument("--permute-limit", type=int, default=20, help="most near-misses to permute per run")
    p.add_argument("--no-protos", action="store_true", help="do not declare landed functions in their owned headers")
    p.add_argument("--no-propagate", action="store_true", help="do not copy matches to identical functions")
    p.add_argument("--legacy", action="store_true", help="run the retired bulk matcher anyway")
    p.add_argument("--commit", action="store_true")
    p.add_argument("--trailer", action="append")
    p.set_defaults(fn=cmd_auto)
    p = sub.add_parser("split-lib", help="cut the PsyQ segment into one file per library object")
    p.add_argument("--yaml", help="configs/USA yaml holding the segment (default: the exe's)")
    p.add_argument("--seg", default="lib/psyq")
    p.add_argument("--dry-run", type=int, nargs="?", const=40, default=0, help="only print the plan")
    p.set_defaults(fn=cmd_split_lib)
    p = sub.add_parser("rename", help="name functions/data: symbol files, sources, re-split, verify")
    p.add_argument("names", nargs="*", help="OLD NEW [OLD NEW ...]; OLD: name, SEG/NAME, key or OVL:ADDRESS")
    p.add_argument("--file", help="file of `OLD NEW` lines (# comments)")
    p.add_argument("--ovl", help="overlay of a bare D_ data symbol")
    p.add_argument("--sync", action="store_true", help="make sources use the names already in the symbol files")
    p.set_defaults(fn=cmd_rename)
    p = sub.add_parser("link-psyq", help="link Sony's PsyQ objects in place of the library code")
    p.add_argument("--keep", action="store_true", help="leave a failed attempt in place to inspect")
    p.set_defaults(fn=cmd_link_psyq)
    p = sub.add_parser("dups", help="report identical and near-identical functions")
    p.add_argument("--list", type=int, default=0, help="show the N largest groups")
    p.set_defaults(fn=cmd_dups)
    p = sub.add_parser("propagate", help="copy matched C to the identical functions elsewhere")
    p.add_argument("--commit", action="store_true")
    p.add_argument("--trailer", action="append")
    p.set_defaults(fn=cmd_propagate)
    p = sub.add_parser("permute", help="run decomp-permuter with a time limit")
    p.add_argument("func")
    p.add_argument("--minutes", type=int, default=15)
    p.add_argument("--jobs", type=int)
    p.set_defaults(fn=cmd_permute)
    p = sub.add_parser("block", help="park a function with a note")
    p.add_argument("func")
    p.add_argument("--note", required=True)
    p.set_defaults(fn=cmd_block)
    p = sub.add_parser("done", help="validate one hand match (scope, lint, function, full build), record it")
    p.add_argument("func")
    p.add_argument("--commit", action="store_true",
                   help="after every check passes, commit src/ include/ configs/ .decomp/ docs/")
    p.add_argument("--note", help="what made it match (types, statement order, ...): goes in the commit body")
    p.add_argument("--hack", help="why a register pin / asm(\"\") barrier is needed (required if used)")
    p.add_argument("--trailer", action="append", help="line appended to the commit message (repeatable)")
    p.add_argument("--defer-build", action="store_true",
                   help="batch mode (tools/swarm): skip the full build; run `decomp.py verify` after the "
                        "batch and publish none of its commits unless it passes")
    p.set_defaults(fn=cmd_done)
    sub.add_parser("verify", help="clean rebuild and byte compare").set_defaults(fn=cmd_verify)
    sub.add_parser("progress", help="refresh docs/ dashboard and history").set_defaults(fn=cmd_progress)
    a = ap.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
