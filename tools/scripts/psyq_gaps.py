"""Find the Sony object for each PsyQ gap (src/main/psyq/unknown/*.c).

usage (in the container):
  python3 tools/scripts/psyq_gaps.py [ext/psyq47/lib] [disc/<EXE>] [--write]

psyq_match.py names library code by signature, but many tiny objects are byte-identical
except for what they call or which global they use (CdSync, CdFlush, CdReady... are all
`jal X` wrappers), so their addresses were left unnamed. This compares every object in the
converted 4.7 libraries with each gap's code, relocated fields masked, and then checks every
relocation: the address the exe holds for each external symbol must be where the exe's
symbol of that name is (configs/USA/sym.main.txt, symbols.ld). Prints, per gap, the objects
that fit, best first:
  ok        the code and every named symbol agree
  unknown   the code agrees; some symbols aren't named in the exe yet (listed with the
            address the exe uses, for symbols.ld)
  conflict  the code agrees but a symbol is somewhere else: a lookalike, not this object
"""
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(__file__))
import decomp as d          # noqa: E402
import psyq_objs as po      # noqa: E402


def exe_symbols(libdir):
    """Symbol addresses the exe is known to use: the symbol files, plus every global defined in
    the data sections of the objects already linked (configs/USA/psyq/layout.txt places them)."""
    out = {}
    for p in (f"{d.CFG}/sym.main.txt", f"{d.CFG}/psyq/symbols.ld"):
        if os.path.exists(p):
            for line in d.read(p).splitlines():
                m = d.SYM_LINE.match(line)
                if m:
                    out[m.group(1)] = int(m.group(2), 16)
    lay = f"{d.CFG}/psyq/layout.txt"
    for line in (d.read(lay).splitlines() if os.path.exists(lay) else []):
        p = line.split()
        if len(p) < 4 or line.startswith("#") or p[3] != "ok":
            continue
        secs = {m.group(1): int(m.group(2), 16) for m in re.finditer(r"(\.\w+)=0x([0-9A-F]+)", line)}
        path = os.path.join(libdir, p[1])
        if secs and os.path.exists(path):
            for name, (sec, off) in po.defined(path).items():
                if sec in secs:
                    out.setdefault(name, secs[sec] + off)
    return out


def linked_objects():
    """Objects the build already links (configs/USA/main.yaml lib entries): lib/member."""
    t = d.read(d.yaml_of(d.need_proj()["name"]))
    return {f"{m.group(1)}/{m.group(2)}" for m in re.finditer(r"- \[0x[0-9A-F]+, lib, (\w+), (\w+), \.text\]", t)}


def gaps():
    """(file, start address, end address, functions) per gap file, from the yaml."""
    proj = d.need_proj()
    off = int(proj["header"]["t_addr"], 16) - 0x800
    # every .text subsegment of the exe, in order (code entries, and lib objects' .text)
    subs = [(int(m.group(1), 16) + off, m.group(2)) for m in
            re.finditer(r"- \[0x([0-9A-F]+), ((?:c|asm|lib)[^\]]*)\]", d.read(d.yaml_of(proj["name"])))
            if not re.search(r"\.(rdata|rodata|data|bss)\b|\.rodata", m.group(2))]
    out = []
    for i, (a, what) in enumerate(subs):
        m = re.match(r"c, (main/psyq/unknown/\w+)$", what)
        if m and i + 1 < len(subs):
            src = f"src/{m.group(1)}.c"
            names = re.findall(r"INCLUDE_ASM\(\"[^\"]+\", (\w+)\)", d.read(src)) if os.path.exists(src) else []
            if True:   # files already matched as C too: linking the object replaces them
                out.append((src, a, subs[i + 1][0], names))
    return out


def main():
    libdir = sys.argv[1] if len(sys.argv) > 1 else "ext/psyq47/lib"
    proj = d.need_proj()
    exe_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join("disc", proj["exe"])
    raw = open(exe_path, "rb").read()
    base = int(proj["header"]["t_addr"], 16)
    exe = raw[0x800:]
    syms = exe_symbols(libdir)
    taken = linked_objects()
    objs = sorted(os.path.join(r, f) for r, _, fs in os.walk(libdir) for f in fs if f.endswith(".o"))
    sizes = {o: po.sections(o).get(".text", 0) for o in objs}
    for src, start, end, names in gaps():
        span = end - start
        print(f"{src} 0x{start:08X} ({span:#x} bytes): {', '.join(names)}")
        hits = []
        for o in objs:
            n = sizes[o]
            if not n or n > span or (n + 15) & ~15 < span:
                continue                    # the object (padded to 16) must fill the gap exactly
            ndiff, nw, _, ext, conflicts = po.check(o, exe, base, start)
            if ndiff:
                continue
            bad = [f"{s} at 0x{a:08X} (exe has 0x{syms[s]:08X})" for s, a in ext.items() if s in syms and syms[s] != a]
            unknown = [f"{s} = 0x{a:08X}" for s, a in ext.items() if s not in syms]
            rel = os.path.relpath(o, libdir)
            lib, mem = rel.split(os.sep)[0], os.path.splitext(os.path.basename(o))[0]
            state = "conflict" if bad or conflicts else "unknown" if unknown else "ok"
            if f"{lib}/{mem}" in taken:
                state += " (already linked elsewhere)"
            hits.append((["ok", "unknown", "conflict"].index(state.split()[0]), lib, mem, state, bad + conflicts, unknown,
                         sorted(s for s in ext if s in syms)))
        if not hits:
            print("  no object fits")
        for _, lib, mem, state, bad, unknown, known in sorted(hits)[:6]:
            print(f"  {state:9} {lib} {mem}" + (f"  uses {', '.join(known)}" if known else "") +
                  (f"; conflicts: {'; '.join(bad)}" if bad else "") + (f"; not named: {'; '.join(unknown)}" if unknown else ""))
        # the pick: the best state; then a library the exe already links objects from (libcd over
        # libds's copies); then one not picked for an earlier gap (identical twins, in order)
        libs = {t.split("/")[0] for t in taken}
        best = [h for h in hits if h[0] == min((x[0] for x in hits), default=2) and h[0] < 2 and "already" not in h[3]]
        best.sort(key=lambda h: (h[1] not in libs, f"{h[1]}/{h[2]}" in picked, h[1], h[2]))
        if best and f"{best[0][1]}/{best[0][2]}" not in picked:
            _, lib, mem = best[0][:3]
            picked[f"{lib}/{mem}"] = (start, sizes[os.path.join(libdir, lib, mem + ".o")], src)
            print(f"  -> {lib} {mem}")
    if WRITE:
        p = f"{d.CFG}/psyq/objects.txt"
        t = d.read(p)
        if BEGIN in t:
            t = t[:t.index(BEGIN)].rstrip("\n") + "\n"
        t += BEGIN + "\n" + "".join(f"0x{a:08X} 0x{n:X} {lm.split('/')[0].upper()}.LIB/{lm.split('/')[1].upper()}.OBJ\n"
                                   for lm, (a, n, _) in sorted(picked.items(), key=lambda kv: kv[1][0])) + END + "\n"
        d.write(p, t)
        print(f"wrote {len(picked)} objects to {p}; next: psyq_objs.py --write, then decomp.py link-psyq")


BEGIN, END = "# --- found by tools/scripts/psyq_gaps.py ---", "# --- end psyq_gaps.py ---"
WRITE = "--write" in sys.argv
picked = {}

if __name__ == "__main__":
    sys.argv = [x for x in sys.argv if x != "--write"]
    main()
