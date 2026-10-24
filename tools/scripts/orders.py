"""Random dependency-respecting orders of a run of C statements (decomp.py orders).

sched1 breaks most ties by luid (the later insn in the RTL stays later), and the RTL follows the
source, so the order of independent statements in a block can decide instruction order and, through
the lives it gives each pseudo, local-alloc's registers. This splits a line range of one function
into statements, works out which pairs must keep their order, and draws random orders that keep
them; decomp.py compiles and ranks them (try --file batches).

Two statements keep their order when one writes a variable the other reads or writes, when one
writes memory and the other reads or writes it, or when either calls a function. A variable is a
parameter or local of the function; any other name is a global, so it is memory. A write is an
assignment, a compound assignment or ++/--; an assignment through `->`, `[` or `*` writes memory.
Block statements ({ ... }, if, loops) are refused: give a range inside one block."""
import random
import re

KEYWORDS = {"if", "else", "while", "for", "do", "switch", "case", "default", "return", "goto", "break",
            "continue", "sizeof", "struct", "union", "enum", "const", "volatile", "unsigned", "signed"}
ASSIGN = re.compile(r"\s*([A-Za-z_]\w*)\s*(=(?!=)|\+=|-=|\*=|/=|%=|&=|\|=|\^=|<<=|>>=)")
INCDEC = re.compile(r"(?:\+\+|--)\s*([A-Za-z_]\w*)|([A-Za-z_]\w*)\s*(?:\+\+|--)")
CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
NAME = re.compile(r"\b[A-Za-z_]\w*")                 # \b: not the x of 0xFFF
STORE = re.compile(r"(?<![=!<>])(<<=|>>=|[+\-*/%&|^]?=)(?!=)")
DEREF = re.compile(r"(^|[=(,+\-*/%&|^!~<>?:])\s*\*")   # a unary * (after an operator), not a product


def statements(text):
    """The statements of TEXT, each with its trailing `;`; refuses blocks and control flow."""
    out, depth, cur = [], 0, ""
    for ch in text:
        if ch in "{}":
            raise ValueError("the range holds a block: give lines inside one block")
        depth += ch in "(["
        depth -= ch in ")]"
        cur += ch
        if ch == ";" and depth == 0:
            out.append(cur.strip())
            cur = ""
    if cur.strip():
        raise ValueError(f"the range ends inside a statement: {cur.strip()[:60]!r}")
    for s in out:
        w = NAME.match(s)
        if w and w.group(0) in KEYWORDS - {"sizeof"} or re.match(r"[A-Za-z_]\w*\s*:", s):
            raise ValueError(f"control flow or a label in the range: {s[:60]!r}")
    return out


def effects(stmt, local):
    """(written vars, read vars, writes memory, reads memory, calls) of one statement; LOCAL is the
    set of the function's parameters and locals."""
    body = stmt.rstrip(";")
    calls = [c for c in CALL.findall(body) if c not in KEYWORDS]
    m = ASSIGN.match(body)
    wr, rd = set(), set()
    mem_w = False
    rhs = body
    if m:
        name, op = m.group(1), m.group(2)
        rhs = body[m.end():]
        if name in local:
            wr.add(name)
            if op != "=":
                rd.add(name)
        else:
            mem_w = True                     # a global
            if op != "=":
                rd.add(name)
    elif STORE.search(body):
        mem_w = True                         # p->f = ..., a[i] += ..., *p = ...
        s = STORE.search(body)
        lhs, rhs = body[:s.start()], body[s.end():]
        rd |= {n for n in NAME.findall(lhs) if n in local}
    for a, b in INCDEC.findall(body):
        n = a or b
        (wr if n in local else rd).add(n)
        rd.add(n)
        if n not in local:
            mem_w = True
    names = set(NAME.findall(rhs)) - KEYWORDS - set(calls)
    rd |= {n for n in names if n in local}
    mem_r = bool(names - local) or "->" in rhs or "[" in rhs or bool(DEREF.search(rhs))
    return wr, rd, mem_w, mem_r, bool(calls)


def must_precede(a, b):
    """Must statement A (effects) stay before B when A comes first?"""
    wa, ra, mwa, mra, ca = a
    wb, rb, mwb, mrb, cb = b
    return bool(ca or cb or wa & (rb | wb) or ra & wb or (mwa and (mrb or mwb)) or (mra and mwb))


def edges(stmts, local):
    eff = [effects(s, local) for s in stmts]
    return {j: {i for i in range(j) if must_precede(eff[i], eff[j])} for j in range(len(stmts))}


def orders(stmts, local, count, seed=0):
    """Up to COUNT distinct orders (lists of indices) of STMTS that keep every edge; the source
    order first."""
    dep = edges(stmts, local)
    rng = random.Random(seed)
    out, seen = [list(range(len(stmts)))], {tuple(range(len(stmts)))}
    tries = 0
    while len(out) < count and tries < count * 20:
        tries += 1
        done, order = set(), []
        while len(order) < len(stmts):
            ready = [j for j in range(len(stmts)) if j not in done and dep[j] <= done]
            j = rng.choice(ready)
            done.add(j)
            order.append(j)
        if tuple(order) not in seen:
            seen.add(tuple(order))
            out.append(order)
    return out
