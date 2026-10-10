"""Name PsyQ library functions in a PS-X EXE using lab313ru/psx_psyq_signatures.

usage (in the container, after `decomp.py index`):
  python3 tools/scripts/psyq_match.py ext/psx_psyq_signatures disc/<EXE> [--lo ADDR] [--hi ADDR]
                              [--version V] [--prefer LIBCD,LIBGPU] [--write]

Scans [lo, hi) (default: the whole .text) for every OBJ signature of every PsyQ
version, picks the version covering the most bytes, and prints:
  - the version ranking and the lowest matched address (the suggested library
    segment start; check it with the call-direction test in references/setup.md),
  - symbol_addrs.txt lines for the best version: unique matches only, generic
    loc_/text_ labels dropped, names that aren't an indexed function start
    written as type:label (secondary entry points in hand-written asm), and
    addresses claimed by several OBJs resolved by --prefer or left unnamed.
--write appends the block to configs/USA/sym.main.txt (re-running replaces it) and writes
configs/USA/psyq/objects.txt: one line per matched OBJ (address, .text size, LIB/OBJ), the
object layout `decomp.py split-lib` cuts the library segment by.
The signature repo has no licence: keep it in gitignored ext/, commit only names.
"""
import argparse, json, os, re, struct, sys
from collections import defaultdict

GENERIC = re.compile(r"^(loc|text|stup|data|bss|rdata|sdata|sbss)_?[0-9A-Fa-f]*$")
BEGIN, END = "// --- PsyQ signatures (tools/scripts/psyq_match.py) ---", "// --- end PsyQ signatures ---"


def load_exe(path):
    d = open(path, "rb").read()
    t_addr, t_size = struct.unpack_from("<II", d, 0x18)
    return d[0x800:0x800 + t_size], t_addr


def sig_regex(sig):
    toks = sig.split()
    while toks and toks[-1] == "00":    # trailing zeros are OBJ section padding
        toks.pop()
    fixed = sum(t != "??" for t in toks)
    pat = b"".join(b"." if t == "??" else re.escape(bytes([int(t, 16)])) for t in toks)
    return re.compile(pat, re.S), len(toks), fixed


def scan(vdir, region, lo):
    hits = []
    for fn in sorted(os.listdir(vdir)):
        if not fn.endswith(".json"):
            continue
        try:
            objs = json.load(open(os.path.join(vdir, fn)))
        except Exception:
            continue
        for o in objs:
            rx, n, fixed = sig_regex(o.get("sig", ""))
            # too short to be unambiguous - except fully fixed ones such as the BIOS call stubs
            # (li t2,0xB0; jr t2; li t1,N), which differ only in the call number
            if fixed < 24 and not (fixed == n and n >= 12):
                continue
            ms = [m.start() for m in rx.finditer(region) if m.start() % 4 == 0]
            if len(ms) == 1:
                hits.append((lo + ms[0], n, fn[:-5], o["name"], o.get("labels", [])))
    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("sigs"); ap.add_argument("exe")
    ap.add_argument("--lo"); ap.add_argument("--hi"); ap.add_argument("--version")
    ap.add_argument("--prefer", default="", help="comma list of LIBs that win address clashes")
    ap.add_argument("--write", action="store_true")
    a = ap.parse_args()
    text, base = load_exe(a.exe)
    lo = int(a.lo, 16) if a.lo else base
    hi = int(a.hi, 16) if a.hi else base + len(text)
    region = text[lo - base:hi - base]
    vers = [a.version] if a.version else sorted((v for v in os.listdir(a.sigs) if v.isdigit()), key=int)
    res = {}
    for v in vers:
        res[v] = scan(os.path.join(a.sigs, v), region, lo)
        print(f"// version {v}: {len(res[v])} objs, {sum(h[1] for h in res[v])} bytes", file=sys.stderr)
    best = max(res, key=lambda v: sum(h[1] for h in res[v]))
    hits = sorted(res[best])
    if not hits:
        sys.exit("no signature matched")
    starts = set()
    if os.path.exists(".decomp/functions.json"):
        starts = {int(k, 16) for k in json.load(open(".decomp/functions.json")) if ":" not in k}
    by_addr = defaultdict(list)
    for addr, n, lib, obj, labels in hits:
        for l in labels:
            if not GENERIC.match(l["name"]):
                by_addr[addr + l["offset"]].append((l["name"], f"{lib}/{obj}"))
    # An object whose first label is generic (data words before the entry point, as in the
    # BIOS stub objects) still needs a symbol at its start: without one splat folds those
    # bytes into nothing and the build comes out short.
    for addr, n, lib, obj, labels in hits:
        if addr not in by_addr and (not starts or addr in starts):
            by_addr[addr].append((f"func_{addr:08X}", f"{lib}/{obj}"))
    prefer =[p.strip().upper() for p in a.prefer.split(",") if p.strip()]
    lines, seen, ambiguous = [], set(), []
    for addr in sorted(by_addr):
        cands = [c for c in by_addr[addr] if c[0] not in seen]
        if not cands:
            continue
        if len({c[0] for c in cands}) > 1:
            pick = [c for p in prefer for c in cands if c[1].upper().startswith(p)]
            if not pick:
                ambiguous.append((addr, cands))
                continue
            cands = pick[:1]
        name, obj = cands[0]
        seen.add(name)
        kind = "func" if not starts or addr in starts else "label"
        lines.append((addr, name, obj, kind))
    out = [BEGIN, f"// PsyQ {best} (lab313ru/psx_psyq_signatures), unique matches only"]
    last = None
    for addr, name, obj, kind in lines:
        if obj != last:
            out.append(f"// {obj}"); last = obj
        out.append(f"{name} = 0x{addr:08X}; // type:{kind}")
    for addr, cands in ambiguous:
        out.append(f"// 0x{addr:08X}: ambiguous ({' / '.join(f'{n} in {o}' for n, o in cands)}) - unnamed")
    out.append(END)
    block = "\n".join(out) + "\n"
    print(f"// best: {best}, {len(hits)} objs, {sum(h[1] for h in hits)} of {hi - lo} bytes; "
          f"{sum(k == 'func' for *_, k in lines)} funcs, {sum(k == 'label' for *_, k in lines)} labels, "
          f"{len(ambiguous)} ambiguous; lowest match 0x{hits[0][0]:08X} ({hits[0][2]}/{hits[0][3]})",
          file=sys.stderr)
    if not a.write:
        sys.stdout.write(block)
        return
    objs, end = [], 0
    for addr, n, lib, obj, _ in hits:          # non-overlapping, earliest first
        if addr >= end:
            objs.append((addr, n, lib, obj))
            end = addr + n
    with open("configs/USA/psyq/objects.txt", "w", newline="\n") as f:
        f.write(f"# PsyQ {best} objects found in {os.path.basename(a.exe)} by tools/scripts/psyq_match.py: "
                "address, .text bytes, LIB/OBJ\n")
        for addr, n, lib, obj in objs:
            f.write(f"0x{addr:08X} 0x{n:X} {lib}/{obj}\n")
    path = "configs/USA/sym.main.txt"
    cur = open(path).read() if os.path.exists(path) else ""
    cur = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + r"\n?", "", cur, flags=re.S)
    open(path, "w", newline="\n").write(cur.rstrip("\n") + ("\n\n" if cur.strip() else "") + block)
    print(f"// wrote {path}", file=sys.stderr)


main()
