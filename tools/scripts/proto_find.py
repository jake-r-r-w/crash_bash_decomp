"""Find a retail function's copies in the prototype and demo discs (decomp.py protos).

Every disc proto_scan.py has scanned (protos/<disc>/scan.md) gives its exe, the engine it cut
out (engine_<id>.bin) and its slot overlays (the minigames: DAT entries, read here through
the exe's DAT index), each with its load address. Each image is split into functions (a
`jr ra` and its delay slot end one, zero padding is skipped) and ranked against the retail
function by a difflib ratio over opcode tokens (no registers or immediates): a histogram
prefilter keeps the closest by opcode counts, then the ratio orders them. A slot function is
looked for in every slot entry of the disc, since the entries' order differs between builds.
PROTOTYPES.md has the discs and their builds.

`protos NAME --diff DISC` (copy_in, diff_lines) takes the closest copy on one disc and aligns
it with retail by mnemonic and registers (decode/key), leaving out the immediates: relocated
%hi/%lo, branch offsets and shift counts change with every rebuild, so only real code changes
remain as hunks."""
import collections
import difflib
import glob
import os
import re
import struct

SECTOR = 2048
EXE = re.compile(r"^- Exe: `([^`]+)`, load `0x([0-9A-Fa-f]+)`", re.M)
ENGINE = re.compile(r"^- Engine: .*load `0x([0-9A-Fa-f]+)`.*cut to `([^`]+)`", re.M)
DAT = re.compile(r"^- DAT: `([^`]+)`, \d+ bytes; index at exe `0x([0-9A-Fa-f]+)`, (\d+) entries", re.M)
SLOTS = re.compile(r"^- Slot overlays: \d+ entries load at `0x([0-9A-Fa-f]+)` \(([^)]*)\)", re.M)


def token(w):
    op = w >> 26
    if op == 0:
        return w & 0x3F
    if op == 1:
        return 0x40 | ((w >> 16) & 0x1F)
    if op in (0x10, 0x12):
        return 0x80 | (op << 5) | ((w >> 21) & 0x1F)
    return 0x100 | op


def words(data):
    return list(struct.unpack(f"<{len(data) // 4}I", data[:len(data) // 4 * 4]))


def functions(ws):
    """(start, end) word ranges: each ends after a `jr ra` and its delay slot."""
    out, start, i = [], 0, 0
    while i < len(ws):
        if ws[i] == 0x03E00008:
            end = min(i + 2, len(ws))
            out.append((start, end))
            i = end
            while i < len(ws) and ws[i] == 0:
                i += 1
            start = i
            continue
        i += 1
    return out


def unpadded(ws):
    """WS without its trailing zero words (a nop delay slot, alignment)."""
    n = len(ws)
    while n and ws[n - 1] == 0:
        n -= 1
    return ws[:n]


def rank(target, ws, base, top=3, keep=12):
    """[(ratio, address, bytes)] of the TOP functions in WS (loaded at BASE) closest to TARGET.
    Both sides are compared without trailing zero words, so a copy scores 1.000 whether or not
    the retail size includes its delay slot."""
    tt = [token(w) for w in unpadded(target)]
    th = collections.Counter(tt)
    cands = []
    for s, e in functions(ws):
        n = e - s
        if n < len(tt) // 3 or n > len(tt) * 3:
            continue
        toks = [token(w) for w in unpadded(ws[s:e])]
        h = collections.Counter(toks)
        pre = sum((th & h).values()) / max(len(tt), n)
        cands.append((pre, s, e, toks))
    cands.sort(key=lambda c: -c[0])
    best = []
    for pre, s, e, toks in cands[:keep]:
        r = difflib.SequenceMatcher(None, tt, toks, autojunk=False).ratio()
        best.append((r, base + s * 4, (e - s) * 4))
    best.sort(key=lambda b: -b[0])
    return best[:top]


def discs(root="protos"):
    """[(disc dir, info)] for every scanned disc: info has "main" (path, load), "engine"
    (path, load) and "slot" (dat, exe, exe load, index address, entries, base, [entry ids])."""
    out = []
    for scan in sorted(glob.glob(os.path.join(root, "*", "scan.md"))):
        d = os.path.dirname(scan)
        text = open(scan, encoding="utf-8").read()
        info = {}
        exe = EXE.search(text)
        if exe:
            info["main"] = (os.path.join(d, exe.group(1)), int(exe.group(2), 16))
        m = ENGINE.search(text)
        if m:
            info["engine"] = (os.path.join(d, m.group(2)), int(m.group(1), 16))
        dat, slots = DAT.search(text), SLOTS.search(text)
        if exe and dat and slots:
            ids = [int(x, 16) for x in re.findall(r"0x([0-9A-Fa-f]+)", slots.group(2))]
            info["slot"] = (os.path.join(d, dat.group(1)), info["main"][0], info["main"][1],
                            int(dat.group(2), 16), int(dat.group(3)), int(slots.group(1), 16), ids)
        out.append((d, info))
    return out


def images(info, kind):
    """[(label, data, load address, where offset 0 is in its file)] for one disc's KIND."""
    if kind not in info:
        return []
    if kind == "main":
        path, load = info["main"]
        return [(os.path.basename(path), open(path, "rb").read()[0x800:], load, 0x800)] \
            if os.path.exists(path) else []
    if kind == "engine":
        path, load = info["engine"]
        return [(os.path.basename(path), open(path, "rb").read(), load, 0)] if os.path.exists(path) else []
    dat, exe, load, index, n, base, ids = info["slot"]
    if not (os.path.exists(dat) and os.path.exists(exe)):
        return []
    with open(exe, "rb") as f:
        f.seek(0x800 + index - load)
        table = f.read(8 * n)
    out = []
    with open(dat, "rb") as f:
        for i in ids:
            sec, size = struct.unpack_from("<II", table, 8 * i)
            f.seek(sec * SECTOR)
            out.append((f"entry 0x{i:X}", f.read(size), base, 0))
    return out


REGS = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp",
        "fp", "ra"]
SPECIAL = {0x00: "sll", 0x02: "srl", 0x03: "sra", 0x04: "sllv", 0x06: "srlv", 0x07: "srav",
           0x08: "jr", 0x09: "jalr", 0x0C: "syscall", 0x0D: "break", 0x10: "mfhi", 0x11: "mthi",
           0x12: "mflo", 0x13: "mtlo", 0x18: "mult", 0x19: "multu", 0x1A: "div", 0x1B: "divu",
           0x20: "add", 0x21: "addu", 0x22: "sub", 0x23: "subu", 0x24: "and", 0x25: "or",
           0x26: "xor", 0x27: "nor", 0x2A: "slt", 0x2B: "sltu"}
OPS = {0x02: "j", 0x03: "jal", 0x04: "beq", 0x05: "bne", 0x06: "blez", 0x07: "bgtz", 0x08: "addi",
       0x09: "addiu", 0x0A: "slti", 0x0B: "sltiu", 0x0C: "andi", 0x0D: "ori", 0x0E: "xori",
       0x0F: "lui", 0x20: "lb", 0x21: "lh", 0x22: "lwl", 0x23: "lw", 0x24: "lbu", 0x25: "lhu",
       0x26: "lwr", 0x28: "sb", 0x29: "sh", 0x2A: "swl", 0x2B: "sw", 0x2E: "swr", 0x32: "lwc2",
       0x3A: "swc2"}
REGIMM = {0x00: "bltz", 0x01: "bgez", 0x10: "bltzal", 0x11: "bgezal"}


def decode(w):
    """(mnemonic, [register operands], immediate or None) for one MIPS I word: enough to compare
    two compiles by opcode and register with the immediates (relocated %lo/%hi, branch offsets,
    shift counts) set aside."""
    op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    imm = w & 0xFFFF
    simm = imm - 0x10000 if imm & 0x8000 else imm
    if w == 0:
        return "nop", [], None
    if op == 0:
        f = w & 0x3F
        m = SPECIAL.get(f, f"special.{f:#x}")
        if f in (0x00, 0x02, 0x03):
            return m, [REGS[rd], REGS[rt]], (w >> 6) & 31
        if f in (0x04, 0x06, 0x07):
            return m, [REGS[rd], REGS[rt], REGS[rs]], None
        if f == 0x08:
            return m, [REGS[rs]], None
        if f == 0x09:
            return m, [REGS[rd], REGS[rs]], None
        if f in (0x10, 0x12):
            return m, [REGS[rd]], None
        if f in (0x11, 0x13):
            return m, [REGS[rs]], None
        if f in (0x18, 0x19, 0x1A, 0x1B):
            return m, [REGS[rs], REGS[rt]], None
        if f in (0x0C, 0x0D):
            return m, [], None
        return m, [REGS[rd], REGS[rs], REGS[rt]], None
    if op == 1:
        return REGIMM.get(rt, f"regimm.{rt:#x}"), [REGS[rs]], simm
    if op in (0x02, 0x03):
        return OPS[op], [], (w & 0x3FFFFFF) << 2
    if op in (0x10, 0x12):
        return f"cop{op >> 1 & 3}.{rs:#x}", [REGS[rt], f"${rd}"], w & 0x7FF
    m = OPS.get(op, f"op.{op:#x}")
    if op in (0x04, 0x05):
        return m, [REGS[rs], REGS[rt]], simm
    if op in (0x06, 0x07):
        return m, [REGS[rs]], simm
    if op == 0x0F:
        return m, [REGS[rt]], imm
    if op >= 0x20:
        return m, [REGS[rt], f"({REGS[rs]})"], simm
    return m, [REGS[rt], REGS[rs]], simm


def key(w):
    """The alignment key: mnemonic and registers, no immediate."""
    m, regs, _ = decode(w)
    return m + " " + ",".join(regs)


def fmt_word(w):
    m, regs, imm = decode(w)
    ops = [r for r in regs if not r.startswith("(")]
    base = next((r for r in regs if r.startswith("(")), None)
    if imm is None:
        return f"{m} {','.join(ops)}".strip()
    if base:
        return f"{m} {','.join(ops)},{imm:#x}{base}"
    return f"{m} {','.join(ops + [hex(imm)])}" if ops else f"{m} {imm:#x}"


def align(a, b):
    """difflib opcodes [(tag, i1, i2, j1, j2)] aligning word lists A and B by key()."""
    sm = difflib.SequenceMatcher(None, [key(w) for w in a], [key(w) for w in b], autojunk=False)
    return sm.get_opcodes(), sm.ratio()


def diff_lines(a, b, a_name="retail", b_name="copy", context=2):
    """Hunks where B differs from A by opcode or register (immediates ignored), offsets in bytes
    from each function's start, with CONTEXT lines of A around each hunk."""
    ops, ratio = align(a, b)
    out = []
    for tag, i1, i2, j1, j2 in ops:
        if tag == "equal":
            continue
        out.append(f"--- {tag} {a_name} 0x{i1 * 4:04x}-0x{i2 * 4:04x} ({i2 - i1})  "
                   f"{b_name} 0x{j1 * 4:04x}-0x{j2 * 4:04x} ({j2 - j1})")
        out += [f"      {k * 4:5x}  {fmt_word(a[k])}" for k in range(max(i1 - context, 0), i1)]
        out += [f"  -   {k * 4:5x}  {fmt_word(a[k])}" for k in range(i1, i2)]
        out += [f"  +   {k * 4:5x}  {fmt_word(b[k])}" for k in range(j1, j2)]
        out += [f"      {k * 4:5x}  {fmt_word(a[k])}" for k in range(i2, min(i2 + context, len(a)))]
    hunks = sum(1 for t, *_ in ops if t != "equal")
    out.append(f"{hunks} hunk(s); {len(a)} vs {len(b)} words; similarity {ratio:.3f} by opcode and register")
    return out


def copy_in(target, kind, disc, root="protos"):
    """(disc dir, words, address) of TARGET's closest function in the one scanned disc whose
    directory name contains DISC, or None."""
    for d, info in discs(root):
        if disc not in os.path.basename(d):
            continue
        best = None
        for label, data, load, ofs in images(info, kind):
            ws = words(data)
            for r, addr, size in rank(target, ws, load, 1):
                if best is None or r > best[0]:
                    s = (addr - load) // 4
                    best = (r, ws[s:s + size // 4], addr)
        return (d, best[1], best[2]) if best else (d, None, None)
    return None


def report(target, kind, root="protos", top=1):
    """One line per disc (TOP candidates each) for a retail function of KIND (main, engine or
    slot)."""
    lines = []
    for d, info in discs(root):
        imgs = images(info, kind)
        best = []
        for label, data, load, ofs in imgs:
            best += [(r, addr, size, label, addr - load + ofs) for r, addr, size in rank(target, words(data), load, top)]
        if not best:
            lines.append(f"{d}: no {kind} code" if not imgs else f"{d}: nothing of a similar size")
            continue
        best.sort(key=lambda b: -b[0])
        for k, (r, addr, size, label, ofs) in enumerate(best[:top]):
            head = f"{d}:" if k == 0 else " " * (len(d) + 1)
            lines.append(f"{head} {r:.3f} at 0x{addr:08X}, {size} B ({label} +0x{ofs:X})")
    return "\n".join(lines)
