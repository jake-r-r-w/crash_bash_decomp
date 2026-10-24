"""Follow one insn, or every insn that mentions one variable's pseudo, through cc1's pass dumps
(decomp.py trace).

cc1 writes NAME's RTL after each pass (PASSES, with their -d letters). An insn keeps its uid
from pass to pass until a pass deletes it: it becomes a NOTE_INSN_DELETED with the same uid, or
leaves the chain. loop.c's move_movables is the exception: it emits a copy of a hoisted insn
under a new uid before the loop and deletes the original, so an insn that vanishes while one
with its pattern appears under a uid the dump before didn't have is followed there ("moved").
When a pass deletes an insn that set a register, the insns that used that register in the dump
before and changed in this pass are where it went (combine's merges, cse's replacements).
From greg on, pseudos are hard registers: that renaming alone is not counted as a rewrite.

--var maps a C variable to its pseudo by declaration order: assign_parms and expand_decl give
each parameter and each register local the next pseudo number as cc1 meets its declaration,
and the dumps mark those reg/v (REG_USERVAR_P). Arrays, statics, externs, volatiles, locals
whose address is taken and locals the body never names get no pseudo, so they are skipped. A
struct or typedef'd value (not a pointer) may or may not get one; the mapping tries both.
"""
import re
from collections import namedtuple

from schedx import TOKEN, parse, show, section, HARD, code_of, kids, first_set

PASSES = [("rtl", "r"), ("jump", "j"), ("cse", "s"), ("addressof", "D"), ("loop", "L"),
          ("cse2", "t"), ("flow", "f"), ("combine", "c"), ("sched", "S"), ("lreg", "l"),
          ("greg", "g"), ("sched2", "R"), ("jump2", "J"), ("dbr", "d")]
ALLOCATED = {"greg", "sched2", "jump2", "dbr"}      # pseudos are hard registers from greg on
DUMP_FLAGS = " ".join("-d" + letter for _, letter in PASSES)

# an rtx code, its flags (/s, /v, ...) and a mode reload or sched2 may give an insn (insn:QI, insn:TI)
ENTRY = re.compile(r"^\((insn|call_insn|jump_insn|note|code_label|barrier)(?:/\w+)*(?::\w+)? (\d+) ", re.M)
Entry = namedtuple("Entry", "kind text regs dead pat")      # kind "note": text is the note kind


def regs_in(n, out, user=None):
    """Register numbers node N mentions into OUT; with USER, the reg/v ones into USER too."""
    if code_of(n) == "reg" and len(n) > 1 and isinstance(n[1], str) and n[1].isdigit():
        out.add(int(n[1]))
        if user is not None and "/v" in n[0].split(":")[0]:
            user.add(int(n[1]))
        return
    for x in kids(n):
        regs_in(x, out, user)


def notes_dead(n):
    """Registers a REG_NOTES chain says die (REG_DEAD) or are never used (REG_UNUSED)."""
    out = []
    while code_of(n) in ("expr_list", "insn_list") and len(n) > 1:
        if n[0].partition(":")[2] in ("REG_DEAD", "REG_UNUSED") and code_of(n[1]) == "reg":
            out.append(int(n[1][1]))
        n = n[2] if len(n) > 2 else None
    return out


def entries(sec, user=None):
    """({uid: Entry}, [uids in chain order]) of one dump section; USER collects reg/v pseudos."""
    ents, order = {}, []
    ms = list(ENTRY.finditer(sec))
    for k, m in enumerate(ms):
        uid = int(m.group(2))
        body = sec[m.start():ms[k + 1].start() if k + 1 < len(ms) else len(sec)]
        try:
            node = parse(TOKEN.findall(body))[0]
        except (IndexError, ValueError):
            continue
        if m.group(1) in ("note", "code_label", "barrier"):
            last = node[-1] if node and isinstance(node[-1], str) else ""
            ents[uid] = Entry(m.group(1) if m.group(1) != "note" else "note",
                              last if last.startswith("NOTE_INSN") else m.group(1), set(), [], None)
        else:
            regs = set()
            regs_in(node[4], regs, user)
            rest = [x for x in node[5:] if not (isinstance(x, str) and x.startswith("{"))]
            dead = notes_dead(rest[2]) if len(rest) > 2 else []
            ents[uid] = Entry(m.group(1), show(node[4]), regs, dead, node[4])
        order.append(uid)
    return ents, order


def load(dumps, name):
    """[(pass, {uid: Entry}, order)] for the dumps (pass, text) that have NAME, plus the reg/v
    pseudos of the first one (the rtl dump: user variables in declaration order)."""
    out, user = [], set()
    for p, text in dumps:
        sec = section(text, name)
        if sec:
            ents, order = entries(sec, user if not out else None)
            out.append((p, ents, order))
    return out, sorted(user)


def abstract(text):
    """TEXT with every register written R: allocation renames, it doesn't rewrite."""
    return re.sub(r"\$\w+|\br\d+\b", "R", text)


def dest_reg(e):
    s = first_set(e.pat) if e and e.pat is not None else None
    return int(s[1][1]) if s and len(s) > 1 and code_of(s[1]) == "reg" else None


def went_into(passes, k, prev):
    """The insns of pass K that changed from the dump before and used the register PREV set."""
    r = dest_reg(prev)
    if r is None or k == 0:
        return []
    _, ents, order = passes[k]
    before = passes[k - 1][1]
    return [u for u in order if ents[u].kind != "note" and u in before and before[u].kind != "note"
            and ents[u].text != before[u].text and r in before[u].regs]


def follow(passes, uid):
    """[(pass, uid, entry, event)] for insn UID: event "first", "same", "changed", "renamed"
    (allocation only), "moved" (to a new uid), "deleted" (a note or gone; entry is the last
    pattern, uid the insns it went into as a list)."""
    rows, prev, cur = [], None, uid
    for k, (p, ents, order) in enumerate(passes):
        e = ents.get(cur)
        if e is not None and e.kind != "note":
            if prev is None:
                ev = "first"
            elif e.text == prev.text:
                ev = "same"
            elif p in ALLOCATED and abstract(e.text) == abstract(prev.text):
                ev = "renamed"
            else:
                ev = "changed"
            rows.append((p, cur, e, ev))
            prev = e
            continue
        if prev is None:
            continue                            # not made yet: a later pass creates this uid
        before = passes[k - 1][1]
        moved = [u for u in order if u not in before and ents[u].kind != "note" and ents[u].text == prev.text]
        if moved:
            cur = moved[0]
            rows.append((p, cur, ents[cur], "moved"))
            prev = ents[cur]
            continue
        rows.append((p, went_into(passes, k, prev), prev, "deleted"))
        break
    return rows


def dead_names(e):
    return ", ".join(f"r{r}" if r >= 80 else f"${HARD[r]}" if r < 32 else f"r{r}" for r in e.dead)


def report_insn(passes, name, uid):
    rows = follow(passes, uid)
    if not rows:
        return f"insn {uid} is in no dump of {name}"
    out = [f"TRACE {name} insn {uid}, pass by pass (= unchanged; dies: REG_DEAD/REG_UNUSED notes):"]
    said = []
    for p, u, e, ev in rows:
        if ev == "deleted":
            into = ", ".join(f"{x} ({passes_entry(passes, p, x)})" for x in u)
            out.append(f"  {p:<9} deleted" + (f"; it went into {into}" if into else ""))
            said.append(f"{p} deletes it")
            continue
        dies = f"   dies: {dead_names(e)}" if e.dead else ""
        if ev == "same":
            out.append(f"  {p:<9} {u:>5}  ={dies}")
        else:
            tag = {"moved": f"  (moved: new uid {u})", "renamed": "  (allocated)"}.get(ev, "")
            out.append(f"  {p:<9} {u:>5}  {e.text}{dies}{tag}")
        if ev == "changed":
            said.append(f"{p} rewrites it")
        elif ev == "moved":
            said.append(f"{p} moves it to uid {u}")
    out.append(f"changes: {', then '.join(said) if said else 'none (kept to the end)'}")
    return "\n".join(out)


def passes_entry(passes, p, uid):
    ents = next(es for q, es, _ in passes if q == p)
    return ents[uid].text if uid in ents else "?"


def fate(passes, uid):
    """(rows of follow, a short account of what happened after the insn first appeared)."""
    rows = follow(passes, uid)
    said = []
    for p, u, e, ev in rows[1:]:
        if ev == "changed":
            said.append(f"{p}: {e.text}")
        elif ev == "moved":
            said.append(f"{p} moves it to {u}")
        elif ev == "deleted":
            said.append(f"{p} deletes it" + (f"; it went into {', '.join(map(str, u))}" if u else ""))
    return rows, "; ".join(said) or "kept"


def report_reg(passes, name, reg, label):
    """Every insn that mentions pseudo REG before allocation, by first appearance, with its fate."""
    out = [f"TRACE {name} {label}", f"{'uid':>6}  {'from':<9} {'pattern there':<34} then"]
    seen, chains = set(), 0
    for k, (p, ents, order) in enumerate(passes):
        if p in ALLOCATED:
            break
        for u in order:
            e = ents[u]
            if u in seen or e.kind == "note" or reg not in e.regs:
                continue
            rows, said = fate(passes[k:], u)
            seen.update(x for _, x, _, ev in rows if ev != "deleted")
            chains += 1
            out.append(f"{u:>6}  {p:<9} {e.text:<34} {said}")
    if not chains:
        out.append(f"  (no insn mentions r{reg})")
    return "\n".join(out)


def dispositions(greg_sec):
    """{pseudo: hard reg} from a greg dump section's "Register dispositions"."""
    part = greg_sec.split("Register dispositions:")[-1].split(";; Hard regs used")[0]
    return {int(a): int(b) for a, b in re.findall(r"(\d+) in (\d+)", part)}


# ---------------------------------------------------------------- C declarations -> pseudos

Decl = namedtuple("Decl", "name param memory maybe")
SCALARS = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "int", "char", "short", "long",
           "unsigned", "signed", "float", "double", "void", "size_t"}
NOT_DECL = {"return", "if", "else", "while", "for", "do", "switch", "case", "default", "goto",
            "break", "continue", "sizeof"}
WORD = re.compile(r"[A-Za-z_]\w*|\S")


def strip_c(text):
    """TEXT without comments and string/char literals (kept as spaces, so offsets stay)."""
    return re.sub(r'/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                  lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)


def split_top(s, sep):
    """S split at SEP characters outside brackets."""
    out, depth, cur = [], 0, ""
    for ch in s:
        depth += ch in "([{"
        depth -= ch in ")]}"
        if ch == sep and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    return out + [cur]


def ident(w):
    return re.match(r"[A-Za-z_]", w) is not None


def declarator(d):
    """(name, is_pointer, is_array, type words before it) of one declarator, its initializer cut
    off: `(*name)` for a function pointer, else the last identifier before any `[`."""
    toks = WORD.findall(split_top(d, "=")[0])
    fp = next((k + 2 for k in range(len(toks) - 2)
               if toks[k] == "(" and toks[k + 1] == "*" and ident(toks[k + 2])), None)
    if fp is None:
        br = toks.index("[") if "[" in toks else len(toks)
        cand = [k for k in range(br) if ident(toks[k]) and toks[k] not in ("const", "volatile")]
        if not cand:
            return None
        fp = cand[-1]
    return (toks[fp], "*" in toks[:fp], fp + 1 < len(toks) and toks[fp + 1] == "[",
            [w for w in toks[:fp] if ident(w)])


def decl_stmt(stmt):
    """[(name, type words, is_pointer, is_array)] when STMT (without its ;) declares, else None."""
    toks = WORD.findall(stmt)
    if len(toks) < 2 or not ident(toks[0]) or toks[0] in NOT_DECL:
        return None
    # `type (*name)(...)` declares a function pointer; `f(*p)` is a call
    fptr = toks[1:3] == ["(", "*"] and len(toks) > 5 and ident(toks[3]) and toks[4:6] == [")", "("]
    if not (ident(toks[1]) or toks[1] == "*" or fptr):
        return None
    parts = split_top(stmt, ",")
    first = declarator(parts[0])
    if first is None or not first[3]:
        return None
    out = [(first[0], first[3], first[1], first[2])]
    for d in parts[1:]:
        r = declarator(d)
        if r is None:
            return None
        out.append((r[0], first[3], r[1], r[2]))
    return out


def decls(ftext):
    """The parameters and locals of the function definition FTEXT, in declaration order."""
    t = strip_c(ftext)
    brace = t.index("{")
    head, body = t[:brace], t[brace:]
    sig = head[head.index("(") + 1:head.rindex(")")] if "(" in head else ""
    out = []
    knr = head[head.rindex(")") + 1:] if ")" in head else ""
    params = [p.strip() for p in split_top(sig, ",") if p.strip() and p.strip() not in ("void", "...")]
    knr_types = {}
    for stmt in knr.split(";"):
        for nm, tw, ptr, arr in decl_stmt(stmt.strip()) or []:
            knr_types[nm] = (tw, ptr, arr)
    for p in params:
        r = (p, *knr_types[p]) if p in knr_types else None
        if r is None:
            ds = decl_stmt(p)
            if not ds:
                continue
            r = ds[0][0], ds[0][1], ds[0][2], ds[0][3]
        nm, tw, ptr, arr = r
        out.append(Decl(nm, True, addr_taken(body, nm) or "volatile" in tw, maybe(tw, ptr)))
    # locals: the declaration statements at the start of each block, in source order
    depth, i, at_start = 0, 0, []
    while i < len(body):
        ch = body[i]
        if ch == "{":
            depth += 1
            at_start.append(True)
            i += 1
            continue
        if ch == "}":
            depth -= 1
            at_start.pop() if at_start else None
            i += 1
            continue
        if ch.isspace():
            i += 1
            continue
        end = stmt_end(body, i)
        stmt = body[i:end]
        if not stmt.strip():                    # a lone ; (after an initializer's braces)
            i = end + 1
            continue
        ds = decl_stmt(stmt) if at_start and at_start[-1] else None
        if ds:
            for nm, tw, ptr, arr in ds:
                if "static" in tw or "extern" in tw or "typedef" in tw:
                    continue
                if not used(body, nm, i, end):
                    continue
                out.append(Decl(nm, False, arr or "volatile" in tw or addr_taken(body, nm), maybe(tw, ptr)))
        elif at_start:
            at_start[-1] = False
        i = end + 1 if end < len(body) and body[end] == ";" else end
    return out


def stmt_end(body, i):
    """Where the statement at I ends: its ; at paren depth 0, or the next { or } (a block)."""
    depth = 0
    for j in range(i, len(body)):
        ch = body[j]
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif depth == 0 and ch in ";{}":
            return j
    return len(body)


def addr_taken(body, nm):
    """Is NM's own address taken (`&x`, `&s.f`)? `&p->f` and `&p[i]` take the address of what a
    pointer points at, not of the pointer, so they don't count."""
    return re.search(rf"(?<![&\w])&\s*{re.escape(nm)}\b(?!\s*(?:\(|->|\[))", body) is not None


def used(body, nm, start, end):
    return re.search(rf"\b{re.escape(nm)}\b", body[:start] + body[end:]) is not None


def maybe(tw, ptr):
    """A value of a struct or typedef'd type: in a pseudo only when its mode isn't BLKmode."""
    return not ptr and not (set(tw) & SCALARS)


def map_vars(ds, user):
    """{name: pseudo} pairing the register declarations DS with the reg/v pseudos USER (sorted),
    or None when the counts don't fit either reading of the doubtful (maybe) declarations."""
    regs = [d for d in ds if not d.memory]
    for pick in (regs, [d for d in regs if not d.maybe]):
        if len(pick) == len(user):
            return {d.name: r for d, r in zip(pick, user)}
    return None
