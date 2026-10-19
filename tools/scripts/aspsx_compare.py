"""Compare our assembler stage (maspsx + aspsx_fill.py + GNU as) with Sony's real ASPSX.

The matching build never runs ASPSX: maspsx rewrites cc1's output to look like ASPSX's, and
aspsx_fill.py adds the slot filling maspsx lacks. This checks that model against the real
assembler on the game's own C. For each file: compile the matched C (INCLUDE_ASM blanked, so
only C goes in), assemble that same text both ways, and compare .text word by word, ignoring the
fields a relocation fills in. ASPSX is a Windows program, so the host runs it; the rest runs in
tools/dk.

    tools/dk python3 tools/scripts/aspsx_compare.py prep [SRC.c ...]     # -> build/aspsx/*.s, *.o
    for s in build/aspsx/*.crlf.s; do ext/aspsx/2.86/ASPSX.EXE -o ${s%.crlf.s}.obj $s; done
    tools/dk python3 tools/scripts/aspsx_compare.py compare [SRC.c ...]

The ASPSX binaries are mkst/maspsx's `aspsx` release (aspsx-binaries.tar.gz); unpack 2.86 into
ext/aspsx/ (ignored, like the rest of ext/). Findings so far: docs/ARCHITECTURE.md, Toolchain."""
import glob
import os
import re
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
import decomp  # noqa: E402

OUT = "build/aspsx"
X = "mipsel-linux-gnu-"
REGS = {n: i for i, n in enumerate(
    "zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra"
    .split())}
REGS["s8"] = 30
REGNAME = re.compile(r"\$(" + "|".join(sorted(REGS, key=len, reverse=True)) + r")\b")


def units(args):
    if args:
        return args
    return sorted(p.replace("\\", "/") for p in glob.glob("src/**/*.c", recursive=True)
                  if "/psyq/" not in p.replace("\\", "/"))


def stem(src):
    return src[len("src/"):-len(".c")].replace("/", "__")


def sh(cmd):
    r = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError(f"{cmd[:120]}...: {r.stderr.strip()[-400:]}")
    return r.stdout


def prep(srcs):
    """cc1 output of each file's C (no INCLUDE_ASM), and our own object from the same text."""
    os.makedirs(OUT, exist_ok=True)
    base = decomp.mk_config()
    for src in srcs:
        cfg = decomp.unit_cfg(base, src)
        g = cfg.get("GVAL", "0")
        cc1 = f"/opt/gcc/{cfg.get('CC_VER')}/cc1 -quiet {cfg.get('OPT_FLAGS', '-O2')} -G{g} " \
              f"{cfg.get('EXTRA_CC1FLAGS', '')}"
        cpp = (f"{X}cpp {' '.join(decomp.make_flags('CPPFLAGS'))} '-DINCLUDE_ASM(F,N)=' "
               f"'-DINCLUDE_RODATA(F,N)=' {src}")
        s = f"{OUT}/{stem(src)}.s"
        sh(f"{cpp} | {cc1} 2>/dev/null | python3 tools/scripts/dmpsx.py > {s}")
        # ASPSX takes numbered registers only (our GTE macros write $t0)
        text = [REGNAME.sub(lambda m: f"${REGS[m.group(1)]}", l) for l in decomp.read(s).split("\n")
                if not l.strip().startswith(".include")]
        decomp.write(f"{OUT}/{stem(src)}.crlf.s", "\r\n".join(text) + "\r\n")
        post = decomp.post_filters(cfg)
        sh(f"python3 {decomp.MASPSX} {cfg.get('MASPSX_ARGS', '')} < {s} {post} | "
           f"{X}as {' '.join(decomp.make_flags('ASFLAGS'))} -o {OUT}/{stem(src)}.o")
        print(f"prepared {src}")


# ---------------------------------------------------------------- Sony's LNK object format

class ObjError(Exception):
    pass


def read_obj(data):
    """{section name: (bytes, set of patched offsets)} and {symbol: (section name, offset)}."""
    if data[:4] != b"LNK\x02":
        raise ObjError("not an LNK object")
    i, cur = 4, None
    sections, code, patched, syms = {}, {}, {}, {}

    def u8():
        nonlocal i
        i += 1
        return data[i - 1]

    def u16():
        nonlocal i
        i += 2
        return struct.unpack_from("<H", data, i - 2)[0]

    def u32():
        nonlocal i
        i += 4
        return struct.unpack_from("<I", data, i - 4)[0]

    def name():
        nonlocal i
        n = u8()
        i += n
        return data[i - n:i].decode("latin-1")

    def expr():
        t = u8()
        if t == 0:
            u32()
        elif t in (2, 4, 6, 8, 0x0C):
            u16()
        elif 0x20 <= t <= 0x4A:
            expr()
            expr()
        else:
            raise ObjError(f"unknown expression tag {t:#x} at {i - 1:#x}")

    while i < len(data):
        rec = u8()
        if rec == 0:
            break
        if rec == 2:
            n = u16()
            code.setdefault(cur, bytearray()).extend(data[i:i + n])
            i += n
        elif rec == 6:
            cur = u16()
        elif rec == 8:
            code.setdefault(cur, bytearray()).extend(b"\0" * u32())
        elif rec == 10:
            u8()
            off = u16()
            patched.setdefault(cur, set()).add(off & ~3)
            expr()
        elif rec == 12:
            u16()
            sect, off = u16(), u32()
            syms[name()] = (sect, off)
        elif rec == 14:
            u16()
            name()
        elif rec == 16:
            sid = u16()
            u16()
            u8()
            sections[sid] = name()
        elif rec == 18:
            sect, off = u16(), u32()
            syms[name()] = (sect, off)
        elif rec == 20:
            u16()
            u8()
            name()
        elif rec == 28:
            u16()
            name()
        elif rec == 46:
            u8()
        elif rec in (50, 60):                   # source line records (SLD): offset
            u16()
        elif rec == 52:
            u16()
            u8()
        elif rec == 54:
            u16()
            u16()
        elif rec == 56:
            u16()
            u32()
        elif rec == 58:
            u16()
            u32()
            u16()
        elif rec == 74:                         # function start (from .ent/.frame)
            sect, off = u16(), u32()
            u16()
            u32()
            u16()
            u32()
            u16()
            u32()
            u32()
            syms.setdefault(name(), (sect, off))
        elif rec in (76, 78, 80):               # function end, block start/end
            u16()
            u32()
            u32()
        elif rec == 82:                         # def
            u16()
            u32()
            u16()
            u16()
            u32()
            name()
        elif rec == 48:
            u16()
            sect = u16()
            u32()
            syms[name()] = (sect, None)
        else:
            raise ObjError(f"unknown record {rec} at {i - 1:#x}")
    out = {sections.get(k, str(k)): (bytes(v), patched.get(k, set())) for k, v in code.items()}
    return out, {n: (sections.get(s, str(s)), o) for n, (s, o) in syms.items()}


# ---------------------------------------------------------------- our ELF object

def read_elf_text(o):
    """(.text bytes, set of relocated offsets, [(offset, size, name)] of functions)."""
    sh(f"{X}objcopy -O binary --only-section=.text {o} {o}.text")
    text = open(f"{o}.text", "rb").read()
    rel = {int(m.group(1), 16) & ~3 for m in
           re.finditer(r"^([0-9a-f]+)\s+R_MIPS", sh(f"{X}objdump -r -j .text {o}"), re.M)}
    funcs = []
    for line in sh(f"{X}readelf -sW {o}").splitlines():
        p = line.split()
        if len(p) >= 8 and p[3] == "FUNC" and p[6] != "UND":
            funcs.append((int(p[1], 16), int(p[2]), p[7]))
    return text, rel, sorted(funcs)


MASK_I = 0xFFFF0000            # opcode, rs, rt: the immediate is the relocation's
MASK_J = 0xFC000000


def masked(w):
    op = w >> 26
    return w & (MASK_J if op in (2, 3) else MASK_I)


def compare(srcs):
    total_f = total_bad = 0
    for src in srcs:
        o, obj = f"{OUT}/{stem(src)}.o", f"{OUT}/{stem(src)}.obj"
        if not os.path.exists(obj):
            print(f"{src}: no ASPSX object (run ASPSX on {OUT}/{stem(src)}.crlf.s)")
            continue
        try:
            secs, _ = read_obj(open(obj, "rb").read())
        except ObjError as e:
            print(f"{src}: {e}")
            continue
        sony, sony_rel = secs.get(".text", (b"", set()))
        ours, our_rel, funcs = read_elf_text(o)
        bad = []
        for off, size, fn in funcs:
            diffs = []
            for k in range(off, off + size, 4):
                if k + 4 > len(sony) or k + 4 > len(ours):
                    diffs.append(k)
                    break
                a, b = struct.unpack_from("<I", ours, k)[0], struct.unpack_from("<I", sony, k)[0]
                if k in our_rel or k in sony_rel:
                    a, b = masked(a), masked(b)
                if a != b:
                    diffs.append(k)
            if diffs:
                bad.append((fn, off, len(diffs)))
        total_f += len(funcs)
        total_bad += len(bad)
        size_note = "" if len(ours) == len(sony) else f" (.text {len(ours)} vs ASPSX {len(sony)} bytes)"
        print(f"{src}: {len(funcs)} functions, {len(bad)} differ{size_note}")
        for fn, off, n in bad[:20]:
            print(f"    {fn} +{off:#x}: {n} word(s)")
    print(f"TOTAL {total_f} functions, {total_bad} differ")


if __name__ == "__main__":
    cmd, rest = (sys.argv[1], sys.argv[2:]) if len(sys.argv) > 1 else ("", [])
    if cmd == "prep":
        prep(units(rest))
    elif cmd == "compare":
        compare(units(rest))
    else:
        sys.exit(__doc__)
