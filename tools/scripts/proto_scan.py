#!/usr/bin/env python3
"""Scan a prototype or demo disc for Crash Bash debug leftovers and map them to retail.

The build tools padded files with whatever was in the build machine's memory: the
retail engine holds source text, and the Aug 23 and Oct 9 prototype DATs and the
Jampack Winter 2000 demo's engine hold pieces of the SN linker's .SYM output
(docs/PROTOTYPES.md). This runs every check on one disc and writes what it finds
next to the disc:

    tools/dk python3 -I tools/scripts/proto_scan.py protos/<disc>/ [--out DIR]

<disc>/ holds the disc image (extract an archive on the host with 7-Zip first) or
its files: with no files/ folder, the data track the .cue names is dumped there
with dumpsxiso. Steps:
  1. CRASHBSH.DAT and the exe whose DAT index fits it (a demo disc carries other games)
  2. the engine module (the entry whose 5 header words are pointers) and its load
     address (where the most jal targets land on function starts)
  3. sym_scan over the exe and the DAT: records, types, line numbers
  4. leak_scan over the same files
  5. each function record placed in the exe or the engine and matched to the
     retail function with the most similar code (immediates and jump targets
     masked); retail's functions come from build/<module>.map (make build)
Writes to DIR (default <disc>/): scan.md (the report), sym.txt (every record),
types.h, lines.txt, engine_<id>.bin and leaks/. Records in a slot overlay show as
unplaced: the slot overlays aren't found yet.
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
    image = data_track(os.path.join(disc, cues[0])) if cues else None
    if not image:
        die(f"{disc}: no files/ folder and no .cue with a data track")
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
    """Code loaded at base: bytes, address range and function starts. shift is
    added to a record's address to find its code (records can come from a link
    that placed the module a few bytes away)."""

    def __init__(self, name, data, base):
        self.name, self.data, self.base, self.shift = name, data, base, 0
        self.w = words(data)
        self.starts = set(starts(self.w))

    def holds(self, a):
        return self.base <= a < self.base + len(self.data)

    def is_start(self, a):
        a += self.shift
        return self.holds(a) and (a - self.base) in self.starts

    def body(self, a, end):
        lo, hi = (a + self.shift - self.base) // 4, (end + self.shift - self.base) // 4
        return tuple(norm(x) for x in self.w[lo:hi])

    def end_of(self, a):
        """The end (in record addresses) of the function at a: after its first
        jr ra and delay slot."""
        i = (a + self.shift - self.base) // 4
        while i < len(self.w) and self.w[i] != JR_RA:
            i += 1
        return self.base + 4 * (i + 2) - self.shift


def fit_shift(mod, addrs, span=0x10000):
    """(shift, votes): the offset that puts the most of addrs on the module's
    function starts, the smallest of equals (0 when they already fit)."""
    st = sorted(mod.base + o for o in mod.starts)
    votes = {}
    for a in addrs:
        for s in st[bisect.bisect_left(st, a - span):bisect.bisect_right(st, a + span)]:
            votes[s - a] = votes.get(s - a, 0) + 1
    if not votes:
        return 0, 0
    best = max(votes, key=lambda s: (votes[s], -abs(s)))
    return best, votes[best]


def retail_funcs(name, mapfile, data):
    """[(address, name, body)] of the retail module's functions, from its link map."""
    sect = re.compile(r"^\.%s\s+0x([0-9a-f]+)\s+0x[0-9a-f]+\s+load address 0x([0-9a-f]+)" % name)
    inp = re.compile(r"^ \.(\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s")
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
                cur = (a + int(m.group(3), 16), {})
                groups.append(cur)
            continue
        m = sym.match(line)
        if m and cur is not None:
            cur[1].setdefault(int(m.group(1), 16) & 0xFFFFFFFF, m.group(2))
    if vma is None:
        die(f"{mapfile}: no .{name} section")
    mod = Module(name, data[load:], vma)
    out = []
    for end, syms in groups:
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


def match_funcs(funcs, demo, retail):
    """Match each function record to retail. funcs: [(address, end, record)] sorted
    by address; demo/retail: {module: Module / [(address, name, body)]}.
    Among equal candidates the first retail function after the last match wins,
    so identical functions keep their order."""
    exact = {m: {} for m in retail}
    for m, fl in retail.items():
        for a, n, b in fl:
            exact[m].setdefault(b, []).append((a, n))
    out, prev = [], {}
    for a, end, r in funcs:
        mod = next((m for m in demo.values() if m.is_start(a)), None)
        if mod is None or mod.name not in retail:
            out.append((r, None, None, []))
            continue
        body = mod.body(a, end or mod.end_of(a))
        cands = best_match(body, retail[mod.name], exact[mod.name]) if body else []
        pick = None
        if cands:
            top = [c for c in cands if c[0] >= cands[0][0] - 0.005]
            after = [c for c in top if c[1] > prev.get(mod.name, 0)]
            pick = min(after, key=lambda c: c[1]) if after else top[0]
            if pick[0] >= LIKELY:
                prev[mod.name] = pick[1]
        out.append((r, mod.name, pick, cands[:3]))
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


def is_global(r):
    """A def of an extern or static variable (not a function) at an address."""
    return (r[0] == "def" and r[2] in (sym_scan.C_EXT, sym_scan.C_STAT)
            and RAM[0] <= r[1] < 0x80800000 and (r[3] >> 4) & 3 != 2)


# --- report ---------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("disc")
    ap.add_argument("--out", help="where to write (default: the disc folder)")
    a = ap.parse_args()
    out = a.out or a.disc
    os.makedirs(out, exist_ok=True)
    root = disc_files(a.disc)
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
    rel = lambda p: os.path.relpath(p, a.disc).replace(os.sep, "/")  # noqa: E731
    rep = [f"# {os.path.basename(os.path.normpath(a.disc))}", "",
           "Written by `tools/scripts/proto_scan.py`.", "",
           f"- Exe: `{rel(exe_path)}`, load `0x{t_addr:08X}`, `0x{len(exe) - 0x800:X}` bytes",
           f"- DAT: `{rel(dat_path)}`, {len(dat)} bytes; index at exe `0x{t_addr + index[0] - 0x800:08X}`, "
           f"{len(entries)} entries"]
    demo = {"main": Module("main", exe[0x800:], t_addr)}
    eng = find_engine(dat, entries)
    if eng:
        i, sec, size = eng
        code = dat[sec * SECTOR:sec * SECTOR + size]
        base, votes = find_base(code)
        with open(os.path.join(out, f"engine_{i:03X}.bin"), "wb") as f:
            f.write(code)
        demo = {"engine": Module("engine", code, base), **demo}
        rep.append(f"- Engine: entry `0x{i:03X}` (sector `0x{sec:X}`, `0x{size:X}` bytes), "
                   f"load `0x{base:08X}` ({votes} jal targets on function starts); "
                   f"cut to `engine_{i:03X}.bin`")

    def place(name, off):
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
                rep.append(f"| `{name}` | `0x{lo:X}..0x{hi:X}` | {place(name, lo)} | {ks} |")
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

    funcs = func_spans(allrecs)
    if funcs:
        print(f"matching {len(funcs)} functions to retail...")
        proj = json.load(open(PROJECT))
        rdat = proj["overlays"][0]["from"]
        rexe = os.path.join(os.path.dirname(os.path.dirname(rdat)), proj["exe"])
        retail = {"main": retail_funcs("main", "build/main.map", open(rexe, "rb").read())}
        o = next(o for o in proj["overlays"] if o["name"] == "engine")
        with open(rdat, "rb") as f:
            f.seek(o["offset"])
            retail["engine"] = retail_funcs("engine", "build/engine.map", f.read(o["size"]))
        rep += ["", "## Functions", ""]
        for mod in demo.values():
            near = [f[0] for f in funcs if mod.base - 0x10000 <= f[0] < mod.base + len(mod.data) + 0x10000]
            shift, n = fit_shift(mod, near)
            if near and n >= max(3, len(near) // 2):
                mod.shift = shift
                rep.append(f"- {mod.name}: {n} of {len(near)} nearby records on function starts"
                           + (f" with their addresses shifted by {shift:+#x}: the records come from "
                              "a link that placed this module elsewhere" if shift else ""))
        rep += ["", f"Similarity of the masked code: {STRONG} and up is the same function, "
                f"{LIKELY} and up probably; below that the best guess is in brackets.", "",
                "| Record | Source line | Demo | Module | Retail | Similarity |", "|---|---|---|---|---|---|"]
        for r, mod, pick, cands in match_funcs(funcs, demo, retail):
            fn = r[5].split("\\")[-1]
            if pick is None:
                rep.append(f"| `{r[2]}` | {fn}:{r[4]} | `0x{r[1]:08X}` | {mod or 'unplaced'} | | |")
                continue
            s, ra, rn = pick
            name = f"`{rn}`" if s >= LIKELY else f"(`{rn}`)"
            rep.append(f"| `{r[2]}` | {fn}:{r[4]} | `0x{r[1]:08X}` | {mod} | {name} | {s:.2f} |")

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
            found.append(f"| `{name}` | `0x{off:X}` | {place(name, off)} | {len(run)} | `leaks/{fn}` |")
    rep += ["", "## Source text", ""]
    rep += ["| File | Offset | Where | Bytes | Saved as |", "|---|---|---|---|---|"] + found if found else ["None."]

    with open(os.path.join(out, "scan.md"), "w") as f:
        f.write("\n".join(rep) + "\n")
    print(f"{len(allrecs)} records, {len(funcs)} functions, {len(found)} text leaks -> {out}/scan.md")


if __name__ == "__main__":
    main()
