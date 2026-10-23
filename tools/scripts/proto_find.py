"""Find a retail function's copies in the prototype and demo discs (decomp.py protos).

Every disc proto_scan.py has scanned (protos/<disc>/scan.md) gives its exe and the engine it
cut out (engine_<id>.bin) with their load addresses. Each binary is split into functions (a
`jr ra` and its delay slot end one, zero padding is skipped) and ranked against the retail
function by a difflib ratio over opcode tokens (no registers or immediates): a histogram
prefilter keeps the closest by opcode counts, then the ratio orders them. Slot overlays (the
minigames) are inside each disc's DAT and are not cut, so only exe and engine functions are
looked up. PROTOTYPES.md has the discs and their builds."""
import collections
import difflib
import glob
import os
import re
import struct

EXE = re.compile(r"^- Exe: `([^`]+)`, load `0x([0-9A-Fa-f]+)`", re.M)
ENGINE = re.compile(r"^- Engine: .*load `0x([0-9A-Fa-f]+)`.*cut to `([^`]+)`", re.M)


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


def rank(target, ws, base, top=3, keep=12):
    """[(ratio, address, bytes)] of the TOP functions in WS (loaded at BASE) closest to TARGET."""
    tt = [token(w) for w in target]
    th = collections.Counter(tt)
    cands = []
    for s, e in functions(ws):
        n = e - s
        if n < len(tt) // 3 or n > len(tt) * 3:
            continue
        toks = [token(w) for w in ws[s:e]]
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
    """[(disc dir, {"main": (path, load, header), "engine": (path, load, 0)})] for every scanned disc."""
    out = []
    for scan in sorted(glob.glob(os.path.join(root, "*", "scan.md"))):
        d = os.path.dirname(scan)
        text = open(scan, encoding="utf-8").read()
        bins = {}
        m = EXE.search(text)
        if m:
            bins["main"] = (os.path.join(d, m.group(1)), int(m.group(2), 16), 0x800)
        m = ENGINE.search(text)
        if m:
            bins["engine"] = (os.path.join(d, m.group(2)), int(m.group(1), 16), 0)
        out.append((d, bins))
    return out


def report(target, kind, root="protos", top=1):
    """One line per disc (TOP candidates each) for a retail function of KIND (main or engine)."""
    lines = []
    for d, bins in discs(root):
        if kind not in bins or not os.path.exists(bins[kind][0]):
            lines.append(f"{d}: no {kind} binary")
            continue
        path, load, header = bins[kind]
        data = open(path, "rb").read()[header:]
        best = rank(target, words(data), load, top)
        if not best:
            lines.append(f"{d}: nothing of a similar size")
            continue
        for k, (r, addr, size) in enumerate(best):
            head = f"{d} ({os.path.basename(path)}):" if k == 0 else " " * (len(d) + len(os.path.basename(path)) + 4)
            lines.append(f"{head} {r:.3f} at 0x{addr:08X}, {size} B (file 0x{addr - load + header:X})")
    return "\n".join(lines)
