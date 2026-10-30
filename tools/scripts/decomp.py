#!/usr/bin/env python3
"""Helper CLI for a PSX matching decompilation.

Runs inside the toolchain container:   tools/dk python3 tools/scripts/decomp.py <command> [args]

Output is deliberately short. The caller is usually a model that pays for every line it
reads, so details go to files and stdout carries only verdicts, small diff windows and
one-line summaries. Run with -h for the command list.
"""
import argparse
import datetime
import difflib
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
# the permuter with the pipeline's Python filters in-process (PERMUTER_PLAIN=1: the stock one on
# the full context)
PERMUTER_FAST = os.path.join(os.path.dirname(os.path.abspath(__file__)), "permuter_fast.py")
P_PERMLOG = os.path.join(DEC, "permuter.jsonl")   # one line per permuter run (permstats)
X = os.environ.get("CROSS", "mipsel-linux-gnu-")
DEFAULT_MASPSX = "--macro-inc --expand-div"

# splat comment on each instruction: /* ROM VRAM WORD */ instruction
ASM_LINE = re.compile(r"/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*(.*)$")
# the same on a data line, whose word may be missing (a string's bytes follow on the next line)
DATA_LINE = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}(?:\s+[0-9A-Fa-f]{8})?\s*\*/\s*(\S.*)$")
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


def jsave_rows(path, rows):
    """A JSON list written one compact row per line: a long log (the progress history) stays
    small and its diffs line by line."""
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    tmp = path + ".tmp"
    with open(tmp, "w", encoding="utf-8", newline="\n") as f:
        f.write("[\n" + ",\n".join(json.dumps(r, sort_keys=True, separators=(",", ":")) for r in rows) + "\n]\n")
    os.replace(tmp, path)


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
    lines = read(P_SUMS).splitlines() if os.path.exists(P_SUMS) else []
    new = f"{sha}  {built}"
    at = [i for i, l in enumerate(lines) if l.endswith("  " + built)]
    if at:                                   # in place, so extracting again leaves the file as it was
        lines[at[0]] = new
        lines = [l for i, l in enumerate(lines) if i not in at[1:]]
    else:
        lines.append(new)
    write(P_SUMS, "\n".join(lines) + "\n")


def sym_file(ovl):
    """Where names live: configs/USA/sym.main.txt for the exe, sym.<ovl>.txt per overlay
    (slot overlays share load addresses, so each needs its own)."""
    return f"{CFG}/sym.{ovl}.txt" if ovl else f"{CFG}/sym.main.txt"


_SYM_CACHE = {}


def sym_names():
    """functions.json-style key (ADDRESS or OVERLAY:ADDRESS) -> name. Read once per command (a
    command asks many times), again only when a symbol file changed (rename writes them)."""
    stamp = tuple((p, os.stat(p).st_mtime_ns, os.stat(p).st_size) for p in sorted(glob.glob(f"{CFG}/sym.*.txt")))
    if _SYM_CACHE.get("stamp") != stamp:
        _SYM_CACHE.update(stamp=stamp, names=read_sym_names())
    return dict(_SYM_CACHE["names"])


def read_sym_names():
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
    """NAME, SEG/NAME, 0xADDRESS, or a functions.json key (ADDRESS or OVERLAY:ADDRESS). A name the
    slot overlays share (they all load at one address) means the copy being worked on, when the
    working tree has removed exactly one of their INCLUDE_ASM lines."""
    syms = sym_names()
    n = name.lower()
    seg = None
    if "/" in n and ":" not in n:
        seg, n = n.rsplit("/", 1)
    addr = f"{int(n, 16):08X}" if re.fullmatch(r"0x[0-9a-f]{1,8}", n) else None
    hits = [(k, e) for k, e in funcs.items()
            if (seg is None or e["seg"].lower() == seg)
            and (n in (e["name"].lower(), e["orig"].lower(), cur_name(k, e, syms).lower(), k.lower())
                 or addr == k.rsplit(":", 1)[-1])]
    if len(hits) > 1:
        work = [(k, e) for k, e in hits if e.get("src") and cur_name(k, e, syms) in matched_in_worktree(e["src"])]
        if len(work) == 1:
            return work[0]
        die(f"{name} is ambiguous - give SEG/NAME or the key: " + ", ".join(f"{e['seg']}/{e['name']} ({k})" for k, e in hits))
    if not hits:
        names = {cur_name(k, e, syms) for k, e in funcs.items()} | set(funcs)
        close = difflib.get_close_matches(name, names, n=3, cutoff=0.8)
        die(f"unknown function {name} (not in .decomp/functions.json - re-run index?)"
            + (f"; did you mean {', '.join(close)}?" if close else ""))
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
    """A function's definition (not a prototype or call); the name may be parenthesized,
    `void (name)(...) {`, which keeps a function-like macro of the same name from expanding; K&R
    definitions (`void f(a) u8 *a; {`) count."""
    return re.compile(r"^[^\n;#{}]*?(?:\(\s*)?\b" + re.escape(name) + r"\b(?:\s*\))?\s*\([^;{]*\)"
                      r"(?:\s*[^;{}()#]+;)*\s*\{", re.M)


DEFS = re.compile(r"^[A-Za-z_][^\n;#{}=]*?(?:\(\s*)?\b([A-Za-z_]\w*)\b(?:\s*\))?\s*\([^;{]*\)"
                  r"(?:\s*[^;{}()#]+;)*\s*\{", re.M)


def defined_names(text):
    """The names of every function TEXT defines (func_def_regex's rule, in one pass)."""
    return {m.group(1) for m in DEFS.finditer(text)}


def locate_def(name):
    rx = func_def_regex(name)
    for p in glob.glob("src/**/*.c", recursive=True):
        if rx.search(read(p)):
            return p.replace("\\", "/")
    return None


def extract_function(text, name, top=False):
    """NAME's definition with its body. TOP: only one starting at column 0 (the regex also takes an
    indented `if (name(x) != 0) {` call line, which comes first when a caller precedes the
    definition)."""
    m = next((m for m in func_def_regex(name).finditer(text)
              if not top or not text[m.start()].isspace()), None)
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
        # a jump within the section: keep its target, as an offset from the function's start
        if typ == "R_MIPS_26" and sym == ".text" and i < len(words):
            relocs[i] = ("R_MIPS_26_LOCAL", (words[i] & 0x3FFFFFF) * 4 - base)
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


def key_vram(key):
    return int(str(key).split(":")[-1], 16)


def compare(tw, cw, relocs, vram=None):
    """Instruction diffs of compiled words `cw` against target words `tw`. Relocated fields are
    masked; with the function's `vram`, a jump within it must also go to the same place."""
    nt, nc = trim_nops(tw), trim_nops(cw)
    diffs, ops = [], 0
    for i in range(max(nt, nc)):
        if i >= nt or i >= nc:
            diffs.append(i)
            ops += 1
            continue
        typ, arg = relocs.get(i, ("", None))
        if typ == "R_MIPS_26_LOCAL":
            typ = "R_MIPS_26"
            if vram is not None and (tw[i] >> 26) == (cw[i] >> 26):
                dest = ((tw[i] & 0x3FFFFFF) * 4 | (vram & 0xF0000000)) - vram
                if dest != arg:
                    diffs.append(i)
                    continue
        keep = RELOC_KEEP.get(typ, 0xFFFFFFFF)
        t, c = tw[i] & keep, cw[i] & keep
        if t != c:
            diffs.append(i)
            special = (t >> 26) == 0 and (c >> 26) == 0
            if (t >> 26) != (c >> 26) or (special and (t & 0x3F) != (c & 0x3F)):
                ops += 1
    return diffs, ops, nt, nc


def is_branch(w):
    """A branch or jump: REGIMM, j, jal, beq..bgtz, the -likely forms, or jr/jalr."""
    op = w >> 26
    return op in (1, 2, 3, 4, 5, 6, 7, 20, 21, 22, 23) or (op == 0 and (w & 0x3F) in (8, 9))




def diff_stage(tw, cw, diffs, ops, nt, nc):
    """How far a function has got: `structure` (length or branch layout differs), `instructions`
    (same skeleton, some opcodes differ), `registers` (operands only), or `match`."""
    if not diffs:
        return "match"
    if nt != nc or [i for i in range(nt) if is_branch(tw[i])] != [i for i in range(nc) if is_branch(cw[i])]:
        return "structure"
    return "instructions" if ops else "registers"


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


DIFF_MARKS = {"same": " ", "register": "!", "operand": "!", "moved": "~", "missing": "-", "extra": "+"}


def show_aligned(rows, counts, ttext, ctext, limit=40):
    """`try`'s diff: target and compiled aligned over the whole function (an extra instruction
    doesn't shift every later row), one line counting each kind, runs of identical rows cut."""
    moved = counts.get("moved", 0) // 2
    print("KINDS " + ", ".join(f"{k} {n}" for k, n in (
        ("register", counts.get("register", 0)), ("operand", counts.get("operand", 0)), ("moved", moved),
        ("missing", counts.get("missing", 0)), ("extra", counts.get("extra", 0))) if n)
        + "  (MATCHING.md, Classify the diff)")
    show = {j for i, r in enumerate(rows) if r[0] != "same" for j in (i - 1, i, i + 1) if 0 <= j < len(rows)}
    print(f"{'':6}{'target':<37}| compiled")
    printed, gap = 0, 0
    for i, (kind, t, c) in enumerate(rows):
        if i not in show:
            gap += 1
            continue
        if printed >= limit:
            rest = sum(1 for r in rows[i:] if r[0] != "same")
            print(f"   ... {rest} more differing rows (DIFF_ROWS=N for more)")
            return
        if gap:
            print(f"   ... {gap} same")
            gap = 0
        tt = f"{t * 4:4x} {ttext[t][:31] if t < len(ttext) else '?'}" if t is not None else ""
        ct = ctext[c][:50] if c is not None and c < len(ctext) else "-"
        print(f"{DIFF_MARKS[kind]} {tt:<36} | {ct}")
        printed += 1
    if gap:
        print(f"   ... {gap} same")


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


_MAKE_VARS = {}


def make_flags(var):
    """A Makefile variable (CPPFLAGS, ASFLAGS) as an argument list, so try/permute/m2c compile
    exactly like the build."""
    if var not in _MAKE_VARS:
        r = run(["make", "-s", "--no-print-directory", f"print-{var}"])
        if r.returncode:
            die(f"make print-{var} failed: {tail(r.stderr + r.stdout)}")
        _MAKE_VARS[var] = r.stdout.split()
    return _MAKE_VARS[var]


def still_asm(src, *names):
    """True while src still has an INCLUDE_ASM line for any of names (its asm matches itself)."""
    return bool(set(names) & {m.group(2) for m in INC_ASM.finditer(read(src))})


PP_LINE = re.compile(r"\s*#\s*(if\w*|else|elif|endif)\b\s*(\w*)")


def non_matching_blocks(text):
    """(C side, #else side) of each `#ifdef NON_MATCHING` block in TEXT, nested #ifs kept whole."""
    out, cur, side, depth = [], None, 0, 0
    for line in text.splitlines(keepends=True):
        m = PP_LINE.match(line)
        d = m.group(1) if m else None
        if cur is None:
            if d == "ifdef" and m.group(2) == "NON_MATCHING":
                cur, side, depth = ["", ""], 0, 0
            continue
        if d and d.startswith("if"):
            depth += 1
        elif d == "endif" and depth == 0:
            out.append(tuple(cur))
            cur = None
            continue
        elif d == "endif":
            depth -= 1
        elif d in ("else", "elif") and depth == 0:
            side = 1
            continue
        cur[side] += line
    return out


def parked_c(src, *names):
    """True when src keeps one of names' C under `#ifdef NON_MATCHING` with its INCLUDE_ASM line in
    the `#else` (a parked function, CONVENTIONS.md): built with -DNON_MATCHING it compiles that C."""
    for c, asm in non_matching_blocks(read(src)):
        inc = {m.group(2) for m in INC_ASM.finditer(asm)}
        if any(n in inc and func_def_regex(n).search(c) for n in names):
            return True
    return False


def c_defines(src, name, e):
    """The cpp flags that give NAME's C in src: none, or -DNON_MATCHING for a parked function's C
    (sched, regions and vars analyse that). Dies when src has no C for it."""
    names = (name, e["name"], e["orig"])
    if src and not still_asm(src, *names):
        return []
    if src and parked_c(src, *names):
        print(f"({name} is parked: reading its C under #ifdef NON_MATCHING)")
        return ["-DNON_MATCHING"]
    die(f"{name}: no C for it yet (still INCLUDE_ASM?)")


LAST_SCORE = {}     # key -> (paired, ok blocks, exact, len, blocks) of the last diff check_function showed


def stall_note(key, e, tries):
    """Log this try's region score in build/attempts/<key>.json and, when the last N tries (more for
    a bigger function) got nothing better than an earlier one, say which try was the closest."""
    s = LAST_SCORE.get(key)
    if not s:
        return None
    path = f"build/attempts/{key.replace(':', '_')}.json"
    log = jload(path, []) if os.path.exists(path) else []
    log.append([tries, *s])
    os.makedirs("build/attempts", exist_ok=True)
    jsave(path, log)
    best = max(log, key=lambda r: (r[1], r[2], r[3]))      # paired, then ok blocks, then exact
    n = min(12, 4 + e["size"] // 128)
    since = sum(1 for r in log if r[0] > best[0])
    if since < n:
        return None
    return (f"(no better in {since} tries: try {best[0]} was the closest, paired {best[1]}/{best[4]}, "
            f"blocks {best[2]}/{best[5]}, exact {best[3]}/{best[4]}; go back to that C if the edits "
            "since went nowhere, or try another idea)")


def check_function(key, e, proj, name, quiet=False):
    """Build e's source file and compare. Returns (ok, ndiffs)."""
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    if still_asm(src, name, e["name"], e["orig"]):
        print(f"STILL-ASM {name}: {src} still has its INCLUDE_ASM line, so its asm would match "
              "itself. Replace that line with the C.")
        return False, None
    obj = f"build/{src}.o"
    if os.path.exists(obj):
        os.remove(obj)
    r = run(["make", "-s", obj])
    if r.returncode:
        print("BUILD FAILED")
        print(compile_error(src, r.stderr + r.stdout))
        return False, None
    return compare_obj(key, e, proj, name, obj, quiet)


def check_file(key, e, proj, name, path):
    """`try --file`: compile the C file PATH (a test copy of the function with its own #includes)
    with the settings of the unit that defines NAME, and compare NAME from it. The unit is not
    edited or rebuilt, so a shape takes a second to try. Returns (ok, ndiffs)."""
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    obj = f"build/tryfile/{os.path.basename(path)}.o"
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    if os.path.exists(obj):
        os.remove(obj)
    r = run("set -o pipefail; " + unit_pipeline(src, obj, mk_config(), c_path=path))
    if r.returncode:
        print("BUILD FAILED")
        print(compile_error(path, r.stderr + r.stdout))
        return False, None
    return compare_obj(key, e, proj, name, obj, link=False)


def file_words(e, name, path):
    """NAME's words built from the test file PATH with its unit's settings, as `try --file`
    builds it (relocations left unapplied)."""
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    obj = f"build/tryfile/protos_{os.path.basename(path)}.o"
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    if os.path.exists(obj):
        os.remove(obj)
    r = run("set -o pipefail; " + unit_pipeline(src, obj, mk_config(), c_path=path))
    if r.returncode:
        die("BUILD FAILED\n" + compile_error(path, r.stderr + r.stdout))
    cw = obj_func(obj, name)[0]
    if not cw:
        die(f"{name} not found in {path}")
    return cw


def try_rank(r):
    """Sort key of one batch result (best first): a match, then the regions score (exact
    instructions, matching blocks, paired instructions), then fewer differing instructions; a
    file that didn't build goes last."""
    s = r.get("score")
    if s is None:
        return (1, 0, 0, 0, 0)
    if s["ndiff"] == 0:
        return (0, -1 << 30, 0, 0, 0)
    return (0, -s["exact"], -s["ok"], -s["paired"], s["ndiff"])


def try_files(key, e, proj, name, paths, jobs=None, top=None, defs=()):
    """`try --file A.c B.c ...`: compile every file as check_file does (with cpp flags DEFS), in
    parallel, rank them by try_rank and print the table (its first TOP rows), then the best one's
    diff. Returns (ok, ndiffs, path) of the best."""
    import regions
    src = e.get("src") or locate_def(name)
    if not src:
        die(f"can't find the source file defining {name}")
    cfg = mk_config()
    make_flags("CPPFLAGS"), make_flags("ASFLAGS")          # cached before the threads use them
    tw, vram = target_words(e, proj), key_vram(key)
    ttext, labels = parse_asm(e["asm"])[3], asm_labels(e["asm"], vram)
    os.makedirs("build/tryfile", exist_ok=True)

    def one(job):
        k, path = job
        obj = f"build/tryfile/{k}_{os.path.basename(path)}.o"
        if os.path.exists(obj):
            os.remove(obj)
        r = run("set -o pipefail; " + unit_pipeline(src, obj, cfg, defs=defs, c_path=path))
        if r.returncode:
            return {"path": path, "error": compile_error(path, r.stderr + r.stdout).splitlines()[-1]}
        cw, _, relocs = obj_func(obj, name)
        if not cw:
            return {"path": path, "error": f"{name} not found in it"}
        diffs, ops, nt, nc = compare(tw, cw, relocs, vram)
        res = regions.score(tw[:nt], cw[:nc], ttext[:nt], relocs, vram, labels)
        return {"path": path, "obj": obj, "score": {
            "stage": diff_stage(tw, cw, diffs, ops, nt, nc), "ndiff": len(diffs), "total": max(nt, nc),
            "exact": res["exact"], "ok": res["ok"], "blocks": res["total"], "paired": res["paired"],
            "len": res["len"]}}
    with ThreadPoolExecutor(jobs or max(1, min(8, (os.cpu_count() or 2) - 1))) as pool:
        out = sorted(pool.map(one, enumerate(paths)), key=try_rank)
    print(f"TRY {name}: {len(paths)} files, best first (exact instructions, then matching blocks, "
          "then paired)")
    w = max(len(r["path"]) for r in out)
    print(f"  {'file':<{w}}  {'stage':<12} {'differ':>9} {'exact':>9} {'blocks':>7} {'paired':>9}")
    for r in out[:top]:
        s = r.get("score")
        if s is None:
            print(f"  {r['path']:<{w}}  BUILD FAILED: {r['error'][:90]}")
            continue
        print(f"  {r['path']:<{w}}  {s['stage']:<12} {s['ndiff']:>4}/{s['total']:<4} "
              f"{s['exact']:>4}/{s['len']:<4} {s['ok']:>3}/{s['blocks']:<3} {s['paired']:>4}/{s['len']:<4}")
    if top is not None and len(out) > top:
        print(f"  ... {len(out) - top} more")
    best = out[0]
    if "obj" not in best:
        return False, None, None
    print(f"\nbest: {best['path']}")
    return (*compare_obj(key, e, proj, name, best["obj"], link=False), best["path"])


def compare_obj(key, e, proj, name, obj, quiet=False, link=True):
    """Compare NAME in the object OBJ with the target and print the diff (or MATCH, with the
    whole binary's link check when LINK). Returns (ok, ndiffs)."""
    cw, ctext, relocs = obj_func(obj, name)
    if not cw:
        print(f"{name} not found in {obj} - still INCLUDE_ASM, misspelled, or static and inlined?")
        return False, None
    tw = target_words(e, proj)
    diffs, ops, nt, nc = compare(tw, cw, relocs, key_vram(key))
    if not diffs:
        if not quiet:
            print(f"MATCH {name} ({nt} instructions)")
            if link:
                # the whole binary too: `done`'s full build would refuse a match that moves data
                print(link_check(e, proj, jload(P_FUNCS, {})))
        return True, 0
    ttext = parse_asm(e["asm"])[3] if os.path.exists(e.get("asm", "")) else [f"{w:08x}" for w in tw]
    stage = diff_stage(tw, cw, diffs, ops, nt, nc)
    print(f"DIFF {name}: {len(diffs)}/{max(nt, nc)} instructions differ "
          f"({ops} opcode, {len(diffs) - ops} operand-only); length target {nt} vs compiled {nc}")
    limit = int(os.environ.get("DIFF_ROWS", "40"))
    try:
        import regions
        vram = key_vram(key)
        tt, ct = tw[:nt], cw[:nc]
        res = regions.score(tt, ct, ttext[:nt], relocs, vram, asm_labels(e["asm"], vram))
        LAST_SCORE[key] = (res["paired"], res["ok"], res["exact"], res["len"], res["total"])
        show_aligned(*regions.diff_rows(tt, ct, ttext[:nt], relocs, res), ttext, ctext, limit)
    except Exception as ex:                          # the positional diff still says something
        print(f"(aligned diff failed: {ex}; positional diff instead)")
        show_diff(diffs, ttext, ctext, nt, nc, limit)
    # one machine-readable line (the swarm's big mode keeps the best C by it)
    print(f"STAGE {name} {stage} {len(diffs)}/{max(nt, nc)} (length {nt} vs {nc})")
    return False, len(diffs)


def pipeline(c_path, o_path, cc, opt, g, maspsx, post="", defs=()):
    gflag = f"-G{g}"
    cpp = " ".join([*make_flags("CPPFLAGS"), *defs])
    return (f"{X}cpp {cpp} {c_path} | {GCC_ROOT}/{cc}/cc1 -quiet {opt} {gflag} "
            f"| python3 tools/scripts/dmpsx.py | python3 {MASPSX} {maspsx}{' ' + gflag if g else ''} "
            f"{post + ' ' if post else ''}| {X}as {' '.join(make_flags('ASFLAGS'))} -o {o_path}")


def unit_cfg(cfg, src):
    """project.mk's settings with the Makefile's per-unit overrides for `src` applied
    (lines `build/<src>.o: VAR = value`)."""
    out = dict(cfg)
    if os.path.exists("Makefile"):
        # the Makefile's default post-maspsx filter (not in project.mk, which write_mk rewrites)
        m = re.search(r"^ASPSX_FILL\s*\?=\s*(.*?)\s*$", read("Makefile"), re.M)
        if m and "ASPSX_FILL" not in out:
            out["ASPSX_FILL"] = m.group(1).replace("$(PYTHON)", "python3")
    if src and os.path.exists("Makefile"):
        pat = re.compile(rf"^build/{re.escape(src)}\.o:\s*([A-Z_0-9]+)\s*:?=\s*(.*?)\s*$")
        for line in read("Makefile").splitlines():
            m = pat.match(line)
            if m:
                out[m.group(1)] = m.group(2).replace("$(PYTHON)", "python3")
    return out


def post_filters(cfg):
    """The Makefile's extra stages after maspsx, as a shell fragment starting with `|`."""
    return " ".join(v for v in (cfg.get("ASPSX_FILL", ""),) if v)


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
    r = run([X + "cpp", *make_flags("CPPFLAGS"), "-DM2CTX", "build/ctx_in.c"])
    write("build/ctx.c", r.stdout if r.returncode == 0 else "")
    return r.stdout if r.returncode == 0 else ""


# macros the permuter keeps as calls (their definitions come back when it compiles), so its result
# still reads gte_ldv0(...) and not the inline asm they expand to, which the gate's lint refuses
PERMUTER_KEEP_MACROS = r"gte_\w+"


def permuter_base(src, decls, body, tag):
    """base.c for decomp-permuter: the file's headers, decls and the function, preprocessed together
    (a preprocessed header alone loses its macros, so a body using NULL etc. wouldn't compile).
    PERMUTER_KEEP_MACROS stay macro calls, through decomp-permuter's own import code."""
    incs = [l for l in read(src).splitlines() if l.lstrip().startswith("#include")]
    p = f"build/permuter/{tag}_in.c"
    write(p, "\n".join(incs + list(decls)) + "\n\n" + body)
    cpp = [X + "cpp", *make_flags("CPPFLAGS"), "-DM2CTX", "-DPERMUTER", p]
    try:
        imp = permuter_import()
        keep = (re.compile(f"^(?:{PERMUTER_KEEP_MACROS})$"), lambda name: "void")
        return imp.preprocess_c_with_macros(cpp, os.getcwd(), keep)[0]
    except Exception as ex:                      # the old way: macros come back expanded
        print(f"(permuter: keeping macros failed: {ex}; expanding them)")
    r = run(cpp)
    return r.stdout if r.returncode == 0 else None


def permuter_import():
    """decomp-permuter's import.py as a module (it has the macro-keeping preprocessor)."""
    import importlib.util
    spec = importlib.util.spec_from_file_location("permuter_import", os.path.join(os.path.dirname(PERMUTER), "import.py"))
    mod = importlib.util.module_from_spec(spec)
    sys.path.insert(0, os.path.dirname(PERMUTER))
    spec.loader.exec_module(mod)
    return mod


def file_decls(text, name):
    """The file's own text before NAME's definition, for the permuter's base (its externs, macros
    and types; a base of the #includes alone doesn't compile a function using them). Other
    functions' definitions become prototypes, but inline ones stay whole (they inline into it);
    #include lines (permuter_base adds them) and #if lines still open at NAME go."""
    m = next((m for m in func_def_regex(name).finditer(text) if not text[m.start()].isspace()), None)
    head = text[:m.start()] if m else ""
    out, i = [], 0
    for d in DEFS.finditer(head):
        if d.start() < i:
            continue
        depth, j = 0, d.end() - 1
        while j < len(head):
            depth += (head[j] == "{") - (head[j] == "}")
            if depth == 0:
                break
            j += 1
        sig = head[d.start():d.end() - 1].rstrip()
        k = re.match(r"(.*?\b" + re.escape(d.group(1)) + r"\b(?:\s*\))?\s*)\((.*?)\)([^()]*)$", sig, re.S)
        if k and ";" in k.group(3):              # K&R: void f(a) u8 *a; {
            sig = k.group(1) + "()"
        out += [head[i:d.start()], head[d.start():j + 1] if re.search(r"\binline\b", sig) else sig + ";"]
        i = j + 1
    lines = ("".join(out) + head[i:]).split("\n")
    open_ifs = []
    for k, line in enumerate(lines):
        s = line.strip()
        if re.match(r"#\s*include\b", s):
            lines[k] = ""
        elif re.match(r"#\s*if", s):
            open_ifs.append([k])
        elif re.match(r"#\s*(else|elif)\b", s) and open_ifs:
            open_ifs[-1].append(k)
        elif re.match(r"#\s*endif\b", s) and open_ifs:
            open_ifs.pop()
    for ks in open_ifs:
        for k in ks:
            lines[k] = ""
    return "\n".join(lines)


def permuter_file_base(src, name, body, tag):
    """permuter_base with the file's own declarations (file_decls), or without them if they
    don't parse for the permuter."""
    base_c = permuter_base(src, [file_decls(read(src), name)], body, tag)
    if base_c is None or prune_context(base_c, name) is None:
        base_c = permuter_base(src, [], body, tag)
    return base_c


IDENT = re.compile(r"[A-Za-z_]\w*")


def prune_context(base_c, name):
    """base_c keeping only the declarations NAME needs, as (text, context), or None if the
    context doesn't parse. The permuter parses and prints the whole file for every candidate, so
    100+ KB of headers cost more than the compile. Kept: the closure over identifiers, from the
    function's text, of the typedefs, tags, enum constants, globals, prototypes and inline helpers
    that define them. The latedefine block and the function stay as they are; the kept
    declarations are printed by pycparser, as the permuter prints them anyway."""
    func = extract_function(base_c, name, top=True)
    at = base_c.find(func.rstrip("\n")) if func else -1
    if at < 0:
        return None
    func = func.rstrip("\n")
    head, rest = base_c[:at], base_c[at + len(func):]
    m = re.search(r"^#pragma _permuter latedefine end[^\n]*\n", head, re.M)
    keep_head, ctx = (head[:m.end()], head[m.end():]) if m else ("", head)
    permuter_path()
    try:
        from perm_pycparser import c_ast as ca, c_generator
        from perm_pycparser.c_parser import CParser
        ast = CParser().parse(ctx, "<context>")
    except Exception:
        return None
    gen = c_generator.CGenerator()

    def defines(node):
        out = set()
        if isinstance(node, ca.FuncDef):
            node = node.decl
        if getattr(node, "name", None):
            out.add(node.name)
        stack = [node.type] if getattr(node, "type", None) else []
        forward = isinstance(node, ca.Decl) and node.name is None    # `struct X;`
        while stack:
            n = stack.pop()
            if isinstance(n, (ca.Struct, ca.Union)) and n.name and (n.decls is not None or forward):
                out.add(n.name)
            elif isinstance(n, ca.Enum) and n.name:
                out.add(n.name)
            elif isinstance(n, ca.Enumerator):
                out.add(n.name)
            stack.extend(c for _, c in n.children())
        return out

    items = ast.ext
    by_name, uses, keep = {}, [], set()
    for i, node in enumerate(items):
        if isinstance(node, ca.Pragma):
            keep.add(i)
        for n in defines(node):
            by_name.setdefault(n, []).append(i)
        uses.append(IDENT.findall(gen.visit(node)))
    todo, seen = IDENT.findall(func + rest), set()
    while todo:
        n = todo.pop()
        if n in seen:
            continue
        seen.add(n)
        for i in by_name.get(n, ()):
            if i not in keep:
                keep.add(i)
                todo.extend(uses[i])
    context = gen.visit(ca.FileAST([items[i] for i in sorted(keep)]))
    # the latedefine block's macros (#pragma _permuter define NAME(...) and `void NAME();`) that
    # are used; none: no block, so a candidate needs no cpp (tools/scripts/permuter_fast.py)
    lines, inside, used = keep_head.split("\n"), False, False
    for k, line in enumerate(lines):
        s = line.strip()
        if s.startswith("#pragma _permuter latedefine"):
            inside = s.endswith("start")
            continue
        if inside:
            mm = re.match(r"(?:#pragma _permuter define\s+|void\s+)(\w+)", s)
            if mm and mm.group(1) not in seen:
                lines[k] = None
            else:
                used = True
    keep_head = "\n".join(line for line in lines if line is not None and
                          (used or not line.strip().startswith("#pragma _permuter latedefine")))
    return keep_head + context + "\n" + func + rest, context


def same_code(d, a_c, b_c):
    """True if a_c and b_c build (d's compile.sh, the permuter's pragmas applied as it applies
    them) to the same .text, relocations included."""
    permuter_path()
    try:
        from src.ast_util import process_pragmas
    except Exception:
        return False
    dumps = []
    for tag, text in (("a", a_c), ("b", b_c)):
        c, o = os.path.abspath(f"{d}/check_{tag}.c"), os.path.abspath(f"{d}/check_{tag}.o")
        write(c, process_pragmas(text))
        ok = run(["bash", f"{d}/compile.sh", c, "-o", o]).returncode == 0
        r = run([X + "objdump", "-drz", "-j", ".text", o]) if ok else None
        for f in (c, o):
            if os.path.exists(f):
                os.remove(f)
        if not r or r.returncode:
            return False
        dumps.append(r.stdout.replace(o, ""))
    return dumps[0] == dumps[1]


def permuter_path():
    """decomp-permuter's directory on sys.path (its perm_pycparser and src packages)."""
    d = os.path.dirname(PERMUTER)
    if d not in sys.path:
        sys.path.insert(0, d)


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
    # the game data, as editable files (docs/ASSETS.md); never over existing edits
    if proj.get("overlays"):
        import dat
        if os.path.exists(dat.ASSETS):
            print(f"{dat.ASSETS}/ kept (it holds your edits; dat.py extract --force starts it again)")
        else:
            dat.extract()


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
    syms = sym_names()
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
        name = syms.get(key, name)     # a .s left from before a rename
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
        elif not e.get("src") or not os.path.exists(e["src"]) or e["src"] != f"src/{seg}.c":
            # no INCLUDE_ASM: matched C, or splat wrote it as C itself (empty `void f(void) {}`); look in
            # its segment's file, which changes when a split moves it
            p = f"src/{seg}.c"
            if os.path.exists(p):
                if p not in src_text:
                    src_text[p] = read(p)
                if func_def_regex(name).search(src_text[p]):
                    e["src"] = p
        gp_funcs += gp
    # every entry against the symbols and src/: a symbol rename gives the name (splat writes no .s
    # for matched C, and a .s from before the rename keeps the old one); an entry that no symbol,
    # INCLUDE_ASM or C definition names any more (a resplit merged it away; its old .s may linger)
    # is dropped
    dropped, defs = [], {}
    for key, e in list(funcs.items()):
        if "psyq" in e.get("seg", ""):
            continue
        name = syms.get(key, e["name"])
        p = f"src/{e['seg']}.c"
        if p not in defs and os.path.exists(p):
            defs[p] = defined_names(src_text.get(p) or read(p))
        if (e["seg"], name) in inc:
            e.update(name=name, src=inc[(e["seg"], name)][0])
        elif name in defs.get(p, ()):
            e.update(name=name, src=p)
        elif key not in syms and p in defs:
            dropped.append(key)
    for key in dropped:
        funcs.pop(key)
        st.pop(key, None)
    if dropped:
        jsave(P_STATE, st)
        print(f"dropped {len(dropped)} entries no symbol, INCLUDE_ASM or C names: {' '.join(dropped)}")
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


def asm_listing(path, limit, start=0, name=None):
    """Compact listing of a function's .s from offset `start`: each instruction at its offset in
    the function (as `try` and `regions` print them), branch labels with theirs, then the
    function's own rodata (strings as text, jump tables as the offsets they go to)."""
    rows, rodata, data, label_off, pending = [], [], [], {}, []
    in_text, base = True, None
    for line in read(path).splitlines():
        s = line.strip()
        if s.startswith(".section"):
            in_text = ".text" in s
            continue
        m = ASM_LINE.search(line)
        if not in_text:
            d = re.match(r"dlabel\s+(\w+)", s)
            if d:
                rodata.append(d.group(1))
                data.append((d.group(1), []))
            elif data and DATA_LINE.search(line):     # a string's line has no instruction word
                data[-1][1].append(re.sub(r"\s+", " ", DATA_LINE.search(line).group(1)).strip())
            continue
        if m:
            off = int(m.group(2), 16) - (base if base is not None else int(m.group(2), 16))
            base = int(m.group(2), 16) - off
            for lab in pending:
                label_off[lab] = off
                rows.append((off, f"  {lab}: (+{off:#x})"))
            pending = []
            rows.append((off, f"  {off:4x}  " + re.sub(r"\s+", " ", m.group(4)).strip()))
        elif re.match(r"^\.L\w+:", s):
            pending.append(s[:-1])
    rows = [r for r in rows if r[0] >= start]
    text = [r[1] for r in rows[:limit]]
    if len(rows) > limit:
        text.append(f"    ... {len(rows) - limit} more lines (decomp.py asm {name or path} --from {rows[limit][0]:#x})")
    for lab, items in data:
        jumps = [re.match(r"\.word\s+(\.L\w+)", x) for x in items]
        if items and all(jumps):
            text.append(f"  {lab}: jump table, {len(jumps)} cases -> " + ", ".join(
                f"+{label_off[j.group(1)]:#x}" if j.group(1) in label_off else j.group(1) for j in jumps))
        else:
            shown = [x for x in items if x.startswith((".asciz", ".ascii"))] or items
            text.append(f"  {lab}: " + " ".join(shown[:4]) + (" ..." if len(shown) > 4 else ""))
    return text, rodata


def callee_lines(key, funcs, syms):
    """What the function calls, each with its prototype where a header has one. A `(void)`
    prototype is flagged: a call that passes arguments to it compiles with none (the mistake
    that made `auto` commit wrong matches)."""
    protos = {}
    header_names([os.path.relpath(h, HEADERS) for h in all_headers()], protos)
    src = funcs[key].get("src")
    for l in (read(src).splitlines() if src and os.path.exists(src) else []):   # its file's own externs
        if file_decl(l):
            protos.setdefault(file_decl(l), l.strip())
    out = []
    for k in sorted(call_graph(funcs)[0].get(key, ())):
        n = cur_name(k, funcs[k], syms)
        p = protos.get(n)
        flag = "  (check arity: the asm may pass arguments)" if p and re.search(r"\(\s*void\s*\)\s*;", p) else ""
        out.append(f"    {p or n + ' (no prototype)'}{flag}")
    return out


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
            # C in place of its INCLUDE_ASM line: matched, or a session resuming earlier C (a port
            # or gate-fix session). The brief is still the evidence; nothing is marked
            print(f"(note: {a.func} already has C in place of its INCLUDE_ASM line; brief for reference)")
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
        if len(targets) == 1 and rec.get("status") != "in_progress" and stat[key] != "matched":
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
        calls = callee_lines(key, funcs, syms)
        if calls:
            print(f"  calls {len(calls)}:")
            print("\n".join(calls))
        lst, rodata = asm_listing(e["asm"], a.asm_lines, name=name)
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
            print(f"... {len(lines) - a.max_lines} more lines (decomp.py m2c {name} --from {a.max_lines + 1})")
        twin = twin_source(key, funcs, stat, syms)
        if twin:
            # the same code is matched elsewhere, but propagate's copy of it didn't fit here
            why = rec.get("copy_failed_stage")
            print(f"--- {twin[1]}'s C (identical code, matched), renamed for {name}"
                  f"{f'; the copy failed: {why}' if why else ''} ---")
            print(twin[2].rstrip())
        elif len(targets) == 1:
            print_template(key, e, name, funcs, stat, syms, proj)
    jsave(P_STATE, st)


def cmd_queue(a):
    """The next targets as JSON, in `next` order, marking nothing (for tools/swarm)."""
    proj, funcs = need_proj(), need_funcs()
    stat, st = statuses(funcs)
    syms = sym_names()
    picked = pick_targets(funcs, stat, st, proj, a.seg, a.count)
    shared = defaultdict(int)
    for k, e in funcs.items():
        shared[cur_name(k, e, syms)] += 1
    # twin: "exact" when an identical function is matched (propagate's copy failed: next prints it),
    # "near" when one differing only in constants is, "template" when a matched look-alike lines up
    # at TEMPLATE_MIN (next prints its C); the swarm treats them as adapt jobs, never big mode
    done_fp = {e.get("fp") for k, e in funcs.items() if stat[k] == "matched" and e["size"] > 8}
    done_fpn = {e.get("fpn") for k, e in funcs.items() if stat[k] == "matched" and e["size"] > 8}
    look = lookalikes(funcs, stat, proj, [k for k, _ in picked])
    # ref: what to pass to the tools. The key when slot overlays share the name (find_func would
    # die "ambiguous" on it), else the name
    out = [{"key": k, "name": cur_name(k, e, syms), "seg": e["seg"], "size": e["size"],
            "ref": k if shared[cur_name(k, e, syms)] > 1 else cur_name(k, e, syms),
            "twin": "exact" if e.get("fp") and e["fp"] in done_fp and e["size"] > 8
            else "near" if e.get("fpn") and e["fpn"] in done_fpn and e["size"] > 8
            else "template" if look.get(k, [0])[0] >= TEMPLATE_MIN else None,
            "src": e.get("src"), "fp": e.get("fp") if e["size"] > 8 else None,
            "tries": st.get(k, {}).get("tries", 0), "note": st.get(k, {}).get("note", "")}
           for k, e in picked]
    print(json.dumps(out))


def cmd_m2c(a):
    funcs = need_funcs()
    key, e = find_func(funcs, a.func)
    ok, draft = m2c_draft(e, e.get("src"))
    lines = draft.rstrip().splitlines()
    print("\n".join(lines[max(a.start, 1) - 1:]))


def cmd_asm(a):
    """A function's compact asm (offsets as try/regions print them), from --from on."""
    funcs = need_funcs()
    key, e = find_func(funcs, a.func)
    if not os.path.exists(e.get("asm", "")):
        die(f"no asm for {a.func} - run `make split`")
    text, _ = asm_listing(e["asm"], a.lines, int(a.start, 0), cur_name(key, e, sym_names()))
    print("\n".join(text))


def cmd_try(a):
    proj, funcs = need_proj(), need_funcs()
    syms = sym_names()
    if a.file:                          # test files: no attempt is recorded
        if len(a.funcs) != 1:
            die("try --file compares one function")
        key, e = find_func(funcs, a.funcs[0])
        name = cur_name(key, e, syms)
        if len(a.file) == 1:
            ok, _ = check_file(key, e, proj, name, a.file[0])
        else:
            ok = try_files(key, e, proj, name, a.file, a.jobs)[0]
        sys.exit(0 if ok else 1)
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
        # big-mode workers have their own measure (regions, checkpoints): normal mode and by hand only
        note = stall_note(key, e, rec["tries"]) if not ok and not os.environ.get("SWARM_BIG") else None
        if note:
            print(note)
        if not ok and rec["tries"] >= 8 and rec["tries"] % 4 == 0:
            if os.environ.get("SWARM_RESULT"):   # a swarm worker: the orchestrator runs the permuter
                print(f"({rec['tries']} tries so far - if no idea is left, finish \"needs_permute\" or "
                      f"\"parked\" in {os.environ['SWARM_RESULT']})")
            else:
                print(f"({rec['tries']} tries so far - consider `permute {name}` or `block {name} --note ...`)")
        all_ok &= ok
    jsave(P_STATE, st)
    sys.exit(0 if all_ok else 1)


def split_points(words, labels):
    """Indices where a function's code looks like the start of another function: code after an
    unconditional jump (and its delay slot) that no branch or jump table reaches, or a second
    stack frame set up after a jump (`addiu $sp,$sp,-N`). Leading nops are skipped."""
    import regions
    n, out = len(words), []
    reached = set(labels)
    for i, w in enumerate(words):
        if regions.is_branch(w):
            reached.add(regions.branch_dest(w, i))
    for i, w in enumerate(words):
        if not regions.is_jump(w) or i + 2 >= n:
            continue
        j = i + 2
        while j < n and words[j] == 0:
            j += 1
        if j >= n or j in reached:
            continue
        frame = (words[j] >> 16) == 0x27BD and words[j] & 0x8000          # addiu $sp, $sp, -N
        out.append((j, "a stack frame set up after a jump" if frame else "code after a jump that nothing reaches"))
    return out


PLACEHOLDER = re.compile(r"\b(func|D|jtbl)_([0-9A-Fa-f]{8})\b")


def diff_parts(diff, names):
    """From a unified diff: (the C that replaced an INCLUDE_ASM line for one of `names`, the other
    added lines as whole declarations). Context and other files' layout are ignored."""
    body, decls = [], []
    for sec in re.split(r"^diff --git ", diff, flags=re.M)[1:]:
        run, after_del = [], False
        for line in sec.split("\n"):
            if line.startswith(("+++", "---")):
                continue
            if line.startswith("-"):
                m = INC_ASM.search(line)
                after_del = bool(m and m.group(2) in names)
                decls += run
                run = []
                continue
            if line.startswith("+"):
                (body if after_del else run).append(line[1:])
                continue
            after_del = False
            decls += run
            run = []
        decls += run
    return "\n".join(body), decls


def port_renames(text, key):
    """Placeholder names (func_/D_/jtbl_ + address) in `text` whose address has a name now: the name
    as seen from `key`'s overlay (its own symbols, then the engine's, then the exe's)."""
    syms = sym_names()
    ovl = key.rpartition(":")[0]
    mp = {}
    for m in PLACEHOLDER.finditer(text):
        addr = m.group(2).upper()
        for k in ([f"{ovl}:{addr}"] if ovl else []) + [f"engine:{addr}", addr]:
            if k in syms:
                if syms[k] != m.group(0):
                    mp[m.group(0)] = syms[k]
                break
    return mp


def decl_units(lines):
    """Lines -> whole declarations: everything up to the semicolon that ends one at brace depth 0
    (a struct or typedef block is one unit). Blank and preprocessor lines are dropped."""
    units, cur, depth = [], [], 0
    # a body's closing brace glued to the next declaration (`}extern u8 D_x[];`, an edit that lost
    # a newline) is two lines
    lines = [x for l in lines for x in (re.match(r"^(\})(\s*(?:extern|static|typedef|void|[su](?:8|16|32))\b.*)$", l).groups()
                                         if re.match(r"^\}\s*(?:extern|static|typedef|void|[su](?:8|16|32))\b", l) else (l,))]
    for l in lines:
        if not cur and (not l.strip() or l.lstrip().startswith("#")):
            continue
        cur.append(l)
        code = re.sub(r"/\*.*?\*/|//.*$", "", l).strip()
        depth += code.count("{") - code.count("}")
        if depth <= 0 and code.endswith((";", "}")):
            units.append(cur)
            cur, depth = [], 0
    return units


def unit_name(unit):
    """The name a declaration unit declares: a typedef's name, else a struct's tag."""
    code = re.sub(r"/\*.*?\*/", "", " ".join(unit), flags=re.S).strip()
    m = re.search(r"(\w+)\s*(?:\[[^\]]*\])?\s*;\s*$", code) if not code.endswith("};") else None
    tag = re.search(r"\b(?:struct|union|enum)\s+(\w+)\s*\{", code)
    return m.group(1) if m else tag.group(1) if tag else None


def cmd_port(a):
    """Re-apply an old diff of NAME's C (a swarm checkpoint) after renames and file splits: the C
    goes in place of NAME's INCLUDE_ASM line wherever that is now, placeholder names become today's
    names by address, and the declarations it added go through declare(). Then try."""
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    syms = sym_names()
    name = cur_name(key, e, syms)
    src = e.get("src")
    diff = read(a.diff)
    body, added = diff_parts(diff, {name, e["name"], e["orig"], f"func_{key.rpartition(':')[2]}"})
    if not body.strip():
        die(f"the diff has no C in place of {name}'s INCLUDE_ASM line")
    if not src or not still_asm(src, name, e["name"], e["orig"]):
        die(f"{name} has no INCLUDE_ASM line to replace (already C?)")
    mp = port_renames(body + "\n" + "\n".join(added), key)
    body, added = rename_syms(body, mp), [rename_syms(l, mp) for l in added]
    units = decl_units(added)
    # an indented line edits an existing declaration (a field added to a struct): that needs a hand
    skipped = [u for u in units if u[0][:1] in (" ", "\t", "}")]
    units = [u for u in units if u not in skipped]
    # helper functions it added (static inline ...) go in the file, just before it
    def header(u):                    # a function definition's first line (after any comment)
        return next((re.match(r"[^;{(/]*?\b(\w+)\s*\([^;]*\)\s*\{", l) for l in u
                     if re.match(r"[^;{(/]*?\b\w+\s*\([^;]*\)\s*\{", l)), None)
    defs = [u for u in units if u[-1].strip() == "}" and header(u)]
    units = [u for u in units if u not in defs]
    text = read(src)
    helpers = "".join("\n".join(u) + "\n\n" for u in defs if not func_def_regex(header(u).group(1)).search(text))
    m = next(x for x in INC_ASM.finditer(text) if x.group(2) in (name, e["name"], e["orig"]))
    write(src, text[:m.start()] + helpers + body.rstrip("\n") + text[m.end() + (1 if text[m.end():m.end() + 1] == ";" else 0):])
    # one-line declarations through declare() (it skips what the file sees); whole struct/typedef
    # blocks into the owned header unless their name is visible already
    declare(src, [u[0] for u in units if len(u) == 1 and decl_name(u[0])])
    have = visible_names(src)
    blocks = [u for u in units if len(u) > 1 and unit_name(u) and unit_name(u) not in have]
    if skipped:
        print(f"not ported ({sum(len(u) for u in skipped)} lines that edit existing declarations; redo by hand):")
        print("\n".join("  " + l for u in skipped for l in u))
    if blocks:
        h = owned_header(src)
        ht = read(h)
        end = ht.rfind("#endif")
        write(h, ht[:end] + "".join("\n".join(u) + "\n\n" for u in blocks) + ht[end:])
    print(f"ported {name} into {src}" + (f"; renamed {', '.join(f'{k} -> {v}' for k, v in mp.items())}" if mp else ""))
    ok, _ = check_function(key, e, proj, name)
    sys.exit(0 if ok else 1)


def cmd_splitcheck(a):
    """Functions that are probably two (a mis-split, like func_80027408): each with the line to add
    to its symbol file, so splat splits it."""
    proj, funcs = need_proj(), need_funcs()
    stat, _ = statuses(funcs)
    syms = sym_names()
    known = set()
    for k in syms:
        known.add(k)
    found = 0
    for k, e in sorted(funcs.items()):
        if a.seg and not e["seg"].startswith(a.seg):
            continue
        if not a.all and stat.get(k) == "matched" or not os.path.exists(e.get("asm", "")):
            continue
        rom, vram, words, *_ = parse_asm(e["asm"])
        words = [int.from_bytes(w.to_bytes(4, "big"), "little") for w in words]
        words = words[:trim_nops(words)]
        for j, why in split_points(words, asm_labels(e["asm"], vram)):
            addr = vram + 4 * j
            ovl = k.rpartition(":")[0]
            nk = f"{ovl}:{addr:08X}" if ovl else f"{addr:08X}"
            if nk in known or nk in funcs:
                continue
            found += 1
            print(f"{e['seg']}/{cur_name(k, e, syms)} +0x{4 * j:X}: {why}. Add to {sym_file(ovl)}:\n"
                  f"  func_{addr:08X} = 0x{addr:08X}; // type:func")
    print(f"{found} probable split point(s){' (matched functions included)' if a.all else ''}")


LABEL_LINE = re.compile(r"^\s*\.L([0-9A-Fa-f]{8}):")


def asm_labels(path, vram):
    """Indices of the instructions the function's `.L` labels point at (branch and jump-table
    targets)."""
    return [(int(m.group(1), 16) - vram) // 4 for m in map(LABEL_LINE.match, read(path).splitlines()) if m]


def unit_pipeline(src, o, cfg, opt="", defs=(), c_path=None):
    """pipeline() for src with its unit's settings, extra cc1 flags OPT and cpp flags DEFS. With
    C_PATH, that file is compiled instead of src (src still picks the settings)."""
    cfg = unit_cfg(cfg, src)
    return pipeline(c_path or src, o, cfg["CC_VER"], f"{cfg['OPT_FLAGS']} {cfg.get('EXTRA_CC1FLAGS', '')} {opt}".strip(),
                    int(cfg.get("GVAL", "0")), re.sub(r"\s*-G\d+", "", cfg.get("MASPSX_ARGS", DEFAULT_MASPSX)),
                    post_filters(cfg), defs)


def c_lines(src, name, cfg, defs=()):
    """Compiled instruction index -> source line, from a -g build of `src` (None if the build
    fails). cpp runs without -P here, so its line markers give cc1 the real file and lines (-P
    squeezes blank lines, so an offset from the function's first line drifts). Code inlined from
    a header maps to None."""
    os.makedirs("build/regions", exist_ok=True)
    o = f"build/regions/{name}.o"
    cmd = unit_pipeline(src, o, cfg, "-g", defs)
    cmd = re.sub(r"^(\S*cpp\b[^|]*?)\s-P\b", r"\1", cmd, count=1)
    if run("set -o pipefail; " + cmd).returncode:
        return None
    r = run([X + "objdump", "-dl", f"--disassemble={name}", o])
    out, line, base = {}, None, None
    want = os.path.basename(src)
    for l in r.stdout.splitlines():
        m = re.match(r"^(\S+):(\d+)(?: \(discriminator \d+\))?$", l)
        if m:
            line = int(m.group(2)) if os.path.basename(m.group(1)) == want else None
            continue
        m = OBJ_LINE.match(l)
        if m:
            off = int(m.group(1), 16)
            base = off if base is None else base
            out[(off - base) // 4] = line
    return out


def region_score(key, e, proj, name, src, want_lines=True, defs=()):
    """Build src and score NAME block by block: (res, lines, tw, ttext, cw, ctext) with lines the
    compiled index -> source line map (None without want_lines or when -g changes the code).
    DEFS (c_defines) compile a parked function's C, into its own object: the build's keeps the asm."""
    import regions
    obj = f"build/regions/{name}.nm.o" if defs else f"build/{src}.o"
    if os.path.exists(obj):
        os.remove(obj)
    # the -g build for the C lines runs alongside the normal one (two compiles of the same file)
    pool = ThreadPoolExecutor(1) if want_lines else None
    lines_job = pool.submit(c_lines, src, name, mk_config(), defs) if pool else None
    if defs:
        os.makedirs("build/regions", exist_ok=True)
        r = run("set -o pipefail; " + unit_pipeline(src, obj, mk_config(), defs=defs))
    else:
        r = run(["make", "-s", obj])
    if r.returncode:
        print("BUILD FAILED")
        print(compile_error(src, r.stderr + r.stdout))
        sys.exit(1)
    cw, ctext, relocs = obj_func(obj, name)
    if not cw:
        die(f"{name} not found in {obj}")
    tw = target_words(e, proj)
    vram = key_vram(key)
    _, _, _, ttext, _, _ = parse_asm(e["asm"])
    nt, nc = trim_nops(tw), trim_nops(cw)
    tw, ttext, cw, ctext = tw[:nt], ttext[:nt], cw[:nc], ctext[:nc]
    res = regions.score(tw, cw, ttext, relocs, vram, asm_labels(e["asm"], vram))
    lines = lines_job.result() if lines_job else None
    if pool:
        pool.shutdown()
    return res, lines, tw, ttext, cw, ctext


def block_span(b, lines):
    """(first, last) source line of a block's compiled instructions, or None."""
    if not lines or b["cstart"] is None:
        return None
    ls = sorted({lines[c] for c in range(b["cstart"], b["cend"]) if lines.get(c)})
    return (ls[0], ls[-1]) if ls else None


REG_NAMES = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
             "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]


def var_homes(src, name, cfg, defs=(), c_path=None):
    """{variable: home} for NAME's parameters and locals, from cc1 -g's ECOFF .def lines: a
    register name, or "sp+N" for a stack slot. Block-scoped locals are included. With C_PATH,
    that file is compiled with src's unit settings."""
    cfg = unit_cfg(cfg, src)
    cmd = (f"{X}cpp {' '.join([*make_flags('CPPFLAGS'), *defs])} {c_path or src} | {GCC_ROOT}/{cfg['CC_VER']}/cc1 -quiet "
           f"{cfg['OPT_FLAGS']} {cfg.get('EXTRA_CC1FLAGS', '')} -G{cfg.get('GVAL', '0')} -g")
    out = run(cmd).stdout
    m = re.search(rf"^{re.escape(name)}:\s*$(.*?)^\s*\.end\s+{re.escape(name)}\b", out, re.M | re.S)
    homes = {}
    for d in re.finditer(r"\.def\s+(\w+);\s*\.val\s+(-?\d+);\s*\.scl\s+(\d+);", m.group(1) if m else ""):
        var, val, scl = d.group(1), int(d.group(2)), int(d.group(3))
        if scl in (4, 17) and 0 <= val < 32:               # register local, register parameter
            homes[var] = REG_NAMES[val]
        elif scl in (1, 9):                                 # auto, argument on the stack
            homes[var] = f"sp{val:+d}"
    return homes


def reg_fields(w):
    """The register fields an instruction uses: rs, rt (and rd for SPECIAL)."""
    f = [(w >> 21) & 31, (w >> 16) & 31]
    if w >> 26 == 0:
        f.append((w >> 11) & 31)
    return f


def cmd_vars(a):
    """Which C variables the register differences of the differing blocks belong to: each
    block's compiled -> target register swaps, the variables cc1 put in the compiled register,
    and of those the ones the block's C lines name. One variable wanting different registers in
    different blocks is a split (`splitvars`)."""
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    defs = c_defines(src, name, e)
    res, lines, tw, _, cw, _ = region_score(key, e, proj, name, src, defs=defs)
    homes = var_homes(src, name, mk_config(), defs)
    if not homes:
        die("no variable homes from cc1 -g (no .def lines for this function)")
    if a.all:
        by = {}
        for v, h in homes.items():
            by.setdefault(h, []).append(v)
        print("homes: " + "; ".join(f"{h}: {', '.join(vs)}" for h, vs in sorted(by.items())))
    src_lines = read(src).split("\n")
    pairs, exact = res["pairs"], res["exact_map"]
    wants = {}                                              # var -> {target reg: [blocks]}
    for k, b in enumerate(res["blocks"]):
        if b["status"] == "ok":
            continue
        swaps = {}
        for t in range(b["start"], b["end"]):
            c = pairs.get(t)
            if c is None or exact.get(t):
                continue
            for tf, cf in zip(reg_fields(tw[t]), reg_fields(cw[c])):
                if tf != cf:
                    swaps[(cf, tf)] = swaps.get((cf, tf), 0) + 1
        span = block_span(b, lines)
        text = "\n".join(src_lines[span[0] - 1:span[1]]) if span else ""
        where = f"L{span[0]}-{span[1]}" if span else "?"
        print(f"block {k} ({b['status']}, {where}):")
        if not swaps:
            print("  no register swaps (opcodes or immediates differ)")
        for (cf, tf), n in sorted(swaps.items(), key=lambda x: -x[1]):
            creg, treg = REG_NAMES[cf], REG_NAMES[tf]
            held = [v for v, h in homes.items() if h == creg]
            named = [v for v in held if re.search(rf"\b{re.escape(v)}\b", text)]
            for v in named:
                wants.setdefault(v, {}).setdefault(treg, []).append(k)
            others = ", ".join(v for v in held if v not in named)
            if named:
                who = ", ".join(named) + (f" (also in {creg}: {others})" if others else "")
            else:
                who = f"in {creg}: {others}" if held else "a temporary"
            print(f"  {creg} -> {treg} x{n}: {who}")
    for v, ts in wants.items():
        here = homes.get(v)
        blocks = sorted({k for ks in ts.values() for k in ks})
        if len(blocks) > 1 or len(ts) > 1:
            want = ", ".join("%s (blocks %s)" % (t, ", ".join(map(str, ks))) for t, ks in ts.items())
            print(f"{v} is in {here} but the target wants {want}: if {v} keeps {here} in the blocks "
                  f"that match, the original had several variables: `decomp.py splitvars {name} {v}`")


REGIONS_ROWS = 12      # differing blocks listed by default (big functions have 20+)


def cmd_regions(a):
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    defs = c_defines(src, name, e)
    res, lines, tw, ttext, cw, ctext = region_score(key, e, proj, name, src, not a.no_lines, defs)
    nt, nc = len(tw), len(cw)

    def span(b):
        s = block_span(b, lines)
        return "" if not s else f"L{s[0]}" + (f"-{s[1]}" if s[1] != s[0] else "")

    bad = [k for k, b in enumerate(res["blocks"]) if b["status"] != "ok"]
    # each call remembers the blocks' state, so --changed can list only what moved since the last
    state_path = f"build/regions/{key.replace(':', '_')}.json"
    last = jload(state_path, {}) if os.path.exists(state_path) else {}
    now = {str(b["start"]): [b["status"], b["exact"]] for b in res["blocks"]}
    os.makedirs("build/regions", exist_ok=True)
    jsave(state_path, now)
    changed = [k for k, b in enumerate(res["blocks"]) if last and last.get(str(b["start"])) != now[str(b["start"])]]
    if a.changed:
        rows = changed
        print(f"changed since the last regions call: {len(changed)} block(s)" if last
              else "no earlier regions call for this function: all differing blocks")
        if not last:
            rows = bad
    else:
        rows = list(range(len(res["blocks"]))) if a.all else bad[:REGIONS_ROWS]
    print(f"{'#':>4} {'target':<13} {'len':>4} {'status':<9} {'exact':>7}  C")
    for k in rows:
        b = res["blocks"][k]
        was = last.get(str(b["start"])) if a.changed and last else None
        print(f"{k:>4} {b['start'] * 4:#06x}-{b['end'] * 4:#06x} {b['end'] - b['start']:>4} {b['status']:<9} "
              f"{b['exact']:>3}/{b['end'] - b['start']:<3}  {span(b)}"
              + (f"  (was {was[0]} {was[1]})" if was else ""))
    if not a.all and not a.changed and len(bad) > REGIONS_ROWS:
        print(f"   ... {len(bad) - REGIONS_ROWS} more differing blocks (--all lists every block)")
    if res["extra"]:
        print(f"extra compiled instructions (no target partner): {len(res['extra'])} at "
              + " ".join(f"{c * 4:#x}" for c in res["extra"][:12]) + (" ..." if len(res["extra"]) > 12 else ""))

    show = a.block if a.block is not None else (bad[0] if bad else None)
    if show is not None and 0 <= show < len(res["blocks"]):
        b, pairs, ex = res["blocks"][show], res["pairs"], res["exact_map"]
        print(f"\nblock {show} ({b['status']}):")
        print(f"{'':6}{'target':<38}| compiled")
        lo = max(b["start"] - 2, 0)
        hi = min(b["end"] + 2, nt)
        rows, prev = [], (pairs.get(lo - 1, -1) if lo else -1)
        for t in range(lo, hi):
            c = pairs.get(t)
            if c is not None:
                for x in range(prev + 1, c):             # compiled-only instructions before this pair
                    rows.append((None, x))
                prev = c
            rows.append((t, c))
        for t, c in rows:
            inside = t is not None and b["start"] <= t < b["end"]
            mark = "+" if t is None else ("!" if inside and not ex.get(t, False) else " ")
            tt = f"{t * 4:4x} {ttext[t][:32]}" if t is not None else ""
            ct = f"{ctext[c][:50]}" if c is not None else "-"
            ln = f"  L{lines[c]}" if lines and c is not None and lines.get(c) else ""
            print(f"{mark} {tt:<37}| {ct}{ln}")

    print(f"REGIONS {name} blocks {res['ok']}/{res['total']} insns {res['exact']}/{res['len']} "
          f"({100 * res['exact'] / max(res['len'], 1):.1f}%) paired {res['paired']}/{res['len']} "
          f"differ {len(bad)} first {bad[0] if bad else '-'}")
    # a difference is the normal result, not an error: only a failed build exits non-zero
    sys.exit(0)


def verify_build(proj, funcs=None):
    shutil.rmtree("build/src", ignore_errors=True)
    r = run(["make", "-s", f"-j{os.cpu_count() or 1}", "all"])  # ~5x faster, same bytes
    if r.returncode:
        out = r.stderr + r.stdout
        # cc1's errors ("stdin:7427: too few arguments ...") carry no "error" word; without them the
        # gate says only "Error 33" and the cause is lost. Its warnings would crowd them out
        errs = [l for l in out.splitlines()
                if re.search(r"error|undefined reference|No rule|Error \d|^\S+:\d+:\s+(?!\s|warning:|info:)", l, re.I)]
        return False, "BUILD FAILED\n" + ("\n".join(errs[:20]) if errs else tail(out, 20))
    oks = []
    for built_p, target_p, binkey in binaries(proj):
        diff = first_difference(built_p, target_p, binkey, funcs)
        if diff is None:
            oks.append(f"{built_p} ({os.path.getsize(built_p)} bytes)")
            continue
        return False, f"MISMATCH in {built_p}: {diff}"
    return True, "OK: " + ", ".join(oks) + " match"


def binaries(proj):
    """(built, target, functions.json `bin` value) for the exe and every overlay."""
    return ([(f"build/{proj['name']}.exe", os.path.join("disc", proj["exe"]), None)]
            + [(f"build/{o['name']}.bin", f"disc/{o['name']}.bin", f"disc/{o['name']}.bin")
               for o in proj.get("overlays", [])])


def first_difference(built_p, target_p, binkey, funcs):
    """None when the two files are equal, else where they first differ (the function, or the
    functions around a data difference) and both sides disassembled there."""
    with open(built_p, "rb") as f:
        built = f.read()
    with open(target_p, "rb") as f:
        target = f.read()
    if built == target:
        return None
    off = next((i for i in range(min(len(built), len(target))) if built[i] != target[i]),
               min(len(built), len(target)))
    mine = sorted((e["rom"], e["size"], e["name"]) for e in (funcs or {}).values() if e.get("bin") == binkey and "rom" in e)
    where = None
    for rom, size, name in mine:
        if rom <= off < rom + size:
            where = f"inside {name} +0x{off - rom:X}"
    if where is None:
        before = [n for r, s, n in mine if r + s <= off]
        after = [n for r, s, n in mine if r > off]
        where = ("outside the functions (data, rodata or padding)"
                 + (f", after {before[-1]}" if before else "") + (f", before {after[0]}" if after else ""))
    out = [f"first difference at file offset 0x{off:X}, {where}; size built {len(built)} vs target {len(target)}"]
    lo = max(off - off % 4 - 8, 0)
    for label, path in (("target", target_p), ("built", built_p)):
        r = run(["mipsel-linux-gnu-objdump", "-D", "-b", "binary", "-m", "mips:3000", "-EL",
                 f"--start-address={lo:#x}", f"--stop-address={lo + 24:#x}", path])
        rows = [l.strip() for l in r.stdout.splitlines() if re.match(r"\s*[0-9a-f]+:\s", l)]
        if rows:
            out.append(f"  {label}:" + "".join(f"\n    {x}" for x in rows))
    return "\n".join(out)


def link_check(e, proj, funcs):
    """After a function matches on its own: link its binary (incrementally) and compare it whole.
    A MATCH that breaks the link (data or rodata that moved) shows up here, before `done`."""
    built_p, target_p, binkey = next(b for b in binaries(proj) if b[2] == e.get("bin"))
    r = run(["make", "-s", built_p])
    if r.returncode:
        return f"LINK failed: {tail(r.stderr + r.stdout, 4)}"
    diff = first_difference(built_p, target_p, binkey, funcs)
    return "LINK ok" if diff is None else f"LINK differs: {diff}"


def cmd_verify(a):
    proj = need_proj()
    ok, msg = verify_build(proj, jload(P_FUNCS, {}))
    print(msg)
    sys.exit(0 if ok else 1)


# every spelling of an inline asm statement: asm, __asm, __asm__, each optionally volatile
# (tools/swarm/swarm.mjs checkPatch refuses the same in a worker's patch)
ASM_CALL = r"\b(?:asm|__asm|__asm__)\s*(?:(?:volatile|__volatile|__volatile__)\s*)?\("
# m2c leftovers and anything that isn't C, which a hand match must not contain (docs/CONVENTIONS.md,
# Matching rules): inline asm of every kind, register pins and empty barriers included
LINT_ERRORS = [
    (re.compile(r"\bM2C_\w+"), "m2c macro"),
    # where a type goes (a declaration, a cast, a parameter), so a ternary's `?` doesn't count
    (re.compile(r"(^[ \t]*|[(,][ \t]*)\?(\s|\*)", re.M), "m2c unknown type `?`"),
    (re.compile(ASM_CALL), "inline asm (only C is allowed)"),
]
INCLUDE_ASM_LINE = re.compile(r'^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*(\w+)\s*\)')


def lint_function(src, name):
    """Errors found in NAME's definition in SRC and in the macros and static inline functions it
    uses: moving asm into a header doesn't make it C."""
    body = extract_function(read(src), name) or ""
    return sorted(set(lint_body(body)) | set(helper_errors(src, body)))


# headers recovered from the leaked original source: their macros are the game's own
ORIGINAL_HEADERS = {"include/bash/pogo.h", "include/bash/arkenoid.h", "include/bash/polar.h",
                    "include/bash/tank.h"}
# the SDK's GTE macros (coprocessor instructions C can't express) and the INCLUDE_ASM machinery
SDK_HELPER = re.compile(r"gte_\w+|INCLUDE_ASM|INCLUDE_RODATA")
INCLUDE_LINE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)
DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)((?:[^\n\\]|\\.|\\\n)*)", re.M)
STATIC_INLINE = re.compile(r"^static\s+(?:__inline__\s+|__inline\s+|inline\s+)[^;{(]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)
_helper_cache = {}


def included_files(src):
    """SRC and every header it includes, transitively, that exists under include/ (or beside it)."""
    seen, todo = [], [src.replace("\\", "/")]
    while todo:
        p = todo.pop()
        if p in seen or not os.path.isfile(p):
            continue
        seen.append(p)
        for inc in INCLUDE_LINE.findall(read(p)):
            todo += [f"include/{inc}", os.path.join(os.path.dirname(p), inc).replace("\\", "/")]
    return seen


def helper_defs(src):
    """{name: (file, errors, text)} for the macros and static inline functions visible in SRC
    (outside the original headers and the SDK's), with what the lint finds in each one's text."""
    if src in _helper_cache:
        return _helper_cache[src]
    defs = {}
    for p in included_files(src):
        if p in ORIGINAL_HEADERS:
            continue
        text = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), read(p), flags=re.S)
        for m in DEFINE.finditer(text):
            if not SDK_HELPER.fullmatch(m.group(1)):
                body = m.group(2).replace("\\\n", "\n")
                defs.setdefault(m.group(1), (p, lint_body(body), body))
        for m in STATIC_INLINE.finditer(text):
            body = extract_function(text[m.start():], m.group(1), top=True) or ""
            defs.setdefault(m.group(1), (p, lint_body(body), body))
    _helper_cache[src] = defs
    return defs


def helper_errors(src, body):
    """Errors in the macros and static inline functions BODY uses (and the ones they use), as
    `<error> in <name> (<file>)`."""
    defs, out, seen = helper_defs(src), [], set()
    todo = [w for w in dict.fromkeys(re.findall(r"\b[A-Za-z_]\w*\b", re.sub(r"/\*.*?\*/", "", body, flags=re.S)))
            if w in defs]
    while todo:
        w = todo.pop(0)
        if w in seen:
            continue
        seen.add(w)
        f, errs, text = defs[w]
        out += [f"{x} in {w} ({f})" for x in errs]
        todo += [x for x in re.findall(r"\b[A-Za-z_]\w*\b", text) if x in defs and x not in seen]
    return out


def lint_body(body):
    """Errors found in a function's definition BODY."""
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    return sorted({why for rx, why in LINT_ERRORS if rx.search(body)})


def matched_in_worktree(path="src"):
    """Functions whose INCLUDE_ASM line the working tree removes from PATH compared with HEAD."""
    r = run(["git", "diff", "HEAD", "-U0", "--", path])
    gone = {m.group(1) for l in r.stdout.splitlines() if l.startswith("-") and not l.startswith("---")
            for m in [INCLUDE_ASM_LINE.match(l[1:])] if m}
    back = {m.group(1) for l in r.stdout.splitlines() if l.startswith("+") and not l.startswith("+++")
            for m in [INCLUDE_ASM_LINE.match(l[1:])] if m}
    return gone - back


DEF_CANDIDATE = re.compile(r"^[A-Za-z_][^;#{}\n]*?\b(\w+)\s*\)?\s*\(", re.M)


def stray_edits(old, new, names):
    """C functions defined in both OLD and NEW (one file's text) whose bodies change on a line that
    doesn't mention any of NAMES. A match's commit may touch other functions only to fix calls to
    it; any other edit (2026-10-19: `goto done` -> `return 1` in a matched neighbour) is out of
    scope, and the full build would only report it as a size or data difference."""
    out = []
    for fn in sorted({m.group(1) for m in DEF_CANDIDATE.finditer(new)} - set(names)):
        a, b = extract_function(old, fn, top=True), extract_function(new, fn, top=True)
        if a is None or b is None or a == b:
            continue
        changed = [l[1:] for l in difflib.unified_diff(a.splitlines(), b.splitlines(), lineterm="", n=0)
                   if l[:1] in "+-" and not l.startswith(("+++", "---"))]
        if any(not any(re.search(rf"\b{re.escape(n)}\b", l) for n in names) for l in changed if l.strip()):
            out.append(fn)
    return out


def edited_in_worktree(names, path="src"):
    """stray_edits over every C file the working tree changes under PATH compared with HEAD."""
    out = []
    for p in run(["git", "diff", "HEAD", "--name-only", "--", path]).stdout.split():
        if not p.endswith(".c") or not os.path.exists(p):
            continue
        old = run(["git", "show", f"HEAD:{p}"])
        if old.returncode == 0:
            out += [f"{fn} ({p})" for fn in stray_edits(old.stdout, read(p), names)]
    return out


def cmd_sched(a):
    """Why cc1 allocated and scheduled NAME as it did (tools/scripts/schedx.py)."""
    import schedx
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    defs = c_defines(src, name, e)
    cfg = unit_cfg(mk_config(), src)
    d = "build/sched"
    os.makedirs(d, exist_ok=True)
    i = f"{d}/{name}.i"
    r = run(f"{X}cpp {' '.join([*make_flags('CPPFLAGS'), *defs])} {src} > {i}")
    if r.returncode:                    # e.g. ext/ missing: cc1 would dump an empty file
        die(f"cpp failed: {r.stderr.strip()[-300:]}")
    r = run(f"cd {d} && {GCC_ROOT}/{cfg['CC_VER']}/cc1 -quiet {cfg['OPT_FLAGS']} "
            f"{cfg.get('EXTRA_CC1FLAGS', '')} -G{cfg.get('GVAL', '0')} -dl -dg -dS -dR "
            f"-o {name}.s {name}.i")
    if r.returncode:
        die(f"cc1 failed: {r.stderr.strip()[-300:]}")
    dumps = {k: read(f"{i}.{k}") for k in ("lreg", "greg", "sched", "sched2") if os.path.exists(f"{i}.{k}")}
    homes = var_homes(src, name, mk_config(), defs)
    insn = {int(x) for x in a.insn.split(",")} if a.insn else None
    print(schedx.report(dumps, name, homes, a.passname, a.block, insn))


def cmd_jtrace(a):
    """Which copy of each repeated tail jump2 keeps for NAME, step by step (tools/scripts/jtrace.py)."""
    import jtrace
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    defs = c_defines(src, name, e)
    cfg = unit_cfg(mk_config(), src)
    d = "build/jtrace"
    os.makedirs(d, exist_ok=True)
    i = f"{d}/{name}.i"
    r = run(f"{X}cpp {' '.join([*make_flags('CPPFLAGS'), *defs])} {src} > {i}")
    if r.returncode:
        die(f"cpp failed: {r.stderr.strip()[-300:]}")
    cc1 = f"{GCC_ROOT}/{cfg['CC_VER']}/cc1"
    flags = [*cfg["OPT_FLAGS"].split(), *cfg.get("EXTRA_CC1FLAGS", "").split(), f"-G{cfg.get('GVAL', '0')}"]
    r = run(f"cd {d} && {cc1} -quiet {' '.join(flags)} -dg -o {name}.s {name}.i")
    if r.returncode:
        die(f"cc1 failed: {r.stderr.strip()[-300:]}")
    names = jtrace.insns(read(f"{i}.greg"), name)
    runs = jtrace.trace([cc1, "-quiet", *flags, "-o", f"{d}/{name}.jt.s", i], name)
    if not runs:
        die(f"no jump2 run for {name} (is it compiled from {src}?)")
    print(jtrace.report(runs, names, a.cross))


def unit_lines(src, cfg):
    """{function: {instruction index: source line}} for every function in the unit src, from one
    -g build (cpp keeps its line markers, as in c_lines). Empty when the build fails."""
    os.makedirs("build/idiom", exist_ok=True)
    o = f"build/idiom/{src.replace('/', '_')}.o"
    cmd = re.sub(r"^(\S*cpp\b[^|]*?)\s-P\b", r"\1", unit_pipeline(src, o, cfg, "-g"), count=1)
    if run("set -o pipefail; " + cmd).returncode:
        return {}
    return objdump_lines(run([X + "objdump", "-dl", o]).stdout, os.path.basename(src))


def objdump_lines(text, want):
    """{function: {instruction index: line in the file named WANT}} from `objdump -dl` TEXT."""
    out, cur, line, base, pending = {}, None, None, None, None
    for l in text.splitlines():
        # a function's header is followed by `name():`; -g's LM<n> line labels look the same but aren't
        m = re.match(r"^[0-9a-f]+ <([\w.$]+)>:$", l)
        if m:
            pending = m.group(1)
            continue
        if pending and l == f"{pending}():":
            cur, base, line = out.setdefault(pending, {}), None, None
            pending = None
            continue
        pending = None
        m = re.match(r"^(\S+):(\d+)(?: \(discriminator \d+\))?$", l)
        if m:
            line = int(m.group(2)) if os.path.basename(m.group(1)) == want else None
            continue
        m = OBJ_LINE.match(l)
        if m and cur is not None:
            off = int(m.group(1), 16)
            base = off if base is None else base
            cur[(off - base) // 4] = line
    return out


def asm_texts(funcs):
    """{key: instruction texts} of every function that has an asm file. Reading ~2500 of them
    through Docker's bind mount takes ~10 s, so they are kept in build/idiom/asm.json until
    functions.json changes (`index` rewrites it after every split)."""
    st = os.stat(P_FUNCS)
    stamp, path = f"{st.st_mtime_ns}:{st.st_size}", "build/idiom/asm.json"
    cache = jload(path, {}) if os.path.exists(path) else {}
    if cache.get("stamp") == stamp:
        return cache["texts"]
    texts = {k: parse_asm(e["asm"])[3] for k, e in funcs.items() if e.get("asm") and os.path.exists(e["asm"])}
    os.makedirs("build/idiom", exist_ok=True)
    with open(path, "w", encoding="utf-8") as f:
        json.dump({"stamp": stamp, "texts": texts}, f, separators=(",", ":"))
    return texts


def cmd_idiom(a):
    """Matched functions whose asm has an instruction pattern, smallest first, with the C lines
    behind each hit (tools/scripts/idiom.py has the pattern syntax)."""
    import idiom
    proj, funcs = need_proj(), need_funcs()
    try:
        pat = idiom.pattern(a.pattern)
    except ValueError as ex:
        die(str(ex))
    stat, syms = statuses(funcs)[0], sym_names()
    want = ("matched", "todo", "blocked", "in_progress") if a.all else ("matched",)
    every = asm_texts(funcs)
    hits, searched = [], 0
    for k, e in sorted(funcs.items(), key=lambda kv: (kv[1]["size"], kv[0])):
        if stat[k] not in want or (a.seg and not e["seg"].startswith(a.seg)) or k not in every:
            continue
        searched += 1
        texts = every[k]
        for s, end, b in idiom.find(pat, [idiom.insn(t) for t in texts], a.gap):
            hits.append((k, e, texts, s, end, b))
    nfunc = len({h[0] for h in hits})
    print(f"IDIOM {a.pattern!r}: {len(hits)} hit(s) in {nfunc} of {searched} "
          f"{'functions' if a.all else 'matched functions'}" + (f" (showing {a.top})" if nfunc > a.top else ""))
    keep = list(dict.fromkeys(h[0] for h in hits))[:a.top]
    shown = [h for h in hits if h[0] in keep]
    units = sorted({h[1]["src"] for h in shown if stat[h[0]] == "matched" and h[1].get("src")})
    cfg = mk_config()
    make_flags("CPPFLAGS"), make_flags("ASFLAGS")          # cached before the threads use them
    with ThreadPoolExecutor(max(1, min(8, len(units)))) as pool:
        lines = dict(zip(units, pool.map(lambda s: unit_lines(s, cfg), units)))
    for k, e, texts, s, end, b in shown:
        name = cur_name(k, e, syms)
        lm = lines.get(e.get("src"), {}).get(name, {})
        print(f"\n{name}  {e['seg']}  +{s * 4:#x}  ({e['size']} B)"
              + ("  " + " ".join(f"{x}={v}" for x, v in sorted(b.items())) if b else ""))
        for i in range(max(0, s - a.context), min(len(texts), end + a.context)):
            ln = lm.get(i)
            print(f"  {'>' if s <= i < end else ' '} {i * 4:4x}  {texts[i]:<44}{f'  L{ln}' if ln else ''}")
        ls = sorted({lm[i] for i in range(s, end) if lm.get(i)})
        src_lines = read(e["src"]).split("\n") if ls else []
        for ln in ls:
            print(f"    {e['src']}:{ln}  {src_lines[ln - 1].strip()[:110]}")
        if not ls:
            print("    (no C lines: " + ("not matched)" if stat[k] != "matched" else
                                         "the -g build failed)" if not lm else "none for these instructions)"))


def cmd_orders(a):
    """Random dependency-respecting orders of the statements on NAME's lines A-B, each compiled
    as a copy of the unit and ranked like try --file (tools/scripts/orders.py)."""
    import orders
    import ptrace
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    path, defs = (a.file, []) if a.file else (src, c_defines(src, name, e))
    lines = read(path).split("\n")
    try:
        lo, hi = (int(x) for x in a.lines.split("-"))
    except ValueError:
        die("--lines takes A-B (source line numbers)")
    if not 1 <= lo <= hi <= len(lines):
        die(f"--lines {a.lines} is outside {path} (1-{len(lines)})")
    # the function's parameters and locals tell its variables from globals
    ftext = extract_function(read(path), name, top=True)
    if not ftext:
        die(f"no definition of {name} in {path}")
    local = {d.name for d in ptrace.decls(ftext)}
    try:
        stmts = orders.statements("\n".join(lines[lo - 1:hi]))
    except ValueError as ex:
        die(str(ex))
    if len(stmts) < 2:
        die("the range holds fewer than two statements")
    indent = re.match(r"\s*", lines[lo - 1]).group(0)
    found = orders.orders(stmts, local, a.count, a.seed, a.free_memory)
    d = f"build/orders/s{a.seed}"                   # one directory per seed: refine from another's best
    if os.path.abspath(path).startswith(os.path.abspath(d) + os.sep):
        die(f"{path} is in {d}, which this run clears: copy it out, or use another --seed")
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    paths = []
    for k, order in enumerate(found):
        p = f"{d}/o{k:03d}.c"
        write(p, "\n".join(lines[:lo - 1] + [indent + stmts[j] for j in order] + lines[hi:]))
        paths.append(p)
    fixed = sum(len(v) for v in orders.edges(stmts, local, a.free_memory).values())
    memory = "memory by place" if a.free_memory else "any two memory accesses meet; --free-memory"
    print(f"ORDERS {name}: {len(stmts)} statements on lines {lo}-{hi} ({fixed} must-keep pairs, "
          f"{memory}), {len(found)} orders; o000 is the source's")
    ok, _, best = try_files(key, e, proj, name, paths, a.jobs, a.top, defs)
    if best:
        k = paths.index(best)
        print(f"\nbest order ({best}; lines {lo}-{hi}):")
        print("\n".join(indent + stmts[j] for j in found[k]))
    sys.exit(0 if ok else 1)


def cmd_trace(a):
    """Follow one insn, or every insn that mentions a variable's pseudo, through cc1's pass dumps,
    and name the pass that rewrote, moved or deleted each (tools/scripts/ptrace.py)."""
    import ptrace
    if sum(x is not None for x in (a.insn, a.var, a.reg)) != 1:
        die("give one of --insn UID, --var NAME or --reg N")
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    path, defs = (a.file, []) if a.file else (src, c_defines(src, name, e))
    cfg = unit_cfg(mk_config(), src)
    d = "build/trace"
    os.makedirs(d, exist_ok=True)
    i = f"{d}/{name}.i"
    for old in glob.glob(f"{i}.*"):                 # a pass that no longer runs leaves no dump
        os.remove(old)
    r = run(f"{X}cpp {' '.join([*make_flags('CPPFLAGS'), *defs])} {path} > {i}")
    if r.returncode:
        die(f"cpp failed: {r.stderr.strip()[-300:]}")
    r = run(f"cd {d} && {GCC_ROOT}/{cfg['CC_VER']}/cc1 -quiet {cfg['OPT_FLAGS']} "
            f"{cfg.get('EXTRA_CC1FLAGS', '')} -G{cfg.get('GVAL', '0')} {ptrace.DUMP_FLAGS} "
            f"-o {name}.s {name}.i")
    if r.returncode:
        die(f"cc1 failed: {r.stderr.strip()[-300:]}")
    dumps = [(p, read(f"{i}.{p}")) for p, _ in ptrace.PASSES if os.path.exists(f"{i}.{p}")]
    passes, user = ptrace.load(dumps, name)
    if not passes:
        die(f"no dump has {name} (is it compiled from {path}?)")
    if a.insn is not None:
        print(ptrace.report_insn(passes, name, a.insn))
        return
    greg = f"{i}.greg"
    disp = ptrace.dispositions(ptrace.section(read(greg), name)) if os.path.exists(greg) else {}
    reg, label = a.reg, None
    if a.var:
        ftext = extract_function(read(path), name, top=True)
        if not ftext:
            die(f"no definition of {name} in {path}")
        ds = ptrace.decls(ftext)
        regs = [x.name for x in ds if not x.memory]
        m = ptrace.map_vars(ds, user)
        if m is None or a.var not in m:
            die(f"can't map {a.var} to a pseudo by declaration order: {len(regs)} register "
                f"declarations ({', '.join(regs)}) against {len(user)} reg/v pseudos "
                f"({', '.join(map(str, user))}); give --reg N")
        reg = m[a.var]
        home = var_homes(src, name, mk_config(), defs, c_path=path).get(a.var)
        got = REG_NAMES[disp[reg]] if disp.get(reg, 99) < 32 else None
        check = ("" if home is None or got is None else "" if home == got
                 else f"; DISAGREES with greg's {got}: check with --reg")
        label = (f"{a.var} = r{reg} (register declaration {list(m).index(a.var) + 1} of {len(m)}; "
                 f"-g home {home or '?'}{check})")
    got = REG_NAMES[disp[reg]] if disp.get(reg, 99) < 32 else "no hard register"
    print(ptrace.report_reg(passes, name, reg, (label or f"r{reg}") + f", greg: {got}"))


def cmd_protos(a):
    """NAME's copies in each scanned prototype and demo disc (tools/scripts/proto_find.py)."""
    import proto_find
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    seg = e["seg"].split("/")
    kind = "main" if seg[0] in ("main", "ps1") else "engine" if seg[1] == "engine" else "slot"
    tw = target_words(e, proj)
    if a.diff:
        found = proto_find.copy_in(tw, kind, a.diff)
        if found is None:
            die(f"no scanned disc under protos/ has '{a.diff}' in its name")
        d, ws, addr = found
        name = cur_name(key, e, sym_names())
        if not ws:
            die(f"{d}: no {kind} function close to {name}")
        left, side = tw, "retail"
        if a.file:                      # a test copy's build in retail's place
            left, side = file_words(e, name, a.file), "yours"
        print(f"{name} against {d} at 0x{addr:08X} (- {side}, + copy; immediates ignored):")
        print("\n".join(proto_find.diff_lines(left, ws, a_name=side)))
        if a.words:
            print(f"the copy in full ({len(ws)} words):")
            print("\n".join(proto_find.word_lines(ws)))
        return
    if a.file or a.words:
        die("--file and --words go with --diff DISC")
    print(f"{cur_name(key, e, sym_names())} ({e['seg']}, {e['size']} B), by opcode similarity:")
    print(proto_find.report(tw, kind, top=a.top))


def cmd_reconcile(a):
    """Put declarations the working tree's headers re-declare back to HEAD's form (headers.py)."""
    import headers
    changed = [p for p in run(["git", "diff", "HEAD", "--name-only", "--", "include"]).stdout.split()
               if p.endswith(".h") and os.path.exists(p)]
    heads = {}
    for p in run(["git", "ls-tree", "-r", "--name-only", "HEAD", "--", "include"]).stdout.split():
        if p.endswith(".h"):
            heads[p] = run(["git", "show", f"HEAD:{p}"]).stdout
    sources = [read(f) for f in glob.glob("src/**/*.c", recursive=True)]
    total = 0
    for p in changed:
        base = heads.get(p, "")
        known = headers.declared(heads[q] for q in sorted(headers.visible(p, sources, heads)))
        text, dropped = headers.reconcile(base, read(p), known)
        if dropped:
            write(p, text)
            total += len(dropped)
            for d in dropped:
                print(f"{p}: kept the existing declaration over: {' '.join(d.split())[:100]}")
    print(f"reconciled {total} declaration(s) in {len(changed)} changed header(s)")


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
    # ... and other functions' C changes only where it calls this one
    stray = edited_in_worktree({name, e["name"], e["orig"]})
    if stray:
        die(f"the working tree also edits {', '.join(stray)} on lines that don't call {name}: "
            "one function per commit. Restore that C (git diff HEAD -- src) and commit it separately.")
    # its INCLUDE_ASM must be gone: with it still there the asm "matches" itself
    if still_asm(src, name, e["name"], e["orig"]):
        die(f"{src} still has the INCLUDE_ASM line for {name}: replace it with the C first "
            "(nothing recorded or committed)")
    # 2. lint: no m2c leftovers, and only C
    errs = lint_function(src, name)
    if errs:
        die(f"{name}: {', '.join(errs)} in the C - clean it up before recording the match")
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
        notes = [a.note] if a.note else []
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

FUNC_DECL_LINE = re.compile(r"^[A-Za-z_?][\w \t*?]*\b\w+\s*\([^;{}]*\)\s*;\s*(/\*.*\*/)?\s*$")


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
FPTR_NAME = re.compile(r"^[^(]*\(\s*\*\s*(\w+)\s*(?:\[[^\]]*\])?\s*\)\s*\(")


def decl_name(line):
    """Name declared by a one-line prototype or extern, or None."""
    l = line.strip()
    if not l or l.startswith(("#", "/*", "*", "typedef", "}")) or not l.endswith((";", "*/")):
        return None
    # a function pointer (or an array of them): `extern s32 (*D_8009D728)(s32);`
    m = FPTR_NAME.search(l) or DECL_NAME.search(l)
    return m.group(1) if m else None


def visible_names(src):
    """Names declared by the headers `src` includes (one level, plus their own includes)."""
    return header_names([re.search(r'"([^"]+)"', l).group(1) for l in src_includes(src) if '"' in l])


def header_names(todo, lines=None):
    """Names declared by headers (paths under include/) and the headers they include. With
    `lines`, each name's declaration line is added to it too (the first one found)."""
    seen, names, todo = set(), set(), list(todo)
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
                if lines is not None and file_decl(l):
                    lines.setdefault(n, l.strip())
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
        write(h, insert_decls(read(h), new))


def func_addr(name, _cache={}):
    """Load address of function `name` (func_XXXXXXXX or a symbol-file name), or None."""
    m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", name)
    if m:
        return int(m.group(1), 16)
    if not _cache:
        _cache.update({n: int(k.split(":")[-1], 16) for k, n in sym_names().items()})
    return _cache.get(name)


def insert_decls(t, new):
    """Add declarations to header text `t`. A function prototype goes in address order into the
    run of one-line prototypes that ends the header (so parallel matches touch different lines);
    anything else, or a prototype with no known address, goes at the end."""
    lines = t.splitlines(keepends=True)
    end = max(i for i, l in enumerate(lines) if l.startswith("#endif"))
    start = end
    while start > 0 and (not lines[start - 1].strip() or (decl_name(lines[start - 1]) and "(" in lines[start - 1])):
        start -= 1
    tail = []
    for d in new:
        a = func_addr(decl_name(d)) if "(" in d else None
        at = None
        if a is not None:
            for i in range(start, end):
                n = decl_name(lines[i])
                b = func_addr(n) if n and "(" in lines[i] else None
                if b is not None and b > a:
                    at = i
                    break
        if at is None:
            tail.append(d)
        else:
            lines[at:at] = [d + "\n", "\n"]
            end += 2
    if tail:
        lines[end:end] = ["\n".join(tail) + "\n\n"]
    return "".join(lines)


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
            others |= {decl_name(x) for x, top in top_level(read(p).splitlines()) if top} - {None}
    text = read(h)
    lines = text.splitlines(keepends=True)

    def drop(l, top):
        # struct members (`s32 x;`) look like declarations too: only top-level lines go, and not
        # a name the header itself uses (its inline functions may come before the other header)
        n = decl_name(l) if top else None
        return n in others and len(re.findall(rf"\b{re.escape(n)}\b", text)) == 1
    kept = [l for l, top in top_level(lines) if not drop(l, top)]
    if len(kept) != len(lines):
        write(h, "".join(kept))


def top_level(lines):
    """(line, True if it starts outside any braces) for each line."""
    depth = 0
    for l in lines:
        yield l, depth == 0
        code = re.sub(r"/\*.*?\*/|//.*", "", l)
        depth = max(0, depth + code.count("{") - code.count("}"))


FUNC_DEF = re.compile(r"^(?!static\b|typedef\b|extern\b)([A-Za-z_][\w \t]*?[\s*]+)(\w+)\s*\(([^;{}()]*)\)\s*\{", re.M)


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


def land(cands, funcs, proj, st, protos=True, tag="auto_failed"):
    """Insert candidates (key, entry, name, src, decls, ftext[, headers to include]) into the real
    sources and keep the ones that still match there, then check the full build. Returns the kept
    candidates, or None when the full build failed (everything reverted)."""
    srcs = sorted(glob.glob("src/**/*.c", recursive=True))
    backup = {p: read(p) for p in srcs + all_headers()}

    def apply(sel):
        for p, t in backup.items():
            write(p, t)
        for h in all_headers():
            if h not in backup:
                os.remove(h)
        for k, e, name, src, decls, ftext, *incs in sel:
            for h in (incs[0] if incs else ()):
                ensure_include(src, h)
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
                    if w and not compare(target_words(c[1], proj), w, rel, key_vram(c[0]))[0]:
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


def split_inputs():
    """[(path, mtime)] of what a re-split depends on: the splat configs and the symbol files."""
    inputs = sorted(glob.glob(f"{CFG}/**/*.yaml", recursive=True) + glob.glob(f"{CFG}/sym.*.txt"))
    return [(p, os.path.getmtime(p)) for p in inputs]


def rodata_syms(seg, inputs=None, cache=None):
    """[(rom, symbol, owners)] for the rodata splat put under asm/nonmatchings/<seg>/: owners is
    the function it was migrated into, else every function that refers to it.
    Cached in build/: parsing ~2800 .s files over Docker's Windows bind mount takes ~11 s, and the
    dashboard (so every `done`) needs it. The .s files only change when splat runs, which depends
    on the configs and symbol files, so those mtimes and the file list are the cache key.
    A caller looking up many segments passes split_inputs() and the loaded cache once: each mtime
    is ~1 ms over the bind mount, and 29 segments x 33 files was ~1 s of every `done`."""
    files = sorted(glob.glob(f"asm/nonmatchings/{seg}/*.s"))
    key = hashlib.sha1(json.dumps([files, split_inputs() if inputs is None else inputs]).encode()).hexdigest()
    if cache is None:
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
PSYQ_DATA_KEEP = f"{PSYQ_CFG}/data_keep.txt"   # objects whose .data stays from the exe (link-psyq --data)


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
        # already linked (an earlier run): keep it, it still needs its sections placed
        if f"- [0x{rom:X}, lib, {lib}, {mem}, .text]" in text:
            members[o] = (lib, mem, secs)
            continue
        # a splat-named library file, or a PsyQ gap identified later (tools/scripts/psyq_gaps.py)
        m = re.search(rf"- \[0x{rom:X}, (c|asm), (lib/[\w/]+|main/psyq/unknown/\w+)\]", text)
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
    # --data: each object's .data from the object, cut out of the exe's data blob (the rest of the
    # blob stays game data, in pieces named by their offset)
    if a.data:
        blobs = [(int(m.group(1), 16), m.group(0)) for m in re.finditer(r"- \[0x([0-9A-F]+), data, \w+\][^\n]*", text)]
        if not blobs:
            die("no `data` subsegment to cut the objects' .data out of")
        ends = [int(m.group(1), 16) for m in re.finditer(r"^\s*- \[0x([0-9A-F]+)\]\s*$", text, re.M)]
        ind = " " * 6
        for start, line in blobs:
            nxt = re.search(r"- \[0x([0-9A-F]+)[,\]]", text[text.index(line) + len(line):])
            end = int(nxt.group(1), 16) if nxt else max(ends)
            # objects whose .data differs from the game's copy keep theirs from the exe (with the reason)
            keep = {l.split()[0] for l in (read(PSYQ_DATA_KEEP).splitlines() if os.path.exists(PSYQ_DATA_KEEP) else [])
                    if l.strip() and not l.startswith("#")}
            objs = sorted((s[".data"][0] - vram_off, s[".data"][1], lib, mem) for o, (lib, mem, s) in members.items()
                          if ".data" in s and s[".data"][0] is not None and s[".data"][1] and o not in keep
                          and start <= s[".data"][0] - vram_off < end)
            if not objs:
                continue
            new, cur = [], start
            for rom, n, lib, mem in objs:
                if rom < cur:
                    die(f"{lib}/{mem} .data at 0x{rom:X} overlaps the previous piece (ends 0x{cur:X})")
                if rom > cur:
                    new.append(f"- [0x{cur:X}, data, {cur:X}]")
                new.append(f"- [0x{rom:X}, lib, {lib}, {mem}, .data]")
                cur = rom + n
            if cur < end:
                new.append(f"- [0x{cur:X}, data, {cur:X}]")
            text = text.replace(line, ("\n" + ind).join(new), 1)
            print(f"  .data: {len(objs)} objects' data from the objects, the blob in {len(new) - len(objs)} pieces")
    in_yaml = set(re.findall(r"- \[0x[0-9A-F]+, lib, (\w+), (\w+), \.data\]", text))
    if "lib_path:" not in text:
        text = text.replace("  asset_path: asm/bin\n", f"  asset_path: asm/bin\n  lib_path: {PSYQ_LIBS}\n", 1)
    # NOLOAD placement of the other sections
    ld = ["/* Sony PsyQ objects' data sections at their original addresses (decomp.py link-psyq).",
          " * NOLOAD: the bytes come from the exe's data; this only makes references resolve.",
          " * The Makefile splices these lines into splat's linker script, ahead of its /DISCARD/. */"]
    spare = 0x81000000                     # unreferenced sections: anywhere out of the way
    for o, (lib, mem, secs) in sorted(members.items(), key=lambda kv: kv[1][2].get(".data", (0,))[0] or 0):
        for s in (".data", ".bss", ".sbss", ".sdata"):
            if s not in secs or not secs[s][1] or (s == ".data" and (lib, mem) in in_yaml):
                continue                   # (linked from the yaml, loaded)
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


def asm_files_of(ovl, name):
    """asm/nonmatchings/.../NAME.s files of the unit a symbol belongs to. A slot overlay's are
    under its own directory only: the overlays share one address range, so another overlay
    can have a function of the same placeholder name (func_800B4848 in bash and titles)."""
    found = glob.glob(f"asm/nonmatchings/**/{name}.s", recursive=True)
    if ovl in ("", "engine"):
        slots = re.compile(r"asm/nonmatchings/overlays/(?!engine/)")
        return [f for f in found if not slots.match(f.replace("\\", "/"))]
    return [f for f in found if f.replace("\\", "/").startswith(f"asm/nonmatchings/overlays/{ovl}/")]


def rename_asm_file(path, old, new):
    """Move asm/nonmatchings/.../OLD.s to NEW.s with its labels renamed; returns the old text."""
    text = read(path)
    write(os.path.join(os.path.dirname(path), f"{new}.s"), re.sub(rf"\b{re.escape(old)}\b", new, text))
    os.remove(path)
    return text


def apply_renames(pairs, proj, write_syms=True):
    """pairs: [(ovl, addr, old, new, typ)]. Writes the names, rewrites references in src/ and
    include/, re-splits, re-indexes and checks the build; undoes everything if it differs."""
    snap = {p: read(p) for p in glob.glob(f"{CFG}/**/*", recursive=True) + all_headers()
            + glob.glob("src/**/*.c", recursive=True) + [P_FUNCS] if os.path.isfile(p)}
    created = set()
    moved_asm = {}
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
        for f in asm_files_of(ovl, old):
            # splat writes only the functions the C file lacks, so a matched function's .s (its
            # target asm, and the owner of its rodata in the data count) must move with it
            moved_asm[f] = rename_asm_file(f, old, new)
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
        for f, text in moved_asm.items():
            write(f, text)
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


def copy_decls(ftext, ssrc, src, incs, decls):
    """Declarations for the names the copied C uses that its original's file sees and the copy's
    file (with the headers `incs` it gets) doesn't: without one a call compiles silently as an
    implicit declaration (there are no -W flags) and the copy mismatches."""
    where = {}
    for l in read(ssrc).splitlines():                # the original file's own externs and prototypes
        n = file_decl(l)
        if n:
            where.setdefault(n, l.strip())
    header_names([re.search(r'"([^"]+)"', l).group(1) for l in src_includes(ssrc) if '"' in l], where)
    owned = [os.path.relpath(h, HEADERS) for h in all_headers() if header_owner(h) == src]
    rel = [os.path.relpath(h, HEADERS).replace("\\", "/") for h in incs] + owned
    have = visible_names(src) | header_names(rel)
    have |= {decl_name(l) for l in read(src).splitlines()} | {decl_name(d) for d in decls}
    body = re.sub(r"/\*.*?\*/", "", ftext, flags=re.S)
    return [where[n] for n in sorted(set(re.findall(r"\b[A-Za-z_]\w*\b", body))) if n in where and n not in have]


def file_decl(line):
    """decl_name for a file-scope extern or prototype only (not a local, a struct member or a
    definition of data, which must not be copied into a header)."""
    if line[:1].isspace() or not (line.startswith("extern ") or "(" in line):
        return None
    return decl_name(line)


TEMPLATE_MIN = 0.9     # seq ratio from which a matched look-alike's C is a template (a known template
                       # pair, func_8007CC80 from func_8007C338, scores 0.97)


def print_template(key, e, name, funcs, stat, syms, proj):
    """A matched look-alike's C (seq ratio >= TEMPLATE_MIN) for `next`: renamed where the two
    functions' relocations line up, else as written, with where its types are declared."""
    r, bk = best_lookalike(key, funcs, opcode_seqs(funcs), stat, proj, TEMPLATE_MIN - 1e-9)
    if not bk:
        return
    se, sname = funcs[bk], cur_name(bk, funcs[bk], syms)
    ssrc = se.get("src") or locate_def(sname)
    base = def_with_decls(read(ssrc), sname) if ssrc else None
    if not base:
        return
    mp = sym_map(se, e, sname, name) if os.path.exists(e.get("asm", "")) else None
    owned = [h for h in all_headers() if header_owner(h) == ssrc]
    print(f"--- template: {sname}'s C ({se['seg']}, matched, seq {r:.2f}), "
          + (f"renamed for {name}" if mp is not None else
             "as written: use this function's own names from its asm") + " ---")
    if owned:
        print(f"(its types and declarations: {', '.join(owned)} and the headers {ssrc} includes)")
    print((rename_syms(base, mp) if mp else base).rstrip())


def twin_source(key, funcs, stat, syms):
    """(original key, name, its C renamed for `key`) when an exact twin of `key` is matched."""
    e = funcs[key]
    if not e.get("fp") or e["size"] <= 8 or not e.get("src") or not os.path.exists(e.get("asm", "")):
        return None
    name = cur_name(key, e, syms)
    for sk in groups_by(funcs).get(e["fp"], []):
        if sk == key or stat.get(sk) != "matched":
            continue
        se, sname = funcs[sk], cur_name(sk, funcs[sk], syms)
        ssrc = se.get("src") or locate_def(sname)
        base = def_with_decls(read(ssrc), sname) if ssrc else None
        mp = sym_map(se, e, sname, name) if base else None
        if mp is not None:
            return sk, sname, rename_syms(base, mp)
    return None


def propagate_candidates(funcs, stat, syms, only=None, st=None, proj=None):
    """(key, entry, name, src, decls, ftext, original key, headers) for every todo function whose
    exact twin is matched. Copies that already failed from the same original (recorded in st) are
    skipped. Writes nothing: land() adds the headers and declarations."""
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
            # file owns: what it needs from those comes in through header_decls and copy_decls)
            incs = []
            for inc in src_includes(ssrc):
                hm = re.search(r'"([^"]+)"', inc)
                if hm and not (os.path.exists(f"{HEADERS}/{hm.group(1)}")
                               and header_owner(f"{HEADERS}/{hm.group(1)}") not in (None, src)):
                    incs.append(f"{HEADERS}/{hm.group(1)}")
            ftext, decls = rename_syms(base, mp), header_decls(mp)
            decls += copy_decls(ftext, ssrc, src, incs, decls)
            out.append((k, e, name, src, decls, ftext, sk, incs))
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
        good = land([c[:6] + (c[7],) for c in cands], funcs, proj, st, tag="copy_failed")
        st = jload(P_STATE, {})
        for k in origin:                              # remember which original didn't fit here
            if k not in {g[0] for g in good or []}:
                rec = st.setdefault(k, {})
                # land recorded where it failed ("in-file" or "full-build") under the same field
                if rec.get("copy_failed") in ("in-file", "full-build"):
                    rec["copy_failed_stage"] = rec["copy_failed"]
                rec["copy_failed"] = origin[k]
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


def opcode_seqs(funcs):
    """key -> the function's opcode sequence (regions.mnemonic of each word), cached in
    build/similar.json. The asm files only change when the project is re-split, which changes
    functions.json's entries, so the cache is keyed by a hash of each function's key, asm, size
    and fingerprint (not the file's mtime: a clone's copy has its own, and statting each asm file
    through the Docker mount took ~20 s). tools/swarm copies the cache into its clones."""
    import regions
    cache_path = "build/similar.json"
    ident = sorted((k, e.get("asm"), e["size"], e.get("fp")) for k, e in funcs.items())
    stamp = [4, hashlib.sha1(json.dumps(ident).encode()).hexdigest()]
    cache = jload(cache_path, {})
    if cache.get("stamp") == stamp:
        return cache["seqs"]
    seqs = {}
    for k, e in funcs.items():
        p = e.get("asm")
        if not p or not os.path.exists(p):
            continue
        # splat prints each word's bytes in file order: swap to the instruction
        words = [int.from_bytes(w.to_bytes(4, "big"), "little") for w in parse_asm(p)[2]]
        seqs[k] = ["%x.%s" % (w >> 26, "-".join(map(str, regions.mnemonic(w)[1:]))) for w in words[:trim_nops(words)]]
    os.makedirs("build", exist_ok=True)
    jsave(cache_path, {"stamp": stamp, "seqs": seqs})
    return seqs


def best_lookalike(key, funcs, seqs, stat, proj, floor=0.0, among=None):
    """(ratio, key) of the matched function (of `among`, default all) whose opcode sequence lines
    up best with key's (game code only, sizes within 2/3..3/2), or (0, None)."""
    me = seqs.get(key)
    if not me:
        return 0.0, None
    best, bk = floor, None
    sm = difflib.SequenceMatcher(None, autojunk=False)
    sm.set_seq2(me)
    for k in (seqs if among is None else among):
        seq, f = seqs.get(k), funcs[k]
        if not seq or k == key or stat.get(k) != "matched" or not 2 * len(me) <= 3 * len(seq) <= 9 * len(me) / 2:
            continue
        if is_lib(f, proj) or f.get("kind") in ("asm", "sony"):
            continue
        sm.set_seq1(seq)
        if sm.real_quick_ratio() <= best or sm.quick_ratio() <= best:
            continue
        r = sm.ratio()
        if r > best:
            best, bk = r, k
    return (best, bk) if bk else (0.0, None)


def lookalikes(funcs, stat, proj, keys):
    """key -> [ratio, look-alike key] for `keys`, cached in build/lookalike.json: a new match only
    needs comparing with the functions to match, so the queue can carry it (a full pass is ~45 s
    for the overlays)."""
    seqs = opcode_seqs(funcs)
    path = "build/lookalike.json"
    stamp = jload("build/similar.json", {}).get("stamp")
    matched = {k for k in seqs if stat.get(k) == "matched"}
    cache = jload(path, {}) if os.path.exists(path) else {}
    if cache.get("stamp") != stamp or not set(cache.get("matched", [])) <= matched:
        cache = {"stamp": stamp, "matched": [], "best": {}}
    new = matched - set(cache["matched"])
    best = cache["best"]
    for k in keys:
        old = best.get(k)
        r, bk = best_lookalike(k, funcs, seqs, stat, proj, old[0] if old else 0.0,
                               among=None if old is None else new)
        if bk or old is None:
            best[k] = [r, bk]
    cache["matched"] = sorted(matched)
    jsave(path, cache)
    return best


def cmd_similar_survey(a, proj, funcs):
    """For every function still to match (in --seg), its best matched look-alike by seq ratio:
    how many could start from another function's C."""
    stat, _ = statuses(funcs)
    syms = sym_names()
    seqs = opcode_seqs(funcs)
    todo = [k for k, e in funcs.items() if stat.get(k) in ("todo", "in_progress", "blocked") and k in seqs
            and (e["seg"].startswith(a.seg) if a.seg else not is_lib(e, proj))]
    best = lookalikes(funcs, stat, proj, todo)
    rows = [(best[k][0], k, best[k][1]) for k in todo]
    rows.sort(key=lambda x: -x[0])
    for r, k, bk in rows[:a.top]:
        e = funcs[k]
        print(f"{r:4.2f} {e['size']:5d}  {e['seg']}/{cur_name(k, e, syms)}"
              + (f"  <- {funcs[bk]['seg']}/{cur_name(bk, funcs[bk], syms)}" if bk else ""))
    total = sum(funcs[k]["size"] for k in todo) or 1
    print(f"{len(todo)} functions to match{f' in {a.seg}' if a.seg else ''}, {total} bytes. "
          "With a matched look-alike at seq ratio >=:")
    for th in (0.95, 0.9, 0.8, 0.7, 0.6):
        hit = [k for r, k, _ in rows if r >= th]
        print(f"  {th:.2f}: {len(hit):4d} functions, {sum(funcs[k]['size'] for k in hit):6d} bytes "
              f"({100 * sum(funcs[k]['size'] for k in hit) / total:.0f}%)")


def ngrams(seq, n=4):
    from collections import Counter
    return Counter(tuple(seq[i:i + n]) for i in range(len(seq) - n + 1))


def cmd_similar(a):
    """Matched functions whose code looks most like NAME's: a start for its C beyond `dups`'
    identical copies (siblings, a minigame's copy of a helper, the same idiom elsewhere).
    Scores compare opcode 4-grams: `sim` is the share both have (Dice), `in` the share of the
    other function found inside this one (a helper or macro it inlines)."""
    proj, funcs = need_proj(), need_funcs()
    if a.survey:
        return cmd_similar_survey(a, proj, funcs)
    if not a.func:
        die("similar needs NAME (or --survey)")
    key, e = find_func(funcs, a.func)
    syms = sym_names()
    stat, _ = statuses(funcs)
    seqs = opcode_seqs(funcs)
    if not seqs.get(key):
        die(f"no asm for {a.func}")
    me = ngrams(seqs[key])
    nme = sum(me.values()) or 1
    calls = set(e.get("calls", []))
    rows, weak = [], []
    for k, seq in seqs.items():
        f = funcs[k]
        if k == key or is_lib(f, proj) or f.get("kind") in ("asm", "sony") or len(seq) < 8:
            continue
        if not a.all and stat.get(k) != "matched":
            continue
        g = ngrams(seq)
        ng = sum(g.values()) or 1
        common = sum((me & g).values())
        sim, inside = 2 * common / (nme + ng), common / ng
        if sim >= a.min or (inside >= a.contain and ng >= 24):
            rows.append((sim, inside, k))
        elif common:
            weak.append((sim, inside, k))
    rows.sort(key=lambda r: (-r[0], -r[1]))
    weak_only = not rows
    if weak_only:                    # nothing close: the nearest few, so there is still a lead
        rows = sorted(weak, key=lambda r: (-r[0], -r[1]))[:3]
    name = cur_name(key, e, syms)
    print(f"{name} ({e['seg']}, {e['size']} B, {len(seqs[key])} insns): most similar "
          f"{'functions' if a.all else 'matched functions'} by opcode 4-grams")
    if weak_only:
        print(f"(nothing at or above --min {a.min} or --contain {a.contain}: the nearest, all weak)")
    print(f"{'sim':>4} {'in':>4} {'size':>5} {'calls':>5}  {'status':<8} function / C")
    for r, inside, k in rows[:a.top]:
        f = funcs[k]
        nm = cur_name(k, f, syms)
        shared = len(calls & set(f.get("calls", [])))
        where = ""
        if stat.get(k) == "matched" and f.get("src") and os.path.exists(f["src"]):
            m = func_def_regex(nm).search(read(f["src"]))
            if m:
                where = f"  {f['src']}:{read(f['src']).count(chr(10), 0, m.start()) + 1}"
        print(f"{r:4.2f} {inside:4.2f} {f['size']:5d} {shared:5d}  {stat.get(k, '?'):<8} {f['seg']}/{nm}{where}")


def partitions(n, cap):
    """Restricted growth strings: every way to put n items in groups (item 0 in group 0)."""
    out = []

    def rec(prefix, top):
        if len(out) >= cap:
            return
        if len(prefix) == n:
            out.append(tuple(prefix))
            return
        for g in range(top + 2):
            rec(prefix + [g], max(top, g))
    rec([0], 0)
    return out


def var_defs(ftext, var):
    """The declaration of `var` in a function's text and its occurrences after it, each marked
    as a plain assignment (a def: `var = ...` whose right side doesn't read var) or a use."""
    decl = re.search(r"^([ \t]*)((?:(?:const|volatile|unsigned|signed|struct\s+\w+|\w+)\s+)+\**)"
                     + re.escape(var) + r"(\s*=[^;]*)?;[ \t]*$", ftext, re.M)
    if not decl:
        return None, []
    occ = []
    for m in re.finditer(r"\b" + re.escape(var) + r"\b", ftext[decl.end():]):
        at = decl.end() + m.start()
        rest = ftext[at + len(var):]
        a = re.match(r"\s*=(?!=)([^;]*);", rest)
        occ.append((at, bool(a) and not re.search(r"\b" + re.escape(var) + r"\b", a.group(1))))
    return decl, occ


def cmd_splitvars(a):
    """Try splitting one local into several: gcc 2.8.1 gives a pseudo one register for its whole
    life, so a variable the original wrote as two (or three) shows up as one that sits in the
    wrong register in some blocks. Every grouping of the variable's assignments is compiled with
    the whole file and scored with `regions`."""
    import concurrent.futures
    import regions
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    text = read(src)
    m = func_def_regex(name).search(text) if src else None
    ftext = extract_function(text, name) if m else None
    if not ftext:
        die(f"no C definition of {name} found")
    fstart = m.start()
    decl, occ = var_defs(ftext, a.var)
    if not decl:
        die(f"no declaration `<type> {a.var};` (one per line) in {name}")
    defs = [at for at, is_def in occ if is_def]
    if len(defs) < 2:
        die(f"{a.var} is assigned {len(defs)} time(s): nothing to split")
    groups = [tuple(int(x) for x in a.groups.split(","))] if a.groups else partitions(len(defs), a.max)
    if a.groups and len(groups[0]) != len(defs):
        die(f"--groups needs {len(defs)} numbers (one per assignment)")
    line_of = lambda at: text.count("\n", 0, fstart + at) + 1

    def variant(gs):
        """The function with the variable split by group (group 0 keeps the name)."""
        names = [a.var if g == 0 else f"{a.var}_{g}" for g in gs]
        out, last, cur = [], 0, None
        for at, is_def in occ:
            if is_def:
                cur = names[defs.index(at)]
            out.append(ftext[last:at] + (cur or a.var))
            last = at + len(a.var)
        body = "".join(out) + ftext[last:]
        extra = "".join(f"{decl.group(1)}{decl.group(2)}{a.var}_{g};\n" for g in sorted(set(gs)) if g)
        cut = decl.end() + 1
        return body[:cut] + extra + body[cut:]

    cfg = unit_cfg(mk_config(), src)
    gval = int(cfg.get("GVAL", "0"))
    tw = target_words(e, proj)
    nt = trim_nops(tw)
    vram = key_vram(key)
    ttext = parse_asm(e["asm"])[3][:nt]
    labels = asm_labels(e["asm"], vram)
    os.makedirs("build/splitvars", exist_ok=True)

    def score(i, gs):
        c, o = f"build/splitvars/v{i}.c", f"build/splitvars/v{i}.o"
        write(c, text[:fstart] + variant(gs) + text[fstart + len(ftext):])
        cmd = pipeline(c, o, cfg["CC_VER"], f"{cfg['OPT_FLAGS']} {cfg.get('EXTRA_CC1FLAGS', '')}".strip(), gval,
                       re.sub(r"\s*-G\d+", "", cfg.get("MASPSX_ARGS", DEFAULT_MASPSX)), post_filters(cfg))
        if run("set -o pipefail; " + cmd).returncode:
            return gs, None
        cw, _, rel = obj_func(o, name)
        nc = trim_nops(cw)
        if not cw:
            return gs, None
        r = regions.score(tw[:nt], cw[:nc], ttext, rel, vram, labels)
        r["match"] = r["ok"] == r["total"] and nt == nc
        return gs, r

    print(f"{name}: {a.var} has {len(defs)} assignments; trying {len(groups)} grouping(s)")
    for k, at in enumerate(defs):
        print(f"  #{k} line {line_of(at)}: {ftext[at:ftext.index(';', at) + 1].strip()[:70]}")
    with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs or max(1, (os.cpu_count() or 2) - 1)) as ex:
        res = list(ex.map(lambda p: score(*p), enumerate(groups)))
    good = [(gs, r) for gs, r in res if r]
    keyf = lambda x: (x[1]["match"], x[1]["paired"] / x[1]["len"], x[1]["ok"], x[1]["exact"] / x[1]["len"])
    good.sort(key=keyf, reverse=True)
    print(f"{'groups':<{2 * len(defs) + 2}} blocks   paired   exact")
    for gs, r in good[:a.top]:
        tag = "  MATCH" if r["match"] else ("  (as written)" if not any(gs) else "")
        print(f"{','.join(map(str, gs)):<{2 * len(defs) + 2}} {r['ok']:>3}/{r['total']:<3} "
              f"{r['paired']:>4}/{r['len']:<4} {r['exact']:>4}/{r['len']:<4}{tag}")
    if len(good) < len(res):
        print(f"({len(res) - len(good)} grouping(s) didn't build)")
    if good and a.apply:
        gs, r = good[0]
        base = next((r0 for g0, r0 in good if not any(g0)), None)
        if any(gs) and (base is None or keyf((gs, r)) > keyf(((), base))):
            write(src, text[:fstart] + variant(gs) + text[fstart + len(ftext):])
            print(f"applied {','.join(map(str, gs))} to {src}: rename {a.var}_N to something meaningful")
        else:
            print("the variable as written scores best: nothing applied")


AUTO_SYM = re.compile(r"^(?:D|jtbl|jlabel|L|func|B)_([0-9A-Fa-f]{8})$")


def sym_addrs():
    """name -> address from every configs/USA/sym.*.txt."""
    out = {}
    for p in glob.glob(f"{CFG}/sym.*.txt"):
        for l in read(p).splitlines():
            m = SYM_LINE.match(l)
            if m:
                out[m.group(1)] = int(m.group(2), 16)
    return out


def func_refs(funcs):
    """key -> [[symbol, offset], ...] each function's asm refers to, and whether its global
    stores go through $at (gcc 2.7.2 + ASPSX), cached in build/units.json against functions.json."""
    path = "build/units.json"
    stamp = [1, os.path.getmtime(P_FUNCS)]
    c = jload(path, {})
    if c.get("stamp") == stamp:
        return c["refs"]
    refs = {}
    for k, e in funcs.items():
        p = e.get("asm")
        if not p or not os.path.exists(p):
            continue
        texts = parse_asm(p)[3]
        refs[k] = {"syms": [s for t in texts for s in insn_syms(t)],
                   "at": sum(1 for t in texts if "%lo(" in t and "($at)" in t),
                   "lo": sum(1 for t in texts if "%lo(" in t)}
    os.makedirs("build", exist_ok=True)
    jsave(path, {"stamp": stamp, "refs": refs})
    return refs


def units_parity(seg, funcs, syms):
    """Proven file boundaries from switch jump tables. cc1 puts `.align 3` before every jump table,
    relative to the object's own .rodata, and psylink places objects only 4-aligned, so inside one
    file every table has the same address mod 8 (odd-length tables get a zero pad word after them).
    A table whose parity differs from the one before it is in a later file. Works for any segment."""
    fl = sorted(((k, e) for k, e in funcs.items() if e["seg"] == seg), key=lambda x: x[1]["rom"])
    idx = {k: i for i, (k, _) in enumerate(fl)}
    names = [cur_name(k, e, syms) for k, e in fl]
    refs = func_refs(funcs)
    tabs = {}
    for k, _ in fl:
        for n, o in refs.get(k, {}).get("syms", []):
            m = re.match(r"jtbl_([0-9A-Fa-f]{8})$", n)
            if m:
                tabs.setdefault(int(m.group(1), 16) + o, set()).add(idx[k])
    rows = sorted(tabs.items())
    print(f"{seg}: {len(rows)} jump tables; a parity change between consecutive tables proves a file boundary:")
    found = 0
    for (a0, u0), (a1, u1) in zip(rows, rows[1:]):
        if a0 % 8 != a1 % 8:
            lo, hi = max(u0), min(u1)
            found += 1
            print(f"  {a0:08X} ({a0 % 8}) -> {a1:08X} ({a1 % 8}): a file starts after {lo} {names[lo]} and by {hi} {names[hi]}"
                  + ("" if lo < hi else "  (code order disagrees: check)"))
    if not found:
        print("  none")
    print()


def zero_tail_start(vram, data):
    """Address where a data block's all-zero tail starts (word-aligned): in an overlay that is
    where the uninitialised globals begin, every file's .bss and the commons after all the .data."""
    return vram + ((len(data.rstrip(b"\0")) + 3) & ~3)


def cmd_units(a):
    """Evidence for splitting a C segment (main/main) into its original translation units. Each
    object's .rodata is linked in the same order as its .text and follows its functions' order,
    file-local statics are used by one run of functions, separate files keep separate copies of a
    string literal, and the compiler's style can change at a file boundary (gcc 2.7.2 + ASPSX
    stores globals through $at). Prints each kind of evidence and ranks the gaps between
    neighbouring functions as boundary candidates."""
    import yaml
    proj, funcs = need_proj(), need_funcs()
    syms = sym_names()
    units_parity(a.seg, funcs, syms)
    # main/... is a unit of the exe; overlays/<ovl>/... one of an overlay, with its own yaml, binary
    # and layout (a slot overlay is .rodata, .text, .data, so its data starts after the code)
    ovl = a.seg.split("/")[1] if a.seg.startswith("overlays/") else None
    target = ovl or "main"
    y = yaml.safe_load(read(yaml_of(target)))
    seg = next((s for s in y["segments"] if isinstance(s, dict) and s.get("name") == target
                and any(isinstance(x, list) and len(x) > 2 and x[2] == a.seg for x in s.get("subsegments", []))), None)
    if seg is None:
        print(f"(no segment of {yaml_of(target)} holds {a.seg}: only the jump-table check)")
        return
    subs = [s for s in seg["subsegments"] if isinstance(s, list)]
    v = lambda rom: rom - seg["start"] + seg["vram"]
    rng = {}
    for i, s in enumerate(subs):
        nxt = subs[i + 1][0] if i + 1 < len(subs) else None
        if len(s) >= 3 and s[2] == a.seg and nxt:
            rng[".rodata" if s[1] == ".rodata" else "text"] = (v(s[0]), v(nxt))
    if "text" not in rng:
        print(f"(the other evidence needs a c subsegment of {yaml_of(target)}; {a.seg} has only the jump-table check)")
        return
    ro = rng.get(".rodata", (0, 0))
    data_kinds = ("data", "sdata", "bss", "sbss") if ovl else ("rodata",)
    code_end = v(next(s[0] for s in subs if len(s) > 1 and str(s[1]).lstrip(".") in data_kinds))  # data starts here
    fl = sorted(((k, e) for k, e in funcs.items() if e["seg"] == a.seg), key=lambda x: x[1]["rom"])
    idx = {k: i for i, (k, _) in enumerate(fl)}
    refs = func_refs(funcs)
    addrs = sym_addrs()

    def addr(name):
        m = AUTO_SYM.match(name)
        return int(m.group(1), 16) if m else addrs.get(name)
    names = [cur_name(k, e, syms) for k, e in fl]
    nf = len(fl)
    gaps = [{"score": 0.0, "why": []} for _ in range(nf - 1)]     # gap i: between function i and i+1

    # 1. compiler style
    style = []
    for k, _ in fl:
        r = refs.get(k, {"at": 0, "lo": 0})
        style.append("2.7.2" if r["at"] else ("2.8.1" if r["lo"] else None))
    last = None
    for i, s in enumerate(style):
        if s and last and s != last[1]:
            for g in range(last[0], i):
                gaps[g]["score"] += 3 / (i - last[0])
                gaps[g]["why"].append(f"style {last[1]} -> {s}")
        if s:
            last = (i, s)

    # 2. rodata order: a function whose rodata comes before an earlier function's
    ro_first, ro_max = {}, -1
    users = {}                                               # address -> functions using it
    for i, (k, _) in enumerate(fl):
        mine = sorted({addr(n) + o for n, o in refs.get(k, {}).get("syms", []) if addr(n) is not None
                       and ro[0] <= addr(n) + o < ro[1]})
        for x in mine:
            users.setdefault(x, set()).add(i)
        if mine:
            if mine[0] < ro_max:
                gaps[i - 1]["score"] += 2
                gaps[i - 1]["why"].append(f"rodata goes back ({mine[0]:08X} < {ro_max:08X})")
            ro_max = max(ro_max, mine[-1])

    # 3. file-local data: a symbol used only by a run of this segment's functions
    used_by = {}
    for k, r in refs.items():
        for n, o in r["syms"]:
            used_by.setdefault(n, set()).add(k)
    windows = []
    for n, ks in used_by.items():
        x = addr(n)
        if x is None or x < code_end or ro[0] <= x < ro[1] or not all(k in idx for k in ks):
            continue
        ii = sorted(idx[k] for k in ks)
        if len(ii) >= 2 and ii[-1] - ii[0] <= a.window:
            windows.append((ii[0], ii[-1], n))
    for lo, hi, n in windows:
        for g in range(lo, hi):
            gaps[g]["score"] -= 0.5

    # 4. duplicate string literals in this segment's rodata
    with open(f"disc/{ovl}.bin" if ovl else os.path.join("disc", proj["exe"]), "rb") as f:
        exe = f.read()
    rom = lambda va: va - seg["vram"] + seg["start"]
    strs = {}
    for x, us in users.items():
        b = exe[rom(x):rom(x) + 256]
        end = b.find(b"\0")
        if end >= 3 and all(32 <= ch < 127 or ch in (9, 10) for ch in b[:end]):
            strs.setdefault(b[:end], []).append((x, us))
    dups = [(s, cs) for s, cs in strs.items() if len(cs) > 1]
    for s, cs in dups:
        cs = sorted(cs, key=lambda c: min(c[1]))
        for (x1, u1), (x2, u2) in zip(cs, cs[1:]):
            if max(u1) < min(u2):
                span = range(max(u1), min(u2))
                for g in span:
                    gaps[g]["score"] += 2 / len(span)
                    gaps[g]["why"].append(f"string {s[:24]!r} copied (users {max(u1)}|{min(u2)})")

    # 5. rodata padded to 8 after a string: a new object's .rodata starting 8-aligned
    xs = sorted(users)
    for x, nx in zip(xs, xs[1:]):
        b = exe[rom(x):rom(nx)]
        end = b.find(b"\0")
        if end < 3 or not all(32 <= ch < 127 or ch in (9, 10) for ch in b[:end]):
            continue
        a4 = (x + end + 1 + 3) & ~3
        if nx == a4 + 4 and nx % 8 == 0 and not any(exe[rom(a4):rom(nx)]):
            i1, i2 = max(users[x]), min(users[nx])
            if i1 < i2:
                for g in range(i1, i2):
                    gaps[g]["score"] += 2 / (i2 - i1)
                    gaps[g]["why"].append(f"rodata padded to 8 at {nx:08X}")

    # clusters: overlapping file-local windows chain functions that must share a file
    cover = [0] * (nf - 1)
    for lo, hi, n in windows:
        for g in range(lo, hi):
            cover[g] += 1
    clusters, start = [], None
    for g in range(nf - 1):
        if cover[g] and start is None:
            start = g
        if not cover[g] and start is not None:
            clusters.append((start, g))
            start = None
    if start is not None:
        clusters.append((start, nf - 1))

    print(f"{a.seg}: {nf} functions, text {rng['text'][0]:08X}-{rng['text'][1]:08X}, rodata {ro[0]:08X}-{ro[1]:08X}")
    print(f"style: " + ", ".join(f"{s} x{style.count(s)}" for s in ("2.8.1", "2.7.2", None)))
    print(f"file-local data candidates (used by one run of <= {a.window} functions): {len(windows)}")
    print(f"duplicate string literals: {len(dups)}")
    print(f"clusters that probably share a file (chained file-local data): {len(clusters)}, covering "
          f"{sum(hi - lo + 1 for lo, hi in clusters)} of {nf} functions; boundaries fall between them")
    if a.clusters:
        for lo, hi in clusters:
            local = sorted({n for l2, h2, n in windows if lo <= l2 and h2 <= hi})
            print(f"  {names[lo]} .. {names[hi]} ({hi - lo + 1} functions): {', '.join(local[:6])}"
                  + (" ..." if len(local) > 6 else ""))
    if a.data:
        # 6. data blocks: each object's .data is one block, linked in the same order as its .text, so
        # the data a run of functions uses alone climbs block by block along the code. Within a
        # block the order is the file's definition order, and a global used by one run can still
        # be defined in another file: a block is evidence for a region, not proof of a file.
        lib = [(v(s[0]), v(subs[i + 1][0])) for i, s in enumerate(subs[:-1])
               if len(s) > 3 and s[1] == "lib" and s[-1] == ".data"]
        dstart = v(next(s[0] for s in subs if len(s) > 1 and s[1] in ("data", ".data")))
        local = []
        for n, ks in used_by.items():
            x = addr(n)
            if x is None or x < dstart or any(lo <= x < hi for lo, hi in lib) or not all(k in idx for k in ks):
                continue
            ii = sorted(idx[k] for k in ks)
            if ii[-1] - ii[0] <= a.window:
                local.append((ii[0], ii[-1], x, n))
        local.sort()
        groups = []                                          # [first fn, last fn, lo addr, hi addr, count]
        for lo, hi, x, n in local:
            g = groups[-1] if groups else None
            if g and g[2] - 0x1000 <= x <= g[3] + 0x1000:
                g[1], g[2], g[3], g[4] = max(g[1], hi), min(g[2], x), max(g[3], x), g[4] + 1
            else:
                groups.append([lo, hi, x, x, 1])
        print(f"\ndata used by one run of <= {a.window} functions, grouped into address blocks along the code "
              f"({len(local)} symbols; a lone symbol out of line is usually a global defined elsewhere):")
        for lo, hi, x0, x1, c in groups:
            print(f"  {lo:4d}-{hi:<4d} {names[lo]} .. {names[hi]}: {x0:08X}-{x1:08X} ({c})")
        # Inversions: run A before run B in the code but A's data after B's can't be two files (both
        # sections keep file order), so B..A is one file. Only inside one game .data chunk (chunks are
        # split by library objects), and only short spans: a global defined in another file and used
        # by one far function inverts against everything in between.
        chunk = lambda x: sum(1 for lo, _ in lib if lo <= x)
        if ovl:
            # an overlay's data block ends with the uninitialised globals (every file's .bss and the
            # commons, after all the .data), seen as the all-zero tail: its own chunk
            dend = next((s[0] for s in subs if s[0] > dstart - seg["vram"] + seg["start"]), len(exe))
            bss = zero_tail_start(dstart, exe[dstart - seg["vram"] + seg["start"]:dend])
            chunk = lambda x: 1 if x >= bss else 0
            print(f"(initialised data {dstart:08X}-{bss:08X}; the zero tail after it counts as .bss)")
        pairs = sorted((l2, h1, n1, x1, n2, x2) for l1, h1, x1, n1 in local for l2, h2, x2, n2 in local
                       if x1 < x2 and l1 > h2 and chunk(x1) == chunk(x2) and h1 - l2 <= 40)
        spans = [(l, h) for l, h, *_ in pairs]
        same = []
        for l, h in spans:
            if same and l <= same[-1][1]:
                same[-1][1] = max(same[-1][1], h)
            else:
                same.append([l, h])
        print("one file each (private data out of code order inside the span; boundaries lie between):")
        for l, h in same:
            print(f"  {l:4d}-{h:<4d} {names[l]} .. {names[h]}")
            if a.why:
                for l2, h1, n1, x1, n2, x2 in pairs:
                    if l <= l2 and h1 <= h:
                        print(f"        {n2} {x2:08X} (used by {l2}..) is after {n1} {x1:08X} (used up to {h1})")
        # 7. calls: a function only nearby code calls (no other unit, no data table, no far caller) is
        # probably static, so it shares its callers' file and no boundary lies between them.
        name2key = {}
        for k, e in funcs.items():
            name2key[e["name"]] = name2key[e.get("orig", e["name"])] = k
        callers = {}
        for k, e in funcs.items():
            for c in e.get("calls", []):
                callers.setdefault(c, set()).add(k)
        for k, r in refs.items():
            for n, o in r["syms"]:
                t = name2key.get(n)
                if t and t != k:
                    callers.setdefault(t, set()).add(k)
        tabled = set()
        for p in glob.glob("asm/data/**/*.s", recursive=True):
            for m in re.finditer(r"\.word (\w+)\s*$", read(p), re.M):
                if name2key.get(m.group(1)) in idx:
                    tabled.add(name2key[m.group(1)])
        tied = [0] * (nf - 1)
        for k, _ in fl:
            cs = callers.get(k, set())
            if not cs or k in tabled or not all(c in idx for c in cs):
                continue
            ii = [idx[c] for c in cs] + [idx[k]]
            if max(ii) - min(ii) <= 40:
                for g in range(min(ii), max(ii)):
                    tied[g] += 1
        inside = {g for l, h in same for g in range(l, h)} | {g for lo, hi in clusters for g in range(lo, hi)}
        free = [g for g in range(nf - 1) if not tied[g] and g not in inside]
        zones = []
        for g in free:
            if zones and zones[-1][1] == g - 1:
                zones[-1][1] = g
            else:
                zones.append([g, g])
        print(f"possible cut zones (no probably-static call, shared private data or data inversion spans "
              f"the gap; a boundary may lie after any function listed, none is proven):")
        for g0, g1 in zones:
            print(f"  after {g0:4d}..{g1:<4d} {names[g0]} .. {names[g1]}")
    ranked = sorted(range(nf - 1), key=lambda g: -gaps[g]["score"])
    print(f"\ntop boundary candidates (gap after function i; + style, rodata, strings; - statics spanning it):")
    for g in ranked[:a.top]:
        if gaps[g]["score"] <= 0:
            break
        k = fl[g + 1][0]
        print(f"{gaps[g]['score']:5.1f}  {names[g]} | {names[g + 1]} @{v(fl[g + 1][1]['rom']):08X}  "
              + "; ".join(sorted(set(gaps[g]["why"])))[:150])


def cmd_block(a):
    funcs = need_funcs()
    key, e = find_func(funcs, a.func)
    st = jload(P_STATE, {})
    rec = st.setdefault(key, {})
    rec.update(status="blocked", note=a.note, blocked_at=now())
    jsave(P_STATE, st)
    print(f"parked {e['name']}: {a.note}")
    cmd_progress(a)  # the dashboard counts parked functions


def cmd_unblock(a):
    """Unpark functions so `next`/`queue` (and the swarm) offer them again; the park note
    stays as the function's note, which `next` and the swarm's workers see."""
    funcs = need_funcs()
    st = jload(P_STATE, {})
    for name in a.funcs:
        key, e = find_func(funcs, name)
        rec = st.get(key, {})
        if rec.get("status") != "blocked":
            die(f"{e['name']} isn't parked")
        rec.update(status="todo", unblocked_at=now(), why_unblocked=a.why)
        print(f"unparked {e['name']}")
    jsave(P_STATE, st)
    cmd_progress(a)


def write_compile_sh(d, cfg):
    """d/compile.sh for the permuter: the unit's pipeline, `compile.sh IN.c -o OUT.o`."""
    g = int(cfg.get("GVAL", "0"))
    cmd = pipeline('"$1"', '"$3"', cfg["CC_VER"], f"{cfg['OPT_FLAGS']} {cfg.get('EXTRA_CC1FLAGS', '')}".strip(), g,
                   re.sub(r"\s*-G\d+", "", cfg.get("MASPSX_ARGS", DEFAULT_MASPSX)), post_filters(cfg))
    cmd = cmd.replace("-Iinclude", "-I/work/include").replace("-Iext/", "-I/work/ext/")
    write(f"{d}/compile.sh", f"#!/bin/bash\nset -eo pipefail\ncd /work\n{cmd}\n")
    os.chmod(f"{d}/compile.sh", 0o755)


def permute_run(d, name, e, base_c, cfg, jobs, seconds, plain_c=None, mode="whole", unit=None):
    """Set up and run decomp-permuter in `d` on base_c for `seconds`. plain_c: base_c without its
    PERM_ macros, if it has any (the pruned context is checked by compiling it). Records the run
    (record_permute). Returns (best score or None, best output dir or None, base score or None, log)."""
    shutil.rmtree(d, ignore_errors=True)
    os.makedirs(d)
    write(f"{d}/base.c", base_c)
    write(f"{d}/target.s", f'.include "macro.inc"\n.set noat\n.set noreorder\n.text\n.include "{e["asm"]}"\n')
    r = run([X + "as", *make_flags("ASFLAGS"), "-o", f"{d}/target.o", f"{d}/target.s"])
    if r.returncode:
        return None, None, None, "could not assemble target:\n" + tail(r.stderr)
    write_compile_sh(d, cfg)
    write(f"{d}/settings.toml", f'func_name = "{name}"\ncompiler_type = "gcc"\n')
    # The permuter looks for mips-linux-gnu-objdump; the image has the mipsel- build of the same tool.
    shim = os.path.abspath("build/permuter/bin")
    os.makedirs(shim, exist_ok=True)
    if not os.path.exists(f"{shim}/mips-linux-gnu-objdump"):
        real = shutil.which(X + "objdump")
        if real:
            os.symlink(real, f"{shim}/mips-linux-gnu-objdump")
    env = dict(os.environ, PATH=shim + os.pathsep + os.environ.get("PATH", ""),
               PERMUTER_SYMBOLS=os.path.abspath(write_permuter_symbols(d, e)))
    plain = os.environ.get("PERMUTER_PLAIN") == "1"
    context = f"full {len(base_c) // 1024} KB"
    if not plain:
        # only the declarations the function needs, if they compile it to the same code
        pruned = prune_context(base_c, name)
        pruned_plain = prune_context(plain_c, name) if pruned and plain_c is not None else pruned
        if (pruned and pruned_plain and pruned[1] == pruned_plain[1]
                and same_code(d, plain_c if plain_c is not None else base_c, pruned_plain[0])):
            write(f"{d}/base.c", pruned[0])
            context = f"pruned {len(base_c) // 1024} -> {len(pruned[0]) // 1024} KB"
        else:
            print("(permuter: the pruned context doesn't parse or compiles differently; using the full one)")
    # own process group, so a timeout also stops the permuter's worker processes
    t0 = time.time()
    p = subprocess.Popen(["python3", PERMUTER if plain else PERMUTER_FAST, d, f"-j{jobs}", "--stop-on-zero"],
                         stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, start_new_session=True, env=env)
    try:
        log = p.communicate(timeout=seconds)[0]
    except subprocess.TimeoutExpired:
        os.killpg(p.pid, 9)
        log = p.communicate()[0] or ""
    elapsed = time.time() - t0
    write(f"{d}/log.txt", log)
    m = re.search(r"base score\D*(\d+)", log, re.I)
    outs = []
    for q in glob.glob(f"{d}/output-*"):
        mm = re.match(r"output-(\d+)-", os.path.basename(q))
        if mm:
            outs.append((int(mm.group(1)), q))
    best, path = min(outs) if outs else (None, None)
    base = int(m.group(1)) if m else None
    its = re.findall(r"iteration (\d+)", log)
    record_permute({"at": now(), "name": name, "seg": e.get("seg"), "size": e.get("size"), "unit": unit,
                    "mode": mode, "seconds": round(elapsed), "jobs": jobs, "iterations": int(its[-1]) if its else None,
                    "per_s": round(int(its[-1]) / elapsed, 1) if its and elapsed else None, "base": base,
                    "best": best if best is not None else base, "matched": best == 0,
                    "pipeline": "stock" if plain else "fast", "context": context})
    return best, path, base, log


def write_permuter_symbols(d, e):
    """d/symbols.txt: `name 0xADDRESS` for every symbol the function can reach (main, the engine
    and its own overlay; the slot overlays share addresses), for permuter_fast's scoring by
    address. A name with two addresses there is left out."""
    seg = e.get("seg") or ""
    ovl = seg.split("/")[1] if seg.startswith("overlays/") else None
    addrs, clash = {}, set()
    for b in ["main", "engine"] + ([ovl] if ovl and ovl != "engine" else []):
        p = f"{CFG}/sym.{b}.txt"
        if not os.path.exists(p):
            continue
        for line in read(p).splitlines():
            m = SYM_LINE.match(line)
            if m:
                a = int(m.group(2), 16)
                if addrs.setdefault(m.group(1), a) != a:
                    clash.add(m.group(1))
    path = f"{d}/symbols.txt"
    write(path, "".join(f"{n} 0x{a:08X}\n" for n, a in sorted(addrs.items()) if n not in clash))
    return path


def record_permute(rec):
    """One permuter run: a PERMUTE-STATS line (tools/swarm keeps them) and a line in P_PERMLOG."""
    line = json.dumps(rec)
    print(f"PERMUTE-STATS {line}")
    try:
        with open(P_PERMLOG, "a") as f:
            f.write(line + "\n")
    except OSError:
        pass


def cmd_permstats(a):
    """The permuter's record (record_permute lines) by function size and mode: how often a run
    improves the score or matches, for how much CPU, and its speed per job."""
    rows = []
    for path in a.files or [P_PERMLOG]:
        if os.path.exists(path):
            rows += [json.loads(line) for line in read(path).splitlines() if line.strip()]
    if not rows:
        die(f"no permuter runs recorded in {', '.join(a.files or [P_PERMLOG])}")
    edges = [0, 150, 250, 400, 600, 900, 1400, float("inf")]

    def med(xs):
        xs = sorted(x for x in xs if x is not None)
        return xs[len(xs) // 2] if xs else None

    print(f"{len(rows)} permuter runs, {sum(r.get('seconds') or 0 for r in rows) / 3600:.1f} h")
    print("  bytes        mode        runs  cpu-min  improved  matched  med it/s/job  med base -> best")
    for lo, hi in zip(edges, edges[1:]):
        for mode in sorted({r.get("mode") or "?" for r in rows}):
            b = [r for r in rows if lo <= (r.get("size") or 0) < hi and (r.get("mode") or "?") == mode]
            if not b:
                continue
            cpu = sum((r.get("seconds") or 0) * (r.get("jobs") or 1) for r in b) / 60
            imp = sum(1 for r in b if r.get("base") is not None and r.get("best") is not None and r["best"] < r["base"])
            won = sum(1 for r in b if r.get("matched"))
            speed = med([r["per_s"] / (r.get("jobs") or 1) for r in b if r.get("per_s")])
            label = f"{lo}-{'' if hi == float('inf') else hi - 1}"
            print(f"  {label:<11}  {mode:<10} {len(b):>5}  {cpu:>7.0f}  {imp:>8}  {won:>7}  "
                  f"{'-' if speed is None else f'{speed:.1f}':>12}  {med([r.get('base') for r in b])} -> "
                  f"{med([r.get('best') for r in b])}")
    for pipe in sorted({r.get("pipeline") or "?" for r in rows}):
        b = [r for r in rows if (r.get("pipeline") or "?") == pipe]
        speed = med([r["per_s"] / (r.get("jobs") or 1) for r in b if r.get("per_s")])
        print(f"  pipeline {pipe}: {len(b)} runs, median {'-' if speed is None else f'{speed:.1f}'} it/s per job")


def cmd_permute_apply(a):
    """Put a permuter output's version of the function in its source if `try` gets a match or at
    least builds (tools/swarm, for a permuter match in a run). Prints the PERMUTE-RESULT line."""
    proj, funcs = need_proj(), need_funcs()
    key, e = find_func(funcs, a.func)
    name = cur_name(key, e, sym_names())
    src = e.get("src") or locate_def(name)
    if not src or not os.path.exists(f"{a.dir}/source.c"):
        die(f"no source for {name} or no {a.dir}/source.c")
    apply_permuter_best(key, e, proj, name, src, a.dir, None)


def apply_permuter_best(key, e, proj, name, src, out_dir, n0):
    """--apply-best: put the permuter's best version of the function in src (it's the preprocessed
    text, so macros come back expanded), and keep it only if `try` gets fewer differences than
    n0 (or a match); else src is restored. Ends with one line for tools/swarm:
    PERMUTE-RESULT NAME kept|reverted before N0 after N1 (N1 0 = match)."""
    new = extract_function(read(f"{out_dir}/source.c"), name)
    text = read(src)
    old = extract_function(text, name)
    if not new or not old:
        print(f"PERMUTE-RESULT {name} reverted before {n0} after ? (function not found in "
              f"{'the permuter output' if not new else src})")
        return
    write(src, text.replace(old, new, 1))
    ok, n1 = check_function(key, e, proj, name)
    n1 = 0 if ok else n1
    if n1 is None or (n0 is not None and n1 >= n0):
        write(src, text)
        print(f"PERMUTE-RESULT {name} reverted before {n0} after {n1}")
    else:
        print(f"PERMUTE-RESULT {name} kept before {n0} after {n1}")


DECL_LINE = re.compile(r"^\s*(?!return\b|goto\b|case\b|else\b)(?:(?:const|volatile|unsigned|signed|struct\s+\w+|\w+)\s+)+"
                       r"\**\s*\w+\s*(\[[^\]]*\])?\s*(=[^;]*)?;\s*$")


def perm_wrap(body, spans):
    """Wrap the function body's lines in each (first, last) span (0-based, inside the body) in
    PERM_RANDOMIZE(...), widened to whole statements, so the permuter only edits there. Spans are
    kept off the declarations and the closing brace, and merged when they overlap."""
    lines = body.split("\n")
    first = 1
    while first < len(lines) and (not lines[first].strip() or DECL_LINE.match(lines[first])):
        first += 1
    last = len(lines) - 1
    while last > 0 and lines[last].strip() != "}":
        last -= 1
    ends = (";", "{", "}", ":")
    out = []
    for lo, hi in sorted(spans):
        lo, hi = max(lo, first), min(hi, last - 1)
        if lo > hi:
            continue
        for _ in range(400):
            depth, low = 0, 0
            for ch in "\n".join(lines[lo:hi + 1]):
                depth += (ch == "{") - (ch == "}")
                low = min(low, depth)
            if low < 0 and lo > first:              # closes a block opened above: take it in
                lo -= 1
            elif depth > low and hi < last - 1:     # opens a block it doesn't close
                hi += 1
            elif not lines[hi].rstrip().endswith(ends) and hi < last - 1:
                hi += 1
            elif lo > first and lines[lo - 1].strip() and not lines[lo - 1].rstrip().endswith(ends):
                lo -= 1
            elif hi < last - 1 and re.match(r"\s*(\}\s*)?else\b", lines[hi + 1]):
                hi += 1
            elif re.match(r"\s*(\}\s*)?else\b", lines[lo]) and lo > first:
                lo -= 1
            else:
                break
        if out and lo <= out[-1][1] + 1:
            out[-1] = (out[-1][0], max(hi, out[-1][1]))
        else:
            out.append((lo, hi))
    for lo, hi in reversed(out):
        lines[hi] += "\n)"
        lines[lo] = "PERM_RANDOMIZE(\n" + lines[lo]
    return "\n".join(lines), out


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
    plain = body
    if a.block or a.lines or a.differing:
        # only edit the C of the blocks that differ (or the given lines): PERM_RANDOMIZE regions
        text = read(src)
        line0 = text.count("\n", 0, func_def_regex(name).search(text).start()) + 1
        spans = [tuple(int(x) for x in s.split("-")) for s in (a.lines or [])]
        if a.block or a.differing:
            res, lines, *_ = region_score(key, e, proj, name, src)
            if not lines:
                die("no C line map for the blocks (the -g build failed); use --lines A-B")
            want = set(a.block or []) | ({k for k, b in enumerate(res["blocks"]) if b["status"] != "ok"}
                                         if a.differing else set())
            spans += [s for k in sorted(want) if k < len(res["blocks"]) for s in [block_span(res["blocks"][k], lines)] if s]
        body, wrapped = perm_wrap(body, [(lo - line0, hi - line0) for lo, hi in spans])
        if not wrapped:
            die("no statements to randomize in those blocks/lines")
        print("randomizing only " + ", ".join(f"L{lo + line0}-{hi + line0}" for lo, hi in wrapped))
    d = f"build/permuter/{name}"
    jobs = a.jobs or max(1, (os.cpu_count() or 2) - 1)
    base_c = permuter_file_base(src, name, body, name)
    if base_c is None:
        die(f"could not preprocess {name} for the permuter - does `try {name}` build?")
    plain_c = permuter_file_base(src, name, plain, f"{name}_plain") if plain != body else None
    n0 = check_function(key, e, proj, name, quiet=True)[1] if a.apply_best else None
    best, path, base, log = permute_run(d, name, e, base_c, unit_cfg(cfg, src), jobs, a.minutes * 60, plain_c,
                                        "region" if plain != body else "whole", src)
    if path is None:
        print(f"permuter found no improvement{f' (base score {base})' if base is not None else ''}; log {d}/log.txt")
        if base is None:
            print(tail(log, 6))
        if a.apply_best:
            print(f"PERMUTE-RESULT {name} unchanged before {n0}")
        return
    if a.apply_best:
        apply_permuter_best(key, e, proj, name, src, path, n0)
        return
    print(f"permuter best score {best}{f' (base {base})' if base is not None else ''}: {path}/source.c"
          + ("  <- MATCH, apply this function's changes and run try" if best == 0 else ""))
    if os.path.exists(f"{path}/diff.txt"):
        print(f"changes vs base: {path}/diff.txt")


# ---------------------------------------------------------------- progress + dashboard

PLACEHOLDER_NAME = re.compile(r"^(func|sub|fn)_[0-9A-Fa-f]{8}$")


# raw offset arithmetic m2c leaves behind: a matched function with this is not typed yet. A line
# whose comment starts "matching:" keeps a byte form on purpose (the comment says what the typed
# form changes) and counts as typed.
UNTYPED = re.compile(r"\(\s*(?:u8|s8|char)\s*\*\s*\)\s*\(?\s*\w+\s*\)?\s*\+|M2C_FIELD")
KEPT_RAW = "/* matching:"


def untyped_lines(body):
    """The lines of BODY with a raw offset not marked as kept for the matching."""
    return [l for l in body.split("\n") if UNTYPED.search(l) and KEPT_RAW not in l]


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


TYPED_CACHE = "build/cache/typed.json"


def typed_matched(funcs, stat, proj, syms):
    """{key: whether its C is free of raw offsets} for the matched game functions; a function with
    no C of its own is left out. Cached in build/ per source file version (mtime, size): finding
    ~1200 bodies by regex took ~8 s of every `done`, and a match changes one file."""
    cache, out, dirty = jload(TYPED_CACHE, {}), {}, False
    by_src = defaultdict(list)
    for k, e in funcs.items():
        if stat[k] == "matched" and e.get("src") and not is_lib(e, proj):
            by_src[e["src"]].append(k)
    for src, keys in by_src.items():
        try:
            st = os.stat(src)
        except FileNotFoundError:
            continue
        ver = [st.st_mtime_ns, st.st_size, UNTYPED.pattern, KEPT_RAW]
        ent = cache.get(src)
        if not ent or ent["ver"] != ver:
            ent = cache[src] = {"ver": ver, "names": {}}
            dirty = True
        text = None
        for k in keys:
            name = cur_name(k, funcs[k], syms)
            if name not in ent["names"]:
                text = read(src) if text is None else text
                body = extract_function(text, name)
                ent["names"][name] = None if body is None else not untyped_lines(body)
                dirty = True
            if ent["names"][name] is not None:
                out[k] = ent["names"][name]
    if dirty:
        jsave(TYPED_CACHE, cache)
    return out


def summary(funcs, stat, proj):
    """Bytes matched per group and per unit. Functions written in assembly originally (kind asm)
    are recovered as .s and counted apart from the C totals; names and copies are tracked too."""
    syms = sym_names()
    blank = lambda: {"done_b": 0, "total_b": 0, "done_f": 0, "total_f": 0, "asm_b": 0, "asm_f": 0,
                     "named_f": 0, "all_f": 0, "blocked": 0, "sony_b": 0, "c_f": 0, "typed_f": 0}
    typed = typed_matched(funcs, stat, proj, syms)
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
        ty = typed.get(k)
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


# subsegments that are memory images, not data any source defined: the engine file carries the
# slot overlays' load area as it was in the build machine's memory (a stub id, zeros, leftover
# copies of CRASHBSH.DAT files and the leaked headers; ARCHITECTURE.md, The engine's slot image).
# It stays a file read from the user's disc and is counted apart from the data to turn into C.
# The engine's .bss (its last 0x465C bytes before the slot) is one too: two words and the mode text
# buffer, holding the build machine's leftovers.
SLOT_IMAGES = {"overlays/engine/engine_slot", "overlays/engine/engine_bss"}


def data_items(funcs, stat, proj, images=False):
    """Every data block outside the code subsegments as [unit, file, symbol, bytes, in C, type]: rodata
    migrated into a matched function and data subsegments owned by a C file (.data etc.) with
    nothing left in asm count as C. Asm-owned subsegments are cut at their splat labels, so the
    dashboard's data map shows each table. The blocks tile the subsegments exactly. The memory
    images in SLOT_IMAGES are left out, or with images=True are all that is returned."""
    syms = sym_names()  # once: inside the comprehension it re-read every symbol file per function
    matched = {cur_name(k, e, syms) for k, e in funcs.items() if stat.get(k) == "matched"}
    out = []
    inputs, cache = split_inputs(), jload(RODATA_CACHE, {})
    elves = {}
    units = [(proj['name'], "main exe")] + [(o['name'], o['name']) for o in proj.get("overlays", [])]
    for yname, unit in units:
        y = yaml_of(yname)
        if not os.path.exists(y):
            continue
        for start, end, typ, name in subsegments(y):
            if typ not in DATA_TYPES or end is None or (name in SLOT_IMAGES) != images:
                continue
            if typ == ".rodata":
                rs = [s for s in rodata_syms(name, inputs, cache) if start <= s[0] < end]
                if not rs or rs[0][0] > start:
                    # a C file's rodata with no asm piece left in it is all the compiler's
                    out.append([unit, name, name, (rs[0][0] if rs else end) - start,
                                not glob.glob(f"asm/nonmatchings/{name}/D_*.s"), typ])
                tables = jump_table_labels(rs)
                linked = elf_rom_names(yname, elves)
                for i, (rom, sym, owners) in enumerate(rs):
                    nxt = rs[i + 1][0] if i + 1 < len(rs) else end
                    done = len(owners) == 1 and owners[0] in matched and not os.path.exists(f"asm/nonmatchings/{name}/{sym}.s")
                    # splat's label predates the C: a piece the C defines goes by the C's name
                    shown = tables.get(sym, sym)
                    if done and DATA_PLACEHOLDER.match(shown):
                        # else it is a function's own literal, nameless in C like a jump table
                        shown = linked.get(rom) or f"{owners[0]} literal"
                    out.append([unit, name, shown, nxt - rom, done, typ])
            elif typ.startswith("."):
                out.append([unit, name, name, end - start, not glob.glob(f"asm/nonmatchings/{name}/D_*.s"), typ])
            else:
                out += [[unit, name, sym, size, False, typ] for sym, size in asm_data_labels(name, typ, start, end)]
    return out


def elf_rom_names(yname, cache):
    """{ROM offset: name} of a build target's linked symbols (build/main.elf or build/<ovl>.ovl.elf),
    through its yaml's segment addresses; empty when it isn't built. Placeholders and the
    linker's section markers are left out."""
    if yname in cache:
        return cache[yname]
    import yaml
    elf = "build/main.elf" if yname == "main" else f"build/{yname}.ovl.elf"
    out = {}
    if os.path.exists(elf):
        segs = [s for s in yaml.safe_load(read(yaml_of(yname))).get("segments", [])
                if isinstance(s, dict) and "vram" in s]
        for line in run(["mipsel-linux-gnu-nm", "-n", "--defined-only", elf]).stdout.splitlines():
            p = line.split()
            if len(p) != 3 or p[1] in "aA" or DATA_PLACEHOLDER.match(p[2]) or PLACEHOLDER_NAME.match(p[2]) \
                    or p[2].endswith(("_START", "_END", "_SIZE", "_VRAM")) or p[2].startswith(("gcc2", "__gnu", "jtbl_")):
                continue
            a = int(p[0], 16)
            rom = None
            for s in segs:
                if s["vram"] <= a:
                    rom = a - s["vram"] + s["start"]
            if rom is not None:
                out.setdefault(rom, p[2])
    cache[yname] = out
    return out


def jump_table_labels(rs):
    """{jtbl_ label: what the dashboard calls it} for rodata_syms() rows: a switch's jump table has
    no name in C (cc1 emits it under a local label), so it is shown by the one function it
    belongs to, "PlotTotemBit switch table", numbered in address order when there are several."""
    own = defaultdict(list)
    for rom, sym, owners in sorted(rs):
        if sym.startswith("jtbl_") and len(owners) == 1:
            own[owners[0]].append(sym)
    out = {}
    for fn, syms in own.items():
        for i, sym in enumerate(syms):
            out[sym] = f"{fn} switch table" + (f" {i + 1}" if len(syms) > 1 else "")
    return out


# nm letters of a C object's data symbols, by section (globals upper case, statics lower)
NM_SECTION = {"d": ".data", "r": ".rodata", "b": ".bss", "g": ".sdata", "s": ".sbss"}
# a data name nobody chose: splat's D_ label, or a whole asm file with no label in it
DATA_PLACEHOLDER = re.compile(r"^D_[0-9A-Fa-f]{8}$")


def object_data_syms(path):
    """{section: [(offset, name)]} of a compiled C object's data symbols, one name per offset."""
    out = defaultdict(dict)
    for line in run(["mipsel-linux-gnu-nm", "-n", "--defined-only", path]).stdout.splitlines():
        p = line.split()
        sect = len(p) == 3 and NM_SECTION.get(p[1].lower())
        if sect and not p[2].startswith((".", "$", "_MACRO")):
            out[sect].setdefault(int(p[0], 16), p[2])
    return {s: sorted(v.items()) for s, v in out.items()}


def split_c_block(name, typ, size, cache):
    """[(symbol, bytes)] tiling the first size bytes of a C file's typ section from its object's
    symbols; the bytes before the first symbol (string literals, jump tables) keep the file's
    name. None when the object isn't built or has no symbols there."""
    path = f"build/src/{name}.c.o"
    if not os.path.exists(path):
        return None
    if path not in cache:
        cache[path] = object_data_syms(path)
    syms = [(o, n) for o, n in cache[path].get(typ, []) if o < size]
    if not syms:
        return None
    if syms[0][0] > 0:
        syms.insert(0, (0, os.path.basename(name)))
    return [(n, (syms[i + 1][0] if i + 1 < len(syms) else size) - o) for i, (o, n) in enumerate(syms)]


def asm_data_labels(name, typ, start, end):
    """[(label, bytes)] tiling an asm data subsegment, from splat's dlabel lines and the ROM offset
    comment on the line after each; the whole subsegment when its file has no labels."""
    p = f"asm/data/{name}.{typ}.s"
    labels = []
    if os.path.exists(p):
        pend = None
        for line in read(p).splitlines():
            m = re.match(r"\s*dlabel\s+(\S+)", line)
            if m:
                pend = m.group(1)
                continue
            m = pend and re.match(r"\s*/\*\s*([0-9A-Fa-f]+)\s", line)
            if m:
                rom = int(m.group(1), 16)
                if start <= rom < end and (not labels or rom > labels[-1][1]):
                    labels.append((pend, rom))
                pend = None
    if not labels or labels[0][1] > start:
        labels.insert(0, (os.path.basename(name), start))
    return [(sym, (labels[i + 1][1] if i + 1 < len(labels) else end) - rom) for i, (sym, rom) in enumerate(labels)]


def data_summary(funcs, stat, proj, items=None):
    """Data bytes (everything outside code subsegments but the slot images) and how much of it
    is produced by C; slot_b is the slot images' bytes, counted apart."""
    items = data_items(funcs, stat, proj) if items is None else items
    total = sum(it[3] for it in items)
    done = sum(it[3] for it in items if it[4])
    slot = sum(it[3] for it in data_items(funcs, stat, proj, images=True))
    return {"done_b": done, "total_b": total, "pct": 100.0 * done / total if total else 0.0, "slot_b": slot}


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


def page_history(hist):
    """The history as the page draws it: one column per figure (t: seconds since the point
    before), and each unit's [point, bytes, functions] only where it changes. A match moves one
    unit, so this is ~10x smaller than the points themselves."""
    pts = [h for h in hist if h.get("game_tb")]
    out = {"t": [], "game_b": [], "game_f": [], "named_f": [], "data_b": [], "units": {}}
    prev, last = 0, {}
    for i, h in enumerate(pts):
        ts = int(datetime.datetime.strptime(h["ts"], "%Y-%m-%dT%H:%M:%SZ")
                 .replace(tzinfo=datetime.timezone.utc).timestamp())
        out["t"].append(ts - prev)
        prev = ts
        for col, k in (("game_b", "game_b"), ("game_f", "game_f"), ("named_f", "game_named_f"), ("data_b", "data_b")):
            out[col].append(h.get(k, 0))
        for un, v in sorted((h.get("units") or {}).items()):
            if last.get(un) != list(v):
                out["units"].setdefault(un, []).append([i, *v])
                last[un] = list(v)
    return out


MAP_STATUS = {"matched": "m", "blocked": "p"}   # anything else still to match: "t"


def page_map(funcs, stat, proj, syms):
    """Every game function for the page's code map, in unit, file and address order: [seg, unit,
    count] per file, then per function its name, size, status letter (m matched, p parked,
    t to do) and whether it has a real name (nm: 1 or 0). The same functions as the game totals."""
    rows = sorted((unit_of(e, proj), e["seg"], e.get("rom", 0), cur_name(k, e, syms), e["size"],
                   MAP_STATUS.get(stat[k], "t"))
                  for k, e in funcs.items() if not is_lib(e, proj) and stat[k] not in ("asm", "sony", "unsplit"))
    out = {"segs": [], "n": [], "s": [], "t": "", "nm": ""}
    for un, seg, _, name, size, st in rows:
        if not out["segs"] or out["segs"][-1][0] != seg:
            out["segs"].append([seg, un, 0])
        out["segs"][-1][2] += 1
        out["n"].append(name)
        out["s"].append(size)
    out["t"] = "".join(r[5] for r in rows)
    out["nm"] = "".join("0" if PLACEHOLDER_NAME.match(r[3]) else "1" for r in rows)
    return out


def page_data_map(items):
    """data_items() packed like page_map: [file, unit, count] per file, then per block its name,
    bytes, status letter (c in C, a still asm) and whether it has a real name (nm: 1 or 0). A
    block a C file owns whole is cut at its object's symbols, so each C object is a cell. A D_
    label is a placeholder, and so is an asm file's name standing for a block with no label. The
    compiler's own data is not: the file's name on a C block's unlabelled bytes, and a matched
    function's rodata (its string literals and jump tables, under splat's labels)."""
    out = {"segs": [], "n": [], "s": [], "t": "", "nm": ""}
    cache = {}
    # a file's blocks together (its .rodata and its .data are apart in address order), files in
    # the order they first appear, so each file is listed once
    first = {}
    for i, it in enumerate(items):
        first.setdefault((it[0], it[1]), i)
    items = sorted(items, key=lambda it: first[(it[0], it[1])])
    for un, seg, sym, size, done, typ in items:
        if size <= 0:
            continue
        parts = split_c_block(seg, typ, size, cache) if done and sym == seg else None
        for name, n in parts or [(sym, size)]:
            if n <= 0:
                continue
            if not out["segs"] or out["segs"][-1][0] != seg:
                out["segs"].append([seg, un, 0])
            out["segs"][-1][2] += 1
            out["n"].append(name)
            out["s"].append(n)
            out["t"] += "c" if done else "a"
            if done and typ == ".rodata" and parts is None:
                unnamed = False
            else:
                unnamed = DATA_PLACEHOLDER.match(name) or (not done and name in (seg, os.path.basename(seg)))
            out["nm"] += "0" if unnamed else "1"
    return out


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
    jsave_rows(P_HIST, hist)
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
    # one line per history point: indented, the points alone pass the 2 MB pre-commit limit
    text = json.dumps({**data, "history": []}, indent=1, sort_keys=True)
    points = ",\n  ".join(json.dumps(p, sort_keys=True) for p in hist)
    text = text.replace('"history": []', '"history": [\n  ' + points + "\n ]" if hist else '"history": []', 1)
    with open("docs/progress.json.tmp", "w", encoding="utf-8", newline="\n") as f:
        f.write(text + "\n")
    os.replace("docs/progress.json.tmp", "docs/progress.json")
    # the page gets the history packed and the code map; progress.json keeps the plain points
    page = {k: v for k, v in data.items() if k not in ("history", "recent", "blocked")}
    page.update(hist=page_history(hist), map=page_map(funcs, stat, proj, syms),
                dmap=page_data_map(data_items(funcs, stat, proj)))
    write("docs/index.html", read(DASHBOARD).replace("/*DATA*/null", json.dumps(page, separators=(",", ":"))
                                                     .replace("</", "<\\/")))
    g, l = s["game"], s["lib"]
    fmt = lambda t: f"{t['pct']:.2f}% ({t['done_b']}/{t['total_b']} bytes, {t['done_f']}/{t['total_f']} functions)"
    print(f"progress: game code {fmt(g)}; libs {fmt(l)}, +{l['asm_b']} bytes hand-written asm; "
          f"game functions named {g['named_f']}/{g['all_f']}; typed {g['typed_f']}/{g['c_f']} matched; "
          f"data {s['data']['pct']:.2f}%; "
          f"parked {s['blocked']} -> docs/index.html")


# the page: tools/scripts/dashboard.html with the data in place of its /*DATA*/null
DASHBOARD = os.path.join(os.path.dirname(os.path.abspath(__file__)), "dashboard.html")


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
    p.add_argument("--asm-lines", type=int, default=400)
    p.set_defaults(fn=cmd_next)
    p = sub.add_parser("queue", help="the next targets as JSON, marking nothing (tools/swarm)")
    p.add_argument("--seg")
    p.add_argument("--count", type=int, default=50)
    p.set_defaults(fn=cmd_queue)
    p = sub.add_parser("m2c", help="print m2c draft")
    p.add_argument("func")
    p.add_argument("--from", dest="start", type=int, default=1, help="from this line of the draft")
    p.set_defaults(fn=cmd_m2c)
    p = sub.add_parser("asm", help="a function's compact asm with offsets, jump tables and strings")
    p.add_argument("func")
    p.add_argument("--from", dest="start", default="0", help="from this offset (0x...)")
    p.add_argument("--lines", type=int, default=400)
    p.set_defaults(fn=cmd_asm)
    p = sub.add_parser("try", help="build and diff one or more functions")
    p.add_argument("funcs", nargs="+")
    p.add_argument("--file", nargs="+", metavar="FILE",
                   help="compile this C file (a test copy of the function, with its own #includes) "
                   "with the function's unit settings and diff it, without editing or rebuilding the "
                   "unit; no attempt is recorded. With several files: compile them in parallel, rank "
                   "them by the regions score and show the best one's diff")
    p.add_argument("--jobs", type=int, help="--file with several files: parallel compiles")
    p.set_defaults(fn=cmd_try)
    p = sub.add_parser("regions", help="score a function block by block (aligned diff, C lines)")
    p.add_argument("func")
    p.add_argument("--block", type=int, help="show this block's diff (default: the first that differs)")
    p.add_argument("--all", action="store_true", help="list matching blocks too")
    p.add_argument("--changed", action="store_true",
                   help="list only the blocks whose status changed since the last regions call")
    p.add_argument("--no-lines", action="store_true", help="skip the -g build that maps blocks to C lines")
    p.set_defaults(fn=cmd_regions)
    p = sub.add_parser("similar", help="matched functions whose code looks most like this one")
    p.add_argument("func", nargs="?")
    p.add_argument("--survey", action="store_true",
                   help="every function to match (in --seg) with its best matched look-alike")
    p.add_argument("--seg", help="--survey: segment prefix")
    p.add_argument("--top", type=int, default=10)
    p.add_argument("--min", type=float, default=0.3, help="lowest similarity (sim) to list, 0-1")
    p.add_argument("--contain", type=float, default=0.6, help="also list functions this share of which is inside NAME (in)")
    p.add_argument("--all", action="store_true", help="unmatched functions too (siblings to do together)")
    p.set_defaults(fn=cmd_similar)
    p = sub.add_parser("units", help="evidence for splitting a C segment into its translation units")
    p.add_argument("seg", nargs="?", default="main/main")
    p.add_argument("--window", type=int, default=12, help="longest run of functions a file-local symbol may span")
    p.add_argument("--top", type=int, default=25)
    p.add_argument("--clusters", action="store_true", help="list each cluster and its file-local symbols")
    p.add_argument("--data", action="store_true", help="group the data each run of functions uses alone into address blocks")
    p.add_argument("--why", action="store_true", help="with --data: the symbol pairs behind each data inversion")
    p.set_defaults(fn=cmd_units)
    p = sub.add_parser("vars", help="which C variables the differing blocks' register swaps belong to")
    p.add_argument("func")
    p.add_argument("--all", action="store_true", help="also list every variable's register or stack slot")
    p.set_defaults(fn=cmd_vars)
    p = sub.add_parser("splitvars", help="try splitting a local into several variables, scored by regions")
    p.add_argument("func")
    p.add_argument("var")
    p.add_argument("--max", type=int, default=300, help="most groupings to try")
    p.add_argument("--groups", help="try one grouping only, e.g. 0,0,1,1 (one number per assignment)")
    p.add_argument("--top", type=int, default=10)
    p.add_argument("--jobs", type=int)
    p.add_argument("--apply", action="store_true", help="write the best grouping into the source")
    p.set_defaults(fn=cmd_splitvars)
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
    p.add_argument("--data", action="store_true", help="also take each object's .data from the object, "
                   "cut out of the exe's data blob")
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
    p.add_argument("--apply-best", action="store_true",
                   help="put the best result in the source if try improves (else restore it)")
    p.add_argument("--differing", action="store_true",
                   help="only randomize the C of the blocks `regions` says differ")
    p.add_argument("--block", type=int, action="append", help="only randomize this block's C (repeatable)")
    p.add_argument("--lines", action="append", help="only randomize source lines A-B (repeatable)")
    p.set_defaults(fn=cmd_permute)
    p = sub.add_parser("port", help="re-apply an old diff of a function's C after renames and splits")
    p.add_argument("func")
    p.add_argument("diff", help="the old diff (a swarm checkpoint's best.diff)")
    p.set_defaults(fn=cmd_port)
    p = sub.add_parser("splitcheck", help="functions that look like two (a missing symbol to split them)")
    p.add_argument("--seg", help="segment prefix")
    p.add_argument("--all", action="store_true", help="matched functions too")
    p.set_defaults(fn=cmd_splitcheck)
    p = sub.add_parser("permute-apply", help="put a permuter output's function in the source (if try builds it)")
    p.add_argument("func")
    p.add_argument("dir", help="the output directory (with source.c)")
    p.set_defaults(fn=cmd_permute_apply)
    p = sub.add_parser("permstats", help="permuter runs by function size and mode: speed, improvements, matches")
    p.add_argument("files", nargs="*", help=f"run logs (default {P_PERMLOG}; the swarm's is <swarm dir>/permuter.jsonl)")
    p.set_defaults(fn=cmd_permstats)
    p = sub.add_parser("block", help="park a function with a note")
    p.add_argument("func")
    p.add_argument("--note", required=True)
    p.set_defaults(fn=cmd_block)
    p = sub.add_parser("unblock", help="unpark functions (their park notes stay) so they are offered again")
    p.add_argument("funcs", nargs="+")
    p.add_argument("--why", required=True, help="what changed since they were parked")
    p.set_defaults(fn=cmd_unblock)
    p = sub.add_parser("done", help="validate one hand match (scope, lint, function, full build), record it")
    p.add_argument("func")
    p.add_argument("--commit", action="store_true",
                   help="after every check passes, commit src/ include/ configs/ .decomp/ docs/")
    p.add_argument("--note", help="what made it match (types, statement order, ...): goes in the commit body")
    p.add_argument("--trailer", action="append", help="line appended to the commit message (repeatable)")
    p.add_argument("--defer-build", action="store_true",
                   help="batch mode (tools/swarm): skip the full build; run `decomp.py verify` after the "
                        "batch and publish none of its commits unless it passes")
    p.set_defaults(fn=cmd_done)
    p = sub.add_parser("sched", help="why cc1 allocated and scheduled a function so: global.c "
                       "priorities and each sched pass's choices")
    p.add_argument("func")
    p.add_argument("--pass", dest="passname", choices=["sched", "sched2"], help="one pass only")
    p.add_argument("--block", type=int, help="one basic block only")
    p.add_argument("--insn", help="only these insn uids (comma-separated)")
    p.set_defaults(fn=cmd_sched)
    p = sub.add_parser("jtrace", help="trace jump2's cross-jumping by insn uid: which copy of each "
                       "repeated tail is kept, and why the others gave theirs up")
    p.add_argument("func")
    p.add_argument("--cross", action="store_true", help="only the cross-jumps, not every comparison")
    p.set_defaults(fn=cmd_jtrace)
    p = sub.add_parser("trace", help="follow one insn, or every insn of a variable's pseudo, through "
                       "cc1's pass dumps: the pass that rewrote, moved or deleted it")
    p.add_argument("func")
    p.add_argument("--insn", type=int, help="this insn uid (as sched, jtrace and the dumps print it)")
    p.add_argument("--var", help="every insn that mentions this C variable's pseudo (by declaration order)")
    p.add_argument("--reg", type=int, help="every insn that mentions this pseudo")
    p.add_argument("--file", help="trace a test copy of the function, as try --file compiles it")
    p.set_defaults(fn=cmd_trace)
    p = sub.add_parser("idiom", help="matched functions whose asm has an instruction pattern, with the "
                       "C lines behind each hit, e.g. 'move A, B; sll B, A, N' (syntax: idiom.py)")
    p.add_argument("pattern")
    p.add_argument("--seg", help="segment prefix")
    p.add_argument("--top", type=int, default=12, help="functions to show, smallest first")
    p.add_argument("--gap", type=int, default=8, help="most instructions a `...` skips")
    p.add_argument("--context", type=int, default=1, help="instructions shown around each hit")
    p.add_argument("--all", action="store_true", help="search unmatched functions too (no C for them)")
    p.set_defaults(fn=cmd_idiom)
    p = sub.add_parser("orders", help="random orders of the statements on NAME's lines A-B that keep "
                       "their dependencies, compiled in parallel and ranked like try --file")
    p.add_argument("func")
    p.add_argument("--lines", required=True, help="A-B: source lines inside one block of the function")
    p.add_argument("--count", type=int, default=150, help="orders to try (the source's is the first)")
    p.add_argument("--seed", type=int, default=0)
    p.add_argument("--top", type=int, default=12, help="ranked rows to print")
    p.add_argument("--jobs", type=int, help="parallel compiles")
    p.add_argument("--file", help="reorder lines of this test copy instead of the source")
    p.add_argument("--free-memory", action="store_true", help="compare memory accesses by place "
                   "(a global, or fields and indexes from a name): stores to different globals, "
                   "fields or pointers may swap, as if they never alias")
    p.set_defaults(fn=cmd_orders)
    p = sub.add_parser("protos", help="a function's earlier compiles: its closest match in each "
                       "scanned prototype or demo disc (protos/*/scan.md)")
    p.add_argument("func")
    p.add_argument("--top", type=int, default=1, help="candidates per disc (default 1)")
    p.add_argument("--diff", metavar="DISC", help="align the copy on the disc whose protos/ "
                   "directory contains DISC (e.g. Sep_14) with retail by opcode and register, "
                   "immediates ignored, and print the hunks that differ")
    p.add_argument("--file", metavar="F.c", help="with --diff: align the build of this test copy "
                   "of the function (built as try --file builds it) with the disc's copy instead "
                   "of retail")
    p.add_argument("--words", action="store_true", help="with --diff: also print the disc's copy "
                   "in full, immediates kept")
    p.set_defaults(fn=cmd_protos)
    sub.add_parser("reconcile", help="headers: keep HEAD's declaration where the working tree "
                   "declares a name again (swarm gate clashes)").set_defaults(fn=cmd_reconcile)
    sub.add_parser("verify", help="clean rebuild and byte compare").set_defaults(fn=cmd_verify)
    sub.add_parser("progress", help="refresh docs/ dashboard and history").set_defaults(fn=cmd_progress)
    a = ap.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
