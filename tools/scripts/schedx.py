"""Explain cc1's register allocation and instruction scheduling for one function (decomp.py sched).

Reads the dumps cc1 writes with -dl -dg -dS -dR (.lreg, .greg, .sched, .sched2) and prints:

- global allocation: each pseudo global.c allocates, in its order, with refs, live length,
  priority (floor_log2(refs) * refs / live, global.c's allocno_priority without its constant
  factor), the hard register it got and the C variables cc1 -g homes there;
- each scheduling pass, block by block in final order: every insn with the step that placed it
  and why, by rank_for_schedule's rules (sched.c): a higher priority, the only insn ready (the
  rest blocked on latency), or a tie broken by source order (INSN_LUID) or by its dependence on
  the insn scheduled just before. gcc 2.x schedules each block backwards from its end, so the
  first steps place the last insns. "launch" is LAUNCH_PRIORITY: an insn that just became ready
  and makes a register live (adjust_priority).

The tie-break attribution is inferred from the dump (which doesn't print the deciding rule):
a tie the later insn won is labelled source order, otherwise dependence class. MATCHING.md,
Reading the compiler's rules, has the method this automates."""
import re

FUNC = re.compile(r"^;; Function (\S+)", re.M)
HARD = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
        "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp",
        "fp", "ra"]
LAUNCH = 0x7f000000


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


def insns(sec):
    """{uid: short pattern}."""
    out = {}
    for m in re.finditer(r"^\((?:insn|call_insn|jump_insn) (\d+) \d+ \d+ ", sec, re.M):
        try:
            out[int(m.group(1))] = show(parse(TOKEN.findall(sec[m.end():m.end() + 4000]))[0])[:64]
        except (IndexError, ValueError):
            out[int(m.group(1))] = "?"
    return out


# ---------------------------------------------------------------- the scheduler's trace

def schedule(sec):
    """[(block, [(uid, T, why)])] in final (forward) order for one pass's trace."""
    blocks = []
    for bm in re.finditer(r";;\s+-- basic block number (\d+) from \d+ to \d+ --(.*?)(?=;;\s+-- basic block|\Z)",
                          sec, re.S):
        trace = bm.group(2)
        prio = {int(a): int(b) for a, b in re.findall(r";; insn\[\s*(\d+)\]: priority =\s*(-?\d+)", trace)}
        last_seen, last = {}, 0          # uid -> its last step: once scheduled it leaves the list
        for m in re.finditer(r";; ready list at T-(\d+): (.*?), now ([\d ]+)", trace):
            t, step = int(m.group(1)), trace[last:m.start()]
            last = m.end()
            pr = {int(u): int(p, 16) if p.startswith("7f") else int(p)
                  for u, p in re.findall(r"(\d+) \(([0-9a-f]+)\)", m.group(2))}
            now = [int(x) for x in m.group(3).split()]
            blocked = re.findall(r"blocking insn (\d+)", step)
            for u in now:
                last_seen[u] = (t, now, pr, blocked)
        placed = []
        for u, (t, now, pr, blocked) in last_seen.items():
            p_of = lambda x: pr.get(x, prio.get(x, 0))  # noqa: E731
            others = [x for x in now if x != u]
            if now[0] != u:
                why = f"after {now[0]} in the list, taken when {now[0]} couldn't issue (unit hazard)"
            elif not others:
                why = "only insn ready" + (f" (blocked on latency: {', '.join(blocked)})" if blocked else "")
            else:
                best = max(others, key=p_of)
                if p_of(u) != p_of(best):
                    why = f"priority {fmt(p_of(u))} > {fmt(p_of(best))} of {best}"
                elif u > best:
                    why = f"tie with {best}: source order (the later insn goes first)"
                else:
                    why = f"tie with {best}: dependence on the insn placed after it"
            placed.append((u, t, why))
        placed.sort(key=lambda x: -x[1])             # final order: the last step placed it first
        blocks.append((int(bm.group(1)), placed))
    return blocks


def fmt(p):
    return "launch" if p >= LAUNCH else str(p)


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
        pats = insns(sec)
        lines.append(f"{title}, each block in final order:")
        for b, order in schedule(sec):
            if only_block is not None and b != only_block:
                continue
            shown = [x for x in order if only_insn is None or x[0] in only_insn]
            if not shown:
                continue
            lines.append(f"  block {b}:")
            for uid, t, why in shown:
                lines.append(f"    {uid:>5} T-{t:<3} {pats.get(uid, '?'):<64} {why}")
    return "\n".join(lines)
