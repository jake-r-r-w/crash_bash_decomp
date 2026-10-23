#!/usr/bin/env python3
"""Recover SN Systems .SYM debug records from leftover memory in a disc file.

The prototype CRASHBSH.DAT files end in padding that holds pages of the build
machine's memory, among them pieces of the linker's .SYM output. The pages are
not contiguous, so this scans every offset for a run of valid records.

SYM record: u32 value, u8 tag, payload. Tags:
  01/02 label            pstr name
  80    line + 1         (none)
  82/84 line + n         u8 n / u16 n
  86    line = n         u32 n
  88    line = n in file u32 n, pstr file
  8A    end of lines     (none)
  8C    function start   u16 fp, u32 fsize, u16 ret, u32 mask, i32 maskoff,
                         u32 line, pstr file, pstr name
  8E/90/92 fn end/block  u32 line
  94    def              u16 class, u16 type, u32 size, pstr name
  96    def2             u16 class, u16 type, u32 size, u16 ndims,
                         u32 dims[ndims], pstr tag, pstr name
The value of a line, function or block record (tags 80..92) is a code address.

    tools/dk python3 -I tools/scripts/sym_scan.py FILE [--from OFF] [--to OFF]
        [--raw] [--c] [--lines]

--raw lists every record; --c prints the enums, structs, unions and typedefs
as C (the default prints both a summary and the C); --lines prints the line
table, one "address file:line" per line record.
"""
import argparse
import re
import struct

NAME = re.compile(rb"[A-Za-z_.$][A-Za-z0-9_.$]*\Z")
BASIC = ["null", "void", "char", "short", "int", "long", "float", "double",
         "struct", "union", "enum", "moe", "unsigned char", "unsigned short",
         "unsigned int", "unsigned long"]
C_AUTO, C_EXT, C_STAT, C_REG, C_ARG, C_MOS, C_STRTAG, C_MOU, C_UNTAG = 1, 2, 3, 4, 9, 8, 10, 11, 12
C_TPDEF, C_ENTAG, C_MOE, C_FIELD, C_EOS = 13, 15, 16, 18, 102


def pstr(d, p):
    if p >= len(d):
        return None, p
    n = d[p]
    s = d[p + 1:p + 1 + n]
    if n == 0 or len(s) != n or not NAME.match(s):
        return None, p
    return s.decode(), p + 1 + n


def path(d, p):
    """A file name: holds backslashes and colons, so any printable text."""
    n = d[p]
    s = d[p + 1:p + 1 + n]
    if n == 0 or len(s) != n or not all(0x20 <= c < 0x7F for c in s):
        return None, p
    return s.decode("ascii"), p + 1 + n


def code_addr(v):
    return 0x80000000 <= v < 0x80800000


def rec(d, p):
    """Parse one record at p. Returns (record, next) or None."""
    if p + 5 > len(d):
        return None
    val, tag = struct.unpack_from("<IB", d, p)
    q = p + 5
    if 0x80 <= tag <= 0x92 and not code_addr(val):
        return None
    try:
        if tag in (1, 2):
            name, q2 = pstr(d, q)
            if name is None:
                return None
            return ("label", val, name), q2
        if tag in (0x80, 0x8A):
            return ("line", val, "inc" if tag == 0x80 else "end", 1 if tag == 0x80 else 0), q
        if tag in (0x82, 0x84, 0x86):
            fmt, n = {0x82: ("<B", 1), 0x84: ("<H", 2), 0x86: ("<I", 4)}[tag]
            (v,) = struct.unpack_from(fmt, d, q)
            return ("line", val, "set" if tag == 0x86 else "inc", v), q + n
        if tag == 0x88:
            (v,) = struct.unpack_from("<I", d, q)
            f, q = path(d, q + 4)
            if f is None:
                return None
            return ("line", val, "file", v, f), q
        if tag == 0x8C:
            fp, fsize, ret, mask, moff, line = struct.unpack_from("<HIHIiI", d, q)
            q += 20
            f, q = path(d, q)
            if f is None:
                return None
            name, q = pstr(d, q)
            if name is None or fp > 31 or ret > 31:
                return None
            return ("func", val, name, fsize, line, f), q
        if tag in (0x8E, 0x90, 0x92):
            (line,) = struct.unpack_from("<I", d, q)
            return ({0x8E: "fend", 0x90: "bbeg", 0x92: "bend"}[tag], val, line), q + 4
        if tag in (0x94, 0x96):
            cls, typ, size = struct.unpack_from("<HHI", d, q)
            q += 8
            dims, tagname = [], None
            if tag == 0x96:
                (nd,) = struct.unpack_from("<H", d, q)
                q += 2
                if nd > 4:
                    return None
                dims = list(struct.unpack_from("<%dI" % nd, d, q))
                q += 4 * nd
                if d[q] == 0:  # basic-type arrays have an empty tag
                    tagname, q = "", q + 1
                else:
                    tagname, q = pstr(d, q)
                    if tagname is None:
                        return None
            name, q = pstr(d, q)
            if name is None or cls not in (1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 15, 16, 18, 102):
                return None
            return ("def", val, cls, typ, size, dims, tagname, name), q
    except (struct.error, IndexError, UnicodeDecodeError):
        return None
    return None


def scan(d, lo, hi, minrun=3):
    """Yield (offset, record) for every chain of at least minrun records."""
    p = lo
    while p < hi:
        chain, q = [], p
        while q < hi:
            r = rec(d, q)
            if r is None:
                break
            chain.append((q, r[0]))
            q = r[1]
        if len(chain) >= minrun:
            yield from chain
            p = q
        else:
            p += 1


def records(d, lo=0, hi=None):
    """Every record in d[lo:hi] that is part of a chain, as (offset, record)."""
    return list(scan(d, lo, len(d) if hi is None else hi))


def lines(recs):
    """The line table: (address, file, line) for each line record, the file and line
    carried from the last function start or file record."""
    out, f, n = [], None, None
    for _, r in recs:
        if r[0] == "func":
            f, n = r[5], r[4]
        elif r[0] == "line":
            kind = r[2]
            if kind == "file":
                f, n = r[4], r[3]
            elif kind == "set":
                n = r[3]
            elif kind == "inc" and n is not None:
                n += r[3]
            if kind != "end" and n is not None:
                out.append((r[1], f, n))
    return out


# Anonymous tags (._N) mapped to the typedef that names them; filled by to_c.
ALIAS = {}


def ctype(typ, tagname, dims):
    base = typ & 0xF
    s = BASIC[base]
    if base in (8, 9, 10) and tagname:
        s = ALIAS.get(tagname) or {8: "struct ", 9: "union ", 10: "enum "}[base] + tagname
    derived = typ >> 4
    mods = []
    while derived:
        mods.append(derived & 3)
        derived >>= 2
    return s, mods, dims


def decl(name, typ, tagname, dims):
    s, mods, dims = ctype(typ, tagname, dims)
    out, di = name, 0
    for m in mods:
        if m == 1:
            out = "*" + out
        elif m == 2:
            out = "(" + out + ")()" if out.startswith("*") else out + "()"
        elif m == 3:
            dim = dims[di] if di < len(dims) else ""
            di += 1
            out = ("(" + out + ")" if out.startswith("*") else out) + "[%s]" % dim
    return "%s %s" % (s, out)


def to_c(recs):
    for _, r in recs:
        if r[0] == "def" and r[2] == C_TPDEF and r[6] and r[6].startswith("."):
            ALIAS.setdefault(r[6], r[7])
    out, cur = [], None
    for off, r in recs:
        if r[0] != "def":
            continue
        _, val, cls, typ, size, dims, tagname, name = r
        if cls in (C_STRTAG, C_UNTAG, C_ENTAG):
            kind = {C_STRTAG: "struct", C_UNTAG: "union", C_ENTAG: "enum"}[cls]
            cur = kind
            label = "%s /* %s */" % (name, ALIAS[name]) if name in ALIAS else name
            out.append("%s %s { /* size 0x%X, at 0x%X */" % (kind, label, size, off))
        elif cls == C_EOS:
            out.append("};")
            cur = None
        elif cls == C_MOE:
            out.append("    %s = 0x%X," % (name, val))
        elif cls in (C_MOS, C_MOU):
            out.append("    /* 0x%02X */ %s;" % (val, decl(name, typ, tagname, dims)))
        elif cls == C_FIELD:
            out.append("    /* bit %d */ %s : %d;" % (val, decl(name, typ, tagname, dims), size))
        elif cls == C_TPDEF:
            if ALIAS.get(tagname) != name:
                out.append("typedef %s;" % decl(name, typ, tagname, dims))
        else:
            out.append("/* class %d @0x%X */ %s; /* value 0x%X size 0x%X */"
                       % (cls, off, decl(name, typ, tagname, dims), val, size))
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("file")
    ap.add_argument("--from", dest="lo", type=lambda s: int(s, 0), default=0)
    ap.add_argument("--to", dest="hi", type=lambda s: int(s, 0))
    ap.add_argument("--raw", action="store_true")
    ap.add_argument("--c", action="store_true")
    ap.add_argument("--lines", action="store_true")
    a = ap.parse_args()
    d = open(a.file, "rb").read()
    recs = records(d, a.lo, a.hi)
    if a.lines:
        for addr, f, n in lines(recs):
            print("0x%08X %s:%d" % (addr, f, n))
        return
    if a.raw or not a.c:
        for off, r in recs:
            if a.raw or r[0] in ("func", "label") or (r[0] == "def" and r[2] in (C_EXT, C_STAT)):
                print("0x%08X %s" % (off, r))
        kinds = {}
        for _, r in recs:
            kinds[r[0]] = kinds.get(r[0], 0) + 1
        print("records:", len(recs), kinds)
    if a.c or not a.raw:
        print(to_c(recs))


if __name__ == "__main__":
    main()
