"""Survey the field accesses of a struct across all asm, for recovering layouts.

usage: field_survey.py ROOT SYMBOL STRIDE [--sub] [--ret FUNC[:A1]] [--base-load OFF]

ROOT is an asm tree with every function (`make objdiff-config` writes one to
build/objdiff/asm/nonmatchings). Linear per-function register tracking: a
register holding &SYMBOL[i] (+off) is followed, and each load/store through it
is counted by offset (mod STRIDE) and instruction, so the widths and
signedness of the fields show.

  --sub             words loaded from a field are pointers too; their
                    accesses are listed per field
  --ret FUNC[:A1]   the struct pointer is what FUNC returns (only when called
                    with a1 == A1); pass any SYMBOL and a large STRIDE
  --base-load OFF   the struct pointer is any word loaded from OFF(reg)

e.g. Heroes: `ROOT Heroes 0x6C --sub`; EXTmotion: `ROOT - 0x10000 --ret
FindExtension:0x800`; subengine: `ROOT - 0x10000 --base-load 0x6C`.
"""
import os
import re
import sys
from collections import defaultdict

root, sym, stride = sys.argv[1], sys.argv[2], int(sys.argv[3], 0)
sub = "--sub" in sys.argv
ret = None
if "--ret" in sys.argv:
    r = sys.argv[sys.argv.index("--ret") + 1].split(":")
    ret = (r[0], int(r[1], 0) if len(r) > 1 else None)
base_load = None
if "--base-load" in sys.argv:
    base_load = int(sys.argv[sys.argv.index("--base-load") + 1], 0)

INS = re.compile(r"^\s*/\*.*?\*/\s+(\S+)\s*(.*)$")
MEM = re.compile(r"^(\$\w+), (.+)\((\$\w+)\)$")
LO = re.compile(r"%lo\(" + re.escape(sym) + r"(?: \+ (0x[0-9A-Fa-f]+|\d+))?\)")
HI = re.compile(r"%hi\(" + re.escape(sym) + r"(?: \+ (0x[0-9A-Fa-f]+|\d+))?\)")
LOADS = {"lb", "lbu", "lh", "lhu", "lw", "lwl", "lwr"}
STORES = {"sb", "sh", "sw", "swl", "swr"}

acc = defaultdict(lambda: defaultdict(set))  # off -> op -> funcs
subacc = defaultdict(lambda: defaultdict(set))


def num(s):
    s = s.strip()
    neg = s.startswith("-")
    v = int(s.lstrip("-"), 0)
    return -v if neg else v


for dp, _, fs in os.walk(root):
    for fn in fs:
        if not fn.endswith(".s"):
            continue
        name = fn[:-2]
        t = {}  # reg -> ("H", off) | ("hi", off) | ("F", field)
        a1 = None
        pending = None
        for line in open(os.path.join(dp, fn), errors="replace"):
            if line.strip().startswith("glabel"):
                t = {}
            m = INS.match(line)
            if not m:
                continue
            op, args = m.group(1), m.group(2).strip()
            if pending is not None:
                # the jal's delay slot has run
                if op == "addiu" and args.startswith("$a1, $zero,"):
                    pending[1] = num(args.split(",")[2])
                hit, a1v = pending
                if ret[1] is not None and a1v != ret[1]:
                    hit = False
                pending = None
                for r in ("$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1",
                          "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$at"):
                    t.pop(r, None)
                if hit:
                    t["$v0"] = ("H", 0)
            if ret and op == "jal":
                hit = args == ret[0]
                pending = [hit, a1]
                continue
            if op == "addiu" and args.startswith("$a1, $zero,"):
                a1 = num(args.split(",")[2])
                if pending is not None:
                    pending[1] = a1
            elif args.startswith("$a1,"):
                a1 = None
            if base_load is not None and op == "lw":
                mm = MEM.match(args)
                if mm and not mm.group(2).startswith("%") and num(mm.group(2)) == base_load \
                        and mm.group(3) not in ("$sp",) and t.get(mm.group(3)) is None:
                    t[mm.group(1)] = ("H", 0)
                    continue
            a = [x.strip() for x in args.split(",")] if args else []
            if op in LOADS or op in STORES:
                mm = MEM.match(args)
                if not mm:
                    continue
                rt, offs, base = mm.groups()
                lo = LO.search(offs)
                kind = t.get(base)
                field = None
                if lo and kind and kind[0] == "hi":
                    field = kind[1] + (int(lo.group(1), 0) if lo.group(1) else 0)
                elif kind and kind[0] == "H" and not offs.startswith("%"):
                    field = kind[1] + num(offs)
                elif kind and kind[0] == "F" and sub and not offs.startswith("%"):
                    subacc[kind[1]][(num(offs), op)].add(name)
                if field is not None:
                    acc[field % stride][op].add(name)
                if op in LOADS:
                    if field is not None and op == "lw" and sub:
                        t[rt] = ("F", field % stride)
                    else:
                        t.pop(rt, None)
                continue
            if not a:
                continue
            rd = a[0]
            if op == "lui":
                h = HI.search(args)
                if h:
                    t[rd] = ("hi", int(h.group(1), 0) if h.group(1) else 0)
                else:
                    t.pop(rd, None)
            elif op == "addiu" and len(a) == 3:
                lo = LO.search(a[2])
                src = t.get(a[1])
                if lo and src and src[0] == "hi":
                    t[rd] = ("H", int(lo.group(1), 0) if lo.group(1) else 0)
                elif src and src[0] == "H" and not a[2].startswith("%"):
                    imm = num(a[2])
                    t[rd] = ("H", src[1] if imm % stride == 0 else src[1] + imm)
                else:
                    t.pop(rd, None)
            elif op in ("addu", "or") and len(a) == 3:
                s1, s2 = t.get(a[1]), t.get(a[2])
                s = s1 if s1 and s1[0] in ("H", "hi") else s2 if s2 and s2[0] in ("H", "hi") else None
                if s and (s1 is None or s2 is None):
                    t[rd] = s
                elif s1 and a[2] == "$zero":
                    t[rd] = s1
                elif s2 and a[1] == "$zero":
                    t[rd] = s2
                else:
                    t.pop(rd, None)
            elif op == "move" and len(a) == 2:
                if a[1] in t:
                    t[rd] = t[a[1]]
                else:
                    t.pop(rd, None)
            elif op in ("jal", "jalr"):
                for r in ("$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1",
                          "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9", "$at"):
                    t.pop(r, None)
            elif rd.startswith("$") and op not in ("beq", "bne", "beqz", "bnez", "blez",
                                                   "bgtz", "bltz", "bgez", "j", "jr", "b",
                                                   "mult", "multu", "div", "divu", "mtlo", "mthi"):
                t.pop(rd, None)

for off in sorted(acc):
    ops = acc[off]
    fns = set().union(*ops.values())
    print("+0x%02X  %-40s %4d funcs  e.g. %s" % (
        off, " ".join("%s:%d" % (o, len(ops[o])) for o in sorted(ops)), len(fns), sorted(fns)[0]))
if sub:
    print("\nsub-object accesses (field: suboff op count)")
    for f in sorted(subacc):
        items = sorted(subacc[f].items())
        print("+0x%02X -> " % f + ", ".join("%s+0x%X:%d" % (o, so, len(v)) for (so, o), v in items))
