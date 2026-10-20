"""Make maspsx output look like Sony's ASPSX for gcc 2.7.2 units (stdin -> stdout).

1. Fill j/jr delay slots the way ASPSX does.

gcc 2.7.2 leaves `j` (and the `j $31` return) in reorder mode for the assembler to fill.
maspsx puts a nop there; ASPSX moves the instruction before the jump into the slot, and for
a store to a symbol (a two-instruction macro) it moves the second half:

    sw  $2,D_8006EA24          lui $at,%hi(D_8006EA24)
    j   $L1             ->     j   $L1
    nop                        sw  $2,%lo(D_8006EA24)($at)

2. Expand an indexed load from a symbol through the load's own register, not $at. ASPSX (like
GNU as) builds the address in the destination unless it is also the index register; maspsx
always uses $at:

    lbu $2,D_8006789E($3)      lui  $2,%hi(D_8006789E)       (maspsx: lui $at; addu $at,$at,$3;
                        ->     addu $2,$2,$3                  lbu $2,%lo(D_8006789E)($at))
                               lbu  $2,%lo(D_8006789E)($2)

3. Fill the slots of conditional branches gcc left to the assembler (fill_branches): from below,
else from the target (its first instruction, or the slot of a `j` there), with an instruction
whose register is dead on the other path; a target instruction every branch took and nothing
else reaches is dropped. Derived from func_8002D4F4 against the retail code and checked on every
matched function of the two units.

4. Not ASPSX but gcc (refill_from_jump, run first): Sony's gcc 2.7.2 fills a `beq` followed by a
`j` from the `j`'s slot, where FSF 2.7.2's reorg (our cc1) takes the `beq` target's first
instruction. Redone for the one shape func_8002D4F4 shows.

Units built with gcc 2.7.2 use all of it (Makefile: ASPSX_FILL). gcc 2.8.1 units use step 1
without stores (--jumps-only, the Makefile's default). 2.8.1's reorg fills nearly every slot
itself; of the ten `j` slots it leaves to the assembler in the matched 2.8.1 code (2026-10-19),
nine follow a store (sw, swr, swc2) and keep their nop in retail, and one is the text epilogue's
`j $31` in a function with a frame but no saved registers (mips_epilogue_delay_slots gives it no
delay slot), filled in retail with the stack restore before it (func_800C0F5C:
`addiu sp,-24; jr ra; addiu sp,24`). maspsx writes a nop in all ten.

Despite the name, these fills are not ASPSX's: the real ASPSX (2.56-2.86, run on the same cc1 text
by tools/scripts/aspsx_compare.py, 2026-10-19) fills none of them, in the 2.8.1 epilogue or in the
2.7.2 units. They come from the game's own compiler builds; this script puts retail's bytes back
after maspsx all the same (docs/ARCHITECTURE.md, Toolchain)."""
import re
import sys

BRANCHES = {"j", "jal", "jr", "jalr", "b", "bal", "beq", "bne", "beqz", "bnez", "blez", "bgtz",
            "bltz", "bgez", "bltzal", "bgezal", "beql", "bnel"}
NO_MOVE = BRANCHES | {"nop", "mult", "multu", "div", "divu", "mflo", "mfhi", "mtlo", "mthi", "la",
                      "lw", "lh", "lhu", "lb", "lbu", "lwl", "lwr", "lwc2", "break", "syscall"}
STORES = {"sw", "sh", "sb"}
MEM_WRITES = STORES | {"swl", "swr", "swc2"}
INSN = re.compile(r"^\s*([a-z][a-z0-9.]*)\s*(.*?)\s*(#.*)?$")


def insn(line):
    s = line.strip()
    if not s or s.startswith("#") or s.startswith(".") or s.endswith(":") or ";" in s:
        return None
    m = INSN.match(line)
    return (m.group(1), m.group(2)) if m else None


def regs_written(op, args):
    if op in STORES or op in BRANCHES:
        return set()
    return {args.split(",")[0].strip()}


def fits16(v):
    try:
        n = int(v, 0)
    except ValueError:
        return False
    return -0x8000 <= n <= 0xFFFF


LOADS = {"lw", "lh", "lhu", "lb", "lbu"}
LUI_AT = re.compile(r"^\s*lui\s+\$at,\s*%hi\(([^)]+)\)\s*$")
ADDU_AT = re.compile(r"^\s*addu\s+\$at,\s*\$at,\s*(\$\w+)\s*$")
LOAD_AT = re.compile(r"^\s*(\w+)\s+(\$\w+),\s*%lo\(([^)]+)\)\(\$at\)\s*$")


def load_via_dest(lines):
    """Rewrite `lui $at; addu $at,$at,rI; lX rD,%lo(sym)($at)` to use rD in place of $at."""
    out = list(lines)
    for i in range(len(out) - 2):
        a, b, c = LUI_AT.match(out[i]), ADDU_AT.match(out[i + 1]), LOAD_AT.match(out[i + 2])
        if not (a and b and c) or c.group(1) not in LOADS or c.group(3) != a.group(1):
            continue
        rd, ri = c.group(2), b.group(1)
        if rd in (ri, "$0", "$zero"):
            continue
        out[i] = f"lui\t{rd},%hi({a.group(1)})"
        out[i + 1] = f"addu\t{rd},{rd},{ri}"
        out[i + 2] = f"{c.group(1)}\t{rd},%lo({a.group(1)})({rd})"
    return out


def fill(lines, stores=True):
    """Step 1. STORES=False (gcc 2.8.1 units): leave a slot whose candidate writes memory."""
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        ins = insn(line)
        nxt = lines[i + 1].strip() if i + 1 < len(lines) else ""
        if ins and ins[0] in ("j", "jr") and nxt.startswith("nop") and "branch/jump" in nxt:
            # the previous line must be a plain instruction (no label or directive between)
            p = len(out) - 1
            while p >= 0 and (not out[p].strip() or out[p].strip().startswith("#")):
                p -= 1
            prev = insn(out[p]) if p >= 0 else None
            q = p - 1
            while q >= 0 and (not out[q].strip() or out[q].strip().startswith("#")):
                q -= 1
            before = insn(out[q]) if q >= 0 else None
            target = ins[1].strip()
            ok = (prev and prev[0] not in NO_MOVE and not (before and before[0] in BRANCHES)
                  and (stores or prev[0] not in MEM_WRITES)
                  and target not in regs_written(*prev)
                  and not (prev[0] == "li" and not fits16(prev[1].split(",")[1].strip())))
            if ok:
                op, args = prev
                r, mem = [x.strip() for x in args.split(",", 1)]
                if op in STORES and "(" not in mem:
                    out[p] = f".set\tnoat\nlui\t$at,%hi({mem})"
                    out.append(line)
                    out.append(f"{op}\t{r},%lo({mem})($at)\n.set\tat")
                else:
                    moved = out.pop(p)
                    out.append(line)
                    out.append(moved)
                i += 2
                continue
        out.append(line)
        i += 1
    return out


COND_BRANCHES = {"beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez"}
LABEL = re.compile(r"^\s*(\$?[\w.]+):\s*$")
REG = re.compile(r"\$\w+")
# one instruction, writes one register, nothing else: safe to run on a path that didn't need it
SPECULABLE = {"li", "lui", "move", "addu", "addiu", "subu", "and", "andi", "or", "ori", "xor", "xori",
              "nor", "sll", "srl", "sra", "sllv", "srlv", "srav", "slt", "slti", "sltu", "sltiu"}
SCAN = 16                                       # instructions looked at to prove a register dead


def is_slot_nop(line):
    """maspsx's marker for a slot gcc left to the assembler (reorder mode)."""
    s = line.strip()
    return s.startswith("nop") and "branch/jump" in s


def regs(args):
    return [r for r in REG.findall(args) if r not in ("$0", "$zero")]


def reads_writes(op, args):
    rs = regs(args)
    if op in STORES or op in BRANCHES or op in ("mult", "multu", "div", "divu", "mthi", "mtlo"):
        return set(rs), set()
    if not rs:
        return set(), set()
    first = args.split(",")[0].strip()
    if first in ("$0", "$zero"):
        return set(rs), set()
    return set(rs[1:]), {first}


def speculable(ins):
    if not ins or ins[0] not in SPECULABLE:
        return None
    op, args = ins
    ops = [a.strip() for a in args.split(",")]
    if not all(REG.fullmatch(a) or re.fullmatch(r"-?(0x[0-9a-fA-F]+|\d+)", a) for a in ops):
        return None                             # a symbol: a two-instruction macro
    if op == "li" and not fits16(ops[1]):
        return None                             # a big constant: lui + ori
    w = reads_writes(op, args)[1]
    return next(iter(w)) if len(w) == 1 else None


ARGS = {"$4", "$5", "$6", "$7"}
CALLER_SAVED = {f"${n}" for n in list(range(1, 16)) + [24, 25]}


def dead_at(lines, k, reg, labels=None):
    """True if reg is dead at line k: on every path from there it is written before it is read
    (ASPSX's liveness). A call reads $4-$7 and leaves the caller-saved registers dead; a return, an
    indirect jump or anything unclear counts as a read. A load's write doesn't count as one
    (func_8002E25C's `bne $2,$0` is left unfilled though `lbu $2` follows on its fallthrough)."""
    if labels is None:
        labels = {LABEL.match(l).group(1): n for n, l in enumerate(lines) if LABEL.match(l)}
    seen, todo = set(), [k]
    while todo:
        k = todo.pop()
        while True:
            k = first_insn(lines, k)
            if k >= len(lines) or k in seen or len(seen) > 8 * SCAN:
                if k in seen:
                    break
                return False
            seen.add(k)
            if not insn(lines[k]):
                return False                          # the end of the function
            op, args = insn(lines[k])
            r, w = reads_writes(op, args)
            if op in ("jal", "jalr", "bltzal", "bgezal"):
                if reg in ARGS or reg in r:
                    return False
                slot = first_insn(lines, k + 1)       # the slot runs before the call
                sr, sw = reads_writes(*insn(lines[slot]))
                if reg in sr:
                    return False
                if reg in CALLER_SAVED or reg == "$31" or (reg in sw and insn(lines[slot])[0] not in LOADS):
                    break
                k = slot + 1
                continue
            if reg in r:
                return False
            if op in BRANCHES:
                slot = first_insn(lines, k + 1)
                si = insn(lines[slot]) if slot < len(lines) else None
                if not si:
                    return False
                sr, sw = reads_writes(*si)
                if reg in sr:
                    return False
                if reg in sw and si[0] not in LOADS:
                    break
                dest = args.split(",")[-1].strip()
                if op in ("jr", "jalr") or dest not in labels:
                    return False                      # a return or a jump table: assume it's read
                todo.append(labels[dest])
                if op in ("j", "b"):
                    break
                k = slot + 1                          # and the fallthrough
                continue
            if reg in w:
                if op in LOADS:
                    return False
                break
            k += 1
    return True


def first_insn(lines, k):
    """The index of the first instruction at or after line k (past labels and comments), or of the
    function's `.end`."""
    while k < len(lines) and not insn(lines[k]) and not lines[k].strip().startswith(".end"):
        k += 1
    return k


def fill_branches(lines, stolen=None):
    """Fill the slot of a conditional branch gcc 2.7.2 left to the assembler, as ASPSX does: from
    below if it can, else from the target. Either way the moved instruction must be one that can
    run on the path that didn't need it: a single instruction writing one register that is dead
    on that path (written before it is read).

    From below, the next instruction moves into the slot, even past a label (the label then
    points at the slot, so branches to it still run it). func_8002D4F4:

        bgez $16,$L45              bgez $16,$L45
        nop                  ->  $L175:
    $L175:                         li   $2,-2
        li   $2,-2                 beq  $16,$2,$L156

    From the target, its first instruction is copied into the slot and the branch skips it; when
    the target is a `j X` with a filled slot, its slot is copied and the branch goes to X
    (func_8002DE2C's `beq $4,$2` returns 1 straight to the exit):

        beq  $16,$2,$L157          beq  $16,$2,$L157_1
        nop                  ->    li   $2,1
        ...                        ...
    $L157:                     $L157:
        li   $2,1                  li   $2,1
        sw   $2,D_80067894     $L157_1:
                                   sw   $2,D_80067894
    """
    out = list(lines)
    i, made = 0, 0
    while i < len(out) - 1:
        ins = insn(out[i])
        if not ins or ins[0] not in COND_BRANCHES or not is_slot_nop(out[i + 1]):
            i += 1
            continue
        target = [a.strip() for a in ins[1].split(",")][-1]
        f = first_insn(out, i + 2)
        below = speculable(insn(out[f])) if f < len(out) else None
        tl = next((n for n, l in enumerate(out) if LABEL.match(l) and LABEL.match(l).group(1) == target), None)
        if below and all(not out[n].strip() or LABEL.match(out[n]) or out[n].strip().startswith("#")
                         for n in range(i + 2, f)) and tl is not None and dead_at(out, tl + 1, below):
            del out[i + 1]                       # the next instruction becomes the slot
            i += 1
            continue
        if tl is not None:
            k = first_insn(out, tl + 1)
            t = insn(out[k]) if k < len(out) else None
            reg = speculable(t)
            if reg and dead_at(out, i + 2, reg):
                nk = first_insn(out, k + 1)
                lab = next((LABEL.match(out[n]).group(1) for n in range(k + 1, nk) if LABEL.match(out[n])), None)
                if lab is None:
                    made += 1
                    lab = f"{target}_aspsx{made}"
                    out.insert(k + 1, f"{lab}:")
                    if k + 1 <= i:                  # a target above the branch: it moved down
                        i += 1
                out[i] = out[i].rstrip()[: -len(target)] + lab
                out[i + 1] = out[k]
                if stolen is not None:
                    stolen.add(target)
                i += 2
                continue
            if t and t[0] == "j" and not t[1].strip().startswith("$31") and k + 1 < len(out):
                s = insn(out[k + 1])
                reg = speculable(s) if not is_slot_nop(out[k + 1]) else None
                if reg and dead_at(out, i + 2, reg):
                    out[i] = out[i].rstrip()[: -len(target)] + t[1].strip()
                    out[i + 1] = out[k + 1]
                    i += 2
                    continue
        i += 1
    return out


def drop_unreached(lines, stolen):
    """A target whose first instruction every branch took a copy of, and that nothing falls into
    (it follows a jump's slot), loses that instruction: ASPSX moved it, so it is gone from the
    retail code (func_8002D4F4's `$L157: li $2,1`, after 12 `beq`s to it)."""
    out = list(lines)
    for label in stolen:
        tl = next((n for n, l in enumerate(out) if LABEL.match(l) and LABEL.match(l).group(1) == label), None)
        if tl is None or any(re.search(rf"[\s,]{re.escape(label)}\s*$", l) for l in out if not LABEL.match(l)):
            continue                                  # still a branch or a jump table to it
        p = tl - 1
        while p >= 0 and not insn(out[p]) and not LABEL.match(out[p]) and not out[p].strip().startswith("."):
            p -= 1
        q = p - 1
        while q >= 0 and not insn(out[q]) and not LABEL.match(out[q]):
            q -= 1
        before = insn(out[q]) if q >= 0 and p >= 0 and insn(out[p]) else None
        if not before or before[0] not in ("j", "jr", "b"):
            continue                                  # code falls into it
        k = first_insn(out, tl + 1)
        if any(LABEL.match(out[n]) for n in range(tl + 1, k)):
            continue
        del out[k]
        del out[tl]
    return out


def refill_from_jump(lines):
    """Redo one gcc slot fill in the order Sony's gcc 2.7.2 used (not FSF 2.7.2's, which our cc1 has).

    Sony's reorg fills an unconditional `j` from its target before it fills the conditional
    branches; a `beq` (predicted not taken) followed by that `j` then takes the `j`'s slot
    instruction. FSF 2.7.2 fills the `j` later, so the `beq` takes the first instruction of its
    own target instead. 2.7.2-cdk and 2.8.1-psx do it Sony's way. func_8002D4F4 state 6:

        beq  $16,$2,$L62           beq  $16,$2,$L62
        li   $4,3                  li   $16,6
        j    $L13            ->    j    $L13
        li   $16,6                 nop
        ...                        ...
    $L62:                      $L62:
        lbu  $3,D_8006EA20         li   $4,3
                                   lbu  $3,D_8006EA20

    Only when gcc's eager pass did the fill: nothing above the `beq` could fill it (a label or
    another branch's slot is right before it), its target is reached only from it and nothing
    falls into it (so the slot instruction was moved from there), and the `j`'s slot register is
    dead at the target."""
    out = list(lines)
    labels = {LABEL.match(l).group(1): n for n, l in enumerate(out) if LABEL.match(l)}
    i = 0
    while i < len(out) - 1:
        ins = insn(out[i])
        if not ins or ins[0] not in ("beq", "beqz") or is_slot_nop(out[i + 1]) or not insn(out[i + 1]):
            i += 1
            continue
        target = [a.strip() for a in ins[1].split(",")][-1]
        f = first_insn(out, i + 2)
        jmp = insn(out[f]) if f < len(out) else None
        between = range(i + 2, f)
        if (not jmp or jmp[0] != "j" or jmp[1].strip() == "$31" or f + 1 >= len(out)
                or is_slot_nop(out[f + 1]) or not insn(out[f + 1])
                or any(LABEL.match(out[n]) for n in between) or target not in labels):
            i += 1
            continue
        # nothing above could fill the beq: a label, or another branch's slot, right before it
        p = i - 1
        while p >= 0 and not out[p].strip():
            p -= 1
        q = p - 1
        while q >= 0 and not out[q].strip():
            q -= 1
        prev, before = (insn(out[p]) if p >= 0 else None), (insn(out[q]) if q >= 0 else None)
        if not (p >= 0 and LABEL.match(out[p])) and not (prev and before and before[0] in BRANCHES):
            i += 1
            continue
        # the target is the beq's own: one reference, and code doesn't fall into it
        tl = labels[target]
        refs = sum(1 for l in out if not LABEL.match(l) and re.search(rf"[\s,]{re.escape(target)}\s*$", l))
        a = tl - 1
        while a >= 0 and not insn(out[a]):
            a -= 1
        b = a - 1
        while b >= 0 and not insn(out[b]):
            b -= 1
        fallen_into = not (a >= 0 and b >= 0 and insn(out[b])[0] in ("j", "jr", "b"))
        moved, stolen = out[i + 1], out[f + 1]
        k = first_insn(out, tl + 1)
        reg = speculable(insn(stolen))
        if (refs != 1 or fallen_into or not speculable(insn(moved)) or not reg
                or (k < len(out) and out[k].strip() == moved.strip())):
            i += 1
            continue
        new = out[:tl + 1] + [moved] + out[tl + 1:]
        new[i + 1] = stolen
        new[f + 1] = "nop  # DEBUG: branch/jump"
        if not dead_at(new, tl + 1, reg):
            i += 1
            continue
        out = new
        labels = {LABEL.match(l).group(1): n for n, l in enumerate(out) if LABEL.match(l)}
        i += 2
    return out


def fill_all_branches(lines):
    """fill_branches until nothing changes: a slot filled later in the code can prove a register
    dead for an earlier branch (func_8002D4F4's `beq $16,$2,$L157` before a `bgez` filled from
    below). Then drop the instructions every branch took and nothing else reaches."""
    stolen = set()
    while True:
        new = fill_branches(lines, stolen)
        if new == lines:
            return drop_unreached(new, stolen)
        lines = new


def main():
    text = sys.stdin.read().split("\n")
    if "--jumps-only" in sys.argv[1:]:
        sys.stdout.write("\n".join(fill(text, stores=False)))
    else:
        sys.stdout.write("\n".join(fill_all_branches(fill(load_via_dest(refill_from_jump(text))))))


if __name__ == "__main__":
    main()
