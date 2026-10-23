"""Find a retail function's copies in the prototype and demo discs (decomp.py protos).

Every disc proto_scan.py has scanned (protos/<disc>/scan.md) gives its exe, the engine it cut
out (engine_<id>.bin) and its slot overlays (the minigames: DAT entries, read here through
the exe's DAT index), each with its load address. Each image is split into functions (a
`jr ra` and its delay slot end one, zero padding is skipped) and ranked against the retail
function by a difflib ratio over opcode tokens (no registers or immediates): a histogram
prefilter keeps the closest by opcode counts, then the ratio orders them. A slot function is
looked for in every slot entry of the disc, since the entries' order differs between builds.
PROTOTYPES.md has the discs and their builds."""
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
