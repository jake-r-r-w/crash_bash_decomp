"""Check Sony's own PsyQ library objects against the library code in the exe.

usage (in the container):
  python3 tools/scripts/psyq_objs.py ext/psyq47/lib disc/<EXE> [--write]

For every object listed in configs/USA/psyq/objects.txt (from tools/scripts/psyq_match.py), loads the
matching ELF object from the converted PsyQ 4.7 libraries and compares its .text with the exe
at the object's address, ignoring only the fields the linker fills in. The relocations also
tell where the object's other sections were placed: an instruction that refers to `.data+X`
and holds absolute address A in the exe puts .data at A - X. Prints one line per object and a
summary; --write saves the results to configs/USA/psyq/layout.txt (object, file, text address,
and the address of each other section), which the build uses to link the objects in place.

The converted libraries are Sony's code with no licence: keep them in gitignored ext/.
"""
import argparse, os, re, struct, subprocess, sys, tempfile
from collections import defaultdict

X = "mipsel-linux-gnu-"
REL = re.compile(r"^([0-9a-f]{8})\s+(R_MIPS_\w+)\s+(\S+)")
SECSYM = re.compile(r"^\$.*\.rel\.(\w+)@([0-9a-fA-F]+)$")


def run(cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def find_obj(libdir, lib_obj):
    """LIBGPU.LIB/SYS.OBJ -> <libdir>/libgpu/sys.o (any case); 2MBYTE.OBJ/2MBYTE.OBJ -> 2mbyte.o."""
    lib, obj = lib_obj.split("/")
    sub = lib.lower().replace(".lib", "").replace(".obj", "")
    want = obj.lower().replace(".obj", ".o")
    for d in (os.path.join(libdir, sub), libdir):
        if os.path.isdir(d):
            for f in os.listdir(d):
                if f.lower() == want:
                    return os.path.join(d, f)
    return None


def section_bytes(path, sec):
    with tempfile.NamedTemporaryFile(delete=False) as t:
        out = t.name
    r = run([X + "objcopy", "-O", "binary", f"--only-section={sec}", path, out])
    data = open(out, "rb").read() if r.returncode == 0 else b""
    os.remove(out)
    return data


def relocs(path, sec=".text"):
    r = run([X + "objdump", "-r", "-j", sec, path])
    out = []
    for l in r.stdout.splitlines():
        m = REL.match(l.strip())
        if m:
            out.append((int(m.group(1), 16), m.group(2), m.group(3)))
    return out


def sections(path):
    r = run([X + "objdump", "-h", path])
    out = {}
    for l in r.stdout.splitlines():
        p = l.split()
        if len(p) >= 7 and p[0].isdigit():
            out[p[1]] = int(p[2], 16)
    return out


def check(path, exe, base, addr):
    """Compare .text; infer section bases. Returns (ndiff, nwords, bases, externs, conflicts)."""
    text = section_bytes(path, ".text")
    n = len(text) // 4
    off = addr - base
    tw = struct.unpack_from(f"<{n}I", exe, off)
    ow = struct.unpack_from(f"<{n}I", text, 0)
    rel = relocs(path)
    keep = {}
    for o, typ, _ in rel:
        keep[o // 4] = 0xFC000000 if typ == "R_MIPS_26" else 0xFFFF0000
    ndiff = sum(1 for i in range(n) if (tw[i] & keep.get(i, 0xFFFFFFFF)) != (ow[i] & keep.get(i, 0xFFFFFFFF)))
    bases, ext, conflicts = defaultdict(set), {}, []
    sext = lambda v: v - 0x10000 if v & 0x8000 else v
    pending = {}
    for o, typ, sym in rel:
        i = o // 4
        if typ == "R_MIPS_HI16":
            pending[sym] = i
            continue
        if typ == "R_MIPS_LO16" and sym in pending:
            h = pending[sym]
            val = ((tw[h] & 0xFFFF) << 16) + sext(tw[i] & 0xFFFF)
            add = ((ow[h] & 0xFFFF) << 16) | (ow[i] & 0xFFFF)   # converter: plain 32-bit halves
        elif typ == "R_MIPS_LO16":
            continue                    # a second LO16 of the same HI: already counted
        elif typ == "R_MIPS_26":
            val = ((addr + o) & 0xF0000000) | ((tw[i] & 0x3FFFFFF) << 2)
            add = (ow[i] & 0x3FFFFFF) << 2
        else:
            continue
        s = (val - add) & 0xFFFFFFFF
        m = SECSYM.match(sym)
        if m:
            bases["." + m.group(1)].add(s - int(m.group(2), 16))
        else:
            if sym in ext and ext[sym] != s:
                conflicts.append(sym)
            ext[sym] = s
    # references through the object's own global symbols place their sections too
    for l in run([X + "objdump", "-t", path]).stdout.splitlines():
        p = l.split()
        if len(p) >= 6 and p[-1] in ext and p[-3].startswith(".") and p[-3] not in (".text",):
            bases[p[-3]].add(ext[p[-1]] - int(p[0], 16))
    out = {}
    for sec, vs in bases.items():
        if len(vs) > 1:
            conflicts.append(f"{sec} at {sorted(hex(v) for v in vs)}")
        out[sec] = min(vs)
    if ".text" in out and out[".text"] != addr:
        conflicts.append(f".text relocations point at 0x{out['.text']:X}, not 0x{addr:X}")
    return ndiff, n, out, ext, conflicts


def defined(path):
    """Global symbols an object defines: name -> (section, offset)."""
    out = {}
    for l in run([X + "objdump", "-t", path]).stdout.splitlines():
        m = re.match(r"^([0-9a-f]{8})\s+g\s.*?(\.\w+)\s+[0-9a-f]{8}\s+(\S+)$", l)
        if m:
            out[m.group(3)] = (m.group(2), int(m.group(1), 16))
    return out


def fix(path):
    """Repair one extracted object for GNU ld (the Makefile runs this on each member):
    - HI16/LO16 addends: the converter split the 32-bit addend into plain halves, while REL
      relocation reads (hi << 16) + (short)lo; where lo is negative, hi needs the carry.
    Symbols whose definition doesn't sit where the exe uses them (layout lines `! symbol
    address`) are pinned by configs/USA/psyq/symbols.ld, which the link reads last."""
    secs = {}
    for l in run([X + "objdump", "-h", path]).stdout.splitlines():
        p = l.split()
        if len(p) >= 7 and p[0].isdigit():
            secs[p[1]] = int(p[5], 16)
    data = bytearray(open(path, "rb").read())
    if ".text" in secs:
        off = secs[".text"]
        pending = {}
        for o, typ, sym in relocs(path):
            if typ == "R_MIPS_HI16":
                pending[sym] = o
            elif typ == "R_MIPS_LO16" and sym in pending:
                h = pending.pop(sym)
                lo = struct.unpack_from("<I", data, off + o)[0] & 0xFFFF
                if lo & 0x8000:
                    w = struct.unpack_from("<I", data, off + h)[0]
                    struct.pack_into("<I", data, off + h, (w & 0xFFFF0000) | (((w & 0xFFFF) + 1) & 0xFFFF))
    open(path, "wb").write(bytes(data))


def main():
    if len(sys.argv) == 3 and sys.argv[1] == "fix":
        fix(sys.argv[2])
        return
    ap = argparse.ArgumentParser()
    ap.add_argument("libdir")
    ap.add_argument("exe")
    ap.add_argument("--write", action="store_true")
    ap.add_argument("-v", action="store_true", help="print every object, not only problems")
    a = ap.parse_args()
    exe_full = open(a.exe, "rb").read()
    base = struct.unpack_from("<I", exe_full, 0x18)[0]
    exe = exe_full[0x800:]
    objs = []
    for l in open("configs/USA/psyq/objects.txt"):
        if l.startswith("0x"):
            ad, n, o = l.split()
            objs.append((int(ad, 16), o))
    good = bad = missing = 0
    all_ext = {}
    good_b = 0
    rows = []
    for addr, o in objs:
        p = find_obj(a.libdir, o)
        if not p:
            missing += 1
            print(f"  missing  {o}")
            continue
        nd, n, secs, ext, conf = check(p, exe, base, addr)
        ok = nd == 0 and not conf
        if ok:
            good += 1
            good_b += n * 4
        else:
            bad += 1
        if a.v or not ok:
            print(f"  {'OK  ' if ok else 'DIFF'}  0x{addr:08X} {o:28s} {n * 4:6d} B  {nd} words differ"
                  + "".join(f"  {s}=0x{v:08X}" for s, v in sorted(secs.items()) if s != ".text")
                  + (f"  CONFLICT {conf[:3]}" if conf else ""))
        if ok:
            for sym, v in ext.items():
                if all_ext.setdefault(sym, v) != v:
                    print(f"  external {sym} resolves to 0x{v:08X} and 0x{all_ext[sym]:08X}")
        sizes = sections(p)
        rows.append((addr, o, os.path.relpath(p, a.libdir).replace("\\", "/"), ok, n * 4,
                     {s: (secs.get(s), sizes.get(s, 0)) for s in sizes if s not in (".text", ".note", ".comment")}))
    # sections only other objects refer to: place them by their global symbols
    for addr, o, rp, ok, n, secs in rows:
        for sec, (at, sz) in list(secs.items()):
            if at is not None or not sz:
                continue
            for l in run([X + "objdump", "-t", os.path.join(a.libdir, rp)]).stdout.splitlines():
                p = l.split()
                if len(p) >= 6 and p[-3] == sec and p[-1] in all_ext:
                    secs[sec] = (all_ext[p[-1]] - int(p[0], 16), sz)
                    break
    # still unplaced: sections keep link order, so it follows the previous object's (4-aligned)
    for i, (addr, o, rp, ok, n, secs) in enumerate(rows):
        for sec, (at, sz) in list(secs.items()):
            if at is not None or not sz:
                continue
            prev = next(((r[5][sec][0] + r[5][sec][1]) for r in reversed(rows[:i])
                         if sec in r[5] and r[5][sec][0] is not None), None)
            nxt = next((r[5][sec][0] for r in rows[i + 1:] if sec in r[5] and r[5][sec][0] is not None), None)
            if prev is None:
                continue
            at = (prev + 3) & ~3
            if nxt is not None and at + sz > nxt:
                print(f"  {o} {sec}: no room between neighbours (0x{at:08X}+0x{sz:X} > 0x{nxt:08X})")
                continue
            secs[sec] = (at, sz)
    # a global whose definition isn't where the references go (seen in libspu)
    moved = {}
    for addr, o, rp, ok, n, secs in rows:
        for sym, (sec, off) in defined(os.path.join(a.libdir, rp)).items():
            at = addr if sec == ".text" else secs.get(sec, (None,))[0]
            if sym in all_ext and at is not None and at + off != all_ext[sym]:
                moved[sym] = all_ext[sym]
                print(f"  {sym}: defined at 0x{at + off:08X} in {o}, called at 0x{all_ext[sym]:08X}")
    print(f"{good} objects identical ({good_b} bytes of .text), {bad} differ, {missing} not in {a.libdir}")
    if a.write:
        with open("configs/USA/psyq/layout.txt", "w", newline="\n") as f:
            f.write("# Sony PsyQ objects checked against the exe by tools/scripts/psyq_objs.py.\n"
                    "# object, file under the converted libs, text address, identical?, text size,\n"
                    "# then section=address/size for its other sections (address ? = not referenced)\n")
            for addr, o, rp, ok, n, secs in rows:
                f.write(f"{o} {rp} 0x{addr:08X} {'ok' if ok else 'diff'} 0x{n:X}"
                        + "".join(f" {s}={('0x%08X' % v) if v is not None else '?'}/0x{sz:X}"
                                  for s, (v, sz) in sorted(secs.items()) if sz) + "\n")
            f.write("# references to symbols defined elsewhere than the exe calls them: ! symbol address\n")
            for sym, v in sorted(moved.items()):
                f.write(f"! {sym} 0x{v:08X}\n")
            f.write("# externals: symbol=address the objects' references resolve to in the exe\n")
            for sym, v in sorted(all_ext.items()):
                f.write(f"= {sym} 0x{v:08X}\n")
        print("wrote configs/USA/psyq/layout.txt")


if __name__ == "__main__":
    main()
