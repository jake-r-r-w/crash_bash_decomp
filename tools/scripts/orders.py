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
Block statements ({ ... }, if, loops) are refused: give a range inside one block.

With free=True (`--free-memory`) memory is compared by place instead: a global, or a chain of
fields and indexes from a name (`e->unk10.x`, `D_800C53DC[x][z].unkA`). Two places meet when they
have the same root and no field tells them apart (indexes may be equal); different roots never
meet, so stores to different globals, fields or pointers may swap, which C allows only when they
don't alias. A unary `*` is an unknown place that meets every other."""
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
INDEX = r"\[(?:[^\[\]]|\[[^\[\]]*\])*\]"                # one [...], with one level of nesting
CHAIN = re.compile(r"(&\s*)?\b([A-Za-z_]\w*)((?:\s*(?:->|\.)\s*[A-Za-z_]\w*|\s*" + INDEX + r")*)")
SEL = re.compile(r"(?:->|\.)\s*([A-Za-z_]\w*)|\[((?:[^\[\]]|\[[^\[\]]*\])*)\]")
CAST = re.compile(r"\s*\*+\s*\)")                     # after a type name in a cast: `s32 *)`
TYPES = {"s8", "s16", "s32", "s64", "u8", "u16", "u32", "u64", "char", "short", "int", "long",
         "float", "double", "void", "bool"}
STAR = "*"                                            # an unknown place: behind a unary *, a cast


COMMENT = re.compile(r"/\*.*?\*/", re.S)
TRAILING = re.compile(r"[ \t]*/\*.*?\*/", re.S)      # a comment after a `;` on the same line


def code(stmt):
    """STMT without its comments."""
    return COMMENT.sub(" ", stmt).strip()


def statements(text):
    """The statements of TEXT, each with its trailing `;` (and a comment after it on the same
    line); refuses blocks and control flow."""
    out, depth, cur, k = [], 0, "", 0
    while k < len(text):
        m = COMMENT.match(text, k)
        if m:                                         # braces and `;` in comments don't count
            cur += m.group(0)
            k = m.end()
            continue
        ch = text[k]
        k += 1
        if ch in "{}":
            raise ValueError("the range holds a block: give lines inside one block")
        depth += ch in "(["
        depth -= ch in ")]"
        cur += ch
        if ch == ";" and depth == 0:
            m = TRAILING.match(text, k)
            if m:
                cur += m.group(0)
                k = m.end()
            out.append(cur.strip())
            cur = ""
    if code(cur):
        raise ValueError(f"the range ends inside a statement: {cur.strip()[:60]!r}")
    for s in map(code, out):
        w = NAME.match(s)
        if w and w.group(0) in KEYWORDS - {"sizeof"} or re.match(r"[A-Za-z_]\w*\s*:", s):
            raise ValueError(f"control flow or a label in the range: {s[:60]!r}")
    return out


def effects(stmt, local):
    """(written vars, read vars, writes memory, reads memory, calls) of one statement; LOCAL is the
    set of the function's parameters and locals."""
    body = code(stmt).rstrip(";")
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


def place(text):
    """A place (root, ((kind, field name or index text), ...)) from an lvalue chain's TEXT, or
    STAR for anything else (`*p`, a cast)."""
    m = CHAIN.fullmatch(text.strip())
    if not m or m.group(1):
        return STAR
    sels = tuple(("f", f) if f else ("i", re.sub(r"\s+", "", i)) for f, i in SEL.findall(m.group(3)))
    return m.group(2), sels


def reads_in(text, local, out):
    """The memory places TEXT reads, into OUT: globals and chains, not variables, calls, casts'
    type names or `&` addresses; the index expressions inside a chain too."""
    if DEREF.search(text):
        out.add(STAR)
    for m in CHAIN.finditer(text):
        amp, root, rest = m.groups()
        after = text[m.end():]
        for _, i in SEL.findall(rest):
            if i:
                reads_in(i, local, out)
        if root in KEYWORDS or after.lstrip().startswith("("):
            continue                                  # a keyword, or a call (calls keep order anyway)
        if not rest and (CAST.match(after) or root in TYPES and re.match(r"\s*\)", after)):
            continue                                  # a cast's type name
        if amp or root in local and not rest:
            continue                                  # an address, or a variable
        out.add(place(m.group(0)[len(amp or ""):]))


def places(stmt, local):
    """(written places, read places) of memory in one statement, for free=True."""
    body = code(stmt).rstrip(";").strip()
    wr, rd = set(), set()
    m, s = ASSIGN.match(body), STORE.search(body)
    step = None if m or s else re.fullmatch(r"(?:\+\+|--)\s*(.+)|(.+?)\s*(?:\+\+|--)", body)
    if m:
        lhs, op, rhs = m.group(1), m.group(2), body[m.end():]
    elif s:
        lhs, op, rhs = body[:s.start()], s.group(1), body[s.end():]
    elif step:
        lhs, op, rhs = step.group(1) or step.group(2), "+=", ""
    else:
        lhs, op, rhs = "", "=", body
    if "++" in rhs or "--" in rhs:
        wr.add(STAR)                                  # `x = p->n++`: a write not placed here
    lhs = lhs.strip()
    if lhs:
        p = place(lhs)
        if p == STAR:
            wr.add(STAR)
            reads_in(lhs.lstrip("*"), local, rd)
        elif p[0] not in local or p[1]:
            wr.add(p)
            if op != "=":
                rd.add(p)
            for kind, i in p[1]:
                if kind == "i":
                    reads_in(i, local, rd)
    reads_in(rhs, local, rd)
    return wr, rd


def overlap(a, b):
    """Can places A and B be the same memory?"""
    if STAR in (a, b):
        return True
    (ra, sa), (rb, sb) = a, b
    if ra != rb:
        return False
    for x, y in zip(sa, sb):
        if x[0] != y[0]:
            return True
        if x[0] == "f" and x[1] != y[1]:
            return False
    return True


def must_precede(a, b, pa=None, pb=None):
    """Must statement A (effects; places PA with free=True) stay before B when A comes first?"""
    wa, ra, mwa, mra, ca = a
    wb, rb, mwb, mrb, cb = b
    if ca or cb or wa & (rb | wb) or ra & wb:
        return True
    if pa is None:
        return bool((mwa and (mrb or mwb)) or (mra and mwb))
    (wma, rma), (wmb, rmb) = pa, pb
    return (any(overlap(x, y) for x in wma for y in wmb | rmb)
            or any(overlap(x, y) for x in rma for y in wmb))


def edges(stmts, local, free=False):
    eff = [effects(s, local) for s in stmts]
    pl = [places(s, local) for s in stmts] if free else [None] * len(stmts)
    return {j: {i for i in range(j) if must_precede(eff[i], eff[j], pl[i], pl[j])}
            for j in range(len(stmts))}


def orders(stmts, local, count, seed=0, free=False):
    """Up to COUNT distinct orders (lists of indices) of STMTS that keep every edge; the source
    order first."""
    dep = edges(stmts, local, free)
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
