"""Turn an overlay's asm data (splat's asm/data/overlays/<ovl>/*.data.s) into C definitions.

    tools/dk python3 tools/scripts/data2c.py OVL [--rodata SYM] [--out FILE]

Each object's bytes come from the retail binary (disc/<ovl>.bin), not from splat's directives
(splat reads words of small values as strings). Objects start at splat's labels; a label nothing
in src/ or include/ names is folded into the object before it. An object is written with the type
its `extern` declaration in include/bash/ gives it (structs are laid out from the headers; an
incomplete array gets its length from the object's size); an undeclared one as an array of the
element its directives use. All-zero objects are `= {0}` (or `= 0`), so they stay in .data, where
cc1 (gcc 2.8) puts explicitly zero-initialised data. A 4-byte value that points into the exe, the
engine or the overlay is written as its symbol (name + offset), never as a number, so the data
moves with what it points at; external names the overlay's link doesn't define yet are listed as
PROVIDE lines for configs/USA/undefined_syms_manual.txt.

--rodata SYM also writes the strings of an INCLUDE_RODATA block as `const char` arrays: cc1 emits
string literals of an initialiser in reverse order of first use (cc1plus, which built the game,
in order), so named arrays are what reproduces the retail order. Paste the output where the
data and the INCLUDE_RODATA were (rodata first, so it precedes the functions' jump tables), and
build: `make check` is the test.
"""
import argparse
import glob
import json
import os
import re
import struct
import sys

CFG = "configs/USA"
SCALARS = {"s8": (1, True), "u8": (1, False), "char": (1, True), "s16": (2, True), "u16": (2, False),
           "short": (2, True), "s32": (4, True), "u32": (4, False), "int": (4, True), "long": (4, True),
           "unsigned": (4, False), "bool": (4, True), "signed": (4, True)}
PTR_LO, PTR_HI = 0x80010000, 0x800D7148      # the exe, the engine and its slot
SDK_TYPES = ["ext/psyq47/include/libgte.h"]
# a function pointer field, `RET (*name)(ARGS)`: a 4-byte pointer written as the function's name
FUNC_PTR = re.compile(r"^[\w\s\*]+\(\s*\*\s*(\w+)\s*\)\s*\([^()]*\)$", re.S)
FN = "@fn"
FN_DECLS = {}   # name -> the declaration text of a function pointer (array)


def strip_comments(t):
    return re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", "", t, flags=re.S))


def evaldim(s):
    s = s.strip()
    if not s:
        return None
    try:
        return int(eval(re.sub(r"\b0x([0-9A-Fa-f]+)\b", lambda m: str(int(m.group(1), 16)), s), {}))
    except Exception:
        return False


class Types:
    """Struct layouts and scalar aliases from the headers (natural alignment, as gcc lays them)."""

    def __init__(self, texts):
        self.alias, self.structs = {}, {}
        for t in texts:
            t = strip_comments(t)
            for m in re.finditer(r"typedef\s+(struct|union)\s+(\w+)\s+(\w+)\s*;", t):
                self.alias[m.group(3)] = ("struct", m.group(2))
            for m in re.finditer(r"typedef\s+((?:unsigned\s+|signed\s+)?\w+)\s+(\w+)\s*;", t):
                if m.group(1) not in ("struct", "union", "enum"):
                    self.alias.setdefault(m.group(2), ("name", m.group(1).split()[-1]))
            for m in re.finditer(r"typedef\s+enum\s*\w*\s*\{[^}]*\}\s*(\w+)\s*;", t):
                self.alias[m.group(1)] = ("name", "s32")
            for m in re.finditer(r"(typedef\s+)?(struct|union)\s*(\w*)\s*\{", t):
                body, end = self._body(t, m.end())
                if body is None:
                    continue
                tail = re.match(r"\s*(\w+)?\s*;", t[end:])
                tag = m.group(3)
                name = tag or (tail.group(1) if tail and m.group(1) else None)
                if not name:
                    continue
                key = tag or "@" + name
                self.structs[key] = (m.group(2), body)
                if m.group(1) and tail and tail.group(1):
                    self.alias[tail.group(1)] = ("struct", key)

    @staticmethod
    def _body(t, i):
        depth = 1
        j = i
        while j < len(t) and depth:
            depth += t[j] == "{"
            depth -= t[j] == "}"
            j += 1
        return (t[i:j - 1], j) if not depth else (None, j)

    def resolve(self, name, seen=()):
        """('scalar', size, signed) | ('struct', key) for a type name; None if unknown."""
        if name in SCALARS:
            return ("scalar",) + SCALARS[name]
        if name in seen:
            return None
        a = self.alias.get(name)
        if a is None:
            return ("struct", name) if name in self.structs else None
        if a[0] == "struct":
            return a
        return self.resolve(a[1], seen + (name,))

    def fields(self, key):
        """[(name, type, ptr, dims)] of a struct, or None if a field can't be read."""
        kind, body = self.structs[key]
        out = []
        for decl in body.split(";"):
            decl = decl.strip()
            if not decl:
                continue
            fm = FUNC_PTR.match(decl)
            if fm:
                out.append((fm.group(1), FN, 1, [], kind))
                continue
            if "{" in decl or "(" in decl or ":" in decl:
                return None
            m = re.match(r"((?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+|enum\s+)*\w+)\s+(.*)$", decl, re.S)
            if not m:
                return None
            base = m.group(1).split()
            tname = base[-1] if base[-1] not in ("unsigned", "signed") else "u32"
            if "enum" in base:
                tname = "s32"
            for d in m.group(2).split(","):
                dm = re.match(r"\s*(\**)\s*(\w+)\s*((?:\[[^\]]*\]\s*)*)$", d)
                if not dm:
                    return None
                dims = [evaldim(x) for x in re.findall(r"\[([^\]]*)\]", dm.group(3))]
                if any(x is None or x is False for x in dims):
                    return None
                out.append((dm.group(2), tname, len(dm.group(1)), dims, kind))
        return out

    def layout(self, tname, ptr=0):
        """(size, align) of a type, or None."""
        if ptr:
            return 4, 4
        r = self.resolve(tname)
        if r is None:
            return None
        if r[0] == "scalar":
            return r[1], r[1]
        fs = self.fields(r[1])
        if fs is None:
            return None
        off, al, size = 0, 1, 0
        for fname, ft, fp, dims, kind in fs:
            lo = self.layout(ft, fp)
            if lo is None:
                return None
            n = 1
            for d in dims:
                n *= d
            fsz, fal = lo[0] * n, lo[1]
            al = max(al, fal)
            if kind == "union":
                size = max(size, fsz)
            else:
                off = (off + fal - 1) // fal * fal
                off += fsz
                size = off
        size = (size + al - 1) // al * al
        return size, al

    def members(self, tname):
        """[(name, type, ptr, dims, offset)] of a struct type."""
        r = self.resolve(tname)
        fs = self.fields(r[1])
        out, off = [], 0
        for fname, ft, fp, dims, kind in fs:
            sz, al = self.layout(ft, fp)
            n = 1
            for d in dims:
                n *= d
            if kind != "union":
                off = (off + al - 1) // al * al
            out.append((fname, ft, fp, dims, off if kind != "union" else 0))
            if kind != "union":
                off += sz * n
        return out, r[1] and self.structs[r[1]][0]


def load_syms(ovl):
    """address -> name from the exe's, the engine's and this overlay's symbol files."""
    out = {}
    for p in (f"{CFG}/sym.main.txt", f"{CFG}/sym.engine.txt", f"{CFG}/sym.{ovl}.txt"):
        if not os.path.exists(p):
            continue
        for l in open(p, encoding="utf-8"):
            m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)", l)
            if m:
                out.setdefault(int(m.group(2), 16), m.group(1))
    return out


def asm_labels(path):
    """[(label, rom)] of a splat data file, in order."""
    out, pend = [], None
    for l in open(path, encoding="utf-8"):
        m = re.match(r"\s*dlabel\s+(\w+)", l)
        if m:
            pend = m.group(1)
            continue
        m = pend and re.match(r"\s*/\*\s*([0-9A-Fa-f]+)\s", l)
        if m:
            out.append((pend, int(m.group(1), 16)))
            pend = None
    return out


def asm_symbolic(path):
    """ROM offsets of the words splat wrote as a symbol (it does that only for known labels)."""
    out = set()
    for l in open(path, encoding="utf-8"):
        m = re.match(r"\s*/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s*\*/\s*\.word\s+[A-Za-z_]", l)
        if m:
            out.add(int(m.group(1), 16))
    return out


def asm_kinds(path):
    """label -> set of directives used (word, short, byte, asciz)."""
    out, cur = {}, None
    for l in open(path, encoding="utf-8"):
        m = re.match(r"\s*dlabel\s+(\w+)", l)
        if m:
            cur = out.setdefault(m.group(1), set())
            continue
        m = cur is not None and re.search(r"\*/\s*\.(word|short|byte|asciz|ascii)\b", l)
        if m:
            cur.add(m.group(1))
    return out


def slot_names():
    """The slot overlays' unit names (configs/USA/overlays/*.yaml but the engine)."""
    return sorted(os.path.splitext(f)[0] for f in os.listdir("configs/USA/overlays")
                  if f.endswith(".yaml") and f != "engine.yaml")


def slot_of(path, slots):
    """The slot overlay a source or header belongs to: src/overlays/NAME/..., its owned header
    include/bash/NAME_ovl.h or a later file's include/bash/NAME_<file>.h (the leak's NAME.h is
    shared, not the overlay's)."""
    p = path.replace("\\", "/")
    for s in slots:
        if (re.search(r"(?:^|/)src/overlays/" + s + "/", p)
                or re.search(r"(?:^|/)include/bash/" + s + r"_\w+\.h$", p)):
            return s
    return None


def other_slot(path, ovl, slots=None):
    """Whether a source or header belongs to another slot overlay: the slot overlays share one
    address range, so another one's D_ name at the same address is a different object."""
    s = slot_of(path, slot_names() if slots is None else slots)
    return s is not None and s != ovl


def declarations(texts):
    """name -> (type, ptr, dims) from `extern TYPE NAME[...];` lines."""
    out = {}
    for t in texts:
        # one or more declarators: `extern u8 D_X[], D_Y[];` declares both
        for m in re.finditer(r"^\s*extern\s+((?:const\s+|volatile\s+|struct\s+|unsigned\s+|signed\s+)*\w+)\s+([^;()]+);",
                             strip_comments(t), re.M):
            base = m.group(1).split()[-1]
            for d in m.group(2).split(","):
                dm = re.match(r"\s*(\**)\s*(\w+)\s*((?:\[[^\]]*\]\s*)*)\s*$", d)
                if not dm:
                    break
                dims = [evaldim(x) for x in re.findall(r"\[([^\]]*)\]", dm.group(3))]
                out.setdefault(dm.group(2), (base, len(dm.group(1)), dims))
        # a function pointer or an array of them: `extern s32 (*D_X[5])(s32 player);`
        for m in re.finditer(r"^\s*extern\s+[\w\s\*]+\(\s*\*\s*(\w+)\s*((?:\[[^\]]*\]\s*)*)\)\s*\([^;]*\)\s*;",
                             strip_comments(t), re.M):
            dims = [evaldim(x) for x in re.findall(r"\[([^\]]*)\]", m.group(2))]
            out.setdefault(m.group(1), (FN, 1, dims))
            FN_DECLS.setdefault(m.group(1), re.sub(r"^\s*extern\s+", "", m.group(0)).rstrip(";").strip())
    return out


def header_order(paths, ovl, slots=None):
    """The headers a slot overlay's data is typed from, its own first: the engine's headers may
    declare another overlay's object at the same shared address, and the first declaration wins."""
    slots = slot_names() if slots is None else slots
    keep = [p for p in sorted(paths) if not other_slot(p, ovl, slots)]
    return sorted(keep, key=lambda p: slot_of(p, slots) != ovl)


class Converter:
    def __init__(self, ovl):
        import yaml
        self.ovl = ovl
        y = yaml.safe_load(open(f"{CFG}/overlays/{ovl}.yaml", encoding="utf-8"))
        seg = next(s for s in y["segments"] if isinstance(s, dict) and s.get("name") == ovl)
        self.vram, self.start = seg["vram"], seg["start"]
        self.subs = [s for s in seg["subsegments"] if isinstance(s, list)]
        self.bin = open(f"disc/{ovl}.bin", "rb").read()
        self.end = self.vram + len(self.bin) - self.start
        texts = [open(p, encoding="utf-8").read() for p in header_order(glob.glob("include/bash/*.h"), ovl)]
        # the SDK's types (MATRIX, VECTOR, ...) for layouts only; its declarations aren't the game's
        sdk = [open(p, encoding="latin-1").read() for p in SDK_TYPES if os.path.exists(p)]
        self.types = Types(sdk + texts)
        self.decls = declarations(texts)
        self.srcs = "\n".join(open(p, encoding="utf-8").read() for p in glob.glob("src/**/*.c", recursive=True)
                              if not other_slot(p, ovl))
        self.hdrs = "\n".join(texts)
        # the headers without their declarations and comments: what inline code and macros name
        self.hdr_code = "\n".join(l for l in strip_comments(self.hdrs).splitlines()
                                  if not re.match(r"\s*extern\b", l))
        self.symbolic = set()
        for p in glob.glob(f"asm/data/overlays/{ovl}/*.data.s"):
            self.symbolic |= asm_symbolic(p)
        self.syms = load_syms(ovl)
        self.funcs = {}
        fj = json.load(open(".decomp/functions.json", encoding="utf-8"))
        for k, e in fj.items():
            if k.startswith(ovl + ":"):
                self.funcs[int(k.split(":")[1], 16)] = e.get("name")
        self.provide = {}
        self.externs = set()

    def rom(self, va):
        return va - self.vram + self.start

    def named(self, name, headers=False):
        """Whether C code names it (headers=True: or a header)."""
        pat = re.compile(r"\b" + re.escape(name) + r"\b")
        if pat.search(self.srcs) is not None or (headers and pat.search(self.hdrs) is not None):
            return True
        # code in a header (an inline function, a macro) names it too; a declaration doesn't
        return pat.search(self.hdr_code) is not None

    def targets(self, lo, hi):
        """Addresses in [lo, hi) that some 4-byte data word of the overlay points at."""
        out = set()
        for i, s in enumerate(self.subs):
            if str(s[1]).lstrip(".") not in ("data", "sdata"):
                continue
            end = self.subs[i + 1][0] if i + 1 < len(self.subs) else len(self.bin)
            for off in range(s[0], end - 3, 4):
                v = int.from_bytes(self.bin[off:off + 4], "little")
                if lo <= v < hi:
                    out.add(v)
        return out

    def objects(self, path):
        """[(name, vram, size, kinds)] of a data file. A label is folded into the object before it
        when no C code names it and no data word points at it, or when the object before is
        declared big enough to cover it (a header may still declare it: drop that declaration)."""
        labs = asm_labels(path)
        kinds = asm_kinds(path)
        sub_end = None
        first = labs[0][1]
        for i, s in enumerate(self.subs):
            if s[0] == first or (s[0] <= first and (i + 1 == len(self.subs) or self.subs[i + 1][0] > first)):
                sub_end = self.subs[i + 1][0] if i + 1 < len(self.subs) else len(self.bin)
        lo, hi = labs[0][1] - self.start + self.vram, sub_end - self.start + self.vram
        pointed = self.targets(lo, hi)
        objs = []
        for i, (lab, rom) in enumerate(labs):
            va = rom - self.start + self.vram
            name = self.syms.get(va, lab)
            nxt = labs[i + 1][1] if i + 1 < len(labs) else sub_end
            if objs:
                prev = objs[-1]
                d = self.decls.get(prev[0])
                covers = False
                if d and d[2] and d[2][0]:
                    lo_ = self.types.layout(d[0], d[1])
                    covers = lo_ is not None and prev[1] + lo_[0] * _prod(d[2]) > va
                if covers or (not self.named(name) and not self.named(lab) and va not in pointed):
                    if covers and self.named(name):
                        sys.stderr.write(f"note: {name} is inside {prev[0]}'s declared size; write it as {prev[0]}[...]\n")
                    prev[2] += nxt - rom
                    prev[3] |= kinds.get(lab, set())
                    continue
            objs.append([name, va, nxt - rom, set(kinds.get(lab, set()))])
        return objs

    # ---------------------------------------------------------------- values
    def symexpr(self, t, local, rom=None):
        """C expression for the address t, or None if it isn't a known place. Inside the overlay
        every address in range is one; outside it only where splat wrote the word as a symbol or
        a symbol file names the address, or the value is word-aligned off a 64 KB boundary
        (0x80070000 next to sfx ids is a flag; splat leaves most real pointers into the exe's
        data as numbers, as it has no label there)."""
        if not (PTR_LO <= t < PTR_HI):
            return None
        if not (self.vram <= t < self.end) and t not in self.syms and rom not in self.symbolic \
                and (t & 3 or not t & 0xFFFF):
            return None
        if self.vram <= t < self.end:
            cands = [(a, n) for a, n in local if a <= t]
            cands += [(a, n) for a, n in self.funcs.items() if a <= t]
            cands += [(a, n) for a, n in self.syms.items() if self.vram <= a <= t]
            if not cands:
                return None
            a, n = max(cands)
            if a in self.funcs and a == t:
                return n
            return f"&{n}" if a == t else f"(u8 *)&{n} + 0x{t - a:X}"
        if self.vram <= t < self.vram + 0x30000 and t >= self.end:
            return None
        n = self.syms.get(t)
        if n is None:
            n = f"D_{t:08X}"
        if n not in self.decls:
            self.externs.add(n)
        # splat lists an overlay's outside symbols from its asm, and C data is no longer asm
        self.provide[n] = t
        return f"&{n}"

    def scalar(self, size, signed, off, data, ptr_ok, cast, local):
        raw = int.from_bytes(data[off:off + size], "little")
        if size == 4 and ptr_ok:
            e = self.symexpr(raw, local, self.base_rom + off)
            # a plain word equal to an unnamed exe function's address is data (two halfwords of
            # a hero sequence in Pogo); a real code pointer has a function-pointer field
            if e is not None and re.fullmatch(r"&func_[0-9A-F]{8}", e):
                self.externs.discard(e[1:])
                self.provide.pop(e[1:], None)
                e = None
            if e is not None:
                return f"({cast}){e}" if cast else e
        if signed and raw >= 1 << (size * 8 - 1):
            if raw >= (1 << (size * 8)) - 0x10000:
                raw -= 1 << (size * 8)
            else:                                   # flags, or two halves of a word: keep them hex
                return f"({'s32' if size == 4 else 's16'})0x{raw:X}"
        if -10 < raw < 10:
            return str(raw)
        return f"-0x{-raw:X}" if raw < 0 else f"0x{raw:X}"

    def value(self, tname, ptr, dims, off, data, local, indent):
        """Initializer text for one (possibly array) object at data[off:]."""
        if dims:
            sz = self.types.layout(tname, ptr)[0]
            for d in dims[1:]:
                sz *= d
            items = [self.value(tname, ptr, dims[1:], off + i * sz, data, local, indent + 1) for i in range(dims[0])]
            return self._braces(items, indent)
        if ptr:
            # a function needs no cast into its pointer field; a function pointer field holding
            # anything else (data, an inner address) keeps its number
            if tname == FN:
                raw = int.from_bytes(data[off:off + 4], "little")
                e = self.funcs.get(raw) or self.symexpr(raw, local, self.base_rom + off)
                return e or self.scalar(4, False, off, data, False, None, local)
            return self.scalar(4, False, off, data, True, "void *", local)
        r = self.types.resolve(tname)
        if r[0] == "scalar":
            return self.scalar(r[1], r[2], off, data, r[1] == 4, "s32" if r[2] else "u32", local)
        ms, kind = self.types.members(tname)
        if kind == "union":
            ms = ms[:1]
        items = [self.value(ft, fp, fd, off + fo, data, local, indent + 1) for _, ft, fp, fd, fo in ms]
        return self._braces(items, indent)

    @staticmethod
    def _braces(items, indent):
        flat = "{ " + ", ".join(items) + " }"
        if len(flat) + indent * 4 <= 96 and "\n" not in flat:
            return flat
        pad = "    " * (indent + 1)
        lines, cur = [], ""
        for it in items:
            if "\n" in it or len(cur) + len(it) + 2 > 92 - indent * 4:
                if cur:
                    lines.append(cur.rstrip())
                cur = ""
            if "\n" in it:
                lines.append(it + ",")
            else:
                cur += it + ", "
        if cur:
            lines.append(cur.rstrip())
        return "{\n" + "\n".join(pad + l for l in lines) + "\n" + "    " * indent + "}"

    def word_fields(self, tname, ptr, dims, off, out):
        """Add the offsets of the 4-byte fields (words and pointers) of a type at off to out."""
        if dims:
            sz = self.types.layout(tname, ptr)[0]
            for d in dims[1:]:
                sz *= d
            for i in range(dims[0]):
                self.word_fields(tname, ptr, dims[1:], off + i * sz, out)
            return
        r = None if ptr else self.types.resolve(tname)
        if ptr or r[0] == "scalar":
            if ptr or r[1] == 4:
                out.add(off)
            return
        ms, kind = self.types.members(tname)
        for _, ft, fp, fd, fo in (ms[:1] if kind == "union" else ms):
            self.word_fields(ft, fp, fd, off + fo, out)

    def covered(self, tname, ptr, dims, off, out):
        """Mark the bytes a type's leaves cover (padding stays unmarked)."""
        if dims:
            sz = self.types.layout(tname, ptr)[0]
            for d in dims[1:]:
                sz *= d
            for i in range(dims[0]):
                self.covered(tname, ptr, dims[1:], off + i * sz, out)
            return
        r = None if ptr else self.types.resolve(tname)
        if ptr or r[0] == "scalar":
            out.update(range(off, off + (4 if ptr else r[1])))
            return
        ms, kind = self.types.members(tname)
        for _, ft, fp, fd, fo in (ms[:1] if kind == "union" else ms):
            self.covered(ft, fp, fd, off + fo, out)

    def align_of(self, name):
        """The alignment cc1 gives an object: mips.h DATA_ALIGNMENT word-aligns arrays, structs and
        unions; a declared scalar keeps its own (an undeclared object is written as an array)."""
        decl = self.decls.get(name)
        if decl is None:
            return 4
        tname, ptr, dims = decl
        if ptr or dims or tname not in SCALARS:
            return 4
        return SCALARS[tname][0]

    def define(self, name, va, size, kinds, local, next_align=4):
        """One definition, or two when the declared type is shorter than the object: the rest
        is written as an object of its own, D_<address>. next_align is the alignment of the
        object after this one: a gap it doesn't explain as padding is a rest."""
        self.base_rom = self.rom(va)
        data = self.bin[self.rom(va):self.rom(va) + size]
        decl = self.decls.get(name)
        if decl is None:
            el = "s32" if "word" in kinds or not kinds else "s16" if "short" in kinds else "u8"
            if size % SCALARS[el][0] or size < SCALARS[el][0]:
                # a rest smaller than (or not a multiple of) the element: halfwords or bytes
                el = "s16" if size % 2 == 0 and va % 2 == 0 else "u8"
            esz = SCALARS[el][0]
            # an array is word-aligned (mips.h DATA_ALIGNMENT), so one element off a word is a scalar
            decl = (el, 0, [] if va % 4 and size == esz else [None])
        tname, ptr, dims = decl
        lo = self.types.layout(tname, ptr)
        if lo is None:
            raise ValueError(f"{name}: no layout for type {tname}")
        if dims and dims[0] is None:
            dims = [size // (lo[0] * _prod(dims[1:]))] + dims[1:]
        total = lo[0] * _prod(dims)
        body = data[:total]
        if total > size:
            raise ValueError(f"{name}: type {tname}{''.join(f'[{d}]' for d in dims)} is {total} bytes, the object {size}")
        rest = ""
        padded = (va + total + next_align - 1) & ~(next_align - 1)
        if any(data[total:]) or (va + size) - ((va + total + 3) & ~3) >= 4 or padded < va + size:
            # the rest is another object (cc1 pads only to the next object's alignment), even if
            # it is zero: written as D_<address> so the layout holds
            # the next word address after the object (not va + an aligned size: an s16 at 2 mod 4,
            # D_800C1F16 in Arkenoid, puts its rest at the word right after it)
            rva = (va + total + 3) & ~3
            rkinds = kinds - {"asciz"} or {"word"}
            if rva - va >= size:
                # less than a word is left: a halfword right after (the s16 pair D_800C52E8), or
                # bytes when it starts at an odd address
                rva, rkinds = va + total, {"short"} if (va + total) % 2 == 0 else {"byte"}
            if rva <= va or rva >= va + size:
                raise ValueError(f"{name}: no room for the rest after {tname} ({total} of {size} bytes)")
            rest = "\n" + self.define(f"D_{rva:08X}", rva, size - (rva - va), rkinds, local, next_align)
            size = rva - va
            self.base_rom = self.rom(va)
        ctype = ("const " if False else "") + tname + " " + "*" * ptr
        decl_txt = f"{ctype}{name}{''.join(f'[{d}]' for d in dims)}"
        if tname == FN:
            # a function pointer's definition is its declaration's text (sized from the slot)
            decl_txt = re.sub(r"\[\s*\]", f"[{dims[0]}]" if dims else "", FN_DECLS[name])
        # an address in the overlay where the type has no 4-byte field is written as plain bytes or
        # halves, so a shifted build would keep the old address (CharCards' anim, in a u8[0x20])
        words = set()
        self.word_fields(tname, ptr, dims, 0, words)
        for o in range(0, total - 3, 4):
            v = int.from_bytes(body[o:o + 4], "little")
            if self.vram <= v < self.end and o not in words:
                sys.stderr.write(f"warning: {name}+{o:#x} holds {v:#010x}, an address in the overlay, but "
                                 f"{name}'s type ({tname}) has no 4-byte field there: give it a pointer field\n")
        if not any(body):
            init = "0" if not dims and (ptr or self.types.resolve(tname)[0] == "scalar") else "{0}"
        else:
            cov = set()
            self.covered(tname, ptr, dims, 0, cov)
            stray = [i for i in range(total) if body[i] and i not in cov]
            if stray:
                raise ValueError(f"{name}: nonzero bytes in padding at +{stray[0]:#x}")
            init = self.value(tname, ptr, dims, 0, body, local, 0)
        pad = size - total
        note = f"  /* +{pad} bytes of padding to the next object */" if pad >= 4 else ""
        return f"{decl_txt} = {init};{note}{rest}"

    def strings(self, sym):
        """const char arrays for an INCLUDE_RODATA block of strings."""
        path = next(p for p in glob.glob(f"asm/nonmatchings/overlays/{self.ovl}/**/{sym}.s", recursive=True))
        out, local = [], []
        for l in open(path, encoding="utf-8"):
            m = re.match(r'\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s*\*/\s*\.asciz\s+"(.*)"\s*$', l)
            if m:
                va = int(m.group(2), 16)
                name = sym if not out else f"D_{va:08X}"
                local.append((va, name))
                out.append(f'const char {name}[] = "{m.group(3)}";')
        return out, local


def _prod(xs):
    n = 1
    for x in xs:
        n *= x
    return n


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("ovl")
    ap.add_argument("--rodata", help="an INCLUDE_RODATA block of strings to write as const char arrays")
    ap.add_argument("--out")
    a = ap.parse_args()
    c = Converter(a.ovl)
    lines, local = [], []
    if a.rodata:
        rl, local = c.strings(a.rodata)
        lines += ["/* rodata: the strings (const arrays keep their retail order; see data2c.py) */"] + rl + [""]
    files = sorted(glob.glob(f"asm/data/overlays/{a.ovl}/*.data.s"))
    objs = []
    for p in files:
        objs += c.objects(p)
    local += [(va, n) for n, va, _, _ in objs]
    errors = 0
    lines.append("/* data, in address order */")
    for i, (name, va, size, kinds) in enumerate(objs):
        try:
            nxt = c.align_of(objs[i + 1][0]) if i + 1 < len(objs) else 4
            lines.append(c.define(name, va, size, kinds, local, nxt))
        except ValueError as e:
            errors += 1
            lines.append(f"#error {e}")
    if c.externs:
        lines = ["/* the exe's data the tables point at, not declared in the headers yet */"] + \
            [f"extern u8 {n}[];" for n in sorted(c.externs)] + [""] + lines
    text = "\n".join(lines) + "\n"
    if a.out:
        open(a.out, "w", encoding="utf-8").write(text)
    else:
        sys.stdout.write(text)
    if c.provide:
        sys.stderr.write("PROVIDE lines for configs/USA/undefined_syms_manual.txt:\n")
        for n, t in sorted(c.provide.items(), key=lambda x: x[1]):
            sys.stderr.write(f"PROVIDE({n} = 0x{t:08X});\n")
    sys.stderr.write(f"{len(objs)} objects, {errors} errors\n")


if __name__ == "__main__":
    main()
