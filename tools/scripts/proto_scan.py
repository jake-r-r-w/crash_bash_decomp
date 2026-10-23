#!/usr/bin/env python3
"""Scan a prototype or demo disc for Crash Bash debug leftovers and map them to retail.

The build tools padded files with whatever was in the build machine's memory: the
retail engine holds source text, and the Aug 23 and Oct 9 prototype DATs and the
Jampack Winter 2000 demo's engine hold pieces of the SN linker's .SYM output
(docs/PROTOTYPES.md). This runs every check on one disc and writes what it finds
next to the disc:

    tools/dk python3 -I tools/scripts/proto_scan.py protos/<disc>/ [--out DIR]

<disc>/ holds the disc image (extract an archive on the host with 7-Zip first) or
its files: with no files/ folder, the data track the .cue names (or the folder's
only .bin/.img) is dumped there with dumpsxiso. Steps:
  1. CRASHBSH.DAT and the exe whose DAT index fits it (a demo disc carries other games)
  2. the engine module (the entry whose 5 header words are pointers), the slot
     overlays (the entries sharing one load address) and their load addresses
     (where the most jal targets land on function starts)
  3. sym_scan over the exe and the DAT: records, types, line numbers, labels
  4. leak_scan over the same files
  5. each function record, and each label on a function start, placed in the
     exe, the engine or a slot overlay (fitting the offset of the records' link
     from the shipped code) and matched to the retail function with the most
     similar code (immediates and jump targets masked); retail's functions come
     from build/<module>.map (make build)
Writes to DIR (default <disc>/): scan.md (the report), sym.txt (every record),
types.h, lines.txt, sections.txt, engine_<id>.bin and leaks/.
"""
import argparse
import bisect
import difflib
import json
import os
import re
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import leak_scan  # noqa: E402
import sym_scan  # noqa: E402

SECTOR = 2048
JR_RA = 0x03E00008
RAM = (0x80010000, 0x80200000)
PROJECT = ".decomp/project.json"
STRONG, LIKELY = 0.9, 0.75     # similarity for "same function" / "probably"
MIN_RUN = 10                   # shorter runs of records are chance byte patterns
# the linker's section symbols: _<group>[_<section>]_org/orgend/obj/objend/size
SECTION = re.compile(r"(_(text|data|sdata|rdata|bss|sbss|ctors|dtors))?_(obj|objend|org|orgend|size)$")


def die(msg):
    print(f"proto_scan: {msg}", file=sys.stderr)
    sys.exit(1)


def words(data):
    return struct.unpack_from("<%dI" % (len(data) // 4), data)


# --- the disc ---------------------------------------------------------------

def data_track(cue):
    """The file of the .cue's first data track, matched case-insensitively."""
    cur = None
    for line in open(cue, errors="replace"):
        m = re.match(r'\s*FILE\s+"([^"]+)"', line)
        if m:
            cur = m.group(1)
        elif cur and re.match(r"\s*TRACK\s+\d+\s+MODE", line):
            d = os.path.dirname(cue)
            for f in os.listdir(d):
                if f.lower() == os.path.basename(cur).lower():
                    return os.path.join(d, f)
    return None


def disc_files(disc):
    """The folder holding the disc's files, dumping the image first if needed."""
    root = os.path.join(disc, "files")
    if os.path.isdir(root):
        return root
    cues = [f for f in os.listdir(disc) if f.lower().endswith(".cue")]
    images = [f for f in os.listdir(disc) if f.lower().endswith((".bin", ".img"))]
    if cues:
        image = data_track(os.path.join(disc, cues[0]))
    else:   # no .cue: a lone image starts with its data track
        image = os.path.join(disc, images[0]) if len(images) == 1 else None
    if not image:
        die(f"{disc}: no files/ folder, and no .cue with a data track or single .bin/.img")
    print(f"dumping {image} -> {root}/")
    subprocess.run(["dumpsxiso", "-x", root, "-s", os.path.join(disc, "layout.xml"), image],
                   check=True, stdout=subprocess.DEVNULL)
    return root


def find_files(root):
    """(CRASHBSH.DAT path, [PS-X EXE paths]) under root."""
    dat, exes = None, []
    for d, _, names in os.walk(root):
        for n in sorted(names):
            p = os.path.join(d, n)
            if n.upper() == "CRASHBSH.DAT":
                dat = p
            else:
                with open(p, "rb") as f:
                    if f.read(8) == b"PS-X EXE":
                        exes.append(p)
    return dat, exes


def find_index(exe, dat_size):
    """The longest table of {u32 sector, u32 size} pairs in exe that walks forward
    through the DAT: (file offset, entries), or None."""
    nsec = dat_size // SECTOR
    best = None
    for off in range(0x800, len(exe) - 8, 4):
        n, end, p = 0, 0, off
        while p + 8 <= len(exe):
            sec, size = struct.unpack_from("<II", exe, p)
            if size == 0 or size > dat_size or sec < end or sec >= nsec:
                break
            end = sec + (size + SECTOR - 1) // SECTOR
            n += 1
            p += 8
        if n > 50 and (best is None or n > best[1]):
            best = (off, n)
    if best is None:
        return None
    off, n = best
    return off, [struct.unpack_from("<II", exe, off + 8 * i) for i in range(n)]


def find_engine(dat, entries):
    """The engine module: the biggest entry whose 5 header words are pointers."""
    found = None
    for i, (sec, size) in enumerate(entries):
        if size >= 0x1000 and all(RAM[0] <= w < RAM[1] for w in struct.unpack_from("<5I", dat, sec * SECTOR)):
            if found is None or size > found[2]:
                found = (i, sec, size)
    return found


def starts(w):
    """Word offsets that follow a jr ra and its delay slot: function starts."""
    return [i * 4 for i in range(2, len(w)) if w[i - 2] == JR_RA]


def find_base(code):
    """The load address that puts the most distinct jal targets on function starts."""
    w = words(code)
    targets = {((x & 0x3FFFFFF) << 2) | 0x80000000 for x in w if x >> 26 == 3}
    votes = {}
    for o in starts(w):
        for t in targets:
            b = t - o
            if RAM[0] <= b < RAM[1]:
                votes[b] = votes.get(b, 0) + 1
    if not votes:
        return None, 0
    b = max(votes, key=votes.get)
    return b, votes[b]


# --- code comparison ----------------------------------------------------------

def norm(x):
    """An instruction with what changes between builds masked: jump targets and
    16-bit immediates (offsets, constants, branch distances)."""
    op = x >> 26
    if op in (2, 3):
        return op << 26
    if op not in (0, 0x12):
        return x & 0xFFFF0000
    return x


class Module:
    """Code loaded at base: bytes, address range and function starts. kind is the
    retail code it is compared with: main, engine or slot. shift is added to a
    record's address to find its code: records can come from a link that placed
    the module a few bytes away."""

    def __init__(self, name, data, base, kind=None, first=()):
        self.name, self.data, self.base, self.kind, self.shift = name, data, base, kind or name, 0
        self.w = words(data)
        self.starts = set(starts(self.w)) | set(first)   # first: starts no jr ra precedes
        self.sorted = sorted(base + o for o in self.starts)

    def holds(self, a):
        return self.base <= a < self.base + len(self.data)

    def is_start(self, a):
        return self.holds(a) and (a - self.base) in self.starts

    def starts_near(self, a, span):
        st = self.sorted
        return st[bisect.bisect_left(st, a - span):bisect.bisect_right(st, a + span)]

    def body(self, a, end):
        return tuple(norm(x) for x in self.w[(a - self.base) // 4:(end - self.base) // 4])

    def end_of(self, a):
        """The end of the function at a: after its first jr ra and delay slot."""
        i = (a - self.base) // 4
        while i < len(self.w) and self.w[i] != JR_RA:
            i += 1
        return self.base + 4 * (i + 2)


def fit_shift(mod, addrs, span=0x10000):
    """(shift, votes): the offset that puts the most of addrs on the module's
    function starts, the smallest of equals (0 when they already fit)."""
    votes = {}
    for a in addrs:
        for s in mod.starts_near(a, span):
            votes[s - a] = votes.get(s - a, 0) + 1
    if not votes:
        return 0, 0
    best = max(votes, key=lambda s: (votes[s], -abs(s)))
    return best, votes[best]


def local_shifts(mod, sure, span=0x400, run=3):
    """[(first, last, shift)]: where the code changed size part way through a
    module, the sure function addresses after the change miss its starts at the
    module's shift. Each stretch of at least run of them in a row that one
    other shift puts on starts gets that shift."""
    miss = sorted(a for a in sure if mod.holds(a + mod.shift) and not mod.is_start(a + mod.shift))
    cand = [{s - a for s in mod.starts_near(a, span)} for a in miss]
    out, i = [], 0
    while i < len(miss):
        best, n = None, 0
        for s in cand[i]:
            j = i
            while j < len(miss) and s in cand[j]:
                j += 1
            if j - i > n or (j - i == n and abs(s - mod.shift) < abs(best - mod.shift)):
                best, n = s, j - i
        if n >= run:
            out.append((miss[i], miss[i + n - 1], best))
            i += n
        else:
            i += 1
    return out


def find_slots(dat, entries, skip, min_votes=10):
    """(base, [(id, code)]): the slot overlays, the DAT entries whose calls into
    themselves fit one load address, the one the most entries share."""
    bases = {}
    for i, (sec, size) in enumerate(entries):
        if i == skip or size < 0x1000:
            continue
        code = dat[sec * SECTOR:sec * SECTOR + size]
        b, n = find_base(code)
        if b is not None and n >= min_votes:
            bases.setdefault(b, []).append((i, code))
    if not bases:
        return None, []
    b = max(bases, key=lambda k: len(bases[k]))
    return (b, bases[b]) if len(bases[b]) >= 2 else (None, [])


def retail_funcs(name, mapfile, data):
    """[(address, name, body)] of the retail module's functions, from its link map.
    An object without symbols (a Sony library object) is cut at its function
    starts, each named <object>+<offset>."""
    sect = re.compile(r"^\.%s\s+0x([0-9a-f]+)\s+0x[0-9a-f]+\s+load address 0x([0-9a-f]+)" % name)
    inp = re.compile(r"^ \.(\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)")
    sym = re.compile(r"^\s+0x([0-9a-f]+)\s+([A-Za-z_]\w*)$")
    vma = load = None
    groups, cur = [], None
    for line in open(mapfile):
        if vma is None:
            m = sect.match(line)
            if m:
                vma, load = int(m.group(1), 16), int(m.group(2), 16)
            continue
        if line.startswith("."):
            break
        m = inp.match(line)
        if m:
            cur = None
            if m.group(1) == "text":
                a = int(m.group(2), 16)
                cur = (a, a + int(m.group(3), 16), {}, os.path.basename(m.group(4)))
                groups.append(cur)
            continue
        m = sym.match(line)
        if m and cur is not None:
            cur[2].setdefault(int(m.group(1), 16) & 0xFFFFFFFF, m.group(2))
    if vma is None:
        die(f"{mapfile}: no .{name} section")
    mod = Module(name, data[load:], vma)
    out = []
    for start, end, syms, obj in groups:
        if not syms and end > start:
            for a in [start] + sorted(vma + o for o in mod.starts if start < vma + o < end):
                syms[a] = f"{obj}+0x{a - start:X}"
        addrs = sorted(syms)
        for a, b in zip(addrs, addrs[1:] + [end]):
            out.append((a, syms[a], mod.body(a, b)))
    return out


def best_match(body, funcs, exact):
    """[(similarity, address, name)] of the retail functions most like body."""
    if body in exact:
        return [(1.0, a, n) for a, n in exact[body]]
    scored = []
    for a, n, rb in funcs:
        if not rb or not 0.5 <= len(rb) / len(body) <= 2:
            continue
        sm = difflib.SequenceMatcher(None, body, rb, autojunk=False)
        if sm.real_quick_ratio() < 0.5 or sm.quick_ratio() < 0.5:
            continue
        scored.append((sm.ratio(), a, n))
    return sorted(scored, reverse=True)


def place(funcs, mods, sure):
    """Find the code of each function record or label. funcs: [(address, end or
    None, record)] sorted by address; sure: addresses certainly functions (the
    records and the C++ labels), which fit each module's shifts first.
    Returns ([(record, [(module, code address, code end)])], notes); a label that
    lands nowhere has no candidates (data, or code in a module not found)."""
    notes, runs = [], {}
    slots = [m for m in mods if m.kind == "slot"]
    if slots:   # the overlays share one range and one link: fit them together
        lo, hi = slots[0].base, max(m.base + len(m.data) for m in slots)
        near = [a for a in sure if lo - 0x10000 <= a < hi + 0x10000]
        votes = {}
        for m in slots:
            for a in near:
                for s in m.starts_near(a, 0x10000):
                    votes[s - a] = votes.get(s - a, 0) + 1
        if votes:
            shift = max(votes, key=lambda s: (votes[s], -abs(s)))
            if votes[shift] >= 3:
                for m in slots:
                    m.shift = shift
                notes.append(f"- slot overlays: {votes[shift]} function labels near `0x{lo:08X}` on their "
                             f"function starts" + (f", with their addresses shifted by {shift:+#x}" if shift else ""))
    for mod in mods:
        if mod.kind == "slot":
            continue
        near = [a for a in sure if mod.base - 0x10000 <= a < mod.base + len(mod.data) + 0x10000]
        shift, n = fit_shift(mod, near)
        if near and n >= max(3, len(near) // 2):
            mod.shift = shift
            notes.append(f"- {mod.name}: {n} of {len(near)} nearby function records/labels on function starts"
                         + (f", with their addresses shifted by {shift:+#x}: the records come from a link "
                            "that placed this module elsewhere" if shift else ""))
        runs[mod.name] = local_shifts(mod, sure)
        for lo, hi, s in runs[mod.name]:
            notes.append(f"- {mod.name}: from `0x{lo:08X}` to `0x{hi:08X}` shifted by {s:+#x} "
                         "(the code changed size before there)")
    bounds = [a for a, _, _ in funcs]
    out = []
    for k, (a, end, r) in enumerate(funcs):
        nxt = bounds[k + 1] if k + 1 < len(bounds) and bounds[k + 1] > a else None
        cands = []
        for mod in mods:
            if mod.kind == "slot" and r[0] == "label" and "__F" not in r[2]:
                continue   # in the shared range a data label can sit on another overlay's code
            shifts = [s for lo, hi, s in runs.get(mod.name, []) if lo <= a <= hi] + [mod.shift]
            s = next((s for s in shifts if mod.is_start(a + s)), None)
            if s is None:
                continue
            e = end + s if end else mod.end_of(a + s)
            if not end and nxt and nxt + s < e:
                e = nxt + s
            cands.append((mod, a + s, e))
        out.append((r, cands))
    return out, notes


def overlay_of(name):
    """The retail overlay a slot function's name carries: 'func_X (ovl5f)' -> 'ovl5f'."""
    m = re.search(r" \((\w+)\)$", name)
    return m.group(1) if m else None


def match_funcs(placed, retail, prefer=None):
    """Match each placed record to retail: [(record, module, (similarity, address,
    name) or None)]. retail: {kind: [(address, name, body)]}. With several
    candidate modules (slot overlays share one range) the best match wins. Among
    equal retail candidates one in the module's preferred retail overlay
    (prefer: {module: overlay}) wins, then the first after the module's last
    match, so identical functions keep their order."""
    exact = {k: {} for k in retail}
    for k, fl in retail.items():
        for a, n, b in fl:
            exact[k].setdefault(b, []).append((a, n))
    out, prev = [], {}
    for r, cands in placed:
        best = (None, None)
        for mod, a, e in cands:
            body = mod.body(a, e)
            if mod.kind not in retail or not body:
                continue
            cs = best_match(body, retail[mod.kind], exact[mod.kind])
            if not cs:
                if best[0] is None:
                    best = (mod, None)
                continue
            top = [c for c in cs if c[0] >= cs[0][0] - 0.005]
            want = (prefer or {}).get(mod.name)
            top = [c for c in top if overlay_of(c[2]) == want] or top
            after = [c for c in top if c[1] > prev.get(mod.name, 0)]
            pick = min(after, key=lambda c: c[1]) if after else top[0]
            if best[1] is None or pick[0] > best[1][0]:
                best = (mod, pick)
        mod, pick = best
        if pick and pick[0] >= LIKELY:
            prev[mod.name] = pick[1]
        out.append((r, mod.name if mod else (cands[0][0].name if cands else None), pick))
    return out


# --- records --------------------------------------------------------------------

def func_spans(recs):
    """[(address, end or None, func record)]: the end from the function's fend record."""
    out = []
    for i, (_, r) in enumerate(recs):
        if r[0] != "func":
            continue
        end = None
        for _, s in recs[i + 1:]:
            if s[0] == "func":
                break
            if s[0] == "fend":
                end = s[1]
                break
        out.append((r[1], end, r))
    return sorted(set(out), key=lambda x: x[0])


def clusters(recs, gap=0x1000):
    """Group records into runs whose offsets are no more than gap apart: [[(offset, record)]]."""
    out = []
    for off, r in recs:
        if out and off - out[-1][-1][0] <= gap:
            out[-1].append((off, r))
        else:
            out.append([(off, r)])
    return out


def where(off, entries):
    for i, (sec, size) in enumerate(entries):
        if sec * SECTOR <= off < sec * SECTOR + size:
            return f"entry 0x{i:03X} +0x{off - sec * SECTOR:X}"
    return "filler"


def is_section(name):
    return name.startswith("_") and SECTION.search(name) is not None


def is_global(r):
    """A def of an extern or static variable (not a function) at an address."""
    return (r[0] == "def" and r[2] in (sym_scan.C_EXT, sym_scan.C_STAT)
            and RAM[0] <= r[1] < 0x80800000 and (r[3] >> 4) & 3 != 2)


# --- report ---------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("disc")
    ap.add_argument("--out", help="where to write (default: the disc folder)")
    args = ap.parse_args()
    out = args.out or args.disc
    os.makedirs(out, exist_ok=True)
    root = disc_files(args.disc)
    dat_path, exes = find_files(root)
    if not dat_path:
        die(f"{root}: no CRASHBSH.DAT")
    dat = open(dat_path, "rb").read()
    exe_path = exe = index = None
    for p in exes:
        data = open(p, "rb").read()
        idx = find_index(data, len(dat))
        if idx and (index is None or len(idx[1]) > len(index[1])):
            exe_path, exe, index = p, data, idx
    if not exe:
        die(f"no exe under {root} indexes {dat_path}")
    t_addr = struct.unpack_from("<I", exe, 0x18)[0]
    entries = index[1]
    rel = lambda p: os.path.relpath(p, args.disc).replace(os.sep, "/")  # noqa: E731
    rep = [f"# {os.path.basename(os.path.normpath(args.disc))}", "",
           "Written by `tools/scripts/proto_scan.py`.", "",
           f"- Exe: `{rel(exe_path)}`, load `0x{t_addr:08X}`, `0x{len(exe) - 0x800:X}` bytes",
           f"- DAT: `{rel(dat_path)}`, {len(dat)} bytes; index at exe `0x{t_addr + index[0] - 0x800:08X}`, "
           f"{len(entries)} entries"]
    demo = {"main": Module("main", exe[0x800:], t_addr, first=(0,))}
    eng = find_engine(dat, entries)
    slot_base = None
    if eng:
        i, sec, size = eng
        code = dat[sec * SECTOR:sec * SECTOR + size]
        base, votes = find_base(code)
        with open(os.path.join(out, f"engine_{i:03X}.bin"), "wb") as f:
            f.write(code)
        rep.append(f"- Engine: entry `0x{i:03X}` (sector `0x{sec:X}`, `0x{size:X}` bytes), "
                   f"load `0x{base:08X}` ({votes} jal targets on function starts); "
                   f"cut to `engine_{i:03X}.bin`")
        print("looking for the slot overlays...")
        slot_base, slots = find_slots(dat, entries, i)
        if slot_base:
            if base < slot_base < base + len(code):
                code = code[:slot_base - base]   # the engine's slot image is not engine code
            rep.append(f"- Slot overlays: {len(slots)} entries load at `0x{slot_base:08X}` "
                       f"({', '.join(f'`0x{j:03X}`' for j, _ in slots)})")
            for j, c in slots:
                demo[f"slot {j:03X}"] = Module(f"slot {j:03X}", c, slot_base, kind="slot")
        demo = {"engine": Module("engine", code, base, first=(0x14,)), **demo}

    def where_in(name, off):
        """Where a hit at off in file name is: the exe, a DAT entry or the filler."""
        if name != rel(dat_path):
            return "exe"
        at = where(off, entries)
        return at + " (the engine)" if eng and at.startswith(f"entry 0x{eng[0]:03X} ") else at

    print("scanning for debug records...")
    runs, noise = {}, 0
    for name, data in ((rel(exe_path), exe), (rel(dat_path), dat)):
        runs[name] = []
        for c in clusters(sym_scan.records(data)):
            if len(c) >= MIN_RUN:
                runs[name].append(c)
            else:
                noise += 1
    allrecs = [x for v in runs.values() for c in v for x in c]
    with open(os.path.join(out, "sym.txt"), "w") as f:
        for name, cs in runs.items():
            f.write(f"# {name}\n")
            for off, r in (x for c in cs for x in c):
                f.write("0x%08X %s\n" % (off, r))
    with open(os.path.join(out, "types.h"), "w") as f:
        f.write(sym_scan.to_c(allrecs) + "\n")
    table = sym_scan.lines(allrecs)
    with open(os.path.join(out, "lines.txt"), "w") as f:
        for addr, fn, n in table:
            f.write("0x%08X %s:%d\n" % (addr, fn, n))

    rep += ["", "## Debug records", ""]
    if noise:
        rep += [f"{noise} runs of fewer than {MIN_RUN} records left out as chance matches.", ""]
    if not allrecs:
        rep.append("None.")
    else:
        rep += ["| File | Offsets | Where | Records |", "|---|---|---|---|"]
        for name, cs in runs.items():
            for c in cs:
                kinds = {}
                for _, r in c:
                    kinds[r[0]] = kinds.get(r[0], 0) + 1
                lo, hi = c[0][0], c[-1][0]
                ks = ", ".join(f"{n} {k}" for k, n in sorted(kinds.items(), key=lambda x: -x[1]))
                rep.append(f"| `{name}` | `0x{lo:X}..0x{hi:X}` | {where_in(name, lo)} | {ks} |")
        types = sum(1 for _, r in allrecs if r[0] == "def"
                    and r[2] in (sym_scan.C_STRTAG, sym_scan.C_UNTAG, sym_scan.C_ENTAG, sym_scan.C_TPDEF))
        rep += ["", f"{types} struct, union, enum and typedef records (`types.h`); "
                f"{len(table)} line records (`lines.txt`); every record in `sym.txt`."]
        srcs = {}
        for _, r in allrecs:
            if r[0] == "func":
                srcs.setdefault(r[5], [0, 0])[0] += 1
        for addr, fn, _ in table:
            srcs.setdefault(fn, [0, 0])[1] += 1
        if srcs:
            rep += ["", "| Source file | Functions | Line records | Code (line table) |", "|---|---|---|---|"]
            for fn, (nf, nl) in sorted(srcs.items()):
                span = [t[0] for t in table if t[1] == fn]
                code = f"`0x{min(span):08X}..0x{max(span):08X}`" if span else ""
                rep.append(f"| `{fn}` | {nf} | {nl} | {code} |")

    sect = sorted({(r[1], r[2]) for _, r in allrecs if r[0] == "label" and is_section(r[2])})
    if sect:
        with open(os.path.join(out, "sections.txt"), "w") as f:
            for val, name in sect:
                f.write("0x%08X %s\n" % (val, name))
        groups = {}
        for val, name in sect:
            if RAM[0] <= val < 0x80800000:
                g = groups.setdefault(SECTION.sub("", name) or "(whole link)", [val, val])
                g[0], g[1] = min(g[0], val), max(g[1], val)
        rep += ["", "## Link groups", "",
                f"From {len(sect)} section symbols (`_<group>[_<section>]_org/orgend/obj/objend/size`, "
                "all in `sections.txt`): each group's lowest and highest address.", "",
                "| Group | From | To |", "|---|---|---|"]
        for g, (lo, hi) in sorted(groups.items(), key=lambda x: (x[1][0], x[0])):
            rep.append(f"| `{g}` | `0x{lo:08X}` | `0x{hi:08X}` |")

    labels = sorted({(r[1], r[2]) for _, r in allrecs if r[0] == "label"
                     and RAM[0] <= r[1] < 0x80800000 and not is_section(r[2])})
    funcs = func_spans(allrecs)
    if funcs or labels:
        print(f"matching {len(funcs)} function records and {len(labels)} labels to retail...")
        proj = json.load(open(PROJECT))
        rdat = proj["overlays"][0]["from"]
        rexe = os.path.join(os.path.dirname(os.path.dirname(rdat)), proj["exe"])
        retail = {"main": retail_funcs("main", "build/main.map", open(rexe, "rb").read()), "slot": []}
        with open(rdat, "rb") as f:
            for o in proj["overlays"]:
                f.seek(o["offset"])
                fl = retail_funcs(o["name"], f"build/{o['name']}.map", f.read(o["size"]))
                if o["name"] == "engine":
                    retail["engine"] = fl
                else:   # the slot overlays share one range: name which one
                    retail["slot"] += [(a, f"{n} ({o['name']})", b) for a, n, b in fl]
        # a label names a function when it lands on a function start (the function then
        # ends at the next symbol at the latest); any other label is data
        have = {f[0] for f in funcs}
        funcs += [(a, None, ("label", a, n)) for a, n in labels if a not in have]
        funcs.sort(key=lambda x: x[0])
        sure = [a for a, _, r in funcs if r[0] == "func" or "__F" in r[2]]
        placed, notes = place(funcs, list(demo.values()), sure)
        rep += ["", "## Functions", ""] + notes
        matched = match_funcs(placed, retail)
        # each demo overlay is one retail overlay: the one most of its strong matches name
        votes = {}
        for _, mod, pick in matched:
            if pick and pick[0] >= STRONG and overlay_of(pick[2]):
                votes.setdefault(mod, {}).setdefault(overlay_of(pick[2]), 0)
                votes[mod][overlay_of(pick[2])] += 1
        prefer = {m: max(v, key=v.get) for m, v in votes.items()}
        if prefer:
            matched = match_funcs(placed, retail, prefer)
            rep += ["", "Demo slot overlays and the retail overlay most of their strong matches are in: "
                    + ", ".join(f"{m} = `{o}`" for m, o in sorted(prefer.items())) + "."]
        rep += ["", f"Similarity of the masked code: {STRONG} and up is the same function, "
                f"{LIKELY} and up probably; below that the best guess is in brackets. "
                "Retail library functions are named `<object>+<offset>`, slot overlay ones "
                "`<name> (<overlay>)`.", "",
                "| Record | Source line | Demo | Module | Retail | Similarity |", "|---|---|---|---|---|---|"]
        data_labels = []
        for r, mod, pick in matched:
            if r[0] == "label" and mod is None:
                data_labels.append((r[1], r[2]))
                continue
            src = f"{r[5].split(chr(92))[-1]}:{r[4]}" if r[0] == "func" else "label"
            if pick is None:
                rep.append(f"| `{r[2]}` | {src} | `0x{r[1]:08X}` | {mod or 'unplaced'} | | |")
                continue
            s, ra, rn = pick
            name = f"`{rn}`" if s >= LIKELY else f"(`{rn}`)"
            rep.append(f"| `{r[2]}` | {src} | `0x{r[1]:08X}` | {mod} | {name} | {s:.2f} |")
        if data_labels:
            rep += ["", "## Data labels", "", "Labels not on a function start: globals, or code "
                    "in a module not found.", "",
                    "| Name | Demo address | In |", "|---|---|---|"]
            for a, n in data_labels:
                mod = next((m.name for m in demo.values() if m.kind != "slot" and m.holds(a + m.shift)),
                           "slot" if slot_base and a >= slot_base else "")
                rep.append(f"| `{n}` | `0x{a:08X}` | {mod} |")

    glob = sorted({r[1:5] + (tuple(r[5]),) + r[6:] for _, r in allrecs if is_global(r)})
    if glob:
        rep += ["", "## Globals", "", "| Name | Demo address | Size | Type |", "|---|---|---|---|"]
        for val, cls, typ, size, dims, tag, name in glob:
            decl = sym_scan.decl(name, typ, tag, dims)
            rep.append(f"| `{name}` | `0x{val:08X}` | 0x{size:X} | `{decl}` |")

    print("scanning for source text...")
    leaks = os.path.join(out, "leaks")
    found = []
    for name, data in ((rel(exe_path), exe), (rel(dat_path), dat)):
        for off, run, kw in leak_scan.hits(data, 256):
            os.makedirs(leaks, exist_ok=True)
            fn = f"{os.path.basename(name).replace('.', '_')}_{off:08X}.txt"
            with open(os.path.join(leaks, fn), "wb") as f:
                f.write(run)
            found.append(f"| `{name}` | `0x{off:X}` | {where_in(name, off)} | {len(run)} | `leaks/{fn}` |")
    rep += ["", "## Source text", ""]
    rep += ["| File | Offset | Where | Bytes | Saved as |", "|---|---|---|---|---|"] + found if found else ["None."]

    with open(os.path.join(out, "scan.md"), "w") as f:
        f.write("\n".join(rep) + "\n")
    print(f"{len(allrecs)} records, {len(funcs)} functions, {len(found)} text leaks -> {out}/scan.md")


if __name__ == "__main__":
    main()
