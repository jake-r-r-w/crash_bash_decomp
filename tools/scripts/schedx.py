"""Explain cc1's register allocation and instruction scheduling for one function (decomp.py sched).

Reads the dumps cc1 writes with -dl -dg -dS -dR (.lreg, .greg, .sched, .sched2) and prints:

- global allocation: each pseudo global.c allocates, in its order, with refs, live length,
  priority (floor_log2(refs) * refs / live, global.c's allocno_priority without its constant
  factor), the hard register it got and the C variables cc1 -g homes there;
- each scheduling pass, block by block in final order, headed by the block's count of
  memory-unit insns (loads and stores, which turn schedule_select's potential-hazard tie-break
  on at 2+): every insn with the step that placed it and the rule that chose it from the ready
  list (sched.c). gcc 2.x schedules each block backwards from its end, so the first steps place
  the last insns. The rules, in the order they apply:
  - the block end: a trailing call or jump keeps TAIL_PRIORITY and goes first;
  - priority (rank_for_schedule): the longer path to the block end; "birthing" is the
    0x7f000001 that adjust_priority gives an insn setting a single-set register (sched1 only);
  - class (rank_for_schedule): an insn the last scheduled insn uses with a latency over 1 waits;
  - potential hazard (schedule_select): among equal priorities the first insn with the largest
    potential_hazard goes ("has a greater potential hazard" in the dump when that overrides the
    rank order). The memory unit is unit 0, so its cost is 0 unless the block has 2+ load/store
    insns; then a load or store beats an ALU insn, and a store beats a load (blockage range up
    to 3 against 2: genattrtab takes it over every candidate op, both cpus' loads included).
    Mult/div insns (unit 1) always beat an ALU insn; against memory insns it isn't modelled;
  - luid (rank_for_schedule): the later insn in the RTL stays later.
  sched.c only re-sorts the ready list when insns join it, so a pair already in the list keeps
  the order of the step that last compared them ("order from T-n").

The dump prints the ready lists and the hazard picks; the deciding rule between two insns is
worked out from the priorities, the insns' RTL and LOG_LINKS in the dump, and their order in the
block's priority table (luid). MATCHING.md, Reading the compiler's rules, has the method."""
import re
from collections import namedtuple

FUNC = re.compile(r"^;; Function (\S+)", re.M)
HARD = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp",
        "fp", "ra"]
LAUNCH = 0x7f000000         # LAUNCH_PRIORITY is 0x7f000001: a birthing insn gets it (or more)
TAIL = 0x7fff0000           # TAIL_PRIORITY (0x7ffffffe) - i: the insns kept at the block end



def section(text, name):
    """The part of a dump for function NAME."""
    starts = [(m.start(), m.group(1)) for m in FUNC.finditer(text)]
    for k, (pos, fn) in enumerate(starts):
        if fn == name:
            return text[pos:starts[k + 1][0] if k + 1 < len(starts) else len(text)]
    return ""


def floor_log2(n):
    return n.bit_length() - 1 if n > 0 else -1


def allocation(lreg, greg):
    """[(pseudo, refs, live, priority, hard reg)] in global.c's allocation order."""
    use = {int(m.group(1)): (int(m.group(2)), int(m.group(3))) for m in re.finditer(
        r"^Register (\d+) used (\d+) times across (\d+) insns", lreg, re.M)}
    order = re.search(r"^;; \d+ regs to allocate:([\d ]+)", greg, re.M)
    disp = {int(a): int(b) for a, b in
            re.findall(r"(\d+) in (\d+)", greg.split("Register dispositions:")[-1])}
    out = []
    for p in (int(x) for x in order.group(1).split()) if order else []:
        refs, live = use.get(p, (0, 1))
        out.append((p, refs, live, floor_log2(refs) * refs / max(live, 1), disp.get(p)))
    return out


# ---------------------------------------------------------------- RTL patterns as one short line

TOKEN = re.compile(r'\(|\)|\[|\]|"[^"]*"|[^\s()\[\]"]+')
BIN = {"plus": "+", "minus": "-", "mult": "*", "div": "/", "ashift": "<<", "ashiftrt": ">>",
       "lshiftrt": ">>>", "and": "&", "ior": "|", "xor": "^"}
UNARY = {"subreg": "", "sign_extend": "sext", "zero_extend": "zext", "truncate": "trunc", "neg": "-",
         "not": "~"}


def parse(tokens, k=0):
    """One s-expression from TOKENS at K: (node, next k). A node is a str or a list."""
    t = tokens[k]
    if t in ("(", "["):
        close = ")" if t == "(" else "]"
        out, k = [], k + 1
        while tokens[k] != close:
            node, k = parse(tokens, k)
            out.append(node)
        return out, k + 1
    return t, k + 1


def show(n):
    if isinstance(n, str):
        return n.strip('"')
    if not n or not isinstance(n[0], str):
        return show(n[0]) if n else "()"
    head, args = n[0], n[1:]
    if head.startswith('"'):                    # symbol_ref's ("NAME")
        return head.strip('"')
    op = head.split(":")[0].split("/")[0]
    mode = head.split(":")[1] if ":" in head else ""
    if op == "reg":
        return f"${args[1].lstrip('$')}" if len(args) > 1 and isinstance(args[1], str) \
            and not args[1].isdigit() else f"r{args[0]}"
    if op == "const_int":
        return args[0]
    if op == "symbol_ref":
        return show(args[0])
    if op == "mem":
        return f"{mode}[{show(args[0])}]"
    if op == "set":
        return f"{show(args[0])} = {show(args[1])}"
    if op in BIN and len(args) == 2:
        return f"{show(args[0])}{BIN[op]}{show(args[1])}"
    if op == "high":
        return f"%hi({show(args[0])})"
    if op == "lo_sum":
        return f"{show(args[0])}+%lo({show(args[1])})"
    if op in UNARY and args:
        return f"{UNARY[op]}({show(args[0])})"
    if op == "parallel" and args:
        return show(args[0])
    return f"{op}({', '.join(show(x) for x in args[:3])})"


# ---------------------------------------------------------------- insns: pattern, links, unit

Insn = namedtuple("Insn", "pat code links unit")   # code: the insn code (-1: unrecognized)
INSN_START = re.compile(r"^\((?:insn|call_insn|jump_insn)\S* (\d+) \d+ \d+ ", re.M)
RTX_START = re.compile(r"^\((?:insn|call_insn|jump_insn|note|code_label|barrier)\b", re.M)
MULDIV = {"mult", "div", "udiv", "mod", "umod"}
LATENCY = {"load": 2, "mul": 12}     # result_ready_cost on the r3000 (div: 35); the rest are 1


def code_of(n):
    return n[0].split(":")[0].split("/")[0] if isinstance(n, list) and n and isinstance(n[0], str) else None


def kids(n):
    return [] if isinstance(n, str) else n[1:] if code_of(n) else n


def codes(n):
    """Every rtx code in node N."""
    c = code_of(n)
    return ({c} if c else set()).union(*(codes(x) for x in kids(n)))


def first_set(n):
    """A pattern's set, or a parallel's first one."""
    if code_of(n) == "set":
        return n
    if code_of(n) not in (None, "parallel"):
        return None
    return next((s for s in map(first_set, kids(n)) if s), None)


def hilo(n):
    if code_of(n) == "reg" and len(n) > 2 and n[2] in ("hi", "lo"):
        return True
    return any(hilo(x) for x in kids(n))


def unit_of(pat, code):
    """The function unit an insn uses, by mips.md's insn types: "load" or "store" (the memory
    unit: a mem source or destination), "mul" (mult, div) or "hilo" (moves to and from hi and lo),
    both on the imuldiv unit; None for the rest. An unrecognized insn (asm, use) uses none."""
    s = first_set(pat)
    c = codes(pat)
    if code < 0 or s is None or len(s) < 3 or c & {"call", "asm_operands"}:
        return None
    if code_of(s[1]) == "mem":
        return "store"
    if "mem" in codes(s[2]):
        return "load"
    return "mul" if c & MULDIV else "hilo" if hilo(pat) else None


def insns(sec):
    """{uid: Insn} for each insn a dump prints: its pattern as one short line, insn code,
    LOG_LINKS as [(kind, uid)] ("data", "REG_DEP_ANTI", "REG_DEP_OUTPUT") and function unit."""
    out = {}
    for m in INSN_START.finditer(sec):
        uid, end = int(m.group(1)), RTX_START.search(sec, m.end())
        try:
            node = parse(TOKEN.findall(sec[m.start():end.start() if end else len(sec)]))[0]
        except (IndexError, ValueError):
            out[uid] = Insn("?", -1, [], None)
            continue
        rest = node[5:]
        code = int(rest[0]) if rest and isinstance(rest[0], str) and re.fullmatch(r"-?\d+", rest[0]) else -1
        rest = [x for x in rest[1:] if not (isinstance(x, str) and x.startswith("{"))]
        links, ll = [], rest[0] if rest else None
        while code_of(ll) == "insn_list" and len(ll) > 1:
            links.append((ll[0].partition(":")[2] or "data", int(ll[1])))
            ll = ll[2] if len(ll) > 2 else None
        out[uid] = Insn(show(node[4])[:64], code, links, unit_of(node[4], code))
    return out


# ---------------------------------------------------------------- the scheduler's trace

BLOCK = re.compile(r";;\s+-- basic block number (\d+) from \d+ to \d+ --(.*?)(?=;;\s+-- basic block|\Z)", re.S)
Step = namedtuple("Step", "t raw new now pre hazard blocked stalls")
STEP_END = (";; launching", ";; ready list at", ";; total time")


def steps(trace):
    """The `ready list at T-n` steps of one block's trace, as Steps: the clock; the ready list as
    printed ([(uid, priority)], unsorted: the insns left from the last step, then those that
    joined); the uids that joined; the sorted list it scheduled now[0] from (None when every insn
    was blocked); the sorted list before a potential-hazard pick and that pick; the insns
    schedule_select queued as blocked; stall cycles before the step."""
    init = re.search(r";; ready list initially:\n;; ([\d ]*)", trace)
    left = [int(x) for x in init.group(1).split()] if init else []   # sorted, none scheduled yet
    out, cur, stalls = [], None, 0
    for line in trace.splitlines() + [";; total time"]:
        if cur is not None and line.startswith(STEP_END):
            raw = [(int(u), int(p, 16))           # sched.c prints them with %x
                   for u, p in re.findall(r"(\d+) \(([0-9a-f]+)\)", cur[0].split(":", 1)[1])]
            text = "\n".join(cur)
            nows = [[int(x) for x in g.split()] for g in re.findall(r", now((?: \d+)*)", text)]
            hz = re.search(r"insn (\d+) has a greater potential hazard", text)
            # the first len(left) are the insns left from the last step (SCHED_SORT's OLD_READY)
            old = len(left) if [u for u, _ in raw[:len(left)]] == left else 0
            now = nows[-1] if nows else None
            out.append(Step(int(re.search(r"T-(\d+)", cur[0]).group(1)), raw, [u for u, _ in raw[old:]],
                            now, nows[0] if hz and len(nows) > 1 else None, int(hz.group(1)) if hz else None,
                            [int(u) for u in re.findall(r"blocking insn (\d+)", text)], stalls))
            left, cur, stalls = (now[1:] if now else []), None, 0
        if line.startswith(";; launching"):
            m = re.search(r"with (\d+) stalls", line)
            stalls = max(stalls, int(m.group(1)) if m else 0)
        elif line.startswith(";; ready list at"):
            cur = [line]
        elif cur is not None:
            cur.append(line)
    return out


def klass(x, last, rtl):
    """rank_for_schedule's class of X against LAST, the insn scheduled just before: 1 when LAST
    uses X's result with a latency over 1 (X a load or mult), else 3. MIPS's ADJUST_COST makes
    anti and output dependences cost 1, so class 2 never happens."""
    i = rtl.get(last)
    if i is None or i.code < 0 or x not in rtl or rtl[x].unit not in LATENCY:
        return 3
    return 1 if ("data", x) in i.links else 3


def potential_hazard(u, rtl, n_mem):
    """sched.c's potential_hazard for insn U up to a common factor: (minb * 0x40 + maxb) *
    (n_mem - 1) for a load (blockage 1-2) or store (1-3), 0 for an insn on no unit, None (not
    modelled) for the mult/div unit's. Checked against every tie group of five units' dumps."""
    kind = rtl[u].unit if u in rtl else None
    if kind in ("mul", "hilo"):
        return None
    return {"load": 0x42, "store": 0x43}.get(kind, 0) * max(n_mem - 1, 0)


def hazard_order(a, b, rtl, n_mem):
    """1 when A's potential hazard is greater than B's, 0 when equal, -1 when less, None when it
    isn't modelled (a mult/div insn against a load or store, or against another mult/div)."""
    ha, hb = potential_hazard(a, rtl, n_mem), potential_hazard(b, rtl, n_mem)
    if ha is None and hb is None:
        return None
    if ha is None:                           # mult/div beats an insn on no unit
        return 1 if hb == 0 else None
    if hb is None:
        return -1 if ha == 0 else None
    return (ha > hb) - (ha < hb)


def fmt(p):
    return f"{p:x}" if p >= LAUNCH else str(p)


def plural(n, word):
    return f"{n} {word}" + ("" if n == 1 else "s")


def schedule(sec):
    """[(block, counts, [(uid, T, why)])] in final (forward) order for one pass's dump. COUNTS:
    {"insns", "mem", "mul"}, the block's insns, memory-unit insns (loads and stores: the
    potential hazard needs 2+) and mult/div-unit insns."""
    rtl = insns(sec)
    blocks = []
    for bm in BLOCK.finditer(sec):
        trace = bm.group(2)
        # the priority table is in RTL order: the luid order rank_for_schedule ties on
        table = [(int(a), int(b)) for a, b in re.findall(r";; insn\[\s*(\d+)\]: priority =\s*(-?\d+)", trace)]
        prio0, luid = dict(table), {u: k for k, (u, _) in enumerate(table)}
        units = [rtl[u].unit for u in prio0 if u in rtl]
        info = {"insns": len(table), "mem": sum(x in ("load", "store") for x in units),
                "mul": sum(x in ("mul", "hilo") for x in units)}
        st = steps(trace)
        lasts, last = [], None           # last_scheduled_insn at each step
        for s in st:
            lasts.append(last)
            last = s.now[0] if s.now else last
        placed = [(s.now[0], s.t, why(st, k, lasts, rtl, prio0, luid, info)) for k, s in enumerate(st) if s.now]
        placed.sort(key=lambda x: -x[1])             # final order: the last step placed it first
        blocks.append((int(bm.group(1)), info, placed))
    return blocks


def why(st, k, lasts, rtl, prio0, luid, info):
    """The rule that put step K's pick (now[0]) ahead of the rest of the ready list."""
    s = st[k]
    c, pr = s.now[0], dict(s.raw)
    note = (f"; {', '.join(map(str, s.blocked))} blocked (unit busy)" if s.blocked else "") \
        + (f"; after {plural(s.stalls, 'stall')}" if s.stalls else "")
    if pr.get(c, 0) >= TAIL:
        return "block end (tail priority)" + note
    unit = {u: (rtl[u].unit if u in rtl else None) or "ALU" for u in s.now + (s.pre or [])}
    if s.hazard == c and s.pre and s.pre[0] != c:
        return (f"greater potential hazard than {s.pre[0]}, the rank order's pick ({unit[c]} over "
                f"{unit[s.pre[0]]}; {plural(info['mem'], 'memory insn')} in the block)" + note)
    if len(s.now) == 1:
        return "only insn ready" + note
    r = s.now[1]
    pc, prr = pr.get(c, 0), pr.get(r, 0)
    if pc != prr:
        if pc >= LAUNCH and prio0.get(c, 0) < LAUNCH:
            return f"birthing priority {fmt(pc)} > {fmt(prr)} of {r}" + note
        return f"priority {fmt(pc)} > {fmt(prr)} of {r}" + note
    # SCHED_SORT re-sorts only when insns join the list: find the step that last compared c and r
    d = k
    while d > 0 and len(st[d].new) < 2 and c not in st[d].new and r not in st[d].new:
        d -= 1
    initial = len(st[d].new) < 2 and c not in st[d].new and r not in st[d].new
    when = " (order from the initial sort)" if initial else f" (order from T-{st[d].t})" if d != k else ""
    last = None if initial else lasts[d]
    later = luid.get(c, c) > luid.get(r, r)
    if klass(c, last, rtl) != klass(r, last, rtl):
        return (f"class: {last} uses {r}'s result, latency {LATENCY[rtl[r].unit]} (class 1 < 3; luid "
                f"{'agrees' if later else f'alone would take {r}'})" + when + note)
    # schedule_select kept the rank order's pick: its potential hazard is the largest of the tie
    order = hazard_order(c, r, rtl, info["mem"])
    if order is None:
        hz = f"potential hazard of {unit[c]} against {unit[r]} not modelled"
    elif order > 0:
        hz = f"its potential hazard is greater too: {unit[c]} over {unit[r]}"
    elif order < 0:
        hz = f"but {r}'s potential hazard should be greater: {unit[r]} over {unit[c]}?"
    elif potential_hazard(c, rtl, info["mem"]):
        hz = f"equal potential hazard: {unit[c]} and {unit[r]}"
    elif info["mem"] < 2:
        hz = f"{'only 1 memory insn' if info['mem'] else 'no memory insn'} in the block: no potential hazard"
    else:
        hz = "neither is a load or store: no potential hazard"
    if later:
        return f"tie with {r} at priority {fmt(pc)}: luid, {c} is later in the RTL so it stays after ({hz})" \
            + when + note
    return f"tie with {r} at priority {fmt(pc)}: rule unclear (luid would take {r}; {hz})" + when + note


UNIT_TAG = {"load": "ld", "store": "st", "mul": "mul", "hilo": "hilo"}


def report(dumps, name, homes, only_pass=None, only_block=None, only_insn=None):
    """The text decomp.py sched prints. DUMPS: {"lreg", "greg", "sched", "sched2": text}."""
    lines = []
    lreg, greg = section(dumps.get("lreg", ""), name), section(dumps.get("greg", ""), name)
    by_reg = {}
    for v, h in homes.items():
        by_reg.setdefault(h, []).append(v)
    if not only_insn:
        lines.append("global allocation, in order (priority = floor_log2(refs) * refs / live):")
        for p, refs, live, pri, hard in allocation(lreg, greg):
            hn = HARD[hard] if hard is not None and hard < 32 else str(hard)
            vs = ", ".join(sorted(by_reg.get(hn, []))) or "?"
            lines.append(f"  r{p:<4} {refs:>3} refs / {live:>4} insns = {pri:.3f}  -> {hn:<3} ({vs})")
    for key, title in (("sched", "sched1 (pseudos, before allocation)"), ("sched2", "sched2 (after reload)")):
        if only_pass and key != only_pass:
            continue
        sec = section(dumps.get(key, ""), name)
        if not sec:
            continue
        rtl = insns(sec)
        lines.append(f"{title}, each block in final order (ld/st: memory unit, mul/hilo: mult/div unit):")
        for b, info, order in schedule(sec):
            if only_block is not None and b != only_block:
                continue
            shown = [x for x in order if only_insn is None or x[0] in only_insn]
            if not shown:
                continue
            mem = plural(info["mem"], "memory insn") + (" (potential hazard on)" if info["mem"] >= 2 else "")
            mul = f", {info['mul']} mult/div" if info["mul"] else ""
            lines.append(f"  block {b}: {plural(info['insns'], 'insn')}, {mem}{mul}")
            for uid, t, reason in shown:
                i = rtl.get(uid)
                lines.append(f"    {uid:>5} T-{t:<3} {i.pat if i else '?':<64} "
                             f"{UNIT_TAG.get(i.unit, '') if i else '':<4} {reason}")
    return "\n".join(lines)

