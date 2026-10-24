"""Find an instruction pattern in functions' asm (decomp.py idiom).

A pattern is instructions separated by `;`, in splat's syntax (`sll $v0, $a3, 3`), where:
- a placeholder, an upper-case letter with an optional digit (`A`, `N`, `R2`), stands for one
  operand and binds to it: the same placeholder must be the same register or value each time,
  and two placeholders bound to registers must be different registers;
- `*` (or `_`) is any one operand, and `*` as the opcode any opcode;
- `...` is a gap of up to GAP instructions;
- `N(A)` is a memory operand (offset N, base register A); `%hi(*)` and `%lo(D_80*)` glob;
- registers may be written without `$` (`zero`, `v0`); numbers compare by value (`0x10` = 16).
Both sides are read in canonical form first: `addu A, B, $zero` (or `$zero` first, or `or`) is
`move A, B`, `addiu`/`ori A, $zero, N` is `li A, N`."""
import fnmatch
import re

REGS = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp",
        "ra"}
PLACEHOLDER = re.compile(r"[A-Z][0-9]?")
MEM = re.compile(r"(.*)\((\$?\w+)\)")


def operands(text):
    """Operands of one instruction's operand text, a memory operand as two: its offset and its
    base register marked `@` (`0x5C($v0)` -> `0x5C`, `@$v0`; `%lo(D_1)($v0)` likewise)."""
    out = []
    for o in [x.strip() for x in re.split(r",(?![^(]*\))", text) if x.strip()]:
        m = MEM.fullmatch(o)
        if m and (not o.startswith("%") or o.count("(") > 1):
            out += [m.group(1) or "0", "@" + reg(m.group(2))]
        else:
            out.append(reg(o))
    return out


def reg(o):
    return "$" + o if o in REGS else o


def canon(op, args):
    """(op, args) in canonical form: the moves and constant loads written one way."""
    if op in ("addu", "or") and len(args) == 3 and "$zero" in args[1:]:
        return "move", [args[0], args[2] if args[1] == "$zero" else args[1]]
    if op in ("addiu", "ori") and len(args) == 3 and args[1] == "$zero":
        return "li", [args[0], args[2]]
    if op == "sll" and args == ["$zero", "$zero", "0"]:
        return "nop", []
    return op, args


def insn(text):
    """One instruction's text (`addiu $v0, $v0, 0x44`) as canonical (op, operands)."""
    op, _, rest = text.strip().partition(" ")
    return canon(op, operands(rest))


def pattern(text):
    """A pattern as a list of (op, operands) steps, a gap as None."""
    out = []
    for part in [p.strip() for p in text.split(";") if p.strip()]:
        out.append(None if part == "..." else insn(part))
    if not out or out[0] is None or out[-1] is None:
        raise ValueError("a pattern starts and ends with an instruction")
    return out


def number(x):
    try:
        return int(x, 0)
    except ValueError:
        return None


def is_reg(a):
    return a.startswith("$")


def atom(p, a, binds):
    """Does pattern operand P match operand A under BINDS (extended in place on a match)?"""
    if p.startswith("@") != a.startswith("@"):
        return False
    p, a = p[p.startswith("@"):], a[a.startswith("@"):]
    if p in ("*", "_"):
        return True
    if PLACEHOLDER.fullmatch(p):
        if p in binds:
            return binds[p] == a
        if is_reg(a) and any(v == a for v in binds.values()):
            return False                    # another placeholder has this register
        binds[p] = a
        return True
    if number(p) is not None:
        return number(a) == number(p)
    return fnmatch.fnmatchcase(a, p) if "*" in p else a == p


def step(st, ins, binds):
    op, args = st
    if op != "*" and op != ins[0]:
        return None
    if len(args) != len(ins[1]):
        return None
    b = dict(binds)
    return b if all(atom(p, a, b) for p, a in zip(args, ins[1])) else None


def match_at(pat, code, i, gap, binds=None, k=0):
    """(end index, binds) when PAT matches CODE (canonical insns) from index I, else None."""
    binds = {} if binds is None else binds
    if k == len(pat):
        return i, binds
    if pat[k] is None:                       # a gap: try the shortest first
        for skip in range(gap + 1):
            r = match_at(pat, code, i + skip, gap, binds, k + 1)
            if r:
                return r
        return None
    if i >= len(code):
        return None
    b = step(pat[k], code[i], binds)
    return match_at(pat, code, i + 1, gap, b, k + 1) if b is not None else None


def find(pat, code, gap=8):
    """[(start, end, binds)] for every place PAT matches in CODE, not overlapping."""
    out, i = [], 0
    while i < len(code):
        r = match_at(pat, code, i, gap)
        if r:
            out.append((i, r[0], r[1]))
            i = r[0]
        else:
            i += 1
    return out
